#include "s1/task/vf_internal.hpp"

namespace shenmue::s1::vf_detail {

static_assert(sizeof(VFContactVolumeRecord) == 0x14);


constexpr std::array<VFContactVolumeRecord, 10> kVFContactVolumes{{
    {{0.0900000036f, 0.0149999997f, 0.00499999989f}, 0.330000013f, 0x02, 0, 0x0002},
    {{0.00999999978f, -0.00499999989f, 0.144999996f}, 0.164000005f, 0x2f, 0, 0x0004},
    {{0.135000005f, 0.0199999996f, -0.00999999978f}, 0.208000004f, 0x0f, 0, 0x0008},
    {{0.230000004f, -0.00499999989f, -0.0399999991f}, 0.201000005f, 0x10, 0, 0x0010},
    {{0.135000005f, 0.0199999996f, 0.00999999978f}, 0.208000004f, 0x09, 0, 0x0040},
    {{0.230000004f, -0.00499999989f, 0.0399999991f}, 0.201000005f, 0x0a, 0, 0x0080},
    {{0.224999994f, 0.0f, 0.0f}, 0.263999999f, 0x1f, 0, 0x0400},
    {{0.305000007f, -0.00499999989f, -0.00499999989f}, 0.280999988f, 0x20, 0, 0x0800},
    {{0.224999994f, 0.0f, 0.0f}, 0.263999999f, 0x18, 0, 0x2000},
    {{0.305000007f, -0.00499999989f, 0.00499999989f}, 0.280999988f, 0x19, 0, 0x4000},
}};

constexpr std::array<VFContactVolumeRecord, 10> kVFContactVolumesMgot{{
    {{0.194999993f, 0.00499999989f, 0.00499999989f}, 0.476999998f, 0x02, 0, 0x0002},
    {{0.0250000004f, 0.00499999989f, 0.0549999997f}, 0.187000006f, 0x2f, 0, 0x0004},
    {{0.115000002f, -0.0250000004f, -0.0250000004f}, 0.216000006f, 0x0f, 0, 0x0008},
    {{0.119999997f, -0.00499999989f, -0.00999999978f}, 0.187999994f, 0x10, 0, 0x0010},
    {{0.104999997f, 0.0f, 0.0f}, 0.148000002f, 0x12, 0, 0x0020},
    {{0.115000002f, -0.0250000004f, 0.0250000004f}, 0.216000006f, 0x09, 0, 0x0040},
    {{0.119999997f, -0.00499999989f, 0.00999999978f}, 0.187999994f, 0x0a, 0, 0x0080},
    {{0.104999997f, 0.0f, 0.0f}, 0.148000002f, 0x0c, 0, 0x0100},
    {{0.0649999976f, 0.00499999989f, 0.0f}, 0.476999998f, 0x14, 0, 0x0200},
    {{0.254999995f, 0.0149999997f, 0.0f}, 0.264999986f, 0x1f, 0, 0x0400},
}};

std::uint16_t ReadBigEndian16(const std::byte *bytes) {
    return static_cast<std::uint16_t>((static_cast<std::uint16_t>(bytes[0]) << 8U) | static_cast<std::uint16_t>(bytes[1]));
}

std::uint32_t ReadBigEndian32(const std::byte *bytes) {
    return (static_cast<std::uint32_t>(bytes[0]) << 24U) | (static_cast<std::uint32_t>(bytes[1]) << 16U) | (static_cast<std::uint32_t>(bytes[2]) << 8U) |
        static_cast<std::uint32_t>(bytes[3]);
}

float ReadBigEndianFixed16(const std::byte *bytes) {
    return static_cast<float>(static_cast<std::int32_t>(ReadBigEndian32(bytes))) / 65536.0f;
}



void vfUpdateWorldTransformAndFacingAngle(VFActor &actor) {
    actor.position = actor.character ? GetPositionForEntity(actor.character->entity.node) : Vector3{};
    std::uint16_t yaw = 0;
    for (const CharacterNode *node = actor.character ? &actor.character->entity.node : nullptr; node; node = node->child) {
        yaw = static_cast<std::uint16_t>(yaw + node->rotation.y);
    }
    actor.facingAngle = yaw;
}

std::uint8_t vfStageResolveQuadrantForPointXZ(float x, float z, std::uint8_t quadrant);

void vfActorUpdateState(VFActor &actor);

void vfSelectDefaultReactionAndGuardAnims(VFActor &actor) {
    constexpr std::array<std::uint16_t, 3> baseOffsets{0x0000U, 0x0010U, 0x0014U};
    constexpr std::array<std::uint16_t, 3> defaultOffsets{0x0126U, 0x0128U, 0x012aU};
    constexpr std::array<std::uint16_t, 3> lowHpOffsets{0x0142U, 0x0128U, 0x012aU};
    constexpr std::array<std::uint16_t, 3> farOffsets{0x012cU, 0x012eU, 0x0130U};

    if (!actor.game || !actor.animationTable)
        return;
    const auto targetIndex = static_cast<std::size_t>(static_cast<std::uint8_t>(actor.targetSlotIndex));
    if (actor.game->battleType == 2U && targetIndex < std::size(actor.contactPoints) && actor.contactPoints[targetIndex] < 4.0f) {
        actor.defaultReactionAnimationId = 1327;
        actor.defaultGuardAnimationId = 1325;
        actor.defaultStanceAnimationId = 1326;
        return;
    }

    const std::array<std::uint16_t, 3> *offsets = &baseOffsets;
    if (actor.fightStyleIndex != 0U) {
        const std::uint8_t hpPhase = actor.slot ? actor.slot->currentHpRegenerationPhase : 0U;
        if (actor.fightStyleIndex == 6U || actor.fightStyleIndex == 0x1bU) {
            offsets = hpPhase <= 1U ? &defaultOffsets : &lowHpOffsets;
        } else if (actor.fightStyleIndex == 0x1cU || actor.fightStyleIndex == 0x1dU) {
            offsets = hpPhase <= 2U ? &defaultOffsets : &lowHpOffsets;
        } else if ((actor.stateFlags & 0x0cU) != 0U) {
            offsets = &defaultOffsets;
        }
    } else if (actor.targetActor && actor.guardLevel > actor.targetActor->guardLevel) {
        if (actor.currentHp < actor.maximumHp / 2) {
            offsets = &lowHpOffsets;
        } else if (targetIndex < std::size(actor.contactPoints) && actor.contactPoints[targetIndex] >= 4.0f) {
            offsets = &farOffsets;
        } else {
            offsets = &defaultOffsets;
        }
    }

    std::array<std::int16_t *, 3> destinations{&actor.defaultReactionAnimationId, &actor.defaultGuardAnimationId, &actor.defaultStanceAnimationId};
    for (std::size_t index = 0; index < destinations.size(); ++index) {
        const std::uint16_t offset = (*offsets)[index];
        *destinations[index] = static_cast<std::int16_t>((static_cast<std::uint16_t>(actor.animationTable[offset]) << 8U) | actor.animationTable[offset + 1U]);
    }
}

std::uint16_t ResolveMoveAnimationProgress(std::size_t bankIndex, std::uint16_t moveId) {
    const MoveSetRuntimeState &moveSet = g_playerStats.moveSets[bankIndex];
    for (std::size_t index = 0; index < moveSet.count; ++index) {
        if (moveSet.moveValue[index] != moveId)
            continue;
        const std::int16_t usage = static_cast<std::int16_t>(g_playerStats.moveUsageCounts[bankIndex][index]);
        return static_cast<std::uint16_t>(moveSet.learnValue[index] + (usage > 0 ? usage : 0));
    }
    return 0;
}

bool ResolveMoveSetSlot(std::uint8_t fighterId, std::size_t &bankIndex, std::uint8_t &moveId) {
    if (fighterId < 0x20U) {
        bankIndex = 0;
        moveId = fighterId;
        return true;
    }
    if (fighterId < 0x40U) {
        bankIndex = 1;
        moveId = static_cast<std::uint8_t>(fighterId - 0x20U);
        return true;
    }
    if (fighterId < 0x50U) {
        bankIndex = 2;
        moveId = static_cast<std::uint8_t>(fighterId - 0x40U);
        return true;
    }
    return false;
}


void vfRegisterAnimSetMoveUsage(std::uint8_t fighterId) {
    std::size_t bankIndex = 0;
    std::uint8_t moveId = 0;
    if (!ResolveMoveSetSlot(fighterId, bankIndex, moveId))
        return;
    const MoveSetRuntimeState &moveSet = g_playerStats.moveSets[bankIndex];
    for (std::size_t index = 0; index < moveSet.count; ++index) {
        if (moveSet.moveValue[index] != moveId)
            continue;
        ++g_playerStats.moveUsageCounts[bankIndex][index];
        return;
    }
}


void vfUnlockFreeBattleMove(VFMAData &game, std::uint8_t fighterId) {
    std::size_t bankIndex = 0;
    std::uint8_t moveId = 0;
    if (!ResolveMoveSetSlot(fighterId, bankIndex, moveId))
        return;
    MoveSetRuntimeState &moveSet = g_playerStats.moveSets[bankIndex];
    std::size_t index = 0;
    while (index < moveSet.moveValue.size() && moveSet.moveValue[index] != moveId) {
        ++index;
    }
    if (index >= moveSet.moveValue.size() || moveSet.learnValue[index] >= 12U || (game.flags & 0x3bU) != 0) {
        return;
    }
    g_vfMoveAnimTypeLut[bankIndex][moveId] = 1;
    SYGH_SetMoveSkill(static_cast<std::uint16_t>(bankIndex + 1U), moveId, 12U);
    EnqueueEWZO(0, static_cast<std::uint16_t>(bankIndex + 1U), moveId);
    UI_SICNSetState(2U, UISicnPausePolicy::KeepOnPause);
    game.flags |= 0x10U;
    g_hazukiResidenceGatedFlag = 0;
}

// Free battle refreshes the move-type table this way when the move list opens; vfInitAnimTypeLUT clears it first.
void figpResolveAnimID(const VFMAData &game) {
    for (std::size_t bankIndex = 0; bankIndex < g_vfMoveAnimTypeLut.size(); ++bankIndex) {
        const MoveSetRuntimeState &moveSet = g_playerStats.moveSets[bankIndex];
        for (std::size_t index = 0; index < 20; ++index) {
            if (index >= moveSet.count)
                continue;
            const std::uint8_t moveId = static_cast<std::uint8_t>(moveSet.moveValue[index]);
            if (moveId == 0xffU)
                continue;
            const std::uint16_t progress = ResolveMoveAnimationProgress(bankIndex, moveId);
            std::uint8_t type = 0;
            if (progress >= 12U || game.battleType == 4U)
                type = progress >= 56U ? static_cast<std::uint8_t>(2U + (progress >= 100U)) : 1U;
            g_vfMoveAnimTypeLut[bankIndex][moveId] = type;
        }
    }
}

void vfInitAnimTypeLUT(const VFMAData &game) {
    for (std::array<std::uint8_t, 20> &types : g_vfMoveAnimTypeLut)
        types.fill(0);
    figpResolveAnimID(game);
}

const std::byte *FindDynaEntry(std::uint32_t id) {
    const std::byte *const chunk = PACK_GetCurrentDYNAChunk();
    if (!chunk)
        return nullptr;
    std::uint32_t token = 0;
    std::uint32_t chunkSize = 0;
    std::memcpy(&token, chunk, sizeof(token));
    std::memcpy(&chunkSize, chunk + 4, sizeof(chunkSize));
    if (token != kDynaToken || chunkSize < 8) {
        return nullptr;
    }
    const std::byte *const payload = chunk + 8;
    const std::uint32_t size = chunkSize - 8;
    std::uint32_t cursor = 0;
    while (cursor + 8 <= size) {
        std::uint32_t entryId = 0;
        std::uint32_t stride = 0;
        std::memcpy(&entryId, payload + cursor, sizeof(entryId));
        std::memcpy(&stride, payload + cursor + 4, sizeof(stride));
        const std::uint32_t total = stride ? stride : size - cursor;
        if (total < 8 || total > size - cursor)
            return nullptr;
        if (entryId == id)
            return payload + cursor + 8;
        if (!stride)
            break;
        cursor += stride;
    }
    return nullptr;
}

std::uint16_t ReadPackedU16(const void *value) {
    std::uint16_t result = 0;
    std::memcpy(&result, value, sizeof(result));
    return result;
}

void WritePackedU16(void *destination, std::uint16_t value) {
    std::memcpy(destination, &value, sizeof(value));
}


void UpdateCurrentAssistWindow(VFActor &actor) {
    VFActor *const target = actor.targetActor;
    VFSlotRuntimeState *const slot = actor.slot;
    VFMAData *const game = actor.game;
    if (!target || !slot || !game)
        return;

    constexpr std::uint32_t kGuardStates = 0x1c000U;
    constexpr std::uint32_t kOrdinaryReactionStates = 0x0fefbU;
    float assist = slot->assistWindowDefault;
    if ((target->hitStateFlags2 & kGuardStates) != 0) {
        assist = slot->assistWindowGuard;
    } else if ((target->groundContactFlags & 1) != 0) {
        assist = ((static_cast<std::uint8_t>(game->battleType - 2U) & 0xfdU) != 0) ? 1.2f : 1.5f;
    } else if ((target->hitStateFlags2 & ~kOrdinaryReactionStates) != 0) {
        assist = 2.5f;
    }

    if (game->battleType == 5 && actor.slotIndex != static_cast<std::uint8_t>(game->playerSlot) && game->remainingEnemies > 25U &&
        actor.slotIndex < kBattleSlots) {
        constexpr std::array<float, 8> kRankAssist{2.5f, 2.5f, 1.0f, 0.0f, 1.5f, 0.5f, 2.0f, 0.699999988f};
        const std::uint8_t rank = static_cast<std::uint8_t>(game->enemyRanks[actor.slotIndex]);
        if (rank < 4U) {
            const std::size_t rankIndex = static_cast<std::uint8_t>(game->engagedEnemies) >= rank ? 2U * rank + 1U : 2U * rank;
            assist += kRankAssist[rankIndex];
        }
    }
    slot->assistWindowCurrent = assist;
}


void UpdateSlotRuntimeMetricsAndTimers(VFActor &actor) {
    VFSlotRuntimeState *const slot = actor.slot;
    VFActor *target = actor.targetActor;
    VFMAData *const game = actor.game;
    if (!slot)
        return;
    if (game) {
        for (std::size_t index = 0; index < kBattleSlots; ++index) {
            VFActor *const other = game->actors[index];
            if (!other || !other->character)
                continue;
            target = other;
            slot->healthDifferences[index] = static_cast<std::int16_t>(actor.currentHp - other->currentHp);
            const std::int32_t angleDifference = static_cast<std::int16_t>(actor.facingAngle) - static_cast<std::int16_t>(actor.contactAngles[index]);
            slot->facingAngleDifferences[index] = static_cast<std::uint16_t>(std::abs(angleDifference));
        }
    }

    if (!target)
        return;

    std::uint32_t seed = LcgStep(g_EnvPrngSeed);
    slot->field50 = static_cast<std::uint8_t>(seed >> 16U);
    seed = LcgStep(seed);
    slot->reserved51[0] = static_cast<std::byte>(seed >> 16U);
    seed = LcgStep(seed);
    g_EnvPrngSeed = seed;
    const std::uint16_t randomWord = static_cast<std::uint16_t>((seed >> 16U) & 0x7fffU);
    slot->reserved51[1] = static_cast<std::byte>(randomWord);
    slot->reserved51[2] = static_cast<std::byte>(randomWord >> 8U);

    std::uint16_t updateFrames = ReadPackedU16(&slot->previousUpdateIndex);
    if (updateFrames != 0xffffU)
        WritePackedU16(&slot->previousUpdateIndex, static_cast<std::uint16_t>(updateFrames + 1U));
    std::uint16_t currentUpdateFrames = ReadPackedU16(&slot->byteaa);
    if (currentUpdateFrames < 60000U)
        WritePackedU16(&slot->byteaa, static_cast<std::uint16_t>(currentUpdateFrames + 1U));

    constexpr std::uint32_t kGuardStates = 0x1c000U;
    constexpr std::uint32_t kMidAirRecovery = 0x40000U;
    constexpr std::uint32_t kAirborneOrDowned = 0x80000U;
    constexpr std::uint32_t kForcedReaction = 0x10000000U;
    constexpr std::uint32_t kStageHitResponse = 0x800000U;
    std::uint16_t guardTimer = ReadPackedU16(&slot->reserved9d[1]);
    if ((actor.hitStateFlags2 & kMidAirRecovery) != 0) {
        guardTimer = static_cast<std::uint16_t>(slot->specialTimerInitial);
    } else if (guardTimer != 0) {
        --guardTimer;
    }
    WritePackedU16(&slot->reserved9d[1], guardTimer);

    if ((actor.hitStateFlags2 & kGuardStates) != 0) {
        ++slot->fielda2;
    } else {
        slot->byteb9 = 0;
        slot->fielda2 = 0;
    }
    std::uint16_t targetGuardFrames = static_cast<std::uint16_t>(slot->fielda4 & 0xffff);
    if ((target->hitStateFlags2 & kGuardStates) != 0)
        ++targetGuardFrames;
    else
        targetGuardFrames = 0;
    std::uint16_t targetLightFrames = static_cast<std::uint16_t>(static_cast<std::uint32_t>(slot->fielda4) >> 16U);
    if ((target->hitStateFlags2 & 0x1000U) != 0) {
        if (targetLightFrames < 0xffU)
            ++targetLightFrames;
    } else if (targetLightFrames != 0) {
        --targetLightFrames;
    }
    slot->fielda4 = static_cast<std::int32_t>(targetGuardFrames | (static_cast<std::uint32_t>(targetLightFrames) << 16U));

    std::uint16_t &downedFrames = slot->downedFrames;
    if ((actor.hitStateFlags2 & kAirborneOrDowned) != 0) {
        if (downedFrames < 240U)
            downedFrames = static_cast<std::uint16_t>(downedFrames + 3U);
    } else if (downedFrames != 0) {
        --downedFrames;
    }
    if (slot->framesRemaining != 0)
        --slot->framesRemaining;
    if ((target->hitStateFlags2 & kForcedReaction) != 0) {
        if (slot->fieldb2 < 240U)

            slot->fieldb2 = static_cast<std::uint16_t>(slot->fieldb2 + 8U);
    } else if (slot->fieldb2 != 0) {
        --slot->fieldb2;
    }
    if ((actor.motionFlags & kForcedReaction) != 0) {
        if (slot->currentForcedReactionFrames < 0xffU)
            ++slot->currentForcedReactionFrames;
    } else {
        slot->currentForcedReactionFrames = 0;
    }
    if ((actor.motionFlags & kStageHitResponse) != 0 && static_cast<std::uint16_t>(slot->currentStageHitResponseFrames) < 60000U) {
        ++slot->currentStageHitResponseFrames;
    }
    if (slot->hpPhaseIncrement != 0)
        --slot->hpPhaseIncrement;
}

void vfApplyYawRecursive(VFActor &actor) {
    if (!actor.character)
        return;
    CharacterNode &root = actor.character->entity.node;
    std::int16_t yaw = static_cast<std::int16_t>(actor.facingAngle);
    for (CharacterNode *node = root.child; node; node = node->child) {
        yaw = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(node->rotation.y));
    }
    root.rotation.y = yaw;
    shenApplyTranslationToAll(root, actor.position);
}

