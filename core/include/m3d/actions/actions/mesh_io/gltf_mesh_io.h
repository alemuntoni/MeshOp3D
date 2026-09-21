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

#ifndef M3D_ACTIONS_ACTIONS_MESH_IO_GLTF_MESH_IO_H
#define M3D_ACTIONS_ACTIONS_MESH_IO_GLTF_MESH_IO_H

#include <m3d/actions/interfaces/mesh_io_action_base.h>
#include <m3d/manager.h>
#include <m3d/manager/load_save_textures.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/io.h>

namespace hlmp {

class GltfMeshIO : public MeshIOActionBase<GltfMeshIO>
{
    using Base = MeshIOActionBase<GltfMeshIO>;

public:
    std::string name() const final { return "glTF IO"; }

    Base::IOSupport ioSupport() const final { return Base::IOSupport::LOAD; }

    std::vector<std::pair<vcl::FileFormat, vcl::MeshInfo>> supportedMeshFormats()
        const final
    {
        return {
            {vcl::gltfFileFormat(), vcl::gltfFormatCapability()},
        };
    }

    ParameterVector parametersLoad(const vcl::FileFormat& format) const final
    {
        ParameterVector params;

        if (format == "gltf" || format == "glb") {
            // Currently, no specific parameters are needed for glTF loading
            // Parameters can be added here in the future if needed
        }
        else {
            throw vcl::UnknownFileFormatException(format.description());
        }

        return params;
    }

    ParameterVector parametersSave(const vcl::FileFormat& format) const final
    {
        // glTF saving is not supported yet
        throw vcl::UnknownFileFormatException(
            "Saving in glTF format is not supported yet.");
    }

    template<vcl::MeshConcept MeshType>
    MeshType load(
        const std::string&     filename,
        const vcl::FileFormat& format,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        vcl::AbstractLogger&   log) const
    {
        MeshType mesh;
        vcl::LoadSettings settings;

        std::string basePath = vcl::FileInfo::pathWithoutFileName(filename);

        if (format == "gltf" || format == "glb") {
            vcl::loadGltf(mesh, filename, loadedInfo, settings, log);
            loadTexturesUsingManager(mesh, basePath);
        }
        else {
            throw vcl::UnknownFileFormatException(format.description());
        }

        // update necessary data (e.g. normals) only if not loaded
        Base::postLoad(mesh, loadedInfo);

        return mesh;
    }

    template<vcl::MeshConcept MeshType>
    void save(
        const std::string&     filename,
        const vcl::FileFormat& format,
        const MeshType&        mesh,
        const vcl::MeshInfo&   info,
        const ParameterVector& parameters,
        vcl::AbstractLogger&   log) const
    {
        // glTF saving is not supported yet
        throw vcl::UnknownFileFormatException(
            "Saving in glTF format is not supported yet.");
    }
};

} // namespace hlmp

#endif // M3D_ACTIONS_ACTIONS_MESH_IO_GLTF_MESH_IO_H
