#include "s2/areas/area_0307.hpp"
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
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/weather.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::int32_t weather0307;
}

// @verified - p
void Initialize0307() {
    weather0307 = 0;
}

// @verified - p
void Start0307(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0307, 4, 0x0b, sizeof(Area0307Parameter), AUTHOR_VIEWER_TASK);
    Area0307Parameter *const parent = static_cast<Area0307Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    if (g_currentMap.name == AREA_0307.name)
        EV_SetObjectFlagByIndex(4, 0);
    EV_LoadCutsceneFromCurrentScene("0307A000");
    EV_FreeAuthorPak(1, AREA_0307_CONVERSATION);
    if (g_currentMap.name == AREA_0307_RETURN.name)
        EV_DisableCharacterDisplay(AREA_0307_BIKE);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0307_CHARACTERS, parent->self);
}

// @verified - p
void Update0307(void *taskData) {
    Area0307Parameter *const parent = static_cast<Area0307Parameter *>(taskData);
    if (parent->state == 1)
        Queue0307Author(parent);
    else if (parent->state == 3)
        Cleanup0307(parent);
}

// @verified - p
void Cleanup0307(Area0307Parameter *parent) {
    for (const shen_name *character = AREA_0307_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0307_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0307_ARCHIVE), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    LoadCutsceneAndSoundProgramForArea();
    if (g_currentMap.name == AREA_0307_RETURN.name) {
        EventSetPosition position{0, AREA_0307_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {264.79998779296875f, 100.0f, 183.5800018310547f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0307_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 0x4000, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        EV_NPC_NewLoadSw(1);
        EV_EnableCharacterDisplay(AREA_0307_BIKE);
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
    } else {
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

// @verified - p
std::int64_t Queue0307Author(Area0307Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0307AuthorTask, 4, 0x0b, sizeof(Area0307AuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area0307AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0307_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0307_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0307_JOP_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0307_JOP_, false);
    EV_SetCharacterAlphaEnabled(AREA_0307_a396, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0307_AUTHOR.name));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0307Author(Area0307AuthorParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 271) {
        weather0307 = GetCurrentWeather();
        if (weather0307 == 2)
            EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    } else if (frame == 449 && weather0307 == 2) {
        ResetWeatherOverrides();
        EV_FreeWAIR();
        EV_UpdateWeather();
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 3;
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0307_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0307_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0307_JOP_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0307_JOP_, true);
        EV_SetCharacterAlphaEnabled(AREA_0307_a396, true);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0307AuthorTask(void *taskData) {
    Update0307Author(static_cast<Area0307AuthorParameter *>(taskData));
}

const ScriptAreaDefinition area0307ScriptDefinition{.areaId = AREA_0307, .field04 = 0, .init = Initialize0307, .main = Start0307};

} // namespace shenmue::s2::areas
