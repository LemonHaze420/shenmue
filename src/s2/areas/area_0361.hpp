#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0361_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0361_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0361_CHARACTERS[]{AREA_0361_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0361_CONVERSATION[] = "0361";
inline constexpr shen_name AREA_0361_RETURN{SHEN_NAME(QF00)};
inline constexpr shen_name AREA_0361_AUTHOR{SHEN_NAME(0361)};
struct Area0361Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0361AuthorParameter {
    Area0361Parameter *parent;
};
static_assert(sizeof(Area0361Parameter) == 16);
static_assert(offsetof(Area0361Parameter, selection) == 8 && offsetof(Area0361Parameter, state) == 10);
static_assert(sizeof(Area0361AuthorParameter) == 8);
inline constexpr char AREA_0361_ARCHIVE[] = "0361A";
void Initialize0361();
void Start0361(std::uintptr_t argument);
void Update0361(void *taskData);
void Cleanup0361(Area0361Parameter *parent);
std::int64_t Queue0361Author(Area0361Parameter *parent);
std::int64_t Update0361Author(Area0361AuthorParameter *child);
void Update0361AuthorTask(void *taskData);
void Setup0361Scene();
extern const ScriptAreaDefinition area0361ScriptDefinition;

} // namespace shenmue::s2::areas
