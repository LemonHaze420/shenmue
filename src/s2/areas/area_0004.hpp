#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_utility_dc.h"

#define AREA_0004_A892 SHEN_NAME4('a', '8', '9', '2')
#define AREA_0004_AA87 SHEN_NAME4('a', 'a', '8', '7')
#define AREA_0004_AA88 SHEN_NAME4('a', 'a', '8', '8')
#define AREA_0004_AA90 SHEN_NAME4('a', 'a', '9', '0')
#define AREA_0004_AA91 SHEN_NAME4('a', 'a', '9', '1')
#define AREA_0004_AA92 SHEN_NAME4('a', 'a', '9', '2')
#define AREA_0004_A259 SHEN_NAME4('a', '2', '5', '9')
#define AREA_0004_A814 SHEN_NAME4('a', '8', '1', '4')
#define AREA_0004_A816 SHEN_NAME4('a', '8', '1', '6')
#define AREA_0004_A818 SHEN_NAME4('a', '8', '1', '8')
#define AREA_0004_A820 SHEN_NAME4('a', '8', '2', '0')
#define AREA_0004_A822 SHEN_NAME4('a', '8', '2', '2')
#define AREA_0004_A954 SHEN_NAME4('a', '9', '5', '4')
#define AREA_0004_AA08 SHEN_NAME4('a', 'a', '0', '8')
#define AREA_0004_AA10 SHEN_NAME4('a', 'a', '1', '0')
#define AREA_0004_A796 SHEN_NAME4('a', '7', '9', '6')
#define AREA_0004_AR01 SHEN_NAME4('A', 'R', '0', '1')
#define AREA_0004_WAVE SHEN_NAME4('w', 'a', 'v', 'e')
#define AREA_0004_INVALID_EFFECT 0xffffffffU

typedef struct Area0004EventWork {
    HLibTask *task;
    int debugSelection;
    short state;
} Area0004EventWork;
typedef struct Area0004LoopWork {
    Area0004EventWork *parent;
    short state;
} Area0004LoopWork;

typedef struct Area0004WaveColours {
    unsigned int reserved00[5];
    unsigned int specular;
    unsigned int reserved18;
    unsigned int diffuse;
} Area0004WaveColours;

extern shen_name openingTemporaryCharacters[];
void e0004_EVT_Auth_Main(void);
void e0004_EVT_Auth_Main_Callback(void *taskData);
void Queue_e0004_EVT_Auth_Loop_0004A000(Area0004EventWork *parent);
void e0004_EVT_Auth_Loop_0004A000_Callback(void *taskData);
void SetupOpeningSmoke(void);
void UpdateOpeningWave(void);
void InitializeOpeningEnvironment(void);
void CompleteOpening(Area0004EventWork *event);
void PrepareOpeningAuthorCharacters(void);
void RestoreOpeningAuthorCharacters(void);
void PrepareOpeningTemporaryCharacter(shen_name character);
void RestoreOpeningTemporaryCharacter(shen_name character);
void PrepareOpeningAuthorCharacter(shen_name character);
void RestoreOpeningAuthorCharacter(shen_name character);
void e0004_ET_WAVE_AR00_Callback(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"

#include <array>
#include <cstdint>
namespace shenmue::s2::areas {

inline constexpr std::array<std::uint16_t, 5> OPENING_TEXTURE_PHASE_OFFSETS{0, 10, 7, 12, 5};
inline constexpr std::array<const char *, 11> OPENING_AUTHOR_DEBUG_NAMES{"0004A001.c\n", "0004A089.c\n", "0004A090.c\n", "0004A091.c\n", "0004A092.c\n",
    "0004A093.c\n", "0004A094.c\n", "0004A095.c\n", "0004A096.c\n", "0004A097.c\n", "0004A098.c\n"};
extern const ScriptAreaDefinition area0004ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
