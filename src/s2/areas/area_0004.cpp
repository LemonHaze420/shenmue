#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0004.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void e0004_EVT_Auth_Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(e0004_EVT_Auth_Main_Callback, 4, 11, sizeof(Area0004EventWork), TASK_EVNT);
    Area0004EventWork *event = (Area0004EventWork *)FTBL.HLib_GetTaskParameter(task);
    event->task = task;
    event->state = 1;
    event->debugSelection = 0;
    FTBL.InitializeShipSpray();
    FTBL.HIKI_Enqueue();
    InitializeOpeningEnvironment();
    EVT_UTL_SetNPCLowQualityHands(0);
    Queue_EVT_UTL_LoadNPCChara_3(openingTemporaryCharacters, event->task);
}
#else
shen_name openingTemporaryCharacters[] = {
    SHEN_NAME4('F', 'T', '1', '_'), SHEN_NAME4('F', 'T', '2', '_'), SHEN_NAME4('F', 'T', '3', '_'), SHEN_NAME4('F', 'T', '4', '_'),
    SHEN_NAME4('F', 'T', '5', '_'), SHEN_NAME4('F', 'T', '6', '_'), SHEN_NAME4('F', 'T', '7', '_'), SHEN_NAME4('F', 'T', '8', '_'), INVALID_CHARACTER};
static int openingWavePhase = 0;
static unsigned char openingWaveDistance[0x100];
static unsigned char openingWaveFirstKernel[0x100];
static unsigned char openingWaveSecondKernel[0x100];
static unsigned int openingSmokeEffects[2];

void e0004_EVT_Auth_Main_Callback(void *taskData)
{
    Area0004EventWork *event = (Area0004EventWork *)taskData;
    unsigned int input;
    switch (event->state) {
    case 0:
        input = FTBL.EVT_GetControllerInput(0, 1);
        if (input == 0x400)
            event->state = 1;
        else if (input == 64) {
            if (--event->debugSelection < 0)
                event->debugSelection = 0;
        } else if (input == 128) {
            if (++event->debugSelection > 0)
                event->debugSelection = 0;
        }
        switch (event->debugSelection) {
        case 0:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A001.c\n");
            break;
        case 1:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A089.c\n");
            break;
        case 2:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A090.c\n");
            break;
        case 3:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A091.c\n");
            break;
        case 4:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A092.c\n");
            break;
        case 5:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A093.c\n");
            break;
        case 6:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A094.c\n");
            break;
        case 7:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A095.c\n");
            break;
        case 8:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A096.c\n");
            break;
        case 9:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A097.c\n");
            break;
        case 10:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0004A098.c\n");
            break;
        }
        FTBL.SetDebugScreenTextPosition(2, 5);
        FTBL.DebugLogToScreen("seqdata%d.avb\n", event->debugSelection);
        break;
    case 1:
        Queue_e0004_EVT_Auth_Loop_0004A000(event);
        break;
    case 3:
        CompleteOpening(event);
        break;
    }
}

