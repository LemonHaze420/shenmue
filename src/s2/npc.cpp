#include "s2/npc.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/model_component.hpp"
#include "s2/mt7.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/math.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/npc_package.hpp"
#include "s2/legacy_draw.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {

std::span<NpcRouteLiveEntry> npcRouteLiveEntries;
shen_name scenarioFollowCharacter{};
std::int32_t scenarioFollowGraceFrames{};
std::int32_t scenarioFollowCandidateIndex{-1};

struct NpcMotionPalette {
    std::uint16_t motions[3][5];
    std::uint8_t weights[5];
};


constexpr NpcMotionPalette specialMotionPalettes[18]{
    {{{0x8080, 0x8087, 0x8085, 0x8084, 0x8083}, {0x8080, 0x8087, 0x8083, 0x8086, 0x8085}, {0x80b6, 0x80b7, 0x8086, 0x8082, 0x8081}}, {10, 10, 30, 25, 20}},
    {{{0x8080, 0x8087, 0x8084, 0x8083, 0x8086}, {0x8080, 0x8087, 0x8083, 0x8086, 0x8085}, {0x80b6, 0x80b7, 0x8086, 0x8082, 0x8081}}, {10, 10, 30, 25, 20}},
    {{{0x8080, 0x8087, 0x8083, 0x8082, 0x8086}, {0x8080, 0x8087, 0x8083, 0x8086, 0x8085}, {0x80b6, 0x80b7, 0x8086, 0x8082, 0x8081}}, {10, 10, 30, 25, 20}},
    {{{0x8090, 0x8096, 0x8094, 0x8093, 0x8092}, {0x8090, 0x8096, 0x8092, 0x8095, 0x8094}, {0x80bb, 0x80b8, 0x8095, 0x8094, 0x8091}}, {10, 10, 30, 25, 20}},
    {{{0x8090, 0x8096, 0x8093, 0x8092, 0x8095}, {0x8090, 0x8096, 0x8092, 0x8095, 0x8094}, {0x80bb, 0x80b8, 0x8095, 0x8094, 0x8091}}, {10, 10, 30, 25, 20}},
    {{{0x8090, 0x8096, 0x8092, 0x8091, 0x8095}, {0x8090, 0x8096, 0x8092, 0x8095, 0x8094}, {0x80bb, 0x80b8, 0x8095, 0x8094, 0x8091}}, {10, 10, 30, 25, 20}},
    {{{0x809a, 0x80a0, 0x809e, 0x809f, 0x809c}, {0x809a, 0x80a0, 0x809d, 0x809b, 0x809c}, {0x809a, 0x80a0, 0x809f, 0x809b, 0x809e}}, {20, 5, 30, 25, 20}},
    {{{0x809a, 0x80a0, 0x809d, 0x809f, 0x809b}, {0x809a, 0x80a0, 0x809d, 0x809b, 0x809c}, {0x809a, 0x80a0, 0x809f, 0x809b, 0x809e}}, {20, 5, 30, 25, 20}},
    {{{0x809a, 0x80a0, 0x809b, 0x809c, 0x809e}, {0x809a, 0x80a0, 0x809d, 0x809b, 0x809c}, {0x809a, 0x80a0, 0x809f, 0x809b, 0x809e}}, {20, 5, 30, 25, 20}},
    {{{0x809a, 0x80a0, 0x809e, 0x809f, 0x809c}, {0x809a, 0x80a0, 0x809d, 0x809b, 0x809c}, {0x809a, 0x80a0, 0x809f, 0x809b, 0x809e}}, {20, 5, 30, 25, 20}},
    {{{0x809a, 0x80a0, 0x809d, 0x809f, 0x809b}, {0x809a, 0x80a0, 0x809d, 0x809b, 0x809c}, {0x809a, 0x80a0, 0x809f, 0x809b, 0x809e}}, {20, 5, 30, 25, 20}},
    {{{0x809a, 0x80a0, 0x809b, 0x809c, 0x809e}, {0x809a, 0x80a0, 0x809d, 0x809b, 0x809c}, {0x809a, 0x80a0, 0x809f, 0x809b, 0x809e}}, {20, 5, 30, 25, 20}},
    {{{0x80aa, 0x80b1, 0x80af, 0x80ac, 0x80ad}, {0x80aa, 0x80b1, 0x80ae, 0x80b0, 0x80ab}, {0x80aa, 0x80b1, 0x80ab, 0x80af, 0x80ac}}, {10, 10, 30, 25, 20}},
    {{{0x80aa, 0x80b1, 0x80ad, 0x80ae, 0x80ac}, {0x80aa, 0x80b1, 0x80ae, 0x80b0, 0x80ab}, {0x80aa, 0x80b1, 0x80ab, 0x80af, 0x80ac}}, {10, 10, 30, 25, 20}},
    {{{0x80aa, 0x80b1, 0x80b0, 0x80ae, 0x80ab}, {0x80aa, 0x80b1, 0x80ae, 0x80b0, 0x80ab}, {0x80aa, 0x80b1, 0x80ab, 0x80af, 0x80ac}}, {10, 10, 30, 25, 20}},
    {{{0x80aa, 0x80b1, 0x80af, 0x80ac, 0x80ad}, {0x80aa, 0x80b1, 0x80ae, 0x80b0, 0x80ab}, {0x80aa, 0x80b1, 0x80ab, 0x80af, 0x80ac}}, {10, 10, 30, 25, 20}},
    {{{0x80aa, 0x80b1, 0x80ad, 0x80ae, 0x80ac}, {0x80aa, 0x80b1, 0x80ae, 0x80b0, 0x80ab}, {0x80aa, 0x80b1, 0x80ab, 0x80af, 0x80ac}}, {10, 10, 30, 25, 20}},
    {{{0x80aa, 0x80b1, 0x80ad, 0x80ae, 0x80ac}, {0x80aa, 0x80b1, 0x80ae, 0x80b0, 0x80ab}, {0x80aa, 0x80b1, 0x80ab, 0x80af, 0x80ac}}, {10, 10, 30, 25, 20}}
};

