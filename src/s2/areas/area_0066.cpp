#include "s2/areas/area_0066.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/inventory.hpp"
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

struct E0066LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
};
// @verified - p
void E0066Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0066_INLINE
typedef shen_name E0066CharacterName;
#else
#if defined(_MSC_VER)
#define E0066_INLINE __forceinline
#else
#define E0066_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0066CharacterName;
#endif
void E0066MainCallback(void *);
void E0066LoopCallback(void *);
E0066_INLINE void QueueE0066Loop(AreaImmediateAuthorTaskParameter *);
E0066_INLINE void ConfigureE0066Environment(void);
E0066_INLINE void CleanupE0066(AreaImmediateAuthorTaskParameter *);
E0066_INLINE void SetupE0066Characters(void);
E0066_INLINE void RestoreE0066Characters(void);
E0066_INLINE void Prepare0066Character(E0066CharacterName);
E0066_INLINE void Restore0066Character(E0066CharacterName);
E0066_INLINE void Hide0066Prop(E0066CharacterName);
E0066_INLINE void Show0066Prop(E0066CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0066Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0066MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    ConfigureE0066Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0066_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0066MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0066Loop(parent);
        break;
    case 3:
        CleanupE0066(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0066Loop(parent);
    else if (parent->state == 3)
        CleanupE0066(parent);
#endif
}
E0066_INLINE void QueueE0066Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0066LoopCallback, 4, 11, sizeof(E0066LoopParameter), AREA_AUTHOR_TASK);
    ((E0066LoopParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_spmBeginSampleExecute(0);
    SetupE0066Characters();
    FTBL.EV_authSceneCtrl(AREA_0066_AREA);
    parent->state = 2;
}
void E0066LoopCallback(void *data) {
    E0066LoopParameter *child = (E0066LoopParameter *)data;
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    child->parent->state = 3;
    RestoreE0066Characters();
    FTBL.HLib_DestroyCurrentTask();
}
E0066_INLINE void ConfigureE0066Environment(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetWeatherInstruction(12, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetMapModelDataDefaultVertexShader();
#else
    SetMapModelDataDefaultVertexShader();
#endif
    FTBL.EV_lightLoad(AREA_0066_L001);
    FTBL.PushGlobalLightingProgram();

#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0066_WTA0)
#else
    if (g_currentMap.name == AREA_0066_WTA0.name)
#endif
        FTBL.EV_DisableCharacterDisplay(AREA_0066_0001);
    FTBL.EV_FreeAuthorPak(1, AREA_0066_CONVERSATION);
    FTBL.EV_LoadCutsceneFromCurrentScene("0066A000");
}
E0066_INLINE void CleanupE0066(AreaImmediateAuthorTaskParameter *parent) {
    FTBL.AddInventoryItem(0x1f4c, 1, 0);
    FTBL.AddInventoryItem(0xd, 1, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0066_CHARACTERS);
#else
    for (const shen_name *character = AREA_0066_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_ControlPackageResources(0, 6, "0066A", 0, 0);
    FTBL.EV_FreeAuthorPak(0, AREA_0066_CONVERSATION);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);

#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0066_WTA0)
#else
    if (g_currentMap.name == AREA_0066_WTA0.name)
#endif
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0066_WT00, 80);
    else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
E0066_INLINE void SetupE0066Characters(void) {
    Prepare0066Character(AREA_AUTHOR_RYO);
    Prepare0066Character(AREA_0066_KUN_);
    Hide0066Prop(AREA_0066_A047);
    Hide0066Prop(AREA_0066_A890);
    Hide0066Prop(AREA_0066_A540);
    Hide0066Prop(AREA_0066_A044);
    Hide0066Prop(AREA_0066_A045);
    Hide0066Prop(AREA_0066_A046);
}
E0066_INLINE void RestoreE0066Characters(void) {
    Restore0066Character(AREA_AUTHOR_RYO);
    Restore0066Character(AREA_0066_KUN_);
    Show0066Prop(AREA_0066_A047);
    Show0066Prop(AREA_0066_A890);
    Show0066Prop(AREA_0066_A540);
    Show0066Prop(AREA_0066_A044);
    Show0066Prop(AREA_0066_A045);
    Show0066Prop(AREA_0066_A046);
}
E0066_INLINE void Prepare0066Character(E0066CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0066_INLINE void Restore0066Character(E0066CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0066_INLINE void Hide0066Prop(E0066CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0066_INLINE void Show0066Prop(E0066CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0066_CHARACTERS[] = {AREA_0066_KUN_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0066ScriptDefinition{.areaId = AREA_0066_AREA, .field04 = 0, .init = E0066Init, .main = E0066Main};
} // namespace shenmue::s2::areas
#endif
#undef E0066_INLINE
