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

#ifndef HLMP_MAIN_WINDOW_H
#define HLMP_MAIN_WINDOW_H

#include <hlmp/actions/aggregators/convert_actions.h>
#include <hlmp/actions/aggregators/filter_actions.h>

#include "utils.h"

#include "ui_main_window.h"

#include <vclib/qt/gui/text_edit_logger.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>

#include <QMainWindow>

namespace hlmp {

namespace Ui {
class MainWindow;
} // namespace Ui

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow* mUI;

    //proc::ActionManager mActionManager;

    std::shared_ptr<vcl::DrawableObjectVector> mMeshVector =
        std::make_shared<vcl::DrawableObjectVector>();

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void openMesh();

    void saveMeshAs();

    void openFilterDialog(bool);

    void applyFilter(
        const std::shared_ptr<vcl::proc::FilterActions>& action,
        const vcl::proc::ParameterVector&                params);

    void convertCurrentMesh(bool);

private:
    vcl::qt::TextEditLogger& logger();

    void populateFilterMenu();

    void openFilterDialog(
        const std::shared_ptr<vcl::proc::FilterActions>& action);

    vcl::proc::MeshTypeId getFilterMeshType(
        const std::shared_ptr<vcl::proc::FilterActions>& action,
        const vcl::proc::ParameterVector&                params,
        uint selectedMesh);

    template<vcl::MeshConcept MeshType>
    void executeFilter(
        const std::shared_ptr<vcl::proc::FilterActions>& action,
        const vcl::proc::ParameterVector&                params)
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
            vcl::qt::TextEditLogger::MESSAGE_LOG,
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

    template<vcl::MeshConcept MeshType>
    void convertAndAddMesh(
        const std::shared_ptr<vcl::proc::ConvertActions>& action,
        const MeshType& mesh)
    {
        logger().startTimer();
        auto [id, anyMesh] = action->convert(mesh, logger());
        logger().stopTimer();
        logger().log(
            vcl::qt::TextEditLogger::MESSAGE_LOG,
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

    template<vcl::MeshConcept MeshType>
    std::shared_ptr<vcl::DrawableMesh<MeshType>> toDrawableMesh(
        const std::shared_ptr<vcl::DrawableObject>& drawable)
    {
        return std::dynamic_pointer_cast<vcl::DrawableMesh<MeshType>>(drawable);
    }
};

} // namespace hlmp

#endif // HLMP_MAIN_WINDOW_H
