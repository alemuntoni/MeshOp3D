// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_FILTER_DOCK_WIDGET_H
#define MOP_APPLICATION_GUI_FILTER_DOCK_WIDGET_H

#include <QDockWidget>

#include <mop/actions/interfaces/filter_action.h>

namespace mop {

namespace Ui {
class FilterDockWidget;
} // namespace Ui

class FilterDockWidget : public QDockWidget
{
    Q_OBJECT

    Ui::FilterDockWidget* mUI;

    const std::shared_ptr<FilterAction> mAction;

public:
    explicit FilterDockWidget(
        const std::shared_ptr<FilterAction>& action,
        QWidget*                                    parent = nullptr);

    ~FilterDockWidget();

signals:
    void applyFilter(
        const std::shared_ptr<FilterAction>& action,
        const ParameterVector&                parmas);

private slots:
    void onApplyButtonClicked();

    void onCancelButtonClicked();

private:
    void addOutputMeshTypeParameter(
        ParameterVector&                      vec,
        const std::shared_ptr<FilterAction>& action);
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_FILTER_DOCK_WIDGET_H
