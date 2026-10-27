#include "s2/areas/game/outrun/sound.hpp"

#include "s2/areas/game/arcade/audio_host.hpp"
#include "s2/areas/game/arcade/sound.hpp"
#include "s2/areas/game/outrun/board.hpp"
#include "s1/archive_data.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/settings.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace shenmue::s2 {
namespace {

struct EngineVoice {
    std::uint32_t voiceId;
    float gain;
    std::uint32_t mode;
};
static_assert(sizeof(EngineVoice) == 12);
static_assert(offsetof(EngineVoice, gain) == 4);
static_assert(offsetof(EngineVoice, mode) == 8);
std::array<EngineVoice, 6> engineVoices{};
std::array<EngineVoice, 4> effectVoices{};
std::array<std::int32_t, 6> previousProfiles{};
std::array<std::uint32_t, 4> previousEffects{};
std::array<std::uint8_t, 256> deferredCommands{};
std::array<std::byte, 0x10000> engineSamples{};
std::uint16_t engineInputWord{};
std::int32_t previousEngineInput{};
std::uint8_t alternateEngine{}, enginePaused{};

constexpr std::array<std::uint32_t, 256> commandTypes{
    2,
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
    2,
    2,
    0,
    1,
    2,
    1,
    0,
    0,
    0,
    1,
    1,
    0,
    1,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    0,
    1,
    0,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    2,
    2,
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
};
constexpr std::array<std::uint16_t, 256> soundIndices{
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    0,
    65535,
    1,
    65535,
    65535,
    65535,
    2,
    65535,
    65535,
    3,
    65535,
    4,
    5,
    65535,
    6,
    65535,
    7,
    8,
    65535,
    65535,
    65535,
    9,
    65535,
    10,
    65535,
    11,
    12,
    13,
    14,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
    65535,
};
constexpr std::array<std::int8_t, 32> gainTable{
    1,
    1,
    1,
    2,
    2,
    2,
    3,
    3,
    4,
    5,
    6,
    7,
    9,
    10,
    12,
    14,
    16,
    20,
    24,
    28,
    32,
    40,
    48,
    56,
    56,
    52,
    48,
    42,
    36,
    28,
    22,
    16,
};
constexpr std::array<std::int8_t, 16> panTable{
    16,
    13,
    8,
    0,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    0,
    8,
    13,
};
constexpr std::array<const char *, 15> soundFiles{
    "XOUT\\sound\\sfx\\00000000.wav",
    "XOUT\\sound\\sfx\\00000001.wav",
    "XOUT\\sound\\sfx\\00000002_edited.wav",
    "XOUT\\sound\\sfx\\00000003.wav",
    "XOUT\\sound\\sfx\\00000004.wav",
    "XOUT\\sound\\sfx\\00000005.wav",
    "XOUT\\sound\\sfx\\00000006.wav",
    "XOUT\\sound\\sfx\\00000007.wav",
    "XOUT\\sound\\sfx\\00000008.wav",
    "XOUT\\sound\\sfx\\00000009.wav",
    "XOUT\\sound\\sfx\\0000000a.wav",
    "XOUT\\sound\\sfx\\0000000b.wav",
    "XOUT\\sound\\sfx\\0000000c.wav",
    "XOUT\\sound\\sfx\\0000000d.wav",
    "XOUT\\sound\\sfx\\0000000e_edited.wav",
};
constexpr std::array<const char *, 256> MakeMusicFiles() {
    std::array<const char *, 256> files{};
    files[0x81] = "XOUT\\sound\\outrun_81.wav";
    files[0x82] = "XOUT\\sound\\outrun_82.wav";
    files[0x85] = "XOUT\\sound\\outrun_85.wav";
    files[0x9a] = "XOUT\\sound\\outrun_9a.wav";
    files[0xa4] = "XOUT\\sound\\outrun_a4.wav";
    files[0xa5] = "XOUT\\sound\\outrun_a5.wav";
    return files;
}
constexpr std::array<const char *, 256> musicFiles = MakeMusicFiles();
struct EngineProfile {
    std::uint8_t startLow, startHigh, endPage, gain, pitch;
};
static_assert(sizeof(EngineProfile) == 5);
constexpr std::array<EngineProfile, 83> engineProfiles{
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 63, .pitch = 80},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 63, .pitch = 81},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 63, .pitch = 82},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 62, .pitch = 83},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 62, .pitch = 84},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 62, .pitch = 85},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 61, .pitch = 86},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 61, .pitch = 87},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 61, .pitch = 88},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 60, .pitch = 89},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 60, .pitch = 90},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 60, .pitch = 91},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 59, .pitch = 92},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 59, .pitch = 93},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 59, .pitch = 94},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 58, .pitch = 95},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 58, .pitch = 96},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 58, .pitch = 97},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 57, .pitch = 98},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 57, .pitch = 99},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 57, .pitch = 100},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 56, .pitch = 101},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 56, .pitch = 102},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 56, .pitch = 103},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 55, .pitch = 104},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 55, .pitch = 105},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 55, .pitch = 106},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 54, .pitch = 107},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 54, .pitch = 108},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 54, .pitch = 109},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 53, .pitch = 110},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 53, .pitch = 111},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 53, .pitch = 112},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 52, .pitch = 113},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 52, .pitch = 114},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 52, .pitch = 115},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 51, .pitch = 116},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 51, .pitch = 117},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 51, .pitch = 118},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 50, .pitch = 119},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 50, .pitch = 120},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 50, .pitch = 121},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 49, .pitch = 122},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 49, .pitch = 123},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 49, .pitch = 124},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 48, .pitch = 125},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 48, .pitch = 126},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 48, .pitch = 127},
    EngineProfile{.startLow = 74, .startHigh = 0, .endPage = 5, .gain = 47, .pitch = 128},
    EngineProfile{.startLow = 115, .startHigh = 1, .endPage = 6, .gain = 45, .pitch = 129},
    EngineProfile{.startLow = 123, .startHigh = 2, .endPage = 7, .gain = 44, .pitch = 128},
    EngineProfile{.startLow = 123, .startHigh = 2, .endPage = 7, .gain = 43, .pitch = 129},
    EngineProfile{.startLow = 234, .startHigh = 4, .endPage = 9, .gain = 42, .pitch = 128},
    EngineProfile{.startLow = 155, .startHigh = 8, .endPage = 13, .gain = 40, .pitch = 129},
    EngineProfile{.startLow = 4, .startHigh = 12, .endPage = 16, .gain = 38, .pitch = 125},
    EngineProfile{.startLow = 4, .startHigh = 12, .endPage = 16, .gain = 37, .pitch = 126},
    EngineProfile{.startLow = 4, .startHigh = 12, .endPage = 16, .gain = 35, .pitch = 127},
    EngineProfile{.startLow = 4, .startHigh = 12, .endPage = 16, .gain = 34, .pitch = 128},
    EngineProfile{.startLow = 49, .startHigh = 14, .endPage = 18, .gain = 32, .pitch = 127},
    EngineProfile{.startLow = 49, .startHigh = 14, .endPage = 18, .gain = 31, .pitch = 128},
    EngineProfile{.startLow = 138, .startHigh = 21, .endPage = 25, .gain = 29, .pitch = 125},
    EngineProfile{.startLow = 138, .startHigh = 21, .endPage = 25, .gain = 28, .pitch = 126},
    EngineProfile{.startLow = 138, .startHigh = 21, .endPage = 25, .gain = 26, .pitch = 127},
    EngineProfile{.startLow = 138, .startHigh = 21, .endPage = 25, .gain = 25, .pitch = 128},
    EngineProfile{.startLow = 186, .startHigh = 24, .endPage = 28, .gain = 24, .pitch = 124},
    EngineProfile{.startLow = 186, .startHigh = 24, .endPage = 28, .gain = 22, .pitch = 125},
    EngineProfile{.startLow = 186, .startHigh = 24, .endPage = 28, .gain = 21, .pitch = 126},
    EngineProfile{.startLow = 186, .startHigh = 24, .endPage = 28, .gain = 19, .pitch = 127},
    EngineProfile{.startLow = 186, .startHigh = 24, .endPage = 28, .gain = 18, .pitch = 128},
    EngineProfile{.startLow = 208, .startHigh = 28, .endPage = 32, .gain = 16, .pitch = 124},
    EngineProfile{.startLow = 208, .startHigh = 28, .endPage = 32, .gain = 15, .pitch = 125},
    EngineProfile{.startLow = 208, .startHigh = 28, .endPage = 32, .gain = 13, .pitch = 126},
    EngineProfile{.startLow = 208, .startHigh = 28, .endPage = 32, .gain = 12, .pitch = 127},
    EngineProfile{.startLow = 208, .startHigh = 28, .endPage = 32, .gain = 10, .pitch = 128},
    EngineProfile{.startLow = 52, .startHigh = 30, .endPage = 33, .gain = 9, .pitch = 129},
    EngineProfile{.startLow = 69, .startHigh = 31, .endPage = 34, .gain = 7, .pitch = 129},
    EngineProfile{.startLow = 79, .startHigh = 32, .endPage = 35, .gain = 6, .pitch = 130},
    EngineProfile{.startLow = 142, .startHigh = 33, .endPage = 36, .gain = 4, .pitch = 126},
    EngineProfile{.startLow = 142, .startHigh = 33, .endPage = 36, .gain = 3, .pitch = 127},
    EngineProfile{.startLow = 142, .startHigh = 33, .endPage = 36, .gain = 2, .pitch = 128},
    EngineProfile{.startLow = 168, .startHigh = 34, .endPage = 37, .gain = 2, .pitch = 129},
    EngineProfile{.startLow = 168, .startHigh = 34, .endPage = 37, .gain = 2, .pitch = 130},
    EngineProfile{.startLow = 168, .startHigh = 34, .endPage = 37, .gain = 2, .pitch = 131},
};

