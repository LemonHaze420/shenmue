#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qesc.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void QescMain(void *argument)
{
    if (g_currentMap == AREA_QESC)
        FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    QueueClearNpcs(QueueQescEscapeEvent, argument);
}
#else
const shen_name qescInvalidCharacter = EVENT_CHARACTER_INVALID;
const int qescDefaultArguments[] = {0};
static shen_name qescCharacters[] = {
    QESC_RYO, SHEN_NAME4('Y', 'U', 'N', '_'), SHEN_NAME4('G', 'O', 'I', '_'), SHEN_NAME4('H', 'T', 'K', '_'), SHEN_NAME4('S', 'A', 'M', '_'),
    SHEN_NAME4('L', 'L', 'Y', '_'), EVENT_CHARACTER_INVALID};

void QueueQescEscapeEvent(void *argument)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(QescEscapeCallback, 4, 11, sizeof(QescEventWork), TASK_EVNT);
    QescEventWork *work = (QescEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    if (argument)
        work->arguments = argument;
    else
        work->arguments = &qescDefaultArguments[g_entryPoint];
    InitializeQescEscape(work);
    EventDebugRegister(QESC_DEBUG_ID, QescExitCallback, work, 2000000000);
}

void QescExitCallback(void *data)
{
    FinishQescEscape((QescEventWork *)data);
}

void FinishQescEscape(QescEventWork *work)
{
    ReleaseQescEscape(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(QESC_DEBUG_ID);
    if (g_currentMap == AREA_QESC) {
        FTBL.scenSetTaskFlags(6);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

void ResetQescEscapeState(int *state)
{
}

void QescEscapeCallback(void *data)
{
    EventDebugCheck(QESC_DEBUG_ID, 4);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.AT_SetLocalBaseChara(QESC_DUMMY, QESC_RYO);
    EVT_AT_SetPos(QESC_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QESC_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    EVT_AT_SetPos(QESC_DUMMY, QESC_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QESC_DUMMY, QESC_RYO, 0.0f, 0);
    FTBL.HLib_SetCurrentTaskTo(QescDormantCallback);
}

void QescDormantCallback(void *data)
{
}

void FadeOutAndFinishQescEscape(QescEventWork *work)
{
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
    FinishQescEscape(work);
}

void InitializeQescEscape(QescEventWork *work)
{
    float colour[4] = {0.0f, 0.25f, 0.24f, 0.23f};
    const char *directory;
    work->characters = Queue_EVT_AT_ReadChr(work->task, qescCharacters);
    work->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    ResetQescEscapeState(&work->state);
    directory = FTBL.Filepath_Gen(0);
    FTBL.EV_ControlPackageResources(0, 0, directory, (int)"QESC", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "QESC", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QESC", 0, 0);
    if (g_currentMap == AREA_QESC) {
        FTBL.SetWeatherUpdateEnabledTo(0);
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.UpdateGlobalLighting(colour);
    }
}

void ReleaseQescEscape(QescEventWork *work)
{
    FTBL.DeleteMotion(work->motion);
    FTBL.EV_ControlPackageResources(0, 6, "QESC", 0, 0);
    ET_AT_KillWork(work->characters);
}
#endif
#else
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
    (void)EV_pakUtils(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("QESC"));
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QESC"), 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QESC"), 0);
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
#endif
