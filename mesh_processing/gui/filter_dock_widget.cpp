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

#include "filter_dock_widget.h"
#include "ui_filter_dock_widget.h"

#include <QPushButton>

namespace vcl::qt {

FilterDockWidget::FilterDockWidget(
    const std::shared_ptr<proc::FilterActions>& action,
    QWidget*                                    parent) :
        QDockWidget(parent), mUI(new Ui::FilterDockWidget), mAction(action)
{
    mUI->setupUi(this);

    setWindowTitle(QString::fromStdString(action->name()));

    mUI->filterDescriptionLabel->setText(
        QString::fromStdString(action->description()));

    auto params = action->parameters();

    // if the action has only output mesh(es)
    if (action->inputMeshes().empty() && action->inputOutputMeshes().empty()) {
        // we don't know which type of mesh the user wants
        // we add an additional parameter to let the user choose the mesh type
        addOutputMeshTypeParameter(params, action);
    }

    mUI->parameterFrame->setParameters(params);

    QPushButton* applyButton = mUI->buttonBox->button(QDialogButtonBox::Apply);
    QPushButton* cancelButton =
        mUI->buttonBox->button(QDialogButtonBox::Cancel);

    connect(
        applyButton,
        &QPushButton::clicked,
        this,
        &FilterDockWidget::onApplyButtonClicked);

    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &FilterDockWidget::onCancelButtonClicked);
}

FilterDockWidget::~FilterDockWidget()
{
    delete mUI;
}

void FilterDockWidget::onApplyButtonClicked()
{
    emit applyFilter(mAction, mUI->parameterFrame->parameters());
}

void FilterDockWidget::onCancelButtonClicked()
{
    close();
}

void FilterDockWidget::addOutputMeshTypeParameter(
    proc::ParameterVector&                      params,
    const std::shared_ptr<proc::FilterActions>& action)
{
    auto arr = proc::meshTypeNames();

    std::vector<std::string> enumValues(arr.begin(), arr.end());

    // get first available value
    uint i = 0;
    for (i = 0; i < enumValues.size(); i++) {
        if (action->supportedMeshTypes()[i]) {
            break;
        }
    }
    proc::EnumParameter param(
        "output_mesh_type",
        i,
        enumValues,
        action->supportedMeshTypes(), // todo
        "Output Mesh Type",
        "The type of the output mesh.");

    params.insert(0, param);
}

} // namespace vcl::qt
