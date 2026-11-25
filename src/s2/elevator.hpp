#pragma once

#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "common/math.hpp"
#include "common/types.hpp"
#include <stddef.h>

namespace shenmue {
namespace s2 {
struct MT7Node;
struct ElevatorResourceRecord {
    std::uint32_t modelName;
    std::int16_t currentFloor, yaw;
    Vector3 position;
    std::uint16_t validFloorMasks[4];
    std::int16_t specialFloor;
    std::uint16_t flags;
};

typedef char Layout_elevator_0[(sizeof(ElevatorResourceRecord) * 25 == 0x320) ? 1 : -1];
extern ElevatorResourceRecord g_elevatorResources[25];
struct ElevatorTaskPayload {
    ElevatorResourceRecord *resource;
    std::int16_t flags;
    std::int16_t direction, floorStepTicks, doorFrame, currentFloor, highestValidFloor, lowestValidFloor;
    std::uint16_t floorRequests[3][4], retained2a;
    shen_name actor;
    std::int32_t sourceFloor, destinationFloor;
    float floorListOffset;
};
typedef char Layout_elevator_1[(sizeof(ElevatorResourceRecord) == 0x20) ? 1 : -1];
typedef char Layout_elevator_2[(offsetof(ElevatorResourceRecord, currentFloor) == 4) ? 1 : -1];
typedef char Layout_elevator_3[(offsetof(ElevatorResourceRecord, position) == 8) ? 1 : -1];
typedef char Layout_elevator_4[(offsetof(ElevatorResourceRecord, validFloorMasks) == 0x14) ? 1 : -1];
typedef char Layout_elevator_5[(sizeof(ElevatorTaskPayload) == 0x3c) ? 1 : -1];
typedef char Layout_elevator_6[(offsetof(ElevatorTaskPayload, flags) == 4) ? 1 : -1];
typedef char Layout_elevator_7[(offsetof(ElevatorTaskPayload, direction) == 6) ? 1 : -1];
typedef char Layout_elevator_8[(offsetof(ElevatorTaskPayload, floorStepTicks) == 8) ? 1 : -1];
typedef char Layout_elevator_9[(offsetof(ElevatorTaskPayload, doorFrame) == 0xa) ? 1 : -1];
typedef char Layout_elevator_10[(offsetof(ElevatorTaskPayload, currentFloor) == 0xc) ? 1 : -1];
typedef char Layout_elevator_11[(offsetof(ElevatorTaskPayload, highestValidFloor) == 0xe) ? 1 : -1];
typedef char Layout_elevator_12[(offsetof(ElevatorTaskPayload, lowestValidFloor) == 0x10) ? 1 : -1];
typedef char Layout_elevator_13[(offsetof(ElevatorTaskPayload, floorRequests) == 0x12) ? 1 : -1];
typedef char Layout_elevator_14[(offsetof(ElevatorTaskPayload, actor) == 0x2c) ? 1 : -1];
typedef char Layout_elevator_15[(offsetof(ElevatorTaskPayload, sourceFloor) == 0x30) ? 1 : -1];
typedef char Layout_elevator_16[(offsetof(ElevatorTaskPayload, destinationFloor) == 0x34) ? 1 : -1];
typedef char Layout_elevator_17[(offsetof(ElevatorTaskPayload, floorListOffset) == 0x38) ? 1 : -1];
struct ElevatorModelSource {
    MT7Node *modelRoot;
    const char *filename;
    Vector3 translation;
    std::int16_t yaw;
    std::uint16_t retained16;
    std::uint32_t prefixToken;
    float lockonAngle, yawLimit;
};
typedef char Layout_elevator_18[(sizeof(ElevatorModelSource) == 0x24) ? 1 : -1];
typedef char Layout_elevator_19[(offsetof(ElevatorModelSource, prefixToken) == 0x18) ? 1 : -1];

static const Vector3 ElevatorDrawOffsets[3] = {{SHEN_FIELD(x) 1.0f, SHEN_FIELD(y) 1.18f, SHEN_FIELD(z) 0.11f},
    {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 2.47f, SHEN_FIELD(z) 0.1f}, {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 2.23f, SHEN_FIELD(z) 0.12f}};
struct ElevatorFloorListMaterial {
    std::byte retained00[0x48];
    Vector3 colour;
};
typedef char Layout_elevator_20[(offsetof(ElevatorFloorListMaterial, colour) == 0x48) ? 1 : -1];
void elevator_task_Enqueue();
std::int32_t IsFloorRequested(const ElevatorTaskPayload *payload, std::int32_t floor, std::int32_t bank);
std::int32_t OpenElevatorDoor(ElevatorTaskPayload *payload);
std::int32_t CloseElevatorDoor(ElevatorTaskPayload *payload);
void SelectElevatorDirection(ElevatorTaskPayload *payload);
void StepElevatorFloor(ElevatorTaskPayload *payload);
void DrawElevatorNode(MT7Node *root, std::int32_t id);
void DrawElevator(void *parameter);
std::int32_t UpdateElevatorDoor(ElevatorTaskPayload *payload);
void ElevatorTaskCallback(void *parameter);
extern ElevatorModelSource g_elevatorModelSources[3];
extern ElevatorTaskPayload *g_elevatorRegistry[8];
void PublishElevatorTransforms(ElevatorTaskPayload *payload);
void UpdateAllElevatorTransforms();
ElevatorTaskPayload *FindElevator(shen_name elevator);
std::int32_t GetFloorBit(const ElevatorTaskPayload *payload, std::int32_t floor);
std::int32_t SetFloorRequest(ElevatorTaskPayload *payload, std::int32_t floor, std::int32_t bank, std::int32_t enabled);
std::int32_t IsElevatorFloorValid(shen_name elevator, std::int32_t floor);
std::int32_t GetCurrentElevatorFloor(shen_name elevator);
std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int32_t floor);
std::int32_t SetElevatorIndicatorFlag(shen_name elevator, std::int32_t index, std::int32_t enabled);
std::int32_t SetElevatorDisplayMode(shen_name elevator, std::int32_t selector, std::int32_t value);
std::int32_t GetElevatorTransform(shen_name elevator, Vector3 *position, std::uint32_t *yaw);
std::int32_t RequestElevatorFloor(shen_name elevator, std::int32_t floor, std::int32_t requestBank);
std::int32_t BeginElevatorTravel(shen_name actor, shen_name elevator, std::int32_t sourceFloor, std::int32_t destinationFloor);
std::int32_t ArriveAndOpenElevator(shen_name actor, shen_name elevator);
std::int32_t CloseAndResumeElevator(shen_name elevator);
} // namespace s2
} // namespace shenmue

