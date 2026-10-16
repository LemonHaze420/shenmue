#include "s2/areas/area_bgsm.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {
std::int32_t bgsmOpponentIndex{}, bgsmStartingCash{}, bgsmOpponentPose{};
const ScriptAreaDefinition bgsmScriptDefinition{.areaId = BGSM_AREA, .field04 = 0, .init = AreaBGSMInit, .main = AreaBGSMMain};
namespace {
shen_name bgsmCharacters[]{BGSM_RYO, SHEN_NAME(KYO_), BGSM_INVALID};
}
void AreaBGSMInit() {
    bgsmOpponentPose = 0;
    bgsmOpponentIndex = 0;
    bgsmStartingCash = 0;
    bgsmVoiceCount = 0;
    std::memset(bgsmVoicePrefixes, 0, sizeof(bgsmVoicePrefixes));
    std::memset(bgsmVoices, 0, sizeof(bgsmVoices));
}
void AreaBGSMMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    HLib::Task *film = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), BGSM_TASK);
    RegisterEventTask(film);
    EventFilmControl *fade = static_cast<EventFilmControl *>(HLib::GetTaskParameter(film));
    *fade = {.cleanup = 1,
        .frames = 1,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 1};
    if (g_currentMap.name == BGSM_AREA.name) {
        EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
        GambleInit();
        scenFreeEvent("GSET.EV3");
        EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
    }
    SetPlayerControlEnabled(0);
    HLib::Task *clear = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), BGSM_TASK);
    RegisterEventTask(clear);
    StepNpcClearWork *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(clear));
    bgsmOpponentIndex = static_cast<std::int32_t>(argument);
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->callback = BgsmBegin;
    work->delay = 3;
    work->reserved1c = 0;
}
void BgsmBegin(void *) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BgsmInitialize, 4, 0x0b, sizeof(BgsmWork), BGSM_TASK);
    BgsmWork *work = static_cast<BgsmWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->state = 0;
    const char *music = GetCurrentBgm();
    const char *soundBank = shenGetCurrentSbt();
    std::strcpy(work->music, music);
    std::strcpy(work->soundBank, soundBank);
    bgsmStartingCash = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash));
    work->soundProgram = GetSelectedSoundSceneIndex();
    std::uint32_t table;
    if (g_currentMap.name == BGSM_AREA.name) {
        work->opponent = BGSM_OPPONENTS[0];
        work->standalone = 1;
        table = static_cast<std::uint32_t>(g_entryPoint) % 6 + 7;
    } else {
        table = bgsmOpponentIndex;
        work->standalone = 0;
        work->opponent = BGSM_OPPONENTS[table];
    }
    work->table = static_cast<std::uint16_t>(table);
    bgsmCharacters[1] = work->opponent;
    work->characters = Queue_EVT_AT_ReadChr(task, bgsmCharacters);
}
void BgsmInitialize(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    SetLoadingDrawFlags(3);
    EV_EventSetUpHelpIcon_0(0x48, 0);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0103");
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BGSM"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BGSM"), 0);
    EV_LoadCutsceneFromCurrentScene("01030000");
    EV_LoadAudio("01030000", 0);
    EV_spmBeginSampleExecute();
    SetLowQualityHands(ctrlGetCharaByName(BGSM_RYO));
    SetLowQualityHands(ctrlGetCharaByName(BGSM_HAND));
    SetHighModelXYZ(ctrlGetCharaByName(BGSM_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(BGSM_RYO), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(BGSM_HAND), 1, 0, 0);
    BgsmHand(BGSM_RYO, bgsmOpenHand, 8);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(BGSM_RYO), 1, reinterpret_cast<std::byte *>(bgsmOpenHand), 8);
    EV_NPC_NewLoadSw(1);
    BgsmCreateBoard(work);
    FinishLoading();
    work.reserved4c = 0;
    work.combinations = BGSM_COMBINATIONS[work.board];
    BgsmPlaceCharacters(work);
    HLib::SetCurrentTaskTo(BgsmWaitLoading);
    std::strcpy(bgsmVoicePrefixes[0], "0103A");
    switch (work.opponent.name) {
    case SHEN_NAME(UGN_):
        std::strcpy(bgsmVoicePrefixes[1], "0103B");
        break;
    case SHEN_NAME(MIK_):
        std::strcpy(bgsmVoicePrefixes[1], "0103C");
        break;
    case SHEN_NAME(KYO_):
        std::strcpy(bgsmVoicePrefixes[1], "0103D");
        break;
    case SHEN_NAME(DYU_):
        std::strcpy(bgsmVoicePrefixes[1], "0103E");
        break;
    case SHEN_NAME(TLB_):
        std::strcpy(bgsmVoicePrefixes[1], "0103F");
        break;
    case SHEN_NAME(CAK_):
        std::strcpy(bgsmVoicePrefixes[1], "0103G");
        break;
    case SHEN_NAME(DS1_):
        std::strcpy(bgsmVoicePrefixes[1], "0103H");
        break;
    default:
        break;
    }
    QueueSkNpcWait(work.self);
}
void BgsmWaitLoading(void *) {
    HLib::SetCurrentTaskTo(BgsmTalk);
    RetireLoadingDraw();
}
void BgsmStop(void *) {
    if (g_currentMap.name == BGSM_AREA.name)
        HLib::SetCurrentTaskTo(BgsmTalk);
    else {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        HLib::SetCurrentTaskTo(BgsmEnd);
        HLib::SetupTaskDelay(30);
    }
}
void BgsmEnd(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (((g_currentMap.name - SHEN_NAME(AKT0)) & 0xfcffffffU) == 0) {
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(BGSM_RYO), 1);
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 1);
    }
    npcSetAreaName(BGSM_REGION, nullptr, 0, 0);
    for (shen_name spot : BGSM_SPOTS)
        if (DoesScenarioCharacterExist(spot))
            SetCharaKill(spot, 1);
    EventSetPoint point{.character = BGSM_RYO,
        .target = {},
        .targetBone = 0,
        .trackingMode = 3,
        .positionMode = 0,
        .duration = 0,
        .flags = 0x10000000U,
        .frame = 0,
        .cleanup = 0,
        .delay = 0};
    EVT_AT_SetPoint_Callback(&point);
    point.character = work.opponent;
    EVT_AT_SetPoint_Callback(&point);
    EV_EventSetUpHelpIcon_0(0, 0);
    AT_SetLocalBaseChara(BGSM_HAND, BGSM_INVALID);
    SetCharaKill(BGSM_BASE, 1);
    SetCharaKill(BGSM_HAND_BASE, 1);
    SetCharaKill(BGSM_CAMERA_BASE, 1);
    hndlSetFlags(ctrlGetCharaByName(BGSM_RYO));
    hndlSetFlags(ctrlGetCharaByName(BGSM_HAND));
    EV_LoadCutsceneFromCurrentScene(work.music);
    EV_LoadAudio(work.soundBank, work.soundProgram);
    EV_spmBeginSampleExecute();
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    ET_AT_KillWork(work.characters);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BGSM"), 0);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0103");
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
} // namespace shenmue::s2::areas
