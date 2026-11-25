#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/fight_stage.hpp"
#include "s2/fight_dialogue_tables.hpp"
#include "s2/battle_qte.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/math.hpp"
#include "s2/input_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/math.hpp"
#include <cstring>
#include "s2/field_event_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/model_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#else
#include "s2/fight_stage.hpp"
#include "s2/fight_dialogue_tables.hpp"
#include "s2/fight.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/face.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/map_control.hpp"
#include "s2/event_runtime.hpp"
#include "s2/player.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/input.hpp"
#include "s2/debug_text.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <cmath>
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t InstructionAssignMotScaleAndDir(Character *character, std::uint8_t scale, std::uint8_t direction);
std::uint8_t *InstructionAssignFigureMode(Character *character, std::uint8_t mode);
void InstructionSetRegionEvalTime(float frame, Character *character);
void InstructionSetLayerEndFrame(float frame, Character *character);
void vfPublishInstructionActorMotion(FightActorRuntimePrefix *actor, Character *character, shen_name name, std::int32_t motion);
std::uint32_t InstructionSetRegionFlag(Character *character);
void InstructionSetLocalRotation(Character *character, const Vector3i *rotation);
void InstructionGetLocalRotation(const Character *character, Vector3i *rotation);
std::int32_t StepCharacterYawTowardPoint(Character *character, const Vector3 *target, std::int32_t step);
void InstructionSetLocalPosition(Character *character, const Vector3 *position);
void InstructionBonePosition(Character *character, std::int32_t bone, Vector3 *position);
void InstructionGetLocalPosition(const Character *character, Vector3 *position);
float InstructionMotionFrame(Character *character);
std::uint32_t InstructionSetExtraFlag(Character *character, std::int32_t unused);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// vfSetMoveInstructionConfiguration owns this configuration; VFHD borrows the caller's result record.
shen_name instructionCharacter{}, instructionSecondCharacter{};

std::int16_t instructionProgram{}, instructionSecondProgram{};

std::byte *instructionResult{};

// Native g_vf_dialogue_tbl: all four banks share one native 24-byte record array.
std::int32_t dialoguePlayerFocus{};

struct InstructionCommand {
    std::int32_t operation, argument;
};

constexpr InstructionCommand instructionCommands0[]{{8, 8}, {7, 128}, {3, 362}, {6, 0}};

constexpr InstructionCommand instructionCommands1[]{{8, 8}, {7, 64}, {3, 582}, {6, 0}};

constexpr InstructionCommand instructionCommands2[]{{8, 8}, {7, 67}, {3, 329}, {6, 0}};

constexpr InstructionCommand instructionCommands3[]{{8, 8}, {7, 3}, {3, 1}, {6, 0}};

constexpr InstructionCommand instructionCommands4[]{{8, 53}, {7, 8}, {5, 134218140}, {10, 1}, {7, 496}, {3, -1}, {6, 0}};

constexpr InstructionCommand instructionCommands5[]{{8, 6}, {7, 128}, {3, 652}, {6, 0}};

constexpr InstructionCommand instructionCommands6[]{{8, 6}, {7, 128}, {7, 3}, {3, 281}, {11, 10}, {6, 0}};

constexpr InstructionCommand instructionCommands7[]{{8, 6}, {7, 128}, {7, 3}, {3, 134217799}, {11, 10}, {6, 0}};

constexpr InstructionCommand instructionCommands8[]{{8, 53}, {7, 8}, {5, 134218140}, {10, 1}, {7, 496}, {3, -1}, {11, 20}, {6, 0}};

constexpr InstructionCommand instructionCommands9[]{{8, 8}, {7, 128}, {7, 128}, {3, 583}, {6, 0}};

constexpr InstructionCommand instructionCommands10[]{{8, 8}, {7, 128}, {7, 129}, {3, 56}, {6, 0}};

struct InstructionProgram {
    std::uint8_t mode;
    const InstructionCommand *commands;
    std::uint8_t partnerMode;
};

constexpr InstructionProgram instructionPrograms[]{{0, instructionCommands0, 0}, {0, instructionCommands1, 0}, {0, instructionCommands2, 0},
    {0, instructionCommands3, 0}, {0, instructionCommands4, 1}, {0, instructionCommands5, 0}, {0, instructionCommands6, 0}, {3, instructionCommands7, 2},
    {0, instructionCommands8, 1}, {0, instructionCommands9, 0}, {0, instructionCommands10, 0}};

#pragma pack(push, 1)

struct InstructionState {
    std::int16_t program;
    std::uint8_t done, timedOut, startCountdown, endCountdown, frameCountdown, maximumWindow, skippedFrames;
    std::uint8_t outcome, progress, failure, analysis;
    std::byte reserved0d[3];
    std::uint32_t diagnostic;
    std::uint8_t delay, committedAttempts, endInstruction, ended, reserved18;
    std::int8_t instruction, historyIndex;
    std::uint8_t heldDirection;
    std::int32_t pendingMask, previousMask, resolvedMask;
    std::int16_t previousDirection, chosenDirection;
    std::int32_t motion;
    float endFrame;
    std::uint8_t attempts;
    std::byte reserved35[3];
};

#pragma pack(pop)

static_assert(sizeof(InstructionState) == 56);

static_assert(offsetof(InstructionState, motion) == 44);

struct InstructionHistory {
    std::int16_t held, pressed;
    std::uint32_t reserved;
};

static_assert(sizeof(InstructionHistory) == 8);

constexpr struct {
    std::uint16_t delay;
    const char *voice;
} instructionVoices[]{{0, "0"}, {10, "0068C023"}, {5, "0064E005"}, {5, "0064E006"}, {5, "0064E007"}};

struct InstructionDiagnosis {
    std::uint8_t kind{}, delay{}, attempts{};
    std::uint32_t mask{};
};

std::int8_t instructionProgramResult{}, instructionPoseResult{};

std::int32_t instructionCompletionDelay{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace {

template <class T> void InstructionStore(VFHD_t *owner, std::size_t offset, T value);
std::int16_t DialogueIndex(const VFMA_t &game);
void SetDialogueIndex(VFMA_t &game, std::int16_t index);
FightDialogueRecord *DialogueBank(const VFMA_t &game);
InstructionState &FirstInstruction(VFHD_t *owner);
InstructionState &SecondInstruction(VFHD_t *owner);
const InstructionHistory &InstructionHistoryAt(const VFHD_t *owner, std::int64_t index);
bool InstructionDirectionChanged(std::uint8_t first, std::uint8_t second);
bool InstructionFacesCamera(const VFHD_t *owner);
void RecordInstructionInput(VFActor *actor);
std::int16_t LastInstructionDirection(const VFHD_t *owner);
std::uint8_t InstructionCloseBetweenCharacters(const VFHD_t *owner, const InstructionState &state);
bool InstructionHitWindowAdmitted(const VFHD_t *owner, shen_name name);
void QueueInstructionDialogue(VFHD_t *owner, std::int32_t index);
void QueueInstructionMotion(VFHD_t *owner, std::int32_t motion);
void RunInstructionProgram(VFActor *actor, VFHD_t *owner, InstructionState &state);
InstructionDiagnosis DiagnoseInstructionProgram(VFHD_t *owner, InstructionState &state);
std::int8_t UpdateInstructionPrograms(VFActor *actor);
void InitializeInstructionHistory(VFHD_t *owner);
void PublishInstructionActorMotion(VFActor *actor, Character *character, shen_name name, std::int32_t motion);
void ConvergeInstructionYaw(Character *character, const Vector3 &target, std::int32_t maximumStep);
bool InstructionMotionEnded(Character *character);
std::uint16_t InstructionAnimation(Character *character);
std::uint8_t UpdateInstructionPose(VFActor *actor);
std::uint8_t InstructionRequiredInputs(const InstructionState &state);
bool SuccessfulInstruction(const VFHD_t *owner, const InstructionState &first, const InstructionState &second);
bool InstructionPreviewPastEnd(VFHD_t *owner, const InstructionState &first, const InstructionState &second);
void PublishInstructionResult(VFHD_t *owner, const InstructionState &first, const InstructionState &second);

} // namespace

VFActor *vfGetActorFromSlot(VFMA_t *game, shen_name name);
const char *vfGetFightDialogueVoice(VFMA_t *game, std::int16_t index);
void vfSetFightDialogue(VFMA_t *game, std::int16_t index);
void vfInitializeFightDialogue(VFMA_t *game);
void vfmaSetupDialogueReactionsEtc(VFMA_t *game);
std::byte *vfSetMoveInstructionConfiguration(
    shen_name character, shen_name secondCharacter, std::int16_t program, std::int16_t secondProgram, std::byte *borrowedResult);
std::int32_t vfApplyMoveInstructionConfiguration(VFHD_t *owner);
void vfUpdateVFHDState(VFActor *actor);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

struct InstructionState {
    std::int16_t program;
    std::uint8_t done, timedOut;
    std::int8_t startCountdown, endCountdown, frameCountdown;
    std::uint8_t maximumWindow, skippedFrames;
    std::int8_t outcome, progress, failure, analysis;
    std::byte reserved0d[3];
    std::uint32_t diagnostic;
    std::uint8_t delay, committedAttempts, endInstruction, ended, reserved18;
    std::int8_t instruction, historyIndex;
    std::uint8_t heldDirection;
    std::int32_t pendingMask, previousMask, resolvedMask;
    std::int16_t previousDirection, chosenDirection;
    std::int32_t motion;
    float endFrame;
    std::int8_t attempts;
    std::byte reserved35[3];
};

typedef char InstructionStateSizeCheck[sizeof(InstructionState) == 56 ? 1 : -1];

typedef char InstructionMotionOffsetCheck[offsetof(InstructionState, motion) == 44 ? 1 : -1];

} // namespace

struct VFHD_t {
    FightRuntimePrefix *game;
    InstructionState instructionStates[2];
    struct {
        std::int16_t held, pressed;
        std::uint32_t reserved;
    } history[120];
    std::int8_t historyIndex;
    std::byte retained435[3];
    std::int8_t state;
    std::byte retained439;
    std::int8_t programMode, partnerMode, firstPhase, secondPhase, committed, firstHit, secondHit, turnOnReaction;
    std::int16_t firstHitStart, firstHitEnd;
    std::byte retained446[2];
    float firstEvalTime, firstEndFrame;
    std::int8_t holdOnSuccess;
    std::byte retained451;
    std::int16_t secondHitStart, secondHitEnd;
    std::byte retained456[2];
    std::uint32_t firstMotion, secondMotion;
    std::int16_t firstTimer, secondTimer, dialogueCountdown, secondHitReaction, firstHitReaction;
    std::byte retained46a[2];
    shen_name firstName, secondName;
    Character *character, *secondCharacter;
    std::uint32_t requestedMotion, secondRequestedMotion, currentMotion, secondCurrentMotion;
    std::uint8_t republish, secondRepublish;
    std::byte retained48e[2];
    std::byte *result;
    std::uint16_t programIndices[2];
    std::uint32_t frame;
    std::int32_t startYaw;
    std::int32_t firstPublishedYaw, secondPublishedYaw;
    std::uint32_t firstPublishedMotion, secondPublishedMotion;
    std::int8_t inputCount, instructionPhase;
    std::byte retained4b2[2];
    Vector3 startPosition;
    std::byte retained4c0[0x0c];
    Vector3i startRotation;
    std::byte retained4d8[0x10];
};

typedef char NativeInstructionOwnerSizeCheck[sizeof(VFHD_t) == 0x4e8 ? 1 : -1];

typedef char NativeInstructionCharacterCheck[offsetof(VFHD_t, character) == 0x474 ? 1 : -1];

typedef char NativeInstructionResultCheck[offsetof(VFHD_t, result) == 0x490 ? 1 : -1];

namespace {

struct InstructionCommand {
    std::int32_t operation, argument;
};

struct InstructionProgram {
    const char *name;
    std::uint8_t mode;
    const InstructionCommand *commands;
    std::uint8_t partnerMode;
};

struct InstructionHistory {
    std::int16_t held, pressed;
    std::uint32_t reserved;
};

typedef char InstructionHistorySizeCheck[sizeof(InstructionHistory) == 8 ? 1 : -1];

struct InstructionDiagnosis {
    std::byte reserved0[2];
    std::uint8_t kind;
    std::byte reserved3[2];
    std::uint8_t delay, attempts;
    std::uint32_t mask;
};

typedef char InstructionDiagnosisSizeCheck[sizeof(InstructionDiagnosis) == 12 ? 1 : -1];

} // namespace

FightInstructionConfiguration g_fightInstructionConfiguration;

