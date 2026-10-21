#include "s2/motion_work.hpp"
#include "s2/event_transform_flags.hpp"

#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/map_control.hpp"
#include "s2/fight.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/hand.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <limits>

namespace shenmue::s2 {




std::int32_t g_usingEnv = 1;
std::int32_t g_motionDrawOverridesEnabled{};
std::int32_t g_motionDeformationEnabled{1};
std::uint32_t g_motionDrawOverrideFlags{};
std::array<const Matrix4x4 *, 7> g_motionDeformationControllers{};
MotionWork *g_activeMotionWork{};
std::int32_t g_mtwkCurrentIsRyo{};

// @verified - p
void SetActiveMTWK(MotionWork *work) {
    g_activeMotionWork = work;
}

// @verified - p
void mtwkGetArmHandPoseChanges(MotionWork *work, std::int32_t *shapes, std::int32_t *frames) {
    MotionRuntimeState &state = work->runtime;
    for (std::uint32_t side = 0; side < 2; ++side) {
        frames[side] = 0;
        const std::uint32_t region = side + 3;
        const std::uint16_t flags = state.regionFlags[region];
        const bool mirrored = (flags & 0x100U) ? (flags & 0x40U) != 0 : (state.regionFlags[0] & MRF_Mirrored) != 0;
        const MotionRigClock &clock = side ? work->rig.leftArm.clock : work->rig.rightArm.clock;
        float time = (flags & 0x10U) ? state.layerSampleFrame[region] : clock.current.ticksTime;
        if ((flags & 0x10U) && time < 0.0f)
            time = 0.0f;
        float blend = clock.playbackSpeed;
        const std::byte *track = static_cast<const std::byte *>(side ? state.leftArmTrack : state.rightArmTrack);
        if (track) {
            if (blend < 0.00000011920929f)
                blend = 1.0f;
            std::uint16_t count;
            std::memcpy(&count, track, sizeof(count));
            if (mirrored != (side != 0)) {
                track += static_cast<std::size_t>(count) * 4 + 2;
                std::memcpy(&count, track, sizeof(count));
            }
            track += 2;
            shapes[side] = 0;
            for (std::int32_t entry = 0; entry < static_cast<std::int32_t>(count) - 1; ++entry) {
                std::uint16_t pair[4];
                std::memcpy(pair, track + static_cast<std::size_t>(entry) * 4, sizeof(pair));
                if (!(time >= static_cast<float>(pair[0]) && static_cast<float>(pair[2]) > time))
                    continue;
                shapes[side] = pair[3];
                if (pair[1] != pair[3])
                    frames[side] = TruncateToInt32((static_cast<float>(pair[2]) - time) / blend);
                break;
            }
            if (shapes[side] != state.handPoseIndices[side]) {
                state.handPoseIndices[side] = static_cast<std::uint8_t>(shapes[side]);
                continue;
            }
        } else if (state.handPoseIndices[side]) {
            state.handPoseIndices[side] = 0;
            shapes[side] = 0;
            continue;
        }
        shapes[side] = -1;
    }
}

// @verified - p
std::int64_t mtwkSetOverridesEnabled(std::int32_t enabled) {
    const std::int32_t previous = g_motionDrawOverridesEnabled;
    g_motionDrawOverridesEnabled = enabled;
    return previous;
}
namespace {



std::uint32_t sharedPrngSeed = 1U;
std::int32_t ParentBoneId(std::uint32_t selector);

struct DefaultMotionSet {
    std::uint16_t slots[6];
};


constexpr DefaultMotionSet defaultMotionSets[] = {
    {{0xf085, 0xf005, 0xf001, 0xf001, 0xf006, 0xf085}},
    {{0xf085, 0xf005, 0xf001, 0xf001, 0xf006, 0xf085}},
    {{0xf03f, 0xf03e, 0xf001, 0xf001, 0xf006, 0xf03f}},
    {{0xf05f, 0xf0ff, 0xf001, 0xf001, 0xf006, 0xf05f}},
    {{0xf07f, 0xf07e, 0xf001, 0xf001, 0xf006, 0xf07f}},
    {{0xf057, 0xf084, 0xf001, 0xf001, 0xf0e6, 0xf057}},
    {{0xf09f, 0xf09e, 0xf001, 0xf001, 0xf0e6, 0xf09f}},
    {{0xf0d2, 0xf0d1, 0xf001, 0xf001, 0xf0e6, 0xf0d2}},
    {{0xf061, 0xf060, 0xf001, 0xf001, 0xf006, 0xf061}},
    {{0xf087, 0xf086, 0xf001, 0xf001, 0xf006, 0xf087}},
    {{0xf079, 0xf078, 0xf001, 0xf001, 0xf006, 0xf079}},
    {{0xf059, 0xf058, 0xf001, 0xf001, 0xf006, 0xf059}},
    {{0xf0a8, 0xf058, 0xf001, 0xf001, 0xf006, 0xf0a8}},
    {{0xf085, 0xf084, 0xf001, 0xf001, 0xf006, 0xf085}},
    {{0xf130, 0xf0a4, 0xf001, 0xf001, 0xf006, 0xf130}},
    {{0xf0a7, 0xf0a6, 0xf001, 0xf001, 0xf006, 0xf0a7}},
    {{0xf0ea, 0xf0f6, 0xf001, 0xf001, 0xf006, 0xf0ea}},
};

// getBoneId
// @verified - p
std::size_t DefaultMotionSetIndex(std::uint32_t rigBoneId) {
    if (rigBoneId <= 7U)
        return rigBoneId + 2U;
    if (rigBoneId >= 9U && rigBoneId <= 15U)
        return rigBoneId + 1U;
    return 0;
}

// @verified - p
void EnsureDefaultMotionSet(MotionWork &work) {
    if (work.runtime.extraMotionIds[0])
        return;
    const DefaultMotionSet &defaults = defaultMotionSets[DefaultMotionSetIndex(work.rig.boneId)];
    for (std::size_t slot = 0; slot < 6; ++slot)
        work.runtime.extraMotionIds[slot] = defaults.slots[slot];
}

std::uint8_t ClassifyCurrentLocomotionPhase(const MotionWork &work) {
    const std::uint16_t motion = work.runtime.currentMotionId[0];
    if (motion == work.runtime.extraMotionIds[1])
        return 1;
    if (motion == work.runtime.extraMotionIds[4])
        return 2;
    for (const DefaultMotionSet &defaults : defaultMotionSets) {
        if (motion == defaults.slots[1])
            return 1;
        if (motion == defaults.slots[4])
            return 2;
    }
    return 0;
}

template <typename Work> auto RigClock(Work *work, MotionRegion region) -> decltype(&work->rig.lowerBody.clock) {
    if (!work)
        return nullptr;
    switch (region) {
    case MotionRegion::LowerBody:
        return &work->rig.lowerBody.clock;
    case MotionRegion::Chest:
        return &work->rig.chest.clock;
    case MotionRegion::Head:
        return &work->rig.head.clock;
    case MotionRegion::RightArm:
        return &work->rig.rightArm.clock;
    case MotionRegion::LeftArm:
        return &work->rig.leftArm.clock;
    default:
        return nullptr;
    }
}

// @verified - p
void InitializeMotionTargetOverrides(MotionWork &work) {

    const auto bind = [&](MotionController controller, MotionRigAxis(&axes)[3], Vector4 *pva) {
        MotionTargetParameters &slot = work.targets[motionControllerTargets[controller]];
        for (std::size_t axis = 0; axis < 3; ++axis)
            slot.channels[axis] = &axes[axis].channel;
        slot.pva = pva;
        slot.controller = &motionControllerIndices[controller];
        slot.callback = nullptr;
        slot.user = nullptr;
    };
    bind(MC_Pelvis, work.rig.lowerBody.pelvisPosition, work.rig.lowerBody.pelvisPva);
    bind(MC_LeftFoot, work.rig.lowerBody.leftFoot.axes, work.rig.lowerBody.leftFoot.pva);
    bind(MC_RightFoot, work.rig.lowerBody.rightFoot.axes, work.rig.lowerBody.rightFoot.pva);
    bind(MC_Chest, work.rig.chest.target.axes, work.rig.chest.target.pva);
    bind(MC_Head, work.rig.head.target.axes, work.rig.head.target.pva);
    bind(MC_RightArm, work.rig.rightArm.target.axes, work.rig.rightArm.target.pva);
    bind(MC_LeftArm, work.rig.leftArm.target.axes, work.rig.leftArm.target.pva);
    if (work.rig.useOverride != 0) {
        for (const MotionController controller : {MC_PelvisOverride, MC_HeadAim}) {
            MotionRigTarget *const node = controller == MC_PelvisOverride ? work.rig.lowerBody.overridePelvisTarget : work.rig.head.aimTargetNode;
            MotionTargetParameters &slot = work.targets[motionControllerTargets[controller]];
            for (std::size_t axis = 0; axis < 3; ++axis)
                slot.channels[axis] = &node->axes[axis].channel;
            slot.pva = node->pva;
            slot.controller = &motionControllerIndices[controller];
            slot.callback = nullptr;
            slot.user = nullptr;
        }
    }
}

// @verified-s2: 0x7ff6185de8a0. Native rotation callback parameter blocks are

void InitializeMotionRotationPointers(MotionWork &work) {
    auto *const bytes = reinterpret_cast<std::byte *>(&work);
    const auto storePointer = [&](std::size_t destination, const void *pointer) { std::memcpy(bytes + destination, &pointer, sizeof(pointer)); };
    const auto bind = [&](std::size_t destination, std::byte *channels, void *basis, std::size_t layer) {
        for (std::size_t axis = 0; axis < 3; ++axis)
            storePointer(destination + axis * 8, reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(channels) + axis * 0x50));
        storePointer(destination + 0x18, basis);
        storePointer(destination + 0x20, &work.runtime.layerBlendAlpha[layer]);
    };
    static constexpr std::uint16_t records[][4]{
        {0x2898,0x1a0,0x2d0,0}, {0x2910,0x4e0,0x5d0,0}, {0x2938,0x7c8,0x8f8,0},
        {0x2988,0xab8,0xba8,0}, {0x29b0,0xda0,0xed0,0},
        {0x2a50,0x10d0,0x11c0,1}, {0x2a78,0x1378,0x1468,1},
        {0x2aa0,0x1518,0x1608,2}, {0x2af0,0x1688,0x1778,2},
        {0x2b90,0x19a8,0x1a98,3}, {0x2bb8,0x1b18,0x1c08,3},
        {0x2be0,0x1e80,0x1e00,3}, {0x2c08,0x1e80,0x1fb0,3},
        {0x2c30,0x2010,0x2100,4}, {0x2c58,0x2180,0x2270,4},
        {0x2c80,0x24e8,0x2468,4}, {0x2ca8,0x24e8,0x2618,4}};
    for (const auto &record : records)
        bind(record[0], bytes + record[1], bytes + record[2], record[3]);
    if (work.rig.useOverride != 0) {
        static constexpr std::uint16_t nodeRecords[][2]{{0x28c0, 0x480}, {0x28e8, 0x4d0}, {0x2960, 0xaa8}};
        for (const auto &record : nodeRecords) {
            std::byte *node{};
            std::memcpy(&node, bytes + record[1], sizeof(node));
            bind(record[0], node, node + 0xf0, 0);
        }
        static constexpr std::uint16_t toeRecords[][2]{{0x29d8, 0x1080}, {0x2a00, 0x1090}, {0x2a28, 0x10a0}};
        for (const auto &record : toeRecords) {
            std::byte *node{};
            std::memcpy(&node, bytes + record[1], sizeof(node));
            storePointer(record[0], node);
            storePointer(record[0] + 8, node + 0x50);
            storePointer(record[0] + 0x18, node + 0xa0);
            storePointer(record[0] + 0x20, &work.runtime.layerBlendAlpha[0]);
        }
    } else {
        bind(0x28c0, bytes + 0x350, bytes + 0x440, 0);
        static constexpr std::uint16_t headRecords[][2]{{0x2ac8, 0x1940}, {0x2b18, 0x1958}, {0x2b40, 0x1968}, {0x2b68, 0x1978}};
        for (const auto &record : headRecords) {
            std::byte *node{};
            std::memcpy(&node, bytes + record[1], sizeof(node));
            bind(record[0], node, node ? node + 0xf0 : nullptr, 2);
        }
    }
}

#include "s2/motion_rig_constants.inc"

// @verified - p
void InitializeMotionRig(MotionWork &work, std::uint32_t rigType) {
    std::memset(&work.rig, 0, 0x2658);
    const std::uint32_t *const defaults = motionRigDefaults[rigType];
    if (std::fabs(static_cast<float>(1.1 - std::bit_cast<float>(defaults[0]))) > 0.01) {
        char diagnostic[1024];
        std::sprintf(diagnostic, "%s(%d): nlError(%s)\n", "motion\\src\\nlmotion.c", 165, "NL_ERROR_BONE_VERSION");
    }
    work.rig.boneId = rigType;
    work.rig.useOverride = rigType == 16U;
    g_motionUseOverride = static_cast<std::int32_t>(work.rig.useOverride);
    auto *const storage = reinterpret_cast<std::byte *>(&work);
    const auto put = [&](std::size_t destination, std::size_t source) { std::memcpy(storage + destination, defaults + source / 4, sizeof(std::uint32_t)); };
    if (work.rig.useOverride) {
        auto *const unfo = static_cast<std::byte *>(work.runtime.unfo);
        static constexpr std::uint16_t pointers[][2]{
            {0x480,0}, {0x488,0x130}, {0x4d0,0x268}, {0x4d8,0x398},
            {0xaa8,0x3d8}, {0xab0,0x508}, {0x1080,0x548}, {0x1088,0x628},
            {0x1090,0x668}, {0x1098,0x748}, {0x10a0,0x788}, {0x10a8,0x868},
            {0x1940,0x8a8}, {0x1948,0x9d8}, {0x1950,0xb10}, {0x1958,0xb50},
            {0x1960,0xc80}, {0x1968,0xcc0}, {0x1970,0xdf0}, {0x1978,0xe30},
            {0x1980, 0xf60}};
        for (const auto &entry : pointers) {
            void *const pointer = unfo + entry[1];
            std::memcpy(storage + entry[0], &pointer, sizeof(pointer));
        }
        static constexpr std::uint16_t unfoWords[][2]{
            {0x8f0,0x1c}, {0x940,0x20}, {0xa20,0x24},
            {0xbe8,0x38}, {0xc38,0x3c}, {0xd58,0x40}, {0xda8,0x44},
            {0xe78,0x48}, {0xf18,0x4c}, {0xe8,0xa4}, {0x178,0xa8},
            {0x2b0,0xac}, {0x300,0xb0}, {0x350,0xb4},
            {0x420,0xd4}, {0x470,0xd8}, {0x4c0,0xdc},
            {0x590,0xfc}, {0x5e0,0x100}, {0x6b0,0x104}, {0x700,0x108},
            {0x7d0, 0x10c}, {0x820, 0x110}};
        for (const auto &entry : unfoWords)
            std::memcpy(unfo + entry[0], defaults + entry[1] / 4, sizeof(std::uint32_t));
        static constexpr std::uint16_t words[][2]{
            {0x11b8,8}, {0x15b0,0x18}, {0x16d0,0x28}, {0x1720,0x2c},
            {0x19f0,0x50}, {0x1a40,0x54}, {0x1a90,0x58}, {0x1b60,0x5c},
            {0x1bb0,0x60}, {0x1c00,0x64}, {0x1ce0,0x68},
            {0x20a8,0x7c}, {0x20f8,0x80}, {0x21c8,0x84}, {0x2218,0x88},
            {0x2268,0x8c}, {0x2348,0x90}, {0x528,0xb8}, {0x578,0xbc},
            {0xb00, 0xe0}, {0xb50, 0xe4}};
        for (const auto &entry : words)
            put(entry[0], entry[1]);
    } else {
        static constexpr std::uint16_t words[][2]{
            {0x1168,4}, {0x1560,0x14}, {0x16d0,0x28}, {0x1930,0x30}, {0x1938,0x34},
            {0x19f0,0x50}, {0x1a90,0x58}, {0x1c00,0x64}, {0x1ce0,0x68},
            {0x1ec8,0x74}, {0x1f18,0x70}, {0x20f8,0x80}, {0x2268,0x8c},
            {0x2348,0x90}, {0x2530,0x9c}, {0x2580,0x98},
            {0x3e8,0xa0}, {0x578,0xbc}, {0x5c8,0xc0}, {0x9d0,0xcc},
            {0xa20,0xd0}, {0xb50,0xe4}, {0xba0,0xe8}, {0xfa8,0xf4}, {0xff8,0xf8}};
        for (const auto &entry : words)
            put(entry[0], entry[1]);
    }
    put(0x1248, 0xc);
    put(0x13c0, 0x10);
    put(0x1c90, 0x6c);
    put(0x2058, 0x78);
    put(0x22f8, 0x94);
    put(0x658, 0xc8);
    put(0x6a8, 0xc4);
    put(0xc30, 0xf0);
    put(0xc80, 0xec);
}

// @verified - p
void InitializeMotionRuntime(MotionWork &work, std::int32_t entityType) {
    MotionRuntimeState &state = work.runtime;

    const std::uint32_t rigType = MotionRigForEntityType(entityType);
    InitializeMotionRig(work, rigType);
    work.rig.lowerBody.clock.playbackSpeed = 1.0f;
    work.rig.chest.clock.playbackSpeed = 1.0f;
    work.rig.head.clock.playbackSpeed = 1.0f;
    work.rig.rightArm.clock.playbackSpeed = 1.0f;
    work.rig.leftArm.clock.playbackSpeed = 1.0f;
    InitializeMotionTargetOverrides(work);
    InitializeMotionRotationPointers(work);

    state.projectionCotangentNear = cot_fixed16(DEG_TO_ANG(49.998779296875f));
    state.projectionCotangentMiddle = cot_fixed16(DEG_TO_ANG(29.9981689453125f));
    state.projectionCotangentFar = -cot_fixed16(DEG_TO_ANG(19.9951171875f));
    state.motnFlags |= 1U;
    state.enableEnvironmentMotion = 1;
    state.pelvisYawClamp = DEG_TO_ANG(54.99755859375f);
    state.pelvisYawStep = 384;
    state.entityRotation.y = 0x8000;
    state.overrideBlendTime = -1.0f;
    state.gaitSplitFraction = -1.0f;
    state.transitionTimer = -1.0f;
    state.insideRectangleScale = 1.0f;
    state.channelFlags = 9;
    for (std::size_t region = 0; region < 5; ++region) {
        state.layerLoopStartTime[region] = -1.0f;
        state.pendingLayerStartFrame[region] = -1.0f;
        state.layerEndFrame[region] = -1.0f;
        state.layerSampleFrame[region] = -1.0f;
        state.previousMotionId[region] = 0;
        state.currentMotionId[region] = 0;
        state.queuedMotionId[region] = 0;
        state.regionStepFrames[region] = 8;
        state.layerBlendAlpha[region] = 1.0f;
    }
    for (std::size_t index = 0; index < 9; ++index) {
        state.constraintStepLimit[index] = 0.050000001f;
        if (index == 2)
            state.constraintStepLimit[index] = 0.061359234f;
    }
}

constexpr std::size_t motionRegionCount = 5;

void SubmitPendingFootstepCue(Character &character, MotionRuntimeState &state) {
    const std::uint32_t cue = state.pendingFootstepCue;
    if (!cue)
        return;

    const std::uint32_t material = state.materialFlags;
    const std::uint32_t encoded = (cue & 0x0087ffffU) | ((((material & 0x7fU) << 9U) | (static_cast<std::uint32_t>(state.environmentFlags) & 0xf0U)) << 15U);

    // MTWK_Callback retains the encoded request in +0x0c8 until the next
    // applyRootTranslation pass clears it. Signed material chooses whether
    // PlayFootstepForNode receives the character or the non-spatial null node.
    state.pendingFootstepCue = encoded;
    Character *const node = (material & 0x80U) ? &character : nullptr;
    (void)PlayFootstepForNode(node, encoded);
}

constexpr std::size_t motionConstraintCount = static_cast<std::size_t>(MotionConstraintSlot::Count);
constexpr std::uint32_t constraintStateMask = 7U;
constexpr std::uint32_t constraintTransitionMask = 6U;

std::uint32_t GetConstraintState(const MotionRuntimeState &state, std::size_t slot) {
    return (state.constraintStateBits >> (slot * 3U)) & constraintStateMask;
}




struct CopyMotionContext {
    Character &character;
    MotionWork &work;
    bool hadMotion;

