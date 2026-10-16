#include "s1/task/vf_internal.hpp"

namespace shenmue::s1::vf_detail {

std::uint8_t shenInsertTimeIntoTop5AndGetRank(std::uint32_t ticks) {
    g_70mbLastRankIndex = 5;
    std::int32_t rank = 0;
    std::size_t index = 0;
    while (g_70mbBestTimesTicks[index] <= ticks) {
        ++rank;
        if (++index >= 5) {
            FormatTop5TimesText();
            g_70mbLastTimeTicks = ticks;
            return static_cast<std::uint8_t>(g_70mbLastRankIndex);
        }
    }

    const std::uint32_t dropped = g_70mbBestTimesTicks[4];
    g_activeHpRegenProfile = static_cast<std::uint8_t>(dropped);
    g_shenFightState2 = static_cast<std::uint8_t>(dropped >> 8);
    for (std::size_t destination = 4; destination > index; --destination)
        g_70mbBestTimesTicks[destination] = g_70mbBestTimesTicks[destination - 1];
    g_70mbLastRankIndex = rank;
    g_70mbBestTimesTicks[rank] = ticks;
    FormatTop5TimesText();
    g_70mbLastTimeTicks = ticks;
    return static_cast<std::uint8_t>(g_70mbLastRankIndex);
}

std::uint16_t SaturatingAddU16(std::uint16_t value, std::uint32_t increment) {
    return static_cast<std::uint16_t>(std::min<std::uint32_t>(static_cast<std::uint32_t>(value) + increment, 0xffffU));
}

void vfmaAccumulatePlayerCombatStats(VFMAData &game) {
    g_vfRuntimeAccumFightTicks += game.runtimeTicks;

    if (game.battleType == 2U)
        return;

    PlayerStatsPages &stats = g_playerStats.pages;
    if (game.battleType == 5U) {
        if ((game.flags & DisableRankUpdate) != 0)
            return;

        const std::uint16_t defeated = static_cast<std::uint16_t>(game.defeatedEnemies);
        if (defeated != 0) {
            stats.totalDefeated70MB = SaturatingAddU16(stats.totalDefeated70MB, defeated);
        }
        stats.attemptCount70MB = SaturatingAddU16(stats.attemptCount70MB, 1U);

        std::uint32_t accumulatedTime{};
        std::memcpy(&accumulatedTime, stats.timeAccum70MB, sizeof(accumulatedTime));
        accumulatedTime = std::min<std::uint32_t>(accumulatedTime + g_vfRuntimeAccumFightTicks, 1080000000U);
        std::memcpy(stats.timeAccum70MB, &accumulatedTime, sizeof(accumulatedTime));

        std::array<std::uint32_t, 4> saved{};
        std::memcpy(saved.data(), stats.save70MBSamples, sizeof(saved));
        const bool ranked = shenQueryFightTaskOrRegistryValue(game.playerCharacter) == 3;
        if (saved[0] > 100U) {
            saved[0] = g_70mbBestTimesTicks[0];
        } else if (ranked) {
            saved[0] = g_70mbBestTimesTicks[0];
            stats.stateTick70MB = game.runtimeTicks;
        } else if (defeated > saved[0]) {
            saved[0] = defeated;
        }
        for (std::size_t index = 1; index < saved.size(); ++index) {
            saved[index] = g_70mbBestTimesTicks[index];
        }
        stats.bestTimeTicks = g_70mbBestTimesTicks[4];
        std::memcpy(stats.save70MBSamples, saved.data(), sizeof(saved));
        if (saved[0] > 100U && ranked) {
            stats.stateTick70MB = std::min(stats.stateTick70MB, game.runtimeTicks);
        }
        return;
    }

    if (game.battleType != 4U)
        return;

    for (std::size_t index = 0; index < 128; ++index) {
        if (index >= 81 && index <= 83)
            continue;
        const std::uint16_t contactCount = static_cast<std::uint16_t>(game.skillTable[index]);
        if (contactCount != 0) {
            stats.playerCombatContactCounts[index] = SaturatingAddU16(stats.playerCombatContactCounts[index], contactCount);
        }
        const std::uint16_t confirmedHitCount = static_cast<std::uint16_t>(game.runtimeSkills[index]);
        if (confirmedHitCount != 0) {
            stats.playerCombatConfirmedHitCounts[index] = SaturatingAddU16(stats.playerCombatConfirmedHitCounts[index], confirmedHitCount);
        }
    }
    const std::uint16_t defeatedDelta = static_cast<std::uint16_t>(game.defeatedEnemies - static_cast<std::int8_t>(game.previousDefeatedEnemies));
    if (defeatedDelta != 0)
        stats.totalDefeatsDeltaAccum = SaturatingAddU16(stats.totalDefeatsDeltaAccum, defeatedDelta);
}

void InitVFGW() {
    VFGW *const gauge = static_cast<VFGW *>(HLib_AllocHeapMemBlock(sizeof(VFGW), static_cast<std::int32_t>(SHEN_NAME(VFGW))));
    g_pVFGW_Widget = gauge;
    if (gauge)
        HLib_EnableTokenCheck(gauge);
    *gauge = {.field0 = 100.0f,
        .field4 = 100.0f,
        .field8 = 90.0f,
        .hp = 0,
        .sweepPos = 0,
        .sweepPhaseReset = 80,
        .sweepPhase = 0,
        .sweepPhaseStep = 8,
        .sweepDir = 1,
        .field18 = 129,
        .sweepPosMax = 255,
        .sweepPosMin = 137,
        .sweepActive = 0,
        .pad1f = 0,
        .field20 = 0,
        .field21 = 0,
        .field22 = 0,
        .field23 = 0,
        .pulsePos = 0,
        .pulseAmpBase = 0x50,
        .pulseStep = 30,
        .pulseAmp = 0,
        .pulseDir = 1,
        .pulsePosMax = 0xff,
        .pulsePosMin = 0x42,
        .pulseLocked = 0};
}

std::int32_t vfgwGetGaugeStatus(std::int16_t maximumHp) {
    const float divisor = static_cast<float>(maximumHp) / 12.0f;
    const std::uint16_t hp = static_cast<std::uint16_t>(g_pVFGW_Widget->hp);
    const std::uint8_t segments = static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<float>(hp) / divisor));
    const std::int32_t remainder = hp - static_cast<std::uint16_t>(static_cast<std::int32_t>(divisor)) * segments;
    const std::uint16_t third =
        static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(remainder) / (static_cast<double>(divisor / 3.0f) + 0.5)));
    if (segments >= 12U)
        return 0;
    if (third > 1U)
        return third == 2U ? 2 : 1;
    return segments != 0 || third == 1U ? 2 : 0;
}

