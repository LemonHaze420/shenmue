#pragma once

#include "s2/author_control_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/camera_spline_dc.hpp"

#define SHENMUE_AUTH_CONSTANT static const
#define SHENMUE_AUTH_JOIN_(a, b) a##b
#define SHENMUE_AUTH_JOIN(a, b) SHENMUE_AUTH_JOIN_(a, b)
#define SHENMUE_AUTH_CHECK(condition) typedef char SHENMUE_AUTH_JOIN(LayoutCheckauthor_controller_dc, __LINE__)[(condition) ? 1 : -1]

namespace shenmue {
namespace s2 {

struct Character;
struct AuthActorContext;
typedef std::int32_t (*AuthCommandHandler)(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *context);
struct AuthActiveCommand {
    std::int32_t channel;
    AuthCommandHandler handler;
    const std::int32_t *arguments;
    AuthActorContext *context;
};
struct AuthSplineSet {
    std::int32_t frame;
    CameraSplineTrack tracks[9];
};
struct AuthActorContext {
    shen_name subject;
    std::uint16_t flags;
    std::uint8_t voiceCountdown, setupCountdown;
    std::int16_t motionRemaining;
    std::byte retained0a[0x0e];
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    std::uint32_t clipState;
    AuthSplineSet splines;
    std::int16_t generatedMouthClip[148];
    std::byte retained268[0x130];
    float motionSpeed;
    std::uint32_t motionType;
};
struct AuthorController {
    HLib::Task *task;
    std::int8_t loaded, restartTrack, needsInitAuthorMask, track;
    const std::byte *data, *sequence, *camera, *motion, *strings, *lips;
    const std::byte *currentInstruction, *firstInstruction;
    std::uint32_t retained28;
    std::int32_t currentFrame;
    std::uint32_t framesRemaining;
    std::int32_t authorStopped;
    std::uint32_t skipToNext, skipCountdownFinished, finished;
    std::int32_t dialogueFrames;
    Character *dialogueCharacter;
    std::uint32_t dialogueState;
    std::int32_t audioCountdown[17];
    AuthActiveCommand activeCommands[70];
    std::byte retained4f4[0x10];
    AuthActorContext actorContexts[35];
    std::byte retained83e4[0x3a0];
    std::int32_t retained8784;
    std::uint16_t retained8788, retained878a;
};
struct AuthSubcontrollerSlot {
    HLib::Task *task;
    shen_name subject;
};
SHENMUE_AUTH_CHECK(sizeof(AuthorController) == 0x878c);
SHENMUE_AUTH_CHECK(__alignof__(AuthorController) == 4);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, loaded) == kAuthorLoadedFlagOffset);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, track) == 7);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, data) == 8);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, currentInstruction) == 0x20);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, currentFrame) == 0x2c);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, framesRemaining) == 0x30);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, authorStopped) == 0x34);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, skipToNext) == 0x38);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, skipCountdownFinished) == 0x3c);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, finished) == 0x40);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, dialogueFrames) == 0x44);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, dialogueCharacter) == 0x48);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, dialogueState) == 0x4c);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, audioCountdown) == 0x50);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, activeCommands) == 0x94);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, retained4f4) == 0x4f4);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, actorContexts) == 0x504);
SHENMUE_AUTH_CHECK(offsetof(AuthorController, retained83e4) == 0x83e4);
SHENMUE_AUTH_CHECK(sizeof(AuthActiveCommand) == 0x10);
SHENMUE_AUTH_CHECK(offsetof(AuthActiveCommand, handler) == 4);
SHENMUE_AUTH_CHECK(offsetof(AuthActiveCommand, arguments) == 8);
SHENMUE_AUTH_CHECK(offsetof(AuthActiveCommand, context) == 0x0c);
SHENMUE_AUTH_CHECK(sizeof(AuthActorContext) == 0x3a0);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, flags) == 4);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, motionRemaining) == 8);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, position) == 0x18);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, rotation) == 0x24);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, clipState) == 0x3c);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, splines) == 0x40);
SHENMUE_AUTH_CHECK(offsetof(AuthSplineSet, tracks) == 4);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, generatedMouthClip) == 0x140);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, motionSpeed) == 0x398);
SHENMUE_AUTH_CHECK(offsetof(AuthActorContext, motionType) == 0x39c);
SHENMUE_AUTH_CHECK(sizeof(AuthSubcontrollerSlot) == 8);
SHENMUE_AUTH_CHECK(offsetof(AuthSubcontrollerSlot, subject) == 4);
#define AUTHOR_CONTROL_STORAGE (::shenmue::shen_name(SHEN_NAME4('S', 'C', 'N', 'C')))
#define AUTH_ACAM SHEN_NAME4('A', 'C', 'A', 'M')
#define AUTH_ALIP SHEN_NAME4('A', 'L', 'I', 'P')
#define AUTH_AMOV SHEN_NAME4('A', 'M', 'O', 'V')
#define AUTH_ASEQ SHEN_NAME4('A', 'S', 'E', 'Q')
#define AUTH_ASTR SHEN_NAME4('A', 'S', 'T', 'R')
#define AUTH_AUVW SHEN_NAME4('A', 'U', 'V', 'W')
#define AUTH_HADT SHEN_NAME4('H', 'A', 'D', 'T')
#define AUTH_HAID SHEN_NAME4('H', 'A', 'I', 'D')
#define AUTH_RECV SHEN_NAME4('R', 'E', 'C', 'V')
#define AUTH_SCEY SHEN_NAME4('S', 'C', 'E', 'Y')
#define AUTH_SCPT SHEN_NAME4('S', 'C', 'P', 'T')
#define AUTH_SPLI SHEN_NAME4('S', 'P', 'L', 'I')
#define AUTH_TRCK SHEN_NAME4('T', 'R', 'C', 'K')

