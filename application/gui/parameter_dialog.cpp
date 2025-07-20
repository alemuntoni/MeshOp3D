/*****************************************************************************
 * HLMP                                                                      *
 * HighLevelMeshProcessing                                                   *
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

#include "parameter_dialog.h"

#include <QPushButton>

namespace hlmp {

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

} // namespace hlmp
