#include "s2/areas/area_0361.hpp"
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
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0361() {
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0361_INLINE
#define E0361_LOCAL
typedef shen_name E0361CharacterName;
#else
#if defined(_MSC_VER)
#define E0361_INLINE __forceinline
#else
#define E0361_INLINE inline __attribute__((always_inline))
#endif
#define E0361_LOCAL static
typedef const shen_name &E0361CharacterName;
#endif
E0361_LOCAL E0361_INLINE void Prepare0361Characters(void);
E0361_LOCAL E0361_INLINE void Restore0361Characters(void);
E0361_LOCAL E0361_INLINE void Prepare0361Character(E0361CharacterName);
E0361_LOCAL E0361_INLINE void Restore0361Character(E0361CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Start0361(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0361, 4, 11, sizeof(Area0361Parameter), AUTHOR_VIEWER_TASK);
    Area0361Parameter *parent = (Area0361Parameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    Setup0361Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0361_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - pd
void Update0361(void *data) {
    Area0361Parameter *parent = (Area0361Parameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        Queue0361Author(parent);
        break;
    case 3:
        Cleanup0361(parent);
        break;
    }
#else
    if (parent->state == 1)
        Queue0361Author(parent);
    else if (parent->state == 3)
        Cleanup0361(parent);
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Queue0361Author(Area0361Parameter *parent)
#else
// @verified - p
std::int64_t Queue0361Author(Area0361Parameter *parent)
#endif
{
    HLibTask *task;
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0361Author, 4, 11, sizeof(Area0361AuthorParameter), AUTHOR_VIEWER_TASK);
#else
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0361AuthorTask, 4, 11, sizeof(Area0361AuthorParameter), AUTHOR_VIEWER_TASK);
#endif
    ((Area0361AuthorParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.qronSetFloor(17);
    FTBL.EV_spmBeginSampleExecute(0);
    Prepare0361Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(AREA_0361_AUTHOR);
#else
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0361_AUTHOR.name));
#endif
    parent->state = 2;
#if !defined(SHENMUE_EVENT_OVERLAY)
    return result;
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Update0361Author(void *data)
#else
// @verified - p
std::int64_t Update0361Author(Area0361AuthorParameter *child)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    Area0361AuthorParameter *child = (Area0361AuthorParameter *)data;
    int result = FTBL.EV_authSceneCtrl(-1);
#else
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
#endif
    if ((unsigned int)result == 0) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 3;
        Restore0361Characters();
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
void Update0361AuthorTask(void *taskData) {
    Update0361Author(static_cast<Area0361AuthorParameter *>(taskData));
}
#endif
// @verified - pd
void Setup0361Scene(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 root;
    int i;
#endif
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap != AREA_0361_RETURN) {
#else
    if (g_currentMap.name != AREA_0361_RETURN.name) {
#endif
        FTBL.EV_SetWeatherInstruction(12, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
#else
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
#endif
        FTBL.PushGlobalLightingProgram();
#if defined(SHENMUE_EVENT_OVERLAY)
        root.x = 0.0f;
        root.y = 61.2f;
        root.z = 0.0f;
        FTBL.SetLightRootPosition(&root);
        for (i = 0; i < 9; i++)
            FTBL.SetMapModelSlotTransform(i, &root, 0);
#else
        const Vector3 root{0.0f, 61.20000076293945f, 0.0f};
        SetLightRootPosition(root);
        for (std::uint32_t slot = 0; slot < 9; ++slot)
            SetMapModelSlotTransform(slot, &root, 0);
#endif
    }
    FTBL.EV_FreeAuthorPak(1, AREA_0361_CONVERSATION);
    FTBL.EV_LoadCutsceneFromCurrentScene("0361A000");
}
// @verified - pd
void Cleanup0361(Area0361Parameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0361_CHARACTERS);
#else
    for (const shen_name *character = AREA_0361_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_ControlPackageResources(0, 6, AREA_0361_ARCHIVE, 0, 0);
    FTBL.EV_FreeAuthorPak(0, AREA_0361_CONVERSATION);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0361_RETURN) {
#else
    if (g_currentMap.name == AREA_0361_RETURN.name) {
#endif
        FTBL.LoadCutsceneAndSoundProgramForArea();
#if defined(SHENMUE_EVENT_OVERLAY)
        EVT_AT_SetPos(AREA_0361_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 255.75f, 161.2f, 139.98f, 0);
        EVT_AT_SetAng(AREA_0361_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 90.0f, 0.0f, 0);
#else
        EventSetPosition position{0, AREA_0361_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {255.75f, 161.1999969482422f, 139.97999572753906f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0361_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 0x4000, 0}, 0.0f};
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
E0361_LOCAL E0361_INLINE void Prepare0361Characters(void) {
    Prepare0361Character(AREA_0361_RYO_);
    Prepare0361Character(AREA_0361_REN_);
}
E0361_LOCAL E0361_INLINE void Restore0361Characters(void) {
    Restore0361Character(AREA_0361_RYO_);
    Restore0361Character(AREA_0361_REN_);
}
E0361_LOCAL E0361_INLINE void Prepare0361Character(E0361CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0361_LOCAL E0361_INLINE void Restore0361Character(E0361CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0361_CHARACTERS[] = {AREA_0361_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition area0361ScriptDefinition{.areaId = AREA_0361, .field04 = 0, .init = Initialize0361, .main = Start0361};
} // namespace shenmue::s2::areas
#endif
#undef E0361_INLINE
#undef E0361_LOCAL
