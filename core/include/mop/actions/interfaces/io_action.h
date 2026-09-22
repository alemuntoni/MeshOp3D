// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_INTERFACES_IO_ACTION_H
#define MOP_CORE_ACTIONS_INTERFACES_IO_ACTION_H

#include "action.h"

#include <vclib/io/file_format.h>

namespace mop {

class IOAction : public Action
{
public:
    enum class IOSupport { LOAD, SAVE, BOTH };

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    virtual Type type() const = 0;

    virtual MeshTypeId meshType() const = 0;

    /**
     * @brief Returns the type of support for input/output operations.
     *
     * Possible values are:
     * - LOAD: the action supports only loading images;
     * - SAVE: the action supports only saving images;
     * - BOTH: the action supports both loading and saving images.
     *
     * @return the type of support for input/output operations
     */
    virtual IOSupport ioSupport() const = 0;

    /**
     * @brief Returns the list of file formats supported by the action.
     *
     * Each file format is defined by a list of extensions (all the possible
     * extensions that a file format could have) and a description.
     *
     * @return the list of file formats supported by the action
     */
    virtual std::vector<vcl::FileFormat> supportedFormats() const = 0;
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_INTERFACES_IO_ACTION_H