float ClampGain(float value) {
    if (std::fmax(0.0f, value) > 1.0f)
        return 1.0f;
    return value >= 0.0f ? value : 0.0f;
}
float PitchRatio(double frequency) {
    double value = std::fmax(100.0, frequency);
    if (value > 100000.0)
        value = 100000.0;
    const std::uint32_t integer = static_cast<std::uint32_t>(static_cast<std::int64_t>(value));
    return static_cast<float>(integer) / 8000.0f;
}
void SetMatrix(std::uint32_t voiceId, const float *matrix) {
    s1::SetRawSoundEffectOutputMatrix(s1::GetRawSoundEffectVoice(voiceId), matrix, 1, 6);
}
void SetReverb(std::uint32_t voiceId, float value) {
    s1::SetOutputMatrixVolumeAndChannels(s1::GetRawSoundEffectVoice(voiceId), value, 1);
}
void SetPitch(std::uint32_t voiceId, float value) {
    s1::SetRawSoundEffectFrequencyRatio(s1::GetRawSoundEffectVoice(voiceId), value);
}
void SetGain(const EngineVoice &voice) {
    s1::SetRawSoundEffectVolume(s1::GetRawSoundEffectVoice(voice.voiceId), voice.gain * 0.8f * s1::g_applicationSettings.savedMusicVolume);
}
void SubmitEngine(EngineVoice &voice, std::uint32_t begin, std::uint32_t end, bool looped) {
    SubmitArcadePcmVoice(voice.voiceId, 1, 8000, engineSamples.data(), 0x10000, begin, end, looped);
}
void ProcessSoundCommand(std::uint8_t command) {
    if (command == 0xff) {
        StopArcadeMusic();
        return;
    }
    while (commandTypes[command] != 1) {
        if (commandTypes[command] != 2)
            return;
        if (command) {
            QueueArcadeMusic(musicFiles[command]);
            return;
        }
        StopArcadeMusic();
        command = 0x93;
    }
    if (deferredCommands[command])
        return;
    if (command == 0x8b) {
        StopArcadeSoundEffect(2);
        deferredCommands[0x8a] = 0;
    } else if (command == 0x8e) {
        StopArcadeSoundEffect(3);
        deferredCommands[0x8d] = 0;
    } else if (command == 0xa1) {
        StopArcadeSoundEffect(14);
        deferredCommands[0xa0] = 0;
    }
    if (command == 0x93) {
        for (std::uint32_t index = 0; index < 256; ++index) {
            StopArcadeSoundEffect(soundIndices[index]);
            deferredCommands[index] = 0;
        }
    } else {
        const std::uint32_t index = soundIndices[command];
        if (IsArcadeSoundEffectActive(index)) {
            StopArcadeSoundEffect(index);
            deferredCommands[command] = 1;
        } else {
            const std::uint8_t bit = command + 118;
            const bool looped = bit <= 22 && ((4194313u >> bit) & 1);
            PlayArcadeSoundEffect(index, !looped);
            deferredCommands[command] = 0;
        }
    }
}
void UpdateEffect(std::uint32_t index, std::uint8_t value) {
    if (previousEffects[index] == value)
        return;
    previousEffects[index] = value;
    EngineVoice &voice = effectVoices[index];
    const std::uint32_t gainIndex = value >> 3;
    const std::uint32_t pan = value & 7;
    std::array<float, 6> matrix{};
    if (gainIndex) {
        const std::int32_t gain = gainTable[gainIndex - 1];
        matrix[0] = ClampGain(static_cast<float>((gain * panTable[pan]) >> 4) / 255.0f);
        matrix[1] = ClampGain(static_cast<float>((gain * panTable[pan + 8]) >> 4) / 255.0f);
        SetMatrix(voice.voiceId, matrix.data());
        voice.gain = 1.0f;
        const std::uint32_t pitch = ((value & 1) + 3) << 5;
        if (gainIndex < 22) {
            SetPitch(voice.voiceId, PitchRatio(static_cast<double>(pitch) * 0.0078125 * 15600.0));
        } else {
            constexpr std::array<std::int8_t, 12> offsets{0, 2, 4, 4, 0, -8, -8, -8, -8, -8, -8, -8};
            const std::int32_t offset = offsets[gainIndex - 22] / 128;
            SetPitch(voice.voiceId, PitchRatio(static_cast<double>(pitch) + offset * 15600));
        }
    } else {
        SetMatrix(voice.voiceId, matrix.data());
        voice.gain = 1.0f;
    }
}
void SetSpecialEngine() {
    constexpr std::array<std::array<float, 6>, 3> matrices{std::array<float, 6>{0.0f, 0.125490203499794f, 0.0f, 0.0f, 0.0f, 0.0f},
        std::array<float, 6>{0.125490203499794f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        std::array<float, 6>{0.062745101749897f, 0.062745101749897f, 0.0f, 0.0f, 0.0f, 0.0f}};
    constexpr float pitches[]{0.9750000238418579f, 0.9901250004768372f, 1.0053750276565552f};
    for (std::uint32_t index = 0; index < 6; ++index) {
        EngineVoice &voice = engineVoices[index];
        if (voice.mode != 3) {
            s1::StopSourceVoiceAndFlush(voice.voiceId);
            voice.mode = 3;
            voice.gain = 0.0f;
        } else if (s1::RawSoundEffectVoiceIsActive(s1::GetRawSoundEffectVoice(voice.voiceId))) {
            continue;
        }
        SetMatrix(voice.voiceId, matrices[index / 2].data());
        SetPitch(voice.voiceId, pitches[index / 2]);
        SubmitEngine(voice, 13824, 21760, false);
        voice.gain = 0.05f;
    }
}
void UpdateEngineInput() {
    if (engineInputWord == previousEngineInput)
        return;
    previousEngineInput = engineInputWord;
    engineInputWord = (engineInputWord >> 5) & 0x1ff;
    const std::uint16_t input = engineInputWord;
    if (!input) {
        const std::array<float, 6> matrix{};
        for (EngineVoice &voice : engineVoices) {
            SetMatrix(voice.voiceId, matrix.data());
            SetReverb(voice.voiceId, 0.0f);
            voice.gain = 0.0f;
            voice.mode = 0;
        }
        return;
    }
    if (alternateEngine && input >= 250) {
        SetSpecialEngine();
        return;
    }
    for (std::uint32_t index = 0; index < 6; ++index) {
        EngineVoice &voice = engineVoices[index];
        std::array<float, 6> matrix{};
        float reverb;
        if (input < 48 || index < 4) {
            const std::int32_t profileIndex = input - 1 <= 82 ? input - 1 : 1;
            const EngineProfile &profile = engineProfiles[static_cast<std::size_t>(profileIndex)];
            if (voice.mode != 1 || previousProfiles[index] != profileIndex) {
                voice.mode = 1;
                if (s1::RawSoundEffectVoiceIsActive(s1::GetRawSoundEffectVoice(voice.voiceId)))
                    s1::StopSourceVoiceAndFlush(voice.voiceId);
                SubmitEngine(voice, profile.startLow | (profile.startHigh << 8), (profile.endPage << 8) + 256, true);
            }
            previousProfiles[index] = profileIndex;
            std::uint32_t gain = (63u * profile.gain) >> 6;
            if (gain >= 63)
                gain = 63;
            reverb = std::fmax(0.0f, static_cast<float>(gain) / 255.0f);
            matrix[(index & 1) ? 0 : 1] = ClampGain(static_cast<float>(gain) / 255.0f);
            SetMatrix(voice.voiceId, matrix.data());
            const double frequency = (index & 1) ? static_cast<double>(profile.pitch) * 0.0078125 * 15600.0 : profile.pitch;
            SetPitch(voice.voiceId, PitchRatio(frequency));
        } else {
            const std::uint16_t step = (input - 48) >> 1;
            std::uint16_t gain = static_cast<std::uint16_t>(63 * (step + 1)) >> 6;
            if (gain >= 63)
                gain = 63;
            else if (gain < 23)
                gain = 23;
            std::uint16_t pitch = step + 64;
            if (pitch >= 255)
                pitch = 255;
            if (voice.mode != 2) {
                voice.mode = 2;
                SubmitEngine(voice, 32898, 34560, true);
            }
            matrix[0] = ClampGain(static_cast<float>(gain >> ((index & 1) ? 1 : 2)) / 255.0f);
            matrix[1] = ClampGain(static_cast<float>(gain >> ((index & 1) ? 2 : 1)) / 255.0f);
            SetMatrix(voice.voiceId, matrix.data());
            const double frequency = (index & 1) ? static_cast<double>(pitch) * 0.0078125 * 15600.0 : pitch;
            SetPitch(voice.voiceId, PitchRatio(frequency));
            reverb = std::fmax(0.0f, static_cast<float>(gain) / 255.0f);
        }
        if (reverb > 1.0f)
            reverb = 1.0f;
        SetReverb(voice.voiceId, reverb * 0.2f * s1::g_applicationSettings.savedMusicVolume);
        voice.gain = 1.0f;
    }
    for (EngineVoice &voice : engineVoices) {
        if (!s1::RawSoundEffectVoiceIsActive(s1::GetRawSoundEffectVoice(voice.voiceId)) && voice.mode == 3)
            SubmitEngine(voice, 13824, 21760, true);
    }
}

} // namespace

