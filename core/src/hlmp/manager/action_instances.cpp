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

#include <hlmp/actions/action_types_lists.h>
#include <hlmp/manager/detail/fill_actions.h>

namespace hlmp {

std::vector<std::shared_ptr<Action>> actionInstances()
{
    std::vector<std::shared_ptr<Action>> vec;

    // Convert actions
    fillAggregatedActionInstances<ConvertActionsAggregator>(vec, ConvertActionsList());

    // Filter actions
    fillAggregatedActionInstances<FilterActionsAggregator>(vec, FilterActionsList());

    // ImageIO actions
    // TODO make a function fillActions
    auto f = [&vec]<typename Act>() {
        vec.push_back(std::make_shared<Act>());
    };

    vcl::ForEachType<ImageIOActionsList>::apply(f);

    // MeshIO actions
    fillAggregatedActionInstances<MeshIOActionsAggregator>(vec, MeshIOActionsList());

    return vec;
}

} // namespace hlmp
