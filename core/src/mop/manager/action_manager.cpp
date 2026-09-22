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

#include <mop/manager/action_manager.h>

namespace mop {

void ActionManager::add(const std::shared_ptr<Action>& action)
{
    instance().add(action);
}

// load image

std::vector<vcl::FileFormat> ActionManager::loadImageFormats()
{
    return instance().loadImageFormats();
}

std::shared_ptr<ImageIOAction> ActionManager::loadImageAction(
    vcl::FileFormat fmt)
{
    return instance().loadImageAction(fmt);
}

// save image

std::vector<vcl::FileFormat> ActionManager::saveImageFormats()
{
    return instance().saveImageFormats();
}

std::shared_ptr<ImageIOAction> ActionManager::saveImageAction(
    vcl::FileFormat fmt)
{
    return instance().saveImageAction(fmt);
}

// load mesh

std::vector<vcl::FileFormat> ActionManager::loadMeshFormats()
{
    return instance().loadMeshFormats();
}

ParameterVector ActionManager::loadMeshParameters(vcl::FileFormat fmt)
{
    return instance().loadMeshParameters(fmt);
}

std::shared_ptr<MeshIOAction> ActionManager::loadMeshAction(
    vcl::FileFormat fmt)
{
    return instance().loadMeshAction(fmt);
}

// save mesh

std::vector<vcl::FileFormat> ActionManager::saveMeshFormats()
{
    return instance().saveMeshFormats();
}

std::vector<vcl::FileFormat> ActionManager::saveMeshFormats(MeshTypeId m)
{
    return instance().saveMeshFormats(m);
}

ParameterVector ActionManager::saveMeshParameters(vcl::FileFormat fmt)
{
    return instance().saveMeshParameters(fmt);
}

std::shared_ptr<MeshIOAction> ActionManager::saveMeshAction(
    vcl::FileFormat fmt)
{
    return instance().saveMeshAction(fmt);
}

// filter

std::shared_ptr<FilterAction> ActionManager::filterAction(
    const std::string& name)
{
    return instance().filterAction(name);
}

// convert

std::shared_ptr<ConvertAction> ActionManager::convertAction(
    const std::string& name)
{
    return instance().convertAction(name);
}

detail::Manager& ActionManager::instance()
{
    static detail::Manager instance;
    return instance;
}

} // namespace mop
