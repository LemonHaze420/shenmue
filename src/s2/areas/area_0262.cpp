#include "s2/areas/area_0262.hpp"
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
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/fog.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

// @verified - p
void E0262Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0262_INLINE
#define E0262_LOCAL
typedef shen_name E0262CharacterName;
#else
#if defined(_MSC_VER)
#define E0262_INLINE __forceinline
#else
#define E0262_INLINE inline __attribute__((always_inline))
#endif
#define E0262_LOCAL static
typedef const shen_name &E0262CharacterName;
#endif
void E0262MainCallback(void *);
void E0262LoopCallback(void *);
E0262_LOCAL E0262_INLINE void QueueE0262Author(AreaImmediateAuthorTaskParameter *);
E0262_LOCAL E0262_INLINE void ConfigureE0262Environment(void);
E0262_LOCAL E0262_INLINE void CleanupE0262(AreaImmediateAuthorTaskParameter *);
E0262_LOCAL E0262_INLINE void SetupE0262Characters(void);
E0262_LOCAL E0262_INLINE void RestoreE0262Characters(void);
E0262_LOCAL E0262_INLINE void PrepareE0262Character(E0262CharacterName);
E0262_LOCAL E0262_INLINE void RestoreE0262Character(E0262CharacterName);
E0262_LOCAL E0262_INLINE void HideE0262Prop(E0262CharacterName);
E0262_LOCAL E0262_INLINE void ShowE0262Prop(E0262CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void E0262Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    short *argument
#else
    std::uintptr_t argument
#endif
) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetPlayerFlag(16, 32, time.hour);
    FTBL.EV_SetPlayerFlag(16, 33, time.minute);
    FTBL.EV_SetPlayerFlag(16, 34, time.day);
    FTBL.EV_SetPlayerFlag(16, 48, time.year);
    FTBL.EV_SetPlayerFlag(16, 49, time.month);
#else
    DateTime time;
    shenGetCurrentDateTime(time);
    SetPlayerControlEnabled(0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 32, time.hour);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 33, time.minute);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 34, static_cast<std::int8_t>(time.day));
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 48, time.year);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 49, static_cast<std::int8_t>(time.month));
    HLibTask *task;
    AreaImmediateAuthorTaskParameter *parent;
