#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0543_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0543_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0543_CHARACTERS[]{AREA_0543_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0543_MOTION[] = "0543A.MOT", AREA_0543_CONVERSATION[] = "0543";
inline constexpr const char *AREA_0543_DEBUG_NAMES[]{"0543A000.c\n", "0543A010.c\n"};
void Initialize0543();
void Start0543(std::uintptr_t argument);
void Update0543(void *taskData);
void Cleanup0543(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0543Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0543Author(AuthorViewer519ELoopTaskParameter *child);
void Update0543AuthorTask(void *taskData);
void Prepare0543Characters();
void Restore0543Characters();
extern const ScriptAreaDefinition area0543ScriptDefinition;

} // namespace shenmue::s2::areas
