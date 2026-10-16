#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name YCPA_QUEUED_TASK{SHEN_NAME(EVNT)};
struct YcpaLevelWork {
    std::int32_t cleanup;
    shen_name elevator;
    std::int32_t floor;
    std::int32_t delay;
};
struct YcpaDoorMotionWork {
    std::int32_t cleanup;
    shen_name door;
    std::int32_t animation;
    std::int32_t delay;
};
struct YcpaLampWork {
    std::int32_t cleanup;
    shen_name elevator;
    std::int32_t indicator;
    std::int32_t enabled;
    std::int32_t delay;
};
static_assert(sizeof(YcpaLevelWork) == 0x10 && sizeof(YcpaDoorMotionWork) == 0x10);
static_assert(sizeof(YcpaLampWork) == 0x14 && offsetof(YcpaLampWork, delay) == 0x10);
void YcpaSetLevelCallback(void *data);
void YcpaSetDoorMotionCallback(void *data);
void YcpaSetLampCallback(void *data);
void QueueYcpaSetLevel(shen_name elevator, std::int32_t floor, std::int32_t delay);
YcpaDoorMotionWork *QueueYcpaSetDoorMotion(shen_name door, std::int32_t animation, std::int32_t delay);
YcpaLampWork *QueueYcpaSetLamp(shen_name elevator, std::int32_t indicator, std::int32_t enabled, std::int32_t delay);
} // namespace shenmue::s2::areas
