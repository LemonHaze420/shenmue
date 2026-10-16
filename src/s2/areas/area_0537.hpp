#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0537_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0537_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0537_aa44{SHEN_NAME(aa44)};
inline constexpr shen_name AREA_0537_aa45{SHEN_NAME(aa45)};
inline constexpr shen_name AREA_0537_a726{SHEN_NAME(a726)};
inline constexpr shen_name AREA_0537_a728{SHEN_NAME(a728)};
inline constexpr shen_name AREA_0537_aa62{SHEN_NAME(aa62)};
inline constexpr shen_name AREA_0537_aa63{SHEN_NAME(aa63)};
inline constexpr shen_name AREA_0537_CHARACTERS[]{AREA_0537_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0537_MOTION[] = "0537A.MOT", AREA_0537_CONVERSATION[] = "0537";
inline constexpr const char *AREA_0537_DEBUG_NAMES[]{"0537A000.c\n", "0537A010.c\n"};
void Setup0537();
inline constexpr shen_name AREA_0537_LIGHT0{SHEN_NAME(l001)}, AREA_0537_LIGHT1{SHEN_NAME(l101)};
void Initialize0537();
void Start0537(std::uintptr_t argument);
void Update0537(void *taskData);
void Cleanup0537(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0537Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0537Author(AuthorViewer517ELoopTaskParameter *child);
void Update0537AuthorTask(void *taskData);
void Prepare0537Characters();
void Restore0537Characters();
extern const ScriptAreaDefinition area0537ScriptDefinition;

} // namespace shenmue::s2::areas
