#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"

#define AREA_0175_AREA SHEN_NAME4('0', '1', '7', '5')
#define AREA_0175_RYO_ SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_0175_JOP_ SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0175_WON_ SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_0175_A396 SHEN_NAME4('a', '3', '9', '6')
#define AREA_0175_A417 SHEN_NAME4('a', '4', '1', '7')
#define AREA_0175_AB00 SHEN_NAME4('A', 'B', '0', '0')
#define AREA_0175_AR02 SHEN_NAME4('A', 'R', '0', '2')
#define AREA_0175_AUTHOR_A SHEN_NAME4('1', '7', '5', 'A')
#define AREA_0175_AUTHOR_B SHEN_NAME4('1', '7', '5', 'B')

typedef struct E0175LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
} E0175LoopParameter;

void E0175Main(void *argument);
void E0175MainCallback(void *taskData);
void QueueE0175Loop(AreaImmediateAuthorTaskParameter *parent);
void E0175LoopCallback(void *taskData);
void ConfigureE0175Environment(void);
void CleanupE0175(AreaImmediateAuthorTaskParameter *parent);
void SetupE0175Characters(void);
void RestoreE0175Characters(void);
void SetupE0175AuthorCharacter(shen_name character);
void RestoreE0175AuthorCharacter(shen_name character);
void HideE0175Character(shen_name character);
void ShowE0175Character(shen_name character);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0175_AREA{SHEN_NAME(0175)};
inline constexpr shen_name AREA_0175_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0175_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0175_A396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0175_A417{SHEN_NAME(a417)};
inline constexpr shen_name AREA_0175_CHARACTERS[]{AREA_0175_JOP_, AREA_0175_WON_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0175_MOTION[] = "0175A.MOT", AREA_0175_CONVERSATION[] = "0175";
inline constexpr shen_name AREA_0175_AB00{SHEN_NAME(AB00)}, AREA_0175_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0175_AUTHOR_A{SHEN_NAME(175A)}, AREA_0175_AUTHOR_B{SHEN_NAME(175B)};
struct E0175TaskParameter {
    HLib::Task *self;
    std::int16_t selection, state;
    std::byte reserved0c[4];
};
static_assert(sizeof(E0175TaskParameter) == 0x10);
struct E0175LoopParameter {
    E0175TaskParameter *parent;
};
static_assert(sizeof(E0175LoopParameter) == 8);
extern const ScriptAreaDefinition area0175ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
