#include "s2/areas/area_0139.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/character.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include "s2/inventory.hpp"

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
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

// @verified - p
void E0139Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0139_INLINE
typedef shen_name E0139CharacterName;
#else
#if defined(_MSC_VER)
#define E0139_INLINE __forceinline
#else
#define E0139_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0139CharacterName;
#endif
void E0139MainCallback(void *);
void E0139LoopCallback(void *);
E0139_INLINE void QueueE0139Loop(AreaImmediateAuthorTaskParameter *);
E0139_INLINE void ConfigureE0139Environment(void);
E0139_INLINE void CleanupE0139(AreaImmediateAuthorTaskParameter *);
E0139_INLINE void Setup0139Characters(void);
E0139_INLINE void Restore0139Characters(void);
E0139_INLINE void Prepare0139Character(E0139CharacterName);
E0139_INLINE void Restore0139Character(E0139CharacterName);
E0139_INLINE void Hide0139Prop(E0139CharacterName);
E0139_INLINE void Show0139Prop(E0139CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0139Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(E0139MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    ConfigureE0139Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0139_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0139MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0139Loop(parent);
        break;
    case 3:
        CleanupE0139(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0139Loop(parent);
    else if (parent->state == 3)
        CleanupE0139(parent);
#endif
}
E0139_INLINE void QueueE0139Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task;
    AreaImmediateAuthorFrameParameter *child;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_LoadCutsceneFromCurrentScene("0139A000");
#endif
    task = FTBL.HLib_EnqueueTaskWithParameter(E0139LoopCallback, 4, 11, sizeof(AreaImmediateAuthorFrameParameter), AREA_AUTHOR_TASK);
    child = (AreaImmediateAuthorFrameParameter *)FTBL.HLib_GetTaskParameter(task);
#if defined(SHENMUE_EVENT_OVERLAY)
    child->parent = parent;
    child->state = 0;
#else
    child->state = 0;
    child->parent = parent;
#endif
    Setup0139Characters();
    FTBL.EV_authSceneCtrl(AREA_0139_AREA);
    parent->state = 2;
}
void E0139LoopCallback(void *data) {
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)data;
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
    {
        FTBL.AddInventoryItem(0x1f52, 1, 1);
        FTBL.HLib_SetupTimedTask(child->parent->self, 130);
        Restore0139Characters();
        child->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentEventTask();
    }
}
E0139_INLINE void ConfigureE0139Environment(void) {
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_LoadCutsceneFromCurrentScene("0139A000");
#endif
    FTBL.EV_FreeAuthorPak(1, AREA_0139_CONVERSATION);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0139_AREA) {
#else
    if (g_currentMap.name == AREA_0139_AREA.name) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetMapModelDataDefaultVertexShader();
#else
        SetMapModelDataDefaultVertexShader();
#endif
        FTBL.EV_lightLoad(AREA_0139_0000);
        FTBL.PushGlobalLightingProgram();
    }
}
E0139_INLINE void CleanupE0139(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EventDateTime time;
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0139_CHARACTERS);
#else
    for (const shen_name *character = AREA_0139_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_ControlPackageResources(0, 6, "0139A", 0, 0);
    FTBL.EV_FreeAuthorPak(0, AREA_0139_CONVERSATION);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0139_AR03) {
#else
    if (g_currentMap.name == AREA_0139_AR03.name) {
#endif
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_SetWeatherInstruction(12, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 289.4f, 100.0f, 484.9f, 0);
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 182.5f, 0.0f, 0);
#else
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {289.4f, 100.0f, 484.9f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 33223, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.LeaveDoorSubscene(0);
        FTBL.EV_LoadCutsceneFromCurrentScene("AR03");
        FTBL.GetCurrentDateTime((DateTime *)&time);
        if (FTBL.EV_GetShenmueFlag(132))
            FTBL.EV_LoadAudio("0061A000", 1);
        else if (time.hour >= 19)
            FTBL.EV_LoadAudio("AR03_000", 1);
        else
            FTBL.EV_LoadAudio("AR03_000", 0);
#else
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EnterDoorSubscene(0);
#else
        doorLoadAuthorWithAudio(shen_name{});
#endif
#endif
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_spmBeginSampleExecute(0);
#else
        BeginSecondarySampleExecution(0);
#endif
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
E0139_INLINE void Setup0139Characters(void) {
    Prepare0139Character(AREA_AUTHOR_RYO);
    Prepare0139Character(AREA_0139_SYN_);
    Hide0139Prop(AREA_0139_A450);
}
E0139_INLINE void Restore0139Characters(void) {
    Restore0139Character(AREA_AUTHOR_RYO);
    Restore0139Character(AREA_0139_SYN_);
    Show0139Prop(AREA_0139_A450);
}
E0139_INLINE void Prepare0139Character(E0139CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0139_INLINE void Restore0139Character(E0139CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0139_INLINE void Hide0139Prop(E0139CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0139_INLINE void Show0139Prop(E0139CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0139_CHARACTERS[] = {AREA_0139_SYN_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0139ScriptDefinition{.areaId = AREA_0139_AREA, .field04 = 0, .init = E0139Init, .main = E0139Main};
} // namespace shenmue::s2::areas
#endif
#undef E0139_INLINE
