#pragma once

#include <array>
#include <cstdint>
#include "common/types.hpp"

namespace shenmue::s2 {

extern std::array<std::uint32_t, 5> g_conversationIndexBRecord;
extern std::array<std::uint32_t, 5> g_conversationIndexCRecord;
extern std::array<std::uint32_t, 5> g_conversationIndexDRecord;
static_assert(sizeof(std::array<std::uint32_t, 5>) == 20);
static_assert(alignof(std::array<std::uint32_t, 5>) == 4);

inline constexpr std::array<std::array<std::uint32_t, 7>, 3> CONVERSATION_JAPANESE_CODE_REMAPS{{
    {SHEN_NAME(Q237), SHEN_NAME(9A08), '0', SHEN_NAME(Q237), SHEN_NAME(9A06), '2', 1},
    {SHEN_NAME(Q241), SHEN_NAME(2A06), '3', SHEN_NAME(Q241), SHEN_NAME(2A04), '8', 1},
    {SHEN_NAME(H216), SHEN_NAME(8B07), '1', SHEN_NAME(H216), SHEN_NAME(8B02), '7', 0},
}};
static_assert(sizeof(CONVERSATION_JAPANESE_CODE_REMAPS) == 84);

inline constexpr std::array<std::int32_t, 54> CONVERSATION_SILENT_DURATIONS_FIRST{77, 60, 51, 18, 33, 31, 47, 16, 96, 103, 51, 20, 156, 72, 20, 79, 135, 41,
    152, 168, 16, 13, 156, 35, 21, 18, 16, 16, 12, 12, 21, 10, 14, 37, 27, 16, 86, 76, 43, 110, 114, 45, 102, 30, 37, 25, 34, 16, 23, 33, 162, 92, 31, 0};
inline constexpr std::array<std::int32_t, 10> CONVERSATION_SILENT_DURATIONS_SECOND{20, 31, 33, 18, 15, 5, 46, 0, 0, 0};
inline constexpr std::array<std::int32_t, 16> CONVERSATION_SILENT_DURATIONS_THIRD{43, 70, 36, 56, 153, 104, 146, 200, 166, 164, 153, 70, 89, 28, 87, 62};
inline constexpr std::array<const std::int32_t *, 3> CONVERSATION_SILENT_DURATIONS{
    CONVERSATION_SILENT_DURATIONS_FIRST.data(), CONVERSATION_SILENT_DURATIONS_SECOND.data(), CONVERSATION_SILENT_DURATIONS_THIRD.data()};

} // namespace shenmue::s2
