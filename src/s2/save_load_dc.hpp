#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "common/types.hpp"

namespace shenmue {
namespace s2 {

extern std::byte *g_systemSaveData, *g_gameSaveData;
struct SaveRtcDate {
    std::uint16_t year;
    std::uint8_t month, day, hour, minute, second, dayOfWeek;
    std::byte retained[4];
};
typedef char SaveRtcDateSizeCheck[sizeof(SaveRtcDate) == 12 ? 1 : -1];
extern std::int32_t g_retainedSaveSelection, g_scenarioSaveRestoreState;
struct NativeMoveSkillBank {
    std::int16_t proficiency[40], usage[40];
    std::int8_t count;
    std::uint8_t selected;
};
typedef char NativeMoveSkillBankSizeCheck[sizeof(NativeMoveSkillBank) == 0xa2 ? 1 : -1];
extern NativeMoveSkillBank g_moveSkillBank1, g_moveSkillBank2, g_moveSkillBank3;
extern std::uint16_t g_saveRetainedSceneWords[2];
static const std::int16_t inheritedInventoryMappings[12][2] = {
    {4, 3}, {3, 2}, {5, 4}, {6, 5}, {8, 8005}, {7, 6}, {10, 8006}, {11, 8007}, {12, 8008}, {13, 8009}, {14, 8010}, {15, 8011}};
void CaptureSavedGamePayload(std::byte *payload, std::byte *metadata, std::int32_t initializationMode);
void RestoreSavedGamePayload(const std::byte *metadata, const std::byte *payload);
std::uint32_t CalculateSavePayloadChecksum(const void *source, std::int32_t size);
bool ValidateBackupSaveImage(void *image);
void ImportSelectedTitleSaveData(std::int32_t inherited, const void *bank, std::int32_t slot);
std::int32_t InitializeNewGameFromSaveSlot();
std::int32_t LoadGameFromSaveSlot(std::int32_t slot, std::int32_t restorePlacement);
std::int32_t NormalizeNewGameSaveResult(std::int32_t result);
void ResetGameForSaveLoad(std::int32_t loading);
void InitializeDefaultSaveData();
std::int32_t ReadSaveDataSection(std::int32_t slot);
void SetRetainedSaveSelection(std::int32_t slot);
void RestoreGameSaveSlot(std::uint32_t slot);
void SetScenarioSaveRestoreState(std::int32_t state);

} // namespace s2
} // namespace shenmue
#endif
