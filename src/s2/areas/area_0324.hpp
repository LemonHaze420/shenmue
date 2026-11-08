#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0324_LIGHTS[]{SHEN_NAME(l001), SHEN_NAME(l002)};
inline constexpr shen_name AREA_0324_FIRST_AUTHOR{SHEN_NAME(0324)}, AREA_0324_SECOND_AUTHOR{SHEN_NAME(0262)};
inline constexpr shen_name AREA_0324_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0324_a276{SHEN_NAME(a276)};
inline constexpr shen_name AREA_0324_aa80{SHEN_NAME(aa80)};
inline constexpr shen_name AREA_0324_aa77{SHEN_NAME(aa77)};
inline constexpr shen_name AREA_0324_a464{SHEN_NAME(a464)};
inline constexpr shen_name AREA_0324_a202{SHEN_NAME(a202)};
inline constexpr shen_name AREA_0324_a203{SHEN_NAME(a203)};
inline constexpr shen_name AREA_0324_a247{SHEN_NAME(a247)};
inline constexpr shen_name AREA_0324_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0324_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0324_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0324_SCX_{SHEN_NAME(SCX_)};
inline constexpr shen_name AREA_0324_CHR_{SHEN_NAME(CHR_)};
inline constexpr shen_name AREA_0324_P01_{SHEN_NAME(P01_)};
inline constexpr shen_name AREA_0324_CHARACTERS[]{
    AREA_0324_WON_, AREA_0324_REN_, AREA_0324_TGY_, AREA_0324_SCX_, AREA_0324_CHR_, AREA_0324_P01_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0324_CONVERSATION[] = "0324";
inline constexpr const char *AREA_0324_DEBUG_NAMES[]{"0324A000.c\n", "0262A000.c\n"};
struct Area0324Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::byte reserved0c[4];
};
struct Area0324AuthorParameter {
    Area0324Parameter *parent;
};
static_assert(sizeof(Area0324Parameter) == 16);
static_assert(offsetof(Area0324Parameter, selection) == 8 && offsetof(Area0324Parameter, state) == 10);
static_assert(sizeof(Area0324AuthorParameter) == 8);
inline constexpr char AREA_0324_ARCHIVE[] = "0324A";
inline constexpr shen_name AREA_0324_AUTHORS[]{SHEN_NAME(032A), SHEN_NAME(032B)};
void Initialize0324();
void Start0324(std::uintptr_t argument);
void Update0324(void *taskData);
void Cleanup0324(Area0324Parameter *parent);
std::int64_t Queue0324Author(Area0324Parameter *parent);
std::int64_t Update0324Author(Area0324AuthorParameter *child);
void Update0324AuthorTask(void *taskData);
void Prepare0324Characters(std::int16_t selection);
void Restore0324Characters(std::int16_t selection);
extern const ScriptAreaDefinition area0324ScriptDefinition;

} // namespace shenmue::s2::areas
