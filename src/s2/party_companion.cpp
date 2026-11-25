#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/party_companion.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/scene_state_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#endif

#include "s2/string_utility.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/npc_data_dc.hpp"
#include "s2/math.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/maze_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/modifier_vertices_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include <cfloat>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <algorithm>
#include <array>
#include <bit>
#endif

#include <cmath>

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cstdio>
#include <cstdlib>
#endif

#include <cstring>

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/conversation_fade_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/scenario_talk_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scenario_dc.hpp"
#include <cri_adxt.h>
#include "s2/conversation_playback_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include "s2/scenario_control_dc.hpp"
#include "s2/scenario_actions_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/scenario_scnt.hpp"
#include "s2/model_dc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/ship_spray.hpp"
#include "s2/hiki.hpp"
#include "s2/event_character_state_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/date_time.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/sound_program.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_region_data_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/flag_data.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/hlib.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/texture_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include <cstdarg>
#include <cstdio>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"
#endif

namespace shenmue {

namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetConversationPlaybackEnabled();

std::int32_t InitializePartyGuide();

std::int32_t PickClosestTargetForPlacement(const Vector3 *point);

std::int32_t GetPartyGuideRouteIndex();

void FindClosestTargetForPlacement(const Vector3 *point);

std::int32_t SetPartyGuideRouteHint(std::int32_t routeIndex);

std::int32_t SetPartyGuideDirection();

std::int32_t PartyGuideEndState();

std::int32_t AdvancePartyGuide(float distance);

std::int32_t SamplePartyGuide(Vector3 *point, std::int16_t *yaw);

std::int16_t CopyHeading(const Vector3 &from, const Vector3 &to);

float PartyGuideLaneBounds(float &low, float &high);

bool GetNpcRouteLaneBounds(shen_name name, std::int32_t parameter, float *low, float *high);

std::int32_t PartyGuideCallbackInitialize();

std::int32_t PartyGuideCallbackPick(const Vector3 *point);

void PartyGuideCallbackFind(const Vector3 *point);

std::int32_t PartyGuideCallbackEndState();

std::int32_t PartyGuideCallbackDirection();

void PartyGuideCallbackRouteHint(std::int32_t routeIndex);

void PartyGuideCallbackSample(Vector3 *point, std::int16_t *yaw);

std::int32_t PartyGuideCallbackAdvance(float distance);

PartyCompanion *FindPartyCompanionByName(shen_name name);

std::int32_t AttachPartyCompanion(Character *character);

void StopConversationStream(ConversationStream &stream);

void ExpandFreeTalkRecord(const std::byte *source, ScenarioFreeTalkCharacter *record);

static PartyCompanionTransition s_partyCompanionTransition;
PartyCompanionTransition *g_pPartyCompanionTransition = &s_partyCompanionTransition;

std::int32_t EV_npcLockUnlock(shen_name name);

std::int32_t IsNaviActive();

bool mtwkCopyTransition(MotionWork *work, const Vector3 *position, std::int32_t yaw, std::int32_t operation, std::int32_t copyType);

Vector3 GetPlayerTargetWorldPosition();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
using namespace model_component_detail;
#endif

namespace player_detail {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
PlayerPlacementNode *plymUpdatePlacementChain(PlayerPlacementNode *node);
#endif

} // namespace player_detail

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t EV_ForceLoadNpc(shen_name name);

std::int32_t GetNpcRestingMotion(const Character *character);

static ProcGenInfo s_procGenInfo;
ProcGenInfo *g_pProcGenInfos = &s_procGenInfo;

std::int32_t mazeFindPartitionDirection(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *partition, std::int32_t *element);

void ReleaseConversationPacket(void *request);
#endif

namespace model_component_detail {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ReleaseCompletedPartyGuideRoute(PartyCompanion &party);
void UpdatePartyCharacterState(PartyCompanion &party);
std::int32_t UpdatePartyPlayerProximity(PartyCompanion &party);
void UpdatePartyControl(PartyCompanion &party);
std::int32_t ResetPartyPlacementNearPlayer(PartyCompanion &party);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdatePartyCompanion(Character *character, void *payload);

float SquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float x = left.x - right.x;
    const float y = left.y - right.y;
    const float z = left.z - right.z;
    return x * x + y * y + z * z;
}
#endif

// @verified - d
float Vector3_ComputeSquaredDistance(const Vector3 &left, const Vector3 &right) {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const float x = left.x - right.x;
#endif
    const float y = left.y - right.y;
    const float z = left.z - right.z;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (std::fabs(x) < FLT_EPSILON && std::fabs(y) < FLT_EPSILON && std::fabs(z) < FLT_EPSILON)
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const float x = left.x - right.x;
    if (std::fabs(x) < kCharacterControlEpsilon && std::fabs(y) < kCharacterControlEpsilon && std::fabs(z) < kCharacterControlEpsilon)
#endif
        return 0.0f;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return x * x + y * y + z * z;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return (y * y + x * x) + z * z;
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t RecordPartyPlayerTrail(PartyCompanion &party, const Vector3 &position) {
    std::int32_t appended = 0;
    std::int32_t append = 0;
    Vector3 delta;
    am2Vector3Subtract(&delta, &position, &party.previousPlayerPosition);
    party.trailDistance += am2Vector3Length(&delta);
    if (!party.trailCount) {
        append = 1;
    } else if (party.trailDistance > 1.0f) {
        std::int32_t previous = party.trailWriteIndex - 1;
        if (previous < 0)
            previous = 31;
        if (Vector3_ComputeSquaredDistance(party.trail[previous], position) > 0.80000001f)
            append = 1;
    }
    if (append) {
        party.trail[party.trailWriteIndex] = position;
        if (++party.trailWriteIndex > 31)
            party.trailWriteIndex = 0;
        if (++party.trailCount > 32)
            party.trailCount = 32;
        party.trailDistance = 0.0f;
        appended = 1;
    }
    party.previousPlayerPosition = position;
    return appended;
}


// @verified - d
void ClearPartyPlayerTrail(PartyCompanion &party) {
    party.trailDistance = 0.0f;
    party.trailWriteIndex = 0;
    party.trailCount = 0;
}


// @verified - d
void InitializePartyCompanion(PartyCompanion &party) {
    party.flags = 0;
    party.name = 0;
    party.character = 0;
    party.behaviorState = 0;
    party.motionState = 0;
    party.mode = 2;
    party.runBlend = 0.0f;
    party.frameCount = 0;
    PartyPlacementNode *placement = &party.placement;
    std::size_t placementSize = sizeof(party.placement);
    std::memset(placement, 0, placementSize);
    std::memset(&party.trailPlacement, 0, placementSize);
    party.selectedPlacement = 0;
    party.movement = &party.embeddedMovement;
    InitializePlayerMovementController(party.movement);
    ClearPartyPlayerTrail(party);
    party.followDistance = 3.5f;
    party.routeMarker = 0;
    party.routeIndex = -1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// The XZ form; a separation under the epsilon on both axes is zero.
// @verified - p
float Vector3_PlanarSquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float z = left.z - right.z;
    const float x = left.x - right.x;
    if (std::fabs(x) < kCharacterControlEpsilon && std::fabs(z) < kCharacterControlEpsilon)
        return 0.0f;
    return z * z + x * x;
}

// @verified - p
Vector3 Vector3_ScaleVectorToMagnitude(const Vector3 &vector, float magnitude) {
    const float scale = magnitude / std::sqrt((vector.x * vector.x + vector.y * vector.y) + vector.z * vector.z);
    return {vector.x * scale, scale * vector.y, scale * vector.z};
}

std::int16_t Vector3_GetAngleBetweenPointsXZ(const Vector3 &from, const Vector3 &to) {
    return InverseFixedAtan2(to.x - from.x, to.z - from.z);
}

// The point in the frame at origin turned by yaw, through the prepass-based work matrix.
Vector3 PartyLocalPosition(const Vector3 &position, const Vector3 &origin, std::int16_t yaw) {
    Vector3 local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    Matrix4x4::TransformPoint(position, local);
    Matrix4x4::Pop(1);
    return local;
}

Vector3 PartyWorldPosition(const Vector3 &local, const Vector3 &origin, std::int16_t yaw) {
    Vector3 world{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::TransformPoint(local, world);
    Matrix4x4::Pop(1);
    return world;
}

// The translation row after offsetting the turned frame, as the placement offsets read it.
Vector3 PartyOffsetPosition(const Vector3 &origin, std::int16_t yaw, float x, float y, float z) {
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::preTranslate(x, y, z);
    const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    return position;
}

float PartyHorizontalDistanceSquared(const Vector3 &left, const Vector3 &right) {
    const float x = left.x - right.x;
    const float z = left.z - right.z;
    return x * x + z * z;
}

// @verified - p
std::int32_t PartyTrailIndex(const PartyCompanion &party, std::int32_t relative) {
    if (!party.trailCount || !relative)
        return -1;
    const std::int32_t direction = relative > 0 ? 1 : -1;
    std::int32_t distance = (relative > 0 ? relative : -relative) - 1;
    if (distance >= 32)
        distance = 31;
    if (distance >= party.trailCount)
        distance = party.trailCount - 1;
    std::int32_t index = party.trailWriteIndex;
    if (party.trailCount < 32 && direction < 0)
        index = 0;
    if (direction > 0 && --index < 0)
        index = 31;
    index -= distance * direction;
    if (index < 0)
        index += 32;
    if (index > 31)
        index -= 32;
    return index;
}

// @verified - p
void RecordPartyPlayerTrail(PartyCompanion &party, const Vector3 &position) {
    party.trailDistance += std::sqrt(SquaredDistance(position, party.previousPlayerPosition));
    const std::int32_t previous = PartyTrailIndex(party, 1);
    if (!party.trailCount || (party.trailDistance > 1.0f && Vector3_ComputeSquaredDistance(party.trail[previous], position) > 0.80000001f)) {
        party.trail[party.trailWriteIndex] = position;
        if (++party.trailWriteIndex >= 32)
            party.trailWriteIndex = 0;
        if (++party.trailCount > 32)
            party.trailCount = 32;
        party.trailDistance = 0.0f;
    }
    party.previousPlayerPosition = position;
}

std::uint16_t PartyIdleMotion(const PartyCompanion &party) {
    std::uint16_t motion = 0;
    if (party.name.name == kPartyNames[0].name)
        motion = 41476;
    else if (party.name.name == kPartyNames[1].name)
        motion = 38147;
    else if (party.name.name == kPartyNames[2].name)
        motion = 32885;
    if (motion && FindLoadedMotionSequence(motion))
        return motion;
    constexpr std::array<std::uint16_t, 16> idleMotions{
        61503, 61535, 61567, 61527, 61599, 61650, 61537, 61575, 61573, 61561, 61529, 61608, 61573, 61605, 61607, 61674};
    const std::int32_t rig = GetCharacterBoneId(party.character);
    motion = rig >= 0 && rig < static_cast<std::int32_t>(idleMotions.size()) ? idleMotions[rig] : 61573;
    if (rig == 8 && party.name.name == kPartyNames[0].name)
        motion = 41476;
    return FindLoadedMotionSequence(motion) ? motion : 61442;
}

// @verified - p
void AdjustPartyControllerFloor(std::uint32_t, Vector3 *target, MotionWork *, void *) {
    if (!target)
        return;
    const Vector3 probe{target->x, target->y - 0.117f + 0.5f, target->z};
    const float floor = shenFindIntersectionWithScene(probe, probe.z);
    if (floor != -(std::numeric_limits<float>::max)()) {
        const float height = floor + 0.117f;
        if (height > target->y)
            target->y = height;
    }
}
#endif

// @verified - d
void ConfigurePartyMotion(PartyCompanion &party) {
    MotionWork *const work = party.motionWork;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    mtwkCopyTransition(work, 0, 0, 1, 0);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    mtwkCopyTransition(work, nullptr, 0, 1, 0);
    const std::uint16_t idle = PartyIdleMotion(party);
    mtwkSetRequestedRootMode(work, 1);
    mtwkSetFrameTime(work, 20, MotionRegion::LowerBody);
    mtwkRequestMotion(work, idle, MotionRegion::LowerBody);
    mtwkSetTimeRelated(work, 1, 0, idle);
    mtwkSetExtraFlag(work, 55, 0);
    mtwkAssignTargetCallback(work, AdjustPartyControllerFloor, 25);
    mtwkAssignTargetCallback(work, AdjustPartyControllerFloor, 30);
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ReleaseCompletedPartyGuideRoute(PartyCompanion &party) {
    if (!IsNpcRouteReady(SHEN_NAME(M01_)) || !InitializePartyGuide())
        return 0;
    Vector3 position;
    GetCharacterWorldPosition(party.character, &position);
    PickClosestTargetForPlacement(&position);
    FindClosestTargetForPlacement(0);
    if (SetPartyGuideDirection() < 1) {
        const shen_name guideName = SHEN_NAME(M01_);
        StopNpcScriptRoute(0, guideName, 0);
        EV_npcLockUnlock(guideName);
    }
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void plymUpdatePlacementNode(PartyPlacementNode &node) {
    node.delta = {node.position.x - node.previousPosition.x, node.position.y - node.previousPosition.y, node.position.z - node.previousPosition.z};
    node.movementBearingYaw = std::fabs(node.delta.x) < kCharacterControlEpsilon || std::fabs(node.delta.z) < kCharacterControlEpsilon ?
        0 :
        static_cast<std::int16_t>(FixedAtan2(node.delta.x, node.delta.z) + 0x8000U);
    node.previousPosition = node.position;
    const Vector3 location = GetCharacterWorldPosition(node.character);
    const std::int16_t yaw = GetAllAngYForNode(&node.character->entity.node);
    Vector3 offset{node.position.x - location.x, node.position.y - location.y, node.position.z - location.z};
    const float distance = std::sqrt(offset.z * offset.z + offset.x * offset.x);
    const auto bearing = static_cast<std::int16_t>(FixedAtan2(offset.x, offset.z) + 0x8000U);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::Transform(offset, offset);
    Matrix4x4::Pop(1);
    node.distance = distance;
    if (node.minimumDistance > distance)
        node.minimumDistance = distance;
    std::uint32_t flags = node.flags;
    if (offset.z <= 0.0f)
        flags &= ~6U;
    else {
        if (!(flags & 2U))
            flags |= 4U;
        flags |= 2U;
    }
    flags &= ~0x4000U;
    if ((node.delta.x * node.delta.x + node.delta.y * node.delta.y) + node.delta.z * node.delta.z >= 0.0001f)
        node.stationaryFrames = 0;
    else {
        if (++node.stationaryFrames >= 4)
            node.stationaryFrames = 4;
        if (node.stationaryFrames == 4)
            flags |= 0x4000U;
    }
    node.distance = distance;
    node.flags = flags;
    node.characterBearingYaw = bearing;
}

// Each linked node takes the head's character; the last node is the one followed.
// @verified - p
PartyPlacementNode *plymUpdatePlacementChain(PartyPlacementNode *node) {
    if (!node)
        return nullptr;
    Character *const character = node->character;
    plymUpdatePlacementNode(*node);
    for (PartyPlacementNode *next = node->next; next; next = next->next) {
        next->character = character;
        node = next;
        plymUpdatePlacementNode(*next);
    }
    return node;
}

// @verified - p
void PreparePartyFollowPlacement(PartyCompanion &party) {
    if (!ctrlGetRootNode())
        return;
    Vector3 player = GetPlayerTargetWorldPosition();
    RecordPartyPlayerTrail(party, player);
    Vector3 position = party.position;
    const PlayerMovementState &movement = *party.movement;
    float height = position.y;
    if (movement.flags & 8U) {
        const Vector3 local = PartyLocalPosition(position, movement.targetWorldPosition, movement.targetWorldYaw);
        height = std::fmax(0.0f, -local.z * 0.74999994f);
        if (static_cast<float>(movement.targetHeightScale) * 0.22499999f <= height)
            height = static_cast<float>(movement.targetHeightScale) * 0.22499999f;
        height += movement.targetWorldPosition.y;
    }
    player.y += 0.40000001f;
    position.y = height + 0.40000001f;
    Vector3 offset{position.x - player.x, position.y - player.y, position.z - player.z};
    const float distance = Vector3::NormalizeInPlace(offset);
    const float length = distance <= party.followDistance ? distance : party.followDistance;
    offset = {offset.x * length, offset.y * length, offset.z * length};
    Vector3 target{player.x + offset.x, player.y + offset.y, player.z + offset.z};
    target.y += 1.5f;
    const float ground = shenFindIntersectionWithScene(target, target.y);
    target.y = ground == kNoSceneIntersection ? player.y : ground + 0.40000001f;
    party.placement.position = target;
    party.placement.targetYaw = static_cast<std::int16_t>(InverseFixedAtan2(offset.x, offset.z) + 0x8000);
    if (party.flags & 0x20000U) {
        party.placement.position = party.forcedTarget;
        party.placement.targetYaw = party.forcedYaw;
    }
    if (party.flags & 0x80000U) {
        std::int32_t nearestTarget = -1;
        std::int32_t nearestParty = -1;
        float targetDistance = (std::numeric_limits<float>::max)();
        float partyDistance = (std::numeric_limits<float>::max)();
        for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
            const std::int32_t index = PartyTrailIndex(party, relative);
            if (index < 0)
                break;
            const float fromTarget = Vector3_PlanarSquaredDistance(party.placement.position, party.trail[index]);
            if (targetDistance > fromTarget) {
                targetDistance = fromTarget;
                nearestTarget = relative - 1;
            }
        }
        for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
            const std::int32_t index = PartyTrailIndex(party, relative);
            if (index < 0)
                break;
            const float fromParty = Vector3_PlanarSquaredDistance(position, party.trail[index]);
            if (partyDistance > fromParty) {
                partyDistance = fromParty;
                nearestParty = relative - 1;
            }
        }
        if (party.selectedTrailIndex >= 0) {
            std::int32_t retained = -1;
            if (party.trailCount) {
                retained = party.trailWriteIndex - (party.selectedTrailIndex + 1);
                if (retained < 0)
                    retained += party.trailCount;
                if (retained > party.trailCount - 1)
                    retained -= party.trailCount;
            }
            if (retained < nearestParty)
                nearestParty = retained;
        }
        if (nearestParty > nearestTarget && nearestParty > 0) {
            const float minimumDistance = party.runBlend >= 0.5f ? 1.6500001f : 1.1f;
            for (std::int32_t relative = nearestParty + 1; relative - 1 >= nearestTarget; --relative) {
                const std::int32_t index = PartyTrailIndex(party, relative);
                if (index < 0)
                    break;
                if (minimumDistance * minimumDistance <= Vector3_PlanarSquaredDistance(position, party.trail[index])) {
                    party.placement.next = &party.trailPlacement;
                    party.trailPlacement.position = party.trail[index];
                    break;
                }
            }
        }
        party.selectedTrailIndex = static_cast<std::int8_t>(PartyTrailIndex(party, nearestParty + 1));
        if (party.placement.next)
            return;
    } else {
        party.selectedTrailIndex = -1;
    }
    flddToggleFlag(0x100);
    if (!flddFindIntersections(player, position))
        return;
    Vector3 previous = target;
    for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
        const std::int32_t index = PartyTrailIndex(party, relative);
        if (index < 0)
            break;
        Vector3 sample = party.trail[index];
        sample.y += 0.40000001f;
        flddToggleFlag(0x100);
        if (!flddFindIntersections(sample, position)) {
            Vector3 chosen = sample;
            if (Vector3_ComputeSquaredDistance(sample, position) < 4.0f) {
                flddPushIntersectionFlag(0x40);
                if (flddFindIntersections(sample, previous)) {
                    chosen = previous;
                    party.trailPlacement.flags |= 0x40000U;
                }
            }
            party.placement.next = &party.trailPlacement;
            party.trailPlacement.position = chosen;
            break;
        }
        previous = sample;
    }
    if (!party.placement.next)
        party.placement.flags |= 8U;
}

// @verified - p
bool PartySegmentHitsCylinder(const Vector3 &start, const Vector3 &end, const Vector3 &center, float radius) {
    const float radiusSquared = radius * radius;
    const float maximumY = center.y + 3.25f;
    if (PartyHorizontalDistanceSquared(start, center) <= radiusSquared && start.y >= center.y && start.y <= maximumY)
        return true;
    const Vector3 delta{end.x - start.x, end.y - start.y, end.z - start.z};
    if (!(std::abs(delta.x) >= kCharacterControlEpsilon || std::abs(delta.z) >= kCharacterControlEpsilon || std::abs(delta.z) >= kCharacterControlEpsilon))
        return false;
    if (PartyHorizontalDistanceSquared(end, center) <= radiusSquared && end.y >= center.y && end.y <= maximumY)
        return true;
    const float fraction = ((center.x - start.x) * delta.x + (center.z - start.z) * delta.z) / (delta.z * delta.z + delta.x * delta.x);
    if (!(fraction >= 0.0f && fraction <= 1.0f))
        return false;
    const Vector3 nearest{start.x + fraction * delta.x, start.y + fraction * delta.y, start.z + fraction * delta.z};
    return PartyHorizontalDistanceSquared(nearest, center) <= radiusSquared && nearest.y >= center.y && nearest.y <= maximumY;
}

// Three probes from the companion toward the followed node, offset 0.4 to either side; the nearest blocker is kept.
// @verified - p
void FindPartyObstacles(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    Vector3 sideways{0.40000001f, 0.0f, 0.0f};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(placement.characterBearingYaw));
    Matrix4x4::TransformPoint(sideways, sideways);
    Matrix4x4::Pop(1);
    float plusDistance = 10.0f;
    float minusDistance = 10.0f;
    float centreDistance = 10.0f;
    party.obstacleDistance = 10.0f;
    party.obstacleCharacter = nullptr;
    const Vector3 start{party.position.x, party.position.y + 0.40000001f, party.position.z};
    const Vector3 end{placement.position.x, placement.position.y + 0.40000001f, placement.position.z};
    const Vector3 plusStart{start.x + sideways.x, start.y + sideways.y, start.z + sideways.z};
    const Vector3 plusEnd{end.x + sideways.x, end.y + sideways.y, end.z + sideways.z};
    const Vector3 minusStart{start.x - sideways.x, start.y - sideways.y, start.z - sideways.z};
    const Vector3 minusEnd{end.x - sideways.x, end.y - sideways.y, end.z - sideways.z};
    ccmUpdateEntityFlag(party.character);
    Character *const centre = CheckPointInCylinder_Wrapper(start, end);
    if (centre)
        centreDistance = Vector3_PlanarSquaredDistance(start, GetCharacterWorldPosition(centre));
    ccmUpdateEntityFlag(party.character);
    Character *const minus = CheckPointInCylinder_Wrapper(minusStart, minusEnd);
    if (minus)
        minusDistance = Vector3_PlanarSquaredDistance(minusStart, GetCharacterWorldPosition(minus));
    ccmUpdateEntityFlag(party.character);
    Character *const plus = CheckPointInCylinder_Wrapper(plusStart, plusEnd);
    if (plus)
        plusDistance = Vector3_PlanarSquaredDistance(plusStart, GetCharacterWorldPosition(plus));
    if (minusDistance <= centreDistance || plusDistance <= centreDistance) {
        if (plusDistance <= minusDistance) {
            party.obstacleDistance = plusDistance;
            party.obstacleCharacter = plus;
        } else {
            party.obstacleDistance = minusDistance;
            party.obstacleCharacter = minus;
        }
    } else {
        party.obstacleDistance = centreDistance;
        party.obstacleCharacter = centre;
    }
    if (party.obstacleCharacter)
        party.obstacleDistance = std::sqrt(party.obstacleDistance);
}

// A blocker within 3 units appends the avoidance node one unit to its free side.
// @verified - p
void SelectPartyAvoidancePlacement(PartyCompanion &party) {
    PartyPlacementNode &placement = *party.selectedPlacement;
    if (!party.obstacleCharacter) {
        party.flags &= 0xffff3fffU;
        return;
    }
    const Vector3 local = PartyLocalPosition(GetCharacterWorldPosition(party.obstacleCharacter), party.position, placement.characterBearingYaw);
    std::int16_t direction = 0;
    if (!(party.flags & 0xc000U))
        direction = local.x < 0.0f ? 0x4000 : static_cast<std::int16_t>(0x8000);
    if (party.obstacleDistance > 3.0f)
        return;
    const float side = direction < 0 ? -1.0f : 1.0f;
    PartyPlacementNode &avoidance = party.avoidancePlacement;
    avoidance.position = PartyOffsetPosition(party.position, placement.characterBearingYaw, side, 0.0f, -party.obstacleDistance);
    Vector3 hit{};
    if (RaycastForCollision(&hit, &avoidance.position, &party.position))
        avoidance.position = PartyOffsetPosition(party.position, placement.characterBearingYaw, -side, 0.0f, -party.obstacleDistance);
    avoidance.targetYaw = placement.targetYaw;
    placement.next = &avoidance;
    party.selectedPlacement = &avoidance;
}
#endif

} // namespace model_component_detail

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace model_component_detail

