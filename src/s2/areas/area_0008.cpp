#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0008.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void E0008Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0008MainCallback, 4, 11, sizeof(E0008TaskParameter), TASK_EVNT);
    E0008TaskParameter *work = (E0008TaskParameter *)FTBL.HLib_GetTaskParameter(task);
    work->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0008A.MOT");
    work->handMotion = FTBL.LoadHandPoseBank(g_actNumber, g_currentMap, "HM_0008E.BIN");
    work->self = task;
    work->state = 0;
    work->selection = 0;
    ConfigureE0008Environment();
    Queue_EVT_UTL_LoadNPCChara_3(e0008Characters, work->self);
}
#else
static unsigned char e0008Distance[256];
static unsigned char e0008FirstKernel[256];
static unsigned char e0008SecondKernel[256];

shen_name e0008Characters[] = {
    AREA_0008_HNG, SHEN_NAME4('G', 'A', 'U', '_'), AREA_0008_SYO, INVALID_CHARACTER};

void E0008MainCallback(void *taskData)
{
    E0008TaskParameter *work = (E0008TaskParameter *)taskData;
    int input;
    switch (work->state) {
    case 0:
        input = FTBL.EVT_GetControllerInput(0, 1);
        if (input == 1024)
            work->state = 1;
        else if (input == 64) {
            if (--work->selection < 0)
                work->selection = 1;
        } else if (input == 128) {
            if (++work->selection > 1)
                work->selection = 0;
        }
        switch (work->selection) {
        case 0:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0008A000.c\n");
            break;
        case 1:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0008A012.c\n");
            break;
        }
        FTBL.SetDebugScreenTextPosition(2, 5);
        FTBL.DebugLogToScreen("seqdata%d.avb\n", work->selection);
        break;
    case 1:
        QueueE0008Loop(work);
        break;
    case 3:
        CleanupE0008(work);
        break;
    }
}

