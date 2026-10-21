#include "s2/player_internal.hpp"

namespace shenmue::s2::player_detail {

HeldStateMode GetHeldStateMode(const PlayerModelState &state) {
    return static_cast<HeldStateMode>(state.heldStateMode);
}

void SetHeldStateMode(PlayerModelState &state, HeldStateMode mode) {
    state.heldStateMode = static_cast<std::uint8_t>(mode);
}

bool IsHeldTransitionReady(const PlayerModelState &state) {
    return state.rootYawState == 0 && state.lookAroundState == 0;
}

void PrepareHeldTransition() {
    CloseWristwatch(1);
    SetupWZ2_CtrlWazamaki();
    if (IsItemSelectable())
        RequestMemoClose();
    ResetHelpIconsToItemPrompt();
    CloseHeldStateIcons();
}

bool StartHeldScenarioRecord(shen_name name) {
    ScenarioEventRuntime *const scenario = GetScenarioEventRuntime();
    if (!scenario || IsScenarioBusy())
        return false;
    ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(name);
    if (!record)
        return true;
    scenario->flags |= 0x841U;
    scenario->activeCharacter = name;
    record->executionState = 0;
    record->flags = (record->flags & ~(ScenarioTalkWaitAction | ScenarioTalkWaitStart | ScenarioTalkPromptClockStarted)) | ScenarioTalkSelected;
    record->control->flags &= ~6U;
    return true;
}

void BeginHeldFade() {
    HLib::Task *const film = GetCurrentAreaScreenFilmTask();
    if (film)
        WaitForNextTask_0(film, 1, 30);
    spmFadeOut();
}

bool IsHeldFadeActive() {
    HLib::Task *const film = GetCurrentAreaScreenFilmTask();
    return film && DoesScreenFilmColourExist(film);
}

void QueueHeldSceneChange(PlayerModelState &state, std::int16_t act, shen_name area, std::int16_t entry) {
    CaptureCurrentAreaJumpState();
    shenQueueChangeModuleAndScene(SystemModule::Game, act, area, entry);
    ++state.heldStateStep;
}

void plymUpdateDuringSearchRequest(PlayerModelState &state) {
    if ((g_playerRuntime.flags & 2U) == 0 || GetHeldStateMode(state) != HeldStateMode::None || IsScenarioBusy() || HasScenarioCharacterControl() ||
        scenCheckActiveRecordBusy())
        return;

    std::int32_t hour = g_currentDateTime.hour;
    if (hour < 6)
        hour += 24;
    const std::int32_t minute = g_currentDateTime.minute + 60 * hour;
    const SearchTaskData *const search = GetSearchControl();
    const bool searchBusy = srchCheckFlags(3) >= 0 || srchCheckFlags(6) != 0 || (search && search->mode != SearchMode::Idle);
    const std::uint16_t flags = state.heldStateFlags;

    if ((flags & heldScheduleEnabled) != 0 && !searchBusy && minute >= state.scheduledTransitionMinute) {
        SetHeldStateMode(state, HeldStateMode::ScheduledSceneChange);
        return;
    }
    if ((flags & heldTimedCallback) != 0 && minute >= state.heldStateTriggerMinute) {
        if ((flags & heldAlternateCallbackGate) != 0 || !searchBusy) {
            SetHeldStateMode(state, HeldStateMode::Callback);
            state.heldStateStep = 0;
            return;
        }
    }
    if (minute < state.holdWindowEndMinute) {
        if (minute >= state.holdWindowStartMinute) {
            if (minute >= state.holdWindowStartMinute + 10 || searchBusy) {
                if ((flags & heldMode1Latched) == 0)
                    state.heldStateFlags |= heldMode1Latched;
            } else if ((flags & heldMode1Latched) == 0) {
                SetHeldStateMode(state, HeldStateMode::SystemHold);
            }
        }
    } else if ((flags & heldMode2Latched) == 0) {
        SetHeldStateMode(state, HeldStateMode::WristwatchSceneChange);
    }
}

void plymUpdateHeldStateCallback(PlayerModelState &state) {
    if (state.heldStateStep == 0) {
        state.flags |= controlHeldTransitionActive;
        state.heldStateFlags |= heldCallbackActive;
        if (!IsHeldTransitionReady(state))
            return;
        state.heldStateStep = 1;
        PrepareHeldTransition();
        ++state.heldStateStep;
        state.heldStateTimer = 150;
        return;
    }
    if (state.heldStateStep == 1) {
        ++state.heldStateStep;
        state.heldStateTimer = 150;
        return;
    }
    if (state.heldStateStep == 2) {
        ++state.heldStateStep;
        PlayerFlagCallback const callback = state.heldStateCallback;
        state.heldStateCallback = nullptr;
        if (callback)
            callback();
        return;
    }

    state.flags &= ~controlHeldTransitionActive;
    state.heldStateFlags &= 0x3f7fU;
    SetHeldStateMode(state, HeldStateMode::None);
    if (g_playerRuntime.model && GetHeldStateMode(*g_playerRuntime.model) == HeldStateMode::None) {
        g_playerRuntime.model->heldStateFlags &= 0x3fffU;
        const bool hadTimedCallback = (state.heldStateFlags & heldTimedCallback) != 0;
        state.heldStateTriggerMinute = 0;
        g_playerRuntime.field08 &= ~0x30;
        if (hadTimedCallback)
            g_playerRuntime.field08 |= 0x10;
    }
}

void plymUpdateHeldState(PlayerModelState &state) {
    if ((state.heldStateFlags & heldPreserveAcrossDay) == 0) {
        if (g_currentDateTime.day != g_playerRuntime.currentDay)
            g_playerRuntime.field08 |= 4;
        g_playerRuntime.currentDay = g_currentDateTime.day;
        if ((g_playerRuntime.field08 & 4) != 0 && g_currentDateTime.minute + 60 * g_currentDateTime.hour >= 360) {
            g_playerRuntime.field08 &= ~0x0f;
            state.heldStateFlags &= static_cast<std::uint16_t>(~(heldMode1Latched | heldMode2Latched));
        }
    }
    if ((state.flags & controlHoldStateEnabled) == 0)
        return;

    if ((g_playerRuntime.field08 & 1) != 0)
        state.heldStateFlags |= heldMode1Latched;
    if ((g_playerRuntime.field08 & 2) != 0)
        state.heldStateFlags |= heldMode2Latched;

    if (GetHeldStateMode(state) == HeldStateMode::None) {
        plymUpdateDuringSearchRequest(state);
        const HeldStateMode mode = GetHeldStateMode(state);
        state.heldStateStep = 0;
        if ((mode >= HeldStateMode::SystemHold && mode <= HeldStateMode::ScheduledSceneChange) || mode == HeldStateMode::Callback) {
            if (state.heldStateMode == state.pendingHeldStateMode) {
                state.pendingHeldStateMode = 0;
            } else {
                state.pendingHeldStateMode = state.heldStateMode;
                SetHeldStateMode(state, HeldStateMode::None);
            }
        } else {
            state.pendingHeldStateMode = 0;
        }
    }

    switch (GetHeldStateMode(state)) {
    case HeldStateMode::SystemHold:
        if (state.heldStateStep == 0) {
            state.flags |= controlHeldTransitionActive;
            state.heldStateFlags |= heldMode1Latched;
            g_playerRuntime.field08 |= 1;
            if (!IsHeldTransitionReady(state))
                return;
            PrepareHeldTransition();
            state.heldStateStep = 2;
            if (!StartHeldScenarioRecord(SHEN_NAME(SY08)))
                ++state.heldStateStep;
        } else if (state.heldStateStep == 1) {
            ++state.heldStateStep;
            if (!StartHeldScenarioRecord(SHEN_NAME(SY08)))
                ++state.heldStateStep;
        } else if (state.heldStateStep == 2) {
            if (!IsScenarioBusy())
                ++state.heldStateStep;
        } else {
            state.flags &= ~controlHeldTransitionActive;
            SetHeldStateMode(state, HeldStateMode::None);
        }
        break;

    case HeldStateMode::WristwatchSceneChange:
        switch (state.heldStateStep) {
        case 0:
            state.flags |= controlHeldTransitionActive;
            state.heldStateFlags |= heldMode2Latched;
            g_playerRuntime.field08 |= 2;
            Loop3DSoundEffect(0x8a9U);
            padEnableTask();
            SetScenarioFollowCharacter(shen_name{});
            srchSetFocusObject(shen_name{});
            srchRequestLockonExit();
            ++state.heldStateStep;
            state.heldStateTimer = 25;
            [[fallthrough]];
        case 1:
            if (--state.heldStateTimer < 0)
                state.heldStateTimer = 0;
            if (IsHeldTransitionReady(state) && state.heldStateTimer <= 0 && (!GetSearchControl() || GetSearchControl()->mode == SearchMode::Idle)) {
                ++state.heldStateStep;
                SetupWZ2_CtrlWazamaki();
                if (IsItemSelectable())
                    RequestMemoClose();
                ResetHelpIconsToItemPrompt();
                CloseWristwatchTransitionIcons();
                if (IsWristwatchOpenable())
                    state.heldStateStep = 5;
            }
            return;
        case 2:
            if (SetMotionRegionFlag(state.motionWork, 72, 1, MotionRegion::LowerBody) != 0) {
                state.heldStateStep += 2;
                return;
            }
            ++state.heldStateStep;
            for (std::uint32_t region = 1; region <= 4; ++region) {
                mtwkRequestMotion(state.motionWork, 0xf0f5, static_cast<MotionRegion>(region));
            }
            for (std::uint32_t region = 1; region <= 4; ++region) {
                SetMotionRegionEvalTime(state.motionWork, 0.0f, static_cast<MotionRegion>(region));
            }
            for (std::uint32_t region = 1; region <= 4; ++region) {
                SetMotionFrameTime(state.motionWork, 40, static_cast<MotionRegion>(region));
            }
            state.heldStateTimer = 0;
            return;
        case 3:
            if (state.heldStateTimer++ >= 35)
                ++state.heldStateStep;
            return;
        case 4:
            OpenWristwatch();
            ++state.heldStateStep;
            state.heldStateTimer = 0;
            [[fallthrough]];
        case 5:
            if (state.heldStateTimer++ >= 42) {
                SetCtrlHandWatchFlags(1);
                ++state.heldStateStep;
            }
            return;
        case 6:
            ++state.heldStateStep;
            if (!StartHeldScenarioRecord(SHEN_NAME(SY07)))
                ++state.heldStateStep;
            break;
        case 7:
            if (!IsScenarioBusy())
                ++state.heldStateStep;
            break;
        case 8:
            BeginHeldFade();
            ++state.heldStateStep;
            return;
        case 9:
            if (IsHeldFadeActive())
                return;
            ExecuteAicaCommand(0x1100a0U, 5, 0);
            Loop3DSoundEffect(2218U);
            ++state.heldStateStep;
            [[fallthrough]];
        case 10:
            Loop3DSoundEffect(2218U);
            AdvanceAreaJumpTimeForDestination(state.sceneAreaId, state.sceneEntryId);
            QueueHeldSceneChange(state, state.sceneActId, state.sceneAreaId, state.sceneEntryId);
            g_playerRuntime.field08 |= 8;
            break;
        default:
            state.flags &= ~controlHeldTransitionActive;
            SetHeldStateMode(state, HeldStateMode::None);
            break;
        }
        break;

    case HeldStateMode::ScheduledSceneChange:
        switch (state.heldStateStep) {
        case 0:
            state.flags |= controlHeldTransitionActive;
            state.heldStateFlags &= static_cast<std::uint16_t>(~heldScheduleEnabled);
            if (!IsHeldTransitionReady(state))
                return;
            ++state.heldStateStep;
            PrepareHeldTransition();
            ++state.heldStateStep;
            if (!StartHeldScenarioRecord(SHEN_NAME(SY08)))
                ++state.heldStateStep;
            break;
        case 1:
            ++state.heldStateStep;
            if (!StartHeldScenarioRecord(SHEN_NAME(SY08)))
                ++state.heldStateStep;
            break;
        case 2:
            if (!IsScenarioBusy())
                ++state.heldStateStep;
            break;
        case 3:
            BeginHeldFade();
            ++state.heldStateStep;
            return;
        case 4:
            if (!IsHeldFadeActive())
                ++state.heldStateStep;
            break;
        case 5: {
            if ((state.heldStateFlags & heldApplyRequestedClock) != 0) {
                DateTime dateTime{};
                GetCurrentDateTime(dateTime);
                dateTime.hour = static_cast<std::int8_t>(state.requestedClockHour);
                dateTime.minute = static_cast<std::int8_t>(state.requestedClockMinute);
                dateTime.second = 0;
                SetCurrentDateTimeTo(dateTime);
            }
            QueueHeldSceneChange(state, state.requestedActId, state.requestedAreaId, state.requestedEntryId);
            return;
        }
        default:
            state.flags &= ~controlHeldTransitionActive;
            SetHeldStateMode(state, HeldStateMode::None);
            break;
        }
        break;

    case HeldStateMode::AdvanceTime:
        switch (state.heldStateStep) {
        case 0:
            state.flags |= controlHeldTransitionActive;
            state.heldStateFlags |= heldTimeAdvanceActive;
            if (!IsHeldTransitionReady(state))
                return;
            ++state.heldStateStep;
            PrepareHeldTransition();
            state.heldStateStep += 2;
            break;
        case 1:
            state.heldStateStep += 2;
            break;
        case 2:
            if (!IsScenarioBusy())
                ++state.heldStateStep;
            break;
        case 3: {
            std::int16_t choices[]{-1, 149, 154, -1};
            SetupControlSpeakAndUI(choices, 0x7fff, -1, 2);
            ++state.heldStateStep;
            return;
        }
        case 4:
            if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
                const std::uint16_t selected = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag());
                if (selected == 149)
                    state.heldStateStep = 5;
                else if (selected == 154 || selected == 155 || selected == 0xffff)
                    state.heldStateStep = 8;
                if (state.heldStateStep == 5)
                    SetTimeHudVariant(1);
            }
            return;
        case 5: {
            state.heldStateFlags |= heldTimeAdvanced;
            DateTime target{};
            GetCurrentDateTime(target);
            target.hour = static_cast<std::int8_t>(state.timeAdvanceTargetHour);
            target.minute = static_cast<std::int8_t>(state.timeAdvanceTargetMinute);
            target.second = 0;
            if (CompareDateTimeSeconds(g_currentDateTime, target) >= 0) {
                ++state.heldStateStep;
                SetTimeHudVariant(0);
            } else {
                std::int32_t minutes = DiffMinutes(target, g_currentDateTime) + 1;
                if (minutes > static_cast<std::int8_t>(state.timeAdvanceMinutesPerTick))
                    minutes = static_cast<std::int8_t>(state.timeAdvanceMinutesPerTick);
                DateTime_AddMinutesInPlace(g_currentDateTime, minutes);
            }
            return;
        }
        case 6:
            state.heldStateStep = 8;
            state.heldStateTimer = 45;
            return;
        case 7:
            if (IsScenarioBusy())
                return;
            [[fallthrough]];
        default:
            state.flags &= ~controlHeldTransitionActive;
            state.heldStateFlags &= static_cast<std::uint16_t>(~heldTimeAdvanceActive);
            SetHeldStateMode(state, HeldStateMode::None);
            return;
        }
        break;

