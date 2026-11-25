#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/arcade.hpp"

namespace shenmue {
namespace s2 {

static const std::uint32_t rankingScoreWeights[8] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
extern ArcadeRankingRecord outRunRankingStorage[20], afterBurnerRankingStorage[20], hangOnRankingStorage[20], harrierRankingStorage[20];
extern ArcadeRankingRecord *g_outRunRankings, *g_afterBurnerRankings, *g_hangOnRankings, *g_harrierRankings;
std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits);
void ResetHangOnRankings();
void ResetSpaceHarrierRankings();
void ResetOutRunRankings();
void ResetAfterBurnerRankings();
void ResetArcadeRankings();

} // namespace s2
} // namespace shenmue

#endif
