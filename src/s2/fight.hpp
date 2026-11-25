#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/player_stats_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/hlib.hpp"
#endif

namespace shenmue {
namespace s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define TASK_VCDS shen_name(SHEN_NAME4('V', 'C', 'D', 'S'))
#define TASK_VCDW shen_name(SHEN_NAME4('V', 'C', 'D', 'W'))
#define FIGHT_DEAD SHEN_NAME4('D', 'E', 'A', 'D')
#define TASK_FGST shen_name(SHEN_NAME4('F', 'G', 'S', 'T'))
#define TASK_FCMG shen_name(SHEN_NAME4('F', 'C', 'M', 'G'))
#define TASK_FRMG shen_name(SHEN_NAME4('F', 'R', 'M', 'G'))
#define TASK_FGEN shen_name(SHEN_NAME4('F', 'G', 'E', 'N'))
#define TASK_FBIT shen_name(SHEN_NAME4('F', 'B', 'I', 'T'))
#define TASK_FBSM shen_name(SHEN_NAME4('F', 'B', 'S', 'M'))

struct VCDS_t {
    std::uint8_t state, condition;
    std::uint16_t flags;
    std::int16_t occurrences;
    std::uint16_t retained06;
    std::uint32_t id, idleFrames;
    VCDS_t *previous, *next;
    std::int32_t (*callback)(void *);
    std::uint16_t priority;
    std::int16_t dialogue, previousDialogue;
    std::uint16_t callbackArgument;
    shen_name character, otherCharacter, lastCharacter;
    float probability;
    std::int16_t repeatEvery;
    std::uint16_t completions;
    std::uint32_t period;
    std::uint16_t animation, previousAnimation, health, retained42;
    float healthPercent, distance;
    std::uint32_t reactionMask;
};
typedef char Layout_fight_0[(sizeof(VCDS_t) == 0x50) ? 1 : -1];
typedef char Layout_fight_1[(offsetof(VCDS_t, previous) == 0x10) ? 1 : -1];
typedef char Layout_fight_2[(offsetof(VCDS_t, callback) == 0x18) ? 1 : -1];
typedef char Layout_fight_3[(offsetof(VCDS_t, priority) == 0x1c) ? 1 : -1];
typedef char Layout_fight_4[(offsetof(VCDS_t, probability) == 0x30) ? 1 : -1];
typedef char Layout_fight_5[(offsetof(VCDS_t, previousAnimation) == 0x3e) ? 1 : -1];
typedef char Layout_fight_6[(offsetof(VCDS_t, reactionMask) == 0x4c) ? 1 : -1];

struct VCDW_t {
    std::uint8_t state, currentPriority;
    std::uint16_t flags;
    VCDS_t *records;
    std::uint32_t retained08, nextId, counter, retained14;
    std::int16_t selected, parameters;
    std::byte retained1c[0x34];
};
typedef char Layout_fight_7[(sizeof(VCDW_t) == 0x50) ? 1 : -1];
typedef char Layout_fight_8[(offsetof(VCDW_t, records) == 4) ? 1 : -1];
typedef char Layout_fight_9[(offsetof(VCDW_t, nextId) == 0xc) ? 1 : -1];
typedef char Layout_fight_10[(offsetof(VCDW_t, selected) == 0x18) ? 1 : -1];
#endif
struct Character;
struct FightActorRuntimePrefix;
struct FightRuntimePrefix;
struct FightGaugeWork;
struct MotionWork;
struct FRMG_t;
struct FCMG_t {
    Character *character;
    MotionWork *motion;
    FRMG_t *group;
    Vector3 position;
    std::byte retained18[4];
    std::int8_t quadrant;
    std::uint8_t state;
    union {
        std::uint16_t flags;
        std::uint16_t runningTicks;
    };
    std::uint8_t counterGroup, retained21;
    std::byte retained22[0x1a];
    std::uint8_t fightEntryIndex, retained3d, actorIndex, retained3f;
    FightActorRuntimePrefix *fighter;
    std::byte retained44[4];
    std::uint32_t collisionFlags;
    std::byte retained4c[0x20];
    float stackLayerOffsets[7], groundSamples[7];
    std::uint8_t groundCorrectionEnabled[7];
    std::byte retainedab[5];
    float fadeAlpha;
    Vector4 reflectorColour;
    std::byte retainedc4[9];
    std::int8_t savedPositionState;
    std::byte retainedce[2];
    std::uint32_t positionState;
    std::byte retainedd4[0x44];
};
struct FRMG_t {
    Character *character;
    void *blocker;
    FCMG_t *slot;
    std::uint16_t hp, totalHp;
    std::uint8_t style, poolMode, team, state;
    std::uint32_t waveFlags, selectFlags, formationFlags, activationFlags;
    std::uint8_t retained24, retained25;
    std::uint16_t range, height, direction;
    std::int16_t animation, spawnYaw;
    std::uint16_t actionParameters[2], spawnWords[2];
    Vector3 spawnPosition;
    std::uint8_t activationState, activationParameter, activationIndex;
    std::int8_t assignment, secondaryAssignment;
    std::byte retained49[3];
};
struct FGEN_t {
    std::uint8_t enabled, other;
    std::uint16_t retained02;
    Vector3 position;
};
struct FightStyleSlot {
    std::uint8_t style;
    std::byte retained01[3];
    void *file;
};
struct FightSpawnPoint {
    float parameter, x, z;
};
struct FightSmokeSlot {
    std::int16_t effect;
    std::uint16_t remainingTicks;
    float birthRate;
};
struct FBSM_t {
    std::uint8_t activeCount;
    std::byte retained01[3];
    std::uint32_t effects[9];
    FightSmokeSlot slots[9];
};
typedef char FightSmokeSizeCheck[sizeof(FBSM_t) == 0x70 ? 1 : -1];
struct FGST_t {
    FCMG_t *enemySlots;
    FRMG_t *groups;
    void *areaMasks;
    FGEN_t *generators;
    std::uint8_t enemyCount, groupCount, areaMaskCount, generatorCount;
    FightStyleSlot styles[8];
    FightSpawnPoint spawnPoints[2];
    std::uint16_t teamRespawnTimers[2];
    std::uint8_t spawnPointCount;
    std::byte retained71[3];
    std::uint32_t accumulatedHp;
    Vector3 battleCenter;
    std::uint8_t useCenterBias, enemySlotCount, formationCount[2], readyEnemySlots[2], activeFightersByTeam[2], unknownGroupCounts[2];
    std::uint8_t groupCounters[4], secondaryGroupCounters[4];
    std::byte retained96[2];
    std::uint32_t battleFlags;
    void *packData;
    HLib::Task *smokeInitTask;
    FBSM_t *smoke;
};
typedef char FightContactSlotSizeCheck[sizeof(FCMG_t) == 0x118 ? 1 : -1];
typedef char FightContactSlotActorCheck[offsetof(FCMG_t, fighter) == 0x40 ? 1 : -1];
typedef char FightContactSlotFadeCheck[offsetof(FCMG_t, fadeAlpha) == 0xb0 ? 1 : -1];
typedef char FightFormationSizeCheck[sizeof(FRMG_t) == 0x4c ? 1 : -1];
typedef char FightFormationFlagsCheck[offsetof(FRMG_t, formationFlags) == 0x1c ? 1 : -1];
typedef char FightFormationActivationCheck[offsetof(FRMG_t, activationState) == 0x44 ? 1 : -1];
typedef char FightStageSizeCheck[sizeof(FGST_t) == 0xa8 ? 1 : -1];
typedef char FightStageCountCheck[offsetof(FGST_t, enemySlotCount) == 0x85 ? 1 : -1];
typedef char FightStageFlagsCheck[offsetof(FGST_t, battleFlags) == 0x98 ? 1 : -1];
extern FGST_t *g_pFightStage;
extern std::uint8_t g_vfFormationGroupIndex, g_FGEN_count, g_battleQteCommandSelection;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocateFGST();
#else
FGST_t *AllocateFGST();
#endif
void AllocateFCMG(std::uint8_t count);
void AllocateFRMG(std::uint8_t count);
void AllocateFGEN(std::uint8_t count);
extern std::int32_t g_hasColouredText;
extern std::uint16_t g_fightLoadWidth, g_fightLoadHeight;
void shenStartFGST(std::uint8_t enemies, std::uint8_t groups, std::uint8_t areaMasks, std::uint8_t generators);
void AllocateFBIT();
void FB_CallBack_InitTask_Callback(void *parameter);
void InitializeFightSmokeEffects();
void FreeFightSmokeEffects();
void UpdateFightSmokeEffects();
void FreeFightAreaMaskNullsub();
void ToggleStreetFighterUI();
void vfAddGeneratorPoint(float x, float y, float z);
void vfCopyPosFromEntity(std::uint8_t mode, Character *character, float x, float z, float parameter);
void PublishFightRespawn(std::uint16_t hp, std::uint8_t style, std::uint16_t spawnWord0, float x, float y, float z, std::int32_t spawnWord1);
void check_dead_status(std::uint8_t mode);
void fbqtResetStates(std::uint8_t mode);
std::uint8_t vfUpdateFlagsFor(Character *character, std::uint8_t mode, std::uint32_t flag, std::int32_t value);
std::uint32_t shenInitFightData(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style);
struct Character;
struct FightActorRuntimePrefix {
    std::int8_t groupIndex;
    std::byte retained01[3];
    std::uint32_t moveFlags, stateFlags;
    Character *character;
    std::uint32_t retained10;
    MotionWork *motion;
    FightRuntimePrefix *game;
    void *slot;
    FightActorRuntimePrefix *actor;
    FightActorRuntimePrefix *secondActor;
    std::byte retained28[8];
    std::byte *motionParameters;
    std::byte retained34[0x0c];
    Vector3 position;
    std::byte retained4c[0x1a];
    std::int16_t motionTimer;
    Vector3 targetPosition;
    std::byte retained74[0x78];
    Vector3 contactPosition;
    std::byte retainedf8[0x6c];
    std::uint16_t facingAngle, retained166, facingYaw;
    std::uint16_t temporaryFacingAngle;
    std::int16_t actorFacingAngles[6];
    std::byte retained178[0xf4];
    float motionSpeedScale, inverseMotionSpeedScale;
    std::byte retained274[0x0a];
    std::uint16_t queuedReactionEvent;
    std::byte retained280[0x14];
    std::uint16_t motionFrames;
    std::byte retained296[4];
    std::int16_t facingAngleDelta;
    std::byte retained29c[0x0c];
    std::uint8_t moveId;
    std::byte retained2a9[2];
    std::byte retained2ab[1];
    float attackReach;
    std::uint16_t hitParameter;
    std::byte retained2b2[2];
    float motionImpulseX, motionImpulseZ;
    std::byte retained2bc[6];
    std::uint16_t contactStartFrame, contactEndFrame;
    std::byte retained2c6[0x0a];
    std::uint16_t pairContactStartFrame;
    std::byte retained2d2[0x0e];
    std::uint16_t pairContactEndFrame;
    std::byte retained2e2[6];
    std::uint32_t motionFlags, secondaryMotionFlags;
    std::uint32_t markerMask;
    std::byte retained2f4[2];
    std::uint16_t animationEndFrame;
    std::byte retained2f8[0x30];
    std::uint32_t queuedAction;
    std::uint32_t hitStateFlags, savedHitStateFlags, previousHitStateFlags, retained338, activeMotionFlags;
    std::uint32_t savedMotionFlags, savedSecondaryMotionFlags, retained348, previousActiveMotionFlags, retained350;
    std::uint32_t groundContactFlags, reactionFlags, hitFlags;
    std::byte retained360[8];
    float hp;
    std::int16_t initialHp;
    std::byte retained36e[2];
    float contactDistances[6];
    std::byte retained388[0x1d];
    std::uint8_t retainedMode;
    std::byte retained3a6[0x1a];
    std::uint16_t impulseDelayFrames;
    std::byte retained3c2[0x46];
    float targetGroundY;
    std::byte retained40c[0x20];
    std::uint16_t currentAnimation, retained42e, previousAnimation;
    std::byte retained432[6];
    float animationTime;
    std::byte retained43c[0x18];
    Vector3 knockbackVelocity, slideVelocity;
    std::byte retained46c[0x24];
    std::uint16_t inputFlags;
    std::byte retained492[0x3f6];
    float groundHeights[7];
    std::byte retained8a4[0x1c];
    std::uint32_t nearbyGroundActors[7];
    float groundHeightDifference;
    std::int32_t primaryGroundTarget;
    float previousGroundSamples[7], targetGroundSamples[7];
    float groundReactionScale;
    float groundClearance, attackerGroundClearance;
    std::uint32_t formationFlags;
    std::uint32_t runtimeFlags;
    std::byte retained930[0x0e];
    std::uint16_t moveAngleTableIndex;
    std::byte retained940[5];
    std::uint8_t nearestSpawnPoint;
    std::byte retained946[0x8a];
    FightGaugeWork *gauge;
    std::byte retained9d4[4];
    std::uint32_t hitResolutionFlags;
    std::byte retained9dc[8];
    float retainedMotionValues[3];
    std::byte retained9f0[4];
};
typedef char FightActorCharacterOffsetCheck[offsetof(FightActorRuntimePrefix, character) == 0x0c ? 1 : -1];
typedef char FightActorDialogueEventCheck[offsetof(FightActorRuntimePrefix, queuedReactionEvent) == 0x27e ? 1 : -1];
typedef char FightActorDialogueDistanceCheck[offsetof(FightActorRuntimePrefix, contactDistances) == 0x370 ? 1 : -1];
typedef char FightActorDialogueAnimationCheck[offsetof(FightActorRuntimePrefix, currentAnimation) == 0x42c ? 1 : -1];
typedef char FightActorDialogueInputCheck[offsetof(FightActorRuntimePrefix, inputFlags) == 0x490 ? 1 : -1];
typedef char FightActorPositionOffsetCheck[offsetof(FightActorRuntimePrefix, position) == 0x40 ? 1 : -1];
typedef char FightActorTargetPositionOffsetCheck[offsetof(FightActorRuntimePrefix, targetPosition) == 0x68 ? 1 : -1];
typedef char FightActorAnimationTimeOffsetCheck[offsetof(FightActorRuntimePrefix, animationTime) == 0x438 ? 1 : -1];
typedef char FightActorMotionTimerOffsetCheck[offsetof(FightActorRuntimePrefix, motionTimer) == 0x66 ? 1 : -1];
typedef char FightActorMotionOffsetCheck[offsetof(FightActorRuntimePrefix, motion) == 0x14 ? 1 : -1];
typedef char FightActorLinkedOffsetCheck[offsetof(FightActorRuntimePrefix, actor) == 0x20 ? 1 : -1];
typedef char FightActorContactOffsetCheck[offsetof(FightActorRuntimePrefix, contactPosition) == 0xec ? 1 : -1];
typedef char FightActorFloorOffsetCheck[offsetof(FightActorRuntimePrefix, targetGroundY) == 0x408 ? 1 : -1];
typedef char FightActorMoveAngleOffsetCheck[offsetof(FightActorRuntimePrefix, moveAngleTableIndex) == 0x93e ? 1 : -1];
typedef char FightActorMotionFlagsOffsetCheck[offsetof(FightActorRuntimePrefix, motionFlags) == 0x2e8 ? 1 : -1];
typedef char FightActorHitFlagsOffsetCheck[offsetof(FightActorRuntimePrefix, hitStateFlags) == 0x32c ? 1 : -1];
typedef char FightActorMotionImpulseOffsetCheck[offsetof(FightActorRuntimePrefix, motionImpulseX) == 0x2b4 ? 1 : -1];
typedef char FightActorMotionSpeedOffsetCheck[offsetof(FightActorRuntimePrefix, motionSpeedScale) == 0x26c ? 1 : -1];
typedef char FightActorImpulseDelayOffsetCheck[offsetof(FightActorRuntimePrefix, impulseDelayFrames) == 0x3c0 ? 1 : -1];
typedef char FightActorContactFramesOffsetCheck
    [offsetof(FightActorRuntimePrefix, contactStartFrame) == 0x2c2 && offsetof(FightActorRuntimePrefix, pairContactEndFrame) == 0x2e0 ? 1 : -1];
typedef char FightActorSlideVelocityOffsetCheck[offsetof(FightActorRuntimePrefix, slideVelocity) == 0x460 ? 1 : -1];
typedef char FightActorHitResolutionOffsetCheck[offsetof(FightActorRuntimePrefix, hitResolutionFlags) == 0x9d8 ? 1 : -1];
typedef char FightActorParametersOffsetCheck[offsetof(FightActorRuntimePrefix, motionParameters) == 0x30 ? 1 : -1];
typedef char FightActorHpOffsetCheck[offsetof(FightActorRuntimePrefix, hp) == 0x368 ? 1 : -1];
typedef char FightActorInitialHpOffsetCheck[offsetof(FightActorRuntimePrefix, initialHp) == 0x36c ? 1 : -1];
typedef char FightActorFormationFlagsOffsetCheck[offsetof(FightActorRuntimePrefix, formationFlags) == 0x928 ? 1 : -1];
typedef char FightActorModeOffsetCheck[offsetof(FightActorRuntimePrefix, retainedMode) == 0x3a5 ? 1 : -1];
typedef char FightActorFlagsOffsetCheck[offsetof(FightActorRuntimePrefix, runtimeFlags) == 0x92c ? 1 : -1];
typedef char FightActorMotionValuesOffsetCheck[offsetof(FightActorRuntimePrefix, retainedMotionValues) == 0x9e4 ? 1 : -1];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char FightActorSizeCheck[sizeof(FightActorRuntimePrefix) == 0x9f4 ? 1 : -1];
typedef char FightActorGroundHeightCheck[offsetof(FightActorRuntimePrefix, groundHeights) == 0x888 ? 1 : -1];
typedef char FightActorGroundSampleCheck[offsetof(FightActorRuntimePrefix, previousGroundSamples) == 0x8e4 ? 1 : -1];
typedef char FightActorGroundScaleCheck[offsetof(FightActorRuntimePrefix, groundReactionScale) == 0x91c ? 1 : -1];
#endif

struct FightContactSample {
    Vector3 points[3];
};
struct FightCollisionRuntimePrefix {
    shen_name name;
    FightRuntimePrefix *game;
    FightActorRuntimePrefix *defender, *attacker;
    std::uint32_t flagsWord, contactType, maskA, maskB;
    float slideScale, stickScale;
    std::uint32_t attackerMask, defenderMask;
    float penetrationCharacter, penetrationSpecial, separationLength;
    Vector3 separation;
    FightContactSample samples[6], previousSamples[6];
    std::uint32_t contactMasks[6], ringMasks[6];
    Vector2 contactDeltas[6];
    std::uint32_t runtimeFlags;
};
typedef char FightCollisionSizeCheck[sizeof(FightCollisionRuntimePrefix) == 0x25c ? 1 : -1];
typedef char FightCollisionSamplesCheck[offsetof(FightCollisionRuntimePrefix, samples) == 0x48 ? 1 : -1];
typedef char FightCollisionContactMasksCheck[offsetof(FightCollisionRuntimePrefix, contactMasks) == 0x1f8 ? 1 : -1];
typedef char FightCollisionContactDeltasCheck[offsetof(FightCollisionRuntimePrefix, contactDeltas) == 0x228 ? 1 : -1];
typedef char FightCollisionFlagsOffsetCheck[offsetof(FightCollisionRuntimePrefix, runtimeFlags) == 0x258 ? 1 : -1];
struct FightRuntimePrefix {
    std::uint32_t retained00;
    HLib::Task *task, *collisionTask, *gameInfoTask;
    std::uint32_t retained10, stateFlags;
    std::uint32_t frame;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t savedUiFlags;
    std::int32_t state;
    std::uint32_t elapsedTicks;
    std::uint32_t timedBattleTime, blurAmount, hpTimer, retained34;
    float knockbackAmount;
    std::uint32_t controlFlags, elapsedFrames;
    std::uint8_t retained44, type;
    std::byte retained46[2];
    float gravityScale, dragScale, slideDragScale;
    std::byte retained54[4];
    float floorIntersectionY, floorSnapY, cameraResponse, playerCameraScale, playerAssistScale, contactRange;
    Character *playerCharacter;
    std::int8_t playerSlot;
    std::uint8_t opponentSlot, remainingEnemies;
    std::byte retained77[0x11];
    Character *characters[6];
    std::byte retaineda0[8];
    FCMG_t *enemySlots[6];
    std::byte retainedc0[8];
    FRMG_t *formations[6];
    std::byte retainede0[0x0c];
    FightActorRuntimePrefix *actors[6];
    std::byte retained104[8];
    void *actorSlots[6];
    std::byte retained124[8];
    FightCollisionRuntimePrefix *collision;
    void *savedInput;
    void *inputBuffer;
    std::byte retained138[0x20];
    std::int32_t guardEffectSeconds;
    std::byte retained15c[0x0c];
    std::uint8_t slotPoolModes[8];
    std::uint8_t slotTeams[6];
    std::byte retained176[0x1c];
    std::uint8_t maximumActiveEnemies;
    std::byte retained193[5];
    std::uint32_t sfxSlotFlags[6];
    std::byte retained1b0[0x2c];
    std::uint8_t cameraSet, retained1dd, strikesLeft;
    std::byte retained1df[0x29];
    FightSpawnPoint spawnPoints[2];
    std::uint8_t spawnPointCount;
    std::uint8_t spriteDepth;
    std::byte retained222[6];
    void *dialogue;
    std::byte retained22c[0x0e];
    std::uint8_t battleCounter, retained23b;
    std::int16_t outcome;
    std::byte retained23e[0x20a];
    VCDW_t *cameraDialogue;
    std::byte retained44c[4];
    std::uint8_t cameraParameter;
    std::byte retained451[0x1b];
    std::uint16_t authCameraFrame;
    std::byte retained46e[2];
#endif
};
typedef char Layout_fight_11[(offsetof(FightRuntimePrefix, stateFlags) == 0x14) ? 1 : -1];
typedef char Layout_fight_12[(offsetof(FightRuntimePrefix, frame) == 0x18) ? 1 : -1];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char Layout_fight_13[(offsetof(FightRuntimePrefix, cameraDialogue) == 0x448) ? 1 : -1];
typedef char FightStateOffsetCheck[(offsetof(FightRuntimePrefix, state) == 0x20) ? 1 : -1];
typedef char FightControlOffsetCheck[(offsetof(FightRuntimePrefix, controlFlags) == 0x3c) ? 1 : -1];
typedef char FightElapsedOffsetCheck[(offsetof(FightRuntimePrefix, elapsedFrames) == 0x40) ? 1 : -1];
typedef char FightOutcomeOffsetCheck[(offsetof(FightRuntimePrefix, outcome) == 0x23c) ? 1 : -1];
typedef char FightSoundMaterialOffsetCheck[offsetof(FightRuntimePrefix, cameraParameter) == 0x450 ? 1 : -1];
typedef char FightSpriteDepthOffsetCheck[offsetof(FightRuntimePrefix, spriteDepth) == 0x221 ? 1 : -1];
typedef char FightCharactersOffsetCheck[offsetof(FightRuntimePrefix, characters) == 0x88 ? 1 : -1];
typedef char FightActorsOffsetCheck[offsetof(FightRuntimePrefix, actors) == 0xec ? 1 : -1];
typedef char FightRuntimeSizeCheck[sizeof(FightRuntimePrefix) == 0x470 ? 1 : -1];
typedef char FightInputOffsetCheck[offsetof(FightRuntimePrefix, inputBuffer) == 0x134 ? 1 : -1];
typedef char FightSpawnPointsOffsetCheck[offsetof(FightRuntimePrefix, spawnPoints) == 0x208 ? 1 : -1];
typedef char FightMaximumEnemiesOffsetCheck[offsetof(FightRuntimePrefix, maximumActiveEnemies) == 0x192 ? 1 : -1];
std::uint32_t vfclControlRuntimeFlags(FightCollisionRuntimePrefix *collision, std::uint32_t mask, std::int32_t action);
std::uint32_t EV_vfBattleUtil(std::uint8_t operation, shen_name name, std::int32_t value);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t vfFindCharaIndex(const FightRuntimePrefix *game, const Character *character);
#else
std::uint32_t vfFindCharaIndex(const FightRuntimePrefix *game, const Character *character);
#endif
std::uint32_t GetFightRuntime(Character *character);
std::uint32_t GetSetFightActorByte(std::uint8_t operation, Character *character, std::int32_t value);
float GetSetFightActorFloat(std::uint8_t operation, Character *character, float value);
FightActorRuntimePrefix *btlGetActorByScenarioName(FightRuntimePrefix *game, shen_name name);
FightActorRuntimePrefix *FindFightActorByName(FightRuntimePrefix *game, shen_name name);
std::int32_t FindBattleStatusEntry(Character *character);
void UpdateEnemySlotCooldownsForDefeat(FightRuntimePrefix *game);
void *GetFightTaskParameter();
std::uint32_t IsFightStageFlag2Set();
void SetFightLoadSprite(std::int32_t index, const char *text, std::int32_t width, std::int32_t height);
void FreeFightLoadSprites();
void ResetFightHudGlobals();
void SetFightActorParameterBytes(shen_name character, const std::uint8_t *values);
std::uint32_t btlGetCharaFlag(std::uint8_t operation, Character *character, float hp);
void SetFightOutcomeMode(std::int16_t outcome);
std::int32_t GetFightResult();
std::uint32_t vfGetElapsedSeconds();
void SetActiveFightModeFlag();
std::uint32_t vfSetFightControlFlag(std::int32_t enabled);
std::int32_t vfmDispatchControl(std::uint8_t operation, shen_name name, std::uintptr_t payload, std::int32_t action);
std::uint8_t vfMotnNibbleGet(std::uint8_t code);
void SetFightRoundCounts(std::uint8_t first, std::uint8_t second);
void vfAddRoundResults(std::uint8_t first, std::uint8_t second);
void SetStreetFightLevel(std::uint8_t level);
void CreateFightStrikeLabels();
extern std::uint8_t g_vfRoundWins, g_vfRoundLosses, g_vfRoundWinLimit, g_vfRoundLossLimit, g_vfStreetFightLevel;
extern std::int32_t g_vfStrikeLabelSprite, g_vfStrikesLeftLabelSprite;
#endif

FightRuntimePrefix *vfmaGetTaskParam();
std::uint32_t GetFightStateFlags();
std::uint32_t GetFightFrame();

extern std::uint16_t g_fightModeFlags;
extern std::int32_t g_is_vfm_active;
void fightInit(std::int32_t mode);
void InitVFM();
std::uint32_t GetStreetFightProgress();
std::uint32_t SetStreetFightProgress(std::uint32_t value);
std::uint32_t AddStreetFightProgress(std::int32_t delta);
std::uint32_t IsFightActive();
std::uint32_t GetFightModeFlags();
std::uint32_t SetFightModeFlags(std::uint16_t flags);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t AllocatePeriodicBattleDialogue(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition15(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition2(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float healthPercent, float probability);
std::uint32_t AllocateBattleDialogueCondition3(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t health, float probability);
std::uint32_t AllocateBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::int16_t repeatEvery,
    float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition5(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition6(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition7(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition13(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition16(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition17(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition10(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition11(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition12(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateMotionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t animation,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateReactionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::uint32_t reactionMask, std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition9(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float distance, float probability, std::uint16_t flags);
std::uint32_t QueueBattleDialogue(std::int16_t priority, std::int16_t dialogue);
void SetBattleDialogueFlags(std::uint32_t id, std::uint16_t flags);
void FreeBattleDialogue(std::uint32_t id);
void BeginFightDialogueRetirement();
void SuspendBattleDialogue(std::uint32_t id);
void ResumeBattleDialogue(std::uint32_t id);
std::int32_t GetBattleDialogueOccurrences(std::uint32_t id);
std::uint32_t GetBattleDialogueCompletions(std::uint32_t id);
VCDW_t *GetFightCameraDialogue(FightRuntimePrefix *game);
void UpdateFightCameraDialogue(FightRuntimePrefix *game);
void InitializeFightCameraDialogue(FightRuntimePrefix *game);
VCDS_t *FindCameraDialogue(FightRuntimePrefix *game, std::uint32_t id);
std::uint32_t vfRegisterCameraDialogue(VCDS_t *record);
void RetireCameraDialogueRecords();
void FreeFightCameraDialogue(FightRuntimePrefix *game);
void AssignFightDialogueCallback(std::uint32_t id, std::int32_t (*callback)(void *));
#endif
} // namespace s2
} // namespace shenmue

#else

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

// Native street-fight globals. The fight stage reads the active byte, the fight-UI byte suppresses the
// TIME, NAVI and main-icon HUD owners, and street-fight events read and replace the mode word.
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
void RemoveBattleDialogue(std::uint32_t id);
std::uint32_t GetFightStageObjectFlags();
void SetFightCounterHighWord(std::uint16_t value);

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


// FGST and FRMG allocate 0x108 and 0x58 bytes; style records are eight 0x10-byte entries.
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
// The third native argument is a full R8 payload: integer, descriptor, pointer or filename by operation.
std::int64_t vfmDispatchControl(FightControlOp operation, shen_name name, std::uintptr_t payload, std::int32_t action);

extern std::uint8_t g_savedFightBlurFlag;

} // namespace shenmue::s2

#endif
