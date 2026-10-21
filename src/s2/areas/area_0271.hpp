#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0271_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0271_a073{SHEN_NAME(a073)};
inline constexpr shen_name AREA_0271_a392{SHEN_NAME(a392)};
inline constexpr shen_name AREA_0271_a399{SHEN_NAME(a399)};
inline constexpr shen_name AREA_0271_a414{SHEN_NAME(a414)};
inline constexpr shen_name AREA_0271_a512{SHEN_NAME(a512)};
inline constexpr shen_name AREA_0271_a516{SHEN_NAME(a516)};
inline constexpr shen_name AREA_0271_a026{SHEN_NAME(a026)};
inline constexpr shen_name AREA_0271_a554{SHEN_NAME(a554)};
inline constexpr shen_name AREA_0271_a030{SHEN_NAME(a030)};
inline constexpr shen_name AREA_0271_a323{SHEN_NAME(a323)};
inline constexpr shen_name AREA_0271_a148{SHEN_NAME(a148)};
inline constexpr shen_name AREA_0271_a220{SHEN_NAME(a220)};
inline constexpr shen_name AREA_0271_a069{SHEN_NAME(a069)};
inline constexpr shen_name AREA_0271_a070{SHEN_NAME(a070)};
inline constexpr shen_name AREA_0271_a017{SHEN_NAME(a017)};
inline constexpr shen_name AREA_0271_a018{SHEN_NAME(a018)};
inline constexpr shen_name AREA_0271_a058{SHEN_NAME(a058)};
inline constexpr shen_name AREA_0271_a712{SHEN_NAME(a712)};
inline constexpr shen_name AREA_0271_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0271_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_0271_CHARACTERS[]{AREA_0271_REN_, AREA_0271_YUA_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0271_MOTION[] = "0271A.MOT", AREA_0271_CONVERSATION[] = "0271";
inline constexpr const char *AREA_0271_DEBUG_NAMES[]{"0271A020.c\n", "0271A050.c\n"};
inline constexpr Vector3 AREA_0271_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x4274cccdU), .z = 0.0f};
inline constexpr shen_name AREA_0271_LIGHT{SHEN_NAME(l001)};
inline constexpr float AREA_0271_CUFF_DISTANCE = std::bit_cast<float>(0x3e99999aU);
void Setup0271();
void Initialize0271();
void Start0271(std::uintptr_t argument);
void Update0271(void *taskData);
void Cleanup0271(AuthorViewerTaskParameter *parent);
std::int64_t Queue0271Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0271Author(AuthorViewerLoopTaskParameter *child);
void Update0271AuthorTask(void *taskData);
void Prepare0271Characters();
void Restore0271Characters();
extern const ScriptAreaDefinition area0271ScriptDefinition;

} // namespace shenmue::s2::areas
