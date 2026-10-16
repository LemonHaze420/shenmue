#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0518_LIGHT{SHEN_NAME(l180)};
inline constexpr shen_name AREA_0518_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0518_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0518_a435{SHEN_NAME(a435)};
inline constexpr shen_name AREA_0518_a016{SHEN_NAME(a016)};
inline constexpr shen_name AREA_0518_a466{SHEN_NAME(a466)};
inline constexpr shen_name AREA_0518_a468{SHEN_NAME(a468)};
inline constexpr shen_name AREA_0518_a470{SHEN_NAME(a470)};
inline constexpr shen_name AREA_0518_CHARACTERS[]{AREA_0518_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0518_MOTION[] = "0518A.MOT", AREA_0518_CONVERSATION[] = "0518";
inline constexpr const char *AREA_0518_DEBUG_NAMES[]{"0518A000.c\n"};
void Initialize0518();
void Start0518(std::uintptr_t argument);
void Update0518(void *taskData);
void Cleanup0518(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0518Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0518Author(AuthorViewer517ELoopTaskParameter *child);
void Update0518AuthorTask(void *taskData);
void Prepare0518Characters();
void Restore0518Characters();
extern const ScriptAreaDefinition area0518ScriptDefinition;

} // namespace shenmue::s2::areas
