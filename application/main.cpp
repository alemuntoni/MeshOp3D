// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "main_window.h"

#include <vclib/render/application.h>

#include <QGuiApplication>
#include <QIcon>
#include <QLocalServer>
#include <QLocalSocket>
#include <qglobal.h>

// Name of the local socket used for single-instance IPC
static constexpr const char* MOP_SOCKET_NAME = "MeshOp3D-instance";

// Time (ms) to wait for the running instance to acknowledge our message
static constexpr int MOP_SOCKET_TIMEOUT_MS = 1000;

bool sendToRunningInstance(const QStringList& files);

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

    auto app = vcl::Application(argc, argv);

    QGuiApplication::setDesktopFileName("MeshOp3D-app.desktop");
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

    mw.show();
    mw.showMaximized();

    // Open files passed as command-line arguments / "Open With..."
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
