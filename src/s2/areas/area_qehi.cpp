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

// @verified - p
void QehiMain(std::uintptr_t) {
    EV_SetWeatherInstruction(15, 0);
    (void)SetMapControlStatusFlag(0, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(QehiMainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(QEHI_TASK.name));
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)BeginPrimarySampleExecution(0);
    (void)BeginSecondarySampleExecution(0);
    if (flagRelated(g_currentMap))
        (void)shenUpdatePlayerDataFlags(1);
    if (!shenUpdateFlagEvaluation(1))
        SetPlayerControlEnabled(1);
    if (HasSavedAreaJumpLocation())
        EV_ApplyTranslationClockVisibility(0);
    EVT_QteGameEnter();
    QueueImmediateNextFadeTask(task);
    FinishLoading();
}

// @verified - p
void QehiMainLoopCallback(void *) {
    (void)shenUpdateFlagEvaluation(0);
    (void)UpdateQehiSearchTargets();
}

// @verified - p
std::uint64_t UpdateQehiSearchTargets() {
    Vector3 position;
    EV_GetCharaPosition(QEHI_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    float right = position.x - 306.5f;
    float left = position.x - 302.0f;
    float near = position.z - 77.9f;
    float far = position.z - 80.48f;
    if (std::fabs(right) < 0.00000011920929f)
        right = 0.0f;
    if (std::fabs(left) < 0.00000011920929f)
        left = 0.0f;
    if (std::fabs(near) < 0.00000011920929f)
        near = 0.0f;
    if (std::fabs(far) < 0.00000011920929f)
        far = 0.0f;
    float x = right * left;
    float z = near * far;
    if (std::fabs(x) < 0.00000011920929f)
        x = 0.0f;
    if (std::fabs(z) < 0.00000011920929f)
        z = 0.0f;
    const bool inside = (std::fabs(x) < 0.00000011920929f || x < 0.0f) && (std::fabs(z) < 0.00000011920929f || z < 0.0f);
    std::uint64_t result = 0;
    for (const shen_name target : QEHI_SEARCH_TARGETS)
        result = srchSetTargetEnabled(target, inside ? 1 : 0);
    return result;
}

const ScriptAreaDefinition qehiScriptDefinition{.areaId = AREA_QEHI, .field04 = 0, .init = QehiInit, .main = QehiMain};

} // namespace shenmue::s2
