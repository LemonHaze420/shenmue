// S2-private encoded-handle arcade voice pool and actual XAudio/WAV primitives.
// No S1 dialogue archives, title admission, or SOND task lifecycle are linked.
#include "s1/d3t/namespace.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/settings.hpp"
#include "s2/areas/game_arcade_audio_host.hpp"
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>
#include <cstdarg>
#include <cstdlib>
#include <cwchar>
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <mmdeviceapi.h>
#include <initguid.h>
#include <functiondiscoverykeys_devpkey.h>
#endif

namespace shenmue::s1 {

namespace {

std::array<std::byte *, kEncodedAudioBufferCount> g_encodedAudioBuffers{};
std::uint32_t g_nextEncodedAudioBuffer{};
void (*g_stopEnvelope)(std::uint32_t){};
bool (*g_releaseEnvelope)(std::uint32_t){};

#if defined(_WIN32)
std::array<IXAudio2SourceVoice *, kEncodedAudioVoiceCount> g_dialogueVoices{};
std::uint32_t g_nextDialogueVoice{};
std::unique_ptr<XA_VoiceContext[]> g_rawSoundEffectVoices;
std::array<AudioVoiceRouting, kXAudioVoiceCount> g_voiceRouting{};
AudioVoiceRouting &Routing(const XA_VoiceContext *owner) {
    return g_voiceRouting[owner->slotId];
}
std::array<std::array<wchar_t, 256>, 12> g_audioDeviceIds{};
std::array<std::array<char, 64>, 14> g_audioDeviceNames{};
std::uint32_t g_audioDeviceCount{};
std::int32_t g_audioDeviceIndex = 1;
bool g_audioDeviceSelected{};
std::array<std::int32_t, 6> g_audioDeviceRequests{};
std::uint32_t g_audioDeviceRequestCount{};

IXAudio2 *g_xaudio{};
IXAudio2MasteringVoice *g_masteringVoice{};
IXAudio2SubmixVoice *g_aicaSubmixVoice{};
IUnknown *g_aicaReverbEffect{};
IUnknown *g_masteringLimiterEffect{};
std::recursive_mutex g_audioInitializationMutex;
bool g_audioComInitialized{};
std::uint32_t g_nextRawSoundEffectVoice{};
std::unique_ptr<AudioEngineCallbacks> g_audioEngineCallbacks;
#endif

XA_VoiceContext *ResolveRawVoice(const void *playback) {
#if defined(_WIN32)
    const std::uintptr_t handle = reinterpret_cast<std::uintptr_t>(playback);
    return g_rawSoundEffectVoices && handle >= 1 && handle <= kXAudioVoiceCount ? &g_rawSoundEffectVoices[handle - 1] : nullptr;
#else
    (void)playback;
    return nullptr;
#endif
}

} // namespace

#if defined(_WIN32)
void STDMETHODCALLTYPE AudioEngineCallbacks::OnProcessingPassStart() {
}
void STDMETHODCALLTYPE AudioEngineCallbacks::OnProcessingPassEnd() {
}
void STDMETHODCALLTYPE AudioEngineCallbacks::OnCriticalError(HRESULT error) {
    if (static_cast<std::uint32_t>(error) == 0x88880001U) {
        effectFlag.store(1, std::memory_order_release);
        for (IXAudio2SourceVoice *&voice : g_dialogueVoices)
            voice = nullptr;
    }
}
#endif

bool g_audioFilterBypass{};

void ReleaseEncodedAudioBuffers() {
    for (std::byte *&buffer : g_encodedAudioBuffers) {
        std::free(buffer);
        buffer = nullptr;
    }
}

#if defined(_WIN32)
XA_VoiceContext::XA_VoiceContext() {
    event = CreateEventExA(nullptr, nullptr, 0, SYNCHRONIZE | EVENT_MODIFY_STATE);
}

XA_VoiceContext::~XA_VoiceContext() {
    CloseHandle(event);
    delete state;
}
void STDMETHODCALLTYPE XA_VoiceContext::OnVoiceProcessingPassStart(UINT32) {
}
void STDMETHODCALLTYPE XA_VoiceContext::OnVoiceProcessingPassEnd() {
}
void STDMETHODCALLTYPE XA_VoiceContext::OnStreamEnd() {
}
void STDMETHODCALLTYPE XA_VoiceContext::OnBufferStart(void *) {
}
void STDMETHODCALLTYPE XA_VoiceContext::OnLoopEnd(void *) {
}
void STDMETHODCALLTYPE XA_VoiceContext::OnVoiceError(void *, HRESULT) {
}
void STDMETHODCALLTYPE XA_VoiceContext::OnBufferEnd(void *) {
    if (state && state->phase) {
        d3t::debugLog("** EnvelopeGenerator::StopAndReset **");
        d3t::debugLog("Off %d", state->uid);
        state->phase = 0;
        if (g_stopEnvelope)
            g_stopEnvelope(state->voiceId);
    }
    active = 0;
}
#endif

bool InitializeXAudio() {
#if defined(_WIN32)
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    if (g_xaudio)
        return true;
    if (!g_audioDeviceSelected) {
        g_audioDeviceIndex = g_applicationSettings.savedAudioDevice;
        g_audioDeviceSelected = true;
    }
    const HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(comResult) && comResult != RPC_E_CHANGED_MODE) {
        return false;
    }
    g_audioComInitialized = SUCCEEDED(comResult);
    HRESULT result = XAudio2Create(&g_xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(result)) {
        if (g_audioComInitialized) {
            CoUninitialize();
            g_audioComInitialized = false;
        }
        return false;
    }
    g_audioEngineCallbacks.reset(new AudioEngineCallbacks);
    g_xaudio->RegisterForCallbacks(g_audioEngineCallbacks.get());
    GetAudioDeviceDetails();
    if (g_audioDeviceIndex < 0 || static_cast<std::uint32_t>(g_audioDeviceIndex) >= g_audioDeviceCount)
        g_audioDeviceIndex = 0;
    if (!g_audioDeviceCount || !g_audioDeviceIndex)
        return false;
    result = g_xaudio->CreateMasteringVoice(&g_masteringVoice, 6, 48000, 0, g_audioDeviceIds[g_audioDeviceIndex - 1].data());