// The guide restarts on the area's KMZ maze, or on the M01_ route once its NPC is placed. The high bound and
// the target survive from the previous walk.
// @verified - p
std::int32_t InitializePartyGuide() {
    ProcGenInfo &guide = g_procGenInfos;
    guide.initialised = false;
    guide.maze = false;
    guide.routeIndex = -1;
    guide.parameter = 0.0f;
    guide.name = {};
    guide.currentParameter = 0;
    guide.low = 0;
    guide.stride = 1;
    guide.maximum = 0;
    guide.direction = 1;
    guide.partition = -1;
    guide.partitionCount = 0;
    if (MatchAreaNamePattern(g_currentMap, AREA_PATTERN_MAZE)) {
        guide.maze = true;
        guide.partitionCount = static_cast<std::int8_t>(mazeGetNumPartitions());
    } else {
        guide.maze = false;
        guide.name = SHEN_NAME(M01_);
        guide.currentParameter = GetNpcRouteCurrentParameter(guide.name);
        guide.stride = GetNpcRouteStride(guide.name);
        guide.low = GetNpcRouteLowParameter(guide.name);
        guide.high = GetNpcRouteHighParameter(guide.name);
        guide.maximum = GetNpcRouteMaximumParameter(guide.name);
        guide.parameter = static_cast<float>(guide.currentParameter);
        if (!IsNpcRouteReady(guide.name))
            return 0;
    }
    guide.initialised = true;
    return 1;
}

// Native PickClosestTargetForPlacement: the walk starts at the route point (or maze partition element, seeded by the route hint)
// nearest `point`; the start becomes the new route hint.
// @verified - p
std::int32_t PickClosestTargetForPlacement(const Vector3 *point) {
    ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return 0;
    if (!guide.maze) {
        guide.currentParameter = FindNearestNpcRouteParameter(guide.name, point);
        guide.parameter = static_cast<float>(guide.currentParameter);
    } else {
        std::int32_t partition = -1;
        std::int32_t element = -1;
        if (guide.routeIndex != -1) {
            partition = static_cast<std::uint16_t>(guide.routeIndex >> 16);
            element = static_cast<std::uint16_t>(guide.routeIndex);
        }
        mazeFindClosestTarget(&partition, &element, point);
        guide.partition = static_cast<std::int8_t>(partition);
        guide.currentParameter = element;
        guide.low = 0;
        guide.parameter = static_cast<float>(element);
        guide.high = GetMZNRDataForID(guide.partition) - 1;
        guide.maximum = guide.high;
    }
    guide.routeIndex = guide.maze ? static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::int32_t>(guide.partition)) << 16U) |
            static_cast<std::uint16_t>(guide.currentParameter) :
                                    guide.currentParameter;
    return guide.routeIndex;
}

// Native FindClosestTarget_Wrapper: the walk's goal is the route point (or maze element) nearest `point`, or the end of the current
// partition without one.
// @verified - p
void FindClosestTargetForPlacement(const Vector3 *point) {
    ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return;
    if (!point) {
        guide.targetPartition = guide.partition;
        guide.targetParameter = guide.high;
    } else if (!guide.maze) {
        guide.targetPartition = guide.partition;
        guide.targetParameter = FindNearestNpcRouteParameter(guide.name, point);
    } else {
        std::int32_t partition = -1;
        std::int32_t element = -1;
        if (guide.routeIndex != -1) {
            partition = static_cast<std::uint16_t>(guide.routeIndex >> 16);
            element = static_cast<std::uint16_t>(guide.routeIndex);
        }
        mazeFindClosestTarget(&partition, &element, point);
        guide.targetPartition = partition;
        guide.targetParameter = element;
    }
}

// The PLYM placement builders seed the route hint.
// @verified - p
void SetPartyGuideRouteHint(std::int32_t routeIndex) {
    g_procGenInfos.routeIndex = routeIndex;
}

// The walk's direction toward its goal (maze: through the partition graph).
// @verified - p
std::int32_t SetPartyGuideDirection() {
    ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return 0;
    std::int32_t direction = 0;
    if (guide.maze) {
        std::int32_t partition, element;
        direction = mazeFindPartitionDirection(
            guide.partition, static_cast<std::int32_t>(guide.parameter), guide.targetPartition, guide.targetParameter, &partition, &element);
    } else {
        const float target = static_cast<float>(guide.targetParameter);
        direction = target > guide.parameter;
        if (guide.parameter > target)
            direction = -1;
    }
    guide.direction = static_cast<std::int8_t>(direction);
    return direction;
}

// 1 once the walk reaches its goal (or has no direction), -1 at the end of a partition short of it, 0 on the
// way, 99 without a guide.
// @verified - p
std::int32_t PartyGuideEndState() {
    const ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised)
        return 99;
    if (!guide.direction)
        return 1;
    if (guide.direction > 0) {
        if (guide.partition == guide.targetPartition) {
            if (guide.parameter >= static_cast<float>(guide.targetParameter))
                return 1;
        } else if (guide.parameter >= static_cast<float>(guide.high))
            return -1;
    } else {
        if (guide.partition == guide.targetPartition) {
            if (static_cast<float>(guide.targetParameter) >= guide.parameter)
                return 1;
        } else if (static_cast<float>(guide.low) >= guide.parameter)
            return -1;
    }
    return 0;
}

// A maze walk at the end of its partition crosses into the next one toward the goal.
// @verified - p
std::int32_t AdvancePartyGuidePartition() {
    ProcGenInfo &guide = g_procGenInfos;
    if (guide.maze) {
        std::int32_t partition = guide.partition;
        std::int32_t element = static_cast<std::int32_t>(guide.parameter);
        if (!mazeFindPartitionDirection(guide.partition, element, guide.targetPartition, guide.targetParameter, &partition, &element))
            return 0;
        guide.partition = static_cast<std::int8_t>(partition);
        guide.low = 0;
        guide.parameter = static_cast<float>(element);
        guide.high = GetMZNRDataForID(partition) - 1;
        guide.direction = static_cast<std::int8_t>(mazeFindPartitionDirection(
            guide.partition, static_cast<std::int32_t>(guide.parameter), guide.targetPartition, guide.targetParameter, &partition, &element));
    }
    return 1;
}

// The walk moves `distance` along the route (maze: two-unit steps), clamped at the goal and the route ends;
// 1 when it arrives.
// @verified - p
std::int32_t AdvancePartyGuide(float distance) {
    ProcGenInfo &guide = g_procGenInfos;
    std::int32_t steps = 0;
    if (guide.maze) {
        steps = static_cast<std::int32_t>((distance + 1.99999f) * 0.5f);
    } else {
        float length = 0.0f;
        std::int32_t parameter = static_cast<std::int32_t>(guide.parameter);
        if (guide.direction > 0) {
            while (static_cast<float>(guide.maximum) > static_cast<float>(parameter)) {
                const float segment = GetNpcRouteSegmentLength(guide.name, parameter++);
                if (segment < 0.0f)
                    break;
                length += segment;
                ++steps;
                if (length > distance)
                    break;
            }
        } else {
            while (parameter > 0) {
                const float segment = GetNpcRouteSegmentLength(guide.name, --parameter);
                if (segment < 0.0f)
                    break;
                length += segment;
                ++steps;
                if (length > distance)
                    break;
            }
        }
    }
    guide.parameter += static_cast<float>(steps * guide.direction);
    if (guide.direction > 0) {
        if (guide.partition == guide.targetPartition && guide.parameter > static_cast<float>(guide.targetParameter))
            guide.parameter = static_cast<float>(guide.targetParameter);
        if (guide.parameter > static_cast<float>(guide.maximum))
            guide.parameter = static_cast<float>(guide.maximum);
    } else if (guide.direction < 0) {
        if (guide.partition == guide.targetPartition && static_cast<float>(guide.targetParameter) > guide.parameter)
            guide.parameter = static_cast<float>(guide.targetParameter);
        if (guide.parameter < 0.0f)
            guide.parameter = 0.0f;
    }
    const std::int32_t end = PartyGuideEndState();
    if (end > 0)
        return 1;
    if (end < 0)
        AdvancePartyGuidePartition();
    return 0;
}

std::int32_t PartyGuideParameter() {
    const ProcGenInfo &guide = g_procGenInfos;
    return std::min(std::max(static_cast<std::int32_t>(guide.parameter), guide.low), guide.high);
}

// The walk's point and the heading along the route there.
// @verified - p
void SamplePartyGuide(Vector3 *point, std::int16_t *yaw) {
    const ProcGenInfo &guide = g_procGenInfos;
    const std::int32_t parameter = PartyGuideParameter();
    if (!guide.maze) {
        std::int32_t angle = 0;
        SampleNpcRoute(guide.name, parameter, point, &angle);
        *yaw = static_cast<std::int16_t>(angle);
        return;
    }
    mazeSamplePartition(guide.partition, parameter, point);
    Vector3 adjacent{};
    const bool forward = guide.direction >= 0;
    if ((forward && parameter < guide.high) || (!forward && parameter > guide.low)) {
        mazeSamplePartition(guide.partition, parameter + (forward ? 1 : -1), &adjacent);
        *yaw = Vector3_GetAngleBetweenPointsXZ(*point, adjacent);
    } else {
        mazeSamplePartition(guide.partition, parameter + (forward ? -1 : 1), &adjacent);
        *yaw = Vector3_GetAngleBetweenPointsXZ(adjacent, *point);
    }
}

// The walk's goal point.
// @verified - p
std::int32_t SamplePartyGuideTarget(Vector3 *point) {
    const ProcGenInfo &guide = g_procGenInfos;
    if (!guide.initialised || !point)
        return -1;
    if (guide.maze) {
        mazeSamplePartition(guide.targetPartition, guide.targetParameter, point);
    } else {
        std::int32_t angle = 0;
        SampleNpcRoute(guide.name, guide.targetParameter, point, &angle);
    }
    return 0;
}
#endif

namespace model_component_detail {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Once the player stands past M01_'s goal, the route NPC is released.
void ReleaseCompletedPartyGuideRoute() {
    constexpr shen_name guideName{SHEN_NAME(M01_)};
    if (g_currentMap.name == SHEN_NAME(KSH1) || !IsNpcRouteReady(guideName) || !InitializePartyGuide())
        return;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    PickClosestTargetForPlacement(&player);
    FindClosestTargetForPlacement(nullptr);
    if (SetPartyGuideDirection() <= 0) {
        ReleaseNpcRouteControl(nullptr, guideName, 0);
        EV_npcLockUnlock(guideName);
    }
}

void PartyGuideLaneBounds(float &low, float &high) {
    low = -1.5f;
    high = 1.5f;
    if (!g_procGenInfos.maze)
        GetNpcRouteLaneBounds(g_procGenInfos.name, PartyGuideParameter(), &low, &high);
}

void PreparePartyGuidePlacement(PartyCompanion &party) {
    if (!InitializePartyGuide()) {
        party.placement.flags |= 8U;
        return;
    }
    g_procGenInfos.routeIndex = party.routeIndex;
    party.routeIndex = PickClosestTargetForPlacement(&party.position);
    Vector3 goal{-111.4f, 23.511999f, -132.3f};
    if ((party.flags & 0x40000000U) && party.routeMarker)
        mazeGetMarkerPosition(party.routeMarker, &goal);
    FindClosestTargetForPlacement((party.flags & 0x40000000U) ? &goal : nullptr);
    SamplePartyGuideTarget(&party.routeGoal);
    SetPartyGuideDirection();
    if (PartyGuideEndState() > 0) {
        party.placement.flags |= 0x20U;
        return;
    }
    Vector3 sample{};
    std::int16_t yaw = 0;
    SamplePartyGuide(&sample, &yaw);
    party.routeYaw = yaw;
    float low = 0.0f, high = 0.0f;
    PartyGuideLaneBounds(low, high);
    party.routeWidth = std::abs(low) + std::abs(high);
    const Vector3 local = PartyLocalPosition(sample, party.position, party.yaw);
    if ((local.z > -1.0f || Vector3_PlanarSquaredDistance(party.position, sample) < 2.25f) && PartyGuideEndState() <= 0)
        AdvancePartyGuide(1.0f);
    if (PartyGuideEndState() <= 0)
        AdvancePartyGuide(2.0f);
    SamplePartyGuide(&sample, &yaw);
    if (Vector3_ComputeSquaredDistance(party.position, sample) < 1.0f && PartyGuideEndState() <= 0) {
        AdvancePartyGuide(2.0f);
        SamplePartyGuide(&sample, &yaw);
    }
    party.placement.position = sample;
    party.placement.targetYaw = yaw;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    player = PartyLocalPosition(player, party.position, party.routeYaw);
    PartyGuideLaneBounds(low, high);
    const float lateral = std::min(std::fmax(-1.0f, player.x >= 0.0f ? low + 0.75f : high - 0.75f), 1.0f);
    party.routeLateralOffset += (lateral - party.routeLateralOffset) * 0.079999998f;
    party.placement.position = PartyOffsetPosition(party.placement.position, party.placement.targetYaw, party.routeLateralOffset, 0.0f, 0.0f);
    flddToggleFlag(1064);
    if (!GlobalRayIntersectionTest(party.position, party.placement.position, 0.34999999f))
        return;
    InitializePartyGuide();
    PickClosestTargetForPlacement(&party.placement.position);
    g_procGenInfos.routeIndex = party.routeIndex;
    FindClosestTargetForPlacement(&party.position);
    SetPartyGuideDirection();
    while (PartyGuideEndState() <= 0) {
        SamplePartyGuide(&sample, &yaw);
        sample.y += 0.40000001f;
        flddToggleFlag(1064);
        if (!GlobalRayIntersectionTest(party.position, sample, 0.34999999f)) {
            party.placement.next = &party.trailPlacement;
            party.trailPlacement.position = sample;
            return;
        }
        AdvancePartyGuide(5.0f);
    }
}

// @verified - p
void PreparePartyPlacement(PartyCompanion &party) {
    party.placement.character = party.character;
    party.placement.next = nullptr;
    party.trailPlacement.next = nullptr;
    party.avoidancePlacement.next = nullptr;
    party.placement.flags &= 0x7ffbffd7U;
    party.trailPlacement.flags &= 0x7ffbf7d7U;
    party.trailPlacement.flags |= 0x800U;
    party.avoidancePlacement.flags &= 0x7ff9ffd7U;
    party.avoidancePlacement.flags |= 0x20000U;
    if (party.mode == 1)
        PreparePartyFollowPlacement(party);
    else if (party.mode == 2)
        PreparePartyGuidePlacement(party);
    party.selectedPlacement = plymUpdatePlacementChain(&party.placement);
}

void PublishPartyRelocation(PartyCompanion &party, const Vector3 &position) {
    shenApplyTranslationToAll(&party.character->entity.node, position);
    party.position = position;
    party.character->alpha = 0.0099999998f;
    if (party.motionWork)
        mtwkSetRegionFlag(party.motionWork, 71, 0, MotionRegion::LowerBody);
}

// Out of sight and out of reach, the companion reappears behind the player (guide: ahead of them). Returns -1 when
// already close, -2 when a trail sample still sees it, -3 when nothing separates the two bodies.
// @verified - p
std::int32_t ResetPartyPlacementNearPlayer(PartyCompanion &party) {
    Vector3 offset{0.0f, 0.0f, -1.0f};
    Vector3 position = GetCharacterWorldPosition(party.character);
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    auto yaw = static_cast<std::int16_t>(playerGetRotY());
    player.y = player.y + 0.40000001f;
    position.y = position.y + 0.40000001f;
    Vector3 hit{};
    const bool blocked = RaycastForCollision(&hit, &player, &position);
    player.y = player.y + -0.40000001f;
    position.y = position.y + -0.40000001f;
    const float distance = Vector3_PlanarSquaredDistance(position, player);
    if (blocked) {
        if (std::fabs(position.y - player.y) < 2.0f && distance < 100.0f) {
            Vector3 playerBone{}, partyBone{};
            charaGetMotWorldPos(ctrlGetRootNode(), 6, &playerBone);
            charaGetMotWorldPos(party.character, 6, &partyBone);
            if (!flddFindIntersections(playerBone, partyBone))
                return -3;
        }
    } else if (distance < 900.0f && std::fabs(position.y - player.y) < 5.0f)
        return -1;
    position.y = position.y + 0.40000001f;
    if (party.mode == 1) {
        for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
            const std::int32_t index = PartyTrailIndex(party, relative);
            if (index < 0)
                break;
            const Vector3 sample{party.trail[index].x, party.trail[index].y + 0.40000001f, party.trail[index].z};
            if (!flddFindIntersections(sample, position))
                return -2;
        }
    }
    offset.z = party.mode == 2 ? -1.5f : -1.25f;
    position = PartyWorldPosition(offset, player, static_cast<std::int16_t>(yaw + (party.mode == 2 ? -8192 : -24576)));
    if (RaycastForCollision(&hit, &position, &player)) {
        const Vector3 back = Vector3_ScaleVectorToMagnitude({player.x - hit.x, player.y - hit.y, player.z - hit.z}, 0.0099999998f);
        position = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
    }
    position.y = position.y + 0.40000001f;
    RaycastInWorldGrid(position, position, GetCharacterCollisionRadius(party.character));
    position.y = position.y + -0.40000001f;
    if (party.mode != 2)
        yaw = Vector3_GetAngleBetweenPointsXZ(position, player);
    shenApplyTranslationToAll(&party.character->entity.node, position);
    shenDecrementRotYAllNodes(&party.character->entity.node, yaw);
    party.position = position;
    party.yaw = yaw;
    charaSetFlagFor(party.character, 1);
    toggleCCMflagsByAmt(party.character, CCMF_ResetMotion);
    if (party.motionWork)
        ConfigurePartyMotion(party);
    party.character->alpha = 0.0099999998f;
    if (party.motionWork)
        mtwkSetRegionFlag(party.motionWork, 71, 0, MotionRegion::LowerBody);
    return 1;
}

