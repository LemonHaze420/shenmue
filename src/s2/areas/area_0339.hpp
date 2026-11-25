#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AUTHOR_VIEWER_TASK TASK_EVNT
#define AUTHOR_VIEWER_INVALID_CHARACTER INVALID_CHARACTER
#define AREA_0339_RYO_ RYO_CHARACTER
#define AREA_0339_REN_ SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0339_JOP_ SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0339_a396 SHEN_NAME4('a', '3', '9', '6')
#define AREA_0339 SHEN_NAME4('0', '3', '3', '9')
#define AREA_0339_AUTHOR AREA_0339
#define AREA_0339_RETURN SHEN_NAME4('Q', 'R', '0', '0')
#define AREA_0339_NPC_AREA SHEN_NAME4('A', 'N', 'P', '1')
#define AREA_0339_CONVERSATION "0339"
typedef AreaAuthorTaskParameter AuthorViewerTaskParameter;
typedef struct AuthorViewerLoopTaskParameter {
    AuthorViewerTaskParameter *parent;
} AuthorViewerLoopTaskParameter;
extern shen_name AREA_0339_CHARACTERS[];
void Start0339(void);
void Update0339(void *);
void Queue0339Author(AuthorViewerTaskParameter *);
void Update0339Author(void *);
void Setup0339Scene(void);
void Cleanup0339(AuthorViewerTaskParameter *);
void Prepare0339Characters(void);
void Restore0339Characters(void);
void Prepare0339Character(shen_name);
void Restore0339Character(shen_name);
void Hide0339Prop(shen_name);
void Show0339Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0339_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0339_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0339_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0339_a396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0339_CHARACTERS[]{AREA_0339_JOP_, AREA_0339_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0339_CONVERSATION[] = "0339";
inline constexpr shen_name AREA_0339_RETURN{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_0339_NPC_AREA{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0339_AUTHOR{SHEN_NAME(0339)};
void Initialize0339();
void Start0339(std::uintptr_t argument);
void Update0339(void *taskData);
void Cleanup0339(AuthorViewerTaskParameter *parent);
std::int64_t Queue0339Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0339Author(AuthorViewerLoopTaskParameter *child);
void Update0339AuthorTask(void *taskData);
void Prepare0339Characters();
void Restore0339Characters();
void Setup0339Scene();
extern const ScriptAreaDefinition area0339ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
