#include "s2/areas/area_0222.hpp"
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

// @verified - p
void E0222Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0222_INLINE
#define E0222_LOCAL
typedef shen_name E0222CharacterName;
#else
#if defined(_MSC_VER)
#define E0222_INLINE __forceinline
#else
#define E0222_INLINE inline __attribute__((always_inline))
#endif
#define E0222_LOCAL static
typedef const shen_name &E0222CharacterName;
#endif
void E0222MainCallback(void *);
void E0222LoopCallback(void *);
E0222_LOCAL E0222_INLINE void QueueE0222Author(AreaImmediateAuthorTaskParameter *);
E0222_LOCAL E0222_INLINE void CleanupE0222(AreaImmediateAuthorTaskParameter *);
E0222_LOCAL E0222_INLINE void SetupE0222Characters(short);
E0222_LOCAL E0222_INLINE void RestoreE0222Characters(short);
E0222_LOCAL E0222_INLINE void ConfigureE0222Environment(
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaImmediateAuthorTaskParameter *
#else
    void
#endif
);
E0222_LOCAL E0222_INLINE void PrepareE0222Character(E0222CharacterName);
E0222_LOCAL E0222_INLINE void RestoreE0222Character(E0222CharacterName);
E0222_LOCAL E0222_INLINE void HideE0222Prop(E0222CharacterName);
E0222_LOCAL E0222_INLINE void ShowE0222Prop(E0222CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0222Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    short *argument
#else
    std::uintptr_t argument
#endif
) {
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(E0222MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    if (g_currentMap == AREA_0222_AREA)
        parent->selection = (unsigned int)g_entryPoint > 2 ? 0 : g_entryPoint;
    else
        parent->selection = *argument;
    ConfigureE0222Environment(parent);
#else
    parent->selection = g_currentMap.name == AREA_0222_AREA.name ? static_cast<std::int16_t>(g_entryPoint > 2 ? 0 : g_entryPoint) :
                                                                   *reinterpret_cast<const std::int16_t *>(argument);
    parent->state = 1;
    ConfigureE0222Environment();
#endif
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0222_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0222MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0222Author(parent);
        break;
    case 3:
        CleanupE0222(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0222Author(parent);
    else if (parent->state == 3)
        CleanupE0222(parent);
#endif
}
E0222_LOCAL E0222_INLINE void QueueE0222Author(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    shen_name authors[] = {AREA_0222_YAN1, AREA_0222_YAN2, AREA_0222_YAN3};
#endif
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0222LoopCallback, 4, 11, sizeof(AreaImmediateAuthorFrameParameter), AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)FTBL.HLib_GetTaskParameter(task);
#if defined(SHENMUE_EVENT_OVERLAY)
    child->parent = parent;
    child->state = 0;
#else
    child->state = 0;
    child->parent = parent;
#endif
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0222Characters(parent->selection);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(authors[parent->selection]);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0222_AUTHORS[parent->selection].name));
#endif
    parent->state = 2;
}
// @verified - pd
void E0222LoopCallback(void *data) {
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)data;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        switch (child->parent->selection) {
        case 0:
        case 2:
#else
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    AreaImmediateAuthorTaskParameter *parent = static_cast<AreaImmediateAuthorFrameParameter *>(data)->parent;
    if (parent->selection == 1) {
        parent->state = 1;
        ++parent->selection;
        return;
    }
    if (parent->selection != 0 && parent->selection != 2)
        return;
#endif
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
            RestoreE0222Characters(child->parent->selection);
            child->parent->state = 3;
#else
    RestoreE0222Characters(static_cast<AreaImmediateAuthorFrameParameter *>(data)->parent->selection);
    static_cast<AreaImmediateAuthorFrameParameter *>(data)->parent->state = 3;
#endif
            FTBL.HLib_DestroyCurrentTask();
#if defined(SHENMUE_EVENT_OVERLAY)
            break;
        case 1:
            child->parent->state = 1;
            ++child->parent->selection;
            break;
        }
    }
#endif
}
E0222_LOCAL E0222_INLINE void ConfigureE0222Environment(
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaImmediateAuthorTaskParameter *parent
#else
    void
#endif
) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0222A000");
    FTBL.EV_FreeAuthorPak(1, AREA_0222_CONVERSATION);
}
E0222_LOCAL E0222_INLINE void CleanupE0222(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0222_CHARACTERS);
#else
    for (const shen_name *character = AREA_0222_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0222_CONVERSATION);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0222_AREA)
        FTBL.EV_ControlPackageResources(0, 6, "0222A", 0, 0);
    else if (parent->selection == 0)
        FTBL.EV_ControlPackageResources(0, 6, "0222A1", 0, 0);
    else
        FTBL.EV_ControlPackageResources(0, 6, "0222A2", 0, 0);
    if (g_currentMap == AREA_0222_AREA)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    else if (parent->selection == 0) {
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0222_QAAA, 1);
    } else {
        FTBL.HLib_DestroyTask(parent->self);
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0222_QR00, 50);
    }
#else
    const char *package = g_currentMap.name == AREA_0222_AREA.name ? "0222A" : parent->selection == 0 ? "0222A1" : "0222A2";
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_CleanupMapControlTask();
    if (g_currentMap.name == AREA_0222_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0222_AREA, g_entryPoint);
    else {
        const bool secondReturn = parent->selection != 0;
        HLib::DestroyTask(parent->self);
        if (secondReturn)
            EV_scenQueueLoad(g_actNumber, AREA_0222_QR00, 50);
        else
            EV_scenQueueLoad(g_actNumber, AREA_0222_QAAA, 1);
    }
#endif
}
E0222_LOCAL E0222_INLINE void SetupE0222Characters(short selection) {
    PrepareE0222Character(AREA_AUTHOR_RYO);
    PrepareE0222Character(AREA_AUTHOR_REN);
    PrepareE0222Character(AREA_0222_YAN_);
    HideE0222Prop(AREA_0222_AA22);
    if (selection != 0) {
        HideE0222Prop(AREA_0222_A052);
        HideE0222Prop(AREA_0222_A053);
        HideE0222Prop(AREA_0222_A212);
        HideE0222Prop(AREA_0222_A095);
        HideE0222Prop(AREA_0222_A316);
    }
}
E0222_LOCAL E0222_INLINE void RestoreE0222Characters(short selection) {
    RestoreE0222Character(AREA_AUTHOR_RYO);
    RestoreE0222Character(AREA_AUTHOR_REN);
    RestoreE0222Character(AREA_0222_YAN_);
    ShowE0222Prop(AREA_0222_AA22);
    if (selection != 0) {
        ShowE0222Prop(AREA_0222_A052);
        ShowE0222Prop(AREA_0222_A053);
        ShowE0222Prop(AREA_0222_A212);
        ShowE0222Prop(AREA_0222_A095);
        ShowE0222Prop(AREA_0222_A316);
    }
}
E0222_LOCAL E0222_INLINE void PrepareE0222Character(E0222CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0222_LOCAL E0222_INLINE void RestoreE0222Character(E0222CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0222_LOCAL E0222_INLINE void HideE0222Prop(E0222CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0222_LOCAL E0222_INLINE void ShowE0222Prop(E0222CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0222_CHARACTERS[] = {AREA_0222_YAN_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0222ScriptDefinition{.areaId = AREA_0222_AREA, .field04 = 0, .init = E0222Init, .main = E0222Main};
} // namespace shenmue::s2::areas
#endif
#undef E0222_INLINE
#undef E0222_LOCAL
