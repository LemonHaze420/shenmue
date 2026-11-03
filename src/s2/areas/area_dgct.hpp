#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_exqt.hpp"

#define DGCT_AREA SHEN_NAME4('D', 'G', 'C', 'T')
#define DGCT_RETURN SHEN_NAME4('W', 'S', '0', '0')
typedef struct GameCenterWork {
    HLibTask *task;
    int reserved04;
    unsigned short state;
    unsigned short reserved0a;
    int reserved0c[4];
    ExciteState game;
} GameCenterWork;
void RunDgct(void);
void InitializeGameCenterWork(GameCenterWork *work, HLibTask *task);
void LeaveGameCenter(void);
void ReturnFromGameCenter(void);
void GameCenterMain(void *data);
int IsGameCenterButtonPressed(void);
int IsPlayerLeavingGameCenter(void);
void InitializeGameCenterQte(ExciteState *state);
void ResetGameCenterGame(ExciteState *state);
void ExitGameCenterQte(ExciteState *state);
int UpdateGameCenterQte(ExciteState *state);
void BuildGameCenterInput(ExciteState *state);
int DecrementGameCenterLives(ExciteState *state);
void SetExciteTiming(ExciteState *state);
void SetExciteWindow(ExciteState *state, float patternScale, float timeScale);
void UpdateGameCenterScore(ExciteState *state);
void UpdateGameCenterTimingBonus(ExciteState *state);
void AccumulateGameCenterBonus(ExciteState *state);
void ResetExciteMachineDisplay(void);
void HideExciteMachineParts(void);
void SetExciteMachinePart(int part, int enabled);
void UpdateExciteLives(ExciteState *state);
void UpdateGameCenterDisplay(ExciteState *state);
void DrawGameCenterWindow(ExciteState *state);
void DrawGameCenterBonus(ExciteState *state);
void InitializeExciteMachineParts(void);
int AdvanceGameCenterWait(ExciteState *state);
void StartGameCenterIntro(void);
void StartGameCenterPaidIntro(void);
void GameCenterLostLife(int final);
void QueueExciteResultDisplay(ExciteState *state);
void ExciteHighScoreReaction(void);
void FinishGameCenterQte(void);
void QueueExcitePart(int part, int enabled, int delay);
void ExcitePartCallback(void *data);
void ExciteScoreCallback(void *data);
void QueueExciteScoreFlash(ExciteState *state, signed char which, int delay, int ticks);
void DrawExciteScore(ExciteState *state, signed char which);
#else
#pragma once
#include "s2/areas/area_exqt.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name DGCT_AREA{SHEN_NAME(DGCT)}, DGCT_RETURN{SHEN_NAME(CB00) | 0x1114U};
struct GameCenterWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    std::uint16_t state;
    std::byte reserved0e[18];
    ExciteState game;
};
static_assert(sizeof(GameCenterWork) == 0xa8 && offsetof(GameCenterWork, game) == 0x20 && offsetof(GameCenterWork, state) == 0x0c);
void InitializeDgct();
void RunDgct(std::uintptr_t);
void GameCenterMain(void *);
bool UpdateGameCenterQte(ExciteState &);
void InitializeGameCenterQte(ExciteState &);
void BuildGameCenterInput(ExciteState &);
bool PollGameCenterInput(ExciteInput &);
bool ConsumeGameCenterInput(ExciteInput &);
void UpdateGameCenterDisplay(const ExciteState &);
void DrawGameCenterWindow(const ExciteState &);
void DrawGameCenterBonus(const ExciteState &);
void StartGameCenterIntro();
void StartGameCenterPaidIntro();
void GameCenterLostLife(bool);
void FinishGameCenterQte();
extern const ScriptAreaDefinition dgctScriptDefinition;
} // namespace shenmue::s2::areas
#endif
