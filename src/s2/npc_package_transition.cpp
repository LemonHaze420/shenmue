#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/npc_data_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/math.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/hlib.hpp"
#include <cfloat>
#include <cmath>
#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>
#define SHENMUE_NPC_EPSILON FLT_EPSILON
#include "s2/model_image_component_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/party_companion.hpp"
#define SHENMUE_NPC_NULL 0
#define SHENMUE_NPC_NOEXCEPT
#define SHENMUE_NPC_SIGNED(value) static_cast<std::int32_t>(value)
#include "s2/player_state_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include <cstdio>
#include "s2/texture_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/money_icon.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/gauge.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/pause.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/memo_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/easy_option_state_dc.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/item_selection_queries_dc.hpp"
#include <stdio.h>
#include <string.h>
#define UI_STRLEN ::strlen
#define UI_NOEXCEPT
#define UI_ICON(name) name
#define UI_ZERO
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/hand_motion.hpp"
#endif
#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "s2/directory_cache.hpp"
#include <sg_gd.h>
#define SHENMUE_LOADER_NULL 0
#define SHENMUE_LOADER_ALIGN __attribute__((aligned(32)))
#define SHENMUE_LOADER_INIT =
#define SHENMUE_LOADER_MEMCPY ::memcpy
#define SHENMUE_LOADER_STRCPY ::strcpy
#define SHENMUE_LOADER_STRCMP ::strcmp
#define SHENMUE_LOADER_STRSTR ::strstr
#define SHENMUE_LOADER_STRLEN ::strlen
#define SHENMUE_LOADER_STRNCPY ::strncpy
#define SHENMUE_LOADER_FILE_COUNT (sizeof(g_files) / sizeof(g_files[0]))
#define SHENMUE_LOADER_CONST_BYTES(value) static_cast<const std::byte *>(static_cast<const void *>(value))
#include <zlib.h>
#include "s2/event_overlay.hpp"
#include "s2/map_render_profile.hpp"
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

using namespace npc_package_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace npc_package_detail {

void NpcTransitionDrawNullsub(void *);
void DrawNpcActiveHumans();
std::int32_t ComputeNpcRouteResetPitch(const Vector3 &previous, const Vector3 &next, std::int16_t yaw);
void InterpolateNpcActiveHumans();

} // namespace npc_package_detail
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void DrawNpcActiveHumans();

void ReleaseNpcTransitionSlotPackages();

void ReleaseLoadedNpcTransitionSlotPackages();

void PrepareNpcRouteResetGroups(NpcRouteResetGroupHeader *groups);

void InterpolateNpcActiveHumans();

void ReleaseNpcTransitionRoutePackages();

void ReleaseLoadedNpcTransitionRoutePackages();

std::int32_t ComputeNpcRouteResetPitch(const Vector3 *previous, const Vector3 *next, std::int32_t yaw);

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

void ResetNpcTransitions();

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

void ResetNpcTransitions();

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

void ResetNpcTransitions();

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

void ResetNpcTransitions();

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

void ResetNpcTransitions();

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

void ResetNpcTransitions();

std::uint32_t GetNpcNewLoadState() SHENMUE_NPC_NOEXCEPT;

std::int32_t npcReleasePak(shen_name name);

void ComputeNpcTargetRotation(const Vector3 *position, const Vector3 *target, Vector3i *rotation);

const Vector3 *GetNpcRouteNodeSplinePoints(std::int32_t route);

std::int32_t GetNpcRouteNodePointCount(std::int32_t route);

TimedNodeMotion npcTransitionMotionCaches[4];

shen_name npcInterpolatingCharacter;

extern DateTime npcCurrentDateTime;

void ComputeNpcTargetRotation(const Vector3 *position, const Vector3 *target, Vector3i *rotation);

std::int32_t npcTransitionsEnabled = 0;

std::int32_t InterpolateNpcSplineNormalized(const Vector3 *points, std::int32_t pointCount, std::int32_t parameter, Vector3 *position);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void NpcTransitionDrawNullsub(void *) {
}