struct NamedNpcMotionPalette {
    shen_name name;
    std::uint16_t motions[5];
    std::uint8_t weights[5];
};
constexpr NamedNpcMotionPalette namedMotionPalettes[]{{SHEN_NAME(KNK_), {0xe0e6, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}},
    {SHEN_NAME(KUN_), {0x9313, 0x9313, 0x9315, 0x9316, 0x9314}, {30, 30, 30, 25, 20}}, {SHEN_NAME(SYE_), {0x9603, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}},
    {SHEN_NAME(REN_), {0x8080, 0x8084, 0x8083, 0x0, 0x0}, {30, 30, 30, 0, 0}}, {SHEN_NAME(JOP_), {0x9809, 0x9808, 0x0, 0x0, 0x0}, {50, 50, 0, 0, 0}},
    {SHEN_NAME(SIN_), {0x9503, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}}, {SHEN_NAME(LLY_), {0xe3b7, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}},
    {SHEN_NAME(KEK_), {0x80aa, 0x80b1, 0x80ae, 0x0, 0x0}, {30, 30, 30, 0, 0}}, {SHEN_NAME(SAI_), {0x931a, 0x0, 0x0, 0x0, 0x0}, {100, 0, 0, 0, 0}}};
constexpr std::uint16_t dependentMotionPairs[][2]{{0x80db, 0x80c0}, {0x80dc, 0x80c0}, {0x80da, 0x80c0}, {0x80c0, 0x80c0}, {0x80d8, 0x80bf}, {0x80eb, 0x80be},
    {0x80ea, 0x80bd}, {0x80e9, 0x80bc}, {0x9309, 0x9312}, {0x9305, 0x9311}, {0x9307, 0x9318}, {0x9302, 0x9318}, {0x9303, 0x9318}, {0x9301, 0x9319},
    {0x8074, 0x8074}, {0x8015, 0x8015}, {0x8072, 0x8072}, {0xe310, 0xe332}};
constexpr std::uint16_t seatedMotions[6][3][3]{
    {{0x802c, 0x802b, 0x8027}, {0x802d, 0x8028, 0x802d}, {0x8029, 0x8029, 0x802a}},
    {{0x8001, 0x8001, 0x8004}, {0x8004, 0x8004, 0x8003}, {0x8003, 0x8002, 0x8002}},
    {{0x802d, 0x802d, 0x802d}, {0x802a, 0x802a, 0x802a}, {0x8029, 0x8029, 0x8029}},
    {{0x802d, 0x802d, 0x802d}, {0x802a, 0x802a, 0x802a}, {0x8029, 0x8029, 0x8029}},
    {{0x802d, 0x802d, 0x802d}, {0x802d, 0x802d, 0x802d}, {0x8014, 0x8014, 0x8014}},
    {{0x8003, 0x8003, 0x8003}, {0x8003, 0x8003, 0x8003}, {0x8014, 0x8014, 0x8014}}
};
constexpr std::uint16_t leaningMotions[6][3][3]{
    {{0xe37a, 0xe37a, 0xe37a}, {0xe379, 0xe37b, 0xe37b}, {0xe379, 0xe379, 0xe379}},
    {{0xe377, 0xe377, 0xe377}, {0xe244, 0xe244, 0xe244}, {0xe376, 0xe376, 0xe376}},
    {{0xe249, 0xe249, 0xe249}, {0xe379, 0xe379, 0xe379}, {0xe249, 0xe249, 0xe249}},
    {{0xe249, 0xe249, 0xe249}, {0xe249, 0xe249, 0xe249}, {0xe376, 0xe376, 0xe376}},
    {{0xe24a, 0xe24a, 0xe24a}, {0xe379, 0xe379, 0xe379}, {0xe378, 0xe378, 0xe378}},
    {{0xe24a, 0xe24a, 0xe24a}, {0xe376, 0xe376, 0xe376}, {0xe378, 0xe378, 0xe378}}
};

