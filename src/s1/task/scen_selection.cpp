#include "s1/task/scen_state.hpp"

namespace shenmue::s1::scen_detail {

bool SelectionSlotValid(std::int32_t slot) {
    return (slot >= 0 && slot <= 5) || slot == 10 || slot == 11;
}

Vector3 MatrixTranslation() {
    return {
        workTransformMatrix.rows[3].x,
        workTransformMatrix.rows[3].y,
        workTransformMatrix.rows[3].z,
    };
}

void BeginSelectionRigTransform(const ScenarioSelectorState &selection) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(selection.rigBasePosition.x, selection.rigBasePosition.y, selection.rigBasePosition.z);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(selection.rigBaseRotation.x));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(selection.rigBaseRotation.y));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(selection.rigBaseRotation.z));
}

void EndSelectionRigTransform() {
    Matrix4x4::Pop();
}

void BuildSelectionCameraAnchors(ScenarioSelectorState &selection) {
    selection.rigRoot = selection.rig ? static_cast<Mt5RuntimeNode *>(selection.rig->model) : nullptr;
    selection.cameraTargetAnchors.fill(selection.rigBasePosition);
    selection.cameraEyeAnchors.fill(selection.rigBasePosition);
    BeginSelectionRigTransform(selection);
    for (const std::int32_t slot : kSelectionSlots) {
        matrix_stack::Matrix4x4_Push();
        Mt5RuntimeNode *const node = FindScenarioModelNode(selection.rigRoot, static_cast<std::uint16_t>(slot + 172));
        if (node) {
            Matrix4x4::preTranslate(node->position.x, node->position.y + 0.125f, node->position.z);
        }
        selection.cameraTargetAnchors[static_cast<std::size_t>(slot)] = MatrixTranslation();
        Matrix4x4::preTranslate(0.0f, 0.0f, 0.315f);
        selection.cameraEyeAnchors[static_cast<std::size_t>(slot)] = MatrixTranslation();
        Matrix4x4::Pop();
    }
    EndSelectionRigTransform();
}

Vector3 SelectionRigPoint(const ScenarioSelectorState &selection, std::uint16_t firstNodeKind, std::uint16_t secondNodeKind, const Vector3 &offset) {
    BeginSelectionRigTransform(selection);
    if (firstNodeKind != 0) {
        if (Mt5RuntimeNode *const node = FindScenarioModelNode(selection.rigRoot, firstNodeKind)) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        }
    }
    if (secondNodeKind != 0) {
        if (Mt5RuntimeNode *const node = FindScenarioModelNode(selection.rigRoot, secondNodeKind)) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        }
    }
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    const Vector3 result = MatrixTranslation();
    EndSelectionRigTransform();
    return result;
}

Vector3 SelectionProxyPoint(const ScenarioSelectorState &selection, std::int32_t slot, const Vector3 &offset) {
    if (!SelectionSlotValid(slot))
        return selection.rigBasePosition;
    return SelectionRigPoint(selection, static_cast<std::uint16_t>(slot + 152), 243, offset);
}

void SetSelectionProxyPosition(ScenarioSelectorState &selection, const Vector3 &position) {
    if (selection.selected) {
        shenApplyTranslationToAll(selection.selected->entity.node, position);
    }
}

Vector3 SelectionProxyPosition(const ScenarioSelectorState &selection) {
    return selection.selected ? GetPositionForEntity(selection.selected->entity.node) : Vector3{};
}

void ApplySelectionCamera(const Vector3 &eye, const Vector3 &target) {
    const bool locked = bLockCamera != 0;
    TranslateCameraRelative(eye);
    if (!locked) {
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, target);
    }
}

void AdjustSelectionMarkerZ(ScenarioSelectorState &selection, std::int32_t slot, double delta) {
    if (!SelectionSlotValid(slot))
        return;
    Mt5RuntimeNode *const slotRoot = FindScenarioModelNode(selection.rigRoot, static_cast<std::uint16_t>(slot + 172));
    Mt5RuntimeNode *const marker = FindScenarioModelNode(slotRoot, 244);
    if (marker)
        marker->position.z = static_cast<float>(marker->position.z + delta);
}

// The button press moves the slot node itself, not its marker.
void PressSelectionSlot(ScenarioSelectorState &selection, std::int32_t slot, double delta) {
    Mt5RuntimeNode *const slotRoot = SelectionSlotValid(slot) ? FindScenarioModelNode(selection.rigRoot, static_cast<std::uint16_t>(slot + 172)) : nullptr;
    if (!slotRoot)
        return;
    slotRoot->position.x += 0.0f;
    slotRoot->position.y += 0.0f;
    slotRoot->position.z = static_cast<float>(slotRoot->position.z + delta);
}

