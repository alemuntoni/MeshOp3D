/*****************************************************************************
 * HLMP                                                                      *
 * HighLevelMeshProcessing                                                   *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include "main_window.h"

// #include "gui/action_file_dialog.h"
#include "gui/filter_dock_widget.h"
#include "gui/parameter_dialog.h"

#include <hlmp/functions.h>
#include <hlmp/manager.h>
#include <vclib/qt/utils/file_format.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <QFileDialog>
#include <QMenuBar>
#include <QPushButton>

namespace hlmp {

MainWindow::MainWindow(QWidget* parent) : vcl::qt::MeshViewer(parent)
{
    vcl::pushDefaultEditors(*this);

    createMenus();

    // set this function to mesh viewer
    auto f = [](const vcl::DrawableObject& obj) {
        const auto* tri =
            dynamic_cast<const vcl::DrawableMesh<vcl::TriEdgeMesh>*>(&obj);
        if (tri) {
            return std::make_pair(
                QIcon(QString(HLMP_ASSETS_DIR) + "/icons/tri.png"),
                vcl::meshTypeName<vcl::TriEdgeMesh>().c_str());
        }
        const auto* pol =
            dynamic_cast<const vcl::DrawableMesh<vcl::PolyEdgeMesh>*>(&obj);
        if (pol) {
            return std::make_pair(
                QIcon(QString(HLMP_ASSETS_DIR) + "/icons/poly.png"),
                vcl::meshTypeName<vcl::PolyEdgeMesh>().c_str());
        }
        return std::make_pair(QIcon(), "");
    };

    setDrawVectorIconFunction(f);
}

MainWindow::~MainWindow()
{
}

void MainWindow::openMesh()
{
    std::vector<vcl::FileFormat> formats = ActionManager::loadMeshFormats();
    QString filter = vcl::qt::filterFormatsToQString(formats, true);

    QString f = QFileDialog::getOpenFileName(
        nullptr, QObject::tr("Open Document"), QDir::currentPath(), filter);

    if (!f.isEmpty()) {
        std::string     filename = f.toStdString();
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
            break;
        case MeshTypeId::POLYGON_MESH:
            pushDrawableObject(makeMeshDrawable(
                std::move(std::any_cast<vcl::PolyEdgeMesh>(std::move(m)))));
            break;
        default: break;
        }

        updateGUI();
        fitScene();
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
    QString f = QFileDialog::getSaveFileName(
        nullptr, QObject::tr("Save Mesh"), QDir::currentPath(), filter, &fs);

    if (!f.isEmpty()) {
        std::string filename = f.toStdString();
        std::string pfn      = vcl::FileInfo::fileNameWithExtension(filename);
        std::string format   = vcl::FileInfo::extension(filename);
        // get selected filter
        vcl::FileFormat f = vcl::qt::formatFromQStringFilter(fs);
        if (f != vcl::FileFormat(format)) {
            filename += "." + f.extensions().front();
        }

        auto            actions = ActionManager::saveMeshActions(f);
        ParameterVector params  = actions->parametersSave(f);

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
        switch (type) {
        case MeshTypeId::TRIANGLE_MESH: {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::TriEdgeMesh>>(
                    obj);
            actions->save<vcl::TriEdgeMesh>(filename, *m, params, logger());
        } break;
        case MeshTypeId::POLYGON_MESH: {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::PolyEdgeMesh>>(
                    obj);
            actions->save<vcl::PolyEdgeMesh>(filename, *m, params, logger());
        } break;
        default: break;
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
    auto filter = ActionManager::filterActions(actionId);
    assert(filter);
    if (filter) {
        openFilterDialog(filter);
    }
}

void MainWindow::applyFilter(
    const std::shared_ptr<FilterActionsAggregator>& action,
    const ParameterVector&                          params)
{
    MeshTypeId filterMeshType =
        getFilterMeshType(action, params, selectedDrawableObject());

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

void MainWindow::convertCurrentMesh(bool)
{
    using enum MeshTypeId;

    QAction* sender = qobject_cast<QAction*>(QObject::sender());

    std::string actionId =
        sender->property("action_id").toString().toStdString();

    auto convert = ActionManager::convertActions(actionId);
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

void MainWindow::createMenus()
{
    // Create menus
    mFileMenu    = menuBar()->addMenu(tr("&File"));
    mFilterMenu  = menuBar()->addMenu(tr("F&ilter"));
    mConvertMenu = menuBar()->addMenu(tr("&Convert"));

    // Create actions
    mActionOpenMesh =
        new QAction(QIcon::fromTheme("document-open"), tr("Open Mesh"), this);
    mActionSaveMeshAs = new QAction(
        QIcon::fromTheme("document-save-as"), tr("Save Mesh As..."), this);

    // Add actions to File menu
    mFileMenu->addAction(mActionOpenMesh);
    mFileMenu->addAction(mActionSaveMeshAs);

    // Connect actions
    connect(mActionOpenMesh, &QAction::triggered, this, &MainWindow::openMesh);
    connect(
        mActionSaveMeshAs, &QAction::triggered, this, &MainWindow::saveMeshAs);

    // Populate filter and convert menus
    populateFilterMenu();
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

    for (vcl::uint i = 0; i < vcl::toUnderlying(COUNT); ++i) {
        mFilterMenu->addMenu(menus[i]);
    }

    for (const std::shared_ptr<FilterActionsAggregator>& f : filters) {
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

    for (const std::shared_ptr<ConvertActionsAggregator>& c : convert) {
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

void MainWindow::openFilterDialog(
    const std::shared_ptr<FilterActionsAggregator>& action)
{
    FilterDockWidget* dock = new FilterDockWidget(action, this);

    connect(
        dock, &FilterDockWidget::applyFilter, this, &MainWindow::applyFilter);

    dock->show();
}

MeshTypeId MainWindow::getFilterMeshType(
    const std::shared_ptr<FilterActionsAggregator>& action,
    const ParameterVector&                          params,
    vcl::uint                                       selectedMesh)
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
        // TODO: implement
        return MeshTypeId::COUNT;
    }
}

} // namespace hlmp