std::int32_t NpcProfileGroup(const NpcRouteLiveEntry &entry) {
    const std::int8_t age = static_cast<std::int8_t>(entry.recordBase[5]);
    return (age < 13 ? 2 : age > 55 ? 4 : 0) + (static_cast<std::uint8_t>(entry.recordBase[4]) != 0);
}

std::size_t SelectWeightedNpcMotion(const std::uint8_t *weights, std::size_t count) {
    std::int32_t total = 0;
    for (std::size_t index = 0; index < count; ++index)
        total += weights[index];
    std::int32_t choice = static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(total));
    for (std::size_t index = 0; index < count; ++index) {
        choice -= weights[index];
        if (choice <= 0)
            return index;
    }
    return 0;
}

std::int32_t NpcEulerYaw(const Vector3 &angles) {
    std::int32_t yaw;
    std::memcpy(&yaw, &angles.y, sizeof(yaw));
    return yaw;
}

std::int16_t NpcTargetYaw(const Vector3 &origin, const Vector3 &target) {
    const float x = target.x - origin.x;
    const float z = target.z - origin.z;
    if (std::abs(x) < 0.001f && std::abs(z) < 0.001f)
        return 0;
    return static_cast<std::int16_t>(FixedAtan2(-x, -z));
}

std::int16_t NpcExcessTurn(std::int16_t angle) {
    return angle < -20024 ? static_cast<std::int16_t>(angle + 20024) : angle > 20024 ? static_cast<std::int16_t>(angle - 20024) : 0;
}

void ReleaseNpcSpecialAnchor(NpcRouteLiveEntry &entry) {
    NpcRouteStateContext &context = entry.stateContext;
    context.routeFlags &= ~1U;
    if (!(context.flags & 8U))
        context.flags &= ~0x8000U;
    if (context.flags & 0x100U) {
        if (entry.eyeControlMode) {
            entry.field338 = static_cast<std::uint8_t>(-entry.eyeControlMode);
            entry.eyeControlMode = 10;
        } else {
            entry.field338 = static_cast<std::uint8_t>(entry.routeDirectionMode);
            entry.routeDirectionMode = 10;
        }
    } else if (!(entry.flags & 0x10U)) {
        if (entry.field338 & 0x80U)
            entry.eyeControlMode = 12;
        else
            entry.routeDirectionMode = 12;
    }
}

bool CompleteNpcSpecialMotion(NpcRouteLiveEntry &entry) {
    entry.flags &= ~0x4010U;
    entry.liveState = entry.previousLiveState;
    entry.specialMotionPhase = 0;
    return false;
}

} // namespace

// @verified - p
void HandleNpcEyeControl(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    const float duration = static_cast<float>(static_cast<std::int8_t>(entry.routeEyeMode)) / 10.0f;
    if (entry.eyeControlProfile == 23) {
        SetHighModelAndFaceFlagsForChara(entry.node, 0, 0.0f, duration);
        AT_SetEyeCtrl(entry.node, 30.0f, 1.0f, 1.0f);
    } else {
        AT_SetEyeCtrl(entry.node, 0.0f, 0.0f, 0.0f);
        SetHighModelAndFaceFlagsForChara(entry.node, entry.eyeControlProfile, 30.0f, duration);
    }
}

// @verified - p
void npcInit() {
    ResetCharacterMirrorAdmission();
    SetLegacyMirroredDrawMode(0);
}

