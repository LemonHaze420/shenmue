#pragma once
#include "shenmue.h"
#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#define SHENMUE_SPRITE_ARRAY(type, name, count) type name[count]
#define SHENMUE_SPRITE_CHECK(name, test) typedef char name[(test) ? 1 : -1]
#define SHENMUE_SPRITE_ENUM_CLASS enum
#define SHENMUE_SPRITE_ENUM_BASE
#else
#include <array>
#include <cstddef>
#include <cstdint>
#define SHENMUE_SPRITE_ARRAY(type, name, count) std::array<type, count> name
#define SHENMUE_SPRITE_CHECK(name, test) static_assert(test)
#define SHENMUE_SPRITE_ENUM_CLASS enum class
#define SHENMUE_SPRITE_ENUM_BASE : std::int32_t
#endif
namespace shenmue {
namespace s2 {

SHENMUE_SPRITE_ENUM_CLASS ScreenAlignment SHENMUE_SPRITE_ENUM_BASE{
    VerticalCenter = 1,
    HorizontalCenter = 2,
    BothCentered = 3,
    VerticalStretch = 4,
    HorizontalStretch = 5,
    FullStretch = 6,
};
SHENMUE_SPRITE_ENUM_CLASS ScreenMode SHENMUE_SPRITE_ENUM_BASE{Centered = 1, Stretched = 2, Adjusted = 3, AdjustedWithOffset = 4};

#if SHENMUE_S2_GCC295
struct LegacyScreenPoint {
    float x, y;
};
struct LegacyScreenQuad {
    std::uint32_t flags;
    union {
        float corners[8];
        LegacyScreenPoint points[4];
    };
};
#else
struct LegacyScreenQuad {
    std::uint32_t flags;
    SHENMUE_SPRITE_ARRAY(float, corners, 8);
};
#endif
SHENMUE_SPRITE_CHECK(LegacyScreenQuadSizeCheck, sizeof(LegacyScreenQuad) == 0x24);
SHENMUE_SPRITE_CHECK(LegacyScreenQuadCornersOffsetCheck, offsetof(LegacyScreenQuad, corners) == 4);
struct LegacyColourQuad {
    LegacyScreenQuad quad;
    std::uint32_t colour;
    SHENMUE_SPRITE_ARRAY(std::uint32_t, alternateColours, 3);
};
SHENMUE_SPRITE_CHECK(LegacyColourQuadSizeCheck, sizeof(LegacyColourQuad) == 0x34);

struct LegacySpritePacket {
    LegacyScreenQuad quad;
    std::uint32_t colour;
    SHENMUE_SPRITE_ARRAY(std::byte, reservedColour, 12);
    std::uint32_t secondaryColour;
    SHENMUE_SPRITE_ARRAY(std::byte, reservedUv, 12);
    SHENMUE_SPRITE_ARRAY(float, uv, 4);
    std::uint32_t reservedTail;
};
SHENMUE_SPRITE_CHECK(LegacySpritePacketSizeCheck, sizeof(LegacySpritePacket) == 0x58);
SHENMUE_SPRITE_CHECK(LegacySpritePacketColourOffsetCheck, offsetof(LegacySpritePacket, colour) == 0x24);
SHENMUE_SPRITE_CHECK(LegacySpritePacketSecondaryColourOffsetCheck, offsetof(LegacySpritePacket, secondaryColour) == 0x34);
SHENMUE_SPRITE_CHECK(LegacySpritePacketUvOffsetCheck, offsetof(LegacySpritePacket, uv) == 0x44);

// The native sprite code resets a packet through one statement macro; its do/while(0) is part of the generated code's
// register choice, so keep the macro form at every use.
#define INIT_LEGACY_SPRITE_PACKET(packet)                                                                                                                      \
    do {                                                                                                                                                       \
        (packet).quad.flags = 0;                                                                                                                               \
        (packet).colour = 0xffffffffU;                                                                                                                         \
        (packet).secondaryColour = 0;                                                                                                                          \
        (packet).uv[0] = (packet).uv[1] = 0.0f;                                                                                                                \
        (packet).uv[2] = (packet).uv[3] = 1.0f;                                                                                                                \
        (packet).reservedTail = 0;                                                                                                                             \
    } while (0)

struct TempTextureEntry {
    std::int32_t texture;
    std::uint16_t width, height;
};
SHENMUE_SPRITE_CHECK(TempTextureEntrySizeCheck, sizeof(TempTextureEntry) == 8);
SHENMUE_SPRITE_CHECK(TempTextureEntryWidthOffsetCheck, offsetof(TempTextureEntry, width) == 4);
SHENMUE_SPRITE_CHECK(TempTextureEntryHeightOffsetCheck, offsetof(TempTextureEntry, height) == 6);

#if SHENMUE_S2_GCC295
typedef std::uint16_t TempTextureFlag;
enum { TTF_Loaded = 1, TTF_Region = 2 };
#else
enum TempTextureFlag : std::uint16_t { TTF_Loaded = 1, TTF_Region = 2 };
#endif
struct TempTextureWork {
    std::uint16_t flags;
    std::int16_t texture;
    std::uint16_t width, height;
    SHENMUE_SPRITE_ARRAY(float, uv, 4);
};
SHENMUE_SPRITE_CHECK(TempTextureWorkSizeCheck, sizeof(TempTextureWork) == 0x18);
SHENMUE_SPRITE_CHECK(TempTextureWorkUvOffsetCheck, offsetof(TempTextureWork, uv) == 8);
SHENMUE_SPRITE_CHECK(TempTextureWorkTextureOffsetCheck, offsetof(TempTextureWork, texture) == 2);

struct LegacyColourRect {
    float x, y, scaleX, scaleY, depth;
    std::uint32_t colour;
    std::int16_t width, height;
    std::int32_t rotation;
    std::uint8_t alignment;
    SHENMUE_SPRITE_ARRAY(std::byte, reserved21, 3);
};
SHENMUE_SPRITE_CHECK(LegacyColourRectSizeCheck, sizeof(LegacyColourRect) == 0x24);
SHENMUE_SPRITE_CHECK(LegacyColourRectColourOffsetCheck, offsetof(LegacyColourRect, colour) == 0x14);
SHENMUE_SPRITE_CHECK(LegacyColourRectRotationOffsetCheck, offsetof(LegacyColourRect, rotation) == 0x1c);
SHENMUE_SPRITE_CHECK(LegacyColourRectAlignmentOffsetCheck, offsetof(LegacyColourRect, alignment) == 0x20);

#if SHENMUE_S2_GCC295
struct LegacyIconSprite {
#else
#pragma pack(push, 1)
struct LegacyIconSprite {
#endif
    float x;
    float y;
    float scaleX;
    float scaleY;
    float depth;
    std::uint16_t flags;
    std::uint16_t reserved16;
    std::uint32_t rotation;
#if SHENMUE_S2_GCC295
    std::int16_t icon;
#else
    std::uint16_t icon;
#endif
    std::int16_t width;
    std::int16_t height;
    std::uint8_t alignment;
    SHENMUE_SPRITE_ARRAY(std::byte, reserved23, 1);
};
#if !SHENMUE_S2_GCC295
#pragma pack(pop)
#endif
SHENMUE_SPRITE_CHECK(LegacyIconSpriteSizeCheck, sizeof(LegacyIconSprite) == 0x24);
SHENMUE_SPRITE_CHECK(LegacyIconSpriteFlagsOffsetCheck, offsetof(LegacyIconSprite, flags) == 0x14);
SHENMUE_SPRITE_CHECK(LegacyIconSpriteRotationOffsetCheck, offsetof(LegacyIconSprite, rotation) == 0x18);
SHENMUE_SPRITE_CHECK(LegacyIconSpriteWidthOffsetCheck, offsetof(LegacyIconSprite, width) == 0x1e);
SHENMUE_SPRITE_CHECK(LegacyIconSpriteAlignmentOffsetCheck, offsetof(LegacyIconSprite, alignment) == 0x22);

#if SHENMUE_S2_GCC295
typedef char ScreenAlignmentSizeCheck[sizeof(ScreenAlignment) == 4 ? 1 : -1];
typedef char ScreenModeSizeCheck[sizeof(ScreenMode) == 4 ? 1 : -1];
typedef char TempTextureFlagSizeCheck[sizeof(TempTextureFlag) == 2 ? 1 : -1];
typedef char LegacyIconSpriteAlignmentCheck[__alignof__(LegacyIconSprite) == 4 ? 1 : -1];
#endif

}
} // namespace shenmue::s2

#undef SHENMUE_SPRITE_ARRAY
#undef SHENMUE_SPRITE_CHECK
#undef SHENMUE_SPRITE_ENUM_CLASS
#undef SHENMUE_SPRITE_ENUM_BASE
