#include "s2/areas/game_arcade_sound.hpp"

#include "s1/archive_data.hpp"
#include "s1/archive_handles.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/modules/harrier.hpp"
#include "s1/modules/harrier_audio.hpp"

#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <thread>

namespace shenmue::s2 {
namespace {

constexpr std::uint32_t kInvalidVoiceId = 0xffffffffu;
struct ArcadeSoundSlot {
    std::byte *fileData;
    std::uint64_t fileSize;
    std::uint32_t voiceId;
    std::uint8_t paused;
    std::uint8_t reserved[3];
};
static_assert(sizeof(ArcadeSoundSlot) == 0x18);
static_assert(offsetof(ArcadeSoundSlot, fileSize) == 8);
static_assert(offsetof(ArcadeSoundSlot, voiceId) == 0x10);
static_assert(offsetof(ArcadeSoundSlot, paused) == 0x14);

ArcadeSoundSlot *soundSlots{};
std::int32_t soundCount{};
std::int32_t musicBufferIndex{};
std::array<std::uint32_t, 2> musicVoiceIds{};
std::array<std::byte *, 2> musicBuffers{};
std::array<std::uint64_t, 2> musicBufferSizes{};
std::uint32_t musicArchiveHandle{};
std::int32_t musicFadeFrame{};

void StopAndRelease(std::uint32_t voiceId) {
    s1::StopSourceVoiceAndFlush(voiceId);
    s1::ReleaseRawSoundEffectReservation(s1::GetRawSoundEffectVoice(voiceId));
}
void ClosePendingMusic() {
    if (!s1::Archive_DoesHandleExist(musicArchiveHandle))
        return;
    while (s1::Archive_HasPendingRead(musicArchiveHandle))
        std::this_thread::yield();
    s1::ReadFile_0(0, &musicArchiveHandle);
    musicArchiveHandle = s1::g_invalidArchiveHandle;
}
void SetVolume(std::uint32_t voiceId, float value) {
    s1::SetRawSoundEffectVolume(s1::GetRawSoundEffectVoice(voiceId), value);
}
float MusicVolume() {
    return s1::g_applicationSettings.savedMusicVolume;
}
float SoundVolume() {
    return s1::g_applicationSettings.savedSfxVolume;
}

} // namespace

void InitializeArcadeSoundEffects(const char *const *files, std::int32_t count) {
    soundCount = count;
    soundSlots = static_cast<ArcadeSoundSlot *>(std::malloc(static_cast<std::size_t>(count) * sizeof(ArcadeSoundSlot)));
    std::memset(soundSlots, 0, static_cast<std::size_t>(count) * sizeof(ArcadeSoundSlot));
    for (std::int32_t index = 0; index < count; ++index) {
        ArcadeSoundSlot &slot = soundSlots[index];
        const std::uint64_t size = s1::ArchiveData_GetFileSize(files[index]);
        std::byte *const buffer = static_cast<std::byte *>(std::malloc(static_cast<std::size_t>(size)));
        if (s1::ArchiveData_Read(files[index], 0, buffer, size)) {
            slot.fileData = buffer;
            if (buffer)
                slot.fileSize = size;
        } else {
            std::free(buffer);
        }
        slot.paused = 0;
        slot.voiceId = kInvalidVoiceId;
    }
}
void ResetArcadeMusic() {
    musicBufferIndex = 0;
    musicVoiceIds[0] = kInvalidVoiceId;
    musicVoiceIds[1] = kInvalidVoiceId;
    musicArchiveHandle = s1::g_invalidArchiveHandle;
    musicFadeFrame = -1;
}
bool IsArcadeSoundEffectActive(std::uint32_t index) {
    if (index >= static_cast<std::uint32_t>(soundCount))
        return false;
    return s1::RawSoundEffectVoiceIsActive(s1::GetRawSoundEffectVoice(soundSlots[index].voiceId));
}
void PlayArcadeSoundEffect(std::uint32_t index, bool retrigger) {
    if (index >= static_cast<std::uint32_t>(soundCount))
        return;
    ArcadeSoundSlot &slot = soundSlots[index];
    void *const previous = s1::GetRawSoundEffectVoice(slot.voiceId);
    const bool active = s1::RawSoundEffectVoiceIsActive(previous);
    if (!retrigger && active)
        return;
    if (retrigger && active)
        s1::StopSourceVoiceAndFlush(slot.voiceId);
    if (slot.voiceId != kInvalidVoiceId)
        s1::ReleaseRawSoundEffectReservation(previous);
    void *const playback = s1::AllocateRawSoundEffectVoice(16, 2, false);
    slot.voiceId = s1::RawSoundEffectVoiceId(playback);
    if (retrigger) {
        s1::ProcessWave_Wrapper_0(slot.voiceId, slot.fileData, static_cast<std::size_t>(slot.fileSize));
        s1::SetOutputAudioMatrix(playback, 2);
        SetVolume(slot.voiceId, 0.5f * SoundVolume());
    } else {
        s1::CreateAndStartPlayingSourceVoice(slot.voiceId, slot.fileData, static_cast<std::size_t>(slot.fileSize));
        SetVolume(slot.voiceId, 0.5f * SoundVolume());
        s1::SetOutputAudioMatrix(playback, 2);
    }
}
void StopArcadeSoundEffect(std::uint32_t index) {
    if (index >= static_cast<std::uint32_t>(soundCount))
        return;
    ArcadeSoundSlot &slot = soundSlots[index];
    if (slot.voiceId != kInvalidVoiceId)
        StopAndRelease(slot.voiceId);
    slot.voiceId = kInvalidVoiceId;
}
void PauseArcadeSoundEffects(bool paused) {
    for (std::int32_t index = 0; index < soundCount; ++index) {
        ArcadeSoundSlot &slot = soundSlots[index];
        if (slot.voiceId == kInvalidVoiceId)
            continue;
        void *const playback = s1::GetRawSoundEffectVoice(slot.voiceId);
        if (paused) {
            if (!slot.paused) {
                slot.paused = 1;
                s1::SetRawSoundEffectPaused(playback, true);
            }
        } else {
            SetVolume(slot.voiceId, 0.5f * SoundVolume());
            if (slot.paused) {
                slot.paused = 0;
                s1::StartRawSoundEffectPlayback(playback);
            } else if (!s1::RawSoundEffectVoiceIsActive(playback)) {
                StopAndRelease(slot.voiceId);
                slot.voiceId = kInvalidVoiceId;
            }
        }
    }
}
void StopArcadeSoundEffects() {
    for (std::int32_t index = 0; index < soundCount; ++index) {
        ArcadeSoundSlot &slot = soundSlots[index];
        if (slot.voiceId != kInvalidVoiceId)
            StopAndRelease(slot.voiceId);
        slot.voiceId = kInvalidVoiceId;
    }
}
void ReleaseArcadeSoundEffects() {
    for (std::int32_t index = 0; index < soundCount; ++index) {
        ArcadeSoundSlot &slot = soundSlots[index];
        if (slot.voiceId != kInvalidVoiceId)
            StopAndRelease(slot.voiceId);
        slot.voiceId = kInvalidVoiceId;
        if (slot.fileData)
            std::free(slot.fileData);
    }
    std::free(soundSlots);
    soundSlots = nullptr;
}
void QueueArcadeMusic(const char *path) {
    ClosePendingMusic();
    musicArchiveHandle = s1::Archive_AllocateHandle(0);
    if (!s1::Archive_DoesHandleExist(musicArchiveHandle))
        return;
    const std::uint64_t size = s1::ArchiveData_GetFileSize(path);
    void *const buffer = std::malloc(static_cast<std::size_t>(size));
    s1::ArchiveHandleEntry *const entry = s1::Archive_GetHandle(musicArchiveHandle);
    entry->platformHandle = -1;
    s1::Archive_BindBuffer(musicArchiveHandle, buffer, size, false);
    s1::Archive_QueueRead(musicArchiveHandle, 0, path, 0, size, 1);
}
void AdvanceArcadeMusic(std::int32_t frames) {
    if (s1::Archive_DoesHandleExist(musicArchiveHandle) && !s1::Archive_HasPendingRead(musicArchiveHandle)) {
        musicBufferIndex = musicBufferIndex == 0;
        const std::size_t current = static_cast<std::size_t>(musicBufferIndex);
        if (musicBuffers[current]) {
            s1::StopSourceVoiceAndFlush(musicVoiceIds[current]);
            std::free(musicBuffers[current]);
            musicBuffers[current] = nullptr;
        }
        musicBufferSizes[current] = s1::Archive_GetHandle(musicArchiveHandle)->bufferedDataSize;
        musicBuffers[current] = static_cast<std::byte *>(s1::Archive_DetachBuffer(musicArchiveHandle));
        if (musicVoiceIds[current] == kInvalidVoiceId) {
            musicVoiceIds[current] = s1::RawSoundEffectVoiceId(s1::AllocateRawSoundEffectVoice(16, 2, false));
        }
        s1::CreateAndStartPlayingSourceVoice(musicVoiceIds[current], musicBuffers[current], static_cast<std::size_t>(musicBufferSizes[current]));
        s1::SetOutputAudioMatrix(s1::GetRawSoundEffectVoice(musicVoiceIds[current]), 2);
        s1::ReadFile_0(0, &musicArchiveHandle);
        musicArchiveHandle = s1::g_invalidArchiveHandle;
        SetVolume(musicVoiceIds[current], 0.0f);
        musicFadeFrame = 0;
    }
    const std::size_t previous = musicBufferIndex == 0;
    if (musicFadeFrame > frames) {
        if (musicVoiceIds[previous] != kInvalidVoiceId) {
            StopAndRelease(musicVoiceIds[previous]);
            musicVoiceIds[previous] = kInvalidVoiceId;
        }
        musicFadeFrame = -1;
    } else if (musicFadeFrame > -1) {
        const float ratio = static_cast<float>(musicFadeFrame) / static_cast<float>(frames);
        SetVolume(musicVoiceIds[static_cast<std::size_t>(musicBufferIndex)], ratio * 0.7f * MusicVolume());
        if (musicVoiceIds[previous] != kInvalidVoiceId)
            SetVolume(musicVoiceIds[previous], (1.0f - ratio) * 0.7f * MusicVolume());
        ++musicFadeFrame;
    }
}
void FadeOutArcadeMusic(std::int32_t frames) {
    std::uint32_t &voiceId = musicVoiceIds[static_cast<std::size_t>(musicBufferIndex)];
    if (musicFadeFrame < frames) {
        if (musicFadeFrame > -1) {
            if (voiceId != kInvalidVoiceId) {
                float volume = 0.0f;
#if defined(_WIN32)
                IXAudio2SourceVoice *const voice = static_cast<IXAudio2SourceVoice *>(s1::GetRawSoundEffectVoice(voiceId));
                if (voice && s1::GetXAudioEffectFlagVal() != 1)
                    voice->GetVolume(&volume);
#endif
                SetVolume(voiceId, (volume - 0.7f / static_cast<float>(frames)) * MusicVolume());
            }
            ++musicFadeFrame;
        }
    } else {
        if (voiceId != kInvalidVoiceId) {
            StopAndRelease(voiceId);
            voiceId = kInvalidVoiceId;
        }
        musicFadeFrame = -1;
    }
}
void StopArcadeMusic() {
    ClosePendingMusic();
    for (std::uint32_t voiceId : musicVoiceIds) {
        if (voiceId != kInvalidVoiceId)
            s1::StopSourceVoiceAndFlush(voiceId);
    }
}
void ReleaseArcadeMusic() {
    ClosePendingMusic();
    for (std::size_t index = 0; index < musicVoiceIds.size(); ++index) {
        if (musicVoiceIds[index] != kInvalidVoiceId)
            StopAndRelease(musicVoiceIds[index]);
        if (musicBuffers[index]) {
            std::free(musicBuffers[index]);
            musicBuffers[index] = nullptr;
        }
    }
}

} // namespace shenmue::s2

