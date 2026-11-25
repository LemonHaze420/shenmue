#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/debug.hpp"
#endif
#include "s2/areas/area_0173.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
// @verified - p
void E0173Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0173_INLINE
typedef shen_name E0173CharacterName;
#else
#if defined(_MSC_VER)
#define E0173_INLINE __forceinline
#else
#define E0173_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0173CharacterName;
#endif
void E0173MainCallback(void *);
void E0173LoopCallback(void *);
E0173_INLINE void QueueE0173Loop(AreaImmediateAuthorTaskParameter *);
E0173_INLINE void Configure0173(void);
E0173_INLINE void CleanupE0173(AreaImmediateAuthorTaskParameter *);
E0173_INLINE void Setup0173Characters(void);
E0173_INLINE void Restore0173Characters(void);
E0173_INLINE void Prepare0173Character(E0173CharacterName);
E0173_INLINE void Restore0173Character(E0173CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0173Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const Vector3 center = {270.8f, 114.6f, 279.7f};
#endif
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0173MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (FTBL.npcSetAreaName(AREA_0173_ANP1, &center, 5.0f, 1) == 1)
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("NPC_AREA1 : TRUE\n");
#else
        EV_debug_log("0173\\e0173_et_0173.c", 96, "NPC_AREA1 : TRUE\n");
#endif
    else
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("NPC_AREA1 : FALSE\n");
#else
        EV_debug_log("0173\\e0173_et_0173.c", 98, "NPC_AREA1 : FALSE\n");
#endif
#endif
    Configure0173();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0173_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0173MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0173Loop(parent);
        break;
    case 3:
        CleanupE0173(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0173Loop(parent);
    else if (parent->state == 3)
        CleanupE0173(parent);
#endif
}
E0173_INLINE void QueueE0173Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0173LoopCallback, 4, 11, sizeof(AreaImmediateAuthorFrameParameter), AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)FTBL.HLib_GetTaskParameter(task);
#if defined(SHENMUE_EVENT_OVERLAY)
    child->parent = parent;
    child->state = 0;
#else
    child->state = 0;
    child->parent = parent;
#endif
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    Setup0173Characters();
    FTBL.EV_authSceneCtrl(AREA_0173_AREA);
    parent->state = 2;
}
void E0173LoopCallback(void *data) {
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)data;
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    Restore0173Characters();
    child->parent->state = 3;
    FTBL.HLib_DestroyCurrentTask();
}
E0173_INLINE void Configure0173(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0173A000");
    FTBL.EV_FreeAuthorPak(1, "0173");
}
E0173_INLINE void CleanupE0173(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0173_CHARACTERS);
#else
    for (const shen_name *character = AREA_0173_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_FreeAuthorPak(0, "0173");
    FTBL.EV_ControlPackageResources(0, 6, "0173A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.npcSetAreaName(AREA_0173_ANP1, 0, 0.0f, 0);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0173_WB00) {
#else
    if (g_currentMap.name == AREA_0173_WB00.name) {
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 269.1f, 114.6f, 279.0f, 0);
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 93.5f, 0.0f, 0);
#else
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {269.1000061035156f, 114.5999984741211f, 279.0f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 17021, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
#endif
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
E0173_INLINE void Setup0173Characters(void) {
    Prepare0173Character(AREA_AUTHOR_RYO);
    Prepare0173Character(AREA_0173_SYP_);
}
E0173_INLINE void Restore0173Characters(void) {
    Restore0173Character(AREA_AUTHOR_RYO);
    Restore0173Character(AREA_0173_SYP_);
}
E0173_INLINE void Prepare0173Character(E0173CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0173_INLINE void Restore0173Character(E0173CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0173_CHARACTERS[] = {AREA_0173_SYP_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0173ScriptDefinition{.areaId = AREA_0173_AREA, .field04 = 0, .init = E0173Init, .main = E0173Main};
} // namespace shenmue::s2::areas
#endif
#undef E0173_INLINE
