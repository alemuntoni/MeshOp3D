// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "main_window.h"

// #include "gui/action_file_dialog.h"
#include "gui/filter_dock_widget.h"
#include "gui/mop_settings_tab.h"
#include "gui/parameter_dialog.h"
#include "gui/search_filter_widget.h"

#include <mop/action_instances.h>
#include <mop/functions.h>
#include <mop/manager.h>

#include <vclib/qt/gui/dialog_directories.h>
#include <vclib/qt/utils/file_format.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <nlohmann/json.hpp>

#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>
#include <QToolBar>
#include <QUrl>

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace mop {

MainWindow::MainWindow(QWidget* parent) :
        vcl::qt::MeshViewer(
            parent,
            (vcl::appConfigDirectory("MeshOp3D") / "settings.json").string())
{
    MainWindow::setWindowTitle("MeshOp3D");

    std::string settingsPath = settingsFilePath();
    if (std::filesystem::exists(settingsPath)) {
        std::ifstream in(settingsPath);
        if (in.is_open()) {
            nlohmann::json j;
            try {
                in >> j;
                mMopSettings.loadSettings(j);
            }
            catch (...) {
            }
        }
    }
    this->addSettingsTab(std::make_shared<MopSettingsTab>(mMopSettings));

    mop::ActionManager::add(mop::actionInstances());

    vcl::pushDefaultEditors(*this);

    createMenus();
    setAcceptDrops(true);

    // set this function to mesh viewer
    auto f = [](const vcl::DrawableObject& obj) {
        const auto* tri =
            dynamic_cast<const vcl::DrawableMesh<vcl::TriEdgeMesh>*>(&obj);
        if (tri) {
            return std::make_pair(
                QIcon(":/assets/icons/tri.png"), std::string("Triangle Mesh"));
        }
        const auto* pol =
            dynamic_cast<const vcl::DrawableMesh<vcl::PolyEdgeMesh>*>(&obj);
        if (pol) {
            return std::make_pair(
                QIcon(":/assets/icons/poly.png"),
                std::string("Polygonal Mesh"));
        }
        return std::make_pair(QIcon(), std::string(""));
    };

    setDrawVectorIconFunction(f);

    createSearchFilterWidget();

    loadRecentFiles();
    updateRecentFilesMenu();

    setRightAreaVisible(false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        std::vector<vcl::FileFormat> formats = ActionManager::loadMeshFormats();
        bool                         accept  = false;

        for (const QUrl& url : event->mimeData()->urls()) {
            if (url.isLocalFile()) {
                std::string     filename = url.toLocalFile().toStdString();
                vcl::FileFormat format   = vcl::FileInfo::extension(filename);

                for (const auto& f : formats) {
                    if (f == format) {
                        accept = true;
                        break;
                    }
                }
            }
            if (accept) {
                break;
            }
        }

        if (accept) {
            event->acceptProposedAction();
        }
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        std::vector<vcl::FileFormat> formats = ActionManager::loadMeshFormats();

        for (const QUrl& url : mimeData->urls()) {
            if (url.isLocalFile()) {
                std::string     filename = url.toLocalFile().toStdString();
                vcl::FileFormat format   = vcl::FileInfo::extension(filename);

                bool accept = false;
                for (const auto& f : formats) {
                    if (f == format) {
                        accept = true;
                        break;
                    }
                }

                if (accept) {
                    loadMesh(filename);
                }
            }
        }
        event->acceptProposedAction();
    }
}

void MainWindow::openMesh()
{
    std::vector<vcl::FileFormat> formats = ActionManager::loadMeshFormats();
    QString filter = vcl::qt::filterFormatsToQString(formats, true);

    QString lastDir = vcl::qt::dialogDirectory("MeshOp3DMesh");
    QString f       = QFileDialog::getOpenFileName(
        nullptr, QObject::tr("Open Document"), lastDir, filter);

    if (!f.isEmpty()) {
        vcl::qt::setDialogDirectory(
            "MeshOp3DMesh", QFileInfo(f).absolutePath());
        loadMesh(f.toStdString());
    }
}

void MainWindow::openRecentMesh()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString path = action->data().toString();
        if (!path.isEmpty()) {
            loadMesh(path.toStdString());
        }
    }
}

