#include "s2/debug_text.hpp"

#include "d3t/buffer.hpp"
#ifdef _DEBUG
#include "shenmue_internal.h"
#include "d3t/debug_font.hpp"
#endif

#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {

namespace {

// ShenmueInit leaves unit scales, column 0.0f, line 0 and white before any producer runs.
std::array<char, 0x8404> debugTextQueue{};
std::uint32_t debugTextCount{};
float debugTextWidthScale = 1.0f;
float debugTextHeightScale = 1.0f;
std::uint32_t debugTextColumn{};
std::int32_t debugTextLine{};
std::uint32_t debugTextColour = 0xffffffffU;
std::uint32_t debugTextUsed{};

} // namespace

void SetDebugStringPosition(std::int32_t column, std::int32_t line) {
    debugTextColumn = static_cast<std::uint32_t>(column);
    debugTextLine = line;
}

void DebugTextPrintf(const char *format, ...) {
    std::array<char, 0x400> buffer;
    va_list arguments;
    va_start(arguments, format);
    std::vsnprintf(buffer.data(), 0x3fe, format, arguments);
    va_end(arguments);
    buffer[0x3ff] = '\0';
    const std::size_t length = std::strlen(buffer.data());
    if (!length || debugTextUsed + length + 35 >= kDebugTextQueueCapacity)
        return;

    std::int32_t line = debugTextLine;
    for (std::size_t index = 0; index < length; ++index)
        if (buffer[index] == '\n')
            ++line;
    const d3t::D3TRenderState &state = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext);
    const float widthScale = static_cast<float>(static_cast<std::int32_t>(state.viewport[2])) / 640.0f * debugTextWidthScale;
    const float heightScale = static_cast<float>(static_cast<std::int32_t>(state.viewport[3])) / 480.0f * debugTextHeightScale;
    const DebugTextRecordHeader header{.column = debugTextColumn,
        .line = debugTextLine,
        .colour = debugTextColour,
        .length = static_cast<std::uint32_t>(length),
        .advance = widthScale * 9.0f,
        .lineHeight = heightScale * 18.0f,
        .glyphWidth = widthScale * 10.0f,
        .glyphHeight = heightScale * 16.0f};
    std::memcpy(debugTextQueue.data() + debugTextUsed, &header, sizeof(header));
    std::memcpy(debugTextQueue.data() + debugTextUsed + sizeof(header), buffer.data(), length);
    debugTextLine = line;
    ++debugTextCount;
    debugTextUsed += static_cast<std::uint32_t>(length + 35);
}

void ClearQueuedDebugText() {
    if (!debugTextCount)
        return;
    debugTextCount = 0;
    debugTextUsed = 0;
}

#ifdef _DEBUG
void DrawQueuedDebugText() {
    const std::uint32_t column = debugTextColumn;
    const std::int32_t line = debugTextLine;
    SetDebugStringPosition(1, 1);
    DebugTextPrintf("%s", SHENMUE_COMMIT_HASH);
    debugTextColumn = column;
    debugTextLine = line;
    std::size_t cursor = 0;
    for (std::uint32_t record = 0; record < debugTextCount && cursor + sizeof(DebugTextRecordHeader) <= debugTextUsed; ++record) {
        DebugTextRecordHeader header;
        std::memcpy(&header, debugTextQueue.data() + cursor, sizeof(header));
        cursor += sizeof(header);
        if (header.length > debugTextUsed - cursor)
            break;
        DrawLegacyDebugText({debugTextQueue.data() + cursor, header.length}, static_cast<float>(header.column) * header.advance,
            static_cast<float>(header.line) * header.lineHeight, header.glyphWidth, header.glyphHeight, header.advance, header.lineHeight, header.colour);
        cursor += header.length + 3U;
    }
}
#endif

} // namespace shenmue::s2
