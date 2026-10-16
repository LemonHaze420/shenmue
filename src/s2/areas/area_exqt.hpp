#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name EXQT_AREA{SHEN_NAME(EXQT)}, EXQT_TASK{SHEN_NAME(EVNT)}, EXQT_MACHINE{SHEN_NAME(QTEM)};
inline constexpr shen_name EXQT_RYO{SHEN_NAME(RYO_)}, EXQT_COIN{SHEN_NAME(GCIN)}, EXQT_OMAKE{SHEN_NAME(OMAK)};
inline constexpr shen_name EXQT_INVALID{0xffffffffU};
struct ExciteGameState {
    std::uint32_t reserved00, flags, motion;
    std::int32_t score, best, continued, successes;
    float cumulativeBonus, timingBonus;
    std::uint32_t state, waitElapsed, waitTarget;
};
struct ExciteInput {
    std::uint32_t reserved00[3], required, pressed, reserved14, window, reserved1c, elapsed;
    std::int32_t attempts;
    std::uint32_t part;
};
struct ExciteState {
    ExciteGameState game;
    std::uint32_t reserved30, cycle;
    std::uint8_t lives;
    std::byte reserved39[7];
    float timeScale, patternScale;
    std::int32_t tier, retryFrames;
    std::int16_t patternStage;
    std::uint8_t patternByte, reserved53;
    std::int32_t immediateAdvance;
    union {
        struct {
            std::uint32_t reserved58;
            ExciteInput input;
        } excite;
        struct {
            ExciteInput input;
            std::uint32_t reserved84;
        } arcade;
    } controls;
};
struct ExciteTaskWork {
    std::uint32_t reserved00;
    ExciteState state;
    std::byte reserved8c[36];
};
struct ExcitePartWork {
    std::uint32_t part;
    std::int32_t enabled;
};
struct ExciteScoreWork {
    std::int32_t ticks, score;
    Vector3 position;
};
static_assert(sizeof(ExciteGameState) == 0x30 && sizeof(ExciteInput) == 0x2c && sizeof(ExciteState) == 0x88);
static_assert(offsetof(ExciteState, lives) == 0x38 && offsetof(ExciteState, controls) == 0x58);
static_assert(offsetof(ExciteState, controls.excite.input) == 0x5c && offsetof(ExciteState, controls.arcade.input) == 0x58);
static_assert(sizeof(ExciteTaskWork) == 0xb0 && sizeof(ExcitePartWork) == 8 && sizeof(ExciteScoreWork) == 0x14);
static_assert(offsetof(ExciteInput, required) == 0x0c && offsetof(ExciteInput, pressed) == 0x10);
static_assert(offsetof(ExciteInput, window) == 0x18 && offsetof(ExciteInput, elapsed) == 0x20 && offsetof(ExciteInput, part) == 0x28);
inline std::int32_t exciteCoinHandPose[57]{0, 4185, 63534, 0, 0, 62988, 0, 62624, 3822, 0, 182, 60258, 0, 0, 61168, 0, 0, 57710, 0, 546, 3822, 0, 0, 56618, 0,
    0, 52614, 0, 0, 56072, 0, 546, 1820, 0, 0, 54798, 0, 0, 52614, 0, 0, 58256, 0, 546, 909, 0, 0, 51704, 0, 0, 55708, 0, 0, 55890, 0, 546, 1092};
inline Vector3 exciteCoinPosition{.137f, .043f, -.015f};
inline Vector3i exciteCoinRotation{5461, 9284, 63351};
void InitializeExqt();
void RunExqt(std::uintptr_t);
void ExciteMain(void *);
void InitializeExcitePosition(ExciteState &);
void ResetExciteGame(ExciteState &);
void SetExciteRequiredInput(ExciteInput &, std::uint32_t);
bool PollExciteInput(ExciteInput &);
bool ConsumeExciteInput(ExciteInput &);
void BuildExciteInput(ExciteState &);
void SetExciteTiming(ExciteState &, ExciteInput &);
void InitializeExciteMachineParts();
void HideExciteMachineParts();
void ResetExciteMachineDisplay();
void SetExciteMachinePart(std::uint16_t, std::int32_t);
void UpdateExciteLives(const ExciteState &);
void ExcitePartCallback(void *);
void ExciteScoreCallback(void *);
void QueueExcitePart(std::uint32_t, std::int32_t, std::int32_t);
void QueueExciteStageDisplay(const ExciteState &);
void QueueExciteResultDisplay(const ExciteState &);
void DrawExciteScore(const ExciteState &, std::uint8_t);
void DrawExciteWindow(const ExciteState &);
void DrawExciteBonus(const ExciteState &);
void UpdateExciteDisplay(const ExciteState &);
void QueueExciteScoreFlash(const ExciteState &, bool);
void StartExciteIntro(bool);
void StartExcitePaidIntro(bool);
void StartExciteFreeIntro();
void ExciteLostLife(bool);
void ExciteHighScoreReaction();
void ExciteFinishedReaction(std::int32_t, std::int32_t);
extern const ScriptAreaDefinition exqtScriptDefinition;
} // namespace shenmue::s2::areas
