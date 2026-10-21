#include "s1/task/vf_internal.hpp"

namespace shenmue::s1::vf_detail {

constexpr std::array<std::int32_t, 17> g_vfPadCommandFlags{0, 8, 1, 0, 0, 0, 0x4000, 0x40000000, 0, 4, 2, 0, 0x20, 0x10, 0x80, 0x40, -1};

constexpr std::array<std::int32_t, 17> g_vfSlotCommandFlags{0, 8, 1, 0, 0, 0, 0x4000, 0, 0, 4, 2, 0x100, 0x20, 0x10, 0x80, 0x40, -1};

constexpr std::array<std::uint16_t, 8> g_vfCameraSectors{0x20, 0xa0, 0x80, 0x90, 0x10, 0x50, 0x40, 0x60};

constexpr std::array<std::uint16_t, 16> g_vfCameraYawAngles{
    0x8001, 0x0000, 0x8001, 0x8001, 0x4000, 0x2000, 0x6000, 0x6000, 0xc001, 0xe001, 0xa001, 0xa001, 0x8001, 0x0000, 0x8001, 0x8001};

std::uint16_t vfAssignPadInfoFor(const VFActor &actor) {
    std::uint16_t held = static_cast<const PadCaptureEntry *>(actor.game->padCaptureEntry)->held;
    std::uint16_t flags = 0;
    for (std::uint8_t index = 0; g_vfPadCommandFlags[index] != -1; ++index, held >>= 1) {
        if ((held & 1U) != 0)
            flags = static_cast<std::uint16_t>(flags | g_vfPadCommandFlags[index]);
    }
    return flags;
}

std::uint16_t vfFindInputResponse(std::uint8_t slot) {
    std::uint16_t held = g_inputState.slots[slot].held;
    std::uint16_t flags = 0;
    for (std::uint8_t index = 0; g_vfSlotCommandFlags[index] != -1; ++index, held >>= 1) {
        if ((held & 1U) != 0)
            flags = static_cast<std::uint16_t>(flags | g_vfSlotCommandFlags[index]);
    }
    return flags;
}

static_assert(std::size(kVFMainCallbacks) == 58);


// @verified - p
void vfUpdateMain(VFActor &actor) {
    VFMAData &game = *actor.game;
    g_pFightMain = &game;
    VFSlotRuntimeState &slot = *game.slotRuntime[actor.slotIndex];
    actor.previousCameraControlFlags = actor.cameraControlFlags;
    actor.cameraControlFlags = 0;
    if (actor.targetSlotIndex == -1 || game.slotHpPoolMode[actor.slotIndex] > 1U)
        return;
    const VFActor &target = *game.actors[actor.targetSlotIndex];
    UpdateSlotRuntimeMetricsAndTimers(actor);
    UpdateRepeatedReactionHistory(actor);
    UpdateScenarioHpPhaseConfig(actor);
    UpdateCurrentAssistWindow(actor);
    if ((actor.hitStateFlags2 & 0x000a4000U) != 0) {
        VFSlotRuntimeState &own = *actor.slot;
        own.hpPhase = 0;
        own.cameraScriptStep = 0;
        own.cameraScriptWait = 0;
        own.fielda8 = 0;
        own.fieldb7 = 0;
        own.byte54 = 0x7fU;
        own.byte55 = 0x7fU;
        own.updateCamera = (own.updateCamera & 0xffffdffeU) | 1U;
        slot.field9a = 0;
    }
    if ((target.hitStateFlags2 & 0x00014000U) == 0x00010000U) {
        slot.byte54 = 0x7fU;
        slot.byte55 = 0x7fU;
        slot.field9a = 0;
        slot.fieldb7 = 0;
    }
    slot.slotIndex = static_cast<std::uint8_t>(actor.targetSlotIndex);
    static constexpr std::uint8_t defaultProgram[] = {16, 0};
    static constexpr std::uint8_t sparringProgram[] = {16, 42, 0};
    static constexpr std::uint8_t styled1[] = {51, 0};
    static constexpr std::uint8_t styled2[] = {51, 4, 44, 20, 0};
    static constexpr std::uint8_t styled3[] = {51, 4, 48, 44, 20, 0};
    static constexpr std::uint8_t styled4[] = {51, 44, 20, 49, 0};
    static constexpr std::uint8_t chai[] = {44, 3, 15, 11, 30, 35, 28, 18, 27, 2, 14, 13, 31, 1, 0};
    static constexpr std::uint8_t kisy20[] = {4, 44, 3, 15, 17, 27, 2, 11, 5, 26, 31, 1, 0};
    static constexpr std::uint8_t kisy21[] = {4, 44, 3, 15, 17, 2, 6, 21, 11, 5, 26, 31, 1, 57, 0};
    static constexpr std::uint8_t kisy23[] = {4, 44, 3, 15, 17, 22, 23, 32, 2, 6, 21, 11, 5, 26, 31, 1, 0};
    static constexpr std::uint8_t kisy24[] = {4, 3, 15, 5, 26, 31, 1, 0};
    static constexpr std::uint8_t kisy30[] = {4, 56, 44, 3, 22, 23, 2, 6, 21, 11, 5, 26, 31, 1, 0};
    static constexpr std::uint8_t kisy32[] = {4, 56, 44, 3, 2, 6, 5, 26, 31, 1, 0};
    static constexpr std::uint8_t kisy33[] = {4, 56, 3, 27, 5, 26, 1, 0};
    static constexpr const std::uint8_t *styled[] = {nullptr, styled1, styled2, styled3, styled4};
    static constexpr const std::uint8_t *kisy2[] = {kisy20, kisy21, kisy21, kisy23, kisy24};
    static constexpr const std::uint8_t *kisy3[] = {kisy30, kisy30, kisy32, kisy33};
    const std::uint8_t *program;
    if (actor.fightStyleIndex == 0) {
        program = game.battleType == 2 ? sparringProgram : defaultProgram;
    } else if (actor.fightStyleIndex == 27U) {
        program = chai;
    } else if (actor.fightStyleIndex == 28U) {
        program = kisy2[slot.currentHpRegenerationPhase];
    } else if (actor.fightStyleIndex == 29U) {
        program = kisy3[slot.currentHpRegenerationPhase];
    } else if (game.soundFlag && (game.soundSlots[actor.slotIndex] & 2) != 0 && game.soundSlotState[actor.slotIndex]) {
        program = styled[game.soundSlotState[actor.slotIndex]];
        if (game.actorIndex != 0xffU && game.characters[game.actorIndex])
            slot.slotIndex = game.actorIndex;
    } else {
        program = static_cast<const VFStyleTuningBE *>(actor.styleTuning)->reaction_prog_preset_blob;
    }
    std::uint8_t selected;
    do {
        selected = kVFMainCallbacks[*program++].select(&actor, &slot);
    } while (selected == 0xffU);
    slot.unknown5d[3] = selected;
    if (slot.currentUpdateIndex != selected) {
        slot.currentUpdateIndex = selected;
        slot.byteaa = 0;
        slot.unknownab = 0;
    }
    (void)kVFMainCallbacks[selected].publish(&actor, &slot);
    if ((slot.updateCamera & 1U) == 0)
        (void)AdvanceVFCameraPhaseChain(actor);
}

void vfmaUpdateCollisionCounter(VFMAData &game, VFCollisionCounterEvent event, std::uint16_t fighterId) {
    if (game.battleType == 4U || game.battleType == 5U || fighterId >= std::size(game.skillTable)) {
        return;
    }
    switch (event) {
    case VFCollisionCounterEvent::ContactRegistered:
        if (game.skillTable[fighterId] != -1)
            ++game.skillTable[fighterId];
        game.flags &= ~0x440U;
        break;
    case VFCollisionCounterEvent::HitConfirmed:
        if (game.runtimeSkills[fighterId] != -1 && (game.flags & PadInputOverride) == 0) {
            ++game.runtimeSkills[fighterId];
            game.flags |= PadInputOverride;
        }
        break;
    case VFCollisionCounterEvent::HitSequenceEnded:
        if (game.skillTable[fighterId] != 0 && game.runtimeSkills[fighterId] != 0 && (game.flags & 0x400U) == 0) {
            --game.runtimeSkills[fighterId];
            game.flags |= 0x400U;
        }
        break;
    }
}

static_assert(offsetof(VFActor, reactionStateId) == 0x2d2);

static_assert(offsetof(VFActor, timeScale) == 0x2b0);

static_assert(offsetof(VFActor, reactionMidFrame) == 0x2da);

static_assert(offsetof(VFActor, guardState) == 0x2f1);

static_assert(offsetof(VFActor, reactionState02EndFrame) == 0x312);

static_assert(offsetof(VFActor, stateFlags2) == 0x318);

static_assert(offsetof(VFActor, hitFlags) == 0x338);

static_assert(offsetof(VFActor, facingAngle) == 0x198);

static_assert(offsetof(VFActor, temporaryFacingAngle) == 0x19e);

static_assert(offsetof(VFActor, contactAngles) == 0x1a0);

static_assert(offsetof(VFActor, pairedFighterId) == 0xae7);

static_assert(offsetof(VFActor, stageHitObjectIndex) == 0xacf);

static_assert(offsetof(VFFormationGroup, selectFlags) == 0x24);

static_assert(offsetof(VFSlotRuntimeState, currentStageHitResponseFrames) == 0xac);

static_assert(offsetof(VFMAData, soundSlots) == 0x268 && offsetof(VFMAData, soundFlag) == 0x290 && offsetof(VFMAData, soundSlotState) == 0x29b);

static_assert(offsetof(VFFightStage, battleCenter) == 0x238 && offsetof(VFFightStage, useCenterBias) == 0x244 && offsetof(VFFightStage, aliveByTeam) == 0x24c);

static_assert(offsetof(VFActor, nextAction) == 0x378);

static_assert(offsetof(VFActor, queuedAction) == 0x388);

static_assert(offsetof(VFActor, hitStateFlags2) == 0x390);

static_assert(offsetof(VFActor, motionFlags) == 0x3a0);

static_assert(offsetof(VFActor, groundContactFlags) == 0x3b4);

static_assert(offsetof(VFActor, contactDecisionMask) == 0x3b8);

static_assert(offsetof(VFActor, latchedPairContactPriority) == 0x3c0);

static_assert(offsetof(VFActor, currentHp) == 0x3c8);

static_assert(offsetof(VFActor, contactPoints) == 0x3cc);

static_assert(offsetof(VFActor, hitReactionAnimSelectFlags) == 0x3f9);

static_assert(offsetof(VFActor, guardStat) == 0x400);

static_assert(offsetof(VFActor, currentReactionAnimation) == 0x47c);

static_assert(offsetof(VFActor, currentAnimationEndFrame) == 0x488);

static_assert(offsetof(VFActor, delayedReactionStateActive) == 0x66e);

static_assert(offsetof(VFActor, delayedReactionTimer) == 0x670);

static_assert(offsetof(VFActor, delayedReactionFramesRemaining) == 0x672);

static_assert(offsetof(VFActor, reactionVmProgramCounter) == 0x688);

static_assert(offsetof(VFActor, scheduledReactionYaw) == 0x69a);

static_assert(offsetof(VFActor, pendingAlternateRootReactionProgram) == 0x6a0);

static_assert(offsetof(VFActor, functionReactionSlotIndex) == 0x6ba);

static_assert(offsetof(VFActor, reactionConditionFlags) == 0x8d8);

static_assert(offsetof(VFActor, reactionCounters) == 0x8e1);

static_assert(offsetof(VFActor, reactionStateFlags) == 0x8e8);

static_assert(offsetof(VFActor, reactionPriorityMaskA) == 0x8ec);

static_assert(offsetof(VFActor, fighterRankMin) == 0xa56);

static_assert(offsetof(VFActor, stageHitObjectIndex) == 0xacf);

static_assert(offsetof(VFActor, savedNestedActorCharacter) == 0xb00);

static_assert(offsetof(VFMAData, extraValue) == 0x2c);

static_assert(offsetof(VFMAData, reactionTypeIndex) == 0x2f0);

static_assert(offsetof(VFMAData, primaryTargetSlot) == 0x89);

static_assert(offsetof(VFMAData, remainingEnemies) == 0x8a);

static_assert(offsetof(VFMAData, enemyRanks) == 0x8b);

static_assert(offsetof(VFMAData, soundSlotOrder) == 0x292);

static_assert(offsetof(VFActor, counterReactionFlags) == 0x3c4);

static_assert(offsetof(VFActor, forwardClearance) == 0x404);

static_assert(offsetof(VFActor, backwardClearance) == 0x408);

static_assert(offsetof(VFActor, rightClearance) == 0x40c);

static_assert(offsetof(VFActor, leftClearance) == 0x410);

static_assert(offsetof(VFActor, hitSourceActor) == 0x40);

static_assert(offsetof(VFSlotRuntimeState, assistAngleLimit) == 0x20);

static_assert(offsetof(VFSlotRuntimeState, phaseConfig) == 0x28);

static_assert(offsetof(VFSlotRuntimeState, assistWindowCurrent) == 0x38);

static_assert(offsetof(VFSlotRuntimeState, updateCamera) == 0x3c);

static_assert(offsetof(VFSlotRuntimeState, field50) == 0x50);

static_assert(offsetof(VFSlotRuntimeState, slotIndex) == 0x5a);

static_assert(offsetof(VFSlotRuntimeState, facingAngleDifferences) == 0x64);

static_assert(offsetof(VFSlotRuntimeState, fieldb2) == 0xb2);

static_assert(offsetof(VFSlotRuntimeState, byteb9) == 0xb9);

void vfQueueReactionFlagsForFrame(VFActor &actor, bool clearDiff) {
    const std::uint8_t head = actor.reactionRingHead & 31U;
    const std::uint8_t next = (head + 1U) & 31U;
    const std::uint8_t next2 = (head + 2U) & 31U;
    const VFReactionPriorityMeta &previous = actor.reactionPriority[(head - 1U) & 31U];
    VFReactionPriorityMeta &current = actor.reactionPriority[head];
    std::uint16_t flags = actor.cameraControlFlags;
    actor.priorityMask = static_cast<std::int16_t>(flags);
    actor.priorityFlags = flags & ~previous.mask1;
    actor.priorityFlags2 = actor.priorityFlags;
    current.mask1 = flags;
    current.flags = clearDiff ? 0 : actor.priorityFlags;
    current.unknownFlags = previous.mask1 & ~flags;
    actor.priorityMaskDifference = current.unknownFlags;
    current.value = flags ? flags : previous.mask2;
    if ((flags & 0xc0U) == 0)
        flags |= previous.mask2 & 0xc0U;
    if ((flags & 0x30U) == 0)
        flags |= previous.mask2 & 0x30U;
    current.mask2 = flags;
    actor.reactionRingHead = next;
    if (next == actor.slots.primary)
        actor.slots.primary = next2;
    if (next == static_cast<std::uint8_t>(actor.slots.secondary))
        actor.slots.secondary = static_cast<std::int8_t>(next2);
    if (next == actor.reactionSlotCurrent)
        actor.reactionSlotCurrent = next2;
}

void vfClampReactionQueueLength(VFActor &actor) {
    const std::uint8_t length = (actor.reactionRingHead - static_cast<std::uint8_t>(actor.reactionRingTail)) & 31U;
    const std::uint8_t maximum = (actor.hitStateFlags2 & 0x10000000U) != 0 ? 8U : 6U;
    actor.reactionRingTail = static_cast<std::int8_t>((actor.reactionRingHead - std::min(length, maximum)) & 31U);
}

void vfUpdateDeferredReactionPriority(VFActor &actor, std::uint8_t index) {
    const std::int16_t priority = actor.priorityFlags & 0x030fU;
    auto *const otherFlags = reinterpret_cast<std::uint8_t *>(&actor.otherPriorityFlags);
    std::uint8_t &pending = otherFlags[index + 1U];
    std::int16_t &deferred = index == 0 ? actor.field66a : actor.field66c;
    std::uint8_t &selected = index == 0 ? actor.selectedReactionSlot : actor.savedReactionQueueTail;
    if (pending) {
        if ((actor.reactionOverrideActive & 1) != 0 || deferred != 0x0100 || (actor.priorityMask & 0x0100) != 0 || priority != 1) {
            pending = 0;
            actor.priorityFlags |= static_cast<std::uint16_t>(deferred | priority);
        } else {
            selected = static_cast<std::uint8_t>(actor.reactionRingTail);
            otherFlags[0] = selected;
            pending = 0;
            actor.priorityFlags |= 1U;
        }
    } else {
        if (priority != 0 && priority != 0x030f) {
            pending = 1;
            deferred = priority;
            actor.priorityFlags &= 0xfcf0U;
        }
        selected = static_cast<std::uint8_t>(actor.reactionRingTail);
        otherFlags[0] = selected;
    }
}

void vfUpdateReactionOverrideState(VFActor &actor) {
    bool resetProgram = false;
    if ((actor.moveFlags & 0x10U) != 0) {
        actor.reactionOverrideActive |= 1;
        resetProgram = true;
    } else {
        actor.reactionVmProgramCounter = actor.reactionResumeProgramCounter;
        if ((actor.hitStateFlags2 & 1U) != 0)
            actor.reactionOverrideActive |= 1;
        else if (!actor.reactionResumeProgramCounter || (!actor.delayedReactionTimer && !actor.delayedReactionStateActive))
            resetProgram = true;
    }
    if (resetProgram) {
        actor.delayedReactionStateActive = 0;
        actor.delayedReactionTimer = 0;
        actor.reactionVmProgramCounter = static_cast<VFActionDescriptor *>(actor.reactionScriptBase);
        actor.reactionResumeProgramCounter = nullptr;
    }
    actor.reactionStateFlags = (actor.reactionStateFlags & 2) != 0 ? actor.reactionStateFlags | 4 : actor.reactionStateFlags & ~4;
    actor.reactionStateFlags = (actor.reactionOverrideActive & 1) != 0 ? actor.reactionStateFlags | 2 : actor.reactionStateFlags & ~2;
}

VFStateMachineResult vfHandleSavedCharRootPosReaction(VFActor &actor) {
    actor.currentReactionFlagFilter = 0x01f0;
    actor.previousReactionFlagFilter = 0xffff;
    actor.reactionResumeProgramCounter = nullptr;
    if (actor.rootReactionVmProgramCounter) {
        VFActionDescriptor *const saved = actor.reactionVmProgramCounter;
        actor.reactionVmProgramCounter = actor.rootReactionVmProgramCounter;
        actor.functionReactionSlotIndex = actor.slots.primary;
        const auto result = RunReactionStateMachine(actor);
        if (result == VFStateMachineResult::YieldToCaller)
            return result;
        actor.reactionVmProgramCounter = saved;
        if (!saved)
            return result;
        actor.reactionResumeProgramCounter = nullptr;
    }
    actor.functionReactionSlotIndex = actor.slots.primary;
    return RunReactionStateMachine(actor);
}

VFStateMachineResult vfHandlePriorityReactionFlags(VFActor &actor) {
    if ((actor.reactionOverrideActive & 1) != 0 || (actor.priorityMask & 0x0300) == 0)
        return VFStateMachineResult::Finished;
    actor.reactionStateFlags &= ~1;
    if ((actor.reactionPriority[(actor.reactionRingHead - 1U) & 31U].flags & 0x0300U) != 0)
        actor.reactionRingTail = static_cast<std::int8_t>(actor.reactionRingHead);
    if ((actor.hitStateFlags2 & 0x1000U) == 0) {
        actor.nextAction = {};
        actor.nextAction.state = (actor.priorityMask & 0x0200) != 0 ? 11U : 10U;
        if ((actor.stateFlags & 0x0cU) != 0)
            actor.nextAction.state = (actor.priorityMask & 0x0200) != 0 ? 41U : 40U;
        vfClampReactionQueueLength(actor);
    }
    return VFStateMachineResult::YieldToCaller;
}

VFStateMachineResult vfUpdateReactionQueueState(VFActor &actor) {
    if ((actor.hitStateFlags2 & 0x0001c000U) != 0)
        return VFStateMachineResult::Finished;
    if ((actor.priorityFlags & 0x0300U) == 0)
        return vfHandlePriorityReactionFlags(actor);
    actor.reactionStateFlags &= ~1;
    actor.reactionRingTail = static_cast<std::int8_t>(actor.reactionRingHead);
    if (actor.reactionResumeProgramCounter ? actor.delayedReactionTimer == 0 : (actor.reactionOverrideActive & 1) == 0 && (actor.hitStateFlags2 & 0x10U) == 0) {
        actor.reactionConditionFlags = 4;
        if (vfHandleSavedCharRootPosReaction(actor) == VFStateMachineResult::YieldToCaller)
            return VFStateMachineResult::YieldToCaller;
    }
    actor.slots.primary = actor.selectedReactionSlot;
    actor.slots.secondary = static_cast<std::int8_t>(actor.selectedReactionSlot);
    actor.reactionResumeProgramCounter = nullptr;
    actor.delayedReactionTimer = 0;
    actor.delayedReactionStateActive = 0;
    if ((actor.reactionOverrideActive & 1) == 0)
        (void)vfHandlePriorityReactionFlags(actor);
    return VFStateMachineResult::YieldToCaller;
}

void vfmaUpdateNextReaction(VFActor &actor) {
    VFMAData &game = *actor.game;
    bool inputConsumed = false;
    actor.previousReactionAnimationId = 0xffff;
    actor.previousAnimationEndFrame = 0xffff;
    actor.pairProcessedMask = 0;
    actor.reactionQueuePhase = 0;
    const std::uint32_t gameFlags = game.gameFlags;
    if ((gameFlags & 0x20U) != 0 || game.state != VFS_InBattle)
        return;
    const std::uint8_t slotIndex = actor.slotIndex;
    std::uint8_t inputMode = game.slotHpPoolMode[slotIndex];
    if ((gameFlags & 0x400000U) != 0) {
        if (slotIndex == 0 && (game.flags & DisableRankUpdate) != 0 && (g_inputState.slots[0].pressedPriority & 0x606U) != 0)
            game.gameFlags = gameFlags & ~0x400000U;
        else
            inputMode = 1;
    }
    if (inputMode == 0) {
        const std::uint16_t padFlags = vfAssignPadInfoFor(actor);
        if ((game.gameFlags & 0x40000U) != 0)
            actor.cameraControlFlags = padFlags;
        else
            actor.cameraControlFlags = static_cast<std::uint16_t>(actor.cameraControlFlags | padFlags);
        if ((actor.cameraControlFlags & 0xf0U) == 0xf0U)
            actor.cameraControlFlags = static_cast<std::uint16_t>((actor.cameraControlFlags & 0xff0fU) | 0x4000U);
        const bool eventStopActive = (g_shenmueFlags[0x320U >> 3U] & static_cast<std::uint8_t>(1U << (0x320U & 7U))) != 0;
        bool sequenceStarted = false;
        std::uint32_t flags = game.flags;
        if ((flags & FreeBattleIntroActive) != 0) {
            if ((g_inputState.slots[0].pressedPriority & 0x200U) != 0 && (flags & SequenceCooldown) == 0) {
                shenRefreshUI();
                UI_SICNEnqueue(0x401, UISicnPausePolicy::KeepOnPause);
                game.flags &= ~(FreeBattleIntroActive | ExitConfirmPending);
                inputConsumed = true;
            }
            sequenceStarted = inputConsumed;
        } else if (game.battleType == 4 && (flags & 0x3bU) == 0 && (g_inputState.slots[0].pressedPriority & g_vfConfirmButtonMask) != 0 &&
            game.frameCount > 15 && (flags & SequenceCooldown) == 0 && !eventStopActive) {
            g_cam_shake_damp = 1.0f;
            g_cam_shake_prev = 0.0f;
            g_cam_shake_curr = 0.0f;
            EnqueueWZOG();
            figpResolveAnimID(game);
            UI_SICNSetState(0x823, UISicnPausePolicy::KeepOnPause);
            game.flags |= FreeBattleIntroActive;
            sequenceStarted = true;
        }
        flags = game.flags;
        if ((flags & FreeBattleExitActive) != 0 && ((g_inputState.slots[0].pressedPriority & 0x200U) != 0 || (flags & ExitConfirmPending) != 0) &&
            (flags & SequenceCooldown) == 0) {
            EnqueueEWZO(1, 0, 0);
            game.flags &= ~(FreeBattleExitActive | ExitConfirmPending);
            Shenmue_nPauseState &= ~0x20;
            UI_SICNSetState(0x401, UISicnPausePolicy::KeepOnPause);
            inputConsumed = true;
            g_hazukiResidenceGatedFlag = 1;
            sequenceStarted = true;
        }
        if (sequenceStarted) {
            game.flags |= SequenceCooldown;
            game.cooldownSequenceFrames = 4;
        } else if ((flags & SequenceCooldown) != 0) {
            if (static_cast<std::int16_t>(game.cooldownSequenceFrames) <= 0) {
                Shenmue_nPauseState &= ~0x20;
                game.flags = flags & ~SequenceCooldown;
            } else {
                --game.cooldownSequenceFrames;
            }
        }
        if ((game.flags & 0x3bU) != 0) {
            actor.cameraActiveCounterFrames = 0;
            actor.cameraIdleCounterFrames = 0;
        } else {
            const std::uint32_t activeTrigger = actor.triggerMask & 0x100U;
            if (activeTrigger) {
                if ((actor.cameraControlFlags & 0xf7U) != 0)
                    actor.cameraActiveCounterFrames = 0;
                else
                    actor.cameraActiveCounterFrames = static_cast<std::int16_t>(actor.cameraActiveCounterFrames + 1);
            }
            if ((activeTrigger || actor.game->battleType == 4) && (actor.cameraControlFlags & 0x0fU) == 0) {
                if (static_cast<std::uint16_t>(actor.cameraIdleCounterFrames) < 0xffffU)
                    actor.cameraIdleCounterFrames = static_cast<std::int16_t>(actor.cameraIdleCounterFrames + 1);
            } else {
                actor.cameraIdleCounterFrames = 0;
            }
        }
    } else if (inputMode == 2) {
        actor.cameraControlFlags = vfFindInputResponse(1);
    } else if (inputMode == 3) {
        actor.cameraControlFlags = vfFindInputResponse(2);
    } else if (inputMode == 4) {
        actor.cameraControlFlags = vfFindInputResponse(3);
    } else if (inputMode == 6) {
        actor.cameraControlFlags = 0;
    }
    if ((game.flags & (PlayerAggro | FreeBattleExitPrompt | FreeBattleIntroActive | FreeBattleExitActive | BattleCamUiLock)) != 0)
        actor.cameraControlFlags = 0;
    const std::uint16_t sector = actor.cameraControlFlags & 0xf0U;
    std::uint8_t sectorIndex = 0;
    while (g_vfCameraSectors[sectorIndex] != sector) {
        if (++sectorIndex >= 8) {
            sectorIndex = 0xffU;
            break;
        }
    }
    const auto accumulatedCameraYaw = static_cast<std::int16_t>(g_fCamAccumYaw);
    const auto cameraSector = static_cast<std::int16_t>(accumulatedCameraYaw + static_cast<std::int16_t>(actor.facingYaw) + 0x1000);
    const auto moveIndex = static_cast<std::uint8_t>((sectorIndex - (cameraSector >> 13)) & 7);
    actor.moveAngleTableIndex = static_cast<std::int8_t>(moveIndex);
    const bool playerFrame = actor.slotIndex == static_cast<std::uint8_t>(game.playerSlot) && (game.gameFlags & 0x400000U) == 0;
    actor.moveState = static_cast<std::int8_t>(playerFrame ? moveIndex : sectorIndex);
    const std::uint16_t inputYaw = g_vfCameraYawAngles[(actor.cameraControlFlags >> 4U) & 0x0fU];
    actor.cameraAngle = static_cast<std::uint16_t>(inputYaw + 0x8000U);
    actor.cameraYaw = static_cast<std::int16_t>(inputYaw - accumulatedCameraYaw + 0x8000);
    std::uint16_t contactYaw = actor.facingYaw;
    const std::size_t targetIndex = static_cast<std::size_t>(actor.targetSlotIndex);
    if (targetIndex < std::size(actor.contactPoints) && actor.contactPoints[targetIndex] < 3.0f && (actor.groundContactFlags & 1) == 0 &&
        (actor.targetActor->moveFlags & 0x20U) == 0) {
        contactYaw = actor.contactAngles[targetIndex];
    }
    actor.cameraAngleOffset = static_cast<std::uint16_t>(inputYaw - contactYaw - accumulatedCameraYaw + 0x8000U);
    if (actor.fightStyleIndex == 0) {
        game.partyFacingAngle = static_cast<std::int16_t>(actor.temporaryFacingAngle);
        if (sector != 0) {
            if (game.partyFacingFrames < 30)
                ++game.partyFacingFrames;
            if (game.partyFacingFrames > 15)
                game.partyFacingAngle = actor.cameraYaw;
        } else if (game.partyFacingFrames != 0) {
            --game.partyFacingFrames;
        }
    }
    actor.reactionOverrideActive = 0;
    vfQueueReactionFlagsForFrame(actor, inputConsumed);
    vfUpdateReactionOverrideState(actor);
    actor.reactionPriorityMaskA = 32;
    actor.reactionPriorityMaskB = 16;
    vfUpdateDeferredReactionPriority(actor, 0);
    const std::uint32_t hitState = actor.hitStateFlags2;
    const std::uint32_t move = actor.moveFlags;
    std::uint8_t selection = (hitState & 0x800U) == 0 ? 1U : 0U;
    if ((actor.groundContactFlags & 1) == 0)
        selection |= 2U;
    if ((actor.priorityMask & 0x20) == 0)
        selection |= 4U;
    if ((move & 2U) == 0)
        selection |= 8U;
    actor.hitReactionAnimSelectFlags = static_cast<std::int8_t>(selection);
    if ((move & 0x20U) != 0) {
        if ((move & 0x10U) == 0 && (hitState & 1U) == 0) {
            actor.nextAction = {};
            actor.nextAction.state = 1;
        }
        return;
    }

    std::uint32_t queued = 0;
    if (actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionEndFrame - 1U))
        queued = actor.endFrameQueuedReactionActionWord;
    if (!queued && actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionMidFrame - 1U))
        queued = static_cast<std::uint32_t>(actor.midFrameQueuedReactionActionWord);
    if (queued) {
        actor.nextAction = {};
        std::memcpy(&actor.nextAction, &queued, sizeof(queued));
        actor.midFrameQueuedReactionActionWord = 0;
        actor.endFrameQueuedReactionActionWord = 0;
        return;
    }
    if (actor.queuedAction.state == 13 && !actor.reactionResumeProgramCounter) {
        if (actor.hitPhase < 3U && actor.priorityFlags != 0)
            ++actor.hitPhaseWaitFrames;
        return;
    }
    if (vfUpdateReactionQueueState(actor) == VFStateMachineResult::YieldToCaller)
        return;
    if (actor.reactionResumeProgramCounter) {
        bool resume = actor.delayedReactionStateActive != 0;
        if (actor.delayedReactionTimer != 0) {
            if (actor.delayedReactionTimer != 0x7fffU && static_cast<std::uint16_t>(actor.currentAnimationEndFrame + 1U) < actor.delayedReactionTimer)
                return;
            if (actor.delayedReactionTimer != 0x7fffU)
                actor.delayedReactionTimer = 0;
            resume = true;
        }
        if (resume) {
            actor.functionReactionSlotIndex = actor.slots.primary;
            actor.reactionResumeProgramCounter = nullptr;
            if (RunReactionStateMachine(actor) != VFStateMachineResult::YieldToCaller) {
                actor.slots.primary = static_cast<std::uint8_t>(actor.reactionRingTail);
                actor.slots.secondary = actor.reactionRingTail;
                actor.reactionSlotCurrent = static_cast<std::uint8_t>(actor.reactionRingTail);
            }
            return;
        }
    }
    if ((actor.reactionOverrideActive & 1) != 0) {
        if (!actor.rootReactionVmProgramCounter && !actor.reactionVmProgramCounter) {
            vfClampReactionQueueLength(actor);
            return;
        }
    } else if ((actor.reactionStateFlags & 4) != 0) {
        vfClampReactionQueueLength(actor);
    }
    actor.rewindReactionQueueTail = 0;
    actor.reactionQueuePhase = 1;
    while (static_cast<std::uint8_t>(actor.reactionRingTail) != actor.reactionRingHead) {
        const auto tail = static_cast<std::uint8_t>(actor.reactionRingTail) & 31U;
        const VFReactionPriorityMeta &priority = actor.reactionPriority[tail];
        actor.priorityMask = static_cast<std::int16_t>(priority.mask1);
        const std::uint16_t flags = priority.flags;
        actor.priorityFlags = flags;
        actor.field6c4 = priority.unknownFlags;
        vfUpdateDeferredReactionPriority(actor, 1);
        actor.reactionRingTail = static_cast<std::int8_t>((tail + 1U) & 31U);
        actor.reactionConditionFlags = (actor.priorityFlags & 0x0bU) | ((actor.priorityFlags & 4U) << 2U);
        if (actor.reactionConditionFlags) {
            if ((actor.priorityFlags & 0x0100U) != 0)
                actor.reactionConditionFlags |= 4U;
            if ((actor.moveFlags & 0x10U) != 0)
                actor.reactionConditionFlags |= 0x500U;
            if (vfHandleSavedCharRootPosReaction(actor) == VFStateMachineResult::YieldToCaller || actor.reactionChainFrames > 0)
                return;
        }
        if ((actor.reactionOverrideActive & 1) == 0 && (flags & 7U) == 0 && (actor.priorityFlags | actor.priorityMaskDifference) != 0) {
            actor.functionReactionSlotIndex = actor.slots.secondary;
            actor.reactionConditionFlags = 0;
            actor.currentReactionFlagFilter = 0x01f0;
            actor.previousReactionFlagFilter = 0x00ff;
            if (RunReactionStateMachine(actor) == VFStateMachineResult::YieldToCaller)
                return;
        }
    }
    if (actor.rewindReactionQueueTail)
        actor.reactionRingTail = static_cast<std::int8_t>(actor.savedReactionQueueTail);
    actor.reactionQueuePhase = 2;
    if ((actor.reactionOverrideActive & 1) != 0 || (actor.hitStateFlags2 & 0x1000U) != 0)
        return;
    const std::uint32_t motionFlags = actor.motionFlags;
    const std::uint16_t commands = actor.cameraControlFlags;
    if ((motionFlags & 0x00800000U) != 0) {
        if ((commands & 0x40U) != 0)
            actor.facingAngle = static_cast<std::uint16_t>(actor.facingAngle - 1024U);
        else if ((commands & 0x80U) != 0)
            actor.facingAngle = static_cast<std::uint16_t>(actor.facingAngle + 1024U);
        return;
    }
    actor.nextAction = {};
    if ((commands & 0x1000U) != 0) {
        actor.nextAction.state = (commands & 0xc0U) != 0 ? 44U : (actor.stateFlags & 0x0cU) != 0 ? 39U : 1U;
        return;
    }
    if ((commands & 0x2000U) != 0) {
        actor.nextAction.state = 43;
        return;
    }
    actor.followupState = 0;
    actor.forcedTurnState = 0;
    actor.fielda80 = 0;
    const bool followup = (motionFlags & 0x08000000U) != 0;
    if (!followup || (actor.hitFlags & 4U) == 0) {
        if ((commands & 0x4000U) != 0 && (game.flags & 0x3bU) == 0 &&
            (actor.slotIndex != static_cast<std::uint8_t>(game.playerSlot) || game.runningCooldownFrames == 0)) {
            actor.followupState = 3;
            if (actor.slotIndex == static_cast<std::uint8_t>(game.playerSlot)) {
                if (game.runningFrames < 0xfde8U)
                    ++game.runningFrames;
                if (game.runningFrames > 300U) {
                    actor.followupState = 2;
                    game.runningFrames = 0;
                    game.runningCooldownFrames = 60;
                }
            }
            actor.nextAction.state = 38;
            return;
        }
        if (followup && (actor.hitFlags & 0x404U) == 0) {
            actor.followupState = 2;
            actor.nextAction.state = 38;
            return;
        }
        const float range = (hitState & 0x400U) != 0 && followup ? 3.7f : 4.0f;
        bool nearbyEnemy = false;
        for (std::size_t index = 0; index < kBattleSlots; ++index) {
            if (index != actor.slotIndex && game.characters[index] && game.slotTeamId[index] != game.slotTeamId[actor.slotIndex] &&
                actor.contactPoints[index] < range) {
                nearbyEnemy = true;
                break;
            }
        }
        if (!nearbyEnemy && (actor.priorityMask & 0xf0) != 0 && (actor.hitStateFlags & 0x100U) == 0) {
            actor.followupState = 1;
            actor.nextAction.state = 38;
            return;
        }
        if ((actor.priorityMask & 0xf0) != 0) {
            const auto cameraAngle = static_cast<std::int16_t>(
                actor.slotIndex != static_cast<std::uint8_t>(game.playerSlot) || (game.gameFlags & 0x400000U) != 0 ? actor.cameraAngle :
                                                                                                                     actor.cameraAngleOffset);
            actor.nextAction.state = 37;
            if (std::abs(static_cast<std::int16_t>(cameraAngle + 0x8000U)) <= 8192 && (actor.groundContactFlags & 1) != 0 && actor.targetSlotIndex >= 0 &&
                actor.contactPoints[static_cast<std::uint8_t>(actor.targetSlotIndex)] < 4.0f) {
                actor.slots.secondary = actor.reactionRingTail;
                actor.reactionSlotCurrent = static_cast<std::uint8_t>(actor.reactionRingTail);
                actor.nextAction.state = 7;
            }
            return;
        }
    }
    actor.moveAngleTableIndex2 = actor.moveAngleTableIndex;
    actor.nextAction.state = (actor.stateFlags & 0x0cU) != 0 ? 39U : 1U;
}

constexpr std::array<std::uint32_t, 44> kVFMoveCategoryMasks{0, 8, 64, 16, 128, 48, 384, 2048, 16384, 6144, 49152, 6144, 49152, 432, 7168, 57344, 2, 4, 514, 6,
    514, 55296, 24, 192, 26, 194, 56, 448, 55296, 18432, 6192, 49536, 66, 66, 9730, 76, 20480, 34816, 55296, 432, 504, 0xffffffffU, 65026, 1022};

} // namespace shenmue::s1::vf_detail