std::uint16_t SelectNpcIdleMotion(const NpcRouteLiveEntry &entry) {

    static constexpr std::uint16_t motions[6][3][3]{
        {{0xe108,0x8036,0xe108},{0x8036,0xe108,0xe380},{0xe381,0xe381,0xe380}},
        {{0x800f,0xe0f1,0x8009},{0xe101,0xe101,0xe101},{0x801f,0xe0df,0xe0f2}},
        {{0xe0e3,0xe0e3,0xe0e3},{0xe108,0xe0f5,0xe0f5},{0xe0e4,0xe0e4,0xe381}},
        {{0xe0e3,0xe0e3,0xe0e3},{0xe0f5,0xe0f5,0xe0f5},{0xe0e4,0xe0e4,0xe0e4}},
        {{0xe0f7,0xe107,0xe108},{0xe381,0xe380,0xe381},{0xe107,0xe0f7,0xe107}},
        {{0xe101,0xe101,0xe0f2},{0xe0f0,0xe0f0,0xe0f0},{0xe0f6,0xe0f6,0xe0f6}}};
    const std::int32_t entityClass = entry.node ? static_cast<std::uint16_t>(static_cast<const MT7Model *>(entry.node->model)->entityType) / 32 : 0;
    if (entityClass >= 3)
        return 0x8023;
    const std::int8_t age = static_cast<std::int8_t>(entry.recordBase[5]);
    const std::int32_t ageClass = age < 13 ? 2 : age > 55 ? 4 : 0;
    const std::int32_t sex = static_cast<std::uint8_t>(entry.recordBase[4]) != 0;
    const std::int8_t profile = static_cast<std::int8_t>(entry.recordBase[8]);
    return motions[ageClass + sex][profile][entityClass];
}

std::uint16_t RemapNpcMotionForRig(std::int32_t rig, std::uint16_t motion) {
    const std::uint16_t offset = static_cast<std::uint16_t>(motion + 0x1fffU);
    if (static_cast<std::uint16_t>(motion + 0x2000U) > 0x374U || offset % 17U)
        return motion;
    return static_cast<std::uint16_t>(rig + 17 * (offset / 17) + 0xe002);
}

std::uint16_t SelectNpcSpecialMotion(const NpcRouteLiveEntry &entry, bool varied) {
    if (entry.node && HasMotionDependentTrack(entry.motionWork)) {
        const std::uint16_t current = GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody);
        for (const auto &pair : dependentMotionPairs)
            if (pair[0] == current)
                return pair[1];
        return 0;
    }
    const std::size_t count = varied ? 5 : 2;
    for (const NamedNpcMotionPalette &palette : namedMotionPalettes) {
        if (palette.name.name == entry.name.name)
            return palette.motions[SelectWeightedNpcMotion(palette.weights, count)];
    }
    const std::int8_t profile = static_cast<std::int8_t>(entry.recordBase[8]);
    const NpcMotionPalette &palette = specialMotionPalettes[NpcProfileGroup(entry) * 3 + profile];
    const std::int32_t entityClass = entry.node ? static_cast<std::uint16_t>(static_cast<const MT7Model *>(entry.node->model)->entityType) / 32 : 0;
    return palette.motions[entityClass][SelectWeightedNpcMotion(palette.weights, count)];
}

// @verified - p
std::uint16_t SelectNpcSeatedIdleMotion(const NpcRouteLiveEntry &entry) {
    const std::int32_t entityClass = entry.node ? static_cast<std::uint16_t>(static_cast<const MT7Model *>(entry.node->model)->entityType) / 32 : 0;
    if (entityClass >= 3)
        return 0x802d;
    return seatedMotions[NpcProfileGroup(entry)][static_cast<std::int8_t>(entry.recordBase[8])][entityClass];
}

std::uint16_t SelectNpcAreaActionBaseMotion(const NpcRouteLiveEntry &entry, std::uint8_t action) {
    if (!entry.node)
        return 0;
    switch (action) {
    case 2:
    case 6:
    case 7:
        return RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe111);
    case 16:
    case 17:
    case 18:
        return RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe078);
    case 1:
    case 4:
    case 5:
    case 3:
    case 8:
    case 9: {
        const bool seated = action == 1 || action == 4 || action == 5;
        const std::int32_t entityClass = static_cast<std::uint16_t>(static_cast<const MT7Model *>(entry.node->model)->entityType) / 32;
        if (entityClass >= 3)
            return seated ? 0x802d : 0xe379;
        const std::int8_t profile = static_cast<std::int8_t>(entry.recordBase[8]);
        return seated ? seatedMotions[NpcProfileGroup(entry)][profile][entityClass] : leaningMotions[NpcProfileGroup(entry)][profile][entityClass];
    }
    default:
        return SelectNpcIdleMotion(entry);
    }
}

