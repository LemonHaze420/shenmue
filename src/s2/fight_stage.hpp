#pragma once

#include "s2/fight.hpp"
#include "s2/fight_flags.hpp"
#include <array>

namespace shenmue::s2 {

inline constexpr shen_name TASK_FGTM{SHEN_NAME(FGTM)};
inline constexpr shen_name TASK_FENE{SHEN_NAME(FENE)};
inline constexpr shen_name TASK_VOBJ{SHEN_NAME(VOBJ)};
inline constexpr shen_name TASK_VCDS{SHEN_NAME(VCDS)};
inline constexpr shen_name TASK_VCDW{SHEN_NAME(VCDW)};

struct VFMA_t;
struct VFActor;
struct VFGW_t;
struct VFHD_t;
struct VCDW_t;
struct VFCL_t;
struct PadControlEntry;
#pragma pack(push, 1)
struct VFSlotPhaseConfig {
    std::uint8_t enabled, countdown, selected, reserved;
    std::uint8_t choices[16];
};
struct VFSlotRuntimeState {
    std::uint32_t id, reserved04;
    VFActor *actor;
    std::uint64_t parameter10;
    float assistWindowDefault, assistWindowGuard;
    std::uint16_t parameter20, parameter22, hitTimerMaximum, guardTimerMaximum, parameter28;
    std::int8_t parameter2a;
    std::byte reserved2b[6];
    std::int8_t parameter31;
    std::uint16_t reserved32;
    float assistWindowCurrent;
    std::uint32_t cameraFlags;
    float healthDifferences[8];
    std::uint8_t randomBytes[2];
    std::uint16_t randomWord, resetIndex;
    std::byte reserved62[4];
    std::uint8_t opponentIndex;
    std::byte reserved67[5];
    std::uint8_t reactionIndex, previousReactionIndex;
    std::byte reserved6e[2];
    float ticks, previousTicks;
    std::uint16_t facingDifferences[6];
    std::byte reserved84[5];
    std::uint16_t parameter89;
    std::byte reserved8b;
    std::uint16_t parameter8c, parameter8e;
    std::uint16_t animationHistory[4], reactionHistory[3];
    std::uint16_t pressure, parameterA0;
    std::int8_t parameterA2, parameterA3, parameterA4;
    std::byte reservedA5[9];
    std::int8_t parameterAe;
    std::byte reservedAf[5];
    float hitTimer, guardTimer, parameterBc, guardTicks, opponentGuardTicks, opponentAttackTimer;
    float parameterCc, reactionTicks, motionTicks, hpPhaseTimer;
    std::uint32_t reservedDc;
    float opponentMotionTimer;
    std::uint32_t parameterE4;
    std::byte reservedE8[0x40];
    std::uint8_t parameter128, parameter129, parameter12a, reserved12b, guardPhase;
    VFSlotPhaseConfig phases[6];
    std::int8_t parameter1a5;
    std::byte reserved1a6[6];
    std::int8_t animationPhase;
    std::byte reserved1ad[3];
    float secondaryMotionTimer, motionTimer;
};
#pragma pack(pop)
static_assert(sizeof(VFSlotPhaseConfig) == 0x14);
static_assert(sizeof(VFSlotRuntimeState) == 0x1b8);
static_assert(offsetof(VFSlotRuntimeState, cameraFlags) == 0x38);
static_assert(offsetof(VFSlotRuntimeState, parameter89) == 0x89);
static_assert(offsetof(VFSlotRuntimeState, randomBytes) == 0x5c && offsetof(VFSlotRuntimeState, resetIndex) == 0x60);
static_assert(offsetof(VFSlotRuntimeState, reserved62) == 0x62 && offsetof(VFSlotRuntimeState, parameter8c) == 0x8c);
static_assert(offsetof(VFSlotRuntimeState, parameter8e) == 0x8e && offsetof(VFSlotRuntimeState, parameterCc) == 0xcc);
static_assert(offsetof(VFSlotRuntimeState, parameter12a) == 0x12a);
static_assert(offsetof(VFSlotRuntimeState, phases) == 0x12d);
static_assert(offsetof(VFSlotRuntimeState, secondaryMotionTimer) == 0x1b0);



struct alignas(4) VFReactionPriorityEntry {
    std::uint16_t mask, sectorMask, pressed, released;
};
struct VFReactionCursors {
    union {
        std::uint16_t endpoints;
        std::uint8_t endpoint[2];
    };
    std::uint8_t tracked[3];
    std::uint8_t deferred[2], previous;
};
union VFReactionMask {
    std::uint32_t bits;
    std::uint16_t words[2];
};
union VFReactionProgram {
    const std::uint8_t *program;
    std::uint16_t words[4];
};
#pragma pack(push, 1)
union VFReactionFilterPair {
    std::uint32_t both;
    struct {
        std::uint16_t current, previous;
    } mask;
};
union VFReactionWindow {
    std::uint32_t both;
    struct {
        std::uint16_t remaining, allowed;
    } count;
};
#pragma pack(pop)
struct VFReactionContext {
    const std::uint8_t *program;
    std::int32_t scanRemaining;
    std::uint32_t priorityOnly;
    std::uint16_t priorityFlags, priorityMask, maskSnapshot, allowedMask, remainingFrames;
    std::uint8_t historyIndex;
    std::byte reserved[5];
};
struct VFReactionState {
    std::uint8_t depth, historyIndex, stopIndex, reserved;
    std::uint16_t maskSnapshot, pressed;
    std::int32_t scanRemaining;
    std::uint32_t released;
    std::byte reservedScan[4];
    std::uint16_t releasedFlags;
    VFReactionFilterPair filters;
    VFReactionWindow window;
    std::byte reservedWindow[2];
    VFReactionContext contexts[16];
    std::uint32_t conditionFlags;
    std::byte reservedCondition[4];
    std::int8_t chainFrames;
    std::uint8_t counters[3];
    std::byte reservedCounters[4];
};
union VFReactionMovement {
    float value;
    std::uint16_t command;
    struct {
        std::uint8_t mode, phase;
        std::byte reserved[2];
    } state;
};
inline constexpr std::int32_t VF_REACTION_COUNTER_BIAS = 128;
struct VFReactionCounterOperands {
    std::byte preceding[0x1a9];
    std::uint8_t value[256];
    std::byte following[0x67];
};
struct VFReactionAssistHistory {
    float currentWindow;
    std::byte reserved[0x40];
};
static_assert(sizeof(VFReactionPriorityEntry) == 8 && alignof(VFReactionPriorityEntry) == 4);
static_assert(offsetof(VFReactionPriorityEntry, sectorMask) == 2 && offsetof(VFReactionPriorityEntry, released) == 6);
static_assert(sizeof(VFReactionCursors) == 8 && offsetof(VFReactionCursors, tracked) == 2);
static_assert(sizeof(VFReactionContext) == 32 && offsetof(VFReactionContext, priorityOnly) == 12);
static_assert(offsetof(VFReactionContext, priorityFlags) == 16 && offsetof(VFReactionContext, historyIndex) == 26);
static_assert(sizeof(VFReactionCounterOperands) == 0x310);
static_assert(sizeof(VFReactionState) == 0x230 && offsetof(VFReactionState, contexts) == 0x20);
static_assert(offsetof(VFReactionState, conditionFlags) == 0x220 && offsetof(VFReactionState, counters) == 0x229);

#pragma pack(push, 4)
struct VFActorContactState {
    union {
        struct {
            std::uint32_t priorityMasks[2];
            std::uint32_t excludedMask, pendingMask;
        };
        struct {
            std::uint32_t priorityResetPrefix;
            std::uint64_t priorityResetPair;
            std::uint32_t priorityResetSuffix;
        };
    };
    union {
        std::uint16_t flags;
        struct {
            std::uint8_t processed, linked;
        };
    };
    std::uint16_t reserved12;
    Vector3 previousSamples[10];
    union {
        std::uint32_t shapeMasks[10];
        std::uint64_t shapeMaskPairs[5];
    };
    std::uint32_t activeShapeMask, reservedB8;
    float averageGround;
    union {
        float groundSamples[4];
        std::uint64_t groundSamplePairs[2];
    };
};
#pragma pack(pop)
static_assert(sizeof(VFActorContactState) == 0xd0);
static_assert(offsetof(VFActorContactState, priorityResetPair) == 4);
static_assert(offsetof(VFActorContactState, previousSamples) == 0x14);
static_assert(offsetof(VFActorContactState, shapeMasks) == 0x8c);
static_assert(offsetof(VFActorContactState, activeShapeMask) == 0xb4);
static_assert(offsetof(VFActorContactState, averageGround) == 0xbc);
static_assert(offsetof(VFActorContactState, groundSamples) == 0xc0);
struct VFActor {
    std::uint8_t groupIndex;
    std::int8_t actionIndex;
    std::uint16_t fightStyle;
    std::uint32_t moveFlags, stateFlags, reserved0c;
    Character *character;
    Character *targetingCharacter;
    MotionWork *motion;
    VFMA_t *game;
    VFSlotRuntimeState *slot;
    VFActor *actor;
    VFActor *secondActor;
    const std::byte *motionParameters[6];
    Vector3 position, moveDelta, relativeMoveDelta;
    std::int16_t targetFacingAngle, motionTimer;
    Vector3 targetPosition;
    Vector3 motionContactSamples[10];
    Vector3 contactPosition;
    Vector3 contactSamples[6];
    Vector3 contactVectors[3];
    std::uint16_t facingAngle;
    std::int16_t previousFacingAngle;
    std::uint16_t facingYaw, temporaryFacingAngle;
    std::int16_t contactAngles[6];
    std::byte reserved1b0[4];
    std::uint16_t parameter1b4, parameter1b6, forwardIntersectionTag, backwardIntersectionTag;
    Matrix4x4 chestHitTransform, rightLegHitTransform, leftLegHitTransform;
    std::uint32_t hitBasisPrefix;
    Vector3 hitYaw;
    std::uint16_t hitBasisAngles[2];
    float hitBasisZ;
    std::uint16_t footFacingAngles[2];
    float reserved298;
    float knockbackStrength, animationScale;
    float scale, inverseScale, timeScale, knockbackAcceleration;
    std::uint8_t hitPhase, reserved2b5;
    std::uint16_t queuedReactionEvent;
    std::uint64_t previousAction, previousQueuedAction;
    std::uint8_t actionTriggerPhase, reserved2c9;
    std::uint16_t parameter2ca, currentAnimationEndFrame;
    std::int16_t requestedAnimation, parameter2d0, reactionMiddleFrame;
    std::uint16_t reactionEndFrame, parameter2d6;
    union {
        struct {
            std::int8_t animationHalfPhase, parameter2d9;
        };
        std::uint16_t animationPhaseWord;
    };
    std::int16_t facingAngleDelta;
    std::uint16_t parameter2dc, parameter2de, parameter2e0;
    std::int16_t parameter2e2;
    float groundOffset;
    std::uint8_t moveId;
    std::int8_t guardState, guardStrength;
    std::uint8_t parameter2eb;
    float impulseMagnitude;
    std::uint16_t hitParameter, hitDirection;
    Vector3 hitVector;
    const std::byte *reactionEntries;
    std::uint8_t motionContactFlags;
    std::int8_t guardHitParameter;
    std::uint16_t eventFrame30a, eventFrame30c;
    std::uint8_t parameter30e, reserved30f;
    std::uint32_t reactionEntryFlags, reactionEntryMask;
    std::uint16_t eventFrame318, parameter31a[2], parameter31e, queuedReactionAnimation;
    std::uint16_t someAnimation[2], parameter326, eventFrame328, parameter32a;
    std::uint8_t parameter32c, parameter32d, parameter32e;
    std::int8_t reactionEventCount;
    std::uint32_t motionFlags, secondaryMotionFlags;
    std::uint32_t markerMask;
    std::uint16_t parameter33c[2];
    std::int32_t pairContactPriority;
    std::uint32_t pairContactMiddleFrame, pairContactLastFrame;
    std::uint8_t parameter34c, parameter34d;
    std::uint16_t pairContactEndFrame, parameter350;
    std::byte reserved352[6];
    const std::byte *mirroredReactionProgram;
    const std::byte *reactionProgram;
    const std::byte *reactionProgramCounter;
    std::uint64_t parameter370, parameter378;
    std::uint64_t action;
    std::uint64_t deferredReaction;
    union {
        std::uint64_t queuedAction;
        struct {
            std::uint16_t queuedActionId;
            std::uint8_t queuedActionState;
            std::byte queuedActionReserved[5];
        };
    };
    std::uint32_t hitStateFlags;
    std::uint32_t savedHitStateFlags, previousHitStateFlags, copiedHitStateFlags, activeMotionFlags;
    std::uint32_t savedMotionFlags, savedSecondaryMotionFlags, reserved3b4;
    std::uint32_t previousActiveMotionFlags, savedMarkerMask;
    std::uint32_t groundContactFlags;
    std::uint32_t reactionFlags, hitFlags;
    std::uint32_t pairedContactPriority, guardContactFlags;
    float hp;
    std::int16_t initialHp;
    std::byte reserved3da[2];
    float contactDistances[8];
    float footDistance, footHeightDifference, lowestMotionContactY;
    union {
        std::uint16_t footFacingMode;
        std::uint8_t footFacingBytes[2];
    };
    std::uint16_t hitPhaseWaitFrames;
    union {
        std::uint32_t parameter40c;
        std::int16_t hitPhaseCounters[2];
        std::uint16_t hitPhaseWords[2];
    };
    std::byte reserved410;
    std::int8_t guardLevel;
    std::uint16_t parameter412;
    std::uint16_t pendingGuardDamage;
    std::byte reserved416[2];
    float forwardIntersectionDistance, backwardIntersectionDistance;
    float leftIntersectionDistance, rightIntersectionDistance, hitFacingTimer;
    std::uint16_t predictedKnockbackFrames;
    std::uint16_t impulseDelay;
    std::uint32_t reserved430;
    float hitTimeout, tickFrameTime;
    std::uint16_t hitReaction, parameter43e;
    std::uint64_t parameter440;
    std::uint16_t lastHitMove, activeContactCount, parameter44c, parameter44e;
    union {
        std::uint16_t parameter450;
        struct {
            std::uint8_t contactState, repeatedHitCount;
        };
    };
    std::uint16_t footContactState, previousFootContactState;
    std::byte reserved456[4];
    std::uint16_t parameter45a;
    std::uint16_t groundSnapMask, groundOutsideMask;
    std::uint16_t groundFlags;
    std::uint16_t groundContactMask, previousGroundContactMask;
    std::byte reserved466[0xa];
    float minimumGroundY;
    std::uint32_t groundParameter;
    float groundY, targetGroundY;
    float frontContactRange, reserved484, rearContactRange, sideContactRange;
    float contactRangeParameter, knockbackBaseParameter, contactTimer, contactParameter;
    std::uint16_t currentAnimation, reserved4a2, previousAnimation;
    std::uint16_t parameter4a6;
    float ticks;
    std::uint32_t pendingAnimation;
    float animationTime, animationTimeParameter, pairAnimationTime, pairTimeOffset;
    std::uint16_t deferredActionFrame;
    std::uint8_t processedPairs, pairState;
    std::uint16_t parameter4c4, parameter4c6;
    float parameter4c8;
    Vector3 knockbackVelocity, slideVelocity, pendingKnockback, frameMoveDelta, parameter4fc;
    std::uint64_t reactionOverrideActive;
    std::uint16_t cameraControlFlags, priorityMask, followupFlags, priorityFlags;
    VFReactionPriorityEntry reactionHistory[32];
    VFReactionCursors reactionCursors;
    union {
        std::uint16_t deferredPriorityActive;
        std::uint8_t deferredPriorityState[2];
    };
    std::uint16_t deferredPriorityFlags[2];
    std::uint8_t reactionDelayActive;
    std::byte reserved627;
    VFReactionWindow reactionDelay;
    std::byte reserved62c[0xc];
    const std::uint8_t *reactionContinuation;
    const std::uint8_t *reactionPc;
    VFReactionProgram savedRootReaction;
    std::uint16_t savedRootReactionTicks;
    std::uint16_t pendingHitMove;
    union {
        std::byte reserved654[4];
        std::uint16_t pendingReactionTicks;
    };
    union {
        std::uint64_t pendingHitAction;
        const std::uint8_t *pendingReactionProgram;
    };
    union {
        std::uint64_t currentHitAction;
        std::uint8_t currentHitActionBytes[8];
    };
    union {
        std::uint64_t savedHitAction;
        const std::uint8_t *savedHitReactionProgram;
    };
    union {
        struct {
            VFReactionState reactions;
            std::uint8_t reactionRank;
            std::byte reserved8a1[3];
            VFReactionMask reactionOverrideFlags, reactionMaskA, reactionMaskB;
            VFActorContactState contact;
        };
        VFReactionCounterOperands counterOperands;
    };
    float parameter980;
    union {
        float reactionDepth;
        std::uint32_t reactionDepthBits;
    };
    union {
        float parameter988;
        std::uint32_t hitReactionSelection;
    };
    std::byte reservedImpulse[4];
    float hitImpulseMultiplier, hitImpulseAngle, hitImpulseMagnitude;
    union {
        float reactionPower;
        std::uint8_t reactionVariantBytes[4];
        std::uint16_t reactionVariantWords[2];
        struct {
            std::int16_t hitImpulsePitch;
            std::uint8_t hitImpulseSuppressed;
            std::int8_t hitImpulseMove;
        };
    };
    union {
        std::uint32_t packedHitImpulse;
        std::uint8_t reactionImpulseBytes[4];
    };
    union {
        std::int16_t knockbackAngle;
        std::uint8_t knockbackAngleBytes[2];
    };
    std::int8_t suppressKnockback;
    std::uint8_t fighterRank;
    float groundHeights[7];
    float previousContactHeights[7];
    std::uint32_t nearbyGroundActors[7];
    float groundHeightDifference;
    std::int32_t primaryGroundTarget;
    float previousGroundSamples[7], targetGroundSamples[7];
    float groundReactionScale;
    std::uint32_t actorRuntimeFlags, reservedA44;
    std::int32_t formationFlags;
    std::uint32_t formationRuntimeFlags;
    std::uint16_t formationEntry;
    std::int16_t formationHpFloor;
    union {
        float formationCameraTimer;
        std::int16_t formationCameraTimerWords[2];
    };
    std::uint16_t spawnYaw[2], sfxPeriod, reservedA5e;
    union {
        float moveAngleTableIndex;
        float idleInputFrames;
    };
    union {
        struct {
            std::uint16_t parameterA64;
            std::uint8_t selectFlags, parameterA67;
        };
        float noAttackInputFrames;
    };
    std::uint8_t followupState, forcedTurnState;
    std::uint8_t selectionPriority;
    std::uint8_t nearestSpawnPoint;
    union {
        struct {
            std::uint8_t parameterA6c, parameterA6d;
        };
        std::int16_t reactionFacingAngleDelta;
    };
    union {
        std::uint16_t previousCameraControlFlags;
        std::uint8_t relativeInputSectors[2];
    };
    std::uint8_t cameraMotionFlags;
    std::uint8_t previousCameraMotionFlags;
    union {
        std::uint16_t previousReactionInput;
        std::uint8_t previousReactionInputBytes[2];
    };
    union {
        float parameterA74;
        VFReactionMovement reactionMovement;
    };
    float parameterA78;
    std::uint32_t parameterA7c, parameterA80;
    float previousAssistWindow;
    union {
        struct {
            std::uint32_t parameterA88, parameterA8c;
        };
        std::uint64_t reactionAssistScratch;
    };
    union {
        std::byte reservedA90[0x44];
        VFReactionAssistHistory assistHistory;
    };
    std::int8_t targetCache;
    std::int8_t cooldownQuadrant;
    std::int8_t previousCooldownQuadrant, parameterAd7;
    const std::byte *parameterAd8;
    std::uint64_t parameterAe0;
    std::int16_t contactFacing, reservedAea, alternateContactFacing;
    std::uint16_t defaultAnimations[3];
    union {
        std::byte otherActorStorage[2];
        std::uint8_t reactionEvaluationPhase;
    };
    std::int8_t pairedSlot;
    std::byte pairedSlotReserved;
    Character *pairedCharacter;
    VFActor *pairedActor;
    union {
        std::byte reservedB08[8];
        VFActor *hitSourceActor;
    };
    Character *reactionCharacter;
    VFGW_t *gauge;
    union {
        std::uint32_t parameterB20;
        float runningInputFrames;
    };
    union {
        float parameterB24;
        float runningCooldownFrames;
    };
    std::uint32_t hitResolutionFlags;
    float parameterB2c;
    float guardRateScale;
    float guardHp, guardMaximumHp, regenerationRate, focusedRegenerationScale;
    std::uint32_t reservedB44;
};
static_assert(offsetof(VFActor, reactionHistory) == 0x518);
static_assert(offsetof(VFActor, reactionCursors) == 0x618);
static_assert(offsetof(VFActor, deferredPriorityState) == 0x620);
static_assert(offsetof(VFActor, deferredPriorityFlags) == 0x622);
static_assert(offsetof(VFActor, reactionDelayActive) == 0x626);
static_assert(offsetof(VFActor, reactionDelay) == 0x628);
static_assert(offsetof(VFActor, reactionContinuation) == 0x638);
static_assert(offsetof(VFActor, reactionPc) == 0x640);
static_assert(offsetof(VFActor, savedRootReaction) == 0x648);
static_assert(offsetof(VFActor, savedRootReactionTicks) == 0x650);
static_assert(offsetof(VFActor, pendingReactionTicks) == 0x654);
static_assert(offsetof(VFActor, pendingReactionProgram) == 0x658);
static_assert(offsetof(VFActor, reactions) == 0x670);
static_assert(offsetof(VFActor, reactionOverrideFlags) == 0x8a4);
static_assert(offsetof(VFActor, reactionMaskA) == 0x8a8);
static_assert(offsetof(VFActor, reactionMaskB) == 0x8ac);
static_assert(offsetof(VFActor, reactionVariantBytes) == 0x99c);
static_assert(offsetof(VFActor, reactionImpulseBytes) == 0x9a0);
static_assert(offsetof(VFActor, idleInputFrames) == 0xa60);
static_assert(offsetof(VFActor, noAttackInputFrames) == 0xa64);
static_assert(offsetof(VFActor, relativeInputSectors) == 0xa6e);
static_assert(offsetof(VFActor, previousReactionInput) == 0xa72);
static_assert(offsetof(VFActor, reactionMovement) == 0xa74);
static_assert(offsetof(VFActor, reactionAssistScratch) == 0xa88);
static_assert(offsetof(VFActor, assistHistory) == 0xa90);
static_assert(offsetof(VFActor, reactionEvaluationPhase) == 0xaf4);
static_assert(offsetof(VFActor, reactionCharacter) == 0xb10);
static_assert(offsetof(VFActor, runningInputFrames) == 0xb20);
static_assert(offsetof(VFActor, runningCooldownFrames) == 0xb24);
static_assert(offsetof(VFActor, counterOperands) + offsetof(VFReactionCounterOperands, value) + VF_REACTION_COUNTER_BIAS == 0x899);
static_assert(sizeof(VFActor) == 0xb48);
static_assert(offsetof(VFActor, queuedActionState) == 0x392);
static_assert(offsetof(VFActor, pendingGuardDamage) == 0x414);
static_assert(offsetof(VFActor, repeatedHitCount) == 0x451);
static_assert(offsetof(VFActor, groundSnapMask) == 0x45c);
static_assert(offsetof(VFActor, groundOutsideMask) == 0x45e);
static_assert(offsetof(VFActor, groundContactMask) == 0x462);
static_assert(offsetof(VFActor, previousGroundContactMask) == 0x464);
static_assert(offsetof(VFActor, contact) == 0x8b0);
static_assert(offsetof(VFActor, reactionDepthBits) == 0x984);
static_assert(offsetof(VFActor, hitReactionSelection) == 0x988);
static_assert(offsetof(VFActor, hitImpulseMultiplier) == 0x990);
static_assert(offsetof(VFActor, hitImpulseAngle) == 0x994);
static_assert(offsetof(VFActor, hitImpulseMagnitude) == 0x998);
static_assert(offsetof(VFActor, hitImpulsePitch) == 0x99c);
static_assert(offsetof(VFActor, hitImpulseSuppressed) == 0x99e);
static_assert(offsetof(VFActor, hitImpulseMove) == 0x99f);
static_assert(offsetof(VFActor, reactionFacingAngleDelta) == 0xa6c);
static_assert(offsetof(VFActor, previousCameraMotionFlags) == 0xa71);
static_assert(offsetof(VFActor, hitSourceActor) == 0xb08);
static_assert(offsetof(VFActor, actor) == 0x38);
static_assert(offsetof(VFActor, animationScale) == 0x2a0);
static_assert(offsetof(VFActor, previousAnimation) == 0x4a4);
static_assert(offsetof(VFActor, forwardIntersectionTag) == 0x1b8);
static_assert(offsetof(VFActor, backwardIntersectionTag) == 0x1ba);
static_assert(offsetof(VFActor, pairAnimationTime) == 0x4b8);
static_assert(offsetof(VFActor, groundReactionScale) == 0xa3c);
static_assert(offsetof(VFActor, targetingCharacter) == 0x18);
static_assert(offsetof(VFActor, parameter1b6) == 0x1b6);
static_assert(offsetof(VFActor, motionTimer) == 0x9e);
static_assert(offsetof(VFActor, previousHitStateFlags) == 0x3a0);
static_assert(offsetof(VFActor, initialHp) == 0x3d8);
static_assert(offsetof(VFActor, ticks) == 0x4a8);
static_assert(offsetof(VFActor, parameter4c4) == 0x4c4);
static_assert(offsetof(VFActor, followupFlags) == 0x514);
static_assert(offsetof(VFActor, guardHp) == 0xb34);
static_assert(offsetof(VFActor, guardMaximumHp) == 0xb38);
static_assert(offsetof(VFActor, regenerationRate) == 0xb3c);
static_assert(offsetof(VFActor, focusedRegenerationScale) == 0xb40);
static_assert(offsetof(VFActor, reactionProgramCounter) == 0x368);
static_assert(offsetof(VFActor, reactionRank) == 0x8a0);
static_assert(offsetof(VFActor, previousActiveMotionFlags) == 0x3b8);
static_assert(offsetof(VFActor, moveDelta) == 0x84);
static_assert(offsetof(VFActor, relativeMoveDelta) == 0x90);
static_assert(offsetof(VFActor, targetPosition) == 0xa0);
static_assert(offsetof(VFActor, contactVectors) == 0x178);
static_assert(offsetof(VFActor, facingYaw) == 0x1a0);
static_assert(offsetof(VFActor, queuedReactionEvent) == 0x2b6);
static_assert(offsetof(VFActor, queuedReactionAnimation) == 0x320);
static_assert(offsetof(VFActor, pairContactMiddleFrame) == 0x344);
static_assert(offsetof(VFActor, pairContactLastFrame) == 0x348);
static_assert(offsetof(VFActor, deferredReaction) == 0x388);
static_assert(offsetof(VFActor, reserved3b4) == 0x3b4);
static_assert(offsetof(VFActor, reactionFlags) == 0x3c4);
static_assert(offsetof(VFActor, guardContactFlags) == 0x3d0);
static_assert(offsetof(VFActor, hitFacingTimer) == 0x428);
static_assert(offsetof(VFActor, hitTimeout) == 0x434);
static_assert(offsetof(VFActor, parameter43e) == 0x43e);
static_assert(offsetof(VFActor, footContactState) == 0x452);
static_assert(offsetof(VFActor, previousFootContactState) == 0x454);
static_assert(offsetof(VFActor, parameter45a) == 0x45a);
static_assert(offsetof(VFActor, minimumGroundY) == 0x470);
static_assert(offsetof(VFActor, groundParameter) == 0x474);
static_assert(offsetof(VFActor, groundY) == 0x478);
static_assert(offsetof(VFActor, frontContactRange) == 0x480);
static_assert(offsetof(VFActor, reserved484) == 0x484);
static_assert(offsetof(VFActor, rearContactRange) == 0x488);
static_assert(offsetof(VFActor, sideContactRange) == 0x48c);
static_assert(offsetof(VFActor, contactRangeParameter) == 0x490);
static_assert(offsetof(VFActor, pendingHitMove) == 0x652);
static_assert(offsetof(VFActor, currentHitAction) == 0x660);
static_assert(offsetof(VFActor, reactionDepth) == 0x984);
static_assert(offsetof(VFActor, previousContactHeights) == 0x9c4);
static_assert(offsetof(VFActor, actorRuntimeFlags) == 0xa40);
static_assert(offsetof(VFActor, parameterA64) == 0xa64);
static_assert(offsetof(VFActor, selectFlags) == 0xa66);
static_assert(offsetof(VFActor, parameterA67) == 0xa67);
static_assert(offsetof(VFActor, nearestSpawnPoint) == 0xa6b);
static_assert(offsetof(VFActor, parameterA74) == 0xa74);
static_assert(offsetof(VFActor, parameterA88) == 0xa88);
static_assert(offsetof(VFActor, parameterA8c) == 0xa8c);
static_assert(offsetof(VFActor, reservedA90) == 0xa90);
static_assert(offsetof(VFActor, previousCooldownQuadrant) == 0xad6);
static_assert(offsetof(VFActor, parameterAd7) == 0xad7);
static_assert(offsetof(VFActor, parameterAe0) == 0xae0);
static_assert(offsetof(VFActor, otherActorStorage) == 0xaf4);
static_assert(offsetof(VFActor, parameterB20) == 0xb20);
static_assert(offsetof(VFActor, parameter2ca) == 0x2ca);
static_assert(offsetof(VFActor, animationHalfPhase) == 0x2d8);
static_assert(offsetof(VFActor, parameter2d9) == 0x2d9);
static_assert(offsetof(VFActor, parameter2dc) == 0x2dc);
static_assert(offsetof(VFActor, parameter2de) == 0x2de);
static_assert(offsetof(VFActor, parameter2e2) == 0x2e2);
static_assert(offsetof(VFActor, parameter2eb) == 0x2eb);
static_assert(offsetof(VFActor, impulseMagnitude) == 0x2ec);
static_assert(offsetof(VFActor, hitVector) == 0x2f4);
static_assert(offsetof(VFActor, reactionEntries) == 0x300);
static_assert(offsetof(VFActor, motionContactFlags) == 0x308);
static_assert(offsetof(VFActor, eventFrame30a) == 0x30a);
static_assert(offsetof(VFActor, eventFrame30c) == 0x30c);
static_assert(offsetof(VFActor, eventFrame318) == 0x318);
static_assert(offsetof(VFActor, parameter31a) == 0x31a);
static_assert(offsetof(VFActor, parameter31e) == 0x31e);
static_assert(offsetof(VFActor, someAnimation) == 0x322);
static_assert(offsetof(VFActor, eventFrame328) == 0x328);
static_assert(offsetof(VFActor, parameter32a) == 0x32a);
static_assert(offsetof(VFActor, parameter32d) == 0x32d);
static_assert(offsetof(VFActor, parameter32e) == 0x32e);
static_assert(offsetof(VFActor, reactionEventCount) == 0x32f);
static_assert(offsetof(VFActor, markerMask) == 0x338);
static_assert(offsetof(VFActor, parameter33c) == 0x33c);
static_assert(offsetof(VFActor, pairContactEndFrame) == 0x34e);
static_assert(offsetof(VFActor, mirroredReactionProgram) == 0x358);
static_assert(offsetof(VFActor, reactionProgram) == 0x360);
static_assert(offsetof(VFActor, savedHitStateFlags) == 0x39c);
static_assert(offsetof(VFActor, savedMotionFlags) == 0x3ac);
static_assert(offsetof(VFActor, savedSecondaryMotionFlags) == 0x3b0);
static_assert(offsetof(VFActor, savedMarkerMask) == 0x3bc);
static_assert(offsetof(VFActor, contactDistances) == 0x3dc);
static_assert(offsetof(VFActor, parameter412) == 0x412);
static_assert(offsetof(VFActor, impulseDelay) == 0x42e);
static_assert(offsetof(VFActor, animationTimeParameter) == 0x4b4);
static_assert(offsetof(VFActor, scale) == 0x2a4);
static_assert(offsetof(VFActor, timeScale) == 0x2ac);
static_assert(offsetof(VFActor, knockbackAcceleration) == 0x2b0);
static_assert(offsetof(VFActor, actionTriggerPhase) == 0x2c8);
static_assert(offsetof(VFActor, requestedAnimation) == 0x2ce);
static_assert(offsetof(VFActor, reactionEndFrame) == 0x2d4);
static_assert(offsetof(VFActor, parameter2d6) == 0x2d6);
static_assert(offsetof(VFActor, groundOffset) == 0x2e4);
static_assert(offsetof(VFActor, guardHitParameter) == 0x309);
static_assert(offsetof(VFActor, parameter350) == 0x350);
static_assert(offsetof(VFActor, pairedContactPriority) == 0x3cc);
static_assert(offsetof(VFActor, parameter40c) == 0x40c);
static_assert(offsetof(VFActor, predictedKnockbackFrames) == 0x42c);
static_assert(offsetof(VFActor, contactTimer) == 0x498);
static_assert(offsetof(VFActor, contactParameter) == 0x49c);
static_assert(offsetof(VFActor, pendingAnimation) == 0x4ac);
static_assert(offsetof(VFActor, parameter4c8) == 0x4c8);
static_assert(offsetof(VFActor, slideVelocity) == 0x4d8);
static_assert(offsetof(VFActor, packedHitImpulse) == 0x9a0);
static_assert(offsetof(VFActor, cameraMotionFlags) == 0xa70);
static_assert(offsetof(VFActor, parameterA78) == 0xa78);
static_assert(offsetof(VFActor, parameterA80) == 0xa80);
static_assert(offsetof(VFActor, targetCache) == 0xad4);
static_assert(offsetof(VFActor, contactFacing) == 0xae8);
static_assert(offsetof(VFActor, alternateContactFacing) == 0xaec);
static_assert(offsetof(VFActor, defaultAnimations) == 0xaee);
static_assert(offsetof(VFActor, parameterB24) == 0xb24);
static_assert(offsetof(VFActor, parameterB2c) == 0xb2c);
static_assert(offsetof(VFActor, actionIndex) == 0x1);
static_assert(offsetof(VFActor, motionParameters) == 0x48);
static_assert(offsetof(VFActor, cameraControlFlags) == 0x510);
static_assert(offsetof(VFActor, groupIndex) == 0x0);
static_assert(offsetof(VFActor, character) == 0x10);
static_assert(offsetof(VFActor, game) == 0x28);
static_assert(offsetof(VFActor, secondActor) == 0x40);
static_assert(offsetof(VFActor, motionContactSamples) == 0xac);
static_assert(offsetof(VFActor, parameter2e0) == 0x2e0);
static_assert(offsetof(VFActor, parameterAd8) == 0xad8);
static_assert(offsetof(VFActor, pairContactPriority) == 0x340);
static_assert(offsetof(VFActor, currentAnimation) == 0x4a0);
static_assert(offsetof(VFActor, reactionPower) == 0x99c);
static_assert(offsetof(VFActor, selectionPriority) == 0xa6a);
static_assert(offsetof(VFActor, guardRateScale) == 0xb30);
static_assert(offsetof(VFActor, contactPosition) == 0x124);
static_assert(offsetof(VFActor, knockbackStrength) == 0x29c);
static_assert(offsetof(VFActor, hitStateFlags) == 0x398);
static_assert(offsetof(VFActor, formationFlags) == 0xa48);
static_assert(offsetof(VFActor, spawnYaw) == 0xa58);
static_assert(offsetof(VFActor, sfxPeriod) == 0xa5c);
static_assert(offsetof(VFActor, moveAngleTableIndex) == 0xa60);
static_assert(offsetof(VFActor, hitPhase) == 0x2b4);
static_assert(offsetof(VFActor, previousAction) == 0x2b8);
static_assert(offsetof(VFActor, currentAnimationEndFrame) == 0x2cc);
static_assert(offsetof(VFActor, action) == 0x380);
static_assert(offsetof(VFActor, queuedAction) == 0x390);
static_assert(offsetof(VFActor, tickFrameTime) == 0x438);
static_assert(offsetof(VFActor, animationTime) == 0x4b0);
static_assert(offsetof(VFActor, hitResolutionFlags) == 0xb28);

// g_vfPreUpdateCallbacks rows: vfDispatchCurrentActionState calls clear, vfExecuteDynamicActionFuncs calls update.
struct VFActionCallbacks {
    void (*clear)(VFActor *);
    void (*update)(VFActor *);
};

struct VFCollisionShape {
    Vector3 point;
    float radius;
    std::uint8_t bone, reserved11;
    std::uint16_t tag;
};
static_assert(sizeof(VFCollisionShape) == 20);
struct VFCLContactSample {
    Vector3 points[3];
};
struct VFCL_t {
    shen_name name;
    std::uint32_t reserved04;
    VFMA_t *game;
    VFActor *defender;
    VFActor *attacker;
    union {
        std::uint32_t flagsWord;
        struct {
            std::uint8_t flags;
            std::byte reserved21[3];
        };
    };
    std::uint32_t contactType, maskA, maskB;
    float slideScale, stickScale;
    std::uint32_t attackerMask, defenderMask;
    float penetrationCharacter, penetrationSpecial, separationLength;
    Vector3 separation;
    VFCLContactSample samples[6], previousSamples[6];
    std::uint32_t contactMasks[6], ringMasks[6];
    Vector2 contactDeltas[6];
    std::uint32_t runtimeFlags, reserved26c;
};
static_assert(sizeof(VFCL_t) == 0x270);
static_assert(offsetof(VFCL_t, samples) == 0x58);
static_assert(offsetof(VFCL_t, previousSamples) == 0x130);
static_assert(offsetof(VFCL_t, contactMasks) == 0x208);
static_assert(offsetof(VFCL_t, contactDeltas) == 0x238);
static_assert(offsetof(VFCL_t, runtimeFlags) == 0x268);

void vfclTransformContactShapes(VFActor *actor);
void vfInitializeActorCollision(VFMA_t *game, std::uint8_t index);
void vfclMoveEntityWithContactHistory(VFActor *actor, float x, float z);
void vfclAdjustToGroundOrSlope(VFCL_t *collision);
void vfclUpdateHeightFlagsForGrid(VFActor *actor);
void vfClCopyMotionData(VFMA_t *game);
void vfclComputePenetrationAndOverlap(VFCL_t *collision, VFActor *defender, VFActor *attacker);
void vfclAdjustForSlideAndStick(VFCL_t *collision);
void vfclSetContactCategory(VFActor *defender, VFActor *attacker, VFCL_t *collision);
void vfclPrepareContact(VFCL_t *collision, std::uint8_t first, std::uint8_t second);
void vfClGetBonePos(std::uint8_t index, VFActor *first, VFActor *second);
bool vfClGetBoneStates(std::uint8_t first, std::uint8_t second, VFCL_t *collision);
float vfResolveHitOutcomeAndImpulse(VFActor *self, VFActor *other, float impact);
void vfApplyEnvAudio(float impact, VFActor *actor);
void vfSetReactionKnockbackFromProfile(VFActor *self, VFActor *other, float impulse);
void vfStageObjectApplyImpulse(VFActor *actor);
void vfAssignKnockbackReaction(float impact, float impulse, VFActor *self, VFActor *other);
void vfclApplyHitResolution(VFActor *self, VFActor *other);
struct VFCLParticipantTmp {
    std::uint8_t type, index, link, reserved03;
    std::uint32_t reserved04;
    void *owner;
    float x, z;
};
static_assert(sizeof(VFCLParticipantTmp) == 24);
void vfBuildContactsAndResolvePushOrHit(VFCL_t *collision, VFCLParticipantTmp *participants, std::int32_t count, std::uint8_t phase);
void vfRob_ResolveHitClash(VFCL_t *collision, std::uint8_t first, std::uint8_t second);
void vfc_VF_RobCollision_Callback(void *collision);
HLib::Task *Queue_vfc_VF_RobCollision(VFMA_t *game);



struct VFHD_t {
    VFMA_t *game;
    std::byte instructionStorage[0x478];
    Character *character;
    Character *secondCharacter;
    std::uint32_t requestedMotion, secondRequestedMotion;
    std::uint32_t currentMotion, secondCurrentMotion;
    std::byte reserved4a0[8];
    std::byte *result;
    std::uint16_t programIndices[2];
    std::uint32_t frame;
    std::byte reserved4b8[0x68];
};
static_assert(sizeof(VFHD_t) == 0x520);
static_assert(offsetof(VFHD_t, character) == 0x480);
static_assert(offsetof(VFHD_t, result) == 0x4a8);

std::byte *vfSetMoveInstructionConfiguration(
    shen_name character, shen_name secondCharacter, std::int16_t program, std::int16_t secondProgram, std::byte *borrowedResult);
VFHD_t *AllocateVFHD(VFMA_t *game);
void vfFreeVFHDSpace(VFMA_t *game);
std::int32_t vfApplyMoveInstructionConfiguration(VFHD_t *owner);
void vfUpdateVFHDState(VFActor *actor);

struct VFMA_t {
    shen_name name;
    std::uint32_t parameters;
    HLib::Task *task;
    HLib::Task *collisionTask;
    HLib::Task *gameInfoTask;
    std::uint64_t reserved20;
    std::uint32_t mode, reserved2c, savedUiState;
    std::int32_t state;
    float previousFrame, frame, timedBattleTime, blurAmount, hpTimer;
    std::uint32_t reserved4c;
    float knockbackAmount;
    std::uint32_t gameFlags;
    float guardTimer;
    std::byte reserved5c[4];
    std::uint8_t hitStopFrames, type;
    std::byte reserved62[2];
    float timeParameter, gravityScale, dragScale, slideDragScale;
    std::uint32_t reserved74;
    float floorIntersectionY, floorSnapY;
    float cameraResponse, playerCameraScale, playerAssistScale, contactRange;
    Character *playerCharacter;
    std::int8_t playerSlot;
    std::uint8_t opponentSlot;
    std::uint8_t remainingEnemies;
    std::byte slotParameters[0x15];
    Character *characters[6];
    std::byte reservedE0[0x10];
    FCMG_t *enemySlots[6];
    std::byte reserved120[0x10];
    FRMG_t *groups[6];
    std::byte reserved160[0x10];
    std::uint16_t participantParameters[4];
    VFActor *actors[6];
    std::byte reserved1a8[0x10];
    VFSlotRuntimeState *actorSlots[6];
    std::byte reserved1e8[0x10];
    VFCL_t *collision;
    PadControlEntry *savedInput;
    PadControlEntry *inputBuffer;
    std::byte reserved210[0x20];
    std::int32_t guardEffectSeconds;
    std::byte reserved234[0xc];
    std::uint8_t slotPoolModes[8], slotTeams[6];
    std::byte reserved24e[0x1c];
    std::uint8_t maximumActiveEnemies;
    std::byte reserved26b[0x29];
    float parameter294, parameter298;
    std::byte reserved29c[0x1b];
    std::uint8_t inputLockOnStart, cameraSet, parameter2b9, strikesLeft;
    std::byte reserved2bb[5];
    std::uint64_t parameter2c0, parameter2c8;
    std::byte reserved2d0[0x24];
    float spawnPointParameters[5];
    std::int8_t spawnPointCount;
    std::uint8_t spriteDepth;
    std::byte reserved30a[6];
    VFHD_t *dialogue;
    std::byte reserved318[4];
    float frameDelta, timer320;
    std::byte reserved324[5];
    std::uint8_t savedHelpTopic;
    std::byte reserved32a[2];
    std::uint32_t counter32c;
    std::byte collisionHistory[0x200];
    float parameter530;
    std::int16_t otherFacingYaw, facingParameter;
    float frame3, cameraTimer, targetTime;
    VCDW_t *cameraDialogue;
    std::int16_t perspective, savedPerspective;
    std::int8_t cameraParameter;
    std::byte reserved555[3];
    float cameraDistance, cameraHeight;
    std::byte reserved560[0x10];
    float parameter570;
    std::int32_t parameter574;
};
static_assert(sizeof(VFMA_t) == 0x578);
static_assert(offsetof(VFMA_t, knockbackAmount) == 0x50 && offsetof(VFMA_t, gravityScale) == 0x68);
static_assert(offsetof(VFMA_t, playerAssistScale) == 0x88 && offsetof(VFMA_t, contactRange) == 0x8c && offsetof(VFMA_t, playerCharacter) == 0x90);
static_assert(offsetof(VFMA_t, type) == 0x61 && offsetof(VFMA_t, floorIntersectionY) == 0x78 && offsetof(VFMA_t, cameraParameter) == 0x554);
static_assert(offsetof(VFMA_t, guardTimer) == 0x58 && offsetof(VFMA_t, hitStopFrames) == 0x60 && offsetof(VFMA_t, playerCameraScale) == 0x84);
static_assert(offsetof(VFMA_t, groups) == 0x130 && offsetof(VFMA_t, spawnPointParameters) == 0x2f4 && offsetof(VFMA_t, spawnPointCount) == 0x308);
static_assert(offsetof(VFMA_t, state) == 0x34);
static_assert(offsetof(VFMA_t, slotPoolModes) == 0x240);
static_assert(offsetof(VFMA_t, mode) == 0x28);
static_assert(offsetof(VFMA_t, reserved2c) == 0x2c);
static_assert(offsetof(VFMA_t, previousFrame) == 0x38);
static_assert(offsetof(VFMA_t, frame) == 0x3c);
static_assert(offsetof(VFMA_t, timedBattleTime) == 0x40);
static_assert(offsetof(VFMA_t, blurAmount) == 0x44);
static_assert(offsetof(VFMA_t, hpTimer) == 0x48);
static_assert(offsetof(VFMA_t, gameFlags) == 0x54);
static_assert(offsetof(VFMA_t, guardTimer) == 0x58);
static_assert(offsetof(VFMA_t, hitStopFrames) == 0x60);
static_assert(offsetof(VFMA_t, savedInput) == 0x200);
static_assert(offsetof(VFMA_t, inputBuffer) == 0x208);
static_assert(offsetof(VFMA_t, maximumActiveEnemies) == 0x26a);
static_assert(offsetof(VFMA_t, timer320) == 0x320);
static_assert(offsetof(VFMA_t, counter32c) == 0x32c);
static_assert(offsetof(VFMA_t, parameter530) == 0x530);
static_assert(offsetof(VFMA_t, targetTime) == 0x540);
static_assert(offsetof(VFMA_t, perspective) == 0x550);
static_assert(offsetof(VFMA_t, parameter570) == 0x570);
static_assert(offsetof(VFMA_t, type) == 0x61);
static_assert(offsetof(VFMA_t, playerSlot) == 0x98);
static_assert(offsetof(VFMA_t, characters) == 0xb0);
static_assert(offsetof(VFMA_t, actors) == 0x178);
static_assert(offsetof(VFMA_t, spriteDepth) == 0x309);
static_assert(offsetof(VFMA_t, actorSlots) == 0x1b8);
static_assert(offsetof(VFMA_t, slotTeams) == 0x248);
static_assert(offsetof(VFMA_t, dialogue) == 0x310);
static_assert(offsetof(VFMA_t, cameraDialogue) == 0x548);
static_assert(offsetof(VFMA_t, savedHelpTopic) == 0x329);
static_assert(offsetof(VFActor, position) == 0x78);
static_assert(offsetof(VFActor, motionFlags) == 0x330);
static_assert(offsetof(VFActor, hp) == 0x3d4);
static_assert(offsetof(VFActor, pairedSlot) == 0xaf6);
static_assert(offsetof(VFActor, pairedActor) == 0xb00);
static_assert(offsetof(VFActor, gauge) == 0xb18);
static_assert(offsetof(VFActor, facingAngleDelta) == 0x2da);
static_assert(offsetof(VFActor, groundFlags) == 0x460);
static_assert(offsetof(VFActor, groundHeights) == 0x9a8);
static_assert(offsetof(VFActor, nearbyGroundActors) == 0x9e0);
static_assert(offsetof(VFActor, previousGroundSamples) == 0xa04);
static_assert(offsetof(VFActor, cooldownQuadrant) == 0xad5);

#pragma pack(push, 4)
struct VFGW_t {
    float displayedHp, targetHp, gaugeTime, animationTime;
    std::int32_t state;
    std::uint32_t x, dimensions, colours;
    std::uint16_t sprite;
    std::uint8_t animation, reserved23;
    std::uint64_t animationParameters;
    std::uint32_t layout, scale, alpha;
    std::uint8_t active;
    std::byte reserved39[3];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct VCDS_t {
    std::uint8_t state, condition;
    std::uint16_t flags;
    std::int16_t occurrences;
    std::uint16_t reserved06;
    std::uint32_t id, idleFrames;
    VCDS_t *previous;
    VCDS_t *next;
    std::int64_t (*callback)(void *);
    std::uint16_t priority;
    std::int16_t dialogue, previousDialogue;
    std::uint16_t callbackArgument;
    shen_name character, otherCharacter, lastCharacter;
    float probability;
    std::int16_t repeatEvery;
    std::uint16_t completions;
    std::uint32_t period;
    std::uint16_t animation, previousAnimation, health;
    std::byte reserved4e[2];
    float healthPercent, distance;
    std::uint32_t reactionMask, reserved5c;
};
struct VCDW_t {
    std::uint8_t state, reserved01;
    std::uint32_t flags;
    std::byte reserved06[2];
    VCDS_t *records;
    std::uint64_t reserved10, enabled;
    std::uint32_t counter;
    std::int16_t selected;
    std::uint16_t parameters;
    std::byte reserved28[0x38];
};
#pragma pack(pop)
static_assert(sizeof(VCDS_t) == 0x60);
static_assert(offsetof(VCDS_t, next) == 0x18);
std::uint32_t vfRegisterCameraDialogue(VCDS_t *record);
void vfStartCameraDialogue(VFMA_t *game, std::uint32_t id);
void vfSetFightDialogue(VFMA_t *game, std::int16_t index);
std::uint32_t AllocateBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::int16_t repeatEvery,
    float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition2(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float healthPercent, float probability);
std::uint32_t AllocateBattleDialogueCondition3(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t health, float probability);
std::uint32_t AllocateBattleDialogueCondition5(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition6(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition9(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float distance, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition12(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition16(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateMotionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t animation,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition7(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateReactionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::uint32_t reactionMask, std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition15(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition10(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition13(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocatePeriodicBattleDialogue(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition11(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition17(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
VCDS_t *SetBattleDialogueFlags(std::uint32_t id, std::uint16_t flags);
VCDS_t *AssignBattleDialogueCallback(std::uint32_t id, std::int64_t (*callback)(void *));
VCDS_t *SuspendBattleDialogue(std::uint32_t id);
VCDS_t *ResumeBattleDialogue(std::uint32_t id);
std::int64_t QueueBattleDialogue(std::int16_t priority, std::int16_t dialogue);
void BeginFightDialogueRetirement();
std::uint64_t GetFightCharacterFlag(std::uint8_t mode, Character *character, float value);
std::uint32_t GetFightTaskFlags();
std::uint64_t GetFightCounterHighWord();
std::uint64_t GetFightPlayerGuardEffectTimerSeconds();
std::uint32_t GetFightActorFlags(Character *character);
std::uint64_t GetBattleDialogueCompletionCount(std::uint32_t id);
std::uint32_t SetFightFlag400000(std::int32_t enabled);
static_assert(sizeof(VFGW_t) == 0x3c);
static_assert(sizeof(VCDW_t) == 0x60);
static_assert(offsetof(VCDW_t, flags) == 2);
static_assert(offsetof(VCDW_t, records) == 8);
static_assert(offsetof(VCDW_t, selected) == 0x24);
static_assert(offsetof(VFActor, moveId) == 0x2e8);
static_assert(offsetof(VFActor, targetGroundY) == 0x47c);
static_assert(offsetof(VFActor, groundContactFlags) == 0x3c0);
static_assert(offsetof(VFActor, fightStyle) == 0x2);
static_assert(offsetof(VFActor, moveFlags) == 0x4);
static_assert(offsetof(VFActor, stateFlags) == 0x8);
static_assert(offsetof(VFActor, facingAngle) == 0x19c);
static_assert(offsetof(VFActor, temporaryFacingAngle) == 0x1a2);
static_assert(offsetof(VFActor, contactAngles) == 0x1a4);
static_assert(offsetof(VFActor, parameter1b4) == 0x1b4);
static_assert(offsetof(VFActor, inverseScale) == 0x2a8);
static_assert(offsetof(VFActor, parameter2d0) == 0x2d0);
static_assert(offsetof(VFActor, reactionMiddleFrame) == 0x2d2);
static_assert(offsetof(VFActor, guardState) == 0x2e9);
static_assert(offsetof(VFActor, guardStrength) == 0x2ea);
static_assert(offsetof(VFActor, hitParameter) == 0x2f0);
static_assert(offsetof(VFActor, hitDirection) == 0x2f2);
static_assert(offsetof(VFActor, parameter30e) == 0x30e);
static_assert(offsetof(VFActor, reactionEntryFlags) == 0x310);
static_assert(offsetof(VFActor, reactionEntryMask) == 0x314);
static_assert(offsetof(VFActor, parameter32c) == 0x32c);
static_assert(offsetof(VFActor, secondaryMotionFlags) == 0x334);
static_assert(offsetof(VFActor, parameter34d) == 0x34d);
static_assert(offsetof(VFActor, parameter370) == 0x370);
static_assert(offsetof(VFActor, activeMotionFlags) == 0x3a8);
static_assert(offsetof(VFActor, guardLevel) == 0x411);
static_assert(offsetof(VFActor, lastHitMove) == 0x448);
static_assert(offsetof(VFActor, activeContactCount) == 0x44a);
static_assert(offsetof(VFActor, parameter44c) == 0x44c);
static_assert(offsetof(VFActor, parameter44e) == 0x44e);
static_assert(offsetof(VFActor, knockbackBaseParameter) == 0x494);
static_assert(offsetof(VFActor, knockbackVelocity) == 0x4cc);
static_assert(offsetof(VFActor, pendingKnockback) == 0x4e4);
static_assert(offsetof(VFActor, formationRuntimeFlags) == 0xa4c);

VFGW_t *AllocateVFGW(VFActor *actor);
void FreeVFGW(VFActor *actor);
VCDW_t *AllocateVCDW(VFMA_t *game);
void vfUpdateWorldTransformAndFacingAngle(VFActor *actor);
void vfApplyYawRecursive(VFActor *actor);
void vfUpdatePairingsAndDistances(VFMA_t *game);
void vfSelectDefaultReactionAndGuardAnims(VFActor *actor);
void vfUpdateDefaultReactionAnim(FCMG_t *group, std::uint16_t animation);
void motRemoveCallback(MotionWork *motion);
void vfPrepareEnemySlotMotion(FCMG_t *group);
void vfRestoreEnemySlotMotion(FCMG_t *group);
bool ReactToDead(VFMA_t *game, FCMG_t *group);
void vfRemoveCharaFromGroup(VFMA_t *game, FCMG_t *group);
bool vfEnemySlotResetAndRestartCycle(VFMA_t *game, FCMG_t *group);
bool vfTransitionEnemySlotToInactive(VFMA_t *game, FCMG_t *group);
bool vfTransitionEnemySlotToCooldown(VFMA_t *game, FCMG_t *group);
void vfCaptureGroundForCooldown(VFMA_t *game, VFActor *actor);
float vfAccumEnemyGroundOffsetAndMask(const Vector3 *position, std::uint32_t *nearbyActors);
void vfRegisterGroundTargetCallbacks(MotionWork *motion, VFActor *actor);
std::uint8_t vfRegisterContactSlot(VFMA_t *game, FCMG_t *slot);
void vfAssignWaveSpawnFromGenerator(FRMG_t *formation, float x, float z);
void vfClearEnemySlotStackRefs(std::uint8_t slotIndex);
void vfUpdateEnemySlotCooldowns(VFMA_t *game);
void vfTransitionAllEnemySlotsToInactive(VFMA_t *game);
void vfUpdateEnemySlotSpawnFade(VFMA_t *game);
void vfUpdateRenderOpacityForAll(VFMA_t *game);
extern VFMA_t *g_pFightMain;
void SuspendFight();
std::int32_t ResumeFight(std::int32_t resetActors);
std::int32_t SetTrainingFightReaction(std::uint16_t reaction);
void vfUpdateMain(VFActor *actor);
void vfFightNullsub();
enum class VFCollisionCounterEvent : std::uint32_t { ContactRegistered, HitConfirmed, HitSequenceEnded };
void vfResetMoveTrainingGains();
void vfConsumeMoveTrainingGains();
void vfAccumulateMoveTrainingGain(std::uint8_t encodedMove);
void vfLearnCommittedMove(VFMA_t *game, std::uint8_t encodedMove);
std::int8_t vfDecodeMoveCategoryAndIndex(std::uint8_t encodedMove, std::int8_t *destination) noexcept;
void vfmaUpdateCollisionCounter(VFMA_t *game, VFCollisionCounterEvent event, std::uint16_t encodedMove);
std::uint8_t vfReactCommitActionAndResetSlots(VFActor *actor);
std::int16_t vfGetFacingDeltaFromCameraSectorCode(VFActor *actor, std::uint16_t code);
std::uint8_t VF_RunStateMachine(VFActor *actor);
void vfQueueReactionFlagsForFrame(VFActor *actor, bool clearDifference);
void vfUpdateReactionOverrideState(VFActor *actor);
void vfUpdateDeferredReactionPriority(VFActor *actor, std::uint8_t slot);
std::uint8_t vfHandleSavedCharRootPosReaction(VFActor *actor);
std::uint8_t vfResolvePriorityReaction(VFActor *actor);
void vfmaUpdateNextReaction(VFActor *actor);
void vfResolvePairInteraction(VFActor *actor);
bool vfgGameInfoEntry(HLib::Task *source, HLib::Task *target);
void vfg_Callback_GameInfoEntry_callback(void *game);
HLib::Task *vfg_Callback_GameInfoEntry_Enqueue(VFMA_t *game);
void vfUpdateHPRegeneration(VFActor *actor);
float vfclCalculateGuardKnockbackFactor(VFActor *actor);
void vfInitSfxSlotState(VFMA_t *game);
void vfUpdateSlotRuntimeMetricsAndTimers(VFActor *actor);
void vfUpdateSlotReactionHistory(VFActor *actor);
void vfUpdateSlotRankParameters(VFActor *actor);
void vfUpdateCurrentAssistWindow(VFActor *actor);
void vfUpdateScenarioHPPhaseConfig(VFActor *actor);
std::uint8_t vfSelectOpponentSlotByPriority(VFMA_t *game, std::uint8_t minimumPriority);
void vfUpdateEnemySlotFormationAndYaw(VFMA_t *game);
void vfUpdateGuardLevel(VFMA_t *game, std::uint8_t index);
void vfInitializeSlotPhaseConfigurations(VFActor *actor);
void vfUpdateContactSlotsAndGroundFlags(VFActor *actor);
void vfUpdateHitFacingAndKnockback(VFActor *actor);
void vfRefreshContactReactionState(VFActor *actor);
void vfApplyKnockbackPhysics(VFActor *actor);
void vfUpdatePositionXZ(VFActor *actor);
void vfResolveKnockbackLanding(VFActor *actor);
void vfUpdateAutoAdvanceLanding(VFActor *actor);
void vfUpdateAirborneAnimationTiming(VFActor *actor);
void vfUpdateGroundHeightAndEnemyContacts(VFActor *actor);

struct FightEntry {
    Character *character;
    std::uint8_t status;
    std::byte reserved09[3];
    std::uint32_t hp;
    std::int32_t initialHp, parameters;
};
static_assert(sizeof(FightEntry) == 0x18);

struct VOBJ_t;
struct MT7Node;
struct ObjectMotionKey {
    std::int32_t frame;
    float value;
};
struct ObjectMotionCurve {
    const ObjectMotionKey *keys;
    std::int16_t cursor, count;
    std::uint32_t reserved;
};
struct ObjectMotionTrack {
    MT7Node *node;
    ObjectMotionCurve curves[6];
};
struct ObjectMotionNode {
    std::uint16_t index, reserved;
};
struct ObjectMotionRecord {
    std::int32_t byteSize;
};
struct ObjectMotionChannel {
    std::int32_t keyCount;
};
static_assert(sizeof(ObjectMotionKey) == 8);
static_assert(sizeof(ObjectMotionCurve) == 16);
static_assert(sizeof(ObjectMotionTrack) == 104);
static_assert(sizeof(ObjectMotionNode) == 4);
static_assert(sizeof(ObjectMotionRecord) == 4);
static_assert(sizeof(ObjectMotionChannel) == 4);
#pragma pack(push, 4)
struct FightStageObjectHitbox {
    std::int16_t kind, state;
    Vector3 position;
    float radius;

    std::byte unspecified[8];
};
struct FightStageObjectWorldHitbox {
    std::byte reserved00[4];
    Vector3 position;
    float radius;
    std::byte reserved14[8];
};
struct FightStageObjectDefinition {
    std::int8_t index;
    std::uint8_t attachmentBone;
    std::byte reserved02[2];
    Vector3 attachmentPosition;
    Vector3i attachmentRotation;
    std::uint32_t flags;
    std::int32_t hitboxCount;
    FightStageObjectHitbox hitboxes[8];
    std::int8_t guardStrength, guardState;
    std::int16_t hitParameter;
    std::int32_t motionTrackCount, reserved10c;
    const ObjectMotionNode *motionNodes;
    std::int32_t motionStep, motionFirstFrame, motionLastFrame, reserved124;
    const ObjectMotionRecord *motionKeys;
    std::uint32_t breakSound, hitSound;
    std::uint32_t groundSound, launchSound;
    float groundOffset, responseCoefficient;
    std::int32_t rotationSpeed, launchYaw;
    Vector3 positionOffset;
    std::byte reserved15c[4];
};
struct FightStageObjectRuntime {
    std::int8_t index;
    std::uint8_t state;
    std::byte reserved02[6];
    FightStageObjectDefinition *definition;
    std::uint32_t motionFlags, reactionFlags;
    std::uint64_t contactSlotMask;
    Vector3 contactPosition;
    Vector3i rotation;
    Vector3 reactionRotation;
    Vector3 velocity;
    std::byte reserved50[0xc];
    Vector3i rotationImpulse;
    Vector3 position;
    std::int32_t yaw;
    Character *model;
    Character *controlCharacter;
    Character *brokenModel;
    std::uint32_t savedControlFlags, reserved94;
    Character *reactingCharacter;
    FightStageObjectWorldHitbox hitboxes[8];
};
struct VOBJ_t {
    FightStageObjectRuntime object;
    std::int32_t motionFrame, reserved184;
    ObjectMotionTrack *motionTracks;
    VOBJ_t *next;
    Vector3 scaleTarget;
    std::uint16_t scaleTicks[3];
    std::byte reserved1aa[6];
};
#pragma pack(pop)
static_assert(sizeof(FightStageObjectHitbox) == 28);
static_assert(offsetof(FightStageObjectHitbox, unspecified) == 20);
static_assert(sizeof(FightStageObjectWorldHitbox) == 28);
static_assert(offsetof(FightStageObjectWorldHitbox, position) == 4);
static_assert(offsetof(FightStageObjectWorldHitbox, radius) == 16);
static_assert(sizeof(FightStageObjectDefinition) == 0x160);
static_assert(offsetof(FightStageObjectDefinition, attachmentBone) == 0x1);
static_assert(offsetof(FightStageObjectDefinition, attachmentPosition) == 0x4);
static_assert(offsetof(FightStageObjectDefinition, attachmentRotation) == 0x10);
static_assert(offsetof(FightStageObjectDefinition, flags) == 0x1c);
static_assert(offsetof(FightStageObjectDefinition, hitboxCount) == 0x20);
static_assert(offsetof(FightStageObjectDefinition, hitboxes) == 0x24);
static_assert(offsetof(FightStageObjectDefinition, guardStrength) == 0x104);
static_assert(offsetof(FightStageObjectDefinition, guardState) == 0x105);
static_assert(offsetof(FightStageObjectDefinition, hitParameter) == 0x106);
static_assert(offsetof(FightStageObjectDefinition, motionTrackCount) == 0x108);
static_assert(offsetof(FightStageObjectDefinition, motionNodes) == 0x110);
static_assert(offsetof(FightStageObjectDefinition, motionKeys) == 0x128);
static_assert(offsetof(FightStageObjectDefinition, breakSound) == 0x130);
static_assert(offsetof(FightStageObjectDefinition, responseCoefficient) == 0x144);
static_assert(offsetof(FightStageObjectDefinition, hitSound) == 0x134);
static_assert(offsetof(FightStageObjectDefinition, positionOffset) == 0x150);
static_assert(sizeof(FightStageObjectRuntime) == 0x180);
static_assert(offsetof(FightStageObjectRuntime, state) == 0x1);
static_assert(offsetof(FightStageObjectRuntime, definition) == 0x8);
static_assert(offsetof(FightStageObjectRuntime, motionFlags) == 0x10);
static_assert(offsetof(FightStageObjectRuntime, reactionFlags) == 0x14);
static_assert(offsetof(FightStageObjectRuntime, contactSlotMask) == 0x18);
static_assert(offsetof(FightStageObjectRuntime, contactPosition) == 0x20);
static_assert(offsetof(FightStageObjectRuntime, rotation) == 0x2c);
static_assert(offsetof(FightStageObjectRuntime, reactionRotation) == 0x38);
static_assert(offsetof(FightStageObjectRuntime, velocity) == 0x44);
static_assert(offsetof(FightStageObjectRuntime, rotationImpulse) == 0x5c);
static_assert(offsetof(FightStageObjectRuntime, position) == 0x68);
static_assert(offsetof(FightStageObjectRuntime, yaw) == 0x74);
static_assert(offsetof(FightStageObjectRuntime, model) == 0x78);
static_assert(offsetof(FightStageObjectRuntime, controlCharacter) == 0x80);
static_assert(offsetof(FightStageObjectRuntime, brokenModel) == 0x88);
static_assert(offsetof(FightStageObjectRuntime, savedControlFlags) == 0x90);
static_assert(offsetof(FightStageObjectRuntime, reactingCharacter) == 0x98);
static_assert(offsetof(FightStageObjectRuntime, hitboxes) == 0xa0);
static_assert(sizeof(VOBJ_t) == 0x1b0);
static_assert(offsetof(VOBJ_t, motionFrame) == 0x180);
static_assert(offsetof(VOBJ_t, motionTracks) == 0x188);
static_assert(offsetof(VOBJ_t, next) == 0x190);
static_assert(offsetof(VOBJ_t, scaleTarget) == 0x198);
static_assert(offsetof(VOBJ_t, scaleTicks) == 0x1a4);

struct FightSmokeProfile {
    float horizontalVariance, reservedVariance, birthRate;
    std::uint8_t particleCount, ticks;
    std::uint16_t reserved;
};
inline constexpr FightSmokeProfile fightSmokeProfiles[10]{
    {0.01f, 0.02f, 0.2f, 10, 75, 0}, {0.01f, 0.02f, 0.2f, 10, 75, 0}, {0.01f, 0.02f, 0.2f, 10, 75, 0},
    {0.005f, 0.18f, 0.3f, 25, 40, 0}, {0.02f, 0.08f, 0.2f, 20, 40, 0}, {0.014f, 0.014f, 0.2f, 8, 80, 0},
    {0.008f, 0.0008f, 0.2f, 3, 15, 0}, {0.008f, 0.0008f, 0.2f, 3, 15, 0},
    {0.008f, 0.0008f, 0.2f, 3, 15, 0}, {0.008f, 0.0008f, 0.2f, 3, 15, 0},
};
inline constexpr std::uint8_t fightSmokePaletteIndices[13]{7, 7, 7, 7, 7, 7, 8, 7, 8, 9, 8, 9, 1};
inline constexpr std::uint8_t fightSmokeColours[10][3]{
    {0, 0, 0}, {208, 208, 208}, {80, 80, 80}, {128, 128, 112}, {96, 96, 96},
    {113, 107, 94}, {113, 107, 94}, {160, 160, 144}, {128, 128, 112}, {96, 96, 80},
};
inline constexpr shen_name FIGHT_DEAD{SHEN_NAME(DEAD)};
inline constexpr const char *fightStyleNames[51]{
    "RYOU",
    "DEKU",
    "ZAKO1",
    "ZAKO2",
    "ZAKO3",
    "ZAKO4",
    "ZAKO5",
    "REN",
    "TOGYU",
    "KENMN",
    "YUAN",
    "SAM",
    "RALY",
    "MASTR",
    "BEAR",
    "SYUEI",
    "DNJYU",
    "PUNK",
    "FOUT",
    "SYOUK",
    "CHOU",
    "KOU1",
    "ENOKI",
    "JEFRY",
    "WOLF",
    "SARAH",
    "LAU",
    "NADAM",
    "MNKEY",
    "RUGGR",
    "IHAND",
    "ODEYE",
    "RINKN",
    "KANKI",
    "SFZ01",
    "SFZ02",
    "SFZ03",
    "SFZ04",
    "SFZ05",
    "SFZ06",
    "SFZ07",
    "SFZ08",
    "SFZ09",
    "SFZ10",
    "GUY",
    "LAU2",
    "DNJ2",
    "VSMNK",
    "ZAKO6",
    "AGON",
    "UZUN",
};

extern FightEntry g_activeFightData[16];
extern FightStageObjectDefinition *g_fightStageObjectDefinitions[8];
extern VOBJ_t *g_fightStageObjects[30];
extern VOBJ_t *g_fightStageObjectRoot;
extern float g_fightStageObjectMaximumDistance;

VFActor *AllocateFGTM();
VFSlotRuntimeState *AllocateFENE();
VFMA_t *vfmaGetTaskParam();
VFMA_t *vfmaSetGaugeSuppressed(std::int32_t suppressed);
VFMA_t *SetActiveFightModeFlag();
void vfSetupFightCameraState(VFMA_t *game);
std::intptr_t vfmQueueFightStart(std::uint8_t startSlot, std::uint8_t maximumEnemies, float knockback, std::uint8_t type, std::int32_t guardEffectSeconds);
void vfm_Callback_FightTaskEntry_Callback(void *game);
void blurInit();
std::uint8_t vfMotnNibbleGet(std::int8_t code);
std::uint8_t vfMotnNibbleSet(std::int8_t code, std::int32_t value);
void vfMotnDispatch(VFActor *actor);
void vfApplyMotionDescriptor(VFActor *actor);
void vfUpdateMoveVelocityWithKnockback(VFActor *actor, float acceleration);
void vfRescaleReactionFrameWindows(VFActor *actor);
void vfApplyHitReactionAndStartMotion(VFActor *actor);
const std::byte *vfFindQueuedReactionEventEntry(VFActor *actor, std::uint8_t eventType);
void vfDispatchQueuedReactionEvent(VFActor *actor);
void vfDispatchCurrentActionState(VFActor *actor);
void vfSelectDefaultMotion(VFActor *actor, std::uint32_t index);
void vfInitializeActorMotionState(VFActor *actor);
std::uint32_t vfScaleFrameCount(float scale, std::uint16_t frames);
void vfUpdateMotionTransitionFrames(VFActor *actor);
void vfUpdateReactAnimationFrames(VFActor *actor);
void vfUpdateAnimFlagsAndEvents(VFActor *actor);
void vfUpdateMotionTargetPointAndParams(VFActor *actor);
void vfUpdateRootBlendEnvelope(VFActor *actor);
std::uint32_t vfGetMoveCategoryMaskFor(VFActor *actor);
void vfclUpdatePairingFacing(VFActor *actor);
void vfUpdateDynamicActionState(VFActor *actor);
void vfclEnforceRingBoundary(VFCL_t *collision);
void vfclResolvePairedContactDrift(VFMA_t *game);
void vfClRaycastInWorldGrid();
std::uint8_t AccessFightActorFlag411(std::uint8_t mode, Character *character, std::uint8_t value);
float AccessFightActorGroupFloat(std::uint8_t mode, Character *character, float value);
extern std::uint8_t g_vfRoundResultCounters[4];
void vfAddRoundResults(std::uint8_t won, std::uint8_t lost);
std::int8_t vfUpdatePredicate_40_41_44_50_51_54();
void vfUpdateFightFloorReaction(VFActor *actor);
void vfActorUpdateState(VFActor *actor, VFActor *other);
void vfUpdatePoseWithSfx(VFActor *actor);
void vfInitializeFightActor(VFMA_t *game, std::uint8_t index);
void ConstructColouredText(std::uint32_t elapsedFrames, std::int32_t x, std::int32_t y, float depth);
void vfDrawFightLoadSprite(std::int32_t index, std::int32_t x, std::int32_t y, float depth, std::uint32_t elapsedFrames);
void constructNumStrikesLeft();
void vfUpdateGroundImpactReaction(VFActor *actor);
extern std::int32_t g_vfSelectedReactionAnimation;
std::uint8_t vfSelectHitReactionSequence(VFActor *actor);
void vfSetupHitReactionAnim(VFActor *actor, VFActor *other, std::uint16_t animation);
void vfclChooseReactionAnim(VFActor *actor, VFActor *other);
bool vfTryGuardReactionSwap(VFActor *actor, VFActor *source);
bool vfIsLastEnemyForCamera(VFActor *actor);
bool vfIsWithinFightFloor(const Vector3 &position);
void vfResetScriptedFormationMotion(VFActor *actor);
void vfSnapActorToContact(VFActor *actor);
void vfPredictLandingGround(VFActor *actor);
std::uint16_t vfRotateCameraSectorCode(VFActor *actor, std::uint16_t code);
void vfResetAndSelectCameraPhaseChoice(VFActor *actor, std::uint8_t choice, std::uint16_t delay);
bool vfUpdateCameraScript(VFActor *actor);
void vfAdvanceCameraPhaseChain(VFActor *actor);
void vfResetReactionProgram(VFActor *actor);
void vfResetActorActionState(VFActor *actor);
VFActor *vfGetActorFromSlot(VFMA_t *game, shen_name name);
const char *vfGetFightDialogueVoice(VFMA_t *game, std::int16_t index);
void vfInitializeFightDialogue(VFMA_t *game);
bool vfHasDialoguePlayerFocus();
void vfmaSetupDialogueReactionsEtc(VFMA_t *game);
bool vfCheckAnimTimeWithin(VFActor *actor, float frame);
std::uint8_t vfProcessMotionFrameEvents(VFActor *actor);
std::uint8_t vfFightVoiceStyle(std::uint16_t style);
void vfPlayFightSfx(std::uint32_t sound, VFActor *actor, std::uint8_t mode);
void vfForceActorDefeat(VFActor *actor);
void vfStartAuthoredDefeatCamera(VFActor *actor);
void vfHandleImpactSfxAndDefeat(VFActor *actor, std::int16_t damage, std::uint8_t mode);
void vfInitStageObjects();
void vfFreeStageObjects();
FightStageObjectDefinition **AllocateVOBJ(std::int32_t slot, std::int8_t index);
std::uint32_t ToggleVOBJArrayIndex(
    std::int32_t slot, std::int8_t guardStrength, std::int8_t guardState, std::int16_t hitParameter, std::uint32_t hitSound);
const std::byte *vfStageDefinitionSetMotion(std::int32_t slot, std::uint32_t name, std::int32_t tracks, const std::uint32_t *nodes, std::int32_t step,
    std::int32_t firstFrame, std::int32_t lastFrame, std::uint32_t sound);

void vfStageDefinitionAddHitbox(std::int32_t slot, std::uint32_t flags, std::uint16_t kindAndState, float x, float y, float z, float radius);
std::uint64_t vfStageDefinitionSetHitResponse(std::int32_t slot, std::uint32_t groundSound, std::uint32_t launchSound, std::uint32_t flags,
    std::int32_t launchYaw, std::int32_t rotationSpeed, float coefficient, float groundOffset);
std::uint64_t vfStageDefinitionSetAttachment(
    std::int32_t slot, std::uint8_t bone, float x, float y, float z, std::int32_t rotationX, std::int32_t rotationY, std::uint32_t rotationZ);
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
std::int32_t vfAllocateStageObjectMotionTracks(VOBJ_t *object);
void vfStageObjectEvalHitboxKeyframes();
void vfConvertToStageObjectHitboxSpace(VOBJ_t *object, Vector3 &point);
void vfRecalcStageObjectLocalEuler(VOBJ_t *object);
void vfCreateStageObjectRotationMatrix(VOBJ_t *object, const Vector3 &displacement);
void vfUpdateStageObjectWorldPos(VOBJ_t *object);
void vfClAicaReact(const Vector3 &position, std::uint32_t flags);
void vfDeflectStageObjectYaw(VOBJ_t *object, const Vector3 &direction);
void vfRotateStageObjectAboutOffset(VOBJ_t *object, const Vector3 &offset);
void vfTransformStageObjectHitboxes(VOBJ_t *object);
bool vfStageObjectOverlapsActor(VOBJ_t *object, VFActor *actor, std::int32_t withVelocity);
void vfResolveActorStageObjectPenetration(VFActor *actor, VOBJ_t *object);
void vfProcessStageObjectHitOutcome(VOBJ_t *object, VFActor *actor, std::int8_t guardState, std::int16_t hitParameter, std::int8_t guardStrength);
void vfclFilterObjectHit(VFActor *actor, VOBJ_t *object);
void vfUpdateRotationAnglesFromStageObject(VOBJ_t *object, VFMA_t *game);
void vfBreakStageObject(VOBJ_t *object);
void vfclProcessCollisionAndSound(VFMA_t *game);
void vfclUpdateStageObjectHitboxesPhysics(VFMA_t *game);
void vfclTriggerStageObjectHitReaction(VFMA_t *game);
void vfUpdateStageObjects(VFMA_t *game);
void vfToggleAttachedStageObjectDisplay(VFActor *actor);
void vfSetAttachedStageObjectEffect(VFActor *actor, std::int8_t effect, float amount, std::int16_t frames);
void vfSetAttachedStageObjectFigurePose(VFActor *actor, std::int8_t scale, std::int16_t direction);
float flddFindIntersectionBy(const Vector3 &position);
float flddGetIntersectionFor(Character *character, Vector3 &position);
float vfFieldGetIntersection(VFActor *actor, Vector3 &position);
void EmitFightSmoke(std::uint8_t profile, VFActor *actor);
std::int32_t GetBtlStatus(Character *character);
void EnsureLoadedFightStyle(std::uint8_t style, std::uint8_t slot);
std::uint8_t PublishFightFormation(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style,
    std::uint16_t spawnWord0, std::uint16_t spawnWord1, float x, float y, float z);
std::int64_t PublishFightRespawn(std::uint16_t hp, std::uint8_t style, std::uint16_t spawnWord0, float x, float y, float z, std::uint16_t spawnWord1);
std::uint8_t AccessFightActorFlag411(std::uint8_t mode, Character *character, std::uint8_t value);
float AccessFightActorGroupFloat(std::uint8_t mode, Character *character, float value);
void SetFightActorParameterBytes(shen_name character, const std::uint8_t *values);

} // namespace shenmue::s2