void MainWindow::saveMeshAs()
{
    vcl::uint i = selectedDrawableObject();

    if (drawableObjectsCount() == 0 || i == vcl::UINT_NULL) {
        return;
    }

    auto obj = drawableObject(i);

    MeshTypeId type = meshId(obj);

    if (type == MeshTypeId::COUNT) {
        return;
    }

    std::vector<vcl::FileFormat> formats = ActionManager::saveMeshFormats(type);
    QString filter = vcl::qt::filterFormatsToQString(formats);

    QString fs;
    QString lastDir = vcl::qt::dialogDirectory("MeshOp3DMesh");
    QString f       = QFileDialog::getSaveFileName(
        nullptr, QObject::tr("Save Mesh"), lastDir, filter, &fs);

    if (!f.isEmpty()) {
        vcl::qt::setDialogDirectory(
            "MeshOp3DMesh", QFileInfo(f).absolutePath());
        std::string filename = f.toStdString();
        std::string pfn      = vcl::FileInfo::fileNameWithExtension(filename);
        std::string format   = vcl::FileInfo::extension(filename);
        // get selected filter
        vcl::FileFormat f = vcl::qt::formatFromQStringFilter(fs);
        if (f != vcl::FileFormat(format)) {
            filename += "." + f.extensions().front();
        }

        auto            action = ActionManager::saveMeshAction(f);
        ParameterVector params = action->parametersSave(f);

        if (!params.empty()) {
            ParameterDialog* dialog = new ParameterDialog(params, "Save Mesh");
            dialog->exec();

            if (dialog->result() == QDialog::Rejected) {
                return;
            }
            else {
                params = dialog->parameters();
            }
        }

        logger().startTimer();
        vcl::MeshInfo info = action->formatCapability(f);
        std::any      meshPtrAny;

        switch (type) {
        case MeshTypeId::TRIANGLE_MESH: {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::TriEdgeMesh>>(
                    obj);
            const vcl::TriEdgeMesh* ptr = m.get();
            meshPtrAny = std::make_any<const vcl::TriEdgeMesh*>(ptr);
        } break;
        case MeshTypeId::POLYGON_MESH: {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::PolyEdgeMesh>>(
                    obj);
            const vcl::PolyEdgeMesh* ptr = m.get();
            meshPtrAny = std::make_any<const vcl::PolyEdgeMesh*>(ptr);
        } break;
        default: break;
        }

        if (meshPtrAny.has_value()) {
            action->saveErased(
                type, filename, f, meshPtrAny, info, params, logger());
        }

        logger().stopTimer();
        logger().log(
            pfn + " saved in " + std::to_string(logger().time()) + " seconds.",
            vcl::qt::TextEditLogger::MESSAGE_LOG);
    }
}

void MainWindow::openFilterDialog(bool)
{
    QAction* sender = qobject_cast<QAction*>(QObject::sender());

    std::string actionId =
        sender->property("action_id").toString().toStdString();
    auto filter = ActionManager::filterAction(actionId);
    assert(filter);
    if (filter) {
        openFilterDialog(filter);
    }
}

void MainWindow::applyFilter(
    const std::shared_ptr<FilterAction>& action,
    const ParameterVector&               params)
{
    MeshTypeId filterMeshType =
        getFilterMeshType(action, params, selectedDrawableObject());

    try {
        switch (filterMeshType) {
        case MeshTypeId::TRIANGLE_MESH:
            executeFilter<vcl::TriEdgeMesh>(action, params);
            break;
        case MeshTypeId::POLYGON_MESH:
            executeFilter<vcl::PolyEdgeMesh>(action, params);
            break;
        default: break;
        }
    }
    catch (const std::exception& e) {
        std::string msg = "Error executing filter '" + action->name() +
                          "': " + std::string(e.what());

        logger().log(msg, vcl::qt::TextEditLogger::ERROR_LOG);
        QMessageBox::critical(
            this, "Error", QString::fromStdString(msg), QMessageBox::Ok);
    }
}

