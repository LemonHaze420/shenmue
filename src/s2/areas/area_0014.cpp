#include "s2/areas/area_0014.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/character.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/initialization.hpp"
#include "s2/inventory.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <array>
namespace shenmue::s2::areas {
namespace {
struct Area0014LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
};
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
#define E0014_INLINE
#else
#if defined(_MSC_VER)
#define E0014_INLINE __forceinline
#else
#define E0014_INLINE inline __attribute__((always_inline))
#endif
#endif
void MainCallback(void *data);
void LoopCallback(void *data);
void ConfigureEnvironment(void);
void SetupCharacters(void);
void RestoreCharacters(void);
E0014_INLINE void QueueLoop(AreaImmediateAuthorTaskParameter *parent);
E0014_INLINE void Cleanup(AreaImmediateAuthorTaskParameter *parent);

#if !defined(SHENMUE_EVENT_OVERLAY)
constexpr shen_name characters[]{SHEN_NAME(WON_), AREA_AUTHOR_INVALID};
constexpr std::array<shen_name, 4> alphaCharacters{SHEN_NAME(a016), SHEN_NAME(a009), SHEN_NAME(ab11) | 0x4000000U, SHEN_NAME(aa36) | 0x1000000U};
std::int32_t savedRootRenderMode{};
// @verified - p
void InitNullsub() {
}
void SetCharacters(bool restore) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, restore ? 0 : 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, restore);
    for (shen_name name : alphaCharacters)
        EV_SetCharacterAlphaEnabled(name, restore);
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name) {
        mtwkSetExtraFlag(scenFindMotionwork(*name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(*name, restore);
    }
}
#endif

#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.EventFunction870(0);
    FTBL.SetScenarioFollowCharacter(0);
#endif
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(MainCallback, 4, 0x0b, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    parent->state = 1;
#else
    parent->state = 3;
#endif
    parent->selection = 0;
    FTBL.EV_stepSetFlags(64, 0);
    FTBL.SetPlayerGameFlags(4, 0);
    ConfigureEnvironment();
    Queue_EVT_UTL_LoadNPCChara_3(characters, parent->self);
}
#endif

#if !defined(SHENMUE_EVENT_ENTRY)
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name characters[] = {SHEN_NAME4('W', 'O', 'N', '_'), INVALID_CHARACTER};
#endif

// @verified - pd
void MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
    switch (parent->state) {
#if defined(SHENMUE_EVENT_OVERLAY) && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    case 6:
        if (!FTBL.GetShouldRenderIcons(5))
            parent->state = 0;
        break;
#endif
    case 0:
        FTBL.LoadNPCRoute("NPC.BIN");
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.HLib_SetupTimedTask(parent->self, 30);
        parent->state = 3;
#else
        parent->state = 1;
#endif
        break;
    case 1:
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        QueueLoop(parent);
#else
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_LoadCutsceneFromCurrentScene("AR02");
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.SetRenderClockAndCompassTo(0);
        parent->state = 2;
#endif
        break;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    case 2:
        if (!FTBL.filmCheckFadeTrackColour())
            parent->state = 5;
        break;
#endif
    case 3:
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        Cleanup(parent);
#else
        QueueLoop(parent);
#endif
        break;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    case 5:
        Cleanup(parent);
        break;
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
    case 6:
        if (!GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
            parent->state = 0;
        break;
    default:
        break;
#endif
    }
}

E0014_INLINE void QueueLoop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, sizeof(Area0014LoopParameter), AREA_AUTHOR_TASK);
    ((Area0014LoopParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    SetupCharacters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(SHEN_NAME4('0', '0', '1', '4'));
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(0014)));
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    parent->state = 2;
#else
    parent->state = 4;
#endif
}

// @verified - pd
void LoopCallback(void *data) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Area0014LoopParameter *child = (Area0014LoopParameter *)data;
    short frame = FTBL.EV_authSceneCtrl(-6);
#else
    const std::uint32_t frame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1)
        savedRootRenderMode = GetRootRenderDeferredMode();
    else
#endif
    if (frame == 521) {
#if !defined(SHENMUE_EVENT_OVERLAY)
        SetRootRenderDeferredMode(0);
#endif
        FTBL.SetPlayerGameFlags(4, 1);
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1))
#else
    if (FTBL.EV_authSceneCtrl(-1))
#endif
        return;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetPlayerGameFlags(4, 1);
#endif
    RestoreCharacters();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (g_currentMap == SHEN_NAME4('A', 'R', '0', '2')) {
        child->parent->state = 0;
        FTBL.AddInventoryItem(0x178a, 1, 1);
        FTBL.HLib_SetupTimedTask(child->parent->self, 130);
    } else
        child->parent->state = 3;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
#else
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(RYO_CHARACTER), 0xf003, 0);
    EVT_AT_SetPos(RYO_CHARACTER, INVALID_CHARACTER, 255.88f, 100.2f, 350.5f, 0);
    EVT_AT_SetAng(RYO_CHARACTER, INVALID_CHARACTER, 0.0f, 24.85f, 0.0f, 0);
#else
    mtwkRequestMotion(scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, MotionRegion::LowerBody);
    EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {255.8800048828125f, 100.19999694824219f, 350.5f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 4523, 0}, 0};
    EVT_AT_SetAng_Callback(&angle);
#endif
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.EV_ControlPackageResources(0, 6, "0014A", 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == SHEN_NAME4('A', 'R', '0', '2')) {
#else
    AreaImmediateAuthorTaskParameter *parent = *(AreaImmediateAuthorTaskParameter **)data;
    if (g_currentMap.name == SHEN_NAME(AR02)) {
#endif
        FTBL.AddInventoryItem(0x178a, 1, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
        child->parent->state = 6;
    } else
        child->parent->state = 5;
#else
        parent->state = 6;
    } else
        parent->state = 5;
    SetRootRenderDeferredMode(savedRootRenderMode);
#endif
#endif
    FTBL.HLib_DestroyCurrentTask();
}

