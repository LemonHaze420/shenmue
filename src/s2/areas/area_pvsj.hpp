#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_library_tail_dc.h"
#include "s2/areas/common/et_pati.h"

#define AREA_PVSJ SHEN_NAME4('P', 'V', 'S', 'J')
#define PVSJ_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define PVSJ_BACKPACK SHEN_NAME4('R', 'U', 'C', 'K')
#define PVSJ_OPPONENT_HAND SHEN_NAME4('V', 'S', 'C', 'H')
#define PVSJ_BATTLE_FONT SHEN_NAME4('B', 'T', 'F', 'T')
#define PVSJ_GOAL_POINT 0x4c4f47
#define PVSJ_DEBUG_ID 2
#define PVSJ_INVALID_PARTITION ((void *)-1)
#define PVSJ_READ_NAME(s) \
    (((unsigned char)(s)[3] << 24 & 0xff000000) | ((unsigned char)(s)[2] << 16 & 0xff0000) | ((unsigned char)(s)[1] << 8 & 0xff00) | (unsigned char)(s)[0])

typedef struct PvsjVoiceLength {
    shen_name code;
    int frames;
} PvsjVoiceLength;
typedef struct PvsjVoiceMessage {
    shen_name code;
    const char *text;
} PvsjVoiceMessage;
typedef struct PvsjVoicePartition {
    const char *name;
    int text, sound;
} PvsjVoicePartition;
typedef struct PvsjCustomer {
    shen_name character, initialPoint, currentPoint;
    int kind;
} PvsjCustomer;
typedef struct PvsjBoardGroup {
    shen_name area, owner;
    shen_name *points;
    PvsjCustomer *customers;
    char *greetings;
    int yaw;
    int *motions;
} PvsjBoardGroup;
typedef struct PvsjCustomerPartition {
    shen_name character;
    int models, hands;
    char highHands;
} PvsjCustomerPartition;
typedef struct PvsjDealer {
    shen_name character;
    int playedFlag;
    int *motions;
    int camera;
} PvsjDealer;
typedef struct PatiMotionCacheEntry {
    shen_name character;
    int motion;
    unsigned int age;
} PatiMotionCacheEntry;
typedef struct PvsjBoardPosition {
    Vector3 position;
    shen_name character;
} PvsjBoardPosition;
typedef struct PvsjEventWork {
    HLibTask *parent;
    HLibTask *task;
    HLibTask *hintTask;
    shen_name argument;
    char bgm[13];
    char sbt[15];
    int soundScene;
    int previousCash;
    int state;
    int sessionCash;
    int newCash;
    int delay;
    shen_name owner;
    shen_name customer;
    int pointCount;
    int admittedCustomers;
    int missingCustomers;
    int customerActive;
    PvsjBoardGroup *boards;
    void *jobPartition;
    int customerPartition;
    int greetingIndex;
    int greeting;
    int stateTicks;
    int startMinute;
    int firstCustomer;
    int callFrames;
    int cameraId;
    shen_name cameraEye, cameraTarget;
    char voicePrefix[16];
    PvsjVoiceLength *voiceLengths;
    void *voiceLoadBuffer;
    void *gamePartition;
} PvsjEventWork;

extern PatiMotionCacheEntry g_patiMotionCache[4];
extern PvsjBoardGroup pvsjBoardGroups[];
extern PvsjCustomerPartition pvsjCustomerPartitions[];
extern PvsjVoicePartition pvsjVoicePartitions[];
extern const PvsjDealer pvsjDealers[];
extern int pvsjCustomerModelIndex;

int sprintf(char *buffer, const char *format, ...);
int strcmp(const char *first, const char *second);
double fabs(double value);

