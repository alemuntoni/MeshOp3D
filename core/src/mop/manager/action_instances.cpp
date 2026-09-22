// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
