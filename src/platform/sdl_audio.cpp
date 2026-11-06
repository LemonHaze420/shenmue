#include "platform/audio.hpp"
#include "api/audio.h"

#include <SDL.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

namespace shenmue::platform::audio {
namespace {
struct Block {
    std::vector<float> samples;
    double cursor{};
    std::uint32_t end{}, loopBegin{}, loopEnd{};
    bool looped{};
};
struct VoiceState {
    Voice voice;
    std::deque<Block> blocks;
    std::array<float, 12> matrix{};
    std::array<float, 2> z1{}, z2{};
    double played{};
    float volume{1}, ratio{1};
    float b0{1}, b1{}, b2{}, a1{}, a2{};
    bool playing{}, paused{}, complete{};
};
std::vector<std::unique_ptr<VoiceState>> voices;
std::mutex mutex;
SDL_AudioDeviceID device;
SDL_AudioSpec output;
bool enabled = true;

VoiceState *Find(const Voice *voice) {
    return voice && voice->id && voice->id <= voices.size() ? voices[voice->id - 1].get() : nullptr;
}

void Mix(void *, Uint8 *destination, int bytes) {
    auto *const samples = reinterpret_cast<float *>(destination);
    const std::size_t frames = static_cast<std::size_t>(bytes) / (2 * sizeof(float));
    std::fill(samples, samples + frames * 2, 0.0f);
    const std::lock_guard<std::mutex> lock(mutex);
    for (const auto &entry : voices) {
        if (!entry || !entry->playing || entry->paused)
            continue;
        VoiceState &state = *entry;
        const std::size_t channels = state.voice.channels;
        const double step = static_cast<double>(state.voice.sampleRate) * state.ratio / output.freq;
        for (std::size_t frame = 0; frame < frames; ++frame) {
            while (!state.blocks.empty() && state.blocks.front().cursor >= state.blocks.front().end) {
                Block &block = state.blocks.front();
                if (block.looped) {
                    block.cursor = block.loopBegin + std::fmod(block.cursor - block.loopBegin, block.loopEnd - block.loopBegin);
                    break;
                }
                const double remainder = block.cursor - block.end;
                state.blocks.pop_front();
                if (!state.blocks.empty())
                    state.blocks.front().cursor += remainder;
            }
            if (state.blocks.empty()) {
                state.playing = false;
                state.complete = true;
                break;
            }
            Block &block = state.blocks.front();
            const auto index = static_cast<std::uint32_t>(block.cursor);
            const float fraction = static_cast<float>(block.cursor - index);
            const std::uint32_t next = index + 1 < block.end ? index + 1 : block.looped ? block.loopBegin : index;
            std::array<float, 6> routed{};
            for (std::size_t channel = 0; channel < channels; ++channel) {
                const float current = block.samples[index * channels + channel];
                const float value = current + (block.samples[next * channels + channel] - current) * fraction;
                const float filtered = value * state.b0 + state.z1[channel];
                state.z1[channel] = value * state.b1 - state.a1 * filtered + state.z2[channel];
                state.z2[channel] = value * state.b2 - state.a2 * filtered;
                for (std::size_t target = 0; target < routed.size(); ++target)
                    routed[target] += filtered * state.matrix[target * channels + channel];
            }
            if (enabled) {
                samples[frame * 2] += state.volume * (routed[0] + 0.70710678f * (routed[2] + routed[3] + routed[4]));
                samples[frame * 2 + 1] += state.volume * (routed[1] + 0.70710678f * (routed[2] + routed[3] + routed[5]));
            }
            block.cursor += step;
            state.played += step;
            if (block.looped && block.cursor >= block.loopEnd)
                block.cursor = block.loopBegin + std::fmod(block.cursor - block.loopBegin, block.loopEnd - block.loopBegin);
        }
    }
}
} // namespace

bool Initialize() {
    const std::lock_guard<std::mutex> lock(mutex);
    if (device)
        return true;
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
        return false;
    SDL_AudioSpec desired{};
    desired.freq = 48000;
    desired.format = AUDIO_F32SYS;
    desired.channels = 2;
    desired.samples = 512;
    desired.callback = Mix;
    device = SDL_OpenAudioDevice(nullptr, 0, &desired, &output, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    if (!device)
        return false;
    std::atexit([] { if (device) SDL_CloseAudioDevice(device); });
    SDL_PauseAudioDevice(device, 0);
    return true;
}

bool IsInitialized() { return device != 0; }
void SetEnabled(bool value) { const std::lock_guard<std::mutex> lock(mutex); enabled = value; }

Voice *CreateVoice(std::uint16_t channels, std::uint16_t bitsPerSample) {
    if (!channels || channels > 2 || !Initialize())
        return nullptr;
    const std::lock_guard<std::mutex> lock(mutex);
    std::size_t index = 0;
    while (index < voices.size() && voices[index])
        ++index;
    auto state = std::make_unique<VoiceState>();
    state->voice = {.id = static_cast<std::uint32_t>(index + 1), .sampleRate = 44100, .channels = channels, .bitsPerSample = bitsPerSample};
    for (std::size_t target = 0; target < 6; ++target)
        for (std::size_t source = 0; source < channels; ++source)
            state->matrix[target * channels + source] = channels == 1 ? static_cast<float>(target == 2) : static_cast<float>(target == source);
    Voice *const result = &state->voice;
    if (index == voices.size())
        voices.push_back(std::move(state));
    else
        voices[index] = std::move(state);
    return result;
}

void DestroyVoice(Voice *voice) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (Find(voice))
        voices[voice->id - 1].reset();
}

bool Submit(Voice *voice, const PcmBuffer &buffer) {
    if (!voice || !buffer.data || !buffer.bytes || !buffer.sampleRate || buffer.channels != voice->channels ||
        (buffer.floatingPoint ? buffer.bitsPerSample != 32 :
            buffer.bitsPerSample != 8 && buffer.bitsPerSample != 16 && buffer.bitsPerSample != 24 && buffer.bitsPerSample != 32))
        return false;
    const std::uint32_t sampleBytes = buffer.bitsPerSample / 8, frameBytes = buffer.channels * sampleBytes;
    const std::uint32_t frames = buffer.bytes / frameBytes;
    const std::uint32_t length = buffer.playLength ? buffer.playLength : frames - std::min(frames, buffer.playBegin);
    if (buffer.bytes % frameBytes || buffer.playBegin >= frames || length > frames - buffer.playBegin)
        return false;
    Block block{.samples = std::vector<float>(static_cast<std::size_t>(frames) * buffer.channels), .cursor = static_cast<double>(buffer.playBegin),
        .end = buffer.playBegin + length, .loopBegin = buffer.loopBegin,
        .loopEnd = buffer.loopLength ? buffer.loopBegin + buffer.loopLength : buffer.playBegin + length,
        .looped = buffer.looped};
    if (block.looped && (block.loopBegin < buffer.playBegin || block.loopEnd <= block.loopBegin || block.loopEnd > block.end))
        return false;
    for (std::size_t index = 0; index < block.samples.size(); ++index) {
        const auto *const source = reinterpret_cast<const std::uint8_t *>(buffer.data) + index * sampleBytes;
        if (buffer.floatingPoint) {
            std::memcpy(&block.samples[index], source, sizeof(float));
            if (!std::isfinite(block.samples[index]))
                block.samples[index] = 0;
        } else if (sampleBytes == 1) {
            block.samples[index] = (static_cast<float>(*source) - 128) / 128;
        } else {
            std::uint32_t bits = 0;
            for (std::uint32_t byte = 0; byte < sampleBytes; ++byte)
                bits |= static_cast<std::uint32_t>(source[byte]) << (byte * 8);
            const std::int32_t value = static_cast<std::int32_t>(bits << (32 - buffer.bitsPerSample));
            block.samples[index] = static_cast<float>(value) / 2147483648.0f;
        }
    }
    const std::lock_guard<std::mutex> lock(mutex);
    VoiceState *const state = Find(voice);
    if (!state)
        return false;
    if (!buffer.append) {
        state->blocks.clear();
        state->played = 0;
        state->playing = false;
        state->paused = false;
        state->z1 = state->z2 = {};
    }
    state->voice.sampleRate = buffer.sampleRate;
    state->voice.bitsPerSample = buffer.bitsPerSample;
    state->complete = false;
    state->blocks.push_back(std::move(block));
    return true;
}

bool SubmitWave(Voice *voice, const void *format, const std::byte *data, std::uint32_t bytes, std::uint32_t loopBegin,
    std::uint32_t loopLength, bool looped) {
    if (!format)
        return false;
    shen_audio_wave_format wave{};
    std::memcpy(&wave, format, sizeof(wave));
    std::uint16_t tag = wave.format_tag;
    if (tag == 0xfffe && wave.extra_size >= 22)
        std::memcpy(&tag, static_cast<const std::byte *>(format) + 24, sizeof(tag));
    if (tag != 1 && tag != 3)
        return false;
    return Submit(voice, {.data = data, .bytes = bytes, .sampleRate = wave.sample_rate, .channels = wave.channels,
        .bitsPerSample = wave.bits_per_sample, .loopBegin = loopBegin, .loopLength = loopLength, .looped = looped, .floatingPoint = tag == 3});
}

bool Start(Voice *voice) {
    const std::lock_guard<std::mutex> lock(mutex);
    VoiceState *const state = Find(voice);
    if (!state || state->blocks.empty())
        return false;
    state->playing = true;
    state->paused = false;
    return true;
}

void Stop(Voice *voice) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (VoiceState *const state = Find(voice)) {
        state->playing = false;
        state->paused = false;
        state->blocks.clear();
        state->complete = false;
    }
}

