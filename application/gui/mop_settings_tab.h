// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_GUI_MOP_SETTINGS_TAB_H
#define MOP_APPLICATION_GUI_MOP_SETTINGS_TAB_H

#include <mop/mop_settings.h>
#include <vclib/qt/gui/settings_dialog/settings_dialog_tab.h>

#include <QCheckBox>
#include <QVBoxLayout>

namespace mop {

class MopSettingsTab : public vcl::qt::SettingsDialogTab
{
    MopSettings& mSettings;
    MopSettings  mTempSettings;

public:
    MopSettingsTab(MopSettings& settings) : mSettings(settings) {}

    QString category() const override { return "MeshOp3D"; }

    QString name() const override { return "Actions"; }

    QWidget* createWidget(QWidget* parent) override
    {
        mTempSettings = mSettings;

        QWidget*     widget = new QWidget(parent);
        QVBoxLayout* layout = new QVBoxLayout(widget);

        QCheckBox* cb = new QCheckBox(
            "Bypass Filter Dialog for simple filters (0 or 1 mesh, no "
            "parameters)",
            widget);
        cb->setChecked(mTempSettings.bypassFilterDialogForSimpleFilters);

        QObject::connect(cb, &QCheckBox::toggled, [this](bool checked) {
            mTempSettings.bypassFilterDialogForSimpleFilters = checked;
        });

        layout->addWidget(cb);
        layout->addStretch();

        return widget;
    }

    void applySettings() override { mSettings = mTempSettings; }

    void saveSettings(nlohmann::json& j) const override
    {
        mTempSettings.saveSettings(j["MeshOp3D"]);
    }

    void updateToolbarFrames(QToolBar* /*toolbar*/) override
    {
        // No toolbar updates needed
    }
};

} // namespace mop

#endif // MOP_APPLICATION_GUI_MOP_SETTINGS_TAB_H