void PvsjEntry(void *argument);
void PvsjReleaseVoice(PvsjEventWork *work);
void PvsjLoadVoice(PvsjEventWork *work, shen_name character);
void SetPatiVoice(const char *prefix, const char *code, int delay);
void PvsjQueueVoice(PvsjEventWork *work, const char *code, int *frame);
void PvsjOfferJob(PvsjEventWork *work);
void PvsjBeginShift(PvsjEventWork *work);
void PvsjCallCustomer(PvsjEventWork *work);
void PvsjWelcomeCustomer(PvsjEventWork *work);
void PvsjExplainPartTimeJob(PvsjEventWork *work);
void PvsjStartCustomerGame(PvsjEventWork *work);
void PvsjCustomerDeclines(PvsjEventWork *work);
void PvsjHandleCustomerInquiry(PvsjEventWork *work);
void PvsjReportShift(PvsjEventWork *work);
void PvsjAskToContinue(PvsjEventWork *work);
void PvsjClosingReport(PvsjEventWork *work);
void PvsjTimeExpired(PvsjEventWork *work);
void PvsjContinueShift(PvsjEventWork *work);
void PvsjOfferPay(PvsjEventWork *work);
void PvsjDeclineContinue(PvsjEventWork *work);
void PvsjAskWithoutProfit(PvsjEventWork *work);
void PvsjRestoreSpeech(PvsjEventWork *work);
void PvsjFinishWithoutProfit(PvsjEventWork *work);
void PvsjShowSessionCash(PvsjEventWork *work);
void PvsjShowWallet(PvsjEventWork *work);
void AreaPVSJMain(void *argument);
void PvsjDebugExitCallback(void *data);
void PvsjFinish(PvsjEventWork *work);
void PvsjShowChoice(int first, int second, int third, int fourth, PvsjEventWork *work);
void PvsjSetCamera(int id, shen_name eye, shen_name target, int delay, PvsjEventWork *work);
void ShowPatiVoiceMessage(void *text);
void PvsjHintCallback(void *data);
int PvsjMoveCustomer(shen_name character, shen_name target, PvsjEventWork *work);
int PvsjRestoreCustomerRoute(shen_name character, PvsjEventWork *work);
void PvsjChooseCustomer(PvsjEventWork *work);
void PvsjMainCallback(void *data);
void PvsjInitializeBoards(PvsjEventWork *work);
void ResetPatiMotionCache(void);
void PvsjSetMotion(shen_name character, int motion, float frame, float speed, unsigned int flags, int delay);
void PvsjSavePlayerRotation(void);
void PvsjRefreshOverlay(int mode, PvsjEventWork *work);
void PvsjSetOverlay(int mode, PvsjEventWork *work);
void PvsjUpdateOverlay(int mode, PvsjEventWork *work);
void PvsjBeginGame(void *argument, PvsjEventWork *work);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/area_pati.hpp"
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct PakAfsPartition;
inline constexpr shen_name PVSJ_AREA{SHEN_NAME(PVSJ)}, PVSJ_TASK{SHEN_NAME(EVNT)}, PVSJ_PLAYER{SHEN_NAME(RYO_)};
struct PvsjCustomer {
    shen_name character, initialPoint, currentPoint;
    std::int32_t kind;
};
struct PvsjBoardGroup {
    shen_name area, owner;
    const shen_name *points;
    PvsjCustomer *customers;
    const std::uint8_t *greetings;
    std::int32_t yaw;
    std::uint32_t reserved24;
    const std::int32_t *motions;
};
struct PvsjEventWork {
    HLib::Task *parent;
    HLib::Task *task;
    HLib::Task *hintTask;
    shen_name argument;
    std::uint32_t reserved1c;
    char bgm[13], sbt[19];
    std::int64_t soundScene;
    std::int32_t previousCash, reserved4c, state, sessionCash, newCash, delay;
    shen_name owner, customer;
    std::int32_t pointCount, admittedCustomers, missingCustomers, customerActive;
    PvsjBoardGroup *boards;
    PakAfsPartition *jobPartition;
    std::int32_t customerPartition, greetingIndex, greeting, stateTicks, startMinute, firstCustomer;
    std::int32_t callFrames, reserveda4, cameraId;
    shen_name cameraEye, cameraTarget;
    std::uint32_t reservedb4;
    char voicePrefix[16];
    PatiVoiceLength *voiceLengths;
    void *voiceLoadBuffer;
    PakAfsPartition *gamePartition;
};
struct PvsjBoardPosition {
    Vector3 position;
    shen_name character;
};
struct PvsjCustomerPartition {
    shen_name character;
    std::int32_t models, hands;
    std::uint8_t highHands;
    std::byte reserved0d[3];
};
static_assert(sizeof(PvsjCustomer) == 0x10 && sizeof(PvsjBoardGroup) == 0x30);
static_assert(sizeof(PvsjEventWork) == 0xe0 && offsetof(PvsjEventWork, state) == 0x50 && offsetof(PvsjEventWork, owner) == 0x60);
static_assert(offsetof(PvsjEventWork, boards) == 0x78 && offsetof(PvsjEventWork, voicePrefix) == 0xb8 && offsetof(PvsjEventWork, voiceLengths) == 0xc8);
static_assert(sizeof(PvsjBoardPosition) == 0x10 && sizeof(PvsjCustomerPartition) == 0x10);
extern PvsjBoardGroup pvsjBoardGroups[];
extern const PvsjCustomerPartition PVSJ_CUSTOMER_PARTITIONS[];
extern PvsjBoardPosition pvsjBoardPositions[32];
extern std::int32_t pvsjBoardCount, pvsjCustomerModelIndex, pvsjPlayerCamera, pvsjCustomerCamera, pvsjSkipMotion;
extern std::int16_t pvsjCameraBlend;
extern Vector3i pvsjSavedPlayerRotation;
extern Vector3 pvsjFilteredEye, pvsjFilteredTarget, pvsjDesiredEye, pvsjDesiredTarget;
extern char pvsjGameArgument[24];
void AreaPVSJInit();
void AreaPVSJMain(std::uintptr_t);
extern const ScriptAreaDefinition areaPVSJScriptDefinition;
void PvsjMainCallback(void *);
void PvsjHintCallback(void *);
void PvsjReleaseVoice(PvsjEventWork *);
void PvsjLoadVoice(PvsjEventWork *, shen_name);
void PvsjQueueVoice(PvsjEventWork *, const char *, std::int32_t *);
void PvsjSetMotion(shen_name, std::int32_t, float, float, std::uint32_t, std::int32_t);
void PvsjSetCamera(PvsjEventWork *, std::int32_t, shen_name, shen_name, std::int32_t);
void PvsjShowChoice(PvsjEventWork *);
void PvsjInitializeBoards(PvsjEventWork *);
std::int32_t PvsjMoveCustomer(PvsjEventWork *, shen_name, shen_name);
std::int32_t PvsjRestoreCustomerRoute(PvsjEventWork *, shen_name);
void PvsjChooseCustomer(PvsjEventWork *);
void PvsjSetOverlay(PvsjEventWork *, std::int32_t);
void PvsjUpdateOverlay(PvsjEventWork *, std::int32_t);
void PvsjBeginGame(PvsjEventWork *);
void PvsjFinish(PvsjEventWork *);
void PvsjOfferJob(PvsjEventWork *);
void PvsjBeginShift(PvsjEventWork *);
void PvsjWelcomeCustomer(PvsjEventWork *);
void PvsjCallCustomer(PvsjEventWork *);
void PvsjExplainPartTimeJob(PvsjEventWork *);
void PvsjStartCustomerGame(PvsjEventWork *);
void PvsjCustomerDeclines(PvsjEventWork *);
void PvsjHandleCustomerInquiry(PvsjEventWork *);
void PvsjReportShift(PvsjEventWork *);
void PvsjAskToContinue(PvsjEventWork *);
void PvsjClosingReport(PvsjEventWork *);
void PvsjTimeExpired(PvsjEventWork *);
void PvsjContinueShift(PvsjEventWork *);
void PvsjOfferPay(PvsjEventWork *);
void PvsjDeclineContinue(PvsjEventWork *);
void PvsjAskWithoutProfit(PvsjEventWork *);
void PvsjRestoreSpeech(PvsjEventWork *);
void PvsjFinishWithoutProfit(PvsjEventWork *);
void PvsjShowWallet(PvsjEventWork *);
} // namespace shenmue::s2
#endif
