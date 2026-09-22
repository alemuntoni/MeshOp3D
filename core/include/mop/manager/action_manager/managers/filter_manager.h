// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H
#define MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H

#include "../containers/id_action_container.h"

#include <mop/actions/interfaces/filter_action.h>

namespace mop::detail {

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

} // namespace mop::detail

#endif // MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_FILTER_MANAGER_H
