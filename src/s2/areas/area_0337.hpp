#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_utility_dc.h"

#define AREA_0337_JOP SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0337_a516 SHEN_NAME4('a', '5', '1', '6')
#define AREA_0337_a512 SHEN_NAME4('a', '5', '1', '2')
#define AREA_0337_a452 SHEN_NAME4('a', '4', '5', '2')
#define AREA_0337_RETURN SHEN_NAME4('Q', 'F', '3', '9')
#define AREA_0337_AUTHOR SHEN_NAME4('0', '3', '3', '7')
#define AREA_0337_ELEVATOR SHEN_NAME4('R', '5', '9', '8')
#define AREA_0337_LIGHT SHEN_NAME4('l', '1', '0', '1')
extern shen_name area0337Characters[];
typedef struct Area0337Parameter {
    HLibTask *self;
    short selection, state;
} Area0337Parameter;
typedef struct Area0337AuthorParameter {
    Area0337Parameter *parent;
} Area0337AuthorParameter;
void Start0337(void);
void Update0337(void *taskData);
void Queue0337Author(Area0337Parameter *parent);
void Update0337AuthorTask(void *taskData);
void Setup0337(void);
void Cleanup0337(Area0337Parameter *parent);
void Prepare0337Characters(void);
void Restore0337Characters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0337_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0337_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0337_ab12{SHEN_NAME(ab12)};
inline constexpr shen_name AREA_0337_ab13{SHEN_NAME(ab13)};
inline constexpr shen_name AREA_0337_a452{SHEN_NAME(a452)};
inline constexpr shen_name AREA_0337_CHARACTERS[]{AREA_0337_JOP_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0337_CONVERSATION[] = "0337";
inline constexpr shen_name AREA_0337_RETURN{SHEN_NAME(QF39)};
inline constexpr shen_name AREA_0337_AUTHOR{SHEN_NAME(0337)};
inline constexpr shen_name AREA_0337_ELEVATOR{SHEN_NAME(R598)};
inline constexpr shen_name AREA_0337_LIGHT{SHEN_NAME(l101)};
struct Area0337Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0337AuthorParameter {
    Area0337Parameter *parent;
};
static_assert(sizeof(Area0337Parameter) == 16);
static_assert(offsetof(Area0337Parameter, selection) == 8 && offsetof(Area0337Parameter, state) == 10);
static_assert(sizeof(Area0337AuthorParameter) == 8);
inline constexpr char AREA_0337_ARCHIVE[] = "0337A";
void Initialize0337();
void Start0337(std::uintptr_t argument);
void Update0337(void *taskData);
void Cleanup0337(Area0337Parameter *parent);
std::int64_t Update0337Author(Area0337AuthorParameter *child);
void Update0337AuthorTask(void *taskData);
void Prepare0337Characters();
void Restore0337Characters();
extern const ScriptAreaDefinition area0337ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
