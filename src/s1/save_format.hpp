#pragma once

#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>

namespace shenmue::s1 {

inline constexpr shen_name kS1SaveIdentifier{SHEN_NAME(SHEE)};
inline constexpr std::uint32_t kS1SaveVersion = 0x10e;
inline constexpr std::size_t kS1SaveImageSize = 0xa000;
inline constexpr std::size_t kS1SaveInternalHeaderOffset = 0x680;
inline constexpr std::size_t kS1SaveSummaryOffset = 0x740;
inline constexpr std::size_t kS1SaveMainBlockOffset = 0x2000;
inline constexpr std::size_t kS1SaveGlobalStatsOffset = 0x6800;
#pragma pack(push, 1)
struct ArcadeScoreEntry {
    std::uint32_t score;
    char initials[3];
    std::uint8_t reserved[5];
};
#pragma pack(pop)

static_assert(sizeof(ArcadeScoreEntry) == 0x0c);

struct S1SaveImage {
    std::array<std::uint8_t, kS1SaveImageSize> bytes{};
};
static_assert(sizeof(S1SaveImage) == kS1SaveImageSize);

struct S1SaveDateTime {
    std::uint16_t year{};
    std::uint8_t month{};
    std::uint8_t day{};
    std::uint8_t hour{};
    std::uint8_t minute{};
    std::uint8_t second{};
    std::uint8_t dayOfWeek{};
};

#pragma pack(push, 1)

struct S1SaveSummary {
    S1SaveDateTime systemTime;
    S1SaveDateTime gameTime;
    std::int16_t numberOfSaves;
    std::int16_t actNumber;
    std::uint16_t scenarioStep;
    std::int16_t sceneId;
    std::uint32_t areaId;
    std::int32_t entryId;
    std::uint32_t mainBlockCrc;
    std::uint8_t saveState;
    std::uint8_t unknown25[27];
};

struct BookkeepingRefreshStatus {
    std::uint32_t isLoaded{};
    std::uint32_t hasProgress{};
};

struct BookkeepingCache {
    std::array<std::uint8_t, 0x1800> globalStatsMirror{};
    std::uint8_t aggregateSaveProgress{};
    std::uint8_t progressReserved[3]{};
    std::array<S1SaveSummary, 10> saveSummaries{};
    BookkeepingRefreshStatus refreshStatus{};
};
#pragma pack(pop)
static_assert(sizeof(S1SaveSummary) == 0x40);
static_assert(offsetof(S1SaveSummary, numberOfSaves) == 0x10);
static_assert(offsetof(S1SaveSummary, areaId) == 0x18);
static_assert(offsetof(S1SaveSummary, saveState) == 0x24);
static_assert(sizeof(BookkeepingCache) == 0x1a8c);
static_assert(offsetof(BookkeepingCache, aggregateSaveProgress) == 0x1800);
static_assert(offsetof(BookkeepingCache, saveSummaries) == 0x1804);
static_assert(offsetof(BookkeepingCache, refreshStatus) == 0x1a84);
extern BookkeepingCache g_bookkeepingStatsCache;

struct S1SaveMetadata {
    S1SaveDateTime systemTime{};
    S1SaveDateTime gameTime{};
    std::int16_t numberOfSaves{};
    std::int16_t actNumber{};
    std::uint16_t scenarioStep{};
    std::int16_t sceneId{};
    std::uint32_t areaId{};
    std::int32_t entryId{};
    std::uint8_t saveState{};
    std::uint8_t progress{};
};

enum class S1SaveResult : std::uint8_t {
    Success = 0,
    Unavailable,
    InvalidArgument,
    InvalidSize,
    InvalidHeader,
    InvalidChecksum,
    InvalidData,
    OpenFailed,
    ReadFailed,
    WriteFailed,
    CommitFailed,
};

void S1Save_InitializeImage(S1SaveImage &image);
void shenUpdateFighterStats(S1SaveImage &image);





S1SaveResult S1Save_Capture(S1SaveImage &image, bool skipHomeSnapshotUpdate = false);

S1SaveResult S1Save_Validate(const S1SaveImage &image) noexcept;



S1SaveResult S1Save_Read(const std::filesystem::path &path, S1SaveImage &image);

std::int32_t S1Save_ReadCurrentSlot(const std::filesystem::path &path, S1SaveImage &image);
S1SaveResult S1Save_Load(const std::filesystem::path &path, S1SaveImage &image, std::int32_t block = 1);
S1SaveResult S1Save_ReadMetadata(const std::filesystem::path &path, S1SaveMetadata &metadata);
S1SaveResult S1Save_Write(const std::filesystem::path &path, S1SaveImage &image);
S1SaveResult S1Save_ReadStorageRange(const std::filesystem::path &path, void *destination, std::uint32_t offset, std::uint32_t size);
S1SaveResult S1Save_WriteStorageBytes(const std::filesystem::path &path, const void *source, std::uint32_t size);
void S1Save_PrepareStorageWrite(S1SaveImage &image);
S1SaveResult S1Save_Apply(const S1SaveImage &image, std::int32_t block = 1);
void S1Save_ApplyPendingMapStartState() noexcept;
// A loaded save's home snapshot waits for MapControlTask_SetSceneAndAreaID.
extern std::uint8_t g_homeSnapshotApplyPending;
extern std::int32_t g_loadedSaveBlock;

std::uint32_t S1SettingsFlags_Get() noexcept;
void S1SettingsFlags_Set(std::uint32_t flags) noexcept;

} // namespace shenmue::s1