    if (SUCCEEDED(result))
        result = CreateFX(__uuidof(FXMasteringLimiter), &g_masteringLimiterEffect);
    if (SUCCEEDED(result)) {
        XAUDIO2_EFFECT_DESCRIPTOR limiterDescriptor{g_masteringLimiterEffect, TRUE, 6};
        const XAUDIO2_EFFECT_CHAIN limiterChain{1, &limiterDescriptor};
        const FXMASTERINGLIMITER_PARAMETERS limiter{FXMASTERINGLIMITER_DEFAULT_RELEASE, FXMASTERINGLIMITER_DEFAULT_LOUDNESS};
        result = g_masteringVoice->SetEffectChain(&limiterChain);
        if (SUCCEEDED(result))
            result = g_masteringVoice->SetEffectParameters(0, &limiter, sizeof(limiter));
    }
    if (SUCCEEDED(result))
        result = CreateAudioReverb(&g_aicaReverbEffect);
    XAUDIO2_EFFECT_DESCRIPTOR reverbDescriptor{};
    XAUDIO2_EFFECT_CHAIN reverbChain{};
    if (SUCCEEDED(result)) {
        reverbDescriptor.pEffect = g_aicaReverbEffect;
        reverbDescriptor.InitialState = TRUE;
        reverbDescriptor.OutputChannels = 1;
        reverbChain.EffectCount = 1;
        reverbChain.pEffectDescriptors = &reverbDescriptor;
        result = g_xaudio->CreateSubmixVoice(&g_aicaSubmixVoice, 1, 48000, 0, 0, nullptr, &reverbChain);
    }
    if (SUCCEEDED(result)) {
        result = g_aicaSubmixVoice->SetVolume(5.0f);
    }
    if (SUCCEEDED(result)) {
        XAUDIO2FX_REVERB_PARAMETERS reverb{};
        reverb.WetDryMix = 100.0f;
        reverb.ReflectionsDelay = 2;
        reverb.ReverbDelay = 10;
        reverb.RearDelay = 5;
        reverb.SideDelay = 5;
        reverb.PositionLeft = 6;
        reverb.PositionRight = 6;
        reverb.PositionMatrixLeft = 27;
        reverb.PositionMatrixRight = 27;
        reverb.EarlyDiffusion = 15;
        reverb.LateDiffusion = 15;
        reverb.LowEQGain = 8;
        reverb.LowEQCutoff = 4;
        reverb.HighEQGain = 8;
        reverb.HighEQCutoff = 6;
        reverb.RoomFilterFreq = 5000.0f;
        reverb.RoomFilterMain = -10.0f;
        reverb.RoomFilterHF = -2.0f;
        reverb.ReflectionsGain = 0.0f;
        reverb.ReverbGain = 0.0f;
        reverb.DecayTime = 1.3f;
        reverb.Density = 75.0f;
        reverb.RoomSize = 100.0f;
        reverb.DisableLateField = FALSE;
        result = g_aicaSubmixVoice->SetEffectParameters(0, &reverb, sizeof(reverb));
    }
    if (FAILED(result)) {
        if (g_aicaSubmixVoice) {
            g_aicaSubmixVoice->DestroyVoice();
            g_aicaSubmixVoice = nullptr;
        }
        if (g_aicaReverbEffect) {
            g_aicaReverbEffect->Release();
            g_aicaReverbEffect = nullptr;
        }
        if (g_masteringLimiterEffect) {
            g_masteringLimiterEffect->Release();
            g_masteringLimiterEffect = nullptr;
        }
        if (g_masteringVoice) {
            g_masteringVoice->DestroyVoice();
            g_masteringVoice = nullptr;
        }
        g_xaudio->UnregisterForCallbacks(g_audioEngineCallbacks.get());
        g_audioEngineCallbacks.reset();
        g_xaudio->Release();
        if (g_audioComInitialized) {
            CoUninitialize();
            g_audioComInitialized = false;
        }
        g_xaudio = nullptr;
        return false;
    }
    g_rawSoundEffectVoices.reset(new XA_VoiceContext[kXAudioVoiceCount]);
    for (std::uint32_t i = 0; i < kXAudioVoiceCount; ++i) {
        XA_VoiceContext &owner = g_rawSoundEffectVoices[i];
        owner.slotId = i;
        owner.state = new AicaState{};
        owner.state->uid = i;
        owner.state->currentFilterLevel = 1.0f;
        owner.state->instantFilterAttack = 1;
        owner.state->linearVolume = 1.0f;
    }
    return true;
#else
    return false;
#endif
}

