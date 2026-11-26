#include "s2/pad_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstring>

namespace shenmue {
namespace s2 {

void PadControl_Callback(void *data);
void ResetProcessedInput(LegacyInputSlot &input);
PadControlEntry *BindPadControlEntry(PadControlEntry *entry);

HLib::Task *g_padControlTask;
std::int32_t g_prePadConfirmPulse;

// @verified - d
void Queue_PadControlTask(std::int8_t nextFunctionIndex) {
    g_padControlTask = HLib::EnqueueTaskWithParameter(PadControl_Callback, nextFunctionIndex, 2, sizeof(PadControlTaskData), TASK_PADC);
    PadControlTaskData *const control = static_cast<PadControlTaskData *>(HLib::GetTaskParameter(g_padControlTask));
    control->currentBoundEntry = 0;
    for (std::int32_t index = 0; index < 16; ++index) {
        control->entries[index].flags = 0;
        control->entries[index].input.held = 0;
        ResetProcessedInput(control->entries[index].input);
    }
}

// @verified - d
void PadControl_Callback(void *data) {
    PadControlTaskData &control = *static_cast<PadControlTaskData *>(data);
    for (std::int32_t index = 0; index < 16; ++index) {
        if (control.entries[index].flags & 1U) {
            if (control.entries[index].flags & 2U) {
                PadControlEntry *entry = control.entries + index;
                std::memcpy(&entry->input, &g_inputBuffer[0], sizeof(entry->input));
            } else
                ResetProcessedInput(control.entries[index].input);
        }
    }
}

// @verified - d
void ResetProcessedInput(LegacyInputSlot &input) {
    input.previousHeld = input.held;
    input.unknownMask = 0;
    input.held = 0;
    input.pressedPriority = 0;
    input.repeatedPresses = 0;
    input.leftStickX = 0x80;
    input.leftStickY = 0x80;
    input.triggers[0] = 0;
    input.triggers[1] = 0;
}

// @verified - d
PadControlEntry *AllocatePadControlEntry() {
    PadControlTaskData *const control = static_cast<PadControlTaskData *>(HLib::GetTaskParameter(g_padControlTask));
    std::int32_t index;
    for (index = 0; index < 16; ++index) {
        if ((control->entries[index].flags & 1U) == 0)
            break;
    }
    control->entries[index].flags |= 1U;
    control->entries[index].input.held = 0;
    ResetProcessedInput(control->entries[index].input);
    return &control->entries[index];
}

// @verified - d
void ReleasePadControlEntry(PadControlEntry *entry) {
    HLib::GetTaskParameter(g_padControlTask);
    if (entry->flags & 2U)
        BindPadControlEntry(0);
    entry->flags ^= 1U;
}

// @verified - d
PadControlEntry *BindPadControlEntry(PadControlEntry *entry) {
    PadControlTaskData &control = *static_cast<PadControlTaskData *>(HLib::GetTaskParameter(g_padControlTask));
    if (entry && (entry->flags & 1U) == 0)
        entry = 0;
    PadControlEntry *const previous = control.currentBoundEntry;
    if (entry != previous) {
        if (previous) {
            ResetProcessedInput(previous->input);
            previous->flags &= ~2U;
        }
        if (entry) {
            std::memcpy(&entry->input, &g_inputBuffer[0], sizeof(entry->input));
            entry->flags |= 2U;
        }
        control.currentBoundEntry = entry;
    }
    return previous;
}

// @verified - d
LegacyInputSlot *ctrlGetPadInputBuffer0(PadControlEntry *entry) {
    return &entry->input;
}

// @verified - d
bool IsPadControlEntryBound(const PadControlEntry *entry) {
    if (entry->flags & 2U)
        return true;
    return false;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SuppressGameInput(std::int32_t frames);

void SuppressPadInput(std::int32_t frames) {
    if (frames > 30)
        return;
    SuppressGameInput(frames);
    g_inputBuffer[0].previousHeld = 0;
    g_inputBuffer[0].held = 0;
    g_inputBuffer[0].pressedPriority = 0;
    g_inputBuffer[0].repeatedPresses = 0;
    g_inputBuffer[0].leftStickX = 0x80;
    g_inputBuffer[0].leftStickY = 0x80;
    g_inputBuffer[0].triggers[0] = 0;
    g_inputBuffer[0].triggers[1] = 0;
    PadControlTaskData *const control = static_cast<PadControlTaskData *>(HLib::GetTaskParameter(g_padControlTask));
    for (std::int32_t index = 0; index < 16; ++index)
        ResetProcessedInput(control->entries[index].input);
}
#endif

// @verified - d
void PrePadControl_Callback(void *) {
    if (g_prePadConfirmPulse) {
        g_prePadConfirmPulse = 0;
        LegacyInputSlot &input = g_inputBuffer[0];
        input.held = input.previousHeld = input.pressedPriority = input.repeatedPresses = 0x200;
        input.triggers[0] = input.triggers[1] = input.rightStickX = input.rightStickY = 0;
        input.leftStickX = input.leftStickY = 0x80;
    }
}

// @verified - d
HLib::Task *Queue_PrePadControlTask(std::int8_t nextFunctionIndex) {
    g_prePadConfirmPulse = 0;
    return HLib::EnqueueTaskWithParameter(PrePadControl_Callback, nextFunctionIndex, 2, 0, TASK_PPAD);
}

// @verified - d
void padEnableTask() {
    g_prePadConfirmPulse = 1;
}

} // namespace s2
} // namespace shenmue

#endif
