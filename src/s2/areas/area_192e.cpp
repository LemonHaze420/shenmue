#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_192e.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void Start192E(void)
{
    HLibTask *task;
    Area192EParameter *parent;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(Update192E, 4, 11, sizeof(Area192EParameter), TASK_EVNT);
    parent = (Area192EParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->selection = 0;
    if (g_currentMap == AREA_192E_SOURCE)
        parent->state = 1;
    else
        parent->state = 0;
    Setup192E();
    Queue_EVT_UTL_LoadNPCChara_3(area192ECharacters, parent->self);
}
#else
void Update192E(void *taskData)
{
    Area192EParameter *parent = (Area192EParameter *)taskData;
    switch (parent->state) {
    case 0: {
        unsigned int input = FTBL.EVT_GetControllerInput(0, 1);
        if (input == 1024) {
            parent->state = 1;
            Select192EPackage(parent->selection);
        } else if (input == 64) {
            parent->selection--;
            if (parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128) {
            if (++parent->selection > 1)
                parent->selection = 0;
        }
        switch (parent->selection) {
        case 0:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0192E000.c\n");
            break;
        case 1:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0192E010.c\n");
            break;
        }
        FTBL.SetDebugScreenTextPosition(2, 5);
        FTBL.DebugLogToScreen("seqdata%x.avb\n", parent->selection);
        break;
    }
    case 1:
        Queue192EAuthor(parent);
        break;
    case 3:
        Cleanup192E(parent);
        break;
    }
}

void Queue192EAuthor(Area192EParameter *parent)
{
    shen_name authors[2] = {SHEN_NAME4('1', '9', '2', 'A'), SHEN_NAME4('1', '9', '2', 'B')};
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update192EAuthor, 4, 11, sizeof(Area192EAuthorParameter), TASK_EVNT);
    ((Area192EAuthorParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    Hide192ECharacters();
    FTBL.EV_authSceneCtrl(authors[parent->selection]);
    parent->state = 2;
}

void Select192EPackage(int selection)
{
    if (FTBL.EV_ControlPackageResources(0, 5, "WT00", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "WT00", 0, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, "WTA0", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "WTA0", 0, 0);
    if (selection == 0) {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WT00", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "WT00", 0, 0);
    } else {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WTA0", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "WTA0", 0, 0);
    }
}

void Update192EAuthor(void *taskData)
{
    Area192EAuthorParameter *child = (Area192EAuthorParameter *)taskData;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        if (g_currentMap == AREA_192E_SOURCE)
            child->parent->state = 3;
        else
            child->parent->state = 0;
        Show192ECharacters();
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void Setup192E(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_FreeAuthorPak(1, "0192");
    FTBL.EV_authSceneCtrl(-8);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0192E", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "0192E", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0192E000");
}

const shen_name area192ECharacters[] = {AREA_192E_SYE_, INVALID_CHARACTER};

void Cleanup192E(Area192EParameter *parent)
{
    EVT_UTL_FreeNPCChara(area192ECharacters);
    FTBL.EV_FreeAuthorPak(0, "0192");
    FTBL.EV_ControlPackageResources(0, 6, "0192E", 0, 0);
    FTBL.HLib_DestroyTask(parent->self);
    if (g_currentMap == AREA_192E_SOURCE)
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_192E_RETURN, 1);
    else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

void Hide192ECharacters(void)
{
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorCharacter(AREA_192E_SYE_);
}

void Show192ECharacters(void)
{
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorCharacter(AREA_192E_SYE_);
}

#include "s2/events/event_author_dc.h"
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
#include "s2/event_utility_v1_dc.c"
#endif
#else
#include "s2/areas/area_192e.hpp"
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

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize192E() {
}

// @verified - p
void Start192E(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{});
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update192E, 4, 0x0b, sizeof(Area192EParameter), AUTHOR_VIEWER_TASK);
    Area192EParameter *const parent = static_cast<Area192EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = g_currentMap.name == AREA_192E_SOURCE.name;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_192E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_192E_CHARACTERS, parent->self);
}

// @verified - p
void Update192E(void *taskData) {
    Area192EParameter *const parent = static_cast<Area192EParameter *>(taskData);
    if (parent->state == 1) {
        Queue192EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup192E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
            Select192EPackage(parent->selection);
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 2;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 2)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_192E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup192E(Area192EParameter *parent) {
    for (const shen_name *character = AREA_192E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_192E_CONVERSATION);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_192E_ARCHIVE), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_192E_SOURCE.name)
        EV_scenQueueLoad(g_actNumber, AREA_192E_RETURN, 1);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue192EAuthor(Area192EParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update192EAuthorTask, 4, 0x0b, sizeof(Area192EAuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area192EAuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_192E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_SYE_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_192E_SYE_, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_192E_AUTHORS[parent->selection].name));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update192EAuthor(Area192EAuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = g_currentMap.name == AREA_192E_SOURCE.name ? 3 : 0;
        mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_192E_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_SYE_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_192E_SYE_, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update192EAuthorTask(void *taskData) {
    Update192EAuthor(static_cast<Area192EAuthorParameter *>(taskData));
}

// @verified - p
std::int64_t Select192EPackage(std::int32_t selection) {
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_192E_FIRST_PACKAGE), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_192E_FIRST_PACKAGE), 0);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_192E_SECOND_PACKAGE), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_192E_SECOND_PACKAGE), 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    const char *const package = selection ? AREA_192E_SECOND_PACKAGE : AREA_192E_FIRST_PACKAGE;
    EV_pakUtils(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    return EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

const ScriptAreaDefinition area192EScriptDefinition{.areaId = AREA_192E, .field04 = 0, .init = Initialize192E, .main = Start192E};

} // namespace shenmue::s2::areas
#endif
