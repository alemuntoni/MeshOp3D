// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "point3_parameter_row.h"

#include <QDoubleValidator>

namespace mop {

Point3ParameterRow::Point3ParameterRow(const Point3Parameter& param) :
        ParameterRow(param), mParam(param)
{
    mContainer = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(mContainer);
    layout->setContentsMargins(0, 0, 0, 0);

    QDoubleValidator* validator = new QDoubleValidator(
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::max(),
        4,
        mContainer);

    mLineEditX = new QLineEdit();
    mLineEditX->setToolTip(param.tooltip().c_str());
    mLineEditX->setValidator(validator);
    mLineEditX->setText(QString::number(param.point3Value()[0]));

    mLineEditY = new QLineEdit();
    mLineEditY->setToolTip(param.tooltip().c_str());
    mLineEditY->setValidator(validator);
    mLineEditY->setText(QString::number(param.point3Value()[1]));

    mLineEditZ = new QLineEdit();
    mLineEditZ->setToolTip(param.tooltip().c_str());
    mLineEditZ->setValidator(validator);
    mLineEditZ->setText(QString::number(param.point3Value()[2]));

    layout->addWidget(mLineEditX);
    layout->addWidget(mLineEditY);
    layout->addWidget(mLineEditZ);
}

QWidget* Point3ParameterRow::parameterWidget()
{
    return mContainer;
}

std::shared_ptr<Parameter> Point3ParameterRow::parameterFromWidget() const
{
    auto p = mParam.clone();
    vcl::Point3<ScalarType> pt;
    pt[0] = mLineEditX->text().toDouble();
    pt[1] = mLineEditY->text().toDouble();
    pt[2] = mLineEditZ->text().toDouble();
    p->setPoint3Value(pt);
    return p;
}

} // namespace mop
