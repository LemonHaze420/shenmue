#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/debug.hpp"
#endif
#include "s2/areas/area_0201.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
// @verified - p
void E0201Init() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0201_INLINE
typedef shen_name E0201CharacterName;
#else
#if defined(_MSC_VER)
#define E0201_INLINE __forceinline
#else
#define E0201_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &E0201CharacterName;
#endif
void E0201MainCallback(void *);
void E0201LoopCallback(void *);
E0201_INLINE void SetupE0201Characters(short);
E0201_INLINE void RestoreE0201Characters(short);
void QueueE0201Loop(E0201TaskParameter *);
E0201_INLINE void CleanupE0201(E0201TaskParameter *);
E0201_INLINE void Configure0201(void);
E0201_INLINE void Prepare0201Character(E0201CharacterName);
E0201_INLINE void Restore0201Character(E0201CharacterName);
E0201_INLINE void Hide0201Prop(E0201CharacterName);
E0201_INLINE void Show0201Prop(E0201CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void E0201Main(
#if defined(SHENMUE_EVENT_OVERLAY)
    void *argument
#else
    std::uintptr_t argument
#endif
) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0201MainCallback, 4, 11, sizeof(E0201TaskParameter), AREA_AUTHOR_TASK);
    E0201TaskParameter *parent = (E0201TaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0201_AREA) {
#else
    if (g_currentMap.name == AREA_0201_AREA.name) {
#endif
        parent->state = 0;
        parent->selection = g_entryPoint != 0;
    } else {
        parent->state = 1;
        parent->selection = *(int *)argument <= 1 ? *(short *)argument : 0;
    }
    Configure0201();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0201_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void E0201MainCallback(void *data) {
    E0201TaskParameter *parent = (E0201TaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 0: {
        int input = FTBL.EVT_GetControllerInput(0, 1);
#else
    if (parent->state == 1)
        QueueE0201Loop(parent);
    else if (parent->state == 3)
        CleanupE0201(parent);
    else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
#endif
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128) {
            if (++parent->selection > 1)
                parent->selection = 0;
        }
#if defined(SHENMUE_EVENT_OVERLAY)
        switch (parent->selection) {
        case 0:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0201A000.c\n");
            break;
        case 1:
            FTBL.SetDebugScreenTextPosition(2, 4);
            FTBL.DebugLogToScreen("0201A010.c\n");
            break;
        }
        FTBL.SetDebugScreenTextPosition(2, 5);
        FTBL.DebugLogToScreen("seqdata%d.avb\n", parent->selection);
        break;
    }
#else
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0201_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    case 1:
        QueueE0201Loop(parent);
        break;
    case 3:
        CleanupE0201(parent);
        break;
    }
#endif
}
// @verified - pd
void QueueE0201Loop(E0201TaskParameter *parent) {
    HLibTask *task;
#if defined(SHENMUE_EVENT_OVERLAY)
    shen_name authors[2] = {0, 0};
    authors[0] = SHEN_NAME4('R', 'E', 'N', '1');
    authors[1] = SHEN_NAME4('R', 'E', 'N', '2');
#endif
    task = FTBL.HLib_EnqueueTaskWithParameter(E0201LoopCallback, 4, 11, sizeof(E0201LoopParameter), AREA_AUTHOR_TASK);
    ((E0201LoopParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0201_AREA) {
        if (parent->selection == 0) {
            FTBL.SetObjectFlagByIndex(0, 1);
            FTBL.SetObjectFlagByIndex(1, 1);
            FTBL.SetObjectFlagByIndex(2, 1);
            FTBL.SetObjectFlagByIndex(3, 0);
            FTBL.SetMapModelDataDefaultVertexShader();
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
        } else {
            FTBL.SetObjectFlagByIndex(0, 0);
            FTBL.SetObjectFlagByIndex(1, 0);
            FTBL.SetObjectFlagByIndex(2, 0);
            FTBL.SetObjectFlagByIndex(3, 1);
        }
    }
    if (parent->selection == 0) {
        FTBL.EV_LoadCutsceneFromCurrentScene("0201A000");
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_spmBeginSampleExecute(0);
    } else {
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_SetWeatherInstruction(12, 0);
        FTBL.EV_lightLoad(AREA_0201_LIGHT);
        FTBL.PushGlobalLightingProgram();
        FTBL.EV_LoadCutsceneFromCurrentScene("0201A010");
    }
#else
    if (g_currentMap.name == AREA_0201_AREA.name) {
        for (std::int32_t index = 0; index < 4; ++index)
            EV_SetObjectFlagByIndex(index, parent->selection != 0 ? (index == 3) : (index != 3));
        if (parent->selection == 0) {
            EV_SetModelDataDefaultVertexShader();
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        }
    }
    if (parent->selection != 0) {
        EV_SetModelDataDefaultVertexShader();
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(AREA_0201_LIGHT);
        PushGlobalLightingProgram();
        EV_LoadCutsceneFromCurrentScene("0201A010");
    } else {
        EV_LoadCutsceneFromCurrentScene("0201A000");
        EV_NPC_NewLoadSw(1);
        EV_spmBeginSampleExecute();
    }
#endif
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0201Characters(parent->selection);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(authors[parent->selection]);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0201_AUTHORS[parent->selection].name));
#endif
    parent->state = 2;
}
// @verified - pd
void E0201LoopCallback(void *data) {
    E0201LoopParameter *child = (E0201LoopParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    if (!FTBL.EV_authSceneCtrl(-1)) {
#else
    E0201TaskParameter *const parent = child->parent;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        if (g_currentMap == AREA_0201_AREA)
            child->parent->state = 0;
        else
            child->parent->state = 3;
#else
        parent->state = g_currentMap.name == AREA_0201_AREA.name ? 0 : 3;
#endif
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        RestoreE0201Characters(child->parent->selection);
#else
        RestoreE0201Characters(parent->selection);
#endif
        FTBL.HLib_DestroyCurrentTask();
    }
}
E0201_INLINE void Configure0201(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    const Vector3 center = {110.53f, 92.0f, 147.9f};
#endif
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetRenderClockAndCompassTo(1);
#if !defined(SHENMUE_EVENT_OVERLAY)
    const Vector3 center = {110.53f, 92.0f, 147.9f};
#endif
    if (FTBL.npcSetAreaName(AREA_0201_ANP1, &center, 7.0f, 1) == 1)
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("NPC_AREA1 : TRUE\n");
#else
        EV_debug_log("0201\\e0201_et_0201.c", 247, "NPC_AREA1 : TRUE\n");
#endif
    FTBL.EV_FreeAuthorPak(1, "0201");
}
// @verified - pd
E0201_INLINE void CleanupE0201(E0201TaskParameter *parent) {
    FTBL.EV_FreeAuthorPak(0, "0201");
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.DebugLogToFileRemoved("NPC_AREA1 : FREE\n");
#else
    EV_debug_log("0201\\e0201_et_0201.c", 262, "NPC_AREA1 : FREE\n");
#endif
    FTBL.npcSetAreaName(AREA_0201_ANP1, 0, 0.0f, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0201_AREA)
        FTBL.EV_ControlPackageResources(0, 6, "0201A", 0, 0);
    else {
        if (parent->selection == 1) {
            if (FTBL.EV_ControlPackageResources(0, 5, "0201A2", 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, "0201A2", 0, 0);
        } else {
            if (FTBL.EV_ControlPackageResources(0, 5, "0201A1", 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, "0201A1", 0, 0);
        }
    }
#else
    const char *const package = g_currentMap.name == AREA_0201_AREA.name ? "0201A" : parent->selection == 1 ? "0201A2" : "0201A1";
    if (g_currentMap.name == AREA_0201_AREA.name || EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0201_AREA)
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
#else
    if (g_currentMap.name == AREA_0201_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0201_AREA, g_entryPoint);
#endif
    else if (parent->selection == 1)
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0201_QR00, 97);
    else
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0201_QRR0, 1);
}
// @verified - pd
E0201_INLINE void SetupE0201Characters(short selection) {
    Prepare0201Character(AREA_AUTHOR_RYO);
    Prepare0201Character(AREA_AUTHOR_REN);
    Prepare0201Character(AREA_0201_CLJ_);
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (selection) {
    case 0:
        Hide0201Prop(AREA_0201_A358);
        break;
    case 1:
#else
    if (selection == 1) {
#endif
        Hide0201Prop(AREA_0201_A209);
        Hide0201Prop(AREA_0201_A207);
        Hide0201Prop(AREA_0201_A208);
        Hide0201Prop(AREA_0201_A270);
        Hide0201Prop(AREA_0201_A326);
        Hide0201Prop(AREA_0201_A014);

#if defined(SHENMUE_EVENT_OVERLAY)
        if (g_currentMap == AREA_0201_QRR0) {
#else
        if (g_currentMap.name == AREA_0201_QRR0.name) {
#endif
            FTBL.EV_DisableCharacterDisplay(AREA_0201_DESK);
            FTBL.EV_DisableCharacterDisplay(AREA_0201_LAMP);
        }
#if defined(SHENMUE_EVENT_OVERLAY)
        break;
#else
    } else if (selection == 0) {
        Hide0201Prop(AREA_0201_A358);
#endif
    }
}
// @verified - pd
E0201_INLINE void RestoreE0201Characters(short selection) {
    Restore0201Character(AREA_AUTHOR_RYO);
    Restore0201Character(AREA_AUTHOR_REN);
    Restore0201Character(AREA_0201_CLJ_);
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (selection) {
    case 0:
        Show0201Prop(AREA_0201_A358);
        break;
    case 1:
#else
    if (selection == 1) {
#endif
        Show0201Prop(AREA_0201_A209);
        Show0201Prop(AREA_0201_A207);
        Show0201Prop(AREA_0201_A208);
        Show0201Prop(AREA_0201_A270);
        Show0201Prop(AREA_0201_A326);
        Show0201Prop(AREA_0201_A014);
#if defined(SHENMUE_EVENT_OVERLAY)
        break;
#else
    } else if (selection == 0) {
        Show0201Prop(AREA_0201_A358);
#endif
    }
}
E0201_INLINE void Prepare0201Character(E0201CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0201_INLINE void Restore0201Character(E0201CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0201_INLINE void Hide0201Prop(E0201CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0201_INLINE void Show0201Prop(E0201CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0201_CHARACTERS[] = {AREA_0201_CLJ_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const ScriptAreaDefinition area0201ScriptDefinition{.areaId = AREA_0201_AREA, .field04 = 0, .init = E0201Init, .main = E0201Main};
} // namespace shenmue::s2::areas
#endif
#undef E0201_INLINE
