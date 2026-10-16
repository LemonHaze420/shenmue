#include "s2/aica_middleware.hpp"
#include "s2/audio_data.hpp"
#include "s2/d3t_renderer.hpp"
#include "common/utility.hpp"
#include "common/math.hpp"
#include <bit>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <thread>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#include <immintrin.h>
#endif

namespace shenmue::s2::aica {
namespace {
AicaMemory memory{};
AicaMemory *g_aicaMemory = &memory;
AicaEnvironmentState g_aicaEnvironmentStates[16]{};
std::array<void *, 160> g_aicaPlaybackOwners{};
std::array<std::uint8_t, 160> g_aicaPlaybackPreBankLevels{};
std::array<std::uint8_t, 160> g_aicaPlaybackUsesBankVolume{};
std::array<std::uint8_t, 160> allocationLists{};
std::array<std::uint8_t, 160> soundTypes{};
float SoundTypeVolume(std::uint8_t type) {
    const S2AudioSettings &settings = GetS2AudioSettings();
    return type == 0 ? settings.musicVolume : type == 2 ? settings.dialogueVolume : settings.sfxVolume;
}
std::array<std::vector<std::uint8_t>, 16> banks;
std::array<bool, 16> modernBanks{};
struct DecodedBankSample {
    std::uint32_t descriptorOffset{};
    std::vector<std::uint8_t> pcm;
};
std::array<std::vector<DecodedBankSample>, 16> decodedSamples;

// Yamaha ADPCM conversion adapted from Shenmue-Mods/DTPKUtil (MIT):
// https://github.com/Shenmue-Mods/DTPKUtil
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
void DecodeYamahaAdpcm(const std::uint8_t *input, std::size_t bytes, std::vector<std::uint8_t> &output) {
    constexpr std::array<int, 16> difference{1, 3, 5, 7, 9, 11, 13, 15, -1, -3, -5, -7, -9, -11, -13, -15};
    constexpr std::array<int, 8> scale{0xe6, 0xe6, 0xe6, 0xe6, 0x133, 0x199, 0x200, 0x266};
    output.resize(bytes * 4);
    int sample = 0;
    int quantizer = 0x7f;
    for (std::size_t index = 0; index < bytes * 2; ++index) {
        const unsigned nibble = (input[index / 2] >> ((index & 1U) * 4U)) & 15U;
        const int delta = quantizer * difference[nibble];
        sample = std::clamp(sample + ((delta + static_cast<int>(static_cast<std::uint32_t>(delta) >> 29)) >> 3), -32768, 32767);
        output[2 * index] = static_cast<std::uint8_t>(sample);
        output[2 * index + 1] = static_cast<std::uint8_t>(sample >> 8);
        quantizer = std::clamp((quantizer * scale[nibble & 7U]) >> 8, 0x7f, 0x6000);
        sample = sample * 254 / 256;
    }
}
std::array<std::uint32_t, 16> allocations{0x180000, 0x2000, 0x2000, 0x2000, 0x2000, 0x4d8000, 0x2000, 0x2000};
std::jthread middlewareThread;
bool initialized{};
constexpr std::array<std::uint8_t, 128> kAicaVolumeTable{0, 11, 15, 19, 22, 25, 27, 29, 31, 33, 35, 37, 39, 40, 42, 43, 45, 46, 47, 49, 50, 51, 52, 54, 55, 56,
    57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 73, 74, 75, 76, 77, 78, 78, 79, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 88, 88, 89, 90,
    90, 91, 92, 92, 93, 94, 94, 95, 96, 96, 97, 98, 98, 99, 100, 100, 101, 102, 102, 103, 103, 104, 105, 105, 106, 106, 107, 108, 108, 109, 109, 110, 110, 111,
    112, 112, 113, 113, 114, 114, 115, 116, 116, 117, 117, 118, 118, 119, 119, 120, 120, 121, 121, 122, 122, 123, 123, 124, 124, 125, 125, 126, 127};
std::int32_t SNDFunc(std::uint32_t command);
std::uint8_t AllocateAicaVoice(std::uint8_t list);
std::int32_t ReturnAicaVoice(std::uint8_t list, std::uint8_t slotIndex);
std::int32_t AicaLevelToHundredthDecibels(std::uint32_t level);
float AicaLevelToGain(std::uint32_t level);
void SetVolumeForAicaForXAudio(std::uint32_t physicalId, float volume, float masterVolume);
void UpdateAicaPlaybackVolume(std::size_t index);
bool AicaBankPointerValid(const std::uint8_t *bankData, std::size_t bankSize, const std::uint8_t *pointer, std::size_t bytes);
std::uint8_t *AicaBankOffset(std::uint8_t *bankData, std::size_t bankSize, std::size_t offset, std::size_t bytes);
std::uint8_t AicaBankDriverLevel(std::uint8_t bank, std::uint8_t voiceIndex);
void ResetAicaDriverAssignments(std::uint8_t bank, bool normalDrivers);
void ConfigureAicaProgram(std::uint8_t bank, std::uint8_t *program);
void InitializeLoadedAicaBank(std::uint8_t bank);
void ResetAicaDriverDynamicFields(std::uint8_t voiceIndex);
void SelectAicaProgram(std::uint8_t bank, std::uint8_t programIndex);
void ExecuteAicaCommand_3(std::uint16_t command, std::uint8_t bank);
std::int64_t ExecuteAicaCommand_4(std::uint16_t command, std::int64_t, std::uint8_t bank);
void ExecuteAicaCommand_5(std::uint16_t command, std::int64_t unused);
void ExecuteAicaCommand_6();
std::int64_t ExecuteAicaCommand_8(std::uint8_t enabled);
float AicaEnvironmentEnvelopeDecibels(float level);
void ApplyAicaEnvelopeGain(AicaState &envelope);
void UpdateAicaLowPassFilter(std::uint32_t physicalId, AicaState &envelope, float elapsedSeconds);
std::uint8_t DecodeAicaEnvelopeRate(std::int32_t note, std::int32_t keyRateScaling, std::uint8_t authoredRate);
float AicaAttackRate(std::uint8_t decoded);
float AicaDecayRate(std::uint8_t decoded);
void ConfigureAicaEnvironmentEnvelope(AicaState &state, const std::uint8_t *entry, std::uint32_t owner);
void ConfigureAicaToneEnvelope(AicaState &state, const std::uint8_t *entry, std::int32_t note, std::uint32_t owner);
bool ConfigureAicaToneFilter(AicaState &state, const std::uint8_t *entry, std::int32_t note);
void SetAttackFilterParams(AicaState &state);
void SetAttackFilterParamsForEnvironment(std::uint32_t physicalId, const std::uint8_t *entry, float volume, std::uint32_t owner);
void SetAttackFilterParamsForTone(std::uint32_t physicalId, const std::uint8_t *entry, std::int32_t note, float volume, bool music, std::uint32_t owner);
void EnvelopeGenerator_StopAndReset_1(std::uint32_t physicalId);
bool ReleaseEnvelope(std::uint32_t physicalId);
void ApplyFilter(AicaState &envelope, float elapsedSeconds);
void UpdateModernAicaEnvelope(AicaState &envelope);
void UpdateAicaEnvironmentEnvelopes(float elapsedSeconds);

void StopReleasedAicaEnvelopesForOwner(std::int32_t owner);
void ReleaseAicaVoice(std::uint32_t index, bool releaseEnvelope);
void StopAicaEnvironmentPart(std::uint8_t stateIndex, std::uint8_t partIndex);
void ReleaseAicaEnvironmentPart(std::uint8_t stateIndex, std::uint8_t partIndex);
std::int32_t AicaEnvironmentVolume(const AicaEnvironmentState &state, const std::uint8_t *entry, std::uint8_t value);
std::int32_t AicaEnvironmentPitch(const AicaEnvironmentState &state, const std::uint8_t *entry, std::uint8_t value, std::int32_t *result);
float MIDIToPitchHz(const AicaEnvironmentState &state, const std::uint8_t *entry, std::uint8_t value);
void StartOrUpdateAicaEnvironmentPart(std::uint8_t stateIndex, std::uint8_t partIndex, const std::uint8_t *entry, std::uint8_t value);
void UpdateAicaEnvironment(std::uint8_t stateIndex, std::uint8_t value);
void SetupAicaEnvironment(std::uint8_t stateIndex, std::uint8_t programIndex);
void ExecuteAicaEnvironmentCommand();
bool RecomputeAicaPreBankLevel(std::size_t index, std::uint8_t &level);
void UpdateAicaPlaybackPitch(std::size_t index);
void ApplyVoicePanMatrixToXAudio(std::size_t index);
void EnqueueAicaExecution(std::uint32_t command);
void Aica_InitializeDriverValues();
void ResetAicaVoice(AicaVoiceSlot &voice);
void ReapInactiveAicaVoices();
std::int32_t ReleaseOwnedAicaBankUnlocked(std::size_t bank);
void PromoteQueuedAicaCommands();
void ExecuteAicaInsnForCycles();
std::int64_t ExecuteAicaCommand(std::int32_t bank, std::uint32_t command, std::int16_t volume);
std::int64_t ExecuteEnvironmentAicaCommand(std::int32_t bank, std::uint32_t type, std::int32_t volume);
std::int64_t ExecuteAicaCommand2Ch(std::int32_t bank, std::int32_t channel, std::uint32_t command, std::int16_t volume);
void ExecuteAicaInsn(std::uint32_t command);
void ExecuteAicaBankB();
void ResolveAicaDriverPointer(std::uint8_t bank, std::uint8_t voiceIndex);
void ExecuteAicaBankC();
std::int64_t ExecuteAicaCommand_2();
std::int64_t ExecuteAicaCommand_0(std::uint16_t command);
void ExecuteAicaSequenceStopCommand(std::uint32_t command);
void BeginAicaSequenceFadeOut(std::uint16_t command);
void AicaUpdate(std::uint32_t command);
void ProcessAicaEventStreams();
void ExecuteAicaCommand_1(std::uint16_t command, std::int32_t mask);
void StopAicaVoicesForCommand(std::uint32_t command);
void EmitAudioForAica();
void ExecuteAicaBankA();
std::int64_t LoadAICADriver(std::uint16_t *driver, std::uint8_t voiceIndex);

constexpr std::uint32_t ByteSwap32(std::uint32_t value) noexcept {
    return ((value & 0x000000ffu) << 24) | ((value & 0x0000ff00u) << 8) | ((value & 0x00ff0000u) >> 8) | ((value & 0xff000000u) >> 24);
}
std::uint8_t AllocateAicaVoice(std::uint8_t list) {
    AicaVoicePool &pool = g_aicaMemory->voicePool;
    if (!pool.freeCount)
        return 0xff;
    const std::uint8_t slotIndex = pool.freeHead;
    AicaVoiceSlot &slot = pool.slots[slotIndex];
    const std::uint8_t next = slot.nextSlot;
    if (next == 0xff) {
        pool.freeCount = 0;
        pool.freeHead = 0xff;
        pool.freeTail = 0xff;
    } else {
        pool.slots[next].previousSlot = 0xff;
        pool.freeHead = next;
        --pool.freeCount;
    }
    std::uint8_t &count = list == 1 ? pool.list1Count : pool.list2Count;
    std::uint8_t &head = list == 1 ? pool.list1Head : pool.list2Head;
    std::uint8_t &tail = list == 1 ? pool.list1Tail : pool.list2Tail;
    if (count) {
        pool.slots[tail].nextSlot = slotIndex;
        slot.previousSlot = tail;
    } else {
        head = slotIndex;
        slot.previousSlot = 0xff;
    }
    slot.nextSlot = 0xff;
    tail = slotIndex;
    ++count;
    allocationLists[slotIndex] = list;
    return slotIndex;
}

std::int32_t ReturnAicaVoice(std::uint8_t list, std::uint8_t slotIndex) {
    AicaVoicePool &pool = g_aicaMemory->voicePool;
    AicaVoiceSlot &slot = pool.slots[slotIndex];
    const std::uint8_t previous = slot.previousSlot;
    const std::uint8_t next = slot.nextSlot;
    std::uint8_t &count = list == 1 ? pool.list1Count : pool.list2Count;
    std::uint8_t &head = list == 1 ? pool.list1Head : pool.list2Head;
    std::uint8_t &tail = list == 1 ? pool.list1Tail : pool.list2Tail;
    if (count == 1) {
        head = 0xff;
        tail = 0xff;
        --count;
    } else {
        if (previous == 0xff)
            head = next;
        else
            pool.slots[previous].nextSlot = next;
        if (next == 0xff)
            tail = previous;
        else
            pool.slots[next].previousSlot = previous;
        --count;
    }
    if (pool.freeCount) {
        pool.slots[pool.freeTail].nextSlot = slotIndex;
        slot.previousSlot = pool.freeTail;
    } else {
        pool.freeHead = slotIndex;
        slot.previousSlot = 0xff;
    }
    slot.nextSlot = 0xff;
    pool.freeTail = slotIndex;
    ++pool.freeCount;
    return -1;
}

std::int32_t AicaLevelToHundredthDecibels(std::uint32_t level) {
    std::uint32_t attenuation = level ^ 0xffU;
    if (attenuation != 0xffU)
        attenuation >>= 1U;
    std::int32_t hundredthDb = 0;
    if (attenuation & 0x80U)
        hundredthDb -= 4800;
    if (attenuation & 0x40U)
        hundredthDb -= 2400;
    if (attenuation & 0x20U)
        hundredthDb -= 1200;
    if (attenuation & 0x10U)
        hundredthDb -= 600;
    if (attenuation & 0x08U)
        hundredthDb -= 300;
    if (attenuation & 0x04U)
        hundredthDb -= 150;
    if (attenuation & 0x02U)
        hundredthDb -= 80;
    if (attenuation & 0x01U)
        hundredthDb -= 40;
    return hundredthDb;
}

float AicaLevelToGain(std::uint32_t level) {
    const std::int32_t hundredthDb = AicaLevelToHundredthDecibels(level);
    return hundredthDb > -6400 ? std::pow(10.0f, static_cast<float>(hundredthDb) / 2000.0f) : 0.0f;
}

bool RecomputeAicaPreBankLevel(std::size_t index, std::uint8_t &level);
void ApplyVoicePanMatrixToXAudio(std::size_t index);
void UpdateAicaPlaybackPitch(std::size_t index);
void ApplyAicaEnvelopeGain(AicaState &envelope);

void SetVolumeForAicaForXAudio(std::uint32_t physicalId, float volume, float masterVolume) {
    if (!IsDialogueAudioActive())
        return;
    AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
    if (!state) {
        SetRawSoundEffectVolume(GetRawSoundEffectVoice(physicalId), volume);
        return;
    }
    volume *= masterVolume;
    state->linearVolume = volume;
    state->baseVolumeDb = volume == 0.0f ? -64.0f : (std::max)(-64.0f, std::log10(volume) * 20.0f);
    if (state->phase)
        ApplyAicaEnvelopeGain(*state);
    else
        SetRawSoundEffectVolume(GetRawSoundEffectVoice(physicalId), volume);
}

void UpdateAicaPlaybackVolume(std::size_t index) {
    if (!g_aicaMemory || index >= g_aicaPlaybackOwners.size() || !g_aicaPlaybackOwners[index]) {
        return;
    }
    const AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[index];
    std::uint8_t preBankLevel = g_aicaPlaybackPreBankLevels[index];
    if (RecomputeAicaPreBankLevel(index, preBankLevel))
        g_aicaPlaybackPreBankLevels[index] = preBankLevel;
    std::uint32_t level = preBankLevel;
    if (g_aicaPlaybackUsesBankVolume[index] && voice.bank < 16) {
        const std::uint8_t *const memory = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
        level = (2U * memory[0x751 + 24U * voice.bank] * level) >> 8U;
        if (level)
            ++level;
    }
    level = (2U * static_cast<std::uint8_t>(g_aicaMemory->globalVolume) * level) >> 8U;
    if (level)
        ++level;
    const float linear = AicaLevelToGain(level);
    SetVolumeForAicaForXAudio(voice.xaudioVoiceId, linear, SoundTypeVolume(soundTypes[index]));
}

bool AicaBankPointerValid(const std::uint8_t *bankData, std::size_t bankSize, const std::uint8_t *pointer, std::size_t bytes) {
    if (!bankData || !pointer)
        return false;
    const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(bankData);
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pointer);
    if (address < base)
        return false;
    const std::size_t offset = static_cast<std::size_t>(address - base);
    return offset <= bankSize && bytes <= bankSize - offset;
}

std::uint8_t *AicaBankOffset(std::uint8_t *bankData, std::size_t bankSize, std::size_t offset, std::size_t bytes) {
    if (!bankData || offset > bankSize || bytes > bankSize - offset)
        return nullptr;
    return bankData + offset;
}
struct BankSample {
    const std::byte *data{};
    std::uint32_t bytes{};
    std::uint32_t loopBegin{};
    std::uint32_t loopLength{};
    std::uint32_t rate{44100};
    std::uint16_t channels{1};
    std::uint16_t bits{16};
};
bool ResolveBankSample(std::uint8_t bank, std::uint16_t sampleId, bool loopsAllowed, BankSample &sample, std::uint32_t replacementBytes = 0) {
    if (bank >= 16 || sampleId < 256 || !memory.bankRegionLoaded[bank])
        return false;
    const auto &data = banks[bank];
    const bool modern = modernBanks[bank];
    const std::size_t offsetField = modern ? 0xb8 : 0x3c;
    if (data.size() < offsetField + 4)
        return false;
    const std::size_t stride = modern ? 20 : 16;
    const std::size_t offset =
        static_cast<std::size_t>(read_unaligned<std::uint32_t>(data.data() + offsetField)) + 4 + stride * static_cast<std::size_t>(sampleId - 256);
    if (offset > data.size() || stride > data.size() - offset)
        return false;
    const auto *descriptor = data.data() + offset;
    const std::uint32_t packed = read_unaligned<std::uint32_t>(descriptor);
    const std::size_t sampleOffset = modern ? packed : packed & 0x7fffffU;
    sample.bytes = replacementBytes ? replacementBytes : read_unaligned<std::uint32_t>(descriptor + (modern ? 16 : 12));
    if (!sample.bytes || sampleOffset > data.size() || sample.bytes > data.size() - sampleOffset)
        return false;
    sample.data = reinterpret_cast<const std::byte *>(data.data() + sampleOffset);
    const std::uint32_t begin = modern ? read_unaligned<std::uint32_t>(descriptor + 4) : read_unaligned<std::uint16_t>(descriptor + 4);
    const std::uint32_t end = modern ? read_unaligned<std::uint32_t>(descriptor + 8) : read_unaligned<std::uint16_t>(descriptor + 6);
    const bool loops = loopsAllowed && (modern ? (descriptor[12] & 4U) != 0 : (packed & 0x02000000U) != 0);
    if (loops && end > begin) {
        sample.loopBegin = begin;
        sample.loopLength = end - begin;
    }
    if (modern) {
        sample.bits = (descriptor[12] & 3U) == 1 ? 8 : 16;
        sample.channels = descriptor[13] == 1 ? 2 : 1;
        sample.rate = 48000;
    } else {
        if ((packed & 0x01000000U) != 0 && sample.bytes / 2 <= end) {
            auto &decoded = decodedSamples[bank];
            auto found = std::find_if(decoded.begin(), decoded.end(), [offset](const DecodedBankSample &entry) { return entry.descriptorOffset == offset; });
            if (found == decoded.end()) {
                DecodedBankSample entry{};
                entry.descriptorOffset = static_cast<std::uint32_t>(offset);
                DecodeYamahaAdpcm(data.data() + sampleOffset, sample.bytes, entry.pcm);
                decoded.push_back(std::move(entry));
                found = decoded.end() - 1;
            }
            sample.data = reinterpret_cast<const std::byte *>(found->pcm.data());
            sample.bytes = static_cast<std::uint32_t>(found->pcm.size());
        }
        if ((packed & 0x00800000U) != 0 || descriptor[8] == 0x80)
            sample.rate = 22050;
    }
    return true;
}

std::uint8_t AicaBankDriverLevel(std::uint8_t bank, std::uint8_t voiceIndex) {
    if (!g_aicaMemory || bank >= 16 || voiceIndex >= 64)
        return 0;
    const auto *const raw = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    const std::size_t bankBase = 24U * bank;
    const std::size_t voiceBase = 64U * voiceIndex;
    const bool alternate = (raw[0x8c1 + voiceBase] & 2U) != 0;
    const std::uint8_t base = raw[0x752 + bankBase + (alternate ? 1U : 0U)];
    const std::uint8_t offset = raw[0x754 + bankBase + (alternate ? 1U : 0U)];
    const std::int32_t level = static_cast<std::int32_t>(base) + (offset == 64 ? 0 : 2 * (static_cast<std::int32_t>(offset) - 64));
    return static_cast<std::uint8_t>(std::clamp(level, 0, 127));
}

void ResetAicaDriverAssignments(std::uint8_t bank, bool normalDrivers) {
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    for (std::size_t channel = 0; channel < 16; ++channel) {
        std::uint8_t *const assignments = raw + 0x18c0 + 64U * bank + 4U * channel;
        for (std::size_t candidate = 0; candidate < 4; ++candidate) {
            const std::uint8_t voiceIndex = assignments[candidate];
            if (voiceIndex == 0xff)
                continue;
            const std::size_t voiceBase = 64U * voiceIndex;
            if (((raw[0x8c1 + voiceBase] & 1U) != 0) == normalDrivers)
                continue;
            raw[0x8c0 + voiceBase] = 0;
            raw[0x8c1 + voiceBase] = 0;
            write_unaligned<std::uint8_t *>(raw + 0x8e8 + voiceBase, nullptr);
            write_unaligned<std::uint8_t *>(raw + 0x8f0 + voiceBase, nullptr);
            raw[0x8c2 + voiceBase] = 0;
            raw[0x8e2 + voiceBase] = 64;
            raw[0x8e3 + voiceBase] = 0;
            raw[0x8e4 + voiceBase] = 0;
            raw[0x8e5 + voiceBase] = 0;
            assignments[candidate] = 0xff;
        }
    }
}

void ConfigureAicaProgram(std::uint8_t bank, std::uint8_t *program) {
    if (!g_aicaMemory || bank >= 16 || !program)
        return;
    const AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!AicaBankPointerValid(bankData, bankSize, program, 8))
        return;
    const std::size_t recordCount = static_cast<std::size_t>(program[0]) + 1U;
    const bool modern = modernBanks[bank];
    const std::size_t recordSize = modern ? 24U : 32U;
    if (!AicaBankPointerValid(bankData, bankSize, program + 8, recordSize * recordCount)) {
        return;
    }

