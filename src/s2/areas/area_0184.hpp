#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0184_AREA SHEN_NAME4('0', '1', '8', '4')
#define AREA_0184_A191 SHEN_NAME4('a', '1', '9', '1')
#define AREA_0184_A192 SHEN_NAME4('a', '1', '9', '2')
#define AREA_0184_LKN_ SHEN_NAME4('L', 'K', 'N', '_')
#define AREA_0184_BKN_ SHEN_NAME4('B', 'K', 'N', '_')
#define AREA_0184_MYS_ SHEN_NAME4('M', 'Y', 'S', '_')
#define AREA_0184_CONVERSATION "0184"
typedef struct E0184TaskParameter {
    HLibTask *self;
    unsigned int reserved08;
    short state, reserved0e;
} E0184TaskParameter;
typedef struct E0184FrameParameter {
    E0184TaskParameter *parent;
    short state;
} E0184FrameParameter;
extern shen_name AREA_0184_CHARACTERS[];
void E0184Main(void);
void E0184MainCallback(void *);
void QueueE0184Loop(E0184TaskParameter *);
void E0184LoopCallback(void *);
void Configure0184(void);
void CleanupE0184(E0184TaskParameter *);
void SetupE0184Characters(void);
void RestoreE0184Characters(void);
void Prepare0184Character(shen_name);
void Restore0184Character(shen_name);
void Hide0184Prop(shen_name);
void Show0184Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0184_AREA{SHEN_NAME(0184)};
inline constexpr shen_name AREA_0184_A191{SHEN_NAME(a191)};
inline constexpr shen_name AREA_0184_A192{SHEN_NAME(a192)};
inline constexpr shen_name AREA_0184_LKN_{SHEN_NAME(LKN_)};
inline constexpr shen_name AREA_0184_BKN_{SHEN_NAME(BKN_)};
inline constexpr shen_name AREA_0184_MYS_{SHEN_NAME(MYS_)};
inline constexpr shen_name AREA_0184_CHARACTERS[]{AREA_0184_LKN_, AREA_0184_BKN_, AREA_0184_MYS_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0184_MOTION[] = "0184A.MOT", AREA_0184_CONVERSATION[] = "0184";
struct E0184TaskParameter {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::int16_t state;
    std::int16_t reserved0e;
};
struct E0184FrameParameter {
    E0184TaskParameter *parent;
    std::int16_t state;
};
static_assert(sizeof(E0184TaskParameter) == 0x10);
static_assert(offsetof(E0184TaskParameter, state) == 0x0c);
static_assert(sizeof(E0184FrameParameter) == 0x10);

extern const ScriptAreaDefinition area0184ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
