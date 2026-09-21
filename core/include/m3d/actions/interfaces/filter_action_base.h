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

#ifndef M3D_ACTIONS_INTERFACES_FILTER_ACTION_BASE_H
#define M3D_ACTIONS_INTERFACES_FILTER_ACTION_BASE_H

#include "filter_action.h"

#include <functional>
#include <array>

namespace hlmp {

namespace detail {
    template<typename Derived, typename MeshType>
    concept HasExecuteFilter = requires(
        const Derived* d,
        const std::vector<const MeshType*>& in,
        const std::vector<MeshType*>& inOut,
        std::vector<MeshType>& out,
        const ParameterVector& params,
        vcl::AbstractLogger& log)
    {
        d->template executeFilter<MeshType>(in, inOut, out, params, log);
    };
} // namespace detail

template <typename Derived>
class FilterActionBase : public FilterAction
{
    using ExecutorFunc = std::function<OutputValues(
        const std::vector<const void*>&,
        std::vector<void*>&,
        std::vector<void*>&,
        const ParameterVector&,
        vcl::AbstractLogger&)>;

    static const vcl::uint MESH_TYPE_NUMBER = vcl::toUnderlying(MeshTypeId::COUNT);
    std::array<ExecutorFunc, MESH_TYPE_NUMBER> mExecutors;
    vcl::BitSet32 mSupportedMeshTypes;

public:
    FilterActionBase()
    {
        auto fAct = [&]<typename MeshType>() {
            if constexpr (detail::HasExecuteFilter<Derived, MeshType>) {
                vcl::uint id = vcl::toUnderlying(meshTypeId<MeshType>());
                mSupportedMeshTypes[id] = true;
                
                mExecutors[id] = [this](
                    const std::vector<const void*>& inV,
                    std::vector<void*>&             inOutV,
                    std::vector<void*>&             outV,
                    const ParameterVector&          params,
                    vcl::AbstractLogger&            log) -> OutputValues
                {
                    std::vector<const MeshType*> in;
                    in.reserve(inV.size());
                    for (const void* p : inV) in.push_back(static_cast<const MeshType*>(p));
                    
                    std::vector<MeshType*> inOut;
                    inOut.reserve(inOutV.size());
                    for (void* p : inOutV) inOut.push_back(static_cast<MeshType*>(p));
                    
                    std::vector<MeshType> out;
                    
                    OutputValues res = static_cast<const Derived*>(this)->template executeFilter<MeshType>(
                        in, inOut, out, params, log);
                        
                    outV.reserve(out.size());
                    for (auto& newMesh : out) {
                        outV.push_back(new MeshType(std::move(newMesh)));
                    }
                    
                    return res;
                };
            }
        };

        vcl::ForEachType<MeshTypes>::apply(fAct);
    }

    vcl::BitSet32 supportedMeshTypes() const override
    {
        return mSupportedMeshTypes;
    }

protected:
    OutputValues executeErased(
        MeshTypeId                      typeId,
        const std::vector<const void*>& inputMeshes,
        std::vector<void*>&             inputOutputMeshes,
        std::vector<void*>&             outputMeshes,
        const ParameterVector&          parameters,
        vcl::AbstractLogger&            log) const override
    {
        vcl::uint id = vcl::toUnderlying(typeId);
        if (!mSupportedMeshTypes[id] || !mExecutors[id]) {
            throw std::runtime_error("Action " + name() + " does not support the given mesh type.");
        }
        return mExecutors[id](inputMeshes, inputOutputMeshes, outputMeshes, parameters, log);
    }
};

} // namespace hlmp

#endif // M3D_ACTIONS_INTERFACES_FILTER_ACTION_BASE_H
