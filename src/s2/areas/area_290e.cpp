#include "s2/areas/area_290e.hpp"
#include "s2/input.hpp"
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
#include "s2/model_component.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize290E() {
}

// @verified - p
void Start290E(std::uintptr_t) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update290E, 4, 0x0b, sizeof(Area290EParameter), AUTHOR_VIEWER_TASK);
    Area290EParameter *const parent = static_cast<Area290EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = g_currentMap.name == AREA_290E.name ? 0 : 1;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_290E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_LoadCutsceneFromCurrentScene("0290A000");
    if (g_currentMap.name == AREA_290E.name) {
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        const Vector3 root{0.0f, 43.20000076293945f, 0.0f};
        SetLightRootPosition(root);
        for (std::uint32_t slot = 0; slot < 9; ++slot)
            SetMapModelSlotTransform(slot, &root, 0);
    }
    Queue_EVT_UTL_LoadNPCChara_3(AREA_290E_CHARACTERS, parent->self);
}

// @verified - p
void Update290E(void *taskData) {
    Area290EParameter *const parent = static_cast<Area290EParameter *>(taskData);
    if (parent->state == 1) {
        Queue290EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup290E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 0;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 0)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_290E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup290E(Area290EParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_290E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_290E_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_290E_ARCHIVE), 0);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_290E.name) {
        EV_scenQueueLoad(g_actNumber, AREA_290E, g_entryPoint);
    } else {
        LoadCutsceneAndSoundProgramForArea();
        EventSetPosition position{0, AREA_290E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {213.8300018310547f, 143.1999969482422f, 147.75999450683594f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_290E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 16384, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}

// @verified - p
std::int64_t Queue290EAuthor(Area290EParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update290EAuthorTask, 4, 0x0b, sizeof(Area290EAuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area290EAuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    mtwkSetExtraFlag(scenFindMotionwork(AREA_290E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_290E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_290E_REN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_290E_REN_, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_290E_AUTHOR.name));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update290EAuthor(Area290EAuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = g_currentMap.name == AREA_290E.name ? 0 : 3;
        mtwkSetExtraFlag(scenFindMotionwork(AREA_290E_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_290E_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_290E_REN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_290E_REN_, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update290EAuthorTask(void *taskData) {
    Update290EAuthor(static_cast<Area290EAuthorParameter *>(taskData));
}

const ScriptAreaDefinition area290EScriptDefinition{.areaId = AREA_290E, .field04 = 0, .init = Initialize290E, .main = Start290E};

} // namespace shenmue::s2::areas
