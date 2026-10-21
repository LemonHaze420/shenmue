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
inline constexpr char SFKK_DEBUG_PLAYER_AGREE[] = "\x97\xc1\x81\x75\x82\xa0\x82\xa0\x81\x42\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_WAIT[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xe6\x82\xb5\x91\xd2\x82\xc1\x82\xc4\x82\xc8\x81\x42\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_INTRODUCE[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xb3\x82\xa0\x82\xb3\x82\xa0\x81\x41\x82\xb1\x82\xcc\x8c\x5a\x82\xbf"
                                                      "\x82\xe1\x82\xf1\x81\x41\x81\x95\x82\xdc\x82\xbe\x8e\xe1\x82\xa6\x82\xaa\x81\x41\x82\xaf\x82\xc1"
                                                      "\x82\xb1\x82\xa4\x82\xc8\x83\x63\x83\x8f\x83\x82\x83\x6d\x82\xbe\x81\x42\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_CHALLENGER[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xb3\x82\xa0\x92\x4e\x82\xa9\x81\x41\x82\xb1\x82\xa2\x82\xc2\x82\xc6"
                                                       "\x8f\x9f\x95\x89\x82\xb7\x82\xe9\x82\xe2\x82\xc2\x82\xcd\x82\xa2\x82\xc8\x82\xa2\x82\xa9\x81\x48"
                                                       "\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_ANYONE[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xb3\x82\xa0\x92\x4e\x82\xa9\x81\x49\x81\x49\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_NO_CHALLENGER[] = "\x93\xb7\x8c\xb3\x81\x75\x81\x45\x81\x45\x81\x45\x82\xc1\x82\xc6\x81\x41\x82\xa2\x82\xcb\x82\xa6"
                                                          "\x82\xe6\x82\xa4\x82\xbe\x82\xc8\x81\x42\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_PRIZE[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xd9\x82\xe7\x81\x41\x8c\x5a\x82\xbf\x82\xe1\x82\xf1\x81\x41\x8f\xdc"
                                                  "\x8b\xe0\x82\xbe\x81\x42\x81\x76";
inline constexpr char SFKK_DEBUG_PLAYER_STOP[] = "\x97\xc1\x81\x75\x82\xa2\x82\xe2\x81\x41\x82\xe0\x82\xa4\x82\xe6\x82\xb5\x82\xc6\x82\xad\x82\xe6"
                                                 "\x81\x42\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_ACCEPT_STOP[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xbb\x82\xa4\x82\xa9\x81\x45\x81\x45\x81\x45\x81\x42\x81\x76";
inline constexpr char SFKK_DEBUG_BOOKIE_STOP_PRIZE[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xb5\x82\xe1\x82\xa0\x82\xcb\x82\xa6\x81\x41\x82\xd9\x82\xe7\x81\x41"
                                                       "\x8f\xdc\x8b\xe0\x82\xbe\x81\x42\x81\x76";
extern const ScriptAreaDefinition areaSfkkScriptDefinition;
} // namespace shenmue::s2
