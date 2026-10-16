#include "s2/areas/area_kwwq.hpp"

#include "s2/event_runtime.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"

#include <cstdint>

namespace shenmue::s2::areas {
namespace {

// @verified - p
void KWWQMainLoopCallback(void *) {
}

} // namespace

// @verified - p
void AreaKWWQInit() {
}

// @verified - p
void AreaKWWQMain(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(KWWQMainLoopCallback, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaKWWQCleanup(std::int32_t, std::uint32_t) {
    (void)CheckIsArgEquZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
}

const GameplayAreaDefinition areaKWWQGameplayDefinition{
    .areaId = AREA_KWWQ,
    .padding04 = 0,
    .init = AreaKWWQInit,
    .main = AreaKWWQMain,
    .cleanup = AreaKWWQCleanup,
};

} // namespace shenmue::s2::areas