// A follower left behind jumps to the newest trail sample ahead of the player that the camera cannot see.
// @verified - p
bool RelocatePartyAlongTrail(PartyCompanion &party) {
    if ((party.flags & 0x100U) || party.motionState == 5)
        return false;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    const float maximumDistance = Vector3_PlanarSquaredDistance(party.position, player);
    const float minimumDistance = maximumDistance < 100.0f ? 0.0f : 25.0f;
    bool relocated = false;
    Vector3 sample{}, local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-Vector3_GetAngleBetweenPointsXZ(party.position, player)));
    Matrix4x4::preTranslate(-player.x, -player.y, -player.z);
    for (std::int32_t relative = 0; relative < party.trailCount; ++relative) {
        const std::int32_t index = PartyTrailIndex(party, ~relative);
        if (index < 0)
            break;
        sample = party.trail[index];
        Matrix4x4::TransformPoint(sample, local);
        if (local.z < 0.0f)
            continue;
        const float distance = Vector3_PlanarSquaredDistance(player, sample);
        if (minimumDistance <= distance && distance <= maximumDistance) {
            Matrix4x4_Push();
            Matrix4x4::SetActiveTransform(camCtrlGetProjectionMatrix(nullptr));
            Matrix4x4::TransformPoint(sample, local);
            Matrix4x4::Pop(1);
            if (local.z >= 0.0f || std::fabs(-(camCtrlGetHalfVerticalFovTangent() * local.z)) + 0.44999999f <= std::fabs(local.x)) {
                relocated = true;
                break;
            }
        }
    }
    Matrix4x4::Pop(1);
    if (relocated)
        PublishPartyRelocation(party, sample);
    return relocated;
}

// Kowloon's stacked floors: gravity is withheld while no floor is near, and a companion stranded above the player
// drops back to the last trail sample that still had ground under it.
// @verified - p
bool RelocatePartyAcrossQronFloor(PartyCompanion &party) {
    if (!MatchAreaNamePattern(g_currentMap, AREA_PATTERN_KOWLOON) || !(GetQronCurrentFlags() & 1U))
        return false;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    Vector3 probe = party.position;
    probe.y += 0.5f;
    const float floor = shenFindIntersectionWithScene(probe, probe.y);
    if (floor == kNoSceneIntersection || party.position.y - floor >= 2.699999928474426)
        ccmSetFlagsFor(party.character, CCMF_ApplyGravity);
    else if (!(party.movement->flags & 8U))
        toggleCCMflagsByAmt(party.character, CCMF_ApplyGravity);
    if (party.motionState == 5)
        return false;
    if (floor != kNoSceneIntersection && (party.position.y - floor < 2.6999998f || party.position.y - player.y < 1.8f))
        return false;
    Vector3 previous = player;
    for (std::int32_t relative = 1; relative <= party.trailCount; ++relative) {
        const std::int32_t index = PartyTrailIndex(party, relative);
        if (index < 0)
            break;
        Vector3 sample = party.trail[index];
        sample.y += 0.5f;
        const float ground = shenFindIntersectionWithScene(sample, sample.y);
        sample.y = party.trail[index].y;
        if (ground == kNoSceneIntersection || party.trail[index].y - ground > 1.8f) {
            PublishPartyRelocation(party, previous);
            return true;
        }
        sample.y = ground;
        previous = sample;
    }
    return false;
}

// A follower standing in the player's way steps to the side; on stairs the step stays on the companion's tread.
// @verified - p
bool FindPartySeparationTarget(PartyCompanion &party) {
    if (!ctrlGetRootNode())
        return false;
    const PlayerMovementState &movement = *party.movement;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    const auto playerYaw = static_cast<std::uint16_t>(playerGetRotY());
    const Vector3 origin{party.position.x, party.position.y + 0.40000001f, party.position.z};
    Matrix4x4 playerFrame{};
    Vector3 local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-playerYaw));
    Matrix4x4::preTranslate(-player.x, -player.y, -player.z);
    Matrix4x4::GetTransformMatrix(&playerFrame);
    Matrix4x4::TransformPoint(origin, local);
    Matrix4x4::Pop(1);
    const float radius = GetCharacterCollisionCapsuleMargin(ctrlGetRootNode());
    float width = radius * 2.0f;
    const float lateral = std::fabs(local.x);
    if (lateral > width * 0.94999999f)
        return false;
    bool separate = local.z >= 0.0f;
    if (!separate && std::fabs(local.z) < 1.5f) {
        if (lateral > 0.25f)
            separate = true;
        else if (((movement.flags & 8U) || plymIsLookAllowed(32)) && party.playerOverlapFrames % 150 == 0)
            separate = true;
    }
    if ((width * 0.94999999f) * (width * 0.94999999f) > Vector3_PlanarSquaredDistance(player, party.position))
        separate = true;
    if (!separate)
        return false;
    Vector3 first{width, local.y, local.z};
    Vector3 second{-width, local.y, local.z};
    Vector3 firstTarget{}, secondTarget{};
    std::int32_t firstStairs = 0;
    std::int32_t secondStairs = 0;
    Vector3 hit{};
    if (movement.flags & 8U) {
        auto stairYaw = static_cast<std::uint16_t>(movement.targetWorldYaw);
        if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(stairYaw - playerYaw))) >= 0x4000)
            stairYaw = static_cast<std::uint16_t>(stairYaw + 0x8000U);
        width = 0.30000001f;
        first = {0.30000001f, 0.0f, 0.0f};
        second = {-0.30000001f, 0.0f, 0.0f};
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
        Matrix4x4::RotateAroundY_Wrapper(stairYaw);
        Matrix4x4::TransformPoint(first, first);
        Matrix4x4::TransformPoint(second, second);
        Matrix4x4::Pop(1);
        if (RaycastForCollision(&hit, &first, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({first.x - origin.x, first.y - origin.y, first.z - origin.z}, -0.0099999998f);
            first = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(firstTarget, first, radius);
        if (RaycastForCollision(&hit, &second, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({second.x - origin.x, second.y - origin.y, second.z - origin.z}, -0.0099999998f);
            second = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(secondTarget, second, radius);
        const Vector3 &stair = movement.targetWorldPosition;
        Matrix4x4_PushActiveTransform();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement.targetWorldYaw));
        Matrix4x4::preTranslate(-stair.x, -stair.y, -stair.z);
        Matrix4x4::TransformPoint(firstTarget, firstTarget);
        Matrix4x4::TransformPoint(secondTarget, secondTarget);
        Matrix4x4::Pop(1);
        firstTarget.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
        firstTarget.z = -(static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f);
        secondTarget.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
        secondTarget.z = -(static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(stair.x, stair.y, stair.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(movement.targetWorldYaw));
        Matrix4x4::TransformPoint(firstTarget, firstTarget);
        Matrix4x4::TransformPoint(secondTarget, secondTarget);
        Matrix4x4::Pop(1);
    } else {
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(player.x, player.y, player.z);
        Matrix4x4::RotateAroundY_Wrapper(playerYaw);
        Matrix4x4::TransformPoint(first, first);
        Matrix4x4::TransformPoint(second, second);
        Matrix4x4::Pop(1);
        Vector3 stairOrigin{};
        std::int32_t stairYaw = 0;
        if (RaycastForCollision(&hit, &first, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({first.x - origin.x, first.y - origin.y, first.z - origin.z}, -0.0099999998f);
            first = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(firstTarget, first, radius);
        Vector3 probe{firstTarget.x, firstTarget.y + 0.5f, firstTarget.z};
        probe.y = shenFindIntersectionWithScene(probe, firstTarget.y + 0.5f);
        firstStairs = flddQueryStairOrigin(probe, stairOrigin, stairYaw);
        if (RaycastForCollision(&hit, &second, &origin)) {
            const Vector3 back = Vector3_ScaleVectorToMagnitude({second.x - origin.x, second.y - origin.y, second.z - origin.z}, -0.0099999998f);
            second = {hit.x + back.x, hit.y + back.y, hit.z + back.z};
        }
        RaycastInWorldGrid(secondTarget, second, radius);
        probe = {secondTarget.x, secondTarget.y + 0.5f, secondTarget.z};
        probe.y = shenFindIntersectionWithScene(probe, secondTarget.y + 0.5f);
        secondStairs = flddQueryStairOrigin(probe, stairOrigin, stairYaw);
    }
    Vector3 firstLocal{}, secondLocal{};
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&playerFrame);
    Matrix4x4::TransformPoint(firstTarget, firstLocal);
    Matrix4x4::TransformPoint(secondTarget, secondLocal);
    Matrix4x4::Pop(1);
    const bool preferFirst = local.z >= 0.0f ? local.x > 0.0f : local.x >= 0.0f;
    const double strict = static_cast<double>(width) * 0.85;
    const double loose = static_cast<double>(width) * 0.5;
    const float firstX = std::fabs(firstLocal.x);
    const float secondX = std::fabs(secondLocal.x);
    const Vector3 *target = nullptr;
    if (preferFirst) {
        if (firstX > strict && !firstStairs)
            target = &firstTarget;
        else if (secondX > strict && !secondStairs)
            target = &secondTarget;
        else if (firstX > loose && !firstStairs)
            target = &firstTarget;
        else if (secondX > loose && !secondStairs)
            target = &secondTarget;
    } else {
        if (secondX > strict && !secondStairs)
            target = &secondTarget;
        else if (firstX > strict && !firstStairs)
            target = &firstTarget;
        else if (secondX > loose && !secondStairs)
            target = &secondTarget;
        else if (firstX > loose && !firstStairs)
            target = &firstTarget;
    }
    if (!target)
        return false;
    party.separationTarget = *target;
    return true;
}

// Stair state: the first probe (feet, then 0.6 ahead along the facing, then along the requested yaw) that lands on a
// stair polygon latches the flight; the lower foot picks the tread and the requested yaw picks the climb direction.
// @verified - p
void UpdatePartyStairController(PartyCompanion &party) {
    PlayerMovementState &movement = *party.movement;
    FieldRadiusMetadata metadata{};
    std::int32_t steps = 0;
    movement.stepDirection = 0;
    const Vector3 position = GetCharacterWorldPosition(party.character);
    for (std::int32_t probeIndex = 0; probeIndex < 3 && !steps; ++probeIndex) {
        Vector3 probe =
            probeIndex == 0 ? position : PartyOffsetPosition(party.position, probeIndex == 1 ? party.yaw : party.requestedYaw, 0.0f, 0.0f, -0.60000002f);
        probe.y += 1.5f;
        const float floor = shenFindIntersectionWithScene(probe, probe.y);
        if (probe.y - floor >= 3.41999990940094)
            continue;
        probe.y = floor;
        steps = flddQueryRadiusMetadata(probe, metadata) ? static_cast<std::int32_t>(metadata.stepCount) : 0;
    }
    if (steps > 0) {
        const std::uint32_t controlFlags = GetCharacterControlModelFlags(party.character);
        const std::uint32_t flags = movement.flags;
        if (!(flags & 8U) || movement.targetWorldPosition.y != metadata.anchor.y) {
            movement.targetHeightScale = static_cast<std::int8_t>(steps);
            movement.targetWorldPosition = metadata.anchor;
            movement.targetWorldYaw = static_cast<std::int16_t>(metadata.wallYaw);
            if (!(flags & 8U))
                movement.cachedCharacterControlFlags = controlFlags;
            movement.targetStairEdgeLength = metadata.maximumEdgeLength;
        }
        movement.flags = flags | 8U;
        SetCharacterControlModelFlags(
            party.character, (controlFlags & 0xfffff6bdU) | (party.motionState == 0 && party.requestedAction <= 1U ? 0x942U : 0x900U));
    } else {
        if (movement.flags & 8U)
            SetCharacterControlModelFlags(party.character, movement.cachedCharacterControlFlags);
        movement.flags &= ~8U;
    }
    if (movement.flags & 8U) {
        const float margin = GetCharacterCollisionCapsuleMargin(party.character);
        if (!HasScenarioCharacterControl() && margin >= 0.32499999f)
            SetCharacterCollisionCapsuleMargin(party.character, 0.32499999f);
    }
    Vector3 right{}, left{};
    mtwkGetControllerTargetPosition(party.motionWork, 6, right);
    mtwkGetControllerTargetPosition(party.motionWork, 5, left);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement.targetWorldYaw));
    Matrix4x4::preTranslate(-movement.targetWorldPosition.x, -movement.targetWorldPosition.y, -movement.targetWorldPosition.z);
    Matrix4x4::TransformPoint(right, right);
    Matrix4x4::TransformPoint(left, left);
    Matrix4x4::Pop(1);
    auto step = static_cast<std::int32_t>(left.y / 0.22499999f);
    if (right.y / 0.22499999f < static_cast<float>(step))
        step = static_cast<std::int32_t>(right.y / 0.22499999f);
    const std::int32_t lowest = step < 0 ? 0 : step;
    movement.groundedStepIndex = static_cast<std::int8_t>(lowest >= movement.targetHeightScale ? movement.targetHeightScale : lowest);
    const float along = -PartyLocalPosition(position, movement.targetWorldPosition, movement.targetWorldYaw).z;
    movement.stepDirection = 0;
    if (steps <= 0)
        return;
    const std::int32_t yawDifference = std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(party.requestedYaw - movement.targetWorldYaw)));
    if (yawDifference > 0x4000) {
        if (along > -0.075000003f && static_cast<float>(steps) * 0.30000001f > along && movement.groundedStepIndex > 0)
            movement.stepDirection = -1;
    } else if (along > -0.30000001f && static_cast<float>(steps - 1) * 0.30000001f > along && movement.groundedStepIndex < steps) {
        movement.stepDirection = 1;
    }
}

// @verified - p
void UpdatePartyFollowBehavior(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    float distance = Vector3_ComputeSquaredDistance(player, party.position);
    const float heightDifference = std::abs(party.position.y - player.y);
    if (heightDifference > 3.239999914169311)
        distance += 36.0f;
    party.requestedAction = 0;
    if ((party.flags & 0x40U) && party.behaviorState != 7) {
        party.behaviorState = 7;
        party.requestedAction = 1;
    }
    bool relocated = false;
    if ((placement.flags & 8U) && RelocatePartyAlongTrail(party)) {
        party.behaviorState = party.motionState = 0;
        mtwkCopyTransition(party.motionWork, nullptr, party.yaw, 13, 0);
        relocated = true;
    }
    if (RelocatePartyAcrossQronFloor(party)) {
        party.behaviorState = party.motionState = 0;
        mtwkCopyTransition(party.motionWork, nullptr, party.yaw, 13, 0);
        relocated = true;
    }
    switch (party.behaviorState) {
    case 0:
        party.runBlend = 0.0f;
        if (relocated)
            break;
        if (party.motionState == 1) {
            party.behaviorState = 1;
            break;
        }
        ++party.idleFrames;
        party.requestedAction = 1;
        {
            const float limit = (party.flags & 0x20000U) ? 0.30250001f : (party.followDistance + 0.55000001f) * (party.followDistance + 0.55000001f);
            if (distance > limit) {
                party.requestedAction = 3;
                break;
            }
            if (distance > 4.0f) {
                const std::int16_t yaw = Vector3_GetAngleBetweenPointsXZ(party.position, player);
                if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(yaw - party.yaw))) > 0x4000) {
                    party.requestedAction = 2;
                    party.requestedYaw = yaw;
                    break;
                }
            }
            bool sameFloor = !MatchAreaNamePattern(g_currentMap, AREA_PATTERN_KOWLOON) || !(GetQronCurrentFlags() & 1U);
            if (!sameFloor) {
                Vector3 search{};
                plymUpdatePlayerSearchLocation(&search);
                const std::int32_t floor = GetQronFloorAtPosition(party.position);
                sameFloor = GetQronFloorAtPosition(search) == floor;
            }
            if (sameFloor && party.frameCount > 10U && FindPartySeparationTarget(party))
                party.requestedAction = 13;
        }
        break;
    case 1: {
        const std::uint32_t door = flddQueryDoorIntersection(party.position);
        Vector3 hit{};
        if ((door & 0xff00U) != 0x3500U && door && (placement.flags & 0x40000U) && RaycastForCollision(&hit, &placement.position, &party.position)) {
            party.requestedAction = 11;
            party.behaviorState = 8;
            party.doorPhase = 0;
            party.doorId = door;
            break;
        }
        if (party.flags & 0x800U) {
            if (party.runBlend >= 0.5f) {
                if (distance < 100.0f)
                    party.runBlend = 0.0f;
            } else if (distance > 225.0f)
                party.runBlend = 1.0f;
        }
        const float limit = (party.flags & 0x20000U) ? 0.0f : party.followDistance * party.followDistance;
        if (limit > distance)
            party.requestedAction = 1;
        if (party.placement.distance < 0.55000001f && heightDifference < 1.8f)
            party.requestedAction = 1;
        if (plymIsInterpolationActive() && !(party.flags & 0x100U))
            party.speedScale = distance > 225.0f ? 1.5925951f : distance > 100.0f ? 1.4651874f : distance > 25.0f ? 1.3377799f : 1.0f;
        break;
    }
    case 6:
    case 7:
        party.requestedAction = 1;
        if (!(party.flags & 0x40U))
            party.behaviorState = 0;
        break;
    case 8:
        if (party.doorPhase == 4)
            party.behaviorState = 0;
        else
            party.requestedAction = 11;
        break;
    default:
        break;
    }
    if (party.behaviorState)
        party.idleFrames = 0;
}

// @verified - p
void UpdatePartyGuideBehavior(PartyCompanion &party) {
    const bool conversation = IsConversationCameraCharacter(party.character);
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    if (party.flags & 0x40000U) {
        if (party.placement.distance > 0.55000001f)
            party.flags &= ~0x40000U;
    } else if (party.placement.distance <= 0.55000001f &&
        ((party.placement.flags & 0x20U) || (!party.behaviorState && Vector3_PlanarSquaredDistance(party.position, party.routeGoal) < 0.30250001f))) {
        party.flags |= 0x40000U;
    }
    party.requestedAction = 0;
    if (party.behaviorState == 1) {
        party.requestedAction = party.placement.distance < 0.55000001f ? 14 : 0;
        if ((party.selectedPlacement->flags & 8U) || (party.flags & 0x80U))
            party.requestedAction = 1;
    } else if (party.behaviorState) {
        party.runBlend = 0.0f;
        if (party.behaviorState == 6 || party.behaviorState == 7) {
            if (!conversation)
                party.behaviorState = 0;
            else if (party.flags & 0x40000U) {
                Vector3 target{};
                if (ctrlGetRootNode())
                    charaGetMotWorldPos(ctrlGetRootNode(), 5, &target);
                RequestNpcSpecialMotionTarget(party.name, &target);
            }
        }
    } else {
        party.runBlend = 0.0f;
        if (conversation && (party.flags & 0x40000U)) {
            party.behaviorState = 6;
            return;
        }
        if (party.placement.distance < 0.55000001f && (party.placement.flags & 0x20U)) {
            const Vector3 local = PartyLocalPosition(player, party.position, party.placement.targetYaw);
            const auto yaw = static_cast<std::int16_t>(party.placement.targetYaw + (local.x >= 0.0f ? -24576 : 24576));
            if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(yaw - party.yaw))) > 0x2000) {
                party.requestedYaw = yaw;
                party.requestedAction = 2;
                return;
            }
        }
        if (!(party.selectedPlacement->flags & 8U) && !(party.flags & 0x80U) && party.placement.distance >= 0.55000001f) {
            party.requestedAction = 3;
        }
    }
}

