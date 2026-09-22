// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_ACTION_MANAGER_H
#define MOP_CORE_MANAGER_ACTION_MANAGER_H

#include "action_manager/manager.h"

namespace mop {

class ActionManager
{
public:
    // add action functions

    static void add(const std::shared_ptr<Action>& action);

    template<vcl::Range R>
    requires vcl::RangeOf<R, std::shared_ptr<Action>>
    static void add(R&& actions)
    {
        instance().add(std::forward<R>(actions));
    }

    // load image

    static std::vector<vcl::FileFormat> loadImageFormats();

    static std::shared_ptr<ImageIOAction> loadImageAction(vcl::FileFormat fmt);

    // save image

    static std::vector<vcl::FileFormat> saveImageFormats();

    static std::shared_ptr<ImageIOAction> saveImageAction(vcl::FileFormat fmt);

    // load mesh

    static std::vector<vcl::FileFormat> loadMeshFormats();

    static ParameterVector loadMeshParameters(vcl::FileFormat fmt);

    static std::shared_ptr<MeshIOAction> loadMeshAction(vcl::FileFormat fmt);

    // save mesh

    static std::vector<vcl::FileFormat> saveMeshFormats();

    static std::vector<vcl::FileFormat> saveMeshFormats(MeshTypeId m);

    static ParameterVector saveMeshParameters(vcl::FileFormat fmt);

    static std::shared_ptr<MeshIOAction> saveMeshAction(vcl::FileFormat fmt);

    // filter

    static std::shared_ptr<FilterAction> filterAction(
        const std::string& name);

    static auto filterActions() { return instance().filterActions(); }

    // convert

    static std::shared_ptr<ConvertAction> convertAction(
        const std::string& name);

    static auto convertActions() { return instance().convertActions(); }

private:
    static detail::Manager& instance();
};

} // namespace mop

#endif // MOP_CORE_MANAGER_ACTION_MANAGER_H
