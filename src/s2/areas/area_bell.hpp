#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_library_tail_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/areas/area_loop_dc.h"

#define BELL_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define BELL_REN SHEN_NAME4('R', 'E', 'N', '_')
#define BELL_YANG SHEN_NAME4('Y', 'A', 'N', '_')
#define BELL_ANCHOR SHEN_NAME4('a', '2', '1', '2')
#define BELL_SHUTTER SHEN_NAME4('a', '0', '4', '7')
#define BELL_SIGN SHEN_NAME4('a', '2', '5', '7')
#define BELL_DOOR SHEN_NAME4('R', '1', '0', '7')
#define BELL_TARGET SHEN_NAME4('E', '0', '0', '0')
#define BELL_RETURN_SCENE SHEN_NAME4('Q', 'B', '0', '0')
#define BELL_AUTHOR_START SHEN_NAME4('B', 'E', 'L', '0')
#define BELL_AUTHOR_RING SHEN_NAME4('B', 'E', 'L', '1')
#define BELL_AUTHOR_YANG SHEN_NAME4('B', 'E', 'L', '2')
#define BELL_RESTING_HAND_POSE 27
#define BELL_RINGING_HAND_POSE 12
#define BELL_POSITION_FLAGS 0x78000000U
#define BELL_ROTATION_FLAGS 0x38000000U
#define BELL_SET_GRAVITY(name, enabled) do { FTBL.EV_SetCharaControllerFlags(name, 2, enabled); FTBL.EV_SetCharaControllerFlags(name, 3, enabled); } while (0)

typedef struct BellWork {
    int reserved00;
    HLibTask *task;
    void *motion;
    unsigned int phase, elapsed, command;
    int renRouteCompletion;
    unsigned char busy, escapeTalks, conversations, reserved1f;
} BellWork;

typedef struct BellSearchTarget {
    shen_name name;
    unsigned int flags, field08;
    Vector3 position;
    float lockonAngle;
    short yaw;
    short yawLimit;
} BellSearchTarget;

extern const char bellAuthorPak[], bellYangAuthorPak[], bellPackage[];
void BellMain(const unsigned int *argument);
void BellLoadNpcCallback(void *data);
void BellInteractionInitCallback(void *data);
void BellInteractionCallback(void *data);
void BellRingCallback(void *data);
void BellRestoreControl(void *data);
void BellEyeTowardBell(void *data);
void BellResetEye(void *data);
void BellExitCallback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct BellEventWork {
    std::uint64_t reserved00;
    HLib::Task *task;
    std::int32_t motion;
    std::uint32_t phase;
    std::uint32_t elapsed;
    std::uint32_t command;
    std::uint32_t renRouteCompletion;
    std::uint8_t busy;
    std::uint8_t escapeTalks;
    std::uint8_t conversations;
    std::uint8_t reserved27;
};
static_assert(sizeof(BellEventWork) == 0x28);
static_assert(offsetof(BellEventWork, task) == 8);
static_assert(offsetof(BellEventWork, command) == 0x1c);
static_assert(offsetof(BellEventWork, busy) == 0x24);
void BELL_init_func();
void BELL_main_func(std::uintptr_t argument);
void BellLoadNpcCallback(void *);
void BellInteractionInitCallback(void *);
void BellInteractionCallback(void *);
void BellRingCallback(void *);
void BellExitCallback(void *);
extern const ScriptAreaDefinition areaBELLScriptDefinition;
} // namespace shenmue::s2
#endif
