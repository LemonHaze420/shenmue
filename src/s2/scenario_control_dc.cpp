#include "s2/scenario_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstring>
#include "s2/frame_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/conversation_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/date_time.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/scenario_actions_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/math.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#include "common/native_sqrt.hpp"
#include "platform/dc/matrix.hpp"
#include <cmath>
#endif
#include "s2/scenario_talk_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/scenario_scnt.hpp"
#include "shenmue.h"
#include "s2/player_stats_dc.hpp"
#include "s2/game_system_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/camera_conversation_dc.hpp"
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hand_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/sound_program.hpp"
#endif
#include "s2/calendar_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_region_data_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/party_companion.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/maze_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/field_event_dc.hpp"
#include <cfloat>
#include "s2/conversation_fade_dc.hpp"
#include <cri_adxt.h>
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include "s2/particle_effects.hpp"
#include "s2/ship_spray.hpp"
#include "s2/hiki.hpp"
#include "s2/event_character_state_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/flag_data.hpp"
#include "s2/inventory_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/flag_context_dc.hpp"
#endif
#include "s2/fight_blur_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/hlib.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/texture_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/save_screen_state_dc.hpp"
#include <cstdarg>
#include <cstdio>
#endif
#include "s2/cold_archive_dc.hpp"
#include "s2/scene_data.hpp"

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct ScenarioChoiceCandidate {
    std::int16_t id;
    std::int8_t rank, priority;
    const std::byte *target;
};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct ScenarioCharacterAction {
    shen_name name;
    std::int32_t action;
};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t GetParentMenu();

std::uint8_t UpdateSelectorMemo();

ConversationBuffer *PlaySpeakerDialogue(const char *code, shen_name speaker);

std::int32_t RequestScenarioNpcSceneRoute(shen_name name, shen_name area);

bool QueryNpcAreaSceneCheck(shen_name name, shen_name area);

bool npcExists(shen_name name);

std::uint32_t SetPlayerSpecialMode(std::int32_t mode);
#endif

const std::byte *FindScenarioConversationRoot(ScenarioFreeTalkCharacter *record, std::int32_t step);

std::int32_t ScenarioCharacterIsRyo(shen_name name);

void FreeTalkCharacterUpdateNullsub(Character *, void *);

void FreeTalkCharacterCleanup(Character *character, void *payload);

ScenarioFreeTalkCharacter *FindScenarioFreeTalkCharacter(shen_name character);

shen_name *GetScenarioFreeTalkParticipants(shen_name name);

void ReadFreeTalk();

void scenUpdateFreeTalkInteraction();

FreeTalkRegistration *srchEvaluateSearchTarget();

void scenActivateCandidate(FreeTalkRegistration *candidate);

static std::int32_t EvaluateScenarioSearchGeometry(FreeTalkRegistration *candidate, Vector3 *position, float *distance, std::int16_t *yaw);

void InterruptScenarioFreeTalkRecord(ScenarioFreeTalkCharacter *record);

bool IsScenarioActiveRecordIdle();

void FlushScenarioActionQueue();

std::int32_t AllocateScenarioActionQueue(shen_name name);

std::int32_t WriteScenarioActionQueue(std::uint32_t index, const void *input, std::uint32_t size);

bool CanActivateScenarioCandidate();

std::int32_t IsScenarioBusy();

void scenUpdateCandidateAvailability();

void ExpandFreeTalkRecord(const std::byte *source, ScenarioFreeTalkCharacter *record);

std::int32_t VisitScntBranch(const std::byte *target);

void ResetScntBranchHistory();

void scenExecuteRecord(ScenarioFreeTalkCharacter *record);

const std::byte *scenExecuteChoiceOpcode(ScenarioFreeTalkCharacter *record, const std::byte *cursor, std::int32_t *stop);

static void scenStepRecordProgram(ScenarioFreeTalkCharacter *record);

void CaptureScenarioTalkInput();

void ReleaseScenarioTalkInput();

void ConsumeScenarioCameraRestoreRequest();

void RequestScenarioCameraRestore();

void BeginScenarioPlayerPresentation();

void EndScenarioPlayerPresentation(std::int32_t mode);

void ApplyScenarioFaceLook(std::uint32_t command);

void UpdateGameplayFlags();

void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int32_t flag, std::int32_t value);

const std::byte *DecodeScntExpression(const std::byte *cursor, ScenarioFreeTalkCharacter *record);

void ResetScenarioInteractionState();

void scenProcessCharacterActionQueue();

static void FaceScenarioParticipantsForward();

static void QueueScenarioParticipantTurn(std::int32_t action);

static void TurnScenarioParticipantToward(std::int32_t slot);

static void ReleaseScenarioParticipantLook(shen_name name);

static void LookAtScenarioRecordLocation(std::int32_t action, shen_name name);

static void SetScenarioPlayerLookTarget(const Vector3 *position, std::int32_t mode);

static void RequestScenarioLookRelease(shen_name name);

static void EndScenarioLocationLook(std::int32_t action, shen_name name);

static void SetScenarioLetterMotion(shen_name name, std::int8_t show);

static void AcknowledgeScenarioPlayer(shen_name name);

static void AcknowledgeScenarioLookRelease(shen_name name);

static void HideScenarioLetter();

static void RetireScenarioTalkParticipants(shen_name name);

static void SelectScenarioActionParticipants(shen_name name);

static void TurnScenarioPlayerTowardSpeaker();

static void CaptureScenarioSpeakerPose();

void SolveRyoMatrix(shen_name origin, shen_name other, std::int32_t yaw, Vector3 *result);

void SpawnMoveScroll();

void SetModePerCurrentStep(std::int32_t mode);

void RebuildScenarioEventObjectMapNames();

void InitializeFrameVars();

void GameSys_Callback(void *);

bool GetInternalFlag(std::int32_t flag);

bool GetStoryFlag(std::int32_t flag);

std::uint8_t GetScenarioByteStat(std::int32_t index);

void InitializeFrameVars();

void GameSys_Callback(void *);

bool GetInternalFlag(std::int32_t flag);

bool GetStoryFlag(std::int32_t flag);

std::uint8_t GetScenarioByteStat(std::int32_t index);

void InitializeFrameVars();

void GameSys_Callback(void *);

bool GetInternalFlag(std::int32_t flag);

bool GetStoryFlag(std::int32_t flag);

std::uint8_t GetScenarioByteStat(std::int32_t index);

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetShenmueFlag(std::int32_t flag);
#endif

void InitializeFrameVars();

void GameSys_Callback(void *);

bool GetInternalFlag(std::int32_t flag);

bool GetStoryFlag(std::int32_t flag);

std::uint8_t GetScenarioByteStat(std::int32_t index);

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetShenmueFlag(std::int32_t flag);
#endif

void RecordScenarioTalkGroup(std::int32_t index);

void RecordRecentScenarioTalk(std::int32_t index);

std::int32_t GetPlayerStats(std::int32_t index);

void InitializeFrameVars();

void GameSys_Callback(void *);

bool GetInternalFlag(std::int32_t flag);

bool GetStoryFlag(std::int32_t flag);

std::uint8_t GetScenarioByteStat(std::int32_t index);

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetShenmueFlag(std::int32_t flag);
#endif

void RecordScenarioTalkGroup(std::int32_t index);

void RecordRecentScenarioTalk(std::int32_t index);

std::int32_t GetPlayerStats(std::int32_t index);

void InitializeFrameVars();

std::int32_t GetShenmue1SaveAreaId(shen_name areaId);

void GameSys_Callback(void *);

Character *ctrlGetScenarioChara(shen_name name);

bool GetInternalFlag(std::int32_t flag);

bool GetStoryFlag(std::int32_t flag);

std::uint8_t GetScenarioByteStat(std::int32_t index);

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetShenmueFlag(std::int32_t flag);
#endif

void RecordScenarioTalkGroup(std::int32_t index);

void RecordRecentScenarioTalk(std::int32_t index);

std::int32_t GetPlayerStats(std::int32_t index);

void InitializeFrameVars();

std::int32_t GetShenmue1SaveAreaId(shen_name areaId);

void GameSys_Callback(void *);

extern void *g_scenarioFlagNotificationJingle;

Vector3 *plymUpdatePlayerSearchLocation(Vector3 *position);

FreeTalkRegistration *g_freeTalkRegistrations;

FreeTalkRegistration *g_freeTalkFreeRegistrations;

shen_name g_freeTalkVisibleCharacters[8];

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_scenarioQueuedFaceLooks[8];
#endif

std::int32_t g_freeTalkVisibleCharacterCount;

ScenarioFreeTalkCharacter g_residentFreeTalkCharacters[16];

FreeTalkCharacterState g_savedFreeTalkStates[512];

ScenarioEventContextPrefix *g_scenarioEventContext;

std::int32_t g_scenarioSubtitleHoldFrames;

ScenarioActionQueue *scenarioActionQueueStorage;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_scenarioGameplayFlags[10];
#else
ScenarioGameplayFlagState g_scenarioGameplayFlags;
#endif

void *g_scenarioFlagNotificationJingle;

std::int32_t g_scenarioFlagNotificationPending, g_scenarioFlagNotificationFrames = 0;

std::int32_t g_scenarioFlagNotificationShown;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t scenarioGameplayFlagsInitialized = 0, scenarioGameplayFlagsDay = 0, scenarioGameplayFlagsStep = 0;
#endif

shen_name g_mapRegionNames[6];

std::int32_t g_scntExpressionResult;

static const std::byte *scntBranchTargets[100];

static std::int32_t scntBranchCount = 0;

static std::int32_t scntBranchRepeated = 0;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float scenarioLineLetterFrames = 4.0f, scenarioLineBaseFrames = 15.0f;
#endif

std::int32_t g_currentShenmue1SaveAreaId = 0;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static ScenarioEventContextStorage g_scenarioEventStorage;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioFreeTalkPhase;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name g_scenarioCharacterAdmissionNames[kScenarioAdmissionCount];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 g_scenarioAdmissionPositions[kScenarioAdmissionCount];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioSelectionCount;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_scenarioLineStartTicks;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
ScenarioDialogueCue g_scenarioDialogueCues[kScenarioDialogueCueCount];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioActionQueueId = 0, g_scenarioAcknowledgementQueueId = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
ConversationBuffer *g_scenarioCurrentDialogue;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_scenarioRetainedState94c = 0, g_scenarioRetainedState950 = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_scenarioLookTargetActive = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioRetainedLookTarget;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioTalkAreaActive;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_scenarioFaceForwardMode, g_scenarioFacingCommandsIssued, g_scenarioPlayerFacingIssued, g_scenarioNpcFacingIssued,
    g_scenarioSuppressPlayerFacing, g_scenarioSuppressNpcFacing;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioTalkPromptResumed = 0, g_scenarioFollowPromptShown = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const std::byte *g_externalFreeTalkRecords = 0, *g_extraFreeTalkRecord = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AllocateFIXO(Character *character);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
PadControlEntry *BindPadControlEntry(PadControlEntry *entry);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EndConversationCameraStream();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 scenarioSearchPlayerPosition, scenarioSearchFocusPosition, scenarioSearchLockonPosition;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t scenarioSearchPlayerYaw, scenarioSearchHasLockonPosition;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MT7Node *g_scenarioStepModels[7];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioSelectedFollowFlag;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RestoreConversationCamera();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
ConversationDialogueRecord g_scenarioCameraRecords[24];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint16_t scenarioChoiceIds[5];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int8_t scenarioChoiceRanks[5], scenarioChoiceGroups[5];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::byte *scenarioChoiceTargets[6];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static ScenarioChoiceCandidate scenarioLinkedChoices[100];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t scenarioPromptFrames, scenarioLineHoldFrames;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float scenarioLineTimeOffset;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t scenarioLinePauseTicks;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern std::int32_t g_scenarioTalkPromptResumed, g_scenarioFollowPromptShown;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EndConversationCameraStream();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ActivateConversationCameraStream(const ConversationDialogueRecord *records);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlHoldConversationShot(std::int32_t held);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResumeConversationCamera();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t scenarioInitialHandPose[57] = {0x7cfU, 0xff4aU, 0xed86U, 0x666U, 0xb4U, 0xf6c3U, 0x222U, 0xfccfU, 0xd26U, 0x0U, 0xfd2aU, 0xd1caU, 0x0U,
    0xfd2aU, 0xc0b8U, 0x0U, 0x0U, 0xeaacU, 0x0U, 0x0U, 0xfd2aU, 0x0U, 0x0U, 0xdde0U, 0x0U, 0x0U, 0xb99cU, 0x0U, 0xfe94U, 0xdd2bU, 0x0U, 0x0U, 0xff4aU, 0x0U,
    0x2d9U, 0xd99bU, 0x0U, 0xfe95U, 0xc0baU, 0x0U, 0xfddeU, 0xdbc0U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0xdc72U, 0x0U, 0x667U, 0xc2daU, 0x0U, 0xb7U, 0xe7d6U,
    0xfa50U, 0xff4aU, 0xfbbcU};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlSetConversationLookTarget(const Vector3 *position);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t scenarioHardBusy;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static FreeTalkRegistration g_freeTalkRegistrationPool[kFreeTalkRegistrationCount];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t scenarioSuspendedLineTicks = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t scenarioMeshCount, scenarioTaggedMesh;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name g_scenarioActionParticipants[8], g_scenarioRetainedParticipants[10];
const Vector3 *g_scenarioLookLocation;
Vector3 g_scenarioSpeakerPosition, g_scenarioPlayerPosition;
std::int32_t g_scenarioParticipantMotionIds[8], g_scenarioParticipantWorldYaws[8], g_scenarioParticipantEntityYaws[8];
std::uint8_t g_scenarioActionSystemRecord, g_scenarioRyoAcknowledged, g_scenarioRetainedTalkByte, g_scenarioLookActive, g_scenarioTurnPending[10];
std::uint8_t g_scenarioLookAcknowledged, g_scenarioLookReleasePending[8], g_scenarioLookReleaseRequested, g_scenarioLookLocationPending;
std::uint8_t g_scenarioParticipantTurnIssued[10], g_scenarioPlayerTurnPending, g_scenarioLetterShowPending, g_scenarioLetterHidePending;
std::uint8_t g_scenarioLetterVisible;
std::int8_t g_scenarioParticipantFacing[8];
std::uint8_t g_scenarioFaceForwardPending;
shen_name g_scenarioTurnSource, g_scenarioTurnTarget, g_scenarioLookReleaseNames[8], g_scenarioLookReleaseCharacter, g_scenarioPlayerTurnTarget;
shen_name g_scenarioTurnTargets[10], g_scenarioTurnSources[10], g_scenarioActionCharacter, g_scenarioPreviousPlayerTurnTarget;
std::uint8_t g_scenarioParticipantFlags[8], g_scenarioSpeakerPoseCaptured;
std::int32_t g_scenarioLastTurnAction;
float g_scenarioSpeakerHeadHeight, g_scenarioSpeakerChestHeight;
std::uint8_t g_scenarioLookTargetReleased;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int8_t g_scenarioLookCameraState = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_scenarioParticipantFaceLooks[8] = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3 g_scenarioDefaultLookLocation = {0, 0, 0};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t scenarioPlayerTurnAround;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int8_t scenarioLookReleaseFrames, scenarioLookLocationFrames = 0, scenarioLetterFrames = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int8_t scenarioPlayerTurnFrames = 0, scenarioPlayerTurnFrameLimit = -1;
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t supportIptCurrent;
#endif

} // namespace

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t supportMetricsPage = 0;
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_scenarioTransientState[5];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t g_scenarioTalkOccurrences[512] = {};
#endif

std::byte *g_systemTalkData = 0;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t gameSysActivityFlags, supportIptMaximum, supportIptMinimum;
#else
std::uint32_t gameSysActivityFlags, gameSysUnusedColour = 0;
#endif

std::int32_t gameSysActivityCounters[5];

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t currentDebugMenuPage, inputSuppressionFrames;
#else
std::int32_t gameSysUnusedState = 0, currentDebugMenuPage = 0, inputSuppressionFrames = 0;
#endif

// @verified - d
void InitializeScenarioActionQueues(void *storage) {
    scenarioActionQueueStorage = static_cast<ScenarioActionQueue *>(storage);
    for (std::int32_t index = 0; index < 2; ++index)
        static_cast<ScenarioActionQueue *>(storage)[index].name = 0;
}

// @verified - d
bool scenIsInArea(shen_name name) {
    if (name == g_currentMap || name == drGetCurrentAuthorId())
        return true;
    for (std::int32_t region = 0; region < 6; ++region)
        if (name == g_mapRegionNames[region])
            return true;
    return false;
}

