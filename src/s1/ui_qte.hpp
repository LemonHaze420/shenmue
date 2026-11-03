#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#if SHENMUE_IS_DEMO
#include "s1/screen_space.hpp"
#endif

namespace shenmue::s1::ui_runtime_detail {





#if SHENMUE_IS_DEMO
extern ScreenSpaceParams g_qteSprite;
extern float g_qtePanelX;
extern float g_qtePanelSpeed;
extern std::int32_t g_qteSessionFrameCount;
inline constexpr std::array<std::uint16_t, 5> kTrialQteButtonSprites{17, 25, 28, 29, 0};
void UI_QteInitializeSprite();
void UI_QteDraw();
#endif

struct QteOverlayBoostFrames {
    std::int16_t stepFrames;
    std::int16_t accumulatedFrames;
};

static_assert(sizeof(QteOverlayBoostFrames) == 4);

inline constexpr std::uint8_t kQtePlayPrimingCue = 0x01;

inline constexpr std::uint8_t kQteShowOverlay = 0x02;

inline constexpr std::uint8_t kQteWaitForExternalClose = 0x04;

inline constexpr std::uint8_t kQteAlternateResultSound = 0x08;

inline constexpr std::uint8_t kQteCloseImmediately = 0x10;

inline constexpr std::uint8_t kQteSubsetMaskMode = 0x20;

inline constexpr std::array<std::uint16_t, 16> kQteMaskLut{
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

// Game QTE (element 52, GameQTE.ui): the QTE button prompt ICQu shows while a Script_QTEUtil overlay is up. Its windows are the
// MultiPanel_Mouse_ and MultiPanel_Default_ direction windows (the view authors only Up, Right, Down and Left), then DefaultPanel,
// MousePanel and PlatformPanel.
inline constexpr std::array<const char *, 8> kGameQteDirections{"Up", "Up_Right", "Right", "Down_Right", "Down", "Down_Left", "Left", "Up_Left"};

inline constexpr std::size_t kGameQteDefaultPanel = 16;

inline constexpr std::size_t kGameQteMousePanel = 17;

inline constexpr std::size_t kGameQtePlatformPanel = 18;

} // namespace shenmue::s1::ui_runtime_detail
