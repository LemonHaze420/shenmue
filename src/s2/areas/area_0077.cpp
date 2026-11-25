#include "s2/areas/area_0077.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
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
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/sound_program.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {
ConversationBuffer *e0077Voice;

// @verified - p
void E0077Init() {
    e0077Voice = nullptr;
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0077_INLINE
#define E0077_LOCAL
typedef shen_name E0077CharacterName;
#else
#if defined(_MSC_VER)
#define E0077_INLINE __forceinline
#else
#define E0077_INLINE inline __attribute__((always_inline))
#endif
#define E0077_LOCAL static
typedef const shen_name &E0077CharacterName;
#endif
void E0077MainCallback(void *);
void E0077LoopCallback(void *);
E0077_LOCAL E0077_INLINE void QueueE0077Loop(AreaAuthorTaskParameter *);
E0077_LOCAL E0077_INLINE void ConfigureE0077Environment(void);
E0077_LOCAL E0077_INLINE void CleanupE0077(AreaAuthorTaskParameter *);
E0077_LOCAL E0077_INLINE void SetupE0077Characters(void);
E0077_LOCAL E0077_INLINE void RestoreE0077Characters(void);
E0077_LOCAL E0077_INLINE void PrepareE0077Character(E0077CharacterName);
E0077_LOCAL E0077_INLINE void RestoreE0077Character(E0077CharacterName);
E0077_LOCAL E0077_INLINE void HideE0077Prop(E0077CharacterName);
E0077_LOCAL E0077_INLINE void ShowE0077Prop(E0077CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0077Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
#endif
    HLibTask *task;
    AreaAuthorTaskParameter *parent;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(E0077MainCallback, 4, 11, sizeof(AreaAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 1;
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0077_WN00) {
        FTBL.GetCurrentDateTime((DateTime *)&time);
        if (time.hour >= 19)
            parent->selection = 0;
        else
            parent->selection = 1;
#else
    if (g_currentMap.name == AREA_0077_WN00.name) {
        DateTime time;
        shenGetCurrentDateTime(time);
        parent->selection = time.hour < 19;
#endif
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        parent->selection = g_entryPoint != 0;
    }
    FTBL.EV_stepSetFlags(0x6f, 0);
    ConfigureE0077Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0077_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
#if defined(SHENMUE_EVENT_OVERLAY)
static void *e0077Voice;
#endif
// @verified - pd
void E0077MainCallback(void *data) {
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0077Loop(parent);
        break;
    case 3:
        CleanupE0077(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0077Loop(parent);
    else if (parent->state == 3)
        CleanupE0077(parent);
#endif
}
E0077_LOCAL E0077_INLINE void QueueE0077Loop(AreaAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    shen_name authors[2] = {AREA_0077_AUTHOR0, AREA_0077_AUTHOR1};
#endif
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0077LoopCallback, 4, 11, sizeof(AreaAuthorLoopTaskParameter), AREA_AUTHOR_TASK);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    e0077Voice = FTBL.shenPlayAudioWithFlags("0077C001", 1);
#else
    e0077Voice = shenPlayAudioWithFlags("0077C001", 1);
#endif
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_spmBeginSampleExecute(0);
    SetupE0077Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(authors[parent->selection]);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0077_AUTHORS[parent->selection].name));
#endif
    parent->state = 2;
}
// @verified - pd
void E0077LoopCallback(void *data) {
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)data;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.hsFadeVoice(e0077Voice, 30);
        FTBL.HLib_SetupTimedTask(child->parent->self, 30);
#else
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    if (GetAuthorFlags())
        ExecuteAicaCommand(0x1100a0, 4, 0);
    hsFadeVoice(e0077Voice, 30);
    HLib::SetupTimedTask(static_cast<AreaAuthorLoopTaskParameter *>(data)->parent->self, 30);
#endif
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0077Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
        child->parent->state = 3;
#else
    static_cast<AreaAuthorLoopTaskParameter *>(data)->parent->state = 3;
#endif
        FTBL.HLib_DestroyCurrentTask();
#if defined(SHENMUE_EVENT_OVERLAY)
    }
#endif
}
E0077_LOCAL E0077_INLINE void ConfigureE0077Environment(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0077A000");
    FTBL.EV_FreeAuthorPak(1, "0077");
}
E0077_LOCAL E0077_INLINE void CleanupE0077(AreaAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
#endif
    FTBL.EventFunction857(1);
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0077_CHARACTERS);
#else
    for (const shen_name *character = AREA_0077_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0077_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0077A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0077_WN00) {
        FTBL.GetCurrentDateTime((DateTime *)&time);
        FTBL.SetMapModelDataDefaultVertexShader();
        if (time.hour >= 19) {
            FTBL.EV_lightLoad(AREA_0077_LIGHT);
            FTBL.PushGlobalLightingProgram();
        } else {
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
        }
#else
    if (g_currentMap.name == AREA_0077_WN00.name) {
        DateTime time;
        shenGetCurrentDateTime(time);
        EV_SetModelDataDefaultVertexShader();
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        } else {
            EV_lightLoad(AREA_0077_LIGHT);
            PushGlobalLightingProgram();
        }
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 246.4f, 104.5f, 195.2f, 0);
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 85.5f, 0.0f, 0);
#else
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3{246.4f, 104.5f, 195.2f}, 0);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3i{0, 15564, 0}, 0};
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
E0077_LOCAL E0077_INLINE void SetupE0077Characters(void) {
    PrepareE0077Character(AREA_AUTHOR_RYO);
    PrepareE0077Character(AREA_0077_JOP_);
    HideE0077Prop(AREA_0077_A396);
    HideE0077Prop(AREA_0077_A456);
}
E0077_LOCAL E0077_INLINE void RestoreE0077Characters(void) {
    RestoreE0077Character(AREA_AUTHOR_RYO);
    RestoreE0077Character(AREA_0077_JOP_);
    ShowE0077Prop(AREA_0077_A396);
    ShowE0077Prop(AREA_0077_A456);
}
E0077_LOCAL E0077_INLINE void PrepareE0077Character(E0077CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0077_LOCAL E0077_INLINE void RestoreE0077Character(E0077CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0077_LOCAL E0077_INLINE void HideE0077Prop(E0077CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0077_LOCAL E0077_INLINE void ShowE0077Prop(E0077CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0077_CHARACTERS[] = {AREA_0077_JOP_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0077ScriptDefinition{.areaId = AREA_0077_AREA, .field04 = 0, .init = E0077Init, .main = E0077Main};
} // namespace shenmue::s2::areas
#endif
#undef E0077_INLINE
#undef E0077_LOCAL
