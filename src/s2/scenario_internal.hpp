#pragma once
#include "s2/event_runtime.hpp"
#include "logging.hpp"

#include "s2/area_event.hpp"
#include "s2/area_jump.hpp"
#include "s2/archive_data.hpp"
#include "s2/audio_data.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/face.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/door.hpp"
#include "s2/darts.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_movement.hpp"
#include "s2/event_voice.hpp"
#include "s2/sap_event.hpp"
#include "s2/sl_event.hpp"
#include "s2/pati_nails.hpp"
#include "s2/areas/area_dart.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/lighting.hpp"
#include "s2/osage.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/handcuffs.hpp"
#include "s2/hand.hpp"
#include "s2/math.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_info.hpp"
#include "s2/maze.hpp"
#include "s2/memo.hpp"
#include "s2/npc_package.hpp"
#include "s2/npc.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/mt7.hpp"
#include "s2/player.hpp"
#include "s2/hlib.hpp"
#include "s2/search.hpp"
#include "s2/scene_data.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include "s2/weather.hpp"
#include "s2/window.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#if defined(_MSC_VER)
#include <intrin.h>
#endif
#include <cstring>
#include <limits>

#include <vector>
#include "s2/scenario_sequence_internal.hpp"
#include "s2/scenario_scnt_internal.hpp"

namespace shenmue::s2 {
namespace event_runtime_detail {}
using namespace event_runtime_detail;

extern HLib::Task *eventWeatherControlTask;

extern std::int16_t g_eventQteRetryFrames;

ScenarioFreeTalkRecord *FindMutableScenarioFreeTalkRecord(shen_name character);

void StopScenarioSequence();

void ResetScenarioEventObjectMapNames();

bool ScenarioCharacterIsRyo(shen_name name);

std::int32_t GetScenarioFollowEligibility(shen_name character);

void ApplyScenarioFaceLook(std::uint32_t command);

shen_name GetActiveScenarioCharacter();

extern std::int32_t scenarioTalkPromptResumed;

extern std::int32_t scenarioFollowPromptShown;

void scenUpdateFreeTalkRuntime(ScenarioEventRuntime *scenario);

Character *GetScenarioCharacterNode(HLib::Task *scenarioTask, shen_name characterId);

Character *FindScenarioCharacter(HLib::Task *scenarioTask, shen_name characterId);

shen_name scenGetCharaName(HLib::Task *scenarioTask, Character *character);

bool AssignScenarioCharacter(HLib::Task *scenarioTask, shen_name characterId, Character *character);

bool UnassignScenarioCharacter(HLib::Task *scenarioTask, Character *character);

std::int64_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);

std::uint8_t ClearOrSetShenmueFlags(std::int32_t flag, std::int32_t enabled);

std::int64_t SetScenarioByteStat(std::int32_t index, std::int32_t value);

void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int16_t flag, std::int32_t value);

void EV_GetCharaPosition(shen_name name, std::uint32_t bone, Vector3 *position, std::uint32_t mode);

void GetScenarioCharacterEulerAngles(shen_name name, Vector3i *rotation, std::uint32_t flags);

bool DoesScenarioCharacterExist(shen_name name);

void EV_AT_SetPos(shen_name character, std::uint32_t flags, const Vector3 &requested);

std::int64_t EV_AT_SetAngY(shen_name character, std::uint32_t flags, const Vector3i &requested, const std::uint16_t *customMotions);

void EV_EnsureCharacterDraw(shen_name name);

void EV_EnableCharacterDisplay(shen_name name);

void EV_ToggleCharacterFlag(shen_name name);

void EV_DisableCharacterDisplay(shen_name name);

void EV_SetCharacterAlphaEnabled(shen_name name, bool enabled);

