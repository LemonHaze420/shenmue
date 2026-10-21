#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>

namespace shenmue::s2::aica {

inline constexpr std::int64_t AICA_UPDATE_INTERVAL_NS = 4000000;
inline constexpr double AICA_WORKER_SLEEP_FRACTION = 0.7;
inline constexpr std::uint32_t CORRECTED_SAMPLE_DATA_OFFSET = 0xb975a;
inline constexpr std::uint32_t CORRECTED_SAMPLE_BYTE_COUNT = 18182;



std::recursive_mutex &GetAudioBackendMutex() noexcept;
enum AicaEnvelopePhase : std::uint32_t {
    AICA_ENVELOPE_OFF, AICA_ENVELOPE_ATTACK, AICA_ENVELOPE_HOLD, AICA_ENVELOPE_DECAY1, AICA_ENVELOPE_DECAY2, AICA_ENVELOPE_RELEASE
};
inline constexpr std::int32_t AICA_MODERN_ENVELOPE_STEP = 6;
inline constexpr float AICA_MODERN_ENVELOPE_DECAY_BASE = 0.999888f;

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
    float holdTime;
    float decay1Rate;
    float decay2Rate;
    float releaseRate;
    float sustainLevel;
    float envelopeLevel;
    float linearVolume;
    float currentVolume;
    float baseVolumeDb;
    float filterElapsed;
    std::uint32_t voiceId;
    std::byte reserved_6c[4];
    void *aicaTone;
    std::uint32_t noteNo;
    std::int32_t envelopeCounter;
    std::int32_t releaseStartLevel;
    std::uint32_t ownerId;
    std::uint8_t legacyEnvelope;
    std::byte reserved_89[7];
};
static_assert(sizeof(AicaState) == 0x90);
static_assert(offsetof(AicaState, currentFilterLevel) == 0x38);
static_assert(offsetof(AicaState, attackRate) == 0x3c);
static_assert(offsetof(AicaState, holdTime) == 0x40);
static_assert(offsetof(AicaState, decay1Rate) == 0x44);
static_assert(offsetof(AicaState, releaseRate) == 0x4c);
static_assert(offsetof(AicaState, envelopeLevel) == 0x54);
static_assert(offsetof(AicaState, voiceId) == 0x68);
static_assert(offsetof(AicaState, aicaTone) == 0x70);
static_assert(offsetof(AicaState, envelopeCounter) == 0x7c);
static_assert(offsetof(AicaState, releaseStartLevel) == 0x80);
static_assert(offsetof(AicaState, ownerId) == 0x84);
static_assert(offsetof(AicaState, legacyEnvelope) == 0x88);
struct AicaEnvironmentPartState {
    std::uint8_t active;
    std::uint8_t flags;
    std::uint8_t voiceIndex;
    std::byte reserved;
};

struct AicaEnvironmentState {
    std::uint8_t loadedBank;
    std::uint8_t active;
    std::uint8_t bankType;
    std::uint8_t volume;
    std::uint8_t control;
    std::int8_t coarsePitch;
    std::int8_t finePitch;
    std::uint8_t currentValue;
    std::uint8_t maximumPart;
    std::uint8_t field09;
    std::uint8_t mixLevel;
    std::uint8_t field0b;
    std::int8_t volumeOffset;
    std::byte reserved_0d[3];
    std::uint8_t *partTable;
    AicaEnvironmentPartState parts[16];
};

static_assert(sizeof(AicaEnvironmentPartState) == 4);
static_assert(sizeof(AicaEnvironmentState) == 88);
static_assert(offsetof(AicaEnvironmentState, partTable) == 16);
static_assert(offsetof(AicaEnvironmentState, parts) == 24);
struct AicaBankSlot {
    std::int32_t bankIdOrType;
    std::byte reserved_04[12];
    std::uint8_t *bankData;
    std::uint32_t loadedSize;
    std::uint8_t field_1c;
    std::uint8_t field_1d;
    std::byte reserved_1e[2];
};

static_assert(sizeof(AicaBankSlot) == 0x20);
static_assert(offsetof(AicaBankSlot, bankData) == 0x10);

