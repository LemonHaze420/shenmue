#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct ConversationBuffer;
}
namespace shenmue::s2::areas {
inline constexpr shen_name ANAN_AREA{SHEN_NAME(ANAN)}, ANAN_EVENT{SHEN_NAME(EVNT)}, ANAN_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name ANAN_MAN{SHEN_NAME(ANA_)}, ANAN_BOARD{SHEN_NAME(brdX) | 0x07000000U}, ANAN_STOOL{SHEN_NAME(A_KY)};
inline constexpr shen_name ANAN_BASE{SHEN_NAME(base)}, ANAN_FREE{SHEN_NAME($fre)}, ANAN_INVALID{0xffffffffU};
inline constexpr shen_name ANAN_NPC_REGION{SHEN_NAME(ccea)}, ANAN_BUTTON{SHEN_NAME(BTBA)}, ANAN_EXIT_AREA{SHEN_NAME(WS00)};
struct AnanWork {
    HLib::Task *caller;
    HLib::Task *self;
    std::uint32_t lifecycle, entry;
    std::uint32_t gameFlags;
    std::int32_t idleFrames;
    float tilt;
    std::int32_t failures;
    ConversationBuffer *directionVoice;
    ConversationBuffer *responseVoice;
    std::int32_t endHoldFrames;
    std::int16_t inputPulse;
    std::uint8_t inputHistory[6];
    std::uint32_t reserved44;
    HLib::Task *ryoTask;
    HLib::Task *ananTask;
    shen_name targets[2];
    Vector3 positions[2];
    std::int32_t transitionFrames[2];
    std::int32_t positionModes[2];
};
struct AnanPakWork {
    HLib::Task *caller;
    std::uint64_t npcState;
    std::int32_t specialEntry, reserved14;
};
struct AnanAuthWork {
    HLib::Task *caller;
    std::uint32_t (*frameCallback)(std::int32_t);
    std::int32_t fadeOut, finishBeforeUnload;
    shen_name author;
    std::uint32_t reserved1c;
    shen_name *characters;
    shen_name *parents;
};
using AnanVoiceCallback = void (*)(std::int64_t, std::intptr_t);
#pragma pack(push, 4)
struct AnanVoiceWork {
    HLib::Task *caller;
    ConversationBuffer *dialogue;
    std::int32_t startDelay, remaining;
    AnanVoiceCallback callback;
    std::int32_t mode;
};
struct AnanWaitWork {
    HLib::Task *caller;
    std::int32_t updatePose;
};
#pragma pack(pop)
struct AnanCameraWork {
    std::int32_t camera;
    shen_name target;
    std::int32_t delay;
    std::uint8_t packageBytes[21];
};
static_assert(sizeof(AnanWork) == 0x88 && offsetof(AnanWork, gameFlags) == 0x18 && offsetof(AnanWork, tilt) == 0x20);
static_assert(offsetof(AnanWork, self) == 0x08 && offsetof(AnanWork, lifecycle) == 0x10 && offsetof(AnanWork, entry) == 0x14);
static_assert(offsetof(AnanWork, directionVoice) == 0x28 && offsetof(AnanWork, responseVoice) == 0x30 && offsetof(AnanWork, endHoldFrames) == 0x38);
static_assert(offsetof(AnanWork, inputPulse) == 0x3c && offsetof(AnanWork, inputHistory) == 0x3e && offsetof(AnanWork, ryoTask) == 0x48);
static_assert(offsetof(AnanWork, targets) == 0x58 && offsetof(AnanWork, positions) == 0x60);
static_assert(offsetof(AnanWork, transitionFrames) == 0x78 && offsetof(AnanWork, positionModes) == 0x80);
static_assert(sizeof(AnanPakWork) == 0x18 && offsetof(AnanPakWork, specialEntry) == 0x10);
static_assert(sizeof(AnanAuthWork) == 0x30 && offsetof(AnanAuthWork, characters) == 0x20);
static_assert(sizeof(AnanVoiceWork) == 0x24 && offsetof(AnanVoiceWork, callback) == 0x18 && sizeof(AnanWaitWork) == 12);
static_assert(offsetof(AnanCameraWork, packageBytes) == 12);
void InitializeAnan();
void RunAnan(std::uintptr_t);
void AnanInitialize(void *);
void AnanWaitForNpc(void *);
void AnanBeginEntry(void *);
void AnanLoadBoard(void *);
void AnanTrackRyo(void *);
void AnanTrackMan(void *);
void AnanFinish(void *);
void AnanFinishAuthor(void *);
void AnanBeginFirstChoice(void *);
void AnanWaitFirstChoice(void *);
void AnanExplain(void *);
void AnanBeginAngry(void *);
void AnanPlayAngryAuthor(void *);
void AnanBeginTimeOver(void *);
void AnanPlayTimeOverAuthor(void *);
void AnanFinishTimeOver(void *);
void AnanSuccess(void *);
void AnanFailure(void *);
void AnanOfferRetry(void *);
void AnanWaitRetry(void *);
void AnanFinishFailure(void *);
void AnanFinishFifthFailure(void *);
void AnanRetry(void *);
void AnanInitializeGame(void *);
void AnanRestartGame(void *);
void AnanRunGame(void *);
void AnanAskStraight(void *);
void AnanWaitStraight(void *);
void AnanInitializeAuthor(void *);
void AnanWaitAuthor(void *);
bool QueueAnanAuthor(shen_name, std::int32_t, std::int32_t);
void QueueAnanVoice(const char *, std::int32_t, std::int32_t, AnanVoiceCallback);
void AnanVoiceLoop(void *);
void AnanVoiceResumeCaller(std::int64_t, std::intptr_t);
void AnanVoiceUnlockInput(std::int64_t, std::intptr_t);
void QueueAnanCamera(std::int32_t, std::int32_t);
void AnanCamera(void *);
void QueueAnanWait();
void AnanWait(void *);
void AnanResetHelp();
void AnanSuspendCharacter(shen_name);
void AnanGetHandPosition(float, Vector3 *);
void AnanSetLiftPose(float);
void AnanSetLiftFrame(float);
void AnanUpdatePose(AnanWork *);
void AnanSetTargets(bool);
void AnanStopLift(std::int32_t, std::int32_t);
std::uint32_t AnanHistoryInput(const AnanWork *, std::int32_t);
void AnanReadInput(AnanWork *);
float AnanInputStep(AnanWork *);
void AnanUpdateDirectionVoice(AnanWork *);
std::int32_t AnanUpdateGame(AnanWork *);
bool AnanIsStraight(const AnanWork *);
extern const ScriptAreaDefinition ananScriptDefinition;
} // namespace shenmue::s2::areas
