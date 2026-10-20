#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFS2_BASE{SHEN_NAME(BASE)}, SFS2_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFS2_DEBUG_PACKAGE{SHEN_NAME(QSFB)}, SFS2_RESOURCE_PACKAGE{SHEN_NAME(Q200)};
inline constexpr shen_name SFS2_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFS2_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFS2_STAGE_ORIGIN{334.5f, 84.25f, 80.72000122070312f};
inline constexpr Vector3 SFS2_DEBUG_OPPONENT_ORIGIN{334.5f, 83.75f, 80.72000122070312f};
inline constexpr Vector3 SFS2_DEBUG_PLAYER_POSITION{-9.199999809265137f, -.5f, -10.399999618530273f};
inline constexpr Vector3 SFS2_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, 0, -5.5f};
inline constexpr Vector3 SFS2_DEBUG_BATTLE_ENTRY_BOOKIE_POSITION{.x = -3.299999952316284f, .y = -0.5f, .z = -5.5f};
inline constexpr Vector3 SFS2_DEBUG_EXIT_POSITION{331.0899963378906f, 84.25f, 84.45999908447266f};
inline constexpr Vector3i SFS2_DEBUG_EXIT_ROTATION{0, 54613, 0};
inline constexpr Vector3 SFS2_DEBUG_CONVERSATION_PLAYER_POSITION{0, -.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFS2_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr Vector3 SFS2_FIGHTER_INITIAL_POSITION{-0.5f, 0.0f, -1.0f};
inline constexpr Vector3 SFS2_FIGHTER_INTRO_POSITION{-0.3400000035762787f, 0.5f, -0.33000001311302185f};
inline constexpr Vector3 SFS2_FIGHTER_REPEAT_POSITION{-1.399999976158142f, 0.5f, -1.5f};
inline constexpr Vector3 SFS2_BATTLE_PLAYER_POSITION{0.0f, 0.5f, -1.100000023841858f};
inline constexpr Vector3 SFS2_BATTLE_BOOKIE_POSITION{-3.799999952316284f, 0.0f, -2.4000000953674316f};
inline constexpr Vector3 SFS2_BATTLE_FIGHTER_POSITION{0.0f, 0.5f, 1.100000023841858f};
inline constexpr shen_name SFS2_CYC{SHEN_NAME(CYC_)}, SFS2_VNE{SHEN_NAME(VNE_)}, SFS2_STAG{SHEN_NAME(STAG)}, SFS2_CHN{SHEN_NAME(CHN_)};
inline constexpr shen_name SFS2_BTC1{SHEN_NAME(BTC1)}, SFS2_GRID_ORIGIN{SHEN_NAME(0000)};
inline constexpr shen_name SFS2_BOOKIE_VOICE{SHEN_NAME(dumA)}, SFS2_FIGHTER_VOICE{SHEN_NAME(dumG)};
inline constexpr shen_name SFS2_CHARACTERS[]{STREET_FIGHT_RYO, SFS2_CYC, SFS2_VNE, SFS2_CHN, STREET_FIGHT_INVALID};
inline constexpr std::int32_t SFS2_ENTRY_FEES[]{10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
inline constexpr std::int16_t SFS2_QTE_COMMANDS[16][20]{
    {10, 7, 45, 0, 4, 0, 4, 0, 512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 44, 0, 4, 0, 4, 0, 512, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 43, 0, 4, 0, 4, 0, 512, 0, 256, 0, 4, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 42, 0, 4, 0, 4, 0, 512, 0, 1024, 0, 256, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 41, 0, 4, 0, 4, 0, 512, 0, 16, 0, 512, 0, 0, 0, 0, 0, 0, 0},
    {10, 7, 40, 0, 4, 0, 4, 0, 512, 0, 64, 0, 4, 0, 0, 0, 0, 0, 0, 0},
    {10, 6, 39, 0, 4, 0, 4, 0, 512, 0, 2048, 0, 1024, 0, 0, 0, 0, 0, 0, 0},
    {10, 6, 38, 0, 4, 0, 4, 0, 512, 0, 256, 0, 1024, 0, 2048, 0, 0, 0, 0, 0},
    {10, 6, 37, 0, 4, 0, 4, 0, 512, 0, 1024, 0, 2048, 0, 256, 0, 0, 0, 0, 0},
    {10, 6, 36, 0, 4, 0, 4, 0, 512, 0, 256, 0, 2048, 0, 16, 0, 256, 0, 0, 0},
    {10, 6, 35, 0, 4, 0, 4, 0, 512, 0, 1024, 0, 2048, 0, 1024, 0, 2048, 0, 0, 0},
    {10, 6, 34, 0, 4, 0, 4, 0, 512, 0, 16, 0, 2048, 0, 64, 0, 256, 0, 0, 0},
    {10, 5, 33, 0, 4, 0, 4, 0, 512, 0, 1, 0, 64, 0, 16, 0, 4, 0, 0, 0},
    {10, 5, 32, 0, 4, 0, 4, 0, 512, 0, 64, 0, 2048, 0, 512, 0, 4, 0, 0, 0},
    {10, 5, 31, 0, 4, 0, 4, 0, 512, 0, 1, 0, 1024, 0, 4, 0, 256, 0, 0, 0},
    {10, 5, 30, 0, 4, 0, 4, 0, 512, 0, 16, 0, 1024, 0, 1, 0, 2048, 0, 256, 0},
};
inline constexpr Vector3 SFS2_EXIT_POSITION{331.0899963378906f, 84.25f, 84.45999908447266f};
inline constexpr Vector3i SFS2_EXIT_ROTATION{0, 54613, 0};
inline constexpr std::int8_t SFS2_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr shen_name SFS2_RETRY_AREA{SHEN_NAME(OMAK)}, SFS2_RETURN_AREA{SHEN_NAME(QR00)};
inline constexpr Vector3 SFS2_VICTORY_APPEARANCE_POSITION{335.8699951171875f, 84.25f, 85.83000183105469f};
extern const ScriptAreaDefinition areaSfs2ScriptDefinition;
} // namespace shenmue::s2