// The requested action drives the motion state: 0 idle, 1 following, 3 settling, 4 turning in place, 5 passing a door
// (claim, ingress, door animation, release), 6 climbing a stair tread. A settled stair position is re-snapped to the tread.
// @verified - p
void UpdatePartyMotionController(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    PlayerMovementState &movement = *party.movement;
    MotionWork *const work = party.motionWork;
    const Vector3 position = GetCharacterWorldPosition(party.character);
    UpdatePartyStairController(party);
    const std::uint8_t previousState = party.motionState;
    switch (party.motionState) {
    case 0:
        switch (party.requestedAction) {
        case 1:
        case 14:
            if (party.idleFrames > 150) {
                const std::uint16_t idle = PartyIdleMotion(party);
                if (mtwkGetAnimationId(movement.motionWork, MotionRegion::LowerBody) != idle) {
                    mtwkSetRequestedRootMode(movement.motionWork, 1);
                    mtwkSetFrameTime(movement.motionWork, 20, MotionRegion::LowerBody);
                    mtwkRequestMotion(movement.motionWork, idle, MotionRegion::LowerBody);
                }
            }
            break;
        case 2:
            mtwkCopyTransition(work, nullptr, party.requestedYaw, 13, 0);
            party.motionState = 4;
            break;
        case 3:
            if (movement.stepDirection) {
                party.motionState = 6;
                party.motionPhase = 0;
            } else {
                mtwkCopyTransition(work, &placement.position, placement.targetYaw, 0, 1);
                mtwkCopyTransition(work, nullptr, 0, 16, 0);
                party.behaviorState = party.motionState = 1;
            }
            break;
        case 13: {
            const std::int16_t bearing = Vector3_GetAngleBetweenPointsXZ(party.separationTarget, *plymUpdatePlayerSearchLocation(nullptr));
            const std::int16_t yaw = (movement.flags & 8U) ? party.yaw : bearing;
            mtwkCopyTransition(work, &party.separationTarget, yaw, 0, 0);
            mtwkCopyTransition(work, nullptr, 0, 2, 0);
            party.motionState = 3;
            break;
        }
        default:
            break;
        }
        if (!party.motionState && (movement.flags & 8U) && movement.stepDirection)
            movement.flags |= 0x20000U;
        break;
    case 1:
        if (movement.stepDirection) {
            party.motionState = 6;
            party.motionPhase = 0;
            break;
        }
        mtwkCopyTransition(work, &placement.position, placement.targetYaw, 0, 0);
        mtwkCopyTransition(work, nullptr, 0, 16, 0);
        if (party.runBlend >= 0.5f) {
            if (!mtwkCopyTransition(work, nullptr, 0, 15, 0))
                mtwkCopyTransition(work, nullptr, 0, 8, 0);
            movement.field15c = party.speedScale;
        } else if (mtwkCopyTransition(work, nullptr, 0, 15, 0)) {
            mtwkCopyTransition(work, nullptr, 0, 14, 0);
        }
        if (party.requestedAction == 1 || party.requestedAction == 13) {
            mtwkCopyTransition(work, nullptr, party.yaw, 13, 0);
            party.motionState = 3;
        } else if (party.requestedAction == 11) {
            mtwkCopyTransition(work, nullptr, 0, 2, 0);
            party.motionState = 5;
        } else if (party.requestedAction == 14) {
            mtwkCopyTransition(work, &placement.position, placement.targetYaw, 6, 0);
            party.motionState = 3;
        }
        if (!mtwkSetExtraFlag(work, 58, 1) && party.motionState == 1)
            party.motionState = 3;
        break;
    case 3:
        if (party.requestedAction == 3) {
            mtwkCopyTransition(work, &placement.position, placement.targetYaw, 0, 0);
            party.motionState = 1;
        } else if (party.requestedAction == 11) {
            mtwkCopyTransition(work, nullptr, party.yaw, 13, 0);
            party.motionState = 5;
        }
        if (party.motionState == 3) {
            if (mtwkCopyTransition(work, nullptr, 0, 15, 0))
                mtwkCopyTransition(work, nullptr, 0, 14, 0);
            if (mtwkCopyTransition(work, nullptr, 0, 3, 0))
                mtwkCopyTransition(work, nullptr, 0, 2, 0);
            if (party.stateFrames > 120U) {
                mtwkCopyTransition(work, nullptr, party.yaw, 13, 0);
                party.behaviorState = party.motionState = 0;
            } else if (!mtwkSetExtraFlag(work, 58, 1)) {
                party.behaviorState = party.motionState = 0;
            }
        }
        break;
    case 4:
        if (!mtwkSetExtraFlag(work, 58, 1))
            party.motionState = 0;
        break;
    case 5:
        if (party.doorPhase == 0) {
            if (!ClaimDoor(shen_name{party.doorId}, ctrlGetScenarioCharaName(party.character)))
                break;
            ++party.doorPhase;
        }
        switch (party.doorPhase) {
        case 1: {
            const Vector3 current = GetCharacterWorldPosition(party.character);
            Vector3 target{};
            ComputeDoorIngress(static_cast<std::int32_t>(party.doorId), &current, &target);
            if (mtwkCopyTransition(work, &target, static_cast<std::uint16_t>(g_doorIngressRotation.y), 0, 0)) {
                const auto animation = static_cast<std::uint16_t>(GetNpcDoorAnimation(static_cast<std::int32_t>(party.doorId), party.character, &current));
                mtwkSetTimeRelated(work, 1, 5, animation);
                ++party.doorPhase;
                toggleCCMflagsByAmt(party.character, CCMF_StairMode);
            }
            break;
        }
        case 2:
            if (!mtwkSetExtraFlag(work, 58, 1)) {
                const Vector3 current = GetCharacterWorldPosition(party.character);
                const auto animation = GetNpcDoorAnimation(static_cast<std::int32_t>(party.doorId), party.character, &current);
                BeginDoorAnimation(static_cast<std::int32_t>(party.doorId), ctrlGetScenarioCharaName(party.character), static_cast<std::int32_t>(animation));
                ccmSetFlagsFor(party.character, CCMF_FineRaycast);
                ++party.doorPhase;
            }
            break;
        case 3:
            if (mtwkSetRegionFlag(work, 65, 1, MotionRegion::LowerBody)) {
                toggleCCMflagsByAmt(party.character, CCMF_FineRaycast);
                ccmSetFlagsFor(party.character, CCMF_StairMode);
                ++party.doorPhase;
            }
            break;
        case 4:
            party.motionState = 0;
            party.doorPhase = 0;
            break;
        default:
            break;
        }
        break;
    case 6:
        switch (party.motionPhase) {
        case 0:
            if (!movement.stepDirection) {
                party.motionState = 1;
                break;
            }
            party.stairDirection = movement.stepDirection;
            {
                Vector3 step = PartyLocalPosition(position, movement.targetWorldPosition, movement.targetWorldYaw);
                step.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
                step.z = -(static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f);
                if (step.x > 0.0f)
                    step.x = -GetCharacterCollisionRadius(party.character);
                if (-movement.targetStairEdgeLength > step.x)
                    step.x = GetCharacterCollisionRadius(party.character) - movement.targetStairEdgeLength;
                const Vector3 target = PartyWorldPosition(step, movement.targetWorldPosition, movement.targetWorldYaw);
                auto yaw = static_cast<std::uint16_t>(movement.targetWorldYaw);
                if (party.stairDirection == -1)
                    yaw = static_cast<std::uint16_t>(yaw + 0x8000U);
                mtwkCopyTransition(work, &target, yaw, 0, 0);
                mtwkCopyTransition(work, nullptr, 0, 2, 0);
                ++party.motionPhase;
            }
            break;
        case 1:
            if (party.requestedAction == 1)
                party.motionState = 3;
            else if (mtwkCopyTransition(work, nullptr, 0, 3, 0))
                party.motionPhase = 0;
            else if (!mtwkSetExtraFlag(work, 58, 1)) {
                ++party.motionPhase;
                mtwkSetRequestedRootMode(work, 3);
                mtwkSetFrameTime(work, 1, MotionRegion::LowerBody);
                if (party.stairDirection == 1)
                    RequestPlayerMotion(movement, 61483);
                else if (party.stairDirection == -1)
                    RequestPlayerMotion(movement, 61449);
            }
            break;
        case 2:
            if (!movement.stepDirection || party.requestedAction == 1) {
                if ((movement.playerMotionFlags & 2U) && (movement.playerMotionFlags & 1U)) {
                    mtwkSetRequestedRootMode(movement.motionWork, 1);
                    mtwkSetFrameTime(movement.motionWork, 20, MotionRegion::LowerBody);
                    mtwkRequestMotion(movement.motionWork, PartyIdleMotion(party), MotionRegion::LowerBody);
                    movement.flags |= 0x20000U;
                    party.motionState = party.requestedAction == 1 ? 3 : 1;
                }
            } else if (mtwkSetRegionFlag(work, 65, 1, MotionRegion::LowerBody))
                movement.flags |= 0x20000U;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    if (movement.flags & 0x20000U) {
        movement.flags &= ~0x20000U;
        Vector3 step = PartyLocalPosition(party.position, movement.targetWorldPosition, movement.targetWorldYaw);
        const float depth = static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f;
        step.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
        step.z = -depth;
        shenApplyTranslationToAll(&party.character->entity.node, PartyWorldPosition(step, movement.targetWorldPosition, movement.targetWorldYaw));
    }
    ++party.stateFrames;
    if (party.motionState != previousState)
        party.stateFrames = 0;
    mtwkSetSpeed(work, movement.field160 * movement.field15c, MotionRegion::LowerBody);
    movement.previousField15c = movement.field15c;
}

// Sets 0x100 while the companion projects inside the view; returns 1 when it is off camera.
// @verified - p
std::int32_t UpdatePartyCameraVisibility(PartyCompanion &party) {
    party.flags &= ~0x100U;
    Vector3 projected{};
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(camCtrlGetProjectionMatrix(nullptr));
    Matrix4x4::TransformPoint(party.position, projected);
    Matrix4x4::Pop(1);
    if (projected.z >= 0.0f || std::fabs(-(camCtrlGetHalfVerticalFovTangent() * projected.z)) + 0.44999999f <= std::fabs(projected.x))
        return 1;
    party.flags |= 0x100U;
    return 0;
}
#endif

// @verified - d
void UpdatePartyCharacterState(PartyCompanion &party) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    MotionWork *const work = charFindMotionwork(party.character);
    if (work && !party.motionWork) {
        party.motionWork = work;
        party.movement->motion = work;
        ConfigurePartyMotion(party);
    }
    UpdatePlayerMovementState(*party.movement);
    GetCharacterWorldPosition(party.character, &party.position);
    party.yaw = static_cast<std::int16_t>(GetCharacterWorldYaw(party.character));
    if (IsNpcRouteLoadedOrVisible(party.name))
        SetNpcRouteYaw(party.name, party.yaw);
    party.flags &= ~0x20000U;
    if (party.flags & 0x10000U)
        party.flags |= 0x20000U;
    party.flags &= ~0x10000U;
    party.flags &= ~0x80U;
    if (plymIsPlacementBlockedForParty())
        party.flags |= 0x80U;
    if (IsNaviActive())
        party.flags |= 0x80U;
    const shen_name guideName = SHEN_NAME(M01_);
    if (IsNpcRouteReady(guideName)) {
        EV_CheckWarmStartNpcRoute(guideName);
        SetNpcRouteMotionState(0, guideName, 2);
    }
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    MotionWork *const work = FindMotionWork(party.character);
    if (work && !party.motionWork) {
        party.motionWork = work;
        party.movement->motionWork = work;
        ConfigurePartyMotion(party);
    }
    if (party.motionWork && mtwkSetRegionFlag(party.motionWork, 71, 1, MotionRegion::LowerBody))
        mtwkSetRegionFlag(party.motionWork, 71, 2, MotionRegion::LowerBody);
    UpdatePlayerMovementState(*party.movement);
    party.position = GetCharacterWorldPosition(party.character);
    party.yaw = GetAllAngYForNode(&party.character->entity.node);
    party.speedScale = 1.0f;
    if (IsNpcLoadedOrVisible(party.name))
        SetNpcRouteYaw(party.name, party.yaw);
    party.flags &= 0xfff5ffffU;
    if (party.flags & 0x10000U)
        party.flags = (party.flags & 0xfffcffffU) | 0x20000U;
    if (!IsNpcLoadedOrVisible(party.name))
        EV_CheckWarmStartNpcRoute(party.name);
    party.flags &= ~0x80U;
    if (IsPlayerPartyGuideBlocked())
        party.flags |= 0x80U;
    if (IsNaviActive())
        party.flags |= 0x80U;
    constexpr shen_name guideName{SHEN_NAME(M01_)};
    if (IsNpcRouteReady(guideName)) {
        EV_CheckWarmStartNpcRoute(guideName);
        PauseNpcRouteMotion(nullptr, guideName);
    }
    if (MatchAreaNamePattern(g_currentMap, AREA_PATTERN_KOWLOON) && (GetQronCurrentFlags() & 1U)) {
        Vector3 player{};
        plymUpdatePlayerSearchLocation(&player);
        const std::int32_t floor = GetQronFloorAtPosition(party.position);
        if (GetQronFloorAtPosition(player) != floor)
            party.flags |= 0x80000U;
    }
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t UpdatePartyCameraVisibility(PartyCompanion &party) {
    std::int32_t visible = 0;
    Vector3 projected;
    float width;
    party.flags &= ~0x100U;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(camCtrlGetProjectionMatrix(0));
    Matrix4x4::TransformPoint(party.position, projected);
    Matrix4x4::Pop(1);
    if (projected.z < 0.0f) {
        width = -projected.z * camCtrlGetHalfVerticalFovTangent();
        width = (width < 0.0f ? -width : width) + 0.44999999f;
        if (width > (projected.x < 0.0f ? -projected.x : projected.x))
            visible = 1;
    }
    if (visible)
        party.flags |= 0x100U;
    if (visible)
        return 0;
    return 1;
}


// @verified - d
std::int32_t UpdatePartyPlayerProximity(PartyCompanion &party) {
    UpdatePartyCameraVisibility(party);
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Requested resets, camera visibility, and the count of frames spent close behind the player.
// @verified - p
void UpdatePartyPlayerProximity(PartyCompanion &party) {
    if (party.flags & 0x600U) {
        (void)ResetPartyPlacementNearPlayer(party);
        party.flags &= 0xfffff9ffU;
    }
    (void)UpdatePartyCameraVisibility(party);
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    const auto yaw = static_cast<std::int16_t>(playerGetRotY());
    const Vector3 local = PartyLocalPosition({party.position.x, party.position.y + 0.40000001f, party.position.z}, player, yaw);
    if (local.z >= 0.0f || std::fabs(local.z) >= 1.5f || std::fabs(local.x) > 0.25f)
        party.playerOverlapFrames = 0;
    else
        ++party.playerOverlapFrames;
}
#endif

} // namespace model_component_detail

#if SHENMUE_IS_DC && SHENMUE_IS_S2
 // namespace model_component_detail
std::int32_t FindFieldSegmentOffsetInWorld(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
#endif

namespace model_component_detail {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float Vector3_PlanarSquaredDistance(const Vector3 &left, const Vector3 &right);

// @verified - d
std::int32_t ResetPartyPlacementNearPlayer(PartyCompanion &party) {
    Vector3 position, player, offset, hit, back;
    std::int32_t angle, yaw, heading;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = -1.0f;
    GetCharacterWorldPosition(party.character, &position);
    plymUpdatePlayerSearchLocation(&player);
    yaw = playerGetRotY();
    player.y += 0.40000001f;
    position.y += 0.40000001f;
    const std::int32_t blocked = FindFieldSegmentOffsetInWorld(&hit, &player, &position);
    player.y -= 0.40000001f;
    position.y -= 0.40000001f;
    if (!blocked && Vector3_PlanarSquaredDistance(position, player) < 900.0f) {
        if (((position.y - player.y) < 0.0f ? -(position.y - player.y) : (position.y - player.y)) < 5.0f)
            return 0;
    }
    switch (party.mode) {
    case 1:
    default:
        angle = -0x6000;
        break;
    case 2:
        angle = -0x2000;
        break;
    }
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&player);
    Matrix4x4::RotateAroundY_Wrapper(yaw + angle);
    Matrix4x4::TransformPoint(offset, position);
    PopCurrentMatrix(1);
    if (FindFieldSegmentOffsetInWorld(&hit, &position, &player)) {
        am2Vector3Subtract(&back, &player, &hit);
        dc::sdk::SetVectorLength(&back, &back, 0.0099999998f);
        am2Vector3Add(&position, &hit, &back);
    }
    position.y += 0.40000001f;
    RaycastInWorldGrid(&position, &position, GetCharacterCollisionRadius(party.character));
    position.y -= 0.40000001f;
    switch (party.mode) {
    case 1:
    default:
        heading = CopyHeading(position, player);
        break;
    case 2:
        heading = yaw;
        break;
    }
    SetCharacterWorldPosition(party.character, &position);
    SetCharacterWorldYaw(party.character, heading);
    party.character->alpha = 0.0099999998f;
    return 1;
}


// @verified - d
static float Vector3_PlanarSquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float x = left.x - right.x, z = left.z - right.z;
    if (std::fabs(x) < FLT_EPSILON && std::fabs(z) < FLT_EPSILON)
        return 0.0f;
    return x * x + z * z;
}


// @verified - d
static inline Vector3 PartyLocalPosition(const Vector3 &point, const Vector3 &origin, std::int16_t yaw) {
    Vector3 result;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-yaw);
    matrix_stack::TranslateInverseVector(&origin);
    Matrix4x4::TransformPoint(point, result);
    PopCurrentMatrix(1);
    return result;
}


// @verified - d
static Vector3 PartyWorldPosition(const Vector3 &point, const Vector3 &origin, std::int16_t yaw) {
    Vector3 result;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::TransformPoint(point, result);
    Matrix4x4::Pop(1);
    return result;
}


// @verified - d
static inline void PartyOffsetPosition(Vector3 *output, const Vector3 &origin, std::int16_t yaw, float x, float y, float z) {
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&origin);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::preTranslate(x, y, z);
    matrix_stack::GetTranslation(output);
    PopCurrentMatrix(1);
}


// @verified - d
static const Vector3 *PartyTrailSample(const PartyCompanion &party, std::int32_t relative) {
    std::int32_t direction = 1;
    if (!party.trailCount || !relative)
        return 0;
    if (relative < 0) {
        direction = -1;
        relative = -relative;
    }
    if (relative > 32)
        return 0;
    if (relative > party.trailCount)
        relative = party.trailCount;
    std::int32_t index = party.trailWriteIndex;
    if (party.trailCount < 32 && direction < 0)
        index = 0;
    if (direction < 0)
        --index;
    index -= relative * direction;
    if (index < 0)
        index += 31;
    if (index > 31)
        index -= 32;
    return &party.trail[index];
}
#endif

} // namespace model_component_detail

#if SHENMUE_IS_DC && SHENMUE_IS_S2
 // namespace model_component_detail
void GetPlayerTargetWorldPosition(Vector3 *position);
std::int16_t Vector3_GetFixedYaw(const Vector3 *direction);
#endif

