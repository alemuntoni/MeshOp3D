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

#ifndef HLMP_ACTIONS_AGGREGATORS_MESH_IO_ACTIONS_AGGREGATOR_H
#define HLMP_ACTIONS_AGGREGATORS_MESH_IO_ACTIONS_AGGREGATOR_H

#include "detail/aggregators_utils.h"

#include <hlmp/actions/interfaces/mesh_io_action_t.h>

namespace hlmp {

/**
 * @brief The MeshIOActionsAggregator class is a special class that aggregates
 * MeshIOActionT instances for different mesh types (only the ones that are
 * supported by the MeshIOActionT).
 */
class MeshIOActionsAggregator : public MeshIOAction
{
    static const vcl::uint MESH_TYPE_NUMBER =
        vcl::toUnderlying(MeshTypeId::COUNT);

    std::array<std::shared_ptr<MeshIOAction>, MESH_TYPE_NUMBER> mMeshIOActions;
    vcl::uint mFirstMeshType = MESH_TYPE_NUMBER;

public:
    template<template<typename> typename Act>
    void fillWithSupportedMeshTypes()
    {
        detail::fillActionArrayWithSupportedMeshTypes<Act>(
            mMeshIOActions, mFirstMeshType);
    }

    std::string name() const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->name();
    }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }

    IOSupport ioSupport() const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->ioSupport();
    }

    std::vector<std::pair<vcl::FileFormat, vcl::MeshInfo>> supportedMeshFormats()
        const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->supportedMeshFormats();
    }

    ParameterVector parametersLoad(const vcl::FileFormat& format) const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->parametersLoad(format);
    }

    ParameterVector parametersSave(const vcl::FileFormat& format) const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->parametersSave(format);
    }

    vcl::BitSet32 supportedMeshTypes() const
    {
        checkActionHasBeenFilled();
        vcl::BitSet32 bitset;
        for (vcl::uint i = 0; i < MESH_TYPE_NUMBER; i++) {
            if (mMeshIOActions[i] != nullptr) {
                bitset[i] = true;
            }
        }
        return bitset;
    }

    template<vcl::MeshConcept MeshType>
    std::shared_ptr<MeshIOActionT<MeshType>> action() const
    {
        checkActionHasBeenFilled();
        checkActionForMeshType<MeshType>();
        return std::dynamic_pointer_cast<MeshIOActionT<MeshType>>(
            mMeshIOActions[vcl::toUnderlying(meshTypeId<MeshType>())]);
    }

    template<vcl::MeshConcept MeshType>
    MeshType load(
        const std::string&     filename,
        const vcl::FileFormat& format,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        vcl::AbstractLogger&   log = logger()) const
    {
        checkActionForMeshType<MeshType>(true);
        return action<MeshType>()->load(
            filename, format, parameters, loadedInfo, log);
    }

    template<vcl::MeshConcept MeshType>
    MeshType load(
        const std::string&     filename,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log = logger()) const
    {
        vcl::MeshInfo   info;
        vcl::FileFormat format(vcl::FileInfo::extension(filename));
        auto mesh = load<MeshType>(filename, format, parameters, info, log);
        return mesh;
    }

    template<vcl::MeshConcept MeshType>
    MeshType load(
        const std::string&   filename,
        vcl::AbstractLogger& log = logger()) const
    {
        vcl::FileFormat format(vcl::FileInfo::extension(filename));
        return load<MeshType>(filename, parametersLoad(format), log);
    }

    template<vcl::MeshConcept MeshType>
    void save(
        const std::string&     filename,
        const vcl::FileFormat& format,
        const MeshType&        mesh,
        const vcl::MeshInfo&   info,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log = logger()) const
    {
        checkActionForMeshType<MeshType>(false, true);
        action<MeshType>()->save(filename, format, mesh, info, parameters, log);
    }

    template<vcl::MeshConcept MeshType>
    void save(
        const std::string&     filename,
        const MeshType&        mesh,
        const vcl::MeshInfo&   info,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log = logger()) const
    {
        vcl::FileFormat format(vcl::FileInfo::extension(filename));
        save(filename, format, mesh, info, parameters, log);
    }

    template<vcl::MeshConcept MeshType>
    void save(
        const std::string&   filename,
        const MeshType&      mesh,
        const vcl::MeshInfo& info,
        vcl::AbstractLogger& log = logger()) const
    {
        vcl::FileFormat format(vcl::FileInfo::extension(filename));
        save(filename, mesh, info, parametersSave(format), log);
    }

    template<vcl::MeshConcept MeshType>
    void save(
        const std::string&     filename,
        const MeshType&        mesh,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log = logger()) const
    {
        vcl::FileFormat format(vcl::FileInfo::extension(filename));
        save(filename, mesh, formatCapability(format), parameters, log);
    }

    template<vcl::MeshConcept MeshType>
    void save(
        const std::string&   filename,
        const MeshType&      mesh,
        vcl::AbstractLogger& log = logger()) const
    {
        vcl::FileFormat format(vcl::FileInfo::extension(filename));
        save(filename, mesh, parametersSave(format), log);
    }

private:
    void checkActionHasBeenFilled() const
    {
        if (mFirstMeshType >= MESH_TYPE_NUMBER) {
            throw std::runtime_error("The action has not been filled.");
        }
    }

    template<vcl::MeshConcept MeshType>
    void checkActionForMeshType(bool load = false, bool save = false) const
    {
        checkActionHasBeenFilled();
        checkMeshTypeId<MeshType>();
        vcl::uint id = vcl::toUnderlying(meshTypeId<MeshType>());
        if (mMeshIOActions[id] == nullptr) {
            throw std::runtime_error(
                "The action cannot be instantiated for the given MeshType.");
        }
        if (load && mMeshIOActions[id]->ioSupport() == IOSupport::SAVE) {
            throw std::runtime_error(
                "The action does not support loading meshes.");
        }
        if (save && mMeshIOActions[id]->ioSupport() == IOSupport::LOAD) {
            throw std::runtime_error(
                "The action does not support saving meshes.");
        }
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_AGGREGATORS_MESH_IO_ACTIONS_AGGREGATOR_H
