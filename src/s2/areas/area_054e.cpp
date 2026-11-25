#include "s2/areas/area_054e.hpp"
#include "s2/events/event_api.h"

#if !defined(SHENMUE_EVENT_OVERLAY)
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
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
#define E054E_INLINE
#define E054E_LOCAL
#else
#define E054E_LOCAL static
#if defined(_MSC_VER)
#define E054E_INLINE __forceinline
#else
#define E054E_INLINE inline __attribute__((always_inline))
#endif
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
typedef shen_name E054ECharacterName;
#else
typedef const shen_name &E054ECharacterName;
#endif

E054E_LOCAL E054E_INLINE void Setup054E(void);
E054E_LOCAL E054E_INLINE void Prepare054ECharacter(E054ECharacterName name);
E054E_LOCAL E054E_INLINE void Restore054ECharacter(E054ECharacterName name);
E054E_LOCAL E054E_INLINE void Hide054EProp(E054ECharacterName name);
E054E_LOCAL E054E_INLINE void Show054EProp(E054ECharacterName name);

#if !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void Initialize054E() {
}
#endif

#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void Start054E(
#if defined(SHENMUE_EVENT_OVERLAY)
    const int *argument
#else
    std::uintptr_t argument
#endif
) {
    HLibTask *task;
    Area054EParameter *parent;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(Update054E, 4, 0x0b, sizeof(Area054EParameter), AUTHOR_VIEWER_TASK);
    parent = (Area054EParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_054E) {
#else
    if (g_currentMap.name == AREA_054E.name) {
#endif
        parent->selection = 0;
        parent->state = 0;
    } else {
#if defined(SHENMUE_EVENT_OVERLAY)
        parent->selection = *argument > 1 ? 0 : *(const short *)argument;
#else
        const std::int32_t selection = *reinterpret_cast<const std::int32_t *>(argument);
        parent->selection = selection <= 1 ? std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(selection)) : 0;
#endif
        parent->state = 1;
    }
    Setup054E();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_054E_CHARACTERS, parent->self);
}
#endif

#if !defined(SHENMUE_EVENT_ENTRY)
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_054E_CHARACTERS[] = {AREA_054E_AGO_, AUTHOR_VIEWER_INVALID_CHARACTER};
#endif

// @verified - p
void Update054E(void *taskData) {
    Area054EParameter *const parent = (Area054EParameter *)taskData;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 0: {
        int input = FTBL.EVT_GetControllerInput(0, 1);
#else
    if (parent->state == 1) {
        Queue054EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup054E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
#endif
        if (input == PAD_BUTTON_X) {
            parent->state = 1;
        } else if (input == PAD_TRIGGER_LEFT) {
#if defined(SHENMUE_EVENT_OVERLAY)
            parent->selection--;
#else
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
#endif
            if (parent->selection < 0)
                parent->selection = 1;
        } else if (input == PAD_TRIGGER_RIGHT) {
#if defined(SHENMUE_EVENT_OVERLAY)
            if (++parent->selection > 1)
#else
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 1)
#endif
                parent->selection = 0;
        }
#if defined(SHENMUE_EVENT_OVERLAY)
        switch (parent->selection) {
        case 0:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0054E000.c\n");
            break;
        case 1:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0054E010.c\n");
            break;
        }
        FTBL.SetDebugScreenTextPosition(2, 5);
        FTBL.DebugLogToScreen("seqdata%x.avb\n", parent->selection);
        break;
    }
    case 1:
        Queue054EAuthor(parent);
        break;
    case 3:
        Cleanup054E(parent);
        break;
#else
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_054E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
#endif
    }
}

