#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::platform::audio {

struct Voice {
    std::uint32_t id;
    std::uint32_t sampleRate;
    std::uint16_t channels;
    std::uint16_t bitsPerSample;
};
struct PcmBuffer {
    const std::byte *data;
    std::uint32_t bytes;
    std::uint32_t sampleRate;
    std::uint16_t channels;
    std::uint16_t bitsPerSample;
    std::uint32_t playBegin{};
    std::uint32_t playLength{};
    std::uint32_t loopBegin{};
    std::uint32_t loopLength{};
    bool looped{};
    bool floatingPoint{};
    bool append{};
};

bool Initialize();
bool IsInitialized();
void SetEnabled(bool enabled);
Voice *CreateVoice(std::uint16_t channels, std::uint16_t bitsPerSample = 16);
void DestroyVoice(Voice *voice);
bool Submit(Voice *voice, const PcmBuffer &buffer);
bool SubmitWave(Voice *voice, const void *format, const std::byte *audio, std::uint32_t bytes, std::uint32_t loopBegin = 0, std::uint32_t loopLength = 0,
    bool looped = false);
bool Start(Voice *voice);
void Stop(Voice *voice);
void Pause(Voice *voice, bool paused);
bool HasBuffer(const Voice *voice);
bool IsComplete(const Voice *voice);
bool GetPosition(const Voice *voice, std::uint64_t &samples, std::uint32_t &sampleRate);
void SetVolume(Voice *voice, float volume);
void SetFrequencyRatio(Voice *voice, float ratio);
void SetSampleRate(Voice *voice, std::uint32_t sampleRate);
bool SetLowPass(Voice *voice, float normalizedFrequency, float oneOverQ);
bool SetOutputMatrix(Voice *voice, const float *matrix, std::uint32_t sourceChannels, std::uint32_t outputChannels);
void GetOutputMatrix(const Voice *voice, float *matrix, std::uint32_t sourceChannels, std::uint32_t outputChannels);

} // namespace shenmue::platform::audio
