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
