#include "s1/d3t/namespace.hpp"
#include "s1/file_loader.hpp"
#include "s1/hlib.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/audio_stream.hpp"
#include "s1/archive_handles.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/task/pack.hpp"
#include "s1/subtitles.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <limits>
#include <memory>
#include <mutex>
#include <vector>
#include <string_view>
#include <atomic>
#include <cstdarg>
#include <cstdlib>
#include <new>
#include <cwchar>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
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

#include "s1/audio_metadata_table.inc"

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
// @verified - p
void STDMETHODCALLTYPE AudioEngineCallbacks::OnCriticalError(HRESULT error) {
    if (static_cast<std::uint32_t>(error) == 0x88880001U) {
        effectFlag.store(1, std::memory_order_release);
        for (IXAudio2SourceVoice *&voice : g_dialogueVoices)
            voice = nullptr;
    }
}
#endif

void *g_voiceDecodeBuffer{};
void *g_ahxDecodeBuffer{};
void *g_voiceDecodeBuffer2{};
void *g_ahxDecodeBuffer2{};
bool g_audioFilterBypass{};
std::int32_t g_audioMusicSuppressed{};

// @verified - p
void SetupVBUF() {
    g_voiceDecodeBuffer = HLib_AllocHeapMemBlock(0x23140, static_cast<std::int32_t>(kVoiceBufferToken.name));
    g_ahxDecodeBuffer = HLib_AllocHeapMemBlock(0x3000, static_cast<std::int32_t>(kAhxBufferToken.name));
    g_voiceDecodeBuffer2 = HLib_AllocHeapMemBlock(0x45a00, static_cast<std::int32_t>(kVoiceBuffer2Token.name));
    g_ahxDecodeBuffer2 = HLib_AllocHeapMemBlock(0x3000, static_cast<std::int32_t>(kAhxBuffer2Token.name));
}

void InitializeEncodedAudioBuffers() {
    for (std::byte *&buffer : g_encodedAudioBuffers) {
        if (!buffer)
            buffer = static_cast<std::byte *>(std::malloc(kEncodedAudioBufferBytes));
    }
}

void ReleaseEncodedAudioBuffers() {
    for (std::byte *&buffer : g_encodedAudioBuffers) {
        std::free(buffer);
        buffer = nullptr;
    }
}

// @verified - p
float DecibelsToLinear(float decibels) {
    return decibels > -64.0f ? std::pow(10.0f, decibels / 20.0f) : 0.0f;
}

// @verified - p
const AudioTableBank *ReadAudioFromTable(const char *name) {
    if (g_sceneId < 1 || g_sceneId > 3)
        return nullptr;
    const AudioTableBank *bank = kAudioTables[2 * (g_sceneId - 1) + (g_usingJapaneseAudio ? 1 : 0)];
    while (bank->name) {
        if (std::strcmp(bank->name, name) == 0)
            return bank;
        ++bank;
    }
    return nullptr;
}

#if defined(_WIN32)

// @verified - p
XA_VoiceContext::XA_VoiceContext() {
    event = CreateEventExA(nullptr, nullptr, 0, SYNCHRONIZE | EVENT_MODIFY_STATE);
}

// @verified - p
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
// @verified - p
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

// @verified - p
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

// @verified - p
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

// @verified - p
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
float GetAdjustedVolumeForMusicOrDialogue(std::int32_t volume, bool music) {
    const float decibels = static_cast<float>(volume) / 10.0f;
    if (decibels <= -64.0f)
        return 0.0f;
    const float masterVolume = music ? g_applicationSettings.savedMusicVolume : g_applicationSettings.savedDialogueVolume;
    return std::pow(10.0f, decibels / 20.0f) * masterVolume;
}

PackAdfsWrapper *ReadADFS_Wrapper(char *directory, const char *filename, std::int32_t timeout);


extern std::int32_t g_actNumber;
std::uint8_t g_usingJapaneseAudio = 0;
CVRSEntry g_preloadedAudioEntries[2]{{.name = "BGM01"}, {}};
void *g_bgmAdfs = nullptr;

