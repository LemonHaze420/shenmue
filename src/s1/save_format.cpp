#include "s1/save_format.hpp"

#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/save.hpp"
#include "s1/task/fbgg.hpp"
#include "s1/task/modu.hpp"
#include "s1/task/time.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <atomic>
#include <array>
#include <bit>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <limits>
#include <type_traits>
#include <string>
#include <span>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

namespace shenmue::s1 {

extern std::int32_t g_actNumber;
extern std::array<std::uint8_t, 128> g_shenmueFlags;
extern std::int32_t g_pendingSleepResumeLoad;
extern Vector3 g_ryoHomeSleepPosition;
extern std::uint16_t g_ryoHomeSleepYaw;
extern std::uint8_t g_temporaryRyoEventFlags;
extern std::int16_t g_defaultKaidanId;
extern std::int32_t g_playerKaidanGuideActive;
extern std::uint8_t g_shouldSleep;
extern std::int32_t g_sleepPromptWindowStartMinutes;
extern std::int32_t g_sleepPromptWindowEndMinutes;
extern std::int32_t g_forcedSleepWindowStartMinutes;
extern std::int32_t g_forcedSleepWindowEndMinutes;
extern shen_name g_homeAreaId;
extern std::int32_t g_homeSceneId;
extern std::uint32_t g_requestedEntryPoint;
extern std::array<std::uint8_t, 16> g_ryoHomeSleepAuxBlock;
extern std::array<std::uint8_t, 16> g_ryoHomeSleepAuxBlock1;
extern std::uint16_t g_ryoHomeSleepGuideWord;
extern std::int32_t g_temporaryKaidanId;
extern std::array<std::uint32_t, 32> g_arcadeHighScores;
extern std::array<ArcadeScoreEntry, 20> g_spaceHarrierScores;
extern std::array<ArcadeScoreEntry, 20> g_hangOnScores;
extern std::array<std::uint32_t, 5> g_saveDataRelated;
extern std::int32_t g_temporarySaveSceneId;
extern std::uint32_t g_temporarySaveAreaId;
extern std::uint32_t g_temporarySaveEntryPoint;
extern std::int32_t g_saveDisplayModuleState;
extern std::uint16_t g_notebookLastEntryPage;
extern std::uint16_t g_notebookPersistenceWord;


std::uint32_t g_settingsFlags = 0xEB;

namespace {

constexpr std::size_t kChecksumBytes = 0x1800;
constexpr std::uint32_t kIdentifier = SHEN_NAME(SHEE);
constexpr std::uint32_t kVersion = 0x10eu;
constexpr std::uint32_t kPayloadBytes = 0x9980u;
constexpr std::size_t kInventoryIdsMirrorOffset = 0x1000;
constexpr std::size_t kInventoryCountMirrorOffset = 0x1400;
constexpr std::size_t kStepUtilityFunctionIndex = 388;

using SaveDateTime = S1SaveDateTime;

#pragma pack(push, 1)

struct VmuHeader {
    char shortDescription[16];
    char longDescription[32];
    char applicationId[16];
    std::uint16_t iconCount;
    std::uint16_t iconAnimationSpeed;
    std::uint16_t eyecatchType;
    std::uint16_t vmuCrc;
    std::uint32_t dataSize;
    std::uint8_t reserved[20];
    std::uint16_t iconPalette[16];
};

using SaveSummary = S1SaveSummary;

using MoveSetSave = MoveSetRuntimeState;

struct HomeSnapshot {
    float positionX;
    float positionY;
    float positionZ;
    std::int32_t yawSum;
    std::uint8_t eventFlags;
    std::uint8_t stairGuideEntryId;
    std::uint8_t stairGuideActive;
    std::uint8_t shouldSleep;
    std::uint16_t sleepPromptStartMinutes;
    std::uint16_t sleepPromptEndMinutes;
    std::uint16_t forcedSleepStartMinutes;
    std::uint16_t forcedSleepEndMinutes;
    std::uint32_t homeAreaId;
    std::uint16_t requestEntryPoint;
    std::uint16_t homeActNumber;
    SaveDateTime dateTime;
    std::uint16_t word2C;
    std::uint8_t unknown2E[2];
};

struct MainSaveBlock {
    std::uint32_t playerSavedStat0;
    std::int32_t currentVfSkill[4];
    std::uint32_t playerRuntimeFlags;
    std::int32_t cash;
    std::int32_t segaCoins;
    std::uint16_t alternateHp;
    std::uint16_t defaultHp;
    std::uint16_t cameraShakeAmount;
    std::uint16_t vfPlayerAssistScale;
    std::uint32_t settingsFlags;
    std::uint16_t playerContextActionMode;
    std::uint16_t word02E;
    std::uint8_t vfHpRegenerationProfile;
    std::uint8_t unknown031;
    MoveSetSave handMoves;
    MoveSetSave legMoves;
    MoveSetSave throwMoves;
    std::uint64_t scenarioBitFlags64;
    std::uint8_t shenmueFlags[128];
    std::uint8_t homeAuxiliary[32];
    std::uint8_t unknown1D0_230[96];
    std::uint32_t stepLut0To3[4];
    std::uint32_t stepLut4To7[4];
    std::uint8_t storyItemFlags[256];
    std::uint8_t gachaItemFlags[256];
    std::int16_t cassetteTapes[32];
    std::uint8_t playerStatsStep[16];
    std::uint8_t statBank4A0[160];
    std::uint8_t highScoreScalarBank[128];
    ArcadeScoreEntry spaceHarrierScores[20];
    ArcadeScoreEntry hangOnScores[20];
    std::uint16_t batteryLives[2];
    HomeSnapshot homeSnapshot;
    std::int16_t recentItemOldest;
    std::int16_t recentItemPrevious;
    std::int16_t recentItemLatest;
    std::int16_t uniqueItemCount;
    std::uint32_t saveDataRelated[4];
    std::uint32_t saveDataRelated4;
    std::uint8_t unknown7F0_7FC[12];
    std::uint16_t notebookLastEntryPage;
    std::uint16_t word7FE;
    std::uint8_t statPages[4096];
};

struct InternalSaveHeader {
    std::uint32_t identifier;
    std::uint32_t version;
    std::uint8_t progress;
    std::uint8_t byte009;
    std::uint8_t unknown00A[2];
    std::uint32_t settingsFlags;
    std::uint8_t unknown010[128];
    std::uint16_t word090;
    std::uint8_t unknown092[14];
    std::uint32_t checksum;
    std::uint8_t unknown0A4[28];
};

struct NativeSaveFile {
    VmuHeader vmuHeader;
    std::uint8_t iconFrames[0x600];
    InternalSaveHeader internal;
    SaveSummary summary;
    std::uint8_t unknown0780_2000[0x1880];
    MainSaveBlock main;
    std::uint8_t unknown3800_6800[0x3000];
    std::uint8_t globalStatsMirror[0x1800];
    std::uint8_t tail8000_A000[0x2000];
};
#pragma pack(pop)

static_assert(sizeof(SaveDateTime) == 0x8);
static_assert(sizeof(VmuHeader) == 0x80);
static_assert(sizeof(SaveSummary) == 0x40);
static_assert(sizeof(MoveSetSave) == 0x52);
static_assert(sizeof(ArcadeScoreEntry) == 0xc);
static_assert(sizeof(HomeSnapshot) == 0x30);
static_assert(offsetof(MainSaveBlock, scenarioBitFlags64) == 0x128);
static_assert(offsetof(MainSaveBlock, shenmueFlags) == 0x130);
static_assert(offsetof(MainSaveBlock, highScoreScalarBank) == 0x540);
static_assert(offsetof(MainSaveBlock, spaceHarrierScores) == 0x5c0);
static_assert(offsetof(MainSaveBlock, stepLut0To3) == 0x230);
static_assert(offsetof(MainSaveBlock, storyItemFlags) == 0x250);
static_assert(offsetof(MainSaveBlock, cassetteTapes) == 0x450);
static_assert(offsetof(MainSaveBlock, homeSnapshot) == 0x7a4);
static_assert(offsetof(MainSaveBlock, statPages) == 0x800);
static_assert(sizeof(MainSaveBlock) == kChecksumBytes);
static_assert(sizeof(InternalSaveHeader) == 0xc0);
static_assert(offsetof(NativeSaveFile, internal) == kS1SaveInternalHeaderOffset);
static_assert(offsetof(NativeSaveFile, summary) == kS1SaveSummaryOffset);
static_assert(offsetof(NativeSaveFile, main) == kS1SaveMainBlockOffset);
static_assert(offsetof(NativeSaveFile, globalStatsMirror) == kS1SaveGlobalStatsOffset);
static_assert(sizeof(NativeSaveFile) == kS1SaveImageSize);
static_assert(std::is_trivially_copyable_v<NativeSaveFile>);
static_assert(sizeof(CassetteTapeState) == sizeof(std::int16_t));
static_assert(sizeof(PlayerStatsPages) + 160 == sizeof(MainSaveBlock::statPages));
static_assert(kInventoryIdsMirrorOffset + sizeof(PlayerStatsRuntimeState::inventoryItemIds) <= sizeof(NativeSaveFile::globalStatsMirror));
static_assert(kInventoryCountMirrorOffset + sizeof(std::int16_t) <= sizeof(NativeSaveFile::globalStatsMirror));

struct StepUtilityParameters {
    std::int32_t mode;
    std::uint16_t requestedStep;
};

std::uint32_t CalculateChecksum(const std::uint8_t *bytes) noexcept {
    std::uint32_t crc = 0xffffffffu;
    for (std::size_t index = 0; index < kChecksumBytes; ++index) {
        crc ^= static_cast<std::uint32_t>(bytes[index]) << 24u;
        for (int bit = 0; bit < 8; ++bit) {
            crc = (crc & 0x80000000u) != 0 ? (crc << 1u) ^ 0x04c11db7u : crc << 1u;
        }
    }
    return 1u - crc;
}

void LoadLayout(const S1SaveImage &image, NativeSaveFile &save) noexcept {
    std::memcpy(&save, image.bytes.data(), sizeof(save));
}

void StoreLayout(const NativeSaveFile &save, S1SaveImage &image) noexcept {
    std::memcpy(image.bytes.data(), &save, sizeof(save));
}

VmuHeader CanonicalVmuHeader() noexcept {
    VmuHeader header{};
    constexpr char shortDescription[16] = "US SHENMUE1 ";
    constexpr char longDescription[32] = "US SHENMUE";
    constexpr std::uint8_t applicationId[16]{
        0x18,
        0x09,
        0x53,
        0x41,
        0x07,
        0x54,
        0x55,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    std::memcpy(header.shortDescription, shortDescription, sizeof(shortDescription));
    std::memcpy(header.longDescription, longDescription, sizeof(longDescription));
    std::memcpy(header.applicationId, applicationId, sizeof(applicationId));
    header.iconCount = 3;
    header.iconAnimationSpeed = 10;
    header.dataSize = kPayloadBytes;
    return header;
}

void InsertSaveFileHeader(NativeSaveFile &save) noexcept {
    save.vmuHeader = CanonicalVmuHeader();
    std::memset(save.iconFrames, 0, sizeof(save.iconFrames));
}

void FinalizeChecksums(NativeSaveFile &save) noexcept {
    const std::uint32_t internalChecksum = CalculateChecksum(save.unknown0780_2000 + 0x80);
    save.internal.checksum = internalChecksum;
    std::memcpy(save.unknown0780_2000 + 0x20, &internalChecksum, sizeof(internalChecksum));
    std::memcpy(save.unknown0780_2000 + 0x60, &internalChecksum, sizeof(internalChecksum));
    save.summary.mainBlockCrc = CalculateChecksum(reinterpret_cast<const std::uint8_t *>(&save.main));
}

bool ValidDateTime(const SaveDateTime &value) noexcept {
    return value.month >= 1 && value.month <= 12 && value.day >= 1 && value.day <= 31 && value.hour <= 23 && value.minute <= 59 && value.second <= 59 &&
        value.dayOfWeek <= 6;
}

S1SaveResult ValidateLayout(const NativeSaveFile &save) noexcept {
    const VmuHeader expected = CanonicalVmuHeader();
    if (std::memcmp(&save.vmuHeader, &expected, sizeof(expected)) != 0 ||
        std::any_of(std::begin(save.iconFrames), std::end(save.iconFrames), [](std::uint8_t value) { return value != 0; }) ||
        save.internal.identifier != kIdentifier || save.internal.version != kVersion) {
        return S1SaveResult::InvalidHeader;
    }

    const std::uint32_t internalChecksum = CalculateChecksum(save.unknown0780_2000 + 0x80);
    std::uint32_t mirror0 = 0;
    std::uint32_t mirror1 = 0;
    std::memcpy(&mirror0, save.unknown0780_2000 + 0x20, sizeof(mirror0));
    std::memcpy(&mirror1, save.unknown0780_2000 + 0x60, sizeof(mirror1));
    if (save.internal.checksum != internalChecksum || mirror0 != internalChecksum || mirror1 != internalChecksum ||
        save.summary.mainBlockCrc != CalculateChecksum(reinterpret_cast<const std::uint8_t *>(&save.main))) {
        return S1SaveResult::InvalidChecksum;
    }

    std::int16_t inventoryCount = 0;
    std::memcpy(&inventoryCount, save.globalStatsMirror + kInventoryCountMirrorOffset, sizeof(inventoryCount));
    const SaveDateTime &gameTime = save.summary.gameTime;
    if (inventoryCount < 0 || inventoryCount > static_cast<std::int16_t>(PlayerStatsRuntimeState{}.inventoryItemIds.size()) ||
        !ValidDateTime(save.summary.systemTime) || !ValidDateTime(gameTime) || gameTime.year > std::numeric_limits<std::uint8_t>::max() ||
        save.summary.actNumber < 0 || save.summary.sceneId < 0 || save.summary.entryId < 0 || save.summary.saveState > 9) {
        return S1SaveResult::InvalidData;
    }
    return S1SaveResult::Success;
}

S1SaveResult ValidateSceneBlock(const NativeSaveFile &save, std::int32_t block) noexcept {
    if (block < 0 || block > 3)
        return S1SaveResult::InvalidArgument;
    if (save.internal.identifier != kIdentifier || save.internal.version != kVersion)
        return S1SaveResult::InvalidHeader;
    const std::uint8_t *const bytes = reinterpret_cast<const std::uint8_t *>(&save);
    const std::size_t summaryOffset = kS1SaveSummaryOffset - sizeof(SaveSummary) + static_cast<std::size_t>(block) * sizeof(SaveSummary);
    const std::size_t mainOffset = kS1SaveMainBlockOffset - sizeof(MainSaveBlock) + static_cast<std::size_t>(block) * sizeof(MainSaveBlock);
    std::uint32_t checksum = 0;
    std::memcpy(&checksum, bytes + summaryOffset + offsetof(SaveSummary, mainBlockCrc), sizeof(checksum));
    return checksum == CalculateChecksum(bytes + mainOffset) ? S1SaveResult::Success : S1SaveResult::InvalidChecksum;
}

bool GetLocalSystemTime(SaveDateTime &result) noexcept {
    const std::time_t now = std::time(nullptr);
    if (now == static_cast<std::time_t>(-1))
        return false;
    std::tm local{};
#ifdef _WIN32
    if (localtime_s(&local, &now) != 0)
        return false;
#else
    if (localtime_r(&now, &local) == nullptr)
        return false;
#endif
    result.year = static_cast<std::uint16_t>(local.tm_year + 1900);
    result.month = static_cast<std::uint8_t>(local.tm_mon + 1);
    result.day = static_cast<std::uint8_t>(local.tm_mday);
    result.hour = static_cast<std::uint8_t>(local.tm_hour);
    result.minute = static_cast<std::uint8_t>(local.tm_min);
    result.second = static_cast<std::uint8_t>(local.tm_sec);
    result.dayOfWeek = static_cast<std::uint8_t>(local.tm_wday);
    return true;
}

SaveDateTime ToSaveDateTime(const GameDateTime &value) noexcept {
    return SaveDateTime{value.year, value.month, value.day, value.hours, value.minutes, value.seconds, value.dayOfWeek};
}

GameDateTime ToGameDateTime(const SaveDateTime &value) noexcept {
    return GameDateTime{static_cast<std::uint8_t>(value.year), value.month, value.day, value.dayOfWeek, value.hour, value.minute, value.second, 0};
}
struct PendingMapStartSnapshot {
    std::uint8_t shouldSleep;
    std::int32_t sleepPromptStartMinutes;
    std::int32_t sleepPromptEndMinutes;
    std::int32_t forcedSleepStartMinutes;
    std::int32_t forcedSleepEndMinutes;
    shen_name homeAreaId;
    std::int32_t homeSceneId;
    std::uint32_t requestedEntryPoint;
    GameDateTime temporaryDateTime;
    std::uint16_t temporaryDateTimeWord;
};

PendingMapStartSnapshot g_pendingMapStartSnapshot{};
void RestoreCurrentSlotMetadata(std::uint8_t progress) noexcept {
    g_playerStats.currentSaveProgress = progress;
    std::memcpy(g_playerStats.inventoryItemIds.data(), g_bookkeepingStatsCache.globalStatsMirror.data() + kInventoryIdsMirrorOffset,
        sizeof(g_playerStats.inventoryItemIds));
    std::memcpy(&g_playerStats.inventoryItemCount, g_bookkeepingStatsCache.globalStatsMirror.data() + kInventoryCountMirrorOffset,
        sizeof(g_playerStats.inventoryItemCount));
}

bool StepUtilityAvailable() noexcept {
    return kStepUtilityFunctionIndex < ShenmueScriptFuncs.size() && ShenmueScriptFuncs[kStepUtilityFunctionIndex] != nullptr;
}

bool ReadCurrentScenarioStep(std::int16_t &result) noexcept {
    if (!StepUtilityAvailable())
        return false;
    ScenarioStack stack{};
    ScenarioContext context{nullptr, &stack};
    StepUtilityParameters params{0, 0};
    ShenmueScriptFuncs[kStepUtilityFunctionIndex](&context, &params);
    const std::uint32_t ones = stack.accumulator;
    params.mode = 1;
    stack.accumulator = 0;
    ShenmueScriptFuncs[kStepUtilityFunctionIndex](&context, &params);
    const std::uint32_t tens = stack.accumulator;
    if (ones > 9 || tens > 3276 || tens * 10u + ones > 32767u)
        return false;
    result = static_cast<std::int16_t>(tens * 10u + ones);
    return true;
}

void SetCurrentScenarioStep(std::uint16_t value) noexcept {
    ScenarioStack stack{};
    ScenarioContext context{nullptr, &stack};
    StepUtilityParameters params{2, value};
    ShenmueScriptFuncs[kStepUtilityFunctionIndex](&context, &params);
}

void Script_SetToHome(HomeSnapshot &home, std::uint8_t *auxiliary) noexcept {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!ryo) {
        home.positionX = 0.0f;
        home.positionY = 0.0f;
        home.positionZ = 0.0f;
        home.yawSum = 0;
        home.eventFlags = 0;
        home.stairGuideEntryId = 0;
        home.stairGuideActive = 0;
        home.shouldSleep = 0;
        home.sleepPromptStartMinutes = 0;
        home.sleepPromptEndMinutes = 0;
        home.forcedSleepStartMinutes = 0;
        home.forcedSleepEndMinutes = 0;
        home.homeAreaId = 0;
        home.requestEntryPoint = 0;
        home.homeActNumber = 0;
        home.dateTime = {};
        home.word2C = 0;
        home.unknown2E[0] = 0;
        home.unknown2E[1] = 0;
        return;
    }
    const Vector3 position = GetPositionForEntity(ryo->entity.node);
    home.positionX = position.x;
    home.positionY = position.y;
    home.positionZ = position.z;
    std::int16_t yawSum = 0;
    for (const CharacterNode *node = &ryo->entity.node; node; node = node->child)
        yawSum = static_cast<std::int16_t>(yawSum + static_cast<std::uint16_t>(node->rotation.y));
    home.yawSum = yawSum;
    home.eventFlags = static_cast<std::uint8_t>(ryo->eventGroup);
    home.stairGuideEntryId = static_cast<std::uint8_t>(g_temporaryKaidanId);
    home.stairGuideActive = static_cast<std::uint8_t>(g_playerKaidanGuideActive);
    home.homeActNumber = static_cast<std::uint16_t>((home.homeActNumber & 0x00ffu) | (static_cast<std::uint16_t>(g_shouldSleep) << 8u));
    home.sleepPromptStartMinutes = static_cast<std::uint16_t>(g_sleepPromptWindowStartMinutes);
    home.sleepPromptEndMinutes = static_cast<std::uint16_t>(g_sleepPromptWindowEndMinutes);
    home.forcedSleepStartMinutes = static_cast<std::uint16_t>(g_forcedSleepWindowStartMinutes);
    home.forcedSleepEndMinutes = static_cast<std::uint16_t>(g_forcedSleepWindowEndMinutes);
    home.homeAreaId = g_homeAreaId.name;
    home.homeActNumber = static_cast<std::uint16_t>((home.homeActNumber & 0xff00u) | static_cast<std::uint8_t>(g_homeSceneId));
    home.requestEntryPoint = static_cast<std::uint16_t>(g_requestedEntryPoint);
    std::memcpy(&home.dateTime, &g_temporaryDateTime, sizeof(home.dateTime));
    home.word2C = g_temporaryDateTimeWord;
    std::memcpy(auxiliary, g_ryoHomeSleepAuxBlock.data(), g_ryoHomeSleepAuxBlock.size());
    std::memcpy(auxiliary + g_ryoHomeSleepAuxBlock.size(), g_ryoHomeSleepAuxBlock1.data(), 8);
}

template <typename T> T ReadPacked(const std::uint8_t *bytes, std::size_t offset) noexcept {
    T value{};
    std::memcpy(&value, bytes + offset, sizeof(value));
    return value;
}

template <typename T> void WritePacked(std::uint8_t *bytes, std::size_t offset, T value) noexcept {
    std::memcpy(bytes + offset, &value, sizeof(value));
}

template <typename T> void MergeDelta(std::uint8_t *aggregate, const std::uint8_t *current, const std::uint8_t *previous, std::size_t offset) noexcept {
    const std::int64_t merged = static_cast<std::int64_t>(ReadPacked<T>(aggregate, offset)) + static_cast<std::int64_t>(ReadPacked<T>(current, offset)) -
        static_cast<std::int64_t>(ReadPacked<T>(previous, offset));
    WritePacked<T>(aggregate, offset, static_cast<T>(std::clamp<std::int64_t>(merged, 0, std::numeric_limits<T>::max())));
}

void MergeDelta32(std::uint8_t *aggregate, const std::uint8_t *current, const std::uint8_t *previous, std::size_t offset) noexcept {
    WritePacked<std::uint32_t>(aggregate, offset,
        ReadPacked<std::uint32_t>(aggregate, offset) + ReadPacked<std::uint32_t>(current, offset) - ReadPacked<std::uint32_t>(previous, offset));
}

template <typename T> void MergeMinimum(std::uint8_t *aggregate, const std::uint8_t *current, std::size_t offset) noexcept {
    const T candidate = ReadPacked<T>(current, offset);
    const T existing = ReadPacked<T>(aggregate, offset);
    if (existing == 0 || (candidate != 0 && candidate < existing))
        WritePacked<T>(aggregate, offset, candidate);
}

template <typename T> void MergeMaximum(std::uint8_t *aggregate, T candidate, std::size_t offset) noexcept {
    if (candidate > ReadPacked<T>(aggregate, offset))
        WritePacked<T>(aggregate, offset, candidate);
}

void MergeSparsePairStats(std::uint8_t *aggregate, const std::uint8_t *current, const std::uint8_t *previous) noexcept {
    constexpr std::size_t offset = offsetof(PlayerStatsPages, sparsePairU8Slots);
    constexpr std::size_t count = std::size(PlayerStatsPages{}.sparsePairU8Slots);
    std::array<PlayerStatSparsePairSlot, count> global{};
    std::array<PlayerStatSparsePairSlot, count> old{};
    std::array<PlayerStatSparsePairSlot, count> now{};
    std::memcpy(global.data(), aggregate + offset, sizeof(global));
    std::memcpy(old.data(), previous + offset, sizeof(old));
    std::memcpy(now.data(), current + offset, sizeof(now));

    for (PlayerStatSparsePairSlot &slot : global) {
        if (slot.key0 == 0xff)
            break;
        if (slot.value == 0) {
            slot.key0 = 0xff;
            slot.key1 = 0xff;
            break;
        }
    }
    const auto valueFor = [](const auto &slots, std::uint8_t key0, std::uint8_t key1) {
        for (const PlayerStatSparsePairSlot &slot : slots) {
            if (slot.key0 == 0xff)
                break;
            if (slot.key0 == key0 && slot.key1 == key1)
                return slot.value;
        }
        return static_cast<std::uint8_t>(0);
    };
    for (const PlayerStatSparsePairSlot &slot : now) {
        if (slot.key0 == 0xff)
            break;
        PlayerStatSparsePairSlot *destination = nullptr;
        for (PlayerStatSparsePairSlot &candidate : global) {
            if (candidate.key0 == slot.key0 && candidate.key1 == slot.key1) {
                destination = &candidate;
                break;
            }
            if (candidate.key0 == 0xff) {
                candidate.key0 = slot.key0;
                candidate.key1 = slot.key1;
                candidate.value = 0;
                destination = &candidate;
                break;
            }
        }
        if (!destination) {
            global[0] = {slot.key0, slot.key1, 0};
            if (global.size() > 1)
                global[1].key0 = 0xff;
            destination = &global[0];
        }
        const std::int32_t merged = destination->value + slot.value - valueFor(old, slot.key0, slot.key1);
        destination->value = static_cast<std::uint8_t>(std::clamp(merged, 0, 255));
    }
    std::memcpy(aggregate + offset, global.data(), sizeof(global));
}

void MergeWeightedStat(std::uint8_t *aggregate, const std::uint8_t *current, const std::uint8_t *previous, std::size_t slot) noexcept {
    const std::size_t countOffset = 0xe70 + slot * 2;
    const std::size_t valueOffset = 0xe74 + slot * 2;
    const std::int32_t oldCount = ReadPacked<std::uint16_t>(previous, countOffset);
    const std::int32_t globalCount = ReadPacked<std::uint16_t>(aggregate, countOffset);
    const std::int32_t currentCount = ReadPacked<std::uint16_t>(current, countOffset);
    const std::int32_t mergedCount = currentCount - oldCount + globalCount;
    std::uint16_t mergedValue = 0;
    if (mergedCount > 0) {
        const std::int64_t weighted = static_cast<std::int64_t>(globalCount) * ReadPacked<std::uint16_t>(aggregate, valueOffset) +
            static_cast<std::int64_t>(currentCount) * ReadPacked<std::uint16_t>(current, valueOffset) -
            static_cast<std::int64_t>(oldCount) * ReadPacked<std::uint16_t>(previous, valueOffset);
        mergedValue = static_cast<std::uint16_t>(std::clamp<std::int64_t>(weighted / mergedCount, 0, 0xffff));
    }
    WritePacked<std::uint16_t>(aggregate, countOffset, static_cast<std::uint16_t>(std::clamp(mergedCount, 0, 0xffff)));
    WritePacked<std::uint16_t>(aggregate, valueOffset, mergedValue);
}

void MergeBookkeepingStats(const NativeSaveFile &save) noexcept {
    std::uint8_t *const aggregate = g_bookkeepingStatsCache.globalStatsMirror.data();
    std::uint8_t *const current = reinterpret_cast<std::uint8_t *>(&g_playerStats.pages);
    static constexpr PlayerStatsPages blankPrevious{.sparsePairU8Slots = {{.key0 = 0xff, .key1 = 0xff}}};
    const std::uint8_t *const previousBytes = g_loadedSaveBlock < 0 ? reinterpret_cast<const std::uint8_t *>(&blankPrevious) :
                                                                      reinterpret_cast<const std::uint8_t *>(&save) + kS1SaveMainBlockOffset -
            sizeof(MainSaveBlock) + static_cast<std::size_t>(g_loadedSaveBlock) * sizeof(MainSaveBlock) + offsetof(MainSaveBlock, statPages);
    const std::span<const std::uint8_t> previous{previousBytes, sizeof(PlayerStatsPages)};

    std::uint16_t saveCount = ReadPacked<std::uint16_t>(current, 0);
    if (saveCount != 0xffff)
        ++saveCount;
    WritePacked<std::uint16_t>(current, 0, saveCount);
    std::uint16_t globalSaveCount = ReadPacked<std::uint16_t>(aggregate, 0);
    if (globalSaveCount != 0xffff)
        ++globalSaveCount;
    WritePacked<std::uint16_t>(aggregate, 0, globalSaveCount);
    MergeSparsePairStats(aggregate, current, previous.data());
    MergeWeightedStat(aggregate, current, previous.data(), 0);
    MergeWeightedStat(aggregate, current, previous.data(), 1);

    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 2);
    MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 4);
    std::uint8_t learned = 0;
    for (const MoveSetRuntimeState &moveSet : g_playerStats.moveSets) {
        const std::size_t count = std::min<std::size_t>(moveSet.count, moveSet.learnValue.size());
        for (std::size_t index = 0; index < count; ++index)
            learned = static_cast<std::uint8_t>(learned + (moveSet.learnValue[index] >= 12));
    }
    MergeMaximum<std::uint8_t>(aggregate, learned, 6);
    MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 7);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 8);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 10);
    for (std::size_t offset : {12u, 16u, 20u, 24u, 28u, 36u, 40u, 48u})
        MergeDelta32(aggregate, current, previous.data(), offset);
    MergeMinimum<std::uint32_t>(aggregate, current, 32);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 44);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 46);
    MergeMinimum<std::uint32_t>(aggregate, current, 52);
    MergeMinimum<std::uint32_t>(aggregate, current, 3744);

    for (std::size_t index = 0; index < 4; ++index) {
        const PlayerStatShadowRecord &record = g_playerStatShadowRecords[index];
        const std::uint32_t count =
            ReadPacked<std::uint16_t>(aggregate, 56 + index * 2) + record.count - ReadPacked<std::uint16_t>(previous.data(), 56 + index * 2);
        WritePacked<std::uint16_t>(aggregate, 56 + index * 2, static_cast<std::uint16_t>(std::clamp(std::bit_cast<std::int32_t>(count), 0, 0xffff)));
        WritePacked<std::uint32_t>(aggregate, 88 + index * 4,
            ReadPacked<std::uint32_t>(aggregate, 88 + index * 4) + record.accumulatedValue - ReadPacked<std::uint32_t>(previous.data(), 88 + index * 4));
        MergeMaximum<std::uint32_t>(aggregate, record.bestValue, 152 + index * 4);
    }
    for (std::size_t index = 0; index < 7; ++index) {
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 64 + index * 2);
        MergeDelta32(aggregate, current, previous.data(), 104 + index * 4);
    }

    constexpr std::array<std::size_t, 7> scoreSlots{2, 8, 10, 12, 14, 0, 5};
    for (std::size_t index = 0; index < scoreSlots.size(); ++index)
        MergeMaximum<std::uint32_t>(aggregate, g_arcadeHighScores[scoreSlots[index]], 168 + index * 4);
    MergeMaximum<std::uint32_t>(aggregate, g_playerStats.segaCoins, 216);
    MergeDelta32(aggregate, current, previous.data(), 220);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 3740);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 772);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 774);

    for (std::size_t index = 0; index < 16; ++index) {
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 228 + index * 2);
        MergeDelta32(aggregate, current, previous.data(), 260 + index * 4);
        MergeDelta32(aggregate, current, previous.data(), 324 + index * 4);
        MergeDelta32(aggregate, current, previous.data(), 3760 + index * 4);
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 2924 + index * 2);
        MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 2996 + index);
    }
    for (std::size_t index = 0; index < 128; ++index) {
        MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 776 + index);
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 2668 + index * 2);
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 3012 + index * 2);
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 3268 + index * 2);
    }
    WritePacked<std::uint32_t>(aggregate, 904, ReadPacked<std::uint32_t>(aggregate, 904) | ReadPacked<std::uint32_t>(current, 904));
    for (std::size_t index = 0; index < 32; ++index) {
        MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 908 + index);
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 940 + index * 2);
        MergeMinimum<std::uint32_t>(aggregate, current, 3524 + index * 4);
        MergeMinimum<std::uint8_t>(aggregate, current, 3652 + index);
        MergeMinimum<std::uint8_t>(aggregate, current, 3704 + index);
    }
    for (std::size_t index = 0; index < 256; ++index)
        MergeDelta32(aggregate, current, previous.data(), 1004 + index * 4);
    for (std::size_t index = 0; index < 512; ++index)
        MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 2028 + index);
    for (std::size_t index = 0; index < 256; ++index)
        MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 2540 + index);
    for (std::size_t index = 0; index < 8; ++index)
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 2956 + index * 2);
    MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 3684);
    for (std::size_t index = 0; index < 4; ++index)
        MergeDelta<std::uint8_t>(aggregate, current, previous.data(), 3736 + index);
    for (std::size_t index = 0; index < 5; ++index) {
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 3748 + index * 2);
        MergeDelta32(aggregate, current, previous.data(), 3824 + index * 4);
        MergeDelta<std::uint16_t>(aggregate, current, previous.data(), 3844 + index * 2);
    }
}

