// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <mop/manager/action_manager/manager.h>

namespace mop::detail {

Manager::Manager()
{
}

void Manager::add(const std::shared_ptr<Action>& action)
{
    using enum Action::Type;

    vcl::uint mt;

    std::shared_ptr<ConvertAction> convertActions;
    std::shared_ptr<FilterAction>  filterActions;
    std::shared_ptr<ImageIOAction>  imageIOAction;
    std::shared_ptr<MeshIOAction>  meshIOActions;

    switch (action->type()) {
    case CONVERT_ACTION:
        convertActions = std::dynamic_pointer_cast<ConvertAction>(action);
        ConvertManager::add(convertActions);
        break;
    case FILTER_ACTION:
        filterActions = std::dynamic_pointer_cast<FilterAction>(action);
        FilterManager::add(filterActions);
        break;
    case IMAGE_IO_ACTION:
        imageIOAction = std::dynamic_pointer_cast<ImageIOAction>(action);
        ImageIOManager::add(imageIOAction);
        break;
    case MESH_IO_ACTION:
        meshIOActions = std::dynamic_pointer_cast<MeshIOAction>(action);
        MeshIOManager::add(meshIOActions);
        break;
    default: throw std::runtime_error("Action type not supported");
    }
}

} // namespace mop::detail
