// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_CREATE_CREATE_CONE_FILTER_H
#define MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_CREATE_CREATE_CONE_FILTER_H

#include <mop/actions/interfaces/filter_action_base.h>

#include <vclib/algorithms/mesh.h>

namespace mop {

class CreateConeFilter : public FilterActionBase<CreateConeFilter>
{

public:
    std::string name() const final { return "Create Cone"; }

    std::string description() const final { return "Creates a cone mesh."; }

    vcl::BitSet<vcl::uint> categories() const override
    {
        return {FilterAction::Category::CREATE};
    }

    std::vector<UintParameter> inputMeshes() const final { return {}; }

    std::vector<UintParameter> inputOutputMeshes() const final { return {}; }

    ParameterVector parameters() const final
    {
        ParameterVector params;

        params.pushBack(
            UscalarParameter("bottom_radius", 1, "Bottom Radius", ""));
        params.pushBack(UscalarParameter("top_radius", 1, "Top Radius", ""));
        params.pushBack(UscalarParameter("height", 1, "Height", ""));
        params.pushBack(
            UintParameter("subdivisions", 36, "N. Subdivisions", ""));

        return params;
    }

    template<vcl::MeshConcept MeshType>
    OutputValues executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>&,
        std::vector<MeshType>& outputMeshes,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log = FilterAction::logger()) const
    {
        auto bottomRadius = parameters.get("bottom_radius")->scalarValue();
        auto topRadius    = parameters.get("top_radius")->scalarValue();
        auto height       = parameters.get("height")->scalarValue();
        auto subdivisions = parameters.get("subdivisions")->uintValue();

        auto cone = vcl::createCone<MeshType>(
            bottomRadius, topRadius, height, subdivisions);

        vcl::updatePerVertexAndFaceNormals(cone);

        cone.name() = "Cone";

        outputMeshes.push_back(std::move(cone));

        return OutputValues();
    }
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_CREATE_CREATE_CONE_FILTER_H
