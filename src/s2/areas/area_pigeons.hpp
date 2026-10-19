#pragma once

#include "s2/hlib.hpp"
#include "common/types.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name PIGEON_TASK{SHEN_NAME(hato)};
inline constexpr shen_name PIGEON_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name PIGEON_ABERDEEN_AREA{SHEN_NAME(AR02)};
enum class PigeonState : std::int32_t { Wander, Approach, HoverBob, Cooldown };
struct AreaPigeon {
    shen_name name;
    Vector3 origin;
    float roamRadius;
    Vector3 position;
    Vector3i rotation;
    float distanceToPlayer;
    float distanceToOrigin;
    PigeonState state;
    std::int32_t randomWaitFrames;
    std::int32_t bobPhase;
    std::int32_t birdCount;
};
static_assert(sizeof(AreaPigeon) == 0x44);
static_assert(offsetof(AreaPigeon, origin) == 0x04);
static_assert(offsetof(AreaPigeon, position) == 0x14);
static_assert(offsetof(AreaPigeon, rotation) == 0x20);
static_assert(offsetof(AreaPigeon, state) == 0x34);
static_assert(offsetof(AreaPigeon, birdCount) == 0x40);
extern std::array<AreaPigeon, 10> areaPigeons1;
extern std::array<AreaPigeon, 10> areaPigeons2;
extern std::array<AreaPigeon, 10> areaPigeons3;
extern std::int32_t areaPigeonsActive;
extern Vector3 areaPigeonPlayerPosition;
extern Vector3i areaPigeonPlayerRotation;
void QueueAreaPigeons(const Vector3 *origin, float radius, AreaPigeon *pigeons, shen_name firstCharacter);
void AreaPigeonsCallback(void *taskData);
void DestroyAreaPigeons(HLib::Task *task);
void PigeonCooldown(AreaPigeon *pigeon);
void PigeonWander(AreaPigeon *pigeon);
void PigeonApproach(AreaPigeon *pigeon);
void PigeonHoverBob(AreaPigeon *pigeon);
std::uint16_t EvaluatePigeonRoam(AreaPigeon *pigeon);
std::int16_t ComputePigeonRotation(AreaPigeon *pigeon, const Vector3 *origin, const Vector3 *position, float degrees);
inline constexpr std::array<void (*)(AreaPigeon *), 4> AREA_PIGEON_STATE_CALLBACKS{
    PigeonWander,
    PigeonApproach,
    PigeonHoverBob,
    PigeonCooldown,
};

} // namespace shenmue::s2