// @verified - p
HLib::TaskData *FindPreloadedAudioIndex(const char *name) {
    for (CVRSEntry &cached : g_preloadedAudioEntries) {
        if (!cached.name[0])
            return nullptr;
        if (std::strcmp(cached.name, name) == 0)
            return cached.fileData;
    }
    return nullptr;
}

// @verified - p
PackAdfsWrapper *ReadAFS(const char *name, std::int32_t timeout) {
    char directory[80]{};
    std::snprintf(directory, sizeof(directory), "SCENE/%02d/STREAM/PC/%s/%s", g_sceneId, g_usingJapaneseAudio ? "JAP" : "ENG", name);
    char *const separator = std::strrchr(directory, '/');
    char filename[16]{};
    std::snprintf(filename, sizeof(filename), "%s.AFS", separator + 1);
    *separator = '\0';
    return ReadADFS_Wrapper(directory, filename, timeout);
}


void *LoadAudio(const char *sceneId, std::uint32_t sceneNumber, std::int32_t blockType) {
    const char *const language = g_usingJapaneseAudio ? "JAP" : "ENG";
    char path[80]{};
    std::snprintf(path, sizeof(path), "SCENE/%02u/STREAM/PC/%s/%s", sceneNumber, language, sceneId);
    char *const separator = std::strrchr(path, '/');
    char filename[16]{};
    std::snprintf(filename, sizeof(filename), "%s.IDX", separator + 1);
    *separator = '\0';
    char fullPath[272]{};
    std::snprintf(fullPath, 260, "%s/%s", path, filename);
    const std::int32_t fileSize = static_cast<std::int32_t>(OpenFileInternal(fullPath));
    void *const index =
        blockType ? HLib_AllocMemBlock(fileSize + 12, SHEN_NAME(CVIX)) : HLib_AllocHeapMemBlock(static_cast<std::int64_t>(fileSize) + 12, SHEN_NAME(CVIX));
    shenLoadFile(path, filename, index, fileSize);
    return index;
}

void SelectAudio() {
    const std::uint32_t sceneNumber = static_cast<std::uint32_t>(g_actNumber);
    if (sceneNumber == 99) {
        return;
    }

    char sceneId[16]{};
    std::snprintf(sceneId, sizeof(sceneId), "BGM0%u", sceneNumber);
    std::strcpy(g_preloadedAudioEntries[0].name, sceneId);
    g_preloadedAudioEntries[0].fileData = static_cast<HLib::TaskData *>(LoadAudio(sceneId, sceneNumber, 1));
    if (!g_preloadedAudioEntries[0].fileData)
        return;

    std::uint16_t entryCount = 0;
    std::memcpy(&entryCount, reinterpret_cast<const std::byte *>(g_preloadedAudioEntries[0].fileData) + 4, sizeof(entryCount));
    const std::uint64_t allocationSize = (2ULL * entryCount + 0x118ULL) & ~3ULL;
    g_bgmAdfs = HLib_AllocHeapMemBlock(static_cast<std::int64_t>(allocationSize), SHEN_NAME(ADFS));
}