void CaptureMoveSet(MoveSetSave &destination, const MoveSetRuntimeState &source) noexcept {
    const std::size_t count = std::min<std::size_t>(source.count, source.learnValue.size());
    std::copy_n(source.learnValue.begin(), count, destination.learnValue.begin());
    std::copy_n(source.moveValue.begin(), count, destination.moveValue.begin());
    destination.count = source.count;
    destination.countOrUnknown = source.countOrUnknown;
}

std::uint8_t CountLearnedMoves(const MainSaveBlock &main) noexcept {
    std::uint8_t learned = 0;
    const MoveSetSave *moveSets[]{&main.handMoves, &main.legMoves, &main.throwMoves};
    for (const MoveSetSave *moveSet : moveSets) {
        const std::size_t count = std::min<std::size_t>(moveSet->count, moveSet->learnValue.size());
        for (std::size_t index = 0; index < count; ++index)
            learned = static_cast<std::uint8_t>(learned + (moveSet->learnValue[index] >= 12));
    }
    return learned;
}

void CaptureRuntimeFields(
    NativeSaveFile &save, const SaveDateTime &systemTime, std::int16_t scenarioStep, bool incrementSaveCount, bool skipHomeSnapshotUpdate) noexcept {
    InsertSaveFileHeader(save);
    MainSaveBlock &main = save.main;
    const PlayerStatsRuntimeState &stats = g_playerStats;

    if (incrementSaveCount)
        MergeBookkeepingStats(save);
    main.playerSavedStat0 = stats.savedFlags;
    std::copy(stats.currentVfSkill.begin(), stats.currentVfSkill.end(), std::begin(main.currentVfSkill));
    main.word02E = stats.currentMove;
    CaptureMoveSet(main.handMoves, stats.moveSets[0]);
    CaptureMoveSet(main.legMoves, stats.moveSets[1]);
    CaptureMoveSet(main.throwMoves, stats.moveSets[2]);
    main.playerRuntimeFlags = stats.runtimeFlags;
    main.cash = stats.cash;
    main.segaCoins = std::bit_cast<std::int32_t>(stats.segaCoins);
    main.alternateHp = static_cast<std::uint16_t>(stats.alternateHealth);
    main.defaultHp = static_cast<std::uint16_t>(stats.defaultHealth);
    main.cameraShakeAmount = static_cast<std::uint16_t>(stats.cameraShakeAmount);
    main.vfPlayerAssistScale = static_cast<std::uint16_t>(stats.fightAssistScale);
    main.settingsFlags = g_settingsFlags;
    save.internal.settingsFlags = g_settingsFlags;
    main.playerContextActionMode = static_cast<std::uint16_t>(stats.contextActionMode);
    main.vfHpRegenerationProfile = stats.vfHpRegenerationProfile;

    std::memcpy(&main.scenarioBitFlags64, g_scenarioStepLut.bitFlags64, sizeof(main.scenarioBitFlags64));
    std::memcpy(main.stepLut0To3, g_scenarioStepLut.steps, sizeof(main.stepLut0To3));
    std::memcpy(main.stepLut4To7, g_scenarioStepLut.steps + 16, sizeof(main.stepLut4To7));
    std::memcpy(main.shenmueFlags, g_shenmueFlags.data(), sizeof(main.shenmueFlags));

    std::memcpy(main.storyItemFlags, stats.inventoryItemCounts0To66.data(), sizeof(main.storyItemFlags));
    std::memcpy(main.gachaItemFlags, stats.inventoryItemCounts67To234.data(), sizeof(main.gachaItemFlags));
    std::memcpy(main.cassetteTapes, stats.cassetteTapes.data(), sizeof(main.cassetteTapes));
    std::memcpy(main.playerStatsStep, stats.step.data(), sizeof(main.playerStatsStep));
    std::memcpy(main.statBank4A0, stats.statBank4A0.data(), sizeof(main.statBank4A0));
    std::memcpy(main.statPages, &stats.pages, sizeof(stats.pages));
    std::memcpy(main.statPages + sizeof(stats.pages), stats.statBank4A0.data() + sizeof(main.statBank4A0), 160);
    std::copy(g_spaceHarrierScores.begin(), g_spaceHarrierScores.end(), main.spaceHarrierScores);
    std::copy(g_hangOnScores.begin(), g_hangOnScores.end(), main.hangOnScores);
    std::memcpy(main.highScoreScalarBank, g_arcadeHighScores.data(), sizeof(main.highScoreScalarBank));
    std::memcpy(main.batteryLives, stats.batteryLives.data(), sizeof(main.batteryLives));

    main.recentItemOldest = stats.recentItemKeyOldest;
    main.recentItemPrevious = stats.recentItemKeyPrevious;
    main.recentItemLatest = stats.recentItemKeyLatest;
    main.uniqueItemCount = stats.scenarioStepUniqueItemCount;
    std::copy_n(g_saveDataRelated.begin(), 4, main.saveDataRelated);
    main.saveDataRelated4 = g_saveDataRelated[4];
    main.notebookLastEntryPage = g_notebookLastEntryPage;
    main.word7FE = g_notebookPersistenceWord;
    main.statPages[6] = CountLearnedMoves(main);
    for (std::size_t index = 0; index < g_playerStatShadowRecords.size(); ++index) {
        const PlayerStatShadowRecord &record = g_playerStatShadowRecords[index];
        WritePacked<std::uint32_t>(main.statPages, 152 + index * 4, record.bestValue);
        WritePacked<std::uint16_t>(main.statPages, 56 + index * 2, static_cast<std::uint16_t>(record.count));
        WritePacked<std::uint32_t>(main.statPages, 88 + index * 4, record.accumulatedValue);
    }
    constexpr std::array<std::size_t, 7> mirroredScoreSlots{2, 8, 10, 12, 14, 0, 5};
    for (std::size_t index = 0; index < mirroredScoreSlots.size(); ++index) {
        const std::uint32_t score = g_arcadeHighScores[mirroredScoreSlots[index]];
        std::memcpy(main.statPages + 168 + index * sizeof(score), &score, sizeof(score));
    }

    if (!skipHomeSnapshotUpdate)
        Script_SetToHome(main.homeSnapshot, main.homeAuxiliary);

    std::memcpy(g_bookkeepingStatsCache.globalStatsMirror.data() + kInventoryIdsMirrorOffset, stats.inventoryItemIds.data(), sizeof(stats.inventoryItemIds));
    std::memcpy(g_bookkeepingStatsCache.globalStatsMirror.data() + kInventoryCountMirrorOffset, &stats.inventoryItemCount, sizeof(stats.inventoryItemCount));

    SaveSummary &summary = save.summary;
    summary.systemTime = systemTime;
    summary.gameTime = ToSaveDateTime(g_currentDateTime);
    if (incrementSaveCount) {
        const std::uint16_t next = static_cast<std::uint16_t>(std::bit_cast<std::uint16_t>(summary.numberOfSaves) + 1u);
        summary.numberOfSaves = next == 0 ? static_cast<std::int16_t>(-1) : std::bit_cast<std::int16_t>(next);
    }
    summary.actNumber = static_cast<std::int16_t>(g_actNumber + (g_saveDisplayModuleState == 4 ? 1 : 0));
    summary.scenarioStep = static_cast<std::uint16_t>(scenarioStep);
    summary.sceneId = static_cast<std::int16_t>(g_temporarySaveSceneId);
    summary.areaId = g_temporarySaveAreaId;
    summary.entryId = static_cast<std::int32_t>(g_temporarySaveEntryPoint);
    summary.saveState = static_cast<std::uint8_t>(std::clamp(g_saveDisplayModuleState, 0, 255));

    const std::uint8_t currentProgress = static_cast<std::uint8_t>(stats.currentSaveProgress);
    if (save.internal.progress < currentProgress) {
        save.internal.progress = currentProgress;
        const std::uint32_t currentFrame = static_cast<std::uint32_t>(std::max(g_ctrlInitializedFrameCount, 0));
        const std::uint32_t previousMinimum = ReadPacked<std::uint32_t>(g_bookkeepingStatsCache.globalStatsMirror.data(), 0x1404);
        WritePacked<std::uint32_t>(
            g_bookkeepingStatsCache.globalStatsMirror.data(), 0x1404, previousMinimum != 0 && previousMinimum < currentFrame ? previousMinimum : currentFrame);
        if (g_bookkeepingStatsCache.aggregateSaveProgress != 0xff)
            ++g_bookkeepingStatsCache.aggregateSaveProgress;
    }
    std::memcpy(save.globalStatsMirror, g_bookkeepingStatsCache.globalStatsMirror.data(), g_bookkeepingStatsCache.globalStatsMirror.size());
    FinalizeChecksums(save);
}

