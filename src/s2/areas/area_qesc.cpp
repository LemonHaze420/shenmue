#include "s2/areas/area_qesc.hpp"
#include "s2/scene_data.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/step_event.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2 {

// @verified - p
void QescInit() {
}

void QescMain(std::uintptr_t argument) {
    if (g_currentMap.name == AREA_QESC.name)
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), QESC_TASK);
    (void)RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueQescEscapeEvent;
    work->reserved10 = 0;
    work->state = 0;
    work->callbackData = reinterpret_cast<void *>(argument);
    work->delay = 3.0f;
}

// @verified - p
void QueueQescEscapeEvent(void *argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QescEscapeCallback, 4, 0x0b, sizeof(QescEventWork), QESC_TASK);
    QescEventWork *const work = static_cast<QescEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->arguments = argument ?
        argument :
        reinterpret_cast<const void *>(reinterpret_cast<std::uintptr_t>(QESC_DEFAULT_ARGUMENTS) + 16U * static_cast<std::uint32_t>(g_entryPoint));
    InitializeQescEscape(work);
}

// @verified - p
void QescEscapeCallback(void *) {
    EventFilmControl film{.cleanup = 0,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0.0f};
    EVT_AT_MenCtrl_Callback(&film);
    AT_SetLocalBaseChara(QESC_DUMMY, QESC_RYO);
    EventSetPosition basePosition{
        .cleanup = 0, .character = QESC_BASE, .parent = QESC_INVALID, .position = Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f}, .delay = 0.0f};
    EVT_AT_SetPosRot(&basePosition);
    EventSetAngle baseAngle{.cleanup = 0, .character = QESC_BASE, .parent = QESC_INVALID, .degrees = 0.0f, .delay = 0.0f};
    EVT_AT_SetAngY_Callback(&baseAngle);
    EventSetPosition dummyPosition{
        .cleanup = 0, .character = QESC_DUMMY, .parent = QESC_RYO, .position = Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f}, .delay = 0.0f};
    EVT_AT_SetPosRot(&dummyPosition);
    EventSetAngle dummyAngle{.cleanup = 0, .character = QESC_DUMMY, .parent = QESC_RYO, .degrees = 0.0f, .delay = 0.0f};
    EVT_AT_SetAngY_Callback(&dummyAngle);
    HLib::SetCurrentTaskTo(QescDormantCallback);
}

// @verified - p
void InitializeQescEscape(QescEventWork *work) {
    work->characters = Queue_EVT_AT_ReadChr(work->task, QESC_CHARACTERS);
    work->motion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    (void)EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("QESC"));
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QESC"), 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QESC"), 0);
    if (g_currentMap.name != AREA_QESC.name)
        return;
    SetWeatherUpdateEnabledTo(0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    UpdateGlobalLighting(QESC_LIGHT_COLOUR);
}

// @verified - p
void QescDormantCallback(void *) {
}

const ScriptAreaDefinition qescScriptDefinition{.areaId = AREA_QESC, .field04 = 0, .init = QescInit, .main = QescMain};

} // namespace shenmue::s2
