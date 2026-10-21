#include "s1/task/sond.hpp"
#include "s1/d3t/namespace.hpp"
#include "s1/boot.hpp"
#include "logging.hpp"

#include "s1/hlib.hpp"
#include "common/utility.hpp"
#include "s1/archive_handles.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/came.hpp"
#include "s1/task/tmex.hpp"
#include "s1/player_control.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/task/wexe.hpp"
#include "s1/file_loader.hpp"
#include "s1/save_format.hpp"
#include "s1/task/scen.hpp"
#include "s1/audio_stream.hpp"
#include "s1/subtitles.hpp"
#include "s1/date_time.hpp"
#include "s1/timing.hpp"
#include "d3t/threads.hpp"
#include "s1/d3t/settings.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <mutex>
#include <limits>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <new>
#include <thread>
#if defined(_M_X64) || defined(__x86_64__)
#include <immintrin.h>
#endif

namespace shenmue::s1 {

extern std::array<std::uint8_t, 128> g_shenmueFlags;
extern std::int32_t g_actNumber;
namespace {}

void SOND_UpdateAICA();
// @verified - p
std::int64_t ForEachCewpEntityApplyCallback(std::int64_t, bool (*callback)(Character *)) {
    MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    for (void *bucketHead : control->characterBuckets) {
        for (CharacterBucketNode *bucket = static_cast<CharacterBucketNode *>(bucketHead); bucket; bucket = bucket->next) {
            Character *const character = bucket->character;
            if (character && character->entity.name == SHEN_NAME(CEWP)) {
                const std::uint32_t flags = character->flags.value;
                if (((flags & 1U) != 0 || ((flags & 2U) != 0 && (character->motionFlags & 1U) != 0)) && !callback(character))
                    return 0;
            }
        }
    }
    return 1;
}
void ProcessAmbientSoundEffects();
void ProcessSoundForCewpHandler();
void ExecuteAicaCommandLoop(std::int32_t index);
void PerformAicaSoundCallback(void *entity);
std::int64_t ForEachCewpEntityApplyCallback(std::int64_t parameter, void (*callback)(void *));
std::int64_t ExecuteAicaCommand2Ch(std::int32_t bank, std::int32_t channel, std::uint32_t command, std::int16_t volume);
std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);
void InitAMB1();
void ProcessAudioData(CVWKRuntimeSlot *slot, std::int32_t volume);
void SetVolumeForAicaBank(float volume, std::uint32_t bank);
void AicaUpdate(std::uint32_t command);
void ExecuteAicaInsn(std::uint32_t command);
std::int64_t ExecuteAicaCommand_0(std::uint16_t command);
void ExecuteAicaCommand_1(std::uint16_t command, std::int32_t mask);
void ExecuteAicaBankA();
void ExecuteAicaBankB();
void ExecuteAicaBankC();
std::int64_t ExecuteAicaCommand_2();
std::int64_t LoadAICADriver(std::uint16_t *driver, std::uint8_t voiceIndex);
void ResolveAicaDriverPointer(std::uint8_t bank, std::uint8_t voiceIndex);
void ProcessAicaEventStreams();

EnvironmentAicaBank g_environmentAicaBanks[16]{};
std::int16_t g_environmentAicaBankVolumes[16]{};
AicaEnvironmentState g_aicaEnvironmentStates[16]{};
std::int32_t g_soundTaskState = -1;
std::int32_t g_aicaFrameTimer = 0;
std::int32_t g_performedAicaCommand = 0;
float g_sfxVolume = 1.0f;
AicaMemory g_defaultAicaMemory{};
bool g_bIsAICAInitialized = false;
float g_musicVolume = 1.0f;
std::atomic<bool> g_audioWorkerStop{};
std::atomic<bool> g_audioWorkerRunning{};
bool g_aicaInitialized = false;
HLib::Task *g_sondTask = nullptr;
AicaMemory *g_aicaMemory = nullptr;
std::uint8_t g_aicaNeedsBankCleanup[16]{};
void *g_soundPath = nullptr;
HLib::TaskData *g_soundTaskData = nullptr;
std::int32_t g_soundDataNumber = 0;
std::int32_t g_soundFlag = 0;
std::int32_t g_soundIndex = 0;
char g_soundOffsetName[0x30]{};
char g_soundTable[16][16]{};
std::int32_t g_currentAicaCommandLoops = 0;
void *g_pAMB1 = nullptr;
void *g_pAMB1Tmp = nullptr;
std::uint32_t g_ambientFrameParity = 0;
std::array<void *, 160> g_aicaPlaybackOwners{};
std::array<std::uint8_t, 160> g_aicaPlaybackPreBankLevels{};
std::array<std::uint8_t, 160> g_aicaPlaybackUsesBankVolume{};

namespace {
void ResetAicaVoice(AicaVoiceSlot &voice);
void ReapInactiveAicaVoices();

std::mutex g_aicaCommandMutex;
std::int32_t g_aicaDebugLogEnabled = 0;

} // namespace
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
namespace {

std::uint16_t SoundFixedAtan2(float a1, float direction) {
    if (direction < a1) {
        if (direction < -a1) {
            if (direction == 0.0f)
                return 0x8000U;
            const float angle = std::atan(a1 / direction) * 65536.0f / 6.2831855f + 0.5f;
            return static_cast<std::uint16_t>(static_cast<std::int32_t>(angle) + 0x8000);
        }
        if (a1 == 0.0f)
            return 0x4000U;
        const float angle = std::atan(direction / a1) * 65536.0f / 6.2831855f + 0.5f;
        return static_cast<std::uint16_t>(0x4000 - static_cast<std::int32_t>(angle));
    }
    if (direction < -a1) {
        if (a1 == 0.0f)
            return 0xc000U;
        const float angle = std::atan(direction / a1) * 65536.0f / 6.2831855f + 0.5f;
        return static_cast<std::uint16_t>(0xc000 - static_cast<std::int32_t>(angle));
    }
    if (direction == 0.0f)
        return 0;
    const float angle = std::atan(a1 / direction) * 65536.0f / 6.2831855f + 0.5f;
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(angle));
}

Vector3 AmbientSoundListenerPosition() {
    const MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!ShouldRun())
        return g_CameraEyePos;
    return GetPositionForEntity(static_cast<Character *>(control->ryo)->entity.node);
}

Vector3 ThreeDimensionalSoundListenerPosition() {
    const MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    return ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
}

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

