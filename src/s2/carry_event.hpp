#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name CARRY_EVENT_PLAYER{SHEN_NAME(RYO_)}, CARRY_EVENT_REN{SHEN_NAME(REN_)};
inline constexpr shen_name CARRY_EVENT_STAGE{SHEN_NAME(STRB)};
inline constexpr std::uint16_t CARRY_TRANSITION_MOTIONS[3][2]{{8674, 8671}, {8675, 8672}, {8673, 8670}};
inline constexpr Vector3 CARRY_TRANSITION_REN_POSITIONS[]{
    {-0.6711300015449524f, 0, 1.848620057106018f}, {0.5476700067520142f, 0, 1.2676700353622437f}, {-0.3136399984359741f, 0, 0.7247999906539917f}};
inline constexpr Vector3 CARRY_TRANSITION_TAIL_REN_POSITIONS[]{
    {-0.7711300253868103f, 0, 1.848620057106018f}, {0.5476700067520142f, 0, 1.2676700353622437f}, {-0.3136399984359741f, 0, 0.7247999906539917f}};
inline constexpr float CARRY_TRANSITION_TAIL_START_FRAMES[]{120, 100, 200};
inline constexpr std::uint16_t CARRY_TRANSITION_CAMERAS[3][5]{{7010, 7012, 7015, 7015, 7015}, {7000, 7001, 7002, 7002, 7002}, {7005, 7006, 7007, 7007, 7007}};
inline constexpr std::int16_t CARRY_TRANSITION_CAMERA_DELAYS[3][5]{{0, 170, 417, 417, 417}, {0, 60, 180, 180, 180}, {0, 200, 340, 340, 340}};
inline constexpr const char *CARRY_TRANSITION_VOICES[3][5]{
    {"0301A005","0301B004","0301B005","0301A004","0301B006"}, // @text 0301B004 EN: "Oh no!" | JA: "…やべっ！" ; 0301B005 EN: "How's it look?" | JA: "どうだ？" ; 0301A004 EN: "No.\nThere's too many of them." | JA: "…ここはダメだ。\n数が多すぎる。" ; 0301B006 EN: "We'll take a different course." | JA: "しょうがねえ、\n他に回るぜ。"
    {"0301A005","0301B007","0301B008","0301A006","0301B009"}, // @text 0301B008 EN: "Here too?!" | JA: "ここもかよ！" ; 0301A006 EN: "Yeah=@\nI don't think we can go unnoticed." | JA: "ああ…とても通れそうにないな。" ; 0301B009 EN: "What a pain!\nCome on, let's go!" | JA: "…うざってえ！\nおい、行くぞ。"
    {"0301A007","0301B010","0301B011","0301A008","0301B012"}}; // @text 0301A007 EN: "Damn=@" | JA: "くそっ…" ; 0301B010 EN: "Crap!" | JA: "ちくしょう！" ; 0301B011 EN: "What's going on?!" | JA: "どうなってんだ！" ; 0301A008 EN: "We're surrounded by enemies." | JA: "どこも敵だらけってことらしい。"
inline constexpr std::int16_t CARRY_TRANSITION_VOICE_DELAYS[3][3][5]{{{27, 55, 189, 223, 342}, {27, 55, 132, 173, 314}, {87, 104, 298, 350, 498}},
    {{27, 55, 164, 220, 302}, {27, 56, 129, 178, 319}, {87, 104, 282, 350, 450}}, {{63, 98, 164, 220, 310}, {49, 80, 129, 190, 299}, {95, 146, 294, 350, 485}}};
inline constexpr std::uint16_t CARRY_TRANSITION_TAIL_CAMERAS[3][3]{{7011, 7012, 7015}, {7003, 7002, 7002}, {7008, 7009, 7009}};
inline constexpr std::int16_t CARRY_TRANSITION_TAIL_CAMERA_DELAYS[3][3]{{0, 85, 297}, {0, 150, 150}, {0, 200, 200}};
inline constexpr const char *CARRY_TRANSITION_TAIL_VOICES[3][3]{
    {"0301B014","0301A009","0301B015"},{"0301B014","0301A009","0301B015"},{"0301B014","0301A009","0301B015"}}; // @text 0301B014 EN: "They're here again." | JA: "まだ、いやがるぜ。" ; 0301A009 EN: "I don't think they're going anywhere." | JA: "当分、動きそうにないな。" ; 0301B015 EN: "So we take a different course." | JA: "やっぱここはダメだ。他へ回るぜ！"
inline constexpr std::int32_t CARRY_TRANSITION_TAIL_VOICE_DELAYS[3][3]{{43, 103, 194}, {22, 86, 194}, {67, 150, 263}};
std::uint32_t PlayPlankCarryAuthoredTransition(std::int32_t kind, std::int32_t dialogue);
std::uint32_t PlayPlankCarryAuthoredTransitionTail(std::int32_t kind, std::int32_t dialogue);
} // namespace shenmue::s2
