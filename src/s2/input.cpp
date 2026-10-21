#include "s2/input.hpp"
#include "s2/warning.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input_bindings.hpp"
#include "s2/d3t_renderer.hpp"

#include "s2/hlib.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <bit>
#include <array>
#include <cstdint>
#include <cmath>

#if defined(_WIN32)
#include <Windows.h>
#include <Xinput.h>
#endif

namespace shenmue::s2 {

std::array<LegacyInputSlot, 5> g_inputBuffer{};

// @verified - p
std::int32_t g_mouseLookInputActive = 1;
bool HasPressedCancel() {
    return (g_inputBuffer[0].pressedPriority & 0x200U) != 0;
}

std::uint32_t EVT_GetControllerInput(std::uint8_t userIndex, std::int32_t selector) {
    const LegacyInputSlot &input = g_inputBuffer[userIndex & 3U];
    switch (selector) {
    case 0:
        return input.held;
    case 1:
        return input.pressedPriority;
    case 2:
        return input.repeatedPresses;
    case 3:
        return input.analog0c;
    case 4:
        return input.analog0d;
    case 5:
        return input.analog0e;
    case 6:
        return input.analog0f;
    case 7:
        return input.newPresses;
    case 8:
        return input.analog10;
    case 9:
        return input.analog11;
    case 10:
        return input.connectionState;
    case 11:
        return input.connectionState == 0;
    case 12:
        return input.deviceState;
    case 13:
        return input.deviceState == 0;
    default:
        return 0;
    }
}

// @verified - p
std::uint64_t GetCurrentControlInput(std::int32_t selector) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const LegacyInputSlot &input = *ctrlGetPadInputBuffer0(static_cast<PadControlEntry *>(control->padControl));
    switch (selector) {
    case 0:
        return input.held;
    case 1:
        return input.pressedPriority;
    case 2:
        return input.repeatedPresses;
    case 3:
        return input.analog0c;
    case 4:
        return input.analog0d;
    case 5:
        return input.analog0e;
    case 6:
        return input.analog0f;
    case 7:
        return input.newPresses;
    case 8:
        return input.analog10;
    case 9:
        return input.analog11;
    default:
        return 0;
    }
}

// @verified - p
std::byte *GetControllerEventWork(std::uint8_t userIndex) {
    static std::array<std::array<std::byte, 0x10>, 4> work{};
    return work[userIndex & 3U].data();
}

namespace {

HLib::Task *padControlTask{};
bool clearPrePadInput{};

std::array<PhysicalControllerState, 4> physicalControllers{};
std::int32_t selectedPhysicalControllerIndex = -1;
std::int32_t softResetInputEnabled{};
std::int32_t softResetInputState{};
std::int32_t softResetInputCounter{};
std::array<KeyState, 261> windowKeys{};
std::int32_t keyboardActive = 1;
std::int32_t xInputReserved{};
std::int32_t forceXInputProcessing{};
std::int32_t forceXInputAnalogProcessing{};
std::int32_t dpadAnalogMode{};
Vector2 dpadAnalog{};
std::int32_t customInputHandler{};
std::int32_t currentKeyStateChanged{};
std::int32_t previousKeyStateChanged{};
std::uint32_t lastReleasedKey{};
Vector2 mouseDelta{};
Vector2 previousMouseDelta{};
std::array<Vector2, 10> mouseHistory{};
std::int32_t mouseHistoryCount{};
std::int32_t inputWindowWidth{};
std::int32_t inputWindowHeight{};
WindowUIInput windowUIInput{};
#if defined(_WIN32)
HWND inputWindow{};

// @verified - p
std::uint32_t MapVirtualKeyWithLayout(std::uint32_t key, HKL source, HKL target) {
    if (key - 96U <= 9U)
        return key;
    UINT scan = MapVirtualKeyExA(key, MAPVK_VK_TO_VSC, source);
    switch (key) {
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x2d:
    case 0x2e:
    case 0x6f:
    case 0x90:
        scan |= 0x100U;
        break;
    default:
        break;
    }
    if (scan) {
        const UINT mapped = MapVirtualKeyExA(scan, MAPVK_VSC_TO_VK_EX, target);
        if (mapped)
            return mapped;
    }
    return key;
}
#endif




struct InputSelectionState {
    std::int32_t field00;
    std::int32_t connectedDeviceCount;
    std::array<std::int32_t, 5> confirmEligible;
    std::int32_t selectionSuspended;
    std::array<std::int32_t, 5> startEligible;
    std::int32_t startInputReserved;
    std::array<std::uint16_t, 5> previousDeviceStates;
    std::uint16_t reserved42;
    std::int32_t forceDisconnected;
    std::int32_t field48;
    std::uint16_t injectedButtons;
    std::uint16_t reserved4e;
};
static_assert(sizeof(InputSelectionState) == 0x50);
static_assert(offsetof(InputSelectionState, confirmEligible) == 0x08);
static_assert(offsetof(InputSelectionState, startEligible) == 0x20);
static_assert(offsetof(InputSelectionState, startInputReserved) == 0x34);
static_assert(offsetof(InputSelectionState, injectedButtons) == 0x4c);
InputSelectionState inputSelection{};
bool padPollConnected = false; // GControllers[1].buttons latch behind g_LastXInputError
std::int32_t selectedInputDevice = 4;

bool harnessStickPending{};
std::int16_t harnessStickX{};
std::int16_t harnessStickY{};

ButtonMediationControl buttonMediation{};
std::uint32_t doublePressRequested{};
std::uint32_t doublePressResult{};
std::int32_t doublePressFrames{};

constexpr std::uint32_t ButtonName(const char (&name)[5]) {
    return make_shen_name(name[0], name[1], name[2], name[3]);
}

Vector3 ButtonPlayerLocal(const Vector3 &point) {
    Vector3 location, local;
    plymUpdatePlayerSearchLocation(&location);
    location.y += 1.2f;
    const std::uint16_t yaw = static_cast<std::uint16_t>(-playerGetRotY());
    Matrix4x4_PushActiveTransform();
    ApplyShearToPrePassMatrix(FixedAngleSin(yaw), FixedAngleCos(yaw));
    Matrix4x4::preTranslate(-location.x, -location.y, -location.z);
    Matrix4x4::TransformPoint(point, local);
    Matrix4x4::Pop();
    return local;
}

bool ButtonRequestFacesPlayer(const ButtonMediationRequest &request) {
    const bool admitted = !HasScenarioCharacterControl();
    if ((request.flags & ButtonMediationPlayerRelative) == 0 && ButtonPlayerLocal(request.position).z >= 0.0f)
        return false;
    return admitted;
}

bool ButtonRequestInCamera(const ButtonMediationRequest &request) {
    Vector3 local;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(camCtrlGetProjectionMatrix(nullptr));
    Matrix4x4::TransformPoint(request.position, local);
    Matrix4x4::Pop();
    return local.z < 0.0f && std::fabs(local.x) < -(camCtrlGetHalfVerticalFovTangent() * local.z);
}

void FilterButtonRequests() {
    bool mapRequest = false;
    for (std::int32_t index = 0; index < buttonMediation.count; ++index) {
        auto &request = buttonMediation.requests[index];
        if (!request.name.name)
            continue;

        request.flags = (request.flags & ~buttonMediationFaceBindings) | ButtonMediationShowButtonHint;
        if ((request.flags & ButtonMediationRejected) == 0) {
            mapRequest |= request.name.name == ButtonName("BTMK");
            buttonMediation.flags |= request.flags & ButtonMediationSuppressTalk;
        }
    }
    for (std::int32_t index = 0; index < buttonMediation.count; ++index) {
        auto &request = buttonMediation.requests[index];
        const std::uint32_t name = request.name.name;
        if (!name || (request.flags & ButtonMediationRejected) != 0)
            continue;
        bool admitted = true;
        switch (name) {
        case SHEN_NAME(BTDR):
            if (!ButtonRequestFacesPlayer(request))
                request.flags |= ButtonMediationRejected;
            if (!ButtonRequestInCamera(request))
                request.flags |= ButtonMediationRejected;
            break;
        case ButtonName("BTME"):
        case ButtonName("BTMP"):
        case ButtonName("BTSW"):
            admitted = plymIsSearchable();
            break;
        case ButtonName("BTFT"):
        case ButtonName("BTMT"):
            admitted = (buttonMediation.flags & ButtonMediationSuppressTalk) == 0;
            break;
        case ButtonName("BTMM"):
            admitted = !IsCtrlSpeakIconInactive() || ButtonRequestFacesPlayer(request);
            break;
        default:
            admitted = ButtonRequestFacesPlayer(request);
            break;
        }
        if (mapRequest && name != ButtonName("BTMK"))
            admitted = false;
        const PadControlEntry *const pad = ctrlGetPadData();
        if ((pad->flags & 2U) == 0 && (name != ButtonName("BTMM") || IsCtrlSpeakIconInactive()) &&
            (name != ButtonName("BTFT") || !IsScenarioButtonAdmissionEnabled()))
            admitted = false;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1016)) {
            switch (name) {
            case ButtonName("BTFT"):
            case ButtonName("BTME"):
            case ButtonName("BTMK"):
            case ButtonName("BTSP"):
            case ButtonName("BTDR"):
            case ButtonName("BTMT"):
            case ButtonName("BTNV"):
            case ButtonName("BTSW"):
                admitted = false;
                break;
            default:
                break;
            }
        }
        if (!admitted)
            request.flags |= ButtonMediationRejected;
    }
}

