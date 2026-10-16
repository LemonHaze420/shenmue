#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_031E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_031E_SAI_{SHEN_NAME(SAI_)};
inline constexpr shen_name AREA_031E_SIZ_{SHEN_NAME(SIZ_)};
inline constexpr shen_name AREA_031E_a148{SHEN_NAME(a148)};
inline constexpr shen_name AREA_031E_a048{SHEN_NAME(a048)};
inline constexpr shen_name AREA_031E_CHARACTERS[]{AREA_031E_SAI_, AREA_031E_SIZ_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_031E_MOTION[] = "0031E.MOT", AREA_031E_CONVERSATION[] = "0031";
inline constexpr const char *AREA_031E_DEBUG_NAMES[]{"0031E000.c\n", "0031E001.c\n", "0031E002.c\n", "0031E020.c\n", "0031E021.c\n", "0031E022.c\n",
    "0031E023.c\n", "0031E024.c\n", "0031E025.c\n", "0031E026.c\n", "0031E027.c\n", "0031E003.c\n", "0031E028.c\n"};
void Initialize031E();
void Start031E(std::uintptr_t argument);
void Update031E(void *taskData);
void Cleanup031E(AuthorViewerTaskParameter *parent);
std::int64_t Queue031EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update031EAuthor(AuthorViewerLoopTaskParameter *child);
void Update031EAuthorTask(void *taskData);
void Prepare031ECharacters();
void Restore031ECharacters();
extern const ScriptAreaDefinition area031EScriptDefinition;

} // namespace shenmue::s2::areas
