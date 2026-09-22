// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_IMAGE_IO_MANAGER_H
#define MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_IMAGE_IO_MANAGER_H

#include "../containers/io_action_container.h"

#include <mop/actions/interfaces/image_io_action.h>

namespace mop::detail {

class ImageIOManager
{
    IOActionContainer<ImageIOAction> mImageIOActions;

protected:
    void add(const std::shared_ptr<ImageIOAction>& action)
    {
        mImageIOActions.add(action);
    }

public:
    // load image

    std::vector<vcl::FileFormat> loadImageFormats() const
    {
        return mImageIOActions.loadFormats();
    }

    std::shared_ptr<ImageIOAction> loadImageAction(vcl::FileFormat fmt) const
    {
        return mImageIOActions.loadAction(fmt);
    }

    // save image

    std::vector<vcl::FileFormat> saveImageFormats() const
    {
        return mImageIOActions.saveFormats();
    }

    std::shared_ptr<ImageIOAction> saveImageAction(vcl::FileFormat fmt) const
    {
        return mImageIOActions.saveAction(fmt);
    }
};

} // namespace mop::detail

#endif // MOP_CORE_MANAGER_ACTION_MANAGER_MANAGERS_IMAGE_IO_MANAGER_H
