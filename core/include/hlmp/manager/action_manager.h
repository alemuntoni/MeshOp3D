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

#ifndef HLMP_MANAGER_ACTION_MANAGER_H
#define HLMP_MANAGER_ACTION_MANAGER_H

#include "action_manager/manager.h"

namespace hlmp {

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

    static std::shared_ptr<MeshIOActionsAggregator> loadMeshActions(
        vcl::FileFormat fmt);

    template<vcl::MeshConcept MeshType>
    static std::shared_ptr<MeshIOActionT<MeshType>> loadMeshAction(
        vcl::FileFormat fmt)
    {
        return instance().loadMeshAction<MeshType>(fmt);
    }

    // save mesh

    static std::vector<vcl::FileFormat> saveMeshFormats();

    static std::vector<vcl::FileFormat> saveMeshFormats(MeshTypeId m);

    static ParameterVector saveMeshParameters(vcl::FileFormat fmt);

    static std::shared_ptr<MeshIOActionsAggregator> saveMeshActions(
        vcl::FileFormat fmt);

    template<vcl::MeshConcept MeshType>
    static std::shared_ptr<MeshIOActionT<MeshType>> saveMeshAction(
        vcl::FileFormat fmt)
    {
        return instance().saveMeshAction<MeshType>(fmt);
    }

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

} // namespace hlmp

#endif // HLMP_MANAGER_ACTION_MANAGER_H
