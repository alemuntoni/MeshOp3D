// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_ACTION_MANAGER_MANAGER_H
#define MOP_CORE_MANAGER_ACTION_MANAGER_MANAGER_H

#include "managers/convert_manager.h"
#include "managers/filter_manager.h"
#include "managers/image_io_manager.h"
#include "managers/mesh_io_manager.h"

namespace mop::detail {

class Manager :
        public ConvertManager,
        public FilterManager,
        public ImageIOManager,
        public MeshIOManager
{
public:
    Manager();

    void add(const std::shared_ptr<Action>& action);

    template<vcl::Range R>
    requires vcl::RangeOf<R, std::shared_ptr<Action>>
    void add(R&& actions)
    {
        for (const auto& action : actions) {
            add(action);
        }
    }

    void addDefaultActions();
};

} // namespace mop::detail

#endif // MOP_CORE_MANAGER_ACTION_MANAGER_MANAGER_H
