#include "s2/pause.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/author_control_state_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_talk_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/weather_particles_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/system_modules.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/backup_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/initialization_dc.hpp"
#define SHENMUE_FRAME_NULL 0
#define SHENMUE_FRAME_COUNT(value) (sizeof(value) / sizeof(value[0]))
#define SHENMUE_FRAME_DATA(value) value
#define SHENMUE_FRAME_MODULE(value) SystemModule_##value
#include "s2/magic_room_dc.hpp"
#include "s2/system_modules_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/map_control_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/math.hpp"
#include "s2/stream_audio_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <cri_adxf.h>
#include <cstring>
#endif
#include "s2/scenario_flags_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/event_overlay.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/arcade_rankings_dc.hpp"
#include "s2/texture_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "s2/scene_data.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include <cstdio>
#include "s2/arcade_modules_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "platform/dc/aica.hpp"
#include "platform/dc/aica_stream.hpp"
#include "platform/dc/ninja.hpp"
#include <sg_gd.h>
#if SHENMUE_S2_GCC295
#define SHENMUE_SYSTEM_MODULE(name) SystemModule_##name
#define SHENMUE_SYSTEM_MODULE_NULL 0
#else
#define SHENMUE_SYSTEM_MODULE(name) SystemModule::name
#define SHENMUE_SYSTEM_MODULE_NULL nullptr
#endif

