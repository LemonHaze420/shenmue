#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"

#define AREA_0250_NEXT SHEN_NAME4('0', '2', '5', '2')
#define AREA_0250_LIGHT SHEN_NAME4('l', '0', '0', '2')
#define AREA_0250_AB07 SHEN_NAME4('a', 'b', '0', '7')
#define AREA_0250_A297 SHEN_NAME4('a', '2', '9', '7')
#define AREA_0250_A046 SHEN_NAME4('a', '0', '4', '6')
#define AREA_0250_AB33 SHEN_NAME4('a', 'b', '3', '3')
#define AREA_0250_AB34 SHEN_NAME4('a', 'b', '3', '4')
#define AREA_0250_AB35 SHEN_NAME4('a', 'b', '3', '5')
#define AREA_0250_AB36 SHEN_NAME4('a', 'b', '3', '6')
#define AREA_0250_F002 SHEN_NAME4('F', '0', '0', '2')
#define AREA_0250_R101 SHEN_NAME4('R', '1', '0', '1')
#define AREA_0250_R102 SHEN_NAME4('R', '1', '0', '2')
#define AREA_0250_R103 SHEN_NAME4('R', '1', '0', '3')
#define AREA_0250_R104 SHEN_NAME4('R', '1', '0', '4')
#define AREA_0250_R105 SHEN_NAME4('R', '1', '0', '5')
#define AREA_0250_R143 SHEN_NAME4('R', '1', '4', '3')
#define AREA_0250_R144 SHEN_NAME4('R', '1', '4', '4')
#define AREA_0250_R145 SHEN_NAME4('R', '1', '4', '5')
#define AREA_0250_R208 SHEN_NAME4('R', '2', '0', '8')
#define AREA_0250_R209 SHEN_NAME4('R', '2', '0', '9')
#define AREA_0250_SYP_ SHEN_NAME4('S', 'Y', 'P', '_')
#define AREA_0250_WON_ SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_0250_SDJ_ SHEN_NAME4('S', 'D', 'J', '_')
extern const shen_name area0250Characters[];
extern const unsigned short area0250FirePalette[];
void E0250Main(void);
void E0250MainCallback(void *taskData);
void QueueE0250Loop(AreaImmediateAuthorTaskParameter *parent);
void SetE0250SceneVisibility(char selection);
void E0250LoopCallback(void *taskData);
void SetupE0250Environment(void);
void CleanupE0250(AreaImmediateAuthorTaskParameter *parent);
void SetupE0250Characters(short selection);
void RestoreE0250Characters(short selection);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0250_AREA{SHEN_NAME(0250)}, AREA_0250_NEXT{SHEN_NAME(0252)}, AREA_0250_LIGHT{SHEN_NAME(l002)};
inline constexpr shen_name AREA_0250_AB07{SHEN_NAME(ab07)}, AREA_0250_A297{SHEN_NAME(a297)}, AREA_0250_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0250_PROPS[]{SHEN_NAME(ab33), SHEN_NAME(ab34), SHEN_NAME(ab35), SHEN_NAME(ab36)};
inline constexpr shen_name AREA_0250_HIDDEN_PROPS[]{SHEN_NAME(F002), SHEN_NAME(R101), SHEN_NAME(R102), SHEN_NAME(R103), SHEN_NAME(R104), SHEN_NAME(R105),
    SHEN_NAME(R143), SHEN_NAME(R144), SHEN_NAME(R145), SHEN_NAME(R208), SHEN_NAME(R209)};
inline constexpr shen_name AREA_0250_CHARACTERS[]{SHEN_NAME(SYP_), SHEN_NAME(WON_), SHEN_NAME(SDJ_), shen_name{0xffffffffU}};
inline constexpr std::uint16_t AREA_0250_PALETTE[]{785, 785, 1041, 1297, 1554, 1810, 2610, 2866, 2882, 3152, 3440, 3696, 3712, 4000, 4035, 4054};
extern const ScriptAreaDefinition area0250ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
