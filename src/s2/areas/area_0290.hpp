#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0290_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0290_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0290_KK8_{SHEN_NAME(KK8_)};
inline constexpr shen_name AREA_0290_KK9_{SHEN_NAME(KK9_)};
inline constexpr shen_name AREA_0290_KN1_{SHEN_NAME(KN1_)};
inline constexpr shen_name AREA_0290_RIK_{SHEN_NAME(RIK_)};
inline constexpr shen_name AREA_0290_SBK_{SHEN_NAME(SBK_)};
inline constexpr shen_name AREA_0290_CHARACTERS[]{
    AREA_0290_REN_, AREA_0290_KK8_, AREA_0290_KK9_, AREA_0290_KN1_, AREA_0290_RIK_, AREA_0290_SBK_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0290_MOTION[] = "0290A.MOT", AREA_0290_CONVERSATION[] = "0290";
inline constexpr const char *AREA_0290_DEBUG_NAMES[]{"0290A020.c\n", "0290A030.c\n"};
inline constexpr Vector3 AREA_0290_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x422ccccdU), .z = 0.0f};
void Setup0290();
void Initialize0290();
void Start0290(std::uintptr_t argument);
void Update0290(void *taskData);
void Cleanup0290(AuthorViewerTaskParameter *parent);
std::int64_t Update0290Author(AuthorViewerLoopTaskParameter *child);
void Update0290AuthorTask(void *taskData);
void Prepare0290Characters();
void Restore0290Characters();
extern const ScriptAreaDefinition area0290ScriptDefinition;

} // namespace shenmue::s2::areas