namespace {
std::uint32_t WaveWord(const std::byte *bytes) {
    std::uint32_t value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
}
std::uint16_t WaveHalfWord(const std::byte *bytes) {
    std::uint16_t value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
}
std::int32_t FindRiffChunk(const std::byte *begin, std::size_t extent, std::size_t available, std::uint32_t wanted, RiffChunkView &result) {
    result = {};
    while (extent > 8 && available >= 8) {
        const std::uint32_t bytes = WaveWord(begin + 4);
        if (WaveWord(begin) == wanted) {
            if (bytes > available - 8)
                return kAudioEndOfFile;
            result = {.data = begin + 8, .size = bytes};
            return 0;
        }
        const std::size_t next = static_cast<std::size_t>(bytes) + 8;
        if (next > extent || next > available)
            return kAudioFormatError;
        begin += next;
        extent -= next;
        available -= next;
    }
    return kAudioFormatError;
}
std::int32_t FindWaveRiff(const std::byte *bytes, std::size_t size, RiffChunkView &riff) {
    while (size > 8) {
        const std::uint32_t count = WaveWord(bytes + 4);
        if (WaveWord(bytes) == SHEN_NAME(RIFF)) {
            if (count < 4)
                return kAudioFormatError;
            if (size < 12)
                return kAudioEndOfFile;
            const std::uint32_t kind = WaveWord(bytes + 8);
            if (kind != SHEN_NAME(WAVE) && kind != SHEN_NAME(XWMA))
                return kAudioFormatError;
            riff = {.data = bytes + 8, .size = count};
            return 0;
        }
        const std::size_t next = static_cast<std::size_t>(count) + 8;
        if (next > size)
            return kAudioFormatError;
        bytes += next;
        size -= next;
    }
    return kAudioFormatError;
}
std::int32_t ParseWaveLoop(const std::byte *bytes, std::size_t size, AudioWaveData &wave) {
    if (!bytes)
        return kAudioPointerError;
    if (size < 12)
        return kAudioFormatError;
    wave.loopBegin = 0;
    wave.loopLength = 0;
    RiffChunkView riff{};
    std::int32_t result = FindWaveRiff(bytes, size, riff);
    if (result < 0)
        return result;
    if (WaveWord(riff.data) == SHEN_NAME(XWMA))
        return 0;
    const std::byte *scan = riff.data + 4;
    std::size_t available = size - static_cast<std::size_t>(scan - bytes);
    if (available < 8)
        return kAudioEndOfFile;
    RiffChunkView loop{};
    result = FindRiffChunk(scan, riff.size, available, SHEN_NAME(wsmp), loop);
    if (result == kAudioEndOfFile)
        return result;
    if (result >= 0) {
        scan = loop.data;
        available = size - static_cast<std::size_t>(scan - bytes);
    }
    if (result >= 0 && loop.size >= 20) {
        const std::uint32_t headerSize = WaveWord(loop.data);
        const std::uint32_t count = WaveWord(loop.data + 16);
        if (headerSize <= loop.size && count <= (loop.size - headerSize) / 16) {
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::byte *entry = loop.data + headerSize + index * 16;
                if (WaveWord(entry + 4) >= 2)
                    continue;
                wave.loopBegin = WaveWord(entry + 8);
                wave.loopLength = WaveWord(entry + 12);
                return 0;
            }
        }
    }
    result = FindRiffChunk(scan, riff.size, available, SHEN_NAME(smpl), loop);
    if (result == kAudioEndOfFile)
        return result;
    if (result >= 0 && loop.size >= 36) {
        const std::uint32_t count = WaveWord(loop.data + 28);
        if (count <= (loop.size - 36) / 24) {
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::byte *entry = loop.data + 36 + index * 24;
                if (WaveWord(entry + 4))
                    continue;
                wave.loopBegin = WaveWord(entry + 8);
                wave.loopLength = WaveWord(entry + 8) + 1 + WaveWord(entry + 12);
                break;
            }
        }
    }
    return 0;
}
} // namespace

std::int32_t ParseXMA_Wave(const std::byte *bytes, std::size_t size, AudioWaveData &wave, bool &wma, bool &xma) {
    if (!bytes)
        return kAudioPointerError;
    wma = false;
    xma = false;
    if (size < 34)
        return kAudioFormatError;
    RiffChunkView riff{};
    std::int32_t result = FindWaveRiff(bytes, size, riff);
    if (result < 0)
        return result;
    const std::byte *scan = riff.data + 4;
    const std::size_t available = size - static_cast<std::size_t>(scan - bytes);
    if (available < 8)
        return kAudioEndOfFile;
    RiffChunkView format{};
    result = FindRiffChunk(scan, riff.size, available, make_shen_name('f', 'm', 't', ' '), format);
    if (result < 0)
        return result;
    if (format.size < 16)
        return kAudioFormatError;
    const std::uint16_t tag = WaveHalfWord(format.data);
    if (tag != 1 && tag != 3) {
        if (format.size < 18)
            return kAudioFormatError;
        const std::uint16_t extension = WaveHalfWord(format.data + 16);
        if (format.size < static_cast<std::uint32_t>(18 + extension))
            return kAudioFormatError;
        if (tag == 2) {
            if (format.size < 50 || extension < 32)
                return kAudioFormatError;
        } else if (tag == 0x161 || tag == 0x162) {
            wma = true;
        } else if (tag == 0x166) {
            if (format.size < 52 || extension < 34)
                return kAudioFormatError;
            xma = true;
        } else if (tag == 0xfffe) {
            if (format.size < 40 || extension < 22)
                return kAudioFormatError;
            constexpr std::uint8_t tail[]{0, 0, 0x10, 0, 0x80, 0, 0, 0xaa, 0, 0x38, 0x9b, 0x71};
            if (std::memcmp(format.data + 28, tail, sizeof(tail)) != 0)
                return kAudioUnsupportedFormat;
            const std::uint32_t subtype = WaveWord(format.data + 24);
            if (subtype == 0x161 || subtype == 0x162)
                wma = true;
            else if (subtype != 1 && subtype != 3)
                return kAudioUnsupportedFormat;
        } else {
            return kAudioUnsupportedFormat;
        }
    }
    RiffChunkView data{};
    result = FindRiffChunk(scan, riff.size, available, SHEN_NAME(data), data);
    if (result == kAudioFormatError || (result >= 0 && !data.size))
        return kAudioInvalidData;
    if (result < 0)
        return result;
    wave.format = format.data;
    wave.audio = data.data;
    wave.audioBytes = data.size;
    return 0;
}

