#pragma once

#include "s2/afs.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct ConversationBuffer;
struct DialogueVoice;
struct ScenarioEventRuntime;
struct ScenarioFreeTalkRecord;
struct ConversationResource {
    AfsArchiveSlot *archive;
    std::byte *index;
    std::array<char, 24> bankName;
};
static_assert(sizeof(ConversationResource) == 0x28);
static_assert(offsetof(ConversationResource, index) == 0x08 && offsetof(ConversationResource, bankName) == 0x10);

struct ScenarioFreeTalkControl {
    std::array<std::uint16_t, 3> resumeOffsets;
    std::uint16_t requestedRootOffset;
    std::uint32_t flags;
    std::uint8_t completedCount;
    std::uint8_t activationCount;
    std::byte reserved0e[2];
};



struct ScenarioTalkPresentationState {
    std::array<shen_name, 8> participants{};
    std::array<std::uint32_t, 8> queuedFaceLooks{};
    std::array<std::uint32_t, 8> faceLooks{};
    float voiceWaitDeadline{};
    float voiceWaitElapsed{};
    std::array<ScenarioFreeTalkControl, 850> characterStates{};
    std::int32_t participantCount{};
    std::int32_t queuedFaceLookCount{};
    std::int32_t suppressNpcFacing{};    // g_talk_flag_0
    std::int32_t suppressPlayerFacing{}; // g_talk_mode
    std::int32_t playerFacingMode{};
    std::int32_t npcFacingIssued{}; // g_talk_flag_4
    std::int32_t playerFacingIssued{};
    std::int32_t facingCommandsIssued{}; // g_talk_flag_5
    std::int32_t unfocusedFacingRequested{};
    std::array<std::int8_t, 8> participantFacing{};
    std::array<shen_name, 8> lookReleaseNames{};
    std::array<std::uint8_t, 8> facingApplied{};
    std::int32_t lastTurnAction{-1};
    float speakerHeadHeight{};
    float speakerChestHeight{};
    std::uint8_t letterVisible{};
    std::int32_t followRouteWait{};
    std::int32_t followRequest{};
    std::int32_t lookTargetActive{};
    std::int32_t selectedFollowFlag{};
    std::int32_t talkEndRequest{};

    // scenProcessCharacterActionQueue state. The participant copy is followed natively by ten
    // reset-only words, which actions 58, 59, 148 and 149 index; they read as unnamed participants.
    std::array<shen_name, 18> actionParticipants{};
    std::array<std::int32_t, 8> participantMotionIds{};
    std::array<std::int32_t, 8> participantWorldYaws{};
    std::array<std::uint32_t, 8> participantEntityYaws{};
    const Vector3 *lookLocation{};
    Vector3 speakerPosition{};
    Vector3 playerPosition{};
    std::array<std::uint8_t, 10> turnPending{};
    std::int8_t lookTargetReleased{}; // g_talk_flag
    std::int8_t lookCameraState{};    // g_talk_flag_1
    shen_name turnSource{};
    shen_name turnTarget{};
    std::array<std::uint8_t, 8> lookReleasePending{};
    std::array<std::uint8_t, 10> participantTurnIssued{};
    std::uint8_t lookReleaseFrames{};
    std::uint8_t lookLocationFrames{};
    shen_name lookReleaseCharacter{};
    shen_name playerTurnTarget{};
    std::array<shen_name, 10> turnTargets{};
    std::array<shen_name, 10> turnSources{};
    shen_name actionCharacter{};
    shen_name previousPlayerTurnTarget{};
    std::uint8_t ryoAcknowledged{};
    std::uint8_t unused0d{};
    std::uint8_t lookActive{}; // g_talk_flag_2
    std::uint8_t lookAcknowledged{};
    std::uint8_t lookReleaseRequested{}; // g_talk_flag_3
    std::uint8_t lookLocationPending{};
    std::uint8_t playerTurnPending{};
    std::uint8_t letterShowPending{};
    std::uint8_t letterHidePending{};
    std::uint8_t faceForwardPending{};
    std::uint8_t posesCaptured{};
    std::uint8_t letterFrames{};
    std::uint8_t playerTurnFrames{};
    std::int8_t playerTurnFrameLimit{-1};
    std::int32_t playerTurnAround{};
};
static_assert(offsetof(ScenarioTalkPresentationState, queuedFaceLooks) == 0x20);
static_assert(offsetof(ScenarioTalkPresentationState, faceLooks) == 0x40);
static_assert(offsetof(ScenarioTalkPresentationState, voiceWaitDeadline) == 0x60);
static_assert(offsetof(ScenarioTalkPresentationState, voiceWaitElapsed) == 0x64);
static_assert(offsetof(ScenarioTalkPresentationState, characterStates) == 0x68);
static_assert(offsetof(ScenarioTalkPresentationState, lookReleaseNames) == offsetof(ScenarioTalkPresentationState, participantFacing) + 8);
static_assert(offsetof(ScenarioTalkPresentationState, lastTurnAction) == offsetof(ScenarioTalkPresentationState, facingApplied) + 8);