    MotionRuntimeState &state() {
        return work.runtime;
    }
    Vector3 position() const {
        return GetCharacterWorldPosition(&character);
    }
    std::int16_t yaw() const {
        return static_cast<std::int16_t>(GetCharacterWorldRotation(&character).y);
    }
};

std::int16_t CopyYawDifference(std::int32_t a, std::int32_t b) {
    return static_cast<std::int16_t>(a - b);
}

float CopySine(std::int32_t yaw) {
    return sin_fixed16(static_cast<std::int16_t>(yaw));
}

float CopyCosine(std::int32_t yaw) {
    return sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw))));
}

float CopyDistance(const Vector3 &a, const Vector3 &b) {
    const float x = a.x - b.x, z = a.z - b.z;
    return std::sqrt(x * x + z * z);
}

// The yaw facing from `from` toward `to`, without the coincident-point guard CopyDirection adds.
std::int16_t CopyHeading(const Vector3 &from, const Vector3 &to) {
    return static_cast<std::int16_t>(FixedAtan2(from.x - to.x, from.z - to.z));
}

std::int16_t CopyDirection(const Vector3 &from, const Vector3 &to, std::int16_t zeroYaw) {
    return CopyDistance(from, to) <= 0.00000011920929f ? zeroYaw : CopyHeading(from, to);
}

void CopyMoveForward(Vector3 &position, std::int16_t yaw, float distance) {
    position.x -= CopySine(yaw) * distance;
    position.z -= CopyCosine(yaw) * distance;
}

std::uint16_t CopyGaitMotion(const MotionRuntimeState &state) {
    return state.extraMotionIds[(state.copyFlags & MCF_AlternateLocomotion) ? 4 : 1];
}

float CopyStride(const MotionWork &work) {
    return GetMotionRigScale(work.rig.boneId, 8, 0) * ((work.runtime.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
}


std::uint8_t CopyScaledDelay(const MotionRuntimeState &state, float frames) {
    return static_cast<std::uint8_t>(1.0f / state.motionScale * frames);
}

void CopySetState(MotionRuntimeState &state, std::uint8_t operation, std::uint8_t phase) {
    state.copyOperationState = operation;
    state.copyPhase = phase;
}

void CopySetLeadFoot(MotionWork &work, std::uint16_t motion, bool splitWindow) {
    MotionRuntimeState &state = work.runtime;
    state.motnFlags |= 0x8000U;
    if (splitWindow)
        state.gaitSplitFraction = 0.64999998f;
    if (state.copyFlags & MCF_LeadFoot) {
        state.motnFlags &= ~0x10000U;
        if (motion)
            SetMotionRegionEvalTime(&work, static_cast<float>(GetMotionFrameCount(motion)) * 0.5f, MotionRegion::LowerBody);
    } else {
        state.motnFlags |= 0x10000U;
    }
}

// CopyPlayAt: destination is an animation anchor, not a final solved position.
// The normal MTWK request/clock/SolveBase pass publishes the resulting root.
// @verified - p
void CopyPlayAt(CopyMotionContext &context, const Vector3 &position, std::int16_t yaw, std::uint16_t motion, std::uint32_t mode, std::uint32_t frames) {
    shenApplyTranslationToAll(&context.character.entity.node, position);
    SetCharacterWorldRotation(&context.character, {0, yaw, 0});
    mtwkRequestMotion(&context.work, motion, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(&context.work, mode);
    SetMotionFrameTime(&context.work, static_cast<std::uint32_t>(static_cast<std::int32_t>(1.0f / context.state().motionScale * static_cast<float>(frames))),
        MotionRegion::LowerBody);
    SetMotionSpeed(&context.work, context.state().motionScale, MotionRegion::LowerBody);
}

void CopyReleaseLook(MotionWork &work) {
    if (!(work.runtime.copyFlags & MCF_LookSuppressed))
        return;
    UpdateMotionWorkWithPosition(&work, nullptr, 0);
    UpdateMotionWorkWithPosition(&work, nullptr, 0x304);
}

// mtwkCommitCopyCurrentTransition restores the lead foot, root mode and step frames saved when the copy began.
// @verified - p
void CommitCopyMotionTransition(MotionWork &work) {
    MotionRuntimeState &state = work.runtime;
    state.motnFlags = (state.motnFlags & ~0x18000U) | ((state.copyFlags & (MCF_Unknown01 | MCF_Unknown02)) << 15U);
    state.requestedRootMode = state.copyRegionSelector;
    state.defaultRegionStepFrames[0] = state.transitionFrameTime;
    state.copyScaleCountdown = state.copyStepsRemaining = state.copyStepState = 0;
    state.insideRectangleScale = 1.0f;
    CopyReleaseLook(work);
    state.copyFlags &= MCF_Persistent;
    state.seqFlags &= ~MSF_PositionTransition;
    state.copyLayerStartFrame = 0.0f;
    state.copyOperationState = state.copyPhase = 0;
    state.copyDelayFrames = 0;
}

// @verified - p
void CopyAdvanceScale(MotionWork &work) {
    MotionRuntimeState &state = work.runtime;
    const float scale = static_cast<float>(--state.copyScaleCountdown) * 0.125f * (1.0f - state.insideRectangleScale) + state.insideRectangleScale;
    if (scale < 1.0f) {
        SetMotionRigScale(&work, scale);
        SetMotionSpeed(&work, (1.4f - scale * 0.40000001f) * state.motionScale, MotionRegion::LowerBody);
    }
}


// @verified - p
bool CopyReachedStepWindow(MotionWork &work, std::uint16_t motion) {
    MotionRuntimeState &state = work.runtime;
    const std::uint16_t regionFlags = state.regionFlags[0];
    if (regionFlags & MRF_Transition) {
        const bool secondFoot = (state.motnFlags & 0x8000U) ? (state.motnFlags & 0x10000U) != 0 : (state.copyFlags & MCF_LeadFoot) != 0;
        state.copyFlags = (state.copyFlags & ~MCF_SampledLeadFoot) | (secondFoot ? MCF_SampledLeadFoot : 0U);
        return GetMotionTransitionFramesRemaining(&work, MotionRegion::LowerBody) < ((state.copyFlags & MCF_Running) ? 1.1f : 2.0999999f);
    }
    if (!(regionFlags & MRF_Loop) && (regionFlags & MRF_Unknown02))
        return true;
    const float frame = GetMotionFrame(&work, MotionRegion::LowerBody);
    const float speed = GetMotionSpeed(&work, MotionRegion::LowerBody);
    const int frames = static_cast<int>(GetMotionFrameCount(motion));
    const int margin = (state.copyFlags & MCF_Running) ? 2 : 3;
    const float halfWindow = static_cast<float>(frames / 2 - margin);
    const float end = static_cast<float>(frames) - 1.0f;
    const bool outside = frame < end * 0.5f * 0.5f || end * 0.5f * 1.5f <= frame;
    const bool secondFoot = outside != ((regionFlags & MRF_Mirrored) != 0);
    state.copyFlags = (state.copyFlags & ~MCF_SampledLeadFoot) | (secondFoot ? MCF_SampledLeadFoot : 0U);
    const float previous = frame - speed;
    const float endWindow = static_cast<float>(frames - margin);
    if (previous > 0.0f)
        return (frame >= halfWindow && halfWindow > previous) || (frame >= endWindow && endWindow > previous);
    return (frame >= halfWindow && halfWindow > 0.0f) || (end >= halfWindow && halfWindow > previous + end) || (frame >= endWindow && endWindow > 0.0f) ||
        (end >= endWindow && endWindow > previous + end);
}


void CopyChooseFinalFoot(MotionRuntimeState &state, std::int16_t yaw) {
    const int travel = CopyYawDifference(state.copyTravelYaw, yaw);
    const int final = CopyYawDifference(state.transitionStartFrame, yaw);
    bool firstFoot;
    if (travel > 0) {
        if (final <= 0)
            firstFoot = travel > DEG_TO_ANG(135.0f) && travel + final > 0;
        else if (travel < DEG_TO_ANG(90.0f))
            firstFoot = travel < final;
        else
            firstFoot = false;
    } else {
        if (final > 0)
            firstFoot = travel >= -DEG_TO_ANG(135.0f) || travel + final >= 0;
        else
            firstFoot = travel <= -DEG_TO_ANG(90.0f) || travel <= final;
    }
    state.copyFlags = (state.copyFlags & ~MCF_LeadFoot) | (firstFoot ? MCF_LeadFoot : 0U);
}



// @verified - p
std::uint8_t CopyRequestLook(CopyMotionContext &context, const Vector3 &destination, std::int16_t targetYaw) {
    MotionRuntimeState &state = context.state();
    const Vector3 location = context.position();
    const std::int32_t rotation = GetCharacterWorldRotation(&context.character).y;
    Vector3 target = destination;
    std::int16_t difference = 0;
    const auto lookAlong = [&](std::int16_t facing) {
        target = location;
        CopyMoveForward(target, facing, 10.0f);
        difference = CopyYawDifference(facing, rotation);
    };
    switch (state.transitionSubstate) {
    case 0: {
        const float distance = CopyDistance(destination, location);
        if (GetMotionRigScale(context.work.rig.boneId, 8, 0) * 0.55000001f > distance) {
            std::int16_t facing = targetYaw;
            if (distance > 0.30000001f) {
                const std::int16_t direction = CopyHeading(location, destination);
                const int delta = direction - targetYaw;
                if (std::abs(delta) <= DEG_TO_ANG(157.5f))
                    facing = static_cast<std::int16_t>(targetYaw + delta / 2);
                else if (std::abs(direction - rotation) < std::abs(targetYaw - rotation))
                    facing = direction;
            }
            lookAlong(facing);
        } else {
            difference = CopyYawDifference(CopyHeading(location, destination), rotation);
        }
        break;
    }
    case 1:
        if (CopyDistance(destination, location) < 0.1f)
            return 0;
        difference = CopyYawDifference(CopyHeading(location, destination), rotation);
        break;
    case 2:
        lookAlong(targetYaw);
        break;
    default:
        break;
    }
    if (static_cast<std::uint16_t>(difference + DEG_TO_ANG(157.5f)) > 0xe000U)
        return 0;
    UpdateMotionWorkWithPosition(&context.work, &target, 0x501);
    state.motnFlags |= 0x40000U;
    state.copyFlags |= MCF_LookSuppressed;
    return static_cast<std::uint8_t>(std::abs(static_cast<int>(difference)) >> 11U);
}

// @verified - p
bool CopyUpdateLook(CopyMotionContext &context, bool pending) {
    MotionRuntimeState &state = context.state();
    if (pending) {
        if (!(state.seqFlags & MSF_LookAt) || (state.copyFlags & MCF_LookSuppressed)) {
            Vector3 destination = state.copyDestinationPosition;
            if (state.copyFlags & MCF_Unknown20000) {
                const Vector3 location = context.position();
                destination.x += location.x;
                destination.y += location.y;
                destination.z += location.z;
            }
            CopyRequestLook(context, destination, static_cast<std::int16_t>(state.transitionEndFrame));
        }
    } else if (!(state.seqFlags & MSF_LookAt) && !state.copyDelayFrames) {
        state.copyDelayFrames = CopyRequestLook(context, state.copySourcePosition, static_cast<std::int16_t>(state.transitionStartFrame));
        return state.copyDelayFrames == 0;
    } else if (state.copyDelayFrames) {
        --state.copyDelayFrames;
        return false;
    }
    return true;
}

void CopyLatchDestination(CopyMotionContext &context, bool clearRequest) {
    MotionRuntimeState &state = context.state();
    state.transitionStartFrame = state.transitionEndFrame;
    state.copySourcePosition = state.copyDestinationPosition;
    if (state.copyFlags & MCF_Unknown20000) {
        const Vector3 position = context.position();
        state.copySourcePosition.x += position.x;
        state.copySourcePosition.y += position.y;
        state.copySourcePosition.z += position.z;
    }
    if (clearRequest)
        state.copyFlags &= ~MCF_LookPending;
}

// CopyPrepareSpline lays out the five control points of the curved approach, or marks the status
// failed (1) when the source is too close or too far off the travel heading.
// @verified - p
void CopyPrepareSpline(MotionRuntimeState &state, const Vector3 &position, bool preserveYaw, float stride) {
    const Vector3 target = state.copySourcePosition;
    const float x = target.x - position.x, z = target.z - position.z;
    if (std::fabs(x) < 0.001f && std::fabs(z) < 0.001f) {
        state.copySplineStatus = 1;
        return;
    }
    const std::int16_t angle =
        preserveYaw ? CopyYawDifference(FixedAtan2(-x, -z), state.copyTravelYaw) : CopyYawDifference(state.transitionStartFrame, state.copyTravelYaw);
    if (static_cast<std::uint32_t>(std::abs(static_cast<int>(angle)) - 256) > 0x4618U) {
        state.copySplineStatus = 1;
        return;
    }
    state.copySplineStatus = 0;
    const float distance = CopyDistance(position, target);
    float length = distance;
    if (preserveYaw) {
        length = distance * CopyCosine(angle);
        if (length > 0.89999998f)
            length = 0.89999998f;
        else if (length < 0.5f) {
            state.copySplineStatus = 1;
            return;
        }
    } else if (distance < 0.5f) {
        state.copySplineStatus = 1;
        return;
    }
    Vector3(&points)[5] = state.copySplinePoints;
    const float sine = CopySine(state.copyTravelYaw) * length, cosine = CopyCosine(state.copyTravelYaw) * length;
    points[0] = {sine + position.x, position.y, cosine + position.z};
    points[1] = position;
    points[2] = {position.x - sine, position.y, position.z - cosine};
    std::int16_t finalYaw = static_cast<std::int16_t>(state.transitionStartFrame);
    float lastLength = std::fmin(0.89999998f, distance);
    if (preserveYaw) {
        const float lastX = target.x - points[2].x, lastZ = target.z - points[2].z;
        if (std::fabs(lastX) < 0.001f && std::fabs(lastZ) < 0.001f) {
            state.copySplineStatus = 1;
            return;
        }
        finalYaw = static_cast<std::int16_t>(FixedAtan2(-lastX, -lastZ));
        lastLength = CopyDistance(points[2], target) - stride;
        if (lastLength < 0.5f) {
            state.copySplineStatus = 1;
            return;
        }
    }
    points[3] = points[2];
    CopyMoveForward(points[3], finalYaw, lastLength);
    points[4] = points[3];
    CopyMoveForward(points[4], finalYaw, lastLength);
}

// CopyBuildSpline heads along the current yaw (preserveYaw) or straight at the source, then samples
// the spline's first window, dropping the spline flags when the layout fails.
// @verified - p
bool CopyBuildSpline(CopyMotionContext &context, bool preserveYaw) {
    MotionRuntimeState &state = context.state();
    if (!(state.copyFlags & MCF_ExtraStepRequested))
        return false;
    const Vector3 position = context.position();
    const std::int16_t yaw = context.yaw();
    state.copyTravelYaw = preserveYaw ? yaw : CopyDirection(position, state.copySourcePosition, yaw);
    CopyPrepareSpline(state, position, preserveYaw, CopyStride(context.work));
    if (state.copySplineStatus == 1U) {
        state.copyFlags &= ~(MCF_ExtraStepRequested | MCF_ExtraStepComplete);
    } else {
        state.copySplineFrame = 4;
        InterpolateNpcSplineNormalized(state.copySplinePoints, 5, 4, &state.copySplinePosition);
        state.copyFlags |= MCF_ExtraStepComplete;
    }
    return true;
}

// CopyAdjustStride chooses a whole number of alternating steps from the gait motion's last-frame
// root travel, then scales the next motion, never the target.
// @verified - p
void CopyAdjustStride(CopyMotionContext &context, const Vector3 &position, std::int16_t yaw, std::uint16_t motion, float stride) {
    MotionRuntimeState &state = context.state();
    if ((state.copyFlags & MCF_Unknown40000) || (state.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == 0xc00U)
        return;
    const Vector3 offset{state.copySourcePosition.x - position.x, 0.0f, state.copySourcePosition.z - position.z};
    const float distance = Vector3::CalcMagnitude(offset);
    Vector3 rootTravel;
    SampleNativeMotionRoot(motion, context.work.rig.boneId, -1.0f, rootTravel);
    const float travel = std::fabs(rootTravel.z);
    bool directional;
    std::int16_t heading = 0;
    switch (state.transitionClass) {
    case 0:
    case 2:
        directional = false;
        break;
    case 1:
        if (!(state.copyFlags & MCF_Unknown80))
            return;
        heading = CopyYawDifference(state.transitionStartFrame, yaw);
        if (std::abs(static_cast<int>(heading)) < DEG_TO_ANG(22.5f))
            return;
        directional = true;
        break;
    default:
        return;
    }
    const auto chooseFoot = [&] {
        if (directional) {
            state.copyTravelYaw = static_cast<std::int16_t>(state.transitionStartFrame);
            state.copyFlags = heading <= 0 ? state.copyFlags | MCF_LeadFoot : state.copyFlags & ~MCF_LeadFoot;
        } else {
            state.copyTravelYaw = static_cast<std::int16_t>(FixedAtan2(-offset.x, -offset.z));
            CopyChooseFinalFoot(state, yaw);
        }
    };
    const bool wide = std::abs(static_cast<int>(heading)) >= DEG_TO_ANG(45.0f);
    if (state.copyOperationState == 5U) {
        const float approach = directional ? (wide ? stride * 0.60000002f * 0.60000002f : 0.0f) : stride;
        chooseFoot();
        if (travel + travel + approach <= distance || distance <= travel * 1.5f + approach)
            return;
        const float adjustedDistance = distance - (directional ? approach : stride * 0.5f);
        const int frames = static_cast<int>(GetMotionFrameCount(motion));
        const bool feetMatch = ((state.copyFlags & MCF_LeadFoot) != 0) == ((state.copyFlags & MCF_SampledLeadFoot) != 0);
        const bool fiveSteps = directional ? feetMatch : !feetMatch;
        state.copyStepsRemaining = fiveSteps ? 5U : 4U;
        const int totalFrames = fiveSteps ? static_cast<int>(static_cast<float>(frames) * 2.5f) : frames * 2;
        state.copyScaleCountdown = 8;
        state.copyStepState = 1;
        state.insideRectangleScale = (adjustedDistance / (travel / static_cast<float>(frames)) - 4.0f) / static_cast<float>(totalFrames - 4);
        return;
    }
    if (stride * 0.80000001f > distance) {
        state.copyScaleCountdown = 0;
        state.copyStepState = 0;
        return;
    }
    const float approach = directional ? (wide ? stride * 0.25f : 0.0f) : stride;
    if (travel + travel + approach <= distance)
        return;
    const float halfTravel = travel * 0.5f;
    const float adjustedDistance = distance - (directional ? approach : stride * 0.5f);
    const bool oldFoot = (state.copyFlags & MCF_LeadFoot) != 0;
    const int minimumSteps = static_cast<int>(adjustedDistance / halfTravel) + 1;
    chooseFoot();
    const bool feetMatch = oldFoot == ((state.copyFlags & MCF_LeadFoot) != 0);
    const bool increase = directional ? ((minimumSteps & 1) != 0) == feetMatch : ((minimumSteps & 1) != 0) != feetMatch;
    const int steps = increase ? static_cast<int>(adjustedDistance / halfTravel) + 2 : minimumSteps;
    state.copyStepsRemaining = static_cast<std::uint8_t>(steps);
    state.copyScaleCountdown = 0;
    state.copyStepState = 1;
    float scale = adjustedDistance / (static_cast<float>(steps) * halfTravel);
    if (scale < 0.69999999f && stride > 0.0f) {
        const int alternateSteps = increase ? minimumSteps : static_cast<int>(adjustedDistance / halfTravel) + 2;
        const float alternateScale = adjustedDistance / (static_cast<float>(alternateSteps) * halfTravel);
        if (alternateScale > scale) {
            scale = alternateScale;
            state.copyStepsRemaining = static_cast<std::uint8_t>(alternateSteps);
            state.copyExtraStepDistance = 0.0f;
            state.copyScaleCountdown = 0;
            state.copyStepState = 2;
        }
    }
    state.insideRectangleScale = scale;
    if (scale >= 1.0f)
        return;
    SetMotionRigScale(&context.work, scale);
    SetMotionSpeed(&context.work, (1.4f - state.insideRectangleScale * 0.40000001f) * state.motionScale, MotionRegion::LowerBody);
}

// CopyBeginStep handles the initial step, including the intermediate 112.5-degree turn.
// @verified - p
bool CopyBeginStep(CopyMotionContext &context, bool directional) {
    MotionRuntimeState &state = context.state();
    Vector3 position = context.position();
    const std::int16_t yaw = context.yaw();
    CopyReleaseLook(context.work);
    const float nearStride = GetMotionRigScale(context.work.rig.boneId, 8, 0) * 0.55000001f;
    Vector3 delta{state.copySourcePosition.x - position.x, 0.0f, state.copySourcePosition.z - position.z};
    float distance;
    if (directional) {
        distance = nearStride + nearStride;
        delta = {-(CopySine(state.copyTravelYaw) * distance), 0.0f, -(CopyCosine(state.copyTravelYaw) * distance)};
    } else {
        distance = CopyDistance(position, state.copySourcePosition);
        state.copyTravelYaw = CopyDirection(position, state.copySourcePosition, yaw);
    }
    if (distance <= nearStride && !directional) {
        CopyChooseFinalFoot(state, yaw);
        state.copyDelayFrames = 0;
        CopySetState(state, 2, 1);
        return true;
    }
    const float stride = CopyStride(context.work);
    const bool running = (state.copyFlags & MCF_Running) != 0;
    const std::uint16_t motion = CopyGaitMotion(state);
    const std::int16_t turn = CopyYawDifference(state.copyTravelYaw, yaw);
    state.copyFlags = (state.copyFlags & ~MCF_LeadFoot) | (turn <= 0 ? MCF_LeadFoot : 0U);
    const int angle = std::abs(static_cast<int>(turn));
    const std::uint32_t frames = running ? 8U : 16U;
    if (angle >= 20480) {
        const int direction = state.copyFlags & MCF_LeadFoot ? -1 : 1;
        if (context.hadMotion)
            CopyMoveForward(position, static_cast<std::int16_t>(state.copyTravelYaw + direction * DEG_TO_ANG(22.5f)), stride * 0.5f);
        CopySetLeadFoot(context.work, motion, running);
        CopyPlayAt(context, position, static_cast<std::int16_t>(yaw + direction * 20480), motion, 1, frames);
        CopySetState(state, 4, 5);
        state.copyDelayFrames = CopyScaledDelay(state, static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f));
    } else {
        CopySetLeadFoot(context.work, motion, running);
        if (context.hadMotion) {
            const float advance = (1.0f - static_cast<float>(angle) * 0.30000001f / 20480.0f) * nearStride;
            position.x += delta.x / distance * advance;
            position.z += delta.z / distance * advance;
        }
        CopyPlayAt(context, position, state.copyTravelYaw, motion, 1, frames);
        if (!directional)
            CopyAdjustStride(context, position, state.copyTravelYaw, motion, stride);
        CopySetState(state, 5, 1);
    }
    return false;
}

// CopyFinishStep publishes the final XZ/yaw anchor and optional slot-five motion. CopyActive
// remains set until the actual tracked lower-body motion finishes.
// @verified - p
bool CopyFinishStep(CopyMotionContext &context, bool continueWalking) {
    MotionRuntimeState &state = context.state();
    if (state.copyPhase == 3U) {
        if (!(state.regionFlags[0] & MRF_Transition)) {
            state.copyOperationState = 6;
            return true;
        }
        return false;
    }
    if (state.copyDelayFrames) {
        --state.copyDelayFrames;
        return false;
    }
    Vector3 position = context.position();
    const std::int16_t yaw = context.yaw();
    std::int16_t destinationYaw = static_cast<std::int16_t>(state.transitionStartFrame);
    if (state.copyPhase == 2U) {
        state.copyFlags ^= MCF_LeadFoot;
        state.motnFlags = (state.motnFlags | 0x8000U) & ~0x10000U;
        if (!(state.copyFlags & MCF_LeadFoot))
            state.motnFlags |= 0x10000U;
    } else {
        state.copyScaleCountdown = state.copyStepsRemaining = state.copyStepState = 0;
        state.insideRectangleScale = 1.0f;
        CopyReleaseLook(context.work);
        const std::int16_t difference = CopyYawDifference(destinationYaw, yaw);
        bool intermediate = false;
        state.motnFlags = (state.motnFlags | 0x8000U) & ~0x10000U;
        if (state.copyFlags & MCF_LeadFoot) {
            if (difference > DEG_TO_ANG(45.0f)) {
                destinationYaw = static_cast<std::int16_t>(yaw + DEG_TO_ANG(45.0f));
                intermediate = true;
            } else if (difference < -DEG_TO_ANG(135.0f)) {
                destinationYaw = static_cast<std::int16_t>(yaw - DEG_TO_ANG(135.0f));
                intermediate = true;
            }
        } else {
            state.motnFlags |= 0x10000U;
            if (difference > DEG_TO_ANG(135.0f)) {
                destinationYaw = static_cast<std::int16_t>(yaw + DEG_TO_ANG(135.0f));
                intermediate = true;
            } else if (difference < -DEG_TO_ANG(45.0f)) {
                destinationYaw = static_cast<std::int16_t>(yaw - DEG_TO_ANG(45.0f));
                intermediate = true;
            }
        }
        if (intermediate) {
            const float factor = (state.copyFlags & MCF_Running) ? 0.69999999f : 1.0f;
            state.copyDelayFrames = static_cast<std::uint8_t>(factor * 0.69999999f * 16.0f);
        }
        state.copyDelayFrames = CopyScaledDelay(state, state.copyDelayFrames);
    }
    position.x = state.copySourcePosition.x;
    position.z = state.copySourcePosition.z;
    if (state.copyDelayFrames) {
        CopyPlayAt(context, position, destinationYaw, state.extraMotionIds[0], 1, 16);
        state.copyPhase = 2;
        return false;
    }
    if (continueWalking) {
        state.copyTravelYaw = static_cast<std::int16_t>(state.transitionStartFrame);
        const std::uint16_t motion = CopyGaitMotion(state);
        CopySetLeadFoot(context.work, motion, (state.copyFlags & MCF_Running) != 0);
        CopyPlayAt(context, position, state.copyTravelYaw, motion, 1, (state.copyFlags & MCF_Running) ? 8 : 16);
        CopySetState(state, 5, 1);
    } else {
        std::uint16_t motion = state.extraMotionIds[0];
        if (state.copyFlags & MCF_Unknown1000) {
            const bool leadFoot = state.copyFlags & MCF_Unknown2000 ? (state.copyFlags & MCF_LeadFoot) != 0 : (state.copyFlags & MCF_Unknown80000) != 0;
            if (leadFoot)
                state.regionFlags[0] |= MRF_LeadFoot;
            else if (state.copyFlags & MCF_Unknown2000)
                state.regionFlags[0] &= ~MRF_LeadFoot;
            motion = state.extraMotionIds[5];
        }
        CopyPlayAt(context, position, destinationYaw, motion, 1, 16);
        if (state.copyFlags & MCF_Unknown1000) {
            if (state.copyLayerStartFrame > 0.0f)
                state.pendingLayerStartFrame[0] = state.copyLayerStartFrame;
            state.copyFlags |= MCF_Unknown400000;
        }
        state.copyPhase = 3;
    }
    return false;
}



// @verified - p
bool CopyTurnStep(CopyMotionContext &context, bool towardPosition) {
    MotionRuntimeState &state = context.state();
    Vector3 position = context.position();
    std::int16_t yaw = context.yaw();
    if (state.copyPhase != 2U && state.copyPhase != 3U && state.copyPhase != 4U && state.copyPhase != 5U) {
        state.copyScaleCountdown = state.copyStepsRemaining = state.copyStepState = 0;
        if (towardPosition)
            state.copyTravelYaw = CopyDirection(position, state.copySourcePosition, yaw);
        if (std::abs(static_cast<int>(CopyYawDifference(state.copyTravelYaw, yaw))) < DEG_TO_ANG(22.5f) && state.insideRectangleScale >= 1.0f) {
            const std::uint8_t gait = ClassifyCurrentLocomotionPhase(context.work);
            const bool requestedRun = (state.copyFlags & MCF_AlternateLocomotion) != 0;
            if (gait && ((gait == 2U) == requestedRun)) {
                state.copyPhase = 2;
                state.copyDelayFrames = CopyScaledDelay(state, (state.copyFlags & MCF_Running) ? 3.0f : 6.0f);
            } else
                state.copyPhase = 3;
        } else
            state.copyPhase = 3;
    }
    if (state.copyPhase == 2U) {
        CopyReleaseLook(context.work);
        if (state.copyDelayFrames) {
            Vector3i rotation = GetCharacterWorldRotation(&context.character);
            rotation.y += CopyYawDifference(state.copyTravelYaw, rotation.y) / state.copyDelayFrames;
            SetCharacterWorldRotation(&context.character, rotation);
            --state.copyDelayFrames;
        } else
            CopySetState(state, 5, 1);
        return false;
    }
    if ((state.copyPhase == 4U || state.copyPhase == 5U) && state.copyDelayFrames) {
        --state.copyDelayFrames;
        return false;
    }
    const bool running = (state.copyFlags & MCF_Running) != 0;
    const std::uint32_t frames = state.copyFlags & (MCF_Running | MCF_AlternateLocomotion) ? MCF_SampledLeadFoot : 16U;
    const float stride = CopyStride(context.work);
    const std::uint16_t motion = CopyGaitMotion(state);
    if (state.copyPhase == 4U || state.copyPhase == 5U) {
        const std::int16_t oldDifference = CopyYawDifference(state.copyTravelYaw, yaw);
        state.copyFlags ^= MCF_LeadFoot;
        if (towardPosition)
            state.copyTravelYaw = CopyDirection(position, state.copySourcePosition, yaw);
        CopySetLeadFoot(context.work, motion, running);
        if (state.copyPhase == 4U) {
            const int direction = state.copyFlags & MCF_LeadFoot ? 1 : -1;
            if (context.hadMotion)
                CopyMoveForward(position, static_cast<std::int16_t>(state.copyTravelYaw + direction * 6144), stride * 0.2f);
            CopyPlayAt(context, position, static_cast<std::int16_t>(yaw + direction * DEG_TO_ANG(45.0f)), motion, 1, frames);
            state.copyPhase = 5;
            state.copyDelayFrames = CopyScaledDelay(state, static_cast<std::uint8_t>(static_cast<float>(frames) * 0.30000001f));
            return false;
        }
        if (context.hadMotion)
            CopyMoveForward(position, state.copyTravelYaw, (1.0f - std::abs(static_cast<int>(oldDifference)) * 0.30000001f / 24576.0f) * stride);
    } else {
        state.copyFlags = (state.copyFlags & ~MCF_LeadFoot) | (CopyYawDifference(state.copyTravelYaw, yaw) <= 0 ? MCF_LeadFoot : 0U);
        const std::int16_t difference = CopyYawDifference(state.copyTravelYaw, yaw);
        const int angle = std::abs(static_cast<int>(difference));
        const bool feetMatch = ((state.copyFlags & MCF_LeadFoot) != 0) == ((state.copyFlags & MCF_SampledLeadFoot) != 0);
        if (!feetMatch)
            state.copyFlags ^= MCF_LeadFoot;
        CopySetLeadFoot(context.work, motion, running);
        if (!feetMatch && angle >= DEG_TO_ANG(45.0f)) {
            if (context.hadMotion)
                CopyMoveForward(position, yaw, stride * 0.60000002f);
            CopyPlayAt(context, position, static_cast<std::int16_t>(yaw + ((state.copyFlags & MCF_LeadFoot) ? DEG_TO_ANG(45.0f) : -DEG_TO_ANG(45.0f))), motion,
                1, frames);
            state.copyPhase = 5;
            state.copyDelayFrames = CopyScaledDelay(state, static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f));
            return false;
        }
        if (feetMatch && angle >= 20480) {
            if (running) {
                if (context.hadMotion)
                    CopyMoveForward(position, yaw, stride * 0.5f);
                CopyPlayAt(context, position, yaw, motion, 1, frames);
                state.copyPhase = 4;
            } else {
                const int direction = state.copyFlags & MCF_LeadFoot ? -1 : 1;
                if (context.hadMotion)
                    CopyMoveForward(position, static_cast<std::int16_t>(state.copyTravelYaw + direction * DEG_TO_ANG(22.5f)), stride * 0.5f);
                CopyPlayAt(context, position, static_cast<std::int16_t>(yaw + direction * 20480), motion, 1, frames);
                state.copyPhase = 5;
            }
            state.copyDelayFrames = CopyScaledDelay(state, static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f));
            return false;
        }
        if (context.hadMotion)
            CopyMoveForward(position, state.copyTravelYaw, (1.0f - angle * 0.30000001f / (feetMatch ? 20480.0f : 8192.0f)) * stride);
    }
    CopyPlayAt(context, position, state.copyTravelYaw, motion, 1, frames);
    if (towardPosition)
        CopyAdjustStride(context, position, state.copyTravelYaw, motion, stride);
    CopyReleaseLook(context.work);
    CopySetState(state, 5, 1);
    return false;
}

// CopyTranslateStep performs the translated step which restores the original heading.
// @verified - p
bool CopyTranslateStep(CopyMotionContext &context) {
    MotionRuntimeState &state = context.state();
    if ((state.copyPhase == 2U || state.copyPhase == 3U) && state.copyDelayFrames) {
        --state.copyDelayFrames;
        return false;
    }
    Vector3 position = context.position();
    const std::int16_t yaw = context.yaw();
    Vector3 delta{state.copySourcePosition.x - position.x, 0.0f, state.copySourcePosition.z - position.z};
    const float distance = CopyDistance(position, state.copySourcePosition);
    const bool running = (state.copyFlags & MCF_Running) != 0;
    const float stride = CopyStride(context.work);
    const std::uint32_t frames = running ? 8U : 16U;
    if (state.copyPhase == 2U) {
        state.copyFlags ^= MCF_LeadFoot;
        const std::uint16_t motion = state.extraMotionIds[1];
        CopySetLeadFoot(context.work, motion, false);
        const float fraction = distance <= stride * 2.0f ? 0.5f : stride / distance;
        position.x += delta.x * fraction;
        position.z += delta.z * fraction;
        state.copyFlags |= MCF_TurnStepPending;
        state.copyDelayFrames = CopyScaledDelay(state, static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f));
        CopyPlayAt(context, position, state.copyTravelYaw, motion, 1, frames);
        state.copyPhase = 3;
        return false;
    }
    if (state.copyPhase == 3U) {
        state.copyFlags ^= MCF_LeadFoot;
        const std::uint16_t motion = CopyGaitMotion(state);
        CopySetLeadFoot(context.work, motion, running);
        if (distance > stride) {
            delta.x *= stride / distance;
            delta.z *= stride / distance;
        }
        if (state.copyFlags & MCF_TurnStepPending)
            CopyMoveForward(delta, static_cast<std::int16_t>(state.transitionStartFrame), 0.2f);
        const float moved = std::sqrt(delta.x * delta.x + delta.z * delta.z);
        const float fraction = moved <= stride ? 1.0f : stride / moved;
        position.x += delta.x * fraction;
        position.z += delta.z * fraction;
        CopyPlayAt(context, position, static_cast<std::int16_t>(state.transitionStartFrame), motion, 1, frames);
        CopyReleaseLook(context.work);
        state.copyFlags &= ~MCF_TurnStepPending;
        CopySetState(state, 5, 1);
        return false;
    }
    state.transitionStartFrame = static_cast<std::uint16_t>(yaw);
    state.copyTravelYaw = CopyDirection(position, state.copySourcePosition, yaw);
    state.copyFlags = (state.copyFlags & ~MCF_LeadFoot) | (CopyYawDifference(state.copyTravelYaw, yaw) <= 0 ? MCF_LeadFoot : 0U);
    const bool feetMatch = ((state.copyFlags & MCF_LeadFoot) != 0) == ((state.copyFlags & MCF_SampledLeadFoot) != 0);
    const bool facing = static_cast<std::uint16_t>(CopyYawDifference(state.copyTravelYaw, yaw) + 20480) <= 0xa000U;
    std::int16_t nextYaw = yaw;
    if (feetMatch) {
        std::uint16_t motion = CopyGaitMotion(state);
        CopySetLeadFoot(context.work, motion, running);
        if (facing && distance > stride) {
            const float fraction = distance <= stride * 2.0f ? 0.5f : stride / distance;
            position.x += delta.x * fraction;
            position.z += delta.z * fraction;
            nextYaw = state.copyTravelYaw;
            motion = state.extraMotionIds[1];
            state.copyFlags |= MCF_TurnStepPending;
            state.copyPhase = 3;
            state.copyDelayFrames = CopyScaledDelay(state, static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f));
        } else {
            const float fraction = distance <= stride ? 1.0f : stride / distance;
            position.x += delta.x * fraction;
            position.z += delta.z * fraction;
            CopySetState(state, 5, 1);
        }
        CopyPlayAt(context, position, nextYaw, motion, 1, frames);
        CopyReleaseLook(context.work);
    } else {
        state.copyFlags ^= MCF_LeadFoot;
        const std::uint16_t motion = state.extraMotionIds[1];
        CopySetLeadFoot(context.work, motion, false);
        if (facing && distance > stride) {
            CopyMoveForward(position, yaw, 0.2f);
            nextYaw = state.copyTravelYaw;
            state.copyPhase = 2;
        } else
            state.copyPhase = 3;
        CopyPlayAt(context, position, nextYaw, motion, 1, 16);
        state.copyDelayFrames = CopyScaledDelay(state, 9.0f);
    }
    return false;
}