void AssignButtonPriority(std::size_t button, std::uint32_t name) {
    if (buttonMediation.priorities[button].name)
        return;

    for (std::int32_t index = 0; index < buttonMediation.count; ++index) {
        auto &request = buttonMediation.requests[index];
        if (request.name.name != name)
            continue;


        if ((request.flags & ButtonMediationRejected) == 0) {
            request.flags |= 1U << (buttonMediationBindingShift + button);
            buttonMediation.priorities[button] = request.name;
        }
        return;
    }
}

void PrioritizeButtonRequests() {
    constexpr std::array first{ButtonName("BTMK"), ButtonName("BTKK"), ButtonName("BTBF"), ButtonName("BTFT"), ButtonName("BTBA")};
    constexpr std::array second{ButtonName("BTMK"), ButtonName("BTNV"), ButtonName("BTTT"), ButtonName("BTBF"), ButtonName("BTCC"), ButtonName("BTBB")};
    constexpr std::array third{ButtonName("BTMK"), ButtonName("BTPP"), ButtonName("BTBF"), ButtonName("BTBT"), ButtonName("BTSD"), ButtonName("BTMR"),
        ButtonName("BTD2"), ButtonName("BTLD"), ButtonName("BTEV"), ButtonName("BTDR"), ButtonName("BTMP"), ButtonName("BTBX")};
    constexpr std::array fourth{ButtonName("BTMK"), ButtonName("BTAA"), ButtonName("BTDD"), ButtonName("BTBF"), ButtonName("BTMT"), ButtonName("BTMM"),
        ButtonName("BTIF"), ButtonName("BTIL"), ButtonName("BTSW"), ButtonName("BTBY")};
    if ((GetPersistentHelpIconFlags(0) & 0x200U) != 0)
        AssignButtonPriority(2, ButtonName("BTME"));
    if (GetHelpIconItemEffectState() > 0)
        AssignButtonPriority(3, ButtonName("BTSW"));
    for (auto name : first)
        AssignButtonPriority(0, name);
    for (auto name : second)
        AssignButtonPriority(1, name);
    for (auto name : third)
        AssignButtonPriority(2, name);
    for (auto name : fourth)
        AssignButtonPriority(3, name);
    // The two intervening fallback lists in ConfigureButtonPriorities are explicitly empty.
    AssignButtonPriority(0, ButtonName("BTAD"));
    AssignButtonPriority(0, ButtonName("BTSP"));
    AssignButtonPriority(2, ButtonName("BTME"));
    for (std::int32_t index = 0; index < buttonMediation.count; ++index) {
        auto &request = buttonMediation.requests[index];
        bool selected = false;
        for (std::size_t button = 0; button < 4; ++button)
            selected |= buttonMediation.priorities[button].name == request.name.name;
        if (!selected)
            request.flags |= ButtonMediationRejected;
    }
}

void SelectButtonRequest(std::size_t button) {
    ButtonMediationRequest *selected = nullptr;
    std::int32_t bestAngle = 0x7fff;
    Vector3 location;
    Matrix4x4 playerTransform;
    plymUpdatePlayerSearchLocation(&location);
    const std::uint16_t yaw = static_cast<std::uint16_t>(-playerGetRotY());
    location.y += 1.2f;
    Matrix4x4_PushActiveTransform();
    ApplyShearToPrePassMatrix(FixedAngleSin(yaw), FixedAngleCos(yaw));
    Matrix4x4::preTranslate(-location.x, -location.y, -location.z);
    Matrix4x4::GetTransformMatrix(&playerTransform);
    Matrix4x4::Pop();
    Matrix4x4_Push();
    for (std::int32_t index = 0; index < buttonMediation.count; ++index) {
        ButtonMediationRequest &request = buttonMediation.requests[index];
        if (!request.name.name || (request.flags & ButtonMediationRejected) != 0 || (request.flags & (1U << (buttonMediationBindingShift + button))) == 0)
            continue;
        Vector3 local;
        Matrix4x4::SetActiveTransform(&playerTransform);
        Matrix4x4::TransformPoint(request.position, local);
        const std::int16_t angleYaw = static_cast<std::int16_t>(FixedAtan2(local.x, local.z) + 0x8000U);
        const std::int32_t angle = std::abs(static_cast<std::int32_t>(angleYaw));
        if (angle < bestAngle) {
            bestAngle = angle;
            selected = &request;
        }
    }
    Matrix4x4::Pop();
    if (selected)
        buttonMediation.results[button] = {.name = selected->name, .flags = selected->flags};
}

