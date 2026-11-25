#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {

struct ConditionalMemoLine {
    std::int32_t flags[2];
    const char *text[2];
    const char *ruby[2];
};
typedef char Layout_memo_flags_dc_0[(sizeof(ConditionalMemoLine) == 24) ? 1 : -1];
typedef char Layout_memo_flags_dc_1[(offsetof(ConditionalMemoLine, text) == 8) ? 1 : -1];
typedef char Layout_memo_flags_dc_2[(offsetof(ConditionalMemoLine, ruby) == 16) ? 1 : -1];

extern const std::int16_t memoLineFlags[146][10];

extern const ConditionalMemoLine conditionalMemoLines[14];

} } // namespace shenmue::s2

#endif
