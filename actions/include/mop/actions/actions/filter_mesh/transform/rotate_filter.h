// MeshOp3D
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_TRANSFORM_ROTATE_FILTER_H
#define MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_TRANSFORM_ROTATE_FILTER_H

#include <mop/actions/interfaces/filter_action_base.h>
#include <mop/actions/undo_redo/mesh_snapshot_undo_action.h>
#include <vclib/algorithms/mesh/stat/barycenter.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/mesh/components/transform_matrix.h>

namespace mop {

class RotateFilter : public FilterActionBase<RotateFilter>
{
public:
    std::string name() const final { return "Rotate"; }

    std::string description() const final
    {
        return "Rotates the mesh along one of the principal axes.";
    }

    vcl::BitSet<vcl::uint> categories() const final
    {
        return {FilterAction::Category::TRANSFORM};
    }

    std::vector<UintParameter> inputMeshes() const final { return {}; }

    std::vector<UintParameter> inputOutputMeshes() const final
    {
        return {UintParameter("input_output", 1, "Input/Output Mesh", "")};
    }

    ParameterVector parameters() const override
    {
        ParameterVector p;
        p.pushBack(
            std::make_shared<EnumParameter>(
                "rotation_on",
                0,
                std::vector<std::string> {"X", "Y", "Z", "Custom"},
                vcl::BitSet32().set(),
                "Axis of rotation"));
        auto customAxis = std::make_shared<Point3Parameter>(
            "custom_axis",
            vcl::Point3d(0, 0, 1),
            "Custom axis",
            "The custom axis of rotation (used when 'Custom' is selected)");
        customAxis->setDependency("rotation_on", 3);
        p.pushBack(customAxis);
        p.pushBack(
            std::make_shared<EnumParameter>(
                "center_of_rotation",
                1,
                std::vector<std::string> {"Origin", "Barycenter", "Custom"},
                vcl::BitSet32().set(),
                "Center of rotation",
                "The point around which the mesh is rotated. If 'Origin' is "
                "selected, the mesh is rotated around the origin of the world "
                "coordinate system. If 'Barycenter' is selected, the mesh is "
                "rotated around its barycenter. If 'Custom' is selected, the "
                "mesh is "
                "rotated around the specified custom center."));
        auto customCenter = std::make_shared<Point3Parameter>(
            "custom_center",
            vcl::Point3d(0, 0, 0),
            "Custom center",
            "The custom center of rotation (used when 'Custom' is selected)");
        customCenter->setDependency("center_of_rotation", 2);
        p.pushBack(customCenter);
        p.pushBack(
            std::make_shared<ScalarParameter>(
                "rotation_angle",
                0.0,
                "Rotation angle",
                "Rotation angle in degrees. Positive values rotate "
                "counter-clockwise "
                "when looking along the axis of rotation towards the origin."));
        p.pushBack(
            std::make_shared<BoolParameter>(
                "freeze_transform",
                true,
                "Freeze transform matrix",
                "If true, the mesh vertices and normals are rotated and the "
                "transform matrix is reset to identity. If false, the "
                "transform "
                "matrix is modified and the mesh vertices and normals are not "
                "changed."));
        return p;
    }

    template<vcl::MeshConcept MeshType>
    FilterActionResult executeFilter(
        const std::vector<const MeshType*>&,
        const std::vector<MeshType*>& inputOutputMeshes,
        std::vector<MeshType>&,
        const ParameterVector& params,
        vcl::AbstractLogger&   log = FilterAction::logger()) const
    {
        using enum vcl::AbstractLogger::LogLevel;
        using PositionType = typename MeshType::VertexType::PositionType;
        using ScalarType   = typename PositionType::ScalarType;

        MeshType& mesh     = *inputOutputMeshes.front();
        MeshType  snapshot = mesh;

        uint   axis_id   = params.get("rotation_on")->uintValue();
        uint   center_id = params.get("center_of_rotation")->uintValue();
        double angle     = params.get("rotation_angle")->scalarValue();
        bool   freeze    = params.get("freeze_transform")->boolValue();

        PositionType axis(0, 0, 0);
        if (axis_id == 0)
            axis[0] = 1.0;
        else if (axis_id == 1)
            axis[1] = 1.0;
        else if (axis_id == 2)
            axis[2] = 1.0;
        else if (axis_id == 3) {
            auto custom_axis = params.get("custom_axis")->point3Value();
            axis = PositionType(custom_axis[0], custom_axis[1], custom_axis[2])
                       .normalized();
        }

        PositionType center(0, 0, 0);
        if (center_id == 1) {
            center = vcl::barycenter(mesh);
        }
        else if (center_id == 2) {
            auto custom_center = params.get("custom_center")->point3Value();
            center             = PositionType(
                custom_center[0], custom_center[1], custom_center[2]);
        }

        if (freeze) {
            if (center_id == 1) {
                vcl::translate(mesh, PositionType(-center));
            }
            vcl::rotateDeg(mesh, axis, angle, true);
            if (center_id == 1) {
                vcl::translate(mesh, center);
            }
            log.log("Rotated mesh vertices and normals.", MESSAGE_LOG);
        }
        else {
            using ScalarM = typename MeshType::TransformMatrixType::Scalar;
            vcl::Matrix44<ScalarM> tr1, rot, tr2;
            tr1.setIdentity();
            tr2.setIdentity();
            tr1.col(3).head(3) = -center.template cast<ScalarM>();
            tr2.col(3).head(3) = center.template cast<ScalarM>();

            vcl::Matrix33<ScalarM> m33;
            vcl::setTransformMatrixRotation(
                m33,
                axis.template cast<ScalarM>(),
                vcl::toRad(static_cast<ScalarM>(angle)));

            rot.setIdentity();
            rot.topLeftCorner(3, 3) = m33;

            mesh.transformMatrix() = rot * mesh.transformMatrix();

            // If it wasn't at the origin, we should translate it to origin,
            // rotate it, and translate it back. Or wait: if we want to rotate
            // around `center` (barycenter): M' = Tr(center) * Rot * Tr(-center)
            // * M
            mesh.transformMatrix() = tr2 * rot * tr1 * mesh.transformMatrix();

            log.log("Modified transform matrix.", MESSAGE_LOG);
        }

        auto undoAction = std::make_unique<MeshSnapshotUndoAction<MeshType>>(
            mesh, std::move(snapshot));

        return FilterActionResult(std::move(undoAction), OutputValues());
    }
};

} // namespace mop

#endif // MOP_ACTIONS_ACTIONS_ACTIONS_FILTER_MESH_TRANSFORM_ROTATE_FILTER_H
