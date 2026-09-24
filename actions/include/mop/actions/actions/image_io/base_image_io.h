// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_ACTIONS_ACTIONS_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H
#define MOP_ACTIONS_ACTIONS_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H

#include <mop/actions/interfaces/image_io_action.h>

#include <vclib/io/image.h>

namespace mop {

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

} // namespace mop

#endif // MOP_ACTIONS_ACTIONS_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H
