#pragma once
#include "s1/task/scen.hpp"
#include "s1/task/vfma.hpp"
#include "s1/input.hpp"
#include "s1/motion.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/time.hpp"
#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/lighting.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/btpk.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/char.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/vfcl.hpp"
#include "s1/task/vf_core.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/tmex.hpp"
#include "s1/subtitles.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <type_traits>

namespace shenmue::s1 {
namespace vf_detail {}
using namespace vf_detail;

extern VFFightStage *g_pFightStage;

namespace vf_detail {

inline constexpr std::uint32_t kActorToken = SHEN_NAME(FGTM);
inline constexpr std::uint32_t kSlotToken = SHEN_NAME(FENE);
inline constexpr std::size_t kNativeActorAllocationSize = 0xb08;
extern std::array<std::array<std::uint8_t, 20>, 3> g_vfMoveAnimTypeLut;

struct VFContactVolumeRecord {
    Vector3 offset;
    float radius;
    std::uint8_t node;
    std::uint8_t reserved;
    std::uint16_t mask;
};
extern const std::array<VFContactVolumeRecord, 10> kVFContactVolumes;

enum VFState : std::int32_t {
    VFS_Init = 0,
    VFS_AttachAndFlag = 1,
    VFS_SetupParticipants = 2,
    VFS_InBattle = 3,
    VFS_EvaluateResult = 4,
    VFS_ResultTimeline = 5,
    VFS_PostResultSetup = 6,
    VFS_WaitSettle = 7,
    VFS_PostBattleDelay = 8,
    VFS_WaitRetryDialog = 9,
    VFS_Teardown = 10,
};

enum VFSlotState : std::uint8_t {
    SS_Allocated = 0,
    SS_Ready = 1,
    SS_Active = 2,
    SS_Cooldown = 3,
    SS_Inactive = 4,
};

enum VFBattleFlags : std::uint32_t {
    PlayerAggro = 0x1,
    FreeBattleExitPrompt = 0x2,
    EnemyAggro = 0x4,
    FreeBattleIntroActive = 0x8,
    FreeBattleExitActive = 0x10,
    BattleCamUiLock = 0x20,
    PadInputOverride = 0x40,
    ExitConfirmPending = 0x80,
    SequenceCooldown = 0x100,
    DisableRankUpdate = 0x200,
};
extern std::array<VOBJ *, 8> g_pVOBJ;

extern std::array<VFStageObject *, 20> g_vfStageObjs;

extern VFStageObject *g_vfObjectRoot;

extern VFMAData *g_pFightMain;

extern std::int32_t g_vfShouldLoad70mbAssets;

extern float g_maxDist;

extern std::uint16_t g_vfConfirmButtonMask;

extern std::uint8_t g_fgenNumAnchors;

extern std::uint8_t g_frmgFormationIndex;

extern std::array<VFRegistryEntry, 16> g_vfFightRegistry;

extern Character *const kVFFormationSpawnCharacter;

const std::uint32_t *Words(void *params);

void SetResult(ScenarioContext *context, std::uint32_t value);

void SetResult(ScenarioContext *context, float value);

template <typename T> T *ScenarioPointer(std::uint32_t offset) {
    return offset && g_pCurrentTaskData ? reinterpret_cast<T *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(offset))) : nullptr;
}
Character *FindCharacter(ScenarioContext *context, std::uint32_t id);

void AllocateFCMG(std::uint8_t count);

void AllocateFRMG(std::uint8_t count);

void AllocateFCEL(std::int8_t count);

void AllocateFGEN(std::uint8_t count);

void vfSelectDefaultReactionAndGuardAnims(VFActor &actor);

void vfRegisterAnimSetMoveUsage(std::uint8_t fighterId);

void vfUnlockFreeBattleMove(VFMAData &game, std::uint8_t fighterId);

const std::byte *FindDynaEntry(std::uint32_t id);

std::uint16_t ReadPackedU16(const void *value);

void UpdateRepeatedReactionHistory(VFActor &actor);

bool ApplyPendingHitReactionMotion(VFActor &actor, bool guardRealignYaw = false);

std::uint16_t NextVFFrameEventRandomWord();

void vfHandleImpactSfxAndDefeat(VFActor &self, std::int16_t damage, std::uint8_t attackerSelectMode);

