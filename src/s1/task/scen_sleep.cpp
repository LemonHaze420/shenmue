#include "s1/task/scen.hpp"
#include "s1/task/conv.hpp"
#include "logging.hpp"

#include "s1/archive_data.hpp"
#include "s1/boot.hpp"
#include "s1/audio_stream.hpp"
#include "s1/modules/arcade.hpp"
#include "s1/cycle_stack.hpp"
#include "s1/collision.hpp"
#include "s1/chrd.hpp"
#include "s1/task/char.hpp"
#include "s1/task/gcpn.hpp"
#include "s1/game_utils.hpp"
#include "s1/task/came.hpp"
#include "s1/task/cach.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/time.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/file_loader.hpp"
#include "s1/face.hpp"
#include "s1/task/film.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/task/effe.hpp"
#include "s1/initialization.hpp"
#include "s1/lighting.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/input.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/atck.hpp"
#include "s1/task/cuff.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/dswk.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/edtk.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/obje.hpp"
#include "s1/motion_registry.hpp"
#include "s1/motion.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/gull.hpp"
#include "s1/task/hato.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/lamp.hpp"
#include "s1/task/lght.hpp"
#include "s1/task/mitk.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/modu.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/ppad.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/qte.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/save_format.hpp"
#include "s1/scene.hpp"
#include "s1/task/scrl.hpp"
#include "s1/task/save.hpp"
#include "s1/task/kdan.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/sond.hpp"
#include "s1/subtitles.hpp"
#include "s1/timing.hpp"
#include "s1/task/tmex.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/updb.hpp"
#include "s1/task/wind.hpp"
#include "s1/task/wexe.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/trwv.hpp"
#include "s1/task/telm.hpp"
#include "s1/texture_cache.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/texture_loader.hpp"

#include <algorithm>

#include <array>
#include <bit>
#include <cctype>
#include <cstddef>
#include <cmath>
#include <numbers>
#include <cstdint>
#include <limits>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <utility>

#include "s1/task/scen_internal.hpp"

