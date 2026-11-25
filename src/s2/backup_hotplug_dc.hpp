#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {

struct BackupSlotMetadata {
    std::byte retainedDates[0x10];
    std::uint16_t saveKind;
    std::uint16_t retained12;
    std::int16_t stepHundreds, sceneNumber;
    shen_name mapName;
    std::int32_t entryPoint;
    std::byte retainedDescription[0x20];
};

struct BackupListenerNode {
    BackupListenerNode *previous;
    BackupListenerNode *next;
    HLib::TaskCallback callback;
};

struct BackupHotplugData {
    BackupSlotMetadata imported[3];
    BackupSlotMetadata saves[4];
    std::uint32_t flags[4];
    std::uint32_t retainedState;
    BackupListenerNode listeners[16];
    BackupListenerNode *lastListener;
};

typedef char Layout_backup_hotplug_dc_0[(sizeof(BackupSlotMetadata) == 0x40) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_1[(offsetof(BackupSlotMetadata, saveKind) == 0x10) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_2[(offsetof(BackupSlotMetadata, stepHundreds) == 0x14) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_3[(offsetof(BackupSlotMetadata, sceneNumber) == 0x16) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_4[(offsetof(BackupSlotMetadata, mapName) == 0x18) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_5[(offsetof(BackupSlotMetadata, entryPoint) == 0x1c) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_6[(sizeof(BackupListenerNode) == 0x0c) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_7[(sizeof(BackupHotplugData) == 0x298) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_8[(offsetof(BackupHotplugData, saves) == 0xc0) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_9[(offsetof(BackupHotplugData, flags) == 0x1c0) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_10[(offsetof(BackupHotplugData, listeners) == 0x1d4) ? 1 : -1];
typedef char Layout_backup_hotplug_dc_11[(offsetof(BackupHotplugData, lastListener) == 0x294) ? 1 : -1];

#define TASK_HPLG SHEN_NAME4('H', 'P', 'L', 'G')
extern HLib::Task *g_hotplugTask;
std::uint32_t GetBackupInfo(std::int32_t index);
void RefreshBackupHotplugData(BackupHotplugData *data, std::int32_t index);
void HPLG_NotifyCallbacks(BackupHotplugData *data);
BackupListenerNode *HPLG_RegisterCallback(HLib::TaskCallback callback);
void HPLG_UnregisterCallback(BackupListenerNode *node);
void HPLG_Callback(void *data);
void HPLG_Enqueue();

} } // namespace shenmue::s2

#endif
