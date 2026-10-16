#include "s2/areas/area_bgsm.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/input.hpp"
#include <cstring>
namespace shenmue::s2::areas {
std::int32_t bgsmVoiceCount{};
char bgsmVoices[16][12]{}, bgsmVoicePrefixes[4][16]{}, bgsmMessage[52]{};
void BgsmAppendVoice(std::int32_t speaker, const char *suffix) {
    char *voice = bgsmVoices[bgsmVoiceCount++];
    std::strcpy(voice, bgsmVoicePrefixes[speaker]);
    std::strcat(voice, suffix);
}
void BgsmPlayVoice(std::int32_t speaker, const char *suffix) {
    char voice[64];
    std::strcpy(voice, bgsmVoicePrefixes[speaker]);
    std::strcat(voice, suffix);
    EVT_AT_SetVoice(voice, 1, 0);
}
void BgsmQueueVoices(HLib::Task *parent, std::uint32_t flags) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BgsmWaitVoice, 6, 0x0b, sizeof(BgsmVoiceWork), BGSM_TASK);
    if (!task)
        return;
    BgsmVoiceWork *work = static_cast<BgsmVoiceWork *>(HLib::GetTaskParameter(task));
    work->parent = parent;
    work->delay = 8;
    work->flags = flags;
    work->index = 0;
    work->reserved10 = 0;
    work->conversationFrames = 0;
    bgsmVoices[bgsmVoiceCount][0] = 0;
    bgsmVoiceCount = 0;
    if (parent)
        HLib::SetupTimedTask(parent, 0);
}
void BgsmWaitVoice(void *data) {
    BgsmVoiceWork &work = *static_cast<BgsmVoiceWork *>(data);
    if (cvsHasValidConverseData(nullptr)) {
        if (work.conversationFrames++ > 600)
            scenCvsCtrlFree(nullptr);
        if ((work.flags & 0x40000000U) && CheckMediatedDoublePress())
            scenCvsCtrlFree(nullptr);
        return;
    }
    const char *voice = bgsmVoices[work.index];
    if (*voice) {
        EVT_AT_SetVoice(voice, 1, work.delay);
        if (work.delay > 0)
            HLib::SetupTaskDelay(work.delay);
        ++work.index;
        work.conversationFrames = 0;
        if (bgsmVoices[work.index][0] || !(work.flags & 0x20000000U))
            return;
    }
    if (work.parent)
        HLib::ResolveDeferredCallback(work.parent);
    HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2::areas