void btnButtonMediationCtrl_Callback(void *) {
    PadControlEntry *const pad = ctrlGetPadData();
    LegacyInputSlot *const padInput = ctrlGetPadInputBuffer0(pad);
    ButtonMediationControl &control = buttonMediation;
    control.flags &= 0x80000000U;
    control.field114 = 0;
    control.field08 = 0;
    control.field128 = 0;
    for (std::size_t button = 0; button < control.results.size(); ++button) {
        control.results[button].name = {};
        control.priorities[button] = {};
    }
    if (control.count > 0) {
        FilterButtonRequests();
        PrioritizeButtonRequests();
        for (std::size_t button = 0; button < control.results.size(); ++button) {
            control.currentButton = static_cast<std::int32_t>(button);
            SelectButtonRequest(button);
        }
        control.input = *padInput;
    }
    control.count = 0;
    for (auto &request : control.requests)
        request.name = {};
    control.field08 = 0;
    constexpr std::array<std::uint16_t, 5> masks{0x400, 0x200, 4, 2, 0x800};
    constexpr std::array<std::uint32_t, 4> helpMasks{4, 2, 16, 8};
    const std::uint16_t presses = padInput->pressedPriority;
    for (ButtonMediationResult &result : control.results) {
        if (!result.name.name)
            continue;
        if ((result.flags & ButtonMediationShowButtonHint) != 0) {
            for (std::size_t button = 0; button < helpMasks.size(); ++button)
                if ((result.flags & (1U << (buttonMediationBindingShift + button))) != 0)
                    CheckHelpIcon(helpMasks[button], 0, CheckHelpIconOp::FrameOverlay);
        }
        if ((result.flags & ButtonMediationShowActionHint) != 0)
            CheckHelpIcon(1, 0, CheckHelpIconOp::FrameOverlay);
        for (std::size_t button = 0; button < masks.size(); ++button)
            if ((result.flags & (1U << (buttonMediationBindingShift + button))) != 0 && (presses & masks[button]) != 0)
                result.flags |= 1U << (buttonMediationPressedShift + button);
    }
    doublePressResult = 0;
    if (!doublePressRequested) {
        doublePressFrames = 0;
    } else if ((presses & 0x200U) != 0) {
        doublePressResult = doublePressFrames != 0;
        doublePressFrames = 2;
    } else if (doublePressFrames > 20) {
        doublePressFrames = 0;
    } else if (doublePressFrames != 0) {
        ++doublePressFrames;
    }
    doublePressRequested = 0;
}

#if defined(_WIN32)
void HandleControllerInput(std::uint32_t controllerIndex) {
    if (keyboardActive != 1)
        return;
    PhysicalControllerState &controller = physicalControllers[controllerIndex];
    controller.buttons = 0;
    controller.leftTrigger = controller.rightTrigger = 0;
    controller.thumbLX = controller.thumbLY = controller.thumbRX = controller.thumbRY = 0;
    controller.normalizedSticks = {};
    const std::int32_t type = GetCurrentUIInputType();
    for (std::int32_t action = 0; action < 8; ++action) {
        std::uint32_t key = 0xffffffffU;
        bool pressed = false;
        if (type <= 1) {
            if (action == 0) {
                pressed = windowKeys[13].pressed || windowKeys[32].pressed ||
                    (!IsHelpOverlayVisible(UIElement::GameHudWatch) && IsHelpOverlayVisible(UIElement::PressStart) && windowKeys[256].pressed);
            } else
                key = GetMenuKeyMapping(action);
        } else if (type == static_cast<std::int32_t>(InputType::Vehicle))
            key = GetGameKeyMapping(action + 32);
        else if (type == static_cast<std::int32_t>(InputType::Qte2))
            key = GetQteKeyMapping(action);
        else if (type == static_cast<std::int32_t>(InputType::Qte))
            key = GetGameKeyMapping(action < 4 ? action + 18 : action);
        else if (type == 2 || type == static_cast<std::int32_t>(InputType::Fight))
            key = GetGameKeyMapping(type == static_cast<std::int32_t>(InputType::Fight) && action < 4 ? action + 22 : action);
        if (pressed || (key < windowKeys.size() && windowKeys[key].pressed))
            controller.buttons |= keyboardControllerButtonMasks[action];
    }
    for (std::int32_t action = 14; action < 18; ++action) {
        std::uint32_t key = 0xffffffffU;
        bool pressed = false;
        if (type <= 1 || IsHelpOverlayVisible(UIElement::GameMenu)) {
            key = GetMenuKeyMapping(action);
            if (type <= 1 && !IsHelpOverlayVisible(UIElement::GameMenu))
                pressed = windowKeys[keyboardDirectionKeys[action - 14]].pressed != 0;
        } else if (type == static_cast<std::int32_t>(InputType::Vehicle))
            key = GetGameKeyMapping(action + 32);
        else if (type >= 2 && type <= 4)
            key = GetGameKeyMapping(action);
        if (pressed || (key < windowKeys.size() && windowKeys[key].pressed))
            controller.buttons |= 1U << (action - 14);
    }
    bool lookFromKeys = false;
    for (std::int32_t action = 10; action < 14; ++action) {
        const std::uint32_t key = type <= 1 ? GetMenuKeyMapping(action) : GetGameKeyMapping(action);
        if ((type <= 1 || type == 2 || type == static_cast<std::int32_t>(InputType::Fight)) && key < windowKeys.size() && windowKeys[key].pressed) {
            lookFromKeys = true;
            controller.normalizedSticks[action < 12 ? 3 : 2] = keyboardLookAxisValues[action - 10];
        }
    }
    g_mouseLookInputActive = 0;
    if (type == static_cast<std::int32_t>(InputType::Qte2)) {
        for (std::int32_t action = 14; action < 18; ++action) {
            const std::uint32_t key = GetGameKeyMapping(action);
            const std::uint32_t arrow = keyboardDirectionKeys[action - 14];
            if (windowKeys[arrow].pressed || (key < windowKeys.size() && windowKeys[key].pressed))
                controller.buttons |= 1U << (action - 14);
        }
    } else if (!lookFromKeys && type == 2 && windowKeys[257].pressed) {
        controller.normalizedSticks.z = mouseDelta.x;
        controller.normalizedSticks.w = -mouseDelta.y;
        g_mouseLookInputActive = 1;
    }
    if (type <= 3 || type == static_cast<std::int32_t>(InputType::Vehicle)) {
        const std::uint32_t left = type <= 1 ? GetMenuKeyMapping(8) : GetGameKeyMapping(type == 6 ? 40 : 8);
        const std::uint32_t right = type <= 1 ? GetMenuKeyMapping(9) : GetGameKeyMapping(type == 6 ? 41 : 9);
        if (left < windowKeys.size() && windowKeys[left].pressed)
            controller.leftTrigger = 255;
        if (right < windowKeys.size() && windowKeys[right].pressed)
            controller.rightTrigger = 255;
    }
}

std::int32_t suppressStartFrames = 0;

std::uint32_t TranslateXInputButtons(std::uint16_t buttons) {
    std::uint32_t translated = 0;
    if (buttons & 0x0001U)
        translated |= 0x10U;
    if (buttons & 0x0002U)
        translated |= 0x20U;
    if (buttons & 0x0004U)
        translated |= 0x40U;
    if (buttons & 0x0008U)
        translated |= 0x80U;
    if (buttons & 0x1000U)
        translated |= 0x000004U;
    if (buttons & 0x2000U)
        translated |= 0x000002U;
    if (buttons & 0x4000U)
        translated |= 0x000400U;
    if (buttons & 0x8000U)
        translated |= 0x000200U;
    if (buttons & 0x0010U)
        translated |= 0x000008U;
    if (buttons & 0x0020U)
        translated |= 0x100000U;
    if (buttons & 0x0200U)
        translated |= 0x040000U;
    if (buttons & 0x0100U)
        translated |= 0x080000U;
    if (buttons & 0x0040U)
        translated |= 0x200000U;
    if (buttons & 0x0080U)
        translated |= 0x400000U;
    return translated;
}

std::int16_t NormalizeStick(std::int16_t value, float sign) {
    const float normalized = (static_cast<float>(value) + 0.5f) / 32767.5f;
    if (std::fabs(normalized) < 0.25f)
        return 0;
    return static_cast<std::int16_t>(normalized * 127.0f * sign);
}

