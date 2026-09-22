// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "scalar_parameter_row.h"

#include <QDoubleValidator>

namespace mop {

ScalarParameterRow::ScalarParameterRow(const ScalarParameter& param) :
        ParameterRow(param), mParam(param)
{
    mLineEdit = new QLineEdit();
    mLineEdit->setToolTip(param.tooltip().c_str());
    mLineEdit->setValidator(new QDoubleValidator(
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::max(),
        4,
        mLineEdit));
    mLineEdit->setText(QString::number(param.scalarValue()));
}

QWidget* ScalarParameterRow::parameterWidget()
{
    return mLineEdit;
}

std::shared_ptr<Parameter> ScalarParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setScalarValue(mLineEdit->text().toDouble());
    return p;
}

} // namespace mop