void Pause(Voice *voice, bool paused) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (VoiceState *const state = Find(voice))
        state->paused = paused;
}

bool HasBuffer(const Voice *voice) {
    const std::lock_guard<std::mutex> lock(mutex);
    const VoiceState *const state = Find(voice);
    return state && !state->blocks.empty();
}

bool IsComplete(const Voice *voice) {
    const std::lock_guard<std::mutex> lock(mutex);
    const VoiceState *const state = Find(voice);
    return state && state->complete;
}

bool GetPosition(const Voice *voice, std::uint64_t &samples, std::uint32_t &sampleRate) {
    const std::lock_guard<std::mutex> lock(mutex);
    const VoiceState *const state = Find(voice);
    if (!state || state->complete || state->blocks.empty())
        return false;
    samples = static_cast<std::uint64_t>(state->played);
    sampleRate = state->voice.sampleRate;
    return true;
}

void SetVolume(Voice *voice, float value) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (VoiceState *const state = Find(voice); state && std::isfinite(value) && value >= 0)
        state->volume = value;
}

void SetFrequencyRatio(Voice *voice, float value) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (VoiceState *const state = Find(voice); state && std::isfinite(value) && value > 0)
        state->ratio = value;
}

void SetSampleRate(Voice *voice, std::uint32_t rate) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (VoiceState *const state = Find(voice); state && rate)
        state->voice.sampleRate = rate;
}

