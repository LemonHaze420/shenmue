#include "s2/areas/ksht_authored.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/model_component.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
void SelectKshtCloudPackage(std::int32_t);
void ExitKshtCloudAuthor(KshtAuthorController *parent) {
    LoadCutsceneAndSoundProgramForArea();
    if (parent->selection != 7)
        clmdUtil(FindCharacterByScenarioName(KSHT_SIN), 0);
    EV_SetWeatherInstruction(12, 1);
    EV_FreeAuthorPak(0, "0509");
    EV_FreeAuthorPak(0, "0510");
    EV_FreeAuthorPak(0, "0511");
    EV_SetObjectFlagByIndex(0, 1);
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetObjectFlagByIndex(2, 1);
    EV_SetObjectFlagByIndex(3, 1);
    EV_SetObjectFlagByIndex(4, 0);
    EV_SetObjectFlagByIndex(5, 1);
    if (parent->selection == 6) {
        EV_SetObjectFlagByIndex(1, 1);
        FreeKshtCharacters(kshtCompanionList);
        EV_AT_SetPos(KSHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {147.67999f, 100.45f, 205.60001f});
        EV_AT_SetAngY(KSHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 55523, 0});
        ResetKshtMotion(KSHT_RYO);
        camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        *parent->result = -1;
        HLib::DestroyTask(parent->task);
        SetScenarioCharacterTransparency(KSHT_RYO, 1);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit26, reinterpret_cast<void *>(std::uintptr_t{1}));
        EV_NPC_NewLoadSw(1);
        EV_LoadCutsceneFromCurrentScene("KSH1");
        EV_LoadAudio("KSH1_000", 0);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetPlayerControlEnabled(1);
    } else if (parent->selection == 7) {
        SelectKshtCloudPackage(102);
        parent->state = 5;
    } else {
        EV_npcLockUnlock(SHEN_NAME(SN5_));
        EV_npcFree(SHEN_NAME(SN5_));
        *parent->result = -1;
        EV_LoadCutsceneFromCurrentScene("KSH1");
        EV_LoadAudio("KSH1_000", 0);
        EV_spmBeginSampleExecute();
        HLib::DestroyTask(parent->task);
        SetScenarioCharacterTransparency(KSHT_RYO, 1);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit26, reinterpret_cast<void *>(std::uintptr_t{1}));
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}
} // namespace shenmue::s2::areas