// @verified - d
bool ResolveCurrentMapOperand(std::uint32_t index) {
    return index >= sizeof(scenarioMapOperandNames) / sizeof(scenarioMapOperandNames[0]) ? false : scenIsInArea(scenarioMapOperandNames[index]);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name GetMapOperandName(std::uint32_t index) {
    if (index < sizeof(scenarioMapOperandNames) / sizeof(scenarioMapOperandNames[0]))
        return scenarioMapOperandNames[index];
    return SHEN_NAME4('?', '?', '?', '?');
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ResetScenarioGlobals() {
    g_scenarioFlagNotificationPending = 0;
    g_scenarioFlagNotificationShown = 0;
    g_scenarioEventContext = 0;
    g_scenarioFreeTalkPhase = 0;
    std::memset(g_savedFreeTalkStates, 0, sizeof(g_savedFreeTalkStates));
    g_scenarioRetainedState94c = g_scenarioRetainedState950 = 0;
    g_scenarioSubtitleHoldFrames = 0;
    g_scenarioLookTargetActive = 0;
    g_scenarioRetainedLookTarget = -1;
}
#endif

// @verified - d
std::byte *LoadSystemTalk() {
    std::byte *const data = static_cast<std::byte *>(shenLoadFile("misc", "systalk.bin", SYSTEM_TALK_STORAGE));
    g_systemTalkData = data;
    return data;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void scenUpdateFreeTalkRuntime(ScenarioEventContextPrefix *context) {
    if (!context || g_scenarioEventContext != context)
        return;
    g_scenarioFollowPromptShown = 0;
    scenarioHardBusy = 0;
    switch (g_scenarioFreeTalkPhase) {
    case 0:
        ResetScenarioInteractionState();
        ReadFreeTalk();
        SpawnMoveScroll();
        g_scenarioFreeTalkPhase = 1;
        break;
    case 1:
        reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->pendingCharacter = shen_name();
        RebuildScenarioEventObjectMapNames();
        scenUpdateFreeTalkInteraction();
        scenProcessCharacterActionQueue();
        UpdateGameplayFlags();
        if (reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->pendingCharacter) {
            if ((g_scenarioEventContext->flags >> 16) & 1U) {
                scenDestroySubtitles(g_scenarioEventContext->scenarioTask);
                g_scenarioEventContext->flags &= 0xfffeffffU;
            }
            EndScenarioPlayerPresentation(0);
            ConsumeScenarioCameraRestoreRequest();
            ReleaseScenarioTalkInput();
        }
        break;
    }
    if (g_scenarioSubtitleHoldFrames > 0) {
        if (g_scenarioSubtitleHoldFrames > 5 && (g_inputBuffer[0].pressedPriority & 0x200U))
            g_scenarioSubtitleHoldFrames = 5;
        if (--g_scenarioSubtitleHoldFrames < 1)
            scenDestroySubtitles(g_scenarioEventContext->scenarioTask);
    }
    Vector3 center = {};
    if (g_scenarioTalkAreaActive)
        g_scenarioTalkAreaActive = npcSetAreaName(SCENARIO_TALK_AREA, &center, 0.0f, 0);
    if ((g_scenarioEventContext->flags >> 15) & 1U) {
        plymUpdatePlayerSearchLocation(&center);
        g_scenarioTalkAreaActive = npcSetAreaName(SCENARIO_TALK_AREA, &center, 0.5f, 1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
ScenarioEventContextPrefix *InitializeScenarioEventContext(HLib::Task *task) {
    std::int32_t index;
    g_scenarioEventContext = &g_scenarioEventStorage.prefix;
    std::memset(&g_scenarioEventStorage, 0, sizeof(g_scenarioEventStorage));
    g_scenarioEventContext->scenarioTask = task;
    g_scenarioEventContext->freeTalkCharacter = 0;
    g_freeTalkFreeRegistrations = g_freeTalkRegistrationPool;
    g_freeTalkRegistrations = 0;
    std::memset(g_freeTalkRegistrationPool, 0, sizeof(g_freeTalkRegistrationPool));
    for (index = 0; index < static_cast<std::int32_t>(kFreeTalkRegistrationCount) - 1; ++index)
        g_freeTalkRegistrationPool[index].next = &g_freeTalkRegistrationPool[index + 1];
    g_scenarioFlagNotificationJingle = 0;
    g_scenarioFreeTalkPhase = 0;
    for (index = 0; index < static_cast<std::int32_t>(kScenarioAdmissionCount); ++index)
        g_scenarioCharacterAdmissionNames[index] = shen_name();
    g_freeTalkVisibleCharacters[0] = shen_name();
    g_freeTalkVisibleCharacters[1] = shen_name();
    g_freeTalkVisibleCharacters[2] = shen_name();
    g_freeTalkVisibleCharacters[3] = shen_name();
    g_freeTalkVisibleCharacters[4] = shen_name();
    g_freeTalkVisibleCharacters[5] = shen_name();
    g_freeTalkVisibleCharacters[6] = shen_name();
    g_freeTalkVisibleCharacters[7] = shen_name();
    g_freeTalkVisibleCharacterCount = g_scenarioSelectionCount = 0;
    for (index = 0; index < static_cast<std::int32_t>(kScenarioDialogueCueCount); ++index) {
        g_scenarioDialogueCues[index].triggerTime =
            static_cast<float>(g_scenarioLineStartTicks - *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister)) / 12500000.0f;
        g_scenarioDialogueCues[index].payload = 0;
        g_scenarioDialogueCues[index].speaker = shen_name();
        g_scenarioDialogueCues[index].retained0c = 0;
    }
    g_scenarioActionQueueId = AllocateScenarioActionQueue(shen_name(SHEN_NAME4('S', 'C', 'N', 'A')));
    g_scenarioAcknowledgementQueueId = AllocateScenarioActionQueue(shen_name(SHEN_NAME4('S', 'C', 'N', 'F')));
    g_scenarioCurrentDialogue = 0;
    g_scenarioRetainedState94c = g_scenarioRetainedState950 = 0;
    g_scenarioSubtitleHoldFrames = 0;
    g_scenarioLookTargetActive = 0;
    g_scenarioRetainedLookTarget = -1;
    g_scenarioTalkAreaActive = 0;
    g_scenarioTalkPromptResumed = 0;
    return g_scenarioEventContext;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static shen_name ResolveScenarioParticipant(shen_name name, const ScenarioFreeTalkCharacter *record) {
    if (name == shen_name(SHEN_NAME4('E', 'X', 'T', 'R')))
        name = record->name;
    if (!record->savedStateIndex && !ScenarioCharacterIsRyo(name))
        name = record->name;
    return name;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void CollectScenarioCandidateMesh(MT7Node *node) {
    if (node->mesh) {
        if (node->id == -67)
            scenarioTaggedMesh = scenarioMeshCount;
        ++scenarioMeshCount;
    }
    if (node->child)
        CollectScenarioCandidateMesh(node->child);
    if (node->sibling)
        CollectScenarioCandidateMesh(node->sibling);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t CollectScenarioCandidateMeshIndex(Character *character) {
    MT7Node *const model = character->model;
    scenarioTaggedMesh = scenarioMeshCount = 0;
    if (model)
        CollectScenarioCandidateMesh(model);
    return scenarioTaggedMesh;
}
#endif

// @verified - d
void AllocateScenarioFreeTalkCharacter(Character *character) {
    ScenarioFreeTalkCharacter *const component = static_cast<ScenarioFreeTalkCharacter *>(
        AllocateModelComponent(character, CHARACTER_FREE_TALK, sizeof(ScenarioFreeTalkCharacter), FreeTalkCharacterUpdateNullsub));
    if (component) {
        SetModelComponentCleanup(character, CHARACTER_FREE_TALK, FreeTalkCharacterCleanup);
        std::memset(static_cast<void *>(component), 0, sizeof(*component));
        component->state = &component->localState.saved;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool AttachScenarioFreeTalkCharacter(Character *character, const std::byte *source) {
    ScenarioFreeTalkCharacter *record = 0;
    bool result = false;
    shen_name name = shen_name();
    const std::byte *selected, *external, *maximumRoot;
    FreeTalkRegistration *registration, *candidate;
    std::int32_t step;
    if (character)
        name = scenGetCharaName(g_scenarioEventContext->scenarioTask, character);
    if (name) {
        record = static_cast<ScenarioFreeTalkCharacter *>(charFindComp(character, CHARACTER_FREE_TALK));
        if (record) {
            if (source) {
                if (SHEN_NAME(SCNF) == *reinterpret_cast<const std::uint32_t *>(source))
                    selected = source + 0x10;
                else
                    selected = 0;
                if (g_externalFreeTalkRecords) {
                    for (external = g_externalFreeTalkRecords; *reinterpret_cast<const std::uint32_t *>(external);
                        external += reinterpret_cast<const std::uint32_t *>(external)[1])
                        if (name == *reinterpret_cast<const std::uint32_t *>(external)) {
                            selected = external;
                            break;
                        }
                }
                if (selected) {
                    ExpandFreeTalkRecord(selected, record);
                    if (*reinterpret_cast<std::uint16_t *>(&record->savedStateIndex)) {
                        for (candidate = g_freeTalkRegistrations; candidate; candidate = candidate->next)
                            if (static_cast<std::uint16_t>(candidate->savedStateIndex) == static_cast<std::uint16_t>(record->savedStateIndex))
                                break;
                    } else {
                        record->name = name;
                        record->state = &record->localState.saved;
                    }
                    if (record->name == name)
                        result = true;
                }
            } else if (g_extraFreeTalkRecord) {
                ExpandFreeTalkRecord(g_extraFreeTalkRecord, record);
                record->name = name;
                record->savedStateIndex = 0;
                record->state = &record->localState.saved;
                result = true;
            }
        }
    }
    if (result) {
        registration = g_freeTalkFreeRegistrations;
        if (registration) {
            g_freeTalkFreeRegistrations = registration->next;
            registration->name = record->name;
            registration->savedStateIndex = record->savedStateIndex;
            registration->availability = 0;
            registration->searchState = 0;
            registration->distance = 0.0f;
            registration->meshIndex = CollectScenarioCandidateMeshIndex(character);
            registration->character = character;
            registration->motionWork = charFindMotionwork(character);
            registration->component = record;
            registration->previous = 0;
            registration->next = g_freeTalkRegistrations;
            if (g_freeTalkRegistrations)
                g_freeTalkRegistrations->previous = registration;
            record->localState.registration = g_freeTalkRegistrations = registration;
        } else
            result = false;
    }
    if (result) {
        step = 0;
        record->localState.alternateRoot = 0;
        record->localState.defaultRoot = 0;
        maximumRoot = FindScenarioConversationRoot(record, 999999);
        record->localState.defaultRoot = FindScenarioConversationRoot(record, 0);
        if (record->localState.defaultRoot == maximumRoot)
            record->localState.defaultRoot = 0;
        if (!record->localState.defaultRoot) {
            step = 101000;
            record->localState.defaultRoot = FindScenarioConversationRoot(record, step);
            if (record->localState.defaultRoot == maximumRoot)
                record->localState.defaultRoot = 0;
        }
        if (record->localState.defaultRoot)
            record->localState.alternateRoot = FindScenarioConversationRoot(record, step);
        record->localState.requestedRoot = 0;
        record->localState.retainedRootState = 0;
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void PauseScenarioLineClock(std::int32_t paused) {
    PauseConversationCameraClock(paused);
    if (paused)
        scenarioSuspendedLineTicks = *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister) - g_scenarioLineStartTicks;
    else
        g_scenarioLineStartTicks = *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister) - scenarioSuspendedLineTicks;
}
#endif

// @verified - d
const std::byte *FindScenarioConversationRoot(ScenarioFreeTalkCharacter *record, std::int32_t step) {
    const std::byte *cursor, *returnCursor, *result;
    std::int32_t hasDefault, operand, low;
    std::uint8_t opcode;
    std::int32_t previousStep = GetCurrentStep();
    SetSTEP(step);
    ResetScntBranchHistory();
    cursor = record->localState.defaultRoot;
    if (!cursor)
        cursor = record->script;
    hasDefault = record->localState.defaultRoot != 0;
    returnCursor = 0;
    result = 0;
    while (cursor) {
        opcode = *cursor++;
        if (opcode < 0x80) {
            operand = ((opcode & 15) << 8) + *cursor++;
            switch (opcode & 0xf0) {
            case 0x00:
                break;
            case 0x10:
            case 0x20:
                result = cursor;
                break;
            case 0x30:
                break;
            case 0x40:
                if (!g_scntExpressionResult) {
                    if (operand & 0x800)
                        operand |= 0xfffff000;
                    cursor += operand;
                }
                break;
            case 0x50:
                if (operand & 0x800)
                    operand |= 0xfffff000;
                cursor += operand;
                if (VisitScntBranch(cursor)) {
                    result = 0;
                    cursor = 0;
                }
                break;
            case 0x60:
            case 0x70:
                break;
            }
        } else if (opcode < 0xe0) {
            low = opcode & 15;
            switch (opcode & 0xf0) {
            case 0x80:
                operand = *cursor++ << 8;
                operand += *cursor++;
                operand += low << 16;
                returnCursor = cursor;
                if (operand & 0x80000)
                    operand |= 0xfff00000;
                cursor += operand;
                break;
            case 0x90:
                result = cursor - 1;
                ++cursor;
                if (!hasDefault)
                    cursor = 0;
                break;
            case 0xa0:
                cursor += 2;
                break;
            case 0xb0:
                switch (low) {
                case 0:
                    cursor += 4;
                    break;
                case 1:
                    ++cursor;
                    break;
                }
                break;
            case 0xc0:
                ++cursor;
                break;
            case 0xd0:
                operand = *cursor++ << 8;
                operand += *cursor++;
                operand += low << 16;
                if (operand & 0x80000)
                    operand |= 0xfff00000;
                cursor += operand;
                if (VisitScntBranch(cursor)) {
                    result = 0;
                    cursor = 0;
                }
                break;
            }
        } else {
            switch (opcode) {
            case 0xe1:
            case 0xe2:
            case 0xe5:
                if (hasDefault)
                    result = cursor - 1;
                cursor = 0;
                break;
            case 0xe6:
                if (!hasDefault)
                    cursor = 0;
                break;
            case 0xf2:
                cursor += 3;
                if (!hasDefault)
                    cursor = 0;
                break;
            case 0xf0:
                cursor = DecodeScntExpression(cursor, record);
                break;
            case 0xe0:
            case 0xf5:
                cursor += 3;
                break;
            case 0xf9:
                if (returnCursor)
                    cursor = returnCursor;
                break;
            case 0xf1:
            case 0xf3:
            case 0xf4:
            case 0xfa:
            case 0xfb:
            case 0xfc:
            case 0xfd:
                break;
            default:
                result = 0;
                cursor = 0;
                break;
            }
        }
    }
    SetSTEP(previousStep);
    return result;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool IsScenarioFollowPromptAvailable() {
    if (GetCurrentStep() == 60000)
        return false;
    if (GetCurrentStep() == 70100)
        return false;
    if (GetCurrentStep() == 320000)
        return false;
    if (static_cast<std::int32_t>(GetCurrentStep()) > 39999 && static_cast<std::int32_t>(GetCurrentStep()) < 370000)
        return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::byte *GetScenarioPersistentScratch() {
    return g_scenarioPersistentFlags.scratch;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetNewScenarioTask(const std::byte *scnf) {
    g_scenarioEventContext->freeTalkData = scnf;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline std::uint8_t &ScenarioSpeakerLetter() {
    return reinterpret_cast<std::uint8_t *>(g_scenarioEventContext)[9];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline std::uint8_t &ScenarioListenerLetter() {
    return reinterpret_cast<std::uint8_t *>(g_scenarioEventContext)[10];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct ScenarioLineContext {
    std::byte retained00[0x14];
    ScenarioDialogueCue *cueCursor;
    std::byte retained18[8];
    shen_name lastSpeaker;
};

#define SCENARIO_LINE_CONTEXT (reinterpret_cast<ScenarioLineContext *>(g_scenarioEventContext))
#define SCENARIO_CUE_CURSOR (SCENARIO_LINE_CONTEXT->cueCursor)
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t ScntSigned12(std::uint8_t opcode, const std::byte *cursor) {
    const std::uint32_t value = ((opcode & 15U) << 8) | *cursor;
    return static_cast<std::int32_t>((value ^ 0x800U) - 0x800U);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t ScntSigned20(std::uint8_t opcode, const std::byte *cursor) {
    const std::uint32_t value = ((opcode & 15U) << 16) | (cursor[0] << 8) | cursor[1];
    return static_cast<std::int32_t>((value ^ 0x80000U) - 0x80000U);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SCENARIO_LINE_CLOCK() (static_cast<float>(g_scenarioLineStartTicks - *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister)) / 12500000.0f)
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SCENARIO_FRAME_TICKS() (*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister))
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
shen_name *GetScenarioCharacterAdmissionNames() {
    return g_scenarioCharacterAdmissionNames;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
Vector3 *srchGetScenarioAdmissionPositions() {
    return g_scenarioAdmissionPositions;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name GetActiveFreeTalkCharacter() {
    if (!g_scenarioEventContext)
        return 0;
    if (!g_scenarioEventContext->activeCharacter)
        return 0;
    const ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
    if (!record)
        return 0;
    if (record->executionState == 3)
        return 0;
    if (IsSystemScenarioRecordName(record->name))
        return 0;
    return record->name;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t HasActiveFreeTalkCharacter() {
    return GetActiveFreeTalkCharacter() != 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
shen_name GetActiveScenarioCharacter() {
    return g_scenarioEventContext->activeCharacter;
}
#endif

// @verified - d
bool IsScenarioCharacterForcedVisible(shen_name character) {
    for (std::int32_t index = 0; index < g_freeTalkVisibleCharacterCount; ++index) {
        if (g_freeTalkVisibleCharacters[index] == character)
            return true;
    }
    return false;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool scenIsRecordRunning(shen_name character) {
    if (!g_scenarioEventContext)
        return false;
    const ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(character);
    if (record) {
        if (record->executionState != 3)
            return true;
        return false;
    }
    return false;
}
#endif

// @verified - d
std::int32_t ScenarioCharacterIsRyo(shen_name name) {
    if (name) {
        Character *const character = scenFindChara(g_scenarioEventContext->scenarioTask, name);
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (control->ryo == character || name == CHARACTER_NAME_RYO || name == CHARACTER_NAME_NULL || name == CHARACTER_NAME_AKIR ||
            name == CHARACTER_NAME_AKID || name == CHARACTER_NAME_JAKR || name == CHARACTER_NAME_AKI)
            return 1;
    }
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
Character *ResolveScenarioTalkCharacter(shen_name name) {
    if (name == CHARACTER_NAME_AKIR || name == CHARACTER_NAME_RYO) {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        return control->ryo;
    }
    return scenFindChara(g_scenarioEventContext->scenarioTask, name);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsSystemScenarioRecordName(shen_name name) {
    std::int32_t result = 0;
    if ((name & 0xffffU) == 0x5953U && (name & 0xff000000U) != 0x5f000000U)
        result = 1;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t GetScenarioTalkCompletedCount(const ScenarioFreeTalkCharacter *character) {
    return character ? static_cast<std::int8_t>(character->state->completedCount) : 0;
}
#endif

// @verified - d
void SetScenarioTalkCompletedCount(ScenarioFreeTalkCharacter *character, std::int32_t count) {
    if (character) {
        std::uint8_t *const value = &character->state->completedCount;
        if (count >= 0) {
            if (count > 100)
                count = 100;
            *value = static_cast<std::uint8_t>(count);
        } else
            *value = 0;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t GetScenarioTalkActivationCount(const ScenarioFreeTalkCharacter *character) {
    return character ? static_cast<std::int8_t>(character->state->activationCount) : 0;
}
#endif

// @verified - d
void SetScenarioTalkActivationCount(ScenarioFreeTalkCharacter *character, std::int32_t count) {
    if (character) {
        std::uint8_t *const value = &character->state->activationCount;
        if (count >= 0) {
            if (count > 100)
                count = 100;
            *value = static_cast<std::uint8_t>(count);
        } else
            *value = 0;
    }
}

// @verified - d
void FreeTalkCharacterUpdateNullsub(Character *, void *) {
}

// @verified - d
void FreeTalkCharacterCleanup(Character *character, void *payload) {
    if (!character || !payload)
        return;
    ScenarioFreeTalkCharacter *const component = static_cast<ScenarioFreeTalkCharacter *>(payload);
    FreeTalkRegistration *const registration = component->localState.registration;
    if (!registration || character != registration->character || component != registration->component)
        return;
    if (registration->previous)
        registration->previous->next = registration->next;
    else
        g_freeTalkRegistrations = registration->next;
    if (registration->next)
        registration->next->previous = registration->previous;
    std::memset(static_cast<void *>(registration), 0, sizeof(*registration));
    registration->next = g_freeTalkFreeRegistrations;
    g_freeTalkFreeRegistrations = registration;
}

// @verified - d
void NotifyScenarioMemoFlag(std::int32_t flag) {
    g_scenarioFlagNotificationJingle = 0;
    if (!NotifyMemoFlag(flag, true))
        return;
    switch (flag) {
    case 88:
    case 89:
    case 106:
    case 304:
    case 426:
    case 455:
    case 467:
    case 552:
    case 686:
    case 1204:
    case 1587:
        return;
    default:
        g_scenarioFlagNotificationPending = 1;
        g_scenarioFlagNotificationFrames = 0;
        return;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const std::int8_t scenarioFlagBitMasks[8] = {1, 2, 4, 8, 16, 32, 64, -128};
#else
static const std::int8_t scenarioFlagBitMasks[8] = {1, 2, 4, 8, 16, 32, 64, -128};
#endif

void ClearOrSetShenmueFlags(std::int32_t flag, std::int32_t enabled) {
    if (flag >= 4096)
        return;
    if (enabled) {
        NotifyScenarioMemoFlag(flag);
        std::uint8_t *bank = g_scenarioPersistentFlags.internal;
        bank[flag >> 3] |= scenarioFlagBitMasks[flag & 7];
    } else {
        std::uint8_t *bank = g_scenarioPersistentFlags.internal;
        bank[flag >> 3] &= ~scenarioFlagBitMasks[flag & 7];
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t GetScenarioScratchFlag(std::int32_t flag) {
    if (flag <= 1023) {
        const std::uint8_t *data = reinterpret_cast<const std::uint8_t *>(&g_scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, scratch);
        data += flag >> 3;
        if (*data & scenarioFlagBitMasks[flag & 7])
            return 1;
        return 0;
    }
    return 0;
}
#endif

// @verified - d
void SetStoryFlag(std::int32_t flag, std::int32_t enabled) {
    if (flag >= 64)
        return;
    if (enabled)
        g_scenarioPersistentFlags.story[flag >> 3] |= scenarioFlagBitMasks[flag & 7];
    else
        g_scenarioPersistentFlags.story[flag >> 3] &= ~scenarioFlagBitMasks[flag & 7];
}

// @verified - d
void SetScenarioByteStat(std::int32_t index, std::int32_t value) {
    if (index < 64) {
        std::uint8_t *const data = reinterpret_cast<std::uint8_t *>(&g_scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, stats);
        data[index] = static_cast<std::uint8_t>(value);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name *GetScenarioFreeTalkParticipants(shen_name name) {
    if (FindScenarioFreeTalkCharacter(name))
        return g_freeTalkVisibleCharacters;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t StartScenarioFreeTalkRecord(shen_name name, std::int32_t root) {
    if (!g_scenarioFreeTalkPhase || IsScenarioBusy())
        return 0;
    ScenarioFreeTalkCharacter *record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
    if (record)
        InterruptScenarioFreeTalkRecord(record);
    FlushScenarioActionQueue();
    FreeTalkRegistration *registration = g_freeTalkRegistrations;
    while (registration && registration->name != name)
        registration = registration->next;
    record = registration ? registration->component : 0;
    if (record) {
        registration->availability = 1;
        g_scenarioEventContext->flags |= 1U;
        g_scenarioEventContext->activeCharacter = name;
        g_scenarioFaceForwardMode = g_scenarioFacingCommandsIssued = g_scenarioPlayerFacingIssued = g_scenarioNpcFacingIssued = 0;
        g_scenarioSuppressPlayerFacing = g_scenarioSuppressNpcFacing = 1;
        g_freeTalkVisibleCharacterCount = g_scenarioSelectionCount = 0;
        g_scenarioEventContext->flags |= 0x8000U;
        g_scenarioEventContext->flags |= 0x200U;
        record->executionState = 0;
        record->state->flags &= ~4U;
        record->state->flags &= ~2U;
        record->requestedSection = root;
        record->flags |= 0x20000U;
        record->flags &= ~0x2000U;
        record->flags &= ~0x4000U;
    }
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void StopScenarioFreeTalkRecord() {
    ScenarioEventContextPrefix *const context = g_scenarioEventContext;
    context->flags &= ~1U;
    context->flags &= ~0x200U;
    context->flags &= ~0x8000U;
    ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(context->activeCharacter);
    if (record) {
        EndScenarioPlayerPresentation(0);
        ReleaseScenarioTalkInput();
        InterruptScenarioFreeTalkRecord(record);
    }
    g_scenarioEventContext->activeCharacter = 0;
    FlushScenarioActionQueue();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitiateCleanupForSceneChara(shen_name name) {
    if (!g_scenarioFreeTalkPhase || (((g_scenarioEventContext->flags >> 15) & 1) && name == g_scenarioEventContext->activeCharacter))
        return;
    FreeTalkRegistration *registration = g_freeTalkRegistrations;
    while (registration && registration->name != name)
        registration = registration->next;
    if (!registration)
        return;
    ScenarioFreeTalkCharacter *const record = registration->component;
    record->executionState = 0;
    record->state->flags |= 4U;
    record->state->flags |= 1U;
    record->state->flags &= 0xfffdU;
    record->state->resumeOffsets[3] = 0;
    record->state->resumeOffsets[0] = 0;
    record->state->resumeOffsets[2] = 0;
    record->state->resumeOffsets[1] = 0;
    record->runtime.cursor = record->script;
    SetScenarioTalkCompletedCount(record, 0);
    SetScenarioTalkActivationCount(record, 0);
    record->flags &= ~0x2000U;
    record->flags &= ~0x4000U;
    record->flags &= ~0x100000U;
}
#endif

// @verified - d
ScenarioFreeTalkCharacter *FindScenarioFreeTalkCharacter(shen_name character) {
    ScenarioFreeTalkCharacter *record = 0;
    if (character) {
        FreeTalkRegistration *registration = g_freeTalkRegistrations;
        if (character & 0xffff0000U) {
            for (std::int32_t index = 0; index < g_scenarioEventContext->residentCharacterCount; ++index) {
                if (character == g_residentFreeTalkCharacters[index].name)
                    return &g_residentFreeTalkCharacters[index];
            }
            while (registration && registration->name != character)
                registration = registration->next;
        } else {
            while (registration && static_cast<std::uint16_t>(registration->savedStateIndex) != character)
                registration = registration->next;
        }
        if (registration && registration->availability > 0)
            record = registration->component;
    }
    return record;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ReadFreeTalk() {
    if (!g_scenarioEventContext)
        return;
    std::memset(g_residentFreeTalkCharacters, 0, sizeof(g_residentFreeTalkCharacters));
    std::int32_t count = 0;
    const std::byte *record;
    if (g_systemTalkData) {
        record = g_systemTalkData + 0x10;
        if (*reinterpret_cast<const std::uint32_t *>(record)) {
            do {
                ExpandFreeTalkRecord(record, &g_residentFreeTalkCharacters[count]);
                g_residentFreeTalkCharacters[count].executionState = 0;
                record += reinterpret_cast<const std::uint32_t *>(record)[1];
                if (++count > 15)
                    break;
            } while (*reinterpret_cast<const std::uint32_t *>(record));
        }
    }
    char filename[20];
    std::strcpy(filename, "freetlk0.bin");
    filename[7] += g_loadedActNumber;
    if (!shenFileExists("misc", filename))
        std::strcpy(filename, "freetalk.bin");
    const std::byte *const loaded = static_cast<const std::byte *>(static_cast<const void *>(shenLoadFile("misc", filename, SHEN_NAME4('S', 'C', 'T', 'T'))));
    g_externalFreeTalkRecords = loaded;
    if (g_externalFreeTalkRecords) {
        if (*reinterpret_cast<const std::uint32_t *>(g_externalFreeTalkRecords) == SHEN_NAME4('S', 'C', 'N', 'F'))
            g_externalFreeTalkRecords += 0x10;
        else
            g_externalFreeTalkRecords = 0;
    }
    g_extraFreeTalkRecord = 0;
    if (g_externalFreeTalkRecords) {
        record = g_externalFreeTalkRecords;
        if (*reinterpret_cast<const std::uint32_t *>(record)) {
            do {
                if (*reinterpret_cast<const std::uint32_t *>(record) == SHEN_NAME4('E', 'X', 'T', 'R'))
                    g_extraFreeTalkRecord = record;
                record += reinterpret_cast<const std::uint32_t *>(record)[1];
            } while (*reinterpret_cast<const std::uint32_t *>(record));
        }
    }
    if (g_scenarioEventContext->freeTalkData) {
        const std::byte *const end = g_scenarioEventContext->freeTalkData + reinterpret_cast<const std::uint32_t *>(g_scenarioEventContext->freeTalkData)[1];
        record = g_scenarioEventContext->freeTalkData + 0x10;
        if (count <= 15 && record < end) {
            do {
                ExpandFreeTalkRecord(record, &g_residentFreeTalkCharacters[count]);
                g_residentFreeTalkCharacters[count].executionState = 0;
                record += reinterpret_cast<const std::uint32_t *>(record)[1];
                ++count;
            } while (count <= 15 && record < end);
        }
    }
    g_scenarioEventContext->residentCharacterCount = count;
    if (!g_scenarioEventContext->freeTalkCharacter) {
        g_scenarioEventContext->freeTalkCharacter = AddCharacterHandler(3);
        if (g_scenarioEventContext->freeTalkCharacter) {
            CreateScenarioCharacterId(g_scenarioEventContext->scenarioTask, SHEN_NAME4('$', 'f', 'r', 'e'), g_scenarioEventContext->freeTalkCharacter);
            SetCharacterActive(g_scenarioEventContext->freeTalkCharacter, 0);
            HLib::SetupTimedTask(GetCharacterHandlerTask(g_scenarioEventContext->freeTalkCharacter), 0);
        }
    }
    reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->randomSelectionCount = 0;
    for (count = 0; count < 3; ++count)
        reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->randomSelections[count] = 0;
    g_scenarioEventContext->padControlData = AllocatePadControlEntry();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void scenUpdateFreeTalkInteraction() {
    if (!GetParentMenu() && ((g_scenarioEventContext->flags >> 9) & 1U) == 0)
        return;
    if (UpdateSelectorMemo() || IsSaveAllowed())
        return;
    if (!IsScreenFilmNotShowing(reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
        return;
    g_scenarioEventContext->flags &= ~0x100U;
    g_scenarioEventContext->flags &= ~0x20000U;
    scenUpdateCandidateAvailability();
    FreeTalkRegistration *selected = 0;
    if (g_scenarioEventContext->flags & 1U) {
        ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
        if (record) {
            scenExecuteRecord(record);
            if (IsScenarioActiveRecordIdle()) {
                g_scenarioEventContext->flags &= ~0x200U;
                g_scenarioEventContext->flags &= ~1U;
                g_scenarioEventContext->activeCharacter = shen_name();
                g_scenarioEventContext->flags &= ~0x8000U;
            }
        }
    } else {
        if (((g_scenarioEventContext->flags >> 15) & 1U) == 0) {
            const ScenarioFreeTalkCharacter *const active = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
            if (!active || !(active->flags & 0x1c000U)) {
                const bool lockon = srchCheckFlags(3) >= 0;
                g_scenarioEventContext->flags &= ~0x20U;
                selected = srchEvaluateSearchTarget();
                for (FreeTalkRegistration *candidate = g_freeTalkRegistrations; candidate; candidate = candidate->next) {
                    if (CanActivateScenarioCandidate() && candidate->availability > 0) {
                        SelectCharacterFacialNode(candidate->character, candidate == selected && selected->searchState > 0);
                        SetFaceFlagsForChara(candidate->character, 1);
                    }
                    if (lockon)
                        candidate->availability = -3;
                    if (candidate->availability > 0 && candidate != selected) {
                        if (candidate->searchState == 1)
                            candidate->searchState = 2;
                        scenActivateCandidate(candidate);
                    }
                }
                if (selected && selected->availability > 0)
                    scenActivateCandidate(selected);
            }
        }
        if (g_scenarioEventContext->activeCharacter == g_scenarioEventContext->previousCharacter && g_scenarioEventContext->activeCharacter) {
            ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
            if (record)
                scenExecuteRecord(record);
            else
                reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->pendingCharacter = g_scenarioEventContext->activeCharacter;
        } else {
            g_scenarioEventContext->previousCharacter = g_scenarioEventContext->activeCharacter;
        }
    }
    if (!selected)
        return;
    if (selected->component->name == static_cast<std::uint32_t>(srchCheckFlags(6))) {
        if (g_scenarioRetainedState950) {
            if (QueryNpcRouteLiveState(selected->component->name) == 11)
                return;
            SetScenarioFollowCharacter(shen_name());
            camCtrlSetState(0);
            g_scenarioRetainedState950 = 0;
        } else if (QueryNpcRouteLiveState(selected->component->name) == 11) {
            g_scenarioRetainedState950 = 0xffffffffU;
        }
    } else {
        g_scenarioRetainedState950 = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void GetScenarioSearchPosition(FreeTalkRegistration *candidate, Vector3 *position) {
    const Matrix4x4 *const matrices = mtwkGetDrawOverrideData(candidate->motionWork);
    if (matrices) {
        Vector3i rotation;
        GetCharacterWorldPosition(candidate->character, position);
        GetCharacterWorldRotation(candidate->character, &rotation);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::preTranslate(position->x, position->y, position->z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
        Matrix4x4::MultiplyWithPrePassMatrix(&matrices[candidate->meshIndex]);
        matrix_stack::GetTranslation(position);
        PopCurrentMatrix(1);
    } else if (!candidate->motionWork || !mtwkGetBonePosition(candidate->motionWork, static_cast<MotionBoneId>(5), position)) {
        GetCharacterWorldPosition(candidate->character, position);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
FreeTalkRegistration *srchEvaluateSearchTarget() {
    Vector3 position, selectedPosition;
    std::int16_t yaw;
    float distance;
    FreeTalkRegistration *candidate, *selected, *focused;
    float selectedDistance, sine;
    std::int32_t selectedYaw, focusedYaw, bestBucket, count, bucket, index;
    Character *ryo;
    focused = 0;
    selected = 0;
    selectedDistance = 1000.0f;
    selectedYaw = 0x7fff;
    focusedYaw = 0x7fff;
    bestBucket = 999;
    count = 0;
    const shen_name focus(static_cast<std::uint32_t>(srchCheckFlags(6)));
    scenarioSearchHasLockonPosition = srchCheckFlags(3) >= 0;
    if (scenarioSearchHasLockonPosition)
        scenarioSearchHasLockonPosition = RetrieveSearchFlagFromPtr(scenarioSearchLockonPosition);
    ryo = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (ryo && plymCanEvaluateSearchTargets()) {
        charaGetMotWorldPos(ryo, 5, &scenarioSearchPlayerPosition);
        scenarioSearchPlayerYaw = shenGetRotYFromNode(ryo);
        scenarioSearchFocusPosition = scenarioSearchPlayerPosition;
        if (focus)
            for (candidate = g_freeTalkRegistrations; candidate; candidate = candidate->next)
                if (focus == candidate->name) {
                    GetScenarioSearchPosition(candidate, &scenarioSearchFocusPosition);
                    break;
                }
        matrix_stack::UpdateWorldToScreenMatrix(640.0f, 480.0f);
        SetDebugScreenTextPosition(3, 7);
        for (candidate = g_freeTalkRegistrations; candidate; candidate = candidate->next) {
            if (!candidate->availability)
                continue;
            candidate->searchState = EvaluateScenarioSearchGeometry(candidate, &position, &distance, &yaw);
            candidate->distance = distance;
            if (candidate->availability > 0 && distance > 9.0f)
                candidate->availability = -2;
            if (candidate->searchState <= 0)
                continue;
            if (count < 30) {
                g_scenarioAdmissionPositions[count] = position;
                g_scenarioCharacterAdmissionNames[count] = candidate->name;
                ++count;
            }
            if (focus && focus == candidate->name) {
                focused = candidate;
                focusedYaw = yaw;
            }
            sine = dc::sdk::SinFixed(yaw);
            sine = sine * sine * 3.0f + 1.0f;
            bucket = static_cast<std::int32_t>(NativeSqrt(distance * sine) * 3.3333299f - 1.9f);
            if (bucket < 0)
                bucket = 0;
            if (bucket >= bestBucket - 1) {
                if (bucket > bestBucket)
                    continue;
                if ((yaw < 0 ? -yaw : yaw) >= (selectedYaw < 0 ? -selectedYaw : selectedYaw))
                    continue;
            }
            selected = candidate;
            selectedPosition = position;
            selectedDistance = distance;
            selectedYaw = yaw;
            bestBucket = bucket;
        }
        if (focused) {
            selected = focused;
            selectedPosition = scenarioSearchFocusPosition;
            selectedDistance = selected->distance;
            selectedYaw = focusedYaw;
            if (focus != g_scenarioEventContext->activeCharacter) {
                ScenarioFreeTalkCharacter *const previous = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
                if (previous)
                    previous->runtime.pendingCursor = 0;
                g_scenarioEventContext->activeCharacter = focus;
            }
        } else if (focus) {
            selected = 0;
        }
        if (selected) {
            for (index = 0; index < count; ++index)
                if (g_scenarioCharacterAdmissionNames[index] == selected->name)
                    break;
            if (index >= count) {
                g_scenarioAdmissionPositions[count] = selectedPosition;
                g_scenarioCharacterAdmissionNames[count++] = selected->name;
            }
        }
    }
    g_scenarioCharacterAdmissionNames[count] = shen_name();
    ScenarioEventContextStorage *const context = reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext);
    context->selectedTargetDistanceSquared = selectedDistance;
    context->selectedTargetAbsYaw = static_cast<std::uint16_t>(selectedYaw < 0 ? -selectedYaw : selectedYaw);
    return selected;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void scenActivateCandidate(FreeTalkRegistration *candidate) {
    ScenarioFreeTalkCharacter *const record = candidate->component;
    if (g_scenarioEventContext->activeCharacter && g_scenarioEventContext->activeCharacter != record->name)
        return;
    std::int8_t activate = candidate->searchState == 2 ? 0 : candidate->searchState;
    if (activate < 0 || candidate->distance < -100.0f)
        return;
    if ((record->flags >> 11) & 1U)
        activate = 1;
    if ((record->flags >> 1) & 1U)
        activate = 1;
    if (!activate) {
        if (record->runtime.pendingCursor) {
            if (CanActivateScenarioCandidate() && (plymGetMovingFrames() > 0 || plymGetTurningFrames() > 0)) {
                g_scenarioEventContext->flags |= 0x8000U;
                record->runtime.savedCursor = record->runtime.cursor;
                record->runtime.cursor = record->runtime.pendingCursor;
                record->flags |= 2U;
                g_scenarioFaceForwardMode = g_scenarioFacingCommandsIssued = g_scenarioPlayerFacingIssued = g_scenarioNpcFacingIssued = 0;
                g_scenarioSuppressPlayerFacing = g_scenarioSuppressNpcFacing = 1;
                activate = 1;
            }
            record->runtime.pendingCursor = 0;
        } else {
            if ((record->flags >> 18) & 1U) {
                reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->pendingCharacter = record->name;
                if (g_scenarioEventContext->activeCharacter == record->name) {
                    g_scenarioEventContext->activeCharacter = shen_name();
                    g_scenarioEventContext->flags &= ~0x8000U;
                }
                record->flags &= ~0x40000U;
                InterruptScenarioFreeTalkRecord(record);
            }
            record->flags &= ~0x20000U;
            if (((record->flags >> 25) & 1U) && (record->state->flags & 4U)) {
                record->executionState = 3;
                record->flags &= ~0x2000000U;
                record->flags &= ~0x100000U;
                record->state->resumeOffsets[1] = 0;
                record->state->resumeOffsets[2] = 0;
            }
        }
    }
    if (activate) {
        const ScenarioFreeTalkCharacter *const previous = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
        if (previous && g_scenarioEventContext->activeCharacter != record->name && ((previous->flags >> 17) & 1U))
            activate = 0;
    }
    if (!activate)
        return;
    if (((record->flags >> 18) & 1U) == 0)
        record->flags |= 0x40000U;
    record->flags |= 0x20000U;
    g_scenarioEventContext->flags |= 0x20U;
    g_scenarioEventContext->activeCharacter = record->name;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t EvaluateScenarioSearchGeometry(FreeTalkRegistration *candidate, Vector3 *position, float *distance, std::int16_t *yaw) {
    ScenarioFreeTalkCharacter *record;
    Character *character;
    std::int32_t index, targetYaw;
    std::int32_t found = 0;
    const shen_name focus(static_cast<std::uint32_t>(srchCheckFlags(6)));
    float dx, dy, dz, margin;
    dx = dz = 0.0f;
    margin = candidate->searchState > 0 ? 0.3f : -0.3f;
    *yaw = 0x7fff;
    record = candidate->component;
    character = candidate->character;
    if (!record->approachCount) {
        GetScenarioSearchPosition(candidate, position);
        dx = position->x - scenarioSearchPlayerPosition.x;
        dy = position->y - scenarioSearchPlayerPosition.y;
        dz = position->z - scenarioSearchPlayerPosition.z;
        *distance = dx * dx + dz * dz;
        if (focus == record->name)
            dy = 0.0f;
        if ((dy < 0.0f ? -dy : dy) > 2.0f)
            *distance = 99999.0f;
    } else {
        *distance = 99999.0f;
        record->selectedApproachPoint = 0;
        for (index = 0; index < static_cast<std::int8_t>(record->approachCount); ++index) {
            dx = reinterpret_cast<const Vector3 *>(record->vectors)[index].x - scenarioSearchPlayerPosition.x;
            dy = reinterpret_cast<const Vector3 *>(record->vectors)[index].y - scenarioSearchPlayerPosition.y;
            dz = reinterpret_cast<const Vector3 *>(record->vectors)[index].z - scenarioSearchPlayerPosition.z;
            float value = dx * dx + dy * dy + dz * dz;
            if ((dy < 0.0f ? -dy : dy) > 2.0f)
                value = 99999.0f;
            if (*distance > value) {
                *distance = value;
                record->selectedApproachPoint = reinterpret_cast<const Vector3 *>(record->vectors) + index;
            }
        }
        *position = *record->selectedApproachPoint;
        dy = position->y - scenarioSearchPlayerPosition.y;
    }
    const std::int16_t angle = InverseFixedAtan2(dx, dz);
    targetYaw = static_cast<std::int16_t>(angle - 0x8000);
    *yaw = static_cast<std::int16_t>(scenarioSearchPlayerYaw - angle);
    if (!(*distance < 14.0f)) {
        record->state->flags &= ~2U;
        record->state->flags |= 4U;
    }
    record->flags &= ~0x800000U;
    if (*distance < 4.0f)
        record->flags |= 0x800000U;
    index = 0;
    if (focus && (focus == record->name || !npcGetCanInteract(record->name))) {
        dx = position->x - scenarioSearchFocusPosition.x;
        dz = position->z - scenarioSearchFocusPosition.z;
        if (*distance < margin + 36.0f && (dy < 0.0f ? -dy : dy) < 2.0f && dx * dx + dz * dz < margin + 9.0f)
            index = 1;
    }
    if (scenarioSearchHasLockonPosition) {
        dx = position->x - scenarioSearchLockonPosition.x;
        dz = position->z - scenarioSearchLockonPosition.z;
        if (*distance < margin + 36.0f && (dy < 0.0f ? -dy : dy) < 2.0f && dx * dx + dz * dz < margin + 9.0f)
            index = 1;
    }
    if (!index && *distance < margin + 9.0f) {
        if (record->executionState != 3) {
            if (record->runtime.directionMode == 1) {
                if (static_cast<std::uint16_t>(record->runtime.maximumYaw - record->runtime.minimumYaw) <
                    static_cast<std::uint16_t>(targetYaw - record->runtime.minimumYaw))
                    index = -1;
            } else if (record->runtime.directionMode == 2 && character) {
                const std::int16_t localYaw = shenGetRotYFromNode(character);
                if (static_cast<std::uint16_t>(record->runtime.maximumYaw - record->runtime.minimumYaw) <
                    static_cast<std::uint16_t>(targetYaw - localYaw - record->runtime.minimumYaw))
                    index = -1;
            }
        }
        if (!index && static_cast<std::uint16_t>(angle - scenarioSearchPlayerYaw + 0x4000) <= 0x8000U) {
            Vector3 view;
            Matrix4x4::TransformPoint(*position, view);
            if (view.z < -0.1f) {
                Vector2 screen;
                matrix_stack::ProjectWorldPointToScreen(position, &screen);
                if (screen.x > 0.0f && screen.x < 640.0f && screen.y > 0.0f && screen.y < 480.0f)
                    index = 1;
            }
        }
    }
    if (index > 0) {
        if (focus && focus == record->name && npcGetCanInteract(focus)) {
            found = 1;
        } else {
            flddToggleFlag(0x1000U);
            if (!TestFieldWorldSegment(position, &scenarioSearchPlayerPosition))
                found = 1;
        }
    }
    return found;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InterruptScenarioFreeTalkRecord(ScenarioFreeTalkCharacter *record) {
    if (!record)
        return;
    if ((record->flags >> 12U) & 1U) {
        record->flags &= ~0x1000U;
        g_scenarioEventContext->activeCharacter = 0;
        g_scenarioEventContext->previousCharacter = 0;
        record->flags &= ~0x80000U;
        EndScenarioPlayerPresentation(0);
        ConsumeScenarioCameraRestoreRequest();
    }
    record->flags &= ~0x8000U;
    if (record->executionState == 7) {
        record->executionState = 1;
        record->flags &= ~0x2000U;
        record->flags &= ~0x200U;
        record->runtime.savedCursor = 0;
    }
    if ((record->flags >> 14U) & 1U)
        record->flags &= ~0x4000U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t ReadScenarioActionQueue(std::uint32_t index, void *output, std::int32_t size) {
    ScenarioActionQueue &queue = scenarioActionQueueStorage[index];
    if (index >= 2)
        return 0xffffffffU;
    if (!queue.count)
        return 0;
    if (size < *reinterpret_cast<const std::int32_t *>(queue.read))
        return 0xffffffffU;
    --queue.count;
    const std::int32_t payloadSize = *reinterpret_cast<const std::int32_t *>(queue.read);
    std::memcpy(output, queue.read + 4, payloadSize);
    if (queue.read == queue.write) {
        *reinterpret_cast<std::uint32_t *>(queue.read) = 0;
    } else {
        queue.read += ((static_cast<std::uint32_t>(payloadSize) + 3U) & ~3U) + 4;
        if (queue.read >= queue.storage + sizeof(queue.storage))
            queue.read = queue.storage;
        const std::uint32_t nextSize = *reinterpret_cast<const std::uint32_t *>(queue.read);
        if (queue.read + 4 + nextSize > queue.storage + sizeof(queue.storage))
            queue.read = queue.storage;
    }
    return payloadSize;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsScenarioActiveRecordIdle() {
    const ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
    if (!record)
        return true;
    const std::int32_t state = record->executionState;
    if (state > 3)
        return false;
    if (state <= 0)
        return false;
    if (!(record->flags & 0x1c000U))
        return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void QueueScenarioRecordAction(ScenarioFreeTalkCharacter *record, std::int32_t action) {
    switch (action) {
    case 204:
        record->state->flags |= 1U;
        break;
    case 206:
        record->flags |= 0x10000000U;
        break;
    }
    std::uint32_t data[2] = {record->name, static_cast<std::uint32_t>(action)};
    WriteScenarioActionQueue(g_scenarioActionQueueId, data, sizeof(data));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void QueueScenarioCharacterAction(shen_name name, std::int32_t action) {
    std::uint32_t data[2] = {name, static_cast<std::uint32_t>(action)};
    WriteScenarioActionQueue(g_scenarioActionQueueId, data, sizeof(data));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ReadScenarioAcknowledgement(void *output) {
    return ReadScenarioActionQueue(g_scenarioAcknowledgementQueueId, output, 8);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FlushScenarioActionQueue() {
    std::uint32_t action[2];
    while (ReadScenarioAcknowledgement(action)) {
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetScenarioFlagBanks(std::uint8_t **banks) {
    banks[0] = g_scenarioPersistentFlags.story;
    banks[1] = g_scenarioPersistentFlags.internal;
    banks[2] = g_scenarioPersistentFlags.stats;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t AllocateScenarioActionQueue(shen_name name) {
    std::int32_t index;
    for (index = 0; index < 2; ++index)
        if (scenarioActionQueueStorage[index].name == name)
            return index;
    for (index = 0; index < 2; ++index) {
        if (!scenarioActionQueueStorage[index].name) {
            scenarioActionQueueStorage[index].name = name;
            scenarioActionQueueStorage[index].count = 0;
            scenarioActionQueueStorage[index].read = scenarioActionQueueStorage[index].write = scenarioActionQueueStorage[index].storage;
            *reinterpret_cast<std::uint32_t *>(scenarioActionQueueStorage[index].read) = 0;
            return index;
        }
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t WriteScenarioActionQueue(std::uint32_t index, const void *input, std::uint32_t size) {
    ScenarioActionQueue &queue = scenarioActionQueueStorage[index];
    if (index >= 2)
        return -1;
    const std::uint32_t previousSize = *reinterpret_cast<const std::uint32_t *>(queue.write);
    if (previousSize)
        queue.write += ((previousSize + 3U) & ~3U) + 4;
    if (queue.write + (size + 4) > queue.storage + sizeof(queue.storage) - 4) {
        *reinterpret_cast<std::uint32_t *>(queue.write) = 0x80;
        queue.write = queue.storage;
    }
    if (queue.write < queue.read && queue.write + (size + 4) > queue.read)
        return -1;
    ++queue.count;
    *reinterpret_cast<std::uint32_t *>(queue.write) = size;
    std::memcpy(queue.write + 4, input, size);
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool CanActivateScenarioCandidate() {
    if (!IsPlayerControlEnabled() && ((g_scenarioEventContext->flags >> 9U) & 1U) == 0)
        return false;
    if (GetParentMenu() != 7 && ((g_scenarioEventContext->flags >> 9U) & 1U) == 0)
        return false;
    if (UpdateSelectorMemo())
        return false;
    if (IsWristwatchOpenable() && ((g_scenarioEventContext->flags >> 9U) & 1U) == 0)
        return false;
    if (srchCheckFlags(3) >= 0)
        return false;
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EndScenarioConversation() {
    ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
    if (record)
        InterruptScenarioFreeTalkRecord(record);
    for (std::int32_t index = 0; index < 101; ++index) {
        g_scenarioDialogueCues[index].triggerTime =
            static_cast<float>(g_scenarioLineStartTicks - *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister)) / 12500000.0f;
        g_scenarioDialogueCues[index].payload = 0;
        g_scenarioDialogueCues[index].speaker = shen_name();
        g_scenarioDialogueCues[index].retained0c = 0;
    }
    FlushScenarioActionQueue();
    ReleaseScenarioTalkInput();
    ConsumeScenarioCameraRestoreRequest();
    EndScenarioPlayerPresentation(0);
    if ((g_scenarioEventContext->flags >> 2) & 1)
        EndConversationCameraStream();
    g_scenarioEventContext->flags &= ~8U;
    g_scenarioEventContext->flags &= ~16U;
    g_scenarioEventContext->activeCharacter = shen_name();
    g_scenarioEventContext->flags &= ~1U;
    g_scenarioEventContext->flags &= ~0x200U;
    g_scenarioEventContext->flags &= ~0x800U;
    g_scenarioEventContext->flags &= ~0x8000U;
    g_scenarioLineStartTicks = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t IsScenarioBusy() {
    const bool conversationActive = cvsGet();
    return ((g_scenarioEventContext->flags >> 15U) & 1U) | static_cast<std::uint32_t>(conversationActive);
}
#endif

// @verified - d
std::int32_t HasScenarioCharacterControl() {
    const ScenarioEventContextPrefix *const context = g_scenarioEventContext;
    std::int32_t active = (context->flags >> 15U) & 1U;
    const ScenarioFreeTalkCharacter *const character = FindScenarioFreeTalkCharacter(context->activeCharacter);
    if (character && ((character->flags >> 1U) & 1U))
        active = 0;
    return active;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t HasScenarioRequestedRoot() {
    return (g_scenarioEventContext->flags >> 17U) & 1U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsScenarioBusyForMainIcon() {
    if (cvsGet())
        return true;
    if ((g_scenarioEventContext->flags >> 15) & 1) {
        if (!scenarioHardBusy)
            return true;
    }
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t scenCheckActiveRecordBusy() {
    return g_scenarioEventContext->activeCharacter && ((g_scenarioEventContext->flags >> 8) & 1U) == 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void scenUpdateCandidateAvailability() {
    const ScenarioFreeTalkCharacter *record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
    if (!record || ((record->flags >> 17) & 1U) == 0) {
        g_scenarioFaceForwardMode = g_scenarioFacingCommandsIssued = g_scenarioPlayerFacingIssued = g_scenarioNpcFacingIssued = g_scenarioSuppressPlayerFacing =
            g_scenarioSuppressNpcFacing = 0;
        if (g_scenarioEventContext->activeCharacter && ((g_scenarioEventContext->flags >> 9) & 1U) == 0)
            EndScenarioConversation();
    }
    for (FreeTalkRegistration *candidate = g_freeTalkRegistrations; candidate; candidate = candidate->next) {
        const Character *const character = candidate->character;
        record = candidate->component;
        candidate->availability = 0;
        if (!character || !record || authGetLoaded() || !IsCharacterValid(character) || !IsOfThisVisibilityType(character, CVM_DisplayEnabled) ||
            !record->name || !record->source || !record->script)
            continue;
        if (!npcExists(record->name) || CanNpcEnterScenario(record->name))
            candidate->availability = 1;
        else
            candidate->availability = -1;
    }
}
#endif

// @verified - d
void ExpandFreeTalkRecord(const std::byte *source, ScenarioFreeTalkCharacter *record) {
    const std::uint32_t *const words = reinterpret_cast<const std::uint32_t *>(source);
    record->source = source;
    record->name = shen_name(words[0]);
    record->voicePath = reinterpret_cast<const char *>(source + words[4]);
    record->script = source + words[5];
    record->lineTable = source + words[6];
    record->locationCount = static_cast<std::uint8_t>(source[8]);
    record->participantCount = static_cast<std::uint8_t>(source[10]);
    record->approachCount = static_cast<std::uint8_t>(source[9]);
    record->keyValueCount = static_cast<std::uint8_t>(source[11]);
    record->participants = source + words[8];
    record->locations = source + words[10];
    record->vectors = source + words[9];
    record->keyValues = source + words[11];
    record->savedStateIndex = 0;
    const char *path = std::strrchr(record->voicePath, '/');
    {
        if (!path)
            goto done;
        char c = *++path;
        if (c != 'F' && c != 'H' && c != 'Q' && c != 'K')
            goto done;
        c = *++path;
        if (c != '2')
            goto done;
        c = *++path;
        if (c < '0')
            goto done;
        if (c > '9')
            goto done;
        std::int32_t index = c - '0';
        index *= 100;
        c = *++path;
        if (c < '0')
            goto done;
        if (c > '9')
            goto done;
        index += (c - '0') * 10;
        c = *++path;
        if (c < '0')
            goto done;
        if (c > '9')
            goto done;
        index += c - '0';
        if (index < 512)
            record->savedStateIndex = static_cast<std::uint16_t>(index);
    }
done:
    std::int16_t savedIndex;
    record->state = (savedIndex = record->savedStateIndex) ? &g_savedFreeTalkStates[static_cast<std::uint16_t>(savedIndex)] : &record->localState.saved;
    record->executionState = 1;
    record->selection = -1;
    *reinterpret_cast<std::uint32_t *>(&record->localState.saved.flags) |= 4U;
    record->commandState = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsActiveScenarioCameraSuppressed() {
    const ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter);
    if (record) {
        if ((record->flags >> 19) & 1)
            return true;
        return false;
    }
    return false;
}
#endif

// @verified - d
std::int32_t VisitScntBranch(const std::byte *target) {
    std::int32_t index;
    for (index = 0; index < scntBranchCount; ++index)
        if (target == scntBranchTargets[index])
            break;
    if (index < scntBranchCount) {
        scntBranchRepeated = 1;
    } else {
        if (scntBranchCount > 99) {
            for (index = 1; index < scntBranchCount; ++index)
                scntBranchTargets[index - 1] = scntBranchTargets[index];
            --scntBranchCount;
        }
        scntBranchTargets[scntBranchCount++] = target;
    }
    return scntBranchRepeated;
}
// @verified - d
void ResetScntBranchHistory() {
    scntBranchCount = 0;
    scntBranchRepeated = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SCENARIO_EVENT_STORAGE (reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext))
struct ScenarioCameraPacketHeader {
    std::uint8_t kind, retained01, retained02, flags;
};
#define SCENARIO_PACKET_HEADER(packet) (reinterpret_cast<ScenarioCameraPacketHeader *>(packet))

static inline shen_name ScenarioPlayerName() {
    return scenGetCharaName(g_scenarioEventContext->scenarioTask,
        static_cast<const Character *>(reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo));
}

const std::byte *scenBuildDialogueCues(ScenarioFreeTalkCharacter *record, const std::byte *cursor) {
    ConversationDialogueRecord *packet;
    shen_name previousSpeaker = 0, speaker = 0, listener = 0;
    std::int32_t flags = 0, approachUsed = 0, speakerLetter = 0;
    float time = 0.0f, unattachedTime = 0.0f;
    ScenarioDialogueCue *cue;
    std::int32_t operand, index;
    if (record->participants) {
        if (ScenarioSpeakerLetter())
            speaker = ResolveScenarioParticipant(reinterpret_cast<const shen_name *>(record->participants)[ScenarioSpeakerLetter() - 'A'], record);
        if (ScenarioListenerLetter())
            listener = ResolveScenarioParticipant(reinterpret_cast<const shen_name *>(record->participants)[ScenarioListenerLetter() - 'A'], record);
    }
    g_scenarioEventContext->flags &= ~4U;
    packet = g_scenarioCameraRecords;
    cue = SCENARIO_CUE_CURSOR = g_scenarioDialogueCues;
    ResetScntBranchHistory();
    for (;;) {
        const std::uint8_t opcode = *cursor++;
        switch (opcode & 0xf0) {
        case 0x00:
            ++cursor;
            continue;
        case 0x20:
            operand = ((opcode & 15) << 8) | *cursor++;
            if (!record->dialogue.firstLine)
                record->dialogue.firstLine = record->lineTable + operand * 16;
            break;
        case 0x30: {
            cue->triggerTime = time;
            const std::int32_t code = (opcode << 8) | *cursor++;
            cue->speaker = code;
            cue->payload = 0;
            index = code & 0xfff;
            if (static_cast<std::uint32_t>(index - 20) < 10 || static_cast<std::uint32_t>(index - 217) < 30 || static_cast<std::uint32_t>(index - 120) < 10) {
                g_scenarioEventContext->flags |= 0x1000U;
                flags |= 2;
                if (static_cast<std::uint32_t>(index - 20) < 10)
                    index -= 20;
                else if (static_cast<std::uint32_t>(index - 217) < 30)
                    index -= 207;
                else
                    index -= 120;
                camCtrlSetConversationLookTarget(index >= 0 && index < static_cast<std::int8_t>(record->participantCount) ?
                        reinterpret_cast<const Vector3 *>(record->locations) + index :
                        &g_scenarioDefaultLookLocation);
            } else if (index == 10)
                flags |= 8;
            ++cue;
            if (packet != g_scenarioCameraRecords) {
                packet[-1].duration += 0.033333335f;
                time += 0.033333335f;
            }
            continue;
        }
        case 0x40:
            operand = ((opcode & 15) << 8) | *cursor++;
            if (!g_scntExpressionResult) {
                if (operand & 0x800)
                    operand |= 0xfffff000;
                cursor += operand;
            }
            continue;
        case 0x50:
            operand = ((opcode & 15) << 8) | *cursor++;
            if (operand & 0x800)
                operand |= 0xfffff000;
            cursor += operand;
            if (VisitScntBranch(cursor))
                goto done;
            continue;
        case 0x60:
        case 0x70:
            cue->triggerTime = time;
            cue->speaker = (opcode << 8) | *cursor++;
            cue->payload = 0;
            ++cue;
            continue;
        case 0x80:
            operand = *cursor++ << 8;
            operand += *cursor++;
            operand += (opcode & 15) << 16;
            if (operand & 0x80000)
                operand |= 0xfff00000;
            record->runtime.returnCursor = cursor;
            cursor += operand;
            continue;
        case 0x90:
            ++cursor;
            if (!(record->flags & 0x202U) && !record->localState.requestedRoot) {
                record->flags |= 0x100000U;
                record->state->resumeOffsets[2] =
                    static_cast<std::uint16_t>(reinterpret_cast<std::int32_t>(cursor) - reinterpret_cast<std::int32_t>(record->script));
                record->runtime.pendingCursor = 0;
            }
            continue;
        case 0xa0:
            operand = *cursor++;
            cue->triggerTime = time;
            cue->speaker = (opcode << 16) | (operand << 8) | *cursor++;
            cue->payload = 0;
            ++cue;
            continue;
        case 0xb0:
            switch (opcode & 15) {
            case 0:
                cue->triggerTime = time;
                cue->speaker = opcode;
                cue->retained0c = (cursor[3] << 24) | (cursor[2] << 16) | (cursor[1] << 8) | cursor[0];
                cue->payload = 0;
                cursor += 4;
                ++cue;
                continue;
            case 1:
                cue->triggerTime = time;
                cue->speaker = opcode;
                cue->retained0c = *cursor++;
                cue->payload = 0;
                ++cue;
                continue;
            }
            continue;
        case 0xc0:
            switch (opcode & 15) {
            case 1:
                ScenarioSpeakerLetter() = *cursor++;
                continue;
            case 2:
                ScenarioListenerLetter() = *cursor++;
                continue;
            case 3:
                reinterpret_cast<std::uint8_t *>(&record->retained08)[0] = *cursor++;
                continue;
            case 4:
                reinterpret_cast<std::uint8_t *>(&record->retained08)[1] = *cursor++;
                continue;
            case 5:
                reinterpret_cast<std::uint8_t *>(&record->retained08)[2] = *cursor++;
                continue;
            case 6:
                reinterpret_cast<std::uint8_t *>(&record->retained08)[3] = *cursor++;
                continue;
            case 7:
                record->selection = static_cast<std::int8_t>(*cursor++);
                continue;
            }
            ++cursor;
            continue;
        case 0xd0:
            operand = *cursor++ << 8;
            operand += *cursor++;
            operand += (opcode & 15) << 16;
            if (operand & 0x80000)
                operand |= 0xfff00000;
            cursor += operand;
            if (VisitScntBranch(cursor))
                goto done;
            continue;
        case 0xe0:
            operand = *cursor++;
            if (opcode == 0xe0) {
                index = static_cast<std::int32_t>(dc::sdk::RandomFloat() * static_cast<float>(operand));
                if (SCENARIO_EVENT_STORAGE->randomSelectionCount < 3 &&
                    SCENARIO_EVENT_STORAGE->randomSelections[SCENARIO_EVENT_STORAGE->randomSelectionCount] == static_cast<std::uint32_t>(index)) {
                    if (!index)
                        index = operand;
                    --index;
                }
                if (SCENARIO_EVENT_STORAGE->randomSelectionCount < 3) {
                    SCENARIO_EVENT_STORAGE->randomSelections[SCENARIO_EVENT_STORAGE->randomSelectionCount] = index;
                    ++SCENARIO_EVENT_STORAGE->randomSelectionCount;
                }
                for (; index; --index) {
                    operand = ((cursor[0] & 15) << 8) | cursor[1];
                    cursor += operand + 2;
                }
                continue;
            }
            cursor -= 2;
            goto done;
        case 0xf0:
            if (opcode == 0xfc) {
                cue->triggerTime = time;
                cue->speaker = 0xfc00;
                cue->payload = 0;
                ++cue;
                continue;
            }
            if (opcode >= 0xf6 && opcode <= 0xf8)
                flags |= 4;
            else
                switch (opcode) {
                case 0xf0:
                    cursor = DecodeScntExpression(cursor, record);
                    continue;
                case 0xf1:
                case 0xfd:
                    continue;
                case 0xf9:
                    if ((record->flags >> 9) & 1U) {
                        record->flags &= ~0x200U;
                        cursor = record->runtime.savedCursor;
                        record->runtime.savedCursor = 0;
                        continue;
                    }
                    if (record->runtime.returnCursor) {
                        cursor = record->runtime.returnCursor;
                        record->runtime.returnCursor = 0;
                        continue;
                    }
                    break;
                case 0xff:
                    break;
                }
            --cursor;
            goto done;
        default:
            --cursor;
            goto done;
        }
        const std::byte *const line = record->lineTable + operand * 16;
        float duration;
        if (*reinterpret_cast<const float *>(line + 12) != 0.0f)
            duration = *reinterpret_cast<const float *>(line + 12) + 0.5f;
        else
            duration = (static_cast<float>(static_cast<std::int32_t>(
                            std::strlen(reinterpret_cast<const char *>(record->source) + *reinterpret_cast<const std::int32_t *>(line)))) *
                               scenarioLineLetterFrames * 0.5f +
                           scenarioLineBaseFrames) /
                30.0f;
        SCENARIO_PACKET_HEADER(packet)->flags = flags;
        SCENARIO_PACKET_HEADER(packet)->kind = 4;
        SCENARIO_PACKET_HEADER(packet)->retained01 = 1;
        SCENARIO_PACKET_HEADER(packet)->retained02 = 1;
        packet->duration = duration;
        cue->triggerTime = time;
        cue->speaker = cue->retained0c = CHARACTER_NAME_NULL;
        cue->payload = line;
        if (record->selectedApproachPoint && !approachUsed) {
            approachUsed = 1;
            speaker = shen_name(SHEN_NAME4('$', 'f', 'r', 'e'));
            listener = ScenarioPlayerName();
            SetCharacterWorldPosition(g_scenarioEventContext->freeTalkCharacter, record->selectedApproachPoint);
        }
        shen_name name;
        if (speaker)
            name = speaker;
        else {
            speakerLetter = static_cast<std::int8_t>(line[8]);
            name = 0;
            if (record->participants)
                name = ResolveScenarioParticipant(reinterpret_cast<const shen_name *>(record->participants)[speakerLetter - 'A'], record);
        }
        if (!name) {
            if (speakerLetter == 'A')
                name = ScenarioPlayerName();
            else
                name = record->name;
        }
        if (name == previousSpeaker) {
            cue->speaker = name;
            packet[-1].duration += duration;
            SCENARIO_PACKET_HEADER(packet - 1)->flags |= flags;
        } else {
            shen_name addressed;
            if (listener)
                addressed = listener;
            else if (!previousSpeaker) {
                if (name != record->name) {
                    addressed = record->name;
                    if (!ResolveScenarioTalkCharacter(addressed) && record->participants)
                        for (index = 0; index < static_cast<std::int8_t>(record->locationCount); ++index)
                            if (reinterpret_cast<const shen_name *>(record->participants)[index] &&
                                ResolveScenarioParticipant(reinterpret_cast<const shen_name *>(record->participants)[index], record) != name) {
                                addressed = ResolveScenarioParticipant(reinterpret_cast<const shen_name *>(record->participants)[index], record);
                                break;
                            }
                } else
                    addressed = ScenarioPlayerName();
            } else
                addressed = previousSpeaker;
            packet->speaker = ResolveScenarioTalkCharacter(name);
            packet->listener = ResolveScenarioTalkCharacter(addressed);
            if (packet->speaker && packet->listener) {
                if (packet == g_scenarioCameraRecords) {
                    packet->duration += unattachedTime;
                    SCENARIO_PACKET_HEADER(packet)->flags |= flags;
                }
                if (packet < g_scenarioCameraRecords + 23) {
                    ++packet;
                    cue->speaker = name;
                    cue->retained0c = addressed;
                } else {
                    packet[-1].duration += duration;
                    SCENARIO_PACKET_HEADER(packet - 1)->flags |= flags;
                }
            } else if (packet == g_scenarioCameraRecords)
                unattachedTime += duration;
            else {
                packet[-1].duration += duration;
                SCENARIO_PACKET_HEADER(packet - 1)->flags |= flags;
            }
        }
        ++cue;
        previousSpeaker = name;
        listener = 0;
        speaker = 0;
        speakerLetter = 0;
        flags = 0;
        time += duration;
    }
done:
    ScenarioSpeakerLetter() = ScenarioListenerLetter() = 0;
    SCENARIO_PACKET_HEADER(packet)->flags = flags | 1;
    SCENARIO_PACKET_HEADER(packet)->kind = 2;
    SCENARIO_PACKET_HEADER(packet)->retained01 = 0;
    SCENARIO_PACKET_HEADER(packet)->retained02 = 0;
    packet->duration = time;
    packet->speaker = 0;
    packet->listener = 0;
    cue->triggerTime = time;
    cue->payload = 0;
    cue->speaker = cue->retained0c = 0;
    if ((SCENARIO_PACKET_HEADER(g_scenarioCameraRecords)->flags & 1) == 0 && ((record->flags >> 1) & 1U) == 0 && record->localState.requestedRoot != 3 &&
        record->localState.requestedRoot != 4)
        g_scenarioEventContext->flags |= 4U;
    return cursor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
#define SCENARIO_LIP_CLIP (reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->lipClip)
static void BuildScenarioLipSequence(ScenarioFreeTalkCharacter *record) {
    const float duration = *reinterpret_cast<const float *>(record->dialogue.firstLine + 12);
    std::int32_t count;
    if (duration != 0.0f)
        count = static_cast<std::int32_t>(duration * 60.0f * 0.125f);
    else
        count = std::strlen(reinterpret_cast<const char *>(record->source) + *reinterpret_cast<const std::int32_t *>(record->dialogue.firstLine)) >> 2;
    if (count > 37)
        count = 37;
    SCENARIO_LIP_CLIP[0] = 0;
    SCENARIO_LIP_CLIP[1] = 10;
    std::int32_t index = 2;
    while (index < count * 2) {
        SCENARIO_LIP_CLIP[index] = static_cast<std::int16_t>(dc::sdk::RandomFloat() * 6.0f);
        SCENARIO_LIP_CLIP[index + 1] = 8;
        index += 2;
    }
    SCENARIO_LIP_CLIP[index] = 0;
    SCENARIO_LIP_CLIP[index + 1] = 10;
    SCENARIO_LIP_CLIP[index + 2] = -1;
}
#undef SCENARIO_LIP_CLIP
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static const std::byte *FindScenarioIndexedSection(ScenarioFreeTalkCharacter *record, std::int32_t section) {
    const std::byte *cursor = record->script;
    std::int32_t index = 0;
    std::uint8_t opcode;
    std::uint32_t length;
    while (1) {
        opcode = *cursor++;
        if ((opcode & 0xf0U) != 0x10U)
            continue;
        length = ((opcode & 15U) << 8) | *cursor++;
        if (index == section)
            break;
        ++index;
        cursor += length;
    }
    return cursor - 2;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t CanSkipScenarioLine(ScenarioFreeTalkCharacter *record) {
    if (record) {
        if (!GetS2DialogueMode())
            return 0;
        if (scenarioLineHoldFrames > 0)
            return 0;
        if (((g_scenarioEventContext->flags >> 10U) & 1U) == 0)
            return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static const std::byte *AdvanceScenarioCharacterCommandStream(ScenarioFreeTalkCharacter *record) {
    const std::byte *cursor;
    std::uint8_t opcode;
    std::int32_t operand, low;
    g_scenarioSelectionCount = 0;
    ResetScntBranchHistory();
    cursor = record->script;
    do {
        opcode = *cursor++;
        if (opcode < 0x80) {
            operand = ((opcode & 15) << 8) + *cursor++;
            switch (opcode & 0xf0) {
            case 0x00:
                continue;
            case 0x10:
                return cursor;
            case 0x40:
                if (!g_scntExpressionResult) {
                    if (operand & 0x800)
                        operand |= 0xfffff000;
                    cursor += operand;
                }
                continue;
            case 0x50:
                if (operand & 0x800)
                    operand |= 0xfffff000;
                cursor += operand;
                if (VisitScntBranch(cursor))
                    return 0;
                continue;
            }
            return 0;
        } else if (opcode < 0xe0) {
            low = opcode & 15;
            switch (opcode & 0xf0) {
            case 0xa0:
                operand = *cursor++;
                g_scenarioQueuedFaceLooks[g_scenarioSelectionCount++] = (opcode << 16) | (operand << 8) | *cursor++;
                continue;
            case 0xb0:
                if (low == 1) {
                    record->commandState = *cursor++;
                    continue;
                }
                return 0;
            case 0xd0:
                operand = *cursor++ << 8;
                operand += *cursor++;
                operand += low << 16;
                if (operand & 0x80000)
                    operand |= 0xfff00000;
                cursor += operand;
                if (VisitScntBranch(cursor))
                    return 0;
                continue;
            }
            return 0;
        } else {
            switch (opcode) {
            case 0xf0:
                cursor = DecodeScntExpression(cursor, record);
                continue;
            case 0xf1:
                continue;
            case 0xf2:
            case 0xf3:
            case 0xff:
            default:
                return 0;
            }
        }
    } while (1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static inline void ResolveScenarioChoiceSlot(ScenarioFreeTalkCharacter *record) {
    std::int32_t slot;
    for (slot = 0; slot < 4; ++slot)
        if (scenarioChoiceIds[slot] == static_cast<std::uint16_t>(record->dialogueFlagId))
            break;
    if (slot >= 4)
        record->dialogueFlagId = 0;
    else {
        g_scenarioSelectedFollowFlag = static_cast<std::uint16_t>(record->dialogueFlagId);
        record->dialogueFlagId = static_cast<std::int16_t>(slot + 1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RecordScenarioTalkOccurrence(std::uint16_t index) {
    if (index < 512) {
        if (g_scenarioTalkOccurrences[index] < 255)
            ++g_scenarioTalkOccurrences[index];
        RecordScenarioTalkGroup(index);
        RecordRecentScenarioTalk(index);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BeginScenarioTalkSession(ScenarioFreeTalkCharacter *record) {
    std::int32_t index;
    if (((g_scenarioEventContext->flags >> 15) & 1U) == 0)
        g_scenarioFaceForwardMode = g_scenarioFacingCommandsIssued = g_scenarioPlayerFacingIssued = g_scenarioNpcFacingIssued = g_scenarioSuppressPlayerFacing =
            g_scenarioSuppressNpcFacing = 0;
    g_freeTalkVisibleCharacters[0] = 0;
    g_freeTalkVisibleCharacters[1] = 0;
    g_freeTalkVisibleCharacters[2] = 0;
    g_freeTalkVisibleCharacters[3] = 0;
    g_freeTalkVisibleCharacters[4] = 0;
    g_freeTalkVisibleCharacters[5] = 0;
    g_freeTalkVisibleCharacters[6] = 0;
    g_freeTalkVisibleCharacters[7] = 0;
    g_scenarioParticipantFaceLooks[0] = 0;
    g_scenarioParticipantFaceLooks[1] = 0;
    g_scenarioParticipantFaceLooks[2] = 0;
    g_scenarioParticipantFaceLooks[3] = 0;
    g_scenarioParticipantFaceLooks[4] = 0;
    g_scenarioParticipantFaceLooks[5] = 0;
    g_scenarioParticipantFaceLooks[6] = 0;
    g_scenarioParticipantFaceLooks[7] = 0;
    BeginScenarioPlayerPresentation();
    g_scenarioEventContext->flags = (g_scenarioEventContext->flags | 0x8000U) & ~0x1000U;
    record->flags &= ~0x1000U;
    record->flags &= ~0x200000U;
    record->flags &= ~0x400000U;
    record->selection = -1;
    record->flags &= ~0x100U;
    record->runtime.pendingCursor = 0;
    for (index = 0; index < (record->voicePath - reinterpret_cast<const char *>(record->participants)) / 4; ++index) {
        g_freeTalkVisibleCharacters[index] = ResolveScenarioParticipant(reinterpret_cast<const shen_name *>(record->participants)[index], record);
        if (!g_freeTalkVisibleCharacters[index])
            continue;
        Character *const character = ResolveScenarioTalkCharacter(g_freeTalkVisibleCharacters[index]);
        if (character) {
            SelectCharacterFacialNode(character, 1);
            SetFaceFlagsForChara(character, 0);
            SetHighModelAndFaceFlagsForChara(character, 0, 10, 1.0f);
            CharacterHandComponent *hand = AllocateHNDR(character);
            if (hand)
                SetCharacterHandRegionEnabled(hand, 1, 0, -1);
            hand = AllocateHNDL(character);
            if (hand)
                SetCharacterHandRegionEnabled(hand, 1, 0, -1);
            if (ScenarioCharacterIsRyo(g_freeTalkVisibleCharacters[index]) && hand && GetInternalFlag(123))
                ParseHandPoseCommand(*hand, reinterpret_cast<std::byte *>(scenarioInitialHandPose), 0);
        }
        if (!ScenarioCharacterIsRyo(g_freeTalkVisibleCharacters[index]) && !plymIsAuthorTargetActive())
            BeginNpcSpecialMotion(g_freeTalkVisibleCharacters[index]);
    }
    g_freeTalkVisibleCharacterCount = index;
    for (index = 0; index < g_scenarioSelectionCount; ++index)
        ApplyScenarioFaceLook(g_scenarioQueuedFaceLooks[index]);
    g_scenarioSelectionCount = 0;
    g_scenarioSubtitleHoldFrames = 0;
    g_scenarioLookTargetActive = 0;
    g_scenarioSelectedFollowFlag = -1;
    if (g_scenarioRetainedState950) {
        SetScenarioFollowCharacter(shen_name());
        g_scenarioRetainedState950 = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void EndScenarioTalkSession(ScenarioFreeTalkCharacter *record) {
    record->flags &= ~0x1000U;
    g_scenarioRetainedLookTarget = -1;
    record->flags |= 0x8000U;
    record->runtime.followFrames = 0;
    QueueScenarioRecordAction(record, -8);
    ConsumeScenarioCameraRestoreRequest();
    ReleaseScenarioTalkInput();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static inline void SetScenarioCurrentSpeaker(ScenarioFreeTalkCharacter *record) {
    record->state->flags &= 0xfffbU;
    if (SCENARIO_LINE_CONTEXT->lastSpeaker != record->name) {
        ScenarioFreeTalkCharacter *const previous = FindScenarioFreeTalkCharacter(SCENARIO_LINE_CONTEXT->lastSpeaker);
        if (previous)
            previous->state->flags |= 4U;
        SCENARIO_LINE_CONTEXT->lastSpeaker = record->name;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void scenExecuteRecord(ScenarioFreeTalkCharacter *record) {
    if (!record)
        return;
    if (record->runtime.pendingCursor && cvsGet())
        record->runtime.pendingCursor = 0;
    g_scenarioEventContext->flags |= 0x100U;
    const std::uint32_t rootRequested = record->localState.requestedRoot != 0;
    const std::uint32_t rootRequestedBit = rootRequested & 1U;
    const std::uint32_t rootRequestedFlag = rootRequestedBit << 17;
    g_scenarioEventContext->flags = (g_scenarioEventContext->flags & ~0x20000U) | rootRequestedFlag;
    record->flags &= ~0x1000000U;
    reinterpret_cast<std::uint16_t &>(record->dialogueFlagId) = 0xffffU;
    if ((record->flags >> 3) & 1U) {
        if (!IsCtrlSpeakIconInactive()) {
            if (record->localState.requestedRoot != 4)
                return;
        } else {
            record->flags &= ~0x40U;
            record->dialogueFlagId = shenGetCtrlSpeakIconFlag();
            if (static_cast<std::uint16_t>(record->dialogueFlagId) == 0xffffU)
                record->dialogueFlagId = 0;
        }
    }
    if ((record->flags >> 4) & 1U) {
        if (!IsCtrlSpeakIconInactive()) {
            if (record->localState.requestedRoot != 4)
                return;
        } else {
            record->flags &= ~0x40U;
            record->dialogueFlagId = shenGetCtrlSpeakIconFlag();
            if (static_cast<std::uint16_t>(record->dialogueFlagId) == 0xffffU)
                record->dialogueFlagId = 0;
        }
    }
    if ((record->flags >> 2) & 1U) {
        if (!IsCtrlSpeakIconInactive()) {
            if (record->localState.requestedRoot != 4)
                return;
        } else {
            record->flags &= ~0x40U;
            const std::uint8_t answer = shenGetCtrlSpeakIconFlag();
            if (!answer)
                record->dialogueFlagId = 1;
            else if (answer == 1)
                record->dialogueFlagId = 0;
            else
                record->dialogueFlagId = answer;
            if (static_cast<std::uint16_t>(record->dialogueFlagId) == 0xffffU)
                return;
        }
    }
    if ((record->flags >> 5) & 1U) {
        const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(g_scenarioEventContext->padControlData);
        if (scenarioPromptFrames++ >= 180 || (input && (input->pressedPriority & 0x400U)))
            record->dialogueFlagId = 1;
        else if (!input || (input->pressedPriority & 0x200U) || (input->held & 0xf0c0U) ||
            *reinterpret_cast<const std::uint16_t *>(&input->leftStickX) != 0x8080U)
            record->dialogueFlagId = 0;
        else {
            Vector3 position;
            charaGetMotWorldPos(ctrlGetScenarioChara(record->name), 5, &position);
            g_scenarioTalkPromptResumed = 0;
            RegisterButtonMediation(shen_name(SHEN_NAME4('B', 'T', 'F', 'T')), &position, 0x10005U);
            return;
        }
    }
    if (((g_scenarioEventContext->flags >> 3) & 1U) && ((g_scenarioEventContext->flags >> 2) & 1U))
        camCtrlHoldConversationShot((g_scenarioEventContext->flags >> 4) & 1U);
    while (record->flags & 0x1e000U) {
        std::uint32_t acknowledgement[2];
        if (ReadScenarioAcknowledgement(acknowledgement) < 1) {
            if ((record->flags >> 13) & 1U)
                ++record->runtime.actionFrames;
            if ((record->flags >> 14) & 1U)
                ++record->runtime.startFrames;
            if ((record->flags >> 15) & 1U)
                ++record->runtime.followFrames;
            if ((record->flags >> 16) & 1U)
                ++record->runtime.endFrames;
            if (record->runtime.actionFrames >= 30) {
                acknowledgement[0] = record->name;
                acknowledgement[1] = static_cast<std::uint32_t>(-10);
            } else if (record->runtime.startFrames >= 30) {
                acknowledgement[0] = record->name;
                acknowledgement[1] = static_cast<std::uint32_t>(-1);
            } else if (record->runtime.endFrames >= 30) {
                acknowledgement[0] = record->name;
                acknowledgement[1] = static_cast<std::uint32_t>(-2);
            } else if (record->runtime.followFrames >= 30) {
                acknowledgement[0] = record->name;
                acknowledgement[1] = static_cast<std::uint32_t>(-8);
            } else
                return;
        }
        do {
            if (acknowledgement[0] != record->name)
                return;
            switch (static_cast<std::int32_t>(acknowledgement[1])) {
            case -1:
                record->flags &= ~0x4000U;
                record->runtime.startFrames = 0;
                break;
            case -2: {
                record->flags &= ~0x10000U;
                record->runtime.endFrames = 0;
                const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                mtwkUpdateWithPosition(charFindMotionwork(control->ryo), 0, 0);
                EndScenarioPlayerPresentation(3);
                break;
            }
            case -8:
                g_scenarioEventContext->flags &= ~0x8000U;
                record->flags &= ~0x8000U;
                record->runtime.followFrames = 0;
                if (record->localState.requestedRoot) {
                    record->state->flags |= 1U;
                    record->localState.requestedRoot = 6;
                }
                break;
            case -10:
                record->flags &= ~0x2000U;
                record->runtime.actionFrames = 0;
                if ((record->flags >> 7) & 1U) {
                    g_scenarioEventContext->flags |= 0x10U;
                    g_scenarioLineStartTicks = SCENARIO_FRAME_TICKS() - scenarioLinePauseTicks;
                }
                break;
            }
        } while (ReadScenarioAcknowledgement(acknowledgement) > 0);
    }
    if ((ctrlGetPadInputBuffer0(g_scenarioEventContext->padControlData)->pressedPriority & 0x200U) && CanSkipScenarioLine(record)) {
        const std::int32_t mode = GetS2DialogueMode();
        if (mode == 1 && ((g_scenarioEventContext->flags >> 11) & 1U) == 0) {
            StopConversationBuffers(0);
            RequestForcedScenarioSubtitles(reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask);
            if (record->dialogue.dialoguePreparation) {
                const char *const text =
                    reinterpret_cast<const char *>(record->source) + *reinterpret_cast<const std::int32_t *>(record->dialogue.dialoguePreparation);
                shenPrepareDialogue(g_scenarioEventContext->scenarioTask, mode, text, record->dialogue.playerSpeaking);
                g_scenarioEventContext->flags |= 0x10000U;
            }
            g_scenarioEventContext->flags |= 0x800U;
        } else {
            StopConversationBuffers(0);
            scenarioLineTimeOffset += SCENARIO_CUE_CURSOR->triggerTime - (SCENARIO_LINE_CLOCK() + scenarioLineTimeOffset);
            goto lineEnded;
        }
    } else if (scenarioLineHoldFrames > 0)
        --scenarioLineHoldFrames;
    if (record->localState.requestedRoot == 4 && static_cast<std::uint16_t>(record->dialogueFlagId) != 0xffffU) {
        record->flags &= ~0x800U;
        if ((record->flags >> 7) & 1U) {
            StopConversationBuffers(0);
            scenarioLineTimeOffset += SCENARIO_CUE_CURSOR->triggerTime - (SCENARIO_LINE_CLOCK() + scenarioLineTimeOffset);
            if ((g_scenarioEventContext->flags >> 16) & 1U) {
                scenDestroySubtitles(g_scenarioEventContext->scenarioTask);
                g_scenarioEventContext->flags &= ~0x10000U;
            }
            goto blockEnded;
        }
    }
    if (((record->flags >> 7) & 1U) == 0)
        goto stepProgram;
    if ((record->flags >> 10) & 1U) {
        record->flags &= ~0x400U;
        for (;;) {
            if (SCENARIO_CUE_CURSOR->payload) {
                bool requested = false;
                if ((record->flags >> 13) & 1U)
                    return;
                SetScenarioCurrentSpeaker(record);
                if (g_freeTalkVisibleCharacterCount > 2 && !g_scenarioSuppressNpcFacing) {
                    if (!g_scenarioFacingCommandsIssued && !g_scenarioLookActive) {
                        std::int32_t count = g_freeTalkVisibleCharacterCount;
                        if (count > 4)
                            count = 4;
                        std::int32_t listener = 0, index = 0, speaker = 0;
                        for (; index < count; ++index) {
                            if (g_freeTalkVisibleCharacters[index] == SCENARIO_CUE_CURSOR->speaker)
                                speaker = index;
                            if (g_freeTalkVisibleCharacters[index] == SCENARIO_CUE_CURSOR->retained0c)
                                listener = index;
                        }
                        if (speaker != listener) {
                            if (speaker) {
                                if (speaker < listener)
                                    --listener;
                                QueueScenarioRecordAction(record, (speaker - 1) * 3 + 41 + listener);
                                QueueScenarioRecordAction(record, speaker + 139);
                            } else
                                QueueScenarioRecordAction(record, listener + 139);
                            for (index = 1; index < count; ++index)
                                if (index != speaker) {
                                    listener = speaker;
                                    if (listener > index)
                                        --listener;
                                    QueueScenarioRecordAction(record, (index - 1) * 3 + 41 + listener);
                                }
                            requested = true;
                        }
                    }
                } else {
                    if (!g_scenarioSuppressPlayerFacing && (!g_scenarioPlayerFacingIssued || (!g_scenarioFacingCommandsIssued && !g_scenarioLookActive))) {
                        QueueScenarioRecordAction(record, 140);
                        requested = true;
                    }
                    if (!g_scenarioSuppressNpcFacing && (!g_scenarioNpcFacingIssued || (!g_scenarioFacingCommandsIssued && !g_scenarioLookActive))) {
                        QueueScenarioRecordAction(record, 40);
                        requested = true;
                    }
                }
                if (requested || g_scenarioFaceForwardMode) {
                    if (requested)
                        g_scenarioFacingCommandsIssued = 1;
                    if (g_scenarioFaceForwardMode)
                        QueueScenarioRecordAction(record, 16);
                    g_scenarioEventContext->flags &= ~0x10U;
                    record->flags |= 0x2000U;
                    record->runtime.actionFrames = 0;
                    scenarioLinePauseTicks = SCENARIO_FRAME_TICKS() - g_scenarioLineStartTicks;
                    return;
                }
                const std::byte *const payload = static_cast<const std::byte *>(SCENARIO_CUE_CURSOR->payload);
                record->dialogue.firstLine = payload;
                record->dialogue.dialoguePreparation = payload;
                const char *const source = reinterpret_cast<const char *>(record->source);
                const char *const text = source + *reinterpret_cast<const std::int32_t *>(payload);
                if (((record->flags >> 8) & 1U) == 0) {
                    g_scenarioCurrentDialogue = 0;
                    const std::int32_t audioOffset = reinterpret_cast<const std::int32_t *>(payload)[1];
                    if (audioOffset) {
                        g_scenarioCurrentDialogue = PlaySpeakerDialogue(source + audioOffset, SCENARIO_CUE_CURSOR->speaker);
                        if (g_scenarioCurrentDialogue && record->commandState && !ScenarioCharacterIsRyo(SCENARIO_CUE_CURSOR->speaker))
                            SetScenarioDialoguePitch(g_scenarioCurrentDialogue, static_cast<std::int8_t>(record->commandState));
                    }
                    record->dialogue.playerSpeaking = ScenarioCharacterIsRyo(SCENARIO_CUE_CURSOR->speaker) != 0;
                    if (!g_scenarioCurrentDialogue) {
                        shenPrepareDialogue(g_scenarioEventContext->scenarioTask, 1, text, record->dialogue.playerSpeaking);
                        g_scenarioEventContext->flags |= 0x10000U;
                    }
                    if (((g_scenarioEventContext->flags >> 3) & 1U) == 0) {
                        g_scenarioEventContext->flags |= 8U;
                        g_scenarioLineStartTicks = SCENARIO_FRAME_TICKS();
                        if ((g_scenarioEventContext->flags >> 2) & 1U) {
                            g_scenarioEventContext->flags |= 0x10U;
                            if (srchCheckFlags(6))
                                srchCheckFlags(2);
                            ActivateConversationCameraStream(g_scenarioCameraRecords);
                        }
                    }
                    if (!g_scenarioCurrentDialogue && SCENARIO_CUE_CURSOR->speaker) {
                        Character *const character = ResolveScenarioTalkCharacter(SCENARIO_CUE_CURSOR->speaker);
                        if (character) {
                            BuildScenarioLipSequence(record);
                            AssignFacialClip(
                                character, reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(g_scenarioEventContext) + 0x38));
                        }
                    }
                }
            } else {
                if (!SCENARIO_CUE_CURSOR->speaker)
                    return;
                if (SCENARIO_CUE_CURSOR->speaker == 0xb0U) {
                    g_scenarioSubtitleHoldFrames = -1;
                    if (RequestScenarioNpcSceneRoute(record->name, SCENARIO_CUE_CURSOR->retained0c)) {
                        ResumeNpcRouteAfterSceneChange(record->name);
                        SetScenarioFollowCharacter(record->name);
                    }
                } else if (SCENARIO_CUE_CURSOR->speaker == 0xb1U)
                    record->commandState = static_cast<std::uint8_t>(SCENARIO_CUE_CURSOR->retained0c);
                else if ((SCENARIO_CUE_CURSOR->speaker & 0xf00000U) == 0xa00000U)
                    ApplyScenarioFaceLook(SCENARIO_CUE_CURSOR->speaker);
                else if ((SCENARIO_CUE_CURSOR->speaker & 0xf000U) == 0x3000U) {
                    QueueScenarioRecordAction(record, SCENARIO_CUE_CURSOR->speaker & 0xfffU);
                    g_scenarioEventContext->flags &= ~0x10U;
                    record->flags |= 0x2000U;
                    record->runtime.actionFrames = 0;
                    scenarioLinePauseTicks = SCENARIO_FRAME_TICKS() - g_scenarioLineStartTicks;
                } else if ((SCENARIO_CUE_CURSOR->speaker & 0xf000U) == 0x6000U)
                    ClearOrSetShenmueFlags(SCENARIO_CUE_CURSOR->speaker & 0xfffU, 1);
                else if ((SCENARIO_CUE_CURSOR->speaker & 0xf000U) == 0x7000U)
                    ClearOrSetShenmueFlags(SCENARIO_CUE_CURSOR->speaker & 0xfffU, 0);
                else if (SCENARIO_CUE_CURSOR->speaker == 0xfc00U) {
                    g_scenarioEventContext->flags &= ~0x10U;
                    record->flags |= 0x2000U;
                    record->runtime.actionFrames = 0;
                    scenarioLinePauseTicks = SCENARIO_FRAME_TICKS() - g_scenarioLineStartTicks;
                    ++SCENARIO_CUE_CURSOR;
                    return;
                }
            }
            ++SCENARIO_CUE_CURSOR;
            if (SCENARIO_LINE_CLOCK() + scenarioLineTimeOffset < SCENARIO_CUE_CURSOR->triggerTime)
                break;
        }
        g_scenarioFacingCommandsIssued = 0;
        return;
    }
    if (SCENARIO_LINE_CLOCK() + scenarioLineTimeOffset < SCENARIO_CUE_CURSOR->triggerTime)
        return;
    if (g_scenarioCurrentDialogue && cvsHasValidConverseData(g_scenarioCurrentDialogue))
        StopConversationBuffers(g_scenarioCurrentDialogue);
lineEnded:
    if ((g_scenarioEventContext->flags >> 11) & 1U) {
        g_scenarioEventContext->flags &= ~0x800U;
        record->dialogue.dialoguePreparation = 0;
    }
    if ((g_scenarioEventContext->flags >> 16) & 1U) {
        scenDestroySubtitles(g_scenarioEventContext->scenarioTask);
        g_scenarioEventContext->flags &= ~0x10000U;
    }
    if (SCENARIO_CUE_CURSOR->speaker) {
        record->flags |= 0x400U;
        scenarioLineHoldFrames = 15;
        return;
    }
blockEnded:
    record->flags &= ~0x80U;
    record->flags &= ~0x80000U;
    if ((g_scenarioEventContext->flags >> 3) & 1U) {
        if ((g_scenarioEventContext->flags >> 2) & 1U)
            EndConversationCameraStream();
        g_scenarioEventContext->flags &= ~8U;
        g_scenarioEventContext->flags &= ~0x10U;
    }
stepProgram:
    if ((record->state->flags & 4U) || record->localState.requestedRoot == 6) {
        if (record->localState.requestedRoot == 6)
            record->localState.requestedRoot = 0;
        else
            record->localState.retainedRootState = 0;
        if (record->state->flags & 1U) {
            record->state->resumeOffsets[0] = 0;
            record->state->flags &= 0xfffeU;
        }
        const std::byte *const section = AdvanceScenarioCharacterCommandStream(record);
        if (section) {
            if (section - record->script != record->state->resumeOffsets[0]) {
                record->runtime.cursor = record->script;
                record->state->resumeOffsets[3] = 0;
                record->state->resumeOffsets[0] = section - record->script;
                record->state->resumeOffsets[2] = 0;
                record->state->resumeOffsets[1] = 0;
                SetScenarioTalkCompletedCount(record, 0);
                SetScenarioTalkActivationCount(record, 0);
                record->executionState = 0;
                record->flags &= ~0x100000U;
                scenStepRecordProgram(record);
                return;
            }
        } else {
            record->executionState = 0;
            record->state->resumeOffsets[0] = 0;
            SetScenarioTalkCompletedCount(record, 0);
            SetScenarioTalkActivationCount(record, 0);
        }
    }
    if (record->executionState == 3 && !(record->state->flags & 4U)) {
        if (record->name == CHARACTER_SHENHUA) {
            record->state->flags |= 4U;
            record->state->flags |= 1U;
        }
        return;
    }
    if (record->executionState == 5 && ((record->flags >> 1) & 1U) == 0) {
        record->flags &= ~0x100U;
        if (((record->flags >> 21) & 1U) == 0 || ((g_scenarioEventContext->flags >> 15) & 1U) == 0) {
            record->flags |= 0x200000U;
            scenarioPromptFrames = 0;
        }
        if (scenarioPromptFrames++ < 180 && record->localState.requestedRoot != 2 && record->localState.requestedRoot != 4 &&
            !btnCheck(shen_name(SHEN_NAME4('B', 'T', 'F', 'T'))) && !btnCheck(shen_name(SHEN_NAME4('B', 'T', 'M', 'T')))) {
            if (record->name) {
                Vector3 position;
                charaGetMotWorldPos(ctrlGetScenarioChara(record->name), 5, &position);
                const std::uint32_t ready = CanActivateScenarioCandidate() ? 4U : 0U;
                g_scenarioTalkPromptResumed = (record->flags >> 20) & 1U;
                scenarioHardBusy = 1;
                RegisterButtonMediation(shen_name(SHEN_NAME4('B', 'T', 'F', 'T')), &position, ready | 0x10001U);
                if (!record->localState.requestedRoot && (((g_scenarioEventContext->flags >> 15) & 1U) == 0 || (record->state->flags & 2U)) &&
                    IsScenarioFollowPromptAvailable()) {
                    RegisterButtonMediation(shen_name(SHEN_NAME4('B', 'T', 'M', 'T')), &position, ready | 0x80001U);
                    if (static_cast<std::int32_t>(record->localState.retainedRootState) > 0)
                        g_scenarioFollowPromptShown = 1;
                }
            }
            if (((record->flags >> 12) & 1U) == 0)
                return;
            if (((g_scenarioEventContext->flags >> 12) & 1U) || ((record->flags >> 20) & 1U)) {
                if (GetParentMenu() == 7) {
                    g_scenarioEventContext->flags &= ~0x1000U;
                    record->flags |= 0x200000U;
                    record->flags |= 0x400000U;
                    EndScenarioTalkSession(record);
                }
            }
            if ((record->flags >> 22) & 1U)
                return;
            const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(GetMapPadControlEntry());
            if (input && !(input->pressedPriority & 0x200U) && !(input->held & 0xf0c0U) &&
                *reinterpret_cast<const std::uint16_t *>(&input->leftStickX) == 0x8080U)
                return;
            if (GetParentMenu() == 7) {
                EndScenarioTalkSession(record);
                record->flags |= 0x400000U;
            }
            return;
        }
        if (btnCheck(shen_name(SHEN_NAME4('B', 'T', 'M', 'T')))) {
            if (!record->localState.defaultRoot) {
                ExecuteAicaCommand(0x800abU, 0, 0);
                return;
            }
            record->localState.requestedRoot = 1;
        }
        record->flags &= ~0xc000000U;
        if (UpdateSelectorMemo())
            return;
        if (!IsPlayerControlEnabled() && ((g_scenarioEventContext->flags >> 9) & 1U) == 0)
            return;
        BeginScenarioTalkSession(record);
        if ((record->state->flags & 4U) || static_cast<std::int32_t>(record->localState.retainedRootState) < 0) {
            const std::byte *entry = record->script + record->state->resumeOffsets[1];
            std::uint32_t offset = *entry++;
            offset = (offset << 8) + *entry++;
            offset = (offset << 8) + *entry++;
            if (offset & 0x800000U)
                offset |= 0xff000000U;
            if (offset) {
                record->flags |= 0x200U;
                record->runtime.savedCursor = entry;
                record->runtime.cursor = entry + static_cast<std::int32_t>(offset);
            } else {
                record->runtime.savedCursor = 0;
                record->runtime.cursor = entry;
            }
            record->state->flags &= 0xfffbU;
            if (static_cast<std::int32_t>(record->localState.retainedRootState) < 0)
                record->localState.retainedRootState = 1;
        } else
            record->runtime.cursor += 3;
        switch (record->localState.requestedRoot) {
        case 1:
            if (record->localState.alternateRoot) {
                record->runtime.cursor = record->localState.alternateRoot;
                record->localState.requestedRoot = 3;
            } else {
                record->runtime.cursor = record->localState.defaultRoot;
                record->localState.requestedRoot = 2;
            }
            if (!record->localState.retainedRootState)
                record->localState.retainedRootState = 0xffffffffU;
            record->flags &= ~0x200U;
            record->runtime.savedCursor = 0;
            record->runtime.pendingCursor = 0;
            break;
        case 2:
            record->localState.requestedRoot = 5;
            break;
        }
    }
    scenarioLineTimeOffset = 0.0f;
    scenarioLineHoldFrames = 0;
    reinterpret_cast<ScenarioEventContextStorage *>(g_scenarioEventContext)->randomSelectionCount = 0;
    scenStepRecordProgram(record);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static const std::byte *scenSelectRecordStart(ScenarioFreeTalkCharacter *record) {
    const std::byte *cursor;
    if (((record->flags >> 1) & 1) || record->executionState == 6)
        cursor = record->runtime.cursor;
    else if (((g_scenarioEventContext->flags >> 6) & 1) && record->state->resumeOffsets[0]) {
        g_scenarioEventContext->flags &= ~0x40U;
        cursor = record->script + record->state->resumeOffsets[0];
    } else if (!record->executionState) {
        if ((g_scenarioEventContext->flags & 1U) && record->requestedSection != -1)
            cursor = FindScenarioIndexedSection(record, record->requestedSection);
        else
            cursor = record->script;
    } else if (record->executionState > 3)
        cursor = record->runtime.cursor;
    else if (record->executionState == 1) {
        if (record->state->resumeOffsets[2] > record->state->resumeOffsets[3])
            cursor = record->script + record->state->resumeOffsets[2] - 2;
        else if (record->state->resumeOffsets[1] > record->state->resumeOffsets[3])
            cursor = record->script + record->state->resumeOffsets[1] - 1;
        else
            cursor = record->script + record->state->resumeOffsets[3];
    } else
        cursor = record->script;
    if (static_cast<std::uint16_t>(record->dialogueFlagId) != 0xffff && (record->flags & 0x3cU)) {
        if ((record->flags >> 3) & 1) {
            if (record->localState.requestedRoot == 4)
                cursor = record->localState.alternateRoot + 11;
            ResolveScenarioChoiceSlot(record);
            cursor += static_cast<std::uint16_t>(record->dialogueFlagId) * 2;
        }
        if ((record->flags >> 4) & 1) {
            ResolveScenarioChoiceSlot(record);
            cursor = scenarioChoiceTargets[static_cast<std::uint16_t>(record->dialogueFlagId)];
            if (!cursor)
                cursor = scenarioChoiceTargets[0];
        }
        if ((record->flags >> 2) & 1) {
            if (record->localState.requestedRoot == 4)
                cursor = record->localState.alternateRoot + 2;
            if (record->dialogueFlagId)
                record->dialogueFlagId = 1;
            cursor += static_cast<std::uint16_t>(record->dialogueFlagId) * 2;
        }
        if (((record->flags >> 5) & 1) && record->dialogueFlagId)
            cursor += 2;
        if ((record->flags & 0x18U) && ((record->flags >> 28) & 1)) {
            CloseMoneyIcon();
            record->flags &= ~0x10000000U;
        }
        record->flags &= ~8U;
        record->flags &= ~0x10U;
        record->flags &= ~4U;
        record->flags &= ~0x20U;
        if (record->localState.requestedRoot == 4)
            record->localState.requestedRoot = 5;
        record->flags &= ~0xc000000U;
    }
    return cursor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static const std::byte *scenExecuteLineOpcode(ScenarioFreeTalkCharacter *record, const std::byte *cursor, std::int32_t *stop) {
    const std::uint8_t opcode = *cursor++;
    std::uint32_t value = (opcode & 15) << 8;
    std::int32_t index;
    value += *cursor++;
    switch (opcode & 0xf0) {
    case 0x10:
        if (record->flags & 0x202U)
            break;
        if (!record->runtime.returnCursor) {
            if (cursor == record->state->resumeOffsets[0] + record->script) {
                if (record->state->resumeOffsets[3])
                    cursor = record->state->resumeOffsets[3] + record->script;
                if (cursor < record->state->resumeOffsets[2] + record->script)
                    cursor = record->state->resumeOffsets[2] + record->script - 2;
                if (cursor < record->state->resumeOffsets[1] + record->script) {
                    cursor = record->state->resumeOffsets[1] + record->script;
                    record->executionState = 5;
                    *stop = 1;
                    break;
                }
            } else {
                record->state->resumeOffsets[3] = reinterpret_cast<std::uintptr_t>(cursor) - reinterpret_cast<std::uintptr_t>(record->script);
                record->state->resumeOffsets[2] = 0;
                record->flags &= ~0x100000U;
            }
        }
        record->state->resumeOffsets[1] = 0;
        break;
    case 0x30: {
        index = 0;
        while ((cursor[index] & 0xf0) == 0x30)
            index += 2;
        if ((cursor[index] & 0xf0) != 0x20 && (cursor[index] != 0xe0 || (*(cursor + index + 4) & 0xf0) != 0x20)) {
            QueueScenarioRecordAction(record, value);
            while ((*cursor & 0xf0) == 0x30) {
                value = (*cursor++ & 15) << 8;
                QueueScenarioRecordAction(record, value + *cursor++);
            }
            record->flags |= 0x2000U;
            record->runtime.actionFrames = 0;
            g_scenarioEventContext->flags &= ~0x10U;
            record->executionState = 7;
            *stop = 1;
            break;
        }
    }
    case 0x20:
        record->dialogue.firstLine = 0;
        cursor = scenBuildDialogueCues(record, cursor - 2);
        for (index = 1; index < g_freeTalkVisibleCharacterCount; ++index) {
            if (record->name == g_freeTalkVisibleCharacters[index]) {
                RecordScenarioTalkOccurrence(record->savedStateIndex);
            } else {
                ScenarioFreeTalkCharacter *const participant = FindScenarioFreeTalkCharacter(g_freeTalkVisibleCharacters[index]);
                if (participant)
                    RecordScenarioTalkOccurrence(participant->savedStateIndex);
            }
        }
        record->flags |= 0x1080U;
        record->flags |= 0x400U;
        record->executionState = 4;
        record->dialogue.dialoguePreparation = 0;
        g_scenarioEventContext->flags &= ~0x800U;
        scenarioLineHoldFrames = 15;
        *stop = 1;
        break;
    case 0x40:
        if (g_scntExpressionResult)
            break;
        if (value & 0x800U)
            value |= 0xfffff000U;
        cursor += value;
        break;
    case 0x50:
        if (value & 0x800U)
            value |= 0xfffff000U;
        cursor += value;
        if (VisitScntBranch(cursor))
            *stop = 1;
        break;
    case 0x60:
        ClearOrSetShenmueFlags(value, 1);
        break;
    case 0x70:
        ClearOrSetShenmueFlags(value, 0);
        break;
    case 0x00:
    default:
        break;
    }
    return cursor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static const std::byte *scenExecuteStreamOpcode(ScenarioFreeTalkCharacter *record, const std::byte *cursor, std::int32_t *stop) {
    std::uint8_t opcode;
    std::int32_t low, operand;
    opcode = *cursor++;
    low = opcode & 15;
    switch (opcode & 0xf0) {
    case 0x80:
        operand = *cursor++ << 8;
        operand += *cursor++;
        operand += low << 16;
        if (operand & 0x80000)
            operand |= 0xfff00000;
        record->runtime.returnCursor = cursor;
        cursor += operand;
        break;
    case 0x90:
        ++cursor;
        if (!(record->flags & 0x202U) && !record->localState.requestedRoot) {
            record->flags |= 0x100000U;
            record->state->resumeOffsets[2] =
                static_cast<std::uint16_t>(reinterpret_cast<std::int32_t>(cursor) - reinterpret_cast<std::int32_t>(record->script));
            record->runtime.pendingCursor = 0;
        }
        break;
    case 0xa0:
        operand = *cursor++;
        ApplyScenarioFaceLook((opcode << 16) | (operand << 8) | *cursor++);
        break;
    case 0xb0:
        switch (opcode & 15) {
        case 0:
            g_scenarioSubtitleHoldFrames = -1;
            if (RequestScenarioNpcSceneRoute(record->name, (cursor[3] << 24) | (cursor[2] << 16) | (cursor[1] << 8) | cursor[0])) {
                ResumeNpcRouteAfterSceneChange(record->name);
                SetScenarioFollowCharacter(record->name);
            }
            cursor += 4;
            break;
        case 1:
            record->commandState = *cursor++;
            break;
        }
        break;
    case 0xc0:
        switch (opcode & 15) {
        case 1:
            ScenarioSpeakerLetter() = *cursor++;
            break;
        case 2:
            ScenarioListenerLetter() = *cursor++;
            break;
        case 3:
            reinterpret_cast<std::uint8_t *>(&record->retained08)[0] = *cursor++;
            break;
        case 4:
            reinterpret_cast<std::uint8_t *>(&record->retained08)[1] = *cursor++;
            break;
        case 5:
            reinterpret_cast<std::uint8_t *>(&record->retained08)[2] = *cursor++;
            break;
        case 6:
            reinterpret_cast<std::uint8_t *>(&record->retained08)[3] = *cursor++;
            break;
        case 7:
            record->selection = static_cast<std::int8_t>(*cursor++);
            break;
        default:
            ++cursor;
            break;
        }
        break;
    case 0xd0:
        operand = *cursor++ << 8;
        operand += *cursor++;
        operand += low << 16;
        if (operand & 0x80000)
            operand |= 0xfff00000;
        cursor += operand;
        if (VisitScntBranch(cursor))
            *stop = 1;
        break;
    }
    return cursor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static bool SettleScenarioPromptFacing(ScenarioFreeTalkCharacter *record) {
    bool requested = false;
    if (!g_scenarioSuppressPlayerFacing && !g_scenarioPlayerFacingIssued) {
        QueueScenarioRecordAction(record, 140);
        requested = true;
    }
    if (!g_scenarioSuppressNpcFacing && !g_scenarioNpcFacingIssued) {
        QueueScenarioRecordAction(record, 40);
        requested = true;
    }
    if (g_scenarioFaceForwardMode) {
        QueueScenarioRecordAction(record, 16);
        requested = true;
    }
    if (requested) {
        record->flags |= 0x2000U;
        record->runtime.actionFrames = 0;
        g_scenarioEventContext->flags &= ~0x10U;
        record->executionState = 7;
    }
    return requested;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const std::byte *scenExecuteChoiceOpcode(ScenarioFreeTalkCharacter *record, const std::byte *cursor, std::int32_t *stop) {
    std::int16_t choices[4];
    std::int32_t i, j, k, n;
    const std::int8_t opcode = *cursor++;
    n = *cursor++;
    switch (static_cast<std::uint8_t>(opcode)) {
    case 0xe0:
        i = static_cast<std::int32_t>(static_cast<float>(n) * dc::sdk::RandomFloat());
        if (SCENARIO_EVENT_STORAGE->randomSelectionCount < 3 &&
            SCENARIO_EVENT_STORAGE->randomSelections[SCENARIO_EVENT_STORAGE->randomSelectionCount] == static_cast<std::uint32_t>(i)) {
            if (!i)
                i = n;
            --i;
        }
        if (SCENARIO_EVENT_STORAGE->randomSelectionCount < 3) {
            SCENARIO_EVENT_STORAGE->randomSelections[SCENARIO_EVENT_STORAGE->randomSelectionCount] = i;
            ++SCENARIO_EVENT_STORAGE->randomSelectionCount;
        }
        for (; i != 0; --i) {
            n = ((cursor[0] & 15) << 8) | cursor[1];
            cursor += n + 2;
        }
        break;
    case 0xe1:
        if (record->localState.requestedRoot == 4) {
            record->executionState = 6;
            *stop = 1;
            cursor -= 2;
            break;
        }
        if (SettleScenarioPromptFacing(record)) {
            *stop = 1;
            cursor -= 2;
            break;
        }
        ++cursor;
        record->promptTimeout = 10800;
        for (i = 0; i < 4; ++i) {
            scenarioChoiceIds[i] = (cursor[0] << 8) | cursor[1];
            cursor += 2;
        }
        choices[0] = scenarioChoiceIds[0];
        choices[1] = scenarioChoiceIds[1];
        choices[2] = scenarioChoiceIds[2];
        choices[3] = scenarioChoiceIds[3];
        SetupControlSpeakAndUI_Internal(choices, static_cast<std::int16_t>(record->promptTimeout) ? static_cast<std::int16_t>(record->promptTimeout) : 0x7fff,
            0xffff, static_cast<std::int16_t>(record->promptTimeout) ? 2 : 0, record->localState.requestedRoot ? 3 : 1);
        if (record->localState.requestedRoot || ((record->flags >> 28) & 1)) {
            SetPlayerCashOrMoney(0, 1);
            KeepMoneyIconOpen();
            record->flags |= 0x10000000U;
        }
        record->flags |= 0x48U;
        record->executionState = 6;
        *stop = 1;
        if (plymIsAuthorTargetActive())
            ResumeConversationCameraWrapper();
        if (record->localState.requestedRoot == 2)
            record->localState.requestedRoot = 5;
        if (record->localState.requestedRoot == 3) {
            cursor = record->localState.defaultRoot;
            record->localState.requestedRoot = 4;
        }
        break;
    case 0xe2:
        if (record->localState.requestedRoot == 4) {
            record->executionState = 6;
            *stop = 1;
            cursor -= 2;
            break;
        }
        if (SettleScenarioPromptFacing(record)) {
            *stop = 1;
            cursor -= 2;
            break;
        }
        choices[0] = -1;
        choices[1] = 0;
        choices[2] = 1;
        choices[3] = -1;
        SetupControlSpeakAndUI_Internal(choices, 0x7fff, 0xffff, 3, 1);
        record->flags |= 0x44U;
        record->executionState = 6;
        *stop = 1;
        if (plymIsAuthorTargetActive())
            ResumeConversationCameraWrapper();
        if (record->localState.requestedRoot == 2)
            record->localState.requestedRoot = 5;
        if (record->localState.requestedRoot == 3) {
            cursor = record->localState.defaultRoot;
            record->localState.requestedRoot = 4;
        }
        break;
    case 0xe3:
        break;
    case 0xe5: {
        if (record->localState.requestedRoot == 4) {
            record->executionState = 6;
            *stop = 1;
            cursor -= 2;
            break;
        }
        if (SettleScenarioPromptFacing(record)) {
            *stop = 1;
            cursor -= 2;
            break;
        }
        record->promptTimeout = 10800;
        for (i = 0; i < 100; ++i)
            scenarioLinkedChoices[i].target = 0;
        for (i = 0; i < 5; ++i) {
            scenarioChoiceIds[i] = 0xffff;
            scenarioChoiceRanks[i] = -1;
            scenarioChoiceGroups[i] = 0;
        }
        for (i = 0; i < 6; ++i)
            scenarioChoiceTargets[i] = 0;
        const std::byte *entry = cursor;
        while (i = *entry++, i = i << 8 | *entry++, !(i & 0x8000)) {
            const std::int32_t high = (*entry++ & 15) << 8;
            n = high | *entry++;
            if (n & 0x800)
                n |= ~0xfff;
            const std::byte *data = entry;
            entry += n;
            if (i) {
                const std::int32_t rank = static_cast<std::int8_t>(*data++);
                if (rank < 0) {
                    scenarioChoiceIds[0] = i;
                    scenarioChoiceRanks[0] = -1;
                    scenarioChoiceGroups[0] = 0;
                    scenarioChoiceTargets[1] = data;
                } else {
                    n = *data++;
                    const std::int32_t priority = static_cast<std::int8_t>(*data++);
                    if (*data++ == 0xf0) {
                        data = DecodeScntExpression(data, record);
                        if (g_scntExpressionResult && (!scenarioLinkedChoices[n].target || scenarioLinkedChoices[n].priority < priority)) {
                            scenarioLinkedChoices[n].id = i;
                            scenarioLinkedChoices[n].rank = rank;
                            scenarioLinkedChoices[n].priority = priority;
                            scenarioLinkedChoices[n].target = data;
                        }
                    }
                }
            } else
                scenarioChoiceTargets[0] = data;
        }
        if (!scenarioChoiceTargets[0])
            scenarioChoiceTargets[0] = entry;
        for (i = 0; i < 100; ++i) {
            if (!scenarioLinkedChoices[i].target)
                continue;
            for (j = 0; j < 4; ++j) {
                if (scenarioChoiceRanks[j] < scenarioLinkedChoices[i].rank) {
                    for (k = 3; k > j; --k) {
                        scenarioChoiceIds[k] = scenarioChoiceIds[k - 1];
                        scenarioChoiceRanks[k] = scenarioChoiceRanks[k - 1];
                        scenarioChoiceGroups[k] = scenarioChoiceGroups[k - 1];
                        scenarioChoiceTargets[k + 1] = scenarioChoiceTargets[k];
                    }
                    scenarioChoiceIds[j] = scenarioLinkedChoices[i].id;
                    scenarioChoiceRanks[j] = scenarioLinkedChoices[i].rank;
                    scenarioChoiceGroups[j] = i;
                    scenarioChoiceTargets[j + 1] = scenarioLinkedChoices[i].target;
                    break;
                }
            }
        }
        if (scenarioChoiceIds[1] != 0xffff && scenarioChoiceIds[2] == 0xffff) {
            scenarioChoiceIds[2] = scenarioChoiceIds[1];
            scenarioChoiceRanks[2] = scenarioChoiceRanks[1];
            scenarioChoiceGroups[2] = scenarioChoiceGroups[1];
            scenarioChoiceTargets[3] = scenarioChoiceTargets[2];
            scenarioChoiceIds[1] = scenarioChoiceIds[0];
            scenarioChoiceRanks[1] = scenarioChoiceRanks[0];
            scenarioChoiceGroups[1] = scenarioChoiceGroups[0];
            scenarioChoiceTargets[2] = scenarioChoiceTargets[1];
            scenarioChoiceIds[0] = scenarioChoiceIds[3];
            scenarioChoiceRanks[0] = scenarioChoiceRanks[3];
            scenarioChoiceGroups[0] = scenarioChoiceGroups[3];
            scenarioChoiceTargets[1] = scenarioChoiceTargets[4];
        }
        j = 0;
        for (i = 0, k = -1; i < 4; ++i)
            if (scenarioChoiceIds[i] != 0xffff) {
                if (scenarioChoiceGroups[i] == 98)
                    k = i;
                ++j;
            }
        if (k >= 0) {
            scenarioChoiceIds[4] = scenarioChoiceIds[k];
            scenarioChoiceRanks[4] = scenarioChoiceRanks[k];
            scenarioChoiceGroups[4] = scenarioChoiceGroups[k];
            scenarioChoiceTargets[5] = scenarioChoiceTargets[k + 1];
            for (i = k; i > 0; --i) {
                scenarioChoiceIds[i] = scenarioChoiceIds[i - 1];
                scenarioChoiceRanks[i] = scenarioChoiceRanks[i - 1];
                scenarioChoiceGroups[i] = scenarioChoiceGroups[i - 1];
                scenarioChoiceTargets[i + 1] = scenarioChoiceTargets[i];
            }
            scenarioChoiceIds[i] = 0xffff;
            scenarioChoiceRanks[i] = -1;
            scenarioChoiceGroups[i] = 0;
            scenarioChoiceTargets[i + 1] = 0;
            j = j == 2;
            scenarioChoiceIds[j] = scenarioChoiceIds[4];
            scenarioChoiceRanks[j] = scenarioChoiceRanks[4];
            scenarioChoiceGroups[j] = scenarioChoiceGroups[4];
            scenarioChoiceTargets[j + 1] = scenarioChoiceTargets[5];
        }
        j = 0;
        for (i = 0, k = -1; i < 4; ++i)
            if (scenarioChoiceIds[i] != 0xffff) {
                if (scenarioChoiceGroups[i] == 99)
                    k = i;
                ++j;
            }
        if (k >= 0) {
            scenarioChoiceIds[4] = scenarioChoiceIds[k];
            scenarioChoiceRanks[4] = scenarioChoiceRanks[k];
            scenarioChoiceGroups[4] = scenarioChoiceGroups[k];
            scenarioChoiceTargets[5] = scenarioChoiceTargets[k + 1];
            for (i = k; i < 3; ++i) {
                scenarioChoiceIds[i] = scenarioChoiceIds[i + 1];
                scenarioChoiceRanks[i] = scenarioChoiceRanks[i + 1];
                scenarioChoiceGroups[i] = scenarioChoiceGroups[i + 1];
                scenarioChoiceTargets[i + 1] = scenarioChoiceTargets[i + 2];
            }
            if (j == 3)
                for (i = 3; i > 0; --i) {
                    scenarioChoiceIds[i] = scenarioChoiceIds[i - 1];
                    scenarioChoiceRanks[i] = scenarioChoiceRanks[i - 1];
                    scenarioChoiceGroups[i] = scenarioChoiceGroups[i - 1];
                    scenarioChoiceTargets[i + 1] = scenarioChoiceTargets[i];
                }
            scenarioChoiceIds[i] = 0xffff;
            scenarioChoiceRanks[i] = -1;
            scenarioChoiceGroups[i] = 0;
            scenarioChoiceTargets[i + 1] = 0;
            if (j > 3)
                j = 3;
            else if (j < 2)
                j = 0;
            scenarioChoiceIds[j] = scenarioChoiceIds[4];
            scenarioChoiceRanks[j] = scenarioChoiceRanks[4];
            scenarioChoiceGroups[j] = scenarioChoiceGroups[4];
            scenarioChoiceTargets[j + 1] = scenarioChoiceTargets[5];
        }
        choices[0] = scenarioChoiceIds[0];
        choices[1] = scenarioChoiceIds[1];
        choices[2] = scenarioChoiceIds[2];
        choices[3] = scenarioChoiceIds[3];
        SetupControlSpeakAndUI_Internal(choices, static_cast<std::int16_t>(record->promptTimeout) ? static_cast<std::int16_t>(record->promptTimeout) : 0x7fff,
            0xffff, static_cast<std::int16_t>(record->promptTimeout) ? 2 : 0, record->localState.requestedRoot ? 3 : 1);
        if (record->localState.requestedRoot || ((record->flags >> 28) & 1)) {
            SetPlayerCashOrMoney(0, 1);
            KeepMoneyIconOpen();
            record->flags |= 0x10000000U;
        }
        record->flags |= 0x50U;
        record->executionState = 6;
        *stop = 1;
        if (plymIsAuthorTargetActive())
            ResumeConversationCameraWrapper();
        if (record->localState.requestedRoot == 2)
            record->localState.requestedRoot = 5;
        if (record->localState.requestedRoot == 3) {
            cursor = record->localState.defaultRoot;
            record->localState.requestedRoot = 4;
        }
        break;
    }
    case 0xe6:
        --cursor;
        if (record->localState.requestedRoot == 4)
            break;
        record->flags |= 0x20U;
        record->executionState = 6;
        scenarioPromptFrames = 0;
        CaptureScenarioTalkInput();
        *stop = 1;
        if (plymIsAuthorTargetActive())
            ResumeConversationCameraWrapper();
        if (record->localState.requestedRoot == 2)
            record->localState.requestedRoot = 5;
        break;
    default:
        cursor += n;
    }
    return cursor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
const std::byte *scenExecuteControlOpcode(ScenarioFreeTalkCharacter *record, const std::byte *cursor, std::int32_t *stop) {
    const std::uint32_t opcode = *cursor++;
    switch (opcode) {
    case 0xf0:
        cursor = DecodeScntExpression(cursor, record);
        break;
    case 0xf2:
        if ((cursor[3] & 0xf0U) == 0x90U) {
            cursor += 3;
            break;
        }
        if (!(record->flags & 0x202U) && !record->localState.requestedRoot)
            record->state->resumeOffsets[1] = reinterpret_cast<std::uint32_t>(cursor) - reinterpret_cast<std::uint32_t>(record->script);
        if (record->localState.requestedRoot == 4)
            record->state->resumeOffsets[1] = reinterpret_cast<std::uint32_t>(cursor) - reinterpret_cast<std::uint32_t>(record->script);
        record->executionState = 5;
        *stop = 1;
        break;
    case 0xf3:
    case 0xfa:
        if (CanActivateScenarioCandidate() && !(record->state->flags & 2U) &&
            (((g_scenarioEventContext->flags >> 9) & 1U) || opcode != 0xf3 ||
                (!EV_GetShenmueFlag(401) && !plymIsInterpolationActive() && ((record->flags >> 23) & 1U))))
            goto activate;
        record->flags |= 0x1000000U;
        record->executionState = 3;
        *stop = 1;
        cursor--;
        break;
    activate:
        BeginScenarioTalkSession(record);
        record->flags |= 0x80000U;
        if (record->name != CHARACTER_SHENHUA)
            record->state->flags |= 2U;
        SetScenarioTalkActivationCount(record, GetScenarioTalkActivationCount(record) + 1);
        break;
    case 0xf5: {
        std::int32_t offset = *cursor++;
        offset <<= 8;
        offset += *cursor++;
        offset <<= 8;
        offset += *cursor++;
        if (offset & 0x800000)
            offset |= 0xff000000;
        record->runtime.pendingCursor = cursor + offset;
        break;
    }
    case 0xf6: {
        std::int32_t complete = 1;
        if (((record->flags >> 11) & 1U) == 0) {
            record->flags |= 0x800U;
            complete = 0;
            record->flags &= ~0xc000000U;
            record->flags |= 0x8000000U;
        }
        if (record->state->resumeOffsets[2] > record->state->resumeOffsets[1]) {
            complete = 0;
            record->flags &= ~0xc000000U;
            record->flags |= 0x4000000U;
        }
        if (record->state->resumeOffsets[2] <= record->state->resumeOffsets[3]) {
            complete = 0;
            record->flags |= 0xc000000U;
        }
        if (complete) {
            QueueScenarioRecordAction(record, -3);
            record->executionState = 1;
            record->flags |= 0x100000U;
            SetScenarioTalkCompletedCount(record, GetScenarioTalkCompletedCount(record) + 1);
        } else {
            QueueScenarioRecordAction(record, -5);
            record->executionState = 3;
            cursor = record->script;
            record->runtime.cursor = cursor;
            record->state->resumeOffsets[2] = 0;
            record->state->resumeOffsets[1] = 0;
            record->flags &= ~0x100000U;
        }
        record->flags &= ~0x2000000U;
        record->flags &= ~0x100U;
        *stop = 1;
        break;
    }
    case 0xf7:
        QueueScenarioRecordAction(record, -4);
        record->executionState = 2;
        record->flags &= ~0x100U;
        *stop = 1;
        break;
    case 0xf8:
        if (record->state->resumeOffsets[2] > record->state->resumeOffsets[3] && ((record->flags >> 11) & 1U) &&
            record->state->resumeOffsets[2] < record->state->resumeOffsets[1]) {
            QueueScenarioRecordAction(record, -3);
            record->executionState = 1;
            record->flags |= 0x2000000U;
            record->flags |= 0x100000U;
            SetScenarioTalkCompletedCount(record, GetScenarioTalkCompletedCount(record) + 1);
        } else {
            QueueScenarioRecordAction(record, -5);
            record->executionState = 3;
            cursor = record->script;
            record->runtime.cursor = cursor;
            if (((record->flags >> 11) & 1U) == 0) {
                record->flags &= ~0xc000000U;
                record->flags |= 0x8000000U;
            }
            if (record->state->resumeOffsets[2])
                record->flags |= 0xc000000U;
            record->flags |= 0x800U;
            record->state->resumeOffsets[2] = 0;
            record->state->resumeOffsets[1] = 0;
            record->flags &= ~0x2000000U;
            record->flags &= ~0x100000U;
        }
        record->flags &= ~0x100U;
        *stop = 1;
        break;
    case 0xf9:
        if ((record->flags >> 1) & 1U) {
            g_scenarioEventContext->flags &= ~0x8000U;
            record->flags &= ~2U;
            cursor = record->runtime.savedCursor;
            record->runtime.savedCursor = 0;
            record->executionState = 5;
            record->flags &= ~0x100U;
            *stop = 1;
        } else if ((record->flags >> 9) & 1U) {
            record->flags &= ~0x200U;
            cursor = record->runtime.savedCursor;
            record->runtime.savedCursor = 0;
        } else if (record->runtime.returnCursor) {
            cursor = record->runtime.returnCursor;
            record->runtime.returnCursor = 0;
        } else {
            QueueScenarioRecordAction(record, -3);
            record->executionState = 1;
            record->flags &= ~0x100U;
            *stop = 1;
        }
        break;
    case 0xfc:
        g_scenarioEventContext->flags &= ~0x10U;
        record->flags |= 0x2000U;
        record->runtime.actionFrames = 0;
        record->executionState = 6;
        *stop = 1;
        break;
    case 0xfd:
        record->flags |= 0x1000U;
        record->dialogue.dialoguePreparation = 0;
        g_scenarioEventContext->flags &= ~0x800U;
        break;
    case 0xff:
        record->executionState = 3;
        record->flags &= ~0x100U;
        *stop = 1;
        break;
    }
    return cursor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void scenStepRecordProgram(ScenarioFreeTalkCharacter *record) {
    if (!record->name) {
        g_scenarioEventContext->activeCharacter = shen_name();
        g_scenarioEventContext->previousCharacter = 0;
        return;
    }
    const std::byte *cursor = scenSelectRecordStart(record);
    if (!cursor)
        return;
    ResetScntBranchHistory();
    std::int32_t stop = 0;
    while (!stop) {
        const std::uint8_t opcode = *cursor++;
        if (opcode < 0x80)
            cursor = scenExecuteLineOpcode(record, --cursor, &stop);
        else if (opcode < 0xe0)
            cursor = scenExecuteStreamOpcode(record, --cursor, &stop);
        else if (opcode < 0xf0)
            cursor = scenExecuteChoiceOpcode(record, --cursor, &stop);
        else
            cursor = scenExecuteControlOpcode(record, --cursor, &stop);
    }
    record->runtime.cursor = cursor;
    switch (record->executionState) {
    case 1:
    case 6:
    case 7:
    case 8:
        return;
    case 4:
        if (((record->flags >> 11) & 1U) == 0) {
            record->flags |= 0x800U;
            if (((record->flags >> 1) & 1U) == 0 && record->localState.requestedRoot != 4) {
                record->flags |= 0x4000U;
                record->runtime.startFrames = 0;
                QueueScenarioRecordAction(record, -1);
                RequestScenarioCameraRestore();
                CaptureScenarioTalkInput();
            }
            reinterpret_cast<std::uint32_t *>(g_scenarioEventContext)[3] = 0;
            reinterpret_cast<std::uint32_t *>(g_scenarioEventContext)[4] = 0;
        }
        return;
    case 5:
        if ((record->flags >> 11) & 1U) {
            record->flags = (record->flags & ~0x800U) | 0x10000U;
            record->runtime.endFrames = 0;
            QueueScenarioRecordAction(record, -2);
            ReleaseScenarioTalkInput();
        }
        return;
    default:
        if ((record->flags >> 11) & 1U) {
            record->flags = (record->flags & ~0x800U) | 0x10000U;
            record->runtime.endFrames = 0;
            QueueScenarioRecordAction(record, -2);
            EndScenarioTalkSession(record);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void CaptureScenarioTalkInput() {
    if (!(g_scenarioEventContext->flags & 0x82U) && !IsSystemScenarioRecordName(g_scenarioEventContext->activeCharacter)) {
        g_scenarioEventContext->flags |= 2U;
        g_scenarioEventContext->inputState = BindPadControlEntry(g_scenarioEventContext->padControlData);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ReleaseScenarioTalkInput() {
    ScenarioEventContextPrefix *const context = g_scenarioEventContext;
    if ((context->flags >> 1U) & 1U) {
        context->flags &= ~2U;
        BindPadControlEntry(context->inputState);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsScenarioTalkInputCaptured() {
    return (g_scenarioEventContext->flags >> 1U) & 1U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RequestScenarioCameraRestore() {
    g_scenarioEventContext->flags |= 0x4000U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ConsumeScenarioCameraRestoreRequest() {
    if ((g_scenarioEventContext->flags >> 14U) & 1U) {
        if (GetParentMenu() == 7 && !UpdateSelectorMemo() && !IsWristwatchOpenable())
            RestoreConversationCamera();
        g_scenarioEventContext->flags &= ~0x4000U;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BeginScenarioPlayerPresentation() {
    if (IsPlayerControlEnabled()) {
        g_scenarioEventContext->flags |= 0x2000U;
        plymBeginSpecialMode();
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void EndScenarioPlayerPresentation(std::int32_t mode) {
    if ((g_scenarioEventContext->flags >> 13U) & 1U) {
        g_scenarioEventContext->flags &= ~0x2000U;
        g_scenarioPlayerFacingIssued = 0;
        SetPlayerSpecialMode(mode);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ApplyScenarioFaceLook(std::uint32_t command) {
    static const std::uint32_t expressions[16] = {0, 3, 1, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if ((g_scenarioEventContext->flags >> 15) & 1U) {
        const std::int32_t participant = (static_cast<std::int32_t>(command) >> 12) & 15;
        g_scenarioParticipantFaceLooks[participant] = (command >> 8) & 15U;
        Character *const character = ResolveScenarioTalkCharacter(g_freeTalkVisibleCharacters[participant]);
        if (character)
            SetHighModelAndFaceFlagsForChara(character, expressions[g_scenarioParticipantFaceLooks[participant]], command & 255U, 1.0f);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct ScenarioGameplayFlagFields {
    std::uint32_t value0 : 8, value1 : 6, value2 : 4, value3 : 2, value4 : 3, value5 : 2, value6 : 5, value7 : 2;
};
#define SCENARIO_GAMEPLAY_FIELDS (reinterpret_cast<ScenarioGameplayFlagFields *>(g_scenarioGameplayFlags))
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t MapScenarioOperandKind(std::int32_t index) {
    switch (index) {
    case 6:
        return 2;
    case 5:
        return 1;
    default:
        return 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateGameplayFlags() {
    if (!scenarioGameplayFlagsInitialized) {
        for (std::int32_t index = 0; index < 10; ++index) {
            SCENARIO_GAMEPLAY_FIELDS[index].value0 = 1;
            SCENARIO_GAMEPLAY_FIELDS[index].value1 = 0;
            SCENARIO_GAMEPLAY_FIELDS[index].value2 = 0;
            SCENARIO_GAMEPLAY_FIELDS[index].value3 = 0;
            SCENARIO_GAMEPLAY_FIELDS[index].value4 = 0;
            SCENARIO_GAMEPLAY_FIELDS[index].value5 = 0;
            SCENARIO_GAMEPLAY_FIELDS[index].value6 = 0;
            SCENARIO_GAMEPLAY_FIELDS[index].value7 = 0;
        }
        scenarioGameplayFlagsInitialized = 1;
    }
    DateTime date;
    shenGetCurrentDateTime(date);
    if (scenarioGameplayFlagsDay != date.day) {
        for (std::int32_t index = 0; index < 10; ++index) {
            SCENARIO_GAMEPLAY_FIELDS[index].value4 = 0;
            SCENARIO_GAMEPLAY_FIELDS[index].value5 = 0;
        }
        scenarioGameplayFlagsDay = date.day;
    }
    if (scenarioGameplayFlagsStep != static_cast<std::int32_t>(GetCurrentStep())) {
        for (std::int32_t index = 0; index < 10; ++index)
            SCENARIO_GAMEPLAY_FIELDS[index].value7 = 0;
        const std::int32_t step = static_cast<std::int32_t>(GetCurrentStep());
        scenarioGameplayFlagsStep = step;
    }
}
#else
void UpdateGameplayFlags() {
    if (!g_scenarioGameplayFlags.initialized) {
        for (std::uint32_t index = 0; index < 10; ++index)
            g_scenarioGameplayFlags.words[index] = 1;
        g_scenarioGameplayFlags.initialized = 1;
    }
    DateTime date;
    shenGetCurrentDateTime(date);
    if (g_scenarioGameplayFlags.day != date.day) {
        for (std::uint32_t index = 0; index < 10; ++index)
            g_scenarioGameplayFlags.words[index] &= 0xfe0fffffU;
        g_scenarioGameplayFlags.day = date.day;
    }
    if (g_scenarioGameplayFlags.step != static_cast<std::int32_t>(GetCurrentStep())) {
        for (std::uint32_t index = 0; index < 10; ++index)
            g_scenarioGameplayFlags.words[index] &= 0x3fffffffU;
        g_scenarioGameplayFlags.step = static_cast<std::int32_t>(GetCurrentStep());
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RestoreScenarioGameplayFlags(const ScenarioGameplayFlagState *state) {
    g_scenarioGameplayFlags[0] = state->words[0];
    g_scenarioGameplayFlags[1] = state->words[1];
    g_scenarioGameplayFlags[2] = state->words[2];
    g_scenarioGameplayFlags[3] = state->words[3];
    g_scenarioGameplayFlags[4] = state->words[4];
    g_scenarioGameplayFlags[5] = state->words[5];
    g_scenarioGameplayFlags[6] = state->words[6];
    g_scenarioGameplayFlags[7] = state->words[7];
    g_scenarioGameplayFlags[8] = state->words[8];
    g_scenarioGameplayFlags[9] = state->words[9];
    scenarioGameplayFlagsInitialized = state->initialized;
    scenarioGameplayFlagsDay = state->day;
    scenarioGameplayFlagsStep = state->step;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SaveScenarioGameplayFlags(ScenarioGameplayFlagState *state) {
    state->words[0] = g_scenarioGameplayFlags[0];
    state->words[1] = g_scenarioGameplayFlags[1];
    state->words[2] = g_scenarioGameplayFlags[2];
    state->words[3] = g_scenarioGameplayFlags[3];
    state->words[4] = g_scenarioGameplayFlags[4];
    state->words[5] = g_scenarioGameplayFlags[5];
    state->words[6] = g_scenarioGameplayFlags[6];
    state->words[7] = g_scenarioGameplayFlags[7];
    state->words[8] = g_scenarioGameplayFlags[8];
    state->words[9] = g_scenarioGameplayFlags[9];
    state->initialized = scenarioGameplayFlagsInitialized;
    state->day = scenarioGameplayFlagsDay;
    state->step = scenarioGameplayFlagsStep;
}
#endif

// @verified - d
std::int32_t ResolveScntGameDataOperand(std::int32_t index) {
    DateTime date;
    std::int32_t result = 0;
    switch (index) {
    case 0:
        shenGetCurrentDateTime(date);
        result = date.year;
        break;
    case 1:
        shenGetCurrentDateTime(date);
        result = date.month;
        break;
    case 2:
        shenGetCurrentDateTime(date);
        result = date.day;
        break;
    case 3:
        shenGetCurrentDateTime(date);
        result = static_cast<std::int8_t>(date.week);
        break;
    case 4:
        shenGetCurrentDateTime(date);
        result = date.hour;
        break;
    case 5:
        shenGetCurrentDateTime(date);
        result = date.minute;
        break;
    case 6:
        result = GetPlayerStats(2);
        break;
    case 7:
        result = GetCurrentStep();
        break;
    case 8:
        result = GetScenarioTalkCompletedCount(FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter));
        break;
    case 9:
        result = GetScenarioTalkActivationCount(FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter));
        break;
    case 10:
        result = QueryNpcAreaSceneCheck(g_scenarioEventContext->activeCharacter, shen_name()) != 0;
        break;
    }
    return result;
}

// @verified - d
void AssignScntGameDataOperand(std::int32_t index, std::int32_t value) {
    DateTime date;
    switch (index) {
    case 0:
        shenGetCurrentDateTime(date);
        date.year = value;
        SetCurrentDateTimeTo(date);
        break;
    case 1:
        shenGetCurrentDateTime(date);
        date.month = value;
        SetCurrentDateTimeTo(date);
        break;
    case 2:
        shenGetCurrentDateTime(date);
        date.day = value;
        SetCurrentDateTimeTo(date);
        break;
    case 3:
        shenGetCurrentDateTime(date);
        date.week = value;
        SetCurrentDateTimeTo(date);
        break;
    case 4:
        shenGetCurrentDateTime(date);
        date.hour = value;
        SetCurrentDateTimeTo(date);
        break;
    case 5:
        shenGetCurrentDateTime(date);
        date.minute = value;
        SetCurrentDateTimeTo(date);
        break;
    case 6:
        SetPlayerCashOrMoney(value - GetPlayerStats(2), 1);
        break;
    case 7:
        SetSTEP(value);
        break;
    case 8:
        SetScenarioTalkCompletedCount(FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter), value);
        break;
    case 9:
        SetScenarioTalkActivationCount(FindScenarioFreeTalkCharacter(g_scenarioEventContext->activeCharacter), value);
        break;
    case 10:
        g_scenarioSubtitleHoldFrames = -1;
        SetScenarioFollowCharacter(value ? g_scenarioEventContext->activeCharacter : shen_name());
        break;
    }
}

// @verified - d
std::int32_t ResolveScenarioValue(const ScenarioValueOperand &operand) {
    switch (operand.mode) {
    case 1:
        return operand.index;
    case 6:
        return QueryNpcRouteLiveState(shen_name(operand.index));
    case 7: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value0;
    }
    case 8: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value1;
    }
    case 9: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value2;
    }
    case 10: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value3;
    }
    case 11: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value4;
    }
    case 12: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value5;
    }
    case 13: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value6;
    }
    case 14: {
        ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
        return fields[MapScenarioOperandKind(operand.index)].value7;
    }
    case 15:
        return ResolveCurrentMapOperand(operand.index);
    case 2:
        return GetInternalFlag(operand.index);
    case 3:
        return GetStoryFlag(operand.index);
    case 4:
        return GetScenarioByteStat(operand.index);
    case 5:
        return ResolveScntGameDataOperand(operand.index);
    }
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TickScenarioFlagNotification() {
    if (UpdateSelectorMemo()) {
        g_scenarioFlagNotificationPending = 0;
        g_scenarioFlagNotificationShown = 0;
    }
    if (!IsPlayerControlEnabled() || IsScenarioBusy() || !g_scenarioFlagNotificationPending)
        return;
    g_scenarioFlagNotificationFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_scenarioFlagNotificationFrames) + 1U);
    if (g_scenarioFlagNotificationFrames <= 3)
        return;
    g_scenarioFlagNotificationPending = 0;
    g_scenarioFlagNotificationShown = 1;
    void *const jingle = shenPlayAudioWithFlags("JINGURU03", 2);
    g_scenarioFlagNotificationJingle = jingle;
    RequestHelpIconItemPrompt();
}
#endif

// @verified - d
void AssignScenarioValue(const ScenarioValueOperand &operand, std::int32_t value) {
    switch (operand.mode) {
    case 7:
        if (value < 0)
            value = 0;
        if (value > 85)
            value = 85;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value0 = value;
        }
        break;
    case 8:
        if (value < 0)
            value = 0;
        if (value > 31)
            value = 31;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value1 = value;
        }
        break;
    case 9:
        if (value < 0)
            value = 0;
        if (value > 4)
            value = 4;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value2 = value;
        }
        break;
    case 10:
        if (value < 0)
            value = 0;
        if (value > 1)
            value = 1;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value3 = value;
        }
        break;
    case 11:
        if (value < 0)
            value = 0;
        if (value > 3)
            value = 3;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value4 = value;
        }
        break;
    case 12:
        if (value < 0)
            value = 0;
        if (value > 1)
            value = 1;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value5 = value;
        }
        break;
    case 13:
        if (value < 0)
            value = 0;
        if (value > 15)
            value = 15;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value6 = value;
        }
        break;
    case 14:
        if (value < 0)
            value = 0;
        if (value > 1)
            value = 1;
        {
            std::int32_t kind = MapScenarioOperandKind(operand.index);
            ScenarioGameplayFlagFields *fields = SCENARIO_GAMEPLAY_FIELDS;
            fields[kind].value7 = value;
        }
        break;
    case 2:
        ClearOrSetShenmueFlags(operand.index, value);
        break;
    case 3:
        SetStoryFlag(operand.index, value);
        break;
    case 4:
        SetScenarioByteStat(operand.index, value);
        break;
    case 5:
        AssignScntGameDataOperand(operand.index, value);
        break;
    case 1:
    case 6:
        break;
    }
}

// @verified - d
std::int32_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag) {
    ScenarioValueOperand operand;
    switch (mode) {
    case PlayerFlagReadMode_Internal:
        operand.mode = 2;
        break;
    case PlayerFlagReadMode_Story:
        operand.mode = 3;
        break;
    case PlayerFlagReadMode_Stat:
        operand.mode = 4;
        break;
    }
    operand.index = flag;
    return ResolveScenarioValue(operand);
}

void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int32_t flag, std::int32_t value) {
    ScenarioValueOperand operand;
    switch (mode) {
    case PlayerFlagWriteMode_Internal:
        operand.mode = 2;
        break;
    case PlayerFlagWriteMode_Story:
        operand.mode = 3;
        break;
    case PlayerFlagWriteMode_Stat:
        operand.mode = 4;
        break;
    }
    operand.index = static_cast<std::int16_t>(flag);
    AssignScenarioValue(operand, value);
}

// @verified - d
const std::byte *DecodeScntExpression(const std::byte *cursor, ScenarioFreeTalkCharacter *record) {
    ScenarioValueOperand stack[10];
    ScenarioValueOperand *head = stack + 10;
    for (;;) {
        const std::int32_t opcode = *cursor++;
        switch (opcode & 0xe0) {
        case 0x00:
            --head;
            head->mode = 2;
            head->index = ((opcode & 15) << 8) | *cursor++;
            break;
        case 0x20:
            --head;
            head->mode = 3;
            head->index = ((opcode & 15) << 8) | *cursor++;
            break;
        case 0x40: {
            --head;
            head->mode = 4;
            const std::int32_t value = ((opcode & 15) << 8) | *cursor++;
            head->index = value;
            if (opcode & 0x10) {
                head->mode = 1;
                head->index = -value;
            }
            break;
        }
        case 0x60:
            --head;
            head->mode = 1;
            head->index = ((opcode & 15) << 8) | *cursor++;
            if (opcode & 0x10) {
                head->index = (head->index << 8) | *cursor++;
                if (opcode & 8) {
                    head->index = ((head->index << 8) | *cursor++) & 0x3ffffff;
                    if (opcode & 4)
                        head->index = -head->index;
                }
            }
            break;
        case 0xa0:
            --head;
            head->mode = 5;
            head->index = *cursor++;
            break;
        case 0xc0: {
            --head;
            const std::int32_t index = ((opcode & 3) << 8) | *cursor++;
            if (index)
                head->index = index;
            else
                head->index = static_cast<std::uint16_t>(record->savedStateIndex);
            switch (opcode & 0x1c) {
            case 4:
                head->mode = 7;
                break;
            case 8:
                head->mode = 8;
                break;
            case 12:
                head->mode = 9;
                break;
            case 16:
                head->mode = 10;
                break;
            case 20:
                head->mode = 11;
                break;
            case 24:
                head->mode = 12;
                break;
            case 28:
                head->mode = 13;
                break;
            default:
                head->mode = 14;
                break;
            }
            break;
        }
        case 0xe0: {
            --head;
            const std::int32_t index = ((opcode & 3) << 8) | *cursor++;
            switch (opcode & 0x1c) {
            case 0:
                head->mode = 15;
                head->index = index;
                break;
            case 4:
                head->mode = 6;
                if (index)
                    head->index = 0;
                else
                    head->index = record->name;
                break;
            default:
                head->mode = 1;
                head->index = index;
                break;
            }
            break;
        }
        case 0x80: {
            std::int32_t left, right;
            switch (opcode & 0x1f) {
            case 0:
                g_scntExpressionResult = ResolveScenarioValue(*head);
                goto done;
            case 1:
                AssignScenarioValue(head[1], ResolveScenarioValue(*head));
                ++head;
                break;
            case 2:
                AssignScenarioValue(*head, ResolveScenarioValue(*head) + 1);
                break;
            case 3:
                AssignScenarioValue(*head, ResolveScenarioValue(*head) - 1);
                break;
            case 4:
                head->index = !ResolveScenarioValue(*head);
                head->mode = 1;
                break;
            case 5:
                head[1].index = ResolveScenarioValue(head[1]) && ResolveScenarioValue(*head);
                head[1].mode = 1;
                ++head;
                break;
            case 6:
                head[1].index = ResolveScenarioValue(head[1]) || ResolveScenarioValue(*head);
                head[1].mode = 1;
                ++head;
                break;
            case 7:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left == right;
                head[1].mode = 1;
                ++head;
                break;
            case 8:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left != right;
                head[1].mode = 1;
                ++head;
                break;
            case 9:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left < right;
                head[1].mode = 1;
                ++head;
                break;
            case 10:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left <= right;
                head[1].mode = 1;
                ++head;
                break;
            case 11:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left > right;
                head[1].mode = 1;
                ++head;
                break;
            case 12:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left >= right;
                head[1].mode = 1;
                ++head;
                break;
            case 13:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left + right;
                head[1].mode = 1;
                ++head;
                break;
            case 14:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left - right;
                head[1].mode = 1;
                ++head;
                break;
            case 15:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left * right;
                head[1].mode = 1;
                ++head;
                break;
            case 16:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left / right;
                head[1].mode = 1;
                ++head;
                break;
            case 17:
                left = ResolveScenarioValue(head[1]);
                right = ResolveScenarioValue(*head);
                head[1].index = left % right;
                head[1].mode = 1;
                ++head;
                break;
            }
            break;
        }
        }
    }
done:
    return cursor;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name GetScenarioActorTypeName(std::int32_t isNpc, std::int32_t enabled) {
    if (enabled) {
        if (isNpc)
            return SHEN_NAME4('N', 'P', 'C', '_');
        return SHEN_NAME4('R', 'Y', 'O', '_');
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ResetScenarioInteractionState() {
    g_scenarioLookActive = g_scenarioRyoAcknowledged = g_scenarioActionSystemRecord = 0;
    g_scenarioLookTargetReleased = g_scenarioLookCameraState = 0;
    g_scenarioLookLocationPending = g_scenarioLookReleaseRequested = g_scenarioLookAcknowledged = g_scenarioFaceForwardPending = 0;
    g_scenarioPlayerTurnPending = g_scenarioLetterShowPending = g_scenarioLetterHidePending = g_scenarioLetterVisible = 0;
    g_scenarioRetainedTalkByte = g_scenarioSpeakerPoseCaptured = 0;
    g_scenarioLastTurnAction = -1;
    g_scenarioPreviousPlayerTurnTarget = g_scenarioSpeakerChestHeight = g_scenarioSpeakerHeadHeight = 0;
    g_scenarioEventContext->flags &= ~0x80U;
    for (std::int32_t index = 0; index < 8; ++index) {
        g_scenarioActionParticipants[index] = shen_name();
        g_scenarioParticipantMotionIds[index] = -1;
        g_scenarioRetainedParticipants[index] = g_scenarioParticipantWorldYaws[index] = g_scenarioParticipantEntityYaws[index] = 0;
        g_scenarioTurnPending[index] = g_scenarioParticipantTurnIssued[index] = g_scenarioLookReleasePending[index] = g_scenarioParticipantFlags[index] = 0;
        g_scenarioParticipantFacing[index] = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void scenProcessCharacterActionQueue() {
    Character *first, *second;
    std::int32_t action;
    for (std::int8_t index = 0; index < 8; ++index) {
        if (g_scenarioLookReleasePending[index]) {
            action = reinterpret_cast<std::int32_t>(ResolveScenarioTalkCharacter(g_scenarioLookReleaseNames[index]));
            if (action)
                g_scenarioLookReleasePending[index] = 0;
        }
    }
    if (g_scenarioLookLocationPending) {
        scenarioLookLocationFrames = static_cast<std::int8_t>(scenarioLookLocationFrames + 1);
        if (scenarioLookLocationFrames > 19) {
            g_scenarioLookLocationPending = 0;
            scenarioLookLocationFrames = 0;
            if (g_scenarioLookTargetReleased) {
                g_scenarioLookCameraState = -1;
                g_scenarioLookTargetReleased = 0;
            } else {
                g_scenarioLookCameraState = 1;
                camCtrlSetConversationFlag4000(1);
            }
            SetScenarioPlayerLookTarget(g_scenarioLookLocation, 0);
        }
    }
    if (plymIsAuthorTargetActive() && g_freeTalkVisibleCharacters[0] && g_freeTalkVisibleCharacters[1]) {
        first = ResolveScenarioTalkCharacter(g_freeTalkVisibleCharacters[0]);
        second = ResolveScenarioTalkCharacter(g_freeTalkVisibleCharacters[1]);
        if (first && second) {
            MotionWork *motion = charFindMotionwork(first);
            if (motion) {
                if (g_scenarioParticipantFacing[0] > 0) {
                    Vector3 position;
                    charaGetMotWorldPos(second, 5, &position);
                    mtwkUpdateWithPosition(motion, &position, 0x302);
                }
                if (g_scenarioParticipantFacing[0] < 0) {
                    mtwkUpdateWithPosition(motion, 0, 0);
                    g_scenarioParticipantFacing[0] = 0;
                }
            }
            motion = charFindMotionwork(second);
            if (motion) {
                if (g_scenarioParticipantFacing[1] > 0) {
                    Vector3 position;
                    charaGetMotWorldPos(first, 5, &position);
                    mtwkUpdateWithPosition(motion, &position, 0x302);
                }
                if (g_scenarioParticipantFacing[1] < 0) {
                    mtwkUpdateWithPosition(motion, 0, 0);
                    g_scenarioParticipantFacing[1] = 0;
                }
            }
        }
    }
    if (g_scenarioPlayerTurnPending)
        TurnScenarioPlayerTowardSpeaker();
    if (g_scenarioLetterShowPending) {
        scenarioLetterFrames = static_cast<std::int8_t>(scenarioLetterFrames + 1);
        if (scenarioLetterFrames > 24) {
            g_scenarioLetterShowPending = 0;
            scenarioLetterFrames = 0;
            const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            Character *const ryo = control->ryo;
            Character *const letter = ResolveScenarioTalkCharacter(shen_name(SHEN_NAME4('L', 'E', 'T', 'R')));
            if (letter && ryo) {
                HLib::Task *const task = GetCharacterHandlerTask(letter);
                SetCharacterActive(letter, 1);
                if (task)
                    HLib::ResolveDeferredCallback(task);
                EV_SetCharDisplay(letter, CVM_DisplayEnabled, 1);
                g_scenarioLetterVisible = 1;
                if (IsOfThisVisibilityType(letter, CVM_DisplayEnabled)) {
                    Vector3 offset;
                    std::int32_t rotation[3];
                    rotation[1] = 25486;
                    rotation[2] = rotation[0] = 16384;
                    offset.x = 0.17f;
                    offset.y = 0.050000001f;
                    offset.z = -0.029999999f;
                    SetFixoDataForMotionWork(letter, ryo, 14, offset, rotation);
                }
                SetModePerCurrentStep(-1);
            }
        }
    } else if (g_scenarioLetterHidePending) {
        scenarioLetterFrames = static_cast<std::int8_t>(scenarioLetterFrames + 1);
        if (scenarioLetterFrames > 59) {
            g_scenarioLetterHidePending = 0;
            scenarioLetterFrames = 0;
            second = ResolveScenarioTalkCharacter(shen_name(SHEN_NAME4('L', 'E', 'T', 'R')));
            if (second) {
                HLib::Task *const task = GetCharacterHandlerTask(second);
                SetCharacterActive(second, 0);
                if (task)
                    HLib::SetupTimedTask(task, 0);
                EV_SetCharDisplay(second, CVM_DisplayEnabled, 0);
                g_scenarioLetterVisible = 0;
            }
        }
    }
    if (g_scenarioLookReleaseRequested) {
        scenarioLookReleaseFrames = static_cast<std::int8_t>(scenarioLookReleaseFrames + 1);
        if (scenarioLookReleaseFrames > 29) {
            g_scenarioLookReleaseRequested = 0;
            scenarioLookReleaseFrames = 0;
            first = ResolveScenarioTalkCharacter(g_scenarioLookReleaseCharacter);
            if (first) {
                ResetNpcSpecialMotionLookTarget(g_scenarioLookReleaseCharacter);
                Vector3 zero;
                zero.x = zero.y = zero.z = 0.0f;
                SetEyeToChara(ResolveScenarioTalkCharacter(g_scenarioLookReleaseCharacter), &zero, 8, 0);
                g_scenarioLookActive = 0;
            }
        }
    }
    for (std::int8_t slot = 0; slot < 10; ++slot)
        if (g_scenarioTurnPending[slot])
            TurnScenarioParticipantToward(slot);
    if (g_scenarioFaceForwardPending)
        FaceScenarioParticipantsForward();
    for (;;) {
        ScenarioCharacterAction queued;
        if (static_cast<std::int32_t>(ReadScenarioActionQueue(g_scenarioActionQueueId, &queued, sizeof(queued))) <= 0)
            return;
        g_scenarioActionCharacter = queued.name;
        action = queued.action;
        SelectScenarioActionParticipants(queued.name);
        g_scenarioActionSystemRecord = static_cast<std::uint8_t>(IsSystemScenarioRecordName(g_scenarioActionCharacter));
        switch (action) {
        case -1:
            AcknowledgeScenarioPlayer(g_scenarioActionCharacter);
            break;
        case -8:
            RetireScenarioTalkParticipants(g_scenarioActionCharacter);
            break;
        case 10:
        case 13:
        case 15:
        case 16:
            break;
        case -2:
            g_scenarioLookAcknowledged = 0;
            AcknowledgeScenarioLookRelease(g_scenarioActionCharacter);
            break;
        }
        if (action - 40U <= 9U) {
            g_scenarioFacingCommandsIssued = g_scenarioNpcFacingIssued = 1;
            if (plymIsAuthorTargetActive()) {
                if (action == 40)
                    g_scenarioParticipantFacing[1] = 1;
            } else {
                QueueScenarioParticipantTurn(action);
                g_scenarioLastTurnAction = action;
            }
        } else if (action - 50U <= 9U) {
            const std::int8_t participant = static_cast<std::int8_t>(action - 49U);
            if (plymIsAuthorTargetActive())
                g_scenarioParticipantFacing[participant] = -1;
            else
                ReleaseScenarioParticipantLook(g_scenarioActionParticipants[participant]);
        } else if (action - 140U <= 9U) {
            g_scenarioFacingCommandsIssued = g_scenarioPlayerFacingIssued = 1;
            const std::int8_t participant = static_cast<std::int8_t>(action - 139U);
            if (plymIsAuthorTargetActive())
                g_scenarioParticipantFacing[0] = participant;
            else {
                g_scenarioPlayerTurnTarget = g_scenarioActionParticipants[participant];
                g_scenarioPlayerTurnPending = 1;
            }
        } else if (action == 203) {
            if (plymIsAuthorTargetActive())
                g_scenarioParticipantFacing[0] = -1;
            else {
                const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                mtwkUpdateWithPosition(charFindMotionwork(control->ryo), 0, 0);
            }
        } else if (action - 20U <= 9U || action - 217U <= 29U || action - 120U <= 9U) {
            LookAtScenarioRecordLocation(action, g_scenarioActionCharacter);
        } else if (action == 13) {
            EndScenarioLocationLook(action, g_scenarioActionCharacter);
        } else if (action == 10) {
            if (!g_scenarioLetterVisible) {
                g_scenarioLetterShowPending = 1;
                SetScenarioLetterMotion(g_scenarioActionCharacter, 1);
            }
        } else if (action == 15) {
            if (g_scenarioLetterVisible) {
                g_scenarioLetterHidePending = 1;
                SetScenarioLetterMotion(g_scenarioActionCharacter, 0);
            }
        } else if (action == 200) {
            g_scenarioSuppressNpcFacing = 1;
        } else if (action == 201) {
            g_scenarioSuppressPlayerFacing = 1;
        } else if (action == 202) {
            g_scenarioLookTargetReleased = 1;
        } else if (action == 205) {
            ExecuteAicaCommand(0x800ab, 0, 0);
        } else if (action - 207U <= 9U) {
            plymRequestSpecialPoseSequence(action - 207);
        } else if (action == 16) {
            if (!plymIsAuthorTargetActive() && g_scenarioFaceForwardMode)
                g_scenarioFaceForwardPending = 1;
            g_scenarioFaceForwardMode = !g_scenarioFaceForwardMode;
        }
        std::int32_t acknowledgement[2] = {g_scenarioActionCharacter, -10};
        WriteScenarioActionQueue(g_scenarioAcknowledgementQueueId, acknowledgement, sizeof(acknowledgement));
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void FaceScenarioParticipantsForward() {
    for (std::int32_t index = 0; index < 8; ++index) {
        if (g_scenarioParticipantFlags[index])
            continue;
        Character *const character = ResolveScenarioTalkCharacter(g_scenarioActionParticipants[index]);
        if (!character)
            continue;
        Vector3 position;
        charaGetMotWorldPos(character, 5, &position);
        const std::uint32_t yaw = shenGetRotYFromNode(character);
        position.x -= dc::sdk::SinFixed(yaw);
        position.z -= dc::sdk::CosFixed(yaw);
        RequestNpcSpecialMotionTarget(g_scenarioActionParticipants[index], &position);
        g_scenarioParticipantFlags[index] = 1;
    }
    g_scenarioFaceForwardPending = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void QueueScenarioParticipantTurn(std::int32_t action) {
    const std::int32_t pair = action - 41;
    std::int32_t source, target;
    if (pair < 0) {
        source = 1;
        target = 0;
    } else {
        source = pair / 3 + 1;
        target = pair % 3;
        if (source <= target)
            ++target;
    }
    g_scenarioTurnSource = g_scenarioActionParticipants[source];
    g_scenarioTurnTarget = g_scenarioActionParticipants[target];
    g_scenarioTurnPending[action - 40] = 1;
    g_scenarioTurnSources[action - 40] = g_scenarioTurnSource;
    g_scenarioTurnTargets[action - 40] = g_scenarioTurnTarget;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void TurnScenarioParticipantToward(std::int32_t slot) {
    if (!g_scenarioSpeakerPoseCaptured) {
        CaptureScenarioSpeakerPose();
        g_scenarioSpeakerPoseCaptured = 1;
    }
    std::int32_t participant = 0;
    for (std::int8_t index = 0; index < 8; ++index)
        if (g_scenarioTurnSources[slot] == g_scenarioActionParticipants[index])
            participant = index;
    Vector3 position;
    position.x = position.y = position.z = 0.0f;
    Character *const target = ResolveScenarioTalkCharacter(g_scenarioTurnTargets[slot]);
    if (target)
        charaGetMotWorldPos(target, 5, &position);
    g_scenarioParticipantTurnIssued[slot] = 1;
    RequestNpcSpecialMotionTarget(g_scenarioTurnSources[slot], &position);
    SetEyeToChara(ResolveScenarioTalkCharacter(g_scenarioTurnSources[slot]), &position, 8, 2);
    g_scenarioParticipantFlags[participant] = 1;
    if (g_scenarioLookActive)
        RequestScenarioLookRelease(g_scenarioTurnSources[slot]);
    g_scenarioTurnPending[slot] = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void ReleaseScenarioParticipantLook(shen_name name) {
    if (g_scenarioLookActive)
        RequestScenarioLookRelease(name);
    Vector3 zero;
    zero.x = zero.y = zero.z = 0.0f;
    for (std::int8_t index = 0; index < 8; ++index) {
        if (g_scenarioActionParticipants[index] != name)
            continue;
        if (!g_scenarioParticipantFlags[index])
            return;
        g_scenarioLookReleaseNames[index] = name;
        g_scenarioLookReleasePending[index] = 1;
        ResetNpcSpecialMotionTarget(name);
        SetEyeToChara(ResolveScenarioTalkCharacter(name), &zero, 8, 0);
        g_scenarioParticipantFlags[index] = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void LookAtScenarioRecordLocation(std::int32_t action, shen_name name) {
    if (!ResolveScenarioTalkCharacter(name))
        return;
    std::int32_t location;
    if (static_cast<std::uint32_t>(action) - 20U <= 9U)
        location = action - 20;
    else if (static_cast<std::uint32_t>(action) - 217U <= 29U)
        location = action - 207;
    else
        location = action - 120;
    const ScenarioFreeTalkCharacter *const record = FindScenarioFreeTalkCharacter(name);
    if (!record)
        return;
    g_scenarioLookLocation = location < 0 || location >= static_cast<std::int8_t>(record->participantCount) ?
        &g_scenarioDefaultLookLocation :
        reinterpret_cast<const Vector3 *>(record->locations) + location;
    RequestNpcSpecialMotionLookTarget(name, g_scenarioLookLocation);
    SetEyeToChara(ResolveScenarioTalkCharacter(name), g_scenarioLookLocation, 8, 2);
    g_scenarioLookLocationPending = 1;
    g_scenarioLookActive = 1;
    g_scenarioLookTargetActive = 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void SetScenarioPlayerLookTarget(const Vector3 *position, std::int32_t mode) {
    if (mode)
        plymSetSpecialModeTargetPosition(position, 0);
    else
        plymSetSpecialModeTargetPosition(position, 1);
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetEyeToChara(control->ryo, position, 8, 2);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void RequestScenarioLookRelease(shen_name name) {
    g_scenarioLookReleaseRequested = 1;
    g_scenarioLookReleaseCharacter = name;
    g_scenarioLookActive = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void EndScenarioLocationLook(std::int32_t action, shen_name name) {
    if (action >= 0) {
        RequestScenarioLookRelease(name);
        g_scenarioLookCameraState = 0;
        camCtrlSetConversationFlag4000(0);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void SetScenarioLetterMotion(shen_name name, std::int8_t show) {
    if (name) {
        if (show)
            plymBeginSpecialTargetMotionSequence(shen_name(SHEN_NAME4('L', 'E', 'T', 'R')));
        else
            plymContinueSpecialTargetMotionSequence();
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void AcknowledgeScenarioPlayer(shen_name name) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (control->ryo) {
        if (!g_scenarioRyoAcknowledged)
            g_scenarioRyoAcknowledged = 1;
        std::int32_t acknowledgement[2] = {name, -1};
        WriteScenarioActionQueue(g_scenarioAcknowledgementQueueId, acknowledgement, sizeof(acknowledgement));
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void AcknowledgeScenarioLookRelease(shen_name name) {
    if (g_scenarioLookActive)
        RequestScenarioLookRelease(name);
    std::int32_t acknowledgement[2] = {name, -2};
    WriteScenarioActionQueue(g_scenarioAcknowledgementQueueId, acknowledgement, sizeof(acknowledgement));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static inline void HideScenarioLetter() {
    Character *const letter = ResolveScenarioTalkCharacter(shen_name(SHEN_NAME4('L', 'E', 'T', 'R')));
    if (letter) {
        HLib::Task *const task = GetCharacterHandlerTask(letter);
        SetCharacterActive(letter, 0);
        if (task)
            HLib::SetupTimedTask(task, 0);
        EV_SetCharDisplay(letter, CVM_DisplayEnabled, 0);
    }
    g_scenarioLetterVisible = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void RetireScenarioTalkParticipants(shen_name name) {
    Vector3 zero;
    zero.x = zero.y = zero.z = 0.0f;
    for (std::int8_t index = 0; index < g_freeTalkVisibleCharacterCount; ++index) {
        if (g_freeTalkVisibleCharacters[index]) {
            if (!ScenarioCharacterIsRyo(g_freeTalkVisibleCharacters[index]))
                EndNpcSpecialMotion(g_freeTalkVisibleCharacters[index]);
            Character *const character = ResolveScenarioTalkCharacter(g_freeTalkVisibleCharacters[index]);
            if (plymIsAuthorTargetActive() && character)
                mtwkUpdateWithPosition(charFindMotionwork(character), 0, 0);
            if (character) {
                SetEyeToChara(character, &zero, 8, 0);
                if (ScenarioCharacterIsRyo(g_freeTalkVisibleCharacters[index]))
                    SelectCharacterFacialNode(character, 0);
                SetHighModelAndFaceFlagsForChara(character, 0, 10, 1.0f);
                SetFaceFlagsForChara(character, 1);
                CharacterHandComponent *hand = AllocateHNDR(character);
                if (hand)
                    SetCharacterHandRegionEnabled(hand, 0, 0, -1);
                hand = AllocateHNDL(character);
                if (hand)
                    SetCharacterHandRegionEnabled(hand, 0, 0, -1);
            }
        }
        g_freeTalkVisibleCharacters[index] = shen_name();
        g_scenarioParticipantFaceLooks[index] = 0;
        g_scenarioParticipantFacing[index] = 0;
    }
    g_freeTalkVisibleCharacterCount = 0;
    plymEndSpecialMode(0);
    g_scenarioFaceForwardMode = g_scenarioFacingCommandsIssued = g_scenarioPlayerFacingIssued = g_scenarioNpcFacingIssued = g_scenarioSuppressPlayerFacing =
        g_scenarioSuppressNpcFacing = 0;
    if (g_scenarioLetterVisible)
        HideScenarioLetter();
    ResetScenarioInteractionState();
    std::int32_t acknowledgement[2] = {name, -8};
    WriteScenarioActionQueue(g_scenarioAcknowledgementQueueId, acknowledgement, sizeof(acknowledgement));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void SelectScenarioActionParticipants(shen_name name) {
    const shen_name *const participants = GetScenarioFreeTalkParticipants(name);
    shen_name participant;
    if (participants)
        for (std::int8_t index = 0; index < 8 && (participant = participants[index]) != 0; ++index)
            g_scenarioActionParticipants[index] = participant;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void TurnScenarioPlayerTowardSpeaker() {
    Character *character;
    Vector3 position, blended;
    if (!g_scenarioSpeakerPoseCaptured) {
        CaptureScenarioSpeakerPose();
        g_scenarioSpeakerPoseCaptured = 1;
    }
    if (scenarioPlayerTurnFrameLimit == -1)
        scenarioPlayerTurnFrameLimit = 20;
    if (g_scenarioPlayerTurnTarget != g_scenarioPreviousPlayerTurnTarget && g_scenarioPreviousPlayerTurnTarget) {
        character = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
        if (character) {
            Vector3 current, old;
            SolveRyoMatrix(CHARACTER_NAME_RYO, g_scenarioPlayerTurnTarget, shenGetRotYFromNode(character), &current);
            SolveRyoMatrix(CHARACTER_NAME_RYO, g_scenarioPreviousPlayerTurnTarget, shenGetRotYFromNode(character), &old);
            if (current.z * old.z < 0.0f)
                scenarioPlayerTurnAround = 1;
            else {
                scenarioPlayerTurnAround = 0;
                g_scenarioPreviousPlayerTurnTarget = 0;
            }
        }
    }
    if (scenarioPlayerTurnFrames == scenarioPlayerTurnFrameLimit) {
        g_scenarioPlayerTurnPending = 0;
        scenarioPlayerTurnFrames = 0;
        scenarioPlayerTurnFrameLimit = -1;
        g_scenarioPreviousPlayerTurnTarget = g_scenarioPlayerTurnTarget;
        return;
    }
    if (scenarioPlayerTurnAround) {
        character = ResolveScenarioTalkCharacter(g_scenarioPlayerTurnTarget);
        if (character) {
            Vector3 targetPosition;
            charaGetMotWorldPos(character, 5, &targetPosition);
            SetScenarioPlayerLookTarget(&targetPosition, 1);
            g_scenarioPlayerTurnPending = 0;
            scenarioPlayerTurnFrames = 0;
            scenarioPlayerTurnAround = 0;
            return;
        }
    }
    scenarioPlayerTurnFrames = static_cast<std::int8_t>(scenarioPlayerTurnFrames + 1);
    character = ResolveScenarioTalkCharacter(g_scenarioPlayerTurnTarget);
    if (character) {
        charaGetMotWorldPos(character, 5, &position);
        blended.x = g_scenarioSpeakerPosition.x -
            static_cast<float>(scenarioPlayerTurnFrames) / static_cast<float>(scenarioPlayerTurnFrameLimit) * (g_scenarioSpeakerPosition.x - position.x);
        blended.y = g_scenarioSpeakerPosition.y -
            static_cast<float>(scenarioPlayerTurnFrames) / static_cast<float>(scenarioPlayerTurnFrameLimit) * (g_scenarioSpeakerPosition.y - position.y);
        blended.z = g_scenarioSpeakerPosition.z -
            static_cast<float>(scenarioPlayerTurnFrames) / static_cast<float>(scenarioPlayerTurnFrameLimit) * (g_scenarioSpeakerPosition.z - position.z);
        blended = position;
        g_scenarioSpeakerPosition = blended;
        g_scenarioPlayerTurnPending = 0;
        scenarioPlayerTurnFrames = 0;
        scenarioPlayerTurnFrameLimit = -1;
        SetScenarioPlayerLookTarget(&blended, 1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void CaptureScenarioSpeakerPose() {
    Character *character;
    for (std::int8_t index = 0; index < 8; ++index) {
        character = ResolveScenarioTalkCharacter(g_scenarioActionParticipants[index]);
        if (!character)
            continue;
        MotionWork *const motion = charFindMotionwork(character);
        if (!motion)
            continue;
        g_scenarioParticipantMotionIds[index] = mtwkGetAnimationId(motion, LowerBody);
        g_scenarioParticipantWorldYaws[index] = shenGetRotYFromNode(character);
        g_scenarioParticipantEntityYaws[index] = mtwkGetEntityYaw(motion);
    }
    character = ResolveScenarioTalkCharacter(g_scenarioActionCharacter);
    if (character)
        charaGetMotWorldPos(character, 5, &g_scenarioSpeakerPosition);
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    character = control->ryo;
    if (character)
        charaGetMotWorldPos(character, 5, &g_scenarioPlayerPosition);
    character = ResolveScenarioTalkCharacter(g_scenarioActionCharacter);
    if (character) {
        Vector3 position;
        charaGetMotWorldPos(character, 22, &position);
        g_scenarioSpeakerHeadHeight = position.y;
        charaGetMotWorldPos(character, 27, &position);
        g_scenarioSpeakerChestHeight = position.y;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SolveRyoMatrix(shen_name origin, shen_name other, std::int32_t yaw, Vector3 *result) {
    Character *const first = ResolveScenarioTalkCharacter(origin);
    if (!first)
        return;
    Vector3 originPosition;
    GetCharacterLocalPosition(first, &originPosition);
    Character *const second = ResolveScenarioTalkCharacter(other);
    if (!second)
        return;
    Vector3 otherPosition;
    GetCharacterLocalPosition(second, &otherPosition);
    Matrix4x4 frame;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&originPosition);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    matrix_stack::GetMatrix(&frame);
    matrix_stack::InvertMatrix(&frame);
    PopCurrentMatrix(1);
    matrix_stack::PushUnitMatrix();
    matrix_stack::LoadMatrix(&frame);
    Matrix4x4::TransformPoint(otherPosition, *result);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SpawnMoveScroll() {
    std::int32_t index;
    MT7Model *model;
    for (index = 0; index < 7; ++index)
        g_scenarioStepModels[index] = 0;
    Character *const character = AddCharacterHandler(3);
    if (!character)
        return;
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CreateScenarioCharacterId(control->scenarioTask, SHEN_NAME(LETR), character);
    AllocateFIXO(character);
    void *data = shenLoadFile("misc", "TMEM200G.MT7", SHEN_NAME(LETR));
    if (data) {
        model = InitializeMT7Model(data);
        if (model) {
            for (index = 0; index < 7; ++index)
                g_scenarioStepModels[index] = model->root;
        }
    }
    if (static_cast<std::int32_t>(GetCurrentStep()) <= 49999) {
        data = shenLoadFile("misc", "LMAL200G.MT7", SHEN_NAME(LETR));
        if (data) {
            model = InitializeMT7Model(data);
            if (model)
                g_scenarioStepModels[0] = model->root;
        }
    }
    if (static_cast<std::int32_t>(GetCurrentStep()) <= 119999) {
        data = shenLoadFile("misc", "ABR02MPG.MT7", SHEN_NAME(LETR));
        if (data) {
            model = InitializeMT7Model(data);
            if (model)
                g_scenarioStepModels[2] = model->root;
        }
    } else {
        if (static_cast<std::int32_t>(GetCurrentStep()) <= 359999) {
            data = shenLoadFile("misc", "PSF72AWG.MT7", SHEN_NAME(LETR));
            if (data) {
                model = InitializeMT7Model(data);
                if (model)
                    g_scenarioStepModels[3] = model->root;
            }
            data = shenLoadFile("misc", "PSF6200G.MT7", SHEN_NAME(LETR));
            if (data) {
                model = InitializeMT7Model(data);
                if (model)
                    g_scenarioStepModels[4] = model->root;
            }
            data = shenLoadFile("misc", "PSF3200G.MT7", SHEN_NAME(LETR));
            if (data) {
                model = InitializeMT7Model(data);
                if (model)
                    g_scenarioStepModels[5] = model->root;
            }
        }
        if (static_cast<std::int32_t>(GetCurrentStep()) <= 369999) {
            data = shenLoadFile("misc", "HKSM201G.MT7", SHEN_NAME(LETR));
            if (data) {
                model = InitializeMT7Model(data);
                if (model)
                    g_scenarioStepModels[6] = model->root;
            }
        }
    }
    SetModePerCurrentStep(-1);
    SetCharacterAlpha(character, 0.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetModePerCurrentStep(std::int32_t mode) {
    Character *const character = ResolveScenarioTalkCharacter(SHEN_NAME(LETR));
    if (!character)
        return;
    if (mode < 0) {
        if (static_cast<std::int32_t>(GetCurrentStep()) <= 49999)
            mode = 0;
        else if (static_cast<std::int32_t>(GetCurrentStep()) <= 109999)
            mode = 1;
        else if (static_cast<std::int32_t>(GetCurrentStep()) <= 119999)
            mode = 2;
        else if (static_cast<std::int32_t>(GetCurrentStep()) <= 349999)
            mode = 1;
        else if (static_cast<std::int32_t>(GetCurrentStep()) <= 359999) {
            if (g_scenarioSelectedFollowFlag == 347)
                mode = 3;
            else if (g_scenarioSelectedFollowFlag == 348)
                mode = 4;
            else if (g_scenarioSelectedFollowFlag == 349)
                mode = 5;
            else
                mode = 1;
        } else if (static_cast<std::int32_t>(GetCurrentStep()) <= 369999)
            mode = 6;
        else
            mode = 1;
    }
    character->model = g_scenarioStepModels[mode];
}
#endif

shen_name FindMapRegionName(const Vector3 *position) {
    shen_name name = shen_name();
    std::int32_t index;
    for (index = 0; !name && mapRegions[index].name; ++index) {
        if (g_currentMap == mapRegions[index].map) {
            if (999.0f > mapRegions[index].bounds[3]) {
                if (!(position->x < mapRegions[index].bounds[0]) && !(position->x > mapRegions[index].bounds[2]) &&
                    !(position->z < mapRegions[index].bounds[1]) && !(position->z > mapRegions[index].bounds[3]))
                    name = mapRegions[index].name;
            } else {
                const float x = position->x - mapRegions[index].bounds[0], z = position->z - mapRegions[index].bounds[1];
                if (mapRegions[index].bounds[2] > NativeSqrt(x * x + z * z))
                    name = mapRegions[index].name;
            }
        }
    }
    for (index = 0; !name && mapRegionFallbacks[index].name; ++index)
        if (g_currentMap == mapRegionFallbacks[index].map)
            name = mapRegionFallbacks[index].name;
    return name;
}

void RebuildScenarioEventObjectMapNames() {
    Vector3 position;
    plymUpdatePlayerSearchLocation(&position);
    g_mapRegionNames[0] = FindMapRegionName(&position);
    std::int32_t count = g_mapRegionNames[0] != 0;
    for (std::int32_t index = 0; mapRegionRelations[index].name; ++index) {
        if (g_currentMap == mapRegionRelations[index].map && count < 6)
            g_mapRegionNames[count++] = mapRegionRelations[index].name;
    }
    while (count < 6)
        g_mapRegionNames[count++] = shen_name();
}

// @verified - d
shen_name scenFindMapRegionName(const Vector3 *position) {
    shen_name name;
    if (!position)
        name = g_mapRegionNames[0];
    else
        name = FindMapRegionName(position);
    return name;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GetVertexBufferUsedBytes() {
    return dc::sdk::g_systemMetrics[5];
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
__attribute__((noipa)) std::uint32_t DebugMenuInputBlockedNullsub() {
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AssignEventPrintPointers();
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void PrintSupportMetric(const char *format, ...) {
    char text[256];
    std::va_list arguments;
    va_start(arguments, format);
    std::vsprintf(text, format, arguments);
    va_end(arguments);
    if (!(g_inputBuffer[3].pressedPriority & 4U))
        shenmue_s2_event_DebugLogToScreen(text);
}
#endif

} // namespace

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DisplaySystemMetrics() {
    supportIptCurrent = dc::sdk::g_systemMetrics[5];
    if (supportIptMaximum < supportIptCurrent)
        supportIptMaximum = supportIptCurrent;
    if (supportIptMinimum > supportIptCurrent)
        supportIptMinimum = supportIptCurrent;
    if (g_inputBuffer[3].pressedPriority & 0x200U) {
        supportIptMaximum = supportIptCurrent;
        supportIptMinimum = supportIptCurrent;
    }
    if (g_inputBuffer[3].pressedPriority & 0x400U) {
        supportMetricsPage = static_cast<std::int32_t>(static_cast<std::uint32_t>(supportMetricsPage) + 1U);
        if (supportMetricsPage > 3)
            supportMetricsPage = 0;
    }
    switch (supportMetricsPage) {
    case 0:
        SetDebugScreenTextPosition(1, 25);
        PrintSupportMetric("Max : 0x%x\n", static_cast<unsigned>(supportIptMaximum));
        PrintSupportMetric("Rel : 0x%x\n", static_cast<unsigned>(supportIptCurrent));
        PrintSupportMetric("Min : 0x%x\n", static_cast<unsigned>(supportIptMinimum));
        break;
    case 1:
        SetDebugScreenTextPosition(2, 10);
        PrintSupportMetric("ITPBase      : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[0]));
        PrintSupportMetric("ITPLimit     : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[3]));
        PrintSupportMetric("ITPCurrent   : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[5]));
        PrintSupportMetric("OBJBase      : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[1]));
        PrintSupportMetric("OBJLimit     : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[4]));
        PrintSupportMetric("IVBS Opaque  : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[32]));
        PrintSupportMetric("IVBS OpaqueM : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[33]));
        PrintSupportMetric("IVBS Trans   : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[34]));
        PrintSupportMetric("IVBS TransM  : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[35]));
        PrintSupportMetric("IVBS Punch   : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[36]));
        PrintSupportMetric("MVBS Opaque  : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[37]));
        PrintSupportMetric("MVBS OpaqueM : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[38]));
        PrintSupportMetric("MVBS Trans   : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[39]));
        PrintSupportMetric("MVBS TransM  : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[40]));
        PrintSupportMetric("MVBS Punch   : 0x%x\n", static_cast<unsigned>(dc::sdk::g_systemMetrics[41]));
        break;
    case 2: {
        std::size_t total, largest, waste;
        QueryFreeTextureMemory(&total, &largest);
        SetDebugScreenTextPosition(2, 5);
        PrintSupportMetric("FreeTexture  : %d\n", static_cast<int>(total));
        PrintSupportMetric("MaxBlockSize : %d\n", static_cast<int>(largest));
        HLib::AnalyzeFreeList(&total, &largest, &waste);
        PrintSupportMetric("Heap Rest    : %d\n", static_cast<int>(total));
        PrintSupportMetric("Heap Max     : %d\n", static_cast<int>(largest));
        PrintSupportMetric("Heap Waste   : %d\n", static_cast<int>(waste));
        break;
    }
    }
}
#endif

} // namespace

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DisplayFlagInfo(SupportTaskData *data) {
    if (g_inputBuffer[3].pressedPriority & 0x200U) {
        if (data->markedFlag == data->flagIndex)
            data->markedFlag = -1;
        else
            data->markedFlag = data->flagIndex;
    }
    std::int32_t step = 0;
    if (g_inputBuffer[3].repeatedPresses & 0x4000U)
        step = -1;
    if (static_cast<std::int16_t>(g_inputBuffer[3].repeatedPresses) < 0)
        step = 1;
    if (g_inputBuffer[3].held & 4U)
        step *= 10;
    if (g_inputBuffer[3].held & 2U)
        step *= 100;
    data->flagIndex = static_cast<std::int32_t>(static_cast<std::uint32_t>(data->flagIndex) + static_cast<std::uint32_t>(step));
    if (data->flagIndex < 0)
        data->flagIndex = 0;
    else if (data->flagIndex > 4095)
        data->flagIndex = 4095;
    std::int32_t value = GetInternalFlag(data->flagIndex);
    if (g_inputBuffer[3].pressedPriority & 0x400U) {
        value ^= 1;
        if (data->markedFlag == -1)
            ClearOrSetShenmueFlags(data->flagIndex, value);
        else if (data->flagIndex >= data->markedFlag) {
            for (std::int32_t flag = data->markedFlag; flag <= data->flagIndex; ++flag)
                ClearOrSetShenmueFlags(flag, value);
        } else {
            for (std::int32_t flag = data->flagIndex; flag <= data->markedFlag; ++flag)
                ClearOrSetShenmueFlags(flag, value);
        }
    }
    SetDebugScreenTextPosition(2, 4);
    shenmue_s2_event_DebugLogToScreen("%d\n", static_cast<int>(data->flagIndex));
    if (value)
        shenmue_s2_event_DebugLogToScreen("ON");
    else
        shenmue_s2_event_DebugLogToScreen("OFF");
    if (data->markedFlag != -1) {
        SetDebugScreenTextPosition(2, 6);
        shenmue_s2_event_DebugLogToScreen("MARK %d", static_cast<int>(data->markedFlag));
    }
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SUPP_Callback(void *parameter) {
    SetDebugScreenTextPosition(2, 2);
    switch (currentDebugMenuPage) {
    case 1:
        shenmue_s2_event_DebugLogToScreen("[ STEP ]\n\n");
        DisplayStepInfo();
        break;
    case 2: {
        shenmue_s2_event_DebugLogToScreen("[ TIME ]\n\n");
        std::int32_t step = 1;
        if (g_inputBuffer[3].held & 0x400U)
            step = 2;
        if (g_inputBuffer[3].held & 0x200U)
            step *= 4;
        if (static_cast<std::int16_t>(g_inputBuffer[3].held) < 0) {
            shenAdjustCurrentTimeByMinutes(static_cast<std::int16_t>(step));
            shenmue_s2_event_DebugLogToScreen("+%d minute", static_cast<int>(step));
        }
        if (g_inputBuffer[3].held & 0x4000U) {
            shenAdjustCurrentTimeByMinutes(static_cast<std::int16_t>(-step));
            shenmue_s2_event_DebugLogToScreen("-%d minute", static_cast<int>(step));
        }
        if (g_inputBuffer[3].held & 2U) {
            shenSetTimeSimulationTo(0);
            shenmue_s2_event_DebugLogToScreen("time stop");
        }
        if (g_inputBuffer[3].held & 4U) {
            shenSetTimeSimulationTo(1);
            shenmue_s2_event_DebugLogToScreen("time move");
        }
        break;
    }
    case 4:
        shenmue_s2_event_DebugLogToScreen("[ FLAG ]\n");
        DisplayFlagInfo(static_cast<SupportTaskData *>(parameter));
        break;
    case 3:
        shenmue_s2_event_DebugLogToScreen("[ EVENT ]\n");
        AssignEventPrintPointers();
        break;
    case 5: {
        shenmue_s2_event_DebugLogToScreen("[ STEP ]\n");
        std::int32_t step = static_cast<std::int32_t>(GetCurrentStep());
        if ((g_inputBuffer[3].repeatedPresses & 0x1000U) && step <= 2999999)
            step += 10000;
        if ((g_inputBuffer[3].repeatedPresses & 0x2000U) && step > 9999)
            step -= 10000;
        if (static_cast<std::int16_t>(g_inputBuffer[3].repeatedPresses) < 0 && step <= 2999999)
            step += 100;
        if ((g_inputBuffer[3].repeatedPresses & 0x4000U) && step > 99)
            step -= 100;
        if ((g_inputBuffer[3].repeatedPresses & 4U) && step <= 2999999)
            ++step;
        if ((g_inputBuffer[3].repeatedPresses & 2U) && step > 0)
            --step;
        SetSTEP(step);
        break;
    }
    case 6:
        if (IsSaveAllowed())
            return;
        shenmue_s2_event_DebugLogToScreen("[ SAVE ]\n\nPRESS A BUTTON\n");
        if (g_inputBuffer[3].pressedPriority & 0x400U)
            StartShowScreenOfId(1);
        break;
    case 7:
        shenmue_s2_event_DebugLogToScreen("[ SYSTEM METRICS ]\n\n");
        DisplaySystemMetrics();
        break;
    }
    if (!currentDebugMenuPage) {
        if (g_inputBuffer[3].pressedPriority & 0x800U)
            currentDebugMenuPage = 1;
        return;
    }
    if (DebugMenuInputBlockedNullsub())
        return;
    if (g_inputBuffer[3].pressedPriority & 0x80U) {
        currentDebugMenuPage = static_cast<std::int32_t>(static_cast<std::uint32_t>(currentDebugMenuPage) + 1U);
        if (static_cast<std::uint32_t>(currentDebugMenuPage) > 7U)
            currentDebugMenuPage = 1;
    }
    if (g_inputBuffer[3].pressedPriority & 0x40U) {
        currentDebugMenuPage = static_cast<std::int32_t>(static_cast<std::uint32_t>(currentDebugMenuPage) - 1U);
        if (!currentDebugMenuPage)
            currentDebugMenuPage = 7;
    }
    if (g_inputBuffer[3].pressedPriority & 0x800U)
        currentDebugMenuPage = 0;
}
#endif

// @verified - d
void ResetGameSysActivity() {
    gameSysActivityFlags = 0;
    for (std::int32_t index = 0; index < 4; ++index)
        gameSysActivityCounters[index] = 0;
}

// @verified - d
void SetGameSysActivityFlag(std::int32_t index) {
    gameSysActivityFlags |= 1 << index;
}

// @verified - d
void ClearGameSysActivityFlag(std::int32_t index) {
    gameSysActivityFlags &= ~(1 << index);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GetGameSysActivityFlag(std::int32_t index) {
    return gameSysActivityFlags & (1 << index);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetScenarioTransientState(std::int32_t index, std::int32_t value) {
    if (index < 5)
        gameSysActivityCounters[index] = value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetScenarioTransientState(std::int32_t index) {
    if (index < 5)
        return gameSysActivityCounters[index];
    return 0;
}
#endif

// @verified - d
std::int32_t GetScenarioFlagNotificationShown() {
    return g_scenarioFlagNotificationShown;
}

// @verified - d
void UpdateGameSysActivity() {
    if (g_inputBuffer[0].held & 0xf000U)
        SetGameSysActivityFlag(0);
    else
        ClearGameSysActivityFlag(0);
    if (g_inputBuffer[0].held & 0x40U)
        SetGameSysActivityFlag(1);
    else
        ClearGameSysActivityFlag(1);
    if (g_inputBuffer[0].held & 0x80U)
        SetGameSysActivityFlag(2);
    else
        ClearGameSysActivityFlag(2);
    if (static_cast<std::uint8_t>(g_inputBuffer[0].leftStickX - 126) > 4 || g_inputBuffer[0].leftStickY <= 125 || g_inputBuffer[0].leftStickY > 130)
        SetGameSysActivityFlag(3);
    else
        ClearGameSysActivityFlag(3);
    if (camCtrlIsConversationActive())
        SetGameSysActivityFlag(4);
    else
        ClearGameSysActivityFlag(4);
    if (GetParentMenu() && !UpdateSelectorMemo() && gameSysActivityFlags) {
        gameSysActivityCounters[0] = static_cast<std::int32_t>(static_cast<std::uint32_t>(gameSysActivityCounters[0]) - 1U);
        gameSysActivityCounters[1] = static_cast<std::int32_t>(static_cast<std::uint32_t>(gameSysActivityCounters[1]) - 1U);
        gameSysActivityCounters[2] = static_cast<std::int32_t>(static_cast<std::uint32_t>(gameSysActivityCounters[2]) - 1U);
        gameSysActivityCounters[3] = static_cast<std::int32_t>(static_cast<std::uint32_t>(gameSysActivityCounters[3]) - 1U);
        gameSysActivityCounters[4] = static_cast<std::int32_t>(static_cast<std::uint32_t>(gameSysActivityCounters[4]) + 1U);
    }
}

// @verified - d
void InitializeGameInputSuppression() {
    inputSuppressionFrames = 0;
}

// @verified - d
void UpdateGameInputSuppression() {
    inputSuppressionFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(inputSuppressionFrames) - 1U);
    if (inputSuppressionFrames < 0)
        inputSuppressionFrames = 0;
    else {
        LegacyInputSlot &input = g_inputBuffer[0];
        input.held = 0;
        input.previousHeld = 0;
        input.pressedPriority = 0;
        input.rawPressed = 0;
        input.repeatedPresses = 0;
        input.triggers[0] = 0;
        input.triggers[1] = 0;
        input.leftStickX = 0x80;
        input.leftStickY = 0x80;
    }
}

// @verified - d
void SuppressGameInput(std::int32_t frames) {
    if (frames < 31)
        inputSuppressionFrames = frames;
}

// @verified - d
void GameSys(void *) {
    g_currentShenmue1SaveAreaId = GetShenmue1SaveAreaId(g_currentMap);
    ResetGameSysActivity();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    supportIptMaximum = 0;
    supportIptMinimum = 0x00ffffffU;
#else
    gameSysUnusedState = 0;
    gameSysUnusedColour = 0x00ffffffU;
#endif
    g_worldDrawingEnabled = 1;
    InitializeGameInputSuppression();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SUPP_Callback, 15, 2, 8, TASK_SUPP);
    std::int32_t *const support = static_cast<std::int32_t *>(HLib::GetTaskParameter(task));
    currentDebugMenuPage = 0;
    support[0] = 0;
    support[1] = -1;
    HLib::SetupTimedTask(task, 5);
    ResetSceneArchiveFileHandles();
    InitSceneEffects();
    MapControlTask_Enqueue(2);
    Queue_KeyRepeat(0);
    Queue_KeyRepeat(3);
    InitializeFrameVars();
    HLib::SetCurrentTaskTo(GameSys_Callback);
}

void GameSysNullsub() {
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AssignMapFacialClip(shen_name speaker, const std::int16_t *clip) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    scenAssignFacialClip(control->scenarioTask, clip, speaker);
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_IsScenarioBusy() {
    return shenmue::s2::IsScenarioBusy();
}
#endif

#endif
