#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_202E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_202E_a052{SHEN_NAME(a052)};
inline constexpr shen_name AREA_202E_a053{SHEN_NAME(a053)};
inline constexpr shen_name AREA_202E_a212{SHEN_NAME(a212)};
inline constexpr shen_name AREA_202E_aa22{SHEN_NAME(aa22)};
inline constexpr shen_name AREA_202E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_202E_CHARACTERS[]{AREA_202E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_202E_MOTION[] = "0202E.MOT", AREA_202E_CONVERSATION[] = "0202";
inline constexpr const char *AREA_202E_DEBUG_NAMES[]{"0202E010.c\n"};
struct Area202ELoopParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area202ELoopParameter) == 16);
static_assert(offsetof(Area202ELoopParameter, currentFrame) == 8);
void Initialize202E();
void Start202E(std::uintptr_t argument);
void Update202E(void *taskData);
void Cleanup202E(AuthorViewerTaskParameter *parent);
std::int64_t Update202EAuthor(Area202ELoopParameter *child);
void Update202EAuthorTask(void *taskData);
void Prepare202ECharacters();
void Restore202ECharacters();
extern const ScriptAreaDefinition area202EScriptDefinition;

} // namespace shenmue::s2::areas