std::uint16_t TranslateLegacyHeld(std::uint32_t buttons, bool freeInputType) {
    std::uint16_t held = 0;
    held |= static_cast<std::uint16_t>((buttons & 0x80U) << 8U);
    held |= static_cast<std::uint16_t>((buttons & 0x40U) << 8U);
    held |= static_cast<std::uint16_t>((buttons & 0x20U) << 8U);
    held |= static_cast<std::uint16_t>((buttons & 0x10U) << 8U);
    held |= static_cast<std::uint16_t>((buttons & 0x08U) << 8U);
    if (buttons & 0x04U)
        held |= freeInputType ? 0x0200U : 0x0400U;
    if (buttons & 0x02U)
        held |= freeInputType ? 0x0400U : 0x0200U;
    held |= static_cast<std::uint16_t>((buttons >> 14U) & 0x0100U);
    held |= static_cast<std::uint16_t>((buttons >> 16U) & 0x0020U);
    held |= static_cast<std::uint16_t>((buttons >> 15U) & 0x0010U);
    held |= static_cast<std::uint16_t>((buttons >> 15U) & 0x0008U);
    held |= static_cast<std::uint16_t>((buttons >> 8U) & 0x0004U);
    held |= static_cast<std::uint16_t>((buttons >> 8U) & 0x0002U);
    held |= static_cast<std::uint16_t>((buttons >> 20U) & 0x0001U);
    return held;
}
#endif

// @verified - p
void ResetProcessedInput(LegacyInputSlot &input) {
    input.previousHeld = input.held;
    input.unknown02 = 0;
    input.held = 0;
    input.pressedPriority = 0;
    input.repeatedPresses = 0;
    input.analog0c = 0;
    input.analog0d = 0;
    input.analog0e = 0x80;
    input.analog0f = 0x80;
    input.connectionState = 0;
}

// @verified - p
void PrePadControl_Callback(void *) {
    if (!clearPrePadInput)
        return;
    clearPrePadInput = false;
    LegacyInputSlot &input = g_inputBuffer[0];
    input.held = 0x0200;
    input.previousHeld = 0x0200;
    input.pressedPriority = 0x0200;
    input.repeatedPresses = 0x0200;
    input.analog0c = 0;
    input.analog0d = 0;
    input.analog0e = 0x80;
    input.analog0f = 0x80;
    input.analog10 = 0;
    input.analog11 = 0;
}

void PadControl_Callback(void *taskData) {
    auto *const control = static_cast<PadControlTaskData *>(taskData);
    if (!control)
        return;
    for (PadControlEntry &entry : control->entries) {
        if ((entry.flags & 1U) == 0)
            continue;
        if ((entry.flags & 2U) != 0) {
            entry.input = g_inputBuffer[0];
        } else {
            ResetProcessedInput(entry.input);
        }
    }
}

void KeyRepeat_Callback(void *taskData) {
    auto *const repeat = static_cast<KeyRepeatTaskData *>(taskData);
    if (!repeat || repeat->inputIndex >= g_inputBuffer.size()) {
        return;
    }

    LegacyInputSlot &input = g_inputBuffer[repeat->inputIndex];
    std::uint16_t repeated = input.pressedPriority;
#if SHENMUE_IS_PC
    const float delta = HLib::GetScaledDeltaTick();
#endif
    for (std::size_t index = 0; index < repeatMasks.size(); ++index) {
        const std::uint16_t mask = repeatMasks[index];
        if ((input.held & mask) == 0)
            continue;
        if ((input.pressedPriority & mask) != 0) {
#if SHENMUE_IS_PC
            repeat->counters[index] = 15.0f;
#else
            repeat->counters[index] = 15;
#endif
            continue;
        }
#if SHENMUE_IS_PC
        repeat->counters[index] -= delta;
        if (repeat->counters[index] < 0.0f) {
            repeat->counters[index] = 0.0f;
#else
        --repeat->counters[index];
        if (repeat->counters[index] < 0) {
            repeat->counters[index] = 0;
#endif
            repeated |= mask;
        }
    }
    input.repeatedPresses = repeated;
}

} // namespace

// @verified - p
std::uint64_t GetIsKeyboardActive() {
    return static_cast<std::uint32_t>(keyboardActive);
}

// @verified - p
std::uint32_t MapInputKeyToCurrentLayout(std::uint32_t key) {
#if defined(_WIN32)
    return MapVirtualKeyWithLayout(key, reinterpret_cast<HKL>(0x8090809), GetKeyboardLayout(0));
#else
    return key;
#endif
}

void InitializeWindowInput(std::uint32_t width, std::uint32_t height) {
    ClearWindowInput();
    inputWindowWidth = static_cast<std::int32_t>(width);
    inputWindowHeight = static_cast<std::int32_t>(height);
}

void ClearWindowInput() {
    customInputHandler = 0;
    windowUIInput = {};
    for (KeyState &key : windowKeys)
        key = {};
    previousMouseDelta = {};
    mouseHistory.fill({});
    mouseDelta = {};
    mouseHistoryCount = 0;
}

// @verified - p
void ResetInputHandler() {
    previousKeyStateChanged = currentKeyStateChanged;
    customInputHandler = 0;
    currentKeyStateChanged = 0;
    for (KeyState &key : windowKeys) {
        key.released = 0;
        key.toggled = 0;
    }
}
std::uint32_t GetLastReleasedWindowKey() noexcept {
    return lastReleasedKey;
}
bool HasWindowInputStateChanged() noexcept {
    return customInputHandler != 0 || currentKeyStateChanged != 0;
}

bool IsWindowKeyBindable(std::int32_t action, std::uint32_t key) noexcept {
    // InitInputMapping43A500 and the QTE-only arrow-key exclusion433E00.
    static constexpr auto valid = [] {
        std::array<bool, 261> table{};
        constexpr std::array<std::uint16_t, 55> keys{
            KeyBackspace, KeyTab, KeyEnter, KeyShift, KeyControl, KeyAlt, KeyCapsLock, KeySpace, KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6, KeyF7, KeyF8,
            KeyF9, KeyF11, KeyF12, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', KeyLeftShift, KeyRightShift, KeyLeftControl, KeyRightControl, KeyLeftAlt,
            KeyRightAlt, KeyOem1, KeyOemPlus, KeyOemComma, KeyOemMinus, KeyOemPeriod, KeyOem2, KeyOem3, KeyOem3, KeyOem5, KeyOem6, KeyOem7, KeyOem8, KeyUp,
            KeyDown, KeyLeft, KeyRight, KeyMouse1, KeyMouse2, KeyMouse3, KeyMouse4};
        for (const auto key : keys)
            table[key] = true;
        for (std::uint32_t key = 65; key < 91; ++key)
            table[key] = true;
        for (std::uint32_t key = 96; key < 152; ++key)
            table[key] = true;
        for (const auto key : {186U, 192U, 222U})
            table[key] = false;
        return table;
    }();
    if (static_cast<std::uint32_t>(action - 18) <= 3U && key >= 37 && key <= 40)
        return false;
    return key < valid.size() && valid[key];
}

// @verified - p
bool UpdateAndGetMouseIsMoving() {
    float distance = 0.0f;
    for (std::int32_t index = 1; index < mouseHistoryCount; ++index) {
        const Vector2 &current = mouseHistory[static_cast<std::size_t>(index)];
        const Vector2 &previous = mouseHistory[static_cast<std::size_t>(index - 1)];
        const float dx = current.x - previous.x;
        const float dy = current.y - previous.y;
        distance += std::sqrt(dy * dy + dx * dx);
    }
    if (distance > 0.1f)
        return true;
    for (std::size_t index = 256; index < windowKeys.size(); ++index) {
        const KeyState &key = windowKeys[index];
        if (key.pressed || key.released || std::bit_cast<float>(key.toggled) != 0.0f)
            return true;
    }
    return false;
}

