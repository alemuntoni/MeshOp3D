// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_PARAMETERS_GRID_LAYOUT_H
#define MOP_APPLICATION_GUI_PARAMETERS_GRID_LAYOUT_H

#include <QGridLayout>

#include <mop/parameter_vector.h>

#include "parameters/parameter_row.h"

namespace mop {

class ParametersGridLayout : public QGridLayout
{
    ParameterVector mParameters;

    std::vector<std::shared_ptr<ParameterRow>> mRows;

public:
    ParametersGridLayout(QWidget* parent);

    void setParameters(const ParameterVector& parameters);

    ParameterVector parameters() const;

    void setHelpVisible(bool b);

private:
    void clear();

    static void clear(QLayout* layout);
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETERS_GRID_LAYOUT_H
