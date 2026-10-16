#pragma once

#include <cstdint>
#include <array>
#include <cstddef>
#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/camera.hpp"

namespace shenmue::s2 {
namespace HLib {
struct Task;
}
struct Character;
inline constexpr std::size_t AUTHOR_HAND_CHARACTER_CAPACITY = 32;
inline constexpr shen_name AUTHOR_SPLINE_ALLOCATION{SHEN_NAME(SPLI)};
inline constexpr std::size_t AUTHOR_DIALOGUE_CODE_CAPACITY = 40;
inline constexpr std::size_t AUTHOR_MOUTH_CLIP_CAPACITY = 147;
struct AuthorDialogueStat {
    const char *code;
    std::int32_t stat;
};
inline constexpr std::array<AuthorDialogueStat, 12> authorDialogueStats{{
    {.code = "0014B002", .stat = 0},
    {.code = "0031B007", .stat = 1},
    {.code = "0037A012", .stat = 2},
    {.code = "0068C028", .stat = 15},
    {.code = "0063A001", .stat = 3},
    {.code = "0236C007", .stat = 5},
    {.code = "0248A001", .stat = 6},
    {.code = "0264A002", .stat = 7},
    {.code = "0275A013", .stat = 8},
    {.code = "0527B137", .stat = 17},
    {.code = "0510B013", .stat = 9},
    {.code = "0516A008", .stat = 10},
}};
struct AuthTransformPose {
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
};
static_assert(sizeof(AuthTransformPose) == 0x24);
#pragma pack(push, 4)
struct AuthActorContext {
    shen_name subject;
    std::uint16_t flags;
    std::uint8_t voiceCountdown;
    std::uint8_t setupCountdown;
    float motionRemaining;
    std::byte reserved0c[12];
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    std::int32_t transformFlags;
    float splineFrame;
    std::uint32_t splineFlags;
    std::array<CameraSplineTrack, 9> splineTracks;
    std::array<std::int16_t, AUTHOR_MOUTH_CLIP_CAPACITY> generatedMouthClip;
    std::byte reservedMouthTail[0x132];
    float motionSpeed;
    std::uint32_t motionType;
};
#pragma pack(pop)
static_assert(sizeof(AuthActorContext) == 0x458);
static_assert(offsetof(AuthActorContext, splineTracks) == 0x48);
static_assert(offsetof(AuthActorContext, splineTracks) + sizeof(CameraSplineTrack) * 6 == 0x168);
static_assert(offsetof(AuthActorContext, generatedMouthClip) == 0x1f8);
static_assert(offsetof(AuthActorContext, motionSpeed) == 0x450);


#pragma pack(push, 4)
struct AuthCharacterState {
    shen_name subject{};
    std::uint32_t noMotion{};
    std::uint32_t flags{};
    std::byte reserved0c[4]{};
    const std::int32_t *finalMotionArguments{};
    std::int32_t finalMotionFrame{};
    Vector4 reflector{};
    std::uint8_t reflectorMode{};
    std::uint8_t characterFlag40{};
    std::int8_t motionSnapshot{-1};
    std::int8_t entityType{};
    std::int32_t drawControlEnabled{};
    std::uint32_t controlFlags{};
    std::uint16_t pluralControl{};
    std::uint8_t displayEnabled{};
    std::byte reserved3b[5]{};
};
#pragma pack(pop)
static_assert(sizeof(AuthCharacterState) == 0x40);
static_assert(offsetof(AuthCharacterState, finalMotionArguments) == 0x10);
static_assert(offsetof(AuthCharacterState, finalMotionFrame) == 0x18);
static_assert(offsetof(AuthCharacterState, reflector) == 0x1c);
static_assert(offsetof(AuthCharacterState, characterFlag40) == 0x2d);
static_assert(offsetof(AuthCharacterState, reflectorMode) == 0x2c);
static_assert(offsetof(AuthCharacterState, motionSnapshot) == 0x2e);
static_assert(offsetof(AuthCharacterState, entityType) == 0x2f);
static_assert(offsetof(AuthCharacterState, drawControlEnabled) == 0x30);
static_assert(offsetof(AuthCharacterState, controlFlags) == 0x34);
static_assert(offsetof(AuthCharacterState, pluralControl) == 0x38);
static_assert(offsetof(AuthCharacterState, displayEnabled) == 0x3a);

using AuthCommandHandler = bool (*)(std::int32_t phase, const std::int32_t *arguments, AuthActorContext &context);

struct AuthActiveCommand {
    std::int64_t channel{};
    AuthCommandHandler handler{};
    const std::int32_t *arguments{};
    AuthActorContext *context{};
};
static_assert(sizeof(AuthActiveCommand) == 0x20);
static_assert(offsetof(AuthActiveCommand, arguments) == 0x10);

struct AuthorController {
    HLib::Task *task;
    std::uint8_t loaded;
    std::uint8_t flag2;
    std::uint8_t needsInitAuthorMask;
    std::int8_t track;
    std::byte reserved00c[0x2c];
    const std::byte *lipTable;
    std::byte reserved040[0x18];
    std::int32_t currentFrame;
    std::uint32_t framesRemaining;
    std::int32_t authorStopped;
    float skipTimer;
    std::uint32_t skipToNext;
    std::uint32_t field06c;
    std::uint32_t finished;
    std::int32_t dialogueFrames;
    std::byte reserved078[8];
    Character *dialogueCharacter;
    std::array<std::int32_t, 16> audioCountdown;
    std::byte reserved0c8[8];
    std::array<AuthActiveCommand, 70> activeCommands;
    std::byte reserved990[0x20];
    std::array<AuthActorContext, 35> actorContexts;
    std::byte reserveda1b8[0x45c];
    std::uint16_t currentInput;
    std::byte reserveda616[2];
    std::uint64_t postUpdateContext;
    std::int32_t sequenceCount{};
    std::int32_t remainingFrames;
};
static_assert(offsetof(AuthorController, lipTable) == 0x38);
static_assert(offsetof(AuthorController, dialogueFrames) == 0x74);
static_assert(offsetof(AuthorController, dialogueCharacter) == 0x80);
static_assert(offsetof(AuthorController, audioCountdown) == 0x88);
static_assert(offsetof(AuthorController, activeCommands) == 0xd0);
static_assert(offsetof(AuthorController, actorContexts) == 0x9b0);
static_assert(offsetof(AuthorController, currentInput) == 0xa614);
static_assert(offsetof(AuthorController, sequenceCount) == 0xa620);
static_assert(offsetof(AuthorController, remainingFrames) == 0xa624);
static_assert(sizeof(AuthorController) == 0xa628);

enum class AuthorPackageLoadResult : std::uint8_t {
    Started,
    PackageMissing,
    AuthorBundleMissing,
    AuthorIdMissing,
    MotionMissing,
    ModelMissing,
    InvalidAsset,
    RuntimeUnavailable,
};
enum class AuthorMapInfoLoadResult : std::uint8_t {
    Started,
    MapInfoMissing,
    SceneExtensionMissing,
    InvalidTrack,
    MotionMissing,
    ModelMissing,
    RuntimeUnavailable,
};

enum class AuthSceneCtrl : std::int64_t {
    GetCurrentAuthor = -18,
    ApplyYangleOffset = -17,
    UnusedAuthorFlag = -16,
    SetCharaHQFlags = -14,
    GetCantSkipAuthor = -13,
    GetSkipAuthorEnabled = -12,
    DisallowSkipAuthor = -11,
    AllowSkipAuthor = -10,
    GetFilmBarsActive = -9,
    HideFilmBars = -8,
    ShowFilmBars = -7,
    GetCurrentFrame = -6,
    GetFramesRemaining = -5,
    RefreshAndEnqueue = -4,
    ResetAuthorStopped = -3,
    SetAuthorStopped = -2,
    IsLoaded = -1,
    AU01 = SHEN_NAME(AU01),
    AU02 = SHEN_NAME(AU02),
    AU03 = SHEN_NAME(AU03),
    AU04 = SHEN_NAME(AU04),
    AU05 = SHEN_NAME(AU05),
};

struct MapInfoChunk;



struct AuthorTableEntry {
    std::byte *file;
    const std::byte *track; // +0x08, TRCK chunk
    const std::byte *view;  // +0x10, the AUVW chunk that follows it, if any
    std::int32_t fileOwned;
    shen_name name;         // +0x1c, zero for unnamed TRCK lists
};
static_assert(sizeof(AuthorTableEntry) == 0x20);
static_assert(offsetof(AuthorTableEntry, fileOwned) == 0x18);
static_assert(offsetof(AuthorTableEntry, name) == 0x1c);


struct AuthorPackageRecord {
    const void *package;
    std::uint8_t count;
    std::array<std::uint8_t, 31> slots;
};
static_assert(sizeof(AuthorPackageRecord) == 0x28);



struct AuthorPackageRegistration {
    const void *package;
    std::uint8_t count;
    std::array<std::uint8_t, 70> slots;
};

void authInit();
void ResetAuthorAudioState();
bool IsAuthorMultichannelAudioEnabled();
std::uint64_t GetAuthorStreamingAudioEnabled();
bool SetAuthorStreamingAudioEnabled(std::int32_t enabled);
inline constexpr shen_name AUTHOR_MULTICHANNEL_ALLOCATION{SHEN_NAME(AIXP)};
bool InitializeAuthorMultichannelAudio();
void ReleaseAuthorMultichannelAudio();


std::uint64_t &GetAuthorSilentStreamCounter();
bool IsAuthorMultichannelAudioActive();
void StartAuthorMultichannelAudio(std::int32_t delay);
void AdvanceAuthorMultichannelAudio();
void PauseAuthorMultichannelAudio(std::uint32_t value);
void UpdateAuthorMultichannelDevice();
inline constexpr std::size_t AUTHOR_STATE_FLAG_CAPACITY = 46;
bool IsAuthorStateFlagSet(std::int32_t index);
bool HasAuthorCancelBeenPressed();
std::int64_t SetAuthorStateFlag(std::int32_t index, std::int32_t value);
void ResetAuthorStateFlags();


void SCEX_Reader(const MapInfoChunk *scex);

std::int32_t ReadAUTH(const std::byte *bundle, const void *package);
std::int32_t FreeAUTH(const void *package);
bool FindAuthor(shen_name id);
// The author slot registered under `id`, searched from slot 69 down, or -1.
std::int32_t probeSceneById(std::int32_t id);

void authResetLoadFlag();

std::int64_t EV_authSceneCtrl(AuthSceneCtrl command);
std::int64_t authGetFinished();
shen_name GetCurrentAuthorId();
std::uint64_t GetAuthorFlags();
void EV_SetAuthorActiveTo(std::int32_t active);
bool Author_IsSkipActive();

bool Author_InitializeFromMapInfo(const void *mapInfo);
bool Author_HasMapInfoAuthor(const void *mapInfo, shen_name authorId);
bool Author_InitializeFromMapInfoAuthor(const void *mapInfo, shen_name authorId);
AuthorMapInfoLoadResult Author_GetLastMapInfoLoadResult();
const char *Author_GetLastMapInfoLoadGate();
AuthorPackageLoadResult Author_InitializeFromPackage(const char *packageName, shen_name authorId);
AuthorPackageLoadResult Author_GetLastPackageLoadResult();
const char *Author_GetLastPackageLoadGate();
void Author_Free();
void Author_BindController(AuthorController *controller);
const AuthorController *Author_GetController();
const Matrix4x4 *Author_GetPlacementMatrix();
void Author_SetPlacementMatrix(const Matrix4x4 *matrix);
void Author_Update();
void Author_UpdateDynamic();
void Author_RequestAbort();
int Author_GetCurrentFrame();
std::int32_t authGetLoaded();
bool Author_GetActorTransform(shen_name subject, Vector3 &position, Vector3 &rotation);
bool Author_SetActorPosition(shen_name subject, const Vector3 &position);

} // namespace shenmue::s2
