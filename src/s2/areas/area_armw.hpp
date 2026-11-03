#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_debug_dc.h"
#include "s2/event_queued_dc.h"
#include "s2/event_library_tail_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/event_utility_dc.h"
#include "s2/areas/area_loop_dc.h"

#define ARMW_AREA SHEN_NAME4('A', 'R', 'M', 'W')
#define ARMW_TASK SHEN_NAME4('a', 'r', 'm', 'w')
#define ARMW_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define ARMW_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define ARMW_MONEY SHEN_NAME4('a', '0', '1', '2')
#define ARMW_BOX SHEN_NAME4('B', 'O', 'X', '_')
#define ARMW_SHIYO SHEN_NAME4('S', 'Y', 'O', '_')
#define ARMW_HONG SHEN_NAME4('H', 'N', 'G', '_')
#define ARMW_SF8 SHEN_NAME4('S', 'F', '8', '_')
#define ARMW_SFC SHEN_NAME4('S', 'F', 'C', '_')
#define ARMW_KZN SHEN_NAME4('K', 'Z', 'N', '_')
#define ARMW_SSY SHEN_NAME4('S', 'S', 'Y', '_')
#define ARMW_SF2 SHEN_NAME4('S', 'F', '2', '_')
#define ARMW_FRS SHEN_NAME4('F', 'R', 'S', '_')
#define ARMW_MHK SHEN_NAME4('M', 'H', 'K', '_')
#define ARMW_DML SHEN_NAME4('D', 'M', 'L', '_')
#define ARMW_MSH SHEN_NAME4('M', 'S', 'H', '_')
#define ARMW_DGY SHEN_NAME4('D', 'G', 'Y', '_')
#define ARMW_SD1 SHEN_NAME4('S', 'D', '1', '_')
#define ARMW_WD1 SHEN_NAME4('W', 'D', '1', '_')
#define ARMW_AD2 SHEN_NAME4('A', 'D', '2', '_')
#define ARMW_WD3 SHEN_NAME4('W', 'D', '3', '_')
#define ARMW_GAU SHEN_NAME4('G', 'A', 'U', '_')
#define ARMW_INVALID 0xffffffffU
#define ARMW_MOTION_FLAGS 0x40000000U
#define ARMW_AR02 SHEN_NAME4('A', 'R', '0', '2')
#define ARMW_QTB4 SHEN_NAME4('Q', 'T', 'B', '4')
#define ARMW_YASA SHEN_NAME4('Y', 'A', 'S', 'A')
#define ARMW_WON SHEN_NAME4('W', 'O', 'N', '_')
#define ARMW_BATTLE_INPUT SHEN_NAME4('B', 'T', 'B', 'A')
#define ARMW_EPSILON 1.1920929e-7f
#define ARMW_SNAP(value) if (__builtin_fabs(value) < ARMW_EPSILON) value = 0.0f
#define ARMW_SNAP_VALUE(value) (__builtin_fabs(value) < ARMW_EPSILON ? 0.0f : (value))
#define ARMW_MAX(a, b) ((a) < (b) ? (b) : (a))
#define ARMW_MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct ArmwVoiceWait {
    HLibTask *parent;
    int delay, index, reserved0c, timeout;
    unsigned int flags;
} ArmwVoiceWait;

typedef struct ArmwOpponent {
    shen_name character;
    int strength, minimumStrength, maximumStrength, stamina;
    short strategy, warmupFrames, buttonA, buttonLeft, buttonDown, buttonUp, finishQte, reserved22;
} ArmwOpponent;

typedef struct ArmwWork {
    HLibTask *self;
    EventReadCharactersWork *characters;
    shen_name nextScene;
    int nextEntry, falseStarts, day, rotation, opponentIndex;
    unsigned int flags;
    short step, phase;
    unsigned int playerHistory, opponentHistory, strategyFlags;
    int stamina, initialStamina, strategy, strength, minimumStrength, maximumStrength;
    int opponentRate, playerFace, opponentFace;
    float shake, target, position, acceleration;
    int stalemate, settling;
    shen_name player, opponent, secondOpponent, thirdOpponent, referee;
    int presses;
    unsigned int frames, recentRate;
    int reserved90;
    short reserved94, fatigueFrames;
    short warmupFrames, buttonA, buttonLeft, buttonDown, buttonUp, finishQte;
    float fatigueRate;
    int fatigueVoice, staminaVoice;
} ArmwWork;

