#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
extern std::uint64_t kshtAuthorMapLighting;
namespace {
void DrawingExit(void *) {
    SetRenderClockAndCompassTo(0);
    SetPlayerControlEnabled(1);
    FinishLoading();
    HLib::DestroyCurrentTask();
}
void DrawingRestore(void *) {
    if (kshtAuthSelection != -1)
        return;
    Vector3 position{147.10001f, 500, 210.10001f};
    position.y = shenFindIntersectionWithScene(position, 500);
    EV_AT_SetPos(KSHT_RYO, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(KSHT_RYO, EventTransformFlags::AllAxes, {0, 0, 0});
    mtwkRequestMotion(scenFindMotionwork(KSHT_RYO), 0xf003, MotionRegion::LowerBody);
    ResetKshtMotion(KSHT_RYO);
    ResetKshtCamera();
    EV_NPC_NewLoadSw(1);
    EV_LoadCutsceneFromCurrentScene("KSH1");
    EV_LoadAudio("KSH1_000", 0);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(DrawingExit);
    HLib::SetupTaskDelay(30);
}
void DrawingMorning(void *) {
    ST_LoadTitleFade();
    kshtAuthSelection = 22;
    QueueKshtAuthor(&kshtAuthSelection);
    HLib::SetCurrentTaskTo(DrawingRestore);
}
void DrawingNewDay(void *) {
    if (kshtAuthSelection != -1)
        return;
    DateTime date;
    GetCurrentDateTime(date);
    if (date.hour >= 19)
        ++date.day;
    date.hour = 8;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    EV_stepSetFlags(0, 100);
    Queue_ST_LoadTitleFadeIn(SHEN_NAME(NDAY));
    HLib::SetCurrentTaskTo(DrawingMorning);
    kshtAuthorMapLighting = 0x100000001ULL;
    ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
    HLib::SetupTaskDelay(120);
}
void DrawingFirstWait(void *) {
    HLib::SetCurrentTaskTo(DrawingNewDay);
}
} // namespace
void KshtWaitDrawingCallback(void *) {
    if (kshtAuthSelection == -1) {
        HLib::SetCurrentTaskTo(DrawingFirstWait);
        HLib::SetupTaskDelay(120);
    }
}
void KshtWaitReturnCallback(void *) {
    if (kshtAuthSelection == -1) {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        HLib::DestroyCurrentTask();
        EV_stepSetFlags(0, 0x2d9);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
        EV_scenQueueLoad(static_cast<std::uint32_t>(g_actNumber), SHEN_NAME(KWW4), 0);
    }
}
void KshtWaitAuthorCallback(void *) {
    if (kshtAuthSelection != -1)
        return;
    kshtAuthorMapLighting = 0x100000001ULL;
    SetDisplayFlagForProp(SHEN_NAME(M408), 1);
    SetDisplayFlagForProp(SHEN_NAME(M301), 1);
    ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    ResetKshtCamera();
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetRenderClockAndCompassTo(0);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
    shenSetFreeQuestInactive();
}
} // namespace shenmue::s2::areas
