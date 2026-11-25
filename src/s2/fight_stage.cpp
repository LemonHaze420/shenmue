#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/fight_stage.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/hlib.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/math.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "common/native_sqrt.hpp"

#include <cmath>
#include <cstring>
#include <cstdio>
#include "s2/motion_registry_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/model_dc.hpp"
#include "s2/fight.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/texture_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/window.hpp"
#include "s2/conversation_dc.hpp"
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
#include "s2/conversation_playback_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/conversation_fade_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#else
#include "s2/fight_stage.hpp"

#include "s2/character.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/weather.hpp"
#include "s2/input.hpp"
#include "s2/player.hpp"
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <xmmintrin.h>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

FightEntry g_activeFightData[16];

FightStageObjectDefinition *g_fightStageObjectDefinitions[8];

VOBJ_t *g_fightStageObjects[30]{};

VOBJ_t *g_fightStageObjectRoot{};

float g_fightStageObjectMaximumDistance;

VFMA_t *g_pFightMain{};

namespace {

constexpr std::int32_t fightMotionSelectors[7]{25, 30, 0, 2, 7, 10, 17};

// Native g_VF_move_category_masks contains forty-four entries.
constexpr std::uint32_t fightMoveCategoryMasks[44]{0, 8, 0x40, 0x10, 0x80, 0x30, 0x180, 0x800, 0x4000, 0x1800, 0xc000, 0x1800, 0xc000, 0x1b0, 0x1c00, 0xe000, 2,
    4, 0x202, 6, 0x202, 0xd800, 0x18, 0xc0, 0x1a, 0xc2, 0x38, 0x1c0, 0xd800, 0x4800, 0x1830, 0xc180, 0x42, 0xa, 0x2602, 0x4c, 0x5000, 0x8800, 0xd800, 0x1b0,
    0x1f8, 0xffffffffU, 0xfe02, 0x3fe};

// Default offset words are immediately followed by node indices; definition index 8 reads those adjacent words.
constexpr std::uint32_t stageObjectDefaults[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3e4ccccdU, 0, 152, 153, 154, 155, 156, 157,
    158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
    189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 0, 0};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
VFActor *AllocateFGTM();
VFSlotRuntimeState *AllocateFENE();
VFGW_t *AllocateVFGW(VFActor *actor);
void mtwkRemoveCallback(MotionWork *motion);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, VFActor *actor);
void vfUpdateHitFacingAndKnockback(VFActor *self);
void vfUpdateContactSlotsAndGroundFlags(VFActor *self);
void vfPrepareEnemySlotMotion(FCMG_t *group);
void vfRestoreEnemySlotMotion(FCMG_t *group);
void vfSelectDefaultReactionAndGuardAnims(VFActor *actor);
void vfUpdateDefaultReactionAnim(FCMG_t *group, std::uint16_t animation);
void vfRemoveCharaFromGroup(VFMA_t *game, FCMG_t *group);
bool ReactToDead(VFMA_t *game, FCMG_t *slot);
bool vfEnemySlotResetAndRestartCycle(VFMA_t *game, FCMG_t *group);
float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);
VFMA_t *vfmaGetTaskParam();
void vfInitStageObjects();
void vfFreeStageObjects();
FightStageObjectDefinition **AllocateVOBJ(std::int32_t slot, std::int8_t index);
const std::byte *vfStageDefinitionSetMotion(std::int32_t slot, std::uint32_t name, std::int32_t tracks, const std::uint32_t *nodes, std::int32_t step,
    std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound);
void vfStageDefinitionAddHitbox(std::int32_t slot, std::uint32_t flags, std::uint16_t kindAndState, float x, float y, float z, float radius);
std::uint64_t vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound, std::uint32_t launchSound, std::uint32_t flags,
    std::int32_t launchYaw, std::int32_t rotationSpeed, float coefficient, float groundOffset);
std::uint64_t vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::uint32_t rotationZ);
std::uint32_t ToggleVOBJArrayIndex(std::int32_t slot, std::int8_t hitDamage, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound);
std::int64_t vfStageDefinitionAddContactMask(std::int32_t slot, std::uint32_t flags);
FightStageObjectDefinition *vfAllocateVOBJ(std::int32_t slot, Character *character, std::int32_t definition);
std::int32_t vfStageObjectAssignMotFlags(std::int32_t slot, std::uint32_t flags);
std::uint8_t vfStageObjectSetBrokenModel(std::int32_t slot, Character *character, std::uint32_t flags);
void vfInitializeStageObjectHitboxes();
void vfStageObjectAssignPosXYZ(std::int32_t slot, float x, float y, float z);
void vfStageObjectAssignRotXYZ(std::int32_t slot, std::uint16_t x, std::uint16_t y, std::uint16_t z);
VOBJ_t *vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z);
void vfStageObjectReset(std::int32_t index, Character *character);
void vfLaunchStageObject(std::int32_t index, std::uint16_t angle);
void vfToggleAttachedStageObjectDisplay(VFActor *actor);
void vfSetAttachedStageObjectEffect(VFActor *actor, std::int8_t effect, float amount, std::int16_t frames);
void vfSetAttachedStageObjectFigurePose(VFActor *actor, std::int8_t scale, std::int16_t direction);
float flddFindIntersectionBy(const Vector3 &position);
float flddGetIntersectionFor(Character *character, Vector3 &position);
float vfFieldGetIntersection(VFActor *actor, Vector3 &position);
void EmitFightSmoke(std::uint8_t profile, VFActor *actor);

namespace {

void AdjustCooldownGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *, void *context);
void AdjustActiveGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *context);

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

struct FightSmokeProfileNative {
    float horizontalVariance, reservedVariance, birthRate;
    std::uint8_t particleCount, ticks;
    std::uint16_t retained0e;
};

} // namespace

void vfStageObjectReset(std::int32_t slot, Character *character);

void vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z);

float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);

float flddGetIntersectionFor(Character *character, Vector3 &position);

FightRuntimePrefix *vfmaGetTaskParam();

void vfClAicaReact(std::uint32_t flags, const Vector3 *position);

FightStageObjectDefinition *g_fightStageObjectDefinitions[8];

VOBJ_t *g_fightStageObjects[30] = {};

VOBJ_t *g_fightStageObjectRoot = 0;

float g_fightStageObjectMaximumDistance;

namespace {

static const FightSmokeProfileNative fightSmokeProfiles[10] = {{0.01f, 0.02f, 0.2f, 10, 75, 0}, {0.01f, 0.02f, 0.2f, 10, 75, 0},
    {0.01f, 0.02f, 0.2f, 10, 75, 0}, {0.005f, 0.18f, 0.3f, 25, 40, 0}, {0.02f, 0.08f, 0.2f, 20, 40, 0}, {0.014f, 0.014f, 0.2f, 8, 80, 0},
    {0.008f, 0.0008f, 0.2f, 3, 15, 0}, {0.008f, 0.0008f, 0.2f, 3, 15, 0}, {0.008f, 0.0008f, 0.2f, 3, 15, 0}, {0.008f, 0.0008f, 0.2f, 3, 15, 0}};

static const std::uint8_t fightSmokePaletteIndices[15] = {7, 7, 7, 7, 7, 7, 8, 7, 8, 9, 8, 9, 1, 1, 1};

static const float fightSmokeColours[10][4] = {{0, 0, 0, 0}, {0, 208, 208, 208}, {0, 80, 80, 80}, {0, 128, 128, 112}, {0, 96, 96, 96}, {0, 113, 107, 94},
    {0, 113, 107, 94}, {0, 160, 160, 144}, {0, 128, 128, 112}, {0, 96, 96, 80}};

} // namespace

FightEntry g_activeFightData[16];

namespace {

static const std::uint32_t fightMoveCategoryMasks[44] = {0, 8, 0x40, 0x10, 0x80, 0x30, 0x180, 0x800, 0x4000, 0x1800, 0xc000, 0x1800, 0xc000, 0x1b0, 0x1c00,
    0xe000, 2, 4, 0x202, 6, 0x202, 0xd800, 0x18, 0xc0, 0x1a, 0xc2, 0x38, 0x1c0, 0xd800, 0x4800, 0x1830, 0xc180, 0x42, 0xa, 0x2602, 0x4c, 0x5000, 0x8800, 0xd800,
    0x1b0, 0x1f8, 0xffffffffU, 0xfe02, 0x3fe};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void PrepareFightFieldNode(CharacterNode *) {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void PrepareFightFieldNode(CharacterNode *);
void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);
void vfResetStageObjectSlot(VOBJ_t *object);
void vfAttachActorStageObject(FightActorRuntimePrefix *actor);
void vfReleaseActorStageObject(FightActorRuntimePrefix *actor);
void vfLaunchStageObject(std::int32_t index, std::int32_t angle);
void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor);
void vfLaunchActorStageObject(FightActorRuntimePrefix *actor);
void vfTossActorStageObject(FightActorRuntimePrefix *actor);
void vfTransformStageObjectHitboxes(VOBJ_t *owner);
std::int32_t IsActorBoneNearStageObject(FightActorRuntimePrefix *actor, VOBJ_t *owner);
std::int32_t AreStageObjectsNear(VOBJ_t *first, VOBJ_t *second);
void vfInitStageObjects();
void vfFreeStageObjects();
void AllocateVOBJ(std::int32_t slot, std::int8_t index);
void vfStageDefinitionSetMotion(std::int32_t slot, shen_name name, std::int32_t tracks, const ObjectMotionNode *nodes, std::int32_t step,
    std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound);
void vfStageDefinitionAppendHitbox(std::int32_t slot, std::uint32_t flags, const FightStageObjectHitbox *record);
void vfStageDefinitionAddHitbox(std::int32_t slot, std::uint32_t flags, std::int32_t kindAndState, float x, float y, float z, float radius);
void ToggleVOBJArrayIndex(std::int32_t slot, std::int8_t hitDamage, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound);
void vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound, std::uint32_t launchSound, std::uint32_t flags, std::int32_t launchYaw,
    std::int32_t rotationSpeed, float coefficient, float groundOffset);
void vfStageDefinitionSetAttachmentData(std::int32_t slot, std::uint8_t bone, const Vector3 *position, const Vector3i *rotation);
void vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::int32_t rotationZ);
void vfStageDefinitionAddContactMask(std::int32_t slot, std::uint32_t flags);
void vfAllocateVOBJ(std::int32_t slot, Character *character, std::int32_t definition);
void vfStageObjectSetBrokenModel(std::int32_t slot, Character *character, std::uint32_t flags);
void vfStageObjectAssignPosXYZ(std::int32_t slot, float x, float y, float z);
void vfStageObjectAssignRotXYZ(std::int32_t slot, std::uint32_t x, std::uint32_t y, std::uint32_t z);
void vfStageObjectAssignMotFlags(std::int32_t slot, std::uint32_t flags);
void vfInitializeStageObjectHitboxes();
void vfResetStageObjectRotationImpulse(VOBJ_t *object);
void vfStageObjectReset(std::int32_t slot, Character *character);
void vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z);
void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames);
void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction);
FightActorRuntimePrefix *AllocateFGTM();
void *AllocateFENE();
void AllocateVFGW(FightActorRuntimePrefix *actor);
float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);
float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position);
std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor);
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor);
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot);
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor);
void vfResetActorActionState(FightActorRuntimePrefix *actor);
float flddFindIntersectionBy(const Vector3 &position);
float flddGetIntersectionFor(Character *character, Vector3 &position);
void vfStageObjectRestartMotion(std::int32_t index);
void FightAreaMaskNullsub(std::uint8_t count);

namespace {

void AdjustCooldownGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *, void *context);
void AdjustActiveGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *context);

} // namespace
#else
void EmitFightSmoke(std::uint8_t profile, VFActor *actor);
std::int32_t GetBtlStatus(Character *character);
void EnsureLoadedFightStyle(std::uint8_t style, std::uint8_t slot);
std::uint8_t PublishFightFormation(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style,
    std::uint16_t spawnWord0, std::uint16_t spawnWord1, float x, float y, float z);
std::int64_t PublishFightRespawn(std::uint16_t hp, std::uint8_t style, std::uint16_t spawnWord0, float x, float y, float z, std::uint16_t spawnWord1);
bool vfgGameInfoEntry(HLib::Task *source, HLib::Task *target);
void vfg_Callback_GameInfoEntry_callback(void *data);
HLib::Task *vfg_Callback_GameInfoEntry_Enqueue(VFMA_t *game);
void vfInitSfxSlotState(VFMA_t *game);
std::uint8_t AccessFightActorFlag411(std::uint8_t mode, Character *character, std::uint8_t value);
float AccessFightActorGroupFloat(std::uint8_t mode, Character *character, float value);
void SetFightActorParameterBytes(shen_name character, const std::uint8_t *values);
void SuspendFight();
std::int32_t ResumeFight(std::int32_t resetActors);
void vfLaunchStageObject(std::int32_t index, std::uint16_t angle);
void vfToggleAttachedStageObjectDisplay(VFActor *actor);
void vfInitStageObjects();
void vfFreeStageObjects();
FightStageObjectDefinition **AllocateVOBJ(std::int32_t slot, std::int8_t index);
const std::byte *vfStageDefinitionSetMotion(std::int32_t slot, std::uint32_t name, std::int32_t tracks, const std::uint32_t *nodes, std::int32_t step,
    std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound);
void vfStageDefinitionAddHitbox(std::int32_t slot, std::uint32_t flags, std::uint16_t kindAndState, float x, float y, float z, float radius);
std::uint32_t ToggleVOBJArrayIndex(std::int32_t slot, std::int8_t hitDamage, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound);
std::uint64_t vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound, std::uint32_t launchSound, std::uint32_t flags,
    std::int32_t launchYaw, std::int32_t rotationSpeed, float coefficient, float groundOffset);
std::uint64_t vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::uint32_t rotationZ);
std::int64_t vfStageDefinitionAddContactMask(std::int32_t slot, std::uint32_t flags);
FightStageObjectDefinition *vfAllocateVOBJ(std::int32_t slot, Character *character, std::int32_t definition);
std::uint8_t vfStageObjectSetBrokenModel(std::int32_t slot, Character *character, std::uint32_t flags);
void vfStageObjectAssignPosXYZ(std::int32_t slot, float x, float y, float z);
void vfStageObjectAssignRotXYZ(std::int32_t slot, std::uint16_t x, std::uint16_t y, std::uint16_t z);
std::int32_t vfStageObjectAssignMotFlags(std::int32_t slot, std::uint32_t flags);
void vfInitializeStageObjectHitboxes();
void vfStageObjectReset(std::int32_t index, Character *character);
VOBJ_t *vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z);
void vfSetAttachedStageObjectEffect(VFActor *actor, std::int8_t effect, float amount, std::int16_t frames);
void vfSetAttachedStageObjectFigurePose(VFActor *actor, std::int8_t scale, std::int16_t direction);
VFActor *AllocateFGTM();
VFSlotRuntimeState *AllocateFENE();
VFGW_t *AllocateVFGW(VFActor *actor);
void FreeVFGW(VFActor *actor);
VCDW_t *AllocateVCDW(VFMA_t *game);
void vfUpdateWorldTransformAndFacingAngle(VFActor *actor);
void vfApplyYawRecursive(VFActor *actor);
float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);
void vfAssignWaveSpawnFromGenerator(FRMG_t *formation, float x, float z);
bool vfTransitionEnemySlotToInactive(VFMA_t *game, FCMG_t *group);
void vfClearEnemySlotStackRefs(std::uint8_t slotIndex);
void vfUpdateEnemySlotCooldowns(VFMA_t *game);
void vfUpdateRenderOpacityForAll(VFMA_t *game);
void vfTransitionAllEnemySlotsToInactive(VFMA_t *game);
void vfUpdateEnemySlotSpawnFade(VFMA_t *);
void vfUpdatePairingsAndDistances(VFMA_t *game);
VFMA_t *vfmaGetTaskParam();
VFMA_t *vfmaSetGaugeSuppressed(std::int32_t suppressed);
std::uint64_t GetFightPlayerGuardEffectTimerSeconds();
std::uint32_t GetFightActorFlags(Character *character);
std::uint64_t GetFightCounterHighWord();
std::uint64_t GetBattleDialogueCompletionCount(std::uint32_t id);
void RemoveBattleDialogue(std::uint32_t id);
std::uint32_t GetFightStageObjectFlags();
void SetFightCounterHighWord(std::uint16_t value);
std::uint32_t SetFightFlag400000(std::int32_t enabled);
float vfFieldGetIntersection(VFActor *actor, Vector3 &position);
void mtwkRemoveCallback(MotionWork *motion);
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, VFActor *actor);
std::uint8_t vfRegisterContactSlot(VFMA_t *game, FCMG_t *slot);
void vfApplyKnockbackPhysics(VFActor *self);
void vfUpdatePositionXZ(VFActor *self);
void vfResolveKnockbackLanding(VFActor *self);
void vfUpdateAutoAdvanceLanding(VFActor *self);
void vfUpdateAirborneAnimationTiming(VFActor *self);
void vfUpdateGroundHeightAndEnemyContacts(VFActor *self);
void vfUpdateHitFacingAndKnockback(VFActor *self);
void vfRefreshContactReactionState(VFActor *actor);
void vfUpdateContactSlotsAndGroundFlags(VFActor *self);
void vfUpdateSlotRuntimeMetricsAndTimers(VFActor *self);
void vfUpdateSlotReactionHistory(VFActor *self);
void vfUpdateSlotRankParameters(VFActor *self);
void vfUpdateCurrentAssistWindow(VFActor *self);
void vfUpdateScenarioHPPhaseConfig(VFActor *self);
void vfInitializeSlotPhaseConfigurations(VFActor *self);
void vfUpdateGuardLevel(VFMA_t *game, std::uint8_t index);
void vfPrepareEnemySlotMotion(FCMG_t *group);
void vfRestoreEnemySlotMotion(FCMG_t *group);
void vfSelectDefaultReactionAndGuardAnims(VFActor *actor);
void vfUpdateDefaultReactionAnim(FCMG_t *group, std::uint16_t animation);
std::uint8_t vfSelectOpponentSlotByPriority(VFMA_t *game, std::uint8_t minimumPriority);
void vfUpdateEnemySlotFormationAndYaw(VFMA_t *);
void vfRemoveCharaFromGroup(VFMA_t *game, FCMG_t *group);
bool ReactToDead(VFMA_t *game, FCMG_t *slot);
bool vfEnemySlotResetAndRestartCycle(VFMA_t *game, FCMG_t *group);
void vfCaptureGroundForCooldown(VFMA_t *game, VFActor *actor);
bool vfTransitionEnemySlotToCooldown(VFMA_t *game, FCMG_t *group);
float flddFindIntersectionBy(const Vector3 &position);
float flddGetIntersectionFor(Character *character, Vector3 &position);

namespace {

bool BindAllocatedEnemySlot(FCMG_t *slot);
void AdjustCooldownGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *, void *context);
void AdjustActiveGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *context);
float FightIntersectionDistance(const VFActor &actor, std::uint16_t angle, float length);
std::uint16_t PredictFightKnockbackFrames(const VFActor &actor, float height);
Vector3 FightBoneLocalPosition(Character *character, std::int32_t bone);
void CaptureFightHitTransform(Character *character, std::int32_t bone, Matrix4x4 &output);
std::uint16_t FightFootFacingAngle();

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t vfGetMoveCategoryMaskFor(FightActorRuntimePrefix *actor) {
    if (actor->hitStateFlags & 0x100U)
        return fightMoveCategoryMasks[actor->moveId];
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t vfSelectHitReactionSequence(FightActorRuntimePrefix *actor) {
    const std::uint32_t mask = vfGetMoveCategoryMaskFor(actor);
    std::uint32_t result = 0;
    if (mask & 0x1c0U)
        result |= 4U;
    if (mask & 0x38U)
        result |= 8U;
    if (mask & 0xe000U)
        result |= 0x10U;
    if (mask & 0x1c00U)
        result |= 0x20U;
    if (mask & 3U)
        result |= 2U;
    if (mask & 4U)
        result |= 1U;
    return actor->hitStateFlags & 0x100U ? result : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void *AllocateFENE() {
    void *const slot = HLib::AllocateTaskData(0x188, SHEN_NAME(FENE));
    HLib::AssignDynamicAlignmentForTaskData(slot);
    std::memset(slot, 0, 0x188);
    return slot;
}

#else

// @verified - p
VFSlotRuntimeState *AllocateFENE() {
    VFSlotRuntimeState *const slot = static_cast<VFSlotRuntimeState *>(HLib::AllocateTaskData(sizeof(VFSlotRuntimeState), TASK_FENE));
    HLib::AssignDynamicAlignmentForTaskData(slot);
    std::memset(slot, 0, sizeof(*slot));
    return slot;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void FightAreaMaskNullsub(std::uint8_t count) {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors) {
    *nearbyActors = 0;
    float offset = 0;
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        const FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (!slot.character || slot.state != 3 || (slot.collisionFlags & 1U) == 0)
            continue;
        const float dx = position->x - slot.position.x, dz = position->z - slot.position.z;
        if (std::sqrt(dx * dx + dz * dz) < 0.7f) {
            offset += 0.15f;
            if (nearbyActors)
                *nearbyActors |= 1U << index;
        }
    }
    return offset;
}

#else

// @verified - p
float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors) {
    *nearbyActors = 0;
    float offset = 0.0f;
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        const FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || (group.collisionFlags & 1U) == 0)
            continue;
        const auto yaw = static_cast<std::uint16_t>(GetAllAngYForNode(&group.character->entity.node));
        const float x = sin_fixed16(static_cast<std::int16_t>(yaw)) * 0.40000001f;
        const float z = cos_fixed16(yaw) * 0.40000001f;
        const float plusX = position->x - (group.position.x + x), plusZ = position->z - (group.position.z + z);
        const float minusX = position->x - (group.position.x - x), minusZ = position->z - (group.position.z - z);
        if (plusX * plusX + plusZ * plusZ < 0.25f || minusX * minusX + minusZ * minusZ < 0.25f) {
            offset += 0.15000001f;
            *nearbyActors |= 1U << i;
        }
    }
    return offset;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfAssignWaveSpawnFromGenerator(FRMG_t *formation, float x, float z) {
    std::uint8_t primary = 99, outward = 99;
    float primaryDistance = 999.0f, outwardDistance = 999.0f;
    const float centerX = x - g_pFightStage->battleCenter.x;
    const float centerZ = z - g_pFightStage->battleCenter.z;
    const float distanceFromCenter = std::sqrt(centerZ * centerZ + centerX * centerX);
    for (std::uint8_t i = 0; i < g_FGEN_count; ++i) {
        const FGEN_t &generator = g_pFightStage->generators[i];
        if (generator.enabled != 1 || generator.other != 0)
            continue;
        const float dx = x - generator.position.x, dz = z - generator.position.z;
        const float distance = std::sqrt(dz * dz + dx * dx);
        if (distance <= 8.0f)
            continue;
        const float gx = generator.position.x - g_pFightStage->battleCenter.x;
        const float gz = generator.position.z - g_pFightStage->battleCenter.z;
        const bool isOutward = g_pFightStage->useCenterBias != 0 && std::sqrt(gx * gx + gz * gz) > distanceFromCenter;
        float &bestDistance = isOutward ? outwardDistance : primaryDistance;
        const float error = std::fabs(distance - 8.0f);
        if (bestDistance <= error)
            continue;
        bool available = true;
        for (std::uint8_t enemy = 0; enemy < g_pFightStage->enemyCount; ++enemy) {
            Character *const character = g_pFightStage->enemySlots[enemy].character;
            if (!character)
                continue;
            const Vector3 position = GetCharacterWorldPosition(character);
            const float ex = generator.position.x - position.x, ez = generator.position.z - position.z;
            if (ex * ex + ez * ez < 4.0f) {
                available = false;
                break;
            }
        }
        if (!available)
            continue;
        bestDistance = error;
        (isOutward ? outward : primary) = i;
    }
    const std::uint8_t selected = primary != 99 ? primary : outward;
    if (selected != 99)
        formation->spawnPosition = g_pFightStage->generators[selected].position;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
bool vfTransitionEnemySlotToInactive(VFMA_t *game, FCMG_t *group) {
    const auto team = group->counterGroup;
    switch (group->state) {
    case 0:
    case 4:
        return false;
    case 1:
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2:
        vfRemoveCharaFromGroup(game, group);
        --g_pFightStage->activeFightersByTeam[team];
        break;
    case 3:
        --g_pFightStage->groupCounters[team];
        break;
    default:
        break;
    }
    vfRestoreEnemySlotMotion(group);
    mtwkSetExtraFlag(group->motion, 0, 0);
    mtwkSetExtraFlag(group->motion, 3, 2);
    group->state = 4;
    group->runningTicks = 0.0f;
    ++g_pFightStage->groupCounters[team + 2];
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfClearEnemySlotStackRefs(std::uint8_t slotIndex) {
    const std::uint32_t mask = 1U << slotIndex;
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || (group.stackReferenceMask & mask) == 0)
            continue;
        for (std::uint8_t layer = 0; layer < 7; ++layer) {
            if ((group.stackLayerMasks[layer] & mask) == 0)
                continue;
            group.stackLayerMasks[layer] &= ~mask;
            group.stackLayerOffsets[layer] -= 0.15000001f;
            if (layer == 2) {
                group.position.y -= 0.15000001f;
                shenApplyTranslationToAll(&group.character->entity.node, group.position);
            }
        }
        group.stackReferenceMask &= ~mask;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateEnemySlotCooldowns(VFMA_t *game) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3)
            continue;
        if (group.cooldownDelay != 0) {
            --group.cooldownDelay;
            continue;
        }
        if ((group.collisionFlags & 0x100U) != 0) {
            modelSetCharaAlpha(group.character, 1.0f);
            cpamSetFlag(group.character, 1);
            vfEnemySlotResetAndRestartCycle(game, &group);
        } else {
            cpamSetFlag(group.character, 1);
            vfTransitionEnemySlotToInactive(game, &group);
        }
        vfClearEnemySlotStackRefs(i);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Only the first eligible cooldown slot is faded per call.
// @verified - p
void vfUpdateRenderOpacityForAll(VFMA_t *game) {
    if (g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1] != game->maximumActiveEnemies)
        return;
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || group.cooldownDelay != 0)
            continue;
        cpamSetFlag(group.character, 0);
        if (g_pFightStage->secondaryGroupCounters[group.counterGroup] != 0 && (group.collisionFlags & 0x100U) != 0) {
            group.fadeAlpha -= 0.029999999f;
            if (group.fadeAlpha > 0.0f)
                modelSetCharaAlpha(group.character, group.fadeAlpha);
            else {
                group.fadeAlpha = 0.0f;
                modelSetCharaAlpha(group.character, 0.0f);
                vfUpdateEnemySlotCooldowns(game);
            }
        }
        return;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfTransitionAllEnemySlotsToInactive(VFMA_t *game) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state == 4)
            continue;
        figpAssignModeForCharacter(group.character, 0);
        modelSetCharaAlpha(group.character, 1.0f);
        if (group.state != 0)
            vfTransitionEnemySlotToInactive(game, &group);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// The spawn relocation helper vfRelocateWaveSpawnNearPlayer is