bool TickNpcSpecialMotion(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return false;
    TickNpcSpecialRouteContext(entry);
    if (entry.previousLiveState == 8)
        TickNpcAreaRouteState(entry);
    MotionWork *const work = entry.motionWork;
    switch (entry.specialMotionPhase) {
    case 2:
        if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody) && !mtwkSetExtraFlag(work, 58, 1)) {
            const std::uint16_t motion = SelectNpcSpecialMotion(entry, true);
            if (!motion) {
                SetMotionSpeed(work, 0.0f, MotionRegion::LowerBody);
                entry.specialMotionPhase = 0;
            } else if (motion != GetMotionAnimationId(work, MotionRegion::LowerBody)) {
                if (entry.previousLiveState != 1)
                    shenApplyTranslationToAll(&entry.node->entity.node, entry.specialMotionSavedPosition);
                mtwkRequestMotion(work, motion, MotionRegion::LowerBody);
            }
        }
        return true;
    case 3: {
        UpdateMotionWorkWithPosition(work, &entry.specialMotionTarget, 1);
        if (entry.eyeControlProfile) {
            const std::int8_t saved = entry.eyeControlProfile;
            entry.eyeControlProfile = 0;
            HandleNpcEyeControl(entry);
            entry.eyeControlProfile = saved;
        }
        if (entry.stateContext.flags & 0x400U) {
            SetNpcRouteMotionState(entry.stateContext, 2);
            entry.specialMotionPhase = 0;
            return true;
        }
        if (entry.previousLiveState == 4 || IsPartyCompanionOnStairs(entry.name)) {
            entry.specialMotionPhase = 0;
            return true;
        }
        entry.specialMotionPhase = 2;
        const Vector3 position = GetCharacterWorldPosition(entry.node);
        const Vector3i rotation = GetCharacterWorldRotation(entry.node);
        std::int16_t difference = static_cast<std::int16_t>(NpcTargetYaw(position, entry.specialMotionTarget) - rotation.y);
        if (entry.previousLiveState == 8) {
            std::uint32_t flags = GetNpcAreaSpecialMotionFlags(entry);
            if (flags & 1U) {
                const std::uint32_t transition = SelectNpcAreaTransitionMotion(entry);
                if (const std::uint16_t motion = static_cast<std::uint16_t>(transition)) {
                    if (motion != GetMotionAnimationId(work, MotionRegion::LowerBody))
                        mtwkRequestMotion(work, motion, MotionRegion::LowerBody);
                    flags = transition >> 16;
                }
                if (flags & 4U) {
                    UpdateMotionWorkWithPosition(work, &entry.specialMotionTarget, 0);
                } else {
                    UpdateMotionWorkWithPosition(work, &entry.specialMotionTarget, 1);
                    if (flags & 8U)
                        UpdateMotionWorkWithPosition(work, nullptr, 7);
                    else if (flags & 0x70U) {
                        if (difference < 0) {
                            UpdateMotionWorkWithPosition(work, nullptr, (flags & 0x10U) ? 13 : 12);
                            if (flags & 0x20U)
                                UpdateMotionWorkWithPosition(work, nullptr, 10);
                        } else {
                            UpdateMotionWorkWithPosition(work, nullptr, (flags & 0x10U) ? 10 : 9);
                            if (flags & 0x20U)
                                UpdateMotionWorkWithPosition(work, nullptr, 13);
                        }
                    }
                }
                entry.specialMotionPhase = 0;
                return true;
            }
        }
        std::uint16_t motion = entry.previousLiveState == 8 ? static_cast<std::uint16_t>(SelectNpcAreaTransitionMotion(entry)) : 0;
        if (motion)
            entry.specialMotionPhase = 0;
        if (entry.field414)
            return true;
        entry.field414 = 1;
        if (entry.stateContext.flags & 1U) {
            Vector3 origin = position;
            origin.y -= 0.22499999f;
            std::int32_t yaw;
            if ((entry.stateContext.flags & 0x10000U) || flddQueryStairOrigin(origin, origin, yaw))
                difference = 0;
        }
        const std::int16_t turn = NpcExcessTurn(difference);
        if (!motion)
            motion = SelectNpcSpecialMotion(entry, false);
        if (!motion) {
            SetMotionSpeed(work, 0.0f, MotionRegion::LowerBody);
            entry.specialMotionPhase = 0;
            return true;
        }
        if (turn) {
            entry.streamingCountdown = static_cast<std::uint16_t>((entry.streamingCountdown & 0xffU) | (entry.specialMotionPhase << 8));
            entry.specialMotionPhase = 4;
            UpdateMotionWorkWithPosition(work, nullptr, 0);
            entry.counter416 = 0;
        } else if (motion == GetMotionAnimationId(work, MotionRegion::LowerBody)) {
            return true;
        }
        CopyMotionTransition(work, nullptr, static_cast<std::uint16_t>(turn + rotation.y), 13, 0);
        SetMotionTimeRelated(work, 1, 5, motion);
        SetMotionTimeRelated(work, 1, 0, motion);
        return true;
    }
    case 4:
        if (!mtwkSetExtraFlag(work, 58, 1) || ++entry.counter416 > 8) {
            entry.specialMotionPhase = static_cast<std::uint8_t>(entry.streamingCountdown >> 8);
            UpdateMotionWorkWithPosition(work, &entry.specialMotionTarget, 1);
        }
        return true;
    case 5:
        if (!(entry.stateContext.flags & 0x100U)) {
            entry.specialMotionPhase = 0;
            if (entry.previousLiveState != 4 && !IsPartyCompanionOnStairs(entry.name)) {
                const Vector3i rotation = GetCharacterWorldRotation(entry.node);
                const std::uint16_t yaw = static_cast<std::uint16_t>(NpcEulerYaw(entry.euler));
                if (static_cast<std::uint16_t>(rotation.y) != yaw)
                    CopyMotionTransition(work, nullptr, yaw, 13, 0);
            }
        }
        return true;
    case 6: {
        entry.specialMotionPhase = 0;
        const bool left = HasMotionTrackCommand(work, MotionRegion::RightArm);
        const bool right = HasMotionTrackCommand(work, MotionRegion::LeftArm);
        if (left && right) {
            UpdateMotionWorkWithPosition(work, &entry.specialMotionTarget, 0);
            return true;
        }
        RequestMotionTargetFacing(work, &entry.specialMotionTarget, left ? 2 : right ? 4 : 0);
        if (entry.previousLiveState == 4 || entry.previousLiveState == 8 || IsPartyCompanionOnStairs(entry.name) || (entry.stateContext.flags & 0x400U))
            return true;
        const Vector3 position = GetCharacterWorldPosition(entry.node);
        const Vector3i rotation = GetCharacterWorldRotation(entry.node);
        std::int16_t difference = static_cast<std::int16_t>(NpcTargetYaw(position, entry.specialMotionTarget) - rotation.y);
        if (entry.stateContext.flags & 1U) {
            Vector3 origin = position;
            origin.y -= 0.22499999f;
            std::int32_t yaw;
            if (flddQueryStairOrigin(origin, origin, yaw))
                difference = 0;
        }
        if (entry.stateContext.flags & 0x10000U)
            difference = 0;
        CopyMotionTransition(work, nullptr, static_cast<std::uint16_t>(NpcExcessTurn(difference) + rotation.y), 13, 0);
        const std::uint16_t motion = SelectNpcSpecialMotion(entry, false);
        if (motion) {
            SetMotionTimeRelated(work, 1, 5, motion);
            SetMotionTimeRelated(work, 1, 0, motion);
        } else
            SetMotionSpeed(work, 0.0f, MotionRegion::LowerBody);
        return true;
    }
    case 7:
        entry.specialMotionPhase = 0;
        RequestMotionTargetFacing(work, nullptr, 0);
        return true;
    case 10:
        if (entry.stateContext.flags & 0x8000U)
            ReleaseNpcSpecialAnchor(entry);
        if (entry.eyeControlProfile)
            HandleNpcEyeControl(entry);
        SetMotionTimeRelated(work, 0, 0, 0);
        if (entry.name.name == SHEN_NAME(REN_))
            SetMotionTimeRelated(work, 1, 0, 0xa204);
        SetMotionSpeed(work, entry.motionSpeed, MotionRegion::LowerBody);
        if (entry.previousLiveState == 1) {
            CompleteNpcSpecialMotion(entry);
            SetNpcRouteMotionState(entry.stateContext, 3, false);
            return false;
        }
        if (entry.previousLiveState == 4 || entry.previousLiveState == 8) {
            CompleteNpcSpecialMotion(entry);
            entry.commandFlags &= ~0x40U;
            RestoreNpcAreaMotion(entry);
            return false;
        }
        if (entry.previousLiveState == 5) {
            const Vector3 position = NpcRouteAreaToScene(entry.worldPosition);
            entry.specialMotionPhase = 12;
            CopyMotionTransition(work, &position, static_cast<std::uint16_t>(NpcEulerYaw(entry.euler)), 6, 0);
            return true;
        }
        entry.specialMotionPhase = 11;
        CopyMotionTransition(work, &entry.specialMotionSavedPosition, static_cast<std::uint16_t>(NpcEulerYaw(entry.euler)), 6, 0);
        SetMotionTimeRelated(work, 0, 6, 0);
        SetMotionTimeRelated(work, 1, 5, entry.currentMotionId);
        if (GetCharacterScenarioName(entry.node).name == SHEN_NAME(REN_))
            SetMotionTimeRelated(work, 1, 0, 0xa204);
        SetMotionRegionEvalTime(work, entry.animationFrame, MotionRegion::LowerBody);
        return true;
    case 11:
    case 12:
        if (mtwkSetExtraFlag(work, 58, 1))
            return true;
        if (entry.specialMotionPhase == 12)
            ResumeNpcScheduleMotion(entry);
        return CompleteNpcSpecialMotion(entry);
    default:
        return true;
    }
}

