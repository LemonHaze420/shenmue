#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {

enum ScenarioFreeTalkFlag : std::uint32_t {
    ScenarioTalkInterrupted = 0x2U,
    ScenarioTalkBinaryPrompt = 0x4U,
    ScenarioTalkChoicePrompt = 0x8U,
    ScenarioTalkLinkedPrompt = 0x10U,
    ScenarioTalkContinuePrompt = 0x20U,
    ScenarioTalkLineBlock = 0x40U,
    ScenarioTalkAllowVoiceOverlap = 0x80U,
    ScenarioTalkSavedScript = 0x100U,
    ScenarioTalkNextCue = 0x200U,
    ScenarioTalkCharacterActive = 0x400U,
    ScenarioTalkPresentationActive = 0x800U,
    ScenarioTalkWaitAction = 0x1000U,
    ScenarioTalkWaitStart = 0x2000U,
    ScenarioTalkWaitFollow = 0x4000U,
    ScenarioTalkWaitEnd = 0x8000U,
    ScenarioTalkSelected = 0x10000U,
    ScenarioTalkPreviouslySelected = 0x20000U,
    ScenarioTalkSuppressCamera = 0x40000U,
    ScenarioTalkResumeReached = 0x80000U,
    ScenarioTalkPromptClockStarted = 0x100000U,
    ScenarioTalkAllowUnforcedActivation = 0x200000U,
    ScenarioTalkPlayerUnavailable = 0x400000U,
    ScenarioTalkAlreadyActivated = 0x800000U,
    ScenarioTalkParticipantsAbsent = 0x1000000U,
    ScenarioTalkActivationBlocked = 0x2000000U,
    ScenarioTalkCompletedResume = 0x4000000U,
    ScenarioTalkResumeAfterSection = 0x8000000U,
    ScenarioTalkRestartSection = 0x10000000U,
    ScenarioTalkMoneyIconOpen = 0x20000000U,
};

inline constexpr std::size_t ScenarioFreeTalkRecordCapacity = 16;
#pragma pack(push, 4)




