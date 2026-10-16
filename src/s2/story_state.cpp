#include "s2/story_state.hpp"
#include "s2/date_time.hpp"
#include "s2/debug_text.hpp"
#include "s2/flag.hpp"
#include "s2/player.hpp"

#include <algorithm>
#include <array>

#include <cstdint>
#include <cstring>

namespace shenmue::s2 {

std::int32_t g_storyStep = 0;

namespace {



std::int32_t previousStoryStep = 0;
std::int32_t stepRangeStart = 0;
std::int32_t stepRangeEnd = 0;
std::uint32_t stepElapsedFrames = 0;
std::uint64_t stepRangeElapsedFrames = 0;
std::int32_t stepElapsedSeconds = 0;
std::uint32_t stepDebugFrames = 0;

std::uint8_t stepRangeDebugDisplay = 0;


std::int32_t currentCash{};

struct ScenarioGameplayFlagState {
    std::array<std::uint32_t, 10> words{};
    std::int32_t initialized{};
    std::int32_t day{};
    std::int32_t step{};
};
static_assert(sizeof(ScenarioGameplayFlagState) == 0x34);
static_assert(offsetof(ScenarioGameplayFlagState, initialized) == 0x28);
static_assert(offsetof(ScenarioGameplayFlagState, day) == 0x2c);
static_assert(offsetof(ScenarioGameplayFlagState, step) == 0x30);
ScenarioGameplayFlagState scenarioGameplayFlags{};

std::uint32_t &ScenarioEventStateWord(std::uint16_t objectId) {
    if (objectId == 5)
        return scenarioGameplayFlags.words[1];
    if (objectId == 6)
        return scenarioGameplayFlags.words[2];
    return scenarioGameplayFlags.words[0];
}

bool ScenarioEventStateField(std::uint8_t mode, std::uint8_t &shift, std::uint8_t &mask, std::int32_t &assignmentMaximum) {
    switch (mode) {
    case 7:
        shift = 0;
        mask = 0xff;
        assignmentMaximum = 85;
        return true;
    case 8:
        shift = 8;
        mask = 0x3f;
        assignmentMaximum = 31;
        return true;
    case 9:
        shift = 14;
        mask = 0x0f;
        assignmentMaximum = 4;
        return true;
    case 10:
        shift = 18;
        mask = 0x03;
        assignmentMaximum = 1;
        return true;
    case 11:
        shift = 20;
        mask = 0x07;
        assignmentMaximum = 3;
        return true;
    case 12:
        shift = 23;
        mask = 0x03;
        assignmentMaximum = 1;
        return true;
    case 13:
        shift = 25;
        mask = 0x1f;
        assignmentMaximum = 15;
        return true;
    case 14:
        shift = 30;
        mask = 0x03;
        assignmentMaximum = 1;
        return true;
    default:
        return false;
    }
}

} // namespace

std::uint32_t GetCurrentStep() {
    return static_cast<std::uint32_t>(g_storyStep);
}

std::int32_t GetCurrentCash() {
    return currentCash;
}

void SetCurrentCash(std::int32_t cash) {
    currentCash = cash;
}

void UpdateGameplayFlags() {
    if (!scenarioGameplayFlags.initialized) {
        scenarioGameplayFlags.words.fill(1);
        scenarioGameplayFlags.initialized = 1;
    }
    DateTime now{};
    GetCurrentDateTime(now);
    if (scenarioGameplayFlags.day != now.day) {
        for (std::uint32_t &word : scenarioGameplayFlags.words)
            word &= 0xfe0fffffU;
        scenarioGameplayFlags.day = now.day;
    }
    if (scenarioGameplayFlags.step != g_storyStep) {
        for (std::uint32_t &word : scenarioGameplayFlags.words)
            word &= 0x3fffffffU;
        scenarioGameplayFlags.step = g_storyStep;
    }
}

void CopyShenmueFlagsTo(std::span<std::byte, 0x34> output) noexcept {
    std::memcpy(output.data(), &scenarioGameplayFlags, sizeof(scenarioGameplayFlags));
}

void SetShenmueFlagsTo(std::span<const std::byte, 0x34> input) noexcept {
    std::memcpy(&scenarioGameplayFlags, input.data(), sizeof(scenarioGameplayFlags));
}

bool ResolveScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t &value) {
    std::uint8_t shift{};
    std::uint8_t mask{};
    std::int32_t assignmentMaximum{};
    if (!ScenarioEventStateField(mode, shift, mask, assignmentMaximum)) {
        return false;
    }
    value = static_cast<std::int32_t>((ScenarioEventStateWord(objectId) >> shift) & mask);
    return true;
}

