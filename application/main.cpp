// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "main_window.h"

#include <vclib/render/application.h>

#include <mop_config.h>           // cmake-generated: MOP_DESKTOP_MIME_TYPES

#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QIcon>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>
#include <qglobal.h>

#ifdef Q_OS_MACOS
#include <QEvent>
#include <QFileOpenEvent>
#endif

// Name of the local socket used for single-instance IPC
static constexpr const char* MOP_SOCKET_NAME = "MeshOp3D-instance";

// Time (ms) to wait for the running instance to acknowledge our message
static constexpr int MOP_SOCKET_TIMEOUT_MS = 1000;

bool sendToRunningInstance(const QStringList& files);

#ifdef Q_OS_LINUX
void tryIntegrateAppImage();
#endif // Q_OS_LINUX

int main(int argc, char** argv)
{
#ifdef Q_OS_LINUX
    // Workaround for Qt dark theme detection on Linux (Issue #155)
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORMTHEME")) {
        qputenv("QT_QPA_PLATFORMTHEME", "xdgdesktopportal");
    }
#endif

    // Collect file arguments before creating QApplication so we can
    // decide whether to forward them to a running instance and exit early.
    QStringList fileArgs;
    for (int i = 1; i < argc; ++i) {
        fileArgs << QString::fromLocal8Bit(argv[i]);
    }

    // If a running instance already exists, forward the files and quit.
    if (sendToRunningInstance(fileArgs)) {
        return 0;
    }

    // ---- No running instance: start normally ----

    // NOTE: setDesktopFileName and setApplicationName must be called AFTER
    // QApplication is constructed. Qt uses applicationName for the X11/Wayland
    // WM_CLASS, which GNOME matches against StartupWMClass in the .desktop file
    // to show the correct dock icon. Calling these before construction has no effect.
    auto app = vcl::Application(argc, argv);

    // Force WM_CLASS = "MeshOp3D" regardless of the process/AppImage filename.
    QGuiApplication::setApplicationName("MeshOp3D");
    // Let GNOME/KDE associate running windows with the installed .desktop entry.
    // NOTE: Qt expects the bare name WITHOUT the .desktop extension.
    QGuiApplication::setDesktopFileName(QStringLiteral("MeshOp3D"));

#ifdef Q_OS_LINUX
    // Auto-register the .desktop entry and icon in ~/.local/share/ so that
    // GNOME (and other XDG desktops) can show the correct dock icon without
    // requiring AppImageLauncher or any manual setup from the user.
    tryIntegrateAppImage();
#endif

    QGuiApplication::setWindowIcon(QIcon(":/assets/mop3d-icon.png"));

    mop::MainWindow mw;

    // --- Single-instance server ---
    // Listen for file paths sent by subsequent launches.
    QLocalServer server;
    // Remove any leftover socket from a crashed previous session.
    QLocalServer::removeServer(MOP_SOCKET_NAME);
    server.listen(MOP_SOCKET_NAME);

    QObject::connect(&server, &QLocalServer::newConnection, [&server, &mw]() {
        QLocalSocket* client = server.nextPendingConnection();
        QObject::connect(client, &QLocalSocket::readyRead, [client, &mw]() {
            QByteArray        data = client->readAll();
            const QStringList paths =
                QString::fromUtf8(data).split('\n', Qt::SkipEmptyParts);
            for (const QString& path : std::as_const(paths)) {
                mw.loadMesh(path.toStdString());
            }
            // Bring window to front
            mw.raise();
            mw.activateWindow();
        });
        QObject::connect(
            client,
            &QLocalSocket::disconnected,
            client,
            &QLocalSocket::deleteLater);
    });

#ifdef Q_OS_MACOS
    // On macOS, the Finder sends files via QFileOpenEvent instead of argv.
    // Install an event filter to intercept these events and forward them to
    // the main window. Must be installed before show() to catch launch-time
    // file opens (e.g. double-clicking a .ply in Finder).
    class FileOpenFilter : public QObject {
    public:
        mop::MainWindow* mw;
        explicit FileOpenFilter(mop::MainWindow* w, QObject* parent = nullptr)
            : QObject(parent), mw(w) {}
        bool eventFilter(QObject*, QEvent* e) override {
            if (e->type() == QEvent::FileOpen) {
                auto* foe = static_cast<QFileOpenEvent*>(e);
                mw->loadMesh(foe->file().toStdString());
                mw->raise();
                mw->activateWindow();
                return true;
            }
            return false;
        }
    };
    // Parent to qApp so the filter is destroyed when the application exits.
    auto* fileOpenFilter = new FileOpenFilter(&mw, qApp);
    qApp->installEventFilter(fileOpenFilter);
#endif

    mw.show();
    mw.showMaximized();

    // Open files passed as command-line arguments / "Open With..."
    // On macOS argv-based file opening is used when MeshOp3D is launched
    // from the terminal; Finder launches use QFileOpenEvent (see above).
    for (const QString& path : std::as_const(fileArgs)) {
        mw.loadMesh(path.toStdString());
    }

    return app.exec();
}