void Queue_e0004_EVT_Auth_Loop_0004A000(Area0004EventWork *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(e0004_EVT_Auth_Loop_0004A000_Callback, 4, 11, sizeof(Area0004LoopWork), TASK_EVNT);
    Area0004LoopWork *child = (Area0004LoopWork *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->state = 0;
    SetupOpeningSmoke();
    FTBL.EV_LoadAudio("0004A000", 0);
    PrepareOpeningAuthorCharacters();
    FTBL.Queue_OP_StaffRoll();
    FTBL.EV_authSceneCtrl(parent->debugSelection);
    parent->state = 2;
}

void e0004_EVT_Auth_Loop_0004A000_Callback(void *taskData)
{
    Area0004LoopWork *child = (Area0004LoopWork *)taskData;
    Vector3 position;
    Vector3 smokePosition;
    Vector3 spawnVolume = {0.0f, 0.0545098074f, 0.0f};
    Vector3i rotation;
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (frame <= 2369) {
        FTBL.GetCharaPosition(AREA_0004_A892, -1, &position, 0);
        FTBL.GetScenarioCharacterEulerAngles(AREA_0004_A892, &rotation, 0);
        rotation.y += 0x8000;
        position.y = 95.83f;
        FTBL.AssignShipSprayPosition(&position);
        FTBL.AssignShipSprayYaw(rotation.y);
        position.y = 95.9f;
        FTBL.AssignHikiPosition(&position);
        FTBL.AssignHikiRotation(rotation.y);
        FTBL.GetCharaPosition(AREA_0004_A892, -1, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(AREA_0004_A892, &rotation, 0x40000000);
        smokePosition.x = 0.0f;
        smokePosition.y = 15.0f;
        smokePosition.z = 11.2f;
        FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &smokePosition);
        FTBL.SetParticleEffectPosition(0, openingSmokeEffects[0], &smokePosition, &spawnVolume);
        smokePosition.x = -0.04f;
        smokePosition.y = 15.0f;
        smokePosition.z = 20.97f;
        FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &smokePosition);
        FTBL.SetParticleEffectPosition(0, openingSmokeEffects[1], &smokePosition, &spawnVolume);
    }
    switch (frame) {
    case 1:
        position.x = 0.0f;
        position.y = 0.0f;
        position.z = 0.0f;
        FTBL.SetMapModelSlotTransform(16, &position, 0);
        position.y = -0.8f;
        FTBL.SetMapModelSlotTransform(1, &position, 0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case 2370:
        position.x = 1000.0f;
        FTBL.AssignHikiPosition(&position);
        FTBL.AssignShipSprayPosition(&position);
        FTBL.SuspendParticleEffect(0, openingSmokeEffects[0]);
        FTBL.SuspendParticleEffect(0, openingSmokeEffects[1]);
        FTBL.FreeParticleEffect(0, openingSmokeEffects[0]);
        FTBL.FreeParticleEffect(0, openingSmokeEffects[1]);
        openingSmokeEffects[0] = openingSmokeEffects[1] = AREA_0004_INVALID_EFFECT;
        break;
    case 2539:
        FTBL.AssignIMGMParams(FTBL.ctrlGetCharaByName(AREA_0004_A820), 1, -1, 0.5f, 0);
        break;
    case 2548:
        FTBL.AssignIMGMParams(FTBL.ctrlGetCharaByName(AREA_0004_A814), 1, -1, 0.5f, 0);
        FTBL.AssignIMGMParams(FTBL.ctrlGetCharaByName(AREA_0004_A816), 1, -1, 0.5f, 0);
        FTBL.AssignIMGMParams(FTBL.ctrlGetCharaByName(AREA_0004_A822), 1, -1, 0.5f, 0);
        break;
    case 2555:
        FTBL.AssignIMGMParams(FTBL.ctrlGetCharaByName(AREA_0004_A818), 1, -1, 0.5f, 0);
        break;
    case 2861:
        FTBL.SetObjectFlagByIndex(22, 1);
        break;
    case 2946:
        FTBL.SetObjectFlagByIndex(22, 0);
        break;
    }
    if ((frame >= 2362 && frame <= 2410) || (frame >= 2514 && frame <= 2714)) {
        FTBL.nlSetTexanimUV(0.0f, 0.055f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(17));
        FTBL.nlSetTexanimUV(0.0f, 0.027f);
        FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(18));
    }
    if (frame == 2706) {
        position.x = 0.0f;
        position.y = 1.15f;
        position.z = 0.0f;
        FTBL.SetMapModelSlotTransform(16, &position, 0);
    } else if (frame >= 2707 && frame <= 2779)
        UpdateOpeningWave();
    else if (frame == 2780) {
        position.x = 0.0f;
        position.y = 0.0f;
        position.z = 0.0f;
        FTBL.SetMapModelSlotTransform(16, &position, 0);
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreOpeningAuthorCharacters();
        FTBL.camCtrlSetFarDistance(0.0f);
        child->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void SetupOpeningSmoke(void)
{
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 spawnVolume = {0.0f, 0.0545098074f, 0.0f};
    Vector3 velocity = {0.0f, 0.0815686285f, 0.0f};
    Vector3 acceleration = {0.0f, 0.0545098074f, 0.0f};
    int i;
    for (i = 0; i < 2; i++) {
        openingSmokeEffects[i] = FTBL.CreateSmokeEffect(&position, &spawnVolume, 300, 0.018431373f, 0, 0);
        FTBL.SetParticleEffectVelocity(0, openingSmokeEffects[i], &velocity, &acceleration);
        FTBL.SetParticleEffectColour(0, openingSmokeEffects[i], 0, 3, 8, 5);
        FTBL.SetParticleEffectSize(0, openingSmokeEffects[i], 7.1764708f, 7.1764708f);
        FTBL.SetParticleEffectInitialScale(0, openingSmokeEffects[i], 0.28823531f);
        FTBL.SetSmokeEffectFalloff(openingSmokeEffects[i], 112);
        FTBL.SetSmokeEffectFadeFactor(openingSmokeEffects[i], 21);
        FTBL.SetSmokeEffectPhaseRate(openingSmokeEffects[i], 0.0f);
        FTBL.ResumeParticleEffect(0, openingSmokeEffects[i]);
    }
}

void UpdateOpeningWave(void)
{
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 loweredPosition = {0.0f, 0.0f, 0.0f};
    if (openingWavePhase <= 180) {
        position.y = FTBL.CalculateFixedSine((unsigned short)(int)((float)(openingWavePhase * 8192) / 45.0f)) * 0.7f;
        loweredPosition.y = position.y - 0.8f;
        openingWavePhase++;
    } else {
        openingWavePhase = 0;
        loweredPosition.y = -0.8f;
    }
    FTBL.SetMapModelSlotTransform(0, &position, 0);
    FTBL.SetMapModelSlotTransform(1, &loweredPosition, 0);
    FTBL.SetMapModelSlotTransform(2, &position, 0);
    FTBL.SetMapModelSlotTransform(3, &position, 0);
}

void InitializeOpeningEnvironment(void)
{
    EventDateTime dateTime;
    Vector3 direction;
    void *firstKernel;
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    dateTime.hour = 8;
    dateTime.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&dateTime);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
    FTBL.EV_SetPlayerFlag(12, 123, 1);
    FTBL.camCtrlSetFarDistance(400.0f);
    FTBL.GenerateFFT(2.0f);
    FTBL.fft_fill_dist(FTBL.fft_fill_krn(10.0f, FTBL.generate_distance_falloff_row_major(2.082f, openingWaveDistance)));
    firstKernel = FTBL.fft_fill_krn(0.0564f, FTBL.generate_distance_falloff_kernel_8x8(0.94f, openingWaveFirstKernel));
    FTBL.fft_bins_load_pairwise_tables(firstKernel, FTBL.fft_fill_krn(0.035f, FTBL.generate_distance_falloff_kernel_8x8(0.952f, openingWaveSecondKernel)));
    FTBL.FFTUtil(0.001f, 0.001f);
    direction.x = 0.0598f;
    direction.y = 0.9768f;
    direction.z = 0.2054f;
    FTBL.set_lighting_flags(0.1f, &direction);
    FTBL.HLib_EnqueueTaskWithoutParameter(e0004_ET_WAVE_AR00_Callback, 3, 2, AREA_0004_WAVE);
    FTBL.SetMapModelVertexShaderType(16, 1);
    FTBL.SetObjectFlagByIndex(22, 0);
}

void CompleteOpening(Area0004EventWork *event)
{
    EventDateTime dateTime;
    if (openingSmokeEffects[0] != AREA_0004_INVALID_EFFECT) {
        FTBL.SuspendParticleEffect(0, openingSmokeEffects[0]);
        FTBL.FreeParticleEffect(0, openingSmokeEffects[0]);
    }
    if (openingSmokeEffects[1] != AREA_0004_INVALID_EFFECT) {
        FTBL.SuspendParticleEffect(0, openingSmokeEffects[1]);
        FTBL.FreeParticleEffect(0, openingSmokeEffects[1]);
    }
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    dateTime.hour = 13;
    dateTime.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&dateTime);
    EVT_UTL_FreeNPCChara(openingTemporaryCharacters);
    FTBL.HLib_DestroyTask(event->task);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0004_AR01, 0);
}