bool SetLowPass(Voice *voice, float frequency, float oneOverQ) {
    if (!std::isfinite(frequency) || !std::isfinite(oneOverQ) || oneOverQ <= 0)
        return false;
    const std::lock_guard<std::mutex> lock(mutex);
    VoiceState *const state = Find(voice);
    if (!state)
        return false;
    if (frequency == 1 && oneOverQ == 1) {
        state->b0 = 1;
        state->b1 = state->b2 = state->a1 = state->a2 = 0;
    } else {
        const float omega = 2 * std::asin(std::clamp(frequency, 0.0f, 1.0f) / 2) * state->voice.sampleRate / output.freq;
        const float cosine = std::cos(omega), alpha = std::sin(omega) * std::clamp(oneOverQ, 0.0f, 1.5f) * 0.5f;
        const float scale = 1 / (1 + alpha);
        state->b0 = (1 - cosine) * 0.5f * scale;
        state->b1 = (1 - cosine) * scale;
        state->b2 = state->b0;
        state->a1 = -2 * cosine * scale;
        state->a2 = (1 - alpha) * scale;
    }
    return true;
}

bool SetOutputMatrix(Voice *voice, const float *matrix, std::uint32_t sourceChannels, std::uint32_t outputChannels) {
    if (!voice || !matrix || sourceChannels != voice->channels || outputChannels != 6)
        return false;
    const std::lock_guard<std::mutex> lock(mutex);
    VoiceState *const state = Find(voice);
    if (!state)
        return false;
    std::copy_n(matrix, sourceChannels * outputChannels, state->matrix.begin());
    return true;
}

void GetOutputMatrix(const Voice *voice, float *matrix, std::uint32_t sourceChannels, std::uint32_t outputChannels) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (const VoiceState *const state = Find(voice); state && matrix && sourceChannels == voice->channels && outputChannels == 6)
        std::copy_n(state->matrix.begin(), sourceChannels * outputChannels, matrix);
}

} // namespace shenmue::platform::audio
