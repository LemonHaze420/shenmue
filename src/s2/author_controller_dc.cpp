#include "s2/author_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstring>
#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
#include "s2/air_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/author_doors_dc.hpp"
#include "s2/author_table_dc.hpp"
#include "s2/author_cleanup_dc.hpp"
#include "s2/author_load_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/player_flags_dc.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/particle_effects.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "common/native_sqrt.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/aica.hpp"
#include <cmath>
#include "s2/author_character_state_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/fog_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/object_xforms.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/screen_film_bars_dc.hpp"
#include "s2/weather.hpp"
#endif
#include "s2/author_control_state_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/scenario_subtitles_dc.hpp"
#include "s2/scenario_actions_dc.hpp"
#include "s2/scenario_talk_dc.hpp"
#include "s2/scnw_dc.hpp"
#include "s2/window.hpp"
#include "s2/file_loader_dc.hpp"
#include <cstdio>
#include "s2/pak_dc.hpp"
#include "s2/camera_roll_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/weather_wind_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/hlib.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "s2/font.hpp"
#include "s2/generated_surface_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/frame_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/npc_data_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/date_time.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/math.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#endif
#include "s2/map_region_dc.hpp"
#include "s2/scenario_scnt.hpp"
#include "shenmue.h"
#include "s2/player_stats_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/camera_conversation_dc.hpp"
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/player_ground_state_dc.hpp"
#endif
#include "s2/calendar_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_region_data_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/maze_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/field_event_dc.hpp"
#include <cfloat>
#include "s2/conversation_fade_dc.hpp"
#include <cri_adxt.h>
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include "s2/ship_spray.hpp"
#include "s2/hiki.hpp"
#include "s2/event_character_state_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/flag_data.hpp"
#include "s2/inventory_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/flag_context_dc.hpp"
#endif
#include "s2/fight_blur_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/texture_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/save_screen_state_dc.hpp"
#include <cstdarg>
#endif
#include "s2/scene_data.hpp"

#define AUTH_VALUE(T, base, offset) (*reinterpret_cast<const T *>(reinterpret_cast<const std::byte *>(base) + (offset)))

namespace shenmue {
namespace s2 {

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
struct AuthTransformPose {
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
};
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
struct AuthPartsTaskData {
    std::int32_t state;
    const AuthDynamicCommand *command;
    Vector3 position;
    Vector3i rotation;
    Vector3 positionStep;
    Vector3i rotationStep;
};
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
struct AuthPartsCommand {
    AuthDynamicCommand header;
    std::uint32_t mode;
    std::uint32_t subject;
    std::int8_t parts[4];
};
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
typedef char AuthPartsTaskDataSizeCheck[sizeof(AuthPartsTaskData) == 0x38 ? 1 : -1];
#endif

void ScenarioTaskEntry_Callback(void *parameter);

void SCNF_Handler(WindowSlot *window, std::int32_t event, void *payload);

void shenPrepareDialogue(HLib::Task *scenarioTask, std::int32_t mode, const char *text, std::int32_t speakerMode);

void ProcessScenario(AuthorController *control);

AuthorController *AllocateScenarioControl(HLib::Task *task);

void RequestAuthorMaskCleanup();

std::int32_t authGetLoaded();

std::int32_t Author_GetCurrentFrame();

std::uint32_t Author_GetFramesRemaining();

void UnloadAuthorTableSlot(std::int32_t index);

void SetAuthorFilmBars(std::int32_t enabled);

std::int32_t GetAuthorFilmBars();

static std::int32_t AuthAnimatedObjectCommand(std::int32_t, const std::int32_t *, AuthActorContext *);

static std::int32_t AuthCameraCommand(std::int32_t, const std::int32_t *arguments, AuthActorContext *);

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void RemoveWeatherInstructionFlags(std::uint32_t flags);
#endif

static std::int32_t AuthTransformCommand(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *context);

std::int32_t ApplyAuthorCharacterMotion(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *context);

static std::int32_t AuthFaceCommand(std::int32_t, const std::int32_t *arguments, AuthActorContext *);

static std::int32_t AuthDialogueCommand(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *context);

static std::int32_t AuthSoundCommand(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *);

static AuthActorContext *FindOrCreateAuthorActorContext(shen_name subject);

void authDestroySubtitleConverseData(AuthorController *control);

void SCMS_Enqueue(AuthorController *control);

void Author_Update(AuthorController *control);

static const std::byte *DispatchAuthorTimestampGroup(const std::byte *cursor);

static void InitializeAuthorRuntime(const MapInfoChunk *data);

static const std::byte *FindAuthorChunk(const MapInfoChunk *bundle, std::uint32_t token);

void RegisterAuthorTracks(const MapInfoChunk *bundle, AuthorTrackSlots *tracks);

void ClearAuthorTableEntry(std::int32_t index);

void ClearAuthorTable();

void ClearAuthorPackageRecord(std::int32_t index);

void ClearAuthorPackageRecords();

static const MapInfoChunk *GetAuthorTableFile(std::int32_t index);

void LoadAuthorTableSlot(std::int32_t index, const char *filename, const char *directory);

static void InitializeAuthorDynamicDataForEntry(const AuthorTableEntry *entry);

void Author_UpdateDynamic();

void FinalizeAuthorSkip();

void RestoreAuthCharacterStates();

AuthSavedCharacter *RecordAuthCharacterState(shen_name subject, std::uint32_t value);

AuthSavedCharacter *FindAuthCharacterState(shen_name subject);

AuthSavedCharacter *SetAuthSavedCharacterFlags(shen_name subject, std::uint32_t flags);

AuthSavedCharacter *ClearAuthSavedCharacterFlags(shen_name subject, std::uint32_t flags);

std::int32_t RecordAuthorDoor(shen_name subject);

void RestoreAuthorDoors();

void AuthorMask_Callback(void *);

std::int32_t probeSceneById(std::int32_t id);

void sctlAuthApplyMotion();

std::int32_t PrepareAuthorHands(Character *character, shen_name subject);

static std::int32_t ExecuteAuthorMusicCommand(const char *name);
} // namespace s2
} // namespace shenmue
extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_EV_LoadAudio(const char *name, std::uint32_t sceneIndex);
namespace shenmue {
namespace s2 {

static void CaptureAuthorMapMasks();

void RestoreAuthorMapSlots();

static void ReleaseAuthorSplines(AuthSplineSet *splines);

void InitializeScenarioActionQueues(void *storage);

void scenUpdateFreeTalkRuntime(ScenarioEventContextPrefix *context);

ScenarioEventContextPrefix *InitializeScenarioEventContext(HLib::Task *task);

std::uint16_t ConvertWindowShiftJisCharacter(std::uint16_t character);

WindowSlot *AllocateWindowSlot(HLib::Task *task, const WindowSpecification &specification);

std::uint16_t StripWindowCharacterHighBits(std::uint16_t character);

void InitializeScenarioTextDecoder(std::int32_t mode, const char *text, ScenarioTextDecoder &decoder);

std::int32_t AdvanceScenarioTextDecoder(ScenarioTextDecoder &decoder);

void RecoverWindowSurface(WindowSlot *window);

void AttachWindowCallback(WindowSlot *window, void (*callback)(WindowSlot *, std::int32_t, void *), void *state);

void ClearWindowSlot(WindowSlot *window);

void ClearWindowText(WindowSlot *window);

void SetWindowColourMode(WindowSlot *window, std::int32_t mode);

void RevealWindowGlyphs(WindowSlot *window);

void AdvanceWindowLine(WindowSlot *window);

std::int32_t GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row);

std::int32_t WriteWindowCharacter(WindowSlot *window, std::uint16_t character, std::int32_t encodingMode);

void HideWindowSlot(WindowSlot *window);

void ShowWindowSlot(WindowSlot *window);

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void ReleaseWindowSurface(WindowSlot *window);

void scenReleaseSubtitleSurface(HLib::Task *task);
#endif

void InitializeScenarioActionQueues(void *storage);

void scenUpdateFreeTalkRuntime(ScenarioEventContextPrefix *context);

ScenarioEventContextPrefix *InitializeScenarioEventContext(HLib::Task *task);

void RequestScenarioWindowRecovery(HLib::Task *scenarioTask);

std::int32_t g_authorSkipAllowed, g_authorUnusedFlag, g_authorCannotSkip = 0;

std::int32_t g_authorForceFilmBars, g_authorReservedInitializationFlag = 0, g_authorHighQualityHands = 0;

std::int32_t g_authorReservedHandle = 0, g_authorReservedCounter = 0, g_authorHandAdmissionCount = 0;

shen_name g_authorHandAdmissionNames[32];

const void *g_authorCameraPlacement = 0;

HLib::Task *g_authorMaskTask;

AuthSubcontrollerSlot g_authorEyesControllers[32], g_authorPartsControllers[32];

Vector4 g_authorReflectorValues;

ScenarioEventContextPrefix *InitializeScenarioEventContext(HLib::Task *task);

void scenUpdateFreeTalkRuntime(ScenarioEventContextPrefix *context);

AuthorController *g_authorController;

AuthorTableEntry authorTable[70];

AuthorPackageRecord authorPackageRecords[6];

void SetSoundListenerUsesCamera(std::int32_t enabled);

std::uint32_t g_authorSavedWeatherInstruction, g_authorSavedPlayerFlags = 0;

std::uint32_t g_authorSavedMapModelMask, g_authorSavedMapFieldMask = 0;

std::int32_t g_authorApplyYawOffset;

AuthSmokeState g_authorSmoke;

AuthParticleState g_authorParticles;

std::int32_t g_authorSavedCharacterCount;

AuthSavedCharacter g_authorSavedCharacters[AUTHOR_SAVED_CHARACTER_COUNT];

std::int32_t g_authorControlledDoorCount;

shen_name g_authorControlledDoors[AUTHOR_CONTROLLED_DOOR_COUNT];

std::int32_t g_authorMaskDelay;

void ClearWindowText(WindowSlot *window);

void SetWindowColourMode(WindowSlot *window, std::int32_t mode);

void RevealWindowGlyphs(WindowSlot *window);

void InitializeScenarioTextDecoder(std::int32_t mode, const char *text, ScenarioTextDecoder &decoder);

std::int32_t AdvanceScenarioTextDecoder(ScenarioTextDecoder &decoder);

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void SetHighModelAndFaceFlagsForChara(Character *character, std::int32_t expression, std::int32_t frames, float scale);
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::uint32_t CheckMediatedDoublePress();
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
extern std::uint32_t g_completedGameCount;
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
const AuthDynamicCommand *g_authorDynamicCommands[26];
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
const AuthDynamicCommand *g_authorDynamicCommandHeads[26];
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::byte *g_authorHandCommands = 0;
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
const std::byte *g_authorHandIndices = 0;
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void SetSoundListenerUsesCamera(std::int32_t enabled);
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void SetUpperFacialExpression(Character *character, std::int32_t expression, std::int32_t frames, float scale);
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void SetLowerFacialExpression(Character *character, std::int32_t expression, std::int32_t frames, float scale);
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void SetHighModelAndFaceFlagsForChara(Character *character, std::int32_t expression, std::int32_t frames, float scale);
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t g_authorCaptureCharacterStates;
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static const float *g_authorCameraKeyCursor = 0;
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t g_authorCameraKeysRemaining = 0;
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static AuthCommandHandler authCommandHandlers[7] = {
    AuthAnimatedObjectCommand, AuthCameraCommand, AuthTransformCommand, ApplyAuthorCharacterMotion, AuthFaceCommand, AuthDialogueCommand, AuthSoundCommand};
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void SetMapFieldDepthSlotEnabled(std::int32_t enabled, std::int32_t index);
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static const std::int16_t authorPartNodeIds[104] = {-65, -66, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
    171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
    202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
    233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 150, 151};
#endif

void SetWindowColourMode(WindowSlot *window, std::int32_t mode);

void AdvanceWindowLine(WindowSlot *window);

std::int32_t GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row);

std::int32_t WriteWindowCharacter(WindowSlot *window, std::uint16_t character, std::int32_t encodingMode);

// @verified - d
HLib::Task *ScenarioTaskEntry_Enqueue(std::int8_t nextFunctionIndex) {
    HLib::Task *task;
    ScenarioTaskData *scenario;
    std::int32_t index;
    WindowSpecification specification;
    task = HLib::EnqueueTaskWithParameter(ScenarioTaskEntry_Callback, nextFunctionIndex, 2, sizeof(ScenarioTaskData), SCENARIO_TASK);
    scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(task));
    scenario->name = SCENARIO_TASK;
    scenario->characterTree = 0;
    for (index = 0; index < 13; ++index)
        scenario->characterBuckets[index] = 0;
    InitializeScenarioActionQueues(scenario->retainedScenarioData);
    scenario->scenarioWindow = AllocateSCNW(task);
    scenario->scenarioEventRuntime = InitializeScenarioEventContext(task);
    scenario->scenarioControl = AllocateScenarioControl(task);
    do {
        specification.flags = 0;
        specification.columns = specification.rows = 0;
        specification.field06 = specification.field08 = 0;
        specification.x = specification.y = 0;
        specification.horizontalGap = specification.verticalGap = -1;
        specification.width = specification.height = -1;
        specification.lineWidth = specification.lineHeight = -1;
    } while (0);
    specification.columns = 46;
    specification.rows = 2;
    specification.field06 = 0;
    specification.field08 = 384;
    specification.width = 640;
    specification.height = 96;
    specification.x = 21;
    specification.y = 1;
    specification.lineWidth = -1;
    specification.lineHeight = 30;
    scenario->windowDialog =
        AllocateWindowSlot(reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, specification);
    scenario->windowTextState.surfaceRecoveryFrames = 0;
    scenario->windowTextState.surfaceReady = 1;
    scenario->windowTextState.callbackEnabled = 1;
    scenario->windowTextState.lastWindowEvent = 0;
    scenario->windowTextState.columnCount = 46;
    scenario->windowTextState.rowCount = 2;
    scenario->windowTextState.scenarioStateFlags = 2;
    std::strcpy(scenario->subtitleText, "");
    AttachWindowCallback(scenario->windowDialog, SCNF_Handler, &scenario->windowTextState);
    return task;
}

// @verified - d
void ScenarioTaskEntry_Callback(void *parameter) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(parameter);
    scenUpdateFreeTalkRuntime(static_cast<ScenarioEventContextPrefix *>(scenario->scenarioEventRuntime));
    ProcessScenario(static_cast<AuthorController *>(scenario->scenarioControl));
}

// @verified - d
void CreateScenarioCharacterId(HLib::Task *scenarioTask, shen_name name, Character *character) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    ScenarioCharacterAssignment *const assignment =
        static_cast<ScenarioCharacterAssignment *>(HLib::AllocateTaskData(sizeof(ScenarioCharacterAssignment), MEMORY_CHID));
    assignment->tree.name = name;
    assignment->character = character;
    InsertNameTree(&scenario->characterTree, &assignment->tree);
    const std::uint32_t bucket = reinterpret_cast<std::uintptr_t>(character) % 13U;
    assignment->bucketNext = scenario->characterBuckets[bucket];
    scenario->characterBuckets[bucket] = assignment;
}

// @verified - d
void UnassignScenarioCharacter(HLib::Task *scenarioTask, Character *character) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    const std::uint32_t bucket = reinterpret_cast<std::uintptr_t>(character) % 13U;
    ScenarioCharacterAssignment **link = &scenario->characterBuckets[bucket];
    while (*link && (*link)->character != character)
        link = &(*link)->bucketNext;
    ScenarioCharacterAssignment *const assignment = *link;
    if (assignment) {
        *link = assignment->bucketNext;
        RemoveNameTree(&scenario->characterTree, &assignment->tree);
        HLib::FreeTaskData(assignment);
    }
}

// @verified - d
Character *scenFindChara(HLib::Task *scenarioTask, shen_name name) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    NameTreeNode *const node = FindNameTree(scenario->characterTree, name);
    if (!node)
        return 0;
    return reinterpret_cast<const ScenarioCharacterAssignment *>(node)->character;
}

// @verified - d
shen_name scenGetCharaName(HLib::Task *scenarioTask, const Character *character) {
    ScenarioTaskData *scenario;
    std::uint32_t bucket;
    scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    bucket = reinterpret_cast<std::uintptr_t>(character) % 13U;
    ScenarioCharacterAssignment **link = &scenario->characterBuckets[bucket];
    while (*link) {
        if ((*link)->character == character) {
            ScenarioCharacterAssignment *const assignment = *link;
            *link = assignment->bucketNext;
            assignment->bucketNext = scenario->characterBuckets[bucket];
            scenario->characterBuckets[bucket] = assignment;
            return assignment->tree.name;
        }
        link = &(*link)->bucketNext;
    }
    return shen_name();
}

