#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"
#define KWWT_RYO RYO_CHARACTER
#define KWWT_SHENHUA SHEN_NAME4('S', 'I', 'N', '_')
#define KWWT_PROP SHEN_NAME4('a', '6', '3', '6')
#define KWWT_FIRE_SOURCE SHEN_NAME4('a', '2', '8', '7')
#define KWWT_FIRE_TARGET SHEN_NAME4('a', '2', '8', '8')
#define KWWT_RETURN_AREA SHEN_NAME4('K', 'E', 'S', '1')
#define KWWT_TASK TASK_EVNT
typedef struct KWWTTaskParameter {
    HLibTask *self;
    unsigned int reserved04;
    int selection;
    short state;
    short reserved0e;
} KWWTTaskParameter;
typedef struct KWWTLoopParameter {
    KWWTTaskParameter *parent;
    short currentFrame;
    short reserved06;
} KWWTLoopParameter;
extern const shen_name KWWT_CHARACTERS[];
extern const unsigned short KWWT_FIRE_PALETTE[];
extern const shen_name KWWT_AUTHORS[];
void AreaKWWTMain(int argument);
void KWWTAuthorMainCallback(void *parameter);
void QueueKWWTAuthorLoop(KWWTTaskParameter *parameter);
void KWWTAuthorLoopCallback(void *parameter);
void KWWTAdvanceAuthorCallback(void *parameter);
void InitializeKWWTFires(void);
void KWWTPrepareAuthorCharacters(void);
void KWWTRestoreAuthorCharacters(void);
void KWWTFinishEvent(KWWTTaskParameter *parameter);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name KWWT_TASK{SHEN_NAME(EVNT)}, KWWT_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name KWWT_RYO{SHEN_NAME(RYO_)}, KWWT_SHENHUA{SHEN_NAME(SIN_)}, KWWT_PROP{SHEN_NAME(a636)};
inline constexpr shen_name KWWT_FIRE_SOURCE{SHEN_NAME(a287)}, KWWT_FIRE_TARGET{SHEN_NAME(a288)}, KWWT_RETURN_AREA{SHEN_NAME(KES1)};
inline constexpr shen_name KWWT_CHARACTERS[]{KWWT_SHENHUA, shen_name{0xffffffffU}};
inline constexpr shen_name KWWT_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01), SHEN_NAME(AT02), SHEN_NAME(AT03)};
inline constexpr std::uint16_t KWWT_FIRE_PALETTE[]{
#include "area_kwwt_palette.inc"
};
struct KWWTTaskParameter {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::int32_t selection;
    std::int16_t state;
    std::byte reserved12[6];
};
struct KWWTLoopParameter {
    KWWTTaskParameter *parent;
    std::int16_t currentFrame;
    std::byte reserved0a[6];
};
static_assert(sizeof(KWWTTaskParameter) == 24);
static_assert(offsetof(KWWTTaskParameter, selection) == 12);
static_assert(offsetof(KWWTTaskParameter, state) == 16);
static_assert(sizeof(KWWTLoopParameter) == 16);
static_assert(offsetof(KWWTLoopParameter, currentFrame) == 8);

void AreaKWWTInit();
void AreaKWWTMain(std::uintptr_t argument);
void KWWTAuthorMainCallback(void *parameter);
void QueueKWWTAuthorLoop(KWWTTaskParameter *parameter);
void KWWTAuthorLoopCallback(void *parameter);
void KWWTAdvanceAuthorCallback(void *parameter);
void InitializeKWWTFires();
void KWWTPrepareAuthorCharacters();
void KWWTRestoreAuthorCharacters();
void KWWTFinishEvent(KWWTTaskParameter *parameter);
extern const ScriptAreaDefinition areaKWWTScriptDefinition;

} // namespace shenmue::s2::areas

#endif
