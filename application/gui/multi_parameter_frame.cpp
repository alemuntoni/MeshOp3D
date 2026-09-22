// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "multi_parameter_frame.h"
#include "ui_multi_parameter_frame.h"

namespace mop {

MultiParameterFrame::MultiParameterFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::MultiParameterFrame)
{
    mUI->setupUi(this);

    connect(
        mUI->helpPushButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(helpButtonClicked(bool)));

    connect(
        mUI->headerToolButton,
        SIGNAL(clicked(bool)),
        this,
        SLOT(showAllParametersButtonClicked(bool)));
}

MultiParameterFrame::~MultiParameterFrame()
{
    delete mUI;
}

uint MultiParameterFrame::addSubFrame(
    const std::string&           name,
    const ParameterVector& parameters)
{
    ParametersGridLayout* layout = new ParametersGridLayout(nullptr);
    layout->setParameters(parameters);
    return addSubFrameLayout(name, layout);
}

void MultiParameterFrame::setSubFrameName(uint i, const std::string& name)
{
    subFrame(i)->setTitleLabel(name);
}

void MultiParameterFrame::setSubFramePatameters(
    uint                         i,
    const ParameterVector& parameters)
{
    ParametersGridLayout* layout = new ParametersGridLayout(nullptr);
    layout->setParameters(parameters);
    setSubFrameLayout(i, layout);
}

ParameterVector MultiParameterFrame::parameters(uint i) const
{
    return mParamGrids.at(i)->parameters();
}

void MultiParameterFrame::setHeaderLabel(const std::string& label)
{
    mUI->headerLabel->setText(QString::fromStdString(label));
}

void MultiParameterFrame::setHeaderFrameVisible(bool visible)
{
    mUI->headerFrame->setVisible(visible);
}

void MultiParameterFrame::setHeaderButtonVisible(bool visible)
{
    mUI->headerToolButton->setVisible(visible);
}

void MultiParameterFrame::setHeaderButtonChecked(bool checked)
{
    mUI->headerToolButton->setChecked(checked);
    showAllParametersButtonClicked(checked);
}

void MultiParameterFrame::setSubFrameVisible(uint i, bool visible)
{
    subFrame(i)->setVisible(visible);
}

void MultiParameterFrame::setSubFrameHeaderVisible(uint i, bool visible)
{
    subFrame(i)->setHeaderFrameVisible(visible);
}

void MultiParameterFrame::setSubFrameHeaderButtonVisible(uint i, bool visible)
{
    subFrame(i)->setHeaderButtonVisible(visible);
}

void MultiParameterFrame::setSubFrameHeaderButtonChecked(uint i, bool checked)
{
    subFrame(i)->setHeaderButtonChecked(checked);
}

void MultiParameterFrame::showAllParametersButtonClicked(bool checked)
{
    mUI->parametersFrame->setVisible(checked);
    mUI->resetAllPushButton->setVisible(checked);
    mUI->helpPushButton->setVisible(checked);
    if (checked)
        mUI->headerToolButton->setArrowType(Qt::ArrowType::DownArrow);
    else
        mUI->headerToolButton->setArrowType(Qt::ArrowType::RightArrow);
}

void MultiParameterFrame::helpButtonClicked(bool checked)
{
    for (auto* grid : mParamGrids)
        grid->setHelpVisible(checked);
}

uint MultiParameterFrame::addSubFrameLayout(
    const std::string&    name,
    ParametersGridLayout* layout)
{
    ParameterSubFrame* frame = new ParameterSubFrame(this);

    frame->setTitleLabel(name);
    frame->setSubFrameLayout(layout);

    mUI->parametersLayout->addWidget(frame);
    mParamGrids.push_back(layout);
    return mParamGrids.size() - 1;
}

void MultiParameterFrame::setSubFrameLayout(
    uint                  i,
    ParametersGridLayout* layout)
{
    subFrame(i)->setSubFrameLayout(layout);
    mParamGrids[i] = layout;
}

ParameterSubFrame* MultiParameterFrame::subFrame(uint i)
{
    return static_cast<ParameterSubFrame*>(
        mParamGrids[i]->parentWidget()->parentWidget());
}

} // namespace mop
