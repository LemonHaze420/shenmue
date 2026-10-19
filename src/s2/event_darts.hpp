#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {



struct EventDartsPlacement {
    std::int32_t mode{};
    Vector3i rotation{};
    Vector3 position{};
    shen_name character{};
    shen_name parent{};
};

struct EventDartsEnemy {
    shen_name name{};
    std::int32_t aiIndex{};
    std::uintptr_t context{};
    std::uint64_t reserved{};
};

struct EventEnemyDartsTaskData {
    std::byte reserved000[0x28];
    HLib::Task *task28;
    std::uintptr_t context30;
    std::int32_t count38;
    std::int32_t state3c;
    std::byte reserved040[8];
    std::uint8_t byte48;
    std::uint8_t byte49;
    std::uint8_t byte4a;
    std::uint8_t byte4b;
    std::uint8_t byte4c;
    std::byte reserved04d[7];
    std::int32_t field54;
    std::int32_t field58;
    std::int32_t field5c;
    std::byte reserved060[8];
    HLib::Task *task68;
    const EventDartsPlacement *configuration70;
    const EventDartsEnemy *enemy78;
};

static_assert(sizeof(EventDartsPlacement) == 0x24);
static_assert(offsetof(EventDartsPlacement, character) == 0x1c);
static_assert(sizeof(EventDartsEnemy) == 0x18);
static_assert(sizeof(EventEnemyDartsTaskData) == 0x80);
static_assert(offsetof(EventEnemyDartsTaskData, task28) == 0x28);
static_assert(offsetof(EventEnemyDartsTaskData, context30) == 0x30);
static_assert(offsetof(EventEnemyDartsTaskData, count38) == 0x38);
static_assert(offsetof(EventEnemyDartsTaskData, state3c) == 0x3c);
static_assert(offsetof(EventEnemyDartsTaskData, byte48) == 0x48);
static_assert(offsetof(EventEnemyDartsTaskData, field54) == 0x54);
static_assert(offsetof(EventEnemyDartsTaskData, task68) == 0x68);
static_assert(offsetof(EventEnemyDartsTaskData, configuration70) == 0x70);
static_assert(offsetof(EventEnemyDartsTaskData, enemy78) == 0x78);

struct EventDartsPositionTaskData {
    std::int32_t cleanupTask;
    std::int32_t hasRotation;
    std::int32_t hasPosition;
    Vector3i rotation;
    Vector3 position;
    shen_name parent;
    shen_name character;
};

static_assert(sizeof(EventDartsPositionTaskData) == 0x2c);
static_assert(offsetof(EventDartsPositionTaskData, rotation) == 0x0c);
static_assert(offsetof(EventDartsPositionTaskData, parent) == 0x24);

enum class EventDartsResult : std::uint8_t {
    First = 0,
    Second = 1,
    Third = 2,
    EnemyComplete = 3,
};

struct EventDartsState {
    shen_name enemy{};
    std::uint8_t mode{};
    bool active{};
};

void EVT_EnsureDartsPackageMounted();
void EVT_PrepareDartsPlacement(const EventDartsPlacement &placement);
void QueueDartsPosition(shen_name character, shen_name parent, const Vector3 *position, const Vector3i *rotation, std::int32_t delayFrames);
void DartsPositionCallback(void *taskData);
HLib::Task *EVT_SetEnemyDarts(const EventDartsPlacement *placement, const EventDartsEnemy *enemy);
HLib::Task *EVT_DartsEasySingle(std::int32_t count, const EventDartsPlacement *firstPlacement);
const EventDartsState &EVT_GetDartsState() noexcept;
void EVT_SubmitDartsResult(EventDartsResult result) noexcept;

struct EventDartsArrowTaskData {
    shen_name character;
    shen_name arrow;
};

struct EventDartsAiRecord {
    shen_name character;
    std::uint16_t motionA;
    std::uint16_t motionB;
    std::span<const std::int8_t> dartPattern;
};

struct EventDartsAiScoreRecord {
    std::uint32_t packed;
    std::int32_t minimumScore;
    std::int32_t defaultScore;
    std::int32_t scoreFloor;
};

} // namespace shenmue::s2
