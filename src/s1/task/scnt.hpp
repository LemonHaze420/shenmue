#pragma once

#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
namespace HLib {
struct TaskData;
struct Task;
}

struct ScenarioContext;
struct ScenarioTaskData;
#pragma pack(push, 1)
struct ScntDialogueTokenReader {
    const std::uint8_t *cursor = nullptr;
    std::int32_t encodingMode = 0;
    std::uint32_t reserved0c = 0;
    bool (*isLeadByte)(std::uint8_t) = nullptr;
    bool (*isTrailByte)(std::uint8_t) = nullptr;
    std::uint16_t pendingLead = 0;
    std::uint16_t reserved22 = 0;
    std::uint32_t tokenKind = 0;
    std::uint16_t token = 0;
};

struct ScntDialogueState {
    std::int32_t columns = 46;
    std::array<std::uint8_t, 4> config{2, 0, 0, 0};
    ScntDialogueTokenReader tokenReader{};
    std::array<std::byte, 6> reserved32{};
    std::uint32_t flags = 0;
};

struct ScntDialogueWork {
    ScntDialogueState reader{};
    std::int32_t surfaceAllocationDelay = 0;
    std::uint32_t surfaceAllocated = 0;
    std::int32_t active = 0;
    std::int32_t mode = 0;
};
#pragma pack(pop)

struct FreeTalkEvent {
    std::uint32_t characterName = 0;
    std::int32_t command = -1;
};



extern std::int8_t g_freeTalkBlendFrames;
extern std::uint8_t g_freeTalkLetterExchangeFrameCounter;
extern std::uint8_t g_freetalk_timer_frames;
extern std::uint8_t g_freeTalkCurrentProgress;
extern std::uint32_t g_freeTalkPartnerCrossedHemisphere;
extern std::uint8_t g_has_free_talk_time_limit;
extern std::uint8_t g_freetalk_timer_complete_flag;
extern std::uint8_t g_freeTalkMotionResetCleanupFrameCounter;



extern std::uint32_t g_freeTalkCurrentCharaId;
extern std::uint32_t SecondChara;
extern std::uint8_t g_freeTalkCurrentCharaIsContextOperand;
extern std::array<std::uint32_t, 8> g_talk_name_tbl;
extern std::array<std::uint32_t, 8> g_freeTalkConstraintCharaByTalkSlot;
extern std::uint8_t g_freeTalkParticipantMobjRestorePending;

extern std::uint8_t g_freeTalkRyoConstrained;
extern std::array<std::uint8_t, 10> g_freeTalkFaceBlendUpdatePending;
extern Vector3 g_freeTalkLookTargetPos;
extern const Vector3 *g_freeTalkTargetPoint;
extern std::uint8_t g_freeTalkMotionResetPending;
extern std::uint32_t g_pendingFreeTalkMotionResetCharaId;

extern std::int32_t g_FreeTalk_Status;
extern std::int32_t g_pendingFreeTalkCommandCode;

extern std::uint8_t g_freeTalkDelayedFlagSetPending;
extern std::uint8_t g_freetalk_flag_0;
extern std::array<std::uint32_t, 8> g_prevFreeTalkPartner;
extern std::int32_t g_tmp_chara_rot_y;

extern std::uint8_t g_freeTalkFacePairStarted;
extern float g_freeTalkFaceBaselineHandY;
extern float g_freeTalkFaceBaselineUpperY;
extern std::uint8_t g_freeTalkFaceBaselinesCaptured;



extern std::uint8_t g_freeTalkConstraintsDisabled;
extern std::uint8_t bRyoFacingDirectionFlag;
extern std::uint8_t g_freeTalkMotionResetCleanupPending;
extern std::uint8_t g_faceFlag;
extern std::uint8_t g_freeTalkFaceBlendSuppressLook;
extern std::array<std::uint32_t, 8> g_freeTalkMotionIdBySlot;
extern std::array<std::int32_t, 8> g_freeTalkRelativePosModeBySlot;
extern std::array<std::int32_t, 8> g_freeTalkFacingBySlot;
extern std::array<std::uint8_t, 8> g_freeTalkParticipantRestorePending;
extern std::array<std::uint8_t, 8> g_freeTalkConstraintCompletionPending;
extern std::uint32_t g_freeTalkPairHeadSource;
extern std::uint32_t g_freeTalkPairLookActor;
extern std::array<std::uint32_t, 10> g_freeTalkPairHeadSources;
extern std::array<std::uint32_t, 10> g_freeTalkPairLookActors;
extern std::array<std::uint8_t, 10> g_freeTalkPairSetupPending;
extern std::array<std::int32_t, 10> g_freeTalkPairFacing;


extern std::array<std::uint8_t, 10> g_freeTalkFaceBlendFrame;
extern Vector3 g_freeTalkFaceBlendPoint;
extern std::int32_t g_freeTalkFaceBlendMotionGate;
extern std::int32_t g_freeTalkRyoFacingYaw;
extern std::int32_t g_freeTalkPendingAnimState;

static_assert(sizeof(ScntDialogueTokenReader) == 0x2a);
static_assert(offsetof(ScntDialogueTokenReader, tokenKind) == 0x24);
static_assert(offsetof(ScntDialogueTokenReader, token) == 0x28);
static_assert(sizeof(ScntDialogueState) == 0x3c);
static_assert(offsetof(ScntDialogueState, tokenReader) == 0x08);
static_assert(offsetof(ScntDialogueState, flags) == 0x38);
static_assert(sizeof(ScntDialogueWork) == 0x4c);
static_assert(offsetof(ScntDialogueWork, active) == 0x44);
static_assert(offsetof(ScntDialogueWork, mode) == 0x48);
static_assert(sizeof(FreeTalkEvent) == 8);

void SCNT_ShowSubtitleText(HLib::Task *scenarioTask, const char *text, std::uint32_t speakerClass = 2);
void SCNT_ClearSubtitleText();
bool SCNT_IsTimeLimitComplete();

void SCNT_ResetRuntime();
void SCNT_ResetConversationState();
void SCNT_InitializeScenarioDialogue(ScenarioTaskData &data, HLib::Task *windTask);
HLib::Task *SCNT_GetTask();
void SCNT_EnsureFreeTalkRuntime();


std::int32_t SCNT_GetAndResetFreeTalkCommand();

bool SCNT_IsFreeTalkPairSetupPending(std::size_t index);
std::int32_t SCNT_GetFreeTalkStatus();
std::uint32_t SCNT_GetFreeTalkCurrentCharacter();
void Freetalk_Motion_Main(HLib::TaskData *taskData);
void Freetalk_Motion_UpdateConstraints(std::uint32_t characterName);
void talkRestoreParticipantsAndEchoFNCSMinus8(std::uint32_t characterName);
void FreeTalk_EvaluateTimeLimitAndUpdateFaceTarget(std::int32_t command, std::uint32_t characterName);
void talkClearCharaFaceLookAt(std::uint32_t characterName);


std::int64_t SCNT_ShowSCNFSubtitleText(ScenarioContext *context, void *rawParams);
std::int64_t SCNT_WaitSCNFSubtitleText(ScenarioContext *context, void *rawParams);

// Shenmue scenario functions 151, 418, 427, and 444.
void Script_GetTaskData_0(ScenarioContext *context, void *rawParams);
void Script_EnqueueSCNT(ScenarioContext *context, void *rawParams);
void Script_ProcessDialogueData(ScenarioContext *context, void *rawParams);
void SCNT_Enqueue(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
