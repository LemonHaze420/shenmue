#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFKK_BASE{SHEN_NAME(BASE)}, SFKK_FUY{SHEN_NAME(FUY_)}, SFKK_SD6{SHEN_NAME(SD6_)}, SFKK_OXO{SHEN_NAME(OXO_)},
    SFKK_CHN{SHEN_NAME(CHN_)};
inline constexpr shen_name SFKK_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFKK_FUY, STREET_FIGHT_INVALID};
inline constexpr shen_name SFKK_CHARACTERS[]{STREET_FIGHT_RYO, SFKK_SD6, SFKK_OXO, SFKK_CHN, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFKK_STAGE_ORIGIN{197, 100.5f, 129};
inline constexpr Vector3 SFKK_DEBUG_OPPONENT_ORIGIN{197, 100, 129};
inline constexpr Vector3 SFKK_DEBUG_PLAYER_POSITION{-9.199999809265137f, -0.5f, -10.399999618530273f};
inline constexpr Vector3 SFKK_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -0.5f, -5.5f};
inline constexpr Vector3 SFKK_DEBUG_EXIT_POSITION{190.3000030517578f, 100, 129.6999969482422f};
inline constexpr Vector3i SFKK_DEBUG_EXIT_ROTATION{0, 49152, 0};
inline constexpr std::int8_t SFKK_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr Vector3 SFKK_DEBUG_CONVERSATION_PLAYER_POSITION{0, -0.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFKK_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
extern const ScriptAreaDefinition areaSfkkScriptDefinition;
} // namespace shenmue::s2
