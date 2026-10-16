#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "s1/hlib.hpp"
#include "s1/task/scen.hpp"

namespace shenmue::s1 {
inline constexpr std::size_t kBattleSlots = 6;

struct Character;
struct Mt5RuntimeNode;
struct VFActor;
struct VFSlotRuntimeState;
struct VFFormationGroup;
struct BTPKTaskData;



extern std::int32_t g_vfBattleForceEndRequested;

#pragma pack(push, 1)
struct VFAnchor {
    std::uint8_t enabled;
    std::uint8_t excludeFromSelection;
    std::uint8_t flag2;
    std::uint8_t flag3;
    Vector3 position;
};

struct VFAreaMask {
    std::uint8_t active;
    std::uint8_t shapeMode;
    std::uint8_t unknown02;
    std::uint8_t unknown03;
    std::uint32_t unknown04;
    Vector2 vertices[4];
    std::uint32_t unknown28;
    std::uint32_t unknown2c;
    std::uint32_t quadrantMaskLow;
    std::uint32_t quadrantMaskHigh;
    std::int8_t neighbors[4];
    std::uint32_t unknown3c;
};

struct VFFormationGroup {
    Character *character;
    Character *blockedBy;
    struct VFEnemySlot *slot;
    std::uint16_t hpCurrent;
    std::uint16_t hpTotal;
    std::uint8_t fightStyle;
    std::int8_t hpPoolMode;
    std::int8_t teamId;
    std::int8_t waveId;
    std::uint32_t waveFlags;
    std::uint32_t selectFlags;
    std::uint32_t actorFlags;
    std::int8_t hpRegenRate;
    std::int8_t reserved2d;
    std::uint16_t entryIndex;
    std::uint16_t groupHpFloor;
    std::uint16_t wave;
    std::int16_t rankMin;
    std::int16_t rankMax;
    std::uint16_t spawnCount;
    std::int16_t delay;
    std::uint16_t animationId;
    std::uint16_t spawnYaw;
    Vector3 spawnPosition;
    std::uint8_t contactType;
    std::int8_t scriptParameter;
    std::int8_t field4e;
    std::int8_t field4f;
    std::int8_t startType;
    std::int32_t reserved51;
    std::byte reserved55[3];
};

struct VFEnemySlot {
    Character *character;
    void *motion;
    VFFormationGroup *wave;
    Vector3 position;
    std::uint16_t accumulatedYaw;
    std::byte reserved26[2];
    std::int8_t quadrant;
    std::uint8_t state;
    std::uint16_t tickFrames;
    std::uint8_t teamId;
    std::int8_t isRunning;
    std::int16_t reserved2e;
    std::uint32_t savedConfigFlags;
    float playbackRate;
    std::uint32_t renderMode;
    std::uint32_t savedActiveFlag;
    std::uint32_t savedLightMask;
    std::uint32_t savedVisibleFlag;
    std::int8_t registryIndex;
    std::int8_t field49;
    std::int8_t battleIndex;
    std::byte reserved4b[5];
    VFActor *fighter;
    std::byte reserved58[8];
    std::uint32_t triggerFlags;
    std::uint32_t activeMask;
    std::int32_t effectorEnemyMasks[7];
    float effectorGroundOffsets[7];
    float effectorHeightBlend[7];
    std::uint8_t effectorOffsetActive[7];
    std::int8_t cooldownDelayFrames;
    std::int16_t facingAngle;
    std::int16_t reservedc6;
    float fadeAlpha;
    float referenceRamp;
    Vector3 referenceOffset;
    float positionX;
    float positionZ;
    std::int8_t shouldSetPosition;
    std::int8_t modelSlotIndex;
    std::int16_t reservede6;
    void *model;
};

struct VFFightStage {
    VFEnemySlot *enemySlots;
    VFFormationGroup *formations;
    VFAreaMask *areaMasks;
    VFAnchor *anchors;
    std::uint8_t maxEnemies;
    std::uint8_t formationCapacity;
    std::uint8_t areaMaskCount;
    std::uint8_t generatorCapacity;
    std::byte reserved24[4];
    void *styles[64];
    Vector3 ringBoundary;
    std::int32_t accumulatedHp;
    Vector3 battleCenter;
    std::uint8_t useCenterBias;
    std::int8_t enemySlotCount;
    std::int8_t formationCount[2];
    std::int8_t readyEnemySlots[2];
    std::int8_t activeFightersByTeam[2];
    std::uint8_t aliveByTeam[2];
    std::uint8_t groupCounters[4];
    std::uint8_t teamRespawnTimer[2];
    std::byte reserved254[76];
};

struct VFRegistryEntry {
    Character *character;
    std::int8_t slot;
    std::byte reserved09[3];
    float health;
};

struct VFBattleMeta {
    VFEnemySlot *groups[8];
    VFFormationGroup *waves[8];
};

struct VFMAData {
    shen_name name;
    std::int32_t reserved04;
    HLib::Task *task;
    HLib::Task *collisionTask;
    HLib::Task *gameInfoTask;
    HLib::Task *packTask;
    std::uint32_t flags;
    std::uint32_t extraValue;
    std::uint32_t playerFlags;
    std::int32_t state;
    std::uint32_t frameCount;
    std::uint32_t activeBattleFrames;
    std::uint32_t runtimeTicks;
    std::int32_t hpDerivedTimer;
    std::int32_t delayTimer;
    float knockbackAmount;
    std::uint32_t gameFlags;
    std::uint32_t guardEffectTimer;
    std::uint8_t hitStopFrames;
    std::uint8_t battleType;
    std::uint16_t unknown5a;
    float knockbackGravityScale;
    float knockbackDragScale;
    float knockbackSlideDragScale;
    std::int32_t field68;
    float field6c;
    float field70;
    float playerReactionCameraScale;
    float playerAssistScale;
    std::int32_t reserved7c;
    Character *playerCharacter;
    std::int8_t playerSlot;
    std::uint8_t primaryTargetSlot;
    std::uint8_t remainingEnemies;
    std::int8_t enemyRanks[6];
    std::uint16_t reserved91;
    std::int8_t engagedEnemies;
    std::byte reserved94[8];
    std::byte reserved9c[4];
    Character *characters[kBattleSlots];
    std::byte reservedd0[0x10];
    VFBattleMeta battleMeta;
    std::uint64_t field160;
    VFActor *actors[kBattleSlots];
    std::byte reserved198[0x10];
    VFSlotRuntimeState *slotRuntime[kBattleSlots];
    std::byte reserved1d8[0x10];
    void *collisionData;
    BTPKTaskData *packData;
    void *padCaptureEntry;
    void *inputBuffer;
    std::byte reserved208[0x20];
    std::uint32_t guardEffectSeconds;
    std::uint8_t slotContactMisc[12];
    std::uint8_t slotHpPoolMode[kBattleSlots];
    std::byte reserved23e[2];
    std::uint8_t slotTeamId[kBattleSlots];
    std::byte reserved246[0x1c];
    std::uint8_t maxActiveEnemies;
    std::byte reserved263[5];
    std::int32_t soundSlots[kBattleSlots];
    std::byte reserved280[8];
    float soundContactThreshold;
    std::uint16_t soundTimer;
    std::int16_t soundResetTimer;
    std::int8_t soundFlag;
    std::int8_t soundFrames;
    std::uint8_t soundSlotOrder[8];
    std::uint8_t actorIndex;
    std::uint8_t soundSlotState[kBattleSlots];
    std::byte reserved2a1[10];
    std::uint8_t inputLockOnStart;
    std::int8_t cameraTargetToggled;
    std::uint8_t previousDefeatedEnemies;
    std::byte reserved2ae[2];
    Character *primaryCameraTarget;
    Character *secondaryCameraTarget;
    std::byte reserved2c0[0x10];
    std::uint8_t field2d0;
    std::byte reserved2d1[0x0f];
    float ringBoundary[3];
    std::uint32_t field2ec;
    std::uint8_t reactionTypeIndex;
    std::int8_t voiceCodeIndex;
    std::int16_t voiceDelay;
    std::int16_t timer6;
    std::int16_t reserved2f6;
    std::int8_t otherOne;
    std::uint8_t helpIconId;
    std::byte reserved2fa[2];
    std::uint8_t timer2;
    std::int8_t defeatedEnemies;
    std::int16_t skillTable[128];
    std::int16_t runtimeSkills[128];
    std::uint16_t runningFrames;
    std::uint16_t runningCooldownFrames;
    std::uint16_t comboTimer;
    std::int16_t partyFacingAngle;
    std::int16_t partyFacingFrames;
    std::uint16_t cooldownSequenceFrames;
    std::byte reserved50a[6];
};
static_assert(sizeof(VFMAData) == 0x510);
static_assert(sizeof(VFRegistryEntry) == 0x10);


struct VFStageObjectShape {
    std::uint32_t type;
    float values[6];
};

struct VOBJ {
    std::uint8_t state;
    std::int8_t boneId;
    std::int16_t reserved02;
    Vector3 position;
    Vector3i rotation;
    std::uint32_t entityFlags;
    std::int32_t entryCount;
    VFStageObjectShape entries[3];
    std::int8_t hitGuardStrength;
    std::int8_t hitReactionGuardType;
    std::int16_t hitParameter;
    std::int32_t animatedNodeCount;
    const std::int16_t *animatedNodeKinds;
    std::uint32_t frameRate;
    std::int32_t startFrame;
    std::uint32_t maximumFrame;
    std::byte reserved94[4];
    const std::int32_t *keyframeData;
    std::uint32_t reactionSoundCommand;
    std::int32_t hitReactionSoundCommand;
    std::int32_t landingSoundCommand;
    std::uint32_t soundCommand;
    float groundOffset;
    float timeScale;
    std::int32_t rotationAmount;
    std::int32_t axisRotateStep;
};
#pragma pack(pop)

#pragma pack(push, 8)

struct VFStageObjectKeyframeCursor {
    struct Channel {
        const std::int32_t *keys;
        std::int16_t cursor;
        std::int16_t count;
    };
    Mt5RuntimeNode *node;
    Channel channels[6];
};

struct VFStageObject {
    std::uint8_t objectIndex;
    std::uint8_t state;
    std::byte reserved02[6];
    VOBJ *object;
    std::uint32_t flags;
    std::uint32_t stateFlags;
    std::int32_t contactSlotMask;
    std::int32_t environmentFlags;
    Vector3 position;
    Vector3i rotation;
    std::int32_t field38;
    float velocityX;
    float velocityY;
    Vector3 localVelocity;
    Vector3i angle;
    Character *otherCharacter;
    Character *otherCharacter2;
    Character *linkedObject;
    std::uint32_t savedCollisionFlags;
    std::int32_t reserved7c;
    Character *character;
    VFStageObjectShape worldShapes[3];
    std::int32_t frameIndex;
    VFStageObjectKeyframeCursor *keyframeCursors;
    VFStageObject *next;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct VFSlotRuntimeState {
    std::byte reserved00[8];
    VFActor *actor;
    std::byte reserved10[8];
    float assistWindowDefault;
    float assistWindowGuard;
    std::int16_t assistAngleLimit;
    std::int16_t word22;
    std::int16_t specialTimerInitial;
    std::int16_t phaseArgumentInitial;
    std::uint8_t phaseConfig[14];
    std::byte reserved36[2];
    float assistWindowCurrent;
    std::uint32_t updateCamera;
    std::int16_t healthDifferences[kBattleSlots];
    std::byte reserved4c[4];
    std::uint8_t field50;
    std::byte reserved51[3];
    std::uint8_t byte54;
    std::uint8_t byte55;
    std::uint16_t word56;
    std::byte reserved58[2];
    std::uint8_t slotIndex;
    std::int8_t actorIndex;
    std::uint8_t unknown5c;
    std::uint8_t unknown5d[4];
    std::uint8_t currentUpdateIndex;
    std::uint8_t previousUpdateIndex;
    std::byte reserved63;
    std::uint16_t facingAngleDifferences[kBattleSlots];
    std::byte reserved70[4];
    std::uint8_t currentHpRegenerationPhase;
    std::uint8_t hpPhase;
    std::uint8_t cameraScriptStep;
    std::uint8_t unknown77;
    std::uint16_t cameraScriptWait;
    std::uint16_t savedCameraControlFlags;
    std::uint16_t reactionAnimationHistory[4];
    std::uint16_t rankHistory[3];
    std::uint16_t downedFrames;
    std::uint16_t repeatedReactionAnimation;
    std::uint8_t repeatedRankCount;
    std::uint8_t reactionAnimationHistoryIndex;
    std::uint8_t rankHistoryIndex;
    std::byte reserved91[9];
    std::uint8_t field9a;
    std::int8_t reserved9b;
    std::uint8_t byte9c;
    std::byte reserved9d[3];
    std::uint16_t framesRemaining;
    std::uint16_t fielda2;
    std::int32_t fielda4;
    std::uint16_t fielda8;
    std::uint8_t byteaa;
    std::uint8_t unknownab;
    std::int16_t currentStageHitResponseFrames;
    std::uint16_t hpPhaseIncrement;
    std::uint16_t wordb0;
    std::uint16_t fieldb2;
    std::uint16_t currentForcedReactionFrames;
    std::int8_t fieldb6;
    std::uint8_t fieldb7;
    std::uint8_t byteb8;
    std::uint8_t byteb9;
    std::uint8_t unknownba[4];
    std::byte reservedbe[2];
};

struct VFCameraScriptRow {
    std::uint8_t sectorCode[10];
    std::uint8_t holdFrames[10];
};
static_assert(sizeof(VFCameraScriptRow) == 0x14);

struct VFActionDescriptor {
    std::uint8_t opcode;
    std::uint8_t index;
    std::uint8_t resolve;
    std::uint8_t state;
    std::byte parameters[4];
};
static_assert(sizeof(VFActionDescriptor) == 8);

enum class VFReactResponse : std::uint8_t {
    Continue = 0,
    YieldToCaller = 1,
    Custom2 = 2,
    PopState = 3,
    ContinuePriorityScan = 4,
    CheckPriorityWindow = 5,
    EndAndResetChain = 6,
    End = 7,
    Custom8 = 8,
    Custom9 = 9,
    Custom10 = 10,
};

enum class VFStateMachineResult : std::uint8_t {
    Finished = 1,
    YieldToCaller = 0x7f,
};

struct VFReactionPriorityMeta {
    std::uint16_t mask1;
    std::uint16_t mask2;
    std::uint16_t flags;
    std::uint16_t unknownFlags;
    std::uint16_t value;
    std::uint16_t unknownValue;
};


struct VFReact {
    std::int16_t frameDelay;
    std::int8_t hasUsed;
    std::int8_t usedFlag;
    std::int32_t unk04;
    const char *subtitle;
    std::int8_t nextVoiceCode;
    std::int8_t unk11;
    std::int16_t unk12;
    std::int32_t unk14;
};
static_assert(sizeof(VFReact) == 0x18);
static_assert(offsetof(VFReact, subtitle) == 0x08);


struct VFGW {
    float field0;
    float field4;
    float field8;
    std::int16_t hp;
    std::int16_t sweepPos;
    std::int16_t sweepPhaseReset;
    std::int16_t sweepPhase;
    std::int16_t sweepPhaseStep;
    std::int16_t sweepDir;
    std::int16_t field18;
    std::int16_t sweepPosMax;
    std::int16_t sweepPosMin;
    std::uint8_t sweepActive;
    std::uint8_t pad1f;
    std::uint8_t field20;
    std::uint8_t field21;
    std::uint8_t field22;
    std::uint8_t field23;
    std::int16_t pulsePos;
    std::int16_t pulseAmpBase;
    std::int16_t pulseStep;
    std::int16_t pulseAmp;
    std::int16_t pulseDir;
    std::int16_t pulsePosMax;
    std::int16_t pulsePosMin;
    std::uint8_t pulseLocked;
};
static_assert(offsetof(VFGW, hp) == 0x0c);
static_assert(offsetof(VFGW, sweepActive) == 0x1e);
static_assert(offsetof(VFGW, pulsePos) == 0x24);
static_assert(offsetof(VFGW, pulseLocked) == 0x32);

struct VFReactionStateStackEntry {
    VFActionDescriptor *savedProgramCounter;
    std::int32_t savedPriorityScanCounter;
    std::uint32_t skipPriorityRestore;
    std::uint16_t savedPriorityFlags;
    std::uint16_t savedPriorityMask;
    std::uint16_t savedLastPriorityMask;
    std::uint16_t savedWindowAllowedMask;
    std::uint16_t savedWindowFramesRemaining;
    std::uint8_t savedPriorityIndex;
    std::byte reserved[5];
};

static_assert(sizeof(VFReactionPriorityMeta) == 0x0c);
static_assert(sizeof(VFReactionStateStackEntry) == 0x20);

struct VFActor {
    std::uint8_t slotIndex;
    std::int8_t targetSlotIndex;
    std::uint16_t fightStyleIndex;
    std::uint32_t moveFlags;
    std::uint32_t stateFlags;
    std::int32_t unknown0c;
    Character *character;
    Character *targetingCharacter;
    void *motion;
    VFMAData *game;
    VFSlotRuntimeState *slot;
    VFActor *targetActor;
    VFActor *hitSourceActor;
    void *reactionScriptBase;
    void *cameraScriptTable;
    void *styleTuning;
    void *contactProfile;
    std::uint8_t *animationTable;
    void *cameraPhaseTable;
    Vector3 position;
    Vector3 moveDelta;
    Vector3 relativeMoveDelta;
    Vector3 separation;
    Vector3 pairAnchor;
    Vector3 contactProbePositions[10];
    Vector3 contactCurrentPosition;
    Vector3 lowerSpinePosition;
    Vector3 headAttachmentPosition;
    Vector3 rightFootEndPosition;
    Vector3 leftFootEndPosition;
    Vector3 rightWristPosition;
    Vector3 leftWristPosition;
    Vector3 previousContactPosition;
    Vector3 trackedHitPosition;
    std::uint16_t facingAngle;
    std::uint16_t previousFacingAngle;
    std::uint16_t facingYaw;
    std::uint16_t temporaryFacingAngle;
    std::uint16_t contactAngles[8];
    std::uint16_t moveFacingLimit;
    std::uint16_t trackedContactAngle;
    std::int16_t airAngle;
    std::uint16_t airTiltAngle;
    Matrix4x4 lowerSpineTransform;
    Matrix4x4 rightWristTransform;
    Matrix4x4 leftWristTransform;
    Vector3 guardProbeWorldPosition;
    Vector3 guardProbeWorldDirection;
    std::int16_t angles[2];
    std::int32_t reserved294;
    float forwardX;
    float forwardZ;
    float knockbackStrength;
    float animationScale;
    float scale;
    float inverseScale;
    float timeScale;
    float knockbackAcceleration;
    std::uint8_t hitPhase;
    std::byte reserved2b9;
    std::uint16_t queuedReactionEvent;
    std::int32_t reserved2bc;
    VFActionDescriptor previousAction;
    VFActionDescriptor previousQueuedAction;
    std::int8_t reactionWindowPhase;
    std::byte reserved2d1;
    std::uint16_t reactionStateId;
    std::int16_t reactionAnimationEndFrame;
    std::int16_t postReactionAnimationId;
    std::uint16_t attackWindowEndFrame;
    std::uint16_t reactionMidFrame;
    std::uint16_t reactionEndFrame;
    std::uint16_t fallLandingWindowStartFrame;
    std::int8_t reactionMotionRestartMode;
    std::int8_t field2e1;
    std::uint16_t facingAngleStep;
    std::uint16_t previousHitStateFlags;
    std::uint16_t knockbackAnimationStartParameter;
    std::uint16_t knockbackAnimationEndFrame;
    std::uint16_t trackingSlideStartFrame;
    float knockbackReactionTime;
    std::uint8_t moveId;
    std::uint8_t guardState;
    std::uint8_t guardStrength;
    std::int8_t hitAttributeFlags;
    float hitDamageScale;
    std::uint16_t hitParameter;
    std::uint16_t hitDirectionAngle;
    float hitReactionScaleX;
    float hitReactionScaleZ;
    std::int32_t reserved304;
    void *reactionEventTable;
    std::int16_t skipAlignment;
    std::uint16_t reactionState02EndFrame;
    std::int16_t field314;
    std::uint8_t hitGuardType;
    std::int8_t reserved317;
    std::uint32_t stateFlags2;
    std::uint32_t counterReactionAnimationId;
    std::uint16_t midairRecoveryEndFrame;
    std::int16_t field322;
    std::uint16_t field324;
    std::uint16_t fallLandingWindowEndFrame;
    std::uint16_t queuedReactionAnimationId;
    std::uint16_t knockbackAnimationId;
    std::int32_t field32c;
    std::int16_t field330;
    std::int16_t field332;
    std::int8_t hitStunOverride;
    std::int8_t hitHeightClass;
    std::int8_t hitHeightClass2;
    std::int8_t reactionEventCount;
    std::uint32_t hitFlags;
    std::uint32_t hitStateFlags;
    std::int16_t pairFollowupActionWord;
    std::int16_t pairActionTriggerFrame;
    std::int32_t pairContactPriority;
    std::int32_t midFrameQueuedReactionActionWord;
    std::uint32_t endFrameQueuedReactionActionWord;
    std::uint8_t reactionRequestedMoveId;
    std::int8_t reserved351;
    std::uint16_t pairPriorityAnimationFrame;
    std::int32_t reserved354;
    void *reactionProgramStart;
    void *reactionProgram;
    void *reactionProgramCounter;
    VFActionDescriptor action;
    VFActionDescriptor nextAction;
    VFActionDescriptor deferredPairAction;
    VFActionDescriptor queuedAction;
    std::uint32_t hitStateFlags2;
    std::uint32_t savedReactionStateFlags;
    std::uint32_t previousHitStateFlags2;
    std::int32_t field39c;
    std::uint32_t motionFlags;
    std::uint32_t temporaryHitFlags;
    std::int32_t temporaryHitStateFlags;
    std::int32_t field3ac;
    std::uint32_t previousMotionFlags;
    std::int32_t groundContactFlags;
    std::uint32_t contactDecisionMask;
    std::uint32_t contactActionMask;
    std::int32_t latchedPairContactPriority;
    std::uint32_t counterReactionFlags;
    std::int16_t currentHp;
    std::int16_t maximumHp;
    float contactPoints[8];
    float field3ec;
    float field3f0;
    float field3f4;
    std::int8_t field3f8;
    std::int8_t hitReactionAnimSelectFlags;
    std::int16_t hitPhaseWaitFrames;
    std::uint16_t hitStunFrames;
    std::uint16_t guardImpactAccum;
    std::int8_t guardStat;
    std::int8_t guardLevel;
    std::int16_t reserved402;
    float forwardClearance;
    float backwardClearance;
    float rightClearance;
    float leftClearance;
    std::uint16_t hitStateHoldTimer;
    std::uint16_t knockbackFramesRemaining;
    std::uint16_t knockbackTotalFrames;
    std::uint16_t reserved41a;
    std::uint16_t hitTimeout;
    std::uint16_t actionPhaseFrameCounter;
    std::int16_t hitReactionAnimId;
    std::uint16_t queuedKnockbackReactionAnimId;
    float downedReferenceY;
    std::int32_t reserved428;
    std::uint16_t lastHitMoveId;
    std::uint16_t activeContactCount;
    std::uint16_t ignoredHitCount;
    std::uint16_t lastProcessedHitMoveId;
    std::int8_t knockbackUnknown;
    std::int8_t knockbackPhase;
    std::uint16_t field436;
    std::uint16_t field438;
    std::int32_t reserved43a;
    std::uint16_t field43e;
    std::byte reserved440[12];
    float minimumGroundY;
    float predictedGroundLanding;
    float targetGroundY;
    float groundY;
    float contactRangeFrontMin;
    float contactRangeFrontMax;
    float contactRangeSideMin;
    float contactRangeSideMax;
    float contactRangeBackMin;
    float knockbackBaseParameter;
    float reactionTimer;
    float followupKnockbackAccelerationScale;
    std::uint16_t currentReactionAnimation;
    std::uint16_t temporaryAnimationId;
    std::uint16_t previousAnimationId;
    std::uint16_t hpRegenerationFrames;
    std::uint32_t pendingAnimationId;
    std::uint16_t currentAnimationEndFrame;
    std::uint16_t temporaryAnimationParameter;
    std::uint16_t pairReferenceFrame;
    std::uint16_t pairFrameOffset;
    std::int16_t deferredPairActionTriggerFrame;
    std::int8_t pairProcessedMask;
    std::int8_t reserved493;
    std::uint16_t previousReactionAnimationId;
    std::uint16_t previousAnimationEndFrame;
    Vector3 moveVelocity;
    float knockbackSlideX;
    float knockbackAccelerationBias;
    float knockbackSlideZ;
    Vector3 pendingKnockback;
    Vector3 knockbackFrameDelta;
    float trackingSlideStepX;
    float trackingSlideStepZ;
    std::int64_t reactionOverrideActive;
    std::uint16_t cameraControlFlags;
    std::int16_t priorityMask;
    std::uint16_t priorityFlags2;
    std::uint16_t priorityFlags;
    VFReactionPriorityMeta reactionPriority[32];
    std::uint8_t reactionRingHead;
    std::int8_t reactionRingTail;
    union {
        struct {
            std::uint8_t stopSlotIndex;
            std::int8_t priorityIndex;
        } scan;
        struct {
            std::uint8_t primary;
            std::int8_t secondary;
        } slots;
    };
    std::uint8_t reactionSlotCurrent;
    std::uint8_t selectedReactionSlot;
    std::uint8_t savedReactionQueueTail;
    std::int16_t otherPriorityFlags;
    std::uint8_t rewindReactionQueueTail;
    std::int16_t field66a;
    std::int16_t field66c;
    std::int8_t delayedReactionStateActive;
    std::int8_t reserved66f;
    std::uint16_t delayedReactionTimer;
    std::uint16_t delayedReactionFramesRemaining;
    std::byte reserved674[12];
    VFActionDescriptor *reactionResumeProgramCounter;
    VFActionDescriptor *reactionVmProgramCounter;
    VFActionDescriptor *rootReactionVmProgramCounter;
    std::int16_t rootReactionDelayFrames;
    std::int16_t scheduledReactionYaw;
    std::int16_t alternateRootReactionDelayFrames;
    std::int16_t reserved69e;
    VFActionDescriptor *pendingAlternateRootReactionProgram;
    std::int16_t rootReactionModeFlags;
    std::byte reserved6aa[6];
    VFActionDescriptor *alternateRootReactionProgram;
    std::int8_t reactionStateStackDepth;
    std::uint8_t reactionPriorityIndex;
    std::int16_t functionReactionSlotIndex;
    std::uint16_t lastPriorityMaskSnapshot;
    std::int16_t reserved6be;
    std::uint32_t priorityScanCounter;
    std::uint32_t field6c4;
    std::byte reserved6c8[4];
    std::uint16_t priorityMaskDifference;
    std::uint16_t currentReactionFlagFilter;
    std::uint16_t previousReactionFlagFilter;
    std::uint16_t priorityWindowFramesRemaining;
    std::uint16_t priorityWindowAllowedMask;
    std::uint16_t reserved6d6;
    VFReactionStateStackEntry reactionStateStack[16];
    std::uint32_t reactionConditionFlags;
    std::int32_t reserved8dc;
    std::int8_t reactionChainFrames;
    std::int8_t reactionCounters[7];
    std::int32_t reactionStateFlags;
    std::uint32_t reactionPriorityMaskA;
    std::int32_t reactionPriorityMaskB;
    std::uint32_t field8f4;
    std::int32_t moveCategory;
    std::uint32_t contactMetadataFilter;
    std::int32_t connectionResultMetadata;
    std::int8_t connectionMask;
    std::int8_t terrainCollisionLatched;
    std::int16_t reserved906;
    Vector3 previousContactProbePositions[10];
    std::uint32_t contactProfileMasks[10];
    std::uint32_t collisionHitFlags;
    std::int16_t reserved9ac;
    std::uint16_t field9ae;
    float contactShakeAverage;
    float contactShakeHistory[4];
    std::int64_t reserved9c4;
    std::int32_t hitOutcome;
    std::int32_t reserved9d0;
    float hitPowerScale;
    float temporaryKnockback;
    float hitImpulseMagnitude;
    std::uint16_t knockbackAngle;
    std::int8_t suppressKnockback;
    std::uint8_t fighterRankId;
    std::uint8_t fighterIdAlt;
    std::uint8_t fighterIdAlt2;
    std::uint8_t fighterIdAlt3;
    std::int8_t moveState;
    std::uint8_t linkFlags;
    std::byte reserved9e9[3];
    float effectorGroundOffsets[7];
    float effectorHeightBlend[7];
    std::int32_t effectorEnemyMasks[7];
    float headGroundDelta;
    float copiedGroundDelta;
    std::uint32_t triggerMask;
    std::uint32_t triggerState;
    std::int16_t soundHpFloor;
    std::uint16_t groupHpFloor;
    std::uint16_t requiredFighterRank;
    std::int16_t fighterRankMin;
    std::int16_t fighterRankMax;
    std::int16_t cameraActiveTriggerFrames;
    std::int16_t cameraIdleTriggerFrames;
    std::int16_t cameraActiveCounterFrames;
    std::int16_t cameraIdleCounterFrames;
    std::int8_t hpTickPeriodFrames;
    std::int8_t fielda63;
    std::int16_t priorityRank;
    std::uint16_t moveFacingAngle;
    std::int8_t moveAngleTableIndex;
    std::int8_t moveAngleTableIndex2;
    std::uint8_t moveAnimationFlags;
    std::int8_t previousMoveAnimationFlags;
    std::uint16_t previousCameraControlFlags;
    std::uint8_t followupState;
    std::int8_t forcedTurnState;
    Vector3 followTargetPosition;
    std::int32_t reserveda7c;
    std::int64_t fielda80;
    float fielda88;
    std::int8_t facingQuadrantPath[64];
    std::int8_t facingQuadrantStep;
    std::uint8_t cachedQuadrant;
    std::int8_t previousQuadrant;
    std::int8_t stageHitObjectIndex;
    const std::byte *contactVolumeRecords;
    std::uint16_t cameraAngle;
    std::int16_t cameraYaw;
    std::uint16_t cameraAngleOffset;
    std::int16_t defaultReactionAnimationId;
    std::int16_t defaultGuardAnimationId;
    std::int16_t defaultStanceAnimationId;
    std::uint8_t reactionQueuePhase;
    std::int8_t pairedSlotIndex;
    std::uint8_t knockoutCount;
    std::uint8_t pairedFighterId;
    Character *pairedCharacterCache;
    VFActor *pairedActor;
    VFActor *pairedFighter;
    Character *savedNestedActorCharacter;
    std::int64_t fieldb08;
};

#pragma pack(pop)

inline constexpr Vector3 kVFGuardProbeLocalVectors[2]{{0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};
inline constexpr Vector3 kVFWristAngleLocalVectors[2]{{0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}};

extern VFFightStage *g_pFightStage;
extern std::int32_t g_70ManIconTransitionState;

extern std::uint32_t g_70mbBestTimesTicks[5];
extern std::uint32_t g_mfbtCurrentDefeatedEnemies;
extern std::uint8_t g_prevHelpIconFlag;

extern std::uint8_t g_shenFightState;

extern std::uint8_t g_defaultHpRegenProfile;
extern std::uint8_t g_activeHpRegenProfile;
extern std::uint32_t g_vfRuntimeAccumFightTicks;

extern std::uint8_t g_shenFightState2;
extern std::int32_t g_VFForcePlayerGodmode;


extern std::int32_t g_vfPlayerAggroCombat;

extern std::uint8_t g_bIsFighting;
bool VFMA_IsPlayerGodmodeForced();
std::uint16_t VFCL_SelectReactionAnimation(VFActor &attacker, const VFActor &defender, std::uint32_t selector);

enum class VFMoveRequestFlags : std::uint8_t {
    ReturnDeltaOnly = 0,
    ReturnAbsolute = 1,
    UseEasing = 2,
    ApplyEasedCommitTrigger = 3,
};

std::int16_t VFMA_RequestMove(VFMoveRequestFlags flags);
void VFMA_SetFieldOfViewForSearchOrClear(std::uint8_t mode);

void *vfQueueStartGameMain();
void vfm_Callback_FightTaskEntry_Callback(HLib::TaskData *taskData);
void vfg_Callback_GameInfoEntry(HLib::TaskData *taskData);
VFMAData *vfMainGetCurrent();
VFActor *vfFindFighterFor(Character *character);
std::uint32_t VFMA_GetRemainingFrames();
bool VFMA_IsBattleActive();
bool VFMA_IsFighting();
bool VFMA_HasBoundFightStyles();
std::uint8_t VFMA_ResolveEffectQuadrant(float x, float z, std::uint8_t prior);
bool VFMA_GetBattleCameraTargets(Character *&player, Character *&target);
std::uint32_t VFMA_GetCharacterFightFlags(Character *character);
void VFMA_ReinitializeFightScriptState(ScenarioContext *context, void *params);
void Script_fightingStyle(ScenarioContext *context, void *params);

void Script_vfTransitionToInactive(ScenarioContext *context, void *params);
void Script_vfUtils(ScenarioContext *context, void *params);
void SCN_Enqueue_VFGI(ScenarioContext *context, void *params);
void Script_CPCTRelated(ScenarioContext *context, void *params);
void Script_vfSetup(ScenarioContext *context, void *params);
void Script_ptmvSetFollowTuning(ScenarioContext *context, void *params);
void Script_GetFightingStats(ScenarioContext *context, void *params);
void SCN_GetIsFighting(ScenarioContext *context, void *params);
void Script_InterpolateAndScaleAngles(ScenarioContext *context, void *params);
void Script_CheckIsInsideCullVolume(ScenarioContext *context, void *params);
void Script_vfToggleBattleState(ScenarioContext *context, void *params);
void Script_vfConfigureFormationGroup(ScenarioContext *context, void *params);
void Script_FGTM_Utils(ScenarioContext *context, void *params);
void Script_vfStageAssignRingBoundary(ScenarioContext *context, void *params);
void Script_CPCT_related_0(ScenarioContext *context, void *params);
void Script_vfStageObjectUtils(ScenarioContext *context, void *params);
void Script_vfSetupAreaMasks(ScenarioContext *context, void *params);




inline constexpr std::array<std::array<std::array<std::uint16_t, 8>, 3>, 36> kVFOrdinaryReactionAnimations{{
    {{{{0x0138U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0138U, 0x0138U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0136U, 0x0136U, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x012cU, 0x012cU, 0x0000U, 0x0000U, 0x012cU, 0x012cU, 0x012cU, 0x012cU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x0323U, 0x0323U, 0x046eU, 0x046eU, 0x043dU, 0x043dU, 0x0440U, 0x0440U}}}},
    {{{{0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U}}, {{0x03afU, 0x03b5U, 0x0000U, 0x0000U, 0x03b5U, 0x03b5U, 0x03afU, 0x03afU}},
        {{0x05adU, 0x05adU, 0x05aaU, 0x05aaU, 0x05b3U, 0x05b3U, 0x05b0U, 0x05b0U}}}},
    {{{{0x012cU, 0x012cU, 0x0000U, 0x0000U, 0x012cU, 0x012cU, 0x012cU, 0x012cU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x012cU, 0x012cU, 0x0000U, 0x0000U, 0x012cU, 0x012cU, 0x012cU, 0x012cU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x0323U, 0x0323U, 0x046eU, 0x046eU, 0x043dU, 0x043dU, 0x0440U, 0x0440U}}}},
    {{{{0x012aU, 0x012aU, 0x0000U, 0x0000U, 0x012aU, 0x012aU, 0x012aU, 0x012aU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x012bU, 0x012bU, 0x0000U, 0x0000U, 0x012bU, 0x012bU, 0x012bU, 0x012bU}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05baU, 0x05baU, 0x0428U, 0x0428U, 0x0441U, 0x0441U, 0x043eU, 0x043eU}}}},
    {{{{0x012bU, 0x012bU, 0x0000U, 0x0000U, 0x012bU, 0x012bU, 0x012bU, 0x012bU}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0136U, 0x0136U, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05baU, 0x05baU, 0x0428U, 0x0428U, 0x043eU, 0x043eU, 0x0441U, 0x0441U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0136U, 0x0136U, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05baU, 0x05baU, 0x0428U, 0x0428U, 0x0441U, 0x0441U, 0x043eU, 0x043eU}}}},
    {{{{0x0137U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0137U, 0x0137U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0137U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0137U, 0x0137U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0137U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0137U, 0x0137U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0137U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0137U, 0x0137U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x012aU, 0x012aU, 0x0000U, 0x0000U, 0x012aU, 0x012aU, 0x012aU, 0x012aU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0136U, 0x0136U, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0136U, 0x0136U, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0135U, 0x013aU, 0x0000U, 0x0000U, 0x013aU, 0x013aU, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x012aU, 0x012aU, 0x0000U, 0x0000U, 0x012aU, 0x012aU, 0x012aU, 0x012aU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b1U, 0x03b1U, 0x03b7U, 0x03b7U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x0137U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0137U, 0x0137U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0138U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0138U, 0x0138U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0137U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0137U, 0x0137U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0137U, 0x0137U, 0x0000U, 0x0000U, 0x0137U, 0x0137U, 0x0137U, 0x0137U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U}}, {{0x03afU, 0x03b5U, 0x0000U, 0x0000U, 0x03b5U, 0x03b5U, 0x03afU, 0x03afU}},
        {{0x05adU, 0x05adU, 0x05aaU, 0x05aaU, 0x05b3U, 0x05b3U, 0x05b0U, 0x05b0U}}}},
    {{{{0x012aU, 0x012aU, 0x0000U, 0x0000U, 0x012aU, 0x012aU, 0x012aU, 0x012aU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0136U, 0x0136U, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x0138U, 0x0137U, 0x0000U, 0x0000U, 0x0138U, 0x0137U, 0x0138U, 0x0137U}}, {{0x0138U, 0x0137U, 0x0000U, 0x0000U, 0x0138U, 0x0137U, 0x0138U, 0x0137U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x008eU, 0x008cU, 0x0000U, 0x0000U, 0x008eU, 0x008cU, 0x008eU, 0x008cU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x012aU, 0x012aU, 0x0000U, 0x0000U, 0x012aU, 0x012aU, 0x012aU, 0x012aU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0136U, 0x0136U, 0x0135U, 0x0135U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b9U, 0x03b9U, 0x03b3U, 0x03b3U}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x012aU, 0x012aU, 0x0000U, 0x0000U, 0x012aU, 0x012aU, 0x012aU, 0x012aU}}, {{0x03b1U, 0x03b7U, 0x0000U, 0x0000U, 0x03b7U, 0x03b7U, 0x03b1U, 0x03b1U}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U}}, {{0x03afU, 0x03b5U, 0x0000U, 0x0000U, 0x03b5U, 0x03b5U, 0x03afU, 0x03afU}},
        {{0x05adU, 0x05adU, 0x05aaU, 0x05aaU, 0x05b3U, 0x05b3U, 0x05b0U, 0x05b0U}}}},
    {{{{0x048cU, 0x048cU, 0x0000U, 0x0000U, 0x048cU, 0x048cU, 0x048cU, 0x048cU}}, {{0x048cU, 0x048cU, 0x0000U, 0x0000U, 0x048cU, 0x048cU, 0x048cU, 0x048cU}},
        {{0x05b6U, 0x05b6U, 0x05acU, 0x05acU, 0x05b5U, 0x05b5U, 0x05b2U, 0x05b2U}}}},
    {{{{0x048bU, 0x048bU, 0x0000U, 0x0000U, 0x048bU, 0x048bU, 0x048bU, 0x048bU}}, {{0x048bU, 0x048bU, 0x0000U, 0x0000U, 0x048bU, 0x048bU, 0x048bU, 0x048bU}},
        {{0x05afU, 0x05afU, 0x05abU, 0x05abU, 0x05b4U, 0x05b4U, 0x05b1U, 0x05b1U}}}},
    {{{{0x0135U, 0x0136U, 0x0000U, 0x0000U, 0x0135U, 0x0136U, 0x0135U, 0x0136U}}, {{0x03b3U, 0x03b9U, 0x0000U, 0x0000U, 0x03b3U, 0x03b9U, 0x03b3U, 0x03b9U}},
        {{0x05aeU, 0x05aeU, 0x05aeU, 0x05aeU, 0x05aeU, 0x05aeU, 0x05aeU, 0x05aeU}}}},
}};

} // namespace shenmue::s1
