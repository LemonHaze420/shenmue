#include "s2/areas/etrk_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/math.hpp"
#include "s2/map_control.hpp"
namespace shenmue::s2::areas {
// @verified - p
void InitializeEtrk() {
}
// @verified - p
void RunEtrk(std::uintptr_t entry) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EtrkMain, 4, 0x0b, sizeof(EtrkWork), ETRK_TASK);
    EtrkWork *const work = static_cast<EtrkWork *>(HLib::GetTaskParameter(task));
    std::memset(work, 0, sizeof(*work));
    work->task = task;
    work->keys = etrkKeys;
    work->switches = etrkSwitches;
    work->entry = static_cast<std::int32_t>(entry);
    work->selection = 4;
    if (g_currentMap.name == ETRK_QF00.name) {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1909)) {
            work->key = 3;
            work->state = 4;
        }
    } else if (g_currentMap.name == ETRK_QUG0.name) {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1910)) {
            work->key = 4;
            work->state = 4;
        }
    }
    EtrkSetUp(*work);
}
// @verified - p
void EtrkSetUp(EtrkWork &work) {
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(ETRK_RYO, 1, static_cast<NpcCharacterControllerState>(0));
    EV_SetCharaControllerFlags(ETRK_RYO, 3, static_cast<NpcCharacterControllerState>(0));
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_SetNewSceneIndex();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ETRK"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ETRK"), 0);
    SetLowQualityHands(FindCharacterByScenarioName(ETRK_RYO));
    SetHighModelXYZ(FindCharacterByScenarioName(ETRK_RYO), 1, 0, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ETRK_RYO), 0, reinterpret_cast<std::byte *>(etrkRelaxedHand), 0);
    SetLowQualityHands(FindCharacterByScenarioName(ETRK_HAND));
    SetHighModelXYZ(FindCharacterByScenarioName(ETRK_HAND), 1, 0, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ETRK_HAND), 0, reinterpret_cast<std::byte *>(etrkRelaxedHand), 0);
    ExecuteCleanupFuncForAreaWithCachedScene();
    EV_FreeAuthorPak(1, "0304");
    if (g_currentMap.name == ETRK_QF00.name) {
        work.board = ETRK_QF_BOARD;
        SetDisplayFlagForProp(ETRK_QF_PROP, 0);
    } else if (g_currentMap.name == ETRK_QUG0.name) {
        SetDisplayFlagForProp(ETRK_QUG_PROP, 0);
        work.board = ETRK_QUG_BOARD;
    }
    EVT_AT_SetPos(work.board, ETRK_BOARD, Vector3{}, 0);
    EventSetRawAngle angle{.cleanup = 0, .character = work.board, .parent = ETRK_BOARD, .rotation = {}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    EtrkSetParts(work, 0);
}
// @verified - p
void EtrkIntroduction(EtrkWork &work) {
    EV_EnableCharacterDisplay(ETRK_RYO);
    EVT_AT_SetPos(ETRK_RYO, work.board, Vector3{-.11f, -1.355f, .80500001f}, 0);
    EVT_AT_SetAngY(ETRK_RYO, work.board, 0, 0);
    EVT_AT_SetMot(ETRK_RYO, ETRK_IDLE_MOTION, 0, 1, ETRK_MOTION_FLAGS, 0);
    EV_DisableCharacterDisplay(ETRK_HAND);
    const char *first;
    const char *second;
    if (g_currentMap.name == ETRK_QF00.name) {
        if (EV_GetShenmueFlag(0x308)) {
            EV_stepSetFlags(0, 0x308);
            first = "0304A045"; // @text EN: "Huh?" | JA: "ん…？"
            second = "0304A047"; // @text EN: "I think I can open it with a key." | JA: "カギを使えば開けられそうだ。"
        } else if (shenGetRandomInt32(2)) {
            first = "0304A010"; // @text EN: "I think I can use a key to open it." | JA: "カギを使って開けられそうだな。"
            second = "0304A049"; // @text EN: "The power switch should be inside=@" | JA: "電源スイッチはこの中に…"
        } else {
            first = "0304A031"; // @text EN: "The switchboard." | JA: "配電盤だ。"
            second = "0304A048"; // @text EN: "There's an opening for a key." | JA: "カギ穴があいてる…"
        }
    } else if (g_currentMap.name == ETRK_QUG0.name) {
        if (EV_GetShenmueFlag(0x422)) {
            EV_stepSetFlags(0, 0x422);
            first = "0304A030"; // @text EN: "I found it!" | JA: "んっ…あった！"
            second = "0304A031"; // @text EN: "The switchboard." | JA: "配電盤だ。"
        } else if (shenGetRandomInt32(2)) {
            first = "0304A033"; // @text EN: "If I use the key=@" | JA: "カギを使えば…"
            second = "0304A049"; // @text EN: "The power switch should be inside=@" | JA: "電源スイッチはこの中に…"
        } else {
            first = "0304A031"; // @text EN: "The switchboard." | JA: "配電盤だ。"
            second = "0304A032"; // @text EN: "The key should open it." | JA: "カギで開けられるはずだ…"
        }
    } else
        return;
    EVT_AT_SetCam(1100, work.board, work.board, 0, 0, 0);
    EVT_AT_SetVoice(first, 1, 0);
    EVT_AT_SetCam(1101, work.board, work.board, 0, 0, 60);
    EVT_AT_SetVoice(second, 1, 60);
    HLib::SetupTaskDelay(130);
}
// @verified - p
void EtrkPrepareExit(EtrkWork &work) {
    ExecuteCleanupFuncForAreaWithCachedScene();
    Vector3 position{.2f, 0, .079999998f};
    Vector3i rotation{};
    EV_scenApplyPosOrRotToPartOf(work.board, 153, &position, &rotation, EventCharacterPartTransformMode::Absolute);
    SetHighModelXYZ(FindCharacterByScenarioName(ETRK_RYO), 0, 0, 0);
    hndlSetFlags(FindCharacterByScenarioName(ETRK_RYO));
    SetHighModelXYZ(FindCharacterByScenarioName(ETRK_HAND), 0, 0, 0);
    hndlSetFlags(FindCharacterByScenarioName(ETRK_HAND));
    EV_EnableCharacterDisplay(ETRK_RYO);
    EVT_AT_SetMot(ETRK_RYO, ETRK_IDLE_MOTION, 0, 1, ETRK_MOTION_FLAGS, 0);
    GetMotionFrameCount(ETRK_IDLE_MOTION);
    camCtrlSetState(CameraState::Init);
    camCtrlSetFovTransitionTo(true);
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_DisableCharacterDisplay(ETRK_HAND);
    if (g_currentMap.name == ETRK_QF00.name)
        SetDisplayFlagForProp(ETRK_QF_PROP, 1);
    else if (g_currentMap.name == ETRK_QUG0.name)
        SetDisplayFlagForProp(ETRK_QUG_PROP, 1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetupTaskDelay(30);
}
// @verified - p
void EtrkExit() {
    SetPlayerControlEnabled(1);
    EV_SetCharaControllerFlags(ETRK_RYO, 1, static_cast<NpcCharacterControllerState>(1));
    EV_SetCharaControllerFlags(ETRK_RYO, 3, static_cast<NpcCharacterControllerState>(1));
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(1));
    AT_SetLocalBaseChara(ETRK_RYO, ETRK_INVALID);
    AT_SetLocalBaseChara(ETRK_HAND, ETRK_INVALID);
    AT_SetLocalBaseChara(ETRK_CAMERA, ETRK_INVALID);
    AT_SetLocalBaseChara(ETRK_KEY_BASE, ETRK_INVALID);
    scenFreeEvent("ETRK");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ETRK"), 0);
    EV_FreeAuthorPak(0, "0289");
    LoadCutsceneAndSoundProgramForArea();
    EV_NPC_NewLoadSw(1);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
}
// @verified - p
void EtrkMain(void *data) {
    EtrkWork &work = *static_cast<EtrkWork *>(data);
    std::int32_t result;
    switch (work.state) {
    case 0:
        EtrkIntroduction(work);
        work.state = 1;
        break;
    case 1:
        result = EtrkPrepareKeys(work);
        if (result == -1)
            work.state = 8;
        else if (result == 1)
            work.state = 2;
        break;
    case 2:
        result = EtrkPollKeys(work);
        if (result == -1)
            work.state = 8;
        else if (result == 1)
            work.state = 3;
        break;
    case 3:
        result = EtrkTryKey(work);
        if (!result)
            work.state = 1;
        else if (result == 1)
            work.state = 4;
        break;
    case 4:
        EtrkOpenBoard(work);
        work.state = 5;
        break;
    case 5:
        EtrkPrepareSwitches(work);
        work.state = 6;
        break;
    case 6:
        result = EtrkPollSwitches(work);
        if (result == -1)
            work.state = 8;
        else if (result == 1)
            work.state = 7;
        break;
    case 7:
        EtrkPressSwitch(work);
        work.state = 5;
        break;
    case 8:
        EtrkPrepareExit(work);
        work.state = 9;
        break;
    case 9:
        EtrkExit();
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
