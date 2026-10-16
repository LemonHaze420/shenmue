#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0525_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0525_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0525_CHARACTERS[]{AREA_0525_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0525_MOTION[] = "0525A.MOT", AREA_0525_CONVERSATION[] = "0525";
inline constexpr const char *AREA_0525_DEBUG_NAMES[]{"0525A000.c\n", "0525A010.c\n", "0525A020.c\n", "0525A030.c\n"};
void Setup0525();
inline constexpr shen_name AREA_0525_LIGHT{SHEN_NAME(l250)};
inline constexpr char AREA_0525_AUDIO[] = "0525A000";
void Initialize0525();
void Start0525(std::uintptr_t argument);
void Update0525(void *taskData);
void Cleanup0525(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0525Author(AuthorViewerPairTaskParameter *parent);
extern const ScriptAreaDefinition area0525ScriptDefinition;

} // namespace shenmue::s2::areas
