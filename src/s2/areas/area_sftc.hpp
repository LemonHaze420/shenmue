#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_street_debug.h"
#include "s2/areas/area_loop_dc.h"

#define SFTC_WD2 SHEN_NAME4('W', 'D', '2', '_')
#define SFTC_SF5 SHEN_NAME4('S', 'F', '5', '_')
#define SFTC_CHN SHEN_NAME4('C', 'H', 'N', '_')
#define SFTC_STAG SHEN_NAME4('S', 'T', 'A', 'G')
#define SFTC_PACKAGE SHEN_NAME4('Q', '1', '0', '0')
#define SFTC_OMAKE_SCENE SHEN_NAME4('O', 'M', 'A', 'K')
#define SFTC_QRON_SCENE SHEN_NAME4('Q', 'R', '0', '0')
#define SFTC_ACCEPT_HAND_POSE 41

typedef struct SftcSmokeWork {
    int frame;
    StreetFightEventWork *event;
} SftcSmokeWork;

void SftcMain(StreetFightStartParameters *start);
void QueueNormal(StreetFightStartParameters *start);
void LoadNormalResources(StreetFightEventWork *work);
void PlaceNormal(StreetFightEventWork *work);
StreetFightEventWork *InitializeNormal(HLibTask *task);
void NormalDebugCallback(void *data);
void CleanupNormalCallback(void *data);
void MainCallback(void *data);
void RetryBeginCallback(void *data);
void RetryChoiceCallback(void *data);
void RetryResetCallback(void *data);
void RetryCancelCallback(void *data);
int FirstIntroConversation(void *data);
int RepeatIntroConversation(void *data);
int AcceptConversation(void *data);
int DeclineConversation(void *data);
int InsufficientConversation(void *data);
void BattleEntryCallback(void *data);
void BattlePreEventCallback(void *data);
void BattlePreInitCallback(void *data);
void BattleStartCallback(void *data);
void BattleReadyGoCallback(void *data);
void BattleReleaseCallback(void *data);
void BattleWaitCallback(void *data);
void SmokeControlCallback(void *data);
void BattleRingOutCallback(void *data);
void BattleWaitFadeCallback(void *data);
void BattleEndCallback(void *data);
int VictoryConversation(void *data);
int VictoryAppearanceConversation(void *data);
int DefeatConversation(void *data);
int TimeoutConversation(void *data);
void AllocateBattleVoices(StreetFightEventWork *work);
short SelectBattleVoice(unsigned short *voices, unsigned short *index, int count);
int SelectFighterBattleVoice(void *data);
int SelectMotionBattleVoice(void *data);
int SelectForcedBattleVoice(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFTC_BASE{SHEN_NAME(BASE)}, SFTC_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFTC_DEBUG_PACKAGE{SHEN_NAME(QSFD)}, SFTC_RESOURCE_PACKAGE{SHEN_NAME(Q100)};
inline constexpr shen_name SFTC_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFTC_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFTC_STAGE_ORIGIN{137.5f, 100.7249984741211f, 111.56999969482422f};
inline constexpr Vector3 SFTC_DEBUG_OPPONENT_ORIGIN{137.5f, 100.2249984741211f, 111.56999969482422f};
inline constexpr Vector3 SFTC_DEBUG_PLAYER_POSITION{-9.199999809265137f, -.5f, -10.399999618530273f};
inline constexpr Vector3 SFTC_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -.5f, -5.5f};
inline constexpr Vector3 SFTC_DEBUG_EXIT_POSITION{143.1999969482422f, 100.19999694824219f, 115.19999694824219f};
inline constexpr Vector3i SFTC_DEBUG_EXIT_ROTATION{0, 10012, 0};
inline constexpr Vector3 SFTC_DEBUG_CONVERSATION_PLAYER_POSITION{0, -.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFTC_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr std::int8_t SFTC_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr shen_name SFTC_WD2{SHEN_NAME(WD2_)}, SFTC_SF5{SHEN_NAME(SF5_)}, SFTC_CHN{SHEN_NAME(CHN_)}, SFTC_STAG{SHEN_NAME(STAG)};
inline constexpr shen_name SFTC_RETRY_AREA{SHEN_NAME(OMAK)}, SFTC_RETURN_AREA{SHEN_NAME(QR00)};
inline constexpr shen_name SFTC_CHARACTERS[]{STREET_FIGHT_RYO, SFTC_WD2, SFTC_SF5, SFTC_CHN, STREET_FIGHT_INVALID};
inline constexpr std::int32_t SFTC_ENTRY_FEES[]{10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
inline constexpr Vector3 SFTC_FIGHTER_REPEAT_POSITION{-0.4343000054359436f, -0.05000000074505806f, -2.183199882507324f};
inline constexpr Vector3 SFTC_BATTLE_PLAYER_POSITION{0.0f, 0.0f, -1.100000023841858f};
inline constexpr Vector3 SFTC_BATTLE_BOOKIE_POSITION{-3.299999952316284f, -0.5f, -5.5f};
inline constexpr Vector3 SFTC_BATTLE_FIGHTER_POSITION{0.0f, 0.0f, 1.100000023841858f};
inline constexpr Vector3 SFTC_RING_OUT_PLAYER_POSITION{0.5f, 0.0f, -1.100000023841858f};
inline constexpr Vector3 SFTC_RING_OUT_FIGHTER_POSITION{0.0f, 0.0f, -1.350000023841858f};
inline constexpr Vector3 SFTC_EXIT_POSITION{143.1999969482422f, 100.19999694824219f, 115.19999694824219f};
inline constexpr Vector3i SFTC_EXIT_ROTATION{0, 10012, 0};
struct SftcSmokeWork {
    std::int32_t frame;
    std::uint32_t reserved;
    StreetFightEventWork *event;
};
static_assert(sizeof(SftcSmokeWork) == 16);
inline constexpr std::uint16_t SFTC_FIRST_BATTLE_VOICES[]{141}, SFTC_SECOND_BATTLE_VOICES[]{143, 144};
inline constexpr Vector3 SFTC_FIGHTER_VICTORY_POSITION{-0.5879999995231628f, 0.0f, -1.899999976158142f};
inline constexpr Vector3 SFTC_VICTORY_APPEARANCE_POSITION{132.5500030517578f, 100.0f, 106.63999938964844f};
extern const ScriptAreaDefinition areaSftcScriptDefinition;
} // namespace shenmue::s2
#endif
