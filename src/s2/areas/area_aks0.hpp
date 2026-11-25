#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"
#include "s2/areas/common/et_darts.h"
#include "s2/areas/common/et_npc_load.h"
#include "s2/areas/area_entities.hpp"
#define disableAreaWeatherMask 0x780
#define disableGlobalLightingMask 0x0c
#define nightTimeFlag 0xff
#define primaryDartsFlag 0x393
#define alternateDartsFlag 0x33e
#define suppressAreaAudioPlayerFlag 1015
#define defaultLight LIGHT_DEFAULT_DEFINITION
#define dartsFieldEntity SHEN_NAME4('D', 'A', 'R', 'T')
void AreaMain(unsigned int argument);
void AreaCleanup(int afternoon);
#else

#include "s2/area_event.hpp"
#include "s2/npc_package.hpp"

namespace shenmue::s2::areas {

inline constexpr ConditionalNpcLoadEntry areaAKS0DartsNpcLoads[]{
    {.character = SHEN_NAME(XHO_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = SHEN_NAME(HLN_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = shen_name{0xffffffffU}}};

void AreaAKS0Init();
void AreaAKS0Main(std::uint32_t startParameters);
void AreaAKS0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAKS0GameplayDefinition;

} // namespace shenmue::s2::areas

#endif