// @verified - pd
void ConfigureEnvironment(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.SetRenderClockAndCompassTo(1);
#endif
    if (g_currentMap == SHEN_NAME4('0', '0', '1', '4')) {
        FTBL.GetCurrentDateTime((DateTime *)&time);
        if (time.hour >= 19) {
            FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
            FTBL.PushGlobalLightingProgram();
            FTBL.SetObjectFlagByIndex(14, 1);
            FTBL.SetObjectFlagByIndex(15, 0);
            FTBL.EV_nlSetVertexColourType(1);
        } else {
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
            FTBL.SetObjectFlagByIndex(14, 0);
            FTBL.SetObjectFlagByIndex(15, 1);
            FTBL.EV_nlSetVertexColourType(2);
        }
    }
#else
    shenSetClockAndCompassRenderTo(1);
    if (g_currentMap.name == SHEN_NAME(0014)) {
        DateTime time;
        shenGetCurrentDateTime(time);
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
            EV_SetObjectFlagByIndex(14, 0);
            EV_SetObjectFlagByIndex(15, 1);
            SetLegacyVertexColourType(2);
        } else {
            EV_lightLoad(SHEN_NAME(0000));
            PushGlobalLightingProgram();
            EV_SetObjectFlagByIndex(14, 1);
            EV_SetObjectFlagByIndex(15, 0);
            SetLegacyVertexColourType(1);
        }
    }
#endif
    FTBL.EV_LoadCutsceneFromCurrentScene("0014A000");
    FTBL.EV_FreeAuthorPak(1, "0014");
}

E0014_INLINE void Cleanup(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EventDateTime time;
    EVT_AT_SetPos(RYO_CHARACTER, INVALID_CHARACTER, 255.88f, 100.2f, 350.5f, 0);
    EVT_AT_SetAng(RYO_CHARACTER, INVALID_CHARACTER, 0.0f, 24.85f, 0.0f, 0);
    FTBL.EV_ControlPackageResources(0, 6, "0014A", 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0014");
    FTBL.SetPlayerGameFlags(4, 1);
#else
    FTBL.SetPlayerGameFlags(4, 1);
    FTBL.EV_FreeAuthorPak(0, "0014");
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == SHEN_NAME4('A', 'R', '0', '2')) {
#else
    if (g_currentMap.name == SHEN_NAME(AR02)) {
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.EV_LoadCutsceneFromCurrentScene("AR02");
        FTBL.GetCurrentDateTime((DateTime *)&time);
        if (time.hour >= 19)
            FTBL.EV_LoadAudio("AR02_000", 1);
        else
            FTBL.EV_LoadAudio("AR02_000", 0);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
#endif
        FTBL.SetPlayerControlEnabled(1);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#endif
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - pd
void SetupCharacters(void) {
#if defined(SHENMUE_EVENT_OVERLAY)

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    int i;
    Prepare0014Character(RYO_CHARACTER);
    Hide0014Prop(SHEN_NAME4('a', '0', '1', '6'));
    Hide0014Prop(SHEN_NAME4('a', '0', '0', '9'));
    Hide0014Prop(SHEN_NAME4('a', 'b', '1', '5'));
    Hide0014Prop(SHEN_NAME4('a', 'a', '3', '7'));
#else
    int i;
    Vector3 scale = {1.5f, 1.5f, 1.5f};
    Prepare0014Character(RYO_CHARACTER);
    Hide0014Prop(SHEN_NAME4('a', '0', '1', '6'));
    Hide0014Prop(SHEN_NAME4('a', '0', '2', '7'));
    Hide0014Prop(SHEN_NAME4('a', '0', '0', '9'));
    Hide0014Prop(SHEN_NAME4('a', 'a', '3', '7'));
    FTBL.mtwkSetMotionworkScale3(SHEN_NAME4('a', '0', '2', '7'), &scale);
#endif
    for (i = 0; characters[i] != INVALID_CHARACTER; i++)
        Prepare0014Character(characters[i]);

#else
    SetCharacters(false);
#endif
}
// @verified - pd
void RestoreCharacters(void) {
#if defined(SHENMUE_EVENT_OVERLAY)

    int i;
    Restore0014Character(RYO_CHARACTER);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    Show0014Prop(SHEN_NAME4('a', '0', '1', '6'));
    Show0014Prop(SHEN_NAME4('a', '0', '0', '9'));
    Show0014Prop(SHEN_NAME4('a', 'b', '1', '5'));
    Show0014Prop(SHEN_NAME4('a', 'a', '3', '7'));
#else
    Show0014Prop(SHEN_NAME4('a', '0', '1', '6'));
    Show0014Prop(SHEN_NAME4('a', '0', '2', '7'));
    Show0014Prop(SHEN_NAME4('a', '0', '0', '9'));
    Show0014Prop(SHEN_NAME4('a', 'a', '3', '7'));
#endif
    for (i = 0; characters[i] != INVALID_CHARACTER; i++)
        Restore0014Character(characters[i]);

#else
    SetCharacters(true);
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Prepare0014Character(shen_name name) {
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
void Restore0014Character(shen_name name) {
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
void Hide0014Prop(shen_name name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
void Show0014Prop(shen_name name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
#endif

#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace

const ScriptAreaDefinition area0014ScriptDefinition{.areaId = SHEN_NAME(0014), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
#endif
#undef E0014_INLINE
