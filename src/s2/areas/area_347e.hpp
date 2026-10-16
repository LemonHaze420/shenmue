#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_347E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_347E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_347E_CHARACTERS[]{AREA_347E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_347E_CONVERSATION[] = "0347";
inline constexpr const char *AREA_347E_DEBUG_NAMES[]{"0347E000.c\n", "0347E010.c\n"};
struct Area347EParameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area347EAuthorParameter {
    Area347EParameter *parent;
};
static_assert(sizeof(Area347EParameter) == 16);
static_assert(offsetof(Area347EParameter, selection) == 8 && offsetof(Area347EParameter, state) == 10);
static_assert(sizeof(Area347EAuthorParameter) == 8);
inline constexpr char AREA_347E_ARCHIVE[] = "0347E";
inline constexpr shen_name AREA_347E_AUTHOR{SHEN_NAME(0347)};
void Initialize347E();
void Start347E(std::uintptr_t argument);
void Update347E(void *taskData);
void Cleanup347E(Area347EParameter *parent);
std::int64_t Queue347EAuthor(Area347EParameter *parent);
std::int64_t Update347EAuthor(Area347EAuthorParameter *child);
void Update347EAuthorTask(void *taskData);
extern const ScriptAreaDefinition area347EScriptDefinition;

} // namespace shenmue::s2::areas