    case HeldStateMode::Callback:
        plymUpdateHeldStateCallback(state);
        break;

    case HeldStateMode::RetrySceneChange:
        switch (state.heldStateStep) {
        case 0:
            state.flags |= controlHeldTransitionActive;
            state.heldStateFlags &= static_cast<std::uint16_t>(~heldRetryEnabled);
            if (!IsHeldTransitionReady(state))
                return;
            ++state.heldStateStep;
            PrepareHeldTransition();
            ++state.heldStateStep;
            state.heldStateTimer = 45;
            break;
        case 1:
            ++state.heldStateStep;
            state.heldStateTimer = 45;
            break;
        case 2:
            if (--state.heldStateTimer <= 0)
                ++state.heldStateStep;
            return;
        case 3:
            BeginHeldFade();
            ++state.heldStateStep;
            return;
        case 4:
            if (!IsHeldFadeActive())
                ++state.heldStateStep;
            break;
        case 5:
            QueueHeldSceneChange(state, state.retryActId, state.retryAreaId, state.retryEntryId);
            return;
        default:
            state.flags &= ~controlHeldTransitionActive;
            SetHeldStateMode(state, HeldStateMode::None);
            break;
        }
        break;

    case HeldStateMode::None:
    default:
        break;
    }
}

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {

// @verified - p
std::uint64_t shenToggleAreaFlags(std::int32_t mode) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return 0xffffffffULL;
    if (mode != 0) {
        g_playerRuntime.flags |= 0x00004000U;
        state->motionInputState = static_cast<std::uint8_t>(mode);
        state->padFlags = (state->padFlags & ~0x20U) | 0x40U;
    } else {
        g_playerRuntime.flags &= ~0x00004000U;
        state->padFlags |= 0x20U;
        state->motionInputState = 0;
    }
    return 0;
}