#else

#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct MT7Node;
inline constexpr shen_name TASK_ELVT = SHEN_NAME(ELVT);
inline constexpr std::array<Vector3, 3> ElevatorDrawOffsets{
    {{.x = 1.0f, .y = 1.1799999f, .z = 0.12f}, {.x = 0.0f, .y = 2.47f, .z = 0.1f}, {.x = 0.0f, .y = 2.23f, .z = 0.12f}}};

struct ElevatorModelSource {
    const MT7Node *modelRoot;
    const char *filename;
    Vector3 translation;
    std::int16_t yaw;
    std::uint16_t reserved1e;
    std::uint32_t prefixToken;
    float lockonAngle;
    float yawLimit;
    std::uint32_t reserved2c;
};
static_assert(sizeof(ElevatorModelSource) == 0x30);
static_assert(offsetof(ElevatorModelSource, filename) == 0x08);
static_assert(offsetof(ElevatorModelSource, translation) == 0x10);
static_assert(offsetof(ElevatorModelSource, yaw) == 0x1c);
static_assert(offsetof(ElevatorModelSource, prefixToken) == 0x20);
static_assert(offsetof(ElevatorModelSource, lockonAngle) == 0x24);
static_assert(offsetof(ElevatorModelSource, yawLimit) == 0x28);
inline constexpr std::array<ElevatorModelSource, 3> InitialElevatorModelSources{{
    {.modelRoot = nullptr,
        .filename = "ELVTTOUG.MT7",
        .translation = {1.0f, std::bit_cast<float>(0x3f970a3dU), std::bit_cast<float>(0x3df5c28fU)},
        .yaw = -32768,
        .reserved1e = 0,
        .prefixToken = 0x4445,
        .lockonAngle = 0.5f,
        .yawLimit = std::bit_cast<float>(0x462aa800U),
        .reserved2c = 0},
    {.modelRoot = nullptr,
        .filename = "ELVTTUEG.MT7",
        .translation = {0.0f, std::bit_cast<float>(0x401e147bU), std::bit_cast<float>(0x3dcccccdU)},
        .yaw = -32768,
        .reserved1e = 0,
        .prefixToken = 0,
        .lockonAngle = 0.0f,
        .yawLimit = 0.0f,
        .reserved2c = 0},
    {.modelRoot = nullptr,
        .filename = "ELVTMKNG.MT7",
        .translation = {0.0f, std::bit_cast<float>(0x400eb852U), std::bit_cast<float>(0x3df5c28fU)},
        .yaw = -32768,
        .reserved1e = 0,
        .prefixToken = 0x5545,
        .lockonAngle = 1.0f,
        .yawLimit = std::bit_cast<float>(0x462aa800U),
        .reserved2c = 0},
}};

struct ElevatorResourceRecord {
    shen_name modelName{};
    std::int16_t currentFloor{};
    std::int16_t yaw{};
    Vector3 position{};
    std::array<std::uint16_t, 4> validFloorMasks{};
    std::int16_t specialFloor{};
    std::uint16_t flags{};
};

