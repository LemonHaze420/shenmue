#include "s2/areas/area_slot.hpp"
#include "s2/areas/slot_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2 {

// @verified - p
void SlotInitializationNullsub() {
}
// @verified - p
void DebugFileLogNullsub(const char *, int, const char *, ...) {
}
// @verified - p
void DebugScreenLogNullsub(int, int, const char *, ...) {
}
namespace {
constexpr shen_name slot{SHEN_NAME(SLOT)}, ryo{SHEN_NAME(RYO_)}, drum{SHEN_NAME(DRUM)}, playerModel{SHEN_NAME(SLTT)}, invalid{0xffffffffU};
constexpr shen_name lampNames[]{SHEN_NAME(LMP0), SHEN_NAME(LMP1), SHEN_NAME(LMP2), SHEN_NAME(LMP3)};
constexpr shen_name eventToken{SHEN_NAME(EVNT)};
void Main(void *);
void Player(void *);
void Machine(void *);
void Drum(void *);
void Cylinder(void *);
void ReelLeft(void *);
void ReelMiddle(void *);
void ReelRight(void *);
void LampTop(void *);
void LampMiddle(void *);
void LampBottom(void *);
void AuthorStart(void *);
template <class T> T *Work(HLib::Task *task) {
    return static_cast<T *>(HLib::GetTaskParameter(task));
}
template <class T> T *Child(HLib::TaskCallback callback, HLib::Task *&task) {
    task = HLib::EnqueueTaskWithParameter(callback, 4, 0x0b, sizeof(T), eventToken);
    return Work<T>(task);
}
void ClearSubstate(std::int8_t &state, std::int8_t &phase, std::int8_t value) {
    state = value;
    phase = 0;
}
void Tick(std::int16_t &timer) {
    if (timer > 0)
        --timer;
}
void Gravity(bool enabled) {
    EV_SetCharaControllerFlags(ryo, 2, enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, 3, enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
}
// @verified - p
void Start(SlotRootWork *work) {
    auto *player = Child<SlotPlayerWork>(Player, work->player);
    player->self = player;
    player->task = work->player;
    player->root = work->task;
    player->timer = 0;
    ClearSubstate(player->state, player->phase, 1);
    player->authorState = 0;
    player->rootWork = work;
    auto *machine = Child<SlotMachineWork>(Machine, work->machine);
    machine->self = machine;
    machine->task = work->machine;
    machine->root = work->task;
    machine->timer = 0;
    ClearSubstate(machine->state, machine->phase, 1);
    auto *reels = Child<SlotDrumWork>(Drum, work->drum);
    reels->self = reels;
    reels->task = work->drum;
    reels->root = work->task;
    reels->timer = 0;
    ClearSubstate(reels->state, reels->phase, 1);
    reels->machineIndex = work->machineIndex;
    auto *cylinder = Child<SlotCylinderWork>(Cylinder, work->cylinder);
    cylinder->self = cylinder;
    cylinder->task = work->cylinder;
    cylinder->root = work->task;
    cylinder->timer = 0;
    ClearSubstate(cylinder->state, cylinder->phase, 1);
    EV_LoadCutsceneFromCurrentScene("01120000");
    work->state = 2;
    work->choice = 0;
}
// @verified - p
void Reset(SlotRootWork *work) {
    auto *player = Work<SlotPlayerWork>(work->player);
    auto *machine = Work<SlotMachineWork>(work->machine);
    auto *reels = Work<SlotDrumWork>(work->drum);
    auto *cylinder = Work<SlotCylinderWork>(work->cylinder);
    ClearSubstate(player->state, player->phase, 4);
    ClearSubstate(machine->state, machine->phase, 2);
    ClearSubstate(reels->state, reels->phase, 2);
    ClearSubstate(cylinder->state, cylinder->phase, 2);
    work->state = 2;
    work->choice = 0;
}
// @verified - p
void Exit(SlotRootWork *work) {
    SetScenarioCharacterKillMode(playerModel, 1);
    auto *machine = Work<SlotMachineWork>(work->machine);
    for (int i = 0; i < 5; ++i)
        EV_scenApplyPosOrRotToPartOf(slot, 182 + i, &machine->lamps[i], nullptr, EventCharacterPartTransformMode::Absolute);
    EV_ToggleCharacterFlag(drum);
    for (shen_name name : lampNames)
        EV_ToggleCharacterFlag(name);
    EV_DisableCharacterDisplay(drum);
    for (shen_name name : lampNames)
        EV_DisableCharacterDisplay(name);
    EV_DisableCharacterDisplay(slot);
    EV_EnableCharacterDisplay(slotAuthors[work->machineIndex]);
    AT_SetLocalBaseChara(ryo, invalid);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, work->playerPosition);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, work->playerRotation);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    EVS_LoopCamera(0, shen_name{}, shen_name{});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_EnableCharacterDisplay(ryo);
    SetPlayerControlEnabled(1);
    if (g_currentMap.name == slot.name) {
        scenSetTaskFlags(6);
        HLib::DestroyTask(work->task);
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SLOT"), 0);
}
// @verified - p
void Main(void *data) {
    auto *work = static_cast<SlotRootWork *>(data);
    if (work->debug && work->player) {
        SlotPlayerWork *player = Work<SlotPlayerWork>(work->player);
        SlotMachineWork *machine = Work<SlotMachineWork>(work->machine);
        SlotDrumWork *reels = Work<SlotDrumWork>(work->drum);
        SlotCylinderWork *cylinder = Work<SlotCylinderWork>(work->cylinder);
        std::uintptr_t reelTasks[3], lampTasks[3];
        for (int i = 0; i < 3; ++i) {
            std::memcpy(&reelTasks[i], reels->reels[i]->reserved08, sizeof(reelTasks[i]));
            std::memcpy(&lampTasks[i], cylinder->lamps[i]->reserved08, sizeof(lampTasks[i]));
        }
        DebugScreenLogNullsub(2, 5, "(%08x) MAIN %2d %2d %4d", work->task, work->state, work->choice, work->timer);
        DebugScreenLogNullsub(2, 6, "(%08x) PLAY %2d %2d %4d", player->task, player->state, player->phase, player->timer);
        DebugScreenLogNullsub(2, 7, "(%08x) SLOT %2d %2d %4d", machine->task, machine->state, machine->phase, machine->timer);
        DebugScreenLogNullsub(2, 8, "(%08x) DRUM %2d %2d %4d", reels->task, reels->state, reels->phase, reels->timer);
        DebugScreenLogNullsub(2, 9, "(%08x) REE1 %2d %2d %4d", reelTasks[0], reels->reels[0]->state, reels->reels[0]->phase, reels->reels[0]->timer);
        DebugScreenLogNullsub(2, 10, "(%08x) REE2 %2d %2d %4d", reelTasks[1], reels->reels[1]->state, reels->reels[1]->phase, reels->reels[1]->timer);
        DebugScreenLogNullsub(2, 11, "(%08x) REE3 %2d %2d %4d", reelTasks[2], reels->reels[2]->state, reels->reels[2]->phase, reels->reels[2]->timer);
        DebugScreenLogNullsub(2, 12, "(%08x) CYLN %2d %2d %4d", cylinder->task, cylinder->state, cylinder->phase, cylinder->timer);
        DebugScreenLogNullsub(2, 13, "(%08x) LMP1 %2d %2d %4d", lampTasks[0], cylinder->lamps[0]->state, cylinder->lamps[0]->phase, cylinder->lamps[0]->timer);
        DebugScreenLogNullsub(2, 14, "(%08x) LMP2 %2d %2d %4d", lampTasks[1], cylinder->lamps[1]->state, cylinder->lamps[1]->phase, cylinder->lamps[1]->timer);
        DebugScreenLogNullsub(2, 15, "(%08x) LMP3 %2d %2d %4d", lampTasks[2], cylinder->lamps[2]->state, cylinder->lamps[2]->phase, cylinder->lamps[2]->timer);
    }
    if (EVT_GetControllerInput(0, 1) & 0x40U)
        work->debug = static_cast<std::int8_t>(~work->debug);
    Tick(work->timer);
    switch (work->state) {
    case 1:
        Start(work);
        break;
    case 2:
        if (!work->choice) {
            work->state = 3;
            work->choice = 0;
        }
        break;
    case 3: {
        Work<SlotPlayerWork>(work->player);
        auto *machine = Work<SlotMachineWork>(work->machine);
        if (work->choice == 1) {
            ClearSubstate(machine->state, machine->phase, 3);
            work->choice = 0;
        } else if (!work->choice && machine->state == 2)
            work->choice = 1;
        break;
    }
    case 4:
        Reset(work);
        break;
    case 5:
        Exit(work);
        break;
    default:
        break;
    }
}
// @verified - p
void InitializeSlotPlayer(SlotPlayerWork *work) {
    if (!work->phase) {
        EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0, 0);
        GetMotionFrameCount(0xf003);
        EV_DisableCharacterDisplay(ryo);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        SetPlayerControlEnabled(0);
        work->timer = 0;
        ClearSubstate(work->state, work->phase, 4);
    }
}
// @verified - p
void Player(void *data) {
    auto *work = static_cast<SlotPlayerWork *>(data);
    Tick(work->timer);
    switch (work->state) {
    case 1:
        InitializeSlotPlayer(work);
        break;
    case 3:
        HLib::SetCurrentTaskTo(AuthorStart);
        break;
    case 4:
        work->state = 4;
        break;
    default:
        break;
    }
}
// @verified - p
void InitializeSlotMachine(SlotMachineWork *work) {
    if (!work->phase) {
        work->timer = 0;
        ++work->phase;
    } else if (work->phase == 1)
        ++work->phase;
    else if (work->phase == 2) {
        for (int i = 0; i < 5; ++i)
            EV_scenApplyPosOrRotToPartOf(slot, 182 + i, &work->lamps[i], nullptr, EventCharacterPartTransformMode::Get);
        EV_EnableCharacterDisplay(slot);
        ClearSubstate(work->state, work->phase, 2);
    }
}
// @verified - p
void LowerSlotLamps(SlotMachineWork *work) {
    for (int i = 0; i < 5; ++i) {
        Vector3 position = work->lamps[i];
        position.x += 0.0f;
        position.y += 0.0f;
        position.z += -0.0099999998f;
        EV_scenApplyPosOrRotToPartOf(slot, 182 + i, &position, nullptr, EventCharacterPartTransformMode::Absolute);
    }
}
// @verified - p
void AssignSlotWagerLamp(SlotMachineWork *work) {
    SlotRootWork *root = Work<SlotRootWork>(work->root);
    SlotDrumWork *reels = Work<SlotDrumWork>(root->drum);
    const int lamp = reels->wager / slotBet[reels->machineIndex - 1];
    if (lamp >= 0 && lamp < 5)
        EV_scenApplyPosOrRotToPartOf(slot, 182 + lamp, &work->lamps[lamp], nullptr, EventCharacterPartTransformMode::Absolute);
}
// @verified - p
void ReadSlotWagerInput(SlotMachineWork *work) {
    SlotRootWork *root = Work<SlotRootWork>(work->root);
    SlotDrumWork *reels = Work<SlotDrumWork>(root->drum);
    SlotPlayerWork *player = Work<SlotPlayerWork>(root->player);
    if (!work->phase) {
        EVS_LoopCamera(7109, slot, slot);
        LowerSlotLamps(work);
        reels->wager = 0;
        ++work->phase;
    } else if (work->phase == 1) {
        const std::uint32_t input = EVT_GetControllerInput(0, 1);
        if ((input & 0x400U) && reels->wager) {
            player->coins = reels->credits;
            Loop3DSoundEffect(0x142);
            EVT_AT_SetSound(0x141, 10);
            reels->payout = 0;
            ClearSubstate(work->state, work->phase, 4);
        } else if (input & 0x8000U) {
            const int bet = slotBet[reels->machineIndex - 1];
            if (reels->credits >= bet && reels->wager < 5 * bet) {
                Loop3DSoundEffect(0x13b);
                EVT_AT_SetSound(0x142, 10);
                AssignSlotWagerLamp(work);
                reels->credits -= bet;
                reels->wager += bet;
            }
        } else if ((input & 0x4000U) && reels->credits < 200)
            reels->credits += 20;
        else if (input & 0x200U) {
            root->state = 5;
            root->choice = 0;
        }
    }
}
// @verified - p
void Machine(void *data) {
    auto *work = static_cast<SlotMachineWork *>(data);
    Tick(work->timer);
    switch (work->state) {
    case 1:
        InitializeSlotMachine(work);
        break;
    case 2:
        work->state = 2;
        break;
    case 3:
        ReadSlotWagerInput(work);
        break;
    case 4: {
        SlotRootWork *root = Work<SlotRootWork>(work->root);
        SlotDrumWork *reels = Work<SlotDrumWork>(root->drum);
        if (!work->phase) {
            ClearSubstate(reels->state, reels->phase, 3);
            ++work->phase;
        } else if (work->phase == 1 && reels->state == 2)
            ClearSubstate(work->state, work->phase, 5);
        break;
    }
    case 5:
        if (!work->phase)
            ClearSubstate(work->state, work->phase, 3);
        break;
    default:
        break;
    }
}
int Symbol(const std::int8_t *strip, int reel, int stop, int delta) {
    if (delta < 0 && stop <= 0)
        return 0;
    if (delta > 0 && stop + 1 >= 22)
        return 5;
    return strip[22 * reel + stop + delta];
}
// @verified - p
std::uint16_t Payline(std::int16_t line, int first, int second, int third, SlotDrumWork *work) {
    std::uint16_t payout = 0;
    const int bet = slotBet[work->machineIndex - 1];
    for (int i = 0; i < 17; ++i) {
        if (first != slotPatterns[3 * i])
            continue;
        if (first == 1)
            payout = static_cast<std::uint16_t>(2 * bet);
        if (second != slotPatterns[3 * i + 1])
            continue;
        if (second == 1)
            payout = static_cast<std::uint16_t>(5 * bet);
        if (third != slotPatterns[3 * i + 2])
            continue;
        if (!first && !second && !third)
            payout = 0;
        else if (first == 5 && second == 5 && third == 5) {
            constexpr int jackpots[]{300, 500, 1000, 1500, 3000};
            if (line >= 1 && line <= 5)
                payout = static_cast<std::uint16_t>(jackpots[line - 1] * bet);
        } else
            payout = static_cast<std::uint16_t>(slotPay[i] * bet);
    }
    return payout;
}
// @verified - p
void Spin(SlotDrumWork *work) {
    if (!work->phase) {
        EVS_LoopCamera(7112, slot, slot);
        Loop3DSoundEffect(0x13e);
        if (!work->repeatOutcome) {
            work->outcome = 0;
            float random = GetRandomNumberInRange(100);
            const auto *odds = &slotOdds[6 * slotMachines[work->machineIndex - 1]];
            random -= odds[0];
            while (random > 0)
                random -= odds[++work->outcome];
        }
        const int random[3]{static_cast<std::int32_t>(GetRandomNumberInRange(66)), static_cast<std::int32_t>(GetRandomNumberInRange(66)),
            static_cast<std::int32_t>(GetRandomNumberInRange(66))};
        for (int i = 0; i < 3; ++i) {
            auto *reel = work->reels[i];
            ClearSubstate(reel->state, reel->phase, 3);
            reel->stop = slotStops[198 * work->outcome + 66 * i + static_cast<std::int16_t>(random[i])];
            reel->timer = 60 + 30 * i;
        }
        ++work->phase;
    } else if (work->phase == 1 && work->reels[0]->state == 4 && work->reels[1]->state == 4 && work->reels[2]->state == 4) {
        ClearSubstate(work->state, work->phase, 4);
        work->repeatOutcome = static_cast<std::int8_t>(GetRandomNumberInRange(2));
    }
}
// @verified - p
void Evaluate(SlotDrumWork *work) {
    auto *root = Work<SlotRootWork>(work->root);
    auto *cylinder = Work<SlotCylinderWork>(root->cylinder);
    auto *player = Work<SlotPlayerWork>(root->player);
    if (work->phase) {
        if (work->phase == 1 && cylinder->state == 2)
            ClearSubstate(work->state, work->phase, 2);
        return;
    }
    Loop3DSoundEffect(0x13f);
    std::int16_t total = 0;
    constexpr int offsets[5][3]{{0, 0, 0}, {-1, -1, -1}, {1, 1, 1}, {-1, 0, 1}, {1, 0, -1}};

    for (std::int16_t line = 0; line < work->wager + 1; ++line)
        if (line >= 1 && line <= 5)
            total = static_cast<std::int16_t>(total +
                Payline(line, Symbol(slotStrip, 0, work->reels[0]->stop, offsets[line - 1][0]),
                    Symbol(slotStrip, 1, work->reels[1]->stop, offsets[line - 1][1]), Symbol(slotStrip, 2, work->reels[2]->stop, offsets[line - 1][2]), work));
    if (total > 0) {
        work->credits += total;
        player->coins = work->credits;
        work->payout = total;
        if (total < 200)
            Loop3DSoundEffect(0x13c);
        else
            cylinder->state = 3;
    }
    ++work->phase;
}
// @verified - p
void InitializeSlotReels(SlotDrumWork *work) {
    if (!work->phase) {
        work->timer = 0;
        EV_EnsureCharacterDraw(drum);
        EV_EnableCharacterDisplay(drum);
        ++work->phase;
    } else if (work->phase == 1) {
        AT_SetLocalBaseChara(drum, slot);
        EV_AT_SetPos(drum, EventTransformFlags::AllAxes, Vector3{});
        EV_AT_SetAngY(drum, EventTransformFlags::AllAxes, Vector3i{});
        EV_EnableCharacterDisplay(drum);
        constexpr HLib::TaskCallback callbacks[]{ReelLeft, ReelMiddle, ReelRight};
        for (int i = 0; i < 3; ++i) {
            HLib::Task *task;
            SlotReelWork *const reel = Child<SlotReelWork>(callbacks[i], task);
            work->reels[i] = reel;
            reel->self = reel;
            reel->timer = 0;
            ClearSubstate(reel->state, reel->phase, 1);
        }
        work->repeatOutcome = 0;
        work->payout = 0;
        work->credits = 0;
        work->wager = 0;
        ClearSubstate(work->state, work->phase, 2);
    }
}
// @verified - p
void Drum(void *data) {
    auto *work = static_cast<SlotDrumWork *>(data);
    Tick(work->timer);
    switch (work->state) {
    case 1:
        InitializeSlotReels(work);
        break;
    case 2:
        work->state = 2;
        for (auto *reel : work->reels)
            reel->state = 2;
        break;
    case 3:
        Spin(work);
        break;
    case 4:
        Evaluate(work);
        break;
    default:
        break;
    }
    const Vector3 step{-0.02f, 0, 0}, creditPosition{0.27599999f, 1.001f, 0.28099999f}, payoutPosition{0.46599999f, 1.001f, 0.28099999f},
        wagerPosition{0.366f, 1.001f, 0.28099999f};
    scenCopyCharaToGame(slot);
    SetPosRotForGame(slot, 256, &step, 999999999, 0);
    UpdateGameNodeFlags(slot, work->credits, &creditPosition, 1, nullptr);
    SetPosRotForGame(slot, 256, &step, 99999, 0);
    UpdateGameNodeFlags(slot, work->payout, &payoutPosition, 1, nullptr);
    SetPosRotForGame(slot, 292, &step, 999, 0);
    UpdateGameNodeFlags(slot, work->wager, &wagerPosition, 1, nullptr);
}
// @verified - p
void AdvanceSlotReel(SlotReelWork *work) {
    const int next = work->angle + work->speed;
    work->angle = next < 65516 ? next : 0;
    if (!work->phase) {
        if (work->timer <= 0)
            work->phase = 1;
    } else if (work->phase == 1 && work->angle == (work->stop ? 2978 * (22 - work->stop) : 0)) {
        Loop3DSoundEffect(0x140);
        ClearSubstate(work->state, work->phase, 4);
    }
}
void Reel(SlotReelWork *work, std::uint16_t part) {
    Tick(work->timer);
    if (work->state == 1 && !work->phase) {
        work->speed = 2978;
        work->timer = 0;
        work->stop = 0;
        work->angle = 0;
        ClearSubstate(work->state, work->phase, 2);
    } else if (work->state == 2)
        work->state = 2;
    else if (work->state == 3)
        AdvanceSlotReel(work);
    Vector3i angle{work->angle, 0, 0};
    EV_scenApplyPosOrRotToPartOf(drum, part, nullptr, &angle, EventCharacterPartTransformMode::Absolute);
}
// @verified - p
void ReelLeft(void *data) {
    Reel(static_cast<SlotReelWork *>(data), 153);
}
// @verified - p
void ReelMiddle(void *data) {
    Reel(static_cast<SlotReelWork *>(data), 154);
}
// @verified - p
void ReelRight(void *data) {
    Reel(static_cast<SlotReelWork *>(data), 155);
}
// @verified - p
void InitializeSlotCylinder(SlotCylinderWork *work) {
    if (!work->phase) {
        work->timer = 0;
        for (shen_name name : lampNames)
            EV_EnsureCharacterDraw(name);
        for (shen_name name : lampNames)
            EV_EnableCharacterDisplay(name);
        ++work->phase;
    } else if (work->phase == 1) {
        constexpr HLib::TaskCallback callbacks[]{LampTop, LampMiddle, LampBottom};
        constexpr int speeds[]{8934, 2978, 5956};
        constexpr float heights[]{1.72f, 1.64f, 1.5599999f};
        for (int i = 0; i < 3; ++i) {
            const shen_name name = lampNames[i + 1];
            AT_SetLocalBaseChara(name, slot);
            EV_AT_SetPos(name, EventTransformFlags::AllAxes, Vector3{0.30000001f, heights[i], 0.138f});
            EV_AT_SetAngY(name, EventTransformFlags::AllAxes, Vector3i{});
            EV_EnableCharacterDisplay(name);
            HLib::Task *task;
            SlotLampWork *const lamp = Child<SlotLampWork>(callbacks[i], task);
            work->lamps[i] = lamp;
            lamp->self = lamp;
            lamp->speed = speeds[i];
            lamp->timer = 0;
            ClearSubstate(lamp->state, lamp->phase, 1);
        }
        ClearSubstate(work->state, work->phase, 2);
    }
}
// @verified - p
void CelebrateSlotPayout(SlotCylinderWork *work) {
    if (!work->phase) {
        Loop3DSoundEffect(0x13d);
        EVS_LoopCamera(7113, slot, slot);
        for (SlotLampWork *lamp : work->lamps) {
            ClearSubstate(lamp->state, lamp->phase, 3);
            lamp->timer = 180;
        }
        ++work->phase;
    } else if (work->phase == 1 && work->lamps[0]->state == 4 && work->lamps[1]->state == 4 && work->lamps[2]->state == 4)
        ClearSubstate(work->state, work->phase, 4);
}
// @verified - p
void Cylinder(void *data) {
    auto *work = static_cast<SlotCylinderWork *>(data);
    Tick(work->timer);
    switch (work->state) {
    case 1:
        InitializeSlotCylinder(work);
        break;
    case 2:
        work->state = 2;
        for (SlotLampWork *lamp : work->lamps)
            lamp->state = 2;
        break;
    case 3:
        CelebrateSlotPayout(work);
        break;
    case 4:
        if (!work->phase)
            ClearSubstate(work->state, work->phase, 2);
        break;
    default:
        break;
    }
}
void Lamp(SlotLampWork *work, shen_name name) {
    Tick(work->timer);
    if (work->state == 1 && !work->phase) {
        work->timer = 0;
        ClearSubstate(work->state, work->phase, 2);
        work->angle = 0;
    } else if (work->state == 2)
        work->state = 2;
    else if (work->state == 3) {
        const int next = work->angle + work->speed;
        work->angle = next < 65516 ? next : 0;
        if (!work->phase && work->timer <= 0)
            ClearSubstate(work->state, work->phase, 4);
    }
    EV_AT_SetAngY(name, EventTransformFlags::AllAxes, Vector3i{0, work->angle, 0});
}
// @verified - p
void LampTop(void *data) {
    Lamp(static_cast<SlotLampWork *>(data), lampNames[1]);
}
// @verified - p
void LampMiddle(void *data) {
    Lamp(static_cast<SlotLampWork *>(data), lampNames[2]);
}
// @verified - p
void LampBottom(void *data) {
    Lamp(static_cast<SlotLampWork *>(data), lampNames[3]);
}
// @verified - p
void SerifEnd(void *) {
    ctrlStopSubtitles();
    HLib::DestroyCurrentTask();
}
// @verified - p
void Serif(const char *text) {
    auto *task = HLib::EnqueueTaskWithoutParameter(SerifEnd, 4, 0x0b, eventToken.name);
    dbgPrintEventMessage(text);
    HLib::SetupTimedTask(task, 40);
}
// @verified - p
int Explain(SlotPlayerWork *work) {
    work->coins = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::SegaCoins));
    SlotRootWork *root = Work<SlotRootWork>(work->root);
    const int bet = slotBet[root->machineIndex - 1];
    if (work->coins < bet)
        Serif("４ドルで２０枚か");
    else
        switch (bet) {
        case 1:
            Serif("この台は１ベットか。");
            break;
        case 5:
            Serif("この台は５ベットか。");
            break;
        case 10:
            Serif("この台は１０ベットか。");
            break;
        case 20:
            Serif("この台は２０ベットか。");
            break;
        default:
            break;
        }
    DebugFileLogNullsub("SLOT\\eSLOT_et_slot_sa.c", 186, "Medal   >>>> %d\n", work->coins);
    DebugFileLogNullsub("SLOT\\eSLOT_et_slot_sa.c", 187, "SlotNum >>>> %d\n", root->machineIndex);
    DebugFileLogNullsub("SLOT\\eSLOT_et_slot_sa.c", 188, "Bet     >>>> %d\n", bet);
    return 45;
}
// @verified - p
int Choose(SlotPlayerWork *work) {
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return 0;
    const bool play = shenGetCtrlSpeakIconFlag() == 36;
    const std::uint8_t random = static_cast<std::uint8_t>(static_cast<std::int64_t>(GetRandomNumberInRange(3)));
    if (play) {
        Serif(random == 0 ? "やってみるか。" : random == 1 ? "遊んでみるか。" : "少し遊ぶか。");
        work->authorState = 6;
        return 20;
    }
    Serif(random == 0 ? "やめとくか。" : random == 1 ? "いや、やめとこう。" : "今はやめとくか。");
    work->authorState = 9;
    return 45;
}
// @verified - p
void AuthorStart(void *data) {
    auto *work = static_cast<SlotPlayerWork *>(data);
    auto *root = work->rootWork;
    auto *reels = Work<SlotDrumWork>(root->drum);
    int delay = 0;
    switch (work->authorState) {
    case 0: {
        Gravity(false);
        HLib::SetupTimedTask(root->machine, 0);
        HLib::SetupTimedTask(root->drum, 0);
        HLib::SetupTimedTask(root->cylinder, 0);
        AT_SetLocalBaseChara(playerModel, slot);
        EV_AT_SetPos(playerModel, EventTransformFlags::AllAxes, Vector3{0.30000001f, 0, 0.80000001f});
        EV_AT_SetAngY(playerModel, EventTransformFlags::AllAxes, Vector3i{});
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(playerModel, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(playerModel, &rotation, EventTransformFlags::World);
        CopyMotionTransition(scenFindMotionwork(ryo), &position, rotation.y, 0, 0);
        work->authorState = 1;
        break;
    }
    case 1:
        if (!mtwkSetExtraFlag(scenFindMotionwork(ryo), 58, 1))
            work->authorState = 2;
        break;
    case 2:
        delay = Explain(work);
        work->authorState = 3;
        break;
    case 3: {
        std::int16_t choices[]{-1, 36, 23, -1};
        SetupControlSpeakAndUI(choices, 200, 0, 1);
        work->authorState = 4;
        break;
    }
    case 4:
        delay = Choose(work);
        break;
    case 6:
        reels = Work<SlotDrumWork>(root->drum);
        if (work->coins >= slotBet[root->machineIndex - 1]) {
            reels->credits = work->coins;
            work->authorState = 8;
        } else {
            SetMoneyRelatedUIElement(100, 0, 4);
            work->authorState = 7;
        }
        break;
    case 7:
        if (!GetShouldRenderIcons(RenderIconSlot::MoneyMessage)) {
            const int cash = static_cast<std::int32_t>(GetNumBet());
            if (cash) {
                reels->credits = 5 * cash;
                work->coins = 5 * cash;
                AddThisToCashOrSegaCoins(5 * cash, ECashOrCoins::Coins);
                PATI_StartMoneyCount(-cash, 130);
                work->authorState = 8;
            } else
                work->authorState = 9;
        }
        break;
    case 8:
        HLib::ResolveDeferredCallback(root->machine);
        HLib::ResolveDeferredCallback(root->drum);
        HLib::ResolveDeferredCallback(root->cylinder);
        EV_DisableCharacterDisplay(ryo);
        work->state = 4;
        HLib::SetCurrentTaskTo(Player);
        break;
    case 9:
        root->state = 5;
        Gravity(true);
        HLib::ResolveDeferredCallback(root->machine);
        HLib::ResolveDeferredCallback(root->drum);
        HLib::ResolveDeferredCallback(root->cylinder);
        HLib::SetCurrentTaskTo(Player);
        break;
    default:
        break;
    }
    if (delay)
        HLib::SetupTaskDelay(delay);
    DebugFileLogNullsub("SLOT\\eSLOT_et_slot_sa.c", 151, "Mode  >>>> %d\n", work->authorState);
}
} // namespace
// @verified - p
void QueueSlot(std::uintptr_t argument) {
    auto *task = HLib::EnqueueTaskWithParameter(Main, 4, 0x0b, sizeof(SlotRootWork), eventToken);
    auto *work = Work<SlotRootWork>(task);
    work->self = work;
    work->task = task;
    work->player = nullptr;
    work->machine = nullptr;
    work->drum = nullptr;
    work->cylinder = nullptr;
    work->timer = 0;
    work->choice = 0;
    work->debug = 0;
    work->state = 1;
    work->machineIndex = *reinterpret_cast<const std::int32_t *>(argument);
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &work->playerPosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ryo, &work->playerRotation, EventTransformFlags::World);
    EV_DisableCharacterDisplay(slotAuthors[work->machineIndex]);
    EV_EnableCharacterDisplay(slot);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(slotAuthors[work->machineIndex], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(slotAuthors[work->machineIndex], &rotation, EventTransformFlags::World);
    EV_AT_SetPos(slot, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(slot, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SLOT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, playerModel);
}
const ScriptAreaDefinition slotScriptDefinition{.areaId = SHEN_NAME(SLOT), .field04 = 0, .init = SlotInitializationNullsub, .main = QueueSlot};
} // namespace shenmue::s2