#else

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
            } else if (npcNewLoadState && npcLoadHuman(slot.character, slot.character, false) == 1) {
                MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                slot.node = scenFindChara(control ? control->scenarioTask : nullptr, slot.character);
                if (slot.node) {
                    mtwkAssignDrawCallback(slot.node, NpcTransitionDrawNullsub);
                    imgmAssignUpdateCallback(slot.node, charaPrepareNodesForDraw);
                    modelSetCharaAlpha(slot.node, 0.0f);
                    shenApplyTranslationToAll(&slot.node->entity.node, slot.position);
                    rotation.y = slot.yaw;
                    charaSetWorldRot(slot.node, rotation);
                    if (slot.motions)
                        InitializeTimedNodeMotion(npcTransitionMotionCaches[slot.motionCacheIndex], static_cast<MT7Model *>(slot.node->model),
                            slot.motions[static_cast<std::int8_t>(slot.motionCursor)]);
                }
            }
        } else if (slot.node) {
            SetCharacterUnknownFlag(slot.node, 0);
            const float alpha = charaGetAlphaDraw(slot.node);
            if (alpha <= 0.00000011920929f) {
                npcReleasePak(slot.character);
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
                charaSetWorldRot(node, rotation);
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
                } else if (npcNewLoadState && npcLoadHuman(entry->character, entry->character, false) == 1) {
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
                    npcReleasePak(entry->character);
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InitializeNpcRouteResetGroups(NpcRouteResetGroupHeader *groups) {
    npcTransitionsEnabled = 0;
    PrepareNpcRouteResetGroups(groups);
    ResetNpcTransitions();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void DrawNpcTransitions() {
    if (npcTransitionsEnabled) {
        InterpolateNpcActiveHumans();
        DrawNpcActiveHumans();
    }
    npcTransitionsEnabled = 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ReleaseNpcTransitionHumanPackages() {
    ReleaseNpcTransitionRoutePackages();
    ReleaseNpcTransitionSlotPackages();
}

void ReleaseLoadedNpcTransitionPackages() {
    ReleaseLoadedNpcTransitionRoutePackages();
    ReleaseLoadedNpcTransitionSlotPackages();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void DrawNpcActiveHumans() {
    Vector3 view;
    Vector3i rotation;
    NpcTransitionRuntimeSlot *slot = npcTransitionRuntimeSlots;
    std::int32_t index, visible, inside;
    float distance, alpha;
    std::int32_t motionStride;
    void (*setAlpha)(Character *, float);
    rotation.z = rotation.y = rotation.x = 0;
    for (index = 0; index < 4; ++index, ++slot) {
        setAlpha = SetCharacterAlpha;
        motionStride = sizeof(TimedNodeMotion);
        if (!slot->character)
            return;
        Matrix4x4::TransformPoint(slot->position, view);
        distance = Vector3::CalcMagnitude(view);
        inside = camCtrlIsInsideDepthScaledRect(slot->position, 1.0f);
        visible = 0;
        if ((!slot->visibility || (slot->visibility == 1 && distance > 6.5f)) && distance < 25.0f && inside)
            visible = 1;
        if (visible) {
            if (slot->node) {
                alpha = GetCharacterAlpha(slot->node);
                if (alpha < 1.0f)
                    alpha = alpha + 0.05f > 1.0f ? 1.0f : alpha + 0.05f;
                setAlpha(slot->node, alpha);
            } else if (GetNpcNewLoadState() && npcLoadHuman(slot->character, slot->character, 0) == 1) {
                MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                slot->node = scenFindChara(control->scenarioTask, slot->character);
                if (slot->node) {
                    mtwkAssignDrawCallback(slot->node, NpcTransitionDrawNullsub);
                    imgmAssignUpdateCallback(slot->node, charaPrepareNodesForDraw);
                    setAlpha(slot->node, 0.0f);
                    SetCharacterWorldPosition(slot->node, &slot->position);
                    rotation.y = slot->yaw;
                    charaSetWorldRot(slot->node, rotation);
                    ccmClearFlags(slot->node, 2U);
                    if (slot->motionSequence)
                        InitializeTimedNodeMotion(
                            reinterpret_cast<TimedNodeMotion *>(reinterpret_cast<std::byte *>(npcTransitionMotionCaches) + slot->animationSlot * motionStride),
                            slot->node->model,
                            reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(slot->motionSequence[slot->motionCursor])));
                }
            }
        } else if (slot->node) {
            alpha = GetCharacterAlpha(slot->node);
            if (alpha > FLT_EPSILON) {
                alpha = alpha - 0.05f > 0.0f ? alpha - 0.05f : 0.0f;
                setAlpha(slot->node, alpha);
            } else {
                npcReleasePak(slot->character);
                slot->node = 0;
            }
        }
        if (slot->node && slot->motionSequence &&
            TickTimedNodeMotion(
                reinterpret_cast<TimedNodeMotion *>(reinterpret_cast<std::byte *>(npcTransitionMotionCaches) + slot->animationSlot * motionStride))) {
            if (!slot->motionSequence[slot->motionCursor + 1])
                slot->motionCursor = 0;
            else if (slot->motionSequence[slot->motionCursor + 1] != 1)
                ++slot->motionCursor;
            InitializeTimedNodeMotion(
                reinterpret_cast<TimedNodeMotion *>(reinterpret_cast<std::byte *>(npcTransitionMotionCaches) + slot->animationSlot * motionStride),
                slot->node->model, reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(slot->motionSequence[slot->motionCursor])));
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ReleaseNpcTransitionSlotPackages() {
    NpcTransitionRuntimeSlot *slot = npcTransitionRuntimeSlots;
    for (std::int32_t index = 0; index < 4; ++index, ++slot) {
        if (slot->character) {
            npcReleasePak(slot->character);
            slot->node = SHENMUE_NPC_NULL;
        }
    }
}

void ReleaseLoadedNpcTransitionSlotPackages() {
    NpcTransitionRuntimeSlot *slot = npcTransitionRuntimeSlots;
    for (std::int32_t index = 0; index < 4; ++index, ++slot) {
        if (slot->character && npcCancelHumanPak(slot->character))
            slot->node = SHENMUE_NPC_NULL;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void PrepareNpcRouteResetGroups(NpcRouteResetGroupHeader *groups) {
    NpcRouteResetGroupHeader *group = groups;
    NpcRouteResetEntry *entry;
    npcRouteResetGroup = groups;
    while (group->area) {
        if (npcLoadedArea == group->area) {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry) {
                entry->nodeOffset = 0;
                InterpolateNpcSplineNormalized(
                    GetNpcRouteNodeSplinePoints(entry->routeNode), GetNpcRouteNodePointCount(entry->routeNode), entry->parameter, &entry->position);
                entry->previousPosition = entry->position;
                entry->parameter = static_cast<std::int32_t>(static_cast<std::uint32_t>(entry->parameter) + entry->parameterStep);
                InterpolateNpcSplineNormalized(
                    GetNpcRouteNodeSplinePoints(entry->routeNode), GetNpcRouteNodePointCount(entry->routeNode), entry->parameter, &entry->position);
            }
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        } else {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry)
                ;
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InterpolateNpcActiveHumans() {
    Vector3 position;
    Vector3 view;
    Vector3i rotation = {0, 0, 0};
    NpcRouteResetGroupHeader *group = npcRouteResetGroup;
    MT7Node *model = 0;
    std::int32_t difference;
    NpcRouteResetEntry *entry;
    MT7Node *child;
    while (group->area) {
        if (npcLoadedArea == group->area) {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry) {
                npcInterpolatingCharacter = entry->character;
                if (entry->node) {
                    GetCharacterWorldRotation(entry->node, &rotation);
                    difference = rotation.y;
                    rotation.x = rotation.y = rotation.z = 0;
                    ComputeNpcTargetRotation(&entry->previousPosition, &entry->position, &rotation);
                    difference = rotation.y - difference;
                    charaSetWorldRot(entry->node, rotation);
                    model = entry->node->model;
                    model->rotation.x = ComputeNpcRouteResetPitch(&entry->previousPosition, &entry->position, rotation.y);
                    child = FindMT7Node(model, 0x99);
                    if (child)
                        child->rotation.z = static_cast<std::int32_t>(-(static_cast<float>(static_cast<std::int16_t>(difference)) * 1.5f));
                }
                entry->previousPosition = entry->position;
                entry->parameter = static_cast<std::int32_t>(static_cast<std::uint32_t>(entry->parameter) + entry->parameterStep);
                const Vector3 *const points = GetNpcRouteNodeSplinePoints(entry->routeNode);
                const std::int32_t count = GetNpcRouteNodePointCount(entry->routeNode);
                if (!InterpolateNpcSplineNormalized(points, count, entry->parameter, &entry->position)) {
                    entry->parameter = 0;
                    const Vector3 *const resetPoints = GetNpcRouteNodeSplinePoints(entry->routeNode);
                    const std::int32_t resetCount = GetNpcRouteNodePointCount(entry->routeNode);
                    InterpolateNpcSplineNormalized(resetPoints, resetCount, entry->parameter, &entry->position);
                    continue;
                }
                position = entry->previousPosition;
                position.x -= npcRouteOrigin.x;
                position.z -= npcRouteOrigin.z;
                Matrix4x4::TransformPoint(position, view);
                view.z += 8.0f;
                if (entry->node) {
                    SetCharacterWorldPosition(entry->node, &position);
                    difference = rotation.y;
                    ComputeNpcTargetRotation(&entry->previousPosition, &entry->position, &rotation);
                    difference = rotation.y - difference;
                    child = FindMT7Node(model, 0xcb);
                    if (child)
                        child->rotation.y = difference * 9;
                    child = FindMT7Node(model, 0xcc);
                    if (child)
                        child->rotation.y = difference * 9;
                }
                if (Vector3::CalcMagnitude(view) < 30.0f || (camCtrlIsInsideDepthScaledRect(position, 5.0f) && Vector3::CalcMagnitude(view) < 75.0f)) {
                    if (entry->node) {
                        float alpha = GetCharacterAlpha(entry->node);
                        if (alpha < 1.0f)
                            alpha = alpha + 0.05f > 1.0f ? 1.0f : alpha + 0.05f;
                        SetCharacterAlpha(entry->node, alpha);
                    } else if (GetNpcNewLoadState() && npcLoadHuman(entry->character, entry->character, 0) == 1) {
                        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                        entry->node = scenFindChara(control->scenarioTask, entry->character);
                        if (entry->node) {
                            mtwkAssignDrawCallback(entry->node, NpcTransitionDrawNullsub);
                            imgmAssignUpdateCallback(entry->node, charaPrepareNodesForDraw);
                            SetCharacterAlpha(entry->node, 0.0f);
                            if (npcCurrentDateTime.hour < 19) {
                                model = entry->node->model;
                                if (model) {
                                    child = FindMT7Node(model, 0xc4);
                                    if (child) {
                                        child->scale.x = 0.0f;
                                        child->scale.y = 0.0f;
                                        child->scale.z = 0.0f;
                                    }
                                }
                            }
                        }
                    }
                } else if (entry->node) {
                    float alpha = GetCharacterAlpha(entry->node);
                    if (alpha > FLT_EPSILON) {
                        alpha = alpha - 0.05f > 0.0f ? alpha - 0.05f : 0.0f;
                        SetCharacterAlpha(entry->node, alpha);
                    } else {
                        npcReleasePak(entry->character);
                        entry->node = 0;
                    }
                }
            }
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        } else {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry)
                ;
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        }
    }
    npcInterpolatingCharacter = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ReleaseNpcTransitionRoutePackages() {
    NpcRouteResetGroupHeader *group = npcRouteResetGroup;
    NpcRouteResetEntry *entry;
    while (group->area) {
        if (npcLoadedArea == group->area) {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry) {
                npcReleasePak(entry->character);
                entry->nodeOffset = 0;
            }
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        } else {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry)
                ;
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        }
    }
}

// @verified - d
void ReleaseLoadedNpcTransitionRoutePackages() {
    NpcRouteResetGroupHeader *group = npcRouteResetGroup;
    NpcRouteResetEntry *entry;
    while (group->area) {
        if (npcLoadedArea == group->area) {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry) {
                if (npcCancelHumanPak(entry->character))
                    entry->nodeOffset = 0;
            }
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        } else {
            for (entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1); entry->character; ++entry)
                ;
            group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + sizeof(shen_name));
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t ComputeNpcRouteResetPitch(const Vector3 *previous, const Vector3 *next, std::int32_t yaw) {
    Vector3 point = *next;
    Matrix4x4 transform;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(previous->x, previous->y, previous->z);
    Matrix4x4::SetViewMatrixFromEuler(0, yaw, 0);
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    InverseTransformPointByMatrix(&transform, &point);
    if (!(std::abs(point.y) < 0.001f && std::abs(point.z) < 0.001f))
        return 0x8000 - static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(point.y, point.z));
    return 0;
}

#endif

} // namespace s2
} // namespace shenmue
