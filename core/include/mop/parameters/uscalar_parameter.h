// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_PARAMETERS_USCALAR_PARAMETER_H
#define MOP_CORE_PARAMETERS_USCALAR_PARAMETER_H

#include "parameter.h"

namespace mop {

class UscalarParameter : public Parameter
{
public:
    UscalarParameter(
        const std::string& name,
        ScalarType         value,
        const std::string& description = "",
        const std::string& tooltip     = "",
        const std::string& category    = "") :
            Parameter(name, 0.0, description, tooltip, category)
    {
        setScalarValue(value);
    }

    ParameterType type() const override { return ParameterType::USCALAR; }

    std::shared_ptr<Parameter> clone() const override
    {
        return std::make_shared<UscalarParameter>(*this);
    }

    void setScalarValue(ScalarType value) override
    {
        checkScalarValue(value);
        Parameter::setScalarValue(value);
    }

private:
    void checkScalarValue(ScalarType value) const
    {
        if (value < 0.0)
            throw std::runtime_error(
                "UscalarParameter: value cannot be negative");
    }
};

} // namespace mop

#endif // MOP_CORE_PARAMETERS_USCALAR_PARAMETER_H
