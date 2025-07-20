/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include "enum_parameter_row.h"

#include <QStandardItemModel>

namespace vcl::qt {

EnumParameterRow::EnumParameterRow(const hlmp::EnumParameter& param) :
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

std::shared_ptr<hlmp::Parameter> EnumParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    p->setUintValue(mComboBox->currentIndex());
    return p;
}

} // namespace vcl::qt