std::int32_t ParseWAV(const std::byte *bytes, std::size_t size, std::uint32_t tag, const std::uint32_t *&values, std::uint32_t &count) {
    if (!bytes)
        return kAudioPointerError;
    if (size < 12)
        return kAudioFormatError;
    values = nullptr;
    count = 0;
    RiffChunkView riff{};
    const std::int32_t result = FindWaveRiff(bytes, size, riff);
    if (result < 0)
        return result;
    const std::byte *scan = riff.data + 4;
    const std::size_t available = size - static_cast<std::size_t>(scan - bytes);
    if (available < 8)
        return kAudioEndOfFile;
    RiffChunkView chunk{};
    const std::int32_t chunkResult = FindRiffChunk(scan, riff.size, available, tag, chunk);
    if (chunkResult == kAudioFormatError)
        return 0;
    if (chunkResult < 0)
        return chunkResult;
    if (chunk.size & 3U)
        return kAudioFormatError;
    values = reinterpret_cast<const std::uint32_t *>(chunk.data);
    count = chunk.size / 4;
    return 0;
}

std::int32_t ProcessWave(const std::byte *bytes, std::size_t size, AudioWaveData &wave) {
    if (!bytes)
        return kAudioInvalidArgument;
    wave = {};
    if (size < 34)
        return kAudioFormatError;
    bool wma{}, xma{};
    std::int32_t result = ParseXMA_Wave(bytes, size, wave, wma, xma);
    if (result < 0)
        return result;
    result = ParseWaveLoop(bytes, size, wave);
    if (result < 0)
        return result;
    if (wma || xma)
        return ParseWAV(bytes, size, wma ? SHEN_NAME(dpds) : SHEN_NAME(seek), wave.packetBytes, wave.packetCount);
    return 0;
}

void SetOutputAudioMatrix(void *playback, std::uint32_t sourceChannels) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
    IXAudio2SourceVoice *const voice = owner ? owner->voice : nullptr;
    if (!g_masteringVoice || !voice)
        return;
    if (sourceChannels == 2) {
        constexpr float matrix[]{1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1};
        voice->SetOutputMatrix(g_masteringVoice, sourceChannels, 6, matrix);
    } else {
        constexpr float matrix[]{0, 0, 1, 0, 0, 0};
        voice->SetOutputMatrix(g_masteringVoice, sourceChannels, 6, matrix);
    }
#else
    (void)playback;
    (void)sourceChannels;
#endif
}
void SetOutputMatrixVolumeAndChannels(void *playback, float volume, std::uint32_t sourceChannels) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
    IXAudio2SourceVoice *const voice = owner ? owner->voice : nullptr;
    if (g_xaudio && g_masteringVoice && GetXAudioEffectFlagVal() != 1 && g_aicaSubmixVoice && voice) {
        const float matrix[]{volume, volume};
        voice->SetOutputMatrix(g_aicaSubmixVoice, sourceChannels, 1, matrix);
    }
#else
    (void)playback;
    (void)volume;
    (void)sourceChannels;
#endif
}

