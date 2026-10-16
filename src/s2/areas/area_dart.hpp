#pragma once
#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include <array>
#include <bit>
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name DART_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name DART_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name DART_FCJ{SHEN_NAME(FCJ_)};
inline constexpr shen_name DART_HCY{SHEN_NAME(HCY_)};
inline constexpr shen_name DART_HLN{SHEN_NAME(HLN_)};
inline constexpr shen_name DART_JON{SHEN_NAME(JON_)};
inline constexpr shen_name DART_XHO{SHEN_NAME(XHO_)};
inline constexpr shen_name DART_AKS1{SHEN_NAME(AKS1)};
inline constexpr shen_name DART_COMPACT{SHEN_NAME(DAR3)};
inline constexpr shen_name DART_DUMMY0{SHEN_NAME(DUM0)};
inline constexpr shen_name DART_DUMMY1{SHEN_NAME(DUM1)};

inline std::int32_t dartsHandPoseCommand[19][3]{{0, 64445, 63535}, {0, 0, 62624}, {0, 63716, 2001}, {0, 64445, 55890}, {0, 0, 56983}, {0, 65172, 61350},
    {0, 0, 1456}, {0, 364, 56801}, {0, 64627, 57528}, {0, 0, 61532}, {0, 65354, 2001}, {0, 65354, 60987}, {0, 64445, 56983}, {0, 0, 63898}, {0, 65354, 2730},
    {0, 64809, 60075}, {0, 64809, 61168}, {0, 65172, 62442}, {0, 364, 4368}};
inline constexpr std::array<float, 5> DART_AI_BANDS{0.5f, 0.6f, 0.8f, 0.9f, 1.0f};
inline constexpr std::array<float, 6> DART_GAME_LARGE_SCORE_Y{
    2.1549999713897705f, 2.005000114440918f, 1.8550000190734863f, 1.7050000429153442f, 1.8550000190734863f, std::bit_cast<float>(std::uint32_t{1})};
inline constexpr std::array<float, 6> DART_GAME_SMALL_SCORE_Y{
    2.138000011444092f, 1.9880000352859497f, 1.8380000591278076f, 1.687999963760376f, 1.8380000591278076f, 0.0f};
inline constexpr std::array<const char *, 16> DART_FCJ_VOICES{"0137D044", "0137D045", "0137D046", nullptr, "0137D047", "0137D048", "0137D049", nullptr,
    "0137D050", "0137D051", "0137D046", nullptr, "0137D052", "0137D053", "0137D049", nullptr};
inline constexpr std::array<const char *, 6> DART_HCY_VOICES{"0137F053", nullptr, "0137F054", nullptr, "0137F047", nullptr};
inline constexpr std::array<const char *, 6> DART_HLN_VOICES{"0137C040", nullptr, "0137C041", nullptr, "0137C042", nullptr};
inline constexpr std::array<const char *, 6> DART_JON_VOICES{"0137B040", nullptr, "0137B041", nullptr, "0137B042", nullptr};
inline constexpr std::array<const char *, 16> DART_DEFAULT_VOICES{"0137E040", nullptr, nullptr, nullptr, "0137E041", "0137E042", nullptr, nullptr, "0137E043",
    "0137E044", "0137E045", nullptr, "0137E046", nullptr, nullptr, nullptr};
inline constexpr Vector3 DART_JON_ARROW_OFFSET{0.14000000059604645f, -0.05999999865889549f, 0.03999999910593033f};
inline constexpr Vector3 DART_FCJ_HLN_ARROW_OFFSET{0.10999999940395355f, -0.09000000357627869f, -0.0f};
inline constexpr Vector3 DART_DEFAULT_ARROW_OFFSET{0.14000000059604645f, -0.05999999865889549f, -0.019999999552965164f};
inline constexpr std::array<std::int32_t, 3> DART_ARROW_ROTATION{0x48df, 0x7a9f, 0xc111};
// Throwing-hand offsets by opponent row; the failed-name row reads the adjacent dartInfos bytes.
inline constexpr std::array<Vector3, 6> DART_OPPONENT_ARROW_OFFSETS{{{0.11999999731779099f, -0.09000000357627869f, -0.009999999776482582f},
    {0.14000000059604645f, -0.05999999865889549f, -0.019999999552965164f}, {0.10499999672174454f, -0.07000000029802322f, -0.009999999776482582f},
    {0.14000000059604645f, -0.05999999865889549f, -0.019999999552965164f}, {0.14000000059604645f, -0.05999999865889549f, -0.019999999552965164f},
    {0.0f, std::bit_cast<float>(0x5f4a4346U), 1.4500000476837158f}}};
