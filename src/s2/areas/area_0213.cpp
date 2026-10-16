#include "s2/areas/area_0213.hpp"
#include "s2/scene_data.hpp"
#include "s2/pak.hpp"
#include "s2/character.hpp"

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
#include "s2/npc.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0213MainCallback(void *taskData);
void E0213LoopCallback(void *taskData);
void CleanupE0213(AreaAuthorTaskParameter *parent);

// @verified - p
void E0213Init() {
}

// @verified - p
void ConfigureE0213Environment() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0213_CONVERSATION);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("MAP"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    const Vector3 position{0.0f, 14.4f, 0.0f};
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapModelSlotTransform(index, &position, 0);
}

// @verified - p
void E0213Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0213MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0213_MOTION));
    parent->selection = static_cast<std::int16_t>(g_entryPoint);
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0213_CHARACTERS, parent->self);
    ConfigureE0213Environment();
    if (parent->selection > 1)
        CleanupE0213(parent);
}

// @verified - p
void SetupE0213Characters() {
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_EnableCharacterDisplay(AREA_0213_A330);
    EV_SetCharaControllerFlags(AREA_0213_A330, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0213_A330, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_0213_A330, false);
    for (const shen_name *character = AREA_0213_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0213Characters() {
    EnableCharDisplayAndSetMotionworkData(AREA_AUTHOR_RYO);
    EV_EnableCharacterDisplay(AREA_0213_A330);
    EV_SetCharaControllerFlags(AREA_0213_A330, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0213_A330, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0213_A330, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0213_A330, true);
    for (const shen_name *character = AREA_0213_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0213(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0213_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("MAP"), 0);
    EV_FreeAuthorPak(0, AREA_0213_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0213Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0213LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (parent->selection != 0)
        EV_LoadAudio("0213A000", 0);
    SetupE0213Characters();
    EV_cuffsAttach(AREA_AUTHOR_RYO, 1, AREA_AUTHOR_REN, 0, 0.3f);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0213MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0213Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0213(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128 && ++parent->selection > 1)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0213_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0213LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    EV_cuffsSetFlags();
    RestoreE0213Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0213ScriptDefinition{.areaId = AREA_0213_AREA, .field04 = 0, .init = E0213Init, .main = E0213Main};

} // namespace shenmue::s2::areas
