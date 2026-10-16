#include "s2/areas/area_0339.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0339() {
}

// @verified - p
void Start0339(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    SetNpcColdResetMode(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0339, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    Setup0339Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0339_CHARACTERS, parent->self);
}

// @verified - p
void Update0339(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1)
        Queue0339Author(parent);
    else if (parent->state == 3)
        Cleanup0339(parent);
}

// @verified - p
void Cleanup0339(AuthorViewerTaskParameter *parent) {
    npcSetAreaName(AREA_0339_NPC_AREA, nullptr, 0.0f, 0);
    EV_stepSetFlags(0, 0x392);
    SetNpcColdResetMode(1);
    for (const shen_name *character = AREA_0339_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0339_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0339A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0339_RETURN.name) {
        EventSetPosition position{0, AREA_0339_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {114.08000183105469f, 92.33999633789062f, 159.66000366210938f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0339_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 33942, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else {
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

// @verified - p
std::int64_t Queue0339Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0339AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_NPC_NewLoadSw(1);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0339Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0339_AUTHOR.name));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0339Author(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0339Characters();
        child->parent->state = 3;
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0339AuthorTask(void *taskData) {
    Update0339Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0339Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0339_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0339_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0339_REN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0339_REN_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0339_JOP_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0339_JOP_, false);
    EV_SetCharacterAlphaEnabled(AREA_0339_a396, false);
}

// @verified - p
void Restore0339Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0339_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0339_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0339_REN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0339_REN_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0339_JOP_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0339_JOP_, true);
    EV_SetCharacterAlphaEnabled(AREA_0339_a396, true);
}

// @verified - p
void Setup0339Scene() {
    const Vector3 center{111.22000122070312f, 92.69999694824219f, 152.1999969482422f};
    npcSetAreaName(AREA_0339_NPC_AREA, &center, 5.0f, 1);
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_0339_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene("0339A000");
}

const ScriptAreaDefinition area0339ScriptDefinition{.areaId = AREA_0339, .field04 = 0, .init = Initialize0339, .main = Start0339};

} // namespace shenmue::s2::areas