void RemoveTemporaryFile(const std::filesystem::path &path) noexcept {
    std::error_code ignored;
    std::filesystem::remove(path, ignored);
}

std::filesystem::path MakeTemporaryPath(const std::filesystem::path &destination) {
    static std::atomic<std::uint64_t> sequence{};
    const auto ticks = static_cast<std::uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::filesystem::path temporary = destination;
    temporary += std::filesystem::path(".tmp-" + std::to_string(ticks) + "-" + std::to_string(sequence.fetch_add(1, std::memory_order_relaxed)));
    return temporary;
}

bool FlushFileToDisk(const std::filesystem::path &path) noexcept {
#ifdef _WIN32
    HANDLE const file = CreateFileW(
        path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, nullptr);
    if (file == INVALID_HANDLE_VALUE)
        return false;
    const bool flushed = FlushFileBuffers(file) != 0;
    CloseHandle(file);
    return flushed;
#else
    const int file = ::open(path.c_str(), O_RDONLY);
    if (file < 0)
        return false;
    const bool flushed = ::fsync(file) == 0;
    ::close(file);
    return flushed;
#endif
}

void SyncParentDirectory(const std::filesystem::path &destination) noexcept {
#ifndef _WIN32
    const std::filesystem::path parent = destination.parent_path().empty() ? std::filesystem::path(".") : destination.parent_path();
    const int directory = ::open(parent.c_str(), O_RDONLY | O_DIRECTORY);
    if (directory < 0)
        return;
    ::fsync(directory);
    ::close(directory);
#else
    (void)destination;
#endif
}