void SetVolumeForAicaForXAudio(std::uint32_t physicalId, float volume, bool music) {
    if (!Audio_HasMasteringVoice() || GetXAudioEffectFlagVal() == 1)
        return;
    AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
    if (!state) {
        if (GetXAudioEffectFlagVal() == 0)
            SetRawSoundEffectVolume(GetRawSoundEffectVoice(physicalId), volume);
        return;
    }
    if (music)
        volume *= g_musicVolume;
    state->linearVolume = volume;
    state->baseVolumeDb = volume == 0.0f ? -64.0f : (std::max)(-64.0f, std::log10(volume) * 20.0f);
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
    SetVolumeForAicaForXAudio(voice.xaudioVoiceId, linear, g_aicaMemory->ownerId == 2);
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

std::uint8_t AicaBankDriverLevel(std::uint8_t bank, std::uint8_t voiceIndex) {
    if (!g_aicaMemory || bank >= 16 || voiceIndex >= 128)
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
        std::uint8_t *const assignments = raw + 0x28c0 + 64U * bank + 4U * channel;
        for (std::size_t candidate = 0; candidate < 4; ++candidate) {
            const std::uint8_t voiceIndex = assignments[candidate];
            if (voiceIndex == 0xff)
                continue;
            const std::size_t voiceBase = 64U * voiceIndex;
            if (((raw[0x8c1 + voiceBase] & 1U) != 0) == normalDrivers)
                continue;
            raw[0x8c0 + voiceBase] = 0;
            raw[0x8c1 + voiceBase] = 0;
            write_unaligned<std::uint8_t *>(raw + 0x8f0 + voiceBase, nullptr);
            write_unaligned<std::uint8_t *>(raw + 0x8f8 + voiceBase, nullptr);
            raw[0x8c2 + voiceBase] = 0;
            raw[0x8e8 + voiceBase] = 64;
            raw[0x8e9 + voiceBase] = 0;
            raw[0x8ea + voiceBase] = 0;
            raw[0x8eb + voiceBase] = 0;
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
    if (!AicaBankPointerValid(bankData, bankSize, program + 8, 32U * recordCount)) {
        return;
    }

    auto *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    std::uint8_t voiceIndex = 0xff;
    for (std::size_t recordIndex = 0; recordIndex < recordCount; ++recordIndex) {
        std::uint8_t *const record = program + 8 + 32U * recordIndex;
        if (static_cast<std::int8_t>(record[0]) < 0) {
            voiceIndex = 0xff;
            for (std::uint8_t candidate = 0; candidate < 128; ++candidate) {
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
            raw[0x8ca + voiceBase] = record[8];
            raw[0x8cb + voiceBase] = record[9];
            raw[0x8cc + voiceBase] = record[10];
            raw[0x8cd + voiceBase] = record[11];
            raw[0x8ce + voiceBase] = record[12];
            raw[0x8cf + voiceBase] = record[13];
            std::memcpy(raw + 0x8d2 + voiceBase, record + 16, 7);
            raw[0x8e2 + voiceBase] = AicaBankDriverLevel(bank, voiceIndex);
            raw[0x8e3 + voiceBase] = record[12];
            write_unaligned(raw + 0x8f0 + voiceBase, record);
            ResolveAicaDriverPointer(bank, voiceIndex);
        } else if (voiceIndex == 0xff) {
            return;
        }

        const std::uint8_t channel = record[1];
        if (channel >= 16)
            continue;
        std::uint8_t *const assignments = raw + 0x28c0 + 64U * bank + 4U * channel;
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
    const std::uint32_t programBaseOffset = read_unaligned<std::uint32_t>(bankData + 0x20);
    std::uint8_t *const programBase = AicaBankOffset(bankData, bankSize, programBaseOffset, 1);
    auto *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::size_t bankBase = 24U * bank;
    raw[0x750 + bankBase] = 0xff;
    raw[0x751 + bankBase] = 127;
    const std::uint16_t defaultProgramOffset = read_unaligned<std::uint16_t>(bankData + 0x62);
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
    raw[0x8e8 + voiceBase] = 64;
    raw[0x8e9 + voiceBase] = 0;
    raw[0x8ea + voiceBase] = 0;
    raw[0x8eb + voiceBase] = 0;
    const std::uint8_t *const note = read_unaligned<std::uint8_t *>(raw + 0x8f0 + voiceBase);
    if (note) {
        raw[0x8cd + voiceBase] = note[11];
        raw[0x8ce + voiceBase] = note[12];
        raw[0x8e3 + voiceBase] = note[12];
        raw[0x8cf + voiceBase] = note[13];
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
            const std::uint8_t *const assignments = raw + 0x28c0 + 64U * bank + 4U * channel;
            for (std::size_t candidate = 0; candidate < 4; ++candidate) {
                const std::uint8_t voiceIndex = assignments[candidate];
                if (voiceIndex >= 128 || (raw[0x8c1 + 64U * voiceIndex] & 1U) != 0)
                    continue;
                ResetAicaDriverDynamicFields(voiceIndex);
            }
        }
        return;
    }

    const std::uint16_t maximumProgram = read_unaligned<std::uint16_t>(bankData + 0x60);
    if (programIndex > maximumProgram)
        return;
    const std::size_t offsetEntry = 0x64U + 2U * programIndex;
    const std::uint8_t *const entry = AicaBankOffset(bankData, bankSize, offsetEntry, 2);
    if (!entry)
        return;
    const std::uint16_t programOffset = read_unaligned<std::uint16_t>(entry);
    const std::uint32_t programBaseOffset = read_unaligned<std::uint32_t>(bankData + 0x20);
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
            const std::uint8_t driver = raw[0x28c0 + 64U * bank + 4U * channel + candidate];
            if (driver == 0xff)
                continue;
            const std::size_t base = 64U * driver;
            if (command == 1 || command == 0x11) {
                if ((raw[0x8c1 + base] & 1U) == 0)
                    ResetAicaDriverDynamicFields(driver);
            } else if (command == 0xa004) {
                raw[0x8e2 + base] = AicaBankDriverLevel(bank, driver);
            } else if (command == 0xa007 && (raw[0x8c1 + base] & 1U) == 0) {
                raw[0x8ce + base] = static_cast<std::uint8_t>(value + raw[0x8e3 + base] - 64U);
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
    const std::uint8_t *const assignments = raw + 0x28c0 + 64U * bank + 4U * channel;
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
    const float renderedLevel = envelope.egh && envelope.phase == 1 ? 1.0f : envelope.envelopeLevel;
    const float volumeDb = envelope.baseVolumeDb + AicaEnvironmentEnvelopeDecibels(renderedLevel);
    if (volumeDb == envelope.currentVolumeDb)
        return;
    SetRawSoundEffectVolume(GetRawSoundEffectVoice(envelope.voiceId), volumeDb > -64.0f ? std::pow(10.0f, volumeDb / 20.0f) : 0.0f);
    envelope.currentVolumeDb = volumeDb;
}

void UpdateAicaLowPassFilter(std::uint32_t physicalId, AicaState &envelope, float elapsedSeconds) {
    if (envelope.instantFilterAttack)
        return;
    const float filterElapsed = envelope.filterElapsed;
    if (envelope.phase >= 1 && envelope.phase <= 3) {
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
    } else if (envelope.phase == 4) {
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
    const std::uint8_t attack = DecodeAicaEnvelopeRate(60, 15, entry[6] & 0x1fU);
    const std::uint8_t release = DecodeAicaEnvelopeRate(60, 15, entry[7] & 0x1fU);
    state.attackRate = AicaAttackRate(attack);
    state.decay1Rate = 0.0f;
    state.decay2Rate = 0.0f;
    state.releaseRate = release <= 1 ? 60000.0f : AicaDecayRate(release);
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
    state.phase = 0;
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
    state.currentVolumeDb = (std::numeric_limits<float>::max)();
    state.phase = 1;
    d3t::debugLog("On %d", state.uid);
    ApplyAicaEnvelopeGain(state);
    const float frequency = state.instantFilterAttack && state.filterLevel0 == 0.0f ? 1.0f : std::pow(10.0f, state.currentFilterLevel * 4.0f) / 10000.0f;
    SetRawSoundEffectLowPassFrequency(GetRawSoundEffectVoice(state.voiceId), frequency, state.oneOverQ);
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
        volume *= g_musicVolume;
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

bool AssignAicaVoiceRelease(std::uint32_t physicalId) {
    AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
    if (!state || !state->phase)
        return false;
    if (state->phase != 4) {
        state->voiceId = physicalId;
        d3t::debugLog("Release %d", state->uid);
        state->phase = 4;
        state->filterElapsed = 0.0f;
    }
    return true;
}

void ApplyFilter(AicaState &envelope, float elapsedSeconds) {
    bool stopPlayback = false;
    switch (envelope.phase) {
    case 1:
        envelope.envelopeLevel = (std::min)(1.0f, envelope.envelopeLevel + elapsedSeconds * envelope.attackRate);
        if (envelope.envelopeLevel >= 1.0f) {
            envelope.phase = envelope.sustainLevel < 1.0f ? 2U : 3U;
            d3t::debugLog(envelope.phase == 2 ? "Decay1 %d" : "Decay2 %d", envelope.uid);
        }
        break;
    case 2:
        envelope.envelopeLevel = (std::max)(0.0f, envelope.envelopeLevel - elapsedSeconds * envelope.decay1Rate);
        if (envelope.currentVolumeDb <= -64.0f) {
            stopPlayback = true;
        } else if (envelope.envelopeLevel < envelope.sustainLevel) {
            envelope.envelopeLevel = envelope.sustainLevel;
            envelope.phase = 3;
            d3t::debugLog("Decay2 %d", envelope.uid);
        }
        break;
    case 3:
        envelope.envelopeLevel = (std::max)(0.0f, envelope.envelopeLevel - elapsedSeconds * envelope.decay2Rate);
        stopPlayback = envelope.currentVolumeDb <= -64.0f;
        break;
    case 4:
        envelope.envelopeLevel = (std::max)(0.0f, envelope.envelopeLevel - elapsedSeconds * envelope.releaseRate);
        stopPlayback = envelope.currentVolumeDb <= -64.0f;
        break;
    }
    if (stopPlayback) {
        d3t::debugLog("Off %d", envelope.uid);
        envelope.phase = 0;
        StopRawSoundEffectPlayback(GetRawSoundEffectVoice(envelope.voiceId));
    }
    if (!envelope.phase)
        return;
    UpdateAicaLowPassFilter(envelope.voiceId, envelope, elapsedSeconds);
    ApplyAicaEnvelopeGain(envelope);
}

void UpdateAicaEnvironmentEnvelopes(float elapsedSeconds = 1.0f / 30.0f) {
    for (std::uint32_t physicalId = 0; physicalId < 160; ++physicalId) {
        AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
        if (state && state->phase)
            ApplyFilter(*state, elapsedSeconds);
    }
}

void ApplyAicaFilters() {
    static std::chrono::steady_clock::time_point previousTick;
    static bool firstDelta = true;
    static double elapsedSeconds = 0.0;
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if (firstDelta) {
        previousTick = now;
        firstDelta = false;
    }
    const std::int64_t microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - previousTick).count();
    previousTick = now;
    const std::int32_t delta = static_cast<std::int32_t>(microseconds);
    elapsedSeconds += static_cast<float>((std::max)(delta, 0)) * 0.000001;
    if (elapsedSeconds >= 0.01) {
        UpdateAicaEnvironmentEnvelopes(static_cast<float>(elapsedSeconds));
        elapsedSeconds = 0.0;
    }
}

void StopReleasedAicaEnvelopesForOwner(std::int32_t owner) {
    for (std::uint32_t physicalId = 0; physicalId < 160; ++physicalId) {
        AicaState *const state = GetRawSoundEffectEnvelopeState(physicalId);
        if (state && state->phase == 4 && (state->ownerId == static_cast<std::uint32_t>(owner) || owner == -1)) {
            d3t::debugLog("** EnvelopeGenerator::StopAndReset **");
            d3t::debugLog("Off %d", state->uid);
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

    const std::uint8_t segmentCount = entry[12];
    const std::size_t curveOffset = 12U + entry[13];
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
    const std::size_t velocityOffset =
        static_cast<std::size_t>(read_unaligned<std::uint32_t>(bankData + 0x28)) + 132U + static_cast<std::size_t>(std::clamp(curveValue, 0, 255));
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
    const std::uint8_t segmentCount = entry[20];
    const std::size_t curveOffset = 12U + entry[21];
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
    if (!AicaBankPointerValid(bankData, bankSize, entry, entrySize) || entrySize < 22 || entry[21] == 0 || entry[17] == 0) {
        return 20000.0f;
    }

    const std::uint8_t segmentCount = entry[16];
    const std::size_t curveOffset = 12U + entry[17];
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
    const std::size_t entrySize = entry[0];
    if (!AicaBankPointerValid(bankData, bankSize, entry, entrySize) || entrySize < 22) {
        return;
    }

    AicaEnvironmentPartState &part = state.parts[partIndex];
    const std::int32_t decibels = AicaEnvironmentVolume(state, entry, value);
    const float volume = decibels > -64 ? std::pow(10.0f, static_cast<float>(decibels) / 20.0f) : 0.0f;
    std::int32_t pitchUnits = 0;
    AicaEnvironmentPitch(state, entry, value, &pitchUnits);
    const float frequencyRatio = std::pow(2.0f, (static_cast<float>(pitchUnits) * 3.0f * 0.0009765625f) / 12.0f);
    const float lowPassFrequency = MIDIToPitchHz(state, entry, value) / 20000.0f;
    if (part.active && (entry[10] & 2U) == 0) {
        if (part.voiceIndex < g_aicaPlaybackOwners.size() && g_aicaPlaybackOwners[part.voiceIndex]) {
            SetRawSoundEffectVolume(g_aicaPlaybackOwners[part.voiceIndex], volume);
            SetRawSoundEffectFrequencyRatio(g_aicaPlaybackOwners[part.voiceIndex], frequencyRatio);
            SetRawSoundEffectLowPassFrequency(g_aicaPlaybackOwners[part.voiceIndex], lowPassFrequency);
            const float outputLevel = static_cast<float>(g_aicaMemory->voicePool.slots[part.voiceIndex].field_10) / 127.0f;
            SetRawSoundEffectOutputMatrix(g_aicaPlaybackOwners[part.voiceIndex], &outputLevel, 1, 1);
        }
        return;
    }
    if (part.active && (entry[10] & 3U) == 3U)
        ReleaseAicaEnvironmentPart(stateIndex, partIndex);

    const std::uint16_t sampleId = read_unaligned<std::uint16_t>(entry + 2);
    if (sampleId < 0x100U || bankSize < 0x40)
        return;
    const std::size_t descriptorBase = static_cast<std::size_t>(read_unaligned<std::uint32_t>(bankData + 0x3c)) + 4U;
    const std::size_t descriptorOffset = descriptorBase + 16U * static_cast<std::size_t>(sampleId - 0x100U);
    const std::uint8_t *const descriptor = AicaBankOffset(bankData, bankSize, descriptorOffset, 16);
    if (!descriptor || read_unaligned<std::uint32_t>(descriptor + 8) == 1U)
        return;
    const std::uint32_t packedOffset = read_unaligned<std::uint32_t>(descriptor);
    const std::size_t sampleOffset = packedOffset & 0x007fffffU;
    const std::uint32_t audioBytes = read_unaligned<std::uint32_t>(descriptor + 12);
    if (!audioBytes || sampleOffset > bankSize || audioBytes > bankSize - sampleOffset) {
        return;
    }
    std::uint32_t loopBegin = 0;
    std::uint32_t loopLength = 0;
    const std::uint32_t sampleFormat = (packedOffset >> 23U) & 3U;
    if ((packedOffset & 0x02000000U) != 0) {
        loopBegin = read_unaligned<std::uint16_t>(descriptor + 4);
        std::uint32_t loopEnd = read_unaligned<std::uint16_t>(descriptor + 6);
        if (sampleFormat == 1U) {
            loopBegin >>= 1U;
            loopEnd >>= 1U;
        }
        if (loopEnd > loopBegin)
            loopLength = loopEnd - loopBegin;
    }

    const std::uint8_t allocatedVoice = AllocateAicaVoice(2);
    if (allocatedVoice == 0xff)
        return;
    const std::size_t voiceIndex = allocatedVoice;
    const float playbackPitch = sampleFormat == 1U ? frequencyRatio * 0.5f : frequencyRatio;
    void *const playback = PrepareRawSoundEffect(
        reinterpret_cast<const std::byte *>(bankData + sampleOffset), audioBytes, loopBegin, loopLength, 1, volume, playbackPitch, 44100, true);
    if (!playback) {
        ReturnAicaVoice(2, allocatedVoice);
        return;
    }
    constexpr float kInitialEnvironmentReverbLevel = 0.0f;
    SetRawSoundEffectOutputMatrix(playback, &kInitialEnvironmentReverbLevel, 1, 1);

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
    voice.stereo = 0;
    voice.field_10 = 0;
    voice.field_14 = 0;
    voice.pitchOrBaseNote = kAicaPanAttenuation[entry[8]];
    voice.xaudioVoiceId = RawSoundEffectVoiceId(playback);
    voice.xaudioStreamKind = 1;
    g_aicaPlaybackOwners[voiceIndex] = playback;
    SetRawSoundEffectLowPassFrequency(playback, lowPassFrequency);
    SetAttackFilterParamsForEnvironment(voice.xaudioVoiceId, entry, volume, g_aicaMemory->ownerId);
    StartRawSoundEffectPlayback(playback);
    voice.playState = 1;
    part.active = 1;
    part.flags = entry[10];
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
    const std::size_t tableOffset = read_unaligned<std::uint32_t>(bankData + 0x34);
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
    if (config >= 128 || voice.bank >= 16 || !g_aicaMemory->bankRegionLoaded[voice.bank]) {
        return false;
    }
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[voice.bank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[voice.bank];
    const auto *const table = reinterpret_cast<const std::uint8_t *>(voice.driverPointerTable);
    const auto *const note = reinterpret_cast<const std::uint8_t *>(voice.driverNoteTable);
    if (!AicaBankPointerValid(bankData, bankSize, table, 7) || !AicaBankPointerValid(bankData, bankSize, note, 12) || bankSize < 0x2c) {
        return false;
    }
    const std::uint32_t velocityTableOffset = read_unaligned<std::uint32_t>(bankData + 0x28);
    const std::size_t velocityOffset = static_cast<std::size_t>(velocityTableOffset) + 4U + 128U * static_cast<std::size_t>(table[6]) +
        static_cast<std::size_t>((voice.spawnCommand >> 8) & 0x7fU);
    if (velocityOffset >= bankSize)
        return false;

    const auto *const memory = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * config;
    std::uint32_t computed =
        (static_cast<std::uint32_t>(bankData[velocityOffset]) * (((static_cast<std::uint32_t>(note[8]) + note[9]) >> 1U) & 0x7ffffffeU)) >> 8U;
    if (computed)
        ++computed;
    computed = (2U * memory[0x8cc + configBase] * computed) >> 8U;
    if (computed)
        ++computed;
    computed = (2U * memory[0x8e2 + configBase] * computed) >> 8U;
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
    if (config >= 128 || voice.bank >= 16 || !g_aicaMemory->bankRegionLoaded[voice.bank]) {
        return;
    }
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[voice.bank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[voice.bank];
    const std::uint8_t *const memory = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * config;
    const std::uint8_t *const driverBase = read_unaligned<std::uint8_t *>(memory + 0x8f8 + configBase);
    if (!AicaBankPointerValid(bankData, bankSize, driverBase, 1) || !AicaBankPointerValid(bankData, bankSize, note, 12)) {
        return;
    }
    const std::int32_t pitchNote = driverBase[0] == 0 ? static_cast<std::int32_t>((voice.spawnCommand >> 16) & 0x7fU) - 60 : 0;
    const std::int32_t pitchUnits = static_cast<std::int32_t>(
        (static_cast<double>(static_cast<std::int32_t>(memory[0x8cf + configBase]) + static_cast<std::int8_t>(note[11])) * 1024.0 / 3.0 * 0.015625) +
        (static_cast<double>(static_cast<std::int32_t>(memory[0x8ce + configBase]) + static_cast<std::int8_t>(note[10]) + pitchNote) * 1024.0 / 3.0));
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
    if (config >= 128 || voice.bank >= 16 || !g_aicaMemory->bankRegionLoaded[voice.bank]) {
        return;
    }
    const std::uint8_t *const bankData = g_aicaMemory->bankSlots[voice.bank].bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[voice.bank];
    if (!AicaBankPointerValid(bankData, bankSize, note, 46))
        return;

    const auto *const memory = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * config;
    const std::int8_t authoredPan = static_cast<std::int8_t>(note[6]);
    const std::uint8_t pan = authoredPan >= 0 ? static_cast<std::uint8_t>(authoredPan) & 0x1fU : memory[0x8cd + configBase];
    std::int32_t center = static_cast<std::int32_t>((127.0f - note[9]) * -37.5f);
    std::int32_t middle = static_cast<std::int32_t>((127.0f - (static_cast<float>(note[8]) + note[9]) * 0.5f) * -37.5f);
    std::int32_t side = static_cast<std::int32_t>((127.0f - note[8]) * -37.5f);
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

} // namespace

std::int32_t DoesContainString(const char *left, const char *right) {
    for (;;) {
        std::int32_t a = static_cast<unsigned char>(*left++);
        if (a >= 'a' && a <= 'z')
            a -= 'a' - 'A';
        std::int32_t b = static_cast<unsigned char>(*right++);
        if (b >= 'a' && b <= 'z')
            b -= 'a' - 'A';
        if (a > b)
            return 1;
        if (a < b)
            return -1;
        if (a == 0)
            return 0;
    }
}

namespace {

bool EqualAudioName(const char *left, const char *right) {
    return DoesContainString(left, right) == 0;
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
    for (std::size_t driver = 0; driver < 128; ++driver) {
        const std::size_t base = 0x8c0 + 64 * driver;
        raw[base] = 0;
        raw[base + 2] = 0;
        raw[base + 0x28] = 64;
        raw[base + 0x29] = 0;
        raw[base + 0x2a] = 0;
        raw[base + 0x2b] = 0;
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
    std::memset(raw + 0x28c0, 0xff, 16 * 16 * 4);
    for (std::size_t owner = 0; owner < 16; ++owner)
        raw[0x2cc0 + 56 * owner] = 0;
}

void InitializeAicaMemory() {
    g_aicaMemory = &g_defaultAicaMemory;
    Aica_InitializeDriverValues();
    for (AicaEnvironmentState &state : g_aicaEnvironmentStates) {
        state.active = 0;
        state.volume = 127;
        state.control = 64;
        state.coarsePitch = 64;
        state.finePitch = 64;
        state.currentValue = 0;
        state.maximumPart = 0;
        state.field09 = 0;
        state.mixLevel = 0;
        state.field0b = 0;
        state.volumeOffset = 64;
        state.partTable = nullptr;
        for (AicaEnvironmentPartState &part : state.parts) {
            part.active = 0;
            part.flags = 0;
            part.voiceIndex = 0xff;
        }
    }
    g_aicaPlaybackPreBankLevels.fill(0);
    g_aicaPlaybackUsesBankVolume.fill(0);
    SetRawSoundEffectEnvelopeCallbacks(EnvelopeGenerator_StopAndReset_1, AssignAicaVoiceRelease);
    g_aicaInitialized = true;
}

std::int32_t FinishInitializingAICAMemory() {
    std::int32_t loadedBank = -1;
    std::size_t firstChange = 16;
    for (std::size_t bank = 0; bank < 15; ++bank) {
        if (g_aicaMemory->bankRegionLoaded[bank] == 1 && loadedBank < 0)
            loadedBank = static_cast<std::int32_t>(bank);
        if (g_aicaMemory->bankRegionSize[bank] != kAicaBankRegionCapacities[bank] && firstChange == 16)
            firstChange = bank;
    }
    for (std::size_t bank = 0; bank < 16; ++bank) {
        if ((kAicaBankRegionCapacities[bank] & 31U) != 0)
            return -2;
    }
    if (loadedBank >= static_cast<std::int32_t>(firstChange))
        return -2;
    std::uint32_t used = 0;
    for (std::size_t bank = 0; bank < 15; ++bank) {
        used += kAicaBankRegionCapacities[bank];
        std::uint8_t *const next = g_aicaMemory->bankRegionBase[bank] + kAicaBankRegionCapacities[bank];
        g_aicaMemory->bankRegionSize[bank] = kAicaBankRegionCapacities[bank];
        if (next + kAicaBankRegionCapacities[bank + 1] > g_aicaMemory->bankRegionBase[0] + g_aicaMemory->audioRamSize)
            return -1;
        g_aicaMemory->bankRegionBase[bank + 1] = next;
    }
    used += kAicaBankRegionCapacities[15];
    g_aicaMemory->bankRegionSize[15] = kAicaBankRegionCapacities[15];
    write_unaligned(g_aicaMemory->reserved_02dc + 4, g_aicaMemory->bankRegionBase[0] + used);
    write_unaligned(g_aicaMemory->reserved_02dc + 12, g_aicaMemory->audioRamSize - used);
    g_aicaMemory->reserved_0707 = std::byte{1};
    return 0;
}

void EnsureAicaMemory() {
    std::lock_guard lock(g_aicaCommandMutex);
    if (!g_aicaInitialized || !g_aicaMemory)
        InitializeAicaMemory();
}

void ResetAicaVoice(AicaVoiceSlot &voice) {
    const std::size_t index = static_cast<std::size_t>(&voice - g_aicaMemory->voicePool.slots);
    if (voice.xaudioStreamKind != -1)
        StopRawSoundEffectPlayback(g_aicaPlaybackOwners[index]);
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
    const std::uint8_t list = voice.status;
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
            ReleaseRawSoundEffectReservation(g_aicaPlaybackOwners[index]);
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

std::int32_t UnloadAicaBank(std::uint32_t bank) {
    std::lock_guard lock(g_aicaCommandMutex);
    return ReleaseOwnedAicaBankUnlocked(bank);
}

void ReleaseLoadedSoundBank(std::int32_t bank) {
    if (bank < 0 || bank >= 16)
        return;
    {
        std::lock_guard lock(g_aicaCommandMutex);
        if (!g_soundTable[bank][0])
            return;
    }
    if (bank <= 4) {
        for (std::int32_t environmentBank = 15; environmentBank >= 0; --environmentBank) {
            if (g_environmentAicaBanks[environmentBank].active != 1)
                continue;
            shenExecuteAICACommand(0xa6, static_cast<std::uint32_t>(environmentBank), 0);
            g_environmentAicaBanks[environmentBank].active = 0;
        }
    }
    {
        std::lock_guard lock(g_aicaCommandMutex);
        g_soundTable[bank][0] = '\0';
    }
    shenExecuteAICACommand(0x1100a0, static_cast<std::uint32_t>(bank), 0);
    UnloadAicaBank(static_cast<std::uint32_t>(bank));
}

void StopAicaPlaybackOwnersUnlocked() {
    for (void *&playback : g_aicaPlaybackOwners) {
        if (playback)
            StopRawSoundEffect(playback);
    }
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

std::int32_t SNDFunc(std::uint32_t command) {
    if (!g_aicaMemory || g_aicaMemory->commandQueueBusy)
        return -3;
    std::lock_guard lock(g_aicaCommandMutex);

    g_aicaMemory->commandQueueBusy = 0xff;
    std::int32_t result = -2;
    if (static_cast<std::int8_t>(command & 0xff) < 0) {
        const std::uint32_t packedCommand = ByteSwap32(command);
        if (g_aicaMemory->pendingQueueCount >= 0x80 || g_aicaMemory->pendingCommandRing[g_aicaMemory->pendingWriteIndex] != 0) {
            result = -1;
        } else {
            g_aicaMemory->pendingCommandRing[g_aicaMemory->pendingWriteIndex] = packedCommand;
            g_aicaMemory->pendingWriteIndex = (g_aicaMemory->pendingWriteIndex + 1) & 0x3f;
            ++g_aicaMemory->pendingQueueCount;
            result = 0;
        }
        PromoteQueuedAicaCommands();
        ExecuteAicaInsnForCycles();
    }
    g_aicaMemory->commandQueueBusy = 0;
    return result;
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

} // namespace

std::int32_t SOND_ExecuteRawAicaCommand(std::uint32_t command) {
    return SNDFunc(command);
}

std::int64_t ExecuteAicaCommand2Ch(std::int32_t bank, std::int32_t channel, std::uint32_t command, std::int16_t volume) {
    std::uint32_t packed = command + (((channel - 1) & 0xf) << 24) + (bank & 0x1f);
    if (command == 0 || command == 0xb0780000u) {
        return SNDFunc(ByteSwap32(packed));
    }

    if (command == 0xb00e0000u || command == 0xb0070000u || command == 0xb00d0000u || command == 0xb0270000u || ((command + 0x4fd30000u) & 0xfffeffffu) == 0) {
        packed += (static_cast<std::uint32_t>(volume) << 8) & 0x7f00;
    } else if (command == 0xa0050000u || command == 0xa0110000u || command == 0xb00a0000u || command == 0xb02a0000u) {
        packed += (static_cast<std::uint32_t>(static_cast<std::int32_t>(volume) * 0x100) - 0x4000) & 0x7f00;
    } else {
        return -2;
    }
    return SNDFunc(ByteSwap32(packed));
}

std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume) {
    const std::uint8_t lowCommand = static_cast<std::uint8_t>(command);
    if ((command & 0xf0) == 0xb0) {
        return static_cast<std::int32_t>(ExecuteAicaCommand2Ch(bank, 16, ByteSwap32(command), volume & 0x7f));
    }
    if (lowCommand >= 0xa8) {
        return SNDFunc(command);
    }
    if (lowCommand < 0xa5) {
        return static_cast<std::int32_t>(ExecuteAicaCommand(bank, ByteSwap32(command), volume & 0x7f));
    }
    if (bank > 15) {
        return lowCommand;
    }

    std::int32_t result = lowCommand;
    std::int32_t stateIndex = -1;
    if (command == 0xa6)
        stateIndex = 1;
    if (command == 0x10a5)
        stateIndex = 2;
    if (command == 0x20a5)
        stateIndex = 3;
    if (stateIndex >= 0) {
        auto *const stateBytes = reinterpret_cast<std::int8_t *>(&g_environmentAicaBanks[bank]);
        result = stateBytes[stateIndex];
        if (result == volume) {
            return result;
        }
        stateBytes[stateIndex] = static_cast<std::int8_t>(volume);
    }
    if (g_environmentAicaBanks[bank].active == 1 || command == 0x60a5) {
        return static_cast<std::int32_t>(ExecuteEnvironmentAicaCommand(bank, ByteSwap32(command), volume));
    }
    return result;
}

void Execute3DSoundCommand(
    std::uint32_t baseCommand, std::uint32_t bank, const Vector3 *position, float minimumDistance, float fadeDistance, std::uint32_t releaseVolume) {
    const Vector3 listener = ThreeDimensionalSoundListenerPosition();
    const float deltaX = listener.x - position->x;
    const float deltaZ = listener.z - position->z;
    const float distance = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);

    std::int32_t volume = 0;
    if (minimumDistance > distance) {
        volume = 127;
    } else if (distance <= minimumDistance + fadeDistance) {
        const double remaining = static_cast<double>(fadeDistance - (distance - minimumDistance));
        volume = static_cast<std::int32_t>(remaining * (127.0 / static_cast<double>(fadeDistance)));
    } else if (baseCommand != 0xffffffffU) {
        return;
    }

    const std::int32_t inverseCameraYaw = -g_fCamAccumYaw;
    const std::uint16_t sourceDirection = SoundFixedAtan2(-(position->x - listener.x), -(position->z - listener.z));
    const std::uint16_t relativeDirection = static_cast<std::uint16_t>(0x4000U - static_cast<std::uint16_t>(inverseCameraYaw) + sourceDirection);
    const std::uint16_t foldedDirection = static_cast<std::int16_t>(relativeDirection) < 0 ? static_cast<std::uint16_t>(~relativeDirection) : relativeDirection;
    const std::int16_t centeredDirection = static_cast<std::int16_t>(static_cast<std::uint16_t>(foldedDirection - 0x4000U));
    std::int32_t pan = -((static_cast<std::int32_t>(centeredDirection) * 64) >> 14);
    if (pan > 40)
        pan = 40;
    if (pan < -40)
        pan = -40;

    if (baseCommand == 0xffffffffU) {
        shenExecuteAICACommand(0x10a5U, bank, volume);
        if (volume == 0)
            return;
        shenExecuteAICACommand(0x20a5U, bank, pan);
        shenExecuteAICACommand(0xa6U, bank, static_cast<std::int32_t>(releaseVolume));
        return;
    }

    shenExecuteAICACommand(0x27b0U, bank, volume);
    shenExecuteAICACommand(0x2ab0U, bank, pan);
    shenExecuteAICACommand(baseCommand, 0, 0);
    shenExecuteAICACommand(0x2ab0U, bank, 0);
    shenExecuteAICACommand(0x27b0U, bank, 127);
}
void SNDWrapper(std::uint32_t command, std::uint32_t bank, Character *character) {
    if (command >= 0x00ffffffU) {
        Shenmue_ExecuteAicaCommand(command, character);
        return;
    }
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    Execute3DSoundCommand(command, bank, &position, 2.0f, 20.0f, 0);
}

void SOND_EmitVFSynchronizedReactionSound(Character *character) {
    if (!character)
        return;
    const Vector3 position = character->entity.node.position;
    Execute3DSoundCommand(0x1e76abU, 5U, &position, 2.0f, 20.0f, 0);
}

void Script_SetSequenceAudioVolume(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const SequenceAudioVolumeParameters *>(rawParams);
    if (!params || !((g_sceneId == 1 && g_areaId == SHEN_NAME(OP02)) || (S1SettingsFlags_Get() & 2U) != 0)) {
        return;
    }

    CVWKRuntimeSlot *slot = nullptr;
    if (params->slotOffset != 0 && g_pCurrentTaskData) {
        slot = reinterpret_cast<CVWKRuntimeSlot *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(params->slotOffset));
    }

    if (params->mode == 0) {
        ProcessAudioData(slot, params->value);
        return;
    }
    if (params->mode != 1)
        return;

    const auto apply = [value = static_cast<std::uint32_t>(params->value)](CVWKRuntimeSlot &current) { SetDialogueVoiceLifetime(current.audioContext, value); };
    if (slot) {
        apply(*slot);
        return;
    }
    if (!g_pCVWK)
        return;
    for (CVWKRuntimeSlot &current : g_pCVWK->slots) {
        if (current.callback)
            apply(current);
    }
}

void Script_Execute3DSoundCommand_Wrapper(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const ThreeDimensionalSoundParameters *>(rawParams);
    const Vector3 *position = nullptr;
    if (params->positionOffset) {
        position = reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(params->positionOffset));
    }
    Execute3DSoundCommand(params->baseCommand, params->bank, position, params->minimumDistance, params->fadeDistance, params->releaseVolume);
}
std::int32_t ExecuteAICACommand_Wrapper(std::uint32_t volume) {
    std::int32_t bank = 15;
    while (bank >= 0 && g_environmentAicaBanks[bank].active)
        --bank;
    if (bank < 0)
        return -1;

    g_environmentAicaBanks[bank] = {.active = 1};
    shenExecuteAICACommand(0xa5, static_cast<std::uint32_t>(bank), static_cast<std::int32_t>(volume));
    g_environmentAicaBankVolumes[bank] = static_cast<std::int16_t>(volume);
    return bank;
}
void ReleaseEnvironmentAicaBank(std::uint32_t bank) {
    if (bank > 15)
        return;
    shenExecuteAICACommand(0xa6, bank, 0);
    g_environmentAicaBanks[bank].active = 0;
}
std::int32_t LoadSndBankIntoAicaRam(const std::uint8_t *data, std::int32_t bank) {
    const std::uint8_t units = data[16];
    if (static_cast<std::uint32_t>(g_aicaMemory->loadedBankUnits) + units > 128U)
        return 0;
    g_aicaMemory->loadedBankUnits += units;
    if (g_aicaMemory->bankRegionLoaded[bank] || bank < 0)
        return 0;
    std::uint8_t *const destination = g_aicaMemory->bankRegionBase[bank];
    const std::size_t bytes = read_unaligned<std::uint32_t>(data + 8) & ~3U;
    std::memcpy(destination, data, bytes);
    AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    g_aicaMemory->bankRegionLoaded[bank] = 1;
    slot.bankData = destination;
    slot.loadedSize = (slot.loadedSize & 0xff000000U) | (read_unaligned<std::uint32_t>(data + 16) & 0x00ffffffU);
    slot.bankIdOrType = read_unaligned<std::int32_t>(data + 4);
    g_aicaMemory->globalPanLeft = 0;
    g_aicaMemory->globalPanRight = 0;
    InitializeLoadedAicaBank(static_cast<std::uint8_t>(bank));
    return 0;
}

std::int32_t LoadSndBankIntoAicaRamLocked(const std::uint8_t *data, std::uint32_t bank) {
    if (bank >= 16)
        return -1;
    std::lock_guard lock(g_aicaCommandMutex);
    LoadSndBankIntoAicaRam(data, static_cast<std::int32_t>(bank));
    return 0;
}

void SNDD(const char *filename, std::int32_t bytes) {
    void *const workspace = HLib_AllocHeapMemBlock(bytes, SHEN_NAME(SNDD));
    d3t::LargeFileHandle *handle = LargeFile_OpenPathHandle(filename);
    if (!handle)
        handle = LargeFile_OpenPathHandle(filename);
    if (!handle)
        return;
    GetFileFromCachedBuffer(handle, static_cast<std::uint64_t>(bytes / 2048), workspace, 0);
    for (;;) {
        if (handle->entryIndex < 0 || handle->entryIndex >= 2048)
            break;
        const d3t::LargeFileDataEntry &entry = d3t::g_largeFileData[static_cast<std::size_t>(handle->entryIndex)];
        if (!entry.inUse || !d3t::Archive_HasPendingRead(entry.archiveHandle))
            break;
    }
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(workspace), 0);
}

void ReadAndExecuteAicaForBGM(const char *directory, const char *filename, std::int32_t bank) {
    std::uint8_t *const data = static_cast<std::uint8_t *>(ReadFile(directory, filename, SHEN_NAME(SND1)));
    SNDFunc(ByteSwap32((static_cast<std::uint32_t>(bank) & 0x1fU) - 0x5fffef00U));
    {
        std::lock_guard lock(g_aicaCommandMutex);
        ReleaseOwnedAicaBankUnlocked(static_cast<std::size_t>(bank));
    }
    LoadSndBankIntoAicaRamLocked(data, static_cast<std::uint32_t>(bank));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
    std::strcpy(g_soundTable[bank], filename);
    ExecuteAicaCommandLoop(bank);
}

void sondLoadSndFromDirIntoBank(std::int32_t sceneId, std::int32_t bank, const char *filename) {
    if (!filename || bank < 0 || bank > 15)
        return;
    EnsureAicaMemory();

    char selectedFilename[16]{};
    if (EqualAudioName(filename, "SELECT")) {
        std::int32_t step = SCEN_GetCurrentScenarioStep();
        if (g_actNumber == 1 && step >= 110) {
            step = 0;
        } else if (g_actNumber == 2 && static_cast<std::uint32_t>(step - 110) > 89U) {
            step = 110;
        } else if (g_actNumber == 3 && step < 200) {
            step = 200;
        }
        std::snprintf(selectedFilename, sizeof(selectedFilename), "fre%02d00.snd", step / 10);
        filename = selectedFilename;
    } else if (g_usingJapaneseAudio == 1 && std::strcmp(filename, "battle_1.snd") == 0) {
        filename = "battle_1_jp.snd";
    }

    if (EqualAudioName(filename, "FREE")) {
        ReleaseLoadedSoundBank(bank);
        return;
    }
    {
        std::lock_guard lock(g_aicaCommandMutex);
        if (EqualAudioName(g_soundTable[bank], filename)) {
            return;
        }
    }
    shenExecuteAICACommand(0x1100a0, static_cast<std::uint32_t>(bank), 0);
    if (bank <= 4) {
        for (std::int32_t environmentBank = 15; environmentBank >= 0; --environmentBank) {
            if (g_environmentAicaBanks[environmentBank].active != 1)
                continue;
            shenExecuteAICACommand(0xa6, static_cast<std::uint32_t>(environmentBank), 0);
            g_environmentAicaBanks[environmentBank].active = 0;
        }
    }
    {
        std::lock_guard lock(g_aicaCommandMutex);
        ReleaseOwnedAicaBankUnlocked(static_cast<std::size_t>(bank));
    }
    char directory[32]{};
    if (sceneId >= 0)
        std::snprintf(directory, sizeof(directory), "scene/%02d/sound", sceneId);
    else
        std::strcpy(directory, "misc");
    std::uint64_t largestFreePayload = 0;
    for (HLib::TaskData *block = g_pFirstTaskData; block; block = block->next_ptr) {
        const std::uintptr_t payload = reinterpret_cast<std::uintptr_t>(block + 1);
        if ((payload & 0x1fU) != 0)
            break;
        if (block->status != make_shen_name('U', 'S', 'E', ' ') || block->taskToken == 0) {
            const std::uint32_t bytes = static_cast<std::uint32_t>(block->content_size) - sizeof(HLib::TaskData);
            if (bytes > largestFreePayload)
                largestFreePayload = bytes;
        }
        if (!block->next_ptr || block->next_ptr == g_pLastTaskData)
            break;
    }
    const std::uint32_t size = static_cast<std::uint32_t>(shenOpenFile(directory, filename));
    if (size) {
        if (size > largestFreePayload)
            SNDD(filename, static_cast<std::int32_t>(largestFreePayload & 0xfffff800U));
        else
            ReadAndExecuteAicaForBGM(directory, filename, bank);
    }
}

namespace {


constexpr std::array<const char *, 4> kFrontendSoundFiles{"bgm110.snd", "spp004.snd", "TITLE4_A.GZ", "OPTION.GZ"};
constexpr std::array<std::uint32_t, 2> kFrontendSoundCommands{0x3fa8U, 0x3fa8U};
constexpr std::array<std::int32_t, 2> kFrontendSoundVolumes{0x75, 0x75};

} // namespace



std::int32_t SND_SelectFreeWeird(std::int32_t id) {
    const std::size_t index = static_cast<std::size_t>(id);
    sondLoadSndFromDirIntoBank(-1, 4, "FREE");
    sondLoadSndFromDirIntoBank(-1, 2, kFrontendSoundFiles[index]);
    shenExecuteAICACommand(kFrontendSoundCommands[index], 0, 0);
    return shenExecuteAICACommand(0x4a0U, 2, kFrontendSoundVolumes[index]);
}

void SND_LoadSND(const std::array<const char *, 8> &filenames) {
    for (std::int32_t bank = 7; bank >= 0; --bank) {
        const char *const filename = filenames[static_cast<std::size_t>(bank)];
        bool changed = false;
        if (filename) {
            std::lock_guard lock(g_aicaCommandMutex);
            changed = !EqualAudioName(filename, g_soundTable[bank]);
        }
        if (changed)
            ReleaseLoadedSoundBank(bank);
    }
    for (std::int32_t bank = 0; bank < 8; ++bank) {
        const char *const filename = filenames[static_cast<std::size_t>(bank)];
        if (!filename)
            continue;
        if (EqualAudioName(filename, "FREE"))
            ReleaseLoadedSoundBank(bank);
        else
            sondLoadSndFromDirIntoBank(g_sceneId, bank, filename);
    }
}

void SNDD_Parse(HLib::TaskData *snddBlock) {
    if (!snddBlock)
        return;
    const auto *const chunk = reinterpret_cast<const std::byte *>(snddBlock);
    const std::uint32_t chunkSize = read_unaligned<std::uint32_t>(chunk + 4);
    if (chunkSize < 12)
        return;
    const std::byte *const payload = chunk + 8;
    const std::byte *const end = chunk + chunkSize;
    const auto canRead = [end](const std::byte *bytes, std::size_t size) { return bytes <= end && size <= static_cast<std::size_t>(end - bytes); };
    const auto readI32 = [](const std::byte *bytes) { return read_unaligned<std::int32_t>(bytes); };
    const auto storyFlag = [](std::int32_t index) {
        return index >= 0 && index < 1024 &&
            (g_shenmueFlags[static_cast<std::size_t>(index) >> 3U] & static_cast<std::uint8_t>(1U << (static_cast<std::uint32_t>(index) & 7U))) != 0;
    };
    auto compare = [](std::int32_t kind, std::int32_t left, std::int32_t right) {
        switch (kind) {
        case 0:
            return left == right;
        case 1:
            return left <= right;
        case 2:
            return left >= right;
        case 3:
            return left < right;
        case 4:
            return left > right;
        case 5:
            return left != right;
        default:
            return false;
        }
    };

    const std::byte *cursor = payload;
    for (;;) {
        if (!canRead(cursor, sizeof(std::int32_t)))
            return;
        const std::int32_t opcode = readI32(cursor);
        cursor += sizeof(std::int32_t);
        if (opcode == 0)
            return;
        if (opcode == 1 && !canRead(cursor, 20))
            return;
        if (opcode == 1) {
            const std::int32_t bank = readI32(cursor);
            char filename[17]{};
            std::memcpy(filename, cursor + 4, 16);
            sondLoadSndFromDirIntoBank(g_sceneId, bank, filename);
            cursor += 20;
            continue;
        }
        if ((opcode == 2 || opcode == 3) && !canRead(cursor, 12)) {
            return;
        }
        if (opcode == 2) {
            shenExecuteAICACommand(static_cast<std::uint32_t>(readI32(cursor)), 0, 0);
            shenExecuteAICACommand(0x4a0U, static_cast<std::uint32_t>(readI32(cursor + 4)), readI32(cursor + 8));
            cursor += 12;
            continue;
        }
        if (opcode == 3) {
            shenExecuteAICACommand(static_cast<std::uint32_t>(readI32(cursor)), static_cast<std::uint32_t>(readI32(cursor + 4)), readI32(cursor + 8));
            cursor += 12;
            continue;
        }
        if (opcode == -2) {
            if (!canRead(cursor, sizeof(std::int32_t)))
                return;
            const std::ptrdiff_t relativeOffset = readI32(cursor);
            const std::ptrdiff_t currentOffset = cursor - payload;
            const std::ptrdiff_t payloadSize = end - payload;
            if (relativeOffset < -currentOffset || relativeOffset > payloadSize - currentOffset) {
                return;
            }
            cursor += relativeOffset;
            continue;
        }
        if (opcode != -1)
            continue;

        std::int32_t comparison = 0;
        std::int32_t conditionMode = g_sceneId;
        std::int32_t values[2]{};
        std::size_t valueCount = 0;
        for (;;) {
            if (!canRead(cursor, sizeof(std::int32_t)))
                return;
            const std::int32_t token = readI32(cursor);
            cursor += sizeof(std::int32_t);
            if (token < 0)
                break;
            if (token < 6) {
                comparison = token;
                continue;
            }
            if (token < 16) {
                conditionMode = token;
                continue;
            }

            std::int32_t value = 0;
            bool hasValue = true;
            if ((token == 16 || token == 17) && !canRead(cursor, sizeof(std::int32_t))) {
                return;
            }
            switch (token) {
            case 16:
                value = readI32(cursor);
                cursor += sizeof(std::int32_t);
                break;
            case 17:
                value = storyFlag(readI32(cursor)) ? 1 : 0;
                cursor += sizeof(std::int32_t);
                break;
            case 18:
                value = static_cast<std::int32_t>(g_entryPoint);
                break;
            case 19:
                value = static_cast<std::int8_t>(g_currentDateTime.month);
                break;
            case 20:
                value = static_cast<std::int8_t>(g_currentDateTime.day);
                break;
            default:
                hasValue = false;
                break;
            }
            if (hasValue) {
                if (valueCount < 2)
                    values[valueCount] = value;
                ++valueCount;
            }
        }
        if (valueCount < 2) {
            values[1] = 0;
            comparison = 5;
        }
        bool condition = compare(comparison, values[0], values[1]);
        if (conditionMode == 6)
            condition = !condition;
        if (!canRead(cursor, sizeof(std::int32_t)))
            return;
        if (condition) {
            cursor += sizeof(std::int32_t);
        } else {
            const std::ptrdiff_t relativeOffset = readI32(cursor);
            const std::ptrdiff_t currentOffset = cursor - payload;
            const std::ptrdiff_t payloadSize = end - payload;
            if (relativeOffset < -currentOffset || relativeOffset > payloadSize - currentOffset) {
                return;
            }
            cursor += relativeOffset;
        }
    }
}
void ConfigureAmbientSound(std::int32_t index, std::int32_t value, const void *source, float minimumDistance, float fadeDistance, float spatialBlend,
    std::uint32_t weatherMode, const void *halfHourVolume) {
    if (!g_pAMB1 || index < 1 || index > 16)
        return;
    auto *const entries = static_cast<AmbientSoundEntry *>(g_pAMB1);
    AmbientSoundEntry &entry = entries[index - 1];
    entry.active = 1;
    entry.fixedPan = value;
    entry.position = static_cast<const Vector3 *>(source);
    entry.minimumDistance = minimumDistance;
    entry.fadeDistance = fadeDistance;
    entry.spatialBlend = spatialBlend;
    entry.weatherMode = weatherMode;
    entry.halfHourVolume = static_cast<const float *>(halfHourVolume);
}
std::int32_t ExecuteAicaCommandsForAMB1() {
    g_pAMB1Tmp = g_pAMB1;
    auto *const entries = static_cast<AmbientSoundEntry *>(g_pAMB1);
    std::int32_t result = 0;
    for (std::int32_t channel = 1; channel <= 16; ++channel) {
        ExecuteAicaCommand2Ch(1, channel, 0xb0070000u, 0);
        result = shenExecuteAICACommand(0x28a0, 1, channel);
        entries[channel - 1].currentVolume = 0;
        entries[channel - 1].currentPan = 0;
        entries[channel - 1].currentCrossfeed = 0;
    }
    return result;
}
void Shenmue_ExecuteAicaCommand(std::uint32_t command, Character *character) {
    if (!character)
        return;
    CharacterComponentNode *const stepComponent = HLib_GetCharacterComponent(character, SHEN_NAME(STEP));
    if (!stepComponent)
        return;
    const auto *const step = reinterpret_cast<const std::uint8_t *>(stepComponent + 1);
    if (step[1] == 0)
        return;

    MotionManagerWork *const manager = MOTM_GetWork(character);
    const float x = manager->motion.rootWorldPosition.x;
    const float z = manager->motion.rootWorldPosition.z;
    std::uint16_t soundIndex = CTRL_EvaluateWeatherSoundEnvironment(character, x, z);
    if (soundIndex == 0) {
        soundIndex = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(step[2])));
    }

    std::int32_t soundCommand = kSndCommands[soundIndex];
    if (soundCommand < 0)
        soundCommand &= 0x7fffffff;
    const std::uint32_t type = command & 0x7fffffU;
    if (type == 6U || type == 7U) {
        soundCommand += 0x30000;
    } else {
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        soundCommand += static_cast<std::int32_t>((((g_EnvPrngSeed >> 16U) & 0x7fffU) % 3U) << 16U);
    }
    shenExecuteAICACommand(static_cast<std::uint32_t>(soundCommand), 0, 0);
}

void ExecuteAmbientSoundCommand(std::int32_t mode, std::uint32_t command) {
    switch (mode) {
    case 0: {
        shenExecuteAICACommand(0x9a0, 1, 25);
        shenExecuteAICACommand(command, 0, 0);
        ExecuteAicaCommandsForAMB1();
        return;
    }
    case 1:
        shenExecuteAICACommand(0x9a0, 1, 50);
        shenExecuteAICACommand(command, 0, 0);
        for (std::int32_t channel = 3; channel >= 1; --channel) {
            ExecuteAicaCommand2Ch(3, channel, 0xb0070000u, 0);
        }
        g_performedAicaCommand = 1;
        return;
    case 2: {
        auto *const entries = static_cast<AmbientSoundEntry *>(g_pAMB1Tmp);
        if (!entries)
            return;
        const std::int32_t first = command <= 16 ? static_cast<std::int32_t>(command) : 16;
        const std::int32_t last = command <= 16 ? first : 1;
        if (first < 1)
            return;
        for (std::int32_t channel = first; channel >= last; --channel) {
            AmbientSoundEntry &entry = entries[channel - 1];
            entry.active = 0;
            entry.currentVolume = 0;
            entry.currentPan = 127;
            entry.currentCrossfeed = -1;
            ExecuteAicaCommand2Ch(1, channel, 0xb0070000u, 0);
        }
        return;
    }
    case 3: {
        auto *const entries = static_cast<AmbientSoundEntry *>(g_pAMB1Tmp);
        if (entries && command >= 1 && command <= 16 && entries[command - 1].position) {
            entries[command - 1].active = 1;
        }
        return;
    }
    default:
        return;
    }
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
    const auto publish = [&](std::uint8_t *driver) { write_unaligned(memory + 0x8f8 + voiceBase, driver); };
    publish(nullptr);

    const AicaBankSlot &slot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = slot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!g_aicaMemory->bankRegionLoaded[bank] || !bankData || bankSize < 0x34) {
        return;
    }

    std::uint8_t *driver = nullptr;
    if ((memory[0x8c1 + voiceBase] & 2U) != 0) {
        const std::size_t offset = read_unaligned<std::uint32_t>(bankData + 0x30);
        if (offset != 0)
            driver = AicaBankOffset(bankData, bankSize, offset, 16);
    } else {
        const std::size_t partTableOffset = read_unaligned<std::uint32_t>(bankData + 0x24);
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
    const std::size_t assignmentBase = 4 * (channel + 16 * (static_cast<std::uint32_t>(bank) + 0xa3));
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
    const std::size_t partTableOffset = read_unaligned<std::uint32_t>(bankData + 0x24);
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
    const std::size_t assignmentOffset = 4 * (channel + 16 * (bank + 0xa3));
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
    const std::size_t tableOffset = read_unaligned<std::uint32_t>(bankData + 0x24);
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
    const std::size_t assignmentBase = 4 * (channel + 16 * (bank + 0xa3));
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
                std::uint8_t *const driver = read_unaligned<std::uint8_t *>(memory + 0x8f8 + voiceBase);
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
        case 0xb007:
            memory[0x8cc + voiceBase] = volume;
            break;
        case 0xb00a: {
            const std::uint8_t quarterVolume = volume >> 2;
            memory[0x8cd + voiceBase] = quarterVolume < 16 ? static_cast<std::uint8_t>(31 - quarterVolume) : static_cast<std::uint8_t>(quarterVolume - 16);
            break;
        }
        case 0xb00d:
        case 0xb00e:
            break;
        case 0xb040:
            memory[0x8e9 + voiceBase] = volume < 0x40 ? 0 : volume;
            break;
        case 0xc000:
            memory[0x8c5 + voiceBase] = secondaryValue;
            break;
        case 0xe000:
            memory[0x8e8 + voiceBase] = volume;
            break;
        default:
            break;
        }
    }
    return result;
}

void ExecuteAicaCommandLoop(std::int32_t index) {
    if (!g_aicaNeedsBankCleanup[0] || index == 0) {
        return;
    }
    for (std::int32_t bank = 7; bank >= index; --bank) {
        shenExecuteAICACommand(0x10a0, bank, 0);
        if (bank <= 4) {
            shenExecuteAICACommand(0x28a0, bank, 16);
        }
    }
}

void SetVolumeForAicaBank(float volume, std::uint32_t bank) {
    if (bank == 2) {
        return;
    }
    constexpr std::int32_t kBaseVolumes[8]{};
    const float attenuation = (1.0f - volume) * -64.0f;
    if (bank == 1) {
        const auto environmentVolume = static_cast<std::int8_t>(attenuation < -64.0f ? -64.0f : attenuation);
        for (std::uint32_t environmentBank = 0; environmentBank < 16; ++environmentBank) {
            shenExecuteAICACommand(0x60a5, environmentBank, environmentVolume);
        }
    }
    const float adjusted = static_cast<float>(kBaseVolumes[bank]) + attenuation;
    const auto clamped = static_cast<std::int8_t>(adjusted < -64.0f ? -64.0f : adjusted);
    ExecuteAicaCommand(bank, 0xa0110000u, clamped);
    ExecuteAicaCommand(bank, 0xa0050000u, clamped);
}

void InitAMB1() {
    constexpr std::int32_t kAmb1 = SHEN_NAME(AMB1);
    constexpr std::int64_t kBlockSize = 0x3c0;

    shenExecuteAICACommand(0x1200a0, 1, 0);
    HLib::TaskData *block = g_pFirstTaskData;
    void *payload = nullptr;
    while (block) {
        HLib::TaskData *const next = block->next_ptr;
        if (block->status != HLIB_USED_TOKEN.name && block->content_size >= kBlockSize) {
            const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
            if (oldSize > 0x400) {
                auto *const remainder = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + static_cast<std::size_t>(kBlockSize));
                remainder->prev_ptr = block;
                remainder->next_ptr = next;
                remainder->status = HLIB_FREE_TOKEN.name;
                remainder->alignment = 0;
                remainder->taskToken = HLIB_FREE_TOKEN.name;
                remainder->content_size = static_cast<std::int64_t>(oldSize - kBlockSize);
                block->next_ptr = remainder;
                if (next)
                    next->prev_ptr = remainder;
                block->content_size = kBlockSize;
            }
            block->taskToken = kAmb1;
            block->status = HLIB_USED_TOKEN.name;
            block->alignment = 0;
            payload = block + 1;
            break;
        }
        block = next;
    }

    g_pAMB1 = payload;
    g_pAMB1Tmp = nullptr;
    if (payload)
        std::memset(payload, 0, 0x380);
}

void SOND_UpdateAICA() {
    if (g_soundTaskState) {
        if (g_soundTaskState == 1) {
            std::size_t offset = 0;
            do {
                g_soundTable[g_soundIndex][offset] = g_soundOffsetName[offset];
            } while (g_soundOffsetName[offset++] != '\0');
            ExecuteAicaCommandLoop(g_soundIndex);
            HLib_FreeTaskData(g_soundTaskData, 0);
            g_soundTaskData = nullptr;
            g_soundTaskState = -1;
        }
        return;
    }

    if (CheckFileInfo(g_soundPath) == 3) {
        g_soundDataNumber = 1;
    }
    g_soundFlag = 1;
    if (g_soundDataNumber) {
        ReadCachedFile(g_soundPath);
        ++g_soundTaskState;
    }
}

void ProcessSoundForCewpHandler() {
    if (!g_performedAicaCommand) {
        return;
    }
    g_currentAicaCommandLoops = 0;
    ForEachCewpEntityApplyCallback(0, PerformAicaSoundCallback);
    const std::int32_t loops = g_currentAicaCommandLoops;
    if (g_performedAicaCommand == loops) {
        return;
    }

    constexpr std::int32_t thresholds[3]{0, 4, 7};
    for (std::int32_t channel = 3; channel != 0; --channel) {
        const std::int16_t volume = thresholds[channel - 1] >= loops ? 0 : 127;
        ExecuteAicaCommand2Ch(3, channel, 0xb0070000u, volume);
    }
    g_performedAicaCommand = loops;
}

void ExecuteAicaSequenceStopCommand(std::uint32_t command) {
    if ((command & 0xff00U) > 0x1900U)
        return;
    std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::uint8_t bank = static_cast<std::uint8_t>(command & 15U);
    const bool qualified = (command & 0x80U) != 0;
    for (std::size_t index = 0; index < 16; ++index) {
        const std::size_t base = 0x2cbc + 56 * index;
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
        const std::size_t base = 0x2cbc + 56 * index;
        if (raw[base + 4] && (raw[base + 5] & 0xc0U) && raw[base + 6] == (command & 15U)) {
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
                const std::size_t base = 0x2cbc + 56 * index;
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
        const std::size_t tableOffset = read_unaligned<std::uint32_t>(bankData + 0x2c);
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
                const std::size_t base = 0x2cbc + 56 * index;
                if (raw[base + 4] && (raw[base + 5] & 0x0fU) == key) {
                    selected = index;
                    break;
                }
            }
        } else if (driverKind >= 0) {
            for (std::size_t index = 0; index < 16; ++index) {
                const std::size_t base = 0x2cbc + 56 * index;
                if (raw[base + 4] && raw[base + 6] == bank && static_cast<std::int8_t>(raw[base + 5]) >= 0) {
                    selected = index;
                    break;
                }
            }
        }
        if (selected == 16) {
            for (std::size_t index = 0; index < 16; ++index) {
                if (!raw[0x2cbc + 56 * index + 4]) {
                    selected = index;
                    break;
                }
            }
        }
        if (selected == 16) {
            for (std::size_t index = 0; index < 16; ++index) {
                if (static_cast<std::int8_t>(raw[0x2cbc + 56 * index + 5]) < 0) {
                    selected = index;
                    break;
                }
            }
        }
        if (selected == 16)
            continue;

        const std::size_t base = 0x2cbc + 56 * selected;
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
        const std::size_t base = 0x2cbc + 56 * owner;
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
            current += 4 * step;
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
            if (stream == kAicaEndStream.data()) {
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
            if (config < 128 && memory[0x8e9 + 64U * config]) {
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
            if (voice.playState == 1) {
                StopRawSoundEffectPlayback(g_aicaPlaybackOwners[playbackIndex]);
            }
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
            const auto *const fadeBytes = reinterpret_cast<const std::uint8_t *>(kAicaFadeTable.data());
            reinterpret_cast<std::uint8_t *>(g_aicaMemory)[0x751 + 24U * bank] = fadeBytes[32U + requested];
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
    if (!g_aicaMemory || !driver || voiceIndex >= 160)
        return 0;
    const std::uint32_t bank = g_aicaMemory->ownerId;
    if (bank >= 16 || !g_aicaMemory->bankRegionLoaded[bank])
        return 0;
    AicaBankSlot &bankSlot = g_aicaMemory->bankSlots[bank];
    std::uint8_t *const bankData = bankSlot.bankData;
    const std::size_t bankSize = g_aicaMemory->bankRegionSize[bank];
    if (!bankData || bankSize < 0x40)
        return 0;
    const auto contains = [&](const void *pointer, std::size_t bytes) {
        const auto *const address = static_cast<const std::uint8_t *>(pointer);
        return address >= bankData && static_cast<std::size_t>(address - bankData) <= bankSize &&
            bytes <= bankSize - static_cast<std::size_t>(address - bankData);
    };

    auto *const memory = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
    const std::size_t configBase = 64U * voiceIndex;
    std::uint8_t *const driverBase = read_unaligned<std::uint8_t *>(memory + 0x8f8 + configBase);
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
        if (!contains(table, 64) || commandPart < table[2] || commandPart > table[3] || !contains(table + 64, 64)) {
            return 0;
        }
        note = table + 64;
        const std::uint16_t recordCount = read_unaligned<std::uint16_t>(table);
        for (std::uint32_t index = 0; index < recordCount && note[0] < commandNote; ++index) {
            if (!contains(note + 64, 64))
                return 0;
            note += 64;
        }
        pitchNote = static_cast<std::int32_t>(commandNote) - 60;
    } else if (driverKind == 1) {
        if (!contains(table, 66) || commandNote < table[64] || commandNote > table[65]) {
            return 0;
        }
        const std::size_t offsetIndex = static_cast<std::size_t>(commandNote - table[64]);
        if (!contains(table + 66 + 2 * offsetIndex, 2))
            return 0;
        const std::uint16_t offset = read_unaligned<std::uint16_t>(table + 66 + 2 * offsetIndex);
        if (!offset || !contains(table + offset, 64))
            return 0;
        note = table + offset;
    } else {
        const std::uint32_t index = static_cast<std::uint32_t>(commandNote) | (8U * (g_aicaMemory->currentCommand & 0xf0U));
        if (!contains(table, 2) || index > read_unaligned<std::uint16_t>(table) || !contains(table + 64U * (index + 1U), 64)) {
            return 0;
        }
        note = table + 64U * (index + 1U);
    }
    if (!note || (driverKind != 0 && (commandPart < table[2] || commandPart > table[3]))) {
        return 0;
    }
    if (driverKind != 0) {
        const std::uint8_t exclusiveMode = note[34] & 0x83U;
        if (exclusiveMode == 0x82U) {
            for (const AicaVoiceSlot &active : g_aicaMemory->voicePool.slots) {
                if ((active.status == 1 || active.status == 2) && active.playFlags == 0x82U &&
                    (active.spawnCommand & 0x0f00000fU) == (g_aicaMemory->currentCommand & 0x0f00000fU) && active.layerFlags == note[35]) {
                    return 0;
                }
            }
        } else if (exclusiveMode == 0x80U || exclusiveMode == 0x81U) {
            for (AicaVoiceSlot &active : g_aicaMemory->voicePool.slots) {
                const bool matchesCommon = (active.status == 1 || active.status == 2) && active.playFlags == exclusiveMode;
                const bool matchesCommand = exclusiveMode == 0x80U ?
                    (active.spawnCommand & 0x0f7f00ffU) == (g_aicaMemory->currentCommand & 0x0f7f00ffU) :
                    (active.spawnCommand & 0x0f00000fU) == (g_aicaMemory->currentCommand & 0x0f00000fU) && active.layerFlags == note[35];
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
    std::uint32_t audioBytes = 0;
    std::uint32_t loopBegin = 0;
    std::uint32_t loopLength = 0;
    std::uint16_t channels = 1;
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
        const std::size_t descriptorBase = static_cast<std::size_t>(read_unaligned<std::uint32_t>(bankData + 0x3c)) + 4U;
        const std::size_t descriptorOffset = descriptorBase + 16U * static_cast<std::size_t>(sampleId - 0x100U);
        if (descriptorOffset > bankSize || 16U > bankSize - descriptorOffset) {
            return 0;
        }
        const std::uint8_t *const descriptor = bankData + descriptorOffset;
        const std::uint32_t packedOffset = read_unaligned<std::uint32_t>(descriptor);
        const std::uint32_t sampleOffset = packedOffset & 0x007fffffU;
        audioBytes = read_unaligned<std::uint32_t>(descriptor + 12);
        if (!audioBytes || sampleOffset > bankSize || audioBytes > bankSize - sampleOffset) {
            return 0;
        }
        sampleData = reinterpret_cast<const std::byte *>(bankData + sampleOffset);
        channels = (descriptor[8] & 0x80U) != 0 ? 2U : 1U;
        const bool loops = (packedOffset & 0x02000000U) != 0 && (note[1] & 0x80U) == 0 && packedOffset != 0x0300518cU;
        if (loops) {
            loopBegin = read_unaligned<std::uint16_t>(descriptor + 4);
            const std::uint32_t loopEnd = read_unaligned<std::uint16_t>(descriptor + 6);
            if (loopEnd > loopBegin)
                loopLength = loopEnd - loopBegin;
        }
    }
    const std::uint32_t velocityTableOffset = read_unaligned<std::uint32_t>(bankData + 0x28);
    const std::size_t velocityOffset =
        static_cast<std::size_t>(velocityTableOffset) + 4U + 128U * static_cast<std::size_t>(table[6]) + static_cast<std::size_t>(commandPart);
    if (velocityOffset >= bankSize)
        return 0;
    std::uint32_t level =
        (static_cast<std::uint32_t>(bankData[velocityOffset]) * (((static_cast<std::uint32_t>(note[8]) + note[9]) >> 1U) & 0x7ffffffeU)) >> 8U;
    if (level)
        ++level;
    level = (2U * memory[0x8cc + configBase] * level) >> 8U;
    if (level)
        ++level;
    level = (2U * memory[0x8e2 + configBase] * level) >> 8U;
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
    const float volume = AicaLevelToGain(level);
    const std::int32_t pitchUnits = static_cast<std::int32_t>(
        (static_cast<double>(static_cast<std::int32_t>(memory[0x8cf + configBase]) + static_cast<std::int8_t>(note[11])) * 1024.0 / 3.0 * 0.015625) +
        (static_cast<double>(static_cast<std::int32_t>(memory[0x8ce + configBase]) + static_cast<std::int8_t>(note[10]) + pitchNote) * 1024.0 / 3.0));
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
    voice.status = (memory[0x8c1 + configBase] & 1U) != 0U ? 2U : 1U;
    voice.playState = 0;
    voice.ownerId = static_cast<std::int8_t>(voiceIndex);
    voice.bank = static_cast<std::uint8_t>(bank);
    voice.spawnCommand = g_aicaMemory->currentCommand;
    voice.driverFlags = static_cast<std::uint8_t>(((memory[0x8c1 + configBase] | 0xfeU) << 6U) | (static_cast<std::int8_t>(note[1]) < 0 ? 8U : 0U));
    if (memory[0x8e9 + configBase])
        voice.driverFlags |= 1U;
    voice.layerFlags = note[35];
    voice.playFlags = note[34] & 0xc3U;
    voice.part = static_cast<std::uint8_t>(commandPart);
    voice.noteAttenuation = static_cast<std::uint8_t>(note[36] + 127U);
    voice.stereo = channels == 2 ? 1U : 0U;
    voice.driverNoteTable = reinterpret_cast<std::uintptr_t>(note);
    voice.driverPointerTable = reinterpret_cast<std::uintptr_t>(table);
    voice.xaudioVoiceId = 0xffffffffU;
    voice.xaudioStreamKind = 1;
    voice.pitchOrBaseNote = panBaseAttenuation;
    g_aicaPlaybackOwners[playbackIndex] = nullptr;
    g_aicaPlaybackPreBankLevels[playbackIndex] = preBankLevel;
    g_aicaPlaybackUsesBankVolume[playbackIndex] = usesBankVolume ? 1U : 0U;

    void *const playback = PrepareRawSoundEffect(sampleData, audioBytes, loopBegin, loopLength, channels, volume, frequencyRatio, 44100, true);
    if (!playback)
        return 0;
    voice.xaudioVoiceId = RawSoundEffectVoiceId(playback);
    g_aicaPlaybackOwners[playbackIndex] = playback;
    const std::uint8_t reverbLevel = note[30] != 0 ? static_cast<std::uint8_t>(memory[0x8ca + configBase] >> 4U) : static_cast<std::uint8_t>(note[31] >> 4U);
    const float reverbDecibels = reverbLevel == 0 ? -100.0f : 3.0f * (static_cast<float>(reverbLevel) - 15.0f);
    const float reverbGain = reverbDecibels > -64.0f ? std::pow(10.0f, reverbDecibels / 20.0f) : 0.0f;
    const std::array<float, 2> reverbMatrix{reverbGain, reverbGain};
    SetRawSoundEffectOutputMatrix(playback, reverbMatrix.data(), channels, 1);
    ApplyVoicePanMatrixToXAudio(playbackIndex);
    if (sampleId == 1) {
        note[12] = 0xd2U;
        note[14] = 0x2fU;
    }
    const std::int32_t envelopeNote = driverKind == 0 ? 60 :
        driverKind == 1                               ? commandNote - table[64] :
                                                        static_cast<std::int32_t>(commandNote | (8U * (g_aicaMemory->currentCommand & 0xf0U)));
    SetAttackFilterParamsForTone(voice.xaudioVoiceId, note, envelopeNote, volume, g_aicaMemory->ownerId == 2, g_aicaMemory->ownerId);
    StartRawSoundEffectPlayback(playback);
    voice.playState = 1;
    if (g_aicaMemory->voicePool.paused)
        SetRawSoundEffectPaused(playback, true);
    return 1;
}

void PerformAicaSoundCallback(void *entity) {
    Character *const character = static_cast<Character *>(entity);
    MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr;
    if (manager)
        ++g_currentAicaCommandLoops;
}

std::int64_t ForEachCewpEntityApplyCallback(std::int64_t, void (*callback)(void *)) {
    if (!callback || !g_mapControlTask || !g_mapControlTask->task_data_ptr) {
        return 1;
    }
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    for (void *bucketHead : control->characterBuckets) {
        for (auto *bucket = static_cast<CharacterBucketNode *>(bucketHead); bucket; bucket = bucket->next) {
            Character *const character = bucket->character;
            if (!character || character->entity.name != SHEN_NAME(CEWP)) {
                continue;
            }
            const std::uint32_t flags = character->flags.value;
            if ((flags & 1U) != 0U || ((flags & 2U) != 0U && (character->motionFlags & 1U) != 0U)) {
                callback(character);
            }
        }
    }
    return 1;
}

void ProcessAmbientSoundEffects() {
    AmbientSoundEntry *const entries = static_cast<AmbientSoundEntry *>(g_pAMB1Tmp);
    if (!entries)
        return;

    g_ambientFrameParity ^= 1U;
    std::int32_t profileIndex = 2 * static_cast<std::int32_t>(g_currentDateTime.hours) + (g_currentDateTime.minutes >= 30 ? 1 : 0) - 14;
    if (profileIndex < 0)
        profileIndex = 33;
    const std::int32_t nextProfileIndex = (std::min)(profileIndex + 1, 33);
    float profileMinutes = static_cast<float>(g_currentDateTime.minutes);
    if (static_cast<double>(profileMinutes) >= 30.0)
        profileMinutes = static_cast<float>(static_cast<double>(profileMinutes) - 30.0);

    const Vector3 listener = AmbientSoundListenerPosition();
    const std::int32_t inverseCameraYaw = -g_fCamAccumYaw;
    for (std::int32_t channel = 1; channel <= 16; ++channel) {
        AmbientSoundEntry &entry = entries[channel - 1];
        if (!entry.active || !entry.position)
            continue;

        const float deltaX = listener.x - entry.position->x;
        const float deltaZ = listener.z - entry.position->z;
        const float distance = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
        float effectVolume = 0.0f;
        if (distance <= entry.minimumDistance + entry.fadeDistance) {
            if (distance <= entry.minimumDistance) {
                effectVolume = 127.0f;
            } else if (entry.fadeDistance > 0.0f) {
                const float remaining = entry.fadeDistance - (distance - entry.minimumDistance);
                effectVolume = static_cast<float>(static_cast<double>(remaining) * (127.0 / static_cast<double>(entry.fadeDistance)));
            }
        }

        if (effectVolume > 0.0f && entry.halfHourVolume) {
            const float current = entry.halfHourVolume[profileIndex];
            const float difference = entry.halfHourVolume[nextProfileIndex] - current;
            const double profile = static_cast<double>(difference) * 0.03333 * static_cast<double>(profileMinutes) + static_cast<double>(current);
            effectVolume = static_cast<float>(profile * static_cast<double>(effectVolume));
        }
        if (effectVolume > 0.0f) {
            switch (entry.weatherMode) {
            case 1: {
                const double cloud = static_cast<double>(WEXE_GetCloudIntensity());
                effectVolume = static_cast<float>(static_cast<double>(effectVolume) - cloud * static_cast<double>(effectVolume));
                break;
            }
            case 2: {
                const float precipitation = (std::max)(WEXE_GetSnowIntensity(), WEXE_GetRainIntensity());
                const float weatherFactor =
                    static_cast<double>(precipitation) <= 0.01 ? WEXE_GetCloudIntensity() : static_cast<float>(static_cast<double>(precipitation) + 1.0);
                const double volume = static_cast<double>(effectVolume);
                effectVolume = static_cast<float>(volume - static_cast<double>(weatherFactor) * 0.45 * volume);
                break;
            }
            case 3:
                effectVolume = static_cast<float>(static_cast<double>(WEXE_GetRainIntensity() * effectVolume) + 32.0);
                if (static_cast<double>(effectVolume) > 127.0)
                    effectVolume = 127.0f;
                break;
            default:
                break;
            }
        }
        if ((g_areaId == SHEN_NAME(DXMS) && channel == 11) || effectVolume < 50.0f)
            effectVolume = 0.0f;

        std::int32_t pan = 0;
        bool foldedLeft = false;
        if (effectVolume > 0.0f && (entry.fixedPan == 0 || entry.spatialBlend < 1.0f)) {
            const std::uint16_t sourceDirection = SoundFixedAtan2(-(entry.position->x - listener.x), -(entry.position->z - listener.z));
            std::uint16_t relativeDirection = static_cast<std::uint16_t>(0x4000U - static_cast<std::uint16_t>(inverseCameraYaw) + sourceDirection);
            if (static_cast<std::int16_t>(relativeDirection) < 0) {
                relativeDirection = static_cast<std::uint16_t>(~relativeDirection);
                foldedLeft = true;
            }
            const std::int16_t centered = static_cast<std::int16_t>(static_cast<std::uint16_t>(relativeDirection - 0x4000U));
            pan = -((static_cast<std::int32_t>(centered) * 64) >> 14);
            pan = (std::clamp)(pan, -40, 40);
            if (entry.fixedPan == 0 && pan != entry.currentPan) {
                ExecuteAicaCommand2Ch(1, channel, 0xb00a0000u, static_cast<std::int16_t>(pan));
                entry.currentPan = pan;
            }
        }

        if (effectVolume > 0.0f && entry.spatialBlend < 1.0f) {
            std::int32_t crossfeed = 0;
            if (foldedLeft) {
                const double foldedPan = static_cast<double>(static_cast<float>(pan)) * 0.015625;
                crossfeed = static_cast<std::int32_t>((1.0 - static_cast<double>(entry.spatialBlend)) * 127.0 * (1.0 - std::abs(foldedPan)));
            }
            if (crossfeed != entry.currentCrossfeed) {
                ExecuteAicaCommand2Ch(1, channel, 0xb0020000u, static_cast<std::int16_t>(crossfeed));
                entry.currentCrossfeed = crossfeed;
            }
        }

        const std::int32_t targetVolume = static_cast<std::int32_t>(effectVolume);
        if (targetVolume == entry.currentVolume)
            continue;
        if (targetVolume != 0) {
            if (entry.currentVolume == 0)
                shenExecuteAICACommand(0x29a0, 1, channel);
            if (g_ambientFrameParity == (static_cast<std::uint32_t>(targetVolume) & 1U))
                continue;
        } else {
            shenExecuteAICACommand(0x28a0, 1, channel);
        }
        ExecuteAicaCommand2Ch(1, channel, 0xb0070000u, static_cast<std::int16_t>(targetVolume));
        entry.currentVolume = targetVolume;
    }
}

void ProcessAudioData(CVWKRuntimeSlot *slot, std::int32_t volume) {
    const bool music = g_CVRS && std::strstr(g_CVRS->entries[0].name, "BGM") != nullptr;
    const auto apply = [volume, music](CVWKRuntimeSlot &current) { SetDialogueVoiceVolume(current.audioContext, volume, music); };
    if (slot) {
        apply(*slot);
        return;
    }
    if (!g_pCVWK)
        return;

    for (CVWKRuntimeSlot &current : g_pCVWK->slots) {
        if (current.callback)
            apply(current);
    }
}

void AudioRender() {
    g_audioWorkerRunning.store(true, std::memory_order_release);
    std::chrono::steady_clock::time_point previousTick = std::chrono::steady_clock::now();
    while (!g_audioWorkerStop.load(std::memory_order_acquire) && !d3t::g_d3tShutdownThreadFlag.load(std::memory_order_acquire)) {
        RecoverXAudioDeviceIfNeeded();
        {
            std::lock_guard lock(g_aicaCommandMutex);
            if (g_aicaMemory && g_aicaMemory->updateComplete == 1 && !g_aicaMemory->voicePool.paused) {
                g_aicaMemory->updateComplete = 0;
                std::uint8_t *const raw = reinterpret_cast<std::uint8_t *>(g_aicaMemory);
                for (std::size_t index = 0; index < 16; ++index) {
                    const std::size_t base = 0x2cbc + 56 * index;
                    if (raw[base + 4] && !(raw[base + 4] & 1U)) {
                        const std::uint32_t delay = read_unaligned<std::uint32_t>(raw + base + 16);
                        const std::uint32_t step = read_unaligned<std::uint32_t>(raw + base + 8);
                        write_unaligned(raw + base + 16, delay - 4U * step);
                    }
                }
                ReapInactiveAicaVoices();
                PromoteQueuedAicaCommands();
                ExecuteAicaInsnForCycles();
                ProcessAicaEventStreams();
                ApplyAicaFilters();
                g_aicaMemory->updateComplete = 1;
            }
        }
        const std::chrono::steady_clock::time_point afterWork = std::chrono::steady_clock::now();
        const std::chrono::nanoseconds remaining = std::chrono::milliseconds(4) - (afterWork - previousTick);
        if (remaining.count() > 0)
            std::this_thread::sleep_until(afterWork + remaining * 7 / 10);
        do {
#if defined(_M_X64) || defined(__x86_64__)
            _mm_pause();
#endif
        } while (std::chrono::steady_clock::now() - previousTick < remaining);
        previousTick = std::chrono::steady_clock::now();
    }
    g_audioWorkerRunning.store(false, std::memory_order_release);
}

std::int32_t InitAICAMemory() {
    InitializeAicaMemory();
    g_aicaMemory->alignedAudioRam = static_cast<std::uint8_t *>(std::malloc(0x1f00000));
    if (!g_aicaMemory->alignedAudioRam)
        throw std::bad_alloc();
    g_aicaMemory->audioRamSize = 0x1f00000;
    const std::uintptr_t base = (reinterpret_cast<std::uintptr_t>(g_aicaMemory->alignedAudioRam) + 31U) & ~std::uintptr_t{31};
    g_aicaMemory->bankRegionBase[0] = reinterpret_cast<std::uint8_t *>(base);
    g_audioWorkerStop.store(false, std::memory_order_release);
    if (d3t::g_workerThreadCount > 2)
        d3t::d3t_SetAudioCallback(AudioRender);
    return 0;
}

void InitializeAicaForNotebook() {
    if (g_bIsAICAInitialized)
        return;
    {
        std::lock_guard lock(g_aicaCommandMutex);
        InitializeXAudio();
        InitAICAMemory();
    }
    for (std::int32_t bank = 7; bank >= 0; --bank)
        g_soundTable[bank][0] = 0;
    g_aicaFrameTimer = 0;
    g_aicaNeedsBankCleanup[0] = 0;
    {
        std::lock_guard lock(g_aicaCommandMutex);
        FinishInitializingAICAMemory();
    }
    sondLoadSndFromDirIntoBank(-1, 0, "system1.snd");
    shenExecuteAICACommand(0xa8, 0, 0);
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(HLib_AllocHeapMemBlock(0x4000, SHEN_NAME(BROM))), 0);
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(HLib_AllocHeapMemBlock(0x4000, SHEN_NAME(BROM))), 0);

    shenExecuteAICACommand(0x800a0, 0, 0);
    SNDFunc((15U << 16U) + 0x1ba0U);
    g_bIsAICAInitialized = true;
    for (std::uint32_t bank = 0; bank < 8; ++bank) {
        g_sfxVolume = g_applicationSettings.savedSfxVolume;
        SetVolumeForAicaBank(g_sfxVolume, bank);
    }
    g_musicVolume = g_applicationSettings.savedMusicVolume;
}

void SOND_Callback(HLib::TaskData *) {
    SOND_UpdateAICA();
    ProcessAmbientSoundEffects();
    ProcessSoundForCewpHandler();
}
void SOND_Init() {
    EnsureAicaMemory();
    g_sondTask = HLib_EnqueueTaskWithoutParameter(SOND_Callback, 3, 3, SHEN_NAME(SOND));
    shenExecuteAICACommand(0x300a0, 0, 0);
    shenExecuteAICACommand(0x400a0, 0, 0);
    for (std::int32_t bank = 15; bank >= 0; --bank)
        g_environmentAicaBanks[bank].active = 0;
    g_soundTaskState = -1;
    InitAMB1();
    shenExecuteAICACommand(0x1200a0, 3, 0);
    g_performedAicaCommand = 0;

    while (g_aicaFrameTimer > 0) {
        --g_aicaFrameTimer;
        if (g_aicaFrameTimer < 1) {
            for (std::uint32_t bank = 7; bank != 0; --bank) {
                SetVolumeForAicaBank(g_sfxVolume, bank);
                shenExecuteAICACommand(0x3a0, bank, 0);
                shenExecuteAICACommand(0xca0, bank, 0);
            }
            g_aicaMemory->voicePool.paused = 0;
            for (std::int32_t bank = 15; bank >= 0; --bank) {
                EnvironmentAicaBank &state = g_environmentAicaBanks[bank];
                if (!state.active)
                    continue;
                const std::int32_t volume = state.volume;
                state.volume = 0;
                state.active = 1;
                shenExecuteAICACommand(0xa6, static_cast<std::uint32_t>(bank), volume);
            }
        } else if (g_aicaFrameTimer == 1) {
            ProcessAudioData(nullptr, 0);
            SEQ_Cleanup();
        }
    }
    g_aicaNeedsBankCleanup[0] = 0;
}
void ExecuteAicaCommandsForAllBanks(std::int32_t paused, bool preserveVoicePoolPause) {
    EnsureAicaMemory();
    if (paused) {
        if (g_aicaFrameTimer >= 1) {
            ProcessAudioData(nullptr, -999);
            SEQ_Cleanup();
            ++g_aicaFrameTimer;
            return;
        }
        for (std::uint32_t bank = 7; bank != 0; --bank) {
            shenExecuteAICACommand(0x2a0, bank, 0);
            shenExecuteAICACommand(0xba0, bank, 0);
            SetVolumeForAicaBank(0.0f, bank);
        }
        if (!preserveVoicePoolPause) {
            std::lock_guard lock(g_aicaCommandMutex);
            g_aicaMemory->voicePool.paused = 1;
            for (std::size_t index = 0; index < g_aicaPlaybackOwners.size(); ++index) {
                const std::uint8_t bank = g_aicaMemory->voicePool.slots[index].bank;
                if (bank == 0 || bank > 7)
                    SetRawSoundEffectPaused(g_aicaPlaybackOwners[index], true);
            }
        }
        for (std::int32_t bank = 15; bank >= 0; --bank) {
            EnvironmentAicaBank &state = g_environmentAicaBanks[bank];
            if (!state.active)
                continue;
            const std::int8_t volume = state.volume;
            shenExecuteAICACommand(0xa6, static_cast<std::uint32_t>(bank), 0);
            state.active = 2;
            state.volume = volume;
        }
        ++g_aicaFrameTimer;
        return;
    }

    --g_aicaFrameTimer;
    if (g_aicaFrameTimer >= 1) {
        if (g_aicaFrameTimer == 1) {
            ProcessAudioData(nullptr, 0);
            SEQ_Cleanup();
        }
        return;
    }
    for (std::uint32_t bank = 7; bank != 0; --bank) {
        SetVolumeForAicaBank(g_sfxVolume, bank);
        shenExecuteAICACommand(0x3a0, bank, 0);
        shenExecuteAICACommand(0xca0, bank, 0);
    }
    if (!preserveVoicePoolPause) {
        std::lock_guard lock(g_aicaCommandMutex);
        g_aicaMemory->voicePool.paused = 0;
        for (std::size_t index = 0; index < g_aicaPlaybackOwners.size(); ++index) {
            const std::uint8_t bank = g_aicaMemory->voicePool.slots[index].bank;
            if (bank == 0 || bank > 7)
                SetRawSoundEffectPaused(g_aicaPlaybackOwners[index], false);
        }
    }
    for (std::int32_t bank = 15; bank >= 0; --bank) {
        EnvironmentAicaBank &state = g_environmentAicaBanks[bank];
        if (!state.active)
            continue;
        const std::int8_t volume = state.volume;
        state.volume = 0;
        state.active = 1;
        shenExecuteAICACommand(0xa6, static_cast<std::uint32_t>(bank), volume);
    }
}

void SOND_SetPaused(bool paused) {
    ExecuteAicaCommandsForAllBanks(paused ? 1 : 0, false);
    CVWK_SetAudioPauseState(paused);
}

void SOND_Shutdown() {
    g_audioWorkerStop.store(true, std::memory_order_release);
    while (g_audioWorkerRunning.load(std::memory_order_acquire))
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    d3t::d3t_SetAudioCallback(nullptr);
    HLib::Task *const task = g_sondTask;
    g_sondTask = nullptr;
    if (task)
        HLib_DestroyTask(task);

    {
        std::lock_guard lock(g_aicaCommandMutex);
        if (g_aicaMemory) {
            for (std::size_t bank = 0; bank < 16; ++bank)
                ReleaseOwnedAicaBankUnlocked(bank);
        }
        StopAicaPlaybackOwnersUnlocked();
        if (g_aicaMemory) {
            std::free(g_aicaMemory->alignedAudioRam);
            g_aicaMemory->alignedAudioRam = nullptr;
        }
    }
    if (g_soundTaskData) {
        HLib_FreeTaskData(g_soundTaskData, 0);
        g_soundTaskData = nullptr;
    }
    if (g_pAMB1) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_pAMB1), 0);
        g_pAMB1 = nullptr;
    }
    g_pAMB1Tmp = nullptr;
    g_soundPath = nullptr;
    g_soundTaskState = -1;
    g_soundDataNumber = 0;
    g_soundFlag = 0;
    g_soundIndex = 0;
    std::memset(g_soundOffsetName, 0, sizeof(g_soundOffsetName));
    std::memset(g_soundTable, 0, sizeof(g_soundTable));
    std::memset(g_environmentAicaBanks, 0, sizeof(g_environmentAicaBanks));
    std::memset(g_environmentAicaBankVolumes, 0, sizeof(g_environmentAicaBankVolumes));
    std::memset(g_aicaNeedsBankCleanup, 0, sizeof(g_aicaNeedsBankCleanup));
    g_aicaFrameTimer = 0;
    g_performedAicaCommand = 0;
    g_currentAicaCommandLoops = 0;
    g_ambientFrameParity = 0;
    {
        std::lock_guard lock(g_aicaCommandMutex);
        InitializeAicaMemory();
    }
    g_bIsAICAInitialized = false;
    ShutdownXAudio();
}


std::uint32_t SOND_AccessAicaMemory(std::uint8_t ownerId) {
    std::lock_guard lock(g_aicaCommandMutex);
    if (!g_aicaMemory)
        return 0;
    const auto *const raw = reinterpret_cast<const std::uint8_t *>(g_aicaMemory);
    for (std::size_t index = 0; index < 16; ++index) {
        const std::size_t base = 0x2cbc + 56 * index;
        if (static_cast<std::int8_t>(raw[base + 4]) < 0 && raw[base + 6] == ownerId) {
            return (raw[base + 4] & 1U) | 0x100U;
        }
    }
    return 0;
}

std::uint32_t SOND_ConsumeAicaEvent() {
    std::lock_guard lock(g_aicaCommandMutex);
    if (!g_aicaMemory)
        return 0;
    const std::uint32_t event = g_aicaMemory->field70c;
    g_aicaMemory->field70c = 0;
    return event;
}



std::array<std::uint8_t, 8> g_cassetteSavedBankLevels{};
std::int32_t g_pendingAmbientBank3Volume = 0;

namespace {


void FindAndExecuteAicaResetCommand() {
    if (!SOND_AccessAicaMemory(3)) {
        g_pendingAmbientBank3Volume = 0;
        return;
    }
    {
        std::lock_guard lock(g_aicaCommandMutex);
        g_pendingAmbientBank3Volume = reinterpret_cast<const std::uint8_t *>(g_aicaMemory)[0x79a];
    }
    shenExecuteAICACommand(0x1100a0, 3, 0);
}


void PerformAica2ChannelCommand() {
    if (!g_pendingAmbientBank3Volume)
        return;
    shenExecuteAICACommand(0x5ea8, 0, 0);
    shenExecuteAICACommand(0x4a0, 3, g_pendingAmbientBank3Volume);
    if (g_performedAicaCommand) {
        for (std::int32_t channel = 3; channel; --channel)
            ExecuteAicaCommand2Ch(3, channel, 0xb0070000U, 0);
        g_performedAicaCommand = 1;
    }
}

} // namespace



void ResetAICA(CassetteSoundState &sound, float) {
    sound.bank = 2;
    if (SOND_AccessAicaMemory(2)) {
        std::lock_guard lock(g_aicaCommandMutex);
        std::memcpy(sound.filename.data(), g_soundTable[2], sound.filename.size());
        sound.command = 0x3fa8;
        sound.volume = reinterpret_cast<const std::uint8_t *>(g_aicaMemory)[0x782];
    } else {
        sound.command = 0;
    }
    FindAndExecuteAicaResetCommand();
    for (std::int32_t channel = 16; channel; --channel) {
        if (!g_pAMB1Tmp)
            continue;
        AmbientSoundEntry &entry = static_cast<AmbientSoundEntry *>(g_pAMB1Tmp)[channel - 1];
        entry.active = 0;
        entry.currentVolume = 0;
        entry.currentPan = 127;
        entry.currentCrossfeed = -1;
        ExecuteAicaCommand2Ch(1, channel, 0xb0070000U, 0);
    }
    for (std::int32_t bank = 15; bank >= 0; --bank) {
        EnvironmentAicaBank &state = g_environmentAicaBanks[bank];
        if (!state.active)
            continue;
        const std::int8_t volume = state.volume;
        shenExecuteAICACommand(0xa6, static_cast<std::uint32_t>(bank), 0);
        state.active = 2;
        state.volume = volume;
    }
    for (std::int32_t bank = 7; bank; --bank) {
        {
            std::lock_guard lock(g_aicaCommandMutex);
            g_cassetteSavedBankLevels[static_cast<std::size_t>(bank)] =
                g_aicaMemory ? reinterpret_cast<const std::uint8_t *>(g_aicaMemory)[0x753 + 24 * bank] : 0;
        }
        shenExecuteAICACommand(0x10a0, static_cast<std::uint32_t>(bank), 0);
        if (bank <= 4)
            shenExecuteAICACommand(0x28a0, static_cast<std::uint32_t>(bank), 16);
        g_aicaNeedsBankCleanup[0] = 1;
    }
}



void ExecuteCassetteSND(const CassetteSoundState &sound) {
    const std::int32_t bank = sound.bank;
    shenExecuteAICACommand(0x1100a0, static_cast<std::uint32_t>(bank), 0);
    if (sound.command && sound.filename[0]) {
        sondLoadSndFromDirIntoBank(g_sceneId, bank, sound.filename.data());
        shenExecuteAICACommand(sound.command, 0, 0);
        shenExecuteAICACommand(0x4a0, static_cast<std::uint32_t>(bank), static_cast<std::int32_t>(sound.volume));
    }
    PerformAica2ChannelCommand();
    for (std::int32_t channel = 16; channel; --channel) {
        if (!g_pAMB1Tmp)
            continue;
        AmbientSoundEntry &entry = static_cast<AmbientSoundEntry *>(g_pAMB1Tmp)[channel - 1];
        if (entry.position)
            entry.active = 1;
    }
    for (std::int32_t environmentBank = 15; environmentBank >= 0; --environmentBank) {
        EnvironmentAicaBank &state = g_environmentAicaBanks[environmentBank];
        if (!state.active)
            continue;
        const std::uint32_t channel = static_cast<std::uint32_t>(environmentBank);
        state.active = 1;
        shenExecuteAICACommand(0xa5, channel, g_environmentAicaBankVolumes[environmentBank]);
        const std::int8_t level = static_cast<std::int8_t>(state.reserved_02[0]);
        state.reserved_02[0] = std::byte{};
        state.volume = 0;
        shenExecuteAICACommand(0x10a5, channel, level);
        const std::int8_t pan = static_cast<std::int8_t>(state.reserved_02[1]);
        state.reserved_02[1] = std::byte{};
        shenExecuteAICACommand(0x20a5, channel, pan);
        const std::int8_t release = state.volume;
        state.volume = 0;
        shenExecuteAICACommand(0xa6, channel, release);
    }
    for (std::int32_t level = 7; level; --level) {
        shenExecuteAICACommand(0x10a0, static_cast<std::uint32_t>(level), g_cassetteSavedBankLevels[static_cast<std::size_t>(level)]);
        if (level <= 4)
            shenExecuteAICACommand(0x29a0, static_cast<std::uint32_t>(level), 16);
        g_aicaNeedsBankCleanup[0] = 0;
    }
}

} // namespace shenmue::s1
