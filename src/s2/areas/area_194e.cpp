#include "s2/areas/area_194e.hpp"
#include "s2/input.hpp"
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
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize194E() {
}

// @verified - p
void Start194E(std::uintptr_t argument) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update194E, 4, 0x0b, sizeof(Area194EParameter), AUTHOR_VIEWER_TASK);
    Area194EParameter *const parent = static_cast<Area194EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_194E_CONVERSATION);
    if (g_currentMap.name == AREA_194E.name) {
        parent->selection = parent->state = 0;
        Queue_EVT_UTL_LoadNPCChara_3(AREA_194E_CHARACTERS, task);
    } else {
        parent->selection = *reinterpret_cast<const std::int16_t *>(argument);
        parent->state = 1;
        Queue_EVT_UTL_LoadNPCChara_3(AREA_194E_VARIANTS[parent->selection], task);
    }
}

// @verified - p
void Update194E(void *taskData) {
    Area194EParameter *const parent = static_cast<Area194EParameter *>(taskData);
    if (parent->state == 1) {
        Queue194EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup194E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            parent->state = 1;
            Select194EPackage(parent->selection);
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 3;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 3)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 3) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_194E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup194E(Area194EParameter *parent) {
    const shen_name *character = g_currentMap.name == AREA_194E.name ? AREA_194E_CHARACTERS : AREA_194E_VARIANTS[parent->selection];
    for (; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    const char *archive = nullptr;
    if (g_currentMap.name == AREA_194E.name)
        archive = "0194E";
    else if (parent->selection == 0)
        archive = "0194E1";
    else if (parent->selection == 1)
        archive = "0194E2";
    else if (parent->selection == 2)
        archive = "0194E3";
    if (archive)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(archive), 0);
    EV_FreeAuthorPak(0, AREA_194E_CONVERSATION);
    EV_CleanupMapControlTask();
    if (g_currentMap.name == AREA_194E.name) {
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    } else {
        ExecuteCleanupFuncForAreaWithCachedScene();
        EventSetPosition position{0, AREA_194E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {}, 0.0f};
        if (parent->selection == 0) {
            npcSetAreaName(AREA_194E_NPC_AREA, nullptr, 0.0f, 0);
            position.position = {231.36000061035156f, 104.5f, 178.9499969482422f};
            EVT_AT_SetPosRot(&position);
            EVT_AT_SetAng_Enqueue(AREA_194E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 0.0f, 0.0f, 0);
        } else if (parent->selection == 1) {
            EV_NPC_NewLoadSw(1);
            doorLoadAuthorSilently(shen_name{});
            EV_LoadCutsceneFromCurrentScene("WN00");
            ExecuteCleanupFuncForAreaWithCachedScene();
            position.position = {230.86000061035156f, 104.5f, 174.3000030517578f};
            EVT_AT_SetPosRot(&position);
            EVT_AT_SetAng_Enqueue(AREA_194E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 90.0f, 0.0f, 0);
        } else if (parent->selection == 2) {
            EV_EnableCharacterDisplay(AREA_194E_LIGHT_0001);
            EV_NPC_NewLoadSw(1);
            position.position = {231.6999969482422f, 111.80999755859375f, 222.97999572753906f};
            EVT_AT_SetPosRot(&position);
            EVT_AT_SetAng_Enqueue(AREA_194E_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 0.0f, 0.0f, 0);
        }
        HLib::DestroyTask(parent->self);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
    }
}

// @verified - p
std::int64_t Queue194EAuthor(Area194EParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update194EAuthorTask, 4, 0x0b, sizeof(Area194EAuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area194EAuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    if (parent->selection == 0) {
        const Vector3 center{232.8000030517578f, 104.5f, 178.14999389648438f};
        npcSetAreaName(AREA_194E_NPC_AREA, &center, 5.0f, 1);
        EV_NPC_NewLoadSw(1);
        EV_LoadCutsceneFromCurrentScene("0194E000");
    } else if (parent->selection == 1) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else if (parent->selection == 2 && g_currentMap.name == AREA_194E_HIDE_MAP.name) {
        EV_DisableCharacterDisplay(AREA_194E_LIGHT_0001);
    }
    Prepare194ECharacters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_194E_AUTHORS[parent->selection].name));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update194EAuthor(Area194EAuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = g_currentMap.name == AREA_194E.name ? 0 : 3;
        Restore194ECharacters(child->parent->selection);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update194EAuthorTask(void *taskData) {
    Update194EAuthor(static_cast<Area194EAuthorParameter *>(taskData));
}

// @verified - p
void Prepare194ECharacters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_194E_RYO_, false);
    switch (selection) {
    case 0:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_KUN_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_194E_KUN_, false);
        break;
    case 1:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_SIS_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_194E_SIS_, false);
        EV_SetCharacterAlphaEnabled(AREA_194E_aa76, false);
        break;
    case 2:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_KUN_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_194E_KUN_, false);
        EV_SetCharacterAlphaEnabled(AREA_194E_a044, false);
        EV_SetCharacterAlphaEnabled(AREA_194E_a046, false);
        EV_SetCharacterAlphaEnabled(AREA_194E_aa76, false);
        break;
    case 3:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_KUN_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_194E_KUN_, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_EDL_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_194E_EDL_, false);
        break;
    default:
        break;
    }
}

// @verified - p
void Restore194ECharacters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_194E_RYO_, false);
    switch (selection) {
    case 0:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_KUN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_194E_KUN_, true);
        break;
    case 1:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_SIS_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_194E_SIS_, true);
        EV_SetCharacterAlphaEnabled(AREA_194E_aa76, true);
        break;
    case 2:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_KUN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_194E_KUN_, true);
        EV_SetCharacterAlphaEnabled(AREA_194E_a044, true);
        EV_SetCharacterAlphaEnabled(AREA_194E_a046, true);
        EV_SetCharacterAlphaEnabled(AREA_194E_aa76, true);
        break;
    case 3:
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_KUN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_194E_KUN_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_194E_EDL_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_194E_EDL_, true);
        break;
    default:
        break;
    }
}

// @verified - p
void Select194EPackage(std::int32_t selection) {
    constexpr const char *packages[]{"AKS1", "WN00", "WNX9", "WTA0"};
    for (const char *package : packages) {
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    }
    EV_SetModelDataDefaultVertexShader();
    const char *package;
    if (selection == 0)
        package = "WN00";
    else if (selection == 1)
        package = "WNX9";
    else if (selection == 2)
        package = "WTA0";
    else if (selection == 3)
        package = "AKS1";
    else
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    if (selection == 1) {
        EV_LoadCutsceneFromCurrentScene(package);
        EV_lightLoad(AREA_194E_LIGHT_0001);
        PushGlobalLightingProgram();
    } else if (selection == 2) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        EV_LoadCutsceneFromCurrentScene(package);
    } else {
        EV_LoadCutsceneFromCurrentScene(selection == 0 ? "0194E000" : package);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
}

const ScriptAreaDefinition area194EScriptDefinition{.areaId = AREA_194E, .field04 = 0, .init = Initialize194E, .main = Start194E};

} // namespace shenmue::s2::areas
