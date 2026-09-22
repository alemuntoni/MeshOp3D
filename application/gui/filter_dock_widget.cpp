// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "filter_dock_widget.h"
#include "ui_filter_dock_widget.h"

#include <QPushButton>

namespace mop {

FilterDockWidget::FilterDockWidget(
    const std::shared_ptr<FilterAction>& action,
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
    ParameterVector&                      params,
    const std::shared_ptr<FilterAction>& action)
{
    auto arr = meshTypeNames();

    std::vector<std::string> enumValues(arr.begin(), arr.end());

    // get first available value
    uint i = 0;
    for (i = 0; i < enumValues.size(); i++) {
        if (action->supportedMeshTypes()[i]) {
            break;
        }
    }
    EnumParameter param(
        "output_mesh_type",
        i,
        enumValues,
        action->supportedMeshTypes(), // todo
        "Output Mesh Type",
        "The type of the output mesh.");

    params.insert(0, param);
}

} // namespace mop
