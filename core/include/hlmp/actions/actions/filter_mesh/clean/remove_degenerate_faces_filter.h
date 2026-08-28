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

#ifndef HLMP_ACTIONS_ACTIONS_FILTER_MESH_CLEAN_REMOVE_DEGENERATE_FACES_FILTER_H
#define HLMP_ACTIONS_ACTIONS_FILTER_MESH_CLEAN_REMOVE_DEGENERATE_FACES_FILTER_H

#include <hlmp/actions/interfaces/filter_action_base.h>
#include <vclib/algorithms/mesh/clean.h>

namespace hlmp {

class RemoveDegenerateFacesFilter :
        public FilterActionBase<RemoveDegenerateFacesFilter>
{
public:
    std::string name() const final { return "Remove Degenerate Faces"; }

    std::string description() const final
    {
        return "Removes or repairs all topologically degenerate faces (e.g. "
               "faces containing consecutive duplicate vertex references).";
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

    template<vcl::FaceMeshConcept MeshType>
    OutputValues executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&,
        const ParameterVector&,
        vcl::AbstractLogger& log = FilterAction::logger()) const
    {
        using enum vcl::AbstractLogger::LogLevel;

        MeshType& mesh = *inputOutputMeshes.front();

        vcl::uint removedCount = vcl::removeDegenerateFaces(mesh);

        if (removedCount > 0) {
            log.log(
                "Removed or repaired " + std::to_string(removedCount) +
                    " degenerate faces.",
                MESSAGE_LOG);
        }
        else {
            log.log("No degenerate faces found.", MESSAGE_LOG);
        }

        return OutputValues();
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_ACTIONS_FILTER_MESH_CLEAN_REMOVE_DEGENERATE_FACES_FILTER_H