bool vfAssignVoiceFor(VFMAData &game, std::uint8_t voiceCode);


void UpdateScenarioHpPhaseConfig(VFActor &actor) {
    VFSlotRuntimeState *const slot = actor.slot;
    VFMAData *const game = actor.game;
    if (!slot || !game || !actor.styleTuning || actor.maximumHp <= 0)
        return;

    constexpr std::array<std::uint8_t, 56> kChai2{{0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0xff, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x60, 0x20, 0x20,
        0x20, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x40, 0x40, 0x60, 0xa0, 0xa0, 0x80, 0x80, 0xc0, 0x60, 0x00, 0x00, 0x00, 0x80, 0x60, 0x30, 0x30,
        0x30, 0x20, 0x20, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x00}};
    constexpr std::array<std::uint8_t, 56> kKisy2{{0x08, 0x08, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x30, 0x30, 0x00, 0xa0,
        0xa0, 0xa0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x60, 0x60,
        0x00, 0x40, 0x40, 0x40, 0xc0, 0x50, 0x40, 0x00, 0x00, 0x00, 0x30, 0xa0, 0x00}};
    constexpr std::array<std::uint8_t, 56> kKisy3{{0x60, 0x60, 0x00, 0x20, 0x20, 0x20, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x60, 0xff, 0x40, 0x40, 0x00, 0x30,
        0x30, 0x30, 0x20, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x80, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40, 0x20, 0x00, 0x30, 0x00, 0x00, 0x00, 0x20, 0x40, 0x08,
        0x08, 0x00, 0x80, 0x80, 0x80, 0x50, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00}};

    const float hpFraction = static_cast<float>(actor.currentHp) / static_cast<float>(actor.maximumHp);
    const std::uint8_t *config = static_cast<const std::uint8_t *>(actor.styleTuning) + 0x16;
    if (actor.fightStyleIndex == 0x1bU) {
        const std::uint8_t phase = hpFraction < 0.2 ? 3U : hpFraction < 0.5 ? 2U : hpFraction < 0.8 ? 1U : 0U;
        slot->currentHpRegenerationPhase = phase;
        config = kChai2.data() + 14U * phase;
    } else if (actor.fightStyleIndex == 0x1cU) {
        const std::int8_t phase = static_cast<std::int8_t>(slot->currentHpRegenerationPhase);
        config = kKisy2.data() + 14 * phase;
    } else if (actor.fightStyleIndex == 0x1dU) {
        const std::uint8_t phase = hpFraction < 0.6 ? 3U : hpFraction < 0.75 ? 2U : hpFraction < 0.9 ? 1U : 0U;
        if (phase == 3U && ((actor.groundContactFlags | game->actors[game->playerSlot]->groundContactFlags) & 4U) == 0) {
            vfAssignVoiceFor(*game, 4U);
            vfAssignVoiceFor(*game, 6U);
            vfAssignVoiceFor(*game, 5U);
        }
        slot->currentHpRegenerationPhase = phase;
        config = kKisy3.data() + 14U * phase;
    }
    std::memcpy(slot->phaseConfig, config, sizeof(slot->phaseConfig));

    if (static_cast<std::uint8_t>(slot->fieldb6) > 20U) {
        for (const std::size_t index : {0U, 1U, 9U}) {
            if (slot->phaseConfig[index] != 0)
                slot->phaseConfig[index] = 0xffU;
        }
        slot->fieldb6 = 0;
    }
    if (game->battleType == 1) {
        if (actor.fightStyleIndex == 0x1dU) {
            if (actor.contactPoints[static_cast<std::size_t>(game->playerSlot)] < 5.0f)
                slot->phaseConfig[12] = 0;
        } else if (actor.slotIndex < kBattleSlots) {
            VFFormationGroup *const wave = game->battleMeta.waves[actor.slotIndex];
            if (wave && static_cast<std::int8_t>(wave->selectFlags) < 0) {
                slot->phaseConfig[9] = 0;
                slot->phaseConfig[3] = 0;
                slot->phaseConfig[4] = 0;
                slot->phaseConfig[5] = 0;
            }
        }
    }
}

