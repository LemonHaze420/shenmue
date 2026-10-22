#include "s1/task/scen_state.hpp"

namespace shenmue::s1::scen_detail {

std::int32_t PopulateTimedHelpSelectorChoice(
    std::uint32_t mode, std::int16_t bucket, TimedHelpSelectorChoice &choice, std::uint8_t index, std::int8_t hours, std::int8_t minutes) {
    if (hours >= 23) {
        choice.enabled = 0;
        choice.mapId = 0;
        choice.helpToken = -1;
        choice.entryId = 0;
        return 0;
    }
    const TimedHelpScheduleEntry *const entry = mode == 0 ? kTimedHelpSchedule[static_cast<std::size_t>(index + 4 * bucket)] : nullptr;
    if (!entry) {
        choice.enabled = 0;
        choice.mapId = 0;
        choice.helpToken = -1;
        return 0;
    }
    choice.enabled = 1;
    choice.helpToken = entry->destination->helpToken;
    std::memcpy(&choice.mapId, entry->destination->mapId, sizeof(choice.mapId));
    choice.entryId = entry->destination->entryId;
    std::memcpy(choice.schedule, entry->schedule, sizeof(choice.schedule));
    choice.scheduleWord = entry->scheduleWord;
    const auto travelMinutes = static_cast<std::int8_t>(choice.schedule[5]);
    if (travelMinutes > 0 && minutes + travelMinutes >= 60) {
        choice.schedule[4] = 1;
        choice.schedule[5] = static_cast<std::uint8_t>(travelMinutes - 60);
    }
    return 1;
}

// Pad slots: 0 up (+0x00), 1 down (+0x18), 2 left (+0x30), 3 right (+0x48); table columns fill up, left, right, down.
std::int32_t BuildTimedHelpSelectorChoices(std::uint32_t mode, TimedHelpSelectorChoice *choices) {
    if ((S1SettingsFlags_Get() & 8U) == 0)
        return 0;
    const std::int16_t step = g_currentScenarioStep;
    std::int16_t bucket = 0;
    if (mode == 0) {
        if (step == 12)
            bucket = 1;
        else if (step == 13)
            bucket = 2;
        else if (step != 152 && static_cast<std::uint32_t>(step - 62) > 18U && step > 12 && static_cast<std::uint32_t>(step - 163) > 8U)
            bucket = 3;
    }
    const auto hours = static_cast<std::int8_t>(g_currentDateTime.hours);
    const auto minutes = static_cast<std::int8_t>(g_currentDateTime.minutes);
    PopulateTimedHelpSelectorChoice(mode, bucket, choices[0], 0, hours, minutes);
    PopulateTimedHelpSelectorChoice(mode, bucket, choices[2], 1, hours, minutes);
    PopulateTimedHelpSelectorChoice(mode, bucket, choices[3], 2, hours, minutes);
    PopulateTimedHelpSelectorChoice(mode, bucket, choices[1], 3, hours, minutes);
    const std::uint16_t band = step > 10 ? (step <= 19 ? 2 : 3) : 1;
    for (std::size_t index = 0; index < 4; ++index)
        choices[index].scenarioBand = band;
    if (!choices[0].enabled && !choices[1].enabled && !choices[2].enabled && !choices[3].enabled)
        return 0;
    if (g_timeValue)
        g_shouldTimeAdvance = 0;
    return 1;
}

bool TimedTransitionConditionsMatch(const TimedTransitionEntry *entry) {
    if (!entry)
        return false;
    for (const std::uint16_t condition : entry->conditionWords) {
        if ((condition & 0x3fffU) == 1025U)
            return true;
        const std::uint32_t flagIndex = condition & 0x3fffU;
        const std::uint32_t expected = condition >> 14U;
        const std::uint32_t actual = flagIndex < 1024U ? ((g_shenmueFlags[flagIndex >> 3U] >> (flagIndex & 7U)) & 1U) : 0U;
        if (actual != expected)
            return false;
    }
    return false;
}

bool TimedTransitionInCurrentWindow(const TimedTransitionEntry &entry) {
    if (entry.startHHMM == -1)
        return entry.endHHMM == -1;
    std::int32_t current = 100 * static_cast<std::int8_t>(g_currentDateTime.hours) + static_cast<std::int8_t>(g_currentDateTime.minutes);
    std::int32_t end = entry.endHHMM;
    if (end <= entry.startHHMM) {
        const std::int32_t originalEnd = end;
        end += 2400;
        if (current < originalEnd)
            current += 2400;
    }
    return entry.startHHMM <= current && current <= end;
}

bool TimedTransitionScenarioBusy() {
    if (g_pCVWK) {
        for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
            if (slot.callback && slot.stateOrOwner >= 0)
                return true;
        }
    }
    return g_scenarioFunctionWork && ((g_scenarioFunctionWork->flags >> 17U) & 1U) != 0;
}

