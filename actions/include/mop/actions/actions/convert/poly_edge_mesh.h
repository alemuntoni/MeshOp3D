// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_ACTIONS_ACTIONS_ACTIONS_CONVERT_POLY_EDGE_MESH_H
#define MOP_ACTIONS_ACTIONS_ACTIONS_CONVERT_POLY_EDGE_MESH_H

#include <mop/actions/interfaces/convert_action_base.h>

namespace mop {

class PolyEdgeMeshConvert : public ConvertActionBase<PolyEdgeMeshConvert>
{
    using Base = ConvertActionBase<PolyEdgeMeshConvert>;

public:
    std::string name() const final { return "Convert to PolyEdgeMesh"; }

    template<vcl::MeshConcept MeshType>
    std::pair<MeshTypeId, std::any> convert(
        const MeshType&      inputMesh,
        vcl::AbstractLogger& log) const
    {
        using PolyEdgeMeshType = GetMeshType<MeshTypeId::POLYGON_MESH>;

        PolyEdgeMeshType polyEdgeMesh;
        polyEdgeMesh.enableSameOptionalComponentsOf(inputMesh);
        polyEdgeMesh.importFrom(inputMesh);

        return {MeshTypeId::POLYGON_MESH, std::any(polyEdgeMesh)};
    }
};

} // namespace mop

#endif // MOP_ACTIONS_ACTIONS_ACTIONS_CONVERT_POLY_EDGE_MESH_H
