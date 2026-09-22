// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_ACTION_MANAGER_CONTAINERS_ID_ACTION_CONTAINER_H
#define MOP_CORE_MANAGER_ACTION_MANAGER_CONTAINERS_ID_ACTION_CONTAINER_H

#include <mop/actions/interfaces/action.h>

#include <map>

namespace mop::detail {

template<typename ActionType>
class IDActionContainer
{
    using ActionMap = std::map<std::string, std::shared_ptr<ActionType>>;

    ActionMap mActionMap;

public:
    IDActionContainer() = default;

    void add(std::shared_ptr<ActionType> action)
    {
        if (!action) {
            throw std::runtime_error("Action is nullptr.");
        }
        checkActionDoesNotExist(action->name());
        mActionMap[action->name()] = action;
    }

    std::shared_ptr<ActionType> action(const std::string& name) const
    {
        auto it = findActionExists(name);
        return it->second;
    }

    auto actions()
    {
        auto f = [](const ActionMap::value_type& p) {
            return p.second;
        };
        return std::views::transform(mActionMap, f);
    }

private:
    void checkActionDoesNotExist(const std::string& name) const
    {
        if (mActionMap.find(name) != mActionMap.end()) {
            throw std::runtime_error("Action " + name + " already registered.");
        }
    }

    ActionMap::const_iterator findActionExists(const std::string& name) const
    {
        auto it = mActionMap.find(name);
        if (it == mActionMap.end()) {
            throw std::runtime_error("Action " + name + " not registered.");
        }
        return it;
    }
};

} // namespace mop::detail

#endif // MOP_CORE_MANAGER_ACTION_MANAGER_CONTAINERS_ID_ACTION_CONTAINER_H
