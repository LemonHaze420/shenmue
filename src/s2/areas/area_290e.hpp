#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_290E_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_290E_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_290E_CHARACTERS[]{AREA_290E_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_290E_CONVERSATION[] = "0290";
inline constexpr const char *AREA_290E_DEBUG_NAMES[]{"0290E000.c\n", "0290E010.c\n"};
struct Area290EParameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area290EAuthorParameter {
    Area290EParameter *parent;
};
static_assert(sizeof(Area290EParameter) == 16);
static_assert(offsetof(Area290EParameter, selection) == 8 && offsetof(Area290EParameter, state) == 10);
static_assert(sizeof(Area290EAuthorParameter) == 8);
inline constexpr char AREA_290E_ARCHIVE[] = "0290E";
inline constexpr shen_name AREA_290E_AUTHOR{SHEN_NAME(0290)};
void Initialize290E();
void Start290E(std::uintptr_t argument);
void Update290E(void *taskData);
void Cleanup290E(Area290EParameter *parent);
std::int64_t Queue290EAuthor(Area290EParameter *parent);
std::int64_t Update290EAuthor(Area290EAuthorParameter *child);
void Update290EAuthorTask(void *taskData);
extern const ScriptAreaDefinition area290EScriptDefinition;

} // namespace shenmue::s2::areas
