#include "s2/audio_data.hpp"
#include "s1/audio_stream.hpp"
#include "s2/aica_middleware.hpp"
#include "d3t/archive_handles.hpp"

#include "s2/archive_data.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "common/types.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/input_bindings.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <limits>
#include <span>
#include <mutex>
#include <thread>
#include <chrono>
#include <cwchar>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include "s2/audio_output_settings.hpp"
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#endif

namespace shenmue::s2 {

float g_s2CurrentSfxVolume = 1.0f;
float g_s2CurrentMusicVolume = 1.0f;
void *g_conversationVoiceBuffer{};
void *g_conversationCompressedBuffer{};
void *g_conversationSecondaryVoiceBuffer{};
void *g_conversationSecondaryCompressedBuffer{};

namespace {

constexpr std::string_view openingAixPath = "D:\\dolby\\S2_OP.aix";
constexpr std::size_t openingAixTaskSize = 0x6952c;
constexpr shen_name openingAixTaskName{SHEN_NAME(PXIA)};

FootstepAudioRequestSink footstepAudioRequestSink = nullptr;
OpeningAixRequestSink openingAixRequestSink = nullptr;
AicaBackendRequestSink aicaBackendRequestSink = nullptr;

struct ActiveOpeningAix {
    std::byte *taskData{};
    OpeningAixState state{OpeningAixState::Unsupported};
    bool active{};
};

ActiveOpeningAix activeOpeningAix{};
bool authorStreamingAudioLoaded{true};
std::recursive_mutex audioMutex;
std::array<std::byte *, CONVERSATION_WAVE_ARENA_COUNT> dialogueWaveArenas{};
std::uint32_t dialogueWaveArenaIndex;
std::uint32_t dialogueSourceIndex;

#if defined(_WIN32)
IXAudio2 *xaudio{};
IXAudio2MasteringVoice *masteringVoice{};
IXAudio2SubmixVoice *bankWetOutput{};
std::array<std::atomic<IXAudio2SourceVoice *>, CONVERSATION_SOURCE_COUNT> dialogueSources{};
std::atomic<std::uint32_t> dialogueCriticalError{};

struct DialogueAudioEngineCallback final : IXAudio2EngineCallback {
    void STDMETHODCALLTYPE OnProcessingPassStart() override {
    }
    void STDMETHODCALLTYPE OnProcessingPassEnd() override {
    }
    void STDMETHODCALLTYPE OnCriticalError(HRESULT error) override {
        if (error == static_cast<HRESULT>(0x88880001U)) {
            dialogueCriticalError = 1;
            for (std::atomic<IXAudio2SourceVoice *> &source : dialogueSources)
                source = nullptr;
        }
    }
};
DialogueAudioEngineCallback dialogueEngineCallback;

IXAudio2SourceVoice *GetDialogueSource(const DialogueVoice *voice) {
    const std::uint32_t slot = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(voice)) - 1U;
    return dialogueCriticalError == 0 && slot < CONVERSATION_SOURCE_COUNT ? dialogueSources[slot].load() : nullptr;
}

std::array<std::int32_t, 5> outputChangeQueue{};
std::int32_t outputChangeCount;
std::int32_t currentOutput{1};
std::int32_t acknowledgedOutput{-2};
struct AudioOutputDevice {
    char name[64];
    wchar_t id[256];
};
std::array<AudioOutputDevice, 13> outputDevices{};
std::uint32_t outputDeviceCount;
std::jthread audioOutputThread;

void EnumerateAudioOutputs() {
    const HRESULT apartment = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    IMMDeviceEnumerator *enumerator = nullptr;
    if (FAILED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&enumerator)))) {
        if (SUCCEEDED(apartment))
            CoUninitialize();
        return;
    }
    IMMDeviceCollection *devices = nullptr;
    if (SUCCEEDED(enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices))) {
        UINT count = 0;
        devices->GetCount(&count);
        if (count > 12)
            count = 12;
        outputDeviceCount = count ? count + 1 : 0;
        std::strcpy(outputDevices[0].name, "UIOTGLOFF0"); // @text EN: "Off" | JA: "いいえ"
        for (UINT index = 0; index < count; ++index) {
            AudioOutputDevice &destination = outputDevices[index + 1];
            destination = {};
            IMMDevice *device = nullptr;
            if (FAILED(devices->Item(index, &device)))
                continue;
            LPWSTR id = nullptr;
            if (SUCCEEDED(device->GetId(&id))) {
                std::wcsncpy(destination.id, id, 255);
                CoTaskMemFree(id);
            }
            IPropertyStore *properties = nullptr;
            if (SUCCEEDED(device->OpenPropertyStore(STGM_READ, &properties))) {
                PROPVARIANT name;
                PropVariantInit(&name);
                if (SUCCEEDED(properties->GetValue(PKEY_Device_FriendlyName, &name)) && name.vt == VT_LPWSTR)
                    std::wcstombs(destination.name, name.pwszVal, sizeof(destination.name) - 1);
                PropVariantClear(&name);
                properties->Release();
            }
            device->Release();
        }
        devices->Release();
    }
    enumerator->Release();
    if (SUCCEEDED(apartment))
        CoUninitialize();
}

