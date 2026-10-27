#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "s2/npc_package_internal.hpp"

namespace shenmue::s2 {
using namespace npc_package_detail;

namespace npc_package_detail {

void NpcTransitionDrawNullsub(void *) {
}

// @verified - p
void DrawNpcActiveHumans() {
    Vector3i rotation{};
    for (NpcTransitionRuntimeSlot &slot : npcTransitionRuntimeSlots) {
        if (!slot.character.name)
            break;
        Vector3 view;
        Matrix4x4::TransformPoint(slot.position, view);
        const float distance = std::sqrt(view.x * view.x + view.y * view.y + view.z * view.z);
        const bool inside = camCtrlIsInsideDepthScaledRect(slot.position, 1.0f);
        const bool visible = (slot.activationMode == 0 || (slot.activationMode == 1 && distance > 6.5f)) && distance < 25.0f && inside;
        if (visible) {
            if (slot.node) {
                SetCharacterUnknownFlag(slot.node, 0);
                float alpha = charaGetAlphaDraw(slot.node);
                if (alpha < 1.0f) {
                    alpha += 0.050000001f;
                    if (alpha > 1.0f)
                        alpha = 1.0f;
                    SetCharacterUnknownFlag(slot.node, 1);
                }
                modelSetCharaAlpha(slot.node, alpha);
            } else if (npcNewLoadState && LoadNativeNpcHuman(slot.character, slot.character, false) == 1) {
                MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                slot.node = scenFindChara(control ? control->scenarioTask : nullptr, slot.character);
                if (slot.node) {
                    mtwkAssignDrawCallback(slot.node, NpcTransitionDrawNullsub);
                    imgmAssignUpdateCallback(slot.node, charaPrepareNodesForDraw);
                    modelSetCharaAlpha(slot.node, 0.0f);
                    shenApplyTranslationToAll(&slot.node->entity.node, slot.position);
                    rotation.y = slot.yaw;
                    SetCharacterWorldRotation(slot.node, rotation);
                    if (slot.motions)
                        InitializeTimedNodeMotion(npcTransitionMotionCaches[slot.motionCacheIndex], static_cast<MT7Model *>(slot.node->model),
                            slot.motions[static_cast<std::int8_t>(slot.motionCursor)]);
                }
            }
        } else if (slot.node) {
            SetCharacterUnknownFlag(slot.node, 0);
            const float alpha = charaGetAlphaDraw(slot.node);
            if (alpha <= 0.00000011920929f) {
                ReleaseNativeNpcHumanPackage(slot.character);
                slot.node = nullptr;
            } else {
                float nextAlpha = alpha - 0.050000001f;
                if (nextAlpha <= 0.0f)
                    nextAlpha = 0.0f;
                modelSetCharaAlpha(slot.node, nextAlpha);
                SetCharacterUnknownFlag(slot.node, 1);
            }
        }
        if (slot.node) {
            if (slot.motions && TickTimedNodeMotion(npcTransitionMotionCaches[slot.motionCacheIndex])) {
                const std::uint16_t next = slot.motions[static_cast<std::int8_t>(slot.motionCursor) + 1];
                if (!next)
                    slot.motionCursor = 0;
                else if (next != 1 && IncrementSharedPrngFloat() > 0.89999998f)
                    ++slot.motionCursor;
                InitializeTimedNodeMotion(npcTransitionMotionCaches[slot.motionCacheIndex], static_cast<MT7Model *>(slot.node->model),
                    slot.motions[static_cast<std::int8_t>(slot.motionCursor)]);
            }
            SetCharacterCollisionCapsuleMargin(slot.node, slot.controllerToggle);
        }
    }
}

// @verified - p
void ComputeNpcRouteResetFacing(const Vector3 &previous, const Vector3 &next, Vector3i &rotation) {
    rotation.y = NpcRouteYaw(next.x - previous.x, next.z - previous.z);
}

// @verified - p
std::int32_t ComputeNpcRouteResetPitch(const Vector3 &previous, const Vector3 &next, std::int16_t yaw) {
    Matrix4x4 transform;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(previous.x, previous.y, previous.z);
    Matrix4x4::SetViewMatrixFromEuler(yaw, 0, 0);
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    const Vector3 relative{next.x - transform.rows[3].x, next.y - transform.rows[3].y, next.z - transform.rows[3].z};
    const float y = (relative.y * transform.rows[1].y + relative.x * transform.rows[1].x) + relative.z * transform.rows[1].z;
    const float z = (relative.y * transform.rows[2].y + relative.x * transform.rows[2].x) + relative.z * transform.rows[2].z;
    return std::abs(y) >= 0.001f || std::abs(z) >= 0.001f ? static_cast<std::uint16_t>(0x8000U - static_cast<std::int16_t>(FixedAtan2(y, z))) : 0;
}

// @verified - p
void InterpolateNpcActiveHumans() {
    Vector3i rotation{};
    for (NpcRouteResetGroupHeader *group = npcRouteResetGroup; group->area.name;) {
        NpcRouteResetEntry *entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1);
        for (; entry->character.name; ++entry) {
            if (group->area.name != npcLoadedArea.name)
                continue;
            npcInterpolatingCharacter = entry->character;
            Character *node = entry->nodeOffset ? static_cast<Character *>(HLib::GetPointerFromDynamicHeapOffsetBase(entry->nodeOffset)) : nullptr;
            MT7Model *model = nullptr;
            if (node) {
                rotation = GetCharacterWorldRotation(node);
                const std::uint16_t oldYaw = static_cast<std::uint16_t>(rotation.y);
                rotation = {};
                ComputeNpcRouteResetFacing(entry->previousPosition, entry->position, rotation);
                const std::int16_t difference = static_cast<std::int16_t>(rotation.y - oldYaw);
                SetCharacterWorldRotation(node, rotation);
                model = static_cast<MT7Model *>(node->model);
                model->nodes[0].rotation.x = ComputeNpcRouteResetPitch(entry->previousPosition, entry->position, static_cast<std::int16_t>(rotation.y));
                if (MT7Node *const child = GetChildObjectByIndex(*model, model->nodes.data(), 0x99))
                    child->rotation.z = static_cast<std::int32_t>(static_cast<float>(difference) * -1.5f);
            }
            entry->previousPosition = entry->position;
            entry->parameter = static_cast<std::int32_t>(static_cast<std::uint32_t>(entry->parameter) + entry->parameterStep);
            const std::int32_t count = GetNpcRouteNodePointCount(entry->routeNode);
            const Vector3 *const points = GetNpcRouteNodeSplinePoints(entry->routeNode);
            if (!InterpolateNpcSplineNormalized(points, count, entry->parameter, &entry->position)) {
                entry->parameter = 0;
                const std::int32_t resetCount = GetNpcRouteNodePointCount(entry->routeNode);
                const Vector3 *const resetPoints = GetNpcRouteNodeSplinePoints(entry->routeNode);
                InterpolateNpcSplineNormalized(resetPoints, resetCount, 0, &entry->position);
                continue;
            }
            Vector3 position = entry->previousPosition;
            position.x -= npcSchedule.areaOrigin.x;
            position.z -= npcSchedule.areaOrigin.z;
            Vector3 view;
            Matrix4x4::TransformPoint(position, view);
            view.z += 8.0f;
            if (node) {
                shenApplyTranslationToAll(&node->entity.node, position);
                const std::int32_t oldYaw = rotation.y;
                ComputeNpcRouteResetFacing(entry->previousPosition, entry->position, rotation);
                const std::int32_t difference = rotation.y - oldYaw;
                if (MT7Node *const child = GetChildObjectByIndex(*model, model->nodes.data(), 0xcb))
                    child->rotation.y = 9 * difference;
                if (MT7Node *const child = GetChildObjectByIndex(*model, model->nodes.data(), 0xcc))
                    child->rotation.y = 9 * difference;
            }
            const float distance = std::sqrt(view.x * view.x + view.y * view.y + view.z * view.z);
            const bool visible = distance < 30.0f || (camCtrlIsInsideDepthScaledRect(position, 5.0f) && distance < 75.0f);
            if (visible) {
                if (node) {
                    SetCharacterUnknownFlag(node, 0);
                    float alpha = charaGetAlphaDraw(node);
                    if (alpha < 1.0f) {
                        alpha += 0.050000001f;
                        if (alpha > 1.0f)
                            alpha = 1.0f;
                        SetCharacterUnknownFlag(node, 1);
                    }
                    modelSetCharaAlpha(node, alpha);
                } else if (npcNewLoadState && LoadNativeNpcHuman(entry->character, entry->character, false) == 1) {
                    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                    node = scenFindChara(control ? control->scenarioTask : nullptr, entry->character);
                    entry->nodeOffset = node ? HLib::GetDynamicHeapOffset(node) : 0;
                    if (node) {
                        mtwkAssignDrawCallback(node, NpcTransitionDrawNullsub);
                        imgmAssignUpdateCallback(node, charaPrepareNodesForDraw);
                        modelSetCharaAlpha(node, 0.0f);
                        if (npcCurrentDateTime.hour < 19) {
                            model = static_cast<MT7Model *>(node->model);
                            if (model && !model->nodes.empty())
                                if (MT7Node *const child = GetChildObjectByIndex(*model, model->nodes.data(), 0xc4))
                                    child->scale = {};
                        }
                    }
                }
            } else if (node) {
                SetCharacterUnknownFlag(node, 0);
                const float alpha = charaGetAlphaDraw(node);
                if (alpha <= 0.00000011920929f) {
                    ReleaseNativeNpcHumanPackage(entry->character);
                    entry->nodeOffset = 0;
                } else {
                    float nextAlpha = alpha - 0.050000001f;
                    if (nextAlpha <= 0.0f)
                        nextAlpha = 0.0f;
                    modelSetCharaAlpha(node, nextAlpha);
                    SetCharacterUnknownFlag(node, 1);
                }
            }
        }
        group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + 4);
    }
    npcInterpolatingCharacter = {};
}

// @verified - p
void DrawNpcTransitions() {
    if (npcDrawActive) {
        InterpolateNpcActiveHumans();
        DrawNpcActiveHumans();
    }
    npcDrawActive = true;
}
} // namespace npc_package_detail

} // namespace shenmue::s2