extern std::int32_t g_authorSkipAllowed, g_authorUnusedFlag, g_authorCannotSkip;
extern std::int32_t g_authorForceFilmBars, g_authorReservedInitializationFlag, g_authorHighQualityHands;
extern std::int32_t g_authorReservedHandle, g_authorReservedCounter, g_authorHandAdmissionCount;
extern shen_name g_authorHandAdmissionNames[32];
extern const void *g_authorCameraPlacement;
extern HLib::Task *g_authorMaskTask;
extern AuthSubcontrollerSlot g_authorEyesControllers[32], g_authorPartsControllers[32];
extern Vector4 g_authorReflectorValues;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct AuthDynamicCommand {
    std::int32_t frame;
    std::int16_t operation, length;
};
SHENMUE_AUTH_CHECK(sizeof(AuthDynamicCommand) == 8);
SHENMUE_AUTH_CHECK(offsetof(AuthDynamicCommand, operation) == 4);
SHENMUE_AUTH_CHECK(offsetof(AuthDynamicCommand, length) == 6);
struct AuthEyeTaskData {
    std::int32_t state;
    const AuthDynamicCommand *command;
};
SHENMUE_AUTH_CHECK(sizeof(AuthEyeTaskData) == 8);
SHENMUE_AUTH_CHECK(offsetof(AuthEyeTaskData, command) == 4);
void AuthorEyes_Callback(void *data);
SHENMUE_AUTH_CONSTANT std::uint16_t kAuthorAttachmentBones[7] = {1, 5, 3, 21, 14, 31, 26};
SHENMUE_AUTH_CONSTANT std::uint8_t kAuthorFilmColours[4][8] = {
    {0, 0, 0, 0, 255, 0, 0, 0}, {255, 0, 0, 0, 0, 0, 0, 0}, {0, 255, 255, 255, 255, 255, 255, 255}, {255, 255, 255, 255, 0, 255, 255, 255}};
