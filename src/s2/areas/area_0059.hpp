#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_sleep_dc.h"

#define AREA_0059_AB00 SHEN_NAME4('A', 'B', '0', '0')
#define AREA_0059_WON SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_0059_A021 ((shen_name)(SHEN_NAME4('a', '0', '2', '1') | 0x100U))
#define AREA_0059_A126 SHEN_NAME4('a', '1', '2', '6')
#define AREA_0059_A005 ((shen_name)(SHEN_NAME4('a', '0', '0', '5') | 0x20100U))
#define AREA_0059_A020 ((shen_name)(SHEN_NAME4('a', '0', '2', '0') | 0x4000100U))
#define AREA_0059_A103 ((shen_name)(SHEN_NAME4('a', '1', '0', '3') | 0x20000U))
#define AREA_0059_A022 ((shen_name)(SHEN_NAME4('a', '0', '2', '2') | 0x100U))

typedef struct HarborWeatherColours {
    unsigned char reserved00[20];
    unsigned int specular, reserved18, diffuse;
} HarborWeatherColours;
typedef struct HarborWeatherValues {
    unsigned char reserved00[52];
    HarborWeatherColours colours;
} HarborWeatherValues;

typedef struct HarborMapModel {
    unsigned char reserved00[40];
    void *collision;
} HarborMapModel;
typedef struct HarborMapModelRoot {
    unsigned char reserved00[44];
    HarborMapModel *model;
} HarborMapModelRoot;
typedef struct HarborFloatingModel {
    int index;
    Vector3 position;
} HarborFloatingModel;
extern shen_name area0059Characters[];
void E0059Main(void);
void E0059MainCallback(void *taskData);
void QueueE0059Author(AreaAuthorTaskParameter *parent);
void E0059LoopCallback(void *taskData);
void SetupE0059Environment(void);
void CleanupE0059(AreaAuthorTaskParameter *parent);
void SetupE0059Characters(void);
void RestoreE0059Characters(void);
void HarborAuthorWaveCallback(void *taskData);
void HarborAuthorShip0059Callback(void *taskData);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0059_AREA{SHEN_NAME(0059)}, AREA_0059_AB00{SHEN_NAME(AB00)}, AREA_0059_WON{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0059_A021{SHEN_NAME(a021) | 0x100U};
inline constexpr shen_name AREA_0059_A126{SHEN_NAME(a126)};
inline constexpr shen_name AREA_0059_A005{SHEN_NAME(a005) | 0x20100U};
inline constexpr shen_name AREA_0059_A020{SHEN_NAME(a020) | 0x4000100U};
inline constexpr shen_name AREA_0059_A103{SHEN_NAME(a103) | 0x20000U};
inline constexpr shen_name AREA_0059_A022{SHEN_NAME(a022) | 0x100U};
inline constexpr shen_name AREA_0059_CHARACTERS[]{
    SHEN_NAME(WON_),
    shen_name{0xffffffffU},
};
extern const ScriptAreaDefinition area0059ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
