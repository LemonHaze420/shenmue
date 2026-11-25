#include "s2/conversation_playback_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_info.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

// @verified - d
const ConversationIndexEntry *FindConversationIndexEntry(const std::uint32_t *index, const char *code) {
    MapInfoChunk *const first = reinterpret_cast<MapInfoChunk *>(const_cast<std::uint32_t *>(index));
    const MapInfoChunk *const table = FindNativeChunk("TABL", first);
    if (!table)
        return 0;
    const ConversationIndexEntry *const entries = reinterpret_cast<const ConversationIndexEntry *>(table + 1);
    const MapInfoChunk *const search = FindNativeChunk("SIDX", first);
    if (!search)
        return 0;
    const ConversationIndexBucket *bucket = reinterpret_cast<const ConversationIndexBucket *>(search + 1);
    std::uint32_t words[3];
    std::memcpy(words, code, sizeof(words));
    while (bucket->prefix != CONVERSATION_INDEX_END) {
        if (bucket->prefix == words[0]) {
            const ConversationIndexEntry *entry = entries + bucket->firstEntry;
            std::uint32_t prefix;
            std::memcpy(&prefix, entry->code, sizeof(prefix));
            while (prefix == words[0]) {
                std::uint32_t middle;
                std::memcpy(&middle, entry->code + 4, sizeof(middle));
                if (middle == words[1]) {
                    std::uint32_t tail;
                    std::memcpy(&tail, entry->code + 8, sizeof(tail));
                    if (tail == words[2])
                        return entry;
                }
                ++entry;
                std::memcpy(&prefix, entry->code, sizeof(prefix));
            }
        }
        ++bucket;
    }
    return 0;
}

// @verified - d
bool FindCachedConversationPacket(ConversationPacketBuffer &packet, const char *bankName, std::uint32_t entry, std::int32_t sector) {
    if (packet.sector == sector && packet.entry == entry && !std::strcmp(packet.bankName, bankName))
        return true;
    ConversationBuffer *buffer = g_conversationTaskData->buffers;
    for (std::int32_t count = sizeof(g_conversationTaskData->buffers) / sizeof(g_conversationTaskData->buffers[0]); count; --count, ++buffer) {
        if (buffer->packet != &packet && !buffer->packet->pending && buffer->packet->sector == sector && buffer->packet->entry == entry &&
            !std::strcmp(buffer->packet->bankName, bankName)) {
            std::memcpy(&packet, buffer->packet, sizeof(packet));
            return true;
        }
    }
    return false;
}

} // namespace s2
} // namespace shenmue

#endif
