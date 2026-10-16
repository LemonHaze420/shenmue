#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {


inline constexpr std::uint32_t kDebugTextQueueCapacity = 0x8400;

struct DebugTextRecordHeader {
    std::uint32_t column; // SetDebugStringPosition stores the integer column into the float x slot
    std::int32_t line;
    std::uint32_t colour;
    std::uint32_t length;
    float advance;
    float lineHeight;
    float glyphWidth;
    float glyphHeight;
};

static_assert(sizeof(DebugTextRecordHeader) == 0x20);
static_assert(offsetof(DebugTextRecordHeader, length) == 0x0c);
static_assert(offsetof(DebugTextRecordHeader, advance) == 0x10);

void SetDebugStringPosition(std::int32_t column, std::int32_t line);
void DebugTextPrintf(const char *format, ...);
void ClearQueuedDebugText();
#ifdef _DEBUG
void DrawQueuedDebugText();
#endif

} // namespace shenmue::s2
