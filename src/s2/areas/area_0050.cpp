#include "s2/areas/area_0050.hpp"
#include "s2/map_assets.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
namespace shenmue::s2::areas {
namespace {
std::array<std::byte, 0x100> distance0050{}, kernel0050First{}, kernel0050Second{};
std::uint32_t splash0050{};
std::uint16_t splash0050Frame{};
void E0050MainCallback(void *);
void E0050LoopCallback(void *);
void E0050SplashCallback(void *);
// @verified - p
void E0050Init() {
    distance0050.fill(std::byte{});
    kernel0050First.fill(std::byte{});
    kernel0050Second.fill(std::byte{});
}
// @verified - p
void E0050WaveCallback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.36070001125335693f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.3725000023841858f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.4352000057697296f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.2549000084400177f * 0.2549000084400177f,
            static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3799999952316284f, static_cast<float>(specular & 255U) / 255.0f * 0.28619998693466187f},
        42.540000915527344f, 1.0f);
    ApplyWaveTextureMeshResource(3);
    ApplyWaveReflectionResource(4);
    SetTextureAnimationUV(-0.00030000001f, 0.00030000001f);
    ApplyTextureAnimationResource(4);
}
// @verified - p
void SetupE0050Environment(AreaAuthorTaskParameter *parent) {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0050");
    DateTime time{};
    GetCurrentDateTime(time);
    EV_SetModelDataDefaultVertexShader();
    if (time.hour < 19) {
        EV_SetObjectFlagByIndex(7, 0);
        EV_SetObjectFlagByIndex(6, 1);
        EV_SetWeatherInstruction(0x780, 0);
        SetFogRed(15.0f);
        SetFogGreen(800.0f);
        SetFogBlue(0.25f);
    } else {
        EV_lightLoad(AREA_0050_LIGHT);
        PushGlobalLightingProgram();
        EV_SetObjectFlagByIndex(7, 1);
        EV_SetObjectFlagByIndex(6, 0);
        if (parent->selection == 10)
            EV_lightLoad(AREA_0050_LIGHT_SPECIAL);
        EV_SetWeatherInstruction(0x780, 0);
        SetFogRed(15.0f);
        SetFogGreen(350.0f);
        SetFogBlue(0.25f);
    }
    SetFogDistance(0xfffaeaeaU);
    ReinitFogTasks(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    GenerateFFT(2.0f);
    FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, distance0050)));
    void *const second = FillFftKernel(0.03500000014901161f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, kernel0050Second));
    void *const first = FillFftKernel(0.05640000104904175f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, kernel0050First));
    ApplyFft(first, second);
    FinalizeFft(0.001f, 0.001f);
    SetWaveLighting(0.1f, {0.059799999f, 0.97680002f, 0.2054f});
    HLib::EnqueueTaskWithoutParameter(E0050WaveCallback, 3, 2, static_cast<std::int32_t>(AREA_0050_WAVE.name));
    SetMapModelVertexShaderType(3, 1);
    SetMapModelVertexShaderType(4, 1);
}
// @verified - p
void E0050Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0050MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0050A.MOT"));
    parent->selection = 0;
    parent->state = 0;
    SetupE0050Environment(parent);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0050_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0050Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0050_TKR), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0050_TKR, false);
    EV_SetCharacterAlphaEnabled(AREA_0050_AB10, false);
    EV_SetCharacterAlphaEnabled(AREA_0050_AB11, false);
    const Vector3 zero{};
    EV_AT_SetPos(AREA_0050_AB10, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_AT_SetPos(AREA_0050_AB11, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
}
// @verified - p
void RestoreE0050Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0050_TKR), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0050_TKR, true);
    EV_SetCharacterAlphaEnabled(AREA_0050_AB10, true);
    EV_SetCharacterAlphaEnabled(AREA_0050_AB11, true);
}
// @verified - p
void QueueE0050Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0050LoopCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const loop = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    loop->parent = parent;
    loop->currentFrame = 0;
    EV_authSceneCtrl(
        ((parent->selection >= 1 && parent->selection <= 5) || parent->selection == 11) ? AuthSceneCtrl::HideFilmBars : AuthSceneCtrl::ShowFilmBars);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0050Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void QueueE0050Splash() {
    const Vector3 position{160.66f, 99.800003f, 231.60001f}, volume{0.2f, 0.0f, 0.2f};
    const Vector3 velocity{0.0074509755f, 0.059999995f, 0.016078435f}, variance{0.015294112f, 0.041176476f, 0.039607845f};
    HLib::EnqueueTaskWithoutParameter(E0050SplashCallback, 4, 0x0b, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
    splash0050 =
        static_cast<std::uint32_t>(CreateSplashEffect(position, volume, 1000, 0.02f, shen_name{0}, std::bit_cast<std::int32_t>(98.0f), nullptr, nullptr));
    SetParticleEffectColour(1, splash0050, 255, 175, 175, 255);
    SetParticleEffectPosition(1, splash0050, &position, &volume);
    SetParticleEffectVelocity(1, splash0050, velocity, variance);
    SetParticleEffectBirthRate(1, splash0050, 0.1f);
    SetParticleEffectSize(1, splash0050, 0.012549019418656826f, 0.012549019418656826f);
    ResumeParticleEffect(1, splash0050);
}
// @verified - p
void E0050SplashCallback(void *) {
    if (splash0050Frame == 20)
        SetParticleEffectBirthRate(1, splash0050, 0.0f);
    else if (splash0050Frame == 45) {
        splash0050Frame = 0;
        SuspendParticleEffect(1, splash0050);
        FreeParticleEffect(1, splash0050);
        HLib::DestroyCurrentTask();
        return;
    } else if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        HLib::DestroyCurrentTask();
        ++splash0050Frame;
        return;
    }
    ++splash0050Frame;
}
// @verified - p
void E0050LoopCallback(void *parameter) {
    AreaAuthorFrameTaskParameter *const loop = static_cast<AreaAuthorFrameTaskParameter *>(parameter);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (loop->parent->selection == 10 && frame == 358)
        QueueE0050Splash();
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        RestoreE0050Characters();
        loop->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void CleanupE0050(AreaAuthorTaskParameter *parent) {
    if (FindCharacterByScenarioName(AREA_0050_TKR)) {
        EV_npcLockUnlock(AREA_0050_TKR);
        EV_npcFree(AREA_0050_TKR);
    }
    EV_FreeAuthorPak(0, "0050");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void E0050MainCallback(void *parameter) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(parameter);
    if (parent->state != 0) {
        if (parent->state == 1)
            QueueE0050Loop(parent);
        else if (parent->state == 3)
            CleanupE0050(parent);
        return;
    }
    const std::int16_t input = EVT_GetControllerInput(0, INPUT_PRESSED);
    if (input == 1024)
        parent->state = 1;
    else if (input == 64) {
        if (--parent->selection < 0)
            parent->selection = 12;
    } else if (input == 128) {
        if (++parent->selection > 12)
            parent->selection = 0;
    }
    if (parent->selection >= 0 && parent->selection <= 12) {
        EV_set_debug_text_pos(2, 4);
        EV_debug_log_to_screen("%s\n", AREA_0050_LABELS[parent->selection]);
    }
    EV_set_debug_text_pos(2, 5);
    EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
}
} // namespace
const ScriptAreaDefinition area0050ScriptDefinition{AREA_0050_AREA, 0, E0050Init, E0050Main};
} // namespace shenmue::s2::areas
