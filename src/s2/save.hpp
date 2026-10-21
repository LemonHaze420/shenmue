#pragma once
#include "shenmue.h"

#include "s2/hlib.hpp"
#include "s2/window.hpp"
#include "s2/arcade.hpp"
#include "common/types.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>
#include <string_view>
#include <vector>

namespace shenmue::s2 {
inline constexpr shen_name SAVE_SPRITE_FILE{SHEN_NAME(USER)};
inline constexpr shen_name SNAPSHOT_CHARACTER_ID_FILE{SHEN_NAME(SNCF)};
inline constexpr shen_name TITLE_IMAGE_SPRITE_FILE{SHEN_NAME(TTFL)};
inline constexpr shen_name TITLE_AREA_SPRITE_FILE{SHEN_NAME(NVTT)};
inline constexpr shen_name TITLE_LOCAL_AREA_SPRITE_FILE{SHEN_NAME(NVTX)};
inline constexpr std::array<std::int32_t, 24> SAVE_SCREEN_WARNING_IDS{0, 0, 14, 5, 11, 18, 4, 1, 8, 1, 10, 12, 13, 15, 16, 7, 20, 5, 3, 6, 17, 19, 2, 0};

struct PlayerPersistentState;
struct PadControlEntry;
struct LegacyInputSlot;

#pragma pack(push, 4)
struct SaveTaskData {
    std::uint16_t word0;
    std::array<std::byte, 0x58e> gap002;
    std::uint64_t field590;
    std::uint64_t field598;
    std::uint64_t field5a0;
    std::uint64_t field5a8;
    std::uint64_t field5b0;
    std::uint64_t field5b8;
    std::uint64_t field5c0;
    std::uint64_t field5c8;
    std::int64_t field5d0;
    std::uint32_t field5d8;
    std::int32_t field5dc;
    PadControlEntry *padData;
    PadControlEntry *previousPadData;
    LegacyInputSlot *inputBuffer;
    HLib::TaskCallback updateCallback;
    HLib::TaskCallback preRenderCallback;
    std::int32_t frame;
    std::int32_t selectedSlot;
    std::uint32_t field610;
    std::int32_t previousWarning;
    std::uint32_t colour;
    std::uint32_t initializationMode;
    std::int32_t screenItem;
    std::int32_t fadeTarget;
    float fadeCurrent;
    std::int32_t screenRow;
    std::int32_t screenTable;
    std::int32_t screenSelection;
    std::int32_t screenPage;
    std::int32_t screenOption;
};
#pragma pack(pop)

static_assert(sizeof(SaveTaskData) == 0x640);
static_assert(offsetof(SaveTaskData, field590) == 0x590);
static_assert(offsetof(SaveTaskData, updateCallback) == 0x5f8);
static_assert(offsetof(SaveTaskData, preRenderCallback) == 0x600);
static_assert(offsetof(SaveTaskData, screenItem) == 0x620);
static_assert(offsetof(SaveTaskData, fadeTarget) == 0x624);
static_assert(offsetof(SaveTaskData, screenRow) == 0x62c);
static_assert(offsetof(SaveTaskData, screenSelection) == 0x634);
static_assert(offsetof(SaveTaskData, screenOption) == 0x63c);
inline constexpr std::size_t S2SaveSlotCount = 10;
inline constexpr std::size_t S2SaveFileSize = 0x2c00;
inline constexpr std::size_t S2SavePayloadSize = 0x1d20;
struct S2VmuSaveHeader {
    std::array<char, 16> shortDescription;
    std::array<char, 32> longDescription;
    std::array<char, 16> applicationId;
    std::uint16_t iconCount, iconSpeed, eyecatchType, checksum;
    std::uint32_t dataSize;
    std::array<std::byte, 20> reserved;
    std::array<std::uint16_t, 16> iconPalette;
};
static_assert(sizeof(S2VmuSaveHeader) == 128 && offsetof(S2VmuSaveHeader, iconCount) == 64);
static_assert(offsetof(S2VmuSaveHeader, dataSize) == 72 && offsetof(S2VmuSaveHeader, iconPalette) == 96);
inline constexpr S2VmuSaveHeader S2_VMU_SAVE_HEADER{.shortDescription = {'S', 'H', 'E', 'N', 'M', 'U', 'E', '2', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    .longDescription = {'S', 'H', 'E', 'N', 'M', 'U', 'E', ' ', '2', ' ', 'U', 'S'},
    .applicationId = {},
    .iconCount = 3,
    .iconSpeed = 10,
    .eyecatchType = 0,
    .checksum = 0,
    .dataSize = 0,
    .reserved = {},
    .iconPalette = {}};
struct LcdmTaskData {
    std::array<std::byte, 0x61c> screen{};
    std::uint32_t nativeBufferSize{};
#if SHENMUE_IS_PC
    std::array<std::byte, 0xc0> reserved620{};
#else
    std::array<std::byte, 0xb8> reserved620{};
#endif
    const std::byte *resource{};
#if SHENMUE_IS_PC
    std::array<std::byte, 0x628> resourceState{};
#else
    std::array<std::byte, 0x598> resourceState{};
#endif
};

static_assert(offsetof(LcdmTaskData, nativeBufferSize) == 0x61c);
#if SHENMUE_IS_PC
static_assert(sizeof(LcdmTaskData) == 0xd10);
static_assert(offsetof(LcdmTaskData, resource) == 0x6e0);
static_assert(offsetof(LcdmTaskData, resourceState) == 0x6e8);
#else
static_assert(sizeof(LcdmTaskData) == 0xc74);
static_assert(offsetof(LcdmTaskData, resource) == 0x6d8);
static_assert(offsetof(LcdmTaskData, resourceState) == 0x6dc);
#endif
struct LcdmSectionBinding {
    std::uint32_t name0;
    std::uint32_t name1;
    std::size_t stateOffset;
};

struct WindowTaskData {
    shen_name name;
#if SHENMUE_IS_PC
    std::int32_t reserved004;
    std::array<WindowSlot, 5> slots;
    std::uint32_t customFlag;
    std::uint32_t reserved2b4;
#else
    std::uint32_t activeSlot0;
    std::byte reserved008[0x5c];
    std::uint32_t activeSlot1;
    std::byte reserved068[0x5c];
    std::uint32_t activeSlot2;
    std::byte reserved0c8[0x5c];
    std::uint32_t activeSlot3;
    std::byte reserved128[0x5c];
    std::uint32_t activeSlot4;
    std::byte reserved188[0x5c];
    std::uint32_t customFlag;
#endif
};

#if SHENMUE_IS_PC
static_assert(sizeof(WindowTaskData) == 0x2b8);
static_assert(offsetof(WindowTaskData, slots) == 0x08);
static_assert(offsetof(WindowTaskData, slots) + 4 * sizeof(WindowSlot) == 0x228);
static_assert(offsetof(WindowTaskData, customFlag) == 0x2b0);
#else
static_assert(sizeof(WindowTaskData) == 0x1e8);
static_assert(offsetof(WindowTaskData, activeSlot0) == 0x04);
static_assert(offsetof(WindowTaskData, activeSlot1) == 0x64);
static_assert(offsetof(WindowTaskData, activeSlot4) == 0x184);
static_assert(offsetof(WindowTaskData, customFlag) == 0x1e4);
#endif

#pragma pack(push, 1)
struct S2SaveDataInfo {
    std::uint16_t systemYear;
    std::uint8_t systemMonth;
    std::uint8_t systemDay;
    std::uint8_t systemHour;
    std::uint8_t systemMinute;
    std::uint8_t systemSecond;
    std::uint8_t systemWeek;
    std::int16_t gameYear;
    std::uint8_t gameMonth;
    std::uint8_t gameDay;
    std::uint8_t gameHour;
    std::uint8_t gameMinute;
    std::uint8_t gameSecond;
    std::uint8_t gameWeek;
    std::uint16_t saveKind;
    std::int16_t actNumber;
    std::uint16_t stepHundreds;
    std::int16_t sceneNumber;
    shen_name mapName;
    std::int32_t entryPoint;
    std::uint32_t checksum;
    std::uint8_t clearState;
    std::uint8_t stepRemainder;
    std::array<std::byte, 0x1a> reserved;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SnapshotMetadataRecord {
    std::int32_t id;
    std::int32_t use;
    shen_name area;
    shen_name subArea;
    shen_name shop;
    std::uint32_t reserved14;
    std::uint16_t year;
    std::uint8_t month;
    std::uint8_t day;
    std::uint8_t hour;
    std::uint8_t minute;
    std::uint8_t second;
    std::uint8_t week;
    shen_name lockOnId;
    std::array<std::int32_t, 5> sortKeys;
};

struct SnapshotMetadataOwner {
    std::array<char, 20> identifier;
    std::array<std::uint32_t, 48> omakeFlags;
    std::uint32_t reserve;
    std::array<SnapshotMetadataRecord, 126> records;
};
#pragma pack(pop)

static_assert(sizeof(SnapshotMetadataRecord) == 0x38);
static_assert(offsetof(SnapshotMetadataRecord, area) == 0x08);
static_assert(offsetof(SnapshotMetadataRecord, shop) == 0x10);
static_assert(offsetof(SnapshotMetadataRecord, reserved14) == 0x14);
static_assert(offsetof(SnapshotMetadataRecord, year) == 0x18);
static_assert(offsetof(SnapshotMetadataRecord, lockOnId) == 0x20);
static_assert(offsetof(SnapshotMetadataRecord, sortKeys) == 0x24);
static_assert(offsetof(SnapshotMetadataOwner, records) == 0xd8);
static_assert(sizeof(SnapshotMetadataOwner) == 0x1c68);

static_assert(sizeof(S2SaveDataInfo) == 0x40);
static_assert(offsetof(S2SaveDataInfo, saveKind) == 0x10);
static_assert(offsetof(S2SaveDataInfo, mapName) == 0x18);
static_assert(offsetof(S2SaveDataInfo, checksum) == 0x20);
static_assert(offsetof(S2SaveDataInfo, clearState) == 0x24);

struct S2SaveImage {
    std::array<std::byte, S2SaveFileSize> bytes{};
};

static_assert(sizeof(S2SaveImage) == S2SaveFileSize);

struct S2NativeSaveBacking {
    S2SaveImage image;
    std::int32_t selection;
    std::uint32_t loadedFlags;
    std::uint32_t loadedCompletions;
    std::uint32_t backupFlags;
    std::array<S2SaveDataInfo, 11> primary;
    std::array<S2SaveDataInfo, 10> alternate;
};
static_assert(offsetof(S2NativeSaveBacking, selection) == 0x2c00);
static_assert(offsetof(S2NativeSaveBacking, primary) == 0x2c10);
static_assert(offsetof(S2NativeSaveBacking, alternate) == 0x2ed0);
static_assert(sizeof(S2NativeSaveBacking) == 0x3150);

enum class S2SaveResult : std::int32_t {
    Success = 0,
    Cancelled,
    InvalidArgument,
    OpenFailed,
    ReadFailed,
    SizeMismatch,
    BadSystemHeader,
    BadGameHeader,
    ChecksumMismatch,
    WriteFailed,
    CommitFailed,
};

struct S2SaveSlotInfo {
    S2SaveDataInfo data{};
    S2SaveResult result{S2SaveResult::OpenFailed};
    bool occupied{};
};

enum class S2InventoryBank : std::uint8_t {
    Type0 = 0,
    Type1 = 1,
    Type2 = 2,
    Type3 = 3,
    Type4 = 4,
    Type5 = 5,
};

struct S2MoveSkillBank {
    std::array<std::int16_t, 40> proficiency{};
    std::array<std::int16_t, 40> usage{};
    std::uint8_t count{};
    std::uint8_t selected{};
};

static_assert(sizeof(S2MoveSkillBank) == 0xa2);

struct S2PersistentPlayerStats {
    std::uint32_t savedStat0{};
    std::array<std::byte, 16> statFlags{};
    std::uint32_t runtimeFlags{};
    std::int32_t cash{};
    std::int32_t segaCoins{};
    std::int32_t transferredYen{};
    std::uint16_t alternateHealth{};
    std::uint16_t defaultHealth{};
    std::uint16_t reactionCameraScale{};
    std::uint16_t assistScale{};
    std::uint32_t gameConfigurationFlags{};
    std::uint16_t contextActionMode{};
};

inline constexpr std::size_t S2StreetFightProgressOffset = 0xe44;


struct S2CombatStatistics {
    std::array<std::byte, 0xba4> retainedPrefix{};
    std::array<std::uint16_t, 128> performedMoves{};
    std::array<std::uint16_t, 128> receivedMoves{};
    std::array<std::byte, 0xa0> retainedDa4{};
    std::uint16_t streak{};
    std::array<std::byte, 0x1ba> retainedE46{};
};
static_assert(sizeof(S2CombatStatistics) == 0x1000);
static_assert(offsetof(S2CombatStatistics, performedMoves) == 0xba4);
static_assert(offsetof(S2CombatStatistics, receivedMoves) == 0xca4);
static_assert(offsetof(S2CombatStatistics, streak) == 0xe44);
const S2CombatStatistics &S2Save_GetCombatStatistics() noexcept;
void S2Save_AccumulateCombatStatistics(std::span<const std::byte, 0x200> history, std::uint16_t streakDelta, std::int32_t defaultHealth) noexcept;

enum class PlayerStatsId : std::uint32_t {
    SavedFlags = 0,
    RuntimeFlags = 1,
    Cash = 2,
    AlternateHealth = 3,
    DefaultHealth = 4,
    ReactionCameraScale = 5,
    AssistScale = 6,
    ContextActionMode = 7,
    SegaCoins = 8
};
inline constexpr std::size_t S2CombatSegaCoinHighWaterOffset = 0xc0;
std::uint64_t GetPlayerStat(PlayerStatsId type) noexcept;
void SetPlayerStat(PlayerStatsId type, std::uint32_t value) noexcept;

const S2PersistentPlayerStats &S2Save_GetPersistentPlayerStats() noexcept;
void S2Save_SetPersistentPlayerStats(const S2PersistentPlayerStats &stats) noexcept;
std::int32_t GetCurrentSegaCoins() noexcept;
void SetCurrentSegaCoins(std::int32_t segaCoins) noexcept;
void S2Save_SetSkillFlags(const std::array<std::byte, 16> &flags) noexcept;
bool vfCheckSkillFlag(shen_name flag) noexcept;
std::int32_t GetTransferredYen() noexcept;
std::span<const std::byte> S2Save_GetInventoryBank(S2InventoryBank bank) noexcept;
std::span<const S2MoveSkillBank, 3> S2Save_GetMoveSkillBanks() noexcept;
bool S2Save_SetMoveProficiency(std::uint8_t category, std::size_t index, std::int16_t value) noexcept;
std::uint32_t GetMoveProficiency(std::uint8_t category, std::uint8_t move) noexcept;
std::int64_t SetMoveProficiency(std::int8_t clear, std::uint8_t category, std::int16_t move, std::int16_t value) noexcept;
bool S2Save_SetInventoryItem(S2InventoryBank bank, std::size_t index, std::byte value) noexcept;
std::span<const std::int32_t, 26> S2Save_GetMinigameScores() noexcept;
enum class SavedScoreType : std::uint32_t {
    ExciteQTE_BestScore = 0,
    ExciteQTE_ContinuedScore = 1,
    Darts_BestScore4 = 2,
    Darts_BestScore3 = 3,
    QTETitle_0 = 5,
    QTETitle_1 = 6,
    QTETitle_2 = 7,
    Darts_BestScore1 = 12,
    Darts_BestScore2 = 13
};
std::uint64_t GetSavedMinigameScore(std::uint32_t index) noexcept;
void SetSavedMinigameScore(SavedScoreType type, std::uint32_t score) noexcept;

extern std::array<ArcadeRankingRecord, 20> g_harrierRankings;
extern std::array<ArcadeRankingRecord, 20> g_hangOnRankings;
extern std::array<ArcadeRankingRecord, 20> g_afterBurnerRankings;
extern std::array<ArcadeRankingRecord, 20> g_outRunRankings;
void ResetArcadeRankings() noexcept;

using BackupSaveCompletion = void (*)(std::int32_t result, const void *source);
struct BackupSaveWriteWork {
    std::uint32_t archiveHandle, size;
    std::array<char, 260> path;
    std::uint32_t reserved10c;
    const void *source;
};
static_assert(sizeof(BackupSaveWriteWork) == 0x118);
static_assert(offsetof(BackupSaveWriteWork, path) == 8);
static_assert(offsetof(BackupSaveWriteWork, source) == 0x110);
inline constexpr std::int64_t BACKUP_LOCAL_CAPACITY = 1000000000;
struct BackupSaveReadWork {
    std::uint32_t archiveHandle;
    std::uint32_t reserved04;
    void (*completion)(std::int32_t stage, std::int32_t result, const void *destination);
    std::uint32_t size;
    std::array<char, 260> path;
    void *destination;
};
static_assert(sizeof(BackupSaveReadWork) == 0x120);
static_assert(offsetof(BackupSaveReadWork, completion) == 0x08);
static_assert(offsetof(BackupSaveReadWork, size) == 0x10);
static_assert(offsetof(BackupSaveReadWork, path) == 0x14);
static_assert(offsetof(BackupSaveReadWork, destination) == 0x118);
inline constexpr std::array<const char *, 10> S1_BACKUP_SLOT_NAMES{"SHENMUE1_000", "SHENMUE1_001", "SHENMUE1_002", "SHENMUE1_003", "SHENMUE1_004",
    "SHENMUE1_005", "SHENMUE1_006", "SHENMUE1_007", "SHENMUE1_008", "SHENMUE1_009"};
inline constexpr std::array<std::uint32_t, 10> S2_BACKUP_SLOT_FLAGS{0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 0x40000};
extern std::array<S2SaveDataInfo, 11> &g_backupSaveDataInfos;
S2SaveDataInfo *GetBackupSaveDataInfo(std::int32_t slot) noexcept;
S2SaveDataInfo *GetS1BackupSaveDataInfo(std::uint32_t slot) noexcept;
extern std::array<S2SaveDataInfo, 10> &g_backupS1SaveDataInfos;
std::uint64_t CheckBackupFileExists(const char *name);
std::uint64_t QueueBackupFileRead(const char *name, void *destination, std::uint32_t offset, std::uint32_t size, BackupSaveCompletion completion = nullptr);
std::uint64_t ReadBackupFile(const char *name, void *destination, std::uint32_t offset, std::uint32_t size, BackupSaveCompletion completion = nullptr);
std::uint32_t ScanS1BackupSlots();
std::uint32_t ScanS2BackupSlots();
void RenewLocalBackupInfo();
std::uint64_t GetLocalBackupInfo() noexcept;
std::uint64_t BupReady(std::int32_t reserve);
std::uint64_t QueueSaveFileWrite(const char *name, const void *source, std::uint32_t size, BackupSaveCompletion completion = nullptr);
std::uint32_t QueueSnapshotMetadataWrite(SnapshotMetadataOwner *metadata);
SnapshotMetadataOwner *GetSnapshotMetadataOwner() noexcept;
std::uint32_t GetSnapshotMetadataCount() noexcept;
void InitializeSnapshotMetadata() noexcept;
bool SaveSnapshotMetadata() noexcept;
std::int32_t GetSnapshotDataOption() noexcept;
void SetSnapshotDataOption(std::int32_t option) noexcept;
std::uint64_t GetCurrentGameFlags() noexcept;
std::uint64_t SetCurrentGameFlag(std::uint8_t bit, std::int32_t enabled) noexcept;
std::uint64_t GetCompletedGameCount() noexcept;
std::uint64_t SetCompletedGameCount(std::uint32_t count) noexcept;

std::uint32_t QueueSnapshotSave(
    std::uint32_t slot, std::uint32_t rowPitch, std::uint32_t width, std::uint32_t height, std::uint32_t format, std::vector<std::byte> &&pixels) noexcept;
std::uint32_t IsSnapshotSaveInProgress() noexcept;

#pragma pack(push, 1)
struct LocalTimeSnapshot {
    std::uint16_t year;
    std::uint8_t month;
    std::uint8_t day;
    std::uint8_t hour;
    std::uint8_t minute;
    std::uint8_t second;
    std::uint8_t dayOfWeek;
    std::uint8_t reserved08;
};
#pragma pack(pop)
static_assert(sizeof(LocalTimeSnapshot) == 9);
std::int64_t shenGetLocalTime(LocalTimeSnapshot *snapshot) noexcept;
std::int64_t shenGetLocalTimeSnapshot(LocalTimeSnapshot *snapshot) noexcept;
std::filesystem::path S2Save_SnapshotPath(std::uint32_t slot);
std::uint32_t S2Save_Checksum(std::span<const std::byte> payload) noexcept;
S2SaveResult S2Save_Validate(const S2SaveImage &image) noexcept;
S2SaveResult S2Save_Capture(S2SaveImage &image, std::uint8_t clearState = 1);
std::uint64_t S2Save_CheckData();
S2SaveResult S2Save_Apply(const S2SaveImage &image);
std::uint64_t RestoreCurrentSceneInfoFromSave() noexcept;
S2SaveResult S2Save_Read(const std::filesystem::path &path, S2SaveImage &image);
S2SaveResult S2Save_Write(const std::filesystem::path &path, const S2SaveImage &image);
S2SaveResult S2Save_ReadSlot(std::size_t slot, S2SaveImage &image);
S2SaveResult S2Save_WriteSlot(std::size_t slot, std::uint8_t clearState = 1);
std::uint32_t SaveGame(std::uint32_t slot);
std::int32_t GetGameSaveStatus();
std::uint32_t GetSaveMenuSource() noexcept;
std::uint32_t SetSaveMenuSource(std::uint32_t source) noexcept;
std::filesystem::path S2Save_SlotPath(std::size_t slot);
void S2Save_SetDirectory(std::filesystem::path directory);
const std::filesystem::path &S2Save_GetDirectory() noexcept;
void S2Save_ResetNewGameState();

const std::array<S2SaveSlotInfo, S2SaveSlotCount> &S2Save_RefreshSlotCatalog();
const std::array<S2SaveSlotInfo, S2SaveSlotCount> &S2Save_GetSlotCatalog() noexcept;
bool S2Save_HasContinueSlot();

using S2SaveTaskCompletion = void (*)(S2SaveResult result, std::size_t slot);
HLib::Task *SM_Init_Enqueue(std::int32_t initializationMode);
HLib::Task *QueueWindowTask(std::uint8_t nextFunctionIndex = 2);

extern std::int32_t g_wndMessageDrawDisabled;
void wndEnableMessageDraw();
void wndDisableMessageDraw();
extern std::array<std::uint16_t, 4> g_windowGlyphPalette;
void RasterizeWindowGlyph(std::uint16_t glyph, void *destination, std::int32_t bytePitch, std::int32_t byteOffset);
std::uint64_t RenderWindowTextSurface(
    HLib::Task *task, const char *text, std::uint32_t width, std::int16_t colour, std::int16_t shadow, std::int32_t conversionMode, std::int32_t centered);
std::uint64_t SetupUIElement(HLib::Task *task, const char *text, std::uint32_t width, std::int16_t colour, std::int16_t shadow, std::int32_t conversionMode);
std::uint64_t CreateLeftAlignedWindowTextSurface(
    HLib::Task *task, const char *text, std::uint32_t width, std::int16_t colour, std::int16_t shadow, std::int32_t conversionMode);
void QueueLcdmTask();
void LcdmInit(std::uintptr_t image) noexcept;
void LcdmSelectMessage(std::uintptr_t message) noexcept;
SaveTaskData *SetSaveScreenWarning(std::int32_t warning, std::int32_t value);
SaveTaskData *SelectSaveScreenWarning(SaveTaskData *save, std::int32_t screen);
std::int64_t StartShowScreenOfId(std::uint32_t screenId) noexcept;
std::uint32_t IsSaveAllowed() noexcept;
void wndSetFlag(HLib::Task *task, std::int32_t flag);
void LoadAreaSpriteImages();
void SetTitleWarningLogo(std::int32_t logoIndex, const void *parameters = nullptr) noexcept;
void SetSceneInfos(std::int32_t actNumber, shen_name mapName, std::int32_t entryPoint) noexcept;
void CloseSaveScreen() noexcept;
void CloseSaveScreenFromLoadSave() noexcept;
// LoadSave admission word: the action selector's Save entry raises it; the LoadSave element reads it at setup and
// its close restores the saved player flags when it is set, then clears it.
extern std::int32_t g_saveScreenAdmission;
void EnableSaveScreenAdmission() noexcept;
// LoadSave save/load mode word (1 for the in-game save screen), tested by the LoadSave element and LoadOrSaveGame.
extern std::int32_t g_saveFrontendMode;
void SetSaveFrontendMode(std::int32_t mode) noexcept;
void S2SaveScreen_SelectSlot(std::size_t slot) noexcept;
bool S2SaveScreen_ConfirmSlot(std::size_t slot) noexcept;
bool S2SaveScreen_CancelPrompt() noexcept;
bool S2SaveScreen_HasPrompt() noexcept;
bool S2SaveTask_Begin(std::size_t slot, std::uint8_t clearState, S2SaveTaskCompletion completion = nullptr);

void S2Save_NewGameTitleTask(void *taskData);
void S2Save_ContinueTitleTask(void *taskData);
void S2Save_TitleLoadTask(void *taskData);

bool S2Save_ConsumePendingPlayerPersistentState(PlayerPersistentState &state) noexcept;
bool S2Save_RestorePlayerAtStartup() noexcept;
std::int32_t S2Save_GetPendingStartParameters() noexcept;
bool S2Save_QueryPendingPlayerTransform(Vector3 &position, std::int32_t &yaw) noexcept;

std::uint8_t ResetNativePlayerStats() noexcept;
std::int64_t InitializeMoveSkillStats() noexcept;
void ResetNewGameConfiguration();
std::uint16_t GetImportedPlayerOption() noexcept;
void SetImportedPlayerOption(std::uint16_t value) noexcept;
void SetRetainedSaveSelection(std::int32_t selection) noexcept;
void ResetPendingPlayerPersistentState() noexcept;
void ReinitScenarioAndFlags();
std::uint32_t InitializeDefaultSaveData() noexcept;
std::uint64_t ReadSaveDataSection(std::int32_t gameSection, std::int32_t slot);
std::byte *RestoreSystemSaveProgress() noexcept;
std::int64_t InitializeNewGameFromSaveSlot(std::int32_t slot);

inline constexpr std::array<std::int16_t, 13> InitialHandMoveProficiency{96, 64, 82, 64, 62, 86, 58, 84, 78, 66, 52, 58, 46};
inline constexpr std::array<std::int16_t, 12> InitialLegMoveProficiency{92, 86, 78, 72, 0, 46, 58, 48, 54, 34, 52, 64};
inline constexpr std::array<std::int16_t, 9> InitialThrowMoveProficiency{84, 64, 60, 64, 38, 60, 38, 54, 44};
inline constexpr std::array<std::byte, 12> InitialPlayerSkillFlags{std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0x40}, std::byte{0xff},
    std::byte{0x9d}, std::byte{0x36}, std::byte{0xff}, std::byte{7}, std::byte{0}, std::byte{0}};

} // namespace shenmue::s2
