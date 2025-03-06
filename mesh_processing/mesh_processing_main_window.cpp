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

#include "ui_mesh_processing_main_window.h"
#include "mesh_processing_main_window.h"

//#include "gui/action_file_dialog.h"
//#include "gui/filter_mesh_dock_widget.h"
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

    // populate action manager
    //mActionManager.add(proc::vclibActions());

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
                QIcon(QString(MESH_PROCESSING_ASSETS_DIR) + "/icons/tri.png"),
                "TriMesh");
        }
        const auto* pol =
            dynamic_cast<const DrawableMesh<vcl::PolyEdgeMesh>*>(&obj);
        if (pol) {
            return std::make_pair(
                QIcon(QString(MESH_PROCESSING_ASSETS_DIR) + "/icons/poly.png"),
                "PolyMesh");
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
    QString filter = filterFormatsToQString(formats, true);

    QString f = QFileDialog::getOpenFileName(
        nullptr,
        QObject::tr("Open Document"),
        QDir::currentPath(),
        filter);

    if (!f.isEmpty()) {
        std::string filename = f.toStdString();
        std::string pfn      = FileInfo::fileNameWithExtension(filename);
        FileFormat  format   = FileInfo::extension(filename);

        auto params =
            proc::ActionManager::loadMeshParameters(format);

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
        default:
            break;
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
        QObject::tr("Save Document"),
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

        auto action = proc::ActionManager::saveMeshAction(f, type);
        proc::ParameterVector params = proc::saveMeshParameters(type, f);

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
        switch(type) {
        case vcl::proc::MeshTypeId::TRIANGLE_MESH:
        {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::TriEdgeMesh>>(obj);
            actionDownCast<proc::MeshIOActionT, vcl::TriEdgeMesh>(action)
                ->save(filename, *m, params, logger());
        }
            break;
        case vcl::proc::MeshTypeId::POLYGON_MESH:
        {
            auto m =
                std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::PolyEdgeMesh>>(obj);
            actionDownCast<proc::MeshIOActionT, vcl::PolyEdgeMesh>(action)
                ->save(filename, *m, params, logger());
        }
            break;
        default:
            break;
        }
        logger().stopTimer();
        logger().log(
            TextEditLogger::MESSAGE_LOG,
            pfn + " saved in " + std::to_string(logger().time()) +
                " seconds.");
    }

    // std::vector<FileFormat> formats = mActionManager.saveMeshFormats();

    // ActionSaveFileDialog<proc::SaveMeshAction>* dialog =
    //     new ActionSaveFileDialog<proc::SaveMeshAction>(
    //         mActionManager.saveMeshActionManager(), "Save Mesh", "", this);

    // if (dialog->exec() == QDialog::Accepted) {
    //     auto fs = dialog->selectedFiles();

    //     std::string filename = fs.first().toStdString();
    //     std::string pfn      = FileInfo::fileNameWithExtension(filename);
    //     std::string format   = FileInfo::extension(filename);
    //     FileFormat  f        = dialog->selectedFormat();

    //     if (f != FileFormat(format)) {
    //         filename += "." + f.extensions().front();
    //     }

    //     uint i = mUI->meshViewer->selectedDrawableObject();
    //     std::shared_ptr<DrawableObject> d = mMeshVector->at(i);

    //     std::shared_ptr<proc::MeshI> m = toMesh(d);

    //     if (m) {
    //         auto params = dialog->parameters(format);

    //         logger().startTimer();
    //         mActionManager.saveMeshAction(format)->save(
    //             filename, *m, params, logger());
    //         logger().stopTimer();
    //         logger().log(
    //             TextEditLogger::MESSAGE_LOG,
    //             pfn + " saved in " + std::to_string(logger().time()) +
    //                 " seconds.");
    //     }
    // }
}

void MeshProcessingMainWindow::openFilterDialog(bool)
{
    // QAction* sender = qobject_cast<QAction*>(QObject::sender());

    // std::string filterId =
    //     sender->property("filter_id").toString().toStdString();
    // auto filter = mActionManager.filterMeshActionById(filterId);

    // if (filter) {
    //     openFilterDialog(filter);
    // }
}

