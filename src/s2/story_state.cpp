#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "shenmue.h"

#include "s2/story_state.hpp"
#include "s2/player_stats_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/debug_screen_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/scenario_flags_dc.hpp"
#endif
#include "s2/face_controller_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/pak_dc.hpp"
#include <cstring>
#else
#include "s2/story_state.hpp"
#include "s2/date_time.hpp"
#include "s2/debug_text.hpp"
#include "s2/flag.hpp"
#include "s2/player.hpp"

#include <algorithm>
#include <array>

#include <cstdint>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const char facialModelCacheEmptyName[5] = "NULL";
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t g_storyStep = 0;

namespace {



std::int32_t previousStoryStep = 0;

std::int32_t stepRangeStart = 0;

std::int32_t stepRangeEnd = 0;

std::uint32_t stepElapsedFrames = 0;

std::uint64_t stepRangeElapsedFrames = 0;

std::int32_t stepElapsedSeconds = 0;

std::uint32_t stepDebugFrames = 0;

// Native STEPDEBUG reads this range-display switch; PC v1.07 never sets it.
std::uint8_t stepRangeDebugDisplay;


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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateStepDebugCounters() noexcept;
void SetSTEP(std::int32_t step);
void ResetSTEP();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t &ScenarioEventStateWord(std::uint16_t objectId) {
    if (objectId == 5)
        return scenarioGameplayFlags.words[1];
    if (objectId == 6)
        return scenarioGameplayFlags.words[2];
    return scenarioGameplayFlags.words[0];
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t g_storyStep __attribute__((section(".data.dc_story_step"))) = 0;

StepDebugTiming g_stepDebugTiming = {};
StepDebugRange g_stepDebugRange = {};

FacialModelCacheEntry facialModelCache[facialModelCacheCount] = {};

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t CompareCharacterTableTokens(const char *left, const char *right);
std::uint32_t FindCachedFacialModelIndex(const char *name);
std::int32_t RetainCachedFacialModelNullsub(const MT7Model *);
void FacialModelCacheReleaseNullsub(std::uint8_t, std::int32_t);
void ReleaseCachedFacialModel(const char *name, std::uint8_t index);
void ResetCachedFacialModels();
void ResetStepTiming(std::int32_t step);
void ResetSTEP();
void SetSTEP(std::int32_t step);
std::uint32_t GetCurrentStep();
void DisplayStepInfo();
void UpdateStepDebugCounters();
#else
namespace {

std::uint32_t &ScenarioEventStateWord(std::uint16_t objectId);
bool ScenarioEventStateField(std::uint8_t mode, std::uint8_t &shift, std::uint8_t &mask, std::int32_t &assignmentMaximum);

} // namespace

void ResetSTEP();
void SetSTEP(std::int32_t step);
std::uint32_t GetCurrentStep();
std::int32_t GetCurrentCash();
void SetCurrentCash(std::int32_t cash);
void UpdateGameplayFlags();
void CopyShenmueFlagsTo(std::span<std::byte, 0x34> output) noexcept;
void SetShenmueFlagsTo(std::span<const std::byte, 0x34> input) noexcept;
bool ResolveScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t &value);
bool AssignScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t value);
void UpdateStepDebugCounters() noexcept;
std::uint32_t GetStepElapsedFrames() noexcept;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FacialModelCacheReleaseNullsub(std::uint8_t, std::int32_t) {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t RetainCachedFacialModelNullsub(const MT7Model *) {
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t FindCachedFacialModelIndex(const char *name) {
    std::uint8_t empty = facialModelCacheCount;
    std::uint8_t index = 0;
    for (; index < facialModelCacheCount; ++index) {
        if (!CompareCharacterTableTokens(facialModelCache[index].name, name))
            break;
        if (!CompareCharacterTableTokens(facialModelCache[index].name, facialModelCacheEmptyName) && empty >= index)
            empty = index;
    }
    if (index >= facialModelCacheCount)
        index = empty;
    return index;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReleaseCachedFacialModel(const char *name, std::uint8_t index) {
    if (!CompareCharacterTableTokens(facialModelCache[index].name, facialModelCachePersistentName) ||
        CompareCharacterTableTokens(facialModelCache[index].name, name))
        return;
    if (--facialModelCache[index].references > 0)
        return;
    std::memcpy(facialModelCache[index].name, facialModelCacheEmptyName, sizeof(facialModelCacheEmptyName));
    facialModelCache[index].references = 0;
    std::int32_t released = 0;
    if (!FindRetainingModelPackage(facialModelCache[index].model) && !RetainCachedFacialModelNullsub(facialModelCache[index].model)) {
        ReleaseMT7Model(facialModelCache[index].model);
        released = 1;
    }
    facialModelCache[index].root = 0;
    facialModelCache[index].model = 0;
    FacialModelCacheReleaseNullsub(index, released);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t UnloadCachedFacialModel(const char *name, std::uint8_t index) {
    ReleaseCachedFacialModel(name, index);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetCachedFacialModels() {
    for (std::uint8_t index = 0; index < facialModelCacheCount; ++index) {
        if (facialModelCache[index].root && facialModelCache[index].name)
            ReleaseCachedFacialModel(facialModelCache[index].name, index);
        std::memcpy(facialModelCache[index].name, facialModelCacheEmptyName, sizeof(facialModelCacheEmptyName));
        facialModelCache[index].references = 0;
        facialModelCache[index].root = 0;
        facialModelCache[index].model = 0;
    }
}

const shen_name MEMORY_FACIAL_MODEL = SHEN_NAME4('F', 'A', 'C', 'E');

std::uint8_t LoadCachedFacialModel(const char *directory, const char *filename, std::uint8_t index) {
    std::int32_t result = 0;
    if (filename) {
        result = 1;
        if (!facialModelCache[index].references) {
            result = 2;
            MT7Model *model = reinterpret_cast<MT7Model *(*)(const char *)>(FindPreloadedCharacterImageNullsub)(filename);
            if (!model && GetPakLoadFlags())
                model = LoadGameModel(GetPakLoadTaskDataPointer(), filename);
            if (!model)
                facialModelCache[index].model = InitializeMT7Model(shenLoadFile(directory, filename, MEMORY_FACIAL_MODEL));
            else
                facialModelCache[index].model = model;
            facialModelCache[index].root = facialModelCache[index].model->root;
        }
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ResetStepTiming(std::int32_t step) {
    g_stepDebugRange.previousStoryStep = step;
    g_stepDebugRange.stepRangeStart = step;
    g_stepDebugRange.stepRangeEnd = step;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::int32_t slot = GetStepCounterSlot(step);
#else
    const std::int32_t slot = GetStepCounterSlot();
#endif
    const std::int32_t *const elapsed = &g_combatStatistics.stepElapsedSeconds;
    g_stepDebugTiming.stepElapsedFrames = static_cast<std::uint32_t>(elapsed[slot]) * 30U;
    g_stepDebugTiming.stepDebugFrames = 0;
    g_stepDebugTiming.stepElapsedSeconds = static_cast<std::int32_t>(static_cast<std::uint32_t>(elapsed[slot]) * 30U);
    g_stepDebugTiming.stepRangeElapsedFrames = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetSTEP() {
    g_storyStep = 0;
    ResetStepTiming(0);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace {
extern std::uint8_t stepRangeDebugDisplay;
} // namespace

std::uint8_t ToggleStepRangeDebugDisplay(std::int32_t query) {
    if (!query)
        stepRangeDebugDisplay = (stepRangeDebugDisplay + 1) & 1;
    return stepRangeDebugDisplay;
}
#endif

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetSTEP(std::int32_t step) {
    if (g_storyStep > step)
        ResetStepTiming(step);
    g_storyStep = step;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetCurrentStep() {
    return static_cast<std::uint32_t>(g_storyStep);
}

#else

std::uint32_t GetCurrentStep() {
    return static_cast<std::uint32_t>(g_storyStep);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t GetCurrentCash() {
    return currentCash;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetCurrentCash(std::int32_t cash) {
    currentCash = cash;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void UpdateGameplayFlags() {
    if (!scenarioGameplayFlags.initialized) {
        scenarioGameplayFlags.words.fill(1);
        scenarioGameplayFlags.initialized = 1;
    }
    DateTime now{};
    shenGetCurrentDateTime(now);
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void CopyShenmueFlagsTo(std::span<std::byte, 0x34> output) noexcept {
    std::memcpy(output.data(), &scenarioGameplayFlags, sizeof(scenarioGameplayFlags));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetShenmueFlagsTo(std::span<const std::byte, 0x34> input) noexcept {
    std::memcpy(&scenarioGameplayFlags, input.data(), sizeof(scenarioGameplayFlags));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DisplayStepInfo() {
    SetDebugScreenTextPosition(13, 2);
    const std::int32_t step = g_storyStep;
    shenmue_s2_event_DebugLogToScreen("STEP %2d.%04d", static_cast<int>(step / 10000), static_cast<int>(step % 10000));
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t stepRangeDebugDisplay;
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetStepCounterSlot(std::int32_t) {
#else
std::int32_t GetStepCounterSlot() {
#endif
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateStepDebugCounters() {
    const std::int32_t previous = g_storyStep;
    ++g_stepDebugTiming.stepElapsedFrames;
    ++g_stepDebugTiming.stepDebugFrames;
    const std::int32_t slot = GetStepCounterSlot(previous);
    std::int32_t *const elapsed = &g_combatStatistics.stepElapsedSeconds;
    const std::int32_t seconds = static_cast<std::int32_t>(g_stepDebugTiming.stepElapsedFrames) / 30;
    elapsed[slot] = seconds;
    if (previous != g_storyStep) {
        g_stepDebugTiming.stepElapsedSeconds = seconds;
        g_stepDebugTiming.stepElapsedFrames = static_cast<std::uint32_t>(elapsed[GetStepCounterSlot(g_storyStep)]) * 30U;
        g_stepDebugRange.previousStoryStep = previous;
        const std::int32_t current = g_storyStep;
        if (current / 10000 > previous / 10000) {
            g_stepDebugTiming.stepRangeElapsedFrames = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_stepDebugTiming.stepDebugFrames) / 30);
            g_stepDebugTiming.stepDebugFrames = 0;
            g_stepDebugRange.stepRangeStart = previous;
            g_stepDebugRange.stepRangeEnd = current;
            AwardStepMoveTraining();
        }
        SetScenarioTransientState(4, 0);
        ResetPlayerSaveSelections();
    }
    if (!g_developerDisplayEnabled || DebugMenuInputBlockedNullsub())
        return;
    DisplayStepInfo();
    if (!stepRangeDebugDisplay)
        return;
    const char *const format = "%2d.%1d>%2d.%1d %2d:%02d:%02d";
    std::int32_t stepSeconds = g_stepDebugTiming.stepElapsedSeconds;
    std::int32_t hours = stepSeconds / 3600;
    stepSeconds -= hours * 3600;
    std::int32_t minutes = stepSeconds / 60;
    std::int32_t secondsWithinMinute = stepSeconds - minutes * 60;
    SetDebugScreenTextPosition(9, 3);
    std::int32_t rangeStart = g_stepDebugRange.previousStoryStep;
    std::int32_t rangeStartChapter = rangeStart / 10000;
    rangeStart -= rangeStartChapter * 10000;
    std::int32_t rangeEnd = g_storyStep;
    std::int32_t rangeEndChapter = rangeEnd / 10000;
    rangeEnd -= rangeEndChapter * 10000;
    shenmue_s2_event_DebugLogToScreen(format, rangeStartChapter, rangeStart, rangeEndChapter, rangeEnd, hours, minutes, secondsWithinMinute);
    stepSeconds = static_cast<std::int32_t>(g_stepDebugTiming.stepRangeElapsedFrames);
    hours = stepSeconds / 3600;
    stepSeconds -= hours * 3600;
    minutes = stepSeconds / 60;
    secondsWithinMinute = stepSeconds - minutes * 60;
    SetDebugScreenTextPosition(9, 4);
    rangeStart = g_stepDebugRange.stepRangeStart;
    rangeStartChapter = rangeStart / 10000;
    rangeStart -= rangeStartChapter * 10000;
    rangeEnd = g_stepDebugRange.stepRangeEnd;
    rangeEndChapter = rangeEnd / 10000;
    rangeEnd -= rangeEndChapter * 10000;
    shenmue_s2_event_DebugLogToScreen(format, rangeStartChapter, rangeStart, rangeEndChapter, rangeEnd, hours, minutes, secondsWithinMinute);
}
#endif

#else

// Native STEPDEBUG, the UPDB callback body and the tail of its constructor.
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t GetStepElapsedFrames() noexcept {
    return stepElapsedFrames;
}

#endif

} // namespace s2
} // namespace shenmue
