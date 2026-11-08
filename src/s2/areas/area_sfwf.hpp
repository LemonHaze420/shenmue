#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_street_debug.h"
#include "s2/areas/common/et_plural_control.h"
#include "s2/areas/area_loop_dc.h"

#define SFWF_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define SFWF_CHN SHEN_NAME4('C', 'H', 'N', '_')
#define SFWF_CYC SHEN_NAME4('C', 'Y', 'C', '_')
#define SFWF_REN SHEN_NAME4('R', 'E', 'N', '_')
#define SFWF_EEN SHEN_NAME4('E', 'E', 'N', '_')
#define SFWF_SFA SHEN_NAME4('S', 'F', 'A', '_')
#define SFWF_FIGHTER SHEN_NAME4('S', 'F', '6', '_')
#define SFWF_PROP_A428 SHEN_NAME4('a', '4', '2', '8')
#define SFWF_PROP_A118 SHEN_NAME4('a', '1', '1', '8')
#define SFWF_AUTHOR_0 SHEN_NAME4('A', 'U', '_', '0')
#define SFWF_AUTHOR_1 SHEN_NAME4('A', 'U', '_', '1')
#define SFWF_AUTHOR_2 SHEN_NAME4('A', 'U', '_', '2')
#define SFWF_INVALID 0xffffffffU
#define SFWF_STAG SHEN_NAME4('S', 'T', 'A', 'G')
#define SFWF_EEN_CAMERA SHEN_NAME4('d', 'a', 'm', 'B')
#define SFWF_RETRY_TITLE SHEN_NAME4('R', 'T', 'R', '2')
#define SFWF_OMAKE_SCENE SHEN_NAME4('O', 'M', 'A', 'K')
#define SFWF_QRON_SCENE SHEN_NAME4('Q', 'R', '0', '0')
#define SFWF_OPEN_HAND_POSE 11

typedef struct SfwfAuthorWork {
    HLibTask *task;
    int reserved04;
    shen_name author;
    short state;
} SfwfAuthorWork;
typedef struct SfwfAuthorLoopWork {
    SfwfAuthorWork *parent;
    short frame;
} SfwfAuthorLoopWork;

#define sfwfOpenHandPose ((void *)areaLoopHandPoses[SFWF_OPEN_HAND_POSE])

void SfwfMain(StreetFightStartParameters *start);
void QueueAuthor(shen_name author);
void AuthorMainCallback(void *data);
void QueueAuthorLoop(SfwfAuthorWork *work);
void AuthorLoopCallback(void *data);
void BeginAuthor(void);
void EndAuthor(SfwfAuthorWork *work);
void PrepareAuthorCharacters(void);
void RestoreAuthorCharacters(void);
void HideAuthorCharacter(shen_name character);
void ShowAuthorCharacter(shen_name character);
void HideAuthorProp(shen_name prop);
void ShowAuthorProp(shen_name prop);
void QueueNormal(StreetFightStartParameters *start);
void LoadNormalResources(StreetFightEventWork *work);
void PlaceNormal(StreetFightEventWork *work);
void UnparentStreetFightCharacter(shen_name character);
StreetFightEventWork *InitializeNormal(HLibTask *task);
void MainCallback(void *data);
int DefeatConversation(void *data);
void RetryBeginCallback(void *data);
void RetryChoiceCallback(void *data);
void RetryResetCallback(void *data);
void RetryCancelCallback(void *data);
void NormalDebugCallback(void *data);
void CleanupNormalCallback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
#include "s2/event_characters.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFWF_BASE{SHEN_NAME(BASE)}, SFWF_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFWF_DEBUG_PACKAGE{SHEN_NAME(QGBT)}, SFWF_RESOURCE_PACKAGE{SHEN_NAME(Q200)};
inline constexpr shen_name SFWF_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFWF_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFWF_STAGE_ORIGIN{371.7799987792969f, 76.6500015258789f, 120.77999877929688f};
inline constexpr Vector3 SFWF_DEBUG_OPPONENT_ORIGIN{371.7799987792969f, 76.1500015258789f, 120.77999877929688f};
inline constexpr Vector3 SFWF_DEBUG_PLAYER_POSITION{-9.199999809265137f, -.5f, -10.399999618530273f};
inline constexpr Vector3 SFWF_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -.5f, -5.5f};
inline constexpr Vector3 SFWF_DEBUG_EXIT_POSITION{365.1000061035156f, 76.13999938964844f, 121.5f};
inline constexpr Vector3i SFWF_DEBUG_EXIT_ROTATION{0, 49152, 0};
inline constexpr Vector3 SFWF_DEBUG_CONVERSATION_PLAYER_POSITION{0, -.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFWF_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr std::int8_t SFWF_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};

