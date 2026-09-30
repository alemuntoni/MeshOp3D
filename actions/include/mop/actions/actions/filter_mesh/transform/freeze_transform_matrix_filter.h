// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_TRANSFORM_FREEZE_TRANSFORM_MATRIX_FILTER_H
#define MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_TRANSFORM_FREEZE_TRANSFORM_MATRIX_FILTER_H

#include <mop/actions/interfaces/filter_action_base.h>
#include <mop/actions/undo_redo/mesh_component_snapshot_undo_action.h>

#include <vclib/algorithms/mesh/update/transform.h>

#include <span>

namespace mop {

class FreezeTransformMatrixFilter : public FilterActionBase<FreezeTransformMatrixFilter>
{
public:
    std::string name() const final { return "Freeze Transform Matrix"; }

    std::string description() const final
    {
        return "Applies the transform matrix to the mesh vertices and resets "
               "it to the identity.";
    }

    vcl::BitSet<vcl::uint> categories() const final
    {
        return {FilterAction::Category::TRANSFORM};
    }

    std::vector<UintParameter> inputMeshes() const final { return {}; }

    std::vector<UintParameter> inputOutputMeshes() const final
    {
        return {UintParameter("input_output", 1, "Input/Output Mesh", "")};
    }

    template<vcl::MeshConcept MeshType>
    FilterActionResult executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&,
        const ParameterVector&,
        vcl::AbstractLogger& log = FilterAction::logger()) const
    {
        MeshType& mesh = *inputOutputMeshes.front();

        std::unique_ptr<vcl::UndoRedoAction> undoAction;

        // snapshot only the components that applyTransformMatrix and
        // the identity reset actually touch, not the whole mesh
        auto composite = std::make_unique<vcl::CompositeUndoRedoAction>(name());

        composite->addAction(makeMeshComponentSnapshotUndoAction(
            mesh,
            [](MeshType& m) {
                return m.vertices() | vcl::views::positions;
            },
            "Vertex Positions"));

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                composite->addAction(makeMeshComponentSnapshotUndoAction(
                    mesh,
                    [](MeshType& m) {
                        return m.vertices() | vcl::views::normals;
                    },
                    "Vertex Normals"));
            }
        }
        if constexpr (vcl::HasPerFaceNormal<MeshType>) {
            if (vcl::isPerFaceNormalAvailable(mesh)) {
                composite->addAction(makeMeshComponentSnapshotUndoAction(
                    mesh,
                    [](MeshType& m) {
                        return m.faces() | vcl::views::normals;
                    },
                    "Face Normals"));
            }
        }

        composite->addAction(makeMeshComponentSnapshotUndoAction(
            mesh,
            [](MeshType& m) {
                return std::span(&m.transformMatrix(), 1);
            },
            "Transform Matrix"));

        vcl::applyTransformMatrix(mesh, mesh.transformMatrix());
        mesh.transformMatrix().setIdentity();

        undoAction = std::move(composite);

        return FilterActionResult(std::move(undoAction), OutputValues());
    }
};

} // namespace mop

#endif // MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_TRANSFORM_FREEZE_TRANSFORM_MATRIX_FILTER_H
