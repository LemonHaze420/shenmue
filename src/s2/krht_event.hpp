#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2 {

inline constexpr shen_name KRHT_AREA{SHEN_NAME(KRHT)}, KRHT_TASK{SHEN_NAME(yuku)}, KRHT_SOURCE_AUTHOR{SHEN_NAME(KRZ1)};
inline constexpr shen_name KRHT_TARGET_AUTHOR{SHEN_NAME(AT01)}, KRHT_TARGET_AREA{SHEN_NAME(0523)};
inline constexpr char KRHT_MEDIA[] = "KRH1";
void KRHTInit();
void KRHTMain(std::uintptr_t argument);
void KRHTStartAuthor(void *taskData);
void KRHTWaitAuthor(void *taskData);
extern const ScriptAreaDefinition krhtScriptDefinition;

} // namespace shenmue::s2
