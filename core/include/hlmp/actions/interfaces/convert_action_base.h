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

#ifndef HLMP_ACTIONS_INTERFACES_CONVERT_ACTION_BASE_H
#define HLMP_ACTIONS_INTERFACES_CONVERT_ACTION_BASE_H

#include "convert_action.h"

#include <vclib/mesh.h>

#include <functional>
#include <array>

namespace hlmp {

namespace detail {
    template<typename Derived, typename MeshType>
    concept HasConvert = requires(
        const Derived* d,
        const MeshType& inputMesh,
        vcl::AbstractLogger& log)
    {
        d->template convert<MeshType>(inputMesh, log);
    };
} // namespace detail

template <typename Derived>
class ConvertActionBase : public ConvertAction
{
    using ExecutorFunc = std::function<std::pair<MeshTypeId, std::any>(
        const std::any&,
        vcl::AbstractLogger&)>;

    static const vcl::uint MESH_TYPE_NUMBER = vcl::toUnderlying(MeshTypeId::COUNT);
    std::array<ExecutorFunc, MESH_TYPE_NUMBER> mExecutors;
    vcl::BitSet32 mSupportedMeshTypes;

public:
    ConvertActionBase()
    {
        auto cAct = [&]<typename MeshType>() {
            if constexpr (detail::HasConvert<Derived, MeshType>) {
                vcl::uint id = vcl::toUnderlying(meshTypeId<MeshType>());
                mSupportedMeshTypes[id] = true;
                
                mExecutors[id] = [this](
                    const std::any&      inputMeshAny,
                    vcl::AbstractLogger& log) -> std::pair<MeshTypeId, std::any>
                {
                    const MeshType* const* inputMeshPtrPtr = std::any_cast<const MeshType*>(&inputMeshAny);
                    if (!inputMeshPtrPtr || !(*inputMeshPtrPtr)) {
                        throw std::runtime_error("Invalid mesh pointer inside std::any.");
                    }
                    return static_cast<const Derived*>(this)->template convert<MeshType>(**inputMeshPtrPtr, log);
                };
            }
        };

        vcl::ForEachType<MeshTypes>::apply(cAct);
    }

    MeshTypeId meshType() const override { return MeshTypeId::COUNT; }

    vcl::BitSet32 supportedMeshTypes() const override
    {
        return mSupportedMeshTypes;
    }

protected:
    std::pair<MeshTypeId, std::any> convertErased(
        MeshTypeId           inputMeshType,
        const std::any&      inputMesh,
        vcl::AbstractLogger& log) const override
    {
        vcl::uint id = vcl::toUnderlying(inputMeshType);
        if (!mSupportedMeshTypes[id] || !mExecutors[id]) {
            throw std::runtime_error("Action " + name() + " does not support the given mesh type.");
        }
        return mExecutors[id](inputMesh, log);
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_INTERFACES_CONVERT_ACTION_BASE_H