    auto *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    std::uint8_t voiceIndex = 0xff;
    for (std::size_t recordIndex = 0; recordIndex < recordCount; ++recordIndex) {
        std::uint8_t *const record = program + 8 + recordSize * recordIndex;
        if (static_cast<std::int8_t>(record[0]) < 0) {
            voiceIndex = 0xff;
            for (std::uint8_t candidate = 0; candidate < 64; ++candidate) {
                if (!raw[0x8c0 + 64U * candidate]) {
                    voiceIndex = candidate;
                    break;
                }
            }
            if (voiceIndex == 0xff)
                continue;

            const std::size_t voiceBase = 64U * voiceIndex;
            raw[0x8c0 + voiceBase] = 1;
            raw[0x8c1 + voiceBase] = record[0];
            raw[0x8c3 + voiceBase] = record[1];
            raw[0x8c4 + voiceBase] = record[2];
            raw[0x8c5 + voiceBase] = record[3];
            if (modern) {
                raw[0x8ca + voiceBase] = 0;
                raw[0x8cb + voiceBase] = record[7];
                raw[0x8cc + voiceBase] = record[6];
                raw[0x8cd + voiceBase] = record[8];
                std::memcpy(raw + 0x8ce + voiceBase, record + 9, 14);
            } else {
                raw[0x8ca + voiceBase] = record[8];
                raw[0x8cb + voiceBase] = record[9];
                raw[0x8cc + voiceBase] = record[10];
                raw[0x8cd + voiceBase] = record[11];
                raw[0x8da + voiceBase] = record[12];
                raw[0x8db + voiceBase] = record[13];
            }
            raw[0x8f8 + voiceBase] = modern ? 0 : 1;
            raw[0x8f9 + voiceBase] = 0;
            raw[0x8dc + voiceBase] = AicaBankDriverLevel(bank, voiceIndex);
            raw[0x8dd + voiceBase] = raw[0x8da + voiceBase];
            write_unaligned(raw + 0x8e8 + voiceBase, record);
            ResolveAicaDriverPointer(bank, voiceIndex);
        } else if (voiceIndex == 0xff) {
            return;
        }

        const std::uint8_t channel = record[1];
        if (channel >= 16)
            continue;
        std::uint8_t *const assignments = raw + 0x18c0 + 64U * bank + 4U * channel;
        for (std::size_t candidate = 0; candidate < 4; ++candidate) {
            if (assignments[candidate] == 0xff) {
                assignments[candidate] = voiceIndex;
                break;
            }
        }
    }
}

void InitializeLoadedAicaBank(std::uint8_t bank) {
    if (!g_aicaMemory || bank >= 16)
        return;
    AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!AicaBankPointerValid(bankData, bankSize, bankData, 0x64))
        return;
    const std::uint32_t programBaseOffset = read_unaligned<std::uint32_t>(bankData + (modernBanks[bank] ? 0x98 : 0x20));
    std::uint8_t *const programBase = AicaBankOffset(bankData, bankSize, programBaseOffset, 1);
    auto *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::size_t bankBase = 24U * bank;
    raw[0x750 + bankBase] = 0xff;
    raw[0x751 + bankBase] = 127;
    const std::uint16_t defaultProgramOffset = read_unaligned<std::uint16_t>(bankData + (modernBanks[bank] ? 0xbe : 0x62));
    std::uint8_t *const defaultProgram =
        defaultProgramOffset && programBase ? AicaBankOffset(bankData, bankSize, programBaseOffset + defaultProgramOffset, 2) : nullptr;
    write_unaligned(raw + 0x748 + bankBase, defaultProgram);
    if (!defaultProgram)
        return;
    raw[0x753 + bankBase] = defaultProgram[1];
    ConfigureAicaProgram(bank, defaultProgram);
}

void ResetAicaDriverDynamicFields(std::uint8_t voiceIndex) {
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::size_t voiceBase = 64U * voiceIndex;
    raw[0x8e2 + voiceBase] = 64;
    raw[0x8e3 + voiceBase] = 0;
    raw[0x8e4 + voiceBase] = 0;
    raw[0x8e5 + voiceBase] = 0;
    const std::uint8_t *const note = read_unaligned<std::uint8_t *>(raw + 0x8e8 + voiceBase);
    if (note) {
        const bool modern = raw[0x8f8 + voiceBase] == 0;
        raw[0x8cd + voiceBase] = note[modern ? 8 : 11];
        raw[0x8da + voiceBase] = note[modern ? 21 : 12];
        raw[0x8dd + voiceBase] = raw[0x8da + voiceBase];
        raw[0x8db + voiceBase] = note[modern ? 22 : 13];
    }
}

void SelectAicaProgram(std::uint8_t bank, std::uint8_t programIndex) {
    if (!g_aicaMemory || bank >= 16)
        return;
    AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!AicaBankPointerValid(bankData, bankSize, bankData, 0x64))
        return;
    auto *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::size_t bankBase = 24U * bank;
    if (raw[0x750 + bankBase] == programIndex) {
        for (std::size_t channel = 0; channel < 16; ++channel) {
            const std::uint8_t *const assignments = raw + 0x18c0 + 64U * bank + 4U * channel;
            for (std::size_t candidate = 0; candidate < 4; ++candidate) {
                const std::uint8_t voiceIndex = assignments[candidate];
                if (voiceIndex >= 64 || (raw[0x8c1 + 64U * voiceIndex] & 1U) != 0)
                    continue;
                ResetAicaDriverDynamicFields(voiceIndex);
            }
        }
        return;
    }

    const std::uint16_t maximumProgram = read_unaligned<std::uint16_t>(bankData + (modernBanks[bank] ? 0xbc : 0x60));
    if (programIndex > maximumProgram)
        return;
    const std::size_t offsetEntry = (modernBanks[bank] ? 0xc0U : 0x64U) + 2U * programIndex;
    const std::uint8_t *const entry = AicaBankOffset(bankData, bankSize, offsetEntry, 2);
    if (!entry)
        return;
    const std::uint16_t programOffset = read_unaligned<std::uint16_t>(entry);
    const std::uint32_t programBaseOffset = read_unaligned<std::uint32_t>(bankData + (modernBanks[bank] ? 0x98 : 0x20));
    std::uint8_t *const program = programOffset ? AicaBankOffset(bankData, bankSize, programBaseOffset + programOffset, 2) : nullptr;
    if (!program)
        return;

    write_unaligned(raw + 0x740 + bankBase, program);
    raw[0x750 + bankBase] = programIndex;
    if (program[1] != 0xff)
        raw[0x752 + bankBase] = program[1];
    ResetAicaDriverAssignments(bank, true);
    ConfigureAicaProgram(bank, program);
}

void ExecuteAicaCommand_3(std::uint16_t command, std::uint8_t bank) {
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::uint8_t value = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 8) & 127U);
    for (std::size_t channel = 0; channel < 16; ++channel) {
        for (std::size_t candidate = 0; candidate < 4; ++candidate) {
            const std::uint8_t driver = raw[0x18c0 + 64U * bank + 4U * channel + candidate];
            if (driver == 0xff)
                continue;
            const std::size_t base = 64U * driver;
            if (command == 1 || command == 0x11) {
                if ((raw[0x8c1 + base] & 1U) == 0)
                    ResetAicaDriverDynamicFields(driver);
            } else if (command == 0xa004) {
                raw[0x8dc + base] = AicaBankDriverLevel(bank, driver);
            } else if (command == 0xa007 && (raw[0x8c1 + base] & 1U) == 0) {
                raw[0x8da + base] = static_cast<std::uint8_t>(value + raw[0x8dd + base] - 64U);
            }
        }
    }
}

std::int64_t ExecuteAicaCommand_4(std::uint16_t command, std::int64_t, std::uint8_t bank) {
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::uint8_t value = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 8) & 0x7fU);
    const std::size_t bankBase = 24U * bank;
    switch (command) {
    case 0xa004:
        raw[0x752 + bankBase] = value;
        break;
    case 0xa005:
        raw[0x754 + bankBase] = value;
        break;
    case 0xa010:
        raw[0x753 + bankBase] = value;
        break;
    case 0xa011:
        raw[0x755 + bankBase] = value;
        break;
    default:
        return 0xa011;
    }
    return static_cast<std::int64_t>(reinterpret_cast<std::intptr_t>(g_aicaMemory));
}

void ExecuteAicaCommand_5(std::uint16_t command, std::int64_t unused) {
    if (!g_aicaMemory)
        return;
    const bool allBanks = (g_aicaMemory->currentCommand & 0x10U) != 0;
    const std::uint8_t selectedBank = static_cast<std::uint8_t>(g_aicaMemory->currentCommand & 0x0fU);
    if (allBanks) {
        for (std::uint8_t bank = 0; bank < 16; ++bank) {
            if (g_aicaMemory->bankSlots[bank].bankIdOrType != -1)
                ExecuteAicaCommand_4(command, unused, bank);
        }
    } else {
        ExecuteAicaCommand_4(command, unused, selectedBank);
    }
    if (allBanks) {
        for (std::uint8_t bank = 0; bank < 16; ++bank) {
            if (g_aicaMemory->bankSlots[bank].bankIdOrType != -1)
                ExecuteAicaCommand_3(0xa004, bank);
        }
    } else {
        ExecuteAicaCommand_3(0xa004, selectedBank);
    }
    ExecuteAicaCommand_1(command, allBanks ? 0 : 0x0f);
}

void ExecuteAicaCommand_6() {
    if (!g_aicaMemory)
        return;
    const bool allBanks = (g_aicaMemory->currentCommand & 0x10U) != 0;
    if (allBanks) {
        for (std::uint8_t bank = 0; bank < 16; ++bank) {
            if (g_aicaMemory->bankSlots[bank].bankIdOrType != -1)
                ExecuteAicaCommand_3(0xa007, bank);
        }
    } else {
        ExecuteAicaCommand_3(0xa007, static_cast<std::uint8_t>(g_aicaMemory->currentCommand & 0x0fU));
    }
    ExecuteAicaCommand_1(0xa007, allBanks ? 0 : 0x0f);
}

std::int64_t ExecuteAicaCommand_8(std::uint8_t enabled) {
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::uint8_t bank = static_cast<std::uint8_t>(g_aicaMemory->currentCommand & 0x0fU);
    const std::uint8_t channel = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 8) & 0x7fU);
    const std::uint8_t *const assignments = raw + 0x18c0 + 64U * bank + 4U * channel;
    for (std::size_t candidate = 0; candidate < 4; ++candidate) {
        const std::uint8_t voiceIndex = assignments[candidate];
        if (voiceIndex != 0xff)
            raw[0x8c2 + 64U * voiceIndex] = enabled;
    }
    return 4;
}

float AicaEnvironmentEnvelopeDecibels(float level) {
    const std::uint8_t quantized = static_cast<std::uint8_t>(static_cast<std::int32_t>(level * 255.0f));
    const std::uint8_t attenuation = static_cast<std::uint8_t>(~quantized);
    float decibels = 0.0f;
    if (attenuation & 0x01U)
        decibels += -0.4f;
    if (attenuation & 0x02U)
        decibels += -0.8f;
    if (attenuation & 0x04U)
        decibels += -1.5f;
    if (attenuation & 0x08U)
        decibels += -3.0f;
    if (attenuation & 0x10U)
        decibels += -6.0f;
    if (attenuation & 0x20U)
        decibels += -12.0f;
    if (attenuation & 0x40U)
        decibels += -24.0f;
    if (attenuation & 0x80U)
        decibels += -48.0f;
    return decibels;
}

void ApplyAicaEnvelopeGain(AicaState &envelope) {
    const float renderedLevel = envelope.egh && envelope.phase == AICA_ENVELOPE_ATTACK ? 1.0f : envelope.envelopeLevel;
    const bool linearEnvelope = !envelope.legacyEnvelope;
    const float linearGain = renderedLevel * envelope.linearVolume;
    const float current = linearEnvelope ? linearGain : envelope.baseVolumeDb + AicaEnvironmentEnvelopeDecibels(renderedLevel);
    if (current == envelope.currentVolume)
        return;
    SetRawSoundEffectVolume(GetRawSoundEffectVoice(envelope.voiceId),
        linearEnvelope ? linearGain : current > -64.0f ? std::pow(10.0f, current / 20.0f) : 0.0f);
    envelope.currentVolume = current;
}

void UpdateAicaLowPassFilter(std::uint32_t physicalId, AicaState &envelope, float elapsedSeconds) {
    if (envelope.instantFilterAttack)
        return;
    const float filterElapsed = envelope.filterElapsed;
    if (envelope.phase == AICA_ENVELOPE_ATTACK || (envelope.phase >= AICA_ENVELOPE_DECAY1 && envelope.phase <= AICA_ENVELOPE_DECAY2)) {
        if (filterElapsed < envelope.filterAttackTime) {
            const float fraction = (std::clamp)(filterElapsed / envelope.filterAttackTime, 0.0f, 1.0f);
            envelope.currentFilterLevel = (envelope.filterLevel1 - envelope.filterLevel0) * fraction + envelope.filterLevel0;
        } else if (filterElapsed < envelope.filterDecay1Time) {
            const float fraction =
                (std::clamp)((filterElapsed - envelope.filterAttackTime) / (envelope.filterDecay1Time - envelope.filterAttackTime), 0.0f, 1.0f);
            envelope.currentFilterLevel = (envelope.filterLevel2 - envelope.filterLevel1) * fraction + envelope.filterLevel1;
        } else if (filterElapsed < envelope.filterDecay2Time) {
            const float fraction =
                (std::clamp)((filterElapsed - envelope.filterDecay1Time) / (envelope.filterDecay2Time - envelope.filterDecay1Time), 0.0f, 1.0f);
            envelope.currentFilterLevel = (envelope.filterLevel3 - envelope.filterLevel2) * fraction + envelope.filterLevel2;
        } else {
            envelope.currentFilterLevel = envelope.filterLevel3;
        }
    } else if (envelope.phase == AICA_ENVELOPE_RELEASE) {
        const float fraction = (std::clamp)(1.0f - filterElapsed / envelope.filterReleaseTime, 0.0f, 1.0f);
        envelope.currentFilterLevel = (envelope.filterReleaseLevel - envelope.currentFilterLevel) * fraction + envelope.currentFilterLevel;
    } else {
        return;
    }
    envelope.filterElapsed = filterElapsed + elapsedSeconds;
    SetRawSoundEffectLowPassFrequency(GetRawSoundEffectVoice(physicalId), std::pow(10.0f, envelope.currentFilterLevel * 4.0f) / 10000.0f, envelope.oneOverQ);
}

std::uint8_t DecodeAicaEnvelopeRate(std::int32_t note, std::int32_t keyRateScaling, std::uint8_t authoredRate) {
    std::int32_t noteRate = 0;
    if (keyRateScaling != 15) {
        noteRate = (keyRateScaling * (((note + 36) % 12 >= 7 ? 1 : 0) + 2 * ((note + 36) / 12) - 16)) >> 4;
    }
    std::int32_t decoded = noteRate + 2 * authoredRate;
    if ((authoredRate & 0x1fU) == 0)
        decoded = 0;
    return static_cast<std::uint8_t>(std::clamp(decoded, 0, 63));
}

float AicaAttackRate(std::uint8_t decoded) {
    if (decoded <= 1)
        return 0.0f;
    return 1000.0f / kAicaAttackMilliseconds[decoded];
}

float AicaDecayRate(std::uint8_t decoded) {
    if (decoded <= 1)
        return 0.0f;
    return 1000.0f / kAicaReleaseMilliseconds[decoded];
}

void ConfigureAicaEnvironmentEnvelope(AicaState &state, const std::uint8_t *entry, std::uint32_t owner) {
    state.noteNo = 60;
    state.phase = 0;
    state.aicaTone = nullptr;
    state.ownerId = owner;
    const bool modern = owner < modernBanks.size() && modernBanks[owner];
    state.legacyEnvelope = !modern;
    if (modern)
        state.holdTime = 0.0f;
    const std::uint8_t attack = DecodeAicaEnvelopeRate(60, 15, entry[6] & 0x1fU);
    const std::uint8_t release = DecodeAicaEnvelopeRate(60, 15, entry[7] & 0x1fU);
    state.attackRate = modern ? static_cast<float>(read_unaligned<std::uint16_t>(entry + 6)) : AicaAttackRate(attack);
    state.decay1Rate = 0.0f;
    state.decay2Rate = 0.0f;
    state.releaseRate = modern ? static_cast<float>(read_unaligned<std::uint16_t>(entry + 8)) : release <= 1 ? 60000.0f : AicaDecayRate(release);
    state.egh = 0;
    state.lsl = 0;
    state.sustainLevel = 1.0f;
    state.currentFilterLevel = 1.0f;
    state.instantFilterAttack = 1;
    state.oneOverQ = 1.0f;
}

