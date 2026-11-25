#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/debug.hpp"
#endif
#include "s2/areas/area_0013.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/pause.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>

namespace shenmue::s2::areas {
namespace {
struct E0013LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
};
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
#define E0013_INLINE
typedef shen_name E0013CharacterName;
#else
#if defined(_MSC_VER)
#define E0013_INLINE __forceinline
#else
#define E0013_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0013CharacterName;
#endif
void E0013MainCallback(void *);
void E0013LoopCallback(void *);
void SetupE0013Environment(void);
void QueueE0013Loop(AreaImmediateAuthorTaskParameter *);
void CleanupE0013(AreaImmediateAuthorTaskParameter *);
void SetupE0013Characters(void);
void RestoreE0013Characters(void);
void E0013WaveCallback(void *);
E0013_INLINE void Prepare0013Character(E0013CharacterName);
E0013_INLINE void Restore0013Character(E0013CharacterName);
E0013_INLINE void Hide0013Prop(E0013CharacterName);
E0013_INLINE void Show0013Prop(E0013CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void E0013Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(E0013MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    SetupE0013Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0013_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
#if defined(SHENMUE_EVENT_OVERLAY)
static unsigned char distance0013[256], kernel0013First[256], kernel0013Second[256];
#else
std::array<std::byte, 0x100> distance0013{}, kernel0013First{}, kernel0013Second{};
// @verified - p
void E0013Init() {
    distance0013.fill(std::byte{});
    kernel0013First.fill(std::byte{});
    kernel0013Second.fill(std::byte{});
}
// @verified - p
void E0013EnvironmentNullsub() {
}
#endif
void E0013MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 0:
#else
    if (parent->state == 1)
        QueueE0013Loop(parent);
    else if (parent->state == 3)
        CleanupE0013(parent);
    else if (parent->state == 4 && !FTBL.filmCheckFadeTrackColour()) {
        FTBL.SetPlayerControlEnabled(1);
        FTBL.FinishLoading();
        FTBL.npcLockUnlock(AREA_0013_WON);
        FTBL.SetScenarioFollowCharacter(AREA_0013_WON);
        FTBL.HLib_DestroyTask(parent->self);
    } else if (parent->state == 0) {
#endif
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.npcLockUnlock(AREA_0013_WON);
        FTBL.npcFree(AREA_0013_WON);
        FTBL.npcLockUnlock(AREA_0013_CLJ);
        FTBL.npcFree(AREA_0013_CLJ);
#endif
        FTBL.LoadNPCRoute("NPC_AR02.BIN");
        FTBL.HLib_SetupTimedTask(parent->self, 30);
        parent->state = 3;
#if defined(SHENMUE_EVENT_OVERLAY)
        break;
    case 1:
        QueueE0013Loop(parent);
        break;
    case 3:
        CleanupE0013(parent);
        break;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    case 4:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.SetPlayerControlEnabled(1);
            FTBL.FinishLoading();
            FTBL.npcLockUnlock(AREA_0013_WON);
            FTBL.SetScenarioFollowCharacter(AREA_0013_WON);
            FTBL.HLib_DestroyTask(parent->self);
        }
        break;
#endif
#endif
    }
}
// @verified - pd
void QueueE0013Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0013LoopCallback, 4, 11, sizeof(E0013LoopParameter), AREA_AUTHOR_TASK);
    ((E0013LoopParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_LoadAudio("0013A000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    SetupE0013Characters();
    FTBL.EV_authSceneCtrl(AREA_0013_AREA);
    parent->state = 2;
}
// @verified - pd
void E0013LoopCallback(void *data) {
    E0013LoopParameter *const child = (E0013LoopParameter *)data;

#if defined(SHENMUE_EVENT_OVERLAY)
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
#else
    if (FTBL.EV_authSceneCtrl(-1) != 0)
        return;
#endif
    {
        RestoreE0013Characters();
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        child->parent->state = 0;
        FTBL.HLib_SetupTimedTask(child->parent->self, 10);
#else
        AreaImmediateAuthorTaskParameter *const parent = child->parent;
        parent->state = 0;
        FTBL.HLib_SetupTimedTask(parent->self, 10);
#endif
        FTBL.HLib_DestroyCurrentTask();
    }
}
// @verified - pd
void SetupE0013Environment(void) {
    const Vector3 center = {220.63f, 100.0f, 343.79999f};
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 direction;
    void *first, *second;
#endif
    FTBL.SetPlayerControlEnabled(0);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_stepSetFlags(27, 0);
#endif
    if (FTBL.npcSetAreaName(AREA_0013_ANP1, &center, 5.0f, 1) == 1)
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("NPC_AREA1 : TRUE\n");
#else
        EV_debug_log("0013\\e0013_et_0013.c", 221, "NPC_AREA1 : TRUE\n");
#endif
    FTBL.EV_SetNewSceneIndex();
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0013_AREA) {
#else
    if (g_currentMap.name == AREA_0013_AREA.name) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.GenerateFFT(2.0f);
        FTBL.fft_fill_dist(FTBL.fft_fill_krn(23.529f, FTBL.generate_distance_falloff_row_major(2.082f, distance0013)));
        first = FTBL.fft_fill_krn(0.0564f, FTBL.generate_distance_falloff_kernel_8x8(0.94f, kernel0013First));
        second = FTBL.fft_fill_krn(0.035f, FTBL.generate_distance_falloff_kernel_8x8(0.952f, kernel0013Second));
        FTBL.fft_bins_load_pairwise_tables(first, second);
        FTBL.FFTUtil(0.002f, 0.002f);
        direction.x = 0.0598f;
        direction.y = 0.9768f;
        direction.z = 0.2054f;
        FTBL.set_lighting_flags(0.1f, &direction);
        FTBL.HLib_EnqueueTaskWithoutParameter(E0013WaveCallback, 3, 2, AREA_AUTHOR_TASK);
#else
        GenerateFFT(2.0f);
        FillFftDistance(FillFftKernel(23.52899932861328f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, distance0013)));
        void *const second = FillFftKernel(0.03500000014901161f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, kernel0013Second));
        void *const first = FillFftKernel(0.05640000104904175f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, kernel0013First));
        ApplyFft(first, second);
        FinalizeFft(0.0020000000949949026f, 0.0020000000949949026f);
        SetWaveLighting(0.1f, {0.059799999f, 0.97680002f, 0.2054f});
        HLib::EnqueueTaskWithoutParameter(E0013WaveCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
#endif
    }
    FTBL.EV_LoadCutsceneFromCurrentScene("0013A000");
    FTBL.EV_FreeAuthorPak(1, "0013");
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetLoadingBusy();
#else
    E0013EnvironmentNullsub();
