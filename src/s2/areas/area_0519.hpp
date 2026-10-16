#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0519_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0519_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0519_CHARACTERS[]{AREA_0519_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0519_MOTION[] = "0519A.MOT", AREA_0519_CONVERSATION[] = "0519";

void Setup0519Package(std::int32_t selection);
void Update0519Waves(void *);
void Create0519Splash(std::int32_t selection);
std::int64_t Update0519Splash();
void Update0519SplashTask(void *);
void Initialize0519();
void Start0519(std::uintptr_t argument);
void Update0519(void *taskData);
void Cleanup0519(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0519Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0519Author(AuthorViewer517ELoopTaskParameter *child);
void Update0519AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0519ScriptDefinition;

} // namespace shenmue::s2::areas
