// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_UTILS_H
#define MOP_APPLICATION_UTILS_H

#include <mop/settings.h>

#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>

namespace mop {

template<vcl::MeshConcept MeshType>
std::shared_ptr<vcl::DrawableObject> makeMeshDrawable(
    MeshType&& mesh)
{
    using MT = vcl::RemoveCVRefAndPointer<MeshType>;

    std::shared_ptr<vcl::DrawableMesh<MT>> m =
        std::make_shared<vcl::DrawableMesh<MT>>(std::move(mesh));
    return m;
}

inline MeshTypeId meshId(
    const std::shared_ptr<vcl::DrawableObject>& obj)
{
    auto m = std::dynamic_pointer_cast<vcl::AbstractDrawableMesh>(obj);

    if (!m) {
        assert(0);
        return MeshTypeId::COUNT;
    }

    auto tm = std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::TriEdgeMesh>>(m);

    if (tm) {
        return MeshTypeId::TRIANGLE_MESH;
    }

    auto pm = std::dynamic_pointer_cast<vcl::DrawableMesh<vcl::PolyEdgeMesh>>(m);

    if (pm) {
        return MeshTypeId::POLYGON_MESH;
    }

    assert(0);
    return MeshTypeId::COUNT;
}

} // namespace mop

#endif // MOP_APPLICATION_UTILS_H
