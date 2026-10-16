#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_063E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_063E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_063E_a067{SHEN_NAME(a067)};
inline constexpr shen_name AREA_063E_CHARACTERS[]{AREA_063E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_063E_MOTION[] = "0063E.MOT", AREA_063E_CONVERSATION[] = "0063";
inline constexpr const char *AREA_063E_DEBUG_NAMES[]{"0063E010.c\n", "0063E011.c\n", "0063E012.c\n", "0063E020.c\n", "0063E021.c\n", "0063E022.c\n",
    "0063E030.c\n", "0063E031.c\n", "0063E032.c\n", "0063E043.c\n", "0063E050.c\n", "0063E055.c\n", "0063E051.c\n", "0063E052.c\n", "0063E053.c\n",
    "0063E054.c\n", "0063E040.c\n", "0063E000.c\n", "0063E023.c\n", "0063E041.c\n"};
struct Area063ELoopParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area063ELoopParameter) == 16);
static_assert(offsetof(Area063ELoopParameter, currentFrame) == 8);
void Initialize063E();
void Start063E(std::uintptr_t argument);
void Update063E(void *taskData);
void Cleanup063E(AuthorViewerTaskParameter *parent);
std::int64_t Queue063EAuthor(AuthorViewerTaskParameter *parent);
std::int64_t Update063EAuthor(Area063ELoopParameter *child);
void Update063EAuthorTask(void *taskData);
void Prepare063ECharacters();
void Restore063ECharacters();
extern const ScriptAreaDefinition area063EScriptDefinition;

} // namespace shenmue::s2::areas
