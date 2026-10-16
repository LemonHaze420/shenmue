#include "s2/areas/game_emulator_overlay.hpp"

#include "s2/areas/game_emulator_draw.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s1/modules/harrier.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace shenmue::s2 {
namespace {

struct EmulatorFontRecord {
    std::array<std::uint32_t, 128> glyphs;
    float width, height, scaleX, scaleY;
};
static_assert(sizeof(EmulatorFontRecord) == 528);
std::array<EmulatorFontRecord, 16> fonts{};
std::array<EmulatorPositionColourTextureVertex, 4> fontVertices{};
std::array<std::uint32_t, 16> fontColours{};
std::uint32_t fontTexture{};
std::int32_t selectedFont{}, selectedColour{}, fontEnabled{}, fontReserved{};
float cursorX{}, cursorY{}, width{}, height{}, scaleX{}, scaleY{}, glyphScaleX{}, glyphScaleY{}, fontDepth{};

void SelectFont(std::uint8_t index) {
    selectedFont = index & 15;
    const EmulatorFontRecord &font = fonts[selectedFont];
    width = font.width;
    height = font.height;
    scaleX = font.scaleX;
    scaleY = font.scaleY;
    glyphScaleX = scaleX;
    glyphScaleY = scaleY;
}
void SetFontScale(float x, float y) {
    scaleX = x * fonts[selectedFont].scaleX;
    scaleY = y * fonts[selectedFont].scaleY;
    glyphScaleX = scaleX;
    glyphScaleY = scaleY;
}
void SetFontCursor(std::int32_t x, std::int32_t y) {
    cursorX = static_cast<float>(x) * 16;
    cursorY = static_cast<float>(y) * 16;
}
void SetFontColour(std::int32_t colour) {
    selectedColour = colour;
}
void SetFontRenderFlags() {
    d3t::D3TDeviceContext &context = *d3t::DX11::g_ppD3DDeviceContext;
    const std::array<std::uint64_t, 2> sampler{21, 0};
    if (std::memcmp(context.renderState.data() + 0x574, sampler.data(), sizeof(sampler)) != 0) {
        std::memcpy(context.renderState.data() + 0x574, sampler.data(), sizeof(sampler));
        d3t::GetRenderState(context).samplerDirtyMasks[1] |= 0x10;
    }
    BindEmulatorTexture(fontTexture);
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 1);
    SetEngineFlag(62, 770);
    SetEngineFlag(63, 771);
}
void DrawGlyph(float x, float y, float xScale, float yScale, std::uint8_t glyph, std::int32_t colour) {
    const float halfWidth = xScale * 8;
    const float u = static_cast<float>(glyph & 15) * .0625f;
    const float v = static_cast<float>(glyph / 16) * .0625f;
    for (std::size_t vertex = 0; vertex < 4; ++vertex) {
        EmulatorPositionColourTextureVertex &entry = fontVertices[vertex];
        entry.x = vertex & 1 ? x + halfWidth : x - halfWidth;
        entry.y = vertex < 2 ? y : yScale * 16 + y;
        entry.colour = fontColours[colour];
        entry.u = vertex & 1 ? u + .0625f : u;
        entry.v = vertex < 2 ? v : v + .0625f;
    }
    DrawEmulatorVertices(fontVertices.data(), 28, 4, 31, 15);
}
void PrintFont(const char *text) {
    SetFontRenderFlags();
    cursorX += scaleX * width;
    for (const unsigned char *cursor = reinterpret_cast<const unsigned char *>(text); *cursor; ++cursor) {
        DrawGlyph(cursorX, cursorY, glyphScaleX, glyphScaleY, *cursor, selectedColour);
        cursorX += scaleX * width + scaleX * width;
    }
    cursorX -= scaleX * width;
}
void DrawPauseOverlay() {
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 1);
    SetEngineFlag(62, 770);
    SetEngineFlag(63, 771);
    const std::array<EmulatorPositionColourVertex, 4> vertices{{
        {.x = 0, .y = 0, .z = .5f, .w = 1, .colour = 0x68000000},
        {.x = 640, .y = 0, .z = .5f, .w = 1, .colour = 0x68000000},
        {.x = 0, .y = 480, .z = .5f, .w = 1, .colour = 0x68000000},
        {.x = 640, .y = 480, .z = .5f, .w = 1, .colour = 0x68000000},
    }};
    DrawEmulatorVertices(vertices.data(), 20, 4, 30, 14);
}
void DrawDirectionMenu() {
    if (g_inputBuffer[0].pressedPriority & 0x3000) {
        s1::g_harrierInputOrientation = (s1::g_harrierInputOrientation - 1) & 1;
        const float magnitude = std::fabs(s1::g_harrierInputScaleY);
        s1::g_harrierInputScaleY = s1::g_harrierInputOrientation ? -magnitude : magnitude;
    }
    SelectFont(0);
    SetFontScale(1, 1);
    SetFontColour(15);
    for (std::int32_t row = 12; row <= 16; ++row) {
        SetFontCursor(10, row);
        PrintFont("\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1");
    }
    SetFontColour(15);
    SetFontCursor(11, 12);
    PrintFont("PAD DIRECTION");
    if (s1::g_harrierInputOrientation != 0 && s1::g_harrierInputOrientation != 1)
        return;
    const bool type2 = s1::g_harrierInputOrientation == 1;
    SetFontColour(15);
    SetFontCursor(11, type2 ? 15 : 14);
    PrintFont(">");
    SetFontColour(type2 ? 15 : 2);
    SetFontCursor(12, 14);
    PrintFont("TYPE1");
    SetFontColour(type2 ? 2 : 15);
    SetFontCursor(12, 15);
    PrintFont("TYPE2");
    SetFontColour(2);
    SetFontCursor(18, 14);
    PrintFont(type2 ? "    UP" : "    DOWN");
    SetFontCursor(18, 15);
    PrintFont("LEFT + RIGHT");
    SetFontCursor(18, 16);
    PrintFont(type2 ? "    DOWN" : "    UP");
}

} // namespace

