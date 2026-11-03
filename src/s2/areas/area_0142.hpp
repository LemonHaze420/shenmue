#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_sleep_dc.h"

#define AREA_0142_KUN_ SHEN_NAME4('K', 'U', 'N', '_')
#define AREA_0142_A540 SHEN_NAME4('a', '5', '4', '0')
#define AREA_0142_LIGHT SHEN_NAME4('l', '0', '0', '1')
#define AREA_0142_NEXT_MAP SHEN_NAME4('0', '2', '0', '0')
typedef struct E0142TaskParameter {
    HLibTask *self;
    short selection, state;
} E0142TaskParameter;
typedef struct E0142LoopParameter {
    E0142TaskParameter *parent;
    short frame;
} E0142LoopParameter;
extern shen_name area0142Characters[];
void E0142Main(void);
void E0142MainCallback(void *taskData);
void QueueE0142Loop(E0142TaskParameter *parent);
void E0142LoopCallback(void *taskData);
void SetupE0142Environment(void);
void CleanupE0142(E0142TaskParameter *parent);
void SetupE0142Characters(void);
void RestoreE0142Characters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0142_AREA{SHEN_NAME(0142)};
inline constexpr shen_name AREA_0142_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0142_A540{SHEN_NAME(a540)};
inline constexpr shen_name AREA_0142_CHARACTERS[]{AREA_0142_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0142_CONVERSATION[] = "0142";

struct E0142TaskParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
static_assert(sizeof(E0142TaskParameter) == 0x10);
struct E0142LoopParameter {
    E0142TaskParameter *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(E0142LoopParameter) == 0x10);
inline constexpr shen_name AREA_0142_LIGHT{SHEN_NAME(l001)}, AREA_0142_NEXT_MAP{SHEN_NAME(0200)};
extern const ScriptAreaDefinition area0142ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