Vector3 CopyLocalOffset(const Vector3 &offset, std::int32_t yaw) {
    Vector3 local;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::TransformPoint(offset, local);
    Matrix4x4::Pop(1);
    return local;
}

// CopyContinueStep's running transition consumes real solved frame crossings.
// A recheck may run twice on the same frame after a target replacement;
// clocks and SolveMotionLowerBody are still advanced only once by MTWK.
// @verified - p
bool CopyContinueStep(CopyMotionContext &context, bool recheck, bool stopAtYaw) {
    MotionRuntimeState &state = context.state();
    if (state.copyPhase != 2U && state.copyPhase != 3U && state.copyPhase != 4U) {
        if (state.regionFlags[0] & MRF_Transition) {
            if (!(state.motnFlags & 0x8000U))
                GetParentBoneWrapper(&context.work,
                    (state.copyFlags & MCF_LeadFoot) ? (MB_RightFootTarget | motionTargetIgnoreX | motionTargetIgnoreY) :
                                                       (MB_LeftFootTarget | motionTargetIgnoreX | motionTargetIgnoreY));
        } else {
            state.copyPhase = (state.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == 0xc00U ? 3U : 2U;
        }
    }
    if (state.copyPhase != 4U) {
        if (state.copyPhase == 2U && state.copyScaleCountdown)
            CopyAdvanceScale(context.work);
        if (state.copyPhase == 3U) {
            const Vector3 position = context.position();
            Vector3i rotation = GetCharacterWorldRotation(&context.character);
            const Vector3 offset{state.copySplinePosition.x - position.x, 0.0f, state.copySplinePosition.z - position.z};
            if (CopyLocalOffset(offset, rotation.y).z > 0.050000001f) {
                state.copySplineFrame = static_cast<std::int16_t>(state.copySplineFrame + ((state.copyFlags & MCF_Running) ? 4 : 2));
                if (state.copySplineFrame > 123) {
                    state.copyPhase = 2;
                    state.copyFlags &= ~MCF_ExtraStepRequested;
                } else {
                    const Vector3 previous = state.copySplinePosition;
                    InterpolateNpcSplineNormalized(state.copySplinePoints, 5, state.copySplineFrame, &state.copySplinePosition);
                    const float x = state.copySplinePosition.x - previous.x, z = state.copySplinePosition.z - previous.z;
                    if (std::fabs(x) >= 0.001f || std::fabs(z) >= 0.001f) {
                        rotation.y = FixedAtan2(-x, -z);
                        SetCharacterWorldRotation(&context.character, rotation);
                    }
                }
            }
        }
        if (!CopyReachedStepWindow(context.work, state.extraMotionIds[(state.copyFlags & MCF_Running) ? 4 : 1]))
            return false;
        state.copyPhase = 4;
        return true;
    }
    Vector3 position = context.position();
    const std::int32_t rotationY = GetCharacterWorldRotation(&context.character).y;
    const std::int16_t yaw = static_cast<std::int16_t>(rotationY);
    const Vector3 offset{state.copySourcePosition.x - position.x, 0.0f, state.copySourcePosition.z - position.z};
    const float distance = CopyDistance(position, state.copySourcePosition);
    const float stride = CopyStride(context.work);
    state.copyPhase = 1;
    if (state.copyStepState == 2U) {
        state.copyFlags = (state.copyFlags & ~MCF_LeadFoot) | ((state.copyFlags & MCF_SampledLeadFoot) ? MCF_LeadFoot : 0U);
        const std::uint16_t motion = CopyGaitMotion(state);
        CopySetLeadFoot(context.work, motion, (state.copyFlags & MCF_Running) != 0);
        CopyMoveForward(position, yaw, state.copyExtraStepDistance);
        const std::uint32_t frames = static_cast<std::uint32_t>(((state.copyFlags & MCF_Running) ? 8.0f : 16.0f) * 0.69999999f);
        CopyPlayAt(context, position, yaw, motion, 1, frames);
        if (state.insideRectangleScale < 1.0f) {
            SetMotionRigScale(&context.work, state.insideRectangleScale);
            SetMotionSpeed(&context.work, (1.4f - state.insideRectangleScale * 0.40000001f) * state.motionScale, MotionRegion::LowerBody);
        }
        state.copyStepState = 1;
        return false;
    }
    if (!state.copyStepState)
        CopyAdjustStride(context, position, yaw, state.currentMotionId[0], stride);
    else if (state.copyStepsRemaining && !(state.regionFlags[0] & MRF_Transition))
        --state.copyStepsRemaining;
    if ((state.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == 0xc00U)
        return false;
    if (stride > distance && !state.copyStepsRemaining) {
        if (!(state.seqFlags & MSF_LookAt) && !(state.copyFlags & MCF_LookSuppressed))
            CopyUpdateLook(context, false);
        if (stopAtYaw) {
            state.copyTravelYaw = static_cast<std::int16_t>(state.transitionStartFrame);
            CopySetState(state, 4, 1);
            return true;
        }
        state.copyTravelYaw = CopyDirection(position, state.copySourcePosition, yaw);
        CopyChooseFinalFoot(state, yaw);
        const bool feetMatch = ((state.copyFlags & MCF_LeadFoot) != 0) == ((state.copyFlags & MCF_SampledLeadFoot) != 0);
        if (!feetMatch) {
            state.copyFlags ^= MCF_LeadFoot;
            state.motnFlags = (state.motnFlags | 0x8000U) & ~0x10000U;
            if (!(state.copyFlags & MCF_LeadFoot))
                state.motnFlags |= 0x10000U;
            position.x = (position.x + state.copySourcePosition.x) * 0.5f;
            position.z = (position.z + state.copySourcePosition.z) * 0.5f;
            const std::uint32_t frames = state.copyFlags & MCF_Running ? MCF_SampledLeadFoot : 16U;
            CopyPlayAt(context, position, yaw, state.extraMotionIds[0], 1, frames);
            state.copyDelayFrames = CopyScaledDelay(state, static_cast<std::uint8_t>(frames - 1U));
            state.copyFlags ^= MCF_LeadFoot;
        } else
            state.copyDelayFrames = 0;
        CopySetState(state, 2, 1);
        return true;
    }
    if (!recheck && CopyLocalOffset(offset, rotationY).z > 0.2f) {
        if (stopAtYaw) {
            state.copyTravelYaw = static_cast<std::int16_t>(state.transitionStartFrame);
            CopySetState(state, 4, 1);
        } else {
            state.copyFlags = (state.copyFlags & ~MCF_LeadFoot) | ((state.copyFlags & MCF_SampledLeadFoot) ? MCF_LeadFoot : 0U);
            state.motnFlags = (state.motnFlags | 0x8000U) & ~0x10000U;
            if (!(state.copyFlags & MCF_LeadFoot))
                state.motnFlags |= 0x10000U;
            CopyPlayAt(context, position, yaw, state.extraMotionIds[0], 1, state.copyFlags & MCF_Running ? 8 : 16);
            state.copyDelayFrames = 0;
            CopySetState(state, 2, 3);
        }
        return true;
    }
    return false;
}

// CopyCanStart: a stop request (0x10) ends the copy once the requested yaw and/or position is reached.
// @verified - p
bool CopyCanStart(CopyMotionContext &context) {
    const MotionRuntimeState &state = context.state();
    if (!(state.copyFlags & 0x10U))
        return true;
    const Vector3 position = context.position();
    const bool yawReached = std::abs(static_cast<int>(CopyYawDifference(state.transitionStartFrame, context.yaw()))) < 182;
    const bool positionReached = CopyDistance(position, state.copySourcePosition) < 0.0049999999f;
    const std::uint8_t substate = state.transitionSubstate;
    if (substate > 2U || !(substate == 2U ? yawReached : substate == 1U ? positionReached : yawReached && positionReached))
        return true;
    CommitCopyMotionTransition(context.work);
    return false;
}

// After a finished walk, the copy restarts at state 1 unless the walker stands within 0.1 of
// the source or a stop was requested; a missed source leaves 0x20 set for operation 3.
// @verified - p
bool CopyRetryUnlessArrived(CopyMotionContext &context) {
    MotionRuntimeState &state = context.state();
    if (!(state.copyFlags & MCF_LookPending)) {
        if (CopyDistance(context.position(), state.copySourcePosition) <= 0.1f)
            return false;
        state.copyFlags |= MCF_Persistent;
        if (state.copyFlags & 0x10U)
            return false;
    }
    state.copyOperationState = 1;
    return true;
}

// Class 0 (walk to a position, optionally along a spline) and class 2 (turn in place). Returns true
// to have the dispatcher run the class the request switched to.
// @verified - p
bool UpdateCopyToPosition(CopyMotionContext &context) {
    MotionRuntimeState &state = context.state();
    for (;;) {
        bool next = true;
        switch (state.copyOperationState) {
        case 0:
        case 1: {
            if (state.transitionVariant != state.transitionClass) {
                state.transitionClass = state.transitionVariant;
                return true;
            }
            if (state.copyFlags & MCF_LookPending) {
                CopyLatchDestination(context, true);
                state.copyDelayFrames = 0;
            }
            if (!(state.copyFlags & (MCF_Unknown1000 | MCF_Unknown400000)) && (!CopyCanStart(context) || !CopyUpdateLook(context, false)))
                return false;
            if (state.transitionSubstate == 1U)
                state.transitionStartFrame = static_cast<std::uint16_t>(context.yaw());
            else if (state.transitionSubstate == 2U)
                state.copySourcePosition = context.position();
            else if (!state.transitionSubstate)
                CopyBuildSpline(context, true);
            const bool spline = (state.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == 0xc00U;
            if (spline)
                state.copyTravelYaw = context.yaw();
            next = CopyBeginStep(context, spline);
            break;
        }
        case 2:
            next = CopyFinishStep(context, false);
            break;
        case 4:
            next = CopyTurnStep(context, true);
            break;
        case 5: {
            if (state.transitionVariant != state.transitionClass) {
                state.transitionClass = state.transitionVariant;
                return true;
            }
            if (!CopyContinueStep(context, false, false))
                return false;
            const bool pending = (state.copyFlags & MCF_LookPending) != 0;
            if (pending)
                CopyLatchDestination(context, false);
            if (state.transitionSubstate == 1U) {
                state.transitionStartFrame = static_cast<std::uint16_t>(context.yaw());
            } else if (state.transitionSubstate == 2U) {
                state.copySourcePosition = context.position();
                state.copyStepsRemaining = 0;
            } else if (!state.transitionSubstate && pending) {
                CopyBuildSpline(context, true);
            }
            if (!pending) {
                next = CopyContinueStep(context, false, false);
                break;
            }
            state.copyFlags &= ~MCF_LookPending;
            if ((state.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == 0xc00U) {
                state.copyPhase = 1;
                return false;
            }
            if (CopyContinueStep(context, true, false))
                break;
            state.copyTravelYaw = CopyHeading(context.position(), state.copySourcePosition);
            CopySetState(state, 4, 1);
            break;
        }
        default:
            if (!CopyRetryUnlessArrived(context)) {
                CommitCopyMotionTransition(context.work);
                return false;
            }
            break;
        }
        if (!next)
            return false;
    }
}

// Class 1: walk along the requested heading (0x80 set while the heading still steers the walk).
// @verified - p
bool UpdateCopyAlongHeading(CopyMotionContext &context) {
    MotionRuntimeState &state = context.state();
    const auto latch = [&] {
        CopyLatchDestination(context, true);
        state.copyFlags = state.transitionSubstate ? state.copyFlags & ~MCF_Unknown80 : state.copyFlags | MCF_Unknown80;
    };
    for (;;) {
        bool next = true;
        switch (state.copyOperationState) {
        case 0:
        case 1:
            if (state.transitionVariant != state.transitionClass) {
                state.transitionClass = state.transitionVariant;
                return true;
            }
            if (state.copyFlags & MCF_LookPending)
                latch();
            if (!CopyUpdateLook(context, false))
                return false;
            if (state.transitionSubstate == 1U) {
                state.copyTravelYaw = CopyHeading(context.position(), state.copySourcePosition);
                next = CopyBeginStep(context, true);
            } else if (state.transitionSubstate == 2U) {
                state.copyTravelYaw = static_cast<std::int16_t>(state.transitionStartFrame);
                next = CopyBeginStep(context, true);
            } else if (!state.transitionSubstate) {
                CopyBuildSpline(context, false);
                next = CopyBeginStep(context, false);
            }
            break;
        case 2:
            next = CopyFinishStep(context, true);
            if (state.copyOperationState != 2U)
                state.copyFlags &= ~MCF_Unknown80;
            break;
        case 4:
            next = CopyTurnStep(context, (state.copyFlags & MCF_Unknown80) != 0);
            break;
        case 5:
            if (state.transitionVariant != state.transitionClass) {
                state.transitionClass = state.transitionVariant;
                return true;
            }
            if (!(state.copyFlags & MCF_Unknown80) && !(state.copyFlags & MCF_LookPending) && !(state.regionFlags[0] & MRF_Transition)) {
                state.copyOperationState = 6;
                break;
            }
            if (!CopyContinueStep(context, false, true))
                return false;
            if (state.copyFlags & MCF_LookPending) {
                latch();
                if (state.transitionSubstate == 1U) {
                    state.copyTravelYaw = CopyHeading(context.position(), state.copySourcePosition);
                    CopySetState(state, 4, 1);
                } else if (state.transitionSubstate == 2U) {
                    state.copyTravelYaw = static_cast<std::int16_t>(state.transitionStartFrame);
                    CopySetState(state, 4, 1);
                } else if (!state.transitionSubstate) {
                    CopyBuildSpline(context, false);
                    if (CopyContinueStep(context, true, false))
                        break;
                    state.copyTravelYaw = CopyHeading(context.position(), state.copySourcePosition);
                    CopySetState(state, 4, 1);
                }
                break;
            }
            if (!(state.copyFlags & MCF_Unknown80)) {
                state.copyPhase = 1;
                return false;
            }
            next = CopyContinueStep(context, false, true);
            if (next)
                state.copyFlags &= ~MCF_Unknown80;
            if ((state.copyFlags & MCF_ExtraStepComplete) && !(state.copyFlags & MCF_ExtraStepRequested)) {
                next = true;
                state.copyFlags &= ~MCF_Unknown80;
            }
            break;
        default:
            CommitCopyMotionTransition(context.work);
            return false;
        }
        if (!next)
            return false;
    }
}

// Class 3: a translated side step to a nearby position, keeping the facing.
// @verified - p
bool UpdateCopySideStep(CopyMotionContext &context) {
    MotionRuntimeState &state = context.state();
    const auto restart = [&] {
        switch (state.transitionSubstate) {
        case 0:
            state.copyTravelYaw = CopyHeading(context.position(), state.copySourcePosition);
            return true;
        case 1:
            CopySetState(state, 3, 1);
            return false;
        case 2:
            state.copyTravelYaw = static_cast<std::int16_t>(state.transitionStartFrame);
            return true;
        default:
            return false;
        }
    };
    for (;;) {
        bool next = true;
        switch (state.copyOperationState) {
        case 0:
        case 1:
            if (state.transitionVariant != state.transitionClass) {
                state.transitionClass = state.transitionVariant;
                return true;
            }
            if (state.copyFlags & MCF_LookPending)
                CopyLatchDestination(context, true);
            if (!CopyUpdateLook(context, false))
                return false;
            if (restart())
                next = CopyBeginStep(context, true);
            break;
        case 3:
            next = CopyTranslateStep(context);
            break;
        case 4:
            next = CopyTurnStep(context, false);
            break;
        case 5:
            if (state.transitionVariant != state.transitionClass) {
                state.transitionClass = state.transitionVariant;
                return true;
            }
            if (!(state.copyFlags & MCF_LookPending) && !(state.regionFlags[0] & MRF_Transition)) {
                state.copyOperationState = 6;
                break;
            }
            if (!CopyContinueStep(context, false, true))
                return false;
            if (!(state.copyFlags & MCF_LookPending)) {
                state.copyPhase = 1;
                return false;
            }
            CopyLatchDestination(context, true);
            if (restart())
                CopySetState(state, 4, 1);
            break;
        default:
            CommitCopyMotionTransition(context.work);
            return false;
        }
        if (!next)
            return false;
    }
}

// mtwkUpdateCopyCurrentTransition runs before root-position synchronization and queued-layer
// promotion. An external lower-body request owns cancellation precedence.
// @verified - p
void UpdateCopyMotionTransition(Character &character, MotionWork &work, bool hadMotion) {
    MotionRuntimeState &state = work.runtime;
    if (state.queuedMotionId[0]) {
        CommitCopyMotionTransition(work);
        return;
    }
    CopyMotionContext context{character, work, hadMotion};
    if (state.pendingLayerStartFrame[0] > 0.0f)
        state.copyLayerStartFrame = state.pendingLayerStartFrame[0];
    state.pendingLayerStartFrame[0] = -1.0f;
    if (state.regionFlags[0] & MRF_LeadFoot) {
        state.copyFlags |= MCF_Unknown80000;
        state.regionFlags[0] &= ~MRF_LeadFoot;
    }
    state.layerLoopStartTime[0] = state.layerEndFrame[0] = -1.0f;
    state.requestedRootYawOffset = 0;
    state.channelFlags = 9;
    for (std::size_t controller : {0U, 5U, 6U}) {
        state.activeConstraintTarget[controller] = nullptr;
        state.latchedConstraintTarget[controller] = nullptr;
    }
    const std::uint8_t gait = ClassifyCurrentLocomotionPhase(work);
    if (gait == 2U)
        state.copyFlags |= MCF_Running;
    else if (gait == 1U)
        state.copyFlags &= ~MCF_Running;
    if (state.copyOperationState == 5U && !(state.copyFlags & MCF_LookPending) &&
        ((gait == 2U && !(state.copyFlags & MCF_AlternateLocomotion)) || (gait == 1U && (state.copyFlags & MCF_AlternateLocomotion)))) {
        state.copyDestinationPosition = state.copySourcePosition;
        state.transitionEndFrame = state.transitionStartFrame;
        state.copyFlags |= MCF_LookPending;
    }
    if ((state.copyFlags & MCF_Unknown10000) && !(state.copyFlags & MCF_Unknown20000)) {
        const Vector3 position = context.position();
        state.copyDestinationPosition.x -= position.x;
        state.copyDestinationPosition.y -= position.y;
        state.copyDestinationPosition.z -= position.z;
        state.copyFlags |= MCF_Unknown20000;
    }
    state.copyFlags &= ~MCF_Persistent;
    if (state.copyFlags & MCF_LookPending)
        CopyUpdateLook(context, true);
    for (;;) {
        bool next = true;
        switch (state.transitionClass) {
        case 0:
        case 2:
            next = UpdateCopyToPosition(context);
            break;
        case 1:
            next = UpdateCopyAlongHeading(context);
            break;
        case 3:
            next = UpdateCopySideStep(context);
            break;
        default:
            break;
        }
        if (!next)
            return;
    }
}

// The look transition latches a copy of a controller target; the solver's active target is dropped.
// @verified - p
bool LatchMotionControllerTarget(MotionWork &work, std::uint32_t controller, const Vector3 *point) {
    MotionRuntimeState &state = work.runtime;
    state.activeConstraintTarget[controller] = nullptr;
    if (!point) {
        state.latchedConstraintTarget[controller] = nullptr;
        return false;
    }
    state.constraintBlendSource[controller] = *point;
    state.latchedConstraintTarget[controller] = &state.constraintBlendSource[controller];
    return true;
}

// UpdateCopyLookTransition owns the look request's saved flags and transient controller
// submissions. The solver, not this state machine, owns the yaw/blend result.
// @verified - p
void UpdateCopyLookTransition(MotionWork &work) {
    MotionRuntimeState &state = work.runtime;
    std::uint16_t &flags = state.locomotionMotionSet;
    const bool starting = state.transitionState == 1U;
    if (state.transitionState == 1U) {
        if (state.motnFlags & 0x40U)
            flags |= 1U;
        if (state.seqFlags & MSF_LimitConstraintStep)
            flags |= 2U;
        state.motnFlags |= 0x40U;
        const auto angle = [](float cotangent) {
            const auto fixed = static_cast<std::int16_t>((std::atan(std::fabs(cotangent)) * 65536.0f / 6.2831855f) + 0.5f);
            const auto expanded = static_cast<std::uint16_t>(DEG_TO_ANG(90.0f) - static_cast<int>(static_cast<float>(DEG_TO_ANG(90.0f) - fixed) * 0.7f));
            const float result = std::tan(static_cast<float>(expanded) * 6.2831855f * 0.000015258789f);
            return cotangent < 0.0f ? -result : result;
        };
        state.extraMotionPosition = {angle(state.projectionCotangentNear), angle(state.projectionCotangentMiddle), angle(state.projectionCotangentFar)};
        state.extraMotionMask = 0;
        state.transitionState = 2;
    }
    if (state.transitionState == 2U) {
        if (flags & 0x10U) {
            state.seqFlags &= ~MSF_LimitConstraintStep;
            state.lookAtBlendTime = 1.0f;
        } else
            state.seqFlags |= MSF_LimitConstraintStep;
        if (!starting && !(state.seqFlags & MSF_TargetFacing)) {
            if (!(flags & 4U))
                state.pelvisYawTarget = 0;
            else if (state.extraMotionMask & 8U)
                state.pelvisYawTarget = std::min(static_cast<std::int16_t>(state.pelvisYawCurrent + state.lookAtYawStep), state.pelvisYawClamp);
            else if (state.extraMotionMask & 2U)
                state.pelvisYawTarget =
                    std::max(static_cast<std::int16_t>(state.pelvisYawCurrent + state.lookAtYawStep), static_cast<std::int16_t>(-state.pelvisYawClamp));
            else
                state.pelvisYawTarget = (state.extraMotionMask & 0xfU) ? state.pelvisYawCurrent : 0;
        }
        LatchMotionControllerTarget(work, 2, &state.lookAtTarget);
    } else if (state.transitionState == 3U) {
        if (flags & 0x10U) {
            state.seqFlags &= ~MSF_LimitConstraintStep;
            state.lookAtBlendTime = 0.0f;
            state.pelvisYawCurrent = state.pelvisYawHalf = 0;
        } else
            state.seqFlags |= MSF_LimitConstraintStep;
        if (!(state.seqFlags & MSF_TargetFacing))
            state.pelvisYawTarget = 0;
        if (!(state.constraintStateBits & 0x1c0U) && ((state.seqFlags & MSF_TargetFacing) || !state.lookAtYaw) && state.extraBlendTime == 0.0f) {
            if (!(flags & 1U))
                state.motnFlags &= ~0x40U;
            if (flags & 2U)
                state.seqFlags |= MSF_LimitConstraintStep;
            else
                state.seqFlags &= ~MSF_LimitConstraintStep;
            state.transitionState = state.extraMotionSpeed = 0;
            state.seqFlags &= ~MSF_LookAt;
            flags &= 0xfe13U;
        }
    }
    if (flags & 0xa0U)
        LatchMotionControllerTarget(work, 3, &state.leftControllerTarget);
    if (flags & 0x140U)
        LatchMotionControllerTarget(work, 4, &state.rightControllerTarget);
    flags &= ~0x10U;
}

constexpr std::uint16_t targetFacingMotions[5][20] = {
    {0xf095,0xf091,0xf096,0xf092,0xf08c,0xf08a,0xf08e,0xf123,0xf127,0xf125,0xf094,0xf090,0xf093,0xf08f,0xf08b,0xf089,0xf08d,0xf122,0xf126,0xf124},
    {0xf04e,0xf049,0xf04f,0xf04a,0xf044,0xf042,0xf046,0xf123,0xf127,0xf125,0xf04d,0xf048,0xf04c,0xf047,0xf043,0xf041,0xf045,0xf122,0xf126,0xf124},
    {0xf06f,0xf06b,0xf070,0xf06c,0xf066,0xf064,0xf068,0xf123,0xf127,0xf125,0xf06e,0xf06a,0xf06d,0xf069,0xf065,0xf063,0xf067,0xf122,0xf126,0xf124},
    {0xf0e0,0xf0dc,0xf0e1,0xf0dd,0xf0d7,0xf0d5,0xf0d9,0xf123,0xf127,0xf125,0xf0df,0xf0db,0xf0de,0xf0da,0xf0d6,0xf0d4,0xf0d8,0xf122,0xf126,0xf124},
    {}};


// @verified - p
void PlayTargetFacingTurn(MotionWork &work, bool moving, bool left, std::int16_t yaw) {
    auto &state = work.runtime;
    int remainder = std::abs(static_cast<int>(yaw));
    int index;
    bool sideDependent = true;
    if (remainder > DEG_TO_ANG(135.0f)) {
        index = 5;
        remainder = 0;
    } else if (state.targetFacingFlags & 0x40U) {
        if (remainder > 12743) {
            index = 9;
            remainder -= DEG_TO_ANG(90.0f);
        } else {
            index = 7;
            sideDependent = false;
        }
    } else if (remainder > DEG_TO_ANG(90.0f)) {
        index = 3;
        remainder -= DEG_TO_ANG(90.0f);
    } else if (remainder > DEG_TO_ANG(45.0f)) {
        index = 1;
        remainder -= DEG_TO_ANG(45.0f);
    } else {
        index = 6;
        sideDependent = false;
    }
    if (sideDependent && ((yaw < 0 && !left) || (yaw > 0 && left)))
        --index;
    state.pelvisYawTarget = static_cast<std::int16_t>(yaw < 0 ? -remainder : remainder);
    std::uint8_t motionClass = state.targetFacingMotionClass;
    if (!motionClass) {
        switch (work.rig.boneId) {
        case 0:
        case 1:
        case 2:
        case 13:
        case 14:
            motionClass = 2;
            break;
        case 3:
        case 4:
        case 5:
            motionClass = 4;
            break;
        case 6:
            motionClass = 3;
            break;
        default:
            motionClass = 1;
            break;
        }
    }
    const auto motion = targetFacingMotions[motionClass - 1U][index + (moving ? 0 : 10)];
    const int frames =
        static_cast<int>(GetMotionFrameCount(motion)) / (moving ? 3 : 4) + std::abs(static_cast<int>(state.pelvisYawTarget) - state.pelvisYawCurrent) / 384;
    state.pelvisYawStep = 384;
    mtwkRequestMotion(&work, motion, MotionRegion::Chest);
    mtwkRequestMotion(&work, motion, MotionRegion::Head);
    SetMotionFrameTime(&work, static_cast<std::uint32_t>(frames), MotionRegion::Chest);
    SetMotionFrameTime(&work, static_cast<std::uint32_t>(frames), MotionRegion::Head);
    const MotionRegion arm = left ? MotionRegion::LeftArm : MotionRegion::RightArm;
    mtwkRequestMotion(&work, motion, arm);
    SetMotionFrameTime(&work, static_cast<std::uint32_t>(frames), arm);
    if (left) {
        state.regionFlags[1] |= MRF_LeadFoot;
        state.regionFlags[2] |= MRF_LeadFoot;
        state.regionFlags[4] |= MRF_LeadFoot;
    }
    mtwkSetRequestedRootMode(&work, 3);
    state.chestRotationOverrideBlendTime = 1.0f / static_cast<float>(frames);
}

// UpdateTargetFacingTransition runs before current-copy and look admission, before Sequence.
// @verified - p
void UpdateTargetFacingTransition(Character &character, MotionWork &work) {
    auto &state = work.runtime;
    if (!(state.seqFlags & MSF_TargetFacing))
        return;
    auto &flags = state.targetFacingFlags;
    const auto setSpeeds = [&](float speed, bool both) {
        work.rig.chest.clock.playbackSpeed = speed;
        work.rig.head.clock.playbackSpeed = speed;
        if (both || (flags & 4U))
            work.rig.leftArm.clock.playbackSpeed = speed;
        if (both || !(flags & 4U))
            work.rig.rightArm.clock.playbackSpeed = speed;
    };
    switch (state.targetFacingState) {
    case 1: {
        const auto position = GetCharacterWorldPosition(&character);
        const auto yaw = static_cast<std::uint16_t>(GetCharacterWorldRotation(&character).y);
        state.targetFacingYaw = FixedAtan2(position.x - state.targetFacingPosition.x, position.z - state.targetFacingPosition.z);
        if (flags & 1U) {
            CopyMotionTransition(&work, nullptr, static_cast<std::uint16_t>(state.targetFacingYaw), 0, 2);
            CopyMotionTransition(&work, nullptr, 0, 2, 0);
            state.targetFacingYawDelta = 0;
        } else if (state.seqFlags & MSF_PositionTransition)
            CopyMotionTransition(&work, nullptr, yaw, 0, 2);
        state.targetFacingState = 2;
        return;
    }
    case 2:
        if ((state.seqFlags & MSF_PositionTransition) && ((static_cast<std::uint16_t>(state.entityRotation.y) - state.targetFacingYaw) & 0x7fffU) >= 0x71cU)
            return;
        state.targetFacingState = 3;
        [[fallthrough]];
    case 3: {
        const auto position = GetCharacterWorldPosition(&character);
        const float distance = CopyDistance(position, state.targetFacingPosition);
        state.targetFacingYaw = FixedAtan2(position.x - state.targetFacingPosition.x, position.z - state.targetFacingPosition.z);
        state.targetFacingYawDelta = CopyYawDifference(state.targetFacingYaw, GetCharacterWorldRotation(&character).y);
        if (!(flags & 2U)) {
            if ((state.targetFacingYawDelta > 0) == (distance <= 1.0f))
                flags &= ~4U;
            else
                flags |= 4U;
        }
        if (distance > 40.0f)
            flags |= 0x40U;
        else
            flags &= ~0x40U;
        if ((flags & 8U) && ((flags & 0x14U) == 0x10U || (!(flags & 0x10U) && (flags & 4U))))
            flags |= 0x20U;
        PlayTargetFacingTurn(work, true, (flags & 4U) != 0, state.targetFacingYawDelta);
        state.targetFacingState = 4;
        return;
    }
    case 4:
        setSpeeds(1.0f, false);
        return;
    case 5:
        PlayTargetFacingTurn(work, false, (flags & 4U) != 0, state.targetFacingYawDelta);
        state.targetFacingState = 6;
        state.pelvisYawTarget = 0;
        return;
    case 6: {
        setSpeeds(1.0f, false);
        const float frame = GetMotionFrame(&work, MotionRegion::Chest);
        const float count = static_cast<float>(GetMotionFrameCount(state.currentMotionId[1]));
        if (frame <= count * 0.5f)
            return;
        const float remaining = count - frame;
        for (std::uint32_t region = 1; region < 5; ++region) {
            if (region >= 3U && !(flags & 0x20U) && region != ((flags & 4U) ? 4U : 3U))
                continue;
            state.regionFlags[region] |= MRF_ReturnToLowerBody;
            SetMotionFrameTime(&work, static_cast<std::uint32_t>(static_cast<int>(remaining)), static_cast<MotionRegion>(region));
        }
        state.targetFacingState = 7;
        state.chestRotationOverrideBlendTime = 1.0f / remaining;
        return;
    }
    case 7:
        if (state.regionFlags[1] & MRF_BlendToLowerBody)
            return;
        setSpeeds(work.rig.lowerBody.clock.playbackSpeed, (flags & 0x20U) != 0);
        state.targetFacingState = 0;
        state.seqFlags &= ~MSF_TargetFacing;
        flags &= ~1U;
        return;
    default:
        return;
    }
}

// @verified - p
void CopyRecursiveModelNodeTransform(MT7Node *node, std::uint32_t selector, Matrix4x4 *output) {
    for (;;) {
        if (node->runtimeSibling)
            Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        if (static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(node->id))) == selector) {
            Matrix4x4::GetTransformMatrix(output);
        }
        if (node->runtimeChild)
            CopyRecursiveModelNodeTransform(node->runtimeChild, selector, output);
        if (!node->runtimeSibling)
            break;
        Matrix4x4::Pop(1);
        node = node->runtimeSibling;
    }
}

// @verified - p
void CopyCharacterSourceTransform(const Character *source, std::uint32_t selector, Matrix4x4 *output) {
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    MT7Model *const model = static_cast<MT7Model *>(source->model);
    if (model) {
        const Vector3 position = GetCharacterWorldPosition(source);
        const Vector3i rotation = GetCharacterWorldRotation(source);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
        Matrix4x4::GetTransformMatrix(output);
        if (!model->nodes.empty())
            CopyRecursiveModelNodeTransform(&model->nodes.front(), selector, output);
    } else {
        Matrix4x4::GetTransformMatrix(output);
    }
    Matrix4x4::Pop(1);
}

// mtwkUpdateTransitionControllers: the copy steps read whether a lower-body motion was playing on entry.
// @verified - p
void UpdateMotionTransitionControllers(Character &character, MotionWork &work) {
    const bool hadMotion = work.runtime.currentMotionId[0] != 0;
    if ((work.runtime.seqFlags & MSF_TargetFacing) && !work.rig.useOverride)
        UpdateTargetFacingTransition(character, work);
    if (work.runtime.seqFlags & MSF_PositionTransition)
        UpdateCopyMotionTransition(character, work, hadMotion);
    if (work.runtime.seqFlags & MSF_LookAt)
        UpdateCopyLookTransition(work);
}

void MotionWorkCleanup(Character *character, void *payload) {
    if (!payload)
        return;
    MotionWork &work = *static_cast<MotionWork *>(payload);
    ReleaseMotionPropTracks(work);
    if (work.runtime.unfo)
        HLib::FreeTaskData(work.runtime.unfo);
    work.runtime.unfo = nullptr;
}

// MTWK_Callback.
void MotionWorkUpdate(Character *character, void *payload) {
    auto &work = *static_cast<MotionWork *>(payload);
    if (work.runtime.drawOverrideData || (work.runtime.callbackFlags & 8U))
        return;
    SetActiveMTWK(&work);
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    scenGetCharaName(control->scenarioTask, character);
    g_mtwkCurrentIsRyo = control->ryo == character;
    ClearSearchHandMotionRequests(&work);

    UpdateMotionTransitionControllers(*character, work);

    const Vector3 position = GetCharacterWorldPosition(character);
    SetMotionRootPosition(&work, position);
    work.runtime.entityRotation = GetCharacterWorldRotation(character);
    if (work.runtime.transformSource) {
        CopyCharacterSourceTransform(static_cast<const Character *>(work.runtime.transformSource),
            static_cast<std::uint32_t>(work.runtime.transformSourceIndex), &work.runtime.overrideViewSpaceTransform);
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&work.runtime.overrideViewSpaceTransform);
        const Vector3 &offset = work.runtime.transformOffsetPosition;
        Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
        const Vector3i &rotation = work.runtime.transformOffsetRotation;
        Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
        Matrix4x4::GetTransformMatrix(&work.runtime.overrideViewSpaceTransform);
        Matrix4x4::Pop(1);
    }

    std::int32_t visible = 0;
    if (character->model) {
        work.runtime.depthScale = GetCharacterDisplayRadius(character);
        visible = mtwkEvaluateMotionFrame(&work);
    }
    EV_SetCharDisplay(character, CVM_MotionworkDisplayCheck, visible != 0);
    figpToggleMotionworkFlag(character, &work);
    if ((work.runtime.callbackFlags & 1U) != 0) {
        Vector4 delta;
        UpdateMotionRootDeltaVector(&work, &delta);
        const Vector3 translation{delta.x, delta.y, delta.z};
        AddCharacterChildSpacePosition(character, &translation);
    }

    SubmitPendingFootstepCue(*character, work.runtime);
    std::uint8_t handCommand = 1;
    for (;;) {
        const MotionRigClock &clock = work.rig.lowerBody.clock;
        const std::span<const std::byte> command = FindLoadedMotionTimedCommand(
            work.runtime.currentMotionId[0], 18, clock.current.ticksTime - clock.playbackSpeed, clock.current.ticksTime, handCommand);
        if (command.empty())
            break;
        ++handCommand;
        const std::int32_t side =
            (work.runtime.regionFlags[0] & MRF_Mirrored) ? std::to_integer<std::int8_t>(command[3]) : 1 - std::to_integer<std::int8_t>(command[3]);
        const std::uint8_t shape = std::to_integer<std::uint8_t>(command[4]);
        const std::int32_t frames = std::to_integer<std::int8_t>(command[6]) + std::to_integer<std::int8_t>(command[7]) * 256;
        hmdlSetPoseFor(character, side, motionAuxiliaryHandPoses[shape][side], frames);
    }
    std::int32_t handShapes[2], handFrames[2];
    mtwkGetArmHandPoseChanges(&work, handShapes, handFrames);
    for (std::int32_t side = 0; side < 2; ++side) {
        if (handShapes[side] == -1)
            continue;
        if (handFrames[side] < 0)
            handFrames[side] = 3;
        hmdlSetPoseFor(character, side, motionArmHandPoses[handShapes[side]][side], handFrames[side]);
    }
    BindMotionControllerMatricesToModelNodes(character, &work, static_cast<MT7Model *>(character->model));
    const Vector3 &root = work.runtime.rootPosition;
    Matrix4x4::TransformPoint(root, character->viewPosition);
    const float distance = Vector3::CalcMagnitude(character->viewPosition);
    character->drawDistance = distance >= 0.0000099999997f ? distance : 0.0f;
    GetSearchHandWristAngles(work, g_characterHandWristAngles);
    SetActiveMTWK(nullptr);
}

} // namespace

