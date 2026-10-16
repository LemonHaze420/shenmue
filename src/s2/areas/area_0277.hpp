#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0277_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0277_a209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0277_a207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0277_a208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0277_a270{SHEN_NAME(a270)};
inline constexpr shen_name AREA_0277_a324{SHEN_NAME(a324)};
inline constexpr shen_name AREA_0277_a325{SHEN_NAME(a325)};
inline constexpr shen_name AREA_0277_a047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0277_a342{SHEN_NAME(a342)};
inline constexpr shen_name AREA_0277_a343{SHEN_NAME(a343)};
inline constexpr shen_name AREA_0277_ab18{SHEN_NAME(ab18)};
inline constexpr shen_name AREA_0277_ab19{SHEN_NAME(ab19)};
inline constexpr shen_name AREA_0277_aa23{SHEN_NAME(aa23)};
inline constexpr shen_name AREA_0277_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0277_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0277_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0277_CHARACTERS[]{AREA_0277_WON_, AREA_0277_JOP_, AREA_0277_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0277_CONVERSATION[] = "0277";
inline constexpr shen_name AREA_0277_RETURN_CLEAR{SHEN_NAME(0278)};
inline constexpr shen_name AREA_0277_RETURN_FLAGGED{SHEN_NAME(0362)};
inline constexpr shen_name AREA_0277_LIGHT{SHEN_NAME(l101)};
struct Area0277Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0277AuthorParameter {
    Area0277Parameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0277Parameter) == 16);
static_assert(offsetof(Area0277Parameter, selection) == 8 && offsetof(Area0277Parameter, state) == 10);
static_assert(sizeof(Area0277AuthorParameter) == 16);
inline constexpr char AREA_0277_ARCHIVE[] = "0277E";
void Initialize0277();
void Start0277(std::uintptr_t argument);
void Update0277(void *taskData);
void Cleanup0277(Area0277Parameter *parent);
std::int64_t Queue0277Author(Area0277Parameter *parent);
std::int64_t Update0277Author(Area0277AuthorParameter *child);
void Update0277AuthorTask(void *taskData);
void Prepare0277Characters();
void Restore0277Characters();
void Setup0277Scene();
extern const ScriptAreaDefinition area0277ScriptDefinition;

} // namespace shenmue::s2::areas
