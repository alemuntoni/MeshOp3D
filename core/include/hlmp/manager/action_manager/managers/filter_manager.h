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

#ifndef HLMP_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H
#define HLMP_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H

#include "../containers/id_action_container.h"

#include <hlmp/actions/aggregators/filter_actions_aggregator.h>

namespace hlmp::detail {

class FilterManager
{
    IDActionContainer<FilterActionsAggregator> mFilterActions;

protected:
    void add(const std::shared_ptr<FilterActionsAggregator>& action)
    {
        mFilterActions.add(action);
    }

public:
    // filter

    std::shared_ptr<FilterActionsAggregator> filterActions(const std::string& name) const
    {
        return mFilterActions.action(name);
    }

    template<typename MeshType>
    std::shared_ptr<FilterActionT<MeshType>> filterAction(
        const std::string& name)
    {
        std::shared_ptr<FilterActionsAggregator> actions = filterActions(name);

        return actions->action<MeshType>();
    }

    auto filterActions() { return mFilterActions.actions(); }
};

} // namespace hlmp::detail

#endif // HLMP_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H
