#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_253E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_253E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_253E_KT1_{SHEN_NAME(KT1_)};
inline constexpr shen_name AREA_253E_KT3_{SHEN_NAME(KT3_)};
inline constexpr shen_name AREA_253E_KU6_{SHEN_NAME(KU6_)};
inline constexpr shen_name AREA_253E_KK1_{SHEN_NAME(KK1_)};
inline constexpr shen_name AREA_253E_KK3_{SHEN_NAME(KK3_)};
inline constexpr shen_name AREA_253E_KK6_{SHEN_NAME(KK6_)};
inline constexpr shen_name AREA_253E_a836{SHEN_NAME(a836)};
inline constexpr shen_name AREA_253E_a838{SHEN_NAME(a838)};
inline constexpr shen_name AREA_253E_CHARACTERS[]{
    AREA_253E_REN_, AREA_253E_KT1_, AREA_253E_KT3_, AREA_253E_KU6_, AREA_253E_KK1_, AREA_253E_KK3_, AREA_253E_KK6_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_253E_MOTION[] = "0253E.MOT", AREA_253E_CONVERSATION[] = "0253";
inline constexpr const char *AREA_253E_DEBUG_NAMES[]{"0253E100.c\n", "0253E101.c\n", "0253E102.c\n", "0253E103.c\n", "0253E104.c\n"};
void Initialize253E();
void Start253E(std::uintptr_t argument);
void Update253E(void *taskData);
void Cleanup253E(AuthorViewerTaskParameter *parent);
std::int64_t Queue253EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update253EAuthor(AuthorViewerLoopTaskParameter *child);
void Update253EAuthorTask(void *taskData);
void Prepare253ECharacters();
void Restore253ECharacters();
extern const ScriptAreaDefinition area253EScriptDefinition;

} // namespace shenmue::s2::areas