namespace {

CVRSEntry *FindCVRSEntry(const char *name, bool allowEmpty) {
    if (!g_CVRS || !name)
        return nullptr;
    CVRSEntry *empty = nullptr;
    for (CVRSEntry &entry : g_CVRS->entries) {
        if (entry.name[0] == '\0') {
            if (!empty)
                empty = &entry;
            continue;
        }
        if (std::strcmp(entry.name, name) == 0)
            return &entry;
    }
    return allowEmpty ? empty : nullptr;
}

bool IsPreloadedAudio(const CVRSEntry &entry) {
    for (const CVRSEntry &cached : g_preloadedAudioEntries) {
        if (!cached.name[0])
            break;
        if (std::strcmp(cached.name, entry.name) == 0)
            return cached.fileData != nullptr;
    }
    return false;
}

void ReleaseAudioEntry(CVRSEntry &entry) {
    const bool sharedIndex = IsPreloadedAudio(entry);
    if (entry.afs) {
        PackAdfsWrapper *const wrapper = static_cast<PackAdfsWrapper *>(entry.afs);
        if (wrapper->taskData)
            HLib_FreeTaskData(wrapper->taskData, 0);
        wrapper->taskData = nullptr;
        wrapper->status = -1;
        entry.afs = nullptr;
    }
    if (entry.fileData && !sharedIndex)
        HLib_FreeTaskData(entry.fileData, 0);
    entry.fileData = nullptr;
    entry.name[0] = '\0';
}

void LoadAudioFile(const char *name) {
    if (!g_CVRS)
        return;
    std::size_t slot = 0;
    for (; slot < std::size(g_CVRS->entries); ++slot) {
        const CVRSEntry &entry = g_CVRS->entries[slot];
        if (entry.name[0] == '\0')
            break;
        if (std::strcmp(entry.name, name) == 0)
            return;
    }

    if (slot == std::size(g_CVRS->entries))
        return;
    CVRSEntry &entry = g_CVRS->entries[slot];
    std::strncpy(entry.name, name, sizeof(entry.name) - 1);
    entry.name[sizeof(entry.name) - 1] = '\0';
    HLib::TaskData *const preloaded = FindPreloadedAudioIndex(name);
    entry.fileData = preloaded ? preloaded : static_cast<HLib::TaskData *>(LoadAudio(name, static_cast<std::uint32_t>(g_sceneId), 0));
}


bool ReadAudioBankFromTable(const char *name) {
    CVRSEntry *const entry = FindCVRSEntry(name, false);
    if (!entry || entry->afs)
        return true;

    for (const CVRSEntry &cached : g_preloadedAudioEntries) {
        if (!cached.name[0])
            break;
        if (std::strcmp(cached.name, name) == 0) {
            entry->afs = cached.afs;
            break;
        }
    }
    if (!entry->afs) {
        std::uint16_t timeout = 0;
        std::memcpy(&timeout, reinterpret_cast<const std::byte *>(entry->fileData) + 4, sizeof(timeout));
        entry->afs = ReadAFS(name, timeout);
        entry->audioTable = const_cast<AudioTableBank *>(shenmue::s1::ReadAudioFromTable(name));
    }
    if (!entry->afs) {
        entry->name[0] = '\0';
        HLib_FreeTaskData(entry->fileData, 0);
    }
    return true;
}

void FreeAudioTaskData() {
    if (!g_CVRS)
        return;
    for (CVRSEntry &entry : g_CVRS->entries) {
        if (!entry.afs || IsPreloadedAudio(entry))
            continue;
        ReleaseAudioEntry(entry);
    }
}

void PlayDialogueByVoiceCode(const char *name) {
    if (!g_CVRS || !name)
        return;
    for (CVRSEntry &entry : g_CVRS->entries) {
        if (std::strcmp(entry.name, name) != 0 || IsPreloadedAudio(entry)) {
            continue;
        }
        ReleaseAudioEntry(entry);
    }
}

} // namespace

bool LoadAudioFile_Wrapper(std::int32_t mode, const char *name) {
    switch (mode) {
    case 0:
        PlayDialogueByVoiceCode(name);
        return true;
    case 1:
        LoadAudioFile(name);
        while (!ReadAudioBankFromTable(name)) {
        }
        return true;
    case 2:
        LoadAudioFile(name);
        return true;
    case 3:
        return ReadAudioBankFromTable(name);
    case 4:
        FreeAudioTaskData();
        return true;
    default:
        return true;
    }
}

namespace {
void SelectFREEAudio_0() {
    const std::int32_t currentScene = g_sceneId;
    constexpr const char *kFreeArchives[]{"FREE01", "FREE02", "FREE03"};
    for (std::int32_t scene = 1; scene <= 3; ++scene) {
        g_sceneId = scene;
        LoadAudioFile_Wrapper(1, kFreeArchives[static_cast<std::size_t>(scene - 1)]);
    }
    g_sceneId = currentScene;
}
} // namespace