void CancelActiveTimedTransition() {
    if (g_activeTimedTransition)
        g_activeTimedTransition->state = TimedTransitionState::Queued;
    g_activeTimedTransition = nullptr;
}

void TrimAreaScopedTimedTransitions(bool retainCurrentArea) {
    std::uint8_t removed = 0;
    for (std::uint8_t index = 0; index < g_scenarioTimedTransitionCount; ++index) {
        const TimedTransitionEntry &entry = g_scenarioTimedTransitions[index];
        if ((entry.flags & TimedTransitionAreaScopedPrunable) == 0)
            continue;
        if (!retainCurrentArea || entry.boundAreaId.name != g_areaId)
            ++removed;
    }
    g_scenarioTimedTransitionCount = removed < g_scenarioTimedTransitionCount ? static_cast<std::uint8_t>(g_scenarioTimedTransitionCount - removed) : 0;
}

void RollTimedTransitionDateBackTo2330() {
    GameDateTime date = g_currentDateTime;
    if (date.day > 1) {
        --date.day;
    } else {
        if (date.month > 1) {
            --date.month;
        } else {
            date.month = 12;
            date.year = date.year > 0 ? static_cast<std::uint8_t>(date.year - 1U) : 99U;
        }
        date.day = date.month == 2 && (date.year & 3U) == 0 ? 29U : kTimedTransitionMonthDays[date.month];
    }
    date.hours = 23;
    date.minutes = 30;
    date.seconds = 0;
    SetNewDateTime(date);
}

std::uint8_t TimeUpdate(std::uint8_t dueCount, std::uint32_t signature) {
    std::uint8_t result = 0xff;
    if (!dueCount)
        return 0xff;
    std::int16_t nearest = 16384;
    const std::int16_t current =
        static_cast<std::int16_t>(100 * static_cast<std::int8_t>(g_currentDateTime.hours) + static_cast<std::int8_t>(g_currentDateTime.minutes));
    for (std::uint8_t remaining = static_cast<std::int8_t>(dueCount) > 0 ? dueCount : 0; remaining; --remaining) {
        const std::int8_t index = static_cast<std::int8_t>((signature & 0xfU) - 1U);
        std::int16_t start = g_scenarioTimedTransitions[index].startHHMM;
        std::int16_t end = g_scenarioTimedTransitions[index].endHHMM;
        if (start < current && end <= current) {
            end += 2400;
            start += 2400;
        }
        if ((start > current || end > current) && nearest > start) {
            nearest = start;
            result = static_cast<std::uint8_t>(index);
        }
        signature >>= 4U;
    }
    if (g_timedTransitionUpcoming != result) {
        GameDateTime &upcoming = g_timedTransitionUpcomingTime;
        upcoming.dayOfWeek = g_currentDateTime.dayOfWeek;
        upcoming.seconds = g_currentDateTime.seconds;
        upcoming.year = g_currentDateTime.year;
        upcoming.month = g_currentDateTime.month;
        upcoming.day = g_currentDateTime.day;
        upcoming.hours = g_currentDateTime.hours;
        upcoming.minutes = g_currentDateTime.minutes;
        if (nearest >= 2400) {
            const std::int32_t monthDays = (g_currentDateTime.year & 3U) != 0 || g_currentDateTime.month != 2 ?
                static_cast<std::int8_t>(kTimedTransitionMonthDays[static_cast<std::int8_t>(g_currentDateTime.month)]) :
                29;
            upcoming.day = static_cast<std::uint8_t>(g_currentDateTime.day + 1U);
            if (static_cast<std::int8_t>(upcoming.day) > monthDays) {
                upcoming.day = 1;
                upcoming.month = static_cast<std::uint8_t>(g_currentDateTime.month + 1U);
                if (static_cast<std::int8_t>(upcoming.month) > 12) {
                    upcoming.month = 1;
                    const std::uint8_t year = static_cast<std::uint8_t>(g_currentDateTime.year + 1U);
                    upcoming.year = static_cast<std::int8_t>(year) > 99 ? 0 : year;
                }
            }
            RecalculateDayInTheWeek(&upcoming);
            nearest -= 2400;
        }
        upcoming.seconds = 0;
        upcoming.hours = static_cast<std::uint8_t>(nearest / 100);
        upcoming.minutes = static_cast<std::uint8_t>(nearest % 100);
    }
    return result;
}

