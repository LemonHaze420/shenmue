#include "s2/areas/area_0339.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0339() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0339_INLINE
#define E0339_LOCAL
typedef shen_name E0339CharacterName;
#else
#if defined(_MSC_VER)
#define E0339_INLINE __forceinline
#else
#define E0339_INLINE inline __attribute__((always_inline))
#endif
#define E0339_LOCAL static
typedef const shen_name &E0339CharacterName;
#endif
void Prepare0339Characters(void);
void Restore0339Characters(void);
E0339_LOCAL E0339_INLINE void Prepare0339Character(E0339CharacterName);
E0339_LOCAL E0339_INLINE void Restore0339Character(E0339CharacterName);
E0339_LOCAL E0339_INLINE void Hide0339Prop(E0339CharacterName);
E0339_LOCAL E0339_INLINE void Show0339Prop(E0339CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Start0339(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task;
    AuthorViewerTaskParameter *parent;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EventFunction857(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0339, 4, 11, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    parent = (AuthorViewerTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    Setup0339Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0339_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void Update0339(void *data) {
    AuthorViewerTaskParameter *parent = (AuthorViewerTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        Queue0339Author(parent);
        break;
    case 3:
        Cleanup0339(parent);
        break;
    }
#else
    if (parent->state == 1)
        Queue0339Author(parent);
    else if (parent->state == 3)
        Cleanup0339(parent);
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Queue0339Author(AuthorViewerTaskParameter *parent)
#else
// @verified - p
std::int64_t Queue0339Author(AuthorViewerTaskParameter *parent)
#endif
{
    HLibTask *task;
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0339Author, 4, 11, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
#else
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0339AuthorTask, 4, 11, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
#endif
    ((AuthorViewerLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0339Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(AREA_0339_AUTHOR);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0339_AUTHOR.name));
#endif
    parent->state = 2;
#if !defined(SHENMUE_EVENT_OVERLAY)
    return 2;
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Update0339Author(void *data)
#else
// @verified - p
std::int64_t Update0339Author(AuthorViewerLoopTaskParameter *child)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    AuthorViewerLoopTaskParameter *child = (AuthorViewerLoopTaskParameter *)data;
    int result = FTBL.EV_authSceneCtrl(-1);
#else
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
#endif
    if ((unsigned int)result == 0) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0339Characters();
        child->parent->state = 3;
#if !defined(SHENMUE_EVENT_OVERLAY)
        return
#endif
            FTBL.HLib_DestroyCurrentTask();
    }
#if !defined(SHENMUE_EVENT_OVERLAY)
    return result;
#endif
}
#if !defined(SHENMUE_EVENT_OVERLAY)
void Update0339AuthorTask(void *data) {
    Update0339Author(static_cast<AuthorViewerLoopTaskParameter *>(data));
}
#endif
void Setup0339Scene(void) {
    const Vector3 center = {111.22f, 92.7f, 152.2f};
    FTBL.npcSetAreaName(AREA_0339_NPC_AREA, &center, 5.0f, 1);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_FreeAuthorPak(1, AREA_0339_CONVERSATION);
    FTBL.EV_LoadCutsceneFromCurrentScene("0339A000");
}
void Cleanup0339(AuthorViewerTaskParameter *parent) {
    FTBL.npcSetAreaName(AREA_0339_NPC_AREA, 0, 0.0f, 0);
    FTBL.EV_stepSetFlags(0x392, 0);
    FTBL.EventFunction857(1);
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0339_CHARACTERS);
#else
    for (const shen_name *character = AREA_0339_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0339_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0339A", 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0339_RETURN) {
        EVT_AT_SetPos(AREA_0339_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 114.08f, 92.34f, 159.66f, 0);
        EVT_AT_SetAng(AREA_0339_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 186.45f, 0.0f, 0);
#else
    if (g_currentMap.name == AREA_0339_RETURN.name) {
        EventSetPosition position{0, AREA_0339_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {114.08000183105469f, 92.33999633789062f, 159.66000366210938f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0339_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 33942, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
void Prepare0339Characters(void) {
    Prepare0339Character(AREA_0339_RYO_);
    Prepare0339Character(AREA_0339_REN_);
    Prepare0339Character(AREA_0339_JOP_);
    Hide0339Prop(AREA_0339_a396);
}
void Restore0339Characters(void) {
    Restore0339Character(AREA_0339_RYO_);
    Restore0339Character(AREA_0339_REN_);
    Restore0339Character(AREA_0339_JOP_);
    Show0339Prop(AREA_0339_a396);
}
E0339_LOCAL E0339_INLINE void Prepare0339Character(E0339CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0339_LOCAL E0339_INLINE void Restore0339Character(E0339CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

E0339_LOCAL E0339_INLINE void Hide0339Prop(E0339CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0339_LOCAL E0339_INLINE void Show0339Prop(E0339CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0339_CHARACTERS[] = {AREA_0339_JOP_, AREA_0339_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition area0339ScriptDefinition{.areaId = AREA_0339, .field04 = 0, .init = Initialize0339, .main = Start0339};
} // namespace shenmue::s2::areas
#endif
#undef E0339_INLINE
#undef E0339_LOCAL