void SetAudioLanguage(std::uint32_t japanese) {
    if (japanese > 1 || g_usingJapaneseAudio == japanese)
        return;
    g_usingJapaneseAudio = static_cast<std::uint8_t>(japanese);
    if (!g_CVRS)
        return;
    std::array<std::array<char, 24>, 20> names{};
    std::size_t count = 0;
    for (; count < names.size() && g_CVRS->entries[count].name[0]; ++count)
        std::strcpy(names[count].data(), g_CVRS->entries[count].name);
    FreeAudioTaskData();
    SelectFREEAudio_0();
    for (std::size_t index = 0; index < count; ++index) {
        LoadAudioFile(names[index].data());
        while (!ReadAudioBankFromTable(names[index].data())) {
        }
    }
}
void InitializeSceneDialogueAudio() {
    if (!g_CVRS)
        return;
    const char *sceneBgm = nullptr;
    switch (g_sceneId) {
    case 1:
    case 99:
        sceneBgm = "BGM01";
        break;
    case 2:
        sceneBgm = "BGM02";
        break;
    case 3:
        sceneBgm = "BGM03";
        break;
    default:
        break;
    }
    if (sceneBgm)
        LoadAudioFile_Wrapper(1, sceneBgm);

    SelectFREEAudio_0();
}

// @verified - p
bool EncodedSourceVoiceHasFinished(void *playback) {
#if defined(_WIN32)
    if (!playback)
        return true;
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    XAUDIO2_VOICE_STATE state{};
    static_cast<IXAudio2SourceVoice *>(playback)->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    return state.BuffersQueued == 0;
#else
    (void)playback;
    return true;
#endif
}
// @verified - p
void *GetSourceVoice() {
#if defined(_WIN32)
    for (std::uint32_t attempt = 0; attempt < kEncodedAudioVoiceCount; ++attempt) {
        g_nextDialogueVoice = (g_nextDialogueVoice + 1U) & 3U;
        IXAudio2SourceVoice *&voice = g_dialogueVoices[g_nextDialogueVoice];
        if (!voice)
            return &voice;
        if (EncodedSourceVoiceHasFinished(voice)) {
            voice->DestroyVoice();
            voice = nullptr;
            return &voice;
        }
    }
#endif
    return nullptr;
}
// @verified - p
void SetOutputAudioMatrix(void *playback, std::uint32_t sourceChannels) {
#if defined(_WIN32)
    IXAudio2SourceVoice *const voice = static_cast<IXAudio2SourceVoice *>(playback);
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
// @verified - p
void SetOutputMatrixVolumeAndChannels(void *playback, float volume, std::uint32_t sourceChannels) {
#if defined(_WIN32)
    IXAudio2SourceVoice *const voice = static_cast<IXAudio2SourceVoice *>(playback);
    if (g_aicaSubmixVoice && voice) {
        const float matrix[]{volume, volume};
        voice->SetOutputMatrix(g_aicaSubmixVoice, sourceChannels, 1, matrix);
    }
#else
    (void)playback;
    (void)volume;
    (void)sourceChannels;
#endif
}

// @verified - p
void *CreateAndSubmitSourceVoice(const AudioWaveData &wave, float volume) {
#if defined(_WIN32)
    if (!g_xaudio || !g_masteringVoice || !wave.format)
        return nullptr;
    IXAudio2SourceVoice **const output = static_cast<IXAudio2SourceVoice **>(GetSourceVoice());
    if (!output)
        return nullptr;
    const WAVEFORMATEX *const format = static_cast<const WAVEFORMATEX *>(wave.format);
    XAUDIO2_SEND_DESCRIPTOR descriptors[]{{XAUDIO2_SEND_USEFILTER, g_masteringVoice}, {XAUDIO2_SEND_USEFILTER, g_aicaSubmixVoice}};
    const XAUDIO2_VOICE_SENDS sends{.SendCount = 2, .pSends = descriptors};
    g_xaudio->CreateSourceVoice(output, format, 0, 2.0f, nullptr, &sends, nullptr);
    if (!*output)
        return nullptr;
    IXAudio2SourceVoice *const voice = *output;
    voice->FlushSourceBuffers();
    voice->SetSourceSampleRate(format->nSamplesPerSec);
    voice->SetVolume(volume);
    voice->SetFrequencyRatio(1.0f);
    SetOutputAudioMatrix(voice, format->nChannels);
    XAUDIO2_BUFFER buffer{};
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = wave.audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(wave.audio);
    const XAUDIO2_BUFFER_WMA packets{.pDecodedPacketCumulativeBytes = wave.packetBytes, .PacketCount = wave.packetCount};
    voice->SubmitSourceBuffer(&buffer, format->wFormatTag == WAVE_FORMAT_PCM ? nullptr : &packets);
    voice->Start();
    return voice;
#else
    (void)wave;
    (void)volume;
    return nullptr;
#endif
}
// @verified - p
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
// @verified - p
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
    StopSourceVoiceAndFlush(physicalId);
    owner->voice->SetSourceSampleRate(format->nSamplesPerSec);
    owner->voice->SetVolume(1.0f);
    owner->voice->SetFrequencyRatio(1.0f);
    SetOutputAudioMatrix(owner->voice, format->nChannels);
    SetOutputMatrixVolumeAndChannels(owner->voice, 0.1f, format->nChannels);
    XAUDIO2_BUFFER buffer{};
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = wave.audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(wave.audio);
    owner->voice->SubmitSourceBuffer(&buffer);
    owner->voice->Start();
#else
    (void)physicalId;
#endif
}
// @verified - p
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
    buffer.AudioBytes = wave.audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(wave.audio);
    buffer.PlayLength = wave.audioBytes / (format->nChannels * format->wBitsPerSample / 8);
    buffer.LoopBegin = wave.loopBegin;
    buffer.LoopLength = wave.loopLength;
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    owner->voice->SetSourceSampleRate(format->nSamplesPerSec);
    owner->voice->SetVolume(1.0f);
    owner->voice->SetFrequencyRatio(1.0f);
    SetOutputAudioMatrix(owner->voice, format->nChannels);
    owner->voice->SubmitSourceBuffer(&buffer);
    owner->voice->Start();
