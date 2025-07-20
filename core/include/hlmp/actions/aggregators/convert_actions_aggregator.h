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

#ifndef HLMP_ACTIONS_AGGREGATORS_CONVERT_ACTIONS_AGGREGATOR_H
#define HLMP_ACTIONS_AGGREGATORS_CONVERT_ACTIONS_AGGREGATOR_H

#include "detail/aggregators_utils.h"

#include <hlmp/actions/interfaces/convert_action_t.h>

namespace hlmp {

/**
 * @brief The ConvertActionsAggregator class is a special class that aggregates
 * ConvertActionT instances for different mesh types (only the ones that are
 * supported by the ConvertActionT).
 */
class ConvertActionsAggregator : public ConvertAction
{
    static const vcl::uint MESH_TYPE_NUMBER =
        vcl::toUnderlying(MeshTypeId::COUNT);

    std::array<std::shared_ptr<ConvertAction>, MESH_TYPE_NUMBER>
         mConvertActions;
    vcl::uint mFirstMeshType = MESH_TYPE_NUMBER;

public:
    template<template<typename> typename Act>
    void fillWithSupportedMeshTypes()
    {
        detail::fillActionArrayWithSupportedMeshTypes<Act>(
            mConvertActions, mFirstMeshType);
    }

    std::string name() const final
    {
        checkActionHasBeenFilled();
        return mConvertActions[mFirstMeshType]->name();
    }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }

    vcl::BitSet32 supportedMeshTypes() const
    {
        checkActionHasBeenFilled();
        vcl::BitSet32 bitset;
        for (vcl::uint i = 0; i < MESH_TYPE_NUMBER; i++) {
            if (mConvertActions[i] != nullptr) {
                bitset[i] = true;
            }
        }
        return bitset;
    }

    template<vcl::MeshConcept MeshType>
    std::shared_ptr<ConvertActionT<MeshType>> action() const
    {
        checkActionForMeshType<MeshType>();
        return std::dynamic_pointer_cast<ConvertActionT<MeshType>>(
            mConvertActions[vcl::toUnderlying(meshTypeId<MeshType>())]);
    }

    template<vcl::MeshConcept MeshType>
    std::pair<MeshTypeId, std::any> convert(
        const MeshType& inputMesh,
        vcl::AbstractLogger& log = logger()) const
    {
        checkActionForMeshType<MeshType>();
        return action<MeshType>()->convert(inputMesh, log);
    }

private:
    void checkActionHasBeenFilled() const
    {
        if (mFirstMeshType >= MESH_TYPE_NUMBER) {
            throw std::runtime_error("The action has not been filled.");
        }
    }

    template<vcl::MeshConcept MeshType>
    void checkActionForMeshType() const
    {
        checkActionHasBeenFilled();
        checkMeshTypeId<MeshType>();
        vcl::uint id = vcl::toUnderlying(meshTypeId<MeshType>());
        if (mConvertActions[id] == nullptr) {
            throw std::runtime_error(
                "The action cannot be instantiated for the given MeshType.");
        }
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_AGGREGATORS_CONVERT_ACTIONS_AGGREGATOR_H
