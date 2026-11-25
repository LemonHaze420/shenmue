#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_utility_dc.h"
#define AREA_0502_MAP SHEN_NAME4('K', 'R', 'H', '1')
#define AREA_0502_EVENT TASK_EVNT
#define AREA_0502_EVENT_TOKEN TASK_EVNT
typedef const int *Area0502StartParameters;
typedef signed char Area0502SplashIndex;
#define AREA_0502_RYO SHEN_NAME4('R', 'Y', 'O', '_')
typedef struct Area0502Parameter {
    HLibTask *self;
    void *motion;
    int selection;
    short state, reserved0e, reserved10;
    const int *request;
    void *voice;
} Area0502Parameter;
typedef struct Area0502AuthorParameter {
    Area0502Parameter *parent;
} Area0502AuthorParameter;
extern shen_name AREA_0502_CHARACTERS[];
void Start0502(const int *argument);
void Update0502(void *data);
void Queue0502Author(Area0502Parameter *parent);
void Update0502Author(void *data);
void Create0502Splash(signed char index);
void Update0502Splash(void *data);
void Setup0502Environment(void);
void Cleanup0502(Area0502Parameter *parent);
void Cleanup0502Task(void *data);
void Prepare0502Characters(void);
void Restore0502Characters(void);
void Prepare0502Character(shen_name character);
void Restore0502Character(shen_name character);
void Hide0502Prop(shen_name character);
void Restore0502Prop(shen_name character);
void Update0502Waves(void *data);
#else
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/math.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2 {
struct ConversationBuffer;
}
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0502_MAP{SHEN_NAME(KRH1)}, AREA_0502_EVENT{SHEN_NAME(EVNT)};
using Area0502StartParameters = std::uintptr_t;
using Area0502SplashIndex = std::int32_t;
inline constexpr std::int32_t AREA_0502_EVENT_TOKEN = static_cast<std::int32_t>(AREA_0502_EVENT.name);
inline constexpr shen_name AREA_0502_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0502_CHARACTERS[]{SHEN_NAME(AA1_), SHEN_NAME(HAK_), SHEN_NAME(KNT_), SHEN_NAME(LYO_), SHEN_NAME(SYO_),
                                                  SHEN_NAME(UGN_), SHEN_NAME(CH1_), SHEN_NAME(CH5_), SHEN_NAME(CH6_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0502_PROPS[]{SHEN_NAME(a143), SHEN_NAME(a205), SHEN_NAME(a443), SHEN_NAME(a444), SHEN_NAME(a445), SHEN_NAME(a219)};
inline constexpr Vector3 AREA_0502_SPLASH_POSITIONS[]{
    {1434.160034f, 99.3199997f, 792.450012f}, {1435.219971f, 100.4000015f, 791.609985f}, {1434.0f, 99.0999985f, 792.429993f}};
struct Area0502Parameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int32_t selection, state;
    std::int32_t reserved14;
    const std::int32_t *request;
    ConversationBuffer *voice;
};
struct Area0502AuthorParameter {
    Area0502Parameter *parent;
};
static_assert(sizeof(Area0502Parameter) == 40 && sizeof(Area0502AuthorParameter) == 8);
static_assert(offsetof(Area0502Parameter, state) == 16 && offsetof(Area0502Parameter, request) == 24);
static_assert(offsetof(Area0502Parameter, voice) == 32);
void Initialize0502();
void Start0502(std::uintptr_t argument);
void Update0502(void *data);
void Setup0502Environment();
void Update0502Waves(void *data);
void Queue0502Author(Area0502Parameter *parent);
void Update0502Author(void *data);
void Prepare0502Characters();
void Restore0502Characters();
void Create0502Splash(std::int32_t index);
void Update0502Splash(void *data);
void Cleanup0502(Area0502Parameter *parent);
extern const ScriptAreaDefinition area0502ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
