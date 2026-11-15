#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#pragma once
#include "s2/event_queued_dc.h"
#include "s2/event_utility_dc.h"
typedef struct CwonWork {
    signed char failures, routeChoice, routePass, entry, retained04, audioScene;
    unsigned short retained06, temporarySlot, retained0a;
    Vector3i cachedRotation;
    Vector3 cachedPosition;
    shen_name nextArea;
    int authorState, routeState;
    unsigned int motions[4];
    int smoke;
    HLibTaskCallback startCallback;
    HLibTask *task, *authorTask, *dummyTask, *escapeTask, *temporaryTasks[10];
} CwonWork;
typedef struct CwonMotionWaitWork {
    float frame;
    shen_name character;
    HLibTask *parent;
} CwonMotionWaitWork;
typedef struct CwonAuthorWaitWork {
    int frame;
    HLibTask *parent;
    const shen_name *motionCharacters, *gravityCharacters;
} CwonAuthorWaitWork;
typedef struct CwonLookWork {
    Vector3 position;
    int state;
    shen_name character, target;
    unsigned int operation, flags;
    int duration, bone;
} CwonLookWork;
typedef struct CwonOffsetMotionWork {
    float frame;
    Vector3 position;
    shen_name character;
} CwonOffsetMotionWork;
typedef struct CwonRouteEntry {
    short kind, retained02;
    float threshold;
    int yaw, step, axis, direction;
    shen_name character;
} CwonRouteEntry;
typedef struct CwonTemporaryWork {
    CwonWork *root;
    unsigned int slot;
    int ticks;
} CwonTemporaryWork;

typedef struct CwonConversationWaitWork {
    int ticks;
    void *conversation;
    HLibTask *parent;
} CwonConversationWaitWork;
typedef struct CwonQteWaitWork {
    unsigned short button, frames;
    unsigned short *result;
    HLibTask *parent;
} CwonQteWaitWork;
typedef struct CwonFightWaitWork {
    HLibTask *parent;
    shen_name character;
    int *result;
} CwonFightWaitWork;
typedef struct CwonEscapeWork {
    CwonWork *root;
    HLibTask *task;
    int state;
} CwonEscapeWork;

struct DateTime {
    signed char year, month, day;
    unsigned char week;
    signed char hour, minute, second;
    unsigned char retained[5];
};
void *memset(void *destination, int value, unsigned int bytes);

#define RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define WON SHEN_NAME4('W', 'O', 'N', '_')
#define CENTER SHEN_NAME4('C', 'E', 'N', 'T')
#define DUMMY SHEN_NAME4('D', 'U', 'M', '0')
#define BEE SHEN_NAME4('B', 'D', 'M', 'A')
#define BIRD(n) SHEN_NAME4('B', 'I', 'R', n)
#define BAG SHEN_NAME4('a', '0', '1', '4')
#define SAM SHEN_NAME4('S', 'A', 'M', '_')
#define LLY SHEN_NAME4('L', 'L', 'Y', '_')
#define BASE SHEN_NAME4('B', 'A', 'S', 'E')

extern HLibTask *cwonWaveTask, *cwonSpringTask, *cwonDynamixTask;
extern unsigned int cwonPack, cwonPackIndex, cwonPackState;
extern shen_name CWON_CHARACTERS[], CWON_ADDITIONAL_CHARACTERS[], CWON_AUTHOR_CHARACTERS[], CWON_CHASE_NPC_CHARACTERS[], CWON_BEE_GRAVITY_CHARACTERS[],
    CWON_BEEFAIL_CHARACTERS[], CWON_DODGE_CHARACTERS[], CWON_MARKET_CHARACTERS[], CWON_BEE_PASS_CHARACTERS[];
extern shen_name CWON_CHAIR_CHARACTERS[], CWON_TABLE_CHARACTERS[], CWON_TABLE_MOTION_CHARACTERS[], CWON_MEY_CHARACTERS[], CWON_AFTER_STREET_CHARACTERS[],
    CWON_WAREHOUSE_CHARACTERS[], CWON_LAST_CHASE_CHARACTERS[], CWON_END_GRAVITY_CHARACTERS[], CWON_END_CHARACTERS[], CWON_ENDFAIL_CHARACTERS[],
    CWON_RELEASE_CHARACTERS_1[], CWON_RELEASE_CHARACTERS_2[], CWON_RELEASE_CHARACTERS_3[];
