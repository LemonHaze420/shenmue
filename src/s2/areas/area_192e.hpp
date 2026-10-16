#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_192E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_192E_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_192E_CHARACTERS[]{AREA_192E_SYE_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_192E_CONVERSATION[] = "0192";
inline constexpr const char *AREA_192E_DEBUG_NAMES[]{"0192E000.c\n", "0192E010.c\n", "0192E030.c\n"};
struct Area192EParameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area192EAuthorParameter {
    Area192EParameter *parent;
};
static_assert(sizeof(Area192EParameter) == 16);
static_assert(offsetof(Area192EParameter, selection) == 8 && offsetof(Area192EParameter, state) == 10);
static_assert(sizeof(Area192EAuthorParameter) == 8);
inline constexpr char AREA_192E_ARCHIVE[] = "0192E";
inline constexpr shen_name AREA_192E_AUTHORS[]{SHEN_NAME(192A), SHEN_NAME(192B), SHEN_NAME(192C)};
inline constexpr shen_name AREA_192E_SOURCE{SHEN_NAME(WT00)}, AREA_192E_RETURN{SHEN_NAME(WTA0)};
inline constexpr char AREA_192E_FIRST_PACKAGE[] = "WT00", AREA_192E_SECOND_PACKAGE[] = "WTA0";
std::int64_t Select192EPackage(std::int32_t selection);
void Initialize192E();
void Start192E(std::uintptr_t argument);
void Update192E(void *taskData);
void Cleanup192E(Area192EParameter *parent);
std::int64_t Queue192EAuthor(Area192EParameter *parent);
std::int64_t Update192EAuthor(Area192EAuthorParameter *child);
void Update192EAuthorTask(void *taskData);
extern const ScriptAreaDefinition area192EScriptDefinition;

} // namespace shenmue::s2::areas