struct AicaVoiceSlot {
    std::uint8_t status;
    std::uint8_t playState;
    std::int8_t ownerId;
    std::uint8_t driverFlags;
    std::uint8_t layerFlags;
    std::uint8_t playFlags;
    std::uint8_t bank;
    std::uint8_t part;
    std::uint8_t noteAttenuation;
    std::uint8_t stereo;
    std::byte reserved_0a[2];
    std::uint8_t previousSlot;
    std::uint8_t nextSlot;
    std::byte reserved_0e[2];
    std::uint32_t field_10;
    std::uint32_t field_14;
    std::uint32_t spawnCommand;
    std::int32_t pitchOrBaseNote;
    std::uint64_t driverNoteTable;
    std::uint64_t driverPointerTable;
    std::uint32_t xaudioVoiceId;
    std::int32_t xaudioStreamKind;
};

static_assert(sizeof(AicaVoiceSlot) == 0x38);
static_assert(offsetof(AicaVoiceSlot, spawnCommand) == 0x18);
static_assert(offsetof(AicaVoiceSlot, xaudioVoiceId) == 0x30);

struct AicaVoicePool {
    std::uint8_t freeCount;
    std::uint8_t freeHead;
    std::uint8_t freeTail;
    std::byte reserved_03;
    std::uint8_t list1Count;
    std::uint8_t list1Head;
    std::uint8_t list1Tail;
    std::byte reserved_07;
    std::uint8_t list2Count;
    std::uint8_t list2Head;
    std::uint8_t list2Tail;
    std::byte reserved_0b[5];
    AicaVoiceSlot slots[160];
    std::byte reserved_2310[4];
    std::uint8_t paused;
    std::byte reserved_2315[3];
};

static_assert(sizeof(AicaVoicePool) == 0x2318);
static_assert(offsetof(AicaVoicePool, slots) == 0x10);
static_assert(offsetof(AicaVoicePool, paused) == 0x2314);

struct AicaMemory {
    AicaBankSlot bankSlots[16];
    std::uint8_t *alignedAudioRam;
    std::uint8_t *bankRegionBase[16];
    std::uint32_t bankRegionSize[16];
    std::uint8_t bankRegionLoaded[16];
    std::uint32_t audioRamSize;
    std::byte reserved_02dc[20];
    std::uint32_t pendingCommandRing[64];
    std::byte reserved_03f0[0x100];
    std::uint32_t executionCommandRing[64];
    std::byte reserved_05f0[0x100];
    std::uint8_t executionQueueCount;
    std::uint8_t pendingQueueCount;
    std::uint8_t executionReadIndex;
    std::uint8_t pendingWriteIndex;
    std::uint32_t currentCommand;
    std::byte reserved_06f8[9];
    std::uint8_t executionWriteIndex;
    std::uint8_t pendingReadIndex;
    std::uint8_t ownerId;
    std::byte reserved_0704;
    std::uint8_t updateComplete;
    std::uint8_t maximumPromotionsPerTick;
    std::byte reserved_0707;
    std::uint8_t loadedBankUnits;
    std::int8_t globalVolume;
    std::uint8_t useGlobalPanOverride;
    std::uint8_t commandQueueBusy;
    std::uint8_t field70c;
    std::byte field70d[2];
    std::int8_t field70f;
    std::int16_t globalPanRight;
    std::int16_t globalPanLeft;
    std::int16_t field714;
    std::int16_t field716;
    std::uint32_t field718;
    std::uint32_t field71c;
    std::byte reserved_0720[0x10];
    std::uint8_t field730;
    std::uint8_t field731;
    std::int8_t field732;
    std::byte reserved_0733[0x190d];
    AicaVoicePool voicePool;
};