void MainWindow::convertCurrentMesh(bool)
{
    using enum MeshTypeId;

    QAction* sender = qobject_cast<QAction*>(QObject::sender());

    std::string actionId =
        sender->property("action_id").toString().toStdString();

    auto convert = ActionManager::convertAction(actionId);
    assert(convert);
    if (convert) {
        auto i = selectedDrawableObject();
        if (i != vcl::UINT_NULL) {
            auto obj  = drawableObject(i);
            auto type = meshId(obj);

            if (type != COUNT) {
                switch (type) {
                case TRIANGLE_MESH:
                    convertAndAddMesh<vcl::TriEdgeMesh>(
                        convert,
                        *std::dynamic_pointer_cast<
                            vcl::DrawableMesh<vcl::TriEdgeMesh>>(obj));
                    break;
                case POLYGON_MESH:
                    convertAndAddMesh<vcl::PolyEdgeMesh>(
                        convert,
                        *std::dynamic_pointer_cast<
                            vcl::DrawableMesh<vcl::PolyEdgeMesh>>(obj));
                    break;
                default: break;
                }
            }
        }
    }
}

void MainWindow::loadMesh(const std::string& filename)
{
    std::string     pfn    = vcl::FileInfo::fileNameWithExtension(filename);
    vcl::FileFormat format = vcl::FileInfo::extension(filename);

    auto params = ActionManager::loadMeshParameters(format);

    if (!params.empty()) {
        ParameterDialog* dialog = new ParameterDialog(params, "Load Mesh");
        dialog->exec();

        if (dialog->result() == QDialog::Rejected) {
            return;
        }
        else {
            params = dialog->parameters();
        }
    }

    logger().startTimer();
    auto [m, id] = loadMeshBestFit(filename, params, logger());
    logger().stopTimer();
    logger().log(
        pfn + " loaded in " + std::to_string(logger().time()) + " seconds.",
        vcl::qt::TextEditLogger::MESSAGE_LOG);

    switch (id) {
    case MeshTypeId::TRIANGLE_MESH:
        pushDrawableObject(makeMeshDrawable(
            std::move(std::any_cast<vcl::TriEdgeMesh>(std::move(m)))));
        addRecentFile(filename);
        setRightAreaVisible(true);
        break;
    case MeshTypeId::POLYGON_MESH:
        pushDrawableObject(makeMeshDrawable(
            std::move(std::any_cast<vcl::PolyEdgeMesh>(std::move(m)))));
        addRecentFile(filename);
        setRightAreaVisible(true);
        break;
    default: break;
    }

    updateGUI();
    fitScene();
}

void MainWindow::loadRecentFiles()
{
    std::string filePath = this->settingsFilePath();
    if (filePath.empty())
        return;

    std::ifstream in(filePath);
    if (in.is_open()) {
        nlohmann::json j;
        try {
            in >> j;
            if (j.contains("recent_files") && j["recent_files"].is_array()) {
                mRecentFiles.clear();
                for (const auto& item : j["recent_files"]) {
                    mRecentFiles.push_back(item.get<std::string>());
                }
            }
        }
        catch (...) {
            // Ignore parse errors
        }
    }
}

void MainWindow::saveRecentFiles()
{
    std::string filePath = this->settingsFilePath();
    if (filePath.empty())
        return;

    nlohmann::json j;
    std::ifstream  in(filePath);
    if (in.is_open()) {
        try {
            in >> j;
        }
        catch (...) {
            j = nlohmann::json::object();
        }
        in.close();
    }
    else {
        j = nlohmann::json::object();
    }

    j["recent_files"] = mRecentFiles;

    std::filesystem::path dir = std::filesystem::path(filePath).parent_path();
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }

    std::ofstream out(filePath);
    if (out.is_open()) {
        out << j.dump(4);
    }
}

void MainWindow::addRecentFile(const std::string& filename)
{
    auto it = std::find(mRecentFiles.begin(), mRecentFiles.end(), filename);
    if (it != mRecentFiles.end()) {
        mRecentFiles.erase(it);
    }

    mRecentFiles.insert(mRecentFiles.begin(), filename);

    while (mRecentFiles.size() > 10) {
        mRecentFiles.pop_back();
    }

    saveRecentFiles();
    updateRecentFilesMenu();
}

