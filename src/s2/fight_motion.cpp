#include "s2/fight_stage.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/model_component.hpp"
#include "s2/osage.hpp"
#include "s2/motion.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_control.hpp"
#include "s2/character.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/camera.hpp"
#include "s2/save.hpp"
#include "s2/fight.hpp"
#include "s2/sound_program.hpp"
#include "s2/conversation.hpp"
#include "s2/face.hpp"
#include "s2/pak.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/ui.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/legacy_sprite.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2 {

std::uint8_t g_vfRoundResultCounters[4]{};

// Both observed counters advance with byte wrapping and are then capped by their authored limits.
// @verified - p
void vfAddRoundResults(std::uint8_t won, std::uint8_t lost) {
    const auto first = static_cast<std::uint8_t>(g_vfRoundResultCounters[0] + won), second = static_cast<std::uint8_t>(g_vfRoundResultCounters[1] + lost);
    g_vfRoundResultCounters[0] = first > g_vfRoundResultCounters[2] ? g_vfRoundResultCounters[2] : first;
    g_vfRoundResultCounters[1] = second > g_vfRoundResultCounters[3] ? g_vfRoundResultCounters[3] : second;
}
namespace {

const std::byte *motionMarkerCursor{};
std::uint8_t packedFightSettings[16]{};
constexpr std::uint8_t markerWidths[32]{12, 4, 8, 16, 20, 12, 8, 4, 0, 16, 8, 12, 4, 4, 16, 8, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 4, 8, 8, 8};
constexpr std::uint8_t mirroredMoveIds[48]{0, 2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11, 13, 15, 14, 16, 17, 18, 19, 20, 21, 23, 22, 25, 24, 27, 26, 28, 29, 31, 30,
    33, 32, 34, 35, 37, 36, 38, 39, 40, 41, 42, 0, 0, 0, 0, 0};
constexpr float impulseTableA[16]{1, 1, 1, 1.2f, 1.4f, 1.6f, 1.8f, 2, 2.1f, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f, 2.7f, 2.8f};
constexpr float impulseTableB[16]{1, 1, 1, 1, 1, 1, 1, 1.2f, 1.4f, 1.6f, 2, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f};
constexpr std::uint16_t cameraSectors[8]{0x20, 0xa0, 0x80, 0x90, 0x10, 0x50, 0x40, 0x60};

constexpr std::uint8_t defaultCameraScripts[22][20]{
    {0,127,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0},
    {72,0,127,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0},
    {136,0,127,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0},
    {40,0,127,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0},
    {24,0,127,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0},
    {19,0,127,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
    {19,0,127,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
    {19,0,127,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
    {7,0,127,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
    {15,0,127,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
    {4,127,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {4,127,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {1,127,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {240,241,127,0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0,0},
    {240,242,127,0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0,0},
    {240,243,127,0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0,0},
    {240,250,127,0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0,0},
    {8,0,127,0,0,0,0,0,0,0,1,10,0,0,0,0,0,0,0,0},
    {5,0,127,0,0,0,0,0,0,0,1,5,0,0,0,0,0,0,0,0},
    {8,0,1,127,0,0,0,0,0,0,1,3,1,1,0,0,0,0,0,0},
    {8,0,2,127,0,0,0,0,0,0,1,3,1,1,0,0,0,0,0,0},
    {8,0,3,127,0,0,0,0,0,0,1,3,1,1,0,0,0,0,0,0}};
void ResetCameraPhase(VFSlotRuntimeState &slot) {
    slot.parameter89 = 0;
    slot.parameter8c = 0;
    slot.parameterCc = 0;
    slot.parameter12a = 0;
    slot.resetIndex = 0x7f7f;
    slot.cameraFlags = (slot.cameraFlags & ~0x2001U) | 1U;
}

template <class T> T MarkerValue(std::size_t offset) {
    T value;
    std::memcpy(&value, motionMarkerCursor + offset, sizeof(value));
    return value;
}
std::int32_t AuthoredOffset(const std::byte *data) {
    const auto *p = reinterpret_cast<const std::uint8_t *>(data);
    const std::uint32_t bits = (std::uint32_t(p[0]) << 24) | (std::uint32_t(p[1]) << 16) | (std::uint32_t(p[2]) << 8) | p[3];
    return std::bit_cast<std::int32_t>(bits);
}
float AuthoredFixed(const std::byte *data) {
    return static_cast<float>(AuthoredOffset(data)) * 0.000015258789f;
}
float MarkerFixed(std::size_t offset) {
    return AuthoredFixed(motionMarkerCursor + offset);
}
std::uint16_t SignedByteWord(std::size_t offset) {
    return static_cast<std::uint16_t>(MarkerValue<std::int8_t>(offset));
}
void DecrementFrame(std::uint16_t &frame) {
    if (frame)
        --frame;
}
void DecrementFrame(std::int16_t &frame) {
    if (frame)
        frame = static_cast<std::int16_t>(static_cast<std::uint16_t>(frame) - 1);
}


constexpr std::uint16_t fightVoiceRows[12][17]{
    {46,0,0,0,0,2509,0,0,0,2613,2615,2614,2614,2815,0,3380,0},
    {85,0,0,0,0,2512,0,0,0,2614,2613,2613,2613,2815,0,3382,0},
    {43,0,0,0,0,2511,0,0,0,2613,2615,2614,2614,2815,0,3383,0},
    {44,0,0,0,0,2510,0,0,0,2614,2613,2613,2613,2815,0,3387,0},
    {48,0,0,0,0,2513,0,0,0,2613,2615,2614,2614,2815,0,3388,0},
    {45,0,0,0,0,2515,0,0,0,2614,2613,2613,2613,2815,0,3386,0},
    {51,0,0,0,0,0,0,0,0,2613,2615,2614,2614,2815,0,0,0},
    {50,0,0,0,0,0,0,0,0,2614,2613,2613,2613,2815,0,0,0},
    {49,584,97,309,2343,0,0,0,0,0,0,0,0,0,0,0,50},
    {50,109,580,307,2348,2514,0,0,0,2614,2613,2613,2613,2814,0,3388,0},
    {53,582,576,2593,2345,2519,2505,2493,2553,2621,2623,2625,2627,2816,0,3393,3414},
    {53,581,577,2594,2347,2520,2507,2495,2550,2622,2624,2626,2628,2817,0,3394,3411}};
constexpr std::uint16_t fightDefeatVoiceRow[17]{
    54,583,578,2595,2344,2522,2508,2496,2554,2622,2624,2626,2628,2817,0,3397,3413};
const std::uint16_t *const fightVoices[17]{nullptr, fightVoiceRows[0], fightVoiceRows[1], fightVoiceRows[2], fightVoiceRows[3], fightVoiceRows[4],
    fightVoiceRows[5], fightVoiceRows[6], fightVoiceRows[7], fightVoiceRows[8], fightVoiceRows[8], fightVoiceRows[8], fightVoiceRows[8], fightVoiceRows[9],
    fightVoiceRows[10], fightVoiceRows[11], fightDefeatVoiceRow};
std::uint16_t motionFrameEvent{};
constexpr std::uint8_t frameMarkerWidths[57]{
    0,4,4,16,8,8,8,8,4,4,4,8,8,4,8,8,4,4,8,8,8,4,8,8,8,16,12,28,4,
    8,8,4,8,8,8,8,8,12,8,12,8,8,4,8,8,4,4,4,4,16,8,12,8,4,8,4,4};
constexpr float guardImpactMultipliers[16]{
    6,6.4f,6.8f,7.2f,7.6f,8,8.4f,8.8f,9.2f,9.6f,10,10.4f,10.8f,11.2f,11.6f,12};
template <class T> T ActorValue(const VFActor *actor, std::size_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(actor) + offset, sizeof(value));
    return value;
}
template <class T> void SetActorValue(VFActor *actor, std::size_t offset, T value) {
    std::memcpy(reinterpret_cast<std::byte *>(actor) + offset, &value, sizeof(value));
}
}

// The low six bits select a byte; 0x40/0x80 select its nibbles.
// @verified - p
std::uint8_t vfMotnNibbleGet(std::int8_t code) {
    const auto bits = static_cast<std::uint8_t>(code);
    const auto index = bits & 0x3f;
    if (index >= 16)
        return 0;
    const auto value = packedFightSettings[index];
    if (bits & 0x40)
        return value & 0xf;
    if (code < 0)
        return value >> 4;
    return value;
}
// Authored values are truncated, not clamped to a nibble.
// @verified - p
std::uint8_t vfMotnNibbleSet(std::int8_t code, std::int32_t value) {
    const auto bits = static_cast<std::uint8_t>(code);
    const auto index = static_cast<std::uint8_t>(bits & 0x3f);
    if (index < 16) {
        const auto byte = value < 0 ? std::uint8_t{} : static_cast<std::uint8_t>(value);
        auto &destination = packedFightSettings[index];
        if (bits & 0x40)
            destination = (destination & 0xf0) | byte;
        else if (code >= 0)
            destination = byte;
        else
            destination = (destination & 0xf) | static_cast<std::uint8_t>(16 * byte);
    }
    return index;
}

// The dispatcher and all non-null entries of the 32-entry motnFuncs table.
// @verified - p
void vfMotnDispatch(VFActor *self) {
    bool stop;
    do {
        stop = false;
        const auto tag = MarkerValue<std::uint8_t>(0);
        switch (tag) {
        case 0:
            self->reactionProgram = self->mirroredReactionProgram = motionMarkerCursor;
            stop = true;
            break;
        case 1:
            self->requestedAnimation = MarkerValue<std::int16_t>(2);
            break;
        case 2: {
            auto end = self->currentAnimationEndFrame;
            auto last = MarkerValue<std::uint16_t>(6);
            auto middle = MarkerValue<std::uint16_t>(4);
            auto first = MarkerValue<std::uint16_t>(2);
            if (last > end)
                last = end;
            if (middle > end) {
                middle = end;
                --end;
            }
            self->reactionMiddleFrame = static_cast<std::int16_t>(middle);
            self->reactionEndFrame = last;
            if (first > end)
                first = end;
            self->actionTriggerPhase = 1;
            self->hitStateFlags |= 2;
            self->parameter2d0 = static_cast<std::int16_t>(first);
            break;
        }
        case 3:
            self->parameter2d6 = MarkerValue<std::uint16_t>(2);
            self->parameter2de = MarkerValue<std::uint16_t>(4);
            self->parameter2e0 = MarkerValue<std::uint16_t>(6);
            self->groundOffset = MarkerFixed(8);
            self->motionContactFlags = MarkerValue<std::uint8_t>(1);
            self->parameter31e = MarkerValue<std::uint16_t>(12);
            break;
        case 4:
            self->moveId = MarkerValue<std::uint8_t>(1);
            self->guardState = MarkerValue<std::int8_t>(2);
            self->guardStrength = MarkerValue<std::int8_t>(3);
            self->impulseMagnitude = MarkerFixed(4);
            self->hitDirection = MarkerValue<std::uint16_t>(8);
            self->hitParameter = MarkerValue<std::uint16_t>(10);
            self->parameter2eb = MarkerValue<std::uint8_t>(12);
            self->parameter30e = MarkerValue<std::uint8_t>(13);
            self->parameter32c = MarkerValue<std::uint8_t>(14);
            self->parameter32d = MarkerValue<std::uint8_t>(15);
            self->parameter32e = MarkerValue<std::uint8_t>(16);
            self->parameter34d = MarkerValue<std::uint8_t>(17);
            self->parameter31a[0] &= 0xff00;
            if (self->moveFlags & 0x40)
                self->moveId = mirroredMoveIds[self->moveId];
            break;
        case 5: {
            std::int8_t strength{};
            if (!(self->secondaryMotionFlags & 0xc00)) {
                self->hitParameter = MarkerValue<std::uint16_t>(2);
                strength = MarkerValue<std::int8_t>(1);
            }
            if (self->secondaryMotionFlags & 0x400)
                self->guardHitParameter = strength;
            else
                self->guardStrength = strength;
            self->impulseMagnitude = MarkerFixed(4);
            self->facingAngleDelta = MarkerValue<std::int16_t>(8);
            ++self->activeContactCount;
            self->parameter350 = MarkerValue<std::uint16_t>(10);
            break;
        }
        case 6:
            self->parameter2e0 = MarkerValue<std::uint16_t>(2);
            self->parameter32a = 0;
            self->facingAngleDelta = MarkerValue<std::int16_t>(4);
            break;
        case 7:
            self->facingAngleDelta = MarkerValue<std::int16_t>(2);
            break;
        case 8:
            self->reactionProgram = self->mirroredReactionProgram = motionMarkerCursor + 4;
            if (self->moveFlags & 0x40) {
                self->hitVector.x = -self->hitVector.x;
                self->facingAngleDelta = static_cast<std::int16_t>(-self->facingAngleDelta);
                self->hitDirection = static_cast<std::uint16_t>(-self->hitDirection);
            }
            stop = true;
            break;
        case 9:
            self->hitVector.x = MarkerFixed(4);
            self->hitVector.y = MarkerFixed(8);
            self->impulseDelay = static_cast<std::uint16_t>(static_cast<int>(MarkerValue<std::int8_t>(1) * self->inverseScale));
            self->slideVelocity.y = std::fabs(MarkerFixed(12) * (self->scale * self->scale * self->game->gravityScale));
            self->parameter2e2 = MarkerValue<std::int8_t>(2);
            break;
        case 10:
            if (!self->reactionEventCount)
                self->reactionEntries = motionMarkerCursor;
            self->reactionEventCount = static_cast<std::int8_t>(static_cast<std::uint8_t>(self->reactionEventCount) + 1);
            break;
        case 11:
            self->reactionEntryFlags = MarkerValue<std::uint32_t>(4) ^ ((self->moveFlags & 0x40) ? 1u : 0u);
            self->parameter2ca = self->reactionEntryFlags & 0x7fff;
            self->reactionEntryMask = MarkerValue<std::uint32_t>(8);
            self->parameter31a[1] = MarkerValue<std::uint16_t>(2);
            break;
        case 12:
            self->eventFrame30a = SignedByteWord(1);
            self->eventFrame30c = SignedByteWord(2);
            break;
        case 13:
            self->eventFrame318 = SignedByteWord(1);
            self->eventFrame328 = SignedByteWord(2);
            break;
        case 14: {
            const float first = MarkerFixed(4), middle = MarkerFixed(12), last = MarkerFixed(8);
            auto target = MarkerValue<std::uint8_t>(1);
            auto strength = static_cast<std::uint8_t>(self->guardStrength);
            const float distance = self->contactDistances[self->actionIndex];
            float span = first - last, numerator = distance - last;
            const auto second = MarkerValue<std::uint8_t>(2);
            if (second) {
                if (!(middle >= distance)) {
                    strength = second;
                    span = middle - last;
                } else {
                    span = first - middle;
                    target = second;
                    numerator = distance - middle;
                }
            }

            const bool negative = 0.0f > span;
            const float minimum = negative ? span : 0.0f, maximum = negative ? 0.0f : span;
            const float raised = minimum > numerator ? minimum : numerator, clamped = maximum < raised ? maximum : raised;
            self->guardStrength = static_cast<std::int8_t>(target + static_cast<int>((float(strength) - float(target)) * (clamped / span)));
            break;
        }
        case 15:
            self->parameter33c[0] = MarkerValue<std::uint16_t>(2);
            self->pairContactPriority = MarkerValue<std::int16_t>(4);
            self->parameter33c[1] = SignedByteWord(1);
            self->pairContactEndFrame = SignedByteWord(6);
            break;
        case 18:
            self->moveFlags &= ~0x440u;
            self->hitStateFlags &= ~0x200000u;
            break;
        case 19:
            self->moveFlags ^= 0x40;
            break;
        case 26:
            if (self->pairedActor->fightStyle == MarkerValue<std::int8_t>(1)) {
                self->hitParameter = MarkerValue<std::uint16_t>(4);
                self->guardStrength = MarkerValue<std::int8_t>(2);
            }
            break;
        case 27:
            self->knockbackAcceleration = MarkerFixed(4) * self->game->knockbackAmount;
            break;
        case 28:
            self->someAnimation[0] = MarkerValue<std::uint16_t>(2);
            break;
        case 29:
            self->parameter412 = MarkerValue<std::uint16_t>(((self->moveFlags ^ self->actor->moveFlags) & 0x40) ? 4 : 2);
            break;
        case 30:
            self->markerMask = self->savedMarkerMask = MarkerValue<std::uint32_t>(4);
            break;
        case 31:
            if (MarkerValue<std::uint8_t>(1) == 1)
                self->knockbackAcceleration = impulseTableA[vfMotnNibbleGet(74)];
            else if (MarkerValue<std::uint8_t>(1) == 2)
                self->knockbackAcceleration = impulseTableB[vfMotnNibbleGet(69)];
            break;
        default:
            break;
        }
        motionMarkerCursor += markerWidths[tag];
    } while (!stop);
}

// Marker state is borrowed directly from the registered MOTI bank.
// @verified - p
void vfApplyMotionDescriptor(VFActor *self) {
    self->requestedAnimation = 0;
    self->actionTriggerPhase = 0;
    self->facingAngleDelta = 0;
    self->reactionEventCount = 0;
    self->reactionEntryFlags = 0;
    self->eventFrame30a = 0;
    self->parameter2eb = 0;
    self->parameter31e = self->parameter2de = self->parameter2e0 = 0;
    self->someAnimation[0] = self->someAnimation[1] = 0;
    self->parameter34d = 0;
    self->parameter412 = 0;
    const auto pending = static_cast<std::uint16_t>(self->pendingAnimation);
    if (!pending)
        return;
    const auto id = pending & 0xfff;
    motionMarkerCursor = FindLoadedMotionMetadata(id).data();
    if (!motionMarkerCursor)
        return;
    self->savedHitStateFlags = self->hitStateFlags;
    self->savedMotionFlags = self->motionFlags;
    self->savedSecondaryMotionFlags = self->secondaryMotionFlags;
    self->markerMask = self->savedMarkerMask = 0;
    self->motionFlags = MarkerValue<std::uint32_t>(0);
    self->secondaryMotionFlags = MarkerValue<std::uint32_t>(8);
    self->animationHalfPhase = MarkerValue<std::int8_t>(4);
    self->parameter2d9 = MarkerValue<std::int8_t>(5);
    self->parameter2dc = MarkerValue<std::uint16_t>(6);
    motionMarkerCursor += 12;
    auto flags = self->motionFlags & 0xffdfffff;
    if (self->motionFlags & 0x40)
        self->facingAngleDelta = static_cast<std::int16_t>(0x8000);
    if ((flags & 0x8100) == 0x8100)
        flags = (flags & 0xfffe7fff) | 0x10000;
    self->hitStateFlags = (flags & 0x80000) ? flags & ~0x4000u : flags;
    self->activeMotionFlags = self->secondaryMotionFlags;
    const auto *sequence = FindLoadedMotionSequence(id);
    const auto frames = sequence ? sequence->frameCount : 0;
    const auto end = static_cast<std::uint16_t>(frames - 1 - ((self->motionFlags & 0x20) ? 1 : 0));
    self->parameter2d0 = 0;
    self->currentAnimationEndFrame = self->reactionEndFrame = end;
    self->reactionMiddleFrame = static_cast<std::int16_t>(end);
    self->animationTimeParameter = float(end) - self->scale;
    vfMotnDispatch(self);
    auto *figure = static_cast<FigurePose *>(FindModelComponent(self->character, MODEL_FIGURE_POSE));
    if ((!figure || !(figure->flags & 1)) && !(self->savedMarkerMask & 4))
        figpAssignModeForCharacter(self->character, 0);
}


// @verified - p
void vfUpdateMoveVelocityWithKnockback(VFActor *self, float acceleration) {
    SetMotionSpeed(self->motion, acceleration, MotionRegion::LowerBody);
    const float ratio = self->knockbackAcceleration / acceleration;
    self->inverseScale = 1.0f / acceleration;
    self->scale = acceleration;
    for (Vector3 *velocity : {&self->knockbackVelocity, &self->slideVelocity, &self->pendingKnockback}) {
        velocity->x *= ratio;
        velocity->y *= ratio * ratio;
        velocity->z *= ratio;
    }
}


// @verified - p
void vfRescaleReactionFrameWindows(VFActor *self) {
    DecrementFrame(self->parameter2d0);
    if (static_cast<std::uint16_t>(self->reactionMiddleFrame) != self->currentAnimationEndFrame)
        DecrementFrame(self->reactionMiddleFrame);
    if (self->reactionEndFrame != self->currentAnimationEndFrame)
        DecrementFrame(self->reactionEndFrame);
    DecrementFrame(self->eventFrame30a);
    DecrementFrame(self->eventFrame30c);
    DecrementFrame(self->eventFrame318);
    DecrementFrame(self->eventFrame328);
    DecrementFrame(self->parameter2d6);
    DecrementFrame(self->parameter2de);
    DecrementFrame(self->parameter2e0);
    DecrementFrame(self->parameter31e);
    DecrementFrame(self->parameter2e2);
    DecrementFrame(self->parameter31a[1]);
    DecrementFrame(self->parameter33c[1]);
    DecrementFrame(self->pairContactEndFrame);
    const auto first = static_cast<std::uint16_t>(self->parameter2d0);
    const auto middle = static_cast<std::uint16_t>(self->reactionMiddleFrame);
    if (first >= middle) {
        auto next = static_cast<std::uint16_t>(first + 1);
        if (next > self->currentAnimationEndFrame)
            next = self->currentAnimationEndFrame;
        auto last = static_cast<std::uint16_t>(next + static_cast<std::uint16_t>(self->reactionEndFrame - middle));
        if (last > self->currentAnimationEndFrame)
            last = self->currentAnimationEndFrame;
        self->reactionMiddleFrame = static_cast<std::int16_t>(next);
        self->reactionEndFrame = last;
    }
}


// @verified - p
void vfApplyHitReactionAndStartMotion(VFActor *self) {
    self->moveFlags &= ~0x200u;
    auto hit = self->hitStateFlags;
    self->slideVelocity.y = 0;
    self->queuedReactionEvent = 0;
    if (!self->pendingAnimation) {
        self->currentAnimation = 0;
        self->hitStateFlags = hit & 0xff7ebfb7;
        return;
    }
    self->reserved4a2 = self->currentAnimation;
    self->moveFlags |= 0x80800;
    if (self->pendingAnimation & 0x4000)
        hit &= ~0x200040u;


    if (hit & 0x40) {
        if ((hit & 0x4800) == 0x4000)
            self->facingAngle = self->facingYaw;
        else
            self->facingAngle = static_cast<std::uint16_t>(self->facingAngle + self->facingAngleDelta);
    }
    if (hit & 0x800000) {
        float height = flddGetIntersectionFor(self->character, self->contactPosition);
        if (g_currentMap.name == SHEN_NAME(Q100)) {
            if (VFMA_t *game = vfmaGetTaskParam())
                height = game->floorIntersectionY;
        }
        self->position.y = height;
    }
    if (hit & 0x200000)
        self->moveFlags ^= 0x40;
    if (self->pendingAnimation & 0x8000)
        self->moveFlags ^= 0x40;
    const auto pending = static_cast<std::uint16_t>(self->pendingAnimation);
    self->currentAnimation = pending;
    self->animationTime = 0;
    mtwkSetExtraFlag(self->motion, 0, 0);
    self->moveFlags &= ~1u;
    self->parameter4c8 = self->animationTime;
    vfApplyMotionDescriptor(self);
    VFMA_t *game = vfmaGetTaskParam();
    if (game && (game->mode & 0x80000) && (self->motionFlags & 0xc0000)) {
        if (self->knockbackAcceleration == 0)
            self->knockbackAcceleration = 1;
        vfUpdateMoveVelocityWithKnockback(self, self->knockbackAcceleration * 0.5f);
        self->knockbackAcceleration = 0;
    } else if (self->knockbackAcceleration != 0) {
        vfUpdateMoveVelocityWithKnockback(self, self->knockbackAcceleration);
        self->knockbackAcceleration = 0;
    } else if (self->scale != self->timeScale) {
        vfUpdateMoveVelocityWithKnockback(self, self->timeScale);
    }
    vfRescaleReactionFrameWindows(self);
    hit = self->hitStateFlags;
    if (static_cast<std::int16_t>(pending) < 0 && (self->motionFlags & 0x200000))
        hit |= 0x200000;
    if (hit & 0x100)
        ++self->lastHitMove;
    if (hit & 0x800000)
        self->facingAngle = self->facingYaw;
    if (hit & 0x80)
        mtwkSetExtraFlag(self->motion, 13, 0);
    self->hitStateFlags = hit;
    if (mtwkSetExtraFlag(self->motion, 1, 1))
        mtwkSetExtraFlag(self->motion, 1, 2);
    self->parameter4c4 = self->currentAnimation;
    if (self->moveFlags & 0x10)
        self->currentHitAction &= ~std::uint64_t{0xffff};
    else if (hit & 0x100) {
        const auto move = self->pendingHitMove;
        self->pendingHitMove = 0;
        self->currentHitAction = (self->currentHitAction & ~std::uint64_t{0xffff}) | move;
        if (move)
            self->savedHitAction = self->pendingHitAction;
    }
    self->reservedA44 = (self->motionFlags & 0x100) ? self->actor->actorRuntimeFlags : 0;
    if (!(self->activeMotionFlags & 0x200000))
        self->stateFlags &= ~0x800u;

    auto material = (g_pFightStage->battleFlags & 0x40) ? static_cast<std::uint8_t>(self->game->cameraParameter) :
                                                          static_cast<std::uint8_t>(QueryGroundMaterialToken(self->position, 0) >> 16);
    if (material > 13)
        material = 13;
    if (self->motion)
        self->motion->runtime.environmentFlags |= material & 0xf;
}

// Event type 4 also accepts the authored type-1 entry.
// @verified - p
const std::byte *vfFindQueuedReactionEventEntry(VFActor *self, std::uint8_t eventType) {
    auto remaining = static_cast<std::uint8_t>(self->reactionEventCount);
    if (!remaining)
        return nullptr;
    for (const std::byte *entry = self->reactionEntries;; entry += 8) {
        const auto type = std::to_integer<std::int8_t>(entry[1]);
        if (type == eventType || (eventType == 4 && type == 1))
            return entry;
        if (!--remaining)
            return nullptr;
    }
}

// Dispatch uses the real marker entries retained by MOTI ownership.
// @verified - p
void vfDispatchQueuedReactionEvent(VFActor *self) {
    const auto event = self->queuedReactionEvent;
    if (!event)
        return;
    if (event == 31) {
        self->pendingAnimation = self->queuedReactionAnimation;
        vfApplyHitReactionAndStartMotion(self);
    } else if (event == 30) {
        if (self->hitStateFlags & 0x100) {
            self->pendingAnimation = self->parameter43e;
            self->knockbackAcceleration = self->game->knockbackAmount;
            vfApplyHitReactionAndStartMotion(self);
            self->queuedReactionEvent = 0;
            self->reactionEventCount = 0;
            self->reactionEntries = nullptr;
            self->queuedReactionAnimation = 0;
        }
    } else if (const std::byte *entry = vfFindQueuedReactionEventEntry(self, static_cast<std::uint8_t>(event))) {
        std::int16_t threshold, animation;
        std::memcpy(&threshold, entry + 2, 2);
        std::memcpy(&animation, entry + 4, 2);

        if (!(float(static_cast<std::uint32_t>(threshold - 1)) > self->animationTime)) {
            self->pendingAnimation = animation;
            vfApplyHitReactionAndStartMotion(self);
        }
    } else
        self->queuedReactionEvent = 0;
}

// The three defaults are actor-owned authored animation words.
// @verified - p
void vfSelectDefaultMotion(VFActor *self, std::uint32_t index) {
    self->targetPosition = self->position;
    self->cameraMotionFlags = 0;
    self->reactionDepth = 0;
    self->pendingAnimation = self->defaultAnimations[index];
    vfApplyHitReactionAndStartMotion(self);
}


// @verified - p
std::int8_t vfUpdatePredicate_40_41_44_50_51_54() {
    return -1;
}


// @verified - p
void vfInitializeActorMotionState(VFActor *self) {
    self->moveFlags = self->stateFlags = 0;
    MotionWork *const motion = FindMotionWork(self->character);
    SetMotionSpeed(motion, 1, MotionRegion::LowerBody);
    const float requested = GetMotionSpeed(motion, MotionRegion::LowerBody) * self->game->knockbackAmount;
    const float scale = 0.2f > requested ? 0.2f : 1.5f < requested ? 1.5f : requested;
    SetMotionSpeed(motion, scale, MotionRegion::LowerBody);
    self->knockbackAcceleration = 0;
    self->scale = self->timeScale = scale;
    self->inverseScale = 1 / scale;
    const std::byte *style = nullptr;
    for (std::uint8_t index = 0; index < 8; ++index) {
        if (g_pFightStage->styles[index].style == self->fightStyle) {
            style = static_cast<const std::byte *>(g_pFightStage->styles[index].file);
            break;
        }
    }
    if (!style)
        style = static_cast<const std::byte *>(g_pFightStage->styles[0].file);
    mtwkSetExtraFlag(motion, 6, 0);
    vfRegisterGroundTargetCallbacks(motion, self);
    vfmaSetAngleFor(motion, 0x800);
    self->motionParameters[0] = style + AuthoredOffset(style + 4);
    self->motionParameters[1] = style + AuthoredOffset(style + 8);
    self->motionParameters[2] = style + AuthoredOffset(style + 12);
    self->motionParameters[3] = style + AuthoredOffset(style + 16);
    const std::byte *const parameter = self->motionParameters[3];
    self->motionParameters[4] = style + AuthoredOffset(parameter);
    self->motionParameters[5] = style + AuthoredOffset(self->motionParameters[2]);
    self->knockbackBaseParameter = AuthoredFixed(parameter + 24);
    self->frontContactRange = AuthoredFixed(parameter + 4);
    self->reserved484 = AuthoredFixed(parameter + 8);
    self->rearContactRange = AuthoredFixed(parameter + 12);
    self->sideContactRange = AuthoredFixed(parameter + 16);
    self->contactRangeParameter = AuthoredFixed(parameter + 20);
    self->contactParameter = AuthoredFixed(parameter + 28);
    const auto facing = static_cast<std::uint16_t>(GetAllAngYForNode(&self->character->entity.node));
    self->contactPosition.y += 1;
    self->contactVectors[0] = self->contactPosition;
    self->facingAngle = self->temporaryFacingAngle = self->facingYaw = facing;
    self->actorRuntimeFlags = 0;
    self->knockbackVelocity = self->moveDelta = self->relativeMoveDelta = self->slideVelocity = {};
    self->parameter370 = self->action = self->deferredReaction = self->queuedAction = 0;
    self->hitStateFlags = self->savedHitStateFlags = 0;
    self->motionFlags = self->secondaryMotionFlags = self->activeMotionFlags = self->savedMotionFlags = 0;
    self->savedSecondaryMotionFlags = self->reserved3b4 = 0;
    self->contactTimer = 0;
    self->pairedContactPriority = self->guardContactFlags = 0;
    self->hitResolutionFlags = 0;
    self->currentHitAction &= ~std::uint64_t{0xffff};
    self->pendingHitMove = 0;
    self->footContactState = self->previousFootContactState = self->parameter45a = 0;
    self->currentAnimation = 0;
    self->animationTime = 0;
    self->queuedReactionEvent = self->hitParameter = 0;
    self->targetGroundY = self->position.y;
    self->hitFacingTimer = 0;
    self->cameraControlFlags = 0;
    self->pairContactPriority = 0;
    self->pairContactMiddleFrame = 0;
    self->parameter33c[0] = self->parameter33c[1] = 0;
    self->hitTimeout = 0;
    self->parameter40c = 0;
    self->minimumGroundY = self->groundY = 0;
    self->groundParameter = self->reactionFlags = self->pairContactLastFrame = 0;
    self->packedHitImpulse = (self->packedHitImpulse & 0xff000000) | 0xff6060;
    self->reactionPower = std::bit_cast<float>((std::bit_cast<std::uint32_t>(self->reactionPower) & 0x00ffffff) | 0x60000000);
    self->formationRuntimeFlags = 0;
    self->parameterAd7 = -1;
    std::memset(&self->parameterA74, 0, 2);
    self->parameterA88 = self->parameterA8c = 0;
    std::memset(self->reservedA90, 0, 4);
    self->previousCooldownQuadrant = self->cooldownQuadrant = vfUpdatePredicate_40_41_44_50_51_54();
    self->targetCache = -1;
    self->moveAngleTableIndex = 0;
    self->parameterA64 = 0;
    self->selectFlags = self->parameterA67 = 0;
    self->parameterB20 = 0;
    self->parameterB24 = 0;
    self->otherActorStorage[1] = std::byte{};
    self->parameterB2c = 1;
    self->parameterAe0 = 0;
    self->parameterAd8 = nullptr;
    self->nearestSpawnPoint = 0;
    if (static_cast<std::uint8_t>(self->game->spawnPointCount) > 1) {
        float minimum = 9999;
        for (std::uint8_t index = 0; index < static_cast<std::uint8_t>(self->game->spawnPointCount); ++index) {
            const float x = self->game->spawnPointParameters[3 * index] - self->contactPosition.x;
            const float z = self->game->spawnPointParameters[3 * index + 1] - self->contactPosition.z;
            const float distance = std::sqrt(z * z + x * x);
            if (minimum > distance) {
                minimum = distance;
                self->nearestSpawnPoint = index;
            }
        }
    }
    for (std::uint8_t index = 0; index < 7; ++index)
        self->previousContactHeights[index] = self->previousGroundSamples[index] = -10000;
    vfRefreshContactReactionState(self);
    if (SetMotionRegionFlag(self->motion, 70, 1, MotionRegion::LowerBody))
        self->moveFlags |= 0x40;
    FRMG_t *const group = self->game->groups[self->groupIndex];
    if (self->game->mode & 0x800) {
        if (group->activationFlags) {
            self->pendingAnimation = group->activationFlags;
            vfApplyHitReactionAndStartMotion(self);
            self->game->mode |= 1;
        }
    } else
        group->activationFlags = 0;
    self->moveFlags |= 0x40000000;
}

// Frame 1 is preserved before the rounded scale conversion.
// @verified - p
std::uint32_t vfScaleFrameCount(float scale, std::uint16_t frames) {
    if (scale == 1 || frames == 1)
        return frames;
    return static_cast<std::uint32_t>(static_cast<int>(float(frames) * scale + 0.5f));
}

// Choose the authored transition duration before publishing MTWK mode 2.
// @verified - p
void vfUpdateMotionTransitionFrames(VFActor *self) {
    std::uint16_t frames;
    VFMA_t *const game = vfmaGetTaskParam();
    if (game && (game->mode & 0x80000) && game->parameter570 < 5)
        frames = 1;
    else if (self->fightStyle == 14 && (self->hitStateFlags & 0x20000))
        frames = 5;
    else if ((self->hitStateFlags & 0x200) && (self->savedSecondaryMotionFlags & 0x100) && (self->moveFlags & 0x80000))
        frames = 6;
    else if (self->hitStateFlags & 0xd0000)
        frames = 5;
    else if (self->hitStateFlags & 0x800000)
        frames = 8;
    else if ((self->hitStateFlags & 0x26000) || (self->activeMotionFlags & 0x200000))
        frames = self->fightStyle == 8 ? 3 : 1;
    else if ((self->hitStateFlags & 0x10001000) || (self->activeMotionFlags & 0x2000000))
        frames = 2;
    else if (self->hitStateFlags & 0x100)
        frames = static_cast<std::uint16_t>(self->parameter2d0 - static_cast<int>(self->animationTime));
    else
        frames = ((self->hitStateFlags & 0x20) && (self->moveFlags & 1)) ? 1 : 8;
    frames = static_cast<std::uint16_t>(vfScaleFrameCount(self->inverseScale, frames));
    if (!frames)
        frames = 1;
    else if (frames > 8)
        frames = 8;
    if ((self->game->gameFlags & 0x1000000) || (self->secondaryMotionFlags & 0x8000))
        frames = 1;
    self->motionTimer = static_cast<std::int16_t>(frames);
    SetMotionFrameTime(self->motion, frames, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(self->motion, 2);
}

// Clear the shared scripted-wave lock only after every actor releases it.
// @verified - p
void vfResetScriptedFormationMotion(VFActor *self) {
    VFMA_t *const game = self->game;
    if (!(game->mode & 0x800))
        return;
    self->formationFlags &= ~0x8000;
    std::uint32_t flags = 0;
    for (std::uint8_t index = 0; index < 6; ++index)
        if (game->characters[index])
            flags |= game->actors[index]->formationFlags;
    if (!(flags & 0x8000))
        game->mode &= 0xfffff7fe;
}

// Authored contact flags decide whether the snap includes XZ.
// @verified - p
void vfSnapActorToContact(VFActor *self) {
    const auto move = self->moveFlags;
    if (!(move & 4)) {
        self->position.y = self->contactPosition.y;
        if (!(self->motionContactFlags & 1)) {
            self->position.x = self->contactPosition.x;
            self->position.z = self->contactPosition.z;
        }
    }
    self->hitStateFlags |= 0x10;
    self->moveFlags = move | 0xc;
    mtwkSetExtraFlag(self->motion, 3, 0);
}


// @verified - p
void vfPredictLandingGround(VFActor *self) {
    self->groundParameter = std::bit_cast<std::uint32_t>(self->targetGroundY);
    if (self->knockbackVelocity.x == 0 && self->knockbackVelocity.z == 0)
        return;
    const float count = static_cast<std::int16_t>(self->predictedKnockbackFrames);
    Vector3 point{
        count * self->knockbackVelocity.x + self->contactPosition.x, self->contactPosition.y, count * self->knockbackVelocity.z + self->contactPosition.z};
    const float intersection = vfFieldGetIntersection(nullptr, point);
    if (intersection == self->targetGroundY)
        return;
    float vertical = self->knockbackVelocity.y;
    std::int16_t frames;
    float x, z;
    if (!(self->targetGroundY > intersection)) {
        x = self->contactPosition.x;
        z = self->contactPosition.z;
        frames = 0;
    } else {
        frames = static_cast<std::int16_t>(self->predictedKnockbackFrames);
        vertical = -vertical;
        x = point.x;
        z = point.z;
    }
    point.y = self->contactPosition.y;
    float ground;
    for (;;) {
        do {
            x += self->knockbackVelocity.x;
            z += self->knockbackVelocity.z;
            point.x = x;
            point.z = z;
            vertical = (vertical - self->scale * self->scale * self->game->gravityScale) + self->slideVelocity.y;
        } while (!(0 > vertical));
        ground = vfFieldGetIntersection(nullptr, point);
        point.y += vertical;
        if (ground + 0.02f > point.y - self->groundOffset)
            break;
        x = point.x;
        z = point.z;
        frames = static_cast<std::int16_t>(static_cast<std::uint16_t>(frames) + 1);
    }
    self->predictedKnockbackFrames = static_cast<std::uint16_t>(frames);
    self->groundParameter = std::bit_cast<std::uint32_t>(ground);
}

// Retain low action bits while rotating the authored sector into view space.
// @verified - p
std::uint16_t vfRotateCameraSectorCode(VFActor *self, std::uint16_t code) {
    const auto sector = code & 0xf0;
    if (!sector)
        return code;
    const auto angle = static_cast<std::int16_t>(self->facingYaw - camCtrlGetNextRotY() + 4096);
    const auto rotation = (angle >> 13) & 7;
    std::int32_t index = 0;
    while (cameraSectors[index] != sector) {
        if (++index >= 8) {
            index = -1;
            break;
        }
    }
    return (code & 0xff0f) | cameraSectors[(rotation + index) & 7];
}

// Select from borrowed 17-byte authored phase-chain records.
// @verified - p
void vfResetAndSelectCameraPhaseChoice(VFActor *self, std::uint8_t choice, std::uint16_t delay) {
    auto &slot = *self->slot;
    if (!(slot.cameraFlags & 1))
        return;
    slot.parameter89 = choice;
    slot.cameraFlags &= ~1U;
    slot.parameter8c = 0;
    slot.parameterCc = static_cast<float>(delay);
    slot.randomBytes[0] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    slot.parameter12a = 0;
    slot.resetIndex = 0x7f7f;
    const auto *record = reinterpret_cast<const std::uint8_t *>(self->motionParameters[5]);
    while (*record != 127) {
        if (*record == choice) {
            std::int8_t count = 4;
            if (record[13] == 127)
                --count;
            if (record[9] == 127)
                --count;
            if (record[5] == 127)
                --count;
            if (record[1] == 127)
                --count;
            if (count > 0) {
                const auto pick = static_cast<std::uint8_t>(IncrementSharedPrngUInt16()) % static_cast<std::uint8_t>(count);
                slot.resetIndex = record[4 * pick + 1] | (static_cast<std::uint16_t>(record[4 * pick + 3]) << 8);
                slot.reserved62[1] = static_cast<std::byte>(record[4 * pick + 2]);
                slot.reserved62[2] = static_cast<std::byte>(record[4 * pick + 4]);
            }
            return;
        }
        record += 17;
    }
}


// @verified - p
bool vfUpdateCameraScript(VFActor *self) {
    auto &slot = *g_pFightMain->actorSlots[self->groupIndex];
    auto step = static_cast<std::uint8_t>(slot.parameter89 >> 8);
    if (slot.parameter8c) {
        if (slot.parameter8c != 1) {
            self->cameraControlFlags = slot.parameter8e;
            --slot.parameter8c;
            return false;
        }
        slot.parameter89 = (slot.parameter89 & 0xff) | (static_cast<std::uint16_t>(static_cast<std::uint8_t>(step + 1)) << 8);
        if (step == 10) {
            slot.parameter8c = 0;
            return true;
        }
        ++step;
    }
    const auto choice = static_cast<std::uint8_t>(slot.parameter89);
    const auto *record = choice >= 200 ? defaultCameraScripts[choice - 200] : reinterpret_cast<const std::uint8_t *>(self->motionParameters[1]) + 20 * choice;
    std::uint16_t code = record[step];
    if (code == 127)
        return true;
    if (self->groupIndex == g_pFightMain->playerSlot && !(g_pFightMain->gameFlags & 0x400000) && (code & 0xf0) != 0xf0)
        code = vfRotateCameraSectorCode(self, code);
    self->cameraControlFlags = code;
    slot.parameter8e = code;
    slot.parameter8c = record[step + 10];
    return false;
}


// @verified - p
void vfAdvanceCameraPhaseChain(VFActor *self) {
    auto &slot = *self->slot;
    if (slot.parameterCc > 0) {
        slot.parameterCc -= shenGetScaledDeltaTickTime(1);
        if (slot.parameterCc < 0)
            slot.parameterCc = 0;
        return;
    }
    if (!vfUpdateCameraScript(self))
        return;
    slot.cameraFlags &= ~0x2000U;
    const auto index = slot.parameter12a;
    const auto *raw = reinterpret_cast<const std::uint8_t *>(&slot);
    const auto choice = raw[0x60 + index];
    if (choice == 127) {
        ResetCameraPhase(slot);
        return;
    }
    const auto delay = raw[0x63 + index];
    slot.parameter12a = static_cast<std::uint8_t>(index + 1);
    if (slot.randomBytes[1] & 7) {
        slot.parameter89 = choice;
        slot.parameter8c = 0;
        slot.parameterCc = static_cast<float>(delay);
        slot.randomBytes[0] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    } else {
        ResetCameraPhase(slot);
        vfResetAndSelectCameraPhaseChoice(self, choice, delay);
    }
}


// @verified - p
void vfUpdateHPRegeneration(VFActor *self) {
    const float maximum = static_cast<float>(self->initialHp);
    if (self->hp >= maximum) {
        self->ticks = 0;
        return;
    }
    if ((self->moveFlags & 0x20) || !(self->hp > 0) || (self->hitStateFlags & 0xa4000) || (self->game->mode & 1) || self->game->type == 10)
        return;
    float rate = self->regenerationRate;
    if (self->hitStateFlags & 0x20000000)
        rate *= self->focusedRegenerationScale;

    const float gain = (self->guardHp / self->guardMaximumHp) * rate;
    self->hp += 0.033333335f > gain ? 0.033333335f : gain;
    if (self->hp > maximum)
        self->hp = maximum;
}

// Saved proficiency of a packed move id scales by 0.002 per point; ids outside the three move categories scale by one.
// @verified - p
float vfGetMoveProficiencyScale(std::uint8_t move) {
    constexpr std::uint8_t starts[5]{0, 0, 32, 64, 96};
    constexpr std::uint8_t counts[4]{0, 28, 23, 22};
    std::uint8_t category = 1;
    while (move >= starts[category + 1])
        if (++category >= 4)
            return 1;
    auto index = static_cast<std::int8_t>(move - starts[category]);
    if (index >= static_cast<std::int8_t>(counts[category]))
        index = -1;
    const auto proficiency = static_cast<std::uint8_t>(GetMoveProficiency(category, static_cast<std::uint8_t>(index)));
    return static_cast<float>(proficiency) * 0.0020000001f + 1;
}


// @verified - p
float vfclCalculateGuardKnockbackFactor(VFActor *self) {
    if (self->groupIndex != static_cast<std::uint8_t>(self->game->playerSlot))
        return 1;
    auto move = static_cast<std::uint8_t>(std::bit_cast<std::uint32_t>(self->reactionPower) >> 24);
    if (self->motionFlags & 0x40000)
        move = static_cast<std::uint8_t>(self->packedHitImpulse >> 8);
    else if (move == 96)
        move = static_cast<std::uint8_t>(self->packedHitImpulse);
    return vfGetMoveProficiencyScale(move);
}

// Fight style is not the voice-row index.
// @verified - p
std::uint8_t vfFightVoiceStyle(std::uint16_t style) {
    switch (style) {
    case 0:
    case 16:
    case 46:
        return 0;
    case 2:
    case 6:
    case 35:
        return 11;
    case 3:
    case 36:
        return 12;
    case 4:
    case 38:
        return 10;
    case 8:
        return 4;
    case 10:
        return 3;
    case 11:
        return 1;
    case 12:
        return 2;
    case 19:
        return 16;
    case 22:
        return 13;
    case 23:
        return 7;
    case 24:
        return 6;
    case 25:
        return 5;
    case 26:
    case 45:
        return 8;
    case 48:
        return 15;
    default:
        return 9;
    }
}


// @verified - p
void vfPlayFightSfx(std::uint32_t sound, VFActor *self, std::uint8_t mode) {
    const auto cameraMode = camCtrlGetInterpMode();
    if (!self->character || (self->game->mode & 8) || ((self->game->mode & 1) && cameraMode != CameraMotionMode::AutoQuadratic))
        return;

    std::uint32_t material = (g_pFightStage->battleFlags & 0x40) ?
        static_cast<std::uint8_t>(self->game->cameraParameter) :
        static_cast<std::uint32_t>(static_cast<std::int32_t>(QueryGroundMaterialToken(self->position, 0)) >> 16);
    if (material > 13)
        material = 13;
    if (cameraMode == CameraMotionMode::AutoQuadratic && sound - 37U <= 2U)
        sound = 1704;
    PlayFootstepForNode(self->character, sound | ((material | 0xfe0U) << 19), mode != 0);
}


// @verified - p
void vfForceActorDefeat(VFActor *self) {
    if (!(self->hitStateFlags & 0x814000)) {
        mtwkSetExtraFlag(self->motion, 0, 0);
        motRemoveCallback(self->motion);
        vfUpdateDefaultReactionAnim(self->game->enemySlots[self->groupIndex], 198);
        self->pendingAnimation = 198;
        vfApplyMotionDescriptor(self);
    }
    figpAssignModeForCharacter(self->character, 0);
    self->game->mode |= 0x20;
    self->parameter370 = 0x24000000;
    self->hitPhase = 2;
}

// The authored counterpart at B00 owns the defeat camera pairing.
// @verified - p
void vfStartAuthoredDefeatCamera(VFActor *self) {
    auto &game = *self->game;
    auto *const linked = self->pairedActor;
    std::uint16_t animation;
    std::memcpy(&animation, reinterpret_cast<const std::byte *>(&game) + 0x56e, sizeof(animation));
    game.mode |= 0x80000;
    game.parameter570 = 0;
    self->parameter370 = animation | 0x08000000ULL;
    self->deferredReaction = 0;
    linked->hitPhase = 99;
    linked->parameter370 = linked->deferredReaction = 0;
    if (camCtrlCheckIsEventCam() && !(g_pFightStage->battleFlags & 2)) {
        resetCharaPosYandIntersection();
        camCtrlSetFovTransitionTo(false);
        camCtrlSetState(CameraState::ChaseGoal);
    }
    game.mode &= ~1U;
    camCtrlSetInterpModeTo(CameraMotionMode::AutoQuadratic2);
    vfSetupLinkedFightCamera(linked, 2);
    self->moveFlags |= 0x10000000;
    linked->moveFlags |= 0x10000000;
    game.mode |= 0x4000;
    linked->pairedContactPriority |= 1;
}


// @verified - p
void vfHandleImpactSfxAndDefeat(VFActor *self, std::int16_t damage, std::uint8_t mode) {
    auto &game = *self->game;
    const bool player = game.playerSlot != -1 && self->groupIndex == game.playerSlot;
    if (player ? (game.mode & 1) != 0 : (game.mode & 5) == 5)
        return;
    if (g_forcePlayerGodmode) {
        if (self->groupIndex == static_cast<std::uint8_t>(game.playerSlot))
            return;
        damage = 10000;
    } else if (!damage)
        return;
    const auto queueDefeat = [&] {
        if (const auto *record = vfFindQueuedReactionEventEntry(self, 3))
            self->queuedReactionEvent = static_cast<std::uint16_t>(static_cast<std::int8_t>(std::to_integer<std::uint8_t>(record[1])));
    };
    if (self->hp < 0 || std::fabs(self->hp) < 0.00000011920929f) {
        queueDefeat();
        return;
    }
    VFActor *source{};
    if (!mode)
        source = self->secondActor ? self->secondActor : self->pairedActor;
    else if (mode == 1) {
        std::memcpy(&source, self->reservedB08, sizeof(source));
        if (!source)
            source = self->actor;
    }
    if (game.playerCharacter && self->character == game.playerCharacter) {
        damage = static_cast<std::int16_t>(static_cast<std::int32_t>((1 - game.playerAssistScale) * damage));
        if (damage < 1)
            damage = 1;
    }
    const auto factor = static_cast<std::int16_t>(static_cast<std::int32_t>(self->parameterB2c));
    self->parameterB2c = 1;
    damage = static_cast<std::int16_t>(factor * damage);
    if (source && damage >= self->hp && (source->secondaryMotionFlags & 0x40000))
        damage = static_cast<std::int16_t>(static_cast<std::int32_t>(self->hp) - 1);
    const auto flags = self->formationFlags;
    if ((flags & 0x4010000) ||
        ((flags & 0x80000) && damage >= self->hp &&
            static_cast<std::int8_t>(self->packedHitImpulse >> 16) !=
                static_cast<std::int16_t>(std::bit_cast<std::uint32_t>(self->formationCameraTimer) & 0xffff)))
        return;
    const auto floor = (flags & 0x20000) ? self->formationHpFloor : std::int16_t{};
    if (self->hp - damage <= 0)
        queueDefeat();
    self->hp -= damage;
    if (mode == 1) {
        if (self->stateFlags & 0x40000)
            self->hp = 0;
        else if (self->hp <= 0)
            self->hp = 1;
    }
    if (self->hp < floor)
        self->hp = static_cast<float>(floor);
    self->guardHp -= source ? vfclCalculateGuardKnockbackFactor(source) * damage : static_cast<float>(damage);
    if (self->guardHp < 0)
        self->guardHp = 0;
    if (std::fabs(self->hp) < 0.00000011920929f) {
        auto *bytes = reinterpret_cast<std::uint8_t *>(&game);
        std::uint32_t soundSlot;
        std::memcpy(&soundSlot, bytes + 0x270 + 4 * self->groupIndex, sizeof(soundSlot));
        if (bytes[0x29c] && (soundSlot & 1))
            vfInitSfxSlotState(&game);
        std::memset(bytes + 0x270 + 4 * self->groupIndex, 0, 4);
        const auto voice = fightDefeatVoiceRow[vfFightVoiceStyle(self->fightStyle)];
        if (voice)
            vfPlayFightSfx(voice, self, 1);
        if (self->character == game.playerCharacter || game.state != 3) {
            if (mode == 2)
                vfForceActorDefeat(self);
            vfResetMoveTrainingGains();
            if (!mode) {
                auto *const current = vfmaGetTaskParam();
                if ((!current || !(current->mode & 0x80000)) && !EV_camCtrlToggleCombatCamera(1, -1) && !(g_pFightStage->battleFlags & 0x2000)) {
                    self->hp = 1;
                    vfStartAuthoredDefeatCamera(source);
                }
            }
        } else {
            ++bytes[0x32d];
            if (source) {
                if (source->groupIndex == static_cast<std::uint8_t>(game.playerSlot)) {
                    ++bytes[0x32c];
                    game.parameter530 = 15;
                } else if (game.slotTeams[source->groupIndex] == game.slotTeams[game.playerSlot])
                    ++game.parameter2b9;
            }
        }
        if (source)
            ++reinterpret_cast<std::uint8_t *>(source)[0xaf5];
    }
    if (std::fabs(self->hp) >= 0.00000011920929f && (flags & 1) && static_cast<std::int16_t>(self->formationEntry) >= self->hp)
        self->formationRuntimeFlags |= 1;
}


// @verified - p
bool vfCheckAnimTimeWithin(VFActor *self, float frame) {
    return self->animationTime >= frame && frame > self->animationTime - self->scale;
}

// Every non-null slot of the separate 57-entry frame-event table.
// @verified - p
std::uint8_t vfProcessMotionFrameEvents(VFActor *self) {
    for (;;) {
        motionMarkerCursor = self->reactionProgramCounter;
        auto tag = MarkerValue<std::uint8_t>(2);
        motionFrameEvent = tag ? static_cast<std::uint16_t>(MarkerValue<std::uint16_t>(0) - 1) : 0;
        if (tag && motionFrameEvent > self->animationTime) {
            tag = 0;
            motionFrameEvent = 0;
        }
        bool stop = false;
        const auto argument = MarkerValue<std::uint8_t>(3);
        switch (tag) {
        case 0:
            self->reactionProgram = self->reactionProgramCounter;
            if (self->moveFlags & 0x1000) {
                self->hitStateFlags |= 1;
                self->moveFlags &= ~0x1000U;
            }
            stop = true;
            break;
        case 1:
            self->hitStateFlags ^= 0x200000;
            break;
        case 2:
            mtwkSetExtraFlag(self->motion, 0, 0);
            if ((self->parameter370 & 0xff000000) != 0x08000000)
                self->parameter370 = 0x06000000;
            self->parameter40c &= 0xffff0000U;
            break;
        case 3: {
            const auto angle = static_cast<std::int16_t>((self->moveFlags & 0x40) ? -MarkerValue<std::int16_t>(4) : MarkerValue<std::int16_t>(4));
            const auto yaw = static_cast<std::int16_t>(angle - self->facingAngle);
            const auto *const source = self->pairedActor;
            const float horizontal = MarkerFixed(8) * source->scale;
            self->knockbackVelocity = {sin_fixed16(yaw) * horizontal,
                ((source->scale * source->scale) * self->game->gravityScale) * (MarkerFixed(12) * source->scale),
                -cos_fixed16(static_cast<std::uint16_t>(yaw)) * horizontal};
            vfSnapActorToContact(self);
            break;
        }
        case 4:
            if (camCtrlGetInterpMode() == CameraMotionMode::AutoQuadratic || camCtrlGetInterpMode() == CameraMotionMode::AutoQuadratic2)
                vfPlayFightSfx(MarkerValue<std::uint32_t>(4), self, 0);
            break;
        case 5:
            self->hitStateFlags |= MarkerValue<std::uint32_t>(4);
            break;
        case 6:
            self->hitStateFlags &= MarkerValue<std::uint32_t>(4);
            break;
        case 7:
            self->hitStateFlags ^= MarkerValue<std::uint32_t>(4);
            break;
        case 8:
            if (!(0 >= self->game->guardTimer)) {
                const float factor = vfclCalculateGuardKnockbackFactor(self->secondActor) * argument;
                const auto damage = static_cast<std::uint8_t>(static_cast<std::int64_t>(factor + 0.5f));
                const auto increment = static_cast<std::uint16_t>(static_cast<std::int32_t>((factor * 0.5f + 0.5f) * self->inverseScale));
                self->parameter40c =
                    (self->parameter40c & 0xffffU) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>((self->parameter40c >> 16) + increment)) << 16);
                if (reinterpret_cast<const std::uint8_t *>(self->slot)[0x1a5] == 3 &&
                    ((self->secondaryMotionFlags & 0x10) || static_cast<std::uint16_t>((self->currentAnimation & 0xfff) - 67) <= 1))
                    self->parameterB2c = guardImpactMultipliers[vfMotnNibbleGet(67)];
                SetActorValue(self, 0x414, static_cast<std::uint16_t>(ActorValue<std::uint16_t>(self, 0x414) + damage));
                vfHandleImpactSfxAndDefeat(self, damage, 0);
            }
            break;
        case 9: {
            std::uint16_t dialogue;
            std::memcpy(&dialogue, self->game->reserved318 + 2, sizeof(dialogue));
            if (!cvsHasValidConverseData() && dialogue == 0xffff && !(self->game->mode & 1) &&
                ((self->hitStateFlags & 0xc0000) || static_cast<std::uint32_t>(static_cast<std::int8_t>(argument) - 14) > 1 ||
                    IncrementSharedPrngFloat() <= 0.33000001f)) {
                const auto voice = fightVoices[static_cast<std::int8_t>(argument)][vfFightVoiceStyle(self->fightStyle)];
                if (voice)
                    vfPlayFightSfx(voice, self, 1);
            }
            break;
        }
        case 10:
            check_dead_status(1);
            break;
        case 11:
            if (!MarkerValue<std::uint16_t>(4) || MarkerValue<std::uint16_t>(4) < IncrementSharedPrngUInt16()) {
                self->queuedReactionEvent = 31;
                self->queuedReactionAnimation = MarkerValue<std::uint16_t>(6);
                stop = true;
            }
            break;
        case 13:
            if (argument)
                self->hitStateFlags &= ~0x8000000U;
            else
                self->hitStateFlags |= 0x8000000;
            break;
        case 14:
            if (vfCheckAnimTimeWithin(self, static_cast<float>(motionFrameEvent))) {
                float amount = MarkerFixed(4);
                if (self->groupIndex == self->game->playerSlot)
                    amount *= self->game->playerCameraScale + 1.5f;
                if (!(self->game->mode & 8))
                    camCtrlSetRotationXYZ(0, amount, 0.5f);
            }
            break;
        case 15: {
            auto *const current = vfmaGetTaskParam();
            if (current && (current->mode & 0x80000))
                break;
            const auto index = MarkerValue<std::int16_t>(4);
            if (!pakFindEventCam(index) || (g_pFightStage->battleFlags & 2))
                break;
            auto &game = *self->game;
            if (argument == 1) {
                for (std::uint8_t i = 0; i < 6; ++i)
                    if (game.characters[i]) {
                        vfResetReactionProgram(game.actors[i]);
                        self->moveFlags |= 0x10000;
                    }
                game.mode |= 1;
                if (!(std::to_integer<std::uint8_t>(self->motionParameters[2][25]) & 1))
                    self->moveFlags |= 0x40000;
            } else if (argument == 2)
                game.mode |= 0x4000;
            for (auto *character : game.characters)
                if (character)
                    cpamSetFlag(character, 0);
            if (self->moveFlags & 0x40)
                camCtrlSetSplineShouldRebuildRotTo(true);
            game.targetTime = static_cast<float>(MarkerValue<std::uint16_t>(6));
            const std::uintptr_t request[3]{static_cast<std::uint32_t>(static_cast<std::int32_t>(index)), reinterpret_cast<std::uintptr_t>(self->character),
                reinterpret_cast<std::uintptr_t>(self->character)};
            camCtrlToggleSplineRebuildFlag(3, request);
            camCtrlSetState(CameraState::Scripted);
            camCtrlDisableFilmBars();
            break;
        }
        case 16: {
            constexpr std::int32_t expressions[11]{1, 3, 0, 0, 0, 0, 0, 3, 11, 6, 4};
            SetHighModelAndFaceFlagsForChara(self->character, expressions[argument], 1, 1);
            break;
        }
        case 19:
            self->parameter40c =
                (self->parameter40c & 0xffff0000U) | static_cast<std::uint16_t>(static_cast<std::int32_t>(MarkerValue<std::uint16_t>(4) * self->inverseScale));
            break;
        case 20:
            if (vfCheckAnimTimeWithin(self, static_cast<float>(motionFrameEvent)))
                SetActorValue(self, 0x638, std::uint64_t{});
            break;
        case 22:
            self->hitStateFlags |= 0x40;
            self->facingAngleDelta = MarkerValue<std::int16_t>(4);
            break;
        case 23: {
            const bool value = (self->groundContactFlags & 1) ? argument != 0 : argument == 0;
            if (!value) {
                self->queuedReactionEvent = 31;
                self->queuedReactionAnimation = MarkerValue<std::uint16_t>(4);
                stop = true;
            }
            break;
        }
        case 25: {
            auto flags = MarkerValue<std::uint32_t>(4);
            const auto &other = *self->pairedActor;
            if (other.moveFlags & 0x40) {
                if (flags & 0x4000000)
                    flags = (flags & ~0x4000000U) | 0x2000000;
                else if (flags & 0x2000000)
                    flags = (flags & ~0x2000000U) | 0x4000000;
            }
            if ((flags & self->reactionFlags) && MarkerFixed(12) >= self->contactDistances[self->actionIndex] && !self->game->hitStopFrames &&
                other.knockbackBaseParameter <= 150)
                self->action = static_cast<std::uint64_t>(static_cast<std::int64_t>(MarkerValue<std::int32_t>(8)));
            break;
        }
        case 26: {
            auto cursor = ActorValue<std::uint8_t>(self, 0x618);
            const auto end = ActorValue<std::uint8_t>(self, 0x61a);
            const auto filter = MarkerValue<std::uint16_t>(8);
            bool found = false;
            std::uint16_t mask{};
            while (cursor != end) {
                cursor = static_cast<std::uint8_t>((cursor - 1) & 31);
                const auto flags = ActorValue<std::uint16_t>(self, 0x51c + 8 * cursor) & 0x30f;
                mask = ActorValue<std::uint16_t>(self, 0x518 + 8 * cursor) & 0xf0;
                if (!filter || (filter & flags) == filter) {
                    found = true;
                    break;
                }
                if (flags & 0x100)
                    break;
            }
            auto requested = MarkerValue<std::uint16_t>(4);
            if (found && (!requested || ((requested & 0x400) ? (mask & 0x20) : (mask & ((requested & 0x800) ? 16 : requested))))) {
                self->queuedReactionEvent = 31;
                self->queuedReactionAnimation = MarkerValue<std::uint16_t>(10);
                cursor = static_cast<std::uint8_t>((cursor + 1) & 31);
                SetActorValue(self, 0x619, cursor);
                SetActorValue(self, 0x61a, cursor);
                SetActorValue(self, 0x61b, cursor);
                SetActorValue(self, 0x61c, cursor);
            }
            break;
        }
        case 29:
            self->activeMotionFlags |= MarkerValue<std::uint32_t>(4);
            break;
        case 30:
            self->activeMotionFlags &= MarkerValue<std::uint32_t>(4);
            break;
        case 32:
            vfPlayFightSfx(g_currentMap.name == SHEN_NAME(QKBT) ? 2803 : MarkerValue<std::uint32_t>(4), self, 0);
            break;
        case 45:
            EmitFightSmoke(argument, self);
            break;
        case 46:
            vfStageObjectReset(self->parameterAd7, self->character);
            break;
        case 47:
            vfStageObjectReset(self->parameterAd7, nullptr);
            break;
        case 48:
            vfLaunchStageObject(self->parameterAd7, self->temporaryFacingAngle);
            break;
        case 50: {
            auto voice = argument;
            const auto count = MarkerValue<std::uint8_t>(4);
            if (count > 1)
                voice = static_cast<std::uint8_t>(voice + static_cast<std::int32_t>(IncrementSharedPrngFloat() * count));
            if (self->game->reserved318[0] != std::byte{0xff} && !(g_pFightStage->battleFlags & 2))
                EV_PlayDialogue(vfGetFightDialogueVoice(self->game, voice));
            break;
        }
        case 51: {
            const auto effect = MarkerValue<std::int8_t>(4);
            auto frames = MarkerValue<std::int16_t>(6);
            const float amount = MarkerFixed(8);
            if (!effect)
                vfToggleAttachedStageObjectDisplay(self);
            else if (static_cast<std::uint8_t>(effect) <= 3) {
                if (!frames)
                    frames = 1;
                vfSetAttachedStageObjectEffect(self, effect, amount, frames);
            } else if (effect == 4)
                vfSetAttachedStageObjectFigurePose(self, static_cast<std::int8_t>(static_cast<std::int64_t>(static_cast<double>(amount) + 0.5)), frames);
            break;
        }
        case 53:
            toggleOsagFlags(self->character);
            break;
        case 54:
            if (!(self->stateFlags & 0x80) || g_currentMap.name != SHEN_NAME(QLBT)) {
                std::uint32_t sound = 38;
                if (camCtrlGetInterpMode() != CameraMotionMode::AutoQuadratic) {
                    const auto material = static_cast<std::uint16_t>(QueryGroundMaterialToken(self->contactPosition, 0));
                    constexpr std::uint16_t sounds[9]{37, 38, 39, 642, 642, 1750, 2803, 2803, 2803};
                    sound = sounds[(material == 10 ? 3 : material == 15 ? 6 : 0) + MarkerValue<std::uint16_t>(4)];
                }
                vfPlayFightSfx(sound, self, 0);
            }
            break;
        case 55:
            if (camCtrlCheckIsEventCam() && !(g_pFightStage->battleFlags & 2)) {
                resetCharaPosYandIntersection();
                camCtrlSetFovTransitionTo(false);
                camCtrlSetState(CameraState::ChaseGoal);
            }
            break;
        case 56:
            self->reactionRank = argument;
            break;
        default:
            break;
        }
        const auto width = frameMarkerWidths[tag];
        self->reactionProgramCounter += width;
        if (stop)
            return width;
    }
}
// Reaction windows track the live participants, not a preview actor set.
// @verified - p
void vfUpdateReactAnimationFrames(VFActor *self) {
    if (!self->eventFrame30a)
        return;
    if (self->stateFlags & 0x800) {
        self->reactionMiddleFrame = 5;
        self->reactionEndFrame = 6;
        return;
    }
    if (!(self->motionFlags & 0x40100) && !(self->secondaryMotionFlags & 0x20000000))
        return;
    if (!((self->hitStateFlags ^ self->previousHitStateFlags) & 4) && !((self->activeMotionFlags ^ self->previousActiveMotionFlags) & 0x20000000))
        return;
    VFMA_t *const game = self->game;
    std::uint8_t participants = 0;
    for (Character *character : game->characters)
        if (character)
            ++participants;
    const auto id = self->currentAnimation & 0xfff;
    const auto secondary = FindLoadedMotionSecondaryEntry(id, 2);
    const auto readWord = [](const std::byte *data) {
        std::uint16_t value;
        std::memcpy(&value, data, sizeof(value));
        return value;
    };
    if (participants == 2 && ((self->actor->hitStateFlags & 0x9c000) || (self->hitStateFlags & 0x40000))) {
        self->reactionEndFrame = static_cast<std::uint16_t>(readWord(secondary.data() + 6) - 1);
        self->currentAnimationEndFrame = static_cast<std::uint16_t>(FindLoadedMotionSequence(id)->frameCount - 1);
        self->animationTimeParameter = float(self->currentAnimationEndFrame) - self->scale;
        return;
    }
    const float authored = game->playerSlot == self->groupIndex ? float(self->eventFrame30a) : 0;
    const auto middle = static_cast<std::uint16_t>(self->reactionMiddleFrame);
    auto end = secondary.empty() ? std::uint16_t{} : static_cast<std::uint16_t>(readWord(secondary.data() + 6) - 1);
    if (authored == 0 || float(middle) > authored || authored >= float(end))
        return;
    float nearest = 100;
    for (std::uint8_t index = 0; index < 6; ++index) {
        VFActor *const other = game->actors[index];
        if (game->characters[index] && other && game->playerSlot != index && other->hp > 0 && !(other->motionFlags & 0x80000))
            nearest = std::fmin(self->contactDistances[index], nearest);
    }
    if (nearest <= 3)
        end = self->eventFrame30a;
    else if (nearest < 8)
        end = static_cast<std::uint16_t>(int(((float(end) - authored) / 5) * (nearest - 3) + 0.5f + authored));
    auto selected = static_cast<std::uint16_t>(middle + 3);
    if (end >= std::uint32_t(middle) + 3)
        selected = end;
    if (float(self->reactionEndFrame) >= self->animationTime && self->animationTime > float(selected))
        selected = static_cast<std::uint16_t>(int(self->animationTime));
    const auto previousEnd = self->currentAnimationEndFrame;
    self->reactionEndFrame = self->currentAnimationEndFrame = selected;
    if (self->animationTime >= float(selected)) {
        const auto command = FindLoadedMotionTimedCommand(id, 1, 0, float(previousEnd), 1);
        if (!command.empty() && int(readWord(command.data())) - 1 > int(selected))
            self->hitStateFlags ^= 0x200000;
        const auto *figure = static_cast<FigurePose *>(FindModelComponent(self->character, MODEL_FIGURE_POSE));
        if (!figure || !(figure->flags & 1))
            figpAssignModeForCharacter(self->character, 0);
    }
    if (self->animationTime > float(self->currentAnimationEndFrame))
        vfSelectDefaultMotion(self, 0);
}

// Animation transition, hit-phase gates and the authored frame callbacks.
// @verified - p
void vfUpdateAnimFlagsAndEvents(VFActor *self) {
    MotionWork *const motion = self->motion;
    if (!self->currentAnimation)
        vfSelectDefaultMotion(self, 0);
    else if (self->moveFlags & 1) {
        self->pendingAnimation = static_cast<std::uint16_t>(self->requestedAnimation);
        if (self->requestedAnimation == -1) {
            self->reactionProgram = self->mirroredReactionProgram;
            self->savedHitStateFlags = self->hitStateFlags;
            mtwkSetExtraFlag(motion, 14, 0);
            self->moveFlags &= ~0x80000u;
            const float time = self->animationTime - self->scale - float(self->currentAnimationEndFrame);
            self->animationTime = 0 > time ? 0 : time;
            self->parameter4c8 = self->animationTime;
        } else if (self->requestedAnimation)
            vfApplyHitReactionAndStartMotion(self);
        else if (!(self->stateFlags & 0x80) && (self->hitStateFlags & 0xc000) != 0x4000)
            vfSelectDefaultMotion(self, 0);
    }
    if (self->animationTime < 0)
        self->animationTime = 0;
    if (self->parameter4c4 != 0xffff || self->parameter4c8 != -1) {
        if (self->parameter4c4 == 0xffff)
            self->parameter4c4 = self->currentAnimation;
        if (self->parameter4c8 == -1)
            self->parameter4c8 = 0;
        vfUpdateMotionTransitionFrames(self);
        if (self->moveFlags & 0x20000) {
            self->moveFlags &= ~0x20000u;
            self->pendingAnimation = GetMotionAnimationId(motion, MotionRegion::LowerBody);
            vfApplyMotionDescriptor(self);
        } else
            mtwkRequestMotion(motion, self->parameter4c4 & 0xfff, MotionRegion::LowerBody);
    }
    DecrementFrame(self->motionTimer);
    SetMotionRegionFlag(motion, 69, (self->moveFlags & 0x40) ? 0 : 2, MotionRegion::LowerBody);
    if (self->fightStyle == 16 && (self->motionFlags & 0x400)) {
        std::int32_t selector = 0;
        if (self->cameraMotionFlags & 0x28)
            selector = (self->moveFlags & 0x400) ? 25 : 30;
        else if (self->cameraMotionFlags & 0x50)
            selector = (self->moveFlags & 0x400) ? 30 : 25;
        if (std::bit_cast<std::uint32_t>(self->reactionDepth))
            GetParentBoneWrapper(motion, selector);
    }
    vfClsampleCurrentMotion(motion, self->animationTime, self->moveDelta);
    auto hit = self->hitStateFlags;
    switch (self->actionTriggerPhase) {
    case 1:
        if ((self->motionFlags & 0x8100) == 0x8100 && self->animationTime >= float(static_cast<std::uint16_t>(self->parameter2d0)))
            hit &= ~0x10000u;
        if (self->animationTime < float(static_cast<std::uint16_t>(self->parameter2d0)))
            break;
        hit &= ~2u;
        ++self->actionTriggerPhase;
        [[fallthrough]];
    case 2:
        if (self->animationTime < float(static_cast<std::uint16_t>(self->reactionMiddleFrame)))
            break;
        if (hit & 0x100)
            hit &= ~0x8000u;
        self->activeMotionFlags &= 0x7fffafff;
        hit = (hit & 0xeffefeff) | 4;
        ++self->actionTriggerPhase;
        [[fallthrough]];
    case 3:
        if ((hit & 0x1010) == 0x1010 && self->parameter31e && self->animationTime >= float(self->parameter31e))
            hit &= 1;
        if (self->animationTime >= float(self->reactionEndFrame)) {
            hit &= 0xffffeffe;
            ++self->actionTriggerPhase;
        }
        break;
    default:
        break;
    }
    self->hitStateFlags = hit;
    std::uint8_t soundMode = 0;
    const auto preserved = self->cameraMotionFlags & 0x78;
    if ((self->motionFlags & 0x20000400) == 0x400) {
        soundMode = 2;
        if ((self->motionFlags & 2) || (!(self->savedHitStateFlags & 0x400) && (self->motionFlags & 0x400)))
            soundMode = 3;
        else if ((ActorValue<std::uint8_t>(self, 0xa71) & 0x78) != preserved)
            soundMode = 0x82;
    } else if (self->savedMotionFlags & 0x400)
        soundMode = 4;
    self->cameraMotionFlags = soundMode ? static_cast<std::uint8_t>(preserved | soundMode) : 0;
    if ((hit & 0x80100) == 0x80100)
        self->activeMotionFlags |= 0x80000000;
    if (((self->followupFlags & 0x30f) || (self->cameraControlFlags & 0x6000)) && (self->activeMotionFlags & 0x10000)) {
        self->activeMotionFlags &= ~0x10000u;
        self->hitStateFlags &= ~1u;
    }
    vfUpdateReactAnimationFrames(self);
    self->reactionProgramCounter = self->reactionProgram;
    vfProcessMotionFrameEvents(self);
}


// @verified - p
void vfUpdateMotionTargetPointAndParams(VFActor *self) {
    const auto hit = self->hitStateFlags;
    const auto active = self->activeMotionFlags;
    VFMA_t *const game = self->game;
    const auto target = self->actionIndex;
    if (self->actor->fightStyle != 1 && !(self->moveFlags & 0x800000) && !(game->gameFlags & 0x20000000) &&
        !(self->contactDistances[target] > game->contactRange) && !(active & 0xc00) && !(hit & 0xfc010) && !(game->mode & 1) &&
        g_currentMap.name != SHEN_NAME(GABT) &&
        (((hit & 1) && (active & 0x10000)) || !(hit & 0x101) || (active & 0x200000) ||
            (self->fightStyle == 30 && std::abs(int(self->contactAngles[target]) - int(static_cast<std::int16_t>(self->temporaryFacingAngle))) < 8192))) {
        vfmaSetAngleFor(self->motion, 2048);
        Vector3 position;
        GetCharacterMotionWorldPosition(self->targetingCharacter, 5, &position);
        LatchMotionBoneTarget(FindMotionWork(self->character), 7, &position);
        return;
    }
    if (self->savedMarkerMask & 2)
        vfmaSetAngleFor(self->motion, 0x8000);
    else if (vfHasDialoguePlayerFocus()) {
        const Vector3 position{0, 1.3f, -1};
        vfmaSetAngleFor(self->motion, 0x8000);
        SetMotionBoneTarget(self->motion, 7, &position);
    } else
        vfmaSetAngleFor(self->motion, 4096);
}

// The distance envelope lives in the actor's groundReactionScale word.
// @verified - p
void vfUpdateRootBlendEnvelope(VFActor *self) {
    if (!(self->motionFlags & 0x100)) {
        self->groundReactionScale = 0;
        return;
    }
    const float x = self->contactPosition.x - self->actor->contactPosition.x;
    const float z = self->contactPosition.z - self->actor->contactPosition.z;
    const float distance = std::sqrt(x * x + z * z);
    const float envelope = distance <= 1 ? 1 : (!(distance >= 2) ? 2 - distance : 0);
    const float first = float(static_cast<std::uint16_t>(self->parameter2d0));
    const float middle = float(static_cast<std::uint16_t>(self->reactionMiddleFrame));
    if (self->animationTime < first)
        self->groundReactionScale = (self->animationTime / first) * envelope;
    else if (!(self->animationTime > middle))
        self->groundReactionScale = 1 * envelope;
    else
        self->groundReactionScale = (1 - (self->animationTime - middle) / (float(self->currentAnimationEndFrame) - middle)) * envelope;
}


// @verified - p
void vfclUpdatePairingFacing(VFActor *self) {
    if (self->moveFlags & 0x400000)
        return;
    const auto hit = self->hitStateFlags;
    const auto active = self->activeMotionFlags;
    VFActor *const other = self->actor;
    const auto otherHit = other->hitStateFlags;
    const auto target = self->actionIndex;
    std::int16_t contact;
    if (hit & 0x800000)
        contact = static_cast<std::int16_t>(self->parameter1b6);
    else {
        if ((self->moveFlags & 0x20) || (hit & 0x8404000) || (hit & 0x1010) == 0x1010 || ((hit & 0x10100) == 0x10000 && (otherHit & 0x110)) ||
            (active & 0xc00) || (active & 0x200004) || ((hit & 0x100) && (otherHit & 0x10000000)))
            return;
        contact = self->contactAngles[target];
    }
    std::uint8_t kind = 0;
    if (active & 0x8000000) {
        if (hit & 0x100)
            kind = 5;
        else {
            if (self->fightStyle != 0 || !(self->motionFlags & 4) || !(3 > self->contactDistances[target]) || (ActorValue<std::uint32_t>(self, 0x3c0) & 1) ||
                (self->currentAnimation & 0xfff) == 59 || (self->currentAnimation & 0xfff) == 58)
                return;
            kind = 1;
        }
    }
    if (hit & 0x800000)
        return;
    if (active & 0x1000)
        kind = 5;
    else if (hit & 0x10000000)
        kind = (hit & 0x100) ? 0 : (self->contactDistances[target] < 2 ? 8 : 3);
    else if (active & 0x2084000)
        kind = 2;
    else if (hit & 0x2000)
        kind = 0;
    else if (hit & 0x400)
        kind = 2;
    else {
        if (other->motionFlags & 0x10000000) {
            if (hit & 0x2400)
                kind = 2;
            if ((hit & 0x10000100) || other->hitTimeout > 0)
                return;
        }
        if (hit & 0xc0000) {
            if (hit & 2)
                kind = 4;
            else if (!(active & 0xc00))
                return;
        } else if (((active & 0x800) && (hit & 0x100)) || ((active & 0x400) && (otherHit & 0x100)))
            kind = 2;
        else if ((hit & 0x8100) == 0x8100)
            kind = 2;
        else if (hit & 4) {
            if ((hit & 0x14010) || (self->motionFlags & 0x100))
                return;
            kind = 2;
        } else if ((hit & 0x102) == 0x102 || (hit & 0x1102) == 0x1002)
            kind = 2;
        else if ((hit & 0x20004) == 0x20000)
            kind = 0;
        else if ((otherHit & 0x4010) == 0x4010 && !(hit & 0x200))
            kind = 2;
        else {
            if (hit & 2)
                return;
            if (hit & 0x8000)
                kind = 0;
            else if (hit & 0x40000000)
                kind = 1;
            else
                return;
        }
    }
    constexpr std::int16_t windows[9][2]{
        {32760,5460},{27300,4550},{24570,1820},{18200,16380},{24570,182},
        {32760,182},{16380,910},{25480,3640},{16380,16380}};
    std::int16_t angle = 0, limit = 0, step = 0;
    bool preserveHemisphere = false;
    switch (kind) {
    case 0:
        if (!(hit & 0x8000))
            break;
        [[fallthrough]];
    case 1:
        angle = static_cast<std::int16_t>(contact - self->temporaryFacingAngle);
        limit = windows[kind][0];
        step = windows[kind][1];
        preserveHemisphere = true;
        break;
    case 2: {
        angle = static_cast<std::int16_t>(contact - self->temporaryFacingAngle);
        const auto index = vfCheckSkillFlag(shen_name{0x40000010}) && self->fightStyle == 0 ? 7 : 2;
        limit = windows[index][0];
        step = windows[index][1];
        break;
    }
    case 3:
    case 8:
        angle = static_cast<std::int16_t>(contact - self->temporaryFacingAngle);
        limit = windows[kind][0];
        step = windows[kind][1];
        break;
    case 4:
        angle = static_cast<std::int16_t>(other->facingAngle - self->facingAngle);
        limit = windows[4][0];
        step = windows[4][1];
        break;
    case 5: {
        angle = static_cast<std::int16_t>(contact - self->facingAngle);
        limit = windows[5][0];
        const auto frame = static_cast<std::uint16_t>(self->parameter2d0);
        auto remaining = static_cast<std::int16_t>((frame >> 1) + (frame >> 2) - int(self->animationTime - self->scale));
        if (remaining <= 0)
            remaining = 1;
        step = static_cast<std::int16_t>(angle / remaining);
        break;
    }
    default:
        break;
    }
    if (angle < -int(limit) || angle > limit) {
        if (preserveHemisphere)
            return;
        angle = static_cast<std::int16_t>(angle + 0x8000);
    }
    const auto limited = angle < -int(step) ? static_cast<std::int16_t>(-step) : (angle > step ? step : angle);
    const float degrees = (float(limited) / 182.04445f) * self->scale;
    const float upper = 179.0f < degrees ? 179.0f : degrees, clamped = -179.0f > upper ? -179.0f : upper;
    const auto rotation = static_cast<std::int16_t>(static_cast<std::int32_t>(clamped * 182.0f));
    if (!(self->moveFlags & 4) || !(self->motionContactFlags & 1)) {
        const float x = self->position.x - self->contactPosition.x;
        const float z = self->position.z - self->contactPosition.z;
        const auto opposite = static_cast<std::int16_t>(-rotation);
        const float sine = sin_fixed16(opposite), cosine = cos_fixed16(static_cast<std::uint16_t>(opposite));
        self->position.x = cosine * x - sine * z + self->contactPosition.x;
        self->position.z = sine * x + cosine * z + self->contactPosition.z;
    }
    self->facingAngle = static_cast<std::uint16_t>(self->facingAngle + rotation);
}

// Terrain impact can replace the pending landing reaction.
// @verified - p
void vfUpdateGroundImpactReaction(VFActor *self) {
    if (!(self->moveFlags & 0x8000) || (self->activeMotionFlags & 1))
        return;
    if ((self->hitStateFlags & 0x80000) && self->game->guardTimer > 0) {
        const auto damage = static_cast<std::uint8_t>(self->guardStrength);
        const auto accumulated = static_cast<std::int16_t>(self->parameter40c >> 16);
        self->parameter40c = static_cast<std::uint16_t>(int((float(damage) * 0.5f + float(accumulated)) * self->inverseScale));
        if (damage)
            vfHandleImpactSfxAndDefeat(self, damage, 0);
    }
    const auto direction = static_cast<std::int16_t>(FixedAtan2(self->knockbackVelocity.x, self->knockbackVelocity.z));
    const auto relative = static_cast<std::int16_t>(direction - ((self->hitStateFlags & 0x80000) ? self->temporaryFacingAngle : self->facingAngle));
    std::int16_t animation;
    const auto magnitude = static_cast<std::uint16_t>(relative < 0 ? -int(relative) : int(relative));
    if (static_cast<std::uint16_t>(magnitude + 0x4000) > 0x8000) {
        if (static_cast<std::uint16_t>(self->forwardIntersectionTag + 0x2000) > 0x4000 || !(self->hitStateFlags & 0x800) ||
            !(self->contactPosition.y >= self->targetGroundY + 0.8f))
            return;
        animation = 156;
    } else {
        if (static_cast<std::uint16_t>(self->backwardIntersectionTag + 0x2000) > 0x4000 || (self->hitStateFlags & 0x800) ||
            !(self->contactPosition.y >= self->targetGroundY + 0.8f))
            return;
        animation = !(0.6f > ActorValue<float>(self, 0x280)) ? (self->contactPosition.y > self->targetGroundY + 1.7f ? 353 : 354) : 352;
    }
    self->queuedReactionAnimation = static_cast<std::uint16_t>(animation);
    self->contactPosition.x = self->position.x;
    self->contactPosition.z = self->position.z;
    if (self->knockbackVelocity.y > 0)
        self->knockbackVelocity.y *= 0.4f;
    self->temporaryFacingAngle = static_cast<std::uint16_t>(self->temporaryFacingAngle + self->facingAngleDelta);
    self->queuedReactionEvent = 31;
    self->knockbackAcceleration = self->timeScale;
}

// Restart only a real authored guard reaction for the live contact.
// @verified - p
bool vfTryGuardReactionSwap(VFActor *self, VFActor *source) {
    const auto previous = static_cast<std::int16_t>(self->lastHitMove);
    if ((self->hitStateFlags & 0x2000) && self->parameter44e == static_cast<std::uint16_t>(previous)) {
        float frame = float(static_cast<std::uint16_t>(self->parameter2d0));
        if (frame > 0)
            frame -= self->scale;
        frame = 0 > frame ? 0 : frame;
        if (self->animationTime > frame)
            self->animationTime = self->parameter4c8 = frame;
        return true;
    }
    if (self->groundContactFlags & 1)
        return false;
    g_vfSelectedReactionAnimation = 0;
    vfclChooseReactionAnim(source, self);
    const auto animation = g_vfSelectedReactionAnimation;
    if (!animation)
        return false;
    const float remaining = float(static_cast<std::uint16_t>(source->parameter2d0)) - self->pairAnimationTime;
    const auto metadata = FindLoadedMotionSecondaryEntry(animation & 0xfff, 2);
    if (metadata.empty())
        return false;
    std::int16_t first;
    std::memcpy(&first, metadata.data() + 2, sizeof(first));
    if (remaining > float(first) - 1 - self->scale)
        return false;
    self->pendingAnimation = animation;
    vfApplyHitReactionAndStartMotion(self);
    self->parameter44e = static_cast<std::uint16_t>(previous);
    return true;
}

// Authored last-enemy camera admission consumes real team counters.
// @verified - p
bool vfIsLastEnemyForCamera(VFActor *self) {
    if (self->game->slotTeams[self->groupIndex] != 1 || g_pFightStage->secondaryGroupCounters[1] || g_pFightStage->readyEnemySlots[1])
        return false;
    const auto count = g_pFightStage->unknownGroupCounts[1];
    return (count == 1 && !(self->moveFlags & 0x20)) || (count == 0 && (self->moveFlags & 0x20));
}

// The Q100 authored box or the real type-eight floor bounds admission.
// @verified - p
bool vfIsWithinFightFloor(const Vector3 &position) {
    if (g_currentMap.name == SHEN_NAME(Q100)) {
        const float x = position.x - g_pFightStage->spawnPoints[0].x;
        const float z = position.z - g_pFightStage->spawnPoints[0].z;
        const auto yaw = static_cast<std::int16_t>(-g_pFightStage->teamRespawnTimers[0]);
        const float sine = sin_fixed16(yaw), cosine = cos_fixed16(static_cast<std::uint16_t>(yaw));
        if (std::fabs(cosine * x - sine * z) > 5)
            return false;
        return !(std::fabs(sine * x + cosine * z) > 5);
    }
    const Vector3 query{position.x, position.y + 0.5f, position.z};
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return false;
    if (game->type != 8)
        return true;
    return !(double(game->floorIntersectionY) - 0.45 > double(flddFindIntersectionBy(query)));
}

namespace {
// -1 means no winner among at least two real participants.
// @verified - p
std::int32_t FloorWinner(VFMA_t *game) {
    std::int32_t count = 0;
    for (std::int32_t i = 0; i != 6; ++i)
        if (game->characters[i]) {
            ++count;
            if (game->actors[i]->stateFlags & 0x8000)
                return i;
        }
    return count < 2 ? 99 : -1;
}

// Defeat publication also resets the live paired reaction producer.
// @verified - p
void BeginFloorDefeat(VFActor *self) {
    self->stateFlags |= 0x80;
    self->secondaryMotionFlags |= 0x20;
    self->activeMotionFlags |= 0x20;
    if ((self->hitStateFlags & 0x80000) && self->secondActor)
        vfResetReactionProgram(self->secondActor);
    if (FloorWinner(self->game) != -1)
        return;
    self->stateFlags |= 0x8000;
    const bool player = self->groupIndex == static_cast<std::uint8_t>(self->game->playerSlot);
    if (g_pFightStage->battleFlags & 0x40000)
        vfAddRoundResults(!player, player);
    if (player)
        vfResetMoveTrainingGains();
}
}

// Type-eight authored floor escape/reaction selection.
// @verified - p
void vfUpdateFightFloorReaction(VFActor *self) {
    VFMA_t *const game = self->game;
    VFMA_t *const current = vfmaGetTaskParam();
    const bool authored = current && (current->mode & 0x80000);
    if (static_cast<std::uint32_t>(game->state) < 3 || game->type != 8 || (authored && current->parameter570 < 5))
        return;
    VFActor *const paired = self->pairedActor;
    if (authored && (self->hitStateFlags & 0x40000) && !(paired->stateFlags & 0x80) && !(paired->moveFlags & 0x20))
        return;
    const bool playerEscape = self->groupIndex == game->playerSlot && (self->hitStateFlags & 0x80000) && !authored;
    const Vector3 point{(ActorValue<float>(self, 0x10c) + ActorValue<float>(self, 0xf4)) * 0.5f * 0.3f + ActorValue<float>(self, 0xac) * 0.7f, self->position.y,
        (ActorValue<float>(self, 0x114) + ActorValue<float>(self, 0xfc)) * 0.5f * 0.3f + ActorValue<float>(self, 0xb4) * 0.7f};
    if (vfIsWithinFightFloor(point))
        self->stateFlags &= ~0x100u;
    else
        self->stateFlags |= 0x100;
    if (self->stateFlags & 0x80) {
        if (SetMotionRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody) && FloorWinner(game) == static_cast<std::int8_t>(self->groupIndex))
            self->moveFlags |= 0x20;
        return;
    }
    auto ground = std::uint32_t(self->groundFlags);
    if (self->moveFlags & 0x40) {
        constexpr std::uint32_t swaps[4][2]{{8, 64}, {16, 128}, {1024, 8192}, {2048, 16384}};
        ground &= 0x9327;
        for (const auto &swap : swaps) {
            if (self->groundFlags & swap[0])
                ground |= swap[1];
            if (self->groundFlags & swap[1])
                ground |= swap[0];
        }
    }
    const auto inverse = static_cast<std::uint16_t>(~ground);
    const auto &spawn = g_pFightStage->spawnPoints[self->nearestSpawnPoint];
    const auto quadrant = static_cast<std::uint16_t>((FixedAtan2(point.x - spawn.x, point.z - spawn.z) - 24576) & 0xc000);
    const auto back =
        static_cast<std::uint16_t>((quadrant - ((self->hitStateFlags & 0x40000) ? self->facingYaw : self->temporaryFacingAngle) + 0x4000) & 0x8000);
    const auto hit = self->hitStateFlags;
    const auto contact = self->parameter45a;
    std::int32_t choice;
    bool bypassPlayerEscape = false;
    if ((contact & 0x4800) && !(contact & 0xb7ff) && !(hit & 0x18000)) {
        if (!(inverse & contact & 0x4800) && !(self->stateFlags & 0x100))
            return;
        choice = back != 0 ? 1 : (hit & 0x80000) ? 14 : 0;
    } else if (((hit & 0x84000) != 0x84000 && !(contact & self->footContactState)) ||
        ((hit & 0x1010) && !(self->groundOffset + game->floorIntersectionY >= self->contactPosition.y))) {
        if (ground)
            return;
        if (FloorWinner(game) == -1) {
            if (playerEscape) {
                vfStartAuthoredDefeatCamera(self->secondActor);
                self->stateFlags &= ~0x100u;
                return;
            }
            BeginFloorDefeat(self);
            self->contactVectors[2].x = self->contactPosition.x;
            self->contactVectors[2].z = self->contactPosition.z;
            self->parameter370 = 0x19000000;
            self->hitPhase = 1;
            return;
        }
        choice = 1;
        bypassPlayerEscape = true;
    } else {
        auto edges = static_cast<std::uint8_t>((self->stateFlags >> 7) & 2);
        if (inverse & 0x800)
            ++edges;
        if (inverse & 0x4000)
            ++edges;
        if (edges <= 1)
            return;
        if ((hit & 0xc0000) && static_cast<std::uint8_t>(self->footFacingMode) != 2)
            choice = FloorWinner(game) == -1 && !back ? 14 : 1;
        else {
            if ((inverse & 0x4800) == 0x4800 && !(inverse & 4))
                choice = 2;
            else if ((inverse & 0x4800) != 0x4800 && (inverse & 4))
                choice = 4;
            else if ((ground & 0x4890) == 0x4080)
                choice = 10;
            else if ((ground & 0x4890) == 0x810)
                choice = 12;
            else
                return;
            if (!(hit & 0x400) && (hit & 0x800) && !(ActorValue<float>(self, 0x280) > 0))
                ++choice;
        }
    }
    if (playerEscape && !bypassPlayerEscape) {
        vfStartAuthoredDefeatCamera(self->secondActor);
        self->stateFlags &= ~0x100u;
        return;
    }
    constexpr std::uint32_t animations[15]{279, 278, 271, 273, 270, 272, 275, 277, 274, 276, 99, 32868, 32867, 100, 672};
    constexpr std::uint16_t turns[15]{0, 0x8000, 0, 0, 0x8000, 0x8000, 0, 0, 0x8000, 0x8000, 0x4000, 0xc000, 0xc000, 0x4000, 0x8000};
    self->moveFlags |= 0x10;
    BeginFloorDefeat(self);
    const auto vertical = game->floorIntersectionY - self->position.y;
    self->position.y += vertical;
    self->contactPosition.y += vertical;
    self->pendingAnimation = animations[choice];
    self->knockbackAcceleration = g_currentMap.name == SHEN_NAME(QLBT) ? 0.5f : self->scale;
    if (FloorWinner(self->game) != static_cast<std::int8_t>(self->groupIndex))
        self->knockbackAcceleration *= 0.7f;
    if ((self->hitStateFlags & 0x40000) && !(paired->activeMotionFlags & 0x80) && !(paired->stateFlags & 0x80) && !(paired->hitStateFlags & 0x4000))
        paired->parameter370 = 0x01000000;
    vfApplyHitReactionAndStartMotion(self);
    self->parameter370 = 0x19000000;
    self->hitPhase = 1;
    self->facingAngle = self->parameter1b6 = self->facingYaw = self->temporaryFacingAngle = static_cast<std::uint16_t>(quadrant + turns[choice]);
    float best = 9999, x = self->position.x, z = self->position.z;
    for (std::size_t i = 0; i != 10; ++i) {
        std::uint16_t tag;
        std::memcpy(&tag, self->parameterAd8 + i * 20 + 18, sizeof(tag));
        if (!(tag & ActorValue<std::uint16_t>(self, 0x45e)))
            continue;
        const float sx = self->motionContactSamples[i].x, sz = self->motionContactSamples[i].z;
        const float distance = std::sqrt((sx - spawn.x) * (sx - spawn.x) + (sz - spawn.z) * (sz - spawn.z));
        if (best > distance) {
            best = distance;
            x = sx;
            z = sz;
        }
    }
    self->contactVectors[2].x = x;
    self->contactVectors[2].z = z;
}


// @verified - p
void vfActorUpdateState(VFActor *self, VFActor *other) {
    VFMA_t *const game = self->game;
    if ((self->hitStateFlags & 0x10000002) && (other->hitStateFlags & 0x100))
        self->hitTimeout = 5;
    if (((other->hitStateFlags & 2) || !(other->hitStateFlags & 0x100)) && self->hitTimeout > 0) {
        const float timeout = self->hitTimeout - shenGetScaledDeltaTickTime(1);
        self->hitTimeout = 0 > timeout ? 0 : timeout;
    }
    self->previousHitStateFlags = self->hitStateFlags;
    self->previousActiveMotionFlags = self->activeMotionFlags;
    self->previousAnimation = self->currentAnimation;
    const std::int8_t quadrant = vfUpdatePredicate_40_41_44_50_51_54();
    if (quadrant != self->cooldownQuadrant) {
        self->previousCooldownQuadrant = self->cooldownQuadrant;
        self->cooldownQuadrant = quadrant;
    }
    auto move = self->moveFlags & ~0x801u;
    if (!self->currentAnimation)
        move |= 1;
    else if (self->moveFlags & 0x80) {
        self->animationTime += shenGetScaledDeltaTickTime(self->scale);
        self->tickFrameTime += shenGetScaledDeltaTickTime(1);
        if (self->animationTime > float(self->currentAnimationEndFrame))
            move |= 1;
    }
    self->moveFlags = move;
    vfUpdateAirborneAnimationTiming(self);
    vfDispatchCurrentActionState(self);
    self->action = 0;
    vfUpdateDynamicActionState(self);
    vfUpdateFightFloorReaction(self);
    vfDispatchQueuedReactionEvent(self);
    vfUpdateAnimFlagsAndEvents(self);
    SetActorValue(self, 0x8b4, vfGetMoveCategoryMaskFor(self));
    vfclUpdatePairingFacing(self);
    vfApplyKnockbackPhysics(self);
    if (self->hitStateFlags & 0x800000)
        self->targetGroundY =
            g_currentMap.name == SHEN_NAME(QLBT) && self->animationTime >= float(self->parameter2d6) ? game->floorSnapY : game->floorIntersectionY;
    else
        self->targetGroundY = game->type != 8 || (self->stateFlags & 0x80) ? vfFieldGetIntersection(self, self->contactPosition) : game->floorIntersectionY;
    self->groundY = self->targetGroundY;
    vfUpdateGroundHeightAndEnemyContacts(self);
    vfUpdateAutoAdvanceLanding(self);
    vfUpdateMotionTargetPointAndParams(self);
    vfUpdateRootBlendEnvelope(self);
    self->knockbackStrength = -sin_fixed16(self->facingYaw);
    self->animationScale = -cos_fixed16(self->facingYaw);
    vfUpdatePositionXZ(self);
}

// HP assist/drain follows the pose update, not a separate tick gate.
// @verified - p
void vfUpdatePoseWithSfx(VFActor *self) {
    vfActorUpdateState(self, self->actor);
    VFMA_t *const game = self->game;
    if (game->mode & 0x80000)
        return;
    if (self->formationFlags & 0x40000) {
        if (!(self->hitStateFlags & 0xc0000) &&
            (self->followupState < 2 || (int(game->frame) % self->followupState == 0 && int(game->previousFrame) % self->followupState != 0)))
            vfHandleImpactSfxAndDefeat(self, 1, 2);
    } else {
        vfUpdateHPRegeneration(self);
        if (self->groupIndex == game->playerSlot && game->type != 4 && game->type != 9 && !(self->formationFlags & 0x30000) && game->hpTimer <= 0) {
            self->followupState = 5;
            self->formationFlags = (self->formationFlags & ~0x04000000) | 0x40000;
        }
    }
}


// @verified - p
void vfInitializeFightActor(VFMA_t *game, std::uint8_t index) {
    VFActor *const self = game->actors[index];
    FRMG_t *const group = game->groups[index];
    self->groupIndex = index;
    self->character = game->characters[index];
    self->motion = FindMotionWork(self->character);
    self->game = game;
    const auto *const raw = reinterpret_cast<const std::byte *>(group);
    const auto word = [raw](std::size_t offset) {
        std::uint16_t value;
        std::memcpy(&value, raw + offset, sizeof(value));
        return value;
    };
    self->fightStyle = std::to_integer<std::uint8_t>(raw[0x1c]);
    std::memcpy(&self->formationFlags, raw + 0x28, 4);
    self->formationEntry = word(0x32);
    self->formationHpFloor = static_cast<std::int16_t>(word(0x34));
    SetActorValue(self, 0xa54, word(0x36));
    SetActorValue(self, 0xa56, word(0x38));
    self->spawnYaw[0] = word(0x3a);
    self->followupState = std::to_integer<std::uint8_t>(raw[0x30]);
    self->spawnYaw[1] = word(0x3c);
    self->sfxPeriod = word(0x3e);
    self->selectionPriority = std::to_integer<std::uint8_t>(raw[0x24]);
    if (raw[0x53] != std::byte{0xff})
        self->reactionRank = std::to_integer<std::uint8_t>(raw[0x53]);
    if (!group->totalHp)
        group->totalHp = 200;
    if (!group->hp)
        group->hp = 200;
    self->initialHp = group->hp;
    self->hp = float(group->totalHp);
    self->guardHp = self->guardMaximumHp = self->fightStyle == 0 ? 500 : std::min(float(group->totalHp), 200.0f);
    self->regenerationRate = self->fightStyle == 0 ? 0.4f : 0.5f;
    self->focusedRegenerationScale = self->fightStyle == 0 ? 0.2f * 0.8f : 0.2f;
    vfInitializeActorMotionState(self);
    const auto start = std::to_integer<std::uint8_t>(raw[0x54]);
    if (start == 2 || (start == 0xff && game->type == 2))
        self->stateFlags |= 8;
    else if (start == 1 || start == 0xff)
        self->stateFlags |= 4;
    if ((game->type == 1 && !(std::to_integer<std::uint8_t>(raw[0x24]) & 0x80)) || static_cast<std::uint8_t>(game->type - 2) <= 1)
        self->stateFlags |= 2;
    vfActorUpdateState(self, self->actor);
    self->moveFlags |= 0x10000080;
}

namespace {

std::int8_t timerColourMode{};
constexpr std::uint32_t timerDivisors[6]{1800000, 180000, 30000, 3000, 300, 30};
struct TimerDigitPlace {
    std::uint16_t x, y;
    std::uint8_t size;
};
constexpr TimerDigitPlace timerDigitPlaces[6]{{18, 0, 32}, {46, 0, 32}, {100, 0, 32}, {132, 0, 32}, {178, 8, 16}, {194, 8, 16}};

std::uint32_t TimerColour(std::uint32_t elapsedFrames, std::uint32_t current) {
    switch (timerColourMode) {
    case 0:
        return elapsedFrames < 300 ? 0xffff0000U : 0xffffffffU;
    case 1:
        return elapsedFrames < 300 ? 0xffff0000U : elapsedFrames < 600 ? 0xffffff00U : 0xffffffffU;
    case 2:
        return elapsedFrames < 300 ? 0xffff0000U : 0xffffff00U;
    default:
        return current;
    }
}
}


// @verified - p
void vfDrawFightLoadSprite(std::int32_t index, std::int32_t x, std::int32_t y, float depth, std::uint32_t elapsedFrames) {
    const FightLoadSpriteRecord &sprite = g_fightLoadSprites[index];
    if (sprite.textureIndex == -1)
        return;
    LegacySpritePacket packet;
    packet.quad.flags = 0;
    packet.secondaryColour = 0;
    packet.colour = 0xffffffffU;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, y, sprite.width, sprite.height, static_cast<ScreenMode>(0));
    packet.quad.flags |= 0x400U;
    packet.colour = TimerColour(elapsedFrames, 0);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(sprite.textureIndex), depth);
}



// @verified - p
void ConstructColouredText(std::uint32_t elapsedFrames, std::int32_t x, std::int32_t y, float depth) {
    LegacySpritePacket packet;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    packet.quad.flags = 0x400U;
    std::uint8_t first = elapsedFrames > 18000 ? 0 : elapsedFrames > 1800 ? 1 : 2;
    if (!GetDebugForceUIEnabled(UIElement::StreetFighterHud)) {
        if (first < 2)
            vfDrawFightLoadSprite(0, x + 74, y, depth, elapsedFrames);
        vfDrawFightLoadSprite(0, x + 162, y, depth, elapsedFrames);
    }
    std::array<std::int32_t, 6> digits{};
    std::uint32_t value = (elapsedFrames > 108000 ? 108000 : elapsedFrames) * 100;
    std::uint32_t colour = 0;
    for (; first < 6; ++first) {
        const std::uint32_t digit = value / timerDivisors[first];
        value %= timerDivisors[first];
        digits[first] = static_cast<std::int32_t>(digit);
        colour = TimerColour(elapsedFrames, colour);
        const TimerDigitPlace &place = timerDigitPlaces[first];
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, place.x + x, place.y + y, place.size, place.size, ScreenMode::Centered);
        packet.colour = colour;
        if (!GetDebugForceUIEnabled(UIElement::StreetFighterHud))
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_fightLoadDigits[digit]), depth);
    }
    if (GetDebugForceUIEnabled(UIElement::StreetFighterHud)) {
        SetStreetFighterTimerDigits(digits);
        EnableElement(UIElement::StreetFighterHud);
    }
}


// @verified - p
void constructNumStrikesLeft() {
    if (!(g_pFightStage->battleFlags & 0x20000))
        return;
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return;
    const std::int32_t strikes = static_cast<std::int8_t>(game->strikesLeft);
    ConstructNumStrikesLeftText(strikes);
    LegacySpritePacket packet;
    packet.quad.flags = 0x400U;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 235, 87, 32, 32, ScreenMode::AdjustedWithOffset);
    // Only the digit sprite is guarded; a negative count reads ahead of the digit table natively, into a nullsub.
    if (static_cast<std::uint32_t>(strikes) < 10 && g_fightLoadDigits[strikes] != -1)
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_fightLoadDigits[strikes]), 1.0f);
    packet.quad.flags = 0x400U;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 383, 86, 256, 32, 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_vfStrikeLabelSprites[0]), 1.0f);
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 451, 86, 128, 32, 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_vfStrikeLabelSprites[1]), 1.0f);
}

} // namespace shenmue::s2