void ConfigureAicaToneEnvelope(AicaState &state, const std::uint8_t *entry, std::int32_t note, std::uint32_t owner) {
    state.aicaTone = const_cast<std::uint8_t *>(entry);
    state.noteNo = static_cast<std::uint32_t>(note);
    state.ownerId = owner;
    state.phase = AICA_ENVELOPE_OFF;
    state.legacyEnvelope = 1;
    const std::uint8_t keyRate = (entry[15] >> 2U) & 0x0fU;
    const std::uint8_t attack = DecodeAicaEnvelopeRate(note, keyRate, entry[12] & 0x1fU);
    const std::uint8_t decay1 = DecodeAicaEnvelopeRate(note, keyRate, (entry[12] >> 6U) | (4U * (entry[13] & 7U)));
    const std::uint8_t decay2 = DecodeAicaEnvelopeRate(note, keyRate, entry[13] >> 3U);
    const std::uint8_t release = DecodeAicaEnvelopeRate(note, keyRate, entry[14] & 0x1fU);
    state.attackRate = AicaAttackRate(attack);
    state.decay1Rate = AicaDecayRate(decay1);
    state.decay2Rate = AicaDecayRate(decay2);
    state.releaseRate = release <= 1 ? 60000.0f : AicaDecayRate(release);
    state.egh = (entry[12] & 0x20U) != 0;
    state.lsl = (entry[15] & 0x40U) != 0;
    const std::int32_t decayLevel = (entry[14] >> 5U) | (8U * (entry[15] & 3U));
    state.sustainLevel = static_cast<float>(31 - decayLevel) / 31.0f;
}

bool ConfigureAicaToneFilter(AicaState &state, const std::uint8_t *entry, std::int32_t note) {
    const std::uint8_t keyRate = entry[15] & 0x1fU;
    const std::uint8_t attack = DecodeAicaEnvelopeRate(note, keyRate, entry[17] >> 4U);
    const std::uint8_t decay1 = DecodeAicaEnvelopeRate(note, keyRate, entry[19] >> 4U);
    const std::uint8_t decay2 = DecodeAicaEnvelopeRate(note, keyRate, entry[21] >> 4U);
    const std::uint8_t release = DecodeAicaEnvelopeRate(note, keyRate, entry[23] >> 4U);
    state.filterLevel0 = static_cast<float>(entry[16] | ((entry[17] & 7U) << 8U)) / 2047.0f;
    state.filterLevel1 = static_cast<float>(entry[18] | ((entry[19] & 7U) << 8U)) / 2047.0f;
    state.filterLevel2 = static_cast<float>(entry[20] | ((entry[21] & 7U) << 8U)) / 2047.0f;
    state.filterLevel3 = static_cast<float>(entry[22] | ((entry[23] & 7U) << 8U)) / 2047.0f;
    state.filterReleaseLevel = static_cast<float>(entry[24] | ((entry[25] & 7U) << 8U)) / 2047.0f;
    state.oneOverQ = kAicaFilterQ[entry[25] >> 3U];
    state.filterAttackTime = attack >= 2 ? kAicaFilterMilliseconds[attack] / 1000.0f : 0.0f;
    state.filterDecay1Time = state.filterAttackTime + kAicaFilterMilliseconds[decay1] / 1000.0f;
    state.filterDecay2Time = state.filterDecay1Time + kAicaFilterMilliseconds[decay2] / 1000.0f;
    state.filterReleaseTime = kAicaFilterMilliseconds[release] / 1000.0f;
    state.instantFilterAttack = attack == 0;
    return state.instantFilterAttack != 0;
}

void SetAttackFilterParams(AicaState &state) {
    state.envelopeLevel = 0.0f;
    state.filterElapsed = 0.0f;
    state.currentFilterLevel = state.filterLevel0;
    state.currentVolume = (std::numeric_limits<float>::max)();
    state.envelopeCounter = 0;
    state.phase = AICA_ENVELOPE_ATTACK;
    ApplyAicaEnvelopeGain(state);
    if (state.legacyEnvelope) {
        const float frequency = state.instantFilterAttack && state.filterLevel0 == 0.0f ? 1.0f : std::pow(10.0f, state.currentFilterLevel * 4.0f) / 10000.0f;
        SetRawSoundEffectLowPassFrequency(GetRawSoundEffectVoice(state.voiceId), frequency, state.oneOverQ);
    }
}

void SetAttackFilterParamsForEnvironment(std::uint32_t physicalId, const std::uint8_t *entry, float volume, std::uint32_t owner) {
    AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
    if (!state)
        return;
    state->voiceId = physicalId;
    ConfigureAicaEnvironmentEnvelope(*state, entry, owner);
    state->linearVolume = volume;
    state->baseVolumeDb = volume == 0.0f ? -64.0f : (std::max)(-64.0f, std::log10(volume) * 20.0f);
    SetAttackFilterParams(*state);
}

void SetAttackFilterParamsForTone(std::uint32_t physicalId, const std::uint8_t *entry, std::int32_t note, float volume, bool music, std::uint32_t owner) {
    if (music)
        volume *= g_s2CurrentMusicVolume;
    AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
    if (!state)
        return;
    state->voiceId = physicalId;
    ConfigureAicaToneEnvelope(*state, entry, note, owner);
    ConfigureAicaToneFilter(*state, entry, note);
    state->linearVolume = volume;
    state->baseVolumeDb = volume == 0.0f ? -64.0f : (std::max)(-64.0f, std::log10(volume) * 20.0f);
    SetAttackFilterParams(*state);
}

void EnvelopeGenerator_StopAndReset_1(std::uint32_t physicalId) {
    StopRawSoundEffectPlayback(GetRawSoundEffectVoice(physicalId));
}

bool ReleaseEnvelope(std::uint32_t physicalId) {
    AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
    if (!state || !state->phase)
        return false;
    if (state->phase != AICA_ENVELOPE_RELEASE) {
        state->voiceId = physicalId;
        state->phase = AICA_ENVELOPE_RELEASE;
        state->filterElapsed = 0.0f;
        state->envelopeCounter = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(TruncateToInt32(state->releaseRate)) << 4U);
        state->releaseStartLevel = TruncateToInt32(std::ceil(state->envelopeLevel * 255.0f));
    }
    return true;
}

void ApplyFilter(AicaState &envelope, float elapsedSeconds) {
    bool stopPlayback = false;
    switch (envelope.phase) {
    case AICA_ENVELOPE_ATTACK:
        envelope.envelopeLevel = (std::min)(1.0f, envelope.envelopeLevel + elapsedSeconds * envelope.attackRate);
        if (envelope.envelopeLevel >= 1.0f) {
            envelope.phase = envelope.sustainLevel < 1.0f ? AICA_ENVELOPE_DECAY1 : AICA_ENVELOPE_DECAY2;
        }
        break;
    case AICA_ENVELOPE_DECAY1:
        envelope.envelopeLevel = (std::max)(0.0f, envelope.envelopeLevel - elapsedSeconds * envelope.decay1Rate);
        if (envelope.currentVolume <= -64.0f) {
            stopPlayback = true;
        } else if (envelope.envelopeLevel < envelope.sustainLevel) {
            envelope.envelopeLevel = envelope.sustainLevel;
            envelope.phase = AICA_ENVELOPE_DECAY2;
        }
        break;
    case AICA_ENVELOPE_DECAY2:
        envelope.envelopeLevel = (std::max)(0.0f, envelope.envelopeLevel - elapsedSeconds * envelope.decay2Rate);
        stopPlayback = envelope.currentVolume <= -64.0f;
        break;
    case AICA_ENVELOPE_RELEASE:
        envelope.envelopeLevel = (std::max)(0.0f, envelope.envelopeLevel - elapsedSeconds * envelope.releaseRate);
        stopPlayback = envelope.currentVolume <= -64.0f;
        break;
    }
    if (stopPlayback) {
        envelope.phase = 0;
        StopRawSoundEffectPlayback(GetRawSoundEffectVoice(envelope.voiceId));
    }
    if (!envelope.phase)
        return;
    UpdateAicaLowPassFilter(envelope.voiceId, envelope, elapsedSeconds);
    ApplyAicaEnvelopeGain(envelope);
}

// @verified - p
void UpdateModernAicaEnvelope(AicaState &envelope) {
    bool beginDecay = false;
    bool stopPlayback = false;
    switch (envelope.phase) {
    case AICA_ENVELOPE_ATTACK:
        envelope.envelopeCounter = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(envelope.envelopeCounter) + AICA_MODERN_ENVELOPE_STEP);
        envelope.envelopeLevel = static_cast<float>(envelope.envelopeCounter) / envelope.attackRate;
        if (envelope.envelopeLevel >= 1.0f) {
            envelope.envelopeLevel = 1.0f;
            if (envelope.holdTime > 0.0f)
                envelope.envelopeCounter = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(TruncateToInt32(envelope.holdTime)) << 4U);
            else
                beginDecay = true;
        }
        break;
    case AICA_ENVELOPE_HOLD:
        envelope.envelopeCounter = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(envelope.envelopeCounter) - AICA_MODERN_ENVELOPE_STEP);
        beginDecay = envelope.envelopeCounter <= 0;
        break;
    case AICA_ENVELOPE_DECAY1:
        envelope.envelopeCounter = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(envelope.envelopeCounter) - AICA_MODERN_ENVELOPE_STEP);
        if (envelope.envelopeCounter <= 0) {
            envelope.envelopeLevel = envelope.sustainLevel;
            envelope.phase = AICA_ENVELOPE_DECAY2;
        } else {
            const float elapsed = (envelope.decay1Rate * 16.0f) - static_cast<float>(envelope.envelopeCounter);
            envelope.envelopeLevel = std::pow(AICA_MODERN_ENVELOPE_DECAY_BASE, elapsed * (4096.0f / envelope.decay1Rate));
            if (envelope.envelopeLevel < 0.0f)
                envelope.envelopeLevel = 0.0f;
            if (envelope.envelopeLevel <= 0.0f) {
                stopPlayback = true;
            } else if (envelope.sustainLevel > envelope.envelopeLevel) {
                envelope.envelopeLevel = envelope.sustainLevel;
                envelope.phase = AICA_ENVELOPE_DECAY2;
            }
        }
        break;
    case AICA_ENVELOPE_DECAY2:
        envelope.envelopeCounter = 0;
        envelope.envelopeLevel = envelope.sustainLevel;
        break;
    case AICA_ENVELOPE_RELEASE:
        if (!(envelope.releaseRate > 0.0f)) {
            envelope.envelopeLevel = 0.0f;
        } else {
            envelope.envelopeCounter = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(envelope.envelopeCounter) - AICA_MODERN_ENVELOPE_STEP);
            const std::int32_t scaled = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(envelope.envelopeCounter) *
                static_cast<std::uint32_t>(envelope.releaseStartLevel));
            const float level = static_cast<float>(scaled) / ((envelope.releaseRate * 16.0f) * 255.0f);
            envelope.envelopeLevel = level > 0.0f ? (level * level) * level : 0.0f;
        }
        if (envelope.envelopeLevel <= 0.0f) {
            envelope.envelopeLevel = 0.0f;
            ApplyAicaEnvelopeGain(envelope);
            stopPlayback = true;
        }
        break;
    }
    if (beginDecay) {
        if (!(envelope.sustainLevel >= 1.0f)) {
            envelope.phase = AICA_ENVELOPE_DECAY1;
            envelope.envelopeCounter = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(TruncateToInt32(envelope.decay1Rate)) << 4U);
        } else {
            envelope.phase = AICA_ENVELOPE_DECAY2;
        }
    }
    if (stopPlayback) {
        envelope.phase = AICA_ENVELOPE_OFF;
        StopRawSoundEffectPlayback(GetRawSoundEffectVoice(envelope.voiceId));
    }
    if (envelope.phase)
        ApplyAicaEnvelopeGain(envelope);
}

void UpdateAicaEnvironmentEnvelopes(float elapsedSeconds = 1.0f / 30.0f) {
    for (std::uint32_t physicalId = 0; physicalId < 160; ++physicalId) {
        AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
        if (state && state->phase) {
            if (state->legacyEnvelope)
                ApplyFilter(*state, elapsedSeconds);
            else
                UpdateModernAicaEnvelope(*state);
        }
    }
}

void StopReleasedAicaEnvelopesForOwner(std::int32_t owner) {
    for (std::uint32_t physicalId = 0; physicalId < 160; ++physicalId) {
        AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
        if (state && state->phase == AICA_ENVELOPE_RELEASE && (state->ownerId == static_cast<std::uint32_t>(owner) || owner == -1)) {
            state->phase = 0;
            StopRawSoundEffectPlayback(GetRawSoundEffectVoice(state->voiceId));
        }
    }
}

void ReleaseAicaVoice(std::uint32_t index, bool releaseEnvelope) {
    AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[index];
    if (voice.status == 0xff || voice.playState != 1)
        return;
    if (voice.xaudioStreamKind != -1) {
        StopRawSoundEffectPlayback(g_aicaPlaybackOwners[index], releaseEnvelope);
        voice.xaudioStreamKind = -1;
    }
    voice.playState = 2;
}

void StopAicaEnvironmentPart(std::uint8_t stateIndex, std::uint8_t partIndex) {
    if (!g_aicaMemory || stateIndex >= 16 || partIndex >= 16)
        return;
    AicaEnvironmentPartState &part = g_aicaEnvironmentStates[stateIndex].parts[partIndex];
    if (part.active && part.voiceIndex < std::size(g_aicaMemory->voicePool.slots)) {
        AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[part.voiceIndex];
        if (voice.status == 3 && voice.bank == stateIndex && voice.part == partIndex) {
            ResetAicaVoice(voice);
        }
    }
    part.active = 0;
    part.flags = 0;
    part.voiceIndex = 0xff;
}

void ReleaseAicaEnvironmentPart(std::uint8_t stateIndex, std::uint8_t partIndex) {
    if (!g_aicaMemory || stateIndex >= 16 || partIndex >= 16)
        return;
    AicaEnvironmentPartState &part = g_aicaEnvironmentStates[stateIndex].parts[partIndex];
    if (part.voiceIndex >= std::size(g_aicaMemory->voicePool.slots)) {
        part.active = 0;
        part.flags = 0;
        part.voiceIndex = 0xff;
        return;
    }
    AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[part.voiceIndex];
    if (!part.active || voice.status != 3 || voice.bank != stateIndex || voice.part != partIndex) {
        part.active = 0;
        part.flags = 0;
        part.voiceIndex = 0xff;
        return;
    }
    ReleaseAicaVoice(part.voiceIndex, true);
}

std::int32_t AicaEnvironmentVolume(const AicaEnvironmentState &state, const std::uint8_t *entry, std::uint8_t value) {
    if (!g_aicaMemory || state.loadedBank >= 16 || !entry)
        return -96;
    const AicaBankSlot &slot = g_aicaMemory->bankSlots[state.loadedBank];
    const std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[state.loadedBank];
    const std::size_t entrySize = entry[0];
    if (!AicaBankPointerValid(bankData, bankSize, entry, entrySize) || entrySize < 16) {
        return -96;
    }

    const bool modern = modernBanks[state.loadedBank];
    if (modern && entrySize < 28)
        return -96;
    const std::uint8_t segmentCount = entry[modern ? 16 : 12];
    const std::size_t curveOffset = (modern ? 16U : 12U) + entry[modern ? 17 : 13];
    if (curveOffset > entrySize || 4U > entrySize - curveOffset)
        return -96;
    std::size_t segmentOffset = curveOffset;
    const std::uint8_t *segment = entry + segmentOffset;
    std::uint8_t rangeStart = entry[4];
    for (std::uint8_t index = 0; index < segmentCount; ++index) {
        if (value <= segment[0])
            break;
        rangeStart = static_cast<std::uint8_t>(segment[0] + 1U);
        segmentOffset += 4;
        if (segmentOffset > entrySize || 4U > entrySize - segmentOffset) {
            return -96;
        }
        segment = entry + segmentOffset;
    }
    const std::int32_t curveValue = static_cast<std::int32_t>(segment[1]) +
        ((static_cast<std::int32_t>(read_unaligned<std::int16_t>(segment + 2)) * (static_cast<std::int32_t>(value) - rangeStart)) >> 8);
    if (bankSize < 0x2c)
        return -96;
    const std::size_t velocityOffset = static_cast<std::size_t>(read_unaligned<std::uint32_t>(bankData + (modern ? 0xa0 : 0x28))) + 132U +
        static_cast<std::size_t>(std::clamp(curveValue, 0, 255));
    if (velocityOffset >= bankSize)
        return -96;

    std::uint32_t level = bankData[velocityOffset];
    const auto scale = [](std::uint32_t current, std::uint32_t amount) {
        const std::uint32_t next = (2U * amount * current) >> 8U;
        return next ? next + 1U : 0U;
    };
    level = scale(level, state.volume);
    const std::int32_t offsetLevel = std::clamp(2 * (static_cast<std::int32_t>(state.volumeOffset) - 64) + 127, 0, 127);
    level = scale(level, static_cast<std::uint32_t>(offsetLevel));
    level = scale(level, state.mixLevel);
    level = scale(level, static_cast<std::uint8_t>(g_aicaMemory->globalVolume));
    return AicaLevelToHundredthDecibels(level) / 100;
}

