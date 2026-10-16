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

namespace shenmue::s2 {
namespace {
HLib::Task *battleQteTask{};
BattleQteCommand command = battleQteDefaultCommand;
std::uint8_t completedCommands{};
std::uint16_t activeState{}, bufferedInput{};
bool firstInputFrame{true};

BattleQteTaskData *GetBattleQteTaskData() {
    return battleQteTask ? static_cast<BattleQteTaskData *>(HLib::GetTaskParameter(battleQteTask)) : nullptr;
}

void EndBattleQtePause() {
    g_loadingFlag = (g_loadingFlag & ~0xc00U) | 0x800U;
}

} // namespace


void ResetBattleQteTaskPointer() noexcept {
    battleQteTask = nullptr;
}

// @verified - p
std::uint16_t vfqConvertCommandMask(std::int32_t value) noexcept {
    std::uint16_t result{};
    for (std::size_t i = 0; i < battleQteCommandMasks.size(); ++i)
        if ((static_cast<std::uint32_t>(value) & (1U << i)) != 0)
            result |= battleQteCommandMasks[i];
    return result;
}

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

bool vfqIsBattleQteActive() noexcept {
    return static_cast<std::uint8_t>(activeState) != 0;
}
void fbqtToggleMode() {
    GetBattleQteTaskData()->mode |= 0x80000000U;
}
BattleQteTaskData *fbqtDisableMode() {
    BattleQteTaskData *const data = GetBattleQteTaskData();
    data->mode |= 0x40000000U;
    return data;
}

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

} // namespace shenmue::s2