Vector3 AddSelectionVector(const Vector3 &lhs, const Vector3 &rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

Vector3 SubtractSelectionVector(const Vector3 &lhs, const Vector3 &rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Vector3 ScaleSelectionVector(const Vector3 &value, float scale) {
    return {value.x * scale, value.y * scale, value.z * scale};
}

Vector3 LerpSelectionVector(const Vector3 &from, const Vector3 &to, float factor) {
    return {
        from.x + (to.x - from.x) * factor,
        from.y + (to.y - from.y) * factor,
        from.z + (to.z - from.z) * factor,
    };
}

void ResetSelectionRepeatCounters(ScenarioSelectorState &selection) {
    selection.repeatLeft = 0;
    selection.repeatRight = 0;
    selection.repeatUp = 0;
    selection.repeatDown = 0;
}

void UpdateSelectionRepeatCounters(ScenarioSelectorState &selection, const PadCaptureEntry *input) {
    const std::int16_t pressed = input ? static_cast<std::int16_t>(input->pressedPriority) : 0;
    const std::int16_t repeated = input ? static_cast<std::int16_t>(input->repeatedPresses) : 0;
    if ((pressed & 0x4000) != 0 || (repeated & 0x4000) != 0) {
        ++selection.repeatLeft;
        selection.repeatRight = 0;
        selection.repeatUp = 0;
        selection.repeatDown = 0;
    } else if (pressed < 0 || repeated < 0) {
        ++selection.repeatRight;
        selection.repeatLeft = 0;
        selection.repeatUp = 0;
        selection.repeatDown = 0;
    } else if ((pressed & 0x1000) != 0 || (repeated & 0x1000) != 0) {
        ++selection.repeatUp;
        selection.repeatLeft = 0;
        selection.repeatRight = 0;
        selection.repeatDown = 0;
    } else if ((pressed & 0x2000) != 0 || (repeated & 0x2000) != 0) {
        ++selection.repeatDown;
        selection.repeatLeft = 0;
        selection.repeatRight = 0;
        selection.repeatUp = 0;
    } else {
        ResetSelectionRepeatCounters(selection);
    }
}

bool BeginSelectionSlotTransition(ScenarioSelectorState &selection, std::int32_t slot, std::int32_t repeatCount) {
    if (!SelectionSlotValid(slot) || (repeatCount != 0 && repeatCount <= 15)) {
        return false;
    }
    selection.previousSlot = selection.selectedSlot;
    selection.selectedSlot = slot;
    selection.transitionFrameCount = repeatCount == 0 ? 7 : 5;
    selection.frame = 0;
    selection.state = CharaSelectState::SwitchSlot;
    return true;
}

void HandleSelectionNavigation(ScenarioSelectorState &selection, const PadCaptureEntry *input) {
    const std::int16_t pressed = input ? static_cast<std::int16_t>(input->pressedPriority) : 0;
    const std::int16_t repeated = input ? static_cast<std::int16_t>(input->repeatedPresses) : 0;
    const std::int32_t slot = selection.selectedSlot;
    if ((pressed & 0x4000) != 0 || (repeated & 0x4000) != 0) {
        if (slot > 0 && slot != 10) {
            BeginSelectionSlotTransition(selection, slot - 1, selection.repeatLeft);
        }
        ++selection.repeatLeft;
        selection.repeatRight = 0;
        selection.repeatUp = 0;
        selection.repeatDown = 0;
        return;
    }
    if (pressed < 0 || repeated < 0) {
        if (slot != 5 && slot < 11) {
            BeginSelectionSlotTransition(selection, slot + 1, selection.repeatRight);
        }
        ++selection.repeatRight;
        selection.repeatLeft = 0;
        selection.repeatUp = 0;
        selection.repeatDown = 0;
        return;
    }
    if ((pressed & 0x1000) != 0 || (repeated & 0x1000) != 0) {
        if (slot >= 10) {
            BeginSelectionSlotTransition(selection, slot - 6, selection.repeatUp);
        }
        ++selection.repeatUp;
        selection.repeatLeft = 0;
        selection.repeatRight = 0;
        selection.repeatDown = 0;
        return;
    }
    if ((pressed & 0x2000) != 0 || (repeated & 0x2000) != 0) {
        if (slot < 10) {
            BeginSelectionSlotTransition(selection, std::max(10, slot + 6), selection.repeatDown);
        }
        ++selection.repeatDown;
        selection.repeatLeft = 0;
        selection.repeatRight = 0;
        selection.repeatUp = 0;
        return;
    }
    ResetSelectionRepeatCounters(selection);
}

} // namespace shenmue::s1::scen_detail