bool AssignScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t value) {
    std::uint8_t shift{};
    std::uint8_t mask{};
    std::int32_t assignmentMaximum{};
    if (!ScenarioEventStateField(mode, shift, mask, assignmentMaximum))
        return false;
    const std::uint32_t fieldMask = static_cast<std::uint32_t>(mask) << shift;
    std::uint32_t &word = ScenarioEventStateWord(objectId);
    word = (word & ~fieldMask) | ((static_cast<std::uint32_t>(std::clamp(value, 0, assignmentMaximum)) & mask) << shift);
    return true;
}


void UpdateStepDebugCounters() noexcept {
    ++stepDebugFrames;
    ++stepElapsedFrames;
    stepElapsedSeconds = static_cast<std::int32_t>(stepElapsedFrames) / 30;
    if (!g_systemManageDebugDisplay || ReturnZero() != 0)
        return;
    SetDebugStringPosition(18, 2);
    DebugTextPrintf("%2d.%04d", g_storyStep / 10000, g_storyStep % 10000);
    if (!stepRangeDebugDisplay)
        return;
    std::int32_t elapsed = static_cast<std::int32_t>(stepRangeElapsedFrames);
    std::int32_t hours = elapsed / 3600;
    std::int32_t minutes = elapsed % 3600 / 60;
    std::int32_t seconds = elapsed % 3600 % 60;
    SetDebugStringPosition(9, 3);
    DebugTextPrintf(
        "%2d.%1d>%2d.%1d %2d:%02d:%02d", stepRangeStart / 10000, stepRangeStart % 10000, g_storyStep / 10000, g_storyStep % 10000, hours, minutes, seconds);
    elapsed = static_cast<std::int32_t>(stepRangeElapsedFrames >> 32U);
    hours = elapsed / 3600;
    minutes = elapsed % 3600 / 60;
    seconds = elapsed % 3600 % 60;
    SetDebugStringPosition(9, 4);
    DebugTextPrintf("%2d.%1d>%2d.%1d %2d:%02d:%02d", stepRangeEnd / 10000, stepRangeEnd % 10000, previousStoryStep / 10000, previousStoryStep % 10000, hours,
        minutes, seconds);
}

std::uint32_t GetStepElapsedFrames() noexcept {
    return stepElapsedFrames;
}

void SetSTEP(std::int32_t step) {
    const bool didNotMoveBackwards = g_storyStep <= step;
    g_storyStep = step;
    if (didNotMoveBackwards)
        return;

    const auto elapsed = static_cast<std::uint32_t>(30 * stepElapsedSeconds);
    stepRangeStart = step;
    stepRangeEnd = step;
    previousStoryStep = step;
    stepDebugFrames = 0;
    stepElapsedFrames = elapsed;
    stepRangeElapsedFrames = elapsed;
}

void ResetSTEP() {
    const auto elapsed = static_cast<std::uint32_t>(30 * stepElapsedSeconds);
    g_storyStep = 0;
    stepRangeStart = 0;
    stepRangeEnd = 0;
    previousStoryStep = 0;
    stepDebugFrames = 0;
    stepElapsedFrames = elapsed;
    stepRangeElapsedFrames = elapsed;
}

} // namespace shenmue::s2
