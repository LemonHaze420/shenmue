#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0507_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0507_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0507_a350{SHEN_NAME(a350)};
inline constexpr shen_name AREA_0507_CHARACTERS[]{AREA_0507_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0507_MOTION[] = "0507A.MOT", AREA_0507_CONVERSATION[] = "0507";
inline constexpr const char *AREA_0507_DEBUG_NAMES[]{"0507A000.c\n"};
void Setup0507();
inline constexpr shen_name AREA_0507_WAVE_TASK{SHEN_NAME(EVNT)};
void Wave0507Resources(void *taskData);
void Initialize0507();
void Start0507(std::uintptr_t argument);
void Update0507(void *taskData);
void Cleanup0507(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0507Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0507Author(AuthorViewer519ELoopTaskParameter *child);
void Update0507AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0507ScriptDefinition;

} // namespace shenmue::s2::areas
