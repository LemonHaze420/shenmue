#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_301E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_301E_a452{SHEN_NAME(a452)};
inline constexpr shen_name AREA_301E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_301E_CHARACTERS[]{AREA_301E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_301E_MOTION[] = "0301E.MOT", AREA_301E_CONVERSATION[] = "0301";
inline constexpr const char *AREA_301E_DEBUG_NAMES[]{"0301E040.c\n", "0301E050.c\n", "0301E060.c\n"};
inline constexpr Vector3 AREA_301E_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x41c9999aU), .z = 0.0f};
void Setup301E();
void Initialize301E();
void Start301E(std::uintptr_t argument);
void Update301E(void *taskData);
void Cleanup301E(AuthorViewerTaskParameter *parent);
std::int64_t Queue301EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update301EAuthor(AuthorViewerLoopTaskParameter *child);
void Update301EAuthorTask(void *taskData);
void Prepare301ECharacters();
void Restore301ECharacters();
extern const ScriptAreaDefinition area301EScriptDefinition;

} // namespace shenmue::s2::areas
