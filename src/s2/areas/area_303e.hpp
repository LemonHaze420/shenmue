#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_303E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_303E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_303E_a942{SHEN_NAME(a942)};
inline constexpr shen_name AREA_303E_CHARACTERS[]{AREA_303E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_303E_MOTION[] = "0303E.MOT", AREA_303E_CONVERSATION[] = "0303";
inline constexpr const char *AREA_303E_DEBUG_NAMES[]{"0303E000.c\n", "0303E020.c\n"};
inline constexpr Vector3 AREA_303E_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x422ccccdU), .z = 0.0f};
void Setup303E();
void Initialize303E();
void Start303E(std::uintptr_t argument);
void Update303E(void *taskData);
void Cleanup303E(AuthorViewerTaskParameter *parent);
std::int64_t Queue303EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update303EAuthor(AuthorViewerLoopTaskParameter *child);
void Update303EAuthorTask(void *taskData);
extern const ScriptAreaDefinition area303EScriptDefinition;

} // namespace shenmue::s2::areas
