#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/npc_data_dc.hpp"
#else
#include "s2/npc.hpp"
#endif

#if !SHENMUE_S2_GCC295
#include <cstddef>
#include <cstdint>
#endif
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <span>
#endif

#if SHENMUE_S2_GCC295
#define SHENMUE_NPCAREA_JOIN_(a, b) a##b
#define SHENMUE_NPCAREA_JOIN(a, b) SHENMUE_NPCAREA_JOIN_(a, b)
#define SHENMUE_NPCAREA_LAYOUT(condition) typedef char SHENMUE_NPCAREA_JOIN(NpcAreaLayout_, __LINE__)[(condition) ? 1 : -1]
#else
#define SHENMUE_NPCAREA_LAYOUT(condition) static_assert(condition)
#endif

namespace shenmue {
namespace s2 {


#pragma pack(push, 4)
struct NpcAreaPoint {
    shen_name area;
    std::int16_t routeIndex;
    std::int16_t field06;
#if SHENMUE_IS_DC
    std::int16_t routeParameters[4][5];
#else
    std::byte routeParameters[40];
#endif
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
#if SHENMUE_S2_GCC295
    std::int8_t laneMode;
#else
    std::uint8_t laneMode;
#endif
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
#if SHENMUE_IS_DC
    std::int8_t actionKind;
#else
    std::uint8_t actionKind;
#endif
    std::int8_t adjacentOwners[4];
    std::int32_t linkedControlOffset;
    float positionAlongSegment;
    std::int32_t yaw;
    Vector3 position;
};
#pragma pack(pop)

SHENMUE_NPCAREA_LAYOUT(sizeof(NpcAreaPoint) == 0x44);
SHENMUE_NPCAREA_LAYOUT(offsetof(NpcAreaPoint, routeStateOffset) == 0x40);
SHENMUE_NPCAREA_LAYOUT(sizeof(NpcAreaRouteState) == 0x28);
SHENMUE_NPCAREA_LAYOUT(offsetof(NpcAreaRouteState, npcListHeadOffset) == 0x10);
SHENMUE_NPCAREA_LAYOUT(sizeof(NpcAreaControlRow) == 0x2c);
SHENMUE_NPCAREA_LAYOUT(offsetof(NpcAreaControlRow, forwardOffset) == 0x24);
SHENMUE_NPCAREA_LAYOUT(sizeof(NpcAreaOwnerRow) == 0x20);
SHENMUE_NPCAREA_LAYOUT(offsetof(NpcAreaOwnerRow, position) == 0x14);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct NpcAreaStoreSlot {
    NpcAreaRouteState *state;
    NpcAreaStoreSlot *previous;
    NpcAreaStoreSlot *next;
    float distanceSquared;
};
SHENMUE_NPCAREA_LAYOUT(sizeof(NpcAreaStoreSlot) == 0x10);
SHENMUE_NPCAREA_LAYOUT(offsetof(NpcAreaStoreSlot, previous) == 4);
SHENMUE_NPCAREA_LAYOUT(offsetof(NpcAreaStoreSlot, next) == 8);
SHENMUE_NPCAREA_LAYOUT(offsetof(NpcAreaStoreSlot, distanceSquared) == 0x0c);
#if SHENMUE_S2_GCC295
static const std::int32_t kNpcAreaRouteSlots = 512;
#else
inline constexpr std::int32_t kNpcAreaRouteSlots = 512;
#endif
extern NpcAreaStoreSlot *npcNearestAreaSlot;
extern std::int32_t npcAreaRouteCount;
extern NpcAreaPoint *npcAreaPoints;
std::int32_t PrepareNpcAreaRouteStore(std::int32_t count);
std::int32_t RegisterNpcAreaRouteState(NpcAreaRouteState *state, std::int32_t point);
void InitializeNpcAreaPointTable(NpcAreaPoint *points);
void RebuildNpcAreaPointGround(NpcAreaPoint *points);
NpcAreaRouteState *GetNpcAreaPointRouteState(std::int32_t point);
std::uint32_t GetNpcAreaSpecialMotionFlags(const NpcRouteLiveEntry &entry);
std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState *state, NpcRouteLiveEntry *entry);
void ApplyNpcAreaMotion(NpcRouteLiveEntry *entry, std::uint16_t motion);
std::int32_t ClearNpcAreaRouteForEntry(NpcRouteLiveEntry *entry);
void ActivateNpcAreaRoutes(std::int32_t active);
void PrepareNpcAreaRoutes();
#else
// NPCB is the sole backing for these records. Registration converts file-relative
// row offsets to HLib offsets in place; list heads remain state-relative.
bool InitializeNpcAreaRouteStore();
bool BindNpcAreaRouteResources(std::span<NpcAreaPoint> points, const Vector3 &areaOrigin);
void SetNpcAreaRouteOrigin(const Vector3 &origin);
void ResetNpcAreaRouteResources();
void FreeNpcAreaRouteStore();
NpcAreaRouteState *GetNpcAreaPointRouteState(std::uint16_t point);
std::uint16_t ResolveNpcAreaAnimation(NpcAreaRouteState &state, NpcRouteLiveEntry &entry, bool advancing);
std::uint16_t npcResolveAnimationForRoute(NpcAreaRouteState &state, NpcRouteLiveEntry &entry);
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
#endif

} // namespace s2
} // namespace shenmue
#if SHENMUE_S2_GCC295
#undef SHENMUE_NPCAREA_JOIN_
#undef SHENMUE_NPCAREA_JOIN
#endif
#undef SHENMUE_NPCAREA_LAYOUT