/**
 * @brief Tries to send file paths to an already-running MeshOp3D instance.
 *
 * Each file path is sent as a UTF-8 line terminated by '\n'.
 *
 * @param files  List of absolute file paths to open.
 * @return true  if a running instance was found and the files were forwarded.
 * @return false if no running instance was detected.
 */
bool sendToRunningInstance(const QStringList& files)
{
    if (files.isEmpty())
        return false;

    QLocalSocket socket;
    socket.connectToServer(MOP_SOCKET_NAME);

    if (!socket.waitForConnected(MOP_SOCKET_TIMEOUT_MS))
        return false;

    // Send one path per line
    for (const QString& f : files) {
        socket.write((f + "\n").toUtf8());
    }
    socket.flush();
    socket.waitForBytesWritten(MOP_SOCKET_TIMEOUT_MS);
    socket.disconnectFromServer();
    return true;
}

#ifdef Q_OS_LINUX
/**
 * @brief Auto-integrates MeshOp3D into the desktop environment when running as
 *        an AppImage.
 *
 * On GNOME/Wayland the dock matches running windows to a registered .desktop
 * file via the Wayland app-id (derived from desktopFileName). Without a
 * registered entry the dock shows a generic placeholder icon.
 *
 * This function writes a .desktop file and the application icon to the user's
 * local data directories (~/.local/share/) the first time the AppImage is run,
 * or whenever the AppImage path has changed (e.g. after a version update).
 * It is a no-op when the binary is not running inside an AppImage.
 */
void tryIntegrateAppImage()
{
    // $APPIMAGE is set by the AppImage runtime to the absolute path of the
    // .AppImage file. Its absence means we are not running as an AppImage.
    const QString appImagePath = qEnvironmentVariable("APPIMAGE");
    if (appImagePath.isEmpty())
        return;

    const QString home        = QDir::homePath();
    const QString desktopDir  = home + QLatin1String("/.local/share/applications");
    const QString iconDir     = home + QLatin1String("/.local/share/icons/hicolor/512x512/apps");
    const QString desktopFile = desktopDir + QLatin1String("/MeshOp3D.desktop");
    const QString iconFile    = iconDir    + QLatin1String("/mop3d-icon.png");

    QDir().mkpath(desktopDir);
    QDir().mkpath(iconDir);

           // Build the .desktop content using the AppImage path as Exec target.
           // MOP_DESKTOP_MIME_TYPES is a narrow string literal from mop_config.h
           // (generated by configure_file from formats.cmake) — it concatenates
           // cleanly with the other narrow string literals at compile time.
    const QString desktopContent = QString(
                                       "[Desktop Entry]\n"
                                       "Name=MeshOp3D\n"
                                       "Comment=MeshOp3D Application\n"
                                       "Exec=\"%1\" %%F\n"
                                       "Icon=mop3d-icon\n"
                                       "Terminal=false\n"
                                       "Type=Application\n"
                                       "Categories=Graphics;3DGraphics;\n"
                                       "StartupWMClass=MeshOp3D\n"
                                       "MimeType=" MOP_DESKTOP_MIME_TYPES "\n"
                                       ).arg(appImagePath);

           // Only (re)write the .desktop file if the content has changed.
           // This avoids touching the mtime on every launch, which would cause
           // update-desktop-database to run unnecessarily.
    bool needsDesktopUpdate = true;
    {
        QFile existing(desktopFile);
        if (existing.open(QIODevice::ReadOnly))
            needsDesktopUpdate = (existing.readAll() != desktopContent.toUtf8());
    }
    if (needsDesktopUpdate) {
        QFile df(desktopFile);
        if (df.open(QIODevice::WriteOnly | QIODevice::Text)) {
            df.write(desktopContent.toUtf8());
            df.close();
            df.setPermissions(
                QFileDevice::ReadOwner  | QFileDevice::WriteOwner |
                QFileDevice::ReadGroup  | QFileDevice::ReadOther);
        }
        QProcess::startDetached(
            QStringLiteral("update-desktop-database"), {desktopDir});
    }

           // Install the icon only if it is not already present (icons don't change
           // between AppImage versions, but the file may have been deleted by the user).
    if (!QFile::exists(iconFile)) {
        if (QFile::copy(QStringLiteral(":/assets/mop3d-icon.png"), iconFile)) {
            QFile::setPermissions(
                iconFile,
                QFileDevice::ReadOwner  | QFileDevice::WriteOwner |
                    QFileDevice::ReadGroup  | QFileDevice::ReadOther);
            QProcess::startDetached(
                QStringLiteral("gtk-update-icon-cache"),
                {QStringLiteral("-f"), QStringLiteral("-t"),
                 home + QLatin1String("/.local/share/icons/hicolor")});
        }
    }
}
#endif // Q_OS_LINUX

