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

#include <hlmp/manager/action_instances/mesh_io.h>

#include <hlmp/actions/actions/mesh_io.h>
#include <hlmp/actions/aggregators/mesh_io_actions_aggregator.h>
#include <hlmp/manager/action_instances/detail/fill_actions.h>

#include <memory>
#include <vector>

namespace hlmp {

std::vector<std::shared_ptr<Action>> meshIOActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = vcl::TemplatedTypeWrapper<BaseMeshIO>;

    fillAggregatedActions<MeshIOActionsAggregator>(vec, Actions());

    return vec;
}

}
