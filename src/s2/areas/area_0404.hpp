#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0404_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0404_RYO{SHEN_NAME(RYO_)}, AREA_0404_SIN{SHEN_NAME(SIN_)}, AREA_0404_PROP{SHEN_NAME(a067)};
inline constexpr shen_name AREA_0404_RETURN_MAP{SHEN_NAME(KRC1)};
inline constexpr shen_name AREA_0404_CHARACTERS[]{AREA_0404_SIN, shen_name{0xffffffffU}};
struct Area0404Parameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int16_t selection, state, loadState, characterIndex;
    std::byte reserved14[4];
};
struct Area0404AuthorParameter {
    Area0404Parameter *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(Area0404Parameter) == 24 && offsetof(Area0404Parameter, state) == 14);
static_assert(offsetof(Area0404Parameter, loadState) == 16 && sizeof(Area0404AuthorParameter) == 16);
void Initialize0404();
void Start0404(std::uintptr_t argument);
void Load0404Characters(void *parameter);
void Exit0404Characters(void *parameter);
void Update0404(void *parameter);
void Setup0404Environment();
void Cleanup0404(Area0404Parameter *parent);
void Queue0404Author(Area0404Parameter *parent);
void Update0404Author(void *parameter);
void Prepare0404Character(shen_name name);
void Restore0404Characters();
void Update0404Waves(void *parameter);
bool Is0404ArgumentZero(std::int32_t argument);
extern const ScriptAreaDefinition area0404ScriptDefinition;
} // namespace shenmue::s2::areas