void SetSharedPrngSeed(std::uint32_t seed) noexcept {
    sharedPrngSeed = seed;
}

std::uint16_t IncrementSharedPrngUInt16() noexcept {
    sharedPrngSeed = 0x41c64e6dU * sharedPrngSeed + 12345U;
    return static_cast<std::uint16_t>((sharedPrngSeed >> 16U) & 0x7fffU);
}

float IncrementSharedPrngFloat() noexcept {
    return static_cast<float>(static_cast<double>(IncrementSharedPrngUInt16()) * (1.0 / 32768.0));
}

// @verified - p
std::int32_t shenGetRandomInt32(std::int32_t bound) noexcept {
    return static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(bound));
}

// @verified - p
float GetRandomNumberInRange(float bound) noexcept {
    return IncrementSharedPrngFloat() * bound;
}

void SetMotionEnvironmentEnabled(std::int32_t enabled) noexcept {
    g_usingEnv = enabled;
}

// @verified - p
void BindMotionControllerMatricesToModelNodes(Character *, MotionWork *work, MT7Model *model) {
    if ((work->runtime.seqFlags & MSF_ModelMatricesBound) != 0)
        return;
    if (!model) {
        work->runtime.seqFlags |= MSF_ModelMatricesBound;
        return;
    }
    ClearMT7NodeTransformOverrides(model->nodes.data());
    IKRig &rig = work->rig;
    const Matrix4x4 *const matrices[]{&rig.chest.upperChestWorld, &rig.head.neckWorld, &rig.head.headWorld, &rig.rightArm.shoulderWorld,
        &rig.rightArm.upperArmWorld, &rig.rightArm.forearmWorld, &rig.rightArm.handWorld, &rig.leftArm.shoulderWorld, &rig.leftArm.upperArmWorld,
        &rig.leftArm.forearmWorld, &rig.leftArm.handWorld, &rig.lowerBody.hipsWorld, &rig.lowerBody.leftThighWorld, &rig.lowerBody.leftShinWorld,
        &rig.lowerBody.leftAnkleWorld, &rig.lowerBody.rightThighWorld, &rig.lowerBody.rightShinWorld, &rig.lowerBody.rightAnkleWorld};
    for (std::size_t index = 0; index < motionControllerModelNodeIds.size(); ++index)
        if (MT7Node *const node = GetChildObjectByIndex(*model, model->nodes.data(), motionControllerModelNodeIds[index]))
            SetMT7NodeTransformOverride(*node, matrices[index]);
    if (rig.useOverride != 0) {
        const Matrix4x4 *const overrides[]{&rig.lowerBody.pelvisWorld, rig.lowerBody.leftHipWorld, rig.lowerBody.rightHipWorld, rig.lowerBody.rightToe1World,
            rig.lowerBody.rightToe2World, rig.lowerBody.rightToe3World, rig.head.aimSourceWorld, rig.head.nodeAWorld, rig.head.nodeBWorld, rig.head.nodeCWorld};
        for (std::size_t index = 0; index < motionOverrideModelNodeIds.size(); ++index)
            if (MT7Node *const node = GetChildObjectByIndex(*model, model->nodes.data(), motionOverrideModelNodeIds[index]))
                SetMT7NodeTransformOverride(*node, overrides[index]);
    } else {
        const Matrix4x4 *const normal[]{&rig.chest.chestWorld, &rig.rightArm.wristWorld, &rig.leftArm.wristWorld};
        for (std::size_t index = 0; index < motionNormalModelNodeIds.size(); ++index)
            if (MT7Node *const node = GetChildObjectByIndex(*model, model->nodes.data(), motionNormalModelNodeIds[index]))
                SetMT7NodeTransformOverride(*node, normal[index]);
    }
    work->runtime.seqFlags |= MSF_ModelMatricesBound;
}

