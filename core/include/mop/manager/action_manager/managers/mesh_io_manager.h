// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_MESH_IO_MANAGER_H
#define MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_MESH_IO_MANAGER_H

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

#endif // MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_MESH_IO_MANAGER_H