extern unsigned int armwIntroductionVariant;
extern float armwQtePenalty;
extern int armwQteButton;
extern int armwQteSpeaker;
extern int armwHandMotion;
extern int armwStarted;
extern int armwStartFrames;
extern int armwArgument;
extern short armwLocation;

typedef struct ArmwCrowdPosition {
    float x, y, z, yaw;
} ArmwCrowdPosition;

extern const unsigned int armwRefereeMotions[14][2];
extern const unsigned int armwOpponentMotions[11][4];

int armwWager;
unsigned short armwOpponentVariant;
unsigned short armwRefereeVariant;
int armwLosingBet;

void ArmwPlaceCrowd(const shen_name *names);
int ArmwIntroduceHong(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwIntroduceSf8(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwIntroduceSfc(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwIntroduceKzn(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwOfferRematch(shen_name player, shen_name opponent, shen_name referee);
int ArmwWarnFalseStart(shen_name player, shen_name opponent, shen_name referee, int count);
int ArmwFirstFalseStart(shen_name referee, shen_name opponent, int delay);
int ArmwSecondFalseStart(shen_name referee, shen_name opponent, int delay);
int ArmwThirdFalseStart(shen_name referee, shen_name opponent, int delay);
int ArmwAnnounceBet(shen_name player, shen_name opponent, shen_name referee);
int ArmwHongCollects(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwSf8Collects(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwSfcCollects(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwDgyCollects(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwKznCollects(shen_name player, shen_name opponent, shen_name referee, int delay);
int ArmwRefereeCollects(shen_name player, shen_name opponent, shen_name referee, int delay);

void ArmwSetVoicePrefixes(shen_name referee, shen_name opponent);
void ArmwVoiceWaitCallback(void *data);
void ArmwWaitVoices(HLibTask *parent, int delay, unsigned int flags, int wait);
void ArmwAppendVoice(int speaker, const char *suffix);
void ArmwVoice(int speaker, const char *suffix, int delay);
int ArmwGetVoiceSkipped(void);

void ArmwStart(void *parameter);
extern shen_name armwCharacters[15];
extern shen_name armwTutorialCharacters[2];
extern int armwBets[12][5];
extern const float armwBases[12][4];
extern const int armwExits[12][2];
extern const shen_name armwReferees[12][2];
extern const int armwFlagIds[12];
extern const ArmwOpponent armwOpponents[60];
extern const short armwOrder[12][6];
extern const short armwRotations[7][2][3];
void ArmwMainCallback(void *data);
void ArmwInitializeResources(ArmwWork *work);
void ArmwLoadRefereePackage(int variant);
void ArmwFreeRefereePackages(void);
void ArmwLoadOpponentPackage(int variant);
void ArmwFreeOpponentPackages(void);
int ArmwBeginExit(ArmwWork *work);
void ArmwSetExitStep(ArmwWork *work);
void ArmwDebugExit(void *data);
void ArmwEndCallback(void *data);
void ArmwSelectCharacters(ArmwWork *work);
void ArmwSelectOpponent(ArmwWork *work);
int ArmwGetDay(void);
int ArmwGetRotation(void);
int ArmwGetOpponentIndex(void);
void ArmwFinishLadderFlags(void);
void ArmwResetAuthFlags(void);
short ArmwGetLadder(ArmwWork *work);
int ArmwReady(ArmwWork *work);
int ArmwNextOpponent(ArmwWork *work);
void ArmwReplaceOpponent(ArmwWork *work);
int ArmwOpponentLoaded(shen_name name);
int ArmwAskTutorial(ArmwWork *work);
int ArmwExplain(ArmwWork *work);
int ArmwWalkAway(ArmwWork *work);
int ArmwRefuse(ArmwWork *work);
int ArmwPrepareMatch(ArmwWork *work);
int ArmwOfferNextMatch(ArmwWork *work);
int ArmwFalseStart(ArmwWork *work);
int ArmwWin(ArmwWork *work);
int ArmwEndLadder(ArmwWork *work);
int ArmwLose(ArmwWork *work);
int ArmwTutorial(ArmwWork *work);
int ArmwTutorialResult(ArmwWork *work);
void ArmwResetMatch(ArmwWork *work);
int ArmwPlayMatch(ArmwWork *work);
unsigned int ArmwButtons(ArmwWork *work);
void ArmwUpdatePose(ArmwWork *work, int mode);
void ArmwUpdateFaces(ArmwWork *work);
int ArmwDirectionQte(ArmwWork *work);
int ArmwFinishQte(ArmwWork *work);
void ArmwMatchHands(ArmwWork *work);
void ArmwRestoreHands(ArmwWork *work);
int ArmwWinAnimation(ArmwWork *work);
int ArmwLoseAnimation(ArmwWork *work);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name ARMW_AREA{SHEN_NAME(ARMW)}, ARMW_TASK{SHEN_NAME(armw)}, ARMW_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name ARMW_RYO{SHEN_NAME(RYO_)}, ARMW_MONEY{SHEN_NAME(a012)}, ARMW_BOX{SHEN_NAME(BOX_)};
inline constexpr shen_name ARMW_ROCKO{SHEN_NAME(RCKO)}, ARMW_SHIYO{SHEN_NAME(SYO_)}, ARMW_HONG{SHEN_NAME(HNG_)}, ARMW_BATTLE_INPUT{SHEN_NAME(BTBA)};
struct ArmwOpponent {
    shen_name character;
    std::int32_t strength, minimumStrength, maximumStrength, stamina;
    std::int16_t strategy, warmupFrames, buttonA, buttonLeft, buttonDown, buttonUp, finishQte, reserved22;
};
static_assert(sizeof(ArmwOpponent) == 0x24 && offsetof(ArmwOpponent, strategy) == 0x14);

struct ArmwWork {
    HLib::Task *self;
    EventReadCharactersWork *characters;
    shen_name nextScene;
    std::int32_t nextEntry, falseStarts, day, rotation, opponentIndex;
    std::uint32_t flags;
    std::int16_t step, phase;
    std::uint32_t playerHistory, opponentHistory, strategyFlags;
    std::int32_t stamina, initialStamina, strategy, strength, minimumStrength, maximumStrength;
    float opponentRate;
    std::int32_t playerFace, opponentFace;
    float shake, target, position, acceleration;
    std::int32_t stalemate, settling;
    shen_name player, opponent, secondOpponent, thirdOpponent, referee;
    std::int32_t presses;
    float frames, previousFrames;
    std::uint32_t recentRate;
    std::int32_t reserved9c;
    std::int16_t reserveda0, fatigueFrames, warmupFrames, buttonA, buttonLeft, buttonDown, buttonUp, finishQte;
    float reservedb0;
    std::int32_t fatigueVoice, staminaVoice;
    float authorFrames;
};
static_assert(sizeof(ArmwWork) == 0xc0 && offsetof(ArmwWork, step) == 0x2c && offsetof(ArmwWork, stamina) == 0x3c);
static_assert(offsetof(ArmwWork, target) == 0x64 && offsetof(ArmwWork, player) == 0x78 && offsetof(ArmwWork, frames) == 0x90);
static_assert(offsetof(ArmwWork, warmupFrames) == 0xa4 && offsetof(ArmwWork, finishQte) == 0xae && offsetof(ArmwWork, authorFrames) == 0xbc);
struct ArmwVoiceWait {
    HLib::Task *parent;
    std::int32_t delay, index, reserved10;
    float timeout;
    std::uint32_t flags, reserved1c;
};
static_assert(sizeof(ArmwVoiceWait) == 0x20 && offsetof(ArmwVoiceWait, timeout) == 0x14 && offsetof(ArmwVoiceWait, flags) == 0x18);
extern const ArmwOpponent armwOpponents[60];
inline constexpr std::int32_t ARMW_FLAGS[]{3, 4, 5, 6, 7, 8, 4, 3, 5, 7, 6, 8};
inline constexpr std::int32_t ARMW_BETS[12][5]{{10, 20, 30, 40, 50}, {20, 30, 40, 50, 60}, {40, 50, 60, 70, 80}, {30, 40, 50, 60, 70}, {50, 60, 70, 80, 90},
    {60, 70, 80, 90, 100}, {10, 20, 30, 40, 50}, {20, 30, 40, 50, 60}, {30, 40, 50, 60, 70}, {40, 50, 60, 70, 80}, {50, 60, 70, 80, 90}, {60, 70, 80, 90, 100}};
inline constexpr float ARMW_BASES[12][4]{{205, 100.22f, 320, 120}, {234, 100.22f, 348, 90}, {247.9f, 100.22f, 363.1f, 180}, {299.4f, 107.87f, 329.3f, 120},
    {300.6f, 107.87f, 331.3f, 120}, {301.8f, 107.87f, 333.4f, 120}, {240.3f, 100, 132.3f, 180}, {237.6f, 100, 132.3f, 180}, {234.9f, 100, 132.3f, 180},
    {232.2f, 100, 132.3f, 180}, {229.5f, 100, 132.3f, 180}, {226.8f, 100, 132.3f, 180}};
inline constexpr std::int16_t ARMW_ROTATIONS[7][2][3]{{{0, 1, 2}, {4, 1, 3}}, {{3, 4, 2}, {1, 0, 2}}, {{2, 1, 0}, {3, 2, 4}}, {{3, 4, 0}, {2, 3, 1}},
    {{2, 3, 1}, {0, 1, 4}}, {{4, 3, 0}, {1, 3, 2}}, {{2, 1, 3}, {4, 2, 0}}};
inline constexpr std::int16_t ARMW_OPPONENT_ORDER[12][6]{{0, 1, 2, 3, 4, -1}, {5, 6, 7, 8, 9, -1}, {10, 11, 12, 13, 14, -1}, {15, 16, 17, 18, 19, -1},
    {20, 21, 22, 23, 24, -1}, {25, 26, 27, 28, 29, -1}, {30, 31, 32, 33, 34, -1}, {35, 36, 37, 38, 39, -1}, {40, 41, 42, 43, 44, -1}, {45, 46, 47, 48, 49, -1},
    {50, 51, 52, 53, 54, -1}, {55, 56, 57, 58, 59, -1}};
inline constexpr std::uint32_t ARMW_OPPONENT_MOTIONS[11][4]{{9353, 9288, 9306, 9224}, {9356, 9291, 9307, 9227}, {9357, 9292, 9292, 9228},
    {9358, 9293, 9293, 9229}, {9350, 9294, 9294, 9230}, {9354, 9289, 9289, 9225}, {9351, 9286, 9286, 9222}, {9345, 9281, 9281, 9217}, {9346, 9282, 9282, 9218},
    {9347, 9283, 9283, 9219}, {9348, 9284, 9284, 9220}};
inline constexpr std::uint32_t ARMW_REFEREE_MOTIONS[14][2]{{9419, 9099}, {9413, 9093}, {9409, 9089}, {9421, 9100}, {9416, 9096}, {9417, 9097}, {9418, 9098},
    {9420, 9102}, {9414, 9094}, {9415, 9095}, {9412, 9092}, {9411, 9091}, {9410, 9090}, {9423, 9103}};
struct ArmwState {
    std::int32_t argument, location, bet, refereeMotion, opponentMotion;
    std::int32_t started, startFrames, introductionVariant, qteButton, qteSpeaker, active;
    float qtePenalty;
    std::int32_t voiceCount, voiceSkipped;
    char voicePrefixes[4][16], voices[16][12];
    shen_name characters[6], tutorialCharacters[2];
    void *handMotion;
};
extern ArmwState armw;
void ArmwNullsub();
std::int32_t GetArmwGameplayActive();
void InitializeArmwEvent();
void RunArmwEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition armwScriptDefinition;
void ArmwSetVoicePrefixes(shen_name referee, shen_name opponent);
void ArmwAppendVoice(std::int32_t speaker, const char *suffix);
void ArmwVoice(std::int32_t speaker, const char *suffix, std::int32_t delay = 0);
void ArmwWaitVoices(HLib::Task *parent, std::uint32_t flags = 0);
void ArmwPosition(shen_name character, shen_name parent, const Vector3 &position, std::int32_t yaw);
void ArmwCamera(std::int32_t camera, std::int32_t delay = 0);
void ArmwUpdatePose(ArmwWork &work);
void ArmwResetMatch(ArmwWork &work);
std::int32_t ArmwPlayMatch(ArmwWork &work);
std::int32_t ArmwDirectionQte(ArmwWork &work);
std::int32_t ArmwFinishQte(ArmwWork &work);
std::int32_t ArmwWinAnimation(ArmwWork &work);
std::int32_t ArmwLoseAnimation(ArmwWork &work);
void ArmwRestoreHands(ArmwWork &work);
void ArmwMatchHands(ArmwWork &work);
std::int32_t ArmwIntroduceOpponent(ArmwWork &work);
std::int32_t ArmwWarnFalseStart(ArmwWork &work);
std::int32_t ArmwOfferRematch(ArmwWork &work);
std::int32_t ArmwOpponentCollects(ArmwWork &work);
std::int32_t ArmwRefereeCollects(ArmwWork &work);
} // namespace shenmue::s2::areas
#endif
