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

#ifndef HLMP_ACTIONS_INTERFACES_MESH_IO_ACTION_BASE_H
#define HLMP_ACTIONS_INTERFACES_MESH_IO_ACTION_BASE_H

#include "mesh_io_action.h"

#include <vclib/mesh.h>
#include <vclib/algorithms/mesh/update.h>

#include <functional>
#include <array>

namespace hlmp {

namespace detail {
    template<typename Derived, typename MeshType>
    concept HasLoad = requires(
        const Derived* d,
        const std::string& filename,
        const vcl::FileFormat& format,
        const ParameterVector& parameters,
        vcl::MeshInfo& loadedInfo,
        vcl::AbstractLogger& log)
    {
        d->template load<MeshType>(filename, format, parameters, loadedInfo, log);
    };

    template<typename Derived, typename MeshType>
    concept HasSave = requires(
        const Derived* d,
        const std::string& filename,
        const vcl::FileFormat& format,
        const MeshType& mesh,
        const vcl::MeshInfo& info,
        const ParameterVector& parameters,
        vcl::AbstractLogger& log)
    {
        d->template save<MeshType>(filename, format, mesh, info, parameters, log);
    };
} // namespace detail

template <typename Derived>
class MeshIOActionBase : public MeshIOAction
{
    using LoadExecutorFunc = std::function<std::any(
        const std::string&,
        const vcl::FileFormat&,
        const ParameterVector&,
        vcl::MeshInfo&,
        vcl::AbstractLogger&)>;

    using SaveExecutorFunc = std::function<void(
        const std::string&,
        const vcl::FileFormat&,
        const std::any&,
        const vcl::MeshInfo&,
        const ParameterVector&,
        vcl::AbstractLogger&)>;

    static const vcl::uint MESH_TYPE_NUMBER = vcl::toUnderlying(MeshTypeId::COUNT);
    std::array<LoadExecutorFunc, MESH_TYPE_NUMBER> mLoadExecutors;
    std::array<SaveExecutorFunc, MESH_TYPE_NUMBER> mSaveExecutors;
    vcl::BitSet32 mSupportedMeshTypes;

public:
    MeshIOActionBase()
    {
        auto act = [&]<typename MeshType>() {
            vcl::uint id = vcl::toUnderlying(meshTypeId<MeshType>());
            bool supported = false;

            if constexpr (detail::HasLoad<Derived, MeshType>) {
                supported = true;
                mLoadExecutors[id] = [this](
                    const std::string&     filename,
                    const vcl::FileFormat& format,
                    const ParameterVector& parameters,
                    vcl::MeshInfo&         loadedInfo,
                    vcl::AbstractLogger&   log) -> std::any
                {
                    return std::any(static_cast<const Derived*>(this)->template load<MeshType>(
                        filename, format, parameters, loadedInfo, log));
                };
            }

            if constexpr (detail::HasSave<Derived, MeshType>) {
                supported = true;
                mSaveExecutors[id] = [this](
                    const std::string&     filename,
                    const vcl::FileFormat& format,
                    const std::any&        meshAny,
                    const vcl::MeshInfo&   info,
                    const ParameterVector& parameters,
                    vcl::AbstractLogger&   log)
                {
                    const MeshType* const* meshPtrPtr = std::any_cast<const MeshType*>(&meshAny);
                    if (!meshPtrPtr || !(*meshPtrPtr)) {
                        throw std::runtime_error("Invalid mesh pointer inside std::any.");
                    }
                    static_cast<const Derived*>(this)->template save<MeshType>(
                        filename, format, **meshPtrPtr, info, parameters, log);
                };
            }

            if (supported) {
                mSupportedMeshTypes[id] = true;
            }
        };

        vcl::ForEachType<MeshTypes>::apply(act);
    }

    MeshTypeId meshType() const override { return MeshTypeId::COUNT; }

    vcl::BitSet32 supportedMeshTypes() const override
    {
        return mSupportedMeshTypes;
    }

protected:
    std::any loadErased(
        MeshTypeId             typeId,
        const std::string&     filename,
        const vcl::FileFormat& format,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        vcl::AbstractLogger&   log) const override
    {
        vcl::uint id = vcl::toUnderlying(typeId);
        if (!mSupportedMeshTypes[id] || !mLoadExecutors[id]) {
            throw std::runtime_error("Action " + name() + " does not support loading the given mesh type.");
        }
        return mLoadExecutors[id](filename, format, parameters, loadedInfo, log);
    }

    void saveErased(
        MeshTypeId             typeId,
        const std::string&     filename,
        const vcl::FileFormat& format,
        const std::any&        mesh,
        const vcl::MeshInfo&   info,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log) const override
    {
        vcl::uint id = vcl::toUnderlying(typeId);
        if (!mSupportedMeshTypes[id] || !mSaveExecutors[id]) {
            throw std::runtime_error("Action " + name() + " does not support saving the given mesh type.");
        }
        mSaveExecutors[id](filename, format, mesh, info, parameters, log);
    }

    template<typename MeshType>
    void postLoad(MeshType& mesh, const vcl::MeshInfo& loadedInfo) const
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            if (!loadedInfo.hasPerFaceNormal()) {
                vcl::updatePerFaceNormals(mesh);
            }
            if (!loadedInfo.hasPerVertexNormal()) {
                vcl::updatePerVertexNormalsFromFaceNormals(mesh);
            }
        }
        vcl::updateBoundingBox(mesh);
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_INTERFACES_MESH_IO_ACTION_BASE_H