inline constexpr shen_name SFWF_FIGHTER{SHEN_NAME(SF6_)}, SFWF_CHN{SHEN_NAME(CHN_)}, SFWF_CYC{SHEN_NAME(CYC_)}, SFWF_REN{SHEN_NAME(REN_)};
inline constexpr shen_name SFWF_EEN{SHEN_NAME(EEN_)}, SFWF_SFA{SHEN_NAME(SFA_)}, SFWF_STAG{SHEN_NAME(STAG)};
inline constexpr shen_name SFWF_CHARACTERS[]{STREET_FIGHT_RYO, SFWF_CHN, SFWF_CYC, SFWF_REN, SFWF_EEN, SFWF_SFA, SFWF_FIGHTER, STREET_FIGHT_INVALID};
struct SfwfAuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t state;
    std::byte reserved12[6];
};
struct SfwfAuthorLoopWork {
    SfwfAuthorWork *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(SfwfAuthorWork) == 0x18 && offsetof(SfwfAuthorWork, state) == 0x10);
static_assert(sizeof(SfwfAuthorLoopWork) == 0x10 && offsetof(SfwfAuthorLoopWork, frame) == 8);
inline constexpr AuthoredPluralControlEntry SFWF_FIRST_AUTHOR_CONTROL[]{
    {.frame = 874, .character = SHEN_NAME(RYO_), .control = 1},
    {.frame = 874, .character = SHEN_NAME(REN_), .control = 1},
    {.frame = 874, .character = SHEN_NAME(SF6_), .control = 1},
    {.frame = 1094, .character = SHEN_NAME(REN_), .control = 0},
    {.frame = 1149, .character = SHEN_NAME(REN_), .control = 1},
    {.frame = 1149, .character = SHEN_NAME(SF6_), .control = 1},
    {.frame = 1149, .character = SHEN_NAME(EEN_), .control = 1},
    {.frame = 1439, .character = SHEN_NAME(SF6_), .control = 0},
    {.frame = 1599, .character = SHEN_NAME(SF6_), .control = 1},
    {.frame = 1599, .character = SHEN_NAME(REN_), .control = 0},
    {.frame = 1599, .character = SHEN_NAME(RYO_), .control = 1},
    {.frame = 1839, .character = SHEN_NAME(SF6_), .control = 0},
    {.frame = 1899, .character = SHEN_NAME(REN_), .control = 0},
    {.frame = 1899, .character = SHEN_NAME(SF6_), .control = 1},
    {.frame = 2029, .character = SHEN_NAME(REN_), .control = 0},
    {.frame = 2273, .character = SHEN_NAME(REN_), .control = 1},
    {.frame = 2422, .character = SHEN_NAME(RYO_), .control = 0},
    {.frame = 2422, .character = SHEN_NAME(REN_), .control = 0},
    {.frame = 2607, .character = SHEN_NAME(SF6_), .control = 1},
    {.frame = -1, .character = {}, .control = 0},
};
inline constexpr shen_name SFWF_AU0{SHEN_NAME(AU_0)}, SFWF_AU1{SHEN_NAME(AU_1)}, SFWF_AU2{SHEN_NAME(AU_2)};
inline constexpr shen_name SFWF_AUTHOR_PLAYER_DOUBLE{SHEN_NAME(a428)}, SFWF_AUTHOR_REN_DOUBLE{SHEN_NAME(a118)};
inline constexpr Vector3 SFWF_VICTORY_FIGHTER_POSITION{334.5f, 84.25f, 80.72000122070312f};
inline constexpr Vector3 SFWF_VICTORY_CHN_POSITION{334.5f, 84.25f, 90.72000122070312f};
inline constexpr Vector3 SFWF_EEN_POSITION{367.0f, 76.0999984741211f, 121.16000366210938f};
inline constexpr Vector3i SFWF_EEN_ROTATION{0, 51190, 0};
inline constexpr Vector3 SFWF_NORMAL_EXIT_POSITION{339.0400085449219f, 82.27999877929688f, 120.70999908447266f};
inline constexpr Vector3i SFWF_NORMAL_EXIT_ROTATION{0, 49152, 0};
inline constexpr std::uint8_t SFWF_FIGHTER_PARAMETERS[][12]{{85, 69, 69, 69, 32, 32, 80, 80, 112, 0, 0, 0}, {72, 101, 101, 101, 32, 48, 96, 106, 116, 0, 0, 0},
    {58, 120, 120, 120, 32, 64, 104, 117, 120, 0, 0, 0}, {40, 128, 128, 128, 32, 80, 112, 128, 136, 0, 0, 0},
    {19, 144, 144, 144, 32, 88, 120, 159, 152, 0, 0, 0}, {5, 159, 159, 159, 32, 80, 127, 175, 160, 0, 0, 0}};
