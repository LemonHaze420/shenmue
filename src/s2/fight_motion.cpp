#include "shenmue.h"
#include "common/native_sqrt.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/fight_stage.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/battle_qte_sprites.hpp"
#include <cstring>
#include <cmath>
#include "s2/character_control_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/hlib.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scenario_dc.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/matrix.hpp"
#include <cstdio>
#include "s2/model_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/pause.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/camera_task_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/camera_search_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include <math.h>
#include <float.h>
#include "s2/camera_conversation_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/window.hpp"
#include "s2/fight.hpp"
#include "s2/math.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/conversation_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/conversation_fade_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/npc_data_dc.hpp"
#endif
#else
#include "s2/fight_stage.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/model_component.hpp"
#include "s2/osage.hpp"
#include "s2/motion.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_control.hpp"
#include "s2/character.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/camera.hpp"
#include "s2/save.hpp"
#include "s2/fight.hpp"
#include "s2/sound_program.hpp"
#include "s2/conversation.hpp"
#include "s2/face.hpp"
#include "s2/pak.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/ui.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/legacy_sprite.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native round-result counters: observed results followed by their authored caps.
std::uint8_t g_vfRoundResultCounters[4]{};

namespace {

// Native motnPtr is a shared borrowed cursor, not a second motion-bank owner.
const std::byte *motionMarkerCursor{};

std::uint8_t packedFightSettings[16]{};

constexpr std::uint8_t markerWidths[32]{12, 4, 8, 16, 20, 12, 8, 4, 0, 16, 8, 12, 4, 4, 16, 8, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 4, 8, 8, 8};

constexpr std::uint8_t mirroredMoveIds[48]{0, 2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 13, 15, 14, 16, 17, 18, 19, 20, 21, 23, 22, 25, 24, 27, 26, 28, 29, 31, 30,
    33, 32, 34, 35, 37, 36, 38, 39, 40, 41, 42, 0, 0, 0, 0, 0};

constexpr float impulseTableA[16]{1, 1, 1, 1.2f, 1.4f, 1.6f, 1.8f, 2, 2.1f, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f, 2.7f, 2.8f};

constexpr float impulseTableB[16]{1, 1, 1, 1, 1, 1, 1, 1.2f, 1.4f, 1.6f, 2, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f};

constexpr std::uint16_t cameraSectors[8]{0x20, 0xa0, 0x80, 0x90, 0x10, 0x50, 0x40, 0x60};

// Native g_vfDefaultCameraScripts: ten sector codes followed by ten frame durations per record.
constexpr std::uint8_t defaultCameraScripts[22][20]{{0, 127, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {72, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0}, {136, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {40, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0}, {24, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {19, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {19, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {19, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {7, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {4, 127, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 127, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 127, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {240, 241, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {240, 242, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {240, 243, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {240, 250, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {8, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 10, 0, 0, 0, 0, 0, 0, 0, 0}, {5, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0},
    {8, 0, 1, 127, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0}, {8, 0, 2, 127, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0},
    {8, 0, 3, 127, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0}};

// Native g_vfFightVoiceRows: borrowed native voice rows; 9..12 share the short row.
constexpr std::uint16_t fightVoiceRows[12][17]{{46, 0, 0, 0, 0, 2509, 0, 0, 0, 2613, 2615, 2614, 2614, 2815, 0, 3380, 0},
    {85, 0, 0, 0, 0, 2512, 0, 0, 0, 2614, 2613, 2613, 2613, 2815, 0, 3382, 0}, {43, 0, 0, 0, 0, 2511, 0, 0, 0, 2613, 2615, 2614, 2614, 2815, 0, 3383, 0},
    {44, 0, 0, 0, 0, 2510, 0, 0, 0, 2614, 2613, 2613, 2613, 2815, 0, 3387, 0}, {48, 0, 0, 0, 0, 2513, 0, 0, 0, 2613, 2615, 2614, 2614, 2815, 0, 3388, 0},
    {45, 0, 0, 0, 0, 2515, 0, 0, 0, 2614, 2613, 2613, 2613, 2815, 0, 3386, 0}, {51, 0, 0, 0, 0, 0, 0, 0, 0, 2613, 2615, 2614, 2614, 2815, 0, 0, 0},
    {50, 0, 0, 0, 0, 0, 0, 0, 0, 2614, 2613, 2613, 2613, 2815, 0, 0, 0}, {49, 584, 97, 309, 2343, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50},
    {50, 109, 580, 307, 2348, 2514, 0, 0, 0, 2614, 2613, 2613, 2613, 2814, 0, 3388, 0},
    {53, 582, 576, 2593, 2345, 2519, 2505, 2493, 2553, 2621, 2623, 2625, 2627, 2816, 0, 3393, 3414},
    {53, 581, 577, 2594, 2347, 2520, 2507, 2495, 2550, 2622, 2624, 2626, 2628, 2817, 0, 3394, 3411}};

constexpr std::uint16_t fightDefeatVoiceRow[17]{54, 583, 578, 2595, 2344, 2522, 2508, 2496, 2554, 2622, 2624, 2626, 2628, 2817, 0, 3397, 3413};

const std::uint16_t *const fightVoices[17]{nullptr, fightVoiceRows[0], fightVoiceRows[1], fightVoiceRows[2], fightVoiceRows[3], fightVoiceRows[4],
    fightVoiceRows[5], fightVoiceRows[6], fightVoiceRows[7], fightVoiceRows[8], fightVoiceRows[8], fightVoiceRows[8], fightVoiceRows[8], fightVoiceRows[9],
    fightVoiceRows[10], fightVoiceRows[11], fightDefeatVoiceRow};

std::uint16_t motionFrameEvent{};

constexpr std::uint8_t frameMarkerWidths[57]{0, 4, 4, 16, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4, 4, 8, 8, 8, 4, 8, 8, 8, 16, 12, 28, 4, 8, 8, 4, 8, 8, 8, 8, 8,
    12, 8, 12, 8, 8, 4, 8, 8, 4, 4, 4, 4, 16, 8, 12, 8, 4, 8, 4, 4};

constexpr float guardImpactMultipliers[16]{6, 6.4f, 6.8f, 7.2f, 7.6f, 8, 8.4f, 8.8f, 9.2f, 9.6f, 10, 10.4f, 10.8f, 11.2f, 11.6f, 12};

// Native fight-timer colour scheme byte: read by the timer digits and separators, never written on PC.
std::int8_t timerColourMode{};

constexpr std::uint32_t timerDivisors[6]{1800000, 180000, 30000, 3000, 300, 30};

struct TimerDigitPlace {
    std::uint16_t x, y;
    std::uint8_t size;
};

constexpr TimerDigitPlace timerDigitPlaces[6]{{18, 0, 32}, {46, 0, 32}, {100, 0, 32}, {132, 0, 32}, {178, 8, 16}, {194, 8, 16}};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

float MarkerFixed(std::size_t offset);
void DecrementFrame(std::uint16_t &frame);
void DecrementFrame(std::int16_t &frame);

} // namespace

void vfMotnDispatch(VFActor *self);
void vfApplyMotionDescriptor(VFActor *self);
void vfUpdateMoveVelocityWithKnockback(VFActor *self, float acceleration);
void vfRescaleReactionFrameWindows(VFActor *self);
void vfApplyHitReactionAndStartMotion(VFActor *self);
const std::byte *vfFindQueuedReactionEventEntry(VFActor *self, std::uint8_t eventType);
void vfDispatchQueuedReactionEvent(VFActor *self);
void vfSelectDefaultMotion(VFActor *self, std::uint32_t index);
void vfInitializeActorMotionState(VFActor *self);
void vfResetScriptedFormationMotion(VFActor *self);
void vfSnapActorToContact(VFActor *self);
void vfPredictLandingGround(VFActor *self);
void vfUpdateHPRegeneration(VFActor *self);
float vfclCalculateGuardKnockbackFactor(VFActor *self);
std::uint8_t vfFightVoiceStyle(std::uint16_t style);
void vfPlayFightSfx(std::uint32_t sound, VFActor *self, std::uint8_t mode);
void vfForceActorDefeat(VFActor *self);
void vfStartAuthoredDefeatCamera(VFActor *self);
void vfHandleImpactSfxAndDefeat(VFActor *self, std::int16_t damage, std::uint8_t mode);
bool vfCheckAnimTimeWithin(VFActor *self, float frame);
std::uint8_t vfProcessMotionFrameEvents(VFActor *self);
void vfUpdateReactAnimationFrames(VFActor *self);
void vfUpdateAnimFlagsAndEvents(VFActor *self);
void vfUpdateMotionTargetPointAndParams(VFActor *self);
void vfUpdateRootBlendEnvelope(VFActor *self);
void vfclUpdatePairingFacing(VFActor *self);
void vfUpdateGroundImpactReaction(VFActor *self);
bool vfTryGuardReactionSwap(VFActor *self, VFActor *source);
bool vfIsWithinFightFloor(const Vector3 &position);
void vfUpdateFightFloorReaction(VFActor *self);
void vfActorUpdateState(VFActor *self, VFActor *other);
void vfUpdatePoseWithSfx(VFActor *self);
void vfInitializeFightActor(VFMA_t *game, std::uint8_t index);
void vfDrawFightLoadSprite(std::int32_t index, std::int32_t x, std::int32_t y, float depth, std::uint32_t elapsedFrames);
void ConstructColouredText(std::uint32_t elapsedFrames, std::int32_t x, std::int32_t y, float depth);
void constructNumStrikesLeft();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Both observed counters advance with byte wrapping and are then capped by their authored limits.
// @verified - p
void vfAddRoundResults(std::uint8_t won, std::uint8_t lost) {
    const auto first = static_cast<std::uint8_t>(g_vfRoundResultCounters[0] + won), second = static_cast<std::uint8_t>(g_vfRoundResultCounters[1] + lost);
    g_vfRoundResultCounters[0] = first > g_vfRoundResultCounters[2] ? g_vfRoundResultCounters[2] : first;
    g_vfRoundResultCounters[1] = second > g_vfRoundResultCounters[3] ? g_vfRoundResultCounters[3] : second;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ResetCameraPhase(VFSlotRuntimeState &slot) {
    slot.parameter89 = 0;
    slot.parameter8c = 0;
    slot.parameterCc = 0;
    slot.parameter12a = 0;
    slot.resetIndex = 0x7f7f;
    slot.cameraFlags = (slot.cameraFlags & ~0x2001U) | 1U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);

typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);

struct FightTimerPlace {
    std::uint16_t x, y;
    std::uint8_t size, retained05;
};

typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);

typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);

typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);

typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);

std::int32_t FinishFightMotionMarkers(FightActorRuntimePrefix *actor);

std::int32_t SetFightRequestedAnimation(FightActorRuntimePrefix *actor);

std::int32_t SetFightReactionFrameRange(FightActorRuntimePrefix *actor);

std::int32_t SetFightGroundMotionWindow(FightActorRuntimePrefix *actor);

std::int32_t SetFightAttackParameters(FightActorRuntimePrefix *actor);

std::int32_t SetFightContactAttack(FightActorRuntimePrefix *actor);

std::int32_t SetFightFacingWindow(FightActorRuntimePrefix *actor);

std::int32_t SetFightFacingDelta(FightActorRuntimePrefix *actor);

std::int32_t BeginFightReactionProgram(FightActorRuntimePrefix *actor);

std::int32_t SetFightMotionImpulse(FightActorRuntimePrefix *actor);

std::int32_t AppendFightReactionEvent(FightActorRuntimePrefix *actor);

std::int32_t SetFightReactionEventFlags(FightActorRuntimePrefix *actor);

std::int32_t SetFightContactFrames(FightActorRuntimePrefix *actor);

std::int32_t SetFightPairContactFrames(FightActorRuntimePrefix *actor);

std::int32_t ScaleFightAttackStrength(FightActorRuntimePrefix *actor);

std::int32_t SetFightPairReactionParameters(FightActorRuntimePrefix *actor);

std::int32_t ToggleFightMotionFacing(FightActorRuntimePrefix *actor);

std::int32_t SetFightPartnerAttack(FightActorRuntimePrefix *actor);

std::int32_t SetFightKnockbackScale(FightActorRuntimePrefix *actor);

std::int32_t SetFightExtraAnimation(FightActorRuntimePrefix *actor);

std::int32_t SetFightFacingAnimation(FightActorRuntimePrefix *actor);

std::int32_t SetFightMotionMarkerMask(FightActorRuntimePrefix *actor);

std::int32_t SetFightSavedImpulse(FightActorRuntimePrefix *actor);

std::int32_t FinishFightFrameEvents(FightActorRuntimePrefix *actor);

std::int32_t MirrorFightFrameReaction(FightActorRuntimePrefix *actor);

std::int32_t BeginFightFrameRecovery(FightActorRuntimePrefix *actor);

std::int32_t ApplyFightFrameImpulse(FightActorRuntimePrefix *actor);

std::int32_t PlayFightFrameCameraSound(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameHitFlags(FightActorRuntimePrefix *actor);

std::int32_t MaskFightFrameHitFlags(FightActorRuntimePrefix *actor);

std::int32_t ToggleFightFrameHitFlags(FightActorRuntimePrefix *actor);

std::int32_t ApplyFightFrameGuardDamage(FightActorRuntimePrefix *actor);

std::int32_t PlayFightFrameVoice(FightActorRuntimePrefix *actor);

std::int32_t UpdateFightFrameDeadStatus(FightActorRuntimePrefix *);

std::int32_t QueueFightFrameRandomReaction(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameAxisAngle(FightActorRuntimePrefix *actor);

std::int32_t ShakeFightFrameCamera(FightActorRuntimePrefix *actor);

std::int32_t BeginFightFrameCamera(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameExpression(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameHold(FightActorRuntimePrefix *actor);

std::int32_t ClearFightFrameContinuation(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameFacingStep(FightActorRuntimePrefix *actor);

std::int32_t QueueFightFrameContactReaction(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameContactAction(FightActorRuntimePrefix *actor);

std::int32_t QueueFightFrameBufferedReaction(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameActiveFlags(FightActorRuntimePrefix *actor);

std::int32_t MaskFightFrameActiveFlags(FightActorRuntimePrefix *actor);

std::int32_t PlayFightFrameSound(FightActorRuntimePrefix *actor);

std::int32_t EmitFightFrameSmoke(FightActorRuntimePrefix *actor);

std::int32_t AttachFightFrameStageObject(FightActorRuntimePrefix *actor);

std::int32_t ReleaseFightFrameStageObject(FightActorRuntimePrefix *actor);

std::int32_t LaunchFightFrameStageObject(FightActorRuntimePrefix *actor);

std::int32_t PlayFightFrameDialogue(FightActorRuntimePrefix *actor);

std::int32_t SetFightFrameObjectEffect(FightActorRuntimePrefix *actor);

std::int32_t ToggleFightFrameHair(FightActorRuntimePrefix *actor);

std::int32_t PlayFightFrameGroundSound(FightActorRuntimePrefix *actor);

} // namespace

std::int32_t vfPrepareAuthoredDefeatCamera();

bool vfDidCrossFightFrame(FightActorRuntimePrefix *actor, float frame);

void vfActorUpdateState(FightActorRuntimePrefix *actor, FightActorRuntimePrefix *other);

void vfApplyKnockbackPhysics(FightActorRuntimePrefix *actor);

void vfclUpdatePairingFacing(FightActorRuntimePrefix *actor);

void vfUpdatePositionXZ(FightActorRuntimePrefix *actor);

void vfReleaseUnheldScriptedFormation(FightRuntimePrefix *game);

std::uint8_t vfFightVoiceStyle(std::uint32_t style);

void vfHandleImpactSfxAndDefeat(FightActorRuntimePrefix *actor, std::int16_t damage, std::uint8_t mode);

void vfUpdateFightFloorReaction(FightActorRuntimePrefix *actor);

void vfUpdateAnimFlagsAndEvents(FightActorRuntimePrefix *actor);

void vfUpdateReactAnimationFrames(FightActorRuntimePrefix *actor);

void PrepareFightFieldNode(CharacterNode *);

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);

void vfAttachActorStageObject(FightActorRuntimePrefix *actor);

void vfReleaseActorStageObject(FightActorRuntimePrefix *actor);

void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor);

void vfLaunchActorStageObject(FightActorRuntimePrefix *actor);

void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames);

void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction);

float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);

float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);

std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor);

void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);

void vfResetActorActionState(FightActorRuntimePrefix *actor);

float flddFindIntersectionBy(const Vector3 &position);

void PrepareFightFieldNode(CharacterNode *);

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);

void vfAttachActorStageObject(FightActorRuntimePrefix *actor);

void vfReleaseActorStageObject(FightActorRuntimePrefix *actor);

void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor);

void vfLaunchActorStageObject(FightActorRuntimePrefix *actor);

void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames);

void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction);

float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);

float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);

std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor);

void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);

void vfResetActorActionState(FightActorRuntimePrefix *actor);

float flddFindIntersectionBy(const Vector3 &position);

void PrepareFightFieldNode(CharacterNode *);

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);

void vfAttachActorStageObject(FightActorRuntimePrefix *actor);

void vfReleaseActorStageObject(FightActorRuntimePrefix *actor);

void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor);

void vfLaunchActorStageObject(FightActorRuntimePrefix *actor);

void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames);

void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction);

float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);

float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);

std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor);

void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);

void vfResetActorActionState(FightActorRuntimePrefix *actor);

float flddFindIntersectionBy(const Vector3 &position);

void camCtrlSetInterpModeTo(std::uint8_t mode);

bool IsEarlyAuthFightCamera();

bool IsAuthFightCamera();

void vfSetupLinkedFightCamera(FightActorRuntimePrefix *actor, std::uint8_t kind);

void InitializeCameraGoal();

std::uint8_t camCtrlGetInterpMode();

std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);

void PrepareFightFieldNode(CharacterNode *);

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);

void vfAttachActorStageObject(FightActorRuntimePrefix *actor);

void vfReleaseActorStageObject(FightActorRuntimePrefix *actor);

void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor);

void vfLaunchActorStageObject(FightActorRuntimePrefix *actor);

void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames);

void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction);

float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);

float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);

std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor);

void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);

void vfResetActorActionState(FightActorRuntimePrefix *actor);

float flddFindIntersectionBy(const Vector3 &position);

std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);

std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);

void PrepareFightFieldNode(CharacterNode *);

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);

void vfAttachActorStageObject(FightActorRuntimePrefix *actor);

void vfReleaseActorStageObject(FightActorRuntimePrefix *actor);

void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor);

void vfLaunchActorStageObject(FightActorRuntimePrefix *actor);

void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames);

void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction);

float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);

float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);

std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor);

void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);

void vfResetActorActionState(FightActorRuntimePrefix *actor);

float flddFindIntersectionBy(const Vector3 &position);

static float FightCameraLength(const Vector3 &value);

static void ScaleFightCameraVector(Vector3 &value, float length);

static bool FightCameraRaycast(Vector3 &result, const Vector3 &destination, const Vector3 &origin);

static void ResetFightCameraPattern();

void resetIntersectionAndTest();

void InitializeCameraGoal();

static void MoveFightCameraToward(const Vector3 &desired, Vector3 &retained, Vector3 &direction);

void camCtrlEvaluateGoal();

static std::int32_t CameraAbsInteger(std::int32_t value);

template <class T> static T CameraClamp(T value, T minimum, T maximum);

template <class T> static T CameraMin(T first, T second);

template <class T> static T CameraMax(T first, T second);

static Vector3 CameraVector(float x, float y, float z);

static void ClampFightCameraPoint(Vector3 &point, const Vector3 &centre);

static Vector3 RotateFightCameraPoint(const Vector3 &relative, const Vector3 &centre, std::uint16_t yaw);

static Vector3 FightCameraProbeFor(const Vector3 &target, float y, std::uint16_t angle);

void AccumulateFightCameraVelocity(FightRuntimePrefix *game, FightActorRuntimePrefix *self);

void UpdateFightCameraPattern(FightRuntimePrefix *game);

void UpdateEnvironmentFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateFieldFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateFirstPersonFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

std::uint8_t camCtrlGetInterpMode();

void BeginMotionUpdateCamera(Character *target);

void ResetMotionCameraWaypoints();

void AppendMotionCameraWaypoint(const Vector3 &position);

void BeginMotionInterpolationCamera(Character *target);

void camCtrlSetFightDistance(float distance);

void camCtrlSetFightHeight(float height);

void camCtrlSetInterpModeTo(std::uint8_t mode);

bool IsEarlyAuthFightCamera();

bool IsAuthFightCamera();

void SetFightCameraLinkedKind(std::uint8_t kind);

void vfSetupLinkedFightCamera(FightActorRuntimePrefix *actor, std::uint8_t kind);

void vfUnknownCamRelated(FightRuntimePrefix *game);

void ResetFightCameraToParticipants(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateQuadraticFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

static void PublishFightCameraPlayerAlpha(Character *player, const Vector3 &bone);

void UpdateMotionFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateMotionInterpolationFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *, FightActorRuntimePrefix *);

static std::int16_t CameraLerpAngle(std::int16_t target, std::int16_t base, float amount);

static std::int16_t CameraTransitionAngle(std::int16_t base, std::int16_t target, float amount);

static void camCtrlRetainPreviousPose(CameraRuntime &camera);

static void camCtrlResetDefaultState(CameraRuntime &camera);

static void camCtrlComputePreviousPositionNullsub(CameraRuntime &);

static void camCtrlComputeOrbitPosition(CameraRuntime &camera);

static void camCtrlComputeLookAtPosition(CameraRuntime &camera);

static void camCtrlComputeAnchorPosition(CameraRuntime &camera);

void camCtrlApply(CameraRuntime &camera);

void UpdateCameraTransitions(CameraRuntime &camera);

void camCtrlInterpolateNextPositionRotation(CameraRuntime &camera);

void camCtrlApplyJitterAdjustment(CameraRuntime &camera);

float GenerateOscillationForCamera(std::int32_t phase);

void camCtrlSmoothMotion(CameraRuntime &camera);

void ApplyRotationMatrixFromTransform(CameraRuntime &camera);

void camCtrlCheckCollision(CameraRuntime &camera);

void camCtrlComputeNextPosition(CameraRuntime &camera);

void camCtrlRecalcFov(CameraRuntime &camera);

void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera);

void camCtrlSetup(CameraRuntime &camera);

static void CameraSplineProjectionNullsub();

static void CameraMatrixProjectionNullsub();

static void ConversationCameraProjectionNullsub();

static void CameraProjectionPostCallback(CameraRuntime &);

static void camCtrlProjectCameraMatrices(void *);

static void camCtrlSetFloats(CameraRuntime &camera, float position, float angle);

static Vector3 plymComputeConvTargetAnchor();

static void CameraScaleTo(Vector3 &value, float length);

static float CameraDistance(const Vector3 &first, const Vector3 &second);

std::int32_t camCtrlGetIdealCameraPosition(CameraRuntime &camera);

std::int32_t camCtrlComputeOrbitCameraFromTarget(const Vector3 *target, std::int16_t yaw, Vector3 *position, Vector3 *lookAt);

static void camCtrlInterpolateApplyCameraAngY(CameraRuntime &camera);

static void camCtrlUpdateFollowCamera(CameraRuntime &camera);

static void camCtrlAutoCenterTargetInView(CameraRuntime &camera);

void camCtrlSwitchColiPhase(CameraRuntime &camera);

void camCtrlSetCamSnapAngY(CameraRuntime &camera);

static float ClampCameraValue(float value, float minimum, float maximum);

void camCtrlHandleLookAroundCam(CameraRuntime &camera);

static float SearchCameraEase(const SearchCameraState &state);

static void ResetLockedOnCameraPosition(CameraRuntime &camera, SearchCameraState &state);

void UpdateLockedOnCamera();

void camCtrlHandleStates(CameraRuntime &camera);

void camCtrlCallback(void *);

static float FightCameraLength(const Vector3 &value);

static void ScaleFightCameraVector(Vector3 &value, float length);

static bool FightCameraRaycast(Vector3 &result, const Vector3 &destination, const Vector3 &origin);

static void ResetFightCameraPattern();

void resetIntersectionAndTest();

void InitializeCameraGoal();

static void MoveFightCameraToward(const Vector3 &desired, Vector3 &retained, Vector3 &direction);

void camCtrlEvaluateGoal();

static std::int32_t CameraAbsInteger(std::int32_t value);

template <class T> static T CameraClamp(T value, T minimum, T maximum);

template <class T> static T CameraMin(T first, T second);

template <class T> static T CameraMax(T first, T second);

static Vector3 CameraVector(float x, float y, float z);

static void ClampFightCameraPoint(Vector3 &point, const Vector3 &centre);

static Vector3 RotateFightCameraPoint(const Vector3 &relative, const Vector3 &centre, std::uint16_t yaw);

static Vector3 FightCameraProbeFor(const Vector3 &target, float y, std::uint16_t angle);

void AccumulateFightCameraVelocity(FightRuntimePrefix *game, FightActorRuntimePrefix *self);

void UpdateFightCameraPattern(FightRuntimePrefix *game);

void UpdateEnvironmentFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateFieldFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateFirstPersonFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

std::uint8_t camCtrlGetInterpMode();

void BeginMotionUpdateCamera(Character *target);

void ResetMotionCameraWaypoints();

void AppendMotionCameraWaypoint(const Vector3 &position);

void BeginMotionInterpolationCamera(Character *target);

void camCtrlSetFightDistance(float distance);

void camCtrlSetFightHeight(float height);

void camCtrlSetInterpModeTo(std::uint8_t mode);

bool IsEarlyAuthFightCamera();

bool IsAuthFightCamera();

void SetFightCameraLinkedKind(std::uint8_t kind);

void vfSetupLinkedFightCamera(FightActorRuntimePrefix *actor, std::uint8_t kind);

void vfUnknownCamRelated(FightRuntimePrefix *game);

void ResetFightCameraToParticipants(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateQuadraticFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

static void PublishFightCameraPlayerAlpha(Character *player, const Vector3 &bone);

void UpdateMotionFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);

void UpdateMotionInterpolationFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *, FightActorRuntimePrefix *);

static std::int16_t CameraLerpAngle(std::int16_t target, std::int16_t base, float amount);

static std::int16_t CameraTransitionAngle(std::int16_t base, std::int16_t target, float amount);

static void camCtrlRetainPreviousPose(CameraRuntime &camera);

static void camCtrlResetDefaultState(CameraRuntime &camera);

static void camCtrlComputePreviousPositionNullsub(CameraRuntime &);

static void camCtrlComputeOrbitPosition(CameraRuntime &camera);

static void camCtrlComputeLookAtPosition(CameraRuntime &camera);

static void camCtrlComputeAnchorPosition(CameraRuntime &camera);

void camCtrlApply(CameraRuntime &camera);

void UpdateCameraTransitions(CameraRuntime &camera);

void camCtrlInterpolateNextPositionRotation(CameraRuntime &camera);

void camCtrlApplyJitterAdjustment(CameraRuntime &camera);

float GenerateOscillationForCamera(std::int32_t phase);

void camCtrlSmoothMotion(CameraRuntime &camera);

void ApplyRotationMatrixFromTransform(CameraRuntime &camera);

void camCtrlCheckCollision(CameraRuntime &camera);

void camCtrlComputeNextPosition(CameraRuntime &camera);

void camCtrlRecalcFov(CameraRuntime &camera);

void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera);

void camCtrlSetup(CameraRuntime &camera);

static void CameraSplineProjectionNullsub();

static void CameraMatrixProjectionNullsub();

static void ConversationCameraProjectionNullsub();

static void CameraProjectionPostCallback(CameraRuntime &);

static void camCtrlProjectCameraMatrices(void *);

static void camCtrlSetFloats(CameraRuntime &camera, float position, float angle);

static Vector3 plymComputeConvTargetAnchor();

static void CameraScaleTo(Vector3 &value, float length);

static float CameraDistance(const Vector3 &first, const Vector3 &second);

std::int32_t camCtrlGetIdealCameraPosition(CameraRuntime &camera);

std::int32_t camCtrlComputeOrbitCameraFromTarget(const Vector3 *target, std::int16_t yaw, Vector3 *position, Vector3 *lookAt);

static void camCtrlInterpolateApplyCameraAngY(CameraRuntime &camera);

static void camCtrlUpdateFollowCamera(CameraRuntime &camera);

static void camCtrlAutoCenterTargetInView(CameraRuntime &camera);

void camCtrlSwitchColiPhase(CameraRuntime &camera);

void camCtrlSetCamSnapAngY(CameraRuntime &camera);

static float ClampCameraValue(float value, float minimum, float maximum);

void camCtrlHandleLookAroundCam(CameraRuntime &camera);

static float SearchCameraEase(const SearchCameraState &state);

static void ResetLockedOnCameraPosition(CameraRuntime &camera, SearchCameraState &state);

void UpdateLockedOnCamera();

void camCtrlHandleStates(CameraRuntime &camera);

void camCtrlCallback(void *);

void PrepareFightFieldNode(CharacterNode *);

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);

void vfAttachActorStageObject(FightActorRuntimePrefix *actor);

void vfReleaseActorStageObject(FightActorRuntimePrefix *actor);

void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor);

void vfLaunchActorStageObject(FightActorRuntimePrefix *actor);

void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames);

void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction);

float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);

float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);

std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor);

void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);

void vfResetActorActionState(FightActorRuntimePrefix *actor);

float flddFindIntersectionBy(const Vector3 &position);

void vfInitSfxSlotState(FightRuntimePrefix *game);

namespace {

static const std::uint16_t fightEventVoiceBank0[15] = {46, 0, 0, 0, 0, 0, 0, 0, 0, 2613, 2615, 2617, 2619, 2815, 0};

static const std::uint16_t fightEventVoiceBank1[15] = {85, 0, 0, 0, 0, 0, 0, 0, 0, 2614, 2616, 2618, 2620, 2815, 0};

static const std::uint16_t fightEventVoiceBank2[15] = {43, 0, 0, 0, 0, 0, 0, 0, 0, 2613, 2615, 2617, 2619, 2815, 0};

static const std::uint16_t fightEventVoiceBank3[15] = {44, 0, 0, 0, 0, 0, 0, 0, 0, 2614, 2616, 2618, 2620, 2815, 0};

static const std::uint16_t fightEventVoiceBank4[15] = {48, 0, 0, 0, 0, 0, 0, 0, 0, 2613, 2615, 2617, 2619, 2815, 0};

static const std::uint16_t fightEventVoiceBank5[15] = {45, 0, 0, 0, 0, 0, 0, 0, 0, 2614, 2616, 2618, 2620, 2815, 0};

static const std::uint16_t fightEventVoiceBank6[15] = {51, 0, 0, 0, 0, 0, 0, 0, 0, 2613, 2615, 2617, 2619, 2815, 0};

static const std::uint16_t fightEventVoiceBank7[15] = {50, 0, 0, 0, 0, 0, 0, 0, 0, 2614, 2616, 2618, 2620, 2815, 0};

static const std::uint16_t fightEventVoiceBank8[14] = {49, 584, 97, 309, 2343, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const std::uint16_t fightEventVoiceBank9[15] = {50, 109, 580, 307, 2348, 0, 0, 0, 0, 2614, 2616, 2618, 2620, 2814, 0};

static const std::uint16_t fightEventVoiceBank10[15] = {53, 582, 576, 2593, 2345, 2519, 2505, 2493, 2553, 2621, 2623, 2625, 2627, 2816, 0};

static const std::uint16_t fightEventVoiceBank11[15] = {53, 581, 577, 2594, 2347, 2520, 2507, 2495, 2550, 2622, 2624, 2626, 2628, 2817, 0};

static const std::uint16_t fightEventVoiceBank12[15] = {54, 583, 578, 2595, 2344, 2522, 2508, 2496, 2554, 2622, 2624, 2626, 2628, 2817, 0};

static const std::uint16_t *const fightEventVoices[17] = {0, fightEventVoiceBank0, fightEventVoiceBank1, fightEventVoiceBank2, fightEventVoiceBank3,
    fightEventVoiceBank4, fightEventVoiceBank5, fightEventVoiceBank6, fightEventVoiceBank7, fightEventVoiceBank8, fightEventVoiceBank8, fightEventVoiceBank8,
    fightEventVoiceBank8, fightEventVoiceBank9, fightEventVoiceBank10, fightEventVoiceBank11, fightEventVoiceBank12};

static const std::uint8_t mirroredMoveIds[44] = {0, 2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 13, 15, 14, 16, 17, 18, 19, 20, 21, 23, 22, 25, 24, 27, 26, 28, 29,
    31, 30, 33, 32, 34, 35, 37, 36, 38, 39, 40, 41, 42, 0};

static const float impulseTableA[16] = {1, 1, 1, 1.2f, 1.4f, 1.6f, 1.8f, 2, 2.1f, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f, 2.7f, 2.8f};

static const float impulseTableB[16] = {1, 1, 1, 1, 1, 1, 1, 1.2f, 1.4f, 1.6f, 2, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f};

static const FightMarkerHandler fightMarkerHandlers[32] = {FinishFightMotionMarkers, SetFightRequestedAnimation, SetFightReactionFrameRange,
    SetFightGroundMotionWindow, SetFightAttackParameters, SetFightContactAttack, SetFightFacingWindow, SetFightFacingDelta, BeginFightReactionProgram,
    SetFightMotionImpulse, AppendFightReactionEvent, SetFightReactionEventFlags, SetFightContactFrames, SetFightPairContactFrames, ScaleFightAttackStrength,
    SetFightPairReactionParameters, 0, 0, 0, ToggleFightMotionFacing, 0, 0, 0, 0, 0, 0, SetFightPartnerAttack, SetFightKnockbackScale, SetFightExtraAnimation,
    SetFightFacingAnimation, SetFightMotionMarkerMask, SetFightSavedImpulse};

const std::byte *motionFrameCursor;

std::uint16_t motionFrameEvent;

const FightFrameCallback fightFrameCallbacks[56] = {FinishFightFrameEvents, MirrorFightFrameReaction, BeginFightFrameRecovery, ApplyFightFrameImpulse,
    PlayFightFrameCameraSound, SetFightFrameHitFlags, MaskFightFrameHitFlags, ToggleFightFrameHitFlags, ApplyFightFrameGuardDamage, PlayFightFrameVoice,
    UpdateFightFrameDeadStatus, QueueFightFrameRandomReaction, 0, SetFightFrameAxisAngle, ShakeFightFrameCamera, BeginFightFrameCamera, SetFightFrameExpression,
    0, 0, SetFightFrameHold, ClearFightFrameContinuation, 0, SetFightFrameFacingStep, QueueFightFrameContactReaction, 0, SetFightFrameContactAction,
    QueueFightFrameBufferedReaction, 0, 0, SetFightFrameActiveFlags, MaskFightFrameActiveFlags, 0, PlayFightFrameSound, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    EmitFightFrameSmoke, AttachFightFrameStageObject, ReleaseFightFrameStageObject, LaunchFightFrameStageObject, 0, PlayFightFrameDialogue,
    SetFightFrameObjectEffect, 0, ToggleFightFrameHair, PlayFightFrameGroundSound, reinterpret_cast<FightFrameCallback>(vfPrepareAuthoredDefeatCamera)};

} // namespace

const std::uint8_t motionCommandWidths[32] = {12, 4, 8, 16, 20, 12, 8, 4, 0, 16, 8, 12, 4, 4, 16, 8, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 4, 8, 8, 8};
const std::uint8_t motionFrameCommandWidths[] = {0, 4, 4, 16, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4, 4, 8, 8, 8, 4, 8, 8, 8, 16, 12, 28, 4, 8, 8, 4, 8, 8, 8, 8,
    8, 12, 8, 12, 8, 8, 4, 8, 8, 4, 4, 4, 4, 16, 8, 12, 8, 4, 8, 4, 2, 0};

void vfRefreshContactReactionState(FightActorRuntimePrefix *actor);

void vfInitializeActorCollision(FightRuntimePrefix *game, std::uint8_t index);

namespace {

std::int8_t fightTimerColourMode;

static const std::uint32_t fightTimerDivisors[6] = {1800000, 180000, 30000, 3000, 300, 30};

static const FightTimerPlace fightTimerPlaces[6] = {{18, 0, 32, 0}, {46, 0, 32, 0}, {100, 0, 32, 0}, {132, 0, 32, 0}, {178, 8, 16, 0}, {194, 8, 16, 0}};

} // namespace

void vfDispatchCurrentActionState(FightActorRuntimePrefix *actor);

void vfUpdateDynamicActionState(FightActorRuntimePrefix *actor);

std::int32_t camCtrlSetInterpolationMode(std::int32_t mode);

void camCtrlSetInterpolationSpeed(float speed);

void camCtrlSetInterpolationFrameCount(std::int32_t frames);

std::int32_t camCtrlSetState(std::int32_t state);

void camCtrlSetNextPosition(const Vector3 &position);

void camCtrlSetTargetPosition(const Vector3 &position);

Vector3s *camCtrlGetRot(Vector3s *rotation);

std::int32_t camCtrlSetPerspective(std::int16_t perspective);

std::int16_t camCtrlGetPerspective();

float camCtrlSetMotionScaleFactor(float factor);

void camCtrlSetFovTransitionTo(std::int32_t enabled);

void camCtrlCalculateFov(CameraRuntime &camera);

Vector3 *camCtrlGetNextCamPos(Vector3 *position);

Vector3 *camCtrlGetTargetPosition(Vector3 *position);

void InitializeCameraGoal();

std::int32_t camCtrlSetState(std::int32_t state);

void camCtrlSetFovTransitionTo(std::int32_t enabled);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define MOTION_VALUE(T, actor, offset) (*reinterpret_cast<T *>(reinterpret_cast<std::byte *>(actor) + (offset)))

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define MARKER_VALUE(T, offset) (*reinterpret_cast<const T *>(motionFrameCursor + (offset)))
#define MARKER_WORD(offset)                                                                                                                                    \
    static_cast<std::uint16_t>(static_cast<std::uint32_t>(motionFrameCursor[(offset) + 1]) | static_cast<std::uint32_t>(motionFrameCursor[offset]) << 8U)
#define MARKER_FIXED(offset)                                                                                                                                   \
    (static_cast<float>(static_cast<std::int32_t>(MARKER_WORD((offset) + 2) | static_cast<std::uint32_t>(MARKER_WORD(offset)) << 16U)) * 0.000015258789f)

} // namespace

