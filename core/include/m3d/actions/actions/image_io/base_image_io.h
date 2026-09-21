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

#ifndef M3D_ACTIONS_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H
#define M3D_ACTIONS_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H

#include <m3d/actions/interfaces/image_io_action.h>

#include <vclib/io/image.h>

namespace hlmp {

class BaseImageIO : public ImageIOAction
{
public:
    std::string name() const final { return "Base IO Image"; }

    IOSupport ioSupport() const final { return IOSupport::BOTH; }

    std::vector<vcl::FileFormat> supportedFormats() const final
    {
        std::vector<vcl::FileFormat> formats;
        formats.push_back(vcl::FileFormat("png", "Portable Network Graphics"));
        formats.push_back(vcl::FileFormat("bmp", "Bitmap"));
        formats.push_back(vcl::FileFormat("tga", "Truevision TGA"));
        formats.push_back(vcl::FileFormat(
            std::vector<std::string> {"jpg", "jpeg"},
            "Joint Photographic Experts Group"));

        return formats;
    }

    vcl::Image load(
        const std::string&   filename,
        vcl::AbstractLogger& log = logger()) const final
    {
        return vcl::loadImage(filename);
    }

    void save(
        const std::string& filename,
        const vcl::Image&       image,
        vcl::AbstractLogger&    log = logger()) const final
    {
        assert(!image.isNull());
        vcl::saveImage(image, filename);
    }
};

} // namespace hlmp

#endif // M3D_ACTIONS_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H
