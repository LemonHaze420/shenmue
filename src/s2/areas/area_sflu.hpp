#if defined(SHENMUE_EVENT_OVERLAY)
#ifndef SHENMUE_AREA_SFLU_DC_H
#define SHENMUE_AREA_SFLU_DC_H
#include "s2/areas/common/et_street.h"
#include "s2/areas/common/et_street_debug.h"
#include "s2/areas/common/et_plural_control.h"
#include "s2/areas/area_loop_dc.h"
#define SFLU_BASE SHEN_NAME4('B', 'A', 'S', 'X')
#define SFLU_FUY SHEN_NAME4('F', 'U', 'Y', '_')
#define SFLU_DEBUG_PACKAGE SHEN_NAME4('Q', 'K', 'B', 'T')
#define SFLU_RESOURCE_PACKAGE SHEN_NAME4('Q', '2', '0', '0')
#define SFLU_RETRY_TITLE SHEN_NAME4('R', 'T', 'R', '2')
#define SFLU_SF4 SHEN_NAME4('S', 'F', '4', '_')
#define SFLU_JOP SHEN_NAME4('J', 'O', 'P', '_')
#define SFLU_KF1 SHEN_NAME4('K', 'F', '1', '_')
#define SFLU_KF2 SHEN_NAME4('K', 'F', '2', '_')
#define SFLU_KF3 SHEN_NAME4('K', 'F', '3', '_')
#define SFLU_KR1 SHEN_NAME4('K', 'R', '1', '_')
#define SFLU_KR2 SHEN_NAME4('K', 'R', '2', '_')
#define SFLU_SF9 SHEN_NAME4('S', 'F', '9', '_')
#define SFLU_BF6 SHEN_NAME4('B', 'F', '6', '_')
#define SFLU_GOI SHEN_NAME4('G', 'O', 'I', '_')
#define SFLU_AMI1 SHEN_NAME4('a', 'm', 'i', '1')
#define SFLU_AMI2 SHEN_NAME4('a', 'm', 'i', '2')
#define SFLU_AMI3 SHEN_NAME4('a', 'm', 'i', '3')
#define SFLU_AMI4 SHEN_NAME4('a', 'm', 'i', '4')
#define SFLU_AMI5 SHEN_NAME4('a', 'm', 'i', '5')
#define SFLU_A734 SHEN_NAME4('a', '7', '3', '4')
#define SFLU_A800 SHEN_NAME4('a', '8', '0', '0')
#define SFLU_A798 SHEN_NAME4('a', '7', '9', '8')
#define SFLU_A516 SHEN_NAME4('a', '5', '1', '6')
#define SFLU_A512 SHEN_NAME4('a', '5', '1', '2')
#define SFLU_AUTH_0 SHEN_NAME4('A', 'U', '_', '0')
#define SFLU_AUTH_1 SHEN_NAME4('A', 'U', '_', '1')
#define SFLU_AUTH_2 SHEN_NAME4('A', 'U', '_', '2')
#define SFLU_AUTH_3 SHEN_NAME4('A', 'U', '_', '3')
#define SFLU_AUTH_4 SHEN_NAME4('A', 'U', '_', '4')
#define SFLU_AUTH_5 SHEN_NAME4('A', 'U', '_', '5')
#define SFLU_AUTH_6 SHEN_NAME4('A', 'U', '_', '6')
#define SFLU_STORY_NEXT_AREA SHEN_NAME4('Q', 'F', '3', '9')
#define SFLU_STORY_EXIT_AREA SHEN_NAME4('Q', 'K', 'B', 'T')
typedef struct SfluAuthorWork {
    HLibTask *task;
    unsigned int reserved04;
    shen_name command;
    short state, reserved0e;
} SfluAuthorWork;
typedef struct SfluAuthorLoopWork {
    SfluAuthorWork *owner;
    short frame;
    unsigned short reserved06;
} SfluAuthorLoopWork;
typedef struct SfluElevatorPanelWork {
    int start, end, next, tens, delay;
} SfluElevatorPanelWork;
void SfluMain(StreetFightStartParameters *start);
void QueueAuthor(shen_name command);
void AuthorCallback(void *data);
void QueueAuthorLoop(SfluAuthorWork *work);
void AuthorLoopCallback(void *data);
void PrepareAuthor(shen_name command);
void InitializeElevatorCharacters(void);
void QueueElevatorPanel(int start, int end);
void ElevatorPanelCallback(void *data);
void EndAuthor(SfluAuthorWork *work);
void LoadAuthorCharacters(shen_name command);
void UnloadAuthorCharacters(shen_name command);
void HideAuthorCharacter(shen_name character);
void ShowAuthorCharacter(shen_name character);
void HideAuthorProp(shen_name character);
void ShowAuthorProp(shen_name character);
void ShowElevatorPart(shen_name character, int part, int visible);
void QueueNormal(StreetFightStartParameters *start);
StreetFightEventWork *InitializeNormal(HLibTask *task);
void PlaceNormal(StreetFightEventWork *work);
void MainCallback(void *data);
void NormalDebugCallback(void *data);
void CleanupNormalCallback(void *data);
void RetryBeginCallback(void *data);
void RetryChoiceCallback(void *data);
void ResetMain(StreetFightEventWork *work);
void RetryCancelCallback(void *data);
void UpdateBattleQte(StreetFightEventWork *work);
void ToggleTrainingAudience(int enabled);
void ToggleFence(int enabled);
void UpdateFenceAlpha(void);
void LostExpressionCallback(void *data);
int DefeatConversation(void *data);
void ConfigureTrainingHands(void);
void ConfigureVictoryHands(void);
void ResetQteCharacters(StreetFightEventWork *work);
extern int trainingInputFailed;
#endif
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/street_fight_tables.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFLU_BASE{SHEN_NAME(BASX)}, SFLU_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFLU_DEBUG_PACKAGE{SHEN_NAME(QKBT)}, SFLU_RESOURCE_PACKAGE{SHEN_NAME(Q200)};
inline constexpr shen_name SFLU_RETRY_TITLE{SHEN_NAME(RTR2)};
inline constexpr shen_name SFLU_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFLU_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFLU_STAGE_ORIGIN{238.5f, 82.0f, 126.0f};
inline constexpr Vector3 SFLU_DEBUG_OPPONENT_ORIGIN{238.5f, 81.5f, 126.0f};
inline constexpr Vector3 SFLU_DEBUG_PLAYER_POSITION{-9.199999809265137f, -.5f, -10.399999618530273f};
inline constexpr Vector3 SFLU_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -.5f, -5.5f};
inline constexpr Vector3 SFLU_DEBUG_EXIT_POSITION{239.39999389648438f, 82.0f, 118.0999984741211f};
inline constexpr Vector3i SFLU_DEBUG_EXIT_ROTATION{0, 32768, 0};
inline constexpr Vector3 SFLU_DEBUG_CONVERSATION_PLAYER_POSITION{0, -.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFLU_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr std::int8_t SFLU_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr shen_name SFLU_SF4{SHEN_NAME(SF4_)};
inline constexpr shen_name SFLU_JOP{SHEN_NAME(JOP_)};
inline constexpr shen_name SFLU_KF1{SHEN_NAME(KF1_)};
inline constexpr shen_name SFLU_KF2{SHEN_NAME(KF2_)};
inline constexpr shen_name SFLU_KF3{SHEN_NAME(KF3_)};
inline constexpr shen_name SFLU_KR1{SHEN_NAME(KR1_)};
inline constexpr shen_name SFLU_KR2{SHEN_NAME(KR2_)};
inline constexpr shen_name SFLU_SF9{SHEN_NAME(SF9_)};
inline constexpr shen_name SFLU_BF6{SHEN_NAME(BF6_)};
inline constexpr shen_name SFLU_GOI{SHEN_NAME(GOI_)};
inline constexpr shen_name SFLU_AMI1{SHEN_NAME(ami1)};
inline constexpr shen_name SFLU_AMI2{SHEN_NAME(ami2)};
inline constexpr shen_name SFLU_AMI3{SHEN_NAME(ami3)};
inline constexpr shen_name SFLU_AMI4{SHEN_NAME(ami4)};
inline constexpr shen_name SFLU_AMI5{SHEN_NAME(ami5)};
inline constexpr shen_name SFLU_A734{SHEN_NAME(a734)};
inline constexpr shen_name SFLU_A800{SHEN_NAME(a800)};
inline constexpr shen_name SFLU_A798{SHEN_NAME(a798)};
inline constexpr shen_name SFLU_A516{SHEN_NAME(a516)};
inline constexpr shen_name SFLU_A512{SHEN_NAME(a512)};
inline constexpr shen_name SFLU_AUTH_0{SHEN_NAME(AU_0)};
inline constexpr shen_name SFLU_AUTH_1{SHEN_NAME(AU_1)};
inline constexpr shen_name SFLU_AUTH_2{SHEN_NAME(AU_2)};
inline constexpr shen_name SFLU_AUTH_3{SHEN_NAME(AU_3)};
inline constexpr shen_name SFLU_AUTH_4{SHEN_NAME(AU_4)};
inline constexpr shen_name SFLU_AUTH_5{SHEN_NAME(AU_5)};
inline constexpr shen_name SFLU_AUTH_6{SHEN_NAME(AU_6)};
inline constexpr shen_name SFLU_AUTHOR_CHARACTERS_0[]{
    SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(KF3_), SHEN_NAME(KR1_), SHEN_NAME(KR2_), SHEN_NAME(SF4_), SHEN_NAME(SF9_), STREET_FIGHT_INVALID};
inline constexpr shen_name SFLU_AUTHOR_CHARACTERS_1[]{
    SHEN_NAME(JOP_), SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(KR2_), SHEN_NAME(SF4_), STREET_FIGHT_INVALID};
inline constexpr shen_name SFLU_AUTHOR_CHARACTERS_2[]{
    SHEN_NAME(KR2_), SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(BF6_), SHEN_NAME(SF4_), STREET_FIGHT_INVALID};
inline constexpr shen_name SFLU_AUTHOR_CHARACTERS_3[]{
    SHEN_NAME(JOP_), SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(KR2_), SHEN_NAME(SF4_), SHEN_NAME(BF6_), STREET_FIGHT_INVALID};
inline constexpr shen_name SFLU_AUTHOR_CHARACTERS_4[]{SHEN_NAME(JOP_), STREET_FIGHT_INVALID};
inline constexpr shen_name SFLU_AUTHOR_CHARACTERS_5[]{SHEN_NAME(SF4_), STREET_FIGHT_INVALID};
inline constexpr shen_name SFLU_AUTHOR_CHARACTERS_6[]{SHEN_NAME(SF4_), STREET_FIGHT_INVALID};
inline constexpr const shen_name *SFLU_AUTHOR_CHARACTERS[]{SFLU_AUTHOR_CHARACTERS_0, SFLU_AUTHOR_CHARACTERS_1, SFLU_AUTHOR_CHARACTERS_2,
    SFLU_AUTHOR_CHARACTERS_3, SFLU_AUTHOR_CHARACTERS_4, SFLU_AUTHOR_CHARACTERS_5, SFLU_AUTHOR_CHARACTERS_6};
inline constexpr AuthoredPluralControlEntry SFLU_PLURAL_FIRST[]{{.frame = 0, .character = SHEN_NAME(KF1_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(KF2_), .control = 1}, {.frame = 0, .character = SHEN_NAME(KF3_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(KR1_), .control = 1}, {.frame = 0, .character = SHEN_NAME(KR2_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(BF6_), .control = 1}, {.frame = 0, .character = SHEN_NAME(SF4_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(GOI_), .control = 1}, {.frame = 600, .character = SHEN_NAME(SF4_), .control = 0},
    {.frame = 600, .character = SHEN_NAME(GOI_), .control = 0}, {.frame = 1600, .character = SHEN_NAME(KF1_), .control = 0},
    {.frame = 1600, .character = SHEN_NAME(KF2_), .control = 0}, {.frame = -1, .character = shen_name{}, .control = 0}};
inline constexpr AuthoredPluralControlEntry SFLU_PLURAL_REPEAT[]{{.frame = 841, .character = SHEN_NAME(KF1_), .control = 1},
    {.frame = 841, .character = SHEN_NAME(KF2_), .control = 1}, {.frame = 841, .character = SHEN_NAME(KF3_), .control = 1},
    {.frame = 841, .character = SHEN_NAME(KR1_), .control = 1}, {.frame = 841, .character = SHEN_NAME(KR2_), .control = 1},
    {.frame = 841, .character = SHEN_NAME(BF6_), .control = 1}, {.frame = -1, .character = shen_name{}, .control = 0}};
inline constexpr AuthoredPluralControlEntry SFLU_PLURAL_EVENING[]{{.frame = 0, .character = SHEN_NAME(KF1_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(KF2_), .control = 1}, {.frame = 0, .character = SHEN_NAME(KF3_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(KR1_), .control = 1}, {.frame = 0, .character = SHEN_NAME(KR2_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(BF6_), .control = 1}, {.frame = 421, .character = SHEN_NAME(RYO_), .control = 1},
    {.frame = 421, .character = SHEN_NAME(SF4_), .control = 1}, {.frame = -1, .character = shen_name{}, .control = 0}};
inline constexpr AuthoredPluralControlEntry SFLU_PLURAL_RESULT[]{{.frame = 0, .character = SHEN_NAME(KF1_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(KF2_), .control = 1}, {.frame = 0, .character = SHEN_NAME(KF3_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(KR1_), .control = 1}, {.frame = 0, .character = SHEN_NAME(KR2_), .control = 1},
    {.frame = 0, .character = SHEN_NAME(BF6_), .control = 1}, {.frame = 476, .character = SHEN_NAME(RYO_), .control = 1},
    {.frame = 476, .character = SHEN_NAME(JOP_), .control = 1}, {.frame = -1, .character = shen_name{}, .control = 0}};
inline constexpr std::int32_t SFLU_QTE_FIRST_WAIT[]{16, 20, 24};
inline constexpr std::int32_t SFLU_QTE_SECOND_WAIT[]{24, 30, 36};
inline constexpr std::int32_t SFLU_TRAINING_WAIT[]{32, 40, 48};
inline constexpr std::int32_t SFLU_ELEVATOR_TENS[]{152, 163, 174, 185, 196};
inline constexpr std::int32_t SFLU_ELEVATOR_SEGMENTS[]{
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 162, 163, 174, 185, 196, 207, 231, 233, 234, -1};
inline constexpr std::int32_t SFLU_ELEVATOR_PANEL_SEGMENTS[]{152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 163, 174, 185, 196, 207, 249, 250, 251, -1};
inline constexpr Vector3 SFLU_TRAINING_BASE{239.5f, 82.0f, 124.0f};
inline constexpr Vector3 SFLU_TRAINING_PLAYER_SUCCESS_POSITION{1.100000023841858f, 0.0f, 0.33500000834465027f};
inline constexpr Vector3 SFLU_TRAINING_FIGHTER_SUCCESS_POSITION{0.07999999821186066f, 0.0f, 0.8999999761581421f};
inline constexpr Vector3 SFLU_LOST_EYE_FIRST{239.13760375976562f, 85.65229797363281f, 122.03849792480469f};
inline constexpr Vector3 SFLU_LOST_EYE_SECOND{234.8074951171875f, 82.05979919433594f, 122.96849822998047f};
inline constexpr Vector3 SFLU_STORY_EXIT_POSITION{245.94000244140625f, 89.19999694824219f, 125.5999984741211f};
inline constexpr Vector3i SFLU_STORY_EXIT_ROTATION{0, 17476, 0};
inline constexpr std::uint8_t SFLU_FIGHT_PARAMETERS[][12]{{112, 160, 160, 160, 112, 32, 0, 48, 111, 0, 0, 0}, {96, 168, 168, 168, 96, 48, 0, 42, 116, 0, 0, 0},
    {90, 176, 176, 176, 80, 64, 0, 37, 143, 0, 0, 0}, {74, 184, 184, 184, 64, 80, 0, 32, 160, 0, 0, 0}, {58, 197, 197, 197, 48, 112, 0, 16, 186, 0, 0, 0},
    {31, 213, 213, 213, 32, 144, 0, 5, 186, 0, 0, 0}};
inline std::int32_t SFLU_TRAINING_HIGH_HAND[][3]{{0, 908, 47153}, {0, 365, 63714}, {0, 59712, 2911}, {0, 63716, 45696}, {0, 726, 46244}, {0, 63716, 3821},
    {0, 364, 1273}, {0, 0, 47335}, {0, 726, 45334}, {0, 64445, 3274}, {0, 546, 1820}, {0, 1273, 46608}, {0, 908, 47152}, {0, 65354, 2730}, {0, 546, 2182},
    {0, 1456, 47335}, {0, 908, 46788}, {0, 364, 3456}, {0, 908, 1091}};
inline std::int32_t SFLU_TRAINING_LOW_HAND[][3]{{0, 0, -10922}, {0, 0, -10558}, {0, 1456, 0}, {0, 0, -18022}, {0, 0, -17112}, {0, 0, -18386}, {0, 0, 0},
    {0, 0, -12196}, {0, 0, -18204}, {0, 0, -16748}, {0, 0, 0}, {0, 0, -11832}, {0, 0, -18386}, {0, 0, -15109}, {0, 0, 0}, {0, 910, -12014}, {0, -2184, -16748},
    {0, -2184, -10012}, {0, 0, 0}};
inline constexpr char SFLU_MOVE_TELOP[] = "\x98\x41\x8e\xe8\x96\x43";
inline constexpr std::uint16_t SFLU_TRAINING_BUTTONS[]{4, 64, 1, 256};
inline constexpr shen_name SFLU_STORY_NEXT_AREA{SHEN_NAME(QF39)};
inline constexpr shen_name SFLU_STORY_EXIT_AREA{SHEN_NAME(QKBT)};
inline constexpr const char *SFLU_ROUND_MESSAGES[]{"\xa1\xd6\xa3\xd2\xa3\xcf\xa3\xd5\xa3\xce\xa3\xc4\x20\xa3\xb1\xa1\xd7",
    "\xa1\xd6\xa3\xd2\xa3\xcf\xa3\xd5\xa3\xce\xa3\xc4\x20\xa3\xb2\xa1\xd7",
    "\xa1\xd6\xa3\xc6\xa3\xc9\xa3\xce\xa3\xc1\xa3\xcc\x20\xa3\xd2\xa3\xcf\xa3\xd5\xa3\xce\xa3\xc4\xa1\xd7"};
inline constexpr char SFLU_TRAINING_RETRY_MESSAGE[] = "\x89\xbd\x82\xa9\x83\x4c\x81\x5b\x82\xf0\x89\x9f\x82\xb7\x82\xc6\x83\x8a\x83\x67\x83\x89\x83\x43\x0a";
inline constexpr shen_name SFLU_NORMAL_CHARACTERS[]{SHEN_NAME(RYO_), SHEN_NAME(SF4_), SHEN_NAME(GOI_), SHEN_NAME(JOP_), SHEN_NAME(KF1_), SHEN_NAME(KF2_),
    SHEN_NAME(KF3_), SHEN_NAME(KR1_), SHEN_NAME(KR2_), SHEN_NAME(BF6_), SHEN_NAME(SF9_), STREET_FIGHT_INVALID};
inline constexpr const char *SFLU_DEBUG_NEXT_PRIZE_MESSAGES[]{
    STREET_FIGHT_DEBUG_PRIZE_40, STREET_FIGHT_DEBUG_PRIZE_60, STREET_FIGHT_DEBUG_PRIZE_80, STREET_FIGHT_DEBUG_PRIZE_100, STREET_FIGHT_DEBUG_PRIZE_120};
inline constexpr Vector3 SFLU_DEBUG_CONVERSATION_FIGHTER_POSITION{-0.5879999995231628f, 0.0f, -1.899999976158142f};
inline constexpr Vector3 SFLU_DEBUG_DEFEAT_FIGHTER_POSITION{-0.988f, 0.0f, -1.2f};
extern const ScriptAreaDefinition areaSfluScriptDefinition;
} // namespace shenmue::s2
#endif
