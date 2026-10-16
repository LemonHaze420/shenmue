#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0545_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0545_SIN{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0545_CHARACTERS[]{AREA_0545_SIN, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr shen_name AREA_0545_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01)};
struct Area0545Input {
    std::int32_t author;
    Vector3 position;
    std::int32_t yaw;
};
struct Area0545Parameter {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::int32_t author;
    std::int16_t state;
    std::byte reserved12[6];
    Area0545Input *input;
};
struct Area0545AuthorParameter {
    Area0545Parameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0545Input) == 20);
static_assert(sizeof(Area0545Parameter) == 32);
static_assert(offsetof(Area0545Parameter, author) == 12 && offsetof(Area0545Parameter, state) == 16);
static_assert(offsetof(Area0545Parameter, input) == 24);
static_assert(sizeof(Area0545AuthorParameter) == 16);
static_assert(offsetof(Area0545AuthorParameter, currentFrame) == 8);

void Initialize0545();
void Start0545(std::uintptr_t argument);
void Update0545(void *taskData);
void Cleanup0545(Area0545Parameter *parent);
std::int64_t Queue0545Author(Area0545Parameter *parent);
std::int64_t Update0545Author(Area0545AuthorParameter *child);
void Update0545AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0545ScriptDefinition;

} // namespace shenmue::s2::areas
