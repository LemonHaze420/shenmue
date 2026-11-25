#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AUTHOR_VIEWER_TASK TASK_EVNT
#define AUTHOR_VIEWER_INVALID_CHARACTER INVALID_CHARACTER
#define AREA_0361_RYO_ RYO_CHARACTER
#define AREA_0361_REN_ SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0361_AUTHOR SHEN_NAME4('0', '3', '6', '1')
#define AREA_0361_RETURN SHEN_NAME4('Q', 'F', '0', '0')
#define AREA_0361_CONVERSATION "0361"
#define AREA_0361_ARCHIVE "0361A"
typedef AreaImmediateAuthorTaskParameter Area0361Parameter;
typedef struct Area0361AuthorParameter {
    Area0361Parameter *parent;
} Area0361AuthorParameter;
extern shen_name AREA_0361_CHARACTERS[];
void Start0361(void);
void Update0361(void *);
void Queue0361Author(Area0361Parameter *);
void Update0361Author(void *);
void Setup0361Scene(void);
void Cleanup0361(Area0361Parameter *);
void Prepare0361Characters(void);
void Restore0361Characters(void);
void Prepare0361Character(shen_name);
void Restore0361Character(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0361_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0361_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0361_CHARACTERS[]{AREA_0361_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0361_CONVERSATION[] = "0361";
inline constexpr shen_name AREA_0361_RETURN{SHEN_NAME(QF00)};
inline constexpr shen_name AREA_0361_AUTHOR{SHEN_NAME(0361)};
struct Area0361Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0361AuthorParameter {
    Area0361Parameter *parent;
};
static_assert(sizeof(Area0361Parameter) == 16);
static_assert(offsetof(Area0361Parameter, selection) == 8 && offsetof(Area0361Parameter, state) == 10);
static_assert(sizeof(Area0361AuthorParameter) == 8);
inline constexpr char AREA_0361_ARCHIVE[] = "0361A";
void Initialize0361();
void Start0361(std::uintptr_t argument);
void Update0361(void *taskData);
void Cleanup0361(Area0361Parameter *parent);
std::int64_t Queue0361Author(Area0361Parameter *parent);
std::int64_t Update0361Author(Area0361AuthorParameter *child);
void Update0361AuthorTask(void *taskData);
void Setup0361Scene();
extern const ScriptAreaDefinition area0361ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
