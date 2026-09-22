// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_PARAMETER_DIALOG_H
#define MOP_APPLICATION_GUI_PARAMETER_DIALOG_H

#include "parameters_grid_layout.h"

#include <QDialog>

namespace mop {

class ParameterDialog : public QDialog
{
public:
    ParameterDialog(QWidget* parent = nullptr);

    ParameterDialog(
        const ParameterVector& vec,
        const std::string&     title  = "",
        QWidget*               parent = nullptr);

    ParameterVector parameters();
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETER_DIALOG_H
