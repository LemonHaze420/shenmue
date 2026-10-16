#include "s2/areas/area_mdtp.hpp"
#include "s2/areas/mdtp_tables.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/pause.hpp"
#include "s2/sound_program.hpp"
#include <cmath>
namespace shenmue::s2 {
namespace {
float CleanFloat(float value) {
    return std::fabs(value) < 0.00000011920929f ? 0.0f : value;
}
void CalculateMdtpHandTarget(std::int32_t selected, Vector3 *output, std::int32_t press) {
    if (!output || static_cast<std::uint32_t>(selected + 2) > 4)
        return;
    Vector3 endpoint, transformed, recorderPosition;
    Vector3i recorderRotation;
    Matrix4x4 bone;
    GetCharacterHandEndpoint(ctrlGetCharaByName(MDTP_PLAYER), 0, 3, &endpoint);
    CopyMotionBoneTransform(scenFindMotionwork(MDTP_PLAYER), 14, &bone);
    Matrix4x4_Push(&bone);
    Matrix4x4::TransformPoint(endpoint, transformed);
    Matrix4x4::Pop();
    EV_GetCharaPosition(MDTP_RECORDER, CHARACTER_TRANSLATION_ROOT, &recorderPosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(MDTP_RECORDER, &recorderRotation, EventTransformFlags::World);
    const float x = CleanFloat(CleanFloat(static_cast<float>(selected) * 0.02f) - 0.066f);
    const float pressHeight =
        press <= 8 ? static_cast<float>(press) * -0.00087500003f : CleanFloat(static_cast<float>(press - 8) * 0.00087500003f) - 0.0070000002f;
    Vector3 target{.x = x, .y = CleanFloat(CleanFloat(pressHeight) + 0.068000004f), .z = 0.0080000004f};
    ApplyScenarioTranslationAndRotation(&recorderPosition, &recorderRotation, &target);
    output->x = CleanFloat(CleanFloat(bone.rows[3].x - transformed.x) + target.x);
    output->y = CleanFloat(CleanFloat(bone.rows[3].y - transformed.y) + target.y);
    output->z = CleanFloat(CleanFloat(bone.rows[3].z - transformed.z) + target.z);
}
void BlendMdtpHandPosition(Vector3 *position, const Vector3 *target, std::int32_t frames) {
    if (!position || !target)
        return;
    if (frames <= 1) {
        *position = *target;
        return;
    }
    const float step = CleanFloat(1.0f / static_cast<float>(frames));
    for (std::size_t axis = 0; axis < 3; ++axis)
        (*position)[axis] = CleanFloat((*position)[axis] + CleanFloat(CleanFloat((*target)[axis] - (*position)[axis]) * step));
}
} // namespace

bool UpdateMdtpButtons(std::int32_t selected, std::int32_t *press, std::int32_t *active, std::int32_t *frames, std::int32_t *stopping) {
    const std::int32_t stop = stopping ? *stopping : 0;
    std::int32_t anyPressed = 0;
    bool changed = false;
    if (press && active && frames) {
        if (*press) {
            --*press;
            if (*press < 0)
                *press = 0;
            std::int32_t *const command = *press > 8 ? MDTP_HAND_RELEASE : MDTP_HAND_PRESS;
            const std::int32_t ticks = *press > 8 ? *press - 8 : *press;
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(MDTP_PLAYER), 0, reinterpret_cast<std::byte *>(command), static_cast<std::int16_t>(ticks));
        }
        for (std::int32_t index = 0; MDTP_BUTTON_PARTS[index] != -1; ++index) {
            const std::int32_t old = frames[index];
            anyPressed |= old != 0;
            std::int32_t next;
            bool considerActive = false;
            if (stop) {
                if (MDTP_BUTTON_PARTS[index] == 156) {
                    next = 0;
                    considerActive = true;
                } else {
                    next = old;
                    if (old) {
                        if (old > 8)
                            next = 15 - old;
                        next = next - 1 >= 8 ? next - 1 : next - 2;
                        if (next < 0)
                            next = 0;
                    }
                }
            } else if (index == selected + 2) {
                next = *press;
                considerActive = *press < 8;
            } else {
                next = old;
                if (old) {
                    if (old > 8)
                        next = 15 - old;
                    next = next - 1 >= 8 ? next - 1 : next - 2;
                    if (next < 0)
                        next = 0;
                }
            }
            if (considerActive && (old || stop)) {
                changed = *active != MDTP_BUTTON_PARTS[index];
                if (changed) {
                    if (*active == 152 || *active == 155)
                        Loop3DSoundEffect(0x492);
                    else if (*active == 154) {
                        Loop3DSoundEffect(0x492);
                        FinishLoading();
                    }
                    *active = MDTP_BUTTON_PARTS[index];
                }
            }
            if (!stop && *active == MDTP_BUTTON_PARTS[index] && (index == selected + 2 || frames[selected + 2] > 12.8 || !frames[selected + 2]) && old &&
                ((*active - 152) & ~3) == 0 && *active != 153)
                next = 7;
            frames[index] = next;
            Vector3 position;
            Vector3i rotation;
            EV_scenApplyPosOrRotToPartOf(
                MDTP_RECORDER, static_cast<std::uint16_t>(MDTP_BUTTON_PARTS[index]), &position, &rotation, EventCharacterPartTransformMode::Get);
            const float amount = CleanFloat(CleanFloat(static_cast<float>(next) * 0.125f) - 1.0f);
            const float eased = CleanFloat(1.0f - CleanFloat(amount * amount));
            rotation.x = static_cast<std::uint16_t>(static_cast<std::int32_t>(((eased * 24.0f) * 8192.0f) / 45.0f));
            EV_scenApplyPosOrRotToPartOf(
                MDTP_RECORDER, static_cast<std::uint16_t>(MDTP_BUTTON_PARTS[index]), &position, &rotation, EventCharacterPartTransformMode::Absolute);
        }
        anyPressed = (anyPressed || *active != 156) && stop;
    }
    if (stopping)
        *stopping = anyPressed;
    return changed;
}
void UpdateMdtpHand(MdtpWork *work) {
    if (!work)
        return;
    Vector3 target;
    CalculateMdtpHandTarget(work->selectedButton, &target, work->pressFrames);
    BlendMdtpHandPosition(&work->handPosition, &target, work->handBlendFrames);
    LatchMotionBoneTarget(scenFindMotionwork(MDTP_PLAYER), 10, &work->handPosition);
    const Vector3i rotation{.x = 63352, .y = 0, .z = 0};
    ApplyCharacterHandRotation(ctrlGetCharaByName(MDTP_PLAYER), 0, 1, &rotation);
    if (--work->handBlendFrames < 0)
        work->handBlendFrames = 0;
}
void MaintainMdtpMotion(std::int32_t transition) {
    if (GetMotionAnimationId(scenFindMotionwork(MDTP_PLAYER), MotionRegion::LowerBody) == MDTP_MOTION &&
        GetMotionFrame(scenFindMotionwork(MDTP_PLAYER), MotionRegion::LowerBody) <= 40.0f)
        return;
    mtwkRequestMotion(scenFindMotionwork(MDTP_PLAYER), MDTP_MOTION, MotionRegion::LowerBody);
    if (transition) {
        mtwkSetRequestedRootMode(scenFindMotionwork(MDTP_PLAYER), 1);
        SetMotionFrameTime(scenFindMotionwork(MDTP_PLAYER), 32, MotionRegion::LowerBody);
    } else
        mtwkSetExtraFlag(scenFindMotionwork(MDTP_PLAYER), 17, 0);
    SetMotionLayerLoopStartTime(scenFindMotionwork(MDTP_PLAYER), 0, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(MDTP_PLAYER), 0, MotionRegion::LowerBody);
    SetMotionLayerEndFrame(scenFindMotionwork(MDTP_PLAYER), 114, MotionRegion::LowerBody);
    const Vector3 position{.x = 0.0040000002f, .y = -0.154f, .z = 1.126f};
    const Vector3i rotation{.x = 0, .y = 0, .z = 0};
    EV_AT_SetPos(MDTP_PLAYER, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(MDTP_PLAYER, EventTransformFlags::AllAxes, rotation);
}
} // namespace shenmue::s2
