#include "s1/task/vf_internal.hpp"

namespace shenmue::s1::vf_detail {

// Reaction bytecode operands: the state/resolve pair and parameter pairs are signed big-endian 16-bit values (branch offsets from
// reactionScriptBase), and the four parameter bytes form a big-endian 32-bit value.
std::uint16_t vfReactOperand(const VFActionDescriptor &instruction) {
    return static_cast<std::uint16_t>(instruction.state | (instruction.resolve << 8));
}

std::int16_t vfReactS16(const std::byte *bytes) {
    return static_cast<std::int16_t>((std::to_integer<std::uint8_t>(bytes[0]) << 8) | std::to_integer<std::uint8_t>(bytes[1]));
}

std::uint32_t vfReactU32(const std::byte *bytes) {
    return (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(bytes[0])) << 24U) |
        (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(bytes[1])) << 16U) |
        (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(bytes[2])) << 8U) | std::to_integer<std::uint8_t>(bytes[3]);
}

VFActionDescriptor *vfReactBranch(const VFActor &actor, std::int64_t offset) {
    return reinterpret_cast<VFActionDescriptor *>(static_cast<std::byte *>(actor.reactionScriptBase) + offset);
}

// The action word is stored sign-extended into the 8-byte descriptor.
void vfReactStoreAction(VFActionDescriptor &destination, std::uint32_t word) {
    const std::int64_t value = static_cast<std::int32_t>(word);
    std::memcpy(&destination, &value, sizeof(value));
}

// Frame counts are scaled by 1/timeScale in double precision and truncated to the low byte.
std::uint8_t vfReactScaleFrames(const VFActor &actor, std::int32_t frames) {
    const double value = 1.0 / static_cast<double>(actor.timeScale) * static_cast<double>(frames) + 0.5;
    return value > -9.2233720368547758e18 && value < 9.2233720368547758e18 ? static_cast<std::uint8_t>(static_cast<std::int64_t>(value)) : 0;
}

std::uint16_t vfReactAliasMask(const VFActor &actor, std::uint16_t operand, std::uint16_t mask) {
    if ((operand & 0x400U) != 0)
        mask = static_cast<std::uint16_t>(mask | actor.reactionPriorityMaskA);
    if ((operand & 0x800U) != 0)
        mask = static_cast<std::uint16_t>(mask | actor.reactionPriorityMaskB);
    return mask;
}

std::uint32_t vfReactSelectAliasMask(const VFActor &actor, std::uint16_t operand) {
    if ((operand & 0x400U) != 0)
        return actor.reactionPriorityMaskA;
    if ((operand & 0x800U) != 0)
        return static_cast<std::uint32_t>(actor.reactionPriorityMaskB);
    return operand;
}

std::int16_t vfReactContactYaw(const VFActor &actor, std::uint16_t sectorMask) {
    const std::int8_t target = actor.targetSlotIndex;
    const bool useContact = actor.contactPoints[target] < 3.0f && (actor.groundContactFlags & 1) == 0 && (actor.targetActor->moveFlags & 0x20U) == 0;
    const std::uint16_t facing = useContact ? actor.contactAngles[target] : actor.facingYaw;
    return static_cast<std::int16_t>(g_vfCameraYawAngles[(sectorMask >> 4U) & 0x0fU] - facing);
}

std::int16_t vfCalcFacingYawDelta(const VFActor &actor, std::uint16_t sectorMask) {
    return static_cast<std::int16_t>(vfReactContactYaw(actor, sectorMask) + 0x8000 - static_cast<std::int16_t>(g_fCamAccumYaw));
}

float vfReactAbsYaw(std::int32_t yaw) {
    return std::fabs(static_cast<float>(static_cast<std::int16_t>(yaw)));
}

VFReactResponse vfReactContinueOrScan(const VFActor &actor, std::uint16_t priorityMask) {
    return priorityMask != actor.lastPriorityMaskSnapshot ? VFReactResponse::CheckPriorityWindow : VFReactResponse::ContinuePriorityScan;
}

bool PopReactionState(VFActor &actor) {
    if (--actor.reactionStateStackDepth < 0)
        return false;
    const VFReactionStateStackEntry &entry = actor.reactionStateStack[static_cast<std::uint8_t>(actor.reactionStateStackDepth)];
    if (entry.skipPriorityRestore == 0) {
        actor.priorityFlags = entry.savedPriorityFlags;
        actor.priorityMask = static_cast<std::int16_t>(entry.savedPriorityMask);
        actor.lastPriorityMaskSnapshot = entry.savedLastPriorityMask;
        actor.reactionPriorityIndex = entry.savedPriorityIndex;
        actor.priorityWindowAllowedMask = entry.savedWindowAllowedMask;
        actor.priorityWindowFramesRemaining = entry.savedWindowFramesRemaining;
        actor.priorityScanCounter = static_cast<std::uint32_t>(entry.savedPriorityScanCounter);
    }
    actor.reactionVmProgramCounter = entry.savedProgramCounter;
    return actor.reactionVmProgramCounter != nullptr;
}