void ResetAudioOutput() {
    aica::ResetPlatformVoices();
    for (std::atomic<IXAudio2SourceVoice *> &slot : dialogueSources) {
        IXAudio2SourceVoice *source = slot.exchange(nullptr);
        if (source)
            source->DestroyVoice();
    }
    if (bankWetOutput)
        bankWetOutput->DestroyVoice();

    bankWetOutput = nullptr;
    if (masteringVoice)
        masteringVoice->DestroyVoice();
    masteringVoice = nullptr;
    if (xaudio)
        xaudio->Release();
    xaudio = nullptr;
}

void ProcessAudioOutputChanges() {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    for (std::int32_t index = 0; index < outputChangeCount; ++index) {
        std::int32_t &device = outputChangeQueue[index];
        if (device != currentOutput && device != -2) {
            if (device == -1)
                device = 0;
            ResetAudioOutput();
            currentOutput = device;
            InitializeDialogueAudio();
            acknowledgedOutput = currentOutput;
        }
        device = -2;
    }
    outputChangeCount = 0;
}
#endif

std::uint16_t ReadU16(std::span<const std::byte> bytes, std::size_t offset) noexcept {
    std::uint16_t value{};
    if (offset <= bytes.size() && sizeof(value) <= bytes.size() - offset)
        std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return value;
}

std::uint32_t ReadU32(std::span<const std::byte> bytes, std::size_t offset) noexcept {
    std::uint32_t value{};
    if (offset <= bytes.size() && sizeof(value) <= bytes.size() - offset)
        std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return value;
}

bool ReadConversationVoiceEntry(std::int32_t fileId, std::uint16_t entryIndex, std::span<const std::byte> &bytes) {
    d3t::LargeFileHandle *request = d3t::LargeFile_OpenTocEntryHandle(fileId, entryIndex);
    const std::uint64_t blocks = static_cast<std::uint64_t>(d3t::LargeFile_GetSizeInBlocks(request));
    std::uint32_t contentSize = 0;
    if (request && static_cast<std::uint32_t>(request->entryIndex) < 2048U) {
        const d3t::LargeFileDataEntry &entry = d3t::g_largeFileData[static_cast<std::size_t>(request->entryIndex)];
        if (entry.parent)
            contentSize = entry.parent->tocEntries[entry.tocIndex].size;
        else
            contentSize = static_cast<std::uint32_t>(ArchiveData_GetFileSize(entry.openPath));
    }
    std::byte *destination = dialogueWaveArenas[dialogueWaveArenaIndex];
    if (!d3t::GetFileFromCachedBuffer(request, blocks, destination, 1))
        return false;
    d3t::ReadCachedFile(request);
    bytes = {destination, contentSize};
    return true;
}

#if defined(_WIN32)
struct RiffWaveView {
    const WAVEFORMATEX *format{};
    WAVEFORMATEX pcmFormat{};
    const std::byte *audio{};
    std::uint32_t audioBytes{};
    const std::uint32_t *packetBytes{};
    std::uint32_t packetCount{};
};

