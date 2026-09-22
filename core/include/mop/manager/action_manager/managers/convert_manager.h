// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_CONVERT_MANAGER_H
#define MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_CONVERT_MANAGER_H

#include "../containers/id_action_container.h"

#include <mop/actions/interfaces/convert_action.h>

namespace mop::detail {

class ConvertManager
{
    IDActionContainer<ConvertAction> mConvertActions;

protected:
    void add(const std::shared_ptr<ConvertAction>& action)
    {
        mConvertActions.add(action);
    }

public:
    // convert

    std::shared_ptr<ConvertAction> convertAction(
        const std::string& name) const
    {
        return mConvertActions.action(name);
    }

    auto convertActions() { return mConvertActions.actions(); }
};

} // namespace mop::detail

#endif // MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_CONVERT_MANAGER_H
