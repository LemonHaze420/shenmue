#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0266_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0266_aa99{SHEN_NAME(aa99)};
inline constexpr shen_name AREA_0266_a020{SHEN_NAME(a020)};
inline constexpr shen_name AREA_0266_P02_{SHEN_NAME(P02_)};
inline constexpr shen_name AREA_0266_P04_{SHEN_NAME(P04_)};
inline constexpr shen_name AREA_0266_P05_{SHEN_NAME(P05_)};
inline constexpr shen_name AREA_0266_P07_{SHEN_NAME(P07_)};
inline constexpr shen_name AREA_0266_P08_{SHEN_NAME(P08_)};
inline constexpr shen_name AREA_0266_P09_{SHEN_NAME(P09_)};
inline constexpr shen_name AREA_0266_KF4_{SHEN_NAME(KF4_)};
inline constexpr shen_name AREA_0266_CHARACTERS[]{
    AREA_0266_P02_, AREA_0266_P04_, AREA_0266_P05_, AREA_0266_P07_, AREA_0266_P08_, AREA_0266_P09_, AREA_0266_KF4_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0266_MOTION[] = "0266A.MOT", AREA_0266_CONVERSATION[] = "0266";
inline constexpr const char *AREA_0266_DEBUG_NAMES[]{"0266A000.c\n", "0266A020.c\n", "0266A090.c\n", "0266A110.c\n"};
inline constexpr Vector3 AREA_0266_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x4281999aU), .z = 0.0f};
inline constexpr Vector3 AREA_0266_SEVENTH_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x4274cccdU), .z = 0.0f};
inline constexpr char AREA_0266_AUDIO[] = "0266A000";
void Setup0266();
void Initialize0266();
void Start0266(std::uintptr_t argument);
void Update0266(void *taskData);
void Cleanup0266(AuthorViewerTaskParameter *parent);
std::int64_t Queue0266Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0266Author(AuthorViewer081ELoopTaskParameter *child);
void Update0266AuthorTask(void *taskData);
void Prepare0266Characters();
void Restore0266Characters();
extern const ScriptAreaDefinition area0266ScriptDefinition;

} // namespace shenmue::s2::areas
