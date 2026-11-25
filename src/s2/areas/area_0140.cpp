#include "s2/areas/area_0140.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/inventory.hpp"
#include "s2/npc.hpp"
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
void E0140Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0140_INLINE
typedef shen_name E0140CharacterName;
#else
#if defined(_MSC_VER)
#define E0140_INLINE __forceinline
#else
#define E0140_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0140CharacterName;
#endif
void E0140MainCallback(void *);
void E0140LoopCallback(void *);
E0140_INLINE void QueueE0140Loop(AreaAuthorTaskParameter *);
E0140_INLINE void Configure0140(void);
E0140_INLINE void CleanupE0140(AreaAuthorTaskParameter *);
E0140_INLINE void SetupE0140Characters(void);
E0140_INLINE void RestoreE0140Characters(void);
E0140_INLINE void Prepare0140Character(E0140CharacterName);
E0140_INLINE void Restore0140Character(E0140CharacterName);
E0140_INLINE void Hide0140Prop(E0140CharacterName);
E0140_INLINE void Show0140Prop(E0140CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0140Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AreaAuthorTaskParameter *parent;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_SetPlayerFlag(12, 0x8b4, 1);
    FTBL.EventFunction870(0);
    FTBL.SetScenarioFollowCharacter(0);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
#else
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
#endif
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (g_currentMap == AREA_0140_AR02)
        FTBL.LoadNPCRoute("NPC_EXT0.BIN");
#endif
    task = FTBL.HLib_EnqueueTaskWithParameter(E0140MainCallback, 4, 11, sizeof(AreaAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    Configure0140();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0140_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0140MainCallback(void *data) {
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0140Loop(parent);
        break;
    case 3:
        CleanupE0140(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0140Loop(parent);
    else if (parent->state == 3)
        CleanupE0140(parent);
#endif
}
E0140_INLINE void QueueE0140Loop(AreaAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0140LoopCallback, 4, 11, sizeof(AreaAuthorFrameTaskParameter), AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *child = (AreaAuthorFrameTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_NPC_NewLoadSw_0(1);
#endif
    SetupE0140Characters();
    FTBL.EV_authSceneCtrl(AREA_0140_AREA);
    parent->state = 2;
}
void E0140LoopCallback(void *data) {
    AreaAuthorFrameTaskParameter *child = (AreaAuthorFrameTaskParameter *)data;
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
#if !defined(SHENMUE_EVENT_OVERLAY)
    if (GetAuthorFlags() != 0)
        spmFadeOut();
#endif
    {
        FTBL.AddInventoryItem(0x1f4f, 1, 1);
        FTBL.HLib_SetupTimedTask(child->parent->self, 130);
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0140Characters();
        child->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}
E0140_INLINE void Configure0140(void) {
    FTBL.SetPlayerControlEnabled(0);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.SetRenderClockAndCompassTo(1);
#endif
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_FreeAuthorPak(1, "0140");
    FTBL.EV_LoadCutsceneFromCurrentScene("0140A000");
}
E0140_INLINE void CleanupE0140(AreaAuthorTaskParameter *parent) {
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EV_SetPlayerFlag(12, 0x8b4, 0);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0140_CHARACTERS);
#else
    for (const shen_name *character = AREA_0140_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0140_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0140A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
    if (g_currentMap == AREA_0140_AR02) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.LoadNPCRoute(0);
#else
        FTBL.EV_NPC_NewLoadSw_0(1);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 0.0f, 0.0f, 0);
#else
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 0, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_spmBeginSampleExecute(0);
#else
        BeginSecondarySampleExecution(0);
#endif
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.SetRenderClockAndCompassTo(0);
#endif
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
E0140_INLINE void SetupE0140Characters(void) {

#if defined(SHENMUE_EVENT_OVERLAY)
    int i;
#endif
    Prepare0140Character(AREA_AUTHOR_RYO);
    Hide0140Prop(AREA_0140_A010);
    Hide0140Prop(AREA_0140_A396);
    Hide0140Prop(AREA_0140_A546);
    Hide0140Prop(AREA_0140_A548);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (i = 0; AREA_0140_CHARACTERS[i] != AREA_AUTHOR_INVALID; i++)
        Prepare0140Character(AREA_0140_CHARACTERS[i]);
#else
    for (const shen_name *character = AREA_0140_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        Prepare0140Character(*character);
#endif
}
E0140_INLINE void RestoreE0140Characters(void) {

#if defined(SHENMUE_EVENT_OVERLAY)
    int i;
#endif
    Restore0140Character(AREA_AUTHOR_RYO);
    Show0140Prop(AREA_0140_A010);
    Show0140Prop(AREA_0140_A396);
    Show0140Prop(AREA_0140_A546);
    Show0140Prop(AREA_0140_A548);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (i = 0; AREA_0140_CHARACTERS[i] != AREA_AUTHOR_INVALID; i++)
        Restore0140Character(AREA_0140_CHARACTERS[i]);
#else
    for (const shen_name *character = AREA_0140_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        Restore0140Character(*character);
#endif
}
E0140_INLINE void Prepare0140Character(E0140CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0140_INLINE void Restore0140Character(E0140CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0140_INLINE void Hide0140Prop(E0140CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0140_INLINE void Show0140Prop(E0140CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0140_CHARACTERS[] = {AREA_0140_JOP_, AREA_0140_SHK_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0140ScriptDefinition{.areaId = AREA_0140_AREA, .field04 = 0, .init = E0140Init, .main = E0140Main};
} // namespace shenmue::s2::areas
#endif
#undef E0140_INLINE
