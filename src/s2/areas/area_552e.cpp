#include "s2/areas/area_552e.hpp"
#include "s2/events/event_api.h"

#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/pause.hpp"
#include "s2/flag.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include <bit>
namespace shenmue::s2::areas {
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
#define E552E_INLINE
#define E552E_LOCAL
#else
#define E552E_LOCAL static
#if defined(_MSC_VER)
#define E552E_INLINE __forceinline
#else
#define E552E_INLINE inline __attribute__((always_inline))
#endif
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
typedef shen_name E552ECharacterName;
#else
typedef const shen_name &E552ECharacterName;
#endif
E552E_LOCAL E552E_INLINE void Queue552EAuthor(AuthorViewerPairTaskParameter *parent);
E552E_LOCAL E552E_INLINE void Setup552E(void);
#if !defined(SHENMUE_EVENT_OVERLAY)
static E552E_INLINE void Prepare552ECharactersBody(void);
#endif
E552E_LOCAL E552E_INLINE void Prepare552ECharacter(E552ECharacterName name);
E552E_LOCAL E552E_INLINE void Restore552ECharacter(E552ECharacterName name);
E552E_LOCAL E552E_INLINE void Hide552EProp(E552ECharacterName name);
E552E_LOCAL E552E_INLINE void Show552EProp(E552ECharacterName name);

#if !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void Initialize552E() {
}
#endif

// @verified - p
void Start552E(
#if !defined(SHENMUE_EVENT_OVERLAY)
    std::uintptr_t
#else
    void
#endif
) {
    HLibTask *task;
    AuthorViewerPairTaskParameter *parent;
    FTBL.SetRenderClockAndCompassTo(1);
    task = FTBL.HLib_EnqueueTaskWithParameter(Update552E, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    parent = (AuthorViewerPairTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->selection = 0;
    parent->state = 1;
    FTBL.UpdateWeatherForMinute(0, 0, 0, 1.0f);
    FTBL.EV_authSceneCtrl(-11);
#else
    parent->state = 1;
    parent->selection = 0;
    EV_UpdateWeatherForMinute(0, 0, 0, 1.0f);
#endif
    Setup552E();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_552E_CHARACTERS, parent->self);
}

// @verified - p
void Update552E(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = (AuthorViewerPairTaskParameter *)taskData;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        Queue552EAuthor(parent);
        break;
    case 3:
        Cleanup552E();
        break;
    }
#else
    if (parent->state == 1)
        Queue552EAuthor(parent);
    else if (parent->state == 3)
        Cleanup552E();
#endif
}

E552E_LOCAL E552E_INLINE void Queue552EAuthor(AuthorViewerPairTaskParameter *parent) {
    HLibTask *const task = FTBL.HLib_EnqueueTaskWithParameter(Update552EAuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    ((AuthorViewer519ELoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_spmBeginSampleExecute(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    Prepare552ECharacters();
#else
    Prepare552ECharactersBody();
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(parent->selection);
#else
    FTBL.EV_authSceneCtrl(std::bit_cast<std::int32_t>(parent->selection));
#endif
    parent->state = 2;
}

#if defined(SHENMUE_EVENT_OVERLAY)
void Update552EAuthorTask(void *taskData) {
    AuthorViewer519ELoopTaskParameter *const child = (AuthorViewer519ELoopTaskParameter *)taskData;
    const int result = FTBL.EV_authSceneCtrl(-1);
#else
// @verified - p
std::int64_t Update552EAuthor(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = FTBL.EV_authSceneCtrl(-1);
#endif
    if ((unsigned int)result == 0) {
        child->parent->state = 3;
        Restore552ECharacters();
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.HLib_DestroyCurrentEventTask();
#else
        return FTBL.HLib_DestroyCurrentTask();
#endif
    }
#if !defined(SHENMUE_EVENT_OVERLAY)
    return result;
#endif
}

E552E_LOCAL E552E_INLINE void Setup552E(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, AREA_552E_CONVERSATION);
}

// @verified - p
void Cleanup552E(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_552E_CHARACTERS);
#else
    for (const shen_name *character = AREA_552E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_552E_CONVERSATION);
    FTBL.EV_EventSetUpHelpIcon_0(0x40, 0);
    FTBL.Queue_ST_LoadTitleFadeIn(AREA_552E_TITLE);
    FTBL.FinishLoading();
    FTBL.HLib_SetCurrentTaskTo(Exit552EB);
    FTBL.HLib_SetupTaskDelay(120);
}

// @verified - p
void Exit552EB(void *taskData) {
    FTBL.ST_LoadTitleFade();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetLoadingBusy();
#else
    Initialize552E();
#endif
    FTBL.HLib_SetCurrentTaskTo(Exit552EC);
    FTBL.HLib_SetupTaskDelay(30);
}

// @verified - p
void Exit552EC(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime time;
#endif
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(((AuthorViewerPairTaskParameter *)taskData)->self);
    FTBL.EV_SetPlayerFlag(12, 902, 0);
    FTBL.EV_SetPlayerFlag(12, 901, 1);
    FTBL.PTYCUtils(AREA_552E_SIN_, 3, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0x459)) {
        FTBL.EV_stepSetFlags(0x459, 0);
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 9;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('K', 'M', 'Z', '1'), 99);
    } else if (FTBL.EV_GetShenmueFlag(0x454)) {
        FTBL.EV_stepSetFlags(0x454, 0);
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 9;
        time.minute = 30;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('K', 'M', 'Z', '2'), 0);
    } else if (FTBL.EV_GetShenmueFlag(0x455)) {
        FTBL.EV_stepSetFlags(0x455, 0);
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 10;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('K', 'M', 'Z', '3'), 0);
    } else if (FTBL.EV_GetShenmueFlag(0x456)) {
        FTBL.EV_stepSetFlags(0x456, 0);
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 11;
        time.minute = 30;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('K', 'M', 'Z', '3'), 99);
    } else if (FTBL.EV_GetShenmueFlag(0x457)) {
        FTBL.EV_stepSetFlags(0x457, 0);
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 15;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('K', 'M', 'Z', '4'), 0);
    } else {
        FTBL.EV_stepSetFlags(0x459, 0);
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 9;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('K', 'M', 'Z', '1'), 99);
    }
