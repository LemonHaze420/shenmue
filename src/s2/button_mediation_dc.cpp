#include "s2/button_mediation_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/button_mediation_admission_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <cstring>
#include "s2/button_mediation_runtime_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/scenario_control_dc.hpp"

namespace shenmue {
namespace s2 {

void btnButtonMediationCtrl_Callback(void *);
void ResetButtonMediation(ButtonMediationControl *control);
void ResolveButtonMediationRequests(ButtonMediationControl *control);
void FilterButtonRequests(ButtonMediationControl *control);
void PrioritizeButtonRequests(ButtonMediationControl *control);
ButtonMediationResult *FindButtonMediationResult(shen_name name);
bool ButtonRequestFacesPlayer(const ButtonMediationRequest *request);
bool ButtonRequestInCamera(const ButtonMediationRequest *request);
std::int32_t IsButtonSearchable();
bool IsScenarioButtonAdmissionEnabled();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsScenarioTalkInputCaptured();
#endif

ButtonMediationControl g_buttonMediationStorage = {};
ButtonMediationControl *g_buttonMediationControl = &g_buttonMediationStorage;
std::int32_t g_doublePressResult, g_doublePressRequested = 0, g_doublePressFrames = 0;

// @verified - d
HLib::Task *Queue_ButtonMediation(std::uint8_t nextFunctionIndex) {
    const std::int8_t index = static_cast<std::int8_t>(nextFunctionIndex);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(btnButtonMediationCtrl_Callback, index, 0, sizeof(ButtonMediationControl), TASK_BTNM);
    HLib::GetTaskParameter(task);
    ResetButtonMediation(g_buttonMediationControl);
    g_doublePressRequested = g_doublePressResult = 0;
    g_doublePressFrames = 0;
    return task;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void btnButtonMediationCtrl_Callback(void *) {
    ButtonMediationControl *const control = g_buttonMediationControl;
    ButtonMediationResult *result = 0;
    PadControlEntry *const pad = GetMapPadControlEntry();
    LegacyInputSlot *const input = ctrlGetPadInputBuffer0(pad);
    std::int32_t index;
    control->flags &= 0x80000000U;
    control->field114 = 0;
    control->field08 = 0;
    control->field124 = 0;
    for (index = 0; index < 5; ++index) {
        control->results[index].name = 0;
        control->priorities[index] = 0;
    }
    if (control->count > 0) {
        ResolveButtonMediationRequests(control);
        std::memcpy(&control->input, input, sizeof(control->input));
    }
    control->count = 0;
    for (index = 0; index < 12; ++index)
        control->requests[index].name = 0;
    control->field08 = 0;
    for (index = 0; index < 5; ++index) {
        result = &control->results[index];
        if (!result->name)
            continue;
        if (result->flags & kMediatedButtonHint) {
            if (result->flags & 0x10000U)
                CheckHelpIcon(4, 0, CheckHelpIconOp_FrameOverlay);
            if (result->flags & 0x20000U)
                CheckHelpIcon(2, 0, CheckHelpIconOp_FrameOverlay);
            if (result->flags & 0x40000U)
                CheckHelpIcon(0x10, 0, CheckHelpIconOp_FrameOverlay);
            if (result->flags & 0x80000U)
                CheckHelpIcon(8, 0, CheckHelpIconOp_FrameOverlay);
        }
        if (result->flags & kMediatedActionHint)
            CheckHelpIcon(1, 0, CheckHelpIconOp_FrameOverlay);
        if ((result->flags & 0x10000U) && (input->pressedPriority & 0x400U))
            result->flags |= 0x800U;
        if ((result->flags & 0x20000U) && (input->pressedPriority & 0x200U))
            result->flags |= 0x1000U;
        if ((result->flags & 0x40000U) && (input->pressedPriority & 4U))
            result->flags |= 0x2000U;
        if ((result->flags & 0x80000U) && (input->pressedPriority & 2U))
            result->flags |= 0x4000U;
        if ((result->flags & 0x100000U) && (input->pressedPriority & 0x800U))
            result->flags |= 0x8000U;
    }
    g_doublePressResult = 0;
    if (g_doublePressRequested) {
        if (input->pressedPriority & 0x200U) {
            if (g_doublePressFrames)
                g_doublePressResult = 1;
            g_doublePressFrames = 1;
        }
        if (g_doublePressFrames > 20)
            g_doublePressFrames = 0;
        if (g_doublePressFrames)
            ++g_doublePressFrames;
    } else {
        g_doublePressFrames = 0;
    }
    g_doublePressRequested = 0;
}
#else
// @verified - d
void btnButtonMediationCtrl_Callback(void *) {
    ButtonMediationControl *const control = g_buttonMediationControl;
    PadControlEntry *const pad = GetMapPadControlEntry();
    LegacyInputSlot *const input = ctrlGetPadInputBuffer0(pad);
    control->flags &= 0x80000000U;
    control->field114 = 0;
    control->field08 = 0;
    control->field124 = 0;
    for (std::uint32_t button = 0; button < 5; ++button) {
        control->results[button].name = __typeof__(control->results[button].name)();
        control->priorities[button] = __typeof__(control->priorities[button])();
    }
    if (control->count > 0) {
        ResolveButtonMediationRequests(control);
        std::memcpy(&control->input, input, sizeof(control->input));
    }
    control->count = 0;
    for (std::size_t rangeIndex5 = 0; rangeIndex5 < sizeof(control->requests) / sizeof((control->requests)[0]); ++rangeIndex5) {
        ButtonMediationRequest &request = (control->requests)[rangeIndex5];
        request.name = __typeof__(request.name)();
    }
    control->field08 = 0;
    for (std::size_t rangeIndex6 = 0; rangeIndex6 < sizeof(control->results) / sizeof((control->results)[0]); ++rangeIndex6) {
        ButtonMediationResult &result = (control->results)[rangeIndex6];

        if (!result.name)
            continue;
        if (result.flags & kMediatedButtonHint) {
            for (std::uint32_t button = 0; button < 4; ++button) {
                if (result.flags & (1U << (kMediatedBindingShift + button)))
                    CheckHelpIcon(kMediatedButtonHelpMasks[button], 0, CheckHelpIconOp_FrameOverlay);
            }
        }
        if (result.flags & kMediatedActionHint)
            CheckHelpIcon(1, 0, CheckHelpIconOp_FrameOverlay);
        for (std::uint32_t button = 0; button < 5; ++button) {
            if ((result.flags & (1U << (kMediatedBindingShift + button))) && (input->pressedPriority & kMediatedButtonPressMasks[button]))
                result.flags |= 1U << (kMediatedPressedShift + button);
        }
    }
    g_doublePressResult = 0;
    if (!g_doublePressRequested) {
        g_doublePressFrames = 0;
    } else {
        if (input->pressedPriority & 0x200U) {
            g_doublePressResult = g_doublePressFrames != 0;
            g_doublePressFrames = 1;
        }
        if (g_doublePressFrames > 20)
            g_doublePressFrames = 0;
        if (g_doublePressFrames != 0)
            g_doublePressFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_doublePressFrames) + 1U);
    }
    g_doublePressRequested = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t RegisterButtonMediation(shen_name name, const Vector3 *position, std::uint32_t flags) {
    ButtonMediationControl *const control = g_buttonMediationControl;
    if (control->count > 10)
        return -1;
    if (name == BUTTON_BTME)
        flags &= ~kMediatedButtonHint;
    ButtonMediationRequest &request = control->requests[control->count];
    ++control->count;
    request.flags = flags;
    request.name = name;
    if (position) {
        request.position.x = position->x;
        request.position.y = position->y;
        request.position.z = position->z;
    } else {
        Vector3 location = {};
        std::int32_t yaw = 0;
        if (ctrlGetRootNode()) {
            plymUpdatePlayerSearchLocation(&location);
            yaw = playerGetRotY();
        }
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&location);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::preTranslate(0.0f, 0.0f, -1.0f);
        matrix_stack::GetTranslation(&request.position);
        PopCurrentMatrix(1);
        request.flags |= kButtonRequestPlayerRelative;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t UnregisterButtonMediation(shen_name name) {
    ButtonMediationControl *const control = g_buttonMediationControl;
    ButtonMediationRequest *request = control->requests;
    for (std::int32_t index = 0; index < control->count; ++index, ++request) {
        if (request->name == name) {
            request->name = 0;
            request->flags = 0;
            return 0;
        }
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetMediatedButtonBindings(shen_name name) {
    const ButtonMediationResult *const result = FindButtonMediationResult(name);
    std::uint32_t buttons = 0;
    if (result) {
        if (result->flags & (1U << kMediatedBindingShift))
            buttons |= 0x400U;
        if (result->flags & (1U << (kMediatedBindingShift + 1U)))
            buttons |= 0x200U;
        if (result->flags & (1U << (kMediatedBindingShift + 2U)))
            buttons |= 4U;
        if (result->flags & (1U << (kMediatedBindingShift + 3U)))
            buttons |= 2U;
        if (result->flags & (1U << (kMediatedBindingShift + 4U)))
            buttons |= 0x800U;
    }
    return buttons;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t btnCheck(shen_name name) {
    const ButtonMediationResult *const result = FindButtonMediationResult(name);
    std::uint32_t buttons = 0;
    if (result) {
        if (result->flags & (1U << kMediatedPressedShift))
            buttons |= 0x400U;
        if (result->flags & (1U << (kMediatedPressedShift + 1U)))
            buttons |= 0x200U;
        if (result->flags & (1U << (kMediatedPressedShift + 2U)))
            buttons |= 4U;
        if (result->flags & (1U << (kMediatedPressedShift + 3U)))
            buttons |= 2U;
        if (result->flags & (1U << (kMediatedPressedShift + 4U)))
            buttons |= 0x800U;
    }
    return buttons;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
LegacyInputSlot *GetButtonMediationResultData(shen_name name) {
    ButtonMediationControl *const control = g_buttonMediationControl;
    return FindButtonMediationResult(name) ? &control->input : 0;
}
#endif

// @verified - d
std::uint32_t IsButtonMediationField124Set() {
    return g_buttonMediationControl->field124 != 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetBattleDataConfig(std::int32_t mode, shen_name name) {
    std::int32_t result = 0;
    switch (mode) {
    case 0: {
        std::uint32_t flags = 1U << (kMediatedBindingShift + 2U);
        if (name == BUTTON_BTBT)
            flags += kMediatedButtonHint;
        if (name == BUTTON_BTAD)
            flags = kMediatedButtonHint | (1U << kMediatedBindingShift);
        if (name == BUTTON_BTIL)
            flags = kMediatedButtonHint | (1U << (kMediatedBindingShift + 3U));
        if (name == BUTTON_BTSP)
            flags = kMediatedButtonHint | (1U << kMediatedBindingShift);
        result = RegisterButtonMediation(name, 0, flags);
        break;
    }
    case 1:
        result = GetMediatedButtonBindings(name);
        break;
    case 2:
        result = btnCheck(name);
        break;
    }
    return result;
}
#endif

// @verified - d
shen_name GetMediatedButtonOwner(std::uint32_t button) {
    ButtonMediationControl *const control = g_buttonMediationControl;
    return shen_name(button <= 4 ? control->priorities[button] : 0);
}

// @verified - d
std::uint32_t CheckMediatedDoublePress() {
    g_doublePressRequested = 1;
    return static_cast<std::uint32_t>(g_doublePressResult);
}

// @verified - d
void ResetButtonMediation(ButtonMediationControl *control) {
    control->flags = 0;
    control->count = 0;
    control->field110 = 0;
    control->field114 = 0;
    control->field08 = 0;
    control->field124 = 0;
    for (std::int32_t button = 0; button < 5; ++button) {
        control->results[button].name = 0;
        control->priorities[button] = 0;
    }
}

// @verified - d
void SelectButtonRequest(ButtonMediationControl *control) {
    ButtonMediationResult *const result = &control->results[control->currentButton];
    std::int32_t minimumAngle = 0x7fff;
    ButtonMediationRequest *selected = 0;
    Vector3 player;
    Vector3 local;
    Matrix4x4 playerTransform;
    Vector2s angles;
    plymUpdatePlayerSearchLocation(&player);
    const std::int32_t yaw = playerGetRotY();
    player.y += kButtonPlayerEyeHeight;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(yaw));
    matrix_stack::TranslateInverseVector(&player);
    matrix_stack::GetMatrix(&playerTransform);
    PopCurrentMatrix(1);
    matrix_stack::Matrix4x4_Push(0);
    for (std::int32_t index = 0; index < control->count; ++index) {
        ButtonMediationRequest &request = control->requests[index];
        if (!request.name || (request.flags & kButtonRequestRejected))
            continue;
        if (!((1 << (control->currentButton + kMediatedBindingShift)) & request.flags))
            continue;
        matrix_stack::LoadMatrix(&playerTransform);
        Matrix4x4::TransformPoint(request.position, local);
        Vector3_ToFixed16EulerAngles(local, reinterpret_cast<Vector3s &>(angles));
        if ((angles.y < 0 ? -angles.y : angles.y) < minimumAngle) {
            selected = &request;
            minimumAngle = static_cast<std::int16_t>(angles.y < 0 ? -angles.y : angles.y);
        }
    }
    PopCurrentMatrix(1);
    if (selected) {
        result->name = selected->name;
        result->flags = selected->flags;
    }
}

// @verified - d
void ResolveButtonMediationRequests(ButtonMediationControl *control) {
    FilterButtonRequests(control);
    PrioritizeButtonRequests(control);
    for (std::int32_t button = 0; button < 5; ++button) {
        control->currentButton = button;
        SelectButtonRequest(control);
    }
}

// @verified - d
void FilterButtonRequests(ButtonMediationControl *control) {
    std::int32_t slot;
    ButtonMediationRequest *request;
    bool mapRequest = false;
    for (slot = 0; slot < control->count; ++slot) {
        request = &control->requests[slot];
        if (!request->name)
            continue;
        request->flags = (request->flags & ~0x000f0000U) | kMediatedButtonHint;
        if (!(request->flags & kButtonRequestRejected)) {
            if (request->name == BUTTON_BTMK)
                mapRequest = true;
            if (request->flags & kButtonRequestSuppressTalk)
                control->flags |= kButtonRequestSuppressTalk;
        }
    }
    for (slot = 0; slot < control->count; ++slot) {
        request = &control->requests[slot];
        if (!request->name || (request->flags & kButtonRequestRejected))
            continue;
        switch (request->name) {
        case BUTTON_BTFT:
        case BUTTON_BTMT:
            if (control->flags & kButtonRequestSuppressTalk)
                request->flags |= kButtonRequestRejected;
            break;
        case BUTTON_BTDR:
            if (!ButtonRequestFacesPlayer(request))
                request->flags |= kButtonRequestRejected;
            if (!ButtonRequestInCamera(request))
                request->flags |= kButtonRequestRejected;
            break;
        case BUTTON_BTME:
        case BUTTON_BTSW:
            if (!reinterpret_cast<std::int32_t (*)(ButtonMediationRequest *)>(IsButtonSearchable)(request))
                request->flags |= kButtonRequestRejected;
            break;
        default:
            if (!ButtonRequestFacesPlayer(request))
                request->flags |= kButtonRequestRejected;
            break;
        }
        if (mapRequest && request->name != BUTTON_BTMK)
            request->flags |= kButtonRequestRejected;
        PadControlEntry *const pad = GetMapPadControlEntry();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (!IsPadControlEntryBound(pad) && (request->name != BUTTON_BTFT || !IsScenarioTalkInputCaptured()))
#else
        if (!IsPadControlEntryBound(pad) && (request->name != BUTTON_BTFT || !IsScenarioButtonAdmissionEnabled()))
#endif
            request->flags |= kButtonRequestRejected;
    }
}

// @verified - d
void PrioritizeButtonRequests(ButtonMediationControl *control) {
    std::uint32_t masks[5] = {0x10000, 0x20000, 0x40000, 0x80000, 0};
    std::uint32_t first[] = {BUTTON_BTMK, BUTTON_BTKK, BUTTON_BTBF, BUTTON_BTFT, BUTTON_BTBA, 0};
    std::uint32_t second[] = {BUTTON_BTMK, BUTTON_BTNV, BUTTON_BTTT, BUTTON_BTBF, BUTTON_BTCC, BUTTON_BTBB, 0};
    std::uint32_t third[] = {BUTTON_BTMK, BUTTON_BTPP, BUTTON_BTBF, BUTTON_BTBT, BUTTON_BTSD, BUTTON_BTMR, BUTTON_BTD2, BUTTON_BTLD, BUTTON_BTEV, BUTTON_BTDR,
        BUTTON_BTMP, BUTTON_BTBX, 0};
    std::uint32_t fourth[] = {BUTTON_BTMK, BUTTON_BTDD, BUTTON_BTBF, BUTTON_BTMT, BUTTON_BTMM, BUTTON_BTIF, BUTTON_BTIL, BUTTON_BTSW, BUTTON_BTBY, 0};
    std::uint32_t empty[] = {0};
    std::uint32_t firstFallback[] = {BUTTON_BTAD, BUTTON_BTSP, 0};
    std::uint32_t thirdFallback[2] = {BUTTON_BTME};
    std::int32_t index, slot;
    ButtonMediationRequest *request;
    if ((GetPersistentHelpIconFlags(0) & 0x200U) && !control->priorities[2]) {
        for (slot = 0; slot < control->count; ++slot) {
            request = &control->requests[slot];
            if (request->name == BUTTON_BTME) {
                if (request->flags & kButtonRequestRejected)
                    break;
                request->flags |= masks[2];
                control->priorities[2] = request->name;
                break;
            }
        }
    }
    if (helpIconItemEffectState > 0 && !control->priorities[3]) {
        for (slot = 0; slot < control->count; ++slot) {
            request = &control->requests[slot];
            if (request->name == BUTTON_BTSW) {
                if (request->flags & kButtonRequestRejected)
                    break;
                request->flags |= masks[3];
                control->priorities[3] = request->name;
                break;
            }
        }
    }
#define PRIORITY_PASS(list, button)                                                                                                                            \
    for (index = 0; !control->priorities[button] && list[index]; ++index) {                                                                                    \
        for (slot = 0; slot < control->count; ++slot) {                                                                                                        \
            request = &control->requests[slot];                                                                                                                \
            if (request->name == list[index]) {                                                                                                                \
                if (!(request->flags & kButtonRequestRejected)) {                                                                                              \
                    request->flags |= masks[button];                                                                                                           \
                    control->priorities[button] = request->name;                                                                                               \
                }                                                                                                                                              \
                break;                                                                                                                                         \
            }                                                                                                                                                  \
        }                                                                                                                                                      \
    }
#define EMPTY_PASS(list, button)                                                                                                                               \
    for (index = 0; !control->priorities[button] && list[index]; ++index) {                                                                                    \
        for (slot = 0; slot < control->count; ++slot) {                                                                                                        \
            request = &control->requests[slot];                                                                                                                \
            if (request->name == list[index]) {                                                                                                                \
                if (!(request->flags & kButtonRequestRejected) && control->priorities[0] != request->name && control->priorities[2] != request->name) {        \
                    request->flags |= masks[button];                                                                                                           \
                    control->priorities[button] = request->name;                                                                                               \
                }                                                                                                                                              \
                break;                                                                                                                                         \
            }                                                                                                                                                  \
        }                                                                                                                                                      \
    }
    PRIORITY_PASS(first, 0)
    PRIORITY_PASS(second, 1)
    PRIORITY_PASS(third, 2)
    PRIORITY_PASS(fourth, 3)
    EMPTY_PASS(empty, 0)
    EMPTY_PASS(empty, 2)
    PRIORITY_PASS(firstFallback, 0)
    PRIORITY_PASS(thirdFallback, 2)
#undef PRIORITY_PASS
#undef EMPTY_PASS
    for (slot = 0; slot < control->count; ++slot) {
        request = &control->requests[slot];
        if (control->priorities[0] != request->name && control->priorities[1] != request->name && control->priorities[2] != request->name &&
            control->priorities[3] != request->name)
            request->flags |= kButtonRequestRejected;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
ButtonMediationResult *FindButtonMediationResult(shen_name name) {
    ButtonMediationControl *const control = g_buttonMediationControl;
    for (std::int32_t index = 0; index < 5; ++index) {
        if (control->results[index].name == name)
            return &control->results[index];
    }
    return 0;
}
#endif

// @verified - d
bool ButtonRequestFacesPlayer(const ButtonMediationRequest *request) {
    bool admitted = !HasScenarioCharacterControl();
    if (!(request->flags & kButtonRequestPlayerRelative)) {
        Vector3 player, local;
        plymUpdatePlayerSearchLocation(&player);
        player.y += kButtonPlayerEyeHeight;
        const std::int32_t yaw = playerGetRotY();
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(yaw));
        matrix_stack::TranslateInverseVector(&player);
        Matrix4x4::TransformPoint(request->position, local);
        PopCurrentMatrix(1);
        if (!(0.0f > local.z))
            admitted = false;
    }
    return admitted;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsButtonRequestAdmitted() {
    return !HasScenarioCharacterControl();
}
#endif

// @verified - d
bool ButtonRequestInCamera(const ButtonMediationRequest *request) {
    bool visible = true;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(camCtrlGetProjectionMatrix(0));
    Vector3 local;
    Matrix4x4::TransformPoint(request->position, local);
    PopCurrentMatrix(1);
    if (!(0.0f > local.z))
        visible = false;
    else {
        const float limit = camCtrlGetHalfVerticalFovTangent() * -local.z;
        if (0.0f > local.x) {
            if (limit > -local.x)
                return visible;
        } else if (limit > local.x)
            return visible;
        visible = false;
    }
    return visible;
}

// @verified - d
std::int32_t IsButtonSearchable() {
    return plymIsSearchable() != 0;
}

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool IsScenarioButtonAdmissionEnabled() {
    std::uint32_t flags;
    std::memcpy(&flags, reinterpret_cast<const std::byte *>(g_scenarioEventContext) + kScenarioContextFlagsOffset, sizeof(flags));
    return ((flags >> 1U) & 1U) != 0;
}
#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_GetBattleDataConfig(std::int32_t mode, std::uint32_t name) {
    return shenmue::s2::GetBattleDataConfig(mode, shenmue::shen_name(name));
}
#endif

#endif
