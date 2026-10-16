#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name KEBI_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name KEBI_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name KEBI_GUARD{SHEN_NAME(DEY_)};
inline constexpr shen_name KEBI_MOTION_MAP{SHEN_NAME(AR02)};
inline constexpr std::uint16_t KEBI_DISMISS_MOTION = 0x1942;
inline constexpr std::uint16_t KEBI_STOP_MOTION = 0x1943;
inline constexpr std::uint16_t KEBI_IDLE_MOTION = 0x1945;
struct KebiTaskData {
    HLib::Task *reservedTask;
    HLib::Task *self;
    std::int32_t motionBank;
    std::uint32_t frame;
    std::uint64_t reserved18;
};
static_assert(sizeof(KebiTaskData) == 32);
static_assert(offsetof(KebiTaskData, frame) == 20);
void AreaKEBIInit();
void AreaKEBIMain(std::uintptr_t parameter);
void KEBICheckCallback(void *data);
void KEBIMainCallback(void *data);
void KEBIExitCallback(void *data);
extern const ScriptAreaDefinition areaKEBIScriptDefinition;
} // namespace shenmue::s2::areas
