#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name KEST_TASK{SHEN_NAME(kesT)};
inline constexpr shen_name KEST_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name KEST_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name KEST_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name KEST_SHENHUA{SHEN_NAME(SIN_)};
inline constexpr shen_name KEST_DUMMY{SHEN_NAME(DUMY)};
inline constexpr shen_name KEST_INVALID{0xffffffffU};
inline constexpr shen_name KEST_LIGHT_DEFAULT{SHEN_NAME(0000)};
inline constexpr shen_name KEST_LIGHT_SPECIAL{SHEN_NAME(0002)};
void AreaKESTInit();
void AreaKESTMain(std::uintptr_t entry);
void KESTSetupCallback(void *data);
void KESTQuitCameraDepthCallback(void *data);
void KESTWaitCallback(void *data);
void KESTReleaseCallback(void *data);
extern const ScriptAreaDefinition areaKESTScriptDefinition;
} // namespace shenmue::s2::areas
