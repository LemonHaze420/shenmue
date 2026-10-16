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
