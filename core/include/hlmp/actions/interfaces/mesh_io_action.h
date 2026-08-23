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

#ifndef HLMP_ACTIONS_INTERFACES_MESH_IO_ACTION_H
#define HLMP_ACTIONS_INTERFACES_MESH_IO_ACTION_H

#include "io_action.h"

#include <hlmp/parameter_vector.h>

#include <any>

#include <vclib/io/file_format.h>
#include <vclib/space/complex/mesh_info.h>

namespace hlmp {

class MeshIOAction : public IOAction
{
public:
    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    virtual MeshTypeId meshType() const = 0;

    // From IOAction class

    virtual IOSupport ioSupport() const = 0;

    /**
     * @brief Returns a vector of file formats and their capabilities.
     *
     * Each file format is defined by a list of extensions (all the possible
     * extensions that a file format could have) and a description.
     *
     * The capabilities of the file format are defined by the MeshInfo
     * class, and they are used to determine the information that can be
     * stored or loaded from the file.
     *
     * @return the vectir of file formats supported by the action and their
     * capabilities
     */
    virtual std::vector<std::pair<vcl::FileFormat, vcl::MeshInfo>>
    supportedMeshFormats() const = 0;

    virtual std::any loadErased(
        MeshTypeId             meshType,
        const std::string&     filename,
        const vcl::FileFormat& format,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        vcl::AbstractLogger&   log) const = 0;

    virtual void saveErased(
        MeshTypeId             meshType,
        const std::string&     filename,
        const vcl::FileFormat& format,
        const std::any&        mesh,
        const vcl::MeshInfo&   info,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log) const = 0;

    virtual vcl::BitSet32 supportedMeshTypes() const = 0;

    /**
     * @brief Returns the parameters to load the mesh.
     *
     * By default, the load function has no parameters.
     *
     * You should override this method if your load function requires
     * parameters.
     *
     * @param[in] format: the file format for which the parameters are requested
     *
     * @return The parameters for loading the mesh with the given file format.
     */
    virtual ParameterVector parametersLoad(const vcl::FileFormat& format) const
    {
        return ParameterVector();
    }

    /**
     * @brief Returns the parameters to save the mesh.
     *
     * By default, the save function has no parameters.
     *
     * You should override this method if your save function requires
     * parameters.
     *
     * @param[in] format: the file format for which the parameters are requested
     *
     * @return The parameters for saving the mesh with the given file format.
     */
    virtual ParameterVector parametersSave(const vcl::FileFormat& format) const
    {
        return ParameterVector();
    }

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    Type type() const final { return Type::MESH_IO_ACTION; }

    std::vector<vcl::FileFormat> supportedFormats() const final
    {
        std::vector<vcl::FileFormat> formats;
        for (const auto& [f, _] : supportedMeshFormats()) {
            formats.push_back(f);
        }
        return formats;
    }

public:
    vcl::MeshInfo formatCapability(const vcl::FileFormat& format) const
    {
        for (const auto& [f, info] : supportedMeshFormats()) {
            if (f == format) {
                return info;
            }
        }
        return vcl::MeshInfo();
    }
};

} // namespace hlmp

#endif // HLMP_ACTIONS_INTERFACES_MESH_IO_ACTION_H
