#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_545E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_545E_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_545E_CHARACTERS[]{AREA_545E_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_545E_MOTION[] = "0545E.MOT", AREA_545E_CONVERSATION[] = "0545";
inline constexpr const char *AREA_545E_DEBUG_NAMES[]{"0545E000.c\n", "0545E010.c\n"};
inline constexpr shen_name AREA_545E_SECOND_LIGHT{SHEN_NAME(0001)};
void Setup545E();
void Initialize545E();
void Start545E(std::uintptr_t argument);
void Update545E(void *taskData);
void Cleanup545E(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue545EAuthor(AuthorViewerPairTaskParameter *parent);
extern const ScriptAreaDefinition area545EScriptDefinition;

} // namespace shenmue::s2::areas