S1SaveResult CommitTemporaryFile(const std::filesystem::path &temporary, const std::filesystem::path &destination) {
#ifdef _WIN32
    if (!MoveFileExW(temporary.c_str(), destination.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        return S1SaveResult::CommitFailed;
    }
#else
    std::error_code error;
    std::filesystem::rename(temporary, destination, error);
    if (error)
        return S1SaveResult::CommitFailed;
    SyncParentDirectory(destination);
#endif
    return S1SaveResult::Success;
}

S1SaveResult ReadImageBytes(const std::filesystem::path &path, std::span<std::uint8_t, kS1SaveImageSize> bytes, bool requireExactSize = true) {
    if (path.empty())
        return S1SaveResult::InvalidArgument;
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
        return S1SaveResult::OpenFailed;
    const std::streampos size = file.tellg();
    if (size < static_cast<std::streampos>(kS1SaveImageSize) || (requireExactSize && size != static_cast<std::streampos>(kS1SaveImageSize)))
        return S1SaveResult::InvalidSize;
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    return file ? S1SaveResult::Success : S1SaveResult::ReadFailed;
}

} // namespace

std::uint8_t g_homeSnapshotApplyPending = 0;
std::int32_t g_loadedSaveBlock = 0;

S1SaveResult S1Save_Validate(const S1SaveImage &image) noexcept {
    NativeSaveFile save{};
    LoadLayout(image, save);
    return ValidateLayout(save);
}



