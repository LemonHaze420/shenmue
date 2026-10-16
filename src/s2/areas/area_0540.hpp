#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0540_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0540_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0540_CHARACTERS[]{AREA_0540_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0540_MOTION[] = "0540A.MOT", AREA_0540_CONVERSATION[] = "0540";
inline constexpr const char *AREA_0540_DEBUG_NAMES[]{"0540A000.c\n", "0540A020.c\n", "0540A040.c\n", "0540A050.c\n"};
inline constexpr char AREA_0540_BLUR_MODEL[] = "RYO_B.MT7";
void Initialize0540();
void Start0540(std::uintptr_t argument);
void Update0540(void *taskData);
void Cleanup0540(AuthorViewerTaskParameter *parent);
std::int64_t Queue0540Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0540Author(AuthorViewerLoopTaskParameter *child);
void Update0540AuthorTask(void *taskData);
void Prepare0540Characters();
extern const ScriptAreaDefinition area0540ScriptDefinition;

} // namespace shenmue::s2::areas