namespace event_runtime_detail {

HLib::Task *CurrentScreenFilmTask();

extern ScenarioPersistentFlagBanks scenarioPersistentFlags;

extern std::array<std::uint8_t, 8> &storyFlags;

extern std::array<std::uint8_t, 512> &internalFlags;

extern std::array<std::uint8_t, 64> &stats;

extern std::array<std::byte, 0x80> &scenarioPersistentScratch;

extern EventAuvwRuntime eventAuvwRuntime;

extern std::int32_t butterflyRegionIndex;

extern std::int32_t butterflySlotIndex;


inline constexpr shen_name eventTaskToken{SHEN_NAME(EVNT)};
inline constexpr shen_name ryoName{SHEN_NAME(RYO_)};
extern EventDartsState eventDartsState;

extern shen_name eventDartsTemporaryCharacter;

extern EventQtePresentationState eventQtePresentation;

extern EventQtePresentationSink eventQtePresentationSink;

extern std::uint32_t eventQteFailureCount;

void PublishEventQteState();

ScenarioTaskData *ScenarioData(HLib::Task *task);

extern ScenarioAuthorContext scenarioAuthorContext;

extern ScenarioEventRuntime scenarioEventRuntimeStorage;

extern ScenarioEventRuntime *activeScenarioEventRuntime;

extern std::int32_t scenarioFreeStateState;

inline constexpr shen_name talkArea{SHEN_NAME(TALK)};
inline constexpr shen_name hiddenFreeTalkCharacterName{make_shen_name('$', 'f', 'r', 'e')};
inline constexpr shen_name stepVisibleCharacterName{SHEN_NAME(LETR)};
extern ScenarioFreeTalkStorage scenarioFreeTalkStorage;

extern std::array<shen_name, 32> scenarioCharacterAdmissionNames;

extern std::array<ScenarioFreeTalkCandidate, 64> scenarioCandidatePool;

extern ScenarioFreeTalkCandidate *scenarioCandidateFreeList;

extern ScenarioFreeTalkCandidate *scenarioCandidateList;

extern std::int32_t scenarioCandidateMeshIndex;

extern std::int32_t scenarioCandidateTaggedMesh;

extern std::array<shen_name, 6> scenarioEventObjectMapNames;

extern std::array<SCENDialogueCue, 101> scenarioDialogueCues;

extern std::array<CameraDialogueRecord, 24> scenarioCameraRecords;

extern std::array<std::uint16_t, 4> scenarioChoiceIds;

extern std::uint16_t scenarioChoiceIdSpare;

extern std::array<std::int8_t, 5> scenarioChoiceRanks;

extern std::array<std::uint8_t, 5> scenarioChoiceSources;

extern std::array<const std::byte *, 6> scenarioChoiceTargets;

extern std::array<ScenarioChoiceCandidate, 100> scenarioChoiceCandidates;

extern std::array<const std::byte *, 100> scenarioBranchTargets;

extern std::int32_t scenarioBranchCount;

extern std::int32_t scenarioBranchLoop;

extern std::int32_t scenarioPromptFrames;

extern std::int32_t scenarioLineHoldFrames;

extern std::int32_t scenarioLineSkipped;

extern float scenarioLineTimeOffset;

extern std::int32_t scenarioLinePauseTicks;

extern std::int32_t scenarioLineStartTicks;

extern std::int32_t scenarioVoiceWaitState;

extern std::int32_t scenarioVoiceWaitArmed;

extern ConversationBuffer *scenarioCurrentDialogue;

extern std::int32_t scenarioHardBusy;

extern std::int32_t scenarioFlagNotificationPending;

extern std::int32_t scenarioFlagNotificationFrames;

extern ConversationBuffer *scenarioFlagNotificationJingle;

extern std::int32_t scenarioFlagNotificationShown;

Character *ScenarioRyo();

float ScenarioLineClock();

Character *ResolveScenarioTalkCharacter(shen_name name);

extern ScenarioSequenceRuntime scenarioSequenceRuntime;

extern bool scenarioSequenceFightActive;

void RebuildScenarioEventObjectMapNames();

bool AssignScenarioExpressionOperand(const ScenarioExpressionOperand &operand, std::int32_t value);

extern std::int32_t scntExpressionResult;

const std::byte *FindScenarioConversationRoot(ScenarioFreeTalkRecord &record, std::int32_t step);


// The reset payload is ten scalar control bytes, four eight-word groups,
// eight -1 record selections, three eight-byte groups, one sixteen-byte
// group, four talk flags, three zero counters and a -1 current selection.
inline constexpr Vector3 scenarioTalkDefaultLocation{};
void SolveRyoMatrix(shen_name origin, shen_name other, std::uint16_t yaw, Vector3 *result);



void ReleaseScenarioStepCharacter(ScenarioEventRuntime &scenario);

void ReleaseHiddenFreeTalkCharacter(ScenarioEventRuntime &scenario);

void ReleaseFreeTalk();

void ReleaseScenarioEventRuntime(ScenarioTaskData &scenario);

void FreeScenarioControl(ScenarioTaskData &scenario);

extern void *scenarioWindowRecord;

extern void **g_pSCNW;

void *AllocateSCNW(HLib::Task *task);

void SCNF_Handler(WindowSlot *window, WindowCallbackEvent event, void *payload);

void WriteBit(std::uint8_t *bytes, std::size_t byteCount, std::int32_t index, bool value);

void EndScenarioPlayerPresentation(std::int32_t mode);

void ConsumeScenarioCameraRestoreRequest();

void RetireScenarioFreeTalkRecord(ScenarioFreeTalkRecord *record);

bool ryoCheckBusyFlags();

void scenClearSpeakerPresentation();

void ReleaseScenarioTalkInput();

bool IsScenarioActiveRecordIdle();

const shen_name *ScenarioParticipants(const ScenarioFreeTalkRecord &record);

shen_name ResolveScenarioParticipant(shen_name participant, const ScenarioFreeTalkRecord &record);

extern std::int32_t scenarioTalkHandPoseCommand[19][3];




void scenEndConversation();

void scenExecuteRecord(ScenarioFreeTalkRecord *recordPointer);

} // namespace event_runtime_detail
} // namespace shenmue::s2