VFReactResponse vfReactAdvanceIfPrioFlagsAndMaskMatch(VFActor &actor) {
    const std::uint16_t flags = actor.priorityFlags;
    if (!flags)
        return VFReactResponse::ContinuePriorityScan;
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint16_t mask = static_cast<std::uint8_t>(vfReactAliasMask(actor, vfReactOperand(*instruction), instruction->state));
    if (mask != flags || mask != static_cast<std::uint16_t>(actor.priorityMask))
        return VFReactResponse::PopState;
    actor.lastPriorityMaskSnapshot = mask;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactAdvanceIfPrioMaskMatches(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint16_t operand = vfReactOperand(*instruction);
    const std::uint16_t mask = vfReactAliasMask(actor, operand, operand & 0xffU);
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    if (mask != priorityMask)
        return vfReactContinueOrScan(actor, priorityMask);
    actor.lastPriorityMaskSnapshot = mask;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Custom2;
}


VFReactResponse vfReactAdvanceIfPrioMaskIntersects(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    if ((vfReactSelectAliasMask(actor, vfReactOperand(*instruction)) & priorityMask) == 0)
        return vfReactContinueOrScan(actor, priorityMask);
    actor.lastPriorityMaskSnapshot = priorityMask;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Custom2;
}


VFReactResponse vfReactOpenPriorityWindow(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint8_t mask = static_cast<std::uint8_t>(vfReactAliasMask(actor, vfReactOperand(*instruction), instruction->state));
    const std::uint8_t frames = vfReactScaleFrames(actor, static_cast<std::int8_t>(instruction->index));
    actor.priorityWindowAllowedMask = mask;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    actor.priorityWindowFramesRemaining = frames;
    if (actor.lastPriorityMaskSnapshot != priorityMask && (~static_cast<std::uint32_t>(mask) & priorityMask) != 0)
        return VFReactResponse::PopState;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactSetPriorityScanDuration(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint8_t frames = vfReactScaleFrames(actor, static_cast<std::int8_t>(instruction->index));
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    actor.priorityScanCounter = frames;
    return VFReactResponse::Continue;
}

VFReactResponse vfOnNewReactionAction(VFActor &actor) {
    bool ryoStyle = false;
    VFMAData &game = *actor.game;
    const std::uint8_t fighterId = actor.fighterRankId;
    vfReactStoreAction(actor.nextAction, vfReactU32(actor.reactionVmProgramCounter->parameters));
    if (actor.fightStyleIndex == 0) {
        ryoStyle = true;
        if (game.battleType != 5)
            vfRegisterAnimSetMoveUsage(fighterId);
        vfmaUpdateCollisionCounter(game, VFCollisionCounterEvent::ContactRegistered, fighterId);
        if (game.battleType == 4)
            vfUnlockFreeBattleMove(game, fighterId);
        if (fighterId != 87)
            actor.fighterIdAlt = fighterId;
        if (fighterId < 80)
            actor.fighterIdAlt2 = fighterId;
    }
    if (actor.nextAction.state == 16) {
        if (ryoStyle)
            vfmaUpdateCollisionCounter(game, VFCollisionCounterEvent::HitConfirmed, fighterId);
        if ((actor.triggerMask & 0x400U) != 0) {
            if (fighterId == actor.fighterRankMin)
                actor.triggerState |= 0x400U;
            else if (fighterId == actor.fighterRankMax)
                actor.triggerState |= 0x800U;
        }
        if (actor.fightStyleIndex == 0x1c && static_cast<std::uint8_t>(fighterId - 22) <= 3) {
            std::uint32_t phaseFlag = 2U << ((2 * fighterId - 44) & 31);
            if ((actor.targetActor->moveFlags & 0x400U) != 0)
                phaseFlag *= 2;
            game.extraValue |= phaseFlag;
            VFSlotRuntimeState &slot = *actor.slot;
            VFActor &target = *actor.targetActor;
            const std::uint8_t phase = slot.currentHpRegenerationPhase;
            if (phase == 0 || phase == 2) {
                slot.currentHpRegenerationPhase = static_cast<std::uint8_t>(phase + 1);
                if (phase == 0) {
                    actor.stateFlags &= ~4U;
                    target.stateFlags &= ~4U;
                    actor.stateFlags |= 8U;
                    target.stateFlags |= 8U;
                    if (fighterId == 22)
                        (void)vfAssignVoiceFor(game, 0);
                    else if (fighterId == 24)
                        (void)vfAssignVoiceFor(game, 13);
                } else if (fighterId == 23) {
                    (void)vfAssignVoiceFor(game, 7);
                } else if (fighterId == 25) {
                    (void)vfAssignVoiceFor(game, 17);
                }
                slot.hpPhaseIncrement = static_cast<std::uint16_t>(actor.currentHp + 450);
            }
        }
        for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
            if (game.characters[index])
                game.actors[index]->contactDecisionMask &= 0x300U;
        }
    } else if (game.reactionTypeIndex == 0 && (fighterId == 14 || fighterId == 43)) {
        vfSelectHitReactionSequence(actor, 2);
    }
    actor.delayedReactionTimer = 0;
    actor.delayedReactionStateActive = 0;
    actor.slots.primary = static_cast<std::uint8_t>(actor.reactionRingTail);
    actor.slots.secondary = actor.reactionRingTail;
    actor.reactionSlotCurrent = static_cast<std::uint8_t>(actor.reactionRingTail);
    return VFReactResponse::YieldToCaller;
}

VFReactResponse vfReactQueueActionAndYield(VFActor &actor) {
    if ((actor.motionFlags & 0x40000U) == 0) {
        const std::uint32_t word = vfReactU32(actor.reactionVmProgramCounter->parameters);
        actor.slots.secondary = actor.reactionRingTail;
        actor.reactionSlotCurrent = static_cast<std::uint8_t>(actor.reactionRingTail);
        vfReactStoreAction(actor.nextAction, word);
    }
    return VFReactResponse::YieldToCaller;
}

VFReactResponse vfReactRequireFlags(VFActor &actor, std::uint32_t value, bool require) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (((vfReactU32(instruction->parameters) & value) != 0) != require)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireGroundContactFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, static_cast<std::uint32_t>(actor.groundContactFlags), true);
}


VFReactResponse vfReactRequireMoveFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.moveFlags, true);
}


VFReactResponse vfReactRequireHitStateFlags2(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.hitStateFlags2, true);
}


VFReactResponse vfReactRejectGroundContactFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, static_cast<std::uint32_t>(actor.groundContactFlags), false);
}


VFReactResponse vfReactRejectMoveFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.moveFlags, false);
}


VFReactResponse vfReactRejectHitStateFlags2(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.hitStateFlags2, false);
}

VFReactResponse vfReactSetStatePtrAndAdvanceInline(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    VFActionDescriptor *const resume = vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*instruction)));
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    actor.reactionResumeProgramCounter = resume;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactBranchIfConditionFlagsMatch(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::int32_t condition = static_cast<std::int16_t>(vfReactOperand(*instruction));
    if (static_cast<std::uint32_t>(condition) == actor.reactionConditionFlags)
        actor.reactionVmProgramCounter = vfReactBranch(actor, vfReactS16(instruction->parameters));
    else
        actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}

void vfReactSaveStackEntry(VFActor &actor, VFReactionStateStackEntry &entry) {
    entry.savedPriorityFlags = actor.priorityFlags;
    entry.savedPriorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    entry.savedLastPriorityMask = actor.lastPriorityMaskSnapshot;
    entry.savedPriorityIndex = actor.reactionPriorityIndex;
    entry.savedWindowAllowedMask = actor.priorityWindowAllowedMask;
    entry.savedWindowFramesRemaining = vfReactScaleFrames(actor, actor.priorityWindowFramesRemaining);
}