namespace model_component_detail {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void PreparePartyFollowPlacement(PartyCompanion &party) {
    PartyPlacementNode *const placement = &party.placement;
    PartyPlacementNode *const trail = &party.trailPlacement;
    if (!ctrlGetRootNode())
        return;
    Vector3 player, position, offset, target, chosen, sample, local, previous;
    GetPlayerTargetWorldPosition(&player);
    plymGetAngY();
    RecordPartyPlayerTrail(party, player);
    GetCharacterWorldPosition(party.character, &position);
    GetCharacterWorldYaw(party.character);
    position = party.position;
    if (party.movement->flags & 8U) {
        const PlayerMovementState &movement = *party.movement;
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(-movement.targetWorldYaw);
        Matrix4x4::preTranslate(-movement.targetWorldPosition.x, -movement.targetWorldPosition.y, -movement.targetWorldPosition.z);
        Matrix4x4::TransformPoint(position, local);
        PopCurrentMatrix(1);
        float height = -local.z * 0.74999994f;
        height = (height < 0.0f ? 0.0f : height) < movement.targetHeightScale * 0.22499999f ? (height < 0.0f ? 0.0f : height) :
                                                                                              movement.targetHeightScale * 0.22499999f;
        position.y = movement.targetWorldPosition.y + height;
    }
    player.y += 0.40000001f;
    position.y += 0.40000001f;
    am2Vector3Subtract(&offset, &position, &player);
    const float distance = am2Vector3NormalizeInPlace(&offset);
    if (distance > party.followDistance)
        am2Vector3Scale(&offset, &offset, party.followDistance);
    else
        am2Vector3Scale(&offset, &offset, distance);
    am2Vector3Add(&target, &player, &offset);
    target.y += 1.5f;
    target.y = FindFieldIntersectionHeight(&target);
    if (target.y == FLT_MAX)
        target.y = player.y;
    else
        target.y += 0.40000001f;
    placement->position = target;
    placement->targetYaw = Vector3_GetFixedYaw(&offset) + 0x8000;
    if (party.flags & 0x20000U) {
        placement->position = party.requestedPosition;
        placement->targetYaw = party.requestedPositionYaw;
    }
    if (!TestFieldWorldSegment(&player, &position))
        return;
    previous = target;
    const Vector3 *point;
    for (std::int32_t index = 0; index < party.trailCount && (point = PartyTrailSample(party, index + 1)); ++index) {
        sample.x = point->x;
        sample.y = point->y + 0.40000001f;
        sample.z = point->z;
        if (!TestFieldWorldSegment(&sample, &position)) {
            chosen = sample;
            if (Vector3_ComputeSquaredDistance(sample, position) < 4.0f) {
                flddPushIntersectionFlag(0x40);
                if (TestFieldWorldSegment(&sample, &previous)) {
                    chosen = previous;
                    trail->flags |= 0x40000U;
                }
            }
            placement->next = trail;
            trail->position = chosen;
            break;
        }
        previous = sample;
    }
    if (!placement->next)
        placement->flags |= 8U;
}


// @verified - d
static void PreparePartyGuidePlacement(PartyCompanion &party) {
    Vector3 sample;
    Vector3 goal;
    PartyPlacementNode *placement = &party.placement;
    PartyPlacementNode *trail = &party.trailPlacement;
    goal.x = -111.4f;
    goal.y = 23.511999f;
    goal.z = -132.3f;
    if (!InitializePartyGuide()) {
        placement->flags |= 8U;
        return;
    }
    SetPartyGuideRouteHint(party.routeIndex);
    party.routeIndex = PickClosestTargetForPlacement(&party.position);
    if (party.flags & 0x40000000U) {
        if (party.routeMarker)
            mazeGetMarkerPosition(party.routeMarker, &goal);
        FindClosestTargetForPlacement(&goal);
    } else {
        FindClosestTargetForPlacement(0);
    }
    SetPartyGuideDirection();
    if (PartyGuideEndState() > 0) {
        placement->flags |= 0x20U;
        return;
    }
    std::int16_t yaw;
    SamplePartyGuide(&sample, &yaw);
    party.routeYaw = yaw;
    float low, high;
    party.routeWidth = PartyGuideLaneBounds(low, high);
    Vector3 local;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-party.yaw);
    matrix_stack::TranslateInverseVector(&party.position);
    Matrix4x4::TransformPoint(sample, local);
    PopCurrentMatrix(1);
    if ((local.z > -1.0f || Vector3_PlanarSquaredDistance(party.position, sample) < 2.25f) && PartyGuideEndState() < 1)
        AdvancePartyGuide(1.0f);
    if (PartyGuideEndState() < 1)
        AdvancePartyGuide(2.0f);
    SamplePartyGuide(&sample, &yaw);
    if (Vector3_ComputeSquaredDistance(party.position, sample) < 1.0f && PartyGuideEndState() < 1) {
        AdvancePartyGuide(2.0f);
        SamplePartyGuide(&sample, &yaw);
    }
    placement->position = sample;
    placement->targetYaw = yaw;
    Vector3 player;
    plymUpdatePlayerSearchLocation(&player);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-party.routeYaw);
    matrix_stack::TranslateInverseVector(&party.position);
    Matrix4x4::TransformPoint(player, player);
    PopCurrentMatrix(1);
    PartyGuideLaneBounds(low, high);
    float lateral = player.x < 0.0f ? high - 0.75f : low + 0.75f;
    lateral = (lateral < -1.0f ? -1.0f : lateral) < 1.0f ? (lateral < -1.0f ? -1.0f : lateral) : 1.0f;
    party.routeLateralOffset = InterpolateTo(lateral, party.routeLateralOffset, 0.079999998f);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&placement->position);
    Matrix4x4::RotateAroundY_Wrapper(placement->targetYaw);
    Matrix4x4::preTranslate(party.routeLateralOffset, 0.0f, 0.0f);
    matrix_stack::GetTranslation(&placement->position);
    PopCurrentMatrix(1);
    flddToggleFlag(0x428);
    if (!GlobalRayIntersectionTest(party.position, placement->position, 0.34999999f))
        return;
    PartyGuideCallbackInitialize();
    PartyGuideCallbackPick(&placement->position);
    PartyGuideCallbackRouteHint(party.routeIndex);
    PartyGuideCallbackFind(&party.position);
    PartyGuideCallbackDirection();
    while (PartyGuideCallbackEndState() < 1) {
        Vector3 retry;
        std::int16_t retryYaw;
        PartyGuideCallbackSample(&retry, &retryYaw);
        retry.y += 0.40000001f;
        flddToggleFlag(0x428);
        if (!GlobalRayIntersectionTest(party.position, retry, 0.34999999f)) {
            placement->next = trail;
            trail->position = retry;
            return;
        }
        PartyGuideCallbackAdvance(5.0f);
    }
}


// @verified - d
static void PreparePartyPlacement(PartyCompanion &party) {
    PartyPlacementNode *const placement = &party.placement;
    PartyPlacementNode *const trail = &party.trailPlacement;
    PartyPlacementNode *const avoidance = &party.avoidancePlacement;
    placement->character = party.character;
    placement->next = 0;
    trail->next = 0;
    avoidance->next = 0;
    placement->flags &= 0x7ffbffd7U;
    trail->flags = (trail->flags & 0x7ffbffd7U) | 0x800U;
    avoidance->flags = (avoidance->flags & 0x7ffbffd7U) | 0x20000U;
    switch (party.mode) {
    case 1:
        PreparePartyFollowPlacement(party);
        break;
    case 2:
        PreparePartyGuidePlacement(party);
        break;
    }
    party.selectedPlacement = player_detail::plymUpdatePlacementChain(placement);
}


// @verified - d
static void FindPartyObstacles(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    Vector3 start, plusStart, minusStart, end, plusEnd, minusEnd, sideways, point;
    Character *first, *second, *third;
    float firstDistance, secondDistance, thirdDistance;
    sideways.x = 0.40000001f;
    sideways.y = 0.0f;
    sideways.z = 0.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(placement.characterBearingYaw);
    Matrix4x4::TransformPoint(sideways, sideways);
    PopCurrentMatrix(1);
    party.obstacleDistance = 10.0f;
    party.obstacleCharacter = 0;
    third = 0;
    second = 0;
    first = 0;
    firstDistance = secondDistance = thirdDistance = 10.0f;
    start = party.position;
    end = placement.position;
    start.y += 0.40000001f;
    end.y += 0.40000001f;
    am2Vector3Add(&plusStart, &start, &sideways);
    am2Vector3Add(&plusEnd, &end, &sideways);
    am2Vector3Subtract(&minusStart, &start, &sideways);
    am2Vector3Subtract(&minusEnd, &end, &sideways);
    ccmUpdateEntityFlag(party.character);
    if (CheckPointInCylinder(&start, &end)) {
        first = GetPointFound();
        GetCharacterWorldPosition(first, &point);
        firstDistance = Vector3_PlanarSquaredDistance(start, point);
    }
    ccmUpdateEntityFlag(party.character);
    if (CheckPointInCylinder(&minusStart, &minusEnd)) {
        second = GetPointFound();
        GetCharacterWorldPosition(second, &point);
        secondDistance = Vector3_PlanarSquaredDistance(minusStart, point);
    }
    ccmUpdateEntityFlag(party.character);
    if (CheckPointInCylinder(&plusStart, &plusEnd)) {
        third = GetPointFound();
        GetCharacterWorldPosition(third, &point);
        thirdDistance = Vector3_PlanarSquaredDistance(plusStart, point);
    }
    if (firstDistance < secondDistance && firstDistance < thirdDistance) {
        party.obstacleDistance = firstDistance;
        party.obstacleCharacter = first;
    } else if (secondDistance < thirdDistance) {
        party.obstacleDistance = secondDistance;
        party.obstacleCharacter = second;
    } else {
        party.obstacleDistance = thirdDistance;
        party.obstacleCharacter = third;
    }
    if (party.obstacleCharacter)
        party.obstacleDistance = std::sqrt(party.obstacleDistance);
}


// @verified - d

static void SelectPartyAvoidancePlacement(PartyCompanion &party) {
    PartyPlacementNode &placement = *party.selectedPlacement;
    std::uint32_t direction = 0;
    PartyPlacementNode &avoidance = party.avoidancePlacement;
    float side = 1.0f;
    Vector3 obstacle, local, hit;
    if (!party.obstacleCharacter)
        party.flags &= 0xffff3fffU;
    if (!party.obstacleCharacter)
        return;
    GetCharacterWorldPosition(party.obstacleCharacter, &obstacle);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-placement.characterBearingYaw);
    matrix_stack::TranslateInverseVector(&party.position);
    Matrix4x4::TransformPoint(obstacle, local);
    PopCurrentMatrix(1);
    if (!(party.flags & 0xc000U))
        direction = local.x < 0.0f ? 0x4000U : 0x8000U;
    if (party.obstacleDistance > 3.0f)
        return;
    if (direction & 0x4000U)
        side = 1.0f;
    if (direction & 0x8000U)
        side = -1.0f;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&party.position);
    Matrix4x4::RotateAroundY_Wrapper(placement.characterBearingYaw);
    Matrix4x4::preTranslate(side, 0.0f, -party.obstacleDistance);
    matrix_stack::GetTranslation(&avoidance.position);
    PopCurrentMatrix(1);
    if (FindFieldSegmentOffsetInWorld(&hit, &avoidance.position, &party.position)) {
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&party.position);
        Matrix4x4::RotateAroundY_Wrapper(placement.characterBearingYaw);
        Matrix4x4::preTranslate(-side, 0.0f, -party.obstacleDistance);
        matrix_stack::GetTranslation(&avoidance.position);
        PopCurrentMatrix(1);
    }
    avoidance.targetYaw = placement.targetYaw;
    placement.next = &avoidance;
    party.selectedPlacement = &avoidance;
}


// @verified - d
static void UpdatePartyStairController(PartyCompanion &party) {
    PlayerMovementState &movement = *party.movement;
    std::int32_t steps;
    Vector3 anchor, position, local, probe, right, left;
    std::int32_t yaw;
    float height;
    steps = movement.stepDirection = 0;
    GetCharacterWorldPosition(party.character, &position);
    probe = position;
    probe.y += 1.5f;
    height = FindFieldIntersectionHeight(&probe);
    if (probe.y - height < 3.4199998f) {
        probe.y = height;
        steps = flddQueryStairOrigin(&probe, &anchor, &yaw);
    }
    if (!steps) {
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&party.position);
        Matrix4x4::RotateAroundY_Wrapper(party.yaw);
        Matrix4x4::preTranslate(0.0f, 0.0f, -0.60000002f);
        matrix_stack::GetTranslation(&probe);
        PopCurrentMatrix(1);
        probe.y += 1.5f;
        height = FindFieldIntersectionHeight(&probe);
        if (probe.y - height < 3.4199998f) {
            probe.y = height;
            steps = flddQueryStairOrigin(&probe, &anchor, &yaw);
        }
        if (!steps) {
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&party.position);
            Matrix4x4::RotateAroundY_Wrapper(party.requestedYaw);
            Matrix4x4::preTranslate(0.0f, 0.0f, -0.60000002f);
            matrix_stack::GetTranslation(&probe);
            PopCurrentMatrix(1);
            probe.y += 1.5f;
            height = FindFieldIntersectionHeight(&probe);
            if (probe.y - height < 3.4199998f) {
                probe.y = height;
                steps = flddQueryStairOrigin(&probe, &anchor, &yaw);
            }
        }
    }
    if (steps > 0) {
        std::uint32_t flags = ccmGetFlags(party.character);
        if (!(movement.flags & 8U)) {
            movement.targetHeightScale = static_cast<std::int8_t>(steps);
            movement.targetWorldPosition = anchor;
            movement.targetWorldYaw = static_cast<std::int16_t>(yaw);
            movement.cachedCharacterControlFlags = flags;
        } else if (movement.targetWorldPosition.y != anchor.y) {
            movement.targetHeightScale = static_cast<std::int8_t>(steps);
            movement.targetWorldPosition = anchor;
            movement.targetWorldYaw = static_cast<std::int16_t>(yaw);
        }
        movement.flags |= 8U;
        flags &= 0xffffffbdU;
        flags |= 0x900U;
        AssignCharacterControlFlags(party.character, flags);
    } else {
        if (movement.flags & 8U)
            AssignCharacterControlFlags(party.character, movement.cachedCharacterControlFlags);
        movement.flags &= ~8U;
    }
    mtwkGetControllerTargetPosition(party.motionWork, 6, right);
    mtwkGetControllerTargetPosition(party.motionWork, 5, left);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-movement.targetWorldYaw);
    matrix_stack::TranslateInverseVector(&movement.targetWorldPosition);
    Matrix4x4::TransformPoint(right, right);
    Matrix4x4::TransformPoint(left, left);
    PopCurrentMatrix(1);
    std::int32_t step = static_cast<std::int32_t>(right.y / 0.22499999f);
    std::int32_t leftStep = static_cast<std::int32_t>(left.y / 0.22499999f);
    std::int8_t &grounded = movement.groundedStepIndex;
    std::int32_t lowest = leftStep > step ? step : leftStep;
    if (lowest < 0)
        lowest = 0;
    std::int32_t scale = movement.targetHeightScale;
    if (scale > lowest)
        scale = lowest;
    grounded = static_cast<std::int8_t>(scale);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-movement.targetWorldYaw);
    matrix_stack::TranslateInverseVector(&movement.targetWorldPosition);
    Matrix4x4::TransformPoint(position, local);
    PopCurrentMatrix(1);
    local.z = -local.z;
    movement.stepDirection = 0;
    if (steps > 0) {
        std::int32_t difference, direction;
        if ((static_cast<std::int16_t>(mtwkGetEntityYaw(party.motionWork) - movement.targetWorldYaw) < 0 ?
                    -static_cast<std::int16_t>(mtwkGetEntityYaw(party.motionWork) - movement.targetWorldYaw) :
                    static_cast<std::int16_t>(mtwkGetEntityYaw(party.motionWork) - movement.targetWorldYaw)) > 0x4000) {
            difference = static_cast<std::int16_t>(party.requestedYaw - movement.targetWorldYaw);
            if (difference < 0)
                difference = -difference;
            direction = 1;
            if (difference > 0x4000)
                direction = -1;
        } else {
            difference = static_cast<std::int16_t>(party.requestedYaw - movement.targetWorldYaw);
            if (difference < 0)
                difference = -difference;
            direction = 1;
            if (difference > 0x4000)
                direction = -1;
        }
        if (direction == 1) {
            if (local.z > -0.30000001f && static_cast<float>(steps - 1) * 0.30000001f > local.z && movement.groundedStepIndex < steps)
                movement.stepDirection = 1;
        } else if (direction == -1) {
            if (local.z > -0.075000003f && static_cast<float>(steps) * 0.30000001f > local.z && movement.groundedStepIndex > 0)
                movement.stepDirection = -1;
        }
    }
}