struct DartsArrowWork {
    shen_name character, arrow;
};
static_assert(sizeof(DartsArrowWork) == 8);
extern shen_name dartsTemporaryCharacter;
HLib::Task *QueueDartsGameArrow(shen_name character, shen_name arrow);
void DartsGameArrowCallback(void *taskData);

inline constexpr std::array<std::array<std::uint32_t, 32>, 6> DART_OPPONENT_WORDS{
    {{SHEN_NAME(FCJ_), 1069128090, 14, 6786, 6785, 199, 221, 6786, 30, 6790, 6787, 6846, 199, 6789, 6788, 6825, 6826, 390, 470, 1116471296, 1109393408,
          1067030938, 1082549862, 6827, 430, 1116471296, 1067869798, 1082549862, 6824, 0, 0, 0},
        {SHEN_NAME(HCY_), 1069883064, 14, 6798, 6797, 245, 245, 6798, 60, 6802, 6799, 6847, 135, 6801, 6800, 6829, 6830, 300, 250, 0, 0, 1067030938, 1083808154,
             6831, 340, 0, 1067869798, 1083598438, 6828, 0, 0, 0},
        {SHEN_NAME(HLN_), 1067450368, 14, 6841, 6840, 279, 242, 6841, 50, 6845, 6842, 6850, 157, 6844, 6843, 6822, 6822, 200, 200, 0, 0, 1063675494,
                 1082759578, 6823, 250, 0, 1063675494, 1082759578, 6842, 0, 0, 0},
        {SHEN_NAME(JON_), 1069715292, 21, 6810, 6809, 145, 215, 6810, 70, 6814, 6811, 6848, 144, 6813, 6812, 6833, 6834, 245, 240, 0, 0, 1067869798,
                     1083388723, 6835, 470, 1120403456, 1065353216, 1082969293, 6832, 0, 0, 0},
        {SHEN_NAME(XHO_), 1069799178, 14, 6816, 6815, 180, 150, 6816, 60, 6820, 6817, 6849, 165, 6819, 6818, 6837, 6838, 300, 245, 0, 0, 1067869798,
                         1083388723, 6839, 380, 0, 1068708659, 1083179008, 6836, 0, 0, 0},
                        {0, 1069547520, 14, 6816, 6815, 180, 150, 6816, 60, 6820, 6817, 6849, 165, 6819, 6818, 6837, 6838, 300, 245, 0, 0, 1065353216,
                            1082969293, 6839, 380, 0, 1065353216, 1082969293, 6836, 0, 0, 0}}};
