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

#ifndef M3D_ACTIONS_ACTIONS_MESH_IO_BASE_MESH_IO_H
#define M3D_ACTIONS_ACTIONS_MESH_IO_BASE_MESH_IO_H

#include <m3d/actions/interfaces/mesh_io_action_base.h>
#include <m3d/manager.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/io.h>

namespace hlmp {

class BaseMeshIO : public MeshIOActionBase<BaseMeshIO>
{
    using Base = MeshIOActionBase<BaseMeshIO>;

public:
    std::string name() const final { return "Base IO"; }

    Base::IOSupport ioSupport() const final { return Base::IOSupport::BOTH; }

    std::vector<std::pair<vcl::FileFormat, vcl::MeshInfo>> supportedMeshFormats()
        const final
    {
        return {
            {vcl::objFileFormat(), vcl::objFormatCapability()},
            {vcl::offFileFormat(), vcl::offFormatCapability()},
            {vcl::plyFileFormat(), vcl::plyFormatCapability()},
            {vcl::stlFileFormat(), vcl::stlFormatCapability()},
        };
    }

    ParameterVector parametersLoad(const vcl::FileFormat& format) const final
    {
        ParameterVector params;

        if (format == "stl") {
            params.pushBack(BoolParameter(
                "unify_duplicate_vertices",
                true,
                "Unify Duplicate Vertices",
                "STL files may contain duplicated vertices. "
                "If this option is enabled, the loader will remove them. "
                "This may lead to a reduction in the number of vertices. "
                "The original number of vertices and the number of removed "
                "vertices will be logged."));
        }
        else if (format != "obj" && format != "off" && format != "ply") {
            throw vcl::UnknownFileFormatException(format.description());
        }

        return params;
    }

    ParameterVector parametersSave(const vcl::FileFormat& format) const final
    {
        ParameterVector params;

        if (format == "obj" || format == "ply" || format == "stl") {
            if (format != "stl") { // obj and ply
                params.pushBack(BoolParameter(
                    "save_texture_files",
                    true,
                    "Save Texture Files",
                    "If true, the texture image files will be saved along the "
                    "mesh file. Otherwise, only the mesh will be saved."));
            }

            if (format != "obj") { // ply and stl
                params.pushBack(BoolParameter(
                    "binary",
                    true,
                    "Binary",
                    "If true, the file will be saved as binary."));
            }

            if (format == "stl") {
                params.pushBack(BoolParameter(
                    "magics_mode",
                    false,
                    "Magics Mode",
                    "Magics mode is a specific file format used by the Magics "
                    "software, which includes more information than the "
                    "standard STL format, like face colors. The magicsMode "
                    "works only when the file is saved as binary."));
            }
        }
        else if (format != "off") {
            throw vcl::UnknownFileFormatException(format.description());
        }

        return params;
    }

    template<vcl::MeshConcept MeshType>
    MeshType load(
        const std::string&     filename,
        const vcl::FileFormat& format,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        vcl::AbstractLogger&   log) const
    {
        MeshType     mesh;
        vcl::LoadSettings settings;

        std::string basePath = vcl::FileInfo::pathWithoutFileName(filename);

        if (format == "obj") {
            vcl::loadObj(mesh, filename, loadedInfo, settings, log);
            loadTexturesUsingManager(mesh, basePath);
        }
        else if (format == "off") {
            vcl::loadOff(mesh, filename, loadedInfo, settings, log);
        }
        else if (format == "ply") {
            vcl::loadPly(mesh, filename, loadedInfo, settings, log);
            loadTexturesUsingManager(mesh, basePath);
        }
        else if (format == "stl") {
            vcl::loadStl(mesh, filename, loadedInfo, settings, log);

            if (parameters.get("unify_duplicate_vertices")->boolValue()) {
                vcl::removeDuplicateVertices(mesh);
                mesh.compactVertices();
                // todo: log the number of removed vertices
            }
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
        std::string basePath = vcl::FileInfo::pathWithoutFileName(filename);

        vcl::SaveSettings settings;
        settings.info = info;
        if (format == "obj" || format == "ply" || format == "stl") {
            if (format != "obj") { // ply and stl
                settings.binary = parameters.get("binary")->boolValue();
            }
            if (format == "stl") {
                settings.magicsMode =
                    parameters.get("magics_mode")->boolValue();
            }
        }

        if (format == "obj") {
            saveObj(mesh, filename, settings, log);
            if (parameters.get("save_texture_files")->boolValue())
                saveTexturesUsingManager(mesh, basePath);
        }
        else if (format == "off") {
            saveOff(mesh, filename, settings, log);
        }
        else if (format == "ply") {
            savePly(mesh, filename, settings, log);
            if (parameters.get("save_texture_files")->boolValue())
                saveTexturesUsingManager(mesh, basePath);
        }
        else if (format == "stl") {
            if (!isTriangleMesh(mesh)) {
                throw vcl::MissingTriangularRequirementException(
                    "STL format can save only triangular meshes, but the given "
                    "mesh is a polygonal mesh. You should triangulate it "
                    "before saving it.");
            }
            saveStl(mesh, filename, settings, log);
        }
        else {
            throw vcl::UnknownFileFormatException(format.description());
        }
    }
};

} // namespace hlmp

#endif // M3D_ACTIONS_ACTIONS_MESH_IO_BASE_MESH_IO_H
