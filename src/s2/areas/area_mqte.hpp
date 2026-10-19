#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/step_event.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name MQTE_AREA{SHEN_NAME(MQTE)}, MQTE_TASK{SHEN_NAME(mqte)}, MQTE_EVENT{SHEN_NAME(EVNT)};
inline constexpr shen_name MQTE_RYO{SHEN_NAME(RYO_)}, MQTE_SHENFA{SHEN_NAME(SIN_)}, MQTE_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name MQTE_CAMERA{SHEN_NAME(CAM_)}, MQTE_DUMMY_RYO{SHEN_NAME(DUM1)}, MQTE_DUMMY_SHENFA{SHEN_NAME(DUM2)};
inline constexpr shen_name MQTE_INVALID{0xffffffffU}, MQTE_DUMMY_TASK{SHEN_NAME(dumc)};
inline constexpr shen_name MQTE_RUN_TASK{SHEN_NAME(runs)}, MQTE_FIRE_TASK{SHEN_NAME(fire)}, MQTE_FIREFLY{SHEN_NAME(FFLY)};
inline constexpr shen_name MQTE_FIRE_DUMMY{SHEN_NAME(dumy)}, MQTE_SUCCESS{SHEN_NAME(SUCS)}, MQTE_FAILURE{SHEN_NAME(FAIL)};
inline constexpr shen_name MQTE_RETRY{SHEN_NAME(RTR1)}, MQTE_DESTINATION{SHEN_NAME(KRC1)};
inline constexpr shen_name MQTE_FAILURE_LIGHT{SHEN_NAME(l151)}, MQTE_SUCCESS_LIGHT{SHEN_NAME(l141)};
inline constexpr shen_name MQTE_CHARACTERS[]{MQTE_RYO, MQTE_SHENFA, MQTE_INVALID};
inline constexpr shen_name MQTE_TEST_OBJECTS[]{SHEN_NAME(H___), SHEN_NAME(HI__), SHEN_NAME(T___), SHEN_NAME(LR__), SHEN_NAME(I___), SHEN_NAME(M___),
    SHEN_NAME(HT__), SHEN_NAME(D___), SHEN_NAME(L___), SHEN_NAME(TI__), SHEN_NAME(R___), SHEN_NAME(LRI_), SHEN_NAME(RLH_), SHEN_NAME(RLR_), SHEN_NAME(TDH_),
    SHEN_NAME(K___), SHEN_NAME(LRK_), SHEN_NAME(LRT_)};
inline std::int32_t forestHandPose[57]{1999, 65354, 59167, 1638, 543, 64810, 546, 63262, 4821, 0, 64810, 56252, 0, 64081, 58800, 0, 727, 363, 0, 0, 64628, 0, 0,
    57709, 0, 0, 57344, 0, 65172, 65354, 0, 0, 65354, 0, 729, 60074, 0, 65355, 57526, 0, 64809, 182, 0, 0, 0, 0, 910, 55705, 0, 1456, 59164, 0, 63354, 1092,
    64080, 182, 0};
struct ForestQteResult {
    std::int32_t elapsed;
    std::uint32_t reserved;
    std::uint16_t status, reserved0a;
    std::int32_t succeeded;
};
struct ForestQteWork {
    HLib::Task *task;
    std::int16_t phase, state, obstacle, previousUnit, command, completedUnits;
    shen_name character;
    EventReadCharactersWork *characters;
    ForestQteResult result;
    std::int32_t retried[3];
    std::uint32_t reserved3c;
};
static_assert(sizeof(ForestQteWork) == 0x40 && offsetof(ForestQteWork, phase) == 8 && offsetof(ForestQteWork, result) == 0x20);
static_assert(offsetof(ForestQteWork, characters) == 0x18 && offsetof(ForestQteWork, retried) == 0x30);
struct ForestObstacleCommand {
    std::uint16_t kind, reserved;
    float cameraOffset;
};
struct ForestObstacle {
    float x, z;
    ForestObstacleCommand commands[4];
};
static_assert(sizeof(ForestObstacle) == 40);
inline constexpr ForestObstacle forestObstacles[]{{0, 0, {{3, 0, 0}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}}, {0, 0, {{3, 0, 0}, {2, 0, -.4f}, {7, 0, 0}, {7, 0, 0}}},
    {0, -3.3f, {{4, 0, 0}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}}, {0, 1.5f, {{1, 0, .4f}, {0, 0, -1.4f}, {7, 0, 0}, {7, 0, 0}}},
    {0, -2, {{2, 0, 0}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}}, {0, -1, {{2, 0, 0}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}},
    {0, 0, {{3, 0, 0}, {4, 0, 0}, {7, 0, 0}, {7, 0, 0}}}, {0, -2, {{5, 0, 0}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}},
    {.4f, 0, {{1, 0, .5f}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}}, {0, -4, {{4, 0, 0}, {2, 0, 0}, {7, 0, 0}, {7, 0, 0}}},
    {0, -1.5f, {{0, 0, -.4f}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}}, {.6f, 1.7f, {{1, 0, 0}, {0, 0, 0}, {2, 0, 0}, {7, 0, 0}}},
    {-.4f, 1.7f, {{0, 0, 0}, {1, 0, 0}, {3, 0, 0}, {7, 0, 0}}}, {-.3f, 1.6f, {{0, 0, 0}, {1, 0, 0}, {0, 0, 0}, {7, 0, 0}}},
    {0, 2, {{4, 0, 0}, {5, 0, 0}, {3, 0, 0}, {7, 0, 0}}}, {0, -2, {{6, 0, 0}, {7, 0, 0}, {7, 0, 0}, {7, 0, 0}}},
    {0, 1.7f, {{1, 0, 0}, {0, 0, 0}, {6, 0, 0}, {7, 0, 0}}}, {0, 1.4f, {{1, 0, 0}, {0, 0, 0}, {4, 0, 0}, {7, 0, 0}}}};
