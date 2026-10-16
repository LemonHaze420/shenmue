#pragma once
#include "s2/date_time.hpp"

#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

namespace HLib {
struct Task;
}

#pragma pack(push, 4)
struct SavedAreaJumpLocation {
    Vector3 position;
    std::int32_t yaw;
    std::uint8_t internalState;
    std::array<std::byte, 0x1f> reserved11;
};

static_assert(sizeof(SavedAreaJumpLocation) == 0x30);
static_assert(offsetof(SavedAreaJumpLocation, yaw) == 0x0c);
static_assert(offsetof(SavedAreaJumpLocation, internalState) == 0x10);

struct AreaJumpPersistentState {

    Vector3 returnPosition;
    std::int32_t returnYaw;
    std::uint8_t playerInternalState;
    std::array<std::byte, 0x13> reserved11;
    DateTime capturedTime;
    std::array<std::byte, 5> reserved2b;
    std::array<std::byte, 0x20> playerPlacement;
    std::int32_t sceneActId;
    shen_name sceneAreaId;
    std::int32_t sceneEntryId;
    std::uint16_t warpEnabled;
    std::uint16_t capturedDay;
    shen_name authorId;
    std::int32_t qronFloor;
    std::int32_t magicRoomFlag;
};
#pragma pack(pop)

static_assert(sizeof(AreaJumpPersistentState) == 0x6c);
static_assert(offsetof(AreaJumpPersistentState, capturedTime) == 0x24);
static_assert(offsetof(AreaJumpPersistentState, playerPlacement) == 0x30);
static_assert(offsetof(AreaJumpPersistentState, sceneActId) == 0x50);
static_assert(offsetof(AreaJumpPersistentState, authorId) == 0x60);

HLib::Task *initCtrlAreaJump();
void CaptureCurrentAreaJumpState();
void AdvanceAreaJumpTimeForDestination(shen_name areaId, std::int32_t entryId) noexcept;
void CaptureAreaJumpPersistentState(std::span<std::byte, sizeof(AreaJumpPersistentState)> output) noexcept;
void RestoreAreaJumpPersistentState(std::span<const std::byte, sizeof(AreaJumpPersistentState)> input) noexcept;
bool GetSavedAreaJumpLocation(Vector3 &position, std::int32_t &yaw) noexcept;
std::uint64_t HasSavedAreaJumpLocation() noexcept;
void ResetShenmueFlags() noexcept;
void ResetGlobalStateFlags() noexcept;
void SetUnusedFlag() noexcept;
shen_name GetAreaJumpLoadAuthor() noexcept;
std::int32_t GetRetainedMagicRoomFlag() noexcept;
void SetRetainedMagicRoomFlag(std::int32_t flag) noexcept;
std::uint64_t GetAreaJumpPlacementByte(std::int32_t offset) noexcept;
std::int64_t SetAreaJumpPlacementByte(std::int32_t offset, std::uint8_t value) noexcept;
float GetAreaJumpPlacementFloat(std::int32_t offset) noexcept;
std::uint64_t SetAreaJumpPlacementFloat(std::int32_t offset, float value) noexcept;
void RestoreSavedAreaJumpLocation(const SavedAreaJumpLocation &location, std::span<const std::byte, 0x20> placement) noexcept;
void ActivateSavedAreaJumpLocation() noexcept;

std::int64_t ResetAreaJumpWarpFlag() noexcept;

} // namespace shenmue::s2
