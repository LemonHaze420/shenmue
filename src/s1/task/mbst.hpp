#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct MotionManagerWork;

#pragma pack(push, 1)
struct MBST_t {
    std::uint32_t taskToken;
    std::int16_t capacity;
    std::int16_t state;
};
#pragma pack(pop)

struct MBSTSelectionEntry {
    MotionManagerWork *motion;
    float distanceSquared;
    std::uint32_t reserved0c;
};

static_assert(sizeof(MBST_t) == 0x08);
static_assert(offsetof(MBST_t, capacity) == 0x04);
static_assert(offsetof(MBST_t, state) == 0x06);
static_assert(sizeof(MBSTSelectionEntry) == 0x10);
static_assert(offsetof(MBSTSelectionEntry, motion) == 0x00);
static_assert(offsetof(MBSTSelectionEntry, distanceSquared) == 0x08);
static_assert(offsetof(MBSTSelectionEntry, reserved0c) == 0x0c);

HLib::Task *MBST_Init();
void MBST_Shutdown();
HLib::Task *MBST_GetTask();
MBST_t *MBST_GetWork();

void MBST_SetCapacity(std::int16_t capacity);
void MBST_Refresh(bool releaseUnselected);
void MBST_PrepareMotionManagerCleanup(MotionManagerWork *excludedManager);
void MBST_ResetActiveMotionRows(MBST_t *mbst);
bool MBST_IsVisibleMotionCandidate(const Character *character);
float MBST_DistanceSquaredToCameraTarget(const Character *character);
std::int32_t MBST_SelectMotionCandidates(MBST_t *mbst, MBSTSelectionEntry *entries, MotionManagerWork *excludedMotion = nullptr);
bool MBST_CommitMotionCandidates(MBST_t *mbst, const MBSTSelectionEntry *entries, std::int32_t count, bool releaseUnselected);
void MBST_Callback(HLib::TaskData *taskData);

} // namespace shenmue::s1
