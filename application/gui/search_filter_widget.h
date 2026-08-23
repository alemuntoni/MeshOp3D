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

#ifndef HLMP_APPLICATION_GUI_SEARCH_FILTER_WIDGET_H
#define HLMP_APPLICATION_GUI_SEARCH_FILTER_WIDGET_H

#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QKeyEvent>

#include <hlmp/actions/interfaces/filter_action.h>
#include <memory>
#include <unordered_map>

namespace hlmp {

class SearchFilterWidget : public QLineEdit
{
    Q_OBJECT

    QCompleter*       mCompleter;
    QStringListModel* mModel;

    std::unordered_map<std::string, std::shared_ptr<FilterAction>> mFilterMap;

public:
    explicit SearchFilterWidget(QWidget* parent = nullptr);
    ~SearchFilterWidget() = default;

signals:
    void filterSelected(const std::shared_ptr<FilterAction>& action);

private slots:
    void onActivated(const QString& text);
    void onReturnPressed();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

} // namespace hlmp

#endif // HLMP_APPLICATION_GUI_SEARCH_FILTER_WIDGET_H
