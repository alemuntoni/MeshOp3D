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

#ifndef M3D_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H
#define M3D_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H

#include "../containers/id_action_container.h"

#include <m3d/actions/interfaces/filter_action.h>

namespace hlmp::detail {

class FilterManager
{
    IDActionContainer<FilterAction> mFilterActions;

protected:
    void add(const std::shared_ptr<FilterAction>& action)
    {
        mFilterActions.add(action);
    }

public:
    // filter

    std::shared_ptr<FilterAction> filterAction(const std::string& name) const
    {
        return mFilterActions.action(name);
    }

    auto filterActions() { return mFilterActions.actions(); }
};

} // namespace hlmp::detail

#endif // M3D_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H
