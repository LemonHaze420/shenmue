#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0201_AREA SHEN_NAME4('0', '2', '0', '1')
#define AREA_0201_ANP1 SHEN_NAME4('A', 'N', 'P', '1')
#define AREA_0201_QRR0 SHEN_NAME4('Q', 'R', 'R', '0')
#define AREA_0201_CLJ_ SHEN_NAME4('C', 'L', 'J', '_')
#define AREA_0201_A209 SHEN_NAME4('a', '2', '0', '9')
#define AREA_0201_A207 SHEN_NAME4('a', '2', '0', '7')
#define AREA_0201_A208 SHEN_NAME4('a', '2', '0', '8')
#define AREA_0201_A270 SHEN_NAME4('a', '2', '7', '0')
#define AREA_0201_A326 SHEN_NAME4('a', '3', '2', '6')
#define AREA_0201_A014 SHEN_NAME4('a', '0', '1', '4')
#define AREA_0201_DESK SHEN_NAME4('D', 'E', 'S', 'K')
#define AREA_0201_LAMP SHEN_NAME4('L', 'A', 'M', 'P')
#define AREA_0201_A358 SHEN_NAME4('a', '3', '5', '8')
#define AREA_0201_LIGHT SHEN_NAME4('l', '1', '0', '1')
#define AREA_0201_QR00 SHEN_NAME4('Q', 'R', '0', '0')
typedef AreaImmediateAuthorTaskParameter E0201TaskParameter;
typedef struct E0201LoopParameter {
    E0201TaskParameter *parent;
} E0201LoopParameter;
extern shen_name AREA_0201_CHARACTERS[];
void E0201Main(void *argument);
void E0201MainCallback(void *);
void QueueE0201Loop(E0201TaskParameter *);
void E0201LoopCallback(void *);
void Configure0201(void);
void CleanupE0201(E0201TaskParameter *);
void SetupE0201Characters(short);
void RestoreE0201Characters(short);
void Prepare0201Character(shen_name);
void Restore0201Character(shen_name);
void Hide0201Prop(shen_name);
void Show0201Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0201_AREA{SHEN_NAME(0201)};
inline constexpr shen_name AREA_0201_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0201_QRR0{SHEN_NAME(QRR0)};
inline constexpr shen_name AREA_0201_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0201_A209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0201_A207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0201_A208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0201_A270{SHEN_NAME(a270)};
inline constexpr shen_name AREA_0201_A326{SHEN_NAME(a326)};
inline constexpr shen_name AREA_0201_A014{SHEN_NAME(a014)};
inline constexpr shen_name AREA_0201_DESK{SHEN_NAME(DESK)};
inline constexpr shen_name AREA_0201_LAMP{SHEN_NAME(LAMP)};
inline constexpr shen_name AREA_0201_A358{SHEN_NAME(a358)};
inline constexpr shen_name AREA_0201_CHARACTERS[]{AREA_0201_CLJ_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0201_CONVERSATION[] = "0201";

struct E0201TaskParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
static_assert(sizeof(E0201TaskParameter) == 0x10);
struct E0201LoopParameter {
    E0201TaskParameter *parent;
};
static_assert(sizeof(E0201LoopParameter) == 8);
inline constexpr shen_name AREA_0201_LIGHT{SHEN_NAME(l101)}, AREA_0201_QR00{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_0201_AUTHORS[]{SHEN_NAME(REN1), SHEN_NAME(REN2)};
inline constexpr const char *AREA_0201_DEBUG_NAMES[]{"0201A000.c\n", "0201A010.c\n"};
extern const ScriptAreaDefinition area0201ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