MotionWork *AllocateMotionWork(Character *character) {
    if (!character)
        return nullptr;
    MotionWork *const work = static_cast<MotionWork *>(AllocateModelComponent(character, motionWorkToken, sizeof(MotionWork), MotionWorkUpdate));
    if (!work)
        return nullptr;
    charaAssignCoreCallback(character, mtwkDrawCharacterCore);
    imgmAssignUpdateCallback(character, mtUpdateTransparencyCallback);
    mtwkAssignDrawCallback(character, DrawShenmueEntity);
    SetModelComponentCleanup(character, motionWorkToken, MotionWorkCleanup);
    std::memset(work, 0, sizeof(*work));
    if (MotionRigForEntityType(GetCharacterImageSourceType(character)) == 16U) {
        work->runtime.unfo = HLib::AllocateTaskData(4000, motionOverrideMemoryToken);
        std::memset(work->runtime.unfo, 0, 4000);
    }
    InitializeMotionRuntime(*work, GetCharacterImageSourceType(character));
    work->runtime.entityRotation = GetCharacterWorldRotation(character);
    return work;
}

MotionWork *FindMotionWork(const Character *character) {
    return static_cast<MotionWork *>(FindModelComponent(character, motionWorkToken));
}

bool mtwkSetSequenceFlags(Character *character) {
    MotionWork *const work = FindMotionWork(character);
    if (!work)
        return false;
    work->runtime.seqFlags &= ~MSF_ModelMatricesBound;
    return true;
}

// @verified - p
std::uint32_t GetCharacterMotionYaw(const Character *character) {
    const MotionWork *const work = FindMotionWork(character);
    if (!work)
        return character ? static_cast<std::uint32_t>(character->entity.node.rotation[1]) : 0;
    std::uint32_t yaw = static_cast<std::uint32_t>(work->runtime.entityRotation.y);
    for (const CharacterNode *node = character->entity.node.child; node; node = node->child)
        yaw -= static_cast<std::uint32_t>(node->rotation[1]);
    return yaw;
}



// @verified - p
float AssignCharacterMotionYaw(Character *character, std::int32_t yaw, const std::uint16_t *customMotions, std::uint32_t flags) {
    static constexpr std::uint16_t turnMotions[]{0xf021, 0xf022, 0xf023, 0xf024, 0xf028, 0xf027, 0xf026};
    const std::uint16_t sector = static_cast<std::uint16_t>(static_cast<std::uint32_t>(yaw) - GetCharacterMotionYaw(character) + 4096U) >> 13;
    std::uint16_t motion;
    if (sector) {
        motion = customMotions ? customMotions[sector - 1] : turnMotions[sector - 1];
        mtwkRequestMotion(FindMotionWork(character), motion, MotionRegion::LowerBody);
    } else {
        motion = (flags & EventTransformFlags::CustomIdleMotion) && customMotions ? customMotions[7] : 0xf003;
    }
    const std::span<const std::byte> data = FindLoadedMotionData(motion);
    std::uint16_t frameCount{};
    if (!data.empty())
        std::memcpy(&frameCount, data.data(), sizeof(frameCount));
    std::uint32_t predictedYaw = GetCharacterMotionYaw(character);
    if (sector)
        predictedYaw = static_cast<std::uint16_t>(predictedYaw + sector * 8192U);
    float degrees = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) - predictedYaw)) / 182.04444885253906f;
    if (degrees >= 180.0f)
        degrees = static_cast<float>(static_cast<double>(degrees) - 360.0);
    else if (degrees <= -180.0f)
        degrees = static_cast<float>(static_cast<double>(degrees) + 360.0);
    return degrees / static_cast<float>(frameCount);
}
bool SetMotionConstraintTarget(MotionWork *motionWork, MotionConstraintSlot slot, const Vector3 &worldTarget) {
    const std::size_t index = static_cast<std::size_t>(slot);
    if (!motionWork || index >= motionConstraintCount)
        return false;
    MotionRuntimeState &state = motionWork->runtime;
    state.constraintBlendSource[index] = worldTarget;
    state.activeConstraintTarget[index] = &state.constraintBlendSource[index];
    return true;
}

void ClearMotionConstraintTarget(MotionWork *motionWork, MotionConstraintSlot slot) {
    const std::size_t index = static_cast<std::size_t>(slot);
    if (!motionWork || index >= motionConstraintCount)
        return;
    motionWork->runtime.activeConstraintTarget[index] = nullptr;
    motionWork->runtime.latchedConstraintTarget[index] = nullptr;
}

