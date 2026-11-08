#include "s2/areas/area_wk00.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/pati.hpp"
#include <cstddef>
namespace shenmue::s2::areas {
namespace {
void WK00NativeNullsub(void *) {
}
struct WK00LoopWork {
    std::uint64_t first;
    HLib::TaskCallback callback;
    std::uint64_t second;
    std::uint64_t third;
    std::uint32_t fourth;
    std::uint32_t reserved24;
};
static_assert(sizeof(WK00LoopWork) == 40);
static_assert(offsetof(WK00LoopWork, fourth) == 32);
void ApplyLighting(std::int32_t night) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(5, 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(5, 0);
    }
    EV_LoadAudio("WK00_000", night ? 1 : 0);
}
} // namespace
void AreaWK00Init() {
}
void AreaWK00Main(std::uint32_t) {
    auto *task = HLib::EnqueueTaskWithParameter(WK00NativeNullsub, 4, 0x0b, 40, SHEN_NAME(EVNT));
    auto *work = static_cast<WK00LoopWork *>(HLib::GetTaskParameter(task));
    work->first = 0;
    work->second = 0;
    work->third = 0;
    work->fourth = 0;
    work->callback = WK00NativeNullsub;
    ApplyLighting(EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED));
    InitPati();
    flddSetPointFlagOnGrid(SHEN_NAME(BTC1), 0);
}
void AreaWK00Cleanup(std::int32_t night, std::uint32_t) {
    ApplyLighting(night);
}
const GameplayAreaDefinition areaWK00GameplayDefinition{SHEN_NAME(WK00), 0, AreaWK00Init, AreaWK00Main, AreaWK00Cleanup};
} // namespace shenmue::s2::areas