void PrepareOpeningAuthorCharacters(void)
{
    int i;
    PrepareOpeningAuthorCharacter(AREA_0004_AA87);
    PrepareOpeningAuthorCharacter(AREA_0004_AA88);
    PrepareOpeningAuthorCharacter(AREA_0004_AA90);
    PrepareOpeningAuthorCharacter(AREA_0004_AA91);
    PrepareOpeningAuthorCharacter(AREA_0004_AA92);
    PrepareOpeningAuthorCharacter(AREA_0004_A892);
    PrepareOpeningAuthorCharacter(AREA_0004_A259);
    PrepareOpeningAuthorCharacter(AREA_0004_A814);
    PrepareOpeningAuthorCharacter(AREA_0004_A816);
    PrepareOpeningAuthorCharacter(AREA_0004_A818);
    PrepareOpeningAuthorCharacter(AREA_0004_A820);
    PrepareOpeningAuthorCharacter(AREA_0004_A822);
    PrepareOpeningAuthorCharacter(AREA_0004_A954);
    PrepareOpeningAuthorCharacter(AREA_0004_AA08);
    PrepareOpeningAuthorCharacter(AREA_0004_AA10);
    PrepareOpeningAuthorCharacter(AREA_0004_A892);
    PrepareOpeningAuthorCharacter(AREA_0004_A796);
    for (i = 0; openingTemporaryCharacters[i] != INVALID_CHARACTER; i++)
        PrepareOpeningTemporaryCharacter(openingTemporaryCharacters[i]);
}

