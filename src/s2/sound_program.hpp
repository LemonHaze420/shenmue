#pragma once

#include "s2/map_info.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace shenmue::s2 {
struct Character;

inline constexpr std::array<std::int32_t, 16> INITIAL_SOUND_BANK_ALLOCATIONS{0x180000, 0x2000, 0x2000, 0x2000, 0x2000, 0x4d8000, 0x2000, 0x2000};
inline constexpr std::array<std::int32_t, 8> INITIAL_SOUND_BANK_VOLUMES{-12, -12, -15, -15, -12, -9, -12, -12};

inline constexpr std::size_t MATERIAL_SOUND_HANDLE_COUNT = 16;
inline constexpr std::uint32_t MATERIAL_SOUND_SETUP_COMMAND = 0xa5U;
inline constexpr std::uint32_t MATERIAL_SOUND_GAIN_COMMAND = 0xa6U;
inline constexpr std::uint32_t MATERIAL_SOUND_VOLUME_COMMAND = 0x10a5U;
inline constexpr std::uint32_t MATERIAL_SOUND_CONTROL_COMMAND = 0x20a5U;

struct MaterialSoundPair {
    std::uint32_t material;
    std::uint32_t command;
};
struct MaterialSoundBank {
    MaterialSoundPair *pairs;
    std::uint32_t capacity;
    std::int32_t bank;
    std::uint32_t count;
};
struct SoundEffectHandle {
    std::int32_t active;
    std::int32_t command;
    std::uint32_t slot;
    std::int32_t gain;
    std::int32_t volume;
    std::int32_t control;
};
static_assert(sizeof(MaterialSoundPair) == 8);
static_assert(sizeof(MaterialSoundBank) == 0x18);
static_assert(offsetof(MaterialSoundBank, pairs) == 0);
static_assert(offsetof(MaterialSoundBank, capacity) == 8);
static_assert(offsetof(MaterialSoundBank, bank) == 12);
static_assert(offsetof(MaterialSoundBank, count) == 16);
static_assert(sizeof(SoundEffectHandle) == 0x18);
static_assert(offsetof(SoundEffectHandle, active) == 0);
static_assert(offsetof(SoundEffectHandle, command) == 4);
static_assert(offsetof(SoundEffectHandle, slot) == 8);
static_assert(offsetof(SoundEffectHandle, gain) == 12);
static_assert(offsetof(SoundEffectHandle, volume) == 16);
static_assert(offsetof(SoundEffectHandle, control) == 20);

struct SoundBankState {
    std::array<char, 17> filename{};
    std::int32_t allocation{-1};
    std::int32_t currentVolume{127};
    bool loaded{};
};

struct EventFadeBgmTaskData {
    std::uint32_t cleanupTask;
    std::int32_t frames;
    std::int32_t mode;
    float time;
};

static_assert(sizeof(EventFadeBgmTaskData) == 0x10);

struct SampleProgramRecord {
    std::uint16_t reserved00;
    std::array<std::uint8_t, 8> channels;
    std::uint8_t spatialMode;
    std::uint8_t panMode;
    std::uint8_t timeMode;
    std::uint8_t frontGainPercent;
    std::uint8_t weatherMode;
    std::uint8_t weekdays;
    Vector3 position;
    std::uint32_t characterName;
    float minimumDistance;
    float fadeDistance;
    std::uint8_t minimumMusicVolume;
    std::array<std::byte, 3> reserved29;
    std::uint8_t disabled;
    std::uint8_t volume;
    std::int8_t pan;
    std::uint8_t attenuation;
    std::array<std::uint8_t, 96> quarterHourGains;
};

struct SampleProgramState {
    std::int32_t executing{};
    std::uint32_t command{};
    std::int32_t count{};
    std::int32_t bank{};
    std::int32_t duckVolume{};
    std::int32_t duckDelay{};
    SampleProgramRecord *records{};
};

struct SampleProgramPool {
    SampleProgramRecord *records;
    std::int32_t capacity;
    std::int32_t bank;
};

static_assert(sizeof(SampleProgramRecord) == 0x90);
static_assert(offsetof(SampleProgramRecord, disabled) == 0x2c);
static_assert(offsetof(SampleProgramRecord, position) == 0x10);
static_assert(offsetof(SampleProgramRecord, quarterHourGains) == 0x30);
static_assert(sizeof(SampleProgramState) == 0x20);
static_assert(offsetof(SampleProgramState, records) == 0x18);
static_assert(sizeof(SampleProgramPool) == 0x10);