#else
    (void)physicalId;
#endif
}

std::uint32_t GetCurrentEncodedAudioVoiceHandle() {
#if defined(_WIN32)
    return g_nextDialogueVoice + 1;
#else
    return 5;
#endif
}
std::byte *GetEncodedAudioBuffer(std::uint32_t index) {
    return index < kEncodedAudioBufferCount ? g_encodedAudioBuffers[index] : nullptr;
}

// @verified - p
std::uint32_t LoadFileWrapper_0(std::int32_t fileId, std::int32_t entryIndex, float volume, const AudioTableRecord *record) {
    LargeFileHandle *const file = LargeFile_OpenTocEntryHandle(fileId, entryIndex);
    const std::uint32_t blocks = static_cast<std::uint32_t>(LargeFile_GetSizeInBlocks(file));
    std::uint32_t size = 0;
    if (file && file->entryIndex >= 0 && static_cast<std::size_t>(file->entryIndex) < g_largeFileData.size()) {
        const LargeFileDataEntry &data = g_largeFileData[static_cast<std::size_t>(file->entryIndex)];
        size = data.parent ? data.parent->tocEntries[data.tocIndex].size : static_cast<std::uint32_t>(OpenFileInternal(data.openPath));
    }
    const float reverb = record && record->metadata[11] ? DecibelsToLinear(3.0f * (record->metadata[11] - 15)) : 0.0f;
    std::byte *const bytes = GetEncodedAudioBuffer(g_nextEncodedAudioBuffer);
    std::uint32_t handle = 0;
    if (bytes && size <= kEncodedAudioBufferBytes && GetFileFromCachedBuffer(file, blocks, bytes, 1)) {
        ReadCachedFile(file);
        AudioWaveData wave{};
        handle = 5;
        if (ProcessWave(bytes, size, wave) >= 0) {
            void *const voice = CreateAndSubmitSourceVoice(wave, volume * 1.5f);
            if (voice) {
                handle = GetCurrentEncodedAudioVoiceHandle();
                if (!GetXAudioEffectFlagVal())
                    SetOutputMatrixVolumeAndChannels(voice, reverb, 1);
            }
        }
    }
    g_nextEncodedAudioBuffer = (g_nextEncodedAudioBuffer + 1) % kEncodedAudioBufferCount;
    return handle;
}

