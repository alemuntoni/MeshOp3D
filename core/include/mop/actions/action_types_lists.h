// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_ACTION_TYPES_LISTS_H
#define MOP_CORE_ACTIONS_ACTION_TYPES_LISTS_H

#include "actions/convert.h"
#include "actions/filter_mesh.h"
#include "actions/image_io.h"
#include "actions/mesh_io.h"

namespace mop {

using ConvertActionsList =
    vcl::TypeWrapper<PolyEdgeMeshConvert, TriEdgeMeshConvert>;

using FilterActionsList = vcl::TypeWrapper<
    // apply filters
    LaplacianSmoothingFilter,

    // clean filters
    RemoveDegenerateFacesFilter,
    RemoveDegenerateVerticesFilter,
    RemoveDuplicateFacesFilter,
    RemoveDuplicateVerticesFilter,
    RemoveUnreferencedVerticesFilter,

    // create filters
    CreateConeFilter,

    // generate filters
    ConvexHullFilter
#if defined(VCLIB_WITH_CGAL) && defined(VCLIB_WITH_BOOST)
    ,
    // boolean filters
    UnionFilter,
    IntersectionFilter,
    DifferenceFilter,
    XorFilter,
    ResolveFilter
#endif
    >;

using ImageIOActionsList =
    vcl::TypeWrapper<BaseImageIO>;

using MeshIOActionsList =
    vcl::TypeWrapper<BaseMeshIO, GltfMeshIO>;

} // namespace mop

#endif // MOP_CORE_ACTIONS_ACTION_TYPES_LISTS_H
