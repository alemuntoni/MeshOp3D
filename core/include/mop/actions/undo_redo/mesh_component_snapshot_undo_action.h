// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_CORE_ACTIONS_UNDO_REDO_MESH_COMPONENT_SNAPSHOT_UNDO_ACTION_H
#define MOP_CORE_ACTIONS_UNDO_REDO_MESH_COMPONENT_SNAPSHOT_UNDO_ACTION_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <memory>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

namespace mop {

/**
 * @brief Generic UndoRedoAction that reverts the modification of a single
 * mesh component (e.g. vertex positions, vertex/face normals, a transform
 * matrix) by swapping it back to a snapshot taken before the modification.
 *
 * Unlike MeshSnapshotUndoAction, which copies the whole mesh, this class
 * only stores the values of the component extracted by rangeFunc, making it
 * suitable for large meshes where copying unrelated components (face
 * indices, colors, materials, ...) would be wasteful.
 *
 * rangeFunc is a callable MeshType& -> range of values (e.g.
 * `[](MeshType& m) { return m.vertices() | vcl::views::positions; }`); a
 * single value (e.g. a transform matrix) can be handled the same way by
 * wrapping it in a `std::span` of size 1.
 */
template<vcl::MeshConcept MeshType, typename RangeFunc>
class MeshComponentSnapshotUndoAction : public vcl::UndoRedoAction
{
    using Range     = std::invoke_result_t<RangeFunc, MeshType&>;
    using ValueType = std::ranges::range_value_t<Range>;

    MeshType&              mMesh;
    RangeFunc              mRangeFunc;
    std::vector<ValueType> mSnapshot;
    std::string            mName;

public:
    MeshComponentSnapshotUndoAction(
        MeshType& mesh, RangeFunc rangeFunc, std::string name) :
            mMesh(mesh), mRangeFunc(std::move(rangeFunc)), mName(std::move(name))
    {
        for (auto&& v : mRangeFunc(mMesh))
            mSnapshot.push_back(v);
    }

    void undo() override { toggle(); }

    void redo() override { toggle(); }

    std::string name() const override { return mName; }

private:
    // swapping twice (undo then redo) restores the exact original values
    void toggle()
    {
        std::size_t i = 0;
        for (auto&& v : mRangeFunc(mMesh))
            std::swap(v, mSnapshot[i++]);
    }
};

/**
 * @brief Creates a MeshComponentSnapshotUndoAction, deducing RangeFunc from
 * the given callable (std::make_unique cannot deduce it on its own).
 */
template<vcl::MeshConcept MeshType, typename RangeFunc>
auto makeMeshComponentSnapshotUndoAction(
    MeshType& mesh, RangeFunc rangeFunc, std::string name)
{
    return std::make_unique<MeshComponentSnapshotUndoAction<MeshType, RangeFunc>>(
        mesh, std::move(rangeFunc), std::move(name));
}

} // namespace mop

#endif // MOP_CORE_ACTIONS_UNDO_REDO_MESH_COMPONENT_SNAPSHOT_UNDO_ACTION_H
