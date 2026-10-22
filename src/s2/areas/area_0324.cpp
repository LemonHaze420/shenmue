#include "s2/areas/area_0324.hpp"
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
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/load_title.hpp"
#include "s2/author.hpp"
#include "s2/fog.hpp"
#include "s2/ui.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0324() {
}

// @verified - p
void Start0324(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0324, 4, 0x0b, sizeof(Area0324Parameter), AUTHOR_VIEWER_TASK);
    Area0324Parameter *const parent = static_cast<Area0324Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0324A000");
    EV_FreeAuthorPak(1, "0324");
    EV_FreeAuthorPak(1, "0262");
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0324_LIGHTS[0]);
    PushGlobalLightingProgram();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0324_CHARACTERS, parent->self);
}

void Update0324(void *taskData) {
    Area0324Parameter *const parent = static_cast<Area0324Parameter *>(taskData);
    if (parent->state == 1)
        Queue0324Author(parent);
    else if (parent->state == 3)
        Cleanup0324(parent);
    else if (parent->state == 5) {
        ST_LoadTitleFade();
        HLib::SetupTimedTask(parent->self, 90);
        parent->state = 1;
    } else if (parent->state == 0) {
        DateTime time{};
        GetCurrentDateTime(time);
        time.hour = static_cast<std::int8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 32));
        time.minute = static_cast<std::int8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 33));
        time.day = static_cast<std::int8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 34));
        time.year = static_cast<std::int8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 48));
        time.month = static_cast<std::int8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 49));
        SetCurrentDateTimeTo(time);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0324_LIGHTS[1]);
        PushGlobalLightingProgram();
        EV_LoadCutsceneFromCurrentScene("0262A000");
        SetQronFloor(-1);
        Queue_ST_LoadTitleFadeIn(shen_name{});
        HLib::SetupTimedTask(parent->self, 150);
        parent->state = 5;
    }
}

// @verified - p
void Cleanup0324(Area0324Parameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0324_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, "0324");
    EV_FreeAuthorPak(0, "0262");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0324_ARCHIVE), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0324.name) {
        EV_scenQueueLoad(g_actNumber, AREA_0324, g_entryPoint);
    } else {
        LoadCutsceneAndSoundProgramForArea();
        EventSetPosition position{0, AREA_0324_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {215.3f, 92.8f, 129.8f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0324_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 49152, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        SetAreaFogParams();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
    }
}

// @verified - p
std::int64_t Queue0324Author(Area0324Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0324AuthorTask, 4, 0x0b, sizeof(Area0324AuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area0324AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    Prepare0324Characters(parent->selection);
    if (parent->selection == 0)
        EV_LoadAudio("0324A000", 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>((parent->selection != 0 ? AREA_0324_SECOND_AUTHOR : AREA_0324_FIRST_AUTHOR).name));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0324Author(Area0324AuthorParameter *child) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0324Characters(child->parent->selection);
        if (g_currentMap.name == AREA_0324.name || child->parent->selection != 0)
            child->parent->state = 3;
        else {
            child->parent->selection = 1;
            child->parent->state = 0;
        }
        HLib::DestroyCurrentTask();
    }
    const shen_name author = GetCurrentAuthorId();
    if (author.name != AREA_0324.name)
        return author.name;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1 || frame == 120) {
        snapSetDesc(1);
        return 0;
    }
    if (frame == 96 || frame == 164) {
        snapSetDesc(0);
        return 0;
    }
    return frame - 120;
}

void Update0324AuthorTask(void *taskData) {
    Update0324Author(static_cast<Area0324AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0324Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0324_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0324_RYO_, false);
    if (selection != 0) {
        EV_SetCharacterAlphaEnabled(AREA_0324_a276, false);
        EV_SetCharacterAlphaEnabled(AREA_0324_aa80, false);
        EV_SetCharacterAlphaEnabled(AREA_0324_aa77, false);
    } else {
        EV_SetCharacterAlphaEnabled(AREA_0324_a464, false);
        EV_SetCharacterAlphaEnabled(AREA_0324_a202, false);
        EV_SetCharacterAlphaEnabled(AREA_0324_a203, false);
        EV_SetCharacterAlphaEnabled(AREA_0324_a247, false);
    }
    for (const shen_name *character = AREA_0324_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0324Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0324_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0324_RYO_, true);
    if (selection != 0) {
        EV_SetCharacterAlphaEnabled(AREA_0324_a276, true);
        EV_SetCharacterAlphaEnabled(AREA_0324_aa80, true);
        EV_SetCharacterAlphaEnabled(AREA_0324_aa77, true);
    } else {
        EV_SetCharacterAlphaEnabled(AREA_0324_a464, true);
        EV_SetCharacterAlphaEnabled(AREA_0324_a202, true);
        EV_SetCharacterAlphaEnabled(AREA_0324_a203, true);
        EV_SetCharacterAlphaEnabled(AREA_0324_a247, true);
    }
    for (const shen_name *character = AREA_0324_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0324ScriptDefinition{.areaId = AREA_0324, .field04 = 0, .init = Initialize0324, .main = Start0324};

} // namespace shenmue::s2::areas
