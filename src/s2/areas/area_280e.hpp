#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_280E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_280E_KT1_{SHEN_NAME(KT1_)};
inline constexpr shen_name AREA_280E_aa49{SHEN_NAME(aa49)};
inline constexpr shen_name AREA_280E_a452{SHEN_NAME(a452)};
inline constexpr shen_name AREA_280E_ab19{SHEN_NAME(ab19)};
inline constexpr shen_name AREA_280E_ab18{SHEN_NAME(ab18)};
inline constexpr shen_name AREA_280E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_280E_KT2_{SHEN_NAME(KT2_)};
inline constexpr shen_name AREA_280E_EHY_{SHEN_NAME(EHY_)};
inline constexpr shen_name AREA_280E_CHARACTERS[]{AREA_280E_REN_, AREA_280E_KT2_, AREA_280E_KT1_, AREA_280E_EHY_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_280E_MOTION[] = "0280E.MOT", AREA_280E_CONVERSATION[] = "0280";
inline constexpr const char *AREA_280E_DEBUG_NAMES[]{"0280E000.c\n", "0280E020.c\n", "0280E030.c\n", "0280E040.c\n", "0280E050.c\n", "0280E070.c\n"};
struct Area280ELoopParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area280ELoopParameter) == 16);
static_assert(offsetof(Area280ELoopParameter, currentFrame) == 8);
void Initialize280E();
void Start280E(std::uintptr_t argument);
void Update280E(void *taskData);
void Cleanup280E(AuthorViewerTaskParameter *parent);
std::int64_t Queue280EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update280EAuthor(Area280ELoopParameter *child);
void Update280EAuthorTask(void *taskData);
void Prepare280ECharacters();
void Restore280ECharacters();
extern const ScriptAreaDefinition area280EScriptDefinition;

} // namespace shenmue::s2::areas