// @verified - p
void SetXInputFlags() {
    xInputReserved = 1;
    forceXInputProcessing = 1;
}

void SetXInputDigitalProcessing(std::int32_t enabled) {
    forceXInputProcessing = enabled;
}

// @verified - p
void SetXInputAnalogProcessing(std::int32_t enabled) {
    forceXInputAnalogProcessing = enabled;
}

// @verified - p
std::uint64_t PostProcessControllerState(std::uint32_t controllerIndex) {
    PhysicalControllerState &controller = physicalControllers[controllerIndex];
    Vector4 &sticks = controller.normalizedSticks;
    if (forceXInputProcessing == 0) {
        if (sticks.x < -0.5f)
            controller.buttons |= 4U;
        else if (sticks.x > 0.5f)
            controller.buttons |= 8U;
        if (sticks.y < -0.5f)
            controller.buttons |= 2U;
        else if (sticks.y > 0.5f)
            controller.buttons |= 1U;
        sticks.x = 0.0f;
        sticks.y = 0.0f;
        return 0;
    }
    if (sticks.z == 0.0f && sticks.w == 0.0f) {
        sticks.z = sticks.x;
        sticks.w = sticks.y;
    }
    if (sticks.x != 0.0f || sticks.y != 0.0f || sticks.w != 0.0f || customInputHandler == 1)
        dpadAnalogMode = 0;
    else if (controller.buttons & 15U)
        dpadAnalogMode = 1;
    std::uint32_t result = static_cast<std::uint32_t>(dpadAnalogMode);
    if (forceXInputAnalogProcessing == 1 && result == 1) {
        result = controller.buttons;
        if (result & 4U) {
            dpadAnalog.x -= 0.061999999f;
            if (dpadAnalog.x < -1.0f)
                dpadAnalog.x = -1.0f;
            result &= ~4U;
            controller.buttons = result;
        }
        if (result & 8U) {
            dpadAnalog.x += 0.061999999f;
            if (dpadAnalog.x > 1.0f)
                dpadAnalog.x = 1.0f;
            result &= ~8U;
            controller.buttons = result;
        }
        if (result & 1U) {
            dpadAnalog.y += 0.061999999f;
            if (dpadAnalog.y > 1.0f)
                dpadAnalog.y = 1.0f;
            result &= ~1U;
            controller.buttons = result;
        }
        if (result & 2U) {
            dpadAnalog.y -= 0.061999999f;
            if (dpadAnalog.y < -1.0f)
                dpadAnalog.y = -1.0f;
            result &= ~2U;
            controller.buttons = result;
        }
        sticks.z = dpadAnalog.x;
        sticks.w = dpadAnalog.y;
    } else if (result == 0) {
        dpadAnalog = {};
    }
    return result;
}

// @verified - p
void DetectActiveInputDevice(std::uint32_t controllerIndex) {
    const PhysicalControllerState &controller = physicalControllers[controllerIndex];
    if (keyboardActive == 1) {
        const Vector4 &sticks = controller.normalizedSticks;
        if (controller.active == 1 &&
            (controller.buttons || sticks.x != 0.0f || sticks.y != 0.0f || sticks.z != 0.0f || sticks.w != 0.0f || controller.leftTrigger ||
                controller.rightTrigger)) {
            keyboardActive = 0;
            ClearWindowInput();
        }
    } else if (keyboardActive == 0 && (customInputHandler || UpdateAndGetMouseIsMoving())) {
        keyboardActive = 1;
    }
}

// @verified - p
std::uintptr_t SetKeyPressed(std::uint32_t key) {
#if defined(_WIN32)
    key = MapVirtualKeyWithLayout(key, GetKeyboardLayout(0), reinterpret_cast<HKL>(0x8090809));
#endif
    if (key >= windowKeys.size())
        return key;
    windowKeys[key].pressed = 1;
    return reinterpret_cast<std::uintptr_t>(windowKeys.data());
}

// @verified - p
std::uint64_t SetKeyReleased(std::uint32_t key) {
#if defined(_WIN32)
    key = MapVirtualKeyWithLayout(key, GetKeyboardLayout(0), reinterpret_cast<HKL>(0x8090809));
#endif
    if (key < windowKeys.size() && windowKeys[key].pressed == 1) {
        windowKeys[key].pressed = 0;
        windowKeys[key].released = 1;
        currentKeyStateChanged = 1;
        lastReleasedKey = key;
    }
    return key;
}

void HandleWindowInputMessage(void *window, std::uint32_t message, std::uint64_t wParam, std::int64_t lParam) {
#if defined(_WIN32)
    if (!inputWindow && window) {
        inputWindow = static_cast<HWND>(window);
        SetTimer(inputWindow, 50, 100, nullptr);
    }
    if (message == WM_INPUT) {
        RAWINPUT raw{};
        UINT size = sizeof(raw);
        const HRAWINPUT handle = reinterpret_cast<HRAWINPUT>(lParam);
        if (GetRawInputData(handle, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER)) == static_cast<UINT>(-1))
            return;
        const RAWKEYBOARD &data = raw.data.keyboard;
        std::uint32_t key = data.VKey;
        if (key == 255)
            return;
        if (data.Flags & RI_KEY_E0) {
            if (key == VK_CONTROL)
                key = VK_RCONTROL;
            else if (key == VK_MENU)
                key = VK_RMENU;
        } else {
            switch (key) {
            case VK_CLEAR:
                key = VK_NUMPAD5;
                break;
            case VK_CONTROL:
                key = VK_LCONTROL;
                break;
            case VK_MENU:
                key = VK_LMENU;
                break;
            case VK_PRIOR:
                key = VK_NUMPAD9;
                break;
            case VK_NEXT:
                key = VK_NUMPAD3;
                break;
            case VK_END:
                key = VK_NUMPAD1;
                break;
            case VK_HOME:
                key = VK_NUMPAD7;
                break;
            case VK_LEFT:
                key = VK_NUMPAD4;
                break;
            case VK_UP:
                key = VK_NUMPAD8;
                break;
            case VK_RIGHT:
                key = VK_NUMPAD6;
                break;
            case VK_DOWN:
                key = VK_NUMPAD2;
                break;
            case VK_INSERT:
                key = VK_NUMPAD0;
                break;
            default:
                break;
            }
        }
        if (data.Message == WM_KEYDOWN) {
            SetKeyPressed(key);
            customInputHandler = 1;
        } else if (data.Message == WM_KEYUP &&
            ((data.VKey != VK_SHIFT && static_cast<std::uint16_t>(data.VKey - VK_LSHIFT) > 1U) || data.MakeCode == 42 || data.MakeCode == 54)) {
            SetKeyReleased(key);
            customInputHandler = 1;
        }
        return;
    }
#else
    static_cast<void>(window);
