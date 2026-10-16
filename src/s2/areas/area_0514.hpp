#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0514_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0514_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0514_aa85{SHEN_NAME(aa85)};
inline constexpr shen_name AREA_0514_a287{SHEN_NAME(a287)};
inline constexpr shen_name AREA_0514_a288{SHEN_NAME(a288)};
inline constexpr shen_name AREA_0514_CHARACTERS[]{AREA_0514_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0514_MOTION[] = "0514A.MOT", AREA_0514_CONVERSATION[] = "0514";
inline constexpr const char *AREA_0514_DEBUG_NAMES[]{"0514A000.c\n", "0514A010.c\n", "0514A020.c\n"};
inline constexpr shen_name AREA_0514_LIGHTS[]{SHEN_NAME(l001), SHEN_NAME(l101), SHEN_NAME(l201)};
void Setup0514Scene();
void Create0514FireEffects(std::int32_t selection);
void Update0514FireLighting();
std::int64_t Update0514SecondAuthor(AuthorViewer517ELoopTaskParameter *child);
void Update0514SecondAuthorTask(void *);
void Initialize0514();
void Start0514(std::uintptr_t argument);
void Update0514(void *taskData);
void Cleanup0514(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0514Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0514Author(AuthorViewer517ELoopTaskParameter *child);
void Update0514AuthorTask(void *taskData);
void Prepare0514Characters();
void Restore0514Characters();
extern const ScriptAreaDefinition area0514ScriptDefinition;

} // namespace shenmue::s2::areas
