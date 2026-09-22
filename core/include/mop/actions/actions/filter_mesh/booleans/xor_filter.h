// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_BOOLEANS_XOR_FILTER_H
#define MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_BOOLEANS_XOR_FILTER_H

#if defined(VCLIB_WITH_CGAL) && defined(VCLIB_WITH_BOOST)

#include <mop/actions/interfaces/filter_action_base.h>
#include <vclib/igl/booleans.h>

namespace mop {

class XorFilter : public FilterActionBase<XorFilter>
{
public:
    std::string name() const final { return "Boolean XOR"; }

    std::string description() const final
    {
        return "Computes the boolean symmetric difference (XOR) of two watertight meshes.";
    }

    vcl::BitSet<vcl::uint> categories() const final
    {
        return {FilterAction::Category::BOOLEANS};
    }

    std::vector<UintParameter> inputMeshes() const final
    {
        return {
            UintParameter("mesh_a", 1, "First Mesh", ""),
            UintParameter("mesh_b", 2, "Second Mesh", "")};
    }

    std::vector<UintParameter> inputOutputMeshes() const final { return {}; }

    template<vcl::MeshConcept MeshType>
    OutputValues executeFilter(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&,
        std::vector<MeshType>& outputMeshes,
        const ParameterVector&,
        vcl::AbstractLogger& log = FilterAction::logger()) const
    {
        const MeshType& m1 = *inputMeshes[0];
        const MeshType& m2 = *inputMeshes[1];

        MeshType result = vcl::igl::meshBoolean(m1, m2, vcl::igl::MeshBoolean::XOR);
        result.name() = "XOR";
        outputMeshes.push_back(std::move(result));

        return OutputValues();
    }
};

} // namespace mop

#endif // defined(VCLIB_WITH_CGAL) && defined(VCLIB_WITH_BOOST)

#endif // MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_BOOLEANS_XOR_FILTER_H
