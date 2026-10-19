#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0549_LIGHT{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0549_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0549_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0549_a067{SHEN_NAME(a067)};
inline constexpr shen_name AREA_0549_aa30{SHEN_NAME(aa30)};
inline constexpr shen_name AREA_0549_SRY_{SHEN_NAME(SRY_)};
inline constexpr shen_name AREA_0549_CHARACTERS[]{AREA_0549_SRY_, AREA_0549_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0549_CONVERSATION[] = "0549";
inline constexpr const char *AREA_0549_DEBUG_NAMES[]{"0549A000.c\n", "0549A010.c\n"};
struct Area0549Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0549AuthorParameter {
    Area0549Parameter *parent;
};
static_assert(sizeof(Area0549Parameter) == 16);
static_assert(offsetof(Area0549Parameter, selection) == 8 && offsetof(Area0549Parameter, state) == 10);
static_assert(sizeof(Area0549AuthorParameter) == 8);
inline constexpr char AREA_0549_ARCHIVE[] = "0549E";
inline constexpr shen_name AREA_0549_AUTHORS[]{SHEN_NAME(054A), SHEN_NAME(054B)};
inline constexpr shen_name AREA_0549_TITLE_AUTHOR{SHEN_NAME(GOV2)};
void Update0549Waves(void *parameter);
void Setup0549Environment();
void Initialize0549();
void Start0549(std::uintptr_t argument);
void Update0549(void *taskData);
void Cleanup0549(Area0549Parameter *parent);
std::int64_t Queue0549Author(Area0549Parameter *parent);
std::int64_t Update0549Author(Area0549AuthorParameter *child);
void Update0549AuthorTask(void *taskData);
void Prepare0549Characters();
void Restore0549Characters();
extern const ScriptAreaDefinition area0549ScriptDefinition;

} // namespace shenmue::s2::areas
