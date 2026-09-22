// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "parameter_dialog.h"

#include <QPushButton>

namespace mop {

ParameterDialog::ParameterDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Parameters");
}

ParameterDialog::ParameterDialog(
    const ParameterVector& vec,
    const std::string&                title,
    QWidget*                          parent) : QDialog(parent)
{
    setWindowTitle(title.c_str());

    ParametersGridLayout* layout = new ParametersGridLayout(this);
    layout->setParameters(vec);

    setLayout(layout);
    setModal(true);

    QPushButton* okButton = new QPushButton("Ok", this);
    connect(
        okButton,
        &QPushButton::clicked,
        [&, this]() {
            this->accept();
        });
    layout->addWidget(okButton);
}

ParameterVector ParameterDialog::parameters()
{
    ParameterVector vec;
    auto layout = dynamic_cast<ParametersGridLayout*>(this->layout());
    if (layout) {
        vec = layout->parameters();
    }
    return vec;
}

} // namespace mop
