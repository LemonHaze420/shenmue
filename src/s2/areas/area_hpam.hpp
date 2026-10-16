#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct ConversationBuffer;
struct MapStandEventWork {
    HLib::Task *task;
    shen_name stand, map;
    std::int32_t item, mode;
    std::byte reserved18[8];
    ConversationBuffer *conversation;
    std::int32_t phase, cost, initialChoice;
    std::uint32_t reserved34;
};
static_assert(sizeof(MapStandEventWork) == 0x38);
static_assert(offsetof(MapStandEventWork, conversation) == 0x20);
static_assert(offsetof(MapStandEventWork, phase) == 0x28);
struct EventCling;
namespace map_stand {
std::int32_t Intro(MapStandEventWork &work);
void GiveMap(void *item);
void QueueDisplay(shen_name character, std::int32_t mode, std::int32_t delay);
void QueueCling(const EventCling &request);
} // namespace map_stand
void HPAM_main_func(std::uintptr_t request);
void MapStandEventCallback(void *);
void MapStandExitCallback(void *);
extern const ScriptAreaDefinition areaHPAMScriptDefinition;
} // namespace shenmue::s2
