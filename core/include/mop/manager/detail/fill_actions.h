// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_DETAIL_FILL_ACTIONS_H
#define MOP_CORE_MANAGER_DETAIL_FILL_ACTIONS_H

#include <mop/actions/interfaces/action.h>

namespace mop {

template<typename... Actions>
void fillActionInstances(
    std::vector<std::shared_ptr<Action>>& vec,
    vcl::TypeWrapper<Actions...>)
{
    auto fAct = [&]<typename Act>() {
        vec.push_back(std::make_shared<Act>());
    };

    (fAct.template operator()<Actions>(), ...);
}

/**
 * @brief @brief Given a list of actions in a TemplatedTypeWrapper, this
 * function fills the given vector with instances of Aggregator type (one for
 * each action type) - see mop/actions/action_aggregators.
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

} // namespace mop

#endif // MOP_CORE_MANAGER_DETAIL_FILL_ACTIONS_H
