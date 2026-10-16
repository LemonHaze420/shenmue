#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFTS_BASE{SHEN_NAME(BASE)}, SFTS_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFTS_DEBUG_PACKAGE{SHEN_NAME(QSFE)}, SFTS_RESOURCE_PACKAGE{SHEN_NAME(Q200)};
inline constexpr shen_name SFTS_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFTS_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFTS_STAGE_ORIGIN{203.16969299316406f, 100.5f, 246.56700134277344f};
inline constexpr Vector3 SFTS_DEBUG_OPPONENT_ORIGIN{203.16969299316406f, 100.0f, 246.56700134277344f};
inline constexpr Vector3 SFTS_DEBUG_PLAYER_POSITION{-9.199999809265137f, -.5f, -10.399999618530273f};
inline constexpr Vector3 SFTS_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -.5f, -5.5f};
inline constexpr Vector3 SFTS_DEBUG_EXIT_POSITION{201.19000244140625f, 100.0f, 241.55999755859375f};
inline constexpr Vector3i SFTS_DEBUG_EXIT_ROTATION{0, 34661, 0};
inline constexpr Vector3 SFTS_DEBUG_CONVERSATION_PLAYER_POSITION{0, -.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFTS_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr std::int8_t SFTS_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr Vector3 SFTS_INTRO_FIGHTER_POSITION{-0.2879999876022339f, 0.0f, -0.20000000298023224f};
inline constexpr Vector3 SFTS_ACCEPT_FIGHTER_POSITION{-0.019999999552965164f, 0.0f, -1.100000023841858f};
inline constexpr Vector3 SFTS_REPEAT_PLAYER_POSITION{-2.619999885559082f, -0.5f, -3.759999990463257f};
inline constexpr Vector3 SFTS_BATTLE_BOOKIE_POSITION{-3.299999952316284f, -0.5f, -5.5f};
inline constexpr Vector3 SFTS_FIGHTER_INTRO_POSITION{-1.2879999876022339f, 0.0f, -2.200000047683716f};
inline constexpr Vector3 SFTS_VICTORY_FIGHTER_POSITION{-0.2879999876022339f, 0.0f, -1.7000000476837158f};
inline constexpr Vector3 SFTS_DEFEAT_FIGHTER_POSITION{-0.2879999876022339f, 0.0f, -1.2000000476837158f};
inline constexpr shen_name SFTS_VCN{SHEN_NAME(VCN_)}, SFTS_KFT{SHEN_NAME(KFT_)}, SFTS_CHN{SHEN_NAME(CHN_)}, SFTS_NORMAL_PACKAGE{SHEN_NAME(Q100)};
inline constexpr shen_name SFTS_RETRY_AREA{SHEN_NAME(OMAK)}, SFTS_RETURN_AREA{SHEN_NAME(QR00)};
inline constexpr shen_name SFTS_CHARACTERS[]{STREET_FIGHT_RYO, SFTS_VCN, SFTS_KFT, SFTS_CHN, STREET_FIGHT_INVALID};
inline constexpr std::int32_t SFTS_ENTRY_FEES[]{10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
inline constexpr std::int16_t SFTS_QTE_COMMANDS[16][20]{
    {10, 7, 45, 0, 512, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 44, 0, 512, 0, 1, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 43, 0, 512, 0, 2048, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 42, 0, 512, 0, 2048, 0, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 41, 0, 512, 0, 16, 0, 256, 0, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 40, 0, 512, 0, 1, 0, 16, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 6, 39, 0, 512, 0, 2048, 0, 16, 0, 256, 0, 1024, 0, 0, 0, 0, 0, 0, 0},
    {10, 6, 38, 0, 512, 0, 4, 0, 16, 0, 1, 0, 1024, 0, 0, 0, 0, 0, 0, 0},
    {10, 6, 37, 0, 512, 0, 4, 0, 512, 0, 4, 0, 64, 0, 0, 0, 0, 0, 0, 0},
    {10, 6, 36, 0, 512, 0, 1024, 0, 256, 0, 2048, 0, 4, 0, 0, 0, 0, 0, 0, 0},
    {10, 6, 35, 0, 512, 0, 64, 0, 2048, 0, 4, 0, 256, 0, 1024, 0, 0, 0, 0, 0},
    {10, 6, 34, 0, 512, 0, 64, 0, 512, 0, 1, 0, 2048, 0, 1024, 0, 0, 0, 0, 0},
    {10, 5, 33, 0, 512, 0, 16, 0, 256, 0, 1, 0, 1024, 0, 2048, 0, 16, 0, 0, 0},
    {10, 5, 32, 0, 512, 0, 256, 0, 1, 0, 4, 0, 1024, 0, 64, 0, 512, 0, 0, 0},
    {10, 5, 31, 0, 512, 0, 256, 0, 1, 0, 64, 0, 2048, 0, 1024, 0, 4, 0, 16, 0},
    {10, 5, 30, 0, 512, 0, 64, 0, 1024, 0, 4, 0, 2048, 0, 16, 0, 256, 0, 1, 0},
};
inline std::int32_t SFTS_SUMO_HAND[19][3]{
    {0, 4368, 50431},
    {0, 1820, 61715},
    {0, 60258, 2365},
    {0, 64990, 0},
    {0, 182, 63898},
    {0, 64809, 63535},
    {0, 64990, 909},
    {0, 0, 0},
    {0, 0, 63716},
    {0, 64627, 64809},
    {0, 0, 546},
    {0, 364, 0},
    {0, 65354, 62989},
    {0, 182, 0},
    {0, 0, 182},
    {0, 364, 65172},
    {0, 0, 64445},
    {0, 1456, 64080},
    {0, 0, 0},
};
inline std::int32_t SFTS_CLOSED_HAND[19][3]{
    {1999, 65354, 62444},
    {1638, 180, 63900},
    {546, 64719, 3365},
    {0, 64810, 60984},
    {0, 64628, 61893},
    {0, 0, 64261},
    {0, 0, 64628},
    {0, 0, 60074},
    {0, 0, 59710},
    {0, 65172, 63353},
    {0, 0, 65354},
    {0, 729, 61165},
    {0, 65173, 58073},
    {0, 65172, 64444},
    {0, 0, 0},
    {0, 547, 57161},
    {0, 1639, 61165},
    {0, 64810, 64080},
    {64080, 182, 0},
};
inline constexpr shen_name SFTS_BOOKIE_VOICE{SHEN_NAME(dumA)}, SFTS_FIGHTER_VOICE{SHEN_NAME(dumP)};
inline constexpr Vector3 SFTS_CHEST_PLAYER_POSITION{-3.2100000381469727f, 0.0f, -1.649999976158142f};
inline constexpr Vector3 SFTS_CHEST_FIGHTER_POSITION{0.36000001430511475f, 0.0f, 1.7000000476837158f};
extern const ScriptAreaDefinition areaSftsScriptDefinition;
} // namespace shenmue::s2