void shenUpdateFighterStats(S1SaveImage &image) {
    const PlayerStatsPages &pages = g_playerStats.pages;
    std::uint8_t *const targets[2]{image.bytes.data() + kS1SaveGlobalStatsOffset, g_bookkeepingStatsCache.globalStatsMirror.data()};
    for (std::uint8_t *const stats : targets) {
        std::memcpy(stats + offsetof(PlayerStatsPages, save70MBSamples), pages.save70MBSamples, sizeof(pages.save70MBSamples));
        std::memcpy(stats + offsetof(PlayerStatsPages, bestTimeTicks), &pages.bestTimeTicks, sizeof(pages.bestTimeTicks));
        std::memcpy(stats + offsetof(PlayerStatsPages, stateTick70MB), &pages.stateTick70MB, sizeof(pages.stateTick70MB));
        std::memcpy(stats + offsetof(PlayerStatsPages, attemptCount70MB), &pages.attemptCount70MB, sizeof(pages.attemptCount70MB));
        std::memcpy(stats + offsetof(PlayerStatsPages, timeAccum70MB), pages.timeAccum70MB, sizeof(std::uint32_t));
        std::memcpy(stats + offsetof(PlayerStatsPages, totalDefeated70MB), &pages.totalDefeated70MB, sizeof(pages.totalDefeated70MB));
    }
}



