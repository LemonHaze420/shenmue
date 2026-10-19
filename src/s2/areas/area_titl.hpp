#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/qte_game_entry.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name TITL_RYO{SHEN_NAME(RYO_)}, TITL_COIN{SHEN_NAME(GCIN)}, TITL_OMAK{SHEN_NAME(OMAK)};
inline constexpr shen_name TITL_MITTS[]{SHEN_NAME(MIT1), SHEN_NAME(MIT2), SHEN_NAME(MIT3)};
inline constexpr shen_name TITL_LIVES[]{SHEN_NAME(QTL1), SHEN_NAME(QTL2), SHEN_NAME(QTL3)};
enum TitleState : std::uint16_t {
    TitleWaitFadeClear = 0,
    TitleLoadIntro = 1,
    TitleAskPlay = 2,
    TitleWaitChoice = 3,
    TitleShowChoice = 4,
    TitleTakeMoney = 5,
    TitlePrepareHands = 6,
    TitlePostGame = 7,
    TitleExit = 8,
    TitleEnterGame = 9,
    TitleGameBoot = 10,
    TitleStartCamera = 11,
    TitleSetupRound = 12,
    TitlePrepareInput = 13,
    TitleInput = 14,
    TitleScoreInput = 15,
    TitleLostRound = 16,
    TitleNextRound = 17,
    TitleLowerMitts = 18,
    TitleRoundFinished = 19,
    TitleResults = 20,
    TitleNoMoney = 21
};
enum class TitleMittMode : std::uint16_t { Up, Down, Fake, Knockout, Sleep };
struct TitleMittWork {
    shen_name character;
    std::uint32_t reserved04;
    HLib::Task *task;
    std::uint32_t initialAngle;
    std::uint16_t reserved14, duration, frame, reserved1a;
    std::uint32_t raised, phase, reserved24;
};
struct TitleWork {
    std::uint32_t flags, score, bestScore, previousScore;
    float timingBonus[3];
    TitleState state;
    std::uint16_t reserved1e, frame, waitFrames;
    std::uint32_t reserved24;
    TitleMittWork *mitts[3];
    std::byte reserved40[12];
    std::uint32_t buttons, reserved50, inputWindow;
    std::byte reserved58[4];
    std::uint32_t inputFrames;
    std::int32_t firstComboFrame;
    std::uint32_t reserved64[2];
    std::uint32_t requireB, requireA, requireX;
    std::uint32_t indicators[3], reserved84;
    std::uint32_t *scoreSources[3];
    std::uint16_t pattern[4];
    std::int32_t timingFrames;
    float simplePatterns, doublePatterns, triplePatterns;
    std::uint32_t successes, tripleEnabled, simultaneousEnabled, fakeEnabled, replayAudio, skipEffect;
    float advancedPatternDivisor;
    std::uint32_t stepScore;
    std::uint16_t extraTiming, reservedda;
    std::int32_t step;
    std::uint32_t completedSteps;
    std::uint16_t lives, reservede6, reservede8;
    std::byte reservedea[26];
    std::uint32_t allowCancel;
};
struct TitleScoreFlashWork {
    TitleWork *title;
    std::uint32_t lanes[3], draw;
};
static_assert(sizeof(TitleMittWork) == 0x28 && offsetof(TitleMittWork, duration) == 0x16 && offsetof(TitleMittWork, phase) == 0x20);

static_assert(sizeof(TitleWork) == 0x108 && offsetof(TitleWork, state) == 0x1c && offsetof(TitleWork, mitts) == 0x28);
static_assert(offsetof(TitleWork, buttons) == 0x4c && offsetof(TitleWork, indicators) == 0x78 && offsetof(TitleWork, scoreSources) == 0x88);
static_assert(offsetof(TitleWork, pattern) == 0xa0 && offsetof(TitleWork, successes) == 0xb8 && offsetof(TitleWork, extraTiming) == 0xd8);
static_assert(offsetof(TitleWork, step) == 0xdc && offsetof(TitleWork, lives) == 0xe4 && offsetof(TitleWork, allowCancel) == 0x104);
static_assert(sizeof(TitleScoreFlashWork) == 0x18);
inline std::int32_t titleCoinHandCommand[19][3]{{0, 4185, 63535}, {0, 0, 62989}, {0, 62624, 3821}, {0, 182, 60258}, {0, 0, 61168}, {0, 0, 57710},
    {0, 546, 3821}, {0, 0, 56619}, {0, 0, 52615}, {0, 0, 56072}, {0, 546, 1820}, {0, 0, 54798}, {0, 0, 52615}, {0, 0, 58257}, {0, 546, 908}, {0, 0, 51705},
    {0, 0, 55709}, {0, 0, 55890}, {0, 546, 1091}};
inline std::int32_t titleFistHandCommand[19][3]{{0, 0, -10922}, {0, 0, -10558}, {0, 1456, 0}, {0, 0, -18022}, {0, 0, -17112}, {0, 0, -18386}, {0, 0, 0},
    {0, 0, -12196}, {0, 0, -18204}, {0, 0, -16748}, {0, 0, 0}, {0, 0, -11832}, {0, 0, -18386}, {0, 0, -15109}, {0, 0, 0}, {0, 910, -12014}, {0, -2184, -16748},
    {0, -2184, -10012}, {0, 0, 0}};
inline constexpr Vector3 TITL_INDICATOR_POSITIONS[]{{0, 1.3899999856948853f, 0.715499997138977f},
    {0.2750000059604645f, 1.3899999856948853f, 0.715499997138977f}, {-0.2750000059604645f, 1.3899999856948853f, 0.715499997138977f}};
inline constexpr Vector3 TITL_INDICATOR_SCALE{.4f, .4f, .4f};
void InitializeTitle();
void RunTitle(std::uintptr_t argument);
void TitleMainCallback(void *data);
void TitleGameCallback(void *data);
void TitleRewardCallback(void *data);
void TitleOmakeExitCallback(void *data);
void TitleGameLocalNullsub();
void InitializeTitleSeed(TitleWork *work);
void BuildTitlePattern(TitleWork *work);
void AdvanceTitleDifficulty(TitleWork *work);
bool AdvanceTitlePatternStep(TitleWork *work);
bool PollTitleButtons(TitleWork *work);
bool ConsumeTitleButtons(TitleWork *work);
void CalculateTitleTimingBonus(TitleWork *work);
void ConfigureTitleMitts(TitleWork *work);
void ApplyTitleMittBehavior(TitleMittWork *mitt, TitleMittMode mode, std::uint16_t duration, std::int32_t delay);
void TitleMittIdle(void *data);
void TitleMittUp(void *data);
void TitleMittDown(void *data);
void TitleMittFake(void *data);
void TitleMittKnockout(void *data);
void TitleMittSleep(void *data);
void UpdateTitleScoreDisplay(TitleWork *work);
void TitleScoreFlashCallback(void *data);
void QueueTitleScoreFlashes(TitleWork *work, std::uint8_t lanes);
void ShowTitleLives(TitleWork *work);
void PlayTitleStepMotion(TitleWork *work, bool success);
void SetTitleLaneCamera(std::uint16_t lane);
void PrepareTitlePaidIntro();
void PrepareTitleHands(TitleWork *work);
void FinishTitleMotion(TitleWork *work);
void ExitTitle(TitleWork *work);
extern const ScriptAreaDefinition titlScriptDefinition;
} // namespace shenmue::s2::areas
