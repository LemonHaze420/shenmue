#pragma once
#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name WKTA_EVENT{SHEN_NAME(EVNT)}, WKTA_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name WKTA_KKI{SHEN_NAME(KKI_)}, WKTA_DOOR_BASE{0x5f414f44U}, WKTA_PLAYER_BASE{0x594d4144U};
inline constexpr shen_name WKTA_DOOR{842084690U};
struct WktaWork {
    HLib::Task *self;
    std::int32_t phase;
    std::uint32_t savedAnimation;
    Vector3 originalPosition;
    std::byte reserved1c[0x24];
    std::int32_t response;
    std::uint32_t reserved44;
};
static_assert(sizeof(WktaWork) == 0x48);
static_assert(offsetof(WktaWork, originalPosition) == 0x10);
static_assert(offsetof(WktaWork, response) == 0x40);
void WktaInit();
void WktaMain(std::uintptr_t argument);
void WktaOpeningCallback(void *data);
void WktaClosingCallback(void *data);
void FinishWkta(WktaWork *work);
extern const ScriptAreaDefinition wktaScriptDefinition;
} // namespace shenmue::s2
