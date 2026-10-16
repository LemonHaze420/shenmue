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
        if (FindCharacterByScenarioName(*character)) {
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
        GetCurrentDateTime(time);
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
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
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