bool ParseRiffWave(std::span<const std::byte> bytes, RiffWaveView &result) noexcept {
    if (bytes.size() < 12 || ReadU32(bytes, 0) != SHEN_NAME(RIFF)) {
        return false;
    }
    const std::uint32_t form = ReadU32(bytes, 8);
    if (form != SHEN_NAME(WAVE) && form != SHEN_NAME(XWMA)) {
        return false;
    }
    std::size_t offset = 12;
    while (offset + 8 <= bytes.size()) {
        const std::uint32_t tag = ReadU32(bytes, offset);
        const std::uint32_t size = ReadU32(bytes, offset + 4);
        offset += 8;
        if (size > bytes.size() - offset)
            return false;
        if (tag == make_shen_name('f', 'm', 't', ' ')) {
            if (size == 16 && ReadU16(bytes, offset) == WAVE_FORMAT_PCM) {
                std::memcpy(&result.pcmFormat, bytes.data() + offset, 16);
                result.format = &result.pcmFormat;
            } else if (size >= sizeof(WAVEFORMATEX) && ReadU16(bytes, offset + 16) <= size - sizeof(WAVEFORMATEX)) {
                result.format = reinterpret_cast<const WAVEFORMATEX *>(bytes.data() + offset);
            } else
                return false;
        } else if (tag == SHEN_NAME(data)) {
            result.audio = bytes.data() + offset;
            result.audioBytes = size;
        } else if (tag == SHEN_NAME(dpds) && size % sizeof(std::uint32_t) == 0) {
            result.packetBytes = reinterpret_cast<const std::uint32_t *>(bytes.data() + offset);
            result.packetCount = size / sizeof(std::uint32_t);
        }
        offset += (static_cast<std::size_t>(size) + 1U) & ~1U;
    }
    return result.format && result.audio && result.audioBytes;
}

IXAudio2SourceVoice *SubmitDialogueWave(const RiffWaveView &wave, float gain) {
    if (!xaudio || !masteringVoice || !wave.format)
        return nullptr;
    IXAudio2SourceVoice *source = nullptr;
    for (std::uint32_t attempt = 0; attempt < CONVERSATION_SOURCE_COUNT; ++attempt) {
        dialogueSourceIndex = (dialogueSourceIndex + 1U) & 3U;
        source = dialogueSources[dialogueSourceIndex].load();
        if (!source)
            break;
        XAUDIO2_VOICE_STATE state{};
        source->GetState(&state, 0);
        if (!state.BuffersQueued) {
            source->DestroyVoice();
            dialogueSources[dialogueSourceIndex] = nullptr;
            source = nullptr;
            break;
        }
        if (attempt + 1U == CONVERSATION_SOURCE_COUNT)
            return nullptr;
    }
    if (!source) {
        XAUDIO2_SEND_DESCRIPTOR destinations[]{{XAUDIO2_SEND_USEFILTER, masteringVoice}, {XAUDIO2_SEND_USEFILTER, bankWetOutput}};
        const XAUDIO2_VOICE_SENDS sends{2, destinations};
        xaudio->CreateSourceVoice(&source, wave.format, 0, 4.0f, nullptr, &sends);
        dialogueSources[dialogueSourceIndex] = source;
    }
    if (!source)
        return nullptr;
    source->FlushSourceBuffers();
    source->SetSourceSampleRate(wave.format->nSamplesPerSec);
    source->SetVolume(gain);
    source->SetFrequencyRatio(1.0f);
    const float *outputMatrix = wave.format->nChannels == 2 ? CONVERSATION_STEREO_OUTPUT_MATRIX.data() : CONVERSATION_MONO_OUTPUT_MATRIX.data();
    source->SetOutputMatrix(masteringVoice, wave.format->nChannels, 6, outputMatrix);
    XAUDIO2_BUFFER buffer{};
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = wave.audioBytes;
    buffer.pAudioData = reinterpret_cast<const BYTE *>(wave.audio);
    XAUDIO2_BUFFER_WMA wma{};
    wma.PacketCount = wave.packetCount;
    wma.pDecodedPacketCumulativeBytes = wave.packetBytes;
    source->SubmitSourceBuffer(&buffer, wave.format->wFormatTag == WAVE_FORMAT_PCM ? nullptr : &wma);
    if (bankWetOutput) {
        const float wetMatrix[]{0.0f, 0.0f};
        source->SetOutputMatrix(bankWetOutput, wave.format->nChannels, 1, wetMatrix);
    }
    source->Start();
    return source;
}

#endif
OpeningAixState BuiltInOpeningAixBackend(OpeningAixOperation operation, const OpeningAixRequest &request) noexcept;

