#pragma once

#include "s1/d3t/namespace.hpp"
#include "s1/hlib.hpp"
#include "shenmue.h"

#if SHENMUE_IS_S2
#include "s2/aica_middleware.hpp"
#endif
#include <cstddef>
#include <cstdint>
#include <atomic>

#if defined(_WIN32)
#include <xaudio2.h>
#endif

namespace shenmue::s1 {

struct CVWKAudioContext;
struct PackAdfsWrapper;

inline constexpr std::uint32_t kXAudioVoiceCount = 160;
inline constexpr std::uint32_t kEncodedAudioVoiceCount = 4;
inline constexpr std::uint32_t kEncodedAudioBufferCount = 2;
inline constexpr std::uint32_t kEncodedAudioBufferBytes = 0x400000;
inline constexpr shen_name kVoiceBufferToken{SHEN_NAME(VBUF)};
inline constexpr shen_name kAhxBufferToken{SHEN_NAME(AHXB)};
inline constexpr shen_name kVoiceBuffer2Token{SHEN_NAME(VBU2)};
inline constexpr shen_name kAhxBuffer2Token{SHEN_NAME(AHX2)};

#if SHENMUE_IS_S2
using AicaState = ::shenmue::s2::aica::AicaState;
#else
struct AicaState {
    std::uint32_t phase;
    std::uint32_t uid;
    std::uint8_t egh;
    std::uint8_t lsl;
    std::uint8_t pad0A[2];
    float filterLevel0;
    float filterLevel1;
    float filterLevel2;
    float filterLevel3;
    float filterReleaseLevel;
    float filterAttackTime;
    float filterDecay1Time;
    float filterDecay2Time;
    float filterReleaseTime;
    float oneOverQ;
    std::uint8_t instantFilterAttack;
    std::uint8_t pad35[3];
    float currentFilterLevel;
    float attackRate;
    float decay1Rate;
    float decay2Rate;
    float releaseRate;
    float sustainLevel;
    float envelopeLevel;
    float linearVolume;
    float currentVolumeDb;
    float baseVolumeDb;
    float filterElapsed;
    std::uint32_t voiceId;
    void *aicaTone;
    std::uint32_t noteNo;
    std::uint32_t ownerId;
};
static_assert(sizeof(AicaState) == 0x78);
static_assert(offsetof(AicaState, currentFilterLevel) == 0x38);
static_assert(offsetof(AicaState, voiceId) == 0x64);
static_assert(offsetof(AicaState, aicaTone) == 0x68);

#endif

#if defined (_WIN32)
#pragma pack(push, 1)
struct AudioEngineCallbacks final : IXAudio2EngineCallback {
    std::atomic<std::uint32_t> effectFlag{0};
    virtual ~AudioEngineCallbacks() = default;
    void STDMETHODCALLTYPE OnProcessingPassStart() override;
    void STDMETHODCALLTYPE OnProcessingPassEnd() override;
    void STDMETHODCALLTYPE OnCriticalError(HRESULT error) override;
};
#pragma pack(pop)
static_assert(sizeof(AudioEngineCallbacks) == 12);
struct XA_VoiceContext final : IXAudio2VoiceCallback {
    AicaState *state{};
    std::uint32_t slotId{};
    std::uint32_t padding14{};
    IXAudio2SourceVoice *voice{};
    std::uint8_t active{};
    std::uint8_t reserved{};
    std::uint8_t padding22[6]{};
    HANDLE event{};
    XA_VoiceContext();
    virtual ~XA_VoiceContext();
    void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 bytesRequired) override;
    void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override;
    void STDMETHODCALLTYPE OnStreamEnd() override;
    void STDMETHODCALLTYPE OnBufferStart(void *context) override;
    void STDMETHODCALLTYPE OnBufferEnd(void *context) override;
    void STDMETHODCALLTYPE OnLoopEnd(void *context) override;
    void STDMETHODCALLTYPE OnVoiceError(void *context, HRESULT error) override;
};
static_assert(sizeof(XA_VoiceContext) == 48);
struct AudioVoiceRouting {
    IXAudio2Voice *dryOutputVoice{};
    IXAudio2Voice *reverbOutputVoice{};
    std::uint16_t channels{};
    std::uint16_t dryOutputChannels{};
    std::uint16_t reverbOutputChannels{};
};
#else
struct XA_VoiceContext {
    void *callback{};
    AicaState *state{};
    std::uint32_t slotId{};
    std::uint32_t padding14{};
    void *voice{};
    std::uint8_t active{};
    std::uint8_t reserved{};
    std::uint8_t padding22[6]{};
    void *event{};
};
#endif