void RestoreOpeningAuthorCharacters(void)
{
    int i;
    RestoreOpeningAuthorCharacter(AREA_0004_A892);
    RestoreOpeningAuthorCharacter(AREA_0004_AA87);
    RestoreOpeningAuthorCharacter(AREA_0004_AA88);
    RestoreOpeningAuthorCharacter(AREA_0004_AA90);
    RestoreOpeningAuthorCharacter(AREA_0004_AA91);
    RestoreOpeningAuthorCharacter(AREA_0004_AA92);
    RestoreOpeningAuthorCharacter(AREA_0004_A892);
    RestoreOpeningAuthorCharacter(AREA_0004_A259);
    RestoreOpeningAuthorCharacter(AREA_0004_A814);
    RestoreOpeningAuthorCharacter(AREA_0004_A816);
    RestoreOpeningAuthorCharacter(AREA_0004_A818);
    RestoreOpeningAuthorCharacter(AREA_0004_A820);
    RestoreOpeningAuthorCharacter(AREA_0004_A822);
    RestoreOpeningAuthorCharacter(AREA_0004_A954);
    RestoreOpeningAuthorCharacter(AREA_0004_AA08);
    RestoreOpeningAuthorCharacter(AREA_0004_AA10);
    RestoreOpeningAuthorCharacter(AREA_0004_A892);
    RestoreOpeningAuthorCharacter(AREA_0004_A796);
    for (i = 0; openingTemporaryCharacters[i] != INVALID_CHARACTER; i++)
        RestoreOpeningTemporaryCharacter(openingTemporaryCharacters[i]);
}