void InitializeEmulatorFont() {
    for (EmulatorFontRecord &font : fonts) {
        font.width = 8;
        font.height = 16;
        font.scaleX = 1;
        font.scaleY = 1;
        font.glyphs.fill(0);
    }
    fontTexture = LoadEmulatorImage("XHAR/datas/SYSASCII.BMP");
    fontColours = {0xff404040, 0xff0000ff, 0xffff0000, 0xffff00ff, 0xff00ff00, 0xff00ffff, 0xffffff00, 0xffffffff, 0xff008080, 0xffa02000, 0xffff8000,
        0xffff20a0, 0xff000000, 0xff505050, 0xffa0a0a0, 0xfff0f0f0};
    for (std::size_t vertex = 0; vertex < 4; ++vertex)
        fontVertices[vertex] = {.x = 0, .y = 0, .z = 1, .w = 1, .colour = 0xff404040, .u = static_cast<float>(vertex & 1), .v = 0};
    fontDepth = 1;
    SelectFont(0);
    glyphScaleX = scaleX;
    cursorX = 0;
    cursorY = 0;
    selectedColour = 15;
    fontEnabled = 1;
    fontReserved = 0;
}
void ReleaseEmulatorFont() {
    d3t::InitTexture2DResource(fontTexture);
}

// @verified - p
void RenderArcadeControllerOverlay(GameArcadeRuntime &runtime) {
    if (runtime.paused) {
        DrawPauseOverlay();
        if (runtime.debugEnabled) {
            if (runtime.debugEnabled == 1) {
                DrawDirectionMenu();
                if (runtime.inputOrientation == 1 && (g_inputBuffer[0].pressedPriority & 0x400))
                    runtime.debugEnabled = 0;
            }
        } else {
            if (++runtime.pauseFrames > 24)
                runtime.pauseFrames = 0;
            SelectFont(0);
            SetFontScale(1, 1);
            SetFontColour(runtime.pauseFrames <= 12 ? 15 : 2);
            SetFontCursor(16, 14);
            if (runtime.inputOrientation == 1 && (g_inputBuffer[0].pressedPriority & 0x400))
                runtime.debugEnabled = 1;
        }
    }
    if (runtime.controllerDisconnected) {
        SelectFont(0);
        SetFontScale(1, 1);
        SetFontColour(15);
        SetFontCursor(3, 6);
        PrintFont("\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1");
        SetFontCursor(3, 7);
        PrintFont("\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1");
        SetFontColour(15);
        SetFontCursor(4, 6);
        PrintFont("Please reconnect the controller");
        SetFontCursor(4, 7);
        PrintFont("  and press START to continue. ");
        if (--runtime.disconnectedTimer < 0)
            runtime.disconnectedTimer = 120;
    }
}

} // namespace shenmue::s2
