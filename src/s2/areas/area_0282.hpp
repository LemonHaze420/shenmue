#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0282_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0282_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0282_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0282_CHARACTERS[]{AREA_0282_REN_, AREA_0282_TGY_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0282_MOTION[] = "0282A.MOT", AREA_0282_CONVERSATION[] = "0282";
inline constexpr const char *AREA_0282_DEBUG_NAMES[]{"0282A000.c\n"};
inline constexpr char AREA_0282_AUDIO[] = "0282A000";
inline constexpr Vector3 AREA_0282_MAP_POSITION{.x = 0.0f, .y = 54.0f, .z = 0.0f};
void Setup0282();
void Initialize0282();
void Start0282(std::uintptr_t argument);
void Update0282(void *taskData);
void Cleanup0282(AuthorViewerTaskParameter *parent);
std::int64_t Queue0282Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0282Author(AuthorViewerLoopTaskParameter *child);
void Update0282AuthorTask(void *taskData);
void Prepare0282Characters();
void Restore0282Characters();
extern const ScriptAreaDefinition area0282ScriptDefinition;

} // namespace shenmue::s2::areas
