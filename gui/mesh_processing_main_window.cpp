/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
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

#include "mesh_processing_main_window.h"

// #include "gui/action_file_dialog.h"
#include "gui/filter_dock_widget.h"
#include "gui/parameter_dialog.h"

#include <vclib/processing.h>
#include <vclib/qt/utils/file_format.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <QFileDialog>
#include <QPushButton>

namespace vcl::qt {

MeshProcessingMainWindow::MeshProcessingMainWindow(QWidget* parent) :
        QMainWindow(parent), mUI(new Ui::MeshProcessingMainWindow)
{
    mUI->setupUi(this);

    mUI->meshViewer->setDrawableObjectVector(mMeshVector);

    populateFilterMenu();

    connect(
        mUI->actionOpenMesh,
        &QAction::triggered,
        this,
        &MeshProcessingMainWindow::openMesh);

    connect(
        mUI->actionSaveMeshAs,
        &QAction::triggered,
        this,
        &MeshProcessingMainWindow::saveMeshAs);

    // set this function to mesh viewer
    auto f = [](const DrawableObject& obj) {
        const auto* tri =
            dynamic_cast<const DrawableMesh<vcl::TriEdgeMesh>*>(&obj);
        if (tri) {
            return std::make_pair(
                QIcon(QString(HLGP_ASSETS_DIR) + "/icons/tri.png"),
                vcl::meshTypeName<vcl::TriEdgeMesh>().c_str());
        }
        const auto* pol =
            dynamic_cast<const DrawableMesh<vcl::PolyEdgeMesh>*>(&obj);
        if (pol) {
            return std::make_pair(
                QIcon(QString(HLGP_ASSETS_DIR) + "/icons/poly.png"),
                vcl::meshTypeName<vcl::PolyEdgeMesh>().c_str());
        }
        return std::make_pair(QIcon(), "");
    };

    mUI->meshViewer->setDrawVectorIconFunction(f);
}

MeshProcessingMainWindow::~MeshProcessingMainWindow()
{
    delete mUI;
}

void MeshProcessingMainWindow::openMesh()
{
    std::vector<FileFormat> formats = proc::ActionManager::loadMeshFormats();
    QString                 filter  = filterFormatsToQString(formats, true);

    QString f = QFileDialog::getOpenFileName(
        nullptr, QObject::tr("Open Document"), QDir::currentPath(), filter);

    if (!f.isEmpty()) {
        std::string filename = f.toStdString();
        std::string pfn      = FileInfo::fileNameWithExtension(filename);
        FileFormat  format   = FileInfo::extension(filename);

        auto params = proc::ActionManager::loadMeshParameters(format);

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
        auto [m, id] = proc::loadMeshBestFit(filename, params, logger());
        logger().stopTimer();
        logger().log(
            TextEditLogger::MESSAGE_LOG,
            pfn + " loaded in " + std::to_string(logger().time()) +
                " seconds.");

        switch (id) {
        case vcl::proc::MeshTypeId::TRIANGLE_MESH:
            mMeshVector->pushBack(makeMeshDrawable(
                std::move(std::any_cast<vcl::TriEdgeMesh>(std::move(m)))));
            break;
        case vcl::proc::MeshTypeId::POLYGON_MESH:
            mMeshVector->pushBack(makeMeshDrawable(
                std::move(std::any_cast<vcl::PolyEdgeMesh>(std::move(m)))));
            break;
        default: break;
        }

        mUI->meshViewer->updateGUI();
        mUI->meshViewer->fitScene();
    }
}

void MeshProcessingMainWindow::saveMeshAs()
{
    uint i = mUI->meshViewer->selectedDrawableObject();

    if (mMeshVector->size() == 0 || i == UINT_NULL) {
        return;
    }

    auto obj = mMeshVector->at(i);

    vcl::proc::MeshTypeId type = meshId(obj);

    if (type == vcl::proc::MeshTypeId::COUNT) {
        return;
    }

    std::vector<FileFormat> formats =
        proc::ActionManager::saveMeshFormats(type);
    QString filter = filterFormatsToQString(formats);

    QString fs;
    QString f = QFileDialog::getSaveFileName(
        nullptr,
        QObject::tr("Save Mesh"),
        QDir::currentPath(),
        filter,
        &fs);

    if (!f.isEmpty()) {
        std::string filename = f.toStdString();
        std::string pfn      = FileInfo::fileNameWithExtension(filename);
        std::string format   = FileInfo::extension(filename);
        // get selected filter
        FileFormat f = formatFromQStringFilter(fs);
        if (f != FileFormat(format)) {
            filename += "." + f.extensions().front();
        }

        auto                  actions = proc::ActionManager::saveMeshActions(f);
        proc::ParameterVector params  = actions->parametersSave(f);

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
        case vcl::proc::MeshTypeId::TRIANGLE_MESH: {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::TriEdgeMesh>>(
                    obj);
            actions->save<vcl::TriEdgeMesh>(filename, *m, params, logger());
        } break;
        case vcl::proc::MeshTypeId::POLYGON_MESH: {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::PolyEdgeMesh>>(
                    obj);
            actions->save<vcl::PolyEdgeMesh>(filename, *m, params, logger());
        } break;
        default: break;
        }
        logger().stopTimer();
        logger().log(
            TextEditLogger::MESSAGE_LOG,
            pfn + " saved in " + std::to_string(logger().time()) + " seconds.");
    }
}

