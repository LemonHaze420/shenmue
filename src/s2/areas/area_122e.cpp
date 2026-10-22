#include "s2/areas/area_122e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize122E() {
}

// @verified - p
void Start122E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update122E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_122E_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_122E_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_122E_CHARACTERS, parent->self);
}

// @verified - p
void Update122E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update122EAuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
        static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        Prepare122ECharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup122E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
            Select122EPackage(parent->selection);
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 7;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 7)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 7) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_122E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup122E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_122E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_122E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Update122EAuthor(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore122ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update122EAuthorTask(void *taskData) {
    Update122EAuthor(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare122ECharacters() {
    ResetAreaAuthorCharacter(AREA_122E_RYO_, 0.0f);
    ResetAreaAuthorCharacter(AREA_122E_XGH_, 0.0f);
    ResetAreaAuthorCharacter(AREA_122E_COH_, 0.0f);
    const Vector3 position{};
    EV_AT_SetPos(AREA_122E_aa95, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_122E_aa95, false);
    EV_AT_SetPos(AREA_122E_a003, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_122E_a003, false);
    EV_AT_SetPos(AREA_122E_aa82, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_122E_aa82, false);
}

// @verified - p
void Restore122ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_122E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_122E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_122E_XGH_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_122E_XGH_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_122E_COH_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_122E_COH_, true);
    EV_SetCharacterAlphaEnabled(AREA_122E_aa95, true);
    EV_SetCharacterAlphaEnabled(AREA_122E_a003, true);
    EV_SetCharacterAlphaEnabled(AREA_122E_aa82, true);
}

// @verified - p
void Select122EPackage(std::int32_t selection) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_122E_FIRST_PACKAGE), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_122E_FIRST_PACKAGE), 0);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_122E_SECOND_PACKAGE), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_122E_SECOND_PACKAGE), 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    const char *const package = selection >= 6 ? AREA_122E_FIRST_PACKAGE : AREA_122E_SECOND_PACKAGE;
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetModelDataDefaultVertexShader();
    if (selection >= 6) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        return;
    }
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
}

const ScriptAreaDefinition area122EScriptDefinition{.areaId = AREA_122E, .field04 = 0, .init = Initialize122E, .main = Start122E};

} // namespace shenmue::s2::areas
