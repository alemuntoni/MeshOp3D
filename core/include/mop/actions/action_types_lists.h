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

#ifndef MOP_ACTIONS_ACTION_TYPES_LISTS_H
#define MOP_ACTIONS_ACTION_TYPES_LISTS_H

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
    ConvexHullFilter>;

using ImageIOActionsList =
    vcl::TypeWrapper<BaseImageIO>;

using MeshIOActionsList =
    vcl::TypeWrapper<BaseMeshIO, GltfMeshIO>;

} // namespace mop

#endif // MOP_ACTIONS_ACTION_TYPES_LISTS_H
