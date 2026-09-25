// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_UNDO_REDO_ACTIONS_DUPLICATE_ACTION_H
#define MOP_APPLICATION_UNDO_REDO_ACTIONS_DUPLICATE_ACTION_H

#include <vclib/qt/mesh_viewer.h>
#include <vclib/render/undo_redo/undo_redo_action.h>

namespace meshop3d {

class DuplicateAction : public vcl::UndoRedoAction
{
    vcl::qt::MeshViewer*                 mViewer;
    uint                                 mIndex;
    std::shared_ptr<vcl::DrawableObject> mObj;
    bool                                 mIsAdded = true;

public:
    DuplicateAction(
        vcl::qt::MeshViewer*                 viewer,
        uint                                 index,
        std::shared_ptr<vcl::DrawableObject> obj) :
            mViewer(viewer), mIndex(index), mObj(std::move(obj))
    {
    }

    void undo() override { toggleState(); }

    void redo() override { toggleState(); }

    std::string name() const override { return "Duplicate Mesh"; }

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

} // namespace meshop3d

#endif // MOP_APPLICATION_UNDO_REDO_ACTIONS_DUPLICATE_ACTION_H
