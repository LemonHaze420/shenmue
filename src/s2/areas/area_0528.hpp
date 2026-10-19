#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0528_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0528_SIN{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0528_CHARACTERS[]{AREA_0528_SIN, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr shen_name AREA_0528_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01)};
struct Area0528Input {
    std::int32_t author;
    Vector3 position;
    std::int32_t yaw;
};
struct Area0528Parameter {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::int32_t author;
    std::int16_t state;
    std::byte reserved12[6];
    Area0528Input *input;
};
struct Area0528AuthorParameter {
    Area0528Parameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0528Input) == 20);
static_assert(sizeof(Area0528Parameter) == 32);
static_assert(offsetof(Area0528Parameter, author) == 12 && offsetof(Area0528Parameter, state) == 16);
static_assert(offsetof(Area0528Parameter, input) == 24);
static_assert(sizeof(Area0528AuthorParameter) == 16);
static_assert(offsetof(Area0528AuthorParameter, currentFrame) == 8);

void Initialize0528();
void Start0528(std::uintptr_t argument);
void Update0528(void *taskData);
void Cleanup0528(Area0528Parameter *parent);
std::int64_t Queue0528Author(Area0528Parameter *parent);
std::int64_t Update0528Author(Area0528AuthorParameter *child);
void Update0528AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0528ScriptDefinition;

} // namespace shenmue::s2::areas
