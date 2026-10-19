#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {

struct EventWeatherControlWork {
    HLib::Task *task;
    std::uint8_t state, interpolationStarted;
    std::uint16_t reserved0a;
    std::uint32_t frames;
    std::uint32_t skyColour, cloudColour;
    std::int32_t skyAlpha, cloudAlpha, targetAlpha;
    std::int32_t skyBlue, cloudBlue, targetBlue;
    std::int32_t skyGreen, cloudGreen, targetGreen;
    std::int32_t skyRed, cloudRed, targetRed;
    float skyDelta[4], skyCurrent[4], cloudDelta[4], cloudCurrent[4];
};
static_assert(sizeof(EventWeatherControlWork) == 0x88);
static_assert(offsetof(EventWeatherControlWork, frames) == 0x0c && offsetof(EventWeatherControlWork, skyAlpha) == 0x18);
static_assert(offsetof(EventWeatherControlWork, skyBlue) == 0x24 && offsetof(EventWeatherControlWork, skyDelta) == 0x48);
static_assert(offsetof(EventWeatherControlWork, skyCurrent) == 0x58 && offsetof(EventWeatherControlWork, cloudDelta) == 0x68);
static_assert(offsetof(EventWeatherControlWork, cloudCurrent) == 0x78);
extern HLib::Task *eventWeatherControlTask;
EventWeatherControlWork *QueueEventWeatherControl();
void EventWeatherControlCallback(void *);
void InitializeEventWeatherColours(EventWeatherControlWork *);
void InterpolateEventWeatherColours(EventWeatherControlWork *);
void EV_SetWeatherInstruction(std::int32_t instruction, std::int32_t value);
void EV_FreeWAIR();
void EV_UpdateWeatherForMinute(std::int32_t mode, std::int32_t enable, std::int32_t frames, float value);
void EV_UpdateWeather();
bool EV_InitializeOpeningAix();
void EV_FinalizeOpeningAix();
std::uint32_t EV_GetWeatherInstructions();

} // namespace shenmue::s2