extern const AuthDynamicCommand *g_authorDynamicCommands[26];
extern const AuthDynamicCommand *g_authorDynamicCommandHeads[26];
extern std::byte *g_authorHandCommands;
extern const std::byte *g_authorHandIndices;
SHENMUE_AUTH_CONSTANT std::uint32_t kAuthorDynamicTags[26] = {SHEN_NAME(STGC),
    SHEN_NAME(LGHC),
        SHEN_NAME(CHDS),
            SHEN_NAME(RFBD),
                SHEN_NAME(EYES),
                    SHEN_NAME(HAND),
                        SHEN_NAME(CLTH),
                            SHEN_NAME(OSGC),
                                SHEN_NAME(BLUR),
                                    SHEN_NAME(WAVE),
                                        SHEN_NAME(FACX),
                                            SHEN_NAME(FADE),
                                                SHEN_NAME(MBTK),
                                                    SHEN_NAME(SMKC),
                                                        SHEN_NAME(HAPA),
                                                            SHEN_NAME(PRTS),
                                                                SHEN_NAME(AFOG),
                                                                    SHEN_NAME(FOBJ),
                                                                        SHEN_NAME(SHDW),
                                                                            SHEN_NAME(FCLP),
                                                                                SHEN_NAME(FILT),
                                                                                    SHEN_NAME(LENS),
                                                                                        SHEN_NAME(DOOR), SHEN_NAME(LGHS), SHEN_NAME(WRST), SHEN_NAME(FPLU)};
struct AuthorTableEntry;
void InitializeAuthorDynamicData(const AuthorTableEntry *entry);
typedef const AuthDynamicCommand *(*AuthDynamicCommandHandler)(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicStageControl(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicLightingControl(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicCharacterDisplay(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicReflectorBoard(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicEyes(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicHand(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicCloth(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicOsageControlNullsub(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicBlur(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicWaveNullsub(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicFaceExpression(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicFade(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicMotionBlendTrack(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicSmoke(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicFallingParticles(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicParticles(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicFog(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicFieldObject(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicShadow(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicFaceClip(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicFilter(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicLens(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicDoor(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicLightState(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicWrist(const AuthDynamicCommand *command);
const AuthDynamicCommand *AuthorDynamicPartsLight(const AuthDynamicCommand *command);
SHENMUE_AUTH_CONSTANT AuthDynamicCommandHandler kAuthorDynamicHandlers[26] = {AuthorDynamicStageControl, AuthorDynamicLightingControl,
    AuthorDynamicCharacterDisplay, AuthorDynamicReflectorBoard, AuthorDynamicEyes, AuthorDynamicHand, AuthorDynamicCloth, AuthorDynamicOsageControlNullsub,
    AuthorDynamicBlur, AuthorDynamicWaveNullsub, AuthorDynamicFaceExpression, AuthorDynamicFade, AuthorDynamicMotionBlendTrack, AuthorDynamicSmoke,
    AuthorDynamicFallingParticles, AuthorDynamicParticles, AuthorDynamicFog, AuthorDynamicFieldObject, AuthorDynamicShadow, AuthorDynamicFaceClip,
    AuthorDynamicFilter, AuthorDynamicLens, AuthorDynamicDoor, AuthorDynamicLightState, AuthorDynamicWrist, AuthorDynamicPartsLight};
void Author_UpdateDynamic();

const AuthDynamicCommand *FindLastAuthorDynamicCommand(std::int32_t channel);
void FinalizeAuthorSkip();
void ProcessScenario(AuthorController *control);
#endif

AuthorController *AllocateScenarioControl(HLib::Task *task);
void RequestAuthorTrackRestart();
void RequestAuthorMaskCleanup();
std::int32_t Author_GetCurrentFrame();
std::uint32_t Author_GetFramesRemaining();
std::int32_t Author_GetTrack();
std::uint32_t authGetFinished();
void ResetAuthorDialogueState();
void SetAuthorCameraPlacement(const void *placement);
void SetAuthorFilmBars(std::int32_t enabled);
std::int32_t GetAuthorFilmBars();

} // namespace s2
} // namespace shenmue

#undef SHENMUE_AUTH_CONSTANT
#undef SHENMUE_AUTH_JOIN_
#undef SHENMUE_AUTH_JOIN
#undef SHENMUE_AUTH_CHECK

#endif