void S1Save_InitializeImage(S1SaveImage &image) {
    NativeSaveFile save{};
    save.internal.settingsFlags = g_settingsFlags;
    save.internal.identifier = kIdentifier;
    save.internal.version = kVersion;
    save.internal.byte009 = 0;
    FinalizeChecksums(save);
    std::memcpy(save.globalStatsMirror, g_bookkeepingStatsCache.globalStatsMirror.data(), g_bookkeepingStatsCache.globalStatsMirror.size());
    save.internal.progress = g_bookkeepingStatsCache.aggregateSaveProgress;
    StoreLayout(save, image);
}

S1SaveResult S1Save_Capture(S1SaveImage &image, bool skipHomeSnapshotUpdate) {
    NativeSaveFile candidate{};
    LoadLayout(image, candidate);
    if (candidate.internal.identifier == 0) {
        candidate = {};
        candidate.internal.identifier = kIdentifier;
        candidate.internal.version = kVersion;
        candidate.internal.settingsFlags = g_settingsFlags;
        candidate.internal.progress = g_bookkeepingStatsCache.aggregateSaveProgress;
    }

    std::int16_t scenarioStep = 0;
    if (!ReadCurrentScenarioStep(scenarioStep))
        return S1SaveResult::Unavailable;
    SaveDateTime systemTime{};
    if (!GetLocalSystemTime(systemTime))
        return S1SaveResult::Unavailable;

    CaptureRuntimeFields(candidate, systemTime, scenarioStep, true, skipHomeSnapshotUpdate);
    g_loadedSaveBlock = 1;
    const S1SaveResult result = ValidateLayout(candidate);
    if (result != S1SaveResult::Success)
        return result;
    StoreLayout(candidate, image);
    return S1SaveResult::Success;
}

