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

#include <hlmp/manager/action_instances/filter.h>

#include <hlmp/actions/actions/filter_mesh.h>
#include <hlmp/actions/aggregators/filter_actions_aggregator.h>
#include <hlmp/manager/action_instances/detail/fill_actions.h>

#include <memory>
#include <vector>

namespace hlmp {

namespace detail {

inline std::vector<std::shared_ptr<Action>> applyFilterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = vcl::TemplatedTypeWrapper<LaplacianSmoothingFilter>;

    fillAggregatedActions<FilterActionsAggregator>(vec, Actions());

    return vec;
}

inline std::vector<std::shared_ptr<Action>> createFilterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = vcl::TemplatedTypeWrapper<CreateConeFilter>;

    fillAggregatedActions<FilterActionsAggregator>(vec, Actions());

    return vec;
}

inline std::vector<std::shared_ptr<Action>> generateFilterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = vcl::TemplatedTypeWrapper<ConvexHullFilter>;

    fillAggregatedActions<FilterActionsAggregator>(vec, Actions());

    return vec;
}

} // namespace detail

std::vector<std::shared_ptr<Action>> filterActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    auto a = detail::applyFilterActions();
    vec.insert(vec.begin(), a.begin(), a.end());

    auto c = detail::createFilterActions();
    vec.insert(vec.begin(), c.begin(), c.end());

    auto g = detail::generateFilterActions();
    vec.insert(vec.begin(), g.begin(), g.end());

    return vec;
}

// namespace detail

} // namespace hlmp
