// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_PARAMETERS_ENUM_PARAMETER_ROW_H
#define MOP_APPLICATION_GUI_PARAMETERS_ENUM_PARAMETER_ROW_H

#include <QComboBox>

#include "parameter_row.h"

namespace mop {

class EnumParameterRow : public ParameterRow
{
    EnumParameter mParam;

    QComboBox* mComboBox = nullptr;

public:
    EnumParameterRow(const EnumParameter& param);

    // ParameterRow interface
    QWidget* parameterWidget() override;

    std::shared_ptr<Parameter> parameterFromWidget() const override;
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETERS_ENUM_PARAMETER_ROW_H
