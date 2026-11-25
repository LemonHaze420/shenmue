#pragma once

#include "s2/input_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"

namespace shenmue {
namespace s2 {

struct ButtonMediationRequest {
    std::uint32_t flags;
    shen_name name;
    Vector3 position;
};
struct ButtonMediationResult {
    shen_name name;
    std::uint32_t flags;
};
struct ButtonMediationControl {
    std::uint32_t flags;
    std::int32_t count;
    std::uint32_t field08;
    ButtonMediationRequest requests[12];
    std::byte retainedRequestState[0x14];
    std::uint32_t field110, field114;
    std::byte retainedInputState[0x0c];
    std::uint32_t field124;
    LegacyInputSlot input;
    std::uint32_t retainedResultState;
    std::int32_t currentButton;
    ButtonMediationResult results[5];
    shen_name priorities[5];
};
typedef char sizeofButtonMediationRequestCheck[sizeof(ButtonMediationRequest) == 0x14 ? 1 : -1];
typedef char offsetofButtonMediationRequestflagsCheck[offsetof(ButtonMediationRequest, flags) == 0 ? 1 : -1];
typedef char offsetofButtonMediationRequestnameCheck[offsetof(ButtonMediationRequest, name) == 4 ? 1 : -1];
typedef char offsetofButtonMediationRequestpositionCheck[offsetof(ButtonMediationRequest, position) == 8 ? 1 : -1];
typedef char sizeofButtonMediationResultCheck[sizeof(ButtonMediationResult) == 8 ? 1 : -1];
typedef char sizeofButtonMediationControlCheck[sizeof(ButtonMediationControl) == 0x180 ? 1 : -1];
typedef char offsetofButtonMediationControlrequestsCheck[offsetof(ButtonMediationControl, requests) == 0x0c ? 1 : -1];
typedef char sizeofButtonMediationControlrequestsCheck[sizeof(((ButtonMediationControl *)0)->requests) == 12 * sizeof(ButtonMediationRequest) ? 1 : -1];
typedef char offsetofButtonMediationControlfield110Check[offsetof(ButtonMediationControl, field110) == 0x110 ? 1 : -1];
typedef char offsetofButtonMediationControlfield114Check[offsetof(ButtonMediationControl, field114) == 0x114 ? 1 : -1];
typedef char offsetofButtonMediationControlfield124Check[offsetof(ButtonMediationControl, field124) == 0x124 ? 1 : -1];
typedef char offsetofButtonMediationControlinputCheck[offsetof(ButtonMediationControl, input) == 0x128 ? 1 : -1];
typedef char offsetofButtonMediationControlretainedResultStateCheck[offsetof(ButtonMediationControl, retainedResultState) == 0x13c ? 1 : -1];
typedef char offsetofButtonMediationControlcurrentButtonCheck[offsetof(ButtonMediationControl, currentButton) == 0x140 ? 1 : -1];
typedef char offsetofButtonMediationControlresultsCheck[offsetof(ButtonMediationControl, results) == 0x144 ? 1 : -1];
typedef char offsetofButtonMediationControlprioritiesCheck[offsetof(ButtonMediationControl, priorities) == 0x16c ? 1 : -1];
using ::shenmue::shen_name;
#define TASK_BTNM shen_name(SHEN_NAME4('B', 'T', 'N', 'M'))
extern ButtonMediationControl g_buttonMediationStorage;
extern ButtonMediationControl *g_buttonMediationControl;
extern std::int32_t g_doublePressResult, g_doublePressRequested, g_doublePressFrames;
void ResetButtonMediation(ButtonMediationControl *control);
void btnButtonMediationCtrl_Callback(void *data);
HLib::Task *Queue_ButtonMediation(std::uint8_t nextFunctionIndex);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t RegisterButtonMediation(shen_name name, const Vector3 *position, std::uint32_t flags);
ButtonMediationResult *FindButtonMediationResult(shen_name name);
std::uint32_t GetMediatedButtonBindings(shen_name name);
std::uint32_t btnCheck(shen_name name);
std::int32_t GetBattleDataConfig(std::int32_t mode, shen_name name);
LegacyInputSlot *GetButtonMediationResultData(shen_name name);
#endif
std::uint32_t IsButtonMediationField124Set();
shen_name GetMediatedButtonOwner(std::uint32_t button);

} // namespace s2
} // namespace shenmue

#endif
