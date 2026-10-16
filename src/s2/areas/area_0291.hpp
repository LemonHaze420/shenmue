#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0291_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0291_ab16{SHEN_NAME(ab16)};
inline constexpr shen_name AREA_0291_a392{SHEN_NAME(a392)};
inline constexpr shen_name AREA_0291_a452{SHEN_NAME(a452)};
inline constexpr shen_name AREA_0291_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0291_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_0291_CHARACTERS[]{AREA_0291_REN_, AREA_0291_YUA_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0291_MOTION[] = "0291A.MOT", AREA_0291_CONVERSATION[] = "0291";
inline constexpr const char *AREA_0291_DEBUG_NAMES[]{"0291A000.c\n"};
inline constexpr Vector3 AREA_0291_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x4249999aU), .z = 0.0f};
void Setup0291();
void Initialize0291();
void Start0291(std::uintptr_t argument);
void Update0291(void *taskData);
void Cleanup0291(AuthorViewerTaskParameter *parent);
std::int64_t Queue0291Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0291Author(AuthorViewerLoopTaskParameter *child);
void Update0291AuthorTask(void *taskData);
void Prepare0291Characters();
void Restore0291Characters();
extern const ScriptAreaDefinition area0291ScriptDefinition;

} // namespace shenmue::s2::areas