VFReactResponse vfReactPushState(VFActor &actor) {
    VFReactionStateStackEntry &entry = actor.reactionStateStack[static_cast<std::uint8_t>(actor.reactionStateStackDepth)];
    vfReactSaveStackEntry(actor, entry);
    entry.savedProgramCounter = vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*actor.reactionVmProgramCounter)));
    entry.savedPriorityScanCounter = static_cast<std::int32_t>(actor.priorityScanCounter);
    entry.skipPriorityRestore = 0;
    ++actor.reactionStateStackDepth;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactEndAndResetChain(VFActor &) {
    return VFReactResponse::EndAndResetChain;
}


VFReactResponse vfReactJump(VFActor &actor) {
    actor.reactionVmProgramCounter = vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*actor.reactionVmProgramCounter)));
    return VFReactResponse::Continue;
}

VFReactResponse vfReactYieldUntilReactionFrame(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    std::uint16_t reference = actor.reactionEndFrame;
    if (instruction->state == 0)
        reference = actor.reactionMidFrame;
    else if (instruction->state == 2)
        reference = actor.reactionState02EndFrame;
    VFActionDescriptor *const next = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    actor.reactionVmProgramCounter = next;
    const auto timer = static_cast<std::uint16_t>(vfReactScaleFrames(actor, static_cast<std::int8_t>(instruction->index)) + reference);
    if (static_cast<std::uint16_t>(actor.currentAnimationEndFrame + 1) >= timer)
        return VFReactResponse::Continue;
    actor.slots.primary = static_cast<std::uint8_t>(actor.reactionRingTail);
    actor.slots.secondary = actor.reactionRingTail;
    actor.reactionSlotCurrent = static_cast<std::uint8_t>(actor.reactionRingTail);
    actor.delayedReactionTimer = timer;
    actor.reactionResumeProgramCounter = next;
    return VFReactResponse::YieldToCaller;
}


