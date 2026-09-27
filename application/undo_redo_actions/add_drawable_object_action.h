// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_UNDO_REDO_ACTIONS_ADD_DRAWABLE_OBJECT_ACTION_H
#define MOP_APPLICATION_UNDO_REDO_ACTIONS_ADD_DRAWABLE_OBJECT_ACTION_H

#include <vclib/qt/mesh_viewer.h>

#include <vclib/space/core.h>

namespace mop {

/**
 * @brief Action for undoing/redoing the addition of a drawable object
 * (e.g. a duplicated mesh, or a mesh newly created by a filter).
 */
class AddDrawableObjectAction : public vcl::UndoRedoAction
{
    vcl::qt::MeshViewer*                 mViewer;
    uint                                 mIndex;
    std::shared_ptr<vcl::DrawableObject> mObj;
    std::string                          mName;
    bool                                 mIsAdded = true;

public:
    AddDrawableObjectAction(
        vcl::qt::MeshViewer*                 viewer,
        uint                                 index,
        std::shared_ptr<vcl::DrawableObject> obj,
        std::string                          name = "Add Drawable Object") :
            mViewer(viewer), mIndex(index), mObj(std::move(obj)),
            mName(std::move(name))
    {
    }

    void undo() override { toggleState(); }

    void redo() override { toggleState(); }

    std::string name() const override { return mName; }

private:
    void toggleState()
    {
        if (!mViewer || !mObj)
            return;

        if (mIsAdded) {
            mViewer->removeDrawableObject(mIndex);
            mIsAdded = false;
        }
        else {
            mViewer->insertDrawableObject(mIndex, mObj);
            mIsAdded = true;
        }
    }
};

} // namespace mop

#endif // MOP_APPLICATION_UNDO_REDO_ACTIONS_ADD_DRAWABLE_OBJECT_ACTION_H