void PublishNpcRouteLiveEntries(std::span<NpcRouteLiveEntry> entries) {
    npcRouteLiveEntries = entries;
    for (NpcRouteLiveEntry &entry : npcRouteLiveEntries) {
        entry.node = FindCharacterByScenarioName(entry.name);
        entry.motionWork = entry.node ? FindMotionWork(entry.node) : nullptr;
        entry.stateContext.owner = &entry;
        entry.stateContext.motionWork = entry.motionWork;
    }
}

void UnpublishNpcRouteLiveEntries() {
    npcRouteLiveEntries = {};
}

void ResetNpcRouteLiveEntries() {
    UnpublishNpcRouteLiveEntries();
    // init_search_ctrl resets the map-local candidate, not g_follow_chara
    // or its retained grace counter (scenarioFollowCandidateIndex / npcFollowGraceCounter).
    scenarioFollowCandidateIndex = -1;
}

void BindNpcRouteCharacter(shen_name name, Character *character) {
    if (!character)
        return;
    for (NpcRouteLiveEntry &entry : npcRouteLiveEntries) {
        if (entry.name.name == name.name) {
            entry.node = character;
            entry.motionWork = FindMotionWork(character);
            entry.stateContext.motionWork = entry.motionWork;
        }
    }
}

void UnbindNpcRouteCharacter(const Character *character) {
    if (!character)
        return;
    for (NpcRouteLiveEntry &entry : npcRouteLiveEntries) {
        if (entry.node == character) {
            entry.node = nullptr;
            entry.motionWork = nullptr;
            entry.stateContext.motionWork = nullptr;
        }
    }
}

