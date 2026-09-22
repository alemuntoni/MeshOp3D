// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "parameter_sub_frame.h"
#include "ui_parameter_sub_frame.h"

namespace mop {

ParameterSubFrame::ParameterSubFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::ParameterSubFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->toolButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(showPushButtonClicked(bool)));
}

ParameterSubFrame::~ParameterSubFrame()
{
    delete mUI;
}

void ParameterSubFrame::setTitleLabel(const std::string& title)
{
    mUI->titleLabel->setText(QString::fromStdString(title));
}

void ParameterSubFrame::setSubFrameLayout(QGridLayout* layout)
{
    mUI->subFrame->setLayout(layout);
}

void ParameterSubFrame::setHeaderFrameVisible(bool visible)
{
    mUI->headerFrame->setVisible(visible);
}

void ParameterSubFrame::setHeaderButtonVisible(bool visible)
{
    mUI->toolButton->setVisible(visible);
}

void ParameterSubFrame::setHeaderButtonChecked(bool checked)
{
    mUI->toolButton->setChecked(checked);
    showPushButtonClicked(checked);
}

void ParameterSubFrame::showPushButtonClicked(bool checked)
{
    mUI->subFrame->setVisible(checked);
    if (checked)
        mUI->toolButton->setArrowType(Qt::ArrowType::DownArrow);
    else
        mUI->toolButton->setArrowType(Qt::ArrowType::RightArrow);
}

} // namespace mop