namespace shenmue {
namespace s2 {

bool IsPauseInputAllowed();
void SetLoadingBusy();
void ReleaseConversationBuffersIfPlaybackDisabled(std::int32_t releaseBuffers);

HLib::Task *g_execPauseTask;

namespace {

void ExecPause_Callback(void *) {
    const std::uint32_t helpRequested = g_helpIconState;
    const std::uint32_t entryFlags = g_loadingFlag;
    g_helpIconState = 0;
    if (entryFlags & 0x20eU) {
        if ((entryFlags & 0x10U) == 0) {
            if (g_inputBuffer[0].held == 6)
                g_loadingFlag = entryFlags | 0x10U;
        } else if (g_inputBuffer[0].pressedPriority & 0xf6c6U) {
            g_loadingFlag &= ~0x10U;
        }
        const std::uint32_t flags = g_loadingFlag;
        if ((flags & 0x400U) == 0 &&
            (((flags & 0x200U) == 0 && (flags & 0x820U)) || (g_inputBuffer[0].pressedPriority & 0x800U) || helpRequested ||
                (g_inputBuffer[2].pressedPriority & 0x800U))) {
            const std::uint32_t resumedFlags = g_loadingFlag & ~0xe1eU;
            g_loadingFlag = resumedFlags;
            if ((resumedFlags & 0x100U) == 0)
                goto resumeOwners;
        }
        goto replayCallbacks;
    }
    if ((IsPauseInputAllowed() && (g_loadingFlag & 0x20U) == 0 && (g_inputBuffer[0].pressedPriority & 0x800U)) || helpRequested ||
        (g_inputBuffer[2].pressedPriority & 0x800U) || (g_loadingFlag & 0x400U)) {
        const std::uint32_t pausedFlags = g_loadingFlag | 2U;
        g_loadingFlag = pausedFlags;
        if (g_inputBuffer[2].pressedPriority & 0x800U)
            g_loadingFlag = pausedFlags | 0x200U;
        if ((g_loadingFlag & 0x100U) == 0) {
            HLib::PauseTaskRangeExecution(3, 13);
            PauseScenarioLineClock(1);
            CacheCurrentParticleEffects(1);
            SetSoundPaused(1);
            ReleaseConversationBuffersIfPlaybackDisabled(1);
            SetRainAndRippleFlagsTo(1);
        }
    }
    if (g_loadingFlag & 2U)
        goto replayCallbacks;
    if (g_loadingFlag & 0x100U) {
        if (IsSaveAllowed())
            goto replayCallbacks;
        g_loadingFlag &= ~0x100U;
    resumeOwners:
        HLib::UnpauseTaskEnqueue();
        PauseScenarioLineClock(0);
        CacheCurrentParticleEffects(0);
        SetSoundPaused(0);
        ReleaseConversationBuffersIfPlaybackDisabled(0);
        SetRainAndRippleFlagsTo(0);
    } else if (IsSaveAllowed()) {
        g_loadingFlag |= 0x100U;
        HLib::PauseTaskRangeExecution(3, 13);
        PauseScenarioLineClock(1);
        CacheCurrentParticleEffects(1);
        SetSoundPaused(1);
        ReleaseConversationBuffersIfPlaybackDisabled(1);
        SetRainAndRippleFlagsTo(1);
    }
replayCallbacks:
    if (entryFlags & 0x102U)
        HLib::ReplayPreservedCallbacks();
    if ((g_loadingFlag & 0x102U) == 0)
        HLib::ResetPreservedCallbacks();
}

} // namespace

// @verified - d
bool IsPauseInputAllowed() {
    if (!authGetLoaded())
        return true;
    if (g_actNumber != 0x50)
        return false;
    const std::uint32_t name = g_currentMap;
    if (static_cast<std::uint16_t>(name) == static_cast<std::uint32_t>(kPauseDeveloperArea))
        return true;
    return false;
}

// @verified - d
void ExecPause() {
    g_helpIconState = 0;
    g_execPauseTask = HLib::EnqueueTaskWithParameter(ExecPause_Callback, 15, 2, sizeof(std::uint32_t), SHEN_NAME4('P', 'A', 'U', 'S'));
    g_loadingFlag = 1;
    SetLoadingBusy();
}

void DestroyExecPause() {
    g_loadingFlag = 0;
    if (g_execPauseTask) {
        HLib::DestroyTaskInternal(g_execPauseTask);
        g_execPauseTask = 0;
    }
}

// @verified - d
void SetLoadingBusy() {
    g_loadingFlag |= 0x20U;
}

// @verified - d
void FinishLoading() {
    g_loadingFlag &= ~0x20U;
}

// @verified - d
std::uint32_t shenIsGamePaused() {
    return static_cast<std::uint8_t>(g_loadingFlag) & 2U;
}

void SetLoadingFlag80() {
    g_loadingFlag |= 0x80U;
}

void ClearLoadingFlag80() {
    g_loadingFlag &= ~0x80U;
}

// @verified - d
void RequestFightTelopPause() {
    g_loadingFlag |= 0x400U;
}

// @verified - d
void RequestFightTelopResume() {
    g_loadingFlag = (g_loadingFlag & ~0x400U) | 0x800U;
}

bool IsLoadingFlag10Set() {
    if (g_loadingFlag & 0x10U)
        return true;
    return false;
}

} // namespace s2
} // namespace shenmue

#else
#include "s2/pause.hpp"

#include "s2/conversation.hpp"
#include "s2/help.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/input.hpp"
#include "s2/particle_effects.hpp"
#include "s2/save.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {

std::uint32_t g_loadingFlag = 0;

