#include "s2/areas/area_0080.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"

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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

struct E0080LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
};
// @verified - p
void E0080Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0080_INLINE
typedef shen_name E0080CharacterName;
#else
#if defined(_MSC_VER)
#define E0080_INLINE __forceinline
#else
#define E0080_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0080CharacterName;
#endif
void E0080MainCallback(void *);
void E0080LoopCallback(void *);
E0080_INLINE void QueueE0080Loop(AreaImmediateAuthorTaskParameter *);
E0080_INLINE void ConfigureE0080Environment(void);
E0080_INLINE void CleanupE0080(AreaImmediateAuthorTaskParameter *);
E0080_INLINE void Setup0080Characters(void);
E0080_INLINE void Restore0080Characters(void);
E0080_INLINE void Prepare0080Character(E0080CharacterName);
E0080_INLINE void Restore0080Character(E0080CharacterName);
E0080_INLINE void Hide0080Prop(E0080CharacterName);
E0080_INLINE void Show0080Prop(E0080CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0080Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
    FTBL.EventFunction870(0);
    FTBL.SetScenarioFollowCharacter(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(E0080MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    ConfigureE0080Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0080_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0080MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0080Loop(parent);
        break;
    case 3:
        CleanupE0080(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0080Loop(parent);
    else if (parent->state == 3)
        CleanupE0080(parent);
#endif
}
E0080_INLINE void QueueE0080Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0080LoopCallback, 4, 11, sizeof(E0080LoopParameter), AREA_AUTHOR_TASK);
    ((E0080LoopParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    Setup0080Characters();
    FTBL.EV_authSceneCtrl(AREA_0080_AREA);
    parent->state = 2;
}
void E0080LoopCallback(void *data) {
    E0080LoopParameter *child = (E0080LoopParameter *)data;
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    Restore0080Characters();
    child->parent->state = 3;
    FTBL.HLib_DestroyCurrentTask();
}
E0080_INLINE void ConfigureE0080Environment(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
#endif
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(-11);
#else
    FTBL.EV_authSceneCtrl(-10);
#endif
    FTBL.EV_LoadCutsceneFromCurrentScene("0080A000");
    FTBL.EV_FreeAuthorPak(1, AREA_0080_CONVERSATION);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.GetCurrentDateTime((DateTime *)&time);
#else
    DateTime time;
    shenGetCurrentDateTime(time);
#endif

    if ((unsigned char)(time.hour - 19) <= 5) {
        FTBL.EV_lightLoad(AREA_0080_0000);
        FTBL.PushGlobalLightingProgram();
        FTBL.SetObjectFlagByIndex(7, 1);
        FTBL.SetObjectFlagByIndex(6, 0);
    }
}
E0080_INLINE void CleanupE0080(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0080_CHARACTERS);
#else
    for (const shen_name *character = AREA_0080_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_authSceneCtrl(-10);
    FTBL.EV_FreeAuthorPak(0, AREA_0080_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0080A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0080_AREA)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
#else
    if (g_currentMap.name == AREA_0080_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0080_AREA, g_entryPoint);
#endif
    else
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0080_AKY0, 0);
}
E0080_INLINE void Setup0080Characters(void) {
    Prepare0080Character(AREA_AUTHOR_RYO);
    Prepare0080Character(AREA_AUTHOR_REN);
    Hide0080Prop(AREA_0080_AA48);
}
E0080_INLINE void Restore0080Characters(void) {
    Restore0080Character(AREA_AUTHOR_RYO);
    Restore0080Character(AREA_AUTHOR_REN);
    Show0080Prop(AREA_0080_AA48);
}
E0080_INLINE void Prepare0080Character(E0080CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0080_INLINE void Restore0080Character(E0080CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0080_INLINE void Hide0080Prop(E0080CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0080_INLINE void Show0080Prop(E0080CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0080_CHARACTERS[] = {AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0080ScriptDefinition{.areaId = AREA_0080_AREA, .field04 = 0, .init = E0080Init, .main = E0080Main};
} // namespace shenmue::s2::areas
#endif
#undef E0080_INLINE