// @verified - p
std::uint64_t EV_setHoldStatesFlags(std::int32_t mode) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return 0xffffffffULL;

    const std::uint32_t previous = (g_playerRuntime.flags >> 8U) & 1U;
    if (mode == 0) {
        g_playerRuntime.flags &= ~0x00000100U;
    } else if (mode == 1) {
        g_playerRuntime.flags |= 0x00000100U;
    } else if (mode == 2) {
        g_playerRuntime.field08 &= ~3;
        state->heldStateFlags &= 0xfff9U;
    } else if (mode == 3) {
        g_playerRuntime.field08 |= 3;
    }
    return previous;
}

std::int32_t plymSetCallback(PlayerFlagCallback callback) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return -1;
    state->heldStateCallback = callback;
    return 0;
}

std::int32_t plymScheduleOneShotFlagCallback(std::int16_t minute) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state || state->heldStateMode != 0)
        return -1;
    state->heldStateFlags &= 0x3fffU;
    if (minute != 0)
        state->heldStateFlags |= heldTimedCallback;
    state->heldStateTriggerMinute = minute;
    g_playerRuntime.field08 &= ~0x30;
    if ((state->heldStateFlags & heldTimedCallback) != 0)
        g_playerRuntime.field08 |= 0x10;
    g_playerRuntime.scheduledCallbackMinute = minute;
    return 0;
}