// @verified - d
Character *ctrlGetScenarioChara(shen_name name) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return scenFindChara(control->scenarioTask, name);
}

// @verified - d
shen_name ctrlGetScenarioCharaName(const Character *character) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return scenGetCharaName(control->scenarioTask, character);
}

// @verified - d
std::int32_t FindScenarioTextOpeningQuote(std::int32_t mode, ScenarioTextDecoder &decoder) {
    std::uint16_t (*normalize)(std::uint16_t);
    if (!mode)
        normalize = ConvertWindowShiftJisCharacter;
    else
        normalize = StripWindowCharacterHighBits;
    while (AdvanceScenarioTextDecoder(decoder)) {
        if (decoder.currentCharacterWidth == 2) {
            const std::uint16_t code = normalize(decoder.currentCharacter);
            for (std::uint32_t index = 0; index < 1; ++index)
                if ((&kScenarioTextOpeningQuote)[index] == code)
                    return 1;
        } else if (!decoder.currentCharacterWidth) {
            return 0;
        }
    }
    return 0;
}

// @verified - d
bool IsScenarioTextLineBreak(std::uint32_t mode, std::uint16_t character) {
    if (!mode)
        character = ConvertWindowShiftJisCharacter(character);
    else
        character = StripWindowCharacterHighBits(character);
    if (character == kScenarioTextLineBreak)
        return 1;
    return 0;
}

// @verified - d
bool IsScenarioTextQuote(std::uint32_t mode, std::uint16_t character) {
    if (!mode)
        character = ConvertWindowShiftJisCharacter(character);
    else
        character = StripWindowCharacterHighBits(character);
    if (static_cast<std::uint16_t>(character - kScenarioTextOpeningQuote) < 2U)
        return 1;
    return 0;
}

// @verified - d
bool IsScenarioLineStartForbidden(std::uint32_t mode, std::uint16_t character) {
    if (!mode)
        character = ConvertWindowShiftJisCharacter(character);
    else
        character = StripWindowCharacterHighBits(character);
    const std::uint16_t *entry = kScenarioLineStartForbidden[character % 7U];
    while (*entry) {
        if (*entry == character)
            return true;
        ++entry;
    }
    return false;
}

// @verified - d
std::uint32_t GetScenarioTextControlCode(std::uint32_t mode, std::uint16_t character) {
    if (!mode)
        character = ConvertWindowShiftJisCharacter(character);
    else
        character = StripWindowCharacterHighBits(character);
    if (character == kScenarioTextEscape || character == kScenarioTextColourMode1 || character == kScenarioTextColourMode0)
        return character;
    return 0;
}

// @verified - d
void SCNF_RenderWindowText(WindowSlot *window, ScenarioWindowTextState *state) {
    std::int32_t escape = 0;
    ScenarioTextDecoder *const decoder = &state->decoder;
    std::int32_t column, row;
    do {
        if (state->decoder.currentCharacter) {
            GetWindowWritePosition(window, &column, &row);
            if (column == state->columnCount - 2) {
                if (decoder->currentCharacterWidth != 2 || !IsScenarioLineStartForbidden(decoder->legacyEncodingMode, state->decoder.currentCharacter))
                    AdvanceWindowLine(window);
            } else if (column > state->columnCount - 2) {
                AdvanceWindowLine(window);
            }
            ScenarioTextDecoder &current = state->decoder;
            if (current.currentCharacterWidth == 2) {
                if (IsScenarioTextLineBreak(current.legacyEncodingMode, current.currentCharacter)) {
                    GetWindowWritePosition(window, &column, &row);
                    AdvanceWindowLine(window);
                    continue;
                }
                ScenarioTextDecoder &text = state->decoder;
                if (IsScenarioTextQuote(text.legacyEncodingMode, state->decoder.currentCharacter))
                    continue;
                const std::int32_t control = GetScenarioTextControlCode(text.legacyEncodingMode, state->decoder.currentCharacter);
                switch (control) {
                case 0:
                    if (escape == kScenarioTextEscape) {
                        escape = control;
                        continue;
                    }
                    escape = control;
                    break;
                case kScenarioTextEscape:
                    if (escape != kScenarioTextEscape) {
                        escape = control;
                        continue;
                    }
                    escape = 0;
                    break;
                case kScenarioTextColourMode1:
                    if (escape != kScenarioTextEscape) {
                        escape = 0;
                        break;
                    }
                    SetWindowColourMode(window, 1);
                    escape = 0;
                    if (GetSubtitleSettingEnabled())
                        continue;
                    state->scenarioStateFlags |= SSF_TextComplete;
                    return;
                case kScenarioTextColourMode0:
                    if (escape != kScenarioTextEscape) {
                        escape = 0;
                        break;
                    }
                    SetWindowColourMode(window, 0);
                    escape = 0;
                    if (GetSubtitleSettingEnabled())
                        continue;
                    state->scenarioStateFlags |= SSF_TextComplete;
                    return;
                default:
                    escape = control;
                    break;
                }
            }
        }
        if (static_cast<std::uint32_t>(WriteWindowCharacter(window, state->decoder.currentCharacter, decoder->legacyEncodingMode)) & 0x8000U)
            return;
    } while (AdvanceScenarioTextDecoder(state->decoder));
    state->scenarioStateFlags |= SSF_TextComplete;
}

// @verified - d
static void SCNF_RenderWindowTextAlternate(WindowSlot *window, ScenarioWindowTextState *state) {
    std::int32_t escape = 0;
    ScenarioTextDecoder *const decoder = &state->decoder;
    std::int32_t column, row;
    do {
        if (state->decoder.currentCharacter) {
            GetWindowWritePosition(window, &column, &row);
            if (column == state->columnCount - 2) {
                if (decoder->currentCharacterWidth != 2 || !IsScenarioLineStartForbidden(decoder->legacyEncodingMode, state->decoder.currentCharacter))
                    AdvanceWindowLine(window);
            } else if (column > state->columnCount - 2) {
                AdvanceWindowLine(window);
            }
            ScenarioTextDecoder &current = state->decoder;
            if (current.currentCharacterWidth == 2) {
                if (IsScenarioTextLineBreak(current.legacyEncodingMode, current.currentCharacter)) {
                    GetWindowWritePosition(window, &column, &row);
                    AdvanceWindowLine(window);
                    continue;
                }
                ScenarioTextDecoder &text = state->decoder;
                if (IsScenarioTextQuote(text.legacyEncodingMode, state->decoder.currentCharacter))
                    continue;
                const std::int32_t control = GetScenarioTextControlCode(text.legacyEncodingMode, state->decoder.currentCharacter);
                switch (control) {
                case 0:
                    if (escape == kScenarioTextEscape) {
                        escape = control;
                        continue;
                    }
                    escape = control;
                    break;
                case kScenarioTextEscape:
                    if (escape != kScenarioTextEscape) {
                        escape = control;
                        continue;
                    }
                    escape = 0;
                    break;
                case kScenarioTextColourMode1:
                    if (escape != kScenarioTextEscape) {
                        escape = 0;
                        break;
                    }
                    SetWindowColourMode(window, 1);
                    escape = 0;
                    if (GetSubtitleSettingEnabled())
                        continue;
                    state->scenarioStateFlags |= SSF_TextComplete;
                    return;
                case kScenarioTextColourMode0:
                    if (escape != kScenarioTextEscape) {
                        escape = 0;
                        break;
                    }
                    SetWindowColourMode(window, 0);
                    escape = 0;
                    if (GetSubtitleSettingEnabled())
                        continue;
                    state->scenarioStateFlags |= SSF_TextComplete;
                    return;
                default:
                    escape = control;
                    break;
                }
            }
        }
        if (static_cast<std::uint32_t>(WriteWindowCharacter(window, state->decoder.currentCharacter, decoder->legacyEncodingMode)) & 0x8000U)
            return;
    } while (AdvanceScenarioTextDecoder(state->decoder));
    state->scenarioStateFlags |= SSF_TextComplete;
}

// @verified - d
void SCNF_Handler(WindowSlot *window, std::int32_t event, void *payload) {
    ScenarioWindowTextState *const state = static_cast<ScenarioWindowTextState *>(payload);
    if (event)
        state->lastWindowEvent = event;
    if (!state->callbackEnabled || !(state->scenarioStateFlags & SSF_Preparing))
        return;
    switch (static_cast<std::uint32_t>(event)) {
    case 6:
        if (state->scenarioStateFlags & SSF_AdvanceRequested) {
            ShowWindowSlot(window);
            state->scenarioStateFlags ^= SSF_AdvanceRequested;
        } else {
            state->scenarioStateFlags |= SSF_PageReady;
        }
        break;
    case 7:
        if ((state->scenarioStateFlags & SSF_TextComplete) == 0) {
            ClearWindowSlot(window);
            SCNF_RenderWindowText(window, state);
            RevealWindowGlyphs(window);
        } else {
            state->scenarioStateFlags |= 2U;
        }
        break;
    case 0: {
        const std::int32_t frames = state->surfaceRecoveryFrames;
        if (frames > 0 && !state->surfaceReady) {
            state->surfaceRecoveryFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(frames) + 1U);
            if (frames > 4) {
                RecoverWindowSurface(window);
                state->surfaceRecoveryFrames = 0;
                state->surfaceReady = 1;
            }
        }
        break;
    }
    }
}

// @verified - d
void SCEN_ShowEventMessage(HLib::Task *scenarioTask, std::int32_t mode, const char *message) {
    shenPrepareDialogue(scenarioTask, mode, message, 2);
}

// @verified - d
void shenPrepareDialogue(HLib::Task *scenarioTask, std::int32_t mode, const char *text, std::int32_t speakerMode) {
    ScenarioTaskData *scenario;
    do {
        scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    } while (0);
    WindowSlot *const window = scenario->windowDialog;
    ScenarioWindowTextState &state = scenario->windowTextState;
    if (!state.surfaceReady) {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        RequestScenarioWindowRecovery(control->scenarioTask);
    }
    if (!GetSubtitleSettingEnabled() && !(state.scenarioStateFlags & SSF_ForceSubtitles) && speakerMode != 2) {
        state.scenarioStateFlags = SSF_Suppressed;
        return;
    }
    state.scenarioStateFlags = SSF_Preparing;
    state.lastWindowEvent = 0;
    ClearWindowText(window);
    SetWindowColourMode(window, speakerMode);
    char *const subtitle = reinterpret_cast<char *>(&state + 1);
    std::strcpy(subtitle, text);
    InitializeScenarioTextDecoder(mode, subtitle, state.decoder);
    if (!FindScenarioTextOpeningQuote(mode, state.decoder))
        InitializeScenarioTextDecoder(mode, subtitle, state.decoder);
    AdvanceScenarioTextDecoder(state.decoder);
    SCNF_RenderWindowText(window, &state);
    RevealWindowGlyphs(window);
    state.scenarioStateFlags |= SSF_AdvanceRequested;
}

// @verified - d
void shenAppendDialogue(HLib::Task *scenarioTask, std::int32_t mode, const char *text, std::int32_t speakerMode) {
    ScenarioTaskData *scenario;
    do {
        scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(scenarioTask));
    } while (0);
    WindowSlot *const window = scenario->windowDialog;
    ScenarioWindowTextState &state = scenario->windowTextState;
    if (!state.surfaceReady) {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        RequestScenarioWindowRecovery(control->scenarioTask);
    }
    if (!GetSubtitleSettingEnabled() && !(state.scenarioStateFlags & SSF_ForceSubtitles) && speakerMode != 2) {
        state.scenarioStateFlags = SSF_Suppressed;
        return;
    }
    state.scenarioStateFlags = SSF_Preparing;
    state.lastWindowEvent = 0;
    SetWindowColourMode(window, speakerMode);
    char *const subtitle = reinterpret_cast<char *>(&state + 1);
    std::strcpy(subtitle, text);
    InitializeScenarioTextDecoder(mode, subtitle, state.decoder);
    if (!FindScenarioTextOpeningQuote(mode, state.decoder))
        InitializeScenarioTextDecoder(mode, subtitle, state.decoder);
    AdvanceScenarioTextDecoder(state.decoder);
    SCNF_RenderWindowTextAlternate(window, &state);
    RevealWindowGlyphs(window);
    state.scenarioStateFlags |= SSF_AdvanceRequested;
}