extern shen_name CWON_STREET_STAGE_MODELS[], CWON_STREET_BREAKABLE_MODELS[], CWON_STREET_BROKEN_MODELS[], CWON_MARKET_STAGE_MODELS[],
    CWON_MARKET_BROKEN_MODELS[], CWON_MARKET_MISC_MODELS[];

void AreaCWONMain(int entry);
void CwonApplyLighting(void);
void CwonWaveCallback(void *data);
void CwonSpringCallback(void *data);
void CwonPrepareCharacters(const shen_name *names);
int CwonIsNpcLoadComplete(HLibTask **task);
void CwonMainCallback(CwonWork *work);
void CwonDeleteMotion(CwonWork *work, int index);
void CwonLoadMotion(CwonWork *work, int index);
void CwonLoadAuthorPackages(int load);
void CwonInitialize(CwonWork *work, HLibTask *task, int entry);
void CwonInitializePlayer(CwonWork *work);
void CwonStartPlayerAt(int entry);
void *CwonCreateTask(HLibTask **task, HLibTaskCallback callback, unsigned int size);
void CwonChangeAndWaitMotion(HLibTaskCallback callback, HLibTask *parent, shen_name character, float frame);
HLibTask *CwonQueueMotionOffset(shen_name character, const Vector3 *position, float frame);
void CwonMotionOffsetCallback(CwonOffsetMotionWork *work);
void CwonResetCamera(void);
void CwonGetLocalTransform(shen_name name, Vector3 *position, Vector3i *rotation);
void CwonGetWorldTransform(shen_name name, Vector3 *position, Vector3i *rotation);
void CwonSetPositionMasked(shen_name name, unsigned int flags, const Vector3 *position);
void CwonSetRotationMasked(shen_name name, unsigned int flags, const Vector3i *rotation);
void CwonSetYaw(shen_name name, int yaw);
void CwonAddYaw(shen_name name, int yaw);
void CwonHideAtOrigin(shen_name name, int mode);
void CwonMoveBackward(shen_name name, float distance);
void CwonMoveBackwardAtYaw(shen_name name, int yaw, float distance);
int CwonAdvanceRoute(const CwonRouteEntry *route, int index);
int CwonPositionThreshold(shen_name name, float threshold, int isZ, int reversed);
int CwonTurnTowardYaw(shen_name name, int target, int step, int reversed);
void CwonHideCharacter(shen_name name);
void CwonShowCharacter(shen_name name);
void CwonEnablePositionedCharacter(shen_name name, float x, float y, float z, float ax, float ay, float az);
void CwonRestoreGravity(shen_name name);
void CwonDisableGravity(shen_name name);
unsigned int CwonCreateDust(const Vector3 *position, const Vector3 *volume, const Vector3 *velocity);
void CwonNullsub(void);
void CwonPrepareMotionCharacters(const shen_name *names);
int CwonStartAuthor(int index, HLibTask *parent, int frame, const shen_name *motion, const shen_name *gravity);
void CwonAuthorWaitCallback(CwonAuthorWaitWork *work);
void CwonPrepareGravityCharacters(const shen_name *names);
void CwonRestoreMotionCharacters(const shen_name *names);
void CwonRestoreGravityCharacters(const shen_name *names);
HLibTask *CwonQueueLook(
    shen_name character, shen_name target, int bone, const Vector3 *position, unsigned int operation, unsigned int flags, int duration, int delay);
