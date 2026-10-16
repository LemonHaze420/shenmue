#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_234E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_234E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_234E_KUK_{SHEN_NAME(KUK_)};
inline constexpr shen_name AREA_234E_WTE_{SHEN_NAME(WTE_)};
inline constexpr shen_name AREA_234E_a898{SHEN_NAME(a898)};
inline constexpr shen_name AREA_234E_CHARACTERS[]{AREA_234E_REN_, AREA_234E_KUK_, AREA_234E_WTE_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_234E_MOTION[] = "0234E.MOT", AREA_234E_CONVERSATION[] = "0234";
inline constexpr const char *AREA_234E_DEBUG_NAMES[]{"0234E050.c\n"};
void Initialize234E();
void Start234E(std::uintptr_t argument);
void Update234E(void *taskData);
void Cleanup234E(AuthorViewerTaskParameter *parent);
std::int64_t Queue234EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update234EAuthor(AuthorViewerLoopTaskParameter *child);
void Update234EAuthorTask(void *taskData);
void Prepare234ECharacters();
void Restore234ECharacters();
extern const ScriptAreaDefinition area234EScriptDefinition;

} // namespace shenmue::s2::areas
