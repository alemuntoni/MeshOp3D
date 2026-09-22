// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_PARAMETERS_BOOL_PARAMETER_H
#define MOP_CORE_PARAMETERS_BOOL_PARAMETER_H

#include "parameter.h"

namespace mop {

class BoolParameter : public Parameter
{
public:
    BoolParameter(
        const std::string& name,
        bool               value,
        const std::string& description = "",
        const std::string& tooltip     = "",
        const std::string& category    = "") :
            Parameter(name, value, description, tooltip, category)
    {
    }

    ParameterType type() const override { return ParameterType::BOOL; }

    std::shared_ptr<Parameter> clone() const override
    {
        return std::make_shared<BoolParameter>(*this);
    }
};

} // namespace mop

#endif // MOP_CORE_PARAMETERS_BOOL_PARAMETER_H
