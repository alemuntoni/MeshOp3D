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

#ifndef VCL_QT_MESH_PROCESSING_MAIN_WINDOW_H
#define VCL_QT_MESH_PROCESSING_MAIN_WINDOW_H

#include "utils.h"

#include "ui_mesh_processing_main_window.h"

//#include <vclib/processing/action_manager.h>
#include <vclib/qt/gui/text_edit_logger.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>

#include <QMainWindow>

namespace vcl::qt {

namespace Ui {
class MeshProcessingMainWindow;
} // namespace Ui

class MeshProcessingMainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MeshProcessingMainWindow* mUI;

    //proc::ActionManager mActionManager;

    std::shared_ptr<vcl::DrawableObjectVector> mMeshVector =
        std::make_shared<vcl::DrawableObjectVector>();

public:
    explicit MeshProcessingMainWindow(QWidget* parent = nullptr);
    ~MeshProcessingMainWindow();

public slots:
    void openMesh();

    void saveMeshAs();

    void openFilterDialog(bool);

    void applyFilter(
        const std::shared_ptr<proc::FilterActions>& action,
        const proc::ParameterVector&                params);

    void convertCurrentMesh(bool);

private:
    TextEditLogger& logger();

    void populateFilterMenu();

    void openFilterDialog(
        const std::shared_ptr<proc::FilterActions>& action);

    proc::MeshTypeId getFilterMeshType(
        const std::shared_ptr<proc::FilterActions>& action,
        const proc::ParameterVector&                params,
        uint selectedMesh);

    template<MeshConcept MeshType>
    void executeFilter(
        const std::shared_ptr<proc::FilterActions>& action,
        const proc::ParameterVector&                params)
    {
        std::vector<const MeshType*> inputMeshes;
        std::vector<MeshType*>       inputOutputMeshes;
        std::vector<MeshType>        outputMeshes;

        std::shared_ptr<vcl::DrawableMesh<MeshType>> m;

        uint niMeshes = action->inputMeshes().size();
        uint nioMeshes = action->inputOutputMeshes().size();
        if (niMeshes + nioMeshes == 1) {
            m = toDrawableMesh<MeshType>(
                mMeshVector->at(mUI->meshViewer->selectedDrawableObject()));
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
            TextEditLogger::MESSAGE_LOG,
            action->name() + " applied in " +
                std::to_string(logger().time()) + " seconds.");

        if (nioMeshes > 0) {
            m->updateBuffers();
        }
        for (const auto& m : outputMeshes) {
            mMeshVector->pushBack(makeMeshDrawable(m));
        }
        mUI->meshViewer->updateGUI();
    }

    template<MeshConcept MeshType>
    void convertAndAddMesh(
        const std::shared_ptr<proc::ConvertActions>& action,
        const MeshType& mesh)
    {
        logger().startTimer();
        auto [id, anyMesh] = action->convert(mesh, logger());
        logger().stopTimer();
        logger().log(
            TextEditLogger::MESSAGE_LOG,
            action->name() + " applied in " + std::to_string(logger().time()) +
                " seconds.");

        switch (id) {
        case vcl::proc::MeshTypeId::TRIANGLE_MESH:
            mMeshVector->pushBack(makeMeshDrawable(std::move(
                std::any_cast<vcl::TriEdgeMesh>(std::move(anyMesh)))));
            break;
        case vcl::proc::MeshTypeId::POLYGON_MESH:
            mMeshVector->pushBack(makeMeshDrawable(std::move(
                std::any_cast<vcl::PolyEdgeMesh>(std::move(anyMesh)))));
            break;
        default: break;
        }
        mUI->meshViewer->updateGUI();
    }

    template<MeshConcept MeshType>
    std::shared_ptr<vcl::DrawableMesh<MeshType>> toDrawableMesh(
        const std::shared_ptr<vcl::DrawableObject>& drawable)
    {
        return std::dynamic_pointer_cast<vcl::DrawableMesh<MeshType>>(drawable);
    }
};

} // namespace vcl::qt

#endif // VCL_QT_MESH_PROCESSING_MAIN_WINDOW_H
