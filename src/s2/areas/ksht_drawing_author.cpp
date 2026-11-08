#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
namespace {
struct KshtDrawingLoop {
    KshtAuthorController *parent;
    std::int16_t frame;
    std::byte reserved0a[2];
    float skyAlpha, propAlpha, sceneAlpha;
};
static_assert(sizeof(KshtDrawingLoop) == 0x18);
inline constexpr shen_name drawingSky{SHEN_NAME(a067)}, drawingScene{SHEN_NAME(a460)}, drawingProp{SHEN_NAME(a229)};
inline constexpr shen_name drawingProps[]{drawingSky, drawingScene, SHEN_NAME(a431), SHEN_NAME(a214), SHEN_NAME(aa29), drawingProp};
void PrepareDrawingCharacters() {
    PrepareKshtActor(KSHT_RYO);
    PrepareKshtActor(KSHT_SIN);
    for (shen_name prop : drawingProps)
        PrepareKshtProp(prop);
}
void RestoreDrawingCharacters() {
    RestoreKshtActor(KSHT_RYO);
    RestoreKshtActor(KSHT_SIN);
    for (shen_name prop : drawingProps)
        RestoreKshtProp(prop);
}
void DrawingFrame(KshtDrawingLoop *loop, std::int32_t shift) {
    loop->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    const std::int32_t frame = loop->frame;
    if (frame == 0) {
        PrepareDrawingCharacters();
        SetScenarioCharacterTransparency(drawingProp, loop->propAlpha);
        SetScenarioCharacterTransparency(drawingSky, loop->skyAlpha);
        SetScenarioCharacterTransparency(drawingScene, loop->sceneAlpha);
    } else if (frame == 721 - shift)
        SetIsSkySetup(0);
    else if (frame == 1240 - shift + (shift ? 1 : 0)) {
        SetScenarioCharacterTransparency(drawingSky, 1);
        SetIsSkySetup(1);
    }
    if (static_cast<std::uint32_t>(frame - (1121 - shift)) <= 0x76U) {
        loop->propAlpha += 0.008299999870359898f;
        SetScenarioCharacterTransparency(drawingProp, loop->propAlpha);
    } else if (frame == 1240 - shift)
        SetScenarioCharacterTransparency(drawingProp, 1);
    if (static_cast<std::uint32_t>(frame - (1121 - shift)) <= 0x30U) {
        loop->sceneAlpha += 0.019999999552965164f;
        SetScenarioCharacterTransparency(drawingScene, loop->sceneAlpha);
    } else if (frame == 1170 - shift)
        SetScenarioCharacterTransparency(drawingScene, 1);
    if (static_cast<std::uint32_t>(frame - (1045 - shift)) <= 0x4aU) {
        loop->skyAlpha -= 0.0065999999642372131f;
        SetScenarioCharacterTransparency(drawingSky, loop->skyAlpha);
    } else if (frame == 1120 - shift)
        SetScenarioCharacterTransparency(drawingSky, 0.5f);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (GetAuthorFlags())
            spmFadeOut();
        SetIsSkySetup(1);
        RestoreDrawingCharacters();
        loop->parent->state = 4;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void DrawingFirstCallback(void *data) {
    DrawingFrame(static_cast<KshtDrawingLoop *>(data), 0);
}
void DrawingSecondCallback(void *data) {
    DrawingFrame(static_cast<KshtDrawingLoop *>(data), 180);
}
} // namespace
void QueueKshtDrawingAuthor(KshtAuthorController *parent) {
    constexpr HLib::TaskCallback callbacks[]{DrawingFirstCallback, DrawingSecondCallback};
    HLib::Task *task = HLib::EnqueueTaskWithParameter(callbacks[parent->selection - 8], 4, 0x0b, sizeof(KshtDrawingLoop), SHEN_NAME(EVNT));
    KshtDrawingLoop *loop = static_cast<KshtDrawingLoop *>(HLib::GetTaskParameter(task));
    loop->frame = 0;
    loop->sceneAlpha = 0;
    loop->parent = parent;
    loop->skyAlpha = 1;
    loop->propAlpha = 0;
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    if (parent->selection == 8)
        EV_lightLoad(SHEN_NAME(l120));
    else if (parent->selection == 9)
        EV_lightLoad(SHEN_NAME(l121));
    PushGlobalLightingProgram();
    EV_LoadAudio("0512A000", 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(kshtAuthorIds[parent->selection].name));
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    parent->state = 3;
}
void SetupKshtDrawingAuthor() {
    SetPlayerControlEnabled(0);
    EV_LoadCutsceneFromCurrentScene("0512A000");
    EV_FreeAuthorPak(1, "0512");
}
void ExitKshtDrawingAuthor(KshtAuthorController *parent) {
    EV_FreeAuthorPak(0, "0512");
    *parent->result = -1;
    FreeKshtCharacters(kshtCompanionList);
    HLib::DestroyTask(parent->task);
}
} // namespace shenmue::s2::areas
