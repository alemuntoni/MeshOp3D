// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "enum_parameter_row.h"

#include <QStandardItemModel>

namespace mop {

EnumParameterRow::EnumParameterRow(const EnumParameter& param) :
        ParameterRow(param), mParam(param)
{
    mComboBox = new QComboBox();
    mComboBox->setToolTip(param.tooltip().c_str());

    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(mComboBox->model());
    assert(model != nullptr);

    for (uint i = 0; const auto& value : param.enumValues()) {
        mComboBox->addItem(value.c_str());
        QStandardItem* item = model->item(i);
        if (param.isEnabled(i)) {
            item->setFlags(item->flags() | Qt::ItemIsEnabled);
        }
        else {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
        ++i;
    }

    mComboBox->setCurrentIndex(param.uintValue());
}

QWidget* EnumParameterRow::parameterWidget()
{
    return mComboBox;
}

std::shared_ptr<Parameter> EnumParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setUintValue(mComboBox->currentIndex());
    return p;
}

} // namespace mop