#endif
    switch (message) {
    case 0x200: {
        const std::int32_t width = inputWindowWidth > 0 ? inputWindowWidth : 1;
        const std::int32_t height = inputWindowHeight > 0 ? inputWindowHeight : 1;
        previousMouseDelta = mouseDelta;
        const float x = static_cast<float>(static_cast<std::int16_t>(lParam)) / static_cast<float>(width) - 0.5f;
        const float y = static_cast<float>(static_cast<std::int16_t>(lParam >> 16)) / static_cast<float>(height) - 0.5f;
        mouseDelta = {.x = x + x, .y = y + y};
        windowUIInput.position = {.x = static_cast<float>(static_cast<std::int16_t>(lParam)), .y = static_cast<float>(static_cast<std::int16_t>(lParam >> 16))};
        break;
    }
    case 0x201:
        SetKeyPressed(256);
        windowUIInput.leftDown = 1;
        ++windowUIInput.leftClicks;
        break;
    case 0x202:
        SetKeyReleased(256);
        windowUIInput.leftDown = 0;
        ++windowUIInput.leftClicks;
        break;
    case 0x204:
        SetKeyPressed(257);
        break;
    case 0x205:
        SetKeyReleased(257);
        break;
    case 0x207:
        SetKeyPressed(258);
        break;
    case 0x208:
        SetKeyReleased(258);
        break;
    case 0x20a: {
        const float wheel = static_cast<float>(static_cast<std::int16_t>(wParam >> 16));
        windowKeys[258].toggled = std::bit_cast<std::int32_t>(wheel);
        windowUIInput.wheel += wheel / 120.0f;
        break;
    }
    case 0x20b:
    case 0x20c: {
        const std::uint16_t button = static_cast<std::uint16_t>(wParam >> 16);
        if (button == 1 || button == 2) {
            if (message == 0x20b)
                SetKeyPressed(258U + button);
            else
                SetKeyReleased(258U + button);
        }
        break;
    }
    case 0x113:
        if (wParam == 50 && IsD3TWindowInputEnabled() && mouseDelta.x != 0.0f && mouseDelta.y != 0.0f) {
            for (std::size_t index = mouseHistory.size() - 1; index; --index)
                mouseHistory[index] = mouseHistory[index - 1];
            mouseHistory[0] = mouseDelta;
            if (mouseHistoryCount < 10)
                ++mouseHistoryCount;
        }
        break;
    default:
        break;
    }
}

WindowUIInput ConsumeWindowUIInput() {
    const WindowUIInput input = windowUIInput;
    windowUIInput.previousPosition = windowUIInput.position;
    windowUIInput.leftClicks = 0;
    windowUIInput.wheel = 0.0f;
    return input;
}

// @verified - p
void padEnableTask() {
    clearPrePadInput = true;
}

void InitializeInputSelectionState() {
    // InitializeInputSelectionState clears +00..+0b and +12..+17, retaining all six analog bytes.
    for (LegacyInputSlot &input : g_inputBuffer) {
        input.enabled = 0;
        input.unknown01 = 0;
        input.unknown02 = 0;
        input.held = 0;
        input.previousHeld = 0;
        input.pressedPriority = 0;
        input.repeatedPresses = 0;
        input.newPresses = 0;
        input.connectionState = 0;
        input.deviceState = 0;
    }
    inputSelection.confirmEligible.fill(0);
    inputSelection.startEligible.fill(0);
    inputSelection.previousDeviceStates.fill(0);
    selectedInputDevice = 4;
    inputSelection.field00 = 0;
    inputSelection.selectionSuspended = 0;
    inputSelection.connectedDeviceCount = 0;
    inputSelection.field48 = 0;
    inputSelection.startInputReserved = 1;
}

void SetInputDeviceSelectionSuspended(std::int32_t suspended) {
    inputSelection.selectionSuspended = suspended;
}

void InjectInputButtons(std::uint16_t buttons) {
    inputSelection.injectedButtons |= buttons;
}

void InjectInputAnalog(std::int16_t stickX, std::int16_t stickY) {
    harnessStickPending = true;
    harnessStickX = stickX;
    harnessStickY = stickY;
}

std::int32_t CountConnectedInputDevices() {
    std::int32_t count = 0;
    for (const LegacyInputSlot &input : g_inputBuffer) {
        count += input.deviceState != 0;
    }
    inputSelection.connectedDeviceCount = count;
    return count;
}

bool SelectInputDeviceForButtons(std::uint16_t buttons) {
    const std::uint16_t selectionButtons = buttons & 0x0c00U;
    for (std::size_t index = 0; index < g_inputBuffer.size(); ++index) {
        bool eligible;
        if (selectionButtons == 0x0c00U) {
            eligible = inputSelection.confirmEligible[index] == 1 || inputSelection.startEligible[index] == 1;
        } else if (buttons & 0x0400U) {
            eligible = inputSelection.confirmEligible[index] == 1;
        } else if (buttons & 0x0800U) {
            eligible = inputSelection.startEligible[index] == 1;
        } else {
            return false;
        }
        if (eligible) {
            selectedInputDevice = static_cast<std::int32_t>(index);
            return true;
        }
    }
    return false;
}

// @verified - p
void padcCheckControllerConnection() {
    if (inputSelection.selectionSuspended)
        return;
    if (g_inputBuffer[0].connectionState) {
        for (std::size_t index = 0; index < inputSelection.previousDeviceStates.size(); ++index) {
            inputSelection.previousDeviceStates[index] = g_inputBuffer[index].deviceState;
        }
        return;
    }
    WRN_RequestReconnectPrompt();
    for (std::size_t index = 0; index < inputSelection.startEligible.size(); ++index) {
        if (inputSelection.startEligible[index] != 1)
            continue;
        selectedInputDevice = static_cast<std::int32_t>(index);
        inputSelection.startInputReserved = 0;
        return;
    }
    for (std::size_t index = 0; index < inputSelection.previousDeviceStates.size(); ++index) {
        if (static_cast<std::int32_t>(index) == selectedInputDevice || inputSelection.previousDeviceStates[index] || g_inputBuffer[index].deviceState != 1)
            continue;
        selectedInputDevice = static_cast<std::int32_t>(index);
        return;
    }
}

void ResetSelectedInputDevice() {
    selectedInputDevice = 4;
}

void ForceInputDeviceDisconnected(std::int32_t disconnected) {
    inputSelection.forceDisconnected = disconnected;
}
// @verified - p
std::uint32_t GetControllerUserIndex() {
    return static_cast<std::uint32_t>(selectedPhysicalControllerIndex);
}

// @verified - p
void RefreshTriggerInputs() {
#if defined(_WIN32)
    XINPUT_STATE state;
    for (std::uint32_t index = 0; index < 4; ++index) {
        if (XInputGetState(index, &state) != ERROR_SUCCESS ||
            (state.Gamepad.wButtons == 0 && state.Gamepad.bLeftTrigger == 0 && state.Gamepad.bRightTrigger == 0))
            continue;
        selectedPhysicalControllerIndex = static_cast<std::int32_t>(index);
        physicalControllers[index].active = 1;
        return;
    }
#endif
}
// @verified - p
void InitializeSoftResetInput() {
    softResetInputEnabled = 1;
    softResetInputState = 0;
}

// @verified - p
void SetSoftResetInputEnabled(std::int32_t enabled) {
    softResetInputEnabled = enabled;
}

// @verified - p
bool CheckSoftResetInput() {
    if (!softResetInputEnabled)
        return false;
    if (!softResetInputState) {
        if ((g_inputBuffer[0].held & 0x0801U) == 0x0801U) {
            softResetInputState = 1;
            softResetInputCounter = 0;
        }
    } else if (softResetInputState == 1) {
        if ((g_inputBuffer[0].held & 0x0801U) == 0x0801U) {
            softResetInputCounter = static_cast<std::int32_t>(static_cast<std::uint32_t>(softResetInputCounter) + 1U);
            if (softResetInputCounter >= 90) {
                softResetInputState = 2;
                return true;
            }
        } else
            softResetInputState = 0;
    }
    return false;
}

