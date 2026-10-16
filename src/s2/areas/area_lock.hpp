#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name LOCK_TASK{SHEN_NAME(lock)};
inline constexpr shen_name LOCK_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name LOCK_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name LOCK_HGN{SHEN_NAME(HGN_)};
inline constexpr shen_name LOCK_WTB{SHEN_NAME(WTB_)};
inline constexpr shen_name LOCK_SF8{SHEN_NAME(SF8_)};
inline constexpr shen_name LOCK_INVALID{0xffffffffU};
struct LockEventWork {
    HLib::Task *task;
    std::int32_t state;
    std::int32_t entry;
    EventReadCharactersWork *admission;
    shen_name opponent;
    std::uint32_t reserved1c;
};
static_assert(sizeof(LockEventWork) == 0x20);
static_assert(offsetof(LockEventWork, admission) == 0x10);
void AreaLOCKInit();
void AreaLOCKMain(std::uintptr_t parameter);
void LOCKPrepareEvent();
void LOCKPlaceCharacters(LockEventWork *work);
void LOCKPlacePlayer(LockEventWork *work);
void LOCKReleaseEvent(LockEventWork *work);
void LOCKMainCallback(void *data);
std::int32_t LOCKHgnConversation(LockEventWork *work);
std::int32_t LOCKWtbConversation(LockEventWork *work);
std::int32_t LOCKSf8Conversation(LockEventWork *work);
extern const ScriptAreaDefinition areaLOCKScriptDefinition;
} // namespace shenmue::s2::areas
