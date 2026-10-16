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
