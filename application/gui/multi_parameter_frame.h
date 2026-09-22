// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_MULTI_PARAMETER_FRAME_H
#define MOP_APPLICATION_GUI_MULTI_PARAMETER_FRAME_H

#include <QFrame>

#include "parameters_grid_layout.h"

#include "parameter_frame/parameter_sub_frame.h"

namespace mop {

namespace Ui {
class MultiParameterFrame;
} // namespace Ui

class MultiParameterFrame : public QFrame
{
    Q_OBJECT

    Ui::MultiParameterFrame* mUI;

    std::vector<ParametersGridLayout*> mParamGrids;

public:
    explicit MultiParameterFrame(QWidget* parent = nullptr);
    ~MultiParameterFrame();

    uint addSubFrame(
        const std::string&           name,
        const ParameterVector& parameters);

    void setSubFrameName(uint i, const std::string& name);

    void setSubFramePatameters(uint i, const ParameterVector& parameters);

    ParameterVector parameters(uint i) const;

    uint subFramesNumber() const { return mParamGrids.size(); }

    void setHeaderLabel(const std::string& label);

    void setHeaderFrameVisible(bool visible);

    void setHeaderButtonVisible(bool visible);

    void setHeaderButtonChecked(bool checked);

    void setSubFrameVisible(uint i, bool visible);

    void setSubFrameHeaderVisible(uint i, bool visible);

    void setSubFrameHeaderButtonVisible(uint i, bool visible);

    void setSubFrameHeaderButtonChecked(uint i, bool checked);

private slots:
    void showAllParametersButtonClicked(bool checked);

    void helpButtonClicked(bool checked);

private:
    uint addSubFrameLayout(
        const std::string&    name,
        ParametersGridLayout* layout);

    void setSubFrameLayout(uint i, ParametersGridLayout* layout);

    ParameterSubFrame* subFrame(uint i);
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_MULTI_PARAMETER_FRAME_H