struct ForestShenfaMotion {
    std::uint32_t animation;
    std::int32_t end;
    float start, distance, speed;
    std::int32_t mirrored;
};
inline constexpr ForestShenfaMotion forestShenfaMotions[]{{7517, 24, 5, 1.5f, 1, 0}, {7517, 24, 5, 1.5f, 1, 1}, {7516, 45, 0, 1.5f, 1.1f, 0},
    {7515, 28, 0, 1.1f, 1.3f, 0}, {7512, 30, 0, 1, 1.2f, 0}, {7501, 37, 5, 1, 1.5f, 0}, {7501, 37, 5, 1, 1.5f, 0}};
struct ForestQteCommand {
    std::uint32_t button, reserved;
    StepQteCallback success, failure;
    float distance, offset;
};
static_assert(sizeof(ForestQteCommand) == 32);
struct ForestFirefly {
    Vector3 position;
    std::byte reserved0c[36];
    float alpha;
    std::int32_t angle, reserved38, frame, state, onFrames, offFrames, fadeOutFrames, fadeInFrames;
};
static_assert(sizeof(ForestFirefly) == 84 && offsetof(ForestFirefly, state) == 64);
struct ForestFireflyWork {
    ForestFirefly particles[500];
};
static_assert(sizeof(ForestFireflyWork) == 0xa410);
struct ForestFireflyProfile {
    Vector3 centers[4], extent;
    float amplitude, scale;
    std::int32_t count;
};
inline constexpr ForestFireflyProfile forestFireflyProfiles[]{
    {{{99.7f, 100.7f, 91}, {99.7f, 100.7f, 91}, {99.7f, 100.7f, 91}, {99.7f, 100.7f, 91}}, {6, 1.5f, 6}, .8f, .15f, 20},
    {{{99.7f, 100.7f, 91}, {99.7f, 100.7f, 91}, {99.7f, 100.7f, 91}, {99.7f, 100.7f, 91}}, {6, 1.5f, 6}, .6f, .1f, 20},
    {{{102.2f, 103, 90.8f}, {102.2f, 103, 90.8f}, {97.9f, 103, 98.2f}, {97.9f, 103, 98.2f}}, {8, 4, 8}, .8f, .15f, 200},
    {{{99.7f, 100.7f, 91}, {99.7f, 100.7f, 91}, {95.3f, 100.7f, 96.5f}, {95.3f, 100.7f, 96.5f}}, {8, 1.5f, 8}, .4f, .1f, 150},
    {{{99.7f, 100.7f, 91}, {99.5f, 100.7f, 106.5f}, {93.8f, 100.7f, 99.4f}, {93.8f, 100.7f, 99.4f}}, {8, 1.5f, 8}, 1, .2f, 100}};
extern std::int32_t forestStage, forestFailures, forestRetries, forestRainStarted, forestTestObstacle, forestTestSelection;
extern std::int32_t forestLastCommand, forestShenfaTicks, forestSelectedCamera, forestFireflyCount;
extern float forestObstacleDistance, forestFireflyAmplitude, forestFireflyScale;
extern Vector3 forestObstaclePosition;
extern HLib::Task *forestDummyTask, *forestShenfaTask, *forestFireflyTask;
extern const ForestQteCommand forestQteCommands[7];
void InitializeMqte();
void RunMqte(std::uintptr_t);
void QueueForestQte(void *);
void ForestQteMain(void *);
void ForestQteDummyControl(void *);
void ForestQteRunShenfa(void *);
std::int32_t ForestCharacterRoadIndex(shen_name);
void UpdateForestCharacterRoute(shen_name);
void UpdateForestShenfaSpacing();
void QueueForestRoadCameras(std::int32_t, std::int32_t);
void SetForestObstacleCamera(shen_name, std::int32_t);
void InitializeForestFireflies(std::int32_t);
void ForestFireflyLoop(void *);
void ForestFireflyAuthor(void *);
void PositionForestQteReaction(float);
std::int32_t ForestFinalFailure(ForestQteWork &);
extern const ScriptAreaDefinition mqteScriptDefinition;
} // namespace shenmue::s2::areas
