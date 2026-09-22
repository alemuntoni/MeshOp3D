// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_INTERFACES_CONVERT_ACTION_H
#define MOP_CORE_ACTIONS_INTERFACES_CONVERT_ACTION_H

#include "action.h"
#include <any>

namespace mop {

class ConvertAction : public Action
{
public:
    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    virtual MeshTypeId meshType() const = 0;

    virtual std::pair<MeshTypeId, std::any> convertErased(
        MeshTypeId           inputMeshType,
        const std::any&      inputMesh,
        vcl::AbstractLogger& log) const = 0;

    virtual vcl::BitSet32 supportedMeshTypes() const = 0;

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    Type type() const final { return Type::CONVERT_ACTION; }
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_INTERFACES_CONVERT_ACTION_H
