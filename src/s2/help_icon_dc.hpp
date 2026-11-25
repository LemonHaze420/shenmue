#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstddef>
#include <cstdint>

namespace shenmue {
namespace s2 {

typedef std::int32_t CheckHelpIconOp;
enum { CheckHelpIconOp_Set = 0, CheckHelpIconOp_Add = 1, CheckHelpIconOp_Remove = 2, CheckHelpIconOp_FrameOverlay = 3 };
struct HelpIconFlags {
    std::uint32_t visible, selected;
};
// Blink mode and three frame counters; stride 0x14.
struct HelpDirectionBlink {
    std::uint8_t mode;
    std::byte retainedMode[3];
    std::int32_t onFrames, offFrames, remaining;
    std::int32_t visible;
};
typedef char Layout_help_icon_dc_0[(sizeof(HelpIconFlags) == 8) ? 1 : -1];
typedef char Layout_help_icon_dc_1[(offsetof(HelpIconFlags, selected) == 4) ? 1 : -1];
typedef char Layout_help_icon_dc_4[(sizeof(HelpDirectionBlink) == 0x14) ? 1 : -1];
typedef char Layout_help_icon_dc_5[(offsetof(HelpDirectionBlink, onFrames) == 4) ? 1 : -1];
typedef char Layout_help_icon_dc_6[(offsetof(HelpDirectionBlink, offFrames) == 8) ? 1 : -1];
typedef char Layout_help_icon_dc_7[(offsetof(HelpDirectionBlink, remaining) == 0x0c) ? 1 : -1];
static const std::int32_t kHelpDirectionOnFrames[4][4] = {{6, 0, 0, 0}, {5, 5, 0, 0}, {5, 8, 5, 0}, {5, 5, 5, 5}};
static const std::int32_t kHelpDirectionPhases[4][4] = {{6, 0, 0, 0}, {5, -5, 0, 0}, {5, 8, -5, 0}, {-5, 5, -3, 3}};
static const std::uint8_t kHelpDirectionMissingOrder[4] = {1, 3, 0, 2}, kHelpDirectionRingOrder[4] = {0, 3, 1, 2};
extern HelpIconFlags helpIconPersistent, helpIconFrameOverlay;
extern HelpDirectionBlink helpDirectionBlinks[4];
extern std::uint32_t helpIconDeferredRemoval;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern std::int32_t helpIconItemPromptFrames;
extern std::uint32_t iconSelectFlags;
void RequestHelpIconItemPrompt();
void RequestHelpIconSelection(std::uint32_t mask);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::uint8_t kNativeHelpTopicCount = 148;
extern std::uint8_t helpIconIndex, helpPreviousIconIndex, helpIconMode;
std::uint32_t GetHelpIconEnabled();
std::uint8_t helpGetCurrentIconIdx(std::int32_t slot);
void EV_EventSetUpHelpIcon_0(std::uint8_t topic, std::uint8_t mode);
#endif
void MarkHelpIconsForRemoval(std::uint32_t mask);
void ConfigureHelpDirectionBlink(std::uint32_t visible);
void CheckHelpIcon(std::uint32_t visible, std::uint32_t selected, CheckHelpIconOp operation);
std::uint32_t GetPersistentHelpIconFlags(std::uint32_t index);
std::uint32_t GetUiFlag(std::uint8_t mode);
void AddHelpIconMenuFlags(std::uint16_t flags, std::uint8_t selected = 1);
void SetHelpIconMenuFlags(std::uint16_t flags, std::uint8_t selected = 1);
void ResetHelpIconsToItemPrompt();

} // namespace s2
} // namespace shenmue

#endif
