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

#ifndef MOP_MANAGER_ACTION_MANAGER_CONTAINERS_IO_ACTION_CONTAINER_H
#define MOP_MANAGER_ACTION_MANAGER_CONTAINERS_IO_ACTION_CONTAINER_H

#include <vclib/io/file_format.h>
#include <mop/actions/interfaces/io_action.h>
#include <mop/base/polymorphic_object_vector.h>

#include <map>

namespace mop::detail {

template<typename ActionType>
class IOActionContainer
{
    using FormatMap = std::map<vcl::FileFormat, std::shared_ptr<ActionType>>;

    FormatMap mLoadFormatMap;
    FormatMap mSaveFormatMap;

public:
    IOActionContainer() = default;

    void add(std::shared_ptr<ActionType> action)
    {
        if (!action) {
            throw std::runtime_error("Action is nullptr.");
        }
        std::vector<vcl::FileFormat> formats = action->supportedFormats();

        for (const auto& format : formats) {
            checkFormatDoesNotExist(format);
            auto supp = action->ioSupport();
            if (supp != ActionType::IOSupport::SAVE) {
                mLoadFormatMap[format] = action;
            }
            if (supp != ActionType::IOSupport::LOAD) {
                mSaveFormatMap[format] = action;
            }
        }
    }

    bool loadFormatExists(const vcl::FileFormat& format) const
    {
        return mLoadFormatMap.find(format) != mLoadFormatMap.end();
    }

    std::shared_ptr<ActionType> loadAction(const vcl::FileFormat& format) const
    {
        auto it = findLoadFormatExists(format);
        return it->second;
    }

    std::vector<vcl::FileFormat> loadFormats() const
    {
        std::vector<vcl::FileFormat> formats;
        for (const auto& [format, _] : mLoadFormatMap) {
            formats.push_back(format);
        }
        return formats;
    }

    bool saveFormatExists(const vcl::FileFormat& format) const
    {
        return mSaveFormatMap.find(format) != mSaveFormatMap.end();
    }

    std::shared_ptr<ActionType> saveAction(const vcl::FileFormat& format) const
    {
        auto it = findSaveFormatExists(format);
        return it->second;
    }

    std::vector<vcl::FileFormat> saveFormats() const
    {
        std::vector<vcl::FileFormat> formats;
        for (const auto& [format, _] : mSaveFormatMap) {
            formats.push_back(format);
        }
        return formats;
    }

    std::vector<vcl::FileFormat> saveFormats(MeshTypeId m) const
    {
        // available only if ActionType is MeshIOActions
        std::vector<vcl::FileFormat> formats;
        for (const auto& [format, action] : mSaveFormatMap) {
            if (action->supportedMeshTypes()[vcl::toUnderlying(m)])
                formats.push_back(format);
        }
        return formats;
    }

private:
    void checkFormatDoesNotExist(const vcl::FileFormat& format) const
    {
        if (mLoadFormatMap.find(format) != mLoadFormatMap.end()) {
            throw std::runtime_error("Format already registered for loading.");
        }
        if (mSaveFormatMap.find(format) != mSaveFormatMap.end()) {
            throw std::runtime_error("Format already registered for saving.");
        }
    }

    FormatMap::const_iterator findLoadFormatExists(
        const vcl::FileFormat& format) const
    {
        auto it = mLoadFormatMap.find(format);
        if (it == mLoadFormatMap.end()) {
            throw std::runtime_error("Format not registered for loading.");
        }
        return it;
    }

    FormatMap::const_iterator findSaveFormatExists(
        const vcl::FileFormat& format) const
    {
        auto it = mSaveFormatMap.find(format);
        if (it == mSaveFormatMap.end()) {
            throw std::runtime_error("Format not registered for saving.");
        }
        return it;
    }
};

} // namespace mop::detail

#endif // MOP_MANAGER_ACTION_MANAGER_CONTAINERS_IO_ACTION_CONTAINER_H
