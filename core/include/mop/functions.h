// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_FUNCTIONS_H
#define MOP_CORE_FUNCTIONS_H

#include "manager.h"

#include <any>

namespace mop {

template<template<typename> typename Action, typename MeshType>
auto actionDownCast(const std::shared_ptr<mop::Action>& action)
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

} // namespace mop

#endif // MOP_CORE_FUNCTIONS_H
