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

#ifndef HLMP_APPLICATION_GUI_FILTER_DOCK_WIDGET_H
#define HLMP_APPLICATION_GUI_FILTER_DOCK_WIDGET_H

#include <QDockWidget>

#include <hlmp/actions/aggregators/filter_actions.h>

namespace vcl::qt {

namespace Ui {
class FilterDockWidget;
} // namespace Ui

class FilterDockWidget : public QDockWidget
{
    Q_OBJECT

    Ui::FilterDockWidget* mUI;

    const std::shared_ptr<hlmp::FilterActions> mAction;

public:
    explicit FilterDockWidget(
        const std::shared_ptr<hlmp::FilterActions>& action,
        QWidget*                                    parent = nullptr);

    ~FilterDockWidget();

signals:
    void applyFilter(
        const std::shared_ptr<hlmp::FilterActions>& action,
        const hlmp::ParameterVector&                parmas);

private slots:
    void onApplyButtonClicked();

    void onCancelButtonClicked();

private:
    void addOutputMeshTypeParameter(
        hlmp::ParameterVector&                      vec,
        const std::shared_ptr<hlmp::FilterActions>& action);
};

} // namespace vcl::qt

#endif // HLMP_APPLICATION_GUI_FILTER_DOCK_WIDGET_H
