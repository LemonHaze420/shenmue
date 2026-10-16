#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name EVENT_VOICE_TASK{SHEN_NAME(EVNT)};
inline constexpr std::size_t EVENT_VOICE_LIST_CAPACITY = 32;
inline constexpr std::size_t EVENT_VOICE_PREFIX_CAPACITY = 32;
struct EventVoiceListWork {
    HLib::Task *parent;
    std::int32_t voiceDelayFrames;
    std::int32_t index;
    std::int32_t skipPressFrames;
    std::uint32_t mode;
    char voices[EVENT_VOICE_LIST_CAPACITY][12];
    float delay;
    std::uint32_t reserved19c;
};
static_assert(sizeof(EventVoiceListWork) == 0x1a0);
static_assert(offsetof(EventVoiceListWork, mode) == 0x14);
static_assert(offsetof(EventVoiceListWork, voices) == 0x18);
static_assert(offsetof(EventVoiceListWork, delay) == 0x198);
extern char eventVoicePrefix[EVENT_VOICE_PREFIX_CAPACITY];
void QueueEventVoiceList(HLib::Task *parent, const char *const *voices, std::int32_t voiceDelayFrames, std::uint32_t mode, std::int32_t delayFrames);
void EventVoiceListCallback(void *);
inline constexpr std::size_t SK_PENDING_VOICE_CAPACITY = 16;
extern char skVoicePrefixes[2][16];
struct SkPendingVoiceWork {
    HLib::Task *parent;
    std::int32_t delayFrames, index, reserved10, conversationFrames;
    std::uint32_t mode, reserved1c;
};
static_assert(sizeof(SkPendingVoiceWork) == 0x20);
static_assert(offsetof(SkPendingVoiceWork, conversationFrames) == 0x14);
static_assert(offsetof(SkPendingVoiceWork, mode) == 0x18);
char *AppendSkPendingVoice(std::int32_t prefix, const char *suffix);
void PlaySkSecondPrefixVoice(std::uintptr_t unused, const char *suffix);
void QueueSkPendingVoiceWait(HLib::Task *parent);
void SkPendingVoiceWaitCallback(void *parameter);
} // namespace shenmue::s2