bool TimedTransitionCallbacksPending() {
    for (const CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        if (slot.callback)
            return true;
    }
    return false;
}

void ClearTimedTransitionPrompt() {
    DestroyTMEX();
    shenRefreshUI();
    UI_ResetConversationDetailsState();
    shenExecuteAICACommand(0x30babU, 0, 0);
    SetStatOrNotebookFlag(SHENMUE_FLAG_EV_STOP_ACTIVE, false);
    SetStatOrNotebookFlag(SHENMUE_FLAG_SLEEP_TRANSITION_MODAL_ACTIVE, false);
    g_TMEX_Flag = 5;
    g_tmex_val = 0;
    g_TMEX_CurrentMode = 0;
    g_tmexValHigh = 0;
}

void UpdateTimedTransitionSchedulerInternal() {
    MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    TimedTransitionEntry *const entry = g_activeTimedTransition;
    if (!g_scenarioTimedTransitionEnabled) {
        if (entry && (!TimedTransitionConditionsMatch(entry) || (control->flags & 1U) != 0))
            CancelActiveTimedTransition();
        return;
    }

    if (!entry) {
        std::uint8_t dueCount = 0;
        std::uint32_t dueSignature = 0;
        const std::int32_t tmexMode = static_cast<std::int32_t>(g_TMEX_CurrentMode);
        for (std::uint8_t index = 0; index < g_scenarioTimedTransitionCount; ++index) {
            TimedTransitionEntry &queued = g_scenarioTimedTransitions[index];
            if (queued.state != TimedTransitionState::Queued || !TimedTransitionConditionsMatch(&queued))
                continue;
            ++dueCount;
            dueSignature = index + 16U * dueSignature + 1U;
            if (!TimedTransitionInCurrentWindow(queued))
                continue;
            if (tmexMode <= 0 && (g_shouldSleep & 0x12U) != 0x10U && (g_shenmueFlags[0x0c2U >> 3U] & (1U << (0x0c2U & 7U))) == 0) {
                queued.state = TimedTransitionState::WaitForPromptStart;
                g_activeTimedTransition = &queued;
                break;
            }
        }
        g_timedTransitionUpcoming = TimeUpdate(dueCount, dueSignature);
    } else if ((g_shouldSleep & 0x12U) == 0x10U) {
        CancelActiveTimedTransition();
        return;
    } else {
        switch (entry->state) {
        case TimedTransitionState::WaitForPromptStart:
            if ((g_shenmueFlags[24] & 4U) != 0 || !TimedTransitionConditionsMatch(entry) || (control->flags & 1U) != 0) {
                CancelActiveTimedTransition();
                return;
            }
            if (!TimedTransitionScenarioBusy()) {
                const TMEXUtilityMode mode = (entry->flags & TimedTransitionCaptureCurrentArea) != 0 ? TMEXUtilityMode::OpenTimed : TMEXUtilityMode::OpenSleep;
                if (TMEXUtilities(static_cast<std::uint32_t>(mode)) != 0)
                    g_activeTimedTransition->state = TimedTransitionState::WaitForPromptResult;
            }
            break;
        case TimedTransitionState::WaitForPromptResult: {
            const std::int8_t result = static_cast<std::int8_t>(g_tmex_val);
            if (result == 0) {
                entry->state = TimedTransitionState::StartMessage;
            } else if (result == 1) {
                entry->state = TimedTransitionState::WaitForPromptStart;
            } else if (static_cast<std::uint32_t>(result - 2) <= 2U && (!TimedTransitionConditionsMatch(entry) || (control->flags & 1U) != 0)) {
                ClearTimedTransitionPrompt();
                g_activeTimedTransition->state = TimedTransitionState::Queued;
                g_activeTimedTransition = nullptr;
            }
            break;
        }
        case TimedTransitionState::StartMessage:
        case TimedTransitionState::StartMessageAlt:
            if (!TimedTransitionScenarioBusy()) {
                entry->state = TimedTransitionState::WaitForMessageEnd;
                if ((entry->flags & TimedTransitionCaptureCurrentArea) != 0) {
                    constexpr std::array<const char *, 3> messages{{"SA1091A003", "SA1091A006", "SA1091A013"}};
                    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
                    ProcessSubtitleFile(messages[((g_EnvPrngSeed >> 16U) & 0x7fffU) % messages.size()], 0, 0, nullptr);
                } else if ((entry->flags & TimedTransitionRollbackToPreviousDay2330) != 0) {
                    LoadAudioFile_Wrapper(1, "SA1101");
                    ProcessSubtitleFile("SA1101A008", 0, 0, nullptr);
                } else {
                    ProcessSubtitleFile("SY07A003", 0, 0, nullptr);
                }
            }
            break;
        case TimedTransitionState::WaitForMessageEnd:
            if (!TimedTransitionCallbacksPending()) {
                g_activeTimedTransition->state = TimedTransitionState::StartFadeOut;
                ctrlSetCurrentFilmFade();
            }
            break;
        case TimedTransitionState::StartFadeOut:
            if (!g_NumFramesProcessed) {
                const auto *const film =
                    reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(control->filmTask->task_data_ptr) + 4);
                if (film->remainingFrames == 0)
                    g_activeTimedTransition->state = TimedTransitionState::CommitMapTransition;
            }
            break;
        case TimedTransitionState::CommitMapTransition:
            if ((g_activeTimedTransition->flags & TimedTransitionRollbackToPreviousDay2330) != 0)
                RollTimedTransitionDateBackTo2330();
            if ((entry->flags & TimedTransitionUpdateMemoItem150) != 0 && (g_shenmueFlags[18] & 0x40U) == 0)
                SetStatOrNotebookFlag(SHENMUE_FLAG_JOMO_A01124_INE_EVENT_DONE, true);
            DetermineActNum();
            ClearTimedTransitionPrompt();
            control->flags |= 1U;
            control->module = SystemModule::Map;
            control->sceneId = g_activeTimedTransition->sceneId;
            control->areaId = g_activeTimedTransition->areaId;
            control->entryPoint = static_cast<std::uint32_t>(g_activeTimedTransition->entryId);
            g_shouldOverrideClearColour = 1;
            if (g_shouldUseTemporaryClearColour) {
                g_overrideClearColour = (static_cast<std::uint32_t>(g_temporaryClearColour.red) << 24U) |
                    (static_cast<std::uint32_t>(g_temporaryClearColour.green) << 16U) | (static_cast<std::uint32_t>(g_temporaryClearColour.blue) << 8U) |
                    static_cast<std::uint32_t>(g_temporaryClearColour.alpha);
            } else {
                g_overrideClearColour = 0xff000000U;
            }
            g_activeTimedTransition->flags |= TimedTransitionAreaScopedPrunable;
            g_activeTimedTransition = nullptr;
            break;
        default:
            break;
        }
    }

    if ((control->flags & 1U) != 0)
        TrimAreaScopedTimedTransitions(false);
}