// retail-unreachable: its required predicate vfUpdatePredicate_00 returns zero, not one.
// @verified - p
void vfUpdateEnemySlotSpawnFade(VFMA_t *) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || !group.group || group.state != 1)
            continue;
        group.position = GetCharacterWorldPosition(group.character);
        if (group.fadeAlpha < 1.0f) {
            float alpha = group.fadeAlpha + 0.029999999f;
            if (alpha > 1.0f) {
                cpamSetFlag(group.character, 1);
                alpha = 1.0f;
            }
            group.fadeAlpha = alpha;
            modelSetCharaAlpha(group.character, std::fmax(0.0f, alpha));
        }
        group.accumulatedYaw = static_cast<std::uint16_t>(GetAllAngYForNode(&group.character->entity.node));
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdatePairingsAndDistances(VFMA_t *game) {
    float distances[6][6];
    std::int16_t angles[6][6];
    std::int8_t targets[6]{-1, -1, -1, -1, -1, -1};
    for (auto &row : distances)
        for (float &value : row)
            value = 9999.0f;
    const std::int8_t player = game->playerSlot;
    std::int8_t opponent = -1;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        const auto team = game->slotTeams[i];
        // Native FRMG+0x24 is the formation wave flag word.
        if (team == 1 && (game->groups[i]->selectFlags & 0xa0U) == 0)
            opponent = static_cast<std::int8_t>(i);
        for (std::uint8_t j = 0; j < 6; ++j) {
            if (j == i || !game->characters[j] || game->slotTeams[j] == team)
                continue;
            VFActor *const other = game->actors[j];
            const float x = self->position.x - other->position.x;
            const float z = self->position.z - other->position.z;
            const float distance = std::sqrt(x * x + z * z);
            distances[i][j] = distances[j][i] = distance;
            angles[i][j] = static_cast<std::int16_t>(self->temporaryFacingAngle - FixedAtan2(x, z));
            angles[j][i] = static_cast<std::int16_t>(other->temporaryFacingAngle - FixedAtan2(-x, -z));
        }
    }
    game->opponentSlot = static_cast<std::uint8_t>(opponent);
    // The native second pass retains the last processed actor's selected index into pass three.
    std::int8_t lastSelected = -1;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        bool foundFacing = false;
        float nearest = 9999.0f;
        lastSelected = -1;
        const auto yaw = static_cast<std::int16_t>(i == player ? game->otherFacingYaw - self->facingYaw : 0);
        for (std::uint8_t j = 0; j < 6; ++j) {
            if (!game->characters[j] || game->slotTeams[j] == game->slotTeams[i])
                continue;
            float distance = distances[i][j];
            if ((game->actors[j]->moveFlags & 0x40000U) != 0)
                distance = 999.0f;
            const bool facing = static_cast<std::uint16_t>(yaw + angles[i][j] + 9100) <= 0x4718U && distance < 3.0f;
            if (facing ? (!foundFacing || nearest > distance) : (!foundFacing && nearest > distance)) {
                if (facing)
                    foundFacing = true;
                nearest = distance;
                lastSelected = static_cast<std::int8_t>(j);
            }
        }
        if (lastSelected != -1 &&
            (self->pairedSlot == -1 ||
                ((self->pairedSlot != lastSelected || self->pairedCharacter != game->characters[lastSelected]) && self->deferredReaction == 0 &&
                    ((self->motionFlags | self->hitStateFlags) & 0xc0000U) == 0 && ((self->secondaryMotionFlags | self->activeMotionFlags) & 0x80c00U) == 0))) {
            self->pairedSlot = lastSelected;
            self->pairedActor = game->actors[lastSelected];
            Character *const previous = self->pairedCharacter;
            self->pairedCharacter = game->characters[lastSelected];
            if (previous != self->pairedCharacter)
                self->reactionFlags = 0;
        }
        targets[i] = self->pairedSlot;
    }
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        if (self->targetingCharacter && self->targetingCharacter == game->characters[self->actionIndex]) {
            const auto otherFlags = self->actor->moveFlags;
            if (((otherFlags & 0x40000U) == 0 && (self->motionFlags & VFMOF_Unknown100) != 0 && (self->actor->hitStateFlags & VFHSF_GuardState) == 0 &&
                    (self->moveFlags & VFMF_Airborne) == 0) ||
                ((self->motionFlags & VFMOF_DelayGuardResolution) != 0 && (otherFlags & 0x20U) == 0)) {
                targets[i] = self->actionIndex;
                continue;
            }
        }
        if ((self->activeMotionFlags & VFSMF_Unknown2000000) != 0 && self->targetingCharacter && self->actor &&
            (self->actor->moveFlags & VFMF_ForceDefeat) == 0) {
            targets[i] = self->actionIndex;
            continue;
        }
        if ((g_pFightStage->battleFlags & 2U) != 0 || !game->characters[player] || i == player)
            continue;
        if (i == opponent) {
            targets[i] = player;
            continue;
        }
        std::uint8_t mode = 2;
        if (game->slotTeams[i] == 1) {
            if (opponent == -1)
                mode = 0;
            else {
                if (lastSelected != player || distances[player][opponent] >= 4.0f)
                    continue;
                mode = 1;
            }
        }
        std::int8_t nearestEnemy = -1, nearestAlly = -1, playerEnemy = -1;
        float enemyDistance = 9999.0f, allyDistance = 9999.0f, playerDistance = 9999.0f;
        const auto pairedPlayerSlot = static_cast<std::uint8_t>(game->actors[player]->pairedSlot);
        for (std::uint8_t j = 0; j < 6; ++j) {
            if (!game->characters[j] || j == player)
                continue;
            VFActor *const other = game->actors[j];
            if ((other->moveFlags & VFMF_ForceDefeat) != 0)
                continue;
            const float distance = distances[i][j];
            const float fromPlayer = distances[player][j];
            if (game->slotTeams[j] == 1 && playerDistance > fromPlayer && (other->motionFlags & VFMOF_Unknown80000) == 0 &&
                (other->secondaryMotionFlags & VFSMF_Unknown800) == 0) {
                playerEnemy = static_cast<std::int8_t>(j);
                playerDistance = fromPlayer;
            }
            if (game->slotTeams[j] == 0 && allyDistance > distance) {
                nearestAlly = static_cast<std::int8_t>(j);
                allyDistance = distance;
            }
            if (game->slotTeams[j] == 1 && j != pairedPlayerSlot && enemyDistance > distance) {
                nearestEnemy = static_cast<std::int8_t>(j);
                enemyDistance = distance;
            }
        }
        if (mode == 0) {
            if (playerEnemy != -1 && nearestAlly != -1)
                targets[i] = i == playerEnemy && distances[i][nearestAlly] > 1.5f ? player : nearestAlly;
        } else if (mode == 1) {
            if (nearestAlly != -1)
                targets[i] = nearestAlly;
        } else if (nearestEnemy != -1 && distances[i][targets[i]] > 1.5f) {
            targets[i] = nearestEnemy;
        }
    }
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i] || targets[i] == -1)
            continue;
        const std::int8_t target = targets[i];
        VFActor *const self = game->actors[i];
        Character *const character = game->characters[target];
        if (character && (self->actionIndex != target || self->targetingCharacter != character)) {
            Character *const previous = self->targetingCharacter;
            self->actionIndex = target;
            self->actor = game->actors[target];
            self->targetingCharacter = character;
            if (previous != character)
                self->targetCache = -1;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
VFMA_t *vfmaGetTaskParam() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    return task ? static_cast<VFMA_t *>(HLib::GetTaskParameter(task)) : nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Native: SNAP suppresses the fight gauge update (VFMA +0x574) while it captures.
// @verified - p
VFMA_t *vfmaSetGaugeSuppressed(std::int32_t suppressed) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (game)
        game->parameter574 = suppressed;
    return game;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t GetFightPlayerGuardEffectTimerSeconds() {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0xffffffffU;
    const std::int32_t frames = _mm_cvttss_si32(_mm_set_ss(game->guardTimer));
    return frames == -1 ? 0xffffffffU : static_cast<std::uint32_t>(frames / 30);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t GetFightActorFlags(Character *character) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    for (unsigned index = 0; index < 6; ++index) {
        if (game->characters[index] == character)
            return character ? game->actors[index]->formationRuntimeFlags : 0;
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Invalid EAX is zero-extended, not a signed 64-bit -1.
std::uint64_t GetFightCounterHighWord() {
    const VFMA_t *const game = vfmaGetTaskParam();
    return game ? game->counter32c >> 16 : 0xffffffffU;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// The native counter is the WORD at VCDS +0x42.
std::uint64_t GetBattleDialogueCompletionCount(std::uint32_t id) {
    const VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return 0xffffffffU;
    for (const VCDS_t *record = game->cameraDialogue->records; record; record = record->next)
        if (record->id == id)
            return id ? record->completions : 0xffffffffU;
    return 0xffffffffU;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void RemoveBattleDialogue(std::uint32_t id) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return;
    VCDW_t *const owner = game->cameraDialogue;
    VCDS_t *record = owner->records;
    while (record && record->id != id)
        record = record->next;
    if (!record || !id)
        return;
    if (record->previous)
        record->previous->next = record->next;
    else
        owner->records = record->next;
    if (record->next)
        record->next->previous = record->previous;
    HLib::FreeHeapSpace(record, static_cast<std::int32_t>(TASK_VCDS.name));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t GetFightStageObjectFlags() {
    const VFMA_t *const game = vfmaGetTaskParam();
    return game ? game->reserved2c : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void SetFightCounterHighWord(std::uint16_t value) {
    if (VFMA_t *const game = vfmaGetTaskParam())
        game->counter32c = (game->counter32c & 0xffffU) | static_cast<std::uint32_t>(value) << 16;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Enabling also publishes the mode-0x200 request.
std::uint32_t SetFightFlag400000(std::int32_t enabled) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    const std::uint32_t previous = game->gameFlags & 0x400000U;
    if (enabled) {
        game->mode |= 0x200U;
        game->gameFlags |= 0x400000U;
    } else
        game->gameFlags &= ~0x400000U;
    return previous;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor) {
    Vector3 position, volume, variance, velocity;
    DateTime date;
    volume.x = 0.1f;
    volume.y = 0;
    volume.z = 0.1f;
    velocity.x = 0;
    velocity.y = 0.00050000002f;
    velocity.z = 0;
    const FightSmokeProfileNative *const parameters = &fightSmokeProfiles[profile];
    if (g_pFightStage->smokeInitTask || GetWeatherPressure() > 0.1f)
        return;
    FightSmokeSlot *slot = g_pFightStage->smoke->slots;
    FBSM_t *const smoke = g_pFightStage->smoke;
    std::int32_t index = -1;
    std::uint8_t i = 0;
    if (slot->effect == -1)
        index = 0;
    else {
    next:
        ++slot;
        if (++i > 8)
            goto searched;
        if (slot->effect != -1)
            goto next;
        index = i;
    }
searched:
    if (index == -1)
        return;
    slot->effect = static_cast<std::int16_t>(smoke->effects[index]);
    slot->remainingTicks = parameters->ticks;
    slot->birthRate = parameters->birthRate;
    ++smoke->activeCount;
    FightActorRuntimePrefix *fighter;
    Character *character;
    if (profile == 5) {
        fighter = 0;
        character = reinterpret_cast<Character *>(actor);
    } else {
        fighter = actor;
        character = actor->character;
    }
    GetCharacterWorldPosition(character, &position);
    PrepareFightFieldNode(GetCharacterNode(character));
    switch (profile) {
    case 0:
        velocity.x = *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(fighter) + 0x264) * 0.03f;
        velocity.z = *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(fighter) + 0x268) * 0.03f;
    case 8:
        position.x = reinterpret_cast<float *>(fighter->retainedf8)[12];
        position.z = reinterpret_cast<float *>(fighter->retainedf8)[14];
        break;
    case 1:
        velocity.x = *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(fighter) + 0x264) * 0.03f;
        velocity.z = *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(fighter) + 0x268) * 0.03f;
    case 9:
        position.x = reinterpret_cast<float *>(fighter->retainedf8)[15];
        position.z = reinterpret_cast<float *>(fighter->retainedf8)[17];
        break;
    case 3:
    case 4:
        position.x = fighter->contactPosition.x;
        position.z = fighter->contactPosition.z;
        break;
    case 6:
        position.x = reinterpret_cast<float *>(fighter->retainedf8)[6];
        position.z = reinterpret_cast<float *>(fighter->retainedf8)[8];
        break;
    case 7:
        position.x = reinterpret_cast<float *>(fighter->retainedf8)[9];
        position.z = reinterpret_cast<float *>(fighter->retainedf8)[11];
        break;
    }
    std::int32_t timeIndex = 4, materialIndex;
    if (profile <= 5) {
        shenGetCurrentDateTime(date);
        const std::uint8_t hour = date.hour;
        if (hour < 7)
            timeIndex = 3;
        else if (hour < 11)
            timeIndex = 0;
        else if (hour < 15)
            timeIndex = 1;
        else if (hour < 19)
            timeIndex = 2;
        else
            timeIndex = 3;
        switch (GetFieldSoundProperties(&position) & 0xffffU) {
        case 9:
        case 10:
        case 13:
        case 14:
        case 15:
        case 18:
            materialIndex = 1;
            break;
        case 11:
        case 12:
        case 16:
        case 17:
            materialIndex = 2;
            break;
        default:
            materialIndex = 2;
            break;
        }
    } else
        materialIndex = 0;
    const float *const colour = fightSmokeColours[fightSmokePaletteIndices[materialIndex + timeIndex * 3]];
    switch (profile) {
    case 6:
        position.y = reinterpret_cast<float *>(fighter->retainedf8)[7];
        break;
    case 7:
        position.y = reinterpret_cast<float *>(fighter->retainedf8)[10];
        break;
    case 8:
        position.y = reinterpret_cast<float *>(fighter->retainedf8)[13];
        break;
    case 9:
        position.y = reinterpret_cast<float *>(fighter->retainedf8)[16];
        break;
    default:
        position.y = vfFieldGetIntersection(fighter, position) + 0.050000001f;
        break;
    }
    variance.x = parameters->horizontalVariance;
    variance.y = 0.00050000002f;
    variance.z = variance.x;
    ConfigureSmokeEffect(4, slot->effect, &position, &volume, parameters->particleCount, parameters->birthRate, 0);
    if (static_cast<std::uint8_t>(profile - 6) < 4) {
        SetSmokeEffectSpread(slot->effect, 4.0f, 3.0f);
        SetSmokeEffectRadialMotion(slot->effect, 0, 4.0f);
        SetParticleEffectUniformSize(0, slot->effect, 2.0f);
    } else {
        SetSmokeEffectSpread(slot->effect, 2.0f, 2.0f);
        SetSmokeEffectRadialMotion(slot->effect, 0, 3.0f);
        SetParticleEffectUniformSize(0, slot->effect, 2.0f);
    }
    SetParticleEffectColour(
        0, slot->effect, 0, static_cast<std::int32_t>(colour[1]), static_cast<std::int32_t>(colour[2]), static_cast<std::int32_t>(colour[3]));
    SetParticleEffectVelocity(0, slot->effect, &velocity, &variance);
    ResumeParticleEffect(0, slot->effect);
}

#else

// @verified - p
void EmitFightSmoke(std::uint8_t profile, VFActor *actor) {
    const FightSmokeProfile &parameters = fightSmokeProfiles[profile];
    if (g_pFightStage->smokeInitTask || static_cast<double>(GetWeatherPressure()) > 0.1)
        return;
    FBSM_t &smoke = *g_pFightStage->smoke;
    std::uint8_t index = 0;
    while (smoke.slots[index].effect != 0xffffU || smoke.effects[index] == invalidEffect) {
        if (++index >= fightSmokeSlotCount)
            return;
    }
    FightSmokeSlot &slot = smoke.slots[index];
    slot.effect = static_cast<std::uint16_t>(smoke.effects[index]);
    slot.remainingTicks = static_cast<float>(parameters.ticks);
    slot.birthRate = parameters.birthRate;
    ++smoke.activeCount;
    VFActor *const fighter = profile == 5 ? nullptr : actor;
    Character *const character = profile == 5 ? reinterpret_cast<Character *>(actor) : actor->character;
    Vector3 position = GetCharacterWorldPosition(character);
    Vector3 velocity{0.0f, 0.00050000002f, 0.0f};
    switch (profile) {
    case 0:
    case 1:
        velocity.x = fighter->knockbackStrength * 0.029999999f;
        velocity.z = fighter->animationScale * 0.029999999f;
        position.x = fighter->contactSamples[profile == 0 ? 4 : 5].x;
        position.z = fighter->contactSamples[profile == 0 ? 4 : 5].z;
        break;
    case 3:
    case 4:
        position.x = fighter->contactPosition.x;
        position.z = fighter->contactPosition.z;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        position.x = fighter->contactSamples[profile - 4].x;
        position.z = fighter->contactSamples[profile - 4].z;
        break;
    default:
        break;
    }
    std::size_t paletteIndex;
    if (profile >= 6)
        paletteIndex = 12;
    else {
        DateTime time;
        shenGetCurrentDateTime(time);
        const std::uint8_t hour = static_cast<std::uint8_t>(time.hour);
        const std::size_t timeIndex = hour < 7 || hour >= 19 ? 9 : hour < 11 ? 0 : hour < 15 ? 3 : 6;
        const std::uint16_t material = static_cast<std::uint16_t>(QueryGroundMaterialToken(position, position.y));
        paletteIndex = timeIndex + (material <= 18 && (321024U & (1U << material)) != 0 ? 1 : 2);
    }
    const std::uint8_t *const colour = fightSmokeColours[fightSmokePaletteIndices[paletteIndex]];
    position.y = profile >= 6 ? fighter->contactSamples[profile - 4].y : vfFieldGetIntersection(fighter, position) + 0.050000001f;
    ConfigureSmokeEffect(4, slot.effect, position, Vector3{0.1f, 0.0f, 0.1f}, parameters.particleCount, parameters.birthRate, shen_name{});
    SetSmokeEffectSpread(slot.effect, profile >= 6 ? 4.0f : 2.0f, profile >= 6 ? 3.0f : 2.0f);
    SetSmokeEffectRadialMotion(slot.effect, 0.0f, profile >= 6 ? 4.0f : 3.0f);
    SetParticleEffectInitialScale(0, slot.effect, 2.0f);
    SetParticleEffectColour(0, slot.effect, 0, colour[0], colour[1], colour[2]);
    SetParticleEffectVelocity(0, slot.effect, velocity, Vector3{parameters.horizontalVariance, 0.00050000002f, parameters.horizontalVariance});
    ResumeParticleEffect(0, slot.effect);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int32_t GetBtlStatus(Character *character) {
    if (!character)
        return -1;
    for (std::uint8_t i = 0; i < 16; ++i)
        if (g_activeFightData[i].character == character)
            return g_activeFightData[i].status;
    return -1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void EnsureLoadedFightStyle(std::uint8_t style, std::uint8_t slot) {
    FightStyleSlot &record = g_pFightStage->styles[slot];
    if (record.file)
        return;
    PakGlobalData *package = static_cast<PakGlobalData *>(g_pFightStage->packData);
    if (!package)
        package = GetPakLoadTaskDataPointer();
    char filename[32];
    std::snprintf(filename, sizeof(filename), "EN_%s", fightStyleNames[style]);
    void *file = package ? FileLoader(package, filename, shen_name{(SHEN_NAME(BIN_) & 0x00ffffffU) | 0x20000000U}) : nullptr;
    if (!file) {
        char directory[32];
        std::snprintf(filename, sizeof(filename), "EN_%s.BIN", fightStyleNames[style]);
        std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_actNumber, g_currentMap.characters[0], g_currentMap.characters[1],
            g_currentMap.characters[2], g_currentMap.characters[3]);
        file = LoadLegacySceneFile(directory, filename, TASK_FIGH);
    }
    record.file = file;
    record.style = style;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint8_t PublishFightFormation(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style,
    std::uint16_t spawnWord0, std::uint16_t spawnWord1, float x, float y, float z) {
    const std::uint8_t enemyIndex = g_pFightStage->enemySlotCount;
    if (enemyIndex >= g_pFightStage->enemyCount)
        return 0;
    const std::uint8_t groupIndex = static_cast<std::uint8_t>(g_pFightStage->formationCount[0] + g_pFightStage->formationCount[1]);
    if (groupIndex >= g_pFightStage->groupCount)
        return 0;
    FRMG_t &group = g_pFightStage->groups[groupIndex];
    group.character = character;
    group.blocker = blocker;
    group.hp = hp;
    group.totalHp = hp;
    group.style = style;
    group.poolMode = poolMode;
    group.team = team;
    group.state = 0;
    group.waveFlags = 0;
    group.selectFlags = 0;
    group.formationFlags = 0;
    if (team == 1)
        g_pFightStage->accumulatedHp += hp;
    if (reinterpret_cast<std::uintptr_t>(character) == FIGHT_DEAD.name) {
        group.spawnPosition = {x, y, z};
        group.spawnWords[0] = spawnWord0;
        group.spawnWords[1] = spawnWord1;
        g_vfFormationGroupIndex = groupIndex;
        group.slot = nullptr;
    } else {
        FCMG_t *existing = nullptr;
        for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
            if (g_pFightStage->enemySlots[i].character == character) {
                existing = &g_pFightStage->enemySlots[i];
                break;
            }
        }
        if (existing)
            group.slot = existing;
        else {
            FCMG_t &slot = g_pFightStage->enemySlots[enemyIndex];
            group.slot = &slot;
            slot.character = character;
            slot.group = nullptr;
            slot.fighter = nullptr;
            slot.actorIndex = 0;
            slot.quadrant = -1;
            slot.state = 0;
            slot.runningTicks = 0.0f;
            slot.flags = 0;
            slot.counterGroup = team;
            slot.fadeAlpha = 1.0f;
            slot.reflectorIntensity = 0.0f;
            slot.savedPositionState = -1;
            slot.positionState = 0;
            slot.fightEntryIndex = enemyIndex;
            slot.reflectorColour = {};
            g_activeFightData[enemyIndex].hp = hp;
            g_activeFightData[enemyIndex].character = character;
            g_activeFightData[enemyIndex].status = 0;
            g_activeFightData[enemyIndex].initialHp = 100;
            ++g_pFightStage->enemySlotCount;
        }
    }
    EnsureLoadedFightStyle(style, enemyIndex);
    ++g_pFightStage->formationCount[team];
    ++g_pFightStage->secondaryGroupCounters[team];
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t PublishFightRespawn(std::uint16_t hp, std::uint8_t style, std::uint16_t spawnWord0, float x, float y, float z, std::uint16_t spawnWord1) {
    const std::uint8_t enemyIndex = g_pFightStage->enemySlotCount;
    if (enemyIndex >= g_pFightStage->enemyCount)
        return 0;
    const std::uint8_t groupIndex = static_cast<std::uint8_t>(g_pFightStage->formationCount[0] + g_pFightStage->formationCount[1]);
    if (groupIndex >= g_pFightStage->groupCount)
        return 0;
    FRMG_t &group = g_pFightStage->groups[groupIndex];
    g_vfFormationGroupIndex = groupIndex;
    group.blocker = nullptr;
    group.state = 0;
    group.selectFlags = 0;
    group.formationFlags = 0;
    group.waveFlags = 0;
    group.character = reinterpret_cast<Character *>(static_cast<std::uintptr_t>(FIGHT_DEAD.name));
    group.hp = hp;
    group.totalHp = hp;
    group.style = style;
    group.poolMode = 1;
    group.team = 1;
    g_pFightStage->accumulatedHp += hp;
    group.slot = nullptr;
    group.spawnWords[1] = spawnWord1;
    group.spawnPosition = {x, y, z};
    group.spawnWords[0] = spawnWord0;
    EnsureLoadedFightStyle(style, enemyIndex);
    ++g_pFightStage->formationCount[1];
    ++g_pFightStage->secondaryGroupCounters[1];
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Transfer, rather than share a second owning task-data reference.
// @verified - p
bool vfgGameInfoEntry(HLib::Task *source, HLib::Task *target) {
    if (!source || !target || target->taskData)
        return false;
    target->taskData = source->taskData;
    source->taskData = nullptr;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Each native producer pass covers all live actors before the next pass.
// @verified - p
void vfg_Callback_GameInfoEntry_callback(void *data) {
    auto *const game = static_cast<VFMA_t *>(data);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i])
            vfUpdateHitFacingAndKnockback(game->actors[i]);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i])
            vfUpdateContactSlotsAndGroundFlags(game->actors[i]);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i])
            vfSelectDefaultReactionAndGuardAnims(game->actors[i]);
    vfgGameInfoEntry(game->gameInfoTask, game->task);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
HLib::Task *vfg_Callback_GameInfoEntry_Enqueue(VFMA_t *game) {
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(vfg_Callback_GameInfoEntry_callback, 12, 10, SHEN_NAME(VFGI));
    HLib::AssignTaskDataToTask(task, game);
    return task;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Reset the shared sound slots, retaining the six live actor owners.
void vfInitSfxSlotState(VFMA_t *game) {
    auto *const bytes = reinterpret_cast<std::byte *>(game);
    std::memset(bytes + 0x294, 0, 8);
    std::memset(bytes + 0x29c, 0, 2);
    for (std::uint8_t i = 0; i < 6; ++i) {
        bytes[0x29e + i] = std::byte{};
        std::memset(bytes + 0x270 + 4 * i, 0, 4);
        bytes[0x2a7 + i] = std::byte{};
        if (game->characters[i])
            game->actors[i]->slot->opponentIndex = 0xff;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint8_t AccessFightActorFlag411(std::uint8_t mode, Character *character, std::uint8_t value) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return 0;
    VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    if (!game)
        return 0;
    std::int8_t index = 0;
    while (game->characters[index] != character) {
        if (++index >= 6) {
            index = 99;
            break;
        }
    }
    VFActor *const actor = game->actors[index];
    if (!mode)
        return static_cast<std::uint8_t>(actor->guardLevel);
    actor->guardLevel = static_cast<std::int8_t>(value);
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
float AccessFightActorGroupFloat(std::uint8_t mode, Character *character, float value) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return -1.0f;
    VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    if (!game)
        return -1.0f;
    std::int8_t index = 0;
    while (game->characters[index] != character) {
        if (++index >= 6) {
            index = 99;
            break;
        }
    }
    VFActor *const actor = game->actors[index];
    switch (mode) {
    case 0:
        return actor->focusedRegenerationScale;
    case 1:
        actor->focusedRegenerationScale = value;
        return 0.0f;
    case 2:
        return actor->guardMaximumHp;
    case 3:
    case 5:
        actor->guardMaximumHp = value;
        return 0.0f;
    case 4:
        return actor->regenerationRate;
    default:
        return 0.0f;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void SetFightActorParameterBytes(shen_name character, const std::uint8_t *values) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return;
    VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    VFActor *const actor = btlGetActorByScenarioName(game, character);
    std::byte *const parameters = const_cast<std::byte *>(actor->motionParameters[2]);
    for (std::uint8_t i = 0; i < 9; ++i)
        parameters[0x1a + i] = static_cast<std::byte>(values[i]);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void SuspendFight() {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return;
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->actors[i])
            mtwkRemoveCallback(game->actors[i]->motion);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (slot.character && slot.group && slot.state == 3)
            mtwkRemoveCallback(slot.motion);
    }
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (shenCheckCharaHasAlphaManager(slot.character))
            slot.savedControlFlags |= 0x80000000U;
        else
            slot.savedControlFlags &= ~0x80000000U;
        cpamSetFlag(slot.character, (slot.savedControlFlags & 0x80U) != 0);
    }
    if (game->inputBuffer)
        BindPadControlEntry(game->inputBuffer);
    if (game->savedInput)
        ReleasePadControlEntry(game->savedInput);
    if (!(g_pFightStage->battleFlags & 0x2U) && game->inputLockOnStart) {
        SetPlayerControlEnabled(1);
        game->inputLockOnStart = 0;
    }
    HLib::SetupTimedTask(game->task, -1);
    HLib::SetupTimedTask(game->collisionTask, -1);
    HLib::SetupTimedTask(game->gameInfoTask, -1);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int32_t ResumeFight(std::int32_t resetActors) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    HLib::ResolveDeferredCallback(game->task);
    HLib::ResolveDeferredCallback(game->collisionTask);
    HLib::ResolveDeferredCallback(game->gameInfoTask);
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->actors[i])
            mtwkRegisterGroundTargetCallbacks(game->actors[i]->motion, game->actors[i]);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (!slot.character || !slot.group || slot.state != 3)
            continue;
        for (std::uint32_t selector : fightMotionSelectors) {
            mtwkAssignTargetCallback(slot.motion, AdjustCooldownGroundTarget, selector);
            mtwkAssignTargetParameter(slot.motion, &slot, selector);
        }
    }
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        cpamSetFlag(slot.character, (slot.savedControlFlags & 0x80000000U) != 0);
        slot.savedControlFlags &= ~0x80000000U;
    }
    std::int32_t result = plymIsSearchStartRequested();
    game->inputLockOnStart = static_cast<std::uint8_t>(result);
    if (g_pFightStage->battleFlags & 0x2U) {
        game->inputBuffer = nullptr;
        game->savedInput = nullptr;
    } else {
        if (result)
            SetPlayerControlEnabled(0);
        game->savedInput = AllocatePadControlEntry();
        game->inputBuffer = BindPadControlEntry(game->savedInput);
        result = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(game->inputBuffer));
    }
    if (resetActors) {
        g_pFightStage->battleFlags |= 0x10U;
        result = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(g_pFightStage));
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            VFActor &actor = *game->actors[i];
            MotionWork *const motion = actor.motion;
            actor.position = GetCharacterWorldPosition(actor.character);
            actor.facingAngle = static_cast<std::uint16_t>(GetAllAngYForNode(&actor.character->entity.node));
            vfResetReactionProgram(&actor);
            actor.moveFlags &= ~(VFMF_HitReactionLock | VFMF_MirroredStance);
            actor.facingYaw = actor.facingAngle;
            actor.contactPosition = actor.position;
            actor.queuedAction = 0;
            actor.facingAngleDelta = 0;
            actor.savedMarkerMask = 0;
            actor.activeMotionFlags = 0;
            actor.hitStateFlags = 0;
            actor.secondaryMotionFlags = 0;
            actor.markerMask = 0;
            actor.motionFlags = 0;
            if (mtwkSetRegionFlag(motion, 70, 1, MotionRegion::LowerBody))
                actor.moveFlags |= VFMF_MirrorReactionAngle;
            else
                actor.moveFlags &= ~VFMF_MirrorReactionAngle;
            const float frame = mtwkGetFrame(motion, MotionRegion::LowerBody);
            if (actor.currentAnimation != mtwkGetAnimationId(motion, MotionRegion::LowerBody))
                vfSelectDefaultMotion(&actor, 0);
            result = static_cast<std::int32_t>(mtwkSetExtraFlag(motion, 1, 2));
            actor.stateFlags |= VFSF_Unknown80000;
            actor.animationTime = frame;
        }
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void AllocateVFGW(FightActorRuntimePrefix *actor) {
    FightGaugeWork *const gauge = static_cast<FightGaugeWork *>(HLib::AllocateTaskData(sizeof(FightGaugeWork), SHEN_NAME(VFGW)));
    actor->gauge = gauge;
    HLib::AssignDynamicAlignmentForTaskData(gauge);
    gauge->animationTime = 0;
    gauge->displayedHp = 100;
    gauge->targetHp = 100;
    gauge->gaugeTime = 90;
    gauge->state = 0;
    gauge->x = 80;
    gauge->width = 0;
    gauge->animation = 0;
    gauge->height = 8;
    gauge->sprite = 1;
    gauge->secondSprite = 0x81;
    gauge->colour = 0xff;
    gauge->secondColour = 0x89;
    gauge->phase = 0;
    gauge->secondaryState = 0;
    gauge->secondaryX = 80;
    gauge->secondaryHeight = 0;
    gauge->active = 0;
    gauge->secondaryWidth = 40;
    gauge->secondarySprite = 1;
    gauge->secondaryColour = 0xff;
    gauge->secondaryAnimation = 0;
}

#else

// The gauge pointer is stored at FGTM+0xb18.