void UpdateActorStateBookkeeping(VFActor &actor) {
    VFActor *const opponent = actor.targetActor;
    if (!opponent)
        return;
    if ((actor.hitStateFlags2 & 0x10000002U) != 0 && (opponent->hitStateFlags2 & 0x100U) != 0) {
        actor.hitTimeout = 5;
    }
    if ((opponent->hitStateFlags2 & 0x2U) != 0 || (opponent->hitStateFlags2 & 0x100U) == 0) {
        if (actor.hitTimeout != 0)
            --actor.hitTimeout;
    }
    actor.previousMotionFlags = actor.motionFlags;
    actor.previousAnimationId = actor.currentReactionAnimation;
    actor.previousHitStateFlags2 = actor.hitStateFlags2;
    const auto quadrant = vfStageResolveQuadrantForPointXZ(actor.contactCurrentPosition.x, actor.contactCurrentPosition.z, actor.cachedQuadrant);
    if (quadrant != actor.cachedQuadrant) {
        actor.previousQuadrant = static_cast<std::int8_t>(actor.cachedQuadrant);
        actor.cachedQuadrant = quadrant;
    }

    actor.moveFlags &= ~0x801U;
    if (actor.currentReactionAnimation == 0) {
        actor.moveFlags |= 1U;
    } else if ((actor.moveFlags & 0x80U) != 0) {
        ++actor.actionPhaseFrameCounter;
        actor.temporaryAnimationParameter = actor.currentAnimationEndFrame;
        ++actor.currentAnimationEndFrame;
        if (actor.currentAnimationEndFrame > static_cast<std::uint16_t>(actor.reactionAnimationEndFrame))
            actor.moveFlags |= 1U;
    }
}