void HandleInputs() {
    LegacyInputSlot &input = g_inputBuffer[0];
#if defined(_WIN32)
    XINPUT_STATE state{};
    const std::int32_t selectedController = static_cast<std::int32_t>(GetControllerUserIndex());
    const std::uint32_t controllerIndex = selectedController < 0 ? 0U : static_cast<std::uint32_t>(selectedController);
    const bool connected = XInputGetState(controllerIndex, &state) == ERROR_SUCCESS;
    PhysicalControllerState &physical = physicalControllers[controllerIndex];
    physical.active = connected ? 1U : 0U;
    if (connected) {
        physical.buttons = state.Gamepad.wButtons;
        physical.leftTrigger = state.Gamepad.bLeftTrigger;
        physical.rightTrigger = state.Gamepad.bRightTrigger;
        physical.thumbLX = state.Gamepad.sThumbLX;
        physical.thumbLY = state.Gamepad.sThumbLY;
        physical.thumbRX = state.Gamepad.sThumbRX;
        physical.thumbRY = state.Gamepad.sThumbRY;
        for (std::size_t index = 0; index < 4; ++index) {
            const std::int16_t raw = index == 0 ? physical.thumbLX : index == 1 ? physical.thumbLY : index == 2 ? physical.thumbRX : physical.thumbRY;
            const float value = (static_cast<float>(raw) + 0.5f) / 32767.5f;
            physical.normalizedSticks[index] = std::fabs(value) < 0.25f ? 0.0f : value;
        }
        (void)PostProcessControllerState(controllerIndex);
    } else {
        physical.buttons = 0;
        physical.leftTrigger = physical.rightTrigger = 0;
        physical.thumbLX = physical.thumbLY = physical.thumbRX = physical.thumbRY = 0;
        physical.normalizedSticks = {};
    }
    xInputReserved = 0;
    forceXInputProcessing = 0;
    forceXInputAnalogProcessing = 0;
    const bool inputEnabled = IsD3TWindowInputEnabled();
    if (inputEnabled) {
        DetectActiveInputDevice(controllerIndex);
        HandleControllerInput(controllerIndex);
    } else {
        physical.buttons = 0;
        physical.leftTrigger = physical.rightTrigger = 0;
        physical.thumbLX = physical.thumbLY = physical.thumbRX = physical.thumbRY = 0;
        physical.normalizedSticks = {};
    }
    ResetInputHandler();
    const bool harnessStick = harnessStickPending;
    harnessStickPending = false;
    if (harnessStick) {
        state.Gamepad.sThumbRX = static_cast<SHORT>(harnessStickX * 258);
        state.Gamepad.sThumbRY = static_cast<SHORT>(-harnessStickY * 258);

        state.Gamepad.bRightTrigger = (inputSelection.injectedButtons & 0x0080U) != 0 ? 255 : 0;
        state.Gamepad.bLeftTrigger = (inputSelection.injectedButtons & 0x0040U) != 0 ? 255 : 0;
    } else {
        state.Gamepad.wButtons = static_cast<WORD>(physical.buttons);
        state.Gamepad.bLeftTrigger = physical.leftTrigger;
        state.Gamepad.bRightTrigger = physical.rightTrigger;
        if (!connected)
            selectedPhysicalControllerIndex = -1;
    }

    const std::uint32_t nativeButtons = TranslateXInputButtons(state.Gamepad.wButtons);
    input.previousHeld = input.held;
    input.enabled = 1;
    input.unknown01 = 0;
    input.unknown02 = 0x06fe;
    input.held = TranslateLegacyHeld(nativeButtons, GetIsFreeInputType());
    if (state.Gamepad.bRightTrigger != 0)
        input.held |= 0x0080U;
    if (state.Gamepad.bLeftTrigger != 0)
        input.held |= 0x0040U;
    input.held |= inputSelection.injectedButtons;
    inputSelection.injectedButtons = 0;
    if ((input.held & 0x0202U) != 0)
        suppressStartFrames = 16;
    if (suppressStartFrames > 0) {
        input.held &= ~0x0800U;
        --suppressStartFrames;
    }
    input.connectionState = 1;
    if (!connected && padPollConnected && GetCurrentUIInputType())
        input.connectionState = 0;
    padPollConnected = connected;
    input.analog0c = state.Gamepad.bRightTrigger;
    input.analog0d = state.Gamepad.bLeftTrigger;
    const bool keyboard = keyboardActive == 1 && !harnessStick;
    const std::int16_t leftX = keyboard ? 0 : NormalizeStick(state.Gamepad.sThumbLX, 1.0f);
    const std::int16_t leftY = keyboard ? 0 : NormalizeStick(state.Gamepad.sThumbLY, -1.0f);
    const std::int16_t rightX = keyboard ? static_cast<std::int16_t>(physical.normalizedSticks.z * 127.0f) : NormalizeStick(state.Gamepad.sThumbRX, 1.0f);
    const std::int16_t rightY = keyboard ? static_cast<std::int16_t>(physical.normalizedSticks.w * -127.0f) : NormalizeStick(state.Gamepad.sThumbRY, -1.0f);
    input.analog0e = static_cast<std::uint8_t>(rightX + 0x80);
    input.analog0f = static_cast<std::uint8_t>(rightY + 0x80);
    input.analog10 = static_cast<std::uint8_t>(leftX + 0x80);
    input.analog11 = static_cast<std::uint8_t>(leftY + 0x80);

    const std::uint16_t presses = static_cast<std::uint16_t>(input.held & (input.previousHeld ^ input.held));
    input.newPresses = presses;
    const std::uint16_t shared = presses & 0xf8c0U;
    if (presses & 0x0004U)
        input.pressedPriority = shared | 0x0004U;
    else if (presses & 0x0002U)
        input.pressedPriority = shared | 0x0002U;
    else if (presses & 0x0400U)
        input.pressedPriority = shared | 0x0400U;
    else if (presses & 0x0100U)
        input.pressedPriority = shared | 0x0100U;
    else if (presses & 0x0020U)
        input.pressedPriority = shared | 0x0020U;
    else if (presses & 0x0010U)
        input.pressedPriority = shared | 0x0010U;
    else if (presses & 0x0008U)
        input.pressedPriority = shared | 0x0008U;
    else if (presses & 0x0001U)
        input.pressedPriority = shared | 0x0001U;
    else if (presses & 0x0200U)
        input.pressedPriority = shared | 0x0200U;
    else
        input.pressedPriority = presses;
    RefreshTriggerInputs();

#else
    ResetProcessedInput(input);
    input.held = 0;
#endif
    if (inputSelection.forceDisconnected != 0)
        input.connectionState = 0;
    if (inputSelection.startInputReserved == 0) {
        input.pressedPriority &= ~0x0800U;
        inputSelection.startInputReserved = 1;
    }
}

// @verified - p
HLib::Task *Queue_PrePadControlTask(std::uint8_t nextFunctionIndex) {
    clearPrePadInput = false;
    return HLib::EnqueueTaskWithParameter(PrePadControl_Callback, nextFunctionIndex, 2, 0, SHEN_NAME(PPAD));
}