static_assert(sizeof(ElevatorResourceRecord) == 0x20);
static_assert(offsetof(ElevatorResourceRecord, modelName) == 0x00);
static_assert(offsetof(ElevatorResourceRecord, currentFloor) == 0x04);
static_assert(offsetof(ElevatorResourceRecord, yaw) == 0x06);
static_assert(offsetof(ElevatorResourceRecord, position) == 0x08);
static_assert(offsetof(ElevatorResourceRecord, validFloorMasks) == 0x14);
static_assert(offsetof(ElevatorResourceRecord, specialFloor) == 0x1c);
static_assert(offsetof(ElevatorResourceRecord, flags) == 0x1e);

struct ElevatorTaskPayload {
    ElevatorResourceRecord *resource{};
    std::uint16_t flags{};
    std::int16_t direction{};
    std::int16_t floorStepTicks{};
    std::int16_t doorFrame{};
    std::int16_t currentFloor{};
    std::int16_t highestValidFloor{};
    std::int16_t lowestValidFloor{};
    std::array<std::array<std::uint16_t, 4>, 3> floorRequests{};
    std::uint16_t reserved2e{};
    std::uint32_t actorId{};
    std::int32_t sourceFloor{};
    std::int32_t destinationFloor{};
    float floorListOffset{};
    std::array<std::uint32_t, 3> placementTokens{};
    std::uint32_t reserved4c{};
};

static_assert(sizeof(ElevatorTaskPayload) == 0x50);
static_assert(offsetof(ElevatorTaskPayload, resource) == 0x00);
static_assert(offsetof(ElevatorTaskPayload, flags) == 0x08);
static_assert(offsetof(ElevatorTaskPayload, direction) == 0x0a);
static_assert(offsetof(ElevatorTaskPayload, floorStepTicks) == 0x0c);
static_assert(offsetof(ElevatorTaskPayload, doorFrame) == 0x0e);
static_assert(offsetof(ElevatorTaskPayload, currentFloor) == 0x10);
static_assert(offsetof(ElevatorTaskPayload, highestValidFloor) == 0x12);
static_assert(offsetof(ElevatorTaskPayload, lowestValidFloor) == 0x14);
static_assert(offsetof(ElevatorTaskPayload, floorRequests) == 0x16);
static_assert(offsetof(ElevatorTaskPayload, floorRequests) + sizeof(std::array<std::uint16_t, 4>) == 0x1e);
static_assert(offsetof(ElevatorTaskPayload, floorRequests) + 2 * sizeof(std::array<std::uint16_t, 4>) == 0x26);
static_assert(offsetof(ElevatorTaskPayload, actorId) == 0x30);
static_assert(offsetof(ElevatorTaskPayload, sourceFloor) == 0x34);
static_assert(offsetof(ElevatorTaskPayload, destinationFloor) == 0x38);
static_assert(offsetof(ElevatorTaskPayload, floorListOffset) == 0x3c);
static_assert(offsetof(ElevatorTaskPayload, placementTokens) == 0x40);
static_assert(offsetof(ElevatorTaskPayload, placementTokens) + sizeof(std::uint32_t) == 0x44);
static_assert(offsetof(ElevatorTaskPayload, placementTokens) + 2 * sizeof(std::uint32_t) == 0x48);
static_assert(offsetof(ElevatorTaskPayload, reserved4c) == 0x4c);

inline constexpr std::size_t ElevatorPersistentStateSize = 18;

void QueueElevatorTasks();
void ReleaseElevatorRuntime();
void UpdateAllElevatorTransforms() noexcept;

std::int32_t GetElevatorTransform(shen_name elevator, Vector3 &position, std::uint32_t &yaw) noexcept;
std::int32_t GetCurrentElevatorFloor(shen_name elevator) noexcept;
bool IsElevatorFloorValid(shen_name elevator, std::int16_t floor) noexcept;
std::int32_t RequestElevatorFloor(shen_name elevator, std::int16_t floor, std::uint32_t requestBank) noexcept;
std::int32_t BeginElevatorTravel(float soundContext, shen_name elevator, shen_name actor, std::int32_t sourceFloor, std::int32_t destinationFloor) noexcept;
std::int32_t ArriveAndOpenElevator(float soundContext, shen_name elevator) noexcept;
std::int32_t CloseAndResumeElevator(shen_name elevator) noexcept;
std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int16_t floor) noexcept;
std::int32_t SetElevatorDisplayMode(shen_name elevator, std::int32_t selector, std::int32_t value) noexcept;
std::int32_t SetElevatorIndicatorFlag(shen_name elevator, std::int32_t index, std::int32_t enabled) noexcept;

void CaptureElevatorPersistentState(std::span<std::byte, ElevatorPersistentStateSize> output) noexcept;
void RestoreElevatorPersistentState(std::span<const std::byte, ElevatorPersistentStateSize> input) noexcept;

} // namespace shenmue::s2

#endif
