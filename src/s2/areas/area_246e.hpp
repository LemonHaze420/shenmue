#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_246E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_246E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_246E_a454{SHEN_NAME(a454)};
inline constexpr shen_name AREA_246E_CHARACTERS[]{AREA_246E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_246E_MOTION[] = "0246E.MOT", AREA_246E_CONVERSATION[] = "0246";
inline constexpr const char *AREA_246E_DEBUG_NAMES[]{"0246E000.c\n"};
void Initialize246E();
void Start246E(std::uintptr_t argument);
void Update246E(void *taskData);
void Cleanup246E(AuthorViewerTaskParameter *parent);
std::int64_t Queue246EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update246EAuthor(AuthorViewerLoopTaskParameter *child);
void Update246EAuthorTask(void *taskData);
extern const ScriptAreaDefinition area246EScriptDefinition;

} // namespace shenmue::s2::areas
