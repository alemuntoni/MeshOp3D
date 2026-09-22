// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "bool_parameter_row.h"

namespace mop {

BoolParameterRow::BoolParameterRow(const BoolParameter& param) :
        ParameterRow(param), mParam(param)
{
    mCheckBox = new QCheckBox("");
    mCheckBox->setToolTip(param.tooltip().c_str());
    mCheckBox->setChecked(param.boolValue());

    QObject::connect(
        mDescriptionLabel, SIGNAL(clicked()), mCheckBox, SLOT(toggle()));
}

QWidget* BoolParameterRow::parameterWidget()
{
    return mCheckBox;
}

std::shared_ptr<Parameter> BoolParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setBoolValue(mCheckBox->isChecked());
    return p;
}

} // namespace mop
