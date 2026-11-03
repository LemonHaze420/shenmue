#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"

#define AREA_0200_QR00 SHEN_NAME4('Q', 'R', '0', '0')
#define AREA_0200_BASE SHEN_NAME4('b', 'a', 's', 'e')
#define AREA_0200_A888 SHEN_NAME4('a', '8', '8', '8')
#define AREA_0200_A237 SHEN_NAME4('a', '2', '3', '7')
#define AREA_0200_A238 SHEN_NAME4('a', '2', '3', '8')
#define AREA_0200_A342 SHEN_NAME4('a', '3', '4', '2')
#define AREA_0200_A343 SHEN_NAME4('a', '3', '4', '3')
#define AREA_0200_A239 SHEN_NAME4('a', '2', '3', '9')
#define AREA_0200_AB32 SHEN_NAME4('a', 'b', '3', '2')
#define AREA_0200_A814 SHEN_NAME4('a', '8', '1', '4')
#define AREA_0200_A816 SHEN_NAME4('a', '8', '1', '6')
#define AREA_0200_A818 SHEN_NAME4('a', '8', '1', '8')
#define AREA_0200_A820 SHEN_NAME4('a', '8', '2', '0')
#define AREA_0200_ROOT_POSITION_FLAGS 0x78000000U
extern unsigned int bobPhase0200, bobSpeed0200;
extern Vector3 bobPosition0200;
extern Vector3i handRotation0200;
extern shen_name area0200Characters[];
extern Vector3i fingerRotation0200;
void E0200Main(void);
void E0200MainCallback(void *taskData);
void QueueE0200Loop(AreaImmediateAuthorTaskParameter *parent);
void E0200LoopCallback(void *taskData);
void AnimateE0200(short frame);
void SetupE0200Environment(void);
void CleanupE0200(AreaImmediateAuthorTaskParameter *parent);
void SetupE0200Characters(void);
void RestoreE0200Characters(void);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0200_AREA{SHEN_NAME(0200)}, AREA_0200_QR00{SHEN_NAME(QR00)}, AREA_0200_BASE{SHEN_NAME(base)};
inline constexpr shen_name AREA_0200_A088{SHEN_NAME(a088) | 0x800U};
inline constexpr shen_name AREA_0200_A237{SHEN_NAME(a016) | 0x1020200U};
inline constexpr shen_name AREA_0200_A238{SHEN_NAME(a020) | 0x8010200U};
inline constexpr shen_name AREA_0200_A142{SHEN_NAME(a142) | 0x200U};
inline constexpr shen_name AREA_0200_A143{SHEN_NAME(a143) | 0x200U};
inline constexpr shen_name AREA_0200_A219{SHEN_NAME(a219) | 0x20000U};
inline constexpr shen_name AREA_0200_AB20{SHEN_NAME(ab20) | 0x2010000U};
inline constexpr shen_name AREA_0200_A814{SHEN_NAME(a814)};
inline constexpr shen_name AREA_0200_A816{SHEN_NAME(a816)};
inline constexpr shen_name AREA_0200_A818{SHEN_NAME(a818)};
inline constexpr shen_name AREA_0200_A820{SHEN_NAME(a820)};
inline constexpr shen_name AREA_0200_CHARACTERS[]{
    SHEN_NAME(OJ5_),
    SHEN_NAME(BUS_),
    shen_name{0xffffffffU},
};
extern const ScriptAreaDefinition area0200ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