void vfgwUpdateGauge(VFActor &actor) {
    if (g_updateRenderFrameMode == 1)
        return;
    VFMAData &game = *actor.game;
    if (((game.battleType - 2U) & 0xfdU) == 0U || (game.flags & (FreeBattleIntroActive | FreeBattleExitActive)) != 0)
        return;
    VFGW *const gauge = g_pVFGW_Widget;
    if (!gauge)
        return;

    if (!gauge->sweepActive)
        gauge->sweepPos = static_cast<std::int16_t>(gauge->sweepPos + gauge->sweepPhaseStep * gauge->sweepDir);
    if (gauge->sweepPos >= gauge->sweepPosMax) {
        gauge->sweepPos = gauge->sweepPosMax;
        if (gauge->sweepPhase < 0) {
            gauge->sweepDir = -1;
            gauge->sweepActive = 0;
        } else {
            gauge->sweepDir = 0;
            gauge->sweepActive = 1;
        }
        if (gauge->sweepActive == 1)
            gauge->sweepPhase = static_cast<std::int16_t>(gauge->sweepPhase - gauge->sweepPhaseStep);
    } else if (gauge->sweepPos <= gauge->sweepPosMin) {
        gauge->sweepDir = 1;
        gauge->sweepPos = gauge->sweepPosMin;
        gauge->sweepActive = 0;
        gauge->sweepPhase = gauge->sweepPhaseReset;
    }

    if (!gauge->pulseLocked)
        gauge->pulsePos = static_cast<std::int16_t>(gauge->pulsePos + gauge->pulseStep * gauge->pulseDir);
    if (gauge->pulsePos >= gauge->pulsePosMax) {
        gauge->pulsePos = gauge->pulsePosMax;
        if (gauge->pulseAmp < 0) {
            gauge->pulseDir = -1;
            gauge->pulseLocked = 0;
        } else {
            gauge->pulseDir = 0;
            gauge->pulseLocked = 1;
        }
        if (gauge->pulseLocked == 1)
            gauge->pulseAmp = static_cast<std::int16_t>(gauge->pulseAmp - gauge->pulseStep);
    } else if (gauge->pulsePos <= gauge->pulsePosMin) {
        gauge->pulsePos = gauge->pulsePosMin;
        gauge->pulseAmp = gauge->pulseAmpBase;
        gauge->pulseDir = 1;
        gauge->pulseLocked = 0;
    }

    std::int16_t hpDelta = static_cast<std::int16_t>(actor.currentHp - gauge->hp);
    if (std::fabs(static_cast<float>(hpDelta)) > 8.0f)
        hpDelta = hpDelta < 0 ? -8 : 8;
    gauge->hp = static_cast<std::int16_t>(gauge->hp + hpDelta);
    TryInvokeRegisteredCallback(kUiGameHudGauge);
    const float healthPercentage =
        actor.maximumHp > 0 ? static_cast<float>(static_cast<std::uint16_t>(g_pVFGW_Widget->hp)) / static_cast<float>(actor.maximumHp) : 0.0f;
    UI_VFUpdateHealthPercentage(healthPercentage, vfgwGetGaugeStatus(actor.maximumHp), static_cast<float>(gauge->pulsePos));
    g_uiElements[kUiGameHudGauge].isActive = 1;
}