std::int32_t AicaEnvironmentPitch(const AicaEnvironmentState &state, const std::uint8_t *entry, std::uint8_t value, std::int32_t *result) {
    *result = 0;
    if (!g_aicaMemory || state.loadedBank >= 16 || !entry)
        return *result;
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[state.loadedBank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[state.loadedBank];
    const std::size_t entrySize = entry[0];
    if (!AicaBankPointerValid(bankData, bankSize, entry, entrySize) || entrySize < 22) {
        return *result;
    }
    const bool modern = modernBanks[state.loadedBank];
    if (modern && entrySize < 28)
        return *result;
    const std::uint8_t segmentCount = entry[modern ? 22 : 20];
    const std::size_t curveOffset = (modern ? 16U : 12U) + entry[modern ? 23 : 21];
    if (curveOffset > entrySize || 6U > entrySize - curveOffset)
        return *result;
    std::size_t segmentOffset = curveOffset;
    const std::uint8_t *segment = entry + segmentOffset;
    std::uint8_t rangeStart = entry[4];
    for (std::uint8_t index = 0; index < segmentCount; ++index) {
        if (value <= segment[0])
            break;
        rangeStart = static_cast<std::uint8_t>(segment[0] + 1U);
        segmentOffset += 6;
        if (segmentOffset > entrySize || 6U > entrySize - segmentOffset) {
            return *result;
        }
        segment = entry + segmentOffset;
    }
    const std::int32_t base = read_unaligned<std::uint16_t>(segment + 2);
    const std::int32_t slope = read_unaligned<std::int16_t>(segment + 4);
    const double pitch = ((static_cast<double>(slope * (static_cast<std::int32_t>(value) - rangeStart)) / 256.0) + static_cast<double>(base - 3072)) * 3.125 +
        (static_cast<std::int32_t>(state.coarsePitch) - 64) * 100.0 + (static_cast<std::int32_t>(state.finePitch) - 64) * 1.5625;
    *result = static_cast<std::int32_t>((std::min)(8191.0, pitch) * 4096.0 / 1200.0);
    return *result;
}
float MIDIToPitchHz(const AicaEnvironmentState &state, const std::uint8_t *entry, std::uint8_t value) {
    if (!g_aicaMemory || state.loadedBank >= 16 || !entry)
        return 20000.0f;
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[state.loadedBank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[state.loadedBank];
    const std::size_t entrySize = entry[0];
    const bool modern = modernBanks[state.loadedBank];
    if (!AicaBankPointerValid(bankData, bankSize, entry, entrySize) || entrySize < (modern ? 28U : 22U) || entry[modern ? 23 : 21] == 0 ||
        entry[modern ? 19 : 15] == 0) {
        return 20000.0f;
    }

    const std::uint8_t segmentCount = entry[modern ? 18 : 14];
    const std::size_t curveOffset = (modern ? 16U : 12U) + entry[modern ? 19 : 15];
    if (curveOffset > entrySize || 4U > entrySize - curveOffset)
        return 20000.0f;
    std::size_t segmentOffset = curveOffset;
    const std::uint8_t *segment = entry + segmentOffset;
    std::uint8_t rangeStart = entry[4];
    for (std::uint8_t index = 0; index < segmentCount; ++index) {
        if (value <= segment[0])
            break;
        rangeStart = static_cast<std::uint8_t>(segment[0] + 1U);
        segmentOffset += 4;
        if (segmentOffset > entrySize || 4U > entrySize - segmentOffset) {
            return 20000.0f;
        }
        segment = entry + segmentOffset;
    }
    const double cutoff =
        static_cast<double>(read_unaligned<std::int16_t>(segment + 2)) * static_cast<double>(static_cast<std::int32_t>(value) - rangeStart) / 256.0 +
        8.0 * static_cast<double>(segment[1]) + 6136.0;
    return static_cast<float>(cutoff);
}

void StartOrUpdateAicaEnvironmentPart(std::uint8_t stateIndex, std::uint8_t partIndex, const std::uint8_t *entry, std::uint8_t value) {
    if (!g_aicaMemory || stateIndex >= 16 || partIndex >= 16 || !entry)
        return;
    AicaEnvironmentState &state = g_aicaEnvironmentStates[stateIndex];
    if (state.loadedBank >= 16)
        return;
    AicaBankSlot &slot = g_aicaMemory->bankSlots[state.loadedBank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[state.loadedBank];
    const bool modern = modernBanks[state.loadedBank];
    const std::size_t flagsOffset = modern ? 11U : 10U;
    const std::size_t entrySize = entry[0];
    if (!AicaBankPointerValid(bankData, bankSize, entry, entrySize) || entrySize < (modern ? 28U : 22U)) {
        return;
    }

    AicaEnvironmentPartState &part = state.parts[partIndex];
    const std::int32_t decibels = AicaEnvironmentVolume(state, entry, value);
    const float volume = decibels > -64 ? std::pow(10.0f, static_cast<float>(decibels) / 20.0f) : 0.0f;
    std::int32_t pitchUnits = 0;
    AicaEnvironmentPitch(state, entry, value, &pitchUnits);
    const float frequencyRatio = std::pow(2.0f, (static_cast<float>(pitchUnits) * 3.0f * 0.0009765625f) / 12.0f);
    const float lowPassFrequency = MIDIToPitchHz(state, entry, value) / 20000.0f;
    if (part.active && (entry[flagsOffset] & 2U) == 0) {
        if (part.voiceIndex < g_aicaPlaybackOwners.size() && g_aicaPlaybackOwners[part.voiceIndex]) {
            SetRawSoundEffectVolume(g_aicaPlaybackOwners[part.voiceIndex], volume);
            SetRawSoundEffectFrequencyRatio(g_aicaPlaybackOwners[part.voiceIndex], frequencyRatio);
            SetRawSoundEffectLowPassFrequency(g_aicaPlaybackOwners[part.voiceIndex], lowPassFrequency);
            const AicaVoiceSlot &activeVoice = g_aicaMemory->voicePool.slots[part.voiceIndex];
            const float outputLevel = static_cast<float>(activeVoice.field_10) / 127.0f;
            const std::array<float, 2> outputMatrix{outputLevel, outputLevel};
            SetRawSoundEffectOutputMatrix(g_aicaPlaybackOwners[part.voiceIndex], outputMatrix.data(), activeVoice.stereo ? 2U : 1U, 1);
        }
        return;
    }
    if (part.active && (entry[flagsOffset] & 3U) == 3U)
        ReleaseAicaEnvironmentPart(stateIndex, partIndex);

    const std::uint16_t sampleId = read_unaligned<std::uint16_t>(entry + 2);
    BankSample sample{};
    if (!ResolveBankSample(state.loadedBank, sampleId, true, sample))
        return;
    if (entry[modern ? 10 : 8] >= kAicaPanAttenuation.size())
        return;

    const std::uint8_t allocatedVoice = AllocateAicaVoice(2);
    if (allocatedVoice == 0xff)
        return;
    const std::size_t voiceIndex = allocatedVoice;
    void *const playback = PrepareRawSoundEffect(
        sample.data, sample.bytes, sample.loopBegin, sample.loopLength, sample.channels, volume, frequencyRatio, sample.rate, true, sample.bits);
    if (!playback) {
        ReturnAicaVoice(2, allocatedVoice);
        return;
    }
    constexpr std::array<float, 2> initialEnvironmentReverb{0.0f, 0.0f};
    SetRawSoundEffectOutputMatrix(playback, initialEnvironmentReverb.data(), sample.channels, 1);

    AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[voiceIndex];
    voice.status = 3;
    voice.playState = 0;
    voice.ownerId = -1;
    voice.bank = stateIndex;
    voice.part = partIndex;
    voice.spawnCommand = 0;
    voice.driverFlags = 0;
    voice.layerFlags = 0;
    voice.playFlags = 0;
    voice.noteAttenuation = 0;
    voice.stereo = sample.channels == 2;
    voice.field_10 = 0;
    voice.field_14 = 0;
    voice.pitchOrBaseNote = kAicaPanAttenuation[entry[modern ? 10 : 8]];
    voice.xaudioVoiceId = RawSoundEffectVoiceId(playback);
    voice.xaudioStreamKind = 1;
    g_aicaPlaybackOwners[voiceIndex] = playback;
    SetRawSoundEffectLowPassFrequency(playback, lowPassFrequency);
    SetAttackFilterParamsForEnvironment(voice.xaudioVoiceId, entry, volume, state.loadedBank);
    StartRawSoundEffectPlayback(playback);
    voice.playState = 1;
    part.active = 1;
    part.flags = entry[flagsOffset];
    part.voiceIndex = static_cast<std::uint8_t>(voiceIndex);
}

void UpdateAicaEnvironment(std::uint8_t stateIndex, std::uint8_t value) {
    if (stateIndex >= 16)
        return;
    AicaEnvironmentState &state = g_aicaEnvironmentStates[stateIndex];
    if (!state.active || !state.partTable || state.loadedBank >= 16)
        return;
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[state.loadedBank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[state.loadedBank];
    const std::uint8_t *entry = state.partTable;
    for (std::uint8_t partIndex = 0; partIndex <= state.maximumPart && partIndex < 16; ++partIndex) {
        if (!AicaBankPointerValid(bankData, bankSize, entry, 6) || entry[0] < 6 || !AicaBankPointerValid(bankData, bankSize, entry, entry[0])) {
            StopAicaEnvironmentPart(stateIndex, partIndex);
            break;
        }
        if (value && value >= entry[4] && value <= entry[5]) {
            StartOrUpdateAicaEnvironmentPart(stateIndex, partIndex, entry, value);
        } else if (value) {
            const AicaEnvironmentPartState &part = state.parts[partIndex];
            if (part.active && part.voiceIndex != 0xff) {
                ReleaseAicaVoice(part.voiceIndex, false);
            }
        } else {
            ReleaseAicaEnvironmentPart(stateIndex, partIndex);
        }
        entry += entry[0];
    }
    state.currentValue = value;
}

void SetupAicaEnvironment(std::uint8_t stateIndex, std::uint8_t programIndex) {
    if (!g_aicaMemory || stateIndex >= 16)
        return;
    AicaEnvironmentState &state = g_aicaEnvironmentStates[stateIndex];

    std::uint8_t loadedBank = 0xff;
    for (std::uint8_t bank = 0; bank < 16; ++bank) {
        if (static_cast<std::uint8_t>(g_aicaMemory->bankSlots[bank].loadedSize >> 8U) == state.bankType) {
            loadedBank = bank;
            break;
        }
    }
    if (loadedBank == 0xff)
        return;
    if (state.active) {
        for (std::uint8_t part = 0; part <= state.maximumPart; ++part)
            ReleaseAicaEnvironmentPart(stateIndex, part);
        state.currentValue = 0;
    }
    state.active = 1;
    AicaBankSlot &slot = g_aicaMemory->bankSlots[loadedBank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[loadedBank];
    if (!AicaBankPointerValid(bankData, bankSize, bankData, 0x38))
        return;
    const std::size_t tableOffset = read_unaligned<std::uint32_t>(bankData + (modernBanks[loadedBank] ? 0xac : 0x34));
    std::uint8_t *const table = AicaBankOffset(bankData, bankSize, tableOffset, 8);
    if (!table || read_unaligned<std::uint32_t>(table) < programIndex)
        return;
    const std::size_t offsetEntry = 4U + 4U * programIndex;
    const std::uint8_t *const offsetBytes = AicaBankOffset(bankData, bankSize, tableOffset + offsetEntry, 4);
    if (!offsetBytes)
        return;
    const std::size_t programOffset = read_unaligned<std::uint32_t>(offsetBytes);
    std::uint8_t *const program = AicaBankOffset(bankData, bankSize, tableOffset + programOffset, 8);
    if (!program || program[0] >= 16)
        return;

    state.loadedBank = loadedBank;
    state.maximumPart = program[0];
    state.field09 = program[1];
    state.mixLevel = program[2];
    state.field0b = static_cast<std::uint8_t>(program[3] + 15U);
    state.partTable = program + 8;
    state.currentValue = 0;
    for (AicaEnvironmentPartState &part : state.parts) {
        part.active = 0;
        part.voiceIndex = 0xff;
    }
}

void ExecuteAicaEnvironmentCommand() {
    if (!g_aicaMemory)
        return;
    const std::uint32_t command = g_aicaMemory->currentCommand;
    const std::uint8_t stateIndex = static_cast<std::uint8_t>((command >> 16) & 0x0fU);
    const std::uint32_t operation = command & 0xff700000U;
    const std::uint8_t value = static_cast<std::uint8_t>((command >> 8) & 0x7fU);
    AicaEnvironmentState &state = g_aicaEnvironmentStates[stateIndex];
    switch (operation) {
    case 0xa5000000U:
        SetupAicaEnvironment(stateIndex, value);
        return;
    case 0xa5100000U:
        state.volume = value;
        break;
    case 0xa5200000U:
        state.control = value;
        break;
    case 0xa5400000U:
        for (AicaEnvironmentPartState &part : state.parts) {
            if (part.active && part.voiceIndex < std::size(g_aicaMemory->voicePool.slots)) {
                g_aicaMemory->voicePool.slots[part.voiceIndex].field_10 = value;
            }
        }
        return;
    case 0xa5600000U:
        state.volumeOffset = static_cast<std::int8_t>(value);
        break;
    case 0xa5700000U:
        state.bankType = value;
        return;
    case 0xa6000000U:
        UpdateAicaEnvironment(stateIndex, static_cast<std::uint8_t>(value + ((command >> 13U) & 0x80U)));
        return;
    case 0xa7000000U:
        state.coarsePitch = static_cast<std::int8_t>(value);
        break;
    case 0xa7100000U:
        state.finePitch = static_cast<std::int8_t>(value);
        break;
    default:
        return;
    }
    if (state.active && state.currentValue)
        UpdateAicaEnvironment(stateIndex, state.currentValue);
}

bool RecomputeAicaPreBankLevel(std::size_t index, std::uint8_t &level) {
    if (!g_aicaMemory || index >= g_aicaPlaybackOwners.size())
        return false;
    const AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[index];
    const std::uint8_t config = static_cast<std::uint8_t>(voice.ownerId);
    if (config >= 64 || voice.bank >= 16 || !g_aicaMemory->bankRegionLoaded[voice.bank]) {
        return false;
    }
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[voice.bank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[voice.bank];
    const auto *const table = reinterpret_cast<const std::uint8_t *>(voice.driverPointerTable);
    const auto *const note = reinterpret_cast<const std::uint8_t *>(voice.driverNoteTable);
    if (!AicaBankPointerValid(bankData, bankSize, table, 7) || !AicaBankPointerValid(bankData, bankSize, note, 12) || bankSize < 0x2c) {
        return false;
    }
    const bool modern = modernBanks[voice.bank];
    const std::uint32_t velocityTableOffset = read_unaligned<std::uint32_t>(bankData + (modern ? 0xa0 : 0x28));
    const std::size_t velocityOffset = static_cast<std::size_t>(velocityTableOffset) + 4U + 128U * static_cast<std::size_t>(table[6]) +
        static_cast<std::size_t>((voice.spawnCommand >> 8) & 0x7fU);
    if (velocityOffset >= bankSize)
        return false;

    const auto *const memory = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * config;
    std::uint32_t computed = (static_cast<std::uint32_t>(bankData[velocityOffset]) *
                                 (modern ? note[8] & 0xfeU : ((static_cast<std::uint32_t>(note[8]) + note[9]) >> 1U) & 0x7ffffffeU)) >>
        8U;
    if (computed)
        ++computed;
    computed = (2U * memory[0x8cc + configBase] * computed) >> 8U;
    if (computed)
        ++computed;
    computed = (2U * memory[0x8dc + configBase] * computed) >> 8U;
    if (computed)
        ++computed;
    level = static_cast<std::uint8_t>(computed);
    return true;
}

void UpdateAicaPlaybackPitch(std::size_t index) {
    if (!g_aicaMemory || index >= g_aicaPlaybackOwners.size() || !g_aicaPlaybackOwners[index]) {
        return;
    }
    const AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[index];
    const std::uint8_t config = static_cast<std::uint8_t>(voice.ownerId);
    const std::uint8_t *const note = reinterpret_cast<const std::uint8_t *>(voice.driverNoteTable);
    if (config >= 64 || voice.bank >= 16 || !g_aicaMemory->bankRegionLoaded[voice.bank]) {
        return;
    }
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[voice.bank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[voice.bank];
    const std::uint8_t *const memory = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * config;
    const std::uint8_t *const driverBase = read_unaligned<std::uint8_t *>(memory + 0x8f0 + configBase);
    if (!AicaBankPointerValid(bankData, bankSize, driverBase, 1) || !AicaBankPointerValid(bankData, bankSize, note, 12)) {
        return;
    }
    const std::int32_t pitchNote = driverBase[0] == 0 ? static_cast<std::int32_t>((voice.spawnCommand >> 16) & 0x7fU) - 60 : 0;
    const bool modern = modernBanks[voice.bank];
    const auto *table = reinterpret_cast<const std::uint8_t *>(voice.driverPointerTable);
    if (modern && !AicaBankPointerValid(bankData, bankSize, table, 27))
        return;
    const int bend = static_cast<int>(memory[0x8e2 + configBase]) - (memory[0x8e2 + configBase] > 64 ? 63 : 64);
    const int bendPitch = modern ? bend * static_cast<std::int8_t>(table[bend >= 0 ? 25 : 26]) * 256 : 0;
    const std::int32_t pitchUnits = static_cast<std::int32_t>((static_cast<double>(static_cast<std::int8_t>(memory[0x8db + configBase]) +
                                                                   static_cast<std::int8_t>(note[modern ? 10 : 11]) + ((bendPitch >> 8) & 63)) *
                                                                  1024.0 / 3.0 * 0.015625) +
        (static_cast<double>(
             static_cast<std::int8_t>(memory[0x8da + configBase]) + static_cast<std::int8_t>(note[modern ? 9 : 10]) + pitchNote + (bendPitch >> 14)) *
            1024.0 / 3.0));
    if ((static_cast<std::uint32_t>(pitchUnits) + 0x7fffU) > 0x9ffeU)
        return;
    SetRawSoundEffectFrequencyRatio(g_aicaPlaybackOwners[index], std::pow(2.0f, static_cast<float>(pitchUnits) * (1.0f / 4096.0f)));
}

void ApplyVoicePanMatrixToXAudio(std::size_t index) {
    if (!g_aicaMemory || index >= g_aicaPlaybackOwners.size() || !g_aicaPlaybackOwners[index]) {
        return;
    }
    const AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[index];
    const std::uint8_t config = static_cast<std::uint8_t>(voice.ownerId);
    const auto *const note = reinterpret_cast<const std::uint8_t *>(voice.driverNoteTable);
    if (config >= 64 || voice.bank >= 16 || !g_aicaMemory->bankRegionLoaded[voice.bank]) {
        return;
    }
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[voice.bank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[voice.bank];
    if (!AicaBankPointerValid(bankData, bankSize, note, 46))
        return;

    const auto *const memory = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * config;
    const bool modern = modernBanks[voice.bank];
    const std::int8_t authoredPan = static_cast<std::int8_t>(note[6]);
    const std::uint8_t pan = authoredPan >= 0 ? (modern ? static_cast<std::uint8_t>(authoredPan) : static_cast<std::uint8_t>(authoredPan) & 0x1fU) :
                                             memory[0x8cd + configBase];
    const std::uint8_t centerLevel = modern ? (authoredPan >= 0 ? note[41] : memory[0x8d3 + configBase]) : note[9];
    const std::uint8_t middleLevel = modern ? (authoredPan >= 0 ? note[37] : memory[0x8cf + configBase]) : static_cast<std::uint8_t>((note[8] + note[9]) / 2);
    const std::uint8_t sideLevel = modern ? (authoredPan >= 0 ? note[39] : memory[0x8d1 + configBase]) : note[8];
    std::int32_t center = static_cast<std::int32_t>((127.0f - centerLevel) * -37.5f);
    std::int32_t middle = static_cast<std::int32_t>((127.0f - middleLevel) * -37.5f);
    std::int32_t side = static_cast<std::int32_t>((127.0f - sideLevel) * -37.5f);
    if (g_aicaMemory->useGlobalPanOverride) {
        middle = static_cast<std::int32_t>((127.0f - g_aicaMemory->globalPanLeft) * -37.5f);
        side = static_cast<std::int32_t>((127.0f - g_aicaMemory->globalPanRight) * -37.5f);
        center = side;
    }
    const std::int32_t leftPan = pan >= 16 ? 0 : -300 * static_cast<std::int32_t>(pan);
    const std::int32_t rightPan = pan >= 16 ? 300 * (16 - static_cast<std::int32_t>(pan)) : 0;
    const std::int32_t base = voice.pitchOrBaseNote;
    const auto gain = [](std::int32_t hundredthDb) {
        if (hundredthDb <= -6400)
            return 0.0f;
        const float decibels = static_cast<float>(hundredthDb) / 100.0f;
        return std::pow(10.0f, decibels / 20.0f);
    };

    std::array<float, 12> matrix{};
    const std::uint32_t channels = voice.stereo ? 2U : 1U;
    if (channels == 2) {
        matrix[0] = gain(base + leftPan);
        matrix[3] = gain(base + rightPan);
        matrix[8] = gain(base + leftPan + side);
        matrix[11] = gain(base + rightPan + center);
    } else {
        matrix[0] = gain(base + leftPan);
        matrix[1] = gain(base + rightPan);
        matrix[2] = gain(base + leftPan + rightPan + middle);
        matrix[4] = gain(base + leftPan + side);
        matrix[5] = gain(base + rightPan + center);
    }
    SetRawSoundEffectOutputMatrix(g_aicaPlaybackOwners[index], matrix.data(), channels, 6);
}

void EnqueueAicaExecution(std::uint32_t command) {
    if (!g_aicaMemory || g_aicaMemory->executionQueueCount >= 0x80)
        return;
    g_aicaMemory->executionCommandRing[g_aicaMemory->executionWriteIndex] = command;
    g_aicaMemory->executionWriteIndex = (g_aicaMemory->executionWriteIndex + 1) & 0x3f;
    ++g_aicaMemory->executionQueueCount;
}

void Aica_InitializeDriverValues() {
    g_aicaMemory->executionQueueCount = 0;
    g_aicaMemory->pendingQueueCount = 0;
    g_aicaMemory->executionReadIndex = 0;
    g_aicaMemory->executionWriteIndex = 0;
    g_aicaMemory->pendingWriteIndex = 0;
    g_aicaMemory->pendingReadIndex = 0;
    g_aicaMemory->currentCommand = 0;
    g_aicaMemory->updateComplete = 1;
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    write_unaligned(raw + 0x6f8, std::uint32_t{0});
    write_unaligned(raw + 0x6fc, std::uint32_t{0});
    g_aicaMemory->field718 = 0;
    g_aicaMemory->field71c = 0;
    g_aicaMemory->reserved_0707 = std::byte{0};
    g_aicaMemory->loadedBankUnits = 0;
    g_aicaMemory->globalVolume = 127;
    g_aicaMemory->useGlobalPanOverride = 1;
    g_aicaMemory->maximumPromotionsPerTick = 64;
    g_aicaMemory->commandQueueBusy = 0;
    g_aicaMemory->field70c = 0;
    raw[0x730] = 0;
    raw[0x731] = 0xff;
    raw[0x732] = 0xff;
    g_aicaMemory->voicePool.freeCount = 160;
    g_aicaMemory->voicePool.freeHead = 0;
    g_aicaMemory->voicePool.freeTail = 159;
    g_aicaMemory->voicePool.list1Count = 0;
    g_aicaMemory->voicePool.list2Count = 0;
    g_aicaMemory->globalPanRight = 0;
    g_aicaMemory->globalPanLeft = 0;
    g_aicaMemory->voicePool.paused = 0;
    std::memset(raw + 0x2f0, 0, 0x200);
    std::memset(raw + 0x4f0, 0, 0x200);
    for (std::size_t index = 0; index < 160; ++index) {
        AicaVoiceSlot &slot = g_aicaMemory->voicePool.slots[index];
        slot.status = 0xff;
        slot.driverNoteTable = 0;
        slot.driverPointerTable = 0;
        slot.playState = 0;
        slot.reserved_0a[1] = std::byte{0};
        slot.stereo = 0;
        slot.previousSlot = index == 0 ? 0xff : static_cast<std::uint8_t>(index - 1);
        slot.nextSlot = index == 159 ? 0xff : static_cast<std::uint8_t>(index + 1);
        slot.xaudioStreamKind = -1;
    }
    for (std::size_t driver = 0; driver < 64; ++driver) {
        const std::size_t base = 0x8c0 + 64 * driver;
        raw[base] = 0;
        raw[base + 2] = 0;
        raw[base + 0x22] = 64;
        raw[base + 0x23] = 0;
        raw[base + 0x24] = 0;
        raw[base + 0x25] = 0;
    }
    for (std::size_t bank = 0; bank < 16; ++bank) {
        AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
        slot.bankIdOrType = -1;
        slot.bankData = nullptr;
        slot.loadedSize = (slot.loadedSize & 0xff0000ffU) | 0x00ffff00U;
        raw[0x750 + 24 * bank] = 0xff;
        raw[0x752 + 24 * bank] = 127;
        raw[0x754 + 24 * bank] = 64;
        raw[0x755 + 24 * bank] = 64;
        g_aicaMemory->bankRegionLoaded[bank] = 0;
        g_aicaMemory->bankRegionBase[bank] = nullptr;
        g_aicaMemory->bankRegionSize[bank] = 0;
    }
    std::memset(raw + 0x18c0, 0xff, 16 * 16 * 4);
    for (std::size_t owner = 0; owner < 16; ++owner)
        raw[0x1cc0 + 56 * owner] = 0;
}

void ResetAicaVoice(AicaVoiceSlot &voice) {
    const std::size_t index = static_cast<std::size_t>(&voice - g_aicaMemory->voicePool.slots);
    if (voice.xaudioStreamKind != -1)
        StopRawSoundEffectPlayback(g_aicaPlaybackOwners[index]);
    ReleaseRawSoundEffectReservation(g_aicaPlaybackOwners[index]);
    voice.xaudioStreamKind = -1;
    if (voice.status == 0xff)
        return;
    if (voice.status == 3) {
        AicaEnvironmentPartState &part = g_aicaEnvironmentStates[voice.bank].parts[voice.part];
        if (part.voiceIndex == index) {
            part.voiceIndex = 0xff;
            part.active = 0;
        }
    }
    const std::uint8_t list = allocationLists[index];
    voice.status = 0xff;
    voice.spawnCommand = 0;
    voice.playState = 0;
    ReturnAicaVoice(list, static_cast<std::uint8_t>(index));
    g_aicaPlaybackOwners[index] = nullptr;
}

void ReapInactiveAicaVoices() {
    for (std::size_t index = 0; index < 160; ++index) {
        AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[index];
        if (voice.status != 0xff && !RawSoundEffectVoiceIsActive(g_aicaPlaybackOwners[index])) {
            ResetAicaVoice(voice);
        }
    }
}

std::int32_t ReleaseOwnedAicaBankUnlocked(std::size_t bank) {
    if (!g_aicaMemory || bank >= 16)
        return -1;
    AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    if (slot.bankIdOrType == -1)
        return -2;
    ResetAicaDriverAssignments(static_cast<std::uint8_t>(bank), false);
    ResetAicaDriverAssignments(static_cast<std::uint8_t>(bank), true);
    g_aicaMemory->loadedBankUnits -= static_cast<std::uint8_t>(slot.loadedSize);
    slot.bankIdOrType = -1;
    slot.loadedSize = (slot.loadedSize & 0xff000000U) | 0x00ffff00U;
    if (g_aicaMemory->bankRegionBase[bank] == slot.bankData)
        g_aicaMemory->bankRegionLoaded[bank] = 0;
    return 0;
}

void PromoteQueuedAicaCommands() {
    if (!g_aicaMemory->pendingQueueCount)
        return;
    std::uint8_t remaining = g_aicaMemory->maximumPromotionsPerTick;
    do {
        std::uint32_t command = g_aicaMemory->pendingCommandRing[g_aicaMemory->pendingReadIndex];
        const std::uint8_t commandClass = static_cast<std::uint8_t>(command >> 24);

        if (commandClass == 0xa0) {
            command |= 0x40;
            if (g_aicaMemory->executionQueueCount < 0x80) {
                g_aicaMemory->executionCommandRing[g_aicaMemory->executionWriteIndex] = command;
                g_aicaMemory->executionWriteIndex = (g_aicaMemory->executionWriteIndex + 1) & 0x3f;
                ++g_aicaMemory->executionQueueCount;
            }
            AicaUpdate(command);
        } else if (commandClass >= 0xa8 && commandClass < 0xad) {
            AicaUpdate(command);
        } else if (g_aicaMemory->executionQueueCount < 0x80) {
            g_aicaMemory->executionCommandRing[g_aicaMemory->executionWriteIndex] = command;
            g_aicaMemory->executionWriteIndex = (g_aicaMemory->executionWriteIndex + 1) & 0x3f;
            ++g_aicaMemory->executionQueueCount;
        }

        --remaining;
        g_aicaMemory->pendingCommandRing[g_aicaMemory->pendingReadIndex] = 0;
        g_aicaMemory->pendingReadIndex = (g_aicaMemory->pendingReadIndex + 1) & 0x3f;
        --g_aicaMemory->pendingQueueCount;
    } while (g_aicaMemory->pendingQueueCount && remaining);
}

void ExecuteAicaInsnForCycles() {
    while (g_aicaMemory->executionQueueCount) {
        ExecuteAicaInsn(g_aicaMemory->executionCommandRing[g_aicaMemory->executionReadIndex]);
        g_aicaMemory->executionCommandRing[g_aicaMemory->executionReadIndex] = 0;
        g_aicaMemory->executionReadIndex = (g_aicaMemory->executionReadIndex + 1) & 0x3f;
        --g_aicaMemory->executionQueueCount;
    }
}

std::int64_t ExecuteAicaCommand(std::int32_t bank, std::uint32_t command, std::int16_t volume) {
    std::uint32_t packed = command + (bank & 0x1f);
    if ((command & 0x00ff0000u) == 0) {
        return SNDFunc(ByteSwap32(packed));
    }

    const auto directVolume = [&] {
        packed += (static_cast<std::uint32_t>(volume) & 0x7f) << 8;
        return SNDFunc(ByteSwap32(packed));
    };
    const auto attenuatedVolume = [&] {
        packed += (static_cast<std::uint32_t>(static_cast<std::int32_t>(volume) * 0x100) - 0x4000) & 0x7f00;
        return SNDFunc(ByteSwap32(packed));
    };

    if (command <= 0xa00b0000u) {
        if (command == 0xa00b0000u || ((command + 0x5ffe0000u) & 0xfffeffffu) == 0) {
            return SNDFunc(ByteSwap32(packed));
        }
        if (command == 0xa0010000u || command == 0xa0040000u || ((command + 0x5ff70000u) & 0xfffeffffu) == 0) {
            return directVolume();
        }
        if (command == 0xa0050000u || command == 0xa0070000u) {
            return attenuatedVolume();
        }
        return -2;
    }

    if (command <= 0xa0190000u) {
        if (command == 0xa00c0000u) {
            return SNDFunc(ByteSwap32(packed));
        }
        if (command == 0xa0100000u || command == 0xa0190000u) {
            return directVolume();
        }
        if (command == 0xa0110000u) {
            return attenuatedVolume();
        }
        return -2;
    }
    if (command == 0xa01c0000u) {
        return directVolume();
    }
    if (((command + 0x5fd80000u) & 0xfffeffffu) == 0) {
        packed += (static_cast<std::uint32_t>(static_cast<std::int32_t>(volume) * 0x100) - 0x100) & 0x0f00;
        return SNDFunc(ByteSwap32(packed));
    }
    return -2;
}

std::int64_t ExecuteEnvironmentAicaCommand(std::int32_t bank, std::uint32_t type, std::int32_t volume) {
    const std::uint32_t bankBits = (bank & 0xf) << 16;
    const auto attenuated = [&] {
        const std::uint32_t volumeBits = (static_cast<std::uint32_t>(volume * 0x100) - 0x4000) & 0x7f00;
        return SNDFunc(ByteSwap32(type + bankBits + volumeBits));
    };

    if (type == 0xa5000000u) {
        std::int64_t result = SNDFunc(ByteSwap32(((bank & 0xf) + 0xa570u) << 16 | (volume & 0x7f00)));
        if (result == 0) {
            result = SNDFunc(ByteSwap32((((bank & 0xf) + 0xa500u) << 16) + ((static_cast<std::uint32_t>(volume) << 8) & 0x7f00)));
        }
        return result;
    }
    if (type == 0xa5100000u) {
        return SNDFunc(ByteSwap32(type + bankBits + ((static_cast<std::uint32_t>(volume) << 8) & 0x7f00)));
    }
    if (type == 0xa5500000u || type == 0xa5600000u || ((type + 0x5ae00000u) & 0xffcfffffu) == 0 || ((type + 0x59000000u) & 0xffefffffu) == 0) {
        return attenuated();
    }
    if (type == 0xa6000000u) {
        return SNDFunc(
            ByteSwap32(type + bankBits + ((static_cast<std::uint32_t>(volume) << 8) & 0x7f00) + ((static_cast<std::uint32_t>(volume) << 13) & 0x100000)));
    }
    return -2;
}

std::int64_t ExecuteAicaCommand2Ch(std::int32_t bank, std::int32_t channel, std::uint32_t command, std::int16_t volume) {
    std::uint32_t packed = command + (((channel - 1) & 0xf) << 24) + (bank & 0x1f);
    if (command == 0 || command == 0xb0780000u) {
        return SNDFunc(ByteSwap32(packed));
    }

    if (command == 0xb0010000u || command == 0xb00e0000u || command == 0xb0070000u || command == 0xb00d0000u || command == 0xb0270000u ||
        ((command + 0x4fd30000u) & 0xfffeffffu) == 0) {
        packed += (static_cast<std::uint32_t>(volume) << 8) & 0x7f00;
    } else if (command == 0xa0050000u || command == 0xa0110000u || command == 0xb00a0000u || command == 0xb02a0000u) {
        packed += (static_cast<std::uint32_t>(static_cast<std::int32_t>(volume) * 0x100) - 0x4000) & 0x7f00;
    } else {
        return -2;
    }
    return SNDFunc(ByteSwap32(packed));
}

void ExecuteAicaInsn(std::uint32_t command) {
    g_aicaMemory->currentCommand = command;
    const std::uint8_t commandClass = static_cast<std::uint8_t>(command >> 24);
    switch (commandClass & 0xf0) {
    case 0x80:
        ExecuteAicaCommand_1(0x8000, 0x0f7f00ff);
        break;
    case 0x90:
        if ((command & 0xff00) != 0) {
            ExecuteAicaCommand_0(0x9000);
        } else {
            ExecuteAicaCommand_1(0x8000, 0x0f7f00ff);
        }
        break;
    case 0xa0:
        ExecuteAicaBankA();
        break;
    case 0xb0:
        ExecuteAicaBankB();
        break;
    case 0xc0:
        ExecuteAicaBankC();
        break;
    case 0xe0:
        ExecuteAicaCommand_0(0xe000);
        ExecuteAicaCommand_1(0xe000, 0x0f00000f);
        break;
    default:
        break;
    }
}

void ExecuteAicaBankB() {
    const std::uint8_t operation = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 16) & 0x7f);
    switch (operation) {
    case 0x00:
        ExecuteAicaCommand_2();
        break;
    case 0x01:
        ExecuteAicaCommand_0(0xb001);
        break;
    case 0x07:
        ExecuteAicaCommand_0(0xb007);
        ExecuteAicaCommand_1(0xb007, 0x0f00000f);
        break;
    case 0x0a:
        ExecuteAicaCommand_0(0xb00a);
        ExecuteAicaCommand_1(0xb00a, 0x0f00000f);
        break;
    case 0x0d:
        ExecuteAicaCommand_0(0xb00d);
        ExecuteAicaCommand_1(0xb00d, 0x0f00000f);
        break;
    case 0x0e:
        ExecuteAicaCommand_0(0xb00e);
        ExecuteAicaCommand_1(0xb00e, 0x0f00000f);
        break;
    case 0x27:
        ExecuteAicaCommand_0(0xb007);
        break;
    case 0x2a:
        ExecuteAicaCommand_0(0xb00a);
        break;
    case 0x2d:
        ExecuteAicaCommand_0(0xb00d);
        break;
    case 0x2e:
        ExecuteAicaCommand_0(0xb00e);
        break;
    case 0x40:
        ExecuteAicaCommand_0(0xb040);
        ExecuteAicaCommand_1(0xb040, 0x0f00000f);
        break;
    case 0x78:
        ExecuteAicaCommand_1(0xb078, 0x0f);
        break;
    default:
        break;
    }
}

void ResolveAicaDriverPointer(std::uint8_t bank, std::uint8_t voiceIndex) {
    if (!g_aicaMemory || bank >= std::size(g_aicaMemory->bankSlots) || voiceIndex >= std::size(g_aicaMemory->voicePool.slots)) {
        return;
    }

    auto *const memory = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::size_t voiceBase = 64U * voiceIndex;
    const auto publish = [&](std::uint8_t *driver) { write_unaligned(memory + 0x8f0 + voiceBase, driver); };
    publish(nullptr);

    const AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!g_aicaMemory->bankRegionLoaded[bank] || !bankData || bankSize < 0x34) {
        return;
    }

    std::uint8_t *driver = nullptr;
    if ((memory[0x8c1 + voiceBase] & 2U) != 0) {
        const std::size_t offset = read_unaligned<std::uint32_t>(bankData + (modernBanks[bank] ? 0xa8 : 0x30));
        if (offset != 0)
            driver = AicaBankOffset(bankData, bankSize, offset, 16);
    } else {
        const std::size_t partTableOffset = read_unaligned<std::uint32_t>(bankData + (modernBanks[bank] ? 0x9c : 0x24));
        const std::size_t partOffset = 2U * (static_cast<std::size_t>(memory[0x8c4 + voiceBase]) + 1U);
        std::uint8_t *const partOffsetEntry = AicaBankOffset(bankData, bankSize, partTableOffset + partOffset, 2);
        if (!partOffsetEntry)
            return;

        const std::size_t layerTableOffset = partTableOffset + read_unaligned<std::uint16_t>(partOffsetEntry);
        const std::size_t layerOffset = 2U * (static_cast<std::size_t>(memory[0x8c5 + voiceBase]) + 1U);
        std::uint8_t *const layerOffsetEntry = AicaBankOffset(bankData, bankSize, layerTableOffset + layerOffset, 2);
        if (!layerOffsetEntry)
            return;

        const std::size_t driverOffset = read_unaligned<std::uint16_t>(layerOffsetEntry);
        if (driverOffset != 0) {
            driver = AicaBankOffset(bankData, bankSize, layerTableOffset + driverOffset, 16);
        }
    }
    publish(driver);
}

void ExecuteAicaBankC() {
    const std::uint8_t bank = static_cast<std::uint8_t>(g_aicaMemory->currentCommand & 0x0f);
    const std::uint8_t channel = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 24) & 0x0f);
    const std::size_t assignmentBase = 4 * (channel + 16 * (static_cast<std::uint32_t>(bank) + 0x63));
    auto *const memory = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::uint8_t selectedVoice = memory[assignmentBase];
    if (selectedVoice == 0xff || selectedVoice >= std::size(g_aicaMemory->voicePool.slots)) {
        return;
    }

    const std::size_t voiceBase = 64 * selectedVoice;
    if ((memory[0x8c1 + voiceBase] & 2) != 0)
        return;

    AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!g_aicaMemory->bankRegionLoaded[bank] || !bankData || bankSize < 0x28) {
        return;
    }
    const std::size_t partTableOffset = read_unaligned<std::uint32_t>(bankData + (modernBanks[bank] ? 0x9c : 0x24));
    const std::size_t partOffset = 2U * (static_cast<std::size_t>(memory[0x8c4 + voiceBase]) + 1U);
    std::uint8_t *const partOffsetEntry = AicaBankOffset(bankData, bankSize, partTableOffset + partOffset, 2);
    if (!partOffsetEntry)
        return;
    const std::size_t layerTableOffset = partTableOffset + read_unaligned<std::uint16_t>(partOffsetEntry);
    const std::uint8_t *const layerTable = AicaBankOffset(bankData, bankSize, layerTableOffset, 2);
    if (!layerTable)
        return;
    const std::uint16_t maximumLayer = read_unaligned<std::uint16_t>(layerTable);
    const std::uint16_t requestedLayer = static_cast<std::uint16_t>((g_aicaMemory->currentCommand >> 16) & 0x7f);
    if (maximumLayer < requestedLayer)
        return;

    ExecuteAicaCommand_0(0xc000);
    for (std::size_t candidate = 0; candidate < 4; ++candidate) {
        const std::uint8_t voiceIndex = memory[assignmentBase + candidate];
        if (voiceIndex != 0xff && voiceIndex < std::size(g_aicaMemory->voicePool.slots)) {
            ResolveAicaDriverPointer(bank, voiceIndex);
        }
    }
}

std::int64_t ExecuteAicaCommand_2() {
    const std::uint32_t bank = g_aicaMemory->currentCommand & 0x0f;
    const std::uint32_t channel = (g_aicaMemory->currentCommand >> 24) & 0x0f;
    const std::size_t assignmentOffset = 4 * (channel + 16 * (bank + 0x63));
    auto *const memory = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::uint8_t voiceIndex = memory[assignmentOffset];
    if (voiceIndex == 0xff || voiceIndex >= std::size(g_aicaMemory->voicePool.slots) || (memory[0x8c1 + 64 * voiceIndex] & 2) != 0) {
        return reinterpret_cast<std::intptr_t>(g_aicaMemory);
    }

    const AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    const std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!g_aicaMemory->bankRegionLoaded[bank] || !bankData || bankSize < 0x28) {
        return reinterpret_cast<std::intptr_t>(g_aicaMemory);
    }
    const std::size_t tableOffset = read_unaligned<std::uint32_t>(bankData + (modernBanks[bank] ? 0x9c : 0x24));
    const std::uint8_t *const table = AicaBankOffset(slot.bankData, bankSize, tableOffset, 2);
    if (!table)
        return reinterpret_cast<std::intptr_t>(g_aicaMemory);
    const std::uint16_t maximumOperation = read_unaligned<std::uint16_t>(table);
    const std::uint32_t operation = (g_aicaMemory->currentCommand >> 8) & 0x7f;
    if (maximumOperation >= operation)
        return ExecuteAicaCommand_0(0xb000);
    return reinterpret_cast<std::intptr_t>(g_aicaMemory);
}

std::int64_t ExecuteAicaCommand_0(std::uint16_t command) {
    const std::uint32_t bank = g_aicaMemory->currentCommand & 0x0f;
    const std::uint32_t channel = (g_aicaMemory->currentCommand >> 24) & 0x0f;
    const std::uint8_t volume = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 8) & 0x7f);
    const std::uint8_t secondaryValue = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 16) & 0x7f);
    const std::size_t assignmentBase = 4 * (channel + 16 * (bank + 0x63));
    auto *const memory = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    std::int64_t result = reinterpret_cast<std::intptr_t>(g_aicaMemory);

    for (std::size_t candidate = 0; candidate < 4; ++candidate) {
        const std::uint8_t voiceIndex = memory[assignmentBase + candidate];
        if (voiceIndex == 0xff) {
            continue;
        }
        g_aicaMemory->ownerId = static_cast<std::uint8_t>(bank);
        const std::size_t voiceBase = 64 * voiceIndex;
        switch (command) {
        case 0x9000:
            if (static_cast<std::int8_t>(memory[0x8c1 + voiceBase]) < 0) {
                std::uint8_t *const driver = read_unaligned<std::uint8_t *>(memory + 0x8f0 + voiceBase);
                if (driver) {
                    for (std::size_t offset = 8; offset <= 14; offset += 2) {
                        result = LoadAICADriver(reinterpret_cast<std::uint16_t *>(driver + offset), voiceIndex);
                    }
                }
            }
            break;
        case 0xb000:
            memory[0x8c4 + voiceBase] = volume;
            break;
        case 0xb001:
            memory[0x8f9 + voiceBase] = volume != 0;
            break;
        case 0xb007:
            memory[0x8cc + voiceBase] = volume;
            break;
        case 0xb00a: {
            const std::uint8_t quarterVolume = volume >> 2;
            memory[0x8cd + voiceBase] = quarterVolume < 16 ? static_cast<std::uint8_t>(31 - quarterVolume) : static_cast<std::uint8_t>(quarterVolume - 16);
            break;
        }
        case 0xb00d:
            memory[0x8d7 + voiceBase] = volume;
            break;
        case 0xb00e:
            memory[0x8d9 + voiceBase] = volume;
            break;
        case 0xb040:
            memory[0x8e3 + voiceBase] = volume < 0x40 ? 0 : volume;
            break;
        case 0xc000:
            memory[0x8c5 + voiceBase] = secondaryValue;
            break;
        case 0xe000:
            memory[0x8e2 + voiceBase] = volume;
            break;
        default:
            break;
        }
    }
    return result;
}

