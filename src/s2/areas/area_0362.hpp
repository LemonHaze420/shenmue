#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/area_0273.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0362_KUN{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0362_PROP_A047{SHEN_NAME(a047)}, AREA_0362_PROP_AA76{SHEN_NAME(aa76)}, AREA_0362_LIGHT{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0362_DESTINATION{SHEN_NAME(0278)};
inline constexpr shen_name AREA_0362_CHARACTERS[]{AREA_0362_KUN, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0362_CONVERSATION[] = "0362";
struct Area0362AuthorParameter {
    Area0273Parameter *parent;
    std::uint16_t frame;
};
static_assert(sizeof(Area0362AuthorParameter) == 0x10 && offsetof(Area0362AuthorParameter, frame) == 8);
void Initialize0362();
void Start0362(std::uintptr_t argument);
void Update0362(void *taskData);
void Setup0362();
void Cleanup0362(Area0273Parameter *parent);
std::int64_t Update0362Author(Area0362AuthorParameter *child);
void Update0362AuthorTask(void *taskData);
void Prepare0362Characters();
void Restore0362Characters();
extern const ScriptAreaDefinition area0362ScriptDefinition;

} // namespace shenmue::s2::areas
