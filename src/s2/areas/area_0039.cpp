#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0039.hpp"
#include "s2/event_queued_dc.h"

#if defined(SHENMUE_EVENT_ENTRY)
void E0039Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0039MainCallback, 4, 11, sizeof(AreaAuthorTaskParameter), TASK_EVNT);
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0039A.MOT");
    parent->self = task;
    parent->state = 1;
    if (!g_actNumber)
        parent->selection = (unsigned int)g_entryPoint > 8 ? 0 : g_entryPoint;
    else if (g_entryPoint == 1)
        parent->selection = 1;
    else
        parent->selection = 5;
    SetupE0039Environment();
    Queue_EVT_UTL_LoadNPCChara_3(area0039Characters[parent->selection], parent->self);
}
#else
shen_name area0039Characters0[] = {AREA_0039_TKM_, INVALID_CHARACTER};
shen_name area0039Characters1[] = {AREA_0039_TNZ_, INVALID_CHARACTER};
shen_name area0039Characters2[] = {AREA_0039_ANC_, AREA_0039_GEN_, INVALID_CHARACTER};
shen_name area0039Characters3[] = {AREA_0039_ANC_, AREA_0039_GEN_, AREA_0039_SBO_, AREA_0039_CHR_, INVALID_CHARACTER};
shen_name area0039Characters4[] = {AREA_0039_SBO_, AREA_0039_SYE_, AREA_0039_CHR_, AREA_0039_ANC_, AREA_0039_GEN_, INVALID_CHARACTER};
shen_name area0039Characters5[] = {AREA_0039_KUN_, INVALID_CHARACTER};
shen_name area0039Characters6[] = {AREA_0039_KIC_, INVALID_CHARACTER};
shen_name area0039Characters7[] = {AREA_0039_CLY_, INVALID_CHARACTER};
shen_name area0039Characters8[] = {AREA_0039_HUR_, INVALID_CHARACTER};
shen_name *area0039Characters[10] = {area0039Characters0, area0039Characters1, area0039Characters2, area0039Characters3, area0039Characters4,
    area0039Characters5, area0039Characters6, area0039Characters7, area0039Characters8};

void E0039MainCallback(void *taskData)
{
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)taskData;
    switch (parent->state) {
    case 1:
        QueueE0039Loop(parent);
        break;
    case 3:
        CleanupE0039(parent);
        break;
    }
}

void QueueE0039Loop(AreaAuthorTaskParameter *parent)
{
    HLibTask *task;
    LoadE0039ScenePackages(parent->selection);
    task = FTBL.HLib_EnqueueTaskWithParameter(E0039LoopCallback, 4, 11, sizeof(AreaAuthorLoopTaskParameter), TASK_EVNT);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    if (parent->selection == 5)
        FTBL.EV_LoadAudio("0039A000", 4);
    FTBL.EV_spmBeginSampleExecute(0);
    SetupE0039Characters(parent->selection);
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void E0039LoopCallback(void *taskData)
{
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)taskData;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0039Characters(child->parent->selection);
        child->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void LoadE0039ScenePackages(int selection)
{
    switch (selection) {
    case 0:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WTS3", 0),
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    case 1:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WSY0", 0),
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    case 5:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WTA0", 0),
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    case 6:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WS00", 0),
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    case 7:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WNC0", 0),
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    case 8:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"AR02", 0),
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    default:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WT00", 0),
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    }
    FTBL.SetMapModelDataDefaultVertexShader();
    switch (selection) {
    case 0:
        FTBL.EV_lightLoad(AREA_0039_0000);
        break;
    case 5:
        FTBL.EV_lightLoad(AREA_0039_0002);
        break;
    case 6:
        FTBL.EV_lightLoad(AREA_0039_0003);
        break;
    case 7:
        FTBL.EV_lightLoad(AREA_0039_0004);
        break;
    case 8:
        FTBL.EV_lightLoad(AREA_0039_0005);
        break;
    }
    FTBL.PushGlobalLightingProgram();
}

void SetupE0039Environment(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0039");
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
}

void CleanupE0039(AreaAuthorTaskParameter *parent)
{
    EventDateTime time;
    EVT_UTL_FreeNPCChara(area0039Characters[parent->selection]);
    FTBL.EV_FreeAuthorPak(0, "0039");
    FTBL.DeleteMotion(parent->motion);
    switch (parent->selection) {
    case 1:
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0039_WS00, 79);
        break;
    case 5:
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 8;
        time.minute = 55;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0039_WBBK, 1);
        break;
    default:
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    }
}

