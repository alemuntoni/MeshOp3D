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

#ifndef HLMP_ACTIONS_INTERFACES_FILTER_ACTION_H
#define HLMP_ACTIONS_INTERFACES_FILTER_ACTION_H

#include "action.h"

#include <hlmp/parameter_vector.h>
#include <hlmp/parameters.h>

#include <vclib/algorithms/mesh.h>

namespace hlmp {

class FilterAction : public Action
{
public:
    enum class Category {
        CREATE = 0,
        CLEANING_AND_REPAIRING,
        RECONSTRUCTION,
        SMOOTHING,

        COUNT,
    };

    using CategoryBitSet = vcl::BitSet<vcl::uint>;

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    /**
     * @brief Returns the categories of the filter.
     *
     * Returns a BitSet that contains, for each category listed in the
     * Category enum, whether the action belongs to that category.
     *
     * @return The categories of the action.
     */
    virtual CategoryBitSet categories() const = 0;

    /**
     * @brief Returns the description of the filter.
     *
     * @return The description of the filter.
     */
    virtual std::string description() const = 0;

    /**
     * @brief Returns a vector indicating the input meshes that the filter
     * requires, and their description.
     *
     * Input meshes are the meshes that the filter will use to perform its
     * operation, but that will not be modified (they will be const, and the
     * applyFilter function will receive them in a const vector of const
     * meshes).
     *
     * The UintParameter objects in the returned vector must contain the name
     * and description of the input meshes. The uint value contained in the
     * UintParameter object won't be used. The number of input mesh requires is
     * the size of the returned vector.
     *
     * @return The input meshes for the filter.
     */
    virtual std::vector<UintParameter> inputMeshes() const = 0;

    /**
     * @brief Returns a vector indicating the number of input/output meshes that
     * the filter requires, and their description.
     *
     * Input/Output meshes are the meshes that the filter will use to perform
     * its operation, and that will be modified (they will be non-const, and the
     * applyFilter function will receive them in a const vector of non-const
     * meshes).
     *
     * The UintParameter objects in the returned vector must contain the name
     * and description of the input/output meshes. The uint value contained in
     * the UintParameter object won't be used. The number of input/output mesh
     * requires is the size of the returned vector.
     *
     * @return The input/output meshes for the filter.
     */
    virtual std::vector<UintParameter> inputOutputMeshes() const = 0;

    /**
     * @brief Returns the parameters of the filter.
     *
     * By default, the filter has no parameters.
     *
     * You should override this method if your filter requires
     * parameters.
     *
     * @return The parameters for the filter.
     */
    virtual ParameterVector parameters() const { return ParameterVector(); }

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    /**
     * @brief Returns the supported mesh types for this action.
     */
    virtual vcl::BitSet32 supportedMeshTypes() const = 0;

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    Type type() const final { return Type::FILTER_ACTION; }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes,
        std::vector<MeshType>&              outputMeshes,
        const ParameterVector&              parameters,
        vcl::AbstractLogger&                log = logger()) const
    {
        checkInputVectors(inputMeshes, inputOutputMeshes);
        
        std::vector<const void*> inV;
        inV.reserve(inputMeshes.size());
        for (const auto* m : inputMeshes) inV.push_back(m);
        
        std::vector<void*> inOutV;
        inOutV.reserve(inputOutputMeshes.size());
        for (auto* m : inputOutputMeshes) inOutV.push_back(m);
        
        std::vector<void*> outV;
        
        auto res = executeErased(
            meshTypeId<MeshType>(), inV, inOutV, outV, parameters, log);
            
        for (void* ptr : outV) {
            MeshType* m = static_cast<MeshType*>(ptr);
            outputMeshes.push_back(std::move(*m));
            delete m;
        }
        
        for (MeshType* m : inputOutputMeshes) {
            postExecute(*m);
        }
        for (MeshType& m : outputMeshes) {
            postExecute(m);
        }
        
        return res;
    }

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

    template<vcl::MeshConcept MeshType>
    OutputValues execute(
        const std::vector<const MeshType*>& inputMeshes,
        std::vector<MeshType>&              outputMeshes,
        vcl::AbstractLogger&                log = logger()) const
    {
        return execute(inputMeshes, outputMeshes, parameters(), log);
    }

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

protected:
    virtual OutputValues executeErased(
        MeshTypeId                      typeId,
        const std::vector<const void*>& inputMeshes,
        std::vector<void*>&             inputOutputMeshes,
        std::vector<void*>&             outputMeshes,
        const ParameterVector&          parameters,
        vcl::AbstractLogger&            log) const = 0;
    void checkInputMeshes(vcl::uint provided) const
    {
        vcl::uint n = inputMeshes().size();
        if (n != provided) {
            throw std::runtime_error(
                "The action " + name() + " requires " + std::to_string(n) +
                " input meshes, but " + std::to_string(provided) +
                " was provided. Use a different execute overload.");
        }
    }

    void checkInputOutputMeshes(vcl::uint provided) const
    {
        vcl::uint n = inputOutputMeshes().size();
        if (n != provided) {
            throw std::runtime_error(
                "The action " + name() + " requires " + std::to_string(n) +
                " input/output meshes, but " + std::to_string(provided) +
                " was provided. Use a different execute overload.");
        }
    }

    template<vcl::MeshConcept MeshType>
    void warnOutputMeshesVector(
        const std::vector<MeshType>& outputMeshes,
        vcl::AbstractLogger&         log) const
    {
        if (!outputMeshes.empty()) {
            log.log(
                "The action " + name() +
                    " returned output meshes, but an "
                    "outputMeshes vector was not provided to the execute "
                    "function.",
                log.WARNING_LOG);
        }
    }

    template<vcl::MeshConcept MeshType>
    void checkInputVectors(
        const std::vector<const MeshType*>& inputMeshes,
        const std::vector<MeshType*>&       inputOutputMeshes) const
    {
        checkInputMeshes(inputMeshes.size());
        checkInputOutputMeshes(inputOutputMeshes.size());
    }

    template<vcl::MeshConcept MeshType>
    void postExecute(MeshType& mesh) const
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            vcl::updatePerVertexAndFaceNormals(mesh);
        }
        vcl::updateBoundingBox(mesh);
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_INTERFACES_FILTER_ACTION_H