ScenarioTalkPresentationState &GetScenarioTalkPresentationState() noexcept;

struct ScenarioAction {
    shen_name name;
    std::int32_t action;
};
static_assert(sizeof(ScenarioAction) == 8);

// AllocateMem's variable-record FIFOs; SCEN owns both inside its task data (+0x1a0, +0x228).
struct ScenarioActionQueue {
    shen_name name;
    std::uint32_t count;
    std::byte *read;
    std::byte *write;
    std::array<std::byte, 0x70> storage;
};
static_assert(sizeof(ScenarioActionQueue) == 0x88);
static_assert(offsetof(ScenarioActionQueue, storage) == 0x18);
// The two SCEN FIFOs, published by ScenarioTaskEntry_Enqueue.
extern ScenarioActionQueue *scenarioActionQueueStorage;
void InitializeScenarioConversation();
void QueueScenarioCharacterAction(shen_name name, std::int32_t action);
std::int32_t ReadScenarioCharacterAction(ScenarioAction &action);
void AcknowledgeScenarioCharacterAction(shen_name name, std::int32_t action);
void QueueScenarioRecordAction(ScenarioFreeTalkRecord &record, std::int32_t action);
std::int32_t ReadScenarioAcknowledgement(ScenarioAction &action);
std::int32_t DrainScenarioAcknowledgements();
inline constexpr std::array<std::int16_t, 3> SCEN_DEFAULT_FACE_SEQUENCE{0, 4, -1};

#pragma pack(push, 4)
struct ScenarioTextDecoder {
    const std::uint8_t *cursor;
    std::uint32_t legacyEncodingMode;
    std::uint32_t reserved0c;
    bool (*isLeadByte)(std::uint8_t);
    bool (*isTrailByte)(std::uint8_t);
    std::uint16_t pendingPrefix;
    std::uint16_t reserved22;
    std::uint32_t currentCharacterWidth;
    std::uint16_t currentCharacter;
    std::byte reserved2a[6];
};

struct ScenarioWindowTextState {
    std::int32_t columnCount;
    std::int32_t rowCount;
    ScenarioTextDecoder decoder;
    std::uint32_t scenarioStateFlags;
    std::int32_t surfaceRecoveryFrames;
    std::int32_t surfaceReady;
    std::int32_t callbackEnabled;
    std::int32_t lastWindowEvent;
};

struct SCENDialogueCue {
    float triggerTime;
    std::uint32_t field04;
    const std::int32_t *payload;
    shen_name speaker;
    std::uint32_t reserved14;
};

inline constexpr std::array<std::int8_t, 16> CONVERSATION_CAMERA_REVERB_ATTENUATION{
    -45, -43, -41, -39, -37, -35, -33, -31, -29, -27, -25, -23, -21, -19, -17, -15};
struct ConversationStream {
    DialogueVoice *handle;
    void *compressedBuffer;
    void *decodeBuffer;
    std::int32_t volume;
    std::uint32_t reserved1c;
    std::uint32_t frameScale;
    std::int32_t cameraCode;
    std::uint32_t paused;
    std::uint64_t field2c;
    std::int32_t baseAttenuation;
    std::uint32_t pauseCategory;
    std::int32_t remainingFrames;
};

struct ConversationPacketBuffer {
    std::array<std::byte, 0x800> data;
    std::array<char, 24> bankName;
    std::uint32_t entry;
    std::int32_t sector;
    std::uint32_t pending;
};
#pragma pack(pop)

using ConversationCallback = void (*)(ConversationBuffer &);

struct ConversationBuffer {
    ConversationPacketBuffer *packet; // +0x00, separately allocated CVBF
    ConversationCallback callback;    // +0x08, validity predicate
    std::int32_t packetOffset;        // +0x10, signed busy predicate
    std::uint32_t reserved14;
    ConversationResource *resource;      // +0x18, borrowed CVRS slot
    ConversationStream stream;
    const char *bankName;
    const char *sourceCode;              // +0x68, borrowed request string
    d3t::LargeFileHandle *packetRequest; // +0x70, pending archive child
    std::int32_t priority;
    std::int16_t volume;
    std::uint8_t pauseEnabled;
    std::uint8_t pauseOwned;
    std::int32_t output;
    shen_name speaker;
    std::int32_t fadeVolume;
    std::int32_t fadeStep;
    std::int32_t duckVolume;
    std::int32_t duckState;
    std::uint64_t subtitles;
    std::uint32_t playFlags;
    std::array<char, 12> code;
};

struct ConversationTaskData {

    std::array<ConversationBuffer, 4> buffers;
};