// @verified - d
void scenDestroySubtitles(HLib::Task *task) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(task));
    WindowSlot *const dialog = scenario->windowDialog;
    ScenarioWindowTextState *const state = &scenario->windowTextState;
    state->scenarioStateFlags = SSF_InactiveMask;
    HideWindowSlot(dialog);
    std::strcpy(reinterpret_cast<char *>(state + 1), "");
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void scenReleaseSubtitleSurface(HLib::Task *task) {
    ScenarioTaskData *const scenario = static_cast<ScenarioTaskData *>(HLib::GetTaskParameter(task));
    WindowSlot *const dialog = scenario->windowDialog;
    ScenarioWindowTextState *const state = &scenario->windowTextState;
    state->scenarioStateFlags = SSF_InactiveMask;
    ReleaseWindowSurface(dialog);
    state->surfaceReady = 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void InitializeAuthorSplines(AuthSplineSet *splines) {
    for (std::int32_t index = 0; index < 9; ++index)
        if (splines->tracks[index].count)
            InitializeAuthorSplineTrack(splines->tracks[index], 0, splines->tracks[index].count, splines->tracks[index].times, splines->tracks[index].values,
                splines->tracks[index].tangents, 0);
    splines->frame = 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t EvaluateAuthorSpline(AuthSplineSet *splines, AuthTransformPose *pose, const Matrix4x4 *placement) {
    float values[9];
    std::int32_t result = 0;
    float time = static_cast<float>(splines->frame);
    time /= 30.0f;
    splines->frame++;
    for (std::int32_t index = 0; index < 9; ++index)
        if (splines->tracks[index].count)
            result |= DoKeyframeCubicHermiteInterpolation(splines->tracks[index], time, &values[index]);
    pose->position.x = values[0];
    pose->position.y = values[1];
    pose->position.z = values[2];
    pose->rotation.x = static_cast<std::int32_t>(values[3] * 8192.0f / 45.0f) & 0xffff;
    pose->rotation.y = static_cast<std::int32_t>(values[4] * 8192.0f / 45.0f) & 0xffff;
    pose->rotation.z = static_cast<std::int32_t>(values[5] * 8192.0f / 45.0f) & 0xffff;
    pose->scale.x = values[6];
    pose->scale.y = values[7];
    pose->scale.z = values[8];
    if (placement) {
        Matrix4x4 transform;
        std::int32_t magnitude[3];
        std::int32_t angle[3];
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(placement);
        Matrix4x4::RotateAroundZ_Wrapper(pose->rotation.z);
        Matrix4x4::RotateAroundY_Wrapper(pose->rotation.y);
        Matrix4x4::RotateAroundX_Wrapper(pose->rotation.x);
        matrix_stack::GetMatrix(&transform);
        PopCurrentMatrix(1);
        const float sine = -transform.rows[0].z;
        if (sine > 0.999999f) {
            angle[0] = dc::sdk::ArcTan2Fixed(transform.rows[1].x, transform.rows[1].y);
            angle[1] = 0x4000;
            angle[2] = 0;
        } else if (sine < -0.999999f) {
            angle[0] = dc::sdk::ArcTan2Fixed(-transform.rows[1].x, transform.rows[1].y);
            angle[1] = 0xc000;
            angle[2] = 0;
        } else {
            const float cosine = shenmue::NativeSqrt(1.0f - sine * sine);
            const float sineX = transform.rows[1].z / cosine;
            const float cosineX = transform.rows[2].z / cosine;
            const float sineZ = transform.rows[0].y / cosine;
            const float cosineZ = transform.rows[0].x / cosine;
            angle[0] = dc::sdk::ArcTan2Fixed(sineX, cosineX);
            angle[1] = dc::sdk::ArcTan2Fixed(sine, cosine);
            angle[2] = dc::sdk::ArcTan2Fixed(sineZ, cosineZ);
        }
        if (static_cast<std::int16_t>(angle[0]) < 0 && static_cast<std::int16_t>(angle[2]) < 0) {
            angle[0] = static_cast<std::uint16_t>(angle[0]) - 0x8000;
            angle[1] = 0x8000 - static_cast<std::uint16_t>(angle[1]);
            angle[2] = static_cast<std::uint16_t>(angle[2]) - 0x8000;
        }
        std::int32_t alternateWide = 0;
        std::int32_t wide = 0;
        magnitude[0] = angle[0] & 0xffff;
        if (magnitude[0] & 0x8000)
            magnitude[0] = -(magnitude[0] | 0xffff0000);
        magnitude[1] = angle[1] & 0xffff;
        if (magnitude[1] & 0x8000)
            magnitude[1] = -(magnitude[1] | 0xffff0000);
        magnitude[2] = angle[2] & 0xffff;
        if (magnitude[2] & 0x8000)
            magnitude[2] = -(magnitude[2] | 0xffff0000);
        if (magnitude[0] > 0x4000)
            wide++;
        if (magnitude[1] > 0x4000)
            wide++;
        if (magnitude[2] > 0x4000)
            wide++;
        magnitude[0] = (angle[0] - 0x8000) & 0xffff;
        if (magnitude[0] & 0x8000)
            magnitude[0] = -(magnitude[0] | 0xffff0000);
        magnitude[1] = (0x8000 - angle[1]) & 0xffff;
        if (magnitude[1] & 0x8000)
            magnitude[1] = -(magnitude[1] | 0xffff0000);
        magnitude[2] = (angle[2] - 0x8000) & 0xffff;
        if (magnitude[2] & 0x8000)
            magnitude[2] = -(magnitude[2] | 0xffff0000);
        if (magnitude[0] > 0x4000)
            alternateWide++;
        if (magnitude[1] > 0x4000)
            alternateWide++;
        if (magnitude[2] > 0x4000)
            alternateWide++;
        if (wide > alternateWide) {
            angle[0] -= 0x8000;
            angle[1] = 0x8000 - angle[1];
            angle[2] -= 0x8000;
        }
        pose->rotation.x = (angle[0] & 0x8000) ? (angle[0] | 0xffff0000) : (angle[0] & 0xffff);
        pose->rotation.y = (angle[1] & 0x8000) ? (angle[1] | 0xffff0000) : (angle[1] & 0xffff);
        pose->rotation.z = (angle[2] & 0x8000) ? (angle[2] | 0xffff0000) : (angle[2] & 0xffff);
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(placement);
        Matrix4x4::TransformPoint(pose->position, pose->position);
        matrix_stack::LoadMatrix(placement);
        Matrix4x4::TransformPoint(pose->scale, pose->scale);
        PopCurrentMatrix(1);
    }
    return result != 0;
}
#endif

std::int32_t ResetScenarioActionQueue(std::uint32_t index, std::int32_t mode) {
    ScenarioActionQueue *const queue = &scenarioActionQueueStorage[index];
    if (index > 1 || mode != 1)
        return -1;
    queue->count = 0;
    queue->read = queue->write = queue->storage;
    *reinterpret_cast<std::uint32_t *>(queue->storage) = 0;
    return 0;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t EV_authSceneCtrl(std::int32_t operation) {
    switch (operation) {
    case -1:
        return authGetLoaded();
    case -2:
        g_authorController->authorStopped = 1;
        return 1;
    case -3:
        g_authorController->authorStopped = 0;
        return 1;
    case -4:
        authDestroySubtitleConverseData(g_authorController);
        SCMS_Enqueue(g_authorController);
        return 1;
    case -5:
        return static_cast<std::int32_t>(Author_GetFramesRemaining());
    case -6:
        return Author_GetCurrentFrame();
    case -7:
        SetAuthorFilmBars(1);
        return 1;
    case -8:
        SetAuthorFilmBars(0);
        return 0;
    case -9:
        return GetAuthorFilmBars();
    case -10:
        g_authorSkipAllowed = 1;
        return 1;
    case -11:
        g_authorSkipAllowed = 0;
        return 1;
    case -12:
        return g_authorSkipAllowed;
    case -13:
        return g_authorCannotSkip;
    case -14:
        g_authorHighQualityHands = 0;
        return 1;
    case -15:
        if (probeSceneById(operation) >= 0)
            return 1;
        return 0;
    case -16:
        g_authorUnusedFlag = 1;
        return 1;
    case -17:
        g_authorApplyYawOffset = 1;
        return 1;
    default:
        SCMS_Enqueue(g_authorController);
        if (static_cast<std::uint32_t>(operation) + 100U < 100U)
            return 0;
        if (operation > 70) {
            operation = probeSceneById(operation);
            if (operation == -1)
                return 0;
        }
        g_authorController->track = static_cast<std::int8_t>(operation);
        g_authorController->skipCountdownFinished = 0;
        g_authorController->finished = 0;
        if (g_authorMaskTask) {
            HLib::DestroyTaskInternal(g_authorMaskTask);
            g_authorMaskDelay = 0;
            g_authorMaskTask = 0;
        }
        if (g_authorForceFilmBars)
            filmSetCinemaBarVisibility(reinterpret_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask, FF_ForceShow, 0);
        else
            filmSetCinemaBarVisibility(reinterpret_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask, FF_Force, 0);
        InitializeAuthorRuntime(GetAuthorTableFile(operation));
        InitializeAuthorDynamicDataForEntry(&authorTable[operation]);
        if (HasSavedAreaJumpLocation())
            ResetGlobalStateFlags();
        return 1;
    }
}
#endif

// @verified - d
void SetAuthorTableFile(std::int32_t index, const char *filename, const char *directory) {
    if (static_cast<std::uint32_t>(index) + 100U > 99U) {
        if (filename)
            LoadAuthorTableSlot(index, filename, directory);
        else
            UnloadAuthorTableSlot(index);
    }
}

// @verified - d
bool FindAuthor(shen_name id) {
    if (probeSceneById(static_cast<std::int32_t>(id)) < 0)
        return false;
    return true;
}

// @verified - d
void SetAuthorCameraPlacement(const void *placement) {
    g_authorCameraPlacement = placement;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void ProcessScenario(AuthorController *control) {
    g_authorController = control;
    if (control->loaded) {
        if (!control->skipCountdownFinished && control->skipToNext) {
            if (--control->skipToNext == 0) {
                RequestAuthorMaskCleanup();
                g_authorController->skipCountdownFinished = 1;
            }
        } else {
            Author_Update(g_authorController);
        }
        Author_UpdateDynamic();
        if (!g_authorController->finished && CheckMediatedDoublePress() && (g_authorCannotSkip || (g_authorSkipAllowed && g_completedGameCount))) {
            g_authorController->finished = 1;
            g_authorController->skipToNext = 2;
            scenCvsCtrlFree(0, 20);
            const MapControlData *const map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            scenDestroySubtitles(map->scenarioTask);
            FinalizeAuthorSkip();
        }
    }
    if (g_authorController->restartTrack && g_authorController->track >= 0) {
        g_authorController->restartTrack = 0;
        EV_authSceneCtrl(g_authorController->track);
    }
}
#endif

AuthorController *AllocateScenarioControl(HLib::Task *task) {
    AuthorController *const control = g_authorController =
        static_cast<AuthorController *>(HLib::AllocateTaskData(sizeof(AuthorController), AUTHOR_CONTROL_STORAGE));
    control->task = task;
    control->loaded = 0;
    control->restartTrack = 0;
    control->needsInitAuthorMask = 0;
    control->track = -1;
    control->skipToNext = 0;
    control->skipCountdownFinished = 0;
    control->finished = 0;
    control->dialogueFrames = 0;
    control->dialogueCharacter = 0;
    control->dialogueState = 0;
    g_authorSkipAllowed = 1;
    g_authorUnusedFlag = 0;
    g_authorCannotSkip = 0;
    g_authorCameraPlacement = 0;
    std::int32_t audioCountdownSize = sizeof(control->audioCountdown);
    std::memset(control->audioCountdown, 0, audioCountdownSize);
    g_authorForceFilmBars = 1;
    g_authorReservedInitializationFlag = 1;
    g_authorMaskTask = 0;
    g_authorReservedHandle = -1;
    g_authorHighQualityHands = 1;
    g_authorReservedCounter = 0;
    std::memset(g_authorEyesControllers, 0, sizeof(g_authorEyesControllers));
    std::memset(g_authorPartsControllers, 0, sizeof(g_authorPartsControllers));
    return control;
}

// @verified - d
void RequestAuthorTrackRestart() {
    if (g_authorController) {
        g_authorController->restartTrack = 1;
        g_authorController->needsInitAuthorMask = 0;
    }
}

// @verified - d
void authResetLoadFlag() {
    if (g_authorController)
        reinterpret_cast<std::int8_t *>(g_authorController)[kAuthorLoadedFlagOffset] = 0;
}

// @verified - d
void RequestAuthorMaskCleanup() {
    if (g_authorController) {
        g_authorController->restartTrack = 0;
        g_authorController->needsInitAuthorMask = 1;
    }
}

// @verified - d
std::int32_t authGetLoaded() {
    if (g_authorController)
        return reinterpret_cast<const std::int8_t *>(g_authorController)[kAuthorLoadedFlagOffset];
    return 0;
}

// @verified - d
std::int32_t Author_GetCurrentFrame() {
    return g_authorController && g_authorController->loaded ? g_authorController->currentFrame : -1;
}

// @verified - d
std::uint32_t Author_GetFramesRemaining() {
    return g_authorController && g_authorController->loaded ? g_authorController->framesRemaining : UINT32_MAX;
}

// @verified - d
std::int32_t Author_GetTrack() {
    return g_authorController && g_authorController->track >= 0 ? g_authorController->track : -1;
}

// @verified - d
std::uint32_t authGetFinished() {
    return g_authorController && g_authorController->loaded ? g_authorController->finished : 0;
}

// @verified - d
void SCEX_Reader(const MapInfoChunk *bundle) {
    ClearAuthorTable();
    ClearAuthorPackageRecords();
    if (bundle)
        RegisterAuthorTracks(bundle, 0);
}

// @verified - d
std::int32_t ReadAUTH(const std::byte *bundle, const void *package) {
    std::int32_t index = 0;
    while (index < 6 && authorPackageRecords[index].package)
        ++index;
    if (index == 6)
        return 0;
    authorPackageRecords[index].package = package;
    RegisterAuthorTracks(reinterpret_cast<const MapInfoChunk *>(bundle), &authorPackageRecords[index].tracks);
    return 1;
}

// @verified - d
std::int32_t FreeAUTH(const void *package) {
    std::int32_t index, slot;
    if (!package)
        return 0;
    for (index = 0; index < 6; ++index)
        if (authorPackageRecords[index].package == package)
            break;
    if (index == 6)
        return 0;
    for (slot = 0; slot < authorPackageRecords[index].tracks.count; ++slot)
        ClearAuthorTableEntry(authorPackageRecords[index].tracks.indices[slot]);
    ClearAuthorPackageRecord(index);
    return 1;
}

// @verified - d
void *LoadPackageAuthor(void *data, PakGlobalData *package) {
    ReadAUTH(reinterpret_cast<const std::byte *>(data), package);
    return data;
}

// @verified - d
void ReleasePackageAuthor(PakGlobalData *package) {
    FreeAUTH(package);
}

// @verified - d
void UnloadAuthorTableSlot(std::int32_t index) {
    if (authorTable[index].fileOwned && (g_authorController->track != index || !g_authorController->loaded))
        ClearAuthorTableEntry(index);
}

// @verified - d
void SetAuthorFilmBars(std::int32_t enabled) {
    g_authorForceFilmBars = enabled;
}

// @verified - d
std::int32_t GetAuthorFilmBars() {
    return g_authorForceFilmBars;
}

// @verified - d
HLib::Task *authGetCurrentTask() {
    return g_authorController->task;
}

// @verified - d
void ResetAuthorDialogueState() {
    if (g_authorController) {
        g_authorController->dialogueFrames = 0;
        g_authorController->dialogueCharacter = 0;
        g_authorController->dialogueState = 0;
    }
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void SetAuthorActorRegionFlag(AuthActorContext *context, MotionWork *work) {
    if (!context->motionType) {
        std::int32_t clip = 0x80;
        context->clipState = clip;
        const std::uint32_t set = mtwkSetRegionFlag(work, 0x44, 1, LowerBody) != 0;
        context->clipState = set | clip;
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void ClearAuthorMotionCutState(AuthActorContext *context, MotionWork *) {
    if (!context->motionType && (context->clipState & 0x80U))
        context->clipState = 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t AuthAnimatedObjectCommand(std::int32_t, const std::int32_t *, AuthActorContext *) {
    Vector3 eye, target;
    camCtrlGetNextCamPos(&eye);
    camCtrlGetTargetPosition(&target);
    return 1;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t AuthCameraCommand(std::int32_t, const std::int32_t *arguments, AuthActorContext *) {
    const std::byte *const table = g_authorController->camera;
    const std::int32_t offset = arguments[1] * 4;
    const std::byte *const offsets = table + 0x10;
    const std::byte *const camera = table + *reinterpret_cast<const std::int32_t *>(offsets + offset);
    const void *request[3];
    request[0] = camera;
    g_authorCameraKeysRemaining = reinterpret_cast<const std::int32_t *>(camera)[1];
    const float *const keys = reinterpret_cast<const float *>(camera + 8);
    g_authorCameraKeyCursor = keys;
    request[2] = 0;
    request[1] = 0;
    camCtrlToggleSplineRebuildFlag(3, request);
    camCtrlSetState(8);
    return 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t AuthTransformCommand(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *context) {
    Character *const character = scenFindChara(g_authorController->task, shen_name(arguments[0]));
    AuthSplineSet *const splines = &context->splines;
    AuthTransformPose pose;
    AuthSavedCharacter *state;
    switch (phase) {
    case 0: {
        const std::byte *const motion = g_authorController->motion;
        const std::int32_t offset = arguments[1] * 4;
        const std::byte *const offsets = motion + 0x10;
        const std::byte *cursor = motion + *reinterpret_cast<const std::int32_t *>(offsets + offset);
        for (std::int32_t index = 0; index < 9; ++index) {
            if (index < 6 || reinterpret_cast<const std::int8_t *>(g_authorController->sequence)[9] >= 5) {
                splines->tracks[index].count = *reinterpret_cast<const std::int32_t *>(cursor);
                cursor += 4;
                splines->tracks[index].times = reinterpret_cast<const float *>(cursor);
                cursor += splines->tracks[index].count * 4;
                splines->tracks[index].values = reinterpret_cast<const float *>(cursor);
                cursor += splines->tracks[index].count * 4;
                splines->tracks[index].tangents = reinterpret_cast<const float *>(cursor);
                cursor += splines->tracks[index].count * 4;
            } else {
                splines->tracks[index].count = 0;
                splines->tracks[index].times = 0;
                splines->tracks[index].values = 0;
                splines->tracks[index].tangents = 0;
            }
        }
        context->position.x = splines->tracks[0].values[0];
        context->position.y = splines->tracks[1].values[0];
        context->position.z = splines->tracks[2].values[0];
        context->rotation.x = static_cast<std::int32_t>(splines->tracks[3].values[0] * 8192.0f / 45.0f) & 0xffff;
        context->rotation.y = static_cast<std::int32_t>(splines->tracks[4].values[0] * 8192.0f / 45.0f) & 0xffff;
        context->rotation.z = static_cast<std::int32_t>(splines->tracks[5].values[0] * 8192.0f / 45.0f) & 0xffff;
        if (splines->tracks[6].count) {
            context->scale.x = splines->tracks[6].values[0];
            context->scale.y = splines->tracks[7].values[0];
            context->scale.z = splines->tracks[8].values[0];
        }
        InitializeAuthorSplines(splines);
        context->flags |= 1U;
        context->setupCountdown = 0;
        EvaluateAuthorSpline(splines, &pose, static_cast<const Matrix4x4 *>(g_authorCameraPlacement));
        EvaluateAuthorSpline(splines, &pose, static_cast<const Matrix4x4 *>(g_authorCameraPlacement));
        SetCharacterLocalPosition(character, &pose.position);
        SetCharacterLocalRotation(character, &pose.rotation);
        if (splines->tracks[6].count)
            SetCharacterMotionTarget(character, 7, &pose.scale);
        if (g_authorCaptureCharacterStates && character) {
            const std::int32_t noMotion = HasCharacterMotionWork(character) == 0;
            state = RecordAuthCharacterState(shen_name(arguments[0]), noMotion);
            if (state) {
                state->pluralState = static_cast<std::int16_t>(SetPluralControlFor(character, 0));
                state->displayEnabled = IsOfThisVisibilityType(character, CVM_DisplayEnabled);
                state->controlFlags = ccmClearFlags(character, 15);
                if (noMotion)
                    return 1;
                state = FindAuthCharacterState(shen_name(arguments[0]));
                state->reflectorSector = static_cast<std::int8_t>(GetReflectorBoardSector(character));
                GetReflectorBoardValues(character, state->reflectorValues);
                state->motionSnapshot = static_cast<std::int8_t>(CaptureMotionSnapshot(shen_name(arguments[0])));
                state->storedShadowY = static_cast<std::int8_t>(GetCharacterType(character));
                state->drawControl = GetCharacterDrawControlEnabled(character);
                state->unknown40 = charaCheckFlags(character);
                Vector4 reflector;
                reflector.x = 0.3f;
                reflector.y = 0.3f;
                reflector.z = 0.3f;
                reflector.w = 0.3f;
                const Vector3 eyeTarget = {0.0f, 0.0f, 0.0f};
                MotionWork *const work = charFindMotionwork(character);
                charaToggleFlag(character, 0);
                SelectCharacterFacialNode(character, 1);
                SetHighModelAndFaceFlagsForChara(character, 0, 0, 1.0f);
                SetFacialBlinkDelay(character, 0);
                AT_SetEyeCtrl(character, 0.0f, 0.0f, 0.0f);
                hmdlSetFlagsFor(character, 2, 8, 0);
                mtwkCopyTransition(work, 0, 0, 1, 0);
                mtwkSetRequestedRootMode(work, 0);
                mtwkUpdateWithPosition(work, 0, 0);
                SetEyeToChara(character, &eyeTarget, 0, 0);
                ResetReflectorBoardMode(character, 1);
                SetReflectorBoardValues(character, &reflector);
                if (g_authorHighQualityHands && PrepareAuthorHands(character, shen_name(arguments[0]))) {
                    CharacterHandComponent *hand = AllocateHNDR(character);
                    if (hand) {
                        if (!IsCharacterHandRegionEnabled(hand))
                            SetCharacterHandRegionEnabled(hand, 1, 0, 1);
                        else
                            state->flags |= AuthRestoreRightHand;
                    }
                    hand = AllocateHNDL(character);
                    if (hand) {
                        if (!IsCharacterHandRegionEnabled(hand))
                            SetCharacterHandRegionEnabled(hand, 1, 0, 1);
                        else
                            state->flags |= AuthRestoreLeftHand;
                    }
                }
            }
        }
        return 1;
    }
    case 1:
        if (*reinterpret_cast<std::int8_t *>(&context->setupCountdown)) {
            --context->setupCountdown;
            return 1;
        }
        if (!EvaluateAuthorSpline(splines, &pose, static_cast<const Matrix4x4 *>(g_authorCameraPlacement)))
            return 0;
        state = FindAuthCharacterState(shen_name(arguments[0]));
        if (state->flags & AuthRestoreObjectXForms)
            return 1;
        context->position.x = pose.position.x;
        context->position.y = pose.position.y;
        context->position.z = pose.position.z;
        context->rotation.x = pose.rotation.x;
        context->rotation.y = pose.rotation.y;
        context->rotation.z = pose.rotation.z;
        context->scale.x = pose.scale.x;
        context->scale.y = pose.scale.y;
        context->scale.z = pose.scale.z;
        SetCharacterLocalPosition(character, &pose.position);
        SetCharacterLocalRotation(character, &pose.rotation);
        if (splines->tracks[6].count)
            SetCharacterMotionTarget(character, 7, &pose.scale);
        return 1;
    case 4: {
        std::int32_t active = 0;
        while (splines->frame < static_cast<std::int32_t>(g_authorController->framesRemaining))
            active = EvaluateAuthorSpline(splines, &pose, static_cast<const Matrix4x4 *>(g_authorCameraPlacement));
        if (active) {
            context->position.x = pose.position.x;
            context->position.y = pose.position.y;
            context->position.z = pose.position.z;
            context->rotation.x = pose.rotation.x;
            context->rotation.y = pose.rotation.y;
            context->rotation.z = pose.rotation.z;
            context->scale.x = pose.scale.x;
            context->scale.y = pose.scale.y;
            context->scale.z = pose.scale.z;
            SetCharacterLocalPosition(character, &pose.position);
            SetCharacterLocalRotation(character, &pose.rotation);
            if (splines->tracks[6].count)
                SetCharacterMotionTarget(character, 7, &pose.scale);
        }
    }
    case 2:
        ReleaseAuthorSplines(splines);
        context->flags = static_cast<std::int16_t>(context->flags) & ~1;
        break;
    }
    return 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthSetEntityRotation(std::int32_t motionType, MotionWork *work, const Vector3i *rotation) {
    if (!motionType)
        mtwkSetEntityRotation(work, rotation);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void StartAuthorActorMotion(std::int32_t motionType, Character *character, MotionWork *work, std::int32_t motion) {
    if (!motionType) {
        mtwkRequestMotion(work, motion, LowerBody);
    } else {
        const TimedNodeMotionHeader *const header = reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(motion));
        InitializeTimedNodeMotion(reinterpret_cast<TimedNodeMotion *>(work), character->model, header);
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthSetLayerLoopStartTime(std::int32_t motionType, MotionWork *work, float frame) {
    if (!motionType)
        mtwkSetLayerLoopStartTime(work, frame, LowerBody);
    else
        reinterpret_cast<TimedNodeMotion *>(work)->begin = frame;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void SetAuthorMotionFrame(std::int32_t motionType, MotionWork *work, float frame) {
    if (!motionType)
        mtwkSetRegionEvalTime(work, frame, LowerBody);
    else
        reinterpret_cast<TimedNodeMotion *>(work)->frame = frame;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthSetLayerEndFrame(std::int32_t motionType, MotionWork *work, float frame) {
    if (!motionType)
        mtwkSetLayerEndFrame(work, frame, LowerBody);
    else
        reinterpret_cast<TimedNodeMotion *>(work)->end = frame;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthSetExtraFlag(std::int32_t motionType, MotionWork *work) {
    if (!motionType)
        mtwkSetExtraFlag(work, 9, 0);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthClearExtraFlag(std::int32_t motionType, MotionWork *work) {
    if (!motionType)
        mtwkSetExtraFlag(work, 9, 2);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthSetRegionFlag(std::int32_t motionType, MotionWork *work) {
    if (!motionType)
        mtwkSetRegionFlag(work, 0x45, 0, LowerBody);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void SetAuthorMotionSpeed(std::int32_t motionType, MotionWork *work, float speed) {
    if (!motionType)
        mtwkSetSpeed(work, speed, LowerBody);
    else
        reinterpret_cast<TimedNodeMotion *>(work)->speed = speed;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthGetRootLocalPosition(std::int32_t motionType, Character *character, MotionWork *work, Vector3 *position) {
    if (!motionType)
        mtwkGetRootPosition(work, position);
    else
        GetCharacterLocalPosition(character, position);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t AuthGetRootYaw(std::int32_t motionType, Character *character, MotionWork *work) {
    std::int32_t yaw;
    if (!motionType)
        yaw = mtwkGetEntityYaw(work);
    else
        yaw = shenGetRotYFromNode(character);
    return yaw;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthSetCharacterMotionEnabled(std::int32_t motionType, Character *character, std::int32_t enabled) {
    if (!motionType)
        SetCharacterMotionEnabled(character, enabled);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void SampleAuthorMotionRoot(std::int32_t motionType, std::uint16_t motion, Character *character, float frame, Vector3 *position) {
    if (!motionType)
        SampleNativeMotionRoot(motion, charGetMotionRigType(character), frame, position);
    else
        GetCharacterLocalPosition(character, position);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void ApplyAuthorMotionPose(std::int32_t motionType, std::uint16_t motion, Character *character, float frame, Vector3 *position, Vector3i *rotation) {
    if (!motionType)
        ApplyCharacterMotionToPose(character, position, rotation, frame, motion);
    else {
        GetCharacterLocalPosition(character, position);
        GetCharacterLocalRotation(character, rotation);
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthSetRegionFlagForApply(std::int32_t motionType, MotionWork *work, MotionRegion region) {
    if (!motionType)
        mtwkSetRegionFlag(work, 0x47, 0, region);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t ApplyAuthorCharacterMotion(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *context) {
    std::int32_t motionType;
    Vector3 rootPosition;
    Character *const character = scenFindChara(g_authorController->task, shen_name(arguments[0]));
    MotionWork *work = reinterpret_cast<MotionWork *>(FindTimedNodeMotion(character));
    if (work)
        motionType = 1;
    else {
        motionType = 0;
        work = charFindMotionwork(character);
    }
    context->motionType = motionType;
    switch (phase) {
    case 0:
        if (context->flags & 1)
            AuthSetEntityRotation(motionType, work, &context->rotation);
        else {
            Vector3 position = {0.0f, 0.0f, 0.0f};
            Vector3i rotation = {0, 0, 0};
            SetCharacterLocalPosition(character, &position);
            SetCharacterLocalRotation(character, &rotation);
            AuthSetEntityRotation(motionType, work, &rotation);
            context->position.x = context->position.y = context->position.z = 0.0f;
        }
        AuthSetCharacterMotionEnabled(motionType, character, 0);
        StartAuthorActorMotion(motionType, character, work, arguments[1]);
        AuthSetLayerLoopStartTime(motionType, work, 0.0f);
        SetAuthorMotionFrame(motionType, work, static_cast<float>(static_cast<std::uint32_t>(arguments[2] - 1)));
        AuthSetLayerEndFrame(motionType, work, static_cast<float>(static_cast<std::uint32_t>(arguments[3] - 1)));
        AuthSetExtraFlag(motionType, work);
        if (arguments[4] & 1)
            AuthSetRegionFlag(motionType, work);
        if (static_cast<const std::int8_t *>(static_cast<const void *>(g_authorController->sequence))[9] > 5) {
            *reinterpret_cast<std::int32_t *>(&context->motionSpeed) = arguments[5];
            SetAuthorMotionSpeed(motionType, work, context->motionSpeed);
        } else
            context->motionSpeed = 1.0f;
        context->motionRemaining =
            static_cast<std::int32_t>(static_cast<float>(static_cast<std::uint32_t>(arguments[3] - arguments[2])) / context->motionSpeed + 1.0f);
        context->flags |= 6U;
        SetAuthorMotionSpeed(motionType, work, context->motionSpeed);
        return 1;
    case 1:
        if (context->flags & 4)
            context->flags = static_cast<std::int16_t>(context->flags) & ~4;
        if (!--context->motionRemaining) {
            SetAuthorMotionSpeed(motionType, work, 0.0f);
            return 0;
        }
        return 1;
    case 4:
        AuthSetCharacterMotionEnabled(motionType, character, 1);
        AuthClearExtraFlag(motionType, work);
        ClearAuthorMotionCutState(context, work);
        return 0;
    case 3:
        AuthSetCharacterMotionEnabled(motionType, character, 1);
        SetAuthorMotionSpeed(motionType, work, context->motionSpeed);
        AuthClearExtraFlag(motionType, work);
        AuthGetRootLocalPosition(motionType, character, work, &rootPosition);
        rootPosition.y = GetCharacterLocalPositionY(character);
        SetCharacterLocalPosition(character, &rootPosition);
        if (g_authorApplyYawOffset)
            shenSetRotY(character, AuthGetRootYaw(motionType, character, work));
        ClearAuthorMotionCutState(context, work);
        break;
    }
    return 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t AuthFaceCommand(std::int32_t, const std::int32_t *arguments, AuthActorContext *) {
    if (arguments[0]) {
        Character *const character = scenFindChara(g_authorController->task, shen_name(arguments[0]));
        if (character && HasCharacterMotionWork(character)) {
            SetUpperFacialExpression(character, arguments[1], arguments[4], static_cast<float>(static_cast<std::uint32_t>(arguments[5])) / 100.0f);
            SetLowerFacialExpression(character, arguments[2], arguments[4], static_cast<float>(static_cast<std::uint32_t>(arguments[5])) / 100.0f);
            SetFacialMouthExpression(character, arguments[3], arguments[4], 1.0f);
        }
    }
    return 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t AuthDialogueCommand(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *activeContext) {
    const std::byte *const strings = g_authorController->strings;
    const std::byte *const lips = g_authorController->lips;
    AuthActorContext *const context = activeContext;
    switch (phase) {
    case 0: {
        g_authorController->dialogueFrames = static_cast<std::int32_t>(reinterpret_cast<const float *>(arguments)[4] * 30.0f);
        Character *const character = scenFindChara(g_authorController->task, shen_name(arguments[0]));
        *reinterpret_cast<Character **>(&g_authorController->dialogueState) = character;
        if (!character)
            return 0;
        const std::int32_t index = arguments[1] * 4;
        const std::byte *const offsets = strings + 0x10;
        const char *const slash = std::strrchr(reinterpret_cast<const char *>(strings + *reinterpret_cast<const std::int32_t *>(offsets + index)), '/');
        if (slash) {
            char voiceCode[40];
            std::strcpy(voiceCode, slash + 1);
            char *const dot = std::strchr(voiceCode, '.');
            if (dot)
                *dot = 0;
            if (EV_PlayDialogue(voiceCode))
                return 0;
        }
        context->voiceCountdown = 13;
        if (reinterpret_cast<const std::int8_t *>(g_authorController->sequence)[9] > 2) {
            const std::uint32_t alternate = arguments[3];
            if (alternate != UINT32_MAX && alternate > 0x0c000000U) {
                EV_PlayDialogue(reinterpret_cast<const char *>(alternate));
                context->voiceCountdown = 5;
            }
        }
        if (reinterpret_cast<const std::int8_t *>(g_authorController->sequence)[9] > 3 && arguments[2] == -1) {
            if (reinterpret_cast<const float *>(arguments)[4] < 0.5f)
                return 0;
            if (!arguments[4])
                return 0;
            std::int32_t count = static_cast<std::int32_t>(reinterpret_cast<const float *>(arguments)[4] * 60.0f * 0.125f);
            if (count > 73)
                count = 73;
            std::int32_t words = 0;
            while (words < count * 2) {
                context->generatedMouthClip[words] = static_cast<std::int16_t>(dc::sdk::RandomFloat() * 6.0f);
                context->generatedMouthClip[words + 1] = 8;
                words += 2;
            }
            context->generatedMouthClip[words] = -1;
            return 1;
        }
        if (lips)
            return 1;
        return 0;
    }
    case 1: {
        if (static_cast<std::int8_t>(--context->voiceCountdown) != -1)
            return 1;
        Character *const character = scenFindChara(g_authorController->task, shen_name(arguments[0]));
        if (!character)
            return 0;
        if (arguments[2] != -1) {
            const std::int32_t index = arguments[2] * 4;
            const std::byte *const offsets = lips + 0x10;
            AssignFacialClip(character, reinterpret_cast<const std::int16_t *>(lips + *reinterpret_cast<const std::int32_t *>(offsets + index)));
        } else
            AssignFacialClip(character, context->generatedMouthClip);
        return 0;
    }
    }
    return 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t AuthorSoundPan(shen_name subject) {
    Character *const character = scenFindChara(g_authorController->task, subject);
    if (!character)
        return 0;
    Vector3 position, eye;
    camCtrlGetNextCamPos(&eye);
    GetCharacterLocalPosition(character, &position);
    const std::int32_t rotation = camCtrlGetNextRotY();
    const std::int16_t direction = InverseFixedAtan2(position.z - eye.z, position.x - eye.x);
    std::int16_t angle = static_cast<std::int16_t>(direction - rotation);
    angle = static_cast<std::int16_t>(angle + 0x4000);
    if (angle < 0)
        angle = static_cast<std::int16_t>(~angle);
    angle = static_cast<std::int16_t>(angle - 0x4000);
    return -(angle * 63 / 0x4000);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
static std::int32_t AuthSoundCommand(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *) {
    std::int32_t gain, bank, lane, pan;
    std::uint32_t flags;
    const std::byte *strings = g_authorController->strings;
    switch (phase) {
    case 0: {
        if (arguments[1] > 0) {
            if (arguments[3] == -1) {
                gain = arguments[4];
                bank = arguments[5];
                lane = arguments[6];
                flags = arguments[7];
                pan = 0;
                if (flags & 4U)
                    return 1;
                if (flags & 2U) {
                    pan = AuthorSoundPan(shen_name(arguments[0]));
                    if (!(flags & 4U))
                        g_authorController->audioCountdown[lane] = static_cast<std::int32_t>(AUTH_VALUE(float, arguments, 32) * 30.0f);
                }
            } else {
                gain = 127;
                bank = 4;
                lane = 16;
                flags = 0;
                pan = 0;
            }
            Loop3DSoundEffect(arguments[1]);
            if (flags & 1U)
                DispatchChannelSoundCommand(bank, lane, 0x7b0, gain);
            if (!(flags & 2U))
                return 0;
            DispatchChannelSoundCommand(bank, lane, 0xab0, pan);
            return 1;
        }
        const std::int32_t index = arguments[2] * 4;
        const std::byte *const offsets = strings + 0x10;
        ExecuteAuthorMusicCommand(reinterpret_cast<const char *>(strings + *reinterpret_cast<const std::int32_t *>(offsets + index)));
        return 0;
    }
    case 1:
        bank = arguments[5];
        lane = arguments[6];
        if (--g_authorController->audioCountdown[lane] == -1)
            return 0;
        pan = AuthorSoundPan(shen_name(arguments[0]));
        DispatchChannelSoundCommand(bank, lane, 0xab0, pan);
        return 1;
    }
    return 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void TickActiveAuthCommands(AuthorController *control) {
    AuthActiveCommand *command = control->activeCommands;
    for (std::int32_t index = 0; index < 70; ++index, ++command) {
        if (command->arguments && !command->handler(1, command->arguments, command->context)) {
            command->handler(2, command->arguments, command->context);
            command->arguments = 0;
        }
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void DispatchAuthCommand(std::int32_t channel, const std::int32_t *arguments) {
    AuthActiveCommand *command;
    AuthActiveCommand *previous = 0;
    std::int32_t index;
    command = g_authorController->activeCommands;
    for (index = 0; index < 70; ++index, ++command) {
        if (command->arguments && command->channel == channel && command->context->subject == static_cast<std::uint32_t>(arguments[0])) {
            previous = command;
            break;
        }
    }
    command = g_authorController->activeCommands;
    for (index = 0; index < 70; ++index, ++command) {
        if (command->arguments)
            continue;
        command->channel = channel;
        command->handler = authCommandHandlers[channel];
        command->arguments = arguments;
        AuthActorContext *const context = FindOrCreateAuthorActorContext(shen_name(arguments[0]));
        command->context = context;
        if (!authCommandHandlers[channel](0, arguments, context))
            command->arguments = 0;
        if (previous)
            previous->arguments = 0;
        return;
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
static AuthActorContext *FindOrCreateAuthorActorContext(shen_name subject) {
    std::int32_t index;
    AuthActorContext *context = g_authorController->actorContexts;
    for (index = 0; index < 35; ++index, ++context)
        if (context->subject == subject)
            return context;
    context = g_authorController->actorContexts;
    for (index = 0; index < 35; ++index, ++context) {
        if (!context->subject) {
            context->subject = subject;
            return context;
        }
    }
    return context;
}
#endif

// @verified - d
void CleanupAuthCommands(AuthorController *control) {
    std::int32_t subject;
    std::int32_t actorPhase;
    std::int32_t commandPhase;
    if (control->skipCountdownFinished) {
        actorPhase = 4;
        commandPhase = 4;
    } else {
        commandPhase = 2;
        actorPhase = 3;
    }
    AuthActiveCommand *command = control->activeCommands;
    for (std::int32_t index = 0; index < 70; ++index, ++command) {
        if (command->arguments) {
            command->handler(commandPhase, command->arguments, command->context);
            command->arguments = 0;
        }
    }
    if (control->skipCountdownFinished) {
        SeekAnimatedCameraSequence(control->framesRemaining * 2U);
        sctlAuthApplyMotion();
    }
    AuthActorContext *context = control->actorContexts;
    for (std::int32_t index = 0; index < 35; ++index, ++context) {
        if (context->flags & 2U) {
            subject = static_cast<std::int32_t>(context->subject);
            ApplyAuthorCharacterMotion(actorPhase, &subject, context);
        }
    }
}

// @verified - d
void authDestroySubtitleConverseData(AuthorController *control) {
    if (control && control->loaded) {
        scenCvsCtrlFree(0, 20);
        const MapControlData *const map = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        scenDestroySubtitles(map->scenarioTask);
    }
}

// @verified - d
void SCMS_Enqueue(AuthorController *control) {
    if (!control || !control->loaded)
        return;
    g_authorMaskDelay = 1;
    g_authorMaskTask = HLib::QueueTaskAfterCurrentTask(g_authorController->task, AuthorMask_Callback, 3, 0x0b, AUTHOR_MASK_TASK);
    control->loaded = 0;
    g_authorController->loaded = 0;
    CleanupAuthCommands(control);
    camCtrlSetNextRoll(0);
    camCtrlSetPerspective(0x2aaa);
    RestoreAuthCharacterStates();
    RestoreAuthorDoors();
    SetWeatherInstruction(g_authorSavedWeatherInstruction);
    SetSoundListenerUsesCamera(0);
    RestoreAuthorMapSlots();
    plymAddFlags(g_authorSavedPlayerFlags);
    g_authorHighQualityHands = 1;
    if (g_authorSmoke.effect >= 0) {
        FreeParticleEffect(0, g_authorSmoke.effect);
        g_authorSmoke.effect = -1;
        g_authorSmoke.state = 0;
    }
    if (g_authorParticles.effect >= 0) {
        FreeParticleEffect(3, g_authorParticles.effect);
        g_authorParticles.effect = -1;
        g_authorParticles.state = 0;
    }
    const MapRenderProfile &profile = MatchMapRenderProfile(g_currentMap);
    camCtrlSetFarDistance(profile.farDistance);
    camCtrlSetNearDistance(profile.nearDistance);
    camCtrlSetNpcRadius(profile.npcRadius);
    if (control->skipCountdownFinished)
        g_authorCannotSkip = 1;
    g_authorController->skipToNext = 0;
    g_authorApplyYawOffset = 0;
    g_authorUnusedFlag = 0;
    g_authorCameraPlacement = 0;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void UpdateAuthorMotionCutState(AuthorController *control) {
    std::int32_t crossed = 0;
    if (g_authorCameraKeysRemaining > 1) {
        const std::int32_t frame = control->currentFrame;
        std::uint32_t currentTime, nextTime;
        {
            const float seconds = static_cast<float>(frame) / 30.0f;
            std::memcpy(&currentTime, &seconds, sizeof(currentTime));
        }
        {
            const float seconds = static_cast<float>(frame + 1) / 30.0f;
            std::memcpy(&nextTime, &seconds, sizeof(nextTime));
        }
        float current, next;
        std::memcpy(&current, &currentTime, sizeof(current));
        current -= 0.016666668f;
        std::memcpy(&currentTime, &current, sizeof(currentTime));
        std::memcpy(&next, &nextTime, sizeof(next));
        next -= 0.016666668f;
        std::memcpy(&nextTime, &next, sizeof(nextTime));
        const float *cursor = g_authorCameraKeyCursor;
        float key = *cursor;
        if (current > key) {
            std::memcpy(&next, &nextTime, sizeof(next));
            do {
                if (next > cursor[1] && cursor[1] != key)
                    crossed = 1;
                --g_authorCameraKeysRemaining;
                ++cursor;
                if (g_authorCameraKeysRemaining < 2)
                    break;
                key = *cursor;
                std::memcpy(&current, &currentTime, sizeof(current));
            } while (current > key);
        }
        g_authorCameraKeyCursor = cursor;
    }
    AuthActorContext *context = control->actorContexts;
    for (std::int32_t index = 0; index < 35; ++index, ++context) {
        if (!(context->flags & 2U))
            continue;
        Character *const character = scenFindChara(control->task, context->subject);
        MotionWork *const work = charFindMotionwork(character);
        if (crossed) {
            if (context->flags & 4U)
                SetAuthorActorRegionFlag(context, work);
            SetClothFlag(character, 0x10, 1);
            toggleOsagFlags(character);
        } else
            ClearAuthorMotionCutState(context, work);
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t camCtrlSetFlag80000Wrapper(std::int32_t enabled);

void Author_Update(AuthorController *control) {
    if (!g_authorController->needsInitAuthorMask) {
        const std::int32_t paused = DebugMenuInputBlockedNullsub() | control->authorStopped;
        AuthActorContext *context = control->actorContexts;
        for (std::int32_t index = 0; index < 35; ++index, ++context) {
            if (context->flags & 2U) {
                Character *const character = scenFindChara(g_authorController->task, context->subject);
                MotionWork *const work = charFindMotionwork(character);
                if (paused)
                    mtwkSetSpeed(work, 0.0f, LowerBody);
                else if (context->motionRemaining)
                    mtwkSetSpeed(work, context->motionSpeed, LowerBody);
                else
                    mtwkSetSpeed(work, 0.0f, LowerBody);
            }
        }
        if (paused) {
            camCtrlSetFlag80000Wrapper(1);
            return;
        }
        camCtrlSetFlag80000Wrapper(0);
        control->currentFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(control->currentFrame) + 1U);
        if (g_authorController->dialogueFrames && !--g_authorController->dialogueFrames)
            ResetAuthorDialogueState();
        TickActiveAuthCommands(control);
        if (*reinterpret_cast<const std::uint32_t *>(control->currentInstruction) <= static_cast<std::uint32_t>(control->currentFrame))
            control->currentInstruction = DispatchAuthorTimestampGroup(control->currentInstruction + 4);
        UpdateAuthorMotionCutState(control);
        if (static_cast<std::int32_t>(control->framesRemaining) <= control->currentFrame)
            SCMS_Enqueue(control);
    } else {
        g_authorController->needsInitAuthorMask = 0;
        SCMS_Enqueue(control);
        control->currentFrame = static_cast<std::int32_t>(control->framesRemaining);
    }
    if (g_authorCameraPlacement)
        camCtrlSetXform(static_cast<const Matrix4x4 *>(g_authorCameraPlacement));
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static const std::byte *DispatchAuthorTimestampGroup(const std::byte *cursor) {
    while (*reinterpret_cast<const std::int8_t *>(cursor)) {
        DispatchAuthCommand(*reinterpret_cast<const std::int8_t *>(cursor), reinterpret_cast<const std::int32_t *>(cursor + 4));
        cursor += reinterpret_cast<const std::int8_t *>(cursor)[1] * 4 + 4;
    }
    return cursor + 4;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void InitializeAuthorRuntime(const MapInfoChunk *data) {
    AuthorController *const control = g_authorController;
    control->data = reinterpret_cast<const std::byte *>(data);
    control->sequence = FindAuthorChunk(data, AUTH_ASEQ);
    control->camera = FindAuthorChunk(data, AUTH_ACAM);
    control->motion = FindAuthorChunk(data, AUTH_AMOV);
    control->strings = FindAuthorChunk(data, AUTH_ASTR);
    control->lips = FindAuthorChunk(data, AUTH_ALIP);
    const std::byte *instruction;
    if (reinterpret_cast<const std::int8_t *>(control->sequence)[9] > 6)
        instruction = control->sequence + 0x20;
    else
        instruction = control->sequence + 0x10;
    control->currentInstruction = instruction;
    control->firstInstruction = instruction;
    control->currentFrame = 0;
    control->framesRemaining = reinterpret_cast<const std::uint32_t *>(control->sequence)[3];
    control->authorStopped = 0;
    std::memset(control->activeCommands, 0, 0x86f0);
    std::memset(g_authorSavedCharacters, 0, sizeof(g_authorSavedCharacters));
    std::memset(g_authorControlledDoors, 0, sizeof(g_authorControlledDoors));
    control->retained8784 = -21;
    control->retained8788 = 0;
    ResetAuthorDialogueState();
    g_authorCameraKeysRemaining = 0;
    g_authorCaptureCharacterStates = 1;
    g_authorSavedCharacterCount = 0;
    g_authorControlledDoorCount = 0;
    SetSoundListenerUsesCamera(1);
    g_authorSavedWeatherInstruction = GetCurrentWeatherInstruction();
    CaptureAuthorMapMasks();
    g_authorSavedPlayerFlags = plymRemoveFlags(4) & 4U;
    std::memset(&g_authorSmoke, 0, sizeof(g_authorSmoke));
    std::memset(&g_authorParticles, 0, sizeof(g_authorParticles));
    g_authorParticles.effect = g_authorSmoke.effect = -1;
    do {
        std::memset(g_authorHandAdmissionNames, 0, sizeof(g_authorHandAdmissionNames));
        g_authorHandAdmissionCount = 0;
        control->loaded = 1;
    } while (0);
    g_authorCannotSkip = 0;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static const std::byte *FindAuthorChunk(const MapInfoChunk *bundle, std::uint32_t token) {
    const std::byte *cursor = reinterpret_cast<const std::byte *>(bundle) + 8;
    const std::byte *const end = reinterpret_cast<const std::byte *>(bundle) + bundle->nextByteOffset;
    for (; cursor < end; cursor += reinterpret_cast<const std::uint32_t *>(cursor)[1])
        if (*reinterpret_cast<const std::uint32_t *>(cursor) == token)
            return cursor;
    return 0;
}
#endif

// @verified - d
void RegisterAuthorTracks(const MapInfoChunk *bundle, AuthorTrackSlots *tracks) {
    std::uint8_t *slot;
    const std::byte *end;
    std::int32_t index;
    if (!bundle)
        return;
    slot = tracks->indices;
    end = reinterpret_cast<const std::byte *>(bundle) + bundle->nextByteOffset;
    ++bundle;
    if (bundle->name == AUTH_TRCK) {
        index = 0;
        do {
            if (!authorTable[index].track) {
                authorTable[index].track = bundle;
                authorTable[index].view = 0;
                authorTable[index].fileOwned = 0;
                authorTable[index].name = 0;
                bundle = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(bundle) + bundle->nextByteOffset);
                if (bundle->name == AUTH_AUVW) {
                    authorTable[index].view = bundle;
                    bundle = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(bundle) + bundle->nextByteOffset);
                }
            }
            ++index;
        } while (bundle->name == AUTH_TRCK);
    } else {
        for (index = 69; index >= 0 && reinterpret_cast<const std::byte *>(bundle) < end; --index) {
            if (authorTable[index].track)
                continue;
            authorTable[index].name = bundle->name;
            ++bundle;
            if (tracks) {
                ++tracks->count;
                *slot++ = static_cast<std::uint8_t>(index);
            }
            authorTable[index].track = bundle;
            authorTable[index].view = 0;
            authorTable[index].fileOwned = 0;
            bundle = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(bundle) + bundle->nextByteOffset);
            if (bundle->name == AUTH_AUVW) {
                authorTable[index].view = bundle;
                bundle = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(bundle) + bundle->nextByteOffset);
            }
        }
    }
}

// @verified - d
void ClearAuthorTableEntry(std::int32_t index) {
    authorTable[index].file = 0;
    authorTable[index].track = 0;
    authorTable[index].view = 0;
    authorTable[index].fileOwned = 0;
    authorTable[index].name = 0;
}

// @verified - d
void ClearAuthorTable() {
    for (std::int32_t index = 0; index < 70; ++index)
        ClearAuthorTableEntry(index);
}

// @verified - d
void ClearAuthorPackageRecord(std::int32_t index) {
    authorPackageRecords[index].package = 0;
    authorPackageRecords[index].tracks.count = 0;
}

// @verified - d
void ClearAuthorPackageRecords() {
    for (std::int32_t index = 0; index < 6; ++index)
        ClearAuthorPackageRecord(index);
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static const MapInfoChunk *GetAuthorTableFile(std::int32_t index) {
    return authorTable[index].track;
}
#endif

// @verified - d
void LoadAuthorTableSlot(std::int32_t index, const char *filename, const char *directory) {
    char path[64], warning[68], stars[128];
    if (authorTable[index].track) {
        std::memcpy(warning, AUTHOR_REPLACEMENT_WARNING, sizeof(AUTHOR_REPLACEMENT_WARNING));
        std::memset(stars, '*', std::strlen(warning));
        if (g_authorController && g_authorController->loaded) {
            authDestroySubtitleConverseData(g_authorController);
            SCMS_Enqueue(g_authorController);
        }
        UnloadAuthorTableSlot(index);
    }
    std::sprintf(path, "%s", directory);
    MapInfoChunk *const data = static_cast<MapInfoChunk *>(shenLoadFile(path, filename, AUTHOR_FILE_STORAGE));
    authorTable[index].file = reinterpret_cast<std::byte *>(data);
    authorTable[index].track = data;
    authorTable[index].fileOwned = 1;
    authorTable[index].view = 0;
    authorTable[index].name = 0;
    const MapInfoChunk *const next = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(data) + data->nextByteOffset);
    if (next->name == AUTH_AUVW)
        authorTable[index].view = next;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void InitializeAuthorDynamicData(const AuthorTableEntry *entry) {
    std::int32_t channel;
    const std::byte *chunk;
    const std::byte *end;
    for (channel = 0; channel < 26; ++channel) {
        g_authorDynamicCommands[channel] = g_authorDynamicCommandHeads[channel] = 0;
    }
    if (entry->view) {
        for (channel = 0; channel < 26; ++channel) {
            chunk = reinterpret_cast<const std::byte *>(entry->view);
            end = chunk + AUTH_VALUE(std::uint32_t, chunk, 4);
            chunk += 8;
            while (end > chunk) {
                if (AUTH_VALUE(std::uint32_t, chunk, 0) == kAuthorDynamicTags[channel]) {
                    g_authorDynamicCommands[channel] = g_authorDynamicCommandHeads[channel] = reinterpret_cast<const AuthDynamicCommand *>(chunk + 8);
                }
                chunk += AUTH_VALUE(std::uint32_t, chunk, 4);
            }
        }
    }
    chunk = reinterpret_cast<const std::byte *>(entry->view);
    if (chunk) {
        end = chunk + AUTH_VALUE(std::uint32_t, chunk, 4);
        chunk += 8;
        while (end > chunk) {
            if (AUTH_VALUE(std::uint32_t, chunk, 0) == AUTH_HADT)
                g_authorHandCommands = const_cast<std::byte *>(chunk + 8);
            else if (AUTH_VALUE(std::uint32_t, chunk, 0) == AUTH_HAID)
                g_authorHandIndices = chunk + 8;
            chunk += AUTH_VALUE(std::uint32_t, chunk, 4);
        }
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void InitializeAuthorDynamicDataForEntry(const AuthorTableEntry *entry) {
    InitializeAuthorDynamicData(entry);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void Author_UpdateDynamic() {
    for (std::int32_t channel = 0; channel < 26; ++channel) {
        if (g_authorDynamicCommands[channel] && g_authorDynamicCommands[channel]->frame >= 0) {
            for (;;) {
                const AuthDynamicCommand *command = g_authorDynamicCommands[channel];
                const std::int32_t frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(Author_GetCurrentFrame()) - 1U);
                if (command->frame > frame)
                    break;
                if (g_authorDynamicCommands[channel]->frame < 0)
                    break;
                g_authorDynamicCommands[channel] = kAuthorDynamicHandlers[channel](g_authorDynamicCommands[channel]);
            }
        }
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *FindLastAuthorDynamicCommand(std::int32_t channel) {
    const AuthDynamicCommand *command = g_authorDynamicCommands[channel];
    const AuthDynamicCommand *last = 0;
    if (command) {
        while (command->frame >= 0) {
            last = command;
            command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(last) + last->length);
        }
    }
    return last;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicStageControl(const AuthDynamicCommand *command) {
    if (command->operation == 0) {
        switch (reinterpret_cast<const std::int16_t *>(command)[4]) {
        case 0:
            SetMapModelEnabled(reinterpret_cast<const std::uint8_t *>(command)[11], reinterpret_cast<const std::uint8_t *>(command)[10]);
            break;
        case 1:
            SetMapFieldDepthSlotEnabled(reinterpret_cast<const std::uint8_t *>(command)[11], reinterpret_cast<const std::uint8_t *>(command)[10]);
            break;
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 12);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicLightingControl(const AuthDynamicCommand *command) {
    if (command->operation == 1) {
        SetMapModelDataDefaultVertexShader();
        LGHT_Parser_Internal(reinterpret_cast<const std::uint32_t *>(command)[2]);
        PushGlobalLightingProgram();
        RemoveWeatherInstructionFlags(4);
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 12);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicCharacterDisplay(const AuthDynamicCommand *command) {
    if (command->operation == 2) {
        Character *const character = scenFindChara(g_authorController->task, AUTH_VALUE(shen_name, command, 8));
        EV_SetCharDisplay(character, CVM_DisplayEnabled, AUTH_VALUE(std::int32_t, command, 12));
    }
    command += 2;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicReflectorBoard(const AuthDynamicCommand *command) {
    if (command->operation == 3) {
        Character *const character = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::uint32_t *>(command)[3]));
        if (!reinterpret_cast<const std::uint32_t *>(command)[2]) {
            ResetReflectorBoardMode(character, 0);
        } else {
            ResetReflectorBoardMode(character, 1);
            g_authorReflectorValues.x = 1.0f;
            g_authorReflectorValues.y = reinterpret_cast<const float *>(command)[4];
            g_authorReflectorValues.z = reinterpret_cast<const float *>(command)[5];
            g_authorReflectorValues.w = reinterpret_cast<const float *>(command)[6];
            SetReflectorBoardValues(character, &g_authorReflectorValues);
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 28);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void AuthorEyes_Callback(void *data) {
    AuthEyeTaskData *const eyes = reinterpret_cast<AuthEyeTaskData *>(data);
    std::int32_t index;
    if (!authGetLoaded()) {
        for (index = 0; index < 32; ++index) {
            if (g_authorEyesControllers[index].subject == shen_name(reinterpret_cast<const std::uint32_t *>(eyes->command)[2])) {
                g_authorEyesControllers[index].subject = shen_name();
                g_authorEyesControllers[index].task = 0;
                break;
            }
        }
        HLib::FreeCurrentTaskData();
        HLib::DestroyCurrentTask();
        return;
    }
    Character *const character = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::uint32_t *>(eyes->command)[2]));
    if (!character)
        return;
    Vector3 target;
    switch (eyes->state) {
    case 0:
        if (reinterpret_cast<const std::uint32_t *>(eyes->command)[3]) {
            Character *const targetCharacter = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::uint32_t *>(eyes->command)[3]));
            charaGetMotWorldPos(targetCharacter, kAuthorAttachmentBones[reinterpret_cast<const std::int16_t *>(eyes->command)[8]], &target);
            target.x += reinterpret_cast<const float *>(eyes->command)[5];
            target.y += reinterpret_cast<const float *>(eyes->command)[6];
            target.z += reinterpret_cast<const float *>(eyes->command)[7];
        } else {
            target.x = reinterpret_cast<const float *>(eyes->command)[5];
            target.y = reinterpret_cast<const float *>(eyes->command)[6];
            target.z = reinterpret_cast<const float *>(eyes->command)[7];
            if (g_authorCameraPlacement) {
                matrix_stack::Matrix4x4_Push(0);
                matrix_stack::LoadMatrix(reinterpret_cast<const Matrix4x4 *>(g_authorCameraPlacement));
                Matrix4x4::TransformPoint(target, target);
                PopCurrentMatrix(1);
            }
        }
        SetEyeToChara(character, &target, reinterpret_cast<const std::int16_t *>(eyes->command)[9], 2);
        if (reinterpret_cast<const std::int16_t *>(eyes->command)[9] > 0)
            eyes->state = -1;
        return;
    case 1:
        SetEyeToChara(character, &target, reinterpret_cast<const std::int16_t *>(eyes->command)[9], 0);
    case 2:
        for (index = 0; index < 32; ++index) {
            if (g_authorEyesControllers[index].subject == shen_name(reinterpret_cast<const std::uint32_t *>(eyes->command)[2])) {
                g_authorEyesControllers[index].subject = shen_name();
                g_authorEyesControllers[index].task = 0;
                break;
            }
        }
        HLib::FreeCurrentTaskData();
        HLib::DestroyCurrentTask();
        return;
    default:
        return;
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicEyes(const AuthDynamicCommand *command) {
    if (command->operation == 4) {
        if (scenFindChara(g_authorController->task, AUTH_VALUE(shen_name, command, 8))) {
            std::int32_t index = 0;
            std::int32_t freeSlot = -1;
            AuthEyeTaskData *eyes = 0;
            for (; index < 32; ++index) {
                if (g_authorEyesControllers[index].subject == AUTH_VALUE(shen_name, command, 8))
                    eyes = static_cast<AuthEyeTaskData *>(g_authorEyesControllers[index].task->taskData);
                else if (!g_authorEyesControllers[index].task && freeSlot == -1)
                    freeSlot = index;
            }
            if (eyes || (freeSlot != -1 && AUTH_VALUE(std::uint32_t, command, 12) != AUTH_RECV)) {
                if (!eyes && freeSlot != -1) {
                    HLib::Task *const task = HLib::QueueTaskAfterCurrentTask(g_authorController->task, AuthorEyes_Callback, 3, 0x0b, AUTH_SCEY);
                    g_authorEyesControllers[freeSlot].task = task;
                    eyes = reinterpret_cast<AuthEyeTaskData *>(AllocateTASK(task, sizeof(AuthEyeTaskData)));
                    g_authorEyesControllers[freeSlot].subject = AUTH_VALUE(shen_name, command, 8);
                }
                if (eyes) {
                    if (AUTH_VALUE(std::uint32_t, command, 12) == AUTH_RECV) {
                        eyes->state = 1;
                    } else {
                        eyes->state = 0;
                        eyes->command = command;
                    }
                }
            }
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 44);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicHand(const AuthDynamicCommand *command) {
    if (command->operation == 5) {
        Character *character = scenFindChara(g_authorController->task, AUTH_VALUE(shen_name, command, 8));
        PrepareAuthorHands(character, AUTH_VALUE(shen_name, command, 8));
        CharacterHandComponent *hand;
        if (AUTH_VALUE(std::uint8_t, command, 13))
            hand = AllocateHNDR(character);
        else
            hand = AllocateHNDL(character);
        if (hand) {
            if (AUTH_VALUE(std::uint8_t, command, 12) == 0) {
                SetCharacterHandRegionEnabled(hand, 0, 0, 1);
            } else {
                if (!IsCharacterHandRegionEnabled(hand))
                    SetCharacterHandRegionEnabled(hand, 1, 0, 1);
                ParseHandPoseCommand(*hand, g_authorHandCommands + AUTH_VALUE(std::int16_t, command, 14) * 0xe4, AUTH_VALUE(std::int16_t, command, 16));
            }
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 20);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicCloth(const AuthDynamicCommand *command) {
    if (command->operation == 6) {
        Character *const character = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::int32_t *>(command)[2]));
        if (character) {
            switch (reinterpret_cast<const std::int32_t *>(command)[3]) {
            case -1:
                SetClothFlag(character, 2, 1);
                break;
            case -2:
                SetClothFlag(character, 2, 0);
                break;
            case -3:
                SetClothFlag(character, 4, 1);
                break;
            case -4:
                SetClothFlag(character, 4, 0);
                break;
            case -5:
                SetClothFlag(character, 0x10, 1);
                break;
            default:
                clmdUtil(character, static_cast<std::uint8_t>(reinterpret_cast<const std::int32_t *>(command)[3]));
                break;
            }
        }
    }
    command += 2;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicOsageControlNullsub(const AuthDynamicCommand *command) {
    return reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 16);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicBlur(const AuthDynamicCommand *command) {
    if (command->operation == 8) {
        Character *const character = scenFindChara(g_authorController->task, AUTH_VALUE(shen_name, command, 12));
        if (!AUTH_VALUE(std::int16_t, command, 10)) {
            figpAssignModeForCharacter(character, static_cast<std::uint8_t>(AUTH_VALUE(std::int16_t, command, 10)));
            ClearAuthSavedCharacterFlags(AUTH_VALUE(shen_name, command, 12), AuthRestoreFigurePose);
        } else {
            figpAssignModeForCharacter(character, static_cast<std::uint8_t>(AUTH_VALUE(std::int16_t, command, 10)));
            figpAssignMotScaleAndDirFor(character, AUTH_VALUE(std::uint8_t, command, 8), AUTH_VALUE(std::uint8_t, command, 9));
            SetAuthSavedCharacterFlags(AUTH_VALUE(shen_name, command, 12), AuthRestoreFigurePose);
        }
    }
    command += 2;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicWaveNullsub(const AuthDynamicCommand *command) {
    return reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 12);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
const AuthDynamicCommand *AuthorDynamicFaceExpression(const AuthDynamicCommand *command) {
    const std::byte *const data = reinterpret_cast<const std::byte *>(command);
    if (command->operation == 10) {
        Character *const character = scenFindChara(g_authorController->task, *reinterpret_cast<const shen_name *>(data + 8));
        if (character) {
            const std::int8_t *const selectors = reinterpret_cast<const std::int8_t *>(data + 12);
            SetUpperFacialExpression(character, selectors[0], *reinterpret_cast<const std::int16_t *>(data + 16), *reinterpret_cast<const float *>(data + 20));
            SetLowerFacialExpression(character, selectors[1], *reinterpret_cast<const std::int16_t *>(data + 16), *reinterpret_cast<const float *>(data + 20));
            SetFacialMouthExpression(character, selectors[2], *reinterpret_cast<const std::int16_t *>(data + 16), 1.0f);
        }
    }
    command += 3;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicFade(const AuthDynamicCommand *command) {
    if (command->operation == 11) {
        HLib::Task *const filmTask = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask;
        ScreenFilmColourRequest request;
        request.durationFrames = AUTH_VALUE(std::int16_t, command, 10);
        request.start[0] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][0];
        request.start[1] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][1];
        request.start[2] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][2];
        request.start[3] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][3];
        request.target[0] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][4];
        request.target[1] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][5];
        request.target[2] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][6];
        request.target[3] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, command, 8)][7];
        WaitForNextTask(filmTask, request, 0);
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 12);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicMotionBlendTrack(const AuthDynamicCommand *command) {
    if (command->operation == 12) {
        Character *const character = scenFindChara(g_authorController->task, AUTH_VALUE(shen_name, command, 8));
        if (character) {
            switch (*reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(command) + 12)) {
            case 0:
                break;
            case 1:
                AT_SetEyeCtrl(
                    character, static_cast<float>(*reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(command) + 14)), 1.0f, 1.0f);
                break;
            case 2:
                AT_SetEyeCtrl(
                    character, static_cast<float>(*reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(command) + 14)), 0.0f, 0.0f);
                break;
            case 3:
                AT_SetEyeCtrl(character, 0.0f, 0.0f, 0.0f);
                StartFacialBlink(character);
                break;
            case 4:
                AT_SetEyeCtrl(character, static_cast<float>(*reinterpret_cast<const std::int16_t *>(reinterpret_cast<const std::byte *>(command) + 14)),
                    *reinterpret_cast<const float *>(reinterpret_cast<const std::byte *>(command) + 20),
                    *reinterpret_cast<const float *>(reinterpret_cast<const std::byte *>(command) + 16));
                break;
            case 5:
                AT_SetEyeCtrl(character, 0.0f, 0.0f, 0.0f);
                SetFacialBlinkDelay(character, 90);
                break;
            case 6:
                SetFacialBlinkDelay(character, 0);
                break;
            }
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + command->length);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
const AuthDynamicCommand *AuthorDynamicSmoke(const AuthDynamicCommand *command) {
    AuthSmokeState *smoke = &g_authorSmoke;
    if (command->operation == 13) {
        switch (AUTH_VALUE(std::int16_t, command, 8)) {
        case 0:
            switch (AUTH_VALUE(std::int32_t, command, 12)) {
            case 0:
                if (smoke->state == 2) {
                    ResumeParticleEffect(0, smoke->effect);
                    smoke->state = 1;
                }
                break;
            case 1:
                if (smoke->state == 1) {
                    SuspendParticleEffect(0, smoke->effect);
                    smoke->state = 2;
                }
                break;
            case 2:
                if (smoke->state) {
                    FreeParticleEffect(0, smoke->effect);
                    smoke->state = 0;
                    smoke->effect = -1;
                }
                break;
            }
            break;
        case 1:
            smoke->count = AUTH_VALUE(std::int16_t, command, 12);
            if (!smoke->state) {
                smoke->effect = CreateSmokeEffect(&smoke->position, &smoke->spawnVolume, smoke->count, smoke->birthRate, 0, 0);
                SetParticleEffectColour(0, smoke->effect, 0, smoke->red, smoke->green, smoke->blue);
                SetSmokeEffectFadeFactor(smoke->effect, smoke->fade);
                SetSmokeEffectFalloff(smoke->effect, smoke->falloff);
                SetParticleEffectInitialScale(0, smoke->effect, smoke->initialScale);
                SetParticleEffectSize(0, smoke->effect, smoke->width, smoke->height);
                SetParticleEffectBirthRate(0, smoke->effect, smoke->birthRate);
                SetParticleEffectVelocity(0, smoke->effect, &smoke->velocity, &smoke->velocityVariance);
                smoke->state = 2;
            }
            break;
        case 2:
            smoke->position.x = AUTH_VALUE(float, command, 12);
            smoke->position.y = AUTH_VALUE(float, command, 16);
            smoke->position.z = AUTH_VALUE(float, command, 20);
            if (smoke->state)
                SetParticleEffectPosition(0, smoke->effect, &smoke->position, &smoke->spawnVolume);
            break;
        case 3:
            smoke->spawnVolume.x = AUTH_VALUE(float, command, 12);
            smoke->spawnVolume.y = AUTH_VALUE(float, command, 16);
            smoke->spawnVolume.z = AUTH_VALUE(float, command, 20);
            if (smoke->state)
                SetParticleEffectPosition(0, smoke->effect, &smoke->position, &smoke->spawnVolume);
            break;
        case 4:
            smoke->velocityVariance.x = AUTH_VALUE(float, command, 12);
            smoke->velocityVariance.y = AUTH_VALUE(float, command, 16);
            smoke->velocityVariance.z = AUTH_VALUE(float, command, 20);
            if (smoke->state)
                SetParticleEffectVelocity(0, smoke->effect, &smoke->velocity, &smoke->velocityVariance);
            break;
        case 5:
            smoke->velocity.x = AUTH_VALUE(float, command, 12);
            smoke->velocity.y = AUTH_VALUE(float, command, 16);
            smoke->velocity.z = AUTH_VALUE(float, command, 20);
            if (smoke->state)
                SetParticleEffectVelocity(0, smoke->effect, &smoke->velocity, &smoke->velocityVariance);
            break;
        case 6:
            smoke->falloff = AUTH_VALUE(std::uint8_t, command, 12);
            if (smoke->state)
                SetSmokeEffectFalloff(smoke->effect, smoke->falloff);
            break;
        case 7:
            smoke->initialScale = AUTH_VALUE(float, command, 12);
            if (smoke->state)
                SetParticleEffectInitialScale(0, smoke->effect, smoke->initialScale);
            break;
        case 8:
            smoke->width = AUTH_VALUE(float, command, 12);
            smoke->height = AUTH_VALUE(float, command, 16);
            if (smoke->state)
                SetParticleEffectSize(0, smoke->effect, smoke->width, smoke->height);
            break;
        case 9:
            smoke->fade = AUTH_VALUE(std::uint8_t, command, 12);
            smoke->red = AUTH_VALUE(std::uint8_t, command, 13);
            smoke->green = AUTH_VALUE(std::uint8_t, command, 14);
            smoke->blue = AUTH_VALUE(std::uint8_t, command, 15);
            if (smoke->state) {
                SetParticleEffectColour(0, smoke->effect, 0, smoke->red, smoke->green, smoke->blue);
                SetSmokeEffectFadeFactor(smoke->effect, smoke->fade);
            }
            break;
        case 10:
            smoke->birthRate = AUTH_VALUE(float, command, 12);
            if (smoke->state)
                SetParticleEffectBirthRate(0, smoke->effect, smoke->birthRate);
            break;
        }
    }
    command += 3;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
const AuthDynamicCommand *AuthorDynamicFallingParticles(const AuthDynamicCommand *command) {
    if (command->operation == 14) {
        AuthParticleState *particles = &g_authorParticles;
        switch (AUTH_VALUE(std::int16_t, command, 8)) {
        case 0:
            switch (AUTH_VALUE(std::int32_t, command, 12)) {
            case 0:
                if (particles->state == 2) {
                    ResumeParticleEffect(3, particles->effect);
                    particles->state = 1;
                }
                break;
            case 1:
                if (particles->state == 1) {
                    SuspendParticleEffect(3, particles->effect);
                    particles->state = 2;
                }
                break;
            case 2:
                if (particles->state) {
                    FreeParticleEffect(3, particles->effect);
                    particles->state = 0;
                    particles->effect = -1;
                }
                break;
            }
            break;
        case 1:
            particles->count = AUTH_VALUE(std::int16_t, command, 12);
            if (!particles->state) {
                static const char *fallingLeafTextures[3] = {"GRENLEAF.pvr", "REDLEAF.pvr", "SHENLEAF.pvr"};
                particles->effect = CreateFallingParticleEffect(&particles->position, &particles->spawnVolume, particles->count, particles->birthRate, 0,
                    "misc", fallingLeafTextures[particles->texture], particles->floorHeight);
                SetParticleEffectSize(3, particles->effect, particles->width, particles->height);
                SetParticleEffectBirthRate(3, particles->effect, particles->birthRate);
                SetParticleEffectVelocity(3, particles->effect, &particles->velocity, &particles->velocityVariance);
                particles->state = 2;
            }
            break;
        case 2:
            particles->position.x = AUTH_VALUE(float, command, 12);
            particles->position.y = AUTH_VALUE(float, command, 16);
            particles->position.z = AUTH_VALUE(float, command, 20);
            if (particles->state)
                SetParticleEffectPosition(3, particles->effect, &particles->position, &particles->spawnVolume);
            break;
        case 3:
            particles->spawnVolume.x = AUTH_VALUE(float, command, 12);
            particles->spawnVolume.y = AUTH_VALUE(float, command, 16);
            particles->spawnVolume.z = AUTH_VALUE(float, command, 20);
            if (particles->state)
                SetParticleEffectPosition(3, particles->effect, &particles->position, &particles->spawnVolume);
            break;
        case 4:
            particles->velocityVariance.x = AUTH_VALUE(float, command, 12);
            particles->velocityVariance.y = AUTH_VALUE(float, command, 16);
            particles->velocityVariance.z = AUTH_VALUE(float, command, 20);
            if (particles->state)
                SetParticleEffectVelocity(3, particles->effect, &particles->velocity, &particles->velocityVariance);
            break;
        case 5:
            particles->velocity.x = AUTH_VALUE(float, command, 12);
            particles->velocity.y = AUTH_VALUE(float, command, 16);
            particles->velocity.z = AUTH_VALUE(float, command, 20);
            if (particles->state)
                SetParticleEffectVelocity(3, particles->effect, &particles->velocity, &particles->velocityVariance);
            break;
        case 7:
            particles->floorHeight = AUTH_VALUE(float, command, 12);
            if (particles->state)
                SetFallingParticleFloor(particles->floorHeight);
            break;
        case 8:
            particles->width = AUTH_VALUE(float, command, 12);
            particles->height = AUTH_VALUE(float, command, 16);
            if (particles->state)
                SetParticleEffectSize(3, particles->effect, particles->width, particles->height);
            break;
        case 10:
            particles->birthRate = AUTH_VALUE(float, command, 12);
            if (particles->state)
                SetParticleEffectBirthRate(3, particles->effect, particles->birthRate);
            break;
        case 11:
            particles->texture = AUTH_VALUE(std::int32_t, command, 12);
            break;
        }
    }
    command += 3;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void AuthorParts_Callback(void *data) {
    AuthPartsTaskData *const state = static_cast<AuthPartsTaskData *>(data);
    std::int32_t index;
    if (!authGetLoaded()) {
        for (index = 0; index < 32; ++index) {
            if (g_authorPartsControllers[index].subject == shen_name(reinterpret_cast<const std::uint32_t *>(state->command)[3])) {
                g_authorPartsControllers[index].subject = shen_name();
                g_authorPartsControllers[index].task = 0;
                break;
            }
        }
        HLib::FreeCurrentTaskData();
        HLib::DestroyCurrentTask();
        return;
    }
    Character *const character = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::uint32_t *>(state->command)[3]));
    if (!character)
        return;
    switch (state->state) {
    case 0: {
        MT7Node *const model = character->model;
        for (index = 0; index < 4; ++index) {
            const std::int8_t part = reinterpret_cast<const AuthPartsCommand *>(state->command)->parts[index];
            if (part == -1)
                continue;
            MT7Node *const node = FindMT7Node(model, authorPartNodeIds[part]);
            if (!node)
                continue;
            switch (reinterpret_cast<const std::uint32_t *>(state->command)[2]) {
            case 0:
                node->position.x = reinterpret_cast<const float *>(state->command)[8];
                node->position.y = reinterpret_cast<const float *>(state->command)[9];
                node->position.z = reinterpret_cast<const float *>(state->command)[10];
                node->rotation.x = reinterpret_cast<const std::int32_t *>(state->command)[5];
                node->rotation.y = reinterpret_cast<const std::int32_t *>(state->command)[6];
                node->rotation.z = reinterpret_cast<const std::int32_t *>(state->command)[7];
                break;
            case 1:
            case 2:
            case 4:
                state->position.x += state->positionStep.x;
                state->position.y += state->positionStep.y;
                state->position.z += state->positionStep.z;
                state->rotation.x += state->rotationStep.x;
                state->rotation.y += state->rotationStep.y;
                state->rotation.z += state->rotationStep.z;
                node->position.x += state->position.x;
                node->position.y += state->position.y;
                node->position.z += state->position.z;
                node->rotation.x += state->rotation.x;
                node->rotation.y += state->rotation.y;
                node->rotation.z += state->rotation.z;
                break;
            }
        }
        if (reinterpret_cast<const std::uint32_t *>(state->command)[2] == 2 || reinterpret_cast<const std::uint32_t *>(state->command)[2] == 4)
            return;
        break;
    }
    case 1:
        break;
    default:
        return;
    }
    for (index = 0; index < 32; ++index) {
        if (g_authorPartsControllers[index].subject == shen_name(reinterpret_cast<const std::uint32_t *>(state->command)[3])) {
            g_authorPartsControllers[index].subject = shen_name();
            g_authorPartsControllers[index].task = 0;
            break;
        }
    }
    HLib::FreeCurrentTaskData();
    HLib::DestroyCurrentTask();
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicParticles(const AuthDynamicCommand *command) {
    const std::byte *record = reinterpret_cast<const std::byte *>(command);
    if (reinterpret_cast<const AuthDynamicCommand *>(record)->operation == 15 &&
        scenFindChara(g_authorController->task, *reinterpret_cast<const shen_name *>(record + 12))) {
        std::int32_t index = 0, freeSlot = -1;
        AuthPartsTaskData *state = 0;
        do {
            if (g_authorPartsControllers[index].subject == *reinterpret_cast<const shen_name *>(record + 12))
                state = reinterpret_cast<AuthPartsTaskData *>(g_authorPartsControllers[index].task->taskData);
            else if (!g_authorPartsControllers[index].task && freeSlot == -1)
                freeSlot = index;
            ++index;
        } while (index < 32);
        if (!state) {
            if (freeSlot == -1) {
                record += 44;
                return reinterpret_cast<const AuthDynamicCommand *>(record);
            }
            if (*reinterpret_cast<const std::uint32_t *>(record + 8) == 3) {
                record += 44;
                return reinterpret_cast<const AuthDynamicCommand *>(record);
            }
        }
        if (!state && freeSlot != -1) {
            HLib::Task *const task = HLib::QueueTaskAfterCurrentTask(g_authorController->task, AuthorParts_Callback, 3, 0x0b, AUTH_SCPT);
            g_authorPartsControllers[freeSlot].task = task;
            state = static_cast<AuthPartsTaskData *>(AllocateTASK(task, sizeof(AuthPartsTaskData)));
            g_authorPartsControllers[freeSlot].subject = *reinterpret_cast<const shen_name *>(record + 12);
        }
        if (state) {
            switch (*reinterpret_cast<const std::uint32_t *>(record + 8)) {
            case 3:
                state->state = 1;
                break;
            case 4:
                state->state = 0;
                state->command = reinterpret_cast<const AuthDynamicCommand *>(record);
                state->positionStep.x = reinterpret_cast<const Vector3 *>(record + 32)->x;
                state->positionStep.y = reinterpret_cast<const Vector3 *>(record + 32)->y;
                state->positionStep.z = reinterpret_cast<const Vector3 *>(record + 32)->z;
                state->rotationStep.x = reinterpret_cast<const std::int32_t *>(record)[5];
                state->rotationStep.y = reinterpret_cast<const std::int32_t *>(record)[6];
                state->rotationStep.z = reinterpret_cast<const std::int32_t *>(record)[7];
                break;
            default:
                state->state = 0;
                state->command = reinterpret_cast<const AuthDynamicCommand *>(record);
                state->position.x = reinterpret_cast<const Vector3 *>(record + 32)->x;
                state->position.y = reinterpret_cast<const Vector3 *>(record + 32)->y;
                state->position.z = reinterpret_cast<const Vector3 *>(record + 32)->z;
                state->rotation.x = reinterpret_cast<const std::int32_t *>(record)[5];
                state->rotation.y = reinterpret_cast<const std::int32_t *>(record)[6];
                state->rotation.z = reinterpret_cast<const std::int32_t *>(record)[7];
                state->positionStep.x = state->positionStep.y = state->positionStep.z = 0.0f;
                state->rotationStep.x = state->rotationStep.y = state->rotationStep.z = 0;
                break;
            }
        }
    }
    record += 44;
    return reinterpret_cast<const AuthDynamicCommand *>(record);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicFog(const AuthDynamicCommand *command) {
    if (command->operation == 16) {
        switch (reinterpret_cast<const std::uint32_t *>(command)[2]) {
        case 0:
            RemoveWeatherInstructionFlags(0x780);
            SetFogRed(reinterpret_cast<const float *>(command)[3]);
            SetFogGreen(reinterpret_cast<const float *>(command)[4]);
            SetFogBlue(reinterpret_cast<const float *>(command)[5]);
            SetFogDistance(reinterpret_cast<const std::uint32_t *>(command)[6]);
            ReinitFogTasks(1);
            break;
        case 1:
            RemoveWeatherInstructionFlags(0x1800);
            SetCloudAndSkyColour(reinterpret_cast<const std::uint32_t *>(command)[3], reinterpret_cast<const std::uint32_t *>(command)[4]);
            break;
        case 2:
            RemoveWeatherInstructionFlags(0x2000);
            shenUpdatePlanetByLayer(reinterpret_cast<const float *>(command)[3], 0, 0);
            shenUpdatePlanetByLayer(reinterpret_cast<const float *>(command)[3], 0, 1);
            shenUpdatePlanetByLayer(reinterpret_cast<const float *>(command)[4], 1, 0);
            shenUpdatePlanetByLayer(reinterpret_cast<const float *>(command)[4], 1, 1);
            break;
        case 3:
            RemoveWeatherInstructionFlags(0x1800);
            EV_SetPlanetLayerColours_Impl(reinterpret_cast<const std::uint32_t *>(command)[3], reinterpret_cast<const std::uint32_t *>(command)[4], 0);
            EV_SetPlanetLayerColours_Impl(reinterpret_cast<const std::uint32_t *>(command)[3], reinterpret_cast<const std::uint32_t *>(command)[4], 1);
            break;
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 28);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicFieldObject(const AuthDynamicCommand *command) {
    if (command->operation == 17) {
        Character *target;
        if (reinterpret_cast<const std::uint32_t *>(command)[2] != AUTH_RECV) {
            target = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::uint32_t *>(command)[3]));
            if (target) {
                Character *const source = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::uint32_t *>(command)[2]));
                if (source) {
                    SetFixoDataForMotionWork(target, source, kAuthorAttachmentBones[reinterpret_cast<const std::uint32_t *>(command)[4]],
                        *reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::uint32_t *>(command) + 8),
                        *reinterpret_cast<const std::int32_t (*)[3]>(reinterpret_cast<const std::uint32_t *>(command) + 5));
                    SetAuthSavedCharacterFlags(shen_name(reinterpret_cast<const std::uint32_t *>(command)[3]), 2);
                }
            }
        } else {
            target = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::uint32_t *>(command)[3]));
            if (target) {
                ResetCharaFixo(target);
                ClearAuthSavedCharacterFlags(shen_name(reinterpret_cast<const std::uint32_t *>(command)[3]), 2);
            }
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 44);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicShadow(const AuthDynamicCommand *command) {
    if (command->operation == 18) {
        Character *const character =
            scenFindChara(g_authorController->task, *reinterpret_cast<const shen_name *>(reinterpret_cast<const std::int32_t *>(command) + 2));
        if (character) {
            switch (reinterpret_cast<const std::int16_t *>(command)[6]) {
            case 0:
                SetCharacterType(character, 0);
                SetCharacterDrawControlEnabled(character, reinterpret_cast<const std::int16_t *>(command)[7] > 0);
                break;
            case 1:
                SetCharacterType(character, 4);
                SetCharacterDrawControlEnabled(character, reinterpret_cast<const std::int16_t *>(command)[7] > 0);
                break;
            case 2:
                SetCharacterDrawControlEnabled(character, 0);
                break;
            }
        }
    }
    command += 2;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicFaceClip(const AuthDynamicCommand *command) {
    if (command->operation == 19) {
        if (reinterpret_cast<const std::int32_t *>(command)[2] == 0)
            camCtrlSetFarDistance(reinterpret_cast<const float *>(command)[3]);
        else if (reinterpret_cast<const std::int32_t *>(command)[2] == 1)
            camCtrlSetNearDistance(reinterpret_cast<const float *>(command)[3]);
        else
            camCtrlSetNpcRadius(reinterpret_cast<const float *>(command)[3]);
    }
    command += 2;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicFilter(const AuthDynamicCommand *command) {
    if (command->operation == 20) {
        HLib::Task *const filmTask = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask;
        ScreenFilmColourRequest request;
        request.durationFrames = AUTH_VALUE(std::int16_t, command, 16);
        request.start[0] = AUTH_VALUE(std::uint8_t, command, 8);
        request.start[1] = AUTH_VALUE(std::uint8_t, command, 9);
        request.start[2] = AUTH_VALUE(std::uint8_t, command, 10);
        request.start[3] = AUTH_VALUE(std::uint8_t, command, 11);
        request.target[0] = AUTH_VALUE(std::uint8_t, command, 12);
        request.target[1] = AUTH_VALUE(std::uint8_t, command, 13);
        request.target[2] = AUTH_VALUE(std::uint8_t, command, 14);
        request.target[3] = AUTH_VALUE(std::uint8_t, command, 15);
        WaitForNextTask(filmTask, request, 0);
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 20);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicLens(const AuthDynamicCommand *command) {
    if (command->operation == 21) {
        SetWeatherLightManager(AUTH_VALUE(std::int32_t, command, 8));
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 12);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicDoor(const AuthDynamicCommand *command) {
    if (command->operation == 22) {
        SetDoorSubjectEnabled(
            *reinterpret_cast<const shen_name *>(reinterpret_cast<const std::int32_t *>(command) + 2), reinterpret_cast<const std::int32_t *>(command)[3]);
        RecordAuthorDoor(*reinterpret_cast<const shen_name *>(reinterpret_cast<const std::int32_t *>(command) + 2));
    }
    command += 2;
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicLightState(const AuthDynamicCommand *command) {
    if (command->operation == 23) {
        const std::uint32_t *const parameters = reinterpret_cast<const std::uint32_t *>(command + 1);
        SetLightEntryIndex(parameters[0], parameters[1]);
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 16);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicWrist(const AuthDynamicCommand *command) {
    if (command->operation == 24) {
        Character *const character = scenFindChara(g_authorController->task, shen_name(reinterpret_cast<const std::int32_t *>(command)[2]));
        if (character) {
            CharacterHandComponent *const hand = reinterpret_cast<const std::int32_t *>(command)[3] ? AllocateHNDR(character) : AllocateHNDL(character);
            if (hand)
                mhndSetPrimaryRotation(hand, reinterpret_cast<const std::int32_t *>(command)[4], reinterpret_cast<const std::int32_t *>(command)[5],
                    reinterpret_cast<const std::int32_t *>(command)[6]);
        }
    }
    command = reinterpret_cast<const AuthDynamicCommand *>(reinterpret_cast<const std::byte *>(command) + 28);
    return command;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
const AuthDynamicCommand *AuthorDynamicPartsLight(const AuthDynamicCommand *command) {
    if (command->operation == 25) {
        Character *const character = scenFindChara(g_authorController->task, AUTH_VALUE(shen_name, command, 12));
        if (character)
            SetPluralControlFor(character, AUTH_VALUE(std::int16_t, command, 8));
    }
    command += 2;
    return command;
}
#endif

void RestoreAuthCharacterStates() {
    for (std::int32_t index = 0; index < g_authorSavedCharacterCount; ++index) {
        const std::uintptr_t offset = index * sizeof(AuthSavedCharacter);
        const AuthSavedCharacter &state = *reinterpret_cast<const AuthSavedCharacter *>(reinterpret_cast<std::uintptr_t>(g_authorSavedCharacters) + offset);
        Character *const character = scenFindChara(g_authorController->task, state.subject);
        if (character) {
            SetPluralControlFor(character, state.pluralState);
            EV_SetCharDisplay(character, CVM_DisplayEnabled, state.displayEnabled);
            AssignCharacterControlFlags(character, state.controlFlags);
            SetHighModelAndFaceFlagsForChara(character, 0, 10, 1.0f);
            AT_SetEyeCtrl(character, 10.0f, 0.0f, 0.0f);
            SetFacialBlinkDelay(character, 90);
            if (HasCharacterMotionWork(character)) {
                const Vector3 target = {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f};
                SetEyeToChara(character, &target, 0, 0);
                ResetReflectorBoardMode(character, state.reflectorSector);
                SetReflectorBoardValues(character, &state.reflectorValues);
                const std::int32_t snapshot = state.motionSnapshot;
                if (snapshot > -1) {
                    RestoreMotionSnapshot(*reinterpret_cast<const shen_name *>(reinterpret_cast<std::uintptr_t>(g_authorSavedCharacters) + offset),
                        static_cast<std::uint32_t>(snapshot));
                    ReleaseMotionSnapshot(static_cast<std::uint32_t>(snapshot));
                }
                SetCharacterType(character, state.storedShadowY);
                SetCharacterDrawControlEnabled(character, state.drawControl);
                std::int32_t enabled;
                CharacterHandComponent *hand = AllocateHNDR(character);
                if (hand) {
                    enabled = 0;
                    if (state.flags & AuthRestoreRightHand)
                        enabled = 1;
                    mhndSetPrimaryRotation(hand, 0, 0, 0);
                    SetCharacterHandRegionEnabled(hand, enabled, 0, 1);
                }
                hand = AllocateHNDL(character);
                if (hand) {
                    enabled = 0;
                    if (state.flags & AuthRestoreLeftHand)
                        enabled = 1;
                    mhndSetPrimaryRotation(hand, 0, 0, 0);
                    SetCharacterHandRegionEnabled(hand, enabled, 0, 1);
                }
                if (state.flags & AuthRestoreLowQualityHands)
                    hndlSetFlags(character);
                charaToggleFlag(character, state.unknown40);
                SetClothFlag(character, 4, 0);
                clmdUtil(character, 0);
            }
            if (state.flags & AuthRestoreFigurePose)
                figpAssignModeForCharacter(character, 0);
            if (state.flags & AuthRestoreObjectXForms)
                ResetCharaFixo(character);
        }
    }
    g_authorSavedCharacterCount = 0;
}

// @verified - d
AuthSavedCharacter *RecordAuthCharacterState(shen_name subject, std::uint32_t value) {
    for (std::int32_t index = 0; index < g_authorSavedCharacterCount; ++index) {
        if (g_authorSavedCharacters[index].subject == subject)
            return 0;
    }
    AuthSavedCharacter *const state = &g_authorSavedCharacters[g_authorSavedCharacterCount];
    state->subject = subject;
    state->retained04 = value;
    g_authorSavedCharacterCount = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_authorSavedCharacterCount) + 1U);
    return state;
}

// @verified - d
AuthSavedCharacter *FindAuthCharacterState(shen_name subject) {
    for (std::int32_t index = 0; index < g_authorSavedCharacterCount; index++)
        if (g_authorSavedCharacters[index].subject == subject)
            return &g_authorSavedCharacters[index];
    return 0;
}

// @verified - d
AuthSavedCharacter *SetAuthSavedCharacterFlags(shen_name subject, std::uint32_t flags) {
    AuthSavedCharacter *const state = FindAuthCharacterState(subject);
    if (subject)
        state->flags |= flags;
    return state;
}

// @verified - d
AuthSavedCharacter *ClearAuthSavedCharacterFlags(shen_name subject, std::uint32_t flags) {
    AuthSavedCharacter *const state = FindAuthCharacterState(subject);
    if (subject)
        state->flags &= ~flags;
    return state;
}

// @verified - d
std::int32_t RecordAuthorDoor(shen_name subject) {
    for (std::int32_t index = 0; index < g_authorControlledDoorCount; ++index) {
        if (g_authorControlledDoors[index] == subject)
            return 0;
    }
    g_authorControlledDoors[g_authorControlledDoorCount++] = subject;
    return 1;
}

// @verified - d
void RestoreAuthorDoors() {
    for (std::int32_t index = 0; index < g_authorControlledDoorCount; ++index) {
        const shen_name subject = g_authorControlledDoors[index];
        if (subject)
            SetDoorSubjectEnabled(subject, 1);
    }
}

// @verified - d
void AuthorMask_Callback(void *) {
    if (g_authorMaskDelay) {
        g_authorMaskDelay = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_authorMaskDelay) - 1U);
        return;
    }
    const MapControlData *const map = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    filmSetCinemaBarVisibility(map->filmTask, FF_Hide, 30);
    g_authorMaskTask = 0;
    HLib::DestroyCurrentTask();
}

// @verified - d
std::int32_t probeSceneById(std::int32_t id) {
    for (std::int32_t index = 69; index >= 0; --index) {
        if (authorTable[index].name == static_cast<std::uint32_t>(id))
            return index;
    }
    return -1;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void sctlAuthApplyMotion() {
    std::int32_t frame, index;
    AuthSavedCharacter *state;
    const std::int32_t *arguments;
    Character *character;
    MotionWork *work;
    std::int32_t motionType;
    float speed, time;
    Vector3 position;
    Vector3i rotation;
    Vector3 displacement;
    const std::int32_t *frames = reinterpret_cast<const std::int32_t *>(g_authorController->firstInstruction);
    const std::int8_t *command;
    while (*frames != -1) {
        frame = *frames++;
        command = reinterpret_cast<const std::int8_t *>(frames);
        while (*command) {
            if (*command == 3) {
                arguments = reinterpret_cast<const std::int32_t *>(command + 4);
                state = FindAuthCharacterState(shen_name(arguments[0]));
                if (state) {
                    state->finalMotionArguments = arguments;
                    state->finalMotionFrame = frame;
                }
            }
            command += command[1] * 4 + 4;
        }
        frames = reinterpret_cast<const std::int32_t *>(command + 4);
    }
    for (index = 0, state = g_authorSavedCharacters; index < 32; ++index, ++state) {
        if (!state->finalMotionArguments)
            continue;
        arguments = state->finalMotionArguments;
        if (!(static_cast<float>(g_authorController->currentFrame) < static_cast<float>(state->finalMotionFrame) +
                    (static_cast<float>(arguments[3]) - static_cast<float>(arguments[2]) + 1.0f) / AUTH_VALUE(float, arguments, 20)))
            continue;
        character = scenFindChara(g_authorController->task, shen_name(arguments[0]));
        if (!character)
            continue;
        work = reinterpret_cast<MotionWork *>(FindTimedNodeMotion(character));
        if (work)
            motionType = 1;
        else {
            motionType = 0;
            work = charFindMotionwork(character);
        }
        if (!work)
            continue;
        AuthSetRegionFlagForApply(motionType, work, LowerBody);
        StartAuthorActorMotion(motionType, character, work, arguments[1]);
        speed = AUTH_VALUE(float, arguments, 20);
        time = static_cast<float>(arguments[2] - 1) +
            (static_cast<float>(static_cast<std::int32_t>(g_authorController->framesRemaining)) - static_cast<float>(state->finalMotionFrame)) * speed;
        SetAuthorMotionFrame(motionType, work, time);
        SetAuthorMotionSpeed(motionType, work, speed);
        if (g_authorApplyYawOffset) {
            GetCharacterLocalPosition(character, &position);
            GetCharacterLocalRotation(character, &rotation);
            ApplyAuthorMotionPose(motionType, arguments[1], character, time, &position, &rotation);
            SetCharacterLocalPosition(character, &position);
            SetCharacterLocalRotation(character, &rotation);
        } else {
            SampleAuthorMotionRoot(motionType, arguments[1], character, time, &position);
            GetCharacterLocalRotation(character, &rotation);
            position.y = 0.0f;
            if (arguments[4] & 1)
                position.x = -position.x;
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::UnitCurrentMatrix();
            Matrix4x4::RotateAroundY_Wrapper(rotation.y);
            Matrix4x4::TransformPoint(position, displacement);
            PopCurrentMatrix(1);
            GetCharacterLocalPosition(character, &position);
            displacement.x += position.x;
            displacement.y += position.y;
            displacement.z += position.z;
            SetCharacterLocalPosition(character, &displacement);
        }
    }
}
#endif

// @verified - d
std::int32_t PrepareAuthorHands(Character *character, shen_name subject) {
    std::int32_t index;
    std::int32_t result = 2;
    for (index = 0; index < g_authorHandAdmissionCount; ++index)
        if (g_authorHandAdmissionNames[index] == subject)
            break;
    if (index == g_authorHandAdmissionCount) {
        g_authorHandAdmissionNames[index] = subject;
        g_authorHandAdmissionCount = index + 1;
        if (GetLoadedHandModelFlags(character) != 3) {
            result = SetLowQualityHands(character);
            AuthSavedCharacter *const state = FindAuthCharacterState(subject);
            if (state && result == 1)
                state->flags |= AuthRestoreLowQualityHands;
        }
    }
    return result;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static std::int32_t ExecuteAuthorMusicCommand(const char *name) {
    std::int32_t result = 0;
    if (std::strstr(name, "fade_out")) {
        spmFadeOut(0);
        result = 1;
    } else if (std::strstr(name, "bgm_off")) {
        result = 1;
        shenmue_s2_event_EV_LoadAudio(GetCurrentBgm(), UINT32_MAX);
    } else {
        const char *const command = std::strstr(name, "bgm00");
        if (command) {
            shenmue_s2_event_EV_LoadAudio(GetCurrentBgm(), static_cast<std::int8_t>(command[5]) - '0');
            EV_spmBeginSampleExecute();
            result = 1;
        }
    }
    return result;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void CaptureAuthorMapMasks() {
    std::uint32_t modelMask = 0, fieldMask = 0;
    for (std::int32_t index = 0; index < 32; ++index) {
        const std::uint32_t model = GetMapModelEnabled(index);
        const std::uint32_t field = GetMapFieldDepthSlotEnabled(index);
        modelMask <<= 1;
        if (model)
            modelMask |= 1;
        fieldMask <<= 1;
        if (field)
            fieldMask |= 1;
    }
    g_authorSavedMapModelMask = modelMask;
    g_authorSavedMapFieldMask = fieldMask;
}
#endif

// @verified - d
void RestoreAuthorMapSlots() {
    std::uint32_t modelMask = g_authorSavedMapModelMask, fieldMask = g_authorSavedMapFieldMask;
    for (std::int32_t index = 31; index >= 0; --index) {
        SetMapModelEnabled(modelMask & 1U, index);
        modelMask >>= 1U;
        SetMapFieldDepthSlotEnabled(fieldMask & 1U, index);
        fieldMask >>= 1U;
    }
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void FinalizeAuthorSkip() {
    std::int32_t presetFrame = -1, directFrame = -1;
    const AuthDynamicCommand *const direct = FindLastAuthorDynamicCommand(20);
    const AuthDynamicCommand *const preset = FindLastAuthorDynamicCommand(11);
    if (!direct && !preset)
        return;
    if (direct)
        directFrame = direct->frame;
    if (preset)
        presetFrame = preset->frame;
    const MapControlData *const map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    HLib::Task *const film = map->filmTask;
    ScreenFilmColourRequest request;
    if (presetFrame < directFrame) {
        if (direct->operation != 20)
            return;
        request.durationFrames = 1;
        request.start[0] = AUTH_VALUE(std::uint8_t, direct, 8);
        request.start[1] = AUTH_VALUE(std::uint8_t, direct, 9);
        request.start[2] = AUTH_VALUE(std::uint8_t, direct, 10);
        request.start[3] = AUTH_VALUE(std::uint8_t, direct, 11);
        request.target[0] = AUTH_VALUE(std::uint8_t, direct, 12);
        request.target[1] = AUTH_VALUE(std::uint8_t, direct, 13);
        request.target[2] = AUTH_VALUE(std::uint8_t, direct, 14);
        request.target[3] = AUTH_VALUE(std::uint8_t, direct, 15);
    } else {
        if (preset->operation != 11)
            return;
        request.durationFrames = 1;
        request.start[0] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][0];
        request.start[1] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][1];
        request.start[2] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][2];
        request.start[3] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][3];
        request.target[0] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][4];
        request.target[1] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][5];
        request.target[2] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][6];
        request.target[3] = kAuthorFilmColours[AUTH_VALUE(std::int16_t, preset, 8)][7];
    }
    WaitForNextTask(film, request, 0);
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void AuthorControllerNullsub() {
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
static void ReleaseAuthorSplines(AuthSplineSet *splines) {
    for (std::int32_t index = 0; index < 9; ++index)
        if (splines->tracks[index].count)
            HLib::FreeTaskData(splines->tracks[index].coefficients);
}
#endif

} // namespace s2
} // namespace shenmue

#endif
