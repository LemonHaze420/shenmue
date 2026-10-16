#include "s1/task/ppad.hpp"

#include "s1/input.hpp"

#include <bit>
#include <cstdint>

namespace shenmue::s1 {

std::int32_t g_ppadNeedsSetup = 0;

void PPAD_Init() {
    g_ppadNeedsSetup = 0;
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(PPAD_Callback, 2, 2, SHEN_NAME(PPAD));
    if (task) {
        if (!task->task_data_ptr) {
            void *const data = HLib_AllocHeapMemBlock(0, SHEN_NAME(TASK));
            HLib_EnableTokenCheck(data);
            task->task_data_ptr = static_cast<HLib::TaskData *>(data);
            if (data)
                return;
        }
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
    }
}

void PPAD_Callback(HLib::TaskData *) {
    if (!g_ppadNeedsSetup)
        return;
    g_ppadNeedsSetup = 0;
    InputSlot &input = g_inputState.slots[0];
    input.pressedPriority = 0x200;
    input.repeatedPresses = 0x200;
    input.held = 0x200;
    input.previousHeld = 0x200;
    input.rightStick = 0x8080;
    input.leftStick &= 0xffff0000U;
    input.triggers = 0;
}

} // namespace shenmue::s1
