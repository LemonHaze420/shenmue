#include "s2/areas/area_347e.hpp"
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
void Initialize347E() {
}

// @verified - p
void Start347E(std::uintptr_t) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update347E, 4, 0x0b, sizeof(Area347EParameter), AUTHOR_VIEWER_TASK);
    Area347EParameter *const parent = static_cast<Area347EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = g_currentMap.name == AREA_347E.name ? 0 : 1;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_LoadCutsceneFromCurrentScene("0347E000");
    EV_FreeAuthorPak(1, AREA_347E_CONVERSATION);
    if (g_currentMap.name == AREA_347E.name) {
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        const Vector3 root{0.0f, 39.599998474121094f, 0.0f};
        SetLightRootPosition(root);
        for (std::uint32_t slot = 0; slot < 10; ++slot)
            SetMapModelSlotTransform(slot, &root, 0);
    }
    Queue_EVT_UTL_LoadNPCChara_3(AREA_347E_CHARACTERS, parent->self);
}

// @verified - p
void Update347E(void *taskData) {
    Area347EParameter *const parent = static_cast<Area347EParameter *>(taskData);
    if (parent->state == 1) {
        Queue347EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup347E(parent);
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
            EV_debug_log_to_screen(AREA_347E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup347E(Area347EParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_347E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_347E_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_347E_ARCHIVE), 0);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_347E.name) {
        EV_scenQueueLoad(g_actNumber, AREA_347E, g_entryPoint);
    } else {
        LoadCutsceneAndSoundProgramForArea();
        EventSetPosition position{0, AREA_347E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {244.27000427246094f, 139.60000610351562f, 157.55999755859375f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_347E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 41870, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}

// @verified - p
std::int64_t Queue347EAuthor(Area347EParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update347EAuthorTask, 4, 0x0b, sizeof(Area347EAuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area347EAuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_spmBeginSampleExecute();
    mtwkSetExtraFlag(scenFindMotionwork(AREA_347E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_347E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_347E_REN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_347E_REN_, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_347E_AUTHOR.name));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update347EAuthor(Area347EAuthorParameter *child) {
    const std::int16_t frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 515)
        hmdlSetFlagsFor(FindCharacterByScenarioName(AREA_347E_RYO_), 0, 0, 12);
    else if (frame == 801)
        hmdlSetFlagsFor(FindCharacterByScenarioName(AREA_347E_RYO_), 0, 8, 16);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        hmdlSetFlagsFor(FindCharacterByScenarioName(AREA_347E_RYO_), 0, 8, 16);
        child->parent->state = g_currentMap.name == AREA_347E.name ? 0 : 3;
        mtwkSetExtraFlag(scenFindMotionwork(AREA_347E_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_347E_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_347E_REN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_347E_REN_, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update347EAuthorTask(void *taskData) {
    Update347EAuthor(static_cast<Area347EAuthorParameter *>(taskData));
}

const ScriptAreaDefinition area347EScriptDefinition{.areaId = AREA_347E, .field04 = 0, .init = Initialize347E, .main = Start347E};

} // namespace shenmue::s2::areas