void StopSourceVoiceAndFlush(std::uint32_t physicalId) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(GetRawSoundEffectVoice(physicalId));
    if (!g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return;
    owner->voice->ExitLoop();
    owner->voice->Stop();
    owner->voice->FlushSourceBuffers();
#else
    (void)physicalId;
#endif
}
void CreateAndStartPlayingSourceVoice(std::uint32_t physicalId, const std::byte *bytes, std::size_t size) {
    AudioWaveData wave{};
    if (ProcessWave(bytes, size, wave) < 0 || !wave.format)
        return;
#if defined(_WIN32)
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    XA_VoiceContext *const owner = ResolveRawVoice(GetRawSoundEffectVoice(physicalId));
    if (!g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return;
    const WAVEFORMATEX *const format = static_cast<const WAVEFORMATEX *>(wave.format);
    owner->voice->SetSourceSampleRate(format->nSamplesPerSec);
    owner->voice->SetVolume(1.0f);
    owner->voice->SetFrequencyRatio(1.0f);
    SetOutputAudioMatrix(GetRawSoundEffectVoice(physicalId), format->nChannels);
    XAUDIO2_BUFFER buffer{};
    buffer.AudioBytes = wave.audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(wave.audio);
    buffer.PlayLength = wave.audioBytes / (format->nChannels * format->wBitsPerSample / 8);
    buffer.LoopBegin = wave.loopBegin;
    buffer.LoopLength = wave.loopLength;
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    owner->voice->SubmitSourceBuffer(&buffer);
    owner->voice->Start();
#else
    (void)physicalId;
#endif
}
void ProcessWave_Wrapper_0(std::uint32_t physicalId, const std::byte *bytes, std::size_t size) {
    AudioWaveData wave{};
    if (ProcessWave(bytes, size, wave) < 0 || !wave.format)
        return;
#if defined(_WIN32)
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    XA_VoiceContext *const owner = ResolveRawVoice(GetRawSoundEffectVoice(physicalId));
    if (!g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return;
    const WAVEFORMATEX *const format = static_cast<const WAVEFORMATEX *>(wave.format);
    XAUDIO2_BUFFER buffer{};
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = wave.audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(wave.audio);
    StopSourceVoiceAndFlush(physicalId);
    owner->voice->SetSourceSampleRate(format->nSamplesPerSec);
    owner->voice->SetVolume(1.0f);
    owner->voice->SetFrequencyRatio(1.0f);
    SetOutputAudioMatrix(GetRawSoundEffectVoice(physicalId), format->nChannels);
    SetOutputMatrixVolumeAndChannels(GetRawSoundEffectVoice(physicalId), 0.1f, format->nChannels);
    owner->voice->SubmitSourceBuffer(&buffer);
    owner->voice->Start();
#else
    (void)physicalId;
#endif
}

bool Audio_HasMasteringVoice() {
#if defined(_WIN32)
    return g_masteringVoice != nullptr;
#else
    return false;
#endif
}

void SetRawSoundEffectEnvelopeCallbacks(void (*stop)(std::uint32_t), bool (*release)(std::uint32_t)) {
    g_stopEnvelope = stop;
    g_releaseEnvelope = release;
}
std::uint32_t RawSoundEffectVoiceId(const void *playback) {
    const std::uintptr_t handle = reinterpret_cast<std::uintptr_t>(playback);
    return handle >= 1 && handle <= kXAudioVoiceCount ? static_cast<std::uint32_t>(handle - 1) : UINT32_MAX;
}
void *GetRawSoundEffectVoice(std::uint32_t physicalId) {
#if defined(_WIN32)
    return g_rawSoundEffectVoices && physicalId < kXAudioVoiceCount ? reinterpret_cast<void *>(static_cast<std::uintptr_t>(physicalId + 1)) : nullptr;
#else
    (void)physicalId;
    return nullptr;
#endif
}
AicaState *GetRawSoundEffectEnvelopeState(std::uint32_t physicalId) {
    XA_VoiceContext *const owner = ResolveRawVoice(GetRawSoundEffectVoice(physicalId));
    return owner ? owner->state : nullptr;
}
bool RawSoundEffectVoiceIsActive(const void *playback) {
    const XA_VoiceContext *const owner = ResolveRawVoice(playback);
    return Audio_HasMasteringVoice() && GetXAudioEffectFlagVal() != 1 && owner && owner->active;
}
bool RawSoundEffectVoiceIsReserved(const void *playback) {
    const XA_VoiceContext *const owner = ResolveRawVoice(playback);
    return owner && owner->reserved;
}
void ReleaseRawSoundEffectReservation(void *playback) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
    if (owner)
        owner->reserved = 0;
}

std::uint32_t GetXAudioEffectFlagVal() {
#if defined(_WIN32)
    return g_audioEngineCallbacks ? g_audioEngineCallbacks->effectFlag.load(std::memory_order_acquire) : 1;
#else
    return 1;
#endif
}

bool GetAudioDeviceDetails() {
#if defined(_WIN32)
    if (!g_xaudio)
        return false;
    g_audioDeviceCount = 0;
    std::strcpy(g_audioDeviceNames[0].data(), "UIOTGLOFF0"); // @text EN: "Off" | JA: "いいえ"
    IMMDeviceEnumerator *enumerator = nullptr;
    HRESULT result = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&enumerator));
    if (FAILED(result))
        return false;
    IMMDeviceCollection *devices = nullptr;
    result = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices);
    enumerator->Release();
    if (FAILED(result))
        return false;
    UINT count = 0;
    devices->GetCount(&count);
    if (count > 12)
        count = 12;
    for (UINT index = 0; index < count; ++index) {
        IMMDevice *device = nullptr;
        if (FAILED(devices->Item(index, &device)))
            continue;
        LPWSTR id = nullptr;
        if (SUCCEEDED(device->GetId(&id))) {
            std::wcsncpy(g_audioDeviceIds[index].data(), id, g_audioDeviceIds[index].size() - 1);
            CoTaskMemFree(id);
        }
        IPropertyStore *properties = nullptr;
        if (SUCCEEDED(device->OpenPropertyStore(STGM_READ, &properties))) {
            PROPVARIANT name{};
            if (SUCCEEDED(properties->GetValue(PKEY_Device_FriendlyName, &name)) && name.vt == VT_LPWSTR)
                std::wcstombs(g_audioDeviceNames[index + 1].data(), name.pwszVal, 64);
            PropVariantClear(&name);
            properties->Release();
        }
        device->Release();
    }
    devices->Release();
    if (!count)
        return false;
    g_audioDeviceCount = count + 1;
    return true;
#else
    return false;
#endif
}

void StopEngine(std::uint32_t voiceId) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(GetRawSoundEffectVoice(voiceId));
    if (g_xaudio && g_masteringVoice && !GetXAudioEffectFlagVal() && owner && owner->voice)
        owner->voice->Stop();
#else
    (void)voiceId;
#endif
}

