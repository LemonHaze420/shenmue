#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_utility_dc.h"
#define AREA_QDFO SHEN_NAME4('Q', 'D', 'F', 'O')
#define QDFO_TASK TASK_EVNT
#define QDFO_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define QDFO_REN SHEN_NAME4('R', 'E', 'N', '_')
#define QDFO_BACKGROUND (0x34353061U | 0x400U)
#define QDFO_RETURN_AREA SHEN_NAME4('Q', 'D', 'K', 'J')
typedef struct QdfoEventWork {
    HLibTask *task;
    int state;
} QdfoEventWork;
extern const shen_name QDFO_CHARACTERS[];
void QdfoMain(unsigned int argument);
void QdfoTaskCallback(void *taskData);
HLibTask *InitializeQdfoCharacters(HLibTask *task);
void HideAuthorCharacter(shen_name character);
void HideAuthorProp(shen_name prop);
#else

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name QDFO_TASK{SHEN_NAME(EVNT)}, QDFO_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name QDFO_REN{SHEN_NAME(REN_)}, QDFO_BACKGROUND{0x34353061U | 0x400U}, QDFO_RETURN_AREA{SHEN_NAME(QDKJ)};
inline constexpr shen_name QDFO_CHARACTERS[]{QDFO_REN, shen_name{0xffffffffU}};
struct QdfoEventWork {
    HLib::Task *task;
    std::int32_t state;
    std::uint32_t reserved0c;
};
static_assert(sizeof(QdfoEventWork) == 0x10);
static_assert(offsetof(QdfoEventWork, state) == 8);
void QdfoInit();
void QdfoMain(std::uintptr_t argument);
void QdfoTaskCallback(void *taskData);
HLib::Task *InitializeQdfoCharacters(HLib::Task *task);
extern const ScriptAreaDefinition qdfoScriptDefinition;

} // namespace shenmue::s2

#endif
