#include "s1/task/film.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/date_time.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/render_passes.hpp"
#include "s1/screen_space.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/timing.hpp"
#include "s1/task/sond.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

void ApplyFadeRenderColour_Wrapper(
    void *firstColour, std::uint32_t red, std::int64_t reserved0, float reserved1, std::uint32_t colour, std::int64_t reserved2, float maxExtent);
void ApplyFadeToPacket(void *strip, float maxExtent);
using GetAspectRatioModeFunction = std::int32_t (*)();
GetAspectRatioModeFunction g_getAspectRatioModeFunction = nullptr;
std::uint32_t g_currentFilmColour = 0;
bool g_shouldUseTemporaryClearColour = false;
Colour4b g_temporaryClearColour{};

std::uint32_t ConstructColour(std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha) {
    const auto clamp = [](std::int32_t component) {
        if (component < 0) {
            return 0u;
        }
        if (component > 255) {
            return 255u;
        }
        return static_cast<std::uint32_t>(component);
    };
    return clamp(red) | (clamp(green) << 8) | (clamp(blue) << 16) | (clamp(alpha) << 24);
}

void ApplyFadeRenderColour_Wrapper(void *, std::uint32_t, std::int64_t, float, std::uint32_t colour, std::int64_t, float maxExtent) {
    if (!g_updateRenderFrameMode) {
        return;
    }
    if (maxExtent == 0.0f) {
        if (colour) {
            const auto byte = [colour](std::uint32_t shift) { return static_cast<std::uint8_t>(colour >> shift); };
            g_currentFilmColour = ConstructColour(byte(16), byte(8), byte(0), byte(24));
        }
        g_uiElements[kUiScreenFade].isActive = 1;
        return;
    }

    OriginalStrip strip;
    strip.vertexCount = 0;
    strip.normalizedY1 = 0.0f;
    strip.normalizedX2 = 0.0f;
    strip.normalizedY3 = 0.0f;
    strip.corner0 = colour;
    strip.y3 = 480.0f;
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    strip.x0 = 0.0f;
    strip.y0 = 0.0f;
    strip.x1 = 640.0f;
    strip.y1 = 0.0f;
    strip.x2 = 0.0f;
    strip.y2 = 480.0f;
    strip.x3 = 640.0f;
    if (!g_getAspectRatioModeFunction || !g_getAspectRatioModeFunction()) {
        const DisplayModeInfo &resolution = *GetScreenResolution();
        const float scale = 1.3333334f / (static_cast<float>(resolution.width) / static_cast<float>(resolution.height));
        const float offset = (1.0f - scale) * 640.0f * 0.5f;
        strip.x0 = scale * 0.0f + offset - offset;
        strip.x2 = strip.x0;
        strip.x1 = scale * 640.0f + offset + offset + 0.5f;
        strip.x3 = strip.x1;
    }
    strip.flags = 0x400;
    ApplyFadeToPacket(&strip, maxExtent);
}

void FILM_Callback(HLib::TaskData *taskData) {
    FILM_Callback_0(reinterpret_cast<std::byte *>(taskData) + 4);
}

void ConstructColours(HLib::Task *task, const ColourBlendPair *parameters, ConstructColourMode mode) {
    if (!task || !task->task_data_ptr || !parameters)
        return;
    auto *const data = reinterpret_cast<FilmColourInterpolation *>(reinterpret_cast<std::byte *>(task->task_data_ptr) + 4);

    if (mode == ConstructColourMode::CopyTemporaryClearColour && g_shouldUseTemporaryClearColour) {
        data->current[0] = static_cast<std::int32_t>(g_temporaryClearColour.red) << 16;
        const std::int32_t temporaryGreen = static_cast<std::int32_t>(g_temporaryClearColour.green) << 16;
        data->current[1] = temporaryGreen;
        data->current[2] = static_cast<std::int32_t>(g_temporaryClearColour.blue) << 16;


        data->current[3] = temporaryGreen;
    } else {
        const auto *const first = reinterpret_cast<const std::uint8_t *>(&parameters->colourA);
        for (std::size_t component = 0; component < 4; ++component) {
            data->current[component] = static_cast<std::int32_t>(first[component]) << 16;
        }
    }

    const auto *const second = reinterpret_cast<const std::uint8_t *>(&parameters->colourB);
    for (std::size_t component = 0; component < 4; ++component) {
        data->target[component] = static_cast<std::int32_t>(second[component]) << 16;
    }

    if (mode == ConstructColourMode::UseTemporaryClearColour) {
        g_shouldUseTemporaryClearColour = true;
        g_temporaryClearColour = parameters->colourB;
    } else {
        g_shouldUseTemporaryClearColour = false;
    }

    const std::int32_t frames = static_cast<std::int32_t>(parameters->interpolationFrames);
    for (std::size_t component = 0; component < 4; ++component) {
        data->delta[component] = frames ? (data->target[component] - data->current[component]) / frames : 0;
    }
    data->remainingFrames = frames;
    SetMainCallbackToInitForCleanup(task);
}