void ConfigureAndSubmitXAudioBuffer(std::uint32_t voiceId, const RawAudioSubmission &submission) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(GetRawSoundEffectVoice(voiceId));
    if (!g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return;
    XAUDIO2_BUFFER buffer{};
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = submission.audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(submission.audio);
    if (submission.loopLength) {
        buffer.LoopBegin = submission.loopBegin;
        buffer.LoopLength = submission.loopLength;
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }
    const float volume = submission.volume * (submission.music ? g_applicationSettings.savedMusicVolume : 1.0f);
    float ratio = submission.frequencyRatio;
    std::uint32_t rate = 44100;
    while (ratio > 4.0f) {
        ratio *= 0.5f;
        rate *= 2;
    }
    owner->voice->FlushSourceBuffers();
    owner->voice->SetSourceSampleRate(rate);
    owner->voice->SetVolume(volume);
    owner->voice->SetFrequencyRatio(ratio);
    owner->voice->SubmitSourceBuffer(&buffer);
    const std::uint16_t channels = static_cast<const WAVEFORMATEX *>(submission.format)->nChannels;
    SetOutputMatrixVolumeAndChannels(GetRawSoundEffectVoice(voiceId), volume, channels);

#else
    (void)voiceId;
    (void)submission;
#endif
}
void SetLowPassFilterParamsForAudio(std::uint32_t voiceId, float frequency, float oneOverQ) {
    SetRawSoundEffectLowPassFrequency(GetRawSoundEffectVoice(voiceId), frequency, oneOverQ);
}
void StopSourceVoiceAndFlush_0(std::uint32_t voiceId, bool release) {
    StopRawSoundEffectPlayback(GetRawSoundEffectVoice(voiceId), release);
}

void *PrepareRawSoundEffect(const std::byte *samples, std::uint32_t audioBytes, std::uint32_t loopBegin, std::uint32_t loopLength, std::uint16_t channels,
    float volume, float frequencyRatio, std::uint32_t sampleRate, bool routeThroughAica) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(AllocateRawSoundEffectVoice(16, channels, routeThroughAica));
    if (!owner)
        return nullptr;
    WAVEFORMATEX format{};
    format.nChannels = channels;
    const RawAudioSubmission submission{.format = &format,
        .audio = samples,
        .audioBytes = audioBytes,
        .loopBegin = loopBegin,
        .loopLength = loopLength,
        .frequencyRatio = frequencyRatio,
        .volume = volume};
    ConfigureAndSubmitXAudioBuffer(owner->slotId, submission);
    if (owner->voice && sampleRate != 44100) {
        while (frequencyRatio > 4.0f) {
            sampleRate *= 2;
            frequencyRatio *= 0.5f;
        }
        owner->voice->SetSourceSampleRate(sampleRate);
    }
    return GetRawSoundEffectVoice(owner->slotId);
#else
    (void)samples;
    (void)audioBytes;
    (void)loopBegin;
    (void)loopLength;
    (void)channels;
    (void)volume;
    (void)frequencyRatio;
    (void)sampleRate;
    (void)routeThroughAica;
    return nullptr;
#endif
}

void StartRawSoundEffectPlayback(void *playback) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
    if (g_xaudio && g_masteringVoice && GetXAudioEffectFlagVal() != 1 && owner && owner->voice)
        owner->voice->Start();
#else
    (void)playback;
#endif
}
void *StartRawSoundEffect(const std::byte *samples, std::uint32_t audioBytes, std::uint32_t loopBegin, std::uint32_t loopLength, std::uint16_t channels,
    float volume, float frequencyRatio, std::uint32_t sampleRate, bool routeThroughAica) {
    void *const voice = PrepareRawSoundEffect(samples, audioBytes, loopBegin, loopLength, channels, volume, frequencyRatio, sampleRate, routeThroughAica);
    StartRawSoundEffectPlayback(voice);
    return voice;
}

void *AllocateRawSoundEffectVoice(std::uint16_t bitsPerSample, std::uint16_t channels, bool reclaimable) {
#if defined(_WIN32)
    if (!InitializeXAudio() || !g_masteringVoice || !g_rawSoundEffectVoices)
        return nullptr;
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    XA_VoiceContext *owner = nullptr;
    for (std::uint32_t attempt = 0; attempt < kXAudioVoiceCount && !owner; ++attempt) {
        g_nextRawSoundEffectVoice = (g_nextRawSoundEffectVoice + 1U) % static_cast<std::uint32_t>(kXAudioVoiceCount);
        XA_VoiceContext &candidate = g_rawSoundEffectVoices[g_nextRawSoundEffectVoice];
        if (candidate.active || candidate.reserved)
            continue;
        if (candidate.voice) {
            XAUDIO2_VOICE_STATE state{};
            candidate.voice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
            if (state.BuffersQueued != 0)
                continue;
        }
        owner = &candidate;
    }
    if (!owner)
        return nullptr;
    if (owner->voice)
        owner->voice->DestroyVoice();
    owner->voice = nullptr;
    Routing(owner) = {};
    owner->active = true;
    owner->reserved = !reclaimable;
    WAVEFORMATEX format{};
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = channels;
    format.nSamplesPerSec = 48000;
    format.wBitsPerSample = bitsPerSample;
    format.nBlockAlign = static_cast<WORD>(channels * bitsPerSample / 8);
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    std::array<XAUDIO2_SEND_DESCRIPTOR, 2> sendDescriptors{{{XAUDIO2_SEND_USEFILTER, g_masteringVoice}, {XAUDIO2_SEND_USEFILTER, g_aicaSubmixVoice}}};
    const XAUDIO2_VOICE_SENDS sends{.SendCount = static_cast<UINT32>(sendDescriptors.size()), .pSends = sendDescriptors.data()};
    g_xaudio->CreateSourceVoice(&owner->voice, &format, XAUDIO2_VOICE_USEFILTER, 4.0f, owner, &sends, nullptr);
    Routing(owner).channels = channels;

    Routing(owner).dryOutputVoice = g_masteringVoice;
    Routing(owner).dryOutputChannels = 6;
    Routing(owner).reverbOutputVoice = g_aicaSubmixVoice;
    Routing(owner).reverbOutputChannels = 1;
    return GetRawSoundEffectVoice(owner->slotId);
#else
    static_cast<void>(bitsPerSample);
    static_cast<void>(channels);
    static_cast<void>(reclaimable);
    return nullptr;
#endif
}

