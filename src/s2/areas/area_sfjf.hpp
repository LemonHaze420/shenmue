#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_street_debug.h"

#define SFJF_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define SFJF_AW3 SHEN_NAME4('A', 'W', '3', '_')
#define SFJF_SFA SHEN_NAME4('S', 'F', 'A', '_')
#define SFJF_RE1 SHEN_NAME4('R', 'E', '1', '_')
#define SFJF_CHN SHEN_NAME4('C', 'H', 'N', '_')
#define SFJF_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define SFJF_GUEST SHEN_NAME4('P', '0', '1', 'Q')
#define SFJF_PROP_A428 SHEN_NAME4('a', '4', '2', '8')
#define SFJF_PROP_A534 SHEN_NAME4('a', '5', '3', '4')
#define SFJF_AUTHOR_0 SHEN_NAME4('A', 'U', '_', '0')
#define SFJF_AUTHOR_1 SHEN_NAME4('A', 'U', '_', '1')
#define SFJF_SPECTATOR_P03Q SHEN_NAME4('P', '0', '3', 'Q')
#define SFJF_SPECTATOR_Y01Q SHEN_NAME4('Y', '0', '1', 'Q')
#define SFJF_RETRY_TITLE SHEN_NAME4('R', 'T', 'R', '2')
#define SFJF_OMAKE_SCENE SHEN_NAME4('O', 'M', 'A', 'K')
#define SFJF_QRON_SCENE SHEN_NAME4('Q', 'R', '0', '0')
#define SFJF_INVALID 0xffffffffU

typedef struct SfjfAuthorWork {
    HLibTask *task;
    int reserved04;
    shen_name author;
    short state;
} SfjfAuthorWork;
typedef struct SfjfAuthorLoopWork {
    SfjfAuthorWork *parent;
    short state;
} SfjfAuthorLoopWork;

void SfjfMain(StreetFightStartParameters *start);
void QueueAuthor(shen_name author);
void AuthorMainCallback(void *data);
void StartAuthor(SfjfAuthorWork *work);
void AuthorLoopCallback(void *data);
void BeginAuthor(void);
void EndAuthor(SfjfAuthorWork *work);
void PrepareAuthorCharacters(SfjfAuthorWork *work);
void RestoreAuthorCharacters(void);
void HideAuthorCharacter(shen_name character);
void ShowAuthorCharacter(shen_name character);
void HideAuthorProp(shen_name prop);
void ShowAuthorProp(shen_name prop);
void QueueNormal(StreetFightStartParameters *start);
void LoadNormalResources(StreetFightEventWork *work);
StreetFightEventWork *InitializeNormal(HLibTask *task);
void MainCallback(void *data);
int DefeatPoseConversation(void *data);
void RetryBeginCallback(void *data);
void RetryChoiceCallback(void *data);
void RetryResetCallback(void *data);
void RetryCancelCallback(void *data);
void NormalDebugCallback(void *data);
void CleanupNormalCallback(void *data);
void UnparentAndSetExistingPosRot(shen_name character);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"

namespace shenmue::s2 {
inline constexpr shen_name SFJF_BASE{SHEN_NAME(BASE)}, SFJF_FUY{SHEN_NAME(FUY_)}, SFJF_AW3{SHEN_NAME(AW3_)};
inline constexpr shen_name SFJF_SFA{SHEN_NAME(SFA_)}, SFJF_RE1{SHEN_NAME(RE1_)}, SFJF_CHN{SHEN_NAME(CHN_)};
inline constexpr shen_name SFJF_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFJF_FUY, STREET_FIGHT_INVALID};
inline constexpr shen_name SFJF_CHARACTERS[]{STREET_FIGHT_RYO, SFJF_AW3, SFJF_CHN, SFJF_SFA, SFJF_RE1, STREET_FIGHT_INVALID};
inline constexpr shen_name SFJF_AUTHOR_CHARACTERS[]{SFJF_AW3, SFJF_SFA, SFJF_RE1, SFJF_CHN, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFJF_STAGE_ORIGIN{267.29998779296875f, 86.0999984741211f, 233.10000610351562f};
inline constexpr Vector3 SFJF_DEBUG_OPPONENT_ORIGIN{267.29998779296875f, 85.5999984741211f, 233.10000610351562f};
inline constexpr Vector3 SFJF_DEBUG_PLAYER_POSITION{-9.199999809265137f, -0.5f, -10.399999618530273f};
inline constexpr Vector3 SFJF_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -0.5f, -5.5f};
inline constexpr Vector3 SFJF_DEBUG_EXIT_POSITION{279.2699890136719f, 85.5999984741211f, 235.6699981689453f};
inline constexpr Vector3i SFJF_DEBUG_EXIT_ROTATION{0, 33132, 0};
inline constexpr std::int8_t SFJF_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr Vector3 SFJF_DEBUG_CONVERSATION_PLAYER_POSITION{0, -0.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFJF_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
extern const ScriptAreaDefinition areaSfjfScriptDefinition;
} // namespace shenmue::s2
#endif
