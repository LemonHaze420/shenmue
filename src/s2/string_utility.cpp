#include "s2/string_utility.hpp"

namespace shenmue {
namespace s2 {

// @verified - p, d
#if SHENMUE_S2_GCC295
std::int32_t MatchAreaNamePattern(shen_name area, shen_name pattern) {
#else
bool MatchAreaNamePattern(shen_name area, shen_name pattern) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::uint32_t name = static_cast<std::uint32_t>(pattern);
#endif
    std::uint32_t mask = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if ((name & 0xffU) != '?')
        mask |= 0xffU;
    if (((name >> 8) & 0xffU) != '?')
        mask |= 0xff00U;
    if (((name >> 16) & 0xffU) != '?')
        mask |= 0xff0000U;
    if ((name >> 24) != '?')
        mask |= 0xff000000U;
    return (static_cast<std::uint32_t>(area) & mask) == (name & mask);
#else
    for (std::uint32_t shift = 0; shift < 32; shift += 8)
        if (((pattern.name >> shift) & 0xffU) != static_cast<std::uint32_t>('?'))
            mask |= 0xffU << shift;
    return (area.name & mask) == (pattern.name & mask);
#endif
}

std::int32_t CompareTokenIgnoreCase(const char *left, const char *right) {
    for (;;) {
        std::int32_t leftCharacter = static_cast<std::int8_t>(*left++);
        std::int32_t rightCharacter = static_cast<std::int8_t>(*right++);
        if (static_cast<std::uint32_t>(leftCharacter - 'a') <= 25U)
            leftCharacter -= ' ';
        if (static_cast<std::uint32_t>(rightCharacter - 'a') <= 25U)
            rightCharacter -= ' ';
        if (leftCharacter > rightCharacter)
            return 1;
        if (leftCharacter < rightCharacter)
            return -1;
        if (leftCharacter == 0)
            return 0;
    }
}

} // namespace s2
} // namespace shenmue