void vfUpdateRyoFacing(VFActor &actor) {
    if (!actor.game || actor.game->battleType != 4U || actor.fightStyleIndex != 0U || actor.currentAnimationEndFrame != 1U ||
        ((actor.hitStateFlags & 0x08000000U) == 0 && (actor.hitFlags & 0x00001010U) == 0) || !actor.targetActor) {
        return;
    }

    const std::uint16_t inverseFacing = static_cast<std::uint16_t>(-actor.facingAngle);
    const std::int16_t foldedFacing = inverseFacing <= 0x8000U ? static_cast<std::int16_t>(inverseFacing) : static_cast<std::int16_t>(0x10000U - inverseFacing);
    const float sine = sin_fixed16(static_cast<std::int16_t>(inverseFacing));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedFacing));
    actor.targetActor->position.x = actor.position.x + sine * 3.0f;
    actor.targetActor->position.z = actor.position.z - cosine * 3.0f;
}


void vfSelectCombatShadowLights(VFActor &actor) {
    Character *const character = actor.character;
    if (character)
        character->shadowMode = 0;
    if (g_cachedGlobalLightingAdjustFlag != 2) {
        if (character)
            character->lightMask = 5U;
        else
            g_defaultLightMask = 5U;
        return;
    }

    std::uint32_t nearestIndex = UINT32_MAX;
    float nearestDistanceSquared = 1000.0f;
    for (std::uint32_t index = 0; index < 32U; ++index) {
        const ShenmueLightSlot *const light = LGHT_GetRenderSlot(index);
        if (!light || light->enabled == 0)
            continue;
        const float dx = light->position.x - actor.contactCurrentPosition.x;
        const float dy = light->position.y - actor.contactCurrentPosition.y;
        const float dz = light->position.z - actor.contactCurrentPosition.z;
        const float distanceSquared = dx * dx + dy * dy + dz * dz;
        if (distanceSquared < nearestDistanceSquared) {
            nearestDistanceSquared = distanceSquared;
            nearestIndex = index;
        }
    }
    const std::uint32_t mask = nearestIndex != UINT32_MAX ? 1U << nearestIndex : 0U;
    if (character)
        character->lightMask = mask;
    else
        g_defaultLightMask = mask;
}