// void MeshProcessingMainWindow::applyFilter(
//     const std::shared_ptr<proc::FilterMeshAction>& action,
//     const proc::ParameterVector&                   params)
// {
//     proc::MeshVector                          inputMeshes;
//     std::vector<std::shared_ptr<proc::MeshI>> inputOutputMeshes;
//     proc::MeshVector                          outputMeshes;

//     uint niMeshes  = action->inputMeshParameters().size();
//     uint nioMeshes = action->inputOutputMeshParameters().size();

//     if (niMeshes + nioMeshes == 1) {
//         auto m =
//             toMesh(mMeshVector->at(mUI->meshViewer->selectedDrawableObject()));
//         if (niMeshes == 1) {
//             inputMeshes.pushBack(m);
//         }
//         else {
//             inputOutputMeshes.push_back(m);
//         }
//     }

//     logger().startTimer();

//     action->applyFilter(
//         inputMeshes, inputOutputMeshes, outputMeshes, params, logger());

//     logger().stopTimer();

//     logger().log(
//         TextEditLogger::MESSAGE_LOG,
//         action->name() + " applied in " + std::to_string(logger().time()) +
//             " seconds.");

//     for (const auto& m : inputOutputMeshes) {
//         toAbstractDrawableMesh(m)->updateBuffers();
//     }

//     for (const auto& m : outputMeshes) {
//         mMeshVector->pushBack(makeMeshDrawable(m));
//     }

//     mUI->meshViewer->updateGUI();
//     mUI->meshViewer->fitScene();
// }

TextEditLogger& MeshProcessingMainWindow::logger()
{
    return mUI->meshViewer->logger();
}

void MeshProcessingMainWindow::populateFilterMenu()
{
    // auto filters = mActionManager.filterMeshActions();

    // std::array<QMenu*, proc::FilterMeshAction::N_CATEGORIES> menus;
    // menus[proc::FilterMeshAction::CREATE] =
    //     new QMenu("Create", mUI->menuFilter);
    // menus[proc::FilterMeshAction::CLEANING_AND_REPAIRING] =
    //     new QMenu("Cleaning and Repairing", mUI->menuFilter);
    // menus[proc::FilterMeshAction::RECONSTRUCTION] =
    //     new QMenu("Reconstruction", mUI->menuFilter);
    // menus[proc::FilterMeshAction::SMOOTHING] =
    //     new QMenu("Smoothing", mUI->menuFilter);

    // for (uint i = 0; i < proc::FilterMeshAction::N_CATEGORIES; ++i) {
    //     mUI->menuFilter->addMenu(menus[i]);
    // }

    // for (const std::shared_ptr<proc::FilterMeshAction>& f : filters) {
    //     QAction* action = new QAction(f->name().c_str(), mUI->menuFilter);
    //     action->setProperty(
    //         "filter_id", QVariant(QString::fromStdString(f->identifier())));

    //     for (uint i = 0; i < proc::FilterMeshAction::N_CATEGORIES; ++i) {
    //         if (f->categories()[i]) {
    //             menus[i]->addAction(action);
    //         }
    //     }

    //     connect(
    //         action,
    //         SIGNAL(triggered(bool)),
    //         this,
    //         SLOT(openFilterDialog(bool)));
    // }
}

// void MeshProcessingMainWindow::openFilterDialog(
//     const std::shared_ptr<proc::FilterMeshAction>& action)
// {
//     FilterMeshDockWidget* dock = new FilterMeshDockWidget(action, this);

//     connect(
//         dock,
//         &FilterMeshDockWidget::applyFilter,
//         this,
//         &MeshProcessingMainWindow::applyFilter);

//     dock->show();
// }

// std::shared_ptr<proc::MeshI> MeshProcessingMainWindow::toMesh(
//     const std::shared_ptr<DrawableObject>& drawable)
// {
//     return std::dynamic_pointer_cast<proc::MeshI>(drawable);
// }

// std::shared_ptr<AbstractDrawableMesh> MeshProcessingMainWindow::
//     toAbstractDrawableMesh(const std::shared_ptr<proc::MeshI>& mesh)
// {
//     return std::dynamic_pointer_cast<AbstractDrawableMesh>(mesh);
// }

} // namespace vcl::qt
