#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/hlib.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct LegacyHudText;
struct DuckProfile {
    float speedNormal, speedMax, speedMin, obstacleProbeBackoff, separationRadiusBase;
    std::uint16_t aiStyleId, guts, handling, collisionCost;
    float staminaBase, sizeScale, speedResponse;
};
struct DuckRaceActor {
    HLib::Task *self;
    std::byte reserved08[8];
    Vector3 position;
    Vector3i rotation;
    std::int32_t targetYaw;
    shen_name name;
    std::uint32_t flags, behavior;
    std::int32_t index, replaySlot, collisionSlowTimer, turnLockTimer, actionTimer, steerTimer, currentMotion;
    std::int32_t stamina, maximumStamina;
    std::uint16_t profile;
    std::int16_t handling;
    std::uint16_t speedTier, guts;
    std::int16_t collisionStaminaCost;
    std::uint16_t soundCooldown;
    float speed, maximumSpeed, normalSpeed, minimumSpeed, separationRadius, probeBackoff, speedResponse, sizeScale, distance;
    std::int32_t aiControlled;
    std::int16_t style, reserved92;
    std::int32_t finishTriggerSeen;
};
static_assert(sizeof(DuckProfile) == 40);
static_assert(sizeof(DuckRaceActor) == 0x98);
static_assert(offsetof(DuckRaceActor, position) == 0x10 && offsetof(DuckRaceActor, name) == 0x2c);
static_assert(offsetof(DuckRaceActor, speed) == 0x68 && offsetof(DuckRaceActor, aiControlled) == 0x8c);
struct DuckReplaySample {
    Vector3 position;
    std::int32_t motion;
    float yawDegrees, motionFrame;
};
struct DuckCourseRange {
    Vector3 first, second;
};
struct DuckTurnTrigger {
    Vector3 position;
    std::int32_t yaw;
    float valid;
};
struct DuckRaceRuntime {
    std::array<HLib::Task *, 6> actors;
    std::array<shen_name, 6> names;
    std::array<std::array<DuckReplaySample, 90>, 6> replay;
    std::array<shen_name, 6> finishNames;
    std::array<std::int32_t, 6> finishTicks;
    std::int32_t actorCount, ticks, finishCount, speedExperience, staminaExperience, gutsExperience, turnExperience;
    std::int32_t staminaScale, experiencePublished, playerOverlayDisabled;
    float courseLength;
    std::array<std::uint32_t, 6> dust;
    std::int32_t followCamera, cameraIndex, followedActor;
    Vector3 cameraTarget, cameraProbe;
    std::array<LegacyHudText *, 7> placementText;
};
extern DuckRaceRuntime duckRaceRuntime;
extern const std::array<DuckProfile, 23> duckProfiles;
extern const std::array<DuckCourseRange, 7> duckTurnZones;
extern const std::array<std::array<Vector3, 3>, 6> duckTurnTargets;
extern const std::array<DuckTurnTrigger, 3> duckTurnTriggers;
extern const std::array<DuckCourseRange, 5> duckCourseSegments;
std::int32_t duckSetPlayerOverlayDisabled(std::int32_t disabled);
void ResetDuckRaceRuntime();
void InitializeDuckRaceActor(DuckRaceActor *actor, std::int32_t profile);
void UpdateDuckRaceActorMotion(DuckRaceActor *actor);
void UpdateDuckRaceAiSpeed(DuckRaceActor *actor);
void UpdateDuckRacePlayerSpeed(DuckRaceActor *actor);
void AdvanceDuckRaceActor(DuckRaceActor *actor);
void UpdateDuckRaceTurnZones(DuckRaceActor *actor);
void AvoidDuckRaceCourseMarkers(DuckRaceActor *actor);
void CheckDuckRaceCourseBounds(DuckRaceActor *actor);
void ResolveDuckRaceActorCollisions(DuckRaceActor *actor);
void CheckDuckRaceTurnTriggers(DuckRaceActor *actor);
void UpdateDuckRaceTurnExperience(DuckRaceActor *actor);
void UpdateDuckRaceActorExperience(DuckRaceActor *actor);
void UpdateDuckRaceTurnRecovery(DuckRaceActor *actor);
void UpdateDuckRaceHopRecovery(DuckRaceActor *actor);
void UpdateDuckRaceReplayActor(DuckRaceActor *actor);
void UpdateDuckRacePlayerHopInput(DuckRaceActor *actor);
std::int16_t FindDuckRaceCourseSegment(shen_name character);
std::uint32_t CalculateDuckRaceExperience(std::uint32_t increment);
void TickDuckRaceActor(DuckRaceActor *actor);
void QueueDuckRaceReplay();
void UpdateDuckRacePlacementText(std::int32_t replaySlot, std::int32_t actor, std::int32_t ticks);
void UpdateDuckRaceDust(std::int32_t actor, float speed);
void CreateDuckRaceDust();
void SnapDuckRaceCameraTarget();
void SetDuckRacePreviewCameraTarget(std::int32_t actor);
void UpdateDuckRaceFollowCamera();
}
