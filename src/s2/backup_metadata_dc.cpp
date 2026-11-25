#include "s2/backup_metadata_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/backup_dc.hpp"
#include "s2/save.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/title_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

GameSaveMetadataBuffer g_gameSaveMetadataBuffer = {};
ImportedSaveMetadataBuffer g_importedSaveMetadataBuffer = {};
SystemSaveMetadataBuffer g_systemSaveMetadataBuffer = {};
BackupSlotMetadata g_cachedBackupSlots[4];

// @verified - d
std::int32_t ValidateGameSaveHeader(const BackupFileHeader *header) {
    return header->name == SAVE_GAME_HEADER && header->version == 0x5a;
}

// @verified - d
std::int32_t ValidateSystemSaveHeader(const BackupFileHeader *header) {
    if (header->name == SAVE_SYSTEM_HEADER && header->version == 0x5a)
        return 1;
    return 0;
}

// @verified - d
std::int32_t ValidateImportedSaveHeader(const BackupFileHeader *header) {
    return header->name == SAVE_IMPORTED_HEADER && header->version == 0xe6;
}

// @verified - d
bool IsTitleLogoPresent() {
    if (logoFlags & 4U)
        return true;
    return false;
}

// @verified - d
std::int32_t ReadLocalSaveMetadata(std::uint32_t slot, BackupSlotMetadata *metadata) {
    BackupFileHeader *header = &g_gameSaveMetadataBuffer.header;
    std::int32_t result = WaitForBackupOperationPort0();
    if (slot >= 3)
        return -6;
    std::memset(&g_gameSaveMetadataBuffer, 0, sizeof(g_gameSaveMetadataBuffer));
    if (result == 0)
        result = ReadBackupBlocksPort0(&g_gameSaveMetadataBuffer, 3, 1, kGameSaveFilenames[slot]);
    if (result == 0)
        result = WaitForBackupOperationPort0();
    if (result == 0 && !ValidateGameSaveHeader(header))
        result = -6;
    if (result == 0)
        std::memcpy(metadata, reinterpret_cast<std::byte *>(header) + 0x20, sizeof(*metadata));
    return result;
}

// @verified - d
std::uint32_t ReadImportedSaveMetadata(std::int32_t index, BackupSlotMetadata *metadata) {
    BackupFileHeader *header = &g_importedSaveMetadataBuffer.header;
    std::uint32_t flags = 0;
    std::int32_t result = WaitForBackupOperation(index);
    if (result == 0)
        result = ReadBackupBlocks(index, reinterpret_cast<std::byte *>(header) - 0x80, 3, 1, "SHENMUE1_000");
    if (result == 0)
        result = WaitForBackupOperation(index);
    if (result == 0 && !ValidateImportedSaveHeader(header))
        result = -6;
    if (result == 0) {
        flags |= 4U;
        if (reinterpret_cast<std::uint8_t *>(header)[8])
            flags |= 8U;
        std::memcpy(metadata, reinterpret_cast<std::byte *>(header) + 0xc0, sizeof(g_importedSaveMetadataBuffer.metadata));
    }
    return flags;
}

// @verified - d
std::uint32_t ProbeLocalSaveFiles(BackupSlotMetadata *metadata) {
    std::uint32_t flags = 0;
    std::memset(&g_systemSaveMetadataBuffer, 0, sizeof(g_systemSaveMetadataBuffer));
    std::int32_t result = WaitForBackupOperationPort0();
    if (result == 0)
        result = ReadBackupBlocksPort0(&g_systemSaveMetadataBuffer, 3, 1, kGameSaveFilenames[3]);
    if (result == 0)
        result = WaitForBackupOperationPort0();
    if (result == 0 && !ValidateSystemSaveHeader(&g_systemSaveMetadataBuffer.header))
        result = -6;
    if (result == 0) {
        flags |= kGameSaveSlotFlags[3];
        if (!IsTitleLogoPresent()) {
            const std::uint32_t *saved = reinterpret_cast<const std::uint32_t *>(&g_systemSaveMetadataBuffer.header);
            g_nativePlayerStats.gameConfigurationFlags = saved[2];
            g_completedGameCount = saved[3];
        }
    }
    for (std::int32_t slot = 0; slot < 3; ++slot) {
        result = WaitForBackupOperation(0);
        if (result == 0 && ReadLocalSaveMetadata(slot, metadata + slot) == 0) {
            flags |= kGameSaveSlotFlags[slot];
            if (metadata[slot].saveKind != 0)
                flags |= 0x40;
        }
    }
    if (flags)
        flags |= 0x10U;
    return flags;
}

// @verified - d
void RefreshBackupHotplugData(BackupHotplugData *data, std::int32_t index) {
    switch (GetBackupConnectionStatus(index)) {
    case -1:
        data->flags[index] = 0U;
        break;
    case -3:
        data->flags[index] = 3U;
        break;
    default:
        data->flags[index] = 1U;
        break;
    }
    if (index == 0)
        std::memset(data->imported, 0, sizeof(data->imported));
    data->flags[index] |= ReadImportedSaveMetadata(index, data->imported);
    if (index == 0)
        std::memset(data->saves, 0, 0x80);
    data->flags[index] |= ProbeLocalSaveFiles(data->saves);
    if (index == 0) {
        std::int32_t required = 0x17;
        for (std::int32_t slot = 0; slot < 4; ++slot) {
            BackupSlotMetadata *cached = &g_cachedBackupSlots[slot];
            const std::byte *entry = reinterpret_cast<const std::byte *>(data) + slot * sizeof(BackupSlotMetadata);
            *cached = *reinterpret_cast<const BackupSlotMetadata *>(entry + offsetof(BackupHotplugData, saves));
        }
        if (data->flags[index] & 1U) {
            if (data->flags[index] & 0x100U)
                required -= 4;
            if (data->flags[index] & 0xe00U)
                required -= 0x13;
            if (GetBackupFreeBlocks(index) >= required)
                data->flags[index] |= 0x80U;
        }
    }
    if (index == 0)
        HPLG_NotifyCallbacks(data);
}

} // namespace s2
} // namespace shenmue

#endif