// @verified - p
bool LoadFileWrapperForAudio(CVWKAudioContext &context, std::int32_t fileId, std::uint32_t entryIndex, bool music, const AudioTableBank *bank) {
    if (context.sourceVoice) {
        DestroyEncodedAudioVoice(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(context.sourceVoice)));
        context.sourceVoice = nullptr;
    }
    if (g_audioMusicSuppressed && music)
        return false;
    const bool secondary = context.sourceVoice && DoXAudioStuff(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(context.sourceVoice)));
    context.streamBuffer = secondary ? g_voiceDecodeBuffer2 : g_voiceDecodeBuffer;
    context.task = secondary ? g_ahxDecodeBuffer2 : g_ahxDecodeBuffer;
    context.sourceVoice = nullptr;
    context.streamCookie = -1;
    context.playbackScale = 256;
    context.field28 = 0;
    context.fadeFrames = 30;
    const AudioTableRecord *const record = bank && entryIndex < bank->entryCount ? &bank->records[entryIndex] : nullptr;
    const std::uint32_t handle = LoadFileWrapper_0(
        fileId, static_cast<std::int32_t>(entryIndex), GetAdjustedVolumeForMusicOrDialogue(static_cast<std::int32_t>(context.volume), music), record);
    context.sourceVoice = reinterpret_cast<void *>(static_cast<std::uintptr_t>(handle));
    return true;
}
bool StartDialogueVoice(CVRSEntry &source, std::uint16_t entryIndex, CVWKAudioContext &context, bool music) {
    PackAdfsWrapper *const afs = static_cast<PackAdfsWrapper *>(source.afs);
    return LoadFileWrapperForAudio(context, afs ? afs->status : -1, entryIndex, music, static_cast<const AudioTableBank *>(source.audioTable));
}

// @verified - p
std::uint32_t DoXAudioStuff(std::uint32_t handle) {
#if defined(_WIN32)
    if (GetXAudioEffectFlagVal() || handle < 1 || handle > kEncodedAudioVoiceCount)
        return 0;
    IXAudio2SourceVoice *const voice = g_dialogueVoices[handle - 1];
    if (!voice)
        return 0;
    XAUDIO2_VOICE_STATE state{};
    voice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    return state.BuffersQueued ? 3 : 0;
#else
    (void)handle;
    return 0;
#endif
}

bool DialogueVoiceIsPlaying(const CVWKAudioContext &context) {
    return DoXAudioStuff(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(context.sourceVoice))) != 0;
}


bool Audio_HasMasteringVoice() {
#if defined(_WIN32)
    return g_masteringVoice != nullptr;
#else
    return false;
#endif
}

void SetDialogueVoiceVolume(CVWKAudioContext &context, std::int32_t volume, bool music) {
    std::memcpy(&context.streamCookie, &volume, sizeof(volume));
#if defined(_WIN32)
    const std::uintptr_t handle = reinterpret_cast<std::uintptr_t>(context.sourceVoice);
    if (!GetXAudioEffectFlagVal() && handle >= 1 && handle <= kEncodedAudioVoiceCount && g_dialogueVoices[handle - 1])
        g_dialogueVoices[handle - 1]->SetVolume(GetAdjustedVolumeForMusicOrDialogue(volume, music));
#else
    (void)music;
#endif
}

void SetDialogueVoiceLifetime(CVWKAudioContext &context, std::uint32_t lifetime) {
    static_assert(offsetof(CVWKAudioContext, streamCookie) == 0x18);
    std::memcpy(reinterpret_cast<std::byte *>(&context) + 0x1c, &lifetime, sizeof(lifetime));
}
void SetDialogueVoicePaused(CVWKAudioContext &context, bool paused) {
#if defined(_WIN32)
    const std::uintptr_t handle = reinterpret_cast<std::uintptr_t>(context.sourceVoice);
    if (!GetXAudioEffectFlagVal() && handle >= 1 && handle <= kEncodedAudioVoiceCount && g_dialogueVoices[handle - 1])
        g_dialogueVoices[handle - 1]->SetFrequencyRatio(paused ? 0.0f : 1.0f);
#endif
    context.field28 = paused ? 1U : 0U;
}

