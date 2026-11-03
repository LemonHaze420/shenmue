#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
 
#include "s2/areas/common/et_darts.h"
#include "s2/event_debug_dc.h"

int CalculateDartsPositionOrScore(char score, Vector3 *position);
void SetDartsTemporaryCharacter(shen_name character);
void SetDartsPosition(shen_name parent, shen_name character, float x, float y, float z, int yaw, int delay);
void UpdateDartsScoreDisplay(shen_name board, int score, int row);
void UpdateDartsCompactScoreDisplay(shen_name board, int score, int row);
void QueueDartsPosition(shen_name parent, shen_name character, Vector3 *position, Vector3i *rotation, int delay);
HLibTask *QueueDartsArrow(shen_name character, shen_name arrow);
void RestoreOpponentDartsScores(shen_name character, int save, int *bestScore, int *opponentScore);
shen_name GetDartsArrowName(int set, int index);

#define DART_TASK TASK_EVNT
#define DART_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define DART_FCJ SHEN_NAME4('F', 'C', 'J', '_')
#define DART_HCY SHEN_NAME4('H', 'C', 'Y', '_')
#define DART_HLN SHEN_NAME4('H', 'L', 'N', '_')
#define DART_JON SHEN_NAME4('J', 'O', 'N', '_')
#define DART_XHO SHEN_NAME4('X', 'H', 'O', '_')
#define DART_AKS0 SHEN_NAME4('A', 'K', 'S', '0')
#define DART_AKS1 SHEN_NAME4('A', 'K', 'S', '1')
#define DART_COIN SHEN_NAME4('C', 'o', 'i', 'n')
#define DART_DUMMY0 SHEN_NAME4('D', 'U', 'M', '0')
#define DART_DUMMY1 SHEN_NAME4('D', 'U', 'M', '1')
#define DART_REWARD_AREA SHEN_NAME4('O', 'M', 'A', 'K')
#define DART_DEBUG_ID 3
#define DART_AIM_X (-0.119f)
#define DART_AIM_Y 1.5891f
#define DART_FSQRT(x) ((x) = __builtin_sqrtf(x))
#define DART_FSQRT_OF(out, in) ((out) = __builtin_sqrtf(in))

typedef struct DartsOpponentInfo {
    shen_name name;
    float height;
    int bone;
    int motions[16];
    float loseFrame[2];
    float loseX, loseZ;
    int winMotion, winTotal;
    float winFrame, winX, winZ;
    int drawMotion;
    HLibTaskCallback throwCallback;
} DartsOpponentInfo;
typedef struct DartsVoiceWaitWork {
    int frames;
    void *conversation;
    HLibTask *parent;
} DartsVoiceWaitWork;
typedef struct DartsCameraWaitWork {
    struct DartsEventWork *root;
    int camera;
    shen_name eye, target;
} DartsCameraWaitWork;
 
typedef struct DartsScoreBank {
    int bestScore, previousScore, ticker, throws;
    int score[4];
    int bullseyes;
} DartsScoreBank;
typedef struct DartsAdmissionScoreWork {
    int soundStarted;
    shen_name board;
    int scores[4];
    int remaining;
} DartsAdmissionScoreWork;
typedef struct DartsProjectileWork {
    Vector3 step, target;
    unsigned int arcPhase;
    int arcStep;
    Vector3i targetRotation;
    int frames;
    shen_name board, arrow;
} DartsProjectileWork;
typedef struct DartsChildTaskWork {
    HLibTask *root;
    struct DartsEventWork *work;
    int reserved;
} DartsChildTaskWork;
typedef struct DartsEventWork {
    char nextGameText[8];
    unsigned char field08, field09, standardBoard;
    signed char state, scoreStep;
    unsigned char scoreUiFlags, field0e;
    signed char pendingReset;
    unsigned char field10;
    unsigned char usedBands[5];
    signed char enemyTurn, playerWon;
    signed char actorControl[4];
    float targetBand;
    char previousBgm[16], previousSbt[16];
    Vector3 ryoRotation;
    Vector3i previousRotation;
    Vector3 dartPosition, impactPosition, previousPosition;
    int previousSoundProgram;
    int difficulty, gameFlag;
    int scoreAnimationActive, requestIcon;
    int pendingScore;
    int *scoreTarget;
    int scoreTickInterval;
    int scoreTick[4];
    int wobblePhaseX, wobblePhaseY;
    DartsScoreBank banks[3];
    int gameState, nextState, frames, audioBank;
    HLibTask *task;
    HLibTask *childTasks[4];
    EventDartsGame *entry;
    EventDateTime previousTime;
} DartsEventWork;

