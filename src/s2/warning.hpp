#pragma once

#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name WARNING_TASK{SHEN_NAME(WARN)};
inline constexpr shen_name WARNING_TEXT_TEXTURE{SHEN_NAME(WARN)};
inline constexpr shen_name RECONNECT_TEXT_TEXTURE{SHEN_NAME(WARP)};
inline constexpr shen_name WARNING_TEXT_TEXTURE_ID{SHEN_NAME(9999)};
inline constexpr std::uint16_t WARNING_TEXT_FORMAT = 0x8089;
inline constexpr std::int32_t WARNING_TEXT_WIDTH = 1024;
inline constexpr std::int32_t WARNING_TEXT_HEIGHT = 16;
// Line textures use the even slots; slot 28 holds the base '9999' admission.
inline constexpr std::size_t WARNING_TEXT_SLOTS = 29;
inline constexpr std::int32_t WARNING_LINE_TEXTURES = 14;
inline constexpr std::uint32_t WARNING_BASE_COLOUR = 0x76000000U;
inline constexpr std::uint32_t RECONNECT_BASE_COLOUR = 0xaf000000U;
inline constexpr std::uint32_t WARNING_COLOUR_STEP = 0x08000000U;


enum class WarningState : std::int32_t { Idle, Layout, Shown, Retiring };

// Native WARN message table: the Xbox save/system messages, indexed by warning id - 1. '%' ends a line, '@1'..'@6' open and

inline constexpr std::array<const char *, 29> WARNING_MESSAGES{
    "%%@1 Your Xbox doesn't have%enough free blocks.% ^ more block(s) must be free%to save Game Data.% Press A to continue.@2%",
    "%@1No memory card is inserted.%%Insert a memory card in Expansion%Socket 1 of Control Port A.%%%A minimum of ^ free memory "
    "blocks%is required to save game data.@2%",
    "%%%%@1No memory card or controller%is inserted, or now checking.@2%",
    "%%%@1This memory card has not been formatted.%%Format the memory card %from the Data Menu.@2%",
    "%%%     @3Yes@4        @5N o@6%%%@1Cancel saving game data?@2%",
    "%%%%%@1Cancelled save.@2%",
    "%%@1Create New Game Data before saving.%%Game Data%saves game progress data.%",
    "%@1There is not enough free memory.%%A minimum of ^ %free memory blocks%is required to save the Game Data.%%Insert a memory "
    "card in Expansion%Socket 1 of Control Port A.@2%",
    "%@1Now saving.%%Please don't turn off your Xbox console.@2%",
    "%%%%%@1Attempt to save unsuccessful.@2%",
    "%%%@1 Saving Game Data.%Please don't turn off%your Xbox console. @2%",
    "%%%@1Unable to locate System Data.%%A System Data is required %to play Shenmue II.@2%",
    "%%%    @3Yes@4     @5N o@6%%%@1Create System Data?@2%",
    "%%%      @3Yes@4        @5N o@6%%@1Overwrite saved Game Data?@2%",
    "%%%@1Unable to locate Game Data.%%A Game Data is required %to save game progress data.@2%",
    "%%%   @3Yes@4     @5N o@6%%%@1Create Game Data?@2%",
    "%%%         @3Yes@4     @5N o@6%%%@1Attempt to save unsuccessful.%Retry?@2%",
    "%%%%@1No memory card is inserted in%Expansion Socket 1 of Control.@2%",
    "%%%%@1DISC^ is complete.%Press A Button.@2%",
    "%%%   @3Yes@4   @5N o@6%%%@1Save Game Data?@2%",
    "%%%%%@1snap snap snap snap.@2%",
    "%%%@1 Your Xbox doesn't have%enough free blocks.% ^ more block(s) must be free%to save the snap-shots.% Press A to continue.@2%",
    "%%%@1Snapshots over ^ pictures.@2%",
    "%%%@1Please reconnect the controller%and press START to continue.@2%",
    "%%%@1Unable to load snap-shots data.%Press A to continue.@2%",
    "%%%@1There's a problem with %the disc you're using.%It may be dirty or damaged.@2%",
    "%%%@1    Save failed.%Press A to continue.@2%",
    "%%%@1There's a problem with the snap-shots data.%Press A to continue.@2%",
    "%%%@1There's a problem with the snap-shot.%Re-sort the snap-shots.%Press A to continue.@2%",
};

inline constexpr const char *RECONNECT_MESSAGE = "%Please reconnect the controller%";
inline constexpr std::array<char, 10> WARNING_DIGITS{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};




void *WRN_CtrlWarning_Enqueue();
void *AllocateWARP(std::int32_t textureSize);


void WRN_SetWarning(std::int32_t warning, std::int32_t value);
// Requests the reconnect prompt for the next WARN frame; the request is consumed every frame.
void WRN_RequestReconnectPrompt();
// Replaces the warning dim colour and returns the previous one.
std::uint32_t WRN_SetBaseColour(std::uint32_t colour);
std::int16_t *WRN_GetWarningTextures();
// Uploads the WARN text buffer into line slot `slot`; HELP and ADVC also render their legacy text through it.
std::uint32_t WRN_UploadWarningLine(std::int16_t slot);

} // namespace shenmue::s2
