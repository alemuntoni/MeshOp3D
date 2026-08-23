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

#include "search_filter_widget.h"
#include <hlmp/manager.h>

#include <QStringList>
#include <QTimer>

namespace hlmp {

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



} // namespace hlmp
