#include "s2/event_voice.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include <cstring>
namespace shenmue::s2 {
char eventVoicePrefix[EVENT_VOICE_PREFIX_CAPACITY]{};
char skVoicePrefixes[2][16]{};
namespace {
char skPendingVoiceIds[SK_PENDING_VOICE_CAPACITY][12]{};
std::int32_t skPendingVoiceCount{};
}

// @verified - p
char *AppendSkPendingVoice(std::int32_t prefix, const char *suffix) {
    char *const voice = skPendingVoiceIds[skPendingVoiceCount];
    std::strcpy(voice, skVoicePrefixes[prefix]);
    char *destination = voice;
    while (*destination)
        ++destination;
    std::strcpy(destination, suffix);
    ++skPendingVoiceCount;
    return destination;
}

// @verified - p
void PlaySkSecondPrefixVoice(std::uintptr_t, const char *suffix) {
    char voice[64];
    std::strcpy(voice, skVoicePrefixes[1]);
    std::strcat(voice, suffix);
    EVT_AT_SetVoice(voice, 1, 0);
}

// @verified - p
void QueueSkPendingVoiceWait(HLib::Task *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkPendingVoiceWaitCallback, 6, 0x0b, sizeof(SkPendingVoiceWork), EVENT_VOICE_TASK);
    HLib::SetupTimedTask(task, 30);
    SkPendingVoiceWork *const work = static_cast<SkPendingVoiceWork *>(HLib::GetTaskParameter(task));
    const std::int32_t count = skPendingVoiceCount;
    skPendingVoiceCount = 0;
    work->parent = parent;
    work->delayFrames = 0;
    work->index = 0;
    work->conversationFrames = 0;
    work->reserved10 = 0;
    skPendingVoiceIds[count][0] = 0;
    if (work->parent)
        HLib::SetupTimedTask(work->parent, 0);
}

// @verified - p
void SkPendingVoiceWaitCallback(void *parameter) {
    SkPendingVoiceWork *const work = static_cast<SkPendingVoiceWork *>(parameter);
    if (cvsHasValidConverseData(nullptr)) {
        if (work->conversationFrames++ > 600)
            scenCvsCtrlFree(nullptr);
        if (CheckMediatedDoublePress() && (work->mode & 0x40000000U))
            scenCvsCtrlFree(nullptr);
    } else {
        const char *const voice = skPendingVoiceIds[work->index];
        if (*voice) {
            EVT_AT_SetVoice(voice, 1, work->delayFrames);
            if (work->delayFrames > 0)
                HLib::SetupTaskDelay(work->delayFrames);
            ++work->index;
            work->conversationFrames = 0;
            if (skPendingVoiceIds[work->index][0] || !(work->mode & 0x20000000U))
                return;
        }
        if (work->parent)
            HLib::ResolveDeferredCallback(work->parent);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void QueueEventVoiceList(HLib::Task *parent, const char *const *voices, std::int32_t voiceDelayFrames, std::uint32_t mode, std::int32_t delayFrames) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventVoiceListCallback, 6, 0x0b, sizeof(EventVoiceListWork), EVENT_VOICE_TASK);
    RegisterEventTask(task);
    EventVoiceListWork *const work = static_cast<EventVoiceListWork *>(HLib::GetTaskParameter(task));
    work->voiceDelayFrames = voiceDelayFrames;
    work->mode = mode;
    work->parent = parent;
    work->index = 0;
    work->delay = static_cast<float>(delayFrames);
    std::size_t index = 0;
    while (voices[index]) {
        if ((mode & 0x80000000U) != 0) {
            std::strcpy(work->voices[index], eventVoicePrefix);
            std::strcat(work->voices[index], voices[index]);
        } else {
            std::strcpy(work->voices[index], voices[index]);
        }
        if (++index >= EVENT_VOICE_LIST_CAPACITY)
            break;
    }
    if (index < EVENT_VOICE_LIST_CAPACITY)
        work->voices[index][0] = 0;
    if (work->parent)
        HLib::SetupTimedTask(work->parent, 0);
}
// @verified - p
void EventVoiceListCallback(void *parameter) {
    EventVoiceListWork *const work = static_cast<EventVoiceListWork *>(parameter);
    if (work->delay > 0) {
        work->delay -= HLib::GetScaledDeltaTick();
        return;
    }
    if (cvsHasValidConverseData(nullptr)) {
        if ((work->mode & 0x40000000U) != 0) {
            if (static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_NEW_PRESSES)) == 512) {
                if (work->skipPressFrames != 0)
                    scenCvsCtrlFree(nullptr);
                else
                    work->skipPressFrames = 10;
            } else if (work->skipPressFrames > 0) {
                --work->skipPressFrames;
            }
        }
        return;
    }
    const char *const voice = work->voices[work->index];
    if (*voice) {
        EVT_AT_SetVoice(voice, 1, work->voiceDelayFrames);
        if (work->voiceDelayFrames > 0)
            work->delay = static_cast<float>(work->voiceDelayFrames);
        ++work->index;
    } else {
        if (work->parent)
            HLib::ResolveDeferredCallback(work->parent);
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    }
}
} // namespace shenmue::s2
