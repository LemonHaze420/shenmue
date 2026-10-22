#include "s2/areas/area_pvsj.hpp"
#include "s2/areas/pati_event.hpp"
#include "s2/camera.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include <cmath>
#include <initializer_list>
namespace shenmue::s2 {
namespace {
float Sine(std::uint16_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(angle));
}
float Cosine(std::uint16_t angle) {
    const auto folded = angle <= 0x8000U ? angle : static_cast<std::uint16_t>(-angle);
    return Sine(static_cast<std::uint16_t>(0x4000U - folded));
}
} // namespace

void PvsjSetCamera(PvsjEventWork *work, std::int32_t id, shen_name eye, shen_name target, std::int32_t delay) {
    if (id == work->cameraId && eye.name == work->cameraEye.name && target.name == work->cameraTarget.name)
        return;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EVT_AT_SetCam(id, eye, target, 0, 0, delay);
    work->cameraId = id;
    pvsjCameraBlend = -1;
    work->cameraEye = eye;
    work->cameraTarget = target;
    g_patiCameraTime = delay;
}
void PvsjShowChoice(PvsjEventWork *work) {
    PvsjSetCamera(work, pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay);
    EVT_AT_KsIcon_Enqueue(-1, 37, 34, -1, 10800, 0xffff, 2, work->delay);
    work->delay += 5;
}

void PvsjSetOverlay(PvsjEventWork *work, std::int32_t mode) {
    PvsjUpdateOverlay(work, mode);
    pvsjFilteredEye = pvsjDesiredEye;
    pvsjFilteredTarget = pvsjDesiredTarget;
    PvsjUpdateOverlay(work, mode);
}

void PvsjUpdateOverlay(PvsjEventWork *work, std::int32_t mode) {
    using enum CameraState;
    Matrix4x4_Push();
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(PVSJ_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(PVSJ_PLAYER, &rotation, EventTransformFlags::World);
    SetXInputDigitalProcessing(1);
    const float x = static_cast<std::uint8_t>(GetCurrentControlInput(INPUT_RIGHT_X)) / 255.0f,
                y = static_cast<std::uint8_t>(GetCurrentControlInput(INPUT_RIGHT_Y)) / 255.0f;
    const float blend = (mode == 2 ? 0.0f : mode == 3 ? static_cast<float>(pvsjCameraBlend) : 255.0f) / 255.0f;
    const auto pitch = static_cast<std::uint16_t>(static_cast<std::int32_t>(((0.5 - static_cast<double>(y)) * 2.0) * 45.0 * 8192.0 / 45.0));
    const auto yaw = static_cast<std::uint16_t>(static_cast<std::int32_t>(((0.5 - static_cast<double>(x)) * 2.0) * 60.0 * 8192.0 / 45.0));
    workTransformMatrix = transformMatrix_prePass;
    for (const auto angle : {static_cast<std::uint16_t>(rotation.z), static_cast<std::uint16_t>(rotation.y), static_cast<std::uint16_t>(rotation.x)})
        ApplyShearToPrePassMatrix(Sine(angle), Cosine(angle));
    Matrix4x4::RotateAroundZ(Sine(0), Cosine(0));
    ApplyShearToPrePassMatrix(Sine(yaw), Cosine(yaw));
    Matrix4x4::RotateAroundX(Sine(pitch), Cosine(pitch));
    const auto &matrix = workTransformMatrix;
    const float dx = ((matrix.rows[0].x * 0.0f + matrix.rows[1].x * 0.0f) - matrix.rows[2].x * 0.5f) + matrix.rows[3].x;
    const float dy = ((matrix.rows[0].y * 0.0f + matrix.rows[1].y * 0.0f) - matrix.rows[2].y * 0.5f) + matrix.rows[3].y;
    const float dz = ((matrix.rows[0].z * 0.0f + matrix.rows[1].z * 0.0f) - matrix.rows[2].z * 0.5f) + matrix.rows[3].z;
    const float twiceBlend = blend + blend;
    pvsjDesiredEye = {
        (twiceBlend - 1.0f) * dx + (position.x + 0.0f), (twiceBlend - 1.0f) * dy + (position.y + 1.6f), (twiceBlend - 1.0f) * dz + (position.z + 0.0f)};
    pvsjDesiredTarget = {twiceBlend * dx + (position.x + 0.0f), twiceBlend * dy + (position.y + 1.6f), twiceBlend * dz + (position.z + 0.0f)};
    Matrix4x4::Pop(1);
    pvsjFilteredEye.x += (pvsjDesiredEye.x - pvsjFilteredEye.x) * 0.125f;
    pvsjFilteredEye.z += (pvsjDesiredEye.z - pvsjFilteredEye.z) * 0.125f;
    pvsjFilteredEye.y += (pvsjDesiredEye.y - pvsjFilteredEye.y) * 0.125f;
    pvsjFilteredTarget.y += (pvsjDesiredTarget.y - pvsjFilteredTarget.y) * 0.125f;
    pvsjFilteredTarget.x += (pvsjDesiredTarget.x - pvsjFilteredTarget.x) * 0.125f;
    pvsjFilteredTarget.z += (pvsjDesiredTarget.z - pvsjFilteredTarget.z) * 0.125f;
    work->cameraId = 0;
    work->cameraEye = {};
    work->cameraTarget = {};
    camCtrlSetState(Callback);
    camCtrlSetNextPosition(pvsjFilteredEye);
    camCtrlSetTargetPosition(pvsjFilteredTarget);
    float trigger = 0;
    if (mode == 0)
        trigger = static_cast<std::uint8_t>(GetCurrentControlInput(!GetS2IsEnglishVersion() ? 4 : 3));
    const float degrees = 60.0f - std::fmax(0.0f, std::fmin(1.0f, trigger / 255.0f)) * 30.0f;
    const auto current = static_cast<std::int16_t>(camCtrlGetPerspective());
    const auto difference = static_cast<std::int32_t>((degrees * 8192.0f) / 45.0f) - current;
    camCtrlSetPerspective(static_cast<std::uint16_t>(current + difference / (std::fabs(degrees) < 0.00000011920929f ? 4 : 2)));
}
} // namespace shenmue::s2
