// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_PARAMETER_FRAME_PARAMETER_SUB_FRAME_H
#define MOP_APPLICATION_GUI_PARAMETER_FRAME_PARAMETER_SUB_FRAME_H

#include <QFrame>

#include <QGridLayout>

namespace mop {

namespace Ui {
class ParameterSubFrame;
} // namespace Ui

class ParameterSubFrame : public QFrame
{
    Q_OBJECT

    Ui::ParameterSubFrame* mUI;

public:
    explicit ParameterSubFrame(QWidget* parent = nullptr);
    ~ParameterSubFrame();

    void setTitleLabel(const std::string& title);

    void setSubFrameLayout(QGridLayout* layout);

    void setHeaderFrameVisible(bool visible);

    void setHeaderButtonVisible(bool visible);

    void setHeaderButtonChecked(bool checked);

private slots:
    void showPushButtonClicked(bool checked);
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETER_FRAME_PARAMETER_SUB_FRAME_H
