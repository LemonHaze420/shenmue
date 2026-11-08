#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define AREA_0001_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_0001_FIGURE_FIRST SHEN_NAME4('a', '0', '7', '7')
#define AREA_0001_FIGURE_SECOND SHEN_NAME4('a', '0', '6', '7')
#define AREA_0001_PRESS_MODEL SHEN_NAME4('a', '0', '8', '8')
#define AREA_0001_A075 SHEN_NAME4('a', '0', '7', '5')
#define AREA_0001_A076 SHEN_NAME4('a', '0', '7', '6')
#define AREA_0001_A079 SHEN_NAME4('a', '0', '7', '9')
#define AREA_0001_A083 SHEN_NAME4('a', '0', '8', '3')
#define AREA_0001_A084 SHEN_NAME4('a', '0', '8', '4')
#define AREA_0001_A085 SHEN_NAME4('a', '0', '8', '5')
#define AREA_0001_A086 SHEN_NAME4('a', '0', '8', '6')
#define AREA_0001_A089 SHEN_NAME4('a', '0', '8', '9')
#define AREA_0001_A090 SHEN_NAME4('a', '0', '9', '0')
extern const shen_name area0001AuthorCharacters[];
extern const shen_name area0001CleanupCharacters[];

typedef struct Title0001TaskParameter {
    HLibTask *self;
    short state;
    short mainState;
} Title0001TaskParameter;
typedef struct Title0001AuthorTaskParameter {
    Title0001TaskParameter *parent;
    short currentFrame;
    short blinkFrames;
    short blinkVisible;
} Title0001AuthorTaskParameter;
typedef struct Title0001AlphaTaskParameter {
    shen_name character;
    short increasing;
    short remaining;
    float current;
    float target;
    float step;
} Title0001AlphaTaskParameter;
typedef struct Title0001LightingTaskParameter {
    unsigned short firstRemaining;
    unsigned short secondRemaining;
    short firstState;
    short secondState;
    float firstDelta;
    float secondDelta;
    float firstRange;
    float secondRange;
    float firstUpper;
    float secondUpper;
    float firstLower;
    float secondLower;
} Title0001LightingTaskParameter;

void Area0001Main(void);
void Update0001(void *taskData);
void Queue0001Author(Title0001TaskParameter *parent);
void Update0001Author(void *taskData);
void Update0001Prompt(void *taskData);
void Queue0001Alpha(shen_name character, short frames, float from, float to, short increasing);
void Update0001Alpha(void *taskData);
void Queue0001Lighting(void);
void Update0001Lighting(void *taskData);
void Setup0001(void);
void Prepare0001Characters(void);
void Present0001PromptCharacters(void);
void Clear0001Characters(void);
void Cleanup0001Characters(void);
void Prepare0001Character(shen_name character);
void Hide0001Character(shen_name character);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/screen_film.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0001_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0001_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0001_HIDDEN_MODEL{SHEN_NAME(TMTM)};
inline constexpr shen_name AREA_0001_FIGURE_FIRST{SHEN_NAME(a077)};
inline constexpr shen_name AREA_0001_FIGURE_SECOND{SHEN_NAME(a067)};
inline constexpr shen_name AREA_0001_PRESS_MODEL{SHEN_NAME(aa78)};
inline constexpr shen_name AREA_0001_LAST_MODEL{SHEN_NAME(a088)};
inline constexpr std::array<shen_name, 10> AREA_0001_AUTHOR_CHARACTERS{SHEN_NAME(a084), SHEN_NAME(a085), SHEN_NAME(a083), SHEN_NAME(a089), SHEN_NAME(a090),
    SHEN_NAME(a079), SHEN_NAME(a086), SHEN_NAME(a075), SHEN_NAME(a076), shen_name{0xffffffffU}};
inline constexpr std::array<shen_name, 10> AREA_0001_CLEANUP_CHARACTERS{SHEN_NAME(a084), SHEN_NAME(a085), SHEN_NAME(a089), SHEN_NAME(a090), SHEN_NAME(a086),
    SHEN_NAME(aa78), SHEN_NAME(a088), SHEN_NAME(a091), SHEN_NAME(TMTM), shen_name{0xffffffffU}};
inline constexpr std::array<shen_name, 6> AREA_0001_CLEAR_CHARACTERS{
    SHEN_NAME(a083), SHEN_NAME(a079), SHEN_NAME(a075), SHEN_NAME(a076), SHEN_NAME(a067), SHEN_NAME(a077)};
inline constexpr std::array<shen_name, 2> AREA_0001_FOREGROUND_CHARACTERS{SHEN_NAME(a067), SHEN_NAME(a077)};
inline constexpr GlowParameters AREA_0001_AUTHOR_GLOW{10, 0.0f, {0.0f, 0.0f, 0.0f, 0.225f}};
inline constexpr GlowParameters AREA_0001_PROMPT_GLOW{10, 0.0f, {0.0f, 0.0f, 0.0f, 0.15f}};

struct Title0001TaskParameter {
    HLib::Task *self;
    std::int16_t state;
    std::int16_t mainState;
    std::byte reserved0c[4];
};
struct Title0001AuthorTaskParameter {
    Title0001TaskParameter *parent;
    std::int16_t currentFrame;
    std::byte reserved0a[2];
    float elapsedFrames;
    std::int16_t active;
    std::byte reserved12[6];
};
struct Title0001LightingTaskParameter {
    std::int16_t firstRemaining;
    std::int16_t secondRemaining;
    std::int16_t firstState;
    std::int16_t secondState;
    float firstDelta;
    float secondDelta;
    float firstRange;
    float secondRange;
    float firstUpper;
    float secondUpper;
    float firstLower;
    float secondLower;
};
struct Title0001AlphaTaskParameter {
    shen_name character;
    std::int16_t increasing;
    std::int16_t remaining;
    float current;
    float target;
    float step;
};
static_assert(sizeof(Title0001TaskParameter) == 16);
static_assert(offsetof(Title0001TaskParameter, mainState) == 10);
static_assert(sizeof(Title0001AuthorTaskParameter) == 24);
static_assert(offsetof(Title0001AuthorTaskParameter, elapsedFrames) == 12);
static_assert(offsetof(Title0001AuthorTaskParameter, active) == 16);
static_assert(sizeof(Title0001LightingTaskParameter) == 40);
static_assert(offsetof(Title0001LightingTaskParameter, firstRange) == 16);
static_assert(sizeof(Title0001AlphaTaskParameter) == 20);
static_assert(offsetof(Title0001AlphaTaskParameter, remaining) == 6);
extern bool g_titleIntroPressedStart;
void NotifyTitleIntroPressStart();
void Area0001Init();
void Area0001Main(std::uintptr_t argument);
void Update0001(void *taskData);
void Setup0001();
std::int64_t Queue0001Author(Title0001TaskParameter *parent);
void Update0001Author(void *taskData);
void Update0001Prompt(void *taskData);
void Prepare0001Characters();
void Clear0001Characters();
void Cleanup0001Characters();
Title0001LightingTaskParameter *Queue0001Lighting();
void Update0001Lighting(void *taskData);
Title0001AlphaTaskParameter *Queue0001Alpha(shen_name character, std::int64_t unused, float from, float to);
void Update0001Alpha(void *taskData);
void Finish0001Noop(void *taskData);
extern const ScriptAreaDefinition area0001ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