// @verified - p
void DestroyEncodedAudioVoice(std::uint32_t handle) {
#if defined(_WIN32)
    if (!GetXAudioEffectFlagVal() && handle >= 1 && handle <= kEncodedAudioVoiceCount) {
        IXAudio2SourceVoice *&voice = g_dialogueVoices[handle - 1];
        if (voice) {
            voice->Stop();
            voice->DestroyVoice();
            voice = nullptr;
        }
    }
#else
    (void)handle;
#endif
}

void DestroyDialogueVoice(CVWKAudioContext &context) {
    DestroyEncodedAudioVoice(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(context.sourceVoice)));
    context.sourceVoice = nullptr;
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

// @verified - p
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
    std::strcpy(g_audioDeviceNames[0].data(), "UIOTGLOFF0"); // @text EN: "Off" | JA: "オフ"
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
std::uint32_t GetAudioDeviceCount() {
#if defined(_WIN32)
    return g_audioDeviceCount;
#else
    return 0;
#endif
}
const char *GetAudioDeviceName(std::uint32_t index) {
#if defined(_WIN32)
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    GetAudioDeviceDetails();
    return index <= g_audioDeviceCount ? g_audioDeviceNames[index].data() : nullptr;
#else
    (void)index;
    return nullptr;
#endif
}
void QueueAudioDeviceChange(std::int32_t device) {
#if defined(_WIN32)
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    if (device != -2 && g_audioDeviceRequestCount < 5)
        g_audioDeviceRequests[g_audioDeviceRequestCount++] = device;
#else
    (void)device;
#endif
}

// @verified - p
void StopEngine(std::uint32_t voiceId) {
#if defined(_WIN32)
    XA_VoiceContext *const owner = ResolveRawVoice(GetRawSoundEffectVoice(voiceId));
    if (g_xaudio && g_masteringVoice && !GetXAudioEffectFlagVal() && owner && owner->voice)
        owner->voice->Stop();
#else
    (void)voiceId;
#endif
}

// @verified - p
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
    SetOutputMatrixVolumeAndChannels(owner->voice, volume, channels);

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
    if (g_xaudio && g_masteringVoice && !GetXAudioEffectFlagVal() && owner && owner->voice)
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
    format.nSamplesPerSec = 44100;
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

    SetOutputAudioMatrix(owner->voice, Routing(owner).channels);
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
    if (owner && owner->voice) {

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
    if (owner && owner->voice)
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
    if (!owner || !owner->voice || !levelMatrix || sourceChannels != Routing(owner).channels) {
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
// @verified - p
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
    if (paused && owner && owner->voice)
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

// @verified - p
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

// @verified - p
void EnvelopeGenerator_StopAndReset_Wrapper() {
#if defined(_WIN32)
    std::lock_guard<std::recursive_mutex> lock(g_audioInitializationMutex);
    for (std::uint32_t i = 0; i < g_audioDeviceRequestCount; ++i) {
        std::int32_t &requested = g_audioDeviceRequests[i];
        if (requested != g_audioDeviceIndex && requested != -2) {
            if (requested == -1)
                requested = 0;
            EnvelopeGenerator_StopAndReset();
            g_audioDeviceIndex = requested;
            g_audioDeviceSelected = true;
            InitializeXAudio();
            g_currentSettings.audioDevice = g_audioDeviceIndex;
            g_applicationSettings.savedAudioDevice = g_audioDeviceIndex;
        }
        requested = -2;
    }
    g_audioDeviceRequestCount = 0;
#endif
}
void RecoverXAudioDeviceIfNeeded() {
    EnvelopeGenerator_StopAndReset_Wrapper();
#if defined(_WIN32)
    if (GetXAudioEffectFlagVal() == 1) {
        QueueAudioDeviceChange(-1);
        EnvelopeGenerator_StopAndReset_Wrapper();
    }
#endif
}

} // namespace shenmue::s1
