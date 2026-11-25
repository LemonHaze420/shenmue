#include "s2/areas/area_0184.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
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

// @verified - p
void E0184Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0184_INLINE
typedef shen_name E0184CharacterName;
#else
#if defined(_MSC_VER)
#define E0184_INLINE __forceinline
#else
#define E0184_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0184CharacterName;
#endif
void E0184MainCallback(void *);
void E0184LoopCallback(void *);
E0184_INLINE void QueueE0184Loop(E0184TaskParameter *);
E0184_INLINE void Configure0184(void);
E0184_INLINE void CleanupE0184(E0184TaskParameter *);
E0184_INLINE void SetupE0184Characters(void);
E0184_INLINE void RestoreE0184Characters(void);
E0184_INLINE void Prepare0184Character(E0184CharacterName);
E0184_INLINE void Restore0184Character(E0184CharacterName);
E0184_INLINE void Hide0184Prop(E0184CharacterName);
E0184_INLINE void Show0184Prop(E0184CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0184Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    E0184TaskParameter *parent;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(E0184MainCallback, 4, 11, sizeof(E0184TaskParameter), AREA_AUTHOR_TASK);
    parent = (E0184TaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->reserved08 = 0;
#else
    parent->reserved08 = 0;
    parent->state = 1;
#endif
    Configure0184();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0184_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0184MainCallback(void *data) {
    E0184TaskParameter *parent = (E0184TaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0184Loop(parent);
        break;
    case 3:
        CleanupE0184(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0184Loop(parent);
    else if (parent->state == 3)
        CleanupE0184(parent);
#endif
}
E0184_INLINE void QueueE0184Loop(E0184TaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0184LoopCallback, 4, 11, sizeof(E0184FrameParameter), AREA_AUTHOR_TASK);
    E0184FrameParameter *child = (E0184FrameParameter *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->state = 0;
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    SetupE0184Characters();
    FTBL.EV_authSceneCtrl(AREA_0184_AREA);
    parent->state = 2;
}
void E0184LoopCallback(void *data) {
    E0184FrameParameter *child = (E0184FrameParameter *)data;
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
    RestoreE0184Characters();
    child->parent->state = 3;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.HLib_DestroyCurrentTask();
}
E0184_INLINE void Configure0184(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0184A000");
    FTBL.EV_FreeAuthorPak(1, "0184");
}
E0184_INLINE void CleanupE0184(E0184TaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0184_CHARACTERS);
#else
    for (const shen_name *character = AREA_0184_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0184_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0184A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0184_AREA)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
#else
    if (g_currentMap.name == AREA_0184_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0184_AREA, g_entryPoint);
#endif
    else {
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 103.4f, 100.0f, 204.43f, 0);
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 270.7f, 0.0f, 0);
#else
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {103.4000015258789f, 100.0f, 204.42999267578125f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 49279, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}
E0184_INLINE void SetupE0184Characters(void) {

#if defined(SHENMUE_EVENT_OVERLAY)
    int i;
#endif
    Prepare0184Character(AREA_AUTHOR_RYO);
    Hide0184Prop(AREA_0184_A191);
    Hide0184Prop(AREA_0184_A192);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (i = 0; AREA_0184_CHARACTERS[i] != AREA_AUTHOR_INVALID; i++)
        Prepare0184Character(AREA_0184_CHARACTERS[i]);
#else
    for (const shen_name *character = AREA_0184_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        Prepare0184Character(*character);
#endif
}
E0184_INLINE void RestoreE0184Characters(void) {

#if defined(SHENMUE_EVENT_OVERLAY)
    int i;
#endif
    Restore0184Character(AREA_AUTHOR_RYO);
    Show0184Prop(AREA_0184_A191);
    Show0184Prop(AREA_0184_A192);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (i = 0; AREA_0184_CHARACTERS[i] != AREA_AUTHOR_INVALID; i++)
        Restore0184Character(AREA_0184_CHARACTERS[i]);
#else
    for (const shen_name *character = AREA_0184_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        Restore0184Character(*character);
#endif
}
E0184_INLINE void Prepare0184Character(E0184CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0184_INLINE void Restore0184Character(E0184CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0184_INLINE void Hide0184Prop(E0184CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0184_INLINE void Show0184Prop(E0184CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0184_CHARACTERS[] = {AREA_0184_LKN_, AREA_0184_BKN_, AREA_0184_MYS_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0184ScriptDefinition{.areaId = AREA_0184_AREA, .field04 = 0, .init = E0184Init, .main = E0184Main};
} // namespace shenmue::s2::areas
#endif
#undef E0184_INLINE
