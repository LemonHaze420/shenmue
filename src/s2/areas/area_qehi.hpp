#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name QEHI_TASK{SHEN_NAME(EVNT)}, QEHI_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name QEHI_SEARCH_TARGETS[]{shen_name{make_shen_name('E', 'V', 'B', '\0')}, shen_name{make_shen_name('E', 'V', 'B', '\1')}};
void QehiInit();
void QehiMain(std::uintptr_t argument);
void QehiMainLoopCallback(void *taskData);
std::uint64_t UpdateQehiSearchTargets();
extern const ScriptAreaDefinition qehiScriptDefinition;

} // namespace shenmue::s2