NpcRouteStateContext *FindNpcRouteStateContext(shen_name character) {
    if (!npcIsNpcTaskActive())
        return nullptr;
    for (NpcRouteLiveEntry &entry : npcRouteLiveEntries) {
        if (entry.name.name == character.name)
            return &entry.stateContext;
    }
    return nullptr;
}

void npcReleaseSpecialAnchor(shen_name character) {
    if (NpcRouteStateContext *const context = FindNpcRouteStateContext(character))
        ReleaseNpcSpecialAnchor(*context->owner);
}

bool IsNpcRouteReady(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context && (context->flags & 0x103U) == 3U;
}

bool IsNpcRouteActive(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context && (context->flags & 1U) != 0;
}

bool HasNpcFreeTalkFlag(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context && (static_cast<std::uint8_t>(context->owner->recordBase[0x0b]) & 4U) != 0;
}

bool InitializeNpcSpecialRouteDirection(NpcRouteLiveEntry &entry) {
    if (!(entry.stateContext.flags & 0x200U) || !(entry.flags & 0x10U))
        return false;
    std::int32_t initialPoint;
    std::memcpy(&initialPoint, entry.recordBase + 0x0c, sizeof(initialPoint));
    const std::int8_t direction = static_cast<std::int16_t>(entry.stateContext.pendingAreaPoint) == initialPoint ? 4 : 2;
    if (entry.field338 & 0x80U)
        entry.eyeControlMode = direction;
    else
        entry.routeDirectionMode = direction;
    return true;
}

std::int32_t AcquireNpcRuntimeEntryForScript(shen_name character, NpcRouteLiveEntry **output) {
    NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    if (!context || !context->owner->node)
        return 0;
    NpcRouteLiveEntry &entry = *context->owner;
    *output = &entry;
    if ((entry.flags & 0x4010U) == 0x10U)
        return -1;
    entry.flags |= 0x10U;
    if (entry.flags & 0x4000U) {
        entry.flags &= ~0x4000U;
        entry.liveState = entry.previousLiveState;
    } else {
        const float frame = static_cast<float>(static_cast<std::int16_t>(static_cast<std::int32_t>(GetMotionFrame(entry.motionWork, MotionRegion::LowerBody))));
        std::memcpy(reinterpret_cast<std::byte *>(&entry) + 0x418, &frame, sizeof(frame));
        entry.specialMotionSavedPosition = GetCharacterWorldPosition(entry.node);
    }
    entry.previousLiveState = entry.liveState;
    InitializeNpcSpecialRouteDirection(entry);
    return 1;
}

