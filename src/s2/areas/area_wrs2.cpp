#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_wrs2.hpp"

ConditionalNpcLoadEntry areaWRS2DartsNpcLoads[3] = {
    {SHEN_NAME4('F', 'C', 'J', '_'), {10000.0f, 0.0f, 0.0f}, 0, 0xf002, -1, AreConditionalNpcLoadsPending},
    {SHEN_NAME4('J', 'O', 'N', '_'), {10000.0f, 0.0f, 0.0f}, 0, 0xf002, -1, AreConditionalNpcLoadsPending},
    {0xffffffffU},
};
EventDartsPlacement areaWRS2DartsPlacements[3] = {
    {0, {0, 0, 0}, {173.9f, 101.35f, 156.0f}, SHEN_NAME4('D', 'A', 'R', '1'), SHEN_NAME4('D', 'A', 'R', '2')},
    {1, {0, 0, 0}, {174.9f, 101.35f, 156.0f}, SHEN_NAME4('D', 'A', 'R', '2'), SHEN_NAME4('D', 'A', 'R', '2')},
    {1, {0, 0, 0}, {173.9f, 101.35f, 156.0f}, SHEN_NAME4('D', 'A', 'R', '1'), SHEN_NAME4('D', 'A', 'R', '1')},
};
EventDartsEnemy areaWRS2FirstEnemy = {SHEN_NAME4('F', 'C', 'J', '_'), 0, 0, 0};
EventDartsEnemy areaWRS2SecondEnemy = {SHEN_NAME4('J', 'O', 'N', '_'), 3, 0, 0};

void AreaMain(unsigned int startParameters)
{
    EventDateTime dateTime;
    FTBL.HLib_EnqueueTaskWithoutParameter(WRS2MainLoopNullsub, 4, 11, WRS2_EVENT_TASK);
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 19) <= 5)
        AreaCleanup(1);
    else
        AreaCleanup(0);
    if (FTBL.EV_GetShenmueFlag(0x394)) {
        EVT_SetEnemyDarts(areaWRS2DartsPlacements, &areaWRS2FirstEnemy, 0);
        EVT_DartsEasySingle(areaWRS2DartsPlacements + 1, 1);
        QueueConditionalNpcLoads(areaWRS2DartsNpcLoads);
    } else if (FTBL.EV_GetShenmueFlag(0x33d)) {
        EVT_SetEnemyDarts(areaWRS2DartsPlacements, &areaWRS2SecondEnemy, 0);
        EVT_DartsEasySingle(areaWRS2DartsPlacements + 1, 1);
        QueueConditionalNpcLoads(areaWRS2DartsNpcLoads);
    } else {
        EVT_DartsEasySingle(areaWRS2DartsPlacements + 1, 2);
        FTBL.ToggleFlagOnFieldEntity(WRS2_DARTS_ENTITY, 0);
    }
}

void AreaCleanup(int afternoon)
{
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    AreaWRS2CleanupNullsub(afternoon);
}

void AreaWRS2CleanupNullsub(int afternoon)
{
}

void WRS2MainLoopNullsub(void *taskData)
{
}
#else
#include "s2/areas/area_wrs2.hpp"
#include "s2/areas/area_dart.hpp"
#include "s2/field_collision.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr EventDartsPlacement placements[]{
    {0, {}, {173.89999389648438f, 101.3499984741211f, 156.0f}, SHEN_NAME(DAR1), SHEN_NAME(DAR2)},
    {1, {}, {174.89999389648438f, 101.3499984741211f, 156.0f}, SHEN_NAME(DAR2), SHEN_NAME(DAR2)},
    {1, {}, {173.89999389648438f, 101.3499984741211f, 156.0f}, SHEN_NAME(DAR1), SHEN_NAME(DAR1)},
};
constexpr EventDartsEnemy firstEnemy{SHEN_NAME(FCJ_), 0, 0, 0};
constexpr EventDartsEnemy secondEnemy{SHEN_NAME(JON_), 3, 0, 0};
void WRS2Nullsub(void *) {
}
void ApplyWRS2LightingAndAudio() {
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    EV_LoadCutsceneFromCurrentScene("WRS2");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
        EV_LoadAudio("WRS2_000", 1);
}
} // namespace
void AreaWRS2Init() {
}
void AreaWRS2Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(WRS2Nullsub, 4, 0x0b, 1414420037);
    (void)EV_GetShenmueFlag(0xff);
    ApplyWRS2LightingAndAudio();
    if (EV_GetShenmueFlag(0x394)) {
        EVT_SetEnemyDarts(placements, &firstEnemy);
        EVT_DartsEasySingle(1, placements + 1);
        QueueConditionalNpcLoads(areaWRS2DartsNpcLoads);
    } else if (EV_GetShenmueFlag(0x33d)) {
        EVT_SetEnemyDarts(placements, &secondEnemy);
        EVT_DartsEasySingle(1, placements + 1);
        QueueConditionalNpcLoads(areaWRS2DartsNpcLoads);
    } else {
        EVT_DartsEasySingle(2, placements + 1);
        ToggleFlagOnFieldEntity(AREA_DART, 0);
    }
}
void AreaWRS2Cleanup(std::int32_t, std::uint32_t) {
    ApplyWRS2LightingAndAudio();
}
const GameplayAreaDefinition areaWRS2GameplayDefinition{SHEN_NAME(WRS2), 0, AreaWRS2Init, AreaWRS2Main, AreaWRS2Cleanup};
} // namespace shenmue::s2::areas
#endif
