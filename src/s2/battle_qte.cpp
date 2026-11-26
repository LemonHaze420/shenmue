#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/battle_qte.hpp"
#include "s2/fight.hpp"
#include "s2/input_dc.hpp"
#include "s2/pause.hpp"
#include "s2/sound_program.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include <cstring>
#include "s2/hlib.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/fight_stage.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/camera_dc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/texture_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/window.hpp"
#include "s2/conversation_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#include "s2/character_control_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/math.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/matrix.hpp"
#include <cstdio>
#include "s2/motion_registry_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/osage_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/conversation_fade_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include "s2/model_dc.hpp"
#include "s2/camera_task_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/camera_search_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include <math.h>
#include <float.h>
#include "s2/camera_conversation_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/npc_data_dc.hpp"
#endif
#else
#include "s2/battle_qte.hpp"

#include "s2/fight.hpp"
#include "s2/input.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "d3t/texture.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

HLib::Task *battleQteTask{};

BattleQteCommand command = battleQteDefaultCommand;

std::uint8_t completedCommands;

std::uint16_t activeState{}, bufferedInput{};

bool firstInputFrame{true};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

BattleQteTaskData *GetBattleQteTaskData();
void EndBattleQtePause();

} // namespace

void ResetBattleQteTaskPointer() noexcept;
std::uint16_t vfqConvertCommandMask(std::int32_t value) noexcept;
void vfqResetBattleQte(BattleQteTaskData &data) noexcept;
std::uint16_t vfqPollBattleQteInput(BattleQteTaskData &data);
void vfqSetBattleQteIcons(BattleQteTaskData &data, std::uint8_t mode, std::uint16_t mask, std::uint16_t frames) noexcept;
void EnableQteUi(BattleQteTaskData &data);
void vfq_BattleQteMain_Callback(void *parameter);
HLib::Task *Queue_vfq_BattleQteMain();
bool vfqIsBattleQteActive() noexcept;
void fbqtToggleMode();
BattleQteTaskData *fbqtDisableMode();
std::int64_t qteBattleUtil(BattleQteMode mode, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

HLib::Task *Queue_vfq_BattleQteMain();

void fbqtToggleMode();

BattleQteTaskData *fbqtDisableMode();

static void DrawBattleQteIcons(BattleQteTaskData &data);

static void DrawBattleQteDirections(BattleQteTaskData &data);

static std::int32_t vfqCheckBattleQteSuccess(BattleQteTaskData &data, FightActorRuntimePrefix *actor, FightActorRuntimePrefix *player);

static void ResetBattleQteDirectionState();

static std::uint32_t LatchBattleQtePadMask();

static std::uint16_t BattleQteIdentity(std::uint16_t value, std::uint8_t count);

void fbqtResetStates(std::uint8_t mode);

FightRuntimePrefix *vfmaGetTaskParam();

void fbqtResetStates(std::uint8_t mode);

FightRuntimePrefix *vfmaGetTaskParam();

void fbqtResetStates(std::uint8_t mode);

FightRuntimePrefix *vfmaGetTaskParam();

void fbqtResetStates(std::uint8_t mode);

FightRuntimePrefix *vfmaGetTaskParam();

void fbqtResetStates(std::uint8_t mode);

FightRuntimePrefix *vfmaGetTaskParam();

void fbqtResetStates(std::uint8_t mode);

FightRuntimePrefix *vfmaGetTaskParam();

namespace {

HLib::Task *battleQteTask = 0;

BattleQteCommand command[1] = {{2, 30, 6, 30, 0, {0x40, 4, 0x10, 0x800, 0, 0, 0, 0}}};

static const BattleQteIcon battleQteDefaultIcons[5] = {{4, 206, 234, 2}, {2, 373, 232, 1}, {3, 428, 177, 1}, {0, 428, 287, 1}, {1, 483, 232, 1}};

static const std::uint16_t battleQteIconMasks[5] = {0xf000, 4, 2, 0x400, 0x200};

static const std::uint16_t battleQteCommandMasks[12] = {0x1000, 0x9000, 0x8000, 0xa000, 0x2000, 0x6000, 0x4000, 0x5000, 0x400, 0x200, 4, 2};

static float responseAverage;

static std::int32_t successCount, failureCount;

std::uint8_t completedCommands;

std::uint32_t bufferedInput = 0;

std::int32_t firstInputFrame = 1;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace {

BattleQteTaskData *GetBattleQteTaskData();

} // namespace

static void vfqInitBattleQteTask(BattleQteTaskData &data);
static void vfqInitBattleQteSequence(BattleQteTaskData &data);
void DrawMainIconOverlay(std::uint32_t colour, float depth);
static std::int32_t vfqRequestBattleQteSuccess();
static std::int32_t vfqGetBattleQteResult();
static std::uint8_t vfqGetBattleQteCompletedCount();
static std::uint8_t vfqCountBattleQteEntries();
void ResetBattleQteTaskPointer();
static std::int32_t vfqCheckBattleQteSuccess(BattleQteTaskData &data, FightActorRuntimePrefix *actor, FightActorRuntimePrefix *player);
std::uint16_t vfqConvertCommandMask(std::int32_t value);
void vfqResetBattleQte(BattleQteTaskData &data);
std::uint16_t vfqPollBattleQteInput(BattleQteTaskData &data);
void vfqSetBattleQteIcons(BattleQteTaskData &data, std::uint8_t mode, std::uint16_t mask, std::uint16_t frames);
std::int32_t qteBattleUtil(BattleQteMode mode, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5);
void EnableQteUi(BattleQteTaskData &data);
void vfq_BattleQteMain_Callback(void *parameter);
HLib::Task *Queue_vfq_BattleQteMain();
bool vfqIsBattleQteActive();
void fbqtToggleMode();
BattleQteTaskData *fbqtDisableMode();
static void DrawBattleQteIcons(BattleQteTaskData &data);
static void DrawBattleQteDirections(BattleQteTaskData &data);
static void ResetBattleQteDirectionState();
static std::uint32_t LatchBattleQtePadMask();
void SetBattleQteParameters(float average, std::uint8_t successes, std::uint8_t failures);
float GetBattleQteParameter(std::int32_t index);
static std::uint16_t BattleQteIdentity(std::uint16_t value, std::uint8_t);
#else
namespace {

BattleQteTaskData *GetBattleQteTaskData();
void EndBattleQtePause();

} // namespace

void ResetBattleQteTaskPointer() noexcept;
std::uint16_t vfqConvertCommandMask(std::int32_t value) noexcept;
void vfqResetBattleQte(BattleQteTaskData &data) noexcept;
std::uint16_t vfqPollBattleQteInput(BattleQteTaskData &data);
void vfqSetBattleQteIcons(BattleQteTaskData &data, std::uint8_t mode, std::uint16_t mask, std::uint16_t frames) noexcept;
std::int64_t qteBattleUtil(BattleQteMode mode, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5);
void EnableQteUi(BattleQteTaskData &data);
void vfq_BattleQteMain_Callback(void *parameter);
HLib::Task *Queue_vfq_BattleQteMain();
bool vfqIsBattleQteActive() noexcept;
void fbqtToggleMode();
BattleQteTaskData *fbqtDisableMode();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t qteBattleUtil(BattleQteMode mode, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5) {
    BattleQteCommand *cmd = command;
    BattleQteTaskData *const data = GetBattleQteTaskData();
    switch (mode) {
    case Load:
        LoadFBQTE();
        break;
    case ResetStates:
        fbqtResetStates(static_cast<std::uint8_t>(a2));
        break;
    case Queue:
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(Queue_vfq_BattleQteMain()));
    case Start:
        return vfqRequestBattleQteSuccess();
    case GetResult:
        return vfqGetBattleQteResult();
    case RetireAndFree:
        fbqtToggleMode();
        break;
    case GetCompletedCount:
        return vfqGetBattleQteCompletedCount();
    case GetCommandCount:
        return vfqCountBattleQteEntries();
    case SetWait:
        cmd->trigger = 8;
        cmd->initialWait = static_cast<std::uint16_t>(a2);
        cmd->commandWait = static_cast<std::uint16_t>(a3);
        cmd->resultWait = static_cast<std::uint16_t>(a4);
        if (data) {
            data->initialWait = static_cast<std::uint16_t>(a2);
            data->commandWait = static_cast<std::uint16_t>(a3);
            data->resultWait = static_cast<std::uint16_t>(a4);
        }
        break;
    case SetFirstCommands:
        cmd->commands[0] = a2;
        cmd->commands[1] = a3;
        cmd->commands[2] = a4;
        cmd->commands[3] = a5;
        break;
    case SetLastCommands:
        cmd->commands[4] = a2;
        cmd->commands[5] = a3;
        cmd->commands[6] = a4;
        cmd->commands[7] = a5;
        break;
    case Retire:
        fbqtDisableMode();
        break;
    case FreeSprites:
        FreeFBQTE();
        break;
    default:
        break;
    }
    return 1;
}

#else

// @verified - p
std::int64_t qteBattleUtil(BattleQteMode mode, std::int32_t a2, std::int32_t a3, std::int32_t a4, std::int32_t a5) {
    BattleQteTaskData *const data = GetBattleQteTaskData();
    switch (mode) {
    case BattleQteMode::Load:
        LoadFBQTE();
        break;
    case BattleQteMode::ResetStates:
        if (a2 == 0)
            fbqtResetStates(0);
        break;
    case BattleQteMode::Queue:
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(Queue_vfq_BattleQteMain()));
    case BattleQteMode::Start:
        SetBattleQteUIMode(2);
        if (!data || data->active != 0)
            return 0;
        data->mode |= 4U;
        return 1;
    case BattleQteMode::GetResult:
        return data ? ((data->mode & 1U) != 0 ? 1 : -static_cast<std::int64_t>((data->mode & 2U) != 0)) : 0;
    case BattleQteMode::RetireAndFree:
        fbqtToggleMode();
        break;
    case BattleQteMode::GetCompletedCount:
        return completedCommands;
    case BattleQteMode::GetCommandCount: {
        std::uint8_t count{};
        if (g_battleQteCommandSelection == 0)
            while (count < 8 && command.commands[count] != 0)
                ++count;
        return count;
    }
    case BattleQteMode::SetWait:
        command.trigger = 8;
        command.initialWait = static_cast<std::uint16_t>(a2);
        command.commandWait = static_cast<std::uint16_t>(a3);
        command.resultWait = static_cast<std::uint16_t>(a4);
        if (data) {
            data->initialWait = command.initialWait;
            data->commandWait = command.commandWait;
            data->resultWait = command.resultWait;
        }
        break;
    case BattleQteMode::SetFirstCommands:
        command.commands[0] = a2;
        command.commands[1] = a3;
        command.commands[2] = a4;
        command.commands[3] = a5;
        break;
    case BattleQteMode::SetLastCommands:
        command.commands[4] = a2;
        command.commands[5] = a3;
        command.commands[6] = a4;
        command.commands[7] = a5;
        break;
    case BattleQteMode::Retire:
        fbqtDisableMode();
        break;
    case BattleQteMode::FreeSprites:
        FreeFBQTE();
        break;
    default:
        break;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
HLib::Task *Queue_vfq_BattleQteMain() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(vfq_BattleQteMain_Callback, 15, 2, sizeof(BattleQteTaskData), SHEN_NAME(FBQT));
    vfqInitBattleQteTask(*static_cast<BattleQteTaskData *>(HLib::GetTaskParameter(task)));
    battleQteTask = task;
    return task;
}

#else

// @verified - p
HLib::Task *Queue_vfq_BattleQteMain() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(vfq_BattleQteMain_Callback, 15, 2, sizeof(BattleQteTaskData), TASK_FBQT);
    BattleQteTaskData &data = *static_cast<BattleQteTaskData *>(HLib::GetTaskParameter(task));
    activeState = 0;
    completedCommands = 0;
    data.mode = 0;
    data.active = 0;
    vfqResetBattleQte(data);
    battleQteTask = task;
    return task;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool vfqIsBattleQteActive() {
    const BattleQteTaskData *const data = GetBattleQteTaskData();
    return data && data->active != 0;
}

#else

bool vfqIsBattleQteActive() noexcept {
    return static_cast<std::uint8_t>(activeState) != 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void fbqtToggleMode() {
    GetBattleQteTaskData()->mode |= 0x80000000U;
}

#else

void fbqtToggleMode() {
    GetBattleQteTaskData()->mode |= 0x80000000U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

BattleQteTaskData *fbqtDisableMode() {
    BattleQteTaskData *const data = GetBattleQteTaskData();
    data->mode |= 0x40000000U;
    return data;
}

#else

BattleQteTaskData *fbqtDisableMode() {
    BattleQteTaskData *const data = GetBattleQteTaskData();
    data->mode |= 0x40000000U;
    return data;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void vfqInitBattleQteTask(BattleQteTaskData &data) {
    data.mode = 0;
    data.active = 0;
    completedCommands = 0;
    vfqInitBattleQteSequence(data);
}

// @verified - d
static void vfqInitBattleQteSequence(BattleQteTaskData &data) {
    data.inputCommand = data.shownCommand = 0;
    data.remaining = 0;
    data.initialWait = command[g_battleQteCommandSelection].initialWait;
    data.commandWait = command[g_battleQteCommandSelection].commandWait;
    data.resultWait = command[g_battleQteCommandSelection].resultWait;
    for (std::int32_t index = 0; index < 8; ++index)
        data.commandAges[index] = 0;
    vfqResetBattleQte(data);
}

// @verified - d
void vfq_BattleQteMain_Callback(void *parameter) {
    BattleQteTaskData &data = *static_cast<BattleQteTaskData *>(parameter);
    if ((data.mode & 0xc0000000U) != 0 && data.active == 0) {
        if ((data.mode & 0x80000000U) != 0)
            FreeFBQTE();
        HLib::DestroyTaskInternal(battleQteTask);
        battleQteTask = 0;
        return;
    }
    std::int32_t playersFound = 0;
    FightActorRuntimePrefix *player = 0;
    FightActorRuntimePrefix *actor = 0;
    FightRuntimePrefix *const stage = vfmaGetTaskParam();
    if (stage && stage->playerSlot != -1) {
        actor = stage->actors[stage->playerSlot];
        if (actor) {
            player = actor->actor;
            if (player)
                ++playersFound;
        }
    }
    data.mode &= ~3U;
    std::int32_t pressed;
    std::int32_t mask;
    switch (data.active) {
    case 0:
        data.mode &= ~0x18U;
        if (playersFound) {
            if (stage->state == 3 && vfqCheckBattleQteSuccess(data, actor, player))
                data.active = 1;
        } else if (data.mode & 4U) {
            data.mode &= ~4U;
            data.active = 1;
        }
        if (data.active == 1)
            RequestFightTelopPause();
        break;
    case 1:
        data.remaining = data.initialWait;
        completedCommands = 0;
        ResetBattleQteDirectionState();
        data.active = 2;
        ExecuteAicaCommand(0x1100abU, 0, 0);
        break;
    case 2:
        if (data.remaining == 0) {
            data.active = 3;
            data.requiredMask = vfqConvertCommandMask(command[g_battleQteCommandSelection].commands[data.inputCommand]);
        }
        break;
    case 3:
        data.remaining = data.commandWait;
        mask = vfqConvertCommandMask(command[g_battleQteCommandSelection].commands[data.shownCommand]);
        vfqSetBattleQteIcons(data, 0, mask, data.remaining - 1);
        if ((data.mode & 0x18U) == 0)
            vfqPollBattleQteInput(data);
        ExecuteAicaCommand(0x1300abU, 0, 0);
        data.active = 4;
        break;
    case 4:
        if ((data.mode & 0x18U) == 0)
            vfqPollBattleQteInput(data);
        if (data.remaining == 0) {
            if (++data.shownCommand < 8 && command[g_battleQteCommandSelection].commands[data.shownCommand] != 0)
                data.active = 3;
            else
                data.active = 5;
        }
        break;
    case 5:
        if ((data.mode & 0x18U) == 0)
            vfqPollBattleQteInput(data);
        data.remaining = BattleQteIdentity(data.resultWait, vfqCountBattleQteEntries());
        data.active = 6;
        break;
    case 6:
        if ((data.mode & 0x18U) == 0)
            pressed = vfqPollBattleQteInput(data);
        else
            pressed = 0;
        if (pressed != 0)
            vfqSetBattleQteIcons(data, 1, pressed, 10);
        if ((data.mode & 8U) != 0) {
            data.remaining = 60;
            vfqSetBattleQteIcons(data, 2, pressed, 15);
            data.active = 7;
        } else if ((data.mode & 0x10U) != 0) {
            if (pressed == 0) {
                data.remaining = 30;
                vfqSetBattleQteIcons(data, 3, 0, 0);
            } else {
                data.remaining = 60;
                vfqSetBattleQteIcons(data, 3, pressed, 30);
            }
            data.active = 7;
        }
        if (data.active == 7) {
            if ((data.mode & 0x10U) != 0) {
                data.retireWait = 20;
            } else {
                RequestFightTelopResume();
                data.retireWait = 1;
            }
        }
        break;
    case 7:
        switch (--data.retireWait) {
        case 1:
            RequestFightTelopResume();
            break;
        case 0:
            if (data.mode & 0x10U) {
                data.mode |= 2U;
                if (failureCount < 255)
                    ++failureCount;
            }
            if (data.mode & 8U) {
                data.mode |= 1U;
                if (successCount < 255)
                    ++successCount;
                float frames = 0;
                std::int32_t count = 0;
                std::int32_t index;
                for (index = 0; index < data.inputCommand; ++index) {
                    ++count;
                    if (data.commandAges[index])
                        frames += static_cast<float>(data.commandAges[index]);
                }
                for (index = 0; index < count; ++index)
                    frames -= static_cast<float>(command[g_battleQteCommandSelection].commandWait * index);
                frames /= static_cast<float>(count) * 30.0f;
                if (frames < 0.05f)
                    frames = 0.05f;
                responseAverage = (responseAverage + frames) * 0.5f;
            }
            vfqInitBattleQteSequence(data);
            data.active = 0;
            break;
        }
        break;
    default:
        break;
    }
    if (data.active >= 1 && data.active <= 4)
        DrawMainIconOverlay(0x7f000000U, 0.0f);
    if (data.active != 0)
        EnableQteUi(data);
    if (data.remaining != 0)
        --data.remaining;
}

#else

// @verified - p
void vfq_BattleQteMain_Callback(void *parameter) {
    BattleQteTaskData &data = *static_cast<BattleQteTaskData *>(parameter);
    if ((data.mode & 0xc0000000U) != 0 && data.active == 0) {
        if ((data.mode & 0x80000000U) != 0)
            FreeFBQTE();
        HLib::DestroyTask(battleQteTask);
        battleQteTask = nullptr;
        return;
    }
    VFMA_t *const stage = vfmaGetTaskParam();
    VFActor *actor = nullptr;
    VFActor *player = nullptr;
    if (stage && stage->playerSlot != -1) {
        actor = stage->actors[static_cast<std::uint8_t>(stage->playerSlot)];
        if (actor)
            player = actor->actor;
    }
    data.mode &= ~3U;
    RefreshBattleQteUIInputDevice();
    std::uint16_t pressed{};
    switch (data.active) {
    case 0:
        data.mode &= ~0x18U;
        if (!player || stage->state == 3) {
            if ((data.mode & 4U) != 0) {
                data.mode &= ~4U;
                data.active = 1;
            } else if (player && (actor->moveFlags & 0x10U) == 0 && (actor->hitStateFlags & 1U) == 0 && player->formationFlags < 0) {
                if ((command.trigger == 1 && (actor->reactionFlags & 1U) != 0) ||
                    (command.trigger == 2 && (actor->hitFlags & 0x2500U) != 0 && (player->hitStateFlags & 2U) != 0))
                    data.active = 1;
            }
        }
        if (data.active == 1)
            g_loadingFlag |= 0x400U;
        break;
    case 1:
        data.remaining = data.initialWait;
        completedCommands = 0;
        bufferedInput = 0;
        firstInputFrame = true;
        data.active = 2;
        ExecuteAicaCommand(0x1100abU, 0, 0);
        break;
    case 2:
        if (data.remaining == 0) {
            data.active = 3;
            data.requiredMask = vfqConvertCommandMask(command.commands[data.inputCommand]);
        }
        break;
    case 3:
        data.remaining = data.commandWait;
        vfqSetBattleQteIcons(data, 0, vfqConvertCommandMask(command.commands[data.shownCommand]), data.commandWait - 1);
        if ((data.mode & 0x18U) == 0)
            vfqPollBattleQteInput(data);
        ExecuteAicaCommand(0x1300abU, 0, 0);
        data.active = 4;
        break;
    case 4:
        if ((data.mode & 0x18U) == 0)
            vfqPollBattleQteInput(data);
        if (data.remaining == 0) {
            ++data.shownCommand;
            data.active = data.shownCommand < 8 && command.commands[data.shownCommand] != 0 ? 3 : 5;
        }
        break;
    case 5:
        if ((data.mode & 0x18U) == 0)
            vfqPollBattleQteInput(data);
        data.remaining = data.resultWait;
        data.active = 6;
        break;
    case 6:
        if ((data.mode & 0x18U) == 0) {
            pressed = vfqPollBattleQteInput(data);
            if (pressed != 0)
                vfqSetBattleQteIcons(data, 1, pressed, 10);
        }
        if ((data.mode & 8U) != 0) {
            data.remaining = 60;
            vfqSetBattleQteIcons(data, 2, pressed, 15);
            data.active = 7;
        } else if ((data.mode & 0x10U) != 0) {
            data.remaining = pressed ? 60 : 30;
            vfqSetBattleQteIcons(data, 3, pressed, pressed ? 30 : 0);
            data.active = 7;
        }
        if (data.active == 7) {
            if ((data.mode & 0x10U) != 0)
                data.retireWait = 20;
            else {
                EndBattleQtePause();
                data.retireWait = 1;
            }
        }
        break;
    case 7:
        if (--data.retireWait != 0) {
            if (data.retireWait == 1)
                EndBattleQtePause();
        } else {
            if ((data.mode & 0x10U) != 0)
                data.mode |= 2U;
            if ((data.mode & 8U) != 0)
                data.mode |= 1U;
            vfqResetBattleQte(data);
            data.active = 0;
        }
        break;
    default:
        break;
    }
    if (data.active >= 1 && data.active <= 4)
        SubmitScreenColourOverlay(0x7f000000U);
    if (data.active != 0) {
        EnableQteUi(data);
        ExecuteElementCallback(UIElement::GameQte);
    }
    if (data.remaining != 0)
        --data.remaining;
    activeState = data.active;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t vfqCheckBattleQteSuccess(BattleQteTaskData &data, FightActorRuntimePrefix *actor, FightActorRuntimePrefix *player) {
    if (data.mode & 4U) {
        data.mode &= ~4U;
        return 1;
    }
    if ((actor->moveFlags & 0x10U) || (actor->hitStateFlags & 1U) || static_cast<std::int32_t>(player->formationFlags) >= 0)
        return 0;
    std::uint32_t result;
    switch (command[g_battleQteCommandSelection].trigger) {
    case 1:
        result = actor->reactionFlags & 1U;
        break;
    case 2:
        do {
            const std::uint32_t flags = actor->hitFlags;
            if (!(flags & 0x2500U))
                return 0;
            result = player->hitStateFlags & 2U;
        } while (0);
        break;
    default:
        return 0;
    }
    if (!result)
        return 0;
    return 1;
}

// @verified - d
std::uint16_t vfqConvertCommandMask(std::int32_t value) {
    std::uint16_t result = 0;
    if (!value)
        return 0;
    for (std::uint8_t i = 0; i < 12; ++i)
        if ((1 << i) & value) {
            const std::uint16_t mask = battleQteCommandMasks[i];
            result |= mask;
        }
    return result;
}

#else

// @verified - p
std::uint16_t vfqConvertCommandMask(std::int32_t value) noexcept {
    std::uint16_t result{};
    for (std::size_t i = 0; i < battleQteCommandMasks.size(); ++i)
        if ((static_cast<std::uint32_t>(value) & (1U << i)) != 0)
            result |= battleQteCommandMasks[i];
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint16_t vfqPollBattleQteInput(BattleQteTaskData &data) {
    for (std::int32_t i = data.inputCommand; i < data.shownCommand; ++i)
        ++data.commandAges[i];
    std::uint16_t pressed = LatchBattleQtePadMask() & 0xf6c6U;
    std::uint16_t direction = pressed & 0xf000U;
    if (direction != 0) {
        const std::uint16_t buttons = data.requiredMask & 0x6c6U;
        if ((data.requiredMask & direction) != 0)
            direction = data.requiredMask & 0xf000U;
        pressed = buttons | direction;
    }
    if (pressed == data.requiredMask) {
        ++completedCommands;
        ++data.inputCommand;
        if (data.inputCommand < 8 && command[g_battleQteCommandSelection].commands[data.inputCommand] != 0) {
            data.requiredMask = vfqConvertCommandMask(command[g_battleQteCommandSelection].commands[data.inputCommand]);
            ExecuteAicaCommand(0xabU, 0, 0);
        } else {
            data.mode |= 8U;
            ExecuteAicaCommand(0x900abU, 0, 0);
        }
    } else if (pressed != 0 || (data.active == 6 && data.remaining == 0)) {
        data.mode |= 0x10U;
        ExecuteAicaCommand(0xa00abU, 0, 0);
    }
    return pressed;
}
#else

// @verified - p
std::uint16_t vfqPollBattleQteInput(BattleQteTaskData &data) {
    for (std::uint8_t i = data.inputCommand; i < data.shownCommand; ++i)
        ++data.commandAges[i];
    const LegacyInputSlot &input = g_inputBuffer[0];
    std::uint16_t pressed = input.newPresses & 0xf606U;
    if (firstInputFrame) {
        if (pressed != 0 && pressed != 0xf606U) {
            bufferedInput = pressed;
            pressed = 0;
            firstInputFrame = false;
        }
    } else {
        const std::uint16_t prior = pressed != 0 ? input.held & bufferedInput : bufferedInput;
        pressed |= prior;
        bufferedInput = 0;
        firstInputFrame = true;
    }
    pressed &= 0xf6c6U;
    std::uint16_t direction = pressed & 0xf000U;
    if (direction != 0) {
        if ((data.requiredMask & direction) != 0)
            direction = data.requiredMask & 0xf000U;
        pressed = direction | (data.requiredMask & 0x6c6U);
    }
    if (g_forcePlayerGodmode)
        pressed = data.requiredMask;
    if (pressed == data.requiredMask) {
        ++data.inputCommand;
        ++completedCommands;
        if (data.inputCommand < 8 && command.commands[data.inputCommand] != 0) {
            data.requiredMask = vfqConvertCommandMask(command.commands[data.inputCommand]);
            ExecuteAicaCommand(0xabU, 0, 0);
        } else {
            data.mode |= 8U;
            ExecuteAicaCommand(0x900abU, 0, 0);
        }
    } else if (pressed != 0 || (data.active == 6 && data.remaining == 0)) {
        data.mode |= 0x10U;
        ExecuteAicaCommand(0xa00abU, 0, 0);
    }
    return pressed;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

BattleQteTaskData *GetBattleQteTaskData() {
    return battleQteTask ? static_cast<BattleQteTaskData *>(HLib::GetTaskParameter(battleQteTask)) : 0;
}

} // namespace

static std::int32_t vfqRequestBattleQteSuccess() {
    BattleQteTaskData *const data = GetBattleQteTaskData();
    if (!data || data->active != 0)
        return 0;
    data->mode |= 4U;
    return 1;
}

static std::int32_t vfqGetBattleQteResult() {
    BattleQteTaskData *const data = GetBattleQteTaskData();
    if (!data)
        return 0;
    if (data->mode & 1U)
        return 1;
    if (data->mode & 2U)
        return -1;
    return 0;
}

static std::uint8_t vfqGetBattleQteCompletedCount() {
    return completedCommands;
}

static std::uint8_t vfqCountBattleQteEntries() {
    if (g_battleQteCommandSelection != 0)
        return 0;
    std::uint8_t count;
    for (count = 0; count < 8; ++count)
        if (!command[g_battleQteCommandSelection].commands[count])
            break;
    return count;
}

#else

namespace {

BattleQteTaskData *GetBattleQteTaskData() {
    return battleQteTask ? static_cast<BattleQteTaskData *>(HLib::GetTaskParameter(battleQteTask)) : nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#else

namespace {

void EndBattleQtePause() {
    g_loadingFlag = (g_loadingFlag & ~0xc00U) | 0x800U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// Native FreeTaskPtr: module heap sweep invalidates only the published FBQT task pointer.
void ResetBattleQteTaskPointer() {
    battleQteTask = 0;
}

#else

// Native FreeTaskPtr: module heap sweep invalidates only the published FBQT task pointer.
void ResetBattleQteTaskPointer() noexcept {
    battleQteTask = nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfqResetBattleQte(BattleQteTaskData &data) {
    std::uint8_t index;
    for (index = 0; index < 5; ++index) {
        const BattleQteIcon *const source = &battleQteDefaultIcons[index];
        BattleQteIcon *const icon = &data.icons[index];
        icon->kind = source->kind;
        icon->scale = source->scale;
        icon->x = source->x;
        icon->y = source->y;
    }
    data.shownMask = 0;
    data.shownFrames = 0;
    for (index = 0; index < 5; ++index)
        data.iconFrames[index] = 0;
    data.directionFlashFrames = 0;
    data.age = 0;
    data.directionFrames = 0;
    data.departureFrames = 0;
    data.resultIcon = 0;
}

#else

// @verified - p
void vfqResetBattleQte(BattleQteTaskData &data) noexcept {
    data.inputCommand = 0;
    data.shownCommand = 0;
    data.remaining = 0;
    data.initialWait = command.initialWait;
    data.commandWait = command.commandWait;
    data.resultWait = command.resultWait;
    data.commandAges = {};
    data.icons = battleQteDefaultIcons;
    data.shownMask = 0;
    data.shownFrames = 0;
    data.iconFrames = {};
    data.directionFlashFrames = 0;
    data.age = 0;
    data.directionFrames = 0;
    data.departureFrames = 0;
    data.resultIcon = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EnableQteUi(BattleQteTaskData &data) {
    DrawBattleQteIcons(data);
    DrawBattleQteDirections(data);
    if (data.shownFrames && !--data.shownFrames)
        data.shownMask = 0;
    if (data.directionFlashFrames)
        --data.directionFlashFrames;
    if (data.age < 100)
        ++data.age;
    if (data.directionFrames)
        --data.directionFrames;
    if (data.departureFrames && !data.shownFrames)
        --data.departureFrames;
}

#else

// @verified - p
void EnableQteUi(BattleQteTaskData &data) {
    if (data.departureFrames != 0 && data.shownFrames == 0) {
        if (data.icons[0].x > -80)
            data.icons[0].x -= 40;
        for (std::size_t i = 1; i < 5; ++i)
            if (data.icons[i].x < 720)
                data.icons[i].x += 40;
    }
    for (std::uint8_t i = 0; i < 5; ++i) {
        float alpha = 0.4f;
        if (data.iconFrames[i] != 0) {
            const float frames = static_cast<float>(data.iconFrames[i]);
            alpha = 1.0f;
            if (i != 0) {
                data.icons[i].scale = std::min(1.5f, frames * 0.5f) + 1.0f;
                alpha = std::min(1.0f, frames / 3.0f * 0.6f + 0.4f);
            } else
                data.icons[i].scale = data.resultIcon ? std::min(1.0f, frames / 3.0f) + 2.0f : 2.0f;
            --data.iconFrames[i];
        } else {
            if ((data.shownMask & battleQteIconMasks[i]) != 0 && (data.directionFlashFrames & 2U) == 0)
                alpha = 1.0f;
            if (i != 0) {
                data.icons[i].scale = 1.0f;
                data.iconFrames[i] = 0;
            }
        }
        SetBattleQteActionIcon(i, data.icons[i].scale, alpha);
    }
    for (std::uint8_t i = 0; i < 4; ++i) {
        float scale = 1.0f, alpha = 0.4f;
        if ((data.shownMask & battleQteDirectionMasks[i]) != 0) {
            const float amount = static_cast<float>(data.directionFrames) / 6.0f;
            scale = amount * 1.5f + 1.0f;
            alpha = amount * 0.6f + 0.4f;
        }
        SetBattleQteDirectionIcon(i, scale, alpha);
    }
    EnableElement(UIElement::GameQte);
    if (data.shownFrames != 0 && --data.shownFrames == 0)
        data.shownMask = 0;
    if (data.directionFlashFrames != 0)
        --data.directionFlashFrames;
    if (data.age < 100)
        ++data.age;
    if (data.directionFrames != 0)
        --data.directionFrames;
    if (data.departureFrames != 0 && data.shownFrames == 0)
        --data.departureFrames;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfqSetBattleQteIcons(BattleQteTaskData &data, std::uint8_t mode, std::uint16_t mask, std::uint16_t frames) {
    data.shownMask = mask;
    data.shownFrames = frames;
    for (std::int32_t i = 0; i < 5; ++i)
        if ((static_cast<std::int16_t>(battleQteIconMasks[i]) & mask) != 0) {
            if (i > 0)
                data.iconFrames[i] = 6;
            else {
                data.age = 0;
                data.directionFrames = 6;
            }
        }
    switch (mode) {
    case 0:
        data.departureFrames = 0;
        data.resultIcon = 0;
        break;
    case 1:
        data.departureFrames = 0;
        data.resultIcon = 1;
        break;
    case 2:
        data.departureFrames = 30;
        data.resultIcon = 1;
        break;
    case 3:
        data.departureFrames = 30;
        data.directionFlashFrames = data.shownFrames;
        data.resultIcon = 1;
        break;
    }
}

#else

// @verified - p
void vfqSetBattleQteIcons(BattleQteTaskData &data, std::uint8_t mode, std::uint16_t mask, std::uint16_t frames) noexcept {
    data.shownFrames = frames;
    data.shownMask = mask;
    for (std::size_t i = 0; i < 5; ++i)
        if ((mask & battleQteIconMasks[i]) != 0) {
            if (i == 0) {
                data.age = 0;
                data.directionFrames = 6;
            } else
                data.iconFrames[i] = 6;
        }
    if (mode == 0) {
        data.resultIcon = 0;
        data.departureFrames = 0;
    } else if (mode == 1) {
        data.resultIcon = 1;
        data.departureFrames = 0;
    } else if (mode == 2) {
        data.resultIcon = 1;
        data.departureFrames = 30;
    } else if (mode == 3) {
        data.resultIcon = 1;
        data.departureFrames = 30;
        data.directionFlashFrames = data.shownFrames;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void DrawBattleQteIcons(BattleQteTaskData &data) {
    std::uint8_t index;
    if (data.departureFrames && !data.shownFrames) {
        if (data.icons[0].x > -80)
            data.icons[0].x -= 40;
        for (index = 1; index < 5; ++index)
            if (data.icons[index].x < 720)
                data.icons[index].x += 40;
    }
    const std::uint16_t shown = static_cast<std::uint16_t>(data.shownMask);
    for (index = 0; index < 5; ++index) {
        float alpha = 0.4f, scale, depth;
        if (data.iconFrames[index]) {
            alpha = 1.0f;
            if (!index) {
                scale = data.iconFrames[index] * 0.3333333432674408f;
                if (scale > 1.0f)
                    scale = 1.0f;
                if (data.resultIcon)
                    scale += 2.0f;
                else
                    scale = 2.0f;
            } else {
                const float frames = data.iconFrames[index];
                scale = frames * 0.5f;
                if (scale > 1.5f)
                    scale = 1.5f;
                scale += 1.0f;
                alpha = data.iconFrames[index] / 3.0f * 0.6f + 0.4f;
                if (alpha > 1.0f)
                    alpha = 1.0f;
            }
            data.icons[index].scale = scale;
            --data.iconFrames[index];
            depth = scale + 2.0f;
        } else {
            if ((battleQteIconMasks[index] & shown) && !(data.directionFlashFrames & 2U))
                alpha = 1.0f;
            if (index) {
                data.icons[index].scale = 1.0f;
                data.iconFrames[index] = 0;
            }
            depth = 1.0f;
        }
        DrawBattleQteSprite(
            static_cast<std::uint8_t>(data.icons[index].kind), data.icons[index].x, data.icons[index].y, data.icons[index].scale, alpha, 0, 0, depth);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void DrawBattleQteDirections(BattleQteTaskData &data) {
    struct Direction {
        std::uint32_t mask;
        std::int32_t x, y, rotation;
    };
    static const Direction directions[4] = {{0x1000, 0, -1, 0}, {0x2000, 0, 1, 0x8000}, {0x4000, -1, 0, 0xc000}, {0x8000, 1, 0, 0x4000}};
    std::int32_t frames[3];
    float alpha[3];
    std::uint16_t baseX, baseY, dirX, dirY, shown;
    std::int32_t rotation, distance;
    std::uint8_t i, index;
    shown = static_cast<std::uint16_t>(data.shownMask);
    baseX = static_cast<std::uint16_t>(data.icons[0].x);
    baseY = static_cast<std::uint16_t>(data.icons[0].y);
    for (i = 0; i < 3; ++i) {
        frames[i] = static_cast<std::int32_t>(data.age) - i * 2;
        alpha[i] = 1.0f - static_cast<float>(i) * 0.25f;
        if (i)
            alpha[i] -= 0.25f;
        if (frames[i] < 0) {
            frames[i] = 0;
            alpha[i] = 0;
        } else if (frames[i] > 4) {
            alpha[i] -= static_cast<float>(frames[i] - 4) * 0.25f;
            frames[i] = 4;
        }
    }
    for (i = 0; i < 4; ++i) {
        if (!(shown & directions[i].mask))
            continue;
        distance = 70;
        dirX = static_cast<std::uint16_t>(directions[i].x);
        dirY = static_cast<std::uint16_t>(directions[i].y);
        rotation = directions[i].rotation;
        for (index = 0; index < 3; ++index) {
            if (data.directionFlashFrames & 2U)
                alpha[index] = 0;
            if (!(alpha[index] > 0.0f))
                continue;
            float scale = static_cast<float>(data.directionFrames + index * 2) * 0.6666666865348816f;
            if (scale > 2.0f)
                scale = 2.0f;
            scale += 1.0f;
            const std::uint16_t x = static_cast<std::uint16_t>(baseX + dirX * (frames[index] * 10 + distance));
            const std::uint16_t y = static_cast<std::uint16_t>(baseY + dirY * (frames[index] * 10 + distance));
            DrawBattleQteSprite(5, x, y, scale, alpha[index], rotation, 0, 3.0f);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void ResetBattleQteDirectionState() {
    bufferedInput = 0;
    firstInputFrame = true;
}

static std::uint32_t LatchBattleQtePadMask() {
    std::uint32_t pressed = g_inputBuffer[0].rawPressed;
    pressed &= 0xf606U;
    if (firstInputFrame) {
        if (pressed != 0 && pressed != 0xf606U) {
            bufferedInput = pressed;
            firstInputFrame = 0;
            pressed = 0;
        }
        return pressed;
    }
    if (pressed != 0)
        bufferedInput &= g_inputBuffer[0].held;
    pressed |= bufferedInput;
    bufferedInput = 0;
    firstInputFrame = 1;
    return pressed;
}

void SetBattleQteParameters(float average, std::uint8_t successes, std::uint8_t failures) {
    responseAverage = average;
    successCount = successes;
    failureCount = failures;
}

float GetBattleQteParameter(std::int32_t index) {
    switch (index) {
    case 0:
        return responseAverage;
    case 1:
        return static_cast<float>(successCount);
    case 2:
        return static_cast<float>(failureCount);
    default:
        return 0.0f;
    }
}

static std::uint16_t BattleQteIdentity(std::uint16_t value, std::uint8_t) {
    return value;
}

#endif

} // namespace s2
} // namespace shenmue