std::uint32_t RegisterTimedTransition(const TimedTransitionParameters &parameters) {
    if (g_scenarioTimedTransitionCount >= g_scenarioTimedTransitions.size()) {
        return 0xffffffffU;
    }
    TrimAreaScopedTimedTransitions(true);
    const std::uint8_t index = g_scenarioTimedTransitionCount;
    TimedTransitionEntry &entry = g_scenarioTimedTransitions[index];
    entry.state = TimedTransitionState::Queued;
    entry.flags = static_cast<std::uint8_t>(parameters.flags);
    entry.sceneId = static_cast<std::int8_t>(parameters.sceneId);
    entry.entryId = static_cast<std::int8_t>(parameters.entryId);
    entry.areaId = parameters.areaId;
    entry.boundAreaId.name = (entry.flags & TimedTransitionCaptureCurrentArea) != 0 ? g_areaId : 0U;

    std::size_t conditionIndex = 0;
    const std::byte *conditionData = parameters.conditionOffset != -1 && parameters.conditionOffset != 0 ?
        ScenarioPointer<const std::byte>(static_cast<std::uint32_t>(parameters.conditionOffset)) :
        nullptr;
    while (conditionData) {
        std::int32_t terminator = 0;
        std::memcpy(&terminator, conditionData, sizeof(terminator));
        if (terminator == -1)
            break;
        if (conditionIndex < 7) {
            std::uint16_t flag = 0;
            std::uint16_t expected = 0;
            std::memcpy(&flag, conditionData, sizeof(flag));
            std::memcpy(&expected, conditionData + 4, sizeof(expected));
            entry.conditionWords[conditionIndex++] = static_cast<std::uint16_t>((flag & 0x3fffU) | (expected << 14U));
        }
        conditionData += 8;
    }
    entry.conditionWords[conditionIndex] = 1025U;
    if (parameters.timeRange == -1) {
        entry.startHHMM = -1;
        entry.endHHMM = -1;
    } else {
        entry.startHHMM = static_cast<std::int16_t>(parameters.timeRange / 10000);
        entry.endHHMM = static_cast<std::int16_t>(parameters.timeRange % 10000);
    }
    for (std::uint8_t existingIndex = 0; existingIndex < index; ++existingIndex) {
        const TimedTransitionEntry &existing = g_scenarioTimedTransitions[existingIndex];
        if (entry.sceneId != existing.sceneId || entry.areaId != existing.areaId || entry.entryId != existing.entryId)
            continue;
        std::size_t entryConditionCount = 0;
        while ((entry.conditionWords[entryConditionCount] & 0x3fffU) != 1025U)
            ++entryConditionCount;
        std::size_t existingConditionCount = 0;
        while ((existing.conditionWords[existingConditionCount] & 0x3fffU) != 1025U)
            ++existingConditionCount;
        if (entryConditionCount == existingConditionCount &&
            std::memcmp(entry.conditionWords, existing.conditionWords, (entryConditionCount + 1U) * sizeof(entry.conditionWords[0])) == 0 &&
            entry.startHHMM == existing.startHHMM && entry.endHHMM == existing.endHHMM) {
            return 0xffffffffU;
        }
    }
    ++g_scenarioTimedTransitionCount;
    return index;
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

void SCEN_ResetTimedTransitions() {
    g_scenarioTimedTransitionEnabled = true;
    g_activeTimedTransition = nullptr;
    g_scenarioTimedTransitionCount = 0;
    g_timedTransitionUpcoming = 0xff;
    g_timedTransitionUpcomingTime.year = 0;
    g_timedTransitionUpcomingTime.month = 0;
    g_timedTransitionUpcomingTime.day = 0;
    g_timedTransitionUpcomingTime.hours = 0;
    g_timedTransitionUpcomingTime.minutes = 0;
    g_timedTransitionUpcomingTime.seconds = 0;
}

void SCEN_UpdateTimedTransitionScheduler() {
    UpdateTimedTransitionSchedulerInternal();
}

} // namespace shenmue::s1
