#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"

#define AREA_0273_JOP SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0273_SOURCE SHEN_NAME4('Q', 'F', '4', '0')
#define AREA_0273_DESTINATION SHEN_NAME4('Q', 'F', 'R', 'R')
#define AREA_0273_AUTHOR SHEN_NAME4('0', '2', '7', '3')
extern const shen_name area0273Characters[];
typedef struct Area0273Parameter {
    HLibTask *task;
    short author, state;
} Area0273Parameter;
typedef struct Area0273AuthorParameter {
    Area0273Parameter *parent;
} Area0273AuthorParameter;
void Start0273(void);
void Update0273(void *taskData);
void Queue0273Author(Area0273Parameter *parent);
void Update0273Author(void *taskData);
void Setup0273(void);
void Cleanup0273(Area0273Parameter *parent);
void Prepare0273Characters(void);
void Restore0273Characters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0273_JOP{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0273_SOURCE{SHEN_NAME(QF40)}, AREA_0273_DESTINATION{SHEN_NAME(QFRR)};
inline constexpr shen_name AREA_0273_CHARACTERS[]{AREA_0273_JOP, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0273_CONVERSATION[] = "0273", AREA_0273_SOUND[] = "0273A000", AREA_0273_ARCHIVE[] = "0273A";
struct Area0273Parameter {
    HLib::Task *task;
    std::int16_t author;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0273AuthorParameter {
    Area0273Parameter *parent;
};
static_assert(sizeof(Area0273Parameter) == 0x10 && offsetof(Area0273Parameter, author) == 8 && offsetof(Area0273Parameter, state) == 10);
static_assert(sizeof(Area0273AuthorParameter) == 8);
void Initialize0273();
void Start0273(std::uintptr_t argument);
void Update0273(void *taskData);
void Cleanup0273(Area0273Parameter *parent);
std::int64_t Queue0273Author(Area0273Parameter *parent);
std::int64_t Update0273Author(Area0273AuthorParameter *child);
void Update0273AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0273ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
