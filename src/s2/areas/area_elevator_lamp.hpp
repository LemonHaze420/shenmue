#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
enum { ELEVATOR_LAMP_CHARACTER = SHEN_NAME4('T', 'L', 'I', 'G') };
enum { AREA_QF39 = SHEN_NAME4('Q', 'F', '3', '9') };
typedef struct ElevatorLampChoice {
    shen_name elevator;
    int flag;
} ElevatorLampChoice;
static ElevatorLampChoice ELEVATOR_LAMP_CHOICES[3] = {
    {SHEN_NAME4('R', '5', '9', '9'), 627}, {SHEN_NAME4('R', '5', '9', '8'), 629}, {SHEN_NAME4('R', '5', '9', '7'), 630}};
int SetElevatorLamp(shen_name elevator, int enabled);
int ConfigureAreaElevatorLamp(int enabled);
#else
#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/area_names.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2::areas {

inline constexpr shen_name ELEVATOR_LAMP_CHARACTER{SHEN_NAME(TLIG)};
struct ElevatorLampChoice {
    shen_name elevator;
    std::int32_t flag;
};
inline constexpr std::array<ElevatorLampChoice, 3> ELEVATOR_LAMP_CHOICES{{
    {.elevator = SHEN_NAME(R599), .flag = 627},
    {.elevator = SHEN_NAME(R598), .flag = 629},
    {.elevator = SHEN_NAME(R597), .flag = 630},
}};
inline constexpr Vector3 ELEVATOR_LAMP_OFFSET{.x = 0.0f, .y = 2.6949999f, .z = 0.2f};
inline constexpr Vector4 ELEVATOR_LAMP_COLOUR{.x = 1.0f, .y = 1.0f, .z = 0.80000001f, .w = 0.5f};
std::int64_t SetElevatorLamp(shen_name elevator, std::int32_t enabled);
std::int64_t ConfigureAreaElevatorLamp(std::int32_t enabled = 1);
void AreaElevatorLampInit();
void AreaElevatorLampMainLoop(void *data);
void AreaElevatorLampCleanup(std::int32_t afternoon, std::uint32_t areaIndex);

} // namespace shenmue::s2::areas

#endif
