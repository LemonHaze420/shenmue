#pragma once

#include "s2/player_stats_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

typedef std::uint32_t GameConfigurationFlag;
enum {
    GCF_SubtitlesEnabled = 1,
    GCF_VoiceEnabled = 2,
    GCF_Unknown08 = 8,
    GCF_DialogueMode = 0x30,
    GCF_NormalTriggerMapping = 0x40,
    GCF_TitleLogo = 0x80,
    GCF_AlternateControls = 0x100
};
static const std::uint8_t kDialogueModeSettings[4][2] = {{1, 0}, {1, 0}, {1, 1}, {0, 1}};
std::uint32_t GetS2GameConfigurationFlags();
void SetS2GameConfigurationFlags(std::uint32_t flags);
void SetSubtitleSettingEnabled(std::uint8_t enabled);
std::int32_t GetSubtitleSettingEnabled();
void SetVoiceSettingEnabled(std::uint8_t enabled);
void ApplyS2DialogueMode(std::int32_t mode);
std::int32_t GetS2DialogueMode();
void ResetConfigurationNullsub();
void SetConfigurationUnknown08(std::uint8_t enabled);
void SetNormalTriggerMapping(std::int32_t enabled);
std::int32_t GetNormalTriggerMapping();
void ApplyS2TitleLogoFlag(std::int32_t enabled);
void ApplyS2ControlMode(std::int32_t mode);
std::int32_t GetS2ControlMode();
void ResetNewGameConfiguration();

} // namespace s2
} // namespace shenmue

#endif
