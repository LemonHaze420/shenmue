#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_282E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_282E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_282E_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_282E_SOY_{SHEN_NAME(SOY_)};
inline constexpr shen_name AREA_282E_SEE_{SHEN_NAME(SEE_)};
inline constexpr shen_name AREA_282E_a896{SHEN_NAME(a896)};
inline constexpr shen_name AREA_282E_YUKB{SHEN_NAME(YUKB)};
inline constexpr shen_name AREA_282E_CHARACTERS[]{AREA_282E_REN_, AREA_282E_TGY_, AREA_282E_SOY_, AREA_282E_SEE_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_282E_MOTION[] = "0282E.MOT", AREA_282E_CONVERSATION[] = "0282";
inline constexpr const char *AREA_282E_DEBUG_NAMES[]{"0282E070.c\n", "0282E080.c\n", "0282E020.c\n", "0282E030.c\n", "0282E050.c\n", "0282E060.c\n"};
void Initialize282E();
void Start282E(std::uintptr_t argument);
void Update282E(void *taskData);
void Cleanup282E(AuthorViewerTaskParameter *parent);
std::int64_t Queue282EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update282EAuthor(AuthorViewerLoopTaskParameter *child);
void Update282EAuthorTask(void *taskData);
void Prepare282ECharacters();
void Restore282ECharacters();
extern const ScriptAreaDefinition area282EScriptDefinition;

} // namespace shenmue::s2::areas
