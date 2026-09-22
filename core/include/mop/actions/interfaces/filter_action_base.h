// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_INTERFACES_FILTER_ACTION_BASE_H
#define MOP_CORE_ACTIONS_INTERFACES_FILTER_ACTION_BASE_H

#include "filter_action.h"

#include <functional>
#include <array>

namespace mop {

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

} // namespace mop

#endif // MOP_CORE_ACTIONS_INTERFACES_FILTER_ACTION_BASE_H
