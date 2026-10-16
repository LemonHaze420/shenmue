#include "s1/task/krep.hpp"

#include "s1/hlib.hpp"
#include "s1/input.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

namespace {

void updateRepeat(std::uint16_t held, std::uint16_t pressed, std::uint16_t mask, std::int32_t &counter, std::uint16_t &repeated) {
    if ((held & mask) == 0) {
        return;
    }
    if ((pressed & mask) != 0) {
        counter = 15;
    } else if (--counter < 0) {
        counter = 0;
        repeated |= mask;
    }
}

} // namespace

void HLTaskFunc_KeyRepeat(HLib::TaskData *taskData) {
    auto *const data = reinterpret_cast<KeyRepeatTaskData *>(taskData);
    InputSlot &input = g_inputState.slots[data->inputIndex];
    const std::uint16_t held = input.held;
    const std::uint16_t pressed = input.pressedPriority;
    std::uint16_t repeated = pressed;

    updateRepeat(held, pressed, 0x1000, data->repeatCountdown[0], repeated);
    updateRepeat(held, pressed, 0x2000, data->repeatCountdown[1], repeated);
    updateRepeat(held, pressed, 0x4000, data->repeatCountdown[2], repeated);
    updateRepeat(held, pressed, 0x8000, data->repeatCountdown[3], repeated);
    updateRepeat(held, pressed, 0x0040, data->repeatCountdown[4], repeated);
    updateRepeat(held, pressed, 0x0080, data->repeatCountdown[5], repeated);
    updateRepeat(held, pressed, 0x0400, data->repeatCountdown[6], repeated);

    if ((held & 0x0200) != 0) {
        if ((pressed & 0x0200) != 0) {
            data->repeatCountdown[7] = 15;
            input.repeatedPresses = repeated;
            return;
        }
        if (--data->repeatCountdown[7] < 0) {
            data->repeatCountdown[7] = 0;
            repeated |= 0x0200;
        }
    }
    input.repeatedPresses = repeated;
}

std::int64_t krepEnqueue(std::uint32_t inputIndex) {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(HLTaskFunc_KeyRepeat, 15, 2, SHEN_NAME(KREP));
    if (task) {
        if (!task->task_data_ptr) {
            auto *const data = static_cast<KeyRepeatTaskData *>(HLib_AllocHeapMemBlock(sizeof(KeyRepeatTaskData), SHEN_NAME(TASK)));
            HLib_EnableTokenCheck(data);
            task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
            if (data) {
                data->inputIndex = inputIndex;
                for (std::int32_t &counter : data->repeatCountdown)
                    counter = 15;
                data->trailingCountdown = 15;
                return 1;
            }
        }
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->initCallbackFnPtr = nullptr;
        task->autoInitTaskCallbackFnPtr = nullptr;
    }
    return 0;
}

} // namespace shenmue::s1