void MeshProcessingMainWindow::openFilterDialog(bool)
{
    QAction* sender = qobject_cast<QAction*>(QObject::sender());

    std::string actionId =
        sender->property("action_id").toString().toStdString();
    auto filter = proc::ActionManager::filterActions(actionId);
    assert(filter);
    if (filter) {
        openFilterDialog(filter);
    }
}

void MeshProcessingMainWindow::applyFilter(
    const std::shared_ptr<proc::FilterActions>& action,
    const proc::ParameterVector&                params)
{
    proc::MeshTypeId filterMeshType = getFilterMeshType(
        action,
        params,
        mUI->meshViewer->selectedDrawableObject());

    switch(filterMeshType) {
        case vcl::proc::MeshTypeId::TRIANGLE_MESH:
            executeFilter<vcl::TriEdgeMesh>(action, params);
            break;
        case vcl::proc::MeshTypeId::POLYGON_MESH:
            executeFilter<vcl::PolyEdgeMesh>(action, params);
            break;
        default:
            break;
    }
}

void MeshProcessingMainWindow::convertCurrentMesh(bool)
{
    using enum vcl::proc::MeshTypeId;

    QAction* sender = qobject_cast<QAction*>(QObject::sender());

    std::string actionId =
        sender->property("action_id").toString().toStdString();

    auto convert = proc::ActionManager::convertActions(actionId);
    assert(convert);
    if (convert) {
        auto i = mUI->meshViewer->selectedDrawableObject();
        if (i != UINT_NULL) {
            auto obj = mMeshVector->at(i);
            auto type = meshId(obj);

            if (type != COUNT) {
                switch(type) {
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
                    default:
                        break;
                }
            }
        }
    }
}

TextEditLogger& MeshProcessingMainWindow::logger()
{
    return mUI->meshViewer->logger();
}

void MeshProcessingMainWindow::populateFilterMenu()
{
    using enum proc::FilterAction::Category;

    auto filters = proc::ActionManager::filterActions();

    std::array<QMenu*, vcl::toUnderlying(COUNT)> menus;
    menus[toUnderlying(CREATE)] = new QMenu("Create", mUI->menuFilter);
    menus[toUnderlying(CLEANING_AND_REPAIRING)] =
        new QMenu("Cleaning and Repairing", mUI->menuFilter);
    menus[toUnderlying(RECONSTRUCTION)] =
        new QMenu("Reconstruction", mUI->menuFilter);
    menus[toUnderlying(SMOOTHING)] = new QMenu("Smoothing", mUI->menuFilter);

    for (uint i = 0; i < vcl::toUnderlying(COUNT); ++i) {
        mUI->menuFilter->addMenu(menus[i]);
    }

    for (const std::shared_ptr<proc::FilterActions>& f : filters) {
        QAction* action = new QAction(f->name().c_str(), mUI->menuFilter);
        action->setProperty(
            "action_id", QVariant(QString::fromStdString(f->name())));

        for (uint i = 0; i < vcl::toUnderlying(COUNT); ++i) {
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

    auto convert = proc::ActionManager::convertActions();

    for (const std::shared_ptr<proc::ConvertActions>& c : convert) {
        QAction* action = new QAction(c->name().c_str(), mUI->menuConvert);
        action->setProperty(
            "action_id", QVariant(QString::fromStdString(c->name())));
        mUI->menuConvert->addAction(action);

        connect(
            action,
            SIGNAL(triggered(bool)),
            this,
            SLOT(convertCurrentMesh(bool)));
    }
}

void MeshProcessingMainWindow::openFilterDialog(
    const std::shared_ptr<proc::FilterActions>& action)
{
    FilterDockWidget* dock = new FilterDockWidget(action, this);

    connect(
        dock,
        &FilterDockWidget::applyFilter,
        this,
        &MeshProcessingMainWindow::applyFilter);

    dock->show();
}

proc::MeshTypeId MeshProcessingMainWindow::getFilterMeshType(
    const std::shared_ptr<proc::FilterActions>& action,
    const proc::ParameterVector&                params,
    uint                                        selectedMesh)
{
    uint niMeshes  = action->inputMeshes().size();
    uint nioMeshes = action->inputOutputMeshes().size();

    if (niMeshes + nioMeshes == 0) {
        // no input meshes, the type of the mesh is given by the user trough
        // the parameters
        uint t = params.get("output_mesh_type")->uintValue();
        return static_cast<proc::MeshTypeId>(t);
    }
    else if (niMeshes + nioMeshes == 1) {
        assert(selectedMesh != UINT_NULL);
        // only one mesh input, the type of the mesh is the same as the
        // selectedMesh
        return meshId(mMeshVector->at(selectedMesh));
    }
    else {
        // TODO: implement
        return proc::MeshTypeId::COUNT;
    }
}

} // namespace vcl::qt
