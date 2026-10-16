#include "s2/areas/wbbk_motion.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"

namespace shenmue::s2::areas {
BookSmoothMovement bookSmoothMovement;
BookCompanionControl bookCompanionControl;
std::int32_t InterpolateBookAngle(std::uint16_t from, std::uint16_t to, float amount) {
    const float start = static_cast<float>(from);
    float end = static_cast<float>(to);
    if (start - end > 32768.0f || start - end < -32767.0f) {
        if (start > end)
            return static_cast<std::int32_t>(((end + 65536.0f - start) * amount) + start);
        end -= 65536.0f;
    }
    return static_cast<std::int32_t>(((end - start) * amount) + start);
}
namespace {
void BookCompanionIdle(void *) {
}
void BookCompanionWaitCopy(void *) {
    if (!mtwkSetExtraFlag(scenFindMotionwork(BOOK_SAI), 58, 1))
        bookCompanionControl.state = 0;
}
void BookCompanionChangeSide(void *) {
    if (bookCompanionControl.frame == 1) {
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_SAI), 0);
        SetMotionFrameTime(scenFindMotionwork(BOOK_SAI), 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(BOOK_SAI), 17, 0);
        const Vector3 position{.x = bookCompanionControl.side ? 328.29999f : 332.70001f, .y = 126.98f, .z = 232.81f};
        SetBookActorPose(BOOK_SAI, position, bookCompanionControl.side ? 32768 : 26214);
        mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0xf006, MotionRegion::LowerBody);
    } else if (bookCompanionControl.frame == 10) {
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_SAI), 1);
        SetMotionFrameTime(scenFindMotionwork(BOOK_SAI), 20, MotionRegion::LowerBody);
        mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0x1897, MotionRegion::LowerBody);
    }
}
} // namespace

void SetBookActorPose(shen_name actor, const Vector3 &position, std::int32_t yaw) {
    const Vector3i rotation{.x = 0, .y = yaw, .z = 0};
    SetMotionRegionFlag(scenFindMotionwork(actor), 71, 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(actor), 0);
    SetMotionFrameTime(scenFindMotionwork(actor), 0, MotionRegion::LowerBody);
    EV_AT_SetPos(actor, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, rotation);
}
void SetBookActorMotion(shen_name actor, std::uint16_t motion) {
    SetMotionRegionFlag(scenFindMotionwork(actor), 71, 0, MotionRegion::LowerBody);
    SetMotionFrameTime(scenFindMotionwork(actor), 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(actor), 0);
    mtwkRequestMotion(scenFindMotionwork(actor), motion, MotionRegion::LowerBody);
}
void SetBookCompanionPose(const Vector3 &position, std::int32_t yaw) {
    if (mtwkSetExtraFlag(scenFindMotionwork(BOOK_SAI), 58, 1))
        CopyMotionTransition(scenFindMotionwork(BOOK_SAI), nullptr, 0, 1, 0);
    SetBookActorPose(BOOK_SAI, position, yaw);
    mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0x1897, MotionRegion::LowerBody);
    bookCompanionControl.state = 0;
}
void BookCompanionControlCallback(void *data) {
    constexpr HLib::TaskCallback callbacks[]{BookCompanionIdle, BookCompanionWaitCopy, BookCompanionChangeSide};
    callbacks[bookCompanionControl.state](data);
    bookCompanionControl.frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(bookCompanionControl.frame) + 1U);
}
void BookSmoothMovementCallback(void *) {
    BookSmoothMovement &movement = bookSmoothMovement;
    movement.frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(movement.frame) + 1U);
    const float fraction = static_cast<float>(movement.frame) / static_cast<float>(movement.duration);
    float amount = 0;
    switch (movement.flags & 0xffff0000U) {
    case 0:
        amount = fraction;
        break;
    case 0x10000:
        amount = (1.0f - cos_fixed16(static_cast<std::uint16_t>(static_cast<std::int32_t>(fraction * 32768.0f)))) * .5f;
        break;
    case 0x20000:
        amount = 1.0f - cos_fixed16(static_cast<std::uint16_t>(static_cast<std::int32_t>(fraction * 16384.0f)));
        break;
    default:
        break;
    }
    const float remaining = 1.0f - amount;
    const Vector3 position{.x = movement.flags & 1U ? movement.fromPosition.x * remaining + movement.toPosition.x * amount : movement.fromPosition.x,
        .y = movement.flags & 2U ? movement.fromPosition.y * remaining + movement.toPosition.y * amount : movement.fromPosition.y,
        .z = movement.flags & 4U ? movement.fromPosition.z * remaining + movement.toPosition.z * amount : movement.fromPosition.z};
    const Vector3i rotation{
        .x = InterpolateBookAngle(static_cast<std::uint16_t>(movement.fromRotation.x), static_cast<std::uint16_t>(movement.toRotation.x), amount),
        .y = InterpolateBookAngle(static_cast<std::uint16_t>(movement.fromRotation.y), static_cast<std::uint16_t>(movement.toRotation.y), amount),
        .z = InterpolateBookAngle(static_cast<std::uint16_t>(movement.fromRotation.z), static_cast<std::uint16_t>(movement.toRotation.z), amount)};
    if (movement.flags & 0x100U)
        AttachScenarioCharacter(movement.character, BOOK_RYO, 21, &position, &rotation);
    else {
        EV_AT_SetPos(movement.character, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(movement.character, EventTransformFlags::AllAxes, rotation);
    }
    if (movement.frame >= movement.duration)
        HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2::areas
