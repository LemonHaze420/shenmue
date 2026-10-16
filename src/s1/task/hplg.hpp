#pragma once

#include "s1/hlib.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
namespace shenmue::s1 {
inline constexpr std::size_t kBookkeepingCacheSize = 0x1a8c;
using BookkeepingRefreshListener = void (*)(const std::byte *cache);

extern HLib::Task *g_hotPlugTask;

extern std::int32_t g_returnToTitleRequested;

void bupHotPlugTaskStart();
void HPLG_RegisterBookkeepingListener(BookkeepingRefreshListener listener);
std::uint32_t HPLG_GetBookkeepingProgress();
bool HPLG_HasBookkeepingProgress();
std::uint32_t HPLG_GetBookkeepingRefreshCount();
bool HPLG_IsStorageConnected();
void HPLG_SetStorageConnected(bool connected);

const struct S1SaveSummary *HPLG_GetSaveSummary(std::size_t slot);
void HPLG_SetSaveSummary(std::size_t slot, const struct S1SaveSummary &summary);
// Local stand-in for the Steam RemoteStorage file SHENMUE1_00<slot>.
std::filesystem::path HPLG_SaveSlotPath(std::size_t slot);
std::int32_t UploadBookkeepingGlobalStats();

void HLib_mainModule(HLib::TaskData *taskData);

} // namespace shenmue::s1
