#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0059.hpp"
#include "s2/event_queued_dc.h"

#if defined(SHENMUE_EVENT_ENTRY)
void E0059Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0059MainCallback, 4, 11, sizeof(AreaAuthorTaskParameter), TASK_EVNT);
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0059A.MOT");
    parent->state = 1;
    parent->selection = 0;
    SetupE0059Environment();
    Queue_EVT_UTL_LoadNPCChara_3(area0059Characters, parent->self);
}
#else
shen_name area0059Characters[] = {AREA_0059_WON, INVALID_CHARACTER};
HarborFloatingModel harborFloatingModels0059[] = {{5, {132.1f, 93.928f, 492.16f}}, {6, {132.1f, 93.928f, 524.16f}}, {7, {132.1f, 93.928f, 556.16f}},
    {8, {100.1f, 93.928f, 524.16f}}, {9, {100.1f, 93.928f, 556.16f}}, {-1, {0.0f, 0.0f, 0.0f}}};
static unsigned char distance0059[0x100];
static unsigned char kernel0059First[0x100];
static unsigned char kernel0059Second[0x100];

void E0059MainCallback(void *taskData)
{
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)taskData;
    switch (parent->state) {
    case 1:
        QueueE0059Author(parent);
        break;
    case 3:
        CleanupE0059(parent);
        break;
    }
}

void QueueE0059Author(AreaAuthorTaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0059LoopCallback, 4, 11, sizeof(AreaAuthorLoopTaskParameter), TASK_EVNT);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_spmBeginSampleExecute(0);
    SetupE0059Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void E0059LoopCallback(void *taskData)
{
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)taskData;
    switch ((short)FTBL.EV_authSceneCtrl(-6)) {
    case 1:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case 1731:
        FTBL.SetChildObjectFlagByIndex(FTBL.ctrlGetCharaByName(RYO_CHARACTER), 3);
        break;
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetChildObjectFlagByIndex(FTBL.ctrlGetCharaByName(RYO_CHARACTER), 0);
        RestoreE0059Characters();
        child->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void SetupE0059Environment(void)
{
    Vector3 direction;
    const float *first;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.EV_FreeAuthorPak(1, "0059");
    FTBL.GenerateFFT(2.0f);
    FTBL.fft_fill_dist(FTBL.fft_fill_krn(10.0f, FTBL.generate_distance_falloff_row_major(1.717f, distance0059)));
    first = (const float *)(FTBL.fft_fill_krn(0.061f, FTBL.generate_distance_falloff_kernel_8x8(0.376f, kernel0059First)));
    FTBL.fft_bins_load_pairwise_tables(
        first, (const float *)(FTBL.fft_fill_krn(0.073f, FTBL.generate_distance_falloff_kernel_8x8(0.647f, kernel0059Second))));
    FTBL.FFTUtil(0.001f, 0.001f);
    direction.x = -0.1632f;
    direction.y = 0.9818f;
    direction.z = -0.0969f;
    FTBL.set_lighting_flags(0.1f, &direction);
    FTBL.HLib_EnqueueTaskWithoutParameter(HarborAuthorWaveCallback, 3, 2, TASK_EVNT);
    FTBL.SetMapModelVertexShaderType(3, 1);
    FTBL.SetMapModelVertexShaderType(12, 1);
    FTBL.HLib_EnqueueTaskWithoutParameter(HarborAuthorShip0059Callback, 3, 2, TASK_EVNT);
}

void CleanupE0059(AreaAuthorTaskParameter *parent)
{
    AdvanceSleepToMorning();
    EVT_UTL_FreeNPCChara(area0059Characters);
    FTBL.EV_FreeAuthorPak(0, "0059");
    FTBL.DeleteMotion(parent->motion);
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0059_AB00, 51);
}

void SetupE0059Characters(void)
{
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorCharacter(AREA_0059_WON);
    HideAuthorProp(AREA_0059_A021);
    HideAuthorProp(AREA_0059_A126);
    HideAuthorProp(AREA_0059_A005);
    HideAuthorProp(AREA_0059_A020);
    HideAuthorProp(AREA_0059_A103);
    HideAuthorProp(AREA_0059_A022);
}

