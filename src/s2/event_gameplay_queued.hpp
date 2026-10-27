#pragma once

#include "s2/hlib.hpp"
#include "s2/face_expression.hpp"
#include "common/types.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name GAMEPLAY_QUEUED_TASK{SHEN_NAME(EVNT)};

struct EventCling {
    std::uint32_t cleanup;
    shen_name target;
    shen_name character;
    std::int32_t bone;
    Vector3 position;
    Vector3i rotation;
    std::uint32_t mode;
    float delay;
};
struct EventMoneyCount {
    std::uint32_t cleanup;
    std::uint32_t cash;
    float delay;
};
static_assert(sizeof(EventCling) == 0x30);
static_assert(offsetof(EventCling, target) == 4);
static_assert(offsetof(EventCling, position) == 0x10);
static_assert(offsetof(EventCling, rotation) == 0x1c);
static_assert(offsetof(EventCling, mode) == 0x28);
static_assert(offsetof(EventCling, delay) == 0x2c);
static_assert(sizeof(EventMoneyCount) == 0x0c);
static_assert(offsetof(EventMoneyCount, cash) == 4);
static_assert(offsetof(EventMoneyCount, delay) == 8);
struct EventFaceLooks {
    std::uint32_t cleanup;
    shen_name character;
    std::uint32_t expression;
    std::int32_t duration;
    float intensity, delay;
};
static_assert(sizeof(EventFaceLooks) == 0x18);
static_assert(offsetof(EventFaceLooks, duration) == 12);
static_assert(offsetof(EventFaceLooks, delay) == 20);

void EVT_AT_Cling(void *taskData);
void EVT_AT_StartMoneyCount_Callback(void *taskData);
std::int64_t PATI_StartMoneyCount(std::uint32_t cash, std::int32_t delay);
void EVT_AT_SetFaceLooks_Callback(void *taskData);
void EVT_AT_SetFaceLooks(shen_name character, std::uint32_t expression, std::int32_t duration, float intensity, std::int32_t delay);

} // namespace shenmue::s2
