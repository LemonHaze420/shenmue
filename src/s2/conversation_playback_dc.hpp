#pragma once

#include "s2/conversation_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"

namespace shenmue {
namespace s2 {

struct ConversationIndexEntry {
    char code[12];
    std::uint16_t audioEntry;
    std::uint16_t packetEntry;
    std::int32_t packetOffset;
};
typedef char Layout_conversation_playback_dc_0[(sizeof(ConversationIndexEntry) == 0x14) ? 1 : -1];
typedef char Layout_conversation_playback_dc_1[(__alignof__(ConversationIndexEntry) == 4) ? 1 : -1];
typedef char Layout_conversation_playback_dc_2[(offsetof(ConversationIndexEntry, audioEntry) == 0x0c) ? 1 : -1];
typedef char Layout_conversation_playback_dc_3[(offsetof(ConversationIndexEntry, packetEntry) == 0x0e) ? 1 : -1];
typedef char Layout_conversation_playback_dc_4[(offsetof(ConversationIndexEntry, packetOffset) == 0x10) ? 1 : -1];

struct ConversationIndexBucket {
    std::uint32_t prefix;
    std::uint32_t firstEntry;
};
typedef char Layout_conversation_playback_dc_5[(sizeof(ConversationIndexBucket) == 8) ? 1 : -1];

#define CONVERSATION_NULL_SPEAKER CHARACTER_NAME_NULL
#define CONVERSATION_PACKET_PENDING_TAG SHEN_NAME4('S', 'E', 'G', 'A')
#define CONVERSATION_INDEX_TABLE SHEN_NAME4('T', 'A', 'B', 'L')
#define CONVERSATION_INDEX_SEARCH SHEN_NAME4('S', 'I', 'D', 'X')
#define CONVERSATION_INDEX_END SHEN_NAME4('E', 'N', 'D', 'I')
#define CONVERSATION_CHARACTER_CHUNK SHEN_NAME4('C', 'H', 'I', 'D')
#define CONVERSATION_TEXT_CHUNK SHEN_NAME4('S', 'T', 'D', 'L')
#define CONVERSATION_CLIP_CHUNK SHEN_NAME4('C', 'L', 'I', 'P')

const ConversationIndexEntry *FindConversationIndexEntry(const std::uint32_t *index, const char *code);
bool AllocateConversationVoice(ConversationStream &stream, std::int32_t output);
bool StartConversationStream(ConversationStream &stream, std::int32_t partition, std::uint32_t entry, std::int32_t output);
std::int32_t IsConversationStreamActive(ConversationStream &stream);
std::int32_t IsConversationPacketReadPending(void *request);
void SetConversationStreamCameraCode(ConversationStream &stream, std::int32_t cameraCode);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetConversationStreamPitch(ConversationStream &stream, std::int32_t pitch);
#endif
void *ReadConversationPacket(AudioPartition *partition, std::uint32_t entry, std::int32_t sector, ConversationPacketBuffer *packet);
bool FindCachedConversationPacket(ConversationPacketBuffer &packet, const char *bankName, std::uint32_t entry, std::int32_t sector);
void ConversationPacketLoadCallback(ConversationBuffer &buffer);
void ConversationPlaybackCallback(ConversationBuffer &buffer);
void ApplyConversationPacket(void *packet, ConversationBuffer &buffer);
ConversationBuffer *shenPlayAudio(const char *code, shen_name speaker, std::uint32_t flags);
ConversationBuffer *shenPlayAudioWithFlags(const char *code, std::uint32_t flags);

} // namespace s2
} // namespace shenmue

#endif