HLib::Task *Queue_PadControlTask(std::uint8_t nextFunctionIndex) {
    padControlTask = HLib::EnqueueTaskWithParameter(PadControl_Callback, nextFunctionIndex, 2, sizeof(PadControlTaskData), SHEN_NAME(PADC));
    PadControlTaskData *const control = static_cast<PadControlTaskData *>(HLib::GetTaskParameter(padControlTask));
    control->currentBoundEntry = nullptr;
    for (PadControlEntry &entry : control->entries) {
        entry.flags = 0;
        entry.input.unknown02 = 0;
        entry.input.held = 0;
        entry.input.previousHeld = 0;
        entry.input.pressedPriority = 0;
        entry.input.repeatedPresses = 0;
        entry.input.analog0c = 0;
        entry.input.analog0d = 0;
        entry.input.analog0e = 0x80;
        entry.input.analog0f = 0x80;
        entry.input.connectionState = 0;
    }
    return padControlTask;
}



PadControlEntry *AllocatePadControlEntry() {
    PadControlTaskData *const control = static_cast<PadControlTaskData *>(HLib::GetTaskParameter(padControlTask));
    std::size_t index = 0;
    while (index < control->entries.size() && (control->entries[index].flags & 1U) != 0)
        ++index;
    if (index == control->entries.size())
        return nullptr;
    PadControlEntry &entry = control->entries[index];
    entry.flags |= 1U;
    entry.input.held = 0;
    entry.input.previousHeld = 0;
    entry.input.unknown02 = 0;
    entry.input.pressedPriority = 0;
    entry.input.repeatedPresses = 0;
    entry.input.analog0c = 0;
    entry.input.analog0d = 0;
    entry.input.analog0e = 0x80;
    entry.input.analog0f = 0x80;
    entry.input.connectionState = 0;
    return &entry;
}

PadControlEntry *BindPadControlEntry(PadControlEntry *entry) {
    auto *const control = static_cast<PadControlTaskData *>(HLib::GetTaskParameter(padControlTask));
    if (!control)
        return nullptr;
    if (entry && (entry->flags & 1U) == 0)
        entry = nullptr;
    PadControlEntry *const previous = control->currentBoundEntry;
    if (previous != entry) {
        if (previous) {
            ResetProcessedInput(previous->input);
            previous->flags &= ~2U;
        }
        if (entry) {
            entry->input = g_inputBuffer[0];
            entry->flags |= 2U;
        }
        control->currentBoundEntry = entry;
    }
    return previous;
}

void ReleasePadControlEntry(PadControlEntry *entry) {
    if (!entry || (entry->flags & 1U) == 0)
        return;
    if ((entry->flags & 2U) != 0)
        BindPadControlEntry(nullptr);
    entry->flags &= ~1U;
}

LegacyInputSlot *ctrlGetPadInputBuffer0(PadControlEntry *control) {
    return &control->input;
}

std::int64_t Queue_KeyRepeat(std::uint32_t inputIndex) {
    if (inputIndex >= g_inputBuffer.size()) {
        return 0;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KeyRepeat_Callback, 15, 2, sizeof(KeyRepeatTaskData), SHEN_NAME(KREP));
    if (!task) {
        return 0;
    }
    auto *const repeat = static_cast<KeyRepeatTaskData *>(HLib::GetTaskParameter(task));
    if (!repeat) {
        return 0;
    }
    repeat->inputIndex = inputIndex;
#if SHENMUE_IS_PC
    repeat->counters.fill(15.0f);
#else
    repeat->counters.fill(15);
#endif
    return 1;
}

HLib::Task *Queue_btnButtonMediationCtrl(std::uint8_t nextFunctionIndex) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(btnButtonMediationCtrl_Callback, nextFunctionIndex, 0, sizeof(ButtonMediationControl), SHEN_NAME(BTNM));
    buttonMediation.flags = 0;
    buttonMediation.count = 0;
    buttonMediation.field110 = 0;
    buttonMediation.field114 = 0;
    buttonMediation.field08 = 0;
    buttonMediation.field128 = 0;
    for (std::size_t button = 0; button < buttonMediation.results.size(); ++button) {
        buttonMediation.results[button].name = {};
        buttonMediation.priorities[button] = {};
    }
    doublePressResult = 0;
    doublePressRequested = 0;
    doublePressFrames = 0;
    return task;
}

std::int64_t RegisterButtonMediation(shen_name name, const Vector3 *position, std::uint32_t flags) {
    if (buttonMediation.count >= 11)
        return 0xffffffffLL;
    ButtonMediationRequest &request = buttonMediation.requests[buttonMediation.count++];
    request.name = name;
    request.flags = name == BUTTON_BTME ? flags & ~ButtonMediationShowButtonHint : flags;
    if (position) {
        request.position = *position;
    } else {
        Vector3 location{};
        std::uint16_t yaw = 0;
        if (ctrlGetRootNode()) {
            plymUpdatePlayerSearchLocation(&location);
            yaw = static_cast<std::uint16_t>(playerGetRotY());
        }
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(location.x, location.y, location.z);
        ApplyShearToPrePassMatrix(FixedAngleSin(yaw), FixedAngleCos(yaw));
        Matrix4x4::preTranslate(0.0f, 0.0f, -1.0f);
        request.position = {.x = workTransformMatrix.rows[3].x, .y = workTransformMatrix.rows[3].y, .z = workTransformMatrix.rows[3].z};
        Matrix4x4::Pop();
        request.flags |= ButtonMediationPlayerRelative;
    }
    return 0;
}

std::uint32_t btnCheck(shen_name name) {
    constexpr std::array<std::uint32_t, 5> masks{0x400, 0x200, 4, 2, 0x800};
    for (const auto &result : buttonMediation.results) {
        if (result.name.name != name.name)
            continue;
        std::uint32_t buttons = 0;
        for (std::size_t button = 0; button < masks.size(); ++button)
            if ((result.flags & (1U << (buttonMediationPressedShift + button))) != 0)
                buttons |= masks[button];
        return buttons;
    }
    return 0;
}

std::uint32_t GetMediatedButtonBindings(shen_name name) {
    constexpr std::array<std::uint32_t, 5> masks{0x400, 0x200, 4, 2, 0x800};
    for (const auto &result : buttonMediation.results) {
        if (result.name.name != name.name)
            continue;
        std::uint32_t buttons = 0;
        for (std::size_t button = 0; button < masks.size(); ++button)
            if ((result.flags & (1U << (buttonMediationBindingShift + button))) != 0)
                buttons |= masks[button];
        return buttons;
    }
    return 0;
}

std::int64_t GetBattleDataConfig(std::int32_t mode, shen_name name) {
    if (mode == 1)
        return GetMediatedButtonBindings(name);
    if (mode == 2)
        return btnCheck(name);
    if (mode != 0)
        return 0;
    std::uint32_t flags = name.name == SHEN_NAME(BTBT) ? (ButtonMediationShowButtonHint | ButtonMediationBindA) : ButtonMediationBindA;
    if (name.name == SHEN_NAME(BTAD))
        return RegisterButtonMediation(name, nullptr, (ButtonMediationShowButtonHint | ButtonMediationBindX));
    if (name.name == SHEN_NAME(BTIL))
        return RegisterButtonMediation(name, nullptr, (ButtonMediationShowButtonHint | ButtonMediationBindB));
    if (name.name == SHEN_NAME(BTSP))
        flags = (ButtonMediationShowButtonHint | ButtonMediationBindX);
    return RegisterButtonMediation(name, nullptr, flags);
}

shen_name GetMediatedButtonOwner(std::uint32_t button) {
    return button < buttonMediation.priorities.size() ? buttonMediation.priorities[button] : shen_name{};
}

std::uint64_t CheckMediatedDoublePress() {
    const std::uint32_t result = doublePressResult;
    doublePressRequested = 1;
    return result;
}

} // namespace shenmue::s2
