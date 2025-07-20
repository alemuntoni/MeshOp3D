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

#ifndef HLMP_ACTIONS_AGGREGATORS_FILTER_ACTIONS_H
#define HLMP_ACTIONS_AGGREGATORS_FILTER_ACTIONS_H

#include "detail/fill_actions.h"

#include <hlmp/actions/interfaces/filter_action_t.h>

namespace hlmp {

class FilterActions : public FilterAction
{
    static const vcl::uint MESH_TYPE_NUMBER =
        vcl::toUnderlying(MeshTypeId::COUNT);

    std::array<std::shared_ptr<FilterAction>, MESH_TYPE_NUMBER> mFilterActions;
    vcl::uint mFirstMeshType = MESH_TYPE_NUMBER;

public:
    template<template<typename> typename Act>
    void fillWithSupportedMeshTypes()
    {
        detail::fillWithSupportedMeshTypes<Act>(mFilterActions, mFirstMeshType);
    }

    std::string name() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->name();
    }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }

    CategoryBitSet categories() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->categories();
    }

    std::string description() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->description();
    }

    std::vector<UintParameter> inputMeshes() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->inputMeshes();
    }

    std::vector<UintParameter> inputOutputMeshes() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->inputOutputMeshes();
    }

    ParameterVector parameters() const final
    {
        checkActionHasBeenFilled();
        return mFilterActions[mFirstMeshType]->parameters();
    }

    vcl::BitSet32 supportedMeshTypes() const
    {
        checkActionHasBeenFilled();
        vcl::BitSet32 bitset;
        for (vcl::uint i = 0; i < MESH_TYPE_NUMBER; i++) {
            if (mFilterActions[i] != nullptr) {
                bitset[i] = true;
            }
        }
        return bitset;
    }

    template<vcl::MeshConcept MeshType>
    std::shared_ptr<FilterActionT<MeshType>> action() const
    {
        checkActionForMeshType<MeshType>();
        return std::dynamic_pointer_cast<FilterActionT<MeshType>>(
            mFilterActions[vcl::toUnderlying(meshTypeId<MeshType>())]);
    }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        vcl::AbstractLogger&                log = logger()) const
    {
        checkActionForMeshType<MeshType>();
        return action<MeshType>()->execute(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters, log);
    }

    // without parameters override
    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        vcl::AbstractLogger&                log = logger()) const
    {
        return execute(
            inputMeshes, inputOutputMeshes, outputMeshes, parameters(), log);
    }

    // without inputOutputMeshes override
    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        vcl::AbstractLogger&                log = logger()) const
    {
        checkInputOutputMeshes(0);
        return execute(inputMeshes, {}, outputMeshes, parameters, log);
    }

    // without inputOutputMeshes and parameters override
    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        std::vector<MeshType>&              outputMeshes,
        vcl::AbstractLogger&                log = logger()) const
    {
        return execute(inputMeshes, outputMeshes, parameters(), log);
    }

    // without inputOutputMeshes and outputMeshes override
    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const ParameterVector&              parameters,
        vcl::AbstractLogger&                log = logger()) const
    {
        std::vector<MeshType> outputMeshes;
        auto out = execute(inputMeshes, outputMeshes, parameters, log);
        warnOutputMeshesVector(outputMeshes, log);
        return out;
    }

    // without inputOutputMeshes, outputMeshes and parameters override
    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        vcl::AbstractLogger&                log = logger()) const
    {
        return execute(inputMeshes, parameters(), log);
    }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&        outputMeshes,
        const ParameterVector&        parameters,
        vcl::AbstractLogger&          log = logger()) const
    {
        return execute({}, inputOutputMeshes, outputMeshes, parameters, log);
    }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&        outputMeshes,
        vcl::AbstractLogger&          log = logger()) const
    {
        return execute(inputOutputMeshes, outputMeshes, parameters(), log);
    }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        const ParameterVector&        parameters,
        vcl::AbstractLogger&          log = logger()) const
    {
        std::vector<MeshType> outputMeshes;
        auto out = execute(inputOutputMeshes, outputMeshes, parameters, log);
        warnOutputMeshesVector(outputMeshes, log);
        return out;
    }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<MeshType*>& inputOutputMeshes,
        vcl::AbstractLogger&          log = logger()) const
    {
        return execute(inputOutputMeshes, parameters(), log);
    }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        std::vector<MeshType>& outputMeshes,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log = logger()) const
    {
        return execute(
            std::vector<const MeshType*>(),
            std::vector<MeshType*>(),
            outputMeshes,
            parameters,
            log);
    }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        std::vector<MeshType>& outputMeshes,
        vcl::AbstractLogger&   log = logger()) const
    {
        return execute(outputMeshes, parameters(), log);
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
        if (mFilterActions[id] == nullptr) {
            throw std::runtime_error(
                "The action cannot be instantiated for the given MeshType.");
        }
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_AGGREGATORS_FILTER_ACTIONS_H
