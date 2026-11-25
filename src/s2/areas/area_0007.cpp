#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0007.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void E0007Main(void)
{
    EventDateTime time;
    HLibTask *task;
    E0007TaskParameter *work;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EventFunction870(0);
    FTBL.SetScenarioFollowCharacter(0);
    FTBL.EventFunction857(0);
#endif
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
#endif
    FTBL.GetCurrentDateTime((DateTime *)&time);
    task = FTBL.HLib_EnqueueTaskWithParameter(E0007MainCallback, 4, 11, sizeof(E0007TaskParameter), TASK_EVNT);
    work = (E0007TaskParameter *)FTBL.HLib_GetTaskParameter(task);
    work->self = task;
    work->state = 1;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    work->selection = time.hour >= 19;
#else
    work->selection = FTBL.EV_GetShenmueFlag(255) != 0;
#endif
    FTBL.EV_stepSetFlags(1, 0);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_SetPlayerFlag(12, 0x8b4, 1);
#endif
    ConfigureE0007Environment();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (e0007Characters[work->selection]) {
        EVT_UTL_SetNPCLowQualityHands(0);
        Queue_EVT_UTL_LoadNPCChara_3(e0007Characters[work->selection], work->self);
    } else
        CleanupE0007(work);
#else
    EVT_UTL_SetNPCLowQualityHands(0);
    Queue_EVT_UTL_LoadNPCChara_3(e0007DayCharacters, work->self);
#endif
}
#else
shen_name e0007DayCharacters[] = {
    SHEN_NAME4('J', 'O', 'P', '_'), SHEN_NAME4('S', 'H', 'I', '_'), SHEN_NAME4('U', 'N', 'H', '_'),
    SHEN_NAME4('G', 'O', 'I', '_'), SHEN_NAME4('H', 'T', 'K', '_'), INVALID_CHARACTER};
shen_name e0007NightCharacters[] = {SHEN_NAME4('J', 'O', 'P', '_'), INVALID_CHARACTER};
shen_name *e0007Characters[] = {e0007DayCharacters, e0007NightCharacters, 0};
static unsigned char e0007Distance[256], e0007FirstKernel[256], e0007SecondKernel[256];
static void *e0007Audio;

void E0007MainCallback(void *data)
{
    E0007TaskParameter *work = (E0007TaskParameter *)data;
    switch (work->state) {
    case 1: QueueE0007Loop(work); break;
    case 3: CleanupE0007(work); break;
    }
}

void QueueE0007Loop(E0007TaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0007LoopCallback, 4, 11, sizeof(E0007LoopParameter), TASK_EVNT);
    E0007LoopParameter *work = (E0007LoopParameter *)FTBL.HLib_GetTaskParameter(task);
    work->parent = parent;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    e0007Audio = FTBL.shenPlayAudioWithFlags("joy", 1);
#else
    e0007Audio = FTBL.shenPlayAudioWithFlags("0007C001", 1);
#endif
    SetupE0007Characters(work->parent->selection);
    FTBL.EV_authSceneCtrl(-14);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (parent->selection)
        FTBL.EV_authSceneCtrl(SHEN_NAME4('H', '0', '1', '0'));
    else
        FTBL.EV_authSceneCtrl(SHEN_NAME4('H', '0', '0', '0'));
#else
    if (parent->selection) {
        FTBL.DebugLogToFileRemoved("NIGHT_AUTH\n");
        FTBL.EV_authSceneCtrl(SHEN_NAME4('0', '0', '7', 'B'));
    } else {
        FTBL.DebugLogToFileRemoved("DEF_AUTH\n");
        FTBL.EV_authSceneCtrl(SHEN_NAME4('0', '0', '7', 'A'));
    }
#endif
    parent->state = 2;
}