void vfMoveToNearestPointFor70MB(VFActor &actor) {
    if (!actor.game || g_areaId != SHEN_NAME(MFBT) || actor.game->battleType == 3U) {
        return;
    }
    constexpr std::array<Vector3, 4> kSpawnBoundaries{{
        {6.7378998f, 66.300499f, 3.9200001f},
        {16.335501f, 56.970299f, 3.0699999f},
        {44.741699f, 35.4002f, 1.49f},
        {57.3997f, 35.758499f, 2.73f},
    }};
    bool inBoundary = false;
    for (const Vector3 &boundary : kSpawnBoundaries) {
        const float dx = actor.position.x - boundary.x;
        const float dz = actor.position.z - boundary.y;
        if (std::sqrt(dx * dx + dz * dz) < boundary.z) {
            inBoundary = true;
            break;
        }
    }
    if (!inBoundary)
        return;

    const VFAnchor *nearest = nullptr;
    float nearestDistance = 999.0f;
    const std::uint8_t anchorCount = std::min<std::uint8_t>(g_fgenNumAnchors, static_cast<std::uint8_t>(kAnchorCapacity));
    const std::uint8_t enemyCount = std::min<std::uint8_t>(g_pFightStage->maxEnemies, static_cast<std::uint8_t>(kEnemyCapacity));
    for (std::uint8_t index = 0; index < anchorCount; ++index) {
        const VFAnchor &anchor = g_pFightStage->anchors[index];
        if (anchor.enabled != 1U || anchor.excludeFromSelection != 0U)
            continue;
        bool occupied = false;
        for (std::uint8_t enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex) {
            Character *const enemy = g_pFightStage->enemySlots[enemyIndex].character;
            if (!enemy)
                continue;
            const Vector3 enemyPosition = GetPositionForEntity(enemy->entity.node);
            const float enemyDx = anchor.position.x - enemyPosition.x;
            const float enemyDz = anchor.position.z - enemyPosition.z;
            if (enemyDx * enemyDx + enemyDz * enemyDz < 4.0f) {
                occupied = true;
                break;
            }
        }
        if (occupied)
            continue;
        const float dx = actor.position.x - anchor.position.x;
        const float dz = actor.position.z - anchor.position.z;
        const float distance = std::sqrt(dx * dx + dz * dz);
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearest = &anchor;
        }
    }
    actor.position.x = nearest ? nearest->position.x : 32.080002f;
    actor.position.z = nearest ? nearest->position.z : 100.12f;
}

void vfActorUpdateState(VFActor &actor) {
    UpdateActorStateBookkeeping(actor);
    vfUpdateAirborneAnimationTiming(actor);
    actor.previousAction = actor.nextAction;
    if (std::bit_cast<std::uint64_t>(actor.nextAction) != 0)
        vfPreUpdateAction(actor);
    actor.nextAction = {};
    actor.previousQueuedAction = actor.queuedAction;
    if (std::bit_cast<std::uint64_t>(actor.queuedAction) != 0)
        UpdateVFQueuedActionLifecycle(actor);
    vfDispatchQueuedReactionEvent(actor);
    vfUpdateAnimFlagsAndEvents(actor);
    actor.moveCategory = (actor.hitStateFlags2 & 0x00000100U) != 0 && actor.moveId < kVFMoveCategoryMasks.size() ?
        static_cast<std::int32_t>(kVFMoveCategoryMasks[actor.moveId]) :
        0;
    vfclUpdatePairingFacing(actor);
    UpdateVFKnockbackPhysics(actor);
    Vector3 probe = actor.contactCurrentPosition;
    if (std::fabs(probe.y) < 0.00000011920929f)
        probe.y = 0.0f;
    probe.y += 1.0f;
    const float ground = mapUpdateIntersectionStateAtLocalPoint(probe);
    actor.targetGroundY = ground;
    actor.groundY = ground;
    vfUpdateGroundHeightAndEnemyContacts(actor);
    UpdateVFAutoAdvanceLanding(actor);
    vfUpdateMotionTargetPointAndParams(actor);
    vfSelectCombatShadowLights(actor);
    if (actor.game && actor.character && actor.game->playerSlot >= 0 && static_cast<std::size_t>(actor.game->playerSlot) < kBattleSlots) {
        const std::size_t playerSlot = static_cast<std::size_t>(actor.game->playerSlot);
        const VFActor *const player = actor.game->actors[playerSlot];
        const bool withinVisibleDistance = actor.slotIndex == playerSlot || (player && actor.slotIndex == static_cast<std::uint8_t>(player->targetSlotIndex)) ||
            actor.contactPoints[playerSlot] < 5.0f;
        if (withinVisibleDistance) {
            actor.character->flags.value |= CharacterFlagWithinVisibleDistance;
        } else {
            actor.character->flags.value &= ~CharacterFlagWithinVisibleDistance;
        }
    }
    actor.forwardX = -sin_fixed16(static_cast<std::int16_t>(actor.facingYaw));
    const std::uint16_t absoluteYaw = actor.facingYaw <= 0x8000U ? actor.facingYaw : static_cast<std::uint16_t>(-actor.facingYaw);
    actor.forwardZ = -sin_fixed16(static_cast<std::int16_t>(0x4000U - absoluteYaw));
    vfUpdateRyoFacing(actor);
    vfMoveToNearestPointFor70MB(actor);
}




