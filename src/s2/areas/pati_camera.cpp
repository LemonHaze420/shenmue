#include "s2/areas/pati_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/input.hpp"
#include "s2/math.hpp"
#include "s2/scene_data.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
std::int32_t g_patiVersus, g_patiMinimumBet, g_patiCameraMoving, g_patiCameraStep, g_patiSelectionAccepted, g_patiInspectRequested, g_patiHeightRepeat;
std::int32_t g_patiDealerInputClock, g_patiDealerInputIndex;
std::uint8_t g_patiMuseumMode, g_patiSingleMode;
float g_patiModelAlpha, g_patiCameraHeight;
Vector3 g_patiCameraPosition, g_patiCameraLookAt, g_patiFilteredEye, g_patiFilteredTarget, g_patiDesiredEye, g_patiDesiredTarget, g_patiBoardPosition;
Vector3i g_patiBoardRotation;
char g_patiBoardSize;
// @verified - p
void SetupPatiBoardSize(std::int32_t mode) {
    using enum CameraState;
    const float cx = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(g_patiBoardRotation.x))));
    const float sx = sin_fixed16(static_cast<std::int16_t>(g_patiBoardRotation.x));
    const float cy = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(g_patiBoardRotation.y))));
    const float sy = sin_fixed16(static_cast<std::int16_t>(g_patiBoardRotation.y));
    const Vector3 base = g_patiBoardPosition;
    float eyeY, eyeZ, targetY, targetZ;
    if (g_patiBoardSize == 'L') {
        eyeY = 1.5369999f;
        eyeZ = 2.0120001f;
        targetY = 0.78829998f;
        targetZ = -0.58109999f;
    } else if (g_patiBoardSize == 'X') {
        eyeY = 12.4497f;
        eyeZ = 16.297201f;
        targetY = 6.3852301f;
        targetZ = -4.7069101f;
    } else {
        eyeY = 0.88620001f;
        eyeZ = 1.1691999f;
        targetY = 0.38080001f;
        targetZ = -0.58109999f;
    }
    const float zero = eyeY * 0.0f, bx = cy * 0.0f + base.x, bz = -sy * 0.0f + base.z;
    const Vector3 distantEye{(zero + bx) + eyeZ * sy, ((base.y + 0.0f) + eyeY) + eyeZ * 0.0f, (bz + zero) + eyeZ * cy};
    const Vector3 distantTarget{(targetY * 0.0f + bx) + targetZ * sy, ((base.y + 0.0f) + targetY) + targetZ * 0.0f, (targetY * 0.0f + bz) + targetZ * cy};
    float eyeLift, closeEyeZ, lookLift, closeTargetZ;
    if (g_patiBoardSize == 'L') {
        eyeLift = 0.1415f;
        closeEyeZ = 0.070799999f;
        lookLift = -0.46250001f;
        closeTargetZ = -0.9228f;
    } else if (g_patiBoardSize == 'X') {
        eyeLift = 1.14615f;
        closeEyeZ = 0.57348001f;
        lookLift = -3.7462502f;
        closeTargetZ = -7.4746804f;
    } else {
        eyeLift = 0.1415f;
        closeEyeZ = 0.2608f;
        lookLift = -0.46250001f;
        closeTargetZ = -0.9228f;
    }
    const float inputX = static_cast<float>(static_cast<std::uint8_t>(GetCurrentControlInput(INPUT_RIGHT_X))) / 255.0f;
    float inputY = static_cast<float>(static_cast<std::uint8_t>(GetCurrentControlInput(INPUT_RIGHT_Y))) / 255.0f;
    float zoom;
    if (mode == 1)
        zoom = 255.0f;
    else if (mode == 2)
        zoom = 0.0f;
    else
        zoom = static_cast<float>(static_cast<std::uint8_t>(GetCurrentControlInput(!GetS2IsEnglishVersion() ? 4 : 3)));
    zoom /= 255.0f;
    float top, skew, maximum, minimum, middle, left, right;
    if (g_patiBoardSize == 'M') {
        top = 1.01f;
        maximum = 0.96000004f;
        middle = 0.495f;
        minimum = 0.20999999f;
        skew = 0.0099999905f;
        left = -0.15000001f;
        right = 0.15000000596f;
    } else if (g_patiBoardSize == 'S') {
        top = 0.81f;
        maximum = 0.75999999f;
        middle = 0.39499998f;
        minimum = 0.20999999f;
        skew = 0.0099999905f;
        left = -0.15000001f;
        right = 0.15000000596f;
    } else if (g_patiBoardSize == 'X') {
        top = 14.661f;
        maximum = 14.256001f;
        middle = 7.2495003f;
        minimum = 2.1059999f;
        skew = -0.32400018f;
        left = -1.2150002f;
        right = 1.2150002f;
    } else {
        top = 1.8099999f;
        maximum = 1.76f;
        middle = 0.89499998f;
        minimum = 0.25999999f;
        skew = -0.040000021f;
        left = -0.15000001f;
        right = 0.15000000596f;
        inputY = ((inputY - 0.5f) * 0.5f) + 0.5f;
    }
    if (static_cast<double>(middle + g_patiCameraHeight) > static_cast<double>(maximum) + 0.006)
        g_patiCameraHeight = maximum - middle;
    if (static_cast<double>(minimum) - 0.006 > static_cast<double>(middle + g_patiCameraHeight))
        g_patiCameraHeight = minimum - middle;
    const float eyeLocalY = (skew * 0.5f + top * 0.5f) + eyeLift;
    const float eyeLocalX = (right * 0.5f + left * 0.5f) + 0.0f;
    Vector3 closeEye{((eyeLocalX * cy + base.x) + eyeLocalY * 0.0f) + closeEyeZ * sy, ((eyeLocalX * 0.0f + base.y) + eyeLocalY) + closeEyeZ * 0.0f,
        ((eyeLocalX * -sy + base.z) + eyeLocalY * 0.0f) + closeEyeZ * cy};
    const float targetLocalX = ((1.0f - inputX) * left + right * inputX) + 0.0f;
    const float targetLocalY = ((1.0f - inputY) * top + skew * inputY) + lookLift;
    Vector3 closeTarget{((targetLocalX * cy + base.x) + targetLocalY * 0.0f) + closeTargetZ * sy,
        ((targetLocalX * 0.0f + base.y) + targetLocalY) + closeTargetZ * 0.0f, ((targetLocalX * -sy + base.z) + targetLocalY * 0.0f) + closeTargetZ * cy};
    const std::uint16_t buttons = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_REPEAT));
    g_patiHeightRepeat = (buttons & 0x3000) ? g_patiHeightRepeat + 1 : 0;
    for (std::int32_t index = 0; index <= (g_patiHeightRepeat >= 45 ? 9 : g_patiHeightRepeat / 5); ++index) {
        if (buttons & 0x1000) {
            if (maximum > middle + g_patiCameraHeight)
                g_patiCameraHeight += 0.0049999999f;
        } else if ((buttons & 0x2000) && middle + g_patiCameraHeight > minimum)
            g_patiCameraHeight += -0.0049999999f;
    }
    const float heightX = g_patiCameraHeight * (sx * sy), heightY = g_patiCameraHeight * cx, heightZ = g_patiCameraHeight * (sx * cy);
    closeEye.x += heightX;
    closeEye.y += heightY;
    closeEye.z += heightZ;
    closeTarget.x += heightX;
    closeTarget.y += heightY;
    closeTarget.z += heightZ;
    g_patiDesiredEye = {
        (1.0f - zoom) * distantEye.x + closeEye.x * zoom, (1.0f - zoom) * distantEye.y + closeEye.y * zoom, (1.0f - zoom) * distantEye.z + closeEye.z * zoom};
    g_patiDesiredTarget = {(1.0f - zoom) * distantTarget.x + closeTarget.x * zoom, (1.0f - zoom) * distantTarget.y + closeTarget.y * zoom,
        (1.0f - zoom) * distantTarget.z + closeTarget.z * zoom};
    if (g_patiCameraStep >= 8) {
        g_patiFilteredEye.x += g_patiDesiredEye.x - g_patiFilteredEye.x;
        g_patiFilteredEye.y += g_patiDesiredEye.y - g_patiFilteredEye.y;
        g_patiFilteredEye.z += g_patiDesiredEye.z - g_patiFilteredEye.z;
        g_patiFilteredTarget.x += g_patiDesiredTarget.x - g_patiFilteredTarget.x;
        g_patiFilteredTarget.y += g_patiDesiredTarget.y - g_patiFilteredTarget.y;
        g_patiFilteredTarget.z += g_patiDesiredTarget.z - g_patiFilteredTarget.z;
    } else {
        g_patiFilteredEye.x += (g_patiDesiredEye.x - g_patiFilteredEye.x) * 0.25f;
        g_patiFilteredEye.z += (g_patiDesiredEye.z - g_patiFilteredEye.z) * 0.25f;
        g_patiFilteredEye.y += (g_patiDesiredEye.y - g_patiFilteredEye.y) * 0.25f;
        g_patiFilteredTarget.y += (g_patiDesiredTarget.y - g_patiFilteredTarget.y) * 0.25f;
        g_patiFilteredTarget.x += (g_patiDesiredTarget.x - g_patiFilteredTarget.x) * 0.25f;
        g_patiFilteredTarget.z += (g_patiDesiredTarget.z - g_patiFilteredTarget.z) * 0.25f;
    }
    camCtrlSetState(Callback);
    g_patiCameraId = 0;
    g_patiCameraEye = {};
    g_patiCameraTarget = {};
    if (g_patiCameraStep <= 0) {
        g_patiCameraPosition.x += g_patiFilteredEye.x - g_patiCameraPosition.x;
        g_patiCameraPosition.y += g_patiFilteredEye.y - g_patiCameraPosition.y;
        g_patiCameraPosition.z += g_patiFilteredEye.z - g_patiCameraPosition.z;
        g_patiCameraLookAt.x += g_patiFilteredTarget.x - g_patiCameraLookAt.x;
        g_patiCameraLookAt.y += g_patiFilteredTarget.y - g_patiCameraLookAt.y;
        g_patiCameraLookAt.z += g_patiFilteredTarget.z - g_patiCameraLookAt.z;
    } else {
        const float step = static_cast<float>(g_patiCameraStep);
        g_patiCameraPosition.x += (g_patiFilteredEye.x - g_patiCameraPosition.x) / step;
        g_patiCameraPosition.y += (g_patiFilteredEye.y - g_patiCameraPosition.y) / step;
        g_patiCameraPosition.z += (g_patiFilteredEye.z - g_patiCameraPosition.z) / step;
        g_patiCameraLookAt.x += (g_patiFilteredTarget.x - g_patiCameraLookAt.x) / step;
        g_patiCameraLookAt.y += (g_patiFilteredTarget.y - g_patiCameraLookAt.y) / step;
        g_patiCameraLookAt.z += (g_patiFilteredTarget.z - g_patiCameraLookAt.z) / step;
    }
    camCtrlSetNextPosition(g_patiCameraPosition);
    camCtrlSetTargetPosition(g_patiCameraLookAt);
    camCtrlSetFov(10922);
    const float ex = distantEye.x - closeEye.x, ey = distantEye.y - closeEye.y, ez = distantEye.z - closeEye.z;
    float distance = (ey * ey + ex * ex) + ez * ez;
    const float px = distantEye.x - g_patiCameraPosition.x, py = distantEye.y - g_patiCameraPosition.y, pz = distantEye.z - g_patiCameraPosition.z;
    const float currentDistance = (py * py + px * px) + pz * pz;
    if (distance <= 0.0000099999997f)
        distance = 0.000099999997f;
    const float fraction = std::fmin(1.0f, std::fmax(0.0f, currentDistance / distance));
    const float dx = g_patiFilteredEye.x - g_patiCameraPosition.x, dy = g_patiFilteredEye.y - g_patiCameraPosition.y,
                dz = g_patiFilteredEye.z - g_patiCameraPosition.z;
    g_patiCameraMoving = (dy * dy + dx * dx) + dz * dz >= 0.001f;
    g_patiModelAlpha += (std::fmin(0.5f, fraction * fraction) - g_patiModelAlpha) * 0.5f;
    modelSetCharaAlpha(FindCharacterByScenarioName(shen_name{static_cast<std::uint32_t>(patiGameplayPlacements[0].yaw)}), g_patiModelAlpha);
}
// @verified - p
void SetupPatiBoard(std::int32_t mode) {
    SetupPatiBoardSize(mode);
    g_patiFilteredEye = g_patiDesiredEye;
    g_patiCameraPosition = g_patiDesiredEye;
    g_patiFilteredTarget = g_patiDesiredTarget;
    g_patiCameraLookAt = g_patiDesiredTarget;
    SetupPatiBoardSize(mode);
}
// @verified - p
void InitializePatiBoardCamera(PatiEventWork *work) {
    g_patiCameraHeight = 0.0f;
    camCtrlGetNextCamPos(&g_patiCameraPosition);
    camCtrlGetTargetPosition(&g_patiCameraLookAt);
    camCtrlGetNextCamPos(&g_patiFilteredEye);
    camCtrlGetTargetPosition(&g_patiFilteredTarget);
    const PatiBoardChoice &choice = work->choices[work->selectedBoard];
    g_patiBoardRotation = choice.rotation;
    g_patiBoardPosition = choice.position;
    g_patiBoardSize = choice.size;
}
// @verified - p
void SelectPatiBoard(PatiEventWork *work, const char *name) {
    shen_name character;
    std::memcpy(&character, name, sizeof(character));
    std::int32_t index = 0;
    while (work->choices[index].character.name && work->choices[index].character.name != character.name)
        ++index;
    work->selectedBoard = work->choices[index].character.name ? index : 0;
    InitializePatiBoardCamera(work);
}
} // namespace shenmue::s2