namespace shenmue::s1 {
namespace {
constexpr std::uint32_t kHarrierSfxCount = 70;
constexpr std::uint16_t kNoSfx = std::numeric_limits<std::uint16_t>::max();
constexpr std::uint32_t kHarrierAudioResetOffset = 0xd0014;

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
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    1,
    2,
    2,
    2,
    2,
    1,
    0,
    0,
    2,
    2,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    1,
    1,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
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

} // namespace

std::int32_t g_harrierMusicFileIndex{-1};
void mallocHarrierSFX() {
    s2::InitializeArcadeSoundEffects(kHarrierSfxFiles.data(), kHarrierSfxCount);
}
void Harrier_ResetAudioForCoreInit() {
    s2::ResetArcadeMusic();
    g_harrierMusicFileIndex = -1;
    const std::uint32_t reset = 0xfdffffffu;
    std::memcpy(g_harrierMemory + kHarrierAudioResetOffset, &reset, sizeof(reset));
}
void CreateAndStartPlayingSourceVoiceForHarrier(std::uint32_t index) {
    s2::PlayArcadeSoundEffect(index, true);
}
void HarrierStopAndResetVoiceSlot(std::uint32_t index) {
    s2::StopArcadeSoundEffect(index);
}
void DoAudio(bool paused) {
    s2::PauseArcadeSoundEffects(paused);
}
void HarrierLoadFileFromPath(const char *path) {
    s2::QueueArcadeMusic(path);
}
void AudioFlushFromArchivesAndPlay() {
    s2::AdvanceArcadeMusic(1);
}
void Harrier_UpdateAudioCommands() {
    const std::uint8_t command = g_harrierMemory[kHarrierAudioCommandOffset];
    if (command != 0) {
        if (kHarrierCommandTypes[command] == 1) {
            s2::PlayArcadeSoundEffect(kHarrierAudioFiles[command], true);
        } else if (kHarrierCommandTypes[command] == 2 && command != g_harrierMusicFileIndex) {
            g_harrierMusicFileIndex = command;
            s2::QueueArcadeMusic(kHarrierMusicFiles[command]);
        }
    } else {
        s2::StopArcadeMusic();
        g_harrierMusicFileIndex = 0;
    }
}
void Harrier_ReleaseSoundEffects() {
    s2::ReleaseArcadeSoundEffects();
}
void CleanupAudioSources() {
    s2::ReleaseArcadeMusic();
}

} // namespace shenmue::s1
