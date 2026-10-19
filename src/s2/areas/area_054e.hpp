#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_054E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_054E_AGO_{SHEN_NAME(AGO_)};
inline constexpr shen_name AREA_054E_a417{SHEN_NAME(a417)};
inline constexpr shen_name AREA_054E_aa50{SHEN_NAME(aa50)};
inline constexpr shen_name AREA_054E_a073{SHEN_NAME(a073)};
inline constexpr shen_name AREA_054E_CHARACTERS[]{AREA_054E_AGO_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_054E_CONVERSATION[] = "0054";
inline constexpr const char *AREA_054E_DEBUG_NAMES[]{"0054E000.c\n", "0054E010.c\n"};
struct Area054EParameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area054EAuthorParameter {
    Area054EParameter *parent;
};
static_assert(sizeof(Area054EParameter) == 16);
static_assert(offsetof(Area054EParameter, selection) == 8 && offsetof(Area054EParameter, state) == 10);
static_assert(sizeof(Area054EAuthorParameter) == 8);
inline constexpr char AREA_054E_ARCHIVE[] = "0054E";
inline constexpr shen_name AREA_054E_AUTHORS[]{SHEN_NAME(054A), SHEN_NAME(054B)};
inline constexpr shen_name AREA_054E_RETURN{SHEN_NAME(AB00)};
inline constexpr char AREA_054E_AUDIO[] = "0054E000";
void Initialize054E();
void Start054E(std::uintptr_t argument);
void Update054E(void *taskData);
void Cleanup054E(Area054EParameter *parent);
std::int64_t Queue054EAuthor(Area054EParameter *parent);
std::int64_t Update054EAuthor(Area054EAuthorParameter *child);
void Update054EAuthorTask(void *taskData);
void Prepare054ECharacters();
void Restore054ECharacters();
extern const ScriptAreaDefinition area054EScriptDefinition;

} // namespace shenmue::s2::areas
