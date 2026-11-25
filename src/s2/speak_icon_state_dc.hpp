#pragma once

#include "s2/ui_state_dc.hpp"
#include "s2/item_effect.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

extern std::int8_t ctrlSpeakIconState;
extern std::uint8_t ctrlSpeakOpenRequested;
extern std::uint16_t ctrlSpeakMode, ctrlSpeakResultChoiceId;
extern ItemEffectPosition ctrlSpeakSelectorPosition;
extern std::uint32_t ctrlSpeakSavedLoadingFlags, g_loadingFlag;
extern std::int32_t ctrlSpeakTexturePoolHandles[4], ctrlSpeakChoiceTextHandles[4];
extern std::uint32_t ctrlSpeakTexturePoolFlags[4];
std::int32_t IsCtrlSpeakIconInactive();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t IsSpeakIconCancellable();
#else
bool IsSpeakIconCancellable();
#endif
std::uint32_t shenGetCtrlSpeakIconFlag();
std::int32_t ReleaseCtrlSpeakTextHandle(std::int32_t handle);
void ReleaseSpeakTextSlots();
bool padcRefreshCurrentInputs(std::int32_t bind);
void CloseSpeakIcon();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const float ctrlSpeakLabelOffsetX[4] = {0.0f, -61.0f, 0.0f, 62.0f}, ctrlSpeakLabelOffsetY[4] = {-35.0f, 0.0f, 36.0f, 0.0f};
static const float ctrlSpeakLabelDepths[4] = {4.0f, 4.0f, 5.0f, 5.0f};
extern LegacyIconSprite ctrlSpeakLabelSprites[4];
bool CloseSpeakIcon(std::int32_t force);
void InitializeCtrlSpeakIconState();
void CtrlSpeakIcon_Callback(void *data);
#endif

} // namespace s2
} // namespace shenmue

#endif
