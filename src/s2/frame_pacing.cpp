#include "s2/frame_pacing.hpp"

#include "platform/performance_counter.hpp"
#include "d3t/content.hpp"
#include "d3t/object_store.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/game_runtime.hpp"
#include "s2/sd_probe.hpp"
#include "s2/ui.hpp"
#include "s2/ui_transition.hpp"
#include "s2/texture_source.hpp"

#include <atomic>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdio>

    namespace shenmue::s2 {
    LoadingScreenUpdate g_updateLoadingScreen{};
    LoadingScreenToggle g_toggleLoadingScreen{};
    bool framePacingTimerUpdated{};
    std::int32_t g_skipArchiveReadWait{};
    std::uint8_t g_stabilizeFps{};
    float g_stabilizedFrameTime{};
    double measuredFrameMilliseconds{};
    char framePerformanceText[64]{};
    std::int32_t framePerformanceSampleState{};
    std::int32_t minimumUnlockedWaitSeconds{};
    std::int32_t maximumUnlockedWaitSeconds{};
    bool waitForRendererReadiness{};
    namespace {

    std::int32_t numFramesProcessed{};
    std::uint32_t numFramesWaited{};
    // normalized by SetupBuffers
    bool rendererInitialized{};
    bool retireFramePacingImmediately{};
    bool forceFramePacingWait{};
    std::chrono::steady_clock::time_point framePacingStart{};

    bool HasPendingContentRemappings() noexcept {
        for (const d3t::D3TContentSlot &slot : d3t::g_d3tContentSlots) {
            const std::int32_t state = slot.state.load(std::memory_order_seq_cst);
            if (state != 0 && state != 12 && state != 13)
                return true;
        }
        return false;
}

} // namespace
void FlushFramePacingRenderQueue() {
    if (numFramesProcessed == 0)
        return;
    ExecuteCurrentCommandList();
    bool canRenderUi = BeginD3TRenderPass(false);
    bool scenePresented = canRenderUi && d3t::PresentSceneFrame();
    if (!scenePresented) {
        ExecuteCurrentCommandList();
        canRenderUi = BeginD3TRenderPass(true);
        if (canRenderUi)
            d3t::PresentSceneFrame();
    }
    ConsumeFrames();
    if (canRenderUi)
        RenderUIContents();
    PresentD3TFrame(GetNumFramesProcessed() == 0 ? 1 : 0);
    ProcessD3TMessages();
}

void UpdateFramePacing() {
    if (numFramesProcessed != 0)
        return;
    framePacingTimerUpdated = true;
    framePacingStart = std::chrono::steady_clock::now();
    if (g_toggleLoadingScreen)
        g_toggleLoadingScreen(numFramesProcessed, 1);
    numFramesProcessed = 1;
    numFramesWaited = 0;
    FlushFramePacingRenderQueue();
}

void UpdateCurrentFramesRendered(std::uint32_t updateUi) {
    if (numFramesProcessed == 0)
        return;
    if (retireFramePacingImmediately || updateUi == 0) {
        if (g_toggleLoadingScreen)
            g_toggleLoadingScreen(numFramesProcessed, 0);
        numFramesProcessed = 0;
        numFramesWaited = 0;
        framePacingTimerUpdated = false;
        return;
    }
    if (g_toggleLoadingScreen)
        g_toggleLoadingScreen(numFramesProcessed, 3);
    numFramesProcessed = 3;
    numFramesWaited = 0;
    framePacingTimerUpdated = false;
}

std::int32_t GetNumFramesProcessed() noexcept {
    return numFramesProcessed;
}

// @verified - p
std::int32_t ConsumeFrames() {
    if (g_updateLoadingScreen)
        g_updateLoadingScreen();
    const std::int32_t current = numFramesProcessed;
    if (current == 0)
        return 0;

    ++numFramesWaited;
    if (current == 3 && numFramesWaited >= 6U * static_cast<std::uint32_t>(g_sdProbeCount)) {
        if (g_toggleLoadingScreen)
            g_toggleLoadingScreen(3, 0);
        numFramesProcessed = 0;
        numFramesWaited = 0;
        return 0;
    }
    return current == 3 ? g_sdProbeCount : current;
}

void WaitForNextFrameWithCallback(const std::uint32_t *handles, std::int32_t count, FramePacingDecision getTotalFramesToRender) {
    if (numFramesProcessed == 0)
        return;
    if (g_toggleLoadingScreen)
        g_toggleLoadingScreen(numFramesProcessed, 2);
    numFramesProcessed = 2;
    numFramesWaited = 0;



    const std::uint32_t *handleCursor = handles;
    std::int32_t handlesRemaining = count;
    for (;;) {
        d3t::RunTask(0);
        d3t::ExecuteADynamicFunctionOnce();
        d3t::ProcessRemappings();

        const bool objectStorePending = d3t::ProcessObjectStore(true, -1);
        const bool textureSourcesPending = PumpEffectTextureSources();
        d3t::ProcessRemappings();
        const bool contentRemappingPending = HasPendingContentRemappings();
        bool ready = !objectStorePending && !textureSourcesPending && !contentRemappingPending && d3t::g_d3tTaskCount.load(std::memory_order_seq_cst) == 0 &&
            d3t::g_d3tDeferredCompletionCount.load(std::memory_order_seq_cst) == 0;

        if (handleCursor && handlesRemaining > 0) {
            do {
                const std::uint32_t handle = *handleCursor++;
                ready &= handle == 0 || !d3t::d3t_IsRenderHandlePending(handle);
            } while (--handlesRemaining > 0);
        }

        if (getTotalFramesToRender) {
            const std::int32_t decision = getTotalFramesToRender();
            ready &= decision != 1;
            if (decision == 0)
                break;
        }
        if (ready)
            break;
        FlushFramePacingRenderQueue();
    }
}

void SetFramePacingRendererInitialized(bool initialized) noexcept {
    rendererInitialized = initialized;
    if (!initialized) {
        numFramesProcessed = 0;
        numFramesWaited = 0;
        framePacingTimerUpdated = false;
    }
}

std::int32_t GetTotalFramesToRender() noexcept {
    UIAnim_UpdateDeltaTime(GetFrameTime());
    const auto now = std::chrono::steady_clock::now();
    const bool unlockedFramerate = numFramesProcessed != 0;
    if (!rendererInitialized)
        return 2;

    const float elapsedSeconds = framePacingTimerUpdated ? std::chrono::duration<float>(now - framePacingStart).count() : 0.0f;
    if (forceFramePacingWait || (minimumUnlockedWaitSeconds > 0 && unlockedFramerate && static_cast<float>(minimumUnlockedWaitSeconds) >= elapsedSeconds)) {
        return 1;
    }
    if ((maximumUnlockedWaitSeconds > 0 && unlockedFramerate && elapsedSeconds >= static_cast<float>(maximumUnlockedWaitSeconds)) ||
        !waitForRendererReadiness) {
        return 0;
    }
    return 2;
}

float GetLoadingElapsedSeconds() noexcept {
    if (numFramesProcessed == 0)
        return 0.0f;
    const std::int64_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - framePacingStart).count();
    return static_cast<float>(milliseconds) / 1000.0f;
}
// @verified - p
std::int64_t ProfileFramePerformance() {
    static const std::int64_t frequency = platform::performanceCounterFrequency();
    const std::int64_t now = platform::performanceCounter();
    static std::int64_t previous = now;
    const std::uint64_t elapsed = static_cast<std::uint64_t>(now) - static_cast<std::uint64_t>(previous);
    const std::int64_t scaled = std::bit_cast<std::int64_t>(1000U * elapsed);
    double milliseconds = static_cast<double>(scaled);
    if (scaled < 0)
        milliseconds += 18446744073709551616.0;
    double divisor = static_cast<double>(frequency);
    if (frequency < 0)
        divisor += 18446744073709551616.0;
    measuredFrameMilliseconds = milliseconds / divisor;
    const std::uint32_t fps = measuredFrameMilliseconds <= 0.0000001192092895507812 ?
        0U :
        static_cast<std::uint32_t>(static_cast<std::int64_t>(std::ceil(1000.0 / measuredFrameMilliseconds)));
    std::snprintf(framePerformanceText, sizeof(framePerformanceText), "Frame: %0.2fms - FPS: %d", measuredFrameMilliseconds, static_cast<std::int32_t>(fps));
    g_stabilizedFrameTime = static_cast<float>(measuredFrameMilliseconds * 0.001);
    previous = platform::performanceCounter();
    framePerformanceSampleState = 0;
    g_stabilizedFrameTime = static_cast<float>(measuredFrameMilliseconds * 0.001);
    return previous;
}

// @verified - p
float GetFrameTime() noexcept {
    return g_stabilizeFps != 0 ? g_stabilizedFrameTime : 0.033333335f;
}

// @verified - p
float shenGetScaledDeltaTickTime(float frames) noexcept {
    if (g_stabilizeFps != 0)
        return frames * 30.0f * g_stabilizedFrameTime;
    return frames;
}

std::int64_t ReturnWindowHandle() noexcept {


    return 1;
}

} // namespace shenmue::s2
