#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AUTHOR_VIEWER_TASK TASK_EVNT
#define AUTHOR_VIEWER_INVALID_CHARACTER INVALID_CHARACTER
#define AREA_0297_RYO_ RYO_CHARACTER
#define AREA_0297_REN_ SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0297_CHD_ SHEN_NAME4('C', 'H', 'D', '_')
#define AREA_0297 SHEN_NAME4('0', '2', '9', '7')
#define AREA_0297_AUTHOR AREA_0297
#define AREA_0297_RETURN SHEN_NAME4('Q', 'F', '0', '2')
#define AREA_0297_CONVERSATION "0297"
#define AREA_0297_ARCHIVE "0297A"
typedef AreaAuthorTaskParameter AuthorViewerTaskParameter;
typedef struct AuthorViewerLoopTaskParameter {
    AuthorViewerTaskParameter *parent;
} AuthorViewerLoopTaskParameter;
extern shen_name AREA_0297_CHARACTERS[];
void Start0297(void);
void Update0297(void *);
void Queue0297Author(AuthorViewerTaskParameter *);
void Update0297Author(void *);
void Setup0297Scene(void);
void Cleanup0297(AuthorViewerTaskParameter *);
void Prepare0297Characters(void);
void Restore0297Characters(void);
void Prepare0297Character(shen_name);
void Restore0297Character(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0297_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0297_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0297_CHD_{SHEN_NAME(CHD_)};
inline constexpr shen_name AREA_0297_CHARACTERS[]{AREA_0297_REN_, AREA_0297_CHD_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0297_CONVERSATION[] = "0297";
inline constexpr shen_name AREA_0297_RETURN{SHEN_NAME(QF02)};
inline constexpr shen_name AREA_0297_AUTHOR{SHEN_NAME(0297)};
void Initialize0297();
void Start0297(std::uintptr_t argument);
void Update0297(void *taskData);
void Cleanup0297(AuthorViewerTaskParameter *parent);
std::int64_t Queue0297Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0297Author(AuthorViewerLoopTaskParameter *child);
void Update0297AuthorTask(void *taskData);
void Prepare0297Characters();
void Restore0297Characters();
void Setup0297Scene();
extern const ScriptAreaDefinition area0297ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
