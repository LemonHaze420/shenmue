#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0524_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0524_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0524_CHARACTERS[]{AREA_0524_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0524_MOTION[] = "0524A.MOT", AREA_0524_CONVERSATION[] = "0524";
inline constexpr const char *AREA_0524_DEBUG_NAMES[]{"0524A000.c\n", "0524A010.c\n"};
void Setup0524();
inline constexpr shen_name AREA_0524_LIGHT{SHEN_NAME(l240)};
inline constexpr char AREA_0524_AUDIO[] = "0524A000";
void Initialize0524();
void Start0524(std::uintptr_t argument);
void Update0524(void *taskData);
void Cleanup0524(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0524Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0524Author(AuthorViewer519ELoopTaskParameter *child);
void Update0524AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0524ScriptDefinition;

} // namespace shenmue::s2::areas