namespace {

HLib::Task *execPauseTask = nullptr;     // g_pExecPause
std::int32_t execPauseValue = 5;         // g_dExecPauseVal
std::int32_t execPauseSnapshotState = 0; // g_dExecPauseVal_0
std::int32_t execPauseRequested = 0;     // g_execPauseRequested

void SuspendPauseOwners(std::int32_t paused) {
    AssignPreviousFrameTimeDelta(paused);
    CacheCurrentParticleEffects(paused);
    PrimeAicaEmulationForPause(paused);
    PauseOrDestroyAudioAndSubtitles(paused);
    SetRainAndRippleFlagsTo(paused);
}

void ResumeGame() {
    g_loadingFlag &= ~0x100U;
    HLib::UnpauseTaskEnqueue();
    SuspendPauseOwners(0);
}

void ExecPause_Callback(void *) {
    if (!HandleGameUIVisibility() && !execPauseRequested && (GetHelpIconEnabled() == 1 || !IsHelpOverlayVisible(UIElement::GameHelp))) {
        HLib::ResetPreservedCallbacks();
        return;
    }
    const std::uint32_t helpRequested = g_helpIconState;
    if (execPauseRequested) {
        g_inputBuffer[0].pressedPriority |= 0x800U;
        execPauseRequested = 0;
    }
    const std::uint16_t entryFlags = static_cast<std::uint16_t>(g_loadingFlag);
    g_helpIconState = 0;
    if (ReturnWindowHandle() == 0)
        g_loadingFlag |= 2U;
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    const bool slotTwoStart = (g_inputBuffer[2].pressedPriority & 0x800U) != 0;
    if (g_loadingFlag & 0x20eU) {
        if ((g_loadingFlag & 0x10U) && (pressed & 0xf6c6U))
            g_loadingFlag &= ~0x10U;
        const std::uint32_t flags = g_loadingFlag;
        if ((flags & 0x400U) == 0 &&
            (((flags & 0x200U) == 0 && (flags & 0x820U)) || (pressed & 0x800U) || helpRequested || slotTwoStart || (pressed & 0x200U))) {
            g_loadingFlag = flags & ~0xe1eU;
            if ((g_loadingFlag & 0x100U) == 0) {
                SelectInputDeviceForButtons(0x800);
                HLib::UnpauseTaskEnqueue();
                SuspendPauseOwners(0);
            }
        }
    } else {
        if ((execPauseValue < 0 && (g_loadingFlag & 0x20U) == 0 && (pressed & 0x800U)) || helpRequested || slotTwoStart || (g_loadingFlag & 0x400U)) {
            g_loadingFlag |= 2U;
            if (slotTwoStart)
                g_loadingFlag |= 0x200U;
            if ((g_loadingFlag & 0x100U) == 0) {
                HLib::PauseTaskRangeExecution(3, 13);
                SuspendPauseOwners(1);
            }
        }
        if ((g_loadingFlag & 2U) == 0) {
            if ((g_loadingFlag & 0x100U) == 0) {
                if (snapCheckState() && !execPauseSnapshotState)
                    execPauseSnapshotState = 1;
                if (IsSaveAllowed() || execPauseSnapshotState == 1) {
                    g_loadingFlag |= 0x100U;
                    HLib::PauseTaskRangeExecution(3, 13);
                    SuspendPauseOwners(1);
                }
            } else if (execPauseSnapshotState == 1) {
                if (!snapCheckState()) {
                    execPauseSnapshotState = 0;
                    ResumeGame();
                }
            } else if (!IsSaveAllowed()) {
                ResumeGame();
            }
        }
    }
    if (entryFlags & 0x102U)
        HLib::ReplayPreservedCallbacks();
    if ((g_loadingFlag & 0x102U) == 0)
        HLib::ResetPreservedCallbacks();
}

} // namespace

HLib::Task *ExecPause() {
    g_helpIconState = 0;
    execPauseTask = HLib::EnqueueTaskWithParameter(ExecPause_Callback, 15, 2, sizeof(std::uint32_t), TASK_PAUS);
    g_loadingFlag = 1;
    execPauseValue = -1;
    execPauseSnapshotState = 0;
    return execPauseTask;
}

// @verified - p
void FinishLoading() {
    g_loadingFlag &= ~0x20U;
}

// @verified - p
std::uint64_t shenIsGamePaused() {
    return g_loadingFlag & 2U;
}

// @verified - p
void RequestExecPause() {
    execPauseRequested = 1;
}

// @verified - p
bool GetNotPausedOrLoading() {
    return execPauseValue < 0 && (g_loadingFlag & 0x20U) == 0;
}

// @verified - p
std::uint64_t GetLoadingBusyFlag() {
    return g_loadingFlag & 0x20U;
}

// @verified - p
void RequestFightTelopPause() {
    g_loadingFlag |= 0x400U;
}

// @verified - p
std::uint64_t RequestFightTelopResume() {
    g_loadingFlag = (g_loadingFlag & ~0x400U) | 0x800U;
    return g_loadingFlag;
}

} // namespace shenmue::s2

#endif
