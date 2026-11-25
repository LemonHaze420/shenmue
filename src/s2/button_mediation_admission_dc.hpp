#pragma once

#include "s2/button_mediation_runtime_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

static const std::uint32_t kButtonRequestRejected = 0x100, kButtonRequestPlayerRelative = 0x20, kButtonRequestSuppressTalk = 0x10;
static const float kButtonPlayerEyeHeight = 1.2f;
#define BUTTON_BTMK SHEN_NAME4('B', 'T', 'M', 'K')
#define BUTTON_BTKK SHEN_NAME4('B', 'T', 'K', 'K')
#define BUTTON_BTBF SHEN_NAME4('B', 'T', 'B', 'F')
#define BUTTON_BTFT SHEN_NAME4('B', 'T', 'F', 'T')
#define BUTTON_BTBA SHEN_NAME4('B', 'T', 'B', 'A')
#define BUTTON_BTNV SHEN_NAME4('B', 'T', 'N', 'V')
#define BUTTON_BTTT SHEN_NAME4('B', 'T', 'T', 'T')
#define BUTTON_BTCC SHEN_NAME4('B', 'T', 'C', 'C')
#define BUTTON_BTBB SHEN_NAME4('B', 'T', 'B', 'B')
#define BUTTON_BTPP SHEN_NAME4('B', 'T', 'P', 'P')
#define BUTTON_BTBT SHEN_NAME4('B', 'T', 'B', 'T')
#define BUTTON_BTSD SHEN_NAME4('B', 'T', 'S', 'D')
#define BUTTON_BTMR SHEN_NAME4('B', 'T', 'M', 'R')
#define BUTTON_BTD2 SHEN_NAME4('B', 'T', 'D', '2')
#define BUTTON_BTLD SHEN_NAME4('B', 'T', 'L', 'D')
#define BUTTON_BTEV SHEN_NAME4('B', 'T', 'E', 'V')
#define BUTTON_BTDR SHEN_NAME4('B', 'T', 'D', 'R')
#define BUTTON_BTMP SHEN_NAME4('B', 'T', 'M', 'P')
#define BUTTON_BTBX SHEN_NAME4('B', 'T', 'B', 'X')
#define BUTTON_BTDD SHEN_NAME4('B', 'T', 'D', 'D')
#define BUTTON_BTMT SHEN_NAME4('B', 'T', 'M', 'T')
#define BUTTON_BTMM SHEN_NAME4('B', 'T', 'M', 'M')
#define BUTTON_BTIF SHEN_NAME4('B', 'T', 'I', 'F')
#define BUTTON_BTIL SHEN_NAME4('B', 'T', 'I', 'L')
#define BUTTON_BTSW SHEN_NAME4('B', 'T', 'S', 'W')
#define BUTTON_BTBY SHEN_NAME4('B', 'T', 'B', 'Y')
#define BUTTON_BTAD SHEN_NAME4('B', 'T', 'A', 'D')
#define BUTTON_BTSP SHEN_NAME4('B', 'T', 'S', 'P')
#define BUTTON_BTME SHEN_NAME4('B', 'T', 'M', 'E')
static const std::uint32_t kButtonFirstPriorities[] = {BUTTON_BTMK, BUTTON_BTKK, BUTTON_BTBF, BUTTON_BTFT, BUTTON_BTBA, 0};
static const std::uint32_t kButtonSecondPriorities[] = {BUTTON_BTMK, BUTTON_BTNV, BUTTON_BTTT, BUTTON_BTBF, BUTTON_BTCC, BUTTON_BTBB, 0};
static const std::uint32_t kButtonThirdPriorities[] = {BUTTON_BTMK, BUTTON_BTPP, BUTTON_BTBF, BUTTON_BTBT, BUTTON_BTSD, BUTTON_BTMR, BUTTON_BTD2, BUTTON_BTLD,
    BUTTON_BTEV, BUTTON_BTDR, BUTTON_BTMP, BUTTON_BTBX, 0};
static const std::uint32_t kButtonFourthPriorities[] = {
    BUTTON_BTMK, BUTTON_BTDD, BUTTON_BTBF, BUTTON_BTMT, BUTTON_BTMM, BUTTON_BTIF, BUTTON_BTIL, BUTTON_BTSW, BUTTON_BTBY, 0};
static const std::uint32_t kButtonEmptyPriorities[] = {0}, kButtonFirstFallback[] = {BUTTON_BTAD, BUTTON_BTSP, 0};
static const std::uint32_t kButtonThirdFallback[] = {BUTTON_BTME, 0};
bool ButtonRequestFacesPlayer(const ButtonMediationRequest *request);
bool ButtonRequestInCamera(const ButtonMediationRequest *request);
std::int32_t IsButtonSearchable();
bool IsScenarioButtonAdmissionEnabled();

} // namespace s2
} // namespace shenmue

#endif
