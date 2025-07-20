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

#ifndef HLMP_ACTIONS_AGGREGATORS_DETAIL_AGGREGATORS_UTILS_H
#define HLMP_ACTIONS_AGGREGATORS_DETAIL_AGGREGATORS_UTILS_H

#include <hlmp/settings.h>

#include <vclib/types.h>

namespace hlmp::detail {

template<template<typename> typename Act>
void fillActionArrayWithSupportedMeshTypes(
    auto&      array,
    vcl::uint& firstMeshType)
{
    firstMeshType = vcl::toUnderlying(MeshTypeId::COUNT);

    vcl::uint i    = 0;
    auto fAct = [&]<typename MeshType>() {
        if constexpr (vcl::IsInstantiable<Act, MeshType>) {
            array[vcl::toUnderlying(meshTypeId<MeshType>())] =
                std::make_shared<Act<MeshType>>();
            if (i < firstMeshType) {
                firstMeshType = i;
            }
        }
        ++i;
    };

    vcl::ForEachType<MeshTypes>::apply(fAct);
}

} // namespace hlmp::detail

#endif // HLMP_ACTIONS_AGGREGATORS_DETAIL_AGGREGATORS_UTILS_H