struct ScenarioFreeTalkRecord {
    shen_name name;
    std::uint8_t reserved004;
    std::uint8_t executionState;
    std::uint8_t reserved006;
    std::uint8_t field007;
    std::uint32_t reserved008;
    std::int32_t selectionIndex;
    std::uint8_t locationCount;
    std::uint8_t approachPointCount;
    std::uint8_t participantCount;
    std::uint8_t keyValueCount;
    std::uint16_t characterStateIndex;
    std::uint16_t dialogueFlagId;
    std::uint32_t flags;
    std::uint16_t dialogueTimeout;
    std::uint16_t dialogueChoiceIndex;
    const std::byte *participants;
    const std::byte *locations;
    const std::byte *vectorData;
    const Vector3 *selectedApproachPoint;
    const char *voicePath;
    const std::byte *keyValues;
    const std::byte *script;
    const std::byte *lineTable;
    const std::byte *firstDialogueLine;
    const std::byte *dialoguePreparation;
    std::uint32_t ryoSpeaking;            // +0x070, shenPrepareDialogue's speaker mode
    std::uint32_t reserved074;
    const std::byte *sourceRecord;
    const std::byte *scriptCursor;
    const std::byte *savedScriptCursor;
    const std::byte *returnScriptCursor;
    const std::byte *pendingScriptCursor;
    std::array<std::uint16_t, 4> timers;
    ScenarioFreeTalkControl *control;     // +0x0a8, shared voice-group control or localControl
    ScenarioFreeTalkControl localControl;
    ScenarioFreeTalkCandidate *candidate;
    const std::byte *defaultRoot;
    const std::byte *alternateRoot;
    std::int32_t requestedRoot;
    std::uint32_t reserved0dc;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct ScenarioFreeTalkCandidate {
    shen_name name;
    std::uint16_t characterStateIndex;
    std::int8_t availability;
    std::int8_t searchState;
    std::uint32_t flags;
    float proximity;
    Character *character;
    void *motionWork;
    ScenarioFreeTalkRecord *record;
    ScenarioFreeTalkCandidate *previous;
    ScenarioFreeTalkCandidate *next;
};
#pragma pack(pop)

#pragma pack(push, 4)


struct FreeTalkComponent {
    std::array<ScenarioFreeTalkCandidate, 4> candidates;
};
#pragma pack(pop)

static_assert(offsetof(ScenarioFreeTalkRecord, selectionIndex) == 0x0c);
static_assert(offsetof(ScenarioFreeTalkRecord, dialogueFlagId) == 0x16);
static_assert(offsetof(ScenarioFreeTalkRecord, flags) == 0x18);
static_assert(offsetof(ScenarioFreeTalkRecord, control) == 0xa8);
static_assert(offsetof(ScenarioFreeTalkRecord, localControl) == 0xb0);
static_assert(offsetof(ScenarioFreeTalkRecord, dialogueTimeout) == 0x1c);
static_assert(offsetof(ScenarioFreeTalkRecord, participants) == 0x20);
static_assert(offsetof(ScenarioFreeTalkRecord, voicePath) == 0x40);
static_assert(offsetof(ScenarioFreeTalkRecord, sourceRecord) == 0x78);
static_assert(offsetof(ScenarioFreeTalkRecord, scriptCursor) == 0x80);
static_assert(sizeof(ScenarioFreeTalkRecord) == 0xe0);
static_assert(sizeof(ScenarioFreeTalkCandidate) == 0x38);
static_assert(offsetof(ScenarioFreeTalkRecord, candidate) == 0xc0);
static_assert(offsetof(ScenarioFreeTalkRecord, requestedRoot) == 0xd8);
static_assert(sizeof(FreeTalkComponent) == 0xe0);
static_assert(offsetof(FreeTalkComponent, candidates) == 0x00);
static_assert(offsetof(FreeTalkComponent, candidates) + sizeof(ScenarioFreeTalkCandidate) == 0x38);
static_assert(offsetof(FreeTalkComponent, candidates) + 2 * sizeof(ScenarioFreeTalkCandidate) == 0x70);
static_assert(offsetof(FreeTalkComponent, candidates) + 3 * sizeof(ScenarioFreeTalkCandidate) == 0xa8);

void scenUpdateFreeTalkRuntime(ScenarioEventRuntime *scenario);
std::int64_t StartScenarioFreeTalkRecord(shen_name character, std::int16_t root);
std::int64_t StopScenarioFreeTalkRecord();

extern std::int32_t scenarioTalkPromptResumed;
extern std::int32_t scenarioFollowPromptShown;
ScenarioFreeTalkRecord *FindMutableScenarioFreeTalkRecord(shen_name character);
std::int32_t GetScenarioFollowEligibility(shen_name character);
void ApplyScenarioFaceLook(std::uint32_t command);

bool ScenarioCharacterIsRyo(shen_name name);
// These selectors discover visibility/control state; they do not activate a
// conversation or populate the participant span.
bool IsScenarioCharacterForcedVisible(shen_name character);
bool HasScenarioCharacterControl();
bool scenCheckActiveRecordBusy();
bool IsScenarioBusy();
bool IsScenarioBusyForMainIcon();
void ResetScenarioFreeTalkResumeFlags();
std::span<shen_name, 32> GetScenarioCharacterAdmissionNames();
std::span<const shen_name> GetScenarioParticipants();
shen_name GetActiveScenarioCharacter();
bool IsActiveScenarioCameraSuppressed();
bool IsScenarioButtonAdmissionEnabled();
ScenarioFreeTalkCandidate *GetScenarioCandidateList();
inline constexpr shen_name SCENARIO_FREE_TALK_COMPONENT{SHEN_NAME(FRTK)};
ScenarioFreeTalkRecord *AllocateScenarioFreeTalkCharacter(Character *character);
bool AttachScenarioFreeTalkCharacter(Character *character, std::span<const std::byte> scnf);
void InitiateCleanupForSceneChara(shen_name name);
std::span<const std::byte> GetScenarioFreeTalkSource(const ScenarioFreeTalkRecord &record);
std::span<const ScenarioFreeTalkRecord> GetScenarioFreeTalkRecords();
std::span<const std::byte> GetScenarioFreeTalkExtraRecord();
const ScenarioFreeTalkRecord *FindScenarioFreeTalkRecord(shen_name character);
bool IsScenarioFreeTalkInitialized();

struct ScenarioFreeTalkContainerHeader {
    shen_name name;
    std::uint32_t wholeSize;
    std::uint32_t version;
    std::uint32_t flags;
};

struct ScenarioFreeTalkSourceHeader {
    shen_name name;
    std::uint32_t size;
    std::uint8_t participantCount;
    std::uint8_t reserved09;
    std::uint8_t locationCount;
    std::uint8_t keyValueCount;
    std::uint32_t reserved0c;
    std::uint32_t voicePathOffset;
    std::uint32_t scriptOffset;
    std::uint32_t lineTableOffset;
    std::uint32_t reserved1c;
    std::uint32_t participantOffset;
    std::uint32_t vectorDataOffset;
    std::uint32_t locationOffset;
    std::uint32_t keyValueOffset;
};

static_assert(sizeof(ScenarioFreeTalkContainerHeader) == 0x10);

static_assert(sizeof(ScenarioFreeTalkSourceHeader) == 0x30);

static_assert(offsetof(ScenarioFreeTalkSourceHeader, voicePathOffset) == 0x10);

static_assert(offsetof(ScenarioFreeTalkSourceHeader, participantOffset) == 0x20);

struct ScenarioStepCharacterState {
    shen_name id{};
    const ScenarioFreeTalkRecord *record{};
    Character *character{};
    HLib::Task *handlerTask{};
};

struct ScenarioFreeTalkStorage {
    std::array<ScenarioFreeTalkRecord, ScenarioFreeTalkRecordCapacity> records{};
    std::byte *externalData{};
    std::size_t externalSize{};
    const std::byte *extraRecord{};
    std::size_t extraRecordSize{};
    std::size_t recordCount{};
    ScenarioStepCharacterState stepCharacter{};
};

} // namespace shenmue::s2
