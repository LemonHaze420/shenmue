#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
float fabsf(float value);
#define QEHI_TASK TASK_EVNT
#define QEHI_RYO SHEN_NAME4('R', 'Y', 'O', '_')
void QehiMainLoopCallback(void *taskData);
void UpdateQehiSearchTargets(void);
void EVT_QteGameEnter(void);
#else
#include "s2/areas/area_qehi.hpp"
#include "s2/area_jump.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/qte_game_entry.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include <cmath>
namespace shenmue::s2 {

// @verified - p
void QehiInit() {
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || defined(SHENMUE_EVENT_ENTRY)
// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void QehiMain(unsigned int parameters) {
    HLibTask *task;
#else
void QehiMain(std::uintptr_t) {
#endif
    FTBL.EV_SetWeatherInstruction(15, 0);
    (void)FTBL.SetMapControlStatusFlag(0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    task = FTBL.HLib_EnqueueTaskWithoutParameter(QehiMainLoopCallback, 4, 0x0b, QEHI_TASK);
#else
    HLib::Task *const task = FTBL.HLib_EnqueueTaskWithoutParameter(QehiMainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(QEHI_TASK.name));
#endif
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    (void)FTBL.BeginPrimarySampleExecution(0);
    (void)FTBL.BeginSecondarySampleExecution(0);
    if (FTBL.flagRelated(g_currentMap))
        (void)FTBL.shenUpdatePlayerDataFlags(1);
    if (!FTBL.shenUpdateFlagEvaluation(1))
        FTBL.SetPlayerControlEnabled(1);
    if (FTBL.HasSavedAreaJumpLocation()) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("ON\n");
#endif
        FTBL.EV_ApplyTranslationClockVisibility();
#if defined(SHENMUE_EVENT_OVERLAY)
    } else {
        FTBL.DebugLogToFileRemoved("OFF\n");
#endif
    }
    EVT_QteGameEnter();
    FTBL.QueueImmediateNextFadeTask(task);
    FTBL.FinishLoading();
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_EVENT_ENTRY)
// @verified - p
void QehiMainLoopCallback(void *taskData) {
    (void)FTBL.shenUpdateFlagEvaluation(0);
    (void)UpdateQehiSearchTargets();
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void UpdateQehiSearchTargets(void) {
#else
std::uint64_t UpdateQehiSearchTargets() {
#endif
    Vector3 position;
    FTBL.GetCharaPosition(QEHI_RYO, -1, &position, 0x40000000);
    {
        float right = position.x - 306.5f;
        float left = position.x - 302.0f;
        float nearDistance = position.z - 77.9f;
        float farDistance = position.z - 80.48f;
#if defined(SHENMUE_EVENT_OVERLAY)
        int index;
#endif
        if (fabsf(right) < 0.00000011920929f)
            right = 0.0f;
        if (fabsf(left) < 0.00000011920929f)
            left = 0.0f;
        if (fabsf(nearDistance) < 0.00000011920929f)
            nearDistance = 0.0f;
        if (fabsf(farDistance) < 0.00000011920929f)
            farDistance = 0.0f;
#if defined(SHENMUE_EVENT_OVERLAY)
        right *= left;
        nearDistance *= farDistance;
        if (fabsf(right) < 0.00000011920929f)
            right = 0.0f;
        if (fabsf(nearDistance) < 0.00000011920929f)
            nearDistance = 0.0f;
        if ((fabsf(right) < 0.00000011920929f || right < 0.0f) && (fabsf(nearDistance) < 0.00000011920929f || nearDistance < 0.0f)) {
            for (index = 0; index < 2; ++index)
                FTBL.srchSetTargetEnabled(SHEN_NAME4('E', 'V', 'B', '\0') + (index << 24), 1);
        } else {
            for (index = 0; index < 2; ++index)
                FTBL.srchSetTargetEnabled(SHEN_NAME4('E', 'V', 'B', '\0') + (index << 24), 0);
        }
#else
        float x = right * left;
        float z = nearDistance * farDistance;
        if (fabsf(x) < 0.00000011920929f)
            x = 0.0f;
        if (fabsf(z) < 0.00000011920929f)
            z = 0.0f;
        const bool inside = (fabsf(x) < 0.00000011920929f || x < 0.0f) && (fabsf(z) < 0.00000011920929f || z < 0.0f);
        std::uint64_t result = 0;
        for (const shen_name target : QEHI_SEARCH_TARGETS)
            result = FTBL.srchSetTargetEnabled(target, inside ? 1 : 0);
        return result;
#endif
    }
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition qehiScriptDefinition{.areaId = AREA_QEHI, .field04 = 0, .init = QehiInit, .main = QehiMain};
} // namespace shenmue::s2
#endif