struct RiffChunkView {
    const std::byte *data{};
    std::uint32_t size{};
};
inline constexpr std::int32_t kAudioPointerError = static_cast<std::int32_t>(0x80004003U);
inline constexpr std::int32_t kAudioFormatError = static_cast<std::int32_t>(0x80004005U);
inline constexpr std::int32_t kAudioEndOfFile = static_cast<std::int32_t>(0x80070026U);
inline constexpr std::int32_t kAudioUnsupportedFormat = static_cast<std::int32_t>(0x80070032U);
inline constexpr std::int32_t kAudioInvalidData = static_cast<std::int32_t>(0x8007000dU);
inline constexpr std::int32_t kAudioInvalidArgument = static_cast<std::int32_t>(0x80070057U);

struct AudioTableRecord {
    const char *archiveName;
    const char *fileName;
    std::uint8_t metadata[16];
};
struct AudioTableBank {
    const char *name;
    const AudioTableRecord *records;
    std::uint16_t lastEntry;
    std::uint16_t entryCount;
    std::uint32_t reserved;
};
static_assert(sizeof(AudioTableRecord) == 32);
static_assert(sizeof(AudioTableBank) == 24);

struct AudioWaveData {
    const void *format;
    const std::byte *audio;
    std::uint32_t audioBytes;
    std::uint32_t loopBegin;
    std::uint32_t loopLength;
    std::uint32_t reserved;
    const std::uint32_t *packetBytes;
    std::uint32_t packetCount;
    std::uint32_t padding;
};
static_assert(sizeof(AudioWaveData) == 48);
static_assert(offsetof(AudioWaveData, packetBytes) == 32);

extern void *g_voiceDecodeBuffer;
extern void *g_ahxDecodeBuffer;
extern void *g_voiceDecodeBuffer2;
extern void *g_ahxDecodeBuffer2;
void SetupVBUF();
float DecibelsToLinear(float decibels);
std::int32_t ParseXMA_Wave(const std::byte *bytes, std::size_t size, AudioWaveData &wave, bool &wma, bool &xma);
std::int32_t ParseWAV(const std::byte *bytes, std::size_t size, std::uint32_t tag, const std::uint32_t *&values, std::uint32_t &count);
std::int32_t ProcessWave(const std::byte *bytes, std::size_t size, AudioWaveData &wave);
const AudioTableBank *ReadAudioFromTable(const char *name);
namespace d3t {
void debugLog(const char *format, ...);
} // namespace d3t
extern bool g_audioFilterBypass;
extern std::int32_t g_audioMusicSuppressed;
std::uint32_t GetXAudioEffectFlagVal();
bool GetAudioDeviceDetails();
std::uint32_t GetAudioDeviceCount();
const char *GetAudioDeviceName(std::uint32_t index);
void QueueAudioDeviceChange(std::int32_t device);
void EnvelopeGenerator_StopAndReset_Wrapper();
void EnvelopeGenerator_StopAndReset();
void StopEngine(std::uint32_t voiceId);
std::uint32_t DoXAudioStuff(std::uint32_t handle);
void DestroyEncodedAudioVoice(std::uint32_t handle);
void *CreateAndSubmitSourceVoice(const AudioWaveData &wave, float volume);
std::uint32_t LoadFileWrapper_0(std::int32_t fileId, std::int32_t entryIndex, float volume, const AudioTableRecord *record);
bool LoadFileWrapperForAudio(CVWKAudioContext &context, std::int32_t fileId, std::uint32_t entryIndex, bool music, const AudioTableBank *bank);
void *GetSourceVoice();
bool EncodedSourceVoiceHasFinished(void *voice);
void SetOutputAudioMatrix(void *voice, std::uint32_t sourceChannels);
void SetOutputMatrixVolumeAndChannels(void *voice, float volume, std::uint32_t sourceChannels);
void CreateAndStartPlayingSourceVoice(std::uint32_t physicalId, const std::byte *bytes, std::size_t size);
void ProcessWave_Wrapper_0(std::uint32_t physicalId, const std::byte *bytes, std::size_t size);
void StopSourceVoiceAndFlush(std::uint32_t physicalId);
std::uint32_t GetCurrentEncodedAudioVoiceHandle();
std::byte *GetEncodedAudioBuffer(std::uint32_t index);

struct RawAudioSubmission {
    const void *format;
    const std::byte *audio;
    std::uint32_t audioBytes;
    std::uint32_t loopBegin;
    std::uint32_t loopLength;
    std::uint32_t reserved;
    float frequencyRatio;
    float volume;
    std::uint32_t music;
    std::uint32_t padding;
};
static_assert(sizeof(RawAudioSubmission) == 48);
void ConfigureAndSubmitXAudioBuffer(std::uint32_t voiceId, const RawAudioSubmission &submission);
void SetLowPassFilterParamsForAudio(std::uint32_t voiceId, float frequency, float oneOverQ);
void StopSourceVoiceAndFlush_0(std::uint32_t voiceId, bool release);
void InitializeEncodedAudioBuffers();
void ReleaseEncodedAudioBuffers();

