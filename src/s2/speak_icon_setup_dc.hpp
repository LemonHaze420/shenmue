#pragma once

#include "s2/speak_icon_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct CtrlSpeakLabel {
    const char *primary, *alternate;
};
typedef char Layout_speak_icon_setup_dc_0[(sizeof(CtrlSpeakLabel) == 8) ? 1 : -1];
static const std::int32_t CTRL_SPEAK_CHOICE_COUNT = 463;
static const std::int32_t ctrlSpeakLabelBaseCharacters[2] = {4, 8}, ctrlSpeakLabelCharacterWidths[2] = {28, 14};
extern std::int32_t ctrlSpeakMarkerGlyphIndex;
extern std::int16_t ctrlSpeakChoiceIds[4], ctrlSpeakChoiceWidths[4];
extern std::int8_t ctrlSpeakChoiceMode, ctrlSpeakLabelPhase, ctrlSpeakChoiceSpecial, ctrlSpeakLabelMode;
extern std::int16_t ctrlSpeakElapsedFrames, ctrlSpeakTimeoutFrames;
extern std::uint16_t ctrlSpeakDefaultChoiceId;
extern float ctrlSpeakSpecialLabelWidthScale, ctrlSpeakHalfWidthExpansion, ctrlSpeakLeftOriginX, ctrlSpeakRightOriginX;
std::int32_t SetupControlSpeakAndUI_Internal(std::int16_t *choices, std::int32_t timeout, std::uint16_t defaultChoice, std::uint16_t mode, std::int32_t style);
std::int32_t SetupControlSpeakAndUI(std::int16_t *choices, std::int16_t timeout, std::uint16_t defaultChoice, std::uint16_t mode);
const char *GetCtrlSpeakChoiceText(std::uint16_t choice, std::uint16_t mode, std::int32_t &form);
void InspectCtrlSpeakChoicesNullsub(const std::int16_t *choices);
float GetCtrlSpeakChoiceWidth(const std::uint16_t *choice, std::int32_t form);

} // namespace s2
} // namespace shenmue

#endif
