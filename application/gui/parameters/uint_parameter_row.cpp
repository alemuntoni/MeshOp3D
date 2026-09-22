// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "uint_parameter_row.h"

#include <QIntValidator>

namespace mop {

UintParameterRow::UintParameterRow(const UintParameter& param) :
        ParameterRow(param), mParam(param)
{
    mLineEdit = new QLineEdit();
    mLineEdit->setToolTip(param.tooltip().c_str());
    mLineEdit->setValidator(
        new QIntValidator(0, std::numeric_limits<int>::max(), mLineEdit));
    mLineEdit->setText(QString::number(param.uintValue()));
}

QWidget* UintParameterRow::parameterWidget()
{
    return mLineEdit;
}

std::shared_ptr<Parameter> UintParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setUintValue(mLineEdit->text().toUInt());
    return p;
}

} // namespace mop