std::int32_t plymScheduleCycleFlagCallback(std::int16_t minute) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state || state->heldStateMode != 0)
        return -1;
    state->heldStateFlags &= 0x3fffU;
    if (minute != 0)
        state->heldStateFlags |= heldTimedCallback | heldAlternateCallbackGate;
    state->heldStateTriggerMinute = minute;
    if ((state->heldStateFlags & heldTimedCallback) != 0)
        g_playerRuntime.field08 |= 0x30;
    else
        g_playerRuntime.field08 &= ~0x30;
    g_playerRuntime.scheduledCallbackMinute = minute;
    return 0;
}

// @verified - p
void plymAdvanceTime(std::uint8_t hours, std::uint8_t minutes) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state || state->heldStateMode != 0)
        return;
    state->timeAdvanceTargetHour = hours;
    state->heldStateFlags &= static_cast<std::uint16_t>(~heldTimeAdvanced);
    state->timeAdvanceTargetMinute = minutes;
    SetHeldStateMode(*state, HeldStateMode::AdvanceTime);
    state->heldStateStep = 0;
}

std::int32_t BuildTargetDateFromFrameClock(PlayerTargetDate &target) {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return -1;
    GetCurrentDateTime(target.dateTime);
    const std::int32_t hour = state->holdWindowEndMinute / 60;
    const std::int32_t minute = state->holdWindowEndMinute % 60;
    if (hour >= 6) {
        if (target.dateTime.hour < 6)
            shenGetDayInMonth(target.dateTime, target.dateTime);
    } else if (target.dateTime.hour >= 6) {
        AddDaysAndCalculateWeekday(target.dateTime, 1);
    }
    target.dateTime.minute = static_cast<std::int8_t>(minute);
    target.field08 = 0;
    target.dateTime.hour = static_cast<std::int8_t>(hour);
    target.dateTime.second = 0;
    return 0;
}

} // namespace shenmue::s2
