#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "s2/stream_audio_dc.hpp"

#define SHENMUE_AUDIO_NULL 0
#define SHENMUE_AUDIO_NOEXCEPT

namespace shenmue {
namespace s2 {

void ScenarioSoundNullsub();
std::uint32_t ConversationResultNullsub();
void ConversationNullsubA();
void ConversationNullsubB();
std::uint32_t ConversationStateNullsub();
std::uint32_t ScenarioConversationResultNullsub();

struct ConversationBuffer;
ConversationBuffer *EV_PlayDialogue(const char *code);
typedef void (*ConversationCallback)(ConversationBuffer &);
typedef SceneAudioBank ConversationResource;
struct ConversationPacketBuffer {
    std::byte data[0x800];
    char bankName[24];
    std::uint32_t entry;
    std::int32_t sector;
    std::uint32_t pending;
};
struct ConversationStream {
    void *handle;
    void *compressedBuffer;
    std::int32_t volume;
    std::int32_t baseAttenuation;
    std::uint32_t frameScale;
    std::int32_t cameraCode;
    std::int32_t paused;
    std::int32_t remainingFrames;
};
struct ConversationBuffer {
    ConversationPacketBuffer *packet;
    ConversationCallback callback;
    std::int32_t packetOffset;
    ConversationResource *resource;
    ConversationStream stream;
    void *packetRequest;
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
};
struct ConversationTaskData {
    ConversationBuffer buffers[2];
};
struct ConversationSpeakerAlias {
    shen_name source;
    shen_name target;
    std::uint32_t flags;
};
typedef char sizeofConversationPacketBufferCheck[sizeof(ConversationPacketBuffer) == 0x824 ? 1 : -1];
typedef char offsetofConversationPacketBuffersectorCheck[offsetof(ConversationPacketBuffer, sector) == 0x81c ? 1 : -1];
typedef char sizeofConversationStreamCheck[sizeof(ConversationStream) == 0x20 ? 1 : -1];
typedef char offsetofConversationStreamvolumeCheck[offsetof(ConversationStream, volume) == 8 ? 1 : -1];
typedef char sizeofConversationBufferCheck[sizeof(ConversationBuffer) == 0x54 ? 1 : -1];
typedef char offsetofConversationBuffercallbackCheck[offsetof(ConversationBuffer, callback) == 4 ? 1 : -1];
typedef char offsetofConversationBufferstreamCheck[offsetof(ConversationBuffer, stream) == 0x10 ? 1 : -1];
typedef char offsetofConversationBufferpacketRequestCheck[offsetof(ConversationBuffer, packetRequest) == 0x30 ? 1 : -1];
typedef char offsetofConversationBufferpriorityCheck[offsetof(ConversationBuffer, priority) == 0x34 ? 1 : -1];
typedef char offsetofConversationBuffervolumeCheck[offsetof(ConversationBuffer, volume) == 0x38 ? 1 : -1];
typedef char offsetofConversationBufferspeakerCheck[offsetof(ConversationBuffer, speaker) == 0x40 ? 1 : -1];
typedef char offsetofConversationBufferduckVolumeCheck[offsetof(ConversationBuffer, duckVolume) == 0x4c ? 1 : -1];
typedef char sizeofConversationTaskDataCheck[sizeof(ConversationTaskData) == 0xa8 ? 1 : -1];
typedef char sizeofConversationSpeakerAliasCheck[sizeof(ConversationSpeakerAlias) == 0x0c ? 1 : -1];
using ::shenmue::shen_name;
#define CONVERSATION_TASK shen_name(SHEN_NAME4('V', 'O', 'I', 'C'))
#define CONVERSATION_WORK_STORAGE shen_name(SHEN_NAME4('C', 'V', 'W', 'K'))
#define CONVERSATION_PACKET_STORAGE shen_name(SHEN_NAME4('C', 'V', 'B', 'F'))
#define CONVERSATION_RESOURCE_STORAGE shen_name(SHEN_NAME4('C', 'V', 'R', 'S'))
#define CONVERSATION_VOICE_STORAGE shen_name(SHEN_NAME4('V', 'B', 'U', 'F'))
#define CONVERSATION_SYP shen_name(SHEN_NAME4('S', 'Y', 'P', '_'))
#define CONVERSATION_SYX shen_name(SHEN_NAME4('S', 'Y', 'X', '_'))
#define CONVERSATION_SYE shen_name(SHEN_NAME4('S', 'Y', 'E', '_'))
#define CONVERSATION_JOP shen_name(SHEN_NAME4('J', 'O', 'P', '_'))
#define CONVERSATION_JOY shen_name(SHEN_NAME4('J', 'O', 'Y', '_'))
#define CONVERSATION_GAI2 shen_name(SHEN_NAME4('G', 'A', 'I', '2'))
#define CONVERSATION_GAI shen_name(SHEN_NAME4('G', 'A', 'I', '_'))
extern ConversationTaskData *g_conversationTaskData;
extern ConversationResource *g_conversationResources;
extern ConversationBuffer *g_currentDialogue;
extern ConversationSpeakerAlias g_conversationSpeakerAliases[8];
extern void *g_conversationVoiceMemory;
extern void *g_sharedConversationStream;
std::uint32_t *FindCachedConversationIndex(const char *name);
AudioPartition *FindCachedConversationArchive(const char *name);
std::uint16_t GetConversationIndexEntryCount(const std::uint32_t *index);
AudioPartition *MountConversationArchive(const char *name, std::int32_t scene, std::int32_t entryCount, void *providedInfo);
void RequestConversationBank(const char *name);
bool FinishConversationBank(const char *name);
void ReleaseConversationBank(const char *name);
bool EV_FreeAuthorPak(std::int32_t operation, const char *name);
void AllocateConversationStreamBuffers();
void ResetConversationSpeakerAliases();
bool RegisterConversationSpeakerAlias(shen_name target, shen_name source, std::uint32_t flags);
void SetConversationStreamVolume(ConversationStream &stream, std::int32_t volume);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool cvsGet() SHENMUE_AUDIO_NOEXCEPT;
void SetScenarioDialoguePitch(ConversationBuffer *buffer, std::int32_t pitch);
bool RemoveConversationSpeakerAlias(shen_name target) SHENMUE_AUDIO_NOEXCEPT;
ConversationBuffer *GetConverseDataForChara(shen_name speaker) SHENMUE_AUDIO_NOEXCEPT;
shen_name cvsFindVoiceDataFor(const ConversationBuffer *buffer) SHENMUE_AUDIO_NOEXCEPT;
std::int32_t GetConversationPaused(const ConversationBuffer *buffer) SHENMUE_AUDIO_NOEXCEPT;
void PauseOrDestroyAudioAndSubtitles(std::int32_t paused);
#endif
ConversationBuffer *FindConversationByPriority(std::uint32_t mask) SHENMUE_AUDIO_NOEXCEPT;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t cvsHasValidConverseData(const ConversationBuffer *buffer = SHENMUE_AUDIO_NULL) SHENMUE_AUDIO_NOEXCEPT;
#else
bool cvsHasValidConverseData(const ConversationBuffer *buffer = SHENMUE_AUDIO_NULL) SHENMUE_AUDIO_NOEXCEPT;
#endif
void UpdateConversationDucking(ConversationBuffer &buffer);
void StopConversationStream(ConversationStream &stream);
void ReleaseConversationPacket(void *request);
void ReleaseConversationBuffer(ConversationBuffer &buffer);
void StopConversationBuffers(ConversationBuffer *buffer);
void SetConversationBufferVolume(ConversationBuffer *buffer, std::int32_t volume);
void SetConversationVolume(ConversationBuffer *buffer, std::int32_t volume);
void PauseConversation(ConversationBuffer *buffer, std::int32_t paused);
void scenCvsCtrlFree(ConversationBuffer *buffer, std::int32_t frames);
void CVS_CtrlConversTask();
void CVS_CtrlConversTask_Callback(void *parameter);
void CVS_CtrlConversTask_Enqueue(std::int32_t loadSceneBanks);

} // namespace s2
} // namespace shenmue

#undef SHENMUE_AUDIO_NULL
#undef SHENMUE_AUDIO_NOEXCEPT
#endif