#else

namespace {

template <class T> T MarkerValue(std::size_t offset) {
    T value;
    std::memcpy(&value, motionMarkerCursor + offset, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t AuthoredOffset(const std::byte *data) {
    const auto *p = reinterpret_cast<const std::uint8_t *>(data);
    const std::uint32_t bits = (std::uint32_t(p[0]) << 24) | (std::uint32_t(p[1]) << 16) | (std::uint32_t(p[2]) << 8) | p[3];
    return std::bit_cast<std::int32_t>(bits);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float AuthoredFixed(const std::byte *data) {
    return static_cast<float>(AuthoredOffset(data)) * 0.000015258789f;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline float MarkerFixed(std::size_t offset) {
    const std::byte *const data = motionFrameCursor + offset;
    return static_cast<float>(
               static_cast<std::int32_t>(static_cast<std::uint16_t>(static_cast<std::uint32_t>(data[3]) | static_cast<std::uint32_t>(data[2]) << 8U) |
                   static_cast<std::uint32_t>(static_cast<std::uint16_t>(static_cast<std::uint32_t>(data[1]) | static_cast<std::uint32_t>(data[0]) << 8U))
                       << 16U)) *
        0.000015258789f;
}

} // namespace

#else

namespace {

float MarkerFixed(std::size_t offset) {
    return AuthoredFixed(motionMarkerCursor + offset);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint16_t SignedByteWord(std::size_t offset) {
    return static_cast<std::uint16_t>(MarkerValue<std::int8_t>(offset));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void DecrementFrame(std::uint16_t &frame) {
    if (frame)
        --frame;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void DecrementFrame(std::int16_t &frame) {
    if (frame)
        frame = static_cast<std::int16_t>(static_cast<std::uint16_t>(frame) - 1);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

template <class T> T ActorValue(const VFActor *actor, std::size_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(actor) + offset, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

template <class T> void SetActorValue(VFActor *actor, std::size_t offset, T value) {
    std::memcpy(reinterpret_cast<std::byte *>(actor) + offset, &value, sizeof(value));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// The low six bits select a byte; 0x40/0x80 select its nibbles.
// @verified - p
std::uint8_t vfMotnNibbleGet(std::int8_t code) {
    const auto bits = static_cast<std::uint8_t>(code);
    const auto index = bits & 0x3f;
    if (index >= 16)
        return 0;
    const auto value = packedFightSettings[index];
    if (bits & 0x40)
        return value & 0xf;
    if (code < 0)
        return value >> 4;
    return value;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Authored values are truncated, not clamped to a nibble.
// @verified - p
std::uint8_t vfMotnNibbleSet(std::int8_t code, std::int32_t value) {
    const auto bits = static_cast<std::uint8_t>(code);
    const auto index = static_cast<std::uint8_t>(bits & 0x3f);
    if (index < 16) {
        const auto byte = value < 0 ? std::uint8_t{} : static_cast<std::uint8_t>(value);
        auto &destination = packedFightSettings[index];
        if (bits & 0x40)
            destination = (destination & 0xf0) | byte;
        else if (code >= 0)
            destination = byte;
        else
            destination = (destination & 0xf) | static_cast<std::uint8_t>(16 * byte);
    }
    return index;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline std::uint16_t MarkerSignedByte(std::size_t offset) {
    return static_cast<std::uint16_t>(MARKER_VALUE(std::int8_t, offset));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void VmResetSlots(FightActorRuntimePrefix *self);

namespace {

float MarkerFixed(std::size_t offset);
std::uint16_t MarkerSignedByte(std::size_t offset);
std::int32_t FinishFightMotionMarkers(FightActorRuntimePrefix *actor);
std::int32_t SetFightRequestedAnimation(FightActorRuntimePrefix *actor);
std::int32_t SetFightReactionFrameRange(FightActorRuntimePrefix *actor);
std::int32_t SetFightGroundMotionWindow(FightActorRuntimePrefix *actor);
std::int32_t SetFightAttackParameters(FightActorRuntimePrefix *actor);
std::int32_t SetFightContactAttack(FightActorRuntimePrefix *actor);
std::int32_t SetFightFacingWindow(FightActorRuntimePrefix *actor);
std::int32_t SetFightFacingDelta(FightActorRuntimePrefix *actor);
std::int32_t BeginFightReactionProgram(FightActorRuntimePrefix *actor);
std::int32_t SetFightMotionImpulse(FightActorRuntimePrefix *actor);
std::int32_t AppendFightReactionEvent(FightActorRuntimePrefix *actor);
std::int32_t SetFightReactionEventFlags(FightActorRuntimePrefix *actor);
std::int32_t SetFightContactFrames(FightActorRuntimePrefix *actor);
std::int32_t SetFightPairContactFrames(FightActorRuntimePrefix *actor);
float ClampFightMotionParameter(float minimum, float maximum, float value);
std::int32_t ScaleFightAttackStrength(FightActorRuntimePrefix *actor);
std::int32_t SetFightPairReactionParameters(FightActorRuntimePrefix *actor);
std::int32_t ToggleFightMotionFacing(FightActorRuntimePrefix *actor);
std::int32_t SetFightPartnerAttack(FightActorRuntimePrefix *actor);
std::int32_t SetFightKnockbackScale(FightActorRuntimePrefix *actor);
std::int32_t SetFightExtraAnimation(FightActorRuntimePrefix *actor);
std::int32_t SetFightFacingAnimation(FightActorRuntimePrefix *actor);
std::int32_t SetFightMotionMarkerMask(FightActorRuntimePrefix *actor);
std::int32_t SetFightSavedImpulse(FightActorRuntimePrefix *actor);
#define FRAME_VALUE(T, offset) (*reinterpret_cast<const T *>(motionFrameCursor + (offset)))
#define FIXED_WORD(data) static_cast<std::uint16_t>(reinterpret_cast<const std::uint8_t *>(data)[1] | reinterpret_cast<const std::uint8_t *>(data)[0] << 8)
#define FIXED_VALUE(data) (static_cast<float>(static_cast<std::int32_t>(FIXED_WORD((data) + 2) | FIXED_WORD(data) << 16)) * 0.000015258789f)
#define FRAME_FIXED(offset) FIXED_VALUE(motionFrameCursor + (offset))
std::int32_t FinishFightFrameEvents(FightActorRuntimePrefix *actor);
std::int32_t MirrorFightFrameReaction(FightActorRuntimePrefix *actor);
std::int32_t BeginFightFrameRecovery(FightActorRuntimePrefix *actor);
std::int32_t ApplyFightFrameImpulse(FightActorRuntimePrefix *actor);
std::int32_t PlayFightFrameCameraSound(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameHitFlags(FightActorRuntimePrefix *actor);
std::int32_t MaskFightFrameHitFlags(FightActorRuntimePrefix *actor);
std::int32_t ToggleFightFrameHitFlags(FightActorRuntimePrefix *actor);
std::int32_t ApplyFightFrameGuardDamage(FightActorRuntimePrefix *actor);
std::int32_t PlayFightFrameVoice(FightActorRuntimePrefix *actor);
std::int32_t UpdateFightFrameDeadStatus(FightActorRuntimePrefix *);
std::int32_t QueueFightFrameRandomReaction(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameAxisAngle(FightActorRuntimePrefix *actor);
std::int32_t ShakeFightFrameCamera(FightActorRuntimePrefix *actor);
std::int32_t BeginFightFrameCamera(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameExpression(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameHold(FightActorRuntimePrefix *actor);
std::int32_t ClearFightFrameContinuation(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameFacingStep(FightActorRuntimePrefix *actor);
std::int32_t QueueFightFrameContactReaction(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameContactAction(FightActorRuntimePrefix *actor);
std::int32_t QueueFightFrameBufferedReaction(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameActiveFlags(FightActorRuntimePrefix *actor);
std::int32_t MaskFightFrameActiveFlags(FightActorRuntimePrefix *actor);
std::int32_t PlayFightFrameSound(FightActorRuntimePrefix *actor);
std::int32_t EmitFightFrameSmoke(FightActorRuntimePrefix *actor);
std::int32_t AttachFightFrameStageObject(FightActorRuntimePrefix *actor);
std::int32_t ReleaseFightFrameStageObject(FightActorRuntimePrefix *actor);
std::int32_t LaunchFightFrameStageObject(FightActorRuntimePrefix *actor);
std::int32_t PlayFightFrameDialogue(FightActorRuntimePrefix *actor);
std::int32_t SetFightFrameObjectEffect(FightActorRuntimePrefix *actor);
std::int32_t ToggleFightFrameHair(FightActorRuntimePrefix *actor);
std::int32_t PlayFightFrameGroundSound(FightActorRuntimePrefix *actor);
std::int32_t AuthoredMotionWord(const std::byte *data);
float AuthoredMotionFixed(const std::byte *data);
std::int32_t vfFindNearestFightSpawn(FightActorRuntimePrefix *actor);
void vfApplyScriptedFormationMotion(FightActorRuntimePrefix *actor);
void InitializeFightActorFromFormation(FightRuntimePrefix *game, std::uint8_t index);
void vfResetFightActorInputState(FightActorRuntimePrefix *actor);
std::int32_t FindFightFloorWinner(FightRuntimePrefix *game);
void BeginFightFloorDefeat(FightActorRuntimePrefix *actor);
void UpdateFightTransitionFrames(FightActorRuntimePrefix *actor);
void UpdateFightLeadFoot(FightActorRuntimePrefix *actor);

} // namespace

void vfMotnDispatch(FightActorRuntimePrefix *actor);
void vfApplyMotionDescriptor(FightActorRuntimePrefix *actor);
void vfUpdateMoveVelocityWithKnockback(FightActorRuntimePrefix *actor, float scale);
void vfRescaleReactionFrameWindows(FightActorRuntimePrefix *actor);
static void DecrementFightFrame(std::uint16_t *frame);
void vfApplyHitReactionAndStartMotion(FightActorRuntimePrefix *actor);
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index);
void vfStartFormationAnimation(FightActorRuntimePrefix *actor);
void vfStartReactionAnimation(FightActorRuntimePrefix *actor);
void vfStartQueuedReactionAnimation(FightActorRuntimePrefix *actor);
void vfResetScriptedFormationMotion(FightActorRuntimePrefix *actor);
void vfSnapActorToContact(FightActorRuntimePrefix *actor);
void vfPredictLandingGround(FightActorRuntimePrefix *actor);
void vfPlayFightSfx(std::uint32_t sound, FightActorRuntimePrefix *actor);
std::int32_t vfPrepareAuthoredDefeatCamera();
void vfSetFightFaceExpression(Character *character, std::int32_t expression, std::int32_t frames);
void vfProcessMotionFrameEvents(FightActorRuntimePrefix *actor);
void vfRotateFightVector(std::int16_t yaw, const Vector3 *input, Vector3 *output);
bool vfIsWithinFightFloor(const Vector3 &position);
void vfFinishAirborneMotion(FightActorRuntimePrefix *actor);
void vfUpdateAirborneAnimationTiming(FightActorRuntimePrefix *actor);
const std::byte *vfFindQueuedReactionEventEntry(FightActorRuntimePrefix *actor, std::uint8_t event);
void vfDispatchQueuedReactionEvent(FightActorRuntimePrefix *actor);
bool vfDidCrossFightFrame(FightActorRuntimePrefix *actor, float frame);
void vfInitializeActorMotionState(FightActorRuntimePrefix *actor);
void FreeFGTM(FightActorRuntimePrefix *actor);
void vfInitializeFightActor(FightRuntimePrefix *game, std::uint8_t index);
void vfUpdateGroundHeightAndEnemyContacts(FightActorRuntimePrefix *actor);
void vfUpdateAutoAdvanceLanding(FightActorRuntimePrefix *actor);
void vfUpdateMotionTargetPointAndParams(FightActorRuntimePrefix *actor);
void vfUpdateRootBlendEnvelope(FightActorRuntimePrefix *actor);
void vfActorUpdateState(FightActorRuntimePrefix *actor, FightActorRuntimePrefix *other);
void vfApplyKnockbackPhysics(FightActorRuntimePrefix *actor);
void vfclUpdatePairingFacing(FightActorRuntimePrefix *actor);
void mtwkRemoveCallback(MotionWork *motion);
void vfUpdatePositionXZ(FightActorRuntimePrefix *actor);
void vfDrawFightLoadSprite(std::int32_t index, std::int32_t x, std::int32_t y, float depth, std::uint32_t frames);
void ConstructColouredText(std::uint32_t frames, std::int32_t x, std::int32_t y, float depth);
void adjustStreetFighterHUD();
void constructNumStrikesLeft();
void vfUpdateHPRegeneration(FightActorRuntimePrefix *actor);
void vfUpdatePoseWithSfx(FightActorRuntimePrefix *actor);
void vfReleaseUnheldScriptedFormation(FightRuntimePrefix *game);
std::uint8_t vfFightVoiceStyle(std::uint32_t style);
void vfForceActorDefeat(FightActorRuntimePrefix *actor);
void vfStartAuthoredDefeatCamera(FightActorRuntimePrefix *actor);
void vfUpdateGroundImpactReaction(FightActorRuntimePrefix *actor);
std::int32_t vfTryGuardReactionSwap(FightActorRuntimePrefix *actor, FightActorRuntimePrefix *source);
void vfHandleImpactSfxAndDefeat(FightActorRuntimePrefix *actor, std::int16_t damage, std::uint8_t mode);
void vfUpdateFightFloorReaction(FightActorRuntimePrefix *actor);
std::uint32_t ScaleFightTransitionFrames(float scale, std::uint16_t frames);
void vfUpdateAnimFlagsAndEvents(FightActorRuntimePrefix *actor);
void vfUpdateReactAnimationFrames(FightActorRuntimePrefix *actor);
#else
void vfAddRoundResults(std::uint8_t won, std::uint8_t lost);
std::uint8_t vfMotnNibbleGet(std::int8_t code);
std::uint8_t vfMotnNibbleSet(std::int8_t code, std::int32_t value);
void vfMotnDispatch(VFActor *self);
void vfApplyMotionDescriptor(VFActor *self);
void vfUpdateMoveVelocityWithKnockback(VFActor *self, float acceleration);
void vfRescaleReactionFrameWindows(VFActor *self);
void vfApplyHitReactionAndStartMotion(VFActor *self);
void vfSelectDefaultMotion(VFActor *self, std::uint32_t index);
std::int8_t vfUpdatePredicate_40_41_44_50_51_54();
void vfResetScriptedFormationMotion(VFActor *self);
void vfSnapActorToContact(VFActor *self);
void vfPredictLandingGround(VFActor *self);
std::uint16_t vfRotateCameraSectorCode(VFActor *self, std::uint16_t code);
void vfSelectAICommand(VFActor *self, std::uint8_t choice, std::uint16_t delay);
bool vfUpdateAIInput(VFActor *self);
void vfAdvanceAICommand(VFActor *self);
void vfPlayFightSfx(std::uint32_t sound, VFActor *self, std::uint8_t mode);
std::uint8_t vfProcessMotionFrameEvents(VFActor *self);
bool vfIsWithinFightFloor(const Vector3 &position);
const std::byte *vfFindQueuedReactionEventEntry(VFActor *self, std::uint8_t eventType);
void vfDispatchQueuedReactionEvent(VFActor *self);
void vfInitializeActorMotionState(VFActor *self);
std::uint32_t vfScaleFrameCount(float scale, std::uint16_t frames);
void vfUpdateMotionTransitionFrames(VFActor *self);
void vfInitializeFightActor(VFMA_t *game, std::uint8_t index);
void vfUpdateMotionTargetPointAndParams(VFActor *self);
void vfUpdateRootBlendEnvelope(VFActor *self);
void vfActorUpdateState(VFActor *self, VFActor *other);
void vfclUpdatePairingFacing(VFActor *self);
void vfDrawFightLoadSprite(std::int32_t index, std::int32_t x, std::int32_t y, float depth, std::uint32_t elapsedFrames);
void ConstructColouredText(std::uint32_t elapsedFrames, std::int32_t x, std::int32_t y, float depth);
void constructNumStrikesLeft();
void vfUpdateHPRegeneration(VFActor *self);
float vfGetMoveProficiencyScale(std::uint8_t move);
float vfclCalculateGuardKnockbackFactor(VFActor *self);
void vfUpdatePoseWithSfx(VFActor *self);
std::uint8_t vfFightVoiceStyle(std::uint16_t style);
void vfForceActorDefeat(VFActor *self);
void vfStartAuthoredDefeatCamera(VFActor *self);
void vfUpdateGroundImpactReaction(VFActor *self);
bool vfTryGuardReactionSwap(VFActor *self, VFActor *source);
bool vfIsLastEnemyForCamera(VFActor *self);
void vfHandleImpactSfxAndDefeat(VFActor *self, std::int16_t damage, std::uint8_t mode);
bool vfCheckAnimTimeWithin(VFActor *self, float frame);
void vfUpdateFightFloorReaction(VFActor *self);
void vfUpdateAnimFlagsAndEvents(VFActor *self);
void vfUpdateReactAnimationFrames(VFActor *self);

namespace {

void ResetCameraPhase(VFSlotRuntimeState &slot);
template <class T> T MarkerValue(std::size_t offset);
std::int32_t AuthoredOffset(const std::byte *data);
float AuthoredFixed(const std::byte *data);
float MarkerFixed(std::size_t offset);
std::uint16_t SignedByteWord(std::size_t offset);
void DecrementFrame(std::uint16_t &frame);
void DecrementFrame(std::int16_t &frame);
template <class T> T ActorValue(const VFActor *actor, std::size_t offset);
template <class T> void SetActorValue(VFActor *actor, std::size_t offset, T value);
std::int32_t FloorWinner(VFMA_t *game);
void BeginFloorDefeat(VFActor *self);
std::uint32_t TimerColour(std::uint32_t elapsedFrames, std::uint32_t current);

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void vfResetFightActorInputState(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(std::uint32_t, actor, 0x2fc) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x300) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3d6) = 0;
    actor->moveFlags |= 0x4000000U;
    MOTION_VALUE(std::uint32_t, actor, 0x784) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x5b0) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x5b8) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x5a0) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x5a1) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x5a8) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x5aa) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x5a6) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x788) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x78c) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x598) = 1;
    MOTION_VALUE(std::uint8_t, actor, 0x599) = 1;
    MOTION_VALUE(std::uint16_t, actor, 0x498) = MOTION_VALUE(std::uint16_t, actor, 0x490);
    MOTION_VALUE(std::uint16_t, actor, 0x49a) = MOTION_VALUE(std::uint16_t, actor, 0x490);
    MOTION_VALUE(std::uint16_t, actor, 0x5d6) = MOTION_VALUE(std::uint16_t, actor, 0x494);
    VmResetSlots(actor);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfInitializeFightActor(FightRuntimePrefix *game, std::uint8_t index) {
    InitializeFightActorFromFormation(game, index);
    vfResetFightActorInputState(game->actors[index]);
    vfInitializeActorCollision(game, index);
    if (game->playerCharacter && game->characters[index] == game->playerCharacter)
        game->playerSlot = index;
}

#else

// Initialize the native actor from its actual FRMG owner.
// @verified - p
void vfInitializeFightActor(VFMA_t *game, std::uint8_t index) {
    VFActor *const self = game->actors[index];
    FRMG_t *const group = game->groups[index];
    self->groupIndex = index;
    self->character = game->characters[index];
    self->motion =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(self->character);
    self->game = game;
    const auto *const raw = reinterpret_cast<const std::byte *>(group);
    const auto word = [raw](std::size_t offset) {
        std::uint16_t value;
        std::memcpy(&value, raw + offset, sizeof(value));
        return value;
    };
    self->fightStyle = std::to_integer<std::uint8_t>(raw[0x1c]);
    std::memcpy(&self->formationFlags, raw + 0x28, 4);
    self->formationEntry = word(0x32);
    self->formationHpFloor = static_cast<std::int16_t>(word(0x34));
    SetActorValue(self, 0xa54, word(0x36));
    SetActorValue(self, 0xa56, word(0x38));
    self->spawnYaw[0] = word(0x3a);
    self->followupState = std::to_integer<std::uint8_t>(raw[0x30]);
    self->spawnYaw[1] = word(0x3c);
    self->sfxPeriod = word(0x3e);
    self->selectionPriority = std::to_integer<std::uint8_t>(raw[0x24]);
    if (raw[0x53] != std::byte{0xff})
        self->reactionRank = std::to_integer<std::uint8_t>(raw[0x53]);
    if (!group->totalHp)
        group->totalHp = 200;
    if (!group->hp)
        group->hp = 200;
    self->initialHp = group->hp;
    self->hp = float(group->totalHp);
    self->guardHp = self->guardMaximumHp = self->fightStyle == 0 ? 500 : std::min(float(group->totalHp), 200.0f);
    self->regenerationRate = self->fightStyle == 0 ? 0.4f : 0.5f;
    self->focusedRegenerationScale = self->fightStyle == 0 ? 0.2f * 0.8f : 0.2f;
    vfInitializeActorMotionState(self);
    const auto start = std::to_integer<std::uint8_t>(raw[0x54]);
    if (start == 2 || (start == 0xff && game->type == 2))
        self->stateFlags |= VFSF_Unknown8;
    else if (start == 1 || start == 0xff)
        self->stateFlags |= VFSF_Unknown4;
    if ((game->type == 1 && !(std::to_integer<std::uint8_t>(raw[0x24]) & 0x80)) || static_cast<std::uint8_t>(game->type - 2) <= 1)
        self->stateFlags |= VFSF_Unknown2;
    vfActorUpdateState(self, self->actor);
    self->moveFlags |= (VFMF_AutoAdvanceMotion | VFMF_RefreshContactHistory);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Both native colour selections inline this rule; an unknown scheme keeps the previous colour.
std::uint32_t TimerColour(std::uint32_t elapsedFrames, std::uint32_t current) {
    switch (timerColourMode) {
    case 0:
        return elapsedFrames < 300 ? 0xffff0000U : 0xffffffffU;
    case 1:
        return elapsedFrames < 300 ? 0xffff0000U : elapsedFrames < 600 ? 0xffffff00U : 0xffffffffU;
    case 2:
        return elapsedFrames < 300 ? 0xffff0000U : 0xffffff00U;
    default:
        return current;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateGroundHeightAndEnemyContacts(FightActorRuntimePrefix *actor) {
    std::uint32_t nearby;
    const float ground = vfAccumEnemyGroundOffsetAndMask(&actor->position, &nearby) + MOTION_VALUE(float, actor, 0x404);
    const float base = MOTION_VALUE(float, actor, 0x404);
    if (ground != base) {
        actor->targetGroundY = ground;
        if (!(actor->moveFlags & 4U)) {
            actor->position.y = ground;
            SetCharacterWorldPosition(actor->character, &actor->position);
            for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
                FCMG_t &slot = g_pFightStage->enemySlots[index];
                if (slot.character && slot.state == 3 && (nearby & (1U << index)) && (slot.collisionFlags & 9U) == 1U) {
                    MotionWork *const motion = slot.motion;
                    slot.collisionFlags |= 8U;
                    const float frame = mtwkGetFrame(motion, static_cast<MotionRegion>(0));
                    const float end = mtwkGetEndFrame(motion, static_cast<MotionRegion>(0));
                    if (!(slot.collisionFlags & 0x10U) && (frame == end || mtwkIsMotionNonLooping(motion, 0))) {
                        std::uint16_t animation;
                        if (slot.collisionFlags & 0x20U)
                            animation = 455;
                        else if (slot.collisionFlags & 0x40U)
                            animation = 454;
                        else if (slot.collisionFlags & 2U)
                            animation = 108;
                        else
                            animation = 107;
                        vfUpdateDefaultReactionAnim(&slot, animation);
                        if (slot.collisionFlags & 0x80U)
                            mtwkSetRegionFlag(slot.motion, 69, 0, static_cast<MotionRegion>(0));
                        else
                            mtwkSetRegionFlag(slot.motion, 69, 2, static_cast<MotionRegion>(0));
                    }
                }
            }
        }
    } else if (base < actor->targetGroundY) {
        actor->targetGroundY = base;
        if (!(actor->moveFlags & 4U)) {
            actor->position.y = base;
            SetCharacterWorldPosition(actor->character, &actor->position);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfDrawFightLoadSprite(std::int32_t index, std::int32_t x, std::int32_t y, float depth, std::uint32_t frames) {
    std::uint32_t colour = 0;
    if (g_fightLoadSprites[index].textureIndex == -1)
        return;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, g_fightLoadSprites[index].width, g_fightLoadSprites[index].height);
    packet.quad.flags |= 0x400U;
    switch (fightTimerColourMode) {
    case 0:
        colour = frames < 300 ? 0xffff0000U : 0xffffffffU;
        break;
    case 1:
        if (frames > 299)
            colour = frames < 600 ? 0xffffff00U : 0xffffffffU;
        else
            colour = 0xffff0000U;
        break;
    case 2:
        colour = frames < 300 ? 0xffff0000U : 0xffffff00U;
        break;
    }
    packet.colour = colour;
    DrawLegacySpritePacket(packet, g_fightLoadSprites[index].textureIndex, depth);
}

#else

// One fight load-sprite record placed at its authored size, coloured by the timer rule, then the native sprite nullsub.
// @verified - p
void vfDrawFightLoadSprite(std::int32_t index, std::int32_t x, std::int32_t y, float depth, std::uint32_t elapsedFrames) {
    const FightLoadSpriteRecord &sprite = g_fightLoadSprites[index];
    if (sprite.textureIndex == -1)
        return;
    LegacySpritePacket packet;
    packet.quad.flags = 0;
    packet.secondaryColour = 0;
    packet.colour = 0xffffffffU;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, y, sprite.width, sprite.height, static_cast<ScreenMode>(0));
    packet.quad.flags |= 0x400U;
    packet.colour = TimerColour(elapsedFrames, 0);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(sprite.textureIndex), depth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ConstructColouredText(std::uint32_t frames, std::int32_t x, std::int32_t y, float depth) {
    std::uint8_t first;
    std::uint32_t colour = 0;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    if (frames > 18000)
        first = 0;
    else if (frames > 1800)
        first = 1;
    else
        first = 2;
    if (first < 2)
        vfDrawFightLoadSprite(0, x + 74, y, depth, frames);
    vfDrawFightLoadSprite(0, x + 162, y, depth, frames);
    std::uint32_t value = frames;
    if (value > 108000)
        value = 108000;
    value *= 100;
    for (; first < 6; ++first) {
        const std::uint32_t remainder = value % fightTimerDivisors[first];
        value /= fightTimerDivisors[first];
        const std::uint8_t size = fightTimerPlaces[first].size;
        switch (fightTimerColourMode) {
        case 0:
            colour = frames < 300 ? 0xffff0000U : 0xffffffffU;
            break;
        case 1:
            if (frames > 299)
                colour = frames < 600 ? 0xffffff00U : 0xffffffffU;
            else
                colour = 0xffff0000U;
            break;
        case 2:
            colour = frames < 300 ? 0xffff0000U : 0xffffff00U;
            break;
        }
        packet.colour = colour;
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x + fightTimerPlaces[first].x, y + fightTimerPlaces[first].y, size, size);
        DrawLegacySpritePacket(packet, g_fightLoadDigits[value], depth);
        value = remainder;
    }
}

extern std::int32_t g_fightCombatCounterSnapshot;

std::int8_t GetFightCombatCounterSnapshot() {
    return static_cast<std::int8_t>(g_fightCombatCounterSnapshot);
}

#else

// The legacy separators and digits are placed and submitted to the native sprite nullsub unless the D3T street-fighter
// HUD is forced on, which instead receives the six digits.
// @verified - p
void ConstructColouredText(std::uint32_t elapsedFrames, std::int32_t x, std::int32_t y, float depth) {
    LegacySpritePacket packet;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    packet.quad.flags = 0x400U;
    std::uint8_t first = elapsedFrames > 18000 ? 0 : elapsedFrames > 1800 ? 1 : 2;
    if (!GetDebugForceUIEnabled(UIElement::StreetFighterHud)) {
        if (first < 2)
            vfDrawFightLoadSprite(0, x + 74, y, depth, elapsedFrames);
        vfDrawFightLoadSprite(0, x + 162, y, depth, elapsedFrames);
    }
    std::array<std::int32_t, 6> digits{};
    std::uint32_t value = (elapsedFrames > 108000 ? 108000 : elapsedFrames) * 100;
    std::uint32_t colour = 0;
    for (; first < 6; ++first) {
        const std::uint32_t digit = value / timerDivisors[first];
        value %= timerDivisors[first];
        digits[first] = static_cast<std::int32_t>(digit);
        colour = TimerColour(elapsedFrames, colour);
        const TimerDigitPlace &place = timerDigitPlaces[first];
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, place.x + x, place.y + y, place.size, place.size, ScreenMode::Centered);
        packet.colour = colour;
        if (!GetDebugForceUIEnabled(UIElement::StreetFighterHud))
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_fightLoadDigits[digit]), depth);
    }
    if (GetDebugForceUIEnabled(UIElement::StreetFighterHud)) {
        SetStreetFighterTimerDigits(digits);
        EnableElement(UIElement::StreetFighterHud);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void adjustStreetFighterHUD() {
    if (!(g_pFightStage->battleFlags & 0x10000U))
        return;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    float x = 600.0f;
    std::int32_t value = g_vfStreetFightLevel;
    do {
        const std::int32_t digit = value % 10;
        if (g_fightLoadDigits[digit] != -1) {
            AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), 90, 32, 32);
            DrawLegacySpritePacket(packet, g_fightLoadDigits[digit], 1.0f);
            x -= 32;
        }
        value /= 10;
    } while (value > 0);
    vfDrawBattleGaugeIcon(12, static_cast<std::int32_t>(x - 32), 90, 1.0f, 1.0f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void constructNumStrikesLeft() {
    if (!(g_pFightStage->battleFlags & 0x20000U))
        return;
    FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game)
        return;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 80, 32, 32);
    const std::int8_t strikes = static_cast<std::int8_t>(game->strikesLeft);
    if (strikes < 10 && g_fightLoadDigits[strikes] != -1)
        DrawLegacySpritePacket(packet, g_fightLoadDigits[strikes], 1.0f);
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    TransformScreenSpace(packet.quad, HorizontalStretch, 272, 80, 32, 32, 2.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, g_vfStrikeLabelSprite, 1.0f);
    TransformScreenSpace(packet.quad, HorizontalStretch, 384, 80, 32, 32, 2.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, g_vfStrikesLeftLabelSprite, 1.0f);
}

#else

// The strike count and its two label surfaces are placed and submitted to the native sprite nullsub after the text update.
// @verified - p
void constructNumStrikesLeft() {
    if (!(g_pFightStage->battleFlags & 0x20000))
        return;
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return;
    const std::int32_t strikes = static_cast<std::int8_t>(game->strikesLeft);
    ConstructNumStrikesLeftText(strikes);
    LegacySpritePacket packet;
    packet.quad.flags = 0x400U;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 235, 87, 32, 32, ScreenMode::AdjustedWithOffset);
    // Only the digit sprite is guarded; a negative count reads ahead of the digit table natively, into a nullsub.
    if (static_cast<std::uint32_t>(strikes) < 10 && g_fightLoadDigits[strikes] != -1)
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_fightLoadDigits[strikes]), 1.0f);
    packet.quad.flags = 0x400U;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 383, 86, 256, 32, 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_vfStrikeLabelSprites[0]), 1.0f);
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 451, 86, 128, 32, 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_vfStrikeLabelSprites[1]), 1.0f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float g_fightHpRegenerationMinimum = 1.0f / 30.0f;

} // namespace

// @verified - d
void vfUpdateHPRegeneration(FightActorRuntimePrefix *actor) {
    if (!(actor->hp < actor->initialHp)) {
        MOTION_VALUE(std::uint16_t, actor, 0x432) = 0;
        return;
    }
    if ((actor->moveFlags & 0x20U) || !(actor->hp > 0) || (actor->hitStateFlags & 0xa4000U) || (actor->game->stateFlags & 1U) || actor->game->type == 10)
        return;
    float rate = MOTION_VALUE(float, actor, 0x9ec);
    if (actor->hitStateFlags & 0x20000000U)
        rate *= MOTION_VALUE(float, actor, 0x9f0);
    rate *= MOTION_VALUE(float, actor, 0x9e4) / MOTION_VALUE(float, actor, 0x9e8);
    if (g_fightHpRegenerationMinimum > rate)
        rate = g_fightHpRegenerationMinimum;
    actor->hp += rate;
    if (actor->hp > actor->initialHp)
        actor->hp = actor->initialHp;
}

#else

// The authored guard-pool ratio scales the native per-tick regeneration.
// @verified - p
void vfUpdateHPRegeneration(VFActor *self) {
    const float maximum = static_cast<float>(self->initialHp);
    if (self->hp >= maximum) {
        self->ticks = 0;
        return;
    }
    if ((self->moveFlags & VFMF_ForceDefeat) || !(self->hp > 0) ||
        (self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_Unknown20000 | VFHSF_AirborneOrDowned)) || (self->game->mode & 1) || self->game->type == 10)
        return;
    float rate = self->regenerationRate;
    if (self->hitStateFlags & VFHSF_CameraFocusTarget)
        rate *= self->focusedRegenerationScale;
    // Native maxss keeps an unordered gain.
    const float gain = (self->guardHp / self->guardMaximumHp) * rate;
    self->hp += 0.033333335f > gain ? 0.033333335f : gain;
    if (self->hp > maximum)
        self->hp = maximum;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Saved proficiency of a packed move id scales by 0.002 per point; ids outside the three move categories scale by one.
// @verified - p
float vfGetMoveProficiencyScale(std::uint8_t move) {
    constexpr std::uint8_t starts[5]{0, 0, 32, 64, 96};
    constexpr std::uint8_t counts[4]{0, 28, 23, 22};
    std::uint8_t category = 1;
    while (move >= starts[category + 1])
        if (++category >= 4)
            return 1;
    auto index = static_cast<std::int8_t>(move - starts[category]);
    if (index >= static_cast<std::int8_t>(counts[category]))
        index = -1;
    const auto proficiency = static_cast<std::uint8_t>(GetMoveProficiency(category, static_cast<std::uint8_t>(index)));
    return static_cast<float>(proficiency) * 0.0020000001f + 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Saved move proficiency supplies the native guard multiplier.
// @verified - p
float vfclCalculateGuardKnockbackFactor(VFActor *self) {
    if (self->groupIndex != static_cast<std::uint8_t>(self->game->playerSlot))
        return 1;
    auto move = static_cast<std::uint8_t>(std::bit_cast<std::uint32_t>(self->reactionPower) >> 24);
    if (self->motionFlags & VFMOF_Unknown40000)
        move = static_cast<std::uint8_t>(self->packedHitImpulse >> 8);
    else if (move == 96)
        move = static_cast<std::uint8_t>(self->packedHitImpulse);
    return vfGetMoveProficiencyScale(move);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfActorMotionNullsub(CharacterNode *node);

// @verified - d
void vfUpdatePoseWithSfx(FightActorRuntimePrefix *actor) {
    vfActorMotionNullsub(GetCharacterNode(actor->character));
    vfActorUpdateState(actor, actor->actor);
    if (actor->game->stateFlags & 0x80000U)
        return;
    if (actor->formationFlags & 0x40000U) {
        const std::uint8_t period = MOTION_VALUE(std::uint8_t, actor, 0x942);
        if (period <= 1 || actor->game->elapsedTicks % period == 0)
            vfHandleImpactSfxAndDefeat(actor, 1, 2);
    } else {
        vfUpdateHPRegeneration(actor);
        if (MOTION_VALUE(std::int8_t, actor, 0) == actor->game->playerSlot && actor->game->type != 4 && actor->game->type != 9 &&
            !(actor->formationFlags & 0x30000U) && !actor->game->hpTimer) {
            actor->formationFlags = (actor->formationFlags | 0x40000U) & 0xfbffffffU;
            MOTION_VALUE(std::uint8_t, actor, 0x942) = 5;
        }
    }
}

#else

// HP assist/drain follows the pose update, not a separate tick gate.
// @verified - p
void vfUpdatePoseWithSfx(VFActor *self) {
    vfActorUpdateState(self, self->actor);
    VFMA_t *const game = self->game;
    if (game->mode & 0x80000)
        return;
    if (self->formationFlags & 0x40000) {
        if (!(self->hitStateFlags & (VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned)) &&
            (self->followupState < 2 || (int(game->frame) % self->followupState == 0 && int(game->previousFrame) % self->followupState != 0)))
            vfHandleImpactSfxAndDefeat(self, 1, 2);
    } else {
        vfUpdateHPRegeneration(self);
        if (self->groupIndex == game->playerSlot && game->type != 4 && game->type != 9 && !(self->formationFlags & 0x30000) && game->hpTimer <= 0) {
            self->followupState = 5;
            self->formationFlags = (self->formationFlags & ~0x04000000) | 0x40000;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void FreeFGTM(FightActorRuntimePrefix *actor) {
    HLib::ClearTaskDataMark(actor);
    HLib::FreeTaskData(actor);
}

namespace {

float g_fightHpRegenerationRate = 0.5f;
float g_fightHpRegenerationGuardScale = 0.2f;

void InitializeFightActorFromFormation(FightRuntimePrefix *game, std::uint8_t index) {
    FightActorRuntimePrefix *actor;
    FRMG_t *formation;
    std::uint16_t hp, initial;
    Character *character = game->characters[index];
    actor = game->actors[index];
    actor->groupIndex = index;
    actor->character = character;
    actor->motion = charFindMotionwork(character);
    actor->game = game;
    formation = game->formations[index];
    MOTION_VALUE(std::uint16_t, actor, 2) = formation->style;
    actor->formationFlags = formation->formationFlags;
    MOTION_VALUE(std::uint16_t, actor, 0x930) = formation->range;
    MOTION_VALUE(std::uint16_t, actor, 0x932) = formation->height;
    MOTION_VALUE(std::uint16_t, actor, 0x934) = formation->direction;
    MOTION_VALUE(std::uint16_t, actor, 0x936) = formation->animation;
    MOTION_VALUE(std::uint16_t, actor, 0x938) = formation->spawnYaw;
    MOTION_VALUE(std::uint8_t, actor, 0x942) = formation->retained24;
    MOTION_VALUE(std::uint16_t, actor, 0x93a) = formation->actionParameters[0];
    MOTION_VALUE(std::uint16_t, actor, 0x93c) = formation->actionParameters[1];
    MOTION_VALUE(std::uint8_t, actor, 0x944) = formation->selectFlags;
    if (static_cast<std::int8_t>(formation->assignment) != -1)
        MOTION_VALUE(std::uint8_t, actor, 0x780) = formation->assignment;
    if (!formation->totalHp)
        hp = 200;
    else
        hp = formation->totalHp;
    if (!formation->hp)
        initial = 200;
    else
        initial = formation->hp;
    actor->game->formations[actor->groupIndex]->totalHp = hp;
    actor->game->formations[actor->groupIndex]->hp = initial;
    actor->hp = hp;
    actor->initialHp = initial;
    if (!MOTION_VALUE(std::uint16_t, actor, 2)) {
        MOTION_VALUE(float, actor, 0x9e4) = MOTION_VALUE(float, actor, 0x9e8) = 500;
        MOTION_VALUE(float, actor, 0x9ec) = g_fightHpRegenerationRate * 0.8f;
        MOTION_VALUE(float, actor, 0x9f0) = g_fightHpRegenerationGuardScale * 0.8f;
    } else {
        MOTION_VALUE(float, actor, 0x9e4) = MOTION_VALUE(float, actor, 0x9e8) = 200;
        MOTION_VALUE(float, actor, 0x9ec) = g_fightHpRegenerationRate;
        MOTION_VALUE(float, actor, 0x9f0) = g_fightHpRegenerationGuardScale;
    }
    vfInitializeActorMotionState(actor);
    if (static_cast<std::int8_t>(formation->secondaryAssignment) == -1) {
        switch (game->type) {
        case 2:
            actor->stateFlags |= 8U;
            break;
        case 1:
        case 3:
        case 4:
        case 5:
        default:
            actor->stateFlags |= 4U;
            break;
        }
    } else if (static_cast<std::int8_t>(formation->secondaryAssignment) == 2) {
        actor->stateFlags |= 8U;
    } else if (static_cast<std::int8_t>(formation->secondaryAssignment) == 1) {
        actor->stateFlags |= 4U;
    }
    switch (game->type) {
    case 1:
    case 5:
        if (game->formations[actor->groupIndex]->selectFlags & 0x80U)
            break;
    case 2:
    case 3:
        actor->stateFlags |= 2U;
        break;
    }
    vfActorUpdateState(actor, actor->actor);
    actor->moveFlags |= 0x10000080U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define STYLE_SECTION(data) reinterpret_cast<const std::byte *>((FIXED_WORD((data) + 2) | FIXED_WORD(data) << 16) + reinterpret_cast<std::int32_t>(style))
void vfInitializeActorMotionState(FightActorRuntimePrefix *actor) {
    Character *const character = actor->character;
    actor->moveFlags = actor->stateFlags = 0;
    MotionWork *const motion = charFindMotionwork(character);
    mtwkSetSpeed(motion, 1, static_cast<MotionRegion>(0));
    float scale = mtwkGetSpeed(motion, static_cast<MotionRegion>(0)) * actor->game->knockbackAmount;
    if (scale < 0.2f)
        scale = 0.2f;
    else if (scale > 1.5f)
        scale = 1.5f;
    mtwkSetSpeed(motion, scale, static_cast<MotionRegion>(0));
    MOTION_VALUE(float, actor, 0x274) = MOTION_VALUE(float, actor, 0x26c) = scale;
    MOTION_VALUE(float, actor, 0x270) = 1 / scale;
    MOTION_VALUE(float, actor, 0x278) = 0;
    std::uint8_t index;
    const std::byte *style = 0;
    for (index = 0; index < 8; ++index)
        if (g_pFightStage->styles[index].style == MOTION_VALUE(std::uint16_t, actor, 2)) {
            style = reinterpret_cast<const std::byte *>(g_pFightStage->styles[index].file);
            break;
        }
    if (!style)
        style = reinterpret_cast<const std::byte *>(g_pFightStage->styles[0].file);
    mtwkSetExtraFlag(motion, 6, 0);
    mtwkRegisterGroundTargetCallbacks(motion, actor);
    MOTION_VALUE(const std::byte *, actor, 0x28) = STYLE_SECTION(style + 4);
    MOTION_VALUE(const std::byte *, actor, 0x2c) = STYLE_SECTION(style + 8);
    MOTION_VALUE(const std::byte *, actor, 0x30) = STYLE_SECTION(style + 12);
    MOTION_VALUE(const std::byte *, actor, 0x34) = STYLE_SECTION(style + 16);
    const std::byte *const parameter = MOTION_VALUE(const std::byte *, actor, 0x34);
    const std::byte *const animations = MOTION_VALUE(const std::byte *, actor, 0x30);
    MOTION_VALUE(const std::byte *, actor, 0x38) = STYLE_SECTION(parameter);
    MOTION_VALUE(const std::byte *, actor, 0x3c) = STYLE_SECTION(animations);
    MOTION_VALUE(float, actor, 0x420) = FIXED_VALUE(parameter + 24);
    MOTION_VALUE(float, actor, 0x40c) = FIXED_VALUE(parameter + 4);
    MOTION_VALUE(float, actor, 0x410) = FIXED_VALUE(parameter + 8);
    MOTION_VALUE(float, actor, 0x414) = FIXED_VALUE(parameter + 12);
    MOTION_VALUE(float, actor, 0x418) = FIXED_VALUE(parameter + 16);
    MOTION_VALUE(float, actor, 0x41c) = FIXED_VALUE(parameter + 20);
    MOTION_VALUE(float, actor, 0x428) = FIXED_VALUE(parameter + 28);
    const std::int32_t yaw = GetCharacterWorldYaw(actor->character);
    actor->facingAngle = yaw;
    actor->facingYaw = actor->temporaryFacingAngle = yaw;
    actor->contactPosition.y += 1;
    MOTION_VALUE(float, actor, 0x140) = actor->contactPosition.x;
    MOTION_VALUE(float, actor, 0x144) = actor->contactPosition.y;
    MOTION_VALUE(float, actor, 0x148) = actor->contactPosition.z;
    actor->groundClearance = 0;
    MOTION_VALUE(float, actor, 0x454) = MOTION_VALUE(float, actor, 0x458) = MOTION_VALUE(float, actor, 0x45c) = 0;
    MOTION_VALUE(float, actor, 0x4c) = MOTION_VALUE(float, actor, 0x50) = MOTION_VALUE(float, actor, 0x54) = 0;
    MOTION_VALUE(float, actor, 0x58) = MOTION_VALUE(float, actor, 0x5c) = MOTION_VALUE(float, actor, 0x60) = 0;
    MOTION_VALUE(float, actor, 0x460) = MOTION_VALUE(float, actor, 0x464) = MOTION_VALUE(float, actor, 0x468) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x318) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x320) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x324) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x328) = 0;
    actor->hitStateFlags = 0;
    actor->motionFlags = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x33c) = 0;
    actor->secondaryMotionFlags = 0;
    actor->savedHitStateFlags = 0;
    actor->savedMotionFlags = 0;
    actor->savedSecondaryMotionFlags = 0;
    MOTION_VALUE(float, actor, 0x424) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x364) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x9d8) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x5c8) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x5be) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3de) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3e0) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3e6) = 0;
    actor->currentAnimation = 0;
    actor->animationTime = 0;
    actor->queuedReactionEvent = 0;
    actor->hitParameter = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3bc) = 0;
    actor->inputFlags = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x348) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x360) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x2f8) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x2f4) = 0;
    actor->animationEndFrame = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3c4) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3a0) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3a2) = 0;
    MOTION_VALUE(float, actor, 0x3fc) = 0;
    MOTION_VALUE(float, actor, 0x400) = 0;
    MOTION_VALUE(float, actor, 0x404) = 0;
    actor->targetGroundY = actor->position.y;
    MOTION_VALUE(std::uint32_t, actor, 0x358) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x2fc) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x300) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x87f) = MOTION_VALUE(std::uint8_t, actor, 0x880) = MOTION_VALUE(std::uint8_t, actor, 0x881) = 0x60;
    MOTION_VALUE(std::int8_t, actor, 0x882) = -1;
    actor->runtimeFlags = 0;
    MOTION_VALUE(std::int8_t, actor, 0x9a7) = -1;
    MOTION_VALUE(std::uint8_t, actor, 0x94e) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x94f) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x95c) = 0;
    MOTION_VALUE(float, actor, 0x960) = 0;
    MOTION_VALUE(std::int8_t, actor, 0x9a5) = MOTION_VALUE(std::int8_t, actor, 0x9a6) =
        vfUpdatePredicate_40_41_44_50_51_54(actor->contactPosition.x, actor->contactPosition.z, -1);
    MOTION_VALUE(std::int8_t, actor, 0x9a4) = -1;
    actor->moveAngleTableIndex = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x940) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x9d4) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x9d6) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x9bd) = 0;
    MOTION_VALUE(float, actor, 0x9dc) = 1;
    MOTION_VALUE(std::uint32_t, actor, 0x9ac) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x9a8) = 0;
    actor->nearestSpawnPoint = vfFindNearestFightSpawn(actor);
    for (index = 0; index < 7; ++index) {
        float *const heights = &MOTION_VALUE(float, actor, 0x8a4);
        heights[index] = -10000;
        actor->previousGroundSamples[index] = -10000;
    }
    vfRefreshActorGameInfo(actor);
    if (mtwkSetRegionFlag(actor->motion, 70, 1, static_cast<MotionRegion>(0)))
        actor->moveFlags |= 0x40U;
    vfApplyScriptedFormationMotion(actor);
    actor->moveFlags |= 0x40000000U;
}
#undef STYLE_SECTION

