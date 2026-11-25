#include "s2/title_dc.hpp"
#include "s2/title_warning.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/legacy_sprite_slot.hpp"
#include "s2/texture_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

void SetTitleWarningLogo(std::int32_t logo, const void *parameters);
void FreeTitleWarningResources();

std::int32_t titleWarningLogo, titleWarningUsesLogoParameters = 0;

std::uint8_t titleWarningLogoParameters[7];

std::int32_t g_titleMenuSelection = -1, g_previousTitleMenuSelection = -1, g_titleMenuUIState = 0, titleWheelRotation = 0;

BackupHotplugData *titleBackupData;

std::int32_t titleDebugSaveSlot;

// @verified - d
void TitleBackupCallback(void *data) {
    titleBackupData = static_cast<BackupHotplugData *>(data);
    titleDebugSaveSlot = 2;
    while (titleDebugSaveSlot >= 0 && static_cast<BackupHotplugData *>(data)->saves[titleDebugSaveSlot].saveKind == 0)
        --titleDebugSaveSlot;
}

// @verified - d
void InitializeTitleWarnings() {
    SetTitleWarningLogo(0, 0);
    FreeTitleWarningResources();
    InitSceneEffects();
}

// @verified - d
void ResetTitleMenuSelection() {
    g_titleMenuSelection = -1;
    titleWheelRotation = 0;
}

// @verified - d
std::int32_t GetTitleMenuUIState() {
    return g_titleMenuUIState;
}

// @verified - d
void DrawTitleWarning41(void *) {
    SetTitleWarningLogo(41, 0);
}

// @verified - d
void DrawTitleWarning40(void *) {
    SetTitleWarningLogo(40, 0);
}

// @verified - d
void DrawTitleWarning1(void *) {
    SetTitleWarningLogo(1, 0);
}

// @verified - d
void DrawTitleWarning43(void *) {
    SetTitleWarningLogo(43, 0);
}

} // namespace s2
} // namespace shenmue

#endif