void RestoreE0059Characters(void)
{
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorCharacter(AREA_0059_WON);
    ShowAuthorProp(AREA_0059_A021);
    ShowAuthorProp(AREA_0059_A126);
    ShowAuthorProp(AREA_0059_A005);
    ShowAuthorProp(AREA_0059_A020);
    ShowAuthorProp(AREA_0059_A103);
    ShowAuthorProp(AREA_0059_A022);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"

void HarborAuthorWaveCallback(void *taskData)
{
    HarborWeatherColours *colours = &((HarborWeatherValues *)FTBL.GetInterpolatedWeatherValues())->colours;
    float diffuseRed = (unsigned char)(colours->diffuse >> 16) / 255.0f;
    float diffuseGreen = (unsigned char)(colours->diffuse >> 8) / 255.0f;
    float diffuseBlue = (unsigned char)colours->diffuse / 255.0f;
    float specularRed = (unsigned char)(colours->specular >> 16) / 255.0f;
    float specularGreen = (unsigned char)(colours->specular >> 8) / 255.0f;
    float specularBlue = (unsigned char)colours->specular / 255.0f;
    float power;
    void *model;
    diffuseRed *= 0.3607f;
    diffuseGreen *= 0.3725f;
    diffuseBlue *= 0.4352f;
    specularRed *= 0.2549f;
    specularGreen *= 0.38f;
    specularBlue *= 0.2862f;
    power = 42.54f;
    FTBL.BuildLightingTableFrom(1, 1.0f, 1.0f, diffuseRed, diffuseGreen, diffuseBlue, specularRed, specularGreen, specularBlue, power, 1.0f);
    FTBL.CalculateFieldCollision(((HarborMapModelRoot *)FTBL.GetMapModelRoot(12))->model->collision);
    model = FTBL.GetMapModelRoot(3);
    FTBL.TransformFieldCollisionForModel(model);
    FTBL.nlSetTexanimUV(-0.001f, 0.001f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(3));
}

void HarborAuthorShip0059Callback(void *taskData)
{
    FTBL.ApplyMapModelTransforms(2, harborFloatingModels0059);
}
#endif
#else
#include "s2/areas/area_0059.hpp"
#include "s2/areas/harbor_author_event.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include <array>
#include <bit>
namespace shenmue::s2::areas {
namespace {
std::array<std::byte, 0x100> distance0059{}, kernel0059First{}, kernel0059Second{};
void E0059MainCallback(void *);
void E0059LoopCallback(void *);
// @verified - p
void E0059Init() {
    distance0059.fill(std::byte{});
    kernel0059First.fill(std::byte{});
    kernel0059Second.fill(std::byte{});
}
// @verified - p
void SetupE0059Environment() {
    SetPlayerControlEnabled(0);
    shenSetClockAndCompassRenderTo(1);
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_FreeAuthorPak(1, "0059");
    InitializeWaterTextureAnimation(7);
    GenerateFFT(2.0f);
    FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(1.7170000076293945f, distance0059)));
    void *const second = FillFftKernel(0.0729999989271164f, GenerateDistanceFalloffKernel8x8(0.6470000147819519f, kernel0059Second));
    void *const first = FillFftKernel(0.061000000685453415f, GenerateDistanceFalloffKernel8x8(0.37599998712539673f, kernel0059First));
    ApplyFft(first, second);
    FinalizeFft(0.0010000000474974513f, 0.0010000000474974513f);
    SetWaveLighting(0.1f, {-0.16320001f, 0.98180002f, -0.096900001f});
    HLib::EnqueueTaskWithoutParameter(HarborAuthorWaveCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
    SetMapModelVertexShaderType(3, 1);
    SetMapModelVertexShaderType(12, 1);
    HLib::EnqueueTaskWithoutParameter(HarborAuthorShip0059Callback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
}
// @verified - p
void E0059Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0059MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0059A.MOT"));
    parent->selection = 0;
    parent->state = 1;
    SetupE0059Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0059_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0059Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0059_WON), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0059_WON, false);
    EV_SetCharacterAlphaEnabled(AREA_0059_A021, false);
    EV_SetCharacterAlphaEnabled(AREA_0059_A126, false);
    EV_SetCharacterAlphaEnabled(AREA_0059_A005, false);
    EV_SetCharacterAlphaEnabled(AREA_0059_A020, false);
    EV_SetCharacterAlphaEnabled(AREA_0059_A103, false);
    EV_SetCharacterAlphaEnabled(AREA_0059_A022, false);
}
// @verified - p
void RestoreE0059Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0059_WON), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0059_WON, true);
    EV_SetCharacterAlphaEnabled(AREA_0059_A021, true);
    EV_SetCharacterAlphaEnabled(AREA_0059_A126, true);
    EV_SetCharacterAlphaEnabled(AREA_0059_A005, true);
    EV_SetCharacterAlphaEnabled(AREA_0059_A020, true);
    EV_SetCharacterAlphaEnabled(AREA_0059_A103, true);
    EV_SetCharacterAlphaEnabled(AREA_0059_A022, true);
}
// @verified - p
void CleanupE0059(AreaAuthorTaskParameter *parent) {
    shenUpdateCurrentDateTime();
    for (const shen_name *character = AREA_0059_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_FreeAuthorPak(0, "0059");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0059_AB00, 51);
}
// @verified - p
void E0059MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0059LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
        static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        EV_spmBeginSampleExecute();
        SetupE0059Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0059(parent);
}
// @verified - p
void E0059LoopCallback(void *taskData) {
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    else if (frame == 1731)
        SetChildObjectFlagByIndex(ctrlGetScenarioChara(AREA_AUTHOR_RYO), 3);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    SetChildObjectFlagByIndex(ctrlGetScenarioChara(AREA_AUTHOR_RYO), 0);
    RestoreE0059Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0059ScriptDefinition{.areaId = AREA_0059_AREA, .field04 = 0, .init = E0059Init, .main = E0059Main};
} // namespace shenmue::s2::areas
#endif
