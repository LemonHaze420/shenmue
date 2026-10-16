#include "s1/task/padc.hpp"

#include "s1/input.hpp"

#include <bit>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

HLib::Task *g_pPadCaptureTask = nullptr;
std::uint16_t g_xinputReserved = 1;

namespace {

void CopyCurrentInput(PadCaptureEntry &entry) {
    const InputSlot &input = g_inputState.slots[0];
    std::memcpy(&entry.enabled, &input, sizeof(input));
    entry.xinputReserved = g_xinputReserved;
}

void ClearInput(PadCaptureEntry &entry) {
    entry.previousHeld = entry.held;
    entry.unknownMask = 0;
    entry.held = 0;
    entry.pressedPriority = 0;
    entry.repeatedPresses = 0;
    entry.triggers = 0;
    entry.rightStick = 0x8080;
}
void ClearInputForTaskCallback(PadCaptureEntry &entry) {
    entry.previousHeld = entry.held;
    entry.unknownMask = 0;
    entry.held = 0;
    entry.pressedPriority = 0;
    entry.repeatedPresses = 0;
    entry.triggers = 0;
    entry.rightStick = 0x8080;
}

void InitializeEntry(PadCaptureEntry &entry) {
    entry.flags = 0;
    entry.held = 0;
    entry.previousHeld = 0;
    entry.unknownMask = 0;
    entry.pressedPriority = 0;
    entry.repeatedPresses = 0;
    entry.triggers = 0;
    entry.rightStick = 0x8080;
}

void ScheduleFailedTaskForDestruction(HLib::Task *task) {
    if (!task)
        return;
    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}

} // namespace

PadCaptureTaskData *PADC_CurrentData() {
    return g_pPadCaptureTask && g_pPadCaptureTask->task_data_ptr ? reinterpret_cast<PadCaptureTaskData *>(g_pPadCaptureTask->task_data_ptr) : nullptr;
}

PadCaptureTaskData *PADC_Init() {
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(PADC_Task_Callback, 2, 2, SHEN_NAME(PADC));
    if (task) {
        if (!task->task_data_ptr) {
            auto *const data = static_cast<PadCaptureTaskData *>(HLib_AllocHeapMemBlock(sizeof(PadCaptureTaskData), SHEN_NAME(TASK)));
            HLib_EnableTokenCheck(data);
            task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
            if (!data) {
                ScheduleFailedTaskForDestruction(task);
                task = nullptr;
            }
        } else {
            ScheduleFailedTaskForDestruction(task);
            task = nullptr;
        }
    }

    g_pPadCaptureTask = task;
    PadCaptureTaskData *const data = PADC_CurrentData();

    data->currentBoundEntry = nullptr;
    for (PadCaptureEntry &entry : data->entries)
        InitializeEntry(entry);
    return data;
}

void PADC_Shutdown() {
    HLib::Task *const task = g_pPadCaptureTask;
    PadCaptureTaskData *const data = PADC_CurrentData();
    if (data) {
        data->currentBoundEntry = nullptr;
        for (PadCaptureEntry &entry : data->entries) {
            ClearInput(entry);
            entry.flags = 0;
        }
    }
    g_pPadCaptureTask = nullptr;
    if (task)
        HLib_DestroyTask(task);
}

void PADC_Task_Callback(HLib::TaskData *taskData) {
    auto *const data = reinterpret_cast<PadCaptureTaskData *>(taskData);

    for (PadCaptureEntry &entry : data->entries) {
        if ((entry.flags & 1U) == 0)
            continue;
        if ((entry.flags & 2U) != 0)
            CopyCurrentInput(entry);
        else
            ClearInputForTaskCallback(entry);
    }
}

PadCaptureEntry *AllocateClearedPADCEntry() {
    PadCaptureTaskData *const data = PADC_CurrentData();
    std::size_t index = 0;
    while (index < 16 && (data->entries[index].flags & 1U) != 0) {
        ++index;
    }

    PadCaptureEntry *const entry = &data->entries[index];
    entry->flags |= 1U;
    entry->held = 0;
    entry->previousHeld = 0;
    entry->unknownMask = 0;
    entry->pressedPriority = 0;
    entry->repeatedPresses = 0;
    entry->triggers = 0;
    entry->rightStick = 0x8080;
    return entry;
}

PadCaptureEntry *shenUpdateInputsInto(PadCaptureEntry *entry) {
    PadCaptureTaskData *const data = PADC_CurrentData();
    if (entry && (entry->flags & 1U) == 0)
        entry = nullptr;

    PadCaptureEntry *const previous = data->currentBoundEntry;
    if (entry == previous)
        return previous;
    if (previous) {
        ClearInput(*previous);
        previous->flags &= ~2U;
    }
    if (entry) {
        CopyCurrentInput(*entry);
        entry->flags |= 2U;
    }
    data->currentBoundEntry = entry;
    return previous;
}

bool ReleasePADCEntry(PadCaptureEntry *entry, PadCaptureEntry *restoreEntry) {
    if (!entry)
        return false;
    PadCaptureTaskData *const data = PADC_CurrentData();
    if (restoreEntry == entry || (restoreEntry && (restoreEntry->flags & 1U) == 0)) {
        restoreEntry = nullptr;
    }
    if (data && data->currentBoundEntry == entry)
        (void)shenUpdateInputsInto(restoreEntry);
    else
        entry->flags &= ~2U;

    ClearInput(*entry);
    entry->flags = 0;
    return true;
}

PadCaptureEntry *BindMapControlPADCEntry() {
    PadCaptureEntry *const entry = AllocateClearedPADCEntry();
    if (!entry)
        return nullptr;
    (void)shenUpdateInputsInto(entry);
    return entry;
}

std::uint16_t ReadPadCaptureMode(const PadCaptureEntry *entry, std::uint32_t mode) {
    if (!entry)
        return ReadInputSlotMode(0, mode);
    switch (mode) {
    case 0:
        return entry->held;
    case 1:
        return entry->pressedPriority;
    case 2:
        return entry->repeatedPresses;
    case 3:
        return static_cast<std::uint8_t>(entry->triggers);
    case 4:
        return static_cast<std::uint8_t>(entry->triggers >> 8U);
    case 5:
        return static_cast<std::uint8_t>(entry->rightStick);
    case 6:
        return static_cast<std::uint8_t>(entry->rightStick >> 8U);
    case 7:
        return static_cast<std::uint16_t>(entry->leftStick >> 16U);
    case 8:
        return ApplyCurrentInputForGameSwap(entry->pressedPriority);
    case 9:
        return ApplyCurrentInputForGameSwap(static_cast<std::uint16_t>(entry->leftStick >> 16U));
    default:
        return 0;
    }
}

} // namespace shenmue::s1
