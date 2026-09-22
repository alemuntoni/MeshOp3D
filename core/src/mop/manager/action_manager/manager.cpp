/*****************************************************************************
 * MeshOp3D                                                                      *
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

#include <mop/manager/action_manager/manager.h>

#include <mop/manager/action_instances.h>

namespace mop::detail {

Manager::Manager()
{
    addDefaultActions();
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

void Manager::addDefaultActions()
{
    add(actionInstances());
}

} // namespace mop::detail
