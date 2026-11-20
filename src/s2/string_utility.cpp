#include "s2/string_utility.hpp"

namespace shenmue { namespace s2 {

// @verified - p, d
bool MatchAreaNamePattern(shen_name area, shen_name pattern) {
    std::uint32_t mask = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8)
        if (((pattern.name >> shift) & 0xffU) != static_cast<std::uint32_t>('?'))
            mask |= 0xffU << shift;
    return (area.name & mask) == (pattern.name & mask);
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

} } // namespace shenmue::s2
