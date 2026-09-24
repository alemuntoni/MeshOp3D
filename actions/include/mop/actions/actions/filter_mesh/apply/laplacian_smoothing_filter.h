// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_APPLY_LAPLACIAN_SMOOTHING_FILTER_H
#define MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_APPLY_LAPLACIAN_SMOOTHING_FILTER_H

#include <mop/actions/interfaces/filter_action_base.h>

#include <vclib/algorithms/mesh/smooth.h>

namespace mop {

class LaplacianSmoothingFilter : public FilterActionBase<LaplacianSmoothingFilter>
{

public:
    std::string name() const final { return "Laplacian Smoothing"; }

    std::string description() const final
    {
        return "Laplacian smooth. Average each vertex position with weighted "
               "positions of neighbour vertices.<br><b>"
               "Laplacian Mesh Processing</b> by <i>Olga Sorkine</i>. "
               "EUROGRAPHICS 2005<br>"
               "<a href='http://dx.doi.org/10.2312/egst.20051044'>doi:10.2312/"
               "egst.20051044</a>";
    }

    vcl::BitSet<vcl::uint> categories() const final
    {
        return {FilterAction::Category::SMOOTHING};
    }

    std::vector<UintParameter> inputMeshes() const final { return {}; }

    std::vector<UintParameter> inputOutputMeshes() const final
    {
        return {UintParameter("input_output", 1, "Input/Output Mesh", "")};
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(UintParameter(
            "smoothing_steps",
            3,
            "Smoothing steps",
            "The number of times that the whole algorithm (normal smoothing + "
            "vertex fitting) is "
            "iterated."));
        params.pushBack(BoolParameter(
            "cotangent_weighting",
            true,
            "Cotangent weighting",
            "Use cotangent weighting scheme for the averaging of the position. "
            "Otherwise the simpler umbrella scheme (1 if the edge is present) "
            "is used."));
        params.pushBack(BoolParameter(
            "only_selected",
            false,
            "Only selected",
            "If checked, the filter is performed only on the selected area."));

        return params;
    }

    template<vcl::MeshConcept MeshType>
    OutputValues executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log = FilterAction::logger()) const
    {
        vcl::uint smoothingSteps = parameters.get("smoothing_steps")->uintValue();
        bool cotangentWeighting =
            parameters.get("cotangent_weighting")->boolValue();
        bool onlySelected = parameters.get("only_selected")->boolValue();

        MeshType& mesh = *inputOutputMeshes.front();

        vcl::laplacianSmoothing(
            mesh, smoothingSteps, onlySelected, cotangentWeighting);

        return OutputValues();
    }
};

} // namespace mop

#endif // MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_APPLY_LAPLACIAN_SMOOTHING_FILTER_H