inline constexpr std::array<std::uint8_t, 8> defaultSampleFadeIn{{0, 30, 30, 30, 30, 0, 30, 30}};
inline constexpr std::array<std::uint8_t, 8> defaultSampleFadeOut{{0, 30, 30, 30, 30, 30, 30, 30}};

struct SoundProgramFadeRecord {
    std::uint16_t frame;
    std::uint8_t volume;
    std::uint8_t duration;
};
struct SelectedSoundProgramRecord {
    std::uint32_t reserved00;
    char bankName[8];
    std::uint16_t sequence;
    std::uint8_t bank;
    std::uint8_t volume;
    std::uint8_t restart;
    std::uint8_t sceneIndex;
    std::uint16_t reserved12;
    std::uint16_t startFrame;
    std::uint16_t reserved16;
    std::uint16_t panFrame;
    std::uint8_t reserved1a;
    std::uint8_t pan;
    std::uint32_t reserved1c;
    SoundProgramFadeRecord fades[16];
};
static_assert(sizeof(SelectedSoundProgramRecord) == 96);
static_assert(offsetof(SelectedSoundProgramRecord, startFrame) == 20);
static_assert(offsetof(SelectedSoundProgramRecord, fades) == 32);

inline constexpr std::int64_t SOUND_PROGRAM_STOP_INDEX = 0xffffffffLL;
inline constexpr shen_name SOUND_BANK_DATA{SHEN_NAME(SND1)};
inline constexpr shen_name SOUND_BANK_CHUNK_DATA{SHEN_NAME(SNDD)};
inline constexpr std::array<const char *, 14> DREAMCAST_MUSIC_BANK_NAMES{"0003_002", "003_014", "009_024", "005_009", "0014_019", "0005_013", "0016_001",
    "0013_022", "0009_018", "0013_024", "0008_015", "0013_029", "0020_007", "0002_020"};
inline constexpr std::array<const char *, 21> JAPANESE_DREAMCAST_SOUND_BANK_NAMES{"battle_2", "samrary", "kenippa", "stgaya", "stgaya_2", "stgaya_3", "togippa",
    "zako", "0028A000", "AR02_as0", "WESM_as0", "AB00_as0", "Q100_as0", "QTB1_as0", "QSFA_as0", "QSFD_as0", "QSFB_as0", "QSFC_as0", "QLBT_as0", "QGBT_as0",
    "QJBT_as0"};
inline constexpr const char *DREAMCAST_AMBIENT_BANK_NAME = "WE00_ab0";
extern std::int32_t g_audioPlaybackEnabled;
extern std::int32_t g_audioPlaybackSuppressed;
extern std::int32_t g_forceDreamcastSoundData;

struct SoundBankFade {
    float volume;
    float step;
};
static_assert(sizeof(SoundBankFade) == 8);
static_assert(alignof(SoundBankFade) == 4);
static_assert(offsetof(SoundBankFade, step) == 4);

struct SoundProgramRuntimeState {
    std::array<SoundBankState, 8> banks{};
    std::array<SoundBankFade, 8> bankFades{};
    std::array<char, 9> programName{};
    std::array<char, 17> soundDataName{};
    std::uint32_t frame{};
    std::int32_t pauseDepth{};
    std::uint8_t callbackParity{};
    std::int32_t activeProgramCommand{-1};
    std::int64_t selectedSceneIndex{};
    std::int32_t selectedProgramBank{};
    const SelectedSoundProgramRecord *selectedRecord{};
    std::array<char, 8> activeMusicName{};
    float musicVolume{};
    float musicVolumeStep{};
    std::int32_t publishedMusicVolume{};
    std::int32_t musicFadeFrames{};
    std::int32_t musicDuckVolume{};
    std::int32_t musicDuckDelay{};
    std::array<SampleProgramState, 4> samplePrograms{};
    bool sampleExecutionActive{};
    bool queued{};
};

const SoundProgramRuntimeState &GetSoundProgramRuntimeState() noexcept;
std::uint64_t GetSoundProgramPhaseFlag(std::int32_t selector) noexcept;
void ResetSoundProgramPhaseFlags(std::int32_t selector) noexcept;
void SetSoundProgramPhaseFlags(std::int32_t selector) noexcept;