static void UpdatePartyMotionController(PartyCompanion &party) {
    const PartyPlacementNode &placement = *party.selectedPlacement;
    PlayerMovementState &movement = *party.movement;
    Vector3 position, target, local, world, direction;
    std::int32_t animation;
    GetCharacterWorldPosition(party.character, &position);
    UpdatePartyStairController(party);
    const std::int32_t previousState = static_cast<std::int8_t>(party.motionState);
    switch (previousState) {
    case 0:
        switch (party.requestedAction) {
        case 0:
            break;
        case 3:
            if (movement.stepDirection) {
                party.motionState = 6;
                party.motionPhase = 0;
            } else {
                mtwkCopyTransition(party.motionWork, &placement.position, placement.targetYaw, 0, 1);
                mtwkCopyTransition(party.motionWork, 0, 0, 16, 0);
                party.motionState = 1;
                party.behaviorState = 1;
            }
            break;
        case 2:
            mtwkCopyTransition(party.motionWork, 0, party.requestedYaw, 13, 0);
            party.motionState = 4;
            break;
        case 13: {
            am2Vector3Subtract(&direction, plymUpdatePlayerSearchLocation(0), &party.separationTarget);
            std::int32_t yaw = Vector3_GetFixedYaw(&direction);
            if (party.movement->flags & 8U)
                yaw = party.yaw;
            mtwkCopyTransition(party.motionWork, &party.separationTarget, yaw, 0, 0);
            mtwkCopyTransition(party.motionWork, 0, 0, 2, 0);
            party.motionState = 3;
            break;
        }
        case 1:
        case 14:
            if (party.idleFrames > 150) {
                NpcRouteLiveEntry *const entry = FindNpcRouteEntry(party.name);
                if (entry) {
                    const std::int8_t *const profile = reinterpret_cast<const std::int8_t *>(entry->scriptRecord);
                    const std::uint16_t idle = SelectNpcProfileMotion(entry->node, profile[5], profile[4], profile[8]);
                    if (mtwkGetAnimationId(movement.motion, LowerBody) != idle) {
                        mtwkSetRequestedRootMode(movement.motion, 1);
                        mtwkSetFrameTime(movement.motion, 20, LowerBody);
                        mtwkRequestMotion(movement.motion, idle, LowerBody);
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    case 1:
        if (movement.stepDirection) {
            party.motionState = 6;
            party.motionPhase = 0;
            break;
        }
        mtwkCopyTransition(party.motionWork, &placement.position, placement.targetYaw, 0, 0);
        mtwkCopyTransition(party.motionWork, 0, 0, 16, 0);
        if (party.runBlend < 0.5f) {
            if (mtwkCopyTransition(party.motionWork, 0, 0, 15, 0))
                mtwkCopyTransition(party.motionWork, 0, 0, 14, 0);
        } else if (!mtwkCopyTransition(party.motionWork, 0, 0, 15, 0))
            mtwkCopyTransition(party.motionWork, 0, 0, 8, 0);
        switch (party.requestedAction) {
        case 1:
        case 13:
            mtwkCopyTransition(party.motionWork, 0, party.yaw, 13, 0);
            party.motionState = 3;
            break;
        case 14:
            mtwkCopyTransition(party.motionWork, &placement.position, placement.targetYaw, 6, 0);
            party.motionState = 3;
            break;
        case 11:
            mtwkCopyTransition(party.motionWork, 0, 0, 2, 0);
            party.motionState = 5;
            break;
        }
        if (!mtwkSetExtraFlag(party.motionWork, 58, 1) && static_cast<std::int8_t>(party.motionState) == 1)
            party.motionState = 3;
        break;
    case 3:
        switch (party.requestedAction) {
        case 3:
            mtwkCopyTransition(party.motionWork, &placement.position, placement.targetYaw, 0, 0);
            party.motionState = 1;
            break;
        case 11:
            mtwkCopyTransition(party.motionWork, 0, party.yaw, 13, 0);
            party.motionState = 5;
            break;
        }
        if (static_cast<std::int8_t>(party.motionState) == 3) {
            if (mtwkCopyTransition(party.motionWork, 0, 0, 15, 0))
                mtwkCopyTransition(party.motionWork, 0, 0, 14, 0);
            if (mtwkCopyTransition(party.motionWork, 0, 0, 3, 0))
                mtwkCopyTransition(party.motionWork, 0, 0, 2, 0);
            if (party.stateFrames > 120U) {
                mtwkCopyTransition(party.motionWork, 0, party.yaw, 13, 0);
                party.motionState = 0;
                party.behaviorState = 0;
            } else if (!mtwkSetExtraFlag(party.motionWork, 58, 1)) {
                party.motionState = 0;
                party.behaviorState = 0;
            }
        }
        break;
    case 4:
        if (!mtwkSetExtraFlag(party.motionWork, 58, 1))
            party.motionState = 0;
        break;
    case 5:
        switch (party.doorPhase) {
        case 0:
            if (!ClaimDoor(party.doorId, ctrlGetScenarioCharaName(party.character)))
                break;
            ++party.doorPhase;
        case 1:
            GetCharacterWorldPosition(party.character, &position);
            ComputeDoorIngress(party.doorId, &position, &target);
            if (mtwkCopyTransition(party.motionWork, &target, g_doorIngress.rotation.y, 0, 0)) {
                animation = GetNpcDoorAnimation(party.doorId, party.character, &position);
                mtwkSetTimeRelated(party.motionWork, 1, 5, animation);
                ++party.doorPhase;
                toggleCCMflagsByAmt(party.character, 0x800U);
            }
            break;
        case 2:
            if (!mtwkSetExtraFlag(party.motionWork, 58, 1)) {
                GetCharacterWorldPosition(party.character, &position);
                animation = GetNpcDoorAnimation(party.doorId, party.character, &position);
                BeginDoorAnimation(party.doorId, ctrlGetScenarioCharaName(party.character), animation);
                ccmClearFlags(party.character, 0x80U);
                ++party.doorPhase;
            }
            break;
        case 3:
            if (mtwkSetRegionFlag(party.motionWork, 65, 1, LowerBody)) {
                toggleCCMflagsByAmt(party.character, 0x80U);
                ccmClearFlags(party.character, 0x800U);
                ++party.doorPhase;
            }
            break;
        case 4:
            party.motionState = 0;
            party.doorPhase = 0;
            break;
        }
        break;
    case 6:
        switch (static_cast<std::int8_t>(party.motionPhase)) {
        case 0:
            if (!movement.stepDirection)
                party.motionState = 1;
            else {
                party.stairDirection = movement.stepDirection;
                matrix_stack::PushUnitMatrix();
                Matrix4x4::RotateAroundY_Wrapper(-movement.targetWorldYaw);
                matrix_stack::TranslateInverseVector(&movement.targetWorldPosition);
                Matrix4x4::TransformPoint(position, local);
                PopCurrentMatrix(1);
                local.y = movement.groundedStepIndex * 0.22499999f;
                local.z = -(movement.groundedStepIndex * 0.30000001f - 0.15000001f);
                matrix_stack::PushUnitMatrix();
                matrix_stack::TranslateVector(&movement.targetWorldPosition);
                Matrix4x4::RotateAroundY_Wrapper(movement.targetWorldYaw);
                Matrix4x4::TransformPoint(local, world);
                PopCurrentMatrix(1);
                std::int16_t yaw = movement.targetWorldYaw;
                if (party.stairDirection == -1)
                    yaw -= 0x8000;
                mtwkCopyTransition(party.motionWork, &world, yaw, 0, 0);
                mtwkCopyTransition(party.motionWork, 0, 0, 2, 0);
                ++party.motionPhase;
            }
            break;
        case 1:
            if (mtwkCopyTransition(party.motionWork, 0, 0, 3, 0))
                party.motionPhase = 0;
            else if (!mtwkSetExtraFlag(party.motionWork, 58, 1)) {
                ++party.motionPhase;
                mtwkSetRequestedRootMode(party.motionWork, 3);
                mtwkSetFrameTime(party.motionWork, 1, LowerBody);
                if (party.stairDirection == 1)
                    RequestPlayerMotion(movement, 0xf02b);
                else if (party.stairDirection == -1)
                    RequestPlayerMotion(movement, 0xf009);
            }
            break;
        case 2:
            if (!movement.stepDirection || party.requestedAction == 1) {
                if ((movement.playerMotionFlags & 2U) && (movement.playerMotionFlags & 1U)) {
                    mtwkSetRequestedRootMode(movement.motion, 1);
                    mtwkSetFrameTime(movement.motion, 20, LowerBody);
                    mtwkRequestMotion(movement.motion, GetNpcRestingMotion(party.character), LowerBody);
                    movement.flags |= 0x20000U;
                    party.motionState = 1;
                    if (party.requestedAction == 1)
                        party.motionState = 3;
                }
            } else if (mtwkSetRegionFlag(party.motionWork, 65, 1, LowerBody))
                movement.flags |= 0x20000U;
            break;
        }
        if (movement.flags & 0x20000U) {
            movement.flags &= ~0x20000U;
            matrix_stack::PushUnitMatrix();
            Matrix4x4::RotateAroundY_Wrapper(-movement.targetWorldYaw);
            matrix_stack::TranslateInverseVector(&movement.targetWorldPosition);
            Matrix4x4::TransformPoint(position, local);
            PopCurrentMatrix(1);
            local.y = movement.groundedStepIndex * 0.22499999f;
            local.z = -(movement.groundedStepIndex * 0.30000001f - 0.15000001f);
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&movement.targetWorldPosition);
            Matrix4x4::RotateAroundY_Wrapper(movement.targetWorldYaw);
            Matrix4x4::TransformPoint(local, world);
            PopCurrentMatrix(1);
            SetCharacterWorldPosition(party.character, &world);
        }
        break;
    }
    ++party.stateFrames;
    if (static_cast<std::int8_t>(party.motionState) != previousState)
        party.stateFrames = 0;
}


// @verified - d
static std::int32_t RelocatePartyAlongTrail(PartyCompanion &party) {
    std::int32_t relocated = 0;
    Vector3 player, local, sample;
    const Vector3 *point;
    std::int32_t index;
    float distance, width;
    if (party.flags & 0x100U)
        return 0;
    plymUpdatePlayerSearchLocation(&player);
    const float maximum = Vector3_PlanarSquaredDistance(party.position, player);
    float minimum = 25.0f;
    if (maximum < 100.0f)
        minimum = 0.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-CopyHeading(party.position, player));
    matrix_stack::TranslateInverseVector(&player);
    for (index = 0; index < party.trailCount && (point = PartyTrailSample(party, ~index)) != 0; ++index) {
        sample = *point;
        Matrix4x4::TransformPoint(sample, local);
        if (local.z < 0.0f)
            continue;
        distance = Vector3_PlanarSquaredDistance(player, sample);
        if (distance < minimum || distance > maximum)
            continue;
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(camCtrlGetProjectionMatrix(0));
        Matrix4x4::TransformPoint(sample, local);
        PopCurrentMatrix(1);
        if (local.z < 0.0f) {
            width = -local.z * camCtrlGetHalfVerticalFovTangent();
            if ((width < 0.0f ? -width : width) + 0.44999999f > (local.x < 0.0f ? -local.x : local.x))
                continue;
        }
        relocated = 1;
        break;
    }
    PopCurrentMatrix(1);
    if (relocated) {
        SetCharacterWorldPosition(party.character, &sample);
        party.position = sample;
    }
    return relocated;
}


static std::int32_t RelocatePartyAcrossQronFloor(PartyCompanion &party) {
    Vector3 probe, previous, player;
    const Vector3 *point;
    std::int32_t index;
    float floor;
    std::int32_t relocated = 0;
    if (!MatchAreaNamePattern(g_currentMap, SHEN_NAME(Q???)))
        return 0;
    if ((GetQronCurrentFlags() & 1U) == 0)
        return 0;
    plymUpdatePlayerSearchLocation(&player);
    probe = party.position;
    probe.y += 0.5f;
    floor = FindFieldIntersectionHeight(&probe);
    if (floor != -FLT_MAX && party.position.y - floor < 2.6999998f) {
        if (!(party.movement->flags & 8U))
            toggleCCMflagsByAmt(party.character, 2U);
    } else
        ccmClearFlags(party.character, 2U);
    if (floor != -FLT_MAX) {
        if (party.position.y - floor < 2.6999998f)
            return 0;
        if (party.position.y - player.y < 1.8f)
            return 0;
    }
    previous = player;
    for (index = 0; index < party.trailCount && (point = PartyTrailSample(party, index + 1)) != 0; ++index) {
        probe = *point;
        probe.y += 0.5f;
        floor = FindFieldIntersectionHeight(&probe);
        probe.y = point->y;
        if (floor == -FLT_MAX || (probe.y = floor, point->y - floor > 1.8f)) {
            relocated = 1;
            break;
        }
        previous = probe;
    }
    if (relocated) {
        SetCharacterWorldPosition(party.character, &previous);
        party.position = previous;
    }
    return relocated;
}


#define PARTY_FABS(value) ((value) < 0.0f ? -(value) : (value))

// @verified - d
static std::int32_t FindPartySeparationTarget(PartyCompanion &party) {
    PlayerMovementState *const movement = party.movement;
    Vector3 player, local, first, second, firstTarget, secondTarget, hit, back;
    Matrix4x4 playerFrame;
    Vector3 origin, probe, stairOrigin;
    std::int32_t stairYaw, yaw, delta, firstStairs, secondStairs, ret;
    std::int16_t angle;
    float radius, width;
    if (!ctrlGetRootNode())
        return 0;
    plymUpdatePlayerSearchLocation(&player);
    yaw = playerGetRotY();
    origin.x = party.position.x;
    origin.y = party.position.y + 0.40000001f;
    origin.z = party.position.z;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-yaw);
    matrix_stack::TranslateInverseVector(&player);
    matrix_stack::GetMatrix(&playerFrame);
    Matrix4x4::TransformPoint(origin, local);
    PopCurrentMatrix(1);
    radius = GetCharacterCollisionCapsuleMargin(ctrlGetRootNode());
    width = radius * 2.0f;
    if (PARTY_FABS(local.x) > width * 0.94999999f)
        return 0;
    if (local.z < 0.0f) {
        ret = 0;
        width = radius * 2.0f;
        if (PARTY_FABS(local.z) < 1.5f && PARTY_FABS(local.x) > 0.25f) {
            first = local;
            first.x = width;
            second = local;
            second.x = -width;
            ret = 1;
        }
    } else {
        ret = 1;
        first = local;
        first.x = width;
        second = local;
        second.x = -width;
    }
    if (!ret)
        return 0;
    if (!(party.movement->flags & 8U)) {
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&player);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::TransformPoint(first, first);
        Matrix4x4::TransformPoint(second, second);
        PopCurrentMatrix(1);
        if (FindFieldSegmentOffsetInWorld(&hit, &first, &origin)) {
            am2Vector3Subtract(&back, &first, &origin);
            dc::sdk::SetVectorLength(&back, &back, -0.0099999998f);
            am2Vector3Add(&first, &hit, &back);
        }
        RaycastInWorldGrid(&firstTarget, &first, radius);
        probe = firstTarget;
        probe.y += 0.5f;
        probe.y = FindFieldIntersectionHeight(&probe);
        firstStairs = flddQueryStairOrigin(&probe, &stairOrigin, &stairYaw);
        if (FindFieldSegmentOffsetInWorld(&hit, &second, &origin)) {
            am2Vector3Subtract(&back, &second, &origin);
            dc::sdk::SetVectorLength(&back, &back, -0.0099999998f);
            am2Vector3Add(&second, &hit, &back);
        }
        RaycastInWorldGrid(&secondTarget, &second, radius);
        probe = secondTarget;
        probe.y += 0.5f;
        probe.y = FindFieldIntersectionHeight(&probe);
        secondStairs = flddQueryStairOrigin(&probe, &stairOrigin, &stairYaw);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&playerFrame);
        Matrix4x4::TransformPoint(firstTarget, first);
        Matrix4x4::TransformPoint(secondTarget, second);
        PopCurrentMatrix(1);
    } else {
        angle = movement->targetWorldYaw;
        delta = static_cast<std::int16_t>(movement->targetWorldYaw - yaw);
        if ((delta < 0 ? -delta : delta) > 0x3fff)
            angle = movement->targetWorldYaw + 0x8000;
        first.x = 0.5f;
        first.y = 0.40000001f;
        first.z = 0.0f;
        second.x = -0.5f;
        second.y = 0.40000001f;
        second.z = 0.0f;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&origin);
        Matrix4x4::RotateAroundY_Wrapper(angle);
        Matrix4x4::TransformPoint(first, first);
        Matrix4x4::TransformPoint(second, second);
        PopCurrentMatrix(1);
        if (FindFieldSegmentOffsetInWorld(&hit, &first, &origin)) {
            am2Vector3Subtract(&back, &first, &origin);
            dc::sdk::SetVectorLength(&back, &back, -0.0099999998f);
            am2Vector3Add(&first, &hit, &back);
        }
        RaycastInWorldGrid(&firstTarget, &first, radius);
        if (FindFieldSegmentOffsetInWorld(&hit, &second, &origin)) {
            am2Vector3Subtract(&back, &second, &origin);
            dc::sdk::SetVectorLength(&back, &back, -0.0099999998f);
            am2Vector3Add(&second, &hit, &back);
        }
        RaycastInWorldGrid(&secondTarget, &second, radius);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&playerFrame);
        Matrix4x4::TransformPoint(firstTarget, first);
        Matrix4x4::TransformPoint(secondTarget, second);
        PopCurrentMatrix(1);
        secondStairs = 0;
        firstStairs = 0;
    }
    if (!ret)
        return 0;
    if (local.z < 0.0f) {
        if (local.x < 0.0f) {
            if (PARTY_FABS(second.x) > width * 0.85000002f && !secondStairs)
                party.separationTarget = secondTarget;
            else if (PARTY_FABS(first.x) > width * 0.85000002f && !firstStairs)
                party.separationTarget = firstTarget;
            else if (PARTY_FABS(second.x) > width * 0.5f && !secondStairs)
                party.separationTarget = secondTarget;
            else if (PARTY_FABS(first.x) > width * 0.5f && !firstStairs)
                party.separationTarget = firstTarget;
            else
                ret = 0;
        } else {
            if (PARTY_FABS(first.x) > width * 0.85000002f && !firstStairs)
                party.separationTarget = firstTarget;
            else if (PARTY_FABS(second.x) > width * 0.85000002f && !secondStairs)
                party.separationTarget = secondTarget;
            else if (PARTY_FABS(first.x) > width * 0.5f && !firstStairs)
                party.separationTarget = firstTarget;
            else if (PARTY_FABS(second.x) > width * 0.5f && !secondStairs)
                party.separationTarget = secondTarget;
            else
                ret = 0;
        }
    } else {
        ret = 1;
        if (local.x > 0.0f) {
            if (PARTY_FABS(first.x) > width * 0.85000002f && !firstStairs)
                party.separationTarget = firstTarget;
            else if (PARTY_FABS(second.x) > width * 0.85000002f && !secondStairs)
                party.separationTarget = secondTarget;
            else if (PARTY_FABS(first.x) > width * 0.5f && !firstStairs)
                party.separationTarget = firstTarget;
            else if (PARTY_FABS(second.x) > width * 0.5f && !secondStairs)
                party.separationTarget = secondTarget;
            else
                ret = 0;
        } else {
            if (PARTY_FABS(second.x) > width * 0.85000002f && !secondStairs)
                party.separationTarget = secondTarget;
            else if (PARTY_FABS(first.x) > width * 0.85000002f && !firstStairs)
                party.separationTarget = firstTarget;
            else if (PARTY_FABS(second.x) > width * 0.5f && !secondStairs)
                party.separationTarget = secondTarget;
            else if (PARTY_FABS(first.x) > width * 0.5f && !firstStairs)
                party.separationTarget = firstTarget;
            else
                ret = 0;
        }
    }
    return ret;
}


#undef PARTY_FABS

static void UpdatePartyFollowBehavior(PartyCompanion &party) {
    PartyPlacementNode *const current = &party.placement;
    const PartyPlacementNode &placement = *party.selectedPlacement;
    Vector3 hit, player, search;
    bool relocated = false;
    float limit;
    playerGetRotY();
    plymUpdatePlayerSearchLocation(&player);
    float distance = Vector3_ComputeSquaredDistance(player, party.position);
    const float height = party.position.y - player.y;
    if ((height < 0.0f ? -height : height) > 3.2399998f)
        distance += 36.0f;
    party.requestedAction = 0;
    if ((party.flags & 0x40U) && static_cast<std::int8_t>(party.behaviorState) != 7) {
        party.behaviorState = 7;
        party.motionState = 0;
    }
    if ((placement.flags & 8U) && RelocatePartyAlongTrail(party)) {
        party.behaviorState = 0;
        party.motionState = 0;
        mtwkCopyTransition(party.motionWork, 0, party.yaw, 13, 0);
        relocated = true;
    }
    if (RelocatePartyAcrossQronFloor(party)) {
        party.behaviorState = 0;
        party.motionState = 0;
        mtwkCopyTransition(party.motionWork, 0, party.yaw, 13, 0);
        relocated = true;
    }
    switch (static_cast<std::int8_t>(party.behaviorState)) {
    case 0: {
        party.runBlend = 0.0f;
        if (relocated)
            break;
        if (static_cast<std::int8_t>(party.motionState) == 1) {
            party.behaviorState = 1;
            break;
        }
        party.requestedAction = 1;
        ++party.idleFrames;
        limit = (party.followDistance + 0.55000001f) * (party.followDistance + 0.55000001f);
        if (party.flags & 0x20000U)
            limit = 0.3025f;
        if (distance > limit) {
            party.requestedAction = 3;
            break;
        }
        if (MatchAreaNamePattern(g_currentMap, SHEN_NAME(Q???)) && (GetQronCurrentFlags() & 1U)) {
            plymUpdatePlayerSearchLocation(&search);
            if (GetQronFloorAtPosition(search) != GetQronFloorAtPosition(party.position))
                break;
        }
        if (static_cast<std::int32_t>(party.frameCount) >= 11 && FindPartySeparationTarget(party))
            party.requestedAction = 13;
        break;
    }
    case 1: {
        std::uint32_t door = flddQueryCategory6InRadius(&party.position);
        if (((door >> 8) & 0xffU) == 0x35U)
            door = 0;
        if (door && (placement.flags & 0x40000U) && FindFieldSegmentOffsetInWorld(&hit, &placement.position, &party.position)) {
            party.requestedAction = 11;
            party.behaviorState = 8;
            party.doorPhase = 0;
            party.doorId = shen_name(door);
            break;
        }
        if (party.flags & 0x800U) {
            if (party.runBlend < 0.5f) {
                if (distance > 225.0f)
                    party.runBlend = 1.0f;
            } else if (distance < 100.0f)
                party.runBlend = 0.0f;
        }
        limit = party.followDistance * party.followDistance;
        if (party.flags & 0x20000U)
            limit = 0.0f;
        if (distance < limit)
            party.requestedAction = 1;
        if (current->distance < 0.55000001f && (height < 0.0f ? -height : height) < 1.8f)
            party.requestedAction = 1;
        break;
    }
    case 6:
    case 7:
        if (!(party.flags & 0x40U))
            party.behaviorState = 0;
        break;
    case 8:
        if (party.doorPhase == 4)
            party.behaviorState = 0;
        else
            party.requestedAction = 11;
        break;
    default:
        break;
    }
    if (party.behaviorState)
        party.idleFrames = 0;
}


// @verified - d
static void UpdatePartyGuideBehavior(PartyCompanion &party) {
    PartyPlacementNode &placement = party.placement;
    PartyPlacementNode *selected = party.selectedPlacement;
    const bool conversation = IsConversationCameraCharacter(party.character);
    Vector3 position, player, local, target;
    GetCharacterWorldPosition(party.character, &position);
    position.y += 0.15000001f;
    plymUpdatePlayerSearchLocation(&player);
    Vector3_ComputeSquaredDistance(player, position);
    if (party.flags & 0x40000U) {
        if (placement.distance > 0.55000001f)
            party.flags &= ~0x40000U;
    } else if (!(placement.distance > 0.55000001f) && (placement.flags & 0x20U)) {
        party.flags |= 0x40000U;
    }
    party.requestedAction = 0;
    switch (static_cast<std::int8_t>(party.behaviorState)) {
    case 0:
        party.runBlend = 0.0f;
        if (conversation && (party.flags & 0x40000U)) {
            party.behaviorState = 6;
            break;
        }
        if (placement.distance < 0.55000001f && (placement.flags & 0x20U)) {
            matrix_stack::PushUnitMatrix();
            Matrix4x4::RotateAroundY_Wrapper(-placement.targetYaw);
            matrix_stack::TranslateInverseVector(&party.position);
            Matrix4x4::TransformPoint(player, local);
            PopCurrentMatrix(1);
            std::int16_t yaw;
            if (local.x < 0.0f)
                yaw = placement.targetYaw + 24576;
            else
                yaw = placement.targetYaw - 24576;
            std::int32_t difference = static_cast<std::int16_t>(yaw - party.yaw);
            if (difference < 0)
                difference = -difference;
            if (difference > 0x2000) {
                party.requestedYaw = yaw;
                party.requestedAction = 2;
                return;
            }
        }
        if (!(selected->flags & 8U) && !(party.flags & 0x80U) && !(placement.distance < 0.55000001f))
            party.requestedAction = 3;
        break;
    case 1:
        if (!(selected->flags & 8U)) {
            if (placement.distance < 0.55000001f)
                party.requestedAction = 14;
            if (!(party.flags & 0x80U))
                return;
        }
        party.requestedAction = 1;
        break;
    case 6:
    case 7:
        party.runBlend = 0.0f;
        if (!conversation)
            party.behaviorState = 0;
        else if (party.flags & 0x40000U) {
            target.x = target.y = target.z = 0.0f;
            if (ctrlGetRootNode())
                charaGetMotWorldPos(ctrlGetRootNode(), 5, &target);
            RequestNpcSpecialMotionTarget(party.name, &target);
        }
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 8:
    default:
        party.runBlend = 0.0f;
        break;
    }
}