#if defined(SHENMUE_EVENT_OVERLAY)
void Queue054EAuthor(Area054EParameter *parent)
#else
// @verified - p
std::int64_t Queue054EAuthor(Area054EParameter *parent)
#endif
{
    HLibTask *task;
#if defined(SHENMUE_EVENT_OVERLAY)
    shen_name authors[] = {SHEN_NAME4('0', '5', '4', 'A'), SHEN_NAME4('0', '5', '4', 'B')};
    task = FTBL.HLib_EnqueueTaskWithParameter(Update054EAuthor, 4, 0x0b, sizeof(Area054EAuthorParameter), AUTHOR_VIEWER_TASK);
#else
    task = FTBL.HLib_EnqueueTaskWithParameter(Update054EAuthorTask, 4, 0x0b, sizeof(Area054EAuthorParameter), AUTHOR_VIEWER_TASK);
#endif
    ((Area054EAuthorParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    Prepare054ECharacters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(authors[parent->selection]);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_054E_AUTHORS[parent->selection].name));
#endif
    parent->state = 2;
#if !defined(SHENMUE_EVENT_OVERLAY)
    return 2;
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void Update054EAuthor(void *taskData) {
    Area054EAuthorParameter *const child = (Area054EAuthorParameter *)taskData;
    const int result = FTBL.EV_authSceneCtrl(-1);
#else
// @verified - p
std::int64_t Update054EAuthor(Area054EAuthorParameter *child) {
    const std::int64_t result = FTBL.EV_authSceneCtrl(-1);
#endif
    if ((unsigned int)result == 0) {
#if defined(SHENMUE_EVENT_OVERLAY)
        if (g_currentMap == AREA_054E)
            child->parent->state = 0;
        else
            child->parent->state = 3;
#else
        child->parent->state = g_currentMap.name == AREA_054E.name ? 0 : 3;
#endif
        Restore054ECharacters();
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.HLib_DestroyCurrentTask();
#else
        return FTBL.HLib_DestroyCurrentTask();
#endif
    }
#if !defined(SHENMUE_EVENT_OVERLAY)
    return result;
#endif
}

E054E_LOCAL E054E_INLINE void Setup054E(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_FreeAuthorPak(1, AREA_054E_CONVERSATION);
    FTBL.EV_authSceneCtrl(-8);
    FTBL.EV_LoadCutsceneFromCurrentScene(AREA_054E_AUDIO);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(-11);
#endif
}

// @verified - p
void Cleanup054E(Area054EParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_054E_CHARACTERS);
#else
    for (const shen_name *character = AREA_054E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_authSceneCtrl(-10);
    FTBL.EV_FreeAuthorPak(0, AREA_054E_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, AREA_054E_ARCHIVE, 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_054E)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
#else
    if (g_currentMap.name == AREA_054E.name)
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_054E, g_entryPoint);
#endif
    else
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_054E_RETURN, 1);
}

// @verified - p
void Prepare054ECharacters(void) {
    Prepare054ECharacter(AREA_054E_RYO_);
    Prepare054ECharacter(AREA_054E_AGO_);
    Hide054EProp(AREA_054E_a417);
    Hide054EProp(AREA_054E_aa50);
    Hide054EProp(AREA_054E_a073);
}
// @verified - p
void Restore054ECharacters(void) {
    Restore054ECharacter(AREA_054E_RYO_);
    Restore054ECharacter(AREA_054E_AGO_);
    Show054EProp(AREA_054E_a417);
    Show054EProp(AREA_054E_aa50);
    Show054EProp(AREA_054E_a073);
}

E054E_LOCAL E054E_INLINE void Prepare054ECharacter(E054ECharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E054E_LOCAL E054E_INLINE void Restore054ECharacter(E054ECharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E054E_LOCAL E054E_INLINE void Hide054EProp(E054ECharacterName name) { FTBL.EV_SetCharacterAlphaEnabled(name, 0); }
E054E_LOCAL E054E_INLINE void Show054EProp(E054ECharacterName name) { FTBL.EV_SetCharacterAlphaEnabled(name, 1); }

#if !defined(SHENMUE_EVENT_OVERLAY)
void Update054EAuthorTask(void *taskData) {
    Update054EAuthor(static_cast<Area054EAuthorParameter *>(taskData));
}

const ScriptAreaDefinition area054EScriptDefinition{.areaId = AREA_054E, .field04 = 0, .init = Initialize054E, .main = Start054E};
} // namespace shenmue::s2::areas
#endif
#endif
#undef E054E_INLINE
#undef E054E_LOCAL