OpeningAixState DispatchOpeningAix(OpeningAixOperation operation) noexcept {
    const OpeningAixRequest request{openingAixPath, activeOpeningAix.taskData, openingAixTaskSize};
    return openingAixRequestSink ? openingAixRequestSink(operation, request) : BuiltInOpeningAixBackend(operation, request);
}

void ReleaseOpeningAixTaskData() noexcept {
    if (activeOpeningAix.taskData) {
        HLib::FreeTaskData(activeOpeningAix.taskData);
        activeOpeningAix.taskData = nullptr;
    }
}

void StopOpeningAix() noexcept {
    if (activeOpeningAix.active && openingAixRequestSink) {
        (void)DispatchOpeningAix(OpeningAixOperation::Stop);
    }
    ReleaseOpeningAixTaskData();
    activeOpeningAix.state = OpeningAixState::Complete;
    activeOpeningAix.active = false;
    authorStreamingAudioLoaded = false;
}

OpeningAixState BuiltInOpeningAixBackend(OpeningAixOperation operation, const OpeningAixRequest &request) noexcept {
    (void)operation;
    (void)request;
    // The game owns the AIX program and the platform owns its Dolby/XAudio
    // decoder.  A platform adapter may be registered above; this source file
    // must not substitute an IDX/AFS voice for S2_OP.aix.
    return OpeningAixState::Unsupported;
}

} // namespace

bool InitializeDialogueAudio() noexcept {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    for (std::byte *&arena : dialogueWaveArenas)
        if (!arena)
            arena = static_cast<std::byte *>(std::malloc(CONVERSATION_WAVE_ARENA_SIZE));
#if defined(_WIN32)
    if (xaudio)
        return masteringVoice != nullptr;
    EnumerateAudioOutputs();
    if (static_cast<std::uint32_t>(currentOutput) >= outputDeviceCount)
        currentOutput = 0;
    if (!currentOutput || !outputDeviceCount)
        return false;
    if (FAILED(XAudio2Create(&xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        return false;
    dialogueCriticalError = 0;
    xaudio->RegisterForCallbacks(&dialogueEngineCallback);
    const wchar_t *const device = outputDevices[currentOutput].id;
    if (FAILED(xaudio->CreateMasteringVoice(&masteringVoice, 6, 48000, 0, device))) {
        xaudio->Release();
        xaudio = nullptr;
        return false;
    }
    IUnknown *limiter = nullptr;
    CreateFX(CLSID_FXMasteringLimiter, &limiter);
    XAUDIO2_EFFECT_DESCRIPTOR limiterDescriptor{.pEffect = limiter, .InitialState = TRUE, .OutputChannels = 6};
    const XAUDIO2_EFFECT_CHAIN limiterChain{.EffectCount = 1, .pEffectDescriptors = &limiterDescriptor};
    masteringVoice->SetEffectChain(&limiterChain);
    masteringVoice->SetEffectParameters(0, &CONVERSATION_MASTERING_LIMITER, sizeof(CONVERSATION_MASTERING_LIMITER));
    if (limiter)
        limiter->Release();
    IUnknown *reverb = nullptr;
    if (FAILED(XAudio2CreateReverb(&reverb)))
        return false;
    XAUDIO2_EFFECT_DESCRIPTOR descriptor{reverb, TRUE, 1};
    const XAUDIO2_EFFECT_CHAIN chain{1, &descriptor};
    const HRESULT created = xaudio->CreateSubmixVoice(&bankWetOutput, 1, 48000, 0, 0, nullptr, &chain);
    reverb->Release();
    if (FAILED(created))
        return false;
    bankWetOutput->SetVolume(CONVERSATION_WET_OUTPUT_GAIN);
    XAUDIO2FX_REVERB_PARAMETERS parameters{};
    ReverbConvertI3DL2ToNative(&CONVERSATION_REVERB_PRESET, &parameters, FALSE);
    bankWetOutput->SetEffectParameters(0, &parameters, sizeof(parameters));
    return true;
#else
    return false;
#endif
}

namespace aica {

std::recursive_mutex &GetAudioBackendMutex() noexcept {
    return audioMutex;
}

void ResetPlatformVoices() noexcept {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    s1::ShutdownXAudio();
}

void *GetRawSoundEffectVoice(std::uint32_t index) {
    return s1::GetRawSoundEffectVoice(index);
}

AicaState *GetRawSoundEffectEnvelopeState(std::uint32_t index) {
    return s1::GetRawSoundEffectEnvelopeState(index);
}

std::uint32_t RawSoundEffectVoiceId(void *playback) {

    return s1 ::RawSoundEffectVoiceId(playback);
}

bool RawSoundEffectVoiceIsActive(void *playback) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);

    return s1::RawSoundEffectVoiceIsActive(playback);
}

void ReleaseRawSoundEffectReservation(void *playback) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    s1::ReleaseRawSoundEffectReservation(playback);
}

void *PrepareRawSoundEffect(const std::byte *samples, std::uint32_t audioBytes, std::uint32_t loopBegin, std::uint32_t loopLength, std::uint16_t channels,
    float volume, float frequencyRatio, std::uint32_t sampleRate, bool reclaimable, std::uint16_t bitsPerSample) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);

    return s1::PrepareRawSoundEffect(samples, audioBytes, loopBegin, loopLength, channels, volume, frequencyRatio, sampleRate, reclaimable, bitsPerSample);
}

