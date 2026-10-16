#include "s2/areas/ksht_authored.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/model_component.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
extern std::uint64_t kshtAuthorMapLighting;
extern std::int32_t kshtFreeTalkSelection, kshtNextPhase, kshtFreeTalkIndicator;
void PrepareKshtFreeTalkActors();
void KshtSetHighHands(void *);
void KshtFreeTalkCallback(void *) {
    if (kshtFreeTalkSelection != -1)
        return;
    switch (kshtPhase) {
    case 0:
        kshtFreeTalkSelection = -1;
        kshtPhase = 1;
        ProcessAicaCommand(0xaa0, 30);
        HLib::SetupTaskDelay(31);
        break;
    case 1:
        ResetCharaFixo(FindCharacterByScenarioName(SHEN_NAME(a726)));
        ResetCharaFixo(FindCharacterByScenarioName(SHEN_NAME(a728)));
        FreeKshtCharacters(kshtCompanionList);
        kshtFreeTalkSelection = 7;
        kshtPhase = 7;
        EV_stepSetFlags(0, 0x371);
        QueueKshtAuthor(&kshtFreeTalkSelection);
        break;
    case 2:
        kshtFreeTalkSelection = -1;
        kshtPhase = 3;
        EV_stepSetFlags(0, 0x372);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1004, 0);
        ProcessAicaCommand(0xaa0, 15);
        HLib::SetupTaskDelay(16);
        break;
    case 3:
        ResetCharaFixo(FindCharacterByScenarioName(SHEN_NAME(a726)));
        ResetCharaFixo(FindCharacterByScenarioName(SHEN_NAME(a728)));
        kshtFreeTalkSelection = 6;
        kshtPhase = 4;
        QueueKshtAuthor(&kshtFreeTalkSelection);
        break;
    case 4:
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_SIN), 1);
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_RYO), 1);
        FinishLoading();
        SetRenderClockAndCompassTo(0);
        kshtFreeTalkSelection = 0;
        HLib::DestroyCurrentTask();
        break;
    case 5:
        kshtFreeTalkSelection = -1;
        kshtPhase = 6;
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        FreeKshtCharacters(kshtCompanionList);
        EV_npcLockUnlock(KSHT_DUMMY);
        EV_NPC_NewLoadSw(1);
        EV_stepSetFlags(0, 0x372);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1004, 0);
        break;
    case 6:
        kshtFreeTalkSelection = 0;
        SetPlayerControlEnabled(1);
        SetRenderClockAndCompassTo(0);
        SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::DestroyCurrentTask();
        break;
    case 7: {
        scenCvsCtrlFree(GetConverseDataForChara(KSHT_SIN));
        scenCvsCtrlFree(GetConverseDataForChara(KSHT_RYO));
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1515))
            SetScreenFilmFade(30, 255, 255, 255, 255, 0, 255, 255, 255);
        else
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_CheckWarmStartNpcRoute(KSHT_DUMMY);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        kshtAuthorMapLighting = 0x100000001ULL;
        ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
        EVS_LoopCamera(1000, shen_name{0}, shen_name{0});
        PrepareKshtFreeTalkActors();
        SetRenderClockAndCompassTo(0);
        CameraUtilityParameters parameters{.first = 0, .second = 0};
        camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&parameters));
        HLib::SetupTaskDelay(60);
        kshtFreeTalkSelection = -1;
        kshtPhase = 8;
        break;
    }
    case 8:
        camCtrlSetState(CameraState::SnapYawTarget);
        if (StartScenarioFreeTalkRecord(KSHT_SIN, -1) == 1) {
            kshtFreeTalkSelection = -1;
            kshtPhase = 9;
            HLib::Task *task = HLib::EnqueueTaskWithoutParameter(KshtSetHighHands, 4, 0x0b, static_cast<std::int32_t>(EVENT_HIGH_MODEL_TASK.name));
            HLib::SetupTimedTask(task, 2);
            FinishLoading();
        }
        break;
    case 9:
        kshtFreeTalkIndicator = 0;
        if (EV_GetShenmueFlag(0x371)) {
            kshtPhase = 10;
            kshtNextPhase = 0;
            kshtFreeTalkIndicator = 1;
        }
        if (EV_GetShenmueFlag(0x372)) {
            kshtFreeTalkIndicator = 2;
            kshtPhase = 10;
            kshtNextPhase = 2;
        } else if (!kshtFreeTalkIndicator)
            return;
        EVS_LoopCamera(2000, shen_name{0}, shen_name{0});
        StopScenarioFreeTalkRecord();
        SetRenderClockAndCompassTo(1);
        HLib::SetupTaskDelay(3);
        break;
    case 10:
        if (EV_GetShenmueFlag(0x371))
            SetScreenFilmFade(30, 0, 255, 255, 255, 255, 255, 255, 255);
        if (EV_GetShenmueFlag(0x372))
            SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        EV_CheckWarmStartNpcRoute(KSHT_DUMMY);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1004, 0);
        kshtFreeTalkIndicator = 0;
        kshtPhase = kshtNextPhase;
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