std::int32_t GetNpcRouteCurrentParameter(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context ? context->parameter : 0;
}

std::int32_t GetNpcRouteStride(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context ? context->stride : 0;
}

std::int32_t GetNpcRouteLowParameter(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context ? context->lowParameter : 0;
}

std::int32_t GetNpcRouteHighParameter(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context ? context->highParameter : 0;
}

std::int32_t GetNpcRouteMaximumParameter(shen_name character) {
    const NpcRouteStateContext *const context = FindNpcRouteStateContext(character);
    return context ? (static_cast<std::int32_t>(context->pointCount) - 3) * 64 : 0;
}

void PauseNpcRouteMotion(NpcRouteStateContext *context, shen_name character) {
    if (!context)
        context = FindNpcRouteStateContext(character);
    if (!context || !(context->flags & 1U))
        return;
    if (!(context->flags & 0x1000U))
        context->routeFlags |= 0x800U;
    NpcRouteLiveEntry &entry = *context->owner;
    if ((entry.flags & NpcRouteStreamForceHidden) && entry.node) {
        std::uint32_t yaw;
        std::memcpy(&yaw, &context->savedEuler.y, sizeof(yaw));
        CopyMotionTransition(entry.motionWork, nullptr, 0, 1, 0);
        CopyMotionTransition(entry.motionWork, nullptr, static_cast<std::uint16_t>(yaw), 13, 0);
        CopyMotionTransition(entry.motionWork, nullptr, 0, 2, 0);
    }
}

// @verified - p
std::int8_t EV_SetCharaControllerFlags(shen_name character, std::uint32_t flags, NpcCharacterControllerState state) {
    Character *const node = ctrlGetCharaByName(character);
    CharacterControlModel *const control = FindCharacterControlModel(node);
    if (!control)
        return false;

    std::uint32_t mask{};
    switch (flags) {
    case 0:
        mask = CharacterControlUpdateHeight;
        break;
    case UpdateHeight:
        mask = CharacterControlApplyGravity;
        break;
    case ApplyGravity:
        mask = state == NpcCharacterControllerState::Enabled ? CharacterControlEvaluateMotion | CharacterControlSnapToGround | CharacterControlFineRaycast :
                                                               CharacterControlEvaluateMotion;
        break;
    case UpdateHeight | ApplyGravity:
        mask = CharacterControlVoxelSweep;
        break;
    case EvaluateMotion | SolverAssist:
        mask = 0x00001000U;
        break;
    case UpdateHeight | Unknown10:
        mask = 0x00020000U;
        break;
    default:
        break;
    }

    if (state == NpcCharacterControllerState::Current) {
        return (control->flags & mask) != 0;
    }
    if (state == NpcCharacterControllerState::Disabled) {
        control->flags &= ~mask;
    } else if (state == NpcCharacterControllerState::Enabled) {
        control->flags |= mask;
    }
    return false;
}

void EV_SetRefBoardParamsFor(shen_name character, std::int32_t sector) {
    ResetReflectorBoardMode(FindCharacterByScenarioName(character), sector);
}

void SetScenarioFollowCharacter(shen_name character) {
    scenarioFollowCharacter = character;
    scenarioFollowGraceFrames = 90;
}

shen_name GetScenarioFollowCharacter() {
    return scenarioFollowCharacter;
}

std::int32_t GetScenarioFollowGraceFrames() {
    return scenarioFollowGraceFrames;
}
void SetScenarioFollowGraceFrames(std::int32_t frames) {
    scenarioFollowGraceFrames = frames;
}

std::int32_t GetScenarioFollowCandidateIndex() {
    return scenarioFollowCandidateIndex;
}

void SetScenarioFollowCandidateIndex(std::int32_t index) {
    scenarioFollowCandidateIndex = index;
}

void UpdateScenarioFollowCharacter(bool characterPresent) {
    if (scenarioFollowCharacter.name == 0 || characterPresent || scenarioFollowGraceFrames <= 0) {
        return;
    }
    --scenarioFollowGraceFrames;
    if (scenarioFollowGraceFrames == 0)
        scenarioFollowCharacter = {};
}

} // namespace shenmue::s2