void ResetStageObjectMotion(VFStageObject &object);

void vfStageObjectApplyVelocity(VFActor &actor);

void vfStageObjectResetAndActivateForChara(std::int32_t index, Character *character);

std::int32_t vfPredictKnockbackFramesToZ(const VFActor &actor, float targetY);

void vfSetPairedFighter(VFMAData &game, std::uint8_t selfId, std::uint8_t pairedFighterId);

void vfUpdatePairActionState(VFActor &self, const VFActor &other);

void vfUpdatePairedFighterState(VFActor &self, VFActor &pairedFighter);

void vfPreUpdateAction(VFActor &actor);

void vfDispatchQueuedReactionEvent(VFActor &actor);

void vfSelectHitReactionSequence(VFActor &actor, std::uint8_t index);

void UpdateVFQueuedActionLifecycle(VFActor &actor);

void vfclUpdatePairingFacing(VFActor &actor);

void UpdateVFKnockbackPhysics(VFActor &actor);

void UpdateVFAutoAdvanceLanding(VFActor &actor);

void vfUpdateMotionTargetPointAndParams(VFActor &actor);

void vfUpdateAnimFlagsAndEvents(VFActor &actor);

void vfUpdateAirborneAnimationTiming(VFActor &actor);

bool vfAssignVoiceFor(VFMAData &game, std::uint8_t voiceCode);

bool vfmaAssignVoiceReactForFrame(VFMAData &game, std::uint8_t voiceCode);

extern const std::array<std::uint16_t, 8> g_vfCameraSectors;

extern const std::array<std::uint16_t, 16> g_vfCameraYawAngles;

std::uint8_t AdvanceVFCameraPhaseChain(VFActor &actor);

bool vfStageCheckPointInQuadrantXZ(float x, float z, std::int8_t quadrant);

std::uint8_t vfStageResolveQuadrantForPointXZ(float x, float z, std::uint8_t quadrant);



struct VFStyleTuningBE {
    std::uint8_t camera_phase_table_off_be[4];
    std::uint8_t assist_window_base_fx16_be[4];
    std::uint8_t assist_window_guard_fx16_be[4];
    std::uint8_t assist_angle_limit_be[2];
    std::uint8_t unk_0e_be[2];
    std::uint8_t special_timer_init_be[2];
    std::uint8_t phase_arg_init;
    std::uint8_t base_guard_level;
    std::uint8_t camera_phase_choice_count;
    std::uint8_t style_flags;
    std::uint8_t hp_phase_cfg[14];
    std::uint8_t camera_phase_pool_24[3][8];
    std::uint8_t camera_phase_pool_3c[3][8];
    std::uint8_t phase_select_pool_54[4];
    std::uint8_t phase_select_pool_58[4];
    std::uint8_t phase_select_pool_5c[4];
    std::uint8_t phase_select_pool_60[2];
    std::uint8_t phase_select_pool_62[2];
    std::uint8_t reserved_64_69[6];
    std::uint8_t phase_select_pool_6a[4];
    std::uint8_t phase_select_pool_6e;
    std::uint8_t reserved_6f_72[4];
    std::uint8_t react_state02_end_pool_near[2];
    std::uint8_t react_state02_end_pool_far[2];
    std::uint8_t reaction_prog_header_77_7b[5];
    std::uint8_t reaction_prog_preset_blob[338];
};
void vfResetSfxTimers(VFMAData &game);

using VFMainCallback = std::uint8_t (*)(VFActor *, VFSlotRuntimeState *);
struct VFMainCallbackPair {
    VFMainCallback select;
    VFMainCallback publish;
};
extern const VFMainCallbackPair kVFMainCallbacks[58];

enum class VFCollisionCounterEvent : std::uint8_t {
    ContactRegistered,
    HitConfirmed,
    HitSequenceEnded,
};
void vfmaUpdateCollisionCounter(VFMAData &game, VFCollisionCounterEvent event, std::uint16_t fighterId);

VFStateMachineResult RunReactionStateMachine(VFActor &actor);

extern const std::array<std::uint32_t, 44> kVFMoveCategoryMasks;

float VF_GetLocalActorAnimScale(const VFActor &actor);

void vfSetupAreaMaskTable();

