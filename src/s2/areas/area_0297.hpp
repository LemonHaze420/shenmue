#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0297_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0297_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0297_CHD_{SHEN_NAME(CHD_)};
inline constexpr shen_name AREA_0297_CHARACTERS[]{AREA_0297_REN_, AREA_0297_CHD_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0297_CONVERSATION[] = "0297";
inline constexpr shen_name AREA_0297_RETURN{SHEN_NAME(QF02)};
inline constexpr shen_name AREA_0297_AUTHOR{SHEN_NAME(0297)};
void Initialize0297();
void Start0297(std::uintptr_t argument);
void Update0297(void *taskData);
void Cleanup0297(AuthorViewerTaskParameter *parent);
std::int64_t Queue0297Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0297Author(AuthorViewerLoopTaskParameter *child);
void Update0297AuthorTask(void *taskData);
void Prepare0297Characters();
void Restore0297Characters();
void Setup0297Scene();
extern const ScriptAreaDefinition area0297ScriptDefinition;

} // namespace shenmue::s2::areas