#else
    std::int32_t flag = 0x459;
    shen_name map = AREA_552E_FOREST1;
    std::uint32_t entry = 99;
    std::int8_t hour = 9, minute = 0;
    if (!EV_GetShenmueFlag(0x459)) {
        if (EV_GetShenmueFlag(0x454)) {
            flag = 0x454;
            map = AREA_552E_FOREST2;
            entry = 0;
            minute = 30;
        } else if (EV_GetShenmueFlag(0x455)) {
            flag = 0x455;
            map = AREA_552E_FOREST3;
            entry = 0;
            hour = 10;
        } else if (EV_GetShenmueFlag(0x456)) {
            flag = 0x456;
            map = AREA_552E_FOREST3;
            hour = 11;
            minute = 30;
        } else if (EV_GetShenmueFlag(0x457)) {
            flag = 0x457;
            map = AREA_552E_FOREST4;
            entry = 0;
            hour = 15;
        }
    }
    EV_stepSetFlags(0, flag);
    DateTime time;
    shenGetCurrentDateTime(time);
    time.hour = hour;
    time.minute = minute;
    SetCurrentDateTimeTo(time);
    scenSetTaskFlags(8);
    EV_scenQueueLoad(g_actNumber, map, entry);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void Prepare552ECharacters(void)
#else
static E552E_INLINE void Prepare552ECharactersBody(void)
#endif
{
    Prepare552ECharacter(AREA_552E_RYO_);
    Prepare552ECharacter(AREA_552E_SIN_);
    Hide552EProp(AREA_552E_ab02);
    Hide552EProp(AREA_552E_ab03);
}
#if !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void Prepare552ECharacters(void) {
    Prepare552ECharactersBody();
}
#endif

// @verified - p
void Restore552ECharacters(void) {
    Restore552ECharacter(AREA_552E_RYO_);
    Restore552ECharacter(AREA_552E_SIN_);
    Show552EProp(AREA_552E_ab02);
    Show552EProp(AREA_552E_ab03);
}
E552E_LOCAL E552E_INLINE void Prepare552ECharacter(E552ECharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E552E_LOCAL E552E_INLINE void Restore552ECharacter(E552ECharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E552E_LOCAL E552E_INLINE void Hide552EProp(E552ECharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E552E_LOCAL E552E_INLINE void Show552EProp(E552ECharacterName name) {
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_552E_CHARACTERS[] = {AREA_552E_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
#else
void Update552EAuthorTask(void *taskData) {
    Update552EAuthor(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

const ScriptAreaDefinition area552EScriptDefinition{.areaId = AREA_552E, .field04 = 0, .init = Initialize552E, .main = Start552E};
} // namespace shenmue::s2::areas
#endif
#undef E552E_INLINE
#undef E552E_LOCAL