void SetDialogueStreamsPaused(bool paused) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
#if defined(_WIN32)
    for (std::atomic<IXAudio2SourceVoice *> &slot : dialogueSources) {
        IXAudio2SourceVoice *source = slot.load();
        if (source) {
            if (paused)
                source->Stop();
            else
                source->Start();
        }
    }
#else
    (void)paused;
#endif
}

void StartRawSoundEffectPlayback(void *playback) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    s1::StartRawSoundEffectPlayback(playback);
}

void StopRawSoundEffectPlayback(void *playback, bool releaseEnvelope) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    s1::StopRawSoundEffectPlayback(playback, releaseEnvelope);
    }

void SetRawSoundEffectPaused(void *playback, bool paused) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);

        if (paused)
            s1::SetRawSoundEffectPaused(playback, true);
        else
            s1::StartRawSoundEffectPlayback(playback);
}

void SetRawSoundEffectVolume(void *playback, float volume) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    s1::SetRawSoundEffectVolume(playback, volume);
}

void SetRawSoundEffectFrequencyRatio(void *playback, float ratio) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    s1 ::SetRawSoundEffectFrequencyRatio(playback, ratio);
}

void SetRawSoundEffectLowPassFrequency(void *playback, float frequency, float oneOverQ) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    (void)s1 ::SetRawSoundEffectLowPassFrequency(playback, frequency, oneOverQ);
}

void SetRawSoundEffectOutputMatrix(void *playback, const float *matrix, std::uint32_t sourceChannels, std::uint32_t outputChannels) {
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    (void)s1::SetRawSoundEffectOutputMatrix(playback, matrix, sourceChannels, outputChannels);
}
} // namespace aica

void SetFootstepAudioRequestSink(FootstepAudioRequestSink sink) noexcept {
    footstepAudioRequestSink = sink;
}

bool SubmitFootstepAudioRequest(const FootstepAudioRequest &request) noexcept {
    if (!footstepAudioRequestSink)
        return false;
    footstepAudioRequestSink(request);
    return true;
}

void AllocateConversationStreamBuffers() {
    g_conversationVoiceBuffer = HLib::AllocateTaskData(CONVERSATION_VOICE_BUFFER_SIZE, CONVERSATION_VOICE_BUFFER);
    g_conversationCompressedBuffer = HLib::AllocateTaskData(CONVERSATION_COMPRESSED_BUFFER_SIZE, CONVERSATION_COMPRESSED_BUFFER);
    g_conversationSecondaryVoiceBuffer = HLib::AllocateTaskData(CONVERSATION_SECONDARY_VOICE_BUFFER_SIZE, CONVERSATION_SECONDARY_VOICE_BUFFER);
    g_conversationSecondaryCompressedBuffer = HLib::AllocateTaskData(CONVERSATION_COMPRESSED_BUFFER_SIZE, CONVERSATION_SECONDARY_COMPRESSED_BUFFER);
}

DialogueVoice *StartDialogueVoice(std::int32_t fileId, std::uint16_t entry, float gain) noexcept {
#if !defined(_WIN32)
    (void)fileId;
    (void)entry;
    (void)gain;
    return nullptr;
#else
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    InitializeDialogueAudio();
    std::span<const std::byte> bytes;
    DialogueVoice *result = nullptr;
    if (ReadConversationVoiceEntry(fileId, entry, bytes)) {
        RiffWaveView wave{};
        result = reinterpret_cast<DialogueVoice *>(5U);
        if (ParseRiffWave(bytes, wave) && SubmitDialogueWave(wave, gain * CONVERSATION_VOICE_SPAWN_GAIN))
            result = reinterpret_cast<DialogueVoice *>(static_cast<std::uintptr_t>(dialogueSourceIndex + 1U));
    }
    dialogueWaveArenaIndex = (dialogueWaveArenaIndex + 1U) & 1U;
    return result;
#endif
}

