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
        if (FindCharacterByScenarioName(*character)) {
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
        SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 3);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
    RestoreE0059Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0059ScriptDefinition{.areaId = AREA_0059_AREA, .field04 = 0, .init = E0059Init, .main = E0059Main};
} // namespace shenmue::s2::areas