inline constexpr std::uint32_t kVFMirrorReactionAngle = 0x40U;
std::uint16_t vfAccumulatedYaw(Character *character);

void vfUpdateDefaultReactionAnim(VFEnemySlot *group, std::uint16_t animationId);

bool vfBindEnemySlotToFormationEntry(VFEnemySlot *group);

bool vfTransitionEnemySlotToInactive(VFMAData *game, VFEnemySlot *group);

void vfApplyEnemySlotAxisOffsetDecay(std::int8_t releasedSlot);

float vfAccumEnemyGroundOffsetAndMask(const Vector3 &position, std::int32_t &nearbyEnemies);

std::uint8_t vfStageFindQuadrantForPointXZ(float x, float z);

bool vfAttachCharaToFormationSlot(VFMAData *game, VFEnemySlot *group);

bool vfEnemySlotResetAndRestartCycle(VFMAData *game, VFEnemySlot *group);

bool vfTransitionEnemySlotToCooldown(VFMAData *game, VFEnemySlot *group);

void vfmaRecycleDefeatedEnemySlots(VFMAData *game);

void vfUpdateRenderOpacityForAll(VFMAData *game);

std::uint8_t vfPickOpponentSlotByPrioRank(VFMAData *game, std::uint8_t rank);

std::uint8_t vfRegisterContactSlot(VFMAData *game, VFEnemySlot *group);

MotionNodeRuntime *vfFindEffectorRow(void *rows, std::uint8_t nodeId);

void vfclResolveContactPoseConstraints(MotionInformationWork *motion, void *user);

void vfUpdateContactEffectorHeightConstraints_Callback(MotionInformationWork *motion, void *user);

void vfResetActorReactionStateForContact(VFMAData *game, std::uint8_t index);

void vfUpdateGuardLevel(VFMAData *game, std::uint8_t index);

void vfAssignNearestOpponents(VFMAData *game);

void vfHandleAggroStateChange(VFMAData *game, VFEnemySlot *group, VFFormationGroup *formation);

void vfUpdateEnemySlotFormationAndYaw(VFMAData *game);

bool vfRegisterFormationSlotForBattle(Character *character);

} // namespace vf_detail

VFMAData *vfMainGetCurrent();

extern std::int32_t g_VFForcePlayerGodmode;

bool VFCL_CheckConnectionGraph(VFActor &attacker, VFActor &defender);

float VFCL_ResolveImpact(VFActor &attacker, VFActor &defender, float baseImpact);

void VFCL_PlayImmediateHitCue(float impact, const VFActor &attacker);

extern std::uint32_t g_vfHitActionContext;

void vfAssignJumpIfNeeded(const VFActor &attacker, const VFActor &defender);

std::uint16_t VFCL_SelectReactionAnimation(VFActor &attacker, const VFActor &defender, std::uint32_t selector);

void VFCL_SetAction(VFActionDescriptor &action, std::uint16_t animation, std::uint8_t state);

bool VFCL_PublishSpecialCounter(VFActor &attacker, VFActor &defender);

void VFCL_ApplyImpact(VFActor &attacker, VFActor &defender);

namespace vf_detail {

void vfSetNearbyFighterXZ(VFEnemySlot &fighter, float x, float z);

void vfclMoveEntityWithContactHistory(VFCLTaskData &context, VFActor &actor, float x, float z);

const std::uint8_t *StageObjectCollisionMask();

} // namespace vf_detail

void vfFreeStageObjects();

namespace vf_detail {

enum class FightCommandOpcode : std::uint32_t {
    SelectAltA = 3,
    SelectAltB = 4,
    SelectContinue = 5,
    End = 6,
    InputMask = 7,
    Window = 8,
    BlendSpeedA = 13,
    BlendSpeedB = 14,
    ForceEnd = 15,
};
} // namespace vf_detail

} // namespace shenmue::s1