void ExecuteAicaSequenceStopCommand(std::uint32_t command) {
    if ((command & 0xff00U) > 0x1900U)
        return;
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::uint8_t bank = static_cast<std::uint8_t>(command & 15U);
    const bool qualified = (command & 0x80U) != 0;
    for (std::size_t index = 0; index < 16; ++index) {
        const std::size_t base = 0x1cbc + 56 * index;
        const bool negative = static_cast<std::int8_t>(raw[base + 5]) < 0;
        const bool sameBank = raw[base + 6] == bank;
        bool cancel = false;
        switch (command & 0xffffff00U) {
        case 0xa0000100U:
            cancel = !qualified || !sameBank || negative;
            break;
        case 0xa0000200U:
            cancel = !negative && (!qualified || !sameBank);
            break;
        case 0xa0000300U:
            cancel = negative;
            break;
        case 0xa0001100U:
            cancel = sameBank && (!qualified || negative);
            break;
        case 0xa0001200U:
            cancel = !qualified && sameBank && !negative;
            break;
        case 0xa0001300U:
            cancel = sameBank && negative;
            break;
        }
        if (cancel)
            raw[base + 4] = 0;
    }
}

void BeginAicaSequenceFadeOut(std::uint16_t command) {
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    for (std::size_t index = 0; index < 16; ++index) {
        const std::size_t base = 0x1cbc + 56 * index;
        if (raw[base + 4] && static_cast<std::int8_t>(raw[base + 5]) < 0 && raw[base + 6] == (command & 15U)) {
            raw[base + 4] |= 6U;
            write_unaligned(raw + base + 52, -static_cast<std::int32_t>(kAicaFadeTable[(command >> 8) & 127U]));
        }
    }
}

