// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_SETTINGS_H
#define MOP_CORE_SETTINGS_H

#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/meshes.h>

namespace mop {

using uint = vcl::uint;

/**
 * @brief Scalar type used in MeshOp3D.
 */
using ScalarType = double;

/**
 * @brief Flag that indicates if the meshes are indexed.
 */
constexpr bool INDEXED_MESHES = false;

enum class MeshTypeId : uint {
    TRIANGLE_MESH = 0,
    POLYGON_MESH  = 1,

    COUNT
};

/**
 * @brief List of mesh types supported by MeshOp3D.
 */
using MeshTypes = vcl::TypeWrapper<
    vcl::TriEdgeMeshT<ScalarType, INDEXED_MESHES>,
    vcl::PolyEdgeMeshT<ScalarType, INDEXED_MESHES>>;

template<MeshTypeId MESH_ID>
using GetMeshType = vcl::TypeAt<vcl::toUnderlying(MESH_ID), MeshTypes>::type;

template<typename MeshType>
constexpr MeshTypeId meshTypeId()
{
    constexpr uint id = vcl::IndexInTypes<MeshType, MeshTypes>::value;
    if constexpr (id == vcl::UINT_NULL) {
        return MeshTypeId::COUNT;
    }
    return static_cast<MeshTypeId>(id);
}

template<typename MeshType>
constexpr void checkMeshTypeId()
{
    constexpr uint id = vcl::toUnderlying(meshTypeId<MeshType>());
    static_assert(id != vcl::UINT_NULL, "Mesh type not supported.");
    static_assert(
        id >= 0 && id < vcl::toUnderlying(MeshTypeId::COUNT),
        "Invalid mesh type id.");
}

constexpr std::array<std::string, vcl::toUnderlying(MeshTypeId::COUNT)>
meshTypeNames()
{
    std::array<std::string, vcl::toUnderlying(MeshTypeId::COUNT)> array = {
        vcl::meshTypeName<GetMeshType<MeshTypeId::TRIANGLE_MESH>>(),
        vcl::meshTypeName<GetMeshType<MeshTypeId::POLYGON_MESH>>()};

    return array;
}

} // namespace mop

#endif // MOP_CORE_SETTINGS_H
