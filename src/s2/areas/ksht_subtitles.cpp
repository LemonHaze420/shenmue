#include "s2/areas/area_ksht.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include <cstring>
namespace shenmue::s2::areas {
namespace {
struct KshtSubtitleRecord {
    std::int32_t frames;
    char text[80];
};
static_assert(sizeof(KshtSubtitleRecord) == 84);
const KshtSubtitleRecord subtitles[]{
    {.frames = 45, .text = "\xe2\x97\x8f\xe9\x9d\x92\xe8\x8e\x8e\xe8\x8a\xb1"},
    {.frames = 60, .text = "\xe3\x82\x82\xe3\x81\x86\xe5\xb0\x91\xe3\x81\x97\xe5\xbe\x85\xe3\x81\xa3\xe3\x81\xa6\xe3\x81\xad"},
    {.frames = 60,
        .text = "\xe3\x81\x82\xe3\x80\x81\xe6\xb6\xbc\xe3\x80\x82\xe3\x81\x8a\xe3\x81\xaf\xe3\x82\x88\xe3\x81\x86"
                "\xe3\x80\x82"},
    {.frames = 45, .text = "\xe2\x97\x8f\xe9\x9d\x92\xe3\x81\x8a\xe3\x81\xaf\xe3\x82\x88\xe3\x81\x86"},
    {.frames = 105,
        .text = "\xe4\xbb\x8a\xe3\x80\x81\xe3\x81\x8a\xe5\xbc\x81\xe5\xbd\x93\xe3\x81\xae\xe6\xba\x96\xe5\x82\x99"
                "\xe3\x82\x92\xe3\x81\x97\xe3\x81\xa6\xe3\x81\x84\xe3\x82\x8b\xe3\x81\xae\xe3\x80\x82\xef\xbc\x86"
                "\xe3\x82\x82\xe3\x81\x86\xe5\xb0\x91\xe3\x81\x97\xe5\xbe\x85\xe3\x81\xa3\xe3\x81\xa6\xe3\x81\xad"
                "\xe3\x80\x82"},
    {.frames = 45,
        .text = "\xe2\x97\x8f\xe9\x9d\x92\xe3\x81\x9d\xe3\x82\x8d\xe3\x81\x9d\xe3\x82\x8d\xe3\x81\x84\xe3\x81\x8f"
                "\xe3\x81\x8b"},
    {.frames = 75,
        .text = "\xe2\x97\x8f\xe9\x9d\x92\xe5\xaf\x9d\xe5\xae\xa4\xe3\x81\xab\xe3\x81\xaf\xe5\x85\xa5\xe3\x82\x89"
                "\xe3\x81\xaa\xe3\x81\x84\xe6\x96\xb9\xe3\x81\x8c\xe3\x81\x84\xe3\x81\x84\xe3\x81\xaa"},
};
struct KshtSubtitleState {
    HLib::Task *task;
    std::int32_t read, write, count, active, countdown;
    std::int32_t messages[16], delays[16];
    std::uint32_t reserved9c;
};
static_assert(sizeof(KshtSubtitleState) == 0xa0 && offsetof(KshtSubtitleState, messages) == 0x1c);
KshtSubtitleState subtitleState;
void KshtSubtitleCallback(void *) {
    if (subtitleState.active == 1) {
        if (subtitleState.delays[subtitleState.read]) {
            --subtitleState.delays[subtitleState.read];
            return;
        }
        --subtitleState.countdown;
        if (GetCurrentControlInput(INPUT_PRESSED) & 0x600)
            subtitleState.countdown = 0;
        else if (subtitleState.countdown > 0)
            return;
        ctrlStopSubtitles();
        --subtitleState.count;
        subtitleState.active = 0;
        subtitleState.read = (subtitleState.read + 1) & 15;
    } else if (subtitleState.count) {
        subtitleState.active = 1;
        const KshtSubtitleRecord &record = subtitles[subtitleState.messages[subtitleState.read]];
        subtitleState.countdown = record.frames;
        dbgPrintEventMessage(record.text);
    }
}
} // namespace
void ResetKshtSubtitles() {
    std::memset(&subtitleState, 0, sizeof(subtitleState));
}
void StartKshtSubtitles() {
    subtitleState.task = HLib::EnqueueTaskWithoutParameter(KshtSubtitleCallback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(kshS)));
    subtitleState.read = 0;
    subtitleState.write = 0;
    subtitleState.count = 0;
    subtitleState.active = 0;
    subtitleState.countdown = 0;
    std::memset(subtitleState.messages, 0, sizeof(subtitleState.messages));
    std::memset(subtitleState.delays, 0, sizeof(subtitleState.delays));
}
void QueueKshtSerif(std::int32_t message, std::int32_t delay) {
    if (subtitleState.count < 16) {
        ++subtitleState.count;
        subtitleState.messages[subtitleState.write] = message;
        subtitleState.delays[subtitleState.write] = delay;
        subtitleState.write = (subtitleState.write + 1) & 15;
    }
}
} // namespace shenmue::s2::areas
