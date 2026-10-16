#include "s1/task/updb.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/sygh.hpp"

#include "s1/player_stats.hpp"
#include "s1/legacy_renderer.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace shenmue::s1 {
extern std::array<std::uint8_t, 128> g_shenmueFlags;
extern std::array<std::uint32_t, 5> g_saveDataRelated;
extern std::uint8_t g_bDebugLog;
extern std::array<float, 2> g_debugTextPosition;
std::uint8_t g_scenarioStepDebugDurationsEnabled = 0;
HLib::Task *g_updbTask = nullptr;

namespace {

std::array<std::int32_t, kScenarioStepStatMap.size()> g_stepElapsedSeconds{};
std::int32_t g_currentStepElapsedFrames = 0;
std::int32_t g_majorStepElapsedFrames = 0;
std::int16_t g_lastScenarioStepFrom = 0;
std::int16_t g_lastMajorScenarioStepFrom = 0;
std::int16_t g_lastMajorScenarioStepTo = 0;
std::int32_t g_lastScenarioStepDurationSeconds = 0;
std::int32_t g_lastMajorScenarioStepDurationSeconds = 0;

std::size_t StepMapIndex(std::int16_t step) noexcept {
    std::size_t index = 0;
    while (kScenarioStepStatMap[index].stepCode != step && kScenarioStepStatMap[index].stepCode != 0)
        ++index;
    return index;
}

bool GlobalStatSet(std::uint16_t bit) noexcept {
    const std::size_t byteIndex = bit >> 3;
    return byteIndex < g_shenmueFlags.size() && (g_shenmueFlags[byteIndex] & static_cast<std::uint8_t>(1U << (bit & 7U))) != 0;
}

} // namespace
void RequestStep(std::int16_t requestedStep) noexcept {
    g_lastScenarioStepFrom = requestedStep;
    g_lastMajorScenarioStepFrom = requestedStep;
    g_lastMajorScenarioStepTo = requestedStep;
    g_majorStepElapsedFrames = 0;
    g_lastMajorScenarioStepDurationSeconds = 0;
    g_currentStepElapsedFrames = 30 * g_stepElapsedSeconds[StepMapIndex(requestedStep)];


    g_lastScenarioStepDurationSeconds = g_currentStepElapsedFrames;
}

void UpdateScenarioProgressStep() {
    const std::int16_t previousStep = SCEN_GetCurrentScenarioStep();
    std::int16_t resolvedStep = previousStep;
    bool resolvedFromFlags = false;
    for (const ScenarioStepStatMapEntry &entry : kScenarioStepStatMap) {
        if (entry.stepCode == 0)
            break;
        if (entry.statBit >= 0 && GlobalStatSet(static_cast<std::uint16_t>(entry.statBit))) {
            resolvedStep = entry.stepCode;
            resolvedFromFlags = true;
            break;
        }
    }

    if (resolvedFromFlags) {
        if (previousStep > resolvedStep)
            RequestStep(resolvedStep);
        SCEN_SetCurrentScenarioStep(resolvedStep);
    }

    const std::int32_t currentStepFrames = ++g_currentStepElapsedFrames;
    const std::int32_t majorStepFrames = ++g_majorStepElapsedFrames;
    g_stepElapsedSeconds[StepMapIndex(previousStep)] = currentStepFrames / 30;
    if (resolvedStep != previousStep) {
        g_lastScenarioStepDurationSeconds = currentStepFrames / 30;
        g_lastScenarioStepFrom = previousStep;
        g_currentStepElapsedFrames = 30 * g_stepElapsedSeconds[StepMapIndex(resolvedStep)];
        if (resolvedStep / 10 > previousStep / 10) {
            g_majorStepElapsedFrames = 0;
            g_lastMajorScenarioStepFrom = previousStep;
            g_lastMajorScenarioStepTo = resolvedStep;
            g_lastMajorScenarioStepDurationSeconds = majorStepFrames / 30;
            SYGH_ApplyMajorStepMoveSkillProgression();
        }
        g_saveDataRelated[4] = 0;
        g_playerStats.recentItemKeyLatest = -1;
        g_playerStats.recentItemKeyPrevious = -1;
        g_playerStats.recentItemKeyOldest = -1;
        g_playerStats.scenarioStepUniqueItemCount = 0;
    }

    if (g_bDebugLog) {
        std::array<char, 128> message{};
        g_debugTextPosition = {14.0f, 2.0f};
        std::snprintf(message.data(), message.size(), "STEP %2d.%1d", resolvedStep / 10, resolvedStep % 10);
        (void)QueueDebugText(g_debugTextPosition[0], g_debugTextPosition[1], message.data());
        if (g_scenarioStepDebugDurationsEnabled) {
            g_debugTextPosition = {9.0f, 3.0f};
            std::snprintf(message.data(), message.size(), "%2d.%1d>%2d.%1d %2d:%02d:%02d", g_lastScenarioStepFrom / 10, g_lastScenarioStepFrom % 10,
                resolvedStep / 10, resolvedStep % 10, g_lastScenarioStepDurationSeconds / 3600, g_lastScenarioStepDurationSeconds % 3600 / 60,
                g_lastScenarioStepDurationSeconds % 3600 % 60);
            (void)QueueDebugText(g_debugTextPosition[0], g_debugTextPosition[1], message.data());
            g_debugTextPosition = {9.0f, 4.0f};
            std::snprintf(message.data(), message.size(), "%2d.%1d>%2d.%1d %2d:%02d:%02d", g_lastMajorScenarioStepFrom / 10, g_lastMajorScenarioStepFrom % 10,
                g_lastMajorScenarioStepTo / 10, g_lastMajorScenarioStepTo % 10, g_lastMajorScenarioStepDurationSeconds / 3600,
                g_lastMajorScenarioStepDurationSeconds % 3600 / 60, g_lastMajorScenarioStepDurationSeconds % 3600 % 60);
            (void)QueueDebugText(g_debugTextPosition[0], g_debugTextPosition[1], message.data());
        }
    }
}

void UPDB_Callback(HLib::TaskData *) {
    UpdateScenarioProgressStep();
}
HLib::Task *UPDB_Init() {
    g_updbTask = HLib_EnqueueTaskWithoutParameter(UPDB_Callback, 3, 0, SHEN_NAME(UPDB));
    return g_updbTask;
}

void UPDB_Shutdown() {
    if (g_updbTask)
        HLib_DestroyTask(g_updbTask);
    g_updbTask = nullptr;
}

HLib::Task *UPDB_GetTask() {
    return g_updbTask;
}


} // namespace shenmue::s1
