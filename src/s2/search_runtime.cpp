#include "s2/search_internal.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "d3t/dx11_renderer.hpp"
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
extern float searchRadius;
namespace {
std::int32_t lastSearchCone{-1};
constexpr Vector3 searchCones[8]{
    {0, -1, .3f}, {-.707f, -.707f, .4f}, {-1, 0, .5f}, {-.707f, .707f, .6f}, {0, 1, .7f}, {.707f, .707f, .6f}, {1, 0, .5f}, {.707f, -.707f, .4f}};
constexpr Vector2 anchorOffsets[8]{{0, 0}, {.45f, 1.65f}, {.45f, 1.65f}, {.45f, 1.5f}, {.45f, 1.5f}, {.45f, 1.5f}, {.45f, 1.5f}, {.45f, 1.5f}};
Vector3 Translation() {
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}
std::int16_t Angle(float x, float z) {
    return static_cast<std::int16_t>(atan2_fixed16(x, z));
}
std::int32_t AbsAngle(std::int32_t angle) {
    const std::int16_t value = static_cast<std::int16_t>(angle);
    return value < 0 ? -value : value;
}
bool ScenarioSearchExcluded() {
    return HasScenarioCharacterControl();
}
Character *Player() {
    return static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
}
bool FindAdmissionPosition(shen_name name, Vector3 &position) {
    const std::span<shen_name, 32> names = GetScenarioCharacterAdmissionNames();
    const std::span<const Vector3, 32> positions = srchGetScenarioAdmissionPositions();
    for (std::size_t i = 0; i < names.size() && names[i].name; ++i)
        if (names[i].name == name.name) {
            position = positions[i];
            return true;
        }
    return false;
}
bool ActorSearchPosition(shen_name name, Vector3 &position) {
    if (FindAdmissionPosition(name, position))
        return true;
    Character *const character = GetScenarioCharacterNode(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
    if (!character)
        return false;
    GetCharacterMotionWorldPosition(character, 5, &position);
    return true;
}
void ApplyInverseNode(const MT7Node &node) {
    if (const Matrix4x4 *const override = GetMT7NodeTransformOverride(node)) {
        Matrix4x4 inverse = *override;
        Matrix4x4::InvertInPlace(&inverse);
        Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
        return;
    }
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-node.rotation.x));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-node.rotation.y));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-node.rotation.z));
    Matrix4x4::preTranslate(-node.position.x, -node.position.y, -node.position.z);
}
bool ApplyMarkerTranslation(MT7Node *node, std::uint32_t marker, Vector3 &position, bool requireMesh) {
    for (; node; node = node->runtimeSibling) {
        Matrix4x4_Push();
        srchApplyObjectTransform(node, nullptr);
        if (IsMDPXorMDP7(node) == marker && (!requireMesh || node->xb01Offset)) {
            position = Translation();
            Matrix4x4::Pop(1);
            return true;
        }
        if (node->runtimeChild && ApplyMarkerTranslation(node->runtimeChild, marker, position, requireMesh)) {
            Matrix4x4::Pop(1);
            return true;
        }
        Matrix4x4::Pop(1);
    }
    return false;
}
void UpdateFollowPositions() {
    plymUpdatePlayerSearchLocation(&playerSearchPosition);
    anchorPosition = followPosition = focusPosition = currentFollowPosition = playerSearchPosition;
    const shen_name follow = GetScenarioFollowCharacter();
    if (follow.name)
        ActorSearchPosition(follow, followPosition);
    if (focusObject.name && !ActorSearchPosition(focusObject, focusPosition))
        focusObject = {};
    anchorCharacter = shen_name{g_playerRuntime.model ? g_playerRuntime.model->searchRegionKey : 0};
    if (anchorCharacter.name) {
        const Matrix4x4 *const transform = doorFindEntryTransform(anchorCharacter.name);
        if (transform) {
            const std::int32_t index = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(anchorCharacter.name >> 8U)) - '0';
            anchorPosition = {anchorOffsets[index].x, anchorOffsets[index].y, 0};
            MT7Node *const root = GetPropRoot();
            MT7Node *const node = root ? FindModelCharacterRecursive(root, anchorCharacter) : nullptr;
            Matrix4x4_Push();
            Matrix4x4::SetActiveTransform(transform);
            Matrix4x4::preTranslate(anchorPosition.x, anchorPosition.y, anchorPosition.z);
            anchorPosition = Translation();
            if (node) {
                Matrix4x4::SetActiveTransform(transform);
                if (!ApplyMarkerTranslation(node->runtimeChild, (anchorCharacter.name & 0xffffff00U) | 'Q', anchorPosition, true)) {
                    Matrix4x4::SetActiveTransform(transform);
                    ApplyMarkerTranslation(node->runtimeChild, (anchorCharacter.name & 0xffffff00U) | 'L', anchorPosition, false);
                }
            }
            Matrix4x4::Pop(1);
        } else
            anchorCharacter = {};
    }
    if (currentFollowCharacter.name) {
        if (currentFollowCharacter.name == anchorCharacter.name)
            currentFollowPosition = anchorPosition;
        else if (currentFollowCharacter.name == focusObject.name)
            currentFollowPosition = focusPosition;
        else if (!FindAdmissionPosition(currentFollowCharacter, currentFollowPosition))
            currentFollowCharacter = {};
    }
}
float FindBestFollowCandidate(const LegacyInputSlot &input) {
    float best = 1000000.0f;
    lastSearchCone = -1;
    currentFollowCharacter = {};
    const shen_name follow = GetScenarioFollowCharacter();
    if (follow.name && GetScenarioFollowGraceFrames() > 0 && plymIsSearchable() && !plymIsSearchBlocked() && !ScenarioSearchExcluded()) {
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (GetScenarioCharacterNode(control->scenarioTask, follow)) {
            currentFollowCharacter = follow;
            currentFollowPosition = followPosition;
            return 0.0f;
        }
        std::int32_t remaining = GetScenarioFollowGraceFrames() - 1;
        if (input.pressedPriority & 0x200U)
            remaining = 0;
        SetScenarioFollowGraceFrames(remaining);
        if (remaining <= 0)
            SetScenarioFollowCharacter({});
        if (camCtrlGetState() == CameraState::Conversation)
            camCtrlSetState(CameraState::Init);
    }
    if (focusObject.name && plymIsSearchable()) {
        currentFollowCharacter = focusObject;
        currentFollowPosition = focusPosition;
        return 0.0f;
    }
    focusObject = {};
    if (!plymIsSearchable() || (input.held & 0x200U) || ctrlGetLookOverlayWeight() >= .05f || camCtrlGetState() != CameraState::LookAround ||
        plymGetLookZoomScale() <= 120)
        return best;
    const float width = static_cast<float>(d3t::g_originalScreenWidth), height = static_cast<float>(d3t::g_originalScreenHeight);
    PushProjectionPrePass(width, height);
    const std::span<shen_name, 32> names = GetScenarioCharacterAdmissionNames();
    const std::span<const Vector3, 32> positions = srchGetScenarioAdmissionPositions();
    for (std::int32_t index = -1; index < 32; ++index) {
        Vector3 point;
        shen_name name;
        if (index < 0) {
            if (!anchorCharacter.name)
                continue;
            point = anchorPosition;
            name = anchorCharacter;
            const float dx = point.x - playerSearchPosition.x, dz = point.z - playerSearchPosition.z;
            if (dx * dx + dz * dz >= 9.0f)
                continue;
        } else {
            if (!names[index].name)
                break;
            point = positions[index];
            name = names[index];
        }
        Vector3 cameraPoint;
        Matrix4x4::TransformPoint(point, cameraPoint);
        if (cameraPoint.z >= -.1f)
            continue;
        Vector2 screen;
        PushTransform(point, screen);
        screen.x -= width * .5f;
        screen.y -= height * .5f;
        if (screen.x <= -width * 3.5f || screen.x >= width * 3.5f || screen.y <= -height * 3.5f || screen.y >= height * 3.5f)
            continue;
        const float distance = screen.x * screen.x + screen.y * screen.y;
        if (distance < best) {
            best = distance;
            currentFollowCharacter = name;
            currentFollowPosition = point;
        }
    }
    return best;
}
void SwitchDetectedTarget() {
    std::int32_t style;
    if (currentFollowCharacter.name == anchorCharacter.name)
        style = 3;
    else if (npcGetCanInteract(currentFollowCharacter))
        style = 5;
    else
        style = 4 - (GetScenarioFollowEligibility(currentFollowCharacter) != 0);
    srchSetLookTarget(nullptr);
    srchSetLookBase(&currentFollowPosition);
    srchSetRotY(Angle(currentFollowPosition.x - playerSearchPosition.x, currentFollowPosition.z - playerSearchPosition.z));
    camCtrlSetLockedOnAngle(0);
    camCtrlSwitchToLockedOn(style);
}
void DetectTargetFromView(const LegacyInputSlot &input) {
    const shen_name old = currentFollowCharacter;
    shen_name name = old;
    Vector3 point = currentFollowPosition;
    std::int32_t cone = -1;
    const shen_name follow = GetScenarioFollowCharacter();
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!old.name && follow.name && !(input.pressedPriority & 0x200U) && plymIsSearchable() && !plymIsSearchBlocked() && !ScenarioSearchExcluded() &&
        GetScenarioCharacterNode(control->scenarioTask, follow)) {
        currentFollowCharacter = follow;
        currentFollowPosition = followPosition;
        return;
    }
    if (focusObject.name && camCtrlGetState() == CameraState::LockedOn && plymIsSearchable()) {
        currentFollowCharacter = focusObject;
        currentFollowPosition = focusPosition;
        return;
    }
    focusObject = {};
    if ((input.pressedPriority & 0x200U) || camCtrlGetState() != CameraState::LockedOn || !plymIsSearchable()) {
        currentFollowCharacter = {};
        lastSearchCone = -1;
        return;
    }
    const std::uint16_t pressed = input.pressedPriority, held = input.held;
    if (pressed & 0x1000U)
        cone = (held & 0x4000U) ? 1 : (held & 0x8000U) ? 7 : 0;
    else if (pressed & 0x2000U)
        cone = (held & 0x4000U) ? 3 : 4 + ((held & 0x8000U) != 0);
    else if (pressed & 0x4000U)
        cone = (held & 0x1000U) ? 1 : (held & 0x2000U) ? 3 : 2;
    else if (pressed & 0x8000U)
        cone = (held & 0x1000U) ? 7 : (held & 0x2000U) ? 5 : 6;
    if ((old.name == anchorCharacter.name || !npcGetCanInteract(old)) && cone >= 0 && cone != lastSearchCone) {
        const Vector3 &config = searchCones[cone];
        Vector3 next;
        camCtrlGetNextCamPos(&next);
        const float dx = next.x - g_cameraControl->targetPosition.x, dz = next.z - g_cameraControl->targetPosition.z;
        float x = config.x * dz + config.y * dx, z = config.y * dz - config.x * dx;
        const float length = std::sqrt(x * x + z * z);
        x /= length;
        z /= length;
        float bestDot = -1.0f;
        std::int32_t bestBucket = 999;
        const std::span<shen_name, 32> names = GetScenarioCharacterAdmissionNames();
        const std::span<const Vector3, 32> positions = srchGetScenarioAdmissionPositions();
        for (std::int32_t index = -1; index < 32; ++index) {
            shen_name candidate;
            Vector3 candidatePoint;
            if (index < 0) {
                if (!anchorCharacter.name || anchorCharacter.name == old.name)
                    continue;
                candidate = anchorCharacter;
                candidatePoint = anchorPosition;
            } else {
                if (!names[index].name)
                    break;
                candidate = names[index];
                candidatePoint = positions[index];
                if (candidate.name == old.name)
                    continue;
            }
            const float cx = candidatePoint.x - currentFollowPosition.x, cz = candidatePoint.z - currentFollowPosition.z;
            const float distance = std::sqrt(cx * cx + cz * cz), dot = (z * cz + x * cx) / distance;
            if (distance >= 3.0f || dot <= config.z)
                continue;
            const float scaled = distance * 2.5f - .9f;
            const std::int32_t bucket = scaled >= 0 ? static_cast<std::int32_t>(scaled) : 0;
            if (bucket < bestBucket - 1 || (bucket == bestBucket - 1 && dot > bestDot - .02f) || (bucket == bestBucket && dot > bestDot) ||
                (bucket == bestBucket + 1 && dot > bestDot + .02f)) {
                name = candidate;
                point = candidatePoint;
                bestBucket = bucket;
                bestDot = dot;
            }
        }
    }
    currentFollowCharacter = name;
    currentFollowPosition = point;
    lastSearchCone = cone;
}
Vector3 CameraScaledPoint(const Vector3 &point) {
    Vector3 result;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&g_cameraControl->projectionMatrix);
    Matrix4x4::TransformPoint(point, result);
    Matrix4x4::Pop(1);
    if (std::fabs(result.z) >= std::numeric_limits<float>::epsilon()) {
        const float scale = g_cameraControl->viewHeightScale / -result.z;
        result.x *= scale;
        result.y *= scale;
    }
    return result;
}
bool TargetOccluded(shen_name name, const Vector3 &point, const Vector3 &player) {
    const std::uint8_t marker = static_cast<std::uint8_t>(name.name), second = static_cast<std::uint8_t>(name.name >> 8U);
    if ((marker == 'K' && point.y - player.y > searchRadius) || (marker == 'E' && (second == 'D' || second == 'U'))) {
        const Vector3 origin{player.x, player.y + 1.6f, player.z};
        return flddFindIntersections(point, origin);
    }
    return false;
}
void SelectObject(SearchTaskData &search, SearchTarget &target, const Vector3 &position, const Vector3 &eye, bool clearLook) {
    currentFollowCharacter = {};
    search.currentTarget = &target;
    search.mode = SearchMode::Signboard;
    srchLoadSignBoardModelForArea(search);
    search.fovStyle = searchPropIKIndex ? 7 : 1;
    if (clearLook)
        srchSetLookTarget(nullptr);
    srchSetLookBase(&position);
    srchSetRotY(Angle(position.x - eye.x, position.z - eye.z));
    camCtrlSetLockedOnAngle(target.lockonAngle);
    camCtrlSwitchToLockedOn(search.fovStyle);
    if (target.flags & 4U)
        shenApplyLockonFlag(1);
}
}

