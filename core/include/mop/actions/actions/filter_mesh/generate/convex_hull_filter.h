// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H
#define MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H

#include <mop/actions/interfaces/filter_action_base.h>

#include <vclib/algorithms/mesh/convex_hull.h>

namespace mop {

class ConvexHullFilter : public FilterActionBase<ConvexHullFilter>
{

public:
    std::string name() const final { return "Convex Hull"; }

    std::string description() const final
    {
        return "Generates a convex hull mesh from a set of 3D points.";
    }

    FilterAction::CategoryBitSet categories() const final
    {
        return {FilterAction::Category::RECONSTRUCTION};
    }

    std::vector<UintParameter> inputMeshes() const final
    {
        return {UintParameter("input", 0, "Input Mesh", "")};
    }

    std::vector<UintParameter> inputOutputMeshes() const final { return {}; }

    ParameterVector parameters() const final { return {}; }

    template<vcl::MeshConcept MeshType>
    OutputValues executeFilter(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        vcl::AbstractLogger&                log = FilterAction::logger()) const
    {
        const MeshType& input = *inputMeshes.front();
        std::string     name  = "Convex Hull";
        if constexpr (vcl::HasName<MeshType>) {
            name = input.name() + " " + name;
        }
        outputMeshes.push_back(convexHull<MeshType>(
            input.vertices() | vcl::views::positions, log));
        if constexpr (vcl::HasName<MeshType>) {
            outputMeshes.back().name() = name;
        }
        return OutputValues();
    }
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_GENERATE_CONVEX_HULL_FILTER_H