void AicaUpdate(std::uint32_t command) {
    if (!g_aicaMemory)
        return;
    g_aicaMemory->currentCommand = command;
    auto *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    if ((command & 0xff000000U) == 0xa0000000U) {
        const std::uint32_t subcommand = command & 0x00ff0000U;
        const std::uint8_t bank = static_cast<std::uint8_t>(command & 0x0f);
        if (subcommand == 0) {
            ExecuteAicaSequenceStopCommand(command);
        } else if (subcommand == 0x00020000U || subcommand == 0x00030000U) {
            for (std::size_t index = 0; index < 16; ++index) {
                const std::size_t base = 0x1cbc + 56 * index;
                if (!raw[base + 4] || raw[base + 6] != bank || static_cast<std::int8_t>(raw[base + 5]) < 0)
                    continue;
                if (subcommand == 0x00020000U)
                    raw[base + 4] |= 1U;
                else
                    raw[base + 4] &= static_cast<std::uint8_t>(~1U);
            }
        } else if (subcommand == 0x00090000U) {
            raw[0x700] = bank;
            write_unaligned(raw + 0x6f8, static_cast<std::uint32_t>(kAicaFadeTable[(command >> 8) & 0x7f]));
        } else if (subcommand == 0x000a0000U) {
            BeginAicaSequenceFadeOut(static_cast<std::uint16_t>(command));
        }
        g_aicaMemory->updateComplete = 1;
        return;
    }

    const std::uint16_t commandId = static_cast<std::uint16_t>(command >> 16);
    const std::uint8_t eventIndex = static_cast<std::uint8_t>(command >> 8);
    for (std::size_t bank = 0; bank < 16; ++bank) {
        AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
        const std::size_t size = g_aicaMemory->bankRegionSize[bank];
        if (!g_aicaMemory->bankRegionLoaded[bank] || !slot.bankData || size < 0x30)
            continue;
        auto *const bankData = slot.bankData;
        const std::size_t tableOffset = read_unaligned<std::uint32_t>(bankData + (modernBanks[bank] ? 0xa4 : 0x2c));
        auto *const table = AicaBankOffset(bankData, size, tableOffset, 4);
        if (!table)
            continue;
        const std::size_t entryCount = static_cast<std::size_t>(read_unaligned<std::uint16_t>(table)) + 1;
        if (entryCount > (size - tableOffset - 4) / 4)
            continue;
        const std::uint8_t *event = nullptr;
        for (std::size_t index = 0; index < entryCount; ++index) {
            const auto *const entry = table + 4 + 4 * index;
            if (read_unaligned<std::uint16_t>(entry + 2) != commandId)
                continue;
            const std::size_t eventOffset = read_unaligned<std::uint16_t>(entry);
            event = AicaBankOffset(bankData, size, tableOffset + eventOffset, 1);
            break;
        }
        if (!event || eventIndex > event[0])
            continue;
        const std::size_t eventOffset = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(event) - reinterpret_cast<std::uintptr_t>(bankData));
        const std::size_t offsetField = eventOffset + 4 + 4 * eventIndex;
        if (!AicaBankOffset(bankData, size, offsetField, 4))
            continue;
        const std::size_t driverOffset = read_unaligned<std::uint32_t>(bankData + offsetField);
        auto *const driver = AicaBankOffset(bankData, size, tableOffset + driverOffset, 1);
        if (!driver)
            continue;

        std::size_t selected = 16;
        const std::int8_t driverKind = static_cast<std::int8_t>(driver[0]);
        if (driverKind < 0 && (driverKind & 0x10) != 0) {
            const std::uint8_t key = static_cast<std::uint8_t>(driverKind) & 0x0f;
            for (std::size_t index = 0; index < 16; ++index) {
                const std::size_t base = 0x1cbc + 56 * index;
                if (raw[base + 4] && (raw[base + 5] & 0x0fU) == key) {
                    selected = index;
                    break;
                }
            }
        } else if (driverKind >= 0) {
            for (std::size_t index = 0; index < 16; ++index) {
                const std::size_t base = 0x1cbc + 56 * index;
                if (raw[base + 4] && raw[base + 6] == bank && static_cast<std::int8_t>(raw[base + 5]) >= 0) {
                    selected = index;
                    break;
                }
            }
        }
        if (selected == 16) {
            for (std::size_t index = 0; index < 16; ++index) {
                if (!raw[0x1cbc + 56 * index + 4]) {
                    selected = index;
                    break;
                }
            }
        }
        if (selected == 16) {
            for (std::size_t index = 0; index < 16; ++index) {
                if (static_cast<std::int8_t>(raw[0x1cbc + 56 * index + 5]) < 0) {
                    selected = index;
                    break;
                }
            }
        }
        if (selected == 16)
            continue;

        const std::size_t base = 0x1cbc + 56 * selected;
        raw[base + 4] = driverKind < 0 ? 0x80 : 0x90;
        raw[base + 5] = driver[0];
        raw[base + 6] = static_cast<std::uint8_t>(bank);
        write_unaligned(raw + base + 8, std::uint32_t{0x10000});
        write_unaligned(raw + base + 12, command);
        write_unaligned(raw + base + 16, std::uint32_t{0});
        if (driverKind < 0) {
            write_unaligned(raw + base + 28, reinterpret_cast<std::uintptr_t>(kAicaEndStream.data()));
            write_unaligned(raw + base + 36, reinterpret_cast<std::uintptr_t>(driver + 1));
        } else {
            write_unaligned(raw + base + 20, reinterpret_cast<std::uintptr_t>(driver));
            write_unaligned(raw + base + 28, reinterpret_cast<std::uintptr_t>(driver + 4));
            write_unaligned(raw + base + 36, reinterpret_cast<std::uintptr_t>(kAicaEndStream.data()));
            if (raw[0x700] == bank && read_unaligned<std::uint32_t>(raw + 0x6f8)) {
                write_unaligned(raw + base + 52, read_unaligned<std::int32_t>(raw + 0x6f8));
                write_unaligned(raw + base + 44, std::uint32_t{0});
                write_unaligned(raw + base + 48, std::uint32_t{0});
                raw[base + 4] |= 2U;
                write_unaligned(raw + 0x6f8, std::uint32_t{0});
            } else {
                write_unaligned(raw + base + 44, std::uint32_t{0x7f0000});
                write_unaligned(raw + base + 48, std::uint32_t{0x7f0000});
            }
        }
        ProcessAicaEventStreams();
        break;
    }
    g_aicaMemory->updateComplete = 1;
}

void ProcessAicaEventStreams() {
    if (!g_aicaMemory)
        return;
    auto *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    for (std::size_t owner = 0; owner < 16; ++owner) {
        const std::size_t base = 0x1cbc + 56 * owner;
        if (!raw[base + 4])
            continue;
        const std::uint32_t bank = raw[base + 6];
        if (bank >= 16 || !g_aicaMemory->bankRegionLoaded[bank] || !g_aicaMemory->bankSlots[bank].bankData) {
            raw[base + 4] = 0;
            continue;
        }
        auto *const bankData = g_aicaMemory->bankSlots[bank].bankData;
        const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
        const auto clearOwner = [&] { raw[base + 4] = 0; };

        if ((raw[base + 4] & 2U) != 0) {
            std::int32_t current = read_unaligned<std::int32_t>(raw + base + 44);
            const std::int32_t step = read_unaligned<std::int32_t>(raw + base + 52);
            current += step;
            if (current < 0)
                current = 0;
            if (current > 0x7f0000)
                current = 0x7f0000;
            write_unaligned(raw + base + 44, current);
        }
        if ((raw[base + 4] & 2U) != 0 && read_unaligned<std::int32_t>(raw + base + 44) != read_unaligned<std::int32_t>(raw + base + 48)) {
            const std::uint32_t current = read_unaligned<std::uint32_t>(raw + base + 44);
            write_unaligned(raw + base + 48, current);
            EnqueueAicaExecution(0xa01c0000U | ((current >> 8U) & 0x7f00U) | bank);
            if (current == 0x7f0000U) {
                raw[base + 4] &= 0xf9U;
            } else if (current == 0) {
                EnqueueAicaExecution(0xa0001200U | bank);
                raw[base + 4] = 0;
            }
        }

        const std::int32_t delay = read_unaligned<std::int32_t>(raw + base + 16);
        if (delay >= 0 && (static_cast<std::uint32_t>(delay) & 0xffff0000U) != 0)
            continue;

        const auto pointerAt = [&](std::size_t offset) { return reinterpret_cast<std::uint8_t *>(read_unaligned<std::uintptr_t>(raw + base + offset)); };
        auto *stream = pointerAt(36);
        auto *control = pointerAt(28);
        bool ownerFinished = false;
        while (!ownerFinished) {
            if (!stream || stream == kAicaEndStream.data()) {
                stream = nullptr;
            } else if (!AicaBankPointerValid(bankData, bankSize, stream, 1)) {
                clearOwner();
                ownerFinished = true;
                break;
            }
            if (!stream) {
                control = pointerAt(28);
                if (control == kAicaEndStream.data()) {
                    clearOwner();
                    break;
                }
                if (!AicaBankPointerValid(bankData, bankSize, control, 4)) {
                    clearOwner();
                    break;
                }
                std::uint8_t type = control[3];
                std::uint32_t offset = read_unaligned<std::uint32_t>(control) & 0x00ffffffU;
                stream = control + 4;
                if (type == 0) {
                    write_unaligned(raw + base + 28, reinterpret_cast<std::uintptr_t>(stream));
                    const std::uintptr_t driverOffset = reinterpret_cast<std::uintptr_t>(pointerAt(20)) - reinterpret_cast<std::uintptr_t>(bankData);
                    stream = AicaBankOffset(bankData, bankSize, driverOffset + offset, 1);
                    if (!stream) {
                        clearOwner();
                        ownerFinished = true;
                        break;
                    }
                    continue;
                }
                for (;;) {
                    if (type == 128) {
                        EnqueueAicaExecution(0x00000080U | (offset << 8) | (static_cast<std::uint32_t>(bank) & 0x0fU));
                        if ((raw[base + 4] & 0x18U) != 0) {
                            std::uint32_t fadeCommand = 0xa01c0000U;
                            if ((raw[base + 4] & 8U) != 0) {
                                raw[base + 4] &= 0xf5U;
                            } else {
                                raw[base + 4] &= static_cast<std::uint8_t>(0xefU);
                                fadeCommand = 0xa01c7f00U;
                            }
                            EnqueueAicaExecution(fadeCommand | bank);
                        }
                        control += 4;
                    } else {
                        const std::uint8_t *const driverBase = pointerAt(20);
                        const std::size_t driverOffset =
                            static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(driverBase) - reinterpret_cast<std::uintptr_t>(bankData));
                        std::uint8_t *const target = AicaBankOffset(bankData, bankSize, driverOffset + offset, 1);
                        if (!target) {
                            clearOwner();
                            ownerFinished = true;
                            break;
                        }
                        if (type != 144) {
                            if (!offset) {
                                clearOwner();
                                ownerFinished = true;
                                break;
                            }
                            write_unaligned(raw + base + 28, reinterpret_cast<std::uintptr_t>(kAicaEndStream.data()));
                            stream = target;
                            break;
                        }
                        control = target;
                    }
                    if (!AicaBankPointerValid(bankData, bankSize, control, 4)) {
                        clearOwner();
                        ownerFinished = true;
                        break;
                    }
                    type = control[3];
                    offset = read_unaligned<std::uint32_t>(control) & 0x00ffffffU;
                    stream = control + 4;
                    if (type == 0) {
                        write_unaligned(raw + base + 28, reinterpret_cast<std::uintptr_t>(stream));
                        const std::uintptr_t driverOffset = reinterpret_cast<std::uintptr_t>(pointerAt(20)) - reinterpret_cast<std::uintptr_t>(bankData);
                        stream = AicaBankOffset(bankData, bankSize, driverOffset + offset, 1);
                        if (!stream) {
                            clearOwner();
                            ownerFinished = true;
                        }
                        break;
                    }
                }
            }
            if (ownerFinished || !stream)
                continue;
            const std::uint8_t first = *stream;
            if (first == 0xff) {
                stream = nullptr;
                continue;
            }
            auto *cursor = stream;
            std::uint8_t status = raw[base + 7];
            if (static_cast<std::int8_t>(*cursor) < 0) {
                status = *cursor++;
                raw[base + 7] = status;
            }
            if (!AicaBankPointerValid(bankData, bankSize, cursor, 2)) {
                clearOwner();
                break;
            }
            const std::uint8_t data = *cursor++;
            std::uint32_t encoded = ((static_cast<std::uint32_t>(data & 0x7fU) | (static_cast<std::uint32_t>(status) << 8)) << 16);
            bool continuation = (data & 0x80U) != 0;
            switch (status & 0xf0U) {
            case 0x90:
            case 0xa0:
            case 0xb0:
            case 0xf0:
                if (!AicaBankPointerValid(bankData, bankSize, cursor, 1)) {
                    clearOwner();
                    ownerFinished = true;
                    break;
                }
                continuation = (*cursor & 0x80U) != 0;
                encoded |= static_cast<std::uint32_t>(*cursor++ & 0x7fU) << 8;
                break;
            case 0xd0:
                if (static_cast<std::int8_t>(raw[base + 5]) < 0) {
                    if (!AicaBankPointerValid(bankData, bankSize, cursor, 2)) {
                        clearOwner();
                        ownerFinished = true;
                        break;
                    }
                    continuation = (cursor[1] & 0x80U) != 0;
                    encoded =
                        (static_cast<std::uint32_t>(cursor[0] & 0x7fU) << 8) | (((static_cast<std::uint32_t>(cursor[1] & 0x7fU) << 4) | encoded) & 0xbfffffffU);
                    cursor += 2;
                }
                break;
            case 0xe0:
                encoded = (static_cast<std::uint32_t>(data & 0x7fU) << 8) | (encoded & 0xff00ffffU);
                break;
            default:
                break;
            }
            if (ownerFinished)
                break;
            EnqueueAicaExecution(encoded | bank);
            if (continuation) {
                stream = cursor;
                continue;
            }
            if (!AicaBankPointerValid(bankData, bankSize, cursor, 1)) {
                clearOwner();
                break;
            }
            std::uint32_t wait = *cursor++;
            if ((wait & 0x80U) != 0) {
                if (!AicaBankPointerValid(bankData, bankSize, cursor, 1)) {
                    clearOwner();
                    break;
                }
                wait = ((wait & 0x7fU) << 7) | (*cursor++ & 0x7fU);
            }
            const std::uint32_t oldDelay = read_unaligned<std::uint32_t>(raw + base + 16);
            write_unaligned(raw + base + 16, oldDelay + (wait << 16));
            write_unaligned(raw + base + 36, reinterpret_cast<std::uintptr_t>(cursor));
            break;
        }
        if (!ownerFinished) {
            const std::uint8_t currentStatus = raw[base + 4];
            if (currentStatus && !stream)
                clearOwner();
        }
    }
}

