// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