#endif
#endif
}
void CleanupE0013(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EVT_UTL_FreeNPCChara(AREA_0013_CHARACTERS);
#endif
    FTBL.EV_FreeAuthorPak(0, "0013");
    FTBL.EV_ControlPackageResources(0, 6, "0013A", 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.DebugLogToFileRemoved("NPC_AREA1 : FREE\n");
#endif
    FTBL.npcSetAreaName(AREA_0013_ANP1, 0, 0.0f, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.HLib_DestroyTask(parent->self);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0013_AR02) {
#else
    if (g_currentMap.name == AREA_0013_AR02.name) {
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 220.42f, 100.2f, 341.63f, 0);
#else
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 220.52f, 100.2f, 341.33f, 0);
#else
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {220.52000427246094f, 100.19999694824219f, 341.3299865722656f}, 0};
        EVT_AT_SetPosRot(&position);
#endif
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 1.47f, 0.0f, 0);
#else
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 267, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.ForceLoadNpc(AREA_0013_WON);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.CheckWarmStartNpcRoute(AREA_0013_WON);
        FTBL.EV_NPC_NewLoadSw_0(1);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, 0);
#else
        mtwkRequestMotion(scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, MotionRegion::LowerBody);
#endif
#endif
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.SetPlayerControlEnabled(1);
#endif
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        parent->state = 4;
#endif
    } else {
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.FinishLoading();
#endif
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
// @verified - pd
void SetupE0013Characters(void) {
    Prepare0013Character(AREA_AUTHOR_RYO);
    Prepare0013Character(AREA_0013_CLJ);
    Prepare0013Character(AREA_0013_WON);
    Hide0013Prop(AREA_0013_A014);
}
// @verified - pd
void RestoreE0013Characters(void) {
    Restore0013Character(AREA_AUTHOR_RYO);
    Restore0013Character(AREA_0013_WON);
    Restore0013Character(AREA_0013_CLJ);
    Show0013Prop(AREA_0013_A014);
}
E0013_INLINE void Prepare0013Character(E0013CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0013_INLINE void Restore0013Character(E0013CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0013_INLINE void Hide0013Prop(E0013CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0013_INLINE void Show0013Prop(E0013CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
// @verified - p
void E0013WaveCallback(void *data) {
#if defined(SHENMUE_EVENT_OVERLAY)
    unsigned char *weather = (unsigned char *)FTBL.GetInterpolatedWeatherValues();
    unsigned int *colours = (unsigned int *)(weather + 0x34);
    float red = (int)((colours[7] >> 16) & 255) / 255.0f;
    float green = (int)((colours[7] >> 8) & 255) / 255.0f;
    float blue = (int)weather[0x50] / 255.0f;
    float specularRed = (int)((colours[5] >> 16) & 255) / 255.0f;
    float specularGreen = (int)((colours[5] >> 8) & 255) / 255.0f;
    float specularBlue = (int)weather[0x48] / 255.0f;
    void *model;
    float power;
    red *= 0.3607f;
    green *= 0.3725f;
    blue *= 0.4352f;
    specularRed *= 0.2549f;
    specularGreen *= 0.38f;
    specularBlue *= 0.2862f;
    power = 42.54f;
    FTBL.BuildLightingTableFrom(1, 1.0f, 1.0f, red, green, blue, specularRed, specularGreen, specularBlue, power, 1.0f);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    model = FTBL.GetMapModelRoot(17);
#else
    model = FTBL.GetMapModelRoot(15);
#endif
    FTBL.CalculateFieldCollision(*(void **)((unsigned char *)*(void **)((unsigned char *)model + 0x2c) + 0x28));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    model = FTBL.GetMapModelRoot(18);
#else
    model = FTBL.GetMapModelRoot(16);
#endif
    FTBL.TransformFieldCollisionForModel(model);
#else
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.36070001125335693f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.3725000023841858f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.4352000057697296f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.2549000084400177f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3799999952316284f,
            static_cast<float>(specular & 255U) / 255.0f * 0.28619998693466187f},
        42.540000915527344f, 1.0f);
    ApplyWaveTextureMeshResource(15);
    ApplyWaveReflectionResource(16);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0013_CHARACTERS[] = {AREA_0013_WON, AREA_0013_CLJ, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0013ScriptDefinition{.areaId = AREA_0013_AREA, .field04 = 0, .init = E0013Init, .main = E0013Main};
} // namespace shenmue::s2::areas
#endif
#undef E0013_INLINE