void DartsAreaMain(EventDartsGame *entry);
void DartsInitializeRoot(DartsEventWork *work, HLibTask *task, EventDartsGame *entry);
void DartsDebugExitCallback(void *data);
void DartsExit(void *data);
void DartsRestorePlayer(DartsEventWork *work);
void DartsRestoreOpponent(DartsEventWork *work);
void DartsRetireCoinAndArrows(DartsEventWork *work);
void DartsPlaceCameraDummies(DartsEventWork *work);
void DartsInitializeScoreBanks(DartsEventWork *work);
void DartsInitStart(void *data);
void DartsPositionOpponentHands(void *data);
void DartsInitStart2(void *data);
void DartsInitializeGame(DartsEventWork *work);
void DartsEnemyShot(void *data);
void DartsMain(void *data);
void DartsResetScoring(DartsEventWork *work);
void DartsResetRound(DartsEventWork *work);
void DartsStartChildTask(DartsEventWork *work, int kind);
void DartsScoreDisplayTask(void *data);
void DartsFrameCounterTask(void *data);
void DartsScoreTickerTask(void *data);
void DartsOpponentChatterTask(void *data);
void DartsPlayOpponentChatter(shen_name opponent);
extern const char *dartsFcjChatter[4][4];
extern const char *dartsHcyChatter[3][2];
extern const char *dartsHlnChatter[3][2];
extern const char *dartsJonChatter[3][2];
extern const char *dartsXhoChatter[4][4];
void DartsOpponentAdmission(void *data);
int DartsAdmissionIntroduction(DartsEventWork *work);
int DartsAdmissionDemonstration(DartsEventWork *work);
HLibTask *DartsQueueAdmissionScore(shen_name board, int best, int previous, int current, int amount, int delay);
void DartsAdmissionScoreTask(void *data);
int DartsAdmissionOffer(DartsEventWork *work);
int DartsAcceptBet(DartsEventWork *work);
int DartsDeclineBet(DartsEventWork *work);
int DartsCheckBet(DartsEventWork *work);
void PrepareDartsOpponentAdmission(shen_name opponent);
int PlayDartsOpponentAdmission(shen_name opponent);
int PlayDartsBetResponse(shen_name opponent, int difficulty, int bet);
int PlayDartsOpponentResult(shen_name opponent, int bet);
extern Vector3i dartsHandRotation;
extern int dartsHandPoseCommand[19][3];
void DartsEnemyPlaying(void *data);
void DartsPlaying(void *data);
void DartsAttachTurnArrows(DartsEventWork *work);
void DartsAskPlay(void *data);
void DartsMainOk(void *data);
void DartsInsertCoin(void *data);
extern Vector3 dartsReadyArrowOffsets[5];
int DartsPlaySample(unsigned int sample);
void DartsNextGameGlyph(char code, int index, shen_name board);
void DartsShotEndRecord(void *data);
int DartsRecordBest(DartsEventWork *work);
void DartsPlayEnd(void *data);
void DartsExitBattle(void *data);
void DartsContinuePrompt(void *data);
void DartsContinueOk(void *data);
void DartsDoubleUp(void *data);
int PlayDartsOpponentExit(shen_name opponent);
void DartsSaveScores(DartsEventWork *work);
void DartsNextGame(void *data);
int DartsScoreImpact(DartsEventWork *work);
void DartsShotEnd(void *data);
void DartsShotEndBlink(void *data);
void DartsRyoShot(void *data);
int DartsComputeAiImpactPoint(DartsEventWork *work);
float DartsVerticalWobble(int phase);
void DartsQueueProjectile(shen_name board, shen_name arrow, int score, Vector3 *impact, float duration, int arc);
void DartsProjectileTask(void *data);
typedef void *(*DartsResponseCallback)(int opponentMatch, int playerTurn, shen_name opponent);
extern const DartsResponseCallback dartsResponseCallbacks[30];
void *PlayDartsOpeningResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsReadyResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsExitResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsContinueResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsLastThrowResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsZeroScoreResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsLowScoreResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsLowerMiddleScoreResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsMiddleScoreResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsUpperMiddleScoreResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsUpperScoreResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsBullseyeResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsHighScoreResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsEndThrowsResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsRoundResultResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsBonusResultResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsFinalResultResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsGameOverResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsReplayResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsStopResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsDeclineOpponentResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsWonBetResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsOpponentPaymentResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsLostBetResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsOpponentReplayResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsOpponentBonusReplayResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsOpponentFinalResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsThrowStartResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsThrowResponse(int mode, int playerTurn, shen_name opponent);
void *PlayDartsRoundOverResponse(int mode, int playerTurn, shen_name opponent);
void DartsGameMain(void *data);
extern Vector3i dartsGameArrowRotation;
extern Vector3 dartsJonGameArrowOffset;
extern Vector3 dartsGameArrowOffsets[5];
int DartsAiTriggerReady(DartsEventWork *work);
void DartsBeginScoreTransfer(DartsEventWork *work, int *score);
int DartsAdvanceScore(DartsEventWork *work, int *score);
void DartsScoreAdd(void *data);
void DartsProjectImpact(DartsEventWork *work, Vector3 *impact);
void DartsComputeCompactImpact(DartsEventWork *work, Vector3 *impact);
void DartsProjectileStart(void *data);
void DartsWobbleVertical(DartsEventWork *work);
void DartsWobbleHorizontal(DartsEventWork *work);
void DartsRotateAimingArrow(DartsEventWork *work);
void DartsCompactWobble(DartsEventWork *work);
int DartsCompactScore(Vector3 *position);
extern float dartsAiBands[5];
void DartsComputeAiTarget(DartsEventWork *work);
void DartsPlayingLoop(void *data);
void DartsHideBoardArrow(shen_name board, char index, char hidden);
int DartsIsPlayerTurn(DartsEventWork *work);
shen_name DartsTurnCharacter(DartsEventWork *work);
int DartsIsOpponentMatch(DartsEventWork *work);
shen_name DartsSelectBoard(DartsEventWork *work, shen_name actor);
int DartsOpponentIndex(shen_name opponent);
shen_name DartsTurnArrowName(DartsEventWork *work, int index);
int DartsIsMatchDecided(DartsEventWork *work);
int DartsBattleResult(DartsEventWork *work);
void DartsShowTurnActor(DartsEventWork *work);
void DartsSetTurn(DartsEventWork *work, char enemyTurn);
int DartsSwitchTurn(DartsEventWork *work);
void DartsStoreTurnBank(DartsEventWork *work);
void DartsLoadAudioBank(DartsEventWork *work, int bankMode);
void DartsShowBankScore(DartsEventWork *work, int row);
void DartsShowCompactScore(DartsEventWork *work, int row);
HLibTask *DartsQueueVoiceWait(HLibTask *parent, void *conversation);
void DartsWaitVoice(void *data);
void *DartsResponse(DartsEventWork *work, int index);
void *DartsExecuteResponse(int index, int opponentMatch, int playerTurn, int playerWon, shen_name opponent);
HLibTask *DartsQueueCameraWait(DartsEventWork *root, int camera, shen_name eye, shen_name target);
void DartsCameraWait(void *data);
void DartsCamera(DartsEventWork *work, int camera, shen_name eye, shen_name target, int delay);
HLibTaskCallback DartsOpponentThrowCallback(shen_name opponent);
void DartsOpponentAdmission(void *data);
int DartsIsHardJon(shen_name opponent, int difficulty);
int DartsOpponentAttachBone(shen_name opponent, int difficulty);
unsigned int DartsBeginOpponentThrow(shen_name opponent, int mode, int difficulty);
#else
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
#endif
