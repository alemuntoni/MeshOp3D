// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_SEARCH_FILTER_WIDGET_H
#define MOP_APPLICATION_GUI_SEARCH_FILTER_WIDGET_H

#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QKeyEvent>

#include <mop/actions/interfaces/filter_action.h>
#include <memory>
#include <unordered_map>

namespace mop {

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

} // namespace mop

#endif // MOP_APPLICATION_GUI_SEARCH_FILTER_WIDGET_H
