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

#ifndef CUSTOM_MESH_VIEWER_RENDER_APP_H
#define CUSTOM_MESH_VIEWER_RENDER_APP_H

#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/imgui_stats_drawer.h>
#include <vclib/qt/widget_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/trackball_viewer_drawer.h>
#include <vclib/render/render_app.h>

namespace vcl::qt {

using MeshViewerRenderApp = vcl::RenderApp<
    vcl::qt::WidgetManager,
    vcl::Canvas,
    // vcl::imgui::ImGuiDrawer,
    // vcl::imgui::ImguiStatsDrawer,
    vcl::TrackBallViewerDrawer>;

} // namespace vcl::qt

#endif // CUSTOM_MESH_VIEWER_RENDER_APP_H
