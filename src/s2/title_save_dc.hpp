#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hlib.hpp"

namespace shenmue {
namespace s2 {

struct BackupSlotMetadata;
struct BackupHotplugData;
struct BackupListenerNode;
struct TitleSaveSelectionSlot {
    std::uint32_t flags;
    BackupSlotMetadata *metadata;
};
struct TitleSaveSelectionPrefix {
    std::int8_t phase, result, inherited, retained03, selectedSlot, availableCount;
    std::uint8_t retained06[2];
    TitleSaveSelectionSlot slots[3];
};
struct TitleSaveStateRule {
    std::int8_t nextState, alternateState, connectedState, disconnectedState;
    std::int16_t warning;
};
typedef char TitleSaveStateRuleSizeCheck[sizeof(TitleSaveStateRule) == 6 ? 1 : -1];
extern TitleSaveStateRule titleSaveTransitionRules[20];
typedef char TitleSaveSelectionSlotSizeCheck[sizeof(TitleSaveSelectionSlot) == 8 ? 1 : -1];
typedef char TitleSaveSelectionPrefixSizeCheck[sizeof(TitleSaveSelectionPrefix) == 0x20 ? 1 : -1];
typedef char TitleSaveSelectedSlotCheck[offsetof(TitleSaveSelectionPrefix, selectedSlot) == 4 ? 1 : -1];
typedef char TitleSaveSlotArrayCheck[offsetof(TitleSaveSelectionPrefix, slots) == 8 ? 1 : -1];

extern TitleSaveSelectionPrefix titleSaveSelection;
extern TitleSaveSelectionPrefix *titleActiveSaveSelection;
extern BackupHotplugData *titleSaveBackupData;
extern BackupListenerNode *titleSaveBackupListener;
extern std::int32_t titleSaveTransitionTimer, titleSaveState;
extern std::uint8_t titleSaveWarningParameters[7];
void DrawTitleFileSelectDate(std::int32_t x, std::int32_t y, const void *date);
void DrawTitleFileSelectCard(std::int32_t slot);
void DrawFileSelectFrame();
void DrawTitleFileSelectCards(std::int32_t result);
void TitleSaveSelectionResetNoOp();
std::int32_t GetTitleSaveBackupPort();
std::int32_t TitleFileSelectCanUseSave(const BackupSlotMetadata *metadata);
void InitializeTitleFileSelectState();
void TitleSaveHotplugCallback(void *data);
void ControlTitleFileSelect(void *data);
void DrawTitleFileSelect(void *data);

} // namespace s2
} // namespace shenmue
#endif