VFReact g_vfReactE1031[23]{
    {.frameDelay = 0, .subtitle = "E1031A012", .nextVoiceCode = -1},
    {.frameDelay = 60, .subtitle = "E1031A013", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1031A014", .nextVoiceCode = -1},
    {.frameDelay = 45, .subtitle = "E1031A015", .nextVoiceCode = -1},
    {.frameDelay = 45, .subtitle = "E1031A016", .nextVoiceCode = -1},
    {.frameDelay = 45, .subtitle = "E1031A017", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031A018", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B016", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B017", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B018", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B019", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B020", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B021", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1031B022", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B023", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B024", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B025", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1031B026", .nextVoiceCode = -1},
    {.frameDelay = 52, .subtitle = "E1031B027", .nextVoiceCode = 1},
    {.frameDelay = 30, .subtitle = "E1031B028", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1031B029", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1031B030", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = nullptr, .nextVoiceCode = -1},
};

VFReact g_vfReactE1037[24]{
    {.frameDelay = 30, .subtitle = "E1037A001", .nextVoiceCode = -1},
    {.frameDelay = 60, .subtitle = "E1037A002", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1037A003", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1037A004", .nextVoiceCode = 17},
    {.frameDelay = 20, .subtitle = "E1037A005", .nextVoiceCode = 15},
    {.frameDelay = 20, .subtitle = "E1037A006", .nextVoiceCode = 15},
    {.frameDelay = 20, .subtitle = "E1037A007", .nextVoiceCode = 16},
    {.frameDelay = 0, .subtitle = "E1037A008", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1037A009", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1037A010", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1037B001", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1037B002", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1037B003", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1037B004", .nextVoiceCode = -1},
    {.frameDelay = 30, .subtitle = "E1037B005", .nextVoiceCode = -1},
    {.frameDelay = 50, .subtitle = "E1037B006", .nextVoiceCode = -1},
    {.frameDelay = 50, .subtitle = "E1037B007", .nextVoiceCode = -1},
    {.frameDelay = 65, .subtitle = "E1037B008", .nextVoiceCode = -1},
    {.frameDelay = 50, .subtitle = "E1037B009", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1037B010", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1037B011", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1037B012", .nextVoiceCode = -1},
    {.frameDelay = 40, .subtitle = "E1037A007", .nextVoiceCode = 18},
    {.frameDelay = 0, .subtitle = nullptr, .nextVoiceCode = -1},
};

VFReact g_vfReactE1047[20]{
    {.frameDelay = 70, .subtitle = "E1047A007", .nextVoiceCode = 1},
    {.frameDelay = 75, .subtitle = "E1047B001", .nextVoiceCode = 2},
    {.frameDelay = 50, .subtitle = "E1047A008", .nextVoiceCode = 3},
    {.frameDelay = 95, .subtitle = "E1047B002", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1047B003", .nextVoiceCode = 5},
    {.frameDelay = 70, .subtitle = "E1047A003", .nextVoiceCode = 6},
    {.frameDelay = 45, .subtitle = "E1047B004", .nextVoiceCode = -1},
    {.frameDelay = 40, .subtitle = "E1047A004", .nextVoiceCode = 8},
    {.frameDelay = 90, .subtitle = "E1047B005", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = "E1047A005", .nextVoiceCode = 10},
    {.frameDelay = 45, .subtitle = "E1047B006", .nextVoiceCode = 11},
    {.frameDelay = 45, .subtitle = "E1047A006", .nextVoiceCode = 12},
    {.frameDelay = 45, .subtitle = "E1047B007", .nextVoiceCode = -1},
    {.frameDelay = 60, .subtitle = "E1047A007", .nextVoiceCode = 14},
    {.frameDelay = 70, .subtitle = "E1047B001", .nextVoiceCode = 15},
    {.frameDelay = 55, .subtitle = "E1047A008", .nextVoiceCode = 16},
    {.frameDelay = 65, .subtitle = "E1047B002", .nextVoiceCode = -1},
    {.frameDelay = 65, .subtitle = "E1047A004", .nextVoiceCode = 18},
    {.frameDelay = 105, .subtitle = "E1047B005", .nextVoiceCode = -1},
    {.frameDelay = 0, .subtitle = nullptr, .nextVoiceCode = -1},
};

VFReact *const g_vfReactTbl[3]{g_vfReactE1031, g_vfReactE1037, g_vfReactE1047};

void vfInitReactTable(VFMAData &game) {
    game.voiceCodeIndex = -1;
    game.voiceDelay = 0;
    std::uint8_t reactionType = 0xffU;
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (!game.characters[index])
            continue;
        const std::uint8_t style = game.battleMeta.waves[index]->fightStyle;
        if (style == 12U || style == 57U) {
            reactionType = 0;
            break;
        }
        if (style == 29U) {
            reactionType = 1;
            break;
        }
        if (style == 28U) {
            reactionType = 2;
            break;
        }
    }
    game.reactionTypeIndex = reactionType;
    if (reactionType == 0xffU)
        return;
    for (VFReact *react = g_vfReactTbl[reactionType]; react->subtitle; ++react)
        react->hasUsed = 0;
}

void vfQueueVoiceReaction(VFMAData &game) {
    if ((game.flags & EnemyAggro) != 0) {
        game.voiceDelay = 0;
        return;
    }
    if (game.voiceCodeIndex == -1)
        return;
    if (game.voiceDelay != 0) {
        --game.voiceDelay;
        return;
    }
    VFReact *const table = g_vfReactTbl[static_cast<std::int8_t>(game.reactionTypeIndex)];
    VFReact &react = table[game.voiceCodeIndex];
    (void)ProcessSubtitleFile(react.subtitle, 0, 0, nullptr);
    const std::int8_t next = react.nextVoiceCode;
    react.hasUsed = 1;
    game.voiceCodeIndex = next;
    game.voiceDelay = next == -1 ? 0 : table[next].frameDelay;
}

void HandleRetry70ManBattlePrompt(UIRetry70ManBattlePromptResult result) {
    if (static_cast<std::int32_t>(result) != 2)
        return;

    UI_CloseRetry70ManBattlePrompt();
    if (VFMAData *const game = vfMainGetCurrent())
        game->state = VFS_Teardown;
}

bool vfAssignVoiceFor(VFMAData &game, std::uint8_t voiceCode) {
    for (std::size_t index = 0; index < 2U; ++index) {
        if (g_pCVWK->slots[index].callback)
            return false;
    }
    if (game.voiceCodeIndex != -1 || (game.flags & EnemyAggro) != 0)
        return false;
    const VFReact &react = g_vfReactTbl[static_cast<std::int8_t>(game.reactionTypeIndex)][voiceCode];
    if (react.hasUsed == 1)
        return false;
    game.voiceCodeIndex = static_cast<std::int8_t>(voiceCode);
    game.voiceDelay = react.frameDelay;
    return true;
}

bool vfmaAssignVoiceReactForFrame(VFMAData &game, std::uint8_t voiceCode) {
    if (game.frameCount < 450U || game.timer6 != 0)
        return false;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);

    const float sample = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125);
    return sample <= 0.5f && vfAssignVoiceFor(game, voiceCode);
}

