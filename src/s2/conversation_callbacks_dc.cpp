#include "s2/conversation_playback_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/conversation_fade_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

// @verified - d
bool UpdateConversationSubtitlePlayback(HLib::Task *scenarioTask) {
    if (scenHasActiveSubtitles(scenarioTask)) {
        ForceScenarioSubtitles(scenarioTask);
        return true;
    }
    scenDestroySubtitles(scenarioTask);
    return false;
}

// @verified - d
void ConversationPacketLoadCallback(ConversationBuffer &buffer) {
    UpdateConversationStream(buffer.stream);
    if (IsConversationPacketReadPending(buffer.packetRequest))
        return;
    ReleaseConversationPacket(buffer.packetRequest);
    buffer.packetRequest = 0;
    if (*reinterpret_cast<const std::uint32_t *>(buffer.packet->data) == CONVERSATION_PACKET_PENDING_TAG) {
        buffer.packetRequest =
            ReadConversationPacket(static_cast<AudioPartition *>(buffer.resource->stream), buffer.packet->entry, buffer.packet->sector, buffer.packet);
        return;
    }
    buffer.packet->pending = 0;
    std::byte *const data = buffer.packet->data;
    ApplyConversationPacket(data + static_cast<std::uint32_t>(buffer.packetOffset) / 4 * 4, buffer);
    buffer.callback = ConversationPlaybackCallback;
}

// @verified - d
void ConversationPlaybackCallback(ConversationBuffer &buffer) {
    UpdateConversationStream(buffer.stream);
    if (IsConversationStreamActive(buffer.stream))
        return;
    StopConversationStream(buffer.stream);
    if (g_currentDialogue == &buffer && buffer.packetOffset >= 0) {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (UpdateConversationSubtitlePlayback(control->scenarioTask))
            return;
        g_currentDialogue = 0;
    }
    if (buffer.speaker) {
        ConversationBuffer *candidate = g_conversationTaskData->buffers;
        std::int32_t remaining = 2;
        while (remaining) {
            if (candidate != &buffer && candidate->callback && buffer.speaker == candidate->speaker)
                break;
            --remaining;
            ++candidate;
        }
        if (!remaining) {
            const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            scenAssignFacialClip(control->scenarioTask, defaultFacialClip, buffer.speaker);
        }
    }
    buffer.callback = 0;
}

} // namespace s2
} // namespace shenmue

#endif