void ClearAllMotionConstraintTargets(MotionWork *motionWork) {
    if (!motionWork)
        return;
    MotionRuntimeState &state = motionWork->runtime;
    std::fill(std::begin(state.activeConstraintTarget), std::end(state.activeConstraintTarget), nullptr);
    std::fill(std::begin(state.latchedConstraintTarget), std::end(state.latchedConstraintTarget), nullptr);
    std::fill(std::begin(state.constraintBlendTotalFrames), std::end(state.constraintBlendTotalFrames), 0);
    std::fill(std::begin(state.constraintBlendRemainingFrames), std::end(state.constraintBlendRemainingFrames), 0);
    state.constraintStateBits = 0;
}

bool IsMotionConstraintTargetSettled(const MotionWork *motionWork, MotionConstraintSlot slot) {
    const std::size_t index = static_cast<std::size_t>(slot);
    if (!motionWork || index >= motionConstraintCount) {
        return false;
    }
    return (GetConstraintState(motionWork->runtime, index) & constraintTransitionMask) == 2U && motionWork->runtime.constraintBlendRemainingFrames[index] == 0;
}

bool EvaluateCurrentMotionPose(const MotionWork *motionWork, MotionPose &pose) {
    if (!motionWork)
        return false;
    const MotnSequence *const sequence = FindLoadedMotionSequence(motionWork->runtime.currentMotionId[0]);
    if (!sequence || !sequence->frameCount)
        return false;
    return EvaluateMotionPose(*sequence, std::max(0.0f, GetMotionPoseFrame(motionWork, MotionRegion::LowerBody)) / 30.0f,
        static_cast<std::uint8_t>(motionWork->rig.boneId), pose, motionWork);
}

bool SetMotionDrawOverrideData(MotionWork *motionWork, const Matrix4x4 *frame) {
    if (!motionWork)
        return false;


    motionWork->runtime.drawOverrideData = frame;
    return true;
}

const Matrix4x4 *GetMotionDrawOverrideData(const MotionWork *motionWork) {
    return motionWork ? motionWork->runtime.drawOverrideData : nullptr;
}

// @verified - p
bool SetMotionIkTargetPosition(MotionWork *work, Vector3 *position) {
    if (!work)
        return false;
    work->runtime.ikTargetPosition = position;
    return true;
}

void SetMotionRootTransformOverride(MotionWork *motionWork, const Matrix4x4 *transform) {
    if (motionWork)
        motionWork->runtime.transformStart = transform;
}

bool BuildMotionRootRebaseTransform(const MotionWork *motionWork, Matrix4x4 &transform) {
    if (!motionWork || !motionWork->runtime.transformStart)
        return false;
    const Matrix4x4 *const root = motionWork->runtime.transformStart;
    const Matrix4x4 saved = workTransformMatrix;
    Matrix4x4::SetActiveTransform(&transformMatrix_prePass);
    Matrix4x4::preTranslate(root->rows[3].x, root->rows[3].y, root->rows[3].z);
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::InvertInPlace(&transform);
    Matrix4x4::SetActiveTransform(root);
    Matrix4x4::MultiplyWithPrePassMatrix(&transform);
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::SetActiveTransform(&saved);
    return true;
}

// @verified - p
void RebaseMotionSolvedMatrices(MotionWork *work) {
    (void)Matrix4x4_Push(&transformMatrix_prePass);
    const Matrix4x4 *const root = work->runtime.transformStart;
    Matrix4x4::preTranslate(root->rows[3].x, root->rows[3].y, root->rows[3].z);
    Matrix4x4 inverseTranslation;
    Matrix4x4::GetTransformMatrix(&inverseTranslation);
    Matrix4x4::InvertInPlace(&inverseTranslation);
    Matrix4x4::SetActiveTransform(&inverseTranslation);
    IKRig &rig = work->rig;
    Matrix4x4 *const matrices[]{&rig.lowerBody.pelvisWorld, &rig.lowerBody.hipsWorld, &rig.lowerBody.leftThighWorld, &rig.lowerBody.leftShinWorld,
        &rig.lowerBody.leftAnkleWorld, &rig.lowerBody.rightThighWorld, &rig.lowerBody.rightShinWorld, &rig.lowerBody.rightAnkleWorld,
        &rig.chest.upperChestWorld, &rig.head.neckWorld, &rig.head.headWorld, &rig.rightArm.shoulderWorld, &rig.rightArm.upperArmWorld,
        &rig.rightArm.forearmWorld, &rig.rightArm.handWorld, &rig.leftArm.shoulderWorld, &rig.leftArm.upperArmWorld, &rig.leftArm.forearmWorld,
        &rig.leftArm.handWorld};
    Matrix4x4 *const tail[]{rig.useOverride ? rig.lowerBody.leftHipWorld : &rig.chest.chestWorld,
        rig.useOverride ? rig.lowerBody.rightHipWorld : &rig.rightArm.wristWorld, rig.useOverride ? rig.lowerBody.rightToe1World : &rig.leftArm.wristWorld,
        rig.useOverride ? rig.lowerBody.rightToe2World : &rig.lowerBody.leftFootWorld,
        rig.useOverride ? rig.lowerBody.rightToe3World : &rig.lowerBody.rightFootWorld, rig.head.aimSourceWorld, rig.head.nodeAWorld, rig.head.nodeBWorld,
        rig.head.nodeCWorld};
    for (Matrix4x4 *matrix : matrices) {
        (void)Matrix4x4_Push();
        Matrix4x4::MultiplyWithPrePassMatrix(matrix);
        Matrix4x4::GetTransformMatrix(matrix);
        Matrix4x4::SetActiveTransform(root);
        Matrix4x4::MultiplyWithPrePassMatrix(matrix);
        Matrix4x4::GetTransformMatrix(matrix);
        Matrix4x4::Pop(1);
    }
    const std::size_t count = rig.useOverride ? std::size(tail) : 5U;
    for (std::size_t index = 0; index < count; ++index) {
        Matrix4x4 *const matrix = tail[index];
        (void)Matrix4x4_Push();
        Matrix4x4::MultiplyWithPrePassMatrix(matrix);
        Matrix4x4::GetTransformMatrix(matrix);
        Matrix4x4::SetActiveTransform(root);
        Matrix4x4::MultiplyWithPrePassMatrix(matrix);
        Matrix4x4::GetTransformMatrix(matrix);
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
}

bool mtwkRequestMotion(MotionWork *motionWork, std::uint16_t motionId, MotionRegion region) {
    if (!motionWork || ReturnZero() != 0 || region >= 5)
        return false;
    motionWork->runtime.queuedMotionId[region] = motionId;
    if (region != 0) {
        motionWork->runtime.regionFlags[region] &= static_cast<std::uint16_t>(~0x0800U);
    }
    return true;
}

std::uint16_t GetMotionAnimationId(const MotionWork *motionWork, MotionRegion region) {
    if (!motionWork || region > 5U)
        return 0;



    return region < 5U ? motionWork->runtime.currentMotionId[region] : motionWork->runtime.queuedMotionId[0];
}

// @verified - p
std::int64_t motnIsCurrentAnimOpenEnded(const MotionWork *motionWork, MotionRegion region) {
    if (!motionWork || region > 5U)
        return 0;
    const std::span<const std::byte> command = FindLoadedMotionSecondaryEntry(GetMotionAnimationId(motionWork, region), 1);
    if (command.empty())
        return 0;
    std::uint16_t count;
    std::memcpy(&count, command.data() + 2, sizeof(count));
    return count == 0xffffU;
}

std::uint16_t GetQueuedMotionAnimationId(const MotionWork *motionWork, MotionRegion region) {
    if (!motionWork || region > 5U)
        return 0;


    return region < 5U ? motionWork->runtime.queuedMotionId[region] : motionWork->runtime.blendTotalFrames[0];
}

// @verified - p
bool SampleNativeMotionRootAndRotation(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 *position, Vector3i *rotation) {
    const MotnSequence *const sequence = FindLoadedMotionSequence(animation);
    if (!sequence) {
        if (position)
            *position = {};
        if (rotation)
            *rotation = {};
        return false;
    }
    const float time = (frame >= 0.0f ? frame : static_cast<float>(sequence->frameCount - 1)) * 0.033333335f;
    const float scale = boneId < 0 ? 1.0f : GetMotionRigTranslationScale(*sequence, static_cast<std::uint8_t>(boneId >= 17 ? 8 : boneId));
    float values[6];
    for (std::size_t channel = 0; channel < 6; ++channel)
        values[channel] = SampleMotnScalar(sequence->scalars[channel], time);
    if (position)
        *position = {values[0] * scale, values[1] * scale, values[2] * scale};
    if (rotation) {
        rotation->x = static_cast<std::int32_t>((values[5] * 57.29578f) * 182.04443f);
        rotation->y = static_cast<std::int32_t>((values[4] * 57.29578f) * 182.04443f);
        rotation->z = static_cast<std::int32_t>((values[3] * 57.29578f) * 182.04443f);
    }
    return true;
}

// @verified-s2: 0x7ff6185db3a0 (vfClsampleCurrentMotion).
bool vfClsampleCurrentMotion(MotionWork *work, float frame, Vector3 &output) {
    const std::int32_t boneId = work ? work->rig.boneId : -1;
    auto &state = work->runtime;
    std::uint16_t animation = state.queuedMotionId[0];
    bool mirrored;
    float previousX = 0.0f, previousZ = 0.0f;
    if (animation != 0) {
        if (frame != 0.0f)
            state.pendingLayerStartFrame[0] = std::fmax(0.0f, frame);
        const MotnSequence *const sequence = FindLoadedMotionSequence(animation);
        mirrored = (state.regionFlags[0] & MRF_LeadFoot) != (sequence && sequence->hasLeadFootCommand ? 0x20U : 0U);
    } else {
        float previousFrame = state.layerPreviousTicks[0];
        animation = state.currentMotionId[0];
        if (previousFrame > frame)
            previousFrame = -1.0f;
        state.pendingLayerStartFrame[0] = std::fmax(0.0f, frame);
        mirrored = (state.regionFlags[0] & MRF_Mirrored) != 0;
        if (previousFrame >= 0.0f) {
            Vector3 previous;
            SampleNativeMotionRoot(animation, boneId, previousFrame, previous);
            previousX = mirrored ? -previous.x : previous.x;
            previousZ = previous.z;
        }
    }
    const bool sampled = SampleNativeMotionRoot(animation, boneId, frame, output);
    if (mirrored)
        output.x = -output.x;
    output.x -= previousX;
    output.z -= previousZ;
    return sampled;
}

void RequestMotionRegionDetach(MotionWork *motionWork, MotionRegion region) {
    if (!motionWork)
        return;
    if (!region) {
        for (std::uint32_t secondary = 1; secondary < motionRegionCount; ++secondary) {
            motionWork->runtime.regionFlags[secondary] |= MRF_ReturnToLowerBody;
        }
    } else if (region < motionRegionCount) {
        motionWork->runtime.regionFlags[region] |= MRF_ReturnToLowerBody;
    }
}

// @verified - p
Matrix4x4 *charaGetBoneTransform(MotionWork *motion, std::int32_t boneId) {
    if (!motion)
        return nullptr;
    IKRig &rig = motion->rig;
    switch (boneId) {
    case MB_Pelvis:
        return &rig.lowerBody.pelvisWorld;
    case MB_UpperChest:
        return &rig.chest.upperChestWorld;
    case MB_Neck:
        return &rig.head.neckWorld;
    case MB_Head:
        return &rig.head.headWorld;
    case MB_RightShoulder:
        return &rig.rightArm.shoulderWorld;
    case MB_RightUpperArm:
        return &rig.rightArm.upperArmWorld;
    case MB_RightForearm:
        return &rig.rightArm.forearmWorld;
    case MB_RightHand:
        return &rig.rightArm.handWorld;
    case MB_LeftShoulder:
        return &rig.leftArm.shoulderWorld;
    case MB_LeftUpperArm:
        return &rig.leftArm.upperArmWorld;
    case MB_LeftForearm:
        return &rig.leftArm.forearmWorld;
    case MB_LeftHand:
        return &rig.leftArm.handWorld;
    case MB_Hips:
        return &rig.lowerBody.hipsWorld;
    case MB_LeftThigh:
        return &rig.lowerBody.leftThighWorld;
    case MB_LeftShin:
        return &rig.lowerBody.leftShinWorld;
    case MB_LeftAnkle:
        return &rig.lowerBody.leftAnkleWorld;
    case MB_RightThigh:
        return &rig.lowerBody.rightThighWorld;
    case MB_RightShin:
        return &rig.lowerBody.rightShinWorld;
    case MB_RightAnkle:
        return &rig.lowerBody.rightAnkleWorld;
    default:
        break;
    }
    if (rig.useOverride) {
        switch (boneId) {
        case MB_HeadAimSource:
            return rig.head.aimSourceWorld;
        case MB_HeadOverrideA:
            return rig.head.nodeAWorld;
        case MB_HeadOverrideB:
            return rig.head.nodeBWorld;
        case MB_HeadOverrideC:
            return rig.head.nodeCWorld;
        case MB_LeftHipOverride:
            return rig.lowerBody.leftHipWorld;
        case MB_RightHipOverride:
            return rig.lowerBody.rightHipWorld;
        case MB_RightToe1:
            return rig.lowerBody.rightToe1World;
        case MB_RightToe2:
            return rig.lowerBody.rightToe2World;
        case MB_RightToe3:
            return rig.lowerBody.rightToe3World;
        default:
            return nullptr;
        }
    }
    switch (boneId) {
    case MB_Chest:
        return &rig.chest.chestWorld;
    case MB_RightWrist:
        return &rig.rightArm.wristWorld;
    case MB_LeftWrist:
        return &rig.leftArm.wristWorld;
    case MB_LeftFoot:
        return &rig.lowerBody.leftFootWorld;
    case MB_RightFoot:
        return &rig.lowerBody.rightFootWorld;
    default:
        return nullptr;
    }
}

// @verified - p
bool CopyMotionBoneTransform(MotionWork *motion, std::int32_t boneId, Matrix4x4 *transform) {
    if (!motion || !transform)
        return false;
    const Matrix4x4 *const source = charaGetBoneTransform(motion, boneId);
    if (!source)
        return false;
    *transform = *source;
    return true;
}

// @verified - p
bool GetMotionBonePosition(MotionWork *motion, MotionBoneId boneId, Vector3 *position) {
    if (!motion)
        return false;
    switch (boneId) {
    case MB_ChestTarget:
    case MB_HeadTarget:
    case MB_RightArmTarget:
    case MB_LeftArmTarget:
    case MB_LeftFootTarget:
    case MB_RightFootTarget:
    case MB_HeadAimTarget:
    case MB_PelvisOverrideTarget: {
        const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(boneId));
        return controller != -1 && GetMotionControllerTargetPosition(motion, static_cast<std::uint32_t>(controller), *position);
    }
    case MB_Eyes: {
        Matrix4x4 eye;
        Matrix4x4_PushMatrixAndTranslate(motion->rig.head, &eye);
        *position = {eye.rows[3].x, eye.rows[3].y, eye.rows[3].z};
        return true;
    }
    case MB_RightForearmTarget:
        return GetMotionBonePosition(motion, MB_RightWrist, position);
    case MB_LeftForearmTarget:
        return GetMotionBonePosition(motion, MB_LeftWrist, position);
    default:
        break;
    }
    const Matrix4x4 *const transform = charaGetBoneTransform(motion, boneId);
    if (!transform)
        return false;
    *position = {transform->rows[3].x, transform->rows[3].y, transform->rows[3].z};
    return true;
}

// @verified - p
bool GetIsRHandAboveShoulder(MotionWork *motion) {
    Vector3 root{};
    Vector3 shoulder{};
    Vector3 hand{};
    GetMotionBonePosition(motion, MB_Pelvis, &root);
    GetMotionBonePosition(motion, MB_LeftFootTarget, &shoulder);
    GetMotionBonePosition(motion, MB_RightFootTarget, &hand);
    shoulder.x -= root.x;
    shoulder.z -= root.z;
    hand.x -= root.x;
    hand.z -= root.z;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-motion->runtime.entityRotation.y));
    Matrix4x4::TransformPoint(shoulder, shoulder);
    Matrix4x4::TransformPoint(hand, hand);
    Matrix4x4::Pop(1);
    return hand.z > shoulder.z;
}

// @verified - p
bool GetIsRHandBelowShoulder(MotionWork *motion) {
    Vector3 shoulder{};
    Vector3 hand{};
    GetMotionBonePosition(motion, MB_LeftFootTarget, &shoulder);
    GetMotionBonePosition(motion, MB_RightFootTarget, &hand);
    return shoulder.y > hand.y;
}

// @verified - p
void TransformScenarioMotionBonePose(shen_name character, std::int32_t boneId, Vector3 *position, Vector3i *rotation) {
    Matrix4x4 transform;
    CopyMotionBoneTransform(scenFindMotionwork(character), boneId, &transform);
    TransformCharacterPoseByMatrix(&transform, position, rotation);
}

// @verified - p
void GetCharacterMotionLocalPosition(Character *character, std::int32_t bone, Vector3 *position) {
    MotionWork *const work = FindMotionWork(character);
    if (work && GetMotionBonePosition(work, static_cast<MotionBoneId>(bone), position)) {
        CharacterNode *const child = GetCharacterNode(character)->child;
        if (child)
            TransformPointToCharacterNodeSpace(child, position);
    } else {
        GetCharacterLocalPosition(character, position);
    }
}

// @verified - p
void GetCharacterMotionNodePosition(Character *character, std::int32_t bone, Vector3 *position) {
    MotionWork *const work = FindMotionWork(character);
    if (work && GetMotionBonePosition(work, static_cast<MotionBoneId>(bone), position))
        TransformPointToCharacterNodeSpace(GetCharacterNode(character), position);
    else
        GetCharacterLocalPosition(character, position);
}

// @verified - p
void GetCharacterMotionWorldPosition(Character *character, std::int32_t bone, Vector3 *position) {
    MotionWork *const work = FindMotionWork(character);
    if (!work || !GetMotionBonePosition(work, static_cast<MotionBoneId>(bone), position))
        *position = GetCharacterWorldPosition(character);
}

bool ResolveAuthoredMotionBoneTransform(const Character *character, std::uint16_t boneId, Matrix4x4 &transform) {
    const MotionWork *const motion = FindMotionWork(character);
    if (!motion)
        return false;
    switch (boneId) {
    case 8:
        transform = motion->rig.rightArm.shoulderWorld;
        return true;
    case 9:
        transform = motion->rig.rightArm.upperArmWorld;
        return true;
    case 12:
        transform = motion->rig.rightArm.forearmWorld;
        return true;
    case 14:
        transform = motion->rig.rightArm.handWorld;
        return true;
    case 15:
        transform = motion->rig.leftArm.shoulderWorld;
        return true;
    case 16:
        transform = motion->rig.leftArm.upperArmWorld;
        return true;
    case 19:
        transform = motion->rig.leftArm.forearmWorld;
        return true;
    case 21:
        transform = motion->rig.leftArm.handWorld;
        return true;
    default:
        break;
    }
    if (boneId == 0) {
        transform = motion->rig.lowerBody.pelvisWorld;
        return true;
    }
    std::uint32_t nodeId{};
    switch (boneId) {
    case 3:
        nodeId = motion->rig.useOverride ? 1 : 0;
        break;
    case 4:
        nodeId = 2;
        break;
    case 5:
        nodeId = 70;
        break;
    case 8:
        nodeId = 4;
        break;
    case 9:
        nodeId = 5;
        break;
    case 12:
        nodeId = 6;
        break;
    case 14:
        nodeId = 191;
        break;
    case 15:
        nodeId = 9;
        break;
    case 16:
        nodeId = 10;
        break;
    case 19:
        nodeId = 11;
        break;
    case 21:
        nodeId = 190;
        break;
    case 22:
        nodeId = 14;
        break;
    case 23:
        nodeId = 16;
        break;
    case 24:
        nodeId = 17;
        break;
    case 26:
        nodeId = 18;
        break;
    case 28:
        nodeId = 21;
        break;
    case 29:
        nodeId = 22;
        break;
    case 31:
        nodeId = 23;
        break;
    case 1:
    case 11:
    case 13:
    case 18:
    case 20:
    case 27:
    case 32:
        if (motion->rig.useOverride)
            return false;
        switch (boneId) {
        case 1:
            nodeId = 1;
            break;
        case 11:
        case 13:
            nodeId = 7;
            break;
        case 18:
        case 20:
            nodeId = 12;
            break;
        case 27:
            nodeId = 19;
            break;
        default:
            nodeId = 24;
            break;
        }
        break;
    case 33:
    case 35:
    case 36:
    case 37:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
        if (!motion->rig.useOverride)
            return false;
        switch (boneId) {
        case 33:
            nodeId = 177;
            break;
        case 35:
            nodeId = 58;
            break;
        case 36:
            nodeId = 59;
            break;
        case 37:
            nodeId = 119;
            break;
        case 39:
            nodeId = 15;
            break;
        case 40:
            nodeId = 20;
            break;
        case 41:
            nodeId = 85;
            break;
        case 42:
            nodeId = 86;
            break;
        default:
            nodeId = 87;
            break;
        }
        break;
    default:
        return false;
    }
    return ResolveCharacterDrawNodeTransform(character, nodeId, transform);
}

