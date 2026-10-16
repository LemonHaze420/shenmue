#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_039E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_039E_a128{SHEN_NAME(a128)};
inline constexpr shen_name AREA_039E_ANC_{SHEN_NAME(ANC_)};
inline constexpr shen_name AREA_039E_CHR_{SHEN_NAME(CHR_)};
inline constexpr shen_name AREA_039E_GEN_{SHEN_NAME(GEN_)};
inline constexpr shen_name AREA_039E_SBO_{SHEN_NAME(SBO_)};
inline constexpr shen_name AREA_039E_CHARACTERS[]{AREA_039E_ANC_, AREA_039E_CHR_, AREA_039E_GEN_, AREA_039E_SBO_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_039E_MOTION[] = "0039E.MOT", AREA_039E_CONVERSATION[] = "0039";
inline constexpr const char *AREA_039E_DEBUG_NAMES[]{"0039E021.c\n", "0039E022.c\n"};
void Initialize039E();
void Start039E(std::uintptr_t argument);
void Update039E(void *taskData);
void Cleanup039E(AuthorViewerPairTaskParameter *parent);
std::int64_t Queue039EAuthor(AuthorViewerPairTaskParameter *parent);
std::int64_t Update039EAuthor(AuthorViewer517ELoopTaskParameter *child);
void Update039EAuthorTask(void *taskData);
void Prepare039ECharacters();
void Restore039ECharacters();
extern const ScriptAreaDefinition area039EScriptDefinition;

} // namespace shenmue::s2::areas
