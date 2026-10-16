#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0541_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0541_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0541_a636{SHEN_NAME(a636)};
inline constexpr shen_name AREA_0541_CHARACTERS[]{AREA_0541_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0541_MOTION[] = "0541A.MOT", AREA_0541_CONVERSATION[] = "0541";
inline constexpr const char *AREA_0541_DEBUG_NAMES[]{"0541A000.c\n", "0541E010.c\n"};
inline constexpr char AREA_0541_AUDIO0[] = "0541A000", AREA_0541_AUDIO1[] = "KWW4";
void Initialize0541();
void Start0541(std::uintptr_t argument);
void Update0541(void *taskData);
void Cleanup0541(AuthorViewerTaskParameter *parent);
std::int64_t Queue0541Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0541Author(AuthorViewerLoopTaskParameter *child);
void Update0541AuthorTask(void *taskData);
void Prepare0541Characters();
extern const ScriptAreaDefinition area0541ScriptDefinition;

} // namespace shenmue::s2::areas
