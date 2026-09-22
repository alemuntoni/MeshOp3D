// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_PARAMETER_FRAME_H
#define MOP_APPLICATION_GUI_PARAMETER_FRAME_H

#include "multi_parameter_frame.h"

namespace mop {

class ParameterFrame : public MultiParameterFrame
{
    using MultiParameterFrame::addSubFrame;
    using MultiParameterFrame::setHeaderButtonChecked;
    using MultiParameterFrame::setHeaderButtonVisible;
    using MultiParameterFrame::setHeaderFrameVisible;
    using MultiParameterFrame::setHeaderLabel;
    using MultiParameterFrame::setSubFrameHeaderButtonChecked;
    using MultiParameterFrame::setSubFrameHeaderButtonVisible;
    using MultiParameterFrame::setSubFrameHeaderVisible;
    using MultiParameterFrame::setSubFrameName;
    using MultiParameterFrame::setSubFramePatameters;
    using MultiParameterFrame::setSubFrameVisible;
    using MultiParameterFrame::subFramesNumber;

public:
    explicit ParameterFrame(QWidget* parent = nullptr);

    explicit ParameterFrame(
        const ParameterVector& parameters,
        QWidget*                     parent = nullptr);

    ParameterVector parameters() const;

    void setParameters(const ParameterVector& parameters);
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETER_FRAME_H
