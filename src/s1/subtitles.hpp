#pragma once

#include "s1/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {
struct LargeFileHandle;
}

namespace shenmue::s1 {


struct SubtitleStringId {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
};

static_assert(sizeof(SubtitleStringId) == 12);

#pragma pack(push, 1)
struct VoiceIndexRecord {
    char id[12];
    std::uint16_t voiceEntry;
    std::uint16_t subtitleEntry;
    std::uint32_t subtitleBytes;
};


struct CVWKAudioContext {
    void *sourceVoice;
    void *streamBuffer;
    void *task;
    std::int64_t streamCookie;
    std::uint64_t playbackScale;
    std::uint32_t field28;
    std::uint64_t fadeFrames;
    std::uint32_t volume;
};

struct CVWKRuntimeSlot {
    void *cvfbBlock;
    void (*callback)(CVWKRuntimeSlot *);
    std::int32_t stateOrOwner;
    std::uint32_t field14;
    void *fileOrSequence;
    CVWKAudioContext audioContext;
    std::byte reserved58[8];
    d3t::LargeFileHandle *fileHandle;
    std::uint32_t field68;
    std::uint32_t participantId;
    std::uint32_t fadeFrame;
    std::uint32_t fadeFrames;
    void *clip;
    void *japaneseText;
    std::byte reserved88[4];
    char voiceId[12];
};


struct CVWK {
    CVWKRuntimeSlot slots[2];
    std::byte reserved130[0x10];
};
#pragma pack(pop)

static_assert(sizeof(VoiceIndexRecord) == 0x14);
static_assert(offsetof(VoiceIndexRecord, voiceEntry) == 0x0c);
static_assert(offsetof(VoiceIndexRecord, subtitleEntry) == 0x0e);
static_assert(offsetof(VoiceIndexRecord, subtitleBytes) == 0x10);
static_assert(sizeof(CVWKAudioContext) == 0x38);
static_assert(offsetof(CVWKAudioContext, sourceVoice) == 0x00);
static_assert(offsetof(CVWKAudioContext, task) == 0x10);
static_assert(offsetof(CVWKAudioContext, playbackScale) == 0x20);
static_assert(offsetof(CVWKAudioContext, volume) == 0x34);
static_assert(sizeof(CVWKRuntimeSlot) == 0x98);
static_assert(offsetof(CVWKRuntimeSlot, callback) == 0x08);
static_assert(offsetof(CVWKRuntimeSlot, audioContext) == 0x20);
static_assert(offsetof(CVWKRuntimeSlot, fileHandle) == 0x60);
static_assert(offsetof(CVWKRuntimeSlot, voiceId) == 0x8c);
static_assert(sizeof(CVWK) == 0x140);

extern CVWK *g_pCVWK;
CVWK *CVWK_Initialize();
void CVWK_SetAudioPauseState(bool paused);
CVWKRuntimeSlot *ProcessSubtitleFile(const char *voiceId, std::uint32_t participantId = 0, std::uint32_t flags = 1, std::uint32_t *aliasState = nullptr);
void SEQ_FreeFileHandles(CVWKRuntimeSlot &slot);
void SEQ_ResetSubtitleAudioSlots();
void SEQ_EndActiveScenarioSubtitles();
void shenForceClearCurrentSubtitles(CVWKRuntimeSlot *slot);
void DisableSubtitles();
bool CVWK_IsSlotActive(const CVWKRuntimeSlot *slot);
void AssignReadSubtitleByID();
void ReadSubtitleByID(HLib::Task *scenarioTask, CVWKRuntimeSlot &slot);

std::uint32_t scntCheckHasParticipant(std::uint32_t name);

const char *ReadStringFromStringTableById(const SubtitleStringId &id);
const char *InitAndRetrieveStringFromID(const char *stringId);
const char *ReadSubtitleStringById(const SubtitleStringId &id, bool japaneseAudio);
void ProcessSubtitleText(const char *text);
void ApplySubtitlePresentationClass(std::uint32_t presentationClass);
const std::array<std::array<char, 512>, 3> &CurrentSubtitleLines();

} // namespace shenmue::s1
