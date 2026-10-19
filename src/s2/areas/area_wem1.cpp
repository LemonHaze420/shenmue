#include "s2/areas/area_wem1.hpp"
#include "s2/areas/area_dart.hpp"
#include "s2/field_collision.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr EventDartsPlacement placements[]{
    {0, {}, {132.10000610351562f, 100.6500015258789f, 132.6999969482422f}, SHEN_NAME(DAR1), SHEN_NAME(DAR2)},
    {1, {}, {133.10000610351562f, 100.6500015258789f, 132.6999969482422f}, SHEN_NAME(DAR2), SHEN_NAME(DAR2)},
    {1, {}, {132.10000610351562f, 100.6500015258789f, 132.6999969482422f}, SHEN_NAME(DAR1), SHEN_NAME(DAR1)},
};
constexpr EventDartsEnemy enemy{SHEN_NAME(HCY_), 1, 0, 0};
void WEM1Nullsub(void *) {
}
void ApplyWEM1Lighting() {
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
}
} // namespace
void AreaWEM1Init() {
}
void AreaWEM1Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(WEM1Nullsub, 4, 0x0b, 1414420037);
    (void)EV_GetShenmueFlag(0xff);
    ApplyWEM1Lighting();
    if (EV_GetShenmueFlag(0x33f)) {
        EVT_SetEnemyDarts(placements, &enemy);
        EVT_DartsEasySingle(1, placements + 1);
        QueueConditionalNpcLoads(areaWEM1DartsNpcLoads);
    } else {
        EVT_DartsEasySingle(2, placements + 1);
        ToggleFlagOnFieldEntity(AREA_DART, 0);
    }
    SetNewWindPhaseAndSpeed(0, 0.0f);
}
void AreaWEM1Cleanup(std::int32_t, std::uint32_t) {
    ApplyWEM1Lighting();
}
const GameplayAreaDefinition areaWEM1GameplayDefinition{SHEN_NAME(WEM1), 0, AreaWEM1Init, AreaWEM1Main, AreaWEM1Cleanup};
} // namespace shenmue::s2::areas
