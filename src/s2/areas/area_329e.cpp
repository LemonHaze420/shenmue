#include "s2/areas/area_329e.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/pak.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
void Initialize329E() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E329E_INLINE
#define E329E_LOCAL
typedef shen_name E329ECharacterName;
#else
#if defined(_MSC_VER)
#define E329E_INLINE __forceinline
#else
#define E329E_INLINE inline __attribute__((always_inline))
#endif
#define E329E_LOCAL static
typedef const shen_name &E329ECharacterName;
#endif
E329E_LOCAL E329E_INLINE void Setup329EScene(void);
E329E_LOCAL E329E_INLINE void Queue329EAuthor(Area329EParameter *);
E329E_LOCAL E329E_INLINE void Prepare329ECharacter(E329ECharacterName);
E329E_LOCAL E329E_INLINE void Restore329ECharacter(E329ECharacterName);
E329E_LOCAL E329E_INLINE void Hide329EProp(E329ECharacterName);
E329E_LOCAL E329E_INLINE void Show329EProp(E329ECharacterName);
#if !defined(SHENMUE_EVENT_OVERLAY)
static E329E_INLINE void Prepare329ECharactersBody(void);
#endif
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Start329E(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 center = {123.38f, 92.8f, 184.15f};
#endif
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update329E, 4, 11, sizeof(Area329EParameter), AREA_329E_TASK);
    Area329EParameter *parent = (Area329EParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->selection = 0;
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_329E_RETURN_MAP)
        parent->state = 1;
    else
        parent->state = 0;
    if (FTBL.npcSetAreaName(AREA_329E_NPC_AREA, &center, 4.0f, 1) == 1)
        FTBL.DebugLogToFileRemoved("NPC_AREA : TRUE\n");
    else
        FTBL.DebugLogToFileRemoved("NPC_AREA : FALSE\n");
#else
    parent->state = g_currentMap.name == AREA_329E_RETURN_MAP.name ? 1 : 0;
    const Vector3 center{123.38f, 92.8f, 184.34f};
    npcSetAreaName(AREA_329E_NPC_AREA, &center, 4.0f, 1);
#endif
    Setup329EScene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_329E_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void Update329E(void *data) {
    Area329EParameter *parent = (Area329EParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 0: {
        int input = FTBL.EVT_GetControllerInput(0, 1);
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 0;
        } else if (input == 128) {
            if (++parent->selection > 0)
                parent->selection = 0;
        }
    }
#else
    if (parent->state == 1)
        Queue329EAuthor(parent);
    else if (parent->state == 3)
        parent->state = 4;
    else if (parent->state == 4)
        Cleanup329E(parent);
    else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64 || input == 128)
            parent->selection = 0;
#endif
        if (parent->selection == 0) {
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0329E000.c\n");
        }
        FTBL.SetDebugScreenTextPosition(2, 5);
        FTBL.DebugLogToScreen("seqdata%d.avb\n", parent->selection);
#if defined(SHENMUE_EVENT_OVERLAY)
        break;
    case 3:
        parent->state = 4;
        break;
    case 1:
        Queue329EAuthor(parent);
        break;
    case 4:
        Cleanup329E(parent);
        break;
    }
#else
    }
#endif
}
E329E_LOCAL E329E_INLINE void Queue329EAuthor(Area329EParameter *parent) {
    HLibTask *task;
    Area329EAuthorParameter *child;
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithParameter(Update329EAuthor, 4, 11, sizeof(Area329EAuthorParameter), AREA_329E_TASK);
#else
    task = FTBL.HLib_EnqueueTaskWithParameter(Update329EAuthorTask, 4, 11, sizeof(Area329EAuthorParameter), AREA_329E_TASK);
#endif
    child = (Area329EAuthorParameter *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->frame = 0;
    FTBL.EV_NPC_NewLoadSw_0(1);
#if defined(SHENMUE_EVENT_OVERLAY)
    Prepare329ECharacters();
#else
    Prepare329ECharactersBody();
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(AREA_329E_AUTHOR);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_329E_AUTHOR.name));
#endif
    parent->state = 2;
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Update329EAuthor(void *data)
#else
std::int64_t Update329EAuthor(Area329EAuthorParameter *child)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    Area329EAuthorParameter *child = (Area329EAuthorParameter *)data;
    int result = FTBL.EV_authSceneCtrl(-1);
