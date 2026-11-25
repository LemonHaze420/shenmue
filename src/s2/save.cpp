#include "shenmue.h"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/save.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/warning.hpp"
#include "common/math.hpp"
#include "s2/save_load_dc.hpp"
#include "s2/backup_dc.hpp"
#include "s2/backup_metadata_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/save_screen_data_dc.hpp"
#include "s2/save_backup_data_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/pause.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/lcdm_dc.hpp"
#include "s2/window.hpp"
#include "s2/title_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/title_warning.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/system_modules.hpp"
#include "s2/startup_assets_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cstdio>
#include <cstring>

extern "C" {
std::int32_t buGetFileSize(std::int32_t port, const char *filename);
std::int32_t buLoadFile(std::int32_t port, const char *filename, void *data, std::int32_t blocks);
std::int32_t buAnalyzeBackupFileImage(void *information, void *data);
std::uint32_t syRtcGetDate(void *date);
std::int32_t buMakeBackupFileImage(void *output, const shenmue::s2::SaveBackupImageDescriptor *descriptor);
std::int32_t buSaveFile(std::int32_t port, const char *filename, void *data, std::int32_t blocks, const void *date, std::uint32_t flags);
}
#include "s2/save_screen_state_dc.hpp"
#include "s2/backup_hotplug_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/arcade_rankings_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/navi.hpp"
extern "C" std::uint32_t syRtcGetDate(void *date);
#else
#if defined(SHENMUE_STEAM)
#include "d3t/steam.hpp"
#endif
#include "s2/save.hpp"
#include "s2/area_jump.hpp"
#include "s2/archive_data.hpp"
#include "s2/cold_archive.hpp"
#include "s2/author.hpp"
#include "s2/arcade_ranking_defaults.hpp"

#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/elevator.hpp"
#include "s2/fight.hpp"
#include "s2/game_runtime.hpp"
#include "s2/battle_qte.hpp"
#include "s2/fight_stage.hpp"
#include "s2/font.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/load_title.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/initialization.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/magic_room.hpp"
#include "s2/model_component.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/story_state.hpp"
#include "s2/scenario_flags.hpp"
#include "s2/search.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/snapshot.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#include "s2/title_area_data.hpp"
#include "s2/item_selector.hpp"
#include "s2/ui_loadsave.hpp"
#include "s2/map_info.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/sound_program.hpp"
#include "s2/flag.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/load_title.hpp"
#include "s2/navi.hpp"
#include "s2/npc.hpp"
#include "s2/title_warning.hpp"
#include "s2/warning.hpp"
#include "d3t/texture.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/threads.hpp"
#include "d3t/archive_handles.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/ui.hpp"
#include <json/json.h>

#include <algorithm>
#include <cstdlib>
#include <atomic>
#include <bit>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <new>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

extern "C" int stbi_write_jpg_to_func(void (*func)(void *context, void *data, int size), void *context, int x, int y, int comp, const void *data, int quality);
#endif

