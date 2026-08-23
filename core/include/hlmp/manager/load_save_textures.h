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

#ifndef HLMP_MANAGER_LOAD_SAVE_TEXTURES_H
#define HLMP_MANAGER_LOAD_SAVE_TEXTURES_H

#include "action_manager.h"

#include <vclib/io.h>

namespace hlmp {

template<vcl::MeshConcept MeshType>
void saveTexturesUsingManager(const MeshType& mesh, const std::string& basePath)
{
    if constexpr (vcl::HasMaterials<MeshType>) {
        for (const auto& [path, texture] : mesh.textureImages()) {
            std::string ext = vcl::FileInfo::extension(path);

            try {
                auto act = ActionManager::saveImageAction(ext);
                act->save(basePath + path, texture);
            }
            catch (const std::exception& e) {
                // todo: log error
                std::cerr << "Error saving texture: " << e.what() << std::endl;
            }
        }
    }
}

template<vcl::MeshConcept MeshType>
void loadTexturesUsingManager(MeshType& mesh, const std::string& basePath)
{
    if constexpr (vcl::HasMaterials<MeshType>) {
        for (vcl::Material& m : mesh.materials()) {
            const uint N_TEXTURES =
                vcl::toUnderlying(vcl::Material::TextureType::COUNT);
            for (uint i = 0; i < N_TEXTURES; ++i) {
                const vcl::TextureDescriptor& td = m.textureDescriptor(i);
                if (!td.isNull() && mesh.textureImage(td.path()).isNull()) {
                    const std::string& path = td.path();
                    std::string        ext  = vcl::FileInfo::extension(path);

                    try {
                        auto act = ActionManager::loadImageAction(ext);
                        mesh.pushTextureImage(path, act->load(basePath + path));
                    }
                    catch (const std::exception& e) {
                        // todo: log error
                        std::cerr << "Error loading texture: " << e.what()
                                  << std::endl;
                    }
                }
            }
        }
    }
}

} // namespace hlmp

#endif // HLMP_MANAGER_LOAD_SAVE_TEXTURES_H
