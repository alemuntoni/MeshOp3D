// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_PARAMETERS_PARAMETER_ROW_H
#define MOP_APPLICATION_GUI_PARAMETERS_PARAMETER_ROW_H

#include <memory>

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include <mop/parameters.h>
#include <vclib/qt/gui/clickable_label.h>

namespace mop {

class ParameterRow
{
    QLabel* mHelpLabel;

    bool mVisible;
    bool mHelpVisible;

    bool mHasBeenModified;

protected:
    vcl::qt::ClickableLabel* mDescriptionLabel;

public:
    ParameterRow(const Parameter& param);

    virtual ~ParameterRow();

    virtual QWidget* parameterWidget() = 0;

    virtual std::shared_ptr<Parameter> parameterFromWidget() const = 0;

    void addRowToGridLayout(QGridLayout* lay, const int row);

    bool hasBeenModified() const;

    void setVisible(bool b);

    void setHelpVisible(bool b);

protected:
    void setModified(bool b);
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_PARAMETERS_PARAMETER_ROW_H
