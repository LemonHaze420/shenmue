#include "s2/event_weather.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;


// @verified - p
EventWeatherControlWork *QueueEventWeatherControl() {
    eventWeatherControlTask = HLib::EnqueueTaskWithParameter(EventWeatherControlCallback, 3, 0x0b, sizeof(EventWeatherControlWork), eventTaskToken);
    EventWeatherControlWork *const work = static_cast<EventWeatherControlWork *>(HLib::GetTaskParameter(eventWeatherControlTask));
    work->task = eventWeatherControlTask;
    work->state = work->interpolationStarted = 0;
    work->skyAlpha = work->cloudAlpha = work->targetAlpha = 0;
    work->skyBlue = work->cloudBlue = work->targetBlue = 0;
    work->skyGreen = work->cloudGreen = work->targetGreen = 0;
    work->skyRed = work->cloudRed = work->targetRed = 0;
    work->skyColour = work->cloudColour = 0;
    for (std::size_t index = 0; index < 4; ++index)
        work->skyDelta[index] = work->skyCurrent[index] = work->cloudDelta[index] = work->cloudCurrent[index] = 0;
    work->frames = 360;
    work->targetAlpha = 255;
    work->targetBlue = 35;
    work->targetGreen = 20;
    work->targetRed = 0;
    return work;
}
// @verified - p
void EventWeatherControlCallback(void *data) {
    EventWeatherControlWork *const work = static_cast<EventWeatherControlWork *>(data);
    if (work->state == 0)
        InitializeEventWeatherColours(work);
    else if (work->state == 1)
        InterpolateEventWeatherColours(work);
    else if (work->state == 2)
        HLib::DestroyTaskInternal(work->task);
}
// @verified - p
void InitializeEventWeatherColours(EventWeatherControlWork *work) {
    SetWeatherUpdateEnabledTo(0);
    const std::uint32_t sky = GetWeatherCloudOrSkyColour(0), cloud = GetWeatherCloudOrSkyColour(1);
    work->cloudAlpha = static_cast<std::int32_t>(cloud >> 24);
    work->cloudBlue = static_cast<std::int32_t>((cloud >> 16) & 0xffU);
    work->skyAlpha = static_cast<std::int32_t>(sky >> 24);
    work->skyBlue = static_cast<std::int32_t>((sky >> 16) & 0xffU);
    ++work->state;
    work->skyGreen = static_cast<std::int32_t>((sky >> 8) & 0xffU);
    work->cloudGreen = static_cast<std::int32_t>((cloud >> 8) & 0xffU);
    work->skyRed = static_cast<std::int32_t>(sky & 0xffU);
    work->cloudRed = static_cast<std::int32_t>(cloud & 0xffU);
}
// @verified - p
void InterpolateEventWeatherColours(EventWeatherControlWork *work) {
    if (work->interpolationStarted == 0) {
        const float frames = static_cast<float>(work->frames);
        const float blue = static_cast<float>(work->targetBlue) + static_cast<float>(work->targetBlue);
        const float green = static_cast<float>(work->targetGreen) + static_cast<float>(work->targetGreen);
        const float red = static_cast<float>(work->targetRed) + static_cast<float>(work->targetRed);
        work->skyCurrent[0] = static_cast<float>(work->skyAlpha);
        work->skyCurrent[1] = static_cast<float>(work->skyBlue);
        work->skyCurrent[3] = static_cast<float>(work->skyRed);
        work->skyCurrent[2] = static_cast<float>(work->skyGreen);
        work->cloudCurrent[0] = static_cast<float>(work->cloudAlpha);
        work->cloudCurrent[1] = static_cast<float>(work->cloudBlue);
        work->cloudCurrent[2] = static_cast<float>(work->cloudGreen);
        work->interpolationStarted = 1;
        work->skyDelta[1] = (blue - work->skyCurrent[1]) / frames;
        work->cloudCurrent[3] = static_cast<float>(work->cloudRed);
        work->cloudDelta[3] = (red - work->cloudCurrent[3]) / frames;
        work->cloudDelta[1] = (blue - work->cloudCurrent[1]) / frames;
        work->cloudDelta[2] = (green - work->cloudCurrent[2]) / frames;
        work->skyDelta[3] = (red - work->skyCurrent[3]) / frames;
        work->skyDelta[2] = (green - work->skyCurrent[2]) / frames;
    }
    if (work->frames != 0) {
        work->skyCurrent[1] += work->skyDelta[1];
        work->skyCurrent[2] += work->skyDelta[2];
        work->skyCurrent[3] += work->skyDelta[3];
        work->skyAlpha = static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::int32_t>(work->skyCurrent[0])) << 24);
        work->skyBlue = (static_cast<std::int32_t>(work->skyCurrent[1]) & 0xff) << 16;
        work->skyGreen = (static_cast<std::int32_t>(work->skyCurrent[2]) & 0xff) << 8;
        work->skyRed = static_cast<std::int32_t>(work->skyCurrent[3]) & 0xff;
        work->skyColour = static_cast<std::uint32_t>(work->skyAlpha | work->skyBlue | work->skyGreen | work->skyRed);
        for (std::size_t index = 0; index < 4; ++index)
            work->cloudCurrent[index] += work->cloudDelta[index];
        work->cloudAlpha = static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::int32_t>(work->cloudCurrent[0])) << 24);
        work->cloudBlue = (static_cast<std::int32_t>(work->cloudCurrent[1]) & 0xff) << 16;
        work->cloudGreen = (static_cast<std::int32_t>(work->cloudCurrent[2]) & 0xff) << 8;
        work->cloudRed = static_cast<std::int32_t>(work->cloudCurrent[3]) & 0xff;
        work->cloudColour = static_cast<std::uint32_t>(work->cloudAlpha | work->cloudGreen | work->cloudBlue | work->cloudRed);
    } else {
        work->skyAlpha = work->cloudAlpha = work->targetAlpha;
        work->skyBlue = work->cloudBlue = work->targetBlue;
        work->skyGreen = work->cloudGreen = work->targetGreen;
        work->skyRed = work->cloudRed = work->targetRed;
        ++work->state;
    }
    SetCloudAndSkyColour(work->skyColour, work->cloudColour);
    --work->frames;
}
void EV_SetWeatherInstruction(std::int32_t instruction, std::int32_t value) {
    SetWeatherInstruction(static_cast<std::uint32_t>(instruction), value != 0);
}
void EV_FreeWAIR() {
    FreeWAIR();
}

void EV_UpdateWeatherForMinute(std::int32_t mode, std::int32_t enable, std::int32_t frames, float value) {
    UpdateWeatherForMinute(mode, enable, frames, value);
}

void EV_UpdateWeather() {
    UpdateWeather();
}

bool EV_InitializeOpeningAix() {
    return InitializeOpeningAix();
}

void EV_FinalizeOpeningAix() {
    FinalizeOpeningAix();
}

std::uint32_t EV_GetWeatherInstructions() {
    return GetCurrentWeatherInstruction();
}

} // namespace shenmue::s2