// @verified - p
VFGW_t *AllocateVFGW(VFActor *actor) {
    auto *const gauge = static_cast<VFGW_t *>(HLib::AllocateTaskData(sizeof(VFGW_t), SHEN_NAME(VFGW)));
    actor->gauge = gauge;
    HLib::AssignDynamicAlignmentForTaskData(gauge);
    gauge->state = 0;
    gauge->animation = 0;
    gauge->sprite = 137;
    gauge->animationParameters = 0;
    gauge->active = 0;
    gauge->gaugeTime = 90.0f;
    gauge->animationTime = 0.0f;
    gauge->displayedHp = gauge->targetHp = 100.0f;
    gauge->x = 80;
    gauge->dimensions = 0x10008U;
    gauge->colours = 0xff0081U;
    gauge->layout = 0x280050U;
    gauge->scale = 0x10000U;
    gauge->alpha = 255;
    return gauge;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void FreeVFGW(VFActor *actor) {
    if (!actor->gauge)
        return;
    HLib::ClearTaskDataMark(actor->gauge);
    HLib::FreeTaskData(actor->gauge);
    actor->gauge = nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
VCDW_t *AllocateVCDW(VFMA_t *game) {
    auto *const dialogue = static_cast<VCDW_t *>(HLib::AllocateTaskData(sizeof(VCDW_t), SHEN_NAME(VCDW)));
    HLib::AssignDynamicAlignmentForTaskData(dialogue);
    game->cameraDialogue = dialogue;
    dialogue->selected = -1;
    dialogue->records = nullptr;
    dialogue->state = 0;
    dialogue->flags = 0;
    dialogue->enabled = 1;
    dialogue->counter = 0;
    dialogue->parameters = 0;
    return dialogue;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateWorldTransformAndFacingAngle(VFActor *actor) {
    actor->position = GetCharacterWorldPosition(actor->character);
    actor->facingAngle = static_cast<std::uint16_t>(GetAllAngYForNode(&actor->character->entity.node));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfApplyYawRecursive(VFActor *actor) {
    shenDecrementRotYAllNodes(&actor->character->entity.node, static_cast<std::int16_t>(actor->facingAngle));
    shenApplyTranslationToAll(&actor->character->entity.node, actor->position);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
bool BindAllocatedEnemySlot(FCMG_t *slot) {
    FRMG_t *formation = nullptr;
    for (std::uint8_t i = 0; i < g_pFightStage->groupCount; ++i) {
        FRMG_t &candidate = g_pFightStage->groups[i];
        if (candidate.character == slot->character && candidate.state == 0) {
            formation = &candidate;
            break;
        }
    }
    if (!formation)
        return false;
    slot->motion =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(slot->character);
    slot->position = GetCharacterWorldPosition(slot->character);
    slot->accumulatedYaw = static_cast<std::uint16_t>(GetAllAngYForNode(&slot->character->entity.node));
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        const FCMG_t &other = g_pFightStage->enemySlots[i];
        if (other.character && &other != slot && std::fabs((other.position.z - slot->position.z) + (other.position.x - slot->position.x)) < 0.00000011920929f) {
            slot->position.x += 0.0099999998f;
            slot->position.z += 0.0099999998f;
            shenApplyTranslationToAll(&slot->character->entity.node, slot->position);
        }
    }
    vfPrepareEnemySlotMotion(slot);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    slot->group = formation;
    formation->slot = slot;
    --g_pFightStage->secondaryGroupCounters[slot->counterGroup];
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfResetStageObjectSlot(VOBJ_t *object) {
    vfStageObjectReset(static_cast<std::uint8_t>(object->object.index), 0);
}

// @verified - d
void vfAttachActorStageObject(FightActorRuntimePrefix *actor) {
    const std::int8_t index = *reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x9a7);
    vfStageObjectReset(index, actor->character);
}

void vfStageObjectResetWrapper(std::int32_t slot, Character *character) {
    vfStageObjectReset(slot, character);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfReleaseActorStageObject(FightActorRuntimePrefix *actor) {
    const std::int8_t index = *reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x9a7);
    vfStageObjectReset(index, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfLaunchStageObject(std::int32_t index, std::int32_t angle) {
    VOBJ_t *const entry = g_fightStageObjects[index];
    vfStageObjectReset(index, 0);
    const float x = -CalculateFixedSine(angle) * (entry->object.definition->responseCoefficient * -0.0166f + 0.5f);
    vfStageObjectAssignVelocityXYZ(index, x, 0.2f, -CalculateFixedCosine(angle) * (entry->object.definition->responseCoefficient * -0.0166f + 0.5f));
    if (entry->object.definition->flags & 0x380U)
        entry->object.reactionFlags |= 2U;
}

#else

// Preserve the authored response coefficient at definition +144.
void vfLaunchStageObject(std::int32_t index, std::uint16_t angle) {
    FightStageObjectRuntime &object = g_fightStageObjects[index]->object;
    vfStageObjectReset(index, nullptr);
    const float coefficient = object.definition->responseCoefficient;
    const float magnitude = 0.5f - coefficient * 0.0166f;
    object.velocity = {-sin_fixed16(static_cast<std::int16_t>(angle)) * magnitude, 0.2f, -cos_fixed16(angle) * magnitude};
    if (object.definition->flags & 0x380U)
        object.reactionFlags |= 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfLaunchActorStageObject(FightActorRuntimePrefix *actor) {
    const std::int8_t index = *reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x9a7);
    vfLaunchStageObject(index, static_cast<std::int16_t>(actor->temporaryFacingAngle));
}

void vfTossActorStageObject(FightActorRuntimePrefix *actor) {
    const std::int8_t *const slot = reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x9a7);
    vfStageObjectReset(*slot, 0);
    const std::int8_t index = *slot;
    const std::int16_t *const direction = reinterpret_cast<std::int16_t *>(reinterpret_cast<std::byte *>(actor) + 0x2b2);
    vfStageObjectAssignVelocityXYZ(index, -CalculateFixedSine(*direction) * 0.2f, 0.1f, -CalculateFixedCosine(*direction) * 0.2f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfTransformStageObjectHitboxes(VOBJ_t *owner) {
    Vector3 position, world;
    if ((!owner->object.state && (owner->object.definition->flags & 2U)) || (owner->object.state && (owner->object.definition->flags & 4U))) {

        const FightStageObjectDefinition *const definition = owner->object.definition;
        for (std::int32_t index = 0; index < definition->hitboxCount; ++index) {
            switch (definition->hitboxes[index].kind) {
            case 1:
                matrix_stack::UnitCurrentMatrix();
                Matrix4x4::preTranslate(owner->object.contactPosition.x, owner->object.contactPosition.y, owner->object.contactPosition.z);
                Matrix4x4::SetViewMatrixFromEuler(owner->object.rotation.x, owner->object.rotation.y, owner->object.rotation.z);
                position.x = definition->hitboxes[index].position.x;
                position.y = definition->hitboxes[index].position.y;
                position.z = definition->hitboxes[index].position.z;
                Matrix4x4::TransformPoint(position, world);
                owner->object.hitboxes[index].position.x = world.x;
                owner->object.hitboxes[index].position.y = world.y;
                owner->object.hitboxes[index].position.z = world.z;
                break;
            }
        }
    }
}

std::int32_t IsActorBoneNearStageObject(FightActorRuntimePrefix *actor, VOBJ_t *owner) {
    const Vector3 &centre = actor->game->collision->samples[*reinterpret_cast<std::int8_t *>(actor)].points[0];
    Vector3 delta;
    delta.x = centre.x - owner->object.contactPosition.x;
    delta.y = centre.y - owner->object.contactPosition.y;
    delta.z = centre.z - owner->object.contactPosition.z;
    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z > g_fightStageObjectMaximumDistance)
        return 0;
    return 1;
}

std::int32_t AreStageObjectsNear(VOBJ_t *first, VOBJ_t *second) {
    Vector3 delta;
    delta.x = first->object.contactPosition.x - second->object.contactPosition.x;
    delta.y = first->object.contactPosition.y - second->object.contactPosition.y;
    delta.z = first->object.contactPosition.z - second->object.contactPosition.z;
    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z > g_fightStageObjectMaximumDistance)
        return 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfResetStageObjectRotationImpulse(VOBJ_t *object) {
    object->object.rotationImpulse.x = 0;
    object->object.rotationImpulse.y = 0;
    object->object.rotationImpulse.z = 0;
    object->object.reactionFlags &= ~0x60U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfInitStageObjects() {
    g_fightStageObjectDefinitions[0] = 0;
    g_fightStageObjectDefinitions[1] = 0;
    g_fightStageObjectDefinitions[2] = 0;
    g_fightStageObjectDefinitions[3] = 0;
    g_fightStageObjectDefinitions[4] = 0;
    g_fightStageObjectDefinitions[5] = 0;
    g_fightStageObjectDefinitions[6] = 0;
    g_fightStageObjectDefinitions[7] = 0;
    std::size_t size = sizeof(g_fightStageObjects);
    std::memset(g_fightStageObjects, 0, size);
    g_fightStageObjectRoot = 0;
    g_fightStageObjectMaximumDistance = 16.0f;
}

#else

// @verified - p
void vfInitStageObjects() {
    // Native resets four of eight definitions and fifteen of thirty object pointers.
    for (std::size_t i = 0; i < 4; ++i)
        g_fightStageObjectDefinitions[i] = nullptr;
    std::memset(g_fightStageObjects, 0, 0x78);
    g_fightStageObjectMaximumDistance = 16.0f;
    g_fightStageObjectRoot = nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfFreeStageObjects() {
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next) {
        if (object->object.state)
            continue;
        if (!(object->object.savedControlFlags & CCMF_ApplyGravity))
            ccmClearFlags(object->object.controlCharacter, CCMF_ApplyGravity);
        if (object->object.savedControlFlags & CCMF_EvaluateMotion)
            AssignCharacterControlFlags(object->object.controlCharacter, CCMF_EvaluateMotion);
        if (object->object.savedControlFlags & CCMF_VoxelSweep)
            AssignCharacterControlFlags(object->object.controlCharacter, CCMF_VoxelSweep);
        if (object->object.savedControlFlags & CCMF_UpdateHeight)
            AssignCharacterControlFlags(object->object.controlCharacter, CCMF_UpdateHeight);
    }
    for (std::int32_t index = 0; index < 8; ++index) {
        if (!g_fightStageObjectDefinitions[index])
            continue;
        HLib::FreeTaskData(g_fightStageObjectDefinitions[index]);
        g_fightStageObjectDefinitions[index] = 0;
    }
    for (std::int32_t index = 0; index < 30; ++index) {
        VOBJ_t *const object = g_fightStageObjects[index];
        if (!object)
            continue;
        if (object->motionTracks)
            HLib::FreeTaskData(object->motionTracks);
        HLib::FreeTaskData(g_fightStageObjects[index]);
        g_fightStageObjects[index] = 0;
    }
    g_fightStageObjectRoot = 0;
}

#else

// @verified - p
void vfFreeStageObjects() {
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next) {
        if (object->object.state != 0)
            continue;
        CharacterControlModel *const control = FindCharacterControlModel(object->object.controlCharacter);
        if (!control)
            continue;
        const std::uint32_t saved = object->object.savedControlFlags;
        if ((saved & 2U) == 0)
            control->flags &= ~2U;
        // FindCCM_OR_flags assigns, despite the old database name.
        if ((saved & 4U) != 0)
            control->flags = 4U;
        if ((saved & 8U) != 0)
            control->flags = 8U;
        if ((saved & 1U) != 0)
            control->flags = 1U;
    }
    for (FightStageObjectDefinition *&definition : g_fightStageObjectDefinitions) {
        if (!definition)
            continue;
        HLib::FreeTaskData(definition);
        definition = nullptr;
    }
    for (VOBJ_t *&object : g_fightStageObjects) {
        if (!object)
            continue;
        if (object->motionTracks)
            HLib::FreeTaskData(object->motionTracks);
        HLib::FreeTaskData(object);
        object = nullptr;
    }
    g_fightStageObjectRoot = nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void AllocateVOBJ(std::int32_t slot, std::int8_t index) {
    do {
        FightStageObjectDefinition *const definition =
            static_cast<FightStageObjectDefinition *>(HLib::AllocateTaskData(sizeof(FightStageObjectDefinition), TASK_VOBJ));
        if (!definition)
            return;
        std::memset(definition, 0, sizeof(*definition));
        g_fightStageObjectDefinitions[slot] = definition;
        definition->index = index;
    } while (0);
}

#else

// @verified - p
FightStageObjectDefinition **AllocateVOBJ(std::int32_t slot, std::int8_t index) {
    FightStageObjectDefinition *const definition =
        static_cast<FightStageObjectDefinition *>(HLib::AllocateTaskData(sizeof(FightStageObjectDefinition), TASK_VOBJ));
    if (!definition)
        return nullptr;
    std::memset(definition, 0, sizeof(*definition));
    g_fightStageObjectDefinitions[slot] = definition;
    definition->index = index;
    return g_fightStageObjectDefinitions;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageDefinitionSetMotion(std::int32_t slot, shen_name name, std::int32_t tracks, const ObjectMotionNode *nodes, std::int32_t step,
    std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.flags |= 1U;
    definition.motionTrackCount = tracks;
    if (!nodes)
        nodes = stageObjectDefaults.nodes;
    definition.motionNodes = nodes;
    definition.motionStep = step;
    definition.motionFirstFrame = firstFrame;
    definition.motionLastFrame = lastFrame;
    definition.breakSound = sound;
    definition.motionKeys =
        reinterpret_cast<const ObjectMotionRecord *>(reinterpret_cast<std::uintptr_t>(dynaRead(reinterpret_cast<const std::uint8_t *>(&name))) + 8U);
}

#else

// DYNA keys and node indices remain resource-owned.
const std::byte *vfStageDefinitionSetMotion(std::int32_t slot, std::uint32_t name, std::int32_t tracks, const std::uint32_t *nodes, std::int32_t step,
    std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.flags |= 1U;
    definition.motionTrackCount = tracks;
    definition.motionNodes = reinterpret_cast<const ObjectMotionNode *>(nodes ? nodes : stageObjectDefaults + 24);
    definition.motionStep = step;
    definition.motionFirstFrame = firstFrame;
    definition.motionLastFrame = lastFrame;
    definition.breakSound = sound;
    std::uint8_t resourceName[4];
    std::memcpy(resourceName, &name, sizeof(name));
    definition.motionKeys = reinterpret_cast<const ObjectMotionRecord *>(reinterpret_cast<std::uintptr_t>(dynaRead(resourceName)) + 8U);
    return reinterpret_cast<const std::byte *>(definition.motionKeys);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageDefinitionAddHitbox(std::int32_t slot, std::uint32_t flags, std::int32_t kindAndState, float x, float y, float z, float radius) {
    FightStageObjectHitbox record;
    const int kind = kindAndState & 0xf;
    record.kind = kind;
    if (kind == 1) {
        record.state = kindAndState & ~0xf;
        record.position.x = x;
        record.position.y = y;
        record.position.z = z;
        record.radius = radius;
    }
    vfStageDefinitionAppendHitbox(slot, flags, &record);
}

#else

// The native 28-byte stride includes eight unread stack bytes.
// Only kind 1 initializes a sphere; other kinds' payload and return register are unspecified.
void vfStageDefinitionAddHitbox(std::int32_t slot, std::uint32_t flags, std::uint16_t kindAndState, float x, float y, float z, float radius) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    const std::uint16_t kind = kindAndState & 0xfU;
    if (kind == 3) {
        definition.flags &= ~6U;
        definition.hitboxCount = 0;
        return;
    }
    const std::int32_t count = definition.hitboxCount;
    definition.flags |= flags;
    FightStageObjectHitbox &record = definition.hitboxes[count];
    record.kind = static_cast<std::int16_t>(kind);
    if (kind == 1) {
        record.state = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(kindAndState & 0xfff0U));
        record.position = {.x = x, .y = y, .z = z};
        record.radius = radius;
    }
    definition.hitboxCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(count) + 1U);
    if (kind == 1) {
        const float diameter = (radius + radius) + 1.0f;
        const float distance = diameter * diameter;
        if (distance > g_fightStageObjectMaximumDistance)
            g_fightStageObjectMaximumDistance = distance;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageDefinitionAppendHitbox(std::int32_t slot, std::uint32_t flags, const FightStageObjectHitbox *record) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    if (record->kind == 3) {
        definition.flags &= ~6U;
        definition.hitboxCount = 0;
        return;
    }
    definition.flags |= flags;
    std::memcpy(definition.hitboxes + definition.hitboxCount, record, sizeof(*record));
    definition.hitboxCount = static_cast<std::int32_t>(static_cast<std::uint32_t>(definition.hitboxCount) + 1U);
    if (record->kind == 1) {
        float distance = record->radius + record->radius + 1.0f;
        distance *= distance;
        if (distance > g_fightStageObjectMaximumDistance)
            g_fightStageObjectMaximumDistance = distance;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ToggleVOBJArrayIndex(std::int32_t slot, std::int8_t hitDamage, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound) {
    FightStageObjectDefinition *definition = g_fightStageObjectDefinitions[slot];
    definition->hitDamage = hitDamage;
    definition->guardState = guardState;
    definition->hitParameter = hitParameter;
    definition->hitSound = hitSound;
}

#else

// @verified - p
std::uint32_t ToggleVOBJArrayIndex(std::int32_t slot, std::int8_t hitDamage, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.hitDamage = hitDamage;
    definition.guardState = guardState;
    definition.hitParameter = hitParameter;
    definition.hitSound = hitSound;
    return hitSound;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound, std::uint32_t launchSound, std::uint32_t flags, std::int32_t launchYaw,
    std::int32_t rotationSpeed, float coefficient, float groundOffset) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.groundSound = groundSound;
    definition.launchSound = launchSound;
    definition.flags |= flags;
    definition.launchYaw = launchYaw;
    const float response = ((coefficient) * (-0.0166f) + (0.5f));
    definition.rotationSpeed = rotationSpeed;
    if (0.01f > response)
        definition.responseCoefficient = 29.51807403564453125f;
    else
        definition.responseCoefficient = coefficient;
    definition.groundOffset = groundOffset;
    const Vector3 *source =
        reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(&stageObjectDefaults) + 50 * sizeof(std::uint32_t)) + definition.index;
    definition.positionOffset.x = source->x;
    definition.positionOffset.y = source->y;
    definition.positionOffset.z = source->z;
}

#else

// The raw result is the authored offset Z's DWORD.
std::uint64_t vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound, std::uint32_t launchSound, std::uint32_t flags,
    std::int32_t launchYaw, std::int32_t rotationSpeed, float coefficient, float groundOffset) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.flags |= flags;
    definition.launchYaw = launchYaw;
    definition.groundSound = groundSound;
    definition.launchSound = launchSound;
    definition.rotationSpeed = rotationSpeed;
    // COMISS/JBE keeps NaN coefficients, rather than selecting the clamp.
    definition.responseCoefficient = 0.5f - coefficient * 0.0166f < 0.01f ? 29.51807403564453125f : coefficient;
    definition.groundOffset = groundOffset;
    std::memcpy(&definition.positionOffset, stageObjectDefaults + 3 * definition.index, sizeof(definition.positionOffset));
    std::uint32_t result;
    std::memcpy(&result, &definition.positionOffset.z, sizeof(result));
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::int32_t rotationZ) {
    Vector3 position;
    position.x = x;
    position.y = y;
    position.z = z;
    Vector3i rotation;
    rotation.x = rotationX;
    rotation.y = rotationY;
    rotation.z = rotationZ;
    vfStageDefinitionSetAttachmentData(slot, bone, &position, &rotation);
}

#else

std::uint64_t vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::uint32_t rotationZ) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.attachmentPosition = {.x = x, .y = y, .z = z};
    definition.attachmentBone = bone;
    definition.attachmentRotation = {.x = rotationX, .y = rotationY, .z = std::bit_cast<std::int32_t>(rotationZ)};
    return rotationZ;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageDefinitionSetAttachmentData(std::int32_t slot, std::uint8_t bone, const Vector3 *position, const Vector3i *rotation) {
    FightStageObjectDefinition &definition = *g_fightStageObjectDefinitions[slot];
    definition.attachmentBone = bone;
    std::memcpy(&definition.attachmentPosition, position, sizeof(*position));
    definition.attachmentRotation.x = rotation->x;
    definition.attachmentRotation.y = rotation->y;
    definition.attachmentRotation.z = rotation->z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageDefinitionAddContactMask(std::int32_t slot, std::uint32_t flags) {
    g_fightStageObjectDefinitions[slot]->flags |= flags;
}

#else

// @verified - p
std::int64_t vfStageDefinitionAddContactMask(std::int32_t slot, std::uint32_t flags) {
    g_fightStageObjectDefinitions[slot]->flags |= flags;
    return slot;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfAllocateVOBJ(std::int32_t slot, Character *character, std::int32_t definition) {
    VOBJ_t *const object = static_cast<VOBJ_t *>(HLib::AllocateTaskData(sizeof(VOBJ_t), TASK_VOBJ));
    do {
        if (!object)
            return;
        std::memset(object, 0, sizeof(*object));
    } while (0);
    g_fightStageObjects[slot] = object;
    object->next = g_fightStageObjectRoot;
    g_fightStageObjectRoot = object;
    object->object.index = static_cast<std::int8_t>(slot);
    if (ccmGetFlags(character) & CCMF_ApplyGravity)
        object->object.savedControlFlags |= CCMF_ApplyGravity;
    if (ccmGetFlags(character) & CCMF_EvaluateMotion)
        object->object.savedControlFlags |= CCMF_EvaluateMotion;
    if (ccmGetFlags(character) & CCMF_VoxelSweep)
        object->object.savedControlFlags |= CCMF_VoxelSweep;
    if (ccmGetFlags(character) & CCMF_UpdateHeight)
        object->object.savedControlFlags |= CCMF_UpdateHeight;
    AssignCharacterControlFlags(character, CCMF_ApplyGravity);
    ccmClearFlags(character, CCMF_UpdateHeight | CCMF_EvaluateMotion | CCMF_VoxelSweep);
    object->object.definition = g_fightStageObjectDefinitions[definition];
    object->object.model = character;
    object->object.controlCharacter = character;
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 1);
    GetCharacterWorldPosition(character, &object->object.contactPosition);
    GetCharacterWorldRotation(character, &object->object.rotation);
    object->object.position.x = object->object.contactPosition.x + object->object.definition->positionOffset.x;
    object->object.position.y = object->object.contactPosition.y + object->object.definition->positionOffset.y;
    object->object.position.z = object->object.contactPosition.z + object->object.definition->positionOffset.z;
}

#else

// @verified - p
FightStageObjectDefinition *vfAllocateVOBJ(std::int32_t slot, Character *character, std::int32_t definition) {
    VOBJ_t *const object = static_cast<VOBJ_t *>(HLib::AllocateTaskData(sizeof(VOBJ_t), TASK_VOBJ));
    if (!object)
        return nullptr;
    std::memset(object, 0, sizeof(*object));
    g_fightStageObjects[slot] = object;
    object->next = g_fightStageObjectRoot;
    g_fightStageObjectRoot = object;
    object->object.index = static_cast<std::int8_t>(slot);
    CharacterControlModel *const control = FindCharacterControlModel(character);
    if (control) {
        object->object.savedControlFlags = control->flags & 0xfU;
        control->flags = 2U;
        control->flags &= ~0xdU;
    }
    object->object.definition = g_fightStageObjectDefinitions[definition];
    object->object.model = character;
    object->object.controlCharacter = character;
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 1);
    object->object.contactPosition = GetCharacterWorldPosition(character);
    object->object.rotation = GetCharacterWorldRotation(character);
    const Vector3 &offset = object->object.definition->positionOffset;
    const Vector3 &position = object->object.contactPosition;
    object->object.position = {offset.x + position.x, offset.y + position.y, offset.z + position.z};
    return object->object.definition;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageObjectSetBrokenModel(std::int32_t slot, Character *character, std::uint32_t flags) {
    VOBJ_t *const object = g_fightStageObjects[slot];
    object->object.motionFlags |= flags;
    AssignCharacterControlFlags(character, CCMF_ApplyGravity);
    ccmClearFlags(character, CCMF_UpdateHeight | CCMF_EvaluateMotion | CCMF_VoxelSweep);
    object->object.brokenModel = character;
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
}

#else

// The hidden broken model is borrowed, not allocated here.
std::uint8_t vfStageObjectSetBrokenModel(std::int32_t slot, Character *character, std::uint32_t flags) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.motionFlags |= flags;
    if (CharacterControlModel *const control = FindCharacterControlModel(character))
        control->flags = 2U;
    if (CharacterControlModel *const control = FindCharacterControlModel(character))
        control->flags &= ~0xdU;
    object.brokenModel = character;
    return EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageObjectAssignPosXYZ(std::int32_t slot, float x, float y, float z) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.contactPosition.x = x;
    object.contactPosition.y = y;
    object.contactPosition.z = z;
    SetCharacterWorldPosition(object.model, &object.contactPosition);
}

#else

// @verified - p
void vfStageObjectAssignPosXYZ(std::int32_t slot, float x, float y, float z) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.contactPosition = {x, y, z};
    shenApplyTranslationToAll(GetCharacterNode(object.model), object.contactPosition);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageObjectAssignRotXYZ(std::int32_t slot, std::uint32_t x, std::uint32_t y, std::uint32_t z) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.rotation.x = x & 0xffffU;
    object.rotation.y = y & 0xffffU;
    object.rotation.z = z & 0xffffU;
    charaSetWorldRot(object.model, object.rotation);
}

#else

// @verified - p
void vfStageObjectAssignRotXYZ(std::int32_t slot, std::uint16_t x, std::uint16_t y, std::uint16_t z) {
    FightStageObjectRuntime &object = g_fightStageObjects[slot]->object;
    object.rotation = {x, y, z};
    charaSetWorldRot(object.model, object.rotation);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageObjectAssignMotFlags(std::int32_t slot, std::uint32_t flags) {
    g_fightStageObjects[slot]->object.motionFlags |= flags;
}

#else

// @verified - p
std::int32_t vfStageObjectAssignMotFlags(std::int32_t slot, std::uint32_t flags) {
    g_fightStageObjects[slot]->object.motionFlags |= flags;
    return slot;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfInitializeStageObjectHitboxes() {
    matrix_stack::Matrix4x4_Push(0);
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next)
        vfTransformStageObjectHitboxes(object);
    PopCurrentMatrix(1);
}

#else

// Initialize the real live root in insertion order.
void vfInitializeStageObjectHitboxes() {
    Matrix4x4_Push();
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next)
        vfTransformStageObjectHitboxes(object);
    Matrix4x4::Pop(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageObjectReset(std::int32_t slot, Character *character) {
    VOBJ_t *const entry = g_fightStageObjects[slot];
    FightStageObjectRuntime &object = entry->object;
    object.velocity.x = 0.0f;
    object.velocity.y = 0.0f;
    object.velocity.z = 0.0f;
    vfResetStageObjectRotationImpulse(entry);
    object.reactionFlags &= 0xfffffd84U;
    if (!character)
        return;
    if (object.definition->flags & 0x40U) {
        Vector3 position;
        GetCharacterWorldPosition(character, &position);
        const std::uint32_t yaw = dc::sdk::ArcTan2Fixed(position.z - object.contactPosition.z, position.x - object.contactPosition.x);
        if (!((0U - yaw - static_cast<std::uint32_t>(object.rotation.y)) & 0x8000U))
            object.reactionFlags |= 0x200U;
        std::memcpy(&object.reactionRotation.x, &object.rotation.x, sizeof(object.reactionRotation.x));
    }
    object.reactingCharacter = character;
    object.reactionFlags |= 1U;
}

#else

// The authored VOBJ owns the reset; attachment borrows its character.
void vfStageObjectReset(std::int32_t index, Character *character) {
    FightStageObjectRuntime &object = g_fightStageObjects[index]->object;
    object.velocity = {};
    object.rotationImpulse = {};
    object.reactionFlags &= 0xfffffd84U;
    if (!character)
        return;
    if (object.definition->flags & 0x40U) {
        const Vector3 position = GetCharacterWorldPosition(character);
        const std::int32_t angle =
            -(object.rotation.y + static_cast<std::int16_t>(FixedAtan2(position.z - object.contactPosition.z, position.x - object.contactPosition.x)));
        if (!(angle & 0x8000))
            object.reactionFlags |= 0x200;
        std::memcpy(&object.reactionRotation.x, &object.rotation.x, sizeof(object.rotation.x));
    }
    object.reactionFlags |= 1;
    object.reactingCharacter = character;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z) {
    g_fightStageObjects[slot]->object.velocity.x = x;
    g_fightStageObjects[slot]->object.velocity.y = y;
    g_fightStageObjects[slot]->object.velocity.z = z;
}

#else

// @verified - p
VOBJ_t *vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z) {
    VOBJ_t *const object = g_fightStageObjects[slot];
    object->object.velocity = {x, y, z};
    return object;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfToggleAttachedStageObjectDisplay(FightActorRuntimePrefix *actor) {
    for (VOBJ_t *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        if ((entry->object.reactionFlags & 1U) == 0 || entry->object.reactingCharacter != actor->character)
            continue;
        if (IsOfThisVisibilityType(entry->object.model, static_cast<CharaVisibilityMode>(3)) == 1)
            EV_SetCharDisplay(entry->object.model, static_cast<CharaVisibilityMode>(3), 0);
        else
            EV_SetCharDisplay(entry->object.model, static_cast<CharaVisibilityMode>(3), 1);
    }
}

#else

// Visit the live attachment list in native order.
void vfToggleAttachedStageObjectDisplay(VFActor *actor) {
    for (VOBJ_t *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        FightStageObjectRuntime &object = entry->object;
        if ((object.reactionFlags & 1) && object.reactingCharacter == actor->character)
            EV_SetCharDisplay(object.model, CVM_DisplayEnabled, !IsOfThisVisibilityType(object.model, 3));
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfSetAttachedStageObjectEffect(FightActorRuntimePrefix *actor, std::uint8_t effect, float amount, std::uint16_t frames) {
    for (VOBJ_t *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        if ((entry->object.reactionFlags & 1U) == 0 || entry->object.reactingCharacter != actor->character)
            continue;
        switch (effect) {
        case 1:
            entry->scaleTarget.x = amount;
            entry->scaleTicks[0] = frames;
            break;
        case 2:
            entry->scaleTarget.y = amount;
            entry->scaleTicks[1] = frames;
            break;
        case 3:
            entry->scaleTarget.z = amount;
            entry->scaleTicks[2] = frames;
            break;
        default:
            break;
        }
    }
}

#else

void vfSetAttachedStageObjectEffect(VFActor *actor, std::int8_t effect, float amount, std::int16_t frames) {
    if (effect < 1 || effect > 3)
        return;
    for (VOBJ_t *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        if (!(entry->object.reactionFlags & 1) || entry->object.reactingCharacter != actor->character)
            continue;
        entry->scaleTarget[effect - 1] = amount;
        entry->scaleTicks[effect - 1] = std::bit_cast<std::uint16_t>(frames);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfSetAttachedStageObjectFigurePose(FightActorRuntimePrefix *actor, std::uint16_t scale, std::uint16_t direction) {
    for (VOBJ_t *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        if ((entry->object.reactionFlags & 1U) != 0 && entry->object.reactingCharacter == actor->character) {
            if (direction) {
                figpAssignModeForCharacter(entry->object.model, 1);
                figpAssignMotScaleAndDirFor(entry->object.model, static_cast<std::uint8_t>(scale), static_cast<std::uint8_t>(direction));
            } else {
                figpAssignModeForCharacter(entry->object.model, 0);
            }
        }
    }
}

#else

void vfSetAttachedStageObjectFigurePose(VFActor *actor, std::int8_t scale, std::int16_t direction) {
    for (auto *entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        auto &object = entry->object;
        if (!(object.reactionFlags & 1) || object.reactingCharacter != actor->character)
            continue;
        figpAssignModeForCharacter(object.model, direction ? 1 : 0);
        if (direction)
            figpAssignMotScaleAndDirFor(object.model, scale, static_cast<std::int8_t>(direction));
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfStageObjectRestartMotion(std::int32_t index) {
    VOBJ_t *object = g_fightStageObjects[index];
    object->motionFrame = object->object.definition->motionFirstFrame;
    const std::uint32_t sound = object->object.definition->breakSound;
    if (sound)
        vfClAicaReact(sound, &object->object.contactPosition);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
FightActorRuntimePrefix *AllocateFGTM() {
    FightActorRuntimePrefix *const actor = static_cast<FightActorRuntimePrefix *>(HLib::AllocateTaskData(sizeof(FightActorRuntimePrefix), SHEN_NAME(FGTM)));
    HLib::AssignDynamicAlignmentForTaskData(actor);
    std::memset(actor, 0, sizeof(FightActorRuntimePrefix));
    return actor;
}

#else

// @verified - p
VFActor *AllocateFGTM() {
    VFActor *const actor = static_cast<VFActor *>(HLib::AllocateTaskData(sizeof(VFActor), TASK_FGTM));
    HLib::AssignDynamicAlignmentForTaskData(actor);
    std::memset(actor, 0, sizeof(*actor));
    return actor;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, FightActorRuntimePrefix *actor) {
    for (std::int32_t index = 0; index < 7; ++index) {
        mtwkAssignTargetCallback(motion, AdjustActiveGroundTarget, fightMotionSelectors[index]);
        mtwkAssignTargetParameter(motion, actor, fightMotionSelectors[index]);
    }
}

#else

// @verified - p
void mtwkRegisterGroundTargetCallbacks(MotionWork *motion, VFActor *actor) {
    for (const auto selector : fightMotionSelectors) {
        mtwkAssignTargetCallback(motion, AdjustActiveGroundTarget, selector);
        mtwkAssignTargetParameter(motion, actor, selector);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Publishes a real FGTM/FENE actor and its target
// callbacks only after the stage's ready/active/cooldown counters transition.
// @verified - p
std::uint8_t vfRegisterContactSlot(VFMA_t *game, FCMG_t *slot) {
    Character *const character = slot->character;
    std::uint8_t index = 0;
    while (game->characters[index]) {
        if (++index >= 6)
            return 99;
    }
    const auto team = slot->counterGroup;
    switch (slot->state) {
    case 0:
    case 4:
        if (!BindAllocatedEnemySlot(slot))
            return 99;
        break;
    case 1:
        if (slot->group && (slot->group->activationState & 0xffU) != 0)
            return 99;
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2:
        return 99;
    case 3:
        if (!ReactToDead(game, slot))
            return 99;
        --g_pFightStage->groupCounters[team];
        break;
    default:
        break;
    }
    FRMG_t *const formation = slot->group;
    game->characters[index] = character;
    game->groups[index] = formation;
    game->enemySlots[index] = slot;
    slot->runningTicks = 0.0f;
    slot->state = 2;
    formation->state = 1;
    g_activeFightData[slot->fightEntryIndex].status = 1;
    VFActor *const actor = slot->fighter;
    game->actors[index] = actor;
    slot->actorIndex = index;
    game->actorSlots[index] = actor->slot;
    actor->pairedCharacter = nullptr;
    actor->targetingCharacter = nullptr;
    actor->pairedActor = nullptr;
    actor->actor = nullptr;
    actor->groupIndex = index;
    actor->pairedSlot = -1;
    actor->actionIndex = -1;
    slot->position = GetCharacterWorldPosition(character);
    actor->contactPosition = actor->position = slot->position;
    actor->fightStyle = formation->style;
    actor->activeMotionFlags = 0;
    actor->hitStateFlags = 0;
    actor->moveFlags = 0;
    actor->motionFlags = actor->secondaryMotionFlags = 0;
    mtwkRegisterGroundTargetCallbacks(slot->motion, actor);
    game->slotTeams[index] = formation->team;
    game->slotPoolModes[index] = formation->poolMode;
    if (formation->poolMode == 0)
        game->playerSlot = static_cast<std::int8_t>(index);
    ++g_pFightStage->activeFightersByTeam[team];
    return index;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// The authored drag adjustment is 0.5 (g_fightKnockbackMaterialBlend).
// @verified - p
void vfApplyKnockbackPhysics(VFActor *self) {
    const Vector3 previous = self->position;
    const VFMA_t &game = *self->game;
    const float drag =
        (((self->knockbackBaseParameter - 60.0f) * 0.5f + 60.0f) * (game.dragScale * game.gravityScale) * 0.033333335f) * (self->contactTimer + 1.0f);
    const float knockbackDrag = self->scale * drag;
    const float slideDrag = self->scale * (drag * game.slideDragScale);
    bool movingKnockback = false;
    if ((self->hitStateFlags & VFHSF_FallLandingRequired) == 0) {
        const float length = std::sqrt(self->knockbackVelocity.x * self->knockbackVelocity.x + self->knockbackVelocity.z * self->knockbackVelocity.z);
        if (length <= knockbackDrag)
            self->knockbackVelocity.x = self->knockbackVelocity.z = 0.0f;
        else {
            movingKnockback = true;
            const float scale = 1.0f - knockbackDrag / length;
            self->knockbackVelocity.x *= scale;
            self->knockbackVelocity.z *= scale;
        }
    }
    const float slideLength = std::sqrt(self->slideVelocity.x * self->slideVelocity.x + self->slideVelocity.z * self->slideVelocity.z);
    if (slideLength <= slideDrag) {
        self->slideVelocity.x = self->slideVelocity.z = 0.0f;
        if (!movingKnockback)
            self->contactTimer = 0.0f;
    } else {
        const float scale = 1.0f - slideDrag / slideLength;
        self->slideVelocity.x *= scale;
        self->slideVelocity.z *= scale;
    }
    if ((self->moveFlags & VFMF_Airborne) != 0)
        self->knockbackVelocity.y = (self->knockbackVelocity.y - self->scale * self->scale * game.gravityScale) + self->slideVelocity.y;
    self->position.x = (self->position.x + self->knockbackVelocity.x) + self->slideVelocity.x;
    self->position.z = (self->position.z + self->knockbackVelocity.z) + self->slideVelocity.z;
    self->position.y += self->knockbackVelocity.y;
    self->frameMoveDelta = {self->position.x - previous.x, self->position.y - previous.y, self->position.z - previous.z};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdatePositionXZ(VFActor *self) {
    if (self->game->type != 4 || self->fightStyle != 0 || self->animationTime != 0.0f ||
        ((self->secondaryMotionFlags & VFSMF_Unknown8000000) == 0 && (self->motionFlags & (VFMOF_Unknown10 | VFMOF_Unknown1000)) == 0))
        return;
    const auto angle = static_cast<std::uint16_t>(-self->facingAngle);
    self->actor->position.x = sin_fixed16(static_cast<std::int16_t>(angle)) * 3.0f + self->position.x;
    self->actor->position.z = cos_fixed16(angle) * -3.0f + self->position.z;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfResolveKnockbackLanding(VFActor *self) {
    if ((self->moveFlags & VFMF_Airborne) == 0)
        return;
    if ((self->motionContactFlags & 1U) == 0) {
        const auto angle = static_cast<std::uint16_t>(-self->facingAngle);
        const float x = cos_fixed16(angle) * self->moveDelta.x - sin_fixed16(static_cast<std::int16_t>(angle)) * self->moveDelta.z;
        const float z = sin_fixed16(static_cast<std::int16_t>(angle)) * self->moveDelta.x + cos_fixed16(angle) * self->moveDelta.z;
        self->position.x -= x;
        self->position.z -= z;
    }
    self->moveDelta.y = self->contactPosition.y - self->position.y;
    self->moveFlags &= ~VFMF_Airborne;
    self->hitStateFlags &= ~VFHSF_FallLandingRequired;
    mtwkSetExtraFlag(self->motion, 3, 2);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateAutoAdvanceLanding(VFActor *self) {
    if ((self->game->gameFlags & 0x20U) != 0 || (self->moveFlags & VFMF_AutoAdvanceMotion) == 0)
        return;
    self->moveFlags &= ~VFMF_AnimationClamp;
    if ((self->moveFlags & VFMF_Airborne) != 0) {
        if ((self->moveFlags & VFMF_Ascending) == 0) {
            const float velocity = self->knockbackVelocity.y - self->scale * self->scale * self->game->gravityScale;
            if (self->targetGroundY + self->groundOffset > velocity + self->contactPosition.y || self->targetGroundY > velocity + self->position.y)
                self->moveFlags |= VFMF_AnimationClamp;
        }
    } else {
        if (self->targetGroundY != self->position.y && self->targetGroundY + 0.5f >= self->position.y)
            self->position.y = self->targetGroundY;
        if ((self->motionFlags & VFMOF_Unknown10) != 0) {
            if (self->animationTime < static_cast<float>(self->parameter2d6) || static_cast<float>(self->parameter2e0) <= self->animationTime)
                self->hitStateFlags &= ~VFHSF_FallLandingRequired;
            else
                self->hitStateFlags |= VFHSF_FallLandingRequired;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateAirborneAnimationTiming(VFActor *self) {
    if ((self->moveFlags & VFMF_AnimationClamp) != 0) {
        if ((self->moveFlags & VFMF_Airborne) != 0) {
            self->knockbackVelocity.y = 0.0f;
            self->position.y = self->targetGroundY;
            if (self->targetGroundY > self->minimumGroundY)
                self->knockbackVelocity.x = self->knockbackVelocity.z = 0.0f;
            vfResolveKnockbackLanding(self);
        }
        if (self->parameter2e0 != 0 && static_cast<float>(self->parameter2e0) > self->animationTime && (self->activeMotionFlags & VFSMF_Unknown1) == 0)
            self->animationTime = self->parameter4c8 = static_cast<float>(self->parameter2e0);
    } else if ((self->moveFlags & VFMF_Airborne) != 0) {
        if (self->knockbackVelocity.y < 0.0f)
            self->moveFlags &= ~VFMF_Ascending;
        if (self->animationTime > static_cast<float>(self->parameter2de)) {
            const std::int32_t frame = self->parameter2e0 - static_cast<std::int16_t>(self->predictedKnockbackFrames) + 1;
            if (frame <= 0 || static_cast<float>(frame) < self->animationTime)
                self->animationTime = self->parameter4c8 = self->animationTime - self->scale;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateGroundHeightAndEnemyContacts(VFActor *self) {
    std::uint32_t nearby = 0;
    const float ground = self->groundY + vfAccumEnemyGroundOffsetAndMask(&self->position, &nearby);
    if (ground == self->groundY) {
        if (self->targetGroundY > self->groundY) {
            self->targetGroundY = self->groundY;
            if ((self->moveFlags & VFMF_Airborne) == 0) {
                self->position.y = self->groundY;
                shenApplyTranslationToAll(&self->character->entity.node, self->position);
            }
        }
        return;
    }
    self->targetGroundY = ground;
    if ((self->moveFlags & VFMF_Airborne) != 0)
        return;
    self->position.y = ground;
    shenApplyTranslationToAll(&self->character->entity.node, self->position);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || group.state != 3 || (nearby & (1U << i)) == 0 || (group.collisionFlags & 8U) != 0 || (group.collisionFlags & 1U) == 0)
            continue;
        group.collisionFlags |= 8U;
        const float frame = mtwkGetFrame(group.motion, MotionRegion::LowerBody);
        const float end = mtwkGetEndFrame(group.motion, MotionRegion::LowerBody);
        const MotnSequence *const sequence = FindLoadedMotionSequence(mtwkGetAnimationId(group.motion, MotionRegion::LowerBody));
        if ((group.collisionFlags & 0x10U) != 0 || (frame != end && !(sequence && sequence->allowsFrameOverflow)))
            continue;
        const std::uint16_t animation = (group.collisionFlags & 0x20U) != 0 ? 455 :
            (group.collisionFlags & 0x40U) != 0                             ? 454 :
            (group.collisionFlags & 2U) != 0                                ? 108 :
                                                                              107;
        vfUpdateDefaultReactionAnim(&group, animation);
        mtwkSetRegionFlag(group.motion, 69, (group.collisionFlags & 0x80U) != 0 ? 0 : 2, MotionRegion::LowerBody);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

float FightIntersectionDistance(const VFActor &actor, std::uint16_t angle, float length) {
    const Vector3 &origin = (actor.moveFlags & VFMF_RefreshContactHistory) != 0 ? actor.contactPosition : actor.motionContactSamples[0];
    const Vector3 destination{origin.x - sin_fixed16(static_cast<std::int16_t>(angle)) * length, origin.y, origin.z - cos_fixed16(angle) * length};
    Vector3 intersection;
    RaycastForCollision(&intersection, &destination, &origin);
    const float x = origin.x - intersection.x, z = origin.z - intersection.z;
    return std::sqrt(x * x + z * z);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::uint16_t PredictFightKnockbackFrames(const VFActor &actor, float height) {
    if ((actor.moveFlags & VFMF_Airborne) == 0)
        return 0;
    float y = actor.contactPosition.y, velocity = actor.knockbackVelocity.y;
    const float acceleration = actor.scale * actor.scale * actor.game->gravityScale - actor.slideVelocity.y;
    std::uint16_t frames = 0;
    for (;;) {
        y += velocity;
        velocity -= acceleration;
        if (height > y && velocity < 0.0f)
            return frames;
        ++frames;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// Native mtwkGetPosFor's matrix-valued bone branch, used here for 0/26/31.
// The child-chain inverse is shenGetPosFor, not a world-position substitution.
Vector3 FightBoneLocalPosition(Character *character, std::int32_t bone) {
    Matrix4x4 *const matrix = mtwkGetBoneTransform(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(character), bone);
    if (!matrix)
        return GetCharacterWorldPosition(character);
    Vector3 position{matrix->rows[3].x, matrix->rows[3].y, matrix->rows[3].z};
    const CharacterNode *const child = character->entity.node.child;
    if (!child)
        return position;
    const Matrix4x4 saved = workTransformMatrix;
    Matrix4x4::SetPrePassToTempTransform();
    ApplyRecursiveCharacterNodeTransforms(child);
    const Matrix4x4 transform = workTransformMatrix;
    const float x = position.x - transform.rows[3].x, y = position.y - transform.rows[3].y, z = position.z - transform.rows[3].z;
    position = {transform.rows[0].y * y + transform.rows[0].x * x + transform.rows[0].z * z,
        transform.rows[1].y * y + transform.rows[1].x * x + transform.rows[1].z * z,
        transform.rows[2].y * y + transform.rows[2].x * x + transform.rows[2].z * z};
    Matrix4x4::SetActiveTransform(&saved);
    return position;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void CaptureFightHitTransform(Character *character, std::int32_t bone, Matrix4x4 &output) {
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::MultiplyWithPrePassMatrix(mtwkGetBoneTransform(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(character), bone));
    Matrix4x4::GetTransformMatrix(&output);
    Matrix4x4::Pop();
    output.rows[3].x = output.rows[3].y = output.rows[3].z = 0.0f;
    Matrix4x4::SetActiveTransform(&output);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::uint16_t FightFootFacingAngle() {
    static constexpr Vector3 forward{0.0f, 0.0f, -1.0f}, up{0.0f, 1.0f, 0.0f};
    Vector3 first, second;
    Matrix4x4::TransformPoint(forward, first);
    Matrix4x4::TransformPoint(up, second);
    if (first.y < -0.2f)
        return FixedAtan2(-second.x, second.y);
    const std::uint16_t angle = first.y > 0.2f ? FixedAtan2(-second.x, second.y) : FixedAtan2(first.x, first.z);
    return static_cast<std::uint16_t>(angle + 0x8000U);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void vfUpdateHitFacingAndKnockback(VFActor *self) {
    self->contactVectors[0] = self->contactPosition;
    const auto readPosition = [self](std::int32_t bone, Vector3 &position) {
        const Matrix4x4 *const transform = mtwkGetBoneTransform(self->motion, bone);
        if (transform)
            position = {transform->rows[3].x, transform->rows[3].y, transform->rows[3].z};
    };
    readPosition(0, self->contactPosition);
    static constexpr std::int32_t bones[6]{3, 28, 26, 5, 8, 15};
    for (std::uint8_t i = 0; i < 6; ++i)
        readPosition(bones[i], self->contactSamples[i]);
    const float height = self->contactSamples[1].y - (self->targetGroundY + 1.5f);
    std::memcpy(&self->actorRuntimeFlags, &height, sizeof(height));
    self->temporaryFacingAngle =
        static_cast<std::uint16_t>(self->facingAngle + ((self->hitStateFlags & VFHSF_FacingStepOnTransition) != 0 ? self->facingAngleDelta : 0));
    if (self->actionTriggerPhase == 4) {
        self->actionTriggerPhase = 0;
        if ((self->hitStateFlags & VFHSF_SpecialThrow) != 0) {
            self->hitStateFlags &= ~VFHSF_SpecialThrow;
            self->game->hitStopFrames = static_cast<std::uint8_t>(static_cast<std::int32_t>(self->inverseScale * 6.0f));
        }
    }
    self->previousFootContactState = self->footContactState;
    if ((self->hitStateFlags & (VFHSF_FacingStepOnTransition | VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) == 0)
        self->facingYaw = self->facingAngle;
    else {
        Matrix4x4_Push();
        CaptureFightHitTransform(self->character, 3, self->chestHitTransform);
        static constexpr Vector3 down{0.0f, -1.0f, 0.0f}, right{1.0f, 0.0f, 0.0f};
        Vector3 first, second;
        Matrix4x4::TransformPoint(down, first);
        Matrix4x4::TransformPoint(right, second);
        std::memcpy(&self->hitBasisPrefix, &first.x, sizeof(first.x));
        self->hitYaw = {first.y, first.z, second.x};
        std::memcpy(self->hitBasisAngles, &second.y, sizeof(second.y));
        self->hitBasisZ = second.z;
        CaptureFightHitTransform(self->character, 31, self->leftLegHitTransform);
        self->footFacingAngles[0] = FightFootFacingAngle();
        CaptureFightHitTransform(self->character, 26, self->rightLegHitTransform);
        self->footFacingAngles[1] = FightFootFacingAngle();
        Matrix4x4::Pop();
        if ((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) != 0) {
            const float threshold = (self->motionFlags & VFMOF_ContactCategoryB) != 0 ? 0.4f : -0.4f;
            if (threshold < self->hitYaw.x)
                self->hitStateFlags &= ~VFHSF_ReversedFacing;
            else
                self->hitStateFlags |= VFHSF_ReversedFacing;
        }
        Vector3 left = FightBoneLocalPosition(self->character, 26);
        const Vector3 rightFoot = FightBoneLocalPosition(self->character, 31);
        const Vector3 root = FightBoneLocalPosition(self->character, 0);
        const float x = rightFoot.x - left.x, z = rightFoot.z - left.z;
        self->footDistance = std::sqrt(x * x + z * z);
        self->pendingAnimation = 0;
        if (std::fabs(rightFoot.y - root.y) <= 0.4f)
            self->pendingAnimation |= 1U;
        if (std::fabs(left.y - root.y) <= 0.4f)
            self->pendingAnimation |= 2U;
        self->footHeightDifference = std::fabs(rightFoot.y - left.y);
        bool fromRoot = false;
        if (self->pendingAnimation >= 3) {
            left.x = (left.x + rightFoot.x) * 0.5f;
            left.z = (left.z + rightFoot.z) * 0.5f;
            self->footFacingMode = static_cast<std::uint16_t>((self->footFacingMode & 0xff00U) | 2U);
            fromRoot = true;
        } else if (self->pendingAnimation != 0 && (self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) != 0) {
            if (self->pendingAnimation == 1) {
                left.x = rightFoot.x;
                left.z = rightFoot.z;
            }
            self->footFacingMode = static_cast<std::uint16_t>((self->footFacingMode & 0xff00U) | 2U);
            fromRoot = true;
        } else if (self->footHeightDifference < 0.4f || self->footDistance <= 0.4f) {
            left.x = (left.x + rightFoot.x) * 0.5f;
            left.z = (left.z + rightFoot.z) * 0.5f;
            self->footFacingMode = static_cast<std::uint16_t>((self->footFacingMode & 0xff00U) | 1U);
            fromRoot = true;
        } else {
            self->footFacingMode &= 0xff00U;
            self->facingYaw =
                rightFoot.y <= left.y ? FixedAtan2(rightFoot.x - left.x, rightFoot.z - left.z) : FixedAtan2(left.x - rightFoot.x, left.z - rightFoot.z);
        }
        if (fromRoot) {
            const auto facing = FixedAtan2(root.x - left.x, root.z - left.z);
            std::uint16_t reverse = self->hitYaw.x < 0.0f ? 0x8000U : 0U;
            if ((left.z - root.z) * self->hitBasisZ + (left.x - root.x) * self->hitYaw.z >= 0.0f)
                reverse ^= 0x8000U;
            if ((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) != 0) {
                if ((reverse & 0x8000U) != 0)
                    self->hitStateFlags |= VFHSF_ReversedFacing;
                else
                    self->hitStateFlags &= ~VFHSF_ReversedFacing;
            }
            self->facingYaw = static_cast<std::uint16_t>(facing + reverse);
        }
    }
    if (((self->hitStateFlags & VFHSF_MirroredReactionAngle) != 0) != ((self->moveFlags & VFMF_MirrorReactionAngle) != 0))
        self->moveFlags |= VFMF_MirroredStance;
    else
        self->moveFlags &= ~VFMF_MirroredStance;
    float lowest = 32000.0f;
    for (const Vector3 &sample : self->motionContactSamples)
        lowest = std::fmin(sample.y, lowest);
    self->lowestMotionContactY = lowest;
    if (self->game->type != 1 || self->groupIndex == self->game->playerSlot) {
        const auto facing = self->temporaryFacingAngle;
        self->leftIntersectionDistance = FightIntersectionDistance(*self, static_cast<std::uint16_t>(facing + 12288), 4.0f);
        self->rightIntersectionDistance = FightIntersectionDistance(*self, static_cast<std::uint16_t>(facing - 12288), 4.0f);
        self->forwardIntersectionDistance = FightIntersectionDistance(*self, facing, 4.0f);
        // Both tags are read from g_vfIntersectionTagSource, which remains zero-initialized.
        self->forwardIntersectionTag = 0;
        self->backwardIntersectionDistance = FightIntersectionDistance(*self, static_cast<std::uint16_t>(facing + 0x8000U), 4.0f);
        self->backwardIntersectionTag = 0;
    }
    if (((self->temporaryFacingAngle - static_cast<std::uint16_t>(camCtrlGetNextRotY())) & 0x8000U) != 0)
        self->moveFlags &= ~VFMF_Unknown02;
    else
        self->moveFlags |= VFMF_Unknown02;
    std::uint32_t guard = 0;
    if ((self->hitStateFlags & VFHSF_AttackActive) != 0) {
        switch (self->guardState) {
        case 0:
            guard = 4;
            [[fallthrough]];
        case 1:
        case 3:
        case 4:
            if ((self->hitParameter & 0x200U) == 0)
                guard |= 1U;
            if (guard == 0)
                guard = 0x8000U;
            break;
        case 2:
        case 5:
        case 6:
            guard = (self->hitParameter & 0x200U) == 0 ? 10U : 8U;
            break;
        default:
            guard = 0x8000U;
            break;
        }
    }
    self->guardContactFlags = guard;
    if ((self->game->gameFlags & 0x20U) == 0) {
        if ((self->hitStateFlags & (VFHSF_ReversedFacing | VFHSF_CameraFocusTarget)) == 0x20000800U)
            self->hitFacingTimer = 30.0f;
        else if (self->hitFacingTimer > 0.0f) {
            self->hitFacingTimer -= shenGetScaledDeltaTickTime(1.0f);
            if (self->hitFacingTimer < 0.0f)
                self->hitFacingTimer = 0.0f;
        }
        const bool secondHalf = static_cast<double>(self->currentAnimationEndFrame) * 0.5 < self->animationTime;
        const bool turning =
            (self->hitStateFlags & VFHSF_Unknown8) != 0 ? self->animationHalfPhase < 0 || secondHalf : self->animationHalfPhase < 0 && !secondHalf;
        if (turning)
            self->moveFlags |= VFMF_Unknown100;
        else
            self->moveFlags &= ~VFMF_Unknown100;
        float groundParameter;
        std::memcpy(&groundParameter, &self->groundParameter, sizeof(groundParameter));
        self->predictedKnockbackFrames = PredictFightKnockbackFrames(*self, groundParameter + self->groundOffset);
        self->previousFacingAngle = static_cast<std::int16_t>(self->facingAngle);
        self->copiedHitStateFlags = self->hitStateFlags;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfRefreshContactReactionState(VFActor *actor) {
    vfUpdateHitFacingAndKnockback(actor);
    vfUpdateContactSlotsAndGroundFlags(actor);
    vfSelectDefaultReactionAndGuardAnims(actor);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateContactSlotsAndGroundFlags(VFActor *self) {
    VFMA_t &game = *self->game;
    if (self->actionIndex == -1 || self->pairedSlot == -1)
        return;
    const auto team = game.slotTeams[self->groupIndex];
    VFActor &paired = *game.actors[self->pairedSlot];
    VFActor &opponent = *game.actors[self->actionIndex];
    const std::uint32_t selfHit = self->hitStateFlags;
    const std::uint32_t pairedHit = paired.hitStateFlags;
    const std::uint32_t opponentHit = opponent.hitStateFlags;
    bool nearOpponent = game.state != 3;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game.characters[i]) {
            self->contactDistances[i] = 0.0f;
            continue;
        }
        const Vector3 &position = game.actors[i]->contactPosition;
        const float x = self->contactPosition.x - position.x, z = self->contactPosition.z - position.z;
        const float distance = std::sqrt(z * z + x * x);
        self->contactDistances[i] = distance;
        if (game.slotTeams[i] != team && distance < 7.0f)
            nearOpponent = true;
    }
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game.characters[i]) {
            self->contactAngles[i] = 0;
            continue;
        }
        const Vector3 &position = game.actors[i]->contactPosition;
        self->contactAngles[i] = static_cast<std::int16_t>(FixedAtan2(self->contactPosition.x - position.x, self->contactPosition.z - position.z));
    }
    const float distance = self->contactDistances[self->pairedSlot];
    const auto pairedAngle = self->contactAngles[self->pairedSlot];
    const auto selfAngle = static_cast<std::int16_t>(self->temporaryFacingAngle - pairedAngle);
    const auto otherAngle = static_cast<std::int16_t>(paired.temporaryFacingAngle - pairedAngle);
    const auto rotation = static_cast<std::uint16_t>(-opponent.facingAngle);
    const float x = self->moveDelta.x - opponent.moveDelta.x, z = self->moveDelta.z - opponent.moveDelta.z;
    // The native caller initializes only the planar input components.
    self->relativeMoveDelta.x = cos_fixed16(rotation) * x - sin_fixed16(static_cast<std::int16_t>(rotation)) * z;
    self->relativeMoveDelta.z = sin_fixed16(static_cast<std::int16_t>(rotation)) * x + cos_fixed16(rotation) * z;
    std::uint32_t ground = nearOpponent ? 4U : 0U;
    const auto angle = self->contactAngles[self->actionIndex];
    if (((angle - static_cast<std::int16_t>(self->temporaryFacingAngle) + 0x4000) & 0x8000) != 0)
        ground |= 1U;
    if (((angle - static_cast<std::int16_t>(opponent.temporaryFacingAngle) + 0x4000) & 0x8000) == 0)
        ground |= 2U;
    std::uint32_t reaction = 0;
    const auto inAngleRange = [](std::int32_t angleValue, std::uint16_t offset, std::uint16_t extent) {
        return static_cast<std::uint16_t>(angleValue + offset) <= extent;
    };
    const bool pairedGuardEligible = (pairedHit & 0x4000U) == 0 || (paired.activeMotionFlags & 0x100000U) != 0;
    if ((self->moveFlags & VFMF_Unknown40000000) != 0 && (self->stateFlags & VFSF_Unknown80) == 0 && (paired.stateFlags & 0x80U) == 0 &&
        (selfHit & 0x8dc100U) == 0 && (self->activeMotionFlags & VFSMF_Unknown800000) == 0 && (pairedHit & 0x8d8110U) == 0 &&
        (paired.activeMotionFlags & 0x800020U) == 0 && !(std::fabs(self->hp) < 0.00000011920929f) && !(std::fabs(paired.hp) < 0.00000011920929f) &&
        pairedGuardEligible && paired.fightStyle != 1 && (std::to_integer<std::uint8_t>(paired.motionParameters[2][25]) & 2U) == 0 &&
        static_cast<std::uint16_t>((paired.currentAnimation & 0xfffU) - 58) > 1U && game.guardTimer > 0.0f) {
        const bool selfFront = inAngleRange(selfAngle, 8190, 0x3ffc);
        const bool selfBack = inAngleRange(selfAngle ^ 0x8000, 8190, 0x3ffc);
        const bool pairedFront = inAngleRange(otherAngle, 8190, 0x3ffc);
        const bool pairedBack = inAngleRange(otherAngle ^ 0x8000, 8190, 0x3ffc);
        const bool selfGrounded = (selfHit & 0x10U) == 0;
        const bool pairedTurning = (paired.moveFlags & 0x100U) != 0;
        if (selfBack && pairedBack && selfGrounded && self->rearContactRange >= distance)
            reaction = 2;
        else if (selfFront) {
            if (pairedFront && selfGrounded && self->rearContactRange >= distance)
                reaction = pairedTurning ? 16 : 8;
            else if (pairedBack && self->frontContactRange >= distance)
                reaction = pairedTurning && selfGrounded ? 4 : selfGrounded ? 1 : 0x400000;
            else if (inAngleRange(otherAngle, static_cast<std::uint16_t>(-8194), 0x3ffc) && selfGrounded && self->sideContactRange >= distance)
                reaction = pairedTurning ? 0x20000000 : 0x4000000;
            else if (inAngleRange(otherAngle, 24574, 0x3ffc) && selfGrounded && self->sideContactRange >= distance)
                reaction = pairedTurning ? 0x10000000 : 0x2000000;
        }
    }
    bool opponentGuard;
    if ((opponent.moveFlags & 0x20000000U) != 0)
        opponentGuard = true;
    else if ((opponentHit & 0x8100U) != 0)
        opponentGuard = false;
    else if ((opponentHit & 0x10000U) != 0)
        opponentGuard = true;
    else if ((opponentHit & 0x4000U) != 0)
        opponentGuard = (opponentHit & 0x10U) == 0 || self->predictedKnockbackFrames == 0;
    else
        opponentGuard = false;
    if (opponentGuard)
        ground |= 0x20U;
    std::uint32_t mask = 0;
    if ((self->moveFlags & VFMF_Unknown40000000) != 0 && (pairedHit & 0x102U) == 0x102U && inAngleRange(selfAngle, 5460, 0x2aa8) &&
        (inAngleRange(otherAngle ^ 0x8000, 5460, 0x2aa8) || (paired.currentAnimation & 0xfffU) == 0xc5cU)) {
        mask = distance > 2.0f ? 0x733f00U : 0x7fff00U;
        if (static_cast<double>(distance) > 1.8)
            mask &= 0xffafd0ffU;
        if (static_cast<double>(distance) > 1.6)
            mask &= ~0x1000U;
        mask &= opponent.reactionEntryFlags;
    }
    self->hitFlags = mask;
    if ((self->pairedContactPriority & 1U) == 0)
        self->pairedContactPriority = static_cast<std::uint32_t>(paired.pairContactPriority);
    if (((self->pairedContactPriority & 1U) != 0 || paired.pairContactPriority != 0) &&
        opponent.animationTime > static_cast<float>(paired.pairContactEndFrame)) {
        self->pairedContactPriority = 0;
        paired.pairContactPriority = 0;
    }
    std::uint32_t orientation = (opponent.reactionEntryFlags & 1U) != 0 ? 0x48U : 0U;
    if ((self->moveFlags & VFMF_MirroredStance) != 0)
        orientation ^= 8U;
    if ((opponent.moveFlags & 0x400U) != 0)
        orientation ^= 0x40U;
    ground |= orientation;
    const std::uint32_t facingDifference = ((selfHit ^ opponentHit) & 0x200000U) != 0 ? 0x40U : selfHit ^ opponentHit;
    if (((self->moveFlags ^ opponent.moveFlags ^ facingDifference) & 0x40U) != 0)
        ground |= 0x10U;
    if (self->forwardIntersectionDistance <= 2.5f)
        reaction |= 0x100U;
    if (self->backwardIntersectionDistance <= 1.5f)
        reaction |= 0x200U;
    self->reactionFlags = reaction;
    self->groundContactFlags = ground;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// The last populated actor from the metrics loop
// remains the timer source.
// @verified - p
void vfUpdateSlotRuntimeMetricsAndTimers(VFActor *self) {
    VFSlotRuntimeState &slot = *self->slot;
    VFActor *actor = self->actor;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!g_pFightMain->characters[i])
            continue;
        actor = g_pFightMain->actors[i];
        slot.healthDifferences[i] = self->hp - actor->hp;
        const auto difference = static_cast<std::int16_t>(self->facingAngle - self->contactAngles[i]);
        slot.facingDifferences[i] = static_cast<std::uint16_t>(std::abs(static_cast<std::int32_t>(difference)));
    }
    slot.randomBytes[0] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    slot.randomBytes[1] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    slot.randomWord = IncrementSharedPrngUInt16();
    slot.previousTicks = slot.ticks;
    const float delta = shenGetScaledDeltaTickTime(1.0f);
    if (static_cast<std::int32_t>(slot.ticks) < 65535)
        slot.ticks += delta;
    if (slot.reactionTicks < 60000.0f)
        slot.reactionTicks += delta;
    const auto decrement = [delta](float &timer) {
        if (timer > 0.0f) {
            timer -= delta;
            if (timer < 0.0f)
                timer = 0.0f;
        }
    };
    if ((self->hitStateFlags & VFHSF_MidAirRecovery) != 0)
        slot.hitTimer = static_cast<float>(slot.hitTimerMaximum);
    else
        decrement(slot.hitTimer);
    if ((self->hitStateFlags & VFHSF_AttackActive) != 0)
        slot.guardTimer = static_cast<float>(slot.guardTimerMaximum);
    else
        decrement(slot.guardTimer);
    if ((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) != 0)
        slot.guardTicks += delta;
    else {
        slot.guardTicks = 0.0f;
        slot.guardPhase = 0;
    }
    if ((actor->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) != 0)
        slot.opponentGuardTicks += delta;
    else
        slot.opponentGuardTicks = 0.0f;
    if ((actor->hitStateFlags & VFHSF_LightReaction) != 0) {
        if (static_cast<std::int32_t>(slot.opponentAttackTimer) < 255)
            slot.opponentAttackTimer += delta;
    } else
        decrement(slot.opponentAttackTimer);
    if ((self->hitStateFlags & VFHSF_AirborneOrDowned) != 0) {
        if (slot.pressure < 240)
            slot.pressure += 3;
    } else if (slot.pressure != 0)
        --slot.pressure;
    decrement(slot.parameterBc);
    if ((actor->hitStateFlags & VFHSF_ForcedReaction) != 0) {
        if (static_cast<std::int32_t>(slot.opponentMotionTimer) < 240)
            slot.opponentMotionTimer += 8.0f;
    } else
        decrement(slot.opponentMotionTimer);
    slot.parameterE4 = 0;
    if ((self->activeMotionFlags & VFSMF_Unknown800000) != 0 && slot.motionTicks < 60000.0f)
        slot.motionTicks += delta;
    decrement(slot.hpPhaseTimer);
    if (!(slot.motionTimer > 0.0f)) {
        if ((self->motionFlags & VFMOF_Unknown80000000) != 0)
            slot.motionTimer = 90.0f;
    } else
        decrement(slot.motionTimer);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateSlotReactionHistory(VFActor *self) {
    VFSlotRuntimeState &slot = *g_pFightMain->actorSlots[self->groupIndex];
    const VFActor &opponent = *g_pFightMain->actors[self->actionIndex];
    if ((opponent.motionFlags & 0x100U) != 0 && (self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_Unknown20000)) != 0 &&
        self->currentAnimation != self->previousAnimation && (opponent.motionFlags & 2U) == 0 && (self->hitStateFlags & VFHSF_GuardState) == 0) {
        auto index = static_cast<std::uint8_t>(slot.parameterA3);
        slot.animationHistory[index++] = opponent.currentAnimation & 0xfffU;
        slot.parameterA3 = static_cast<std::int8_t>(index > 3 ? 0 : index);
    }
    slot.cameraFlags &= ~0x400U;
    for (std::uint8_t i = 0; i < 3; ++i) {
        if (slot.animationHistory[i] == 0)
            continue;
        for (std::uint8_t j = i + 1; j < 4; ++j) {
            if (slot.animationHistory[i] != slot.animationHistory[j])
                continue;
            slot.cameraFlags |= 0x400U;
            slot.parameterA0 = slot.animationHistory[i];
            break;
        }
        if ((slot.cameraFlags & 0x400U) != 0)
            break;
    }
    const VFActor *const second = self->secondActor;
    const auto reactionByte = [](const VFActor &actor) {
        std::uint32_t bits;
        std::memcpy(&bits, &actor.reactionPower, sizeof(bits));
        return static_cast<std::int8_t>(bits >> 24);
    };
    if ((self->previousHitStateFlags & VFHSF_AirborneOrDowned) == 0 && (self->hitStateFlags & VFHSF_AirborneOrDowned) != 0 && second &&
        second->fightStyle == 0 && second->pairContactPriority != 0) {
        auto index = static_cast<std::uint8_t>(slot.parameterA4);
        slot.inputHistory[index++] = static_cast<std::uint16_t>(static_cast<std::int16_t>(reactionByte(*second)));
        slot.parameterA4 = static_cast<std::int8_t>(index == 3 ? 0 : index);
    }
    slot.parameterA2 = 0;
    if (second && second->fightStyle == 0 && slot.inputHistory[1] != 0) {
        for (const auto reaction : slot.inputHistory)
            if (static_cast<std::int32_t>(reactionByte(*second)) == static_cast<std::int32_t>(reaction))
                ++slot.parameterA2;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateSlotRankParameters(VFActor *self) {
    VFSlotRuntimeState &slot = *g_pFightMain->actorSlots[self->groupIndex];
    const std::uint8_t rank = self->reactionRank < 2 ? self->reactionRank : 2;
    std::memcpy(&slot.parameter2a, self->motionParameters[2] + 26 + 9 * rank, 9);
    if (slot.parameter129 > 20) {
        if (slot.parameter2a != 0)
            slot.parameter2a = -1;
        slot.parameter129 = 0;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateCurrentAssistWindow(VFActor *self) {
    const VFActor &actor = *self->actor;
    VFSlotRuntimeState &slot = *self->slot;
    if ((actor.hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) != 0)
        slot.assistWindowCurrent = slot.assistWindowGuard;
    else if ((actor.groundContactFlags & 1U) != 0)
        slot.assistWindowCurrent = (static_cast<std::uint8_t>(g_pFightMain->type - 2U) & 0xfdU) != 0 ? 1.2f : 1.5f;
    else if ((actor.hitStateFlags & (VFHSF_Unknown4 | VFHSF_AttackActive)) != 0)
        slot.assistWindowCurrent = 2.5f;
    else
        slot.assistWindowCurrent = slot.assistWindowDefault;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Six native twenty-byte phase configurations.
// @verified - p
void vfUpdateScenarioHPPhaseConfig(VFActor *self) {
    const auto flags = self->actor->stateFlags;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if ((flags & (i < 5 ? 0x1000U : 0x2000U)) == 0)
            continue;
        VFSlotPhaseConfig &phase = self->slot->phases[i];
        if (phase.enabled == 0)
            continue;
        if (phase.countdown != 0)
            --phase.countdown;
        else {
            phase.countdown = phase.choices[IncrementSharedPrngUInt16() & 0xfU];
            if (phase.countdown > 1)
                --phase.countdown;
            phase.selected = static_cast<std::uint8_t>(IncrementSharedPrngUInt16() % (static_cast<std::uint32_t>(phase.countdown) + 1U));
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfInitializeSlotPhaseConfigurations(VFActor *self) {
    const auto initialize = [](VFSlotPhaseConfig &phase, std::uint8_t parameter) {
        if (parameter == 0) {
            phase.enabled = 0;
            return;
        }
        phase.enabled = 1;
        phase.reserved = parameter >= 0x80U ? 1 : 0;
        const float value = 255.0f / static_cast<float>(parameter >= 0x80U ? 256U - parameter : parameter);
        const auto integral = static_cast<std::uint8_t>(static_cast<std::int32_t>(value));
        std::memset(phase.choices, integral, sizeof(phase.choices));
        float remainder = value - static_cast<float>(integral);
        for (std::uint8_t index = 0; remainder >= 0.0625f; ++index) {
            ++phase.choices[index];
            remainder = static_cast<float>(static_cast<double>(remainder) - 0.0625);
        }
        phase.countdown = phase.choices[IncrementSharedPrngUInt16() & 0xfU];
        if (phase.countdown > 1)
            --phase.countdown;
        phase.selected = static_cast<std::uint8_t>(IncrementSharedPrngUInt16() % (static_cast<std::uint32_t>(phase.countdown) + 1U));
    };
    const std::uint8_t rank = self->reactionRank < 2 ? self->reactionRank : 2;
    const std::byte *const parameters = self->motionParameters[2] + 9 * rank;
    static constexpr std::uint8_t offsets[6]{27, 28, 29, 30, 32, 34};
    for (std::uint8_t i = 0; i < 6; ++i)
        initialize(self->slot->phases[i], std::to_integer<std::uint8_t>(parameters[offsets[i]]));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateGuardLevel(VFMA_t *game, std::uint8_t index) {
    VFActor *const actor = game->actors[index];
    if (!actor)
        return;
    VFSlotRuntimeState *const slot = game->actorSlots[index];
    if (!slot)
        return;
    const auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    slot->id = scenGetCharaName(control->scenarioTask, actor->character).name;
    slot->actor = actor;
    slot->opponentIndex = 0xffU;
    std::memset(reinterpret_cast<std::byte *>(slot) + 0x68, 0, 0x140);
    slot->resetIndex = 0x7f7fU;
    slot->reserved62[0] = std::byte{0x7f};
    slot->hpPhaseTimer = 240.0f;
    slot->cameraFlags = 1;
    const auto *const parameters = reinterpret_cast<const std::uint8_t *>(actor->motionParameters[2]);
    const auto word = [parameters](std::size_t offset) {
        return static_cast<std::uint16_t>((static_cast<std::uint16_t>(parameters[offset]) << 8) | parameters[offset + 1]);
    };
    const auto fixed = [parameters](std::size_t offset) {
        const std::uint32_t bits = (static_cast<std::uint32_t>(parameters[offset]) << 24) | (static_cast<std::uint32_t>(parameters[offset + 1]) << 16) |
            (static_cast<std::uint32_t>(parameters[offset + 2]) << 8) | parameters[offset + 3];
        return static_cast<float>(static_cast<std::int32_t>(bits)) * 0.000015258789f;
    };
    slot->assistWindowCurrent = slot->assistWindowDefault = fixed(4);
    slot->assistWindowGuard = fixed(8);
    slot->parameter20 = word(12);
    slot->parameter22 = word(14);
    slot->hitTimerMaximum = word(16);
    slot->guardTimerMaximum = word(18);
    slot->parameter28 = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(parameters[20])));
    actor->guardLevel = static_cast<std::int8_t>(parameters[21]);
    actor->guardRateScale = 1.0f / static_cast<float>(static_cast<std::int8_t>(parameters[22]));
    slot->parameter1a5 = static_cast<std::int8_t>(parameters[23]);
    slot->secondaryMotionTimer = slot->motionTimer = 0.0f;
    if ((parameters[25] & 1U) != 0)
        actor->moveFlags |= VFMF_NoContactPairing;
    vfInitializeSlotPhaseConfigurations(actor);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfPrepareEnemySlotMotion(FCMG_t *group) {
    Character *const character = group->character;
    MotionWork *const motion = group->motion;
    std::uint32_t flags = SetFlagForToken(character, SHEN_NAME(CCM_), 0) == 1 ? 4U : 0U;
    if ((motion->runtime.callbackFlags & 1U) != 0)
        flags |= 8U;
    else
        mtwkSetEnabled(motion, true);
    if (mtwkSetExtraFlag(motion, 6, 0) != 0)
        flags |= 0x20U;
    if (shenCheckCharaHasAlphaManager(character))
        flags |= 0x80U;
    group->savedControlFlags = flags;
    group->savedMotionScale = mtwkGetSpeed(motion, MotionRegion::LowerBody);
    mtwkSetSpeed(motion, 1.0f, MotionRegion::LowerBody);
    group->savedRootMode = 0;
    for (const auto regionFlags : motion->runtime.regionFlags) {
        if ((regionFlags & 0x10U) != 0) {
            group->savedRootMode = motion->runtime.currentRootMode;
            break;
        }
    }
    mtwkSetRequestedRootMode(motion, 2);
    group->savedConstraintAngle = static_cast<std::uint16_t>(static_cast<std::int32_t>(motion->runtime.constraintStepLimit[2] * 10430.378f));
    motion->runtime.constraintStepLimit[2] = 2048.0f / 10430.378f;
    group->savedProjectionAngle = static_cast<std::uint16_t>(AtanToFixedAngle(std::fabs(motion->runtime.projectionCotangentNear)));
    motion->runtime.projectionCotangentNear = cos_fixed16(10922) / sin_fixed16(10922);
    if ((g_pFightStage->battleFlags & 2U) == 0)
        cpamSetFlag(character, 1);
    group->savedExtraMotionFlag = mtwkSetExtraFlag(motion, 0x37, 1);
    mtwkSetExtraFlag(motion, 0x37, 0);
    for (std::uint8_t i = 0; i < 7; ++i) {
        group->savedMotionCallbacks[i] = mtwkGetTargetCallback(motion, fightMotionSelectors[i]);
        group->savedMotionParameters[i] = mtwkGetTargetParameter(motion, fightMotionSelectors[i]);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfRestoreEnemySlotMotion(FCMG_t *group) {
    const auto flags = group->savedControlFlags;
    Character *const character = group->character;
    MotionWork *const motion = group->motion;
    if ((flags & 4U) != 0) {
        SetFlagForToken(character, SHEN_NAME(CCM_), 1);
        if ((g_pFightStage->battleFlags & 0x8000U) != 0) {
            if (CharacterControlModel *const control = FindCharacterControlModel(character))
                control->flags &= ~3U;
        }
    }
    if ((flags & 8U) == 0)
        mtwkSetEnabled(motion, false);
    if ((flags & 0x20U) == 0)
        mtwkSetExtraFlag(motion, 6, 2);
    cpamSetFlag(character, (flags & 0x80U) != 0);
    mtwkSetSpeed(motion, group->savedMotionScale, MotionRegion::LowerBody);
    if ((group->flags & 2U) != 0)
        mtwkSetCulled(motion, true);
    mtwkSetRequestedRootMode(motion, group->savedRootMode);
    motion->runtime.constraintStepLimit[2] = static_cast<float>(group->savedConstraintAngle) / 10430.378f;
    const auto projection = static_cast<std::uint16_t>(group->savedProjectionAngle);
    motion->runtime.projectionCotangentNear = cos_fixed16(projection) / sin_fixed16(static_cast<std::int16_t>(projection));
    mtwkSetExtraFlag(motion, 0x37, group->savedExtraMotionFlag != 0 ? 0 : 2);
    for (std::uint8_t i = 0; i < 7; ++i) {
        mtwkAssignTargetCallback(motion, group->savedMotionCallbacks[i], fightMotionSelectors[i]);
        mtwkAssignTargetParameter(motion, group->savedMotionParameters[i], fightMotionSelectors[i]);
    }
    mtwkSetExtraFlag(motion, 1, 2);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Native big-endian style animation lookup.
// @verified - p
void vfSelectDefaultReactionAndGuardAnims(VFActor *actor) {
    static constexpr std::uint16_t indices[4][3]{{294, 296, 298}, {300, 302, 304}, {322, 296, 298}, {0, 16, 20}};
    std::uint8_t row = 3;
    if (actor->fightStyle != 0)
        row = actor->reactionRank == 0 ? 0 : 2;
    else if (static_cast<std::int8_t>(actor->guardLevel - actor->actor->guardLevel) > 0) {
        if (static_cast<double>(actor->initialHp) * 0.5 <= actor->hp)
            row = actor->contactDistances[actor->actionIndex] >= 4.0f ? 1 : 0;
        else
            row = 2;
    }
    const auto *const ids = reinterpret_cast<const std::uint8_t *>(actor->motionParameters[4]);
    for (std::uint8_t i = 0; i < 3; ++i) {
        const std::uint16_t offset = indices[row][i];
        actor->defaultAnimations[i] = static_cast<std::uint16_t>((static_cast<std::uint16_t>(ids[offset]) << 8) | ids[offset + 1]);
    }
    actor->stateFlags &= ~VFSF_Unknown80000;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfUpdateDefaultReactionAnim(FCMG_t *group, std::uint16_t animation) {
    mtwkSetFrameTime(group->motion, 5, MotionRegion::LowerBody);
    mtwkRequestMotion(group->motion, animation, MotionRegion::LowerBody);
    if (group->fighter && group->state == 2)
        mtwkSetRegionFlag(group->motion, 69, (group->fighter->moveFlags & 0x40U) != 0 ? 0 : 2, MotionRegion::LowerBody);
    group->flags |= 1U;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// The priority byte is at offset 0xa6a.

// @verified - p
std::uint8_t vfSelectOpponentSlotByPriority(VFMA_t *game, std::uint8_t minimumPriority) {
    float distance = 0.1f;
    std::uint8_t selected = 99;
    for (std::uint8_t i = 0; i < 6; ++i) {
        const VFActor *const actor = game->actors[i];
        if (!game->characters[i] || i == game->playerSlot)
            continue;
        if (game->slotTeams[i] == game->slotTeams[game->playerSlot] || static_cast<std::int32_t>(game->enemySlots[i]->runningTicks) < 15 ||
            (actor->moveFlags & VFMF_ForceDefeat) != 0 || ((actor->motionFlags | actor->hitStateFlags) & 0xdd110U) != 0 ||
            ((actor->secondaryMotionFlags | actor->activeMotionFlags) & 0x900c80U) != 0)
            continue;
        if (actor->selectionPriority > minimumPriority) {
            selected = i;
            minimumPriority = actor->selectionPriority;
        } else if (actor->selectionPriority == minimumPriority && actor->contactDistances[game->playerSlot] > distance) {
            distance = actor->contactDistances[game->playerSlot];
            selected = i;
            minimumPriority = actor->selectionPriority;
        }
    }
    return selected;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Decoded Sequence opcode 1 preserves the
// native 0xffff frame-overflow predicate motnIsCurrentAnimOpenEnded.
// @verified - p
void vfUpdateEnemySlotFormationAndYaw(VFMA_t *) {
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &group = g_pFightStage->enemySlots[i];
        if (!group.character || !group.group || group.state != 3 || (group.collisionFlags & 1U) == 0)
            continue;
        if (mtwkSetRegionFlag(group.motion, 65, 1, MotionRegion::LowerBody) == 0)
            continue;
        const MotnSequence *const sequence = FindLoadedMotionSequence(mtwkGetAnimationId(group.motion, MotionRegion::LowerBody));
        if (sequence && sequence->allowsFrameOverflow)
            continue;
        const std::uint16_t animation = (group.collisionFlags & 0x20U) != 0 ? 453 :
            (group.collisionFlags & 0x40U) != 0                             ? 452 :
            (group.collisionFlags & 2U) != 0                                ? 101 :
                                                                              104;
        vfUpdateDefaultReactionAnim(&group, animation);
        mtwkSetRegionFlag(group.motion, 69, (group.collisionFlags & 0x80U) != 0 ? 0 : 2, MotionRegion::LowerBody);
        if (group.savedFacingAngle != 0) {
            const auto yaw = GetAllAngYForNode(&group.character->entity.node);
            shenDecrementRotYAllNodes(&group.character->entity.node, static_cast<std::int16_t>(yaw + group.savedFacingAngle));
            group.savedFacingAngle = 0;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfRemoveCharaFromGroup(VFMA_t *game, FCMG_t *group) {
    if (group->character)
        group->character->flags &= ~CF_Unknown40;
    game->characters[group->actorIndex] = nullptr;
    group->actorIndex = 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// DEAD is a name-valued pointer sentinel, never a Character allocation.
// @verified - p
bool ReactToDead(VFMA_t *game, FCMG_t *slot) {
    FRMG_t *formation = nullptr;
    for (std::uint8_t i = 0; i < g_pFightStage->groupCount; ++i) {
        FRMG_t &candidate = g_pFightStage->groups[i];
        if (candidate.character ==
            reinterpret_cast<Character *>(static_cast<std::uintptr_t>(SHEN_NAME(DEAD))) && candidate.state == 0 && candidate.team == slot->counterGroup) {
            formation = &candidate;
            break;
        }
    }
    if (!formation)
        return false;
    mtwkRemoveCallback(slot->motion);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    mtwkSetExtraFlag(slot->motion, 3, 2);
    formation->character = slot->character;
    std::uint16_t yaw = formation->spawnWords[1];
    if (game->playerSlot != -1 && (formation->activationState & 0xffU) == 0) {
        VFActor *const player = game->actors[game->playerSlot];
        yaw = FixedAtan2(formation->spawnPosition.x - player->position.x, formation->spawnPosition.z - player->position.z);
    }
    shenApplyTranslationToAll(&formation->character->entity.node, formation->spawnPosition);
    slot->position = formation->spawnPosition;
    shenDecrementRotYAllNodes(&formation->character->entity.node, static_cast<std::int16_t>(yaw));
    vfUpdateDefaultReactionAnim(slot, formation->spawnWords[0]);
    mtwkSetFrameTime(slot->motion, 1, MotionRegion::LowerBody);
    // The target predicate always returns -1.
    slot->quadrant = -1;
    slot->group = formation;
    formation->slot = slot;
    --g_pFightStage->secondaryGroupCounters[slot->counterGroup];
    slot->setPosition = 0;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
bool vfEnemySlotResetAndRestartCycle(VFMA_t *game, FCMG_t *group) {
    const auto team = group->counterGroup;
    switch (group->state) {
    case 0:
    case 4:
        if (!BindAllocatedEnemySlot(group))
            return false;
        break;
    case 1:
        return false;
    case 2:
        if (static_cast<std::int32_t>(group->runningTicks) < 30)
            return false;
        if (group->group)
            group->group->totalHp = static_cast<std::uint16_t>(static_cast<std::int32_t>(group->fighter->hp));
        group->position = GetCharacterWorldPosition(group->character);
        group->accumulatedYaw = static_cast<std::uint16_t>(GetAllAngYForNode(&group->character->entity.node));
        vfSelectDefaultReactionAndGuardAnims(group->fighter);
        vfUpdateDefaultReactionAnim(group, group->fighter->defaultAnimations[0]);
        mtwkRemoveCallback(group->motion);
        vfRemoveCharaFromGroup(game, group);
        group->group->state = 0;
        g_activeFightData[group->fightEntryIndex].status = 0;
        --g_pFightStage->activeFightersByTeam[team];
        break;
    case 3:
        if (!ReactToDead(game, group))
            return false;
        --g_pFightStage->groupCounters[team];
        group->fadeAlpha = -1.0f;
        modelSetCharaAlpha(group->character, 0.0f);
        break;
    default:
        break;
    }
    group->quadrant = -1;
    group->state = 1;
    group->runningTicks = 0.0f;
    ++g_pFightStage->readyEnemySlots[team];
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Installs the real cooldown target producer.
// @verified - p
void vfCaptureGroundForCooldown(VFMA_t *game, VFActor *actor) {
    FCMG_t *const group = game->enemySlots[actor->groupIndex];
    float *const heights = actor->groundHeights;
    if (game->type != 8 && (g_pFightStage->battleFlags & 0x800U) == 0) {
        if (static_cast<double>(heights[3]) - 0.15 > heights[2])
            heights[2] = heights[3] - 0.15000001f;
        const float lower = heights[2] - 0.1f;
        if (lower > heights[3])
            heights[3] = lower;
        if (lower > heights[5])
            heights[5] = lower;
        if (lower > heights[6])
            heights[6] = lower;
        const float foot = heights[2] - 0.15000001f;
        if (foot > heights[0])
            heights[0] = foot;
        if (foot > heights[1])
            heights[1] = foot;
    }
    if (game->type == 8) {
        if ((actor->groundFlags & 0x400U) != 0)
            heights[0] = heights[2];
        if ((actor->groundFlags & 0x2000U) != 0)
            heights[1] = heights[2];
    }
    std::uint32_t mask = 0;
    for (std::uint8_t i = 0; i < 7; ++i) {
        actor->previousGroundSamples[i] = -10000.0f;
        group->stackLayerOffsets[i] = heights[i];
        group->groundCorrectionEnabled[i] = static_cast<double>(std::fabs(group->position.y - heights[i])) > 0.001;
        group->stackLayerMasks[i] = actor->nearbyGroundActors[i];
        mask |= actor->nearbyGroundActors[i];
    }
    group->stackReferenceMask = mask;
    for (const auto selector : fightMotionSelectors) {
        mtwkAssignTargetCallback(actor->motion, AdjustCooldownGroundTarget, selector);
        mtwkAssignTargetParameter(actor->motion, group, selector);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
bool vfTransitionEnemySlotToCooldown(VFMA_t *game, FCMG_t *group) {
    const auto state = group->state;
    const auto team = group->counterGroup;
    group->position = GetCharacterWorldPosition(group->character);
    mtwkRemoveCallback(group->motion);
    switch (state) {
    case 1:
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2: {
        VFActor *const actor = group->fighter;
        vfCaptureGroundForCooldown(game, actor);
        group->savedFacingAngle = actor->facingAngleDelta;
        group->quadrant = actor->cooldownQuadrant;
        for (float &sample : group->groundSamples)
            sample = -10000.0f;
        std::uint32_t flags = 0;
        if ((actor->hitStateFlags & VFHSF_SpecialGuardOrParry) != 0) {
            flags = 1;
            if ((actor->motionFlags & VFMOF_Unknown4000000) != 0)
                flags = (actor->motionFlags & VFMOF_ContactCategoryB) != 0 ? 65U : 33U;
            else if ((actor->hitStateFlags & VFHSF_ReversedFacing) != 0)
                flags = 3;
        }
        if ((actor->moveFlags & VFMF_MirrorReactionAngle) != 0)
            flags |= 0x80U;
        if ((actor->hitStateFlags & VFHSF_AlignedToGround) != 0)
            flags |= 0x200U;
        if ((group->group->selectFlags & 0xa0U) != 0)
            flags |= 0x100U;
        group->collisionFlags = flags;
        vfRemoveCharaFromGroup(game, group);
        --g_pFightStage->activeFightersByTeam[team];
        break;
    }
    case 3:
        return false;
    default:
        break;
    }
    group->state = 3;
    group->runningTicks = 0.0f;
    group->group->state = 2;
    g_activeFightData[group->fightEntryIndex].status = 2;
    group->cooldownDelay = static_cast<std::uint8_t>(g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1]);
    group->fadeAlpha = 1.0f;
    mtwkSetExtraFlag(group->motion, 0, 2);
    ++g_pFightStage->groupCounters[team];
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void mtwkRegisterCooldownTargetCallbacks(MotionWork *motion, FCMG_t *slot) {
    for (std::int32_t index = 0; index < 7; ++index) {
        mtwkAssignTargetCallback(motion, AdjustCooldownGroundTarget, fightMotionSelectors[index]);
        mtwkAssignTargetParameter(motion, slot, fightMotionSelectors[index]);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfSynchronizeActorPosition(FightActorRuntimePrefix *actor) {
    GetCharacterWorldPosition(actor->character, &actor->position);
    actor->facingAngle = static_cast<std::uint16_t>(GetCharacterWorldYaw(actor->character));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfApplyActorPosition(FightActorRuntimePrefix *actor) {
    SetCharacterWorldYaw(actor->character, static_cast<std::int16_t>(actor->facingAngle));
    SetCharacterWorldPosition(actor->character, &actor->position);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfResetActorActionState(FightActorRuntimePrefix *actor) {
    std::byte *const bytes = reinterpret_cast<std::byte *>(actor);
    *reinterpret_cast<std::uint32_t *>(bytes + 0x5b0) = 0;
    *reinterpret_cast<std::uint32_t *>(bytes + 0x5b4) = *reinterpret_cast<std::uint32_t *>(bytes + 0x28);
    *reinterpret_cast<std::uint16_t *>(bytes + 0x5a8) = 0;
    bytes[0x5a6] = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float vfFieldGetIntersection(FightActorRuntimePrefix *actor, Vector3 &position) {
    float height = flddGetIntersectionFor(actor ? actor->character : 0, position);
    if (g_currentMap == SHEN_NAME4('Q', '1', '0', '0')) {
        FightRuntimePrefix *const game = vfmaGetTaskParam();
        if (game)
            height = game->floorIntersectionY;
    }
    return height;
}

#else

// @verified - p
float vfFieldGetIntersection(VFActor *actor, Vector3 &position) {
    const float height = flddGetIntersectionFor(actor ? actor->character : nullptr, position);
    if (g_currentMap.name == SHEN_NAME(Q100)) {
        VFMA_t *const game = vfmaGetTaskParam();
        if (game)
            return game->floorIntersectionY;
    }
    return height;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void AdjustCooldownGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *, void *context) {
    std::uint32_t index;
    switch (controller) {
    case 5:
        index = 0;
        break;
    case 6:
        index = 1;
        break;
    case 0:
        index = 2;
        break;
    case 1:
        index = 3;
        break;
    case 2:
        index = 4;
        break;
    case 3:
        index = 5;
        break;
    case 4:
        index = 6;
        break;
    default:
        return;
    }
    static const float cooldownGroundTargetHeights[7] = {0.13f, 0.13f, 0.17f, 0.2f, 0.2f, 0.15f, 0.15f};
    FCMG_t *const slot = static_cast<FCMG_t *>(context);
    if (!slot->groundCorrectionEnabled[index])
        return;
    const float base = slot->stackLayerOffsets[index];
    const float bodyLimit = cooldownGroundTargetHeights[index] + slot->position.y;
    const float target = base + cooldownGroundTargetHeights[index];
    float &previous = slot->groundSamples[index];
    if (position->y <= bodyLimit && position->y > target) {
        if (previous < -9999.9f)
            previous = position->y - target;
        previous *= 0.5f;
        position->y = previous + base;
    } else {
        if (target > position->y) {
            previous = target - position->y;
            position->y += previous;
        } else
            previous = -10000.0f;
    }
}

} // namespace

#else

namespace {

// Native cooldown target-controller callback.
// @verified - p
void AdjustCooldownGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *, void *context) {
    static constexpr shen_name qlbtMap{SHEN_NAME(QLBT)};
    if (g_currentMap.name == qlbtMap.name)
        return;
    std::uint32_t index;
    switch (controller) {
    case 0:
        index = 2;
        break;
    case 1:
        index = 3;
        break;
    case 3:
        index = 5;
        break;
    case 4:
        index = 6;
        break;
    case 5:
        index = 0;
        break;
    case 6:
        index = 1;
        break;
    default:
        return;
    }
    auto *const group = static_cast<FCMG_t *>(context);
    if (group->groundCorrectionEnabled[index] == 0)
        return;
    static constexpr float heights[7]{0.13f, 0.13f, 0.17f, 0.20f, 0.20f, 0.15f, 0.15f};
    const float y = position->y;
    const float bodyLimit = heights[index] + group->position.y;
    const float base = group->stackLayerOffsets[index];
    const float targetLimit = heights[index] + base;
    float &previous = group->groundSamples[index];
    if (bodyLimit < y || y <= targetLimit) {
        if (targetLimit <= y)
            previous = -10000.0f;
        else {
            previous = targetLimit - y;
            position->y += previous;
        }
    } else {
        if (previous < -9999.9f)
            previous = y - targetLimit;
        previous *= 0.5f;
        position->y = previous + base;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void AdjustActiveGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *context) {
    FightActorRuntimePrefix *const actor = static_cast<FightActorRuntimePrefix *>(context);
    int index;
    switch (controller) {
    case 5:
        index = 0;
        break;
    case 6:
        index = 1;
        break;
    case 0:
        index = 2;
        break;
    case 1:
        index = 3;
        break;
    case 2:
        index = 4;
        break;
    case 3:
        index = 5;
        break;
    case 4:
        index = 6;
        break;
    default:
        return;
    }
    if (actor->previousGroundSamples[index] < -9999.9f)
        actor->previousGroundSamples[index] = position->y;
    int query = controller != 6 && controller != 0;
    if (actor->game->type == 8)
        query = 1;
    if (query) {
        const float ground = vfFieldGetIntersection(actor, *position);
        actor->groundHeights[index] = ground + vfAccumEnemyGroundOffsetAndMask(position, &actor->nearbyGroundActors[index]);
    }
    if (actor->game->type == 8)
        return;
    if (actor->hitStateFlags & 0x81d010U) {
        actor->previousGroundSamples[index] = position->y;
        return;
    }
    std::uint32_t category = fightMoveCategoryMasks[actor->moveId];
    switch (static_cast<int>(controller)) {
    case 5: {
        Vector3 otherFoot, pelvis;
        mtwkTransformWithoutIKEnabled(motion, &otherFoot, &pelvis);
        float ground = vfFieldGetIntersection(actor, otherFoot);
        actor->groundHeights[1] = ground + vfAccumEnemyGroundOffsetAndMask(&otherFoot, &actor->nearbyGroundActors[1]);
        ground = vfFieldGetIntersection(actor, actor->contactPosition);
        actor->groundHeights[2] = ground + vfAccumEnemyGroundOffsetAndMask(&actor->contactPosition, &actor->nearbyGroundActors[2]);
        std::uint32_t primary = actor->groundHeights[0] >= actor->groundHeights[1];
        if (actor->motionFlags & 0x100U) {
            if (!primary && (category & 0x1c00U))
                primary = 1;
            else if (category & 0xe000U)
                primary = 0;
        }
        actor->primaryGroundTarget = primary;
        actor->groundHeightDifference = actor->groundHeights[primary] - actor->groundHeights[2];
        if (!(std::fabs(actor->groundHeightDifference) > 0.0025f))
            actor->groundHeightDifference = 0.0025f;
        actor->targetGroundSamples[2] = pelvis.y + actor->groundHeightDifference;
        if (!primary) {
            actor->targetGroundSamples[0] = position->y + actor->groundHeightDifference;
            actor->targetGroundSamples[1] = otherFoot.y + (actor->groundHeights[1] - actor->groundHeights[0]) + actor->groundHeightDifference;
        } else {
            actor->targetGroundSamples[1] = otherFoot.y + actor->groundHeightDifference;
            actor->targetGroundSamples[0] = position->y + (actor->groundHeights[0] - actor->groundHeights[1]) + actor->groundHeightDifference;
        }
        if (actor->stateFlags & 0x80000U)
            for (int slot = 0; slot < 7; ++slot)
                actor->previousGroundSamples[slot] = actor->targetGroundSamples[slot];
        if ((!(actor->motionFlags & 0x100U) || !(category & 0x1c00U)) && std::fabs(actor->targetGroundSamples[0] - position->y) > 0.0025f)
            position->y = (actor->previousGroundSamples[0] + actor->targetGroundSamples[0]) * 0.5f;
        break;
    }
    case 6:
        if ((actor->motionFlags & 0x100U) && (category & 0xe000U))
            break;
    case 0:
        if (std::fabs(actor->targetGroundSamples[index] - position->y) > 0.0025f)
            position->y = (actor->previousGroundSamples[index] + actor->targetGroundSamples[index]) * 0.5f;
        break;
    }
    FightActorRuntimePrefix *const opponent = actor->actor;
    if ((actor->motionFlags & 0x100U) && !(actor->moveFlags & 4U) && !(opponent->secondaryMotionFlags & 0x2000000U)) {
        float limit = 0;
        int apply = 0;
        category = fightMoveCategoryMasks[actor->moveId];
        switch (controller) {
        case 5:
            if (category & 0x1000U) {
                limit = 0.3f;
                apply = 1;
            }
            break;
        case 6:
            if (category & 0x8000U) {
                limit = 0.3f;
                apply = 1;
            }
            break;
        case 3:
            if (category & 0x20U) {
                limit = 0.2f;
                apply = 1;
            }
            break;
        case 4:
            if (category & 0x100U) {
                limit = 0.2f;
                apply = 1;
            }
            break;
        }
        if (apply) {
            float difference = (opponent->targetGroundY - actor->targetGroundY) * actor->groundReactionScale;
            if (difference > limit)
                difference = limit;
            else if (-limit > difference)
                difference = -limit;
            position->y += difference;
        }
    }
    actor->previousGroundSamples[index] = position->y;
}

} // namespace

#else

namespace {

// The actual active-slot ground target callback.
// @verified - p
void AdjustActiveGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *motion, void *context) {
    static constexpr shen_name qlbtMap{SHEN_NAME(QLBT)};
    if (g_currentMap.name == qlbtMap.name || controller > 6)
        return;
    auto *const actor = static_cast<VFActor *>(context);
    static constexpr std::uint8_t indices[7]{2, 3, 4, 5, 6, 0, 1};
    const auto index = indices[controller];
    if (actor->previousGroundSamples[index] < -9999.9f)
        actor->previousGroundSamples[index] = position->y;
    const bool addNearby = static_cast<std::uint32_t>(actor->game->state) < 4U || (actor->moveFlags & VFMF_ForceDefeat) != 0;
    const bool query = controller != 0 && controller != 6;
    if (query || actor->game->type == 8 ||
        (actor->hitStateFlags &
            (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_AlignedToGround)) !=
            0) {
        actor->groundHeights[index] = vfFieldGetIntersection(actor, *position);
        if (addNearby)
            actor->groundHeights[index] += vfAccumEnemyGroundOffsetAndMask(position, &actor->nearbyGroundActors[index]);
    }
    if (actor->game->type == 8)
        return;
    if ((actor->hitStateFlags &
            (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_AlignedToGround)) !=
        0) {
        actor->previousGroundSamples[index] = position->y;
        return;
    }
    const auto category = fightMoveCategoryMasks[actor->moveId];
    if (controller == 5) {
        Vector3 otherFoot, pelvis;
        mtwkTransformWithoutIKEnabled(motion, &otherFoot, &pelvis);
        actor->groundHeights[1] = vfFieldGetIntersection(actor, otherFoot);
        if (addNearby)
            actor->groundHeights[1] += vfAccumEnemyGroundOffsetAndMask(&otherFoot, &actor->nearbyGroundActors[1]);
        actor->groundHeights[2] = vfFieldGetIntersection(actor, actor->contactPosition);
        if (addNearby)
            actor->groundHeights[2] += vfAccumEnemyGroundOffsetAndMask(&actor->contactPosition, &actor->nearbyGroundActors[2]);
        std::uint32_t primary = actor->groundHeights[1] <= actor->groundHeights[0] ? 1U : 0U;
        if ((actor->motionFlags & VFMOF_Unknown100) != 0) {
            if (primary == 0 && (category & 0x1c00U) != 0)
                primary = 1;
            else if ((category & 0xe000U) != 0)
                primary = 0;
        }
        actor->primaryGroundTarget = static_cast<std::int32_t>(primary);
        float difference = actor->groundHeights[primary] - actor->groundHeights[2];
        if (std::fabs(difference) <= 0.0024999999f)
            difference = 0.0024999999f;
        actor->groundHeightDifference = difference;
        actor->targetGroundSamples[2] = pelvis.y + difference;
        if (primary != 0) {
            actor->targetGroundSamples[1] = otherFoot.y + difference;
            actor->targetGroundSamples[0] = (actor->groundHeights[0] - actor->groundHeights[1] + position->y) + difference;
        } else {
            actor->targetGroundSamples[0] = difference + position->y;
            actor->targetGroundSamples[1] = (actor->groundHeights[1] - actor->groundHeights[0] + otherFoot.y) + difference;
        }
        if ((actor->stateFlags & VFSF_Unknown80000) != 0) {
            for (std::uint8_t i = 0; i < 7; ++i)
                actor->previousGroundSamples[i] = actor->targetGroundSamples[i];
        }
        if (((actor->motionFlags & VFMOF_Unknown100) == 0 || (category & 0x1c00U) == 0) &&
            std::fabs(actor->targetGroundSamples[0] - position->y) > 0.0024999999f)
            position->y = (actor->previousGroundSamples[0] + actor->targetGroundSamples[0]) * 0.5f;
    } else if (controller == 0 || (controller == 6 && ((actor->motionFlags & VFMOF_Unknown100) == 0 || (category & 0xe000U) == 0))) {
        if (std::fabs(actor->targetGroundSamples[index] - position->y) > 0.0024999999f)
            position->y = (actor->previousGroundSamples[index] + actor->targetGroundSamples[index]) * 0.5f;
    }
    if ((actor->motionFlags & VFMOF_Unknown100) != 0 && (actor->moveFlags & VFMF_Airborne) == 0 &&
        (actor->actor->secondaryMotionFlags & VFSMF_Unknown2000000) == 0) {
        float limit = 0.0f;
        if ((controller == 3 && (category & 0x20U) != 0) || (controller == 4 && (category & 0x100U) != 0))
            limit = 0.2f;
        else if ((controller == 5 && (category & 0x1000U) != 0) || (controller == 6 && (category & 0x8000U) != 0))
            limit = 0.30000001f;
        if (limit != 0.0f) {
            const float difference = (actor->actor->targetGroundY - actor->targetGroundY) * actor->groundReactionScale;
            const float offset = difference <= limit ? std::fmax(-limit, difference) : limit;
            position->y += offset;
        }
    }
    actor->previousGroundSamples[index] = position->y;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void mtwkRemoveCallback(MotionWork *motion) {
    for (const auto selector : fightMotionSelectors)
        mtwkAssignTargetCallback(motion, nullptr, selector);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float flddGetIntersectionFor(Character *character, Vector3 &position) {
    if (std::fabs(position.y) < 0.00000011920929f)
        position.y = 0.0f;
    Vector3 probe;
    probe.x = position.x;
    probe.y = position.y + 1.0f;
    probe.z = position.z;
    float height = flddFindIntersectionBy(probe);
    if (character)
        for (const CharacterNode *node = GetCharacterNode(character)->parent; node; node = node->parent)
            height += node->position.y;
    return height;
}

#else

// @verified - p
float flddGetIntersectionFor(Character *character, Vector3 &position) {
    if (std::abs(position.y) < 0.00000011920929f)
        position.y = 0.0f;
    const Vector3 probe{position.x, position.y + 1.0f, position.z};
    float height = flddFindIntersectionBy(probe);
    if (character) {
        for (const CharacterNode *node = GetCharacterNode(character)->child; node; node = node->child)
            height += node->position.y;
    }
    return height;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
float flddFindIntersectionBy(const Vector3 &position) {
    flddToggleFlag(2);
    float height = FindFieldIntersectionHeight(&position);
    if (g_currentMap == SHEN_NAME4('Q', 'L', 'B', 'T')) {
        if (height < 111.0f)
            height = 111.0f;
    } else if (static_cast<std::uint32_t>(height) == 0x80000000U) {
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Vector3 playerPosition;
        GetCharacterWorldPosition(control->ryo, &playerPosition);
        height = playerPosition.y;
    }
    return height;
}

#else

// @verified - p
float flddFindIntersectionBy(const Vector3 &position) {
    flddToggleFlag(2);
    const float height = shenFindIntersectionWithScene(position, position.y);
    bool usePlayerHeight;
    if (g_currentMap.name == SHEN_NAME(QLBT)) {
        usePlayerHeight = height < 111.0f;
        if (usePlayerHeight) {
            VFMA_t *const game = vfmaGetTaskParam();
            if (game && game->type == 8)
                return 111.0f;
        }
    } else {
        const std::int64_t converted = std::isfinite(height) && height >= -9223372036854775808.0f && height < 9223372036854775808.0f ?
            static_cast<std::int64_t>(height) :
            std::numeric_limits<std::int64_t>::min();
        usePlayerHeight = height == -std::numeric_limits<float>::max() || static_cast<std::uint32_t>(converted) == 0x80000000U || height < -10.0f;
    }
    if (!usePlayerHeight)
        return height;
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return GetCharacterWorldPosition(static_cast<Character *>(control->ryo)).y;
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void __attribute__((renesas)) shenmue_s2_event_AllocateVOBJ(std::int32_t slot, std::int8_t index) {
    shenmue::s2::AllocateVOBJ(slot, index);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageDefinitionSetMotion(std::int32_t slot, std::uint32_t name, std::int32_t tracks,
    const shenmue::s2::ObjectMotionNode *nodes, std::int32_t step, std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound) {
    shenmue::s2::vfStageDefinitionSetMotion(slot, shenmue::shen_name(name), tracks, nodes, step, firstFrame, lastFrame, sound);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageDefinitionAddHitbox(
    std::int32_t slot, std::uint32_t flags, std::uint16_t kindAndState, float x, float y, float z, float radius) {
    shenmue::s2::vfStageDefinitionAddHitbox(slot, flags, kindAndState, x, y, z, radius);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_ToggleVOBJArrayIndex(
    std::int32_t slot, std::int8_t hitDamage, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound) {
    shenmue::s2::ToggleVOBJArrayIndex(slot, hitDamage, guardState, hitParameter, hitSound);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound,
    std::uint32_t launchSound, std::uint32_t flags, std::int32_t launchYaw, std::int32_t rotationSpeed, float coefficient, float groundOffset) {
    shenmue::s2::vfStageDefinitionSetHitResponse(slot, groundSound, launchSound, flags, launchYaw, rotationSpeed, coefficient, groundOffset);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::int32_t rotationZ) {
    shenmue::s2::vfStageDefinitionSetAttachment(slot, bone, x, y, z, rotationX, rotationY, rotationZ);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageDefinitionAddContactMask(std::int32_t slot, std::uint32_t flags) {
    shenmue::s2::vfStageDefinitionAddContactMask(slot, flags);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfAllocateVOBJ(std::int32_t slot, shenmue::s2::Character *character, std::int32_t definition) {
    shenmue::s2::vfAllocateVOBJ(slot, character, definition);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageObjectSetBrokenModel(
    std::int32_t slot, shenmue::s2::Character *character, std::uint32_t flags) {
    shenmue::s2::vfStageObjectSetBrokenModel(slot, character, flags);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageObjectAssignPosXYZ(std::int32_t slot, float x, float y, float z) {
    shenmue::s2::vfStageObjectAssignPosXYZ(slot, x, y, z);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageObjectAssignRotXYZ(std::int32_t slot, std::uint16_t x, std::uint16_t y, std::uint16_t z) {
    shenmue::s2::vfStageObjectAssignRotXYZ(slot, x, y, z);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageObjectAssignMotFlags(std::int32_t slot, std::uint32_t flags) {
    shenmue::s2::vfStageObjectAssignMotFlags(slot, flags);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageObjectAssignMotFlagsAlternative(std::int32_t slot, std::uint32_t flags) {
    shenmue::s2::g_fightStageObjects[slot]->object.motionFlags |= flags;
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfInitializeStageObjectHitboxes() {
    shenmue::s2::vfInitializeStageObjectHitboxes();
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageObjectReset(std::int32_t slot, shenmue::s2::Character *character) {
    shenmue::s2::vfStageObjectReset(slot, character);
}

extern "C" void __attribute__((renesas)) shenmue_s2_event_vfStageObjectAssignVelocityXYZ(std::int32_t slot, float x, float y, float z) {
    shenmue::s2::vfStageObjectAssignVelocityXYZ(slot, x, y, z);
}

namespace shenmue {
namespace s2 {
namespace {
template <class T> inline T &StageValue(void *owner, std::size_t offset) {
    return *reinterpret_cast<T *>(static_cast<std::byte *>(owner) + offset);
}
} // namespace

// @verified - d
std::int32_t vfCompareFightGuardLevels(const FightActorRuntimePrefix *first, const FightActorRuntimePrefix *second) {
    return static_cast<std::int8_t>(static_cast<std::int8_t>(first->retainedMode) - static_cast<std::int8_t>(second->retainedMode));
}

// @verified - d
std::int8_t vfUpdatePredicate_40_41_44_50_51_54(float, float, std::int32_t) {
    return -1;
}

std::int32_t vfUpdatePredicateZero() {
    return 0;
}

// @verified - d
void vfSelectDefaultReactionAndGuardAnims(FightActorRuntimePrefix *actor) {
    static const std::uint16_t closeIndices[3] = {294, 296, 298};
    static const std::uint16_t farIndices[3] = {300, 302, 304};
    static const std::uint16_t weakIndices[3] = {322, 296, 298};
    static const std::uint16_t indices[3] = {0, 16, 20};
    std::uint8_t index;
    if (actor->game->type == 2 && actor->contactDistances[StageValue<std::int8_t>(actor, 1)] < 4.0f) {
        static const std::uint16_t animations[3] = {152, 118, 119};
        std::uint16_t *const reactions = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::uint8_t *>(actor) + 0x9b6);
        for (index = 0; index < 3; ++index)
            reactions[index] = animations[index];
        return;
    }
    const std::uint16_t *row = indices;
    if (!StageValue<std::uint16_t>(actor, 2)) {
        if (vfCompareFightGuardLevels(actor, actor->actor) > 0) {
            if (actor->hp < static_cast<float>(actor->initialHp) * 0.5f)
                row = weakIndices;
            else
                row = actor->contactDistances[StageValue<std::int8_t>(actor, 1)] < 4.0f ? closeIndices : farIndices;
        }
    } else {
        row = StageValue<std::uint8_t>(actor, 0x780) ? weakIndices : closeIndices;
    }
    for (index = 0; index < 3; ++index) {
        std::uint16_t *const reactions = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::uint8_t *>(actor) + 0x9b6);
        const std::uint16_t offset = row[index];
        const std::uint8_t *const table = StageValue<const std::uint8_t *>(actor, 0x38);
        std::uint16_t *const slot = &reactions[index];
        *slot = static_cast<std::uint16_t>(*(table + offset + 1) | (table[offset] << 8));
    }
    if (actor->stateFlags & 0x80000U)
        actor->stateFlags &= ~0x80000U;
}

// @verified - d
void vfUpdateDefaultReactionAnim(FCMG_t *slot, std::uint16_t animation) {
    MotionWork *const motion = slot->motion;
    FightActorRuntimePrefix *const fighter = slot->fighter;
    mtwkSetFrameTime(motion, 5, static_cast<MotionRegion>(0));
    mtwkRequestMotion(motion, animation, static_cast<MotionRegion>(0));
    if (fighter && slot->state == 2) {
        if (fighter->moveFlags & 0x40U)
            mtwkSetRegionFlag(motion, 69, 0, static_cast<MotionRegion>(0));
        else
            mtwkSetRegionFlag(motion, 69, 2, static_cast<MotionRegion>(0));
    }
    slot->retained21 |= 1U;
}

void vfPrepareEnemySlotMotion(FCMG_t *slot) {
    Character *const character = slot->character;
    MotionWork *const motion = slot->motion;
    std::uint32_t flags = mtwkGetClipMode(motion) & 3U;
    mtwkSetClipMode(motion, 1);
    if (mtwkSetRegionFlag(motion, 68, 1, static_cast<MotionRegion>(0)))
        flags |= 0x10U;
    if (SetCharacterCollisionEnabled(character, 0) == 1)
        flags |= 4U;
    if (charIsMotionworkEnabled(character) == 1)
        flags |= 8U;
    else
        SetCharacterMotionEnabled(character, 1);
    if (mtwkSetExtraFlag(motion, 6, 0))
        flags |= 0x20U;
    if (shenCheckCharaHasAlphaManager(character))
        flags |= 0x80U;
    StageValue<std::uint32_t>(slot, 0x24) = flags;
    StageValue<float>(slot, 0x28) = mtwkGetSpeed(motion, static_cast<MotionRegion>(0));
    mtwkSetSpeed(motion, 1, static_cast<MotionRegion>(0));
    StageValue<std::uint32_t>(slot, 0xd4) = mtwkGetTrackedRootMode(motion);
    mtwkSetRequestedRootMode(motion, 2);
    StageValue<std::uint32_t>(slot, 0xd8) = mtwkGetConstraintAngle(motion);
    mtwkSetConstraintAngle(motion, 0x1000);
    StageValue<std::uint32_t>(slot, 0xdc) = mtwkGetProjectionAngle(motion, 0);
    mtwkSetProjectionAngle(motion, 0xb8d8, 0);
    for (std::int32_t index = 0; index < 7; ++index) {
        const std::int32_t selector = fightMotionSelectors[index];
        StageValue<MotionTargetCallback>(slot, 0xe0 + index * 4) = mtwkGetTargetCallback(motion, selector);
        StageValue<void *>(slot, 0xfc + index * 4) = mtwkGetTargetParameter(motion, selector);
    }
}

void vfRestoreEnemySlotMotion(FCMG_t *slot) {
    Character *const character = slot->character;
    MotionWork *const motion = slot->motion;
    const std::uint32_t flags = StageValue<std::uint32_t>(slot, 0x24);
    mtwkSetClipMode(motion, flags & 3U);
    if (flags & 4U)
        SetCharacterCollisionEnabled(character, 1);
    if (!(flags & 8U))
        SetCharacterMotionEnabled(character, 0);
    if (!(flags & 0x20U))
        mtwkSetExtraFlag(motion, 6, 2);
    if (flags & 0x80U)
        cpamSetFlag(character, 1);
    else
        cpamSetFlag(character, 0);
    mtwkSetSpeed(motion, StageValue<float>(slot, 0x28), static_cast<MotionRegion>(0));
    mtwkSetRequestedRootMode(motion, StageValue<std::uint32_t>(slot, 0xd4));
    mtwkSetConstraintAngle(motion, StageValue<std::int32_t>(slot, 0xd8));
    mtwkSetProjectionAngle(motion, StageValue<std::uint32_t>(slot, 0xdc), 0);
    for (std::int32_t index = 0; index < 7; ++index) {
        const std::int32_t selector = fightMotionSelectors[index];
        mtwkAssignTargetCallback(motion, reinterpret_cast<MotionTargetCallback *>(reinterpret_cast<std::byte *>(slot) + 0xe0)[index], selector);
        mtwkAssignTargetParameter(motion, reinterpret_cast<void **>(reinterpret_cast<std::byte *>(slot) + 0xfc)[index], selector);
    }
}

namespace {
// @verified - d
// @verified - d
bool BindAllocatedEnemySlot(FCMG_t *slot) {
    bool found = false;
    FRMG_t *formation = 0;
    std::uint8_t index;
    for (index = 0; index < g_pFightStage->groupCount; ++index) {
        formation = &g_pFightStage->groups[index];
        if (formation->character == slot->character && !formation->state) {
            found = true;
            break;
        }
    }
    if (!found)
        return false;
    slot->motion = charFindMotionwork(slot->character);
    GetCharacterWorldPosition(slot->character, &slot->position);
    const std::uint16_t yaw = GetCharacterWorldYaw(slot->character);
    StageValue<std::uint16_t>(slot, 0x18) = yaw;
    for (index = 0; index < g_pFightStage->enemyCount; ++index) {
        const FCMG_t &other = g_pFightStage->enemySlots[index];
        if (other.character && &other != slot && std::fabs((other.position.x - slot->position.x) + (other.position.z - slot->position.z)) < 0.00000011920929f) {
            slot->position.x += 0.01f;
            slot->position.z += 0.01f;
            SetCharacterWorldPosition(slot->character, &slot->position);
        }
    }
    vfPrepareEnemySlotMotion(slot);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    mtwkSetExtraFlag(slot->motion, 0, 0);
    slot->group = formation;
    formation->slot = slot;
    --g_pFightStage->secondaryGroupCounters[slot->counterGroup];
    return true;
}

bool ReactToDead(FightRuntimePrefix *game, FCMG_t *slot) {
    bool found = false;
    FRMG_t *formation = 0;
    for (std::uint8_t index = 0; index < g_pFightStage->groupCount; ++index) {
        formation = &g_pFightStage->groups[index];
        if (reinterpret_cast<std::uintptr_t>(formation->character) == SHEN_NAME4('D', 'E', 'A', 'D') && !formation->state &&
            formation->team == slot->counterGroup) {
            found = true;
            break;
        }
    }
    if (!found)
        return false;
    MotionWork *const motion = slot->motion;
    mtwkRemoveCallback(motion);
    mtwkSetExtraFlag(motion, 0, 0);
    mtwkSetExtraFlag(motion, 3, 2);
    formation->character = slot->character;
    std::int16_t yaw;
    if (game->playerSlot != -1 && !formation->activationState) {
        const FightActorRuntimePrefix *const player = game->actors[game->playerSlot];
        const float playerX = player->position.x;
        const float playerZ = player->position.z;
        const float dx = formation->spawnPosition.x;
        const float dz = formation->spawnPosition.z;
        yaw = dc::sdk::ArcTan2Fixed(dx - playerX, dz - playerZ);
    } else
        yaw = formation->spawnWords[1];
    SetCharacterWorldPosition(formation->character, &formation->spawnPosition);
    slot->position.x = formation->spawnPosition.x;
    slot->position.y = formation->spawnPosition.y;
    slot->position.z = formation->spawnPosition.z;
    SetCharacterWorldYaw(formation->character, yaw);
    vfUpdateDefaultReactionAnim(slot, formation->spawnWords[0]);
    mtwkSetFrameTime(motion, 1, static_cast<MotionRegion>(0));
    slot->quadrant = vfUpdatePredicate_40_41_44_50_51_54(slot->position.x, slot->position.z, -1);
    slot->group = formation;
    formation->slot = slot;
    --g_pFightStage->secondaryGroupCounters[slot->counterGroup];
    StageValue<std::uint8_t>(slot, 0xcc) = 0;
    return true;
}

// @verified - d
void vfRemoveCharaFromGroup(FightRuntimePrefix *game, FCMG_t *slot) {
    charaToggleFlag(slot->character, 0);
    game->characters[slot->actorIndex] = 0;
    slot->actorIndex = 0;
}
} // namespace

std::int32_t vfFightStageZeroNullsub() {
    return 0;
}

static std::int32_t vfIsGeneratorClearOfEnemies(float x, float z);

void vfAssignWaveSpawnFromGenerator(FRMG_t *formation, float x, float z) {
    std::uint8_t primary, outward;
    float primaryDistance, outwardDistance;
    outward = primary = 99;
    primaryDistance = outwardDistance = 999.0f;
    const std::uint8_t useCenterBias = g_pFightStage->useCenterBias;
    const float centerX = x - g_pFightStage->battleCenter.x;
    const float centerZ = z - g_pFightStage->battleCenter.z;
    const float distanceFromCenter = NativeSqrt(centerX * centerX + centerZ * centerZ);
    for (std::uint8_t index = 0; index < g_FGEN_count; ++index) {
        FGEN_t *const generator = &g_pFightStage->generators[index];
        if (generator->enabled == 1 && generator->other == 0) {
            const float dx = x - generator->position.x;
            const float dz = z - generator->position.z;
            const float distance = NativeSqrt(dx * dx + dz * dz);
            if (distance > 8.0f) {
                if (useCenterBias) {
                    const float gx = generator->position.x - g_pFightStage->battleCenter.x;
                    const float gz = generator->position.z - g_pFightStage->battleCenter.z;
                    if (NativeSqrt(gx * gx + gz * gz) > distanceFromCenter) {
                        const float error = std::fabs(distance - 8.0f);
                        if (outwardDistance > error && vfIsGeneratorClearOfEnemies(generator->position.x, generator->position.z) == 1) {
                            outwardDistance = error;
                            outward = index;
                        }
                        continue;
                    }
                }
                const float error = std::fabs(distance - 8.0f);
                if (primaryDistance > error && vfIsGeneratorClearOfEnemies(generator->position.x, generator->position.z) == 1) {
                    primaryDistance = error;
                    primary = index;
                }
            }
        }
    }
    if (primary == 99)
        primary = outward;
    if (primary != 99) {
        const FGEN_t &selected = g_pFightStage->generators[primary];
        formation->spawnPosition.x = selected.position.x;
        formation->spawnPosition.y = selected.position.y;
        formation->spawnPosition.z = selected.position.z;
    }
}

static std::int32_t vfIsGeneratorClearOfEnemies(float x, float z) {
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        Character *character = g_pFightStage->enemySlots[index].character;
        if (character) {
            Vector3 position;
            GetCharacterWorldPosition(character, &position);
            const float dx = x - position.x;
            const float dz = z - position.z;
            if (4.0f > dx * dx + dz * dz)
                return 0;
        }
    }
    return 1;
}

bool vfEnemySlotResetAndRestartCycle(FightRuntimePrefix *game, FCMG_t *slot) {
    const std::uint8_t state = slot->state;
    const std::uint8_t team = slot->counterGroup;
    switch (state) {
    case 0:
    case 4:
        if (!BindAllocatedEnemySlot(slot))
            return false;
        break;
    case 1:
        return false;
    case 2: {
        if (slot->runningTicks <= 29)
            return false;
        FightActorRuntimePrefix *const fighter = slot->fighter;
        if (slot->group)
            slot->group->totalHp = static_cast<std::uint16_t>(static_cast<std::int32_t>(fighter->hp));
        GetCharacterWorldPosition(slot->character, &slot->position);
        const std::uint16_t yaw = GetCharacterWorldYaw(slot->character);
        StageValue<std::uint16_t>(slot, 0x18) = yaw;
        vfSelectDefaultReactionAndGuardAnims(fighter);
        vfUpdateDefaultReactionAnim(slot, StageValue<std::uint16_t>(fighter, 0x9b6));
        mtwkRemoveCallback(slot->motion);
        vfRemoveCharaFromGroup(game, slot);
        slot->group->state = 0;
        g_activeFightData[slot->fightEntryIndex].status = 0;
        --g_pFightStage->activeFightersByTeam[team];
        break;
    }
    case 3:
        if (!ReactToDead(game, slot))
            return false;
        --g_pFightStage->groupCounters[team];
        slot->fadeAlpha = -1;
        SetCharacterAlpha(slot->character, 0);
        break;
    default:
        break;
    }
    slot->quadrant = vfUpdatePredicate_40_41_44_50_51_54(slot->position.x, slot->position.z, -1);
    slot->state = 1;
    slot->runningTicks = 0;
    ++g_pFightStage->readyEnemySlots[team];
    return true;
}

// @verified - d
std::uint8_t vfRegisterContactSlot(FightRuntimePrefix *game, FCMG_t *slot) {
    Character *const character = slot->character;
    FRMG_t *formation;
    FightActorRuntimePrefix *actor;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const std::uint8_t index = vfFindCharaIndex(game, 0);
#else
    const std::uint32_t index = vfFindCharaIndex(game, 0);
#endif
    if (index == 99)
        return 99;
    const std::uint8_t team = slot->counterGroup;
    switch (slot->state) {
    case 0:
    case 4:
        if (!BindAllocatedEnemySlot(slot))
            return 99;
        break;
    case 1:
        if (slot->group && slot->group->activationState)
            return 99;
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2:
        return 99;
    case 3:
        if (!ReactToDead(game, slot))
            return 99;
        --g_pFightStage->groupCounters[team];
        break;
    default:
        break;
    }
    formation = slot->group;
    game->characters[index] = character;
    game->formations[index] = formation;
    game->enemySlots[index] = slot;
    slot->state = 2;
    slot->runningTicks = 0;
    formation->state = 1;
    g_activeFightData[slot->fightEntryIndex].status = 1;
    game->actors[index] = actor = slot->fighter;
    slot->actorIndex = index;
    game->actorSlots[index] = actor->slot;
    StageValue<std::uint8_t>(actor, 0) = index;
    *reinterpret_cast<Character **>(reinterpret_cast<std::byte *>(actor) + 0x9c0) = 0;
    StageValue<Character *>(actor, 0x10) = 0;
    *reinterpret_cast<FightActorRuntimePrefix **>(reinterpret_cast<std::byte *>(actor) + 0x9c4) = 0;
    actor->actor = 0;
    StageValue<std::int8_t>(actor, 1) = *reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x9be) = -1;
    GetCharacterWorldPosition(character, &slot->position);
    actor->position.x = actor->contactPosition.x = slot->position.x;
    actor->position.y = actor->contactPosition.y = slot->position.y;
    actor->position.z = actor->contactPosition.z = slot->position.z;
    StageValue<std::uint16_t>(actor, 2) = formation->style;
    actor->hitStateFlags = actor->activeMotionFlags = 0;
    actor->moveFlags = 0;
    actor->motionFlags = actor->secondaryMotionFlags = 0;
    mtwkRegisterGroundTargetCallbacks(slot->motion, actor);
    game->slotTeams[index] = formation->team;
    game->slotPoolModes[index] = formation->poolMode;
    if (!game->slotPoolModes[index])
        game->playerSlot = index;
    ++g_pFightStage->activeFightersByTeam[team];
    return index;
}

namespace {
std::uint16_t FightAuthoredWord(const std::byte *data, std::uint32_t offset) {
    return static_cast<std::uint16_t>((static_cast<std::uint8_t>(data[offset]) << 8) | static_cast<std::uint8_t>(data[offset + 1]));
}
float FightAuthoredFixed(const std::byte *data, std::uint32_t offset) {
    const std::uint32_t value = (static_cast<std::uint32_t>(FightAuthoredWord(data, offset)) << 16) | FightAuthoredWord(data, offset + 2);
    return static_cast<float>(static_cast<std::int32_t>(value)) * 0.0000152587890625f;
}
const std::byte *FightPhaseProfile(FightActorRuntimePrefix *actor) {
    const std::byte *const profile = actor->motionParameters + 0x1a;
    std::uint8_t phase = StageValue<std::uint8_t>(actor, 0x780);
    std::uint8_t &selected = phase;
    if (selected > 2)
        selected = 2;
    return profile + selected * 9;
}
struct FightPhaseState {
    std::uint8_t active, current, remaining, reversed, steps[16];
};

// @verified - d
void SelectFightPhase(std::uint8_t *phase) {
    phase[1] = reinterpret_cast<FightPhaseState *>(phase)->steps[dc::sdk::RandomInt() & 15];
    if (phase[1] > 1)
        --phase[1];
    std::int32_t random = dc::sdk::RandomInt();
    phase[2] = random % (phase[1] + 1);
}
// @verified - d
void InitializeFightPhase(std::uint8_t *phase, std::uint8_t value) {
    if (!value) {
        phase[0] = 0;
        return;
    }
    phase[0] = 1;
    float divisor;
    if (static_cast<std::int8_t>(value) >= 0) {
        phase[3] = 0;
        divisor = value;
    } else {
        phase[3] = 1;
        divisor = 256 - value;
    }
    const float count = 255.0f / divisor;
    const std::uint8_t whole = static_cast<std::uint8_t>(static_cast<std::int32_t>(count));
    for (std::uint8_t index = 0; index < 16; ++index)
        reinterpret_cast<FightPhaseState *>(phase)->steps[index] = whole;
    float remainder = count - whole;
    std::uint8_t index = 0;
    while (remainder >= 0.0625f) {
        ++reinterpret_cast<FightPhaseState *>(phase)->steps[index++];
        remainder -= 0.0625f;
    }
    SelectFightPhase(phase);
}
} // namespace

// @verified - d
void vfInitializeSlotPhaseConfigurations(FightActorRuntimePrefix *actor) {
    std::uint8_t *const slot = reinterpret_cast<std::uint8_t *>(actor->slot);
    const std::byte *const profile = FightPhaseProfile(actor);
    InitializeFightPhase(slot + 0xfd, static_cast<std::uint8_t>(profile[1]));
    InitializeFightPhase(slot + 0x111, static_cast<std::uint8_t>(profile[2]));
    InitializeFightPhase(slot + 0x125, static_cast<std::uint8_t>(profile[3]));
    InitializeFightPhase(slot + 0x139, static_cast<std::uint8_t>(profile[4]));
    InitializeFightPhase(slot + 0x14d, static_cast<std::uint8_t>(profile[6]));
    InitializeFightPhase(slot + 0x161, static_cast<std::uint8_t>(profile[8]));
}

// @verified - d
void vfUpdateGuardLevel(FightRuntimePrefix *game, std::uint8_t index) {
    FightActorRuntimePrefix *const actor = game->actors[index];
    if (!actor || !game->actorSlots[index])
        return;
    void *const slot = game->actorSlots[index];
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    StageValue<shen_name>(slot, 0) = scenGetCharaName(control->scenarioTask, actor->character);
    StageValue<FightActorRuntimePrefix *>(slot, 4) = actor;
    StageValue<std::int8_t>(slot, 0x5a) = -1;
    std::memset(static_cast<std::byte *>(slot) + 0x5c, 0, 0x11c);
    StageValue<std::uint8_t>(slot, 0x54) = StageValue<std::uint8_t>(slot, 0x55) = StageValue<std::uint8_t>(slot, 0x56) = 127;
    StageValue<std::uint16_t>(slot, 0xb0) = 240;
    StageValue<std::uint32_t>(slot, 0x2c) = 1;
    const std::byte *const parameters = actor->motionParameters;
    StageValue<float>(slot, 0xc) = StageValue<float>(slot, 0x28) =
        static_cast<float>(
            static_cast<std::int32_t>(static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[7]) | (static_cast<std::int8_t>(parameters[6]) * 0x100)) |
                (static_cast<std::uint32_t>(
                     static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[5]) | (static_cast<std::int8_t>(parameters[4]) * 0x100)))
                    << 16))) *
        0.0000152587890625f;
    StageValue<float>(slot, 0x10) =
        static_cast<float>(static_cast<std::int32_t>(
            static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[11]) | (static_cast<std::int8_t>(parameters[10]) * 0x100)) |
            (static_cast<std::uint32_t>(
                 static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[9]) | (static_cast<std::int8_t>(parameters[8]) * 0x100)))
                << 16))) *
        0.0000152587890625f;
    StageValue<std::uint16_t>(slot, 0x14) =
        static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[13]) | (static_cast<std::int8_t>(parameters[12]) * 0x100));
    StageValue<std::uint16_t>(slot, 0x16) =
        static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[15]) | (static_cast<std::int8_t>(parameters[14]) * 0x100));
    StageValue<std::uint16_t>(slot, 0x18) =
        static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[17]) | (static_cast<std::int8_t>(parameters[16]) * 0x100));
    StageValue<std::uint16_t>(slot, 0x1a) =
        static_cast<std::uint16_t>(static_cast<std::uint8_t>(parameters[19]) | (static_cast<std::int8_t>(parameters[18]) * 0x100));
    StageValue<std::int16_t>(slot, 0x1c) = static_cast<std::int8_t>(parameters[20]);
    actor->retainedMode = static_cast<std::uint8_t>(parameters[21]);
    StageValue<float>(actor, 0x9e0) = 1.0f / static_cast<float>(static_cast<std::int8_t>(parameters[22]));
    StageValue<std::int8_t>(slot, 0x175) = static_cast<std::int8_t>(parameters[23]);
    std::uint32_t *const counters = &StageValue<std::uint32_t>(slot, 0x17c);
    counters[2] = counters[1] = 0;
    if (static_cast<std::uint8_t>(parameters[25]) & 1U)
        actor->moveFlags |= 0x40000U;
    vfInitializeSlotPhaseConfigurations(actor);
}

// @verified - d
std::uint8_t vfSelectOpponentSlotByPriority(FightRuntimePrefix *game, std::uint8_t minimum) {
    float furthest = 0.1f;
    std::uint8_t priority = minimum;
    std::uint8_t selected = 99;
    for (std::uint8_t index = 0; index < 6; ++index) {
        FightActorRuntimePrefix *const actor = game->actors[index];
        if (!game->characters[index] || index == game->playerSlot || game->slotTeams[index] == game->slotTeams[game->playerSlot])
            continue;
        if (game->enemySlots[index]->runningTicks <= 14 || (actor->moveFlags & 0x20U) || ((actor->hitStateFlags | actor->motionFlags) & 0xc5010U) ||
            ((actor->activeMotionFlags | actor->secondaryMotionFlags) & 0x800c00U))
            continue;
        const std::uint8_t candidate = static_cast<std::uint8_t>(actor->retained940[4]);
        if (candidate > priority) {
            priority = candidate;
            selected = index;
        } else if (candidate == priority) {
            const float distance = actor->contactDistances[game->playerSlot];
            if (distance > furthest) {
                priority = candidate;
                selected = index;
                furthest = distance;
            }
        }
    }
    return selected;
}

// @verified - d
bool vfTransitionEnemySlotToCooldown(FightRuntimePrefix *game, FCMG_t *slot) {
    const std::uint8_t state = slot->state, team = slot->counterGroup;
    MotionWork *const motion = slot->motion;
    GetCharacterWorldPosition(slot->character, &slot->position);
    mtwkRemoveCallback(motion);
    switch (state) {
    case 0:
    case 4:
        break;
    case 1:
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2: {
        FightActorRuntimePrefix *const actor = slot->fighter;
        std::uint32_t masks = 0;
        for (std::uint8_t index = 0; index < 7; ++index) {
            slot->stackLayerOffsets[index] = actor->groundHeights[index];
            if (std::fabs(slot->position.y - slot->stackLayerOffsets[index]) > 0.001f)
                slot->groundCorrectionEnabled[index] = 1;
            else
                slot->groundCorrectionEnabled[index] = 0;
            (&StageValue<std::uint32_t>(slot, 0x50))[index] = (&StageValue<std::uint32_t>(actor, 0x8c0))[index];
            masks |= (&StageValue<std::uint32_t>(actor, 0x8c0))[index];
        }
        (&slot->collisionFlags)[1] = masks;
        mtwkRegisterCooldownTargetCallbacks(motion, slot);
        StageValue<std::uint16_t>(slot, 0xac) = slot->fighter->facingAngleDelta;
        slot->quadrant = static_cast<std::int8_t>(slot->fighter->retained946[0x9a5 - 0x946]);
        for (std::uint8_t index = 0; index < 7; ++index)
            slot->groundSamples[index] = -10000.0f;
        std::uint32_t flags = 0;
        if (actor->hitStateFlags & 0x4000U) {
            flags = 1;
            if (actor->motionFlags & 0x4000000U) {
                if (!(actor->motionFlags & 0x800U))
                    flags |= 0x20U;
                else
                    flags |= 0x40U;
            } else if (actor->hitStateFlags & 0x800U)
                flags |= 2;
        }
        if (actor->moveFlags & 0x40U)
            flags |= 0x80U;
        if (actor->hitStateFlags & 0x800000U)
            flags |= 0x200U;
        if (slot->group->selectFlags & 0xa0U)
            flags |= 0x100U;
        slot->collisionFlags = flags;
        vfRemoveCharaFromGroup(game, slot);
        --g_pFightStage->activeFightersByTeam[team];
        break;
    }
    case 3:
        return false;
    }
    slot->state = 3;
    slot->runningTicks = 0;
    slot->group->state = 2;
    g_activeFightData[slot->fightEntryIndex].status = 2;
    StageValue<std::uint8_t>(slot, 0xab) = g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1];
    slot->fadeAlpha = 1;
    mtwkSetExtraFlag(slot->motion, 0, 2);
    ++g_pFightStage->groupCounters[team];
    return true;
}

// @verified - d
bool vfTransitionEnemySlotToInactive(FightRuntimePrefix *game, FCMG_t *slot) {
    const std::uint8_t state = slot->state;
    const std::uint8_t team = slot->counterGroup;
    switch (state) {
    case 0:
    case 4:
        return false;
    case 1:
        --g_pFightStage->readyEnemySlots[team];
        break;
    case 2:
        vfRemoveCharaFromGroup(game, slot);
        --g_pFightStage->activeFightersByTeam[team];
        break;
    case 3:
        --g_pFightStage->groupCounters[team];
        break;
    default:
        break;
    }
    vfRestoreEnemySlotMotion(slot);
    MotionWork *const motion = slot->motion;
    mtwkSetExtraFlag(motion, 0, 0);
    mtwkSetExtraFlag(motion, 3, 2);
    slot->state = 4;
    slot->runningTicks = 0;
    std::uint8_t *counters = g_pFightStage->groupCounters + 2;
    ++counters[team];
    return true;
}

// @verified - d
void RemoveFightGroundSlotReference(std::uint32_t index) {
    const std::uint32_t mask = 1U << static_cast<std::uint8_t>(index);
    for (std::uint8_t other = 0; other < g_pFightStage->enemyCount; ++other) {
        FCMG_t &slot = g_pFightStage->enemySlots[other];
        if (!slot.character || slot.state != 3 || !((&slot.collisionFlags)[1] & mask))
            continue;
        for (std::uint8_t sample = 0; sample < 7; ++sample) {
            std::uint32_t *const references = &slot.collisionFlags + 2;
            std::uint32_t &reference = references[sample];
            if (!(reference & mask))
                continue;
            reference &= ~mask;
            slot.stackLayerOffsets[sample] -= 0.15000001f;
            if (sample == 2) {
                slot.position.y -= 0.15000001f;
                SetCharacterWorldPosition(slot.character, &slot.position);
            }
        }
        (&slot.collisionFlags)[1] &= ~mask;
    }
}

// @verified - d
bool SetupContactSlot(FightRuntimePrefix *game, FRMG_t *group) {
    const std::uint8_t index = vfRegisterContactSlot(game, group->slot);
    if (index == 99)
        return false;
    vfUpdatePairingsAndDistances(game);
    vfInitializeFightActor(game, index);
    vfUpdateGuardLevel(game, index);
    return true;
}

namespace {

// @verified - d
void ActivateBlockedFormations(FightRuntimePrefix *game, FCMG_t *retired) {
    if (g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1] == 6)
        return;
    for (std::uint8_t index = 0; index < g_pFightStage->groupCount; ++index) {
        FRMG_t &group = g_pFightStage->groups[index];
        if (group.state || !group.character || reinterpret_cast<std::uintptr_t>(group.character) == FIGHT_DEAD || group.slot->state != 1 ||
            group.blocker != retired->character || group.team != retired->counterGroup)
            continue;
        const std::uint8_t other = group.team ^ 1;
        if (g_pFightStage->activeFightersByTeam[other] || g_pFightStage->secondaryGroupCounters[other])
            SetupContactSlot(game, &group);
    }
}

// @verified - d
std::int32_t FightSpawnRelocationPredicate(std::int32_t, std::int32_t) {
    return 0;
}

void TryRelocateFightWave(FightRuntimePrefix *game, FCMG_t *slot, FRMG_t *group) {
    if (game->playerSlot != -1 && (group->selectFlags & 0x80U)) {
        const std::int8_t quadrant = vfUpdatePredicate_40_41_44_50_51_54(slot->position.x, slot->position.z, slot->quadrant);
        FightActorRuntimePrefix *player = game->actors[game->playerSlot];
        const std::int32_t relocate = FightSpawnRelocationPredicate(StageValue<std::int8_t>(player, 0x9a5), quadrant);
        if (relocate == 1) {
            vfAssignWaveSpawnFromGenerator(group, player->position.x, player->position.z);
            SetCharacterWorldPosition(group->character, &group->spawnPosition);
            slot->position.x = group->spawnPosition.x;
            slot->position.z = group->spawnPosition.z;
            SetCharacterWorldYaw(
                group->character, dc::sdk::ArcTan2Fixed(group->spawnPosition.x - player->position.x, group->spawnPosition.z - player->position.z));
            StageValue<std::uint8_t>(slot, 0xcc) = 0;
            slot->fadeAlpha = 0.0f;
            SetCharacterAlpha(slot->character, 0.0f);
            cpamSetFlag(slot->character, relocate);
        }
    }
}

// @verified - d
void UpdateFightSpawnAlpha(FCMG_t *slot) {
    if (!(slot->fadeAlpha < 1))
        return;
    float alpha = slot->fadeAlpha + 0.03f;
    if (alpha > 1) {
        cpamSetFlag(slot->character, 1);
        alpha = 1;
    }
    slot->fadeAlpha = alpha;
    if (alpha < 0)
        alpha = 0;
    SetCharacterAlpha(slot->character, alpha);
}
} // namespace

// @verified - d
void vfUpdateEnemySlotCooldowns(FightRuntimePrefix *game) {
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (!slot.character || slot.state != 3)
            continue;
        if (!StageValue<std::uint8_t>(&slot, 0xab)) {
            if (!(slot.collisionFlags & 0x100U)) {
                cpamSetFlag(slot.character, 1);
                vfTransitionEnemySlotToInactive(game, &slot);
            } else {
                SetCharacterAlpha(slot.character, 1);
                cpamSetFlag(slot.character, 1);
                vfEnemySlotResetAndRestartCycle(game, &slot);
            }
            RemoveFightGroundSlotReference(index);
        } else {
            --StageValue<std::uint8_t>(&slot, 0xab);
        }
    }
}

// @verified - d
void vfUpdateRenderOpacityForAll(FightRuntimePrefix *game) {
    if (static_cast<std::uint8_t>(g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1]) != game->maximumActiveEnemies)
        return;
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (!slot.character || slot.state != 3 || static_cast<std::uint8_t>(slot.retainedab[0]))
            continue;
        cpamSetFlag(slot.character, 0);
        const std::uint8_t team = slot.counterGroup;
        if (!g_pFightStage->secondaryGroupCounters[team] || !(slot.collisionFlags & 0x100U))
            return;
        slot.fadeAlpha -= 0.03f;
        if (!(slot.fadeAlpha > 0)) {
            slot.fadeAlpha = 0;
            SetCharacterAlpha(slot.character, 0);
            vfUpdateEnemySlotCooldowns(game);
        } else
            SetCharacterAlpha(slot.character, slot.fadeAlpha);
        return;
    }
}

// @verified - d
void UpdateDefeatAndRespawn(FightRuntimePrefix *game) {
    for (std::uint8_t index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[index];
        if (std::fabs(actor->hp) < 0.00000011920929f && !(actor->moveFlags & 0x20U)) {
            actor->moveFlags |= 0x20U;
            if (g_pFightStage->battleFlags & 0x8000U) {
                if (StageValue<std::int8_t>(actor, 0) == game->playerSlot)
                    vfAddRoundResults(0, 1);
                else
                    vfAddRoundResults(1, 0);
            }
        }
        if ((actor->moveFlags & 0x24U) != 0x20U || !mtwkSetRegionFlag(actor->motion, 0x41, 1, static_cast<MotionRegion>(0)))
            continue;
        FCMG_t *const slot = game->enemySlots[index];
        vfTransitionEnemySlotToCooldown(game, slot);
        UpdateEnemySlotCooldownsForDefeat(game);
        ActivateBlockedFormations(game, slot);
    }
}

// @verified - d
void vfUpdateEnemySlotSpawnFade(FightRuntimePrefix *game) {
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (!slot.character || !slot.group || slot.state != 1)
            continue;
        GetCharacterWorldPosition(slot.character, &slot.position);
        FRMG_t *const group = slot.group;
        switch (group->activationState) {
        case 0:
            TryRelocateFightWave(game, &slot, group);
            break;
        case 1:
        case 2:
            break;
        }
        UpdateFightSpawnAlpha(&slot);
        *reinterpret_cast<std::uint16_t *>(slot.retained18) = GetCharacterWorldYaw(slot.character);
    }
}

void vfUpdateEnemySlotFormationAndYaw(FightRuntimePrefix *) {
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t &slot = g_pFightStage->enemySlots[index];
        if (slot.character && slot.group && slot.state == 3 && (slot.collisionFlags & 1U) != 0 &&
            mtwkSetRegionFlag(slot.motion, 0x41, 1, static_cast<MotionRegion>(0)) && !mtwkIsMotionNonLooping(slot.motion, 0)) {
            if (slot.collisionFlags & 0x200U) {
                slot.position.y -= 0.5f;
                SetCharacterWorldPosition(slot.character, &slot.position);
                slot.collisionFlags &= ~0x200U;
            }
            std::uint16_t animation;
            if (slot.collisionFlags & 0x20U)
                animation = 453;
            else if (slot.collisionFlags & 0x40U)
                animation = 452;
            else if (slot.collisionFlags & 2U)
                animation = 101;
            else
                animation = 104;
            vfUpdateDefaultReactionAnim(&slot, animation);
            if (slot.collisionFlags & 0x80U)
                mtwkSetRegionFlag(slot.motion, 0x45, 0, static_cast<MotionRegion>(0));
            else
                mtwkSetRegionFlag(slot.motion, 0x45, 2, static_cast<MotionRegion>(0));
            std::int16_t &yaw = StageValue<std::int16_t>(&slot, 0xac);
            if (yaw) {
                SetCharacterWorldYaw(slot.character, GetCharacterWorldYaw(slot.character) + yaw);
                yaw = 0;
            }
        }
    }
}

// @verified - d
void UpdateEnemySlotActivation(FightRuntimePrefix *game) {
    std::uint8_t counts[2], normal[2];
    Vector3 position;
    std::uint8_t index, selected;
    bool replace, forced;
    std::uint8_t active = g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1];
    if (active > 4) {
        while (active > 4) {
            selected = vfSelectOpponentSlotByPriority(game, 0);
            if (selected == 99)
                break;
            vfEnemySlotResetAndRestartCycle(game, game->enemySlots[selected]);
            --active;
        }
    }
    counts[0] = counts[1] = normal[0] = normal[1] = 0;
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index] || (game->actors[index]->moveFlags & 0x20U))
            continue;
        const std::uint8_t team = game->slotTeams[index];
        ++counts[team];
        if (!(game->actors[index]->activeMotionFlags & 0x800U))
            ++normal[team];
    }
    for (index = 0; index < g_pFightStage->groupCount; ++index) {
        FRMG_t *const group = &g_pFightStage->groups[index];
        const std::uint8_t other = group->team ^ 1;
        if ((!g_pFightStage->activeFightersByTeam[other] && !g_pFightStage->secondaryGroupCounters[other]) || group->state || !group->character ||
            reinterpret_cast<std::uintptr_t>(group->character) == SHEN_NAME(DEAD) || group->blocker || group->slot->state != 1 || group->activationState)
            continue;
        replace = false;
        forced = false;
        if (group->waveFlags & 1U)
            forced = active < 4;
        if (!forced && (game->type == 1 || game->type == 5) && !(group->selectFlags & 0xa0U) && active < 4)
            forced = true;
        if (!forced && game->playerSlot != -1) {
            float radius;
            if (group->waveFlags & 8U)
                radius = 10;
            else if (group->waveFlags & 4U)
                radius = 7;
            else if (group->waveFlags & 2U)
                radius = 5;
            else
                radius = 2.5f;
            GetCharacterWorldPosition(group->character, &position);
            const Vector3 &player = game->actors[game->playerSlot]->position;
            const float playerX = player.x, playerZ = player.z;
            const float x = position.x - playerX, z = position.z - playerZ;
            const float distance = std::sqrt(x * x + z * z);
            if (distance < radius) {
                forced = true;
                if (distance < 2.5f)
                    replace = true;
            }
        }
        if ((group->waveFlags & 0x800U) && !normal[group->team])
            forced = true;
        if (forced != true)
            continue;
        if (static_cast<std::uint8_t>(g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1]) > 3) {
            if (!replace)
                continue;
            selected = vfSelectOpponentSlotByPriority(game, static_cast<std::uint8_t>(group->selectFlags));
            if (selected == 99) {
                if (g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1] == 6)
                    continue;
            } else
                vfEnemySlotResetAndRestartCycle(game, game->enemySlots[selected]);
        }
        if (SetupContactSlot(game, group) == true)
            ++counts[group->team];
    }
    g_pFightStage->unknownGroupCounts[0] = counts[0];
    g_pFightStage->unknownGroupCounts[1] = counts[1];
}

#define FIGHT_FIELD(T, owner, offset) (*reinterpret_cast<T *>(reinterpret_cast<std::byte *>(owner) + (offset)))
void vfUpdateContactSlotsAndGroundFlags(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *game;
    FightActorRuntimePrefix *opponent, *paired;
    std::int32_t withinTen, nearOpponent;
    std::uint8_t team;
    std::uint32_t selfHit, opponentHit, pairedHit, ground, reaction;
    std::int32_t action, pairedIndex, selfAngle, otherAngle;
    float distance;
    std::uint8_t index;
    Vector3 position, delta;
    game = actor->game;
    team = game->slotTeams[actor->groupIndex];
    action = FIGHT_FIELD(std::int8_t, actor, 1);
    pairedIndex = FIGHT_FIELD(std::int8_t, actor, 0x9be);
    if (action == -1 || pairedIndex == -1)
        return;
    opponent = game->actors[action];
    paired = game->actors[pairedIndex];
    selfHit = actor->hitStateFlags;
    opponentHit = opponent->hitStateFlags;
    pairedHit = paired->hitStateFlags;
    position.x = actor->contactPosition.x;
    position.z = actor->contactPosition.z;
    reaction = 0;
    ground = 0;
    withinTen = 0;
    nearOpponent = game->state != 3;
    for (index = 0; index < 6; ++index) {
        FightActorRuntimePrefix *other;
        float length = 0;
        if (!game->characters[index])
            actor->contactDistances[index] = length;
        else {
            other = game->actors[index];
            const float dx = position.x - other->contactPosition.x, dz = position.z - other->contactPosition.z;
            length = shenmue::NativeSqrt(dx * dx + dz * dz);
            actor->contactDistances[index] = length;
            if (game->slotTeams[index] != team) {
                if (length < 10)
                    withinTen = 1;
                if (length < 7)
                    nearOpponent = 1;
            }
        }
    }
    if (nearOpponent == 1)
        ground |= 4U;
    distance = actor->contactDistances[pairedIndex];
    if (FIGHT_FIELD(std::int8_t, game, 0x22c) == 1 && !FIGHT_FIELD(std::uint16_t, actor, 2) && !withinTen) {
        vfTryFightDialogue(game, 7);
        vfTryFightDialogue(game, 8);
    }
    for (index = 0; index < 6; ++index) {
        std::uint32_t angle = 0;
        if (game->characters[index]) {
            FightActorRuntimePrefix *const other = game->actors[index];
            angle = dc::sdk::ArcTan2Fixed(position.x - other->contactPosition.x, position.z - other->contactPosition.z);
        }
        actor->actorFacingAngles[index] = angle;
    }
    selfAngle = static_cast<std::int16_t>(actor->temporaryFacingAngle);
    selfAngle -= actor->actorFacingAngles[pairedIndex];
    otherAngle = static_cast<std::int16_t>(paired->temporaryFacingAngle) - actor->actorFacingAngles[pairedIndex];
    delta.x = FIGHT_FIELD(float, actor, 0x4c) - FIGHT_FIELD(float, opponent, 0x4c);
    delta.z = FIGHT_FIELD(float, actor, 0x54) - FIGHT_FIELD(float, opponent, 0x54);
    vfRotateFightVector(-static_cast<std::int16_t>(opponent->facingAngle), &delta, &FIGHT_FIELD(Vector3, actor, 0x58));
    if ((actor->actorFacingAngles[action] - static_cast<std::int16_t>(actor->temporaryFacingAngle) + 0x4000) & 0x8000U)
        ground |= 1U;
    if (!((actor->actorFacingAngles[action] - static_cast<std::int16_t>(opponent->temporaryFacingAngle) + 0x4000) & 0x8000U))
        ground |= 2U;
    if ((actor->moveFlags & 0x40000000U) && !(selfHit & 0x10U) && (opponentHit & 0x8d8110U) == 0x10000U && !(opponent->moveFlags & 0x20U) &&
        static_cast<std::uint16_t>(selfAngle + 0x1ffe) <= 0x3ffc && static_cast<std::int16_t>(otherAngle ^ 0x8000) >= -0x1ffe &&
        static_cast<std::int16_t>(otherAngle ^ 0x8000) <= 0x1ffe && !(distance > 3.0f) && (FIGHT_FIELD(std::uint8_t, paired, 0x39d) & 1U))
        reaction |= 0x40U;
    std::int32_t guardEligible = 1;
    if (paired->hitStateFlags & 0x4000U) {
        if (paired->activeMotionFlags & 0x100000U)
            guardEligible = 1;
        else
            guardEligible = 0;
    }
    if ((actor->moveFlags & 0x40000000U) && !(selfHit & 0x8dc100U) && !(actor->activeMotionFlags & 0x800020U) && !(pairedHit & 0x8d8110U) &&
        !(paired->activeMotionFlags & 0x800020U) && paired->hp > 0 && guardEligible && FIGHT_FIELD(std::uint16_t, paired, 2) != 1 &&
        !(FIGHT_FIELD(const std::byte *, paired, 0x30)[25] & 2U) && (FIGHT_FIELD(std::uint16_t, paired, 0x42c) & 0xfffU) != 59 &&
        (FIGHT_FIELD(std::uint16_t, paired, 0x42c) & 0xfffU) != 58) {
        if (static_cast<std::uint16_t>((selfAngle ^ 0x8000) + 0x1ffe) <= 0x3ffc && static_cast<std::uint16_t>((otherAngle ^ 0x8000) + 0x1ffe) <= 0x3ffc &&
            !(selfHit & 0x10U) && !(distance > FIGHT_FIELD(float, actor, 0x414)))
            reaction |= 2U;
        else if (static_cast<std::uint16_t>(selfAngle + 0x1ffe) <= 0x3ffc) {
            if (static_cast<std::uint16_t>(otherAngle + 0x1ffe) <= 0x3ffc && !(selfHit & 0x10U) && !(distance > FIGHT_FIELD(float, actor, 0x414))) {
                if (paired->moveFlags & 0x100U)
                    reaction |= 0x10U;
                else
                    reaction |= 8U;
            } else if (static_cast<std::int16_t>(otherAngle ^ 0x8000) >= -0x1ffe && static_cast<std::int16_t>(otherAngle ^ 0x8000) <= 0x1ffe &&
                !(distance > FIGHT_FIELD(float, actor, 0x40c))) {
                if ((paired->moveFlags & 0x100U) && !(selfHit & 0x10U))
                    reaction |= 4U;
                else if (selfHit & 0x10U)
                    reaction |= 0x400000U;
                else
                    reaction |= 1U;
            } else if (static_cast<std::uint16_t>(otherAngle - 0x2002) <= 0x3ffc && !(selfHit & 0x10U) && !(distance > FIGHT_FIELD(float, actor, 0x418))) {
                if (paired->moveFlags & 0x100U)
                    reaction |= 0x20000000U;
                else
                    reaction |= 0x4000000U;
            } else if (static_cast<std::uint16_t>(otherAngle + 0x5ffe) <= 0x3ffc && !(selfHit & 0x10U) && !(distance > FIGHT_FIELD(float, actor, 0x418))) {
                if (paired->moveFlags & 0x100U)
                    reaction |= 0x10000000U;
                else
                    reaction |= 0x2000000U;
            }
        }
    }
    std::int32_t opponentGuard;
    if (opponent->moveFlags & 0x20000000U)
        opponentGuard = 1;
    else if (opponentHit & 0x8100U)
        opponentGuard = 0;
    else if (opponentHit & 0x10000U)
        opponentGuard = 1;
    else if (!(opponentHit & 0x4000U))
        opponentGuard = 0;
    else if (!(opponentHit & 0x10U))
        opponentGuard = 1;
    else
        opponentGuard = !FIGHT_FIELD(std::int16_t, actor, 0x3be);
    if (opponentGuard == 1)
        ground |= 0x20U;
    std::uint32_t mask = 0;
    if ((actor->moveFlags & 0x40000000U) && (pairedHit & 0x102U) == 0x102U && static_cast<std::uint16_t>(selfAngle + 0x1554) <= 0x2aa8 &&
        static_cast<std::int16_t>(otherAngle ^ 0x8000) >= -0x1554 && static_cast<std::int16_t>(otherAngle ^ 0x8000) <= 0x1554) {
        mask = 0x7fff00U;
        if (distance > 2.0f)
            mask &= 0xfff33fffU;
        if (distance > 1.8f)
            mask &= 0xffafd0ffU;
        if (distance > 1.6f)
            mask &= 0xffffefffU;
        mask &= FIGHT_FIELD(std::uint32_t, opponent, 0x2c8);
    }
    actor->hitFlags = mask;
    std::int32_t hasPriority = 1;
    if ((FIGHT_FIELD(std::uint32_t, actor, 0x360) & 1U) == 0)
        hasPriority = (FIGHT_FIELD(std::uint32_t, actor, 0x360) = FIGHT_FIELD(std::uint32_t, paired, 0x2f8)) != 0;
    if (hasPriority == 1 && static_cast<float>(FIGHT_FIELD(std::uint16_t, paired, 0x306)) < opponent->animationTime) {
        FIGHT_FIELD(std::uint32_t, actor, 0x360) = 0;
        FIGHT_FIELD(std::uint32_t, paired, 0x2f8) = 0;
    }
    std::uint32_t orientation = 0;
    if (FIGHT_FIELD(std::uint32_t, opponent, 0x2c8) & 1U)
        orientation = 0x48U;
    if (actor->moveFlags & 0x400U)
        orientation ^= 8U;
    if (opponent->moveFlags & 0x400U)
        orientation ^= 0x40U;
    ground |= orientation;
    std::uint32_t difference = selfHit ^ opponentHit;
    if (difference & 0x200000U)
        difference = 0x40;
    if ((actor->moveFlags ^ opponent->moveFlags ^ difference) & 0x40U)
        ground |= 0x10U;
    if (!(FIGHT_FIELD(float, actor, 0x3ac) > 2.5f))
        reaction |= 0x100U;
    if (!(FIGHT_FIELD(float, actor, 0x3b0) > 1.5f))
        reaction |= 0x200U;
    actor->groundContactFlags = ground;
    actor->reactionFlags = reaction;
    if ((actor->moveFlags & 0x40000000U) && FIGHT_FIELD(std::uint8_t, actor, 0x32b) == 14)
        vfDidCrossFightFrame(actor, static_cast<float>(FIGHT_FIELD(std::uint16_t, actor, 0x292)) - FIGHT_FIELD(float, actor, 0x26c));
}
#undef FIGHT_FIELD

namespace {
// @verified - d
void CaptureFightHitTransform(Character *character, std::uint8_t bone, Matrix4x4 *output) {
    MotionWork *const motion = charFindMotionwork(character);
    matrix_stack::Matrix4x4_Push();
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::MultiplyWithPrePassMatrix(mtwkGetBoneTransform(motion, bone));
    matrix_stack::GetMatrix(output);
    PopCurrentMatrix(1);
}

// @verified - d
void ClearFightTransformTranslation(Matrix4x4 *transform) {
    transform->rows[3].x = transform->rows[3].y = transform->rows[3].z = 0;
}

// @verified - d
void TransformFightBasisPair(const Vector3 *input, Vector3 *output) {
    Matrix4x4 saved;
    matrix_stack::GetMatrix(&saved);
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(&saved);
    Matrix4x4::TransformPoint(input[0], output[0]);
    matrix_stack::GetMatrix(&saved);
    PopCurrentMatrix(1);
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(&saved);
    Matrix4x4::TransformPoint(input[1], output[1]);
    matrix_stack::GetMatrix(&saved);
    PopCurrentMatrix(1);
}

// @verified - d
std::uint32_t FightFootFacingAngle(const Vector3 *basis) {
    Vector3 result[2];
    TransformFightBasisPair(basis, result);
    if (0.2f < result[0].y)
        return dc::sdk::ArcTan2Fixed(-result[1].x, result[1].y) + 0x8000U;
    if (result[0].y < -0.2f)
        return dc::sdk::ArcTan2Fixed(-result[1].x, result[1].y);
    return dc::sdk::ArcTan2Fixed(result[0].x, result[0].z) + 0x8000U;
}

// @verified - d
float MinimumFightMotionHeight(FightActorRuntimePrefix *actor) {
    float minimum = 32000;
    for (std::int8_t index = 0; index < 10; ++index) {
        if ((&StageValue<float>(actor, 0x78))[index * 3] < minimum)
            minimum = (&StageValue<float>(actor, 0x78))[index * 3];
    }
    return minimum;
}

std::int16_t PredictFightKnockbackFrames(FightActorRuntimePrefix *actor, float height) {
    FightRuntimePrefix *game = actor->game;
    if (!(actor->moveFlags & 4U))
        return 0;
    float acceleration, y, velocity;
    acceleration = StageValue<float>(actor, 0x26c) * StageValue<float>(actor, 0x26c) * game->gravityScale;
    acceleration -= StageValue<float>(actor, 0x464);
    y = actor->contactPosition.y;
    velocity = StageValue<float>(actor, 0x458);
    std::int16_t count = 0;
    while (y += velocity, velocity -= acceleration, !(y < height && velocity < 0))
        count = static_cast<std::int16_t>(count + 1);
    return count;
}
} // namespace

void vfRefreshActorGameInfo(FightActorRuntimePrefix *actor) {
    vfUpdateHitFacingAndKnockback(actor);
    vfUpdateContactSlotsAndGroundFlags(actor);
    vfSelectDefaultReactionAndGuardAnims(actor);
}

const Vector3 fightChestBasis[2] = {{0, -1, 0}, {1, 0, 0}};
const Vector3 fightRightFootBasis[2] = {{0, 0, -1}, {0, 1, 0}};
const Vector3 fightLeftFootBasis[2] = {{0, 0, -1}, {0, 1, 0}};

#define FIGHT_FIELD(T, owner, offset) (*reinterpret_cast<T *>(reinterpret_cast<std::byte *>(owner) + (offset)))
// @verified - d
void vfUpdateHitFacingAndKnockback(FightActorRuntimePrefix *actor) {
    Character *const character = actor->character;
    FightRuntimePrefix *const game = actor->game;
    MotionWork *const motion = actor->motion;
    std::int32_t angle;
    std::uint32_t halfTurn;
    float value, dot;
    FIGHT_FIELD(float, actor, 0x140) = actor->contactPosition.x;
    FIGHT_FIELD(float, actor, 0x144) = actor->contactPosition.y;
    FIGHT_FIELD(float, actor, 0x148) = actor->contactPosition.z;
    mtwkGetBonePosition(motion, static_cast<MotionBoneId>(0), &actor->contactPosition);
    mtwkGetBonePosition(motion, static_cast<MotionBoneId>(3), &FIGHT_FIELD(Vector3, actor, 0xf8));
    mtwkGetBonePosition(motion, static_cast<MotionBoneId>(5), &FIGHT_FIELD(Vector3, actor, 0x104));
    mtwkGetBonePosition(motion, static_cast<MotionBoneId>(14), &FIGHT_FIELD(Vector3, actor, 0x110));
    mtwkGetBonePosition(motion, static_cast<MotionBoneId>(21), &FIGHT_FIELD(Vector3, actor, 0x11c));
    mtwkGetBonePosition(motion, static_cast<MotionBoneId>(26), &FIGHT_FIELD(Vector3, actor, 0x128));
    mtwkGetBonePosition(motion, static_cast<MotionBoneId>(31), &FIGHT_FIELD(Vector3, actor, 0x134));
    actor->groundClearance = FIGHT_FIELD(float, actor, 0x108) - (actor->targetGroundY + 1.5f);
    if (actor->hitStateFlags & 0x40U)
        actor->temporaryFacingAngle = actor->facingAngle + actor->facingAngleDelta;
    else
        actor->temporaryFacingAngle = actor->facingAngle;
    if (FIGHT_FIELD(std::uint8_t, actor, 0x288) == 4) {
        FIGHT_FIELD(std::uint8_t, actor, 0x288) = 0;
        if (actor->hitStateFlags & 0x8000U) {
            actor->hitStateFlags &= ~0x8000U;
            game->retained44 = static_cast<std::int32_t>(FIGHT_FIELD(float, actor, 0x270) * 6);
        }
    }
    FIGHT_FIELD(std::uint16_t, actor, 0x3e0) = FIGHT_FIELD(std::uint16_t, actor, 0x3de);
    const std::uint32_t hitFlags = actor->hitStateFlags;
    const std::int16_t facingAngle = actor->facingAngle;
    if (!(hitFlags & 0x14040U))
        actor->facingYaw = facingAngle;
    else {
        matrix_stack::Matrix4x4_Push();
        CaptureFightHitTransform(character, 3, &FIGHT_FIELD(Matrix4x4, actor, 0x184));
        ClearFightTransformTranslation(&FIGHT_FIELD(Matrix4x4, actor, 0x184));
        matrix_stack::LoadMatrix(&FIGHT_FIELD(Matrix4x4, actor, 0x184));
        TransformFightBasisPair(fightChestBasis, &FIGHT_FIELD(Vector3, actor, 0x244));
        CaptureFightHitTransform(character, 31, &FIGHT_FIELD(Matrix4x4, actor, 0x204));
        ClearFightTransformTranslation(&FIGHT_FIELD(Matrix4x4, actor, 0x204));
        matrix_stack::LoadMatrix(&FIGHT_FIELD(Matrix4x4, actor, 0x204));
        FIGHT_FIELD(std::uint16_t, actor, 0x25c) = FightFootFacingAngle(fightRightFootBasis);
        CaptureFightHitTransform(character, 26, &FIGHT_FIELD(Matrix4x4, actor, 0x1c4));
        ClearFightTransformTranslation(&FIGHT_FIELD(Matrix4x4, actor, 0x1c4));
        matrix_stack::LoadMatrix(&FIGHT_FIELD(Matrix4x4, actor, 0x1c4));
        FIGHT_FIELD(std::uint16_t, actor, 0x25e) = FightFootFacingAngle(fightLeftFootBasis);
        PopCurrentMatrix(1);
        if (actor->hitStateFlags & 0x14000U) {
            value = actor->motionFlags & 0x800U ? 0.4f : -0.4f;
            if (!(value < FIGHT_FIELD(float, actor, 0x248)))
                actor->hitStateFlags |= 0x800U;
            else
                actor->hitStateFlags &= ~0x800U;
        }
        Vector3 left, right, root;
        charaGetMotLocalPos(character, 26, &left);
        charaGetMotLocalPos(character, 31, &right);
        charaGetMotLocalPos(character, 0, &root);
        const float dx = right.x - left.x, dz = right.z - left.z;
        FIGHT_FIELD(float, actor, 0x390) = ::sqrtf(dx * dx + dz * dz);
        FIGHT_FIELD(std::uint32_t, actor, 0x434) = 0;
        value = std::fabs(right.y - root.y);
        if (!(0.4f < value))
            FIGHT_FIELD(std::uint32_t, actor, 0x434) = 1;
        value = std::fabs(left.y - root.y);
        if (!(0.4f < value))
            FIGHT_FIELD(std::uint32_t, actor, 0x434) |= 2U;
        FIGHT_FIELD(float, actor, 0x394) = std::fabs(right.y - left.y);
        if (FIGHT_FIELD(std::uint32_t, actor, 0x434) == 0 || (FIGHT_FIELD(std::uint32_t, actor, 0x434) < 3 && !(actor->hitStateFlags & 0x14000U))) {
            if (!(FIGHT_FIELD(float, actor, 0x394) < 0.4f) && 0.4f < FIGHT_FIELD(float, actor, 0x390)) {
                FIGHT_FIELD(std::uint8_t, actor, 0x39c) = 0;
                if (left.y < right.y)
                    angle = dc::sdk::ArcTan2Fixed(left.x - right.x, left.z - right.z);
                else
                    angle = dc::sdk::ArcTan2Fixed(right.x - left.x, right.z - left.z);
            } else {
                left.x = (left.x + right.x) * 0.5f;
                left.z = (left.z + right.z) * 0.5f;
                angle = dc::sdk::ArcTan2Fixed(root.x - left.x, root.z - left.z);
                FIGHT_FIELD(std::uint8_t, actor, 0x39c) = 1;
                halfTurn = 0;
                if (FIGHT_FIELD(float, actor, 0x248) < 0)
                    halfTurn = 0x8000U;
                dot = FIGHT_FIELD(float, actor, 0x250) * (left.x - root.x) + FIGHT_FIELD(float, actor, 0x258) * (left.z - root.z);
                if (!(dot < 0))
                    halfTurn ^= 0x8000U;
                if (actor->hitStateFlags & 0x14000U) {
                    if (halfTurn & 0x8000U)
                        actor->hitStateFlags |= 0x800U;
                    else
                        actor->hitStateFlags &= ~0x800U;
                }
                angle += halfTurn;
            }
            actor->facingYaw = angle;
        } else {
            if (FIGHT_FIELD(std::uint32_t, actor, 0x434) < 3) {
                if (FIGHT_FIELD(std::uint32_t, actor, 0x434) == 1) {
                    left.x = right.x;
                    left.z = right.z;
                }
            } else {
                left.x = (left.x + right.x) * 0.5f;
                left.z = (left.z + right.z) * 0.5f;
            }
            angle = dc::sdk::ArcTan2Fixed(root.x - left.x, root.z - left.z);
            FIGHT_FIELD(std::uint8_t, actor, 0x39c) = 2;
            halfTurn = 0;
            if (FIGHT_FIELD(float, actor, 0x248) < 0)
                halfTurn = 0x8000U;
            dot = FIGHT_FIELD(float, actor, 0x250) * (left.x - root.x) + FIGHT_FIELD(float, actor, 0x258) * (left.z - root.z);
            if (!(dot < 0))
                halfTurn ^= 0x8000U;
            if (actor->hitStateFlags & 0x14000U) {
                if (halfTurn & 0x8000U)
                    actor->hitStateFlags |= 0x800U;
                else
                    actor->hitStateFlags &= ~0x800U;
            }
            actor->facingYaw = angle + halfTurn;
        }
    }
    if (actor->hitStateFlags & 0x200000U) {
        if (actor->moveFlags & 0x40U)
            actor->moveFlags &= ~0x400U;
        else
            actor->moveFlags |= 0x400U;
    } else if (actor->moveFlags & 0x40U)
        actor->moveFlags |= 0x400U;
    else
        actor->moveFlags &= ~0x400U;
    FIGHT_FIELD(float, actor, 0x398) = MinimumFightMotionHeight(actor);
    std::uint8_t type = game->type;
    if (type == 5)
        type = 1;
    if (type != 1 || FIGHT_FIELD(std::int8_t, actor, 0) == actor->game->playerSlot) {
        angle = static_cast<std::int16_t>(actor->temporaryFacingAngle);
        FIGHT_FIELD(float, actor, 0x3b4) = FightIntersectionDistance(actor, angle + 0x3000, 5);
        FIGHT_FIELD(float, actor, 0x3b8) = FightIntersectionDistance(actor, angle + 0xd000, 5);
        FIGHT_FIELD(float, actor, 0x3ac) = FightIntersectionDistance(actor, angle, 10);
        FIGHT_FIELD(std::uint16_t, actor, 0x180) = g_vfIntersectionTagSource;
        FIGHT_FIELD(float, actor, 0x3b0) = FightIntersectionDistance(actor, angle + 0x8000, 10);
        FIGHT_FIELD(std::uint16_t, actor, 0x182) = g_vfIntersectionTagSource;
    }
    angle = camCtrlGetNextRotY();
    halfTurn = static_cast<std::int16_t>(actor->temporaryFacingAngle) - angle;
    if (halfTurn & 0x8000U)
        actor->moveFlags &= ~2U;
    else
        actor->moveFlags |= 2U;
    const std::uint32_t flags = actor->hitStateFlags;
    const std::uint8_t guardKind = FIGHT_FIELD(std::uint8_t, actor, 0x2a9);
    const std::uint32_t guardBits = FIGHT_FIELD(std::uint16_t, actor, 0x2b0);
    std::uint32_t guard = 0;
    if (flags & 0x100U) {
        switch (guardKind) {
        case 0:
            guard |= 4U;
        case 1:
        case 3:
        case 4:
            if (!(guardBits & 0x200U))
                guard |= 1U;
            break;
        case 2:
        case 5:
        case 6:
            guard |= 8U;
            if (!(guardBits & 0x200U))
                guard |= 2U;
            break;
        }
        if (!guard)
            guard = 0x8000U;
    }
    FIGHT_FIELD(std::uint32_t, actor, 0x364) = guard;
    if (game->controlFlags & 0x20U)
        return;
    if ((actor->hitStateFlags & 0x20000800U) == 0x20000800U)
        FIGHT_FIELD(std::uint16_t, actor, 0x3bc) = 30;
    else if (FIGHT_FIELD(std::uint16_t, actor, 0x3bc))
        --FIGHT_FIELD(std::uint16_t, actor, 0x3bc);
    if (!(actor->hitStateFlags & 8U)) {
        if (!(FIGHT_FIELD(std::uint16_t, actor, 0x298) & 0x80U) || actor->animationTime > static_cast<float>(FIGHT_FIELD(std::uint16_t, actor, 0x28c)) * 0.5f)
            actor->moveFlags &= ~0x100U;
        else
            actor->moveFlags |= 0x100U;
    } else if (!(FIGHT_FIELD(std::uint16_t, actor, 0x298) & 0x80U) &&
        !(actor->animationTime > static_cast<float>(FIGHT_FIELD(std::uint16_t, actor, 0x28c)) * 0.5f))
        actor->moveFlags &= ~0x100U;
    else
        actor->moveFlags |= 0x100U;
    value = FIGHT_FIELD(float, actor, 0x2a4) + FIGHT_FIELD(float, actor, 0x400);
    FIGHT_FIELD(std::int16_t, actor, 0x3be) = PredictFightKnockbackFrames(actor, value);
    actor->retained166 = actor->facingAngle;
    actor->retained338 = actor->hitStateFlags;
}
#undef FIGHT_FIELD

// @verified - d
void vfRefreshContactReactionState(FightActorRuntimePrefix *actor) {
    vfUpdateHitFacingAndKnockback(actor);
    vfUpdateContactSlotsAndGroundFlags(actor);
    vfSelectDefaultReactionAndGuardAnims(actor);
}

// @verified - d
bool vfgGameInfoEntry(HLib::Task *source, HLib::Task *target) {
    if (!source || !target || target->taskData)
        return false;
    target->taskData = source->taskData;
    source->taskData = 0;
    return true;
}

// @verified - d
void vfg_Callback_GameInfoEntry_callback(void *parameter) {
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(parameter);
    std::uint8_t index;
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        vfUpdateHitFacingAndKnockback(game->actors[index]);
        vfUpdateContactSlotsAndGroundFlags(game->actors[index]);
    }
    for (index = 0; index < 6; ++index)
        if (game->characters[index])
            vfSelectDefaultReactionAndGuardAnims(game->actors[index]);
    HLib::TransferTaskData(game->gameInfoTask, game->task);
}

// @verified - d
HLib::Task *vfg_Callback_GameInfoEntry_Enqueue(FightRuntimePrefix *game) {
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(vfg_Callback_GameInfoEntry_callback, 12, 10, SHEN_NAME(VFGI));
    HLib::AssignTaskDataToTask(task, game);
    return task;
}

} // namespace s2
} // namespace shenmue
#else
#endif
