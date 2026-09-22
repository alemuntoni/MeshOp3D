// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "parameter_frame.h"

namespace mop {

ParameterFrame::ParameterFrame(QWidget* parent) : MultiParameterFrame(parent)
{
    setHeaderLabel("Parameters:");
    setHeaderFrameVisible(true);
    setHeaderButtonChecked(true);
    setHeaderButtonVisible(false);
}

ParameterFrame::ParameterFrame(
    const ParameterVector& parameters,
    QWidget*                     parent) : ParameterFrame(parent)
{
    setParameters(parameters);
}

ParameterVector ParameterFrame::parameters() const
{
    return MultiParameterFrame::parameters(0);
}

void ParameterFrame::setParameters(const ParameterVector& parameters)
{
    if (subFramesNumber() == 0) {
        addSubFrame("", parameters);

        setSubFrameHeaderVisible(0, false);
        setSubFrameVisible(0, true);
    }
    else {
        setSubFramePatameters(0, parameters);
    }
}

} // namespace mop