#endif
    task = FTBL.HLib_EnqueueTaskWithParameter(E0262MainCallback, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    if (g_currentMap == AREA_0262_AREA)
        parent->selection = (unsigned int)g_entryPoint > 1 ? 0 : g_entryPoint;
    else
        parent->selection = *argument;
#else
    parent->selection = g_currentMap.name == AREA_0262_AREA.name ? static_cast<std::int16_t>(g_entryPoint > 1 ? 0 : g_entryPoint) :
                                                                   *reinterpret_cast<const std::int16_t *>(argument);
    parent->state = 1;
#endif
    ConfigureE0262Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0262_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0262MainCallback(void *data) {
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0262Author(parent);
        break;
    case 3:
        CleanupE0262(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0262Author(parent);
    else if (parent->state == 3)
        CleanupE0262(parent);
#endif
}
E0262_LOCAL E0262_INLINE void QueueE0262Author(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    shen_name authors[2] = {0};
#endif
    HLibTask *task;
    AreaImmediateAuthorFrameParameter *child;
#if defined(SHENMUE_EVENT_OVERLAY)
    authors[0] = AREA_0262_262A;
    authors[1] = AREA_0262_262B;
#endif
    task = FTBL.HLib_EnqueueTaskWithParameter(E0262LoopCallback, 4, 11, sizeof(AreaImmediateAuthorFrameParameter), AREA_AUTHOR_TASK);
    child = (AreaImmediateAuthorFrameParameter *)FTBL.HLib_GetTaskParameter(task);
#if defined(SHENMUE_EVENT_OVERLAY)
    child->parent = parent;
    child->state = 0;
#else
    child->state = 0;
    child->parent = parent;
#endif
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_spmBeginSampleExecute(0);
#else
    BeginSecondarySampleExecution(0);
#endif
    SetupE0262Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(authors[parent->selection]);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0262_AUTHORS[parent->selection].name));
#endif
    parent->state = 2;
}
// @verified - pd
void E0262LoopCallback(void *data) {
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaImmediateAuthorFrameParameter *child = (AreaImmediateAuthorFrameParameter *)data;
    if (!FTBL.EV_authSceneCtrl(-1)) {
#else
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
#endif
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.HLib_SetupTimedTask(child->parent->self, 130);
#else
    HLib::SetupTimedTask(static_cast<AreaImmediateAuthorFrameParameter *>(data)->parent->self, 130);
#endif
        FTBL.AddInventoryItem(0x1787, 1, 1);
        RestoreE0262Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
        child->parent->state = 3;
#else
    static_cast<AreaImmediateAuthorFrameParameter *>(data)->parent->state = 3;
#endif
        FTBL.HLib_DestroyCurrentTask();
#if defined(SHENMUE_EVENT_OVERLAY)
    }
#endif
}
E0262_LOCAL E0262_INLINE void ConfigureE0262Environment(void) {
    FTBL.EV_SetWeatherInstruction(12, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetMapModelDataDefaultVertexShader();
#else
    SetMapModelDataDefaultVertexShader();
#endif
    FTBL.EV_lightLoad(AREA_0262_L001);
    FTBL.PushGlobalLightingProgram();
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0262A000");
    FTBL.EV_FreeAuthorPak(1, AREA_0262_CONVERSATION);
}
E0262_LOCAL E0262_INLINE void CleanupE0262(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0262_CHARACTERS);
#else
    for (const shen_name *character = AREA_0262_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0262_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0262A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_NPC_NewLoadSw_0(1);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0262_AREA)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
#else
    if (g_currentMap.name == AREA_0262_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0262_AREA, g_entryPoint);
#endif
    else {
        FTBL.LoadCutsceneAndSoundProgramForArea();
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 215.3f, 92.8f, 129.8f, 0);
        EVT_AT_SetAng(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0.0f, 270.0f, 0.0f, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
#else
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {215.3f, 92.8f, 129.8f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 49152, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
        SetMapModelDataDefaultVertexShader();
#endif
        FTBL.EV_lightLoad(AREA_0262_0000);
        FTBL.PushGlobalLightingProgram();
        FTBL.SetAreaFogParams();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_spmBeginSampleExecute(0);
#else
        BeginSecondarySampleExecution(0);
#endif
    }
}
E0262_LOCAL E0262_INLINE void SetupE0262Characters(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    int i;
#endif
    PrepareE0262Character(AREA_AUTHOR_RYO);
    HideE0262Prop(AREA_0262_A276);
    HideE0262Prop(AREA_0262_AA80);
    HideE0262Prop(AREA_0262_AA77);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (i = 0; AREA_0262_CHARACTERS[i] != AREA_AUTHOR_INVALID; i++)
        PrepareE0262Character(AREA_0262_CHARACTERS[i]);
#else
    for (const shen_name *character = AREA_0262_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
#endif
}
E0262_LOCAL E0262_INLINE void RestoreE0262Characters(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    int i;
#endif
    RestoreE0262Character(AREA_AUTHOR_RYO);
    ShowE0262Prop(AREA_0262_A276);
    ShowE0262Prop(AREA_0262_AA80);
    ShowE0262Prop(AREA_0262_AA77);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (i = 0; AREA_0262_CHARACTERS[i] != AREA_AUTHOR_INVALID; i++)
        RestoreE0262Character(AREA_0262_CHARACTERS[i]);
#else
    for (const shen_name *character = AREA_0262_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
#endif
}
E0262_LOCAL E0262_INLINE void PrepareE0262Character(E0262CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0262_LOCAL E0262_INLINE void RestoreE0262Character(E0262CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0262_LOCAL E0262_INLINE void HideE0262Prop(E0262CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0262_LOCAL E0262_INLINE void ShowE0262Prop(E0262CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0262_CHARACTERS[] = {AREA_AUTHOR_REN, AREA_0262_WON_, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0262ScriptDefinition{.areaId = AREA_0262_AREA, .field04 = 0, .init = E0262Init, .main = E0262Main};
} // namespace shenmue::s2::areas
#endif
#undef E0262_INLINE
#undef E0262_LOCAL