#else

// Initialize the native authored parameter sections and active rig owner.
// @verified - p
void vfInitializeActorMotionState(VFActor *self) {
    self->moveFlags = self->stateFlags = 0;
    MotionWork *const motion =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(self->character);
    mtwkSetSpeed(motion, 1, MotionRegion::LowerBody);
    const float requested = mtwkGetSpeed(motion, MotionRegion::LowerBody) * self->game->knockbackAmount;
    const float scale = 0.2f > requested ? 0.2f : 1.5f < requested ? 1.5f : requested;
    mtwkSetSpeed(motion, scale, MotionRegion::LowerBody);
    self->knockbackAcceleration = 0;
    self->scale = self->timeScale = scale;
    self->inverseScale = 1 / scale;
    const std::byte *style = nullptr;
    for (std::uint8_t index = 0; index < 8; ++index) {
        if (g_pFightStage->styles[index].style == self->fightStyle) {
            style = static_cast<const std::byte *>(g_pFightStage->styles[index].file);
            break;
        }
    }
    if (!style)
        style = static_cast<const std::byte *>(g_pFightStage->styles[0].file);
    mtwkSetExtraFlag(motion, 6, 0);
    mtwkRegisterGroundTargetCallbacks(motion, self);
    mtwkSetAngle(motion, 0x800);
    self->motionParameters[0] = style + AuthoredOffset(style + 4);
    self->motionParameters[1] = style + AuthoredOffset(style + 8);
    self->motionParameters[2] = style + AuthoredOffset(style + 12);
    self->motionParameters[3] = style + AuthoredOffset(style + 16);
    const std::byte *const parameter = self->motionParameters[3];
    self->motionParameters[4] = style + AuthoredOffset(parameter);
    self->motionParameters[5] = style + AuthoredOffset(self->motionParameters[2]);
    self->knockbackBaseParameter = AuthoredFixed(parameter + 24);
    self->frontContactRange = AuthoredFixed(parameter + 4);
    self->reserved484 = AuthoredFixed(parameter + 8);
    self->rearContactRange = AuthoredFixed(parameter + 12);
    self->sideContactRange = AuthoredFixed(parameter + 16);
    self->contactRangeParameter = AuthoredFixed(parameter + 20);
    self->contactParameter = AuthoredFixed(parameter + 28);
    const auto facing = static_cast<std::uint16_t>(GetAllAngYForNode(&self->character->entity.node));
    self->contactPosition.y += 1;
    self->contactVectors[0] = self->contactPosition;
    self->facingAngle = self->temporaryFacingAngle = self->facingYaw = facing;
    self->actorRuntimeFlags = 0;
    self->knockbackVelocity = self->moveDelta = self->relativeMoveDelta = self->slideVelocity = {};
    self->parameter370 = self->action = self->deferredReaction = self->queuedAction = 0;
    self->hitStateFlags = self->savedHitStateFlags = 0;
    self->motionFlags = self->secondaryMotionFlags = self->activeMotionFlags = self->savedMotionFlags = 0;
    self->savedSecondaryMotionFlags = self->reserved3b4 = 0;
    self->contactTimer = 0;
    self->pairedContactPriority = self->guardContactFlags = 0;
    self->hitResolutionFlags = 0;
    self->currentHitAction &= ~std::uint64_t{0xffff};
    self->pendingHitMove = 0;
    self->footContactState = self->previousFootContactState = self->parameter45a = 0;
    self->currentAnimation = 0;
    self->animationTime = 0;
    self->queuedReactionEvent = self->hitParameter = 0;
    self->targetGroundY = self->position.y;
    self->hitFacingTimer = 0;
    self->inputFlags = 0;
    self->pairContactPriority = 0;
    self->pairContactMiddleFrame = 0;
    self->parameter33c[0] = self->parameter33c[1] = 0;
    self->hitTimeout = 0;
    self->parameter40c = 0;
    self->minimumGroundY = self->groundY = 0;
    self->groundParameter = self->reactionFlags = self->pairContactLastFrame = 0;
    self->packedHitImpulse = (self->packedHitImpulse & 0xff000000) | 0xff6060;
    self->reactionPower = std::bit_cast<float>((std::bit_cast<std::uint32_t>(self->reactionPower) & 0x00ffffff) | 0x60000000);
    self->formationRuntimeFlags = 0;
    self->parameterAd7 = -1;
    std::memset(&self->parameterA74, 0, 2);
    self->parameterA88 = self->parameterA8c = 0;
    std::memset(self->reservedA90, 0, 4);
    self->previousCooldownQuadrant = self->cooldownQuadrant = vfUpdatePredicate_40_41_44_50_51_54();
    self->targetCache = -1;
    self->moveAngleTableIndex = 0;
    self->parameterA64 = 0;
    self->selectFlags = self->parameterA67 = 0;
    self->parameterB20 = 0;
    self->parameterB24 = 0;
    self->otherActorStorage[1] = std::byte{};
    self->parameterB2c = 1;
    self->parameterAe0 = 0;
    self->parameterAd8 = nullptr;
    self->nearestSpawnPoint = 0;
    if (static_cast<std::uint8_t>(self->game->spawnPointCount) > 1) {
        float minimum = 9999;
        for (std::uint8_t index = 0; index < static_cast<std::uint8_t>(self->game->spawnPointCount); ++index) {
            const float x = self->game->spawnPointParameters[3 * index] - self->contactPosition.x;
            const float z = self->game->spawnPointParameters[3 * index + 1] - self->contactPosition.z;
            const float distance = std::sqrt(z * z + x * x);
            if (minimum > distance) {
                minimum = distance;
                self->nearestSpawnPoint = index;
            }
        }
    }
    for (std::uint8_t index = 0; index < 7; ++index)
        self->previousContactHeights[index] = self->previousGroundSamples[index] = -10000;
    vfRefreshContactReactionState(self);
    if (mtwkSetRegionFlag(self->motion, 70, 1, MotionRegion::LowerBody))
        self->moveFlags |= VFMF_MirrorReactionAngle;
    FRMG_t *const group = self->game->groups[self->groupIndex];
    if (self->game->mode & 0x800) {
        if (group->activationFlags) {
            self->pendingAnimation = group->activationFlags;
            vfApplyHitReactionAndStartMotion(self);
            self->game->mode |= 1;
        }
    } else
        group->activationFlags = 0;
    self->moveFlags |= VFMF_Unknown40000000;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Frame 1 is preserved before the rounded scale conversion.
// @verified - p
std::uint32_t vfScaleFrameCount(float scale, std::uint16_t frames) {
    if (scale == 1 || frames == 1)
        return frames;
    return static_cast<std::uint32_t>(static_cast<int>(float(frames) * scale + 0.5f));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Choose the authored transition duration before publishing MTWK mode 2.
// @verified - p
void vfUpdateMotionTransitionFrames(VFActor *self) {
    std::uint16_t frames;
    VFMA_t *const game = vfmaGetTaskParam();
    if (game && (game->mode & 0x80000) && game->parameter570 < 5)
        frames = 1;
    else if (self->fightStyle == 14 && (self->hitStateFlags & VFHSF_Unknown20000))
        frames = 5;
    else if ((self->hitStateFlags & VFHSF_AttackGuardWindow) && (self->savedSecondaryMotionFlags & VFSMF_AlternateFootTarget) &&
        (self->moveFlags & VFMF_Unknown80000))
        frames = 6;
    else if (self->hitStateFlags & (VFHSF_GuardState | VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned))
        frames = 5;
    else if (self->hitStateFlags & VFHSF_AlignedToGround)
        frames = 8;
    else if ((self->hitStateFlags & (VFHSF_HeavyReaction | VFHSF_SpecialGuardOrParry | VFHSF_Unknown20000)) || (self->activeMotionFlags & VFSMF_Unknown200000))
        frames = self->fightStyle == 8 ? 3 : 1;
    else if ((self->hitStateFlags & (VFHSF_LightReaction | VFHSF_ForcedReaction)) || (self->activeMotionFlags & VFSMF_Unknown2000000))
        frames = 2;
    else if (self->hitStateFlags & VFHSF_AttackActive)
        frames = static_cast<std::uint16_t>(self->parameter2d0 - static_cast<int>(self->animationTime));
    else
        frames = ((self->hitStateFlags & VFHSF_SnapToContact) && (self->moveFlags & VFMF_NeedsAnimationStep)) ? 1 : 8;
    frames = static_cast<std::uint16_t>(vfScaleFrameCount(self->inverseScale, frames));
    if (!frames)
        frames = 1;
    else if (frames > 8)
        frames = 8;
    if ((self->game->gameFlags & 0x1000000) || (self->secondaryMotionFlags & VFSMF_Unknown8000))
        frames = 1;
    self->motionTimer = static_cast<std::int16_t>(frames);
    mtwkSetFrameTime(self->motion, frames, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(self->motion, 2);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfActorUpdateState(FightActorRuntimePrefix *actor, FightActorRuntimePrefix *other) {
    FightRuntimePrefix *const game = actor->game;
    if ((actor->hitStateFlags & 0x10000002U) && (other->hitStateFlags & 0x100U))
        MOTION_VALUE(std::uint16_t, actor, 0x3c4) = 5;
    if ((other->hitStateFlags & 0x102U) != 0x100U && MOTION_VALUE(std::uint16_t, actor, 0x3c4))
        --MOTION_VALUE(std::uint16_t, actor, 0x3c4);
    actor->previousHitStateFlags = actor->hitStateFlags;
    actor->previousActiveMotionFlags = actor->activeMotionFlags;
    MOTION_VALUE(std::uint16_t, actor, 0x430) = MOTION_VALUE(std::uint16_t, actor, 0x42c);
    const std::int8_t quadrant =
        vfUpdatePredicate_40_41_44_50_51_54(actor->contactPosition.x, actor->contactPosition.z, MOTION_VALUE(std::int8_t, actor, 0x9a5));
    if (quadrant != MOTION_VALUE(std::int8_t, actor, 0x9a5)) {
        MOTION_VALUE(std::int8_t, actor, 0x9a6) = MOTION_VALUE(std::int8_t, actor, 0x9a5);
        MOTION_VALUE(std::int8_t, actor, 0x9a5) = quadrant;
    }
    std::uint32_t move = actor->moveFlags;
    move &= 0xfffff7feU;
    if (!MOTION_VALUE(std::uint16_t, actor, 0x42c))
        move |= 1U;
    else if (actor->moveFlags & 0x80U) {
        actor->animationTime += MOTION_VALUE(float, actor, 0x26c);
        ++MOTION_VALUE(std::uint16_t, actor, 0x3c6);
        if (static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x28c)) < actor->animationTime)
            move |= 1U;
    }
    actor->moveFlags = move;
    vfUpdateAirborneAnimationTiming(actor);
    vfDispatchCurrentActionState(actor);
    MOTION_VALUE(std::uint32_t, actor, 0x320) = 0;
    vfUpdateDynamicActionState(actor);
    vfUpdateFightFloorReaction(actor);
    vfDispatchQueuedReactionEvent(actor);
    vfUpdateAnimFlagsAndEvents(actor);
    MOTION_VALUE(std::uint32_t, actor, 0x794) = vfGetMoveCategoryMaskFor(actor);
    vfclUpdatePairingFacing(actor);
    vfApplyKnockbackPhysics(actor);
    if (actor->hitStateFlags & 0x800000U) {
        const shen_name area = g_currentMap;
        if (area == SHEN_NAME(QLBT) && !(actor->animationTime < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x296))))
            actor->targetGroundY = game->floorSnapY;
        else
            actor->targetGroundY = game->floorIntersectionY;
    } else if (game->type == 8 && !(actor->stateFlags & 0x80U)) {
        actor->targetGroundY = game->floorIntersectionY;
    } else {
        actor->targetGroundY = vfFieldGetIntersection(actor, actor->contactPosition);
    }
    MOTION_VALUE(float, actor, 0x404) = actor->targetGroundY;
    vfUpdateGroundHeightAndEnemyContacts(actor);
    vfUpdateAutoAdvanceLanding(actor);
    vfUpdateMotionTargetPointAndParams(actor);
    vfUpdateRootBlendEnvelope(actor);
    MOTION_VALUE(float, actor, 0x264) = -CalculateFixedSine(static_cast<std::uint32_t>(static_cast<std::int16_t>(actor->facingYaw)));
    MOTION_VALUE(float, actor, 0x268) = -CalculateFixedCosine(static_cast<std::uint32_t>(static_cast<std::int16_t>(actor->facingYaw)));
    vfUpdatePositionXZ(actor);
}

#else

// Preserve the native actor update ordering and shared tick owner.
// @verified - p
void vfActorUpdateState(VFActor *self, VFActor *other) {
    VFMA_t *const game = self->game;
    if ((self->hitStateFlags & (VFHSF_HitTimeout | VFHSF_ForcedReaction)) && (other->hitStateFlags & VFHSF_AttackActive))
        self->hitTimeout = 5;
    if (((other->hitStateFlags & VFHSF_HitTimeout) || !(other->hitStateFlags & VFHSF_AttackActive)) && self->hitTimeout > 0) {
        const float timeout = self->hitTimeout - shenGetScaledDeltaTickTime(1);
        self->hitTimeout = 0 > timeout ? 0 : timeout;
    }
    self->previousHitStateFlags = self->hitStateFlags;
    self->previousActiveMotionFlags = self->activeMotionFlags;
    self->previousAnimation = self->currentAnimation;
    const std::int8_t quadrant = vfUpdatePredicate_40_41_44_50_51_54();
    if (quadrant != self->cooldownQuadrant) {
        self->previousCooldownQuadrant = self->cooldownQuadrant;
        self->cooldownQuadrant = quadrant;
    }
    auto move = self->moveFlags & ~(VFMF_NeedsAnimationStep | VFMF_Unknown800);
    if (!self->currentAnimation)
        move |= 1;
    else if (self->moveFlags & VFMF_AutoAdvanceMotion) {
        self->animationTime += shenGetScaledDeltaTickTime(self->scale);
        self->tickFrameTime += shenGetScaledDeltaTickTime(1);
        if (self->animationTime > float(self->currentAnimationEndFrame))
            move |= 1;
    }
    self->moveFlags = move;
    vfUpdateAirborneAnimationTiming(self);
    vfDispatchCurrentActionState(self);
    self->action = 0;
    vfUpdateDynamicActionState(self);
    vfUpdateFightFloorReaction(self);
    vfDispatchQueuedReactionEvent(self);
    vfUpdateAnimFlagsAndEvents(self);
    SetActorValue(self, 0x8b4, vfGetMoveCategoryMaskFor(self));
    vfclUpdatePairingFacing(self);
    vfApplyKnockbackPhysics(self);
    if (self->hitStateFlags & VFHSF_AlignedToGround)
        self->targetGroundY =
            g_currentMap.name == SHEN_NAME(QLBT) && self->animationTime >= float(self->parameter2d6) ? game->floorSnapY : game->floorIntersectionY;
    else
        self->targetGroundY =
            game->type != 8 || (self->stateFlags & VFSF_Unknown80) ? vfFieldGetIntersection(self, self->contactPosition) : game->floorIntersectionY;
    self->groundY = self->targetGroundY;
    vfUpdateGroundHeightAndEnemyContacts(self);
    vfUpdateAutoAdvanceLanding(self);
    vfUpdateMotionTargetPointAndParams(self);
    vfUpdateRootBlendEnvelope(self);
    self->knockbackStrength = -sin_fixed16(self->facingYaw);
    self->animationScale = -cos_fixed16(self->facingYaw);
    vfUpdatePositionXZ(self);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateAirborneAnimationTiming(FightActorRuntimePrefix *actor) {
    if (!(actor->moveFlags & 0x2000U)) {
        if (!(actor->moveFlags & 4U))
            return;
        if (MOTION_VALUE(float, actor, 0x458) < 0)
            actor->moveFlags &= ~8U;
        if (actor->animationTime > static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x29e))) {
            std::int32_t last = MOTION_VALUE(std::uint16_t, actor, 0x2a0);
            const std::int32_t lead = MOTION_VALUE(std::int16_t, actor, 0x3be) - 1;
            last -= lead;
            if (last > 0 && !(static_cast<float>(last) < actor->animationTime))
                return;
            actor->animationTime -= MOTION_VALUE(float, actor, 0x26c);
            MOTION_VALUE(float, actor, 0x450) = actor->animationTime;
        }
    } else {
        if (actor->moveFlags & 4U) {
            actor->position.y = actor->targetGroundY;
            MOTION_VALUE(float, actor, 0x458) = 0;
            if (actor->targetGroundY > MOTION_VALUE(float, actor, 0x3fc))
                MOTION_VALUE(float, actor, 0x454) = MOTION_VALUE(float, actor, 0x45c) = 0;
            vfFinishAirborneMotion(actor);
        }
        const std::uint16_t last = MOTION_VALUE(std::uint16_t, actor, 0x2a0);
        if (last && actor->animationTime < static_cast<float>(last) && (actor->activeMotionFlags & 1U) == 0) {
            actor->animationTime = static_cast<float>(last);
            MOTION_VALUE(float, actor, 0x450) = actor->animationTime;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfDispatchQueuedReactionEvent(FightActorRuntimePrefix *actor) {
    if (!MOTION_VALUE(std::uint16_t, actor, 0x27e))
        return;
    if (MOTION_VALUE(std::uint16_t, actor, 0x27e) == 31) {
        MOTION_VALUE(std::uint32_t, actor, 0x434) = MOTION_VALUE(std::uint16_t, actor, 0x2d8);
        vfStartReactionAnimation(actor);
    } else if (MOTION_VALUE(std::uint16_t, actor, 0x27e) == 30) {
        if (actor->hitStateFlags & 0x100U) {
            MOTION_VALUE(std::uint32_t, actor, 0x434) = MOTION_VALUE(std::uint16_t, actor, 0x3ca);
            MOTION_VALUE(float, actor, 0x278) = actor->game->knockbackAmount;
            vfStartReactionAnimation(actor);
            MOTION_VALUE(std::uint16_t, actor, 0x27e) = 0;
            MOTION_VALUE(std::uint8_t, actor, 0x2e7) = 0;
            MOTION_VALUE(const std::byte *, actor, 0x2bc) = 0;
            MOTION_VALUE(std::uint16_t, actor, 0x2d8) = 0;
        }
    } else {
        const std::byte *const entry = vfFindQueuedReactionEventEntry(actor, MOTION_VALUE(std::uint8_t, actor, 0x27e));
        if (!entry) {
            MOTION_VALUE(std::uint16_t, actor, 0x27e) = 0;
        } else {
            std::uint32_t frame = *reinterpret_cast<const std::int16_t *>(entry + 2) - 1;
            if (!(actor->animationTime < frame)) {
                MOTION_VALUE(std::uint32_t, actor, 0x434) = *reinterpret_cast<const std::int16_t *>(entry + 4);
                vfStartReactionAnimation(actor);
            }
        }
    }
}

#else

// Dispatch uses the real marker entries retained by MOTI ownership.
// @verified - p
void vfDispatchQueuedReactionEvent(VFActor *self) {
    const auto event = self->queuedReactionEvent;
    if (!event)
        return;
    if (event == 31) {
        self->pendingAnimation = self->queuedReactionAnimation;
        vfApplyHitReactionAndStartMotion(self);
    } else if (event == 30) {
        if (self->hitStateFlags & VFHSF_AttackActive) {
            self->pendingAnimation = self->parameter43e;
            self->knockbackAcceleration = self->game->knockbackAmount;
            vfApplyHitReactionAndStartMotion(self);
            self->queuedReactionEvent = 0;
            self->reactionEventCount = 0;
            self->reactionEntries = nullptr;
            self->queuedReactionAnimation = 0;
        }
    } else if (const std::byte *entry = vfFindQueuedReactionEventEntry(self, static_cast<std::uint8_t>(event))) {
        std::int16_t threshold, animation;
        std::memcpy(&threshold, entry + 2, 2);
        std::memcpy(&animation, entry + 4, 2);
        // Native converts the decremented frame as an unsigned 32-bit value.
        if (!(float(static_cast<std::uint32_t>(threshold - 1)) > self->animationTime)) {
            self->pendingAnimation = animation;
            vfApplyHitReactionAndStartMotion(self);
        }
    } else
        self->queuedReactionEvent = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfUpdateAnimFlagsAndEvents(FightActorRuntimePrefix *actor) {
    const std::uint16_t current = MOTION_VALUE(std::uint16_t, actor, 0x42c);
    MotionWork *const motion = actor->motion;
    if (!current)
        vfSelectDefaultMotion(actor, 0);
    else if (actor->moveFlags & 1U) {
        const std::uint16_t requested = MOTION_VALUE(std::uint16_t, actor, 0x28e);
        MOTION_VALUE(std::uint32_t, actor, 0x434) = requested;
        if (requested == 0xffffU) {
            MOTION_VALUE(const std::byte *, actor, 0x310) = MOTION_VALUE(const std::byte *, actor, 0x30c);
            actor->savedHitStateFlags = actor->hitStateFlags;
            mtwkSetExtraFlag(actor->motion, 14, 0);
            float frame = actor->animationTime - MOTION_VALUE(float, actor, 0x26c) - static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x28c));
            if (frame < 0)
                frame = 0;
            actor->animationTime = frame;
            MOTION_VALUE(float, actor, 0x450) = frame;
            actor->moveFlags &= ~0x80000U;
        } else if (requested)
            vfStartFormationAnimation(actor);
        else if (!(actor->stateFlags & 0x80U) && (actor->hitStateFlags & 0xc000U) != 0x4000U)
            vfSelectDefaultMotion(actor, 0);
    }
    if (actor->animationTime < 0)
        actor->animationTime = 0;
    if (MOTION_VALUE(std::uint16_t, actor, 0x44c) != 0xffffU || MOTION_VALUE(float, actor, 0x450) != -1) {
        if (MOTION_VALUE(std::uint16_t, actor, 0x44c) == 0xffffU)
            MOTION_VALUE(std::uint16_t, actor, 0x44c) = MOTION_VALUE(std::uint16_t, actor, 0x42c);
        if (MOTION_VALUE(float, actor, 0x450) == -1)
            MOTION_VALUE(float, actor, 0x450) = 0;
        UpdateFightTransitionFrames(actor);
        if (actor->moveFlags & 0x20000U) {
            actor->moveFlags &= ~0x20000U;
            MOTION_VALUE(std::uint32_t, actor, 0x434) = mtwkGetAnimationId(motion, static_cast<MotionRegion>(0));
            vfApplyMotionDescriptor(actor);
        } else {
            mtwkRequestMotion(motion, MOTION_VALUE(std::uint16_t, actor, 0x44c) & 0xfffU, static_cast<MotionRegion>(0));
        }
    }
    std::int32_t timer = actor->motionTimer;
    if (timer)
        actor->motionTimer = static_cast<std::int16_t>(timer - 1);
    if (actor->moveFlags & 0x40U)
        mtwkSetRegionFlag(motion, 69, 0, static_cast<MotionRegion>(0));
    else
        mtwkSetRegionFlag(motion, 69, 2, static_cast<MotionRegion>(0));
    UpdateFightLeadFoot(actor);
    mtwkSampleCurrentMotion(motion, actor->animationTime, &MOTION_VALUE(Vector3, actor, 0x4c));
    std::uint32_t flags = 0;
    std::int32_t phase = MOTION_VALUE(std::uint8_t, actor, 0x288);
    if (phase) {
        const float frame = actor->animationTime;
        flags = actor->hitStateFlags;
        switch (phase) {
        case 1:
            if ((actor->motionFlags & 0x8100U) == 0x8100U && !(frame < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x290))))
                flags &= ~0x10000U;
            if (frame < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x290)))
                break;
            flags &= ~2U;
            ++MOTION_VALUE(std::uint8_t, actor, 0x288);
        case 2:
            if (frame < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x292)))
                break;
            if (flags & 0x100U)
                flags &= ~0x8000U;
            flags = (flags | 4U) & 0xeffefeffU;
            actor->activeMotionFlags &= 0x7fffafffU;
            ++MOTION_VALUE(std::uint8_t, actor, 0x288);
        case 3:
            if ((flags & 0x1010U) == 0x1010U && MOTION_VALUE(std::uint16_t, actor, 0x2d6) &&
                !(frame < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x2d6))))
                flags &= 1U;
            if (!(frame < static_cast<float>(actor->motionFrames))) {
                flags &= ~0x1001;
                ++MOTION_VALUE(std::uint8_t, actor, 0x288);
            }
            break;
        default:
            break;
        }
        actor->hitStateFlags = flags;
        flags = 0;
    }
    std::uint32_t direction = MOTION_VALUE(std::uint8_t, actor, 0x94a) & 0x78U;
    const std::uint32_t motionFlags = actor->motionFlags;
    if ((motionFlags & 0x20000400U) == 0x400U) {
        flags = 2;
        if ((motionFlags & 2U) || (!(actor->savedHitStateFlags & 0x400U) && (motionFlags & 0x400U)))
            flags |= 1U;
        else if ((MOTION_VALUE(std::uint8_t, actor, 0x94b) & 0x78U) != direction)
            flags |= 0x80U;
    } else if (actor->savedMotionFlags & 0x400U)
        flags |= 4U;
    if (!(flags & 0x87U))
        direction = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x94a) = flags | direction;
    flags = actor->hitStateFlags;
    if ((flags & 0x80100U) == 0x80100U)
        actor->activeMotionFlags |= 0x80000000U;
    if ((MOTION_VALUE(std::uint16_t, actor, 0x494) & 0x30fU) || (MOTION_VALUE(std::uint16_t, actor, 0x490) & 0x6000U)) {
        if (actor->activeMotionFlags & 0x10000U) {
            actor->activeMotionFlags &= ~0x10000U;
            actor->hitStateFlags &= ~1U;
        }
    }
    vfUpdateReactAnimationFrames(actor);
    MOTION_VALUE(const std::byte *, actor, 0x314) = MOTION_VALUE(const std::byte *, actor, 0x310);
    vfProcessMotionFrameEvents(actor);
}

