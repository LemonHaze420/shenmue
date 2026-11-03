#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_movement.hpp"
#include "s2/areas/common/et_movesp.h"

#define QLGT_AREA SHEN_NAME4('Q', 'L', 'G', 'T')
#define QLGT_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define QLGT_REN SHEN_NAME4('R', 'E', 'N', '_')
#define QLGT_START SHEN_NAME4('S', 'T', 'R', 'B')
#define QLGT_END SHEN_NAME4('E', 'N', 'D', 'B')
#define QLGT_ENEMY SHEN_NAME4('E', 'M', 'Y', 'B')
#define QLGT_BATTLE SHEN_NAME4('B', 'T', 'L', 'B')
#define QLGT_LIGHT SHEN_NAME4('L', 'G', 'T', 'B')
#define QLGT_DRUM SHEN_NAME4('D', 'R', 'M', '0')
#define QLGT_RYO_DUMMY SHEN_NAME4('D', 'U', 'M', 'R')
#define QLGT_REN_DUMMY SHEN_NAME4('D', 'U', 'M', 'L')
#define QLGT_DOOR_OWNER SHEN_NAME4('$', 'f', 'r', 'e')
#define QLGT_BATTLE_DATA SHEN_NAME4('B', 'T', 'B', 'A')
#define QLGT_RETRY_TITLE SHEN_NAME4('R', 'T', 'R', '2')
#define QLGT_FIRST_PROP SHEN_NAME4('F', '0', '6', '0')
#define QLGT_INVALID 0xffffffffU
#define QLGT_DEBUG_ID 3
#define QLGT_DOOR_COUNT 25
#define QLGT_NEGATIVE_ZERO 0x80000000
#define QLGT_SQRT(x) ({ float root_; root_ = __builtin_sqrtf(x); root_; })
#define QLGT_SET_RYO_GRAVITY(operation) \
    do { \
        FTBL.EV_SetCharaControllerFlags(QLGT_RYO, 2, operation); \
        FTBL.EV_SetCharaControllerFlags(QLGT_RYO, 3, operation); \
    } while (0)

typedef struct QlgtEntry {
    int course, step;
} QlgtEntry;
typedef struct QlgtFlashPattern {
    int lights;
    signed char states[16], frames[16];
} QlgtFlashPattern;
typedef struct QlgtFlashWork {
    HLibTask *task;
    const QlgtFlashPattern *pattern;
    int state, transition, index, remaining, reserved18;
} QlgtFlashWork;
typedef struct QlgtPose {
    Vector3 position;
    float yaw;
} QlgtPose;
typedef struct QlgtSafeZone {
    int enabled;
    Vector3 centre;
    float radius;
} QlgtSafeZone;
typedef struct QlgtCourse {
    QlgtFlashPattern *pattern;
    Vector3 boundsFirst, boundsSecond;
    QlgtPose start;
    float startRadius;
    QlgtPose end;
    float endRadius;
    Vector3 goal;
    float goalRadius;
    QlgtPose enemy;
    float enemyRadius;
    QlgtPose battle;
    Vector3 fail;
    float failRadius;
    int enemyCount, reserved8c;
    Vector3 failBattle;
    float failBattleRadius;
    QlgtPose ryoExit, renExit, returnPosition;
    const shen_name *enemies;
    const QlgtSafeZone *safeZones;
    const EventMoveWaypoint *route;
    const signed char *routeActions;
} QlgtCourse;
typedef struct QlgtSequence {
    int reserved00;
    EventChaseWork chase;
    int startSide, endSide, movement, waypoint, grace;
} QlgtSequence;
typedef struct QlgtWork {
    EventReadCharactersWork *characters;
    HLibTask *subtask, *task;
    void *motionBank;
    int previousNavigation;
    QlgtSequence sequence;
    const QlgtEntry *entry;
    const QlgtCourse *course;
    QlgtFlashWork *flash;
    float height;
} QlgtWork;

extern QlgtFlashPattern qlgtPatterns[];
extern shen_name qlgtActors[];
extern shen_name qlgtDrums[];

void QlgtMain(void *argument);
void QueueQlgtEvent(const QlgtEntry *entry);
void QlgtDebugExitCallback(void *data);
void QlgtFinish(QlgtWork *work);
void QlgtSubtaskCallback(void *data);
void QlgtComputeSides(QlgtSequence *sequence);
void QlgtPlaceEnemies(QlgtWork *work, shen_name parent, float radius, const unsigned short *motions, int delay);
void QlgtPlaceBattleEnemies(QlgtWork *work, shen_name parent, float radius, const unsigned short *motions, int delay);
int QlgtIsInSafeZone(const QlgtSafeZone *zones);
void QlgtStart(void *data);
void QlgtStartLecture(void *data);
void QlgtRunLecture(void *data);
void QlgtWaitLecture(void *data);
void QlgtEndLecture(void *data);
void QlgtPlay(void *data);
void QlgtSearch(void *data);
void QlgtSuccess(void *data);
void QlgtFail(void *data);
void QlgtStartBattle(void *data);
void QlgtPlayBattle(void *data);
void QlgtWin(void *data);
void QlgtRetry(void *data);
void QlgtEnd(void *data);
QlgtFlashWork *QlgtQueueFlash(const QlgtFlashPattern *pattern);
void QlgtKillFlash(QlgtFlashWork *flash);
void QlgtSetFlashLights(QlgtFlashWork *flash, int enabled);
void QlgtResetFlash(QlgtFlashWork *flash);
void QlgtFlash(void *data);
void QlgtPrepare(QlgtWork *work);
void QlgtCleanup(QlgtWork *work);
void QlgtClaimDoors(QlgtWork *work);
void QlgtReleaseDoors(QlgtWork *work);
void QlgtTrackDrumTransparency(int mode);
void QlgtQueueSearchPose(shen_name character, shen_name parent, int side, int delay);
int QlgtFirstIntroduction(int side);
int QlgtIntroduction(int side, int course);
int QlgtStartLectureCamera(int enemyCount);
int QlgtLectureFinish(int side);
int QlgtSuccessDialogue(int side, int course, int delay);
int QlgtFailDialogue(int side, int course);
int QlgtCallFailDialogue(int side, int course);
int QlgtWinDialogue(int side, int course);
int QlgtEmptyDialogue(void);
int QlgtReturnDialogue(int delay);
#else
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
#endif
