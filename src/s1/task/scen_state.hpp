#pragma once
#include "s1/task/scen_internal.hpp"
#include "s1/task/scen_transitions.hpp"

namespace shenmue::s1::scen_detail {

inline constexpr std::uint32_t kScenarioFlagActiveRuntimeObject = 0x1U;

inline constexpr std::uint32_t kScenarioFlagConversationMarkers = 0x4U;

inline constexpr std::uint32_t kScenarioFlagTimelineActive = 0x8U;

inline constexpr std::uint32_t kScenarioFlagConversationCamera = 0x10U;

inline constexpr std::uint32_t kScenarioFlagInteractiveFocused = 0x20U;

inline constexpr std::uint32_t kScenarioFlagCoreRanThisFrame = 0x100U;

inline constexpr std::uint32_t kScenarioFlagUnknown200 = 0x200U;

inline constexpr std::uint32_t kScenarioFlagInteractionResume = 0x2000U;

inline constexpr std::uint32_t kScenarioFlagQueueConfirmPrompt = 0x4000U;

inline constexpr std::uint32_t kScenarioFlagInteractionModal = 0x20000U;

inline constexpr std::uint32_t kScenarioFlagPadBlockOverride = 0x8000U;

inline constexpr std::uint32_t kScenarioFlagAuthorGameState = 0x10000U;

inline constexpr std::uint32_t kRuntimeFlagStateTransitionLatch = 0x2000U;

inline constexpr std::uint32_t kRuntimeFlagInteractionResume = 0x4000U;

inline constexpr std::uint32_t kRuntimeFlagPendingWait = 0x10000U;

inline constexpr std::uint32_t kRuntimeFlagPendingFailure1 = 0x20000U;

inline constexpr std::uint32_t kRuntimeFlagPendingFailure2 = 0x40000U;

inline constexpr std::uint32_t kRuntimeFlagPendingFailure3 = 0x80000U;

inline constexpr std::uint32_t kRuntimeFlagConversationCamera = 0x40U;

inline constexpr std::uint32_t kRuntimeFlagConversationCameraActive = 0x400000U;

inline constexpr std::uint32_t kRuntimeFlagStateDecode = 0x400U;

inline constexpr std::uint32_t kRuntimeFlagStateActive = 0x100000U;

inline constexpr std::uint32_t kRuntimeFlagStateEntered = 0x200000U;

inline constexpr std::uint32_t kRuntimeFlagExitLatched = 0x4000000U;


inline constexpr std::uint32_t kScenarioFlagSuppressAutoSubtitleStart = 0x800U;

inline constexpr std::uint32_t kScenarioFlagSubtitleLineActive = 0x1000U;



inline constexpr std::uint32_t kRuntimeFlagSilentTimeline = 0x80U;

inline constexpr std::uint32_t kRuntimeFlagSubtitleCleanupPending = 0x800U;

inline constexpr std::uint32_t kRuntimeFlagTimelineAdvancePending = 0x1000U;

// SCEN_TaskData::dialogue.wndb_state.flags, in the scenario task data.
inline constexpr std::size_t kScenarioDialogueFlagsOffset = 0xd8;

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

extern std::int32_t g_currentSearchState;

extern std::int32_t g_searchCandidateId;

extern std::int32_t g_scenarioTaskDeltaTime;

extern float g_scenarioTaskSubtitleTime;

extern std::uint32_t g_freeTalkInitialized;

extern std::int16_t g_debug_talk_status;

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

extern std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntWord20;

extern std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntWord22;

extern std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntWord24;

extern std::array<std::uint16_t, kScntScriptSlotCount * kScntWordsPerSlot> g_scntWord26;

extern std::array<std::uint32_t, 9> g_scenSearchCharacters;

extern std::array<std::uint32_t, 12> g_scenOwnedSequenceCharacters;

extern std::int32_t g_scenarioTaskStartTime;

extern std::vector<std::pair<std::uint32_t, const ScenRuntimeRecord *>> g_scenIndexedRecords;

extern ScenOwnedRecordStorage g_scenOwnedRecordStorage;

extern std::int32_t g_scntFrameTimer;

extern std::int32_t g_scnfCanSkip;

extern std::int32_t g_scnfDeltaTime;

void SetScenActiveObjectName(std::uint32_t name);

void SetScenPreviousObjectName(std::uint32_t name);

bool ScenInteractionBlocked();

void RestoreScenPadContext();

void CaptureScenPadContext();

void ClearScenAuthorGameState();

void IndexScenRuntimeContainer(const ScenRuntimeContainer *container);

bool ScenRuntimeRecordHeaderValid(const ScenRuntimeRecord *record);

bool ScenCycleEntryEligible(const CycleManager::NpcRecord &npc, bool runningPrograms, bool &carry);

void UpdateScenTaskData();

void ResetScenRuntimeState();

void FinalizeScenRuntimeObject(ScenRuntimeObject *object);

void SCNF_LogStateChange(const ScenRuntimeObject &object, std::int32_t eventCode);

void SCNF_EnqueueLocationEvent(const ScenRuntimeObject &object, std::uint32_t location);

void SCNF_UpdateObjectActivation(ScenRuntimeObject *object, std::int32_t stateHint);

Character *CurrentScenarioRyo();

std::int32_t SCEN_InteractiveObjects(std::array<float, 24> &distances, std::array<std::int32_t, 24> &stateHints);

const std::byte *ScenScriptEnd(const ScenRuntimeObject &object);

void QueueScenConfirmPrompt();

void ClearScenConfirmPrompt();

bool UpdateScenPendingRuntimeEvent(ScenRuntimeObject &object);

void BeginScenInteraction(ScenRuntimeObject &object);

bool UpdateScenStateFiveRecovery(ScenRuntimeObject &object, const PadCaptureEntry *pad);

bool RunScenBytecode(ScenRuntimeObject &object);

std::byte *ScanScenConditionalFlow(ScenRuntimeObject &object, std::byte *ip);

bool ScenCanRead(const ScenRuntimeObject &object, const std::byte *pointer, std::size_t size);

std::byte *ScenAddRelative(ScenRuntimeObject &object, std::byte *base, std::int32_t delta);

std::int32_t ScenSignExtend(std::uint32_t value, std::uint32_t signBit);

std::size_t ScenWordSlotIndex(const ScenRuntimeObject &object);

std::byte *BuildScenConversationMarkers(ScenRuntimeObject &object, std::byte *ip);

void scnfGenerateLipCues(const ScenRuntimeObject &object);

bool SCNT_IsLineVoicePlaying();

void IncrementInventoryItemCountAndTrackStep(std::uint16_t itemId);

void SCNT_Core(ScenRuntimeObject *object);

std::byte *DecodeScenExpression(std::byte *ip, ScenRuntimeObject &object);

extern std::array<std::uint32_t, 17> g_scenRuntimeObjectTextScratch;

Mt5RuntimeNode *FindScenarioModelNode(Mt5RuntimeNode *node, std::uint16_t kind);

extern std::array<TimedTransitionEntry, 8> g_scenarioTimedTransitions;

extern std::uint8_t g_scenarioTimedTransitionCount;

extern TimedTransitionEntry *g_activeTimedTransition;

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

extern std::uint8_t g_timedTransitionUpcoming;

extern GameDateTime g_timedTransitionUpcomingTime;

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

bool TimedTransitionScenarioBusy();

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1 {

extern std::array<LegacyScenarioFunction, kLegacyScenarioFunctionCount> LegacyFunctions;

extern std::array<GeneralScenarioFunction, kGeneralScenarioFunctionCount> GeneralPurposeFuncs;

extern std::array<ShenmueScenarioFunction, kShenmueScenarioFunctionCount> ShenmueScriptFuncs;

extern std::array<SCNFScenarioFunction, kSCNFScenarioFunctionCount> SCNF_Functions;

extern std::array<DoorScenarioFunction, kDoorScenarioFunctionCount> DSWKFunctions;

void SetStrForScnArg(ScenarioContext *context, std::int64_t pointer, std::uint32_t, ScenarioRegisters *);

void ExecuteRemovedScriptFn(std::int32_t index, ScenarioContext *context, std::int64_t, void *);

std::int64_t ExecuteMemScriptFn(std::int32_t index, void *rawParams);

std::int64_t ExecuteShenmueScriptFn(ScenarioContext *context, std::int32_t index, void *params);

extern std::uint32_t g_tmpScnfReturnValue;

std::int64_t ExecuteSCNFScriptFn(ScenarioContext *context, std::uint32_t index, std::int64_t stackPointer, std::int32_t argumentCount);

std::int64_t ExecuteDoorSupportFunc(ScenarioContext *context, std::uint32_t index, void *params);

ScenarioWindow *ASEQ_SCNW_Init(HLib::Task *scenarioTask);

void scenParseStackRecursively(ScenarioWindow *scenarioWindow);

ScenarioStack *Startup_EventBase(ScenarioWindow *scenarioWindow, ScenarioHeader *info);

} // namespace shenmue::s1