// @verified - p
void srchApplyPropHierarchyTransform(MT7Node &node) {
    Vector3 prop;
    GetPropVector4(&prop);
    Matrix4x4::preTranslate(prop.x, prop.y, prop.z);
    Matrix4x4_Push(&transformMatrix_prePass);
    for (MT7Node *parent = node.runtimeParent; parent; parent = parent->runtimeParent) {
        const bool overridden = GetMT7NodeTransformOverride(*parent) != nullptr;
        ApplyInverseNode(*parent);
        if (overridden)
            break;
    }
    Matrix4x4::InvertInPlace(&workTransformMatrix);
    const Matrix4x4 transform = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4::MultiplyWithPrePassMatrix(&transform);
    srchApplyObjectTransform(&node, nullptr);
}

// @verified - p
bool srchGetEulerAngles(const SearchTarget *target, Vector3 *position, std::uint32_t *yaw) {
    if (!target || !position)
        return false;
    *position = {};
    if (yaw)
        *yaw = 0;
    if (static_cast<std::uint8_t>(target->name.name) != 'M') {
        *position = target->position;
        if (yaw)
            *yaw = static_cast<std::uint16_t>(target->yaw) | (static_cast<std::uint32_t>(target->yawLimit) << 16U);
        return true;
    }
    MT7Node *const root = GetPropRoot();
    MT7Node *const node = root ? FindModelCharacterRecursive(root, target->name) : nullptr;
    if (!node)
        return false;
    Matrix4x4_Push(&transformMatrix_prePass);
    srchApplyPropHierarchyTransform(*node);
    MT7Node *current = node;
    if (!current->xb01Offset && current->runtimeChild) {
        current = current->runtimeChild;
        srchApplyObjectTransform(current, nullptr);
    }
    const Vector3 translation = Translation();
    if (current->runtimeChild && IsMDPXorMDP7(current) == IsMDPXorMDP7(current->runtimeChild)) {
        srchApplyObjectTransform(current->runtimeChild, nullptr);
        workTransformMatrix.rows[3].x = translation.x;
        workTransformMatrix.rows[3].y = translation.y;
        workTransformMatrix.rows[3].z = translation.z;
    }
    *position = Translation();
    if (yaw)
        *yaw = static_cast<std::uint16_t>(Angle(workTransformMatrix.rows[2].x, workTransformMatrix.rows[2].z) + target->yaw);
    Matrix4x4::Pop(1);
    return true;
}

