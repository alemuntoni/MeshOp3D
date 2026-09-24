// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_ACTIONS_ACTION_INSTANCES_H
#define MOP_ACTIONS_ACTION_INSTANCES_H

#include <mop/actions/interfaces/action.h>

namespace mop {

std::vector<std::shared_ptr<Action>> actionInstances();

} // namespace mop

#endif // MOP_ACTIONS_ACTION_INSTANCES_H
