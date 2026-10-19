#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name rabbit{SHEN_NAME(a349)}, rabbitProp{SHEN_NAME(a636)}, cup{SHEN_NAME(aa84)}, secondCup{SHEN_NAME(aa86)};
void PrepareParty() {
    PrepareKshtActor(KSHT_RYO);
    PrepareKshtActor(KSHT_SIN);
}
void RestoreParty() {
    RestoreKshtActor(KSHT_RYO);
    RestoreKshtActor(KSHT_SIN);
}
void RabbitCallback(void *data) {
    KshtAuthorLoop *loop = static_cast<KshtAuthorLoop *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        loop->parent->state = 4;
        RestoreParty();
        RestoreKshtProp(rabbit);
        RestoreKshtProp(rabbitProp);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void ConversationCallback(void *data) {
    KshtAuthorController *parent = *static_cast<KshtAuthorController **>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreParty();
        RestoreKshtProp(cup);
        RestoreKshtProp(secondCup);
        parent->state = 4;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void MorningCallback(void *data) {
    KshtAuthorLoop *loop = static_cast<KshtAuthorLoop *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        loop->parent->state = 4;
        RestoreParty();
        HLib::DestroyCurrentTask();
    }
}
void RabbitDropCallback(void *data) {
    KshtAuthorLoop *loop = static_cast<KshtAuthorLoop *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        loop->parent->state = 4;
        RestoreParty();
        HLib::DestroyCurrentTask();
    }
}
void FinishAuthor(KshtAuthorController *parent) {
    *parent->result = -1;
    FreeKshtCharacters(kshtCompanionList);
    HLib::DestroyTask(parent->task);
}
KshtAuthorLoop *AllocateLoop(HLib::TaskCallback callback, KshtAuthorController *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(callback, 4, 0x0b, sizeof(KshtAuthorLoop), SHEN_NAME(EVNT));
    KshtAuthorLoop *loop = static_cast<KshtAuthorLoop *>(HLib::GetTaskParameter(task));
    loop->parent = parent;
    loop->frame = 0;
    return loop;
}
void BeginAuthor(KshtAuthorController *parent) {
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(kshtAuthorIds[parent->selection].name));
    parent->state = 3;
}
} // namespace
void QueueKshtRabbitAuthor(KshtAuthorController *parent) {
    AllocateLoop(RabbitCallback, parent);
    EV_spmBeginSampleExecute();
    PrepareParty();
    PrepareKshtProp(rabbit);
    PrepareKshtProp(rabbitProp);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    BeginAuthor(parent);
}
void SetupKshtRabbitAuthor() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0528");
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
}
void ExitKshtRabbitAuthor(KshtAuthorController *parent) {
    EV_FreeAuthorPak(0, "0528");
    FinishAuthor(parent);
}
void QueueKshtConversationAuthor(KshtAuthorController *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ConversationCallback, 4, 0x0b, sizeof(parent), SHEN_NAME(EVNT));
    *static_cast<KshtAuthorController **>(HLib::GetTaskParameter(task)) = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    PrepareParty();
    PrepareKshtProp(cup);
    PrepareKshtProp(secondCup);
    EventSetPosition position{.cleanup = 0, .character = cup, .parent = KSHT_INVALID, .position = {0, 0, 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    position.character = secondCup;
    EVT_AT_SetPosRot(&position);
    SetDisplayFlagForProp(SHEN_NAME(M408), 0);
    SetDisplayFlagForProp(SHEN_NAME(M301), 0);
    BeginAuthor(parent);
}
void SetupKshtConversationAuthor() {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_FreeAuthorPak(1, "0521");
}
void ExitKshtConversationAuthor(KshtAuthorController *parent) {
    EV_FreeAuthorPak(0, "0521");
    ResetKshtMotion(KSHT_RYO);
    *parent->result = -1;
    HLib::DestroyTask(parent->task);
    LoadCutsceneAndSoundProgramForArea();
    EV_LoadAudio("KSH1_000", 1);
    FreeKshtCharacters(kshtCompanionList);
}
void QueueKshtMorningAuthor(KshtAuthorController *parent) {
    AllocateLoop(MorningCallback, parent);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    if (parent->selection == 21)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    PrepareParty();
    BeginAuthor(parent);
}
void SetupKshtMorningAuthor() {
    SetPlayerControlEnabled(0);
    EV_LoadCutsceneFromCurrentScene("0543A010");
    EV_FreeAuthorPak(1, "0543");
}
void ExitKshtMorningAuthor(KshtAuthorController *parent) {
    EV_FreeAuthorPak(0, "0543");
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    LoadCutsceneAndSoundProgramForArea();
    FinishAuthor(parent);
}
void QueueKshtRabbitDropAuthor(KshtAuthorController *parent) {
    AllocateLoop(RabbitDropCallback, parent);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    PrepareParty();
    BeginAuthor(parent);
}
void SetupKshtRabbitDropAuthor() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0545");
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
}
void ExitKshtRabbitDropAuthor(KshtAuthorController *parent) {
    EV_FreeAuthorPak(0, "0545");
    *parent->result = -1;
    FreeKshtCharacters(kshtCompanionList);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::DestroyTask(parent->task);
}
} // namespace shenmue::s2::areas
