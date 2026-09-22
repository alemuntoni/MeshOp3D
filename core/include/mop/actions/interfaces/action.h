// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_INTERFACES_ACTION_H
#define MOP_CORE_ACTIONS_INTERFACES_ACTION_H

#include <mop/settings.h>

#include <vclib/base.h>

#include <algorithm>
#include <memory>

namespace mop {

class Action
{
    inline static vcl::ConsoleLogger log;

public:
    enum class Type {
        IMAGE_IO_ACTION = 0,
        MESH_IO_ACTION,
        FILTER_ACTION,
        CONVERT_ACTION,

        COUNT
    };

    Action()          = default;
    virtual ~Action() = default;

    /**
     * @brief Returns the name of the action.
     *
     * The name of the action should be unique.
     *
     * @return The name of the action.
     */
    virtual std::string name() const = 0;

    /**
     * @brief Returns the type of the action.
     *
     * See the enum class @ref Type for the list of available types.
     *
     * @return The type of the action.
     */
    virtual Type type() const = 0;

    /**
     * @brief Returns the type of the mesh for which the action is designed for.
     *
     * This is usually the mesh type on which the action is templated.
     * If the action is not templated, the return value should be
     * MeshTypeId::COUNT.
     *
     * @return The type of the mesh for which the action is designed for.
     */
    virtual MeshTypeId meshType() const = 0;

    /**
     * @brief Returns the identifier of the action.
     *
     * The identifier is a string that is used to identify the action.
     *
     * @return The identifier of the action.
     */
    std::string identifier() const { return identifierFromName(name()); }

protected:
    static vcl::ConsoleLogger& logger() { return log; }

    static std::string identifierFromName(const std::string& name)
    {
        std::string n = name;

        std::replace(n.begin(), n.end(), ' ', '_');
        n = vcl::toLower(n);

        return n;
    }
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_INTERFACES_ACTION_H
