#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_275E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_275E_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_275E_a532{SHEN_NAME(a532)};
inline constexpr shen_name AREA_275E_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_275E_SGT_{SHEN_NAME(SGT_)};
inline constexpr shen_name AREA_275E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_275E_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_275E_SRY_{SHEN_NAME(SRY_)};
inline constexpr shen_name AREA_275E_CHARACTERS[]{
    AREA_275E_JOP_, AREA_275E_SGT_, AREA_275E_REN_, AREA_275E_WON_, AREA_275E_TGY_, AREA_275E_SRY_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_275E_MOTION[] = "0275E.MOT", AREA_275E_CONVERSATION[] = "0275";
inline constexpr const char *AREA_275E_DEBUG_NAMES[]{"0275E030.c\n", "0275E060.c\n", "0275E070.c\n", "0275E040.c\n", "0275E041.c\n", "0275E042.c\n"};
void Initialize275E();
void Start275E(std::uintptr_t argument);
void Update275E(void *taskData);
void Cleanup275E(AuthorViewerTaskParameter *parent);
std::int64_t Queue275EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update275EAuthor(AuthorViewerLoopTaskParameter *child);
void Update275EAuthorTask(void *taskData);
void Prepare275ECharacters();
void Restore275ECharacters();
extern const ScriptAreaDefinition area275EScriptDefinition;

} // namespace shenmue::s2::areas