void CwonLookCallback(CwonLookWork *work);
HLibTask *CwonQueueLoadingTitle(shen_name title, int delay);
void CwonTitleFadeCallback(void *data);
float CwonBallisticHeight(float time, float height);
unsigned int CwonPollQte(CwonWork *work, unsigned int requested, HLibTaskCallback pass, HLibTaskCallback fail);
unsigned int CwonQueueTemporary(CwonWork *work, HLibTaskCallback callback, int delay);
void CwonReleaseAndLoadPackages(HLibTask *task, int freeIndex, int freeSlot, int loadIndex, int loadSlot);
void CwonAuthNullsub(void);
void CwonQueueEscape(CwonWork *work, int entry);
void CwonDestroyEscape(CwonWork *work);
void CwonResetSmoke(CwonWork *work);
HLibTask *CwonQueueFadeWait(HLibTask *parent);
void CwonFreeControlCallback(CwonWork *work);
void CwonRootStreetIntroCallback(CwonWork *work);
void StreetRun(CwonWork *work);
void AdvanceStreetRoute(CwonWork *work);
void BeeQte(CwonWork *work);
void BeePass(CwonWork *work);
void BeeFail(CwonWork *work);
void DodgePass(CwonWork *work);
void DodgeFail(CwonWork *work);
void CwonRootMarketIntroCallback(CwonWork *work);
void CheckStreet(CwonWork *work);
void ChairQte(CwonWork *work);
void ChairPass(CwonWork *work);
void ChairFail(CwonWork *work);
void TableQte(CwonWork *work);
void TablePass(CwonWork *work);
void TableFail(CwonWork *work);
void MeyQte(CwonWork *work);
void MeyPass(CwonWork *work);
void MeyFail(CwonWork *work);
void CheckMarket(CwonWork *work);
void BoxQte(CwonWork *work);
void BoxPass(CwonWork *work);
void BoxFail(CwonWork *work);
void MarketExit(CwonWork *work);
void WorkerPass(CwonWork *work);
void WorkerFail(CwonWork *work);
void DoorQte(CwonWork *work);
void DoorPass(CwonWork *work);
void DoorFail(CwonWork *work);
void CleanQte(CwonWork *work);
void CleanPass(CwonWork *work);
void CleanFail(CwonWork *work);
void CwonRootWarehouseIntroCallback(CwonWork *work);
void CheckWarehouse(CwonWork *work);
void StairsQte(CwonWork *work);
void StairsPass(CwonWork *work);
void StairsFail(CwonWork *work);
void SignQte(CwonWork *work);
void SignPass(CwonWork *work);
void SignFail(CwonWork *work);
void EndQte(CwonWork *work);
void EndPass(CwonWork *work);
void WonSmoke(CwonWork *work);
void EndFail(CwonWork *work);
void RunEnd(CwonWork *work);
void LostEnd(CwonWork *work);
void CwonEndChaseCallback(CwonWork *work);
int CwonDestroyTemporary(CwonWork *work, unsigned int slot);
HLibTask *CwonGetTemporaryTask(CwonTemporaryWork *data);
void *CwonGetTemporaryWork(CwonWork *work, unsigned int slot);
int CwonCountTemporaryTasks(CwonWork *work);
int CwonSetTemporaryTicks(CwonWork *work, unsigned int slot, int ticks);
int CwonFindTemporarySlot(CwonWork *work);
void CwonLoadAudioForPhase(CwonWork *work, int phase);
void CwonInitializeWaveLighting(void);
void CwonBirdCallback(CwonTemporaryWork *work);
void CwonBirdFlightCallback(CwonTemporaryWork *work);
void CwonAdvanceBird(shen_name name, float speed, float height, int tick);
void CwonSetPipeStep(int step);
void CwonPipeCallback(CwonTemporaryWork *work);
void CwonPlayerDustCallback(CwonTemporaryWork *work);
void CwonPlayerOffsetDustCallback(CwonTemporaryWork *work);
void CwonCreateSmokeAt(CwonWork *work, const Vector3 *position);
void CwonFadeWaitCallback(HLibTask **work);
HLibTask *CwonQueueMotionWait(HLibTask *parent, shen_name character, float frame);
void CwonMotionWaitCallback(CwonMotionWaitWork *work);
HLibTask *CwonQueueConversationWait(HLibTask *parent, void *conversation);
void CwonConversationWaitCallback(CwonConversationWaitWork *work);
HLibTask *CwonQueueQteWait(HLibTask *parent, short button, short frames, unsigned short *result);
void CwonQteWaitCallback(CwonQteWaitWork *work);
void CwonFightWaitCallback(CwonFightWaitWork *work);
void CwonDummyControlCallback(void *data);
void CwonPlaceScenarioCharacter(shen_name name, float x, float y, float z, float yaw);
void CwonGetDodgeCallback(CwonTemporaryWork *work);
void CwonMarketPropsCallback(CwonTemporaryWork *work);
void CwonRestoreMarketProps(int showTables);
void CwonPrepareTableProps(void);
void CwonWarehousePropsCallback(CwonTemporaryWork *work);
HLibTask *CwonQueuePipeDynamics(void);
void CwonFallingWorkerCallback(CwonTemporaryWork *work);
void CwonFallingSignCallback(CwonTemporaryWork *work);
void CwonOpeningDoorCallback(CwonTemporaryWork *work);
void CwonTurningPedestrianCallback(CwonTemporaryWork *work);
void CwonAttachBagPose1(shen_name character);
void CwonAttachBagPose2(shen_name character);
void CwonAttachBagPose3(shen_name character);
void CwonAttachBagPose4(shen_name character);
void CwonPrepareChairProps(void);
void CwonSetCollisionTables(void);
void CwonRestoreChair17(void);
void CwonSetFallenTable(void);
void CwonStartOpponentAt(int entry);
void CwonStartWarehouseEscapeCallback(void *data);
void CwonStartStairsEscapeCallback(void *data);
void CwonEscapeStreetCallback(CwonEscapeWork *work);
void CwonEscapeWarehouseCallback(CwonEscapeWork *work);
void CwonAdvanceWonWarehouse(void);
void CwonEscapeStairsCallback(CwonEscapeWork *work);
void CwonFinishEscape(CwonWork *work);
int CwonUpdateBattleIntro(CwonWork *work);
int CwonUpdateStreetWin(CwonWork *work);
int CwonUpdateStreetLost(CwonWork *work);
int CwonUpdateStreetLostOutro(CwonWork *work);
int CwonUpdateMarketWin(CwonWork *work);
int CwonUpdateWarehouseWin(CwonWork *work);
void CwonAttachIntroBag(void);
void CwonInitializeBattleCallback(CwonWork *work);
void CwonStartBattle(CwonWork *work);
void CwonInitializeClubObject(int slot);
void CwonBattleAuthorMarket(CwonWork *work);
void CwonBattleAuthorWarehouse(CwonWork *work);
void CwonBattlePrepareCallback(CwonWork *work);
void CwonBattleCallback(CwonWork *work);
void CwonBattleEndCallback(CwonWork *work);
void CwonBattleAuthorEndCallback(void *data);
void CwonBattleLostCallback(CwonWork *work);
void CwonBattleLostChoiceCallback(CwonWork *work);
void CwonInitializeStreetObjects(void);
void CwonInitializeMarketObjects(void);
void CwonInitializeWarehouseObjects(void);
void CwonInitializeFighters(void);
void CwonShowBattleCharacters(void);
void CwonRestoreBattlePoses(CwonWork *work);
void CwonRestoreBattleProps(CwonWork *work, int restore);

