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

#ifndef M3D_FUNCTIONS_H
#define M3D_FUNCTIONS_H

#include "manager.h"

#include <any>

namespace hlmp {

template<template<typename> typename Action, typename MeshType>
auto actionDownCast(const std::shared_ptr<hlmp::Action>& action)
{
    return std::dynamic_pointer_cast<Action<MeshType>>(action);
}

std::pair<std::any, MeshTypeId> loadMeshBestFit(
    const std::string&     filename,
    const ParameterVector& parameters,
    auto&                  logger)
{
    std::any    res;
    std::string ext = vcl::FileInfo::extension(filename);

    vcl::MeshInfo info;
    std::any resAny = ActionManager::loadMeshAction(ext)->loadErased(
        MeshTypeId::POLYGON_MESH, filename, vcl::FileFormat(ext), parameters, info, logger);
    vcl::PolyEdgeMesh mesh = std::any_cast<vcl::PolyEdgeMesh>(std::move(resAny));

    if (isTriangleMesh(mesh)) {
        vcl::TriEdgeMesh m;
        m.enableSameOptionalComponentsOf(mesh);
        m.importFrom(mesh);
        res = std::move(m);
        return {res, MeshTypeId::TRIANGLE_MESH};
    }
    else {
        res = std::move(mesh);
        return {res, MeshTypeId::POLYGON_MESH};
    }
}

} // namespace hlmp

#endif // M3D_FUNCTIONS_H
