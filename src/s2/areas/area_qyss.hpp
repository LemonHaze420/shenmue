#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_library_tail_dc.h"
#include "s2/sk_event_dc.h"

#define QYSS_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define QYSS_REN SHEN_NAME4('R', 'E', 'N', '_')
#define QYSS_YUAN SHEN_NAME4('Y', 'U', 'A', '_')
#define QYSS_OHS SHEN_NAME4('O', 'H', 'S', '_')
#define QYSS_JIA SHEN_NAME4('J', 'I', 'A', '_')
#define QYSS_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define QYSS_ELEVATOR SHEN_NAME4('R', '5', '9', '7')
#define QYSS_CYCLE SHEN_NAME4('c', 'y', 'l', 'f')
#define QYSS_QAE6 SHEN_NAME4('Q', 'A', 'E', '6')
#define QYSS_AUTHOR_START SHEN_NAME4('S', 'T', 'R', 'T')
#define QYSS_AUTHOR_ELV1 SHEN_NAME4('E', 'L', 'V', '1')
#define QYSS_AUTHOR_ELV2 SHEN_NAME4('E', 'L', 'V', '2')
#define QYSS_DEBUG_ID 3

typedef struct QyssStalkWork {
    HLibTask *self;
    short authorState, state;
    EventReadCharactersWork *characters;
} QyssStalkWork;

extern int qyssEntryMode;
void QyssMain(int argument);
void QueueQyssStalkCallback(void *data);
int InitializeQyssStalk(QyssStalkWork *work);
int FinishQyssStalk(QyssStalkWork *work);
void QyssEndStalkCallback(void *data);
void QyssStalkCallback(void *data);
int ChooseQyssWaiting(QyssStalkWork *work);
int FadeQyssWaiting(QyssStalkWork *work);
int PlayQyssElevatorAuthor(QyssStalkWork *work);
void QyssElevatorAuthorFrameCallback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "common/math.hpp"
#include <cstddef>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name QYSS_EVENT_TASK{SHEN_NAME(EVNT)}, QYSS_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name QYSS_REN{SHEN_NAME(REN_)}, QYSS_YUAN{SHEN_NAME(YUA_)}, QYSS_OHS{SHEN_NAME(OHS_)};
inline constexpr shen_name QYSS_JIA{SHEN_NAME(JIA_)}, QYSS_BASE{SHEN_NAME(BASE)}, QYSS_INVALID{0xffffffffU};
inline constexpr shen_name QYSS_ELEVATOR{SHEN_NAME(R597)}, QYSS_CYCLE{SHEN_NAME(cylf)}, QYSS_QAE6{SHEN_NAME(QAE6)};
inline constexpr shen_name QYSS_AUTHOR_START{SHEN_NAME(STRT)}, QYSS_AUTHOR_ELV1{SHEN_NAME(ELV1)}, QYSS_AUTHOR_ELV2{SHEN_NAME(ELV2)};
inline constexpr shen_name QYSS_CHARACTERS[]{QYSS_PLAYER, QYSS_REN, QYSS_YUAN, QYSS_OHS, QYSS_JIA, QYSS_INVALID};
inline constexpr Vector3 QYSS_PLAYER_WAIT_POSITION{241.5F, 139.60000610351562F, 218.3000030517578F};
inline constexpr Vector3 QYSS_REN_WAIT_POSITION{240.5F, 139.60000610351562F, 217.5F};
inline constexpr Vector3 QYSS_YUAN_END_POSITION{235.8F, 121.6F, 219.2F};
inline constexpr Vector3 QYSS_PLAYER_END_POSITION{227.5F, 121.6F, 221.39999F};
struct QyssStalkWork {
    HLib::Task *self;
    std::int16_t authorState;
    std::int16_t state;
    std::int32_t reserved0c;
    EventReadCharactersWork *characters;
};
static_assert(sizeof(QyssStalkWork) == 0x18);
static_assert(offsetof(QyssStalkWork, authorState) == 8);
static_assert(offsetof(QyssStalkWork, characters) == 0x10);
extern std::int32_t qyssEntryMode;
void QyssInit();
void QyssMain(std::uintptr_t argument);
void QueueQyssStalkCallback(void *);
void QyssStalkCallback(void *);
void QyssEndStalkCallback(void *);
std::int32_t InitializeQyssStalk(QyssStalkWork *);
std::int32_t ChooseQyssWaiting(QyssStalkWork *);
std::int32_t FadeQyssWaiting(QyssStalkWork *);
std::int32_t PlayQyssElevatorAuthor(QyssStalkWork *);
void QyssElevatorAuthorFrameCallback(void *);
void QyssDormantNullsub();
extern const ScriptAreaDefinition qyssScriptDefinition;
} // namespace shenmue::s2
#endif
