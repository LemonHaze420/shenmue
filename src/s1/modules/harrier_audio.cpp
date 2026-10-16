#include "s1/modules/harrier_audio.hpp"

#include "s1/archive_data.hpp"
#include "s1/archive_handles.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/modules/harrier.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <thread>

namespace shenmue::s1 {

namespace {

constexpr std::uint32_t kHarrierSfxCount = 70;
constexpr std::uint32_t kInvalidVoiceId = std::numeric_limits<std::uint32_t>::max();
constexpr std::uint16_t kNoSfx = std::numeric_limits<std::uint16_t>::max();
constexpr std::uint32_t kHarrierAudioResetOffset = 0xd0014;
constexpr float kHarrierSfxGain = 0.5f;
constexpr float kHarrierMusicGain = 0.7f;

struct HarrierSfxSlot {
    std::byte *fileData;
    std::uint64_t fileSize;
    std::uint32_t voiceId;
    std::uint8_t paused;
    std::uint8_t padding[3];
};
static_assert(sizeof(HarrierSfxSlot) == 0x18);
static_assert(offsetof(HarrierSfxSlot, fileSize) == 0x08);
static_assert(offsetof(HarrierSfxSlot, voiceId) == 0x10);
static_assert(offsetof(HarrierSfxSlot, paused) == 0x14);

constexpr std::array<const char *, kHarrierSfxCount> kHarrierSfxFiles{
    "XHAR\\sound\\sfx\\00000000.wav",
    "XHAR\\sound\\sfx\\00000001.wav",
    "XHAR\\sound\\sfx\\00000002.wav",
    "XHAR\\sound\\sfx\\00000003.wav",
    "XHAR\\sound\\sfx\\00000004.wav",
    "XHAR\\sound\\sfx\\00000005.wav",
    "XHAR\\sound\\sfx\\00000006.wav",
    "XHAR\\sound\\sfx\\00000007.wav",
    "XHAR\\sound\\sfx\\00000008.wav",
    "XHAR\\sound\\sfx\\00000009.wav",
    "XHAR\\sound\\sfx\\0000000a.wav",
    "XHAR\\sound\\sfx\\0000000b.wav",
    "XHAR\\sound\\sfx\\0000000c.wav",
    "XHAR\\sound\\sfx\\0000000d.wav",
    "XHAR\\sound\\sfx\\0000000e.wav",
    "XHAR\\sound\\sfx\\0000000f.wav",
    "XHAR\\sound\\sfx\\00000010.wav",
    "XHAR\\sound\\sfx\\00000011.wav",
    "XHAR\\sound\\sfx\\00000012.wav",
    "XHAR\\sound\\sfx\\00000013.wav",
    "XHAR\\sound\\sfx\\00000014.wav",
    "XHAR\\sound\\sfx\\00000015.wav",
    "XHAR\\sound\\sfx\\00000016.wav",
    "XHAR\\sound\\sfx\\00000017.wav",
    "XHAR\\sound\\sfx\\00000018.wav",
    "XHAR\\sound\\sfx\\00000019.wav",
    "XHAR\\sound\\sfx\\0000001a.wav",
    "XHAR\\sound\\sfx\\0000001b.wav",
    "XHAR\\sound\\sfx\\0000001c.wav",
    "XHAR\\sound\\sfx\\0000001d.wav",
    "XHAR\\sound\\sfx\\0000001e.wav",
    "XHAR\\sound\\sfx\\0000001f.wav",
    "XHAR\\sound\\sfx\\00000020.wav",
    "XHAR\\sound\\sfx\\00000021.wav",
    "XHAR\\sound\\sfx\\00000022.wav",
    "XHAR\\sound\\sfx\\00000023.wav",
    "XHAR\\sound\\sfx\\00000024.wav",
    "XHAR\\sound\\sfx\\00000025.wav",
    "XHAR\\sound\\sfx\\00000026.wav",
    "XHAR\\sound\\sfx\\00000027.wav",
    "XHAR\\sound\\sfx\\00000028.wav",
    "XHAR\\sound\\sfx\\00000029.wav",
    "XHAR\\sound\\sfx\\0000002a.wav",
    "XHAR\\sound\\sfx\\0000002b.wav",
    "XHAR\\sound\\sfx\\0000002c.wav",
    "XHAR\\sound\\sfx\\0000002d.wav",
    "XHAR\\sound\\sfx\\0000002e.wav",
    "XHAR\\sound\\sfx\\0000002f.wav",
    "XHAR\\sound\\sfx\\00000030.wav",
    "XHAR\\sound\\sfx\\00000031.wav",
    "XHAR\\sound\\sfx\\00000032.wav",
    "XHAR\\sound\\sfx\\00000033.wav",
    "XHAR\\sound\\sfx\\00000034.wav",
    "XHAR\\sound\\sfx\\00000035.wav",
    "XHAR\\sound\\sfx\\00000036.wav",
    "XHAR\\sound\\sfx\\00000037.wav",
    "XHAR\\sound\\sfx\\00000038.wav",
    "XHAR\\sound\\sfx\\00000039.wav",
    "XHAR\\sound\\sfx\\0000003a.wav",
    "XHAR\\sound\\sfx\\0000003b.wav",
    "XHAR\\sound\\sfx\\0000003c.wav",
    "XHAR\\sound\\sfx\\0000003d.wav",
    "XHAR\\sound\\sfx\\0000003e.wav",
    "XHAR\\sound\\sfx\\0000003f.wav",
    "XHAR\\sound\\sfx\\00000040.wav",
    "XHAR\\sound\\sfx\\00000041.wav",
    "XHAR\\sound\\sfx\\00000042.wav",
    "XHAR\\sound\\sfx\\00000043.wav",
    "XHAR\\sound\\sfx\\00000044.wav",
    "XHAR\\sound\\sfx\\00000045.wav",
};

constexpr std::array<std::uint16_t, 256> kHarrierAudioFiles{
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    0,
    1,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    2,
    3,
    kNoSfx,
    4,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    5,
    kNoSfx,
    6,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    7,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    8,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    9,
    10,
    11,
    12,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    kNoSfx,
    27,
    28,
    29,
    30,
    31,
    kNoSfx,
    kNoSfx,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    kNoSfx,
    kNoSfx,
    kNoSfx,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    64,
    65,
    66,
    67,
    68,
    69,
    kNoSfx,
    kNoSfx,
    kNoSfx,
};

constexpr std::array<std::uint8_t, 256> kHarrierCommandTypes{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 1, 2, 2, 2, 2, 1, 0, 0, 2, 2, 1, 2, 2, 2,
    2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};

constexpr std::array<const char *, 256> MakeHarrierMusicFiles() {
    std::array<const char *, 256> files{};
    files[0xa3] = "XHAR\\sound\\harrier_a3.wav";
    files[0xa4] = "XHAR\\sound\\harrier_a4.wav";
    files[0xa5] = "XHAR\\sound\\harrier_a5.wav";
    files[0xa6] = "XHAR\\sound\\harrier_a6.wav";
    files[0xaa] = "XHAR\\sound\\harrier_aa.wav";
    files[0xab] = "XHAR\\sound\\harrier_ab.wav";
    files[0xad] = "XHAR\\sound\\harrier_ad.wav";
    files[0xae] = "XHAR\\sound\\harrier_ae.wav";
    files[0xaf] = "XHAR\\sound\\harrier_af.wav";
    files[0xb0] = "XHAR\\sound\\harrier_b0.wav";
    files[0xb1] = "XHAR\\sound\\harrier_b1.wav";
    files[0xb2] = "XHAR\\sound\\harrier_b2.wav";
    files[0xb7] = "XHAR\\sound\\harrier_b7.wav";
    files[0xb8] = "XHAR\\sound\\harrier_b8.wav";
    files[0xb9] = "XHAR\\sound\\harrier_b9.wav";
    return files;
}

constexpr std::array<const char *, 256> kHarrierMusicFiles = MakeHarrierMusicFiles();

HarrierSfxSlot *g_harrierSfxSlots{};
std::uint32_t g_harrierSfxCount{};
std::uint32_t g_harrierMusicArchiveHandle{};
std::array<char, 260> g_harrierMusicPath{};
std::array<std::byte *, 2> g_harrierMusicBuffers{};
std::array<std::uint64_t, 2> g_harrierMusicBufferSizes{};
std::array<std::uint32_t, 2> g_harrierMusicVoiceIds{kInvalidVoiceId, kInvalidVoiceId};
std::int32_t g_harrierMusicBufferIndex{};
std::int32_t g_harrierMusicCrossfadeFrame{-1};

void ReleaseVoice(std::uint32_t &voiceId) {
    if (voiceId == kInvalidVoiceId)
        return;
    StopSourceVoiceAndFlush(voiceId);
    ReleaseRawSoundEffectReservation(GetRawSoundEffectVoice(voiceId));
    voiceId = kInvalidVoiceId;
}

void WaitForAndCloseMusicRead() {
    if (!Archive_DoesHandleExist(g_harrierMusicArchiveHandle))
        return;
    while (Archive_HasPendingRead(g_harrierMusicArchiveHandle))
        std::this_thread::yield();
    ReadFile_0(0, &g_harrierMusicArchiveHandle);
    g_harrierMusicArchiveHandle = g_invalidArchiveHandle;
}

void SetMusicVoiceVolume(std::uint32_t voiceId, float volume) {
    if (voiceId == kInvalidVoiceId)
        return;
    SetRawSoundEffectVolume(GetRawSoundEffectVoice(voiceId), volume * g_applicationSettings.savedMusicVolume);
}

} // namespace

std::int32_t g_harrierMusicFileIndex{-1};

void Harrier_ResetAudioForCoreInit() {
    CleanupAudioSources();
    g_harrierMusicArchiveHandle = g_invalidArchiveHandle;
    g_harrierMusicFileIndex = -1;
    g_harrierMusicBufferIndex = 0;
    g_harrierMusicCrossfadeFrame = -1;
    g_harrierMusicPath.fill('\0');
    const std::uint32_t resetValue = 0xfdffffffU;
    std::memcpy(g_harrierMemory + kHarrierAudioResetOffset, &resetValue, sizeof(resetValue));
}

void mallocHarrierSFX() {
    if (g_harrierSfxSlots)
        Harrier_ReleaseSoundEffects();
    g_harrierSfxCount = kHarrierSfxCount;
    g_harrierSfxSlots = static_cast<HarrierSfxSlot *>(std::calloc(kHarrierSfxCount, sizeof(HarrierSfxSlot)));
    if (!g_harrierSfxSlots) {
        g_harrierSfxCount = 0;
        return;
    }
    for (std::uint32_t index = 0; index < kHarrierSfxCount; ++index) {
        HarrierSfxSlot &slot = g_harrierSfxSlots[index];
        slot.voiceId = kInvalidVoiceId;
        slot.fileSize = ArchiveData_GetFileSize(kHarrierSfxFiles[index]);
        if (slot.fileSize == 0 || slot.fileSize > std::numeric_limits<std::size_t>::max())
            continue;
        slot.fileData = static_cast<std::byte *>(std::malloc(static_cast<std::size_t>(slot.fileSize)));
        if (!slot.fileData) {
            slot.fileSize = 0;
            continue;
        }
        if (!ArchiveData_Read(kHarrierSfxFiles[index], 0, slot.fileData, slot.fileSize)) {
            std::free(slot.fileData);
            slot.fileData = nullptr;
            slot.fileSize = 0;
        }
    }
}

void CreateAndStartPlayingSourceVoiceForHarrier(std::uint32_t soundIndex) {
    if (!g_harrierSfxSlots || soundIndex >= g_harrierSfxCount)
        return;
    HarrierSfxSlot &slot = g_harrierSfxSlots[soundIndex];
    if (slot.voiceId != kInvalidVoiceId) {
        if (RawSoundEffectVoiceIsActive(GetRawSoundEffectVoice(slot.voiceId)))
            StopSourceVoiceAndFlush(slot.voiceId);
        ReleaseRawSoundEffectReservation(GetRawSoundEffectVoice(slot.voiceId));
    }
    void *const playback = AllocateRawSoundEffectVoice(16, 2, false);
    slot.voiceId = RawSoundEffectVoiceId(playback);
    slot.paused = 0;
    if (slot.voiceId == kInvalidVoiceId)
        return;
    CreateAndStartPlayingSourceVoice(slot.voiceId, slot.fileData, static_cast<std::size_t>(slot.fileSize));
    SetRawSoundEffectVolume(playback, kHarrierSfxGain * g_applicationSettings.savedSfxVolume);
}

void HarrierStopAndResetVoiceSlot(std::uint32_t soundIndex) {
    if (!g_harrierSfxSlots || soundIndex >= g_harrierSfxCount)
        return;
    HarrierSfxSlot &slot = g_harrierSfxSlots[soundIndex];
    ReleaseVoice(slot.voiceId);
    slot.paused = 0;
}

void DoAudio(bool paused) {
    if (!g_harrierSfxSlots)
        return;
    for (std::uint32_t index = 0; index < g_harrierSfxCount; ++index) {
        HarrierSfxSlot &slot = g_harrierSfxSlots[index];
        if (slot.voiceId == kInvalidVoiceId)
            continue;
        void *const playback = GetRawSoundEffectVoice(slot.voiceId);
        if (paused) {
            if (!slot.paused) {
                slot.paused = 1;
                SetRawSoundEffectPaused(playback, true);
            }
        } else if (slot.paused) {
            slot.paused = 0;
            StartRawSoundEffectPlayback(playback);
        } else if (!RawSoundEffectVoiceIsActive(playback)) {
            ReleaseVoice(slot.voiceId);
        }
    }
}

void HarrierLoadFileFromPath(const char *path) {
    if (!path)
        return;
    if (std::strcmp(g_harrierMusicPath.data(), path) == 0 && Archive_DoesHandleExist(g_harrierMusicArchiveHandle))
        return;
    WaitForAndCloseMusicRead();
    const std::size_t pathLength = std::strlen(path);
    if (pathLength >= g_harrierMusicPath.size())
        return;
    std::memcpy(g_harrierMusicPath.data(), path, pathLength + 1);
    const std::uint64_t fileSize = ArchiveData_GetFileSize(path);
    if (fileSize == 0 || fileSize > std::numeric_limits<std::size_t>::max())
        return;
    void *const buffer = std::malloc(static_cast<std::size_t>(fileSize));
    if (!buffer)
        return;
    g_harrierMusicArchiveHandle = Archive_AllocateHandle(0);
    ArchiveHandleEntry *const entry = Archive_GetHandle(g_harrierMusicArchiveHandle);
    if (!entry || !Archive_DoesHandleExist(g_harrierMusicArchiveHandle)) {
        std::free(buffer);
        g_harrierMusicArchiveHandle = g_invalidArchiveHandle;
        return;
    }
    entry->platformHandle = -1;
    Archive_BindBuffer(g_harrierMusicArchiveHandle, buffer, fileSize, false);
    Archive_QueueRead(g_harrierMusicArchiveHandle, 0, path, 0, fileSize, 1);
}

void AudioFlushFromArchivesAndPlay() {
    if (Archive_DoesHandleExist(g_harrierMusicArchiveHandle)) {
        if (Archive_HasPendingRead(g_harrierMusicArchiveHandle))
            return;
        g_harrierMusicBufferIndex = g_harrierMusicBufferIndex == 0 ? 1 : 0;
        const std::size_t current = static_cast<std::size_t>(g_harrierMusicBufferIndex);
        if (g_harrierMusicBuffers[current]) {
            if (g_harrierMusicVoiceIds[current] != kInvalidVoiceId)
                StopSourceVoiceAndFlush(g_harrierMusicVoiceIds[current]);
            std::free(g_harrierMusicBuffers[current]);
            g_harrierMusicBuffers[current] = nullptr;
            g_harrierMusicBufferSizes[current] = 0;
        }
        ArchiveHandleEntry *const entry = Archive_GetHandle(g_harrierMusicArchiveHandle);
        g_harrierMusicBufferSizes[current] = entry ? entry->bufferedDataSize : 0;
        g_harrierMusicBuffers[current] = static_cast<std::byte *>(Archive_DetachBuffer(g_harrierMusicArchiveHandle));
        if (g_harrierMusicVoiceIds[current] == kInvalidVoiceId) {
            void *const playback = AllocateRawSoundEffectVoice(16, 2, false);
            g_harrierMusicVoiceIds[current] = RawSoundEffectVoiceId(playback);
        }
        if (g_harrierMusicVoiceIds[current] != kInvalidVoiceId && g_harrierMusicBuffers[current]) {
            ProcessWave_Wrapper_0(
                g_harrierMusicVoiceIds[current], g_harrierMusicBuffers[current], static_cast<std::size_t>(g_harrierMusicBufferSizes[current]));
        }
        ReadFile_0(0, &g_harrierMusicArchiveHandle);
        g_harrierMusicArchiveHandle = g_invalidArchiveHandle;
        SetMusicVoiceVolume(g_harrierMusicVoiceIds[current], 0.0f);
        g_harrierMusicCrossfadeFrame = 0;
    }
    if (g_harrierMusicCrossfadeFrame > 1) {
        const std::size_t previous = g_harrierMusicBufferIndex == 0 ? 1U : 0U;
        ReleaseVoice(g_harrierMusicVoiceIds[previous]);
        g_harrierMusicCrossfadeFrame = -1;
        return;
    }
    if (g_harrierMusicCrossfadeFrame < 0)
        return;
    const float fade = static_cast<float>(g_harrierMusicCrossfadeFrame);
    const std::size_t current = static_cast<std::size_t>(g_harrierMusicBufferIndex);
    const std::size_t previous = current == 0 ? 1U : 0U;
    SetMusicVoiceVolume(g_harrierMusicVoiceIds[current], fade * kHarrierMusicGain);
    SetMusicVoiceVolume(g_harrierMusicVoiceIds[previous], (1.0f - fade) * kHarrierMusicGain);
    ++g_harrierMusicCrossfadeFrame;
}

void Harrier_UpdateAudioCommands() {
    if (g_harrierPaused)
        return;
    const std::uint8_t command = g_harrierMemory[kHarrierAudioCommandOffset];
    if (command != 0) {
        if (kHarrierCommandTypes[command] == 1) {
            CreateAndStartPlayingSourceVoiceForHarrier(kHarrierAudioFiles[command]);
        } else if (kHarrierCommandTypes[command] == 2 && command != g_harrierMusicFileIndex) {
            g_harrierMusicFileIndex = command;
            HarrierLoadFileFromPath(kHarrierMusicFiles[command]);
        }
        return;
    }
    WaitForAndCloseMusicRead();
    for (std::uint32_t voiceId : g_harrierMusicVoiceIds) {
        if (voiceId != kInvalidVoiceId)
            StopSourceVoiceAndFlush(voiceId);
    }
    g_harrierMusicFileIndex = 0;
}

void Harrier_ReleaseSoundEffects() {
    if (!g_harrierSfxSlots) {
        g_harrierSfxCount = 0;
        return;
    }
    for (std::uint32_t index = 0; index < g_harrierSfxCount; ++index) {
        HarrierStopAndResetVoiceSlot(index);
        std::free(g_harrierSfxSlots[index].fileData);
        g_harrierSfxSlots[index].fileData = nullptr;
        g_harrierSfxSlots[index].fileSize = 0;
    }
    std::free(g_harrierSfxSlots);
    g_harrierSfxSlots = nullptr;
    g_harrierSfxCount = 0;
}

void CleanupAudioSources() {
    WaitForAndCloseMusicRead();
    for (std::size_t index = 0; index < g_harrierMusicVoiceIds.size(); ++index) {
        ReleaseVoice(g_harrierMusicVoiceIds[index]);
        std::free(g_harrierMusicBuffers[index]);
        g_harrierMusicBuffers[index] = nullptr;
        g_harrierMusicBufferSizes[index] = 0;
    }
    g_harrierMusicCrossfadeFrame = -1;
}

} // namespace shenmue::s1
