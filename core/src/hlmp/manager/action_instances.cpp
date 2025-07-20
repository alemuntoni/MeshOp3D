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

#include <hlmp/manager/action_instances.h>

#include <hlmp/manager/action_instances/convert.h>
#include <hlmp/manager/action_instances/filter.h>
#include <hlmp/manager/action_instances/image_io.h>
#include <hlmp/manager/action_instances/mesh_io.h>

namespace hlmp {

std::vector<std::shared_ptr<Action>> actionInstances()
{
    std::vector<std::shared_ptr<Action>> vec;

    // Convert actions
    auto convertVector = convertActions();
    vec.insert(vec.end(), convertVector.begin(), convertVector.end());

    // Filter actions
    auto filterVector = filterActions();
    vec.insert(vec.end(), filterVector.begin(), filterVector.end());

    // ImageIO actions
    auto imgIOVector = imageIOActions();
    vec.insert(vec.end(), imgIOVector.begin(), imgIOVector.end());

    // MeshIO actions
    auto meshIOVector = meshIOActions();
    vec.insert(vec.end(), meshIOVector.begin(), meshIOVector.end());

    return vec;
}

} // namespace hlmp
