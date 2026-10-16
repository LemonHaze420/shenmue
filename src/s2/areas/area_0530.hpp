#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0530_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0530_a199{SHEN_NAME(a199)};
inline constexpr shen_name AREA_0530_a197{SHEN_NAME(a197)};
inline constexpr shen_name AREA_0530_a271{SHEN_NAME(a271)};
inline constexpr shen_name AREA_0530_a272{SHEN_NAME(a272)};
inline constexpr shen_name AREA_0530_a273{SHEN_NAME(a273)};
inline constexpr shen_name AREA_0530_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0530_CHARACTERS[]{AREA_0530_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0530_MOTION[] = "0530A.MOT", AREA_0530_CONVERSATION[] = "0530";
inline constexpr shen_name AREA_0530_RUNTIME_MAP{SHEN_NAME(KRC1)};
struct Area0530Parameter {
    HLib::Task *self;
    std::uint32_t motion, selection;
    std::int16_t state;
    std::byte reserved12[6];
    std::int32_t *result;
};
struct Area0530AuthorParameter {
    Area0530Parameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0530Parameter) == 32 && offsetof(Area0530Parameter, result) == 24);
static_assert(sizeof(Area0530AuthorParameter) == 16);
void Setup0530Scene();
void Update0530Waves(void *);
void Initialize0530();
void Start0530(std::uintptr_t argument);
void Update0530(void *taskData);
void Cleanup0530(Area0530Parameter *parent);
std::int64_t Queue0530Author(Area0530Parameter *parent);
std::int64_t Update0530Author(Area0530AuthorParameter *child);
void Update0530AuthorTask(void *taskData);
void Prepare0530Characters();
void Restore0530Characters();
extern const ScriptAreaDefinition area0530ScriptDefinition;

} // namespace shenmue::s2::areas
