#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/legacy_types.hpp"

namespace shenmue {
namespace s2 {

extern void *g_backupWorkBuffer;
extern volatile std::int32_t g_backupResults[1];
extern volatile std::int32_t g_backupProgress;
extern std::int32_t g_backupProgressLimit;

std::int32_t BackupCompleteCallback(std::int32_t port, std::int32_t operation, std::int32_t result);
std::int32_t BackupProgressCallback();
void InitializeBackupMemory();
void WaitForBackupMemoryIdle();
std::int32_t GetBackupConnectionStatus(std::int32_t index);
std::int32_t WaitForBackupOperation(std::int32_t index);
std::int32_t CheckBackupFileExists(std::int32_t index, const char *filename);
std::int32_t GetBackupFreeBlocks(std::int32_t index);
std::int32_t ReadBackupBlocks(std::int32_t index, void *destination, std::int32_t firstBlock, std::int32_t blocks, const char *filename);

} // namespace s2
} // namespace shenmue

#endif
