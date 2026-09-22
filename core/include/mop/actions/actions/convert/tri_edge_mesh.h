// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_ACTIONS_CONVERT_TRI_EDGE_MESH_H
#define MOP_CORE_ACTIONS_ACTIONS_CONVERT_TRI_EDGE_MESH_H

#include <mop/actions/interfaces/convert_action_base.h>

namespace mop {

class TriEdgeMeshConvert : public ConvertActionBase<TriEdgeMeshConvert>
{
    using Base = ConvertActionBase<TriEdgeMeshConvert>;

public:
    std::string name() const final { return "Convert to TriEdgeMesh"; }

    template<vcl::MeshConcept MeshType>
    std::pair<MeshTypeId, std::any> convert(
        const MeshType&      inputMesh,
        vcl::AbstractLogger& log) const
    {
        using TriEdgeMeshType = GetMeshType<MeshTypeId::TRIANGLE_MESH>;

        TriEdgeMeshType triEdgeMesh;
        triEdgeMesh.enableSameOptionalComponentsOf(inputMesh);
        triEdgeMesh.importFrom(inputMesh);

        return {MeshTypeId::TRIANGLE_MESH, std::any(triEdgeMesh)};
    }
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_ACTIONS_CONVERT_TRI_EDGE_MESH_H