static_assert(sizeof(AicaMemory) == 0x4358);
static_assert(offsetof(AicaMemory, pendingCommandRing) == 0x2f0);
static_assert(offsetof(AicaMemory, executionCommandRing) == 0x4f0);
static_assert(offsetof(AicaMemory, executionQueueCount) == 0x6f0);
static_assert(offsetof(AicaMemory, pendingQueueCount) == 0x6f1);
static_assert(offsetof(AicaMemory, executionReadIndex) == 0x6f2);
static_assert(offsetof(AicaMemory, pendingWriteIndex) == 0x6f3);
static_assert(offsetof(AicaMemory, currentCommand) == 0x6f4);
static_assert(offsetof(AicaMemory, executionWriteIndex) == 0x701);
static_assert(offsetof(AicaMemory, pendingReadIndex) == 0x702);
static_assert(offsetof(AicaMemory, updateComplete) == 0x705);
static_assert(offsetof(AicaMemory, maximumPromotionsPerTick) == 0x706);
static_assert(offsetof(AicaMemory, commandQueueBusy) == 0x70b);
static_assert(offsetof(AicaMemory, field70c) == 0x70c);
static_assert(offsetof(AicaMemory, globalPanRight) == 0x710);
static_assert(offsetof(AicaMemory, globalPanLeft) == 0x712);
static_assert(offsetof(AicaMemory, field731) == 0x731);
static_assert(offsetof(AicaMemory, field732) == 0x732);
static_assert(offsetof(AicaMemory, voicePool) == 0x2040);
static_assert(offsetof(AicaMemory, voicePool) + offsetof(AicaVoicePool, paused) == 0x4354);
constexpr std::array<std::uint16_t, 128> kAicaFadeTable{0xa3d7, 0x6d3a, 0x51eb, 0x4189, 0x369d, 0x2ecf, 0x28f5, 0x2468, 0x20c4, 0x1dca, 0x1b4e, 0x1934, 0x1767,
    0x15d8, 0x147a, 0x1346, 0x1234, 0x113f, 0x1062, 0x0f9a, 0x0ee5, 0x0e3f, 0x0da7, 0x0d1b, 0x0c9a, 0x0c22, 0x0bb3, 0x0b4c, 0x0aec, 0x0a92, 0x0a3d, 0x09ee,
    0x09a3, 0x095c, 0x091a, 0x08d8, 0x089f, 0x0866, 0x0831, 0x07ee, 0x07cd, 0x079e, 0x0772, 0x0748, 0x071f, 0x06f8, 0x06d3, 0x06af, 0x068d, 0x066c, 0x064d,
    0x062e, 0x0611, 0x05f5, 0x05d9, 0x05bf, 0x05a6, 0x058d, 0x0576, 0x055f, 0x0549, 0x0533, 0x051e, 0x050a, 0x04f7, 0x04e4, 0x04d1, 0x04bf, 0x04ae, 0x049d,
    0x048d, 0x047d, 0x046d, 0x045e, 0x044f, 0x0441, 0x0433, 0x0425, 0x0418, 0x040b, 0x03ff, 0x03f2, 0x03e6, 0x03da, 0x03cf, 0x03c4, 0x03b9, 0x03ae, 0x03a4,
    0x0399, 0x038f, 0x0386, 0x037c, 0x0373, 0x0369, 0x0360, 0x0357, 0x034f, 0x0346, 0x033e, 0x0336, 0x032e, 0x0326, 0x031e, 0x0317, 0x030f, 0x0308, 0x0301,
    0x02fa, 0x02f3, 0x02ec, 0x02e6, 0x02df, 0x02d9, 0x02d3, 0x02cc, 0x02c6, 0x02c0, 0x02bb, 0x02b5, 0x02af, 0x02aa, 0x02a4, 0x029f, 0x0299, 0x0294, 0x028f,
    0x028a};
constexpr std::array<std::int16_t, 16> kAicaPanAttenuation{
    -10000, -4200, -3900, -3600, -3300, -3000, -2700, -2400, -2100, -1800, -1500, -1200, -900, -600, -300, 0};

constexpr std::array<std::uint8_t, 4> kAicaEndStream{0xff, 0, 0, 0};
constexpr float kAicaInfiniteEnvelopeMilliseconds = (std::numeric_limits<float>::max)();
constexpr std::array<float, 64> kAicaReleaseMilliseconds{kAicaInfiniteEnvelopeMilliseconds, kAicaInfiniteEnvelopeMilliseconds, 118200.0f, 101300.0f, 88600.0f,
    70900.0f, 59100.0f, 50700.0f, 44300.0f, 35500.0f, 29600.0f, 25300.0f, 22200.0f, 17700.0f, 14800.0f, 12700.0f, 11100.0f, 8900.0f, 7400.0f, 6300.0f, 5500.0f,
    4400.0f, 3700.0f, 3200.0f, 2800.0f, 2200.0f, 1800.0f, 1600.0f, 1400.0f, 1100.0f, 920.0f, 90.0f, 690.0f, 550.0f, 460.0f, 390.0f, 340.0f, 270.0f, 230.0f,
    200.0f, 170.0f, 140.0f, 110.0f, 98.0f, 85.0f, 68.0f, 57.0f, 49.0f, 43.0f, 34.0f, 28.0f, 25.0f, 22.0f, 18.0f, 14.0f, 12.0f, 11.0f, 8.5f, 7.1f, 6.1f, 5.4f,
    4.3f, 3.6f, 3.1f};
