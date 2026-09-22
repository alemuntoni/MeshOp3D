// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "parameter_row.h"

namespace mop {

ParameterRow::ParameterRow(const Parameter& param) :
        mVisible(true), mHelpVisible(false)
{
    QString dl = param.description().c_str();
    QString tt = param.tooltip().c_str();

    mDescriptionLabel = new vcl::qt::ClickableLabel();
    mDescriptionLabel->setText(dl);
    mDescriptionLabel->setToolTip(tt);
    mDescriptionLabel->setSizePolicy(
        QSizePolicy::Minimum, QSizePolicy::Preferred);

    mHelpLabel = new QLabel("<small>" + tt + "</small>");
    mHelpLabel->setTextFormat(Qt::RichText);
    mHelpLabel->setWordWrap(true);
    mHelpLabel->setVisible(false);
    mHelpLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mHelpLabel->setMinimumWidth(250);
    mHelpLabel->setMinimumHeight(mHelpLabel->sizeHint().height());
}

ParameterRow::~ParameterRow()
{
}

void ParameterRow::addRowToGridLayout(QGridLayout* lay, const int row)
{
    if (lay != NULL) {
        parameterWidget()->setSizePolicy(
            QSizePolicy::Expanding, QSizePolicy::Preferred);
        lay->addWidget(mDescriptionLabel, row, 0, 1, 1, Qt::AlignRight);
        lay->addWidget(parameterWidget(), row, 1);
        lay->addWidget(mHelpLabel, row, 2);
    }
}

bool ParameterRow::hasBeenModified() const
{
    return mHasBeenModified;
}

void ParameterRow::setVisible(bool b)
{
    mVisible = b;
    mDescriptionLabel->setVisible(b);
    parameterWidget()->setVisible(b);
    if (b && mHelpVisible)
        mHelpLabel->setVisible(true);
    else if (!b)
        mHelpLabel->setVisible(false);
}

void ParameterRow::setHelpVisible(bool b)
{
    mHelpVisible = b;
    mHelpLabel->setVisible(mVisible && mHelpVisible);
    if (b) {
        parameterWidget()->setSizePolicy(
            QSizePolicy::Minimum, QSizePolicy::Preferred);
    }
    else {
        parameterWidget()->setSizePolicy(
            QSizePolicy::Expanding, QSizePolicy::Preferred);
    }
}

void ParameterRow::setModified(bool b)
{
    mHasBeenModified = b;
}

} // namespace mop
