#pragma once

#include "s2/button_mediation_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const std::uint16_t kMediatedButtonPressMasks[5]= {0x400, 0x200, 4, 2, 0x800};
static const std::uint32_t kMediatedButtonHelpMasks[4]= {4, 2, 0x10, 8};
static const std::uint32_t kMediatedButtonHint = 4, kMediatedActionHint = 8;
static const std::uint32_t kMediatedBindingShift = 16, kMediatedPressedShift = 11;
void FilterButtonRequests(ButtonMediationControl *control);
void PrioritizeButtonRequests(ButtonMediationControl *control);
void SelectButtonRequest(ButtonMediationControl *control, Vector3s &angles);
void ResolveButtonMediationRequests(ButtonMediationControl *control);
std::uint32_t CheckMediatedDoublePress();

} } // namespace shenmue::s2

#endif
