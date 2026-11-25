#include "s2/areas/area_0265.hpp"
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
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/fog.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

struct E0265LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
};
// @verified - p
void E0265Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0265_INLINE
typedef shen_name E0265CharacterName;
#else
#if defined(_MSC_VER)
#define E0265_INLINE __forceinline
#else
#define E0265_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0265CharacterName;
#endif
void E0265MainCallback(void *);
void E0265LoopCallback(void *);
E0265_INLINE void QueueE0265Loop(AreaImmediateAuthorTaskParameter *);
E0265_INLINE void ConfigureE0265Environment(void);
E0265_INLINE void CleanupE0265(AreaImmediateAuthorTaskParameter *);
E0265_INLINE void SetupE0265Characters(void);
E0265_INLINE void RestoreE0265Characters(void);
E0265_INLINE void Prepare0265Character(E0265CharacterName);
E0265_INLINE void Restore0265Character(E0265CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0265Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(E0265MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    ConfigureE0265Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0265_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0265MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0265Loop(parent);
        break;
    case 3:
        CleanupE0265(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0265Loop(parent);
    else if (parent->state == 3)
        CleanupE0265(parent);
#endif
}
E0265_INLINE void QueueE0265Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0265LoopCallback, 4, 11, sizeof(E0265LoopParameter), AREA_AUTHOR_TASK);
    ((E0265LoopParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    SetupE0265Characters();
    FTBL.EV_authSceneCtrl(AREA_0265_AREA);
    parent->state = 2;
}
// @verified - pd
void E0265LoopCallback(void *data) {
    E0265LoopParameter *child = (E0265LoopParameter *)data;
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
    RestoreE0265Characters();
    child->parent->state = 3;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.HLib_DestroyCurrentTask();
}
// @verified - pd
E0265_INLINE void ConfigureE0265Environment(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_SetWeatherInstruction(12, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetMapModelDataDefaultVertexShader();
#else
    SetMapModelDataDefaultVertexShader();
#endif
    FTBL.EV_lightLoad(AREA_0265_L001);
    FTBL.PushGlobalLightingProgram();
    FTBL.EV_LoadCutsceneFromCurrentScene("0265A000");
    FTBL.EV_FreeAuthorPak(1, AREA_0265_CONVERSATION);
}
// @verified - pd
E0265_INLINE void CleanupE0265(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0265_CHARACTERS);
#else
    for (const shen_name *character = AREA_0265_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0265_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0265A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0265_AREA)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
#else
    if (g_currentMap.name == AREA_0265_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0265_AREA, g_entryPoint);
#endif
    else {
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 151.26f, 92.8f, 178.9f, 0);
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 0.0f, 0.0f, 0);
#else
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {151.26f, 92.8f, 178.9f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 0, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetMapModelDataDefaultVertexShader();
#else
        SetMapModelDataDefaultVertexShader();
#endif
        FTBL.EV_lightLoad(AREA_0265_0000);
        FTBL.PushGlobalLightingProgram();
        FTBL.SetAreaFogParams();
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}
// @verified - pd
E0265_INLINE void SetupE0265Characters(void) {
    Prepare0265Character(AREA_AUTHOR_RYO);
    Prepare0265Character(AREA_AUTHOR_REN);
    Prepare0265Character(AREA_0265_CHN_);
}
// @verified - pd
E0265_INLINE void RestoreE0265Characters(void) {
    Restore0265Character(AREA_AUTHOR_RYO);
    Restore0265Character(AREA_AUTHOR_REN);
    Restore0265Character(AREA_0265_CHN_);
}
E0265_INLINE void Prepare0265Character(E0265CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0265_INLINE void Restore0265Character(E0265CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0265_CHARACTERS[] = {AREA_AUTHOR_REN, AREA_0265_CHN_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0265ScriptDefinition{.areaId = AREA_0265_AREA, .field04 = 0, .init = E0265Init, .main = E0265Main};
} // namespace shenmue::s2::areas
#endif
#undef E0265_INLINE
