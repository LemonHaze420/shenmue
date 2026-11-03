#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"

#define VSYB_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define VSYB_REN SHEN_NAME4('R', 'E', 'N', '_')
#define VSYB_WONG SHEN_NAME4('W', 'O', 'N', '_')
#define VSYB_YUAN SHEN_NAME4('Y', 'U', 'A', '_')
#define VSYB_FIRST_OPPONENT SHEN_NAME4('K', 'T', '8', '_')
#define VSYB_SECOND_OPPONENT SHEN_NAME4('K', 'T', 'A', '_')
#define VSYB_THIRD_OPPONENT SHEN_NAME4('K', 'U', '9', '_')
#define VSYB_COH SHEN_NAME4('C', 'O', 'H', '_')
#define VSYB_BATTLE_POINT SHEN_NAME4('B', 'T', 'C', '1')
#define VSYB_RETURN_AREA SHEN_NAME4('A', 'K', '0', '0')
#define VSYB_LIGHT SHEN_NAME4('0', '0', '0', '0')
#define VSYB_OPENING_AUTHOR SHEN_NAME4('A', 'U', 'T', '0')
#define VSYB_WIN_AUTHOR SHEN_NAME4('A', 'U', 'T', '1')
#define VSYB_LOSE_AUTHOR SHEN_NAME4('A', 'U', 'T', '2')
#define VSYB_DEBUG_ID 3
typedef struct VsybEventWork {
    HLibTask *self;
    int reserved04;
    void *motion;
    unsigned short reserved0c, state;
    unsigned int elapsedFrames, durationFrames;
    EventReadCharactersWork *characters;
} VsybEventWork;

void QueueSkAuthorStart(HLibTask *parent, int command, HLibTaskCallback callback, void *callbackData, int delayFrames);
void VsybMain(void *argument);
void InitializeVsybWork(VsybEventWork *work, HLibTask *task);
void InitializeVsybResources(VsybEventWork *work);
void CleanupVsybEvent(VsybEventWork *work);
void VsybDebugExitCallback(void *data);
void VsybMainCallback(void *data);
void PrepareVsybBattle(void);
void VsybBattleCallback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name VSYB_AREA{SHEN_NAME(VSYB)}, VSYB_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name VSYB_PLAYER{SHEN_NAME(RYO_)}, VSYB_REN{SHEN_NAME(REN_)}, VSYB_WONG{SHEN_NAME(WON_)};
inline constexpr shen_name VSYB_YUAN{SHEN_NAME(YUA_)}, VSYB_FIRST_OPPONENT{SHEN_NAME(KT8_)}, VSYB_SECOND_OPPONENT{SHEN_NAME(KTA_)};
inline constexpr shen_name VSYB_THIRD_OPPONENT{SHEN_NAME(KU9_)}, VSYB_COH{SHEN_NAME(COH_)}, VSYB_INVALID{0xffffffffU};
inline constexpr shen_name VSYB_BATTLE_POINT{SHEN_NAME(BTC1)}, VSYB_RETURN_AREA{SHEN_NAME(AK00)}, VSYB_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name VSYB_OPENING_AUTHOR{SHEN_NAME(AUT0)}, VSYB_WIN_AUTHOR{SHEN_NAME(AUT1)}, VSYB_LOSE_AUTHOR{SHEN_NAME(AUT2)};
inline constexpr shen_name VSYB_CHARACTERS[]{
    VSYB_PLAYER, VSYB_REN, VSYB_WONG, VSYB_YUAN, VSYB_FIRST_OPPONENT, VSYB_SECOND_OPPONENT, VSYB_THIRD_OPPONENT, VSYB_COH, VSYB_INVALID};
struct VsybEventWork {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::int32_t motion;
    std::uint16_t reserved10;
    std::int16_t state;
    std::int32_t elapsedFrames;
    std::int32_t durationFrames;
    std::uint32_t reserved1c;
    EventReadCharactersWork *characters;
};
static_assert(sizeof(VsybEventWork) == 0x28);
static_assert(offsetof(VsybEventWork, state) == 0x12);
static_assert(offsetof(VsybEventWork, elapsedFrames) == 0x14);
static_assert(offsetof(VsybEventWork, characters) == 0x20);
void VsybInit();
void VsybMain(std::uintptr_t);
void VsybMainCallback(void *);
void VsybBattleCallback(void *);
void InitializeVsybResources(VsybEventWork *);
void PrepareVsybBattle();
void CleanupVsybEvent(VsybEventWork *);
extern const ScriptAreaDefinition vsybScriptDefinition;
} // namespace shenmue::s2
#endif