FightDialogueRecord dialogueRecords[253] = {{0, 0, 0, {0, 0, 0}, 0, -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, 0, -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, 0, -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100A002", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A003", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A004", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100A005", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A006", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A007", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100A008", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0100A009", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0100A010", -1, {0, 0}},
    {30, 0, 0, {0, 0, 0}, "0100A011", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0100A012", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100K005", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100L018", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100M016", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100G001", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G002", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G003", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100G004", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G005", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G006", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100G007", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G008", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G009", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100G010", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G011", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100G012", -1, {0, 0}},
    {30, 0, 0, {0, 0, 0}, "0100I001", 48, {0, 0}}, {60, 0, 0, {0, 0, 0}, "0100A009", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100I002", 50, {0, 0}},
    {60, 1, 0, {0, 0, 0}, "0100A017", 51, {0, 0}}, {80, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {0, 2, 0, {0, 0, 0}, "0100I003", 53, {0, 0}},
    {55, 0, 0, {0, 0, 0}, "0100A018", 54, {0, 0}}, {1, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0100I004", -1, {0, 0}},
    {30, 0, 0, {0, 0, 0}, "0100I005", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0100I006", 58, {0, 0}}, {90, 0, 0, {0, 0, 0}, "0100I007", -1, {0, 0}},
    {0, 3, 0, {0, 0, 0}, 0, 60, {0, 0}}, {15, 0, 0, {0, 0, 0}, "0032B016", 61, {0, 0}}, {90, 9, 0, {0, 0, 0}, 0, 62, {0, 0}},
    {15, 0, 0, {0, 0, 0}, "0032A014", 63, {0, 0}}, {75, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {0, 8, 0, {0, 0, 0}, 0, 65, {0, 0}},
    {15, 0, 0, {0, 0, 0}, "0032B022", 66, {0, 0}}, {80, 4, 0, {0, 0, 0}, 0, 67, {0, 0}}, {15, 0, 0, {0, 0, 0}, "0032A026", 68, {0, 0}},
    {90, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {0, 0, 0, {0, 0, 0}, 0, -1, {0, 0}}, {0, 4, 0, {0, 0, 0}, 0, 71, {0, 0}},
    {15, 0, 0, {0, 0, 0}, "0032A027", 72, {0, 0}}, {35, 8, 0, {0, 0, 0}, 0, 73, {0, 0}}, {15, 0, 0, {0, 0, 0}, "0032B023", 74, {0, 0}},
    {70, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {0, 0, 0, {0, 0, 0}, 0, -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B061", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B059", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B060", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B063", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B064", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B066", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B067", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B068", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B069", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B070", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B071", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B072", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B065", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B062", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B073", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N001", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100N002", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N003", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N004", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100N005", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N006", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N007", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100N008", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N009", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N015", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100N016", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N002", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N017", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100N018", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100K027", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100K022", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100L015", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100L016", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100K008", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100L017", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100L017", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "00000000", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B074", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B068", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B075", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B076", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B077", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B078", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B067", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B079", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B080", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B081", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B069", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B082", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B083", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B084", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B085", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B063", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B086", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B064", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B066", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B087", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B088", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0026B089", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0026B065", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100B072", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100B073", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100B074", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100B075", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100A041", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A042", -1, {0, 0}}, {0, 1, 0, {0, 0, 0}, "0100A043", 150, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100P004", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100P005", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100P001", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100P002", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100P003", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A044", -1, {0, 0}},
    {0, 1, 0, {0, 0, 0}, "0100A047", 151, {0, 0}}, {75, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {75, 0, 0, {0, 0, 0}, 0, -2, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100Q009", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A081", -1, {0, 0}}, {22, 5, 0, {0, 0, 0}, "0100Q010", 155, {0, 0}},
    {50, 1, 0, {0, 0, 0}, 0, 156, {0, 0}}, {40, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {30, 7, 0, {0, 0, 0}, "0100A050", 158, {0, 0}},
    {120, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {0, 7, 0, {0, 0, 0}, "0100A052", 160, {0, 0}}, {50, 0, 0, {0, 0, 0}, 0, -2, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100R007", -1, {0, 0}}, {0, 6, 0, {0, 0, 0}, "0100R008", 163, {0, 0}}, {90, 1, 0, {0, 0, 0}, "0100A054", 164, {0, 0}},
    {90, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100R009", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100R010", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100R011", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100R012", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100R013", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100R014", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100R015", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100R016", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100R017", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0032B019", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0032B022", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0032B023", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0064B007", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0064A006", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0064B008", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0064A007", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0064B004", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0064B021", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0064B022", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0232A023", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0232A024", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0232A025", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0232A026", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0232A027", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0232B048", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0232B049", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0232B050", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0232B051", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0232B052", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100N010", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N011", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N012", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100N013", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100N014", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100J008", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100J009", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100J010", -1, {0, 0}}, {22, 5, 0, {0, 0, 0}, "0100Q017", 203, {0, 0}},
    {80, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {22, 5, 0, {0, 0, 0}, "0100Q018", 205, {0, 0}}, {80, 0, 0, {0, 0, 0}, 0, -2, {0, 0}},
    {22, 5, 0, {0, 0, 0}, "0100Q019", 207, {0, 0}}, {80, 0, 0, {0, 0, 0}, 0, -2, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A083", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100Q020", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100A082", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100Q011", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100Q013", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100Q014", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0100L015", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0100L016", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275D002", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275D003", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275D004", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275D005", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275G002", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275G003", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275G004", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275G005", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275C005", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275C006", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275C007", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275C008", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275D006", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275D007", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275D008", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275G006", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275G007", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275G008", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275C009", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275C010", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275C011", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275D009", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0275D010", -1, {0, 0}},
    {30, 0, 0, {0, 0, 0}, "0275D011", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0275D012", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0275G009", -1, {0, 0}},
    {0, 0, 0, {0, 0, 0}, "0275G010", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0275G011", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0275G012", -1, {0, 0}},
    {30, 0, 0, {0, 0, 0}, "0275C012", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, "0275C013", -1, {0, 0}}, {30, 0, 0, {0, 0, 0}, "0275C014", -1, {0, 0}},
    {30, 0, 0, {0, 0, 0}, "0275C015", -1, {0, 0}}, {0, 0, 0, {0, 0, 0}, 0, -1, {0, 0}}};

FightDialogueRecord *const dialogueBanks[4] = {dialogueRecords, dialogueRecords + 1, dialogueRecords + 2, dialogueRecords + 3};

static const std::uint8_t dialogueReactionList0[] = {3, 8, 11, 12};
static const std::uint8_t dialogueReactionList1[] = {7, 0, 7, 8, 9, 10, 20, 21};
static const std::uint8_t dialogueReactionList2[] = {3, 14, 15, 16};
static const std::uint8_t dialogueReactionList3[] = {2, 2, 17};
static const std::uint8_t dialogueReactionList4[] = {1, 4};
static const std::uint8_t dialogueReactionList5[] = {2, 13, 19};
static const std::uint8_t dialogueReactionList6[] = {1, 6};
static const std::uint8_t dialogueReactionList7[] = {1, 18};
static const std::uint8_t *const dialogueReactionLists[2][5] = {{dialogueReactionList0, dialogueReactionList1, 0, dialogueReactionList2, 0},
    {dialogueReactionList3, dialogueReactionList4, dialogueReactionList5, dialogueReactionList6, dialogueReactionList7}};
static const float dialogueReactionChances[5] = {0.3f, 0.3f, 1.0f, 0.2f, 1.0f};

std::int32_t dialoguePlayerFocus;

namespace {

const InstructionCommand instructionCommands0[] = {{8, 8}, {7, 128}, {3, 362}, {6, 0}};

const InstructionCommand instructionCommands1[] = {{8, 8}, {7, 64}, {3, 582}, {6, 0}};

const InstructionCommand instructionCommands2[] = {{8, 8}, {7, 67}, {3, 329}, {6, 0}};

const InstructionCommand instructionCommands3[] = {{8, 8}, {7, 3}, {3, 1}, {6, 0}};

const InstructionCommand instructionCommands4[] = {{8, 53}, {7, 8}, {5, 134218140}, {10, 1}, {9, 10}, {7, 496}, {3, -1}, {6, 0}};

const InstructionCommand instructionCommands5[] = {{8, 6}, {7, 128}, {3, 652}, {6, 0}};

const InstructionCommand instructionCommands6[] = {{8, 6}, {7, 128}, {7, 3}, {3, 281}, {11, 10}, {6, 0}};

const InstructionCommand instructionCommands7[] = {{8, 6}, {7, 128}, {7, 3}, {3, 134217799}, {11, 10}, {6, 0}};

const InstructionCommand instructionCommands8[] = {{8, 53}, {7, 8}, {5, 134218140}, {10, 1}, {9, 10}, {7, 496}, {3, -1}, {11, 20}, {6, 0}};

const InstructionCommand instructionCommands9[] = {{8, 8}, {7, 128}, {7, 128}, {3, 583}, {6, 0}};

const InstructionCommand instructionCommands10[] = {{8, 8}, {7, 128}, {7, 129}, {3, 56}, {6, 0}};

const InstructionProgram instructionPrograms[] = {{"00:DUMMY", 0, instructionCommands0, 0}, {"01:SYOUSUI", 0, instructionCommands1, 0},
    {"02:SYOUSUI", 0, instructionCommands2, 0}, {"03:SYOUKINDA", 0, instructionCommands3, 0}, {"04:NAGE", 0, instructionCommands4, 1},
    {"05:GAIMON", 0, instructionCommands5, 0}, {"06:GAIMON", 0, instructionCommands6, 0}, {"07:GAIMON", 3, instructionCommands7, 2},
    {"08:NAGE", 0, instructionCommands8, 1}, {"09:KENKA UPPER", 0, instructionCommands9, 0}, {"10:KENKA UPPER", 0, instructionCommands10, 0}};

#pragma pack(push, 1)

#pragma pack(pop)

const struct {
    std::uint16_t delay;
    const char *voice;
} instructionVoices[] = {{0, "0"}, {10, "0068C023"}, {5, "0064E005"}, {5, "0064E006"}, {5, "0064E007"}};

char instructionProgramResult = 0;

std::int8_t instructionPoseResult = 0;

std::int32_t instructionCompletionDelay = 0;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void vfSetMoveInstructionConfiguration(shen_name character, shen_name secondCharacter, std::int16_t program, std::int16_t secondProgram, std::byte *result);
std::int32_t vfApplyStoredMoveInstructionConfiguration(VFHD_t *owner);
void vfInitializeFightDialogue(FightRuntimePrefix *game);
void vfClAicaReact(std::uint32_t flags, const Vector3 *position);
void vfSetFightDialogue(FightRuntimePrefix *game, std::int16_t index);
const char *vfGetFightDialogueVoice(FightRuntimePrefix *game, std::int16_t index);
bool vfHasDialoguePlayerFocus();
bool vfRequestFightDialogue(FightRuntimePrefix *game, std::int16_t index);
void vfTryFightDialogue(FightRuntimePrefix *game, std::int16_t index);
void vfmaSetupDialogueReactionsEtc(FightRuntimePrefix *game);
std::int32_t vfApplyMoveInstructionConfiguration(
    VFHD_t *owner, shen_name character, shen_name secondCharacter, std::uint16_t program, std::uint16_t secondProgram, std::byte *result);
void vfUpdateVFHDState(FightActorRuntimePrefix *actor);

namespace {

template <class T> T &DialogueValue(void *owner, std::size_t offset);
FightDialogueRecord *DialogueBank(FightRuntimePrefix *game);
template <class T> T InstructionValue(const VFHD_t *owner, std::size_t offset);
template <class T> void InstructionStore(VFHD_t *owner, std::size_t offset, T value);
template <class T> T &InstructionActorValue(void *owner, std::size_t offset);
Vector3 InstructionCharacterPosition(Character *character);
Vector3i InstructionCharacterRotation(Character *character);
std::int32_t InstructionCharacterYaw(CharacterNode *node);
void SetInstructionState(VFHD_t *owner, std::uint8_t state);
void SetInstructionPhase(VFHD_t *owner, std::uint8_t &phase, std::uint8_t state);
void InstructionFailureNullsub();
void InstructionAnalysisNullsub(VFHD_t *, const InstructionState &state);
InstructionState &FirstInstruction(VFHD_t *owner);
InstructionState &SecondInstruction(VFHD_t *owner);
const InstructionHistory &InstructionHistoryAt(const VFHD_t *owner, std::int32_t index);
std::uint8_t MaximumInstructionInputs(const InstructionState &first, const InstructionState &second);
void InitializeInstructionState(InstructionState &state);
std::uint8_t DirectionSector(std::int32_t direction);
std::int32_t InstructionDirectionChanged(std::int16_t first, std::int16_t second);
bool InstructionFacesCamera(const VFHD_t *owner);
std::int32_t DecodeInstructionInput(const VFHD_t *owner, std::int16_t input);
void CountInstructionCollisionInput(FightActorRuntimePrefix *actor);
void RecordInstructionInput(FightActorRuntimePrefix *actor);
std::int16_t LastInstructionDirection(const VFHD_t *owner);
bool InstructionPartnerReady(shen_name name);
std::int32_t InstructionPartnerWithinRange(const VFHD_t *owner);
std::uint8_t InstructionCloseBetweenCharacters(const VFHD_t *owner, const InstructionState &state);
std::int32_t InstructionHitWindowAdmitted(const VFHD_t *owner, shen_name name);
std::int32_t SeparateInstructionCharacters(Character *first, Character *second);
std::int32_t PlaceInstructionCharacterPair(Character *first, Character *second);
void QueueInstructionDialogue(VFHD_t *owner, std::int32_t index);
void TickInstructionDialogue(VFHD_t *owner);
void QueueInstructionMotion(VFHD_t *owner, std::int32_t slot, std::int32_t motion);
std::int32_t InstructionDiagnosisNullsub(VFHD_t *, std::int16_t, std::int16_t, InstructionState &, InstructionState &);
void ResetInstructionPairState(VFHD_t *owner, std::int16_t, std::int16_t, InstructionState &);
std::int32_t PeekProgramInstruction(const InstructionState &state, std::int8_t instruction);
std::int32_t RunInstructionProgram(FightActorRuntimePrefix *actor, VFHD_t *owner, InstructionState &state, std::int32_t primary);
std::int32_t DiagnoseInstructionProgram(VFHD_t *owner, InstructionState &state, InstructionDiagnosis *result);
std::int8_t UpdateInstructionPrograms(FightActorRuntimePrefix *actor, InstructionState &first, InstructionState &second);
void InitializeInstructionHistory(VFHD_t *owner);
void PublishInstructionActorMotion(FightActorRuntimePrefix *actor, Character *character, shen_name name, std::int32_t motion);
void ConvergeInstructionYaw(Character *character, const Vector3 &target, std::int32_t maximumStep);
bool InstructionMotionEnded(Character *character);
std::uint16_t InstructionAnimation(Character *character);
float InstructionPoseFrame(Character *character);
std::int32_t UpdateInstructionPose(FightActorRuntimePrefix *actor, InstructionState &first, InstructionState &second);
std::uint8_t InstructionRequiredInputs(const InstructionState &state);
bool SuccessfulInstruction(const VFHD_t *owner, const InstructionState &first, const InstructionState &second);
bool InstructionPreviewPastEnd(VFHD_t *owner, const InstructionState &first, const InstructionState &second);
void ResetInstructionPreview(VFHD_t *owner);
void PublishInstructionResult(VFHD_t *owner, const InstructionState &first, const InstructionState &second);

} // namespace
#else
std::byte *vfSetMoveInstructionConfiguration(
    shen_name character, shen_name secondCharacter, std::int16_t program, std::int16_t secondProgram, std::byte *borrowedResult);
VFHD_t *AllocateVFHD(VFMA_t *game);
void vfFreeVFHDSpace(VFMA_t *game);
void vfInitializeFightDialogue(VFMA_t *game);
void vfSetFightDialogue(VFMA_t *game, std::int16_t index);
const char *vfGetFightDialogueVoice(VFMA_t *game, std::int16_t index);
bool vfHasDialoguePlayerFocus();
void vfmaSetupDialogueReactionsEtc(VFMA_t *game);
VFActor *vfGetActorFromSlot(VFMA_t *game, shen_name name);
std::int32_t vfApplyMoveInstructionConfiguration(VFHD_t *owner);
void vfUpdateVFHDState(VFActor *actor);
std::int32_t SetTrainingFightReaction(std::uint16_t reaction);

namespace {

FightDialogueRecord *DialogueBank(const VFMA_t &game);
template <class T> T InstructionValue(const VFHD_t *owner, std::size_t offset);
template <class T> void InstructionStore(VFHD_t *owner, std::size_t offset, T value);
std::int16_t DialogueIndex(const VFMA_t &game);
void SetDialogueIndex(VFMA_t &game, std::int16_t index);
InstructionState &FirstInstruction(VFHD_t *owner);
InstructionState &SecondInstruction(VFHD_t *owner);
const InstructionHistory &InstructionHistoryAt(const VFHD_t *owner, std::int64_t index);
void InitializeInstructionState(InstructionState &state);
std::uint8_t DirectionSector(std::uint8_t direction);
bool InstructionDirectionChanged(std::uint8_t first, std::uint8_t second);
bool InstructionFacesCamera(const VFHD_t *owner);
std::int16_t DecodeInstructionInput(const VFHD_t *owner, std::uint8_t input);
void RecordInstructionInput(VFActor *actor);
std::int16_t LastInstructionDirection(const VFHD_t *owner);
std::uint8_t InstructionCloseBetweenCharacters(const VFHD_t *owner, const InstructionState &state);
bool InstructionHitWindowAdmitted(const VFHD_t *owner, shen_name name);
void QueueInstructionDialogue(VFHD_t *owner, std::int32_t index);
void QueueInstructionMotion(VFHD_t *owner, std::int32_t motion);
void RunInstructionProgram(VFActor *actor, VFHD_t *owner, InstructionState &state);
std::uint8_t InstructionMaskDiagnosis(std::int32_t mask, bool missing, bool directionRequired);
InstructionDiagnosis DiagnoseInstructionProgram(VFHD_t *owner, InstructionState &state);
std::int8_t UpdateInstructionPrograms(VFActor *actor);
void InitializeInstructionHistory(VFHD_t *owner);
void PublishInstructionActorMotion(VFActor *actor, Character *character, shen_name name, std::int32_t motion);
void ConvergeInstructionYaw(Character *character, const Vector3 &target, std::int32_t maximumStep);
void AdjustInstructionPairPosition(VFActor *actor, const Vector3 &start, const Vector3 &otherStart);
bool InstructionMotionEnded(Character *character);
std::uint16_t InstructionAnimation(Character *character);
std::uint8_t UpdateInstructionPose(VFActor *actor);
std::uint8_t InstructionRequiredInputs(const InstructionState &state);
bool SuccessfulInstruction(const VFHD_t *owner, const InstructionState &first, const InstructionState &second);
bool InstructionPreviewPastEnd(VFHD_t *owner, const InstructionState &first, const InstructionState &second);
void ResetInstructionPreview(VFHD_t *owner);
void PublishInstructionResult(VFHD_t *owner, const InstructionState &first, const InstructionState &second);

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfSetMoveInstructionConfiguration(shen_name character, shen_name secondCharacter, std::int16_t program, std::int16_t secondProgram, std::byte *result) {
    g_fightInstructionConfiguration.character = character;
    g_fightInstructionConfiguration.secondCharacter = secondCharacter;
    g_fightInstructionConfiguration.program = program;
    g_fightInstructionConfiguration.secondProgram = secondProgram;
    g_fightInstructionConfiguration.result = result;
}

std::int32_t vfApplyMoveInstructionConfiguration(
    VFHD_t *owner, shen_name character, shen_name secondCharacter, std::uint16_t program, std::uint16_t secondProgram, std::byte *result);

std::int32_t vfApplyStoredMoveInstructionConfiguration(VFHD_t *owner) {
    FightInstructionConfiguration *configuration = &g_fightInstructionConfiguration;
    return vfApplyMoveInstructionConfiguration(owner, shen_name(configuration->character), shen_name(configuration->secondCharacter), configuration->program,
        configuration->secondProgram, configuration->result);
}

#else

// Names and signed program indices remain native; the result is borrowed.
// @verified - p
std::byte *vfSetMoveInstructionConfiguration(
    shen_name character, shen_name secondCharacter, std::int16_t program, std::int16_t secondProgram, std::byte *borrowedResult) {
    instructionCharacter = character;
    instructionSecondCharacter = secondCharacter;
    instructionProgram = program;
    instructionSecondProgram = secondProgram;
    instructionResult = borrowedResult;
    return borrowedResult;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// VFHD owns its aligned native task-data allocation.
// @verified - p
VFHD_t *AllocateVFHD(VFMA_t *game) {
    auto *const owner = static_cast<VFHD_t *>(HLib::AllocateTaskData(0x520, SHEN_NAME(VFHD)));
    HLib::AssignDynamicAlignmentForTaskData(owner);
    owner->game = game;
    InstructionStore(owner, 0x440, std::uint8_t{0});
    return owner;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void vfFreeVFHDSpace(VFMA_t *game) {
    if (game->dialogue) {
        HLib::FreeHeapSpace(game->dialogue, SHEN_NAME(VFHD));
        game->dialogue = nullptr;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

template <class T> inline T &DialogueValue(void *owner, std::size_t offset) {
    return *reinterpret_cast<T *>(static_cast<std::byte *>(owner) + offset);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

inline FightDialogueRecord *DialogueBank(FightRuntimePrefix *game) {
    return dialogueBanks[DialogueValue<std::int8_t>(game, 0x22c)];
}

} // namespace

#else

namespace {

FightDialogueRecord *DialogueBank(const VFMA_t &game) {
    return dialogueBanks[static_cast<std::int8_t>(game.reserved318[0])];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfUpdateVFHDState(FightActorRuntimePrefix *actor) {
    VFHD_t *owner;
    if (actor->groupIndex != actor->game->playerSlot || !(owner = static_cast<VFHD_t *>(actor->game->dialogue)))
        return;
    switch (owner->state) {
    case 0:
        vfApplyStoredMoveInstructionConfiguration(owner);
        owner->instructionStates[0].program = owner->programIndices[0];
        owner->instructionStates[1].program = owner->programIndices[1];
        owner->partnerMode = instructionPrograms[owner->instructionStates[0].program].partnerMode;
        owner->programMode = instructionPrograms[owner->instructionStates[0].program].mode;
        owner->currentMotion = 0;
        owner->secondCurrentMotion = 0;
        InitializeInstructionHistory(owner);
        InitializeInstructionState(owner->instructionStates[0]);
        InitializeInstructionState(owner->instructionStates[1]);
        owner->state = 1;
        InstructionGetLocalPosition(owner->character, &owner->startPosition);
        InstructionGetLocalRotation(owner->character, &owner->startRotation);
        owner->startYaw = owner->startRotation.y;
        InstructionSetExtraFlag(owner->character, -1);
        for (std::int32_t i = 1; i < 8; ++i) {
            if (owner->partnerMode)
                actor->game->slotPoolModes[i] = 1;
            else
                actor->game->slotPoolModes[i] = 6;
        }
        break;
    case 1:
        instructionProgramResult = UpdateInstructionPrograms(actor, owner->instructionStates[0], owner->instructionStates[1]);
        UpdateInstructionPose(actor, owner->instructionStates[0], owner->instructionStates[1]);
        if (instructionProgramResult != -1) {
            actor->game->slotPoolModes[0] = 6;
            owner->state = 2;
        }
        if ((owner->instructionStates[0].outcome == 1 || owner->instructionStates[1].outcome == 1) && owner->committed == 1) {
            actor->game->slotPoolModes[0] = 6;
            if (owner->instructionStates[0].endFrame > 0 &&
                (static_cast<std::uint32_t>(owner->instructionStates[0].motion) & 0xf7ffffff) == InstructionAnimation(owner->character) &&
                InstructionMotionFrame(owner->character) > owner->instructionStates[0].endFrame)
                owner->state = 3;
        }
        CountInstructionCollisionInput(actor);
        break;
    case 2:
        if ((owner->instructionStates[0].outcome == 1 || owner->instructionStates[1].outcome == 1) && owner->committed == 1 &&
            owner->instructionStates[0].endFrame > 0 &&
            (static_cast<std::uint32_t>(owner->instructionStates[0].motion) & 0xf7ffffff) == InstructionAnimation(owner->character) &&
            InstructionMotionFrame(owner->character) > owner->instructionStates[0].endFrame) {
            EV_figpAssignFlagsFor(owner->character, 0, 0);
            InstructionAssignFigureMode(owner->character, 0);
            owner->state = 3;
        } else {
            const std::int32_t pose = UpdateInstructionPose(actor, owner->instructionStates[0], owner->instructionStates[1]);
            instructionPoseResult = pose;
            if ((owner->instructionStates[0].outcome == 2 || owner->instructionStates[0].endFrame == 0) &&
                (actor->animationTime >= float(*reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x294)) -
                            *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(actor) + 0x26c) ||
                    InstructionAnimation(owner->character) == 306)) {
                EV_figpAssignFlagsFor(owner->character, 0, 0);
                InstructionAssignFigureMode(owner->character, 0);
                owner->state = 3;
            }
            if (owner->inputCount > MaximumInstructionInputs(owner->instructionStates[0], owner->instructionStates[1]) + 1) {
                actor->game->slotPoolModes[0] = 6;
                if (InstructionAnimation(owner->character) == 306)
                    owner->state = 3;
            }
        }
        break;
    case 3:
        owner->result[0] = std::byte(2);
        owner->result[3] = static_cast<std::byte>(owner->instructionStates[0].endInstruction);
        if ((owner->instructionStates[0].outcome == 1 || owner->instructionStates[1].outcome == 1) && owner->committed == 1) {
            owner->result[1] = std::byte(1);
            if (owner->instructionStates[1].outcome == 1) {
                if (owner->instructionStates[1].analysis == 22)
                    owner->result[2] = std::byte(22);
                else if (owner->instructionStates[1].analysis == 23) {
                    owner->result[2] = std::byte(23);
                    owner->result[6] = static_cast<std::byte>(owner->instructionStates[1].committedAttempts);
                }
                *reinterpret_cast<std::int32_t *>(owner->result + 8) = owner->instructionStates[1].chosenDirection;
            } else
                *reinterpret_cast<std::int32_t *>(owner->result + 8) = owner->instructionStates[0].chosenDirection;
        } else {
            owner->result[1] = std::byte(2);
            if (owner->instructionStates[0].analysis) {
                owner->result[2] = static_cast<std::byte>(owner->instructionStates[0].analysis);
                *reinterpret_cast<std::uint32_t *>(owner->result + 8) = owner->instructionStates[0].diagnostic;
                owner->result[5] = static_cast<std::byte>(owner->instructionStates[0].delay);
                owner->result[6] = static_cast<std::byte>(owner->instructionStates[0].committedAttempts);
            } else
                owner->result[2] = static_cast<std::byte>(owner->instructionStates[0].failure);
        }
        for (std::int32_t i = 0; i < 32; ++i) {
        }
        if (!((owner->instructionStates[0].outcome == 1 || owner->instructionStates[1].outcome == 1) && owner->committed == 1))
            InstructionAnalysisNullsub(owner, owner->instructionStates[0]);
        instructionCompletionDelay = 1;
        owner->state = 4;
        UpdateInstructionPose(actor, owner->instructionStates[0], owner->instructionStates[1]);
        break;
    case 4:
        UpdateInstructionPose(actor, owner->instructionStates[0], owner->instructionStates[1]);
        if (--instructionCompletionDelay == 0)
            owner->state = 5;
        break;
    case 5:
        owner->state = 6;
        vfFreeVFHDSpace(actor->game);
    case 6:
        break;
    }
    if (actor->game->dialogue) {
        TickInstructionDialogue(owner);
        ++owner->frame;
    }
}

#else

// The player FGTM owns the full instruction/preview/result lifecycle.
// @verified - p
void vfUpdateVFHDState(VFActor *actor) {
    auto *const game = actor->game;
    if (actor->groupIndex != static_cast<std::uint8_t>(game->playerSlot) || !game->dialogue)
        return;
    auto *const owner = game->dialogue;
    auto &first = FirstInstruction(owner);
    auto &second = SecondInstruction(owner);
    const auto setState = [&](std::uint8_t state) { InstructionStore(owner, 0x440, state); };
    switch (InstructionValue<std::uint8_t>(owner, 0x440)) {
    case 0: {
        vfApplyMoveInstructionConfiguration(owner);
        InstructionStore(owner, 8, static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int16_t>(owner->programIndices[0]))));
        InstructionStore(owner, 0x40, static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int16_t>(owner->programIndices[1]))));
        const auto &program = instructionPrograms[first.program];
        InstructionStore(owner, 0x443, program.partnerMode);
        InstructionStore(owner, 0x442, program.mode);
        owner->currentMotion = owner->secondCurrentMotion = 0;
        InitializeInstructionHistory(owner);
        InitializeInstructionState(first);
        InitializeInstructionState(second);
        setState(1);
        InstructionStore(owner, 0x4d0, GetCharacterWorldPosition(owner->character));
        const auto rotation = GetCharacterWorldRotation(owner->character);
        InstructionStore(owner, 0x4e8, rotation);
        InstructionStore(owner, 0x4b8, rotation.y);
        InstructionStore(owner, 0x500, GetCharacterWorldPosition(actor->character));
        InstructionStore(owner, 0x50c, GetCharacterWorldPosition(actor->actor->character));
        mtwkSetExtraFlag(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(owner->character), 6, 0);
        for (std::size_t i = 1; i != 8; ++i)
            game->slotPoolModes[i] = program.partnerMode ? 1 : 6;
        break;
    }
    case 1:
        instructionProgramResult = UpdateInstructionPrograms(actor);
        UpdateInstructionPose(actor);
        if (instructionProgramResult != -1) {
            game->slotPoolModes[0] = 6;
            setState(2);
        }
        if (SuccessfulInstruction(owner, first, second)) {
            game->slotPoolModes[0] = 6;
            if (InstructionPreviewPastEnd(owner, first, second))
                setState(3);
        }
        if (actor->followupFlags)
            InstructionStore(owner, 0x4cc, static_cast<std::uint8_t>(InstructionValue<std::uint8_t>(owner, 0x4cc) + 1));
        break;
    case 2:
        if (InstructionPreviewPastEnd(owner, first, second)) {
            ResetInstructionPreview(owner);
            setState(3);
        } else {
            instructionPoseResult = UpdateInstructionPose(actor);
            if ((first.outcome == 2 || first.endFrame == 0) &&
                (actor->animationTime >= float(actor->reactionEndFrame) - actor->scale || InstructionAnimation(owner->character) == 306)) {
                ResetInstructionPreview(owner);
                setState(3);
            }
            const auto maximumInputs = std::max(InstructionRequiredInputs(first), InstructionRequiredInputs(second));
            if (InstructionValue<std::int8_t>(owner, 0x4cc) > std::int32_t(maximumInputs) + 1) {
                game->slotPoolModes[0] = 6;
                if (InstructionAnimation(owner->character) == 306)
                    setState(3);
            }
        }
        break;
    case 3:
        PublishInstructionResult(owner, first, second);
        if (!SuccessfulInstruction(owner, first, second))
            vfFightNullsub();
        instructionCompletionDelay = 1;
        setState(4);
        UpdateInstructionPose(actor);
        break;
    case 4:
        UpdateInstructionPose(actor);
        if (--instructionCompletionDelay == 0)
            setState(5);
        break;
    case 5:
        setState(6);
        vfFreeVFHDSpace(game);
        break;
    default:
        break;
    }
    if (game->dialogue) {
        auto delay = InstructionValue<std::uint16_t>(owner, 0x518);
        if (delay) {
            InstructionStore(owner, 0x518, --delay);
            if (!delay)
                EV_PlayDialogue(instructionVoices[InstructionValue<std::int16_t>(owner, 0x51a)].voice);
        }
        ++owner->frame;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int32_t SetTrainingFightReaction(std::uint16_t reaction) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    if (!game->dialogue)
        game->dialogue = AllocateVFHD(game);
    VFHD_t *const owner = game->dialogue;
    InstructionStore(owner, 0x440, std::uint8_t{0});
    game->slotPoolModes[0] = 0;
    const shen_name character = InstructionValue<shen_name>(owner, 0x474);
    if (vfmDispatchControl(FightControlOp::ReactionRank, character, 0, -1) == 2)
        vfmDispatchControl(FightControlOp::ReactionRank, character, 1, 1);
    if (reaction) {
        VFActor *const actor = game->actors[game->playerSlot];

#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(owner->character);
        InstructionStore(owner, 0x4c4, static_cast<std::uint32_t>(reaction));
        InstructionStore(owner, 0x4bc, static_cast<std::uint32_t>(GetAllAngYForNode(&owner->character->entity.node)));
        actor->parameter370 = reaction;
        actor->reactionDepth = 0;
    }
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

bool InstructionFacesCamera(const VFHD_t *owner) {
    Vector3 position, camera, direction, perpendicular;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, owner->firstName);
    GetCharacterWorldPosition(character, &position);
    direction.x = am2NegativeSineFixed(owner->startYaw);
    direction.y = 0.0f;
    direction.z = am2NegativeCosineFixed(owner->startYaw);
    perpendicular.x = direction.z;
    perpendicular.z = -direction.x;
    perpendicular.y = 0.0f;
    camCtrlGetNextCamPos(&camera);
    direction.x = camera.x - position.x;
    direction.y = 0.0f;
    direction.z = camera.z - position.z;
    const float dot = am2Vector3Dot(&direction, &perpendicular);
    SetDebugScreenTextPosition(3, 4);
    if (dot < 0.0f)
        return true;
    return false;
}

} // namespace

#else

namespace {

// Left/right interpretation follows the saved authored yaw and CAMC.
// @verified - p
bool InstructionFacesCamera(const VFHD_t *owner) {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    auto *const character = scenFindChara(control->scenarioTask, InstructionValue<shen_name>(owner, 0x474));
    const auto position = GetCharacterWorldPosition(character);
    const auto yaw = InstructionValue<std::int32_t>(owner, 0x4b8);
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    const float dot = (camera.x - position.x) * cos_fixed16(yaw) + (camera.z - position.z) * (-sin_fixed16(yaw));
    SetDebugStringPosition(3, 4);
    return dot < 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void QueueInstructionDialogue(VFHD_t *owner, std::int32_t index) {
    if (index == 2) {
        switch (static_cast<std::int32_t>(dc::sdk::RandomFloat() * 3.0f)) {
        case 0:
            index = 2;
            break;
        case 1:
            index = 3;
            break;
        case 2:
            index = 4;
            break;
        }
    }
    reinterpret_cast<std::uint16_t *>(owner->retained4d8 + 12)[0] = instructionVoices[index].delay;
    reinterpret_cast<std::uint16_t *>(owner->retained4d8 + 12)[1] = index;
}

} // namespace

#else

namespace {

void QueueInstructionDialogue(VFHD_t *owner, std::int32_t index) {
    if (index == 2)
        index = 2 + static_cast<std::int32_t>(double(IncrementSharedPrngFloat()) * 3.0);
    InstructionStore(owner, 0x51a, static_cast<std::uint16_t>(index));
    InstructionStore(owner, 0x518, instructionVoices[index].delay);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void TickInstructionDialogue(VFHD_t *owner) {
    std::byte *const storage = reinterpret_cast<std::byte *>(owner);
    if (*reinterpret_cast<std::int16_t *>(storage + 0x4e4) && !--*reinterpret_cast<std::int16_t *>(storage + 0x4e4))
        EV_PlayDialogue(instructionVoices[*reinterpret_cast<const std::int16_t *>(storage + 0x4e6)].voice);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

bool InstructionPartnerReady(shen_name) {
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t InstructionPartnerWithinRange(const VFHD_t *owner) {
    Vector3 second, first;
    InstructionGetLocalPosition(owner->character, &first);
    InstructionGetLocalPosition(owner->secondCharacter, &second);
    if (GetSquaredDistanceXZ(&second, &first) > 2.5600002f)
        return 0;
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t InstructionHitWindowAdmitted(const VFHD_t *owner, shen_name name) {
    std::int32_t result = 0;
    if (name == owner->firstName) {
        if (owner->secondPhase == 1 && owner->programMode == 3 && owner->firstHit)
            return true;
        if (owner->secondHit)
            return result;
        if (owner->partnerMode >= 2 && owner->programMode == 3) {
            if (owner->secondPhase != 1)
                return result;
            if (owner->secondTimer > owner->secondHitStart)
                return result;
        }
    } else if (owner->firstHit)
        return result;
    if (InstructionPartnerReady(name))
        result = 1;
    return result;
}

} // namespace

#else

namespace {

bool InstructionHitWindowAdmitted(const VFHD_t *owner, shen_name name) {
    if (name.name == InstructionValue<shen_name>(owner, 0x474).name) {
        const auto phase = InstructionValue<std::uint8_t>(owner, 0x445);
        if ((phase != 1 || InstructionValue<std::uint8_t>(owner, 0x442) != 3 || !InstructionValue<std::uint8_t>(owner, 0x447)) &&
            (InstructionValue<std::uint8_t>(owner, 0x448) ||
                (InstructionValue<std::int8_t>(owner, 0x443) >= 2 && InstructionValue<std::uint8_t>(owner, 0x442) == 3 &&
                    (phase != 1 || InstructionValue<std::int16_t>(owner, 0x46a) > InstructionValue<std::int16_t>(owner, 0x45a)))))
            return false;
    } else if (InstructionValue<std::uint8_t>(owner, 0x447))
        return false;
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t SeparateInstructionCharacters(Character *first, Character *second) {
    Vector3 secondPosition, firstPosition, offset;
    InstructionGetLocalPosition(first, &firstPosition);
    InstructionGetLocalPosition(second, &secondPosition);
    const float distance = GetSquaredDistanceXZ(&secondPosition, &firstPosition);
    if (0.9f > distance) {
        const std::uint16_t angle =
            static_cast<std::uint16_t>(dc::sdk::ArcTan2Fixed(secondPosition.x - firstPosition.x, secondPosition.z - firstPosition.z) + 0x8000);
        offset.x = am2NegativeSineFixed(angle) * (0.9f - distance);
        offset.z = am2NegativeCosineFixed(angle) * (0.9f - distance);
        secondPosition.x += offset.x;
        secondPosition.z += offset.z;
        InstructionSetLocalPosition(second, &secondPosition);
        return 1;
    }
    return 0;
}

std::int32_t PlaceInstructionCharacterPair(Character *first, Character *second) {
    Vector3 points[4], target, origin, offset, position;
    float distances[4];
    const std::int32_t bones[4] = {21, 14, 31, 26};
    InstructionBonePosition(first, bones[0], &points[0]);
    InstructionBonePosition(first, bones[1], &points[1]);
    InstructionBonePosition(first, bones[2], &points[2]);
    InstructionBonePosition(first, bones[3], &points[3]);
    InstructionBonePosition(second, 3, &target);
    InstructionBonePosition(first, 3, &origin);
    distances[0] = GetSquaredDistanceXZ(&target, &points[0]);
    distances[1] = GetSquaredDistanceXZ(&target, &points[1]);
    distances[2] = GetSquaredDistanceXZ(&target, &points[2]);
    distances[3] = GetSquaredDistanceXZ(&target, &points[3]);
    InstructionGetLocalPosition(second, &position);
    std::int32_t nearest = -1;
    if (distances[0] < distances[1] && distances[0] < distances[2] && distances[0] < distances[3])
        nearest = 0;
    if (nearest == -1 && distances[1] < distances[2] && distances[1] < distances[3])
        nearest = 1;
    if (nearest == -1)
        nearest = distances[2] < distances[3] ? 2 : 3;
    if (!(0.3f > distances[nearest]))
        return 0;
    const std::uint16_t angle = static_cast<std::uint16_t>(dc::sdk::ArcTan2Fixed(target.x - origin.x, target.z - origin.z) + 0x8000);
    offset.x = am2NegativeSineFixed(angle) * (0.3f - distances[nearest]);
    offset.z = am2NegativeCosineFixed(angle) * (0.3f - distances[nearest]);
    position.x += offset.x;
    position.z += offset.z;
    InstructionSetLocalPosition(second, &position);
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t DecodeInstructionInput(const VFHD_t *owner, std::int16_t input) {
    std::int32_t result = 0;
    if (input & 2)
        result |= 2;
    if (input & 4)
        result |= 8;
    if (input & 1)
        result |= 1;
    if (input & 8)
        result |= 4;
    if (!InstructionFacesCamera(owner)) {
        if (input & 0x40)
            result |= 0x80;
        if (input & 0x80)
            result |= 0x40;
    } else {
        if (input & 0x40)
            result |= 0x40;
        if (input & 0x80)
            result |= 0x80;
    }
    if (input & 0x20)
        result |= 0x20;
    if (input & 0x10)
        result |= 0x10;
    return result;
}

} // namespace

#else

namespace {

std::int16_t DecodeInstructionInput(const VFHD_t *owner, std::uint8_t input) {
    std::uint16_t result = (input & 2) ? 2 : 0;
    if (input & 4)
        result |= 8;
    if (input & 1)
        result |= 1;
    if (input & 8)
        result |= 4;
    if (InstructionFacesCamera(owner)) {
        if (input & 0x40)
            result |= 0x40;
        if (input & 0x80)
            result |= 0x80;
    } else {
        if (input & 0x40)
            result |= 0x80;
        if (input & 0x80)
            result |= 0x40;
    }
    if (input & 0x20)
        result |= 0x20;
    if (input & 0x10)
        result |= 0x10;
    return static_cast<std::int16_t>(result);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void CountInstructionCollisionInput(FightActorRuntimePrefix *actor) {
    VFHD_t *owner = static_cast<VFHD_t *>(actor->game->dialogue);
    if (owner && *reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(actor) + 0x494))
        ++reinterpret_cast<std::uint8_t *>(owner)[0x4b0];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void RecordInstructionInput(FightActorRuntimePrefix *actor) {
    VFHD_t *const owner = static_cast<VFHD_t *>(actor->game->dialogue);
    if (!owner)
        return;
    const std::int32_t pressed = DecodeInstructionInput(owner, *reinterpret_cast<const std::int16_t *>(actor->retained492 + 2));
    const std::int32_t held = DecodeInstructionInput(owner, static_cast<std::int16_t>(actor->inputFlags));
    owner->history[owner->historyIndex].held = held;
    owner->history[owner->historyIndex].pressed = pressed;
    ++owner->historyIndex;
    if (owner->historyIndex == 120)
        owner->historyIndex = 0;
}

} // namespace

#else

namespace {

void RecordInstructionInput(VFActor *actor) {
    auto *const owner = actor->game->dialogue;
    if (!owner)
        return;
    const auto pressed = DecodeInstructionInput(owner, static_cast<std::uint8_t>(actor->followupFlags));
    const auto held = DecodeInstructionInput(owner, static_cast<std::uint8_t>(actor->inputFlags));
    auto index = InstructionValue<std::int8_t>(owner, 0x438);
    InstructionStore(owner, 0x78 + 8 * index, held);
    InstructionStore(owner, 0x7a + 8 * index, pressed);
    ++index;
    InstructionStore(owner, 0x438, index == 120 ? std::int8_t{0} : index);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int16_t LastInstructionDirection(const VFHD_t *owner) {
    const std::int8_t end = *reinterpret_cast<const std::int8_t *>(reinterpret_cast<const std::byte *>(owner) + 0x434);
    std::int32_t index = end ? end - 1 : 0;
    std::int16_t result;
    do {
        result = reinterpret_cast<const InstructionHistory *>(reinterpret_cast<const std::byte *>(owner) + 0x74 + 8 * index)->pressed & 0xf0;
        if (result)
            break;
    } while (--index != 0);
    return result;
}

} // namespace

#else

namespace {

std::int16_t LastInstructionDirection(const VFHD_t *owner) {
    const auto end = InstructionValue<std::int8_t>(owner, 0x438);
    std::int64_t index = end ? end - 1 : 0;
    for (;;) {
        const auto result = InstructionHistoryAt(owner, index).pressed & 0xf0;
        if (result)
            return result;
        --index;
        if (index == 0)
            return result;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::uint8_t DirectionSector(std::int32_t direction) {
    switch (static_cast<std::uint8_t>(direction) & 0xf0) {
    case 0x20:
        return 0;
    case 0x60:
        return 32;
    case 0x40:
        return 64;
    case 0x50:
        return 96;
    case 0x10:
        return 128;
    case 0x90:
        return 160;
    case 0x80:
        return 192;
    case 0xa0:
        return 224;
    default:
        return 0;
    }
}

} // namespace

#else

namespace {

std::uint8_t DirectionSector(std::uint8_t direction) {
    switch (direction & 0xf0) {
    case 0x10:
        return 128;
    case 0x40:
        return 64;
    case 0x50:
        return 96;
    case 0x60:
        return 32;
    case 0x80:
        return 192;
    case 0x90:
        return 160;
    case 0xa0:
        return 224;
    default:
        return 0;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t InstructionDirectionChanged(std::int16_t first, std::int16_t second) {
    if (!(first & 0xf0) || !(second & 0xf0))
        return 0;
    const std::uint8_t a = DirectionSector(first), b = DirectionSector(second);
    const std::uint8_t difference = a > b ? a - b : b - a;
    if (difference > 63U)
        return 1;
    return 0;
}

} // namespace

#else

namespace {

bool InstructionDirectionChanged(std::uint8_t first, std::uint8_t second) {
    if (!(first & 0xf0) || !(second & 0xf0))
        return false;
    const auto a = DirectionSector(first), b = DirectionSector(second);
    return static_cast<std::uint8_t>(a > b ? a - b : b - a) >= 64;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::uint8_t InstructionCloseBetweenCharacters(const VFHD_t *owner, const InstructionState &state) {
    if (owner->partnerMode >= 2 && !state.attempts && owner->programMode == 3) {
        if (owner->secondPhase != 1)
            return 1;
        if (!InstructionPartnerReady(owner->firstName))
            return 2;
        if (owner->secondTimer > owner->secondHitStart)
            return 3;
    } else if (owner->partnerMode == 1 && !state.attempts) {
        if (!InstructionPartnerWithinRange(owner))
            return 2;
    }
    return 0;
}

} // namespace

#else

namespace {

std::uint8_t InstructionCloseBetweenCharacters(const VFHD_t *owner, const InstructionState &state) {
    const auto mode = InstructionValue<std::int8_t>(owner, 0x443);
    if (mode >= 2 && !state.attempts && InstructionValue<std::uint8_t>(owner, 0x442) == 3) {
        if (InstructionValue<std::uint8_t>(owner, 0x445) != 1)
            return 1;
        if (InstructionValue<std::int16_t>(owner, 0x46a) > InstructionValue<std::int16_t>(owner, 0x45a))
            return 3;
    } else if (mode == 1 && !state.attempts) {
        const auto a = GetCharacterWorldPosition(owner->character), b = GetCharacterWorldPosition(owner->secondCharacter);
        if (double((b.x - a.x) * (b.x - a.x) + (b.z - a.z) * (b.z - a.z)) > 2.56)
            return 2;
    }
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void QueueInstructionMotion(VFHD_t *owner, std::int32_t, std::int32_t motion) {
    std::int32_t zero = 0;
    owner->requestedMotion = motion;
    std::byte *phase = reinterpret_cast<std::byte *>(owner) + offsetof(VFHD_t, firstPhase);
    const std::size_t motionOffset = offsetof(VFHD_t, firstMotion) - offsetof(VFHD_t, firstPhase);
    *reinterpret_cast<std::uint32_t *>(phase + motionOffset) = motion;
    const std::byte *const metadata = MOTN_FindAnimationExtraRecord(*reinterpret_cast<const std::uint16_t *>(phase + motionOffset), 2);
    if (owner->partnerMode >= 2 && owner->programMode == 3) {
        owner->firstHitStart = zero;
        owner->firstHitEnd = 1000;
        owner->firstTimer = zero;
        return;
    }
    if (metadata) {
        owner->firstHitStart = *reinterpret_cast<const std::int16_t *>(metadata + 2);
        owner->firstHitEnd = *reinterpret_cast<const std::int16_t *>(metadata + 4);
        owner->firstTimer = 0;
    } else {
        owner->firstHitStart = 0;
        owner->firstHitEnd = 0;
        owner->firstTimer = 0;
    }
}

} // namespace

#else

namespace {

void QueueInstructionMotion(VFHD_t *owner, std::int32_t motion) {
    owner->requestedMotion = motion;
    InstructionStore(owner, 0x460, motion);
    const auto metadata = FindLoadedMotionSecondaryEntry(static_cast<std::uint16_t>(motion), 2);
    if (InstructionValue<std::int8_t>(owner, 0x443) >= 2 && InstructionValue<std::uint8_t>(owner, 0x442) == 3)
        InstructionStore(owner, 0x44a, std::uint32_t{65536000});
    else if (!metadata.empty()) {
        InstructionStore(owner, 0x44a, InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(metadata.data()), 2));
        InstructionStore(owner, 0x44c, InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(metadata.data()), 4));
    } else
        InstructionStore(owner, 0x44a, std::uint32_t{0});
    InstructionStore(owner, 0x468, std::uint16_t{0});
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t InstructionDiagnosisNullsub(VFHD_t *, std::int16_t, std::int16_t, InstructionState &, InstructionState &) {
    return 0;
}

std::int32_t InstructionUnreferencedValue() {
    return 84;
}

void ResetInstructionPairState(VFHD_t *owner, std::int16_t, std::int16_t, InstructionState &) {
    reinterpret_cast<std::uint8_t *>(owner)[0x43c] = 2;
    reinterpret_cast<std::uint8_t *>(owner)[0x43d] = 2;
}

std::int32_t PeekProgramInstruction(const InstructionState &state, std::int8_t instruction) {
    std::int32_t argument;
    for (;;) {
        ++instruction;
        const InstructionCommand *const command = &instructionPrograms[state.program].commands[instruction];
        if (command->operation == 7) {
            argument = command->argument;
            break;
        }
        if (command->operation == 6) {
            argument = 0;
            break;
        }
    }
    return argument;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::uint8_t InstructionRequiredInputs(const InstructionState &state) {
    std::int32_t count = 0, i = 0;
    if (state.program == -1)
        return 0;
    const InstructionCommand *command;
    const InstructionCommand *commands = instructionPrograms[state.program].commands;
    goto read;
next:
    ++i;
read:
    command = reinterpret_cast<const InstructionCommand *>(static_cast<std::uintptr_t>(i) * sizeof(*commands) + reinterpret_cast<std::uintptr_t>(commands));
    if (command->operation == 7) {
        ++count;
        goto next;
    }
    if (command->operation != 6)
        goto next;
    return count;
}

} // namespace

#else

namespace {

std::uint8_t InstructionRequiredInputs(const InstructionState &state) {
    if (state.program == -1)
        return 0;
    std::uint8_t count = 0;
    for (const auto *command = instructionPrograms[state.program].commands; command->operation != 6; ++command)
        if (command->operation == 7)
            ++count;
    return count;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

bool SuccessfulInstruction(const VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    return (first.outcome == 1 || second.outcome == 1) && InstructionValue<std::uint8_t>(owner, 0x43e) == 1;
}

} // namespace

#else

namespace {

bool SuccessfulInstruction(const VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    return (first.outcome == 1 || second.outcome == 1) && InstructionValue<std::uint8_t>(owner, 0x446) == 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

bool InstructionPreviewPastEnd(VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    return SuccessfulInstruction(owner, first, second) && first.endFrame > 0 &&
        (static_cast<std::uint32_t>(first.motion) & 0xf7ffffff) == InstructionAnimation(owner->character) &&
        mtwkGetFrame(charFindMotionwork(owner->character), static_cast<MotionRegion>(0)) > first.endFrame;
}

} // namespace

#else

namespace {

bool InstructionPreviewPastEnd(VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    return SuccessfulInstruction(owner, first, second) && first.endFrame > 0 &&
        (static_cast<std::uint32_t>(first.motion) & 0xf7ffffff) == InstructionAnimation(owner->character) &&
        mtwkGetFrame(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(owner->character), MotionRegion::LowerBody) > first.endFrame;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void ResetInstructionPreview(VFHD_t *owner) {
    EV_figpAssignFlagsFor(owner->character, 0, 0);
    figpAssignModeForCharacter(owner->character, 0);
}

} // namespace

#else

namespace {

void ResetInstructionPreview(VFHD_t *owner) {
    EV_figpAssignFlagsFor(owner->character, 0, 0);
    figpAssignModeForCharacter(owner->character, 0);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void PublishInstructionResult(VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    owner->result[0] = std::byte(2);
    owner->result[3] = static_cast<std::byte>(first.endInstruction);
    if (SuccessfulInstruction(owner, first, second)) {
        owner->result[1] = std::byte(1);
        if (second.outcome == 1) {
            if (second.analysis == 22)
                owner->result[2] = std::byte(22);
            else if (second.analysis == 23) {
                owner->result[2] = std::byte(23);
                owner->result[6] = static_cast<std::byte>(second.committedAttempts);
            }
            __typeof__(static_cast<std::int32_t>(InstructionValue<std::int16_t>(owner, 0x66))) detail =
                static_cast<std::int32_t>(InstructionValue<std::int16_t>(owner, 0x66));
            std::memcpy(owner->result + 8, &detail, 4);
        } else {
            __typeof__(static_cast<std::int32_t>(InstructionValue<std::int16_t>(owner, 0x2e))) detail =
                static_cast<std::int32_t>(InstructionValue<std::int16_t>(owner, 0x2e));
            std::memcpy(owner->result + 8, &detail, 4);
        }
    } else {
        owner->result[1] = std::byte(2);
        if (first.analysis) {
            owner->result[2] = static_cast<std::byte>(first.analysis);
            std::memcpy(owner->result + 8, &first.diagnostic, 4);
            owner->result[5] = static_cast<std::byte>(first.delay);
            owner->result[6] = static_cast<std::byte>(first.committedAttempts);
        } else
            owner->result[2] = static_cast<std::byte>(first.failure);
    }
}

} // namespace

#else

namespace {

void PublishInstructionResult(VFHD_t *owner, const InstructionState &first, const InstructionState &second) {
    owner->result[0] = std::byte{2};
    owner->result[3] = static_cast<std::byte>(first.endInstruction);
    if (SuccessfulInstruction(owner, first, second)) {
        owner->result[1] = std::byte{1};
        if (second.outcome == 1) {
            if (second.analysis == 22)
                owner->result[2] = std::byte{22};
            else if (second.analysis == 23) {
                owner->result[2] = std::byte{23};
                owner->result[6] = static_cast<std::byte>(second.committedAttempts);
            }
            const auto detail = InstructionValue<std::uint32_t>(owner, 0x6a);
            std::memcpy(owner->result + 8, &detail, 4);
        } else {
            const auto detail = InstructionValue<std::uint32_t>(owner, 0x32);
            std::memcpy(owner->result + 8, &detail, 4);
        }
    } else {
        owner->result[1] = std::byte{2};
        if (first.analysis) {
            owner->result[2] = static_cast<std::byte>(first.analysis);
            std::memcpy(owner->result + 8, &first.diagnostic, 4);
            owner->result[5] = static_cast<std::byte>(first.delay);
            owner->result[6] = static_cast<std::byte>(first.committedAttempts);
        } else
            owner->result[2] = static_cast<std::byte>(first.failure);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::uint8_t MaximumInstructionInputs(const InstructionState &first, const InstructionState &second) {
    std::uint8_t inputs[2];
    inputs[0] = InstructionRequiredInputs(first);
    inputs[1] = InstructionRequiredInputs(second);
    if (inputs[0] > inputs[1])
        return inputs[0];
    return inputs[1];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t RunInstructionProgram(FightActorRuntimePrefix *actor, VFHD_t *owner, InstructionState &state, std::int32_t) {
    std::int8_t instruction = state.instruction;
    std::int32_t history = state.historyIndex;
    std::int32_t originalHistory = state.historyIndex;
    std::int32_t done = 0;
    if (!history) {
        state.resolvedMask = 0;
        state.previousMask = state.pendingMask = 0;
        state.heldDirection = 0;
    }
    do {
        const InstructionCommand *command = &instructionPrograms[state.program].commands[instruction];
        switch (command->operation) {
        case 6:
            state.ended = 1;
            if (state.progress == 3) {
                state.progress = 1;
                state.analysis = 0;
            }
            state.done = 1;
            done = 1;
            break;
        case 10:
            if (state.outcome != 2)
                QueueInstructionDialogue(owner, command->argument);
            ++instruction;
            state.instruction = instruction;
            state.historyIndex = history;
            break;
        case 11:
            state.endFrame = command->argument;
            ++instruction;
            state.instruction = instruction;
            state.historyIndex = history;
            break;
        case 9:
            state.skippedFrames = command->argument;
            ++instruction;
            state.instruction = instruction;
            state.historyIndex = history;
            break;
        case 8:
            if (static_cast<std::int8_t>(state.maximumWindow) == 120)
                state.maximumWindow = command->argument;
            else
                state.maximumWindow = command->argument + history - 1;
            state.frameCountdown = command->argument + 1;
            ++instruction;
            state.instruction = instruction;
            state.historyIndex = history;
            break;
        case 5:
            if (state.progress == 3 || state.progress == 1 || state.progress == 0) {
                state.motion = command->argument;
                if (owner->firstPhase != 2) {
                    switch (InstructionCloseBetweenCharacters(owner, state)) {
                    case 0: {
                        QueueInstructionMotion(owner, 0, state.motion);
                        owner->firstHitReaction = 1;
                        owner->firstHit = 1;
                        std::int32_t pending = PeekProgramInstruction(state, instruction) & state.pendingMask;
                        if (pending) {
                            state.failure = 14;
                            state.outcome = 2;
                            state.diagnostic = pending;
                            owner->instructionPhase = 3;
                        }
                        break;
                    }
                    case 1:
                        owner->requestedMotion = 394;
                        state.failure = 18;
                        state.outcome = 2;
                        owner->instructionPhase = 3;
                        break;
                    case 2:
                        owner->requestedMotion = 394;
                        state.outcome = 2;
                        state.failure = 21;
                        owner->instructionPhase = 3;
                        break;
                    case 3:
                        owner->requestedMotion = 394;
                        if (state.resolvedMask)
                            ResetInstructionPairState(owner, state.resolvedMask, state.resolvedMask, state);
                        else
                            ResetInstructionPairState(owner, owner->history[0].held, owner->history[0].pressed, state);
                        state.outcome = 2;
                        state.failure = 19;
                        owner->instructionPhase = 3;
                        break;
                    }
                } else {
                    state.outcome = 2;
                    state.failure = 19;
                    owner->instructionPhase = 3;
                }
                ++state.attempts;
            }
            ++instruction;
            state.instruction = instruction;
            state.historyIndex = history;
            break;
        case 16:
            owner->firstEvalTime = command->argument;
            ++instruction;
            state.instruction = instruction;
            break;
        case 17:
            owner->firstEndFrame = command->argument;
            if (owner->firstEndFrame < .1f)
                owner->firstEndFrame = .001f;
            ++instruction;
            state.instruction = instruction;
            break;
        case 18:
            owner->holdOnSuccess = command->argument;
            ++instruction;
            state.instruction = instruction;
            break;
        case 3:
        case 4:
            if ((state.progress == 3 || state.progress == 1) && !(state.pendingMask | state.previousMask)) {
                state.motion = command->argument;
                if (state.motion == -1) {
                    std::int16_t direction;
                    state.chosenDirection = direction = LastInstructionDirection(owner);
                    if (actor->actor->moveFlags & 0x40) {
                        if (direction & 0x20)
                            state.motion = 603;
                        else if (direction & 0x10)
                            state.motion = 602;
                        else if (direction & 0x80)
                            state.motion = 379;
                        else if (direction & 0x40)
                            state.motion = 601;
                    } else {
                        if (direction & 0x20)
                            state.motion = 602;
                        else if (direction & 0x10)
                            state.motion = 603;
                        else if (direction & 0x80)
                            state.motion = 379;
                        else if (direction & 0x40)
                            state.motion = 601;
                    }
                    state.motion |= 0x8000000;
                }
                ++instruction;
                state.instruction = instruction;
                state.historyIndex = history;
            } else {
                state.progress = 2;
                if (!state.analysis)
                    state.analysis = 13;
                state.done = 1;
                done = 1;
            }
            break;
        case 7:
            if (history > static_cast<std::int8_t>(state.maximumWindow)) {
                switch (state.progress) {
                case 0:
                    state.progress = 2;
                    state.analysis = 20;
                    break;
                case 1:
                case 2:
                    break;
                case 3:
                    state.progress = 2;
                    state.analysis = 0;
                    break;
                }
                done = 1;
            }
            if (history < owner->historyIndex) {
                state.endInstruction = instruction;
                std::int32_t requirement = command->argument;
                std::int32_t combined;
                if (!state.heldDirection || (owner->history[history].pressed & requirement & 0xf0)) {
                    combined = owner->history[history].pressed | state.pendingMask;
                    if (owner->history[history].pressed & 0xf0)
                        state.heldDirection = 1;
                } else if (InstructionDirectionChanged(owner->history[history].held, state.previousDirection))
                    combined = (owner->history[history].held & 0xf0) | (owner->history[history].pressed & 0xf) | state.pendingMask;
                else {
                    combined = (owner->history[history].pressed & 0xf) | state.pendingMask;
                    if (!(owner->history[history].held & 0xf0))
                        state.heldDirection = 0;
                }
                if (combined & 0xf0)
                    state.previousDirection = combined & 0xf0;
                state.previousMask = state.pendingMask;
                state.pendingMask = 0;
                if ((requirement & 0xf0) && (requirement & combined & 0xf0))
                    combined = (combined & 0xf) | (requirement & 0xf0);
                if (requirement & 0x100) {
                    std::int32_t direction = combined & 0xf0;
                    if (direction & requirement)
                        requirement = (requirement & 0xf) | direction;
                }
                if (combined == requirement) {
                    state.progress = 3;
                    state.previousMask = 0;
                    state.pendingMask = 0;
                    ++instruction;
                } else if (state.previousMask) {
                    state.progress = 2;
                    state.done = 1;
                    done = 1;
                    state.resolvedMask = combined;
                } else if ((requirement & combined) == requirement) {
                    state.pendingMask = combined ^ requirement;
                    ++instruction;
                } else
                    state.pendingMask = combined;
                ++history;
            } else
                done = 1;
            break;
        }
    } while (!done);
    if (originalHistory == state.historyIndex) {
        state.pendingMask = 0;
        state.previousMask = 0;
    }
    return 0;
}

} // namespace

#else

namespace {

// Advance an authored program against the actual VFHD input history.
// @verified - p
void RunInstructionProgram(VFActor *actor, VFHD_t *owner, InstructionState &state) {
    const std::int8_t originalHistory = state.historyIndex;
    std::int8_t instruction = state.instruction;
    std::int32_t history = originalHistory;
    if (!originalHistory) {
        state.previousMask = state.resolvedMask = state.pendingMask = 0;
        state.heldDirection = 0;
    }
    const auto *const commands = instructionPrograms[state.program].commands;
    const auto advance = [&] {
        ++instruction;
        state.historyIndex = static_cast<std::int8_t>(history);
        state.instruction = instruction;
    };
    const auto finish = [&] {
        if (originalHistory == state.historyIndex) {
            state.pendingMask = state.previousMask = 0;
        }
    };
    for (;;) {
        const auto command = commands[instruction];
        switch (command.operation) {
        case 3:
        case 4:
            if (((state.progress - 1) & 0xfd) == 0 && !state.pendingMask && !state.previousMask) {
                state.motion = command.argument;
                if (state.motion == -1) {
                    state.chosenDirection = LastInstructionDirection(owner);
                    if (actor->actor->moveFlags & VFMF_MirrorReactionAngle) {
                        if (state.chosenDirection & 0x20)
                            state.motion = 603;
                        else if (state.chosenDirection & 0x10)
                            state.motion = 602;
                        else if (state.chosenDirection & 0x80)
                            state.motion = 379;
                        else if (state.chosenDirection & 0x40)
                            state.motion = 601;
                    } else {
                        if (state.chosenDirection & 0x20)
                            state.motion = 602;
                        else if (state.chosenDirection & 0x10)
                            state.motion = 603;
                        else if (state.chosenDirection & 0x80)
                            state.motion = 379;
                        else if (state.chosenDirection & 0x40)
                            state.motion = 601;
                    }
                    state.motion |= 0x8000000;
                }
                advance();
                break;
            }
            state.progress = 2;
            if (!state.analysis)
                state.analysis = 13;
            state.done = 1;
            finish();
            return;
        case 5:
            if (!(state.progress & 0xfc) && state.progress != 2) {
                state.motion = command.argument;
                if (InstructionValue<std::uint8_t>(owner, 0x444) == 2) {
                    state.outcome = 2;
                    state.failure = 19;
                    InstructionStore(owner, 0x4cd, std::uint8_t{3});
                } else {
                    const auto close = InstructionCloseBetweenCharacters(owner, state);
                    if (close) {
                        if (close == 1 || close == 2 || close == 3) {
                            owner->requestedMotion = 394;
                            state.failure = close == 1 ? 18 : close == 2 ? 21 : 19;
                            state.outcome = 2;
                            if (close == 3)
                                InstructionStore(owner, 0x444, std::uint16_t{514});
                            InstructionStore(owner, 0x4cd, std::uint8_t{3});
                        }
                    } else {
                        QueueInstructionMotion(owner, state.motion);
                        if (state.motion == 0x800019c)
                            owner->result[7] |= std::byte{1};
                        InstructionStore(owner, 0x470, std::uint16_t{1});
                        InstructionStore(owner, 0x447, std::uint8_t{1});
                        std::int8_t next = instruction + 1;
                        while (commands[next].operation != 7 && commands[next].operation != 6)
                            ++next;
                        const auto requirement = commands[next].operation == 7 ? commands[next].argument : 0;
                        const auto pending = requirement & state.pendingMask;
                        if (pending) {
                            state.failure = 14;
                            state.outcome = 2;
                            state.diagnostic = pending;
                            InstructionStore(owner, 0x4cd, std::uint8_t{3});
                        }
                    }
                }
                ++state.attempts;
            }
            advance();
            break;
        case 6:
            state.ended = 1;
            if (state.progress == 3) {
                state.progress = 1;
                state.analysis = 0;
            }
            state.done = 1;
            finish();
            return;
        case 7: {
            bool failed = false;
            if (history > static_cast<std::int8_t>(state.maximumWindow)) {
                if (!state.progress)
                    state.analysis = 20;
                else if (state.progress == 3)
                    state.analysis = 0;
                if (!state.progress || state.progress == 3)
                    state.progress = 2;
                failed = true;
            }
            if (history < InstructionValue<std::int8_t>(owner, 0x438)) {
                state.endInstruction = instruction;
                auto requirement = command.argument;
                const auto input = InstructionHistoryAt(owner, history);
                const auto pending = state.pendingMask;
                std::int32_t combined;
                if (!state.heldDirection || (std::uint8_t(requirement) & std::uint8_t(input.pressed) & 0xf0)) {
                    combined = pending | input.pressed;
                    if (input.pressed & 0xf0)
                        state.heldDirection = 1;
                } else {
                    combined = pending | (input.pressed & 0xf);
                    if (InstructionDirectionChanged(input.held, state.previousDirection))
                        combined |= input.held & 0xf0;
                    else if (!(input.held & 0xf0))
                        state.heldDirection = 0;
                }
                if (combined & 0xf0)
                    state.previousDirection = combined & 0xf0;
                state.previousMask = pending;
                state.pendingMask = 0;
                if ((requirement & 0xf0) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                    combined = (requirement & 0xf0) | (combined & 0xf);
                if ((requirement & 0x100) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                    requirement = (requirement & 0xf) | (combined & 0xf0);
                if (combined == requirement) {
                    ++instruction;
                    state.progress = 3;
                    state.pendingMask = state.previousMask = 0;
                } else if (pending) {
                    state.progress = 2;
                    failed = true;
                    state.done = 1;
                    state.resolvedMask = combined;
                } else {
                    if ((requirement & combined) == requirement) {
                        combined ^= requirement;
                        ++instruction;
                    }
                    state.pendingMask = combined;
                }
                ++history;
                if (!failed)
                    continue;
            }
            finish();
            return;
        }
        case 8:
            state.maximumWindow = static_cast<std::uint8_t>(state.maximumWindow == 120 ? command.argument : history + command.argument - 1);
            state.frameCountdown = static_cast<std::uint8_t>(command.argument + 1);
            advance();
            break;
        case 9:
            state.skippedFrames = static_cast<std::uint8_t>(command.argument);
            advance();
            break;
        case 10:
            if (state.outcome != 2)
                QueueInstructionDialogue(owner, command.argument);
            advance();
            break;
        case 11:
            state.endFrame = float(command.argument);
            advance();
            break;
        case 16:
            InstructionStore(owner, 0x450, float(command.argument));
            ++instruction;
            state.instruction = instruction;
            break;
        case 17: {
            const float endFrame = float(command.argument);
            InstructionStore(owner, 0x454, double(endFrame) < .1 ? .001f : endFrame);
            ++instruction;
            state.instruction = instruction;
            break;
        }
        case 18:
            InstructionStore(owner, 0x458, static_cast<std::uint8_t>(command.argument));
            ++instruction;
            state.instruction = instruction;
            break;
        default:
            break;
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::uint8_t InstructionMaskDiagnosis(std::int32_t mask, bool missing, bool directionRequired) {
    if (mask & 1)
        return missing ? 3 : 8;
    if (mask & 2)
        return missing ? 4 : 9;
    if (mask & 8)
        return missing ? 5 : 10;
    if (mask & 4)
        return missing ? 6 : 11;
    if (mask & 0xf0)
        return missing ? 7 : directionRequired ? 1 : 12;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t DiagnoseInstructionProgram(VFHD_t *owner, InstructionState &state, InstructionDiagnosis *result) {
    std::int32_t history, diagnostic, done, pending, progress, mask;
    std::int8_t window, delay;
    std::uint8_t finalAttempts;
    std::int32_t held;
    std::uint8_t attempts;
    std::int32_t lastMatched;
    std::int16_t previousRequirement, previousDirection;
    std::int32_t previous;
    std::int32_t kind;
    std::int8_t instruction;
    std::int32_t inputs;
    lastMatched = 0;
    previousRequirement = 0;
    pending = 0;
    previous = 0;
    diagnostic = 0;
    previousDirection = 0;
    progress = 0;
    kind = 0;
    delay = 0;
    finalAttempts = 0;
    mask = 0;
    window = 120;
    attempts = 0;
    instruction = 0;
    history = 0;
    held = 0;
    done = 0;
    do {
        const InstructionCommand *command = &instructionPrograms[state.program].commands[instruction];
        switch (command->operation) {
        case 6:
            if (lastMatched > window) {
                progress = 2;
                kind = 16;
                delay = lastMatched - window;
                finalAttempts = attempts;
            }
            done = 1;
            break;
        case 8:
            window = window == 120 ? command->argument : history + command->argument - 1;
            ++instruction;
            break;
        case 5:
            ++attempts;
            ++instruction;
            break;
        case 3:
        case 4:
            if ((pending | previous) && (!kind || kind == 13)) {
                inputs = previous;
                if (pending)
                    inputs = pending;
                if (inputs & 1)
                    kind = 8;
                else if (inputs & 2)
                    kind = 9;
                else if (inputs & 8)
                    kind = 10;
                else if (inputs & 4)
                    kind = 11;
                else if (inputs & 0xf0) {
                    kind = 12;
                    if (previousRequirement & 0xf0)
                        kind = 1;
                }
                done = 1;
                break;
            }
            ++instruction;
            break;
        case 9:
        case 10:
        case 11:
        case 16:
        case 17:
        case 18:
            ++instruction;
            break;
        case 7: {
            state.endInstruction = instruction;
            std::int32_t requirement = command->argument;
            if (history < owner->historyIndex) {
                std::int32_t combined;
                if (!held || (owner->history[history].pressed & requirement & 0xf0)) {
                    combined = owner->history[history].pressed | pending;
                    if (owner->history[history].pressed & 0xf0)
                        held = 1;
                } else if (InstructionDirectionChanged(owner->history[history].held, previousDirection))
                    combined = (owner->history[history].held & 0xf0) | (owner->history[history].pressed & 0xf) | pending;
                else {
                    combined = (owner->history[history].pressed & 0xf) | pending;
                    if (!(owner->history[history].held & 0xf0))
                        held = 0;
                }
                if (combined & 0xf0)
                    previousDirection = combined & 0xf0;
                previous = pending;
                pending = 0;
                if ((requirement & 0xf0) && (requirement & combined & 0xf0))
                    combined = (combined & 0xf) | (requirement & 0xf0);
                if (requirement & 0x100) {
                    std::int32_t direction = combined & 0xf0;
                    if (direction & requirement)
                        requirement = (requirement & 0xf) | direction;
                }
                if (combined == requirement) {
                    progress = 3;
                    pending = previous = 0;
                    if (history > window) {
                        progress = 2;
                        kind = 16;
                        delay = history - window;
                        finalAttempts = attempts;
                        done = 1;
                        break;
                    }
                    lastMatched = history;
                    previousRequirement = requirement;
                    ++instruction;
                } else if (previous) {
                    std::int32_t frames;
                    progress = 2;
                    done = 1;
                    if (!kind) {
                        if (combined & requirement) {
                            diagnostic = (combined ^ requirement) & combined;
                            if (diagnostic & 1)
                                kind = 8;
                            else if (diagnostic & 2)
                                kind = 9;
                            else if (diagnostic & 8)
                                kind = 10;
                            else if (diagnostic & 4)
                                kind = 11;
                            else if (diagnostic & 0xf0)
                                kind = requirement & 0xf0 ? 1 : 12;
                        } else if (combined & 0xf) {
                            diagnostic = combined;
                            if (combined & 1)
                                kind = 8;
                            else if (combined & 2)
                                kind = 9;
                            else if (combined & 8)
                                kind = 10;
                            else if (combined & 4)
                                kind = 11;
                        } else if (combined & 0xf0) {
                            diagnostic = combined;
                            kind = requirement & 0xf0 ? 1 : 12;
                        }
                    }
                    if (!kind) {
                        inputs = 0;
                        for (frames = 1; history + frames < owner->historyIndex; ++frames) {
                            inputs |= owner->history[history + frames].pressed;
                            if (((inputs | combined) & requirement) == requirement) {
                                kind = 15;
                                mask = inputs;
                                delay = frames + 1;
                                break;
                            }
                        }
                    }
                    if (!kind && (combined & requirement)) {
                        diagnostic = combined;
                        std::int32_t missing = (combined ^ requirement) & requirement;
                        if (missing & 1)
                            kind = 3;
                        else if (missing & 2)
                            kind = 4;
                        else if (missing & 8)
                            kind = 5;
                        else if (missing & 4)
                            kind = 6;
                        else if ((missing & 0xf0) && !kind)
                            kind = 7;
                    }
                    if (!kind && (requirement & 0xf0)) {
                        if ((requirement & 0xf0) != (combined & 0xf0) && (combined & 0xf0))
                            kind = 1;
                        else if (!(requirement & 0xf) && (combined & 0xf))
                            kind = 2;
                    }
                    if (!kind)
                        kind = 13;
                } else {
                    if ((requirement & combined) == requirement) {
                        pending = combined ^ requirement;
                        ++instruction;
                    } else
                        pending = combined;
                }
                ++history;
            } else {
                if (!progress)
                    kind = 20;
                else if (progress == 3) {
                    if (requirement & 0xf0)
                        kind = 7;
                    else if (requirement & 1)
                        kind = 3;
                    else if (requirement & 2)
                        kind = 4;
                    else if (requirement & 8)
                        kind = 5;
                    else if (requirement & 4)
                        kind = 6;
                    state.endInstruction = instruction;
                }
                done = 1;
            }
            break;
        }
        }
    } while (!done);
    switch (kind) {
    case 1:
    case 3 ... 12:
        mask = diagnostic;
        break;
    }
    result->kind = kind;
    result->mask = mask;
    result->delay = delay;
    result->attempts = finalAttempts;
    return kind;
}

} // namespace

#else

namespace {

// Classify the full authored input sequence, not a single test move.
// @verified - p
InstructionDiagnosis DiagnoseInstructionProgram(VFHD_t *owner, InstructionState &state) {
    InstructionDiagnosis result;
    const auto *const commands = instructionPrograms[state.program].commands;
    std::int8_t instruction = 0;
    std::int32_t history = 0, pending = 0, previous = 0, diagnostic = 0;
    std::uint8_t progress = 0, kind = 0, attempts = 0, delay = 0, window = 120;
    std::uint8_t previousRequirement = 0, previousDirection = 0, held = 0;
    std::int32_t lastMatchedHistory = 0;
    const auto finish = [&] {
        result.kind = kind;
        result.delay = delay;
        result.attempts = attempts;
        if (kind == 1 || static_cast<std::uint8_t>(kind - 3) <= 9)
            result.mask = diagnostic;
        return result;
    };
    for (;;) {
        const auto command = commands[instruction];
        switch (command.operation) {
        case 3:
        case 4:
            if (pending || previous) {
                if (kind && kind != 13) {
                    ++instruction;
                    break;
                }
                const auto bits = pending ? static_cast<std::uint8_t>(pending) : previous;
                const auto diagnosed = InstructionMaskDiagnosis(bits, false, (previousRequirement & 0xf0) != 0);
                if (diagnosed)
                    kind = diagnosed;
                attempts = 0;
                return finish();
            }
            ++instruction;
            break;
        case 5:
            ++attempts;
            ++instruction;
            break;
        case 6:
            if (lastMatchedHistory > window) {
                kind = 16;
                delay = lastMatchedHistory - window;
            }
            attempts = 0;
            return finish();
        case 7: {
            state.endInstruction = instruction;
            const auto length = InstructionValue<std::int8_t>(owner, 0x438);
            const auto savedAttempts = attempts;
            auto requirement = command.argument;
            if (history >= length) {
                if (!progress)
                    kind = 20;
                else if (progress == 3)
                    kind = InstructionMaskDiagnosis(requirement, true, false);
                attempts = 0;
                return finish();
            }
            const auto input = InstructionHistoryAt(owner, history);
            std::int32_t combined;
            if (!held || (std::uint8_t(requirement) & std::uint8_t(input.pressed) & 0xf0)) {
                combined = pending | input.pressed;
                if (input.pressed & 0xf0)
                    held = 1;
            } else {
                combined = pending | (input.pressed & 0xf);
                if (InstructionDirectionChanged(input.held, previousDirection))
                    combined |= input.held & 0xf0;
                else if (!(input.held & 0xf0))
                    held = 0;
            }
            if (combined & 0xf0)
                previousDirection = combined & 0xf0;
            previous = pending;
            pending = 0;
            if ((requirement & 0xf0) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                combined = (requirement & 0xf0) | (combined & 0xf);
            if ((requirement & 0x100) && (std::uint8_t(requirement) & std::uint8_t(combined) & 0xf0))
                requirement = (requirement & 0xf) | (combined & 0xf0);
            if (combined == requirement) {
                previous = 0;
                progress = 3;
                if (history > window) {
                    kind = 16;
                    delay = history - window;
                    attempts = savedAttempts;
                    return finish();
                }
                lastMatchedHistory = history;
                previousRequirement = requirement;
                ++instruction;
            } else if (!previous) {
                pending = combined;
                if ((requirement & combined) == requirement) {
                    pending ^= requirement;
                    ++instruction;
                }
            } else {
                progress = 2;
                if (!kind) {
                    if (requirement & combined) {
                        diagnostic = combined & (requirement ^ combined);
                        kind = InstructionMaskDiagnosis(diagnostic, false, (requirement & 0xf0) != 0);
                    } else if (combined & 0xf) {
                        diagnostic = combined;
                        kind = InstructionMaskDiagnosis(combined, false, false);
                    } else if (combined & 0xf0) {
                        diagnostic = combined;
                        kind = (requirement & 0xf0) ? 1 : 12;
                    }
                    if (!kind) {
                        std::int32_t next = history + 1, accumulated = 0;
                        std::uint8_t frames = 1;
                        for (; next < length; ++next, ++frames) {
                            accumulated |= InstructionHistoryAt(owner, next).pressed;
                            if ((requirement & (accumulated | combined)) == requirement)
                                break;
                        }
                        if (next < length) {
                            result.mask = accumulated;
                            delay = frames + 1;
                            kind = 15;
                        } else {
                            if (requirement & combined) {
                                diagnostic = combined;
                                kind = InstructionMaskDiagnosis(requirement & (requirement ^ combined), true, false);
                            }
                            if (!kind) {
                                if ((requirement & 0xf0) && (requirement & 0xf0) != (combined & 0xf0) && (combined & 0xf0))
                                    kind = 1;
                                else if ((requirement & 0xf0) && !(requirement & 0xf) && (combined & 0xf))
                                    kind = 2;
                                else
                                    kind = 13;
                            }
                        }
                    }
                }
                ++history;
                attempts = 0;
                return finish();
            }
            ++history;
            attempts = savedAttempts;
            break;
        }
        case 8:
            window = static_cast<std::uint8_t>(window == 120 ? command.argument : history + command.argument - 1);
            ++instruction;
            break;
        case 9:
        case 10:
        case 11:
        case 16:
        case 17:
        case 18:
            ++instruction;
            break;
        default:
            break;
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int8_t UpdateInstructionPrograms(FightActorRuntimePrefix *actor, InstructionState &first, InstructionState &second) {
    VFHD_t *owner = static_cast<VFHD_t *>(actor->game->dialogue);
    if (!owner)
        return 0;
    std::int8_t result = -1;
    switch (owner->instructionPhase) {
    case 0:
        owner->instructionPhase = 1;
        break;
    case 1: {
        if (owner->firstPhase) {
            owner->instructionPhase = 3;
            break;
        }
        if (--first.startCountdown == 0) {
            first.outcome = 2;
            first.failure = 20;
            owner->instructionPhase = 4;
            break;
        }
        std::int32_t ready = 0;
        if (first.program == 4 || first.program == 8) {
            if (*reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x494) & 4) {
                ready = 1;
                owner->result[7] |= std::byte(1);
            }
        } else if (g_inputBuffer[0].rawPressed & 0xf606)
            ready = 1;
        if (!ready)
            break;
        first.frameCountdown = 120;
        first.maximumWindow = 120;
        second.frameCountdown = 120;
        second.maximumWindow = 120;
        owner->instructionPhase = 2;
    }
    case 2:
        if (first.skippedFrames) {
            owner->history[owner->historyIndex].held = 0;
            owner->history[owner->historyIndex].pressed = 0;
            owner->historyIndex++;
            if (owner->historyIndex == 120)
                owner->historyIndex = 0;
            --first.skippedFrames;
        } else
            RecordInstructionInput(actor);
        if (owner->firstPhase) {
            owner->instructionPhase = 3;
            break;
        }
        if (--first.frameCountdown == 0) {
            if (!second.ended && !first.ended) {
                if (owner->firstPhase != 2 && second.attempts && second.attempts > first.attempts && !InstructionCloseBetweenCharacters(owner, second)) {
                    owner->committed = 1;
                    second.analysis = 23;
                    second.outcome = 1;
                    first.done = 1;
                    if (!owner->firstPhase)
                        owner->firstPhase = 1;
                    owner->firstHit = 1;
                    owner->instructionPhase = 4;
                    return result;
                }
                first.outcome = 2;
                first.timedOut = 1;
                if (!second.attempts && !first.attempts) {
                    if (first.resolvedMask)
                        ResetInstructionPairState(owner, std::int16_t(first.resolvedMask), std::int16_t(first.resolvedMask), first);
                    else
                        ResetInstructionPairState(owner, owner->history[0].held, owner->history[0].pressed, first);
                }
                owner->instructionPhase = 3;
                break;
            }
            second.done = first.done = 1;
        }
        if (second.program != -1 && !second.done)
            RunInstructionProgram(actor, owner, second, 0);
        if (!first.done)
            RunInstructionProgram(actor, owner, first, 1);
        if (!second.done || !first.done)
            break;
        if (second.program != -1 && second.ended) {
            if (owner->firstPhase == 2 || InstructionCloseBetweenCharacters(owner, second))
                break;
            QueueInstructionMotion(owner, 0, second.motion);
            owner->committed = 1;
            second.analysis = 22;
            if (!owner->firstPhase)
                owner->firstPhase = 1;
            owner->firstHit = 1;
            second.outcome = 1;
            owner->instructionPhase = 4;
            break;
        }
        if (first.ended) {
            if (first.timedOut) {
                if (!second.attempts && !first.attempts) {
                    if (first.resolvedMask)
                        ResetInstructionPairState(owner, std::int16_t(first.resolvedMask), std::int16_t(first.resolvedMask), first);
                    else
                        ResetInstructionPairState(owner, owner->history[0].held, owner->history[0].pressed, first);
                }
                first.failure = 17;
                owner->instructionPhase = 3;
                first.outcome = 2;
                break;
            }
            if (owner->firstPhase != 2) {
                switch (InstructionCloseBetweenCharacters(owner, first)) {
                case 0:
                    owner->committed = 1;
                    if (!owner->firstPhase)
                        owner->firstPhase = 1;
                    if (second.attempts && second.attempts > first.attempts) {
                        second.analysis = 23;
                        second.outcome = 1;
                    } else {
                        first.outcome = 1;
                        QueueInstructionMotion(owner, 0, first.motion);
                    }
                    owner->firstHit = 1;
                    owner->instructionPhase = 4;
                    break;
                case 1:
                    if (!first.attempts)
                        owner->requestedMotion = 394;
                    first.failure = 18;
                    first.outcome = 2;
                    owner->instructionPhase = 3;
                    break;
                case 2:
                    if (!first.attempts)
                        owner->requestedMotion = 394;
                    first.outcome = 2;
                    first.failure = 21;
                    owner->instructionPhase = 3;
                    break;
                case 3:
                    if (!first.attempts)
                        owner->requestedMotion = 394;
                    if (!second.attempts && !first.attempts) {
                        if (first.resolvedMask)
                            ResetInstructionPairState(owner, std::int16_t(first.resolvedMask), std::int16_t(first.resolvedMask), first);
                        else
                            ResetInstructionPairState(owner, owner->history[0].held, owner->history[0].pressed, first);
                    }
                    first.outcome = 2;
                    first.failure = 19;
                    owner->instructionPhase = 3;
                    break;
                }
            } else {
                first.outcome = 2;
                first.failure = 19;
                owner->instructionPhase = 3;
            }
        } else if (first.progress == 2) {
            if (!second.attempts && !first.attempts) {
                if (first.resolvedMask)
                    ResetInstructionPairState(owner, std::int16_t(first.resolvedMask), std::int16_t(first.resolvedMask), first);
                else
                    ResetInstructionPairState(owner, owner->history[0].held, owner->history[0].pressed, first);
            }
            first.outcome = 2;
            owner->instructionPhase = 3;
        }
        break;
    case 3:
        RecordInstructionInput(actor);
        if (--first.endCountdown == 0) {
            first.timedOut = 1;
            if (first.progress != 1 && (!first.analysis || first.analysis == 13)) {
                InstructionDiagnosis diagnosis;
                first.progress = 2;
                first.analysis = DiagnoseInstructionProgram(owner, first, &diagnosis);
                first.diagnostic = diagnosis.mask;
                first.delay = diagnosis.delay;
                first.committedAttempts = first.attempts;
                second.committedAttempts = second.attempts;
            }
            owner->instructionPhase = 4;
        }
        InstructionDiagnosisNullsub(owner, owner->history[owner->historyIndex - 1].held, owner->history[owner->historyIndex - 1].pressed, first, second);
        break;
    case 4:
        result = 0;
        break;
    }
    if (owner->instructionPhase == 4)
        result = 0;
    return result;
}

} // namespace

#else

namespace {

// Both programs share the real capture clock and completion producer.
// @verified - p
std::int8_t UpdateInstructionPrograms(VFActor *actor) {
    auto *const owner = actor->game->dialogue;
    if (!owner)
        return 0;
    auto &first = FirstInstruction(owner);
    auto &second = SecondInstruction(owner);
    auto phase = InstructionValue<std::uint8_t>(owner, 0x4cd);
    const auto setPhase = [&](std::uint8_t value) {
        phase = value;
        InstructionStore(owner, 0x4cd, value);
    };
    if (phase == 0) {
        setPhase(1);
        return -1;
    }
    if (phase == 1) {
        if (InstructionValue<std::uint8_t>(owner, 0x444)) {
            setPhase(3);
            return -1;
        }
        if (first.startCountdown-- == 1) {
            first.outcome = 2;
            first.failure = 20;
            setPhase(4);
            return 0;
        }
        const bool constrained = ((static_cast<std::uint16_t>(first.program - 4)) & 0xfffb) == 0;
        if (constrained ? !(actor->followupFlags & 4) : !(g_inputBuffer[0].newPresses & 0xf606))
            return -1;
        first.frameCountdown = second.frameCountdown = 120;
        first.maximumWindow = second.maximumWindow = 120;
        setPhase(2);
    } else if (phase == 3) {
        RecordInstructionInput(actor);
        if (first.endCountdown-- == 1) {
            first.timedOut = 1;
            if (first.progress != 1 && (!first.analysis || first.analysis == 13)) {
                first.progress = 2;
                const auto diagnosis = DiagnoseInstructionProgram(owner, first);
                first.analysis = diagnosis.kind;
                first.diagnostic = diagnosis.mask;
                first.delay = diagnosis.delay;
                first.committedAttempts = first.attempts;
                second.committedAttempts = second.attempts;
            }
            setPhase(4);
        }
        return phase == 4 ? 0 : -1;
    } else if (phase == 4)
        return 0;
    else
        return -1;
    if (first.skippedFrames) {
        auto index = InstructionValue<std::int8_t>(owner, 0x438);
        InstructionStore(owner, 0x78 + 8 * index, std::uint16_t{0});
        InstructionStore(owner, 0x7a + 8 * index, std::uint16_t{0});
        ++index;
        InstructionStore(owner, 0x438, index == 120 ? std::int8_t{0} : index);
        --first.skippedFrames;
    } else
        RecordInstructionInput(actor);
    if (InstructionValue<std::uint8_t>(owner, 0x444)) {
        setPhase(3);
        return -1;
    }
    if (first.frameCountdown-- == 1) {
        if (!first.ended && !second.ended) {
            if (InstructionValue<std::uint8_t>(owner, 0x444) != 2 && second.attempts &&
                static_cast<std::int8_t>(second.attempts) > static_cast<std::int8_t>(first.attempts) && !InstructionCloseBetweenCharacters(owner, second)) {
                InstructionStore(owner, 0x446, std::uint8_t{1});
                second.analysis = 23;
                second.outcome = 1;
                first.done = 1;
                if (!InstructionValue<std::uint8_t>(owner, 0x444))
                    InstructionStore(owner, 0x444, std::uint8_t{1});
                InstructionStore(owner, 0x447, std::uint8_t{1});
                setPhase(4);
                return -1;
            }
            first.outcome = 2;
            first.timedOut = 1;
            if (!first.attempts && !second.attempts)
                InstructionStore(owner, 0x444, std::uint16_t{514});
            setPhase(3);
            return -1;
        }
        first.done = second.done = 1;
    }
    if (second.program != -1 && !second.done)
        RunInstructionProgram(actor, owner, second);
    if (!first.done)
        RunInstructionProgram(actor, owner, first);
    if (!first.done || !second.done)
        return -1;
    if (second.program != -1 && second.ended) {
        if (InstructionValue<std::uint8_t>(owner, 0x444) != 2 && !InstructionCloseBetweenCharacters(owner, second)) {
            QueueInstructionMotion(owner, second.motion);
            InstructionStore(owner, 0x446, std::uint8_t{1});
            second.analysis = 22;
            if (!InstructionValue<std::uint8_t>(owner, 0x444))
                InstructionStore(owner, 0x444, std::uint8_t{1});
            InstructionStore(owner, 0x447, std::uint8_t{1});
            second.outcome = 1;
            setPhase(4);
        }
        return phase == 4 ? 0 : -1;
    }
    if (!first.ended) {
        if (first.progress == 2) {
            if (!first.attempts && !second.attempts)
                InstructionStore(owner, 0x444, std::uint16_t{514});
            first.outcome = 2;
            setPhase(3);
        }
        return -1;
    }
    if (first.timedOut) {
        if (!first.attempts && !second.attempts)
            InstructionStore(owner, 0x444, std::uint16_t{514});
        first.failure = 17;
        first.outcome = 2;
        setPhase(3);
        return -1;
    }
    auto close = InstructionValue<std::uint8_t>(owner, 0x444) == 2 ? std::uint8_t{3} : InstructionCloseBetweenCharacters(owner, first);
    if (!close) {
        InstructionStore(owner, 0x446, std::uint8_t{1});
        if (!InstructionValue<std::uint8_t>(owner, 0x444))
            InstructionStore(owner, 0x444, std::uint8_t{1});
        if (second.attempts && static_cast<std::int8_t>(second.attempts) > static_cast<std::int8_t>(first.attempts)) {
            second.analysis = 23;
            second.outcome = 1;
        } else {
            first.outcome = 1;
            QueueInstructionMotion(owner, first.motion);
        }
        InstructionStore(owner, 0x447, std::uint8_t{1});
        setPhase(4);
        return 0;
    }
    if (close <= 3) {
        if (!first.attempts)
            owner->requestedMotion = 394;
        if (close == 3 && !first.attempts && !second.attempts)
            InstructionStore(owner, 0x444, std::uint16_t{514});
        first.failure = close == 1 ? 18 : close == 2 ? 21 : 19;
        first.outcome = 2;
        setPhase(3);
    }
    return -1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t UpdateInstructionPose(FightActorRuntimePrefix *actor, InstructionState &first, InstructionState &second) {
    VFHD_t *owner = static_cast<VFHD_t *>(actor->game->dialogue);
    if (!owner)
        return 0;
    bool published[2];
    Vector3i rotation;
    Vector3 position;
    published[0] = false;
    published[1] = false;
    std::int32_t result = 0;
    if (owner->partnerMode > 0) {
        if (owner->firstPhase) {
            if (owner->firstPhase == 1) {
                ++owner->firstTimer;
                if (owner->firstTimer >= owner->firstHitStart && owner->firstTimer <= owner->firstHitEnd && owner->currentMotion != 394 &&
                    InstructionHitWindowAdmitted(owner, owner->firstName)) {
                    switch (owner->firstMotion) {
                    case 221:
                        owner->firstHitReaction = 1;
                        break;
                    case 210:
                        owner->firstHitReaction = 12;
                        InstructionPoseFrame(owner->character);
                        InstructionPoseFrame(owner->secondCharacter);
                        break;
                    default:
                        owner->firstHitReaction = 3;
                        break;
                    }
                    owner->firstPhase = 2;
                    owner->secondPhase = 2;
                    owner->firstHit = 1;
                    owner->committed = 1;
                }
            } else
                ++owner->firstTimer;
        }
        switch (owner->secondPhase) {
        case 0:
            if (owner->programMode == 3) {
                if (owner->dialogueCountdown == 10)
                    vfmDispatchControl(1, owner->firstName, 2, 1);
                if (cvsHasValidConverseData(0) && GetConverseDataForChara(SHEN_NAME(SYE_))) {
                    if (owner->dialogueCountdown && owner->dialogueCountdown != 15)
                        --owner->dialogueCountdown;
                } else if (owner->dialogueCountdown) {
                    if (--owner->dialogueCountdown == 10 && owner->partnerMode == 2)
                        QueueInstructionDialogue(owner, 2);
                    if (!owner->dialogueCountdown && owner->partnerMode == 2) {
                        owner->secondRequestedMotion = 2326;
                        owner->secondMotion = 2326;
                        owner->secondTimer = 0;
                        owner->secondPhase = 1;
                        const std::uint8_t *entry = MOTN_FindAnimationExtraRecord(owner->secondMotion, 2);
                        owner->secondHitStart = reinterpret_cast<const std::int16_t *>(entry)[1] - 1;
                        owner->secondHitEnd = reinterpret_cast<const std::int16_t *>(entry)[2];
                    }
                } else
                    owner->dialogueCountdown = 1;
            }
            break;
        case 1:
            ++owner->secondTimer;
            if (owner->secondTimer >= owner->secondHitStart && owner->secondTimer <= owner->secondHitEnd &&
                InstructionHitWindowAdmitted(owner, owner->secondName)) {
                owner->secondHitReaction = 3;
                owner->firstPhase = 2;
                owner->secondPhase = 2;
                owner->secondHit = 1;
                if (!first.failure) {
                    first.outcome = 2;
                    first.failure = 19;
                }
            }
            break;
        default:
            ++owner->secondTimer;
            break;
        }
    }
    if (owner->requestedMotion && (owner->requestedMotion != owner->currentMotion || owner->republish)) {
        switch (InstructionAnimation(owner->character)) {
        case 221:
            if (owner->requestedMotion == 210 && InstructionPoseFrame(owner->secondCharacter) < 12.0f)
                goto publishSecond;
            break;
        case 52:
            if (owner->requestedMotion == 54 && InstructionPoseFrame(owner->character) < 24.0f)
                goto publishSecond;
            break;
        }
        if ((first.outcome == 1 || second.outcome == 1) && owner->committed == 1) {
            if (owner->holdOnSuccess != 1) {
                EV_figpAssignFlagsFor(owner->character, 0, 1);
                InstructionAssignMotScaleAndDir(owner->character, 8, 2);
                InstructionAssignFigureMode(owner->character, 1);
                if (InstructionAnimation(owner->character) != 306 || owner->requestedMotion != 306)
                    vfPublishInstructionActorMotion(actor, owner->character, owner->firstName, owner->requestedMotion);
            }
        } else if (InstructionAnimation(owner->character) != 306 || owner->requestedMotion != 306)
            vfPublishInstructionActorMotion(actor, owner->character, owner->firstName, owner->requestedMotion);
        owner->republish = 0;
        published[0] = true;
        if (owner->firstEvalTime > 0)
            InstructionSetRegionEvalTime(owner->firstEvalTime, owner->character);
        if (owner->firstEndFrame > 0)
            InstructionSetLayerEndFrame(owner->firstEndFrame, owner->character);
        owner->firstEvalTime = -1.0f;
        owner->firstEndFrame = -1.0f;
        owner->currentMotion = owner->requestedMotion;
    }
publishSecond:
    if (owner->secondRequestedMotion && (owner->secondRequestedMotion != owner->secondCurrentMotion || owner->secondRepublish)) {
        if (owner->partnerMode) {
            vfPublishInstructionActorMotion(actor->actor, owner->secondCharacter, owner->secondName, owner->secondRequestedMotion);
            owner->secondRepublish = 0;
            published[1] = true;
        }
        owner->secondCurrentMotion = owner->secondRequestedMotion;
    }
    if (owner->partnerMode) {
        if (InstructionAnimation(owner->character) == 306 && !published[0]) {
            InstructionGetLocalPosition(owner->secondCharacter, &position);
            StepCharacterYawTowardPoint(owner->character, &position, 1820);
        }
        if (InstructionAnimation(owner->secondCharacter) == 306 && !published[1]) {
            InstructionGetLocalPosition(owner->character, &position);
            StepCharacterYawTowardPoint(owner->secondCharacter, &position, 1820);
        }
    }
    if (owner->turnOnReaction && owner->requestedMotion && (owner->requestedMotion == 5 || owner->requestedMotion == 326)) {
        InstructionGetLocalRotation(owner->character, &rotation);
        rotation.y += 0x8000;
        InstructionSetLocalRotation(owner->character, &rotation);
    }
    bool pairedReaction = false;
    if (owner->partnerMode > 0 && (owner->firstMotion == 210 || owner->firstMotion == 221))
        pairedReaction = true;
    if ((first.outcome == 1 || second.outcome == 1) && owner->committed == 1 && owner->holdOnSuccess == 1) {
        owner->currentMotion = 0;
        InstructionAssignFigureMode(owner->character, 0);
        result = 1;
    } else if (owner->currentMotion && owner->currentMotion != 306 && InstructionSetRegionFlag(owner->character)) {
        owner->currentMotion = 0;
        InstructionAssignFigureMode(owner->character, 0);
    }
    if (owner->secondCurrentMotion && owner->secondCurrentMotion != 306 && InstructionSetRegionFlag(owner->secondCharacter))
        owner->secondCurrentMotion = 0;
    if (!owner->currentMotion && !owner->secondCurrentMotion)
        result = 0;
    if (owner->secondPublishedMotion == 476 && InstructionSetRegionFlag(owner->secondCharacter))
        result = 1;
    if (owner->requestedMotion == owner->currentMotion)
        owner->requestedMotion = 0;
    if (owner->secondRequestedMotion == owner->secondCurrentMotion)
        owner->secondRequestedMotion = 0;
    if (owner->currentMotion == 306 && owner->secondCurrentMotion == 306)
        result = 1;
    if ((first.outcome == 1 || second.outcome == 1) && owner->committed == 1 && owner->holdOnSuccess == 1)
        result = 1;
    return result;
}

} // namespace

#else

namespace {

// Real FGTM reaction publication, FIGP playback and paired pose retention.
// @verified - p
std::uint8_t UpdateInstructionPose(VFActor *actor) {
    auto *const owner = actor->game->dialogue;
    if (!owner)
        return 0;
    auto &first = FirstInstruction(owner);
    auto &second = SecondInstruction(owner);
    bool result = false, firstPublished = false, secondPublished = false;
    const auto mode = InstructionValue<std::int8_t>(owner, 0x443);
    if (mode >= 1) {
        const auto firstPhase = InstructionValue<std::int8_t>(owner, 0x444);
        if (firstPhase) {
            const auto time = static_cast<std::int16_t>(InstructionValue<std::uint16_t>(owner, 0x468) + 1);
            InstructionStore(owner, 0x468, time);
            if (firstPhase == 1 && time >= InstructionValue<std::int16_t>(owner, 0x44a) && time <= InstructionValue<std::int16_t>(owner, 0x44c) &&
                owner->currentMotion != 394 && InstructionHitWindowAdmitted(owner, InstructionValue<shen_name>(owner, 0x474))) {
                InstructionStore(owner, 0x470, std::uint16_t{3});
                InstructionStore(owner, 0x444, std::uint32_t{0x1010202});
            }
        }
        const auto secondPhase = InstructionValue<std::int8_t>(owner, 0x445);
        if (secondPhase) {
            const auto time = static_cast<std::int16_t>(InstructionValue<std::uint16_t>(owner, 0x46a) + 1);
            InstructionStore(owner, 0x46a, time);
            if (secondPhase == 1 && time >= InstructionValue<std::int16_t>(owner, 0x45a) && time <= InstructionValue<std::int16_t>(owner, 0x45c) &&
                InstructionHitWindowAdmitted(owner, InstructionValue<shen_name>(owner, 0x478))) {
                InstructionStore(owner, 0x46e, std::uint16_t{3});
                InstructionStore(owner, 0x444, std::uint16_t{514});
                InstructionStore(owner, 0x448, std::uint8_t{1});
                if (!first.failure) {
                    first.outcome = 2;
                    first.failure = 19;
                }
            }
        } else if (InstructionValue<std::uint8_t>(owner, 0x442) == 3) {
            auto time = InstructionValue<std::int16_t>(owner, 0x46c);
            if (time == 10) {
                const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                auto *const character = scenFindChara(control->scenarioTask, InstructionValue<shen_name>(owner, 0x474));
                auto *const game = vfmaGetTaskParam();
                if (game && character) {
                    for (std::size_t i = 0; i != 6; ++i)
                        if (game->characters[i] == character) {
                            InstructionStore(reinterpret_cast<VFHD_t *>(game->actors[i]), 0x898, std::uint64_t{2});
                            break;
                        }
                }
            }
            if ((cvsHasValidConverseData() && GetConverseDataForChara(SHEN_NAME(SYE_))) || (actor->game->mode & 0x8000)) {
                if (time && time != 15)
                    InstructionStore(owner, 0x46c, static_cast<std::int16_t>(time - 1));
            } else if (time) {
                InstructionStore(owner, 0x46c, --time);
                if (time == 10 && mode == 2)
                    QueueInstructionDialogue(owner, 2);
                if (!time && mode == 2) {
                    InstructionStore(owner, 0x464, std::uint32_t{2326});
                    owner->secondRequestedMotion = 2326;
                    InstructionStore(owner, 0x46a, std::uint16_t{0});
                    InstructionStore(owner, 0x445, std::uint8_t{1});
                    const auto entry = FindLoadedMotionSecondaryEntry(2326, 2);
                    InstructionStore(
                        owner, 0x45a, static_cast<std::uint16_t>(InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(entry.data()), 2) - 1));
                    InstructionStore(owner, 0x45c, InstructionValue<std::uint16_t>(reinterpret_cast<const VFHD_t *>(entry.data()), 4));
                }
            } else
                InstructionStore(owner, 0x46c, std::uint16_t{1});
        }
    }
    const bool successful = (first.outcome == 1 || second.outcome == 1) && InstructionValue<std::uint8_t>(owner, 0x446) == 1;
    if (owner->requestedMotion && (owner->requestedMotion != owner->currentMotion || InstructionValue<std::uint8_t>(owner, 0x4a0))) {
        bool skipPublication = false;
        InstructionAnimation(owner->character);
        if (successful) {
            if (InstructionValue<std::uint8_t>(owner, 0x458) == 1)
                skipPublication = true;
            else {
                EV_figpAssignFlagsFor(owner->character, 0, 1);
                figpAssignMotScaleAndDirFor(owner->character, 8, 2);
                figpAssignModeForCharacter(owner->character, 1);
                if (InstructionAnimation(owner->character) == 306 && owner->requestedMotion == 306)
                    skipPublication = true;
                else if (((static_cast<std::uint16_t>(owner->programIndices[0] - 4)) & 0xfffb) == 0 &&
                    actor->animationTime < float(static_cast<std::uint16_t>(actor->parameter2d0)))
                    return 0;
            }
        } else if (InstructionAnimation(owner->character) == 306 && owner->requestedMotion == 306)
            skipPublication = true;
        if (!skipPublication)
            PublishInstructionActorMotion(actor, owner->character, InstructionValue<shen_name>(owner, 0x474), owner->requestedMotion);
        firstPublished = true;
        InstructionStore(owner, 0x4a0, std::uint8_t{0});
        const float start = InstructionValue<float>(owner, 0x450), end = InstructionValue<float>(owner, 0x454);
        if (start > 0)
            mtwkSetRegionEvalTime(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(owner->character), start, MotionRegion::LowerBody);
        if (end > 0)
            mtwkSetLayerEndFrame(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(owner->character), end, MotionRegion::LowerBody);
        owner->currentMotion = owner->requestedMotion;
        InstructionStore(owner, 0x450, -1.0f);
        InstructionStore(owner, 0x454, -1.0f);
    }
    if (owner->secondRequestedMotion && (owner->secondRequestedMotion != owner->secondCurrentMotion || InstructionValue<std::uint8_t>(owner, 0x4a1))) {
        if (mode) {
            PublishInstructionActorMotion(actor->actor, owner->secondCharacter, InstructionValue<shen_name>(owner, 0x478), owner->secondRequestedMotion);
            InstructionStore(owner, 0x4a1, std::uint8_t{0});
            secondPublished = true;
        }
        owner->secondCurrentMotion = owner->secondRequestedMotion;
    }
    if (mode) {
        if (InstructionAnimation(owner->character) == 306 && !firstPublished)
            ConvergeInstructionYaw(owner->character, GetCharacterWorldPosition(owner->secondCharacter), 1820);
        if (InstructionAnimation(owner->secondCharacter) == 306 && !secondPublished)
            ConvergeInstructionYaw(owner->secondCharacter, GetCharacterWorldPosition(owner->character), 1820);
    }
    if (actor->targetingCharacter) {
        const auto firstStart = InstructionValue<Vector3>(owner, 0x500), secondStart = InstructionValue<Vector3>(owner, 0x50c);
        AdjustInstructionPairPosition(actor, firstStart, secondStart);
        AdjustInstructionPairPosition(actor->actor, secondStart, firstStart);
    }
    if (successful && InstructionValue<std::uint8_t>(owner, 0x458) == 1) {
        owner->currentMotion = 0;
        figpAssignModeForCharacter(owner->character, 0);
        result = true;
    } else if (owner->currentMotion && owner->currentMotion != 306 && InstructionMotionEnded(owner->character)) {
        owner->currentMotion = 0;
        figpAssignModeForCharacter(owner->character, 0);
    }
    if (owner->secondCurrentMotion && owner->secondCurrentMotion != 306 && InstructionMotionEnded(owner->secondCharacter))
        owner->secondCurrentMotion = 0;
    if (!owner->currentMotion && !owner->secondCurrentMotion)
        result = false;
    if (InstructionValue<std::uint32_t>(owner, 0x4c8) == 476 && InstructionMotionEnded(owner->secondCharacter))
        result = true;
    if (owner->requestedMotion == owner->currentMotion)
        owner->requestedMotion = 0;
    if (owner->secondRequestedMotion == owner->secondCurrentMotion)
        owner->secondRequestedMotion = 0;
    if (owner->currentMotion == 306 && owner->secondCurrentMotion == 306)
        result = true;
    if (successful && InstructionValue<std::uint8_t>(owner, 0x458) == 1)
        return 1;
    return result;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Names belong to FENE, not a separately manufactured character view.
VFActor *vfGetActorFromSlot(VFMA_t *game, shen_name name) {
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->actors[i]->slot && game->actors[i]->slot->id == name.name)
            return game->actors[i];
    return nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t vfApplyMoveInstructionConfiguration(
    VFHD_t *input, shen_name character, shen_name secondCharacter, std::uint16_t program, std::uint16_t secondProgram, std::byte *result) {
    VFHD_t *owner = input;
    owner->firstName = character;
    owner->secondName = secondCharacter;
    owner->state = 0;
    owner->requestedMotion = 0;
    owner->secondRequestedMotion = 0;
    owner->currentMotion = 0;
    owner->secondCurrentMotion = 0;
    owner->programIndices[0] = program;
    owner->programIndices[1] = secondProgram;
    owner->result = result;
    owner->result[0] = std::byte(0);
    owner->result[1] = std::byte(0);
    owner->result[2] = std::byte(0);
    owner->result[3] = std::byte(0xff);
    *reinterpret_cast<std::uint32_t *>(owner->result + 8) = 0;
    owner->result[7] = std::byte(0);
    owner->secondTimer = 0;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    HLib::Task *const scenario = control->scenarioTask;
    owner->character = scenFindChara(scenario, character);
    if (secondCharacter)
        owner->secondCharacter = scenFindChara(scenario, secondCharacter);
    else
        owner->secondCharacter = 0;
    return SetPlayerControlEnabled(0);
}

#else

// Consume the genuine scenario/configuration producer.
// @verified - p
std::int32_t vfApplyMoveInstructionConfiguration(VFHD_t *owner) {
    owner->programIndices[0] = static_cast<std::uint16_t>(instructionProgram);
    owner->programIndices[1] = static_cast<std::uint16_t>(instructionSecondProgram);
    owner->result = instructionResult;
    InstructionStore(owner, 0x474, instructionCharacter);
    InstructionStore(owner, 0x478, instructionSecondCharacter);
    InstructionStore(owner, 0x440, std::uint8_t{0});
    owner->requestedMotion = owner->secondRequestedMotion = 0;
    owner->currentMotion = owner->secondCurrentMotion = 0;
    owner->result[0] = owner->result[1] = owner->result[2] = owner->result[7] = std::byte{0};
    owner->result[3] = std::byte{0xff};
    std::memset(owner->result + 8, 0, 4);
    InstructionStore(owner, 0x46a, std::uint16_t{0});
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    owner->character = scenFindChara(control->scenarioTask, instructionCharacter);
    owner->secondCharacter = instructionSecondCharacter.name ? scenFindChara(control->scenarioTask, instructionSecondCharacter) : nullptr;
    return SetPlayerControlEnabled(0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void InitializeInstructionHistory(VFHD_t *owner) {
    for (std::int8_t index = 0; index < 120; ++index) {
        owner->history[index].held = 0;
        owner->history[index].pressed = 0;
        owner->history[index].reserved = 0;
    }
    owner->historyIndex = 0;
    owner->turnOnReaction = owner->partnerMode ? 1 : owner->partnerMode;
    reinterpret_cast<std::uint16_t *>(owner->retained4d8 + 12)[0] = 0;
    reinterpret_cast<std::uint16_t *>(owner->retained4d8 + 12)[1] = 0;
    owner->instructionPhase = 0;
    owner->firstPhase = 0;
    owner->secondPhase = 0;
    owner->firstMotion = 0;
    owner->secondMotion = 0;
    owner->republish = 0;
    owner->secondRepublish = 0;
    owner->firstHit = 0;
    owner->secondHit = 0;
    owner->dialogueCountdown = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 10.0f) + 50;
    owner->committed = 0;
    owner->secondHitReaction = 0;
    owner->firstHitReaction = 0;
    owner->frame = 0;
    owner->retained4b2[0] = std::byte(0);
    owner->retained4b2[1] = std::byte(0);
    owner->firstEvalTime = -1.0f;
    owner->firstEndFrame = -1.0f;
    owner->holdOnSuccess = 0;
    owner->firstPublishedYaw = 0;
    owner->firstPublishedMotion = 0;
    owner->secondPublishedYaw = 0;
    owner->secondPublishedMotion = 0;
    owner->inputCount = 0;
    InstructionAssignFigureMode(owner->character, 0);
    EV_figpAssignFlagsFor(owner->character, 0, 1);
    if (owner->secondName)
        EV_figpAssignFlagsFor(owner->secondCharacter, 0, 1);
}

} // namespace

#else

namespace {

void InitializeInstructionHistory(VFHD_t *owner) {
    std::memset(reinterpret_cast<std::byte *>(owner) + 0x78, 0, 120 * 8);
    InstructionStore(owner, 0x438, std::uint8_t{0});
    InstructionStore(owner, 0x518, std::uint16_t{0});
    InstructionStore(owner, 0x51a, std::uint16_t{0});
    InstructionStore(owner, 0x449, static_cast<std::uint8_t>(InstructionValue<std::uint8_t>(owner, 0x443) != 0));
    for (auto offset : {0x4cd, 0x444, 0x445, 0x4a0, 0x4a1, 0x446, 0x4cc})
        InstructionStore(owner, offset, std::uint8_t{0});
    InstructionStore(owner, 0x460, std::uint64_t{0});
    InstructionStore(owner, 0x447, std::uint16_t{0});
    InstructionStore(owner, 0x46e, std::uint16_t{0});
    InstructionStore(owner, 0x470, std::uint16_t{0});
    owner->frame = 0;
    InstructionStore(owner, 0x46c, static_cast<std::uint16_t>(50 - static_cast<std::int32_t>(double(IncrementSharedPrngFloat()) * -10.0)));
    InstructionStore(owner, 0x4ce, std::uint16_t{0});
    InstructionStore(owner, 0x450, -1.0f);
    InstructionStore(owner, 0x454, -1.0f);
    InstructionStore(owner, 0x458, std::uint8_t{0});
    for (auto offset : {0x4bc, 0x4c0, 0x4c4, 0x4c8})
        InstructionStore(owner, offset, std::uint32_t{0});
    figpAssignModeForCharacter(owner->character, 0);
    EV_figpAssignFlagsFor(owner->character, 0, 1);
    if (InstructionValue<shen_name>(owner, 0x478).name)
        EV_figpAssignFlagsFor(owner->secondCharacter, 0, 1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void PublishInstructionActorMotion(FightActorRuntimePrefix *actor, Character *character, shen_name name, std::int32_t motion) {
    __typeof__(static_cast<VFHD_t *>(actor->game->dialogue)) owner = static_cast<VFHD_t *>(actor->game->dialogue);
    charFindMotionwork(character);
    __typeof__(static_cast<std::int32_t>(InstructionCharacterYaw(GetCharacterNode(character)))) yaw =
        static_cast<std::int32_t>(InstructionCharacterYaw(GetCharacterNode(character)));
    if (name == InstructionValue<shen_name>(owner, 0x46c)) {
        InstructionStore(owner, 0x4a8, motion);
        InstructionStore(owner, 0x4a0, yaw);
    } else {
        InstructionStore(owner, 0x4ac, motion);
        InstructionStore(owner, 0x4a4, yaw);
    }
    InstructionActorValue<std::uint32_t>(actor, 0x318) = static_cast<std::uint32_t>(motion);
    InstructionActorValue<float>(actor, 0x864) = 0;
}

} // namespace

#else

namespace {

void PublishInstructionActorMotion(VFActor *actor, Character *character, shen_name name, std::int32_t motion) {
    auto *const owner = actor->game->dialogue;

#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(character);
    const auto yaw = static_cast<std::int32_t>(GetAllAngYForNode(GetCharacterNode(character)));
    if (name.name == InstructionValue<shen_name>(owner, 0x474).name) {
        InstructionStore(owner, 0x4c4, motion);
        InstructionStore(owner, 0x4bc, yaw);
    } else {
        InstructionStore(owner, 0x4c8, motion);
        InstructionStore(owner, 0x4c0, yaw);
    }
    actor->parameter370 = static_cast<std::uint64_t>(static_cast<std::int64_t>(motion));
    actor->reactionDepth = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void ConvergeInstructionYaw(Character *character, const Vector3 &target, std::int32_t maximumStep) {
    __typeof__(InstructionCharacterPosition(character)) position = InstructionCharacterPosition(character);
    __typeof__(InstructionCharacterRotation(character)) rotation = InstructionCharacterRotation(character);
    __typeof__(static_cast<std::uint16_t>(FixedAtan2(target.x - position.x, target.z - position.z) + 32768)) desired =
        static_cast<std::uint16_t>(FixedAtan2(target.x - position.x, target.z - position.z) + 32768);
    __typeof__(static_cast<std::uint16_t>(desired - rotation.y)) clockwise = static_cast<std::uint16_t>(desired - rotation.y);
    std::int32_t delta;
    if (clockwise >= static_cast<std::uint16_t>(rotation.y - desired))
        delta = static_cast<std::uint16_t>(rotation.y - desired);
    else if (desired <= rotation.y)
        delta = -std::int32_t(clockwise);
    else
        delta = rotation.y - desired;
    if ((delta < 0 ? -delta : delta) > maximumStep)
        rotation.y += delta < 0 ? maximumStep : -maximumStep;
    charaSetWorldRot(character, rotation);
}

} // namespace

#else

namespace {

void ConvergeInstructionYaw(Character *character, const Vector3 &target, std::int32_t maximumStep) {
    const auto position = GetCharacterWorldPosition(character);
    auto rotation = GetCharacterWorldRotation(character);
    const auto desired = static_cast<std::uint16_t>(FixedAtan2(target.x - position.x, target.z - position.z) + 32768);
    const auto clockwise = static_cast<std::uint16_t>(desired - rotation.y);
    std::int32_t delta;
    if (clockwise >= static_cast<std::uint16_t>(rotation.y - desired))
        delta = static_cast<std::uint16_t>(rotation.y - desired);
    else if (desired <= rotation.y)
        delta = -std::int32_t(clockwise);
    else
        delta = rotation.y - desired;
    if (std::abs(delta) > maximumStep)
        rotation.y += delta < 0 ? maximumStep : -maximumStep;
    charaSetWorldRot(character, rotation);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void AdjustInstructionPairPosition(VFActor *actor, const Vector3 &start, const Vector3 &otherStart) {
    if ((actor->motionFlags & (VFMOF_Unknown200 | VFMOF_Unknown4000)) != 0x200 &&
        !((actor->motionFlags & VFMOF_Unknown100) && (actor->hitStateFlags & VFHSF_Unknown4) && !(actor->motionFlags & VFMOF_Unknown80000)))
        return;
    const auto position = GetCharacterWorldPosition(actor->character);
    const float dx = position.x - otherStart.x, dz = position.z - otherStart.z;
    const float distance = std::sqrt(dx * dx + dz * dz);
    const Vector3 separation{start.x - otherStart.x, start.y - otherStart.y, start.z - otherStart.z};
    const float scale = distance / std::sqrt((separation.y * separation.y + separation.x * separation.x) + separation.z * separation.z);
    const float x = separation.x * scale + otherStart.x, z = separation.z * scale + otherStart.z;
    if (double((position.x - x) * (position.x - x) + (position.z - z) * (position.z - z)) > .0025) {
        actor->position.x = (x + position.x) * .5f;
        actor->position.z = (z + position.z) * .5f;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

bool InstructionMotionEnded(Character *character) {
    return static_cast<std::uint16_t>(mtwkSetRegionFlag(charFindMotionwork(character), 65, 1, static_cast<MotionRegion>(0))) != 0;
}

} // namespace

#else

namespace {

bool InstructionMotionEnded(Character *character) {
    return static_cast<std::uint16_t>(mtwkSetRegionFlag(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(character), 65, 1, MotionRegion::LowerBody)) != 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void InitializeInstructionState(InstructionState &state) {
    state.motion = 0;
    state.previousMask = state.pendingMask = state.resolvedMask = 0;
    state.heldDirection = 0;
    state.previousDirection = 0;
    state.outcome = 0;
    state.progress = 0;
    state.instruction = 0;
    state.historyIndex = 0;
    state.ended = 0;
    state.failure = 0;
    state.analysis = 0;
    state.timedOut = 0;
    state.startCountdown = 240;
    state.endInstruction = 0;
    state.endCountdown = 30;
    state.attempts = 0;
    state.endFrame = 0;
    state.maximumWindow = 120;
    state.skippedFrames = 0;
    if (state.program == -1)
        state.done = 1;
    else
        state.done = 0;
}

} // namespace

#else

namespace {

void InitializeInstructionState(InstructionState &state) {
    state.timedOut = 0;
    state.startCountdown = 240;
    state.motion = 0;
    state.endFrame = 0;
    state.previousMask = state.resolvedMask = state.pendingMask = 0;
    state.historyIndex = 0;
    state.heldDirection = 0;
    state.previousDirection = 0;
    state.outcome = state.progress = state.failure = state.analysis = 0;
    state.instruction = 0;
    state.endInstruction = state.ended = state.attempts = 0;
    state.done = state.program == -1;
    state.endCountdown = 30;
    state.maximumWindow = 120;
    state.skippedFrames = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void InstructionAnalysisNullsub(VFHD_t *, const InstructionState &state) {
    if (static_cast<const volatile InstructionState &>(state).analysis)
        return;
}

// @verified - d
void InstructionFailureNullsub() {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

InstructionState &FirstInstruction(VFHD_t *owner) {
    return *reinterpret_cast<InstructionState *>(reinterpret_cast<std::byte *>(owner) + 4);
}

} // namespace

#else

namespace {

InstructionState &FirstInstruction(VFHD_t *owner) {
    return *reinterpret_cast<InstructionState *>(reinterpret_cast<std::byte *>(owner) + 8);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

InstructionState &SecondInstruction(VFHD_t *owner) {
    return *reinterpret_cast<InstructionState *>(reinterpret_cast<std::byte *>(owner) + 0x3c);
}

} // namespace

#else

namespace {

InstructionState &SecondInstruction(VFHD_t *owner) {
    return *reinterpret_cast<InstructionState *>(reinterpret_cast<std::byte *>(owner) + 0x40);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

const InstructionHistory &InstructionHistoryAt(const VFHD_t *owner, std::int32_t index) {
    return *reinterpret_cast<const InstructionHistory *>(reinterpret_cast<const std::byte *>(owner) + 0x74 + 8 * index);
}

} // namespace

#else

namespace {

const InstructionHistory &InstructionHistoryAt(const VFHD_t *owner, std::int64_t index) {
    return *reinterpret_cast<const InstructionHistory *>(reinterpret_cast<const std::byte *>(owner) + 0x78 + 8 * index);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::uint16_t InstructionAnimation(Character *character) {
    return mtwkGetAnimationId(charFindMotionwork(character), static_cast<MotionRegion>(0));
}

float InstructionPoseFrame(Character *character) {
    return mtwkGetFrame(charFindMotionwork(character), static_cast<MotionRegion>(0));
}

void InstructionSampleRootPosition(Character *character, std::uint16_t animation, Vector3 *position) {
    Vector3i rotation;
    GetCharacterLocalRotation(character, &rotation);
    SampleNativeMotionRoot(animation, mtwkGetRigBoneId(charFindMotionwork(character)), -1.0f, position);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(rotation.y);
    Matrix4x4::TransformPoint(*position, *position);
    PopCurrentMatrix(1);
}

} // namespace

#else

namespace {

std::uint16_t InstructionAnimation(Character *character) {
    return mtwkGetAnimationId(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(character), MotionRegion::LowerBody);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfClAicaReact(std::uint32_t flags, const Vector3 *position) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    FightRuntimePrefix *game;
    std::int32_t material;
    if (task && (game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task))) != 0 && (g_pFightStage->battleFlags & 0x40U))
        material = static_cast<std::uint8_t>(game->cameraParameter);
    else
        material = static_cast<std::int16_t>(GetFieldSoundProperties(position) >> 16U);
    const std::uint32_t command = static_cast<std::uint32_t>(material) << 19U | 0x7f000000U | flags;
    Execute3DSoundCommand(command, position, 2.0f, 30.0f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfInitializeFightDialogue(FightRuntimePrefix *game) {
    DialogueValue<std::int16_t>(game, 0x22e) = -1;
    DialogueValue<std::uint16_t>(game, 0x230) = 0;
    dialoguePlayerFocus = 0;
    std::int8_t bankIndex = 3;
    DialogueValue<std::int8_t>(game, 0x22c) = bankIndex;
    FightDialogueRecord *const bank = dialogueBanks[bankIndex];
    do {
        for (std::uint8_t index = 0; bank[index].voice; ++index)
            bank[index].played = 0;
    } while (0);
}

#else

// Reset only played flags; authored delays and links remain intact.
// @verified - p
void vfInitializeFightDialogue(VFMA_t *game) {
    SetDialogueIndex(*game, -1);
    game->frameDelta = 0;
    dialoguePlayerFocus = 0;
    game->reserved318[0] = std::byte{3};
    for (auto *record = dialogueBanks[3]; record->voice; ++record)
        record->played = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool vfRequestFightDialogue(FightRuntimePrefix *game, std::int16_t index) {
    if (cvsHasValidConverseData(0) || DialogueValue<std::int16_t>(game, 0x22e) != -1 || (game->stateFlags & 4U) || (g_pFightStage->battleFlags & 2U))
        return false;
    FightDialogueRecord *const bank = DialogueBank(game);
    if (bank[index].played == 1)
        return false;
    DialogueValue<std::int16_t>(game, 0x22e) = index;
    DialogueValue<std::uint16_t>(game, 0x230) = bank[index].delay;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfSetFightDialogue(FightRuntimePrefix *game, std::int16_t index) {
    if (DialogueValue<std::int8_t>(game, 0x22c) == -1)
        return;
    FightDialogueRecord *const bank = DialogueBank(game);
    DialogueValue<std::int16_t>(game, 0x22e) = index;
    DialogueValue<std::uint16_t>(game, 0x230) = bank[index].delay;
}

#else

// Select an authored dialogue without replacing its bank.
// @verified - p
void vfSetFightDialogue(VFMA_t *game, std::int16_t index) {
    if (game->reserved318[0] == std::byte{0xff})
        return;
    SetDialogueIndex(*game, index);
    game->frameDelta = static_cast<float>(DialogueBank(*game)[index].delay);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
const char *vfGetFightDialogueVoice(FightRuntimePrefix *game, std::int16_t index) {
    return DialogueValue<std::int8_t>(game, 0x22c) == -1 ? 0 : DialogueBank(game)[index].voice;
}

#else

// Retain the native bank's genuine absent-record result.
// @verified - p
const char *vfGetFightDialogueVoice(VFMA_t *game, std::int16_t index) {
    if (game->reserved318[0] == std::byte{0xff})
        return nullptr;
    return DialogueBank(*game)[index].voice;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool vfHasDialoguePlayerFocus() {
    return dialoguePlayerFocus != 0;
}

#else

bool vfHasDialoguePlayerFocus() {
    return dialoguePlayerFocus != 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfTryFightDialogue(FightRuntimePrefix *game, std::int16_t index) {
    if (game->elapsedTicks > 449 && !*reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(game) + 0x232) &&
        !(dc::sdk::RandomFloat() > 0.5f))
        vfRequestFightDialogue(game, index);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfTryReactionDialogue(FightActorRuntimePrefix *actor, std::uint8_t kind) {
    FightRuntimePrefix *const game = actor->game;
    std::int32_t bank = -1;
    if (cvsHasValidConverseData(0))
        return;
    if (DialogueValue<std::int16_t>(game, 0x230))
        return;
    if (game->elapsedTicks <= 149)
        return;
    if (!DialogueValue<std::uint16_t>(actor, 2))
        bank = 1;
    if (bank == -1)
        return;
    const std::uint8_t *const list = dialogueReactionLists[bank][kind];
    if (!list)
        return;
    if (dc::sdk::RandomFloat() > dialogueReactionChances[kind])
        return;
    for (std::uint8_t index = 1; index < static_cast<std::uint8_t>(list[0] + 1); ++index)
        if (vfRequestFightDialogue(game, list[index]) == 1)
            break;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfmaSetupDialogueReactionsEtc(FightRuntimePrefix *game) {
    std::uint32_t request[3];
    Vector3 origin, destination;
    FightDialogueRecord *bank;
    std::int32_t index, slot;
    std::uint16_t cameraIndex;
    index = DialogueValue<std::int16_t>(game, 0x22e);
    if (game->stateFlags & 4U) {
        index = -1;
        DialogueValue<std::uint16_t>(game, 0x230) = 0;
    }
    if (index == -1)
        return;
    if (DialogueValue<std::uint16_t>(game, 0x230))
        return static_cast<void>(--DialogueValue<std::uint16_t>(game, 0x230));
    if (index == -2) {
        game->stateFlags &= ~1U;
        DialogueValue<std::int16_t>(game, 0x22e) = -1;
        camCtrlSetState(10);
        dialoguePlayerFocus = 0;
        for (slot = 0; slot < 6; ++slot) {
            if (game->characters[slot]) {
                EV_SetCharDisplay(game->characters[slot], CVM_DisplayEnabled, 1);
                cpamSetFlag(game->characters[slot], 1);
            }
        }
        return;
    }
    bank = DialogueBank(game);
    cameraIndex = bank[index].camera;
    if (cameraIndex) {
        game->stateFlags |= 1U;
        for (slot = 0; slot < 6; ++slot) {
            if (!game->characters[slot])
                continue;
            FightActorRuntimePrefix *actor = game->actors[slot];
            if ((actor->hitStateFlags & 0x4200U) != 0x200U || actor->motionTimer || actor->animationTime < 2.0f)
                return;
        }
        FightActorRuntimePrefix *const speaker = FindFightActorByName(game, dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].character);
        if (speaker) {
            if (dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].character == SHEN_NAME4('R', 'Y', 'O', '_'))
                dialoguePlayerFocus = 1;
            if (dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].expression != -1)
                SetHighModelAndFaceFlagsForChara(speaker->character, dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].expression, 15, 1.0f);
            if (dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].target) {
                FightActorRuntimePrefix *const target = FindFightActorByName(game, dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].target);
                if (target) {
                    GetCharacterWorldPosition(speaker->character, &origin);
                    GetCharacterWorldPosition(target->character, &destination);
                    origin.x = destination.x - origin.x;
                    origin.z = destination.z - origin.z;
                    const std::int32_t yaw = InverseFixedAtan2(origin.x, origin.z);
                    SetCharacterWorldYaw(speaker->character, yaw);
                }
            }
            if (speaker->moveFlags & 0x40U)
                camCtrlSetSplineShouldRebuildRotTo(1);
            request[0] = dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].camera;
            request[1] = reinterpret_cast<std::uint32_t>(speaker->character);
            request[2] = reinterpret_cast<std::uint32_t>(speaker->character);
            camCtrlToggleSplineRebuildFlag(3, request);
            camCtrlSetState(6);
            camCtrlSetFovTransitionTo(1);
            if (dialogueCameras[static_cast<std::uint16_t>(cameraIndex)].isolate) {
                for (slot = 0; slot < 6; ++slot) {
                    if (!game->characters[slot])
                        continue;
                    if (speaker != game->actors[slot]) {
                        EV_SetCharDisplay(game->characters[slot], CVM_DisplayEnabled, 0);
                    } else {
                        EV_SetCharDisplay(game->characters[slot], CVM_DisplayEnabled, 1);
                        cpamSetFlag(game->characters[slot], 0);
                    }
                }
            }
        }
    }
    if (bank[index].voice)
        EV_PlayDialogue(bank[index].voice);
    bank[index].played = 1;
    index = bank[index].next;
    DialogueValue<std::int16_t>(game, 0x22e) = index;
    if (static_cast<std::uint16_t>(index + 2) > 1)
        DialogueValue<std::uint16_t>(game, 0x230) = bank[index].delay;
    else
        DialogueValue<std::uint16_t>(game, 0x230) = 0;
}

#else

// The real dialogue clock, actor readiness gate and CAMT request owner.
// @verified - p
void vfmaSetupDialogueReactionsEtc(VFMA_t *game) {
    using enum CameraState;
    auto index = DialogueIndex(*game);
    if (game->mode & 4) {
        game->frameDelta = 0;
        return;
    }
    if (index == -1)
        return;
    if (game->frameDelta > 0) {
        game->frameDelta -= shenGetScaledDeltaTickTime(1.0f);
        if (game->frameDelta < 0)
            game->frameDelta = 0;
        return;
    }
    if (index == -2) {
        game->mode &= ~1U;
        SetDialogueIndex(*game, -1);
        camCtrlSetState(ChaseGoal);
        dialoguePlayerFocus = 0;
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            EV_SetCharDisplay(game->characters[i], CVM_DisplayEnabled, 1);
            auto &slot = *game->enemySlots[i];
            if (slot.savedControlFlags & 0x40000000) {
                cpamSetFlag(game->characters[i], 1);
                slot.savedControlFlags &= ~0x40000000U;
            }
        }
        return;
    }
    auto *const bank = DialogueBank(*game);
    auto &record = bank[index];
    if (record.camera) {
        game->mode |= 1;
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            auto &actor = *game->actors[i];
            if ((actor.hitStateFlags & (VFHSF_AttackGuardWindow | VFHSF_SpecialGuardOrParry)) != 0x200 || actor.motionTimer || actor.animationTime < 2)
                return;
        }
        const auto &camera = dialogueCameras[record.camera];
        if (auto *const speaker = vfGetActorFromSlot(game, camera.character)) {
            if (camera.character.name == SHEN_NAME(RYO_))
                dialoguePlayerFocus = 1;
            if (camera.expression != -1)
                SetHighModelAndFaceFlagsForChara(speaker->character, camera.expression, 15, 1);
            if (camera.target.name) {
                if (auto *const target = vfGetActorFromSlot(game, camera.target)) {
                    const auto origin = GetCharacterWorldPosition(speaker->character);
                    const auto destination = GetCharacterWorldPosition(target->character);
                    shenDecrementRotYAllNodes(
                        GetCharacterNode(speaker->character), static_cast<std::int16_t>(FixedAtan2(origin.x - destination.x, origin.z - destination.z)));
                }
            }
            if (speaker->moveFlags & 0x40)
                camCtrlSetSplineShouldRebuildRotTo(true);
            const std::uintptr_t request[3]{
                camera.camera, reinterpret_cast<std::uintptr_t>(speaker->character), reinterpret_cast<std::uintptr_t>(speaker->character)};
            camCtrlToggleSplineRebuildFlag(3, request);
            camCtrlSetState(Scripted);
            camCtrlSetFovTransitionTo(true);
            camCtrlDisableFilmBars();
            if (camera.isolate) {
                for (std::uint8_t i = 0; i < 6; ++i) {
                    if (!game->characters[i])
                        continue;
                    const bool isSpeaker = game->actors[i] == speaker;
                    EV_SetCharDisplay(game->characters[i], CVM_DisplayEnabled, isSpeaker);
                    if (isSpeaker) {
                        if (shenCheckCharaHasAlphaManager(game->characters[i]))
                            game->enemySlots[i]->savedControlFlags |= 0x40000000;
                        cpamSetFlag(game->characters[i], 0);
                    }
                }
            }
        }
    }
    if (record.voice)
        EV_PlayDialogue(record.voice);
    record.played = 1;
    index = record.next;
    SetDialogueIndex(*game, index);
    game->frameDelta = static_cast<std::uint16_t>(index + 2) <= 1 ? 0 : static_cast<float>(bank[index].delay);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

template <class T> T InstructionValue(const VFHD_t *owner, std::size_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(owner) + offset, sizeof(value));
    return value;
}

} // namespace

#else

namespace {

template <class T> T InstructionValue(const VFHD_t *owner, std::size_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(owner) + offset, sizeof(value));
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

template <class T> void InstructionStore(VFHD_t *owner, std::size_t offset, T value) {
    std::memcpy(reinterpret_cast<std::byte *>(owner) + offset, &value, sizeof(value));
}

} // namespace

#else

namespace {

template <class T> void InstructionStore(VFHD_t *owner, std::size_t offset, T value) {
    std::memcpy(reinterpret_cast<std::byte *>(owner) + offset, &value, sizeof(value));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int16_t DialogueIndex(const VFMA_t &game) {
    std::int16_t value;
    std::memcpy(&value, game.reserved318 + 2, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void SetDialogueIndex(VFMA_t &game, std::int16_t index) {
    std::memcpy(game.reserved318 + 2, &index, sizeof(index));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

template <class T> T &InstructionActorValue(void *owner, std::size_t offset) {
    return *reinterpret_cast<T *>(static_cast<std::byte *>(owner) + offset);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

Vector3 InstructionCharacterPosition(Character *character) {
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    return position;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

Vector3i InstructionCharacterRotation(Character *character) {
    Vector3i rotation;
    GetCharacterWorldRotation(character, &rotation);
    return rotation;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t InstructionCharacterYaw(CharacterNode *node) {
    Vector3i rotation;
    GetNodeWorldRotation(node, &rotation);
    return rotation.y;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void SetInstructionState(VFHD_t *owner, std::uint8_t state) {
    InstructionStore(owner, 0x438, state);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void SetInstructionPhase(VFHD_t *owner, std::uint8_t &phase, std::uint8_t state) {
    phase = state;
    InstructionStore(owner, 0x4b1, state);
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
