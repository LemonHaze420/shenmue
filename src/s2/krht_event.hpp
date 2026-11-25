#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
typedef unsigned int KrhtStartParameters;
#define KRHT_TASK_TOKEN KRHT_TASK
#define KRHT_TASK SHEN_NAME4('y', 'u', 'k', 'u')
#define KRHT_SOURCE_AUTHOR SHEN_NAME4('K', 'R', 'Z', '1')
#define KRHT_TARGET_AUTHOR_COMMAND KRHT_TARGET_AUTHOR
#define KRHT_TARGET_AUTHOR SHEN_NAME4('A', 'T', '0', '1')
#define KRHT_TARGET_AREA SHEN_NAME4('0', '5', '2', '3')
#define KRHT_MEDIA "KRH1"
void KRHTMain(unsigned int argument);
void KRHTStartAuthor(void *taskData);
void KRHTWaitAuthor(void *taskData);
#else
#include "s2/area_event.hpp"

namespace shenmue::s2 {

inline constexpr shen_name KRHT_AREA{SHEN_NAME(KRHT)}, KRHT_TASK{SHEN_NAME(yuku)}, KRHT_SOURCE_AUTHOR{SHEN_NAME(KRZ1)};
inline constexpr shen_name KRHT_TARGET_AUTHOR{SHEN_NAME(AT01)}, KRHT_TARGET_AREA{SHEN_NAME(0523)};
inline constexpr char KRHT_MEDIA[] = "KRH1";
using KrhtStartParameters = std::uintptr_t;
inline constexpr std::int32_t KRHT_TASK_TOKEN = static_cast<std::int32_t>(KRHT_TASK.name);
inline constexpr std::int32_t KRHT_TARGET_AUTHOR_COMMAND = static_cast<std::int32_t>(KRHT_TARGET_AUTHOR.name);
void KRHTInit();
void KRHTMain(std::uintptr_t argument);
void KRHTStartAuthor(void *taskData);
void KRHTWaitAuthor(void *taskData);
extern const ScriptAreaDefinition krhtScriptDefinition;

} // namespace shenmue::s2
#endif
