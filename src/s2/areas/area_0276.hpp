#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0276_EVENT{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0276_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0276_FIRE_PROP{SHEN_NAME(a412)}, AREA_0276_TRANSPARENT_PROP{SHEN_NAME(aa89)};
inline constexpr shen_name AREA_0276_REN{SHEN_NAME(REN_)}, AREA_0276_SRY{SHEN_NAME(SRY_)};
inline constexpr shen_name AREA_0276_SGT{SHEN_NAME(SGT_)}, AREA_0276_CHO{SHEN_NAME(CHO_)};
inline constexpr shen_name AREA_0276_LIGHT{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0276_RETURN_FLAGGED{SHEN_NAME(0362)};
inline constexpr shen_name AREA_0276_RETURN_CLEAR{SHEN_NAME(0278)};
inline constexpr shen_name AREA_0276_CHARACTERS[]{
    SHEN_NAME(SRY_), SHEN_NAME(WON_), SHEN_NAME(REN_), SHEN_NAME(JOP_), SHEN_NAME(CHO_), SHEN_NAME(SGT_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0276_INITIAL_PROPS[]{SHEN_NAME(a209), SHEN_NAME(a207), SHEN_NAME(a208), SHEN_NAME(a326), SHEN_NAME(a270), SHEN_NAME(a325)};
inline constexpr shen_name AREA_0276_INDOOR_PROPS[]{SHEN_NAME(a342), SHEN_NAME(a343), SHEN_NAME(ab19), SHEN_NAME(ab18), SHEN_NAME(aa23)};
inline constexpr shen_name AREA_0276_FIRST_PROPS[]{
    SHEN_NAME(a300), SHEN_NAME(a067), SHEN_NAME(a393), SHEN_NAME(a394), SHEN_NAME(a395), SHEN_NAME(a412), SHEN_NAME(aa89)};
struct Area0276Parameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::int16_t loadState;
    std::int16_t characterIndex;
};
struct Area0276AuthorParameter {
    std::int16_t frame;
    std::byte reserved02[6];
    Area0276Parameter *parent;
};
static_assert(sizeof(Area0276Parameter) == 16 && sizeof(Area0276AuthorParameter) == 16);
static_assert(offsetof(Area0276Parameter, selection) == 8 && offsetof(Area0276Parameter, state) == 10);
static_assert(offsetof(Area0276AuthorParameter, frame) == 0 && offsetof(Area0276AuthorParameter, parent) == 8);
void Initialize0276();
void Start0276(std::uintptr_t argument);
void Load0276Characters(void *data);
void Update0276(void *data);
void Update0276FirstAuthor(void *data);
void Update0276SecondAuthor(void *data);
void Setup0276Environment();
void Prepare0276Characters();
void Restore0276Characters();
void Prepare0276FirstScene();
void Restore0276FirstScene();
void Queue0276Author(Area0276Parameter *parent);
void Cleanup0276(Area0276Parameter *parent);
extern const ScriptAreaDefinition area0276ScriptDefinition;
} // namespace shenmue::s2::areas