void vfResolvePairInteraction(VFActor &self) {
    if (((1U << (self.pairedFighterId & 31U)) & static_cast<std::uint8_t>(self.pairProcessedMask)) != 0) {
        self.action = {};
        return;
    }
    VFActor &pair = *self.pairedActor;
    const bool mutual = pair.pairedActor == &self;
    pair.pairProcessedMask = static_cast<std::int8_t>(pair.pairProcessedMask | (1U << (static_cast<std::int8_t>(self.slotIndex) & 31)));
    self.pairReferenceFrame = pair.currentAnimationEndFrame;
    if (mutual)
        pair.pairReferenceFrame = self.currentAnimationEndFrame;
    vfUpdatePairActionState(self, pair);
    if (mutual)
        vfUpdatePairActionState(pair, self);

    const std::uint8_t state = self.nextAction.state;
    const std::uint8_t pairState = pair.nextAction.state;
    VFActor *reactor = nullptr;
    VFActor *attacker = nullptr;
    if (state == 25) {
        reactor = &self;
        attacker = &pair;
    } else if (pairState == 25 && mutual) {
        reactor = &pair;
        attacker = &self;
    }
    if (reactor) {
        reactor->pendingAnimationId =
            static_cast<std::uint32_t>(static_cast<std::int16_t>(VFCore_ActionAnimation(reactor->nextAction.opcode, reactor->nextAction.index)));
        reactor->knockbackAcceleration = reactor->game->knockbackAmount;
        attacker->knockbackAcceleration = attacker->game->knockbackAmount;
        (void)ApplyPendingHitReactionMotion(*reactor);
        reactor->hitReactionAnimId = static_cast<std::int16_t>(attacker->currentReactionAnimation);
        reactor->pairFrameOffset = static_cast<std::uint16_t>(attacker->attackWindowEndFrame - reactor->field330);
        attacker->guardStrength = reactor->guardStrength;
        const std::uint32_t reactorMirror = (reactor->moveFlags >> 6U) & 1U;
        const std::uint32_t mirror = (attacker->moveFlags & kRunOrDashMode) != 0 ? reactorMirror ^ 1U : reactorMirror;
        const std::uint64_t reaction = mirror == 1U ? (reactor->hitParameter + 0x11000000U) | 0x8000U : reactor->hitParameter + 0x11000000U;
        vfSetPairedFighter(*reactor->game, attacker->slotIndex, reactor->slotIndex);
        attacker->hitSourceActor = reactor;
        if (static_cast<std::int8_t>(attacker->currentAnimationEndFrame) <= static_cast<std::int8_t>(reactor->pairFrameOffset) - 1) {
            attacker->nextAction = std::bit_cast<VFActionDescriptor>(reaction);
            self.action = {};
            pair.action = {};
            return;
        }
        attacker->deferredPairAction = std::bit_cast<VFActionDescriptor>(reaction);
        attacker->deferredPairActionTriggerFrame = static_cast<std::int16_t>(reactor->pairFrameOffset - 1U);
    }

    VFActor *thrower = nullptr;
    VFActor *thrown = nullptr;
    if (state == 27) {
        thrower = &self;
        thrown = &pair;
    } else if (pairState == 27 && mutual) {
        thrower = &pair;
        thrown = &self;
    }
    if (thrower) {
        thrower->nextAction.state = 16;
        std::memset(thrower->nextAction.parameters, 0, sizeof(thrower->nextAction.parameters));
        static_cast<MotionInformationWork *>(thrown->motion)->motionFlags &= ~8U;
        vfUpdatePairedFighterState(*thrower, *thrown);
        thrower->action = {};
        thrown->action = {};
        thrown->reactionResumeProgramCounter = nullptr;
        thrown->reactionVmProgramCounter = static_cast<VFActionDescriptor *>(self.reactionScriptBase);
        thrown->delayedReactionTimer = 0;
        thrown->delayedReactionStateActive = 0;
        return;
    }

    if (state == 16 || pairState == 16) {
        VFActor *owner = &self;
        VFActor *caught = &pair;
        if (pairState == 16 && (state != 16 || (NextVFFrameEventRandomWord() & 1U) == 0)) {
            owner = &pair;
            caught = &self;
        }
        if (owner == &self || mutual) {
            vfUpdatePairedFighterState(*owner, *caught);
            owner->action = {};
            caught->action = {};
        }
    }
    self.action = {};
}

void vfUpdateHPRegeneration(VFActor &actor) {
    VFMAData *const game = actor.game;
    if (actor.currentHp >= actor.maximumHp) {
        actor.hpRegenerationFrames = 0;
        return;
    }
    static constexpr std::array<std::uint8_t, 34> kStyleCategory{
        0, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 1, 1, 2, 3, 3, 3, 1};
    const std::size_t style = actor.fightStyleIndex;
    if (style >= kStyleCategory.size())
        return;

    std::uint8_t interval = 0;
    switch (kStyleCategory[style]) {
    case 0: {
        static constexpr std::array<std::uint8_t, 5> kIntervals{13, 12, 10, 8, 5};
        const std::uint8_t profile = g_playerStats.vfHpRegenerationProfile > 4U ? 4U : g_playerStats.vfHpRegenerationProfile;
        interval = kIntervals[profile];
        if (actor.currentHp < 100) {
            const float scale = static_cast<float>(static_cast<double>(static_cast<float>(actor.currentHp) * 0.003f) + 0.7);
            interval =
                static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<double>(scale * static_cast<float>(static_cast<std::int8_t>(interval))) + 0.5));
            if (static_cast<std::int8_t>(interval) < 5)
                interval = 5;
        }
        break;
    }
    case 1:
        if ((actor.linkFlags & 1U) != 0)
            return;
        interval = 8;
        break;
    case 2:
        if (game->voiceDelay == 1 && static_cast<std::uint8_t>(game->voiceCodeIndex - 15) <= 3U) {
            actor.currentHp = actor.maximumHp;
            return;
        }
        if ((actor.linkFlags & 2U) != 0)
            return;
        interval = 8;
        break;
    default:
        return;
    }
    if ((actor.moveFlags & 0x20U) != 0 || actor.currentHp <= 0 || actor.hpRegenerationFrames <= 30U || actor.hpRegenerationFrames % interval != 0) {
        return;
    }
    ++actor.currentHp;
}

void vfUpdatePoseWithSfx(VFActor &actor) {
    (void)SelectCharacterMapQuery(actor.character);
    vfActorUpdateState(actor);
    constexpr std::uint32_t kHpPeriodicDamage = 0x00040000U;
    constexpr std::uint32_t kPlayerAssist = 0x00010000U;
    if ((actor.triggerMask & kHpPeriodicDamage) != 0) {
        const std::uint8_t period = static_cast<std::uint8_t>(actor.hpTickPeriodFrames);
        if (period < 2U || (actor.game && actor.game->frameCount % period == 0U)) {
            vfHandleImpactSfxAndDefeat(actor, 1, 2);
        }
        return;
    }

    vfUpdateHPRegeneration(actor);
    VFMAData *const game = actor.game;
    if (game && actor.slotIndex == static_cast<std::uint8_t>(game->playerSlot) && game->battleType != 4U && (actor.triggerMask & 0x00030000U) == 0 &&
        game->hpDerivedTimer == 0) {
        actor.hpTickPeriodFrames = 5;
        actor.triggerMask = (actor.triggerMask & ~(kHpPeriodicDamage | kPlayerAssist)) | kHpPeriodicDamage;
    }
}

std::uint8_t VF_GetAnimSetValue(std::uint8_t animSetIndex) {
    std::size_t bankIndex = 0;
    std::size_t localIndex = animSetIndex;
    if (animSetIndex < 0x20U) {
        bankIndex = 0;
    } else if (animSetIndex < 0x40U) {
        bankIndex = 1;
        localIndex -= 0x20U;
    } else if (animSetIndex < 0x50U) {
        bankIndex = 2;
        localIndex -= 0x40U;
    } else {
        return 0;
    }
    const auto *const moveSet = reinterpret_cast<const std::byte *>(&g_playerStats.moveSets[bankIndex]);
    return std::to_integer<std::uint8_t>(moveSet[localIndex * sizeof(std::uint16_t)]);
}