S1SaveResult S1Save_Read(const std::filesystem::path &path, S1SaveImage &image) {
    S1SaveImage candidate{};
    const S1SaveResult readResult = ReadImageBytes(path, candidate.bytes);
    if (readResult != S1SaveResult::Success)
        return readResult;
    const S1SaveResult validation = S1Save_Validate(candidate);
    if (validation != S1SaveResult::Success)
        return validation;
    image = candidate;
    return S1SaveResult::Success;
}


std::int32_t S1Save_ReadCurrentSlot(const std::filesystem::path &path, S1SaveImage &image) {
    if (ReadImageBytes(path, image.bytes, false) != S1SaveResult::Success)
        return -4;
    const std::uint8_t *const header = image.bytes.data() + kS1SaveInternalHeaderOffset;
    if (ReadPacked<std::uint32_t>(header, 0) != kIdentifier || ReadPacked<std::uint32_t>(header, 4) != kVersion)
        return -6;
    RestoreCurrentSlotMetadata(header[8]);
    return 0;
}

S1SaveResult S1Save_Load(const std::filesystem::path &path, S1SaveImage &image, std::int32_t block) {
    g_loadedSaveBlock = -1;
    S1SaveImage candidate{};
    const S1SaveResult readResult = ReadImageBytes(path, candidate.bytes);
    if (readResult != S1SaveResult::Success)
        return readResult;
    image = candidate;

    NativeSaveFile save{};
    LoadLayout(candidate, save);
    if (save.internal.identifier == kIdentifier && save.internal.version == kVersion)
        RestoreCurrentSlotMetadata(save.internal.progress);

    const S1SaveResult validation = ValidateSceneBlock(save, block);
    if (validation != S1SaveResult::Success)
        return validation;
    g_loadedSaveBlock = block;
    const S1SaveResult applied = S1Save_Apply(candidate, block);
    if (applied != S1SaveResult::Success)
        return applied;

    PlayerStatsPages &pages = g_playerStats.pages;
    const std::uint8_t *const stats = g_bookkeepingStatsCache.globalStatsMirror.data();
    std::memcpy(&pages.attemptCount70MB, stats + offsetof(PlayerStatsPages, attemptCount70MB), sizeof(pages.attemptCount70MB));
    std::memcpy(pages.timeAccum70MB, stats + offsetof(PlayerStatsPages, timeAccum70MB), sizeof(std::uint32_t));
    std::memcpy(&pages.totalDefeated70MB, stats + offsetof(PlayerStatsPages, totalDefeated70MB), sizeof(pages.totalDefeated70MB));
    std::memcpy(pages.save70MBSamples, stats + offsetof(PlayerStatsPages, save70MBSamples), sizeof(pages.save70MBSamples));
    std::memcpy(&pages.bestTimeTicks, stats + offsetof(PlayerStatsPages, bestTimeTicks), sizeof(pages.bestTimeTicks));
    std::memcpy(&pages.stateTick70MB, stats + offsetof(PlayerStatsPages, stateTick70MB), sizeof(pages.stateTick70MB));
    return S1SaveResult::Success;
}

S1SaveResult S1Save_ReadMetadata(const std::filesystem::path &path, S1SaveMetadata &metadata) {
    S1SaveImage image{};
    const S1SaveResult result = S1Save_Read(path, image);
    if (result != S1SaveResult::Success)
        return result;

    NativeSaveFile save{};
    LoadLayout(image, save);
    const auto copyDateTime = [](const SaveDateTime &source) {
        return S1SaveDateTime{source.year, source.month, source.day, source.hour, source.minute, source.second, source.dayOfWeek};
    };
    S1SaveMetadata candidate{};
    candidate.systemTime = copyDateTime(save.summary.systemTime);
    candidate.gameTime = copyDateTime(save.summary.gameTime);
    candidate.numberOfSaves = save.summary.numberOfSaves;
    candidate.actNumber = save.summary.actNumber;
    candidate.scenarioStep = save.summary.scenarioStep;
    candidate.sceneId = save.summary.sceneId;
    candidate.areaId = save.summary.areaId;
    candidate.entryId = save.summary.entryId;
    candidate.saveState = save.summary.saveState;
    candidate.progress = save.internal.progress;
    metadata = candidate;
    return S1SaveResult::Success;
}

S1SaveResult S1Save_Write(const std::filesystem::path &path, S1SaveImage &image) {
    if (path.empty())
        return S1SaveResult::InvalidArgument;
    const S1SaveResult sourceValidation = S1Save_Validate(image);
    if (sourceValidation != S1SaveResult::Success)
        return sourceValidation;

    NativeSaveFile save{};
    LoadLayout(image, save);
    InsertSaveFileHeader(save);
    FinalizeChecksums(save);
    const S1SaveResult finalizedValidation = ValidateLayout(save);
    if (finalizedValidation != S1SaveResult::Success)
        return finalizedValidation;

    if (const std::filesystem::path parent = path.parent_path(); !parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        if (error)
            return S1SaveResult::OpenFailed;
    }

    const std::filesystem::path temporary = MakeTemporaryPath(path);
    {
        std::ofstream file(temporary, std::ios::binary | std::ios::trunc);
        if (!file) {
            RemoveTemporaryFile(temporary);
            return S1SaveResult::OpenFailed;
        }
        file.write(reinterpret_cast<const char *>(&save), sizeof(save));
        file.flush();
        if (!file) {
            file.close();
            RemoveTemporaryFile(temporary);
            return S1SaveResult::WriteFailed;
        }
        file.close();
        if (!file) {
            RemoveTemporaryFile(temporary);
            return S1SaveResult::WriteFailed;
        }
    }

    if (!FlushFileToDisk(temporary)) {
        RemoveTemporaryFile(temporary);
        return S1SaveResult::WriteFailed;
    }
    S1SaveImage verified{};
    const S1SaveResult verification = S1Save_Read(temporary, verified);
    S1SaveImage expected{};
    StoreLayout(save, expected);
    if (verification != S1SaveResult::Success || verified.bytes != expected.bytes) {
        RemoveTemporaryFile(temporary);
        return verification == S1SaveResult::Success ? S1SaveResult::WriteFailed : verification;
    }

    const S1SaveResult commit = CommitTemporaryFile(temporary, path);
    if (commit != S1SaveResult::Success) {
        RemoveTemporaryFile(temporary);
        return commit;
    }
    StoreLayout(save, image);
    return S1SaveResult::Success;
}