bool GetCharacterMotionBonePosition(Character *character, std::uint16_t boneId, Vector3 &position) {
    if (!character)
        return false;
    const MotionWork *const motion = FindMotionWork(character);
    if (!motion)
        return false;
    if (boneId == 10 || boneId == 17) {
        const MotionRigTarget &target = boneId == 10 ? motion->rig.rightArm.target : motion->rig.leftArm.target;
        position = {target.axes[2].dimensions[0], target.axes[2].dimensions[1], target.outputZ};
        return true;
    }
    Matrix4x4 transform{};
    if (ResolveAuthoredMotionBoneTransform(character, boneId, transform)) {
        position = {transform.rows[3].x, transform.rows[3].y, transform.rows[3].z};
    } else {
        position = GetCharacterWorldPosition(character);
    }
    return true;
}

// mtwkApplyTransformsForEntityAndBone: the facing of a solved rig matrix.
// @verified - p
std::int16_t GetMotionBoneYaw(MotionWork *motion, std::int32_t bone) {
    return static_cast<std::int16_t>(mtwkApplyTransforms(charaGetBoneTransform(motion, bone)));
}

std::int32_t GetMotionRigBoneId(const MotionWork *motionWork) {
    return motionWork ? static_cast<std::int32_t>(motionWork->rig.boneId) : -1;
}
MotionTimePair *GetMotionLayerTimePair(MotionWork *motionWork, MotionRegion region) {
    MotionRigClock *clock = RigClock(motionWork, region);
    return clock ? &clock->current : nullptr;
}

const MotionTimePair *GetMotionLayerTimePair(const MotionWork *motionWork, MotionRegion region) {
    const MotionRigClock *clock = RigClock(motionWork, region);
    return clock ? &clock->current : nullptr;
}

// @verified - p
std::uint32_t GetMotionTrackedRootMode(const MotionWork *work) {
    for (std::size_t region = 0; region < 5; ++region)
        if (work->runtime.regionFlags[region] & MRF_Transition)
            return work->runtime.currentRootMode;
    return 0;
}

// @verified - p
float GetMotionTransitionFramesRemaining(const MotionWork *work, MotionRegion region) {
    if (!work || !(work->runtime.regionFlags[region] & MRF_Transition))
        return 0.0f;
    const MotionRuntimeState &state = work->runtime;
    if (state.currentRootMode == 2)
        return static_cast<float>(state.blendRemainingFrames[region]);
    if (state.currentRootMode != 1 && static_cast<std::uint8_t>(state.currentRootMode - 3U) > 2U)
        return 0.0f;
    return static_cast<float>(state.blendTotalFrames[region]) - RigClock(work, region)->current.ticksTime;
}

// @verified - p
void CopyMotionRegionTimes(const MotionWork *work, float *split, float *current, float *maximum, float *speed, MotionRegion region) {
    if (!work)
        return;
    const MotionRigClock &clock = *RigClock(work, region);
    if (split)
        *split = clock.current.splitTimeFrames;
    if (current)
        *current = clock.current.ticksTime;
    if (maximum)
        *maximum = clock.current.maxTime;
    if (speed)
        *speed = clock.playbackSpeed;
}

// @verified - p
float GetMotionSplitFrame(const MotionWork *work, MotionRegion region) {
    return work ? RigClock(work, region)->current.splitTimeFrames : -1.0f;
}

// mtwkResetScaleAndCopyRigDefaults: the region's clock frame, or its blended frame while tracked.
// @verified - p
float GetMotionFrame(const MotionWork *motionWork, MotionRegion region) {
    const MotionTimePair *pair = GetMotionLayerTimePair(motionWork, region);
    if (!pair)
        return -1.0f;
    const MotionRuntimeState &state = motionWork->runtime;
    float frame = pair->ticksTime;
    if ((state.regionFlags[region] & MRF_Transition) == 0)
        return frame;
    const float remaining = (state.blendTotalFrames[region] - frame) * shenGetScaledDeltaTickTime(GetMotionSpeed(motionWork, region));
    if (state.currentRootMode == 3U) {
        frame = state.layerSampleFrame[region] - remaining;
        while (frame < 0.0f)
            frame += static_cast<float>(GetMotionFrameCount(state.currentMotionId[region])) - 1.0f;
    } else if (state.currentRootMode == 4U) {
        frame = (static_cast<float>(GetMotionFrameCount(state.previousMotionId[region])) - 1.0f) - remaining;
    } else if (state.currentRootMode == 5U && region == 0U) {
        frame = state.transitionTimer;
    }
    return frame;
}

float GetMotionPoseFrame(const MotionWork *motionWork, MotionRegion region) {
    const MotionTimePair *pair = GetMotionLayerTimePair(motionWork, region);
    if (!pair)
        return -1.0f;
    const MotionRuntimeState &state = motionWork->runtime;
    if ((state.regionFlags[region] & MRF_Transition) != 0 && state.currentRootMode != 2U) {
        return std::max(0.0f, state.layerSampleFrame[region]);
    }
    return pair->ticksTime;
}

float GetMotionEndFrame(const MotionWork *motionWork, MotionRegion region) {
    const MotionTimePair *pair = GetMotionLayerTimePair(motionWork, region);
    return pair ? pair->maxTime : -1.0f;
}

void SetMotionEntityRotation(MotionWork *motionWork, const Vector3i &rotation) {
    if (motionWork)
        motionWork->runtime.entityRotation = rotation;
}

namespace {

std::int32_t ParentBoneId(std::uint32_t selector) {
    switch (selector) {
    case 0:
        return 0;
    case 2:
        return 1;
    case 7:
        return 2;
    case 10:
        return 3;
    case 17:
        return 4;
    case 25:
        return 5;
    case 30:
        return 6;
    case 34:
        return 8;
    case 38:
        return 7;
    default:
        return -1;
    }
}

} // namespace

bool GetParentBoneWrapper(MotionWork *motionWork, std::int32_t selector) {
    if (!motionWork)
        return false;
    const std::uint32_t absolute = selector < 0 ? 0U - static_cast<std::uint32_t>(selector) : static_cast<std::uint32_t>(selector);
    std::uint8_t flags = selector < 0 ? 0x80U : 0U;
    if (selector >= 0 && (absolute & 0x70000000U) != 0) {
        const std::uint8_t bit30 = static_cast<std::uint8_t>((absolute >> 23U) & 0x80U);
        flags = (absolute & motionTargetIgnoreX) != 0 ? static_cast<std::uint8_t>(bit30 | 0x40U) : bit30;
        if ((absolute & motionTargetIgnoreZ) != 0)
            flags |= 0x20U;
    }
    const std::int32_t parent = ParentBoneId(absolute & 0xffU);
    if (parent < 0 || motionWork->runtime.currentMotionId[0] == 0) {
        return false;
    }
    motionWork->runtime.channelFlags = static_cast<std::uint8_t>(flags | static_cast<std::uint8_t>(parent));
    return true;
}


bool vfmaSetAngleFor(MotionWork *work, std::int32_t angle) {
    if (angle < 0)
        return false;
    work->runtime.constraintStepLimit[2] = float(angle) / 10430.378f;
    return true;
}

// @verified - p
bool SetMotionBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *point) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return false;
    MotionRuntimeState &state = work->runtime;
    if (!point) {
        state.activeConstraintTarget[controller] = nullptr;
        state.latchedConstraintTarget[controller] = nullptr;
        return false;
    }
    state.constraintBlendSource[controller] = *point;
    state.activeConstraintTarget[controller] = &state.constraintBlendSource[controller];
    state.latchedConstraintTarget[controller] = nullptr;
    return true;
}

// @verified - p
bool LatchMotionBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *point) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return false;
    MotionRuntimeState &state = work->runtime;
    if (!point) {
        state.activeConstraintTarget[controller] = nullptr;
        state.latchedConstraintTarget[controller] = nullptr;
        return false;
    }
    state.constraintBlendSource[controller] = *point;
    state.activeConstraintTarget[controller] = nullptr;
    state.latchedConstraintTarget[controller] = &state.constraintBlendSource[controller];
    return true;
}

// @verified - p
float GetMotionBoneTargetStepLimit(const MotionWork *work, std::int32_t selector) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    return controller == -1 ? 0.0f : work->runtime.constraintStepLimit[controller];
}

// @verified - p
bool SetMotionBoneTargetStepLimit(MotionWork *work, std::int32_t selector, float limit) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1 || limit < 0.0f)
        return false;
    work->runtime.constraintStepLimit[controller] = limit;
    return true;
}
bool AssignMotionTargetCallback(MotionWork *work, MotionTargetCallback callback, std::int32_t selector) {
    if (!work)
        return false;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller < 0 || (controller >= 7 && !work->rig.useOverride))
        return false;
    constexpr std::size_t offsets[] = {0x2660, 0x2760, 0x27e0, 0x2820, 0x2860, 0x26e0, 0x2720, 0x26a0, 0x27a0};
    std::memcpy(reinterpret_cast<std::byte *>(work) + offsets[controller] + 0x28, &callback, sizeof(callback));
    return true;
}

// @verified - p
bool AssignMotionTargetParameter(MotionWork *work, void *parameter, std::int32_t selector) {
    if (!work)
        return false;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller < 0 || (controller >= 7 && !work->rig.useOverride))
        return false;
    constexpr std::size_t offsets[] = {0x2660, 0x2760, 0x27e0, 0x2820, 0x2860, 0x26e0, 0x2720, 0x26a0, 0x27a0};
    std::memcpy(reinterpret_cast<std::byte *>(work) + offsets[controller] + 0x30, &parameter, sizeof(parameter));
    return true;
}

// @verified-s2: 0x7ff6185d9100.
MotionTargetCallback GetMotionTargetCallback(const MotionWork *work, std::int32_t selector) {
    if (!work)
        return nullptr;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller < 0 || (controller >= 7 && !work->rig.useOverride))
        return nullptr;
    constexpr std::size_t offsets[] = {0x2660, 0x2760, 0x27e0, 0x2820, 0x2860, 0x26e0, 0x2720, 0x26a0, 0x27a0};
    MotionTargetCallback callback{};
    std::memcpy(&callback, reinterpret_cast<const std::byte *>(work) + offsets[controller] + 0x28, sizeof(callback));
    return callback;
}

// @verified-s2: 0x7ff6185d9130.
void *GetMotionTargetParameter(const MotionWork *work, std::int32_t selector) {
    if (!work)
        return nullptr;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller < 0 || (controller >= 7 && !work->rig.useOverride))
        return nullptr;
    constexpr std::size_t offsets[] = {0x2660, 0x2760, 0x27e0, 0x2820, 0x2860, 0x26e0, 0x2720, 0x26a0, 0x27a0};
    void *parameter{};
    std::memcpy(&parameter, reinterpret_cast<const std::byte *>(work) + offsets[controller] + 0x30, sizeof(parameter));
    return parameter;
}

// Evaluates the right-foot and pelvis target callbacks with IK disabled, then leaves IK enabled.
// @verified - p
void TransformWithoutIKEnabled(MotionWork *work, Vector3 *foot, Vector3 *pelvis) {
    g_ikEnabled = 0;
    MotionCallback &footCallback = work->rig.lowerBody.rightFoot.callback;
    static_cast<MotionTargetParameters *>(footCallback.parameters)->output = foot;
    footCallback.evaluate(footCallback.parameters);
    Matrix4x4::TransformPoint(*foot, *foot);
    MotionCallback &pelvisCallback = work->rig.lowerBody.pelvisPositionCallback;
    static_cast<MotionTargetParameters *>(pelvisCallback.parameters)->output = pelvis;
    pelvisCallback.evaluate(pelvisCallback.parameters);
    Matrix4x4::TransformPoint(*pelvis, *pelvis);
    g_ikEnabled = 1;
}

// mtwkGetControllerTargetPosition: the pelvis position, or a controller's solved target.
// @verified - p
bool GetMotionControllerTargetPosition(const MotionWork *work, std::uint32_t controller, Vector3 &position) {
    if (!work || controller >= 9U)
        return false;
    const IKRig &rig = work->rig;
    const MotionRigTarget *target;
    switch (controller) {
    case 0: {
        const Vector4 &root = rig.lowerBody.pelvisWorld.rows[3];
        position = {root.x, root.y, root.z};
        return true;
    }
    case 1:
        target = &rig.chest.target;
        break;
    case 2:
        target = &rig.head.target;
        break;
    case 3:
        target = &rig.rightArm.target;
        break;
    case 4:
        target = &rig.leftArm.target;
        break;
    case 5:
        target = &rig.lowerBody.leftFoot;
        break;
    case 6:
        target = &rig.lowerBody.rightFoot;
        break;
    default:
        if (!rig.useOverride)
            return false;
        target = controller == 7U ? rig.lowerBody.overridePelvisTarget : rig.head.aimTargetNode;
        break;
    }
    position = {target->axes[2].dimensions[0], target->axes[2].dimensions[1], target->outputZ};
    return true;
}

// @verified - p
void SetMotionSpeed(MotionWork *motionWork, float speed, MotionRegion region) {
    if (motionWork && speed >= 0.0f && speed <= 4.0f)
        RigClock(motionWork, region)->playbackSpeed = speed;
}

float GetMotionSpeed(const MotionWork *motionWork, MotionRegion region) {
    const MotionRigClock *clock = RigClock(motionWork, region);
    return clock ? clock->playbackSpeed : 0.0f;
}

// @verified - p
float SetMotionRigScale(MotionWork *work, float scale) {
    if (!work || !work->runtime.currentMotionId[0])
        return -1.0f;
    const MotnSequence *const sequence = FindLoadedMotionSequence(work->runtime.currentMotionId[0]);
    if (!sequence)
        return -1.0f;
    const float factor = mtwkGetScaleFactorFor(work->rig.boneId, static_cast<std::int8_t>(sequence->animationFlags), 0);
    if (factor == 0.0f)
        return -1.0f;
    work->runtime.overrideBlendTime = scale;
    return work->rig.lowerBody.pelvisPosition[2].channel.scale / factor;
}

std::uint32_t mtwkSetExtraFlag(MotionWork *motionWork, std::uint32_t flag, std::int32_t operation) {
    if (!motionWork || flag >= 64U)
        return 0;

    std::uint32_t *word = flag < 32U ? &motionWork->runtime.motnFlags : &motionWork->runtime.seqFlags;
    const std::uint32_t bit = 1U << (flag & 31U);
    const std::uint32_t previous = *word & bit;



    switch (flag) {
    case 0:
    case 1:
    case 3:
    case 6:
    case 9:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 55:
    case 56:
        if (operation == 0)
            *word |= bit;
        else if (operation == 2)
            *word &= ~bit;
        break;
    default:
        break;
    }
    return previous;
}
// @verified - p
void EV_mtwkSetEnabled(MotionWork *motionWork, std::int32_t enabled) {
    if (!motionWork)
        return;
    if (enabled)
        motionWork->runtime.callbackFlags |= 1U;
    else
        motionWork->runtime.callbackFlags &= ~1U;
}

// @verified - p
std::uint32_t IsMotionWorkEnabled(const MotionWork *motionWork) {
    return motionWork ? motionWork->runtime.callbackFlags & 1U : 0U;
}

// @verified - p
void SetMotionReactFlags(MotionWork *work, std::uint8_t flags) {
    if (work)
        work->runtime.environmentFlags |= flags & 0x0fU;
}

void SetMotionWorkCulled(MotionWork *motionWork, bool culled) {
    if (!motionWork)
        return;
    if (culled)
        motionWork->runtime.callbackFlags |= 8U;
    else
        motionWork->runtime.callbackFlags &= ~8U;
}

void SetMotionEnvironmentEnabled(MotionWork *motionWork, std::uint32_t enabled) {
    if (motionWork)
        motionWork->runtime.enableEnvironmentMotion = enabled;
}

std::int64_t ExecuteMotionworkUtility(Character *character, std::uint32_t yaw, float angle, std::int32_t flags, const void *motionData) {
    const std::int32_t currentYaw = shenGetRotYFromNode(character);
    float scaledAngle = angle * -8192.0f;
    scaledAngle /= 45.0f;
    const std::int32_t yawDelta = std::isfinite(scaledAngle) && scaledAngle >= -2147483648.0f && scaledAngle < 2147483648.0f ?
        static_cast<std::int32_t>(scaledAngle) :
        std::numeric_limits<std::int32_t>::min();
    const std::uint16_t adjustedYaw = static_cast<std::uint16_t>(static_cast<std::uint32_t>(currentYaw) - static_cast<std::uint32_t>(yawDelta));
    shenSetRotY(character, static_cast<std::int32_t>(adjustedYaw));

    if (SetMotionRegionFlag(FindMotionWork(character), 65, 1, MotionRegion::LowerBody) == 0)
        return 0;

    shenSetRotY(character, std::bit_cast<std::int32_t>(yaw));
    std::uint16_t motionId = 0xf003;
    if ((flags & 0x200000) != 0 && motionData) {
        std::memcpy(&motionId, static_cast<const std::byte *>(motionData) + 0x0e, sizeof(motionId));
    }
    (void)mtwkRequestMotion(FindMotionWork(character), motionId, MotionRegion::LowerBody);
    return 0x10000;
}

bool BeginAuthoredCharacterMotion(
    Character *character, std::uint16_t motionId, const Vector3i *authoredRotation, float startFrame, float endFrame, float speed, std::uint32_t commandFlags) {
    MotionWork *const work = FindMotionWork(character);
    if (!work)
        return false;
    if (authoredRotation) {
        SetMotionEntityRotation(work, *authoredRotation);
    } else {
        character->entity.node.position = {};
        std::fill(std::begin(character->entity.node.rotation), std::end(character->entity.node.rotation), 0);
        SetMotionEntityRotation(work, {});
    }
    EV_mtwkSetEnabled(work, false);
    mtwkRequestMotion(work, motionId, MotionRegion::LowerBody);
    work->runtime.layerLoopStartTime[0] = 0.0f;
    work->runtime.pendingLayerStartFrame[0] = std::fmax(0.0f, startFrame);
    work->runtime.layerEndFrame[0] = endFrame < 0.0f ? 10000.0f : endFrame;
    mtwkSetExtraFlag(work, 9, 0);
    if ((commandFlags & 1U) != 0)
        SetMotionRegionFlag(work, 69, 0, MotionRegion::LowerBody);
    SetMotionSpeed(work, speed, MotionRegion::LowerBody);
    return true;
}

void FinishAuthoredCharacterMotion(Character *character, float speed, bool interrupted, bool applyYawOffset) {
    MotionWork *const work = FindMotionWork(character);
    if (!work)
        return;
    EV_mtwkSetEnabled(work, true);
    if (!interrupted)
        SetMotionSpeed(work, speed, MotionRegion::LowerBody);
    mtwkSetExtraFlag(work, 9, 2);
    if (interrupted)
        return;
    const float height = character->entity.node.position.y;
    character->entity.node.position = work->runtime.rootPosition;
    character->entity.node.position.y = height;
    if (applyYawOffset) {
        character->entity.node.rotation[1] = work->runtime.entityRotation.y;
    }
}

// @verified - p
bool ApplyCharacterMotionToPose(Character *character, Vector3 *position, Vector3i *rotation, float frame, std::uint16_t motion) {
    MotionWork *const work = FindMotionWork(character);
    if (!work || !position || !rotation)
        return false;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(position->x, position->y, position->z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation->y));
    SampleNativeMotionRootAndRotation(motion, GetMotionRigBoneId(work), frame, position, rotation);
    Matrix4x4::preTranslate(position->x, 0.0f, position->z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation->z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation->y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation->x));
    Matrix4x4 transform{};
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    *position = {transform.rows[3].x, transform.rows[3].y, transform.rows[3].z};
    *rotation = {0, static_cast<std::int32_t>(mtwkApplyTransforms(&transform)), 0};
    return true;
}

