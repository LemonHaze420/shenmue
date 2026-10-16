#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_529E_SIN{SHEN_NAME(SIN_)}, AREA_529E_PROP{SHEN_NAME(a636)};
inline constexpr shen_name AREA_529E_CHARACTERS[]{AREA_529E_SIN, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_529E_MOTION[] = "0529E.MOT", AREA_529E_CONVERSATION[] = "0529";
void Initialize529E();
void Start529E(std::uintptr_t argument);
void Update529E(void *taskData);
void Cleanup529E(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue529EAuthor(AuthorViewerPairTaskParameter *parent);
std::int64_t Update529EAuthor(AuthorViewer519ELoopTaskParameter *child);
void Update529EAuthorTask(void *taskData);
extern const ScriptAreaDefinition area529EScriptDefinition;

} // namespace shenmue::s2::areas
