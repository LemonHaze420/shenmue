#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0307_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0307_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0307_a396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0307_CHARACTERS[]{AREA_0307_JOP_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0307_CONVERSATION[] = "0307";
inline constexpr shen_name AREA_0307_RETURN{SHEN_NAME(Q300)};
inline constexpr shen_name AREA_0307_BIKE{SHEN_NAME(BIKE)};
inline constexpr shen_name AREA_0307_AUTHOR{SHEN_NAME(0307)};
struct Area0307Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0307AuthorParameter {
    Area0307Parameter *parent;
};
static_assert(sizeof(Area0307Parameter) == 16);
static_assert(offsetof(Area0307Parameter, selection) == 8 && offsetof(Area0307Parameter, state) == 10);
static_assert(sizeof(Area0307AuthorParameter) == 8);
inline constexpr char AREA_0307_ARCHIVE[] = "0307A";
void Initialize0307();
void Start0307(std::uintptr_t argument);
void Update0307(void *taskData);
void Cleanup0307(Area0307Parameter *parent);
std::int64_t Queue0307Author(Area0307Parameter *parent);
std::int64_t Update0307Author(Area0307AuthorParameter *child);
void Update0307AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0307ScriptDefinition;

} // namespace shenmue::s2::areas
