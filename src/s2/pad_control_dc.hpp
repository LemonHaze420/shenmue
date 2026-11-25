#pragma once

#include "s2/input_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct PadControlEntry {
    std::uint32_t flags;
    LegacyInputSlot input;
};
struct PadControlTaskData {
    PadControlEntry entries[16];
    PadControlEntry *currentBoundEntry;
};
typedef char Layout_pad_control_dc_0[(sizeof(PadControlEntry) == 0x18) ? 1 : -1];
typedef char Layout_pad_control_dc_1[(offsetof(PadControlEntry, input) == 4) ? 1 : -1];
typedef char Layout_pad_control_dc_2[(sizeof(PadControlTaskData) == 0x184) ? 1 : -1];
typedef char Layout_pad_control_dc_3[(offsetof(PadControlTaskData, currentBoundEntry) == 0x180) ? 1 : -1];
#define TASK_PADC SHEN_NAME4('P', 'A', 'D', 'C')
#define TASK_PPAD SHEN_NAME4('P', 'P', 'A', 'D')
extern HLib::Task *g_padControlTask;
extern std::int32_t g_prePadConfirmPulse;
void ResetProcessedInput(LegacyInputSlot &input);
void PadControl_Callback(void *data);
void Queue_PadControlTask(std::int8_t nextFunctionIndex);
PadControlEntry *AllocatePadControlEntry();
PadControlEntry *BindPadControlEntry(PadControlEntry *entry);
void ReleasePadControlEntry(PadControlEntry *entry);
LegacyInputSlot *ctrlGetPadInputBuffer0(PadControlEntry *entry);
bool IsPadControlEntryBound(const PadControlEntry *entry);
void PrePadControl_Callback(void *data);
HLib::Task *Queue_PrePadControlTask(std::int8_t nextFunctionIndex);
void padEnableTask();

} // namespace s2
} // namespace shenmue

#endif
