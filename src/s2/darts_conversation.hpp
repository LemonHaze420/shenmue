#pragma once

#include "common/types.hpp"
#include <cstdint>

namespace shenmue::s2 {
struct ConversationBuffer;
extern std::int32_t dartsPlayerWon;
using DartsResponseCallback = ConversationBuffer *(*)(std::int32_t, bool, shen_name);
extern const DartsResponseCallback dartsResponseCallbacks[30];
ConversationBuffer *ExecuteDartsResponse(std::int32_t index, std::int32_t mode, bool playerTurn, shen_name opponent);
ConversationBuffer *PlayDartsThrowStartResponse(std::int32_t mode, bool playerTurn, shen_name opponent);
void PrepareDartsOpponentAdmission(shen_name opponent);
std::int32_t PlayDartsOpponentAdmission(shen_name opponent);
std::int32_t PlayDartsBetResponse(shen_name opponent, std::int32_t difficulty, std::int32_t bet);
std::int32_t PlayDartsOpponentResult(shen_name opponent, std::int32_t bet);
std::int32_t PlayDartsOpponentExit(shen_name opponent);
} // namespace shenmue::s2
