// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_PARAMETERS_USCALAR_PARAMETER_ROW_H
#define MOP_APPLICATION_GUI_PARAMETERS_USCALAR_PARAMETER_ROW_H

#include <QLineEdit>

#include "parameter_row.h"

namespace mop {

class UscalarParameterRow : public ParameterRow
{
    UscalarParameter mParam;

    QLineEdit* mLineEdit = nullptr;

public:
    UscalarParameterRow(const UscalarParameter& param);

    // ParameterRow interface
    QWidget* parameterWidget() override;

    std::shared_ptr<Parameter> parameterFromWidget() const override;
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETERS_USCALAR_PARAMETER_ROW_H