void StartRawSoundEffectVoice(void *playback) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    std::lock_guard lock(g_audioInitializationMutex);
    if (!g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return;
    owner->voice->SetSourceSampleRate(48000);

    owner->voice->SetVolume(1.0f);

    SetOutputAudioMatrix(playback, Routing(owner).channels);
    owner->voice->Start();
#else
    static_cast<void>(owner);
#endif
}

void SubmitRawSoundEffectBuffer(void *playback, const std::byte *samples, std::uint32_t audioBytes, std::uint32_t playLength) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    std::lock_guard lock(g_audioInitializationMutex);
    if (!g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return;
    XAUDIO2_BUFFER buffer{};
    buffer.AudioBytes = audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(samples);
    buffer.PlayLength = playLength;
    owner->voice->SubmitSourceBuffer(&buffer);
#else
    static_cast<void>(owner);
    static_cast<void>(samples);
    static_cast<void>(audioBytes);
    static_cast<void>(playLength);
#endif
}

bool RawSoundEffectIsPlaying(const void *playback) {
    const XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    if (!owner || !owner->voice)
        return false;
    XAUDIO2_VOICE_STATE state{};
    owner->voice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    return state.BuffersQueued != 0;
#else
    static_cast<void>(owner);
    return false;
#endif
}

void SetRawSoundEffectVolume(void *playback, float volume) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    if (g_xaudio && g_masteringVoice && GetXAudioEffectFlagVal() == 0 && owner && owner->voice) {

        owner->voice->SetVolume(volume);
    }
#else
    static_cast<void>(owner);
    static_cast<void>(volume);
#endif
}
void SetRawSoundEffectFrequencyRatio(void *playback, float frequencyRatio) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    if (g_xaudio && g_masteringVoice && GetXAudioEffectFlagVal() != 1 && owner && owner->voice)
        owner->voice->SetFrequencyRatio(frequencyRatio);
#else
    static_cast<void>(owner);
    static_cast<void>(frequencyRatio);
#endif
}
bool SetRawSoundEffectLowPassFrequency(void *playback, float normalizedFrequency, float oneOverQ) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    if (g_audioFilterBypass || !g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return false;
    XAUDIO2_FILTER_PARAMETERS filter{};
    filter.Type = LowPassFilter;
    filter.Frequency = std::fmin(1.0f, normalizedFrequency);
    filter.OneOverQ = std::fmin(1.5f, oneOverQ);
    return SUCCEEDED(owner->voice->SetFilterParameters(&filter));
#else
    static_cast<void>(owner);
    static_cast<void>(normalizedFrequency);
    static_cast<void>(oneOverQ);
    return false;
#endif
}

bool SetRawSoundEffectOutputMatrix(void *playback, const float *levelMatrix, std::uint32_t sourceChannels, std::uint32_t destinationChannels) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    if (!g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice || !levelMatrix ||
        sourceChannels != Routing(owner).channels) {
        return false;
    }
    IXAudio2Voice *outputVoice = nullptr;
    if (destinationChannels == Routing(owner).dryOutputChannels) {
        outputVoice = Routing(owner).dryOutputVoice;
    } else if (destinationChannels == Routing(owner).reverbOutputChannels) {
        outputVoice = Routing(owner).reverbOutputVoice;
    }
    if (!outputVoice)
        return false;
    if (destinationChannels == 1) {
        std::array<float, 2> repeated{};
        repeated.fill(levelMatrix[0]);
        return SUCCEEDED(owner->voice->SetOutputMatrix(outputVoice, sourceChannels, 1, repeated.data()));
    }
    return SUCCEEDED(owner->voice->SetOutputMatrix(outputVoice, sourceChannels, destinationChannels, levelMatrix));
#else
    static_cast<void>(owner);
    static_cast<void>(levelMatrix);
    static_cast<void>(sourceChannels);
    static_cast<void>(destinationChannels);
    return false;
#endif
}
void StopRawSoundEffectPlayback(void *playback, bool release) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    if (!owner || !g_xaudio || !g_masteringVoice || GetXAudioEffectFlagVal() == 1 || !owner->active)
        return;
    if (release && g_releaseEnvelope && g_releaseEnvelope(owner->slotId))
        return;
    if (RawSoundEffectIsPlaying(playback)) {
        owner->voice->ExitLoop();
        owner->voice->Stop(XAUDIO2_PLAY_TAILS);
        owner->voice->FlushSourceBuffers();
    }
    owner->reserved = false;
#else
    (void)owner;
    (void)release;
#endif
}