void PrepareOpeningTemporaryCharacter(shen_name character)
{
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void RestoreOpeningTemporaryCharacter(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void PrepareOpeningAuthorCharacter(shen_name character)
{
    FTBL.EV_EnsureCharacterDraw(character);
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void RestoreOpeningAuthorCharacter(shen_name character)
{
    FTBL.EV_ToggleCharacterFlag(character);
    FTBL.EV_DisableCharacterDisplay(character);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void e0004_ET_WAVE_AR00_Callback(void *taskData)
{
    Area0004WaveColours *weather = (Area0004WaveColours *)((char *)FTBL.GetInterpolatedWeatherValues() + 52);
    float diffuseRed = (unsigned char)(weather->diffuse >> 16) / 255.0f;
    float diffuseGreen = (unsigned char)(weather->diffuse >> 8) / 255.0f;
    float diffuseBlue = (unsigned char)weather->diffuse / 255.0f;
    float specularRed = (unsigned char)(weather->specular >> 16) / 255.0f;
    float specularGreen = (unsigned char)(weather->specular >> 8) / 255.0f;
    float specularBlue = (unsigned char)weather->specular / 255.0f;
    float shininess;
    void *root;
    diffuseRed *= 0.3607f;
    diffuseGreen *= 0.3725f;
    diffuseBlue *= 0.4352f;
    specularRed *= 0.2549f;
    specularGreen *= 0.38f;
    specularBlue *= 0.2862f;
    shininess = 42.54f;
    FTBL.BuildLightingTableFrom(1, 1.0f, 1.0f, diffuseRed, diffuseGreen, diffuseBlue, specularRed, specularGreen, specularBlue, shininess, 1.0f);
    root = FTBL.GetMapModelRoot(16);
    FTBL.TransformFieldCollisionForModel(root);
    FTBL.nlSetTexanimUV(0.00035f, 0.0003f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(16));
}
#endif
#else
#include "s2/areas/area_0004.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hiki.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_image.hpp"
#include "s2/mt7.hpp"
#include "s2/motion_work.hpp"
#include "s2/scenario_characters.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ship_spray.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
namespace {

struct EventTaskParameter {
    HLib::Task *task;
    std::int32_t debugSelection;
    std::int16_t state;
    std::int16_t padding0e;
};

struct AuthorLoopTaskParameter {
    EventTaskParameter *parent;
    std::int16_t state;
    std::byte reserved0a[6];
};

static_assert(sizeof(EventTaskParameter) == 0x10);
static_assert(offsetof(EventTaskParameter, debugSelection) == 0x08);
static_assert(offsetof(EventTaskParameter, state) == 0x0c);
static_assert(sizeof(AuthorLoopTaskParameter) == 0x10);
static_assert(offsetof(AuthorLoopTaskParameter, state) == 0x08);

constexpr std::uint32_t invalidEffect = 0xffffffffU;
std::array<std::uint32_t, 2> openingSmokeEffects{invalidEffect, invalidEffect};
std::int32_t openingWavePhase{};
std::array<std::uint32_t, 16> openingCharacterTextureFrames{};
std::array<std::uint32_t *, 16> openingCharacterTextureTargets{};
std::array<std::byte, 0x100> openingWaveDistance{};
std::array<std::byte, 0x100> openingWaveFirstKernel{};
std::array<std::byte, 0x100> openingWaveSecondKernel{};

constexpr shen_name a892{SHEN_NAME(a892)};
constexpr shen_name aa87{SHEN_NAME(aa87)};
constexpr shen_name aa88{SHEN_NAME(aa88)};
constexpr shen_name aa90{SHEN_NAME(aa90)};
constexpr shen_name aa91{SHEN_NAME(aa91)};
constexpr shen_name aa92{SHEN_NAME(aa92)};
constexpr shen_name a259{SHEN_NAME(a259)};
constexpr shen_name a814{SHEN_NAME(a814)};
constexpr shen_name a816{SHEN_NAME(a816)};
constexpr shen_name a818{SHEN_NAME(a818)};
constexpr shen_name a820{SHEN_NAME(a820)};
constexpr shen_name a822{SHEN_NAME(a822)};
constexpr shen_name a954{SHEN_NAME(a954)};
constexpr shen_name aa08{SHEN_NAME(aa08)};
constexpr shen_name aa10{SHEN_NAME(aa10)};
constexpr shen_name a796{SHEN_NAME(a796)};
constexpr std::array<shen_name, 8> openingTemporaryCharacters{{
    SHEN_NAME(FT1_),
    SHEN_NAME(FT2_),
    SHEN_NAME(FT3_),
    SHEN_NAME(FT4_),
    SHEN_NAME(FT5_),
    SHEN_NAME(FT6_),
    SHEN_NAME(FT7_),
    SHEN_NAME(FT8_),
}};

constexpr std::array<shen_name, 18> openingAuthorCharacters{{
    a892,
    aa87,
    aa88,
    aa90,
    aa91,
    aa92,
    a892,
    a259,
    a814,
    a816,
    a818,
    a820,
    a822,
    a954,
    aa08,
    aa10,
    a892,
    a796,
}};

void CaptureOpeningCharacterTextures() {
    const MapControlData *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *character = GetScenarioCharacterNode(control->scenarioTask, aa87);
    MT7Node *node = mtGetTransform(static_cast<MT7Node *>(character->model));
    std::int32_t count = 0;
    for (; node && count < 16; node = mtGetSibling(node)) {
        const std::int32_t sample = (static_cast<std::int16_t>(node->id) - 152) % 16;
        if (sample >= 0) {
            std::uint32_t *const texture = FindSpringTextureCommand(node, 0);
            ++count;
            openingCharacterTextureFrames[sample] = *texture;
        }
    }
    const std::array<shen_name, 5> characters{aa87, aa88, aa90, aa91, aa92};
    for (std::size_t index = 0; index < characters.size(); ++index) {
        control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = GetScenarioCharacterNode(control->scenarioTask, characters[index]);
        node = mtGetTransform(static_cast<MT7Node *>(character->model));
        for (; node; node = mtGetSibling(node)) {
            if ((static_cast<std::int16_t>(node->id) - 152) % 16 == 0) {
                openingCharacterTextureTargets[index] = FindSpringTextureCommand(node, 0);
                break;
            }
        }
    }
}

void SetOpeningTime(std::int8_t hour) {
    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
    dateTime.hour = hour;
    dateTime.minute = 0;
    SetCurrentDateTimeTo(dateTime);
}

void RestoreOpeningAuthorCharacters() {
    for (const shen_name character : openingAuthorCharacters) {
        EV_ToggleCharacterFlag(character);
        EV_DisableCharacterDisplay(character);
        EV_SetCharacterAlphaEnabled(character, true);
    }
    for (const shen_name character : openingTemporaryCharacters) {
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
        EV_SetCharacterAlphaEnabled(character, true);
    }
}
void PrepareOpeningAuthorCharacters() {
    for (std::size_t index = 1; index < openingAuthorCharacters.size(); ++index) {
        const shen_name character = openingAuthorCharacters[index];
        EV_EnsureCharacterDraw(character);
        EV_EnableCharacterDisplay(character);
        EV_SetCharacterAlphaEnabled(character, false);
    }
    for (const shen_name character : openingTemporaryCharacters) {
        EV_EnableCharacterDisplay(character);
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
    }
}

void SetupOpeningSmoke() {
    const Vector3 position{0.0f, 0.0f, 0.0f};
    const Vector3 spawnVolume{0.0f, 0.0545098074f, 0.0f};
    const Vector3 velocity{0.0f, 0.0815686285f, 0.0f};
    const Vector3 acceleration{0.0f, 0.0545098074f, 0.0f};
    for (std::uint32_t &effect : openingSmokeEffects) {
        effect = static_cast<std::uint32_t>(CreateSmokeEffect(position, spawnVolume, 300, 0.018431372940540314f, shen_name{}, 0));
        SetParticleEffectVelocity(0, effect, velocity, acceleration);
        SetParticleEffectColour(0, effect, 0, 3, 8, 5);
        SetParticleEffectSize(0, effect, 7.176470756530762f, 7.176470756530762f);
        SetParticleEffectInitialScale(0, effect, 0.28823530673980713f);
        SetSmokeEffectFalloff(effect, 112);
        SetSmokeEffectFadeFactor(effect, 21);
        SetSmokeEffectPhaseRate(effect, 0.0f);
        ResumeParticleEffect(0, effect);
    }
}


void e0004_ET_WAVE_AR00_Callback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f + 0.699999988079071f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f + 0.699999988079071f,
            static_cast<float>(diffuse & 255U) / 255.0f - 0.023000000044703484f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f + 0.3100000023841858f, static_cast<float>((specular >> 8) & 255U) / 255.0f + 0.3100000023841858f,
            static_cast<float>(specular & 255U) / 255.0f - 0.6520000100135803f},
        42.540000915527344f, 1.0f);
    SetTextureAnimationUV(0.000699999975040555f, 0.0003000000142492354f);
    ApplyTextureAnimationResource(16);
    SetMapModelSlotEnabled(16, 0);
    AdvanceWaterReflectionSet();
}

void InitializeOpeningEnvironment() {
    SetOpeningTime(8);
    SetPlayerControlEnabled(0);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x7b, 1);
    camCtrlSetFarDistance(400.0f);
    GenerateFFT(2.0f);
    FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(2.082f, openingWaveDistance)));
    void *const secondKernel = FillFftKernel(0.035f, GenerateDistanceFalloffKernel8x8(0.952f, openingWaveSecondKernel));
    void *const firstKernel = FillFftKernel(0.0564f, GenerateDistanceFalloffKernel8x8(0.94f, openingWaveFirstKernel));
    ApplyFft(firstKernel, secondKernel);
    FinalizeFft(0.001f, 0.001f);
    SetWaveLighting(0.1f, Vector3{0.0598f, 0.97680002f, 0.2054f});
    HLib::EnqueueTaskWithoutParameter(e0004_ET_WAVE_AR00_Callback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(wave)));
    EV_SetEntityVertexShaderType(16, 1);
    EV_SetObjectFlagByIndex(22, 0);
}

