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

#ifndef HLMP_APPLICATION_MAIN_WINDOW_H
#define HLMP_APPLICATION_MAIN_WINDOW_H

#include <hlmp/actions/interfaces/convert_action.h>
#include <hlmp/actions/interfaces/filter_action.h>

#include "utils.h"

#include <vclib/qt/gui/text_edit_logger.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/mesh_viewer.h>

#include <QAction>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMenu>
#include <QMimeData>

namespace hlmp {

class MainWindow : public vcl::qt::MeshViewer
{
    Q_OBJECT

    // Menus
    QMenu* mFileMenu;
    QMenu* mFilterMenu;
    QMenu* mConvertMenu;

    // Actions
    QAction* mActionOpenMesh;
    QAction* mActionSaveMeshAs;

    // Open Recent
    std::vector<std::string> mRecentFiles;
    QMenu*                   mOpenRecentMenu;
    QAction*                 mRecentFileActions[10];

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

public slots:
    void openMesh();

    void openRecentMesh();

    void saveMeshAs();

    void openFilterDialog(bool);

    void applyFilter(
        const std::shared_ptr<FilterAction>& action,
        const ParameterVector&               params);

    void convertCurrentMesh(bool);

private:
    void loadRecentFiles();

    void saveRecentFiles();

    void addRecentFile(const std::string& filename);

    void updateRecentFilesMenu();

    void loadMesh(const std::string& filename);

    void createMenus();

    void createSearchFilterWidget();

    void populateFilterMenu();

    void openFilterDialog(const std::shared_ptr<FilterAction>& action);

    MeshTypeId getFilterMeshType(
        const std::shared_ptr<FilterAction>& action,
        const ParameterVector&               params,
        vcl::uint                            selectedMesh);

    template<vcl::MeshConcept MeshType>
    void executeFilter(
        const std::shared_ptr<FilterAction>& action,
        const ParameterVector&               params)
    {
        std::vector<const MeshType*> inputMeshes;
        std::vector<MeshType*>       inputOutputMeshes;
        std::vector<MeshType>        outputMeshes;

        std::shared_ptr<vcl::DrawableMesh<MeshType>> m;

        vcl::uint niMeshes  = action->inputMeshes().size();
        vcl::uint nioMeshes = action->inputOutputMeshes().size();
        if (niMeshes + nioMeshes == 1) {
            m = toDrawableMesh<MeshType>(
                this->drawableObject(this->selectedDrawableObject()));
            if (niMeshes == 1) {
                inputMeshes.push_back(m.get());
            }
            else {
                inputOutputMeshes.push_back(m.get());
            }
        }
        logger().startTimer();
        action->execute(
            inputMeshes, inputOutputMeshes, outputMeshes, params, logger());
        logger().stopTimer();

        logger().log(
            action->name() + " applied in " + std::to_string(logger().time()) +
                " seconds.",
            vcl::qt::TextEditLogger::MESSAGE_LOG);

        if (nioMeshes > 0) {
            m->updateBuffers();
        }
        for (const auto& m : outputMeshes) {
            this->pushDrawableObject(makeMeshDrawable(m));
        }
        
        if (nioMeshes > 0 || outputMeshes.size() > 0) {
            this->setRightAreaVisible(true);
        }
        
        this->updateGUI();
    }

    template<vcl::MeshConcept MeshType>
    void convertAndAddMesh(
        const std::shared_ptr<ConvertAction>& action,
        const MeshType&                       mesh)
    {
        logger().startTimer();
        const MeshType* meshPtr = &mesh;
        auto [id, anyMesh]      = action->convertErased(
            meshTypeId<MeshType>(), std::any(meshPtr), logger());
        logger().stopTimer();
        logger().log(
            action->name() + " applied in " + std::to_string(logger().time()) +
                " seconds.",
            vcl::qt::TextEditLogger::MESSAGE_LOG);

        switch (id) {
        case MeshTypeId::TRIANGLE_MESH:
            this->pushDrawableObject(makeMeshDrawable(
                std::move(
                    std::any_cast<vcl::TriEdgeMesh>(std::move(anyMesh)))));
            break;
        case MeshTypeId::POLYGON_MESH:
            this->pushDrawableObject(makeMeshDrawable(
                std::move(
                    std::any_cast<vcl::PolyEdgeMesh>(std::move(anyMesh)))));
            break;
        default: break;
        }
        
        this->setRightAreaVisible(true);
        this->updateGUI();
    }

    template<vcl::MeshConcept MeshType>
    std::shared_ptr<vcl::DrawableMesh<MeshType>> toDrawableMesh(
        const std::shared_ptr<vcl::DrawableObject>& drawable)
    {
        return std::dynamic_pointer_cast<vcl::DrawableMesh<MeshType>>(drawable);
    }
};

} // namespace hlmp

#endif // HLMP_APPLICATION_MAIN_WINDOW_H