static void PartyBehaviorNullsub(PartyCompanion &) {
}


static void PartyBehaviorClearFlag(PartyCompanion &party) {
    party.flags &= ~0x10U;
}


static void UpdatePartyBehavior(PartyCompanion &party) {
    party.requestedYaw = party.selectedPlacement->characterBearingYaw;
    switch (party.mode) {
    case 1:
        UpdatePartyFollowBehavior(party);
        break;
    case 2:
        UpdatePartyGuideBehavior(party);
        break;
    case 3:
        PartyBehaviorNullsub(party);
        break;
    case 4:
        PartyBehaviorClearFlag(party);
        break;
    }
}
#endif

void UpdatePartyControl(PartyCompanion &party) {
    PreparePartyPlacement(party);
    if (party.flags & 0x2000U)
        return;
    FindPartyObstacles(party);
    SelectPartyAvoidancePlacement(party);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    UpdatePartyBehavior(party);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    party.requestedAction = 0;
    party.requestedYaw = party.selectedPlacement->characterBearingYaw;
    if (party.frameCount >= 2U) {
        if (party.mode == 1)
            UpdatePartyFollowBehavior(party);
        else if (party.mode == 2)
            UpdatePartyGuideBehavior(party);
        else if (party.mode == 4)
            party.flags &= ~0x10U;
    }
#endif
    UpdatePartyMotionController(party);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t UpdatePartyRequestedPlacement(PartyCompanion &party) {
    if (party.flags & 0x200U) {
        ResetPartyPlacementNearPlayer(party);
        party.flags &= 0xfffff9ffU;
    }
    return 0;
}
#endif

// @verified - d
void UpdatePartyCompanion(Character *character, void *payload) {
    PartyCompanion &party = *static_cast<PartyCompanion *>(payload);

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const bool inactive = (party.flags & 2U) == 0;
#endif
    party.character = character;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (!(party.flags & 2U) && party.mode == 1) {
        Vector3 position;
        GetPlayerTargetWorldPosition(&position);
        RecordPartyPlayerTrail(party, position);
    }
    if (party.flags & 2U) {
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (inactive && party.mode == 1)
        RecordPartyPlayerTrail(party, GetPlayerTargetWorldPosition());
    if (!(party.flags & 2U))
        return;
#endif
        ++party.frameCount;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
        UpdatePartyRequestedPlacement(party);
#endif
        UpdatePartyCharacterState(party);
        UpdatePartyPlayerProximity(party);
        UpdatePartyControl(party);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    }
#endif
}

} // namespace model_component_detail

 // namespace model_component_detail

// @verified - d
PartyCompanion *FindPartyCompanion(const Character *character) {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return character ? static_cast<PartyCompanion *>(charFindComp(character, SHEN_NAME(PTYC))) : 0;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return static_cast<PartyCompanion *>(FindModelComponent(character, kPartyToken));
#endif
}


// @verified - d
shen_name FindNavigationPartyCompanion(shen_name name) {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const PartyCompanion *party = 0;
    std::uint32_t names[4] = {SHEN_NAME4('R', 'E', 'N', '_'), SHEN_NAME4('S', 'I', 'N', '_'), SHEN_NAME4('J', 'O', 'P', '_'), 0};
    if (!name) {
        const std::uint32_t *entry = names;
        for (;;) {
            if (!*entry)
                break;
            party = FindPartyCompanionByName(shen_name(*entry));
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    PartyCompanion *party = nullptr;
    if (name.name) {
        party = FindPartyCompanion(ctrlGetScenarioChara(name));
    } else {
        for (const shen_name candidate : kPartyNames) {
            party = FindPartyCompanion(ctrlGetScenarioChara(candidate));
#endif
            if (party)
                break;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
            ++entry;
#endif
        }

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    } else
        party = FindPartyCompanionByName(name);
    if (!party)
        return shen_name(0);
    if (!(party->flags & 2U))
        return shen_name(0);
    if (!(party->flags & 0x10000000U))
        return shen_name(0);
    return party->name;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    }
    return party && (party->flags & 0x10000002U) == 0x10000002U ? party->name : shen_name{};
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsPartyCompanion(shen_name name) {
    if (!FindPartyCompanionByName(name))
        return 0;
    return 1;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsPartyCompanionPlaced(shen_name name) {
    const PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    return party && (party->flags & 0x202U) == 2U;
}

bool IsPartyCompanionOnStairs(shen_name name) {
    const PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    return party && (party->movement->flags & 8U) != 0;
}

// @verified - p
std::int64_t ReleaseAbsentPartyCharacter(shen_name name) {
    if (ctrlGetScenarioChara(name))
        return 0xffffffffLL;
    EV_npcFree(name);
    return 0;
}

// @verified - p
void InitializePartyCompanion(PartyCompanion &party) {
    party.mode = 2;
    party.flags = 0;
    party.name = {};
    party.character = nullptr;
    party.behaviorState = 0;
    party.motionState = 0;
    party.runBlend = 0.0f;
    party.frameCount = 0;
    std::memset(&party.placement, 0, sizeof(party.placement));
    std::memset(&party.trailPlacement, 0, sizeof(party.trailPlacement));
    party.selectedPlacement = nullptr;
    party.movement = &party.embeddedMovement;
    InitializePlayerMovementState(party.embeddedMovement);
    party.trailDistance = 0.0f;
    party.trailWriteIndex = 0;
    party.trailCount = 0;
    party.selectedTrailIndex = -1;
    party.followDistance = 3.5f;
    party.routeMarker = 0;
    party.routeIndex = -1;
}

// @verified - p
bool AttachPartyCompanion(Character *character) {
    if (!character)
        return false;
    PartyCompanion *const party = static_cast<PartyCompanion *>(AllocateModelComponent(character, kPartyToken, sizeof(PartyCompanion), UpdatePartyCompanion));
    if (!party)
        return false;
    InitializePartyCompanion(*party);
    party->flags |= 0xa12U;
    party->name = ctrlGetScenarioCharaName(character);
    party->character = character;
    party->movement->character = character;
    party->movement->flags |= 0x24U;
    mtwkSetEnabled(FindMotionWork(character), true);
    cpamSetFlag(party->character, 1);
    toggleCCMflagsByAmt(character, 0x300000U);
    if (party->name.name == kPartyNames[1].name) {
        SetCharacterType(character, CharacterTypeStencilShadow);
        party->flags |= 0x800U;
    }
    party->routeMarker = g_currentMap.name == SHEN_NAME(KMZ1) ? 9 : 0;
    if (MatchAreaNamePattern(g_currentMap, AREA_PATTERN_MAZE)) {
        party->flags |= 0x40000000U;
    }
    if (MotionWork *const motion = FindMotionWork(character)) {
        party->motionWork = motion;
        party->movement->motionWork = motion;
        ConfigurePartyMotion(*party);
    }
    if (partyTransition.present && partyTransition.name.name == party->name.name) {
        party->flags &= ~0x200U;
        shenApplyTranslationToAll(&character->entity.node, partyTransition.position);
        shenDecrementRotYAllNodes(&character->entity.node, partyTransition.yaw);
        party->routeIndex = partyTransition.routeIndex;
        party->routeMarker = partyTransition.routeMarker;
        party->mode = partyTransition.mode;
        partyTransition.present = 0;
        party->character->alpha = 0.0f;
        charaSetFlagFor(party->character, 1);
        toggleCCMflagsByAmt(character, CCMF_ResetMotion);
        if (party->mode == 1)
            party->flags |= 0x200U;
        if (party->motionWork)
            mtwkSetRegionFlag(party->motionWork, 71, 0, MotionRegion::LowerBody);
    }
    return true;
}

// An existing PTYC restarts from its idle states with full control and a fresh motion setup.
// @verified - p
void ReactivatePartyCompanion(PartyCompanion &party) {
    party.flags |= 0x402U;
    party.behaviorState = 0;
    party.motionState = 0;
    party.motionPhase = 0;
    party.runBlend = 0.0f;
    party.frameCount = 0;
    const std::int16_t yaw = GetAllAngYForNode(&party.character->entity.node);
    mtwkCopyTransition(FindMotionWork(party.character), nullptr, yaw, 13, 0);
    toggleCCMflagsByAmt(party.character, 0x300000U);
    cpamSetFlag(party.character, 1);
    if (MotionWork *const motion = FindMotionWork(party.character)) {
        party.motionWork = motion;
        party.movement->motionWork = motion;
        ConfigurePartyMotion(party);
    }
}

// @verified - p
// @verified - d
std::int32_t ActivatePartyCompanion(shen_name name) {
    EV_CheckWarmStartNpcRoute(name);
    if (!EV_ForceLoadNpc(name))
        return -1;
    EV_CheckWarmStartNpcRoute(name);
    Character *const character = ctrlGetScenarioChara(name);
    if (!character)
        return -1;
    if (PartyCompanion *const party = FindPartyCompanion(character)) {
        party->character = character;
        ReactivatePartyCompanion(*party);
    } else if (!AttachPartyCompanion(character))
        return -1;
    MarkNpcRoutePartyControl(name);
    ReleaseCompletedPartyGuideRoute();
    return 0;
}

// @verified - p
std::int32_t DeactivatePartyCompanion(shen_name name) {
    Character *const character = ctrlGetScenarioChara(name);
    if (!character)
        return -1;
    PartyCompanion *const party = FindPartyCompanion(character);
    if (!party)
        return -1;
    party->flags &= ~2U;
    EV_CheckWarmStartNpcRoute(name);
    EV_npcFree(name);
    if (party->motionState == 5 && party->doorId != 0 && party->doorPhase > 0) {
        party->doorPhase = 0;
        ReleaseDoor(static_cast<std::int32_t>(party->doorId), ctrlGetScenarioCharaName(party->character));
    }
    return 0;
}

std::int32_t UnlockPartyCompanion(shen_name name) {
    PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    if (!party)
        return -1;
    if (!(party->flags & 0x40000000U) && name.name != kPartyNames[1].name) {
        if (!(party->flags & 0x2000U))
            EV_npcLockUnlock(name);
        party->flags &= ~2U;
        if (!(party->flags & 0x2000U) && party->motionState == 5 && party->doorId && party->doorPhase > 0) {
            party->doorPhase = 0;
            ReleaseDoor(static_cast<std::int32_t>(party->doorId), ctrlGetScenarioCharaName(party->character));
        }
    }
    return 0;
}

// @verified - p
std::int32_t plymGetPartyAuthorStatus(shen_name name) {
    Character *const character = ctrlGetScenarioChara(name);
    if (!character)
        return 0;
    const PartyCompanion *const party = FindPartyCompanion(character);
    if (!party)
        return 0;
    if (party->mode != 2)
        return party->behaviorState == 0 || party->motionState == 0;
    if (!(party->placement.flags & 0x20U) && !(party->flags & 0x40000U))
        return 0;
    return party->behaviorState ? 2 - (party->motionState != 0) : 2;
}
#endif

// @verified - d
std::int32_t FindPTYCForChara(shen_name name) {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const PartyCompanion *const party = FindPartyCompanionByName(name);
    return party ? (static_cast<std::int32_t>(party->flags) >> 1) & 1 : 0;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    return party && (party->flags & 2U) != 0;
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t SetPartyCompanionActiveBit(shen_name name, std::int32_t enabled) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return 0;
    std::int32_t previous = static_cast<std::int32_t>(party->flags) >> 1;
    previous &= 1;
    if (enabled)
        party->flags |= 2U;
    else
        party->flags &= ~2U;
    return previous;
}


// @verified - d
std::int32_t SetPartyCompanionMode(shen_name name, std::int32_t mode) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return 0;
    switch (mode) {
    case 0:
    default:
        party->mode = 0;
        break;
    case 1:
        party->mode = 1;
        break;
    case 2:
        party->mode = 2;
        break;
    case 4:
        party->mode = 4;
        party->behaviorState = 10;
        break;
    }
    return 1;
}


std::int32_t IsPartyCompanionBehaviorIdle(shen_name name) {
    const PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party || !(party->flags & 2U))
        return 0;
    if (*reinterpret_cast<const std::uint32_t *>(&party->behaviorState) & 0xffffU)
        return 0;
    return 1;
}


std::int32_t PausePartyConversation(shen_name name) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return -1;
    party->flags |= 0x40U;
    return 0;
}


// @verified - d
std::int32_t ResumePartyConversation(shen_name name) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return -1;
    party->flags &= ~0x40U;
    return 0;
}


// @verified - d
void ReactivatePartyCompanion(PartyCompanion &party) {
    party.flags |= 0x402U;
    party.behaviorState = 0;
    party.motionState = 0;
    party.motionPhase = 0;
    party.runBlend = 0.0f;
    party.frameCount = 0;
    mtwkCopyTransition(charFindMotionwork(party.character), 0, GetCharacterWorldYaw(party.character), 13, 0);
    toggleCCMflagsByAmt(party.character, 0x300000U);
    cpamSetFlag(party.character, 1);
}


// @verified - d
void ReleaseConversationBuffer(ConversationBuffer &buffer) {
    StopConversationStream(buffer.stream);
    ReleaseConversationPacket(buffer.packetRequest);
    if (buffer.speaker) {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        scenAssignFacialClip(control->scenarioTask, defaultFacialClip, buffer.speaker);
    }
    buffer.packetRequest = 0;
    buffer.callback = 0;
}


// @verified - d
void PauseOrDestroyAudioAndSubtitles(std::int32_t paused) {
    if (GetConversationPlaybackEnabled())
        return;
    if (paused) {
        ConversationBuffer *const buffers = g_conversationTaskData->buffers;
        for (std::int32_t index = 0; index < 2; ++index)
            if (buffers[index].callback)
                ReleaseConversationBuffer(buffers[index]);
    } else {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        scenDestroySubtitles(control->scenarioTask);
    }
}


// @verified - d
std::int32_t InitializePartyGuide() {
    shen_name name;
    ProcGenInfo &guide = *g_pProcGenInfos;
    std::int32_t result = 0;
    guide.initialised = 0;
    guide.maze = 0;
    guide.routeIndex = -1;
    guide.parameter = 0.0f;
    guide.name = 0;
    guide.currentParameter = 0;
    guide.stride = 1;
    guide.maximum = 0;
    guide.low = 0;
    guide.direction = 1;
    guide.partition = -1;
    guide.partitionCount = 0;
    if (!MatchAreaNamePattern(g_currentMap, SHEN_NAME4('K', 'M', 'Z', '?'))) {
        guide.maze = 0;
        name = SHEN_NAME4('M', '0', '1', '_');
        guide.name = name;
        guide.currentParameter = GetNpcRouteCurrentParameter(name);
        guide.stride = GetNpcRouteStride(name);
        guide.low = GetNpcRouteLowParameter(name);
        guide.high = GetNpcRouteHighParameter(name);
        guide.maximum = GetNpcRouteMaximumParameter(name);
        guide.parameter = static_cast<float>(guide.currentParameter);
        if (IsNpcRouteReady(name))
            result = 1;
    } else {
        guide.maze = 1;
        guide.partitionCount = static_cast<std::int8_t>(mazeGetNumPartitions());
        result = 1;
    }
    if (result)
        guide.initialised = 1;
    return result;
}


std::int32_t PartyGuideInitializeAdapter() {
    return InitializePartyGuide();
}


// @verified - d
std::int32_t PickClosestTargetForPlacement(const Vector3 *point) {
    ProcGenInfo &guide = *g_pProcGenInfos;
    if (!guide.initialised)
        return 0;
    if (!guide.maze) {
        guide.currentParameter = FindNearestNpcRouteParameter(guide.name, point);
        guide.parameter = static_cast<float>(guide.currentParameter);
    } else {
        std::int32_t partition, element;
        element = -1;
        partition = -1;
        if (guide.routeIndex != -1) {
            partition = (guide.routeIndex >> 16) & 0xffff;
            element = guide.routeIndex & 0xffff;
        }
        mazeFindClosestTarget(&partition, &element, point);
        guide.partition = static_cast<std::int8_t>(partition);
        guide.currentParameter = element;
        guide.parameter = static_cast<float>(guide.currentParameter);
        guide.low = 0;
        guide.high = GetMZNRDataForID(guide.partition) - 1;
        guide.maximum = guide.high;
    }
    guide.routeIndex = GetPartyGuideRouteIndex();
    return guide.routeIndex;
}


std::int32_t GetPartyGuideRouteIndex() {
    const ProcGenInfo &guide = *g_pProcGenInfos;
    std::int32_t routeIndex;
    if (!guide.maze)
        routeIndex = guide.currentParameter;
    else
        routeIndex = (guide.currentParameter & 0xffff) | (guide.partition << 16);
    return routeIndex;
}


// @verified - d
void FindClosestTargetForPlacement(const Vector3 *point) {
    ProcGenInfo &guide = *g_pProcGenInfos;
    if (!guide.initialised)
        return;
    if (!point) {
        guide.targetPartition = guide.partition;
        guide.targetParameter = guide.high;
    } else if (!guide.maze) {
        guide.targetPartition = guide.partition;
        guide.targetParameter = FindNearestNpcRouteParameter(guide.name, point);
    } else {
        std::int32_t partition, element;
        element = -1;
        partition = -1;
        if (guide.routeIndex != -1) {
            partition = (guide.routeIndex >> 16) & 0xffff;
            element = guide.routeIndex & 0xffff;
        }
        mazeFindClosestTarget(&partition, &element, point);
        guide.targetPartition = partition;
        guide.targetParameter = element;
    }
}


// @verified - d
std::int32_t SetPartyGuideRouteHint(std::int32_t routeIndex) {
    g_pProcGenInfos->routeIndex = routeIndex;
    return 0;
}


void SetPartyGuideBackward() {
    g_pProcGenInfos->direction = -1;
}


// @verified - d
std::int32_t SetPartyGuideDirection() {
    ProcGenInfo &guide = *g_pProcGenInfos;
    if (!guide.initialised)
        return 0;
    std::int32_t direction;
    if (!guide.maze) {
        direction = 0;
        if (static_cast<float>(guide.targetParameter) > guide.parameter)
            direction = 1;
        if (static_cast<float>(guide.targetParameter) < guide.parameter)
            direction = -1;
    } else {
        std::int32_t partition, element;
        direction = mazeFindPartitionDirection(
            guide.partition, static_cast<std::int32_t>(guide.parameter), guide.targetPartition, guide.targetParameter, &partition, &element);
    }
    guide.direction = static_cast<std::int8_t>(direction);
    return direction;
}


std::int32_t RefreshPartyGuideDirection() {
    return SetPartyGuideDirection();
}


// @verified - d
std::int32_t PartyGuideEndState() {
    const ProcGenInfo &guide = *g_pProcGenInfos;
    if (!guide.initialised)
        return 99;
    if (!guide.direction)
        return 1;
    if (guide.direction > 0) {
        if (guide.partition == guide.targetPartition) {
            if (!(guide.parameter < static_cast<float>(guide.targetParameter)))
                return 1;
        } else if (!(guide.parameter < static_cast<float>(guide.high)))
            return -1;
    } else {
        if (guide.partition == guide.targetPartition) {
            if (!(guide.parameter > static_cast<float>(guide.targetParameter)))
                return 1;
        } else if (!(guide.parameter > static_cast<float>(guide.low)))
            return -1;
    }
    return 0;
}


// @verified - d
std::int32_t AdvancePartyGuidePartition() {
    ProcGenInfo &guide = *g_pProcGenInfos;
    if (!guide.maze)
        return 1;
    std::int32_t partition, element;
    if (!mazeFindPartitionDirection(
            guide.partition, static_cast<std::int32_t>(guide.parameter), guide.targetPartition, guide.targetParameter, &partition, &element))
        return 0;
    guide.partition = static_cast<std::int8_t>(partition);
    guide.parameter = static_cast<float>(element);
    guide.low = 0;
    guide.high = GetMZNRDataForID(partition) - 1;
    guide.direction = static_cast<std::int8_t>(mazeFindPartitionDirection(
        guide.partition, static_cast<std::int32_t>(guide.parameter), guide.targetPartition, guide.targetParameter, &partition, &element));
    return 1;
}


