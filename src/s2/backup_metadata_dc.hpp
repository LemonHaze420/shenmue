#pragma once

#include "s2/backup_hotplug_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct BackupFileHeader {
    shen_name name;
    std::uint32_t version;
};

struct GameSaveMetadataBuffer {
    std::byte retainedVmuHeader[0x80];
    BackupFileHeader header;
    std::byte retainedHeader[0x18];
    BackupSlotMetadata metadata;
    std::byte retainedTail[0x120];
};

struct ImportedSaveMetadataBuffer {
    std::byte retainedVmuHeader[0x80];
    BackupFileHeader header;
    std::uint8_t completed;
    std::byte retainedHeader[0xb7];
    BackupSlotMetadata metadata[3];
};

struct SystemSaveMetadataBuffer {
    std::byte retainedVmuHeader[0x80];
    BackupFileHeader header;
    std::byte retainedData[0x178];
};

typedef char Layout_backup_metadata_dc_0[(sizeof(BackupFileHeader) == 8) ? 1 : -1];
typedef char Layout_backup_metadata_dc_1[(sizeof(GameSaveMetadataBuffer) == 0x200 && offsetof(GameSaveMetadataBuffer, metadata) == 0xa0) ? 1 : -1];
typedef char Layout_backup_metadata_dc_2[(sizeof(ImportedSaveMetadataBuffer) == 0x200 && offsetof(ImportedSaveMetadataBuffer, metadata) == 0x140) ? 1 : -1];
typedef char Layout_backup_metadata_dc_3[(offsetof(ImportedSaveMetadataBuffer, completed) == 0x88) ? 1 : -1];
typedef char Layout_backup_metadata_dc_4[(sizeof(SystemSaveMetadataBuffer) == 0x200 && offsetof(SystemSaveMetadataBuffer, header) == 0x80) ? 1 : -1];

#define SAVE_GAME_HEADER SHEN_NAME4('S', '2', 'G', 'A')
#define SAVE_SYSTEM_HEADER SHEN_NAME4('S', '2', 'S', 'Y')
#define SAVE_IMPORTED_HEADER SHEN_NAME4('S', 'H', 'E', 'N')
static const char *kGameSaveFilenames[4] = {"SHENMUE2_001", "SHENMUE2_002", "SHENMUE2_003", "SHENMUE2_SYS"};
static const std::uint32_t kGameSaveSlotFlags[4] = {0x200, 0x400, 0x800, 0x100};
extern GameSaveMetadataBuffer g_gameSaveMetadataBuffer;
extern ImportedSaveMetadataBuffer g_importedSaveMetadataBuffer;
extern SystemSaveMetadataBuffer g_systemSaveMetadataBuffer;
extern BackupSlotMetadata g_cachedBackupSlots[4];

std::int32_t ValidateGameSaveHeader(const BackupFileHeader *header);
std::int32_t ValidateSystemSaveHeader(const BackupFileHeader *header);
std::int32_t ValidateImportedSaveHeader(const BackupFileHeader *header);
std::int32_t ReadLocalSaveMetadata(std::uint32_t slot, BackupSlotMetadata *metadata);
std::uint32_t ReadImportedSaveMetadata(std::int32_t index, BackupSlotMetadata *metadata);
std::uint32_t ProbeLocalSaveFiles(BackupSlotMetadata *metadata);
bool IsTitleLogoPresent();

} // namespace s2
} // namespace shenmue

#endif
