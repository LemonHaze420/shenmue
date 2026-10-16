#include "s2/areas/ycpa_queued.hpp"
#include "s2/door.hpp"
#include "s2/elevator.hpp"
#include "s2/game_runtime.hpp"
namespace shenmue::s2::areas {
// @verified - p
void YcpaSetLevelCallback(void *data) {
    YcpaLevelWork &work = *static_cast<YcpaLevelWork *>(data);
    const std::int32_t remaining = work.delay;
    work.delay = static_cast<std::int32_t>(static_cast<std::uint32_t>(remaining) - 1U);
    if (remaining <= 0) {
        SetCurrentElevatorFloor(work.elevator, static_cast<std::int16_t>(work.floor));
        if (work.cleanup) {
            UnregisterEventTask(HLib::GetCurrentTask());
            HLib::DestroyCurrentTask();
        }
    }
}
// @verified - p
void QueueYcpaSetLevel(shen_name elevator, std::int32_t floor, std::int32_t delay) {
    if (!delay) {
        SetCurrentElevatorFloor(elevator, static_cast<std::int16_t>(floor));
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(YcpaSetLevelCallback, 6, 0x0b, 0x10, YCPA_QUEUED_TASK);
    RegisterEventTask(task);
    YcpaLevelWork &work = *static_cast<YcpaLevelWork *>(HLib::GetTaskParameter(task));
    work.cleanup = 1;
    work.elevator = elevator;
    work.floor = floor;
    work.delay = delay;
}
// @verified - p
void YcpaSetDoorMotionCallback(void *data) {
    YcpaDoorMotionWork &work = *static_cast<YcpaDoorMotionWork *>(data);
    const std::int32_t remaining = work.delay;
    work.delay = static_cast<std::int32_t>(static_cast<std::uint32_t>(remaining) - 1U);
    if (remaining <= 0) {
        doorPlayRigidMotion(work.door, static_cast<std::uint16_t>(work.animation), 0, 1);
        if (work.cleanup) {
            UnregisterEventTask(HLib::GetCurrentTask());
            HLib::DestroyCurrentTask();
        }
    }
}
// @verified - p
YcpaDoorMotionWork *QueueYcpaSetDoorMotion(shen_name door, std::int32_t animation, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(YcpaSetDoorMotionCallback, 6, 0x0b, 0x10, YCPA_QUEUED_TASK);
    RegisterEventTask(task);
    YcpaDoorMotionWork *work = static_cast<YcpaDoorMotionWork *>(HLib::GetTaskParameter(task));
    work->animation = animation;
    work->delay = delay;
    work->cleanup = 1;
    work->door = door;
    return work;
}
// @verified - p
void YcpaSetLampCallback(void *data) {
    YcpaLampWork &work = *static_cast<YcpaLampWork *>(data);
    const std::int32_t remaining = work.delay;
    work.delay = static_cast<std::int32_t>(static_cast<std::uint32_t>(remaining) - 1U);
    if (remaining <= 0) {
        SetElevatorIndicatorFlag(work.elevator, work.indicator, work.enabled);
        if (work.cleanup) {
            UnregisterEventTask(HLib::GetCurrentTask());
            HLib::DestroyCurrentTask();
        }
    }
}
// @verified - p
YcpaLampWork *QueueYcpaSetLamp(shen_name elevator, std::int32_t indicator, std::int32_t enabled, std::int32_t delay) {

    HLib::Task *task = HLib::EnqueueTaskWithParameter(YcpaSetLampCallback, 6, 0x0b, 0x10, YCPA_QUEUED_TASK);
    RegisterEventTask(task);
    YcpaLampWork *work = static_cast<YcpaLampWork *>(HLib::GetTaskParameter(task));
    work->elevator = elevator;
    work->indicator = indicator;
    work->enabled = enabled;
    work->cleanup = 1;
    work->delay = delay;
    return work;
}
} // namespace shenmue::s2::areas
