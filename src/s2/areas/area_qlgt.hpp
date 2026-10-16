#pragma once
#include "s2/area_event.hpp"
#include "s2/event_movement.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
namespace shenmue::s2::areas {
void AreaQLGTInit();
void AreaQLGTMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaQLGTScriptDefinition;
namespace qlgt {
inline constexpr shen_name area{SHEN_NAME(QLGT)}, taskName{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)};
inline constexpr shen_name start{SHEN_NAME(STRB)}, end{SHEN_NAME(ENDB)}, enemy{SHEN_NAME(EMYB)}, battle{SHEN_NAME(BTLB)};
inline constexpr shen_name light{SHEN_NAME(LGTB)}, fail{SHEN_NAME(FAIL)}, drum{SHEN_NAME(DRM0)}, ryoDummy{SHEN_NAME(DUMR)}, renDummy{SHEN_NAME(DUML)};
inline constexpr shen_name invalid{0xffffffffU}, doorOwner{SHEN_NAME($fre)}, battleData{SHEN_NAME(BTBA)}, retryTitle{SHEN_NAME(RTR2)};
struct Entry {
    std::int32_t course, step;
};
struct FlashPattern {
    std::int32_t lights;
    std::int8_t states[16], frames[16];
};
struct FlashWork {
    HLib::Task *task;
    const FlashPattern *pattern;
    std::int32_t state, transition, index, remaining;
    std::uint64_t reserved20;
    std::int32_t delay;
    std::uint32_t reserved2c;
};
struct Pose {
    Vector3 position;
    float yaw;
};
struct SafeZone {
    std::int32_t enabled;
    Vector3 centre;
    float radius;
};
struct Course {
    const FlashPattern *pattern;
    Vector3 boundsFirst, boundsSecond;
    Pose start;
    float startRadius;
    Pose end;
    float endRadius;
    Vector3 goal;
    float goalRadius;
    Pose enemy;
    float enemyRadius;
    Pose battle;
    Vector3 fail;
    float failRadius;
    std::int32_t enemyCount, reserved90;
    Vector3 failBattle;
    float failBattleRadius;
    Pose ryoExit, renExit, returnPosition;
    std::uint32_t reservedd4;
    const shen_name *enemies;
    const SafeZone *safeZones;
    const EventMovementWaypoint *route;
    const std::uint8_t *routeActions;
};
struct Work {
    EventReadCharactersWork *characters;
    std::uint64_t reserved08;
    HLib::Task *task;
    std::int32_t motionBank, previousNavigation;
    std::uint64_t reserved20;
    EventChaseWork chase;
    std::int32_t startSide, endSide, movement, waypoint, grace;
    const Entry *entry;
    const Course *course;
    FlashWork *flash;
    float height;
    std::int32_t remaining, retry;
    std::uint32_t reserved3c4;
};
static_assert(sizeof(Entry) == 8 && sizeof(FlashPattern) == 36 && sizeof(FlashWork) == 48);
static_assert(sizeof(Course) == 248 && offsetof(Course, enemies) == 216 && offsetof(Course, route) == 232);
static_assert(sizeof(Work) == 0x3c8 && offsetof(Work, chase) == 0x28 && offsetof(Work, startSide) == 0x38c);
static_assert(offsetof(Work, entry) == 0x3a0 && offsetof(Work, remaining) == 0x3bc);
extern const Entry entries[4];
extern const Course courses[4];
extern shen_name actors[9];
extern const shen_name doors[25], hiddenDoors[4], lightDefinitions[4];
extern const std::int32_t lightSlots[4];
extern const std::uint16_t idleMotions[8], battleMotions[10];
extern const std::int32_t ordinaryStyles[6], ordinaryHealth[6], advancedStyles[4], advancedHealth[4];
extern const EventMovementMotions routeMotions;
extern const EventChaseWork chaseDefaults;
extern HLib::Task *flashTask;
void Film(std::int32_t frames, std::int32_t from, std::int32_t to, std::int32_t delay = 0);
void Camera(std::int32_t camera, shen_name anchor, std::int32_t delay = 0, bool negativeZero = false);
void Prepare(Work &work);
void Cleanup(Work &work);
FlashWork *QueueFlash(const FlashPattern *pattern);
void ResetFlash(FlashWork &flash);
void TrackDrumTransparency(bool opaque);
void PlaceEnemies(Work &work, float radius);
void PlaceBattleEnemies(Work &work, float radius);
bool IsInSafeZone(const SafeZone *zones);
void Start(void *);
void StartLectureWait(void *);
void StartLecture(void *);
void RunLecture(void *);
void WaitLecture(void *);
void EndLecture(void *);
void Play(void *);
void Search(void *);
void Success(void *);
void SuccessWait(void *);
void Fail(void *);
void FailWait(void *);
void StartBattle(void *);
void PlayBattle(void *);
void Win(void *);
void WinWait(void *);
void Retry(void *);
void End(void *);
void QueueSearchPose(std::int32_t side, std::int32_t delay);
std::int32_t FirstIntroduction(std::int32_t side);
void SkipFirstIntroduction(std::int32_t side);
std::int32_t Introduction(std::int32_t side, std::int32_t course);
void SkipIntroduction(std::int32_t side);
std::int32_t LectureFinish(std::int32_t side);
std::int32_t SuccessDialogue(std::int32_t side, std::int32_t course);
std::int32_t FailDialogue(std::int32_t side, std::int32_t course);
std::int32_t WinDialogue(std::int32_t side, std::int32_t course);
std::int32_t ReturnDialogue();
} // namespace qlgt
} // namespace shenmue::s2::areas
