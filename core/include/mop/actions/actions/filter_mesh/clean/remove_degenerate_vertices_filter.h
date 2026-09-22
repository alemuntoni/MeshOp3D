// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_CLEAN_REMOVE_DEGENERATE_VERTICES_FILTER_H
#define MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_CLEAN_REMOVE_DEGENERATE_VERTICES_FILTER_H

#include <mop/actions/interfaces/filter_action_base.h>
#include <vclib/algorithms/mesh/clean.h>

namespace mop {

class RemoveDegenerateVerticesFilter :
        public FilterActionBase<RemoveDegenerateVerticesFilter>
{
public:
    std::string name() const final { return "Remove Degenerate Vertices"; }

    std::string description() const final
    {
        return "Removes all vertices that have position with invalid floating "
               "point values (NaN or inf). \nNote: this filter also "
               "automatically deletes any element (faces, edges, etc.) that "
               "becomes incident to a deleted vertex.";
    }

    vcl::BitSet<vcl::uint> categories() const final
    {
        return {FilterAction::Category::CLEANING_AND_REPAIRING};
    }

    std::vector<UintParameter> inputMeshes() const final { return {}; }

    std::vector<UintParameter> inputOutputMeshes() const final
    {
        return {UintParameter("input_output", 1, "Input/Output Mesh", "")};
    }

    ParameterVector parameters() const override { return ParameterVector(); }

    template<vcl::MeshConcept MeshType>
    OutputValues executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&,
        const ParameterVector&,
        vcl::AbstractLogger& log = FilterAction::logger()) const
    {
        using enum vcl::AbstractLogger::LogLevel;

        MeshType& mesh = *inputOutputMeshes.front();

        // Always pass deleteAlsoIncidentElements = true
        vcl::uint removedCount = vcl::removeDegenerateVertices(mesh, true);

        if (removedCount > 0) {
            log.log(
                "Removed " + std::to_string(removedCount) +
                    " degenerate vertices and any incident elements.",
                MESSAGE_LOG);
        }
        else {
            log.log("No degenerate vertices found.", MESSAGE_LOG);
        }

        return OutputValues();
    }
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_ACTIONS_FILTER_MESH_CLEAN_REMOVE_DEGENERATE_VERTICES_FILTER_H
