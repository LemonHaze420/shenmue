#include "s2/areas/area_0361.hpp"
#include "s2/author.hpp"
#include "s2/map_assets.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0361() {
}

// @verified - p
void Start0361(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0361, 4, 0x0b, sizeof(Area0361Parameter), AUTHOR_VIEWER_TASK);
    Area0361Parameter *const parent = static_cast<Area0361Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    Setup0361Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0361_CHARACTERS, parent->self);
}

// @verified - p
void Update0361(void *taskData) {
    Area0361Parameter *const parent = static_cast<Area0361Parameter *>(taskData);
    if (parent->state == 1)
        Queue0361Author(parent);
    else if (parent->state == 3)
        Cleanup0361(parent);
}

// @verified - p
void Cleanup0361(Area0361Parameter *parent) {
    for (const shen_name *character = AREA_0361_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0361_ARCHIVE), 0);
    EV_FreeAuthorPak(0, AREA_0361_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0361_RETURN.name) {
        LoadCutsceneAndSoundProgramForArea();
        EventSetPosition position{0, AREA_0361_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {255.75f, 161.1999969482422f, 139.97999572753906f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0361_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 0x4000, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
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
std::int64_t Queue0361Author(Area0361Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0361AuthorTask, 4, 0x0b, sizeof(Area0361AuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area0361AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetQronFloor(17);
    EV_spmBeginSampleExecute();
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0361_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0361_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0361_REN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0361_REN_, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0361_AUTHOR.name));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0361Author(Area0361AuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 3;
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0361_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0361_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0361_REN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0361_REN_, true);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0361AuthorTask(void *taskData) {
    Update0361Author(static_cast<Area0361AuthorParameter *>(taskData));
}

// @verified - p
void Setup0361Scene() {
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    if (g_currentMap.name != AREA_0361_RETURN.name) {
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        const Vector3 root{0.0f, 61.20000076293945f, 0.0f};
        SetLightRootPosition(root);
        for (std::uint32_t slot = 0; slot < 9; ++slot)
            SetMapModelSlotTransform(slot, &root, 0);
    }
    EV_FreeAuthorPak(1, AREA_0361_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene("0361A000");
}

const ScriptAreaDefinition area0361ScriptDefinition{.areaId = AREA_0361, .field04 = 0, .init = Initialize0361, .main = Start0361};

} // namespace shenmue::s2::areas
