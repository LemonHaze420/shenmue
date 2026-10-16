#include "s2/areas/area_game.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/event_characters.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/help.hpp"
#include "s2/inventory.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
constexpr shen_name taskToken{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, machine{SHEN_NAME(DGAM)}, arena{SHEN_NAME(QJBT)};
std::int32_t pendingAchievement, pendingPrize, moneyPhase;
constexpr Vector3 entrancePositions[]{{1.4660500288009644f, 0, -.7832599878311157f}, {1.2237999439239502f, 0, -.3384000062942505f},
    {.43540000915527344f, 0, 1.5322999954223633f}, {1.1699999570846558f, 0, 0}};
constexpr float entranceAngles[]{135, 105, 35, 90};
constexpr Vector3 playPositions[]{
    entrancePositions[0], {1.0985699892044067f, 0, .148389995098114f}, {.6909999847412109f, 0, 1.0199999809265137f}, {.8500000238418579f, 0, 0}};
constexpr float playAngles[]{180, 90, 0, 90};
constexpr Vector3 resumePositions[]{Vector3{}, {207.2449951171875f, 100.80000305175781f, 93.00499725341797f},
    {210.718994140625f, 100.80000305175781f, 94.73100280761719f}, {228.93499755859375f, 107.19999694824219f, 120.56999969482422f}};
constexpr float resumeAngles[]{0, 214.21099853515625f, 180, 325};
constexpr std::uint32_t enterMotions[]{0x2801, 0x2881, 0x2901, 0x2c81}, loopMotions[]{0x2803, 0x2883, 0x2901, 0x2c81},
    exitMotions[]{0x2802, 0x2882, 0x2904, 0x2c82};
constexpr std::uint8_t helpTopics[]{0x17, 0x16, 0x34, 0x35};
constexpr std::int16_t achievements[]{1739, 1738, 1740, 1741}, prizes[]{1743, 1742, 1744, 1745};
void Camera(std::int32_t id) {
    EventSetCamera request{0, id, machine, machine, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void Place(shen_name parent, const Vector3 &position) {
    EventSetPosition request{0, ryo, parent, position, 0};
    EVT_AT_SetPosRot(&request);
}
void Turn(shen_name parent, float degrees) {
    EventSetAngle request{0, ryo, parent, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void Voice(const char *token, int delay = 0) {
    EVT_AT_SetVoice(token, delay);
}
void RandomVoice(const char *first, const char *second, int delay = 0) {
    Voice(GetRandomNumberInRange(2) == 0 ? first : second, delay);
}
void RandomVoice(const char *first, const char *second, const char *third, int delay = 0) {
    const float value = GetRandomNumberInRange(3);
    Voice(value == 0 ? first : value == 1 ? second : third, delay);
}
void Help(std::uint32_t kind) {
    if (kind <= 3)
        EV_EventSetUpHelpIcon_0(helpTopics[kind], 0);
    else if (kind >= 10 && kind <= 13)
        EV_EventSetUpHelpIcon_0(helpTopics[kind - 10], 0);
}
void Gravity(bool enabled) {
    EV_SetCharaControllerFlags(ryo, 2, enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, 3, enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
}
void Film(int frames, int from, int to, float delay, bool queued) {
    EventFilmControl request{queued ? 1 : 0, frames, from, 0, 0, 0, to, 0, 0, 0, delay};
    if (!queued)
        EVT_AT_MenCtrl_Callback(&request);
    else {
        auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(request), taskToken);
        RegisterEventTask(task);
        *static_cast<EventFilmControl *>(HLib::GetTaskParameter(task)) = request;
    }
}
void Reject(GameWork *work, bool cameraAfterBurner) {
    if (!work->hasPlayed) {
        RandomVoice("0147A008", "0147A009");
        if (work->kind == 1 || work->kind == 2)
            Camera(3050 + work->kind);
        HLib::SetupTaskDelay(60);
    } else if (cameraAfterBurner && !work->kind)
        Camera(3050);
    work->state = 10;
}
} // namespace

void GameLocalDebugNullsub() {
}
void InitGame() {
    pendingAchievement = 0;
    pendingPrize = 0;
    moneyPhase = 0;
}
void QueueGame(std::uintptr_t argument) {
    auto *request = reinterpret_cast<GameRequest *>(argument);
    SetPlayerControlEnabled(0);
    if (request->kind == 10) {
        auto *task = HLib::EnqueueTaskWithParameter(GameWaitStreetFightCallback, 4, 0x0b, sizeof(GameRequest), taskToken);
        std::memcpy(HLib::GetTaskParameter(task), request, sizeof(GameRequest));
        SetFightModeFlags(GetFightModeFlags() | 7U);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
    } else {
        SetPlayerControlEnabled(0);
        auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), taskToken);
        RegisterEventTask(task);
        auto *clear = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
        clear->callback = GameEnqueueCallback;
        clear->callbackData = request;
        clear->reserved10 = 0;
        clear->state = 0;
        clear->delay = 3;
    }
}
void GameWaitStreetFightCallback(void *data) {
    GameLocalDebugNullsub();
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
        return;
    GameLocalDebugNullsub();
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    GameEnqueueCallback(data);
    HLib::DestroyCurrentTask();
}
void GameEnqueueCallback(void *data) {
    const auto *request = static_cast<const GameRequest *>(data);
    const Vector4 board{1, 1, 1, 1};
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(GameMainCallback, 5, 0x0b, sizeof(GameWork), taskToken);
    auto *work = static_cast<GameWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->kind = request->kind;
    work->cost = 5;
    work->hasPlayed = 0;
    // state/purchase/reserved1c are deliberately not initialized by this producer.
    pendingAchievement = 0;
    pendingPrize = 0;
    CreateEventCharacter(CharacterCreationHighModel, nullptr, machine);
    const Vector3i rotation{0, static_cast<std::uint16_t>(static_cast<std::int32_t>((request->yaw * 8192) / 45)), 0};
    EV_AT_SetPos(machine, EventTransformFlags::AllAxes, request->position);
    EV_AT_SetAngY(machine, EventTransformFlags::AllAxes, rotation);
    SetScenarioReflectorBoardValues(ryo, &board);
    EV_SetRefBoardParamsFor(ryo, 1);
    EV_SetNewSceneIndex();
    Gravity(false);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("GAME"), 0);
    EV_LoadCutsceneFromCurrentScene("0118E000.smp");
    EV_FreeAuthorPak(1, "0147");
}
void GameMainCallback(void *data) {
    auto *work = static_cast<GameWork *>(data);
    if (work->kind <= 3) {
        work->state = 0;
        HLib::SetCurrentTaskTo(GameAuthorCallback);
    } else if (work->kind >= 10 && work->kind <= 13) {
        work->hasPlayed = 1;
        work->state = 8;
        HLib::SetCurrentTaskTo(GameAuthorCallback);
    } else if (work->kind == 99)
        HLib::SetCurrentTaskTo(GameExitCallback);
    Help(work->kind);
    FinishLoading();
}
void GameAuthorCallback(void *data) {
    auto *work = static_cast<GameWork *>(data);
    int delay = 0;
    switch (work->state) {
    case 0:
        work->state = 1;
        delay = 8;
        break;
    case 1: {
        Film(30, 255, 0, 0, false);
        Vector3 target;
        EV_GetCharaPosition(machine, CHARACTER_TRANSLATION_ROOT, &target, EventTransformFlags::World);
        target.y += 1.2f;
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &target, 1);
        EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0x40000000, 0);
        Voice("0147A001", 10); // @text EN: "Five dollars a game=@" | JA: "１回５ドルか…"
        if (work->kind <= 3) {
            Place(machine, entrancePositions[work->kind]);
            Turn(machine, entranceAngles[work->kind]);
            Camera(3000 + work->kind);
            delay = 40;
        }
        work->state = 2;
        break;
    }
    case 2: {
        std::int16_t choices[]{-1, 36, 23, -1};
        SetupControlSpeakAndUI(choices, 600, 65535, 2);
        work->state = 3;
        break;
    }
    case 3:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return;
        if (shenGetCtrlSpeakIconFlag() != 36) {
            Reject(work, true);
            return;
        }
        if (GetPlayerStat(PlayerStatsId::Cash) >= work->cost) {
            work->state = 4;
            delay = 20;
            break;
        }
        AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash2);
        RandomVoice("0147A005", "0147A006", "0147A007");
        if (!work->hasPlayed && work->kind <= 2)
            Camera(3050 + work->kind);
        work->state = 10;
        delay = 60;
        break;
    case 4:
        if (!work->hasPlayed && work->kind <= 1)
            Camera(3010 + work->kind);
        Help(work->kind);
        FinishLoading();
        SetMoneyRelatedUIElement(45, 0, 5);
        work->state = 5;
        break;
    case 5:
        if (!moneyPhase) {
            if (GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
                ++moneyPhase;
            return;
        }
        if (moneyPhase == 1) {
            if (!GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
                ++moneyPhase;
            return;
        }
        if (moneyPhase != 2)
            return;
        moneyPhase = 0;
        work->purchase = static_cast<std::int32_t>(GetNumBet());
        if (work->purchase >= 1) {
            AddThisToCashOrSegaCoins(-work->purchase, ECashOrCoins::Cash2);
            work->state = 6;
            if (!work->hasPlayed)
                RandomVoice("0147A002", "0147A003", "0147A004", 20);
        } else {
            if (!work->hasPlayed) {
                RandomVoice("0147A008", "0147A009");
                delay = 60;
            }
            work->state = 10;
        }
        break;
    case 6:
        GameLocalDebugNullsub();
        spmFadeOut();
        if (work->hasPlayed) {
            RandomVoice("0147A014", "0147A015");
            Film(20, 0, 255, 60, true);
            delay = 80;
            work->state = 7;
            break;
        }
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
        if (work->kind <= 3) {
            SetCurrentGameFlag(static_cast<std::uint8_t>(3 - work->kind), 1);
            Camera(3020 + work->kind);
            EVT_AT_SetMot(ryo, enterMotions[work->kind], 0, 1, 0x40000000, 0);
            Place(machine, playPositions[work->kind]);
            Turn(machine, playAngles[work->kind]);
            Film(20, 0, 255, work->kind == 3 ? 220 : 240, true);
            delay = work->kind == 3 ? 240 : 260;
        }
        work->hasPlayed = 1;
        work->state = 7;
        break;
    case 7:
        SetArcadeEmulatorState(EmulatorState::SetEntryID, work->purchase / 5);
        switch (work->kind) {
        case 0:
            SetArcadeEmulatorState(EmulatorState::StartAfterBurner, 50);
            break;
        case 1:
            SetArcadeEmulatorState(EmulatorState::StartOutRun, 50);
            break;
        case 2:
            SetArcadeEmulatorState(EmulatorState::StartHarrier, 51);
            break;
        case 3:
            SetArcadeEmulatorState(EmulatorState::StartHangOn, 50);
            break;
        default:
            break;
        }
        return;
    case 8: {
        ST_LoadTitleFade();
        Film(20, 255, 0, 3, true);
        EV_spmBeginSampleExecute();
        const int credits = SetArcadeEmulatorState(EmulatorState::GetEntryID, 0);
        if (credits) {
            PATI_StartMoneyCount(5 * credits, 20);
            delay = 90;
        }
        if (work->kind >= 10)
            work->kind -= 10;
        if (work->kind <= 3) {
            if (!work->kind)
                EV_DisableCharacterDisplay(ryo);
            SetSceneInfos(g_actNumber, g_currentMap, work->kind ? 1 : 0);
            EVT_AT_SetMot(ryo, loopMotions[work->kind], work->kind >= 2 ? 194 : 0, work->kind >= 2 ? 0 : 1, 0x40000000, 0);
            if (work->kind) {
                Place(ryo, resumePositions[work->kind]);
                Turn(ryo, resumeAngles[work->kind]);
            }
            Camera(3030 + work->kind);
            if (SetArcadeEmulatorState(EmulatorState::GetScoreThresholdReached, 0))
                pendingAchievement = achievements[work->kind];
            if (SetArcadeEmulatorState(EmulatorState::GetCompletionFlag, 0) == 1 && !GetItemState(4244 + work->kind))
                pendingPrize = prizes[work->kind];
        }
        if (shenGetMinutesUntilNextFrameMinute()) {
            if (pendingAchievement || pendingPrize) {
                RandomVoice("0147A011", "0147A012", 10);
                work->state = 10;
            } else {
                RandomVoice("0147A011", "0147A013", 10);
                work->state = 2;
            }
            delay += 40;
        } else {
            if (work->hasPlayed) {
                if (!work->kind)
                    Camera(3040);
            } else {
                RandomVoice("0147A008", "0147A009");
                if (work->kind <= 2)
                    Camera(3050 + work->kind);
            }
            work->state = 10;
            delay = 60;
        }
        break;
    }
    case 10:
        GameLocalDebugNullsub();
        if (work->hasPlayed) {
            RandomVoice("0147A008", "0147A017");
            if (work->kind <= 3) {
                if (!work->kind)
                    EV_EnableCharacterDisplay(ryo);
                Camera(!work->kind ? 3045 : 3040 + work->kind);
                EVT_AT_SetMot(ryo, exitMotions[work->kind], 0, 1, 0x40000000, 0);
                if (work->kind == 3)
                    CreateClothControlTask();
                delay = !work->kind ? 200 : work->kind == 1 ? 230 : 129;
            }
        }
        work->state = 11;
        break;
    case 11:
        Film(30, 0, 255, 0, false);
        if (((pendingAchievement | pendingPrize) == 0 && g_currentMap.name != arena.name) || !work->hasPlayed)
            Film(16, 255, 0, 30, true);
        EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0x40000000, 30);
        switch (work->kind) {
        case 0:
            EVT_AT_SetPos(ryo, shen_name{0}, {268.89999f, 101.575f, 233.38f}, 30);
            EVT_AT_SetAngY(ryo, shen_name{0}, 150, 30);
            break;
        case 1:
            EVT_AT_SetPos(ryo, machine, playPositions[1], 30);
            EVT_AT_SetAngY(ryo, machine, 90, 30);
            break;
        case 2:
            EVT_AT_SetPos(ryo, machine, playPositions[2], 30);
            EVT_AT_SetAngY(ryo, machine, 60, 30);
            break;
        case 3:
            EVT_AT_SetPos(ryo, machine, entrancePositions[3], 30);
            EVT_AT_SetAngY(ryo, machine, 90, 30);
            break;
        default:
            break;
        }
        HLib::SetCurrentTaskTo(GameMainCallback);
        work->kind = 99;
        delay = 30;
        break;
    default:
        return;
    }
    if (delay)
        HLib::SetupTaskDelay(delay);
}
void GameExitCallback(void *data) {
    auto *work = static_cast<GameWork *>(data);
    SetScenarioCharacterKillMode(machine, 1);
    LoadCutsceneAndSoundProgramForArea();
    EV_EnableCharacterDisplay(ryo);
    EV_SetRefBoardParamsFor(ryo, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
    Gravity(true);
    EVS_LoopCamera(0, shen_name{}, shen_name{});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("GAME"), 0);
    if (!(pendingAchievement | pendingPrize)) {
        EV_NPC_NewLoadSw(1);
        SetPlayerControlEnabled(1);
    }
    if (pendingAchievement)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(pendingAchievement), 1);
    if (pendingPrize)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(pendingPrize), 1);
    if (!pendingAchievement && !pendingPrize && g_currentMap.name == arena.name)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
}
const ScriptAreaDefinition gameScriptDefinition{.areaId = SHEN_NAME(GAME), .field04 = 0, .init = InitGame, .main = QueueGame};
} // namespace shenmue::s2