inline constexpr AuthoredPluralControlEntry SFWF_VICTORY_AUTHOR_CONTROL[]{{.frame = 646, .character = STREET_FIGHT_RYO, .control = 1},
    {.frame = 646, .character = SFWF_REN, .control = 1}, {.frame = 646, .character = SFWF_EEN, .control = 0},
    {.frame = 646, .character = SFWF_FIGHTER, .control = 1}, {.frame = 999, .character = STREET_FIGHT_RYO, .control = 1},
    {.frame = 999, .character = SFWF_FIGHTER, .control = 1}, {.frame = 1109, .character = STREET_FIGHT_RYO, .control = 1},
    {.frame = 1109, .character = SFWF_REN, .control = 0}, {.frame = 1109, .character = SFWF_FIGHTER, .control = 1},
    {.frame = 1480, .character = STREET_FIGHT_RYO, .control = 0}, {.frame = 1480, .character = SFWF_REN, .control = 0},
    {.frame = -1, .character = {}, .control = 0}};
inline constexpr AuthoredPluralControlEntry SFWF_PREVIEW_AUTHOR_CONTROL[]{{.frame = 385, .character = STREET_FIGHT_RYO, .control = 1},
    {.frame = 385, .character = SFWF_FIGHTER, .control = 1}, {.frame = 385, .character = SFWF_SFA, .control = 1},
    {.frame = 645, .character = STREET_FIGHT_RYO, .control = 0}, {.frame = 645, .character = SFWF_FIGHTER, .control = 0},
    {.frame = 645, .character = SFWF_SFA, .control = 0}, {.frame = 1050, .character = SFWF_SFA, .control = 1},
    {.frame = 1050, .character = SFWF_EEN, .control = 1}, {.frame = -1, .character = {}, .control = 0}};
inline constexpr char SFWF_FIRST_ROUND_TEXT[] = "\xa1\xd6\xa3\xd2\xa3\xcf\xa3\xd5\xa3\xce\xa3\xc4 \xa3\xb1\xa1\xd7";
inline constexpr char SFWF_SECOND_ROUND_TEXT[] = "\xa1\xd6\xa3\xd2\xa3\xcf\xa3\xd5\xa3\xce\xa3\xc4 \xa3\xb2\xa1\xd7";
inline constexpr char SFWF_FINAL_ROUND_TEXT[] = "\xa1\xd6\xa3\xc6\xa3\xc9\xa3\xce\xa3\xc1\xa3\xcc \xa3\xd2\xa3\xcf\xa3\xd5\xa3\xce\xa3\xc4\xa1\xd7";
inline constexpr shen_name SFWF_RETRY_TITLE{SHEN_NAME(RTR2)}, SFWF_EXIT_AREA{SHEN_NAME(QR00)}, SFWF_MINIGAME_AREA{SHEN_NAME(OMAK)};
extern const ScriptAreaDefinition areaSfwfScriptDefinition;
} // namespace shenmue::s2
#endif