float VF_GetAnimSetImpactScale(std::uint8_t animSetIndex) {
    if (animSetIndex >= 0x50U)
        return 1.0f;
    return static_cast<float>(static_cast<double>(VF_GetAnimSetValue(animSetIndex)) * 0.002 + 1.0);
}


float VF_GetLocalActorAnimScale(const VFActor &actor) {
    if (!actor.game || actor.slotIndex != static_cast<std::uint8_t>(actor.game->playerSlot)) {
        return 1.0f;
    }
    if ((actor.hitFlags & 0x00040000U) != 0)
        return VF_GetAnimSetImpactScale(actor.fighterIdAlt2);
    std::uint8_t animSetIndex = actor.fighterRankId;
    if (animSetIndex == 80U)
        animSetIndex = actor.fighterIdAlt;
    return VF_GetAnimSetImpactScale(animSetIndex);
}

void vfInitMoveParamsFromFightStyle(VFActor *self) {
    Character *const character = self->character;
    self->moveFlags = 0;
    self->stateFlags = 0;
    MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr;
    MotionInformationWork *const motion = manager ? &manager->motion : nullptr;
    motion->playbackRate = 1.0f;
    const float playbackRate = std::fmin(1.5f, g_globalFlags[0] == 80 ? 1.0799999f : 0.89999998f);
    VFFightStage *const stage = g_pFightStage;
    motion->playbackRate = playbackRate;
    self->scale = playbackRate;
    self->inverseScale = 1.0f / playbackRate;
    const float timeScale = playbackRate * self->game->knockbackAmount;
    self->knockbackAcceleration = 0.0f;
    self->timeScale = timeScale;
    self->knockbackStrength = timeScale;
    self->animationScale = 1.0f / timeScale;
    auto *style = static_cast<std::byte *>(stage->styles[self->fightStyleIndex]);
    if (!style)
        style = static_cast<std::byte *>(stage->styles[0]);
    motion->motionFlags |= 0x4000U;
    const auto styleOffset = [style](std::size_t offset) { return static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(ReadBigEndian32(style + offset))); };
    self->reactionScriptBase = style + styleOffset(4);
    self->cameraScriptTable = style + styleOffset(8);
    auto *const tuning = style + styleOffset(12);
    self->styleTuning = tuning;
    auto *const profile = style + styleOffset(16);
    self->contactProfile = profile;
    self->animationTable = reinterpret_cast<std::uint8_t *>(style + static_cast<std::int32_t>(ReadBigEndian32(profile)));
    self->cameraPhaseTable = style + static_cast<std::int32_t>(ReadBigEndian32(tuning));
    self->knockbackBaseParameter = ReadBigEndianFixed16(profile + 24);
    self->contactRangeFrontMin = ReadBigEndianFixed16(profile + 4);
    self->contactRangeFrontMax = ReadBigEndianFixed16(profile + 8);
    self->contactRangeSideMin = ReadBigEndianFixed16(profile + 12);
    self->contactRangeSideMax = ReadBigEndianFixed16(profile + 16);
    self->contactRangeBackMin = ReadBigEndianFixed16(profile + 20);
    self->followupKnockbackAccelerationScale = ReadBigEndianFixed16(profile + 28);
    const std::uint16_t facing = vfAccumulatedYaw(self->character);
    const float contactX = self->contactCurrentPosition.x;
    const float contactZ = self->contactCurrentPosition.z;
    const float contactY = self->contactCurrentPosition.y;
    self->facingAngle = facing;
    self->temporaryFacingAngle = facing;
    self->facingYaw = facing;
    const float positionY = self->position.y;
    self->headGroundDelta = 0.0f;
    self->previousContactPosition.x = contactX;
    self->previousContactPosition.z = contactZ;
    self->moveVelocity = {};
    self->moveDelta = {};
    self->relativeMoveDelta = {};
    self->knockbackAccelerationBias = 0.0f;
    self->knockbackSlideZ = 0.0f;
    self->knockbackSlideX = 0.0f;
    self->action = {};
    self->nextAction = {};
    self->contactCurrentPosition.y = contactY + 1.0f;
    self->previousContactPosition.y = contactY + 1.0f;
    self->deferredPairAction = {};
    self->queuedAction = {};
    self->hitStateFlags2 = 0;
    self->savedReactionStateFlags = 0;
    self->hitFlags = 0;
    self->hitStateFlags = 0;
    self->motionFlags = 0;
    self->temporaryHitFlags = 0;
    self->temporaryHitStateFlags = 0;
    self->field3ac = 0;
    self->reactionTimer = 0.0f;
    self->latchedPairContactPriority = 0;
    self->counterReactionFlags = 0;
    self->rootReactionModeFlags = 0;
    self->scheduledReactionYaw = 0;
    self->field436 = 0;
    self->field438 = 0;
    self->field43e = 0;
    self->currentReactionAnimation = 0;
    self->currentAnimationEndFrame = 0;
    self->queuedReactionEvent = 0;
    self->hitParameter = 0;
    self->hitStateHoldTimer = 0;
    self->cameraControlFlags = 0;
    self->pairContactPriority = 0;
    self->midFrameQueuedReactionActionWord = 0;
    self->pairFollowupActionWord = 0;
    self->pairActionTriggerFrame = 0;
    self->hitTimeout = 0;
    self->hitStunFrames = 0;
    self->guardImpactAccum = 0;
    self->minimumGroundY = 0.0f;
    self->predictedGroundLanding = 0.0f;
    self->targetGroundY = 0.0f;
    self->groundY = positionY;
    self->contactDecisionMask = 0;
    self->endFrameQueuedReactionActionWord = 0;
    self->fighterIdAlt = 80;
    self->fighterIdAlt2 = 80;
    self->fighterRankId = 80;
    self->fighterIdAlt3 = 0xffU;
    self->triggerState = 0;
    self->stageHitObjectIndex = -1;
    self->followupState = 0;
    self->forcedTurnState = 0;
    self->fielda80 = 0;
    self->fielda88 = 0.0f;
    const std::uint8_t quadrant = vfStageFindQuadrantForPointXZ(contactX, contactZ);
    self->previousQuadrant = static_cast<std::int8_t>(quadrant);
    self->cachedQuadrant = quadrant;
    self->facingQuadrantStep = -1;
    self->cameraActiveCounterFrames = 0;
    self->cameraIdleCounterFrames = 0;
    self->knockoutCount = 0;
    for (float &blend : self->effectorHeightBlend)
        blend = -10000.0f;
    vfUpdateHitFacingAndKnockback(*self);
    vfUpdateContactSlotsAndGroundFlags(*self);
    vfSelectDefaultReactionAndGuardAnims(*self);
    if ((static_cast<MotionInformationWork *>(self->motion)->motionFlags & 0x20000U) != 0)
        self->moveFlags |= 0x40000040U;
    else
        self->moveFlags |= 0x40000000U;
}

