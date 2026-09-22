// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_ACTIONS_MESH_IO_GLTF_MESH_IO_H
#define MOP_CORE_ACTIONS_ACTIONS_MESH_IO_GLTF_MESH_IO_H

#include <mop/actions/interfaces/mesh_io_action_base.h>
#include <mop/manager.h>
#include <mop/manager/load_save_textures.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/io.h>

namespace mop {

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

} // namespace mop

#endif // MOP_CORE_ACTIONS_ACTIONS_MESH_IO_GLTF_MESH_IO_H