#pragma pack(push, 1)
struct CVRSEntry {
    void *afs;
    HLib::TaskData *fileData;
    void *audioTable;
    char name[24];
};

struct CVRSTable {
    CVRSEntry entries[20];
};
#pragma pack(pop)

static_assert(sizeof(CVRSEntry) == 0x30);
static_assert(offsetof(CVRSEntry, fileData) == 0x08);
static_assert(offsetof(CVRSEntry, audioTable) == 0x10);
static_assert(offsetof(CVRSEntry, name) == 0x18);
static_assert(sizeof(CVRSTable) == 0x3c0);

extern CVRSTable *g_CVRS;
extern std::uint8_t g_usingJapaneseAudio;
extern CVRSEntry g_preloadedAudioEntries[2];
HLib::TaskData *FindPreloadedAudioIndex(const char *name);
PackAdfsWrapper *ReadAFS(const char *name, std::int32_t timeout);

bool InitializeXAudio();
void ShutdownXAudio();
void RecoverXAudioDeviceIfNeeded();
void SetAudioLanguage(std::uint32_t japanese);

float GetAdjustedVolumeForMusicOrDialogue(std::int32_t volume, bool music);
bool LoadAudioFile_Wrapper(std::int32_t mode, const char *name);
bool StartDialogueVoice(CVRSEntry &source, std::uint16_t entryIndex, CVWKAudioContext &context, bool music);
bool DialogueVoiceIsPlaying(const CVWKAudioContext &context);
bool Audio_HasMasteringVoice();
void SetDialogueVoiceVolume(CVWKAudioContext &context, std::int32_t volume, bool music);
void SetDialogueVoiceLifetime(CVWKAudioContext &context, std::uint32_t lifetime);
void DestroyDialogueVoice(CVWKAudioContext &context);
void SetDialogueVoicePaused(CVWKAudioContext &context, bool paused);
void InitializeSceneDialogueAudio();

void *StartRawSoundEffect(const std::byte *samples, std::uint32_t audioBytes, std::uint32_t loopBegin, std::uint32_t loopLength, std::uint16_t channels,
    float volume, float frequencyRatio, std::uint32_t sampleRate = 44100, bool routeThroughAica = false);
#if SHENMUE_IS_S2
void *PrepareRawSoundEffect(const std::byte *samples, std::uint32_t audioBytes, std::uint32_t loopBegin, std::uint32_t loopLength, std::uint16_t channels,
    float volume, float frequencyRatio, std::uint32_t sampleRate = 44100, bool reclaimable = false, std::uint16_t bitsPerSample = 16);
#else
void *PrepareRawSoundEffect(const std::byte *samples, std::uint32_t audioBytes, std::uint32_t loopBegin, std::uint32_t loopLength, std::uint16_t channels,
    float volume, float frequencyRatio, std::uint32_t sampleRate = 44100, bool routeThroughAica = false);
#endif
void StartRawSoundEffectPlayback(void *playback);



void *AllocateRawSoundEffectVoice(std::uint16_t bitsPerSample, std::uint16_t channels, bool reclaimable);
std::uint32_t RawSoundEffectVoiceId(const void *playback);
void *GetRawSoundEffectVoice(std::uint32_t physicalId);
AicaState *GetRawSoundEffectEnvelopeState(std::uint32_t physicalId);
bool RawSoundEffectVoiceIsActive(const void *playback);
bool RawSoundEffectVoiceIsReserved(const void *playback);
void ReleaseRawSoundEffectReservation(void *playback);
void SetRawSoundEffectEnvelopeCallbacks(void (*stop)(std::uint32_t), bool (*release)(std::uint32_t));
void StartRawSoundEffectVoice(void *playback);
void SubmitRawSoundEffectBuffer(void *playback, const std::byte *samples, std::uint32_t audioBytes, std::uint32_t playLength);
bool RawSoundEffectIsPlaying(const void *playback);
void SetRawSoundEffectVolume(void *playback, float volume);
void SetRawSoundEffectFrequencyRatio(void *playback, float frequencyRatio);
bool SetRawSoundEffectLowPassFrequency(void *playback, float normalizedFrequency, float oneOverQ = 1.0f);
bool SetRawSoundEffectOutputMatrix(void *playback, const float *levelMatrix, std::uint32_t sourceChannels, std::uint32_t destinationChannels);
void StopRawSoundEffectPlayback(void *playback, bool release = false);
void SetRawSoundEffectPaused(void *playback, bool paused);
void StopRawSoundEffect(void *&playback);

} // namespace shenmue::s1