std::int32_t DartsOpponentAttachBone(shen_name opponent, std::int32_t difficulty);
std::uint32_t DartsBeginOpponentThrow(shen_name opponent, std::int32_t difficulty);
struct DartsGameEntry {
    std::uint8_t mode;
    std::int8_t placementIndex, opponentMode, maximumRadius;
    std::int32_t minimumRadius;
    std::int32_t bet, reserved0c;
    std::array<std::int32_t, 4> savedScores;
    HLib::Task *parent;
    const EventDartsPlacement *placements;
    const EventDartsEnemy *enemy;
};
struct DartsScoreBlock {
    std::int32_t impactIterations, score, finalScore, accumulated;
};
struct DartsEventWork {
    float cursorX, cursorY;
    std::uint8_t field08, field09, standardBoard;
    std::int8_t state;
    std::int8_t scoreStep;
    std::uint8_t scoreUiFlags, field0e, pendingReset, field10;
    std::array<std::uint8_t, 5> usedBands;
    std::int8_t enemyTurn, playerWon;
    std::array<std::uint8_t, 4> actorControl;
    float targetBand;
    std::array<char, 16> previousBgm, previousSbt;
    Vector3 ryoRotation;
    Vector3i previousRotation;
    Vector3 dartPosition, impactPosition, previousPosition;
    std::uint32_t reserved7c;
    std::uintptr_t previousSoundProgram;
    std::int32_t difficulty, gameFlag;
    std::int32_t scoreAnimationActive, requestIcon, pendingScore, reserved9c;
    std::int32_t *scoreTarget;
    std::int32_t scoreTickInterval;
    std::array<std::int32_t, 4> scoreTick;
    std::uint32_t wobblePhaseX, wobblePhaseY;
    std::int32_t bestScore, previousScore, scoreTicker, throws;
    DartsScoreBlock opponentScore;
    std::int32_t opponentBullseyes, fieldE8, fieldEc, fieldF0, fieldF4, fieldF8, fieldFc, field100, field104;
    std::int32_t bullseyes, alternateBestScore, alternatePreviousScore, field114, field118;
    DartsScoreBlock playerScore;
    std::int32_t playerBullseyes;
    std::uint32_t gameState, nextState, frames, audioBank;
    HLib::Task *task;
    std::array<HLib::Task *, 3> childTasks;
    HLib::Task *voiceTask;
    DartsGameEntry *entry;
    DateTime previousTime;
    std::array<std::byte, 5> reserved177;
    std::uint32_t resetFrame;
};
struct DartsChildWork {
    HLib::Task *rootTask;
    DartsEventWork *root;
    std::int32_t state, reserved14;
};
static_assert(sizeof(DartsChildWork) == 0x18 && offsetof(DartsChildWork, root) == 8);
enum class DartsChildTask : std::uint32_t { Display, Frames, VoiceMonitor, OpponentVoice };
static_assert(sizeof(DartsGameEntry) == 0x38 && offsetof(DartsGameEntry, placements) == 0x28);
static_assert(sizeof(DartsScoreBlock) == 0x10);
static_assert(sizeof(DartsEventWork) == 0x180);
static_assert(offsetof(DartsEventWork, usedBands) == 0x11 && offsetof(DartsEventWork, targetBand) == 0x1c);
static_assert(offsetof(DartsEventWork, previousSoundProgram) == 0x80 && offsetof(DartsEventWork, wobblePhaseX) == 0xbc);
static_assert(offsetof(DartsEventWork, opponentScore) == 0xd4 && offsetof(DartsEventWork, playerScore) == 0x11c);
static_assert(offsetof(DartsEventWork, task) == 0x140 && offsetof(DartsEventWork, voiceTask) == 0x160);
static_assert(offsetof(DartsEventWork, entry) == 0x168 && offsetof(DartsEventWork, previousTime) == 0x170);
std::uint32_t DartsSelectAiTargetBand(DartsEventWork *work);
std::int32_t DartsComputeAiImpactPoint(DartsEventWork *work);
shen_name DartsSelectBoard(DartsEventWork *work, shen_name actor);
void DartsLoadAudioBank(DartsEventWork *work, std::int32_t bankMode);
std::uint32_t DartsResetScoring(DartsEventWork *work);
std::uint8_t DartsSwitchScoringTurn(DartsEventWork *work);
bool DartsAdvanceScoreTransfer(DartsEventWork *work, std::int32_t *score, float soundContext);
void DartsUpdateMachineScore(shen_name board, std::int32_t score, std::int32_t row);
void DartsUpdateCompactMachineScore(shen_name board, std::uint32_t score, std::int32_t row);
void DartsDisplayCallback(void *taskData);
void DartsIncrementFramesCallback(void *taskData);
void DartsMonitorOpponentVoiceCallback(void *taskData);
void DartsOpponentVoiceCallback(void *taskData);
void DartsQueueOpponentVoice(shen_name opponent);
void DartsStartChildTask(DartsEventWork *work, DartsChildTask kind);
void DartsInitializeScoreBanks(DartsEventWork *work);
void DartsPlaceCameraDummies(DartsEventWork *work);
extern const ScriptAreaDefinition dartScriptDefinition;
} // namespace shenmue::s2::areas
