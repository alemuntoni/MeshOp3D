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

#ifndef M3D_MANAGER_ACTION_MANAGER_MANAGER_H
#define M3D_MANAGER_ACTION_MANAGER_MANAGER_H

#include "managers/convert_manager.h"
#include "managers/filter_manager.h"
#include "managers/image_io_manager.h"
#include "managers/mesh_io_manager.h"

namespace hlmp::detail {

class Manager :
        public ConvertManager,
        public FilterManager,
        public ImageIOManager,
        public MeshIOManager
{
public:
    Manager();

    void add(const std::shared_ptr<Action>& action);

    template<vcl::Range R>
    requires vcl::RangeOf<R, std::shared_ptr<Action>>
    void add(R&& actions)
    {
        for (const auto& action : actions) {
            add(action);
        }
    }

    void addDefaultActions();
};

} // namespace hlmp::detail

#endif // M3D_MANAGER_ACTION_MANAGER_MANAGER_H