// @verified - d
std::int32_t AdvancePartyGuide(float distance) {
    ProcGenInfo &guide = *g_pProcGenInfos;
    std::int32_t steps = 0;
    if (!guide.maze) {
        float length = 0.0f;
        const std::int32_t start = static_cast<std::int32_t>(guide.parameter);
        std::int32_t parameter = start;
        float minimum = 0.0f;
        const float maximum = static_cast<float>(guide.maximum);
        if (guide.direction > 0) {
            for (;;) {
                if (!(maximum > static_cast<float>(parameter)))
                    break;
                const float segment = GetNpcRouteSegmentLength(guide.name, parameter);
                ++parameter;
                if (segment < 0.0f)
                    break;
                length += segment;
                steps = parameter - start;
                if (length > distance)
                    break;
            }
        } else {
            for (;;) {
                if (!(static_cast<float>(parameter) > minimum))
                    break;
                const float segment = GetNpcRouteSegmentLength(guide.name, --parameter);
                if (segment < 0.0f)
                    break;
                length += segment;
                steps = start - parameter;
                if (length > distance)
                    break;
            }
        }
    } else {
        steps = static_cast<std::int32_t>((distance + 1.99999f) * 0.5f);
    }
    guide.parameter += static_cast<float>(guide.direction * steps);
    if (guide.direction > 0) {
        if (guide.partition == guide.targetPartition && static_cast<float>(guide.targetParameter) < guide.parameter)
            guide.parameter = static_cast<float>(guide.targetParameter);
        if (guide.parameter > static_cast<float>(guide.maximum))
            guide.parameter = static_cast<float>(guide.maximum);
    } else if (guide.direction < 0) {
        if (guide.partition == guide.targetPartition && guide.parameter < static_cast<float>(guide.targetParameter))
            guide.parameter = static_cast<float>(guide.targetParameter);
        if (guide.parameter < 0.0f)
            guide.parameter = 0.0f;
    }
    const std::int32_t end = PartyGuideEndState();
    if (end > 0)
        return 1;
    if (end < 0)
        AdvancePartyGuidePartition();
    return 0;
}


// @verified - d
std::int32_t SamplePartyGuide(Vector3 *point, std::int16_t *yaw) {
    const ProcGenInfo &guide = *g_pProcGenInfos;
    std::int32_t clamped = static_cast<std::int32_t>(guide.parameter);
    if (clamped < guide.low)
        clamped = guide.low;
    std::int32_t parameter = guide.high;
    if (parameter > clamped)
        parameter = clamped;
    Vector3 next, previous;
    std::int32_t angle;
    if (!guide.maze) {
        SampleNpcRoute(guide.name, parameter, point, &angle);
        *yaw = static_cast<std::int16_t>(angle);
    } else {
        mazeSamplePartition(guide.partition, parameter, point);
        if (guide.direction >= 0) {
            if (parameter < guide.high) {
                mazeSamplePartition(guide.partition, parameter + 1, &next);
                *yaw = CopyHeading(*point, next);
            } else {
                mazeSamplePartition(guide.partition, parameter - 1, &previous);
                *yaw = CopyHeading(previous, *point);
            }
        } else {
            if (parameter > guide.low) {
                mazeSamplePartition(guide.partition, parameter - 1, &next);
                *yaw = CopyHeading(*point, next);
            } else {
                mazeSamplePartition(guide.partition, parameter + 1, &previous);
                *yaw = CopyHeading(previous, *point);
            }
        }
    }
    return 0;
}


// @verified - d
float PartyGuideLaneBounds(float &low, float &high) {
    const ProcGenInfo &guide = *g_pProcGenInfos;
    std::int32_t parameter = static_cast<std::int32_t>(guide.parameter);
    if (parameter < guide.low)
        parameter = guide.low;
    std::int32_t clamped = guide.high;
    if (clamped > parameter)
        clamped = parameter;
    if (!guide.maze)
        GetNpcRouteLaneBounds(guide.name, clamped, &low, &high);
    else {
        low = -1.5f;
        high = 1.5f;
    }
    return (low < 0.0f ? -low : low) + (high < 0.0f ? -high : high);
}


void DrawPartyGuideDebug() {
    const ProcGenInfo &guide = *g_pProcGenInfos;
    SetDebugScreenTextPosition(20, 5);
    shenmue_s2_event_DebugLogToScreen("FLAG: %d\n", guide.initialised);
    shenmue_s2_event_DebugLogToScreen("TYPE: %d\n", guide.maze);
    shenmue_s2_event_DebugLogToScreen("TIME: %d\n", static_cast<std::int32_t>(guide.parameter));
    shenmue_s2_event_DebugLogToScreen(" STA: %d\n", guide.low);
    shenmue_s2_event_DebugLogToScreen(" END: %d\n", guide.high);
    shenmue_s2_event_DebugLogToScreen("LINE: %d\n", guide.partition);
    shenmue_s2_event_DebugLogToScreen(" DIR: %d\n", guide.direction);
    shenmue_s2_event_DebugLogToScreen("TGTL: %d\n", guide.targetPartition);
    shenmue_s2_event_DebugLogToScreen("TGTT: %d\n", guide.targetParameter);
}


std::int32_t PartyGuideCallbackReinitialize() {
    return PartyGuideInitializeAdapter();
}


void PartyGuideCallbackBackward() {
    SetPartyGuideBackward();
}


PartyCompanion *FindPartyCompanionByName(shen_name name) {
    Character *const character = ctrlGetScenarioChara(name);
    return character ? static_cast<PartyCompanion *>(charFindComp(character, shen_name(PARTY_COMPANION_COMPONENT))) : 0;
}


// @verified - d
std::int32_t ActivatePartyCompanion(shen_name name) {
    EV_CheckWarmStartNpcRoute(name);
    if (!EV_ForceLoadNpc(name))
        return -1;
    EV_CheckWarmStartNpcRoute(name);
    Character *const character = ctrlGetScenarioChara(name);
    if (!character)
        return -1;
    PartyCompanion *party = static_cast<PartyCompanion *>(charFindComp(character, SHEN_NAME(PTYC)));
    if (party) {
        party->character = character;
        ReactivatePartyCompanion(*party);
    } else {
        if (AttachPartyCompanion(character) == -1)
            return -1;
        party = static_cast<PartyCompanion *>(charFindComp(character, SHEN_NAME(PTYC)));
    }
    EV_npcUpdateFlags(name);
    model_component_detail::ReleaseCompletedPartyGuideRoute(*party);
    if (name == SHEN_NAME(REN_))
        mtwkSetTimeRelated(charFindMotionwork(character), 1, 0, GetNpcRestingMotion(character));
    return 0;
}


// @verified - d
std::int32_t FreeInactivePartyCompanion(shen_name name) {
    if (ctrlGetScenarioChara(name))
        return -1;
    EV_npcFree(name);
    return 0;
}
#endif

// @verified - d
std::int32_t ptycCharaUnlock(shen_name name) {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    PartyCompanion *const party = FindPartyCompanionByName(name);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
#endif
    if (!party)
        return -1;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (!(party->flags & 0x40000000U) && name != SHEN_NAME(SIN_))
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if ((party->flags & 0x40000000U) != 0 || name.name == SHEN_NAME(SIN_))
        return 0;
    if ((party->flags & 0x2000U) == 0)
#endif
        EV_npcLockUnlock(name);
    party->flags &= ~2U;

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if ((party->flags & 0x2000U) == 0 && party->motionState == 5 && party->doorId != 0 && party->doorPhase > 0) {
        party->doorPhase = 0;
        ReleaseDoor(static_cast<std::int32_t>(party->doorId), ctrlGetScenarioCharaName(party->character));
    }
#endif
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t DeactivatePartyCompanion(shen_name name) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return -1;
    party->flags &= ~2U;
    EV_CheckWarmStartNpcRoute(name);
    EV_npcFree(name);
    return 0;
}


// @verified - d
std::int32_t SetPartyNpcRouteReload(shen_name name, std::int32_t enabled) {
    if (enabled) {
        if (!IsNpcRouteLoadedOrVisible(name))
            EV_CheckWarmStartNpcRoute(name);
    } else if (IsNpcRouteLoadedOrVisible(name)) {
        EV_npcLockUnlock(name);
    }
    return 0;
}


std::int32_t SetPartyCompanionRouteReloadFlag(shen_name name, std::int32_t enabled) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return -1;
    if (enabled) {
        party->flags |= 0x10U;
        EV_CheckWarmStartNpcRoute(name);
    } else {
        party->flags &= ~0x10U;
        EV_npcLockUnlock(name);
    }
    return 0;
}


std::int32_t IsPartyGuideCharaPresent() {
    if (ctrlGetScenarioChara(SHEN_NAME(M01_)))
        return 1;
    return 0;
}


shen_name GetPartyGuideCharaName() {
    if (ctrlGetScenarioChara(SHEN_NAME(M01_)))
        return SHEN_NAME(M01_);
    return shen_name(0);
}


// @verified - d
std::int32_t ResetPartyCompanionTransition(PartyCompanionTransition *) {
    std::uint32_t *const words = reinterpret_cast<std::uint32_t *>(g_pPartyCompanionTransition);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
    words[4] = 0;
    words[5] = 0;
    words[6] = 0;
    words[7] = 0;
    return 0;
}


// @verified - d
std::int32_t ExportPartyCompanionTransition(std::uint8_t *state) {
    PartyCompanion *party = 0;
    shen_name names[4] = {SHEN_NAME4('R', 'E', 'N', '_'), SHEN_NAME4('S', 'I', 'N', '_'), SHEN_NAME4('J', 'O', 'P', '_'), 0};
    shen_name name = 0;
    PartyCompanionTransition *const transition = g_pPartyCompanionTransition;
    std::int32_t result = 0;
    for (const shen_name *entry = names; *entry; ++entry) {
        name = *entry;
        party = FindPartyCompanionByName(name);
        if (party)
            break;
    }
    if (!party) {
        std::uint32_t *const words = reinterpret_cast<std::uint32_t *>(transition);
        words[0] = 0;
        words[1] = 0;
        words[2] = 0;
        words[3] = 0;
        words[4] = 0;
        words[5] = 0;
        words[6] = 0;
        words[7] = 0;
    } else {
        transition->present = 1;
        transition->name = name;
        GetCharacterWorldPosition(party->character, &transition->position);
        transition->yaw = static_cast<std::int16_t>(GetCharacterWorldYaw(party->character));
        transition->routeIndex = party->routeIndex;
        transition->routeMarker = party->routeMarker;
        transition->mode = party->mode;
        result = 1;
    }
    std::memcpy(state, transition, sizeof(*transition));
    ResetPartyCompanionTransition(transition);
    return result;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int32_t PTYCUtils(shen_name name, std::int32_t command, std::int32_t argument) {
    PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    if (!party)
        return 0;
    switch (command) {
    case 0:
        party->mode = argument == 1 || argument == 2 || argument == 4 ? static_cast<std::uint8_t>(argument) : 0;
        if (argument == 4)
            party->behaviorState = 10;
        return party->mode;
    case 1:
        if (argument < 0)
            return party->routeMarker;
        party->routeMarker = argument;
        return 0;
    case 2: {
        const std::uint32_t previous = (party->flags >> 1) & 1U;
        if (argument && !previous) {
            party->behaviorState = 0;
            party->motionState = 0;
            party->motionPhase = 0;
            party->flags |= 0x1002U;
        } else if (!argument && previous) {
            party->flags &= ~0x1002U;
            if (mtwkSetExtraFlag(party->motionWork, 58, 1)) {
                mtwkCopyTransition(party->motionWork, nullptr, party->yaw, 13, 0);
            }
        }
        return static_cast<std::int32_t>(previous);
    }
    case 3: {
        const std::uint32_t previous = (party->flags >> 13) & 1U;
        if (argument && !previous) {
            party->flags |= 0x2000U;
            if (mtwkSetExtraFlag(party->motionWork, 58, 1)) {
                mtwkCopyTransition(party->motionWork, nullptr, party->yaw, 13, 0);
            }
            if (party->motionState == 5 && party->doorId && party->doorPhase > 0) {
                party->doorPhase = 0;
                ReleaseDoor(static_cast<std::int32_t>(party->doorId), ctrlGetScenarioCharaName(party->character));
            }
        } else if (!argument && previous) {
            party->behaviorState = 0;
            party->motionState = 0;
            party->motionPhase = 0;
            party->flags &= ~0x2000U;
        }
        return static_cast<std::int32_t>(previous);
    }
    case 4:
        return plymGetPartyAuthorStatus(name);
    case 6:
        return 1;
    case 7:
        return static_cast<std::int32_t>((party->flags >> 1) & 1U);
    case 8:
        return DeactivatePartyCompanion(name);
    default:
        return 0;
    }
}

void ResetPartyCompanionTransition() {
    partyTransition = {};
}
#endif

// @verified - d
std::uint32_t GetPartyCompanionTransitionPending() {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return g_pPartyCompanionTransition->present;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return partyTransition.present;
#endif
}


// @verified - d
std::int32_t ImportPartyCompanionTransition(const PartyCompanionTransition &state) {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    PartyCompanionTransition *const transition = g_pPartyCompanionTransition;
#endif
    if (!state.present)
        return -1;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
    *transition = state;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    partyTransition = state;
#endif
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PartyCompanionTransitionNullsub1() {
}


void PartyCompanionTransitionNullsub2() {
}


std::int32_t SetPartyCompanionFlag2(shen_name name, std::int32_t enabled) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return -1;
    std::int32_t previous = static_cast<std::int32_t>(party->flags) >> 1;
    previous &= 1;
    if (enabled) {
        if (previous)
            return previous;
        party->flags |= 0x1002U;
        party->behaviorState = 0;
        party->motionState = 0;
        party->motionPhase = 0;
        return previous;
    }
    if (!previous)
        return 0;
    party->flags &= ~2U;
    party->flags &= ~0x1000U;
    if (mtwkSetExtraFlag(party->motionWork, 58, 1))
        mtwkCopyTransition(party->motionWork, 0, party->yaw, 13, 0);
    return previous;
}


std::int32_t SetPartyCompanionFlag2000(shen_name name, std::int32_t enabled) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return -1;
    std::int32_t previous = static_cast<std::int32_t>(party->flags) >> 13;
    previous &= 1;
    if (enabled) {
        if (previous == 0) {
            const volatile std::uint32_t &flags = party->flags;
            party->flags = flags | 0x2000U;
            if (mtwkSetExtraFlag(party->motionWork, 58, 1))
                mtwkCopyTransition(party->motionWork, 0, party->yaw, 13, 0);
            TouchCameraMathWork(reinterpret_cast<void *>(name));
        }
        return previous;
    }
    if (!previous)
        return 0;
    party->flags &= ~0x2000U;
    party->behaviorState = 0;
    party->motionState = 0;
    party->motionPhase = 0;
    TouchCameraMathWork(reinterpret_cast<void *>(name));
    return previous;
}


std::int32_t RequestPartyCompanionPosition(shen_name name, const Vector3 *position, std::int16_t yaw) {
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party || !position || party->mode != 1)
        return -1;
    if (model_component_detail::Vector3_ComputeSquaredDistance(*position, party->position) < 0.3025f && !party->behaviorState)
        return 1;
    party->requestedPosition = *position;
    party->requestedPositionYaw = yaw;
    party->flags |= 0x10000U;
    return 0;
}


// @verified - d
std::int32_t PTYCUtils(shen_name name, std::int32_t command, std::int32_t argument) {
    std::int32_t result = 0;
    PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return 0;
    switch (command) {
    case 0:
        result = SetPartyCompanionMode(name, argument);
        break;
    case 1:
        if (argument >= 0)
            party->routeMarker = argument;
        else
            result = party->routeMarker;
        break;
    case 2:
        result = SetPartyCompanionFlag2(name, argument);
        break;
    case 3:
        result = SetPartyCompanionFlag2000(name, argument);
        break;
    case 4:
        result = plymGetPartyAuthorStatus(name);
        break;
    case 5: {
        const Vector3 *const *const request = reinterpret_cast<const Vector3 *const *>(argument);
        result = RequestPartyCompanionPosition(name, request[0], *reinterpret_cast<const std::int16_t *>(request + 1));
        break;
    }
    case 6:
        result = IsPartyCompanion(name);
        break;
    case 7:
        result = FindPTYCForChara(name);
        break;
    case 8:
        result = DeactivatePartyCompanion(name);
        break;
    }
    return result;
}


std::int32_t PartyGuideCallbackInitialize() {
    return InitializePartyGuide();
}


std::int32_t PartyGuideCallbackPick(const Vector3 *point) {
    return PickClosestTargetForPlacement(point);
}


void PartyGuideCallbackFind(const Vector3 *point) {
    FindClosestTargetForPlacement(point);
}


std::int32_t PartyGuideCallbackEndState() {
    return PartyGuideEndState();
}


std::int32_t PartyGuideCallbackDirection() {
    return SetPartyGuideDirection();
}


void PartyGuideCallbackRouteHint(std::int32_t routeIndex) {
    SetPartyGuideRouteHint(routeIndex);
}


void PartyGuideCallbackSample(Vector3 *point, std::int16_t *yaw) {
    SamplePartyGuide(point, yaw);
}


std::int32_t PartyGuideCallbackAdvance(float distance) {
    return AdvancePartyGuide(distance);
}


float PartyGuideCallbackLaneBounds(float &low, float &high) {
    return PartyGuideLaneBounds(low, high);
}


// @verified - d
std::int32_t AttachPartyCompanion(Character *character) {
    using namespace model_component_detail;
    PartyCompanion *const party =
        static_cast<PartyCompanion *>(AllocateModelComponent(character, SHEN_NAME4('P', 'T', 'Y', 'C'), sizeof(PartyCompanion), UpdatePartyCompanion));
    if (!party)
        return -1;
    InitializePartyCompanion(*party);
    party->flags |= 0x12U;
    party->flags |= 0x800U;
    party->flags |= 0x200U;
    party->name = ctrlGetScenarioCharaName(character);
    party->character = character;
    party->embeddedMovement.character = character;
    PlayerMovementState *const movement = &party->embeddedMovement;
    movement->flags |= 0x24U;
    SetCharacterMotionEnabled(character, 1);
    cpamSetFlag(party->character, 1);
    toggleCCMflagsByAmt(character, 0x300000U);
    if (party->name == SHEN_NAME4('S', 'I', 'N', '_')) {
        SetCharacterType(character, 4);
        party->flags |= 0x800U;
    }
    party->routeMarker = 9;
    if (g_currentMap != SHEN_NAME4('K', 'M', 'Z', '1'))
        party->routeMarker = 0;
    if (MatchAreaNamePattern(g_currentMap, SHEN_NAME4('K', 'M', 'Z', '?')))
        party->flags |= 0x40000000U;
    PartyCompanionTransition *const transition = g_pPartyCompanionTransition;
    if (transition->present && party->name == transition->name) {
        party->flags &= ~0x200U;
        SetCharacterWorldPosition(character, &transition->position);
        SetCharacterWorldYaw(character, transition->yaw);
        party->routeIndex = transition->routeIndex;
        party->routeMarker = transition->routeMarker;
        party->mode = transition->mode;
        transition->present = 0;
        party->flags |= 0x20000000U;
        party->character->alpha = 0.0f;
        toggleCCMflagsByAmt(character, 0x200000U);
        if (party->mode == 1)
            party->flags |= 0x200U;
    }
    if (charFindMotionwork(character)) {
        party->motionWork = charFindMotionwork(character);
        movement->motion = party->motionWork;
        ConfigurePartyMotion(*party);
    }
    return 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int32_t ExportPartyCompanionTransition(PartyCompanionTransition &state) {
    std::int32_t present = 0;
    for (const shen_name name : kPartyNames) {
        const PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
        if (!party)
            continue;
        partyTransition.name = name;
        present = 1;
        partyTransition.present = 1;
        partyTransition.position = GetCharacterWorldPosition(party->character);
        partyTransition.yaw = GetAllAngYForNode(&party->character->entity.node);
        partyTransition.routeIndex = party->routeIndex;
        partyTransition.routeMarker = party->routeMarker;
        partyTransition.mode = party->mode;
        break;
    }
    if (!present)
        partyTransition = {};
    state = partyTransition;
    partyTransition = {};
    return present;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (!(SHENMUE_IS_DC && SHENMUE_IS_S2)))
std::int32_t SetPartyConversationPaused(shen_name name, bool paused) {
    PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    if (!party)
        return -1;
    if (paused)
        party->flags |= 0x40U;
    else
        party->flags &= ~0x40U;
    return 0;
}
#endif

} // namespace s2

} // namespace shenmue