#else

// Animation transition, hit-phase gates and the authored frame callbacks.
// @verified - p
void vfUpdateAnimFlagsAndEvents(VFActor *self) {
    MotionWork *const motion = self->motion;
    if (!self->currentAnimation)
        vfSelectDefaultMotion(self, 0);
    else if (self->moveFlags & VFMF_NeedsAnimationStep) {
        self->pendingAnimation = static_cast<std::uint16_t>(self->requestedAnimation);
        if (self->requestedAnimation == -1) {
            self->reactionProgram = self->mirroredReactionProgram;
            self->savedHitStateFlags = self->hitStateFlags;
            mtwkSetExtraFlag(motion, 14, 0);
            self->moveFlags &= ~VFMF_Unknown80000;
            const float time = self->animationTime - self->scale - float(self->currentAnimationEndFrame);
            self->animationTime = 0 > time ? 0 : time;
            self->parameter4c8 = self->animationTime;
        } else if (self->requestedAnimation)
            vfApplyHitReactionAndStartMotion(self);
        else if (!(self->stateFlags & VFSF_Unknown80) && (self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow)) != 0x4000)
            vfSelectDefaultMotion(self, 0);
    }
    if (self->animationTime < 0)
        self->animationTime = 0;
    if (self->parameter4c4 != 0xffff || self->parameter4c8 != -1) {
        if (self->parameter4c4 == 0xffff)
            self->parameter4c4 = self->currentAnimation;
        if (self->parameter4c8 == -1)
            self->parameter4c8 = 0;
        vfUpdateMotionTransitionFrames(self);
        if (self->moveFlags & VFMF_ForceFightingStyleTransition) {
            self->moveFlags &= ~VFMF_ForceFightingStyleTransition;
            self->pendingAnimation = mtwkGetAnimationId(motion, MotionRegion::LowerBody);
            vfApplyMotionDescriptor(self);
        } else
            mtwkRequestMotion(motion, self->parameter4c4 & 0xfff, MotionRegion::LowerBody);
    }
    DecrementFrame(self->motionTimer);
    mtwkSetRegionFlag(motion, 69, (self->moveFlags & VFMF_MirrorReactionAngle) ? 0 : 2, MotionRegion::LowerBody);
    if (self->fightStyle == 16 && (self->motionFlags & VFMOF_ContactCategoryA)) {
        std::int32_t selector = 0;
        if (self->cameraMotionFlags & 0x28)
            selector = (self->moveFlags & VFMF_MirroredStance) ? 25 : 30;
        else if (self->cameraMotionFlags & 0x50)
            selector = (self->moveFlags & VFMF_MirroredStance) ? 30 : 25;
        if (std::bit_cast<std::uint32_t>(self->reactionDepth))
            mtwkGetParentBoneWrapper(motion, selector);
    }
    mtwkSampleCurrentMotion(motion, self->animationTime, self->moveDelta);
    auto hit = self->hitStateFlags;
    switch (self->actionTriggerPhase) {
    case 1:
        if ((self->motionFlags & (VFMOF_Unknown100 | VFMOF_Unknown8000)) == 0x8100 &&
            self->animationTime >= float(static_cast<std::uint16_t>(self->parameter2d0)))
            hit &= ~0x10000u;
        if (self->animationTime < float(static_cast<std::uint16_t>(self->parameter2d0)))
            break;
        hit &= ~2u;
        ++self->actionTriggerPhase;
        [[fallthrough]];
    case 2:
        if (self->animationTime < float(static_cast<std::uint16_t>(self->reactionMiddleFrame)))
            break;
        if (hit & 0x100)
            hit &= ~0x8000u;
        self->activeMotionFlags &= ~(VFSMF_Unknown1000 | VFSMF_Unknown4000 | VFSMF_Unknown80000000);
        hit = (hit & 0xeffefeff) | 4;
        ++self->actionTriggerPhase;
        [[fallthrough]];
    case 3:
        if ((hit & 0x1010) == 0x1010 && self->parameter31e && self->animationTime >= float(self->parameter31e))
            hit &= 1;
        if (self->animationTime >= float(self->reactionEndFrame)) {
            hit &= 0xffffeffe;
            ++self->actionTriggerPhase;
        }
        break;
    default:
        break;
    }
    self->hitStateFlags = hit;
    std::uint8_t soundMode = 0;
    const auto preserved = self->cameraMotionFlags & 0x78;
    if ((self->motionFlags & (VFMOF_ContactCategoryA | VFMOF_Unknown20000000)) == 0x400) {
        soundMode = 2;
        if ((self->motionFlags & VFMOF_Unknown2) || (!(self->savedHitStateFlags & VFHSF_CameraAssistLock) && (self->motionFlags & VFMOF_ContactCategoryA)))
            soundMode = 3;
        else if ((ActorValue<std::uint8_t>(self, 0xa71) & 0x78) != preserved)
            soundMode = 0x82;
    } else if (self->savedMotionFlags & VFMOF_ContactCategoryA)
        soundMode = 4;
    self->cameraMotionFlags = soundMode ? static_cast<std::uint8_t>(preserved | soundMode) : 0;
    if ((hit & 0x80100) == 0x80100)
        self->activeMotionFlags |= VFSMF_Unknown80000000;
    if (((self->followupFlags & 0x30f) || (self->inputFlags & 0x6000)) && (self->activeMotionFlags & VFSMF_Unknown10000)) {
        self->activeMotionFlags &= ~VFSMF_Unknown10000;
        self->hitStateFlags &= ~VFHSF_ReactionActive;
    }
    vfUpdateReactAnimationFrames(self);
    self->reactionProgramCounter = self->reactionProgram;
    vfProcessMotionFrameEvents(self);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateReactAnimationFrames(FightActorRuntimePrefix *actor) {
    float authored, end;
    std::uint8_t index;
    if (!MOTION_VALUE(std::uint16_t, actor, 0x2c2))
        return;
    if (actor->stateFlags & 0x800U) {
        MOTION_VALUE(std::uint16_t, actor, 0x292) = 5;
        actor->motionFrames = 6;
        return;
    }
    if (!(actor->motionFlags & 0x40100U) && !(actor->secondaryMotionFlags & 0x20000000U))
        return;
    if (!((actor->hitStateFlags ^ actor->previousHitStateFlags) & 4U) && !((actor->activeMotionFlags ^ actor->previousActiveMotionFlags) & 0x20000000U))
        return;
    FightRuntimePrefix *const game = actor->game;
    std::uint8_t participants = 0;
    for (index = 0; index < 6; ++index)
        if (game->characters[index])
            ++participants;
    if (participants == 2 && ((actor->actor->hitStateFlags & 0x9c000U) || (actor->hitStateFlags & 0x40000U))) {
        const std::byte *const extra = MOTN_FindAnimationExtraRecord(MOTION_VALUE(std::uint16_t, actor, 0x42c) & 0xfffU, 2);
        end = static_cast<float>(*reinterpret_cast<const std::int16_t *>(extra + 6)) - 1;
        actor->motionFrames = static_cast<std::uint16_t>(end);
        MOTION_VALUE(std::uint16_t, actor, 0x28c) = static_cast<std::uint16_t>(GetMotionFrameCount(MOTION_VALUE(std::uint16_t, actor, 0x42c) & 0xfffU) - 1);
        MOTION_VALUE(float, actor, 0x43c) = static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x28c)) - MOTION_VALUE(float, actor, 0x26c);
        return;
    }
    authored = static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x2c2));
    if (actor->game->playerSlot != MOTION_VALUE(std::int8_t, actor, 0))
        authored = 0;
    const float middle = static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x292));
    const std::byte *const extra = MOTN_FindAnimationExtraRecord(MOTION_VALUE(std::uint16_t, actor, 0x42c) & 0xfffU, 2);
    if (!extra)
        end = 0;
    else
        end = static_cast<float>(*reinterpret_cast<const std::int16_t *>(extra + 6)) - 1;
    if (authored == 0 || middle > authored || !(end > authored))
        return;
    float nearest = 100;
    for (index = 0; index < 6; ++index) {
        if (actor->game->characters[index]) {
            FightActorRuntimePrefix *const other = actor->game->actors[index];
            if (other && actor->game->playerSlot != index && other->hp > 0 && !(other->motionFlags & 0x80000U) && nearest > actor->contactDistances[index])
                nearest = actor->contactDistances[index];
        }
    }
    if (!(nearest > 3))
        end = static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x2c2));
    else if (nearest < 8)
        end = static_cast<float>(static_cast<std::uint16_t>(((end - authored) / 5) * (nearest - 3) + 0.5f)) + authored;
    if (middle + 3 > end)
        end = middle + 3;
    if (!(actor->animationTime > static_cast<float>(actor->motionFrames)) && actor->animationTime > end)
        end = actor->animationTime;
    actor->motionFrames = static_cast<std::uint16_t>(end);
    const std::uint16_t previousEnd = MOTION_VALUE(std::uint16_t, actor, 0x28c);
    MOTION_VALUE(std::uint16_t, actor, 0x28c) = static_cast<std::uint16_t>(end);
    if (!(end > actor->animationTime)) {
        const std::byte *const command = MOTN_FindFrameEvent(1, MOTION_VALUE(std::uint16_t, actor, 0x42c) & 0xfffU, 0, static_cast<float>(previousEnd), 1);
        if (command && static_cast<float>(static_cast<std::int32_t>(*reinterpret_cast<const std::uint16_t *>(command)) - 1) > end)
            actor->hitStateFlags ^= 0x200000U;
        if (!figpGetFlagMask(actor->character, 0))
            figpAssignModeForCharacter(actor->character, 0);
    }
    if (actor->animationTime > static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x28c)))
        vfSelectDefaultMotion(actor, 0);
}

#else