void PlayWaveBuffer(const std::byte *data, std::int32_t size, float gain) {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    if (!InitializeDialogueAudio())
        return;
    RiffWaveView wave{};
    if (ParseRiffWave({data, static_cast<std::size_t>(static_cast<std::int64_t>(size))}, wave))
        SubmitDialogueWave(wave, gain);
#else
    (void)data;
    (void)size;
    (void)gain;
#endif
}

DialogueVoiceState PollDialogueVoice(const DialogueVoice *voice) noexcept {
#if !defined(_WIN32)
    (void)voice;
    return DialogueVoiceState::Stopped;
#else
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    IXAudio2SourceVoice *source = GetDialogueSource(voice);
    if (!source)
        return DialogueVoiceState::Stopped;
    XAUDIO2_VOICE_STATE state{};
    source->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    return state.BuffersQueued ? DialogueVoiceState::Playing : DialogueVoiceState::Stopped;
#endif
}

bool GetDialogueVoicePosition(const DialogueVoice *voice, std::uint64_t &samples, std::uint32_t &sampleRate) noexcept {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    IXAudio2SourceVoice *source = GetDialogueSource(voice);
    if (!source)
        return false;
    XAUDIO2_VOICE_DETAILS details{};
    XAUDIO2_VOICE_STATE state{};
    source->GetVoiceDetails(&details);
    source->GetState(&state, 0);
    if (!state.BuffersQueued)
        return false;
    samples = static_cast<std::uint32_t>(state.SamplesPlayed);
    sampleRate = details.InputSampleRate;
    return true;
#else
    (void)voice;
    (void)samples;
    (void)sampleRate;
    return false;
#endif
}

void StopDialogueVoice(DialogueVoice *voice) noexcept {
    if (!voice)
        return;
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    IXAudio2SourceVoice *source = GetDialogueSource(voice);
    if (source) {
        source->Stop();
        source->DestroyVoice();
        dialogueSources[static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(voice)) - 1U] = nullptr;
    }
#endif
}

bool PauseDialogueVoice(DialogueVoice *voice, std::int32_t paused) noexcept {
#if !defined(_WIN32)
    (void)voice;
    (void)paused;
    return false;
#else
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    IXAudio2SourceVoice *source = GetDialogueSource(voice);
    return source && SUCCEEDED(source->SetFrequencyRatio(paused == 1 ? 0.0f : 1.0f));
#endif
}

bool SetDialogueVoiceVolume(DialogueVoice *voice, float gain) noexcept {
#if !defined(_WIN32)
    (void)voice;
    (void)gain;
    return false;
#else
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    IXAudio2SourceVoice *source = GetDialogueSource(voice);
    return source && xaudio && masteringVoice && SUCCEEDED(source->SetVolume(gain));
#endif
}

namespace {
void SetVoiceReverbOutputMatrix(void *voice, float gain, std::uint32_t channels) {
#if defined(_WIN32)
    if (!bankWetOutput || !voice)
        return;
    const std::array<float, 2> matrix{gain, gain};
    static_cast<IXAudio2SourceVoice *>(voice)->SetOutputMatrix(bankWetOutput, channels, 1, matrix.data(), 0);
#else
    (void)voice;
    (void)gain;
    (void)channels;
#endif
}
} // namespace

void SetDialogueVoiceReverbAttenuation(DialogueVoice *voice, std::int32_t, std::int32_t attenuation) noexcept {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    IXAudio2SourceVoice *const source = GetDialogueSource(voice);
    if (!source)
        return;
    const float decibels = static_cast<float>(attenuation);
    const float gain = decibels > -64.0f ? std::pow(10.0f, decibels / 20.0f) : 0.0f;
    SetVoiceReverbOutputMatrix(source, gain * 3.0f, 1);
#else
    (void)voice;
    (void)attenuation;
#endif
}

bool IsDialogueAudioActive() noexcept {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    return masteringVoice != nullptr;
#else
    return false;
#endif
}