void UpdateOpeningWave() {
    Vector3 position{};
    Vector3 loweredPosition{0.0f, -0.8f, 0.0f};
    if (openingWavePhase > 180) {
        openingWavePhase = 0;
    } else {
        const auto angle = static_cast<std::int16_t>(static_cast<std::int32_t>((static_cast<float>(openingWavePhase) * 8192.0f) / 45.0f));
        position.y = sin_fixed16(angle) * 0.7f;
        loweredPosition.y = position.y - 0.8f;
        ++openingWavePhase;
    }
    SetMapModelSlotTransform(0, &position, 0);
    SetMapModelSlotTransform(1, &loweredPosition, 0);
    SetMapModelSlotTransform(2, &position, 0);
    SetMapModelSlotTransform(3, &position, 0);
}

void UpdateOpeningCharacterEffects(std::uint16_t frame) {
    EV_SetGlobalShadowLightingEnabled(true);
    if ((static_cast<std::uint16_t>(frame - 2180U) <= 0xb4U) || (static_cast<std::uint16_t>(frame - 2606U) <= 0x63U)) {
        EV_SetGlobalShadowLightingEnabled(false);
    }
    if (frame == 2861U) {
        EV_SetObjectFlagByIndex(22, 1);
    } else if (frame == 2946U) {
        EV_SetObjectFlagByIndex(22, 0);
    }
    if (frame <= 2385U) {
        Vector3 position{};
        Vector3i rotation{};
        EV_GetCharaPosition(a892, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(a892, &rotation, EventTransformFlags::Local);
        rotation.y += 0x8000;
        position.y = 95.830002f;
        AssignShipSprayPosition(position);
        AssignShipSprayYaw(rotation.y);
        position.y = 95.900002f;
        AssignHikiPosition(position);
        AssignHikiRotation(rotation.y);

        Vector3 recursivePosition{};
        Vector3i recursiveRotation{};
        EV_GetCharaPosition(a892, CHARACTER_TRANSLATION_ROOT, &recursivePosition, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(a892, &recursiveRotation, EventTransformFlags::World);
        const Vector3 smokeSpawnVolume{0.0f, 0.0545098074f, 0.0f};
        Vector3 smokePosition{0.0f, 15.0f, 11.2f};
        scenApplyTransRot(&recursivePosition, &recursiveRotation, &smokePosition);
        SetParticleEffectPosition(0, openingSmokeEffects[0], &smokePosition, &smokeSpawnVolume);
        smokePosition = Vector3{-0.04f, 15.0f, 20.969999f};
        scenApplyTransRot(&recursivePosition, &recursiveRotation, &smokePosition);
        SetParticleEffectPosition(0, openingSmokeEffects[1], &smokePosition, &smokeSpawnVolume);

        Vector3 aa87Position{};
        Vector3 aa90Position{};
        EV_GetCharaPosition(aa87, CHARACTER_TRANSLATION_ROOT, &aa87Position, EventTransformFlags::World);
        EV_GetCharaPosition(aa90, CHARACTER_TRANSLATION_ROOT, &aa90Position, EventTransformFlags::World);
        aa90Position.y = aa87Position.y;
        EV_AT_SetPos(aa90, EventTransformFlags::World | EventTransformFlags::AllAxes, aa90Position);
        for (std::size_t index = 0; index < OPENING_TEXTURE_PHASE_OFFSETS.size(); ++index) {
            if (openingCharacterTextureTargets[index]) {
                const std::uint32_t texture = openingCharacterTextureFrames[(frame + OPENING_TEXTURE_PHASE_OFFSETS[index]) % 16];
                if (texture)
                    *openingCharacterTextureTargets[index] = texture;
            }
        }
    }
    if (frame == 1U) {
        SetParticleEffectColour(0, openingSmokeEffects[0], 0, 65, 65, 65);
        SetParticleEffectColour(0, openingSmokeEffects[1], 0, 65, 65, 65);
        Vector3 position{};
        SetMapModelSlotTransform(16, &position, 0);
        position.y = -0.8f;
        SetMapModelSlotTransform(1, &position, 0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetOpeningTime(7);
    } else if (frame == 2370U) {
        Vector3 position{1000.0f, 0.0f, 0.0f};
        AssignHikiPosition(position);
        AssignShipSprayPosition(position);
        SuspendParticleEffect(0, openingSmokeEffects[0]);
        SuspendParticleEffect(0, openingSmokeEffects[1]);
        FreeParticleEffect(0, openingSmokeEffects[0]);
        FreeParticleEffect(0, openingSmokeEffects[1]);
        openingSmokeEffects.fill(invalidEffect);
    } else if (frame == 2539U) {
        (void)AssignImageModuleParameters(ctrlGetCharaByName(a820), 1, -1, 0.5f, 0);
    } else if (frame == 2548U) {
        (void)AssignImageModuleParameters(ctrlGetCharaByName(a814), 1, -1, 0.5f, 0);
        (void)AssignImageModuleParameters(ctrlGetCharaByName(a816), 1, -1, 0.5f, 0);
        (void)AssignImageModuleParameters(ctrlGetCharaByName(a822), 1, -1, 0.5f, 0);
    } else if (frame == 2555U) {
        (void)AssignImageModuleParameters(ctrlGetCharaByName(a818), 1, -1, 0.5f, 0);
    }
    if ((frame >= 2362U && frame <= 2410U) || (frame >= 2514U && frame <= 2714U)) {
        EV_SetObjectFlagByIndex(17, 0);
        EV_SetObjectFlagByIndex(18, 0);
        AdvanceSpringTextureAnimation();
    }
    if (frame == 2706U) {
        const Vector3 position{0.0f, 1.15f, 0.0f};
        SetMapModelSlotTransform(16, &position, 0);
    } else if (frame >= 2707U && frame <= 2779U) {
        UpdateOpeningWave();
    } else if (frame == 2780U) {
        const Vector3 position{};
        SetMapModelSlotTransform(16, &position, 0);
    }
}

void CompleteOpening(EventTaskParameter *event) {
    for (std::uint32_t &effect : openingSmokeEffects) {
        if (effect == invalidEffect)
            continue;
        SuspendParticleEffect(0, effect);
        FreeParticleEffect(0, effect);
        effect = invalidEffect;
    }
    SetOpeningTime(13);
    EV_CleanupOpeningTemporaryCharacters();
    EV_CleanupMapControlTask();
    HLib::DestroyTask(event->task);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(AR01), 0);
    EV_FinalizeOpeningAix();
    EV_SetGlobalShadowLightingEnabled(true);
}

void e0004_EVT_Auth_Loop_0004A000_Callback(void *taskData) {
    auto *const child = static_cast<AuthorLoopTaskParameter *>(taskData);
    if (!child || !child->parent) {
        HLib::DestroyCurrentTask();
        return;
    }
    EventTaskParameter *const parent = child->parent;
    if (Author_GetLastMapInfoLoadResult() != AuthorMapInfoLoadResult::Started) {
        return;
    }
    const std::uint16_t frame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    UpdateOpeningCharacterEffects(frame);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreOpeningAuthorCharacters();
        camCtrlSetFarDistance(0.0f);
        parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}

void Queue_e0004_EVT_Auth_Loop_0004A000(EventTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(e0004_EVT_Auth_Loop_0004A000_Callback, 4, 0x0b, sizeof(AuthorLoopTaskParameter), SHEN_NAME(EVNT));
    auto *const child = static_cast<AuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    if (!parent || !child)
        return;
    child->parent = parent;
    child->state = 0;
    SetupOpeningSmoke();
    if (IsAuthorMultichannelAudioEnabled() && IsAuthorMultichannelAudioActive())
        StartAuthorMultichannelAudio(0);
    else
        EV_LoadAudio("0004A000", 0);
    PrepareOpeningAuthorCharacters();
    Queue_OP_StaffRoll();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->debugSelection));
    parent->state = 2;
}

