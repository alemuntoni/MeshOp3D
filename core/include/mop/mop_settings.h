// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MOP_SETTINGS_H
#define MOP_CORE_MOP_SETTINGS_H

#include <nlohmann/json.hpp>

namespace mop {

struct MopSettings
{
    bool bypassFilterDialogForSimpleFilters = true;

    void saveSettings(nlohmann::json& j) const
    {
        j["bypassFilterDialogForSimpleFilters"] =
            bypassFilterDialogForSimpleFilters;
    }

    void loadSettings(const nlohmann::json& j)
    {
        if (j.contains("MeshOp3D")) {
            auto& mj = j["MeshOp3D"];
            if (mj.contains("bypassFilterDialogForSimpleFilters") &&
                mj["bypassFilterDialogForSimpleFilters"].is_boolean()) {
                bypassFilterDialogForSimpleFilters =
                    mj["bypassFilterDialogForSimpleFilters"].get<bool>();
            }
        }
    }
};

} // namespace mop

#endif // MOP_CORE_MOP_SETTINGS_H