static_assert(sizeof(ScenarioTextDecoder) == 0x30);
static_assert(sizeof(ScenarioWindowTextState) == 0x4c);
static_assert(offsetof(ScenarioWindowTextState, scenarioStateFlags) == 0x38);
static_assert(sizeof(SCENDialogueCue) == 0x18);
static_assert(offsetof(SCENDialogueCue, payload) == 0x08);
static_assert(sizeof(ConversationStream) == 0x40);
static_assert(offsetof(ConversationStream, paused) == 0x28);
static_assert(offsetof(ConversationStream, baseAttenuation) == 0x34);
static_assert(offsetof(ConversationStream, cameraCode) == 0x24);
static_assert(offsetof(ConversationStream, remainingFrames) == 0x3c);
static_assert(sizeof(ConversationPacketBuffer) == 0x824);
static_assert(offsetof(ConversationPacketBuffer, sector) == 0x81c);
static_assert(sizeof(ConversationBuffer) == 0xb0);
static_assert(offsetof(ConversationBuffer, callback) == 0x08);
static_assert(offsetof(ConversationBuffer, packetOffset) == 0x10);
static_assert(offsetof(ConversationBuffer, stream) == 0x20);
static_assert(offsetof(ConversationBuffer, priority) == 0x78);
static_assert(offsetof(ConversationBuffer, speaker) == 0x84);
static_assert(offsetof(ConversationBuffer, subtitles) == 0x98);
static_assert(offsetof(ConversationBuffer, code) == 0xa4);
static_assert(sizeof(ConversationTaskData) == 0x2c0);

inline constexpr shen_name TASK_CONVERSATION{SHEN_NAME(VOIC)}, CONVERSATION_PACKET_MEMORY{SHEN_NAME(CVBF)}, CONVERSATION_RESOURCE_MEMORY{SHEN_NAME(CVRS)};
inline constexpr shen_name CONVERSATION_INDEX_MEMORY{SHEN_NAME(CVIX)}, CONVERSATION_ADFS_MEMORY{SHEN_NAME(ADFS)};

// AllocateAudioIndexData owns the persistent BGM CVIX and separate ADFS.
// Cold-scene admission precedes the HLib heap partition; MODU only retires borrowers.
void AllocateAudioIndexData();
bool adfsSetCameraIdxForChara(shen_name target, shen_name source);
bool RemoveConversationSpeakerAlias(shen_name target);


void CVS_CtrlConversTask_Enqueue(std::int32_t loadSceneBanks);
void CVS_CtrlConversTask();
void UpdateConversationStreams();
void ReleaseConversationResources();
void SetConversationAudioLanguage(std::int32_t language);
std::uint64_t GetConversationAudioLanguage() noexcept;

bool cvsGet() noexcept;
bool cvsHasValidConverseData(const ConversationBuffer *buffer = nullptr) noexcept;
shen_name cvsFindVoiceDataFor(const ConversationBuffer *buffer) noexcept;
ConversationBuffer *GetConverseDataForChara(shen_name speaker) noexcept;
ConversationBuffer *FindConversationByPriority(std::uint32_t mask) noexcept;

bool EV_FreeAuthorPak(std::int32_t operation, const char *bankName);
ConversationBuffer *shenPlayAudio(const char *voiceCode, shen_name speaker, std::uint32_t playFlags, bool showSubtitles, std::int32_t *sceneRemap = nullptr);
ConversationBuffer *shenPlayAudioWithFlags(const char *voiceCode, std::uint32_t playFlags);
ConversationBuffer *EV_PlayDialogue(const char *voiceCode);
std::uint64_t IsScenarioDialoguePlaying(ConversationBuffer *buffer);
std::uint64_t GetConversationPaused(const ConversationBuffer *buffer = nullptr) noexcept;
float GetConversationPlaybackSeconds(const ConversationBuffer *buffer) noexcept;
void SetScenarioDialoguePitch(ConversationBuffer *buffer, std::int32_t pitch);
void scenCvsCtrlFree(ConversationBuffer *buffer = nullptr);
void hsFadeVoice(ConversationBuffer *buffer, std::int32_t frames);
void SetConversationVolume(ConversationBuffer *buffer, std::int32_t volume);
const char *GetCurrentAfsName() noexcept;
void PauseConversation(ConversationBuffer *buffer, std::int32_t paused);
void PauseConversationCategories(bool paused);
std::uint8_t PauseOrDestroyAudioAndSubtitles(std::int32_t paused);
void SetConversationCameraCodesEnabled(std::int32_t enabled) noexcept;

std::uint64_t SCEN_ShowEventMessage(HLib::Task *scenarioTask, std::int64_t window, const char *message);
bool shenPrepareDialogue(HLib::Task *scenarioTask, std::int64_t window, const char *message, std::int32_t speakerMode);
void scenDestroySubtitles(HLib::Task *scenarioTask);
void ForceScenarioSubtitles(HLib::Task *scenarioTask);

} // namespace shenmue::s2
