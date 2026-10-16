#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1::ui_runtime_detail {





struct SfxOverlayBoostFrames {
    std::int16_t stepFrames;
    std::int16_t accumulatedFrames;
};

static_assert(sizeof(SfxOverlayBoostFrames) == 4);

inline constexpr std::uint8_t kSfxPlayPrimingCue = 0x01;

inline constexpr std::uint8_t kSfxShowOverlay = 0x02;

inline constexpr std::uint8_t kSfxWaitForExternalClose = 0x04;

inline constexpr std::uint8_t kSfxAlternateResultSound = 0x08;

inline constexpr std::uint8_t kSfxCloseImmediately = 0x10;

inline constexpr std::uint8_t kSfxSubsetMaskMode = 0x20;

inline constexpr std::array<std::uint16_t, 16> kSfxMaskLut{
    0x000,
    0x001,
    0x010,
    0x000,
    0x040,
    0x080,
    0x020,
    0x000,
    0x004,
    0x002,
    0x008,
    0x000,
    0x011,
    0x000,
    0x010,
    0x000,
};




inline constexpr std::array<const char *, 4> kQteMinigameKeyIconIds{"Multi_Left_PC", "Multi_Up_PC", "Multi_Down_PC", "Multi_Right_PC"};

inline constexpr std::array<const char *, 4> kQteMinigamePadIconIds{"Multi_Left", "Multi_Up", "Multi_Down", "Multi_Right"};

// Game QTE (element 52, GameQTE.ui): the QTE button prompt ICQu shows while a Script_SFXUtil overlay is up. Its windows are the
// MultiPanel_Mouse_ and MultiPanel_Default_ direction windows (the view authors only Up, Right, Down and Left), then DefaultPanel,
// MousePanel and PlatformPanel.
inline constexpr std::array<const char *, 8> kGameQteDirections{"Up", "Up_Right", "Right", "Down_Right", "Down", "Down_Left", "Left", "Up_Left"};

inline constexpr std::size_t kGameQteDefaultPanel = 16;

inline constexpr std::size_t kGameQteMousePanel = 17;

inline constexpr std::size_t kGameQtePlatformPanel = 18;

} // namespace shenmue::s1::ui_runtime_detail