void MainWindow::updateRecentFilesMenu()
{
    if (mOpenRecentMenu) {
        mOpenRecentMenu->setEnabled(!mRecentFiles.empty());
    }

    for (int i = 0; i < 10; ++i) {
        if (!mRecentFileActions[i])
            continue;

        if (i < mRecentFiles.size()) {
            std::string path     = mRecentFiles[i];
            std::string filename = vcl::FileInfo::fileNameWithExtension(path);
            int         key      = (i + 1) % 10;
            QString     text     = QString("&%1 %2").arg(key).arg(
                QString::fromStdString(filename));

            mRecentFileActions[i]->setText(text);
            mRecentFileActions[i]->setData(QString::fromStdString(path));
            mRecentFileActions[i]->setVisible(true);
            mRecentFileActions[i]->setToolTip(QString::fromStdString(path));
        }
        else {
            mRecentFileActions[i]->setVisible(false);
        }
    }
}

void MainWindow::createMenus()
{
    QAction* before =
        menuBar()->actions().isEmpty() ? nullptr : menuBar()->actions().first();

    mFileMenu    = new QMenu(tr("&File"), this);
    mFilterMenu  = new QMenu(tr("F&ilter"), this);
    mConvertMenu = new QMenu(tr("&Convert"), this);

    // file first,filter and convert after other menus
    menuBar()->insertMenu(before, mFileMenu);
    menuBar()->addMenu(mFilterMenu);
    menuBar()->addMenu(mConvertMenu);

    // Create actions
    mActionOpenMesh =
        new QAction(QIcon::fromTheme("document-open"), tr("Open Mesh"), this);
    mActionSaveMeshAs = new QAction(
        QIcon::fromTheme("document-save-as"), tr("Save Mesh As..."), this);

    mOpenRecentMenu = new QMenu(tr("Open &Recent"), this);
    for (int i = 0; i < 10; ++i) {
        mRecentFileActions[i] = new QAction(this);
        mRecentFileActions[i]->setVisible(false);
        int key = (i + 1) % 10;
        mRecentFileActions[i]->setShortcut(
            QKeySequence(QString("Ctrl+%1").arg(key)));
        connect(
            mRecentFileActions[i],
            &QAction::triggered,
            this,
            &MainWindow::openRecentMesh);
        mOpenRecentMenu->addAction(mRecentFileActions[i]);
    }

    // Add actions to File menu
    mFileMenu->addAction(mActionOpenMesh);
    mFileMenu->addMenu(mOpenRecentMenu);
    mFileMenu->addAction(mActionSaveMeshAs);

    // Connect actions
    connect(mActionOpenMesh, &QAction::triggered, this, &MainWindow::openMesh);
    connect(
        mActionSaveMeshAs, &QAction::triggered, this, &MainWindow::saveMeshAs);

    // Populate filter and convert menus
    populateFilterMenu();
}

void MainWindow::createSearchFilterWidget()
{
    SearchFilterWidget* searchWidget = new SearchFilterWidget(this);
    menuBar()->setCornerWidget(searchWidget);

    connect(
        searchWidget,
        &SearchFilterWidget::filterSelected,
        this,
        [this](const std::shared_ptr<FilterAction>& filter) {
            openFilterDialog(filter);
        });

    QShortcut* shortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(shortcut, &QShortcut::activated, searchWidget, [searchWidget]() {
        searchWidget->setFocus();
        searchWidget->selectAll();
    });
}