// Reaction windows track the live participants, not a preview actor set.
// @verified - p
void vfUpdateReactAnimationFrames(VFActor *self) {
    if (!self->eventFrame30a)
        return;
    if (self->stateFlags & VFSF_Unknown800) {
        self->reactionMiddleFrame = 5;
        self->reactionEndFrame = 6;
        return;
    }
    if (!(self->motionFlags & (VFMOF_Unknown100 | VFMOF_Unknown40000)) && !(self->secondaryMotionFlags & VFSMF_Unknown20000000))
        return;
    if (!((self->hitStateFlags ^ self->previousHitStateFlags) & 4) && !((self->activeMotionFlags ^ self->previousActiveMotionFlags) & 0x20000000))
        return;
    VFMA_t *const game = self->game;
    std::uint8_t participants = 0;
    for (Character *character : game->characters)
        if (character)
            ++participants;
    const auto id = self->currentAnimation & 0xfff;
    const auto secondary = FindLoadedMotionSecondaryEntry(id, 2);
    const auto readWord = [](const std::byte *data) {
        std::uint16_t value;
        std::memcpy(&value, data, sizeof(value));
        return value;
    };
    if (participants == 2 &&
        ((self->actor->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_AirborneOrDowned)) ||
            (self->hitStateFlags & VFHSF_MidAirRecovery))) {
        self->reactionEndFrame = static_cast<std::uint16_t>(readWord(secondary.data() + 6) - 1);
        self->currentAnimationEndFrame = static_cast<std::uint16_t>(FindLoadedMotionSequence(id)->frameCount - 1);
        self->animationTimeParameter = float(self->currentAnimationEndFrame) - self->scale;
        return;
    }
    const float authored = game->playerSlot == self->groupIndex ? float(self->eventFrame30a) : 0;
    const auto middle = static_cast<std::uint16_t>(self->reactionMiddleFrame);
    auto end = secondary.empty() ? std::uint16_t{} : static_cast<std::uint16_t>(readWord(secondary.data() + 6) - 1);
    if (authored == 0 || float(middle) > authored || authored >= float(end))
        return;
    float nearest = 100;
    for (std::uint8_t index = 0; index < 6; ++index) {
        VFActor *const other = game->actors[index];
        if (game->characters[index] && other && game->playerSlot != index && other->hp > 0 && !(other->motionFlags & VFMOF_Unknown80000))
            nearest = std::fmin(self->contactDistances[index], nearest);
    }
    if (nearest <= 3)
        end = self->eventFrame30a;
    else if (nearest < 8)
        end = static_cast<std::uint16_t>(int(((float(end) - authored) / 5) * (nearest - 3) + 0.5f + authored));
    auto selected = static_cast<std::uint16_t>(middle + 3);
    if (end >= std::uint32_t(middle) + 3)
        selected = end;
    if (float(self->reactionEndFrame) >= self->animationTime && self->animationTime > float(selected))
        selected = static_cast<std::uint16_t>(int(self->animationTime));
    const auto previousEnd = self->currentAnimationEndFrame;
    self->reactionEndFrame = self->currentAnimationEndFrame = selected;
    if (self->animationTime >= float(selected)) {
        const auto command = FindLoadedMotionTimedCommand(id, 1, 0, float(previousEnd), 1);
        if (!command.empty() && int(readWord(command.data())) - 1 > int(selected))
            self->hitStateFlags ^= VFHSF_MirroredReactionAngle;
        const auto *figure = static_cast<FigurePose *>(
#if SHENMUE_IS_DC
charFindComp
#else
FindModelComponent
#endif
(self->character, MODEL_FIGURE_POSE));
        if (!figure || !(figure->flags & 1))
            figpAssignModeForCharacter(self->character, 0);
    }
    if (self->animationTime > float(self->currentAnimationEndFrame))
        vfSelectDefaultMotion(self, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateFightTransitionFrames(FightActorRuntimePrefix *actor) {
    const std::uint32_t hit = actor->hitStateFlags, active = actor->activeMotionFlags;
    std::uint32_t frames;
    if (IsEarlyAuthFightCamera())
        frames = 1;
    else if (MOTION_VALUE(std::uint16_t, actor, 2) == 14 && (hit & 0x20000U))
        frames = 5;
    else if ((actor->hitStateFlags & 0x200U) && (actor->savedSecondaryMotionFlags & 0x100U) && (actor->moveFlags & 0x80000U))
        frames = 6;
    else if (hit & 0xd0000U)
        frames = 5;
    else if (hit & 0x800000U)
        frames = 8;
    else if ((hit & 0x26000U) || (active & 0x200000U))
        frames = MOTION_VALUE(std::uint16_t, actor, 2) == 8 ? 3 : 1;
    else if ((hit & 0x10001000U) || (active & 0x2000000U))
        frames = 2;
    else if (hit & 0x100U)
        frames = MOTION_VALUE(std::uint16_t, actor, 0x290) - static_cast<std::uint16_t>(actor->animationTime);
    else
        frames = (hit & 0x20U) && (actor->moveFlags & 1U) ? 1 : 8;
    frames = ScaleFightTransitionFrames(MOTION_VALUE(float, actor, 0x270), frames);
    if (!frames)
        frames = 1;
    else if (frames > 8)
        frames = 8;
    if ((actor->game->controlFlags & 0x1000000U) || (actor->secondaryMotionFlags & 0x8000U))
        frames = 1;
    MOTION_VALUE(std::uint16_t, actor, 0x66) = static_cast<std::uint16_t>(frames);
    mtwkSetFrameTime(actor->motion, frames, static_cast<MotionRegion>(0));
    mtwkSetRequestedRootMode(actor->motion, 2);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateFightLeadFoot(FightActorRuntimePrefix *actor) {
    std::uint32_t selector = 0;
    if (MOTION_VALUE(std::uint16_t, actor, 2) == 16 && (actor->motionFlags & 0x400U)) {
        if (MOTION_VALUE(std::int8_t, actor, 0x94a) & 0x28)
            selector = actor->moveFlags & 0x400U ? 25 : 30;
        else if (MOTION_VALUE(std::int8_t, actor, 0x94a) & 0x50)
            selector = actor->moveFlags & 0x400U ? 30 : 25;
        if (MOTION_VALUE(std::uint32_t, actor, 0x864))
            mtwkGetParentBoneWrapper(actor->motion, selector);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfclUpdatePairingFacing(FightActorRuntimePrefix *actor) {
    bool preserve = false;
    std::uint32_t kind = 0;
    std::int16_t angle = 0, step = 0, limit = 0;
    Vector3 delta, rotated;
    if (actor->moveFlags & 0x400000U)
        return;
    FightActorRuntimePrefix *const other = actor->actor;
    const std::uint32_t hit = actor->hitStateFlags;
    const std::uint32_t otherHit = other->hitStateFlags;
    const std::uint32_t active = actor->activeMotionFlags;
    std::int32_t contact;
    if (hit & 0x800000U)
        contact = MOTION_VALUE(std::int16_t, actor, 0x17e);
    else {
        if (actor->moveFlags & 0x20U)
            return;
        if (hit & 0x8404000U)
            return;
        if ((hit & 0x1010U) == 0x1010U)
            return;
        if ((hit & 0x10100U) == 0x10000U && (otherHit & 0x110U))
            return;
        if (active & 0xc00U)
            return;
        if (active & 0x200004U)
            return;
        if ((hit & 0x100U) && (otherHit & 0x10000000U))
            return;
        contact = actor->actorFacingAngles[MOTION_VALUE(std::int8_t, actor, 1)];
    }
    if (actor->activeMotionFlags & 0x8000000U) {
        if (actor->hitStateFlags & 0x100U)
            kind = 5;
        else {
            if (MOTION_VALUE(std::uint16_t, actor, 2))
                return;
            if (!(actor->motionFlags & 4U))
                return;
            if (!(actor->contactDistances[MOTION_VALUE(std::int8_t, actor, 1)] < 3))
                return;
            if (actor->groundContactFlags & 1U)
                return;
            kind = 1;
            if (static_cast<std::uint16_t>((actor->currentAnimation & 0xfffU) - 58) < 2)
                return;
        }
    }
    if (hit & 0x800000U)
        return;
    if (active & 0x1000U)
        kind = 5;
    else if (hit & 0x10000000U)
        if (hit & 0x100U)
            kind = 0;
        else if (actor->contactDistances[MOTION_VALUE(std::int8_t, actor, 1)] < 0.4f)
            kind = 2;
        else
            kind = 3;
    else if (active & 0x2084000U)
        kind = 2;
    else if (hit & 0x2000U)
        kind = 0;
    else if (hit & 0x400U)
        kind = 2;
    else {
        if (other->motionFlags & 0x10000000U) {
            if (hit & 0x2400U)
                kind = 2;
            if ((hit & 0x10000100U) || MOTION_VALUE(std::int16_t, other, 0x3c4))
                return;
        }
        if (hit & 0xc0000U) {
            if (hit & 2U)
                kind = 4;
            else if (!(active & 0xc00U))
                return;
        } else if (((active & 0x800U) && (hit & 0x100U)) || ((active & 0x400U) && (otherHit & 0x100U)) || (hit & 0x8100U) == 0x8100U)
            kind = 2;
        else if (hit & 4U) {
            if ((hit & 0x14010U) || (actor->motionFlags & 0x100U))
                return;
            kind = 2;
        } else if ((hit & 0x102U) == 0x102U || (hit & 0x1102U) == 0x1002U)
            kind = 2;
        else if ((hit & 0x20004U) == 0x20000U)
            kind = 0;
        else if ((otherHit & 0x4010U) == 0x4010U && !(hit & 0x200U))
            kind = 2;
        else {
            if (hit & 2U)
                return;
            if (hit & 0x8000U)
                kind = 0;
            else if (hit & 0x40000000U)
                kind = 1;
            else
                return;
        }
    }
    static std::int16_t windows[8][2] = {{16380, 546}, {27300, 4550}, {24570, 1820}, {18200, 16380}, {24570, 182}, {32760, 182}, {16380, 910}, {25480, 3640}};
    switch (kind) {
    case 0:
        if (!(otherHit & 0x8000U))
            break;
        angle = contact - static_cast<std::int16_t>(actor->temporaryFacingAngle);
        limit = windows[0][0];
        step = windows[0][1];
        preserve = true;
        break;
    case 1:
        angle = contact - static_cast<std::int16_t>(actor->temporaryFacingAngle);
        limit = windows[1][0];
        step = windows[1][1];
        preserve = true;
        break;
    case 2:
        angle = contact - static_cast<std::int16_t>(actor->temporaryFacingAngle);
        if (vfCheckSkillFlag(shen_name(0x40000010U)) && !MOTION_VALUE(std::uint16_t, actor, 2)) {
            limit = windows[7][0];
            step = windows[7][1];
        } else {
            limit = windows[2][0];
            step = windows[2][1];
        }
        preserve = false;
        break;
    case 3:
        angle = contact - static_cast<std::int16_t>(actor->temporaryFacingAngle);
        limit = windows[3][0];
        step = windows[3][1];
        preserve = false;
        break;
    case 4:
        angle = other->facingAngle - actor->facingAngle;
        limit = windows[4][0];
        step = windows[4][1];
        preserve = false;
        break;
    case 5: {
        angle = contact - static_cast<std::int16_t>(actor->facingAngle);
        limit = windows[5][0];
        const std::uint16_t frame = MOTION_VALUE(std::uint16_t, actor, 0x290);
        std::int16_t remaining = static_cast<std::int16_t>((frame >> 1) + (frame >> 2));
        remaining -= static_cast<std::int32_t>(actor->animationTime - MOTION_VALUE(float, actor, 0x26c));
        if (remaining <= 0)
            remaining = 1;
        step = angle / remaining;
        preserve = false;
        break;
    }
    case 6:
        angle = contact - static_cast<std::int16_t>(actor->facingAngle);
        limit = windows[5][0];
        step = windows[5][1];
        preserve = false;
        break;
    default:
        break;
    }
    if (angle < -static_cast<std::int32_t>(limit) || limit < angle) {
        if (preserve)
            return;
        angle = static_cast<std::int16_t>(angle + 0x8000U);
    }
    if (-static_cast<std::int32_t>(step) > angle)
        angle = -step;
    else if (step < angle)
        angle = step;
    float degrees = MOTION_VALUE(float, actor, 0x26c) * (static_cast<float>(angle) / 182.04445f);
    if (!(degrees < 179.0f))
        degrees = 179.0f;
    if (!(degrees > -179.0f))
        degrees = -179.0f;
    const std::int16_t rotation = static_cast<std::int16_t>(static_cast<std::int32_t>((degrees * 8192.0f) / 45.0f));
    if (!(actor->moveFlags & 4U) || !(MOTION_VALUE(std::uint8_t, actor, 0x2c0) & 1U)) {
        delta.x = actor->position.x - actor->contactPosition.x;
        delta.z = actor->position.z - actor->contactPosition.z;
        vfRotateFightVector(-rotation, &delta, &rotated);
        actor->position.x = actor->contactPosition.x + rotated.x;
        actor->position.z = actor->contactPosition.z + rotated.z;
    }
    actor->facingAngle += rotation;
}

#else

// Paired facing chooses the native signed angle window before rotating.
// @verified - p
void vfclUpdatePairingFacing(VFActor *self) {
    if (self->moveFlags & VFMF_Unknown400000)
        return;
    const auto hit = self->hitStateFlags;
    const auto active = self->activeMotionFlags;
    VFActor *const other = self->actor;
    const auto otherHit = other->hitStateFlags;
    const auto target = self->actionIndex;
    std::int16_t contact;
    if (hit & 0x800000)
        contact = static_cast<std::int16_t>(self->parameter1b6);
    else {
        if ((self->moveFlags & VFMF_ForceDefeat) || (hit & 0x8404000) || (hit & 0x1010) == 0x1010 || ((hit & 0x10100) == 0x10000 && (otherHit & 0x110)) ||
            (active & 0xc00) || (active & 0x200004) || ((hit & 0x100) && (otherHit & 0x10000000)))
            return;
        contact = self->contactAngles[target];
    }
    std::uint8_t kind = 0;
    if (active & 0x8000000) {
        if (hit & 0x100)
            kind = 5;
        else {
            if (self->fightStyle != 0 || !(self->motionFlags & VFMOF_Unknown4) || !(3 > self->contactDistances[target]) ||
                (ActorValue<std::uint32_t>(self, 0x3c0) & 1) || (self->currentAnimation & 0xfff) == 59 || (self->currentAnimation & 0xfff) == 58)
                return;
            kind = 1;
        }
    }
    if (hit & 0x800000)
        return;
    if (active & 0x1000)
        kind = 5;
    else if (hit & 0x10000000)
        kind = (hit & 0x100) ? 0 : (self->contactDistances[target] < 2 ? 8 : 3);
    else if (active & 0x2084000)
        kind = 2;
    else if (hit & 0x2000)
        kind = 0;
    else if (hit & 0x400)
        kind = 2;
    else {
        if (other->motionFlags & VFMOF_DelayGuardResolution) {
            if (hit & 0x2400)
                kind = 2;
            if ((hit & 0x10000100) || other->hitTimeout > 0)
                return;
        }
        if (hit & 0xc0000) {
            if (hit & 2)
                kind = 4;
            else if (!(active & 0xc00))
                return;
        } else if (((active & 0x800) && (hit & 0x100)) || ((active & 0x400) && (otherHit & 0x100)))
            kind = 2;
        else if ((hit & 0x8100) == 0x8100)
            kind = 2;
        else if (hit & 4) {
            if ((hit & 0x14010) || (self->motionFlags & VFMOF_Unknown100))
                return;
            kind = 2;
        } else if ((hit & 0x102) == 0x102 || (hit & 0x1102) == 0x1002)
            kind = 2;
        else if ((hit & 0x20004) == 0x20000)
            kind = 0;
        else if ((otherHit & 0x4010) == 0x4010 && !(hit & 0x200))
            kind = 2;
        else {
            if (hit & 2)
                return;
            if (hit & 0x8000)
                kind = 0;
            else if (hit & 0x40000000)
                kind = 1;
            else
                return;
        }
    }
    constexpr std::int16_t windows[9][2]{
        {32760, 5460}, {27300, 4550}, {24570, 1820}, {18200, 16380}, {24570, 182}, {32760, 182}, {16380, 910}, {25480, 3640}, {16380, 16380}};
    std::int16_t angle = 0, limit = 0, step = 0;
    bool preserveHemisphere = false;
    switch (kind) {
    case 0:
        if (!(hit & 0x8000))
            break;
        [[fallthrough]];
    case 1:
        angle = static_cast<std::int16_t>(contact - self->temporaryFacingAngle);
        limit = windows[kind][0];
        step = windows[kind][1];
        preserveHemisphere = true;
        break;
    case 2: {
        angle = static_cast<std::int16_t>(contact - self->temporaryFacingAngle);
        const auto index = vfCheckSkillFlag(shen_name{0x40000010}) && self->fightStyle == 0 ? 7 : 2;
        limit = windows[index][0];
        step = windows[index][1];
        break;
    }
    case 3:
    case 8:
        angle = static_cast<std::int16_t>(contact - self->temporaryFacingAngle);
        limit = windows[kind][0];
        step = windows[kind][1];
        break;
    case 4:
        angle = static_cast<std::int16_t>(other->facingAngle - self->facingAngle);
        limit = windows[4][0];
        step = windows[4][1];
        break;
    case 5: {
        angle = static_cast<std::int16_t>(contact - self->facingAngle);
        limit = windows[5][0];
        const auto frame = static_cast<std::uint16_t>(self->parameter2d0);
        auto remaining = static_cast<std::int16_t>((frame >> 1) + (frame >> 2) - int(self->animationTime - self->scale));
        if (remaining <= 0)
            remaining = 1;
        step = static_cast<std::int16_t>(angle / remaining);
        break;
    }
    default:
        break;
    }
    if (angle < -int(limit) || angle > limit) {
        if (preserveHemisphere)
            return;
        angle = static_cast<std::int16_t>(angle + 0x8000);
    }
    const auto limited = angle < -int(step) ? static_cast<std::int16_t>(-step) : (angle > step ? step : angle);
    const float degrees = (float(limited) / 182.04445f) * self->scale;
    const float upper = 179.0f < degrees ? 179.0f : degrees, clamped = -179.0f > upper ? -179.0f : upper;
    const auto rotation = static_cast<std::int16_t>(static_cast<std::int32_t>(clamped * 182.0f));
    if (!(self->moveFlags & VFMF_Airborne) || !(self->motionContactFlags & 1)) {
        const float x = self->position.x - self->contactPosition.x;
        const float z = self->position.z - self->contactPosition.z;
        const auto opposite = static_cast<std::int16_t>(-rotation);
        const float sine = sin_fixed16(opposite), cosine = cos_fixed16(static_cast<std::uint16_t>(opposite));
        self->position.x = cosine * x - sine * z + self->contactPosition.x;
        self->position.z = sine * x + cosine * z + self->contactPosition.z;
    }
    self->facingAngle = static_cast<std::uint16_t>(self->facingAngle + rotation);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float g_fightKnockbackMaterialBlend = 0.5f;

void vfApplyKnockbackPhysics(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *game = actor->game;
    float previousX = actor->position.x;
    float previousY = actor->position.y;
    float previousZ = actor->position.z;
    float drag = game->dragScale * game->gravityScale * ((MOTION_VALUE(float, actor, 0x420) - 60.0f) * g_fightKnockbackMaterialBlend + 60.0f) * 0.033333335f *
        (MOTION_VALUE(float, actor, 0x424) + 1);
    float slideDrag = drag * game->slideDragScale;
    drag *= MOTION_VALUE(float, actor, 0x26c);
    slideDrag *= MOTION_VALUE(float, actor, 0x26c);
    float length;
    std::int32_t moving = 0;
    if (!(actor->hitStateFlags & 0x10U)) {
        length = shenmue::NativeSqrt(
            MOTION_VALUE(float, actor, 0x454) * MOTION_VALUE(float, actor, 0x454) + MOTION_VALUE(float, actor, 0x45c) * MOTION_VALUE(float, actor, 0x45c));
        if (length > drag) {
            length = 1 - drag / length;
            MOTION_VALUE(float, actor, 0x454) *= length;
            MOTION_VALUE(float, actor, 0x45c) *= length;
            moving = 1;
        } else {
            MOTION_VALUE(float, actor, 0x454) = 0;
            MOTION_VALUE(float, actor, 0x45c) = 0;
        }
    }
    length = shenmue::NativeSqrt(
        MOTION_VALUE(float, actor, 0x460) * MOTION_VALUE(float, actor, 0x460) + MOTION_VALUE(float, actor, 0x468) * MOTION_VALUE(float, actor, 0x468));
    if (length > slideDrag) {
        length = 1 - slideDrag / length;
        MOTION_VALUE(float, actor, 0x460) *= length;
        MOTION_VALUE(float, actor, 0x468) *= length;
    } else {
        MOTION_VALUE(float, actor, 0x460) = 0;
        MOTION_VALUE(float, actor, 0x468) = 0;
        if (!moving)
            MOTION_VALUE(float, actor, 0x424) = 0;
    }
    if (actor->moveFlags & 4U)
        MOTION_VALUE(float, actor, 0x458) = MOTION_VALUE(float, actor, 0x458) -
            MOTION_VALUE(float, actor, 0x26c) * MOTION_VALUE(float, actor, 0x26c) * game->gravityScale + MOTION_VALUE(float, actor, 0x464);
    actor->position.x += MOTION_VALUE(float, actor, 0x454);
    actor->position.y += MOTION_VALUE(float, actor, 0x458);
    actor->position.z += MOTION_VALUE(float, actor, 0x45c);
    actor->position.x += MOTION_VALUE(float, actor, 0x460);
    actor->position.z += MOTION_VALUE(float, actor, 0x468);
    MOTION_VALUE(float, actor, 0x478) = actor->position.x - previousX;
    MOTION_VALUE(float, actor, 0x47c) = actor->position.y - previousY;
    MOTION_VALUE(float, actor, 0x480) = actor->position.z - previousZ;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateAutoAdvanceLanding(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    if (game->controlFlags & 0x20U)
        return;
    const std::uint32_t flags = actor->moveFlags;
    if (!(flags & 0x80U))
        return;
    actor->moveFlags = flags & ~0x2000U;
    const float ground = actor->targetGroundY;
    if (!(flags & 4U)) {
        if (ground != actor->position.y && !(ground + 0.5f < actor->position.y))
            actor->position.y = ground;
        if (actor->motionFlags & 0x10U) {
            if (!(actor->animationTime < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x296))) &&
                actor->animationTime < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x2a0)))
                actor->hitStateFlags |= 0x10U;
            else
                actor->hitStateFlags &= ~0x10U;
        }
    } else if (!(actor->moveFlags & 8U)) {
        const float *const contact = &actor->contactPosition.y;
        float *const fall = &MOTION_VALUE(float, actor, 0x458);
        const float scale = MOTION_VALUE(float, actor, 0x26c);
        const float velocity = *fall - (scale * scale) * game->gravityScale;
        if (velocity + *contact < MOTION_VALUE(float, actor, 0x2a4) + actor->targetGroundY || actor->position.y + velocity < actor->targetGroundY)
            actor->moveFlags |= 0x2000U;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct FightFloorReaction {
    std::int32_t animation;
    std::int16_t turn;
};

const FightFloorReaction floorReactions[15] = {{279, 0}, {278, -0x8000}, {271, 0}, {273, 0}, {270, -0x8000}, {272, -0x8000}, {275, 0}, {277, 0}, {274, -0x8000},
    {276, -0x8000}, {99, -0x4000}, {32868, 0x4000}, {32867, -0x4000}, {100, 0x4000}, {672, -0x8000}};

const std::uint32_t floorEdgeSwaps[8] = {8, 64, 16, 128, 1024, 8192, 2048, 16384};

// Ten sampled body positions.
struct FightActorSamplePoints {
    std::byte retained00[0x74];
    Vector3 positions[10];
};

} // namespace

// @verified - d
void vfUpdateFightFloorReaction(FightActorRuntimePrefix *actor) {
    std::int16_t quadrant;
    std::int16_t back;
    int playerEscape = 0;
    std::int8_t choice;
    FightRuntimePrefix *const game = actor->game;
    choice = -1;
    if (static_cast<std::uint32_t>(game->state) < 3U || game->type != 8 || IsEarlyAuthFightCamera())
        return;
    if (actor->groupIndex == game->playerSlot && (actor->hitStateFlags & 0x80000U))
        playerEscape = IsAuthFightCamera() == 0;
    Vector3 point;
    point.x = (MOTION_VALUE(float, actor, 0xbc) + MOTION_VALUE(float, actor, 0xd4)) * 0.5f * 0.3f + MOTION_VALUE(float, actor, 0x74) * 0.7f;
    point.y = actor->position.y;
    point.z = (MOTION_VALUE(float, actor, 0xc4) + MOTION_VALUE(float, actor, 0xdc)) * 0.5f * 0.3f + MOTION_VALUE(float, actor, 0x7c) * 0.7f;
    if (game->type != 8 || vfIsWithinFightFloor(point))
        actor->stateFlags &= ~0x100U;
    else
        actor->stateFlags |= 0x100U;
    if (actor->stateFlags & 0x80U) {
        if (mtwkSetRegionFlag(actor->motion, 65, 1, static_cast<MotionRegion>(0)) && FindFightFloorWinner(game) == actor->groupIndex)
            actor->moveFlags |= 0x20U;
        return;
    }
    std::uint32_t ground = MOTION_VALUE(std::uint16_t, actor, 0x3ec);
    if (actor->moveFlags & 0x40U) {
        ground &= ~0x6cd8U;
        for (std::uint8_t swap = 0; swap < 4; ++swap) {
            if (MOTION_VALUE(std::uint16_t, actor, 0x3ec) & floorEdgeSwaps[swap * 2])
                ground |= floorEdgeSwaps[swap * 2 + 1];
            if (MOTION_VALUE(std::uint16_t, actor, 0x3ec) & floorEdgeSwaps[swap * 2 + 1])
                ground |= floorEdgeSwaps[swap * 2];
        }
    }
    const std::uint32_t inverse = ~ground;
    quadrant = (static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(
                    point.x - g_pFightStage->spawnPoints[actor->nearestSpawnPoint].x, point.z - g_pFightStage->spawnPoints[actor->nearestSpawnPoint].z)) +
                   0xa000) &
        0xc000;
    const std::int16_t relative = quadrant - actor->facingYaw;
    back = (relative + 0x4000) & 0x8000;
    const int side = MOTION_VALUE(float, actor, 0x248) > 0 ? 0 : 1;
    const int bothEdges = (inverse & 0x4800U) == 0x4800U;
    if ((MOTION_VALUE(std::uint16_t, actor, 0x3e6) & 0x4800U) && !(MOTION_VALUE(std::uint16_t, actor, 0x3e6) & ~0x4800U) &&
        !(actor->hitStateFlags & 0x9c000U)) {
        if ((inverse & MOTION_VALUE(std::uint16_t, actor, 0x3e6) & 0x4800U) || (actor->stateFlags & 0x100U))
            choice = back != 0;
    } else if ((MOTION_VALUE(std::uint16_t, actor, 0x3e6) & MOTION_VALUE(std::uint16_t, actor, 0x3de)) && MOTION_VALUE(std::uint8_t, actor, 0x39c) == 2 &&
        (!(actor->hitStateFlags & 0x1010U) || !(actor->contactPosition.y > game->floorIntersectionY + MOTION_VALUE(float, actor, 0x2a4)))) {
        std::int8_t edges = 0;
        if (actor->stateFlags & 0x100U)
            edges += 2;
        if (inverse & 0x800U)
            ++edges;
        if (inverse & 0x4000U)
            ++edges;
        if (inverse & 0x10U)
            ++edges;
        if (inverse & 0x80U)
            ++edges;
        if (edges >= 2) {
            if (actor->hitStateFlags & 0xc0000U) {
                if (FindFightFloorWinner(game) != -1)
                    choice = 1;
                else if (!back)
                    choice = 14;
                else
                    choice = 1;
            } else {
                if (bothEdges && !(inverse & 4U))
                    choice = 2;
                else if (!bothEdges && (inverse & 4U))
                    choice = 4;
                else if ((ground & 0x4890U) == 0x4080U)
                    choice = 10;
                else if ((ground & 0x4890U) == 0x810U)
                    choice = 12;
                if (choice != -1) {
                    if ((actor->hitStateFlags & 0xc00U) == 0x800U && side == 1)
                        ++choice;
                    if (playerEscape) {
                        vfStartAuthoredDefeatCamera(actor->secondActor);
                        actor->stateFlags &= ~0x100U;
                        choice = -1;
                    }
                }
            }
        }
    } else if (!ground) {
        if (FindFightFloorWinner(game) != -1)
            choice = 1;
        else if (playerEscape) {
            vfStartAuthoredDefeatCamera(actor->secondActor);
            actor->stateFlags &= ~0x100U;
        } else {
            BeginFightFloorDefeat(actor);
            MOTION_VALUE(std::uint32_t, actor, 0x318) = 0x19000000U;
            MOTION_VALUE(std::uint8_t, actor, 0x27c) = 1;
            MOTION_VALUE(float, actor, 0x158) = actor->contactPosition.x;
            MOTION_VALUE(float, actor, 0x160) = actor->contactPosition.z;
        }
    }
    if (choice == -1)
        return;
    actor->moveFlags |= 0x10U;
    BeginFightFloorDefeat(actor);
    const float height = game->floorIntersectionY - actor->position.y;
    actor->position.y += height;
    actor->contactPosition.y += height;
    MOTION_VALUE(std::int32_t, actor, 0x434) = floorReactions[choice].animation;
    if (g_currentMap != AREA_QLBT)
        MOTION_VALUE(float, actor, 0x278) = MOTION_VALUE(float, actor, 0x26c);
    else
        MOTION_VALUE(float, actor, 0x278) = 0.5f;
    if (FindFightFloorWinner(actor->game) != actor->groupIndex)
        MOTION_VALUE(float, actor, 0x278) *= 0.7f;
    vfStartFormationAnimation(actor);
    MOTION_VALUE(std::uint32_t, actor, 0x318) = 0x19000000U;
    MOTION_VALUE(std::uint8_t, actor, 0x27c) = 1;
    const std::int16_t turn = floorReactions[choice].turn;
    MOTION_VALUE(std::uint16_t, actor, 0x17e) = actor->facingAngle = turn + quadrant;
    actor->temporaryFacingAngle = actor->facingYaw = MOTION_VALUE(std::uint16_t, actor, 0x17e);
    actor->facingAngleDelta = 0;
    float nearest = 9999;
    float x = actor->position.x, z = actor->position.z;
    for (std::int32_t sample = 0; sample < 10; ++sample) {
        if (!(MOTION_VALUE(std::uint16_t, actor, 0x3ea) &
                *reinterpret_cast<const std::uint16_t *>(MOTION_VALUE(const std::byte *, actor, 0x9a8) + sample * 20 + 18)))
            continue;
        const float distance =
            NativeSqrt((reinterpret_cast<FightActorSamplePoints *>(actor)->positions[sample].x - g_pFightStage->spawnPoints[actor->nearestSpawnPoint].x) *
                    (reinterpret_cast<FightActorSamplePoints *>(actor)->positions[sample].x - g_pFightStage->spawnPoints[actor->nearestSpawnPoint].x) +
                (reinterpret_cast<FightActorSamplePoints *>(actor)->positions[sample].z - g_pFightStage->spawnPoints[actor->nearestSpawnPoint].z) *
                    (reinterpret_cast<FightActorSamplePoints *>(actor)->positions[sample].z - g_pFightStage->spawnPoints[actor->nearestSpawnPoint].z));
        if (distance < nearest) {
            nearest = distance;
            x = reinterpret_cast<FightActorSamplePoints *>(actor)->positions[sample].x;
            z = reinterpret_cast<FightActorSamplePoints *>(actor)->positions[sample].z;
        }
    }
    MOTION_VALUE(float, actor, 0x158) = x;
    MOTION_VALUE(float, actor, 0x160) = z;
}

#else

// Type-eight authored floor escape/reaction selection.
// @verified - p
void vfUpdateFightFloorReaction(VFActor *self) {
    VFMA_t *const game = self->game;
    VFMA_t *const current = vfmaGetTaskParam();
    const bool authored = current && (current->mode & 0x80000);
    if (static_cast<std::uint32_t>(game->state) < 3 || game->type != 8 || (authored && current->parameter570 < 5))
        return;
    VFActor *const paired = self->pairedActor;
    if (authored && (self->hitStateFlags & VFHSF_MidAirRecovery) && !(paired->stateFlags & 0x80) && !(paired->moveFlags & 0x20))
        return;
    const bool playerEscape = self->groupIndex == game->playerSlot && (self->hitStateFlags & VFHSF_AirborneOrDowned) && !authored;
    const Vector3 point{(ActorValue<float>(self, 0x10c) + ActorValue<float>(self, 0xf4)) * 0.5f * 0.3f + ActorValue<float>(self, 0xac) * 0.7f, self->position.y,
        (ActorValue<float>(self, 0x114) + ActorValue<float>(self, 0xfc)) * 0.5f * 0.3f + ActorValue<float>(self, 0xb4) * 0.7f};
    if (vfIsWithinFightFloor(point))
        self->stateFlags &= ~VFSF_Unknown100;
    else
        self->stateFlags |= VFSF_Unknown100;
    if (self->stateFlags & VFSF_Unknown80) {
        if (mtwkSetRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody) && FloorWinner(game) == static_cast<std::int8_t>(self->groupIndex))
            self->moveFlags |= VFMF_ForceDefeat;
        return;
    }
    auto ground = std::uint32_t(self->groundFlags);
    if (self->moveFlags & VFMF_MirrorReactionAngle) {
        constexpr std::uint32_t swaps[4][2]{{8, 64}, {16, 128}, {1024, 8192}, {2048, 16384}};
        ground &= 0x9327;
        for (const auto &swap : swaps) {
            if (self->groundFlags & swap[0])
                ground |= swap[1];
            if (self->groundFlags & swap[1])
                ground |= swap[0];
        }
    }
    const auto inverse = static_cast<std::uint16_t>(~ground);
    const auto &spawn = g_pFightStage->spawnPoints[self->nearestSpawnPoint];
    const auto quadrant = static_cast<std::uint16_t>((FixedAtan2(point.x - spawn.x, point.z - spawn.z) - 24576) & 0xc000);
    const auto back = static_cast<std::uint16_t>(
        (quadrant - ((self->hitStateFlags & VFHSF_MidAirRecovery) ? self->facingYaw : self->temporaryFacingAngle) + 0x4000) & 0x8000);
    const auto hit = self->hitStateFlags;
    const auto contact = self->parameter45a;
    std::int32_t choice;
    bool bypassPlayerEscape = false;
    if ((contact & 0x4800) && !(contact & 0xb7ff) && !(hit & 0x18000)) {
        if (!(inverse & contact & 0x4800) && !(self->stateFlags & VFSF_Unknown100))
            return;
        choice = back != 0 ? 1 : (hit & 0x80000) ? 14 : 0;
    } else if (((hit & 0x84000) != 0x84000 && !(contact & self->footContactState)) ||
        ((hit & 0x1010) && !(self->groundOffset + game->floorIntersectionY >= self->contactPosition.y))) {
        if (ground)
            return;
        if (FloorWinner(game) == -1) {
            if (playerEscape) {
                vfStartAuthoredDefeatCamera(self->secondActor);
                self->stateFlags &= ~VFSF_Unknown100;
                return;
            }
            BeginFloorDefeat(self);
            self->contactVectors[2].x = self->contactPosition.x;
            self->contactVectors[2].z = self->contactPosition.z;
            self->parameter370 = 0x19000000;
            self->hitPhase = 1;
            return;
        }
        choice = 1;
        bypassPlayerEscape = true;
    } else {
        auto edges = static_cast<std::uint8_t>((self->stateFlags >> 7) & 2);
        if (inverse & 0x800)
            ++edges;
        if (inverse & 0x4000)
            ++edges;
        if (edges <= 1)
            return;
        if ((hit & 0xc0000) && static_cast<std::uint8_t>(self->footFacingMode) != 2)
            choice = FloorWinner(game) == -1 && !back ? 14 : 1;
        else {
            if ((inverse & 0x4800) == 0x4800 && !(inverse & 4))
                choice = 2;
            else if ((inverse & 0x4800) != 0x4800 && (inverse & 4))
                choice = 4;
            else if ((ground & 0x4890) == 0x4080)
                choice = 10;
            else if ((ground & 0x4890) == 0x810)
                choice = 12;
            else
                return;
            if (!(hit & 0x400) && (hit & 0x800) && !(ActorValue<float>(self, 0x280) > 0))
                ++choice;
        }
    }
    if (playerEscape && !bypassPlayerEscape) {
        vfStartAuthoredDefeatCamera(self->secondActor);
        self->stateFlags &= ~VFSF_Unknown100;
        return;
    }
    constexpr std::uint32_t animations[15]{279, 278, 271, 273, 270, 272, 275, 277, 274, 276, 99, 32868, 32867, 100, 672};
    constexpr std::uint16_t turns[15]{0, 0x8000, 0, 0, 0x8000, 0x8000, 0, 0, 0x8000, 0x8000, 0x4000, 0xc000, 0xc000, 0x4000, 0x8000};
    self->moveFlags |= VFMF_HitReactionLock;
    BeginFloorDefeat(self);
    const auto vertical = game->floorIntersectionY - self->position.y;
    self->position.y += vertical;
    self->contactPosition.y += vertical;
    self->pendingAnimation = animations[choice];
    self->knockbackAcceleration = g_currentMap.name == SHEN_NAME(QLBT) ? 0.5f : self->scale;
    if (FloorWinner(self->game) != static_cast<std::int8_t>(self->groupIndex))
        self->knockbackAcceleration *= 0.7f;
    if ((self->hitStateFlags & VFHSF_MidAirRecovery) && !(paired->activeMotionFlags & 0x80) && !(paired->stateFlags & 0x80) &&
        !(paired->hitStateFlags & 0x4000))
        paired->parameter370 = 0x01000000;
    vfApplyHitReactionAndStartMotion(self);
    self->parameter370 = 0x19000000;
    self->hitPhase = 1;
    self->facingAngle = self->parameter1b6 = self->facingYaw = self->temporaryFacingAngle = static_cast<std::uint16_t>(quadrant + turns[choice]);
    float best = 9999, x = self->position.x, z = self->position.z;
    for (std::size_t i = 0; i != 10; ++i) {
        std::uint16_t tag;
        std::memcpy(&tag, self->parameterAd8 + i * 20 + 18, sizeof(tag));
        if (!(tag & ActorValue<std::uint16_t>(self, 0x45e)))
            continue;
        const float sx = self->motionContactSamples[i].x, sz = self->motionContactSamples[i].z;
        const float distance = std::sqrt((sx - spawn.x) * (sx - spawn.x) + (sz - spawn.z) * (sz - spawn.z));
        if (best > distance) {
            best = distance;
            x = sx;
            z = sz;
        }
    }
    self->contactVectors[2].x = x;
    self->contactVectors[2].z = z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void BeginFightFloorDefeat(FightActorRuntimePrefix *actor) {
    actor->stateFlags |= 0x80U;
    if (FindFightFloorWinner(actor->game) != -1)
        return;
    actor->stateFlags |= 0x8000U;
    if (g_pFightStage->battleFlags & 0x8000U) {
        if (MOTION_VALUE(std::int8_t, actor, 0) == actor->game->playerSlot)
            vfAddRoundResults(0, 1);
        else
            vfAddRoundResults(1, 0);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t FindFightFloorWinner(FightRuntimePrefix *game) {
    std::int32_t count = 0;
    std::int32_t index;
    for (index = 0; index < 6; ++index) {
        if (game->characters[index]) {
            ++count;
            if (game->actors[index]->stateFlags & 0x8000U)
                goto found;
        }
    }
    if (count < 2)
        goto alone;
    return -1;
found:
    return index;
alone:
    return 99;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfUpdateMotionTargetPointAndParams(FightActorRuntimePrefix *actor) {
    const std::uint32_t hit = actor->hitStateFlags, active = actor->activeMotionFlags;
    MotionWork *const motion = actor->motion;
    std::int32_t target;
    Vector3 bone, focus;
    if (MOTION_VALUE(std::uint16_t, actor->actor, 2) == 1 || (actor->moveFlags & 0x800000U) || (actor->game->controlFlags & 0x20000000U) ||
        actor->contactDistances[MOTION_VALUE(std::int8_t, actor, 1)] > actor->game->contactRange || (active & 0xc00U) || (hit & 0xfc010U) ||
        (actor->game->stateFlags & 1U))
        target = 0;
    else if ((hit & 1U) && (active & 0x10000U))
        target = 1;
    else if (!(hit & 0x101U))
        target = 1;
    else if (active & 0x200000U)
        target = 1;
    else
        target = (MOTION_VALUE(std::uint16_t, actor, 2) == 30 &&
            std::fabs(static_cast<float>(
                actor->actorFacingAngles[MOTION_VALUE(std::int8_t, actor, 1)] - static_cast<std::int16_t>(actor->temporaryFacingAngle))) < 8192)
            ? 1 : 0;
    if (target) {
        charaGetMotWorldPos(MOTION_VALUE(Character *, actor, 0x10), 5, &bone);
        charaLatchBoneTarget(actor->character, 7, &bone);
    } else if (dialoguePlayerFocus) {
        focus.x = 0;
        focus.y = 1.3f;
        focus.z = -1;
        mtwkSetConstraintAngle(motion, 0x8000);
        mtwkSetBoneTarget(motion, 7, &focus);
    } else
        mtwkSetConstraintAngle(motion, 0x1000);
}

#else

// Publish the live opponent's leg position to the native head controller.
// @verified - p
void vfUpdateMotionTargetPointAndParams(VFActor *self) {
    const auto hit = self->hitStateFlags;
    const auto active = self->activeMotionFlags;
    VFMA_t *const game = self->game;
    const auto target = self->actionIndex;
    if (self->actor->fightStyle != 1 && !(self->moveFlags & VFMF_Unknown800000) && !(game->gameFlags & 0x20000000) &&
        !(self->contactDistances[target] > game->contactRange) && !(active & 0xc00) && !(hit & 0xfc010) && !(game->mode & 1) &&
        g_currentMap.name !=
            SHEN_NAME(GABT) &&
                (((hit & 1) && (active & 0x10000)) || !(hit & 0x101) || (active & 0x200000) ||
                    (self->fightStyle == 30 &&
                        std::abs(int(self->contactAngles[target]) - int(static_cast<std::int16_t>(self->temporaryFacingAngle))) < 8192))) {
        mtwkSetAngle(self->motion, 2048);
        Vector3 position;
        charaGetMotWorldPos(self->targetingCharacter, 5, &position);
        mtwkLatchBoneTarget(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(self->character), 7, &position);
        return;
    }
    if (self->savedMarkerMask & 2)
        mtwkSetAngle(self->motion, 0x8000);
    else if (vfHasDialoguePlayerFocus()) {
        const Vector3 position{0, 1.3f, -1};
        mtwkSetAngle(self->motion, 0x8000);
        mtwkSetBoneTarget(self->motion, 7, &position);
    } else
        mtwkSetAngle(self->motion, 4096);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateRootBlendEnvelope(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const other = actor->actor;
    if (!(actor->motionFlags & 0x100U)) {
        actor->groundReactionScale = 0;
        return;
    }
    const float dx = actor->contactPosition.x - other->contactPosition.x;
    const float dz = actor->contactPosition.z - other->contactPosition.z;
    float distance;
    distance = NativeSqrt(dx * dx + dz * dz);
    float envelope;
    if (!(distance > 1))
        envelope = 1;
    else if (!(2 > distance))
        envelope = 0;
    else
        envelope = 2 - distance;
    const float middle = static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x292));
    float scale;
    if (actor->animationTime < static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x290)))
        scale = actor->animationTime / static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x290));
    else if (actor->animationTime > middle)
        scale = 1 - (actor->animationTime - middle) / (static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x28c)) - middle);
    else
        scale = 1;
    actor->groundReactionScale = scale * envelope;
}

#else

// The distance envelope lives in the actor's groundReactionScale word.
// @verified - p
void vfUpdateRootBlendEnvelope(VFActor *self) {
    if (!(self->motionFlags & VFMOF_Unknown100)) {
        self->groundReactionScale = 0;
        return;
    }
    const float x = self->contactPosition.x - self->actor->contactPosition.x;
    const float z = self->contactPosition.z - self->actor->contactPosition.z;
    const float distance = std::sqrt(x * x + z * z);
    const float envelope = distance <= 1 ? 1 : (!(distance >= 2) ? 2 - distance : 0);
    const float first = float(static_cast<std::uint16_t>(self->parameter2d0));
    const float middle = float(static_cast<std::uint16_t>(self->reactionMiddleFrame));
    if (self->animationTime < first)
        self->groundReactionScale = (self->animationTime / first) * envelope;
    else if (!(self->animationTime > middle))
        self->groundReactionScale = 1 * envelope;
    else
        self->groundReactionScale = (1 - (self->animationTime - middle) / (float(self->currentAnimationEndFrame) - middle)) * envelope;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mtwkRemoveCallback(MotionWork *motion) {
    for (std::int32_t index = 0; index < 7; ++index)
        mtwkAssignTargetCallback(motion, 0, fightMotionSelectors[index]);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdatePositionXZ(FightActorRuntimePrefix *actor) {
    if (actor->game->type == 4 && !MOTION_VALUE(std::uint16_t, actor, 2) && actor->animationTime == 0 &&
        ((actor->secondaryMotionFlags & 0x8000000U) || (actor->motionFlags & 0x1010U))) {
        Vector3 offset;
        Vector3 delta;
        offset.x = 0;
        offset.z = -3;
        vfRotateFightVector(-static_cast<std::int16_t>(actor->facingAngle), &offset, &delta);
        actor->actor->position.x = actor->position.x + delta.x;
        actor->actor->position.z = actor->position.z + delta.z;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfForceActorDefeat(FightActorRuntimePrefix *actor) {
    if (!(actor->hitStateFlags & 0x814000U)) {
        mtwkSetExtraFlag(actor->motion, 0, 0);
        mtwkRemoveCallback(actor->motion);
        vfUpdateDefaultReactionAnim(actor->game->enemySlots[MOTION_VALUE(std::int8_t, actor, 0)], 198);
        MOTION_VALUE(std::int32_t, actor, 0x434) = 198;
        vfApplyMotionDescriptor(actor);
    }
    figpAssignModeForCharacter(actor->character, 0);
    actor->game->stateFlags |= 0x20U;
    MOTION_VALUE(std::uint32_t, actor, 0x318) = 0x24000000U;
    MOTION_VALUE(std::uint8_t, actor, 0x27c) = 2;
}

#else

// Only a real actor's installed motion and native target callbacks are changed.
// @verified - p
void vfForceActorDefeat(VFActor *self) {
    if (!(self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState | VFHSF_AlignedToGround))) {
        mtwkSetExtraFlag(self->motion, 0, 0);
        mtwkRemoveCallback(self->motion);
        vfUpdateDefaultReactionAnim(self->game->enemySlots[self->groupIndex], 198);
        self->pendingAnimation = 198;
        vfApplyMotionDescriptor(self);
    }
    figpAssignModeForCharacter(self->character, 0);
    self->game->mode |= 0x20;
    self->parameter370 = 0x24000000;
    self->hitPhase = 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void vfApplyScriptedFormationMotion(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    if (game->stateFlags & 0x800U) {
        if (game->formations[MOTION_VALUE(std::int8_t, actor, 0)]->activationFlags) {
            MOTION_VALUE(std::uint32_t, actor, 0x434) = game->formations[MOTION_VALUE(std::int8_t, actor, 0)]->activationFlags;
            vfStartFormationAnimation(actor);
            game->stateFlags |= 1U;
        }
    } else
        game->formations[MOTION_VALUE(std::int8_t, actor, 0)]->activationFlags = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfResetScriptedFormationMotion(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    if (game->stateFlags & 0x800U) {
        actor->formationFlags &= ~0x8000U;
        vfReleaseUnheldScriptedFormation(game);
    }
}

#else

// Clear the shared scripted-wave lock only after every actor releases it.
// @verified - p
void vfResetScriptedFormationMotion(VFActor *self) {
    VFMA_t *const game = self->game;
    if (!(game->mode & 0x800))
        return;
    self->formationFlags &= ~0x8000;
    std::uint32_t flags = 0;
    for (std::uint8_t index = 0; index < 6; ++index)
        if (game->characters[index])
            flags |= game->actors[index]->formationFlags;
    if (!(flags & 0x8000))
        game->mode &= 0xfffff7fe;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfReleaseUnheldScriptedFormation(FightRuntimePrefix *game) {
    std::uint32_t formations = 0;
    for (std::uint8_t index = 0; index < 6; ++index)
        if (game->characters[index])
            formations |= game->actors[index]->formationFlags;
    if (!(formations & 0x8000U))
        game->stateFlags &= 0xfffff7feU;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t vfFindNearestFightSpawn(FightActorRuntimePrefix *actor) {
    float minimum = 9999;
    FightRuntimePrefix *const game = actor->game;
    std::int32_t result = 0;
    if (game->spawnPointCount > 1) {
        for (std::int32_t index = 0; index < game->spawnPointCount; ++index) {
            const float distance =
                NativeSqrt((game->spawnPoints[index].x - actor->contactPosition.x) * (game->spawnPoints[index].x - actor->contactPosition.x) +
                    (game->spawnPoints[index].z - actor->contactPosition.z) * (game->spawnPoints[index].z - actor->contactPosition.z));
            if (distance < minimum) {
                minimum = distance;
                result = index;
            }
        }
    }
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfStartFormationAnimation(FightActorRuntimePrefix *actor) {
    vfApplyHitReactionAndStartMotion(actor);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfStartReactionAnimation(FightActorRuntimePrefix *actor) {
    vfApplyHitReactionAndStartMotion(actor);
}

void vfStartQueuedReactionAnimation(FightActorRuntimePrefix *actor) {
    vfApplyHitReactionAndStartMotion(actor);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

struct FightActorDefaultMotions {
    std::byte retained000[0x9b6];
    std::uint16_t motions[3];
};

// @verified - d
void vfSelectDefaultMotion(FightActorRuntimePrefix *actor, std::int32_t index) {
    actor->targetPosition.x = actor->position.x;
    actor->targetPosition.y = actor->position.y;
    actor->targetPosition.z = actor->position.z;
    MOTION_VALUE(std::uint8_t, actor, 0x94a) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x864) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x434) = reinterpret_cast<FightActorDefaultMotions *>(actor)->motions[index];
    vfApplyHitReactionAndStartMotion(actor);
}

#else

// The three defaults are actor-owned authored animation words.
// @verified - p
void vfSelectDefaultMotion(VFActor *self, std::uint32_t index) {
    self->targetPosition = self->position;
    self->cameraMotionFlags = 0;
    self->reactionDepth = 0;
    self->pendingAnimation = self->defaultAnimations[index];
    vfApplyHitReactionAndStartMotion(self);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Shared native predicate (reaction predicates 40, 41, 44, 50, 51 and 54, cooldown-quadrant refresh): constant -1 on PC.
// @verified - p
std::int8_t vfUpdatePredicate_40_41_44_50_51_54() {
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfApplyHitReactionAndStartMotion(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(float, actor, 0x464) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x27e) = 0;
    actor->moveFlags &= ~0x200U;
    std::uint32_t hit = actor->hitStateFlags;
    if (!MOTION_VALUE(std::uint32_t, actor, 0x434)) {
        actor->hitStateFlags = hit & 0xff7ebfb7U;
        MOTION_VALUE(std::uint16_t, actor, 0x42c) = 0;
        return;
    }
    actor->moveFlags |= 0x80800U;
    MOTION_VALUE(std::uint16_t, actor, 0x42e) = MOTION_VALUE(std::uint16_t, actor, 0x42c);
    if (MOTION_VALUE(std::uint32_t, actor, 0x434) & 0x4000U)
        hit &= 0xffdfffbfU;
    static const std::uint16_t hitReactionAnimations[] = {0};
    std::uint16_t motion = MOTION_VALUE(std::uint16_t, actor, 0x434) & 0xfffU;
    for (std::uint8_t index = 0; hitReactionAnimations[index]; ++index)
        if (hitReactionAnimations[index] == motion) {
            actor->moveFlags |= 0x20000000U;
            break;
        }
    if (hit & 0x40U) {
        if ((hit & 0x4800U) == 0x4000U)
            actor->facingAngle = actor->facingYaw;
        else
            actor->facingAngle = static_cast<std::uint16_t>(actor->facingAngle + actor->facingAngleDelta);
    }
    if (hit & 0x800000U)
        actor->position.y = actor->targetGroundY;
    if (hit & 0x200000U)
        actor->moveFlags ^= 0x40U;
    if (MOTION_VALUE(std::uint32_t, actor, 0x434) & 0x8000U)
        actor->moveFlags ^= 0x40U;
    motion = MOTION_VALUE(std::uint32_t, actor, 0x434);
    MOTION_VALUE(std::uint16_t, actor, 0x42c) = motion;
    actor->animationTime = 0;
    mtwkSetExtraFlag(actor->motion, 0, 0);
    MOTION_VALUE(float, actor, 0x450) = actor->animationTime;
    actor->moveFlags &= ~1U;
    vfApplyMotionDescriptor(actor);
    if (IsAuthFightCamera() && (actor->motionFlags & 0xc0000U)) {
        if (MOTION_VALUE(float, actor, 0x278) == 0)
            MOTION_VALUE(float, actor, 0x278) = 1;
        vfUpdateMoveVelocityWithKnockback(actor, MOTION_VALUE(float, actor, 0x278) * 0.5f);
        MOTION_VALUE(float, actor, 0x278) = 0;
    } else if (MOTION_VALUE(float, actor, 0x278) != 0) {
        vfUpdateMoveVelocityWithKnockback(actor, MOTION_VALUE(float, actor, 0x278));
        MOTION_VALUE(float, actor, 0x278) = 0;
    } else if (MOTION_VALUE(float, actor, 0x26c) != MOTION_VALUE(float, actor, 0x274))
        vfUpdateMoveVelocityWithKnockback(actor, MOTION_VALUE(float, actor, 0x274));
    vfRescaleReactionFrameWindows(actor);
    std::uint32_t flags = actor->hitStateFlags;
    if ((motion & 0x8000U) && (actor->motionFlags & 0x200000U))
        flags |= 0x200000U;
    if (flags & 0x100U)
        ++MOTION_VALUE(std::uint16_t, actor, 0x3d4);
    if (flags & 0x800000U)
        actor->facingAngle = actor->facingYaw;
    if (flags & 0x80U)
        mtwkSetExtraFlag(actor->motion, 13, 0);
    actor->hitStateFlags = flags;
    if (mtwkSetExtraFlag(actor->motion, 1, 1))
        mtwkSetExtraFlag(actor->motion, 1, 2);
    MOTION_VALUE(std::uint16_t, actor, 0x44c) = MOTION_VALUE(std::uint16_t, actor, 0x42c);
    if (actor->moveFlags & 0x10U)
        MOTION_VALUE(std::uint16_t, actor, 0x5c8) = 0;
    else if (flags & 0x100U) {
        motion = MOTION_VALUE(std::uint16_t, actor, 0x5be);
        MOTION_VALUE(std::uint16_t, actor, 0x5be) = 0;
        if (!motion)
            MOTION_VALUE(std::uint16_t, actor, 0x5c8) = motion;
        else {
            MOTION_VALUE(std::uint16_t, actor, 0x5c8) = motion;
            MOTION_VALUE(std::uint32_t, actor, 0x5cc) = MOTION_VALUE(std::uint32_t, actor, 0x5c4);
        }
    }
    if (actor->motionFlags & 0x100U)
        actor->attackerGroundClearance = actor->actor->groundClearance;
    else
        actor->attackerGroundClearance = 0;
    if (!(actor->activeMotionFlags & 0x200000U) && (actor->stateFlags & 0x800U))
        actor->stateFlags &= ~0x800U;
    std::int32_t material;
    if (g_pFightStage->battleFlags & 0x40U)
        material = actor->game->cameraParameter;
    else
        material = static_cast<std::int32_t>(GetFieldSoundProperties(&actor->position)) >> 16;
    mtwkSetReactFlags(actor->motion, material);
}

#else

// The body behind the native vfApplyHitReactionAndStartMotion thunk: install the authored motion and restore native hit state.
// @verified - p
void vfApplyHitReactionAndStartMotion(VFActor *self) {
    self->moveFlags &= ~VFMF_Unknown200;
    auto hit = self->hitStateFlags;
    self->slideVelocity.y = 0;
    self->queuedReactionEvent = 0;
    if (!self->pendingAnimation) {
        self->currentAnimation = 0;
        self->hitStateFlags = hit & 0xff7ebfb7;
        return;
    }
    self->reserved4a2 = self->currentAnimation;
    self->moveFlags |= (VFMF_Unknown800 | VFMF_Unknown80000);
    if (self->pendingAnimation & 0x4000)
        hit &= ~0x200040u;
    // g_VF_hit_react_anim_ids is unloaded, zero-initialized BSS with no producer
    // references in this S2 executable; its native terminated loop has no entries.
    if (hit & 0x40) {
        if ((hit & 0x4800) == 0x4000)
            self->facingAngle = self->facingYaw;
        else
            self->facingAngle = static_cast<std::uint16_t>(self->facingAngle + self->facingAngleDelta);
    }
    if (hit & 0x800000) {
        float height = flddGetIntersectionFor(self->character, self->contactPosition);
        if (g_currentMap.name == SHEN_NAME(Q100)) {
            if (VFMA_t *game = vfmaGetTaskParam())
                height = game->floorIntersectionY;
        }
        self->position.y = height;
    }
    if (hit & 0x200000)
        self->moveFlags ^= VFMF_MirrorReactionAngle;
    if (self->pendingAnimation & 0x8000)
        self->moveFlags ^= VFMF_MirrorReactionAngle;
    const auto pending = static_cast<std::uint16_t>(self->pendingAnimation);
    self->currentAnimation = pending;
    self->animationTime = 0;
    mtwkSetExtraFlag(self->motion, 0, 0);
    self->moveFlags &= ~VFMF_NeedsAnimationStep;
    self->parameter4c8 = self->animationTime;
    vfApplyMotionDescriptor(self);
    VFMA_t *game = vfmaGetTaskParam();
    if (game && (game->mode & 0x80000) && (self->motionFlags & (VFMOF_Unknown40000 | VFMOF_Unknown80000))) {
        if (self->knockbackAcceleration == 0)
            self->knockbackAcceleration = 1;
        vfUpdateMoveVelocityWithKnockback(self, self->knockbackAcceleration * 0.5f);
        self->knockbackAcceleration = 0;
    } else if (self->knockbackAcceleration != 0) {
        vfUpdateMoveVelocityWithKnockback(self, self->knockbackAcceleration);
        self->knockbackAcceleration = 0;
    } else if (self->scale != self->timeScale) {
        vfUpdateMoveVelocityWithKnockback(self, self->timeScale);
    }
    vfRescaleReactionFrameWindows(self);
    hit = self->hitStateFlags;
    if (static_cast<std::int16_t>(pending) < 0 && (self->motionFlags & VFMOF_Unknown200000))
        hit |= 0x200000;
    if (hit & 0x100)
        ++self->lastHitMove;
    if (hit & 0x800000)
        self->facingAngle = self->facingYaw;
    if (hit & 0x80)
        mtwkSetExtraFlag(self->motion, 13, 0);
    self->hitStateFlags = hit;
    if (mtwkSetExtraFlag(self->motion, 1, 1))
        mtwkSetExtraFlag(self->motion, 1, 2);
    self->parameter4c4 = self->currentAnimation;
    if (self->moveFlags & VFMF_HitReactionLock)
        self->currentHitAction &= ~std::uint64_t{0xffff};
    else if (hit & 0x100) {
        const auto move = self->pendingHitMove;
        self->pendingHitMove = 0;
        self->currentHitAction = (self->currentHitAction & ~std::uint64_t{0xffff}) | move;
        if (move)
            self->savedHitAction = self->pendingHitAction;
    }
    self->reservedA44 = (self->motionFlags & VFMOF_Unknown100) ? self->actor->actorRuntimeFlags : 0;
    if (!(self->activeMotionFlags & VFSMF_Unknown200000))
        self->stateFlags &= ~VFSF_Unknown800;
    // FindClosestIntersection_0 never reads its historical xmm0 maxY input.
    auto material = (g_pFightStage->battleFlags & 0x40) ? static_cast<std::uint8_t>(self->game->cameraParameter) :
                                                          static_cast<std::uint8_t>(QueryGroundMaterialToken(self->position, 0) >> 16);
    if (material > 13)
        material = 13;
    if (self->motion)
        self->motion->runtime.environmentFlags |= material & 0xf;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfApplyMotionDescriptor(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(std::uint16_t, actor, 0x28e) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x288) = 0;
    actor->facingAngleDelta = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x2e7) = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x2c8) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x2c2) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x2ab) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x2d6) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x2a0) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x29e) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x2da) = 0;
    MOTION_VALUE(std::uint8_t, actor, 0x305) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x2dc) = 0;
    MOTION_VALUE(std::uint16_t, actor, 0x3a6) = 0;
    std::uint16_t id = MOTION_VALUE(std::uint16_t, actor, 0x434);
    if (!id)
        return;
    id &= 0xfffU;
    const std::byte *marker;
    motionFrameCursor = marker = MOTN_GetMarkerData(id);
    if (!marker)
        return;
    actor->savedHitStateFlags = actor->hitStateFlags;
    actor->savedMotionFlags = actor->motionFlags;
    actor->savedSecondaryMotionFlags = actor->secondaryMotionFlags;
    actor->markerMask = 0;
    actor->retained350 = 0;
    actor->motionFlags = *reinterpret_cast<const std::uint32_t *>(marker);
    actor->secondaryMotionFlags = *reinterpret_cast<const std::uint32_t *>(marker + 8);
    MOTION_VALUE(std::uint16_t, actor, 0x298) = *reinterpret_cast<const std::uint16_t *>(marker + 4);
    MOTION_VALUE(std::uint16_t, actor, 0x29c) = *reinterpret_cast<const std::uint16_t *>(marker + 6);
    motionFrameCursor = marker + 12;
    std::uint32_t flags = actor->motionFlags;
    flags &= ~0x200000U;
    if (flags & 0x40U)
        actor->facingAngleDelta = static_cast<std::int16_t>(0x8000);
    if ((flags & 0x8100U) == 0x8100U)
        flags = (flags & ~0x8000U) | 0x10000U;
    actor->hitStateFlags = flags;
    actor->activeMotionFlags = actor->secondaryMotionFlags;
    std::uint16_t end = GetMotionFrameCount(id) - 1;
    if (actor->motionFlags & 0x20U)
        --end;
    MOTION_VALUE(std::uint16_t, actor, 0x28c) = end;
    MOTION_VALUE(float, actor, 0x43c) = static_cast<float>(end) - MOTION_VALUE(float, actor, 0x26c);
    MOTION_VALUE(std::uint16_t, actor, 0x292) = end;
    MOTION_VALUE(std::uint16_t, actor, 0x294) = end;
    MOTION_VALUE(std::uint16_t, actor, 0x290) = 0;
    if (!figpGetFlagMask(actor->character, 0))
        figpAssignModeForCharacter(actor->character, 0);
    vfMotnDispatch(actor);
}

