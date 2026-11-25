#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_552E_RYO_ RYO_CHARACTER
#define AREA_552E_SIN_ SHEN_NAME4('S', 'I', 'N', '_')
#define AREA_552E_ab02 SHEN_NAME4('a', 'b', '0', '2')
#define AREA_552E_ab03 SHEN_NAME4('a', 'b', '0', '3')
#define AREA_552E_TITLE SHEN_NAME4('R', 'T', 'R', '1')
#define AREA_552E_CONVERSATION "0552"
#define AUTHOR_VIEWER_TASK TASK_EVNT
#define AUTHOR_VIEWER_INVALID_CHARACTER INVALID_CHARACTER
typedef struct AuthorViewerPairTaskParameter {
    HLibTask *self;
    unsigned int motion, selection;
    short state;
} AuthorViewerPairTaskParameter;
typedef struct AuthorViewer519ELoopTaskParameter {
    AuthorViewerPairTaskParameter *parent;
} AuthorViewer519ELoopTaskParameter;
extern shen_name AREA_552E_CHARACTERS[];
void Start552E(void);
void Update552E(void *data);
void Update552EAuthorTask(void *data);
void Cleanup552E(void);
void Exit552EB(void *data);
void Exit552EC(void *data);
void Prepare552ECharacters(void);
void Restore552ECharacters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_552E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_552E_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_552E_ab02{SHEN_NAME(ab02)};
inline constexpr shen_name AREA_552E_ab03{SHEN_NAME(ab03)};
inline constexpr shen_name AREA_552E_CHARACTERS[]{AREA_552E_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_552E_CONVERSATION[] = "0552";

inline constexpr shen_name AREA_552E_TITLE{SHEN_NAME(RTR1)};
inline constexpr shen_name AREA_552E_FOREST1{SHEN_NAME(KMZ1)}, AREA_552E_FOREST2{SHEN_NAME(KMZ2)};
inline constexpr shen_name AREA_552E_FOREST3{SHEN_NAME(KMZ3)}, AREA_552E_FOREST4{SHEN_NAME(KMZ4)};
void Exit552EB(void *taskData);
void Exit552EC(void *taskData);
void Initialize552E();
void Start552E(std::uintptr_t argument);
void Update552E(void *taskData);
void Cleanup552E();
std::int64_t Update552EAuthor(AuthorViewer519ELoopTaskParameter *child);
void Update552EAuthorTask(void *taskData);
void Prepare552ECharacters();
void Restore552ECharacters();
extern const ScriptAreaDefinition area552EScriptDefinition;

} // namespace shenmue::s2::areas
#endif