namespace shenmue::s1::vf_detail {

constexpr std::uint8_t kVFContactEffectorNodes[7]{0x21, 0x1a, 0x00, 0x03, 0x06, 0x11, 0x0b};

constexpr float kVFContactEffectorHeights[7]{0.13f, 0.13f, 0.17f, 0.2f, 0.2f, 0.15f, 0.15f};

struct VFContactConfig {
    std::uint32_t maskA;
    std::uint32_t maskB;
    float slideScale;
    float stickScale;
};

constexpr std::array<VFContactConfig, 7> kVFContactConfigurations{{
    {0xffffffffU, 0xffffffffU, 0.5f, 0.5f},
    {0xffffffffU, 0xffffffffU, 0.0f, 1.0f},
    {0xffffffffU, 0xffffffffU, 1.0f, 0.0f},
    {0x202U, 0x202U, 0.0f, 1.0f},
    {0x202U, 0x202U, 1.0f, 0.0f},
    {0xffffffffU, 0xffffffffU, 0.0f, 1.0f},
    {0xffffffffU, 0xffffffffU, 1.0f, 0.0f},
}};

} // namespace shenmue::s1::vf_detail

namespace shenmue::s1 {

using namespace vf_detail;

struct VFKnockbackInfo {
    float baseScale;
    float reserved;
    float groundScale;
    float counterGroundScale;
    float fallScale;
    float counterFallScale;
    std::uint16_t groundAngle;
    std::uint16_t counterGroundAngle;
    std::uint16_t fallAngle;
    std::uint16_t counterFallAngle;
};

constexpr std::array<VFKnockbackInfo, 5> kVFKnockbackInfo{{
    {1.0f, 1.0f, 1.0f, 1.2f, 1.0f, 1.2f, 0x1000, 0x1700, 0x0c00, 0x1300},
    {1.0f, 1.0f, 1.5f, 1.8f, 1.2f, 1.44f, 0x1800, 0x1b00, 0x1200, 0x1900},
    {1.0f, 1.0f, 2.0f, 2.1f, 1.2f, 1.44f, 0x1600, 0x1800, 0x1200, 0x1600},
    {1.0f, 1.0f, 2.5f, 2.5f, 2.5f, 2.5f, 0x1c00, 0x1c00, 0x1c00, 0x1c00},
    {1.0f, 1.0f, 1.3f, 1.2f, 1.2f, 1.2f, 0x0900, 0x1700, 0x0800, 0x1300},
}};

} // namespace shenmue::s1

namespace shenmue::s1 {

using namespace vf_detail;

extern std::int32_t g_vfBattleForceEndRequested;

extern VFFightStage *g_pFightStage;

extern std::uint8_t g_shenFightState;

extern std::uint8_t g_activeHpRegenProfile;

extern std::uint8_t g_shenFightState2;

extern std::uint32_t g_vfRuntimeAccumFightTicks;

} // namespace shenmue::s1

namespace shenmue::s1::vf_detail {

extern std::int32_t g_vfAirborneAngle;

extern std::array<std::array<std::uint8_t, 20>, 3> g_vfMoveAnimTypeLut;

extern VFGW *g_pVFGW_Widget;

extern VFMAData *g_pFightMain;

extern std::int32_t g_70mbLastRankIndex;

extern std::uint32_t g_70mbLastTimeTicks;

extern std::uint16_t g_vfConfirmButtonMask;

extern std::array<VFRegistryEntry, 16> g_vfFightRegistry;

std::uint8_t shenInsertTimeIntoTop5AndGetRank(std::uint32_t ticks);

void vfmaAccumulatePlayerCombatStats(VFMAData &game);

void InitVFGW();

void vfgwUpdateGauge(VFActor &actor);

void vfInitReactTable(VFMAData &game);

void vfQueueVoiceReaction(VFMAData &game);

void HandleRetry70ManBattlePrompt(UIRetry70ManBattlePromptResult result);

Character *FindCharacter(ScenarioContext *context, std::uint32_t id);

void vfUpdateWorldTransformAndFacingAngle(VFActor &actor);

void vfSelectDefaultReactionAndGuardAnims(VFActor &actor);

void figpResolveAnimID(const VFMAData &game);

void vfInitAnimTypeLUT(const VFMAData &game);

void UpdateCurrentAssistWindow(VFActor &actor);

void UpdateSlotRuntimeMetricsAndTimers(VFActor &actor);

void vfApplyYawRecursive(VFActor &actor);

void UpdateScenarioHpPhaseConfig(VFActor &actor);

bool vfAssignVoiceFor(VFMAData &game, std::uint8_t voiceCode);

bool vfmaAssignVoiceReactForFrame(VFMAData &game, std::uint8_t voiceCode);

void vfUpdateHitFacingAndKnockback(VFActor &actor);

std::uint32_t vfUpdateContactSlotsAndGroundFlags(VFActor &self);

void vfUpdateMain(VFActor &actor);

void vfmaUpdateCollisionCounter(VFMAData &game, VFCollisionCounterEvent event, std::uint16_t fighterId);

void vfmaUpdateNextReaction(VFActor &actor);

extern const std::array<std::uint32_t, 44> kVFMoveCategoryMasks;

void vfUpdateGroundHeightAndEnemyContacts(VFActor &actor);

void vfResolvePairInteraction(VFActor &self);

void vfUpdatePoseWithSfx(VFActor &actor);

std::uint8_t VF_GetAnimSetValue(std::uint8_t animSetIndex);

float VF_GetLocalActorAnimScale(const VFActor &actor);

std::int32_t shenQueryFightTaskOrRegistryValue(Character *character);

std::uint8_t vfRegisterContactSlot(VFMAData *game, VFEnemySlot *group);

void vfResetActorReactionStateForContact(VFMAData *game, std::uint8_t index);

void vfUpdateGuardLevel(VFMAData *game, std::uint8_t index);

void vfMonitorDrawMode(VFMAData *game);

void vfUpdateUI(VFMAData *game);

} // namespace shenmue::s1::vf_detail

