// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_MANAGER_LOAD_SAVE_TEXTURES_H
#define MOP_CORE_MANAGER_LOAD_SAVE_TEXTURES_H

#include "action_manager.h"

#include <vclib/io.h>

namespace mop {

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

} // namespace mop

#endif // MOP_CORE_MANAGER_LOAD_SAVE_TEXTURES_H