VFReactResponse vfReactRandomThreshold(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const auto roll = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125);
    const float threshold = static_cast<float>(static_cast<std::int32_t>(vfReactU32(instruction->parameters))) * 0.000015258789f;
    if (threshold < roll)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireHpPhaseSettled(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const VFSlotRuntimeState &slot = *actor.slot;
    if (slot.currentHpRegenerationPhase != instruction->index || slot.hpPhaseIncrement != 0)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactScheduleRootReaction(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const auto yaw = static_cast<std::int16_t>(vfReactOperand(*instruction));
    const std::uint8_t delay = vfReactScaleFrames(actor, static_cast<std::int8_t>(instruction->index));
    VFActionDescriptor *const program = vfReactBranch(actor, vfReactS16(instruction->parameters));
    if (yaw) {
        actor.scheduledReactionYaw = yaw;
        actor.reactionVmProgramCounter = instruction + 1;
        actor.alternateRootReactionDelayFrames = delay;
        actor.pendingAlternateRootReactionProgram = program;
    } else {
        actor.rootReactionDelayFrames = delay;
        actor.reactionVmProgramCounter = instruction + 1;
        actor.rootReactionVmProgramCounter = program;
    }
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireConditionFlags(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::int32_t condition = static_cast<std::int16_t>(vfReactOperand(*instruction));
    if (actor.reactionConditionFlags != static_cast<std::uint32_t>(condition))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactBeginIndefiniteDelay(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    actor.reactionResumeProgramCounter = instruction;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    actor.delayedReactionTimer = 0x7fff;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactClearDelayAndAdvance(VFActor &actor) {
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    actor.reactionResumeProgramCounter = nullptr;
    actor.delayedReactionTimer = 0;
    actor.delayedReactionStateActive = 0;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactClearRootReactPcAndAdvance(VFActor &actor) {
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    actor.rootReactionVmProgramCounter = nullptr;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireTargetDistanceAtLeast(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const float threshold = static_cast<float>(static_cast<std::int32_t>(vfReactU32(instruction->parameters))) * 0.000015258789f;
    if (!(actor.contactPoints[actor.targetSlotIndex] >= threshold))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireTargetDistanceBelow(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const float threshold = static_cast<float>(static_cast<std::int32_t>(vfReactU32(instruction->parameters))) * 0.000015258789f;
    if (!(threshold > actor.contactPoints[actor.targetSlotIndex]))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireQueuedActionFlags(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    std::uint32_t queued = 0;
    std::memcpy(&queued, &actor.queuedAction, sizeof(queued));
    if ((vfReactU32(instruction->parameters) & queued) == 0)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireBeforeReactionMidFrame(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const auto limit = static_cast<std::uint16_t>(vfReactScaleFrames(actor, static_cast<std::int8_t>(instruction->index)) + actor.reactionMidFrame);
    if (static_cast<std::uint16_t>(actor.currentAnimationEndFrame + 1) > limit)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactAdvanceIfPrioFlagsIntersect(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if ((vfReactOperand(*instruction) & actor.priorityFlags) == 0)
        return VFReactResponse::ContinuePriorityScan;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    actor.lastPriorityMaskSnapshot = static_cast<std::uint16_t>(actor.priorityMask);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactAdvance(VFActor &actor) {
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactSelectScanStopSlot(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint8_t slot = instruction->index ? static_cast<std::uint8_t>(actor.slots.secondary) : actor.slots.primary;
    actor.functionReactionSlotIndex = static_cast<std::int16_t>((actor.functionReactionSlotIndex & 0xff00) | slot);
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactPopState(VFActor &) {
    return VFReactResponse::PopState;
}


VFReactResponse vfReactSetReactionStateFlags(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    actor.reactionStateFlags |= static_cast<std::int32_t>(vfReactU32(instruction->parameters));
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireReactionStateFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, static_cast<std::uint32_t>(actor.reactionStateFlags), true);
}


VFReactResponse vfReactRequireTargetReactionState(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (static_cast<std::int32_t>(actor.targetActor->reactionStateId) != vfReactS16(instruction->parameters))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireGuardStatAtLeast(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (static_cast<std::uint8_t>(actor.guardStat) < instruction->index)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactSavePriorityWindowContext(VFActor &actor) {
    VFReactionStateStackEntry &entry = actor.reactionStateStack[static_cast<std::uint8_t>(actor.reactionStateStackDepth)];
    vfReactSaveStackEntry(actor, entry);
    entry.savedProgramCounter = nullptr;
    entry.skipPriorityRestore = 1;
    entry.savedPriorityScanCounter = static_cast<std::int32_t>(actor.priorityScanCounter);
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactPushStateWithoutPriorityRestore(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    actor.reactionStateStack[actor.reactionStateStackDepth].savedProgramCounter = vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*instruction)));
    actor.reactionStateStack[actor.reactionStateStackDepth].skipPriorityRestore = 1;
    ++actor.reactionStateStackDepth;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactBranchByScannedDirection(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint32_t mask2 = actor.reactionPriority[actor.reactionPriorityIndex].mask2;
    if ((mask2 & actor.reactionPriorityMaskA) != 0)
        actor.reactionVmProgramCounter = vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*instruction)));
    else if ((mask2 & static_cast<std::uint32_t>(actor.reactionPriorityMaskB)) != 0)
        actor.reactionVmProgramCounter = vfReactBranch(actor, vfReactS16(instruction->parameters));
    else
        actor.reactionVmProgramCounter = vfReactBranch(actor, vfReactS16(instruction->parameters + 2));
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRejectTargetHitStateFlags2(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.targetActor->hitStateFlags2, false);
}


VFReactResponse vfReactRequireTargetHitStateFlags2(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.targetActor->hitStateFlags2, true);
}


VFReactResponse vfReactAdvanceIfPrioMaskExcludes(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    if ((vfReactSelectAliasMask(actor, vfReactOperand(*instruction)) & priorityMask) != 0)
        return vfReactContinueOrScan(actor, priorityMask);
    actor.lastPriorityMaskSnapshot = priorityMask;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Custom2;
}

std::int8_t &vfReactCounter(VFActor &actor, std::uint8_t index) {
    return *(reinterpret_cast<std::int8_t *>(&actor) + offsetof(VFActor, reactionCounters) + static_cast<std::int8_t>(index));
}

VFReactResponse vfReactCounterSet(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    vfReactCounter(actor, instruction->resolve) = static_cast<std::int8_t>(instruction->index);
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactCounterIncrement(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    ++vfReactCounter(actor, instruction->index);
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactCounterDecrement(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    --vfReactCounter(actor, instruction->index);
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactBranchIfCounterGreater(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (vfReactCounter(actor, instruction->resolve) <= static_cast<std::int8_t>(instruction->index))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactBranchIfCounterEqual(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (vfReactCounter(actor, instruction->resolve) != static_cast<std::int8_t>(instruction->index))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactBranchIfCounterLess(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (vfReactCounter(actor, instruction->resolve) >= static_cast<std::int8_t>(instruction->index))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


bool vfReactFollowupWindowOpen(const VFActor &actor, std::uint16_t animationId) {
    if ((actor.hitStateFlags2 & 0x10U) == 0)
        return false;
    const std::uint8_t *const record = shenLookupAnimationIndex(static_cast<std::uint16_t>(animationId & 0x0fffU), 3);
    if (!record)
        return false;
    std::int16_t frame = 0;
    std::memcpy(&frame, record + 6, sizeof(frame));
    return static_cast<std::int16_t>(actor.currentAnimationEndFrame) < vfReactScaleFrames(actor, frame);
}


VFReactResponse vfReactQueueFollowupIfWindowOpen(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (!vfReactFollowupWindowOpen(actor, vfReactOperand(*instruction)))
        return VFReactResponse::PopState;
    vfReactStoreAction(actor.nextAction, vfReactU32(instruction->parameters));
    actor.delayedReactionTimer = 0;
    actor.delayedReactionStateActive = 0;
    actor.slots.primary = static_cast<std::uint8_t>(actor.reactionRingTail);
    actor.slots.secondary = actor.reactionRingTail;
    actor.reactionSlotCurrent = static_cast<std::uint8_t>(actor.reactionRingTail);
    return VFReactResponse::YieldToCaller;
}


VFReactResponse vfReactSetFallbackFighterId(VFActor &actor) {
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    actor.fighterIdAlt2 = 80;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactResetQueueCursorAndAdvance(VFActor &actor) {
    const std::uint8_t head = actor.reactionRingHead;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    actor.reactionRingTail = static_cast<std::int8_t>(head);
    actor.slots.primary = head;
    actor.slots.secondary = static_cast<std::int8_t>(head);
    actor.reactionSlotCurrent = head;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireBattleTypeMatch(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const bool matches = static_cast<std::int32_t>(actor.game->battleType) == static_cast<std::int8_t>(instruction->resolve);
    if (static_cast<std::uint8_t>(instruction->index ^ (matches ? 1U : 0U)) != 1)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse VF_FlagCheck(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint32_t flags = vfReactU32(instruction->parameters);
    if ((static_cast<std::uint32_t>(g_playerStats.currentVfSkill[flags >> 30U]) & flags & 0x3fffffffU) == 0)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactSetStatePtrAndEnd(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    actor.reactionChainFrames = -1;
    actor.reactionResumeProgramCounter = vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*instruction)));
    return VFReactResponse::End;
}

VFReactResponse vfReactAdvanceIfCurrentReactAnimMatches(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (static_cast<std::int16_t>(vfReactOperand(*instruction)) != static_cast<std::int32_t>(actor.currentReactionAnimation))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfUpdateReactionStateDelay(VFActor &actor) {
    if (!actor.delayedReactionStateActive) {
        VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
        const std::uint16_t midFrame = actor.reactionMidFrame;
        const std::uint16_t frame = actor.currentAnimationEndFrame;
        const std::uint8_t delay = vfReactScaleFrames(actor, static_cast<std::int16_t>(vfReactOperand(*instruction)));
        if (frame > static_cast<std::uint16_t>(midFrame + delay)) {
            actor.delayedReactionFramesRemaining = 0;
            actor.reactionVmProgramCounter = instruction + 1;
            actor.delayedReactionStateActive = 0;
            return VFReactResponse::Continue;
        }
        actor.delayedReactionStateActive = -1;
        actor.delayedReactionFramesRemaining = static_cast<std::uint16_t>(delay - frame + midFrame);
    }
    if (actor.delayedReactionFramesRemaining) {
        --actor.delayedReactionFramesRemaining;
        actor.reactionResumeProgramCounter = actor.reactionVmProgramCounter;
        return VFReactResponse::YieldToCaller;
    }
    ++actor.reactionVmProgramCounter;
    actor.delayedReactionFramesRemaining = 0;
    actor.delayedReactionStateActive = 0;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireContactDecisionFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.contactDecisionMask, true);
}


VFReactResponse vfReactRejectContactDecisionFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.contactDecisionMask, false);
}


VFReactResponse vfReactRequireTargetBeforeFrame(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint8_t frame = vfReactScaleFrames(actor, static_cast<std::int16_t>(vfReactOperand(*instruction)));
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return frame < actor.targetActor->currentAnimationEndFrame ? VFReactResponse::PopState : VFReactResponse::Continue;
}


VFReactResponse vfReactRequireContactActionFlags(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint32_t flags = vfReactU32(instruction->parameters);
    actor.reactionVmProgramCounter = instruction + 1;
    return (actor.contactActionMask & flags) != 0 ? VFReactResponse::Continue : VFReactResponse::PopState;
}


VFReactResponse vfReactRequirePairContactPriority(VFActor &actor) {
    return vfReactRequireFlags(actor, static_cast<std::uint32_t>(actor.latchedPairContactPriority), true);
}


VFReactResponse vfReactRequirePairContactUnlatched(VFActor &actor) {
    if ((actor.latchedPairContactPriority & 1) != 0)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactLatchPairContact(VFActor &actor) {
    actor.latchedPairContactPriority |= 1;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfOnCounterHitFollowup(VFActor &actor) {
    VFActor &source = *actor.hitSourceActor;
    vfReactStoreAction(actor.nextAction, static_cast<std::uint16_t>(source.pairFollowupActionWord) + 0x1b000000U);
    VFMAData &game = *actor.game;
    if (game.playerSlot != -1 && source.slotIndex == static_cast<std::uint8_t>(game.playerSlot))
        vfmaUpdateCollisionCounter(game, VFCollisionCounterEvent::HitSequenceEnded, static_cast<std::uint16_t>(static_cast<std::int8_t>(source.fighterRankId)));
    actor.delayedReactionTimer = 0;
    actor.delayedReactionStateActive = 0;
    actor.slots.primary = static_cast<std::uint8_t>(actor.reactionRingTail);
    actor.slots.secondary = actor.reactionRingTail;
    actor.reactionSlotCurrent = static_cast<std::uint8_t>(actor.reactionRingTail);
    return VFReactResponse::YieldToCaller;
}


VFReactResponse vfReactRequireTargetGuardCoversHp(VFActor &actor) {
    if (static_cast<std::int16_t>(actor.targetActor->guardStrength) < actor.currentHp)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfStoreQueuedReactionActionWord(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint32_t word = vfReactU32(instruction->parameters);
    if (instruction->index)
        actor.endFrameQueuedReactionActionWord = word;
    else
        actor.midFrameQueuedReactionActionWord = static_cast<std::int32_t>(word);
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactBranchByAnimSelectFlags(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const auto row = static_cast<std::uint8_t>((instruction->index & static_cast<std::uint8_t>(actor.hitReactionAnimSelectFlags)) << 2);
    const std::byte *const table = reinterpret_cast<const std::byte *>(vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*instruction))));
    actor.reactionVmProgramCounter = vfReactBranch(actor, static_cast<std::int32_t>(vfReactU32(table + 4U * row)));
    return VFReactResponse::Continue;
}


VFReactResponse vfReactSetFighterRankId(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    actor.fighterRankId = instruction->index;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}

VFReactResponse vfReactAdvanceIfPrioMaskCameraYawWithinParam(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint16_t operand = vfReactOperand(*instruction);
    const std::uint16_t mask = vfReactAliasMask(actor, operand, operand & 0xffU);
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    if (!priorityMask) {
        if (mask)
            return VFReactResponse::ContinuePriorityScan;
        actor.lastPriorityMaskSnapshot = 0;
        actor.reactionVmProgramCounter = instruction + 1;
        return VFReactResponse::Custom2;
    }
    if (!mask)
        return VFReactResponse::ContinuePriorityScan;
    if ((priorityMask & 0xfc0fU) != 0)
        return VFReactResponse::PopState;
    const std::int32_t yaw = vfReactContactYaw(actor, priorityMask) - static_cast<std::int16_t>(g_fCamAccumYaw) - g_vfCameraYawAngles[(mask >> 4U) & 0x0fU];
    if (!(static_cast<float>(vfReactS16(instruction->parameters)) >= vfReactAbsYaw(yaw)))
        return vfReactContinueOrScan(actor, priorityMask);
    actor.lastPriorityMaskSnapshot = priorityMask;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Custom2;
}

VFReactResponse vfReactAdvanceIfPrioMaskCameraAndFacingYawWithinParam(VFActor &actor) {
    const std::uint16_t flags = actor.priorityFlags;
    if (!flags)
        return VFReactResponse::ContinuePriorityScan;
    if ((flags & 0xfe0fU) != 0)
        return VFReactResponse::PopState;
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint16_t operand = vfReactOperand(*instruction);
    const std::uint16_t mask = vfReactAliasMask(actor, operand, operand & 0xffU);
    const auto cameraYaw = static_cast<std::uint16_t>(g_vfCameraYawAngles[(mask >> 4U) & 0x0fU] + 0x8000U);
    const std::int32_t yaw = vfReactContactYaw(actor, flags) - static_cast<std::int16_t>(g_fCamAccumYaw) - cameraYaw;
    const auto limit = static_cast<float>(vfReactS16(instruction->parameters));
    if (!(limit >= vfReactAbsYaw(yaw - 0x8000)))
        return VFReactResponse::PopState;
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    if (!(limit >= vfReactAbsYaw(vfCalcFacingYawDelta(actor, priorityMask) - cameraYaw)))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    actor.lastPriorityMaskSnapshot = priorityMask;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactBranchIfPrioMaskCameraYawWithin90Deg(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint16_t operand = vfReactOperand(*instruction);
    const std::uint16_t mask = vfReactAliasMask(actor, operand, operand & 0xffU);
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    if (!priorityMask) {
        if (mask)
            return VFReactResponse::ContinuePriorityScan;
        actor.lastPriorityMaskSnapshot = 0;
        actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
        return VFReactResponse::Custom2;
    }
    if (!mask)
        return VFReactResponse::ContinuePriorityScan;
    if ((priorityMask & 0xfc0fU) != 0)
        return VFReactResponse::PopState;
    const std::int32_t yaw = vfReactContactYaw(actor, priorityMask) - static_cast<std::int16_t>(g_fCamAccumYaw) - g_vfCameraYawAngles[(mask >> 4U) & 0x0fU];
    if (!(16384.0f >= vfReactAbsYaw(yaw)))
        return vfReactContinueOrScan(actor, priorityMask);
    actor.lastPriorityMaskSnapshot = priorityMask;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Custom2;
}


VFReactResponse vfReactAdvanceIfPrioMaskCameraYawOutside90Deg(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint16_t operand = vfReactOperand(*instruction);
    const std::uint16_t mask = vfReactAliasMask(actor, operand, operand & 0xffU);
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    const std::int32_t yaw = vfReactContactYaw(actor, priorityMask) - static_cast<std::int16_t>(g_fCamAccumYaw) - g_vfCameraYawAngles[(mask >> 4U) & 0x0fU];
    if (16384.0f >= vfReactAbsYaw(yaw))
        return vfReactContinueOrScan(actor, priorityMask);
    actor.lastPriorityMaskSnapshot = priorityMask;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Custom2;
}


VFReactResponse vfReactBranchByScannedCameraYaw(VFActor &actor) {
    const std::uint16_t sector = actor.reactionPriority[actor.reactionPriorityIndex].value;
    const std::int32_t yaw = vfReactContactYaw(actor, sector) - static_cast<std::int16_t>(g_fCamAccumYaw) + 0x8000;
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const auto *const bytes = reinterpret_cast<const std::byte *>(instruction);
    if (static_cast<float>(vfReactS16(bytes + 8)) >= vfReactAbsYaw(yaw))
        actor.reactionVmProgramCounter = vfReactBranch(actor, static_cast<std::int16_t>(vfReactOperand(*instruction)));
    else if (static_cast<float>(vfReactS16(bytes + 10)) >= vfReactAbsYaw(yaw - 0x8000))
        actor.reactionVmProgramCounter = vfReactBranch(actor, vfReactS16(instruction->parameters));
    else
        actor.reactionVmProgramCounter = vfReactBranch(actor, vfReactS16(instruction->parameters + 2));
    return VFReactResponse::Continue;
}


VFReactResponse vfReactOpenCameraYawPriorityWindow(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint16_t operand = vfReactOperand(*instruction);
    actor.priorityWindowAllowedMask = vfReactAliasMask(actor, operand, operand & 0xf0U);
    VFActionDescriptor *const next = instruction + 1;
    actor.reactionVmProgramCounter = next;
    actor.priorityWindowFramesRemaining = vfReactScaleFrames(actor, static_cast<std::int8_t>(instruction->index));
    const auto priorityMask = static_cast<std::uint16_t>(actor.priorityMask);
    const std::int32_t yaw = vfReactContactYaw(actor, priorityMask) - static_cast<std::int16_t>(g_fCamAccumYaw);
    if (priorityMask == actor.lastPriorityMaskSnapshot)
        return VFReactResponse::Continue;
    if ((priorityMask & 0xf0U) == 0)
        return VFReactResponse::PopState;
    const auto limit = static_cast<float>(static_cast<std::int16_t>(0x8000 - vfReactS16(next->parameters)));
    return limit < vfReactAbsYaw(yaw) ? VFReactResponse::Continue : VFReactResponse::PopState;
}


VFReactResponse vfReactSelectStageHitObjectSide(VFActor &actor) {
    std::uint8_t side = actor.reactionVmProgramCounter->state;
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
        if ((object->stateFlags & 1U) != 0 && object->character == actor.character)
            return VFReactResponse::PopState;
    }
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
        if (object->state != 0 || (object->stateFlags & 1U) != 0 || (object->flags & 0xb00000U) != 0xb00000U ||
            ((object->environmentFlags >> actor.slotIndex) & 1) == 0) {
            continue;
        }
        if (object->object->state == 2) {
            const Vector3 otherPosition = object->otherCharacter ? GetPositionForEntity(object->otherCharacter->entity.node) : Vector3{};
            std::int16_t otherYaw = 0;
            if (object->otherCharacter) {
                for (const CharacterNode *node = &object->otherCharacter->entity.node; node; node = node->child)
                    otherYaw = static_cast<std::int16_t>(otherYaw + node->rotation.y);
            }
            const std::uint16_t towardActor =
                Shenmue_Atan2Fixed(actor.contactCurrentPosition.x - otherPosition.x, actor.contactCurrentPosition.z - otherPosition.z);
            const std::int32_t difference = otherYaw - static_cast<std::int32_t>(towardActor);
            const auto absolute = static_cast<std::int32_t>(std::fabs(static_cast<double>(static_cast<float>(difference))));
            if (static_cast<std::uint16_t>(absolute - 10241) <= 12286U) {
                if (static_cast<std::int32_t>(actor.stageHitObjectIndex) == object->objectIndex)
                    actor.stageHitObjectIndex = -1;
                continue;
            }
        }
        const std::uint16_t yaw = Shenmue_Atan2Fixed(actor.contactCurrentPosition.x - object->position.x, actor.contactCurrentPosition.z - object->position.z);
        if ((static_cast<std::uint16_t>(0x4000U - actor.temporaryFacingAngle + yaw) & 0x8000U) != 0)
            side ^= 1U;
        if (side == 0) {
            actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
            actor.stageHitObjectIndex = static_cast<std::int8_t>(object->objectIndex);
            return VFReactResponse::Continue;
        }
    }
    return VFReactResponse::PopState;
}


VFReactResponse vfReactRequireMotionFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.motionFlags, true);
}


VFReactResponse vfReactRequireHitFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.hitFlags, true);
}


VFReactResponse vfReactRejectHitFlags(VFActor &actor) {
    return vfReactRequireFlags(actor, actor.hitFlags, false);
}


VFReactResponse vfReactRequireTargetHpAtMost(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (actor.targetActor->currentHp > static_cast<std::int16_t>(vfReactOperand(*instruction)))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireTargetFightStyle(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (static_cast<std::int32_t>(actor.targetActor->fightStyleIndex) != static_cast<std::int8_t>(instruction->index))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


std::uint8_t vfReadMoveAnimType(std::uint8_t moveId) {
    static constexpr std::array<std::uint8_t, 5> kAnimSetTypes{0, 0, 32, 64, 80};
    std::uint8_t bank = 1;
    while (moveId >= kAnimSetTypes[bank + 1]) {
        if (++bank >= 4)
            return 0xff;
    }
    const auto local = static_cast<std::uint8_t>(moveId - kAnimSetTypes[bank]);
    if (local == 0xff)
        return 0xff;
    return g_vfMoveAnimTypeLut[0].data()[20 * (bank - 1) + local];
}


VFReactResponse vfReactRequireMoveAnimTypeAtLeast(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint8_t type = vfReadMoveAnimType(instruction->index);
    if (type == 0xff || type < instruction->resolve)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireHpRegenerationPhase(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (actor.slot->currentHpRegenerationPhase != instruction->index)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireTargetGuardState(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    if (static_cast<std::int32_t>(actor.targetActor->guardState) != static_cast<std::int8_t>(instruction->index))
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(instruction->parameters);
    return VFReactResponse::Continue;
}


VFReactResponse vfReactBranchByConditionClass(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint32_t condition = actor.reactionConditionFlags;
    const std::int32_t conditionClass = static_cast<std::int16_t>(vfReactOperand(*instruction));
    if (static_cast<std::uint32_t>(conditionClass) == (condition & 0xff00U) && (condition & 0x1fU) != 0)
        actor.reactionVmProgramCounter = vfReactBranch(actor, vfReactS16(instruction->parameters));
    else
        actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}


VFReactResponse vfReactRequireTargetStateFlagBit(VFActor &actor) {
    VFActionDescriptor *const instruction = actor.reactionVmProgramCounter;
    const std::uint8_t bit = static_cast<std::uint8_t>(vfReactU32(instruction->parameters)) & 31U;
    if ((actor.targetActor->stateFlags2 & (1U << bit)) == 0)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = instruction + 1;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactStoreNestedActorChara(VFActor &actor) {
    Character *const character = actor.targetActor->character;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    actor.savedNestedActorCharacter = character;
    return VFReactResponse::Continue;
}

VFReactResponse vfReactAdvanceIfNestedActorCharaMatches(VFActor &actor) {
    if (actor.savedNestedActorCharacter != actor.targetActor->character)
        return VFReactResponse::PopState;
    actor.reactionVmProgramCounter = reinterpret_cast<VFActionDescriptor *>(actor.reactionVmProgramCounter->parameters);
    return VFReactResponse::Continue;
}

constexpr VFReactResponse (*g_vfReactCallbacks[93])(VFActor &){
    vfReactAdvanceIfPrioFlagsAndMaskMatch,
    vfReactAdvanceIfPrioMaskMatches,
    vfReactAdvanceIfPrioMaskIntersects,
    vfReactOpenPriorityWindow,
    vfReactSetPriorityScanDuration,
    vfOnNewReactionAction,
    vfReactQueueActionAndYield,
    vfReactRequireGroundContactFlags,
    vfReactRequireMoveFlags,
    vfReactRequireHitStateFlags2,
    vfReactRejectGroundContactFlags,
    vfReactRejectMoveFlags,
    vfReactRejectHitStateFlags2,
    vfReactSetStatePtrAndAdvanceInline,
    vfReactBranchIfConditionFlagsMatch,
    vfReactPushState,
    vfReactEndAndResetChain,
    vfReactJump,
    vfReactYieldUntilReactionFrame,
    vfReactRandomThreshold,
    vfReactRequireHpPhaseSettled,
    vfReactScheduleRootReaction,
    vfReactRequireConditionFlags,
    vfReactBeginIndefiniteDelay,
    vfReactClearDelayAndAdvance,
    vfReactClearRootReactPcAndAdvance,
    vfReactRequireTargetDistanceAtLeast,
    vfReactRequireTargetDistanceBelow,
    vfReactRequireQueuedActionFlags,
    vfReactRequireBeforeReactionMidFrame,
    vfReactAdvanceIfPrioFlagsIntersect,
    vfReactAdvance,
    vfReactSelectScanStopSlot,
    vfReactPopState,
    vfReactSetReactionStateFlags,
    vfReactRequireReactionStateFlags,
    vfReactRequireTargetReactionState,
    vfReactRequireGuardStatAtLeast,
    vfReactSavePriorityWindowContext,
    vfReactPushStateWithoutPriorityRestore,
    vfReactBranchByScannedDirection,
    vfReactRejectTargetHitStateFlags2,
    vfReactRequireTargetHitStateFlags2,
    vfReactAdvanceIfPrioMaskExcludes,
    vfReactCounterSet,
    vfReactCounterIncrement,
    vfReactCounterDecrement,
    vfReactBranchIfCounterGreater,
    vfReactBranchIfCounterEqual,
    vfReactBranchIfCounterLess,
    vfReactQueueFollowupIfWindowOpen,
    vfReactSetFallbackFighterId,
    vfReactResetQueueCursorAndAdvance,
    vfReactRequireBattleTypeMatch,
    VF_FlagCheck,
    vfReactSetStatePtrAndEnd,
    vfReactAdvanceIfCurrentReactAnimMatches,
    vfUpdateReactionStateDelay,
    vfReactRequireContactDecisionFlags,
    vfReactRejectContactDecisionFlags,
    vfReactPopState,
    vfReactRequireTargetBeforeFrame,
    vfReactRequireContactActionFlags,
    vfReactAdvance,
    vfReactRequirePairContactPriority,
    vfReactRequirePairContactUnlatched,
    vfReactLatchPairContact,
    vfOnCounterHitFollowup,
    vfReactRequireTargetGuardCoversHp,
    vfStoreQueuedReactionActionWord,
    vfReactBranchByAnimSelectFlags,
    vfReactSetFighterRankId,
    vfReactAdvanceIfPrioMaskCameraYawWithinParam,
    vfReactAdvanceIfPrioMaskCameraAndFacingYawWithinParam,
    vfReactBranchIfPrioMaskCameraYawWithin90Deg,
    vfReactAdvanceIfPrioMaskCameraYawOutside90Deg,
    vfReactBranchByScannedCameraYaw,
    vfReactOpenCameraYawPriorityWindow,
    vfReactAdvance,
    vfReactSelectStageHitObjectSide,
    vfReactRequireMotionFlags,
    vfReactRequireMotionFlags,
    vfReactRequireHitFlags,
    vfReactRejectHitFlags,
    vfReactRequireTargetHpAtMost,
    vfReactRequireTargetFightStyle,
    vfReactRequireMoveAnimTypeAtLeast,
    vfReactRequireHpRegenerationPhase,
    vfReactRequireTargetGuardState,
    vfReactBranchByConditionClass,
    vfReactRequireTargetStateFlagBit,
    vfReactStoreNestedActorChara,
    vfReactAdvanceIfNestedActorCharaMatches,
};


VFStateMachineResult RunReactionStateMachine(VFActor &actor) {
    if (!actor.reactionVmProgramCounter)
        return VFStateMachineResult::Finished;
    const std::int16_t savedPriorityMask = actor.priorityMask;
    const std::uint16_t savedLastPriorityMask = actor.lastPriorityMaskSnapshot;
    const std::uint16_t savedPriorityFlags = actor.priorityFlags;
    VFActionDescriptor *const savedProgramCounter = actor.reactionVmProgramCounter;
    const std::int64_t savedOverride = actor.reactionOverrideActive;
    actor.reactionStateStackDepth = 0;
    actor.reactionPriorityIndex = static_cast<std::uint8_t>((static_cast<std::uint8_t>(actor.reactionRingTail) - 1U) & 31U);
    const std::uint16_t scanMask = actor.queuedAction.state == 13 ? 0x01f2U : 0x00f0U;
    actor.priorityMask = static_cast<std::int16_t>(static_cast<std::uint16_t>(actor.priorityMask) & scanMask);
    actor.priorityFlags &= scanMask;
    actor.lastPriorityMaskSnapshot = static_cast<std::uint16_t>(actor.priorityMask);
    actor.priorityScanCounter = (actor.reactionPriorityIndex - static_cast<std::uint8_t>(actor.otherPriorityFlags)) & 31U;
    actor.priorityWindowFramesRemaining = 0;
    actor.priorityWindowAllowedMask = 0;
    const auto restore = [&] {
        actor.lastPriorityMaskSnapshot = savedLastPriorityMask;
        actor.priorityMask = savedPriorityMask;
        actor.priorityFlags = savedPriorityFlags;
        actor.reactionVmProgramCounter = savedProgramCounter;
        actor.reactionOverrideActive = savedOverride;
    };
    for (;;) {
        VFReactResponse response = g_vfReactCallbacks[actor.reactionVmProgramCounter->opcode](actor);
        if (response == VFReactResponse::CheckPriorityWindow) {
            if (actor.priorityWindowAllowedMask != 0) {
                const std::uint16_t remaining = actor.priorityWindowFramesRemaining;
                actor.priorityWindowFramesRemaining = static_cast<std::uint16_t>(remaining - 1U);
                if (remaining == 0) {
                    response = VFReactResponse::PopState;
                } else {
                    actor.lastPriorityMaskSnapshot = static_cast<std::uint16_t>(actor.priorityMask);
                    if (actor.priorityMask != 0 && ((~actor.priorityWindowAllowedMask & static_cast<std::uint16_t>(actor.priorityMask)) != 0)) {
                        response = VFReactResponse::PopState;
                    } else {
                        if (actor.priorityMask != 0) {
                            actor.priorityWindowAllowedMask = static_cast<std::uint16_t>(actor.priorityMask);
                        }
                        response = VFReactResponse::ContinuePriorityScan;
                    }
                }
            } else {
                response = actor.priorityMask != static_cast<std::int16_t>(actor.lastPriorityMaskSnapshot) ? VFReactResponse::ContinuePriorityScan :
                                                                                                             VFReactResponse::PopState;
            }
        }
        if (response == VFReactResponse::Continue || response == VFReactResponse::Custom2) {
            continue;
        }
        if (response == VFReactResponse::ContinuePriorityScan) {
            const std::int32_t scanCounter = static_cast<std::int32_t>(actor.priorityScanCounter);
            --actor.priorityScanCounter;
            const auto stopIndex = static_cast<std::uint8_t>(actor.functionReactionSlotIndex);
            bool pop = scanCounter < 0 || actor.reactionPriorityIndex == stopIndex;
            if (!pop) {
                const auto oldestIndex = static_cast<std::uint8_t>(actor.otherPriorityFlags);
                const auto index = static_cast<std::uint8_t>((actor.reactionPriorityIndex - 1U) & 31U);
                actor.reactionPriorityIndex = index;
                const VFReactionPriorityMeta &priority = actor.reactionPriority[index];
                actor.priorityMask = static_cast<std::int16_t>(priority.mask1 & 0xf0U);
                const std::uint8_t wrapped = oldestIndex >= index ? 1U : 0U;
                const std::uint8_t windowCount = stopIndex <= oldestIndex ? wrapped : static_cast<std::uint8_t>(wrapped + 1U);
                const std::uint8_t position = stopIndex <= index ? windowCount : static_cast<std::uint8_t>(windowCount - 1U);
                if (static_cast<std::int8_t>(position) < 1) {
                    actor.priorityFlags = priority.flags & actor.currentReactionFlagFilter;
                    pop = (priority.flags & actor.previousReactionFlagFilter) != actor.priorityFlags;
                } else {
                    actor.priorityFlags = priority.flags & 0xf0U;
                }
            }
            if (pop && !PopReactionState(actor)) {
                actor.reactionChainFrames = 0;
                break;
            }
            continue;
        }
        if (response == VFReactResponse::PopState) {
            if (!PopReactionState(actor)) {
                actor.reactionChainFrames = 0;
                break;
            }
            continue;
        }
        if (response == VFReactResponse::YieldToCaller) {
            return VFStateMachineResult::YieldToCaller;
        }
        if (response == VFReactResponse::EndAndResetChain) {
            actor.reactionChainFrames = 0;
        }
        if (response == VFReactResponse::End || response == VFReactResponse::EndAndResetChain) {
            break;
        }
    }
    restore();
    return VFStateMachineResult::Finished;
}
} // namespace shenmue::s1::vf_detail