namespace shenmue::s1 {

using namespace vf_detail;

VFMAData *vfMainGetCurrent();

extern std::uint32_t g_70mbBestTimesTicks[5];

void vfclResolveHitVsHitOutcome(VFActor &actorA, VFActor &actorB);

void vfclResolveHitVsHitOutcome(VFCLTaskData *context, std::uint8_t entityA, std::uint8_t entityB);

extern std::uint8_t g_globalFlags[1];

extern std::uint32_t g_searchLockOnFlags;

extern std::int32_t g_searchAnyInputPressedLatched;

extern std::int32_t g_searchAPressedLatched;

extern std::array<float, 2> g_debugTextPosition;

extern std::int32_t g_characterDrawMode;

extern std::int32_t g_drawMode;

extern std::uint8_t g_disableClockAndActionSelector;

extern std::array<std::uint8_t, 128> g_shenmueFlags;

void HLib_InertTaskCallback(HLib::TaskData *taskData);

} // namespace shenmue::s1

namespace shenmue::s1::vf_detail {

inline constexpr std::uint32_t kTaskDataToken = SHEN_NAME(TASK);

inline constexpr std::uint32_t kVfmaToken = SHEN_NAME(VFMA);

inline constexpr std::uint32_t kFightName = SHEN_NAME(FGHT);

inline constexpr std::uint32_t kDynaToken = SHEN_NAME(DYNA);

inline constexpr std::uint32_t kRunOrDashMode = 0x400U;

inline constexpr std::uint32_t kPtmvToken = SHEN_NAME(PTMV);

inline constexpr std::size_t kEnemyCapacity = 16;

inline constexpr std::size_t kFormationCapacity = 127;

inline constexpr std::size_t kAreaMaskCapacity = 64;

inline constexpr std::size_t kAnchorCapacity = 64;

inline constexpr float kVFFixedUnitsPerRadian = 65536.0f / 6.2831853071795864769f;

inline constexpr std::size_t kFightStyleCapacity = 64;

inline constexpr std::uint32_t kVFAirborne = 0x4U;

inline constexpr std::uint32_t kVFNoContactPairing = 0x40000U;

inline constexpr std::uint32_t kVFLockMovementInput = 0x4000000U;

struct VFAggroReactionConfig {
    std::uint8_t enemyCountThreshold;
    std::uint16_t initialReactionId;
    std::uint16_t followupReactionId;
};

constexpr std::array<VFAggroReactionConfig, 3> g_vfAggroReactionConfig{{
    {.enemyCountThreshold = 5, .initialReactionId = 0x0997, .followupReactionId = 0x09bd},
    {.enemyCountThreshold = 31, .initialReactionId = 0x08f3, .followupReactionId = 0x095b},
    {.enemyCountThreshold = 51, .initialReactionId = 0x09dc, .followupReactionId = 0x09dd},
}};

} // namespace shenmue::s1::vf_detail