void InitializeOutRunSound() {
    g_outRunSoundCommandCount = 0;
    deferredCommands.fill(0);
    InitializeArcadeSoundEffects(soundFiles.data(), 15);
    ResetArcadeMusic();
    const std::uint64_t size = s1::ArchiveData_GetFileSize("XOUT\\sound\\eng.dat");
    void *const buffer = std::malloc(static_cast<std::size_t>(size));
    s1::ArchiveData_Read("XOUT\\sound\\eng.dat", 0, buffer, size);
    engineSamples.fill(std::byte{});
    std::memcpy(engineSamples.data(), buffer, static_cast<std::size_t>(size));
    previousEngineInput = -1;
    previousProfiles.fill(0);
    previousEffects.fill(0);
    alternateEngine = 0;
    enginePaused = 0;
    const std::array<float, 6> matrix{};
    for (EngineVoice &voice : engineVoices) {
        voice.voiceId = s1::RawSoundEffectVoiceId(s1::AllocateRawSoundEffectVoice(8, 1, true));
        voice.gain = 0.0f;
        voice.mode = 0;
        SetMatrix(voice.voiceId, matrix.data());
        s1::SetRawSoundEffectVolume(s1::GetRawSoundEffectVoice(voice.voiceId), 0.0f);
    }
    for (EngineVoice &voice : effectVoices) {
        voice.voiceId = s1::RawSoundEffectVoiceId(s1::AllocateRawSoundEffectVoice(8, 1, true));
        voice.gain = 0.0f;
        SubmitEngine(voice, 32898, 34560, true);
        SetMatrix(voice.voiceId, matrix.data());
        SetReverb(voice.voiceId, 0.0f);
        s1::SetRawSoundEffectVolume(s1::GetRawSoundEffectVoice(voice.voiceId), 0.0f);
    }
}
void UpdateOutRunSound(const GameArcadeRuntime &runtime) {
    if (runtime.paused) {
        if (!enginePaused) {
            for (EngineVoice &voice : engineVoices)
                s1::SetRawSoundEffectPaused(s1::GetRawSoundEffectVoice(voice.voiceId), true);
            for (EngineVoice &voice : effectVoices)
                s1::SetRawSoundEffectPaused(s1::GetRawSoundEffectVoice(voice.voiceId), true);
            enginePaused = 1;
        }
    } else {
        if (enginePaused) {
            for (EngineVoice &voice : engineVoices)
                s1::StartRawSoundEffectPlayback(s1::GetRawSoundEffectVoice(voice.voiceId));
            for (EngineVoice &voice : effectVoices)
                s1::StartRawSoundEffectPlayback(s1::GetRawSoundEffectVoice(voice.voiceId));
            enginePaused = 0;
        }
        for (std::uint32_t command = 0; command < 256; ++command) {
            if (deferredCommands[command] && !IsArcadeSoundEffectActive(soundIndices[command])) {
                PlayArcadeSoundEffect(soundIndices[command], true);
                deferredCommands[command] = 0;
            }
        }
        for (std::uint32_t index = 0; index < g_outRunSoundCommandCount; ++index) {
            const OutRunSoundCommand &command = g_outRunSoundCommands[index];
            if (command.type == 0) {
                if (((static_cast<std::uint8_t>(command.value + 127)) & 0xfa) == 0 && command.value != 0x86)
                    alternateEngine = 1;
                else if (command.value == 0x95)
                    alternateEngine = 0;
                ProcessSoundCommand(command.value);
            } else if (command.type == 1) {
                engineInputWord = (engineInputWord & 0xff) | (command.value << 8);
            } else if (command.type == 2) {
                engineInputWord = (engineInputWord & 0xff00) | command.value;
                UpdateEngineInput();
            } else if (command.type >= 4 && command.type <= 7) {
                UpdateEffect(command.type - 4, command.value);
            }
        }
        g_outRunSoundCommandCount = 0;
    }
    for (const EngineVoice &voice : engineVoices)
        SetGain(voice);
    for (const EngineVoice &voice : effectVoices)
        SetGain(voice);
}
void ReleaseOutRunSound() {
    ReleaseArcadeSoundEffects();
    ReleaseArcadeMusic();
    for (const EngineVoice &voice : engineVoices) {
        s1::StopSourceVoiceAndFlush(voice.voiceId);
        s1::ReleaseRawSoundEffectReservation(s1::GetRawSoundEffectVoice(voice.voiceId));
    }
    for (const EngineVoice &voice : effectVoices) {
        s1::StopSourceVoiceAndFlush(voice.voiceId);
        s1::ReleaseRawSoundEffectReservation(s1::GetRawSoundEffectVoice(voice.voiceId));
    }
}

} // namespace shenmue::s2