// @verified - p
bool srchLockTarget(SearchTaskData &search, shen_name name, bool faceTarget) {
    SearchTarget *entry = nullptr;
    for (std::size_t bank = 0; bank < 4; ++bank)
        for (SearchTarget *target = search.targets[bank]; target && target->name.name; ++target)
            if (target->name.name == name.name)
                entry = target;
    Vector3 position;
    std::uint32_t yaw;
    if (!entry || (entry->flags & 2U) || !srchGetEulerAngles(entry, &position, &yaw))
        return false;
    search.currentTarget = entry;
    search.mode = SearchMode::Signboard;
    srchLoadSignBoardModelForArea(search);
    if (faceTarget) {
        Vector3 player;
        plymUpdatePlayerSearchLocation(&player);
        yaw = static_cast<std::uint16_t>(Angle(position.x - player.x, position.z - player.z));
    }
    srchSetLookBase(&position);
    srchSetRotY(static_cast<std::int16_t>(yaw));
    camCtrlSetLockedOnAngle(entry->lockonAngle);
    camCtrlSwitchToLockedOn(search.fovStyle);
    return true;
}

// @verified - p
void srchUpdate(SearchTaskData &search) {
    UpdateFollowPositions();
    float best = FindBestFollowCandidate(*search.input);
    if (currentFollowCharacter.name && (currentFollowCharacter.name == focusObject.name || currentFollowCharacter.name == GetScenarioFollowCharacter().name)) {
        SwitchDetectedTarget();
        search.currentTarget = nullptr;
        search.state = 0;
        search.fovStyle = 1;
        search.mode = SearchMode::Lockon;
        return;
    }
    if (search.initialCameraState != 22 || (search.input->held & 0x200U) || (search.state & 1U) || search.targetFov < 120 || ctrlGetLookOverlayWeight() > .05f)
        return;
    Vector3 next, player;
    camCtrlGetNextCamPos(&next);
    plymUpdatePlayerSearchLocation(&player);
    SearchTarget *chosen = nullptr;
    Vector3 chosenPosition;
    for (std::size_t bank = 0; bank < 4; ++bank)
        for (SearchTarget *target = search.targets[bank]; target && target->name.name; ++target) {
            const std::uint8_t marker = static_cast<std::uint8_t>(target->name.name);
            if ((target->flags & 2U) || (marker == 'K' && GetCurrentAuthorId().name))
                continue;
            Vector3 point;
            std::uint32_t yaw;
            if (!srchGetEulerAngles(target, &point, &yaw))
                continue;
            if (static_cast<std::int16_t>(AbsAngle(Angle(next.x - point.x, next.z - point.z) - camCtrlGetNextRotY())) > 5461)
                continue;
            const float dx = player.x - point.x, dz = player.z - point.z, dy = player.y - point.y, distance = dz * dz + dx * dx;
            if (marker == 'K') {
                const float range = std::fmax(std::fabs(dy) * 1.5f, 3.0f);
                if (distance > range * range || dy > 0)
                    continue;
            } else if (distance > 9.0f || (!(target->flags & 8U) && (dy < -3 || dy > 0)))
                continue;
            if (target->yawLimit && AbsAngle(Angle(-dx, -dz) - static_cast<std::int16_t>(yaw)) > target->yawLimit)
                continue;
            const Vector3 screen = CameraScaledPoint(point);
            if (screen.z > 0 || std::fabs(screen.x) > 180 || std::fabs(screen.y) > 140)
                continue;
            const float screenDistance = screen.y * screen.y + screen.x * screen.x;
            if (TargetOccluded(target->name, point, player))
                continue;
            if (best > screenDistance) {
                best = screenDistance;
                chosen = target;
                chosenPosition = point;
            }
        }
    if (!chosen) {
        if (currentFollowCharacter.name) {
            SwitchDetectedTarget();
            search.currentTarget = nullptr;
            search.state = 0;
            search.fovStyle = 1;
            search.mode = SearchMode::Lockon;
        }
        return;
    }
    search.state = 0;
    SelectObject(search, *chosen, chosenPosition, next, true);
}

