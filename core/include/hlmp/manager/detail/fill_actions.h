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

#ifndef HLMP_MANAGER_DETAIL_FILL_ACTIONS_H
#define HLMP_MANAGER_DETAIL_FILL_ACTIONS_H

#include <hlmp/actions/interfaces/action.h>

namespace hlmp {

/**
 * @brief @brief Given a list of actions in a TemplatedTypeWrapper, this
 * function fills the given vector with instances of Aggregator type (one for
 * each action type) - see hlmp/actions/action_aggregators.
 * Each Aggregator contains the action instances that can be instantiated for
 * the supported mesh types.
 * @param vec
 */
template<typename Aggregator, template<typename> typename... Actions>
void fillAggregatedActionInstances(
    std::vector<std::shared_ptr<Action>>& vec,
    vcl::TemplatedTypeWrapper<Actions...>)
{
    auto fAct = [&]<template<typename> typename Act>() {
        std::shared_ptr<Aggregator> a = std::make_shared<Aggregator>();
        a->template fillWithSupportedMeshTypes<Act>();
        vec.push_back(a);
    };

    (fAct.template operator()<Actions>(), ...);
}

} // namespace hlmp

#endif // HLMP_MANAGER_DETAIL_FILL_ACTIONS_H