void ReleaseDialogueAudio() noexcept {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    ResetAudioOutput();
#endif
    for (std::byte *&arena : dialogueWaveArenas) {
        std::free(arena);
        arena = nullptr;
    }
}

std::string_view GetS2AudioOutputName(std::int32_t index) noexcept {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    EnumerateAudioOutputs();
    return static_cast<std::uint32_t>(index) < outputDeviceCount ? std::string_view(outputDevices[index].name) : std::string_view{};
#else
    static_cast<void>(index);
    return {};
#endif
}

void QueueS2AudioOutputChange(std::int32_t index) {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    if (outputChangeCount < 5 && index != -2)
        outputChangeQueue[outputChangeCount++] = index;
    if (!audioOutputThread.joinable())
        audioOutputThread = std::jthread([](std::stop_token stop) {
            const HRESULT apartment = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            while (!stop.stop_requested()) {
                ProcessAudioOutputChanges();
                std::this_thread::sleep_for(std::chrono::milliseconds(4));
            }
            if (SUCCEEDED(apartment))
                CoUninitialize();
        });
#else
    static_cast<void>(index);
#endif
}

void PublishS2AudioOutputAcknowledgement() {
#if defined(_WIN32)
    const std::lock_guard<std::recursive_mutex> lock(audioMutex);
    if (acknowledgedOutput != -2) {
        AcknowledgeS2AudioOutput(acknowledgedOutput);
        acknowledgedOutput = -2;
    }
#endif
}

void ApplyS2AudioVolumeSettings() noexcept {
    if (!aicaBackendRequestSink)
        return;
    const S2AudioSettings &settings = GetS2AudioSettings();
    g_s2CurrentSfxVolume = settings.sfxVolume;
    g_s2CurrentMusicVolume = settings.musicVolume;
    aica::RefreshOutputVolumes();
}
void SetOpeningAixRequestSink(OpeningAixRequestSink sink) noexcept {
    StopOpeningAix();
    openingAixRequestSink = sink;
}

bool InitializeOpeningAix() noexcept {
    if (activeOpeningAix.active)
        return true;
    ReleaseOpeningAixTaskData();
    activeOpeningAix.state = OpeningAixState::Unsupported;
    authorStreamingAudioLoaded = false;
    if (!openingAixRequestSink)
        return false;

    activeOpeningAix.taskData = static_cast<std::byte *>(HLib::AllocateTaskData(openingAixTaskSize, openingAixTaskName));
    if (!activeOpeningAix.taskData) {
        activeOpeningAix.state = OpeningAixState::Failed;
        return false;
    }
    std::memset(activeOpeningAix.taskData, 0, openingAixTaskSize);
    const OpeningAixState state = DispatchOpeningAix(OpeningAixOperation::Start);
    activeOpeningAix.state = state;
    if (state == OpeningAixState::Unsupported || state == OpeningAixState::Failed) {
        ReleaseOpeningAixTaskData();
        return false;
    }
    activeOpeningAix.active = true;
    authorStreamingAudioLoaded = true;
    return true;
}

OpeningAixState PollOpeningAix() noexcept {
    if (!activeOpeningAix.active)
        return activeOpeningAix.state;
    const OpeningAixState state = DispatchOpeningAix(OpeningAixOperation::Poll);
    activeOpeningAix.state = state;
    if (state == OpeningAixState::Unsupported || state == OpeningAixState::Failed) {
        StopOpeningAix();
        activeOpeningAix.state = state;
    } else if (state == OpeningAixState::Complete) {
        ReleaseOpeningAixTaskData();
        activeOpeningAix.active = false;
        authorStreamingAudioLoaded = false;
    }
    return state;
}

void FinalizeOpeningAix() noexcept {
    StopOpeningAix();
}

// @verified - p
bool IsAuthorStreamingAudioLoaded() noexcept {
    return authorStreamingAudioLoaded;
}

std::string_view OpeningAixPath() noexcept {
    return openingAixPath;
}

void SetAicaBackendRequestSink(AicaBackendRequestSink sink) noexcept {
    aicaBackendRequestSink = sink;
}

std::int32_t SubmitAicaBackendRequest(const AicaBackendRequest &request) noexcept {
    return aicaBackendRequestSink ? aicaBackendRequestSink(request) : -1;
}

} // namespace shenmue::s2