#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct PakAfsPartition;
inline constexpr shen_name CWON_AREA{SHEN_NAME(CWON)}, CWON_TASK{SHEN_NAME(EVNT)}, CWON_RYO{SHEN_NAME(RYO_)}, CWON_WON{SHEN_NAME(WON_)};
inline constexpr shen_name CWON_CENTER{SHEN_NAME(CENT)}, CWON_DUMMY{SHEN_NAME(DUM0)}, CWON_SECOND_DUMMY{SHEN_NAME(DUM1)}, CWON_INVALID{0xffffffffU};
struct CwonEventWork {
    std::uint8_t failures, routeChoice, routePass, entry, reserved04, audioScene;
    std::uint16_t reserved06, temporarySlot, reserved0a;
    std::int32_t birdTimer;
    std::uint32_t reserved10, reserved14;
    float routePosition, routeSpeed;
    std::int32_t routeTimer;
    shen_name nextArea;
    std::int32_t authorState, routeState, battleResult;
    std::uint32_t motions[4];
    std::int32_t smoke;
    HLib::TaskCallback startCallback;
    HLib::Task *task;
    HLib::Task *authorTask;
    HLib::Task *dummyTask;
    HLib::Task *escapeTask;
    HLib::Task *temporaryTasks[10];
    float followHeight;
    std::uint32_t reservedc4;
};
struct CwonTemporaryWork {
    CwonEventWork *root;
    std::uint32_t slot;
    std::int32_t ticks;
};
struct CwonEscapeWork {
    CwonEventWork *root;
    HLib::Task *task;
    std::int32_t state, ticks;
};
struct CwonAuthorWaitWork {
    std::int32_t frame;
    std::uint32_t reserved04;
    HLib::Task *parent;
    const shen_name *motionCharacters;
    const shen_name *gravityCharacters;
};
static_assert(sizeof(CwonEventWork) == 0xc8 && offsetof(CwonEventWork, entry) == 3 && offsetof(CwonEventWork, authorState) == 0x28);
static_assert(offsetof(CwonEventWork, motions) == 0x34 && offsetof(CwonEventWork, task) == 0x50 && offsetof(CwonEventWork, temporaryTasks) == 0x70 &&
    offsetof(CwonEventWork, followHeight) == 0xc0);