constexpr std::array<float, 64> kAicaAttackMilliseconds{kAicaInfiniteEnvelopeMilliseconds, kAicaInfiniteEnvelopeMilliseconds, 8100.0f, 6900.0f, 6000.0f,
    4800.0f, 4000.0f, 3400.0f, 3000.0f, 2400.0f, 2000.0f, 1700.0f, 1500.0f, 1200.0f, 1000.0f, 860.0f, 760.0f, 600.0f, 500.0f, 430.0f, 380.0f, 300.0f, 250.0f,
    220.0f, 190.0f, 150.0f, 130.0f, 110.0f, 95.0f, 76.0f, 63.0f, 55.0f, 47.0f, 38.0f, 31.0f, 27.0f, 24.0f, 19.0f, 15.0f, 13.0f, 12.0f, 9.4f, 7.9f, 6.8f, 6.0f,
    4.7f, 3.8f, 3.4f, 3.0f, 2.4f, 2.0f, 1.8f, 1.6f, 1.3f, 1.1f, 0.93f, 0.85f, 0.65f, 0.53f, 0.44f, 0.4f, 0.35f, 0.1f, 0.1f};
constexpr std::array<float, 64> kAicaFilterMilliseconds{kAicaInfiniteEnvelopeMilliseconds, kAicaInfiniteEnvelopeMilliseconds, 472800.0f, 405200.0f, 354400.0f,
    283600.0f, 236400.0f, 202800.0f, 177200.0f, 142000.0f, 118400.0f, 101200.0f, 88800.0f, 70800.0f, 59200.0f, 50800.0f, 44400.0f, 35600.0f, 29600.0f, 25200.0f,
    22000.0f, 17600.0f, 14800.0f, 12800.0f, 11200.0f, 8800.0f, 7200.0f, 6400.0f, 5600.0f, 4400.0f, 3680.0f, 3160.0f, 2760.0f, 2200.0f, 1840.0f, 1560.0f,
    1360.0f, 1080.0f, 920.0f, 800.0f, 680.0f, 560.0f, 440.0f, 392.0f, 340.0f, 272.0f, 228.0f, 196.0f, 172.0f, 154.0f, 136.0f, 100.0f, 88.0f, 72.0f, 56.0f,
    48.0f, 44.0f, 34.0f, 28.0f, 24.0f, 22.0f, 17.0f, 14.0f, 12.0f};
constexpr std::array<std::uint8_t, 8600> kAicaResidentPcm{
#include "s1/aica_resident_samples.inc"
};
alignas(16) inline constexpr std::array<std::uint8_t, 32768> kAicaResidentNoisePcm{
#include "s1/aica_resident_noise_samples.inc"
};
constexpr std::array<float, 32> kAicaFilterQ{
    1.5f, 1.4f, 1.31f, 1.12f, 1.0f, 0.9f, 0.95f, 0.8f,
    0.75f, 0.7f, 0.65f, 0.6f, 0.55f, 0.5f, 0.45f, 0.4f,
    0.35f, 0.3f, 0.25f, 0.2f, 0.18f, 0.16f, 0.15f, 0.145f,
    0.142f, 0.139f, 0.133f, 0.129f, 0.125f, 0.12f, 0.115f, 0.1f};

void *PrepareRawSoundEffect(const std::byte *samples, std::uint32_t audioBytes, std::uint32_t loopBegin, std::uint32_t loopLength, std::uint16_t channels,
    float volume, float frequencyRatio, std::uint32_t sampleRate, bool reclaimable, std::uint16_t bitsPerSample = 16);
void StartRawSoundEffectPlayback(void *playback);
void StopRawSoundEffectPlayback(void *playback, bool releaseEnvelope = false);
void ReleaseRawSoundEffectReservation(void *playback);
bool RawSoundEffectVoiceIsActive(void *playback);
std::uint32_t RawSoundEffectVoiceId(void *playback);
void *GetRawSoundEffectVoice(std::uint32_t index);
AicaState *GetRawSoundEffectEnvelopeState(std::uint32_t index);
void SetRawSoundEffectVolume(void *playback, float volume);
void SetRawSoundEffectFrequencyRatio(void *playback, float frequencyRatio);
void SetRawSoundEffectLowPassFrequency(void *playback, float frequency, float oneOverQ = 1.0f);
void SetRawSoundEffectOutputMatrix(void *playback, const float *matrix, std::uint32_t sourceChannels, std::uint32_t outputChannels);
void SetRawSoundEffectPaused(void *playback, bool paused);
void SetDialogueStreamsPaused(bool paused);
bool AssignAicaVoiceRelease(std::uint32_t index);
void ResetPlatformVoices() noexcept;
void RefreshOutputVolumes() noexcept;
} // namespace shenmue::s2::aica
