#include "s2/areas/area_0297.hpp"
#include "s2/events/event_api.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/author.hpp"
#include "s2/map_assets.hpp"
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
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0297() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0297_INLINE
#define E0297_LOCAL
typedef shen_name E0297CharacterName;
#else
#if defined(_MSC_VER)
#define E0297_INLINE __forceinline
#else
#define E0297_INLINE inline __attribute__((always_inline))
#endif
#define E0297_LOCAL static
typedef const shen_name &E0297CharacterName;
#endif
void Prepare0297Characters(void);
void Restore0297Characters(void);
E0297_LOCAL E0297_INLINE void Prepare0297Character(E0297CharacterName);
E0297_LOCAL E0297_INLINE void Restore0297Character(E0297CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Start0297(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0297, 4, 11, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *parent = (AuthorViewerTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    Setup0297Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0297_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void Update0297(void *data) {
    AuthorViewerTaskParameter *parent = (AuthorViewerTaskParameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        Queue0297Author(parent);
        break;
    case 3:
        Cleanup0297(parent);
        break;
    }
#else
    if (parent->state == 1)
        Queue0297Author(parent);
    else if (parent->state == 3)
        Cleanup0297(parent);
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Queue0297Author(AuthorViewerTaskParameter *parent)
#else
// @verified - p
std::int64_t Queue0297Author(AuthorViewerTaskParameter *parent)
#endif
{
    HLibTask *task;
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0297Author, 4, 11, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
#else
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0297AuthorTask, 4, 11, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
#endif
    ((AuthorViewerLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0297Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(AREA_0297_AUTHOR);
#else
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0297_AUTHOR.name));
#endif
    parent->state = 2;
#if !defined(SHENMUE_EVENT_OVERLAY)
    return 2;
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Update0297Author(void *data)
#else
// @verified - p
std::int64_t Update0297Author(AuthorViewerLoopTaskParameter *child)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    AuthorViewerLoopTaskParameter *child = (AuthorViewerLoopTaskParameter *)data;
    int result = FTBL.EV_authSceneCtrl(-1);
#else
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
#endif
    if ((unsigned int)result == 0) {
        Restore0297Characters();
        child->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
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
void Update0297AuthorTask(void *data) {
    Update0297Author(static_cast<AuthorViewerLoopTaskParameter *>(data));
}
#endif
void Setup0297Scene(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 root;
    int i;
#endif
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0297) {
#else
    if (g_currentMap.name == AREA_0297.name) {
#endif
        FTBL.SetMapModelDataDefaultVertexShader();
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
#else
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
#endif
        FTBL.PushGlobalLightingProgram();
#if defined(SHENMUE_EVENT_OVERLAY)
        root.x = 0.0f;
        root.y = 10.8f;
        root.z = 0.0f;
        FTBL.SetLightRootPosition(&root);
        for (i = 0; i < 9; i++)
            FTBL.SetMapModelSlotTransform(i, &root, 0);
#else
        const Vector3 root{0.0f, 10.800000190734863f, 0.0f};
        SetLightRootPosition(root);
        for (std::uint32_t slot = 0; slot < 9; ++slot)
            SetMapModelSlotTransform(slot, &root, 0);
#endif
    }
    FTBL.EV_FreeAuthorPak(1, AREA_0297_CONVERSATION);
    FTBL.EV_LoadCutsceneFromCurrentScene("0297A000");
}
void Cleanup0297(AuthorViewerTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_AT_SetPos(AREA_0297_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 242.15f, 110.8f, 125.0f, 0);
    EVT_AT_SetAng(AREA_0297_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 90.0f, 0.0f, 0);
    EVT_AT_SetPos(AREA_0297_REN_, AUTHOR_VIEWER_INVALID_CHARACTER, 238.15f, 110.8f, 125.0f, 0);
    EVT_AT_SetAng(AREA_0297_REN_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 90.0f, 0.0f, 0);
#else
    EventSetPosition position{0, AREA_0297_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {242.14999389648438f, 110.80000305175781f, 125.0f}, 0.0f};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{0, AREA_0297_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 0x4000, 0}, 0.0f};
    EVT_AT_SetAng_Callback(&angle);
    position.character = AREA_0297_REN_;
    position.position = {238.14999389648438f, 110.80000305175781f, 125.0f};
    EVT_AT_SetPosRot(&position);
    angle.character = AREA_0297_REN_;
    EVT_AT_SetAng_Callback(&angle);
#endif
    FTBL.npcLockUnlock(AREA_0297_REN_);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_FreeAuthorPak(0, AREA_0297_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0297A", 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0297_RETURN) {
#else
    if (g_currentMap.name == AREA_0297_RETURN.name) {
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
void Prepare0297Characters(void) {
    Prepare0297Character(AREA_0297_RYO_);
    Prepare0297Character(AREA_0297_REN_);
    Prepare0297Character(AREA_0297_CHD_);
}
void Restore0297Characters(void) {
    Restore0297Character(AREA_0297_RYO_);
    Restore0297Character(AREA_0297_REN_);
    Restore0297Character(AREA_0297_CHD_);
}
E0297_LOCAL E0297_INLINE void Prepare0297Character(E0297CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0297_LOCAL E0297_INLINE void Restore0297Character(E0297CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0297_CHARACTERS[] = {AREA_0297_REN_, AREA_0297_CHD_, AUTHOR_VIEWER_INVALID_CHARACTER};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition area0297ScriptDefinition{.areaId = AREA_0297, .field04 = 0, .init = Initialize0297, .main = Start0297};
} // namespace shenmue::s2::areas
#endif
#undef E0297_INLINE
#undef E0297_LOCAL