#else
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
#endif
    if (result == 0) {
#if defined(SHENMUE_EVENT_OVERLAY)
        if (g_currentMap == AREA_329E_RETURN_MAP) {
            child->parent->state = 3;
            EVT_AT_SetPos(AREA_329E_RYO, INVALID_CHARACTER, 122.01f, 92.8f, 184.34f, 0);
            EVT_AT_SetAng(AREA_329E_RYO, INVALID_CHARACTER, 0.0f, 274.54f, 0.0f, 0);
#else
        if (g_currentMap.name == AREA_329E_RETURN_MAP.name) {
            child->parent->state = 3;
            EventSetPosition position{0, AREA_329E_RYO, shen_name{0xffffffffU}, {122.01f, 92.8f, 184.34f}, 0.0f};
            EVT_AT_SetPosRot(&position);
            EventSetRawAngle angle{0, AREA_329E_RYO, shen_name{0xffffffffU}, {0, 49978, 0}, 0.0f};
            EVT_AT_SetAng_Callback(&angle);
#endif
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.camCtrlSetState(0);
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            FTBL.InitiateCleanupForSceneChara(AREA_329E_GAI);
            FTBL.HLib_SetupTimedTask(child->parent->self, 30);
        } else {
            child->parent->state = 0;
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        Restore329ECharacters();
#if !defined(SHENMUE_EVENT_OVERLAY)
        return
#endif
            FTBL.HLib_DestroyCurrentEventTask();
    }
#if !defined(SHENMUE_EVENT_OVERLAY)
    return result;
#endif
}
#if !defined(SHENMUE_EVENT_OVERLAY)
void Update329EAuthorTask(void *data) {
    Update329EAuthor(static_cast<Area329EAuthorParameter *>(data));
}
#endif
E329E_LOCAL E329E_INLINE void Setup329EScene(void) {
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_329E) {
#else
    if (g_currentMap.name == AREA_329E.name) {
#endif
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
    }
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0329E000");
}
// @verified - pd
void Cleanup329E(Area329EParameter *parent) {
    FTBL.SetPlayerControlEnabled(1);
    FTBL.npcLockUnlock(AREA_329E_GAI);
    FTBL.npcSetAreaName(AREA_329E_NPC_AREA, 0, 0.0f, 0);
    FTBL.EV_ControlPackageResources(0, 6, "0329E", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_329E_RETURN_MAP) {
#else
    if (g_currentMap.name == AREA_329E_RETURN_MAP.name) {
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.EV_spmBeginSampleExecute(0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void Prepare329ECharacters(void)
#else
static E329E_INLINE void Prepare329ECharactersBody(void)
#endif
{
    Prepare329ECharacter(AREA_329E_RYO);
    Prepare329ECharacter(AREA_329E_GAI);
    Hide329EProp(AREA_329E_PROP0);
    Hide329EProp(AREA_329E_PROP1);
}
#if !defined(SHENMUE_EVENT_OVERLAY)
void Prepare329ECharacters(void) {
    Prepare329ECharactersBody();
}
#endif
// @verified - pd
void Restore329ECharacters(void) {
    Restore329ECharacter(AREA_329E_RYO);
    Prepare329ECharacter(AREA_329E_GAI);
    Show329EProp(AREA_329E_PROP0);
    Show329EProp(AREA_329E_PROP1);
}
E329E_LOCAL E329E_INLINE void Prepare329ECharacter(E329ECharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E329E_LOCAL E329E_INLINE void Restore329ECharacter(E329ECharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E329E_LOCAL E329E_INLINE void Hide329EProp(E329ECharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E329E_LOCAL E329E_INLINE void Show329EProp(E329ECharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_329E_CHARACTERS[] = {AREA_329E_GAI, INVALID_CHARACTER};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition area329EScriptDefinition{.areaId = AREA_329E, .field04 = 0, .init = Initialize329E, .main = Start329E};
} // namespace shenmue::s2::areas
#endif
#undef E329E_INLINE
#undef E329E_LOCAL