#else

// Marker state is borrowed directly from the registered MOTI bank.
// @verified - p
void vfApplyMotionDescriptor(VFActor *self) {
    self->requestedAnimation = 0;
    self->actionTriggerPhase = 0;
    self->facingAngleDelta = 0;
    self->reactionEventCount = 0;
    self->reactionEntryFlags = 0;
    self->eventFrame30a = 0;
    self->parameter2eb = 0;
    self->parameter31e = self->parameter2de = self->parameter2e0 = 0;
    self->someAnimation[0] = self->someAnimation[1] = 0;
    self->parameter34d = 0;
    self->parameter412 = 0;
    const auto pending = static_cast<std::uint16_t>(self->pendingAnimation);
    if (!pending)
        return;
    const auto id = pending & 0xfff;
    motionMarkerCursor = FindLoadedMotionMetadata(id).data();
    if (!motionMarkerCursor)
        return;
    self->savedHitStateFlags = self->hitStateFlags;
    self->savedMotionFlags = self->motionFlags;
    self->savedSecondaryMotionFlags = self->secondaryMotionFlags;
    self->markerMask = self->savedMarkerMask = 0;
    self->motionFlags = MarkerValue<std::uint32_t>(0);
    self->secondaryMotionFlags = MarkerValue<std::uint32_t>(8);
    self->animationHalfPhase = MarkerValue<std::int8_t>(4);
    self->parameter2d9 = MarkerValue<std::int8_t>(5);
    self->parameter2dc = MarkerValue<std::uint16_t>(6);
    motionMarkerCursor += 12;
    auto flags = self->motionFlags & ~VFMOF_Unknown200000;
    if (self->motionFlags & VFMOF_Unknown40)
        self->facingAngleDelta = static_cast<std::int16_t>(0x8000);
    if ((flags & 0x8100) == 0x8100)
        flags = (flags & 0xfffe7fff) | 0x10000;
    self->hitStateFlags = (flags & 0x80000) ? flags & ~0x4000u : flags;
    self->activeMotionFlags = self->secondaryMotionFlags;
    const auto *sequence = FindLoadedMotionSequence(id);
    const auto frames = sequence ? sequence->frameCount : 0;
    const auto end = static_cast<std::uint16_t>(frames - 1 - ((self->motionFlags & VFMOF_Unknown20) ? 1 : 0));
    self->parameter2d0 = 0;
    self->currentAnimationEndFrame = self->reactionEndFrame = end;
    self->reactionMiddleFrame = static_cast<std::int16_t>(end);
    self->animationTimeParameter = float(end) - self->scale;
    vfMotnDispatch(self);
    auto *figure = static_cast<FigurePose *>(
#if SHENMUE_IS_DC
charFindComp
#else
FindModelComponent
#endif
(self->character, MODEL_FIGURE_POSE));
    if ((!figure || !(figure->flags & 1)) && !(self->savedMarkerMask & 4))
        figpAssignModeForCharacter(self->character, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfRescaleReactionFrameWindows(FightActorRuntimePrefix *actor) {
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x290));
    if (MOTION_VALUE(std::uint16_t, actor, 0x292) != MOTION_VALUE(std::uint16_t, actor, 0x28c))
        DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x292));
    if (MOTION_VALUE(std::uint16_t, actor, 0x294) != MOTION_VALUE(std::uint16_t, actor, 0x28c))
        DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x294));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2c2));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2c4));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2d0));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2e0));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x296));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x29e));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2a0));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2d6));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2a2));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2d4));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x2f6));
    DecrementFightFrame(&MOTION_VALUE(std::uint16_t, actor, 0x306));
    const std::uint16_t first = MOTION_VALUE(std::uint16_t, actor, 0x290);
    std::uint16_t next = MOTION_VALUE(std::uint16_t, actor, 0x292);
    std::uint16_t last = MOTION_VALUE(std::uint16_t, actor, 0x294);
    if (first >= next) {
        const std::uint16_t end = MOTION_VALUE(std::uint16_t, actor, 0x28c);
        last = static_cast<std::uint16_t>(last - next);
        next = static_cast<std::uint16_t>(first + 1U);
        if (next > end)
            next = end;
        last = static_cast<std::uint16_t>(last + next);
        if (last > end)
            last = end;
        MOTION_VALUE(std::uint16_t, actor, 0x292) = next;
        MOTION_VALUE(std::uint16_t, actor, 0x294) = last;
    }
}

// @verified - d
static void DecrementFightFrame(std::uint16_t *frame) {
    const std::int32_t value = *frame;
    if (value)
        *frame = static_cast<std::uint16_t>(value - 1);
}

FightActorRuntimePrefix *vfFindEnemySlotFighter(Character *character) {
    FightActorRuntimePrefix *fighter = 0;
    std::uint8_t i = 0;
    FGST_t *const stage = g_pFightStage;
    for (; i < stage->enemyCount; ++i) {
        if (stage->enemySlots[i].character == character) {
            fighter = stage->enemySlots[i].fighter;
            break;
        }
    }
    return fighter;
}

#else

