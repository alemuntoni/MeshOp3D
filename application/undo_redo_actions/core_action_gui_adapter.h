// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_APPLICATION_UNDO_REDO_ACTIONS_CORE_ACTION_GUI_ADAPTER_H
#define MOP_APPLICATION_UNDO_REDO_ACTIONS_CORE_ACTION_GUI_ADAPTER_H

#include <vclib/qt/mesh_viewer.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/space/core.h>

namespace mop {

/**
 * @brief Wraps a Core-produced UndoRedoAction (which only knows how to swap
 * raw mesh data) adding the GUI-side refresh that the Core cannot perform:
 * re-uploading the GPU buffers of the affected drawable meshes, and
 * refreshing the viewer's GUI (e.g. the Transform Matrix info panel).
 *
 * Without this adapter, undo()/redo() would correctly restore the mesh data,
 * but the rendered mesh and the info panels would keep showing stale state,
 * since the generic viewer undo()/redo() only triggers a repaint, not a
 * buffers/GUI refresh.
 */
class CoreActionGuiAdapter : public vcl::UndoRedoAction
{
    std::unique_ptr<vcl::UndoRedoAction> mCoreAction;
    std::vector<std::shared_ptr<vcl::AbstractDrawableMesh>> mModifiedDrawables;
    vcl::qt::MeshViewer& mViewer;
    std::string           mName;

public:
    CoreActionGuiAdapter(
        std::unique_ptr<vcl::UndoRedoAction>                     coreAction,
        std::vector<std::shared_ptr<vcl::AbstractDrawableMesh>>  modifiedDrawables,
        vcl::qt::MeshViewer&                                     viewer) :
            mCoreAction(std::move(coreAction)),
            mModifiedDrawables(std::move(modifiedDrawables)), mViewer(viewer),
            mName(mCoreAction ? mCoreAction->name() : "Modify Mesh")
    {
    }

    void undo() override
    {
        if (mCoreAction)
            mCoreAction->undo();
        refresh();
    }

    void redo() override
    {
        if (mCoreAction)
            mCoreAction->redo();
        refresh();
    }

    std::string name() const override { return mName; }

private:
    void refresh()
    {
        for (auto& d : mModifiedDrawables)
            d->updateBuffers();
        mViewer.updateGUI();
    }
};

} // namespace mop

#endif // MOP_APPLICATION_UNDO_REDO_ACTIONS_CORE_ACTION_GUI_ADAPTER_H
