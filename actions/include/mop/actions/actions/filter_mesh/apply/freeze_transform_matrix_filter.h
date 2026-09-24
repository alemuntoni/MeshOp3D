// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_APPLY_FREEZE_TRANSFORM_MATRIX_FILTER_H
#define MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_APPLY_FREEZE_TRANSFORM_MATRIX_FILTER_H

#include <mop/actions/interfaces/filter_action_base.h>

#include <vclib/algorithms/mesh/update/transform.h>

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
    OutputValues executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&,
        const ParameterVector&,
        vcl::AbstractLogger& log = FilterAction::logger()) const
    {
        MeshType& mesh = *inputOutputMeshes.front();
        if constexpr (vcl::comp::HasTransformMatrix<MeshType>) {
            if (vcl::comp::isTransformMatrixAvailableOn(mesh)) {
                vcl::applyTransformMatrix(mesh, mesh.transformMatrix());
                mesh.transformMatrix().setIdentity();
            } else {
                log.log("The mesh does not have a transform matrix component enabled.", log.WARNING_LOG);
            }
        } else {
            log.log("The mesh does not have a transform matrix component.", log.WARNING_LOG);
        }
        return OutputValues();
    }
};

} // namespace mop

#endif // MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_APPLY_FREEZE_TRANSFORM_MATRIX_FILTER_H
