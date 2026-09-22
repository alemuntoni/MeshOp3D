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

#ifndef MOP_MANAGER_ACTION_MANAGER_MANAGERS_MESH_IO_MANAGER_H
#define MOP_MANAGER_ACTION_MANAGER_MANAGERS_MESH_IO_MANAGER_H

#include "../containers/io_action_container.h"

#include <mop/actions/interfaces/mesh_io_action.h>

namespace mop::detail {

class MeshIOManager
{
    IOActionContainer<MeshIOAction> mMeshIOActions;

protected:
    void add(const std::shared_ptr<MeshIOAction>& action)
    {
        mMeshIOActions.add(action);
    }

public:
    // load mesh

    std::vector<vcl::FileFormat> loadMeshFormats() const
    {
        return mMeshIOActions.loadFormats();
    }

    ParameterVector loadMeshParameters(vcl::FileFormat fmt) const
    {
        return loadMeshAction(fmt)->parametersLoad(fmt);
    }

    std::shared_ptr<MeshIOAction> loadMeshAction(vcl::FileFormat fmt) const
    {
        return mMeshIOActions.loadAction(fmt);
    }

    // save mesh

    std::vector<vcl::FileFormat> saveMeshFormats() const
    {
        return mMeshIOActions.saveFormats();
    }

    std::vector<vcl::FileFormat> saveMeshFormats(MeshTypeId m) const
    {
        return mMeshIOActions.saveFormats(m);
    }

    ParameterVector saveMeshParameters(vcl::FileFormat fmt) const
    {
        return saveMeshAction(fmt)->parametersSave(fmt);
    }

    std::shared_ptr<MeshIOAction> saveMeshAction(vcl::FileFormat fmt) const
    {
        return mMeshIOActions.saveAction(fmt);
    }
};

} // namespace mop::detail

#endif // MOP_MANAGER_ACTION_MANAGER_MANAGERS_MESH_IO_MANAGER_H
