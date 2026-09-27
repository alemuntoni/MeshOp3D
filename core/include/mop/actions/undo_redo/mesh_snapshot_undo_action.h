// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_UNDO_REDO_MESH_SNAPSHOT_UNDO_ACTION_H
#define MOP_CORE_ACTIONS_UNDO_REDO_MESH_SNAPSHOT_UNDO_ACTION_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <cassert>
#include <functional>

namespace mop {

/**
 * @brief Generic UndoRedoAction that reverts an in-place mesh modification by
 * swapping one or more meshes back to the snapshots taken before the
 * modification.
 *
 * This is a general-purpose building block for FilterAction implementations
 * that modify their inputOutputMeshes in place: the filter takes a copy of
 * each mesh before running its algorithm, and hands it over to this class
 * together with a reference to the (now modified) mesh(es).
 *
 * Being full-mesh copies, it is not the most memory/performance efficient
 * option for every filter (e.g. a filter touching only vertex positions could
 * store just those), but it is always correct and requires no per-filter
 * boilerplate. Filters with stricter requirements can implement a dedicated
 * vcl::UndoRedoAction instead.
 */
template<vcl::MeshConcept MeshType>
class MeshSnapshotUndoAction : public vcl::UndoRedoAction
{
    std::vector<std::reference_wrapper<MeshType>> mMeshes;
    std::vector<MeshType>                         mSnapshots;

public:
    MeshSnapshotUndoAction(MeshType& mesh, MeshType snapshot)
    {
        mMeshes.emplace_back(mesh);
        mSnapshots.push_back(std::move(snapshot));
    }

    MeshSnapshotUndoAction(
        std::vector<std::reference_wrapper<MeshType>> meshes,
        std::vector<MeshType>                         snapshots) :
            mMeshes(std::move(meshes)), mSnapshots(std::move(snapshots))
    {
        assert(mMeshes.size() == mSnapshots.size());
    }

    void undo() override { toggle(); }

    void redo() override { toggle(); }

    std::string name() const override { return "Mesh Modification"; }

private:
    // swapping twice (undo then redo) restores the exact original state
    void toggle()
    {
        for (std::size_t i = 0; i < mMeshes.size(); ++i)
            std::swap(mMeshes[i].get(), mSnapshots[i]);
    }
};

} // namespace mop

#endif // MOP_CORE_ACTIONS_UNDO_REDO_MESH_SNAPSHOT_UNDO_ACTION_H
