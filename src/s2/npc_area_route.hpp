#pragma once

#include "s2/npc.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

#pragma pack(push, 4)
struct NpcAreaPoint {
    shen_name area;
    std::int16_t routeIndex;
    std::int16_t field06;
    std::byte routeParameters[40];
    shen_name owner;
    Vector3 position;
    std::int32_t routeStateOffset;
};

struct NpcAreaRouteState {
    std::uint8_t flags;
    std::int8_t controlRowCount;
    std::int8_t ownerRowCount;
    std::uint8_t field03;
    std::uint8_t participantCount;
    std::uint8_t activeRowCount;
    std::uint8_t freeRowCount;
    std::uint8_t occupiedRowCount;
    std::int16_t areaPointIndex;
    std::uint16_t field0a;
    std::uint32_t field0c;
    std::int32_t npcListHeadOffset[2];
    std::int32_t animationScriptOffset;
    std::uint32_t areaMask;
    std::int32_t controlRowsOffset;
    std::int32_t ownerRowsOffset;
};

struct NpcAreaControlRow {
    std::uint8_t index;
    std::uint8_t flags;
    std::uint8_t laneMode;
    std::uint8_t spanCount;
    std::uint32_t previousForwardMask;
    std::uint32_t previousBackwardMask;
    std::uint32_t forwardMask;
    std::uint32_t backwardMask;
    float length;
    Vector3 position;
    std::int32_t forwardOffset;
    std::int32_t backwardOffset;
};

struct NpcAreaOwnerRow {
    std::uint8_t flags;
    std::uint8_t runtimeMask;
    std::uint8_t index;
    std::uint8_t actionKind;
    std::int8_t adjacentOwners[4];
    std::int32_t linkedControlOffset;
    float positionAlongSegment;
    std::int32_t yaw;
    Vector3 position;
};
#pragma pack(pop)

static_assert(sizeof(NpcAreaPoint) == 0x44);
static_assert(offsetof(NpcAreaPoint, routeStateOffset) == 0x40);
static_assert(sizeof(NpcAreaRouteState) == 0x28);
static_assert(offsetof(NpcAreaRouteState, npcListHeadOffset) == 0x10);
static_assert(sizeof(NpcAreaControlRow) == 0x2c);
static_assert(offsetof(NpcAreaControlRow, forwardOffset) == 0x24);
static_assert(sizeof(NpcAreaOwnerRow) == 0x20);
static_assert(offsetof(NpcAreaOwnerRow, position) == 0x14);

// NPCB is the sole backing for these records. Registration converts file-relative
// row offsets to HLib offsets in place; list heads remain state-relative.
bool InitializeNpcAreaRouteStore();
bool BindNpcAreaRouteResources(std::span<NpcAreaPoint> points, const Vector3 &areaOrigin);
void SetNpcAreaRouteOrigin(const Vector3 &origin);
void ResetNpcAreaRouteResources();
void FreeNpcAreaRouteStore();
NpcAreaRouteState *GetNpcAreaPointRouteState(std::uint16_t point);
std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing);
void ApplyNpcAreaMotion(NpcRouteLiveEntry &entry, std::uint16_t motion);
std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry);
std::uint32_t SelectNpcAreaTransitionMotion(const NpcRouteLiveEntry &entry);
void RestoreNpcAreaMotion(NpcRouteLiveEntry &entry);
bool TickNpcAreaRouteState(NpcRouteLiveEntry &entry);
void ActivateNpcAreaRoutes(bool active);
std::uint32_t GetNpcAreaRouteStoreCount();
void PrepareNpcAreaRoutes();
bool ResetNpcAreaRouteState(NpcRouteLiveEntry &entry);
bool TryEnterNpcAreaRoute(NpcRouteLiveEntry &entry);
bool RequestNpcAreaRouteCompletion(NpcRouteLiveEntry &entry);

} // namespace shenmue::s2