std::int32_t ctrlSetCurrentFilmFade() {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    HLib::Task *const filmTask = control->filmTask;
    auto *const data = reinterpret_cast<FilmColourInterpolation *>(reinterpret_cast<std::byte *>(filmTask->task_data_ptr) + 4);

    g_shouldUseTemporaryClearColour = false;
    std::memset(data->current, 0, sizeof(data->current));
    data->target[0] = 0xff0000;
    data->target[1] = 0;
    data->target[2] = 0;
    data->target[3] = 0;
    data->remainingFrames = 30;
    for (std::size_t component = 0; component < 4; ++component)
        data->delta[component] = (data->target[component] - data->current[component]) / 30;

    SetMainCallbackToInitForCleanup(filmTask);
    shenExecuteAICACommand(2720, 1, 30);
    shenExecuteAICACommand(2720, 2, 30);
    return shenExecuteAICACommand(2720, 3, 30);
}

bool FILM_Callback_0(void *interpolationData) {
    auto &data = *static_cast<FilmColourInterpolation *>(interpolationData);
    if (data.remainingFrames) {
        --data.remainingFrames;
        if (data.remainingFrames) {
            for (std::size_t component = 0; component < 4; ++component) {
                data.current[component] += data.delta[component];
            }
        } else {
            for (std::size_t component = 0; component < 4; ++component) {
                data.current[component] = data.target[component];
            }
        }
    }
    if (data.current[0]) {
        const auto component = [&data](std::size_t index) { return static_cast<std::uint8_t>(static_cast<std::uint32_t>(data.current[index]) >> 16); };
        data.packedColour = (static_cast<std::uint32_t>(component(0)) << 24) | (static_cast<std::uint32_t>(component(1)) << 16) |
            (static_cast<std::uint32_t>(component(2)) << 8) | component(3);
        ApplyFadeRenderColour_Wrapper(
            reinterpret_cast<void *>(static_cast<std::uintptr_t>(data.packedColour)), component(0), 0, 0.0f, data.packedColour, 0, 0.0f);
    }
    return data.remainingFrames != 0;
}

HLib::Task *FILM_Init(std::uint16_t nextFunctionIndex) {
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(FILM_Callback, static_cast<std::int16_t>(nextFunctionIndex), 2, SHEN_NAME(FILM));
    if (!task)
        return nullptr;
    if (task->task_data_ptr) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }

    void *const allocation = HLib_AllocHeapMemBlock(0x3c, SHEN_NAME(TASK));
    task->task_data_ptr = static_cast<HLib::TaskData *>(allocation);
    if (!allocation) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }
    HLib_EnableTokenCheck(allocation);

    std::memset(allocation, 0, 0x3c);
    *static_cast<std::uint32_t *>(allocation) = SHEN_NAME(SCFT);
    if (!task->initCallbackFnPtr) {
        task->initCallbackFnPtr = task->mainTaskCallbackFnPtr;
        task->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
        task->mainExecutionIterations = 0;
    }
    task->mainExecutionIterations = 0;
    return task;
}

bool FILM_HasTemporaryClearColour() {
    return g_shouldUseTemporaryClearColour;
}

Colour4b FILM_GetTemporaryClearColour() {
    return g_temporaryClearColour;
}

} // namespace shenmue::s1