namespace shenmue {
namespace s2 {

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::array<ArcadeRankingRecord, 20> g_harrierRankings{};

std::array<ArcadeRankingRecord, 20> g_hangOnRankings{};

std::array<ArcadeRankingRecord, 20> g_afterBurnerRankings{};

std::array<ArcadeRankingRecord, 20> g_outRunRankings{};

static S2NativeSaveBacking nativeSaveBacking{};

std::array<S2SaveDataInfo, 11> &g_backupSaveDataInfos = nativeSaveBacking.primary;

std::array<S2SaveDataInfo, 10> &g_backupS1SaveDataInfos = nativeSaveBacking.alternate;

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace {

std::span<const std::byte> Payload(const S2SaveImage &image) noexcept;
std::span<std::byte> Payload(S2SaveImage &image) noexcept;
void SaveTaskIdle(void *);
void SaveTaskDispatch(void *taskData);
void SaveTaskInitCallback(void *);
void SaveScreenDiskFade(void *taskData);
void SaveScreenComplete(void *taskData);
void SaveScreenFadeOut(void *taskData);
void SaveScreenRun(void *taskData);
void SaveScreenInitialize(void *taskData);
void SaveScreenUpdate(void *taskData);
void SaveTaskCallback(void *taskData);
void RunTitleLoadFlow(bool completedOnly);
std::filesystem::path MakeTemporaryPath(const std::filesystem::path &destination);
bool FlushFileToDisk(const std::filesystem::path &path) noexcept;
void RemoveTemporaryFile(const std::filesystem::path &path) noexcept;
S2SaveResult CommitTemporaryFile(const std::filesystem::path &temporary, const std::filesystem::path &destination);
void QueueBackupArchiveRead(const char *path, void *destination, std::uint32_t offset, std::uint32_t size);
void DrawLegacySaveSlotList(SaveTaskData &save);

} // namespace

std::uint64_t BupReady(std::int32_t reserve);
std::uint64_t QueueSaveFileWrite(const char *name, const void *source, std::uint32_t size, BackupSaveCompletion completion);
std::uint64_t CheckBackupFileExists(const char *name);
S2SaveDataInfo *GetBackupSaveDataInfo(std::int32_t slot) noexcept;
void ValidateSnapshotMetadataFiles() noexcept;
bool SaveSnapshotMetadata() noexcept;
std::uint32_t S2Save_Checksum(std::span<const std::byte> payload) noexcept;
S2SaveResult S2Save_Capture(S2SaveImage &image, std::uint8_t clearState);
std::filesystem::path S2Save_SlotPath(std::size_t slot);
void SetRetainedSaveSelection(std::int32_t selection) noexcept;
void ReinitScenarioAndFlags();
std::uint32_t InitializeDefaultSaveData() noexcept;
std::uint64_t ReadSaveDataSection(std::int32_t gameSection, std::int32_t slot);
void RestoreGameSaveSlot(std::uint32_t slot);
const std::array<S2SaveSlotInfo, S2SaveSlotCount> &S2Save_RefreshSlotCatalog();
bool S2Save_HasContinueSlot();
void LcdmInit(std::uintptr_t image) noexcept;
HLib::Task *QueueWindowTask(std::uint8_t nextFunctionIndex);
void ClearWindowSlot(WindowSlot *window);
void QueueTitleWarningTask();
void QueueTitleSaveDependencies();
HLib::Task *SM_Init_Enqueue(std::int32_t initializationMode);
SaveTaskData *SetSaveScreenWarning(std::int32_t warning, std::int32_t value);
SaveTaskData *SelectSaveScreenWarning(SaveTaskData *save, std::int32_t screen);
std::int64_t StartShowScreenOfId(std::uint32_t screenId) noexcept;
std::uint32_t IsSaveAllowed() noexcept;
void SetSceneInfos(std::int32_t actNumber, shen_name mapName, std::int32_t entryPoint) noexcept;
void GetSceneInfos(std::int32_t *actNumber, shen_name *mapName, std::int32_t *entryPoint) noexcept;
void SetSaveFrontendMode(std::int32_t mode) noexcept;
void TFSEL_ModChange_Callback(void *);
#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

constexpr std::uint32_t SnapshotName(const char *name) noexcept {
    return make_shen_name(name[0], name[1], name[2], name[3]);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

constexpr std::uint32_t kSystemMagic = 0x59533255U;

constexpr std::uint32_t kGameMagic = 0x41473255U;

constexpr std::uint32_t kSaveVersion = 110U;

constexpr std::size_t kSystemHeaderOffset = 0x680;

constexpr std::size_t kSystemSaveCountOffset = 0x688;

constexpr std::size_t kSystemConfigurationOffset = 0x6c0;

constexpr std::size_t kCompletedGameCountOffset = 0x6c4;

constexpr std::size_t kCurrentGameFlagsOffset = 0x6c8;

constexpr std::size_t kGameHeaderOffset = 0x800 + 0x680;

constexpr std::size_t kSaveDataInfoOffset = 0x800 + 0x6a0;

constexpr std::size_t kPayloadOffset = 0x800 + 0x6e0;

constexpr std::size_t kPlayerSavedStatOffset = 0x000;

constexpr std::size_t kPlayerStatFlagsOffset = 0x004;

constexpr std::size_t kPlayerRuntimeFlagsOffset = 0x014;

constexpr std::size_t kCashOffset = 0x018;

constexpr std::size_t kSegaCoinsOffset = 0x01c;

constexpr std::size_t kHealthOffset = 0x020;

constexpr std::size_t kGameConfigurationOffset = 0x028;

constexpr std::size_t kContextActionOffset = 0x02c;

constexpr std::size_t kMoveSkillBankOffset = 0x02e;

constexpr std::size_t kMoveSkillBankStride = 0x0a2;

constexpr std::size_t kStoryFlagsOffset = 0x214;

constexpr std::size_t kInternalFlagsOffset = 0x29c;

constexpr std::size_t kInventoryType0Offset = 0x4fc;

constexpr std::size_t kStatsOffset = 0x4bc;

static_assert(kMoveSkillBankOffset + 3 * kMoveSkillBankStride == kStoryFlagsOffset);

constexpr std::size_t kInventoryType3Offset = 0x67c;

constexpr std::size_t kInventoryType1Offset = 0x6fc;

constexpr std::size_t kInventoryType5Offset = 0x8fc;

constexpr std::size_t kInventoryType4Offset = 0x93c;

constexpr std::size_t kMinigameScoresOffset = 0x97c;

constexpr std::size_t kHarrierRankingsOffset = 0x9fc;

constexpr std::size_t kHangOnRankingsOffset = 0xaec;

constexpr std::size_t kAfterBurnerRankingsOffset = 0xbdc;

constexpr std::size_t kOutRunRankingsOffset = 0xccc;

constexpr std::size_t kActivityCountersOffset = 0xdec;

constexpr std::size_t kFightMotionNibblesOffset = 0xe90;

constexpr std::size_t kNaviMarkersOffset = 0xff8;

constexpr std::size_t kNaviTrailOffset = 0x11f8;

constexpr std::size_t kMagicRoomActiveOffset = 0x1c64;

constexpr std::size_t kMemoPageOffset = 0x0e00;

constexpr std::size_t kMemoFlagOffset = 0x0e02;

constexpr std::size_t kPlayerAuxiliaryOffset = 0x49c;

constexpr std::size_t kAreaJumpStateOffset = 0x1bf8;

constexpr std::size_t kPlayerTransformOffset = 0xdbc;

constexpr std::size_t kShenmueFlagBlockOffset = 0xe04;

static_assert(kAreaJumpStateOffset + sizeof(AreaJumpPersistentState) == 0x1c64);

static_assert(kMemoFlagOffset + sizeof(std::uint16_t) == kShenmueFlagBlockOffset);

constexpr std::size_t kTransferredYenOffset = 0xe48;

constexpr std::size_t kQueuedDoorAuthorOffset = 0xe4c;

constexpr std::size_t kInventoryType2Offset = 0xe50;

static_assert(kTransferredYenOffset + sizeof(std::int32_t) <= kInventoryType2Offset);

constexpr std::size_t kNpcScheduleSeedOffset = 0xea0;

constexpr std::size_t kPlayerPersistentStateOffset = 0xf70;

constexpr std::size_t kCompanionStateOffset = 0xfb0;

static_assert(kNpcScheduleSeedOffset + NpcScheduleSeedBufferSize == kPlayerPersistentStateOffset);

constexpr std::size_t kQronFloorOffset = 0xfe0;

constexpr std::size_t kElevatorStateOffset = 0xfe4;

static_assert(kElevatorStateOffset + ElevatorPersistentStateSize <= S2SavePayloadSize);

static_assert(kSaveDataInfoOffset + sizeof(S2SaveDataInfo) == kPayloadOffset);

static_assert(kPayloadOffset + S2SavePayloadSize == S2SaveFileSize);

constexpr std::array<const char *, S2SaveSlotCount> kSaveSlotNames{
    "Game Data 1", "Game Data 2", "Game Data 3", "Game Data 4", "Game Data 5", "Game Data 6", "Game Data 7", "Game Data 8", "Game Data 9", "Game Data 10"};

#pragma pack(push, 1)

struct SavedPlayerTransform {
    Vector3 position;
    std::int32_t yaw;
    std::uint8_t characterState;
    std::array<std::byte, 0x1f> reserved;
};

#pragma pack(pop)

static_assert(sizeof(SavedPlayerTransform) == 0x30);

static_assert(offsetof(SavedPlayerTransform, yaw) == 0x0c);

static_assert(offsetof(SavedPlayerTransform, characterState) == 0x10);

struct SaveTransitionDescriptor {
    std::uint8_t inputKind;
    std::uint8_t resultKind;
    std::uint8_t blockSoftReset;
};

struct SaveTransitionTable {
    std::string_view rows;
};

constexpr std::size_t kSaveTransitionRowSize = 11;

constexpr std::array<SaveTransitionDescriptor, 16> kSaveTransitionDescriptors{{
    {0, 0, 0},
    {5, 0, 0},
    {7, 0, 1},
    {3, 0, 0},
    {1, 0, 0},
    {6, 0, 0},
    {7, 3, 1},
    {7, 4, 1},
    {7, 7, 1},
    {0, 9, 1},
    {0, 8, 1},
    {0, 10, 1},
    {0, 1, 1},
    {0, 6, 1},
    {0, 5, 1},
    {0, 0, 0},
}};

constexpr std::array<std::uint32_t, 8> kSaveHelpFlags{
    4,
    38,
    2,
    4,
    0,
    0,
    0,
    241,
};

// PC v1.07 save-screen transition rows. Names identify each mode-specific or
// shared table slot; the native signed 11-byte records are kept intact to
// preserve negative table/substate redirects.
constexpr std::string_view kDefaultSaveScreen0Rows{"\x00\x00\xfd\xfd\xfd\xfd\xfd\xfd\xfd\xfd\xfd", 11};

constexpr std::string_view kDefaultSaveScreen1Rows{"\x00\x0c\xff\xff\xd8\xdd\xff\xff\xff\xfb\xfc\x0b\x01\xfa\xff\xe1\xff\xff\xff\xff\xff\xff", 22};

constexpr std::string_view kSharedSaveScreen2Rows{
    "\x00\x09\xff\xff\xe0\xff\xde\xff\xff\xfb\xff\x01\x07\xff\xff\xdf\xff\xff\xff\x03\x02\xff\x01\x02\xff\xff\xdf\xff\xff\xff\x03\xf7\xff\x0a\x01\x04\xff\xe1"
    "\xff\xff\xff\xff\xff\xff\x14\x03\x05\x08\xe1\xff\xff\xff\xff\xff\xff\x00\x09\xff\xff\x06\x07\x07\xff\xff\x07\xff\x05\x01\x08\xff\xe1\xff\xff\xff\xff\xff"
    "\xff\x09\x01\x08\xff\xe1\xff\xff\xff\xff\xff\xff\x11\x03\xe2\x04\xe1\xff\xff\xff\xff\xff\xff",
    99};

constexpr std::string_view kSharedSaveScreen3Rows{
    "\x00\x0d\xff\xff\xe1\xdd\xff\xff\xff\x01\x01\x17\x04\x03\xdb\xe1\xff\xff\xff\xff\xff\xff\x0d\x01\x04\xff\xe1\xff\xff\xff\xff\xff\xff\x02\x03\x05\x01\xe1"
    "\xff\xff\xff\xff\xff\xff\x0e\x03\xf9\xdb\xe1\xff\xff\xff\xff\xff\xff\x01\x08\xff\xff\xdf\xdd\x0d\xff\x09\x06\x07\x01\x02\xff\xff\xdf\xdd\x0d\xff\x09\x07"
    "\xff\x01\x07\xff\xff\xdf\xdd\x0d\xff\x09\x08\xff\x01\x02\xff\xff\xdf\xdd\x0d\xff\x09\xf7\xff\x0a\x01\x0a\xff\xe1\xff\xff\xff\xff\xff\xff\x14\x03\x0b\x0e"
    "\x0c\xff\xff\xff\xff\xff\xff\x00\x0b\xff\xff\x0c\xff\x0d\xff\xff\x05\xff\x05\x01\x0e\xff\xe1\xff\xff\xff\xff\xff\xff\x09\x01\xfd\xff\xe1\xff\xff\xff\xff"
    "\xff\xff\x11\x03\xe2\x0a\xe1\xff\xff\xff\xff\xff\xff",
    165};

constexpr std::string_view kSharedSaveScreen4Rows{
    "\x0c\x03\x01\xfb\xe1\xff\xff\xff\xff\xff\xff\x00\x0a\xff\xff\xe1\xff\xda\xff\xff\x02\xff\x01\x06\xff\xff\xdf\xff\xff\xff\x05\x03\xff\x01\x02\xff\xff\xdf"
    "\xff\xff\xff\x05\x04\xff\x04\x01\xfb\xff\xe1\xff\xff\xff\xff\xff\xff\x0a\x01\x06\xff\xe1\xff\xff\xff\xff\xff\xff\x14\x03\x07\x0a\xe1\xff\xff\xff\xff\xff"
    "\xff\x00\x0e\xff\xff\xe1\xff\xff\xff\xff\x02\x09\x05\x01\x0a\xff\xe1\xff\xff\xff\xff\xff\xff\x05\x01\x0a\xff\xe1\xff\xff\xff\xff\xff\xff\x11\x03\xe2\x06"
    "\xe1\xff\xff\xff\xff\xff\xff",
    121};

constexpr std::string_view kSharedSaveScreen5Rows{
    "\x01\x07\xff\xff\xdf\xdd\x08\xff\x04\x01\xff\x01\x02\xff\xff\xdf\xff\xff\xff\x04\x02\xff\x01\x08\xff\xff\xdf\xff\xff\xff\x04\x03\xff\x01\x02\xff\xff\xdf"
    "\xff\xff\xff\x04\xf7\xff\x0a\x01\x05\xff\xe1\xff\xff\xff\xff\xff\xff\x14\x03\x06\x09\xe1\xff\xff\xff\xff\xff\xff\x00\x0b\xff\xff\x07\xff\x08\xff\xff\x00"
    "\xff\x05\x01\x09\xff\xe1\xff\xff\xff\xff\xff\xff\x09\x01\xfd\xff\xe1\xff\xff\xff\xff\xff\xff\x11\x03\xe2\x05\xe1\xff\xff\xff\xff\xff\xff",
    110};

constexpr std::string_view kDefaultSaveScreen6Rows{
    "\x13\x01\xdb\xff\xff\xff\xff\xff\xff\xff\xff\x12\x01\xdb\xff\xff\xff\xff\xff\xff\xff\xff\x05\x01\xdb\xff\xff\xff\xff\xff\xff\xff\xff\x0a\x01\xe1\xff\xff"
    "\xff\xff\xff\xff\xe1\xff\x07\x01\xdb\xff\xe1\xff\xff\xff\xff\xff\xff\x06\x01\xdb\xff\xe1\xff\xff\xff\xff\xe1\xff\x00\x00\x63\x63\x63\x63\x63\x63\x63\x63"
    "\x63\x00\x00\x63\x63\x63\x63\x63\x63\x63\x63\x63\x08\x01\xdb\xff\xe1\xff\xff\xff\xff\xff\xff\x09\x01\xdb\xff\xe1\xff\xff\xff\xff\xff\xff\x16\x01\xdb\xff"
    "\xff\xff\xff\xff\xff\xff\xff",
    121};

constexpr std::string_view kSharedSaveScreen7Rows{"\x04\x05\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc", 11};

constexpr std::string_view kLoadSaveScreen0Rows{"\x15\x01\xfd\xff\xff\xff\xff\xff\xff\xff\xff", 11};

constexpr std::string_view kLoadSaveScreen1Rows{
    "\x10\x03\x01\xdc\xff\xff\xff\xff\xff\xff\xff\x00\x0c\xff\xff\xd8\xdd\xff\xff\xff\xfb\xfc\x0b\x01\xfa\xff\xe1\xff\xff\xff\xff\xff\xff", 33};

constexpr std::string_view kLoadSaveScreen6Rows{
    "\x13\x01\xdb\xff\xff\xff\xff\xff\xff\xff\xff\x12\x01\xdb\xff\xff\xff\xff\xff\xff\xff\xff\x05\x01\xdb\xff\xff\xff\xff\xff\xff\xff\xff\x0a\x01\xe1\xff\xff"
    "\xff\xff\xff\xff\xff\xff\x07\x01\xdb\xff\xe1\xff\xff\xff\xff\xff\xff\x06\x01\xdb\xff\xe1\xff\xff\xff\xff\xe1\xff\x00\x00\x63\x63\x63\x63\x63\x63\x63\x63"
    "\x63\x00\x00\xfd\xfd\xfd\xfd\xfd\xfd\xfd\xfd\xfd\x08\x01\xdb\xff\xe1\xff\xff\xff\xff\xff\xff\x09\x01\xdb\xff\xe1\xff\xff\xff\xff\xff\xff\x16\x01\xdb\xff"
    "\xff\xff\xff\xff\xff\xff\xff",
    121};

constexpr std::array<SaveTransitionTable, 8> kDefaultSaveTables{{
    {kDefaultSaveScreen0Rows},
    {kDefaultSaveScreen1Rows},
    {kSharedSaveScreen2Rows},
    {kSharedSaveScreen3Rows},
    {kSharedSaveScreen4Rows},
    {kSharedSaveScreen5Rows},
    {kDefaultSaveScreen6Rows},
    {kSharedSaveScreen7Rows},
}};

constexpr std::array<SaveTransitionTable, 8> kLoadSaveTables{{
    {kLoadSaveScreen0Rows},
    {kLoadSaveScreen1Rows},
    {kSharedSaveScreen2Rows},
    {kSharedSaveScreen3Rows},
    {kSharedSaveScreen4Rows},
    {kSharedSaveScreen5Rows},
    {kLoadSaveScreen6Rows},
    {kSharedSaveScreen7Rows},
}};

constexpr std::array<SaveTransitionTable, 8> kStaffSaveTables{{
    {kDefaultSaveScreen0Rows},
    {kLoadSaveScreen1Rows},
    {kSharedSaveScreen2Rows},
    {kSharedSaveScreen3Rows},
    {kSharedSaveScreen4Rows},
    {kSharedSaveScreen5Rows},
    {kLoadSaveScreen6Rows},
    {kSharedSaveScreen7Rows},
}};

enum class SaveTaskPhase : std::uint32_t {
    Idle = 0,
    Confirm = 1,
    Result = 2,
};

struct SaveOperation {
    SaveTaskPhase phase{SaveTaskPhase::Idle};

    S2SaveResult result{S2SaveResult::Success};
    std::size_t slot{};
    std::uint8_t clearState{};
    S2SaveTaskCompletion completion{};
};

enum class TitleLoadPhase : std::uint32_t {
    Idle = 0,
    Selecting = 1,
    Confirm = 2,
    Failure = 3,
};

struct InventoryState {
    std::array<std::byte, 0x180> type0{};
    std::array<std::byte, 0x200> type1{};
    std::array<std::byte, 0x040> type2{};
    std::array<std::byte, 0x080> type3{};
    std::array<std::byte, 0x01c> type4{};
    std::array<std::byte, 0x010> type5{};
};

struct SavedSceneInfo {
    std::int32_t actNumber{};
    shen_name mapName{};
    std::int32_t entryPoint{};
};

InventoryState inventoryState{};

std::array<std::uint16_t, 64> inventoryResetEntries{};

S2PersistentPlayerStats playerStats{};

std::uint16_t retainedImportedPlayerOption{};

std::int32_t &retainedSaveSelection = nativeSaveBacking.selection;

std::array<std::byte, 0x400> playerRetainedResetStorage{};

std::array<std::uint64_t, 2> playerRetainedResetWords{};

std::uint16_t playerRetainedResetWord{};

std::array<std::int16_t, 3> playerRetainedSelections{};

std::uint32_t playerRetainedResetMask{};

S2CombatStatistics combatStatistics{};

std::uint32_t currentGameFlags{};

std::uint32_t completedGameCount{};

std::uint32_t &loadedGameFlags = nativeSaveBacking.loadedFlags;

std::uint32_t &loadedCompletedGameCount = nativeSaveBacking.loadedCompletions;

std::array<S2MoveSkillBank, 3> moveSkillBanks{};

std::array<std::int32_t, 26> minigameScores{};

std::array<std::byte, 32> playerAuxiliaryState{};

S2SaveImage &activeSaveImage = nativeSaveBacking.image;

bool activeSaveImageInitialized = false;

S2SaveImage gameSaveWriteBuffer{};

std::atomic<std::int32_t> gameSaveStatus{};

std::uint32_t gameSaveSlot{}, saveMenuSource{};

SavedSceneInfo savedSceneInfo;

SnapshotMetadataOwner snapshotMetadata{};

bool snapshotMetadataInitialized = false;

std::int32_t snapshotDataOption = 0;

std::filesystem::path saveDirectory{"saves"};

std::atomic<std::uint32_t> snapshotSaveInProgress{0};

d3t::D3TByteBuffer snapshotJpegBuffer{};

std::atomic<std::int32_t> activeBackupState{0};

std::mutex backupAdmissionMutex;

const void *backupCompletionSource{};

std::int32_t backupCompletionStage{}, backupCompletionResult{};

BackupSaveCompletion backupSaveCompletion{};

std::uint32_t &localBackupInfo = nativeSaveBacking.backupFlags;

std::int32_t allowIncompleteS1Import{1};

std::array<std::byte, 0x200> backupS1Prefix{};

std::array<std::byte, 0x200> backupS2Prefix{};

// Native snapshotAberdeenAreas. Entries are grouped by their sort-key high word; the
// lookup is otherwise the byte-wise wildcard match used by FindSnapshotArea.
constexpr std::array<std::uint32_t, 31> snapshotAberdeenAreas{
    SnapshotName("AR01"),
    SnapshotName("AR02"),
    SnapshotName("AR30"),
    SnapshotName("CWON"),
    SnapshotName("ARA0"),
    SnapshotName("ARC0"),
    SnapshotName("ARM0"),
    SnapshotName("ARSF"),
    SnapshotName("AR03"),
    SnapshotName("AR09"),
    SnapshotName("ARZ0"),
    SnapshotName("ART0"),
    SnapshotName("ARP0"),
    SnapshotName("ARCB"),
    SnapshotName("CB00"),
    SnapshotName("AK00"),
    SnapshotName("AK09"),
    SnapshotName("ARBT"),
    SnapshotName("AKB0"),
    SnapshotName("AKS0"),
    SnapshotName("AKS1"),
    SnapshotName("AKT0"),
    SnapshotName("AKA3"),
    SnapshotName("AKA4"),
    SnapshotName("AKT3"),
    SnapshotName("AKT1"),
    SnapshotName("AKT2"),
    SnapshotName("AKY0"),
    SnapshotName("AB00"),
    SnapshotName("ABLB"),
    SnapshotName("ABR0"),
};

constexpr std::array<std::uint32_t, 87> snapshotWanChaiAreas{
    SnapshotName("WS00"),
    SnapshotName("WS09"),
    SnapshotName("BKW0"),
    SnapshotName("WSY0"),
    SnapshotName("WSP0"),
    SnapshotName("WSS0"),
    SnapshotName("WSS1"),
    SnapshotName("WSS2"),
    SnapshotName("WSS5"),
    SnapshotName("WSS3"),
    SnapshotName("WSS4"),
    SnapshotName("WSUR"),
    SnapshotName("WSG1"),
    SnapshotName("WK00"),
    SnapshotName("WK09"),
    SnapshotName("WKA0"),
    SnapshotName("WKP0"),
    SnapshotName("WKP1"),
    SnapshotName("WE00"),
    SnapshotName("WESM"),
    SnapshotName("WET0"),
    SnapshotName("WEG0"),
    SnapshotName("WES1"),
    SnapshotName("WEM1"),
    SnapshotName("WEA0"),
    SnapshotName("WER0"),
    SnapshotName("WEA1"),
    SnapshotName("WECF"),
    SnapshotName("WEIC"),
    SnapshotName("WEH0"),
    SnapshotName("WN00"),
    SnapshotName("WN10"),
    SnapshotName("WNS4"),
    SnapshotName("TOFB"),
    SnapshotName("0048"),
    SnapshotName("WNS5"),
    SnapshotName("WNC0"),
    SnapshotName("WNDA"),
    SnapshotName("WNN0"),
    SnapshotName("WNN1"),
    SnapshotName("WNX7"),
    SnapshotName("WNX9"),
    SnapshotName("WNXJ"),
    SnapshotName("WNXK"),
    SnapshotName("WNX8"),
    SnapshotName("WNT0"),
    SnapshotName("WNXL"),
    SnapshotName("WNXM"),
    SnapshotName("WND0"),
    SnapshotName("WNXE"),
    SnapshotName("WNXF"),
    SnapshotName("WNXG"),
    SnapshotName("WNXH"),
    SnapshotName("WNUR"),
    SnapshotName("WT00"),
    SnapshotName("WT10"),
    SnapshotName("YMQB"),
    SnapshotName("WTA0"),
    SnapshotName("WTR0"),
    SnapshotName("WTS3"),
    SnapshotName("WTK0"),
    SnapshotName("WTK1"),
    SnapshotName("WTK2"),
    SnapshotName("WTP0"),
    SnapshotName("WTP1"),
    SnapshotName("WTP2"),
    SnapshotName("WTP3"),
    SnapshotName("WTP4"),
    SnapshotName("WTX2"),
    SnapshotName("WTX3"),
    SnapshotName("WTX4"),
    SnapshotName("WTTB"),
    SnapshotName("WTXB"),
    SnapshotName("WTY0"),
    SnapshotName("WTKB"),
    SnapshotName("WB10"),
    SnapshotName("WBBK"),
    SnapshotName("SYW0"),
    SnapshotName("WB01"),
    SnapshotName("WB00"),
    SnapshotName("WB20"),
    SnapshotName("WBS2"),
    SnapshotName("WR00"),
    SnapshotName("YTQB"),
    SnapshotName("WRP0"),
    SnapshotName("WRS1"),
    SnapshotName("WRS2"),
};

constexpr std::array<std::uint32_t, 69> snapshotKowloonAreas{
    SnapshotName("QQQQ"),
    SnapshotName("QR00"),
    SnapshotName("QR09"),
    SnapshotName("QRR0"),
    SnapshotName("Q100"),
    SnapshotName("Q109"),
    SnapshotName("QSFA"),
    SnapshotName("QSFD"),
    SnapshotName("QTB1"),
    SnapshotName("QTB4"),
    SnapshotName("Q200"),
    SnapshotName("QGBT"),
    SnapshotName("QSFB"),
    SnapshotName("QSFC"),
    SnapshotName("Q300"),
    SnapshotName("QTBB"),
    SnapshotName("QTB2"),
    SnapshotName("QT09"),
    SnapshotName("QTB3"),
    SnapshotName("QA10"),
    SnapshotName("QA13"),
    SnapshotName("QA12"),
    SnapshotName("QA14"),
    SnapshotName("QA15"),
    SnapshotName("QRC0"),
    SnapshotName("QA22"),
    SnapshotName("QAAG"),
    SnapshotName("QAAJ"),
    SnapshotName("QAFJ"),
    SnapshotName("QAFK"),
    SnapshotName("QB00"),
    SnapshotName("QB10"),
    SnapshotName("QB11"),
    SnapshotName("QBAA"),
    SnapshotName("QC10"),
    SnapshotName("QCAE"),
    SnapshotName("QABT"),
    SnapshotName("QCCE"),
    SnapshotName("QUG0"),
    SnapshotName("QCFE"),
    SnapshotName("QCHF"),
    SnapshotName("QCJE"),
    SnapshotName("QCLG"),
    SnapshotName("QD10"),
    SnapshotName("QDTB"),
    SnapshotName("QDKJ"),
    SnapshotName("QD20"),
    SnapshotName("QE09"),
    SnapshotName("QE12"),
    SnapshotName("QEDJ"),
    SnapshotName("GABT"),
    SnapshotName("QE53"),
    SnapshotName("QEEC"),
    SnapshotName("QEHI"),
    SnapshotName("QE10"),
    SnapshotName("QLBT"),
    SnapshotName("QE11"),
    SnapshotName("QEHH"),
    SnapshotName("QEFO"),
    SnapshotName("QEFP"),
    SnapshotName("QEFQ"),
    SnapshotName("QEFR"),
    SnapshotName("QEFS"),
    SnapshotName("QF10"),
    SnapshotName("QDTG"),
    SnapshotName("QF11"),
    SnapshotName("QF12"),
    SnapshotName("QFRR"),
    SnapshotName("QFHG"),
};

constexpr std::array<std::uint32_t, 17> snapshotGuilinAreas{
    SnapshotName("KRF1"),
    SnapshotName("KRH1"),
    SnapshotName("KRM3"),
    SnapshotName("0523"),
    SnapshotName("KRC1"),
    SnapshotName("KRK6"),
    SnapshotName("KRK*"),
    SnapshotName("KMZ*"),
    SnapshotName("KWM1"),
    SnapshotName("KWW1"),
    SnapshotName("KWW4"),
    SnapshotName("KWWQ"),
    SnapshotName("KWQ*"),
    SnapshotName("KSH1"),
    SnapshotName("KES1"),
    SnapshotName("KRZ1"),
    SnapshotName("BADK"),
};

std::mutex snapshotMetadataFileMutex;

std::array<S2SaveSlotInfo, S2SaveSlotCount> slotCatalog{};

bool slotCatalogInitialized = false;

HLib::Task *saveTask;

SaveTaskData *saveTaskData;

LegacySaveListState legacySaveList{};

float legacySaveGlyphScale = 1.0f;

SaveOperation saveOperation{};

std::uint32_t currentSaveState;

HLib::Task *lcdmTask = nullptr;

HLib::Task *windowTask = nullptr;

HLib::Task *titleWarningTask = nullptr;

std::int32_t titleWarningLogo = 0;

std::int32_t titleWarningUsesLogoParameters = 0;

std::array<std::byte, 0x0c> titleWarningLogoParameters{};

TitleWarningPhase titleWarningPhase = TitleWarningPhase::Inactive;

std::int32_t titleWarningTailFrames = 0;

std::uint32_t titleWarningBaseColour = 0;

std::uint32_t titleWarningDesiredColour = 0;

std::uint32_t titleWarningCurrentColour = 0;

enum class SaveScreenPrompt : std::uint8_t {
    None,
    Overwrite,
    Result,
};

SaveScreenPrompt saveScreenPrompt = SaveScreenPrompt::None;

std::size_t saveScreenPromptSlot = 0;

std::int32_t saveScreenStorageStatus = 0;

LegacyInputSlot saveScreenInput{};

TitleFileSelectState titleFileSelect{};

TitleControlData *activeTitleControl{};

std::int32_t gameLoadSlot{};

std::uint8_t gameLoadActive{};

std::int32_t gameLoadStatus{};

std::int32_t titleNewGameState{};

std::int32_t titleImportSelection{-1};

TitleLoadPhase titleLoadPhase = TitleLoadPhase::Idle;

bool titleLoadOnly = false;

bool titleFlowActive = false;

bool titleFailureReturnsToSelection = false;

std::array<char, 13> activeMessageToken{};

SavedPlayerTransform pendingPlayerTransform{};

PlayerPersistentState pendingPlayerPersistentState{};

bool pendingPlayerState = false;

// The native trail copy crosses the serialized SAVE boundary into live selection and backup metadata.
constexpr std::size_t kSerializedNaviTrailBytes = S2SavePayloadSize - kNaviTrailOffset;

constexpr std::size_t kAlternateNaviTrailBytes = sizeof(NaviTrailStorage) - kSerializedNaviTrailBytes - 16 - sizeof(g_backupSaveDataInfos);

static_assert(kSerializedNaviTrailBytes == 0xb28 && kAlternateNaviTrailBytes == 0x204);

NaviTrailStorage saveNaviTrailView{};

void SaveTaskCallback(void *taskData);

void SaveScreenInitialize(void *taskData);

void SaveScreenComplete(void *taskData);

void SaveScreenFadeOut(void *taskData);

} // namespace

#if SHENMUE_IS_PC
std::int16_t g_windowParticleDebugFlags{};
#endif

// Native: SNAP keeps the window message sprites out of a capture.
std::int32_t g_wndMessageDrawDisabled{};

std::array<std::uint16_t, 4> g_windowGlyphPalette{};

std::int32_t g_saveScreenAdmission = 0;

std::int32_t g_saveFrontendMode = 0;

TitleFileSelectState *g_titleFileSelect = &titleFileSelect;

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// get_tbl keeps the bank-reset order, including
// OutRun's additional WORD publication into each Harrier record's extra.
void ResetArcadeRankings() noexcept {
    g_hangOnRankings = arcade_defaults::hangOn;
    for (std::size_t index = 0; index != g_harrierRankings.size(); ++index) {
        g_harrierRankings[index].score = arcade_defaults::harrier[index].score;
        g_harrierRankings[index].initials = arcade_defaults::harrier[index].initials;
    }
    for (std::size_t index = 0; index != g_outRunRankings.size(); ++index) {
        g_outRunRankings[index] = arcade_defaults::outRun[index];
        g_harrierRankings[index].extra = arcade_defaults::outRunHarrierExtras[index];
    }
    g_afterBurnerRankings = arcade_defaults::afterBurner;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void (*backupLayerCallback)(std::int32_t stage, std::int32_t result, const void *source){};

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool SnapshotAreaMatches(std::uint32_t pattern, std::uint32_t area) noexcept {
    for (std::uint32_t shift = 0; shift < 32; shift += 8) {
        const std::uint32_t part = (pattern >> shift) & 0xffU;
        if (part != static_cast<std::uint32_t>('*') && part != ((area >> shift) & 0xffU)) {
            return false;
        }
    }
    return true;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

template <std::size_t Size> std::uint32_t MatchSnapshotArea(std::uint32_t area, const std::array<std::uint32_t, Size> &patterns, std::uint32_t base) noexcept {
    for (std::size_t index = 0; index < patterns.size(); ++index) {
        if (SnapshotAreaMatches(patterns[index], area))
            return base + static_cast<std::uint32_t>(index);
    }
    return 0x100000U;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::uint32_t GetSnapshotAreaSortKey(std::uint32_t area) noexcept {
    std::uint32_t result = MatchSnapshotArea(area, snapshotAberdeenAreas, 0x10000U);
    if (result != 0x100000U)
        return result;
    result = MatchSnapshotArea(area, snapshotWanChaiAreas, 0x20000U);
    if (result != 0x100000U)
        return result;
    result = MatchSnapshotArea(area, snapshotKowloonAreas, 0x30000U);
    if (result != 0x100000U)
        return result;
    result = MatchSnapshotArea(area, snapshotGuilinAreas, 0x40000U);
    if (result != 0x100000U)
        return result;
    if (area == SnapshotName("0549"))
        return 0x80000U;
    return 0x100000U;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int32_t GetSnapshotBioSortKey(std::uint32_t name) noexcept {
    std::int32_t index = 0;
    for (; index < 48; ++index)
        if (SNAPSHOT_BIOGRAPHIES[index].character.name == name)
            break;
    if (name == SNAPSHOT_BIO_SYP.name || name == SNAPSHOT_BIO_SYX.name || name == SNAPSHOT_BIO_KSY.name)
        return 8;
    if (name == SNAPSHOT_BIO_WSH.name || name == SNAPSHOT_BIO_SN5.name || name == SNAPSHOT_BIO_SNB.name)
        return 40;
    if (name == SNAPSHOT_BIO_GAI2.name)
        return 28;
    return index;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void UpdateSnapshotAreaSortKey(SnapshotMetadataRecord &record) noexcept {
    if (!record.use) {
        record.sortKeys[2] = 0x100000;
        return;
    }
    if (!record.area.name) {
        record.area = SNAPSHOT_EMPTY_AREA;
        record.sortKeys[2] = 0x100000;
        return;
    }
    std::uint32_t key = GetSnapshotAreaSortKey(record.area.name);
    const std::uint32_t subArea = GetSnapshotAreaSortKey(record.subArea.name), shop = GetSnapshotAreaSortKey(record.shop.name);
    if (shop != 0x100000U)
        key = shop;
    if (key == 0x100000U)
        key = subArea;
    else if (subArea != 0x100000U && subArea < key)
        key = subArea;
    if (key == 0x100000U) {
        switch ((record.area.name & 0xffU) | 0x30000000U) {
        case SNAPSHOT_AREA_ABERDEEN.name:
            key = 0x10000U;
            break;
        case SNAPSHOT_AREA_GUILIN.name:
            key = 0x40001U;
            break;
        case SNAPSHOT_AREA_KOWLOON.name:
            key = 0x30001U;
            break;
        case SNAPSHOT_AREA_WAN_CHAI.name:
            key = 0x2004fU;
            break;
        default:
            key = 0xfffffU;
            break;
        }
    }
    record.sortKeys[2] = static_cast<std::int32_t>(key);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void UpdateSnapshotSortKeys(SnapshotMetadataRecord &record) noexcept {
    if (!record.use) {
        record.sortKeys[0] = -1;
        record.sortKeys[1] = -1;
        record.sortKeys[2] = 0x100000;
        record.sortKeys[3] = GetSnapshotBioSortKey(record.lockOnId.name);
        return;
    }
    const std::uint32_t packed = static_cast<std::uint32_t>(record.second) | ((static_cast<std::uint32_t>(record.year) << 26U) - 0x40000000U) |
        ((static_cast<std::uint32_t>(record.minute) |
             ((static_cast<std::uint32_t>(record.hour) | (32U * (static_cast<std::uint32_t>(record.day) | (32U * static_cast<std::uint32_t>(record.month)))))
                 << 6U))
            << 6U);
    record.sortKeys[0] = static_cast<std::int32_t>(~packed);
    record.sortKeys[1] = static_cast<std::int32_t>(packed);

    UpdateSnapshotAreaSortKey(record);
    record.sortKeys[3] = GetSnapshotBioSortKey(record.lockOnId.name);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::filesystem::path SnapshotMetadataPath() {
    return saveDirectory / "SnapshotData";
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void FreeSnapshotMetadataWrite(std::int32_t, const void *source) {
    std::free(const_cast<void *>(source));
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool WriteSnapshotMetadataFile() {
#if defined(SHENMUE_STEAM)
    std::unique_lock<std::mutex> lock(snapshotMetadataFileMutex);
#else
    const std::lock_guard lock(snapshotMetadataFileMutex);
#endif
    Json::Value root(Json::objectValue);
    Json::Value& header = root["Header"];
    header["id"] = std::string(snapshotMetadata.identifier.begin(),
        std::find(snapshotMetadata.identifier.begin(),
                  snapshotMetadata.identifier.end(), '\0'));
    Json::Value flags(Json::arrayValue);
    for (const std::uint32_t flag : snapshotMetadata.omakeFlags) {
        Json::Value value(Json::objectValue);
        value["use"] = flag;
        flags.append(std::move(value));
    }
    header["omakeflags"] = std::move(flags);
    header["reserve"] = snapshotMetadata.reserve;
    Json::Value snapshots(Json::arrayValue);
    for (const SnapshotMetadataRecord& record :
         snapshotMetadata.records) {
        if (record.use != 1) continue;
        Json::Value value(Json::objectValue);
        value["id"] = record.id;
        value["area"] = record.area.name;
        value["sub_area"] = record.subArea.name;
        value["lockon_id"] = record.lockOnId.name;
        value["shop"] = record.shop.name;
        value["use"] = record.use;
        Json::Value date(Json::objectValue);
        date["year"] = record.year;
        date["month"] = record.month;
        date["day"] = record.day;
        date["hour"] = record.hour;
        date["minute"] = record.minute;
        date["second"] = record.second;
        date["week"] = record.week;
        value["date"] = std::move(date);
        snapshots.append(std::move(value));
    }
    root["Snapshots"]["Snap"] = std::move(snapshots);

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    const std::string json = Json::writeString(builder, root);
    void* const buffer = std::malloc(json.size());
    if (!buffer) return false;
    std::memcpy(buffer, json.data(), json.size());
#if defined(SHENMUE_STEAM)
    lock.unlock();
#endif
    return QueueSaveFileWrite("SnapshotData", buffer, static_cast<std::uint32_t>(json.size()), FreeSnapshotMetadataWrite) == 0;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool LoadSnapshotMetadataFile() {
#if defined(SHENMUE_STEAM)
    const std::vector<std::byte> bytes = ::shenmue::d3t::ReadSteamFileBytes("SnapshotData");
    if (bytes.empty()) return false;
    std::istringstream input(std::string(reinterpret_cast<const char *>(bytes.data()), bytes.size()));
    const std::lock_guard<std::mutex> lock(snapshotMetadataFileMutex);
#else
    const std::lock_guard lock(snapshotMetadataFileMutex);
    std::ifstream input(SnapshotMetadataPath(), std::ios::binary);
#endif
    if (!input) return false;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    Json::Value root;
    std::string errors;
    if (!Json::parseFromStream(builder, input, &root, &errors)) return false;
    const Json::Value& header = root["Header"];
    if (header["id"].asString() != "S2XSnapshots1.07") return false;
    const Json::Value& flags = header["omakeflags"];
    for (Json::ArrayIndex index = 0;
         index < flags.size() &&
         index < snapshotMetadata.omakeFlags.size(); ++index) {
        snapshotMetadata.omakeFlags[index] =
            flags[index]["use"].asUInt();
    }
    snapshotMetadata.reserve = header["reserve"].asUInt();
    const Json::Value& snapshots = root["Snapshots"]["Snap"];
    if (!snapshots.isArray()) return true;
    for (const Json::Value& value : snapshots) {
        const std::int32_t id = value["id"].asInt();
        if (id < 0 ||
            static_cast<std::size_t>(id) >=
                snapshotMetadata.records.size()) {
            continue;
        }
        SnapshotMetadataRecord& record =
            snapshotMetadata.records[static_cast<std::size_t>(id)];
        record.id = id;
        record.use = value["use"].asInt();
        record.area.name = value["area"].asUInt();
        record.subArea.name = value["sub_area"].asUInt();
        record.shop.name = value["shop"].asUInt();
        record.reserved14 = 0;
        record.lockOnId.name = value["lockon_id"].asUInt();
        const Json::Value& date = value["date"];
        record.year = static_cast<std::uint16_t>(
            date["year"].asUInt());
        record.month = static_cast<std::uint8_t>(
            date["month"].asUInt());
        record.day = static_cast<std::uint8_t>(
            date["day"].asUInt());
        record.hour = static_cast<std::uint8_t>(
            date["hour"].asUInt());
        record.minute = static_cast<std::uint8_t>(
            date["minute"].asUInt());
        record.second = static_cast<std::uint8_t>(
            date["second"].asUInt());
        record.week = static_cast<std::uint8_t>(
            date["week"].asUInt());
        UpdateSnapshotSortKeys(record);
    }
    return true;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if defined(_WIN32)
void CompleteSnapshotSave(std::int32_t result, const void *) {
    if (result)
        ValidateSnapshotMetadataFiles();
    else
        SaveSnapshotMetadata();
    snapshotSaveInProgress.store(0, std::memory_order_seq_cst);
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if defined(_WIN32)
void AppendSnapshotJpegBytes(void *context, void *data, int size) {
    std::memcpy(d3t::d3t_AppendBuffer(*static_cast<d3t::D3TByteBuffer *>(context), static_cast<std::uint32_t>(size)), data, static_cast<std::size_t>(size));
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if defined(_WIN32)
std::int32_t WriteSnapshotTask(void *data) {
    SnapshotSaveWork &work = *static_cast<SnapshotSaveWork *>(data);
    snapshotJpegBuffer.size = 0;
    stbi_write_jpg_to_func(AppendSnapshotJpegBytes, &snapshotJpegBuffer, static_cast<int>(work.width), static_cast<int>(work.height), 4, work.pixels, 85);
    std::free(work.pixels);
    char path[32];
    std::snprintf(path, sizeof(path), "%u.jpg", work.slot);
    QueueSaveFileWrite(path, snapshotJpegBuffer.data, snapshotJpegBuffer.size, CompleteSnapshotSave);
    return 0;
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::span<std::byte> InventoryBank(S2InventoryBank bank) noexcept {
    switch (bank) {
    case S2InventoryBank::Type0:
        return inventoryState.type0;
    case S2InventoryBank::Type1:
        return inventoryState.type1;
    case S2InventoryBank::Type2:
        return inventoryState.type2;
    case S2InventoryBank::Type3:
        return inventoryState.type3;
    case S2InventoryBank::Type4:
        return inventoryState.type4;
    case S2InventoryBank::Type5:
        return inventoryState.type5;
    }
    return {};
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::span<const std::byte> InventoryBankConst(S2InventoryBank bank) noexcept {
    const std::span<std::byte> bankData = InventoryBank(bank);
    return {bankData.data(), bankData.size()};
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

template <typename Value> Value ReadValue(const S2SaveImage &image, std::size_t offset) noexcept {
    Value value{};
    std::memcpy(&value, image.bytes.data() + offset, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

template <typename Value> void WriteValue(S2SaveImage &image, std::size_t offset, const Value &value) noexcept {
    std::memcpy(image.bytes.data() + offset, &value, sizeof(value));
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::span<const std::byte> Payload(const S2SaveImage &image) noexcept {
    return {image.bytes.data() + kPayloadOffset, S2SavePayloadSize};
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::span<std::byte> Payload(S2SaveImage &image) noexcept {
    return {image.bytes.data() + kPayloadOffset, S2SavePayloadSize};
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void InitializeImageLayout(S2SaveImage &image) noexcept {
    image = {};
    WriteValue(image, kSystemHeaderOffset, kSystemMagic);
    WriteValue(image, kSystemHeaderOffset + 4, kSaveVersion);
    WriteValue(image, kGameHeaderOffset, kGameMagic);
    WriteValue(image, kGameHeaderOffset + 4, kSaveVersion);
    S2SaveDataInfo info{};
    info.checksum = S2Save_Checksum(Payload(image));
    WriteValue(image, kSaveDataInfoOffset, info);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

S2SaveDataInfo SaveDataInfo(const S2SaveImage &image) noexcept {
    return ReadValue<S2SaveDataInfo>(image, kSaveDataInfoOffset);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void WriteSaveDataInfo(S2SaveImage &image, const S2SaveDataInfo &info) noexcept {
    WriteValue(image, kSaveDataInfoOffset, info);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool SaveDataInfoIsValid(const S2SaveDataInfo &info) noexcept {
    if (info.saveKind == 0)
        return true;
    return info.systemMonth >= 1 && info.systemMonth <= 12 && info.systemDay >= 1 && info.systemDay <= 31 && info.systemHour < 24 && info.systemMinute < 60 &&
        info.systemSecond < 60 && info.systemWeek < 7 && info.gameYear >= 0 && info.gameYear <= 99 && info.gameMonth >= 1 && info.gameMonth <= 12 &&
        info.gameDay >= 1 && info.gameDay <= 31 && info.gameHour < 24 && info.gameMinute < 60 && info.gameSecond < 60 && info.gameWeek < 7;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool GameDateTimeIsValid(const DateTime &value) noexcept {
    return value.year >= 0 && value.year <= 99 && value.month >= 1 && value.month <= 12 && value.day >= 1 && value.day <= 31 && value.hour >= 0 &&
        value.hour < 24 && value.minute >= 0 && value.minute < 60 && value.second >= 0 && value.second < 60 && value.week < 7;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool GetLocalTime(std::tm &result) noexcept {
    const std::time_t value = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#ifdef _WIN32
    return localtime_s(&result, &value) == 0;
#else
    return localtime_r(&value, &result) != nullptr;
#endif
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void CaptureBitFlags(std::span<std::byte> output, PlayerFlagReadMode mode) noexcept {
    std::fill(output.begin(), output.end(), std::byte{});
    if (mode == PlayerFlagReadMode::Stat) {
        for (std::size_t index = 0; index < output.size(); ++index) {
            output[index] = static_cast<std::byte>(EV_GetPlayerFlag(mode, static_cast<std::int16_t>(index)));
        }
        return;
    }
    for (std::size_t byte = 0; byte < output.size(); ++byte) {
        std::uint8_t value = 0;
        for (std::size_t bit = 0; bit < 8; ++bit) {
            const auto flag = static_cast<std::int16_t>(byte * 8 + bit);
            if (EV_GetPlayerFlag(mode, flag) != 0)
                value |= static_cast<std::uint8_t>(1U << bit);
        }
        output[byte] = static_cast<std::byte>(value);
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void LoadSaveNaviTrail(std::span<const std::byte> payload) noexcept {
    std::byte *cursor = reinterpret_cast<std::byte *>(&saveNaviTrailView);
    std::memcpy(cursor, payload.data() + kNaviTrailOffset, kSerializedNaviTrailBytes);
    cursor += kSerializedNaviTrailBytes;
    std::memcpy(cursor, &retainedSaveSelection, 4);
    cursor += 4;
    std::memcpy(cursor, &loadedGameFlags, 4);
    cursor += 4;
    std::memcpy(cursor, &loadedCompletedGameCount, 4);
    cursor += 4;
    std::memcpy(cursor, &localBackupInfo, 4);
    cursor += 4;
    std::memcpy(cursor, g_backupSaveDataInfos.data(), sizeof(g_backupSaveDataInfos));
    cursor += sizeof(g_backupSaveDataInfos);
    std::memcpy(cursor, g_backupS1SaveDataInfos.data(), kAlternateNaviTrailBytes);
    LoadNaviTrail(&saveNaviTrailView);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void CaptureSaveNaviTrail(std::span<std::byte> payload) noexcept {
    SaveNaviTrail(&saveNaviTrailView);
    const std::byte *cursor = reinterpret_cast<const std::byte *>(&saveNaviTrailView);
    std::memcpy(payload.data() + kNaviTrailOffset, cursor, kSerializedNaviTrailBytes);
    cursor += kSerializedNaviTrailBytes;
    std::memcpy(&retainedSaveSelection, cursor, 4);
    cursor += 4;
    std::memcpy(&loadedGameFlags, cursor, 4);
    cursor += 4;
    std::memcpy(&loadedCompletedGameCount, cursor, 4);
    cursor += 4;
    std::memcpy(&localBackupInfo, cursor, 4);
    cursor += 4;
    std::memcpy(g_backupSaveDataInfos.data(), cursor, sizeof(g_backupSaveDataInfos));
    cursor += sizeof(g_backupSaveDataInfos);
    std::memcpy(g_backupS1SaveDataInfos.data(), cursor, kAlternateNaviTrailBytes);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void CaptureSavedPlayerPlacement(SavedPlayerTransform &transform, std::span<std::byte, 0x20> placement) noexcept {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = static_cast<Character *>(control->ryo);
    if (!character) {
        transform = {};
        return;
    }
    transform.position = GetCharacterWorldPosition(character);
    transform.yaw = static_cast<std::int16_t>(GetAllAngYForNode(&character->entity.node));
    transform.characterState = static_cast<std::uint8_t>(GetCharacterInternalState(character));
    for (std::int32_t offset = 0; offset < 0x20; ++offset)
        placement[offset] = static_cast<std::byte>(GetAreaJumpPlacementByte(offset));
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void CaptureRuntimePayload(S2SaveImage &image, bool capturePlacement = true) noexcept {
    std::span<std::byte> payload = Payload(image);

    playerStats.gameConfigurationFlags = GetS2GameConfigurationFlags();
    playerStats.cash = GetCurrentCash();
    std::memcpy(payload.data() + kPlayerSavedStatOffset, &playerStats.savedStat0, sizeof(playerStats.savedStat0));
    std::copy(playerStats.statFlags.begin(), playerStats.statFlags.end(), payload.begin() + kPlayerStatFlagsOffset);
    std::memcpy(payload.data() + kPlayerRuntimeFlagsOffset, &playerStats.runtimeFlags, sizeof(playerStats.runtimeFlags));
    std::memcpy(payload.data() + kCashOffset, &playerStats.cash, sizeof(playerStats.cash));
    std::memcpy(payload.data() + kSegaCoinsOffset, &playerStats.segaCoins, sizeof(playerStats.segaCoins));
    std::memcpy(payload.data() + kHealthOffset, &playerStats.alternateHealth, sizeof(std::uint16_t));
    std::memcpy(payload.data() + kHealthOffset + 2, &playerStats.defaultHealth, sizeof(std::uint16_t));
    std::memcpy(payload.data() + kHealthOffset + 4, &playerStats.reactionCameraScale, sizeof(std::uint16_t));
    std::memcpy(payload.data() + kHealthOffset + 6, &playerStats.assistScale, sizeof(std::uint16_t));
    std::memcpy(payload.data() + kGameConfigurationOffset, &playerStats.gameConfigurationFlags, sizeof(playerStats.gameConfigurationFlags));
    std::memcpy(payload.data() + kContextActionOffset, &playerStats.contextActionMode, sizeof(playerStats.contextActionMode));
    for (std::size_t bank = 0; bank < moveSkillBanks.size(); ++bank) {
        const S2MoveSkillBank &moves = moveSkillBanks[bank];
        const std::size_t offset = kMoveSkillBankOffset + bank * kMoveSkillBankStride;
        const std::int32_t count = static_cast<std::int8_t>(moves.count);
        for (std::int32_t entry = 0; entry < count; ++entry) {
            std::memcpy(payload.data() + offset + entry * 2, &moves.proficiency[entry], sizeof(std::uint16_t));
            std::memcpy(payload.data() + offset + 0x50 + entry * 2, &moves.usage[entry], sizeof(std::uint16_t));
        }
        payload[offset + 0xa0] = static_cast<std::byte>(moves.count);
        payload[offset + 0xa1] = static_cast<std::byte>(moves.selected);
    }

    std::array<std::byte, 8> storyFlags{};
    std::array<std::byte, 0x200> internalFlags{};
    std::array<std::byte, 0x40> stats{};
    CaptureBitFlags(storyFlags, PlayerFlagReadMode::Story);
    CaptureBitFlags(internalFlags, PlayerFlagReadMode::Internal);
    CaptureBitFlags(stats, PlayerFlagReadMode::Stat);
    std::copy(storyFlags.begin(), storyFlags.end(), payload.begin() + kStoryFlagsOffset);
    std::copy(internalFlags.begin(), internalFlags.end(), payload.begin() + kInternalFlagsOffset);
    std::copy(stats.begin(), stats.end(), payload.begin() + kStatsOffset);

    std::copy(inventoryState.type0.begin(), inventoryState.type0.end(), payload.begin() + kInventoryType0Offset);
    std::copy(inventoryState.type1.begin(), inventoryState.type1.end(), payload.begin() + kInventoryType1Offset);
    std::copy(inventoryState.type2.begin(), inventoryState.type2.end(), payload.begin() + kInventoryType2Offset);
    std::copy(inventoryState.type3.begin(), inventoryState.type3.end(), payload.begin() + kInventoryType3Offset);
    std::copy(inventoryState.type4.begin(), inventoryState.type4.end(), payload.begin() + kInventoryType4Offset);
    std::copy(inventoryState.type5.begin(), inventoryState.type5.end(), payload.begin() + kInventoryType5Offset);
    std::memcpy(payload.data() + kMinigameScoresOffset, minigameScores.data(), sizeof(minigameScores));
    std::memcpy(payload.data() + kHarrierRankingsOffset, g_harrierRankings.data(), sizeof(g_harrierRankings));
    std::memcpy(payload.data() + kHangOnRankingsOffset, g_hangOnRankings.data(), sizeof(g_hangOnRankings));
    std::memcpy(payload.data() + kAfterBurnerRankingsOffset, g_afterBurnerRankings.data(), sizeof(g_afterBurnerRankings));
    std::memcpy(payload.data() + kOutRunRankingsOffset, g_outRunRankings.data(), sizeof(g_outRunRankings));
    for (std::int32_t index = 0; index < 5; ++index) {
        const std::int32_t value = GetGameSysActivityCounter(index);
        std::memcpy(payload.data() + kActivityCountersOffset + index * 4, &value, sizeof(value));
    }
    const std::uint16_t memoPage = GetMemoPageState(1);
    const std::uint16_t memoFlag = GetMemoPageState(2);
    std::memcpy(payload.data() + kMemoPageOffset, &memoPage, sizeof(memoPage));
    std::memcpy(payload.data() + kMemoFlagOffset, &memoFlag, sizeof(memoFlag));
    const shen_name queuedAuthor = GetCurrentAuthorId();
    std::memcpy(payload.data() + kQueuedDoorAuthorOffset, &queuedAuthor, sizeof(queuedAuthor));

    const std::int32_t magicRoom = static_cast<std::int32_t>(GetMagicRoomActiveFlag());
    std::memcpy(payload.data() + kMagicRoomActiveOffset, &magicRoom, sizeof(magicRoom));
    if (capturePlacement) {
        SavedPlayerTransform transform;
        std::memcpy(&transform, payload.data() + kPlayerTransformOffset, sizeof(transform));
        CaptureSavedPlayerPlacement(transform, payload.subspan<kPlayerAuxiliaryOffset, 0x20>());
        std::memcpy(payload.data() + kPlayerTransformOffset, &transform, sizeof(transform));
    }
    PlayerPersistentState persistent{};
    plymExportPersistentState(&persistent);
    std::memcpy(payload.data() + kPlayerPersistentStateOffset, &persistent, sizeof(persistent));
    payload[S2StreetFightProgressOffset] = static_cast<std::byte>(GetStreetFightProgress());
    std::memcpy(payload.data() + kTransferredYenOffset, &playerStats.transferredYen, sizeof(playerStats.transferredYen));
    const std::int32_t floor = static_cast<std::int32_t>(GetQronNextStream());
    std::memcpy(payload.data() + kQronFloorOffset, &floor, sizeof(floor));
    CaptureElevatorPersistentState(payload.subspan<kElevatorStateOffset, ElevatorPersistentStateSize>());
    for (std::int8_t index = 0; index < 16; ++index)
        payload[kFightMotionNibblesOffset + index] = static_cast<std::byte>(vfMotnNibbleGet(index));
    CaptureNpcScheduleSeeds(payload.subspan<kNpcScheduleSeedOffset, NpcScheduleSeedBufferSize>());
    if (capturePlacement) {
        PartyCompanionTransition companion{};
        ExportPartyCompanionTransition(companion);
        std::memcpy(payload.data() + kCompanionStateOffset, &companion, sizeof(companion));
    } else
        std::memset(payload.data() + kCompanionStateOffset, 0, sizeof(PartyCompanionTransition));
    CopyShenmueFlagsTo(payload.subspan<kShenmueFlagBlockOffset, 0x34>());
    SaveNaviMarkers(reinterpret_cast<std::uint32_t *>(payload.data() + kNaviMarkersOffset));
    CaptureSaveNaviTrail(payload);
    CaptureAreaJumpPersistentState(payload.subspan<kAreaJumpStateOffset, sizeof(AreaJumpPersistentState)>());
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
std::int32_t ImportPendingPlayerPersistentState(const PlayerPersistentState &state) noexcept {
    if (state.present == 0)
        return -1;
    pendingPlayerPersistentState = state;
    return 0;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DecodeRuntimePayload(const S2SaveImage &image) noexcept {
    const std::span<const std::byte> payload = Payload(image);

    std::memcpy(&playerStats.savedStat0, payload.data() + kPlayerSavedStatOffset, sizeof(playerStats.savedStat0));
    std::copy_n(payload.begin() + kPlayerStatFlagsOffset, playerStats.statFlags.size(), playerStats.statFlags.begin());
    std::memcpy(&playerStats.runtimeFlags, payload.data() + kPlayerRuntimeFlagsOffset, sizeof(playerStats.runtimeFlags));
    std::memcpy(&playerStats.cash, payload.data() + kCashOffset, sizeof(playerStats.cash));
    SetCurrentCash(playerStats.cash);
    std::memcpy(&playerStats.segaCoins, payload.data() + kSegaCoinsOffset, sizeof(playerStats.segaCoins));
    std::memcpy(&playerStats.alternateHealth, payload.data() + kHealthOffset, sizeof(std::uint16_t));
    std::memcpy(&playerStats.defaultHealth, payload.data() + kHealthOffset + 2, sizeof(std::uint16_t));
    std::memcpy(&playerStats.reactionCameraScale, payload.data() + kHealthOffset + 4, sizeof(std::uint16_t));
    std::memcpy(&playerStats.assistScale, payload.data() + kHealthOffset + 6, sizeof(std::uint16_t));
    if (!GetTitleVersionOverride()) {
        std::memcpy(&playerStats.gameConfigurationFlags, payload.data() + kGameConfigurationOffset, sizeof(playerStats.gameConfigurationFlags));
        SetS2GameConfigurationFlags(playerStats.gameConfigurationFlags);
    } else
        playerStats.gameConfigurationFlags = GetS2GameConfigurationFlags();
    std::memcpy(&playerStats.contextActionMode, payload.data() + kContextActionOffset, sizeof(playerStats.contextActionMode));
    for (std::size_t bank = 0; bank < moveSkillBanks.size(); ++bank) {
        S2MoveSkillBank &moves = moveSkillBanks[bank];
        const std::size_t offset = kMoveSkillBankOffset + bank * kMoveSkillBankStride;

        moves.count = std::to_integer<std::uint8_t>(payload[offset + 0xa0]);
        moves.selected = std::to_integer<std::uint8_t>(payload[offset + 0xa1]);
        const std::int32_t count = static_cast<std::int8_t>(moves.count);
        for (std::int32_t entry = 0; entry < count; ++entry) {
            std::memcpy(&moves.proficiency[entry], payload.data() + offset + entry * 2, sizeof(std::uint16_t));
            std::memcpy(&moves.usage[entry], payload.data() + offset + 0x50 + entry * 2, sizeof(std::uint16_t));
        }
    }

    std::uint8_t *flagBanks[3];
    GetScenarioFlagBanks(flagBanks);
    std::memcpy(flagBanks[0], payload.data() + kStoryFlagsOffset, 8);
    std::memcpy(flagBanks[1], payload.data() + kInternalFlagsOffset, 0x200);
    std::memcpy(flagBanks[2], payload.data() + kStatsOffset, 0x40);

    std::copy_n(payload.begin() + kInventoryType0Offset, inventoryState.type0.size(), inventoryState.type0.begin());
    std::copy_n(payload.begin() + kInventoryType1Offset, inventoryState.type1.size(), inventoryState.type1.begin());
    std::copy_n(payload.begin() + kInventoryType2Offset, inventoryState.type2.size(), inventoryState.type2.begin());
    std::copy_n(payload.begin() + kInventoryType3Offset, inventoryState.type3.size(), inventoryState.type3.begin());
    std::copy_n(payload.begin() + kInventoryType4Offset, inventoryState.type4.size(), inventoryState.type4.begin());
    std::copy_n(payload.begin() + kInventoryType5Offset, inventoryState.type5.size(), inventoryState.type5.begin());
    std::memcpy(minigameScores.data(), payload.data() + kMinigameScoresOffset, sizeof(minigameScores));
    std::memcpy(g_harrierRankings.data(), payload.data() + kHarrierRankingsOffset, sizeof(g_harrierRankings));
    std::memcpy(g_hangOnRankings.data(), payload.data() + kHangOnRankingsOffset, sizeof(g_hangOnRankings));
    std::memcpy(g_afterBurnerRankings.data(), payload.data() + kAfterBurnerRankingsOffset, sizeof(g_afterBurnerRankings));
    std::memcpy(g_outRunRankings.data(), payload.data() + kOutRunRankingsOffset, sizeof(g_outRunRankings));
    std::copy_n(payload.begin() + kPlayerAuxiliaryOffset, playerAuxiliaryState.size(), playerAuxiliaryState.begin());

    for (std::int32_t index = 0; index < 5; ++index) {
        std::int32_t value;
        std::memcpy(&value, payload.data() + kActivityCountersOffset + index * 4, sizeof(value));
        SetGameSysActivityCounter(index, value);
    }
    std::uint16_t memoPage{};
    std::uint16_t memoFlag{};
    std::memcpy(&memoPage, payload.data() + kMemoPageOffset, sizeof(memoPage));
    std::memcpy(&memoFlag, payload.data() + kMemoFlagOffset, sizeof(memoFlag));
    shen_name queuedAuthor;
    std::memcpy(&queuedAuthor, payload.data() + kQueuedDoorAuthorOffset, sizeof(queuedAuthor));
    doorQueueAuthorForMap(queuedAuthor);
    SetTemporaryAreaJumpAuthor(queuedAuthor);
    std::int32_t magicRoom;
    std::memcpy(&magicRoom, payload.data() + kMagicRoomActiveOffset, sizeof(magicRoom));
    SetMagicRoomActiveFlag(magicRoom);
    InitializeMemoPageState(static_cast<std::int16_t>(memoPage), static_cast<std::int16_t>(memoFlag));
    PlayerPersistentState persistent{};
    std::memcpy(&persistent, payload.data() + kPlayerPersistentStateOffset, sizeof(persistent));
    ImportPendingPlayerPersistentState(persistent);
    std::memcpy(&pendingPlayerTransform, payload.data() + kPlayerTransformOffset, sizeof(pendingPlayerTransform));
    SavedAreaJumpLocation location;
    std::memcpy(&location, payload.data() + kPlayerTransformOffset, sizeof(location));
    RestoreSavedAreaJumpLocation(location, payload.subspan<kPlayerAuxiliaryOffset, 0x20>());
    SetStreetFightProgress(std::to_integer<std::uint8_t>(payload[S2StreetFightProgressOffset]));
    std::memcpy(&playerStats.transferredYen, payload.data() + kTransferredYenOffset, sizeof(playerStats.transferredYen));
    for (std::int8_t index = 0; index < 16; ++index)
        vfMotnNibbleSet(index, std::to_integer<std::uint8_t>(payload[kFightMotionNibblesOffset + index]));
    SetPendingNpcScheduleSeedBuffer(payload.subspan<kNpcScheduleSeedOffset, NpcScheduleSeedBufferSize>());
    PartyCompanionTransition companion;
    std::memcpy(&companion, payload.data() + kCompanionStateOffset, sizeof(companion));
    ImportPartyCompanionTransition(companion);
    std::int32_t floor{};
    std::memcpy(&floor, payload.data() + kQronFloorOffset, sizeof(floor));
    SetQronEntryFloor(floor);
    RestoreElevatorPersistentState(payload.subspan<kElevatorStateOffset, ElevatorPersistentStateSize>());
    SetShenmueFlagsTo(payload.subspan<kShenmueFlagBlockOffset, 0x34>());
    LoadNaviMarkers(reinterpret_cast<const std::uint32_t *>(payload.data() + kNaviMarkersOffset));
    LoadSaveNaviTrail(payload);
    RestoreAreaJumpPersistentState(payload.subspan<kAreaJumpStateOffset, sizeof(AreaJumpPersistentState)>());
    const S2SaveDataInfo info = SaveDataInfo(image);
    g_currentDateTime = {.year = static_cast<std::int8_t>(info.gameYear),
        .month = static_cast<std::int8_t>(info.gameMonth),
        .day = static_cast<std::int8_t>(info.gameDay),
        .week = info.gameWeek,
        .hour = static_cast<std::int8_t>(info.gameHour),
        .minute = static_cast<std::int8_t>(info.gameMinute),
        .second = static_cast<std::int8_t>(info.gameSecond)};
    SetSTEP(static_cast<std::int16_t>(info.stepHundreds) * 100 + static_cast<std::int8_t>(info.stepRemainder));
    pendingPlayerState = true;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void ResetInventory() noexcept {
    inventoryState = {};
    minigameScores = {};
    playerAuxiliaryState = {};
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool SaveSlotEligible(std::size_t slot, bool completedOnly) noexcept {
    if (slot >= slotCatalog.size())
        return false;
    const S2SaveSlotInfo &info = slotCatalog[slot];
    return info.result == S2SaveResult::Success && info.data.saveKind != 0 && (!completedOnly || info.data.clearState == 6);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t FirstEligibleSlot(bool completedOnly) noexcept {
    for (std::size_t slot = 0; slot < slotCatalog.size(); ++slot) {
        if (SaveSlotEligible(slot, completedOnly))
            return static_cast<std::int32_t>(slot);
    }
    return -1;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void SetMessageToken(const char *token) {
    activeMessageToken = {};
    if (!token)
        return;
    const std::size_t length = std::min<std::size_t>(std::strlen(token), activeMessageToken.size() - 1);
    std::copy_n(token, length, activeMessageToken.begin());
    const std::string_view localized = GetLocalizedSceneText(std::string_view{activeMessageToken.data(), length});
    if (!localized.empty()) {
        BeginSubtitlePresentation(std::string_view{activeMessageToken.data(), length}, localized, g_sceneTextLanguage, 0);
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void ShowMessage(const char *token, bool confirmation) {
    SetMessageToken(token);
    if (confirmation) {
        ShowElementByIndex(UIElement::YesNoPrompt);
        EnableElement(UIElement::YesNoPrompt);
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DismissMessage() {
    activeMessageToken = {};
    EndSubtitlePresentation();
    DestroyElementByIndex(UIElement::YesNoPrompt);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool ConsumeConfirmInput(LegacyInputSlot &input) {
    const bool confirmed = ConsumeUIEnterPressed() || (input.pressedPriority & 0x0200U) != 0;
    if (confirmed)
        input.pressedPriority &= 0xfdffU;
    return confirmed;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool ConsumeCancelInput(LegacyInputSlot &input) {
    const bool cancelled = ConsumeUICancelPressed() || (input.pressedPriority & 0x0400U) != 0;
    if (cancelled)
        input.pressedPriority &= 0xfbffU;
    return cancelled;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void SaveScreenComplete(void *data);

void SaveScreenDiskFade(void *);

void SaveScreenDiskComplete(void *);

void SaveScreenRun(void *data);

std::int32_t MeasureSaveText(const std::uint8_t *text, std::int32_t font);

void DrawSaveText(std::int32_t x, std::int32_t y, const char *source, std::int32_t font);

} // namespace

std::int32_t BackupCompleteCallback(std::int32_t port, std::int32_t operation, std::int32_t result);

std::int32_t WaitForBackupOperation(std::int32_t index);

std::int32_t CheckBackupFileExists(std::int32_t index, const char *filename);

std::int32_t GetBackupFreeBlocks(std::int32_t index);

bool ValidateBackupSaveImage(void *image);

void SetSceneInfos(std::int32_t actNumber, shen_name mapName, std::int32_t entryPoint);

void SaveScreenUpdate(void *taskData);

void CaptureSavedGamePayload(std::byte *payload, std::byte *metadata, std::int32_t initializationMode);

void RestoreSavedGamePayload(const std::byte *metadata, const std::byte *payload);

void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);

void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);

LegacyInputSlot saveScreenInput = {};

namespace {

std::byte systemSaveImage[0x6b4] __attribute__((aligned(4)));
std::byte gameSaveImage[0x24e0] __attribute__((aligned(4)));

} // namespace

std::byte *g_systemSaveData = systemSaveImage, *g_gameSaveData = gameSaveImage;

void *g_backupWorkBuffer = reinterpret_cast<void *>(0x0c016ba0);

std::int32_t g_retainedSaveSelection, g_scenarioSaveRestoreState;

namespace {

std::int32_t savedSceneActNumber;
shen_name savedSceneMapName;
std::int32_t savedSceneEntryPoint;

HLib::Task *saveTask;

float saveTextScale;

} // namespace

SaveTaskData *saveTaskData;

std::uint32_t currentSaveState;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_saveScreenAdmission = 0;
#endif

volatile std::int32_t g_backupResults[1];

volatile std::int32_t g_backupProgress;

std::int32_t g_backupProgressLimit;

namespace {

extern "C" std::int32_t buInit(std::int32_t drives, std::int32_t count, void *work, void (*callback)());

extern "C" std::int32_t buExit();

extern "C" std::int32_t buStat(std::int32_t port);

extern "C" std::int32_t buIsFormat(std::int32_t port);

extern "C" std::int32_t buIsExistFile(std::int32_t port, const char *filename);

extern "C" std::int32_t buGetFileSize(std::int32_t port, const char *filename);

extern "C" std::int32_t buGetDiskFree(std::int32_t port, std::int32_t mode);

extern "C" std::int32_t buLoadFileEx(std::int32_t port, const char *filename, void *data, std::int32_t firstBlock, std::int32_t blocks);

extern "C" void buSetCompleteCallback(std::int32_t (*callback)(std::int32_t, std::int32_t, std::int32_t));

extern "C" void buSetProgressCallback(std::int32_t (*callback)());

} // namespace

HLib::Task *g_hotplugTask;

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetSaveScreenLanguage() {
    return 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SaveTaskIdle(void *) {
}

} // namespace

#else

namespace {

// SM_Init_Enqueue installs the native empty update until a save UI is opened.
void SaveTaskIdle(void *) {
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void SaveTaskAcquireInput(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    if (save->initializationMode == 0)
        save->previousPadData = BindPadControlEntry(save->padData);
    save->preRenderCallback = SaveTaskCallback;
    ShowMessage("UISAVECONF", true); // @text EN: "Are you sure you want to save?" | JA: "セーブしてもよろしいですか？"
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void SaveTaskProcessInput(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    save->inputBuffer = save->initializationMode != 0 ? &g_inputBuffer[0] : (save->padData ? &save->padData->input : nullptr);
    if (save->inputBuffer)
        save->preRenderCallback(save);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SaveTaskDispatch(void *data) {
    saveTaskData->updateCallback(data);
}

} // namespace

#else

namespace {

// @verified - p
void SaveTaskDispatch(void *taskData) {
    saveTaskData->updateCallback(taskData);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SaveTaskInitCallback(void *) {
    saveTaskData->padData = saveTaskData->initializationMode == 0 ? AllocatePadControlEntry() : 0;
    HLib::SetCurrentTaskTo(SaveTaskDispatch);
}

} // namespace

#else

namespace {

void SaveTaskInitCallback(void *) {
    auto *const save = saveTaskData;
    save->padData = save->initializationMode != 0 ? nullptr : AllocatePadControlEntry();
    HLib::SetCurrentTaskTo(SaveTaskDispatch);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int8_t SaveTransitionValue(std::string_view row, std::size_t index) noexcept {
    return static_cast<std::int8_t>(static_cast<std::uint8_t>(row[index]));
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool SaveScreenYesNoChoice(bool draw) {
    if (saveTaskData->initializationMode == 0) {
        set_icon_select_flags_to(0x0400U);
        set_icon_select_flags_to(0x0200U);
        CheckHelpIcon(0x20U, 0, CheckHelpIconOp::FrameOverlay);
    }
    if (draw) {
        if (currentSaveState == 7) {
            nullsub();
            nullsub();
        } else {
            SetSaveScreenWarning(12, static_cast<std::uint8_t>(saveTaskData->field5d0) == 0);
        }
    }
    const std::uint16_t pressed = saveTaskData->inputBuffer ? saveTaskData->inputBuffer->pressedPriority : 0;
    const std::uint64_t field = static_cast<std::uint64_t>(saveTaskData->field5d0) & ~0xffULL;
    if (pressed & 0x4000U) {
        saveTaskData->field5d0 = static_cast<std::int64_t>(field | 1ULL);
        return false;
    }
    if (pressed & 0x8000U) {
        saveTaskData->field5d0 = static_cast<std::int64_t>(field);
        return false;
    }
    return (pressed & 0x0400U) != 0;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void ApplySaveScreenTransition(SaveTaskData &save) {
    const auto &tables = currentSaveState == 4 ? kLoadSaveTables : (currentSaveState == 6 ? kStaffSaveTables : kDefaultSaveTables);
    if (save.screenTable < 0 || static_cast<std::size_t>(save.screenTable) >= tables.size()) {
        return;
    }
    const std::string_view rows = tables[static_cast<std::size_t>(save.screenTable)].rows;
    if (save.screenRow < 0 || static_cast<std::size_t>(save.screenRow) >= rows.size() / kSaveTransitionRowSize) {
        return;
    }
    const std::string_view row = rows.substr(static_cast<std::size_t>(save.screenRow) * kSaveTransitionRowSize, kSaveTransitionRowSize);
    save.screenItem = static_cast<std::uint8_t>(row[0]);
    const std::uint8_t descriptorIndex = static_cast<std::uint8_t>(row[1]);
    if (descriptorIndex >= kSaveTransitionDescriptors.size())
        return;
    const SaveTransitionDescriptor descriptor = kSaveTransitionDescriptors[descriptorIndex];
    const std::int32_t transitionEnabled = (descriptor.blockSoftReset & 1U) ? 0 : 1;
    SetRootSaveTransitionEnabled(transitionEnabled);
    SetRootSoftResetEnabled(transitionEnabled);
    if (save.initializationMode == 0 && descriptor.inputKind < kSaveHelpFlags.size()) {
        const std::uint32_t helpFlags = kSaveHelpFlags[descriptor.inputKind];
        if ((helpFlags & 0x0800U) != 0)
            CheckHelpIcon(0x0800U, 0, CheckHelpIconOp::FrameOverlay);
        if ((helpFlags & 4U) != 0)
            set_icon_select_flags_to(0x0400U);
        if ((helpFlags & 2U) != 0)
            set_icon_select_flags_to(0x0200U);
        if ((helpFlags & 0x20U) != 0)
            CheckHelpIcon(0x20U, 0, CheckHelpIconOp::FrameOverlay);
    }
    if (GetDebugForceUIEnabled(UIElement::LoadSave)) {
        EnableElement(UIElement::LoadSave);
        ExecuteElementCallback(UIElement::LoadSave);
    } else {
        SelectSaveScreenWarning(&save, static_cast<std::uint8_t>(row[0]));
    }
    if (row[0] == 1) {
        LcdmInit(5);
    } else if (row[0] == 10) {
        LcdmInit(9);
    } else if (row[0] == 4 && save.frame <= 45) {
        LcdmInit(7);
        if (!saveTaskData->frame)
            ExecuteAicaCommand(0x900abU, 0, 0);
    } else {
        LcdmInit(0);
    }
    std::int32_t next = save.screenRow;
    const std::uint16_t pressed = save.inputBuffer ? save.inputBuffer->pressedPriority : 0;
    const auto rowValue = [&](std::size_t index) { return static_cast<std::int32_t>(SaveTransitionValue(row, index)); };
    const auto chooseIfPresent = [&](std::size_t index) {
        const std::int32_t candidate = rowValue(index);
        if (candidate != -1)
            next = candidate;
    };

    switch (descriptor.inputKind) {
    case 1:
        if ((pressed & 0x1000U) != 0) {
            --save.screenSelection;
            if (save.screenSelection >= 0) {
                save.selectedSlot = std::max(save.selectedSlot - 1, 0);
            } else if (save.screenPage < 1) {
                save.screenSelection = 0;
            } else {
                --save.screenPage;
                save.screenSelection = 0;
                save.selectedSlot = save.screenPage;
            }
        } else if ((pressed & 0x2000U) != 0) {
            ++save.screenSelection;
            if (save.screenSelection <= 2) {
                save.selectedSlot = std::min(save.selectedSlot + 1, 9);
            } else if (save.screenPage >= 7) {
                save.screenSelection = 2;
            } else {
                ++save.screenPage;
                save.screenSelection = 2;
                save.selectedSlot = save.screenPage + 2;
            }
        }
        if ((pressed & 0x0400U) != 0) {
            const auto &catalog = S2Save_RefreshSlotCatalog();
            next = rowValue(2);
            if (!catalog[static_cast<std::size_t>(save.selectedSlot)].occupied) {
                ++next;
            }
        } else if ((pressed & 0x0200U) != 0) {
            next = rowValue(3);
        }
        break;
    case 2:
    case 3:
        if (SaveScreenYesNoChoice(descriptor.inputKind == 2) && (descriptor.inputKind == 2 || !GetDebugForceUIEnabled(UIElement::LoadSave))) {
            const bool affirmative = static_cast<std::uint8_t>(save.field5d0) != 0;
            if (affirmative)
                save.field5d0 = static_cast<std::int64_t>(static_cast<std::uint64_t>(save.field5d0) & ~0xffULL);
            next = rowValue(affirmative ? 2 : 3);
        }
        break;
    case 4:
        if ((pressed & 0x0200U) != 0)
            next = rowValue(3);
        break;
    case 5:
        if ((pressed & 0x0400U) != 0)
            next = rowValue(2);
        break;
    case 6:
        if (save.frame < 45) {
            ++save.frame;
            return;
        }
        break;
    default:
        break;
    }

    const auto hasOccupiedSlot = [] {
        const auto &catalog = S2Save_RefreshSlotCatalog();
        return std::any_of(catalog.begin(), catalog.end(), [](const S2SaveSlotInfo &slot) { return slot.occupied; });
    };
    switch (descriptor.resultKind) {
    case 1: {
        const bool anyOccupied = hasOccupiedSlot();
        save.selectedSlot = 0;
        save.screenSelection = 0;
        save.screenPage = 0;
        chooseIfPresent(anyOccupied ? 9 : 10);
        break;
    }
    case 6:
        chooseIfPresent(hasOccupiedSlot() ? 9 : 10);
        break;
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
    case 10:
        chooseIfPresent(9);
        break;
    default:
        switch (saveScreenStorageStatus) {
        case -6:
            chooseIfPresent(8);
            break;
        case -5:
            chooseIfPresent(6);
            break;
        case -4:
            chooseIfPresent(10);
            break;
        case 0:
            chooseIfPresent(9);
            break;
        default:
            break;
        }
        break;
    }

    ++save.frame;
    if (next == save.screenRow)
        return;
    save.field5d0 = static_cast<std::int64_t>(static_cast<std::uint64_t>(save.field5d0) & ~1ULL);
    save.screenRow = next;
    save.frame = 0;
    if (next >= -40 && next <= -30) {
        save.screenTable = 6;
        save.screenRow = std::abs(next) - 30;
    } else if (next == -9 || (next >= -7 && next <= -2)) {
        save.screenRow = 0;
        save.screenTable = std::abs(next) - 2;
    }
    if (next == 98 || next == 99) {
        if (currentSaveState != 0)
            save.preRenderCallback = SaveScreenFadeOut;
        const std::uint64_t high = static_cast<std::uint64_t>(next != 99) << 32U;
        save.field5d0 = static_cast<std::int64_t>((static_cast<std::uint64_t>(save.field5d0) & 0x00000000ffffffffULL) | high);
        SetRootSaveTransitionEnabled(1);
        SetRootSoftResetEnabled(1);
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void SetDiskChange(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    currentSaveState = 0;
    save->updateCallback = SaveTaskIdle;
    save->preRenderCallback = SaveScreenInitialize;
    d3t::SetSceneClearColour(0xff000000U);
    SetNextSceneNumber(g_actNumber + 1U);
    ChangeSystemModule(SystemModule::DiskChange, 0);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SelectSaveScreenHelp(std::uint32_t flags) {
    if (saveTaskData->initializationMode != 0)
        return;
    if (!flags && GetUiFlag(0))
        SetHelpIconMenuFlags(0);
    else if (!GetUiFlag(0))
        AddHelpIconMenuFlags(static_cast<std::uint16_t>(flags));
    else if (GetUiFlag(0) != flags)
        SetHelpIconMenuFlags(static_cast<std::uint16_t>(flags));
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SetSaveScreenWarning(std::int32_t warning, std::int32_t value) {
    if (currentSaveState != 7)
        saveTaskData->fadeTarget = 118;
    else
        saveTaskData->fadeTarget = 190;
    if (saveTaskData->previousWarning != warning) {
        WRN_SetWarning(0, 0);
        saveTaskData->previousWarning = warning;
    } else {
        WRN_SetWarning(warning, value);
        switch (warning) {
        case 2:
        case 3:
        case 5:
        case 8:
        case 14:
            saveTaskData->fadeTarget += 15;
        }
    }
    saveTaskData->screenItem = warning;
}

} // namespace

// 24-byte gauge descriptor.
struct SaveProgressGauge {
    std::int16_t width, height, x, y;
    std::uint32_t fill, border, colour;
    float depth;
};

void DrawSaveProgressGauge(const SaveProgressGauge *gauge, float progress) {
    if (!(progress > 0.0f))
        return;
    shenSubmitQuadsUI(static_cast<float>(gauge->x), static_cast<float>(gauge->y), gauge->fill, gauge->border, gauge->width, gauge->height, gauge->depth);
    LegacyColourRect rect;
    rect.x = static_cast<float>(gauge->x - gauge->width / 2 + 2);
    rect.y = static_cast<float>(gauge->y - gauge->height / 2 + 2);
    rect.scaleX = 1.0f;
    rect.scaleY = 1.0f;
    rect.width = static_cast<std::int16_t>(static_cast<float>(gauge->width - 4) * progress);
    rect.height = gauge->height - 4;
    rect.rotation = 0;
    rect.depth = gauge->depth + 0.1f;
    rect.alignment = 0;
    rect.colour = gauge->colour;
    DrawLegacyColourRect(rect);
}

#else

// @verified - p
SaveTaskData *SetSaveScreenWarning(std::int32_t warning, std::int32_t value) {
    saveTaskData->fadeTarget = currentSaveState == 7 ? 190 : 118;
    if (saveTaskData->previousWarning == warning) {
        WRN_SetWarning(warning, value);
    } else {
        WRN_SetWarning(0, 0);
        saveTaskData->previousWarning = warning;
    }
    saveTaskData->screenItem = warning;
    return saveTaskData;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

SaveTaskData *SelectSaveScreenWarning(SaveTaskData *save, std::int32_t screen) {
    std::int32_t value = 0;
    switch (screen) {
    case 0:
    case 1:
    case 4:
    case 5:
    case 6:
    case 10:
    case 11:
    case 13:
    case 15:
    case 18:
    case 19:
        break;
    case 2:
    case 3:
    case 12:
    case 14:
    case 16:
    case 17:
    case 20:
        value = static_cast<std::uint8_t>(save->field5d0) == 0;
        break;
    case 7:
    case 9:
    case 22:
        value = save->screenOption;
        break;
    case 8:
        value = 4;
        break;
    case 21:
        value = g_actNumber;
        break;
    case 23:
        DrawLegacySaveSlotList(*save);
        return save;
    default:
        return nullptr;
    }
    return SetSaveScreenWarning(SAVE_SCREEN_WARNING_IDS[static_cast<std::size_t>(screen)], value);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SetSaveScreenSystemProcessing(std::int32_t enabled) {
    SetBackupProcessing(enabled);
    SetSoftResetProcessing(enabled);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

const char *saveSpriteDirectory = "sprite";
char saveDiscChangeSprite[] = "DISCCHG?.SPR";

// @verified - d
bool PrepareResumeData(std::int32_t mode) {
    saveTaskData->choice = 0;
    SaveTaskData *save = saveTaskData;
    save->glyphTables[0] = reinterpret_cast<const SaveGlyphRecord *>(save->resumeData);
    save->glyphTables[1] = reinterpret_cast<const SaveGlyphRecord *>(save->resumeData + 0x1b0);
    save->glyphTables[2] = reinterpret_cast<const SaveGlyphRecord *>(save->resumeData + 0x32a);
    if (mode != 1)
        return false;
    if (!*reinterpret_cast<const std::uint16_t *>(save->resumeData))
        shenLoadFile("misc", "RESUME.DAT", save->resumeData, 0x575);
    if (saveTaskData->textures[1] < 0 || saveTaskData->textures[2] < 0)
        return false;
    for (std::int32_t index = 0; index < 8; ++index)
        saveTaskData->discTextures[index] = -1;
    if (currentSaveState == 4) {
        saveDiscChangeSprite[7] = static_cast<char>(GetSaveScreenLanguage() + '0');
        void *const resource = shenLoadFile(saveSpriteDirectory, saveDiscChangeSprite, SHEN_NAME(SAVE));
        if (resource) {
            ReadTitleSpriteTextures(resource, shenOpenFile(saveSpriteDirectory, saveDiscChangeSprite), saveTaskData->discTextures);
            HLib::FreeTaskData(resource);
        }
    }
    return true;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SaveScreenInitialize(void *data) {
    SaveTaskData *const save = static_cast<SaveTaskData *>(data);
    saveTextScale = 1.0f;
    save->retained5cc = 0;
    save->fadeTarget = 0;
    save->fadeCurrent = 0.0f;
    save->screenItem = 0;
    save->selectedSlot = 0;
    save->previousHelpFlags = GetUiFlag(0);
    save->screenColour = save->previousWarning = 0;
    if (!save->initializationMode) {
        SetLoadingBusy();
        save->previousPadData = BindPadControlEntry(save->padData);
    }
    switch (currentSaveState) {
    case 3:
    case 7:
    case 8:
        return;
    case 4:
        if (!PrepareResumeData(1)) {
            save->preRenderCallback = SaveScreenComplete;
            return;
        }
        SelectSaveScreenHelp(4);
        save->screenRow = save->screenColour = save->previousWarning = 0;
        save->preRenderCallback = SaveScreenRun;
        return;
    case 6:
        if (static_cast<std::int32_t>(g_completedGameCount) <= 254)
            ++g_completedGameCount;
    default:
        if (!PrepareResumeData(1)) {
            save->preRenderCallback = SaveScreenComplete;
            return;
        }
        save->screenRow = save->screenColour = save->previousWarning = 0;
        save->preRenderCallback = SaveScreenRun;
        SelectSaveScreenHelp(38);
        return;
    }
}

} // namespace

#else

namespace {

void SaveScreenInitialize(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    save->field5d8 = 0;
    save->previousWarning = 0;
    save->screenItem = 0;
    save->fadeTarget = 0;
    save->fadeCurrent = 0.0f;
    legacySaveGlyphScale = 1.0f;
    save->field610 = GetUiFlag(0);
    save->selectedSlot = 0;
    save->screenRow = 0;
    save->screenTable = 0;
    save->screenSelection = 0;
    save->screenPage = 0;
    save->screenOption = 0;
    legacySaveList.locationIds.fill(-1);
    legacySaveList.areaIds.fill(-1);
    if (save->initializationMode == 0 && save->padData && !save->previousPadData) {
        nullsub(&legacySaveList.textOffset);
        save->previousPadData = BindPadControlEntry(save->padData);
    }

    if (currentSaveState == 3)
        return;
    if (currentSaveState == 4) {
        save->preRenderCallback = SaveScreenComplete;
        return;
    }
    if (currentSaveState == 7 || currentSaveState == 8)
        return;
    if (currentSaveState == 6 && completedGameCount < 255U)
        ++completedGameCount;

    if (GetDebugForceUIEnabled(UIElement::LoadSave)) {
        ShowElementByIndex(UIElement::GameBackground);
        SetSaveFrontendMode(1);
        ShowElementByIndex(UIElement::LoadSave);
    }
    save->colour = 0;
    save->preRenderCallback = SaveScreenRun;
    if (saveTaskData->initializationMode == 0) {
        set_icon_select_flags_to(0x0400U);
        set_icon_select_flags_to(0x0200U);
        CheckHelpIcon(0x20U, 0, CheckHelpIconOp::FrameOverlay);
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SaveScreenComplete(void *data) {
    SaveTaskData *const save = static_cast<SaveTaskData *>(data);
    LcdmInit(0);
    if (!save->initializationMode) {
        FinishLoading();
        BindPadControlEntry(save->previousPadData);
    }
    if (currentSaveState == 2 || currentSaveState == 5)
        SetSleepActionIcon(0);
    if (currentSaveState == 2)
        SelectSaveScreenHelp(3);
    else if (currentSaveState != 7)
        SelectSaveScreenHelp(save->previousHelpFlags);
    if (currentSaveState != 7) {
        save->updateCallback = SaveTaskIdle;
        save->preRenderCallback = SaveScreenInitialize;
        currentSaveState = 0;
    } else {
        save->preRenderCallback = SaveScreenDiskFade;
        save->screenColour = 118;
        if (save->returnToGame) {
            save->updateCallback = SaveTaskIdle;
            save->preRenderCallback = SaveScreenInitialize;
            currentSaveState = 0;
        } else {
            SetHelpIconMenuFlags(0);
        }
    }
}

} // namespace

#else

namespace {

void SaveScreenComplete(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    LcdmInit(0);
    if (save->initializationMode == 0 && currentSaveState != 4)
        FinishLoading();
    if (save->initializationMode == 0 && save->previousPadData) {
        BindPadControlEntry(save->previousPadData);
        save->previousPadData = nullptr;
    }
    if (currentSaveState == 2 || currentSaveState == 5)
        SetSleepActionIcon(0);
    if (saveScreenPrompt != SaveScreenPrompt::None) {
        DismissMessage();
        saveScreenPrompt = SaveScreenPrompt::None;
    }
    saveScreenStorageStatus = 0;
    DestroyElementByIndex(UIElement::LoadSave);
    HLib::FreeTaskData(legacySaveList.locationPack);
    HLib::FreeTaskData(legacySaveList.areaPack);
    if (currentSaveState == 4) {
        save->colour = 118;
        save->preRenderCallback = SaveScreenDiskFade;
        return;
    }
    currentSaveState = 0;
    save->updateCallback = SaveTaskIdle;
    save->preRenderCallback = SaveScreenInitialize;
    save->fadeTarget = 0;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

#define INIT_SAVE_SPRITE_PACKET(packet)                                                                                                                        \
    do {                                                                                                                                                       \
        (packet).quad.flags = 0;                                                                                                                               \
        (packet).colour = UINT32_MAX;                                                                                                                          \
        (packet).secondaryColour = 0;                                                                                                                          \
        (packet).uv[0] = (packet).uv[1] = 0.0f;                                                                                                                \
        (packet).uv[2] = (packet).uv[3] = 1.0f;                                                                                                                \
        (packet).reservedTail = 0;                                                                                                                             \
    } while (0)

// @verified - d
void SaveScreenDiskFade(void *) {
    LegacySpritePacket packet;
    INIT_SAVE_SPRITE_PACKET(packet);
    packet.colour = static_cast<std::uint32_t>(saveTaskData->screenColour) << 24U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480);
    packet.quad.flags |= 0x400U;
    DrawLegacyColourPacket(packet, 16.0f);
    if (saveTaskData->screenColour != 255) {
        saveTaskData->screenColour += 4;
        if (saveTaskData->screenColour > 254)
            saveTaskData->screenColour = 255;
    } else {
        saveTaskData->preRenderCallback = SaveScreenDiskComplete;
    }
}

} // namespace

#else

namespace {

void SaveScreenDiskFade(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    SetScreenFilmColour(save->colour << 24U);
    if (save->colour == 255) {
        if (S2Save_Capture(activeSaveImage, static_cast<std::uint8_t>(currentSaveState)) == S2SaveResult::Success) {
            activeSaveImageInitialized = true;
        }
        save->preRenderCallback = SetDiskChange;
        return;
    }
    save->colour = std::min(save->colour + 4U, 255U);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void SaveScreenDiskComplete(void *) {
    std::int32_t initialState = 0;
    LegacySpritePacket packet;
    saveTaskData->updateCallback = SaveTaskIdle;
    saveTaskData->preRenderCallback = SaveScreenInitialize;
    currentSaveState = initialState;
    INIT_SAVE_SPRITE_PACKET(packet);
    packet.colour = 0xff000000U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480);
    packet.quad.flags |= 0x400U;
    DrawLegacyColourPacket(packet, 16.0f);
    dc::sdk::njSetBackColor(0xff000000U, 0xff000000U, 0xff000000U);
    if (!saveTaskData->returnToGame)
        ChangeSystemModule(SystemModule_Title, 0);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void SaveScreenFadeOut(void *data) {
    SaveTaskData *const save = static_cast<SaveTaskData *>(data);
    RetireLoadingDraw();
    SetSaveScreenWarning(0, 0);
    const std::int32_t frame = ++save->frame;
    if (!(save->fadeCurrent > 15.0f) || frame > 44) {
        save->preRenderCallback = SaveScreenComplete;
        save->frame = 0;
    }
    save->fadeTarget = 0;
}

} // namespace

#else

namespace {

void SaveScreenFadeOut(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    saveTaskData->fadeTarget = currentSaveState == 7 ? 190 : 118;
    WRN_SetWarning(0, 0);
    saveTaskData->previousWarning = 0;
    saveTaskData->screenItem = 0;
    ++save->frame;
    save->fadeTarget = 0;
    if (save->fadeCurrent <= 15.0f || save->frame >= 45) {
        save->frame = 0;
        save->preRenderCallback = SaveScreenComplete;
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
std::int32_t CheckSaveFileSpace(const char *filename, std::int32_t bytes) {
    std::int32_t freeBlocks = GetBackupFreeBlocksPort0();
    if (freeBlocks < 0)
        return -6;
    const std::int32_t result = CheckBackupFileExistsPort0(filename);
    if (!result) {
        const std::int32_t previousBlocks = buGetFileSize(0, filename);
        if (previousBlocks < 0)
            return -6;
        freeBlocks += previousBlocks;
    } else if (result != -4) {
        return result;
    }
    return freeBlocks >= (bytes + 511) / 512 ? 0 : -5;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void BuildSaveBackupImage(std::byte *image, std::int32_t bytes) {
    static const std::uint8_t saveVmuShortDescription[18] = {83, 72, 69, 78, 77, 85, 69, 49, 32, 214, 186, 189, 182, 0, 0, 0, 0, 0};
    static const std::uint8_t saveVmuLongDescription[34] = {131, 86, 131, 70, 131, 147, 131, 128, 129, 91, 129, 64, 136, 234, 143, 205, 129, 64, 137, 161, 144, 123,
        137, 234, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static const std::uint8_t saveVmuApplicationId[16] = {24, 9, 83, 65, 7, 84, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    SaveBackupImageDescriptor descriptor = {};
    std::memcpy(descriptor.shortDescription, saveVmuShortDescription, sizeof(saveVmuShortDescription));
    std::memcpy(descriptor.longDescription, saveVmuLongDescription, sizeof(saveVmuLongDescription));
    std::memcpy(descriptor.applicationId, saveVmuApplicationId, sizeof(saveVmuApplicationId));
    descriptor.palette = saveVmuPalette;
    descriptor.icons = saveVmuIcons;
    descriptor.iconCount = 3;
    descriptor.iconSpeed = 10;
    descriptor.eyecatch = 0;
    descriptor.eyecatchType = 0;
    descriptor.data = image + 0x680;
    descriptor.bytes = bytes - 0x680;
    buMakeBackupFileImage(image, &descriptor);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t CheckSaveSectionSpace(std::int32_t slot) {
    if (slot == -1)
        return 0;
    if (slot == 3)
        return CheckSaveFileSpace(kGameSaveFilenames[3], 0x6b4);
    return CheckSaveFileSpace(kGameSaveFilenames[slot], 0x24e0);
}

static std::int32_t WriteSystemSaveFile() {
    const char *const filename = kGameSaveFilenames[3];
    std::byte *const image = g_systemSaveData;
    *reinterpret_cast<std::uint32_t *>(image + 0x688) = g_nativePlayerStats.gameConfigurationFlags;
    *reinterpret_cast<std::uint32_t *>(image + 0x68c) = g_completedGameCount;
    std::int32_t result = CheckSaveFileSpace(filename, 0x6b4);
    if (!result)
        result = WaitForBackupOperationPort0();
    if (result)
        return result;
    BuildSaveBackupImage(g_systemSaveData, 0x6b4);
    std::uint32_t date[3];
    syRtcGetDate(date);
    g_backupResults[0] = -2;
    if (buSaveFile(0, filename, g_systemSaveData, 4, date, 0x80000000U) == 0)
        result = 0;
    else
        result = -6;
    return result;
}

static std::int32_t WriteGameSaveFile(std::int32_t slot) {
    std::int32_t result = CheckSaveFileSpace(kGameSaveFilenames[slot], 0x24e0);
    if (!result)
        result = WaitForBackupOperationPort0();
    if (result)
        return result;
    BuildSaveBackupImage(g_gameSaveData, 0x24e0);
    std::uint32_t date[3];
    syRtcGetDate(date);
    g_backupResults[0] = -2;
    if (buSaveFile(0, kGameSaveFilenames[slot], g_gameSaveData, 19, date, 0x80000000U) == 0)
        result = 0;
    else
        result = -6;
    return result;
}

namespace {

std::int32_t WriteSaveDataSection(std::int32_t slot) {
    if (static_cast<std::uint32_t>(slot) < 3U)
        return WriteGameSaveFile(slot);
    if (slot == 3)
        return WriteSystemSaveFile();
    return -6;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void CacheSaveMetadata(std::int32_t slot, std::int32_t initializationMode) {
    if (slot != -1) {
        if (slot >= -1) {
            if (slot < 3) {
                std::byte *const save = g_gameSaveData;
                std::byte *const payload = save + 0x6e0;
                std::byte *const metadata = save + 0x6a0;
                CaptureSavedGamePayload(payload, metadata, initializationMode);
                BackupSlotMetadata *slots = g_cachedBackupSlots;
                slots[slot] = *reinterpret_cast<BackupSlotMetadata *>(metadata);
            }
        }
    } else {
        std::size_t size = sizeof(BackupSlotMetadata);
        std::memset(g_cachedBackupSlots + 3, 0, size);
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void DrawSaveGlyph(std::int32_t x, std::int32_t y, const SaveGlyphRecord *glyph, std::int32_t font) {
    LegacySpritePacket packet;
    float u = glyph->x, v = glyph->y, width = glyph->width, height = glyph->height;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.uv[0] = u * 0.00390625f;
    packet.uv[1] = 1.0f - (v + height) * 0.00390625f;
    packet.uv[2] = (u + width) * 0.00390625f;
    packet.uv[3] = 1.0f - v * 0.00390625f;
    SetScreenQuadBounds(packet.quad, x, y, static_cast<std::int32_t>(x + width * saveTextScale), static_cast<std::int32_t>(y + height * saveTextScale));
    packet.quad.flags |= 0x400U;
    DrawLegacySpritePacket(packet, saveTaskData->textures[font], 8.0f);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetSaveScreenAdmission() {
    return g_saveScreenAdmission;
}
#endif

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetSaveScreenAdmission(std::int32_t enabled) {
    g_saveScreenAdmission = enabled;
}
#endif

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void SM_Destroy() {
    HLib::DestroyTaskInternal(saveTask);
}

// @verified - d
std::uint32_t StartShowScreenOfId(std::uint32_t screenId) {
    if (currentSaveState)
        return 0;
    currentSaveState = screenId;
    saveTaskData->updateCallback = SaveScreenUpdate;
    return screenId;
}

#else

std::int64_t StartShowScreenOfId(std::uint32_t screenId) noexcept {
    if (currentSaveState != 0)
        return 0;
    saveScreenPrompt = SaveScreenPrompt::None;
    saveScreenPromptSlot = 0;
    saveScreenStorageStatus = 0;
    currentSaveState = screenId;
    saveTaskData->updateCallback = SaveScreenUpdate;
    return static_cast<std::int64_t>(screenId);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t IsSaveAllowed() {
    return currentSaveState;
}

#else

std::uint32_t IsSaveAllowed() noexcept {
    return currentSaveState;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void DrawCenteredSaveText(std::int32_t y, const char *text, std::int32_t font) {
    const std::int32_t width = MeasureSaveText(reinterpret_cast<const std::uint8_t *>(text), font);
    DrawSaveText(320 - (width >> 1), y, text, font);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void DrawSaveScreenMessage(std::int32_t message) {
    std::int32_t japanese = GetSaveScreenLanguage() == 0;
    LegacySpritePacket packet;
    if (message > 5) {
        INIT_SAVE_SPRITE_PACKET(packet);
        packet.quad.flags |= 0x400U;
        SetScreenQuadBounds(packet.quad, 64, 323, 576, 451);
        DrawLegacySpritePacket(packet, saveTaskData->textures[0], 7.0f);
        SetScreenQuadBounds(packet.quad, 64, 371, 576, 403);
        DrawLegacySpritePacket(packet, saveTaskData->textures[message == 6 ? japanese ? 5 : 6 : 7], 7.0f);
    } else if (message < 3) {
        shenSubmitQuadsUI(320.0f, 330.0f, 0x3b00008cU, 0xff4040c0U, 623, 100, 6.9f);
        saveTextScale = 1.2f;
        switch (message) {
        case 0:
            DrawCenteredSaveText(286, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x336 + (japanese ? 0 : 0xc2)), 0);
            DrawCenteredSaveText(317, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x336 + (japanese ? 25 : 0xe3)), 0);
            DrawCenteredSaveText(345, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x336 + (japanese ? 60 : 0x108)), 0);
            break;
        case 1:
            DrawCenteredSaveText(286, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x39d), 0);
            DrawCenteredSaveText(317, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x336 + (japanese ? 0x7a : 0x13d)), 0);
            DrawCenteredSaveText(345, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x336 + (japanese ? 0x9b : 0x160)), 0);
            break;
        case 2:
            DrawCenteredSaveText(293, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x3f2), 0);
            DrawCenteredSaveText(325, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x336 + (japanese ? 0xbf : 0x187)), 0);
            break;
        }
        saveTextScale = 1.0f;
    } else {
        shenSubmitQuadsUI(320.0f, 380.0f, 0x3b00008cU, 0xff4040c0U, 422, 76, 6.9f);
        const std::byte *text;
        if (japanese) {
            if (message == 3) {
                DrawCenteredSaveText(358, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x560), 1);
                DrawCenteredSaveText(384, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x4c0), 1);
                return;
            }
            text = saveTaskData->resumeData + (message == 4 ? 0x4d3 : 0x4f0);
        } else {
            if (message == 3) {
                DrawCenteredSaveText(358, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x560), 1);
                DrawCenteredSaveText(384, reinterpret_cast<const char *>(saveTaskData->resumeData + 0x50d), 1);
                return;
            }
            text = saveTaskData->resumeData + (message == 4 ? 0x524 : 0x541);
        }
        DrawCenteredSaveText(369, reinterpret_cast<const char *>(text), 1);
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

BackupSlotMetadata *GetCachedBackupSlot(std::int32_t slot);

namespace {

// @verified - d
void DrawSaveSlotList(SaveTaskData *save) {
    char text[80];
    std::int32_t textX = 220;
    SetSaveScreenWarning(0, 0);
    DrawSaveScreenMessage(3);
    std::int32_t y = 48;
    for (std::int32_t slot = 0; slot < 3; ++slot) {
        const std::uint8_t *const dates = reinterpret_cast<const std::uint8_t *>(GetCachedBackupSlot(slot));
        std::uint32_t colour, edge;
        if (slot != save->selectedSlot) {
            colour = 0x3b00008cU;
            edge = 0xff4040c0U;
        } else {
            colour = 0x3b8c0000U;
            edge = 0xffc04040U;
        }
        shenSubmitQuadsUI(320.0f, static_cast<float>(y + 43), colour, edge, 480, 86, 6.9f);
        std::sprintf(text, "bfb%1d", slot + 1);
        DrawSaveText(107, static_cast<std::int32_t>(static_cast<float>(y) + 3.1999969482421875f), "IC", 2);
        DrawSaveText(94, static_cast<std::int32_t>(static_cast<float>(y) + 21.1999969482421875f), text, 1);
        if (*reinterpret_cast<const std::uint16_t *>(dates + 0x10)) {
            std::sprintf(text, "DK%2d", *reinterpret_cast<const std::int16_t *>(dates + 0x12));
            DrawSaveText(textX, static_cast<std::int32_t>(static_cast<float>(y) + 10.1999969482421875f - 2.0f), text, 1);
            std::sprintf(text, "GA%s   1 9%2d%2d /%2d%2d /%2d%2d   %c%2d :%2d%2d", !GetSaveScreenLanguage() ? "D0" : "  D1", reinterpret_cast<const std::uint16_t *>(dates)[4] / 10, reinterpret_cast<const std::uint16_t *>(dates)[4] % 10, dates[10] / 10,
                dates[10] % 10, dates[11] / 10, dates[11] % 10, dates[12] / 10 ? dates[12] / 10 + '0' : 'a', dates[12] % 10, dates[13] / 10, dates[13] % 10);
            DrawSaveText(textX, static_cast<std::int32_t>(static_cast<float>(y + 22) + 10.1999969482421875f), text, 1);
            std::sprintf(text, "SA%s  %2d%2d%2d%2d /%2d%2d /%2d%2d   %c%2d :%2d%2d", !GetSaveScreenLanguage() ? "D0" : "  D1", reinterpret_cast<const std::uint16_t *>(dates)[0] / 1000,
                (reinterpret_cast<const std::uint16_t *>(dates)[0] - reinterpret_cast<const std::uint16_t *>(dates)[0] / 1000 * 1000) / 100,
                (reinterpret_cast<const std::uint16_t *>(dates)[0] - reinterpret_cast<const std::uint16_t *>(dates)[0] / 100 * 100) / 10, reinterpret_cast<const std::uint16_t *>(dates)[0] % 10, dates[2] / 10, dates[2] % 10, dates[3] / 10, dates[3] % 10, dates[4] / 10 ? dates[4] / 10 + '0' : 'a', dates[4] % 10,
                dates[5] / 10, dates[5] % 10);
            DrawSaveText(textX, static_cast<std::int32_t>(static_cast<float>(y + 44) + 10.1999969482421875f + 2.0f), text, 1);
            if (dates[0x24] == 6) {
                const std::int32_t stateY = static_cast<std::int32_t>(static_cast<float>(y) + 10.1999969482421875f);
                DrawSaveText(418, stateY, "CL", 1);
                DrawSaveText(474, stateY, "DT", 1);
            } else if (dates[0x24] == 4) {
                const std::int32_t stateY = static_cast<std::int32_t>(static_cast<float>(y) + 10.1999969482421875f);
                DrawSaveText(413, stateY, "ST", 1);
                DrawSaveText(474, stateY, "DT", 1);
            }
        } else {
            DrawSaveText(textX + 32, static_cast<std::int32_t>(static_cast<float>(y) + 21.1999969482421875f), "EY", 1);
        }
        y = static_cast<std::int32_t>(static_cast<float>(y) + 94.399993896484375f);
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

const float saveDiscUvStarts[5] = {0, 0, 0.328125f, 0.65625f, 0.984375f};

void DrawSaveDiscChange(SaveTaskData *save) {
    static std::int32_t discChangeFirstLabelX[2] = {258, 354};
    static std::int32_t discChangeFirstLabelY[2] = {89, 71};
    static std::int32_t discChangeSecondLabelX[2] = {349, 353};
    static std::int32_t discChangeSecondLabelY[2] = {327, 343};
    const float *uv = saveDiscUvStarts + g_loadedActNumber;
    const std::int32_t language = GetSaveScreenLanguage();
    SetSaveScreenWarning(0, 0);
    LegacySpritePacket packet;
    INIT_SAVE_SPRITE_PACKET(packet);
    TransformScreenSpace(packet.quad, HorizontalStretch, 320, 240, 512, 512, 1.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, save->discTextures[0], 7.95f);
    INIT_SAVE_SPRITE_PACKET(packet);
    packet.uv[0] = *uv++;
    packet.uv[1] = 0.0f;
    packet.uv[2] = *uv;
    packet.uv[3] = 1.0f;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), discChangeFirstLabelX[language], discChangeFirstLabelY[language], 21, 32, 1.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, save->discTextures[2], 21.0f);
    packet.uv[0] = *uv++;
    packet.uv[1] = 0.0f;
    packet.uv[2] = *uv;
    packet.uv[3] = 1.0f;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), discChangeSecondLabelX[language], discChangeSecondLabelY[language], 21, 32, 1.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, save->discTextures[2], 21.0f);
}

} // namespace

static void DrawSaveDiscChangeNoLanguage(SaveTaskData *save) {
    const float *uv = saveDiscUvStarts + 1 + g_loadedActNumber;
    SetSaveScreenWarning(0, 0);
    LegacySpritePacket packet;
    INIT_SAVE_SPRITE_PACKET(packet);
    TransformScreenSpace(packet.quad, HorizontalStretch, 320, 240, 512, 512, 1.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, save->discTextures[1], 7.95f);
    INIT_SAVE_SPRITE_PACKET(packet);
    packet.uv[0] = *uv++;
    packet.uv[1] = 0.0f;
    packet.uv[2] = *uv;
    packet.uv[3] = 1.0f;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), 218, 157, 21, 32, 1.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, save->discTextures[2], 21.0f);
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), 231, 293, 21, 32, 1.0f, 1.0f, 0);
    DrawLegacySpritePacket(packet, save->discTextures[2], 21.0f);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void DrawSaveScreen(SaveTaskData *save, std::int32_t screen) {
    std::int32_t value = 0;
    switch (static_cast<std::uint32_t>(screen)) {
    case 2:
    case 3:
    case 10:
    case 12:
    case 14:
    case 15:
    case 18:
        value = !save->choice;
    case 0:
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 11:
    case 13:
    case 16:
    case 17:
        SetSaveScreenWarning(saveScreenWarnings[screen], value);
        break;
    case 21:
        DrawSaveSlotList(save);
        break;
    case 19:
        DrawSaveDiscChange(save);
        break;
    case 20:
        DrawSaveDiscChangeNoLanguage(save);
        break;
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

std::int32_t saveChoiceTop = 241;
std::int32_t saveChoiceGap = 2;
std::int32_t saveChoiceOffsetY = -195;
std::int32_t saveChoiceOffsetX = -193;

// @verified - d
bool ProcessSaveChoice(std::int32_t draw) {
    SelectSaveScreenHelp(38);
    if (draw) {
        if (currentSaveState != 7) {
            SetSaveScreenWarning(12, !saveTaskData->choice);
        } else {
            LegacySpritePacket packet;
            INIT_SAVE_SPRITE_PACKET(packet);
            packet.quad.flags |= 0x400U;
            SetScreenQuadBounds(packet.quad, static_cast<std::int32_t>(320 - saveChoiceGap - 89.6f + saveChoiceOffsetX), saveChoiceTop + saveChoiceOffsetY,
                                320 - saveChoiceGap + saveChoiceOffsetX, static_cast<std::int32_t>(saveChoiceTop + 44.8f + saveChoiceOffsetY));
            DrawLegacySpritePacket(packet, saveTaskData->textures[saveTaskData->choice ? 2 : 1], 8.0f);
            SetScreenQuadBounds(packet.quad, 320 + saveChoiceGap + saveChoiceOffsetX, saveChoiceTop + saveChoiceOffsetY,
                                static_cast<std::int32_t>(409.6f + saveChoiceGap + saveChoiceOffsetX),
                                static_cast<std::int32_t>(saveChoiceTop + 44.8f + saveChoiceOffsetY));
            DrawLegacySpritePacket(packet, saveTaskData->textures[!saveTaskData->choice ? 4 : 3], 8.0f);
        }
    }
    if (saveTaskData->inputBuffer->pressedPriority & 0x4000U) {
        if (!saveTaskData->choice)
            ExecuteAicaCommand(0x500ab, 0, 0);
        saveTaskData->choice = 1;
        return false;
    }
    if (saveTaskData->inputBuffer->pressedPriority & 0x8000U) {
        if (saveTaskData->choice)
            ExecuteAicaCommand(0x500ab, 0, 0);
        saveTaskData->choice = 0;
        return false;
    }
    if (saveTaskData->inputBuffer->pressedPriority & 0x400U) {
        if (saveTaskData->choice)
            ExecuteAicaCommand(0x500ab, 0, 0);
        else
            ExecuteAicaCommand(0x100ab, 0, 0);
        return true;
    }
    if (saveTaskData->inputBuffer->pressedPriority & 0x200U) {
        saveTaskData->choice = 0;
        ExecuteAicaCommand(0x100ab, 0, 0);
        return true;
    }
    return false;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
std::int32_t CheckAllSaveFilesPresent() {
    std::int32_t result = GetBackupConnectionStatusPort0();
    if (!result) {
        result = CheckSaveFileExistsByIndex(3);
        if (result) {
            for (std::int32_t slot = 0; slot < 3; ++slot) {
                result = CheckSaveFileExistsByIndex(slot);
                if (!result)
                    return 0;
            }
            if (result)
                result = -4;
        }
    }
    return result;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
std::int32_t CheckAllGameSaveFilesPresent() {
    std::int32_t result = GetBackupConnectionStatusPort0();
    if (!result) {
        for (std::int32_t slot = 0; slot < 3; ++slot)
            if (!CheckSaveFileExistsByIndex(slot))
                return 0;
        result = -4;
    }
    return result;
}

} // namespace

std::int32_t CheckSystemSaveFilePresent() {
    std::int32_t result = GetBackupConnectionStatusPort0();
    if (!result)
        result = CheckSaveFileExistsByIndex(3);
    return result;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static const SaveScreenRow *const saveScreenRowTables[6] = {0, 0, saveDefaultRows, saveLoadRows, saveStaffRows, 0};

namespace {

// @verified - d
void SaveScreenRun(void *data) {
    SaveTaskData *const save = static_cast<SaveTaskData *>(data);
    SaveTaskData *const screen = save;
    std::int32_t next = save->screenRow;
    const SaveScreenRow *row;
    switch (currentSaveState) {
    case 3:
    case 7:
    case 8:
    case 9:
        return;
    case 4:
        row = saveScreenRowTables[3];
        break;
    case 6:
        row = saveScreenRowTables[4];
        break;
    default:
        row = saveScreenRowTables[2];
        break;
    }
    row += save->screenRow;
    SetSaveScreenSystemProcessing(row->systemEnabled);
    switch (row->lcd) {
    case 0:
        LcdmInit(0);
        break;
    case 1:
        LcdmInit(GetSaveScreenLanguage() ? 2 : 3);
        break;
    case 2:
        LcdmInit(GetSaveScreenLanguage() ? 4 : 5);
        break;
    case 3:
        if (save->frame <= 45) {
            LcdmInit(GetSaveScreenLanguage() ? 6 : 7);
            if (!save->frame)
                ExecuteAicaCommand(0x900ab, 0, 0);
        } else {
            LcdmInit(0);
        }
        break;
    case 4:
        LcdmInit(GetSaveScreenLanguage() ? 8 : 9);
        break;
    case 5:
        LcdmInit(GetSaveScreenLanguage() ? 10 : 11);
        break;
    }
    SelectSaveScreenHelp(saveScreenHelpFlags[row->input]);
    DrawSaveScreen(screen, row->display);
    switch (row->input) {
    case 0:
        break;
    case 2:
        if (ProcessSaveChoice(1)) {
            if (save->choice) {
                save->choice = 0;
                next = row->accepted;
            } else {
                next = row->cancelled;
            }
        }
        break;
    case 3:
        if (ProcessSaveChoice(0)) {
            if (save->choice) {
                save->choice = 0;
                next = row->accepted;
            } else {
                next = row->cancelled;
            }
        }
        break;
    case 5:
        if (save->inputBuffer->pressedPriority & 0x400) {
            ExecuteAicaCommand(0x500ab, 0, 0);
            next = row->accepted;
        }
        break;
    case 1:
        if (save->inputBuffer->pressedPriority & 0x1000) {
            ExecuteAicaCommand(0x200ab, 0, 0);
            if (--save->selectedSlot < 0)
                save->selectedSlot = 2;
        } else if (save->inputBuffer->pressedPriority & 0x2000) {
            ExecuteAicaCommand(0x200ab, 0, 0);
            if (++save->selectedSlot > 2)
                save->selectedSlot = 0;
        }
        if (save->inputBuffer->pressedPriority & 0x400) {
            BackupSlotMetadata *const slot = GetCachedBackupSlot(save->selectedSlot);
            ExecuteAicaCommand(0x500ab, 0, 0);
            if (slot->saveKind)
                next = row->accepted;
            else
                next = row->accepted + 1;
        } else if (save->inputBuffer->pressedPriority & 0x200) {
            ExecuteAicaCommand(0x100ab, 0, 0);
            next = row->cancelled;
        }
        break;
    case 4:
        if (save->inputBuffer->pressedPriority & 0x200) {
            ExecuteAicaCommand(0x100ab, 0, 0);
            next = row->cancelled;
        }
        break;
    case 7:
        DrawLoadingModes();
        DrawLoadingModes();
        break;
    case 6:
        if (save->frame <= 44) {
            ++save->frame;
            return;
        }
        break;
    }
    std::int32_t result;
    switch (row->operation) {
    case 1:
        ReloadLocalSaveSlots();
        result = CheckAllSaveFilesPresent();
        break;
    case 2:
        result = GetBackupConnectionStatusPort0();
        if (!result) {
            if (GetBackupFreeBlocksPort0() > 22)
                save->selectedSlot = 0;
            else
                result = -5;
        }
        break;
    case 5:
        result = CheckSaveFileExistsByIndex(3);
        break;
    case 8:
        result = GetBackupConnectionStatusPort0();
        if (!result)
            GetBackupFreeBlocksPort0();
        result = -5;
        break;
    case 10:
        result = GetBackupConnectionStatusPort0();
        if (!result) {
            result = 19;
            if (CheckSaveFileExistsByIndex(save->selectedSlot))
                result -= 19;
            if (GetBackupFreeBlocksPort0() >= result)
                result = 0;
            else
                result = -5;
        }
        break;
    case 9:
        result = GetBackupConnectionStatusPort0();
        if (!result) {
            result = 23;
            if (CheckSaveFileExistsByIndex(3))
                result = 19;
            if (CheckSaveFileExistsByIndex(save->selectedSlot))
                result -= 19;
            if (GetBackupFreeBlocksPort0() >= result)
                result = 0;
            else
                result = -5;
        }
        break;
    case 6:
        result = CheckAllGameSaveFilesPresent();
        break;
    case 3:
        result = WriteSaveDataSection(3);
        if (!result) {
            SetLoadingDrawFlags(2);
            SetItemSelectorDelayedResult(32);
        }
        break;
    case 7:
        result = CheckSaveFileExistsByIndex(3);
        if (result) {
            result = 0;
            break;
        }
        result = WriteSaveDataSection(3);
        if (!result) {
            SetLoadingDrawFlags(2);
            SetItemSelectorDelayedResult(32);
        }
        break;
    case 4:
        CacheSaveMetadata(save->selectedSlot, save->initializationMode);
        result = WriteSaveDataSection(save->selectedSlot);
        if (!result) {
            SetLoadingDrawFlags(2);
            SetItemSelectorDelayedResult(32);
        }
        break;
    case 0:
    default:
        result = GetBackupConnectionStatusPort0();
        break;
    }
    switch (result) {
    case -3:
        if (row->results[1] >= 0)
            next = row->results[1];
        break;
    case -1:
        if (row->results[0] >= 0)
            next = row->results[0];
        break;
    case -4:
        if (row->results[6] >= 0)
            next = row->results[6];
        break;
    case -6:
        if (row->results[4] >= 0)
            next = row->results[4];
        break;
    case 0:
        if (row->results[5] >= 0)
            next = row->results[5];
        break;
    case -5:
        if (row->results[2] >= 0)
            next = row->results[2];
        break;
    }
    ++save->frame;
    if (save->screenRow != next) {
        save->choice = 0;
        save->screenRow = next;
        save->frame = 0;
        if (next == 98 || next == 99) {
            save->preRenderCallback = SaveScreenFadeOut;
            if (next == 99)
                save->returnToGame = 0;
            else
                save->returnToGame = 1;
            SetSaveScreenSystemProcessing(1);
        }
    }
}

} // namespace

#else

namespace {

void SaveScreenRun(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    padcCheckControllerConnection();
    if (currentSaveState == 3 || (currentSaveState >= 7 && currentSaveState <= 9))
        return;
    save->fadeTarget = currentSaveState == 7 ? 190 : 118;
    const bool staffRollCancelled = currentSaveState == 6 && (g_inputBuffer[0].pressedPriority & 0x0200U) != 0;
    if (staffRollCancelled) {
        g_inputBuffer[0].pressedPriority = 0;
        DestroyElementByIndex(UIElement::LoadSave);
        DestroyElementByIndex(UIElement::GameBackground);
        DestroyElementByIndex(UIElement::GameInfoBar);
    }
    ApplySaveScreenTransition(*save);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SM_Init_Enqueue(std::int32_t initializationMode) {
    saveTask = HLib::EnqueueTaskWithParameter(SaveTaskInitCallback, 14, 2, sizeof(SaveTaskData), SHEN_NAME(SAVE));
    saveTaskData = static_cast<SaveTaskData *>(HLib::GetTaskParameter(saveTask));
    if (saveTaskData) {
        saveTaskData->initializationMode = initializationMode;
        if (initializationMode) {
            initlcdTask();
            QueueWindowTask(2);
            WRN_CtrlWarning_Enqueue();
            QueueTitleWarningTask();
        }
        saveTaskData->updateCallback = SaveTaskIdle;
        saveTaskData->preRenderCallback = SaveScreenInitialize;
        *reinterpret_cast<std::uint16_t *>(saveTaskData->resumeData) = 0;
    }
    currentSaveState = 0;
    SetSceneInfos(g_actNumber, g_currentMap, g_entryPoint);
    EV_SetCanDoAreaJumpTo(1);
    if (saveTaskData) {
        for (std::int32_t index = 0; index < 16; ++index)
            saveTaskData->textures[index] = -1;
        void *resource = shenLoadFile("sprite", "RESUME02.SPR", SHEN_NAME(RESU));
        if (resource) {
            saveTaskData->textures[1] = LoadAndFindTEXN_Wrapper(static_cast<TextureChunk *>(resource));
            HLib::FreeTaskData(resource);
        }
        resource = shenLoadFile("sprite", "RESUME03.SPR", SHEN_NAME(RESU));
        if (resource) {
            saveTaskData->textures[2] = LoadAndFindTEXN_Wrapper(static_cast<TextureChunk *>(resource));
            HLib::FreeTaskData(resource);
        }
    }
}

#else

HLib::Task *SM_Init_Enqueue(std::int32_t initializationMode) {
    saveOperation = {};
    saveTask = HLib::EnqueueTaskWithParameter(SaveTaskInitCallback, 0x0e, 2, sizeof(SaveTaskData), SHEN_NAME(SAVE));
    saveTaskData = saveTask ? static_cast<SaveTaskData *>(HLib::GetTaskParameter(saveTask)) : nullptr;
    if (saveTaskData) {
        saveTaskData->initializationMode = initializationMode;
        if (initializationMode != 0)
            QueueTitleSaveDependencies();
        saveTaskData->updateCallback = SaveTaskIdle;
        saveTaskData->preRenderCallback = SaveScreenInitialize;
        saveTaskData->word0 = 0;
    }
    currentSaveState = 0;
    SetSceneInfos(g_actNumber, g_currentMap, g_entryPoint);
    EV_SetCanDoAreaJumpTo(true);
    if (saveTaskData) {
        saveTaskData->field590 = saveTaskData->field598 = saveTaskData->field5a0 = saveTaskData->field5a8 = saveTaskData->field5b0 = saveTaskData->field5b8 =
            saveTaskData->field5c0 = saveTaskData->field5c8 = std::numeric_limits<std::uint64_t>::max();
        void *const resume02 = LoadLegacySceneFile("sprite", "RESUME02.SPR", SAVE_SPRITE_FILE);
        if (resume02) {
            const std::uint32_t texture = static_cast<std::uint32_t>(ReadTexnTexture(resume02));
            saveTaskData->field590 = (saveTaskData->field590 & 0x00000000ffffffffULL) | (static_cast<std::uint64_t>(texture) << 32U);
            HLib::FreeTaskData(resume02);
        }
        void *const resume03 = LoadLegacySceneFile("sprite", "RESUME03.SPR", SAVE_SPRITE_FILE);
        if (resume03) {
            const std::uint32_t texture = static_cast<std::uint32_t>(ReadTexnTexture(resume03));
            saveTaskData->field598 = (saveTaskData->field598 & 0xffffffff00000000ULL) | texture;
            HLib::FreeTaskData(resume03);
        }
    }
    return saveTask;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawLegacySaveText(const char *text) {
    for (const char *pair = text; pair[0]; pair += 2) {
        const std::uint16_t code = static_cast<std::uint16_t>((static_cast<std::uint8_t>(pair[0]) << 8U) | static_cast<std::uint8_t>(pair[1]));
        for (const LegacySaveGlyph &glyph : LEGACY_SAVE_GLYPHS) {
            if (!glyph.code)
                break;
            if (glyph.code == code) {
                nullsub();
                break;
            }
        }
        if (!pair[1])
            break;
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawLegacySaveNumber(const char *text) {
    std::array<char, 80> digits{};
    std::strncpy(digits.data(), text, digits.size() - 1);
    std::size_t length = std::strlen(digits.data());
    const std::int32_t value = std::atoi(digits.data());
    if (value < 0)
        return;
    if (value > 50000) {
        DrawLegacySaveText(" +");
        std::strcpy(digits.data(), "50000");
        length = std::strlen(digits.data());
    }
    for (std::size_t index = 0; index < length; ++index) {
        if (index == 3)
            DrawLegacySaveText(" ,");
        digits[length - 1 - index] = 0;
        nullsub();
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool MatchLegacySaveArea(std::uint32_t pattern, shen_name map, std::int32_t &exact) {
    exact = 0;
    std::int32_t wildcards = 0;
    for (std::uint32_t index = 0; index < 4; ++index) {
        const std::uint8_t wanted = static_cast<std::uint8_t>(pattern >> (8U * index));
        if (wanted == static_cast<std::uint8_t>(map.name >> (8U * index)))
            ++exact;
        else if (wanted == '*')
            ++wildcards;
    }
    return wildcards + exact >= 4;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t GetLegacySaveAreaId(shen_name map) {
    std::int32_t best = 0;
    std::int32_t area = -1;
    for (std::size_t row = 0; row < LEGACY_SAVE_AREAS.size(); ++row) {
        std::int32_t exact = 0;
        if (MatchLegacySaveArea(LEGACY_SAVE_AREAS[row].pattern, map, exact) && exact > best) {
            best = exact;
            area = static_cast<std::int32_t>(row);
        }
    }
    if (best < 4) {
        for (const LegacySaveArea &alias : LEGACY_SAVE_AREA_ALIASES) {
            std::int32_t exact = 0;
            if (MatchLegacySaveArea(alias.pattern, map, exact) && exact > best) {
                area = alias.index;
                best = exact;
            }
        }
    }
    return area;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t GetLegacySaveLocationId(std::int32_t area) {
    if (area < 0)
        return 0;
    const std::uint32_t name = LEGACY_SAVE_AREAS[static_cast<std::size_t>(area)].pattern;
    if (name == make_shen_name("0200"))
        return 2;
    if (name == make_shen_name("WAT0"))
        return 1;
    if (name == make_shen_name("KRF1"))
        return 3;
    if (name == make_shen_name("CLS1"))
        return 4;
    if (name == make_shen_name("CLS2"))
        return 5;
    switch (static_cast<char>(name & 0xffU)) {
    case 'A':
        return 0;
    case 'K':
        return 3;
    case 'Q':
        return 2;
    case 'W':
        return 1;
    default:
        return -1;
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawLegacySaveArea(std::int32_t area, std::size_t row) {
    if (area < 0) {
        legacySaveList.areaIds[row] = -1;
        return;
    }
    if (area != legacySaveList.areaIds[row])
        legacySaveList.areaIds[row] = area;
    nullsub();
    DrawLegacySaveText("k1");
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawLegacySaveLocation(std::int32_t area, std::size_t row) {
    const std::int32_t location = GetLegacySaveLocationId(area);
    if (location < 0) {
        legacySaveList.locationIds[row] = -1;
        return;
    }
    if (location != legacySaveList.locationIds[row])
        legacySaveList.locationIds[row] = location;
    nullsub();
    DrawLegacySaveText("k1");
    DrawLegacySaveArea(area, row);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawLegacySaveSlotIndex(const SaveTaskData &save) {
    for (std::int32_t slot = 0; slot < 10; ++slot) {
        DrawLegacySaveText("a0");
        const bool visible = save.screenPage <= slot && slot < save.screenPage + 3;
        if (save.selectedSlot == slot)
            DrawLegacySaveText("a2");
        else if (visible)
            DrawLegacySaveText("a1");
    }
    DrawLegacySaveText("a3");
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawLegacySaveSlotList(SaveTaskData &save) {
    saveTaskData->fadeTarget = currentSaveState == 7 ? 190 : 118;
    WRN_SetWarning(0, 0);
    saveTaskData->previousWarning = 0;
    saveTaskData->screenItem = 0;
    (void)ReturnZero();
    shenSubmitQuadsUI(320.0f, 380.0f, 0x3b00008cU, 0xff4040c0U, 422, 76, 6.9000001f);
    nullsub();
    char text[80];
    std::snprintf(text, sizeof(text), "%d", 100);
    DrawLegacySaveNumber(text);
    DrawLegacySaveText("FB");
    DrawLegacySaveSlotIndex(save);
    std::int32_t y = 48;
    for (std::size_t row = 0; row < 3; ++row) {
        const std::int32_t slot = static_cast<std::int32_t>(row) + save.screenPage;
        const S2SaveDataInfo *const info = GetBackupSaveDataInfo(slot);
        const bool selected = static_cast<std::int32_t>(row) == save.screenSelection;
        shenSubmitQuadsUI(320.0f, static_cast<float>(y + 43), selected ? 0x3b8c0000U : 0x3b00008cU, selected ? 0xffc04040U : 0xff4040c0U, 480, 86, 6.9000001f);
        std::snprintf(text, sizeof(text), "bfb%1d", slot);
        DrawLegacySaveText("bf");
        DrawLegacySaveText(text + 2);
        if (info->saveKind) {
            nullsub();
            const shen_name map = info->clearState != 6 ? info->mapName : LEGACY_SAVE_CLEARED_AREA;
            const std::int32_t area = GetLegacySaveAreaId(map);
            if (area != -1)
                DrawLegacySaveLocation(area, row);
            const std::uint32_t gameHour = info->gameHour / 10U;
            std::snprintf(text, sizeof(text), "  %2u%2u /%2u%2u / 1 9%2d%2d   %c%2u :%2u%2u", info->gameMonth / 10U, info->gameMonth % 10U, info->gameDay / 10U,
                info->gameDay % 10U, info->gameYear / 10, info->gameYear % 10, static_cast<int>(gameHour ? gameHour + 48 : 97), info->gameHour % 10U,
                info->gameMinute / 10U, info->gameMinute % 10U);
            DrawLegacySaveText(text);
            const std::uint32_t systemHour = info->systemHour / 10U;
            std::snprintf(text, sizeof(text), "  %2u%2u /%2u%2u /%2u%2u%2u%2u   %c%2u :%2u%2u", info->systemMonth / 10U, info->systemMonth % 10U,
                info->systemDay / 10U, info->systemDay % 10U, info->systemYear / 1000U, info->systemYear % 1000U / 100U, info->systemYear % 100U / 10U,
                info->systemYear % 10U, static_cast<int>(systemHour ? systemHour + 48 : 97), info->systemHour % 10U, info->systemMinute / 10U,
                info->systemMinute % 10U);
            DrawLegacySaveText(text);
        } else {
            DrawLegacySaveText("EY");
        }
        y += 94;
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static bool GetSaveSlotPayload(std::byte **header, std::byte **payload, std::int32_t slot);

std::int32_t ValidateSaveSlotChecksum(std::int32_t slot) {
    std::byte *header;
    std::byte *payload;
    if (GetSaveSlotPayload(&header, &payload, slot))
        return CalculateSavePayloadChecksum(payload, 0x1e00) == *reinterpret_cast<std::uint32_t *>(header + 0x20);
    return 0;
}

std::int32_t ValidateSaveSlot(std::int32_t slot) {
    return ValidateSaveSlotChecksum(slot);
}

std::int32_t LoadImportedSaveFile(void *image) {
    std::int32_t result = CheckBackupFileExists(0, "SHENMUE1_000");
    if (result)
        return result;
    if (buGetFileSize(0, "SHENMUE1_000") < 1)
        return -6;
    g_backupResults[0] = -2;
    return buLoadFile(0, "SHENMUE1_000", image, 0x50) == 0 ? 0 : -6;
}

void RestoreSaveGameTime(const std::byte *time) {
    g_calendarRecords[0].year = static_cast<std::int8_t>(time[0]);
    g_calendarRecords[0].month = static_cast<std::int8_t>(time[2]);
    g_calendarRecords[0].day = static_cast<std::int8_t>(time[3]);
    g_calendarRecords[0].hour = static_cast<std::int8_t>(time[4]);
    g_calendarRecords[0].minute = static_cast<std::int8_t>(time[5]);
    g_calendarRecords[0].second = static_cast<std::int8_t>(time[6]);
    g_calendarRecords[0].week = static_cast<std::uint8_t>(time[7]);
}

void GetScenarioFlagBanks(std::uint8_t **banks);

// @verified - d
void CaptureSaveBufferBlocks(std::byte *stats, std::byte *story, std::byte *internal) {
    std::uint8_t *banks[4];
    GetScenarioFlagBanks(banks);
    std::memcpy(stats, banks[2], 0x40);
    std::memcpy(story, banks[0], 8);
    std::memcpy(internal, banks[1], 0x200);
}

void RestoreSaveBufferBlocks(const std::byte *stats, const std::byte *story, const std::byte *internal) {
    std::uint8_t *banks[4];
    GetScenarioFlagBanks(banks);
    std::memcpy(banks[2], stats, 0x40);
    std::memcpy(banks[0], story, 8);
    std::memcpy(banks[1], internal, 0x200);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t CalculateSavePayloadChecksum(const void *source, std::int32_t size) {
    const std::int8_t *bytes = static_cast<const std::int8_t *>(source);
    std::uint32_t checksum = 0xffffffffU;
    for (std::int32_t index = 0; index < size; ++index) {
        checksum ^= static_cast<std::uint32_t>(static_cast<std::int32_t>(*bytes++)) << 24U;
        for (std::int32_t bit = 0; bit < 8; ++bit) {
            if (checksum & 0x80000000U)
                checksum = (checksum << 1U) ^ 0x04c11db7U;
            else
                checksum <<= 1U;
        }
    }
    return ~checksum;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void RegisterBackupCompleteCallback() {
    buSetCompleteCallback(BackupCompleteCallback);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t BackupCompleteCallback(std::int32_t port, std::int32_t operation, std::int32_t result) {
    switch (operation) {
    case 1: {
        while (buStat(port) == -1) {
        }
        const std::int32_t format = buIsFormat(port);
        if (format == 0) {
            g_backupResults[port] = format;
            return format;
        }
        g_backupResults[port] = -3;
        break;
    }
    case 2:
        g_backupResults[port] = -1;
        break;
    case 11:
    case 12:
    case 14:
        while (buStat(port) == -1) {
        }
        if (result == 0) {
            g_backupResults[port] = result;
            return result;
        }
        g_backupResults[port] = -6;
        break;
    default:
        break;
    }
    return 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t BackupProgressCallback() {
    g_backupProgress = g_backupProgress + 1 < g_backupProgressLimit ? g_backupProgress + 1 : g_backupProgressLimit;
    return 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void InitializeBackupMemory() {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(g_backupResults) / sizeof(g_backupResults[0])); ++index)
        g_backupResults[index] = -1;
    while (buInit(15, 1, g_backupWorkBuffer, RegisterBackupCompleteCallback) != 0) {
    }
    g_backupProgressLimit = 38;
    g_backupProgress = 0;
    buSetProgressCallback(BackupProgressCallback);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void WaitForBackupMemoryIdle() {
    while (buExit() != 0) {
    }
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

BackupSlotMetadata *GetCachedBackupSlot(std::int32_t slot) {
    return g_cachedBackupSlots + slot;
}

std::int32_t GetBackupConnectionStatusPort0() {
    return g_backupResults[0];
}

// @verified - d
std::int32_t GetBackupConnectionStatus(std::int32_t index) {
    return g_backupResults[index];
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t WaitForBackupOperation(std::int32_t index) {
    do {
    } while (g_backupResults[index] == -2);
    return g_backupResults[index];
}

std::int32_t WaitForBackupOperationPort0() {
    return WaitForBackupOperation(0);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t CheckBackupFileExists(std::int32_t index, const char *filename) {
    if (g_backupResults[index] != 0)
        return g_backupResults[index];
    return buIsExistFile(index, filename) == 0 ? 0 : -4;
}

std::int32_t CheckBackupFileExistsPort0(const char *filename) {
    return CheckBackupFileExists(0, filename);
}

std::int32_t CheckSaveFileExistsByIndex(std::int32_t index) {
    if (index == -1)
        return 0;
    return CheckBackupFileExistsPort0(kGameSaveFilenames[index]);
}

#else

// @verified - p
std::uint64_t CheckBackupFileExists(const char *name) {
#if defined(SHENMUE_STEAM)
    return ::shenmue::d3t::SteamFileExists(name) ? 0U : 0xfffffffcU;
#else
    const std::filesystem::path path = saveDirectory / name;
#ifdef _WIN32
    const DWORD attributes = GetFileAttributesA(path.string().c_str());
    return attributes == INVALID_FILE_ATTRIBUTES || (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ? 0xfffffffcU : 0U;
#else
    struct stat attributes{};
    return stat(path.c_str(), &attributes) || S_ISDIR(attributes.st_mode) ? 0xfffffffcU : 0U;
#endif
#endif
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t QueueBackupFileRead(const char *name, void *destination, std::uint32_t offset, std::uint32_t size, BackupSaveCompletion completion) {
    BupReady(1);
    activeBackupState.store(-2, std::memory_order_seq_cst);
    backupSaveCompletion = completion;
    const std::uint64_t preflight = CheckBackupFileExists(name);
    if (preflight) {
        backupCompletionSource = destination;
        backupCompletionStage = 2;
        backupCompletionResult = -4;
        const BackupSaveCompletion callback = backupSaveCompletion;
        activeBackupState.store(0, std::memory_order_seq_cst);
        if (callback)
            callback(backupCompletionResult, backupCompletionSource);
        return preflight;
    }
    const std::string path = (saveDirectory / name).string();
    QueueBackupArchiveRead(path.c_str(), destination, offset, size);
    return 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t ReadBackupFile(const char *name, void *destination, std::uint32_t offset, std::uint32_t size, BackupSaveCompletion completion) {
    const std::uint64_t result = QueueBackupFileRead(name, destination, offset, size, completion);
    if (result != 0)
        return result;
    BupReady(0);
    return static_cast<std::uint32_t>(backupCompletionResult);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t ScanS1BackupSlots() {
    std::uint32_t flags = 0;
    for (std::size_t slot = 0; slot < g_backupS1SaveDataInfos.size(); ++slot) {
        const char *const name = S1_BACKUP_SLOT_NAMES[slot];
        if (BupReady(0) != 0 || CheckBackupFileExists(name) != 0 || ReadBackupFile(name, backupS1Prefix.data(), 0x600, 0x200) != 0)
            continue;
        std::uint32_t signature, version;
        std::memcpy(&signature, backupS1Prefix.data() + 0x80, sizeof(signature));
        std::memcpy(&version, backupS1Prefix.data() + 0x84, sizeof(version));
        if (signature != 0x45454853U || version != 270)
            continue;
        flags |= 4U;
        if (allowIncompleteS1Import != 0 || backupS1Prefix[0x164] == std::byte{6}) {
            flags |= 8U;
            std::memcpy(&g_backupS1SaveDataInfos[slot], backupS1Prefix.data() + 0x140, sizeof(S2SaveDataInfo));
        }
    }
    return flags;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t ScanS2BackupSlots() {
    std::uint32_t flags = 0;
    for (std::size_t slot = 0; slot < kSaveSlotNames.size(); ++slot) {
        const char *const name = kSaveSlotNames[slot];
        if (BupReady(0) != 0 || CheckBackupFileExists(name) != 0)
            continue;
        backupS2Prefix.fill(std::byte{});
        if (ReadBackupFile(name, backupS2Prefix.data(), 0xe00, 0x200) != 0)
            continue;
        std::uint32_t signature, version;
        std::memcpy(&signature, backupS2Prefix.data() + 0x80, sizeof(signature));
        std::memcpy(&version, backupS2Prefix.data() + 0x84, sizeof(version));
        if (signature != kGameMagic || version != kSaveVersion)
            continue;
        flags |= S2_BACKUP_SLOT_FLAGS[slot];
        S2SaveDataInfo &info = g_backupSaveDataInfos[slot];
        std::memcpy(&info, backupS2Prefix.data() + 0xa0, sizeof(info));
        if (info.saveKind != 0) {
            flags |= 0x40U;
            if (info.clearState == 6)
                flags |= 0x20U;
        }
    }
    return flags != 0 ? flags | 0x10U : 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
S2SaveDataInfo *GetBackupSaveDataInfo(std::int32_t slot) noexcept {
    if (slot >= 11)
        return nullptr;
    const std::ptrdiff_t offset = std::ptrdiff_t{offsetof(S2NativeSaveBacking, primary)} + std::ptrdiff_t{slot} * std::ptrdiff_t{sizeof(S2SaveDataInfo)};
    return reinterpret_cast<S2SaveDataInfo *>(reinterpret_cast<std::uintptr_t>(&nativeSaveBacking) + static_cast<std::uintptr_t>(offset));
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
S2SaveDataInfo *GetS1BackupSaveDataInfo(std::uint32_t slot) noexcept {
    return slot < 10 ? &g_backupS1SaveDataInfos[slot] : nullptr;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void RenewLocalBackupInfo() {
    for (std::size_t slot = 0; slot < S2SaveSlotCount; ++slot)
        g_backupSaveDataInfos[slot] = {};
    g_backupS1SaveDataInfos.fill({});
    localBackupInfo |= ScanS1BackupSlots();
    localBackupInfo |= ScanS2BackupSlots();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t GetLocalBackupInfo() noexcept {
    return localBackupInfo;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t GetCurrentGameFlags() noexcept {
    return currentGameFlags;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t SetCurrentGameFlag(std::uint8_t bit, std::int32_t enabled) noexcept {
    const std::uint32_t mask = 1U << (bit & 31U);
    if (enabled) {
        currentGameFlags |= mask;
        return mask;
    }
    currentGameFlags &= ~mask;
    return ~mask;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t GetCompletedGameCount() noexcept {
    return completedGameCount;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t SetCompletedGameCount(std::uint32_t count) noexcept {
    if (count <= 255U)
        completedGameCount = count;
    return completedGameCount;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const S2CombatStatistics &S2Save_GetCombatStatistics() noexcept {
    return combatStatistics;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Runtime counters saturate and skip the three non-move IDs.
void S2Save_AccumulateCombatStatistics(std::span<const std::byte, 0x200> history, std::uint16_t streakDelta, std::int32_t defaultHealth) noexcept {
    for (std::size_t index = 0; index < 128; ++index) {
        if (index >= 97 && index <= 99)
            continue;
        std::uint16_t performed, received;
        std::memcpy(&performed, history.data() + 2 * index, sizeof(performed));
        std::memcpy(&received, history.data() + 0x100 + 2 * index, sizeof(received));
        combatStatistics.performedMoves[index] =
            static_cast<std::uint16_t>(std::min(std::uint32_t{65535}, std::uint32_t(combatStatistics.performedMoves[index]) + performed));
        combatStatistics.receivedMoves[index] =
            static_cast<std::uint16_t>(std::min(std::uint32_t{65535}, std::uint32_t(combatStatistics.receivedMoves[index]) + received));
    }
    combatStatistics.streak = static_cast<std::uint16_t>(std::min(std::uint32_t{65535}, std::uint32_t(combatStatistics.streak) + streakDelta));
    if (defaultHealth >= 0)
        playerStats.defaultHealth = static_cast<std::uint16_t>(defaultHealth);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const S2PersistentPlayerStats &S2Save_GetPersistentPlayerStats() noexcept {
    return playerStats;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2Save_SetPersistentPlayerStats(const S2PersistentPlayerStats &stats) noexcept {
    playerStats = stats;
    SetCurrentCash(playerStats.cash);
    SetS2GameConfigurationFlags(playerStats.gameConfigurationFlags);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t GetPlayerStat(PlayerStatsId type) noexcept {
    switch (type) {
    case PlayerStatsId::SavedFlags:
        return playerStats.savedStat0;
    case PlayerStatsId::RuntimeFlags:
        return playerStats.runtimeFlags;
    case PlayerStatsId::Cash:
        return static_cast<std::uint32_t>(GetCurrentCash());
    case PlayerStatsId::AlternateHealth:
        return static_cast<std::uint32_t>(std::bit_cast<std::int16_t>(playerStats.alternateHealth));
    case PlayerStatsId::DefaultHealth:
        return static_cast<std::uint32_t>(std::bit_cast<std::int16_t>(playerStats.defaultHealth));
    case PlayerStatsId::ReactionCameraScale:
        return static_cast<std::uint32_t>(std::bit_cast<std::int16_t>(playerStats.reactionCameraScale));
    case PlayerStatsId::AssistScale:
        return static_cast<std::uint32_t>(std::bit_cast<std::int16_t>(playerStats.assistScale));
    case PlayerStatsId::ContextActionMode:
        return static_cast<std::uint32_t>(std::bit_cast<std::int16_t>(playerStats.contextActionMode));
    case PlayerStatsId::SegaCoins:
        return static_cast<std::uint32_t>(playerStats.segaCoins);
    default:
        return 0xffffffffULL;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void SetPlayerStat(PlayerStatsId type, std::uint32_t value) noexcept {
    switch (type) {
    case PlayerStatsId::SavedFlags:
        playerStats.savedStat0 = value;
        break;
    case PlayerStatsId::RuntimeFlags:
        playerStats.runtimeFlags = value;
        break;
    case PlayerStatsId::Cash:
        SetCurrentCash(std::bit_cast<std::int32_t>(value));
        break;
    case PlayerStatsId::AlternateHealth:
        playerStats.alternateHealth = static_cast<std::uint16_t>(value);
        break;
    case PlayerStatsId::DefaultHealth:
        playerStats.defaultHealth = static_cast<std::uint16_t>(value);
        break;
    case PlayerStatsId::ReactionCameraScale:
        playerStats.reactionCameraScale = static_cast<std::uint16_t>(value);
        break;
    case PlayerStatsId::AssistScale:
        playerStats.assistScale = static_cast<std::uint16_t>(value);
        break;
    case PlayerStatsId::ContextActionMode:
        playerStats.contextActionMode = static_cast<std::uint16_t>(value);
        break;
    case PlayerStatsId::SegaCoins: {
        playerStats.segaCoins = std::bit_cast<std::int32_t>(value);
        std::uint32_t highWater{};
        std::memcpy(&highWater, combatStatistics.retainedPrefix.data() + S2CombatSegaCoinHighWaterOffset, sizeof(highWater));
        if (highWater < value)
            highWater = value;
        std::memcpy(combatStatistics.retainedPrefix.data() + S2CombatSegaCoinHighWaterOffset, &highWater, sizeof(highWater));
        break;
    }
    default:
        break;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int32_t GetCurrentSegaCoins() noexcept {
    return playerStats.segaCoins;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SetCurrentSegaCoins(std::int32_t segaCoins) noexcept {
    playerStats.segaCoins = segaCoins;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2Save_SetSkillFlags(const std::array<std::byte, 16> &flags) noexcept {
    playerStats.statFlags = flags;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
bool vfCheckSkillFlag(shen_name flag) noexcept {
    const std::uint32_t value = flag.name;
    std::uint32_t bank{};
    std::memcpy(&bank, playerStats.statFlags.data() + 4U * (value >> 30U), sizeof(bank));
    return (value & bank & 0x3fffffffU) != 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int32_t GetTransferredYen() noexcept {
    return playerStats.transferredYen;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::span<const S2MoveSkillBank, 3> S2Save_GetMoveSkillBanks() noexcept {
    return moveSkillBanks;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2Save_SetMoveProficiency(std::uint8_t category, std::size_t index, std::int16_t value) noexcept {
    if (category < 1 || category > moveSkillBanks.size() || index >= moveSkillBanks[category - 1U].proficiency.size())
        return false;
    moveSkillBanks[category - 1U].proficiency[index] = value;
    return true;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t GetMoveProficiency(std::uint8_t category, std::uint8_t move) noexcept {
    if (static_cast<std::uint8_t>(category - 1U) > 2U || move >= 40U)
        return 0;
    return static_cast<std::uint32_t>(moveSkillBanks[category - 1U].proficiency[move]);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int64_t SetMoveProficiency(std::int8_t clear, std::uint8_t category, std::int16_t move, std::int16_t value) noexcept {
    S2MoveSkillBank &bank = moveSkillBanks[category - 1U];
    if (move < static_cast<std::int8_t>(bank.count)) {
        if (clear)
            value = 0;
        SetMoveEquipment(clear ? 1 : 0, category, static_cast<std::uint8_t>(move));
        bank.proficiency[move] = value;
    }
    return 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::span<const std::byte> S2Save_GetInventoryBank(S2InventoryBank bank) noexcept {
    return InventoryBankConst(bank);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2Save_SetInventoryItem(S2InventoryBank bank, std::size_t index, std::byte value) noexcept {
    std::span<std::byte> data = InventoryBank(bank);
    if (index >= data.size())
        return false;
    data[index] = value;
    return true;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::span<const std::int32_t, 26> S2Save_GetMinigameScores() noexcept {
    return minigameScores;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t GetSavedMinigameScore(std::uint32_t index) noexcept {
    return index <= 25 ? static_cast<std::uint32_t>(minigameScores[index]) : 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SetSavedMinigameScore(SavedScoreType type, std::uint32_t score) noexcept {
    const std::uint32_t index = static_cast<std::uint32_t>(type);
    if (index <= 25)
        minigameScores[index] = std::bit_cast<std::int32_t>(score);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void InitializeSnapshotMetadata() noexcept {
    snapshotMetadata = {};
    constexpr char identifier[] = "S2XSnapshots1.07";
    std::copy_n(identifier, sizeof(identifier), snapshotMetadata.identifier.begin());
    for (std::size_t index = 0; index < snapshotMetadata.records.size(); ++index) {
        snapshotMetadata.records[index].id = static_cast<std::int32_t>(index);
    }
    bool loaded = false;
    try {
        loaded = LoadSnapshotMetadataFile();
    } catch (...) {
        loaded = false;
        snapshotMetadata = {};
        std::copy_n(identifier, sizeof(identifier), snapshotMetadata.identifier.begin());
        for (std::size_t index = 0; index < snapshotMetadata.records.size(); ++index) {
            snapshotMetadata.records[index].id = static_cast<std::int32_t>(index);
        }
    }
    snapshotMetadataInitialized = true;
    if (loaded)
        ValidateSnapshotMetadataFiles();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void ValidateSnapshotMetadataFiles() noexcept {
    bool changed = false;
    for (SnapshotMetadataRecord &record : snapshotMetadata.records) {
        char path[32];
        std::snprintf(path, sizeof(path), "%d.jpg", record.id);
        if (record.use == 1 && CheckBackupFileExists(path) != 0) {
            snapResetMetadataRecord(record);
            changed = true;
        }
    }
    if (changed)
        SaveSnapshotMetadata();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool SaveSnapshotMetadata() noexcept {
    try {
        if (!snapshotMetadataInitialized)
            InitializeSnapshotMetadata();
        return WriteSnapshotMetadataFile();
    } catch (...) {
        return false;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

SnapshotMetadataOwner *GetSnapshotMetadataOwner() noexcept {
    if (!snapshotMetadataInitialized)
        InitializeSnapshotMetadata();
    return &snapshotMetadata;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t GetSnapshotMetadataCount() noexcept {
    const SnapshotMetadataOwner *const owner = GetSnapshotMetadataOwner();
    return static_cast<std::uint32_t>(
        std::count_if(owner->records.begin(), owner->records.end(), [](const SnapshotMetadataRecord &record) { return record.use == 1; }));
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int32_t GetSnapshotDataOption() noexcept {
    return snapshotDataOption;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SetSnapshotDataOption(std::int32_t option) noexcept {
    snapshotDataOption = option;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t QueueSnapshotSave(
    std::uint32_t slot, std::uint32_t rowPitch, std::uint32_t width, std::uint32_t height, std::uint32_t format, std::vector<std::byte> &&pixels) noexcept {
    static_cast<void>(format);
#if defined(_WIN32)
    std::byte *const data = static_cast<std::byte *>(std::malloc(pixels.size()));
    if (!pixels.empty())
        std::memcpy(data, pixels.data(), pixels.size());
    SnapshotSaveWork work{.slot = slot, .rowPitch = rowPitch, .width = width, .height = height, .pixels = data};
    const d3t::D3TTaskDescriptor descriptor{.data = &work, .callback = WriteSnapshotTask, .completion = nullptr, .priority = 2};
    snapshotSaveInProgress.store(1, std::memory_order_seq_cst);
    return d3t::d3t_QueueTask(&descriptor, sizeof(work));
#else
    static_cast<void>(slot);
    static_cast<void>(rowPitch);
    static_cast<void>(width);
    static_cast<void>(height);
    static_cast<void>(pixels);
    return 0;
#endif
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t IsSnapshotSaveInProgress() noexcept {
    return snapshotSaveInProgress.load(std::memory_order_seq_cst);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Native shenGetLocalTime: the wall-clock SYSTEMTIME fields; the trailing byte is always 15.
std::int64_t shenGetLocalTime(LocalTimeSnapshot *snapshot) noexcept {
    if (!snapshot)
        return 0xffffffff;
    std::tm now{};
    GetLocalTime(now);
    snapshot->year = static_cast<std::uint16_t>(now.tm_year + 1900);
    snapshot->month = static_cast<std::uint8_t>(now.tm_mon + 1);
    snapshot->day = static_cast<std::uint8_t>(now.tm_mday);
    snapshot->hour = static_cast<std::uint8_t>(now.tm_hour);
    snapshot->minute = static_cast<std::uint8_t>(now.tm_min);
    snapshot->second = static_cast<std::uint8_t>(now.tm_sec);
    snapshot->dayOfWeek = static_cast<std::uint8_t>(now.tm_wday);
    snapshot->reserved08 = 15;
    return 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Native shenGetLocalTimeSnapshot: shenGetLocalTime without the trailing byte.
std::int64_t shenGetLocalTimeSnapshot(LocalTimeSnapshot *snapshot) noexcept {
    LocalTimeSnapshot now{};
    shenGetLocalTime(&now);
    snapshot->year = now.year;
    snapshot->month = now.month;
    snapshot->day = now.day;
    snapshot->hour = now.hour;
    snapshot->minute = now.minute;
    snapshot->second = now.second;
    snapshot->dayOfWeek = now.dayOfWeek;
    return now.dayOfWeek;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::filesystem::path S2Save_SnapshotPath(std::uint32_t slot) {
    return saveDirectory / (std::to_string(slot) + ".jpg");
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t S2Save_Checksum(std::span<const std::byte> payload) noexcept {
    std::uint32_t checksum = 0xffffffffU;
    for (const std::byte byte : payload) {
        checksum ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(byte)) << 24U;
        for (std::uint32_t bit = 0; bit < 8; ++bit) {
            checksum = (checksum & 0x80000000U) != 0 ? (checksum << 1U) ^ 0x04c11db7U : checksum << 1U;
        }
    }
    return 0U - checksum;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

S2SaveResult S2Save_Validate(const S2SaveImage &image) noexcept {
    if (ReadValue<std::uint32_t>(image, kSystemHeaderOffset) != kSystemMagic || ReadValue<std::uint32_t>(image, kSystemHeaderOffset + 4) != kSaveVersion) {
        return S2SaveResult::BadSystemHeader;
    }
    if (ReadValue<std::uint32_t>(image, kGameHeaderOffset) != kGameMagic || ReadValue<std::uint32_t>(image, kGameHeaderOffset + 4) != kSaveVersion) {
        return S2SaveResult::BadGameHeader;
    }
    const S2SaveDataInfo info = SaveDataInfo(image);
    if (!SaveDataInfoIsValid(info))
        return S2SaveResult::BadGameHeader;
    if (info.checksum != S2Save_Checksum(Payload(image)))
        return S2SaveResult::ChecksumMismatch;
    return S2SaveResult::Success;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void CaptureGameSaveData(S2SaveImage &image, std::uint8_t clearState, const std::tm &systemTime, const DateTime &gameTime, bool capturePlacement) {
    CaptureRuntimePayload(image, capturePlacement);
    S2SaveDataInfo info = SaveDataInfo(image);
    info.systemYear = static_cast<std::uint16_t>(systemTime.tm_year + 1900);
    info.systemMonth = static_cast<std::uint8_t>(systemTime.tm_mon + 1);
    info.systemDay = static_cast<std::uint8_t>(systemTime.tm_mday);
    info.systemHour = static_cast<std::uint8_t>(systemTime.tm_hour);
    info.systemMinute = static_cast<std::uint8_t>(systemTime.tm_min);
    info.systemSecond = static_cast<std::uint8_t>(systemTime.tm_sec);
    info.systemWeek = static_cast<std::uint8_t>(systemTime.tm_wday);

    info.gameYear = gameTime.year;
    info.gameMonth = static_cast<std::uint8_t>(gameTime.month);
    info.gameDay = static_cast<std::uint8_t>(gameTime.day);
    info.gameHour = static_cast<std::uint8_t>(gameTime.hour);
    info.gameMinute = static_cast<std::uint8_t>(gameTime.minute);
    info.gameSecond = static_cast<std::uint8_t>(gameTime.second);
    info.gameWeek = gameTime.week;
    ++info.saveKind;
    if (info.saveKind == 0)
        info.saveKind = 0xffffU;
    info.actNumber = static_cast<std::int16_t>(g_loadedActNumber + (clearState == 4));
    const std::int32_t step = static_cast<std::int32_t>(GetCurrentStep());
    info.stepHundreds = static_cast<std::uint16_t>(step / 100);
    info.sceneNumber = static_cast<std::int16_t>(savedSceneInfo.actNumber);
    info.mapName = savedSceneInfo.mapName;
    info.entryPoint = savedSceneInfo.entryPoint;
    info.clearState = clearState;
    info.stepRemainder = static_cast<std::uint8_t>(step % 100);
    info.checksum = S2Save_Checksum(Payload(image));
    WriteSaveDataInfo(image, info);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::uint64_t RefreshSaveSystemHeader(S2SaveImage &image) {
    WriteValue(image, kSystemHeaderOffset, kSystemMagic);
    WriteValue(image, kSystemHeaderOffset + 4, kSaveVersion);
    WriteValue(image, kSystemConfigurationOffset, GetS2GameConfigurationFlags());
    WriteValue(image, kCompletedGameCountOffset, completedGameCount);
    WriteValue(image, kCurrentGameFlagsOffset, currentGameFlags);
    std::uint32_t saveCount = ReadValue<std::uint32_t>(image, kSystemSaveCountOffset);
    if (saveCount < 255U)
        ++saveCount;
    WriteValue(image, kSystemSaveCountOffset, saveCount);
    loadedGameFlags = 0;
    loadedCompletedGameCount = 0;
    return saveCount;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::uint64_t CaptureSaveImageContent(S2SaveImage &image, std::uint8_t clearState, const std::tm &systemTime, const DateTime &gameTime, bool capturePlacement) {
    CaptureGameSaveData(image, clearState, systemTime, gameTime, capturePlacement);
    return RefreshSaveSystemHeader(image);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void PackSaveBlock(std::byte *block, std::uint32_t size) {
    std::memset(block, 0, 0x680);
    std::memcpy(block, &S2_VMU_SAVE_HEADER, sizeof(S2_VMU_SAVE_HEADER));
    const std::uint32_t dataSize = size - 0x680;
    std::memcpy(block + offsetof(S2VmuSaveHeader, dataSize), &dataSize, sizeof(dataSize));
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void CompleteGameSave(std::int32_t result, const void *) {
#if defined(SHENMUE_STEAM)
    if (!result)
        ::shenmue::d3t::WriteSteamStatisticsFile(static_cast<std::int32_t>(gameSaveSlot));
#endif
    gameSaveStatus.store(result == -5 ? 3 : result ? 4 : 2, std::memory_order_seq_cst);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t QueueGameSaveWrite(void *) {
    RefreshSaveSystemHeader(activeSaveImage);
    PackSaveBlock(activeSaveImage.bytes.data(), 0x800);
    std::memcpy(gameSaveWriteBuffer.bytes.data(), activeSaveImage.bytes.data(), 0x800);
    PackSaveBlock(activeSaveImage.bytes.data() + 0x800, 0x2400);
    std::memcpy(gameSaveWriteBuffer.bytes.data() + 0x800, activeSaveImage.bytes.data() + 0x800, 0x2400);
    QueueSaveFileWrite(kSaveSlotNames[gameSaveSlot], gameSaveWriteBuffer.bytes.data(), S2SaveFileSize, CompleteGameSave);
    return 0;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint64_t S2Save_CheckData() {
    loadedGameFlags = 0;
    loadedCompletedGameCount = 0;
    if (!activeSaveImageInitialized)
        InitializeImageLayout(activeSaveImage);
    std::tm systemTime{};
    GetLocalTime(systemTime);
    DateTime gameTime{};
    shenGetCurrentDateTime(gameTime);
    const std::uint64_t saveCount = CaptureSaveImageContent(activeSaveImage, static_cast<std::uint8_t>(IsSaveAllowed()), systemTime, gameTime, false);
    activeSaveImageInitialized = true;
    return saveCount;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t SaveGame(std::uint32_t slot) {
    loadedGameFlags = 0;
    loadedCompletedGameCount = 0;
    loadedGameFlags = ReadValue<std::uint32_t>(activeSaveImage, kCurrentGameFlagsOffset);
    loadedCompletedGameCount = ReadValue<std::uint32_t>(activeSaveImage, kCompletedGameCountOffset);
    SetLoadingDrawFlags(2);
    SetItemSelectorDelayedResult(32);
    WriteValue(activeSaveImage, kGameHeaderOffset, kGameMagic);
    WriteValue(activeSaveImage, kGameHeaderOffset + 4, kSaveVersion);
    if (slot < S2SaveSlotCount) {
        std::tm systemTime{};
        GetLocalTime(systemTime);
        DateTime gameTime{};
        shenGetCurrentDateTime(gameTime);
        CaptureGameSaveData(activeSaveImage, static_cast<std::uint8_t>(IsSaveAllowed()), systemTime, gameTime, saveTaskData->initializationMode == 0);
    }
    gameSaveStatus.store(1, std::memory_order_seq_cst);
    gameSaveSlot = slot;
    const d3t::D3TTaskDescriptor descriptor{.data = nullptr, .callback = QueueGameSaveWrite, .completion = nullptr, .priority = 2};
    return d3t::d3t_QueueTask(&descriptor, 4);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int32_t GetGameSaveStatus() {
    const std::int32_t status = gameSaveStatus.load(std::memory_order_seq_cst);
    if (status == 2) {
        RenewLocalBackupInfo();
        CheckAllSaveFiles(0, saveMenuSource);
    }
    return status >= 1 && status <= 4 ? status : -1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t GetSaveMenuSource() noexcept {
    return saveMenuSource;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t SetSaveMenuSource(std::uint32_t source) noexcept {
    if (source <= 2)
        saveMenuSource = source;
    return saveMenuSource;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

S2SaveResult S2Save_Capture(S2SaveImage &image, std::uint8_t clearState) {
    std::tm systemTime{};
    if (!GetLocalTime(systemTime))
        return S2SaveResult::ReadFailed;
    DateTime gameTime{};
    shenGetCurrentDateTime(gameTime);
    if (!GameDateTimeIsValid(gameTime))
        return S2SaveResult::BadGameHeader;
    const S2SaveResult validation = S2Save_Validate(image);
    if (validation != S2SaveResult::Success) {
        const bool empty = std::all_of(image.bytes.begin(), image.bytes.end(), [](std::byte value) { return value == std::byte{}; });
        if (!empty)
            return validation;
        InitializeImageLayout(image);
    }

    CaptureSaveImageContent(image, clearState, systemTime, gameTime, true);
    return S2Save_Validate(image);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
std::int32_t SaveRestorePlatformNullsub() noexcept {
    return 0;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t RestoreCurrentSceneInfoFromSave() noexcept {
    DecodeRuntimePayload(activeSaveImage);
    if (ReadValue<std::uint32_t>(activeSaveImage, kSystemSaveCountOffset)) {
        SetCompletedGameCount(ReadValue<std::uint32_t>(activeSaveImage, kCompletedGameCountOffset));
        currentGameFlags = ReadValue<std::uint32_t>(activeSaveImage, kCurrentGameFlagsOffset);
    }
    if (!SaveRestorePlatformNullsub())
        ActivateSavedAreaJumpLocation();
    const S2SaveDataInfo info = SaveDataInfo(activeSaveImage);
    g_actNumber = info.sceneNumber;
    g_currentMap = info.mapName;
    g_entryPoint = info.entryPoint;
    return static_cast<std::uint32_t>(g_entryPoint);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

S2SaveResult S2Save_Apply(const S2SaveImage &image) {
    const S2SaveResult validation = S2Save_Validate(image);
    if (validation != S2SaveResult::Success)
        return validation;

    const S2SaveDataInfo info = SaveDataInfo(image);
    if (info.saveKind == 0)
        return S2SaveResult::BadGameHeader;
    activeSaveImage = image;
    activeSaveImageInitialized = true;
    DecodeRuntimePayload(activeSaveImage);
    if (ReadValue<std::uint32_t>(image, kSystemSaveCountOffset)) {
        SetCompletedGameCount(ReadValue<std::uint32_t>(image, kCompletedGameCountOffset));
        currentGameFlags = ReadValue<std::uint32_t>(image, kCurrentGameFlagsOffset);
        loadedGameFlags = currentGameFlags;
        loadedCompletedGameCount = completedGameCount;
    }

    g_actNumber = info.sceneNumber;
    g_currentMap = info.mapName;
    g_entryPoint = info.entryPoint;
    return S2SaveResult::Success;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

S2SaveResult S2Save_Read(const std::filesystem::path &path, S2SaveImage &image) {
#if defined(SHENMUE_STEAM)
    if (path.empty())
        return S2SaveResult::InvalidArgument;
    const std::string name = path.filename().string();
    const std::int32_t size = ::shenmue::d3t::GetSteamFileSize(name.c_str());
    if (size < 0)
        return S2SaveResult::OpenFailed;
    if (size != S2SaveFileSize)
        return S2SaveResult::SizeMismatch;
    S2SaveImage candidate{};
    if (!::shenmue::d3t::ReadSteamFile(name.c_str(), candidate.bytes.data(), 0, static_cast<std::uint32_t>(candidate.bytes.size())))
        return S2SaveResult::ReadFailed;
    const S2SaveResult validation = S2Save_Validate(candidate);
    if (validation != S2SaveResult::Success)
        return validation;
    image = candidate;
    return S2SaveResult::Success;
#else
    if (path.empty())
        return S2SaveResult::InvalidArgument;
    std::error_code error;
    const std::uintmax_t size = std::filesystem::file_size(path, error);
    if (error)
        return S2SaveResult::OpenFailed;
    if (size != S2SaveFileSize)
        return S2SaveResult::SizeMismatch;

    S2SaveImage candidate{};
    std::ifstream file(path, std::ios::binary);
    if (!file)
        return S2SaveResult::OpenFailed;
    file.read(reinterpret_cast<char *>(candidate.bytes.data()), static_cast<std::streamsize>(candidate.bytes.size()));
    if (file.gcount() != static_cast<std::streamsize>(candidate.bytes.size()))
        return S2SaveResult::ReadFailed;
    const S2SaveResult validation = S2Save_Validate(candidate);
    if (validation != S2SaveResult::Success)
        return validation;
    image = candidate;
    return S2SaveResult::Success;
#endif
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

S2SaveResult S2Save_Write(const std::filesystem::path &path, const S2SaveImage &image) {
#if defined(SHENMUE_STEAM)
    if (path.empty())
        return S2SaveResult::InvalidArgument;
    const S2SaveResult validation = S2Save_Validate(image);
    if (validation != S2SaveResult::Success)
        return validation;
    return ::shenmue::d3t::WriteSteamFile(path.filename().string().c_str(), image.bytes.data(), static_cast<std::uint32_t>(image.bytes.size())) ?
        S2SaveResult::Success :
        S2SaveResult::WriteFailed;
#else
    if (path.empty())
        return S2SaveResult::InvalidArgument;
    const S2SaveResult validation = S2Save_Validate(image);
    if (validation != S2SaveResult::Success)
        return validation;

    if (const std::filesystem::path parent = path.parent_path(); !parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        if (error)
            return S2SaveResult::OpenFailed;
    }
    const std::filesystem::path temporary = MakeTemporaryPath(path);
    {
        std::ofstream file(temporary, std::ios::binary | std::ios::trunc);
        if (!file) {
            RemoveTemporaryFile(temporary);
            return S2SaveResult::OpenFailed;
        }
        file.write(reinterpret_cast<const char *>(image.bytes.data()), static_cast<std::streamsize>(image.bytes.size()));
        file.flush();
        if (!file) {
            file.close();
            RemoveTemporaryFile(temporary);
            return S2SaveResult::WriteFailed;
        }
        file.close();
        if (!file) {
            RemoveTemporaryFile(temporary);
            return S2SaveResult::WriteFailed;
        }
    }
    if (!FlushFileToDisk(temporary)) {
        RemoveTemporaryFile(temporary);
        return S2SaveResult::WriteFailed;
    }

    S2SaveImage verified{};
    const S2SaveResult readResult = S2Save_Read(temporary, verified);
    if (readResult != S2SaveResult::Success || verified.bytes != image.bytes) {
        RemoveTemporaryFile(temporary);
        return readResult == S2SaveResult::Success ? S2SaveResult::WriteFailed : readResult;
    }
    const S2SaveResult commit = CommitTemporaryFile(temporary, path);
    if (commit != S2SaveResult::Success)
        RemoveTemporaryFile(temporary);
    return commit;
#endif
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

S2SaveResult S2Save_ReadSlot(std::size_t slot, S2SaveImage &image) {
    if (slot >= S2SaveSlotCount)
        return S2SaveResult::InvalidArgument;
    return S2Save_Read(S2Save_SlotPath(slot), image);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

S2SaveResult S2Save_WriteSlot(std::size_t slot, std::uint8_t clearState) {
    if (slot >= S2SaveSlotCount)
        return S2SaveResult::InvalidArgument;
    S2SaveImage candidate{};
    if (activeSaveImageInitialized) {
        candidate = activeSaveImage;
    } else {
        InitializeImageLayout(candidate);
    }
    S2SaveResult result = S2Save_Capture(candidate, clearState);
    if (result != S2SaveResult::Success)
        return result;
    result = S2Save_Write(S2Save_SlotPath(slot), candidate);
    if (result != S2SaveResult::Success)
        return result;
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::WriteSteamStatisticsFile(static_cast<std::int32_t>(slot));
#endif
    activeSaveImage = candidate;
    activeSaveImageInitialized = true;
    S2Save_RefreshSlotCatalog();
    SetContinueAvailability(S2Save_HasContinueSlot());
    return S2SaveResult::Success;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::filesystem::path S2Save_SlotPath(std::size_t slot) {
    if (slot >= S2SaveSlotCount)
        return {};
    return saveDirectory / kSaveSlotNames[slot];
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2Save_SetDirectory(std::filesystem::path directory) {
    saveDirectory = std::move(directory);
    slotCatalogInitialized = false;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const std::filesystem::path &S2Save_GetDirectory() noexcept {
    return saveDirectory;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t InitializeMoveSkillStats() noexcept {
    moveSkillBanks[0].count = 28;
    ResetMoveLearningPresentationState();
    moveSkillBanks[1].count = 23;
    moveSkillBanks[2].count = 22;
    for (std::size_t index = 0; index < 40; ++index) {
        moveSkillBanks[0].proficiency[index] = index < InitialHandMoveProficiency.size() ? InitialHandMoveProficiency[index] : 0;
        moveSkillBanks[1].proficiency[index] = index < InitialLegMoveProficiency.size() ? InitialLegMoveProficiency[index] : 0;
        moveSkillBanks[2].proficiency[index] = index < InitialThrowMoveProficiency.size() ? InitialThrowMoveProficiency[index] : 0;
    }
    SetMoveEquipment(0, 1, 15);
    moveSkillBanks[0].proficiency[15] = 96;
    SetMoveEquipment(0, 1, 18);
    moveSkillBanks[0].proficiency[18] = 32;
    SetMoveEquipment(0, 2, 13);
    moveSkillBanks[1].proficiency[13] = 44;
    SetMoveEquipment(0, 2, 16);
    moveSkillBanks[1].proficiency[16] = 38;
    vfResetMoveTrainingGains();
    return ResetWazamakiState();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint8_t ResetNativePlayerStats() noexcept {
    g_savedFightBlurFlag = 0;
    playerStats = {};
    for (std::size_t index = 0; index < InitialPlayerSkillFlags.size(); ++index)
        playerStats.statFlags[index] = InitialPlayerSkillFlags[index];
    g_fightActive = 0;
    g_fightUiActive = 0;
    playerStats.cash = 596;
    playerStats.transferredYen = 17880;
    playerStats.alternateHealth = 200;
    playerStats.defaultHealth = 200;
    SetCurrentCash(playerStats.cash);
    ResetStreetFightProgress();
    ResetBattleQteTaskPointer();
    minigameScores = {};
    playerRetainedResetWords[0] = 0;
    playerRetainedResetWords[1] = 0;
    playerRetainedResetStorage = {};
    playerRetainedResetWord = 0;
    playerRetainedSelections[2] = -1;
    playerRetainedSelections[1] = -1;
    playerRetainedSelections[0] = -1;
    playerRetainedResetMask &= 0xffff0000U;
    InitializeMoveSkillStats();
    ResetMainIconState();
    ResetSleepActionDefaultState();
    combatStatistics = {};
    const std::int16_t combatSentinel = -1;
    std::memcpy(combatStatistics.retainedPrefix.data() + 0x168, &combatSentinel, sizeof(combatSentinel));
    ResetSavedFrameCounter();
    ResetShenmueFlags();
    ResetQronSceneChange();
    std::uint8_t result{};
    for (std::int8_t code = 0; code < 16; ++code)
        result = vfMotnNibbleSet(code, 0);
    currentGameFlags = 0;
    completedGameCount = 0;
    playerStats.gameConfigurationFlags = GetS2GameConfigurationFlags();
    return result;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void ResetNewGameConfiguration() {
    retainedImportedPlayerOption = 0;
    if (g_titleFlagShouldRun == 1 || g_titleFlagShouldRun == 11)
        SetTitleLogoPresent();
    else {
        SetS2GameConfigurationFlags(GetS2GameConfigurationFlags() & ~0x80U);
        if (!GetTitleVersionOverride()) {
            ApplyS2DialogueMode(2);
            SetS2GameConfigurationFlags((GetS2GameConfigurationFlags() & ~0x100U) | 0x40U);
        }
    }
    InitializeMemoPageState(0, 0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint16_t GetImportedPlayerOption() noexcept {
    return retainedImportedPlayerOption;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SetImportedPlayerOption(std::uint16_t value) noexcept {
    retainedImportedPlayerOption = value;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetBackupFreeBlocks(std::int32_t index) {
    if (g_backupResults[index] != 0)
        return g_backupResults[index];
    return buGetDiskFree(index, 0);
}

std::int32_t GetBackupFreeBlocksPort0() {
    return GetBackupFreeBlocks(0);
}

std::int32_t LoadBackupFile(std::int32_t index) {
    if (index == -1)
        return 0;
    const char *const filename = kGameSaveFilenames[index];
    std::int32_t bytes;
    std::byte *image;
    if (index == 3) {
        bytes = 0x6b4;
        image = g_systemSaveData;
    } else {
        bytes = 0x24e0;
        image = g_gameSaveData;
    }
    std::int32_t result = CheckBackupFileExistsPort0(filename);
    if (result)
        return result;
    if (buGetFileSize(0, filename) < 1)
        return -6;
    g_backupResults[0] = -2;
    if (buLoadFile(0, filename, image, (bytes + 511) / 512) == 0)
        result = WaitForBackupOperationPort0();
    else
        result = -6;
    return result;
}

std::int32_t LoadBackupFilePort0(std::int32_t index) {
    return LoadBackupFile(index);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t ReadBackupBlocks(std::int32_t index, void *destination, std::int32_t firstBlock, std::int32_t blocks, const char *filename) {
    const std::int32_t result = CheckBackupFileExists(index, filename);
    if (result != 0)
        return result;
    if (buGetFileSize(index, filename) < firstBlock + blocks)
        return -6;
    g_backupResults[index] = -2;
    return buLoadFileEx(index, filename, destination, firstBlock, blocks) == 0 ? 0 : -6;
}

std::int32_t ReadBackupBlocksPort0(void *destination, std::int32_t firstBlock, std::int32_t blocks, const char *filename) {
    return ReadBackupBlocks(0, destination, firstBlock, blocks, filename);
}

std::int32_t ReadSystemSaveFile() {
    std::int32_t result = LoadBackupFilePort0(3);
    if (!result) {
        if (!ValidateBackupSaveImage(g_systemSaveData))
            result = -6;
        if (!result && !ValidateSystemSaveHeader(reinterpret_cast<const BackupFileHeader *>(g_systemSaveData + 0x680)))
            result = -6;
    }
    return result;
}

std::int32_t ReadGameSaveFile(std::int32_t slot) {
    std::int32_t result = LoadBackupFilePort0(slot);
    if (!result) {
        if (!ValidateBackupSaveImage(g_gameSaveData))
            result = -6;
        if (!result && !ValidateGameSaveHeader(reinterpret_cast<const BackupFileHeader *>(g_gameSaveData + 0x680)))
            result = -6;
    }
    return result;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t ReadSaveDataSection(std::int32_t slot) {
    if (slot == 3)
        return ReadSystemSaveFile();
    return ReadGameSaveFile(slot);
}

#else

// @verified - p
std::uint64_t ReadSaveDataSection(std::int32_t gameSection, std::int32_t slot) {
    const char *const name = kSaveSlotNames[static_cast<std::size_t>(slot)];
    if (gameSection)
        return ReadBackupFile(name, activeSaveImage.bytes.data() + 0x800, 0x800, 0x2400);
    const std::uint64_t result = ReadBackupFile(name, activeSaveImage.bytes.data(), 0, 0x800);
    if (result)
        return result;
    return ReadValue<std::uint32_t>(activeSaveImage, kSystemHeaderOffset) == kSystemMagic &&
            ReadValue<std::uint32_t>(activeSaveImage, kSystemHeaderOffset + 4) == kSaveVersion ?
        0U :
        0xfffffffaU;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::byte *RestoreSystemSaveProgress() noexcept {
    if (ReadValue<std::uint32_t>(activeSaveImage, kSystemSaveCountOffset)) {
        SetCompletedGameCount(ReadValue<std::uint32_t>(activeSaveImage, kCompletedGameCountOffset));
        currentGameFlags = ReadValue<std::uint32_t>(activeSaveImage, kCurrentGameFlagsOffset);
    }
    return activeSaveImage.bytes.data();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t ReadSystemSaveHeader(std::int32_t slot) {
    backupS2Prefix.fill(std::byte{});
    const std::uint64_t result = ReadBackupFile(kSaveSlotNames[static_cast<std::size_t>(slot)], backupS2Prefix.data(), 0x600, 0x200);
    if (result != 0)
        return result;
    std::uint32_t signature, version;
    std::memcpy(&signature, backupS2Prefix.data() + 0x80, sizeof(signature));
    std::memcpy(&version, backupS2Prefix.data() + 0x84, sizeof(version));
    return signature == kSystemMagic && version == kSaveVersion ? 0U : 0xfffffffaU;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t RestoreSaveSystemSettings(std::int32_t slot) {
    if (ReadSystemSaveHeader(slot) == 0) {
        std::uint32_t configuration;
        std::memcpy(&configuration, backupS2Prefix.data() + 0xc0, sizeof(configuration));
        SetS2GameConfigurationFlags(configuration);
        std::memcpy(&completedGameCount, backupS2Prefix.data() + 0xc4, sizeof(completedGameCount));
    }
    return 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static void InitSystemSaveHeader(std::uint32_t *header) {
    header[0] = SAVE_SYSTEM_HEADER;
    header[1] = 0x5a;
    header[2] = g_nativePlayerStats.gameConfigurationFlags;
}

static void InitGameSaveHeader(std::uint32_t *header) {
    header[0] = SAVE_GAME_HEADER;
    header[1] = 0x5a;
    header[2] = g_nativePlayerStats.gameConfigurationFlags;
}

// @verified - d
void InitializeDefaultSaveData() {
    std::memset(g_gameSaveData, 0, 0x24e0);
    std::memset(g_systemSaveData, 0, 0x6b4);
    InitSystemSaveHeader(reinterpret_cast<std::uint32_t *>(g_systemSaveData + 0x680));
    InitGameSaveHeader(reinterpret_cast<std::uint32_t *>(g_gameSaveData + 0x680));
    std::uint32_t checksum = CalculateSavePayloadChecksum(g_gameSaveData + 0x6e0, 0x1e00);
    std::byte *save = g_gameSaveData;
    *reinterpret_cast<std::uint32_t *>(save + 0x6c0) = checksum;
}

#else

// @verified - p
std::uint32_t InitializeDefaultSaveData() noexcept {
    InitializeImageLayout(activeSaveImage);
    activeSaveImageInitialized = true;
    return SaveDataInfo(activeSaveImage).checksum;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool ValidateBackupSaveImage(void *image) {
    std::byte information[0x60];
    if (buAnalyzeBackupFileImage(information, image) == 0)
        return true;
    return false;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static bool GetSaveSlotPayload(std::byte **header, std::byte **payload, std::int32_t slot) {
    if (slot == -1)
        return false;
    if (slot < -1)
        return false;
    if (slot >= 3)
        return false;
    *header = g_gameSaveData + 0x6a0;
    *payload = g_gameSaveData + 0x6e0;
    return true;
}

// @verified - d
void RestoreGameSaveSlot(std::uint32_t slot) {
    std::byte *header = 0;
    std::byte *payload = 0;
    if (GetSaveSlotPayload(&header, &payload, static_cast<std::int32_t>(slot)))
        RestoreSavedGamePayload(header, payload);
}

void ReloadLocalSaveSlots() {
    std::memset(g_cachedBackupSlots, 0, sizeof(g_cachedBackupSlots));
    for (std::int32_t slot = 0; slot < 3; ++slot)
        ReadLocalSaveMetadata(slot, g_cachedBackupSlots + slot);
}

#else

// @verified - p
void RestoreGameSaveSlot(std::uint32_t slot) {
    if (slot <= 9U)
        DecodeRuntimePayload(activeSaveImage);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t LoadGameFromSaveSlot(std::int32_t slot, std::int32_t restorePlacement) {
    const std::uint32_t configuration = GetS2GameConfigurationFlags();
    ReinitScenarioAndFlags();
    RestoreSaveSystemSettings(slot);
    std::int64_t result = 0;
    if (ReadSaveDataSection(0, slot) == 0) {
        if (GetTitleVersionOverride() == 0)
            ResetTitleMenuState();
        RestoreSystemSaveProgress();
        if (ReadSaveDataSection(1, slot) == 0) {
            SetRetainedSaveSelection(slot);
            RestoreGameSaveSlot(static_cast<std::uint32_t>(slot));
            if (restorePlacement != 0)
                ActivateSavedAreaJumpLocation();
            result = 1;
        }
    }
    SetS2GameConfigurationFlags(configuration);
    playerStats.gameConfigurationFlags = configuration;
    return result;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t InitializeNewGameFromSaveSlot(std::int32_t slot) {
    const std::uint32_t configuration = GetS2GameConfigurationFlags();
    ReinitScenarioAndFlags();
    InitializeDefaultSaveData();
    if (slot != -1) {
        if (ReadSaveDataSection(0, slot))
            InitializeDefaultSaveData();
        else
            RestoreSystemSaveProgress();
    }
    SetS2GameConfigurationFlags(configuration);
    playerStats.gameConfigurationFlags = configuration;
    return 1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2Save_ResetNewGameState() {
    (void)InitializeNewGameFromSaveSlot(-1);
    pendingPlayerTransform = {};
    pendingPlayerState = false;
    g_actNumber = 1;
    g_currentMap = SHEN_NAME(0004);
    g_entryPoint = 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const std::array<S2SaveSlotInfo, S2SaveSlotCount> &S2Save_RefreshSlotCatalog() {
    for (std::size_t slot = 0; slot < slotCatalog.size(); ++slot) {
        S2SaveSlotInfo &catalog = slotCatalog[slot];
        catalog = {};
        const std::filesystem::path path = S2Save_SlotPath(slot);
#if defined(SHENMUE_STEAM)
        catalog.occupied = ::shenmue::d3t::SteamFileExists(path.filename().string().c_str());
#else
        std::error_code error;
        catalog.occupied = std::filesystem::exists(path, error) && !error;
#endif
        S2SaveImage image{};
        catalog.result = S2Save_Read(path, image);
        if (catalog.result == S2SaveResult::Success)
            catalog.data = SaveDataInfo(image);
    }
    slotCatalogInitialized = true;
    return slotCatalog;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const std::array<S2SaveSlotInfo, S2SaveSlotCount> &S2Save_GetSlotCatalog() noexcept {
    return slotCatalog;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2Save_HasContinueSlot() {
    if (!slotCatalogInitialized)
        S2Save_RefreshSlotCatalog();
    return FirstEligibleSlot(false) >= 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void LCDM_Idle(void *) {
    // PC v1.07 enqueues LCDM with nullsub_1.  The task owns its parsed
    // section directory; callers update individual LCD images explicitly.
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

const std::byte *FindLcdmSection(std::span<const std::byte> resource, std::uint32_t name0, std::uint32_t name1) noexcept {
    if (resource.empty())
        return nullptr;
    const auto address = reinterpret_cast<std::uintptr_t>(resource.data());
    const std::size_t alignment = static_cast<std::size_t>((32U - (address & 31U)) & 31U);
    if (alignment > resource.size())
        return nullptr;
    resource = resource.subspan(alignment);
    if (resource.size() < 12)
        return nullptr;
    std::uint32_t count{};
    std::uint32_t directoryOffset{};
    std::memcpy(&count, resource.data() + 4, sizeof(count));
    std::memcpy(&directoryOffset, resource.data() + 8, sizeof(directoryOffset));
    if (directoryOffset > resource.size() || count > (resource.size() - directoryOffset) / 16U) {
        return nullptr;
    }
    for (std::uint32_t index = 0; index < count; ++index) {
        const std::size_t entryOffset = static_cast<std::size_t>(directoryOffset) + index * 16U;
        std::uint32_t entryName0{};
        std::uint32_t entryName1{};
        std::uint32_t dataOffset{};
        std::memcpy(&entryName0, resource.data() + entryOffset, sizeof(entryName0));
        std::memcpy(&entryName1, resource.data() + entryOffset + 4, sizeof(entryName1));
        if (entryName0 != name0 || entryName1 != name1)
            continue;
        std::memcpy(&dataOffset, resource.data() + entryOffset + 12, sizeof(dataOffset));
        return dataOffset < resource.size() ? resource.data() + dataOffset : nullptr;
    }
    return nullptr;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

constexpr std::uint32_t LcdmName(char a, char b, char c, char d) noexcept {
    return make_shen_name(a, b, c, d);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void SetLcdmResourcePointer(LcdmTaskData &data, std::size_t offset, const std::byte *pointer) noexcept {
    std::memcpy(data.resourceState.data() + offset, &pointer, sizeof(pointer));
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

const std::byte *GetLcdmResourcePointer(const LcdmTaskData &data, std::size_t offset) noexcept {
    const std::byte *pointer{};
    std::memcpy(&pointer, data.resourceState.data() + offset, sizeof(pointer));
    return pointer;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool LcdmSectionHasBytes(std::span<const std::byte> resource, const std::byte *section, std::size_t size) noexcept {
    if (!section || section < resource.data())
        return false;
    const std::size_t offset = static_cast<std::size_t>(section - resource.data());
    return offset <= resource.size() && size <= resource.size() - offset;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void BuildLcdmResourceState(LcdmTaskData &data, std::span<const std::byte> resource) noexcept {
    constexpr std::array<LcdmSectionBinding, 26> bindings{{
        {LcdmName('S', 'H', 'E', 'N'), LcdmName('M', 'U', 'E', ' '), 0},
        {LcdmName('P', 'K', ' ', ' '), LcdmName(' ', ' ', ' ', ' '), 8},
        {LcdmName('L', 'O', 'A', 'D'), LcdmName('_', 'J', 'P', ' '), 16},
        {LcdmName('L', 'O', 'A', 'D'), LcdmName('_', 'U', 'S', ' '), 24},
        {LcdmName('S', 'A', 'V', 'E'), LcdmName('_', 'J', 'P', ' '), 32},
        {LcdmName('S', 'A', 'V', 'E'), LcdmName('_', 'U', 'S', ' '), 40},
        {LcdmName('O', 'K', '_', 'J'), LcdmName('P', ' ', ' ', ' '), 48},
        {LcdmName('O', 'K', '_', 'U'), LcdmName('S', ' ', ' ', ' '), 56},
        {LcdmName('N', 'G', '_', 'J'), LcdmName('P', ' ', ' ', ' '), 64},
        {LcdmName('N', 'G', '_', 'U'), LcdmName('S', ' ', ' ', ' '), 72},
        {LcdmName('R', 'S', 'M', '_'), LcdmName('J', 'P', ' ', ' '), 80},
        {LcdmName('R', 'S', 'M', '_'), LcdmName('U', 'S', ' ', ' '), 88},
        {LcdmName('R', 'S', 'M', '_'), LcdmName('O', 'K', ' ', ' '), 96},
        {LcdmName('R', 'S', 'M', '_'), LcdmName('N', 'G', ' ', ' '), 104},
        {LcdmName('B', 'T', '_', 'S'), LcdmName('T', 'A', 'R', 'T'), 128},
        {LcdmName('B', 'T', '_', 'C'), LcdmName('O', 'U', 'N', 'T'), 136},
        {LcdmName('B', 'T', '_', 'E'), LcdmName('N', 'D', ' ', ' '), 144},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'K', 'B', ' '), 1432},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'K', 'F', ' '), 1440},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'B', 'A', ' '), 1448},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'B', 'B', ' '), 1456},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'B', 'X', ' '), 1464},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'B', 'Y', ' '), 1472},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'B', 'L', ' '), 1480},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'B', 'R', ' '), 1488},
        {LcdmName('W', 'A', 'Z', 'A'), LcdmName('_', 'B', 'P', ' '), 1496},
    }};
    for (const LcdmSectionBinding &binding : bindings) {
        SetLcdmResourcePointer(data, binding.stateOffset, FindLcdmSection(resource, binding.name0, binding.name1));
    }

    constexpr std::array<std::size_t, 8> moveOrder{{1464, 1448, 1440, 1432, 1480, 1456, 1472, 1496}};
    for (std::size_t index = 0; index < moveOrder.size(); ++index) {
        SetLcdmResourcePointer(data, 1504 + index * sizeof(const std::byte *), GetLcdmResourcePointer(data, moveOrder[index]));
        data.resourceState[1568 + index] = static_cast<std::byte>(index);
    }

    std::array<const std::byte *, 10> largeDigits{};
    std::array<const std::byte *, 10> smallDigits{};
    for (std::size_t digit = 0; digit < 10; ++digit) {
        const std::uint32_t suffix = LcdmName(static_cast<char>('0' + digit), ' ', ' ', ' ');
        largeDigits[digit] = FindLcdmSection(resource, LcdmName('B', 'T', '_', 'L'), suffix);
        smallDigits[digit] = FindLcdmSection(resource, LcdmName('B', 'T', '_', 'S'), suffix);
    }

    for (std::size_t digit = 0; digit < 10; ++digit) {
        const std::byte *const source = largeDigits[digit];
        if (!LcdmSectionHasBytes(resource, source, 46))
            continue;
        for (std::size_t index = 0; index < 22; ++index) {
            const std::uint16_t value =
                static_cast<std::uint16_t>((std::to_integer<std::uint8_t>(source[2 + index * 2]) << 8) | std::to_integer<std::uint8_t>(source[3 + index * 2]));
            const std::uint16_t half = value >> 1;
            const std::uint16_t sixteenth = value >> 4;
            data.resourceState[152 + digit * 44 + index * 2] = static_cast<std::byte>(half >> 8);
            data.resourceState[153 + digit * 44 + index * 2] = static_cast<std::byte>(half);
            data.resourceState[592 + digit * 44 + index * 2] = static_cast<std::byte>(sixteenth >> 8);
            data.resourceState[593 + digit * 44 + index * 2] = static_cast<std::byte>(sixteenth);
        }
    }

    for (std::size_t digit = 0; digit < 10; ++digit) {
        const std::byte *const source = smallDigits[digit];
        if (!LcdmSectionHasBytes(resource, source, 12))
            continue;
        for (std::size_t index = 0; index < 10; ++index) {
            const std::uint16_t value = std::to_integer<std::uint8_t>(source[2 + index]);
            const std::uint16_t eight = static_cast<std::uint16_t>((value << 3) & 0x7ffU);
            const std::uint16_t thirtyTwo = static_cast<std::uint16_t>((value << 5) & 0x1fffU);
            data.resourceState[1032 + digit * 20 + index * 2] = static_cast<std::byte>(eight >> 8);
            data.resourceState[1033 + digit * 20 + index * 2] = static_cast<std::byte>(eight);
            data.resourceState[1232 + digit * 20 + index * 2] = static_cast<std::byte>(thirtyTwo >> 8);
            data.resourceState[1233 + digit * 20 + index * 2] = static_cast<std::byte>(thirtyTwo);
        }
    }
    std::memset(data.resourceState.data() + 152, 0, 44);
    std::memset(data.resourceState.data() + 1032, 0, 20);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void LcdmSelectMessage(std::uintptr_t message) noexcept {
    LcdmTaskData *const data = lcdmTask ? static_cast<LcdmTaskData *>(HLib::GetTaskParameter(lcdmTask)) : nullptr;
    if (!data)
        return;
    static constexpr std::uint8_t idle[]{0xff, 0, 0, 0, 0, 0, 0, 0};
    static constexpr std::uint8_t training[]{0xf0, 0xe0, 0x20, 0, 0, 0x20, 0xf0, 0xe0, 0x20, 0xff, 0, 0, 0, 0, 0, 0};
    static constexpr std::uint8_t completion[]{
        0xf0, 0xe0, 8, 0xf2, 0xe4, 8, 0xf4, 0xe8, 8, 0xf6, 0xec, 8, 0xf8, 0xf0, 8, 0xfa, 0xf4, 8, 0xfc, 0xf8, 8, 0xfe, 0xfc, 8, 0xff, 0, 0, 0, 0, 0, 0, 0};
    static constexpr const std::uint8_t *commands[]{idle, training, completion};
    const std::uintptr_t command = message < 0x10000 ? reinterpret_cast<std::uintptr_t>(commands[message]) : message;
    constexpr std::uint32_t active = 1;
    std::memcpy(data->screen.data(), &active, sizeof(active));
    std::memcpy(data->screen.data() + 8, &command, sizeof(command));
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void LcdmInit(std::uintptr_t image) noexcept {
    LcdmTaskData *const data = lcdmTask ? static_cast<LcdmTaskData *>(HLib::GetTaskParameter(lcdmTask)) : nullptr;
    if (!data)
        return;
    const std::size_t stateOffset = static_cast<std::size_t>(image) * sizeof(const std::byte *);
    const std::byte *const section = image < 0x10000 ?
        (stateOffset + sizeof(const std::byte *) <= data->resourceState.size() ? GetLcdmResourcePointer(*data, stateOffset) : nullptr) :
        reinterpret_cast<const std::byte *>(image);
    if (!section)
        return;
    const std::byte *const source = section + 2;
    constexpr std::size_t pixelOffset = 0x1c;
    for (std::size_t byte = 0; byte < 192; ++byte) {
        const std::uint8_t bits = std::to_integer<std::uint8_t>(source[byte]);
        for (std::size_t bit = 0; bit < 8; ++bit) {
            data->screen[pixelOffset + byte * 8 + bit] = static_cast<std::byte>((bits & (0x80U >> bit)) != 0 ? 8 : 0);
        }
    }
    constexpr std::uint32_t initialized = 1;
    std::memcpy(data->screen.data() + 0x18, &initialized, sizeof(initialized));
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void QueueLcdmTask() {
    lcdmTask = HLib::EnqueueTaskWithParameter(LCDM_Idle, 2, 2, sizeof(LcdmTaskData), SHEN_NAME(LCDM));
    LcdmTaskData *const data = lcdmTask ? static_cast<LcdmTaskData *>(HLib::GetTaskParameter(lcdmTask)) : nullptr;
    if (!data || !g_lcdMemory || !g_lcdMemorySize) {
        if (lcdmTask)
            HLib::DestroyTask(lcdmTask);
        lcdmTask = nullptr;
        return;
    }
    *data = {};
    data->nativeBufferSize = 8240;
    data->resource = g_lcdMemory;
    BuildLcdmResourceState(*data, {g_lcdMemory, g_lcdMemorySize});
    LcdmInit(0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if SHENMUE_IS_PC
// @verified - p
const std::byte *GetWindowGlyphBitmap(std::uint16_t glyph) {
    const FontRuntimeData *const font = GetFontRuntimeData();
    if ((glyph & 0x8080U) == 0x8080U || (glyph & 0xff00U) == 0x100U)
        return static_cast<const std::byte *>(font->kanaData) + 30U * static_cast<std::uint8_t>(glyph);
    const WindowTaskData *const window = static_cast<const WindowTaskData *>(HLib::GetTaskParameter(windowTask));
    const std::int32_t index = 94 * (glyph >> 8U) + static_cast<std::uint8_t>(glyph) - 3135;
    return static_cast<const std::byte *>(window->customFlag != 0 || font->ryouName[0] == '\0' ? font->kanjiData : font->ryouData) + 45 * index;
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if SHENMUE_IS_PC
void AllocateWindowBackBuffer(GeneratedTextureSurface *surface) {
    if (surface->slots[1] == -1)
        surface->slots[1] = static_cast<std::int16_t>(AllocateGeneratedTextureSlot(surface->width, surface->height, WINDOW_SURFACE_FORMAT));
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if SHENMUE_IS_PC
// @verified - p
void RasterizeWindowGlyphShadow(WindowSlot *window, std::uint16_t glyph, std::int16_t column, std::int16_t row) {
    const std::byte *const bitmap = GetWindowGlyphBitmap(glyph);
    const std::int32_t columns = glyph < 0x2100U ? 16 : 24;
    for (std::int32_t bit = 0; bit < 15 * columns; ++bit) {
        if (!(std::to_integer<std::uint8_t>(bitmap[bit >> 3]) & (1U << (bit & 7))))
            continue;
        std::uint8_t *output = window->shadowPixels + 608 * (bit / columns + 17 * row) + 13 * column + bit % columns;
        const std::uint8_t *kernel = windowGlyphShadowKernel.data();
        for (std::int32_t y = 0; y < 7; ++y) {
            for (std::int32_t x = 0; x < 13; ++x)
                *output++ |= *kernel++;
            output += 596;
        }
    }
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if SHENMUE_IS_PC
// @verified - p
void UploadWindowShadowSurface(WindowSlot *window, GeneratedTextureSurface *surface, std::uint16_t foreground, std::uint16_t shadow) {
    std::uint16_t palette[256];
    palette[0] = 0;
    shadow &= 0x0fffU;
    for (std::uint32_t index = 1; index < 16; ++index)
        palette[index] = shadow | ((index & 0xeU) << 11U);
    for (std::uint32_t index = 16; index < 128; ++index)
        palette[index] = shadow | windowShadowAlphaBands[index >> 4U];
    for (std::uint32_t index = 128; index < 256; ++index)
        palette[index] = foreground;
    const std::uint8_t *source = window->shadowPixels;
    std::uint32_t *output = static_cast<std::uint32_t *>(GetGeneratedSurfacePixels(surface));
    for (std::int32_t y = 0; y < 38; ++y) {
        for (std::int32_t x = 0; x < 608; ++x) {
            const std::uint32_t value = palette[*source++];
            *output++ = 17U * ((value & 15U) + ((((value >> 4U) & 15U) + ((((value >> 8U) & 15U) + ((value >> 12U) << 8U)) << 8U)) << 8U));
        }
        output += 416;
    }
    CommitGeneratedSurface(surface);
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if SHENMUE_IS_PC
void UpdateWindowRow(WindowSlot *window, WindowTaskData *, std::int32_t line, std::int32_t physicalLine) {
    std::int32_t center = 0;
    if (!(g_windowParticleDebugFlags & 0x200)) {
        const std::uint8_t first = window->lineWidths[0], second = window->lineWidths[1];
        if (line != 0) {
            if (first > second)
                center = first - second;
        } else if (first < second)
            center = second - first;
        center /= 2;
    }
    std::uint8_t *flags = window->glyphFlags + line * window->columns;
    const std::uint8_t *bytes = window->glyphBytes + line * window->columns;
    std::int32_t column = 0;
    while (column < window->columns) {
        std::int32_t count = 1;
        if (*flags & 0x80U) {
            *flags &= 0x7fU;
            std::uint16_t glyph;
            if (*flags & 1U) {
                count = 2;
                if (flags[1] & 2U) {
                    flags[1] &= 0x7fU;
                    glyph = static_cast<std::uint16_t>((bytes[0] << 8U) | bytes[1]);
                } else
                    glyph = 0x120;
            } else
                glyph = bytes[0] | 0x100U;
            RasterizeWindowGlyphShadow(window, glyph, static_cast<std::int16_t>((column + center) % 46), static_cast<std::int16_t>(physicalLine % 2));
        }
        column += count;
        flags += count;
        bytes += count;
    }
    std::uint16_t shadow;
    switch (window->colourMode) {
    case 0:
    case 2:
        shadow = 1585;
        break;
    case 1:
        shadow = 7;
        break;
    case 3:
        shadow = 865;
        break;
    default:
        return;
    }
    UploadWindowShadowSurface(window, *window->surface, 0xffffU, shadow);
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

#if SHENMUE_IS_PC
// @verified - p
void DrawWindowMessageSprites(WindowSlot *window) {
    window->lineWidths.data()[window->row] = static_cast<std::uint8_t>(window->column);
    const std::uint8_t second = window->lineWidths[1];
    const std::uint8_t longest = window->lineWidths[0] > second ? window->lineWidths[0] : second;
    const std::int32_t x = 320 - ((13 * longest) >> 1), y = second ? 369 : 403;
    std::int32_t line = window->firstLine;
    for (std::int32_t index = 0; index < window->rows; ++index) {
        if ((window->lineFlags[line] & 1U) && g_wndMessageDrawDisabled == 0) {
            LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0, 0, 1, 1}, .reservedTail = 0};
            AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), x, y + 128, 1024, -128, ScreenMode::Centered);
            packet.quad.flags |= 0x400U;
            const GeneratedTextureSurface *const surface = *window->surface;
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(surface->slots[surface->activeSlot ^ 1U]), 0.0f);
        }
        if (++line >= window->rows)
            line = 0;
    }
}
#endif

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void WindowTaskEntry_Callback(void *parameter) {
#if SHENMUE_IS_PC
    WindowTaskData *const data = static_cast<WindowTaskData *>(parameter);
    for (WindowSlot &window : data->slots) {
        if (!(window.flags & 1U))
            continue;
        bool swapped = false;
        if (window.callback)
            window.callback(&window, WindowCallbackEvent::Tick, window.callbackState);
        if (!*window.surface)
            continue;
        if (window.flags & 4U) {
            window.lineWidths.data()[window.row] = static_cast<std::uint8_t>(window.column);
            std::int32_t line = window.firstLine, count = 0;
            while (count < window.rows && !(window.lineFlags[line] & 2U)) {
                if (++line >= window.rows)
                    line = 0;
                ++count;
            }
            if (count < window.rows) {
                UpdateWindowRow(&window, data, count, line);
                window.lineFlags[line] ^= 6U;
            }
            if (count == window.rows) {
                window.flags ^= 0xcU;
                if (window.callback)
                    window.callback(&window, WindowCallbackEvent::GlyphRevealComplete, window.callbackState);
            }
        }
        if ((window.flags & 0x18U) == 0x18U) {
            window.flags = (window.flags ^ 0x18U) | 2U;
            for (std::int32_t line = 0; line < window.rows; ++line) {
                if (window.lineFlags[line] & 4U) {
                    if (!swapped) {
                        (*window.surface)->activeSlot ^= 1U;
                        swapped = true;
                    }
                    window.lineFlags[line] ^= 4U;
                    window.lineFlags[line] |= 1U;
                } else if (window.lineFlags[line] & 8U)
                    window.lineFlags[line] &= 0xf6U;
            }
            if (window.callback)
                window.callback(&window, WindowCallbackEvent::PageRotationComplete, window.callbackState);
        }
        if ((window.flags & 0x22U) == 0x22U)
            DrawWindowMessageSprites(&window);
    }
#else
    (void)parameter;
#endif
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
HLib::Task *QueueWindowTask(std::uint8_t nextFunctionIndex) {
    windowTask = HLib::EnqueueTaskWithParameter(WindowTaskEntry_Callback, nextFunctionIndex, 2, sizeof(WindowTaskData), WINDOW_TASK_TOKEN);
#if SHENMUE_IS_PC
    WindowTaskData *const data = static_cast<WindowTaskData *>(HLib::GetTaskParameter(windowTask));
#else
    WindowTaskData *const data = windowTask ? static_cast<WindowTaskData *>(HLib::GetTaskParameter(windowTask)) : nullptr;
    if (!data)
        return windowTask;
#endif
#if SHENMUE_IS_PC
    data->name = WINDOW_TASK_MARKER;
    for (WindowSlot &slot : data->slots)
        slot.flags = 0;
#else
    data->name = WINDOW_TASK_MARKER;
    data->activeSlot0 = 0;
    data->activeSlot1 = 0;
    data->activeSlot2 = 0;
    data->activeSlot3 = 0;
    data->activeSlot4 = 0;
#endif
    data->customFlag = 1;
    return windowTask;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
WindowSlot *AllocateWindowSlot(HLib::Task *task, const WindowSpecification &specification) {
    if (!task)
        task = windowTask;
    WindowTaskData *const owner = static_cast<WindowTaskData *>(HLib::GetTaskParameter(task));
    WindowSlot *slot = owner->slots.data();
    for (std::int32_t index = 0; index < 5 && slot->flags != 0; ++index)
        ++slot;
    slot->flags = 1;
    slot->firstLine = 0;
    slot->horizontalGap = specification.horizontalGap == -1 ? 1 : specification.horizontalGap;
    slot->verticalGap = specification.verticalGap == -1 ? 1 : specification.verticalGap;
    const std::int32_t width = specification.columns * (slot->horizontalGap + 12);
    slot->x = static_cast<float>(specification.x);
    slot->y = static_cast<float>(specification.y);
    slot->columns = specification.columns;
    slot->rows = specification.rows;
    slot->field40 = specification.field06;
    slot->field42 = specification.field08;
    slot->width = specification.width == -1 ? static_cast<std::int16_t>(width) : specification.width;
    slot->height = specification.height;
    if (specification.height == -1) {
        const std::int16_t rowHeight = static_cast<std::int16_t>(slot->verticalGap + 15);
        slot->height = static_cast<std::int16_t>(slot->rows * rowHeight);
    }
    slot->lineWidth = specification.lineWidth == -1 ? static_cast<std::int16_t>(width) : specification.lineWidth;
    slot->lineHeight = specification.lineHeight == -1 ? 15 : specification.lineHeight;
    std::int32_t powerOfTwo = 8;
    while (powerOfTwo < width)
        powerOfTwo *= 2;
    slot->u = specification.flags & 1U ? 1.0f : static_cast<float>(width) / static_cast<float>(powerOfTwo);
    slot->bytePitch = static_cast<std::int16_t>(4 * powerOfTwo);
    slot->v = 0.9375f;
    slot->textureHeight = 16;
    slot->allocation = HLib::AllocateTaskData(slot->rows * (2 * slot->columns + 1) + 23112, WINDOW_SLOT_ALLOCATION);
    slot->surface = static_cast<GeneratedTextureSurface **>(slot->allocation);
    slot->shadowPixels = static_cast<std::uint8_t *>(slot->allocation) + 8;
    const std::int32_t cells = slot->rows * slot->columns;
    slot->glyphFlags = slot->shadowPixels + 23104;
    slot->glyphBytes = slot->glyphFlags + cells;
    slot->lineFlags = slot->glyphBytes + cells;
    slot->callbackState = nullptr;
    slot->callback = nullptr;
    slot->colourMode = 0;
    *slot->surface = AllocateSHAD_SRUF(1024, 64, 2, 5);
    if (*slot->surface)
        AllocateWindowBackBuffer(*slot->surface);
    ClearWindowSlot(slot);
    return slot;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void ClearWindowSlot(WindowSlot *window) {
    window->firstLine = 0;
    for (std::int32_t row = 0; row < window->rows; ++row)
        window->lineFlags[row] = 0;
    const std::size_t cells = static_cast<std::size_t>(window->rows * window->columns);
    std::memset(window->glyphFlags, 128, cells);
    std::memset(window->glyphBytes, 32, cells);
    window->column = 0;
    window->row = 0;
    for (std::int32_t row = 0; row < window->rows; ++row)
        window->lineFlags[row] |= 8U;
    std::memset(window->shadowPixels, 0, 23104);
    if (window->callback)
        window->callback(window, WindowCallbackEvent::WindowCleared, window->callbackState);
    window->lineWidths[0] = window->lineWidths[1] = 0;
    if ((window->flags & 4U) && window->callback)
        window->callback(window, WindowCallbackEvent::GlyphRevealComplete, window->callbackState);
    window->flags &= 0xfffffff3U;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void ClearWindowText(WindowSlot *window) {
    const std::size_t cells = static_cast<std::size_t>(window->rows * window->columns);
    std::memset(window->glyphFlags, 128, cells);
    std::memset(window->glyphBytes, 32, cells);
    window->column = 0;
    window->row = 0;
    for (std::int32_t row = 0; row < window->rows; ++row)
        window->lineFlags[row] |= 8U;
    std::memset(window->shadowPixels, 0, 23104);
    if (window->callback)
        window->callback(window, WindowCallbackEvent::WindowCleared, window->callbackState);
    window->lineWidths[0] = window->lineWidths[1] = 0;
    if ((window->flags & 4U) && window->callback)
        window->callback(window, WindowCallbackEvent::GlyphRevealComplete, window->callbackState);
    window->flags &= 0xfffffff3U;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void AttachWindowCallback(WindowSlot *window, WindowCallback callback, void *state) {
    window->callback = callback;
    window->callbackState = state;
    if (callback)
        callback(window, WindowCallbackEvent::CallbackAttached, state);
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void AdvanceWindowLine(WindowSlot *window) {
    window->lineWidths.data()[window->row++] = static_cast<std::uint8_t>(window->column);
    window->column = 0;
    if (window->row > window->rows)
        window->row = window->rows;
    if (window->callback)
        window->callback(window, WindowCallbackEvent::LineAdvanced, window->callbackState);
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row) {
#else
bool GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row) {
#endif
    *column = window->column;
    *row = window->row;
    return window->column >= 0 && window->column < window->columns && window->row >= 0 && window->row < window->rows;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
std::int32_t WriteWindowGlyphByte(WindowSlot *window, std::uint8_t flags, std::uint8_t value) {
    if (window->flags & 0xcU)
        return 0x8001;
    if (window->row >= window->rows)
        return 0x8000;
    while ((flags & 1U) && window->column > window->columns - 2) {
        AdvanceWindowLine(window);
        if (window->row >= window->rows)
            return 0x8000;
    }
    const std::int32_t cell = window->column + window->row * window->columns;
    window->glyphFlags[cell] = flags | 0x80U;
    window->glyphBytes[cell] = value;
    std::int32_t physicalLine = window->firstLine + window->row;
    if (physicalLine >= window->rows)
        physicalLine -= window->rows;
    window->lineFlags[physicalLine] &= 0xf7U;
    window->lineFlags[physicalLine] |= 2U;
    if (++window->column >= window->columns)
        AdvanceWindowLine(window);
    return 0;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
std::uint16_t ConvertWindowCharacterToHalfwidth(std::uint16_t character) {
    if (character < 0x100U)
        return character;
    const std::uint16_t code = character & 0x7f7fU;
    if (static_cast<std::uint16_t>(code - 9008U) <= 9U || static_cast<std::uint16_t>(code - 9025U) <= 25U || static_cast<std::uint16_t>(code - 9057U) <= 25U)
        return code - 0x2300U;
    if (static_cast<std::uint16_t>(code - 9505U) <= 85U) {
        const std::uint8_t kana = windowHalfwidthKana[code - 9505U];
        return kana | (((static_cast<std::uint32_t>(kana) - 110U) & 0xfcU) != 0 || kana == 111 ? 0x8080U : 0xa580U);
    }
    for (std::size_t index = 0; index < windowHalfwidthSymbols.size(); index += 2)
        if (code == (windowHalfwidthSymbols[index] | 0x2100U))
            return windowHalfwidthSymbols[index + 1];
    return code | 0x8080U;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
std::int32_t WriteWindowCharacter(WindowSlot *window, std::uint16_t character, std::int32_t encodingMode) {
    if (encodingMode == 2) {
        character = ConvertWindowCharacterToHalfwidth(character);
        encodingMode = 1;
    }
    if (character < 0x100U) {
        if (character == 10) {
            AdvanceWindowLine(window);
            return 1;
        }
        return WriteWindowGlyphByte(window, 0, static_cast<std::uint8_t>(character));
    }
    if (encodingMode == 1)
        character &= 0x7f7fU;
    else {
        std::uint8_t high = static_cast<std::uint8_t>(character >> 8U), low = static_cast<std::uint8_t>(character);
        if (high >= 224U)
            high -= 64U;
        high = static_cast<std::uint8_t>(2U * high + 31U);
        if (low >= 128U)
            --low;
        low -= 31U;
        if (low >= 127U) {
            low -= 94U;
            ++high;
        }
        character = static_cast<std::uint16_t>((high << 8U) | low);
    }
    const std::int32_t result = WriteWindowGlyphByte(window, 1, static_cast<std::uint8_t>(character >> 8U));
    return result ? result : WriteWindowGlyphByte(window, 2, static_cast<std::uint8_t>(character));
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
std::uint16_t ConvertWindowShiftJisCharacter(std::uint16_t character) {
    std::uint8_t high = static_cast<std::uint8_t>(character >> 8U), low = static_cast<std::uint8_t>(character);
    if (high >= 224U)
        high -= 64U;
    high = static_cast<std::uint8_t>(2U * high + 31U);
    if (low >= 128U)
        --low;
    low -= 31U;
    if (low >= 127U) {
        low -= 94U;
        ++high;
    }
    return static_cast<std::uint16_t>((high << 8U) | low);
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void RecoverWindowSurface(WindowSlot *window) {
    if (!*window->surface) {
        *window->surface = AllocateSHAD_SRUF(1024, 64, 2, 5);
        if (*window->surface)
            AllocateWindowBackBuffer(*window->surface);
    }
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void HideWindowSlot(WindowSlot *window) {
    window->flags &= ~0x20U;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void ShowWindowSlot(WindowSlot *window) {
    window->flags |= 0x30U;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void RevealWindowGlyphs(WindowSlot *window) {
    window->flags |= 0x24U;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#if SHENMUE_IS_PC
// @verified - p
void SetWindowColourMode(WindowSlot *window, std::int32_t mode) {
    window->colourMode = mode;
}
#endif

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void wndEnableMessageDraw() {
    g_wndMessageDrawDisabled = 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void wndDisableMessageDraw() {
    g_wndMessageDrawDisabled = 1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void RasterizeWindowGlyph(std::uint16_t glyph, void *destination, std::int32_t bytePitch, std::int32_t byteOffset) {
#if SHENMUE_IS_PC
    const std::byte *const bitmap = GetWindowGlyphBitmap(glyph);
#else
    const FontRuntimeData *const font = GetFontRuntimeData();
    const std::byte *bitmap;
    if ((glyph & 0x8080U) == 0x8080U || (glyph & 0xff00U) == 0x100U)
        bitmap = static_cast<const std::byte *>(font->kanaData) + 30U * static_cast<std::uint8_t>(glyph);
    else {
        const WindowTaskData *const window = static_cast<const WindowTaskData *>(HLib::GetTaskParameter(windowTask));
        const std::int32_t index = 94 * (glyph >> 8U) + static_cast<std::uint8_t>(glyph) - 3135;
        bitmap = static_cast<const std::byte *>(window->customFlag != 0 || font->ryouName[0] == '\0' ? font->kanjiData : font->ryouData) + 45 * index;
    }
#endif
    std::uint8_t coverage[400]{};
    const bool kana = glyph < 0x2100U || (glyph & 0x8080U) == 0x8080U;
    const std::uint32_t columns = kana ? 16U : 24U;
    for (std::uint32_t bit = 0; bit < 15U * columns; ++bit) {
        if ((std::to_integer<std::uint8_t>(bitmap[bit >> 3U]) & (1U << (bit & 7U))) == 0)
            continue;
        const std::uint32_t row = bit / columns;
        const std::uint32_t column = bit % columns;
        coverage[25U * row + column] |= 2U;
        coverage[25U * row + column + 1U] |= 1U;
        coverage[25U * (row + 1U) + column] |= 1U;
        coverage[25U * (row + 1U) + column + 1U] |= 1U;
    }
    std::uint32_t colours[4];
    for (std::size_t index = 0; index < g_windowGlyphPalette.size(); ++index) {
        const std::uint32_t value = g_windowGlyphPalette[index];
        colours[index] = 17U * ((value & 15U) + (((((value >> 4U) & 15U) + ((((value >> 8U) & 15U) + ((value >> 12U) << 8U)) << 8U))) << 8U));
    }
    std::byte *row = static_cast<std::byte *>(destination) + 15 * bytePitch + byteOffset;
    for (std::uint32_t y = 0; y < 16; ++y) {
        for (std::uint32_t x = 0; x < (kana ? 13U : 25U); ++x)
            std::memcpy(row + 4U * x, &colours[coverage[25U * y + x]], 4);
        row -= bytePitch;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint64_t RenderWindowTextSurface(
    HLib::Task *task, const char *, std::uint32_t width, std::int16_t colour, std::int16_t shadow, std::int32_t, std::int32_t centered) {
    if (!task)
        task = windowTask;
    (void)HLib::GetTaskParameter(task);
    if (std::bit_cast<std::int32_t>(width) < 12)
        return 0xffffffffULL;
    GeneratedTextureSurface *const surface = AllocateSHAD_SRUF(width, 16, 2, 5);
    if (!surface)
        return 0xffffffffULL;
    void *const pixels = GetGeneratedSurfacePixels(surface);
    std::memset(pixels, 0, 2U * width * 16U);
    g_windowGlyphPalette = {0, static_cast<std::uint16_t>(shadow), static_cast<std::uint16_t>(colour), static_cast<std::uint16_t>(colour)};
    // PC v1.07 loads "." unconditionally, not the supplied legacy string.
    const std::uint32_t glyphX = centered ? (width - 12U) / 2U : 0U;
    RasterizeWindowGlyph(0x12eU, pixels, static_cast<std::int32_t>(4U * width), static_cast<std::int32_t>(4U * glyphX));
    CommitGeneratedSurface(surface);
    return static_cast<std::uint32_t>(RetireGeneratedSurface(surface));
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t SetupUIElement(HLib::Task *task, const char *text, std::uint32_t width, std::int16_t colour, std::int16_t shadow, std::int32_t conversionMode) {
    return RenderWindowTextSurface(task, text, width, colour, shadow, conversionMode, 1);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t CreateLeftAlignedWindowTextSurface(
    HLib::Task *task, const char *text, std::uint32_t width, std::int16_t colour, std::int16_t shadow, std::int32_t conversionMode) {
    return RenderWindowTextSurface(task, text, width, colour, shadow, conversionMode, 0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Native wndSetFlag: a null task selects the global WNDT task.
void wndSetFlag(HLib::Task *task, std::int32_t flag) {
    if (!task)
        task = windowTask;
    WindowTaskData *const data = static_cast<WindowTaskData *>(HLib::GetTaskParameter(task));
#if SHENMUE_IS_PC
    data->customFlag = static_cast<std::uint32_t>(flag);
#else
    if (data)
        data->customFlag = static_cast<std::uint32_t>(flag);
#endif
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void LoadAreaSpriteImages() {
    titleFileSelect.imageTask = 0;
    titleFileSelect.imageSprite = 0xffffffffU;
    titleFileSelect.field0cc = 0xffffffffU;
    void *allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "T_IMG.SPR", TITLE_IMAGE_SPRITE_FILE);
    if (allocation) {
        titleFileSelect.imageTask = reinterpret_cast<std::uintptr_t>(allocation);
        titleFileSelect.imageSprite = ReadTexnTexture(allocation);
        titleFileSelect.field0cc = 0xffffffffU;
    }
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "T_AREA.SPR", TITLE_AREA_SPRITE_FILE);
    if (allocation) {
        titleFileSelect.areaTask = reinterpret_cast<std::uintptr_t>(allocation);
        titleFileSelect.field0c4 = ReadTexnTexture(allocation);
    }
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "T_LAREA.SPR", TITLE_LOCAL_AREA_SPRITE_FILE);
    if (allocation) {
        titleFileSelect.localAreaTask = reinterpret_cast<std::uintptr_t>(allocation);
        titleFileSelect.field0c8 = static_cast<std::int32_t>(ReadTexnTexture(allocation));
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SetTitleWarningLogo(std::int32_t logoIndex, const void *parameters) noexcept {
    titleWarningLogo = logoIndex < 47 ? logoIndex : 0;
    if (parameters) {
        std::memcpy(titleWarningLogoParameters.data(), parameters, titleWarningLogoParameters.size());
        titleWarningUsesLogoParameters = 1;
    } else {
        titleWarningUsesLogoParameters = 0;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void LoadTitleWarningLogo(TitleWarningTaskData *data, std::int32_t texture, std::int32_t x, std::int32_t y) {
    if (data->textureIndex != texture) {
        if (data->textureIndex >= 0) {
            FreeTempSprites(data->textureIndex);
            data->textureIndex = -1;
            return;
        }
        if (AllocateTempSprites(texture) != 1)
            return;
        BindTempSprite(texture, 1, 34, TITLE_WARNING_DECORATION_RECTANGLES[0].data());
    }
    data->textureIndex = texture;
    DrawTempSpriteSlot(static_cast<std::uint32_t>(texture) << 16U, (static_cast<std::uint32_t>(x) << 16U) | static_cast<std::uint16_t>(y), 13.0f, 0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void DrawTitleWarningDecorations(std::int32_t logo, const std::byte *parameters) {
    if (logo == 1 || logo == 4) {
        std::int32_t number;
        std::memcpy(&number, parameters + 4, sizeof(number));
        if (number > 999)
            return;
        std::int32_t digits[3];
        std::int32_t count = 0;
        if (number / 100 > 0) {
            count = 3;
            digits[2] = number / 100;
            number %= 100;
        }
        if (number / 10 > 0) {
            digits[1] = number / 10;
            if (!count)
                count = 2;
            number %= 10;
        } else if (count == 3)
            digits[1] = 0;
        if (!count)
            count = 1;
        digits[0] = number % 10;
        std::uint32_t position = static_cast<std::uint32_t>(TITLE_WARNING_DIGIT_X[count - 1]) << 16U;
        for (std::int32_t digit = 0; digit < count; ++digit) {
            DrawTempSpriteDigit(
                (static_cast<std::uint32_t>(TITLE_WARNING_LOGOS[logo][1]) << 16U) | 1U, position | (logo == 1 ? 236U : 264U), 14.0f, digits[digit]);
            position -= 18U << 16U;
        }
    } else if (logo == 28) {
        DrawTempSpriteSlot(0x220000U | (std::to_integer<std::uint8_t>(parameters[2]) + 10U), 0x12e00dcU, 14.0f, 0);
        DrawTempSpriteSlot(0x220000U | (std::to_integer<std::uint8_t>(parameters[3]) + 1U), 0x1d900dcU, 14.0f, 0);
    } else if (logo == 33) {
        DrawTempSpriteSlot(0x220000U | std::to_integer<std::uint8_t>(parameters[8]), 0x0e10134U, 14.0f, 0);
    } else if (logo == 36) {
        DrawTempSpriteSlot(0x220000U | std::to_integer<std::uint8_t>(parameters[8]), 0x13100bfU, 14.0f, 0);
        DrawTempSpriteSlot(0x220000U | std::to_integer<std::uint8_t>(parameters[9]), 0x0ee0110U, 14.0f, 0);
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void FreeTitleWarningResources() {
    ReleaseBorrowedLegacySpriteSlot(37);
    ReleaseBorrowedLegacySpriteSlot(36);
    FreeTempSprites(0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SubmitTitleWarningLogo(void *) {
    TitleWarningTaskData *const data = static_cast<TitleWarningTaskData *>(HLib::GetTaskParameter(titleWarningTask));
    const std::int16_t category = TITLE_WARNING_LOGOS[titleWarningLogo][0];
    if (titleWarningUsesLogoParameters) {
        if (category >= 0) {
            LoadTitleWarningLogo(data, TITLE_WARNING_LOGOS[titleWarningLogo][1], 320, TITLE_WARNING_LOGO_Y[category]);
            DrawTitleWarningDecorations(titleWarningLogo, titleWarningLogoParameters.data());
            // Indexed TEMP ownership and sprite/digit slot primitives require the excluded legacy_sprite interface.
            if (category == 3)
                DrawTitleSelection(320, 162, 90, std::to_integer<std::uint8_t>(titleWarningLogoParameters[1]));
        }

    } else if (category >= 0)
        LoadTitleWarningLogo(data, TITLE_WARNING_LOGOS[titleWarningLogo][1], 320, TITLE_WARNING_LOGO_Y[category]);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void AdvanceTitleWarningColour() noexcept {
    constexpr std::uint32_t maximumStep = 0xff000000U;
    if (titleWarningCurrentColour <= titleWarningDesiredColour) {
        std::uint32_t difference = titleWarningDesiredColour - titleWarningCurrentColour;
        if (difference > maximumStep)
            difference = maximumStep;
        titleWarningCurrentColour += difference;
    } else {
        std::uint32_t difference = titleWarningCurrentColour - titleWarningDesiredColour;
        if (difference > maximumStep)
            difference = maximumStep;
        titleWarningCurrentColour -= difference;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void TitleWarningCallback(void *) {
    if (titleWarningPhase != TitleWarningPhase::Inactive) {
        if (titleWarningPhase == TitleWarningPhase::Begin) {
            titleWarningPhase = TitleWarningPhase::Visible;
        } else if (titleWarningPhase == TitleWarningPhase::Visible) {
            if (titleWarningLogo <= 0) {
                titleWarningPhase = TitleWarningPhase::Tail;
                titleWarningTailFrames = 4;
                titleWarningDesiredColour = 0;
            } else {
                HLib::ExecuteCallbackNow(SubmitTitleWarningLogo, nullptr);
            }
        } else if (titleWarningPhase == TitleWarningPhase::Tail && --titleWarningTailFrames < 0) {
            titleWarningPhase = TitleWarningPhase::Inactive;
        }
    } else if (titleWarningLogo > 0) {
        titleWarningDesiredColour = titleWarningBaseColour;
        titleWarningPhase = TitleWarningPhase::Begin;
    }

    AdvanceTitleWarningColour();
    const bool visibleForParameters = titleWarningUsesLogoParameters == 0 || titleWarningLogoParameters[0] != std::byte{};
    if (titleWarningCurrentColour != 0 && visibleForParameters)
        vfSubmitQuad(titleWarningCurrentColour, 6.0f);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void QueueTitleWarningTask() {
    titleWarningPhase = TitleWarningPhase::Inactive;
    titleWarningLogo = 0;
    titleWarningTask = HLib::EnqueueTaskWithParameter(TitleWarningCallback, 0x0f, 0, sizeof(TitleWarningTaskData), TITLE_WARNING_TASK);
    TitleWarningTaskData *const data = static_cast<TitleWarningTaskData *>(HLib::GetTaskParameter(titleWarningTask));
    if (data)
        data->textureIndex = -1;
    titleWarningBaseColour = 0x76000000U;
    titleWarningDesiredColour = 0;
    titleWarningCurrentColour = 0;
    titleWarningUsesLogoParameters = 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void QueueTitleSaveDependencies() {
    QueueLcdmTask();
    QueueWindowTask();
    WRN_CtrlWarning_Enqueue();
    QueueTitleWarningTask();
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetRetainedSaveSelection(std::int32_t slot) {
    g_retainedSaveSelection = slot;
}

std::int32_t GetRetainedSaveSelection() {
    return g_retainedSaveSelection;
}

std::int32_t ClampSaveValue(std::int32_t value, std::int32_t minimum, std::int32_t maximum) {
    return value < minimum ? minimum : value > maximum ? maximum : value;
}

#else

// @verified - p
void SetRetainedSaveSelection(std::int32_t selection) noexcept {
    retainedSaveSelection = selection;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void ResetPendingPlayerPersistentState() noexcept {
    pendingPlayerPersistentState = {};
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void ReinitScenarioAndFlags() {
    ResetScenarioPersistentFlags();
    ResetScenarioGlobals();
    InitializePlayerRuntime();
    ResetNewGameConfiguration();
    ResetNativePlayerStats();
    InitItems();
    InitCalendar();
    ResetArcadeRankings();
    InitializeNpcFlags();
    doorQueueAuthorForMap({});
    SetMagicRoomActiveFlag(0);
    ResetAreaJumpWarpFlag();
    SetScenarioFollowCharacter({});
    ResetTemporaryAreaJumpAuthor();
    ResetNaviSavedData();
    ResetMemoPageState();
    SetRetainedSaveSelection(-1);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t ReadS1SaveDataByIndex(void *bank, std::int32_t slot) {
    return ReadBackupFile(S1_BACKUP_SLOT_NAMES[static_cast<std::uint32_t>(slot)], bank, 0, S1SaveFileSize);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
bool ValidateS1SaveHeader(const void *header) noexcept {
    const S1ImportedSaveHeader &data = *static_cast<const S1ImportedSaveHeader *>(header);
    return data.signature == S1SaveSignature && data.version == S1SaveVersion;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int32_t FindInheritedMoveIndex(std::int32_t category, std::int32_t move) noexcept {
    if (static_cast<std::uint32_t>(category) - 1U > 2U)
        return -1;
    const std::span<const std::uint8_t> ids = InheritedMoveIds[category];
    if (move >= static_cast<std::int32_t>(ids.size()))
        return -1;
    for (std::size_t index = 0; index < ids.size(); ++index) {
        if (move == static_cast<std::int32_t>(ids[index]) - InheritedMoveSourceOffsets[category])
            return static_cast<std::int32_t>(index);
    }

    // @verified - p
    return -1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void InitializeInheritedMoveProficiency() {
    for (std::int32_t category = 1; category < 4; ++category) {
        const std::span<const std::int8_t> minimum = InheritedMoveProficiency[category];
        for (std::int32_t move = 0; move < static_cast<std::int32_t>(minimum.size()); ++move) {
            const std::int32_t index = FindInheritedMoveIndex(category, move);
            if (index > 0 && minimum[index] != 0)
                SetMoveProficiency(0, static_cast<std::uint8_t>(category), static_cast<std::int16_t>(index), minimum[index]);
        }
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t ResetItemInventoryState() noexcept {
    inventoryState.type0.fill(std::byte{});
    inventoryState.type1.fill(std::byte{});
    inventoryState.type2.fill(std::byte{});
    inventoryResetEntries.fill(0xff00U);
    inventoryState.type4.fill(std::byte{});
    inventoryState.type3.fill(std::byte{});
    inventoryState.type5.fill(std::byte{});
    return 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t ImportS1SaveData(const void *bank, std::int32_t slot) {
    const std::byte *const bytes = static_cast<const std::byte *>(bank);
    const S2SaveDataInfo &info =
        *reinterpret_cast<const S2SaveDataInfo *>(bytes + S1SaveSlotInfoOffset + static_cast<std::ptrdiff_t>(slot) * sizeof(S2SaveDataInfo));
    const S1ImportedPlayerData &data =
        *reinterpret_cast<const S1ImportedPlayerData *>(bytes + S1SaveSystemSize + static_cast<std::ptrdiff_t>(slot) * S1SavePlayerStride);
    DateTime date{.year = static_cast<std::int8_t>(info.gameYear),
        .month = static_cast<std::int8_t>(info.gameMonth),
        .day = static_cast<std::int8_t>(info.gameDay),
        .week = info.gameWeek,
        .hour = 12,
        .minute = 30,
        .second = 0};
    SetCurrentDateTimeTo(date);
    AddDaysAndCalculateWeekdayFromCurrentDate(20);
    playerStats.savedStat0 = data.savedStat0;
    playerStats.runtimeFlags = data.runtimeFlags;
    playerStats.cash = data.cash;
    SetCurrentCash(playerStats.cash);
    playerStats.segaCoins = data.segaCoins;
    playerStats.alternateHealth = data.alternateHealth;
    playerStats.defaultHealth = data.defaultHealth;
    playerStats.reactionCameraScale = data.reactionCameraScale;
    playerStats.assistScale = data.assistScale;
    playerStats.gameConfigurationFlags = data.gameConfigurationFlags;
    SetS2GameConfigurationFlags(data.gameConfigurationFlags);
    playerStats.contextActionMode = data.contextActionMode;
    retainedImportedPlayerOption = data.playerOption;
    g_savedFightBlurFlag = data.blurFlag;
    playerStats.transferredYen = data.cash;
    playerStats.cash = static_cast<std::int32_t>(static_cast<float>(playerStats.transferredYen) * 0.033333335f);
    SetCurrentCash(playerStats.cash);
    for (std::int32_t category = 1; category < 4; ++category) {
        const S1ImportedMoveBank &moves = data.moves[category - 1];
        const std::span<const std::int8_t> minimum = InheritedMoveProficiency[category];
        for (std::int32_t entry = 0; entry < moves.count; ++entry) {
            const std::int32_t move = moves.moveIds[entry];
            if (move < 0)
                continue;
            const std::int32_t index = FindInheritedMoveIndex(category, move);
            if (index < 0)
                continue;
            std::int32_t proficiency = moves.proficiency[entry];
            if (proficiency < 1)
                proficiency = 1;
            if (index < static_cast<std::int32_t>(minimum.size()) && proficiency < minimum[index])
                proficiency = minimum[index];
            SetMoveProficiency(0, static_cast<std::uint8_t>(category), static_cast<std::int16_t>(index), static_cast<std::int16_t>(proficiency));
        }
    }
    std::memcpy(GetScenarioPersistentScratch(), data.scenarioScratch.data(), data.scenarioScratch.size());
    ResetItemInventoryState();
    ImportS1CapsuleStock(data.capsuleStock.data());
    return ImportS1KeyItems(data.keyItems.data());
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void ImportSelectedS1SaveData(const void *bank, std::int32_t slot) {
    if (slot >= 0)
        ImportS1SaveData(bank, std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(slot) + 1U));
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
std::int32_t MeasureSaveText(const std::uint8_t *text, std::int32_t font) {
    std::int32_t width = 0;
    std::uint32_t high;
    while ((high = *text++) != 0) {
        std::uint8_t low = *text++;
        std::int32_t advance = 14;
        const SaveGlyphRecord *glyph = saveTaskData->glyphTables[font];
        for (; glyph->code; ++glyph) {
            if ((high << 8) + low == glyph->code) {
                advance = static_cast<std::int32_t>(static_cast<float>(glyph->width) * saveTextScale);
                break;
            }
        }
        width += advance;
    }
    return width;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void DrawSaveText(std::int32_t x, std::int32_t y, const char *source, std::int32_t font) {
    std::uint32_t high;
    while ((high = *reinterpret_cast<const std::uint8_t *>(source++)) != 0) {
        std::uint8_t low = *reinterpret_cast<const std::uint8_t *>(source++);
        std::int32_t advance = 14;
        const SaveGlyphRecord *glyph = saveTaskData->glyphTables[font];
        std::uint32_t code = low + (high << 8);
        for (; glyph->code; ++glyph) {
            if (code == glyph->code) {
                DrawSaveGlyph(x, y, glyph, font);
                advance = static_cast<std::int32_t>(static_cast<float>(glyph->width) * saveTextScale);
                break;
            }
        }
        x += advance;
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetScenarioSaveRestoreState(std::int32_t state) {
    g_scenarioSaveRestoreState = state;
}

std::int32_t GetScenarioSaveRestoreState() {
    return g_scenarioSaveRestoreState;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetSceneInfos(std::int32_t actNumber, shen_name mapName, std::int32_t entryPoint) {
    savedSceneActNumber = actNumber;
    savedSceneMapName = mapName;
    savedSceneEntryPoint = entryPoint;
}

#else

void SetSceneInfos(std::int32_t actNumber, shen_name mapName, std::int32_t entryPoint) noexcept {
    savedSceneInfo.actNumber = actNumber;
    savedSceneInfo.mapName = mapName;
    savedSceneInfo.entryPoint = entryPoint;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void GetSceneInfos(std::int32_t *actNumber, shen_name *mapName, std::int32_t *entryPoint) {
    *actNumber = savedSceneActNumber;
    *mapName = savedSceneMapName;
    *entryPoint = savedSceneEntryPoint;
}

#else

void GetSceneInfos(std::int32_t *actNumber, shen_name *mapName, std::int32_t *entryPoint) noexcept {
    *actNumber = savedSceneInfo.actNumber;
    *mapName = savedSceneInfo.mapName;
    *entryPoint = savedSceneInfo.entryPoint;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void CloseSaveScreen() noexcept {
    if (currentSaveState == 0 || !saveTaskData)
        return;
    saveTaskData->preRenderCallback = SaveScreenFadeOut;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Native LoadSave close step: retire the save screen, then hand the sleep-action icon back from its Save submenu.
void CloseSaveScreenFromLoadSave() noexcept {
    CloseSaveScreen();
    ResumeSleepActionIconAfterSave();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void EnableSaveScreenAdmission() noexcept {
    g_saveScreenAdmission = 1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void SetSaveFrontendMode(std::int32_t mode) noexcept {
    g_saveFrontendMode = mode;
    if (mode != 0)
        SetTitleMenuTransitionCallback(nullptr);
    else
        SetTitleMenuUIState(1);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2SaveScreen_SelectSlot(std::size_t slot) noexcept {
    if (!saveTaskData || slot >= S2SaveSlotCount)
        return;
    saveTaskData->selectedSlot = static_cast<std::int32_t>(slot);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2SaveScreen_ConfirmSlot(std::size_t slot) noexcept {
    if (currentSaveState == 0 || slot >= S2SaveSlotCount)
        return false;
    S2SaveScreen_SelectSlot(slot);
    if (saveScreenPrompt == SaveScreenPrompt::Result) {
        DismissMessage();
        saveScreenPrompt = SaveScreenPrompt::None;
        return true;
    }
    if (saveScreenPrompt == SaveScreenPrompt::Overwrite) {
        slot = saveScreenPromptSlot;
        DismissMessage();
    } else {
        const auto &catalog = S2Save_RefreshSlotCatalog();
        if (catalog[slot].occupied) {
            saveScreenPrompt = SaveScreenPrompt::Overwrite;
            saveScreenPromptSlot = slot;
            ShowMessage("UISAVECONF", true); // @text EN: "Are you sure you want to save?" | JA: "セーブしてもよろしいですか？"
            return true;
        }
    }

    SetItemSelectorDelayedResult(32);
    const S2SaveResult result = S2Save_WriteSlot(slot, currentSaveState == 6 ? 6 : 1);
    saveScreenStorageStatus = result == S2SaveResult::Success ? 0 : -5;
    saveScreenPrompt = SaveScreenPrompt::Result;
    saveScreenPromptSlot = slot;
    ShowMessage(result == S2SaveResult::Success ?
            "UISAVEOK" :
            "UISAVEERRDS", // @text UISAVEOK EN: "Saved successfully" | JA: "セーブが完了しました。" ; UISAVEERRDS EN: "Save failed. No storage space available" | JA: "空きスペースがないため、セーブできません。"
        true);
    return true;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2SaveScreen_CancelPrompt() noexcept {
    if (saveScreenPrompt == SaveScreenPrompt::None)
        return false;
    DismissMessage();
    saveScreenPrompt = SaveScreenPrompt::None;
    return true;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2SaveScreen_HasPrompt() noexcept {
    return saveScreenPrompt != SaveScreenPrompt::None;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2SaveTask_Begin(std::size_t slot, std::uint8_t clearState, S2SaveTaskCompletion completion) {
    if (slot >= S2SaveSlotCount || currentSaveState != 0 || !saveTask || !saveTask->activeCallback || saveOperation.phase != SaveTaskPhase::Idle) {
        return false;
    }
    auto *const data = static_cast<SaveTaskData *>(HLib::GetTaskParameter(saveTask));
    if (!data || data->updateCallback != SaveTaskIdle)
        return false;
    saveOperation.phase = SaveTaskPhase::Confirm;
    saveOperation.result = S2SaveResult::Success;
    saveOperation.slot = slot;
    saveOperation.clearState = clearState;
    saveOperation.completion = completion;
    data->preRenderCallback = SaveTaskAcquireInput;
    data->updateCallback = SaveTaskProcessInput;
    return true;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
TitleFileSelectState *InitializeTitleFileSelectState() {
    titleFileSelect.selectedSlot = -1;
    titleFileSelect.field004 = 0;
    titleFileSelect.field002 = 0;
    titleFileSelect.field0dc = -1;
    titleFileSelect.field0e0 = 0xffffffffU;
    S2SaveDataInfo *const data = titleFileSelect.titleSequence ? GetBackupSaveDataInfo(0) : GetS1BackupSaveDataInfo(0);
    for (std::int8_t index = 0; index < 10; ++index) {
        TitleFileSelectSlot &slot = titleFileSelect.slots[index];
        slot.flags = 0;
        slot.data = &data[index];
        if (data[index].saveKind != 0) {
            slot.flags = data[index].clearState == 6 ? 3U : data[index].clearState == 4 ? 5U : 1U;
            if (data[index].saveKind != 0 && (titleFileSelect.titleSequence != 0 || data[index].clearState == 6)) {
                if (titleFileSelect.field004 == 0) {
                    titleFileSelect.selectedSlot = index;
                    titleFileSelect.field0dc = index > 7 ? index - 7 : 0;
                    titleFileSelect.field0e0 = index > 7 ? 7 : index;
                }
                ++titleFileSelect.field004;
            }
        }
    }
    titleFileSelect.field0cc = 0xffffffffU;
    titleFileSelect.field0d0 = (titleFileSelect.field0d0 & 0xffffffff00000000ULL) | 0xffffffffU;
    return &titleFileSelect;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void TitleContinueControl(void *) {
    SetTitleWarningLogo(0, nullptr);
    InitializeTitleFileSelectState();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
TitleFileSelectState *AssignActiveTCRL(TitleControlData *control) {
    activeTitleControl = control;
    TitleContinueControl(control);
    return &titleFileSelect;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t StartTitleLoadGame() {
    if (LoadGameFromSaveSlot(titleFileSelect.selectedSlot, 1) == 0)
        return 0;
    DisableFilmBarAndUpdateFramePacing();
    FlushCurrentMessageAndRenderQueue();
    titleFileSelect.state = 2;
    HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
    titleFileSelect.titleSequence2 = 0;
    HLib::SetupTaskDelay(10);
    S2SaveDataInfo *const data = GetBackupSaveDataInfo(0);
    const std::int32_t act = data[titleFileSelect.selectedSlot].actNumber;
    if (act != g_loadedActNumber) {
        SetNextSceneNumber(act);
        titleFileSelect.state = 3;
        HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
        titleFileSelect.titleSequence2 = 0;
        HLib::SetupTaskDelay(10);
    }
    titleFileSelect.titleSequence2 = 0;
    const S2SaveDataInfo &selected = data[titleFileSelect.selectedSlot];
    g_actNumber = selected.sceneNumber;
    g_currentMap = selected.mapName;
    g_entryPoint = selected.entryPoint;
    return 1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t StartTitleCollectionGame() {
    if (LoadGameFromSaveSlot(titleFileSelect.selectedSlot, 0) == 0)
        return 30;
    titleFileSelect.state = g_loadedActNumber != 4;
    HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
    titleFileSelect.titleSequence2 = 0;
    HLib::SetupTaskDelay(10);
    return 10;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t StartTitleCompletedGame() {
    if (InitializeNewGameFromSaveSlot(titleFileSelect.selectedSlot) == 0)
        return 4;
    InitializeInheritedMoveProficiency();
    titleFileSelect.state = g_loadedActNumber == 1 ? 4 : 5;
    HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
    titleFileSelect.titleSequence2 = 0;
    HLib::SetupTaskDelay(10);
    titleFileSelect.titleSequence2 = 0;
    return 10;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void StartTitleImportedGame(TitleControlData *control) {
    void *const bank = control->inheritedSaveBank;
    if (ReadS1SaveDataByIndex(bank, titleFileSelect.selectedSlot) == 0 && BupReady(0) == 0) {
        if (ValidateS1SaveHeader(static_cast<const std::byte *>(bank) + 0x680) && InitializeNewGameFromSaveSlot(titleFileSelect.selectedSlot) != 0) {
            ImportSelectedS1SaveData(bank, 0);
            titleFileSelect.titleSequence2 = 0;
            titleFileSelect.state = g_loadedActNumber == 1 ? 4 : 5;
            HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
            titleFileSelect.titleSequence2 = 0;
            HLib::SetupTaskDelay(10);
        }
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint8_t ClearTitleLoadScenarioFlags() {
    for (std::int32_t flag = 1011; flag <= 1017; ++flag)
        ClearOrSetShenmueFlags(flag, 0);
    return ClearOrSetShenmueFlags(1018, 0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void TFSEL_ModChange_Callback(void *) {
    SystemModule module = SystemModule::Title;
    switch (titleFileSelect.state) {
    case 0:
        g_actNumber = 4;
        g_currentMap = TITLE_AREA_OMAK;
        g_entryPoint = 0;
        ResetGlobalStateFlags();
        module = SystemModule::Game;
        break;
    case 1:
        SetSceneInfos(4, TITLE_AREA_OMAK, 0);
        S2Save_CheckData();
        SetNextSceneNumber(4);
        module = SystemModule::DiskChange;
        break;
    case 2:
        ApplyTitleMenuOptions();
        ClearTitleLoadScenarioFlags();
        module = SystemModule::Game;
        break;
    case 3:
        ApplyTitleMenuOptions();
        ClearTitleLoadScenarioFlags();
        module = SystemModule::DiskChange;
        break;
    case 4:
        g_actNumber = 1;
        g_currentMap = TITLE_AREA_0004;
        g_entryPoint = 0;
        ApplyTitleMenuOptions();
        module = SystemModule::Game;
        break;
    case 5:
        SetSceneInfos(1, TITLE_AREA_0004, 0);
        S2Save_CheckData();
        SetNextSceneNumber(1);
        module = SystemModule::DiskChange;
        break;
    case 6:
        g_actNumber = 1;
        g_currentMap = SNAPSHOT_DEBUG_TASK;
        g_entryPoint = 0;
        ResetGlobalStateFlags();
        module = SystemModule::Game;
        break;
    }
    TitleAudioControl(-1);
    g_bPaused = false;
    ExecuteAicaCommand(0x1200a0U, 5, 0);
    ChangeSystemModule(module, 0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t CompleteSaveMenuLoad() {
    SetTitleMenuTransitionCallback(nullptr);
    titleFileSelect.selectedSlot = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(gameLoadSlot));
    const S2SaveDataInfo *const data = GetBackupSaveDataInfo(titleFileSelect.selectedSlot);
    if (saveMenuSource == 1) {
        StartTitleCollectionGame();
        gameLoadStatus = 1;
        return 0;
    }
    if (saveMenuSource == 0) {
        const std::int64_t result = data->clearState == 6 ? StartTitleCompletedGame() : StartTitleLoadGame();
        if (result == 0) {
            gameLoadStatus = 2;
            return 0;
        }
    } else if (saveMenuSource == 2) {
        StartTitleImportedGame(activeTitleControl);
    }
    gameLoadStatus = 1;
    return 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void BeginSaveMenuLoad(std::int32_t slot) {
    ReadSm2Saves(slot);
    gameLoadSlot = slot;
    SetTitleMenuTransitionCallback(CompleteSaveMenuLoad);
    gameLoadActive = 1;
    gameLoadStatus = -1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
bool GetSaveMenuSlotEligibility(std::uint32_t slot) noexcept {
    if (saveMenuSource != 2)
        return true;
    const S2SaveDataInfo *const info = GetS1BackupSaveDataInfo(slot);
    return info && info->clearState == 6;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
TitleFileSelectState *InitializeTitleContinueFlags() {
    titleFileSelect.titleSequence2 = 9;
    titleFileSelect.titleSequence = 1;
    return &titleFileSelect;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
TitleFileSelectState *InitializeTitleNewGameFlags() {
    titleFileSelect.titleSequence2 = 8;
    titleFileSelect.titleSequence = 0;
    return &titleFileSelect;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
TitleFileSelectState *InitializeTitleCollectionFlags() {
    titleFileSelect.titleSequence2 = 9;
    titleFileSelect.titleSequence = 2;
    return &titleFileSelect;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void ResetTitleImportSelection() {
    titleImportSelection = -1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void StartTitleModuleChange(TitleControlData *, std::int32_t state) {
    titleFileSelect.state = state;
    HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
    titleFileSelect.titleSequence2 = 0;
    HLib::SetupTaskDelay(10);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void DrawTitleFileSelectSprite(std::int32_t group, std::int32_t item, const Vector3 &position) {
    if (group <= 12) {
        if (group == 0 && item == 5) {
            DrawTitleSpriteRect(TITLE_SAVE_BOARD_UV[item].data(), position, 10, 1.25f);
        } else if (group == 2) {
            std::uint32_t colour;
            if (item == 0 || item == 3)
                colour = 0xbfffffffU;
            else if (item == 1)
                colour = 0x7f0000ffU;
            else if (item == 2)
                colour = 0x7fff0000U;
            DrawTitleColourSpriteRect(TITLE_SAVE_COLOUR_UV[item].data(), position, 12, 0.5f, colour);
        } else {
            const std::uint16_t *const uv = group == 0 ? TITLE_SAVE_BOARD_UV[item].data() : TITLE_SAVE_CHARACTER_UV[item].data();
            DrawTitleSpriteRect(uv, position, group == 0 ? 10 : 11, 1.0f);
        }
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void DrawTitleFileSelectNumber(std::int32_t value, std::int32_t digits, std::int32_t padZero, float x, float y) {
    Vector3 position{.x = x, .y = y, .z = -2.6000004f};
    for (std::int32_t index = 4 - digits; index < 4; ++index) {
        const std::int8_t digit = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(value / TITLE_SAVE_DECIMAL_DIVISORS[index]));
        value %= TITLE_SAVE_DECIMAL_DIVISORS[index];
        if (digit > 0)
            padZero = 1;
        if (digit > 0 || padZero != 0 || index == 3)
            DrawTitleFileSelectSprite(1, digit, position);
        position.x += 16.0f;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void DrawTitleFileSelectDate(std::int32_t x, std::int32_t y, const void *source) {
    TitleFileSelectDate date;
    std::memcpy(&date, source, sizeof(date));
    const std::int8_t month = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(date.month + 23U));
    Vector3 position{.x = static_cast<float>(x) - 40.0f, .y = static_cast<float>(y), .z = -2.6000004f};
    DrawTitleFileSelectSprite(1, month, position);
    if (month != 28) {
        position.x = static_cast<float>(x) - 10.0f;
        DrawTitleFileSelectSprite(1, 10, position);
    }
    DrawTitleFileSelectNumber(date.day, 2, 0, static_cast<float>(x) + 2.0f, static_cast<float>(y));
    position.x = static_cast<float>(x) + 32.0f;
    DrawTitleFileSelectSprite(1, 11, position);
    DrawTitleFileSelectNumber(date.year >= 100 ? date.year : date.year + 1900, 4, 0, static_cast<float>(x) + 44.0f, static_cast<float>(y));
    DrawTitleFileSelectNumber(date.hour, 2, 1, static_cast<float>(x) + 118.0f, static_cast<float>(y));
    position.x = static_cast<float>(x) + 146.0f;
    DrawTitleFileSelectSprite(1, 12, position);
    DrawTitleFileSelectNumber(date.minute, 2, 1, static_cast<float>(x) + 160.0f, static_cast<float>(y));
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void DrawTitleFileSelectLocalPreview(std::int32_t area) {
    if (area >= 0) {
        if (area != static_cast<std::int32_t>(titleFileSelect.field0d0)) {
            const MapInfoChunk *chunk = reinterpret_cast<const MapInfoChunk *>(titleFileSelect.localAreaTask);
            std::int32_t index = 0;
            if (chunk) {
                while (chunk->name.name != 0) {
                    if (chunk->nextByteOffset == 0)
                        break;
                    if (chunk->name == TITLE_SAVE_TEXN) {
                        if (area == index) {
                            ReplacePtxTexture(static_cast<std::uint32_t>(titleFileSelect.field0c8), chunk);
                            break;
                        }
                        ++index;
                    }
                    chunk = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->nextByteOffset);
                }
            }
            titleFileSelect.field0d0 = (titleFileSelect.field0d0 & 0xffffffff00000000ULL) | static_cast<std::uint32_t>(area);
        }
        if (titleFileSelect.field0c8 >= 0) {
            LegacySpritePacket packet;
            packet.colour = 0xffffffffU;
            packet.secondaryColour = 0;
            packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
            packet.reservedTail = 0;
            packet.quad.flags = 1024;
            AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 512, 235, 128, 32, ScreenMode::Centered);
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(titleFileSelect.field0c8), -2.7f);
        }
    } else {
        titleFileSelect.field0d0 = (titleFileSelect.field0d0 & 0xffffffff00000000ULL) | 0xffffffffU;
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void DrawTitleFileSelectAreaPreview(std::int32_t area) {
    const shen_name pattern = UI_SAVE_AREAS[area].pattern;
    std::int32_t image = -1, location = -1;
    if (pattern == SAVE_MAP_0200)
        image = location = 2;
    else if (pattern == SAVE_MAP_WAT0)
        image = location = 1;
    else if (pattern == SAVE_MAP_KRF1)
        image = location = 3;
    else if (pattern == SAVE_MAP_CLS1) {
        image = 4;
        location = 4;
    } else if (pattern == SAVE_MAP_CLS2) {
        image = 5;
        location = 4;
    } else {
        switch (pattern.characters[0]) {
        case 'A':
            image = location = 0;
            break;
        case 'K':
            image = location = 3;
            break;
        case 'Q':
            image = location = 2;
            break;
        case 'W':
            image = location = 1;
            break;
        }
    }
    if (image < 0) {
        titleFileSelect.field0cc = 0xffffffffU;
        return;
    }
    if (image != static_cast<std::int32_t>(titleFileSelect.field0cc)) {
        if (static_cast<std::int32_t>(titleFileSelect.imageSprite) >= 0) {
            const MapInfoChunk *chunk = reinterpret_cast<const MapInfoChunk *>(titleFileSelect.imageTask);
            std::int32_t index = 0;
            if (chunk) {
                while (chunk->name.name != 0) {
                    if (chunk->nextByteOffset == 0)
                        break;
                    if (chunk->name == TITLE_SAVE_TEXN) {
                        if (image == index) {
                            ReplacePtxTexture(titleFileSelect.imageSprite, chunk);
                            break;
                        }
                        ++index;
                    }
                    chunk = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->nextByteOffset);
                }
            }
            chunk = reinterpret_cast<const MapInfoChunk *>(titleFileSelect.areaTask);
            index = 0;
            if (chunk) {
                while (chunk->name.name != 0) {
                    if (chunk->nextByteOffset == 0)
                        break;
                    if (chunk->name == TITLE_SAVE_TEXN) {
                        if (location == index) {
                            ReplacePtxTexture(titleFileSelect.field0c4, chunk);
                            break;
                        }
                        ++index;
                    }
                    chunk = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->nextByteOffset);
                }
            }
        }
        titleFileSelect.field0cc = static_cast<std::uint32_t>(image);
    }
    if (static_cast<std::int32_t>(titleFileSelect.imageSprite) >= 0) {
        LegacySpritePacket packet;
        packet.colour = 0xffffffffU;
        packet.secondaryColour = 0;
        packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
        packet.reservedTail = 0;
        packet.quad.flags = 1024;
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 512, 149, 100, 100, ScreenMode::Centered);
        LegacySpriteNullsub(packet, titleFileSelect.imageSprite, -2.7f);
        packet.colour = 0xffffffffU;
        packet.secondaryColour = 0;
        packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
        packet.reservedTail = 0;
        packet.quad.flags = 1024;
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 512, 213, 128, 32, ScreenMode::Centered);
        LegacySpriteNullsub(packet, titleFileSelect.field0c4, -2.7f);
        if (UI_SAVE_AREAS[area].image >= 0) {
            DrawTitleFileSelectLocalPreview(area);
            DrawSizedTitleSpriteRect(TITLE_SAVE_MAP_FRAME_UV[UI_SAVE_AREAS[area].image].data(), {.x = 512.0f, .y = 299.0f, .z = -2.7000003f}, 13, 100, 100);
        }
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void DrawTitleFileSelectCard(std::int32_t index, std::int32_t game) {
    const S2SaveDataInfo *const info = titleFileSelect.slots[index].data;
    const std::int32_t relative = static_cast<std::int32_t>(static_cast<std::uint32_t>(index) - titleFileSelect.field0e0);
    const float y = static_cast<float>(relative) * 97.0f;
    if (static_cast<std::uint32_t>(relative) <= 2U) {
        titleFileSelect.sprite = TITLE_SAVE_SPRITES[8];
        titleFileSelect.sprite.position.y = (y - 4.0f) + TITLE_SAVE_SPRITES[8].position.y;
        DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
        titleFileSelect.sprite = TITLE_SAVE_SPRITES[9];
        titleFileSelect.sprite.position.y = (y - 4.0f) + TITLE_SAVE_SPRITES[9].position.y;
        titleFileSelect.sprite.item = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(titleFileSelect.sprite.item + index));
        DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
    }
    const std::uint32_t flags = titleFileSelect.slots[index].flags;
    if (index == titleFileSelect.selectedSlot) {
        if (static_cast<std::uint32_t>(relative) <= 2U) {
            titleFileSelect.sprite = TITLE_SAVE_SPRITES[5];
            titleFileSelect.sprite.position.y = TITLE_SAVE_SPRITES[5].position.y + y;
            DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
        }
        if ((flags & 2) != 0) {
            DrawTitleFileSelectAreaPreview(ui_detail::SaveMenuAreaIndex(game ? SAVE_MAP_CLS2 : SAVE_MAP_CLS1));
        } else {
            const std::int32_t area = ui_detail::SaveMenuAreaIndex(info->mapName);
            if (area >= 0)
                DrawTitleFileSelectAreaPreview(area);
        }
    } else if (static_cast<std::uint32_t>(relative) <= 2U) {
        titleFileSelect.sprite = TITLE_SAVE_SPRITES[5];
        ++titleFileSelect.sprite.item;
        titleFileSelect.sprite.position.y = TITLE_SAVE_SPRITES[5].position.y + y;
        DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
    }
    if ((flags & 1) != 0) {
        if (static_cast<std::uint32_t>(relative) <= 2U) {
            titleFileSelect.sprite = TITLE_SAVE_SPRITES[3];
            titleFileSelect.sprite.position.y = TITLE_SAVE_SPRITES[3].position.y + y;
            DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
            titleFileSelect.sprite = TITLE_SAVE_SPRITES[4];
            titleFileSelect.sprite.position.y = TITLE_SAVE_SPRITES[4].position.y + y;
            DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
            titleFileSelect.sprite = TITLE_SAVE_SPRITES[3];
            ++titleFileSelect.sprite.item;
            titleFileSelect.sprite.position.y = (y + 42.0f) + TITLE_SAVE_SPRITES[3].position.y;
            DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
            titleFileSelect.sprite = TITLE_SAVE_SPRITES[4];
            titleFileSelect.sprite.position.y = (y + 42.0f) + TITLE_SAVE_SPRITES[4].position.y;
            DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
            DrawTitleFileSelectDate(242, TruncateSpriteCoordinate(y + 116.0f), &info->gameYear);
            DrawTitleFileSelectDate(242, TruncateSpriteCoordinate(y + 160.0f), &info->systemYear);
        }
    } else if (static_cast<std::uint32_t>(relative) <= 2U) {
        titleFileSelect.sprite = TITLE_SAVE_SPRITES[7];
        titleFileSelect.sprite.position.y = TITLE_SAVE_SPRITES[7].position.y + y;
        DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
    }
    if (static_cast<std::uint32_t>(relative) <= 2U) {
        titleFileSelect.sprite = TITLE_SAVE_SPRITES[6];
        titleFileSelect.sprite.position.y = TITLE_SAVE_SPRITES[6].position.y + y;
        DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void DrawTitleFileSelectScrollbar() {
    float y = 125.0f;
    for (std::int32_t index = 0; index < 10; ++index) {
        titleFileSelect.sprite = TITLE_SAVE_SPRITES[10];
        titleFileSelect.sprite.position.y = TITLE_SAVE_SPRITES[10].position.y + y;
        DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
        const std::int32_t first = static_cast<std::int32_t>(titleFileSelect.field0e0);
        if (first <= index && first + 3 > index && titleFileSelect.selectedSlot != index) {
            titleFileSelect.sprite = TITLE_SAVE_SPRITES[11];
            titleFileSelect.sprite.position.y = (y + 1.25f) + TITLE_SAVE_SPRITES[11].position.y;
            DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
        } else if (titleFileSelect.selectedSlot == index) {
            titleFileSelect.sprite = TITLE_SAVE_SPRITES[12];
            titleFileSelect.sprite.position.y = (y + 1.25f) + TITLE_SAVE_SPRITES[12].position.y;
            DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
        }
        y += 22.5f;
    }
    titleFileSelect.sprite = TITLE_SAVE_SPRITES[13];
    titleFileSelect.sprite.position.y = (((y - 11.25f) + 1.25f) - 0.5f) + TITLE_SAVE_SPRITES[13].position.y;
    DrawTitleFileSelectSprite(titleFileSelect.sprite.group, titleFileSelect.sprite.item, titleFileSelect.sprite.position);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void TFSEL_DispFileSelect_Callback(void *) {
    std::int32_t flags = (titleFileSelect.titleSequence2 & 8) != 0;
    if ((titleFileSelect.titleSequence2 & 1) != 0) {
        DrawTitleFileSelectSprite(TITLE_SAVE_SPRITES[0].group, TITLE_SAVE_SPRITES[0].item, TITLE_SAVE_SPRITES[0].position);
        DrawTitleFileSelectSprite(TITLE_SAVE_SPRITES[1].group, TITLE_SAVE_SPRITES[1].item, TITLE_SAVE_SPRITES[1].position);
        flags |= 2;
    }
    if ((titleFileSelect.titleSequence2 & 4) != 0) {
        DrawTitleFileSelectSprite(TITLE_SAVE_SPRITES[2].group, TITLE_SAVE_SPRITES[2].item, TITLE_SAVE_SPRITES[2].position);
        for (std::int32_t index = 0; index < 10; ++index)
            DrawTitleFileSelectCard(index, titleFileSelect.titleSequence != 0);
        DrawTitleCaption(titleFileSelect.titleSequence ? 4 : 12, -1);
        DrawTitleFileSelectScrollbar();
    }
    DrawTitleFrame(flags);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void TFSEL_DispFileSelect(void *) {
    if ((titleFileSelect.titleSequence2 & 1) != 0) {
        DrawTitleFileSelectSprite(TITLE_SAVE_SPRITES[0].group, TITLE_SAVE_SPRITES[0].item, TITLE_SAVE_SPRITES[0].position);
        DrawTitleFileSelectSprite(TITLE_SAVE_SPRITES[1].group, TITLE_SAVE_SPRITES[1].item, TITLE_SAVE_SPRITES[1].position);
    }
    DrawTitleFrame(1);
    HLib::SetCurrentTaskTo(TFSEL_DispFileSelect_Callback);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t SelectTitleS1Save() {
    const S2SaveDataInfo *const infos = GetS1BackupSaveDataInfo(0);
    GetLocalBackupInfo();
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    const std::uint16_t input = InterpretTitleAnalogInput() | pressed;
    const std::uint16_t choice = titleFileSelect.field004 > 0 ? input : input & 0xcfffU;
    if ((g_inputBuffer[0].pressedPriority & 0x200) != 0) {
        titleFileSelect.titleSequence2 = 8;
        return 14;
    }
    if ((choice & 0x2000) != 0) {
        do {
            if (++titleFileSelect.selectedSlot >= 10)
                titleFileSelect.selectedSlot = 0;
        } while (
            infos[titleFileSelect.selectedSlot].saveKind == 0 || (titleFileSelect.titleSequence == 0 && infos[titleFileSelect.selectedSlot].clearState != 6));
    } else if ((choice & 0x1000) != 0) {
        do {
            if (--titleFileSelect.selectedSlot < 0)
                titleFileSelect.selectedSlot = 9;
        } while (
            infos[titleFileSelect.selectedSlot].saveKind == 0 || (titleFileSelect.titleSequence == 0 && infos[titleFileSelect.selectedSlot].clearState != 6));
    }
    return (choice & 0xc00) != 0 ? 23 : titleNewGameState;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t SelectTitleS2Save(const TitleFileSelectSelectionStates &states, std::int32_t) {
    const S2SaveDataInfo *const infos = GetBackupSaveDataInfo(0);
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    const std::uint16_t input = InterpretTitleAnalogInput() | pressed;
    if ((input & 0x200) != 0)
        return states.cancelState;
    if ((input & 0x3000) != 0) {
        std::array<std::int32_t, 10> invalid;
        for (std::int32_t index = 0; index < 10; ++index)
            invalid[index] = !(infos[index].saveKind != 0 && (titleFileSelect.titleSequence != 0 || infos[index].clearState == 6));
        const std::int8_t previous = titleFileSelect.selectedSlot;
        if ((input & 0x2000) != 0) {
            do {
                if (++titleFileSelect.selectedSlot >= 10)
                    titleFileSelect.selectedSlot = previous;
            } while (invalid[titleFileSelect.selectedSlot] != 0);
            const std::int8_t next = titleFileSelect.selectedSlot;
            if (previous != next) {
                const std::int32_t distance = std::abs(next - previous);
                if (distance <= 2 - titleFileSelect.field0dc) {
                    if (next <= 9)
                        titleFileSelect.field0dc += distance;
                } else {
                    titleFileSelect.field0dc = 2;
                    titleFileSelect.field0e0 = next > 8 ? 7 : titleFileSelect.selectedSlot - 2;
                }
            }
        } else {
            do {
                if (--titleFileSelect.selectedSlot < 0)
                    titleFileSelect.selectedSlot = previous;
            } while (invalid[titleFileSelect.selectedSlot] != 0);
            const std::int8_t next = titleFileSelect.selectedSlot;
            if (previous != next) {
                const std::int32_t distance = std::abs(next - previous);
                if (distance <= titleFileSelect.field0dc) {
                    if (next >= 0)
                        titleFileSelect.field0dc -= distance;
                } else {
                    titleFileSelect.field0dc = 0;
                    titleFileSelect.field0e0 = next < 1 ? 0 : titleFileSelect.selectedSlot;
                }
            }
        }
    }
    return (input & 0xc00) != 0 ? states.confirmState : titleNewGameState;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t ConfirmTitleS1Save(TitleControlData *control, std::uint32_t, std::uint16_t input) {
    LegacyColourQuad packet;
    packet.quad.flags = 0;
    packet.colour = 0x7f000000U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480, ScreenMode::Centered);
    packet.quad.flags |= 0x400U;
    shenSubmitQuad(packet, -2.5900004f);
    std::int32_t selected = -1;
    if ((input & 0x4000) != 0)
        titleFileSelect.warning.choice = 1;
    if ((input & 0x8000) != 0)
        titleFileSelect.warning.choice = 0;
    if ((input & 0xc00) != 0) {
        selected = titleFileSelect.warning.choice != 1;
        SetTitleWarningLogo(0, nullptr);
        titleFileSelect.warning.choice = 0;
    }
    if (selected != 0) {
        if (selected == 1)
            return 5;
        SetTitleWarningLogo(14, &titleFileSelect.warning);
        return 23;
    }
    void *const bank = control->inheritedSaveBank;
    if (ReadS1SaveDataByIndex(bank, titleFileSelect.selectedSlot) != 0 || BupReady(0) != 0)
        return 24;
    if (!ValidateS1SaveHeader(static_cast<const std::byte *>(bank) + 0x680) || InitializeNewGameFromSaveSlot(titleFileSelect.selectedSlot) == 0)
        return 24;
    ImportSelectedS1SaveData(bank, 0);
    titleFileSelect.titleSequence2 = 0;
    StartTitleModuleChange(control, g_loadedActNumber == 1 ? 4 : 5);
    return 10;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t ConfirmTitleS2Save(TitleControlData *, std::uint32_t, std::uint16_t pressed) {
    LegacyColourQuad packet;
    packet.quad.flags = 0;
    packet.colour = 0x7f000000U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480, ScreenMode::Centered);
    packet.quad.flags |= 0x400U;
    shenSubmitQuad(packet, -2.5900004f);
    const std::uint16_t input = InterpretTitleAnalogInput() | pressed;
    std::int32_t selected = -1;
    if ((input & 0x4000) != 0)
        titleFileSelect.warning.choice = 1;
    if ((input & 0x8000) != 0)
        titleFileSelect.warning.choice = 0;
    if ((input & 0xc00) != 0) {
        selected = titleFileSelect.warning.choice != 1;
        SetTitleWarningLogo(0, nullptr);
        titleFileSelect.warning.choice = 0;
    }
    if (selected != 0) {
        if (selected == 1)
            return 7;
        SetTitleWarningLogo(38, &titleFileSelect.warning);
        return titleNewGameState;
    }
    if (titleFileSelect.titleSequence == 2)
        return StartTitleCollectionGame();
    return (titleFileSelect.slots[titleFileSelect.selectedSlot].flags & 2) != 0 ? StartTitleCompletedGame() : StartTitleLoadGame();
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t ConfirmTitleSceneChange(TitleControlData *, std::uint32_t, std::uint16_t input) {
    const S2SaveDataInfo *const infos = GetBackupSaveDataInfo(0);
    titleFileSelect.warning.act = static_cast<std::uint8_t>(infos[titleFileSelect.selectedSlot].actNumber);
    titleFileSelect.warning.previousAct = titleFileSelect.warning.act;
    LegacyColourQuad packet;
    packet.quad.flags = 0;
    packet.colour = 0x7f000000U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480, ScreenMode::Centered);
    packet.quad.flags |= 0x400U;
    shenSubmitQuad(packet, -2.5900004f);
    const S2SaveDataInfo *const saved = GetBackupSaveDataInfo(0);
    std::int32_t selected = -1;
    if ((input & 0x4000) != 0)
        titleFileSelect.warning.choice = 1;
    if ((input & 0x8000) != 0)
        titleFileSelect.warning.choice = 0;
    if ((input & 0xc00) != 0) {
        selected = titleFileSelect.warning.choice != 1;
        SetTitleWarningLogo(0, nullptr);
        titleFileSelect.warning.choice = 0;
    }
    if (selected != 0) {
        if (selected == 1)
            return 7;
        SetTitleWarningLogo(33, &titleFileSelect.warning);
        return 32;
    }
    SetNextSceneNumber(saved[titleFileSelect.selectedSlot].actNumber);
    titleFileSelect.state = 3;
    HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
    titleFileSelect.titleSequence2 = 0;
    HLib::SetupTaskDelay(10);
    return 10;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void TFSEL_NewGameCtrl_Callback(void *data) {
    TitleControlData *const control = static_cast<TitleControlData *>(data);
    padcCheckControllerConnection();
    (void)GetPublishedItemSelectorOwnedMask();
    const std::uint16_t input = g_inputBuffer[0].pressedPriority;
    const std::uint32_t backup = static_cast<std::uint32_t>(GetLocalBackupInfo());
    const TitleFileSelectStateRule *const rule = titleNewGameState >= 11 ? &TITLE_SAVE_NEW_GAME_RULES[titleNewGameState - 11] : nullptr;
    std::int64_t next;
    switch (titleNewGameState) {
    case 0:
        next = titleImportSelection == -1 ? 3 : 14;
        break;
    case 1:
        if ((backup & 8) != 0) {
            titleFileSelect.titleSequence2 |= 5U;
            next = 5;
        } else
            next = 18;
        break;
    case 3:
    case 14:
        next = StartTitleCompletedGame();
        break;
    case 4:
    case 8:
        ExecuteTitleMenuCallback(control, SystemModule::Test);
        return;
    case 5:
        next = SelectTitleS1Save();
        break;
    case 6:
        if ((backup & 0x7fe00U) != 0) {
            titleFileSelect.titleSequence2 |= 4U;
            next = 7;
        } else
            next = 31;
        break;
    case 7:
        next = SelectTitleS2Save(TITLE_SAVE_SELECTION_STATES, titleFileSelect.titleSequence == 2 ? 0 : 1);
        break;
    case 11:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
    case 21:
    case 22:
    case 24:
    case 25:
    case 26:
    case 27:
    case 29:
    case 30:
    case 31:
        SetTitleWarningLogo(rule->warning, nullptr);
        if ((input & 0xc00) == 0)
            return;
        [[fallthrough]];
    case 12:
        titleNewGameState = rule->nextState;
        SetTitleWarningLogo(0, nullptr);
        titleFileSelect.warning.choice = 0;
        return;
    case 13:
        titleFileSelect.warning.parameter = 0;
        [[fallthrough]];
    case 19:
        next = rule->nextState;
        break;
    case 23:
        next = ConfirmTitleS1Save(control, backup, input);
        break;
    case 28:
        next = ConfirmTitleS2Save(control, backup, input);
        break;
    case 32:
        next = ConfirmTitleSceneChange(control, backup, input);
        break;
    default:
        return;
    }
    titleNewGameState = static_cast<std::int32_t>(next);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void StartTitleNewGame(void *) {
    SetTitleWarningLogo(0, nullptr);
    InitializeTitleFileSelectState();
    titleNewGameState = 0;
    titleFileSelect.titleSequence2 = 8;
    ResetAuthorStateFlags();
    HLib::SetCurrentTaskTo(TFSEL_NewGameCtrl_Callback);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void StartTitleCollection(void *) {
    SetTitleWarningLogo(0, nullptr);
    InitializeTitleFileSelectState();
    titleNewGameState = 6;
    titleFileSelect.titleSequence2 |= 1U;
    HLib::SetCurrentTaskTo(TFSEL_NewGameCtrl_Callback);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void StartTitleSnapshotViewer(void *) {
    SetTitleWarningLogo(0, nullptr);
    InitializeTitleFileSelectState();
    SetTitleWarningLogo(0, nullptr);
    titleFileSelect.state = 6;
    HLib::SetCurrentTaskTo(TFSEL_ModChange_Callback);
    titleFileSelect.titleSequence2 = 0;
    HLib::SetupTaskDelay(10);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2Save_NewGameTitleTask(void *) {
    padcCheckControllerConnection();
    (void)GetPublishedItemSelectorOwnedMask();
    S2Save_ResetNewGameState();
    ChangeSystemModule(SystemModule::Game, 0);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2Save_ContinueTitleTask(void *) {
    RunTitleLoadFlow(false);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void S2Save_TitleLoadTask(void *) {
    RunTitleLoadFlow(true);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int32_t S2Save_GetPendingStartParameters() noexcept {
    return pendingPlayerState ? -1 : 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2Save_QueryPendingPlayerTransform(Vector3 &position, std::int32_t &yaw) noexcept {
    position = pendingPlayerTransform.position;
    yaw = pendingPlayerTransform.yaw;
    return pendingPlayerState;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2Save_ConsumePendingPlayerPersistentState(PlayerPersistentState &state) noexcept {
    if (pendingPlayerPersistentState.present == 0)
        return false;
    state = pendingPlayerPersistentState;
    pendingPlayerPersistentState.present = 0;
    return true;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

bool S2Save_RestorePlayerAtStartup() noexcept {
    Character *const character = g_playerRuntime.character;
    PlayerModelState *const model = g_playerRuntime.model;
    if (!character || !model)
        return false;

    // Area main and the common startup owner may both restore placement.
    // Only the decoded runtime/persistent payload is consumed on the first call.
    SetPlayerRootPosition(pendingPlayerTransform.position);
    shenDecrementRotYAllNodes(&character->entity.node, static_cast<std::int16_t>(pendingPlayerTransform.yaw));
    character->internalState = pendingPlayerTransform.characterState;
    model->currentYaw = static_cast<std::int16_t>(pendingPlayerTransform.yaw);
    model->desiredYaw = model->currentYaw;
    model->committedYaw = model->currentYaw;
    model->yaw = static_cast<std::uint16_t>(model->currentYaw);
    if (model->movementState) {
        model->movementState->currentYaw = model->currentYaw;
        model->movementState->targetYaw = model->currentYaw;
    }
    pendingPlayerState = false;
    return true;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void SaveScreenUpdate(void *taskData) {
    SaveTaskData *save = static_cast<SaveTaskData *>(taskData);
    if (save->initializationMode != 0) {
        save->inputBuffer = g_inputBuffer;
    } else if (currentSaveState == 1) {
        const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(save->padData);
        saveScreenInput.pressedPriority = input->pressedPriority | g_inputBuffer[3].pressedPriority;
        save->inputBuffer = &saveScreenInput;
    } else {
        save->inputBuffer = ctrlGetPadInputBuffer0(save->padData);
    }
    save->preRenderCallback(taskData);
    float *const fade = &save->fadeCurrent;
    const float difference = static_cast<float>(save->fadeTarget) - *fade;
    const float previous = *fade;
    *fade = difference * (currentSaveState != 7 ? 0.2f : 0.07f) + previous;
    if (0.0f > save->fadeCurrent)
        save->fadeCurrent = 0.0f;
    if (save->fadeCurrent > 255.0f)
        save->fadeCurrent = 255.0f;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
#if SHENMUE_S2_GCC295
    packet.colour = static_cast<std::uint32_t>(static_cast<std::int32_t>(save->fadeCurrent)) << 24U;
#else
    packet.colour = static_cast<std::uint32_t>(TruncateToInt32(save->fadeCurrent)) << 24U;
#endif
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480);
    packet.quad.flags |= 0x400U;
    DrawLegacyColourPacket(packet, 6.0f);
    WRN_SetBaseColour(0);
}

#else

namespace {

// @verified - p
void SaveScreenUpdate(void *taskData) {
    SaveTaskData *const save = static_cast<SaveTaskData *>(taskData);
    if (save->initializationMode != 0) {
        save->inputBuffer = g_inputBuffer.data();
    } else if (currentSaveState == 1) {
        const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(save->padData);
        saveScreenInput.pressedPriority = input->pressedPriority | g_inputBuffer[3].pressedPriority;
        save->inputBuffer = &saveScreenInput;
    } else {
        save->inputBuffer = ctrlGetPadInputBuffer0(save->padData);
    }
    save->preRenderCallback(save);

    const float rate = currentSaveState == 7 ? 0.07f : 0.2f;
    save->fadeCurrent = (static_cast<float>(save->fadeTarget) - save->fadeCurrent) * rate + save->fadeCurrent;
    if (save->fadeCurrent < 0.0f)
        save->fadeCurrent = 0.0f;
    if (save->fadeCurrent > 255.0f)
        save->fadeCurrent = 255.0f;
    LegacyColourQuad packet;
    packet.quad.flags = 0;
    packet.colour = static_cast<std::uint32_t>(TruncateSpriteCoordinate(save->fadeCurrent)) << 24U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480, ScreenMode::Adjusted);
    packet.quad.flags |= 0x400U;
    shenSubmitQuad(packet, 6.0f);
    WRN_SetBaseColour(0);
    d3t::SetSceneClearColour(0xff000000U);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void FinishSaveTask(SaveTaskData &save, S2SaveResult result) {
    const std::size_t slot = saveOperation.slot;
    DismissMessage();
    if (save.initializationMode == 0)
        BindPadControlEntry(save.previousPadData);
    save.previousPadData = nullptr;
    saveOperation.phase = SaveTaskPhase::Idle;
    save.updateCallback = SaveTaskIdle;
    save.preRenderCallback = SaveTaskAcquireInput;
    const S2SaveTaskCompletion completion = saveOperation.completion;
    saveOperation.completion = nullptr;
    if (completion)
        completion(result, slot);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void SaveTaskCallback(void *taskData) {
    auto *const save = static_cast<SaveTaskData *>(taskData);
    if (!save)
        return;

    if (saveOperation.phase == SaveTaskPhase::Idle)
        return;
    if (saveOperation.phase == SaveTaskPhase::Confirm) {
        if (ConsumeCancelInput(*save->inputBuffer)) {
            FinishSaveTask(*save, S2SaveResult::Cancelled);
            return;
        }
        if (!ConsumeConfirmInput(*save->inputBuffer))
            return;
        DismissMessage();
        saveOperation.result = S2Save_WriteSlot(saveOperation.slot, saveOperation.clearState);
        saveOperation.phase = SaveTaskPhase::Result;
        ShowMessage(saveOperation.result == S2SaveResult::Success ?
                "UISAVEOK" :
                "UISAVEERRDS", // @text UISAVEOK EN: "Saved successfully" | JA: "セーブが完了しました。" ; UISAVEERRDS EN: "Save failed. No storage space available" | JA: "空きスペースがないため、セーブできません。"
            true);
        return;
    }
    if (!ConsumeConfirmInput(*save->inputBuffer) && !ConsumeCancelInput(*save->inputBuffer))
        return;
    FinishSaveTask(*save, saveOperation.result);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool MoveTitleSelection(std::int32_t direction) noexcept {
    std::int32_t slot = titleFileSelect.selectedSlot;
    for (slot += direction; slot >= 0 && slot < 10; slot += direction) {
        if (SaveSlotEligible(static_cast<std::size_t>(slot), titleLoadOnly)) {
            titleFileSelect.selectedSlot = static_cast<std::int8_t>(slot);
            return true;
        }
    }
    return false;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void EndTitleLoadFlow() {
    titleFlowActive = false;
    titleLoadPhase = TitleLoadPhase::Idle;
    titleFailureReturnsToSelection = false;
    titleFileSelect = {};
    DismissMessage();
    DestroyElementByIndex(UIElement::LoadSave);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void BeginTitleLoadFlow(bool completedOnly) {
    titleFileSelect = {};
    titleLoadOnly = completedOnly;
    titleFailureReturnsToSelection = false;
    titleFlowActive = true;
    S2Save_RefreshSlotCatalog();
    const std::int32_t first = FirstEligibleSlot(completedOnly);
    if (first < 0) {
        titleFileSelect.selectedSlot = -1;
        titleLoadPhase = TitleLoadPhase::Failure;
        ShowMessage("UILOADFAILED", true);
        return;
    }
    titleFileSelect.selectedSlot = static_cast<std::int8_t>(first);
    titleLoadPhase = TitleLoadPhase::Selecting;
    ShowElementByIndex(UIElement::LoadSave);
    EnableElement(UIElement::LoadSave);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void RunTitleLoadFlow(bool completedOnly) {
    if (!titleFlowActive || titleLoadOnly != completedOnly) {
        BeginTitleLoadFlow(completedOnly);
        return;
    }

    if (titleLoadPhase == TitleLoadPhase::Failure) {
        if (!ConsumeConfirmInput(g_inputBuffer[0]) && !ConsumeCancelInput(g_inputBuffer[0]))
            return;
        if (titleFailureReturnsToSelection) {
            DismissMessage();
            titleFailureReturnsToSelection = false;
            titleLoadPhase = TitleLoadPhase::Selecting;
            return;
        }
        EndTitleLoadFlow();
        ReturnToTitleMenuTask();
        return;
    }
    if (titleLoadPhase == TitleLoadPhase::Confirm) {
        if (ConsumeCancelInput(g_inputBuffer[0])) {
            DismissMessage();
            titleLoadPhase = TitleLoadPhase::Selecting;
            return;
        }
        if (!ConsumeConfirmInput(g_inputBuffer[0]))
            return;
        DismissMessage();

        const std::size_t slot = static_cast<std::uint8_t>(titleFileSelect.selectedSlot);
        S2SaveImage image{};
        S2SaveResult result = S2Save_ReadSlot(slot, image);
        if (result == S2SaveResult::Success)
            result = S2Save_Apply(image);
        if (result != S2SaveResult::Success || !ChangeSystemModule(SystemModule::Game, 0)) {
            titleLoadPhase = TitleLoadPhase::Failure;
            ShowMessage("UILOADFAILED", true);
            titleFailureReturnsToSelection = true;
            return;
        }
        EndTitleLoadFlow();
        return;
    }
    if (ConsumeCancelInput(g_inputBuffer[0])) {
        EndTitleLoadFlow();
        ReturnToTitleMenuTask();
        return;
    }

    const std::uint16_t repeated = g_inputBuffer[0].repeatedPresses | g_inputBuffer[0].pressedPriority;
    if ((repeated & 0x1000U) != 0) {
        MoveTitleSelection(-1);
    } else if ((repeated & 0x2000U) != 0) {
        MoveTitleSelection(1);
    }
    if (!ConsumeConfirmInput(g_inputBuffer[0]))
        return;
    titleLoadPhase = TitleLoadPhase::Confirm;
    ShowMessage("UILOADDOYO", true); // @text EN: "Do you want to load this game?" | JA: "このゲームデータをロードしてもよろしいですか？"
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::filesystem::path MakeTemporaryPath(const std::filesystem::path &destination) {
    static std::atomic<std::uint64_t> sequence{};
    const auto ticks = static_cast<std::uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::filesystem::path temporary = destination;
    temporary += std::filesystem::path(".tmp-" + std::to_string(ticks) + "-" + std::to_string(sequence.fetch_add(1, std::memory_order_relaxed)));
    return temporary;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void RemoveTemporaryFile(const std::filesystem::path &path) noexcept {
    std::error_code ignored;
    std::filesystem::remove(path, ignored);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

S2SaveResult CommitTemporaryFile(const std::filesystem::path &temporary, const std::filesystem::path &destination) {
#ifdef _WIN32
    if (!MoveFileExW(temporary.c_str(), destination.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        return S2SaveResult::CommitFailed;
#else
    std::error_code error;
    std::filesystem::rename(temporary, destination, error);
    if (error)
        return S2SaveResult::CommitFailed;
    SyncParentDirectory(destination);
#endif
    return S2SaveResult::Success;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void CompleteBackupSave(std::int32_t stage, std::int32_t result, const void *source) {
    backupCompletionSource = source;
    backupCompletionStage = stage;
    switch (result) {
    case -3:
        backupCompletionResult = -4;
        break;
    case -2:
        backupCompletionResult = -5;
        break;
    case -1:
        backupCompletionResult = -2;
        break;
    default:
        backupCompletionResult = result ? -6 : 0;
        break;
    }
    const BackupSaveCompletion completion = backupSaveCompletion;
    activeBackupState.store(0, std::memory_order_seq_cst);
    if (completion)
        completion(backupCompletionResult, backupCompletionSource);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t CompleteBackupSaveWrite(void *parameter) {
    BackupSaveWriteWork *const work = static_cast<BackupSaveWriteWork *>(parameter);
    if (!work) {
        backupLayerCallback(3, -9, nullptr);
        return 0;
    }
    while (d3t::Archive_HasPendingRead(work->archiveHandle)) {
    }
    const d3t::ArchiveHandleEntry *const archive = d3t::Archive_GetHandle(work->archiveHandle);
    const std::int32_t result = archive->readSucceeded ? 0 : -9;
    d3t::ReadFile_0(0, &work->archiveHandle);
    backupLayerCallback(3, result, work->source);
    return 0;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool EnsureBackupParentDirectories(const char *source) {
    char path[261];
    std::strcpy(path, source);
#ifdef _WIN32
    FilepathCleaner(path);
#else
    for (char *cursor = path; *cursor; ++cursor)
        if (*cursor == '\\')
            *cursor = '/';
#endif
    char *cursor = path;
    while (char *const slash = std::strchr(cursor, '/')) {
        *slash = 0;
        if (*path) {
#ifdef _WIN32
            const DWORD attributes = GetFileAttributesA(path);
            if ((attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY)) && !CreateDirectoryA(path, nullptr))
                return false;
#else
            struct stat attributes{};
            if ((stat(path, &attributes) || !S_ISDIR(attributes.st_mode)) && mkdir(path, 0777))
                return false;
#endif
        }
        *slash = '/';
        cursor = slash + 1;
    }
    return true;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void QueueBackupArchiveWrite(const char *path, const void *source, std::uint32_t size) {
#if defined(SHENMUE_STEAM)
    const bool accepted = ::shenmue::d3t::WriteSteamFileAsync(
        path, source, size, [](std::int32_t stage, std::int32_t result, void *data) { backupLayerCallback(stage, result, data); }, const_cast<void *>(source));
    if (!accepted)
        backupLayerCallback(2, -4, source);
#else
    if (!EnsureBackupParentDirectories(path))
        return;
    const std::uint32_t handle = d3t::Archive_AllocateHandle(0);
    if (!d3t::Archive_DoesHandleExist(handle)) {
        backupLayerCallback(2, -4, source);
        return;
    }
    d3t::Archive_GetHandle(handle)->platformHandle = -1;
    void *const buffer = std::malloc(size);
    d3t::Archive_BindBuffer(handle, buffer, size, false);
    if (!buffer)
        return;
    std::memcpy(buffer, source, size);
    d3t::Archive_QueueRead(handle, 1, path, 0, 0, 35);
    BackupSaveWriteWork work{.archiveHandle = handle, .size = size, .path = {}, .reserved10c = 0, .source = source};
    std::strcpy(work.path.data(), path);
    const d3t::D3TTaskDescriptor descriptor{.data = &work, .callback = CompleteBackupSaveWrite, .completion = nullptr, .priority = 0};
    d3t::d3t_QueueTask(&descriptor, sizeof(work));
#endif
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t CompleteBackupFileRead(void *parameter) {
    BackupSaveReadWork *const work = static_cast<BackupSaveReadWork *>(parameter);
    if (!work) {
        backupLayerCallback(2, -9, nullptr);
        return 0;
    }
    while (d3t::Archive_HasPendingRead(work->archiveHandle)) {
    }
    const d3t::ArchiveHandleEntry *const archive = d3t::Archive_GetHandle(work->archiveHandle);
    const std::int32_t result = archive->readSucceeded ? 0 : -4;
    if (result == 0)
        std::memcpy(work->destination, archive->bufferedData, work->size);
    d3t::ReadFile_0(0, &work->archiveHandle);
    work->completion(2, result, work->destination);
    return 0;
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void QueueBackupArchiveRead(const char *path, void *destination, std::uint32_t offset, std::uint32_t size) {
#if defined(SHENMUE_STEAM)
    const bool accepted = ::shenmue::d3t::ReadSteamFileAsync(
        path, destination, offset, size, [](std::int32_t stage, std::int32_t result, void *data) { backupLayerCallback(stage, result, data); }, destination);
    if (!accepted)
        backupLayerCallback(2, -4, destination);
#else
    const std::uint32_t handle = d3t::Archive_AllocateHandle(0);
    if (!d3t::Archive_DoesHandleExist(handle)) {
        backupLayerCallback(2, -4, destination);
        return;
    }
    d3t::Archive_GetHandle(handle)->platformHandle = -1;
    d3t::Archive_BindBuffer(handle, std::malloc(size), size, false);
    d3t::Archive_QueueRead(handle, 0, path, offset, size, 33);
    BackupSaveReadWork work{.archiveHandle = handle, .reserved04 = 0, .completion = backupLayerCallback, .size = size, .path = {}, .destination = destination};
#ifdef _WIN32
    strcpy_s(work.path.data(), work.path.size(), path);
#else
    const std::size_t length = std::strlen(path);
    if (length >= work.path.size())
        std::abort();
    std::memcpy(work.path.data(), path, length + 1);
#endif
    const d3t::D3TTaskDescriptor descriptor{.data = &work, .callback = CompleteBackupFileRead, .completion = nullptr, .priority = 0};
    d3t::d3t_QueueTask(&descriptor, sizeof(work));
#endif
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t PreflightBackupSaveWrite(const char *path, std::uint32_t size) {
#if defined(SHENMUE_STEAM)
    const std::int32_t previousSize = ::shenmue::d3t::SteamFileExists(path) ? ::shenmue::d3t::GetSteamFileSize(path) : 0;
    if (previousSize < 0)
        return -4;
    const std::int64_t growth = static_cast<std::int64_t>(size) - previousSize;
    return growth <= 0 || ::shenmue::d3t::GetSteamStorageAvailableBytes() >= static_cast<std::uint64_t>(growth) ? 0 : -5;
#else
    std::int32_t previousSize = 0;
#ifdef _WIN32
    const DWORD attributes = GetFileAttributesA(path);
    if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        WIN32_FILE_ATTRIBUTE_DATA data{};
        const std::uint32_t length = GetFileAttributesExA(path, GetFileExInfoStandard, &data) ? data.nFileSizeLow : 0;
        previousSize = std::bit_cast<std::int32_t>(length);
    }
#else
    struct stat attributes{};
    if (!stat(path, &attributes) && !S_ISDIR(attributes.st_mode))
        previousSize = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(attributes.st_size));
#endif
    if (previousSize < 0) {
        if (previousSize == -3)
            return -4;
        if (previousSize == -2)
            return -5;
        return previousSize == -1 ? -2 : -6;
    }
    const std::int32_t growth = std::bit_cast<std::int32_t>(size - static_cast<std::uint32_t>(previousSize));
    return BACKUP_LOCAL_CAPACITY >= growth ? 0 : -5;
#endif
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t DeleteBackupArchiveFile(const char *name) {
    std::int32_t result = 0;
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::DeleteSteamFile(name);
    backupLayerCallback(1, result, nullptr);
    return result;
#else
    const std::string path = (saveDirectory / name).string();
#ifdef _WIN32
    SetFileAttributesA(path.c_str(), GetFileAttributesA(path.c_str()) & ~static_cast<DWORD>(FILE_ATTRIBUTE_READONLY));
    if (!DeleteFileA(path.c_str()))
        result = -6;
#else
    if (unlink(path.c_str()))
        result = -6;
#endif
    backupLayerCallback(1, result, nullptr);
    return result;
#endif
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void SetSaveDataStuff() {
    activeBackupState.store(0, std::memory_order_seq_cst);
    (void)ReturnZero();
    backupLayerCallback = CompleteBackupSave;
    const BackupSaveCompletion completion = backupSaveCompletion;
    backupCompletionSource = nullptr;
    backupCompletionStage = 0;
    backupCompletionResult = 0;
    activeBackupState.store(0, std::memory_order_seq_cst);
    if (completion)
        completion(backupCompletionResult, backupCompletionSource);
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint64_t BupReady(std::int32_t reserve) {
#if defined(SHENMUE_STEAM)
    while (true) {
        {
            const std::lock_guard<std::mutex> lock(backupAdmissionMutex);
            if (activeBackupState.load(std::memory_order_seq_cst) != -2) {
                if (reserve)
                    activeBackupState.store(-2, std::memory_order_seq_cst);
                return static_cast<std::uint32_t>(activeBackupState.load(std::memory_order_seq_cst));
            }
        }
        ::shenmue::d3t::RunSteamCallbacks();
        std::this_thread::yield();
    }
#else
    const std::lock_guard<std::mutex> lock(backupAdmissionMutex);
    while (activeBackupState.load(std::memory_order_seq_cst) == -2) {
    }
    if (reserve)
        activeBackupState.store(-2, std::memory_order_seq_cst);
    return static_cast<std::uint32_t>(activeBackupState.load(std::memory_order_seq_cst));
#endif
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint64_t QueueSaveFileWrite(const char *name, const void *source, std::uint32_t size, BackupSaveCompletion completion) {
    BupReady(1);
    activeBackupState.store(-2, std::memory_order_seq_cst);
    backupSaveCompletion = completion;
    const std::string path = (saveDirectory / name).string();
    const std::int32_t preflight = PreflightBackupSaveWrite(path.c_str(), size);
    if (preflight) {
        backupCompletionSource = source;
        backupCompletionStage = 3;
        backupCompletionResult = -5;
        const BackupSaveCompletion callback = backupSaveCompletion;
        activeBackupState.store(0, std::memory_order_seq_cst);
        if (callback)
            callback(backupCompletionResult, backupCompletionSource);
        return static_cast<std::uint32_t>(preflight);
    }
    QueueBackupArchiveWrite(path.c_str(), source, size);
    return 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint32_t QueueSnapshotMetadataWrite(SnapshotMetadataOwner *metadata) {
    return QueueSaveFileWrite("Snap-shots", metadata, 0x1c68) ? 5 : 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::int32_t DeleteSaveFile(const char *name) {
    BupReady(1);
    activeBackupState.store(-2, std::memory_order_seq_cst);
    switch (const std::int32_t result = DeleteBackupArchiveFile(name)) {
    case -3:
        return -4;
    case -2:
        return -5;
    case -1:
        return -2;
    default:
        return result ? -6 : 0;
    }
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
BackupListenerNode *HPLG_RegisterCallback(HLib::TaskCallback callback) {
    if (!callback || !g_hotplugTask)
        return 0;
    BackupHotplugData *const data = static_cast<BackupHotplugData *>(HLib::GetTaskParameter(g_hotplugTask));
    if (!data)
        return 0;
    BackupListenerNode *node = data->listeners;
    while (node->callback)
        ++node;
    node->previous = data->lastListener;
    node->next = 0;
    node->callback = callback;
    if (data->lastListener)
        data->lastListener->next = node;
    data->lastListener = node;
    callback(data);
    return node;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void HPLG_UnregisterCallback(BackupListenerNode *node) {
    do {
        if (!node || !g_hotplugTask)
            return;
    } while (0);
    BackupHotplugData *const data = static_cast<BackupHotplugData *>(HLib::GetTaskParameter(g_hotplugTask));
    BackupListenerNode *const listener = node;
    if (!data)
        return;
    if (listener->previous)
        listener->previous->next = listener->next;
    if (listener->next)
        listener->next->previous = listener->previous;
    else
        data->lastListener = listener->previous;
    listener->callback = 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void HPLG_NotifyCallbacks(BackupHotplugData *data) {
    for (BackupListenerNode *node = data->lastListener; node; node = node->previous)
        node->callback(data);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void HPLG_Callback(void *taskData) {
    BackupHotplugData *const data = static_cast<BackupHotplugData *>(taskData);
    for (std::int32_t index = 0; index < 1; ++index) {
        if ((data->flags[index] & 1U) == 0 && GetBackupConnectionStatus(index) != -1)
            RefreshBackupHotplugData(data, index);
        else if ((data->flags[index] & 1U) && GetBackupConnectionStatus(index) == -1)
            RefreshBackupHotplugData(data, index);
    }
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void HPLG_Enqueue() {
    g_hotplugTask = HLib::EnqueueTaskWithParameter(HPLG_Callback, 2, 2, sizeof(BackupHotplugData), TASK_HPLG);
    do {
        if (!g_hotplugTask)
            return;
    } while (0);
    BackupHotplugData *const data = static_cast<BackupHotplugData *>(HLib::GetTaskParameter(g_hotplugTask));
    if (!data) {
        HLib::DestroyTaskInternal(g_hotplugTask);
        g_hotplugTask = 0;
        return;
    }
    std::memset(data, 0, sizeof(*data));
    for (std::int32_t index = 0; index < 1; ++index)
        RefreshBackupHotplugData(data, index);
}

// @verified - d
void HPLG_Destroy() {
    do {
        if (!g_hotplugTask)
            return;
    } while (0);
    HLib::DestroyTaskInternal(g_hotplugTask);
}

void HPLG_Refresh(std::int32_t index) {
    RefreshBackupHotplugData(static_cast<BackupHotplugData *>(HLib::GetTaskParameter(g_hotplugTask)), index);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t GetBackupInfo(std::int32_t index) {
    const BackupHotplugData *const data = reinterpret_cast<const BackupHotplugData *>(HLib::GetTaskParameter(g_hotplugTask));
    if (!data)
        return 0;
    return data->flags[index];
}

float GetBackupProgressRatio() {
    return static_cast<float>(g_backupProgress) / static_cast<float>(g_backupProgressLimit);
}

#endif

} // namespace s2
} // namespace shenmue

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void __attribute__((renesas)) shenmue_s2_event_SetSceneInfos(std::int32_t actNumber, std::uint32_t mapName, std::int32_t entryPoint) {
    shenmue::s2::SetSceneInfos(actNumber, shenmue::shen_name(mapName), entryPoint);
}
#else
#endif
