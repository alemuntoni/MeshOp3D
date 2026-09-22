// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_PARAMETER_VECTOR_H
#define MOP_CORE_PARAMETER_VECTOR_H

#include "base/polymorphic_object_vector.h"
#include "parameters/parameter.h"

namespace mop {

class ParameterVector : public PolymorphicObjectVector<Parameter>
{
public:
    std::shared_ptr<const Parameter> get(const std::string& name) const
    {
        for (const auto& parameter : *this) {
            if (parameter->name() == name) {
                return parameter;
            }
        }

        return nullptr;
    }

    std::shared_ptr<Parameter> get(const std::string& name)
    {
        for (auto& parameter : *this) {
            if (parameter->name() == name) {
                return parameter;
            }
        }

        return nullptr;
    }
};

using OutputValues = ParameterVector;

} // namespace mop

#endif // MOP_CORE_PARAMETER_VECTOR_H
