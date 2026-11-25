#include "s2/areas/area_0307.hpp"
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
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/weather.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::int32_t weather0307;
}

// @verified - p
void Initialize0307() {
    weather0307 = 0;
}
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
#define E0307_INLINE
#define E0307_LOCAL
typedef shen_name E0307CharacterName;
#else
#if defined(_MSC_VER)
#define E0307_INLINE __forceinline
#else
#define E0307_INLINE inline __attribute__((always_inline))
#endif
#define E0307_LOCAL static
typedef const shen_name &E0307CharacterName;
#endif
E0307_LOCAL E0307_INLINE void Setup0307Scene(void);
E0307_LOCAL E0307_INLINE void Prepare0307Characters(void);
E0307_LOCAL E0307_INLINE void Restore0307Characters(void);
E0307_LOCAL E0307_INLINE void Prepare0307Character(E0307CharacterName);
E0307_LOCAL E0307_INLINE void Restore0307Character(E0307CharacterName);
E0307_LOCAL E0307_INLINE void Hide0307Prop(E0307CharacterName);
E0307_LOCAL E0307_INLINE void Show0307Prop(E0307CharacterName);
#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
// @verified - pd
void Start0307(
#if defined(SHENMUE_EVENT_OVERLAY)
    void
#else
    std::uintptr_t
#endif
) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0307, 4, 11, sizeof(Area0307Parameter), AUTHOR_VIEWER_TASK);
    Area0307Parameter *parent = (Area0307Parameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    Setup0307Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0307_CHARACTERS, parent->self);
}
#endif
#if !defined(SHENMUE_EVENT_ENTRY)
#if defined(SHENMUE_EVENT_OVERLAY)
static int weather0307;
#endif
// @verified - pd
void Update0307(void *data) {
    Area0307Parameter *parent = (Area0307Parameter *)data;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        Queue0307Author(parent);
        break;
    case 3:
        Cleanup0307(parent);
        break;
    }
#else
    if (parent->state == 1)
        Queue0307Author(parent);
    else if (parent->state == 3)
        Cleanup0307(parent);
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Queue0307Author(Area0307Parameter *parent)
#else
// @verified - p
std::int64_t Queue0307Author(Area0307Parameter *parent)
#endif
{
    HLibTask *task;
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0307Author, 4, 11, sizeof(Area0307AuthorParameter), AUTHOR_VIEWER_TASK);
#else
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0307AuthorTask, 4, 11, sizeof(Area0307AuthorParameter), AUTHOR_VIEWER_TASK);
#endif
    ((Area0307AuthorParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    Prepare0307Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(AREA_0307_AUTHOR);
#else
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0307_AUTHOR.name));
#endif
    parent->state = 2;
#if !defined(SHENMUE_EVENT_OVERLAY)
    return result;
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)
void Update0307Author(void *data)
#else
// @verified - p
std::int64_t Update0307Author(Area0307AuthorParameter *child)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    Area0307AuthorParameter *child = (Area0307AuthorParameter *)data;
    int result;
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == 271) {
        weather0307 = FTBL.GetCurrentWeather();
        if (weather0307 == 2)
            FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    } else if (frame == 449 && weather0307 == 2) {
        FTBL.GetWeatherRainPressure();
        FTBL.FreeWAIR();
        FTBL.UpdateWeather();
    }
    result = FTBL.EV_authSceneCtrl(-1);
#else
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 271) {
        weather0307 = GetCurrentWeather();
        if (weather0307 == 2)
            EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    } else if (frame == 449 && weather0307 == 2) {
        ResetWeatherOverrides();
        EV_FreeWAIR();
        EV_UpdateWeather();
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
#endif
    if ((unsigned int)result == 0) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 3;
        Restore0307Characters();
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
void Update0307AuthorTask(void *data) {
    Update0307Author(static_cast<Area0307AuthorParameter *>(data));
}
#endif
E0307_LOCAL E0307_INLINE void Setup0307Scene(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0307)
#else
    if (g_currentMap.name == AREA_0307.name)
#endif
        FTBL.SetObjectFlagByIndex(4, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0307A000");
    FTBL.EV_FreeAuthorPak(1, AREA_0307_CONVERSATION);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0307_RETURN)
#else
    if (g_currentMap.name == AREA_0307_RETURN.name)
#endif
        FTBL.EV_DisableCharacterDisplay(AREA_0307_BIKE);
}
// @verified - pd
void Cleanup0307(Area0307Parameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0307_CHARACTERS);
#else
    for (const shen_name *character = AREA_0307_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0307_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, AREA_0307_ARCHIVE, 0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.LoadCutsceneAndSoundProgramForArea();
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0307_RETURN) {
        EVT_AT_SetPos(AREA_0307_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 264.8f, 100.0f, 183.58f, 0);
        EVT_AT_SetAng(AREA_0307_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, 0.0f, 90.0f, 0.0f, 0);
#else
    if (g_currentMap.name == AREA_0307_RETURN.name) {
        EventSetPosition position{0, AREA_0307_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {264.79998779296875f, 100.0f, 183.5800018310547f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0307_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 0x4000, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
#endif
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_EnableCharacterDisplay(AREA_0307_BIKE);
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_spmBeginSampleExecute(0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
E0307_LOCAL E0307_INLINE void Prepare0307Characters(void) {
    Prepare0307Character(AREA_0307_RYO_);
    Prepare0307Character(AREA_0307_JOP_);
    Hide0307Prop(AREA_0307_a396);
}
E0307_LOCAL E0307_INLINE void Restore0307Characters(void) {
    Restore0307Character(AREA_0307_RYO_);
    Restore0307Character(AREA_0307_JOP_);
    Show0307Prop(AREA_0307_a396);
}
E0307_LOCAL E0307_INLINE void Prepare0307Character(E0307CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0307_LOCAL E0307_INLINE void Restore0307Character(E0307CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0307_LOCAL E0307_INLINE void Hide0307Prop(E0307CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0307_LOCAL E0307_INLINE void Show0307Prop(E0307CharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0307_CHARACTERS[] = {AREA_0307_JOP_, AUTHOR_VIEWER_INVALID_CHARACTER};
#endif
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition area0307ScriptDefinition{.areaId = AREA_0307, .field04 = 0, .init = Initialize0307, .main = Start0307};
} // namespace shenmue::s2::areas
#endif
#undef E0307_INLINE
#undef E0307_LOCAL
