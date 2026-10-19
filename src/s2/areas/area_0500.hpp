#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0500_MAP{SHEN_NAME(KRF1)}, AREA_0500_RETURN{SHEN_NAME(KRH1)};
inline constexpr shen_name AREA_0500_EVENT{SHEN_NAME(EVNT)}, AREA_0500_WAVE{SHEN_NAME(wave)};
inline constexpr shen_name AREA_0500_RYO{SHEN_NAME(RYO_)}, AREA_0500_BOAT{SHEN_NAME(a143)}, AREA_0500_PROP{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0500_SGT{SHEN_NAME(SGT_)}, AREA_0500_LIGHT{SHEN_NAME(l002)};
inline constexpr shen_name AREA_0500_CHARACTERS[]{SHEN_NAME(AA1_), SHEN_NAME(HAK_), SHEN_NAME(KNT_), SHEN_NAME(LYO_), SHEN_NAME(SYO_), AREA_0500_SGT,
    SHEN_NAME(UGN_), SHEN_NAME(CH1_), SHEN_NAME(CH5_), SHEN_NAME(CH6_), shen_name{0xffffffffU}};
struct Area0500Parameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int16_t selection, state;
};
struct Area0500AuthorParameter {
    Area0500Parameter *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(Area0500Parameter) == 16 && sizeof(Area0500AuthorParameter) == 16);
static_assert(offsetof(Area0500Parameter, selection) == 12 && offsetof(Area0500Parameter, state) == 14);
static_assert(offsetof(Area0500AuthorParameter, frame) == 8);
void Initialize0500();
void Start0500(std::uintptr_t argument);
void Update0500(void *data);
void Setup0500Environment();
void Queue0500Author(Area0500Parameter *parent);
void Update0500Author(void *data);
void Update0500Waves(void *data);
void Prepare0500Characters();
void Restore0500Characters();
void Cleanup0500(Area0500Parameter *parent);
extern const ScriptAreaDefinition area0500ScriptDefinition;
} // namespace shenmue::s2::areas