void QueueE0008Loop(E0008TaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0008LoopCallback, 4, 11, sizeof(E0008LoopParameter), TASK_EVNT);
    E0008LoopParameter *work = (E0008LoopParameter *)FTBL.HLib_GetTaskParameter(task);
    work->parent = parent;
    work->currentFrame = 0;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_LoadAudio("0008A000", parent->selection);
    SetupE0008Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void E0008LoopCallback(void *taskData)
{
    E0008LoopParameter *work = (E0008LoopParameter *)taskData;
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == 1) {
        FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(AREA_0008_HNG), 0, 1025, 0, 1.0f, frame, 0, 4, 0);
        FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(AREA_0008_SYO), 0, 1033, 0, 1.0f, frame, 0, 4, 0);
    }
    if (FTBL.EV_authSceneCtrl(-1) == 0) {
        work->parent->state = 0;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0008Characters();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void ConfigureE0008Environment(void)
{
    EventDateTime time;
    Vector3 direction;
    void *first, *second;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (time.hour >= 19) {
        FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
        FTBL.PushGlobalLightingProgram();
        FTBL.SetObjectFlagByIndex(15, 1);
        FTBL.SetObjectFlagByIndex(16, 0);
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.SetObjectFlagByIndex(15, 0);
        FTBL.SetObjectFlagByIndex(16, 1);
    }
    FTBL.EV_FreeAuthorPak(1, "0008");
    FTBL.GenerateFFT(2.0f);
    FTBL.fft_fill_dist(FTBL.fft_fill_krn(10.0f, FTBL.generate_distance_falloff_row_major(2.082f, e0008Distance)));
    first = FTBL.fft_fill_krn(0.0564f, FTBL.generate_distance_falloff_kernel_8x8(0.94f, e0008FirstKernel));
    second = FTBL.fft_fill_krn(0.035f, FTBL.generate_distance_falloff_kernel_8x8(0.952f, e0008SecondKernel));
    FTBL.fft_bins_load_pairwise_tables(first, second);
    FTBL.FFTUtil(0.001f, 0.001f);
    direction.x = 0.0598f;
    direction.y = 0.9768f;
    direction.z = 0.2054f;
    FTBL.set_lighting_flags(0.1f, &direction);
    FTBL.HLib_EnqueueTaskWithoutParameter(E0008WaveCallback, 3, 2, TASK_EVNT);
    FTBL.SetMapModelVertexShaderType(18, 1);
    FTBL.SetMapModelVertexShaderType(19, 1);
}

void CleanupE0008(E0008TaskParameter *parent)
{
    EVT_UTL_FreeNPCChara(e0008Characters);
    FTBL.EV_FreeAuthorPak(0, "0008");
    FTBL.DeleteMotion(parent->motion);
    FTBL.ReleaseHandPoseBank(parent->handMotion);
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

void SetupE0008Characters(void)
{
    int i;
    PrepareE0008Character(AREA_0008_RYO);
    HideE0008Prop(AREA_0008_A012);
    HideE0008Prop(AREA_0008_A195);
    for (i = 0; e0008Characters[i] != INVALID_CHARACTER; i++)
        PrepareE0008Character(e0008Characters[i]);
}

void RestoreE0008Characters(void)
{
    int i;
    RestoreE0008Character(AREA_0008_RYO);
    ShowE0008Prop(AREA_0008_A012);
    ShowE0008Prop(AREA_0008_A195);
    for (i = 0; e0008Characters[i] != INVALID_CHARACTER; i++)
        RestoreE0008Character(e0008Characters[i]);
}

void PrepareE0008Character(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}

void RestoreE0008Character(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

void HideE0008Prop(shen_name name)
{
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}

void ShowE0008Prop(shen_name name)
{
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

void E0008WaveCallback(void *taskData)
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
    model = FTBL.GetMapModelRoot(18);
    FTBL.CalculateFieldCollision(*(void **)((unsigned char *)*(void **)((unsigned char *)model + 0x2c) + 0x28));
    model = FTBL.GetMapModelRoot(19);
    FTBL.TransformFieldCollisionForModel(model);
    FTBL.nlSetTexanimUV(-0.001f, 0.001f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(19));
}
#endif
#else
#include "s2/areas/area_0008.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/hand.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>
#include <array>

namespace shenmue::s2::areas {
namespace {

void E0008MainCallback(void *taskData);
void E0008LoopCallback(void *taskData);

// @verified - p
std::array<std::byte, 0x100> e0008FirstBuffer, e0008SecondBuffer, e0008ThirdBuffer;
void E0008Init() {
    e0008FirstBuffer.fill(std::byte{});
    e0008SecondBuffer.fill(std::byte{});
    e0008ThirdBuffer.fill(std::byte{});
}

// @verified - p
void ConfigureE0008Environment() {
    SetPlayerControlEnabled(0);
    DateTime time;
    shenGetCurrentDateTime(time);
    if (time.hour < 19) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(15, 0);
        EV_SetObjectFlagByIndex(16, 1);
    } else {
        EV_lightLoad(AREA_0008_0000);
        PushGlobalLightingProgram();
        EV_SetObjectFlagByIndex(15, 1);
        EV_SetObjectFlagByIndex(16, 0);
    }
    EV_FreeAuthorPak(1, AREA_0008_CONVERSATION);
}

// @verified - p
void E0008Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0008MainCallback, 4, 0x0b, 0x20, AREA_AUTHOR_TASK);
    E0008TaskParameter *const parent = static_cast<E0008TaskParameter *>(HLib::GetTaskParameter(task));
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0008_MOTION));
    parent->handMotion = LoadHandPoseBank(g_actNumber, g_currentMap, AREA_0008_HAND_MOTION);
    parent->self = task;
    parent->selection = 0;
    parent->state = 0;
    ConfigureE0008Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0008_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0008Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0008_A012, false);
    EV_SetCharacterAlphaEnabled(AREA_0008_A195, false);
    for (const shen_name *character = AREA_0008_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0008Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0008_A012, true);
    EV_SetCharacterAlphaEnabled(AREA_0008_A195, true);
    for (const shen_name *character = AREA_0008_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0008(E0008TaskParameter *parent) {
    for (const shen_name *character = AREA_0008_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0008_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    ReleaseHandPoseBank(parent->handMotion);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0008Loop(E0008TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0008LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0008LoopParameter *const child = static_cast<E0008LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_LoadAudio("0008A000", parent->selection);
    SetupE0008Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0008MainCallback(void *taskData) {
    E0008TaskParameter *const parent = static_cast<E0008TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0008Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0008(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128 && ++parent->selection > 1)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0008_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0008LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 1) {
        HandParams *const firstHand = GetCharacterHand(ctrlGetScenarioChara(AREA_0008_HNG_), 0);
        if (firstHand)
            ConfigureHandPoseSequence(*firstHand, 1025, 0, 1, 0, 4, 1.0f, 0);
        HandParams *const secondHand = GetCharacterHand(ctrlGetScenarioChara(AREA_0008_SYO_), 0);
        if (secondHand)
            ConfigureHandPoseSequence(*secondHand, 1033, 0, 1, 0, 4, 1.0f, 0);
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    static_cast<E0008LoopParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0008Characters();
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0008ScriptDefinition{.areaId = AREA_0008_AREA, .field04 = 0, .init = E0008Init, .main = E0008Main};

} // namespace shenmue::s2::areas
#endif