// Unsigned frame windows retain the native decrement/clamp ordering.
// @verified - p
void vfRescaleReactionFrameWindows(VFActor *self) {
    DecrementFrame(self->parameter2d0);
    if (static_cast<std::uint16_t>(self->reactionMiddleFrame) != self->currentAnimationEndFrame)
        DecrementFrame(self->reactionMiddleFrame);
    if (self->reactionEndFrame != self->currentAnimationEndFrame)
        DecrementFrame(self->reactionEndFrame);
    DecrementFrame(self->eventFrame30a);
    DecrementFrame(self->eventFrame30c);
    DecrementFrame(self->eventFrame318);
    DecrementFrame(self->eventFrame328);
    DecrementFrame(self->parameter2d6);
    DecrementFrame(self->parameter2de);
    DecrementFrame(self->parameter2e0);
    DecrementFrame(self->parameter31e);
    DecrementFrame(self->parameter2e2);
    DecrementFrame(self->parameter31a[1]);
    DecrementFrame(self->parameter33c[1]);
    DecrementFrame(self->pairContactEndFrame);
    const auto first = static_cast<std::uint16_t>(self->parameter2d0);
    const auto middle = static_cast<std::uint16_t>(self->reactionMiddleFrame);
    if (first >= middle) {
        auto next = static_cast<std::uint16_t>(first + 1);
        if (next > self->currentAnimationEndFrame)
            next = self->currentAnimationEndFrame;
        auto last = static_cast<std::uint16_t>(next + static_cast<std::uint16_t>(self->reactionEndFrame - middle));
        if (last > self->currentAnimationEndFrame)
            last = self->currentAnimationEndFrame;
        self->reactionMiddleFrame = static_cast<std::int16_t>(next);
        self->reactionEndFrame = last;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
const std::byte *vfFindQueuedReactionEventEntry(FightActorRuntimePrefix *actor, std::uint8_t event) {
    std::int8_t count = MOTION_VALUE(std::int8_t, actor, 0x2e7);
    if (!count)
        return 0;
    const std::byte *entry = MOTION_VALUE(const std::byte *, actor, 0x2bc);
    for (; count; count = static_cast<std::int8_t>(count - 1), entry += 8) {
        if (static_cast<std::int8_t>(entry[1]) == event)
            return entry;
        if (event == 4 && static_cast<std::int8_t>(entry[1]) == 1)
            return entry;
    }
    return 0;
}

#else

// Event type 4 also accepts the authored type-1 entry.
// @verified - p
const std::byte *vfFindQueuedReactionEventEntry(VFActor *self, std::uint8_t eventType) {
    auto remaining = static_cast<std::uint8_t>(self->reactionEventCount);
    if (!remaining)
        return nullptr;
    for (const std::byte *entry = self->reactionEntries;; entry += 8) {
        const auto type = std::to_integer<std::int8_t>(entry[1]);
        if (type == eventType || (eventType == 4 && type == 1))
            return entry;
        if (!--remaining)
            return nullptr;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfSnapActorToContact(FightActorRuntimePrefix *actor) {
    if (!(actor->moveFlags & 4U)) {
        actor->position.y = actor->contactPosition.y;
        if ((MOTION_VALUE(std::uint8_t, actor, 0x2c0) & 1) == 0) {
            actor->position.x = actor->contactPosition.x;
            actor->position.z = actor->contactPosition.z;
        }
    }
    actor->moveFlags |= 0xcU;
    actor->hitStateFlags |= 0x10U;
    mtwkSetExtraFlag(actor->motion, 3, 0);
}

#else

// Authored contact flags decide whether the snap includes XZ.
// @verified - p
void vfSnapActorToContact(VFActor *self) {
    const auto move = self->moveFlags;
    if (!(move & 4)) {
        self->position.y = self->contactPosition.y;
        if (!(self->motionContactFlags & 1)) {
            self->position.x = self->contactPosition.x;
            self->position.z = self->contactPosition.z;
        }
    }
    self->hitStateFlags |= VFHSF_FallLandingRequired;
    self->moveFlags = move | 0xc;
    mtwkSetExtraFlag(self->motion, 3, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfFinishAirborneMotion(FightActorRuntimePrefix *actor) {
    if (!(actor->moveFlags & 4U))
        return;
    if ((MOTION_VALUE(std::uint8_t, actor, 0x2c0) & 1) == 0) {
        Vector3 delta;
        vfRotateFightVector(-static_cast<std::int16_t>(actor->facingAngle), &MOTION_VALUE(Vector3, actor, 0x4c), &delta);
        actor->position.x -= delta.x;
        actor->position.z -= delta.z;
    }
    MOTION_VALUE(float, actor, 0x50) = actor->contactPosition.y - actor->position.y;
    actor->moveFlags &= ~4U;
    actor->hitStateFlags &= ~0x10U;
    mtwkSetExtraFlag(actor->motion, 3, 2);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfRotateFightVector(std::int16_t yaw, const Vector3 *input, Vector3 *output) {
    Vector3 saved;
    saved.x = input->x;
    saved.y = input->y;
    saved.z = input->z;
    const float cosine = CalculateFixedCosine(yaw);
    output->x = saved.x * cosine - saved.z * CalculateFixedSine(yaw);
    output->y = saved.y;
    const float sine = CalculateFixedSine(yaw);
    output->z = sine * saved.x + saved.z * CalculateFixedCosine(yaw);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t ScaleFightTransitionFrames(float scale, std::uint16_t frames) {
    if (scale != 1 && frames != 1)
        return static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(frames) * scale + 0.5f));
    return frames;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateMoveVelocityWithKnockback(FightActorRuntimePrefix *actor, float scale) {
    mtwkSetSpeed(actor->motion, scale, static_cast<MotionRegion>(0));
    MOTION_VALUE(float, actor, 0x26c) = scale;
    MOTION_VALUE(float, actor, 0x270) = 1.0f / scale;
    const float ratio = MOTION_VALUE(float, actor, 0x278) / MOTION_VALUE(float, actor, 0x26c);
    const float squared = ratio * ratio;
    MOTION_VALUE(float, actor, 0x454) *= ratio;
    MOTION_VALUE(float, actor, 0x458) *= squared;
    MOTION_VALUE(float, actor, 0x45c) *= ratio;
    MOTION_VALUE(float, actor, 0x460) *= ratio;
    MOTION_VALUE(float, actor, 0x464) *= squared;
    MOTION_VALUE(float, actor, 0x468) *= ratio;
    MOTION_VALUE(float, actor, 0x46c) *= ratio;
    MOTION_VALUE(float, actor, 0x470) *= squared;
    MOTION_VALUE(float, actor, 0x474) *= ratio;
}

#else

// Native numerator is knockbackAcceleration, not the old scale.
// @verified - p
void vfUpdateMoveVelocityWithKnockback(VFActor *self, float acceleration) {
    mtwkSetSpeed(self->motion, acceleration, MotionRegion::LowerBody);
    const float ratio = self->knockbackAcceleration / acceleration;
    self->inverseScale = 1.0f / acceleration;
    self->scale = acceleration;
    for (Vector3 *velocity : {&self->knockbackVelocity, &self->slideVelocity, &self->pendingKnockback}) {
        velocity->x *= ratio;
        velocity->y *= ratio * ratio;
        velocity->z *= ratio;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void DecrementFightFrame(std::uint16_t *frame);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float g_fightFloorExtent = 10.0f;

} // namespace

// @verified - d
bool vfIsWithinFightFloor(const Vector3 &position) {
    Vector3 query;
    const std::uint32_t map = g_currentMap;
    if (map == SHEN_NAME(Q100)) {
        query.x = position.x;
        query.z = position.z;
        query.x -= g_pFightStage->spawnPoints[0].x;
        query.z -= g_pFightStage->spawnPoints[0].z;
        Vector3 local;
        vfRotateFightVector(-g_pFightStage->teamRespawnTimers[0], &query, &local);
        const float x = std::fabs(local.x), half = g_fightFloorExtent * 0.5f;
        if (x > half)
            return false;
        if (std::fabs(local.z) > half)
            return false;
        return true;
    }
    query.x = position.x;
    query.y = position.y + 0.5f;
    query.z = position.z;
    FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game)
        return false;
    if (game->type != 8)
        return true;
    const float height = flddFindIntersectionBy(query);
    if (game->floorIntersectionY - 0.45f > height)
        return false;
    return true;
}

#else

// The Q100 authored box or the real type-eight floor bounds admission.
// @verified - p
bool vfIsWithinFightFloor(const Vector3 &position) {
    if (g_currentMap.name == SHEN_NAME(Q100)) {
        const float x = position.x - g_pFightStage->spawnPoints[0].x;
        const float z = position.z - g_pFightStage->spawnPoints[0].z;
        const auto yaw = static_cast<std::int16_t>(-g_pFightStage->teamRespawnTimers[0]);
        const float sine = sin_fixed16(yaw), cosine = cos_fixed16(static_cast<std::uint16_t>(yaw));
        if (std::fabs(cosine * x - sine * z) > 5)
            return false;
        return !(std::fabs(sine * x + cosine * z) > 5);
    }
    const Vector3 query{position.x, position.y + 0.5f, position.z};
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return false;
    if (game->type != 8)
        return true;
    return !(double(game->floorIntersectionY) - 0.45 > double(flddFindIntersectionBy(query)));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// -1 means no winner among at least two real participants.
// @verified - p
std::int32_t FloorWinner(VFMA_t *game) {
    std::int32_t count = 0;
    for (std::int32_t i = 0; i != 6; ++i)
        if (game->characters[i]) {
            ++count;
            if (game->actors[i]->stateFlags & 0x8000)
                return i;
        }
    return count < 2 ? 99 : -1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Defeat publication also resets the live paired reaction producer.
// @verified - p
void BeginFloorDefeat(VFActor *self) {
    self->stateFlags |= VFSF_Unknown80;
    self->secondaryMotionFlags |= VFSMF_Unknown20;
    self->activeMotionFlags |= VFSMF_Unknown20;
    if ((self->hitStateFlags & VFHSF_AirborneOrDowned) && self->secondActor)
        vfResetReactionProgram(self->secondActor);
    if (FloorWinner(self->game) != -1)
        return;
    self->stateFlags |= VFSF_Unknown8000;
    const bool player = self->groupIndex == static_cast<std::uint8_t>(self->game->playerSlot);
    if (g_pFightStage->battleFlags & 0x40000)
        vfAddRoundResults(!player, player);
    if (player)
        vfResetMoveTrainingGains();
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool vfDidCrossFightFrame(FightActorRuntimePrefix *actor, float frame) {
    if (!(actor->animationTime < frame)) {
        if (actor->animationTime - MOTION_VALUE(float, actor, 0x26c) < frame)
            return true;
    }
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t AuthoredMotionWord(const std::byte *data) {
    return static_cast<std::int32_t>(
        (static_cast<std::uint32_t>(data[0]) << 24U) | (static_cast<std::uint32_t>(data[1]) << 16U) | (static_cast<std::uint32_t>(data[2]) << 8U) | data[3]);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float AuthoredMotionFixed(const std::byte *data) {
    return static_cast<float>(AuthoredMotionWord(data)) * 0.000015258789f;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfMotnDispatch(FightActorRuntimePrefix *actor) {
    std::int32_t stop;
    do {
        const std::uint8_t opcode = *motionFrameCursor;
        stop = fightMarkerHandlers[opcode] ? fightMarkerHandlers[opcode](actor) : 0;
        motionFrameCursor += motionCommandWidths[opcode];
    } while (!stop);
}

#else

// The dispatcher and all non-null entries of the 32-entry motnFuncs table.
// @verified - p
void vfMotnDispatch(VFActor *self) {
    bool stop;
    do {
        stop = false;
        const auto tag = MarkerValue<std::uint8_t>(0);
        switch (tag) {
        case 0:
            self->reactionProgram = self->mirroredReactionProgram = motionMarkerCursor;
            stop = true;
            break;
        case 1:
            self->requestedAnimation = MarkerValue<std::int16_t>(2);
            break;
        case 2: {
            auto end = self->currentAnimationEndFrame;
            auto last = MarkerValue<std::uint16_t>(6);
            auto middle = MarkerValue<std::uint16_t>(4);
            auto first = MarkerValue<std::uint16_t>(2);
            if (last > end)
                last = end;
            if (middle > end) {
                middle = end;
                --end;
            }
            self->reactionMiddleFrame = static_cast<std::int16_t>(middle);
            self->reactionEndFrame = last;
            if (first > end)
                first = end;
            self->actionTriggerPhase = 1;
            self->hitStateFlags |= VFHSF_HitTimeout;
            self->parameter2d0 = static_cast<std::int16_t>(first);
            break;
        }
        case 3:
            self->parameter2d6 = MarkerValue<std::uint16_t>(2);
            self->parameter2de = MarkerValue<std::uint16_t>(4);
            self->parameter2e0 = MarkerValue<std::uint16_t>(6);
            self->groundOffset = MarkerFixed(8);
            self->motionContactFlags = MarkerValue<std::uint8_t>(1);
            self->parameter31e = MarkerValue<std::uint16_t>(12);
            break;
        case 4:
            self->moveId = MarkerValue<std::uint8_t>(1);
            self->guardState = MarkerValue<std::int8_t>(2);
            self->hitDamage = MarkerValue<std::int8_t>(3);
            self->attackReach = MarkerFixed(4);
            self->hitDirection = MarkerValue<std::uint16_t>(8);
            self->hitParameter = MarkerValue<std::uint16_t>(10);
            self->parameter2eb = MarkerValue<std::uint8_t>(12);
            self->parameter30e = MarkerValue<std::uint8_t>(13);
            self->parameter32c = MarkerValue<std::uint8_t>(14);
            self->parameter32d = MarkerValue<std::uint8_t>(15);
            self->parameter32e = MarkerValue<std::uint8_t>(16);
            self->parameter34d = MarkerValue<std::uint8_t>(17);
            self->parameter31a[0] &= 0xff00;
            if (self->moveFlags & VFMF_MirrorReactionAngle)
                self->moveId = mirroredMoveIds[self->moveId];
            break;
        case 5: {
            std::int8_t strength{};
            if (!(self->secondaryMotionFlags & (VFSMF_Unknown400 | VFSMF_Unknown800))) {
                self->hitParameter = MarkerValue<std::uint16_t>(2);
                strength = MarkerValue<std::int8_t>(1);
            }
            if (self->secondaryMotionFlags & VFSMF_Unknown400)
                self->guardHitParameter = strength;
            else
                self->hitDamage = strength;
            self->attackReach = MarkerFixed(4);
            self->facingAngleDelta = MarkerValue<std::int16_t>(8);
            ++self->activeContactCount;
            self->parameter350 = MarkerValue<std::uint16_t>(10);
            break;
        }
        case 6:
            self->parameter2e0 = MarkerValue<std::uint16_t>(2);
            self->parameter32a = 0;
            self->facingAngleDelta = MarkerValue<std::int16_t>(4);
            break;
        case 7:
            self->facingAngleDelta = MarkerValue<std::int16_t>(2);
            break;
        case 8:
            self->reactionProgram = self->mirroredReactionProgram = motionMarkerCursor + 4;
            if (self->moveFlags & VFMF_MirrorReactionAngle) {
                self->hitVector.x = -self->hitVector.x;
                self->facingAngleDelta = static_cast<std::int16_t>(-self->facingAngleDelta);
                self->hitDirection = static_cast<std::uint16_t>(-self->hitDirection);
            }
            stop = true;
            break;
        case 9:
            self->hitVector.x = MarkerFixed(4);
            self->hitVector.y = MarkerFixed(8);
            self->impulseDelay = static_cast<std::uint16_t>(static_cast<int>(MarkerValue<std::int8_t>(1) * self->inverseScale));
            self->slideVelocity.y = std::fabs(MarkerFixed(12) * (self->scale * self->scale * self->game->gravityScale));
            self->parameter2e2 = MarkerValue<std::int8_t>(2);
            break;
        case 10:
            if (!self->reactionEventCount)
                self->reactionEntries = motionMarkerCursor;
            self->reactionEventCount = static_cast<std::int8_t>(static_cast<std::uint8_t>(self->reactionEventCount) + 1);
            break;
        case 11:
            self->reactionEntryFlags = MarkerValue<std::uint32_t>(4) ^ ((self->moveFlags & VFMF_MirrorReactionAngle) ? 1u : 0u);
            self->parameter2ca = self->reactionEntryFlags & 0x7fff;
            self->reactionEntryMask = MarkerValue<std::uint32_t>(8);
            self->parameter31a[1] = MarkerValue<std::uint16_t>(2);
            break;
        case 12:
            self->eventFrame30a = SignedByteWord(1);
            self->eventFrame30c = SignedByteWord(2);
            break;
        case 13:
            self->eventFrame318 = SignedByteWord(1);
            self->eventFrame328 = SignedByteWord(2);
            break;
        case 14: {
            const float first = MarkerFixed(4), middle = MarkerFixed(12), last = MarkerFixed(8);
            auto target = MarkerValue<std::uint8_t>(1);
            auto strength = static_cast<std::uint8_t>(self->hitDamage);
            const float distance = self->contactDistances[self->actionIndex];
            float span = first - last, numerator = distance - last;
            const auto second = MarkerValue<std::uint8_t>(2);
            if (second) {
                if (!(middle >= distance)) {
                    strength = second;
                    span = middle - last;
                } else {
                    span = first - middle;
                    target = second;
                    numerator = distance - middle;
                }
            }
            // Native clamps with maxss/minss, whose unordered results follow the second operand.
            const bool negative = 0.0f > span;
            const float minimum = negative ? span : 0.0f, maximum = negative ? 0.0f : span;
            const float raised = minimum > numerator ? minimum : numerator, clamped = maximum < raised ? maximum : raised;
            self->hitDamage = static_cast<std::int8_t>(target + static_cast<int>((float(strength) - float(target)) * (clamped / span)));
            break;
        }
        case 15:
            self->parameter33c[0] = MarkerValue<std::uint16_t>(2);
            self->pairContactPriority = MarkerValue<std::int16_t>(4);
            self->parameter33c[1] = SignedByteWord(1);
            self->pairContactEndFrame = SignedByteWord(6);
            break;
        case 18:
            self->moveFlags &= ~(VFMF_MirrorReactionAngle | VFMF_MirroredStance);
            self->hitStateFlags &= ~VFHSF_MirroredReactionAngle;
            break;
        case 19:
            self->moveFlags ^= VFMF_MirrorReactionAngle;
            break;
        case 26:
            if (self->pairedActor->fightStyle == MarkerValue<std::int8_t>(1)) {
                self->hitParameter = MarkerValue<std::uint16_t>(4);
                self->hitDamage = MarkerValue<std::int8_t>(2);
            }
            break;
        case 27:
            self->knockbackAcceleration = MarkerFixed(4) * self->game->knockbackAmount;
            break;
        case 28:
            self->someAnimation[0] = MarkerValue<std::uint16_t>(2);
            break;
        case 29:
            self->parameter412 = MarkerValue<std::uint16_t>(((self->moveFlags ^ self->actor->moveFlags) & 0x40) ? 4 : 2);
            break;
        case 30:
            self->markerMask = self->savedMarkerMask = MarkerValue<std::uint32_t>(4);
            break;
        case 31:
            if (MarkerValue<std::uint8_t>(1) == 1)
                self->knockbackAcceleration = impulseTableA[vfMotnNibbleGet(74)];
            else if (MarkerValue<std::uint8_t>(1) == 2)
                self->knockbackAcceleration = impulseTableB[vfMotnNibbleGet(69)];
            break;
        default:
            break; // Native null entries 16/17 and 20..25 only advance the cursor.
        }
        motionMarkerCursor += markerWidths[tag];
    } while (!stop);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t FinishFightMotionMarkers(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(const std::byte *, actor, 0x30c) = MOTION_VALUE(const std::byte *, actor, 0x310) = motionFrameCursor;
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightRequestedAnimation(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(std::uint16_t, actor, 0x28e) = MARKER_VALUE(std::uint16_t, 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightReactionFrameRange(FightActorRuntimePrefix *actor) {
    std::uint16_t first = MARKER_VALUE(std::uint16_t, 2), middle = MARKER_VALUE(std::uint16_t, 4), last = MARKER_VALUE(std::uint16_t, 6);
    std::uint16_t end = MOTION_VALUE(std::uint16_t, actor, 0x28c);
    if (last > end)
        last = end;
    if (middle > end) {
        middle = end;
        --end;
    }
    if (first > end)
        first = end;
    MOTION_VALUE(std::uint16_t, actor, 0x290) = first;
    MOTION_VALUE(std::uint16_t, actor, 0x292) = middle;
    MOTION_VALUE(std::uint16_t, actor, 0x294) = last;
    MOTION_VALUE(std::uint8_t, actor, 0x288) = 1;
    actor->hitStateFlags |= 2U;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightGroundMotionWindow(FightActorRuntimePrefix *actor) {
    std::uint16_t &start = MOTION_VALUE(std::uint16_t, actor, 0x296);
    const std::byte *const cursor = motionFrameCursor;
    start = *reinterpret_cast<const std::uint16_t *>(cursor + 2);
    MOTION_VALUE(std::uint16_t, actor, 0x29e) = *reinterpret_cast<const std::uint16_t *>(cursor + 4);
    MOTION_VALUE(std::uint16_t, actor, 0x2a0) = *reinterpret_cast<const std::uint16_t *>(cursor + 6);
    MOTION_VALUE(float, actor, 0x2a4) = FIXED_VALUE(cursor + 8);
    MOTION_VALUE(std::uint8_t, actor, 0x2c0) = static_cast<std::uint8_t>(cursor[1]);
    MOTION_VALUE(std::uint16_t, actor, 0x2d6) = MARKER_VALUE(std::uint16_t, 12);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightAttackParameters(FightActorRuntimePrefix *actor) {
    actor->moveId = MARKER_VALUE(std::uint8_t, 1);
    MOTION_VALUE(std::int8_t, actor, 0x2a9) = MARKER_VALUE(std::int8_t, 2);
    float &strength = actor->attackReach;
    const std::byte *const cursor = motionFrameCursor;
    strength = FIXED_VALUE(cursor + 4);
    MOTION_VALUE(std::uint16_t, actor, 0x2b2) = *reinterpret_cast<const std::uint16_t *>(cursor + 8);
    MOTION_VALUE(std::uint16_t, actor, 0x2b0) = *reinterpret_cast<const std::uint16_t *>(cursor + 10);
    MOTION_VALUE(std::uint8_t, actor, 0x2ab) = static_cast<std::uint8_t>(cursor[12]);
    MOTION_VALUE(std::uint8_t, actor, 0x2c6) = MARKER_VALUE(std::uint8_t, 13);
    MOTION_VALUE(std::uint8_t, actor, 0x2e4) = MARKER_VALUE(std::uint8_t, 14);
    MOTION_VALUE(std::uint8_t, actor, 0x2e5) = MARKER_VALUE(std::uint8_t, 15);
    MOTION_VALUE(std::uint8_t, actor, 0x2e6) = MARKER_VALUE(std::uint8_t, 16);
    MOTION_VALUE(std::uint8_t, actor, 0x305) = MARKER_VALUE(std::uint8_t, 17);
    MOTION_VALUE(std::uint8_t, actor, 0x2aa) = MARKER_VALUE(std::uint8_t, 3);
    MOTION_VALUE(std::uint8_t, actor, 0x2d2) = 0;
    if (actor->moveFlags & 0x40U)
        actor->moveId = mirroredMoveIds[actor->moveId];
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightContactAttack(FightActorRuntimePrefix *actor) {
    std::uint8_t strength = 0;
    if (!(actor->secondaryMotionFlags & 0xc00U)) {
        actor->hitParameter = MARKER_VALUE(std::uint16_t, 2);
        strength = MARKER_VALUE(std::uint8_t, 1);
    }
    if (actor->secondaryMotionFlags & 0x400U)
        MOTION_VALUE(std::uint8_t, actor, 0x2c1) = strength;
    else
        MOTION_VALUE(std::uint8_t, actor, 0x2aa) = strength;
    actor->attackReach = MARKER_FIXED(4);
    actor->facingAngleDelta = MARKER_VALUE(std::int16_t, 8);
    MOTION_VALUE(std::uint16_t, actor, 0x308) = MARKER_VALUE(std::uint16_t, 10);
    ++MOTION_VALUE(std::uint16_t, actor, 0x3d6);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFacingWindow(FightActorRuntimePrefix *actor) {
    reinterpret_cast<std::uint16_t *>(actor)[0x2a0 / 2] = MARKER_VALUE(std::uint16_t, 2);
    actor->facingAngleDelta = MARKER_VALUE(std::int16_t, 4);
    MOTION_VALUE(std::uint16_t, actor, 0x2e2) = 0;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFacingDelta(FightActorRuntimePrefix *actor) {
    actor->facingAngleDelta = MARKER_VALUE(std::int16_t, 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t BeginFightReactionProgram(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(const std::byte *, actor, 0x30c) = MOTION_VALUE(const std::byte *, actor, 0x310) = motionFrameCursor + 4;
    if (actor->moveFlags & 0x40U) {
        actor->facingAngleDelta = static_cast<std::int16_t>(-actor->facingAngleDelta);
        MOTION_VALUE(std::uint16_t, actor, 0x2b2) = static_cast<std::uint16_t>(-MOTION_VALUE(std::uint16_t, actor, 0x2b2));
        MOTION_VALUE(float, actor, 0x2b4) = -MOTION_VALUE(float, actor, 0x2b4);
    }
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightMotionImpulse(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *game = actor->game;
    actor->motionImpulseX = MARKER_FIXED(4);
    actor->motionImpulseZ = MARKER_FIXED(8);
    actor->impulseDelayFrames =
        static_cast<std::int32_t>(actor->inverseMotionSpeedScale * static_cast<float>(static_cast<std::uint32_t>(MARKER_VALUE(std::int8_t, 1))));
    const std::int32_t impulse = actor->motionSpeedScale * actor->motionSpeedScale * game->gravityScale * MARKER_FIXED(12);
    actor->slideVelocity.y = std::fabs(static_cast<float>(impulse));
    MOTION_VALUE(std::uint16_t, actor, 0x2a2) = MARKER_VALUE(std::int8_t, 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t AppendFightReactionEvent(FightActorRuntimePrefix *actor) {
    if (!MOTION_VALUE(std::uint8_t, actor, 0x2e7))
        MOTION_VALUE(const std::byte *, actor, 0x2bc) = motionFrameCursor;
    ++MOTION_VALUE(std::uint8_t, actor, 0x2e7);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightReactionEventFlags(FightActorRuntimePrefix *actor) {
    std::uint32_t flags = MARKER_VALUE(std::uint32_t, 4);
    if (actor->moveFlags & 0x40U)
        flags ^= 1;
    MOTION_VALUE(std::uint32_t, actor, 0x2c8) = flags;
    MOTION_VALUE(std::uint16_t, actor, 0x28a) = flags & 0x7fffU;
    const std::byte *const cursor = motionFrameCursor;
    MOTION_VALUE(std::uint32_t, actor, 0x2cc) = *reinterpret_cast<const std::uint32_t *>(cursor + 8);
    MOTION_VALUE(std::uint16_t, actor, 0x2d4) = *reinterpret_cast<const std::uint16_t *>(cursor + 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightContactFrames(FightActorRuntimePrefix *actor) {
    actor->contactStartFrame = MARKER_VALUE(std::int8_t, 1);
    actor->contactEndFrame = MARKER_VALUE(std::int8_t, 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightPairContactFrames(FightActorRuntimePrefix *actor) {
    actor->pairContactStartFrame = MARKER_VALUE(std::int8_t, 1);
    actor->pairContactEndFrame = MARKER_VALUE(std::int8_t, 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ScaleFightAttackStrength(FightActorRuntimePrefix *actor) {
    float first = MARKER_FIXED(4);
    float middle = MARKER_FIXED(12);
    float last = MARKER_FIXED(8);
    float distance = actor->contactDistances[MOTION_VALUE(std::int8_t, actor, 1)];
    std::uint8_t target = MARKER_VALUE(std::uint8_t, 1);
    float span = first - last, numerator = distance - last;
    std::uint8_t strength = MOTION_VALUE(std::uint8_t, actor, 0x2aa);
    const std::uint8_t second = MARKER_VALUE(std::uint8_t, 2);
    if (second) {
        if (!(distance > middle)) {
            target = second;
            span = first - middle;
            numerator = distance - middle;
        } else {
            span = middle - last;
            strength = second;
        }
    }
    numerator = span < 0 ? ClampFightMotionParameter(span, 0, numerator) : ClampFightMotionParameter(0, span, numerator);
    const std::uint8_t step = static_cast<std::int32_t>((static_cast<float>(strength) - static_cast<float>(target)) * (numerator / span));
    target += step;
    MOTION_VALUE(std::uint8_t, actor, 0x2aa) = target;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightPairReactionParameters(FightActorRuntimePrefix *actor) {
    reinterpret_cast<std::uint16_t *>(actor)[0x2f4 / 2] = MARKER_VALUE(std::uint16_t, 2);
    reinterpret_cast<std::int32_t *>(actor)[0x2f8 / 4] = MARKER_VALUE(std::int16_t, 4);
    actor->animationEndFrame = MARKER_VALUE(std::int8_t, 1);
    reinterpret_cast<std::uint16_t *>(actor)[0x306 / 2] = MARKER_VALUE(std::int8_t, 6);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ToggleFightMotionFacing(FightActorRuntimePrefix *actor) {
    actor->moveFlags ^= 0x40U;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightPartnerAttack(FightActorRuntimePrefix *actor) {
    const std::uint16_t pairedStyle = MOTION_VALUE(std::uint16_t, MOTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c4), 2);
    const std::byte *const marker = motionFrameCursor;
    if (pairedStyle == *reinterpret_cast<const std::int8_t *>(marker + 1)) {
        MOTION_VALUE(std::uint16_t, actor, 0x2b0) = *reinterpret_cast<const std::uint16_t *>(marker + 4);
        MOTION_VALUE(std::uint8_t, actor, 0x2aa) = *reinterpret_cast<const std::uint8_t *>(marker + 2);
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightKnockbackScale(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(float, actor, 0x278) = actor->game->knockbackAmount * FRAME_FIXED(4);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightExtraAnimation(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(std::uint16_t, actor, 0x2da) = MARKER_VALUE(std::uint16_t, 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFacingAnimation(FightActorRuntimePrefix *actor) {
    if ((actor->moveFlags ^ actor->actor->moveFlags) & 0x40U)
        MOTION_VALUE(std::uint16_t, actor, 0x3a6) = MARKER_VALUE(std::uint16_t, 4);
    else
        MOTION_VALUE(std::uint16_t, actor, 0x3a6) = MARKER_VALUE(std::uint16_t, 2);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightMotionMarkerMask(FightActorRuntimePrefix *actor) {
    actor->markerMask = MARKER_VALUE(std::uint32_t, 4);
    actor->retained350 = actor->markerMask;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightSavedImpulse(FightActorRuntimePrefix *actor) {
    switch (MARKER_VALUE(std::int8_t, 1)) {
    case 1: {
        float table[16] = {1, 1, 1, 1.2f, 1.4f, 1.6f, 1.8f, 2, 2.1f, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f, 2.7f, 2.8f};
        MOTION_VALUE(float, actor, 0x278) = table[vfMotnNibbleGet(74)];
        break;
    }
    case 2: {
        float table[16] = {1, 1, 1, 1, 1, 1, 1, 1.2f, 1.4f, 1.6f, 2, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f};
        MOTION_VALUE(float, actor, 0x278) = table[vfMotnNibbleGet(69)];
        break;
    }
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfProcessMotionFrameEvents(FightActorRuntimePrefix *actor) {
    std::int32_t stop;
    do {
        const std::byte *const record = MOTION_VALUE(const std::byte *, actor, 0x314);
        std::uint32_t type = static_cast<std::uint8_t>(record[2]);
        if (type) {
            motionFrameEvent = static_cast<std::uint16_t>(*reinterpret_cast<const std::int16_t *>(record) - 1);
            if (actor->animationTime < static_cast<float>(motionFrameEvent))
                type = 0;
        }
        if (!type)
            motionFrameEvent = static_cast<std::uint16_t>(type);
        motionFrameCursor = MOTION_VALUE(const std::byte *, actor, 0x314);
        stop = fightFrameCallbacks[type] ? fightFrameCallbacks[type](actor) : 0;
        MOTION_VALUE(const std::byte *, actor, 0x314) += motionFrameCommandWidths[type];
    } while (!stop);
}

#else

// Every non-null slot of the separate 57-entry frame-event table.
// @verified - p
std::uint8_t vfProcessMotionFrameEvents(VFActor *self) {
    using enum CameraState;
    for (;;) {
        motionMarkerCursor = self->reactionProgramCounter;
        auto tag = MarkerValue<std::uint8_t>(2);
        motionFrameEvent = tag ? static_cast<std::uint16_t>(MarkerValue<std::uint16_t>(0) - 1) : 0;
        if (tag && motionFrameEvent > self->animationTime) {
            tag = 0;
            motionFrameEvent = 0;
        }
        bool stop = false;
        const auto argument = MarkerValue<std::uint8_t>(3);
        switch (tag) {
        case 0:
            self->reactionProgram = self->reactionProgramCounter;
            if (self->moveFlags & VFMF_Unknown1000) {
                self->hitStateFlags |= VFHSF_ReactionActive;
                self->moveFlags &= ~VFMF_Unknown1000;
            }
            stop = true;
            break;
        case 1:
            self->hitStateFlags ^= VFHSF_MirroredReactionAngle;
            break;
        case 2:
            mtwkSetExtraFlag(self->motion, 0, 0);
            if ((self->parameter370 & 0xff000000) != 0x08000000)
                self->parameter370 = 0x06000000;
            self->parameter40c &= 0xffff0000U;
            break;
        case 3: {
            const auto angle =
                static_cast<std::int16_t>((self->moveFlags & VFMF_MirrorReactionAngle) ? -MarkerValue<std::int16_t>(4) : MarkerValue<std::int16_t>(4));
            const auto yaw = static_cast<std::int16_t>(angle - self->facingAngle);
            const auto *const source = self->pairedActor;
            const float horizontal = MarkerFixed(8) * source->scale;
            self->knockbackVelocity = {sin_fixed16(yaw) * horizontal,
                ((source->scale * source->scale) * self->game->gravityScale) * (MarkerFixed(12) * source->scale),
                -cos_fixed16(static_cast<std::uint16_t>(yaw)) * horizontal};
            vfSnapActorToContact(self);
            break;
        }
        case 4:
            if (camCtrlGetInterpMode() == CameraMotionMode::AutoQuadratic || camCtrlGetInterpMode() == CameraMotionMode::AutoQuadratic2)
                vfPlayFightSfx(MarkerValue<std::uint32_t>(4), self, 0);
            break;
        case 5:
            self->hitStateFlags |= MarkerValue<std::uint32_t>(4);
            break;
        case 6:
            self->hitStateFlags &= MarkerValue<std::uint32_t>(4);
            break;
        case 7:
            self->hitStateFlags ^= MarkerValue<std::uint32_t>(4);
            break;
        case 8:
            if (!(0 >= self->game->guardTimer)) {
                const float factor = vfclCalculateGuardKnockbackFactor(self->secondActor) * argument;
                const auto damage = static_cast<std::uint8_t>(static_cast<std::int64_t>(factor + 0.5f));
                const auto increment = static_cast<std::uint16_t>(static_cast<std::int32_t>((factor * 0.5f + 0.5f) * self->inverseScale));
                self->parameter40c =
                    (self->parameter40c & 0xffffU) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>((self->parameter40c >> 16) + increment)) << 16);
                if (reinterpret_cast<const std::uint8_t *>(self->slot)[0x1a5] == 3 &&
                    ((self->secondaryMotionFlags & VFSMF_Unknown10) || static_cast<std::uint16_t>((self->currentAnimation & 0xfff) - 67) <= 1))
                    self->parameterB2c = guardImpactMultipliers[vfMotnNibbleGet(67)];
                SetActorValue(self, 0x414, static_cast<std::uint16_t>(ActorValue<std::uint16_t>(self, 0x414) + damage));
                vfHandleImpactSfxAndDefeat(self, damage, 0);
            }
            break;
        case 9: {
            std::uint16_t dialogue;
            std::memcpy(&dialogue, self->game->reserved318 + 2, sizeof(dialogue));
            if (!cvsHasValidConverseData() && dialogue == 0xffff && !(self->game->mode & 1) &&
                ((self->hitStateFlags & (VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned)) ||
                    static_cast<std::uint32_t>(static_cast<std::int8_t>(argument) - 14) > 1 || IncrementSharedPrngFloat() <= 0.33000001f)) {
                const auto voice = fightVoices[static_cast<std::int8_t>(argument)][vfFightVoiceStyle(self->fightStyle)];
                if (voice)
                    vfPlayFightSfx(voice, self, 1);
            }
            break;
        }
        case 10:
            check_dead_status(1);
            break;
        case 11:
            if (!MarkerValue<std::uint16_t>(4) || MarkerValue<std::uint16_t>(4) < IncrementSharedPrngUInt16()) {
                self->queuedReactionEvent = 31;
                self->queuedReactionAnimation = MarkerValue<std::uint16_t>(6);
                stop = true;
            }
            break;
        case 13:
            if (argument)
                self->hitStateFlags &= ~VFHSF_HasAxisAngle;
            else
                self->hitStateFlags |= VFHSF_HasAxisAngle;
            break;
        case 14:
            if (vfCheckAnimTimeWithin(self, static_cast<float>(motionFrameEvent))) {
                float amount = MarkerFixed(4);
                if (self->groupIndex == self->game->playerSlot)
                    amount *= self->game->playerCameraScale + 1.5f;
                if (!(self->game->mode & 8))
                    camCtrlSetRotationXYZ(0, amount, 0.5f);
            }
            break;
        case 15: {
            auto *const current = vfmaGetTaskParam();
            if (current && (current->mode & 0x80000))
                break;
            const auto index = MarkerValue<std::int16_t>(4);
            if (!pakFindEventCam(index) || (g_pFightStage->battleFlags & 2))
                break;
            auto &game = *self->game;
            if (argument == 1) {
                for (std::uint8_t i = 0; i < 6; ++i)
                    if (game.characters[i]) {
                        vfResetReactionProgram(game.actors[i]);
                        self->moveFlags |= VFMF_Unknown10000;
                    }
                game.mode |= 1;
                if (!(std::to_integer<std::uint8_t>(self->motionParameters[2][25]) & 1))
                    self->moveFlags |= VFMF_NoContactPairing;
            } else if (argument == 2)
                game.mode |= 0x4000;
            for (auto *character : game.characters)
                if (character)
                    cpamSetFlag(character, 0);
            if (self->moveFlags & VFMF_MirrorReactionAngle)
                camCtrlSetSplineShouldRebuildRotTo(true);
            game.targetTime = static_cast<float>(MarkerValue<std::uint16_t>(6));
            const std::uintptr_t request[3]{static_cast<std::uint32_t>(static_cast<std::int32_t>(index)), reinterpret_cast<std::uintptr_t>(self->character),
                reinterpret_cast<std::uintptr_t>(self->character)};
            camCtrlToggleSplineRebuildFlag(3, request);
            camCtrlSetState(Scripted);
            camCtrlDisableFilmBars();
            break;
        }
        case 16: {
            constexpr std::int32_t expressions[11]{1, 3, 0, 0, 0, 0, 0, 3, 11, 6, 4};
            SetHighModelAndFaceFlagsForChara(self->character, expressions[argument], 1, 1);
            break;
        }
        case 19:
            self->parameter40c =
                (self->parameter40c & 0xffff0000U) | static_cast<std::uint16_t>(static_cast<std::int32_t>(MarkerValue<std::uint16_t>(4) * self->inverseScale));
            break;
        case 20:
            if (vfCheckAnimTimeWithin(self, static_cast<float>(motionFrameEvent)))
                SetActorValue(self, 0x638, std::uint64_t{});
            break;
        case 22:
            self->hitStateFlags |= VFHSF_FacingStepOnTransition;
            self->facingAngleDelta = MarkerValue<std::int16_t>(4);
            break;
        case 23: {
            const bool value = (self->groundContactFlags & 1) ? argument != 0 : argument == 0;
            if (!value) {
                self->queuedReactionEvent = 31;
                self->queuedReactionAnimation = MarkerValue<std::uint16_t>(4);
                stop = true;
            }
            break;
        }
        case 25: {
            auto flags = MarkerValue<std::uint32_t>(4);
            const auto &other = *self->pairedActor;
            if (other.moveFlags & VFMF_MirrorReactionAngle) {
                if (flags & 0x4000000)
                    flags = (flags & ~0x4000000U) | 0x2000000;
                else if (flags & 0x2000000)
                    flags = (flags & ~0x2000000U) | 0x4000000;
            }
            if ((flags & self->reactionFlags) && MarkerFixed(12) >= self->contactDistances[self->actionIndex] && !self->game->hitStopFrames &&
                other.knockbackBaseParameter <= 150)
                self->action = static_cast<std::uint64_t>(static_cast<std::int64_t>(MarkerValue<std::int32_t>(8)));
            break;
        }
        case 26: {
            auto cursor = ActorValue<std::uint8_t>(self, 0x618);
            const auto end = ActorValue<std::uint8_t>(self, 0x61a);
            const auto filter = MarkerValue<std::uint16_t>(8);
            bool found = false;
            std::uint16_t mask{};
            while (cursor != end) {
                cursor = static_cast<std::uint8_t>((cursor - 1) & 31);
                const auto flags = ActorValue<std::uint16_t>(self, 0x51c + 8 * cursor) & 0x30f;
                mask = ActorValue<std::uint16_t>(self, 0x518 + 8 * cursor) & 0xf0;
                if (!filter || (filter & flags) == filter) {
                    found = true;
                    break;
                }
                if (flags & 0x100)
                    break;
            }
            auto requested = MarkerValue<std::uint16_t>(4);
            if (found && (!requested || ((requested & 0x400) ? (mask & 0x20) : (mask & ((requested & 0x800) ? 16 : requested))))) {
                self->queuedReactionEvent = 31;
                self->queuedReactionAnimation = MarkerValue<std::uint16_t>(10);
                cursor = static_cast<std::uint8_t>((cursor + 1) & 31);
                SetActorValue(self, 0x619, cursor);
                SetActorValue(self, 0x61a, cursor);
                SetActorValue(self, 0x61b, cursor);
                SetActorValue(self, 0x61c, cursor);
            }
            break;
        }
        case 29:
            self->activeMotionFlags |= MarkerValue<std::uint32_t>(4);
            break;
        case 30:
            self->activeMotionFlags &= MarkerValue<std::uint32_t>(4);
            break;
        case 32:
            vfPlayFightSfx(g_currentMap.name == SHEN_NAME(QKBT) ? 2803 : MarkerValue<std::uint32_t>(4), self, 0);
            break;
        case 45:
            EmitFightSmoke(argument, self);
            break;
        case 46:
            vfStageObjectReset(self->parameterAd7, self->character);
            break;
        case 47:
            vfStageObjectReset(self->parameterAd7, nullptr);
            break;
        case 48:
            vfLaunchStageObject(self->parameterAd7, self->temporaryFacingAngle);
            break;
        case 50: {
            auto voice = argument;
            const auto count = MarkerValue<std::uint8_t>(4);
            if (count > 1)
                voice = static_cast<std::uint8_t>(voice + static_cast<std::int32_t>(IncrementSharedPrngFloat() * count));
            if (self->game->reserved318[0] != std::byte{0xff} && !(g_pFightStage->battleFlags & 2))
                EV_PlayDialogue(vfGetFightDialogueVoice(self->game, voice));
            break;
        }
        case 51: {
            const auto effect = MarkerValue<std::int8_t>(4);
            auto frames = MarkerValue<std::int16_t>(6);
            const float amount = MarkerFixed(8);
            if (!effect)
                vfToggleAttachedStageObjectDisplay(self);
            else if (static_cast<std::uint8_t>(effect) <= 3) {
                if (!frames)
                    frames = 1;
                vfSetAttachedStageObjectEffect(self, effect, amount, frames);
            } else if (effect == 4)
                vfSetAttachedStageObjectFigurePose(self, static_cast<std::int8_t>(static_cast<std::int64_t>(static_cast<double>(amount) + 0.5)), frames);
            break;
        }
        case 53:
            toggleOsagFlags(self->character);
            break;
        case 54:
            if (!(self->stateFlags & VFSF_Unknown80) || g_currentMap.name != SHEN_NAME(QLBT)) {
                std::uint32_t sound = 38;
                if (camCtrlGetInterpMode() != CameraMotionMode::AutoQuadratic) {
                    const auto material = static_cast<std::uint16_t>(QueryGroundMaterialToken(self->contactPosition, 0));
                    constexpr std::uint16_t sounds[9]{37, 38, 39, 642, 642, 1750, 2803, 2803, 2803};
                    sound = sounds[(material == 10 ? 3 : material == 15 ? 6 : 0) + MarkerValue<std::uint16_t>(4)];
                }
                vfPlayFightSfx(sound, self, 0);
            }
            break;
        case 55:
            if (camCtrlCheckIsEventCam() && !(g_pFightStage->battleFlags & 2)) {
                resetCharaPosYandIntersection();
                camCtrlSetFovTransitionTo(false);
                camCtrlSetState(ChaseGoal);
            }
            break;
        case 56:
            self->reactionRank = argument;
            break;
        default:
            break; // Native null callbacks advance by their authored width.
        }
        const auto width = frameMarkerWidths[tag];
        self->reactionProgramCounter += width;
        if (stop)
            return width;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t FinishFightFrameEvents(FightActorRuntimePrefix *actor) {
    MOTION_VALUE(const std::byte *, actor, 0x310) = MOTION_VALUE(const std::byte *, actor, 0x314);
    if (actor->moveFlags & 0x1000U) {
        actor->moveFlags &= ~0x1000U;
        actor->hitStateFlags |= 1U;
    }
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t MirrorFightFrameReaction(FightActorRuntimePrefix *actor) {
    actor->hitStateFlags ^= 0x200000U;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t BeginFightFrameRecovery(FightActorRuntimePrefix *actor) {
    mtwkSetExtraFlag(actor->motion, 0, 0);
    MOTION_VALUE(std::uint32_t, actor, 0x318) = 0x6000000U;
    MOTION_VALUE(std::uint16_t, actor, 0x3a0) = 0;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ApplyFightFrameImpulse(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    FightActorRuntimePrefix *const source = MOTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c4);
    std::int32_t yaw = FRAME_VALUE(std::int16_t, 4);
    if (actor->moveFlags & 0x40U)
        yaw = static_cast<std::int16_t>(-yaw);
    yaw = -static_cast<std::int16_t>(actor->facingAngle - yaw);
    const float sine = CalculateFixedSine(static_cast<std::uint32_t>(yaw)), cosine = -CalculateFixedCosine(static_cast<std::uint32_t>(yaw));
    const float horizontal = MOTION_VALUE(float, source, 0x26c) * FRAME_FIXED(8);
    const float vertical = MOTION_VALUE(float, source, 0x26c) * FRAME_FIXED(12);
    MOTION_VALUE(float, actor, 0x454) = horizontal * sine;
    MOTION_VALUE(float, actor, 0x45c) = horizontal * cosine;
    MOTION_VALUE(float, actor, 0x458) = ((MOTION_VALUE(float, source, 0x26c) * MOTION_VALUE(float, source, 0x26c)) * game->gravityScale) * vertical;
    vfSnapActorToContact(actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameHitFlags(FightActorRuntimePrefix *actor) {
    actor->hitStateFlags |= FRAME_VALUE(std::uint32_t, 4);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t MaskFightFrameHitFlags(FightActorRuntimePrefix *actor) {
    actor->hitStateFlags &= FRAME_VALUE(std::uint32_t, 4);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ToggleFightFrameHitFlags(FightActorRuntimePrefix *actor) {
    actor->hitStateFlags ^= FRAME_VALUE(std::uint32_t, 4);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ApplyFightFrameGuardDamage(FightActorRuntimePrefix *actor) {
    if (actor->game->elapsedFrames) {
        const std::int32_t strength = FRAME_VALUE(std::uint8_t, 3);
        const float amount = static_cast<float>(strength) * vfclCalculateGuardKnockbackFactor(actor->secondActor, 1.0f);
        const std::uint8_t damage = static_cast<std::uint8_t>(amount + 0.5f);
        MOTION_VALUE(std::uint16_t, actor, 0x3a2) += static_cast<std::int32_t>(MOTION_VALUE(float, actor, 0x270) * (amount * 0.5f + 0.5f));
        MOTION_VALUE(std::uint16_t, actor, 0x3a8) += damage;
        vfHandleImpactSfxAndDefeat(actor, damage, 0);
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t PlayFightFrameVoice(FightActorRuntimePrefix *actor) {
    if (cvsHasValidConverseData(0) || MOTION_VALUE(std::int16_t, actor->game, 0x22e) != -1 || (actor->game->stateFlags & 1U))
        return 0;
    const std::int8_t voice = FRAME_VALUE(std::int8_t, 3);
    if (!(actor->hitStateFlags & 0xc0000U) && static_cast<std::uint32_t>(voice - 14) <= 1U && dc::sdk::RandomFloat() > 0.33f)
        return 0;
    const std::uint16_t *const voices = fightEventVoices[FRAME_VALUE(std::int8_t, 3)];
    const std::uint16_t sound = voices[vfFightVoiceStyle(MOTION_VALUE(std::uint16_t, actor, 2))];
    if (sound)
        vfPlayFightSfx(sound, actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t UpdateFightFrameDeadStatus(FightActorRuntimePrefix *) {
    check_dead_status(1);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t QueueFightFrameRandomReaction(FightActorRuntimePrefix *actor) {
    const std::uint16_t *const chance = &FRAME_VALUE(std::uint16_t, 4);
    if (!*chance || *chance < static_cast<std::uint16_t>(dc::sdk::RandomInt())) {
        MOTION_VALUE(std::uint16_t, actor, 0x27e) = 31;
        MOTION_VALUE(std::uint16_t, actor, 0x2d8) = FRAME_VALUE(std::uint16_t, 6);
        return 1;
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameAxisAngle(FightActorRuntimePrefix *actor) {
    if (!FRAME_VALUE(std::uint8_t, 3))
        actor->hitStateFlags |= 0x8000000U;
    else
        actor->hitStateFlags &= ~0x8000000U;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ShakeFightFrameCamera(FightActorRuntimePrefix *actor) {
    if (vfDidCrossFightFrame(actor, static_cast<float>(motionFrameEvent))) {
        float amount = FRAME_FIXED(4);
        if (MOTION_VALUE(std::int8_t, actor, 0) == actor->game->playerSlot)
            amount *= actor->game->playerCameraScale + 1.5f;
        if (!(actor->game->stateFlags & 8U))
            camCtrlSetRotationXYZ(0, amount, 0.5f);
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t BeginFightFrameCamera(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    if (IsAuthFightCamera())
        return 0;
    reinterpret_cast<std::uint16_t *>(game)[0x446 / 2] = FRAME_VALUE(std::uint16_t, 6);
    const std::int32_t camera = FRAME_VALUE(std::int16_t, 4);
    std::uintptr_t request[3];
    request[0] = static_cast<std::uintptr_t>(camera);
    request[2] = request[1] = reinterpret_cast<std::uintptr_t>(actor->character);
    if (camCtrlFindEventCamera(camera)) {
        if (!(g_pFightStage->battleFlags & 2U)) {
            switch (FRAME_VALUE(std::int8_t, 3)) {
            case 1:
                for (std::uint8_t index = 0; index < 6; ++index)
                    if (game->characters[index]) {
                        vfResetReactionProgram(game->actors[index]);
                        actor->moveFlags |= 0x10000U;
                    }
                game->stateFlags |= 1U;
                if ((actor->motionParameters[25] & 1U) == 0)
                    actor->moveFlags |= 0x40000U;
                break;
            case 2:
                game->stateFlags |= 0x4000U;
                break;
            }
            for (std::uint8_t index = 0; index < 6; ++index)
                if (game->characters[index])
                    cpamSetFlag(game->characters[index], 0);
            if (actor->moveFlags & 0x40U)
                camCtrlSetSplineShouldRebuildRotTo(1);
            camCtrlToggleSplineRebuildFlag(3, request);
            camCtrlSetState(6);
        }
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameExpression(FightActorRuntimePrefix *actor) {
    vfSetFightFaceExpression(actor->character, FRAME_VALUE(std::uint8_t, 3), 1);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameHold(FightActorRuntimePrefix *actor) {
    const std::uint16_t frames = FRAME_VALUE(std::uint16_t, 4);
    MOTION_VALUE(std::uint16_t, actor, 0x3a0) = static_cast<std::uint16_t>(MOTION_VALUE(float, actor, 0x270) * static_cast<float>(frames));
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ClearFightFrameContinuation(FightActorRuntimePrefix *actor) {
    if (vfDidCrossFightFrame(actor, static_cast<float>(motionFrameEvent)))
        MOTION_VALUE(void *, actor, 0x5b0) = 0;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameFacingStep(FightActorRuntimePrefix *actor) {
    actor->facingAngleDelta = FRAME_VALUE(std::int16_t, 4);
    actor->hitStateFlags |= 0x40U;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t QueueFightFrameContactReaction(FightActorRuntimePrefix *actor) {
    std::int32_t value = 0;
    if (!FRAME_VALUE(std::uint8_t, 3))
        value++;
    if (actor->groundContactFlags & 1U)
        value ^= 1;
    if (!value) {
        actor->queuedReactionEvent = 31;
        MOTION_VALUE(std::uint16_t, actor, 0x2d8) = FRAME_VALUE(std::uint16_t, 4);
        return 1;
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameContactAction(FightActorRuntimePrefix *actor) {
    if ((actor->reactionFlags & FRAME_VALUE(std::uint32_t, 4)) && !(actor->contactDistances[MOTION_VALUE(std::int8_t, actor, 1)] > FRAME_FIXED(12)) &&
        !actor->game->retained44)
        MOTION_VALUE(std::uint32_t, actor, 0x320) = FRAME_VALUE(std::uint32_t, 8);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t QueueFightFrameBufferedReaction(FightActorRuntimePrefix *actor) {
    std::uint16_t mask, held = 0;
    std::uint8_t cursor = MOTION_VALUE(std::uint8_t, actor, 0x598);
    const std::uint8_t end = MOTION_VALUE(std::uint8_t, actor, 0x59a);
    std::int32_t state = cursor == end ? 2 : 0;
    const std::uint16_t filter = FRAME_VALUE(std::uint16_t, 8);
    if (state != 2) {
        while (cursor != end) {
            cursor = static_cast<std::uint8_t>((cursor - 1) & 31);
            const std::uint16_t *const entry = &MOTION_VALUE(std::uint16_t, actor, 0x498 + cursor * 8);
            held = entry[0];
            const std::uint16_t flags = entry[2] & 0x30fU;
            if (!filter || (flags & filter) == filter) {
                state = 1;
                break;
            }
            if (flags & 0x100U) {
                state = 2;
                break;
            }
        }
    }
    if (state != 1)
        state = 2;
    while (state != 5) {
        switch (state) {
        case 1: {
            held &= 0xf0U;
            const std::uint16_t requested = FRAME_VALUE(std::uint16_t, 4);
            mask = FRAME_VALUE(std::uint16_t, 6);
            if (!requested) {
                state = 3;
                break;
            }
            if (requested & 0x400U)
                mask = 32;
            else if (requested & 0x800U)
                mask = 16;
            else
                mask = requested;
            state = held & mask ? 4 : 2;
            break;
        }
        case 2:
            state = 5;
            break;
        case 3:
        case 4:
            MOTION_VALUE(std::uint16_t, actor, 0x27e) = 31;
            MOTION_VALUE(std::uint16_t, actor, 0x2d8) = FRAME_VALUE(std::uint16_t, 10);
            cursor = static_cast<std::uint8_t>((cursor + 1) & 31);
            MOTION_VALUE(std::uint8_t, actor, 0x599) = cursor;
            MOTION_VALUE(std::uint8_t, actor, 0x59a) = cursor;
            MOTION_VALUE(std::uint8_t, actor, 0x59b) = cursor;
            MOTION_VALUE(std::uint8_t, actor, 0x59c) = cursor;
            state = 5;
            break;
        }
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameActiveFlags(FightActorRuntimePrefix *actor) {
    actor->activeMotionFlags |= FRAME_VALUE(std::uint32_t, 4);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t MaskFightFrameActiveFlags(FightActorRuntimePrefix *actor) {
    actor->activeMotionFlags &= *(const std::uint32_t *)(motionFrameCursor + 4);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t PlayFightFrameSound(FightActorRuntimePrefix *actor) {
    const shen_name map = g_currentMap;
    std::uint32_t sound;
    if (map == SHEN_NAME(QKBT))
        sound = 2803;
    else
        sound = FRAME_VALUE(std::uint16_t, 4);
    vfPlayFightSfx(sound, actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t EmitFightFrameSmoke(FightActorRuntimePrefix *actor) {
    EmitFightSmoke(*(const std::uint8_t *)(motionFrameCursor + 3), actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t AttachFightFrameStageObject(FightActorRuntimePrefix *actor) {
    vfAttachActorStageObject(actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ReleaseFightFrameStageObject(FightActorRuntimePrefix *actor) {
    vfReleaseActorStageObject(actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t LaunchFightFrameStageObject(FightActorRuntimePrefix *actor) {
    vfLaunchActorStageObject(actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t PlayFightFrameDialogue(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    std::uint8_t voice = FRAME_VALUE(std::uint8_t, 3);
    const std::uint8_t count = FRAME_VALUE(std::uint8_t, 4);
    if (count > 1)
        voice = static_cast<std::uint8_t>(voice + static_cast<std::int32_t>(static_cast<float>(count) * dc::sdk::RandomFloat()));
    if (MOTION_VALUE(std::int8_t, game, 0x22c) != -1 && !(g_pFightStage->battleFlags & 2U))
        EV_PlayDialogue(vfGetFightDialogueVoice(game, voice));
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t SetFightFrameObjectEffect(FightActorRuntimePrefix *actor) {
    const std::uint8_t effect = FRAME_VALUE(std::uint8_t, 4);
    std::uint16_t frames = FRAME_VALUE(std::uint16_t, 6);
    float amount = FRAME_FIXED(8);
    switch (effect) {
    case 0:
        vfToggleAttachedStageObjectDisplay(actor);
        break;
    case 1:
    case 2:
    case 3:
        if (!frames)
            frames = 1;
        vfSetAttachedStageObjectEffect(actor, effect, amount, frames);
        break;
    case 4:
        vfSetAttachedStageObjectFigurePose(actor, static_cast<std::uint16_t>(amount + 0.5f), frames);
        break;
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ToggleFightFrameHair(FightActorRuntimePrefix *actor) {
    toggleOsagFlags(actor->character);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t PlayFightFrameCameraSound(FightActorRuntimePrefix *actor) {
    const std::uint8_t mode = camCtrlGetInterpMode();
    if (mode == 4 || mode == 8)
        vfPlayFightSfx(FRAME_VALUE(std::uint32_t, 4), actor);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t PlayFightFrameGroundSound(FightActorRuntimePrefix *actor) {
    const std::uint16_t sounds[3][3] = {{37, 38, 39}, {642, 642, 1750}, {2803, 2803, 2803}};
    if (camCtrlGetInterpMode() == 4)
        vfPlayFightSfx(38, actor);
    else {
        const std::uint16_t material = static_cast<std::uint16_t>(GetFieldSoundProperties(&actor->contactPosition));
        std::uint16_t group;
        switch (material) {
        case 10:
            group = 1;
            break;
        case 15:
            group = 2;
            break;
        default:
            group = 0;
            break;
        }
        vfPlayFightSfx(sounds[group][FRAME_VALUE(std::uint16_t, 4)], actor);
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t vfPrepareAuthoredDefeatCamera() {
    if (camCtrlCheckIsEventCam() && !(g_pFightStage->battleFlags & 2U)) {
        InitializeCameraGoal();
        camCtrlSetFovTransitionTo(0);
        camCtrlSetState(10);
    }
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t vfTryGuardReactionSwap(FightActorRuntimePrefix *actor, FightActorRuntimePrefix *source) {
    const std::uint16_t move = MOTION_VALUE(std::uint16_t, actor, 0x3d4);
    float frame;
    if (!(actor->hitStateFlags & 0x2000U) || MOTION_VALUE(std::uint16_t, actor, 0x3da) != move) {
        if ((actor->groundContactFlags & 1U) == 0) {
            g_vfSelectedReactionAnimation = 0;
            vfclChooseReactionAnim(source, actor);
            const std::int32_t animation = g_vfSelectedReactionAnimation;
            if (!animation)
                return 0;
            frame = static_cast<float>(MOTION_VALUE(std::uint16_t, source, 0x290)) - MOTION_VALUE(float, actor, 0x440);
            const std::byte *const extra = MOTN_FindAnimationExtraRecord(animation & 0xfffU, 2);
            if (extra && !(frame > static_cast<float>(*reinterpret_cast<const std::int16_t *>(extra + 2)) - 1 - MOTION_VALUE(float, actor, 0x26c))) {
                MOTION_VALUE(std::int32_t, actor, 0x434) = animation;
                vfStartFormationAnimation(actor);
                MOTION_VALUE(std::uint16_t, actor, 0x3da) = move;
                return 1;
            }
        }
    } else {
        frame = static_cast<float>(MOTION_VALUE(std::uint16_t, actor, 0x290));
        if (frame > 0)
            frame -= MOTION_VALUE(float, actor, 0x26c);
        if (frame < 0)
            frame = 0;
        if (actor->animationTime > frame) {
            actor->animationTime = frame;
            MOTION_VALUE(float, actor, 0x450) = frame;
        }
        return 1;
    }
    return 0;
}

#else

// Restart only a real authored guard reaction for the live contact.
// @verified - p
bool vfTryGuardReactionSwap(VFActor *self, VFActor *source) {
    const auto previous = static_cast<std::int16_t>(self->lastHitMove);
    if ((self->hitStateFlags & VFHSF_HeavyReaction) && self->parameter44e == static_cast<std::uint16_t>(previous)) {
        float frame = float(static_cast<std::uint16_t>(self->parameter2d0));
        if (frame > 0)
            frame -= self->scale;
        frame = 0 > frame ? 0 : frame;
        if (self->animationTime > frame)
            self->animationTime = self->parameter4c8 = frame;
        return true;
    }
    if (self->groundContactFlags & 1)
        return false;
    g_vfSelectedReactionAnimation = 0;
    vfclChooseReactionAnim(source, self);
    const auto animation = g_vfSelectedReactionAnimation;
    if (!animation)
        return false;
    const float remaining = float(static_cast<std::uint16_t>(source->parameter2d0)) - self->pairAnimationTime;
    const auto metadata = FindLoadedMotionSecondaryEntry(animation & 0xfff, 2);
    if (metadata.empty())
        return false;
    std::int16_t first;
    std::memcpy(&first, metadata.data() + 2, sizeof(first));
    if (remaining > float(first) - 1 - self->scale)
        return false;
    self->pendingAnimation = animation;
    vfApplyHitReactionAndStartMotion(self);
    self->parameter44e = static_cast<std::uint16_t>(previous);
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Authored last-enemy camera admission consumes real team counters.
// @verified - p
bool vfIsLastEnemyForCamera(VFActor *self) {
    if (self->game->slotTeams[self->groupIndex] != 1 || g_pFightStage->secondaryGroupCounters[1] || g_pFightStage->readyEnemySlots[1])
        return false;
    const auto count = g_pFightStage->unknownGroupCounts[1];
    return (count == 1 && !(self->moveFlags & VFMF_ForceDefeat)) || (count == 0 && (self->moveFlags & VFMF_ForceDefeat));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
float ClampFightMotionParameter(float minimum, float maximum, float value) {
    if (minimum > value)
        value = minimum;
    if (value > maximum)
        value = maximum;
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfHandleImpactSfxAndDefeat(FightActorRuntimePrefix *actor, std::int16_t damage, std::uint8_t mode) {
    std::int32_t floor = 0;
    FightRuntimePrefix *const game = actor->game;
    if (game->playerSlot != -1 && actor->groupIndex == game->playerSlot) {
        if (game->stateFlags & 1U)
            return;
    } else if ((game->stateFlags & 5U) == 5U)
        return;
    if (!damage || !(actor->hp > 0))
        return;
    FightActorRuntimePrefix *source;
    switch (mode) {
    case 0:
        source = actor->secondActor;
        if (!source)
            source = MOTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c4);
        break;
    case 1:
        source = MOTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c8);
        if (!source)
            source = actor->actor;
        break;
    default:
        source = 0;
        break;
    }
    if (game->playerCharacter && actor->character == game->playerCharacter) {
        damage = static_cast<std::int16_t>(static_cast<float>(damage) * (1 - game->playerAssistScale));
        if (damage < 1)
            damage = 1;
    }
    damage = static_cast<std::int16_t>(static_cast<float>(damage) * MOTION_VALUE(float, actor, 0x9dc));
    MOTION_VALUE(float, actor, 0x9dc) = 1;
    if (actor->hp <= static_cast<float>(static_cast<std::uint8_t>(actor->actor->retained2a9[1])) && (actor->actor->secondaryMotionFlags & 0x40000U))
        damage = static_cast<std::int16_t>(actor->hp - 1);
    const std::uint32_t formation = actor->formationFlags;
    if (formation & 0x4010000U)
        return;
    if ((formation & 0x80000U) && actor->hp <= static_cast<float>(damage) &&
        MOTION_VALUE(std::int8_t, actor, 0x882) != MOTION_VALUE(std::int16_t, actor, 0x934))
        return;
    if (formation & 0x20000U)
        floor = MOTION_VALUE(std::int16_t, actor, 0x932);
    if (std::fabs(actor->hp) < 1.1920929e-7f || !(actor->hp - static_cast<float>(damage) > 0)) {
        const std::byte *const event = vfFindQueuedReactionEventEntry(actor, 3);
        if (event)
            actor->queuedReactionEvent = static_cast<std::int8_t>(event[1]);
    }
    actor->hp -= static_cast<float>(damage);
    if (mode == 1) {
        if (actor->stateFlags & 0x40000U)
            actor->hp = 0;
        else if (!(actor->hp > 0))
            actor->hp = 1;
    }
    if (actor->hp < static_cast<float>(floor))
        actor->hp = static_cast<float>(floor);
    float guardDamage;
    if (source) {
        const float factor = vfclCalculateGuardKnockbackFactor(source, 1.5f);
        guardDamage = static_cast<float>(damage) * factor;
    } else
        guardDamage = static_cast<float>(damage);
    actor->retainedMotionValues[0] -= guardDamage;
    if (actor->retainedMotionValues[0] < 0)
        actor->retainedMotionValues[0] = 0;
    if (std::fabs(actor->hp) < 1.1920929e-7f) {
        if (MOTION_VALUE(std::uint8_t, game, 0x1c0) && (game->sfxSlotFlags[actor->groupIndex] & 1U))
            vfInitSfxSlotState(game);
        game->sfxSlotFlags[actor->groupIndex] = 0;
        const std::uint16_t *const voices = fightEventVoices[16];
        const std::uint16_t voice = voices[vfFightVoiceStyle(MOTION_VALUE(std::uint16_t, actor, 2))];
        if (voice)
            vfPlayFightSfx(voice, actor);
        if (actor->character != game->playerCharacter && game->state == 3) {
            ++game->retained23b;
            if (source) {
                if (source->groupIndex == game->playerSlot) {
                    ++game->battleCounter;
                    MOTION_VALUE(std::uint16_t, game, 0x43e) = 15;
                } else if (game->slotTeams[source->groupIndex] == game->slotTeams[game->playerSlot])
                    ++game->retained1dd;
            }
        } else {
            if (mode == 2)
                vfForceActorDefeat(actor);
            vfResetMoveTrainingGains();
            if (!mode && !IsAuthFightCamera() && !EV_camCtrlToggleCombatCamera(1, -1) && !(g_pFightStage->battleFlags & 0x2000U)) {
                actor->hp = 1;
                vfStartAuthoredDefeatCamera(source);
            }
        }
        if (source)
            ++MOTION_VALUE(std::uint8_t, source, 0x9bd);
    }
    if (!(std::fabs(actor->hp) < 1.1920929e-7f) && (formation & 1U) && !(static_cast<float>(MOTION_VALUE(std::int16_t, actor, 0x930)) < actor->hp))
        actor->runtimeFlags |= 1U;
}

#else

// Native damage, sound slots, defeat publication and authored camera admission.
// @verified - p
void vfHandleImpactSfxAndDefeat(VFActor *self, std::int16_t damage, std::uint8_t mode) {
    auto &game = *self->game;
    const bool player = game.playerSlot != -1 && self->groupIndex == game.playerSlot;
    if (player ? (game.mode & 1) != 0 : (game.mode & 5) == 5)
        return;
    if (g_forcePlayerGodmode) {
        if (self->groupIndex == static_cast<std::uint8_t>(game.playerSlot))
            return;
        damage = 10000;
    } else if (!damage)
        return;
    const auto queueDefeat = [&] {
        if (const auto *record = vfFindQueuedReactionEventEntry(self, 3))
            self->queuedReactionEvent = static_cast<std::uint16_t>(static_cast<std::int8_t>(std::to_integer<std::uint8_t>(record[1])));
    };
    if (self->hp < 0 || std::fabs(self->hp) < 0.00000011920929f) {
        queueDefeat();
        return;
    }
    VFActor *source{};
    if (!mode)
        source = self->secondActor ? self->secondActor : self->pairedActor;
    else if (mode == 1) {
        std::memcpy(&source, self->reservedB08, sizeof(source));
        if (!source)
            source = self->actor;
    }
    if (game.playerCharacter && self->character == game.playerCharacter) {
        damage = static_cast<std::int16_t>(static_cast<std::int32_t>((1 - game.playerAssistScale) * damage));
        if (damage < 1)
            damage = 1;
    }
    const auto factor = static_cast<std::int16_t>(static_cast<std::int32_t>(self->parameterB2c));
    self->parameterB2c = 1;
    damage = static_cast<std::int16_t>(factor * damage);
    if (source && damage >= self->hp && (source->secondaryMotionFlags & VFSMF_Unknown40000))
        damage = static_cast<std::int16_t>(static_cast<std::int32_t>(self->hp) - 1);
    const auto flags = self->formationFlags;
    if ((flags & 0x4010000) ||
        ((flags & 0x80000) && damage >= self->hp &&
            static_cast<std::int8_t>(self->packedHitImpulse >> 16) !=
                static_cast<std::int16_t>(std::bit_cast<std::uint32_t>(self->formationCameraTimer) & 0xffff)))
        return;
    const auto floor = (flags & 0x20000) ? self->formationHpFloor : std::int16_t{};
    if (self->hp - damage <= 0)
        queueDefeat();
    self->hp -= damage;
    if (mode == 1) {
        if (self->stateFlags & VFSF_Unknown40000)
            self->hp = 0;
        else if (self->hp <= 0)
            self->hp = 1;
    }
    if (self->hp < floor)
        self->hp = static_cast<float>(floor);
    self->guardHp -= source ? vfclCalculateGuardKnockbackFactor(source) * damage : static_cast<float>(damage);
    if (self->guardHp < 0)
        self->guardHp = 0;
    if (std::fabs(self->hp) < 0.00000011920929f) {
        auto *bytes = reinterpret_cast<std::uint8_t *>(&game);
        std::uint32_t soundSlot;
        std::memcpy(&soundSlot, bytes + 0x270 + 4 * self->groupIndex, sizeof(soundSlot));
        if (bytes[0x29c] && (soundSlot & 1))
            vfInitSfxSlotState(&game);
        std::memset(bytes + 0x270 + 4 * self->groupIndex, 0, 4);
        const auto voice = fightDefeatVoiceRow[vfFightVoiceStyle(self->fightStyle)];
        if (voice)
            vfPlayFightSfx(voice, self, 1);
        if (self->character == game.playerCharacter || game.state != 3) {
            if (mode == 2)
                vfForceActorDefeat(self);
            vfResetMoveTrainingGains();
            if (!mode) {
                auto *const current = vfmaGetTaskParam();
                if ((!current || !(current->mode & 0x80000)) && !EV_camCtrlToggleCombatCamera(1, -1) && !(g_pFightStage->battleFlags & 0x2000)) {
                    self->hp = 1;
                    vfStartAuthoredDefeatCamera(source);
                }
            }
        } else {
            ++bytes[0x32d];
            if (source) {
                if (source->groupIndex == static_cast<std::uint8_t>(game.playerSlot)) {
                    ++bytes[0x32c];
                    game.parameter530 = 15;
                } else if (game.slotTeams[source->groupIndex] == game.slotTeams[game.playerSlot])
                    ++game.parameter2b9;
            }
        }
        if (source)
            ++reinterpret_cast<std::uint8_t *>(source)[0xaf5];
    }
    if (std::fabs(self->hp) >= 0.00000011920929f && (flags & 1) && static_cast<std::int16_t>(self->formationEntry) >= self->hp)
        self->formationRuntimeFlags |= 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// The native window includes the current frame but excludes the previous.
// @verified - p
bool vfCheckAnimTimeWithin(VFActor *self, float frame) {
    return self->animationTime >= frame && frame > self->animationTime - self->scale;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateGroundImpactReaction(FightActorRuntimePrefix *actor) {
    std::int32_t reacting = 0;
    if (!(actor->moveFlags & 0x8000U) || (actor->activeMotionFlags & 1U))
        return;
    if ((actor->hitStateFlags & 0x80000U) && actor->game->elapsedFrames) {
        const std::int32_t damage = MOTION_VALUE(std::uint8_t, actor, 0x2aa);
        MOTION_VALUE(std::uint16_t, actor, 0x3a0) = static_cast<std::uint16_t>(
            MOTION_VALUE(float, actor, 0x270) * (static_cast<float>(MOTION_VALUE(std::int16_t, actor, 0x3a2)) + static_cast<float>(damage) * 0.5f));
        MOTION_VALUE(std::int16_t, actor, 0x3a2) = 0;
        if (damage)
            vfHandleImpactSfxAndDefeat(actor, damage, 0);
    }
    std::int16_t angle = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(MOTION_VALUE(float, actor, 0x454), MOTION_VALUE(float, actor, 0x45c)));
    std::int16_t facing;
    if (!(actor->hitStateFlags & 0x80000U))
        facing = static_cast<std::int16_t>(actor->facingAngle);
    else
        facing = static_cast<std::int16_t>(actor->temporaryFacingAngle);
    angle = static_cast<std::int16_t>(angle - facing);
    if (angle < 0)
        angle = static_cast<std::int16_t>(-angle);
    if (angle >= -0x4000 && angle <= 0x4000) {
        if (static_cast<std::uint16_t>(MOTION_VALUE(std::int16_t, actor, 0x182) + 0x2000) <= 0x4000U && !(actor->hitStateFlags & 0x800U) &&
            !(actor->contactPosition.y < actor->targetGroundY + 0.8f)) {
            float threshold = 0.6f;
            const std::uint16_t animation =
                threshold > MOTION_VALUE(float, actor, 0x248) ? 352 : (actor->contactPosition.y > actor->targetGroundY + 1.7f ? 353 : 354);
            MOTION_VALUE(std::uint16_t, actor, 0x2d8) = animation;
            reacting = 1;
        }
    } else if (static_cast<std::uint16_t>(MOTION_VALUE(std::int16_t, actor, 0x180) + 0x2000) <= 0x4000U && (actor->hitStateFlags & 0x800U) &&
        !(actor->contactPosition.y < actor->targetGroundY + 0.8f)) {
        MOTION_VALUE(std::uint16_t, actor, 0x2d8) = 156;
        reacting = 1;
    }
    if (reacting == 1) {
        actor->contactPosition.x = actor->position.x;
        actor->contactPosition.z = actor->position.z;
        if (MOTION_VALUE(float, actor, 0x458) > 0)
            MOTION_VALUE(float, actor, 0x458) *= 0.4f;
        actor->temporaryFacingAngle = static_cast<std::uint16_t>(actor->temporaryFacingAngle + actor->facingAngleDelta);
        MOTION_VALUE(std::uint16_t, actor, 0x27e) = 31;
        MOTION_VALUE(float, actor, 0x278) = MOTION_VALUE(float, actor, 0x274);
    }
}

#else

// Terrain impact can replace the pending landing reaction.
// @verified - p
void vfUpdateGroundImpactReaction(VFActor *self) {
    if (!(self->moveFlags & VFMF_AirborneOrFalling) || (self->activeMotionFlags & VFSMF_Unknown1))
        return;
    if ((self->hitStateFlags & VFHSF_AirborneOrDowned) && self->game->guardTimer > 0) {
        const auto damage = static_cast<std::uint8_t>(self->hitDamage);
        const auto accumulated = static_cast<std::int16_t>(self->parameter40c >> 16);
        self->parameter40c = static_cast<std::uint16_t>(int((float(damage) * 0.5f + float(accumulated)) * self->inverseScale));
        if (damage)
            vfHandleImpactSfxAndDefeat(self, damage, 0);
    }
    const auto direction = static_cast<std::int16_t>(FixedAtan2(self->knockbackVelocity.x, self->knockbackVelocity.z));
    const auto relative =
        static_cast<std::int16_t>(direction - ((self->hitStateFlags & VFHSF_AirborneOrDowned) ? self->temporaryFacingAngle : self->facingAngle));
    std::int16_t animation;
    const auto magnitude = static_cast<std::uint16_t>(relative < 0 ? -int(relative) : int(relative));
    if (static_cast<std::uint16_t>(magnitude + 0x4000) > 0x8000) {
        if (static_cast<std::uint16_t>(self->forwardIntersectionTag + 0x2000) > 0x4000 || !(self->hitStateFlags & VFHSF_ReversedFacing) ||
            !(self->contactPosition.y >= self->targetGroundY + 0.8f))
            return;
        animation = 156;
    } else {
        if (static_cast<std::uint16_t>(self->backwardIntersectionTag + 0x2000) > 0x4000 || (self->hitStateFlags & VFHSF_ReversedFacing) ||
            !(self->contactPosition.y >= self->targetGroundY + 0.8f))
            return;
        animation = !(0.6f > ActorValue<float>(self, 0x280)) ? (self->contactPosition.y > self->targetGroundY + 1.7f ? 353 : 354) : 352;
    }
    self->queuedReactionAnimation = static_cast<std::uint16_t>(animation);
    self->contactPosition.x = self->position.x;
    self->contactPosition.z = self->position.z;
    if (self->knockbackVelocity.y > 0)
        self->knockbackVelocity.y *= 0.4f;
    self->temporaryFacingAngle = static_cast<std::uint16_t>(self->temporaryFacingAngle + self->facingAngleDelta);
    self->queuedReactionEvent = 31;
    self->knockbackAcceleration = self->timeScale;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfPredictLandingGround(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *game = actor->game;
    std::uint8_t frames;
    float vertical, ground;
    Vector3 point;
    MOTION_VALUE(float, actor, 0x400) = actor->targetGroundY;
    if (MOTION_VALUE(float, actor, 0x454) == 0 && MOTION_VALUE(float, actor, 0x45c) == 0)
        return;
    point.x = actor->contactPosition.x + MOTION_VALUE(float, actor, 0x454) * MOTION_VALUE(std::int16_t, actor, 0x3be);
    point.y = actor->contactPosition.y;
    point.z = actor->contactPosition.z + MOTION_VALUE(float, actor, 0x45c) * MOTION_VALUE(std::int16_t, actor, 0x3be);
    ground = vfFieldGetIntersection(0, point);
    if (ground == actor->targetGroundY)
        return;
    if (actor->targetGroundY > ground) {
        frames = MOTION_VALUE(std::uint8_t, actor, 0x3be);
        vertical = -MOTION_VALUE(float, actor, 0x458);
    } else {
        frames = 0;
        vertical = MOTION_VALUE(float, actor, 0x458);
        point.x = actor->contactPosition.x;
        point.z = actor->contactPosition.z;
    }
    point.y = actor->contactPosition.y;
    for (;;) {
        point.x += MOTION_VALUE(float, actor, 0x454);
        point.z += MOTION_VALUE(float, actor, 0x45c);
        vertical = vertical - MOTION_VALUE(float, actor, 0x26c) * MOTION_VALUE(float, actor, 0x26c) * game->gravityScale + MOTION_VALUE(float, actor, 0x464);
        if (!(0 > vertical))
            continue;
        ground = vfFieldGetIntersection(0, point);
        point.y += vertical;
        if (point.y - MOTION_VALUE(float, actor, 0x2a4) < ground + 0.02f && 0 > vertical) {
            MOTION_VALUE(std::uint16_t, actor, 0x3be) = frames;
            MOTION_VALUE(float, actor, 0x400) = ground;
            return;
        }
        ++frames;
    }
}

#else

// Predict the native terrain intersection along the knockback arc.
// @verified - p
void vfPredictLandingGround(VFActor *self) {
    self->groundParameter = std::bit_cast<std::uint32_t>(self->targetGroundY);
    if (self->knockbackVelocity.x == 0 && self->knockbackVelocity.z == 0)
        return;
    const float count = static_cast<std::int16_t>(self->predictedKnockbackFrames);
    Vector3 point{
        count * self->knockbackVelocity.x + self->contactPosition.x, self->contactPosition.y, count * self->knockbackVelocity.z + self->contactPosition.z};
    const float intersection = vfFieldGetIntersection(nullptr, point);
    if (intersection == self->targetGroundY)
        return;
    float vertical = self->knockbackVelocity.y;
    std::int16_t frames;
    float x, z;
    if (!(self->targetGroundY > intersection)) {
        x = self->contactPosition.x;
        z = self->contactPosition.z;
        frames = 0;
    } else {
        frames = static_cast<std::int16_t>(self->predictedKnockbackFrames);
        vertical = -vertical;
        x = point.x;
        z = point.z;
    }
    point.y = self->contactPosition.y;
    float ground;
    for (;;) {
        do {
            x += self->knockbackVelocity.x;
            z += self->knockbackVelocity.z;
            point.x = x;
            point.z = z;
            vertical = (vertical - self->scale * self->scale * self->game->gravityScale) + self->slideVelocity.y;
        } while (!(0 > vertical));
        ground = vfFieldGetIntersection(nullptr, point);
        point.y += vertical;
        if (ground + 0.02f > point.y - self->groundOffset)
            break;
        x = point.x;
        z = point.z;
        frames = static_cast<std::int16_t>(static_cast<std::uint16_t>(frames) + 1);
    }
    self->predictedKnockbackFrames = static_cast<std::uint16_t>(frames);
    self->groundParameter = std::bit_cast<std::uint32_t>(ground);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Retain low action bits while rotating the authored sector into view space.
// @verified - p
std::uint16_t vfRotateCameraSectorCode(VFActor *self, std::uint16_t code) {
    const auto sector = code & 0xf0;
    if (!sector)
        return code;
    const auto angle = static_cast<std::int16_t>(self->facingYaw - camCtrlGetNextRotY() + 4096);
    const auto rotation = (angle >> 13) & 7;
    std::int32_t index = 0;
    while (cameraSectors[index] != sector) {
        if (++index >= 8) {
            index = -1;
            break;
        }
    }
    return (code & 0xff0f) | cameraSectors[(rotation + index) & 7];
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Select from borrowed 17-byte authored phase-chain records.
// @verified - p
void vfSelectAICommand(VFActor *self, std::uint8_t choice, std::uint16_t delay) {
    auto &slot = *self->slot;
    if (!(slot.cameraFlags & 1))
        return;
    slot.parameter89 = choice;
    slot.cameraFlags &= ~1U;
    slot.parameter8c = 0;
    slot.parameterCc = static_cast<float>(delay);
    slot.randomBytes[0] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    slot.parameter12a = 0;
    slot.resetIndex = 0x7f7f;
    const auto *record = reinterpret_cast<const std::uint8_t *>(self->motionParameters[5]);
    while (*record != 127) {
        if (*record == choice) {
            std::int8_t count = 4;
            if (record[13] == 127)
                --count;
            if (record[9] == 127)
                --count;
            if (record[5] == 127)
                --count;
            if (record[1] == 127)
                --count;
            if (count > 0) {
                const auto pick = static_cast<std::uint8_t>(IncrementSharedPrngUInt16()) % static_cast<std::uint8_t>(count);
                slot.resetIndex = record[4 * pick + 1] | (static_cast<std::uint16_t>(record[4 * pick + 3]) << 8);
                slot.reserved62[1] = static_cast<std::byte>(record[4 * pick + 2]);
                slot.reserved62[2] = static_cast<std::byte>(record[4 * pick + 4]);
            }
            return;
        }
        record += 17;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Advance the native code/duration script without replacing its clock.
// @verified - p
bool vfUpdateAIInput(VFActor *self) {
    auto &slot = *g_pFightMain->actorSlots[self->groupIndex];
    auto step = static_cast<std::uint8_t>(slot.parameter89 >> 8);
    if (slot.parameter8c) {
        if (slot.parameter8c != 1) {
            self->inputFlags = slot.parameter8e;
            --slot.parameter8c;
            return false;
        }
        slot.parameter89 = (slot.parameter89 & 0xff) | (static_cast<std::uint16_t>(static_cast<std::uint8_t>(step + 1)) << 8);
        if (step == 10) {
            slot.parameter8c = 0;
            return true;
        }
        ++step;
    }
    const auto choice = static_cast<std::uint8_t>(slot.parameter89);
    const auto *record = choice >= 200 ? defaultCameraScripts[choice - 200] : reinterpret_cast<const std::uint8_t *>(self->motionParameters[1]) + 20 * choice;
    std::uint16_t code = record[step];
    if (code == 127)
        return true;
    if (self->groupIndex == g_pFightMain->playerSlot && !(g_pFightMain->gameFlags & 0x400000) && (code & 0xf0) != 0xf0)
        code = vfRotateCameraSectorCode(self, code);
    self->inputFlags = code;
    slot.parameter8e = code;
    slot.parameter8c = record[step + 10];
    return false;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Choose direct continuation or a new authored chain at the native phase gate.
// @verified - p
void vfAdvanceAICommand(VFActor *self) {
    auto &slot = *self->slot;
    if (slot.parameterCc > 0) {
        slot.parameterCc -= shenGetScaledDeltaTickTime(1);
        if (slot.parameterCc < 0)
            slot.parameterCc = 0;
        return;
    }
    if (!vfUpdateAIInput(self))
        return;
    slot.cameraFlags &= ~0x2000U;
    const auto index = slot.parameter12a;
    const auto *raw = reinterpret_cast<const std::uint8_t *>(&slot);
    const auto choice = raw[0x60 + index];
    if (choice == 127) {
        ResetCameraPhase(slot);
        return;
    }
    const auto delay = raw[0x63 + index];
    slot.parameter12a = static_cast<std::uint8_t>(index + 1);
    if (slot.randomBytes[1] & 7) {
        slot.parameter89 = choice;
        slot.parameter8c = 0;
        slot.parameterCc = static_cast<float>(delay);
        slot.randomBytes[0] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    } else {
        ResetCameraPhase(slot);
        vfSelectAICommand(self, choice, delay);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfSetFightFaceExpression(Character *character, std::int32_t expression, std::int32_t frames) {
    const std::int32_t expressions[11] = {1, 3, 0, 0, 0, 0, 0, 3, 11, 6, 4};
    SetHighModelAndFaceFlagsForChara(character, expressions[expression], frames, 1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t vfFightVoiceStyle(std::uint32_t style) {
    switch (style) {
    case 0:
    case 16:
    case 46:
        return 0;
    case 11:
        return 1;
    case 12:
        return 2;
    case 10:
        return 3;
    case 8:
        return 4;
    case 25:
        return 5;
    case 24:
        return 6;
    case 23:
        return 7;
    case 26:
    case 45:
        return 8;
    case 2:
    case 6:
    case 35:
        return 11;
    case 3:
    case 36:
        return 12;
    case 4:
    case 38:
        return 10;
    case 22:
        return 13;
    default:
        return 9;
    }
}

#else

// Fight style is not the voice-row index.
// @verified - p
std::uint8_t vfFightVoiceStyle(std::uint16_t style) {
    switch (style) {
    case 0:
    case 16:
    case 46:
        return 0;
    case 2:
    case 6:
    case 35:
        return 11;
    case 3:
    case 36:
        return 12;
    case 4:
    case 38:
        return 10;
    case 8:
        return 4;
    case 10:
        return 3;
    case 11:
        return 1;
    case 12:
        return 2;
    case 19:
        return 16;
    case 22:
        return 13;
    case 23:
        return 7;
    case 24:
        return 6;
    case 25:
        return 5;
    case 26:
    case 45:
        return 8;
    case 48:
        return 15;
    default:
        return 9;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfStartAuthoredDefeatCamera(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *game;
    FightActorRuntimePrefix *linked;
    game = actor->game;
    linked = MOTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c4);
    game->stateFlags |= 0x80000U;
    game->authCameraFrame = 0;
    MOTION_VALUE(std::uint32_t, actor, 0x318) = MOTION_VALUE(std::uint16_t, game, 0x46a) | 0x8000000U;
    MOTION_VALUE(std::uint32_t, actor, 0x324) = 0;
    vfPrepareAuthoredDefeatCamera();
    game->stateFlags &= ~1U;
    camCtrlSetInterpModeTo(8);
    vfSetupLinkedFightCamera(linked, 2);
    actor->moveFlags |= 0x10000000U;
    linked->moveFlags |= 0x10000000U;
    MOTION_VALUE(std::uint32_t, linked, 0x360) |= 1U;
}

#else

// The authored counterpart at B00 owns the defeat camera pairing.
// @verified - p
void vfStartAuthoredDefeatCamera(VFActor *self) {
    using enum CameraState;
    auto &game = *self->game;
    auto *const linked = self->pairedActor;
    std::uint16_t animation;
    std::memcpy(&animation, reinterpret_cast<const std::byte *>(&game) + 0x56e, sizeof(animation));
    game.mode |= 0x80000;
    game.parameter570 = 0;
    self->parameter370 = animation | 0x08000000ULL;
    self->deferredReaction = 0;
    linked->hitPhase = 99;
    linked->parameter370 = linked->deferredReaction = 0;
    if (camCtrlCheckIsEventCam() && !(g_pFightStage->battleFlags & 2)) {
        resetCharaPosYandIntersection();
        camCtrlSetFovTransitionTo(false);
        camCtrlSetState(ChaseGoal);
    }
    game.mode &= ~1U;
    camCtrlSetInterpModeTo(CameraMotionMode::AutoQuadratic2);
    vfSetupLinkedFightCamera(linked, 2);
    self->moveFlags |= VFMF_RefreshContactHistory;
    linked->moveFlags |= 0x10000000;
    game.mode |= 0x4000;
    linked->pairedContactPriority |= 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfPlayFightSfx(std::uint32_t sound, FightActorRuntimePrefix *actor) {
    Character *const character = actor->character;
    FightRuntimePrefix *const game = actor->game;
    if (!character || (game->stateFlags & 8U) || ((game->stateFlags & 1U) && camCtrlGetInterpMode() != 4))
        return;
    std::uint32_t material;
    if (g_pFightStage->battleFlags & 0x40U)
        material = game->cameraParameter;
    else
        material = static_cast<std::uint32_t>(static_cast<std::int16_t>(GetFieldSoundProperties(&actor->position) >> 16U));
    if (camCtrlGetInterpMode() == 4 && sound - 37U < 3U)
        sound = 1704;
    PlayFootstepForNode((material << 19U) | 0x7f000000U | sound, character);
}

#else

// The existing native sound command owner carries the voice gate.
// @verified - p
void vfPlayFightSfx(std::uint32_t sound, VFActor *self, std::uint8_t mode) {
    const auto cameraMode = camCtrlGetInterpMode();
    if (!self->character || (self->game->mode & 8) || ((self->game->mode & 1) && cameraMode != CameraMotionMode::AutoQuadratic))
        return;
    // Native clamps the arithmetic-shifted material word as an unsigned 32-bit value.
    std::uint32_t material = (g_pFightStage->battleFlags & 0x40) ?
        static_cast<std::uint8_t>(self->game->cameraParameter) :
        static_cast<std::uint32_t>(static_cast<std::int32_t>(QueryGroundMaterialToken(self->position, 0)) >> 16);
    if (material > 13)
        material = 13;
    if (cameraMode == CameraMotionMode::AutoQuadratic && sound - 37U <= 2U)
        sound = 1704;
    PlayFootstepForNode(self->character, sound | ((material | 0xfe0U) << 19), mode != 0);
}

#endif

} // namespace s2
} // namespace shenmue