void E0007LoopCallback(void *data)
{
    E0007LoopParameter *work = (E0007LoopParameter *)data;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == 0) {
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(RYO_CHARACTER), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('J','O','P','_')), 1);
    }
    if (work->parent->selection == 0 && frame == 341)
        FTBL.EV_SetPlayerFlag(12, 0x8b4, 0);
    else if (work->parent->selection == 0 && frame == 370) {
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('G','O','I','_')), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('H','T','K','_')), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('S','H','I','_')), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('U','N','H','_')), 1);
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('G','O','I','_'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('H','T','K','_'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('S','H','I','_'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('U','N','H','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('G','O','I','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('H','T','K','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('S','H','I','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('U','N','H','_'));
    } else if (work->parent->selection == 1 && frame == 0) {
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('G','O','I','_'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('H','T','K','_'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('S','H','I','_'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('U','N','H','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('G','O','I','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('H','T','K','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('S','H','I','_'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('U','N','H','_'));
    }
#endif
    if (FTBL.EV_authSceneCtrl(-1) == 0) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.hsFadeVoice(e0007Audio, 30);
#else
        FTBL.EV_EnsureCharacterDraw(SHEN_NAME4('G','O','I','_'));
        FTBL.EV_EnsureCharacterDraw(SHEN_NAME4('H','T','K','_'));
        FTBL.EV_EnsureCharacterDraw(SHEN_NAME4('S','H','I','_'));
        FTBL.EV_EnsureCharacterDraw(SHEN_NAME4('U','N','H','_'));
        FTBL.EV_EnableCharacterDisplay(SHEN_NAME4('G','O','I','_'));
        FTBL.EV_EnableCharacterDisplay(SHEN_NAME4('H','T','K','_'));
        FTBL.EV_EnableCharacterDisplay(SHEN_NAME4('S','H','I','_'));
        FTBL.EV_EnableCharacterDisplay(SHEN_NAME4('U','N','H','_'));
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(RYO_CHARACTER), 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('J','O','P','_')), 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('G','O','I','_')), 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('H','T','K','_')), 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('S','H','I','_')), 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SHEN_NAME4('U','N','H','_')), 0);
#endif
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0007Characters(work->parent->selection);
        work->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void ConfigureE0007Environment(void)
{
    EventDateTime time;
    const char *directory;
    Vector3 direction;
    void *first, *second;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    if (g_currentMap != SHEN_NAME4('A', 'R', '0', '2')) {
        FTBL.GetCurrentDateTime((DateTime *)&time);
        if (time.hour >= 19) {
            FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
            FTBL.PushGlobalLightingProgram();
            FTBL.CheckIsArgEquZero(1, 1);
        } else {
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
        }
    }
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    directory = FTBL.Filepath_Gen(0);
    FTBL.EV_ControlPackageResources(0, 0, (void *)directory, (int)"0007A", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "0007A", 0, 0);
#endif
    FTBL.EV_FreeAuthorPak(1, "0007");
    FTBL.EV_LoadCutsceneFromCurrentScene("0007A000");
    if (g_currentMap != SHEN_NAME4('A', 'R', '0', '2')) {
        FTBL.GenerateFFT(2.0f);
        FTBL.fft_fill_dist(FTBL.fft_fill_krn(23.529f, FTBL.generate_distance_falloff_row_major(2.082f, e0007Distance)));
        first = FTBL.fft_fill_krn(0.0564f, FTBL.generate_distance_falloff_kernel_8x8(0.94f, e0007FirstKernel));
        second = FTBL.fft_fill_krn(0.035f, FTBL.generate_distance_falloff_kernel_8x8(0.952f, e0007SecondKernel));
        FTBL.fft_bins_load_pairwise_tables(first, second);
        FTBL.FFTUtil(0.002f, 0.002f);
        direction.x = 0.0598f;
        direction.y = 0.9768f;
        direction.z = 0.2054f;
        FTBL.set_lighting_flags(0.1f, &direction);
        FTBL.HLib_EnqueueTaskWithoutParameter(E0007WaveCallback, 3, 2, TASK_EVNT);
    }
}

void CleanupE0007(E0007TaskParameter *parent)
{
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EVT_UTL_FreeNPCChara(e0007Characters[parent->selection]);
#else
    EVT_UTL_FreeNPCChara(e0007DayCharacters);
    FTBL.EV_SetPlayerFlag(12, 0x8b4, 0);
#endif
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_FreeAuthorPak(0, "0007");
    FTBL.EV_ControlPackageResources(0, 6, "0007A", 0, 0);
    FTBL.HLib_DestroyTask(parent->self);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EventFunction857(1);
#endif
    if (g_currentMap == SHEN_NAME4('A', 'R', '0', '2')) {
        EVT_AT_SetPos(RYO_CHARACTER, INVALID_CHARACTER, 224.88f, 100.0f, 327.6f, 0);
        EVT_AT_SetAng(RYO_CHARACTER, INVALID_CHARACTER, 0.0f, 141.47f, 0.0f, 0);
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

void SetupE0007Characters(short selection)
{
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(SHEN_NAME4('J','O','P','_')));
#endif
    PrepareE0007Character(RYO_CHARACTER);
    PrepareE0007Character(SHEN_NAME4('J', 'O', 'P', '_'));
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    HideE0007Prop(SHEN_NAME4('a','a','7','9'));
#endif
    if (selection == 0) {
        PrepareE0007Character(SHEN_NAME4('S', 'H', 'I', '_'));
        PrepareE0007Character(SHEN_NAME4('U', 'N', 'H', '_'));
        PrepareE0007Character(SHEN_NAME4('G', 'O', 'I', '_'));
        PrepareE0007Character(SHEN_NAME4('H', 'T', 'K', '_'));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        HideE0007Prop(SHEN_NAME4('a', '3', '9', '6'));
    } else
        HideE0007Prop(SHEN_NAME4('a', '4', '5', '6'));
#else
    }
#endif
}

void RestoreE0007Characters(short selection)
{
    RestoreE0007Character(RYO_CHARACTER);
    RestoreE0007Character(SHEN_NAME4('J', 'O', 'P', '_'));
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    ShowE0007Prop(SHEN_NAME4('a','a','7','9'));
#endif
    if (selection == 0) {
        RestoreE0007Character(SHEN_NAME4('S', 'H', 'I', '_'));
        RestoreE0007Character(SHEN_NAME4('U', 'N', 'H', '_'));
        RestoreE0007Character(SHEN_NAME4('G', 'O', 'I', '_'));
        RestoreE0007Character(SHEN_NAME4('H', 'T', 'K', '_'));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        ShowE0007Prop(SHEN_NAME4('a', '3', '9', '6'));
    } else
        ShowE0007Prop(SHEN_NAME4('a', '4', '5', '6'));
#else
    }
#endif
}

void PrepareE0007Character(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
void RestoreE0007Character(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
void HideE0007Prop(shen_name name) { FTBL.EV_SetCharacterAlphaEnabled(name, 0); }
void ShowE0007Prop(shen_name name) { FTBL.EV_SetCharacterAlphaEnabled(name, 1); }
void E0007WaveCallback(void *taskData)

{
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
    model = FTBL.GetMapModelRoot(17);
    FTBL.CalculateFieldCollision(*(void **)((unsigned char *)*(void **)((unsigned char *)model + 0x2c) + 0x28));
    model = FTBL.GetMapModelRoot(18);
    FTBL.TransformFieldCollisionForModel(model);
}
#endif
#else
#include "s2/debug.hpp"
#include "s2/areas/area_0007.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
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
std::array<std::byte, 0x100> distance0007{}, kernel0007First{}, kernel0007Second{};
void E0007MainCallback(void *);
void E0007LoopCallback(void *);
// @verified - p
void E0007Init() {
    distance0007.fill(std::byte{});
    kernel0007First.fill(std::byte{});
    kernel0007Second.fill(std::byte{});
}
// @verified - p
bool E0007CheckArgumentZero(std::int32_t value) {
    return value == 0;
}
// @verified - p
void E0007WaveCallback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.36070001125335693f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.3725000023841858f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.4352000057697296f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.2549000084400177f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3799999952316284f,
            static_cast<float>(specular & 255U) / 255.0f * 0.28619998693466187f},
        42.540000915527344f, 1.0f);
    ApplyWaveTextureMeshResource(17);
    ApplyWaveReflectionResource(18);
}
// @verified - p
void SetupE0007Environment() {
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    if (g_currentMap.name != AREA_0007_AR02.name) {
        DateTime time;
        shenGetCurrentDateTime(time);
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        } else {
            EV_lightLoad(AREA_0007_LIGHT);
            PushGlobalLightingProgram();
            (void)E0007CheckArgumentZero(1);
        }
    }
    EV_FreeAuthorPak(1, "0007");
    EV_LoadCutsceneFromCurrentScene("0007A000");
    if (g_currentMap.name != AREA_0007_AR02.name) {
        GenerateFFT(2.0f);
        FillFftDistance(FillFftKernel(23.52899932861328f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, distance0007)));
        void *const second = FillFftKernel(0.03500000014901161f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, kernel0007Second));
        void *const first = FillFftKernel(0.05640000104904175f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, kernel0007First));
        ApplyFft(first, second);
        FinalizeFft(0.0020000000949949026f, 0.0020000000949949026f);
        SetWaveLighting(0.1f, {0.059799999f, 0.97680002f, 0.2054f});
        HLib::EnqueueTaskWithoutParameter(E0007WaveCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
    }
}
// @verified - p
void E0007Main(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{0});
    SetNpcColdResetMode(0);
    SetPlayerControlEnabled(0);
    DateTime time;
    shenGetCurrentDateTime(time);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0007MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    parent->selection = EV_GetShenmueFlag(0xff) != 0;
    EV_stepSetFlags(0, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, AREA_0007_DUCK_GUTS_FLAG, 1);
    SetupE0007Environment();
    SetAreaNpcLowQualityHands(false);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0007_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0007Characters(std::int16_t selection) {
    SetLowQualityHands(ctrlGetScenarioChara(AREA_0007_JOP));
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_JOP), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0007_JOP, false);
    EV_SetCharacterAlphaEnabled(AREA_0007_AA39, false);
    if (selection == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_SHI), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_SHI, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_UNH), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_UNH, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_GOI), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_GOI, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_HTK), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_HTK, false);
    }
}
// @verified - p
void RestoreE0007Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_JOP), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0007_JOP, true);
    EV_SetCharacterAlphaEnabled(AREA_0007_AA39, true);
    if (selection == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_SHI), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_SHI, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_UNH), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_UNH, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_GOI), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_GOI, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_HTK), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_HTK, true);
    }
}
// @verified - p
void CleanupE0007(AreaAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0007_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, AREA_0007_DUCK_GUTS_FLAG, 0);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0007");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0007A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    SetNpcColdResetMode(1);
    if (g_currentMap.name == AREA_0007_AR02.name) {
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {224.8800048828125f, 100, 327.6000061035156f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 25753, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        sbtLoadProgram();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void QueueE0007Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0007LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    shenPlayAudioWithFlags("0007C001", 1);
    SetupE0007Characters(parent->selection);
    EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
    if (parent->selection != 0)
        EV_debug_log("0007\\e0007_et_0007.c", 194, "NIGHT_AUTH\n");
    else
        EV_debug_log("0007\\e0007_et_0007.c", 197, "DEF_AUTH\n");
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0007_AUTHORS[parent->selection != 0].name));
    parent->state = 2;
}
// @verified - p
void E0007MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0007Loop(parent);
        charaSetDisplayEnabledFlag(ctrlGetScenarioChara(AREA_0007_RCKO), 0);
    } else if (parent->state == 3) {
        CleanupE0007(parent);
        charaSetDisplayEnabledFlag(ctrlGetScenarioChara(AREA_0007_RCKO), 1);
    }
}
// @verified - p
void E0007LoopCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent;
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 0) {
        SetPluralControlFor(ctrlGetScenarioChara(AREA_AUTHOR_RYO), 1);
        SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_JOP), 1);
    }
    if (parent->selection == 0 && frame == 341)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, AREA_0007_DUCK_GUTS_FLAG, 0);
    if ((parent->selection == 0 && frame == 370) || (parent->selection == 1 && frame == 0)) {
        if (parent->selection == 0) {
            SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_GOI), 1);
            SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_HTK), 1);
            SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_SHI), 1);
            SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_UNH), 1);
        }
        EV_ToggleCharacterFlag(AREA_0007_GOI);
        EV_ToggleCharacterFlag(AREA_0007_HTK);
        EV_ToggleCharacterFlag(AREA_0007_SHI);
        EV_ToggleCharacterFlag(AREA_0007_UNH);
        EV_DisableCharacterDisplay(AREA_0007_GOI);
        EV_DisableCharacterDisplay(AREA_0007_HTK);
        EV_DisableCharacterDisplay(AREA_0007_SHI);
        EV_DisableCharacterDisplay(AREA_0007_UNH);
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    EV_EnsureCharacterDraw(AREA_0007_GOI);
    EV_EnsureCharacterDraw(AREA_0007_HTK);
    EV_EnsureCharacterDraw(AREA_0007_SHI);
    EV_EnsureCharacterDraw(AREA_0007_UNH);
    EV_EnableCharacterDisplay(AREA_0007_GOI);
    EV_EnableCharacterDisplay(AREA_0007_HTK);
    EV_EnableCharacterDisplay(AREA_0007_SHI);
    EV_EnableCharacterDisplay(AREA_0007_UNH);
    SetPluralControlFor(ctrlGetScenarioChara(AREA_AUTHOR_RYO), 0);
    SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_JOP), 0);
    SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_GOI), 0);
    SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_HTK), 0);
    SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_SHI), 0);
    SetPluralControlFor(ctrlGetScenarioChara(AREA_0007_UNH), 0);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0007Characters(parent->selection);
    parent->state = 3;
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0007ScriptDefinition{.areaId = AREA_0007_AREA, .field04 = 0, .init = E0007Init, .main = E0007Main};
} // namespace shenmue::s2::areas

#endif