void e0004_EVT_Auth_Main_Callback(void *taskData) {
    auto *const event = static_cast<EventTaskParameter *>(taskData);
    if (!event)
        return;
    if (event->state == 1) {
        Queue_e0004_EVT_Auth_Loop_0004A000(event);
    } else if (event->state == 3) {
        CompleteOpening(event);
    } else if (event->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == PAD_BUTTON_X) {
            event->state = 1;
        } else if (input == PAD_TRIGGER_LEFT) {
            if (--event->debugSelection < 0)
                event->debugSelection = 0;
        } else if (input == PAD_TRIGGER_RIGHT) {
            if (++event->debugSelection > 0)
                event->debugSelection = 0;
        }
        if (event->debugSelection >= 0 && event->debugSelection < static_cast<std::int32_t>(OPENING_AUTHOR_DEBUG_NAMES.size())) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(OPENING_AUTHOR_DEBUG_NAMES[event->debugSelection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", event->debugSelection);
    }
}
void e0004_EVT_Auth_Init() {
    openingSmokeEffects.fill(invalidEffect);
    openingWavePhase = 0;
    ResetAuthorAudioState();
    EV_AUVW_Initialize(0);
    if (IsAuthorMultichannelAudioEnabled()) {
        if (EV_InitializeOpeningAix()) {
            SetSamplePlaybackSuppressed(1);
            SetAuthorStreamingAudioEnabled(0);
        }
    }
}

void e0004_EVT_Auth_Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(e0004_EVT_Auth_Main_Callback, 4, 0x0b, sizeof(EventTaskParameter), SHEN_NAME(EVNT));
    auto *const event = static_cast<EventTaskParameter *>(HLib::GetTaskParameter(task));
    if (!event)
        return;
    event->task = task;
    event->debugSelection = 0;
    event->state = 1;

    InitializeShipSpray();
    HIKI_Enqueue();
    InitializeOpeningEnvironment();
    EV_InitializeOpeningTemporaryCharacters(task);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(TASK_NPC_LOAD.name));
    InitializeWaterReflectionSet();
    InitializeSpringTextureAnimation(23);
    CaptureOpeningCharacterTextures();
}

} // namespace

const ScriptAreaDefinition area0004ScriptDefinition{SHEN_NAME(0004), 0, e0004_EVT_Auth_Init, e0004_EVT_Auth_Main};

} // namespace shenmue::s2::areas
#endif
