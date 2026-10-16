#pragma once

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {



extern std::uint8_t g_fightActive;
extern std::uint8_t g_fightUiActive;
extern std::uint16_t g_fightModeFlags;
extern std::int32_t g_forcePlayerGodmode;
extern std::int32_t g_hasColouredText;
extern std::int32_t g_is_vfm_active;
extern std::uint8_t g_battleQteCommandSelection;
extern std::uint16_t g_fightLoadWidth, g_fightLoadHeight;
extern std::int32_t g_fightCombatCounterSnapshot;

// CTRL construction calls this with mode 0.
void fightInit(std::uint16_t mode);
void InitVFM();
std::uint64_t IsFightActive();
std::uint64_t GetFightModeFlags();
std::uint64_t SetFightModeFlags(std::uint16_t flags);

struct Character;
struct VFActor;
struct FRMG_t;
struct MotionWork;
struct VFMA_t;
struct VFCL_t;
using FightMotionCallback = void (*)(std::uint32_t, Vector3 *, MotionWork *, void *);

inline constexpr shen_name TASK_FGST{SHEN_NAME(FGST)};
inline constexpr shen_name TASK_FCMG{SHEN_NAME(FCMG)};
inline constexpr shen_name TASK_FRMG{SHEN_NAME(FRMG)};
inline constexpr shen_name TASK_FGEN{SHEN_NAME(FGEN)};
inline constexpr shen_name TASK_FBIT{SHEN_NAME(FBIT)};
inline constexpr shen_name TASK_FBSM{SHEN_NAME(FBSM)};
inline constexpr shen_name TASK_FIGH{SHEN_NAME(FIGH)};
inline constexpr std::size_t fightSmokeSlotCount = 9;

// The unrecovered gameplay fields retain their physical storage, not fabricated actor behavior.
struct FCMG_t {
    Character *character;
    MotionWork *motion;
    FRMG_t *group;
    Vector3 position;
    std::uint16_t accumulatedYaw, reserved26;
    std::int8_t quadrant;
    std::uint8_t state;
    std::uint16_t tickFrames;
    float runningTicks;
    std::uint8_t counterGroup, flags;
    std::uint16_t reserved32;
    std::uint32_t savedControlFlags;
    float savedMotionScale;
    std::byte reserved3c[0xc];
    std::uint32_t savedExtraMotionFlag;
    std::uint8_t fightEntryIndex, reserved4d, actorIndex, reserved4f;
    VFActor *fighter;
    std::byte reserved58[8];
    std::uint32_t collisionFlags, stackReferenceMask;
    std::uint32_t stackLayerMasks[7];
    float stackLayerOffsets[7], groundSamples[7];
    std::uint8_t groundCorrectionEnabled[7];
    std::uint8_t cooldownDelay;
    std::int16_t savedFacingAngle;
    std::byte reservedC6[2];
    float fadeAlpha, reflectorIntensity;
    Vector3 reflectorColour;
    float offsetX, offsetZ;
    std::uint8_t setPosition;
    std::int8_t savedPositionState;
    std::uint16_t reservedE6;
    std::uint64_t positionState;
    std::uint32_t savedRootMode, savedConstraintAngle, savedProjectionAngle, reservedFc;
    FightMotionCallback savedMotionCallbacks[7];
    void *savedMotionParameters[7];
};

struct FRMG_t {
    Character *character;
    void *blocker;
    FCMG_t *slot;
    std::uint16_t hp, totalHp;
    std::uint8_t style, poolMode, team, state;
    std::uint32_t waveFlags, selectFlags, formationFlags, activationFlags;
    std::uint32_t gameplayParameters[2];
    std::uint16_t animation, spawnYaw;
    std::uint16_t actionParameters[2];
    std::uint16_t spawnWords[2];
    Vector3 spawnPosition;
    std::uint32_t activationState;
    std::uint8_t assignment;
    std::byte reserved[3];
};

struct FGEN_t {
    std::uint8_t enabled;
    std::uint8_t other;
    std::uint16_t reserved;
    Vector3 position;
};

struct FightStyleSlot {
    std::uint8_t style;
    std::byte reserved[7];
    void *file;
};

struct FightSpawnPoint {
    float parameter;
    float x;
    float z;
};

struct FightSmokeSlot {
    std::uint16_t effect;
    std::uint16_t reserved;
    float remainingTicks;
    float birthRate;
};

struct FBSM_t {
    std::uint8_t activeCount;
    std::byte reserved[3];
    std::uint32_t effects[fightSmokeSlotCount];
    FightSmokeSlot slots[fightSmokeSlotCount];
};



struct FGST_t {
    FCMG_t *enemySlots;
    FRMG_t *groups;
    void *areaMasks;
    FGEN_t *generators;
    std::uint8_t enemyCount;
    std::uint8_t groupCount;
    std::uint8_t areaMaskCount;
    std::uint8_t generatorCount;
    std::uint32_t stageParameters;
    FightStyleSlot styles[8];
    FightSpawnPoint spawnPoints[2];
    std::uint16_t teamRespawnTimers[2];
    std::uint8_t spawnPointCount;
    std::byte reservedSpawn[3];
    std::uint32_t accumulatedHp;
    Vector3 battleCenter;
    std::uint8_t useCenterBias;
    std::uint8_t enemySlotCount;
    std::uint8_t formationCount[2];
    std::uint8_t readyEnemySlots[2];
    std::uint8_t activeFightersByTeam[2];
    std::uint8_t unknownGroupCounts[2];
    std::uint8_t groupCounters[4];
    std::uint8_t secondaryGroupCounters[4];
    std::byte reservedCounters[2];
    std::uint32_t battleFlags;
    void *packData;
    HLib::Task *smokeInitTask;
    FBSM_t *smoke;
};

static_assert(sizeof(FCMG_t) == 0x170);
static_assert(offsetof(FCMG_t, flags) == 0x31);
static_assert(sizeof(FRMG_t) == 0x58);
static_assert(offsetof(FRMG_t, activationFlags) == 0x2c);
static_assert(offsetof(FRMG_t, formationFlags) == 0x28);
static_assert(offsetof(FRMG_t, gameplayParameters) == 0x30);
static_assert(offsetof(FRMG_t, actionParameters) == 0x3c);
static_assert(offsetof(FRMG_t, spawnWords) == 0x40);
static_assert(offsetof(FRMG_t, animation) == 0x38);
static_assert(offsetof(FRMG_t, activationState) == 0x50);
static_assert(offsetof(FRMG_t, assignment) == 0x54);
static_assert(sizeof(FGEN_t) == 0x10);
static_assert(sizeof(FightStyleSlot) == 0x10);
static_assert(sizeof(FightSmokeSlot) == 0x0c);
static_assert(sizeof(FBSM_t) == 0x94);
static_assert(offsetof(FBSM_t, effects) == 4);
static_assert(offsetof(FBSM_t, slots) == 0x28);
static_assert(sizeof(FGST_t) == 0x108);
static_assert(offsetof(FGST_t, styles) == 0x28);
static_assert(offsetof(FGST_t, spawnPoints) == 0xa8);
static_assert(offsetof(FGST_t, teamRespawnTimers) == 0xc0);
static_assert(offsetof(FGST_t, spawnPointCount) == 0xc4);
static_assert(offsetof(FGST_t, groupCounters) == 0xe2);
static_assert(offsetof(FGST_t, battleFlags) == 0xec);
static_assert(offsetof(FGST_t, packData) == 0xf0);
static_assert(offsetof(FGST_t, smokeInitTask) == 0xf8);
static_assert(offsetof(FGST_t, smoke) == 0x100);

extern FGST_t *g_pFightStage;
extern std::uint8_t g_FGEN_count;
extern std::uint8_t g_vfFormationGroupIndex;
extern std::uint8_t g_vfStreetFightLevel;
extern std::int32_t g_vfStrikeLabelSprites[2];

FGST_t *AllocateFGST();
void AllocateFCMG(std::uint8_t count);
void AllocateFRMG(std::uint8_t count);
void AllocateFGEN(std::uint8_t count);
FGST_t *vfAddGeneratorPoint(float x, float y, float z);
FGST_t *vfCopyPosFromEntity(std::uint8_t mode, Character *character, float x, float z, float parameter);
void AllocateFBIT();
void FB_CallBack_InitTask_Callback(void *taskData);
void InitializeFightSmokeEffects();
void UpdateFightSmokeEffects();
void FreeFightSmokeEffects();
std::uint8_t shenStartFGST(std::uint8_t enemies, std::uint8_t groups, std::uint8_t areaMasks, std::uint8_t generators);
VFMA_t *check_dead_status(std::uint8_t mode);
std::intptr_t ToggleStreetFighterUI();

// FTBL178 returns the formation producer's signed byte through EAX.
std::uint32_t shenInitFightData(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style);
VFActor *btlGetActorByScenarioName(VFMA_t *game, shen_name name);
std::uint32_t vfclControlRuntimeFlags(VFCL_t *collision, std::uint32_t mask, std::int32_t action);
std::uint8_t vfUpdateFlagsFor(Character *character, std::int8_t mode, std::uint32_t flag, std::int32_t value);
void ToggleFightMotionCallbackFlag(Character *character, std::int32_t enabled);
std::uint32_t SetFightRoundCounts(std::uint8_t first, std::uint8_t second);
void SetStreetFightLevel(std::uint8_t level);
std::uint64_t GetStreetFightProgress() noexcept;
std::uint64_t SetStreetFightProgress(std::uint32_t value) noexcept;
std::uint64_t AddStreetFightProgress(std::int32_t delta) noexcept;
void ResetStreetFightProgress() noexcept;
std::uint64_t CreateFightStrikeLabels();

enum class FightBattleUtilOp : std::uint8_t {
    CollisionFlags = 0,
    CombatCameraFlags = 1,
    ActorField = 2,
    StageBattleFlags = 3,
    FightModeFlags = 4,
    ForceActorDefeat = 5,
    MoveFlag400000 = 6,
    SkillMask = 7,
    MotionOrSlotFlag = 8,
    MoveFlag40000 = 9
};
std::uint32_t EV_vfBattleUtil(FightBattleUtilOp operation, shen_name name, std::int32_t value);

enum class FightControlOp : std::uint8_t {
    ReactionRank = 1,
    Package = 2,
    SpawnTimer = 4,
    MotionUpdateCamera = 5,
    CameraMode = 6,
    QueuedAction = 7,
    CameraParameter = 8,
    GroupCounter = 9,
    SpriteDepth = 10,
    ResetCameraWaypoints = 11,
    AppendCameraWaypoint = 12,
    MotionInterpolationCamera = 13,
    MotionNibble = 14,
    RoundCounts = 15,
    StreetFightLevel = 16,
    StrikeLabels = 17,
    StrikesLeft = 18,
    BattleCounter = 19,
    GaugeResources = 20,
    MovesScroll = 21,
    BattleType = 22
};

std::int64_t vfmDispatchControl(FightControlOp operation, shen_name name, std::uintptr_t payload, std::int32_t action);

extern std::uint8_t g_savedFightBlurFlag;

} // namespace shenmue::s2
