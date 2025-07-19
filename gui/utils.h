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

#ifndef UTILS_H
#define UTILS_H

#include <hlmp/settings.h>
#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>

template<vcl::MeshConcept MeshType>
std::shared_ptr<vcl::DrawableObject> makeMeshDrawable(
    MeshType&& mesh)
{
    using MT = vcl::RemoveCVRefAndPointer<MeshType>;

    std::shared_ptr<vcl::DrawableMesh<MT>> m =
        std::make_shared<vcl::DrawableMesh<MT>>(std::move(mesh));
    return m;
}

inline vcl::proc::MeshTypeId meshId(
    const std::shared_ptr<vcl::DrawableObject>& obj)
{
    auto m = std::dynamic_pointer_cast<vcl::AbstractDrawableMesh>(obj);

    if (!m) {
        assert(0);
        return vcl::proc::MeshTypeId::COUNT;
    }

    auto tm = std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::TriEdgeMesh>>(m);

    if (tm) {
        return vcl::proc::MeshTypeId::TRIANGLE_MESH;
    }

    auto pm = std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::PolyEdgeMesh>>(m);

    if (pm) {
        return vcl::proc::MeshTypeId::POLYGON_MESH;
    }

    assert(0);
    return vcl::proc::MeshTypeId::COUNT;
}

#endif // UTILS_H