S1SaveResult S1Save_Apply(const S1SaveImage &image, std::int32_t block) {
    NativeSaveFile save{};
    LoadLayout(image, save);
    const S1SaveResult validation = ValidateSceneBlock(save, block);
    if (validation != S1SaveResult::Success)
        return validation;
    if (!StepUtilityAvailable())
        return S1SaveResult::Unavailable;

    const std::uint8_t *const bytes = reinterpret_cast<const std::uint8_t *>(&save);
    const MainSaveBlock &main = *reinterpret_cast<const MainSaveBlock *>(
        bytes + kS1SaveMainBlockOffset - sizeof(MainSaveBlock) + static_cast<std::size_t>(block) * sizeof(MainSaveBlock));
    const SaveSummary &summary =
        *reinterpret_cast<const SaveSummary *>(bytes + kS1SaveSummaryOffset - sizeof(SaveSummary) + static_cast<std::size_t>(block) * sizeof(SaveSummary));
    PlayerStatsRuntimeState stats = g_playerStats;
    stats.savedFlags = main.playerSavedStat0;
    std::copy(std::begin(main.currentVfSkill), std::end(main.currentVfSkill), stats.currentVfSkill.begin());
    stats.currentMove = main.word02E;
    stats.moveSets = {main.handMoves, main.legMoves, main.throwMoves};
    stats.runtimeFlags = main.playerRuntimeFlags;
    stats.cash = main.cash;
    stats.segaCoins = std::bit_cast<std::uint32_t>(main.segaCoins);
    stats.alternateHealth = main.alternateHp;
    stats.defaultHealth = main.defaultHp;
    stats.cameraShakeAmount = main.cameraShakeAmount;
    stats.fightAssistScale = main.vfPlayerAssistScale;
    stats.contextActionMode = main.playerContextActionMode;
    stats.vfHpRegenerationProfile = main.vfHpRegenerationProfile;
    std::memcpy(stats.step.data(), main.playerStatsStep, sizeof(main.playerStatsStep));
    std::memcpy(stats.statBank4A0.data(), main.statBank4A0, sizeof(main.statBank4A0));
    std::memcpy(&stats.pages, main.statPages, sizeof(stats.pages));
    std::memcpy(stats.statBank4A0.data() + sizeof(main.statBank4A0), main.statPages + sizeof(stats.pages), 160);
    std::memcpy(stats.inventoryItemCounts0To66.data(), main.storyItemFlags, sizeof(main.storyItemFlags));
    std::memcpy(stats.inventoryItemCounts67To234.data(), main.gachaItemFlags, sizeof(main.gachaItemFlags));
    std::memcpy(stats.cassetteTapes.data(), main.cassetteTapes, sizeof(main.cassetteTapes));
    std::memcpy(stats.batteryLives.data(), main.batteryLives, sizeof(main.batteryLives));
    stats.recentItemKeyOldest = main.recentItemOldest;
    stats.recentItemKeyPrevious = main.recentItemPrevious;
    stats.recentItemKeyLatest = main.recentItemLatest;
    stats.scenarioStepUniqueItemCount = main.uniqueItemCount;

    ScenarioStepTable scenarioStepLut = g_scenarioStepLut;
    std::memcpy(scenarioStepLut.steps, main.stepLut0To3, sizeof(main.stepLut0To3));
    std::memcpy(scenarioStepLut.steps + 16, main.stepLut4To7, sizeof(main.stepLut4To7));
    std::memcpy(scenarioStepLut.bitFlags64, &main.scenarioBitFlags64, sizeof(main.scenarioBitFlags64));
    std::array<std::uint8_t, 128> shenmueFlags{};
    std::memcpy(shenmueFlags.data(), main.shenmueFlags, sizeof(main.shenmueFlags));
    std::array<std::uint32_t, 32> arcadeHighScores = g_arcadeHighScores;
    std::array<ArcadeScoreEntry, 20> spaceHarrierScores{};
    std::copy(std::begin(main.spaceHarrierScores), std::end(main.spaceHarrierScores), spaceHarrierScores.begin());
    std::array<ArcadeScoreEntry, 20> hangOnScores{};
    std::copy(std::begin(main.hangOnScores), std::end(main.hangOnScores), hangOnScores.begin());
    std::memcpy(arcadeHighScores.data(), main.highScoreScalarBank, sizeof(main.highScoreScalarBank));
    std::array<std::uint32_t, 5> saveDataRelated{};
    std::copy(std::begin(main.saveDataRelated), std::end(main.saveDataRelated), saveDataRelated.begin());
    saveDataRelated[4] = main.saveDataRelated4;
    const GameDateTime gameTime = ToGameDateTime(summary.gameTime);

    g_playerStats = stats;
    for (std::size_t index = 0; index < g_playerStatShadowRecords.size(); ++index) {
        g_playerStatShadowRecords[index] = {
            .bestValue = ReadPacked<std::uint32_t>(main.statPages, 152 + index * 4),
            .count = ReadPacked<std::uint16_t>(main.statPages, 56 + index * 2),
            .accumulatedValue = ReadPacked<std::uint32_t>(main.statPages, 88 + index * 4),
        };
    }
    g_notebookLastEntryPage = main.notebookLastEntryPage;
    g_notebookPersistenceWord = main.word7FE;
    g_settingsFlags = main.settingsFlags;
    g_scenarioStepLut = scenarioStepLut;
    g_shenmueFlags = shenmueFlags;
    g_arcadeHighScores = arcadeHighScores;
    g_spaceHarrierScores = spaceHarrierScores;
    g_hangOnScores = hangOnScores;
    g_saveDataRelated = saveDataRelated;
    g_ryoHomeSleepPosition = {main.homeSnapshot.positionX, main.homeSnapshot.positionY, main.homeSnapshot.positionZ};
    g_ryoHomeSleepYaw = static_cast<std::uint16_t>(main.homeSnapshot.yawSum);
    g_temporaryRyoEventFlags = main.homeSnapshot.eventFlags;
    g_defaultKaidanId = main.homeSnapshot.stairGuideEntryId;
    g_ryoHomeSleepGuideWord =
        static_cast<std::uint16_t>(main.homeSnapshot.stairGuideActive | (static_cast<std::uint16_t>(main.homeSnapshot.shouldSleep) << 8u));
    g_pendingMapStartSnapshot = {
        static_cast<std::uint8_t>(main.homeSnapshot.homeActNumber >> 8u),
        main.homeSnapshot.sleepPromptStartMinutes,
        main.homeSnapshot.sleepPromptEndMinutes,
        main.homeSnapshot.forcedSleepStartMinutes,
        main.homeSnapshot.forcedSleepEndMinutes,
        shen_name{main.homeSnapshot.homeAreaId},
        static_cast<std::int32_t>(main.homeSnapshot.homeActNumber & 0xffu),
        main.homeSnapshot.requestEntryPoint,
        {},
        main.homeSnapshot.word2C,
    };
    std::memcpy(&g_pendingMapStartSnapshot.temporaryDateTime, &main.homeSnapshot.dateTime, sizeof(g_pendingMapStartSnapshot.temporaryDateTime));
    std::memcpy(g_ryoHomeSleepAuxBlock.data(), main.homeAuxiliary, g_ryoHomeSleepAuxBlock.size());
    std::memcpy(g_ryoHomeSleepAuxBlock1.data(), main.homeAuxiliary + g_ryoHomeSleepAuxBlock.size(), g_ryoHomeSleepAuxBlock1.size());
    g_currentDateTime = gameTime;
    g_homeSnapshotApplyPending = 1;
    g_sceneId = summary.sceneId;
    g_areaId = summary.areaId;
    g_entryPoint = static_cast<std::uint32_t>(summary.entryId);
    g_temporarySaveSceneId = g_sceneId;
    g_temporarySaveAreaId = g_areaId;
    g_temporarySaveEntryPoint = g_entryPoint;
    if (summary.saveState == 3)
        g_pendingSleepResumeLoad = 1;
    SetCurrentScenarioStep(summary.scenarioStep);
    return S1SaveResult::Success;
}

void S1Save_ApplyPendingMapStartState() noexcept {
    if (!g_homeSnapshotApplyPending)
        return;

    g_shouldSleep = g_pendingMapStartSnapshot.shouldSleep;
    g_sleepPromptWindowStartMinutes = g_pendingMapStartSnapshot.sleepPromptStartMinutes;
    g_sleepPromptWindowEndMinutes = g_pendingMapStartSnapshot.sleepPromptEndMinutes;
    g_forcedSleepWindowStartMinutes = g_pendingMapStartSnapshot.forcedSleepStartMinutes;
    g_forcedSleepWindowEndMinutes = g_pendingMapStartSnapshot.forcedSleepEndMinutes;
    g_homeAreaId = g_pendingMapStartSnapshot.homeAreaId;
    g_homeSceneId = g_pendingMapStartSnapshot.homeSceneId;
    g_requestedEntryPoint = static_cast<std::uint32_t>(static_cast<std::int16_t>(g_pendingMapStartSnapshot.requestedEntryPoint));
    g_temporaryDateTimeWord = g_pendingMapStartSnapshot.temporaryDateTimeWord;
    g_homeSnapshotApplyPending = 0;
    g_temporaryDateTime = g_pendingMapStartSnapshot.temporaryDateTime;
}

std::uint32_t S1SettingsFlags_Get() noexcept {
    return g_settingsFlags;
}

void S1SettingsFlags_Set(std::uint32_t flags) noexcept {
    g_settingsFlags = flags;
}

} // namespace shenmue::s1