bool UpdateMotionWorkWithPosition(MotionWork *work, const Vector3 *target, std::uint16_t operation) {
    if (target && static_cast<std::int32_t>(camCtrlGetState()) == 28)
        return true;
    if (!work)
        return false;
    auto &state = work->runtime;
    state.copyFlags &= ~MCF_LookSuppressed;
    state.motnFlags &= ~0x40000U;
    const std::uint8_t mode = static_cast<std::uint8_t>(operation);
    const std::uint8_t speed = static_cast<std::uint8_t>(operation >> 8U ? operation >> 8U : 3U);
    if (mode <= 2U || mode == 4U) {
        if (mode == 0U)
            state.locomotionMotionSet &= 0xfff3U;
        if (mode == 1U)
            state.locomotionMotionSet = (state.locomotionMotionSet & 0xfff3U) | 4U;
        if (mode == 2U)
            state.locomotionMotionSet |= 12U;
        if (mode == 4U || !state.extraMotionSpeed)
            state.extraMotionSpeed = speed;
        float scale = 1.0f;
        switch (state.extraMotionSpeed) {
        case 1:
            scale = 0.5f;
            break;
        case 2:
            scale = 0.75f;
            break;
        case 4:
            scale = 1.5f;
            break;
        case 5:
            scale = 2.0f;
            break;
        default:
            break;
        }
        state.constraintStepLimit[2] = scale * 0.061359234f;
        state.extraBlendScale = scale * 0.050000001f;
        state.pelvisYawStep = static_cast<std::uint16_t>(scale * 384.0f);
        if (mode == 4U)
            return true;
        if (state.seqFlags & MSF_TargetFacing)
            state.locomotionMotionSet &= 0xfff3U;
        if (target) {
            state.transitionState = (state.seqFlags & MSF_LookAt) && state.transitionState != 1U ? 2U : 1U;
            state.seqFlags |= MSF_LookAt;
            state.lookAtTarget = *target;
        } else if (state.seqFlags & MSF_LookAt) {
            state.transitionState = 3;
        } else {
            state.extraMotionSpeed = 0;
        }
        return true;
    }
    if (!(state.seqFlags & MSF_LookAt))
        return false;
    switch (mode) {
    case 3:
        state.locomotionMotionSet |= 0x10U;
        return true;
    case 5:
        state.locomotionMotionSet &= 0xfe1fU;
        return true;
    case 6:
        state.locomotionMotionSet |= 0x60U;
        break;
    case 7:
        state.locomotionMotionSet |= 0x180U;
        break;
    case 8:
        state.locomotionMotionSet &= 0xff5fU;
        return true;
    case 9:
        state.locomotionMotionSet |= 0x20U;
        break;
    case 10:
        state.locomotionMotionSet |= 0x80U;
        break;
    case 11:
        state.locomotionMotionSet &= 0xfebfU;
        return true;
    case 12:
        state.locomotionMotionSet |= 0x40U;
        break;
    case 13:
        state.locomotionMotionSet |= 0x100U;
        break;
    default:
        return false;
    }
    if (mode == 6U || mode == 7U || mode == 9U || mode == 10U)
        GetMotionControllerTargetPosition(work, 3, state.leftControllerTarget);
    if (mode == 6U || mode == 7U || mode == 12U || mode == 13U)
        GetMotionControllerTargetPosition(work, 4, state.rightControllerTarget);
    return true;
}

bool RequestMotionTargetFacing(MotionWork *work, const Vector3 *target, std::uint32_t flags) {
    if (!work || work->rig.useOverride)
        return false;
    auto &state = work->runtime;
    if (!target) {
        if ((state.seqFlags & MSF_TargetFacing) && static_cast<std::uint8_t>(state.targetFacingState - 6U) > 1U)
            state.targetFacingState = 5;
        return true;
    }
    if ((state.seqFlags & MSF_TargetFacing) && state.targetFacingState > 3U) {
        if (state.targetFacingState == 4U)
            PlayTargetFacingTurn(*work, false, (state.targetFacingFlags & 4U) != 0, state.targetFacingYawDelta);
        state.targetFacingFlags |= 8U;
        if (state.targetFacingFlags & 4U)
            state.targetFacingFlags |= 0x10U;
        else
            state.targetFacingFlags &= ~0x10U;
    }
    if (flags & 1U)
        state.targetFacingFlags |= 1U;
    else
        state.targetFacingFlags &= ~1U;
    if (flags & 6U) {
        state.targetFacingFlags |= 2U;
        if (flags & 2U)
            state.targetFacingFlags |= 4U;
        else
            state.targetFacingFlags &= ~4U;
    } else
        state.targetFacingFlags &= ~2U;
    state.targetFacingMotionClass = static_cast<std::uint8_t>(flags >> 16U);
    state.targetFacingPosition = *target;
    state.seqFlags |= MSF_TargetFacing;
    state.targetFacingState = 1;
    return true;
}

bool ResetMotionWorkPositionTransition(MotionWork *work) {
    return UpdateMotionWorkWithPosition(work, nullptr, 0);
}

void SetMotionLayerEndFrame(MotionWork *work, float frame, MotionRegion region) {
    if (!work)
        return;
    if (frame < 0.0f)
        frame = 10000.0f;
    if (region < 5U)
        work->runtime.layerEndFrame[region] = frame;
}

void SetMotionLayerLoopStartTime(MotionWork *work, float frame, MotionRegion region) {
    if (work && region < 5U)
        work->runtime.layerLoopStartTime[region] = frame < 0.0f ? 0.0f : frame;
}

// @verified - p
void SetMotionLayerFrameRange(MotionWork *work, float loopStart, float startFrame, float endFrame, MotionRegion region) {
    if (!work || region >= 5U)
        return;
    work->runtime.layerLoopStartTime[region] = loopStart < 0.0f ? 0.0f : loopStart;
    work->runtime.pendingLayerStartFrame[region] = startFrame < 0.0f ? 0.0f : startFrame;
    work->runtime.layerEndFrame[region] = endFrame < 0.0f ? 10000.0f : endFrame;
}

bool AssignMotionRigYawOffset(MotionWork *work, std::uint32_t yaw) {
    if (!work)
        return false;
    work->runtime.requestedRootYawOffset = std::bit_cast<std::int32_t>(yaw);
    return true;
}

void SetMotionPelvisYawStep(MotionWork *work, std::int16_t step) {
    work->runtime.pelvisYawStep = static_cast<std::uint16_t>(step);
}

// @verified - p
bool SetMotionFrameTime(MotionWork *work, std::uint32_t frames, MotionRegion region) {
    if (region >= 5U)
        return false;
    work->runtime.defaultRegionStepFrames[region] = static_cast<std::uint16_t>(std::min(frames, 255U));
    return true;
}

void SetMotionRegionEvalTime(MotionWork *work, float frame, MotionRegion region) {
    if (work && region < 5U)
        work->runtime.pendingLayerStartFrame[region] = frame < 0.0f ? 0.0f : frame;
}

bool HasMotionTrackCommand(const MotionWork *work, MotionRegion region) {
    if (!work || region >= 5U)
        return false;
    const auto &state = work->runtime;
    const auto *stream = static_cast<const std::byte *>(state.regionTrack[region && (state.regionFlags[region] & MRF_Independent) ? region : 0]);
    if (!stream)
        return false;
    std::uint16_t count{};
    std::memcpy(&count, stream, sizeof(count));
    for (std::uint32_t index = 0; index < count; ++index) {
        std::uint16_t offset{}, command{};
        std::memcpy(&offset, stream + (index + 1U) * 2U, sizeof(offset));
        std::memcpy(&command, stream + offset + 2U, sizeof(command));
        if ((command & 0xffU) != 44U && (command >> 8U) == region)
            return true;
    }
    return false;
}

bool HasMotionDependentTrack(const MotionWork *work) {
    if (!work)
        return false;
    for (std::size_t region = 0; region < 5; ++region)
        if (!(work->runtime.regionFlags[region] & MRF_Independent) && work->runtime.regionTrack[region])
            return true;
    return false;
}

std::uint16_t SetMotionRegionFlag(MotionWork *motionWork, std::uint32_t mode, std::int32_t operation, MotionRegion region) {
    if (!motionWork || mode <= 63U)
        return 0;

    if (mode == 70U) {
        std::uint16_t result = motionWork->runtime.regionFlags[region] & MRF_Mirrored;
        if (const MotnSequence *sequence = FindLoadedMotionSequence(motionWork->runtime.currentMotionId[region]); sequence && sequence->hasLeadFootCommand) {
            result ^= MRF_Mirrored;
        }
        return result;
    }
    const std::uint16_t bit = static_cast<std::uint16_t>(1U << ((mode - 64U) & 31U));
    auto apply = [&](std::uint32_t target) {
        std::uint16_t &flags = motionWork->runtime.regionFlags[target];
        const std::uint16_t previous = flags & bit;
        if (operation == 0)
            flags |= bit;
        else if (operation == 2)
            flags &= static_cast<std::uint16_t>(~bit);
        return previous;
    };

    if (mode == 72U && region == 0U) {
        std::uint16_t result = 0;
        for (std::uint32_t target = 1; target < 5; ++target) {
            result |= motionWork->runtime.regionFlags[target] & bit;
        }
        return result;
    }
    if (mode == 72U) {
        return motionWork->runtime.regionFlags[region] & bit;
    }
    if (mode == 66U && operation == 0) {
        std::uint16_t &flags = motionWork->runtime.regionFlags[region];
        const std::uint16_t previous = flags & 0x1000U;
        flags |= 0x1000U;
        return previous;
    }
    switch (mode) {
    case 66U:
    case 69U:
    case 71U:
    case 73U:
    case 74U:
    case 76U:
        return apply(region);
    default:
        return motionWork->runtime.regionFlags[region] & bit;
    }
}

bool mtwkSetRequestedRootMode(MotionWork *motionWork, std::uint32_t mode) {
    if (mode >= 6U)
        return false;
    motionWork->runtime.requestedRootMode = static_cast<std::uint8_t>(mode);
    return true;
}

// @verified - p
std::uint64_t SetMotionTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId) {
    if (!work)
        return false;
    EnsureDefaultMotionSet(*work);
    MotionRuntimeState &state = work->runtime;
    if (mode == 1U) {
        if (slot >= 6U)
            return false;
        if (!motionId || FindLoadedMotionData(motionId).empty()) {
            if (slot != 5U)
                return false;
            state.extraMotionIds[5] = state.extraMotionIds[0];
            state.copyFlags &= ~MCF_Unknown1000;
        } else {
            state.extraMotionIds[slot] = motionId;
            if (slot == 5U)
                state.copyFlags |= MCF_Unknown1000;
        }
        return true;
    }
    if (mode >= std::size(defaultMotionSets))
        return false;
    const DefaultMotionSet &defaults = defaultMotionSets[mode ? mode : DefaultMotionSetIndex(work->rig.boneId)];
    for (std::size_t index = 0; index < 6; ++index)
        state.extraMotionIds[index] = defaults.slots[index];
    return mode != 0U;
}

bool SetMotionExtraMotion(MotionWork *work, std::uint32_t slot, std::uint16_t motionId) {
    return slot < 6U && SetMotionTimeRelated(work, 1, static_cast<std::uint16_t>(slot), motionId);
}

// @verified - p
bool CopyMotionTransition(MotionWork *motionWork, const Vector3 *position, std::uint16_t yaw, std::int16_t operation, std::int32_t copyType) {
    if (!motionWork)
        return false;
    EnsureDefaultMotionSet(*motionWork);
    MotionRuntimeState &state = motionWork->runtime;
    const bool active = (state.seqFlags & MSF_PositionTransition) != 0;
    switch (operation) {
    case 1:
        if (!active)
            return false;
        CommitCopyMotionTransition(*motionWork);
        return true;
    case 2:
        if (!active)
            return false;
        state.copyFlags |= 0x10U;
        return true;
    case 3:
        return (state.copyFlags & MCF_Persistent) != 0;
    case 8:
        if (!active)
            return false;
        state.copyFlags |= MCF_AlternateLocomotion;
        return true;
    case 9:
    case 10:
    case 11:
        if (!active)
            return false;
        state.copyFlags |= MCF_ExtraStepRequested;
        return true;
    case 12:
        if (!active)
            return false;
        state.copyFlags |= MCF_Unknown2000;
        return true;
    case 14:
        if (!active)
            return false;
        state.copyFlags &= ~MCF_AlternateLocomotion;
        return true;
    case 15:
        return (state.copyFlags & MCF_Running) != 0;
    case 16:
        if (!active)
            return false;
        state.copyFlags |= MCF_Unknown40000;
        return true;
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 13:
        break;
    default:
        return false;
    }
    const std::uint8_t variant = operation == 13 ? 2U : operation >= 5 ? static_cast<std::uint8_t>(operation - 4) : 0U;
    if (!variant)
        state.copyFlags |= MCF_Unknown80;
    state.transitionVariant = variant;
    if (!active)
        state.transitionClass = variant;

    const bool negativeAdmitted = variant == 3U || (variant == 1U && active);
    if (operation != 13 && (copyType > 3 || (copyType < 0 && !negativeAdmitted) || (copyType != 2 && !position)))
        return false;
    if (active) {
        if (operation == 13) {
            state.transitionSubstate = 2;
            state.transitionEndFrame = yaw;
            state.copyFlags = (state.copyFlags & ~(MCF_LookPending | MCF_Unknown4000 | MCF_Unknown10000 | MCF_Unknown20000)) | 0x4100U;
            return true;
        }
        if (position && copyType != 2)
            state.copyDestinationPosition = *position;
        else if (copyType == 2 && (variant == 0 || variant == 2))
            state.copyDestinationPosition = state.rootPosition;
        state.transitionEndFrame = yaw;
        state.copyFlags = (state.copyFlags & (variant == 3 ? 0xfffcbfffU : 0xfffcbeffU)) | 0x100U;
        state.transitionSubstate = copyType == 3 ? 0U : static_cast<std::uint8_t>(copyType);
        if (((variant == 1 || variant == 2) && copyType == 3) || (variant == 3 && copyType == 1))
            state.copyFlags |= MCF_Unknown10000;
        return true;
    }
    if (position && copyType != 2 && operation != 13) {
        state.copySourcePosition = *position;
        if (variant)
            state.copyDestinationPosition = *position;
    }
    if (variant == 1) {
        if (copyType == 1 || copyType == 2)
            state.copyFlags &= ~MCF_Unknown80;
        else
            state.copyFlags |= MCF_Unknown80;
    }
    if (state.motnFlags & 0x8000U)
        state.copyFlags |= MCF_Unknown01;
    if (state.motnFlags & 0x10000U)
        state.copyFlags |= MCF_Unknown02;
    state.copyRegionSelector = state.requestedRootMode;
    state.transitionFrameTime = state.defaultRegionStepFrames[0];
    state.transitionStartFrame = yaw;
    if (variant)
        state.transitionEndFrame = yaw;
    state.copyFlags &= (variant == 0 || variant == 3) ? 0xfffcf7ffU : 0xfffcffffU;
    if (operation == 13)
        state.copyFlags = (state.copyFlags & ~(MCF_LookPending | MCF_Unknown4000 | MCF_Unknown10000 | MCF_Unknown20000)) | 0x4100U;
    else if (((variant == 1 || variant == 2) && copyType == 3) || (variant == 3 && copyType == 1))
        state.copyFlags |= MCF_Unknown10000;
    state.transitionSubstate = operation == 13 ? 2U : copyType == 3 ? 0U : static_cast<std::uint8_t>(copyType);
    if (ClassifyCurrentLocomotionPhase(*motionWork)) {
        if (!variant) {
            state.copyDestinationPosition = state.copySourcePosition;
            state.transitionEndFrame = yaw;
        }
        state.copyFlags |= MCF_LookPending;
        state.copyOperationState = 5;
        state.copyPhase = 1;
    } else {
        state.copyOperationState = 1;
    }
    if (variant == 2 || variant == 3)
        state.copyFlags |= MCF_LookPending;
    state.motionScale = GetMotionSpeed(motionWork, MotionRegion::LowerBody);
    if (state.motionScale < 0.5f) {
        state.motionScale = 0.5f;
        SetMotionSpeed(motionWork, 0.5f, MotionRegion::LowerBody);
    }
    state.seqFlags |= MSF_PositionTransition;
    if (operation != 13)
        state.queuedMotionId[0] = 0;
    return true;
}

// @verified - p
bool RequestMotionPositionTransition(MotionWork *work, const Vector3 *position, std::uint16_t yaw, std::uint32_t motion, float startFrame) {
    if (!work)
        return false;
    const std::uint16_t animation = static_cast<std::uint16_t>(motion);
    const bool mirrored = (motion & 0x00ff0000U) != 0;
    if (position) {
        Vector3 target;
        if (startFrame < 0.0f) {
            target = *position;
        } else {
            SampleNativeMotionRoot(animation, static_cast<std::int32_t>(work->rig.boneId), startFrame, target);
            target.y = 0.0f;
            if (mirrored)
                target.x = -target.x;
            Matrix4x4_Push(&transformMatrix_prePass);
            Matrix4x4::preTranslate(position->x, position->y, position->z);
            Matrix4x4::RotateAroundY_Wrapper(yaw);
            Matrix4x4::TransformPoint(target, target);
            Matrix4x4::Pop(1);
        }
        CopyMotionTransition(work, &target, yaw, 6, 0);
        if (motion & 0xff000000U)
            CopyMotionTransition(work, nullptr, 0, 11, 0);
    } else {
        CopyMotionTransition(work, nullptr, yaw, 13, 0);
    }
    EnsureDefaultMotionSet(*work);
    if (animation != 0 && !FindLoadedMotionData(animation).empty()) {
        work->runtime.extraMotionIds[5] = animation;
        work->runtime.copyFlags |= MCF_Unknown1000;
    } else {
        work->runtime.extraMotionIds[5] = work->runtime.extraMotionIds[0];
        work->runtime.copyFlags &= ~MCF_Unknown1000;
    }
    if (startFrame >= 0.0f)
        SetMotionRegionEvalTime(work, startFrame, MotionRegion::LowerBody);
    if (mirrored)
        work->runtime.regionFlags[0] |= MRF_LeadFoot;
    return true;
}

bool BeginMotionPositionTransition(MotionWork *motionWork, const Vector3 &target, std::uint16_t targetYaw) {
    return CopyMotionTransition(motionWork, &target, targetYaw, 0, 0);
}

bool CompleteMotionPositionTransitionRequest(MotionWork *motionWork) {
    if (!motionWork || !(motionWork->runtime.seqFlags & MSF_PositionTransition)) {
        return false;
    }
    // mtwkCopyCurrent op 14 precedes mtwkSetTimeRelated mode 1/slot 5.
    motionWork->runtime.copyFlags &= ~MCF_AlternateLocomotion;
    return SetMotionExtraMotion(motionWork, 5U, 0xf002U);
}

// 5d94c0 / 5db020: the runtime root is distinct from solved pelvis output.
Vector3 GetMotionRootPosition(const MotionWork *motion) {
    return motion->runtime.rootPosition;
}
void SetMotionRootPosition(MotionWork *motion, const Vector3 &position) {
    motion->runtime.rootPosition = position;
}



void ApplyMotionCollisionTranslation(MotionWork *motion, float x, float y, float z) {
    if (!motion)
        return;
    motion->runtime.rootDelta.x += x;
    motion->runtime.rootDelta.y += y;
    motion->runtime.rootDelta.z += z;
    auto *const raw = reinterpret_cast<std::byte *>(motion);
    const auto translate = [x, y, z](std::byte *owner, std::size_t offset) {
        Vector3 value;
        std::memcpy(&value, owner + offset, sizeof(value));
        value.x += x;
        value.y += y;
        value.z += z;
        std::memcpy(owner + offset, &value, sizeof(value));
    };
    constexpr std::size_t matrices[]{
        0x2c0,0x4c0,0x778,0x7b8,0x8e8,0xd50,0xd90,0xec0,0x14d8,0x1678,
        0x1920,0x1b08,0x1db0,0x1df0,0x1fa0,0x2170,0x2418,0x2458,0x2608};
    for (const auto offset : matrices)
        translate(raw, offset);
    std::uint32_t custom;
    std::memcpy(&custom, raw + 4, sizeof(custom));
    if (custom) {
        constexpr std::size_t pointers[]{0x4d8, 0xab0, 0x1088, 0x1098, 0x10a8, 0x1950, 0x1960, 0x1970, 0x1980};
        for (const auto offset : pointers) {
            std::byte *matrix;
            std::memcpy(&matrix, raw + offset, sizeof(matrix));
            translate(matrix, 0x30);
        }
    } else {
        constexpr std::size_t standard[]{0x1368, 0x1e70, 0x24d8, 0xa58, 0x1030};
        for (const auto offset : standard)
            translate(raw, offset);
    }
    constexpr std::size_t targets[]{0x12e8, 0x18a0, 0x1d30, 0x2398, 0x6f8, 0xcd0};
    for (const auto offset : targets)
        translate(raw, offset);
    if (custom) {
        for (const auto offset : {std::size_t{0x488}, std::size_t{0x1948}}) {
            std::byte *node;
            std::memcpy(&node, raw + offset, sizeof(node));
            translate(node, 0xe8);
        }
    }
    motion->runtime.rootPosition.x = motion->rig.lowerBody.pelvisWorld.rows[3].x;
    motion->runtime.rootPosition.z = motion->rig.lowerBody.pelvisWorld.rows[3].z;
}

} // namespace shenmue::s2
