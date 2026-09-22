// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "search_filter_widget.h"
#include <mop/manager.h>

#include <QStringList>
#include <QTimer>

namespace mop {

SearchFilterWidget::SearchFilterWidget(QWidget* parent) : QLineEdit(parent)
{
    setPlaceholderText("Search filter...");
    setMinimumWidth(150);
    setMaximumWidth(250);

    auto filters = ActionManager::filterActions();

    QStringList filterNames;
    for (const auto& f : filters) {
        filterNames << QString::fromStdString(f->name());
        mFilterMap[f->name()] = f;
    }

    mModel = new QStringListModel(filterNames, this);
    mCompleter = new QCompleter(mModel, this);
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mCompleter->setCompletionMode(QCompleter::PopupCompletion);
    mCompleter->setFilterMode(Qt::MatchContains);

    setCompleter(mCompleter);

    connect(mCompleter, QOverload<const QString&>::of(&QCompleter::activated),
            this, &SearchFilterWidget::onActivated);
            
    connect(this, &QLineEdit::returnPressed, this, &SearchFilterWidget::onReturnPressed);
}

void SearchFilterWidget::onActivated(const QString& text)
{
    // Prevent double execution if both returnPressed and QCompleter::activated fire
    if (this->text() != text) return;

    std::string name = text.toStdString();
    auto it = mFilterMap.find(name);
    if (it != mFilterMap.end()) {
        emit filterSelected(it->second);
        // Clear text to be ready for next search
        clear();
        clearFocus();
    }
}

void SearchFilterWidget::onReturnPressed()
{
    // Optionally handle when the user types a full correct name and presses enter
    // without clicking the completer popup.
    onActivated(text());
}

void SearchFilterWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        clear();
        clearFocus();
    }
    QLineEdit::keyPressEvent(event);
}



} // namespace mop
