#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
namespace CycleManager {


struct NpcRecord;
} // namespace CycleManager


struct KaidanOccupancyEntry {
    std::int32_t stairGuideEntryId;
    shen_name charaId;
    CycleManager::NpcRecord *npc;
    KaidanOccupancyEntry *activeNext;
    KaidanOccupancyEntry *freeNext;
};

struct CameraStairGuideEntry {
    float x;
    float y;
    float z;
    std::uint32_t yawAngle16;
    std::uint32_t flags;
    std::int32_t stepCount;
};

struct CameraStairEventEntry {
    std::uint32_t kind;
    std::uint32_t guideIndex;
    float probeHalfWidth;
};

static_assert(sizeof(KaidanOccupancyEntry) == 0x20);
static_assert(offsetof(KaidanOccupancyEntry, activeNext) == 0x10);
static_assert(sizeof(CameraStairGuideEntry) == 0x18);
static_assert(sizeof(CameraStairEventEntry) == 0x0c);

extern KaidanOccupancyEntry *g_kaidanOccupancyHead;
extern KaidanOccupancyEntry *g_kaidanFreeListHead;
extern KaidanOccupancyEntry *g_kaidanData;
extern KaidanOccupancyEntry *g_kaidanIterator;
extern std::int32_t g_playerRawStairGuideEntryId;
extern std::int32_t g_playerLatchedStairGuideEntryId;
extern std::int32_t g_playerInStairPrivateVolume;
extern std::int32_t g_playerStairGuidePairConflict;
extern std::int32_t g_temporaryKaidanId;
extern std::int32_t g_playerKaidanGuideActive;

void KDAN_Init();
Vector3 RotateAroundYAndTransformWorldCoords(const Vector3 &localOffset, const Vector3 &worldOrigin, std::uint16_t yaw);
void KDAN_ResetOccupancy();
void KDAN_RemoveOccupancyByCharacterId(std::uint32_t characterId);
void KDAN_PruneStaleOccupancyEntries();
bool KDAN_IsCharacterOccupyingStairs(std::uint32_t characterId);
void KDAN_AddOccupancy(std::int32_t guideEntryId, CycleManager::NpcRecord &npc);
bool npcKaidanUpdateStairWalk(CycleManager::NpcRecord &npc, bool steeringActive);
void npcKaidanSetStairMotion(CycleManager::NpcRecord &npc);
std::int32_t CheckSTIR_case(const Vector3 &position, std::uint16_t yaw);
bool Angle16_IsWithinMaxDelta(std::int32_t angle, std::int32_t otherAngle, std::int32_t maxAngle);
std::int32_t CameraStair_QueryGuideCollision(HLib::Task *scenarioTask, std::uint32_t charaId, std::uint32_t guideCount, const CameraStairGuideEntry *guides,
    const CameraStairEventEntry *events, std::uint32_t maxYawDelta);
std::uint32_t CameraStair_BuildDynamicGuideFromSTIR(HLib::Task *scenarioTask, std::uint32_t charaId, CameraStairGuideEntry *guide, std::int32_t maxAngle);
void CameraStair_ApplyGuideFacingCorrection(HLib::Task *scenarioTask, std::uint32_t charaId, std::uint32_t guideIndex, const CameraStairGuideEntry *guides);

} // namespace shenmue::s1