// @verified - p
void srchComputeLockOn(SearchTaskData &search) {
    if (doorGetSearchRegion().name && searchPropIKIndex == 0) {
        srchReset();
        camCtrlSetLookAroundMode();
        return;
    }
    const shen_name previous = currentFollowCharacter;
    UpdateFollowPositions();
    Vector3 locked;
    std::uint32_t lockedYaw = 0;
    if (previous.name) {
        locked = currentFollowPosition;
        DetectTargetFromView(*search.input);
        if (!currentFollowCharacter.name) {
            srchReset();
            camCtrlSetLookAroundMode();
            return;
        }
    } else {
        if (!srchGetEulerAngles(search.currentTarget, &locked, &lockedYaw)) {
            srchReset();
            camCtrlSetLookAroundMode();
            return;
        }
        currentFollowPosition = locked;
        DetectTargetFromView(*search.input);
    }
    if (currentFollowCharacter.name && (currentFollowCharacter.name == focusObject.name || currentFollowCharacter.name == GetScenarioFollowCharacter().name)) {
        SwitchDetectedTarget();
        return;
    }
    Vector3 look;
    srchGetLookTarget(&look);
    look.y -= 1.6f;
    const std::uint16_t yaw = static_cast<std::uint16_t>(shenGetRotYFromNode(Player()));
    const Vector3 forward{-sin_fixed16(yaw), 0, -cos_fixed16(yaw)};
    const bool haveInput = (search.input->pressedPriority & 0xf000U) != 0;
    SearchTarget *chosen = nullptr;
    Vector3 chosenPosition;
    if (haveInput) {
        float x = 0, z = 0;
        const std::uint16_t held = search.input->held;
        if (held & 0x8000U)
            z = 1;
        if (held & 0x4000U)
            z = -1;
        if (held & 0x1000U)
            x = 1;
        if (held & 0x2000U)
            x = -1;
        const std::int16_t direction = Angle(x, z);
        Matrix4x4_Push();
        Vector3 eye;
        srchGetLookTarget(&eye);
        Vector3 axis{eye.x - locked.x, eye.y - locked.y, eye.z - locked.z};
        float length = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        axis.x /= length;
        axis.y /= length;
        axis.z /= length;
        Vector3 right{axis.z, 0, -axis.x};
        length = std::sqrt(right.x * right.x + right.z * right.z);
        right.x /= length;
        right.z /= length;
        const Vector3 up{axis.y * right.z - axis.z * right.y, axis.z * right.x - axis.x * right.z, axis.x * right.y - axis.y * right.x};
        Matrix4x4 inverse{{{right.x, right.y, right.z, 0}, {up.x, up.y, up.z, 0}, {axis.x, axis.y, axis.z, 0}, {eye.x, eye.y, eye.z, 1}}};
        Matrix4x4::InvertInPlace(&inverse);
        std::int32_t best = 32767;
        if (currentFollowCharacter.name && currentFollowCharacter.name != previous.name) {
            Vector3 relative;
            Matrix4x4::SetActiveTransform(&inverse);
            Matrix4x4::TransformPoint(currentFollowPosition, relative);
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-direction));
            Matrix4x4::TransformPoint(relative, relative);
            if (AbsAngle(Angle(relative.y, relative.x)) < 10922) {
                const float q = relative.y * relative.y + relative.x * relative.x;
                best = Angle(q * q, -relative.z);
            }
        }
        for (std::size_t bank = 0; bank < 4; ++bank) {
            if (searchPropIKIndex && bank != 3)
                continue;
            for (SearchTarget *target = search.targets[bank]; target && target->name.name; ++target) {
                const std::uint8_t marker = static_cast<std::uint8_t>(target->name.name);
                if ((search.currentTarget && target->name.name == search.currentTarget->name.name) || (target->flags & 2U) ||
                    (marker == 'K' && GetCurrentAuthorId().name))
                    continue;
                Vector3 point;
                std::uint32_t targetYaw;
                if (!srchGetEulerAngles(target, &point, &targetYaw))
                    continue;
                const Vector3 fromLook{point.x - look.x, point.y - look.y, point.z - look.z};
                if (!searchPropIKIndex) {
                    const float distance = fromLook.z * fromLook.z + fromLook.x * fromLook.x;
                    if (marker == 'K') {
                        if (distance > std::fmax(fromLook.y * fromLook.y * 1.5f, 36.0f) || fromLook.y < 0)
                            continue;
                    } else if (distance > 36 || (!(target->flags & 8U) && (fromLook.y > 3 || fromLook.y < 0)))
                        continue;
                    if (target->yawLimit && AbsAngle(Angle(fromLook.x, fromLook.z) - static_cast<std::int16_t>(targetYaw)) > target->yawLimit)
                        continue;
                    if (fromLook.x * forward.x + (fromLook.y - 1.6f) * forward.y + fromLook.z * forward.z < 0)
                        continue;
                }
                const Vector3 delta{point.x - locked.x, point.y - locked.y, point.z - locked.z};
                if (!searchPropIKIndex && delta.x * delta.x + delta.y * delta.y + delta.z * delta.z > 9)
                    continue;
                if (TargetOccluded(target->name, point, look))
                    continue;
                Vector3 projected;
                Matrix4x4::SetActiveTransform(&inverse);
                Matrix4x4::TransformPoint(point, projected);
                Matrix4x4::SetPrePassToTempTransform();
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-direction));
                Matrix4x4::TransformPoint(projected, projected);
                const std::int32_t angular = AbsAngle(Angle(projected.y, projected.x));
                if (angular > 12743)
                    continue;
                const std::int32_t score = static_cast<std::int32_t>(
                    Angle(std::sqrt(projected.y * projected.y + projected.x * projected.x), -projected.z) - ((angular / 12743.0f) * -1820.0f));
                if (score < best) {
                    best = score;
                    chosen = target;
                    chosenPosition = point;
                }
            }
        }
        Matrix4x4::Pop(1);
    }
    if (chosen) {
        SelectObject(search, *chosen, chosenPosition, look, false);
        return;
    }
    if (currentFollowCharacter.name) {
        SwitchDetectedTarget();
        search.mode = SearchMode::Lockon;
        search.currentTarget = nullptr;
        search.state = 0;
        search.fovStyle = 1;
        selectedSearchName = {};
        search.context->field18 = 0;
        search.context->cursor = nullptr;
        srchRestoreRyoNpcStates(search);
    }
}

// @verified - p
void srchUpdateLockOn(SearchTaskData &search) {
    if (currentFollowCharacter.name) {
        srchComputeLockOn(search);
        if (plymIsSearchable() && !plymIsSearchBlocked() && !ScenarioSearchExcluded())
            SetScenarioFollowCharacter(npcGetCanInteract(currentFollowCharacter) ? currentFollowCharacter : shen_name{});
        SetScenarioFollowGraceFrames(90);
        return;
    }
    if (!shenGetMinutesUntilNextFrameMinute() || search.idleTicks > 10800)
        searchExitRequested = 1;
    if (search.state & 2U) {
        search.state &= ~2U;
        mtwkResetBlends();
        mtwkSetOverridesEnabled(1);
        srchReset();
        camCtrlSetLookAroundMode();
    } else {
        if (!srchProcessSearchIK(search)) {
            if (((plymIsSearchStartRequested() && (search.input->held & 0x200U)) || searchExitRequested) && searchPropIKIndex == 0)
                search.state |= 2U;
            else
                srchComputeLockOn(search);
        }
    }
}

} // namespace shenmue::s2
