#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
std::uint64_t kshtAuthorMapLighting;
namespace {
inline constexpr shen_name departureProps[]{SHEN_NAME(a726), SHEN_NAME(a728), SHEN_NAME(aa62), SHEN_NAME(aa63)};
inline constexpr shen_name basket{SHEN_NAME(aa44)}, secondBasket{SHEN_NAME(aa45)};
void PrepareDeparture(std::int32_t selection) {
    PrepareKshtActor(KSHT_RYO);
    PrepareKshtActor(KSHT_SIN);
    for (shen_name prop : departureProps)
        PrepareKshtProp(prop);
    if (selection == 19) {
        PrepareKshtProp(basket);
        PrepareKshtProp(secondBasket);
    } else if (selection == 20) {
        charaSetDisplayEnabledFlag(FindCharacterByScenarioName(departureProps[0]), 1);
        charaSetDisplayEnabledFlag(FindCharacterByScenarioName(departureProps[1]), 1);
        EV_DisableCharacterDisplay(basket);
        EV_DisableCharacterDisplay(secondBasket);
    }
}
void RestoreDeparture() {
    RestoreKshtActor(KSHT_RYO);
    RestoreKshtActor(KSHT_SIN);
    for (shen_name prop : departureProps)
        RestoreKshtProp(prop);
    RestoreKshtProp(basket);
    RestoreKshtProp(secondBasket);
}
void DepartureCallback(void *data) {
    KshtAuthorController *parent = *static_cast<KshtAuthorController **>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (parent->selection == 19) {
            parent->selection = 20;
            parent->state = 1;
        } else {
            RestoreDeparture();
            parent->state = 4;
        }
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
} // namespace
void QueueKshtDepartureAuthor(KshtAuthorController *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(DepartureCallback, 4, 0x0b, sizeof(parent), SHEN_NAME(EVNT));
    *static_cast<KshtAuthorController **>(HLib::GetTaskParameter(task)) = parent;
    if (parent->selection == 19) {
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(SHEN_NAME(l371));
        PushGlobalLightingProgram();
    } else if (parent->selection == 20) {
        kshtAuthorMapLighting = 0x200000001ULL;
        ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
        EV_lightLoad(SHEN_NAME(l372));
        SetCurrentActiveLight(3);
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    PrepareDeparture(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(kshtAuthorIds[parent->selection].name));
    parent->state = 3;
}
void SetupKshtDepartureAuthor() {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    EV_LoadCutsceneFromCurrentScene("0537A000");
    EV_LoadAudio("0537A000", 0);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_FreeAuthorPak(1, "0537");
}
void ExitKshtDepartureAuthor(KshtAuthorController *parent) {
    EV_FreeAuthorPak(0, "0537");
    kshtAuthorMapLighting = 0x100000001ULL;
    ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
    EventSetPosition position{.cleanup = 0, .character = KSHT_RYO, .parent = KSHT_INVALID, .position = {142.97000122070312f, 100, 210}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{.cleanup = 0, .character = KSHT_RYO, .parent = KSHT_INVALID, .rotation = {0, 0, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    ResetKshtMotion(KSHT_RYO);
    *parent->result = -1;
    HLib::DestroyTask(parent->task);
    LoadCutsceneAndSoundProgramForArea();
    EV_LoadAudio("KSH1_000", 1);
    EV_spmBeginSampleExecute();
    FreeKshtCharacters(kshtCompanionList);
    ResetKshtCamera();
    SetScenarioCharacterTransparency(KSHT_RYO, 1);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit26, reinterpret_cast<void *>(std::uintptr_t{1}));
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetRenderClockAndCompassTo(0);
    FinishLoading();
    SetPlayerControlEnabled(1);
}
} // namespace shenmue::s2::areas
