#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0538_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0538_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0538_aa85{SHEN_NAME(aa85)};
inline constexpr shen_name AREA_0538_a287{SHEN_NAME(a287)};
inline constexpr shen_name AREA_0538_a288{SHEN_NAME(a288)};
inline constexpr shen_name AREA_0538_CHARACTERS[]{AREA_0538_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0538_MOTION[] = "0538A.MOT", AREA_0538_CONVERSATION[] = "0538";
inline constexpr const char *AREA_0538_DEBUG_NAMES[]{"0538A000.c\n"};
inline constexpr shen_name AREA_0538_LIGHT{SHEN_NAME(l001)};
struct Area0538AuthorParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0538AuthorParameter) == 16);
void Setup0538Scene();
void Create0538Fire();
void Update0538FireLighting();
void Initialize0538();
void Start0538(std::uintptr_t argument);
void Update0538(void *taskData);
void Cleanup0538(AuthorViewerTaskParameter *parent);
std::int64_t Update0538Author(Area0538AuthorParameter *child);
void Update0538AuthorTask(void *taskData);
extern const ScriptAreaDefinition area0538ScriptDefinition;

} // namespace shenmue::s2::areas
