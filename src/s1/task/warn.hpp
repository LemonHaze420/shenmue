#pragma once

#include "s1/hlib.hpp"

#include <cstdint>

namespace shenmue::s1 {

// LegacySavingMessages: the Dreamcast memory-card messages WARN shows by one-based index. '%' ends a line, "@1".."@6" open and close
// the three panels, and '^' prints g_warningChoiceOrDisc.
inline constexpr const char *kLegacySavingMessages[28]{
    "%%%%@1Now checking the memory card.%Do not remove the memory card.@2%",
    "%@1No memory card is inserted.%%Insert a memory card%in Expansion Socket 1 of Control Port A."
    "%%A minimum of 80 free memory blocks%is required to save game data.@2%",
    "%%@1There is not enough%free memory blocks available.%%A minimum of 80 free memory blocks%is required to save game data.@2%",
    "%              @3YES@4      @5N O@6%%%@1Start a game without a memory card?%"
    "%Note that saving or loading of game data%is not possible without a memory card.%Continue?@2%",
    "%@1No memory card is inserted.%%Insert a memory card%in Expansion Socket 1 of Control Port A."
    "%%A minimum of 80 free memory blocks%is required to save game data.@2%",
    "%%@1Now Saving.%%To avoid damaging the game file,%do not turn the Dreamcast power OFF%or attempt to remove the memory card.@2%",
    "%%%%%@1Game save is complete.@2%",
    "%%%%%@1Attempt to save game unsuccessful.@2%",
    "%%%@1No memory card is inserted.%%Insert a memory card into%Expansion Socket 1 of Control Port A.@2%",
    "%%@1Now loading.%%To avoid damaging the game file,%do not turn the Dreamcast power OFF%or attempt to remove the memory card.@2%",
    "%%%%%@1Game load is complete.@2%",
    "%%%%@1Attempt to load game unsuccessful.%Retry loading of the game file.@2%",
    "%%        @3YES@4      @5N O@6%%%@1Unable to locate saved file.%Start a new game?@2%",
    "%%%%@1This memory card has not been formatted.%Format the memory card from the File Menu.@2%",
    "%%%%%@1The file has been created.@2%",
    "@1This memory card has not been formatted.%Format the memory card from the File Menu.%%To save now,%insert a formatted memory card into"
    "%Expansion Socket 1 of Control Port A.%%A minimum of 80 free memory blocks%is required to save game data.@2%",
    "%%%@1The selected file is data from DISC ^.%%Press the Open Button on the Dreamcast and%replace the current disc with DISC ^.@2%",
    "%%@1Creating file.%%To avoid damaging the game file,%do not turn the Dreamcast power OFF%or attempt to remove the memory card.@2%",
    "%           @3YES@4      @5N O@6%%@1This memory card is different from%the one previously used. Insert the"
    "%previously used memory card.%Saving the game at this time will%cause the contents of this memory%card to be overwritten.%Continue?@2%",
    "%%%@3YES@4      @5N O@6%%",
    "%%@1A new game can only%be started from DISC 1.%%Press the Open Button%on the Dreamcast and%replace the current disc with DISC 1.@2%",
    "%%@1The [70 Person Free Battle] is a mode%specific to DISC 3.%%Press the Open Button on the Dreamcast and%replace the current disc with DISC 3.@2%",
    "%%         @3YES@4      @5N O@6%%%@1Return to title screen without%saving the [Cleared Game File]?@2%",
    "%%@1A game cannot be started%from [Cleared Game File].%%Select another game file%or select [New Game]%from the Start Menu.@2%",
    "%%@1Loading the [Resume File].%%To avoid damaging the game file,%do not turn the Dreamcast power OFF%or attempt to remove the memory card.@2%",
    "%%%@1Attempt to load%the [Resume File] unsuccessful.%%Retrying.@2%",
    "%%%%@1Attempt to save unsuccessful.%%Retrying.@2%",
    "%%%%@1Attempt to create new file unsuccessful.%%Retrying.@2%",
};

// A message line's surface width in 16-pixel units, by its last column.
inline constexpr std::int8_t kWarningLineWidthBuckets[42]{
    2,
    4,
    4,
    4,
    8,
    8,
    8,
    8,
    8,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    32,
    64,
    64,
    64,
    64,
    64,
    64,
};

inline constexpr std::uint32_t kWarningShowColour = 0x76000000U;

extern std::int32_t g_warningState;
extern std::int32_t g_warningMessageIndex;
extern std::int32_t g_warningChoiceOrDisc;
extern const char *g_warningMessage;
extern std::uint32_t g_warningShowColour;
extern std::uint32_t g_warningTargetColour;
extern std::uint32_t g_warningPackedColour;
extern std::int32_t g_warningLineBaseX;
extern std::int16_t g_warningSurfaces[10];
extern std::int16_t g_warningLineWidths[10];
extern std::int16_t g_warningPanelX[3];
extern std::int16_t g_warningPanelY[3];
extern std::int16_t g_warningPanelWidth[3];
extern std::int16_t g_warningPanelHeight[3];

void WARN_Callback(HLib::TaskData *taskData);
void WRN_CtrlWarning_Enqueue();

} // namespace shenmue::s1
