#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0521_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0521_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0521_aa84{SHEN_NAME(aa84)};
inline constexpr shen_name AREA_0521_aa86{SHEN_NAME(aa86)};
inline constexpr shen_name AREA_0521_CHARACTERS[]{AREA_0521_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0521_MOTION[] = "0521A.MOT", AREA_0521_CONVERSATION[] = "0521";
inline constexpr const char *AREA_0521_DEBUG_NAMES[]{"0521A010.c\n", "0521A020.c\n", "0521A040.c\n", "0521A050.c\n", "0521A080.c\n", "0521A030.c\n"};
void Setup0521();
void Initialize0521();
void Start0521(std::uintptr_t argument);
void Update0521(void *taskData);
void Cleanup0521(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0521Author(AuthorViewer519ELoopTaskParameter *child);
void Update0521AuthorTask(void *taskData);
void Prepare0521Characters();
void Restore0521Characters();
extern const ScriptAreaDefinition area0521ScriptDefinition;

} // namespace shenmue::s2::areas