void ExecuteAicaCommand_1(std::uint16_t command, std::int32_t mask) {
    if (!g_aicaMemory)
        return;
    auto *const memory = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    for (AicaVoiceSlot &voice : g_aicaMemory->voicePool.slots) {
        if (voice.status == 0xff || voice.status == 3 ||
            (voice.spawnCommand & static_cast<std::uint32_t>(mask)) != (g_aicaMemory->currentCommand & static_cast<std::uint32_t>(mask))) {
            continue;
        }
        g_aicaMemory->ownerId = static_cast<std::uint8_t>(g_aicaMemory->currentCommand & 0x0fU);
        const std::size_t playbackIndex = static_cast<std::size_t>(&voice - g_aicaMemory->voicePool.slots);
        const std::uint8_t config = static_cast<std::uint8_t>(voice.ownerId);
        switch (command) {
        case 0x8000:
            if (voice.playState == 1 && (voice.driverFlags & 8U) == 0) {
                if ((voice.driverFlags & 1U) != 0) {
                    voice.driverFlags |= 2U;
                } else {
                    StopRawSoundEffectPlayback(g_aicaPlaybackOwners[playbackIndex]);
                    voice.xaudioStreamKind = -1;
                    voice.playState = 2;
                }
            }
            break;
        case 0xa002:
            if (voice.status == 1) {
                SetRawSoundEffectPaused(g_aicaPlaybackOwners[playbackIndex], true);
            }
            break;
        case 0xa00b:
            if (voice.status == 2) {
                SetRawSoundEffectPaused(g_aicaPlaybackOwners[playbackIndex], true);
            }
            break;
        case 0xa003:
            if (voice.status == 1) {
                SetRawSoundEffectPaused(g_aicaPlaybackOwners[playbackIndex], false);
            }
            break;
        case 0xa00c:
            if (voice.status == 2) {
                SetRawSoundEffectPaused(g_aicaPlaybackOwners[playbackIndex], false);
            }
            break;
        case 0xa001:
        case 0xa004:
        case 0xa005:
        case 0xa010:
        case 0xa011:
        case 0xa01c:
        case 0xb007:
            UpdateAicaPlaybackVolume(playbackIndex);
            break;
        case 0xa007:
            UpdateAicaPlaybackPitch(playbackIndex);
            break;
        case 0xb00a: {
            const auto *const note = reinterpret_cast<const std::uint8_t *>(voice.driverNoteTable);
            const std::uint8_t *const bankData = voice.bank < 16 ? g_aicaMemory->bankSlots[voice.bank].bankData : nullptr;
            const std::size_t bankSize = voice.bank < 16 ? g_aicaMemory->bankRegionSize[voice.bank] : 0;
            if (AicaBankPointerValid(bankData, bankSize, note, 7) && static_cast<std::int8_t>(note[6]) < 0) {
                ApplyVoicePanMatrixToXAudio(playbackIndex);
            }
            break;
        }
        case 0xb00d:
        case 0xb00e: {
            const auto *const note = reinterpret_cast<const std::uint8_t *>(voice.driverNoteTable);
            const std::uint8_t *const bankData = voice.bank < 16 ? g_aicaMemory->bankSlots[voice.bank].bankData : nullptr;
            const std::size_t bankSize = voice.bank < 16 ? g_aicaMemory->bankRegionSize[voice.bank] : 0;
            if (AicaBankPointerValid(bankData, bankSize, note, 46) && static_cast<std::int8_t>(note[45]) < 0) {
                ApplyVoicePanMatrixToXAudio(playbackIndex);
            }
            break;
        }
        case 0xb040:
            voice.driverFlags &= static_cast<std::uint8_t>(~1U);
            if (config < 64 && memory[0x8e3 + 64U * config]) {
                voice.driverFlags |= 1U;
            } else if ((voice.driverFlags & 2U) != 0 && voice.playState == 1) {
                StopRawSoundEffectPlayback(g_aicaPlaybackOwners[playbackIndex]);
                voice.xaudioStreamKind = -1;
                voice.playState = 2;
            }
            break;
        case 0xb078:
            if ((voice.spawnCommand & 0x0f000000U) == (g_aicaMemory->currentCommand & 0x0f000000U)) {
                StopRawSoundEffectPlayback(g_aicaPlaybackOwners[playbackIndex]);
            }
            break;
        case 0xe000:
            if (voice.playState == 1)
                UpdateAicaPlaybackPitch(playbackIndex);
            break;
        default:
            break;
        }
    }
}

void StopAicaVoicesForCommand(std::uint32_t command) {
    const std::uint8_t bank = static_cast<std::uint8_t>(g_aicaMemory->currentCommand & 15U);
    StopReleasedAicaEnvelopesForOwner(bank);
    for (std::uint32_t index = 0; index < 160; ++index) {
        AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[index];
        const bool sameBank = (voice.spawnCommand & 15U) == bank;
        bool selected = false;
        switch (command) {
        case 0xa0000100U:
            selected = true;
            break;
        case 0xa0000200U:
            selected = voice.status == 1;
            break;
        case 0xa0000300U:
            selected = voice.status == 2;
            break;
        case 0xa0001100U:
            selected = sameBank;
            break;
        case 0xa0001200U:
            selected = sameBank && voice.status == 1;
            break;
        case 0xa0001300U:
            selected = sameBank && voice.status == 2;
            break;
        }
        if (!selected)
            continue;
        if (voice.xaudioStreamKind != -1) {
            StopRawSoundEffectPlayback(GetRawSoundEffectVoice(voice.xaudioVoiceId));
            voice.xaudioStreamKind = -1;
        }
        if (voice.status < 4)
            ResetAicaVoice(voice);
    }
    if (command == 0xa0000100U) {
        for (std::uint8_t loadedBank = 0; loadedBank < 16; ++loadedBank) {
            if (g_aicaMemory->bankSlots[loadedBank].bankIdOrType != -1)
                ExecuteAicaCommand_3(1, loadedBank);
        }
    } else if (command == 0xa0001100U) {
        ExecuteAicaCommand_3(0x11, bank);
    }
    if ((g_aicaMemory->currentCommand & 0x40U) == 0)
        AicaUpdate(g_aicaMemory->currentCommand);
}

void EmitAudioForAica() {
    const std::uint8_t operation = static_cast<std::uint8_t>((g_aicaMemory->currentCommand >> 8) & 127U);
    switch (operation) {
    case 1:
        StopAicaVoicesForCommand(0xa0000100U);
        [[fallthrough]];
    case 4:
        for (AicaEnvironmentState &state : g_aicaEnvironmentStates) {
            for (const AicaEnvironmentPartState &part : state.parts) {
                if (part.active == 1 && part.voiceIndex != 0xff)
                    ReleaseAicaVoice(part.voiceIndex, true);
            }
            state.currentValue = 0;
        }
        return;
    case 2:
    case 3:
    case 0x11:
    case 0x12:
    case 0x13:
        StopAicaVoicesForCommand(0xa0000000U | (static_cast<std::uint32_t>(operation) << 8));
        return;
    case 0x18:
    case 0x19:
        if ((g_aicaMemory->currentCommand & 0x40U) == 0)
            AicaUpdate(g_aicaMemory->currentCommand);
        return;
    }
}

void ExecuteAicaBankA() {
    if (!g_aicaMemory)
        return;
    const std::uint32_t command = g_aicaMemory->currentCommand;
    const std::uint8_t operation = static_cast<std::uint8_t>(command >> 24) & 0x0f;
    if (operation == 0) {
        const std::uint8_t subcommand = static_cast<std::uint8_t>((command >> 16) & 0x7f);
        switch (subcommand) {
        case 0:
            EmitAudioForAica();
            return;
        case 1:
            g_aicaMemory->globalVolume = static_cast<std::int8_t>((command >> 8) & 0x7f);
            ExecuteAicaCommand_1(0xa001, 0);
            break;
        case 2:
            ExecuteAicaCommand_1(0xa002, (command & 0x10U) == 0 ? 0x0f : 0);
            break;
        case 3:
            ExecuteAicaCommand_1(0xa003, (command & 0x10U) == 0 ? 0x0f : 0);
            break;
        case 4:
            ExecuteAicaCommand_5(0xa004, 0);
            break;
        case 5:
            ExecuteAicaCommand_5(0xa005, 0);
            break;
        case 7:
            ExecuteAicaCommand_6();
            break;
        case 0x0b:
            ExecuteAicaCommand_1(0xa00b, (command & 0x10U) == 0 ? 0x0f : 0);
            break;
        case 0x0c:
            ExecuteAicaCommand_1(0xa00c, (command & 0x10U) == 0 ? 0x0f : 0);
            break;
        case 0x10:
            ExecuteAicaCommand_5(0xa010, 0);
            break;
        case 0x11:
            ExecuteAicaCommand_5(0xa011, 0);
            break;
        case 0x19:
            SelectAicaProgram(static_cast<std::uint8_t>(command & 0x0fU), static_cast<std::uint8_t>((command >> 8) & 0xffU));
            break;
        case 0x1c: {
            const std::uint8_t bank = static_cast<std::uint8_t>(command & 0x0f);
            const std::uint8_t requested = static_cast<std::uint8_t>((command >> 8) & 0x7f);
            reinterpret_cast<std::uint8_t *>(g_aicaMemory)[0x751 + 24U * bank] = kAicaVolumeTable[requested];
            ExecuteAicaCommand_1(0xa01c, 0x0f);
            break;
        }
        case 0x28:
            ExecuteAicaCommand_8(true);
            break;
        case 0x29:
            ExecuteAicaCommand_8(false);
            break;
        case 0x24:
            reinterpret_cast<std::uint8_t *>(g_aicaMemory)[0x70c] = static_cast<std::uint8_t>((command >> 8) & 0x7f);
            break;
        case 0x30:
            g_aicaMemory->maximumPromotionsPerTick = static_cast<std::uint8_t>((command >> 8) & 0x7f);
            break;
        default:
            break;
        }
        if ((command & 0x40U) == 0)
            AicaUpdate(command);
        return;
    }
    if (operation == 4 && (command & 0xff700000U) == 0xa4700000U) {
        reinterpret_cast<std::uint8_t *>(g_aicaMemory)[0x733] = static_cast<std::uint8_t>((command >> 8) & 0x7fU);
        return;
    }
    if (operation >= 5 && operation <= 7) {
        ExecuteAicaEnvironmentCommand();
        return;
    }
    if (operation >= 8 && operation <= 13 && (command & 0x40U) == 0) {
        AicaUpdate(command);
    }
}

std::int64_t LoadAICADriver(std::uint16_t *driver, std::uint8_t voiceIndex) {
    if (!g_aicaMemory || !driver || voiceIndex >= 64)
        return 0;
    const std::uint32_t bank = g_aicaMemory->ownerId;
    if (bank >= 16 || !g_aicaMemory->bankRegionLoaded[bank])
        return 0;
    AicaBankSlot &bankSlot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = bankSlot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!bankData || bankSize < 0x40)
        return 0;
    const bool modern = modernBanks[bank];
    const std::size_t tableHeader = modern ? 32U : 64U;
    const std::size_t exclusiveOffset = modern ? 50U : 34U;
    const std::size_t layerOffset = modern ? 51U : 35U;
    const auto contains = [&](const void *pointer, std::size_t bytes) {
        const auto *const address = static_cast<const std::uint8_t *>(pointer);
        return address >= bankData && static_cast<std::size_t>(address - bankData) <= bankSize &&
            bytes <= bankSize - static_cast<std::size_t>(address - bankData);
    };

    auto *const memory = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * voiceIndex;
    std::uint8_t *const driverBase = read_unaligned<std::uint8_t *>(memory + 0x8f0 + configBase);
    if (!driverBase || !contains(driverBase, 16))
        return 0;
    const std::uint16_t recordOffset = *driver;
    if (!recordOffset || !contains(driverBase + recordOffset, 4))
        return 0;
    std::uint8_t *const table = driverBase + recordOffset;
    const std::uint8_t driverKind = driverBase[0];
    const std::int32_t commandPart = static_cast<std::uint8_t>(g_aicaMemory->currentCommand >> 8);
    const std::uint8_t commandNote = static_cast<std::uint8_t>(g_aicaMemory->currentCommand >> 16);
    std::uint8_t *note = nullptr;
    std::int32_t pitchNote = 0;
    if (driverKind == 0) {
        if (!contains(table, tableHeader) || commandPart < table[2] || commandPart > table[3] || !contains(table + tableHeader, 64)) {
            return 0;
        }
        note = table + tableHeader;
        const std::uint16_t recordCount = read_unaligned<std::uint16_t>(table);
        for (std::uint32_t index = 0; index < recordCount && note[0] < commandNote; ++index) {
            if (!contains(note + 64, 64))
                return 0;
            note += 64;
        }
        pitchNote = static_cast<std::int32_t>(commandNote) - 60;
    } else if (driverKind == 1) {
        if (!contains(table, tableHeader + 2) || commandNote < table[tableHeader] || commandNote > table[tableHeader + 1]) {
            return 0;
        }
        const std::size_t offsetIndex = static_cast<std::size_t>(commandNote - table[tableHeader]);
        if (!contains(table + tableHeader + 2 + 2 * offsetIndex, 2))
            return 0;
        const std::uint16_t offset = read_unaligned<std::uint16_t>(table + tableHeader + 2 + 2 * offsetIndex);
        if (!offset || !contains(table + offset, 64))
            return 0;
        note = table + offset;
    } else {
        const std::uint32_t index = static_cast<std::uint32_t>(commandNote) | (8U * (g_aicaMemory->currentCommand & 0xf0U));
        if (!contains(table, 2) || index > read_unaligned<std::uint16_t>(table) || !contains(table + tableHeader + 64U * index, 64)) {
            return 0;
        }
        note = table + tableHeader + 64U * index;
    }
    if (!note || (driverKind != 0 && (commandPart < table[2] || commandPart > table[3]))) {
        return 0;
    }
    if (driverKind != 0) {
        const std::uint8_t exclusiveMode = note[exclusiveOffset] & 0x83U;
        if (exclusiveMode == 0x82U) {
            for (const AicaVoiceSlot &active : g_aicaMemory->voicePool.slots) {
                if ((active.status == 1 || active.status == 2) && active.playFlags == 0x82U &&
                    (active.spawnCommand & 0x0f00000fU) == (g_aicaMemory->currentCommand & 0x0f00000fU) && active.layerFlags == note[layerOffset]) {
                    return 0;
                }
            }
        } else if (exclusiveMode == 0x80U || exclusiveMode == 0x81U) {
            for (AicaVoiceSlot &active : g_aicaMemory->voicePool.slots) {
                const bool matchesCommon = (active.status == 1 || active.status == 2) && active.playFlags == exclusiveMode;
                const bool matchesCommand = exclusiveMode == 0x80U ?
                    (active.spawnCommand & 0x0f7f00ffU) == (g_aicaMemory->currentCommand & 0x0f7f00ffU) :
                    (active.spawnCommand & 0x0f00000fU) == (g_aicaMemory->currentCommand & 0x0f00000fU) && active.layerFlags == note[layerOffset];
                if (!matchesCommon || !matchesCommand || active.playState != 1)
                    continue;
                const std::ptrdiff_t activeIndex = &active - g_aicaMemory->voicePool.slots;
                if (activeIndex >= 0 && activeIndex < static_cast<std::ptrdiff_t>(g_aicaPlaybackOwners.size()))
                    StopRawSoundEffectPlayback(g_aicaPlaybackOwners[static_cast<std::size_t>(activeIndex)]);
                active.xaudioStreamKind = -1;
                active.playState = 2;
            }
        }
    }

    const std::uint16_t sampleId = read_unaligned<std::uint16_t>(note + 2);
    const std::byte *sampleData = nullptr;
    std::uint8_t *sampleLengthCorrection = nullptr;
    std::uint32_t audioBytes = 0;
    std::uint32_t loopBegin = 0;
    std::uint32_t loopLength = 0;
    std::uint16_t channels = 1;
    std::uint16_t bits = 16;
    std::uint32_t sampleRate = modern ? 48000U : 44100U;
    if (sampleId < 0x100U) {
        std::size_t residentOffset = 0;
        if (sampleId >= 1 && sampleId <= 42) {
            residentOffset = 200U * (sampleId - 1U);
            audioBytes = 200;
            loopLength = 100;
        } else if (sampleId >= 43 && sampleId <= 84) {
            residentOffset = 200U * (sampleId - 43U);
            audioBytes = 400;
            loopLength = 200;
        } else if (sampleId >= 85 && sampleId <= 168) {
            residentOffset = 100U * (sampleId - 85U);
            audioBytes = 100;
            loopLength = 50;
        } else if (sampleId == 0 || sampleId == 169) {
            audioBytes = static_cast<std::uint32_t>(kAicaResidentNoisePcm.size());
            loopLength = 16384;
        } else {
            return 0;
        }
        if (sampleId == 0 || sampleId == 169) {
            sampleData = reinterpret_cast<const std::byte *>(kAicaResidentNoisePcm.data());
        } else {
            if (residentOffset > kAicaResidentPcm.size() || audioBytes > kAicaResidentPcm.size() - residentOffset)
                return 0;
            sampleData = reinterpret_cast<const std::byte *>(kAicaResidentPcm.data() + residentOffset);
        }
    } else {
        BankSample sample{};
        if (modern) {
            const std::size_t sampleTable = read_unaligned<std::uint32_t>(bankData + 0xb8);
            const std::size_t descriptorOffset = sampleTable + 4U + 20U * static_cast<std::size_t>(sampleId - 256U);
            std::uint8_t *const descriptor = AicaBankOffset(bankData, bankSize, descriptorOffset, 20);
            if (descriptor && read_unaligned<std::uint32_t>(descriptor) == CORRECTED_SAMPLE_DATA_OFFSET)
                sampleLengthCorrection = descriptor + 16;
        }
        const std::uint32_t replacementBytes = sampleLengthCorrection ? CORRECTED_SAMPLE_BYTE_COUNT : 0;
        if (!ResolveBankSample(static_cast<std::uint8_t>(bank), sampleId, modern || (note[1] & 0x80U) == 0, sample, replacementBytes))
            return 0;
        sampleData = sample.data;
        audioBytes = sample.bytes;
        loopBegin = sample.loopBegin;
        loopLength = sample.loopLength;
        channels = sample.channels;
        bits = sample.bits;
        sampleRate = sample.rate;
    }
    const std::uint32_t velocityTableOffset = read_unaligned<std::uint32_t>(bankData + (modern ? 0xa0 : 0x28));
    const std::size_t velocityOffset =
        static_cast<std::size_t>(velocityTableOffset) + 4U + 128U * static_cast<std::size_t>(table[6]) + static_cast<std::size_t>(commandPart);
    if (velocityOffset >= bankSize)
        return 0;
    std::uint32_t level = (static_cast<std::uint32_t>(bankData[velocityOffset]) *
                              (modern ? note[8] & 0xfeU : ((static_cast<std::uint32_t>(note[8]) + note[9]) >> 1U) & 0x7ffffffeU)) >>
        8U;
    if (level)
        ++level;
    level = (2U * memory[0x8cc + configBase] * level) >> 8U;
    if (level)
        ++level;
    level = (2U * memory[0x8dc + configBase] * level) >> 8U;
    if (level)
        ++level;
    const std::uint8_t preBankLevel = static_cast<std::uint8_t>(level);
    const bool usesBankVolume = (memory[0x8c1 + configBase] & 1U) == 0U;
    if (usesBankVolume) {
        level = (2U * memory[0x751 + 24U * bank] * level) >> 8U;
        if (level)
            ++level;
    }
    level = (2U * static_cast<std::uint8_t>(g_aicaMemory->globalVolume) * level) >> 8U;
    if (level)
        ++level;
    const std::uint8_t soundType = bank == 5 ? 0 : static_cast<std::uint8_t>((memory[0x8f9 + configBase] != 0) + 1);
    const float volume = AicaLevelToGain(level) * SoundTypeVolume(soundType);
    const int bend = static_cast<int>(memory[0x8e2 + configBase]) - (memory[0x8e2 + configBase] > 64 ? 63 : 64);
    const int bendRange = modern ? static_cast<std::int8_t>(table[bend >= 0 ? 25 : 26]) * 256 : 0;
    const int bendPitch = bend * bendRange;
    const std::int32_t pitchUnits = static_cast<std::int32_t>((static_cast<double>(static_cast<std::int8_t>(memory[0x8db + configBase]) +
                                                                   static_cast<std::int8_t>(note[modern ? 10 : 11]) + ((bendPitch >> 8) & 63)) *
                                                                  1024.0 / 3.0 * 0.015625) +
        (static_cast<double>(
             static_cast<std::int8_t>(memory[0x8da + configBase]) + static_cast<std::int8_t>(note[modern ? 9 : 10]) + pitchNote + (bendPitch >> 14)) *
            1024.0 / 3.0));
    if (pitchUnits > 0x2000)
        return 0;
    const float frequencyRatio = std::pow(2.0f, static_cast<float>(pitchUnits) * (1.0f / 4096.0f));
    const std::int8_t authoredAttenuation = static_cast<std::int8_t>(note[7]);
    const std::uint8_t attenuationIndex = authoredAttenuation >= 0 ? static_cast<std::uint8_t>(authoredAttenuation) : memory[0x8cb + configBase];
    if (attenuationIndex >= kAicaPanAttenuation.size())
        return 0;
    const std::int16_t panBaseAttenuation = kAicaPanAttenuation[attenuationIndex];

    const std::uint8_t activeList = driverKind == 2 ? 2U : 1U;
    const std::uint8_t allocatedVoice = AllocateAicaVoice(activeList);
    if (allocatedVoice == 0xff)
        return 0;
    const std::size_t playbackIndex = allocatedVoice;
    AicaVoiceSlot &voice = g_aicaMemory->voicePool.slots[playbackIndex];
    voice.status = (memory[0x8c1 + configBase] & 1U) ? 2U : 1U;
    soundTypes[playbackIndex] = soundType;
    voice.playState = 0;
    voice.ownerId = static_cast<std::int8_t>(voiceIndex);
    voice.bank = static_cast<std::uint8_t>(bank);
    voice.spawnCommand = g_aicaMemory->currentCommand;
    voice.driverFlags = static_cast<std::uint8_t>(((memory[0x8c1 + configBase] | 0xfeU) << 6U) | (static_cast<std::int8_t>(note[1]) < 0 ? 8U : 0U));
    if (memory[0x8e3 + configBase])
        voice.driverFlags |= 1U;
    voice.layerFlags = note[layerOffset];
    voice.playFlags = note[exclusiveOffset] & 0xc3U;
    voice.part = static_cast<std::uint8_t>(commandPart);
    voice.noteAttenuation = static_cast<std::uint8_t>(note[modern ? 52 : 36] + 127U);
    voice.stereo = channels == 2 ? 1U : 0U;
    voice.driverNoteTable = reinterpret_cast<std::uintptr_t>(note);
    voice.driverPointerTable = reinterpret_cast<std::uintptr_t>(table);
    voice.xaudioVoiceId = 0xffffffffU;
    voice.xaudioStreamKind = 1;
    voice.pitchOrBaseNote = panBaseAttenuation;
    g_aicaPlaybackOwners[playbackIndex] = nullptr;
    g_aicaPlaybackPreBankLevels[playbackIndex] = preBankLevel;
    g_aicaPlaybackUsesBankVolume[playbackIndex] = usesBankVolume ? 1U : 0U;

    if (sampleLengthCorrection)
        write_unaligned(sampleLengthCorrection, CORRECTED_SAMPLE_BYTE_COUNT);
    void *const playback = PrepareRawSoundEffect(sampleData, audioBytes, loopBegin, loopLength, channels, volume, frequencyRatio, sampleRate, true, bits);
    if (!playback) {
        voice.xaudioStreamKind = -1;
        ResetAicaVoice(voice);
        return 0;
    }
    voice.xaudioVoiceId = RawSoundEffectVoiceId(playback);
    g_aicaPlaybackOwners[playbackIndex] = playback;
    const std::uint8_t reverbLevel =
        modern ? 0 : (note[30] != 0 ? static_cast<std::uint8_t>(memory[0x8ca + configBase] >> 4U) : static_cast<std::uint8_t>(note[31] >> 4U));
    const bool firstEffectUnassigned = modern && (static_cast<std::int8_t>(note[45]) >= 0 ? note[44] == 0xff : memory[0x8d6 + configBase] == 0xff);
    const std::uint8_t effectLevel =
        modern ? (static_cast<std::int8_t>(note[45]) >= 0 ? note[45] : static_cast<std::uint8_t>(std::min(127, memory[0x8d7 + configBase] + 10))) : 0;
    const std::int32_t wetAttenuation = firstEffectUnassigned ? 0 : TruncateToInt32((127.0f - effectLevel) * -37.5f);
    const float reverbDecibels = modern ? static_cast<float>(wetAttenuation) / 100.0f :
        reverbLevel == 0                ? -100.0f :
                                          3.0f * (static_cast<float>(reverbLevel) - 15.0f);
    const float reverbGain = reverbDecibels > -64.0f ? std::pow(10.0f, reverbDecibels / 20.0f) : 0.0f;
    const std::array<float, 2> reverbMatrix{reverbGain, reverbGain};
    SetRawSoundEffectOutputMatrix(playback, reverbMatrix.data(), channels, 1);
    ApplyVoicePanMatrixToXAudio(playbackIndex);
    if (!modern && sampleId == 1) {
        note[12] = 0xd2U;
        note[14] = 0x2fU;
    }
    const std::int32_t envelopeNote = driverKind == 0 ? 60 :
        driverKind == 1                               ? commandNote - table[tableHeader] :
                                                        static_cast<std::int32_t>(commandNote | (8U * (g_aicaMemory->currentCommand & 0xf0U)));
    if (modern) {
        AicaState *envelope = GetRawSoundEffectEnvelopeState(voice.xaudioVoiceId);
        if (envelope) {
            envelope->attackRate = static_cast<float>(read_unaligned<std::uint16_t>(note + 16));
            envelope->holdTime = static_cast<float>(read_unaligned<std::uint16_t>(note + 18));
            envelope->decay1Rate = static_cast<float>(read_unaligned<std::uint16_t>(note + 20));
            envelope->decay2Rate = 0.0f;
            envelope->releaseRate = note[13] ? static_cast<float>(read_unaligned<std::uint16_t>(note + 24)) : 10.0f;
            envelope->sustainLevel = read_unaligned<std::uint16_t>(note + 22) / 255.0f;
            envelope->egh = note[13] == 0 || note[13] == 3;
            const float envelopeVolume = SoundTypeVolume(soundType) * volume;
            envelope->linearVolume = envelopeVolume;
            envelope->baseVolumeDb = envelopeVolume > 0 ? std::max(-64.0f, std::log10(envelopeVolume) * 20.0f) : -64.0f;
            envelope->ownerId = bank;
            envelope->legacyEnvelope = 0;
            envelope->phase = AICA_ENVELOPE_OFF;
            envelope->filterLevel0 = 0;
            envelope->instantFilterAttack = 1;
            envelope->oneOverQ = 1;
            ApplyAicaEnvelopeGain(*envelope);
            SetAttackFilterParams(*envelope);
        }
    } else {
        SetAttackFilterParamsForTone(voice.xaudioVoiceId, note, envelopeNote, volume, false, g_aicaMemory->ownerId);
    }
    StartRawSoundEffectPlayback(playback);
    voice.playState = 1;
    if (g_aicaMemory->voicePool.paused)
        SetRawSoundEffectPaused(playback, true);
    return 1;
}

