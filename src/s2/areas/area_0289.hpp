#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0289_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0289_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0289_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0289_a942{SHEN_NAME(a942)};
inline constexpr shen_name AREA_0289_CHARACTERS[]{AREA_0289_REN_, AREA_0289_TGY_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0289_MOTION[] = "0289A.MOT", AREA_0289_CONVERSATION[] = "0289";
inline constexpr const char *AREA_0289_DEBUG_NAMES[]{"0289A000.c\n", "0289A005.c\n", "0289A020.c\n"};
inline constexpr Vector3 AREA_0289_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x423b3333U), .z = 0.0f};
void Setup0289();
void Initialize0289();
void Start0289(std::uintptr_t argument);
void Update0289(void *taskData);
void Cleanup0289(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue0289Author(AuthorViewerPairTaskParameter *parent);
std::int64_t Update0289Author(AuthorViewer519ELoopTaskParameter *child);
void Update0289AuthorTask(void *taskData);
void Prepare0289Characters();
void Restore0289Characters();
extern const ScriptAreaDefinition area0289ScriptDefinition;

} // namespace shenmue::s2::areas