void vfInitActorFromBattleSlot(VFMAData *game, std::uint8_t index) {
    Character *const character = game->characters[index];
    VFActor *const actor = game->actors[index];
    actor->slotIndex = index;
    actor->character = character;
    MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr;
    actor->game = game;
    actor->motion = manager ? &manager->motion : nullptr;
    VFFormationGroup *const wave = game->battleMeta.waves[index];
    actor->fightStyleIndex = wave->fightStyle;
    actor->triggerMask = wave->actorFlags;
    actor->soundHpFloor = static_cast<std::int16_t>(wave->entryIndex);
    actor->groupHpFloor = wave->groupHpFloor;
    actor->requiredFighterRank = wave->wave;
    actor->fighterRankMin = wave->rankMin;
    actor->fighterRankMax = wave->rankMax;
    actor->hpTickPeriodFrames = wave->hpRegenRate;
    actor->cameraActiveTriggerFrames = static_cast<std::int16_t>(wave->spawnCount);
    actor->cameraIdleTriggerFrames = wave->delay;
    actor->priorityRank = static_cast<std::int16_t>((actor->priorityRank & 0xff00) | static_cast<std::uint8_t>(wave->selectFlags));
    actor->pairedSlotIndex = wave->field4f;
    if (game->playerCharacter && character == game->playerCharacter) {
        actor->currentHp = static_cast<std::int16_t>(g_playerStats.defaultHealth);
        actor->maximumHp = static_cast<std::int16_t>(g_playerStats.defaultHealth);
    } else {
        actor->currentHp = static_cast<std::int16_t>(wave->hpTotal);
        actor->maximumHp = static_cast<std::int16_t>(wave->hpCurrent);
    }
    vfInitMoveParamsFromFightStyle(actor);
    switch (wave->startType) {
    case -1:
        actor->stateFlags |= game->battleType == 2 ? 0x8U : 0x4U;
        break;
    case 1:
        actor->stateFlags |= 0x4U;
        break;
    case 2:
        actor->stateFlags |= 0x8U;
        break;
    default:
        break;
    }
    const std::uint8_t type = game->battleType;
    if (((type == 1 || type == 5) && static_cast<std::int8_t>(game->battleMeta.waves[actor->slotIndex]->selectFlags) >= 0) || type == 2 || type == 3)
        actor->stateFlags |= 0x2U;
    vfActorUpdateState(*actor);
    actor->moveFlags |= 0x10000080U;
}

void vfResetActorCombatRuntimeState(VFMAData *game, std::uint8_t index) {
    VFActor *const actor = game->actors[index];
    actor->contactVolumeRecords =
        reinterpret_cast<const std::byte *>(IMGM_GetImageName(actor->character) == SHEN_NAME(TOGM) ? kVFContactVolumesMgot.data() : kVFContactVolumes.data());
    matrix_stack::Matrix4x4_Push();
    GetTransformAndTransformVector3(*actor);
    Matrix4x4::Pop(1);
    std::memset(actor->contactProfileMasks, 0, sizeof(actor->contactProfileMasks));
    for (float &history : actor->contactShakeHistory)
        history = 0.0f;
    actor->moveCategory = 0;
    actor->contactMetadataFilter = 0;
    actor->connectionMask = 0;
    actor->terrainCollisionLatched = 0;
    actor->collisionHitFlags = 0;
    static_cast<VFCLTaskData *>(game->collisionData)->ringContactFlags[index] = 0;
}

void vfResetActorReactionStateForContact(VFMAData *game, std::uint8_t index) {
    vfInitActorFromBattleSlot(game, index);
    VFActor *const actor = game->actors[index];
    actor->moveFlags |= kVFLockMovementInput;
    actor->midFrameQueuedReactionActionWord = 0;
    actor->endFrameQueuedReactionActionWord = 0;
    actor->activeContactCount = 0;
    actor->reactionStateFlags = 0;
    actor->reactionPriorityMaskA = 0;
    actor->reactionResumeProgramCounter = nullptr;
    actor->rootReactionVmProgramCounter = nullptr;
    actor->otherPriorityFlags &= 0x00ff;
    actor->rewindReactionQueueTail = 0;
    actor->delayedReactionTimer = 0;
    actor->delayedReactionFramesRemaining = 0;
    actor->delayedReactionStateActive = 0;
    actor->reactionPriorityMaskB = 0;
    actor->reactionRingHead = 1;
    actor->reactionRingTail = 1;
    actor->reactionPriority[0].mask1 = actor->cameraControlFlags;
    actor->reactionPriority[0].mask2 = actor->cameraControlFlags;
    actor->reactionPriority[0].value = actor->cameraControlFlags;
    actor->slots.primary = static_cast<std::uint8_t>(actor->reactionRingTail);
    actor->slots.secondary = actor->reactionRingTail;
    actor->reactionSlotCurrent = static_cast<std::uint8_t>(actor->reactionRingTail);
    vfResetActorCombatRuntimeState(game, index);
    if (game->playerCharacter && game->characters[index] == game->playerCharacter)
        game->playerSlot = static_cast<std::int8_t>(index);
}

void vfUpdateGuardLevel(VFMAData *game, std::uint8_t index) {
    VFActor *const actor = game->actors[index];
    if (!actor)
        return;
    VFSlotRuntimeState *const slot = game->slotRuntime[index];
    if (!slot)
        return;
    slot->actor = actor;
    slot->slotIndex = 0xffU;
    std::memset(&slot->unknown5c, 0, 0x60);
    slot->byte55 = 127;
    slot->word56 = static_cast<std::uint16_t>((slot->word56 & 0xff00U) | 127U);
    slot->byte54 = 127;
    slot->hpPhaseIncrement = 240;
    slot->updateCamera = 1;
    const auto *const style = static_cast<const std::byte *>(actor->styleTuning);
    const float assistWindow = ReadBigEndianFixed16(style + 4);
    slot->assistWindowCurrent = assistWindow;
    slot->assistWindowDefault = assistWindow;
    slot->assistWindowGuard = ReadBigEndianFixed16(style + 8);
    slot->assistAngleLimit = static_cast<std::int16_t>(ReadBigEndian16(style + 12));
    slot->word22 = static_cast<std::int16_t>(ReadBigEndian16(style + 14));
    slot->specialTimerInitial = static_cast<std::int16_t>(ReadBigEndian16(style + 16));
    slot->phaseArgumentInitial = static_cast<std::int8_t>(style[18]);
    actor->guardLevel = static_cast<std::int8_t>(style[19]);
    if (game->battleType == 2) {
        actor->guardLevel = 1;
    } else if ((game->battleType == 1 || game->battleType == 5) && (index == static_cast<std::uint8_t>(game->playerSlot) || actor->fightStyleIndex == 0x1dU)) {
        actor->guardLevel = 50;
    }
    if ((static_cast<std::uint8_t>(style[21]) & 1U) != 0)
        actor->moveFlags |= kVFNoContactPairing;
}

} // namespace shenmue::s1::vf_detail