void SetupE0039Characters(short selection)
{
    int i;
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorProp(AREA_0039_A128);
    HideAuthorProp(AREA_0039_A474);
    HideAuthorProp(AREA_0039_A052);
    HideAuthorProp(AREA_0039_A053);
    HideAuthorProp(AREA_0039_A035);
    HideAuthorProp(AREA_0039_A030);
    HideAuthorProp(AREA_0039_AA83);
    HideAuthorProp(AREA_0039_A034);
    HideAuthorProp(AREA_0039_A033);
    FTBL.TelCode_Enqueue();
    if (selection == 4)
        FTBL.LoadBlurModelForCharacter_0(AREA_0039_SYE_, "SYE_B.MT7", 1.0f);
    for (i = 0; area0039Characters[selection][i] != INVALID_CHARACTER; i++)
        HideAuthorCharacter(area0039Characters[selection][i]);
}

void RestoreE0039Characters(short selection)
{
    int i;
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorProp(AREA_0039_A128);
    ShowAuthorProp(AREA_0039_A474);
    ShowAuthorProp(AREA_0039_A052);
    ShowAuthorProp(AREA_0039_A053);
    ShowAuthorProp(AREA_0039_A035);
    ShowAuthorProp(AREA_0039_A030);
    ShowAuthorProp(AREA_0039_AA83);
    ShowAuthorProp(AREA_0039_A034);
    ShowAuthorProp(AREA_0039_A033);
    FTBL.UpdateTelephoneState();
    for (i = 0; area0039Characters[selection][i] != INVALID_CHARACTER; i++)
        ShowAuthorCharacter(area0039Characters[selection][i]);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"
#endif
#else
#include "s2/areas/area_0039.hpp"
#include "s2/scene_data.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"

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
#include "s2/blur_figure_pose.hpp"
#include "s2/telephone_cord.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0039MainCallback(void *taskData);
void E0039LoopCallback(void *taskData);

// @verified - p
void E0039Init() {
}

// @verified - p
void E0039Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0039MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0039_MOTION));
    parent->self = task;
    parent->state = 1;
    parent->selection =
        g_actNumber == 0 ? static_cast<std::int16_t>(g_entryPoint > 8 ? 0 : g_entryPoint) : static_cast<std::int16_t>(g_entryPoint == 1 ? 1 : 5);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0039_CONVERSATION);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0039_CHARACTER_TABLES[parent->selection], parent->self);
}

// @verified - p
void SetupE0039Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A128, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A474, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A052, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A053, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A035, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A030, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_AA83, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A034, false);
    EV_SetCharacterAlphaEnabled(AREA_0039_A033, false);
    EnqueueTelephoneCord();
    if (selection == 4)
        LoadBlurModelForCharacter(AREA_0039_SYE_, "SYE_B.MT7", 1.0f);
    for (const shen_name *character = AREA_0039_CHARACTER_TABLES[selection]; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0039Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A128, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A474, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A052, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A053, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A035, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A030, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_AA83, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A034, true);
    EV_SetCharacterAlphaEnabled(AREA_0039_A033, true);
    CleanupTelephoneCord();
    for (const shen_name *character = AREA_0039_CHARACTER_TABLES[selection]; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0039(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0039_CHARACTER_TABLES[parent->selection]; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0039_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    const std::int16_t selection = parent->selection;
    if (selection == 5) {
        DateTime time;
        shenGetCurrentDateTime(time);
        time.hour = 8;
        time.minute = 55;
        SetCurrentDateTimeTo(time);
    }
    HLib::DestroyTask(parent->self);
    if (selection == 1)
        EV_scenQueueLoad(g_actNumber, AREA_0039_WS00, 79);
    else if (selection == 5)
        EV_scenQueueLoad(g_actNumber, AREA_0039_WBBK, 1);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void LoadE0039ScenePackages(std::int32_t selection) {
    const char *package;
    switch (selection) {
    case 0:
        package = "WTS3";
        break;
    case 1:
        package = "WSY0";
        break;
    case 5:
        package = "WTA0";
        break;
    case 6:
        package = "WS00";
        break;
    case 7:
        package = "WNC0";
        break;
    case 8:
        package = "AR02";
        break;
    default:
        package = "WT00";
        break;
    }
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    SetMapModelDataDefaultVertexShader();
    if (selection == 0)
        EV_lightLoad(AREA_0039_0000);
    else if (selection == 5)
        EV_lightLoad(AREA_0039_0002);
    else if (selection == 6)
        EV_lightLoad(AREA_0039_0003);
    else if (selection == 7)
        EV_lightLoad(AREA_0039_0004);
    else if (selection == 8)
        EV_lightLoad(AREA_0039_0005);
    PushGlobalLightingProgram();
}

// @verified - p
void QueueE0039Loop(AreaAuthorTaskParameter *parent) {
    LoadE0039ScenePackages(parent->selection);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0039LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    if (parent->selection == 5)
        EV_LoadAudio("0039A000", 4);
    BeginSecondarySampleExecution(0);
    SetupE0039Characters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0039MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0039Loop(parent);
    else if (parent->state == 3)
        CleanupE0039(parent);
}

// @verified - p
void E0039LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0039Characters(static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->selection);
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0039ScriptDefinition{.areaId = AREA_0039_AREA, .field04 = 0, .init = E0039Init, .main = E0039Main};

} // namespace shenmue::s2::areas
#endif
