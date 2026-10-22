#include "s2/areas/area_0143.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::array<std::byte, 0x100> distance0143{}, kernel0143First{}, kernel0143Second{};
void E0143MainCallback(void *);
void E0143LoopCallback(void *);

// @verified - p
void E0143Init() {
    distance0143.fill(std::byte{});
    kernel0143First.fill(std::byte{});
    kernel0143Second.fill(std::byte{});
}

// @verified - p
void E0143WaveCallback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.36070001125335693f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.3725000023841858f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.4352000057697296f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.2549000084400177f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3799999952316284f,
            static_cast<float>(specular & 255U) / 255.0f * 0.28619998693466187f},
        42.540000915527344f, 1.0f);
    ApplyWaveTextureMeshResource(2);
    ApplyWaveReflectionResource(3);
}

// @verified - p
void SetupE0143Wave() {
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 23;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0143_CONVERSATION);
    GenerateFFT(2.0f);
    void *const distance = GenerateDistanceFalloffRowMajor(1.717f, distance0143);
    FillFftDistance(FillFftKernel(11.764f, distance));
    void *const second = FillFftKernel(0.073f, GenerateDistanceFalloffKernel8x8(0.647f, kernel0143Second));
    void *const first = FillFftKernel(0.061f, GenerateDistanceFalloffKernel8x8(0.376f, kernel0143First));
    ApplyFft(first, second);
    FinalizeFft(0.0002f, 0.0002f);
    SetWaveLighting(0.1f, {-0.16320001f, 0.98180002f, -0.096900001f});
    HLib::EnqueueTaskWithoutParameter(E0143WaveCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
}

// @verified - p
void E0143Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0143MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0143_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetupE0143Wave();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0143_CHARACTERS, parent->self);
}

// @verified - p
void CleanupE0143(AreaAuthorTaskParameter *parent) {
    if (FindCharacterByScenarioName(AREA_0143_WON)) {
        EV_npcLockUnlock(AREA_0143_WON);
        EV_npcFree(AREA_0143_WON);
    }
    EV_FreeAuthorPak(0, AREA_0143_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0143Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0143LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0143_WON), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0143_WON, false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0143MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0143Loop(parent);
    else if (parent->state == 3)
        CleanupE0143(parent);
    else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64 || input == 128)
            parent->selection = 0;
        if (parent->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0143A000.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0143LoopCallback(void *taskData) {
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 570)
        SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 4);
    else if (frame == 802)
        SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0143_WON), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0143_WON, true);
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0143ScriptDefinition{.areaId = AREA_0143_AREA, .field04 = 0, .init = E0143Init, .main = E0143Main};
} // namespace shenmue::s2::areas