// Free battle saves the SICN state (low bits), 0x200 and the pause policy (bit 16) in playerFlags and raises SICN 0x401;
// vfUpdateUI restores or clears it at teardown.
void vfMonitorDrawMode(VFMAData *game) {
    std::uint32_t flags = 0;
    if (useAnalogInput == 1) {
        useAnalogInput = 0;
        flags = 0x400U;
    }
    if ((Shenmue_nPauseState & 0x20) == 0)
        flags |= 0x800U;
    Shenmue_nPauseState &= ~0x20;
    if (game->battleType == 4) {
        const UIPresentationSnapshot presentation = UI_GetPresentationSnapshot();
        if (UI_SICNIsTaskActive()) {
            flags |= (static_cast<std::uint32_t>(static_cast<std::uint8_t>(presentation.sicnPausePolicy)) << 16U) | 0x200U | presentation.sicnState;
            UI_SICNSetState(0x401, UISicnPausePolicy::KeepOnPause);
        } else {
            UI_SICNEnqueue(0x401, UISicnPausePolicy::KeepOnPause);
        }
    } else {
        if (g_characterDrawMode)
            flags |= 0x1000U;
        g_characterDrawMode = 1;
        if (g_drawMode)
            flags |= 0x2000U;
        g_drawMode = 1;
    }
    if (shenGetCanUpdate()) {
        g_disableClockAndActionSelector = 1;
        flags |= 0x4000U;
    }
    game->playerFlags = flags;
}

void vfUpdateUI(VFMAData *game) {
    const std::uint32_t flags = game->playerFlags;
    if ((flags & 0x400U) != 0)
        useAnalogInput = 1;
    if (game->battleType == 4) {
        if ((flags & 0x200U) != 0)
            UI_SICNSetState(static_cast<std::uint8_t>(flags), static_cast<UISicnPausePolicy>((flags >> 16U) & 1U));
        else
            UI_SICNDispatch(1, 0, UISicnPausePolicy::ClearOnPause);
    }
    g_characterDrawMode = static_cast<std::int32_t>((flags >> 12U) & 1U);
    g_drawMode = static_cast<std::int32_t>((flags >> 13U) & 1U);
    if ((flags & 0x4000U) != 0)
        g_disableClockAndActionSelector = 0;
}

} // namespace shenmue::s1::vf_detail