void InitAica() noexcept;
void DisableAicaEmulation() noexcept;
void DisableAicaIfEnabled() noexcept;
void QueueSoundProgramControl() noexcept;

std::int64_t SNDP_SoundProgramReader(const MapInfoChunk *sndp, std::int32_t sceneId, shen_name map) noexcept;
void SNDD_Reader(const MapInfoChunk *sndd, std::int32_t sceneId, std::int32_t entryId) noexcept;
void ShutdownSoundProgramControl() noexcept;

void PrimeAicaEmulationForPause(std::int32_t enabled) noexcept;

std::int32_t aicaSetMainCommand(std::uint32_t command) noexcept;
std::int32_t ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) noexcept;
std::uint64_t ExecuteAicaBankCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value) noexcept;
std::uint64_t LoadAudioIntoAicaMemory(std::span<const std::byte> data, std::uint32_t bank) noexcept;
std::int64_t ProcessAicaCommand(std::uint32_t command, std::int32_t value) noexcept;



std::int64_t Loop3DSoundEffect(std::uint32_t flags, float context = 0.0f) noexcept;
bool Execute3DSoundCommand(std::uint32_t flags, const Vector3 &position, float minimumDistance, float fadeDistance, float context = 0.0f) noexcept;
bool PlayFootstepForNode(Character *character, std::uint32_t flags, bool debugPulse = false) noexcept;
void SetSoundListenerUsesCamera(std::int32_t enabled) noexcept;
std::uint64_t FindMaterialSoundBank(std::uint32_t material) noexcept;
SoundEffectHandle *AllocateMaterialSoundHandle(std::int32_t key) noexcept;

std::int64_t ExecuteMaterialSoundHandleCommand(SoundEffectHandle *handle, std::uint32_t command, std::int32_t value) noexcept;
std::int64_t FreeMaterialSoundHandle(SoundEffectHandle *handle) noexcept;

std::int64_t spmBeginSampleExecute(std::uint32_t flags, SampleProgramState *program) noexcept;
std::int64_t BeginPrimarySampleExecution(std::uint32_t flags = 0) noexcept;
std::int64_t BeginSecondarySampleExecution(std::uint32_t flags = 0) noexcept;
std::int64_t EV_spmBeginSampleExecute() noexcept;
// One-based sample index; the caller must own an admitted primary sample program.
std::int64_t spmSetSampleEnabled(std::int32_t sample, std::int32_t enabled) noexcept;
void spmFadeOut() noexcept;

std::int64_t spmFadeInBank(std::int32_t bank, std::int32_t frames) noexcept;
std::int64_t spmFadeOutBank(std::int32_t bank, std::int32_t frames) noexcept;
bool AreSoundBankFadesComplete() noexcept;
void SetSamplePlaybackSuppressed(std::int32_t suppressed) noexcept;
std::uint64_t GetSamplePlaybackSuppressed() noexcept;

std::int64_t LoadSoundProgram(const char *name, std::int64_t sceneIndex, std::uint32_t currentScene) noexcept;
std::int64_t GetSelectedSoundSceneIndex() noexcept;
char *shenGetCurrentSbt() noexcept;
char *GetCurrentBgm() noexcept;
std::uint64_t GetSoundProgramSceneCount(const char *name) noexcept;
std::int64_t LoadSoundProgramWithoutSelectedRecord(const char *name, std::uint32_t sceneIndex) noexcept;
const char *FindSoundProgramNameForMap(const char *name) noexcept;
std::int64_t SaveCurrentSoundProgramContext() noexcept;
void shenAssignSoundProgramByPtr(std::span<const std::byte> table) noexcept;
std::uint64_t shenAssignActiveSmp(const char *name) noexcept;
std::uint64_t LoadCutsceneSoundProgram(const char *name, std::int32_t actNumber) noexcept;
std::uint64_t EV_LoadCutsceneFromCurrentScene(const char *name) noexcept;
bool LoadCutsceneAndSoundProgramForArea() noexcept;
void StopSoundProgram() noexcept;

void SoundProgramControl_Callback(void *);
void EVT_AT_FadeBGM_Callback(void *data);
void EVT_AT_FadeBGM(std::int32_t frames, std::int32_t mode, std::int32_t delay);

} // namespace shenmue::s2