namespace shenmue::s1 {
using namespace scen_detail;

namespace scen_detail {

void Script_SetHomeArea(ScenarioContext *, void *rawParams) {
    const auto *const offset = static_cast<const std::uint32_t *>(rawParams);
    const HomeAreaParameters *const params = offset ? ScenarioPointer<const HomeAreaParameters>(*offset) : nullptr;
    if (!params)
        return;
    const auto promptStart = static_cast<std::int16_t>(static_cast<std::int16_t>(params->promptHour) * 60 + params->promptMinute);
    g_sleepPromptWindowStartMinutes = promptStart;
    g_sleepPromptWindowEndMinutes = static_cast<std::int16_t>(promptStart + 10);
    g_forcedSleepWindowStartMinutes = static_cast<std::int16_t>(static_cast<std::int16_t>(params->forcedHour) * 60 + params->forcedMinute);
    g_forcedSleepWindowEndMinutes = 1919;
    g_homeAreaId = params->areaId;
    g_homeSceneId = params->sceneId;
    g_requestedEntryPoint = params->entryPoint;
}
} // namespace scen_detail

void SetShouldSleepFlag(std::uint8_t mode) {
    const auto flagIsSet = [](std::uint16_t flag) { return (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0; };
    std::uint8_t current = g_shouldSleep;
    for (;;) {
        if (mode == 0) {
            g_shouldSleep = static_cast<std::uint8_t>(current | 0x40U);
            return;
        }
        if (mode == 1) {
            g_shouldSleep = static_cast<std::uint8_t>(current & 0xbfU);
            return;
        }
        if (mode != 2)
            break;
        g_shouldSleep = 0;
        if (flagIsSet(0x249U)) {
            if (!flagIsSet(0x0baU))
                current = 0x20U;
            g_shouldSleep = current;
        }
        mode = 1;
    }
    if (mode != 3)
        return;
    (void)flagIsSet(0x248U);
    if ((current & 0x20U) != 0 && flagIsSet(0x249U))
        current = static_cast<std::uint8_t>(current & 0xdfU);
    g_shouldSleep = static_cast<std::uint8_t>(current | 0x1eU);
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
namespace scen_detail {

void Script_SetShouldSleepFlag(ScenarioContext *, void *rawParams) {
    SetShouldSleepFlag(*static_cast<const std::uint8_t *>(rawParams));
}

void RyoMoveToHomeSleep() {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!ryo)
        return;

    shenApplyTranslationToAll(ryo->entity.node, g_ryoHomeSleepPosition);
    std::int16_t yaw = static_cast<std::int16_t>(g_ryoHomeSleepYaw);
    for (CharacterNode *child = ryo->entity.node.child; child; child = child->child) {
        yaw = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(child->rotation.y));
    }
    ryo->entity.node.rotation.y = yaw;
    ryo->eventGroup = g_temporaryRyoEventFlags;
    const std::int32_t previousAuthorState = g_nAuthorState;
    g_temporaryKaidanId = g_defaultKaidanId;
    g_playerKaidanGuideActive = g_ryoHomeSleepGuideWord;
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = 9;
    ApplyPendingGameState(9);
    if ((g_searchLockOnFlags & 1U) != 0)
        SRCH_ClearLockAndHandleReset();
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_previousAuthorState = previousAuthorState;
    ApplyPendingGameState(previousAuthorState);
    g_bIsConversationCamInactive = 1;
}

void Script_ryoSleepUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const RyoSleepParameters *>(rawParams);
    if (!context || !context->stack || !params)
        return;

    switch (params->mode) {
    case 0:
        context->stack->accumulator = g_pendingSleepResumeLoad ? 0xffffffffU : 0U;
        break;
    case 1:

        RyoMoveToHomeSleep();
        break;
    case 2:
        g_saveScreenFlag = params->operation != 0;
        break;
    case 3: {
        std::uint8_t *const address = g_ryoHomeSleepAuxBlock.data() + params->offset;
        switch (params->operation) {
        case 0:
            context->stack->accumulator = *address;
            break;
        case 1:
            context->stack->accumulator = *address;
            *address = static_cast<std::uint8_t>(params->value);
            break;
        case 2:
            std::memcpy(&context->stack->accumulator, address, sizeof(std::uint32_t));
            break;
        case 3:
            std::memcpy(&context->stack->accumulator, address, sizeof(std::uint32_t));
            std::memcpy(address, &params->value, sizeof(std::uint32_t));
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

std::int64_t SCNF_RequestSleepSaveTraining(ScenarioContext *, void *rawParams) {
    if (rawParams) {
        UI_ICSVRequest(static_cast<std::int8_t>(*static_cast<const std::uint32_t *>(rawParams)));
    }
    return 0;
}

std::int64_t SCNF_WaitSleepSaveTraining(ScenarioContext *context, void *) {
    if (!context || !context->stack)
        return 0;
    std::uint32_t result = 0;
    if (!UI_ICSVPeekScriptResult(result))
        return 0;
    context->stack->accumulator = result;
    return 0x10000;
}
std::uint32_t UpdateSleepPromptScenario(std::int32_t mode, bool forced) {
    if (mode == 1) {
        constexpr std::array<std::uint32_t, 4> promptNames{{
            SHEN_NAME(SY07),
            SHEN_NAME(SY08),
            SHEN_NAME(SY09),
            SHEN_NAME(SY10),
        }};
        const bool namedAct = DetermineActNum() != 0;
        std::int16_t stateValue = 0;
        if (!namedAct) {
            const auto flagIsSet = [](std::uint16_t flag) { return (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0; };
            if (flagIsSet(0x248U) && !flagIsSet(0x0b8U))
                g_shouldSleep |= 0x20U;
            stateValue = static_cast<std::int16_t>((g_shouldSleep >> 5U) & 1U);
        }
        g_cpctPreviousOperand = promptNames[(namedAct ? 2U : 0U) + (forced ? 1U : 0U)];
        if (!g_cpctInputUpdatePending && g_cpctSleepPromptReady) {
            EnqueueSCNT(g_cpctPreviousOperand, stateValue);
            g_cpctInputUpdatePending = true;
            return 1;
        }
        return 0;
    }
    if (mode == 2) {
        SCEN_FinalizeReadyPlayerControlRuntimeObjects();
        return 0;
    }
    if (mode == 3)
        return !g_cpctInputUpdatePending && g_cpctSleepPromptReady ? 1U : 0U;

    g_cpctPreviousOperand = 0;
    g_cpctSleepPromptReady = true;
    g_cpctInputUpdatePending = false;
    return 0;
}
} // namespace scen_detail

bool SCEN_HasSleepTransitionFinished(const PlayerControlWork &player, std::int32_t mode) {
    constexpr std::uint8_t kRegularSleepRequested = 0x40U;
    if ((g_shouldSleep & kRegularSleepRequested) != 0)
        return false;

    MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (mode != 0) {
        return mode == 1 && g_TMEX_CurrentMode == 0 && control && (control->flags & 1U) == 0;
    }
    if (player.controlGate < 0 || static_cast<std::int32_t>(player.ticks) < 150 || !ShouldRun() || !control->padCaptureEntry ||
        (static_cast<PadCaptureEntry *>(control->padCaptureEntry)->flags & 2U) == 0 || shenHasFocusedNotLockedOn()) {
        return false;
    }

    const auto *const film = control->filmTask && control->filmTask->task_data_ptr ?
        reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(control->filmTask->task_data_ptr) + 4) :
        nullptr;
    const auto flagIsSet = [](std::uint16_t flag) { return (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0; };
    return !g_NumFramesProcessed && (!film || film->current[0] == 0) && g_nAuthorState == 0 && (!flagIsSet(0x1aeU) || flagIsSet(0x1b8U));
}

void SCEN_UpdatePlayerSleepScheduler(PlayerControlWork &player) {
    const std::uint8_t sleepSaveTrainingReadiness = UI_ICSVNativeReadinessComposite();
    if (g_sleepSaveTrainingReadiness != 7U && sleepSaveTrainingReadiness == 7U) {
        SetShouldSleepFlag(3);
    }
    g_sleepSaveTrainingReadiness = sleepSaveTrainingReadiness;

    constexpr std::uint8_t kFadeActive = 0x01U;
    constexpr std::uint8_t kHomeStateActive = 0x02U;
    constexpr std::uint8_t kPromptTriggered = 0x04U;
    constexpr std::uint8_t kForcedSleepPending = 0x08U;
    constexpr std::uint8_t kTmexPromptActive = 0x10U;
    constexpr std::uint8_t kRegularSleepRequested = 0x40U;
    const auto flagIsSet = [](std::uint16_t flag) { return (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0; };
    MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const auto movePlayerHome = [&]() {
        if (!control)
            return;
        DetermineActNum();
        std::uint32_t area = g_homeAreaId.name;
        std::uint32_t entry = g_requestedEntryPoint;
        if (flagIsSet(0x118U) && !flagIsSet(0x11bU)) {
            area = SHEN_NAME(JHD0);
            entry = 1;
        } else {
            const bool useHome = flagIsSet(0x316U) || flagIsSet(0x1f4U) || (g_currentDateTime.month == 12 && g_currentDateTime.day < 26) ||
                g_currentDateTime.hours != 23 || g_areaId == SHEN_NAME(JOMO) || g_areaId == SHEN_NAME(JHD0) || g_areaId == SHEN_NAME(YB11) ||
                g_areaId == SHEN_NAME(JU00) || ((!flagIsSet(0x0a0U) || flagIsSet(0x159U)) && (!flagIsSet(0x15eU) || flagIsSet(0x1f4U)));
            if (!useHome) {
                area = SHEN_NAME(JD00);
                entry = 30;
            }
        }
        control->sceneId = g_actNumber;
        control->areaId = area;
        control->entryPoint = entry;
        control->module = SystemModule::Map;
        control->flags |= 1U;
        g_shouldOverrideClearColour = 1;
        if (g_shouldUseTemporaryClearColour) {
            g_overrideClearColour = (static_cast<std::uint32_t>(g_temporaryClearColour.red) << 24U) |
                (static_cast<std::uint32_t>(g_temporaryClearColour.green) << 16U) | (static_cast<std::uint32_t>(g_temporaryClearColour.blue) << 8U) |
                static_cast<std::uint32_t>(g_temporaryClearColour.alpha);
        } else {
            g_overrideClearColour = 0xff000000U;
        }
    };
    const auto enqueueSleepPrompt = [](bool forced) { UpdateSleepPromptScenario(1, forced); };

    std::int32_t hour = g_currentDateTime.hours;
    if (hour < 8)
        hour += 24;
    const std::int32_t minutes = 60 * hour + g_currentDateTime.minutes;

    if ((g_shouldSleep & kHomeStateActive) == 0) {
        if ((g_shouldSleep & kFadeActive) != 0) {
            const auto *const film = control && control->filmTask && control->filmTask->task_data_ptr ?
                reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(control->filmTask->task_data_ptr) + 4) :
                nullptr;
            if (!g_NumFramesProcessed && (!film || film->remainingFrames == 0)) {
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
                if (g_areaId == player.areaId) {
                    DetermineActNum();
                } else {
                    movePlayerHome();
                }
                g_shouldSleep = static_cast<std::uint8_t>((g_shouldSleep & 0xfcU) | kHomeStateActive);
                if ((g_shouldSleep & 0x20U) != 0 && (flagIsSet(0x248U) || flagIsSet(0x249U))) {
                    g_shouldSleep = static_cast<std::uint8_t>(g_shouldSleep & 0xdfU);
                }
            }
        } else {
            const auto *const film = control && control->filmTask && control->filmTask->task_data_ptr ?
                reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(control->filmTask->task_data_ptr) + 4) :
                nullptr;
            const bool promptWindow =
                (g_shouldSleep & kPromptTriggered) == 0 && minutes >= g_sleepPromptWindowStartMinutes && minutes <= g_sleepPromptWindowEndMinutes;
            if (promptWindow && SCEN_HasSleepTransitionFinished(player, 0)) {
                g_shouldSleep |= kPromptTriggered;
                navRequestNewMode(1);
                g_suppressDefaultHudPromptIcons = 1;
                player.blockedCanControl = 0;
                player.state = 2;
                g_tmexValHigh = 0;
                enqueueSleepPrompt(false);
                player.reserved25f = std::byte{1};
            }

            ScenRuntimeObject *const active = g_scenRuntimeState.activeObject;
            const bool activeAllowsSleep = !active || (active->runtimeState >= 1 && active->runtimeState <= 3 && (active->runtimeFlags & 0x000e0000U) == 0);
            if ((g_shouldSleep & kForcedSleepPending) != 0 && activeAllowsSleep) {
                g_shouldSleep |= kFadeActive;
                g_TMEX_forceShowWatchNode162 = 0;
                ctrlSetCurrentFilmFade();
            } else {
                const std::uint8_t state = g_shouldSleep;
                if ((state & (kForcedSleepPending | kTmexPromptActive)) == 0 && minutes >= g_forcedSleepWindowStartMinutes &&
                    minutes <= g_forcedSleepWindowEndMinutes && (state & kRegularSleepRequested) == 0 && g_TMEX_CurrentMode == 0 && control &&
                    (control->flags & 1U) == 0) {
                    if (TMEXUtilities(static_cast<std::uint32_t>(TMEXUtilityMode::OpenSleep)) != 0) {
                        g_shouldSleep |= kTmexPromptActive;
                    }
                } else if ((state & (kForcedSleepPending | kTmexPromptActive)) == kTmexPromptActive) {
                    if (g_tmex_val == 0) {
                        SetStatOrNotebookFlag(SHENMUE_FLAG_EV_STOP_ACTIVE, false);
                        SetStatOrNotebookFlag(SHENMUE_FLAG_SLEEP_TRANSITION_MODAL_ACTIVE, false);
                        g_shouldSleep |= kForcedSleepPending;
                        enqueueSleepPrompt(true);
                    } else if (g_tmex_val >= 2 && g_tmex_val <= 4 && (minutes < g_forcedSleepWindowStartMinutes || (state & kRegularSleepRequested) != 0)) {
                        DestroyTMEX();
                        shenRefreshUI();
                        UI_ResetConversationDetailsState();
                        shenExecuteAICACommand(0x30babU, 0, 0);
                        SetStatOrNotebookFlag(SHENMUE_FLAG_EV_STOP_ACTIVE, false);
                        SetStatOrNotebookFlag(SHENMUE_FLAG_SLEEP_TRANSITION_MODAL_ACTIVE, false);
                        g_shouldSleep = static_cast<std::uint8_t>(g_shouldSleep & ~kTmexPromptActive);
                        g_TMEX_Flag = 5;
                        g_tmex_val = 0;
                        g_TMEX_CurrentMode = 0;
                        g_tmexValHigh = 0;
                    }
                } else if (minutes >= 540 && minutes <= 1319) {
                    g_shouldSleep &= kRegularSleepRequested;
                }
            }
        }
    }

    if (player.reserved25f != std::byte{}) {
        if (SCEN_HasActiveScript()) {
            player.busy = 1;
        } else {
            player.reserved25f = std::byte{};
            navResetAuthAndInputStates();
        }
    }
}
} // namespace shenmue::s1
