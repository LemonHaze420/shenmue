#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_528E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_528E_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_528E_a349{SHEN_NAME(a349)};
inline constexpr shen_name AREA_528E_a636{SHEN_NAME(a636)};
inline constexpr shen_name AREA_528E_CHARACTERS[]{AREA_528E_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_528E_MOTION[] = "0528E.MOT", AREA_528E_CONVERSATION[] = "0528";
inline constexpr const char *AREA_528E_DEBUG_NAMES[]{"0528E010.c\n", "0528E020.c\n", "0528E040.c\n"};
void Setup528E();
void Initialize528E();
void Start528E(std::uintptr_t argument);
void Update528E(void *taskData);
void Cleanup528E(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue528EAuthor(AuthorViewerPairTaskParameter *parent);
std::int64_t Update528EAuthor(AuthorViewer517ELoopTaskParameter *child);
void Update528EAuthorTask(void *taskData);
void Prepare528ECharacters();
void Restore528ECharacters();
extern const ScriptAreaDefinition area528EScriptDefinition;

} // namespace shenmue::s2::areas
