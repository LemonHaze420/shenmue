#pragma once

#include "s2/event_runtime.hpp"

#include <array>

namespace shenmue::s2 {

extern std::uint32_t dartsEventActive;

inline constexpr shen_name DART_COMPACT_BOARD{SHEN_NAME(DAR3)};
inline constexpr char DART_EVENT_SLOT[] = "DART.EV3";
inline constexpr std::array<float, 4> DART_LARGE_SCORE_Y{2.1549999713897705f, 2.005000114440918f, 1.8550000190734863f, 1.7050000429153442f};
inline constexpr std::array<float, 4> DART_SMALL_SCORE_Y{2.138000011444092f, 1.9880000352859497f, 1.8380000591278076f, 1.687999963760376f};
inline constexpr Vector3 DART_LARGE_DIGIT_STEP{-0.05000000074505806f, 0.0f, 0.0f};
inline constexpr Vector3 DART_SMALL_DIGIT_STEP{-0.02500000037252903f, 0.0f, 0.0f};
inline constexpr std::array<float, 4> DART_COMPACT_DIGIT_STEP{-0.04740000143647194f, -0.04740000143647194f, 0.0f, -0.10610000044107437f};
inline constexpr std::array<Vector3, 4> DART_COMPACT_SCORE_POSITION{{
    {0.19050000607967377f, 1.7258000373840332f, -0.03880000114440918f},
    {-0.05849999934434891f, 1.7258000373840332f, -0.03880000114440918f},
    {0.0f, 0.0f, 0.0f},
    {0.13439999520778656f, 1.909999966621399f, -0.03880000114440918f},
}};
inline constexpr std::array<Vector3, 4> DART_COMPACT_SCORE_SCALE{{
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, 0.0f},
    {1.7999999523162842f, 1.600000023841858f, 1.0f},
}};
inline constexpr std::array<std::int8_t, 21> DART_SECTOR_ORDER{20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, -1};
inline constexpr float DART_INNER_BULL_RADIUS = 0.012000000104308128f;
inline constexpr float DART_TRIPLE_MINIMUM_RADIUS = 0.08799999952316284f;
inline constexpr float DART_TRIPLE_MAXIMUM_RADIUS = 0.10400000214576721f;
inline constexpr float DART_BOARD_Y_SCALE = 1.0072135925292969f;


inline constexpr std::array<std::uint32_t, 18> DART_SAVED_SCORE_WORDS{SHEN_NAME(FCJ_), 16, 17, SHEN_NAME(HCY_), 18, 19, SHEN_NAME(HLN_), 20, 21,
    SHEN_NAME(JON_), 22, 23, SHEN_NAME(XHO_), 24, 25, 1048576000, 1044583023, 1071441668};
void RestoreOpponentDartsScores(shen_name character, std::int32_t *bestScore, std::int32_t *previousScore);

std::int64_t CalculateDartsPositionOrScore(std::int8_t score, Vector3 *position);

void UpdateDartsScoreDisplay(shen_name character, std::int32_t score, std::int32_t mode);
void UpdateDartsCompactScoreDisplay(shen_name character, std::uint32_t score, std::int32_t mode);

} // namespace shenmue::s2
