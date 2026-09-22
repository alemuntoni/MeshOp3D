/*****************************************************************************
 * MeshOp3D                                                                      *
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

#include <mop/manager/action_instances.h>

#include <mop/actions/action_types_lists.h>
#include <mop/manager/detail/fill_actions.h>

namespace mop {

std::vector<std::shared_ptr<Action>> actionInstances()
{
    std::vector<std::shared_ptr<Action>> vec;

    // Convert actions
    fillActionInstances(vec, ConvertActionsList());

    // Filter actions
    fillActionInstances(vec, FilterActionsList());

    // ImageIO actions
    fillActionInstances(vec, ImageIOActionsList());

    // MeshIO actions
    fillActionInstances(vec, MeshIOActionsList());

    return vec;
}

} // namespace mop