void MainWindow::populateFilterMenu()
{
    using enum FilterAction::Category;

    auto filters = ActionManager::filterActions();

    std::array<QMenu*, vcl::toUnderlying(COUNT)> menus;
    menus[vcl::toUnderlying(CREATE)] = new QMenu("Create", mFilterMenu);
    menus[vcl::toUnderlying(CLEANING_AND_REPAIRING)] =
        new QMenu("Cleaning and Repairing", mFilterMenu);
    menus[vcl::toUnderlying(RECONSTRUCTION)] =
        new QMenu("Reconstruction", mFilterMenu);
    menus[vcl::toUnderlying(SMOOTHING)] = new QMenu("Smoothing", mFilterMenu);
    menus[vcl::toUnderlying(BOOLEANS)]  = new QMenu("Booleans", mFilterMenu);
    menus[vcl::toUnderlying(TRANSFORM)] = new QMenu("Transform", mFilterMenu);

    for (vcl::uint i = 0; i < vcl::toUnderlying(COUNT); ++i) {
        mFilterMenu->addMenu(menus[i]);
    }

    for (const std::shared_ptr<FilterAction>& f : filters) {
        QAction* action = new QAction(f->name().c_str(), mFilterMenu);
        action->setProperty(
            "action_id", QVariant(QString::fromStdString(f->name())));

        for (vcl::uint i = 0; i < vcl::toUnderlying(COUNT); ++i) {
            if (f->categories()[i]) {
                menus[i]->addAction(action);
            }
        }

        connect(
            action,
            SIGNAL(triggered(bool)),
            this,
            SLOT(openFilterDialog(bool)));
    }

    auto convert = ActionManager::convertActions();

    for (const std::shared_ptr<ConvertAction>& c : convert) {
        QAction* action = new QAction(c->name().c_str(), mConvertMenu);
        action->setProperty(
            "action_id", QVariant(QString::fromStdString(c->name())));
        mConvertMenu->addAction(action);

        connect(
            action,
            SIGNAL(triggered(bool)),
            this,
            SLOT(convertCurrentMesh(bool)));
    }
}

void MainWindow::openFilterDialog(const std::shared_ptr<FilterAction>& action)
{
    vcl::uint niMeshes  = action->inputMeshes().size();
    vcl::uint nioMeshes = action->inputOutputMeshes().size();

    if (nioMeshes > drawableObjectsCount()) {
        QMessageBox::warning(
            this,
            "Error",
            "Not enough meshes loaded. At least " + QString::number(nioMeshes) +
                " meshes are required for this operation.");
        return;
    }
    if (niMeshes > 0 && drawableObjectsCount() == 0) {
        QMessageBox::warning(
            this,
            "Error",
            "No meshes loaded. At least 1 mesh is required for this "
            "operation.");
        return;
    }

    if (mMopSettings.bypassFilterDialogForSimpleFilters &&
        action->parameters().empty() && (niMeshes + nioMeshes <= 1)) {
        applyFilter(action, action->parameters());
        return;
    }

    std::vector<std::string> meshNames;
    for (vcl::uint i = 0; i < drawableObjectsCount(); ++i) {
        meshNames.push_back(
            drawableObject(i)->name() + " (" + std::to_string(i) + ")");
    }

    FilterDockWidget* dock =
        new FilterDockWidget(action, meshNames, selectedDrawableObject(), this);

    connect(
        dock, &FilterDockWidget::applyFilter, this, &MainWindow::applyFilter);

    dock->show();
}

MeshTypeId MainWindow::getFilterMeshType(
    const std::shared_ptr<FilterAction>& action,
    const ParameterVector&               params,
    vcl::uint                            selectedMesh)
{
    vcl::uint niMeshes  = action->inputMeshes().size();
    vcl::uint nioMeshes = action->inputOutputMeshes().size();

    if (niMeshes + nioMeshes == 0) {
        // no input meshes, the type of the mesh is given by the user trough
        // the parameters
        vcl::uint t = params.get("output_mesh_type")->uintValue();
        return static_cast<MeshTypeId>(t);
    }
    else if (niMeshes + nioMeshes == 1) {
        assert(selectedMesh != vcl::UINT_NULL);
        // only one mesh input, the type of the mesh is the same as the
        // selectedMesh
        return meshId(drawableObject(selectedMesh));
    }
    else {
        if (niMeshes > 0) {
            vcl::uint id = params.get("mesh_input_0")->uintValue();
            return meshId(drawableObject(id));
        }
        else if (nioMeshes > 0) {
            vcl::uint id = params.get("mesh_inout_0")->uintValue();
            return meshId(drawableObject(id));
        }
        return MeshTypeId::COUNT;
    }
}

} // namespace mop
