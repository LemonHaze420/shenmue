#include "s2/areas/area_0257.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/character.hpp"
#include "s2/pak.hpp"
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
#include "s2/npc.hpp"
#include "s2/door.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/ui.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

// @verified - p
std::uint8_t e0257RewardIndex;
void E0257Init() {
    e0257RewardIndex = 0;
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0257_INLINE
#define E0257_LOCAL
typedef shen_name E0257CharacterName;
#else
#if defined(_MSC_VER)
#define E0257_INLINE __forceinline
#else
#define E0257_INLINE inline __attribute__((always_inline))
#endif
#define E0257_LOCAL static
typedef const shen_name &E0257CharacterName;
#endif
void E0257MainCallback(void *);
void E0257LoopCallback(void *);
E0257_LOCAL E0257_INLINE void QueueE0257Author(AreaImmediateAuthorTaskParameter *);
E0257_LOCAL E0257_INLINE void ConfigureE0257Environment(void);
E0257_LOCAL E0257_INLINE void CleanupE0257(AreaImmediateAuthorTaskParameter *);
E0257_LOCAL E0257_INLINE void SetupE0257Characters(void);
E0257_LOCAL E0257_INLINE void RestoreE0257Characters(void);
E0257_LOCAL E0257_INLINE void PrepareE0257Character(E0257CharacterName);
E0257_LOCAL E0257_INLINE void RestoreE0257Character(E0257CharacterName);
E0257_LOCAL E0257_INLINE void HideE0257Prop(E0257CharacterName);
E0257_LOCAL E0257_INLINE void ShowE0257Prop(E0257CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0257Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(E0257MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    ConfigureE0257Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0257_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
#if defined(SHENMUE_EVENT_OVERLAY)
extern signed char e0257RewardIndex;
#endif
// @verified - pd
void E0257MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 4:
        if (!FTBL.GetShouldRenderIcons(5)) {
#else
    if (parent->state == 1)
        QueueE0257Author(parent);
    else if (parent->state == 3)
        CleanupE0257(parent);
    else if (parent->state == 4 && !GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage)) {
#endif
            unsigned int item = AREA_0257_ITEMS[e0257RewardIndex];
#if defined(SHENMUE_EVENT_OVERLAY)
            if (item != -1U) {
                FTBL.AddInventoryItem(item, 1, 1);
                ++e0257RewardIndex;
            } else {
                e0257RewardIndex = 0;
                parent->state = 3;
            }
#else
        if (item == 0xffffffffU) {
            e0257RewardIndex = 0;
            parent->state = 3;
        } else {
            AddInventoryItem(item, 1, 1);
            ++e0257RewardIndex;
        }
#endif
        }
#if defined(SHENMUE_EVENT_OVERLAY)
        break;
    case 1:
        QueueE0257Author(parent);
        break;
    case 3:
        CleanupE0257(parent);
        break;
    }
#endif
}
E0257_LOCAL E0257_INLINE void QueueE0257Author(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0257LoopCallback, 4, 11, sizeof(AreaImmediateAuthorFrameParameter), AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)FTBL.HLib_GetTaskParameter(task);
#if defined(SHENMUE_EVENT_OVERLAY)
    child->parent = parent;
    child->state = 0;
#else
    child->state = 0;
    child->parent = parent;
#endif
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0257Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(AREA_0257_AREA);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0257_AREA.name));
#endif
    parent->state = 2;
}
// @verified - pd
void E0257LoopCallback(void *data) {
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)data;
    if ((short)FTBL.EV_authSceneCtrl(-6) == 557) {
#else
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 557) {
#endif
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    if (!FTBL.EV_authSceneCtrl(-1)) {
#else
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
#endif
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0257Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
        child->parent->state = 4;
#else
    static_cast<AreaImmediateAuthorFrameParameter *>(data)->parent->state = 4;
#endif
        FTBL.HLib_DestroyCurrentTask();
#if defined(SHENMUE_EVENT_OVERLAY)
    }
#endif
}
E0257_LOCAL E0257_INLINE void ConfigureE0257Environment(void) {
    const Vector3 center = {116.51f, 92.7f, 170.56f};
    FTBL.npcSetAreaName(AREA_0257_ANP1, &center, 5.0f, 1);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0257A000");
    FTBL.EV_FreeAuthorPak(1, AREA_0257_CONVERSATION);
}
E0257_LOCAL E0257_INLINE void CleanupE0257(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0257_CHARACTERS);
#else
    for (const shen_name *character = AREA_0257_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
#endif
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_FreeAuthorPak(0, AREA_0257_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0257A", 0, 0);
    FTBL.npcSetAreaName(AREA_0257_ANP1, 0, 0.0f, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0257_AREA)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
#else
    if (g_currentMap.name == AREA_0257_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0257_AREA, g_entryPoint);
#endif
    else {
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_spmBeginSampleExecute(0);
    }
}
E0257_LOCAL E0257_INLINE void SetupE0257Characters(void) {
    PrepareE0257Character(AREA_AUTHOR_RYO);
    PrepareE0257Character(AREA_0257_CHN_);
    HideE0257Prop(AREA_0257_A428);
    HideE0257Prop(AREA_0257_A438);
    HideE0257Prop(AREA_0257_A534);
}
E0257_LOCAL E0257_INLINE void RestoreE0257Characters(void) {
    RestoreE0257Character(AREA_AUTHOR_RYO);
    RestoreE0257Character(AREA_0257_CHN_);
    ShowE0257Prop(AREA_0257_A428);
    ShowE0257Prop(AREA_0257_A438);
    ShowE0257Prop(AREA_0257_A534);
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 100.1f, 90.8f, 173.1f, 0);
    EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 90.0f, 0.0f, 0);
#else
    EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {100.1f, 90.8f, 173.1f}, 0.0f};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 0x4000, 0}, 0.0f};
    EVT_AT_SetAng_Callback(&rotation);
#endif
}
E0257_LOCAL E0257_INLINE void PrepareE0257Character(E0257CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0257_LOCAL E0257_INLINE void RestoreE0257Character(E0257CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0257_LOCAL E0257_INLINE void HideE0257Prop(E0257CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0257_LOCAL E0257_INLINE void ShowE0257Prop(E0257CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0257_CHARACTERS[] = {AREA_0257_CHN_, AREA_AUTHOR_INVALID};
unsigned int AREA_0257_ITEMS[] = {8001, 8002, 8003, -1U};
signed char e0257RewardIndex = 0;
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0257ScriptDefinition{.areaId = AREA_0257_AREA, .field04 = 0, .init = E0257Init, .main = E0257Main};
} // namespace shenmue::s2::areas
#endif
#undef E0257_INLINE
#undef E0257_LOCAL
