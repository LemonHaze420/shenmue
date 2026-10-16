#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {

struct GameRequest {
    std::int32_t kind;
    Vector3 position;
    float yaw;
};
struct GameWork {
    HLib::Task *task;
    std::uint32_t kind;
    std::int32_t cost, state, purchase, hasPlayed;
    std::uint32_t reserved1c;
};
static_assert(sizeof(GameRequest) == 0x14 && offsetof(GameRequest, position) == 4 && offsetof(GameRequest, yaw) == 0x10);
static_assert(sizeof(GameWork) == 0x20 && offsetof(GameWork, kind) == 8 && offsetof(GameWork, state) == 0x10);
static_assert(offsetof(GameWork, purchase) == 0x14 && offsetof(GameWork, hasPlayed) == 0x18);
void InitGame();
void QueueGame(std::uintptr_t request);
void GameEnqueueCallback(void *request);
void GameWaitStreetFightCallback(void *request);
void GameMainCallback(void *);
void GameAuthorCallback(void *);
void GameExitCallback(void *);
void GameLocalDebugNullsub();
extern const ScriptAreaDefinition gameScriptDefinition;
} // namespace shenmue::s2
