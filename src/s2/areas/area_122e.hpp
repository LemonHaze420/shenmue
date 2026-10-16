#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_122E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_122E_XGH_{SHEN_NAME(XGH_)};
inline constexpr shen_name AREA_122E_COH_{SHEN_NAME(COH_)};
inline constexpr shen_name AREA_122E_aa95{SHEN_NAME(aa95)};
inline constexpr shen_name AREA_122E_a003{SHEN_NAME(a003)};
inline constexpr shen_name AREA_122E_aa82{SHEN_NAME(aa82)};
inline constexpr shen_name AREA_122E_PG5_{SHEN_NAME(PG5_)};
inline constexpr shen_name AREA_122E_JJ1_{SHEN_NAME(JJ1_)};
inline constexpr shen_name AREA_122E_CLY_{SHEN_NAME(CLY_)};
inline constexpr shen_name AREA_122E_JN8_{SHEN_NAME(JN8_)};
inline constexpr shen_name AREA_122E_OJ5_{SHEN_NAME(OJ5_)};
inline constexpr shen_name AREA_122E_WTC_{SHEN_NAME(WTC_)};
inline constexpr shen_name AREA_122E_CHARACTERS[]{AREA_122E_COH_, AREA_122E_XGH_, AREA_122E_PG5_, AREA_122E_JJ1_, AREA_122E_CLY_, AREA_122E_JN8_,
    AREA_122E_OJ5_, AREA_122E_WTC_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_122E_MOTION[] = "0122E.MOT", AREA_122E_CONVERSATION[] = "0122";
inline constexpr const char *AREA_122E_DEBUG_NAMES[]{
    "0122E010.c\n", "0122E020.c\n", "0122E030.c\n", "0122E040.c\n", "0122E050.c\n", "0122E060.c\n", "0122E070.c\n", "0122E080.c\n"};
inline constexpr char AREA_122E_FIRST_PACKAGE[] = "WT00", AREA_122E_SECOND_PACKAGE[] = "WET0";
void Select122EPackage(std::int32_t selection);
void Initialize122E();
void Start122E(std::uintptr_t argument);
void Update122E(void *taskData);
void Cleanup122E(AuthorViewerTaskParameter *parent);
std::int64_t Update122EAuthor(AuthorViewerLoopTaskParameter *child);
void Update122EAuthorTask(void *taskData);
void Prepare122ECharacters();
void Restore122ECharacters();
extern const ScriptAreaDefinition area122EScriptDefinition;

} // namespace shenmue::s2::areas