void TickMiddleware() {
    if (!initialized)
        return;
    ReapInactiveAicaVoices();
    if (memory.voicePool.paused)
        return;
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(&memory);
    for (std::size_t index = 0; index < 16; ++index) {
        const std::size_t base = 0x1cbc + 56 * index;
        if (raw[base + 4] && !(raw[base + 4] & 1U)) {
            const std::uint32_t delay = read_unaligned<std::uint32_t>(raw + base + 16);
            const std::uint32_t step = read_unaligned<std::uint32_t>(raw + base + 8);
            write_unaligned(raw + base + 16, delay - 4U * step);
        }
    }
    PromoteQueuedAicaCommands();
    ExecuteAicaInsnForCycles();
    ProcessAicaEventStreams();
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point previous = now;
    const std::int32_t microseconds = static_cast<std::int32_t>(std::chrono::duration_cast<std::chrono::microseconds>(now - previous).count());
    previous = now;
    UpdateAicaEnvironmentEnvelopes(static_cast<float>(static_cast<double>(static_cast<float>(microseconds)) * 0.000001));
}

std::int32_t SNDFunc(std::uint32_t command) {
    if (memory.commandQueueBusy)
        return -3;
    if (static_cast<std::int8_t>(command & 0xffU) >= 0)
        return -2;
    if (memory.pendingQueueCount >= 64 || memory.pendingCommandRing[memory.pendingWriteIndex] != 0)
        return -1;
    memory.pendingCommandRing[memory.pendingWriteIndex] = ByteSwap32(command);
    memory.pendingWriteIndex = (memory.pendingWriteIndex + 1) & 0x3f;
    ++memory.pendingQueueCount;
    PromoteQueuedAicaCommands();
    ExecuteAicaInsnForCycles();
    return 0;
}

std::int32_t Submit(const AicaBackendRequest &request) noexcept {
    const std::lock_guard<std::recursive_mutex> lock(GetAudioBackendMutex());
    if (!initialized)
        return -1;
    ReapInactiveAicaVoices();
    try {
        switch (request.operation) {
        case AicaBackendOperation::SetBankProperty:
            if (request.bank == 1)
                memory.globalPanRight = static_cast<std::uint8_t>(request.value);
            else if (request.bank == 0)
                memory.globalPanLeft = static_cast<std::uint8_t>(request.value);
            else
                return -2;
            return 0;
        case AicaBackendOperation::SetBankAllocation: {
            if (request.bank >= banks.size() || request.value < 0 || (request.value & 31) != 0)
                return -2;
            if (memory.bankRegionLoaded[request.bank])
                return -2;
            std::uint64_t total = static_cast<std::uint32_t>(request.value);
            for (std::size_t bank = 0; bank < allocations.size(); ++bank)
                if (bank != request.bank)
                    total += allocations[bank];
            if (total > 0x1000000)
                return -1;
            allocations[request.bank] = static_cast<std::uint32_t>(request.value);
            return 0;
        }
        case AicaBackendOperation::LoadBank: {
            if (request.bank >= banks.size() || !request.data || request.dataSize < 0x64)
                return -2;
            const std::uint32_t signature = read_unaligned<std::uint32_t>(request.data);
            const bool modern = signature == 0x78626f78U;
            if (modern &&
                (request.dataSize < 0xc0 || read_unaligned<std::uint32_t>(request.data + 4) != 0x4454504bU ||
                    read_unaligned<std::uint16_t>(request.data + 8) < 0x1402))
                return -7;
            const std::uint64_t fullSize =
                modern ? static_cast<std::uint64_t>(read_unaligned<std::uint32_t>(request.data + 12)) + 72 : read_unaligned<std::uint32_t>(request.data + 8);
            if (fullSize < (modern ? 0xc0U : 0x64U) || fullSize > request.dataSize)
                return -2;
            const std::uint32_t size = static_cast<std::uint32_t>(fullSize);
            const std::uint8_t units = std::to_integer<std::uint8_t>(request.data[modern ? 0x89 : 16]);
            if (memory.bankRegionLoaded[request.bank])
                return -3;
            if (size > allocations[request.bank])
                return -6;
            if (static_cast<unsigned>(memory.loadedBankUnits) + units > 64)
                return -5;
            std::vector<std::uint8_t> &destination = banks[request.bank];
            destination.resize(allocations[request.bank]);
            std::memset(destination.data(), 0, destination.size());
            std::memcpy(destination.data(), request.data, size);
            modernBanks[request.bank] = modern;
            decodedSamples[request.bank].clear();
            memory.bankRegionBase[request.bank] = destination.data();
            memory.bankRegionSize[request.bank] = allocations[request.bank];
            memory.bankRegionLoaded[request.bank] = 1;
            AicaBankSlot &slot = memory.bankSlots[request.bank];
            slot.bankData = destination.data();
            slot.bankIdOrType = read_unaligned<std::int32_t>(request.data + (modern ? 0x84 : 4));
            slot.loadedSize = modern ?
                (units | (std::to_integer<std::uint32_t>(request.data[0x8c]) << 8) | (std::to_integer<std::uint32_t>(request.data[0x8d]) << 16)) :
                0x01000000U | (read_unaligned<std::uint32_t>(request.data + 16) & 0x00ffffffU);
            memory.loadedBankUnits += units;
            memory.globalPanLeft = modern ? std::to_integer<std::uint8_t>(request.data[0x91]) : 0;
            memory.globalPanRight = modern ? std::to_integer<std::uint8_t>(request.data[0x92]) : 0;
            InitializeLoadedAicaBank(static_cast<std::uint8_t>(request.bank));
            return 0;
        }
        case AicaBackendOperation::UnloadBank: {
            if (request.bank >= banks.size())
                return -1;
            for (AicaVoiceSlot &voice : memory.voicePool.slots) {
                if (voice.status != 0xff &&
                    (voice.bank == request.bank || (voice.status == 3 && g_aicaEnvironmentStates[voice.bank].loadedBank == request.bank)))
                    ResetAicaVoice(voice);
            }
            std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(&memory);
            for (std::size_t index = 0; index < 16; ++index)
                if (raw[0x1cc2 + 56 * index] == request.bank)
                    raw[0x1cc0 + 56 * index] = 0;
            const std::int32_t result = ReleaseOwnedAicaBankUnlocked(request.bank);
            memory.bankSlots[request.bank].bankData = nullptr;
            memory.bankRegionBase[request.bank] = nullptr;
            memory.bankRegionSize[request.bank] = 0;
            banks[request.bank].clear();
            decodedSamples[request.bank].clear();
            modernBanks[request.bank] = false;
            return result;
        }
        case AicaBackendOperation::BankCommand:
            return static_cast<std::int32_t>(
                ExecuteAicaCommand(static_cast<std::uint8_t>(request.bank), ByteSwap32(request.command), static_cast<std::int16_t>(request.value)));
        case AicaBackendOperation::WideBankCommand:
            return static_cast<std::int32_t>(
                ExecuteEnvironmentAicaCommand(static_cast<std::uint8_t>(request.bank), ByteSwap32(request.command), request.value));
        case AicaBackendOperation::MainCommand:
            return SNDFunc(request.command);
        case AicaBackendOperation::TwoChannelCommand:
            return static_cast<std::int32_t>(ExecuteAicaCommand2Ch(
                static_cast<std::uint8_t>(request.bank), request.channel, ByteSwap32(request.command), static_cast<std::int16_t>(request.value)));
        case AicaBackendOperation::QueryBankVolume: {
            if (request.bank >= 16)
                return -128;
            const std::uint32_t command = ByteSwap32(request.command);
            const std::uint8_t *const raw = reinterpret_cast<const std::uint8_t *>(&memory);
            if (command == 0xa0040000U)
                return raw[0x752 + 24 * request.bank];
            if (command == 0xa0100000U)
                return raw[0x753 + 24 * request.bank];
            return -128;
        }
        case AicaBackendOperation::QueryBankSequenceState: {
            const std::uint8_t *const raw = reinterpret_cast<const std::uint8_t *>(&memory);
            for (std::size_t index = 0; index < 16; ++index) {
                const std::size_t base = 0x1cbc + 56 * index;
                if (static_cast<std::int8_t>(raw[base + 4]) < 0 && raw[base + 6] == static_cast<std::uint8_t>(request.bank))
                    return (raw[base + 4] & 1U) | 0x100;
            }
            return 0;
        }
        case AicaBackendOperation::Synchronize:
            PromoteQueuedAicaCommands();
            ExecuteAicaInsnForCycles();
            return 0;
        case AicaBackendOperation::PauseStreams:
        case AicaBackendOperation::ResumeStreams: {
            const bool paused = request.operation == AicaBackendOperation::PauseStreams;
            memory.voicePool.paused = paused;
            for (void *playback : g_aicaPlaybackOwners)
                if (playback)
                    SetRawSoundEffectPaused(playback, paused);
            SetDialogueStreamsPaused(paused);
            return 0;
        }
        }
    } catch (...) {
        return -1;
    }
    return -2;
}
} // namespace

bool AssignAicaVoiceRelease(std::uint32_t physicalId) {
    const std::lock_guard<std::recursive_mutex> lock(GetAudioBackendMutex());
    return ReleaseEnvelope(physicalId);
}
void RefreshOutputVolumes() noexcept {
    const std::lock_guard<std::recursive_mutex> lock(GetAudioBackendMutex());
    if (!initialized)
        return;
    for (std::size_t index = 0; index < 160; ++index) {
        if (g_aicaPlaybackOwners[index] && memory.voicePool.slots[index].status != 3)
            UpdateAicaPlaybackVolume(index);
    }
}
} // namespace shenmue::s2::aica

namespace shenmue::s2 {
bool InitializeS2AicaMiddleware() noexcept {
    const std::lock_guard<std::recursive_mutex> lock(aica::GetAudioBackendMutex());
    if (aica::initialized)
        return true;
    aica::memory = {};
    aica::Aica_InitializeDriverValues();
    for (aica::AicaEnvironmentState &state : aica::g_aicaEnvironmentStates) {
        state = {};
        state.loadedBank = 0xff;
        state.volume = 127;
        state.control = 64;
        state.coarsePitch = 64;
        state.finePitch = 64;
        state.volumeOffset = 64;
        for (aica::AicaEnvironmentPartState &part : state.parts)
            part.voiceIndex = 0xff;
    }
    aica::g_aicaPlaybackOwners.fill(nullptr);
    aica::initialized = true;
    SetAicaBackendRequestSink(aica::Submit);
    try {
        aica::middlewareThread = std::jthread([](std::stop_token stop) {
#if defined(_WIN32)
            const HANDLE delayTimer =
                CreateWaitableTimerExW(nullptr, nullptr, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_MODIFY_STATE | SYNCHRONIZE);
#endif
            std::chrono::steady_clock::time_point previous = std::chrono::steady_clock::now();
            while (!stop.stop_requested()) {
                {
                    const std::lock_guard<std::recursive_mutex> workerLock(aica::GetAudioBackendMutex());
                    aica::TickMiddleware();
                }
                const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                const std::chrono::nanoseconds remaining =
                    std::chrono::nanoseconds(aica::AICA_UPDATE_INTERVAL_NS) - std::chrono::duration_cast<std::chrono::nanoseconds>(now - previous);
#if defined(_WIN32)
                if (delayTimer && remaining.count() > 0) {
                    LARGE_INTEGER due;
                    due.QuadPart = -static_cast<LONGLONG>(static_cast<double>(remaining.count()) * aica::AICA_WORKER_SLEEP_FRACTION / 100.0);
                    if (due.QuadPart < 0 && SetWaitableTimer(delayTimer, &due, 0, nullptr, nullptr, FALSE))
                        WaitForSingleObject(delayTimer, INFINITE);
                }
#else
                {
                    std::mutex waitMutex;
                    std::condition_variable waitCondition;
                    std::unique_lock<std::mutex> waitLock(waitMutex);
                    waitCondition.wait_until(waitLock,
                        now + std::chrono::duration_cast<std::chrono::steady_clock::duration>(remaining * aica::AICA_WORKER_SLEEP_FRACTION));
                }
#endif
                while (!stop.stop_requested() && std::chrono::steady_clock::now() - previous < remaining) {
#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
                    _mm_pause();
#else
                    std::this_thread::yield();
#endif
                }
                previous = std::chrono::steady_clock::now();
            }
#if defined(_WIN32)
            if (delayTimer)
                CloseHandle(delayTimer);
#endif
        });
    } catch (...) {
        SetAicaBackendRequestSink(nullptr);
        aica::initialized = false;
        return false;
    }
    return true;
}
void ReleaseS2AicaMiddleware() noexcept {
    aica::middlewareThread.request_stop();
    if (aica::middlewareThread.joinable())
        aica::middlewareThread.join();
    const std::lock_guard<std::recursive_mutex> lock(aica::GetAudioBackendMutex());
    SetAicaBackendRequestSink(nullptr);
    aica::ResetPlatformVoices();
    for (std::vector<std::uint8_t> &bank : aica::banks)
        bank = {};
    for (auto &samples : aica::decodedSamples)
        samples.clear();
    aica::modernBanks.fill(false);
    aica::initialized = false;
}
} // namespace shenmue::s2
