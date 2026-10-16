#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_235E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_235E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_235E_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_235E_OB4_{SHEN_NAME(OB4_)};
inline constexpr shen_name AREA_235E_CHARACTERS[]{AREA_235E_YUA_, AREA_235E_OB4_, AREA_235E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_235E_MOTION[] = "0235E.MOT", AREA_235E_CONVERSATION[] = "0235";
inline constexpr const char *AREA_235E_DEBUG_NAMES[]{"0235E032.c\n", "0235E072.c\n", "0235E073.c\n"};
void Initialize235E();
void Start235E(std::uintptr_t argument);
void Update235E(void *taskData);
void Cleanup235E(AuthorViewerTaskParameter *parent);
std::int64_t Queue235EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update235EAuthor(AuthorViewerLoopTaskParameter *child);
void Update235EAuthorTask(void *taskData);
void Prepare235ECharacters();
void Restore235ECharacters();
extern const ScriptAreaDefinition area235EScriptDefinition;

} // namespace shenmue::s2::areas
