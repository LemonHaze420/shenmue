#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0512_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0512_a067{SHEN_NAME(a067)};
inline constexpr shen_name AREA_0512_a460{SHEN_NAME(a460)};
inline constexpr shen_name AREA_0512_a431{SHEN_NAME(a431)};
inline constexpr shen_name AREA_0512_a214{SHEN_NAME(a214)};
inline constexpr shen_name AREA_0512_aa29{SHEN_NAME(aa29)};
inline constexpr shen_name AREA_0512_a229{SHEN_NAME(a229)};
inline constexpr shen_name AREA_0512_SIN_{SHEN_NAME(SIN_)};
inline constexpr shen_name AREA_0512_CHARACTERS[]{AREA_0512_SIN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0512_MOTION[] = "0512A.MOT", AREA_0512_CONVERSATION[] = "0512";
inline constexpr const char *AREA_0512_DEBUG_NAMES[]{"0512A000.c\n", "0512A010.c\n"};
inline constexpr shen_name AREA_0512_LIGHTS[]{SHEN_NAME(l001), SHEN_NAME(l101)};
struct Area0512AuthorParameter {
    AuthorViewerTaskParameter *parent;
    float skyAlpha, propAlpha, sceneAlpha;
};
static_assert(sizeof(Area0512AuthorParameter) == 24);
static_assert(offsetof(Area0512AuthorParameter, skyAlpha) == 8);
static_assert(offsetof(Area0512AuthorParameter, sceneAlpha) == 16);
std::int64_t Update0512SecondAuthor(Area0512AuthorParameter *child);
void Update0512SecondAuthorTask(void *);
void Initialize0512();
void Start0512(std::uintptr_t argument);
void Update0512(void *taskData);
void Cleanup0512(AuthorViewerTaskParameter *parent);
std::int64_t Queue0512Author(AuthorViewerTaskParameter *parent);
std::int64_t Update0512Author(Area0512AuthorParameter *child);
void Update0512AuthorTask(void *taskData);
void Prepare0512Characters();
void Restore0512Characters();
extern const ScriptAreaDefinition area0512ScriptDefinition;

} // namespace shenmue::s2::areas
