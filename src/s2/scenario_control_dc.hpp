#pragma once

#include "s2/scenario_talk_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define SHENMUE_SCNC_CONSTANT static const
#define SHENMUE_SCNC_JOIN_(a, b) a##b
#define SHENMUE_SCNC_JOIN(a, b) SHENMUE_SCNC_JOIN_(a, b)
#define SHENMUE_SCNC_CHECK(condition) typedef char SHENMUE_SCNC_JOIN(LayoutCheckscenario_control_dc, __LINE__)[(condition) ? 1 : -1]

namespace shenmue {
namespace s2 {
extern std::int32_t g_scenarioTalkPromptResumed, g_scenarioFollowPromptShown;

SHENMUE_SCNC_CONSTANT std::size_t kScenarioContextFlagsOffset = 4, kScenarioFreeTalkFlagsOffset = 0x18;
std::int32_t HasScenarioCharacterControl();

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsScenarioBusy();
bool IsScenarioFollowPromptAvailable();
bool IsScenarioBusyForMainIcon();
shen_name GetActiveScenarioCharacter();
bool scenIsRecordRunning(shen_name character);
std::int32_t scenCheckActiveRecordBusy();
bool IsActiveScenarioCameraSuppressed();
std::int32_t StartScenarioFreeTalkRecord(shen_name character, std::int32_t root);
void StopScenarioFreeTalkRecord();
void InitiateCleanupForSceneChara(shen_name character);
struct ConversationBuffer;
struct ConversationDialogueRecord;
extern ConversationDialogueRecord g_scenarioCameraRecords[24];
struct ScenarioEventContextStorage {
    ScenarioEventContextPrefix prefix;
    std::int16_t lipClip[0xa0];
    std::int32_t randomSelectionCount;
    std::uint32_t randomSelections[3];
    float selectedTargetDistanceSquared;
    std::uint32_t selectedTargetAbsYaw;
    shen_name pendingCharacter;
};
struct ScenarioDialogueCue {
    float triggerTime;
    const void *payload;
    shen_name speaker;
    std::uint32_t retained0c;
};
SHENMUE_SCNC_CHECK(sizeof(ScenarioEventContextStorage) == 0x194);
SHENMUE_SCNC_CHECK(offsetof(ScenarioEventContextStorage, prefix) == 0);
SHENMUE_SCNC_CHECK(offsetof(ScenarioEventContextStorage, lipClip) == 0x38);
SHENMUE_SCNC_CHECK(offsetof(ScenarioEventContextStorage, randomSelectionCount) == 0x178);
SHENMUE_SCNC_CHECK(offsetof(ScenarioEventContextStorage, randomSelections) == 0x17c);
SHENMUE_SCNC_CHECK(offsetof(ScenarioEventContextStorage, pendingCharacter) == 0x190);
SHENMUE_SCNC_CHECK(sizeof(ScenarioDialogueCue) == 0x10);
SHENMUE_SCNC_CHECK(offsetof(ScenarioDialogueCue, payload) == 4);
SHENMUE_SCNC_CHECK(offsetof(ScenarioDialogueCue, speaker) == 8);
SHENMUE_SCNC_CHECK(offsetof(ScenarioDialogueCue, retained0c) == 0x0c);
SHENMUE_SCNC_CONSTANT std::size_t kScenarioAdmissionCount = 32, kScenarioDialogueCueCount = 101;
extern std::int32_t g_scenarioFreeTalkPhase;
extern shen_name g_scenarioCharacterAdmissionNames[kScenarioAdmissionCount];
extern Vector3 g_scenarioAdmissionPositions[kScenarioAdmissionCount];
extern std::int32_t g_scenarioSelectionCount;
extern std::uint32_t g_scenarioLineStartTicks;
extern ScenarioDialogueCue g_scenarioDialogueCues[kScenarioDialogueCueCount];
extern std::int32_t g_scenarioActionQueueId, g_scenarioAcknowledgementQueueId;
extern ConversationBuffer *g_scenarioCurrentDialogue;
extern std::uint32_t g_scenarioRetainedState94c, g_scenarioRetainedState950;
extern std::uint32_t g_scenarioLookTargetActive;
extern std::int32_t g_scenarioRetainedLookTarget;
extern std::int32_t g_scenarioTalkAreaActive;
extern const std::byte *g_externalFreeTalkRecords, *g_extraFreeTalkRecord;
extern MT7Node *g_scenarioStepModels[7];
extern std::int32_t g_scenarioSelectedFollowFlag;
void SetModePerCurrentStep(std::int32_t mode);
void SpawnMoveScroll();
void ResetScenarioGlobals();
void ReadFreeTalk();
void EndScenarioPlayerPresentation(std::int32_t mode);
void ConsumeScenarioCameraRestoreRequest();
void InterruptScenarioFreeTalkRecord(ScenarioFreeTalkCharacter *record);
ScenarioEventContextPrefix *InitializeScenarioEventContext(HLib::Task *task);
void SetNewScenarioTask(const std::byte *scnf);
std::int32_t IsSystemScenarioRecordName(shen_name name);
struct ScenarioGameplayFlagState;
void RestoreScenarioGameplayFlags(const ScenarioGameplayFlagState *state);
void SaveScenarioGameplayFlags(ScenarioGameplayFlagState *state);
#endif

} // namespace s2
} // namespace shenmue

#undef SHENMUE_SCNC_CONSTANT
#undef SHENMUE_SCNC_JOIN_
#undef SHENMUE_SCNC_JOIN
#undef SHENMUE_SCNC_CHECK

#endif
