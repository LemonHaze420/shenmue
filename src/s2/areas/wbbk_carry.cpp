#include "s2/areas/wbbk_carry.hpp"
#include "s2/areas/wbbk_models.hpp"
#include "s2/areas/wbbk_qte.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/motion_work.hpp"
#include "s2/scene_data.hpp"
namespace shenmue::s2::areas {
BookCarryMotion bookCarryMotion;
shen_name bookCameraAnchor, bookPlayerAnchor, bookStackAnchor;
std::int8_t bookCarryMode, bookCarryPhase, bookCarryVariant;
float bookCarryDistance;
std::int32_t bookCarrySegmentFrame;
const BookPoseKey *bookCarryTrack;
std::uint16_t bookDesiredCamera, bookCurrentCamera;
const float bookCarryThresholds[14]{110, 110, 110, 140, 160, 210, 130, 160, 170, 240, 230, 310, 240, 290};
const BookMotionPreset bookMotionPresets[22]{
    {.motion = 6309, .reserved = 0, .firstFrame = 1, .sideFrame = -1, .lastFrame = -1, .category = 1, .next = 1, .flags = 0},
    {.motion = 6306, .reserved = 0, .firstFrame = 1, .sideFrame = 19, .lastFrame = -1, .category = 1, .next = -2, .flags = 0},
    {.motion = 6305, .reserved = 0, .firstFrame = 1, .sideFrame = -1, .lastFrame = -1, .category = 0, .next = 9, .flags = 0},
    {.motion = 6304, .reserved = 0, .firstFrame = 1, .sideFrame = -1, .lastFrame = -1, .category = 2, .next = 4, .flags = 0},
    {.motion = 6301, .reserved = 0, .firstFrame = 1, .sideFrame = 15, .lastFrame = -1, .category = 2, .next = -2, .flags = 0},
    {.motion = 6300, .reserved = 0, .firstFrame = 1, .sideFrame = -1, .lastFrame = -1, .category = 0, .next = 9, .flags = 0},
    {.motion = 6313, .reserved = 0, .firstFrame = 1, .sideFrame = -1, .lastFrame = -1, .category = 3, .next = 7, .flags = 0},
    {.motion = 6311, .reserved = 0, .firstFrame = 1, .sideFrame = 10, .lastFrame = -1, .category = 3, .next = -2, .flags = 0},
    {.motion = 6310, .reserved = 0, .firstFrame = 3, .sideFrame = -1, .lastFrame = -1, .category = 0, .next = 9, .flags = 0},
    {.motion = 6305, .reserved = 0, .firstFrame = 22, .sideFrame = -1, .lastFrame = -1, .category = 4, .next = -1, .flags = 0},
    {.motion = 61449, .reserved = 0, .firstFrame = 1, .sideFrame = -1, .lastFrame = 26, .category = 1, .next = 11, .flags = 1},
    {.motion = 6308, .reserved = 0, .firstFrame = 67, .sideFrame = -1, .lastFrame = 114, .category = 0, .next = 18, .flags = 0},
    {.motion = 6307, .reserved = 0, .firstFrame = 35, .sideFrame = -1, .lastFrame = 1115, .category = 0, .next = -1, .flags = 0},
    {.motion = 6303, .reserved = 0, .firstFrame = 1, .sideFrame = -1, .lastFrame = 171, .category = 0, .next = -1, .flags = 0},
    {.motion = 6302, .reserved = 0, .firstFrame = 27, .sideFrame = -1, .lastFrame = 190, .category = 0, .next = -1, .flags = 0},
    {.motion = 6312, .reserved = 0, .firstFrame = 0, .sideFrame = -1, .lastFrame = 601, .category = 0, .next = -1, .flags = 0},
    {.motion = 61448, .reserved = 0, .firstFrame = 0, .sideFrame = -1, .lastFrame = 9, .category = 3, .next = 19, .flags = 1},
    {.motion = 6306, .reserved = 0, .firstFrame = 26, .sideFrame = -1, .lastFrame = 19, .category = 1, .next = 1, .flags = 0},
    {.motion = 6306, .reserved = 0, .firstFrame = 26, .sideFrame = 19, .lastFrame = -1, .category = 1, .next = 1, .flags = 0},
    {.motion = 6303, .reserved = 0, .firstFrame = 21, .sideFrame = 56, .lastFrame = -1, .category = 0, .next = 21, .flags = 0},
    {.motion = 6303, .reserved = 0, .firstFrame = 32, .sideFrame = 60, .lastFrame = -1, .category = 0, .next = 20, .flags = 0},
    {.motion = 6308, .reserved = 0, .firstFrame = 67, .sideFrame = -1, .lastFrame = 114, .category = 0, .next = 18, .flags = 2},
};
std::uint32_t SampleBookCarryTrack(const BookPoseKey *track, Vector3 &position, float frame, std::int32_t immediate) {
    const std::int32_t index = static_cast<std::int32_t>(frame);
    const float amount = frame - static_cast<float>(index), inverse = 1.0f - amount;
    const BookPoseKey &first = track[index];
    const BookPoseKey &second = track[index + 1];
    if (second.position.x < 0)
        return 0x10000;
    if (second.position.y < 0)
        return 0x20000;
    position = {.x = second.position.x * amount + inverse * first.position.x,
        .y = inverse * first.position.y + amount * second.position.y,
        .z = inverse * first.position.z + amount * second.position.z};
    const std::uint32_t yaw = InterpolateBookAngle(first.yaw, second.yaw, amount);
    Vector3i rotation;
    if (immediate) {
        EV_AT_SetPos(bookCameraAnchor, EventTransformFlags::AllAxes, position);
        rotation = {.x = 0, .y = static_cast<std::int32_t>(yaw), .z = 0};
    } else {
        Vector3 camera;
        EV_GetCharaPosition(bookCameraAnchor, CHARACTER_TRANSLATION_ROOT, &camera, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(bookCameraAnchor, &rotation, EventTransformFlags::Local);
        camera = {.x = position.x * .2f + camera.x * .80000001f, .y = position.y * .2f + camera.y * .80000001f, .z = camera.z * .80000001f + position.z * .2f};
        rotation.y = InterpolateBookAngle(static_cast<std::uint16_t>(rotation.y), static_cast<std::uint16_t>(yaw), .050000001f);
        EV_AT_SetPos(bookCameraAnchor, EventTransformFlags::AllAxes, camera);
    }
    EV_AT_SetAngY(bookCameraAnchor, EventTransformFlags::AllAxes, rotation);
    position.y = bookCarryMotion.raised ? 127.42f : 126.97f;
    if (bookCarryVariant < 2 && position.x < 343.29999f) {
        const std::int32_t next = bookMotionPresets[bookCarryMotion.current].category > 2 ? 16 : 10;
        bookCarryVariant = 2;
        bookCarryMotion.requested = static_cast<std::int16_t>(next);
        ChangeBookCarryMotion(next, 2);
        bookCarryMotion.pendingQte = 1;
    }
    return yaw;
}
Vector3 &SampleBookMotionRoot(std::int32_t preset, float frame) {
    static Vector3 root;
    MotionWork *motion = scenFindMotionwork(BOOK_RYO);
    const std::uint16_t animation = GetMotionAnimationId(motion, MotionRegion::LowerBody);
    const std::int32_t bone = GetMotionRigBoneId(motion);
    if (preset == 9)
        root = {.x = 0, .y = 0, .z = 0};
    else
        SampleNativeMotionRoot(animation, bone, frame, root);
    return root;
}
bool IsBookMotionComplete(std::int16_t preset) {
    const BookMotionPreset &definition = bookMotionPresets[preset];
    MotionWork *motion = scenFindMotionwork(BOOK_RYO);
    bool complete = definition.lastFrame >= 0 ? GetMotionFrame(motion, MotionRegion::LowerBody) == definition.lastFrame :
                                                SetMotionRegionFlag(motion, 65, 1, MotionRegion::LowerBody) != 0;
    if (bookMotionPresets[bookCarryMotion.current].sideFrame > 0 && GetMotionFrame(motion, MotionRegion::LowerBody) == definition.sideFrame) {
        complete = true;
        bookCarryMotion.side ^= 1;
    }
    if (bookMotionPresets[bookCarryMotion.requested].flags & 2)
        bookCarryMotion.side ^= 1;
    return complete;
}
void ChangeBookCarryMotion(std::int32_t preset, std::int8_t transition) {
    MotionWork *motion = scenFindMotionwork(BOOK_RYO);
    if (bookCarryMotion.current == 10 || bookCarryMotion.current == 16)
        bookCarryMotion.raised = 0;
    bookCarryMotion.transition = transition == 1 ? 1 : static_cast<std::uint8_t>((transition != 0) + 2);
    const BookMotionPreset &old = bookMotionPresets[bookCarryMotion.current];
    const BookMotionPreset &next = bookMotionPresets[preset];
    if (next.flags & 1) {
        if (!(old.flags & 1)) {
            mtwkRequestMotion(motion, bookMotionPresets[1].motion, MotionRegion::RightArm);
            mtwkRequestMotion(motion, bookMotionPresets[1].motion, MotionRegion::LeftArm);
        }
    } else if (old.flags & 1) {
        SetMotionRegionFlag(motion, 73, 0, MotionRegion::RightArm);
        SetMotionRegionFlag(motion, 73, 0, MotionRegion::LeftArm);
    }
    if (bookCarryMotion.side)
        SetMotionRegionFlag(motion, 69, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(motion, next.motion, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(motion, next.firstFrame, MotionRegion::LowerBody);
    bookCarryMotion.current = static_cast<std::int16_t>(preset);
    bookCarryMotion.frame = next.firstFrame;
    bookCarryMotion.previousRootX = 0;
}
void SelectBookCarryInput() {
    bookCarryMotion.requested = 9;
    const std::uint16_t keys = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_HELD));
    const std::uint8_t trigger = static_cast<std::uint8_t>(EVT_GetControllerInput(0, !GetS2IsEnglishVersion() ? 4 : 3));
    std::int32_t input = (keys & 0x1000U) != 0;
    if (static_cast<std::uint8_t>(trigger - 1) <= 249U)
        input = 2;
    if (trigger >= 251)
        input = 3;
    if (input >= 2 && bookCarryDistance > bookCarryThresholds[bookModelState.currentBook] && bookCarryVariant > 2)
        input = 1;
    bookCarryMotion.input = static_cast<std::uint8_t>(input);
    const std::int32_t category = bookMotionPresets[bookCarryMotion.current].category;
    if ((category & 0xfb) == 0) {
        if (input == 1)
            bookCarryMotion.requested = 0;
        else if (input == 2)
            bookCarryMotion.requested = 3;
        else if (input == 3)
            bookCarryMotion.requested = 6;
    }
    if (static_cast<std::uint32_t>(category - 1) <= 2U) {
        if (input == 0)
            bookCarryMotion.requested = static_cast<std::int16_t>(category * 3 - 1);
        else if (static_cast<std::uint32_t>(input - 1) <= 2U)
            bookCarryMotion.requested = static_cast<std::int16_t>(input * 3 - 2);
    }
}
std::int32_t AdvanceBookCarryMotion() {
    const BookMotionPreset &current = bookMotionPresets[bookCarryMotion.current];
    if (bookCarryMotion.current == bookCarryMotion.requested || current.category == 0) {
        const std::int16_t dispatch =
            static_cast<std::int16_t>(static_cast<std::uint8_t>(current.category) | (static_cast<std::uint16_t>(static_cast<std::uint8_t>(current.next)) << 8));
        if (dispatch >= 0 && IsBookMotionComplete(bookCarryMotion.current)) {
            bookCarryMotion.requested = current.next;
            ChangeBookCarryMotion(current.next, 1);
            return bookCarryMotion.requested;
        }
        return 0;
    }
    if (static_cast<std::uint8_t>(current.category - 1) > 2) {
        if (current.category == 4) {
            bookCarryMotion.side = 0;
            ChangeBookCarryMotion(bookCarryMotion.requested, 0);
            return bookCarryMotion.requested;
        }
        return 0;
    }
    const std::int16_t complete = IsBookMotionComplete(bookCarryMotion.current);
    if (!complete)
        return 0;
    if (complete & 2)
        bookCarryMotion.side ^= 1;
    if (bookMotionPresets[bookCarryMotion.requested].flags & 2)
        bookCarryMotion.side ^= 1;
    ChangeBookCarryMotion(bookCarryMotion.requested, 2);
    return bookCarryMotion.requested;
}
void PositionBookCarrier(Vector3 &position, std::int32_t yaw) {
    Vector3 &root = SampleBookMotionRoot(bookCarryMotion.current, bookCarryMotion.frame);
    const float delta = root.x - bookCarryMotion.previousRootX;
    bookCarryMotion.previousRootX = root.x;
    bookCarryMotion.radialOffset += delta;
    const float radius = bookCarryMotion.radialOffset;
    if (bookCarryMotion.current != 9)
        bookCarryMotion.radialOffset *= .98000002f;
    Vector3 previous;
    EV_GetCharaPosition(bookPlayerAnchor, CHARACTER_TRANSLATION_ROOT, &previous, EventTransformFlags::Local);
    const Vector3i rotation{.x = 0, .y = yaw, .z = 0};
    EV_AT_SetPos(bookPlayerAnchor, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(bookPlayerAnchor, EventTransformFlags::AllAxes, rotation);
    position.x += cos_fixed16(static_cast<std::uint16_t>(yaw)) * radius;
    position.z -= sin_fixed16(static_cast<std::int16_t>(yaw)) * radius;
    EV_AT_SetPos(BOOK_RYO, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(BOOK_RYO, EventTransformFlags::AllAxes, rotation);
}
void UpdateBookCarryLoop() {
    bookCarryMotion.transition = 0;
    bookCarryMotion.frame = GetMotionFrame(scenFindMotionwork(BOOK_RYO), MotionRegion::LowerBody);
    SelectBookCarryInput();
    if (bookCarryVariant == 3 && bookCarryDistance > bookQteTimings[bookQteTimingIndex]) {
        ++bookQteTimingIndex;
        if (bookCarryThresholds[bookModelState.currentBook] > bookCarryDistance)
            bookCarryMotion.pendingQte = 1;
    }
    Vector3 position;
    std::uint32_t yaw = SampleBookCarryTrack(bookCarryTrack, position, bookCarryDistance * .125f, 0);
    if (yaw == 0x10000) {
        bookCarryDistance = 0;
        bookCarrySegmentFrame = 0;
        bookCarryVariant = 3;
        bookCarryTrack = bookContinuationCarryTracks[bookModelState.currentBook];
        yaw = SampleBookCarryTrack(bookCarryTrack, position, 0, 0);
    }
    if (position.x - 343.29999f < -.69998777f && bookCarryVariant == 2) {
        bookCarryDistance = 0;
        bookCarrySegmentFrame = 0;
        bookCarryVariant = 3;
        bookCarryTrack = bookContinuationCarryTracks[bookModelState.currentBook];
    }
    if (yaw == 0x20000) {
        bookCarryPhase = 8;
        bookModelState.animationFrame = 0;
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_RYO), 1);
        SetMotionFrameTime(scenFindMotionwork(BOOK_RYO), 10, MotionRegion::LowerBody);
        mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), 0x188c, MotionRegion::LowerBody);
        SetMotionRegionEvalTime(scenFindMotionwork(BOOK_RYO), 40, MotionRegion::LowerBody);
        return;
    }
    if (CheckBookCarryQte() == 1) {
        if (bookCarryMotion.current == 11) {
            if (bookCarryMotion.requested == 11)
                bookCarryMotion.requested = 9;
        } else if (static_cast<std::uint16_t>(bookCarryMotion.requested - 19) <= 1)
            bookCarryMotion.requested = 9;
        ChangeBookCarryMotion(bookCarryMotion.requested, 1);
        bookCarryMotion.pendingQte = 0;
    } else if (bookCarryMotion.pendingQte) {
        if (bookMotionPresets[bookCarryMotion.current].category == 3) {
            if (bookCarryVariant != 3 || bookCarryThresholds[bookModelState.currentBook] > bookCarryDistance + 70)
                bookCarryMotion.requested = 19;
        } else if (bookCarryVariant != 3 || bookCarryThresholds[bookModelState.currentBook] > bookCarryDistance + 20)
            bookCarryMotion.requested = 11;
    }
    const std::int32_t transition = AdvanceBookCarryMotion();
    if (bookCarryMotion.current == 11 && bookCarryMotion.pendingQte && bookCarryMotion.frame > 60) {
        bookCarryMotion.pendingQte = 0;
        StartBookCarryQte(0);
    }
    if (transition == 19) {
        bookCarryMotion.pendingQte = 0;
        StartBookCarryQte(2);
    }
    yaw = SampleBookCarryTrack(bookCarryTrack, position, bookCarryDistance * .125f, 0);
    PositionBookCarrier(position, static_cast<std::int32_t>(yaw));
    const float nextZ = SampleBookMotionRoot(bookCarryMotion.current, bookCarryMotion.frame + 1).z;
    bookCarryDistance -= (nextZ - SampleBookMotionRoot(bookCarryMotion.current, bookCarryMotion.frame).z) * 25;
    switch (bookCarryMotion.transition) {
    case 1:
        SetMotionFrameTime(scenFindMotionwork(BOOK_RYO), 3, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_RYO), 1);
        break;
    case 2:
        SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 71, 0, MotionRegion::LowerBody);
        break;
    case 3:
        SetMotionFrameTime(scenFindMotionwork(BOOK_RYO), 5, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_RYO), 2);
        break;
    default:
        break;
    }
    if (static_cast<std::uint32_t>(bookModelState.currentBook - 9) <= 1 && bookCarryDistance > 240)
        bookDesiredCamera = 2007;
}
} // namespace shenmue::s2::areas