void SetRawSoundEffectPaused(void *playback, bool paused) {
    XA_VoiceContext *const owner = ResolveRawVoice(playback);
#if defined(_WIN32)
    if (paused && g_xaudio && g_masteringVoice && GetXAudioEffectFlagVal() != 1 && owner && owner->voice)
        owner->voice->Stop();
#else
    static_cast<void>(owner);
    static_cast<void>(paused);
#endif
}

void StopRawSoundEffect(void *&playback) {
    StopRawSoundEffectPlayback(playback);
    playback = nullptr;
}

void EnvelopeGenerator_StopAndReset() {
#if defined(_WIN32)
    if (g_xaudio) {
        if (g_rawSoundEffectVoices) {
            std::unique_ptr<XA_VoiceContext[]> contexts;
            contexts.swap(g_rawSoundEffectVoices);
            for (std::uint32_t i = 0; i < kXAudioVoiceCount; ++i) {
                XA_VoiceContext &owner = contexts[i];
                if (owner.state && owner.state->phase) {
                    d3t::debugLog("** EnvelopeGenerator::StopAndReset **");
                    d3t::debugLog("Off %d", owner.state->uid);
                    owner.state->phase = 0;
                    if (g_stopEnvelope)
                        g_stopEnvelope(owner.state->voiceId);
                }
                if (owner.voice) {
                    owner.voice->DestroyVoice();
                    owner.voice = nullptr;
                }
                delete owner.state;
                owner.state = nullptr;
                owner.active = 0;
            }
            contexts.reset();
        }
        g_xaudio->UnregisterForCallbacks(g_audioEngineCallbacks.get());
        for (IXAudio2SourceVoice *&voice : g_dialogueVoices) {
            if (voice)
                voice->DestroyVoice();
            voice = nullptr;
        }
        g_audioEngineCallbacks.reset();
        if (g_aicaSubmixVoice)
            g_aicaSubmixVoice->DestroyVoice();
        if (g_masteringVoice)
            g_masteringVoice->DestroyVoice();
        g_xaudio->Release();
    }
    g_aicaSubmixVoice = nullptr;
    g_masteringVoice = nullptr;
    g_xaudio = nullptr;
    g_voiceRouting = {};
#endif
}

void ShutdownXAudio() {
#if defined(_WIN32)
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    EnvelopeGenerator_StopAndReset();
    if (g_aicaReverbEffect)
        g_aicaReverbEffect->Release();
    g_aicaReverbEffect = nullptr;
    if (g_masteringLimiterEffect)
        g_masteringLimiterEffect->Release();
    g_masteringLimiterEffect = nullptr;
    if (g_audioComInitialized)
        CoUninitialize();
    g_audioComInitialized = false;
#endif
    ReleaseEncodedAudioBuffers();
}

namespace {
std::int32_t g_aicaDebugLogEnabled = 0;
}
namespace d3t {
void debugLog(const char *format, ...) {
    std::va_list arguments;
    va_start(arguments, format);
    if (g_aicaDebugLogEnabled != 0) {
        char buffer[1024];
        std::vsprintf(buffer, format, arguments);
    }
    va_end(arguments);
}
} // namespace d3t

} // namespace shenmue::s1

namespace shenmue::s2 {

void SubmitArcadePcmVoice(std::uint32_t voiceId, std::uint32_t channels, std::uint32_t sampleRate, const std::byte *data, std::uint32_t size,
    std::uint32_t begin, std::uint32_t end, bool looped) {
#if defined(_WIN32)
    std::lock_guard lock(s1::g_audioInitializationMutex);
    s1::XA_VoiceContext *const owner = s1::ResolveRawVoice(s1::GetRawSoundEffectVoice(voiceId));
    IXAudio2SourceVoice *const voice = owner ? owner->voice : nullptr;
    if (!s1::g_xaudio || !s1::g_masteringVoice || s1::GetXAudioEffectFlagVal() == 1 || !voice)
        return;
    XAUDIO2_BUFFER buffer{};
    buffer.AudioBytes = size;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(data);
    buffer.PlayBegin = begin;
    buffer.PlayLength = end - begin;
    if (looped) {
        buffer.LoopBegin = begin;
        buffer.LoopLength = end - begin;
        buffer.LoopCount = 255;
    }
    voice->ExitLoop(0);
    voice->Stop(0, 0);
    voice->FlushSourceBuffers();
    voice->SetSourceSampleRate(sampleRate);
    voice->SetVolume(1.0f, 0);
    voice->SetFrequencyRatio(1.0f, 0);
    s1::SetOutputAudioMatrix(s1::GetRawSoundEffectVoice(voiceId), channels);
    voice->SubmitSourceBuffer(&buffer, nullptr);
    voice->Start(0, 0);
#else
    static_cast<void>(voiceId);
    static_cast<void>(channels);
    static_cast<void>(sampleRate);
    static_cast<void>(data);
    static_cast<void>(size);
    static_cast<void>(begin);
    static_cast<void>(end);
    static_cast<void>(looped);
#endif
}

void GetArcadePcmVoiceOutputMatrix(std::uint32_t voiceId, float *matrix) {
#if defined(_WIN32)
    s1::XA_VoiceContext *const owner = s1::ResolveRawVoice(s1::GetRawSoundEffectVoice(voiceId));
    if (!s1::g_xaudio || !s1::g_masteringVoice || s1::GetXAudioEffectFlagVal() == 1 || !owner || !owner->voice)
        return;
    owner->voice->GetOutputMatrix(s1::g_masteringVoice, 1, 6, matrix);
#else
    static_cast<void>(voiceId);
    static_cast<void>(matrix);
#endif
}

} // namespace shenmue::s2
