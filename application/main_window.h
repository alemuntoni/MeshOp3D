// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_MAIN_WINDOW_H
#define MOP_APPLICATION_MAIN_WINDOW_H

#include <mop/actions/interfaces/convert_action.h>
#include <mop/actions/interfaces/filter_action.h>

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

namespace mop {

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

    void loadMesh(const std::string& filename);

private:
    void loadRecentFiles();

    void saveRecentFiles();

    void addRecentFile(const std::string& filename);

    void updateRecentFilesMenu();

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

        std::vector<std::shared_ptr<vcl::DrawableMesh<MeshType>>> modifiedDrawables;

        vcl::uint niMeshes  = action->inputMeshes().size();
        vcl::uint nioMeshes = action->inputOutputMeshes().size();
        if (niMeshes + nioMeshes == 1) {
            auto m = toDrawableMesh<MeshType>(
                this->drawableObject(this->selectedDrawableObject()));
            if (!m) {
                throw std::runtime_error(
                    "Selected mesh is of incompatible type.");
            }
            if (niMeshes == 1) {
                inputMeshes.push_back(m.get());
            }
            else {
                inputOutputMeshes.push_back(m.get());
                modifiedDrawables.push_back(m);
            }
        }
        else if (niMeshes + nioMeshes > 1) {
            for (vcl::uint i = 0; i < niMeshes; ++i) {
                vcl::uint id = params.get("mesh_input_" + std::to_string(i))->uintValue();
                auto mesh = toDrawableMesh<MeshType>(this->drawableObject(id));
                if (!mesh) {
                    throw std::runtime_error(
                        "Selected mesh for input " + std::to_string(i) +
                        " is of incompatible type.");
                }
                inputMeshes.push_back(mesh.get());
            }

            std::vector<vcl::uint> inoutIds;
            for (vcl::uint i = 0; i < nioMeshes; ++i) {
                vcl::uint id = params.get("mesh_inout_" + std::to_string(i))->uintValue();
                if (std::find(inoutIds.begin(), inoutIds.end(), id) != inoutIds.end()) {
                    throw std::runtime_error(
                        "Duplicate mesh selected for input/output.");
                }
                inoutIds.push_back(id);

                auto mesh = toDrawableMesh<MeshType>(this->drawableObject(id));
                if (!mesh) {
                    throw std::runtime_error(
                        "Selected mesh for input/output " + std::to_string(i) +
                        " is of incompatible type.");
                }
                inputOutputMeshes.push_back(mesh.get());
                modifiedDrawables.push_back(mesh);
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
            for (auto& mod : modifiedDrawables) {
                mod->updateBuffers();
            }
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

} // namespace mop

#endif // MOP_APPLICATION_MAIN_WINDOW_H