static_assert(sizeof(CwonTemporaryWork) == 0x10 && sizeof(CwonEscapeWork) == 0x18 && sizeof(CwonAuthorWaitWork) == 0x20);
extern PakAfsPartition *cwonPack;
extern HLib::Task *cwonDynamixTask;
extern HLib::Task *cwonNpcLoadTask;
extern std::int32_t cwonWaveDisabled, cwonSpringDisabled, cwonPipeStep, cwonSmoke, cwonPackIndex, cwonPackState;
extern Vector3 cwonSmokePosition, cwonSmokeVolume, cwonSmokeVelocity, cwonSmokeVariance;
extern const shen_name CWON_CHARACTERS[], CWON_ADDITIONAL_CHARACTERS[], CWON_AUTHOR_CHARACTERS[], CWON_AUTHORS[];
void AreaCWONInit();
void AreaCWONMain(std::uintptr_t);
extern const ScriptAreaDefinition areaCWONScriptDefinition;
void CwonMainCallback(void *);
void CwonInitialize(CwonEventWork *, HLib::Task *, std::int32_t);
void CwonInitializePlayer(CwonEventWork *);
void CwonStartPlayerAt(std::int32_t);
void CwonWaveCallback(void *);
void CwonSpringCallback(void *);
void CwonPrepareCharacters(const shen_name *);
std::int32_t CwonPollQte(CwonEventWork *, std::int32_t, HLib::TaskCallback, HLib::TaskCallback);
std::uint32_t CwonQueueTemporary(CwonEventWork *, HLib::TaskCallback, std::int32_t);
std::int64_t CwonDestroyTemporary(CwonEventWork *, std::int32_t);
std::int32_t CwonStartAuthor(std::int32_t, HLib::Task *, std::int32_t, const shen_name *, const shen_name *);
void CwonAuthorWaitCallback(void *);
void CwonPrepareMotionCharacters(const shen_name *);
void CwonRestoreMotionCharacters(const shen_name *);
void CwonQueueEscape(CwonEventWork *, std::int32_t);
void CwonDummyControlCallback(void *);
void CwonEndChaseCallback(void *);
void CwonInitializeBattleCallback(void *);
void CwonBattleAuthorCallback(void *);
void CwonBattleCallback(void *);
void CwonBattleEndCallback(void *);
void CwonBattleLostCallback(void *);
void CwonBattleLostChoiceCallback(void *);
void CwonRestoreBattleProps(CwonEventWork *, std::int32_t);
void CwonRestoreBattlePoses(CwonEventWork *);
std::int32_t CwonUpdateVictoryAuthorHands(CwonEventWork *);
void CwonRootStreetIntroCallback(void *);
void CwonRootMarketIntroCallback(void *);
void CwonRootWarehouseIntroCallback(void *);
} // namespace shenmue::s2

#endif
