#include "shenmue.h"
#include "common/native_sqrt.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/fight_stage.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/math.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/modifier_vertices_dc.hpp"
#include <cstring>
#include <cmath>
#else
#include "s2/fight_stage.hpp"
#include "s2/fight.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/model_component.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/map_control.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/camera.hpp"

#include <bit>
#include <cstring>
#include <cmath>
#include <algorithm>
#if SHENMUE_IS_PC
#include <emmintrin.h>
#endif
#include "s2/area_names.hpp"
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t g_vfSelectedReactionAnimation{};

namespace {

constexpr VFCollisionShape bodyShapes[10]{{{-.055f, .015f, .005f}, .295f, 3, 0, 2}, {{.025f, 0, .05f}, .156f, 5, 0, 4}, {{.135f, .02f, -.01f}, .18f, 9, 0, 8},
    {{.23f, -.005f, -.04f}, .173f, 12, 0, 16}, {{.135f, .02f, .01f}, .18f, 16, 0, 64}, {{.23f, -.005f, .04f}, .173f, 19, 0, 128},
    {{.225f, 0, 0}, .213f, 23, 0, 1024}, {{.305f, .035f, -.005f}, .232f, 24, 0, 2048}, {{.225f, 0, 0}, .213f, 28, 0, 8192},
    {{.305f, .035f, .005f}, .232f, 29, 0, 16384}};

constexpr VFCollisionShape togyuBodyShapes[10]{{{-.155f, .005f, .025f}, .489f, 3, 0, 2}, {{.015f, 0, .05f}, .211f, 5, 0, 4},
    {{.15f, -.03f, -.055f}, .267f, 9, 0, 8}, {{.33f, -.01f, .01f}, .224f, 12, 0, 16}, {{.15f, -.03f, .055f}, .267f, 16, 0, 64},
    {{.33f, -.01f, -.01f}, .224f, 19, 0, 128}, {{.31f, .025f, -.005f}, .325f, 23, 0, 1024}, {{.305f, .035f, -.005f}, .311f, 24, 0, 2048},
    {{.31f, .025f, .005f}, .325f, 28, 0, 8192}, {{.305f, .035f, .005f}, .311f, 29, 0, 16384}};

constexpr VFCollisionShape pushShapes[3]{{{.04f, -.02f, 0}, .4f, 3, 0, 2}, {{.3f, 0, 0}, .25f, 24, 0, 2048}, {{.3f, 0, 0}, .25f, 29, 0, 16384}};

constexpr VFCollisionShape togyuPushShapes[3]{{{-.05f, -.145f, 0}, .745f, 3, 0, 2}, {{.34f, 0, 0}, .392f, 24, 0, 2048}, {{.34f, 0, 0}, .392f, 29, 0, 16384}};

struct HitProfile {
    float attackScale[2], impulseScale[4];
    std::int16_t angles[4];
};

static_assert(sizeof(HitProfile) == 32);

constexpr HitProfile hitProfiles[]{{.attackScale = {1, 1}, .impulseScale = {1, 1.2f, 1, 1.2f}, .angles = {11776, 6144, 9728, 4864}},
    {.attackScale = {1, 1}, .impulseScale = {1.5f, 1.8f, 1.2f, 1.44f}, .angles = {13824, 9216, 12800, 6400}},
    {.attackScale = {1, 1}, .impulseScale = {2, 5, 1.2f, 1.44f}, .angles = {12288, 14336, 11264, 6144}},
    {.attackScale = {1, 1}, .impulseScale = {2.5f, 2.5f, 2.5f, 2.5f}, .angles = {14592, 14592, 14592, 14592}},
    {.attackScale = {1, 1}, .impulseScale = {1.3f, 1.2f, 1.2f, 1.2f}, .angles = {11776, 4096, 9728, 4864}},
    {.attackScale = {1, 1}, .impulseScale = {3.8f, 3.8f, 3.8f, 3.8f}, .angles = {9216, 9216, 9216, 9216}},
    {.attackScale = {1, 1}, .impulseScale = {2.5f, 2.5f, 2.5f, 2.5f}, .angles = {12800, 12800, 12800, 12800}}};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

const VFCollisionShape *PushShapes(const VFActor *actor);
void MoveReadyEnemy(FCMG_t *group, float x, float z);
std::int32_t StyleAnimation(const VFActor &actor, std::size_t offset);
template <bool ActiveOpponent> Vector3 ReadyContactDisplacement(Vector3 separation, Vector3 history);
void ResolveReadyActive(VFCLParticipantTmp &ready, VFCLParticipantTmp &active);
void ResolveReadyReady(VFCLParticipantTmp &first, VFCLParticipantTmp &second);
const Vector3 &ParticipantPosition(const VFCLParticipantTmp &participant);
bool PairBlockedByWorld(const VFActor *a, const VFActor *b);

} // namespace

void vfSetupHitReactionAnim(VFActor *self, VFActor *other, std::uint16_t animation);
void vfclChooseReactionAnim(VFActor *source, VFActor *destination);
void vfclTransformContactShapes(VFActor *self);
void vfInitializeActorCollision(VFMA_t *game, std::uint8_t index);
void vfclMoveEntityWithContactHistory(VFActor *self, float x, float z);
void vfclAdjustToGroundOrSlope(VFCL_t *collision);
void vfclUpdateHeightFlagsForGrid(VFActor *self);
void vfClCopyMotionData(VFMA_t *game);
void vfclEnforceRingBoundary(VFCL_t *collision);
void vfclResolvePairedContactDrift(VFMA_t *game);
void vfClRaycastInWorldGrid();
void vfclComputePenetrationAndOverlap(VFCL_t *collision, VFActor *defender, VFActor *attacker);
void vfclAdjustForSlideAndStick(VFCL_t *collision);
void vfclSetContactCategory(VFActor *a, VFActor *b, VFCL_t *collision);
void vfclPrepareContact(VFCL_t *collision, std::uint8_t first, std::uint8_t second);
void vfClGetBonePos(std::uint8_t index, VFActor *first, VFActor *second);
bool vfClGetBoneStates(std::uint8_t first, std::uint8_t second, VFCL_t *collision);
float vfResolveHitOutcomeAndImpulse(VFActor *self, VFActor *other, float impact);
void vfApplyEnvAudio(float impact, VFActor *self);
void vfSetReactionKnockbackFromProfile(VFActor *self, VFActor *other, float impulse);
void vfStageObjectApplyImpulse(VFActor *self);
void vfAssignKnockbackReaction(float impact, float impulse, VFActor *self, VFActor *other);
void vfclApplyHitResolution(VFActor *self, VFActor *other);
void vfRob_ResolveHitClash(VFCL_t *collision, std::uint8_t first, std::uint8_t second);
void vfBuildContactsAndResolvePushOrHit(VFCL_t *collision, VFCLParticipantTmp *participants, std::int32_t count, std::uint8_t phase);
void vfc_VF_RobCollision_Callback(void *parameter);
HLib::Task *Queue_vfc_VF_RobCollision(VFMA_t *game);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

template <class T> T ReadFightData(const void *owner, std::size_t offset) {
    T result;
    std::memcpy(&result, static_cast<const std::byte *>(owner) + offset, sizeof(result));
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t GetMoveProficiency(std::uint8_t category, std::uint8_t move);

std::int32_t g_vfSelectedReactionAnimation;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float GetMoveProficiencyScale(std::uint8_t move);

float vfclCalculateGuardKnockbackFactor(FightActorRuntimePrefix *actor, float) {
    const std::byte *data = reinterpret_cast<const std::byte *>(actor);
    if (*reinterpret_cast<const std::int8_t *>(data) != *(reinterpret_cast<const std::int8_t *>(actor->game) + 0x74))
        return 1.0f;
    const std::int8_t *const bytes = reinterpret_cast<const std::int8_t *>(data);
    std::int8_t level;
    if (*reinterpret_cast<const std::uint32_t *>(data + 0x2e8) & 0x40000U)
        level = bytes[0x881];
    else
        level = bytes[0x87f] == 96 ? bytes[0x880] : bytes[0x87f];
    return GetMoveProficiencyScale(level);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void EmitFightSmoke(std::uint8_t profile, FightActorRuntimePrefix *actor);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

template <class T> static inline T &FightCollisionValue(void *actor, std::size_t offset) {
    return *reinterpret_cast<T *>(static_cast<std::byte *>(actor) + offset);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static inline std::int32_t FightStyleAnimation(FightActorRuntimePrefix *actor, std::size_t offset) {
    const std::byte *const *data = reinterpret_cast<const std::byte *const *>(reinterpret_cast<const std::byte *>(actor) + 0x38);
    const std::int32_t low = static_cast<std::uint8_t>((*data + offset)[1]);
    return low | static_cast<std::int32_t>(static_cast<std::int8_t>((*data)[offset])) << 8;
}

static std::int32_t RandomReactionVariant(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target) {
    static const std::uint16_t primarySlots[6] = {78, 80, 82, 84, 86, 88};
    static const std::uint16_t secondarySlots[2] = {90, 92};
    const std::uint8_t guard = FightCollisionValue<std::uint8_t>(source, 0x2a9);
    const std::uint16_t *slots;
    std::int32_t index;
    if (guard == 3 || guard == 0 || guard == 1) {
        index = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 6.0f);
        slots = primarySlots;
    } else {
        index = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 2.0f);
        slots = secondarySlots;
    }
    return FightStyleAnimation(target, slots[static_cast<std::uint8_t>(index)]);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void vfSetupHitReactionAnim(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target, std::uint16_t animation) {
    if (!animation)
        return;
    if (!(source->stateFlags & 12U))
        return;
    const std::uint16_t *response = 0;
    const std::uint8_t category = vfSelectHitReactionSequence(source);
    const std::uint16_t *record;
    for (record = reinterpret_cast<const std::uint16_t *>(g_fallbackMotionSection3); record[0]; record += 7) {
        if (animation == record[0] || animation == record[1]) {
            response = record + 4;
            break;
        }
    }
    if (!response)
        return;
    if ((FightCollisionValue<std::uint32_t>(source, 0x2ec) & 0x4000008U) || (category & 0x33U))
        ++response;
    if (!(FightCollisionValue<float>(target, 0x438) > static_cast<float>(FightCollisionValue<std::uint16_t>(target, 0x290)))) {
        target->stateFlags |= 0x800U;
        std::uint8_t mode = 0;
        std::uint8_t particle = 0;
        const std::uint8_t guard = FightCollisionValue<std::uint8_t>(source, 0x2a9);
        ++response;
        if ((category & 16U) && (guard == 2 || guard == 5 || guard == 6)) {
            particle = 9;
            mode = 7;
        } else if ((category & 32U) && (guard == 2 || guard == 5 || guard == 6)) {
            particle = 8;
            mode = 6;
        } else if (category & 0x28U) {
            particle = 6;
            mode = 4;
        } else if (category & 0x14U) {
            particle = 7;
            mode = 5;
        }
        if (mode) {
            figpAssignModeForCharacter(target->character, mode);
            figpAssignMotScaleAndDirFor(target->character, 20, 1);
        }
        if (particle)
            EmitFightSmoke(particle, target);
    }
    std::uint16_t reaction = *response;
    if (!reaction)
        reaction = record[5];
    if (FightCollisionValue<std::uint16_t>(source, 0x3a6))
        reaction = FightCollisionValue<std::uint16_t>(source, 0x3a6);
    if (source->stateFlags & 8U)
        animation = record[2];
    else if ((source->stateFlags & 4U) && !(FightCollisionValue<std::uint32_t>(source, 0x2ec) & 0x4000008U) && (category & 12U))
        animation = record[1];
    g_vfSelectedReactionAnimation = animation;
    if (static_cast<std::int32_t>(FightCollisionValue<std::uint8_t>(target, 0x3a5) - FightCollisionValue<std::uint8_t>(source, 0x3a5)) < 50 ||
        !(category & 12U) || (FightCollisionValue<std::uint32_t>(source, 0x2ec) & 0x4000008U) || (target->stateFlags & 0x800U) ||
        FightCollisionValue<std::uint8_t>(source, 0x305) == 3) {
        FightCollisionValue<std::uint16_t>(source, 0x27e) = 30;
        FightCollisionValue<std::uint16_t>(source, 0x3ca) = reaction;
        source->moveFlags &= ~0x440U;
        source->hitStateFlags &= ~0x200000U;
        source->moveFlags = (target->moveFlags & 0x440U) | source->moveFlags;
        source->hitStateFlags = (target->hitStateFlags & 0x200000U) | source->hitStateFlags;
    }
}

#else

// Matching FBMOTTBL records retain their borrowed seven-word layout.
void vfSetupHitReactionAnim(VFActor *self, VFActor *other, std::uint16_t animation) {
    if (!animation || !(self->stateFlags & (VFSF_Unknown4 | VFSF_Unknown8)))
        return;
    const auto category = vfSelectHitReactionSequence(self);
    const auto *record = reinterpret_cast<const std::byte *>(g_fallbackMotionSections[3]);
    for (;;) {
        const auto first = ReadFightData<std::uint16_t>(record, 0);
        if (!first)
            return;
        if (animation == first || animation == ReadFightData<std::uint16_t>(record, 2))
            break;
        record += 14;
    }
    std::size_t selection = ((self->secondaryMotionFlags & (VFSMF_Unknown8 | VFSMF_Unknown4000000)) || (category & 0x33)) ? 10 : 8;
    if (float(static_cast<std::uint16_t>(other->parameter2d0)) >= other->animationTime) {
        other->stateFlags |= VFSF_Unknown800;
        selection += 2;
        const auto guard = static_cast<std::uint8_t>(self->guardState);
        const bool specialGuard = guard == 2 || std::uint32_t(guard - 5) <= 1;
        std::uint8_t particle = 0;
        std::int8_t mode = 0;
        if ((category & 0x10) && specialGuard) {
            particle = 9;
            mode = 7;
        } else if ((category & 0x20) && specialGuard) {
            particle = 8;
            mode = 6;
        } else if (category & 0x28) {
            particle = 6;
            mode = 4;
        } else if (category & 0x14) {
            particle = 7;
            mode = 5;
        }
        if (particle) {
            const auto *figure = static_cast<FigurePose *>(
#if SHENMUE_IS_DC
charFindComp
#else
FindModelComponent
#endif
(other->character, MODEL_FIGURE_POSE));
            if (!figure || !(figure->flags & 1)) {
                figpAssignModeForCharacter(other->character, mode);
                figpAssignMotScaleAndDirFor(other->character, 20, 1);
            }
            EmitFightSmoke(particle, other);
        }
    }
    auto response = ReadFightData<std::uint16_t>(record, selection);
    if (!response)
        response = ReadFightData<std::uint16_t>(record, 10);
    if (self->parameter412)
        response = self->parameter412;
    if (self->stateFlags & VFSF_Unknown8)
        animation = ReadFightData<std::uint16_t>(record, 4);
    else if ((self->stateFlags & VFSF_Unknown4) && !(self->secondaryMotionFlags & (VFSMF_Unknown8 | VFSMF_Unknown4000000)) && (category & 0xc))
        animation = ReadFightData<std::uint16_t>(record, 2);
    g_vfSelectedReactionAnimation = animation;
    if (int(static_cast<std::uint8_t>(other->guardLevel)) - int(static_cast<std::uint8_t>(self->guardLevel)) < 50 || !(category & 0xc) ||
        (self->secondaryMotionFlags & (VFSMF_Unknown8 | VFSMF_Unknown4000000)) || (other->stateFlags & VFSF_Unknown800) || self->parameter34d == 3) {
        self->moveFlags &= ~(VFMF_MirrorReactionAngle | VFMF_MirroredStance);
        self->hitStateFlags &= ~VFHSF_MirroredReactionAngle;
        self->queuedReactionEvent = 30;
        self->parameter43e = response;
        self->moveFlags |= other->moveFlags & (VFMF_MirrorReactionAngle | VFMF_MirroredStance);
        self->hitStateFlags |= other->hitStateFlags & VFHSF_MirroredReactionAngle;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfclApplyHitResolution(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target);

static std::uint16_t SelectStyleReactionAnimation(FightActorRuntimePrefix *source, std::uint32_t index);

static std::int32_t RandomReactionVariant(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target);

static void vfAdjustReactionForGround(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target);

void vfclApplyHitResolutionForSlots(VFCL_t *collision, std::uint8_t self, std::uint8_t other) {
    vfclApplyHitResolution(collision->game->actors[self], collision->game->actors[other]);
}

// @verified - d
void vfclChooseReactionAnim(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target) {
    const std::uint32_t hit = target->hitStateFlags;
    const std::uint16_t parameter = FightCollisionValue<std::uint16_t>(source, 0x2b0);
    const std::int16_t direction = FightCollisionValue<std::int16_t>(source, 0x2b2);
    vfSelectHitReactionSequence(source);
    std::uintptr_t table = g_fallbackMotionSection0[static_cast<std::uint8_t>(parameter)];
    table += reinterpret_cast<std::uintptr_t>(GetFallbackMotionTable());
    const std::int32_t reactionClass = g_vfSelectedReactionAnimation;
    if (reactionClass == 9) {
        if (hit & 0x800U)
            g_vfSelectedReactionAnimation = 108;
        else
            g_vfSelectedReactionAnimation = 107;
        return;
    }
    if (g_vfSelectedReactionAnimation == 8 && static_cast<std::uint16_t>(FightCollisionValue<std::int16_t>(target, 2)) == 8) {
        std::size_t slot = 78;
        switch (FightCollisionValue<std::uint8_t>(target, 0x2a9)) {
        case 1:
            slot = 80;
            break;
        case 2:
        case 5:
        case 6:
            slot = 90;
            break;
        case 0:
        case 3:
        case 4:
            slot = 78;
            break;
        }
        const std::byte *const *data = reinterpret_cast<const std::byte *const *>(reinterpret_cast<const std::byte *>(target) + 0x38);
        g_vfSelectedReactionAnimation = static_cast<std::uint8_t>((*data + slot)[1]) | static_cast<std::int32_t>(static_cast<std::int8_t>((*data)[slot])) << 8;
        return;
    }
    std::uint32_t facingSide = 0;
    if (hit & 0x1000U)
        table = reinterpret_cast<std::uintptr_t>(g_fallbackMotionSection2);
    std::uint32_t side = 0;
    if (parameter & 0x8000U)
        ++side;
    if (source->moveFlags & 0x40U)
        side ^= 1U;
    if (target->moveFlags & 0x400U)
        side ^= 1U;
    facingSide += side;
    std::int32_t angle = static_cast<std::int16_t>(FightCollisionValue<std::int16_t>(source, 0x2b2) + FightCollisionValue<std::int16_t>(source, 0x164) -
        static_cast<std::int16_t>(target->temporaryFacingAngle) + 0x4000);
    if (FightCollisionValue<std::uint32_t>(target, 0x33c) & 0x1000000U)
        angle = static_cast<std::int16_t>(angle + FightCollisionValue<std::int16_t>(source, 0x946));
    if ((hit & 0x4800U) == 0x4800U)
        angle ^= -32768;
    if (angle >= 0)
        facingSide += 2;
    std::uint32_t quadrant = 0;
    if (static_cast<std::uint16_t>(angle + 0x1fff) <= 0x3ffeU)
        quadrant = target->moveFlags & 0x40U ? 6 : 4;
    else if (static_cast<std::uint16_t>(angle + 0x9fff) <= 0x1ffeU)
        quadrant = target->moveFlags & 0x40U ? 4 : 6;
    if (direction)
        quadrant = 6;
    std::uint32_t index;
    if (!quadrant)
        index = g_vfSelectedReactionAnimation * 8 + facingSide;
    else
        index = side + g_vfSelectedReactionAnimation * 8 + quadrant;
    if (g_vfSelectedReactionAnimation == 2) {
        g_vfSelectedReactionAnimation = SelectStyleReactionAnimation(source, index);
    } else {
        g_vfSelectedReactionAnimation = reinterpret_cast<const std::uint16_t *>(table)[index];
        if (FightCollisionValue<std::uint16_t>(source, 0x2da) && (index & 7U) < 2)
            g_vfSelectedReactionAnimation = FightCollisionValue<std::uint16_t>(source, 0x2da);
        if (static_cast<std::uint16_t>(FightCollisionValue<std::int16_t>(target, 2)) == 14 && reactionClass != 8)
            g_vfSelectedReactionAnimation = RandomReactionVariant(source, target);
    }
    vfSetupHitReactionAnim(source, target, static_cast<std::uint16_t>(g_vfSelectedReactionAnimation));
    vfAdjustReactionForGround(source, target);
}

static std::uint16_t SelectStyleReactionAnimation(FightActorRuntimePrefix *source, std::uint32_t index) {
    static const std::uint32_t alternatives[3][2] = {{565, 566}, {566, 565}, {309, 33077}};
    std::uint8_t sub = 0;
    const std::uint16_t parameter = FightCollisionValue<std::uint16_t>(source, 0x2b0);
    std::uintptr_t table = g_fallbackMotionSection0[static_cast<std::uint8_t>(parameter)];
    table += reinterpret_cast<std::uintptr_t>(GetFallbackMotionTable());
    if (!FightCollisionValue<std::uint8_t>(source, 0x305))
        return reinterpret_cast<const std::uint16_t *>(table)[index];
    if (FightCollisionValue<std::uint32_t>(source, 0x354) & 16U)
        ++sub;
    return alternatives[reinterpret_cast<const std::uint8_t *>(source)[0x305] - 1][sub];
}

#else

// The shared selected-class word becomes the real authored animation.
void vfclChooseReactionAnim(VFActor *source, VFActor *destination) {
    const auto hit = destination->hitStateFlags;
    auto parameter = source->hitParameter;
    const auto direction = static_cast<std::int16_t>(source->hitDirection);
    (void)vfSelectHitReactionSequence(source);
    if (destination->moveFlags & 4) {
        parameter = 0;
        g_vfSelectedReactionAnimation = 7;
    }
    const auto offset = ReadFightData<std::uint32_t>(g_fallbackMotionSections[0], 4 * static_cast<std::uint8_t>(parameter));
    const auto *table = g_fallbackMotionTable + offset;
    const auto reactionClass = g_vfSelectedReactionAnimation;
    if (reactionClass == 9) {
        g_vfSelectedReactionAnimation = ((hit & 0x800) ? 1 : 0) + 107;
        if (destination->hitStateFlags & 0x8000)
            destination->facingAngleDelta = 0;
        return;
    }
    if (reactionClass == 8 && destination->fightStyle == 8) {
        const auto guard = static_cast<std::uint8_t>(destination->guardState);
        const auto slot = guard == 1 ? 80 : (guard == 2 || guard == 5 || guard == 6 ? 90 : 78);
        g_vfSelectedReactionAnimation = StyleAnimation(*destination, slot);
        return;
    }
    if ((hit & 0x1010) == 0x1010)
        table = reinterpret_cast<const std::byte *>(g_fallbackMotionSections[2]);
    auto side = parameter >> 15;
    if (source->moveFlags & VFMF_MirrorReactionAngle)
        side ^= 1;
    if (destination->moveFlags & 0x400)
        side ^= 1;
    auto angle = static_cast<std::int16_t>(source->hitDirection + source->facingAngle - destination->temporaryFacingAngle + 0x4000);
    if (destination->activeMotionFlags & 0x1000000)
        angle = static_cast<std::int16_t>(angle + std::bit_cast<std::uint16_t>(source->reactionFacingAngleDelta));
    if ((hit & 0x4800) == 0x4800)
        angle = static_cast<std::int16_t>(static_cast<std::uint16_t>(angle) ^ 0x8000);
    const auto facingSide = angle < 0 ? side : side + 2;
    std::uint16_t quadrant = 0;
    if (static_cast<std::uint16_t>(angle + 0x1fff) <= 0x3ffe)
        quadrant = ((destination->moveFlags >> 5) & 2) | 4;
    else if (static_cast<std::uint16_t>(angle) > 0x6000 && static_cast<std::uint32_t>(angle) < 0xa000)
        quadrant = (destination->moveFlags & 0x40) ? 4 : 6;
    if (direction)
        quadrant = 6;
    const auto index = std::uint32_t(quadrant ? side + quadrant : facingSide) + 8 * reactionClass;
    std::int32_t animation;
    if (reactionClass == 2) {
        // Native g_vfAlternateReactionAnimations: alternate reactions retain the interleaved zero words.
        constexpr std::uint16_t alternatives[18]{565, 0, 566, 0, 566, 0, 565, 0, 309, 0, 33077, 0, 0, 0, 968, 0, 29, 0};
        animation = source->parameter34d ? alternatives[4 * source->parameter34d - 4 + ((source->groundContactFlags >> 2) & 4) / 2] :
                                           ReadFightData<std::uint16_t>(g_fallbackMotionTable + offset, 2 * index);
        g_vfSelectedReactionAnimation = animation;
    } else {
        animation = ReadFightData<std::uint16_t>(table, 2 * index);
        g_vfSelectedReactionAnimation = animation;
        if (source->someAnimation[1] && (index & 7) < 2) {
            animation = source->someAnimation[1];
            g_vfSelectedReactionAnimation = animation;
        }
        if (destination->fightStyle == 14 && reactionClass != 8) {
            constexpr std::uint16_t bearSlots[8]{90, 92, 78, 80, 82, 84, 86, 88};
            const auto guard = static_cast<std::uint8_t>(source->guardState);
            const auto selected = (guard & 0xfc) || guard == 2 ? static_cast<std::uint8_t>(int(IncrementSharedPrngFloat() * 2)) :
                                                                 static_cast<std::uint8_t>(int(IncrementSharedPrngFloat() * 6)) + 2;
            animation = StyleAnimation(*destination, bearSlots[selected]);
            g_vfSelectedReactionAnimation = animation;
        }
    }
    vfSetupHitReactionAnim(source, destination, static_cast<std::uint16_t>(animation));
    if (g_vfSelectedReactionAnimation == 349 && double(source->targetGroundY) + 0.9 >= double(destination->contactPosition.y))
        g_vfSelectedReactionAnimation = double(source->targetGroundY) + 0.6 >= double(destination->contactPosition.y) ? 351 : 91;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

static VFCollisionShape bodyShapes[10] = {{{-.055f, .015f, .005f}, .295f, 3, 0, 2}, {{.025f, 0, .05f}, .156f, 5, 0, 4}, {{.135f, .02f, -.01f}, .18f, 9, 0, 8},
    {{.23f, -.005f, -.04f}, .173f, 12, 0, 16}, {{.135f, .02f, .01f}, .18f, 16, 0, 64}, {{.23f, -.005f, .04f}, .173f, 19, 0, 128},
    {{.225f, 0, 0}, .213f, 23, 0, 1024}, {{.305f, .035f, -.005f}, .232f, 24, 0, 2048}, {{.225f, 0, 0}, .213f, 28, 0, 8192},
    {{.305f, .035f, .005f}, .232f, 29, 0, 16384}};

static const VFCollisionShape togyuBodyShapes[10] = {{{-.155f, .005f, .025f}, .489f, 3, 0, 2}, {{.015f, 0, .05f}, .211f, 5, 0, 4},
    {{.15f, -.03f, -.055f}, .267f, 9, 0, 8}, {{.33f, -.01f, .01f}, .224f, 12, 0, 16}, {{.15f, -.03f, .055f}, .267f, 16, 0, 64},
    {{.33f, -.01f, -.01f}, .224f, 19, 0, 128}, {{.31f, .025f, -.005f}, .325f, 23, 0, 1024}, {{.305f, .035f, -.005f}, .311f, 24, 0, 2048},
    {{.31f, .025f, .005f}, .325f, 28, 0, 8192}, {{.305f, .035f, .005f}, .311f, 29, 0, 16384}};

} // namespace

VFCollisionShape pushShapes[3] = {{{.04f, -.02f, 0}, .4f, 3, 0, 2}, {{.3f, 0, 0}, .25f, 24, 0, 2048}, {{.3f, 0, 0}, .25f, 29, 0, 16384}};

namespace {

static const VFCollisionShape togyuPushShapes[3] = {
    {{-.05f, -.145f, 0}, .745f, 3, 0, 2}, {{.34f, 0, 0}, .392f, 24, 0, 2048}, {{.34f, 0, 0}, .392f, 29, 0, 16384}};

struct ActorContactState {
    std::uint32_t priorityReset[2];
    std::byte retained79c[4];
    std::uint8_t flags, linked;
    std::byte retained7a2[2];
    Vector3 previousSamples[10];
    std::uint32_t shapeMaskWords[10], activeShapeMask;
    std::byte retained848[8];
    float groundSamples[4];
};

typedef char ActorContactPreviousCheck[offsetof(ActorContactState, previousSamples) == 0x10 ? 1 : -1];

typedef char ActorContactMaskCheck[offsetof(ActorContactState, activeShapeMask) == 0xb0 ? 1 : -1];

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

ActorContactState &ContactState(FightActorRuntimePrefix *actor) {
    return *reinterpret_cast<ActorContactState *>(reinterpret_cast<std::byte *>(actor) + 0x794);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

const VFCollisionShape *BodyShapes(FightActorRuntimePrefix *actor) {
    return *reinterpret_cast<const VFCollisionShape **>(reinterpret_cast<std::byte *>(actor) + 0x9a8);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

const VFCollisionShape *PushShapes(const VFActor *actor) {
    return reinterpret_cast<const VFCollisionShape *>(actor->parameterAe0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

Vector3 ScaleToMagnitude(const Vector3 &input, float magnitude) {
    const float scale = magnitude / std::sqrt(input.x * input.x + input.y * input.y + input.z * input.z);
    return {input.x * scale, input.y * scale, input.z * scale};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint8_t ActorSlot(FightActorRuntimePrefix *actor) {
    return *reinterpret_cast<std::uint8_t *>(actor);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void MoveReadyEnemy(FCMG_t *group, float x, float z) {
    x = ::fabsf(x) < 0.0000099999997f ? 0.0f : x;
    z = ::fabsf(z) < 0.0000099999997f ? 0.0f : z;
    group->position.x += x;
    group->position.z += z;
    Vector3 root;
    mtwkGetRootPosition(group->motion, &root);
    root.x += x;
    root.z += z;
    mtwkSetRootPosition(group->motion, &root);
    SetCharacterWorldPosition(group->character, &group->position);
}

} // namespace

#else

namespace {

// A ready enemy retains its own FCMG root/position owner.
// @verified - p
void MoveReadyEnemy(FCMG_t *group, float x, float z) {
    if (std::fabs(x) < 0.0000099999997f)
        x = 0;
    if (std::fabs(z) < 0.0000099999997f)
        z = 0;
    group->position.x += x;
    group->position.z += z;
    auto root = mtwkGetRootPosition(group->motion);
    root.x += x;
    root.z += z;
    mtwkSetRootPosition(group->motion, root);
    shenApplyTranslationToAll(&group->character->entity.node, group->position);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t StyleAnimation(const VFActor &actor, std::size_t offset) {
    const auto *data = actor.motionParameters[4] + offset;
    const auto high = static_cast<std::int16_t>(std::uint16_t(std::to_integer<std::uint8_t>(data[0])) << 8);
    return int(high) | std::to_integer<std::uint8_t>(data[1]);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfBuildContactsAndResolvePushOrHit(VFCL_t *collision, VFCLParticipantTmp *participants, std::int32_t count, std::int32_t phase);

void vfclEnforceRingBoundary(VFCL_t *collision);

void vfclResolveWorldContacts(VFCL_t *collision);

void vfclAdjustToGroundOrSlope(VFCL_t *collision);

void vfclUpdateHeightFlagsForGrid(FightActorRuntimePrefix *actor);

void vfUpdateStageObjects(FightRuntimePrefix *game);

void vfclResolvePairedContactDrift(FightRuntimePrefix *game);

void vfClRaycastInWorldGrid();

void vfClCopyMotionData(FightRuntimePrefix *game);

void mtwkTranslateRigInterpolated(MotionWork *work, float x, float y, float z);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float vfclGetPushRadius(FightActorRuntimePrefix *actor) {
    const VFCollisionShape *shapes = *reinterpret_cast<const VFCollisionShape *const *>(reinterpret_cast<std::byte *>(actor) + 0x9ac);
    if (!shapes) {
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        shapes = scenGetCharaName(control->scenarioTask, actor->character) == SHEN_NAME(TGY_) ? togyuPushShapes : pushShapes;
    }
    float radius = 0;
    for (std::uint8_t index = 0; index < 3; ++index, ++shapes)
        if (shapes->radius > radius)
            radius = shapes->radius;
    return radius;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint32_t g_vfIntersectionTagSource;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::byte *GetMapRetainedCharacterState();

float FightIntersectionDistance(FightActorRuntimePrefix *actor, std::uint32_t yaw, float length) {
    FightFieldProbe probe;
    Vector3 origin;
    if (actor->moveFlags & 0x10000000U) {
        origin.x = actor->contactPosition.x;
        origin.y = actor->contactPosition.y;
        origin.z = actor->contactPosition.z;
    } else {
        origin.x = reinterpret_cast<const Vector3 *>(actor->retained74)->x;
        origin.y = reinterpret_cast<const Vector3 *>(actor->retained74)->y;
        origin.z = reinterpret_cast<const Vector3 *>(actor->retained74)->z;
    }
    probe.mode = 10;
    probe.fraction = 1.0e37f;
    probe.flags = (probe.flags & 0x1fffffffU) | 0x10000000U;
    const std::uint32_t angle = yaw + 0x4000U;
    probe.x = length * CalculateFixedCosine(angle);
    probe.z = -length * CalculateFixedSine(angle);
    probe.originX = origin.x;
    probe.originZ = origin.z;
    probe.radius = vfclGetPushRadius(actor) - 0.3f;
    probe.flags &= ~0x08000000U;
    probe.kind = 10;
    ResolveFightFieldProbe(&probe, reinterpret_cast<MapControlData *>(GetMapRetainedCharacterState()));
    float result;
    switch (static_cast<std::int32_t>(probe.flags >> 29U)) {
    case 1:
        result = probe.fraction;
        if (result < 1.0f) {
            if (result < 0)
                result = 0;
            else {
                result *= length;
                g_vfIntersectionTagSource = probe.tag;
            }
        } else
            result = length;
        break;
    case 0:
        result = length;
        break;
    default:
        result = length;
        break;
    case 2:
        result = 0;
        break;
    }
    if (result > 0)
        result -= 0.3f;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclTransformContactShapes(FightActorRuntimePrefix *actor) {
    std::int32_t index;
    Vector3 *sample = reinterpret_cast<Vector3 *>(reinterpret_cast<std::byte *>(actor) + 0x74);
    std::int32_t transformDisabled = 0;
    for (index = 0; index < 10; ++index, ++sample) {
        const Matrix4x4 *const bone =
            mtwkGetBoneTransform(actor->motion, (*reinterpret_cast<const VFCollisionShape *const *>(reinterpret_cast<std::byte *>(actor) + 0x9a8))[index].bone);
        if (!transformDisabled && bone) {
            matrix_stack::LoadMatrix(bone);
            const VFCollisionShape *const shapes = *reinterpret_cast<const VFCollisionShape *const *>(reinterpret_cast<std::byte *>(actor) + 0x9a8);
            Vector3 point;
            point.x = shapes[index].point.x;
            point.y = shapes[index].point.y;
            point.z = shapes[index].point.z;
            Matrix4x4::TransformPoint(point, *sample);
        }
    }
    Vector3 *contact = actor->game->collision->samples[*reinterpret_cast<const std::int8_t *>(actor)].points;
    const VFCollisionShape *push = *reinterpret_cast<const VFCollisionShape *const *>(reinterpret_cast<std::byte *>(actor) + 0x9ac);
    for (index = 0; index < 3; ++index, ++contact, ++push) {
        matrix_stack::LoadMatrix(mtwkGetBoneTransform(actor->motion, push->bone));
        Matrix4x4::TransformPoint(push->point, *contact);
    }
}

#else

// Native GetTransformAndTransformVector3: world samples are produced from the real motion bone transforms.
// @verified - p
void vfclTransformContactShapes(VFActor *self) {
    const auto *const shapes = reinterpret_cast<const VFCollisionShape *>(self->parameterAd8);
    for (std::size_t i = 0; i != 10; ++i) {
        const auto *const bone = mtwkGetBoneTransform(self->motion, shapes[i].bone);
        if (!bone)
            continue;
        Matrix4x4::SetActiveTransform(bone);
        Matrix4x4::TransformPoint(shapes[i].point, self->motionContactSamples[i]);
    }
    const auto *const pushes = PushShapes(self);
    for (std::size_t i = 0; i != 3; ++i) {
        Matrix4x4::SetActiveTransform(mtwkGetBoneTransform(self->motion, pushes[i].bone));
        Matrix4x4::TransformPoint(pushes[i].point, self->game->collision->samples[self->groupIndex].points[i]);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfInitializeActorCollision(FightRuntimePrefix *game, std::uint8_t index) {
    FightActorRuntimePrefix *const actor = game->actors[index];
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (scenGetCharaName(control->scenarioTask, actor->character) == SHEN_NAME(TGY_)) {
        *reinterpret_cast<const VFCollisionShape **>(reinterpret_cast<std::byte *>(actor) + 0x9a8) = togyuBodyShapes;
        *reinterpret_cast<const VFCollisionShape **>(reinterpret_cast<std::byte *>(actor) + 0x9ac) = togyuPushShapes;
    } else {
        *reinterpret_cast<const VFCollisionShape **>(reinterpret_cast<std::byte *>(actor) + 0x9a8) = bodyShapes;
        *reinterpret_cast<const VFCollisionShape **>(reinterpret_cast<std::byte *>(actor) + 0x9ac) = pushShapes;
    }
    matrix_stack::Matrix4x4_Push(0);
    vfclTransformContactShapes(actor);
    PopCurrentMatrix(1);
    for (std::uint8_t word = 0; word < 10; ++word)
        (&FightCollisionValue<std::uint32_t>(actor, 0x81c))[word] = 0;
    for (std::uint8_t sample = 0; sample < 4; ++sample)
        (&FightCollisionValue<float>(actor, 0x850))[sample] = 0.0f;
    *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x794) = 0;
    *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x798) = 0;
    FightCollisionValue<std::uint8_t>(actor, 0x7a0) = 0;
    FightCollisionValue<std::uint8_t>(actor, 0x7a1) = 0;
    FightCollisionValue<std::uint32_t>(actor, 0x844) = 0;
    FightCollisionRuntimePrefix *collision = game->collision;
    collision->ringMasks[index] = 0;
    collision->runtimeFlags = 0;
}

#else

// TOGYU's scenario identity chooses the authored geometry bank.
void vfInitializeActorCollision(VFMA_t *game, std::uint8_t index) {
    VFActor *const self = game->actors[index];
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const bool togyu = scenGetCharaName(control->scenarioTask, self->character).name == SHEN_NAME(TGY_);
    self->parameterAd8 = reinterpret_cast<const std::byte *>(togyu ? togyuBodyShapes : bodyShapes);
    self->parameterAe0 = reinterpret_cast<std::uintptr_t>(togyu ? togyuPushShapes : pushShapes);
    Matrix4x4_Push();
    vfclTransformContactShapes(self);
    Matrix4x4::Pop(1);
    for (std::uint64_t &pair : self->contact.shapeMaskPairs)
        pair = 0;
    self->contact.groundSamplePairs[0] = 0;
    self->contact.groundSamplePairs[1] = 0;
    self->contact.priorityResetPair = 0;
    self->contact.flags = std::uint16_t{0};
    self->contact.activeShapeMask = std::uint32_t{0};
    game->collision->ringMasks[index] = 0;
    game->collision->runtimeFlags = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfClCopyMotionData(FightRuntimePrefix *game) {
    for (std::int32_t index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        const Vector2 &delta = game->collision->contactDeltas[index];
        if ((delta.x < 0 ? -delta.x : delta.x) < 0.00000011920929f && (delta.y < 0 ? -delta.y : delta.y) < 0.00000011920929f)
            continue;
        mtwkTranslateRigInterpolated(game->actors[index]->motion, delta.x, 0, delta.y);
    }
}

#else

// Publish accumulated collision displacement into the solved MTWK rig.
// @verified - p
void vfClCopyMotionData(VFMA_t *game) {
    for (std::size_t i = 0; i != 6; ++i)
        if (game->characters[i]) {
            const auto &delta = game->collision->contactDeltas[i];
            if (std::fabs(delta.x) >= 0.00000011920929f || std::fabs(delta.y) >= 0.00000011920929f)
                mtwkApplyCollisionTranslation(game->actors[i]->motion, delta.x, 0, delta.y);
        }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfClRaycastInWorldGrid() {
    Vector3 point, hit;
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t *group = &g_pFightStage->enemySlots[index];
        if (!group->character)
            continue;
        if (group->state == 2) {
            const Vector3 *position = &group->fighter->game->collision->samples[group->actorIndex].points[0];
            if (!RaycastInWorldGrid(&hit, position, pushShapes[0].radius)) {
                FightCollisionValue<float>(group, 0xc4) = position->x;
                FightCollisionValue<float>(group, 0xc8) = position->z;
                FightCollisionValue<std::uint8_t>(group, 0xcc) = 1;
            }
        } else if (group->state == 1) {
            if (group->group && group->group->activationState == 3)
                continue;
            const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            const VFCollisionShape *shape = scenGetCharaName(control->scenarioTask, group->character) == SHEN_NAME(TGY_) ? togyuPushShapes : pushShapes;
            matrix_stack::LoadMatrix(mtwkGetBoneTransform(group->motion, shape->bone));
            Matrix4x4::TransformPoint(shape->point, point);
            if (!RaycastInWorldGrid(&hit, &point, shape->radius)) {
                FightCollisionValue<float>(group, 0xc4) = point.x;
                FightCollisionValue<float>(group, 0xc8) = point.z;
                FightCollisionValue<std::uint8_t>(group, 0xcc) = 1;
            } else if (FightCollisionValue<std::uint8_t>(group, 0xcc)) {
                MoveReadyEnemy(group, FightCollisionValue<float>(group, 0xc4) - point.x, FightCollisionValue<float>(group, 0xc8) - point.z);
            }
        }
    }
}

#else

// Active and ready actors use their respective real geometry owners.
// @verified - p
void vfClRaycastInWorldGrid() {
    for (std::size_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &group = g_pFightStage->enemySlots[i];
        if (!group.character)
            continue;
        Vector3 hit;
        if (group.state == 2) {
            const auto &point = group.fighter->game->collision->samples[group.actorIndex].points[0];
            if (!RaycastInWorldGrid(hit, point, 0.4f)) {
                group.offsetX = point.x;
                group.offsetZ = point.z;
                group.setPosition = 1;
            }
        } else if (group.state == 1 && (!group.group || static_cast<std::uint8_t>(group.group->activationState) != 3)) {
            const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            const bool togyu = scenGetCharaName(control->scenarioTask, group.character).name == SHEN_NAME(TGY_);
            const auto &shape = togyu ? togyuPushShapes[0] : pushShapes[0];
            Vector3 point;
            Matrix4x4::SetActiveTransform(mtwkGetBoneTransform(group.motion, shape.bone));
            Matrix4x4::TransformPoint(shape.point, point);
            if (RaycastInWorldGrid(hit, point, shape.radius)) {
                if (group.setPosition)
                    MoveReadyEnemy(&group, group.offsetX - point.x, group.offsetZ - point.z);
            } else {
                group.offsetX = point.x;
                group.offsetZ = point.z;
                group.setPosition = 1;
            }
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void vfclApplyDeltaToContactHistory(FightActorRuntimePrefix *actor, float x, float z) {
    Vector3 *point = reinterpret_cast<Vector3 *>(reinterpret_cast<std::byte *>(actor) + 0x74);
    std::int32_t index;
    for (index = 0; index < 10; ++index, ++point) {
        point->x += x;
        point->z += z;
    }
    point = actor->game->collision->samples[*reinterpret_cast<const std::int8_t *>(actor)].points;
    for (index = 0; index < 3; ++index, ++point) {
        point->x += x;
        point->z += z;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclMoveEntityWithContactHistory(FightActorRuntimePrefix *actor, float x, float z) {
    Vector3 world, root;
    if ((x < 0 ? -x : x) < 0.00000011920929f && (z < 0 ? -z : z) < 0.00000011920929f)
        return;
    mtwkGetRootPosition(actor->motion, &root);
    root.x += x;
    root.z += z;
    mtwkSetRootPosition(actor->motion, &root);
    actor->contactPosition.x += x;
    actor->contactPosition.z += z;
    actor->position.x += x;
    actor->position.z += z;
    GetCharacterWorldPosition(actor->character, &world);
    world.x += x;
    world.z += z;
    SetCharacterWorldPosition(actor->character, &world);
    Vector2 *const delta = &actor->game->collision->contactDeltas[*reinterpret_cast<const std::int8_t *>(actor)];
    delta->x += x;
    delta->y += z;
    vfclApplyDeltaToContactHistory(actor, x, z);
}

#else

// Calls vfclApplyDeltaToContactHistory: movement preserves root, character and contact-history ownership.
// @verified - p
void vfclMoveEntityWithContactHistory(VFActor *self, float x, float z) {
    if (std::fabs(x) < 0.00000011920929f && std::fabs(z) < 0.00000011920929f)
        return;
    Vector3 root = mtwkGetRootPosition(self->motion);
    root.x += x;
    root.z += z;
    mtwkSetRootPosition(self->motion, root);
    self->contactPosition.x += x;
    self->contactPosition.z += z;
    self->position.x += x;
    self->position.z += z;
    Vector3 world = GetCharacterWorldPosition(self->character);
    world.x += x;
    world.z += z;
    shenApplyTranslationToAll(&self->character->entity.node, world);
    auto &collision = *self->game->collision;
    collision.contactDeltas[self->groupIndex].x += x;
    collision.contactDeltas[self->groupIndex].y += z;
    for (auto &point : self->motionContactSamples) {
        point.x += x;
        point.z += z;
    }
    for (auto &point : collision.samples[self->groupIndex].points) {
        point.x += x;
        point.z += z;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclAdjustToGroundOrSlope(VFCL_t *collision) {
    struct ActorView {
        std::byte retained000[2];
        std::uint16_t fightStyle;
        std::byte retained004[0x790];
        ActorContactState contact;
    };
    std::int32_t index, slot;
    FightRuntimePrefix *game;
    FightActorRuntimePrefix *actor;
    std::int32_t collided;
    const VFCollisionShape *shape;
    Vector3 *point, *previous;
    float radius, distance, largest;
    Vector3 delta, hit, best, direction;
    if (collision->runtimeFlags & 1U)
        return;
    game = collision->game;
    delta.y = 0.0f;
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index] || !game->actors[index] || reinterpret_cast<ActorView *>(game->actors[index])->fightStyle == 1)
            continue;
        actor = game->actors[index];
        if (!(actor->stateFlags & 0x40U))
            continue;
        actor->stateFlags &= ~0x40U;
        collided = 0;
        largest = 0.0f;
        shape = *reinterpret_cast<const VFCollisionShape *const *>(reinterpret_cast<const std::byte *>(actor) + 0x9ac);
        for (slot = 0; slot < 3; ++slot, ++shape) {
            point = &collision->samples[index].points[slot];
            previous = &collision->previousSamples[index].points[slot];
            radius = shape->radius;
            if (FindNearestRayIntersection(&hit, point, previous)) {
                direction.x = point->x - previous->x;
                direction.y = point->y - previous->y;
                direction.z = point->z - previous->z;
                am2Vector3NormalizeFast(&direction);
                hit.x -= direction.x * radius;
                hit.z -= direction.z * radius;
                delta.x = hit.x - point->x;
                delta.z = hit.z - point->z;
                distance = delta.x * delta.x + delta.z * delta.z;
                if (distance > largest) {
                    largest = distance;
                    collided = 1;
                    best.x = delta.x;
                    best.z = delta.z;
                    reinterpret_cast<ActorView *>(actor)->contact.activeShapeMask = reinterpret_cast<ActorView *>(actor)->contact.activeShapeMask | shape->mask;
                }
            } else if (RaycastInWorldGrid(&hit, point, radius)) {
                delta.x = hit.x - point->x;
                delta.z = hit.z - point->z;
                distance = delta.x * delta.x + delta.z * delta.z;
                if (distance > largest) {
                    largest = distance;
                    collided = 1;
                    best.x = delta.x;
                    best.z = delta.z;
                    reinterpret_cast<ActorView *>(actor)->contact.activeShapeMask = reinterpret_cast<ActorView *>(actor)->contact.activeShapeMask | shape->mask;
                }
            }
        }
        if (collided) {
            actor->moveFlags |= 0x8000U;
            vfclMoveEntityWithContactHistory(actor, best.x, best.z);
        }
    }
}

#else

// Resolve the largest native static/transformed world push.
// @verified - p
void vfclAdjustToGroundOrSlope(VFCL_t *collision) {
    if (collision->runtimeFlags & 1)
        return;
    VFMA_t *const game = collision->game;
    for (std::size_t i = 0; i != 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        if (!self || self->fightStyle == 1 || game->type == 8 || !(self->stateFlags & VFSF_Unknown40))
            continue;
        self->stateFlags &= ~VFSF_Unknown40;
        const auto *const shapes = PushShapes(self);
        float best = 0, x = 0, z = 0;
        bool collided = false;
        for (std::size_t j = 0; j != 3; ++j) {
            const auto &point = collision->samples[i].points[j];
            const auto &previous = collision->previousSamples[i].points[j];
            Vector3 hit;
            if (FindNearestRayIntersection(&hit, &point, &previous)) {
                Vector3 direction{point.x - previous.x, point.y - previous.y, point.z - previous.z};
                Vector3::NormalizeInPlace(direction);
                hit.x -= direction.x * shapes[j].radius;
                hit.z -= direction.z * shapes[j].radius;
            } else if (!RaycastInWorldGrid(hit, point, shapes[j].radius))
                continue;
            const auto dx = hit.x - point.x, dz = hit.z - point.z;
            const auto distance = dz * dz + dx * dx;
            if (distance > best) {
                best = distance;
                x = dx;
                z = dz;
                collided = true;
                self->contact.activeShapeMask = self->contact.activeShapeMask | shapes[j].tag;
            }
        }
        if (collided) {
            self->moveFlags |= VFMF_AirborneOrFalling;
            vfclMoveEntityWithContactHistory(self, x, z);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclUpdateHeightFlagsForGrid(FightActorRuntimePrefix *actor) {
    std::uint8_t index;
    std::uint16_t snap, local, outside, inside, floor;
    float localY, floorY, snapY;
    std::uint32_t fixedGround;
    FightRuntimePrefix *game;
    const VFCollisionShape *shape;
    Vector3 *point;
    inside = 0;
    outside = 0;
    local = 0;
    snap = 0;
    floor = 0;
    localY = actor->position.y + 0.05f;
    game = actor->game;
    floorY = game->floorIntersectionY;
    snapY = game->floorSnapY;
    fixedGround = g_pFightStage->battleFlags & 0x800U;
    point = reinterpret_cast<Vector3 *>(reinterpret_cast<std::byte *>(actor) + 0x74);
    shape = bodyShapes;
    for (index = 0; index < 10; ++index, ++shape, ++point) {
        const std::uint16_t mask = shape->mask;
        const float bottom = point->y - bodyShapes[0].radius;
        if (game->type != 8) {
            float ground;
            if (fixedGround)
                ground = actor->targetGroundY;
            else {
                point->y += 1.0f;
                ground = flddFindIntersectionBy(*point);
                point->y -= 1.0f;
            }
            if (ground + 0.05f > bottom)
                floor |= mask;
            if (localY > bottom)
                local |= mask;
            inside |= mask;
        } else {
            if (floorY + 0.05f > bottom)
                floor |= mask;
            if (snapY + 0.05f > bottom)
                snap |= mask;
            if (localY > bottom)
                local |= mask;
            if (vfIsWithinFightFloor(*point))
                inside |= mask;
            else
                outside |= mask;
        }
    }
    FightCollisionValue<std::uint16_t>(actor, 0x3de) = inside & floor;
    FightCollisionValue<std::uint16_t>(actor, 0x3ec) = inside;
    FightCollisionValue<std::uint16_t>(actor, 0x3ea) = outside;
    FightCollisionValue<std::uint16_t>(actor, 0x3e8) = outside & snap;
    FightCollisionValue<std::uint16_t>(actor, 0x3e6) = local;
    FightCollisionValue<std::uint16_t>(actor, 0x3f0) = FightCollisionValue<std::uint16_t>(actor, 0x3ee);
    FightCollisionValue<std::uint16_t>(actor, 0x3ee) = FightCollisionValue<std::uint16_t>(actor, 0x3de) | FightCollisionValue<std::uint16_t>(actor, 0x3e8);
}

#else

// The fixed height-tag bank is independent of TOGYU geometry selection.
// @verified - p
void vfclUpdateHeightFlagsForGrid(VFActor *self) {
    VFMA_t *const game = self->game;
    std::uint16_t inside = 0, outside = 0, floor = 0, snap = 0, local = 0;
    const float localY = self->position.y + 0.05f;
    for (std::size_t i = 0; i != 10; ++i) {
        auto &point = self->motionContactSamples[i];
        const auto tag = bodyShapes[i].tag;
        const float bottom = point.y - bodyShapes[0].radius;
        if (game->type == 8) {
            if (game->floorIntersectionY + 0.05 > bottom)
                floor |= tag;
            if (game->floorSnapY + 0.05 > bottom)
                snap |= tag;
            if (localY > bottom)
                local |= tag;
            if (vfIsWithinFightFloor(point))
                inside |= tag;
            else
                outside |= tag;
        } else {
            float ground = self->targetGroundY;
            if (!(g_pFightStage->battleFlags & 0x800)) {
                point.y += 1;
                ground = flddFindIntersectionBy(point);
                point.y -= 1;
            }
            if (ground + 0.05 > bottom)
                floor |= tag;
            if (localY > bottom)
                local |= tag;
            inside |= tag;
        }
    }
    const auto previous = self->groundContactMask;
    self->groundOutsideMask = outside;
    self->groundSnapMask = static_cast<std::uint16_t>(snap & outside);
    self->groundContactMask = static_cast<std::uint16_t>((floor & inside) | (snap & outside));
    self->footContactState = floor & inside;
    self->groundFlags = inside;
    self->parameter45a = local;
    self->previousGroundContactMask = previous;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclResolvePairedContactDrift(FightRuntimePrefix *game) {
    FightCollisionRuntimePrefix *collision = game->collision;
    std::int32_t index;
    for (index = 0; index < 6; ++index) {
        FightActorRuntimePrefix *self, *other;
        Vector2 *first, *second;
        if (!game->characters[index])
            continue;
        self = game->actors[index];
        if (!(self->hitStateFlags & 0x80000U) || (self->activeMotionFlags & 0x80U))
            continue;
        if (!self->secondActor || !(self->secondActor->hitStateFlags & 0x40000U))
            continue;
        other = self->secondActor;
        first = &collision->contactDeltas[index];
        second = &collision->contactDeltas[other->groupIndex];
        if (collision->ringMasks[index] && collision->ringMasks[other->groupIndex] &&
            !*reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(self) + 0x844) && !(self->stateFlags & 0x20U) &&
            !*reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(other) + 0x844) && !(other->stateFlags & 0x20U)) {
            if ((first->x < 0.0f ? -first->x : first->x) < 0.00000011920929f && (first->y < 0.0f ? -first->y : first->y) < 0.00000011920929f &&
                (second->x < 0.0f ? -second->x : second->x) < 0.00000011920929f && (second->y < 0.0f ? -second->y : second->y) < 0.00000011920929f)
                continue;
            if (first->x * first->x + first->y * first->y > second->x * second->x + second->y * second->y) {
                vfclMoveEntityWithContactHistory(other, first->x - second->x, first->y - second->y);
                other->stateFlags |= 0x40U;
            } else {
                vfclMoveEntityWithContactHistory(self, second->x - first->x, second->y - first->y);
                self->stateFlags |= 0x40U;
            }
        } else if (*reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(self) + 0x844) || collision->ringMasks[index] ||
            (self->stateFlags & 0x20U)) {
            if (!*reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(other) + 0x844) && !collision->ringMasks[other->groupIndex] &&
                !(other->stateFlags & 0x20U)) {
                vfclMoveEntityWithContactHistory(other, first->x - second->x, first->y - second->y);
                other->stateFlags |= 0x40U;
            }
        } else {
            vfclMoveEntityWithContactHistory(self, second->x - first->x, second->y - first->y);
            self->stateFlags |= 0x40U;
        }
    }
}

#else

// Paired throw actors share the winning real contact displacement.
// @verified - p
void vfclResolvePairedContactDrift(VFMA_t *game) {
    auto &collision = *game->collision;
    for (std::size_t i = 0; i != 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        if (!(self->hitStateFlags & VFHSF_AirborneOrDowned) || (self->activeMotionFlags & VFSMF_Unknown80))
            continue;
        VFActor *const other = self->secondActor;
        if (!other || !(other->hitStateFlags & VFHSF_MidAirRecovery))
            continue;
        const auto index = static_cast<std::int8_t>(other->groupIndex);
        const auto &a = collision.contactDeltas[i];
        const auto &b = collision.contactDeltas[index];
        const auto selfHit = self->contact.activeShapeMask;
        const auto otherHit = other->contact.activeShapeMask;
        const auto moveSelf = [&] {
            vfclMoveEntityWithContactHistory(self, b.x - a.x, b.y - a.y);
            self->stateFlags |= VFSF_Unknown40;
        };
        const auto moveOther = [&] {
            vfclMoveEntityWithContactHistory(other, a.x - b.x, a.y - b.y);
            other->stateFlags |= VFSF_Unknown40;
        };
        if (collision.ringMasks[i] && collision.ringMasks[index] && !selfHit && !(self->stateFlags & VFSF_Unknown20) && !otherHit &&
            !(other->stateFlags & VFSF_Unknown20)) {
            if (std::fabs(a.x) < 0.00000011920929f && std::fabs(a.y) < 0.00000011920929f && std::fabs(b.x) < 0.00000011920929f &&
                std::fabs(b.y) < 0.00000011920929f)
                continue;
            if (a.x * a.x + a.y * a.y <= b.x * b.x + b.y * b.y)
                moveSelf();
            else
                moveOther();
        } else if (!selfHit && !collision.ringMasks[i] && !(self->stateFlags & VFSF_Unknown20))
            moveSelf();
        else if (!otherHit && !collision.ringMasks[index] && !(other->stateFlags & VFSF_Unknown20))
            moveOther();
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclEnforceRingBoundary(VFCL_t *collision) {
    std::uint8_t index, selected;
    FightRuntimePrefix *game;
    FightActorRuntimePrefix *actor;
    const VFCollisionShape *shape;
    std::uint8_t point, spawn;
    float boundaryX, boundaryZ, radius, x, z, square, extent, largest, distance;
    Vector3 push, step;
    if (collision->runtimeFlags & 2U)
        return;
    game = collision->game;
    if (!game->spawnPointCount)
        return;
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        actor = game->actors[index];
        actor->stateFlags &= ~1U;
        if ((actor->retained350 & 1U) && !(g_pFightStage->battleFlags & 2U))
            continue;
        spawn = actor->nearestSpawnPoint;
        boundaryX = game->spawnPoints[spawn].x;
        boundaryZ = game->spawnPoints[spawn].z;
        radius = game->spawnPoints[spawn].parameter;
        collision->ringMasks[index] = largest = selected = distance = 0;
        shape = *reinterpret_cast<const VFCollisionShape *const *>(reinterpret_cast<const std::byte *>(actor) + 0x9ac);
        for (point = 0; point < 3; ++point, ++shape) {
            x = collision->samples[index].points[point].x;
            z = collision->samples[index].points[point].z;
            x -= boundaryX;
            z -= boundaryZ;
            square = x * x + z * z;
            if ((radius - shape->radius) * (radius - shape->radius) > square)
                continue;
            collision->ringMasks[index] |= shape->mask;
            square = ::sqrtf(square);
            extent = shape->radius + square;
            if (extent > largest) {
                largest = extent;
                selected = point;
                distance = square;
            }
        }
        if (largest < 0.00000011920929f) {
            actor->stateFlags &= ~0x400U;
            continue;
        }
        if (actor->stateFlags & 0x4000U)
            actor->stateFlags |= 0x400U;
        else if (actor->game->type == 8 && !(g_pFightStage->battleFlags & 0x402U) && game->elapsedTicks > 450U &&
            (((actor->hitStateFlags & 0x10U) && (actor->hitStateFlags & 0x24000U)) || (actor->activeMotionFlags & 0x80U) || (actor->retained350 & 1U) ||
                (actor->stateFlags & 0x600U)))
            actor->stateFlags |= 0x400U;
        else {
            shape = *reinterpret_cast<const VFCollisionShape *const *>(reinterpret_cast<const std::byte *>(actor) + 0x9ac) + selected;
            distance -= radius - shape->radius - 0.01f;
            if (distance < 0.00000011920929f)
                continue;
            push.x = boundaryX - collision->samples[index].points[selected].x;
            push.z = boundaryZ - collision->samples[index].points[selected].z;
            push.y = 0.0f;
            dc::sdk::SetVectorLength(&step, &push, distance);
            vfclMoveEntityWithContactHistory(actor, step.x, step.z);
            actor->stateFlags |= 1U;
        }
    }
}

#else

// Authored circular fight boundaries publish tags before applying push.
// @verified - p
void vfclEnforceRingBoundary(VFCL_t *collision) {
    VFMA_t *const game = collision->game;
    if ((collision->runtimeFlags & 2) || !game->spawnPointCount)
        return;
    bool otherEscaping = false;
    for (std::size_t i = 0; i != 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const self = game->actors[i];
        self->stateFlags &= ~VFSF_Unknown1;
        const auto offset = std::size_t(self->nearestSpawnPoint) * 12;
        const float centreX = ReadFightData<float>(game, 0x2f4 + offset);
        const float centreZ = ReadFightData<float>(game, 0x2f8 + offset);
        const float radius = ReadFightData<float>(game, 0x2f0 + offset);
        const auto *const shapes = PushShapes(self);
        auto &mask = collision->ringMasks[i];
        mask = 0;
        std::size_t selected = 0;
        float largest = 0, selectedDistance = 0;
        for (std::size_t j = 0; j != 3; ++j) {
            const auto &point = collision->samples[i].points[j];
            const float x = point.x - centreX, z = point.z - centreZ;
            const float squared = z * z + x * x;
            if ((radius - shapes[j].radius) * (radius - shapes[j].radius) > squared)
                continue;
            mask |= shapes[j].tag;
            const float distance = std::sqrt(squared);
            if (distance + shapes[j].radius > largest) {
                largest = distance + shapes[j].radius;
                selected = j;
                selectedDistance = distance;
            }
        }
        if (largest < 0.00000011920929f) {
            self->stateFlags &= ~VFSF_Unknown400;
            continue;
        }
        if (game->type == 8 && !(g_pFightStage->battleFlags & 0x402)) {
            for (std::size_t j = 0; j != 6; ++j)
                if (game->characters[j] && (game->actors[j]->stateFlags & 0x400)) {
                    otherEscaping = true;
                    break;
                }
            const std::uint32_t timeout = g_currentMap == AREA_QSFD ? 450 : 900;
            if ((self->stateFlags & VFSF_Unknown4000) || (self->savedMarkerMask & 1) || otherEscaping ||
                (static_cast<std::uint32_t>(int(game->frame)) > timeout &&
                    (((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_Unknown20000)) && (self->hitStateFlags & VFHSF_FallLandingRequired)) ||
                        (self->activeMotionFlags & VFSMF_Unknown80) || (self->stateFlags & VFSF_Unknown200)))) {
                self->stateFlags |= VFSF_Unknown400;
                continue;
            }
        }
        const float penetration = selectedDistance - (radius - shapes[selected].radius - 0.01f);
        if (penetration < 0.00000011920929f)
            continue;
        const auto &point = collision->samples[i].points[selected];
        const auto delta = ScaleToMagnitude({centreX - point.x, 0, centreZ - point.z}, penetration);
        vfclMoveEntityWithContactHistory(self, delta.x, delta.z);
        self->stateFlags |= VFSF_Unknown1;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclResolveWorldContacts(VFCL_t *collision) {
    std::int32_t index;
    FightRuntimePrefix *const game = collision->game;
    Vector3 point, step;
    step.y = 0.0f;
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[index];
        const Vector3 &previous = collision->previousSamples[index].points[0];
        const Vector3 &current = collision->samples[index].points[0];
        step.x = current.x - previous.x;
        step.z = current.z - previous.z;
        if ((step.x < 0.0f ? -step.x : step.x) < 0.00000011920929f && (step.z < 0.0f ? -step.z : step.z) < 0.00000011920929f)
            continue;
        point.x = current.x;
        point.y = current.y;
        point.z = current.z;
        am2Vector3NormalizeFast(&step);
        step.x *= 0.2f;
        step.z *= 0.2f;
        point.x += step.x * 3;
        point.z += step.z * 3;
        const float first = flddGetIntersectionFor(actor->character, point) - actor->targetGroundY;
        point.x += step.x;
        point.y += first;
        point.z += step.z;
        const float second = flddGetIntersectionFor(actor->character, point) - actor->targetGroundY;
        float change = second - first;
        if (game->type != 8)
            change = ::fabsf(change);
        if (change < 0.5f) {
            point.x += step.x;
            point.y += second;
            point.z += step.z;
            const float third = flddGetIntersectionFor(actor->character, point) - actor->targetGroundY;
            change = third - second;
            if (game->type != 8)
                change = ::fabsf(change);
            if (change < 0.5f)
                continue;
        }
        vfclMoveEntityWithContactHistory(actor, previous.x - current.x, previous.z - current.z);
    }
}

std::uint8_t vfclFindBodyShapeForBone(std::uint8_t bone) {
    std::uint8_t index;
    for (index = 0; index < 10; ++index) {
        if (bodyShapes[index].bone == bone)
            break;
    }
    return index;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void vfclSetContactCategory(FightActorRuntimePrefix *a, FightActorRuntimePrefix *b, VFCL_t *collision) {
    struct ContactMask {
        std::uint32_t first, second;
        float slide, stick;
    };
    static const ContactMask entries[7] = {{0xffffffffU, 0xffffffffU, .5f, .5f}, {0xffffffffU, 0xffffffffU, 0, 1}, {0xffffffffU, 0xffffffffU, 1, 0},
        {0x202, 0x202, 0, 1}, {0x202, 0x202, 1, 0}, {0xffffffffU, 0xffffffffU, 0, 1}, {0xffffffffU, 0xffffffffU, 1, 0}};
    const std::uint32_t first = a->hitStateFlags, second = b->hitStateFlags, either = first | second;
    std::uint32_t flags = 0, type = 0;
    if ((either & 0xc0000U) == 0xc0000U)
        flags = 1;
    else if (FightCollisionValue<std::uint8_t>(b, 2) == 1) {
        type = 1;
        goto publish;
    } else if (FightCollisionValue<std::uint8_t>(a, 2) == 1) {
        type = 2;
        goto publish;
    } else if (a->game->type == 9) {
        type = 2;
        goto publish;
    } else if (((first ^ second) & 0x100U) && a->moveId != 41 && b->moveId != 41)
        flags = 2;
    else if (first & second & 0x100U)
        flags = 4;
    if (either & 0x4000U)
        flags |= 8;
    if (!(flags & 5U)) {
        if (either & 0x10000U) {
            if (!(first & 0x10000U))
                type = !(second & 0x8000U) && (first & 0x400000U) && (a->motionFlags & 0x1010U) ? 6 : 2;
            else
                type = !(first & 0x8000U) && (second & 0x400000U) && (b->motionFlags & 0x1010U) ? 5 : 1;
        } else if (ContactState(a).linked)
            type = 1;
        else if (ContactState(b).linked)
            type = 2;
        else if (ContactState(a).activeShapeMask)
            type = 1;
        else if (ContactState(b).activeShapeMask)
            type = 2;
        else if (first & 0xc0000U)
            type = 1;
        else if (second & 0xc0000U)
            type = 2;
        else if (flags & 2U) {
            type = 4;
            if (first & 0x100U)
                type = (second & 0x10002000U) != 0x10002000U && !(b->activeMotionFlags & 0x200000U) ? 3 : 1;
        }
    }
publish:
    collision->flagsWord = flags;
    collision->contactType = type;
    collision->maskA = entries[type].first;
    collision->maskB = entries[type].second;
    collision->slideScale = entries[type].slide;
    collision->stickScale = entries[type].stick;
    const std::int16_t animationA = FightCollisionValue<std::int16_t>(a, 0x42c), animationB = FightCollisionValue<std::int16_t>(b, 0x42c);
    if (animationA == 283 || animationA == 571)
        collision->maskA &= 0xffff93ffU;
    if (animationB == 283 || animationB == 571)
        collision->maskB &= 0xffff93ffU;
}

#else

// The PC exception-animation list contains only its native -1 terminator.
// @verified - p
void vfclSetContactCategory(VFActor *a, VFActor *b, VFCL_t *collision) {
    struct MaskEntry {
        std::uint32_t first, second;
        float slide, stick;
    };
    static constexpr MaskEntry entries[]{{0xffffffff, 0xffffffff, .5f, .5f}, {0xffffffff, 0xffffffff, 0, 1}, {0xffffffff, 0xffffffff, 1, 0},
        {0x202, 0x202, 0, 1}, {0x202, 0x202, 1, 0}, {0xffffffff, 0xffffffff, 0, 1}, {0xffffffff, 0xffffffff, 1, 0}};
    const auto first = a->hitStateFlags, second = b->hitStateFlags, either = first | second;
    std::uint32_t flags = 0, type = 0;
    if ((either & 0xc0000) == 0xc0000)
        flags = 1;
    else if (a->fightStyle == 1 || b->fightStyle == 1)
        type = 1;
    else if (a->game->type == 9)
        type = 2;
    else {
        if (((first ^ second) & 0x100) && a->moveId != 41 && b->moveId != 41)
            flags = 2;
        if (first & second & 0x100)
            flags |= 4;
    }
    if (!type) {
        if (either & 0x4000)
            flags |= 8;
        if (!(flags & 5)) {
            if (either & 0x10000) {
                if (!(first & 0x10000))
                    type = !(second & 0x8000) && (first & 0x400000) && (a->motionFlags & (VFMOF_Unknown10 | VFMOF_Unknown1000)) ? 6 : 2;
                else
                    type = !(first & 0x8000) && (second & 0x400000) && (b->motionFlags & (VFMOF_Unknown10 | VFMOF_Unknown1000)) ? 5 : 1;
            } else if (a->contact.linked || b->contact.linked || a->contact.activeShapeMask)
                type = 1;
            else if (b->contact.activeShapeMask)
                type = 2;
            else if (first & 0xc0000)
                type = 1;
            else if (second & 0xc0000)
                type = 2;
            else if (flags & 2) {
                type = 3;
                if ((first & 0x100) && ((second & 0x10002000) == 0x10002000 || (b->activeMotionFlags & VFSMF_Unknown200000)))
                    type = 2;
            }
        }
    }
    collision->flagsWord = flags;
    collision->contactType = type;
    collision->maskA = entries[type].first;
    collision->maskB = entries[type].second;
    collision->slideScale = entries[type].slide;
    collision->stickScale = entries[type].stick;
    if (a->currentAnimation == 283 || a->currentAnimation == 571)
        collision->maskA &= 0xffff93ff;
    if (b->currentAnimation == 283 || b->currentAnimation == 571)
        collision->maskB &= 0xffff93ff;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct ActorCollisionView {
    std::int8_t groupIndex;
    std::byte retained01[0x73];
    Vector3 currentSamples[10];
    std::byte retainedEc[0x6a8];
    ActorContactState contact;
    std::byte retained860[0x148];
    const VFCollisionShape *bodyShapes;
};

typedef char ActorCollisionViewCheck[offsetof(ActorCollisionView, bodyShapes) == 0x9a8 ? 1 : -1];

} // namespace

static void vfclComputePenetrationAndOverlap(VFCL_t *collision, FightActorRuntimePrefix *defender, FightActorRuntimePrefix *attacker) {
    ActorCollisionView *a = reinterpret_cast<ActorCollisionView *>(defender), *b = reinterpret_cast<ActorCollisionView *>(attacker);
    std::int32_t i, j;
    float penetration = 0, radiusB, radius, distance, x, y, z, movementSquared, fraction, projected;
    Vector3 movement, previous, current;
    if (collision->contactType - 5U < 2) {
        for (j = 0; j < 10; ++j) {
            radiusB = b->bodyShapes[j].radius;
            for (i = 0; i < 10; ++i) {
                x = b->currentSamples[j].x - a->currentSamples[i].x;
                y = b->currentSamples[j].y - a->currentSamples[i].y;
                z = b->currentSamples[j].z - a->currentSamples[i].z;
                distance = x * x + y * y + z * z;
                distance = NativeSqrt(distance);
                radius = radiusB + a->bodyShapes[i].radius;
                if (distance < radius) {
                    a->contact.shapeMaskWords[i] |= b->bodyShapes[j].mask;
                    b->contact.shapeMaskWords[j] |= a->bodyShapes[i].mask;
                    radius -= distance;
                    if (radius > penetration)
                        penetration = radius;
                }
            }
        }
        collision->penetrationCharacter = 0;
        collision->penetrationSpecial = penetration;
    } else {
        const Vector3 &previousA = collision->previousSamples[a->groupIndex].points[0];
        const Vector3 &previousB = collision->previousSamples[b->groupIndex].points[0];
        const float separationX = previousB.x - previousA.x, separationZ = previousB.z - previousA.z;
        distance = separationX * separationX + separationZ * separationZ;
        if (distance < 0.00000011920929f) {
            collision->separation.x = 0;
            collision->separation.z = 0;
            collision->separationLength = 0;
        } else {
            distance = NativeSqrt(distance);
            collision->separation.x = separationX / distance;
            collision->separation.z = separationZ / distance;
            collision->separationLength = distance;
        }
        collision->separation.y = 0;
        std::uint32_t attackerMask = collision->attackerMask, defenderMask = collision->defenderMask;
        std::uint32_t maskA = collision->maskA, maskB = collision->maskB;
        std::int32_t excluded, swept;
        for (j = 0; j < 10; ++j) {
            radiusB = b->bodyShapes[j].radius;
            excluded = 0;
            if (defenderMask != maskB && ((defenderMask & b->bodyShapes[j].mask) || !(maskB & b->bodyShapes[j].mask)))
                excluded = 1;
            for (i = 0; i < 10; ++i) {
                am2Vector3Subtract(&current, &b->currentSamples[j], &a->currentSamples[i]);
                am2Vector3Subtract(&previous, &b->contact.previousSamples[j], &a->contact.previousSamples[i]);
                am2Vector3Subtract(&movement, &current, &previous);
                distance = previous.x * movement.x + previous.y * movement.y + previous.z * movement.z;
                swept = 0;
                movementSquared = 0;
                fraction = 0;
                if (distance < 0) {
                    distance = -distance;
                    if (distance > 0.00000011920929f) {
                        movementSquared = am2Vector3LengthSquared(&movement);
                        if (movementSquared > distance) {
                            swept = 2;
                            fraction = distance / movementSquared;
                            current.x = fraction * movement.x + previous.x;
                            current.y = fraction * movement.y + previous.y;
                            current.z = fraction * movement.z + previous.z;
                        }
                    }
                }
                distance = am2Vector3LengthSquared(&current);
                radius = radiusB + a->bodyShapes[i].radius;
                radius *= radius;
                if (distance < radius) {
                    a->contact.shapeMaskWords[i] |= b->bodyShapes[j].mask;
                    b->contact.shapeMaskWords[j] |= a->bodyShapes[i].mask;
                    if (!excluded && (attackerMask == maskA || (!(attackerMask & a->bodyShapes[i].mask) && (maskA & a->bodyShapes[i].mask)))) {
                        if (!swept) {
                            projected = collision->separation.x * current.x + collision->separation.z * current.z;
                            if (projected > 0)
                                radius = NativeSqrt(radius - (distance - projected * projected)) - projected;
                            else
                                radius = NativeSqrt(radius - (distance - projected * projected)) + projected;
                        } else {
                            const float movementLength = NativeSqrt(movementSquared);
                            radius = NativeSqrt(radius - distance) + movementLength * (1 - fraction);
                        }
                        if (radius > penetration)
                            penetration = radius;
                    }
                }
            }
        }
        collision->penetrationCharacter = penetration;
        collision->penetrationSpecial = 0;
    }
}

#else

// Continuous body-sphere overlap keeps both native per-bone mask banks.
// @verified - p
void vfclComputePenetrationAndOverlap(VFCL_t *collision, VFActor *defender, VFActor *attacker) {
    const auto *a = reinterpret_cast<const VFCollisionShape *>(defender->parameterAd8);
    const auto *b = reinterpret_cast<const VFCollisionShape *>(attacker->parameterAd8);
    float penetration = 0;
    const bool special = std::uint32_t(collision->contactType - 5) <= 1;
    if (!special) {
        const auto &previousA = collision->previousSamples[defender->groupIndex].points[0];
        const auto &previousB = collision->previousSamples[attacker->groupIndex].points[0];
        const float x = previousB.x - previousA.x;
        const float z = previousB.z - previousA.z;
        const float distanceSquared = z * z + x * x;
        if (!(distanceSquared < 0.00000011920929f)) {
            const float distance = std::sqrt(distanceSquared);
            collision->separation = {x / distance, 0, z / distance};
            collision->separationLength = distance;
        } else {
            collision->separation = {};
            collision->separationLength = 0;
        }
    }
    for (std::size_t j = 0; j != 10; ++j) {
        const bool excluded = collision->defenderMask != collision->maskB && ((b[j].tag & collision->defenderMask) || !(b[j].tag & collision->maskB));
        for (std::size_t i = 0; i != 10; ++i) {
            const auto &first = defender->motionContactSamples[i];
            const auto &second = attacker->motionContactSamples[j];
            Vector3 current{second.x - first.x, second.y - first.y, second.z - first.z};
            const float radius = b[j].radius + a[i].radius;
            float movementSquared = 0;
            float fraction = 0;
            bool swept = false;
            if (!special) {
                const auto previousA = defender->contact.previousSamples[i];
                const auto previousB = attacker->contact.previousSamples[j];
                const Vector3 previous{previousB.x - previousA.x, previousB.y - previousA.y, previousB.z - previousA.z};
                const Vector3 movement{current.x - previous.x, current.y - previous.y, current.z - previous.z};
                const float dot = (movement.y * previous.y + movement.x * previous.x) + movement.z * previous.z;
                if (dot < 0 && -dot > 0.00000011920929f) {
                    movementSquared = (movement.x * movement.x + movement.y * movement.y) + movement.z * movement.z;
                    if (movementSquared > -dot) {
                        swept = true;
                        fraction = -dot / movementSquared;
                        current = {fraction * movement.x + previous.x, fraction * movement.y + previous.y, fraction * movement.z + previous.z};
                    }
                }
            }
            const float distanceSquared = (current.x * current.x + current.y * current.y) + current.z * current.z;
            float distance = 0;
            const bool overlaps = special ? (distance = std::sqrt(distanceSquared), !(distance >= radius)) : !(distanceSquared >= radius * radius);
            if (!overlaps)
                continue;
            defender->contact.shapeMasks[i] = defender->contact.shapeMasks[i] | b[j].tag;
            attacker->contact.shapeMasks[j] = attacker->contact.shapeMasks[j] | a[i].tag;
            if (special) {
                penetration = std::fmax(radius - distance, penetration);
            } else if (!excluded && (collision->attackerMask == collision->maskA || (!(a[i].tag & collision->attackerMask) && (a[i].tag & collision->maskA)))) {
                float amount;
                if (swept) {
                    amount = std::sqrt(radius * radius - distanceSquared) + std::sqrt(movementSquared) * (1 - fraction);
                } else {
                    const float projected = current.z * collision->separation.z + current.x * collision->separation.x;
                    const float chord = std::sqrt(radius * radius - (distanceSquared - projected * projected));
                    amount = !(projected > 0) ? chord + projected : chord - projected;
                }
                penetration = std::fmax(amount, penetration);
            }
        }
    }
    collision->penetrationSpecial = special ? penetration : 0;
    collision->penetrationCharacter = special ? 0 : penetration;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void vfclAdjustForSlideAndStick(VFCL_t *collision) {
    FightActorRuntimePrefix *defender = collision->defender, *attacker = collision->attacker;
    float factor;
    if (collision->contactType - 5U < 2) {
        factor = collision->penetrationSpecial + *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(defender) + 0x84c);
        float floor = defender->targetGroundY, lift = 0.2f;
        if (!(factor > floor + lift))
            factor = floor + lift;
        FightCollisionValue<float>(defender, 0x850) = factor;
    } else {
        Vector3 a, b;
        factor = -collision->slideScale * collision->penetrationCharacter;
        a.x = factor * collision->separation.x;
        a.z = factor * collision->separation.z;
        factor = collision->stickScale * collision->penetrationCharacter;
        b.x = factor * collision->separation.x;
        b.z = factor * collision->separation.z;
        const Vector3 &pa = collision->samples[*reinterpret_cast<const std::int8_t *>(defender)].points[0];
        const Vector3 &pb = collision->samples[*reinterpret_cast<const std::int8_t *>(attacker)].points[0];
        float z, x;
        x = (pb.x + b.x) - (pa.x + a.x);
        z = (pb.z + b.z) - (pa.z + a.z);
        float distance;
        distance = shenmue::NativeSqrt(x * x + z * z);
        if (distance > 0.00000011920929f) {
            const float midpoint = (collision->separationLength + distance) * 0.5f;
            x = x * midpoint / distance;
            z = z * midpoint / distance;
            x -= pb.x - pa.x;
            z -= pb.z - pa.z;
            factor = -collision->slideScale;
            a.x = factor * x * FightCollisionValue<float>(defender, 0x26c);
            a.z = factor * z * FightCollisionValue<float>(defender, 0x26c);
            factor = collision->stickScale;
            b.x = factor * x * FightCollisionValue<float>(attacker, 0x26c);
            b.z = factor * z * FightCollisionValue<float>(attacker, 0x26c);
        }
        vfclMoveEntityWithContactHistory(defender, a.x, a.z);
        vfclMoveEntityWithContactHistory(attacker, b.x, b.z);
    }
    if (collision->penetrationSpecial < 0.05f)
        FightCollisionValue<float>(defender, 0x850) = 0;
}

#else

// Owner motion scales are applied after the midpoint correction.
// @verified - p
void vfclAdjustForSlideAndStick(VFCL_t *collision) {
    auto *const defender = collision->defender;
    auto *const attacker = collision->attacker;
    if (std::uint32_t(collision->contactType - 5) <= 1) {
        const float ground = defender->targetGroundY + 0.2f, adjusted = defender->contact.averageGround + collision->penetrationSpecial;
        defender->contact.groundSamples[0] = ground > adjusted ? ground : adjusted;
    } else {
        const auto &a = collision->samples[defender->groupIndex].points[0];
        const auto &b = collision->samples[attacker->groupIndex].points[0];
        const float stick = collision->penetrationCharacter * collision->stickScale;
        const float slide = -(collision->penetrationCharacter * collision->slideScale);
        float ax = collision->separation.x * slide, az = collision->separation.z * slide;
        float bx = collision->separation.x * stick, bz = collision->separation.z * stick;
        const float x = (bx + b.x) - (ax + a.x), z = (bz + b.z) - (az + a.z);
        const float distance = std::sqrt(z * z + x * x);
        if (distance > 0.00000011920929f) {
            const float midpoint = (distance + collision->separationLength) * 0.5f;
            const float correctionX = midpoint * x / distance - (b.x - a.x);
            const float correctionZ = midpoint * z / distance - (b.z - a.z);
            bx = correctionX * collision->stickScale * attacker->scale;
            bz = correctionZ * collision->stickScale * attacker->scale;
            ax = correctionX * (-collision->slideScale) * defender->scale;
            az = correctionZ * (-collision->slideScale) * defender->scale;
        }
        vfclMoveEntityWithContactHistory(defender, ax, az);
        vfclMoveEntityWithContactHistory(attacker, bx, bz);
    }
    if (double(collision->penetrationSpecial) < 0.05)
        defender->contact.groundSamples[0] = 0.0f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclPrepareContact(VFCL_t *collision, std::uint8_t first, std::uint8_t second) {
    FightActorRuntimePrefix *a = collision->game->actors[first], *b = collision->game->actors[second];
    collision->defender = a;
    collision->attacker = b;
    vfclSetContactCategory(a, b, collision);
    if (collision->flagsWord & 1U)
        return;
    collision->attackerMask = (a->hitStateFlags & 0x102U) == 0x102U && a->moveId != 41 ? vfGetMoveCategoryMaskFor(a) : 0;
    collision->defenderMask = (b->hitStateFlags & 0x102U) == 0x102U && b->moveId != 41 ? vfGetMoveCategoryMaskFor(b) : 0;
    vfclComputePenetrationAndOverlap(collision, a, b);
    float *history = ContactState(a).groundSamples;
    const float oldest = history[3], previous = history[2], recent = history[1], current = history[0];
    history[3] = previous;
    history[2] = recent;
    history[1] = current;
    FightCollisionValue<float>(a, 0x84c) = ((current + recent) + previous + oldest) / 4.0f;
    if (collision->penetrationSpecial + collision->penetrationCharacter + oldest != 0)
        vfclAdjustForSlideAndStick(collision);
}

#else

// Only the defender maintains this ordered four-frame penetration history.
// @verified - p
void vfclPrepareContact(VFCL_t *collision, std::uint8_t first, std::uint8_t second) {
    auto *const a = collision->game->actors[first];
    auto *const b = collision->game->actors[second];
    collision->defender = a;
    collision->attacker = b;
    vfclSetContactCategory(a, b, collision);
    if (collision->flags & 1)
        return;
    collision->attackerMask = (a->hitStateFlags & (VFHSF_HitTimeout | VFHSF_AttackActive)) == 0x102 && a->moveId != 41 ? vfGetMoveCategoryMaskFor(a) : 0;
    collision->defenderMask = (b->hitStateFlags & (VFHSF_HitTimeout | VFHSF_AttackActive)) == 0x102 && b->moveId != 41 ? vfGetMoveCategoryMaskFor(b) : 0;
    vfclComputePenetrationAndOverlap(collision, a, b);
    const float oldest = a->contact.groundSamples[3];
    const float previous = a->contact.groundSamples[2];
    const float recent = a->contact.groundSamples[1];
    const float current = a->contact.groundSamples[0];
    a->contact.groundSamples[3] = previous;
    a->contact.groundSamples[2] = recent;
    a->contact.groundSamples[1] = current;
    a->contact.averageGround = ((current + recent) + previous + oldest) * 0.25f;
    if (collision->penetrationSpecial + collision->penetrationCharacter + oldest != 0)
        vfclAdjustForSlideAndStick(collision);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfclPrepareContact(VFCL_t *collision, std::uint8_t first, std::uint8_t second);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static float vfResolveHitOutcomeAndImpulse(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target, float impact) {
    const std::uint32_t hit = target->hitStateFlags;
    if (target->moveFlags & 0x20000000U) {
        if (!(hit & 0x14000U))
            *static_cast<volatile std::uint32_t *>(&target->moveFlags) &= ~0x20000000U;
        impact *= 1.2f;
    } else if (hit & 0x100U) {
        impact *= 1.05f;
        FightCollisionValue<std::uint32_t>(source, 0x868) = 6;
        FightCollisionValue<std::uint32_t>(source, 0x320) = 0x0c000004U;
    } else if ((hit & 0x4010U) == 0x10U) {
        impact *= 1.1f;
    } else if ((target->motionFlags & 0x100U) && (hit & 1U)) {
        impact = impact * 1.03f + 2.0f;
        FightCollisionValue<std::uint32_t>(source, 0x868) = 37;
    } else if (target->activeMotionFlags & 4U) {
        const std::byte *profile = static_cast<std::byte *>(target->slot);
        profile += 0x175;
        if (static_cast<std::uint32_t>(static_cast<std::int8_t>(*profile)) == 1U)
            FightCollisionValue<float>(target, 0x9dc) = 25;
        else
            impact *= 1.3f;
    } else if ((source->motionFlags & 0x100000U) && (target->motionFlags & 0x80000U)) {
        const std::byte *profile = static_cast<std::byte *>(target->slot);
        profile += 0x175;
        if (static_cast<std::uint32_t>(static_cast<std::int8_t>(*profile)) == 2U)
            FightCollisionValue<float>(target, 0x9dc) = 20;
    } else if (target->secondaryMotionFlags & 16U) {
        const std::byte *profile = static_cast<std::byte *>(target->slot);
        profile += 0x175;
        if (static_cast<std::uint32_t>(static_cast<std::int8_t>(*profile)) == 3U)
            FightCollisionValue<float>(target, 0x9dc) = 3;
    } else if (target->hitStateFlags & 0x80000U) {
        impact *= 1.25f;
    } else if (!(hit & 0x14000U) &&
        static_cast<std::int16_t>(FightCollisionValue<std::int16_t>(source, 0x2b2) + source->facingAngle - target->temporaryFacingAngle + 0x4000) >= 0) {
        impact *= 1.1f;
        FightCollisionValue<std::uint32_t>(source, 0x868) = 37;
    }
    return impact;
}

#else

// Defender phase and native guard counters select the authored multiplier.
float vfResolveHitOutcomeAndImpulse(VFActor *self, VFActor *other, float impact) {
    const auto hit = other->hitStateFlags;
    if (other->moveFlags & VFMF_Unknown20000000) {
        if (!(hit & 0x14000))
            other->moveFlags &= ~VFMF_Unknown20000000;
        return impact * 1.2f;
    }
    if (hit & 0x100) {
        self->hitReactionSelection = 6u;
        self->action = 0x0c000004;
        return impact * 1.05f;
    }
    if ((hit & 0x4010) == 0x10)
        return impact * 1.1f;
    if ((other->motionFlags & VFMOF_Unknown100) && (hit & 1)) {
        self->hitReactionSelection = 37u;
        return impact * 1.03f + 2;
    }
    if (other->activeMotionFlags & VFSMF_Unknown4) {
        if (other->slot->parameter1a5 != 1)
            return impact * 1.3f;
        other->parameterB2c = 25;
        return impact;
    }
    if ((self->motionFlags & VFMOF_ReverseFacingOnHit) && (other->motionFlags & VFMOF_Unknown80000)) {
        if (other->slot->parameter1a5 == 2)
            other->parameterB2c = 20;
        return impact;
    }
    if (other->secondaryMotionFlags & VFSMF_Unknown10) {
        if (other->slot->parameter1a5 == 3) {
            constexpr float values[]{6, 6.4f, 6.8f, 7.2f, 7.6f, 8, 8.4f, 8.8f, 9.2f, 9.6f, 10, 10.4f, 10.8f, 11.2f, 11.6f, 12};
            other->parameterB2c = values[vfMotnNibbleGet(67)];
        }
        return impact;
    }
    if (hit & 0x80000)
        return float(double(impact) * 1.25);
    if ((hit & 0x14000) || static_cast<std::int16_t>(self->facingAngle + self->hitDirection - other->temporaryFacingAngle + 0x4000) < 0)
        return impact;
    self->hitReactionSelection = 37u;
    return impact * 1.1f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void vfApplyEnvAudio(float impact, FightActorRuntimePrefix *source) {
    static std::uint32_t vfEnvAudioParameterSounds[3] = {0, 968, 29};
    if (impact == 0 || (source->hitStateFlags & 0x40100U) == 0x40000U)
        return;
    const std::uint8_t parameter = FightCollisionValue<std::uint8_t>(source, 0x2ab);
    if (static_cast<std::uint32_t>(parameter - 1) < 2) {
        vfPlayFightSfx(vfEnvAudioParameterSounds[parameter], source);
        return;
    }
    std::int32_t level = 0;
    if (impact >= 20)
        level = 1;
    if (impact >= 40)
        ++level;
    if (static_cast<std::uint8_t>(source->moveId - 5) < 2) {
        if (!level)
            vfPlayFightSfx(22, source);
        else
            vfPlayFightSfx(23, source);
    } else
        vfPlayFightSfx(16, source);
}

#else

// The fixed PC sound owner ignores the legacy impact-volume parameter.
void vfApplyEnvAudio(float impact, VFActor *self) {
    if (impact == 0 || (self->hitStateFlags & (VFHSF_AttackActive | VFHSF_MidAirRecovery)) == 0x40000)
        return;
    const auto parameter = self->parameter2eb;
    std::uint32_t sound;
    if (std::uint32_t(parameter - 1) <= 1)
        sound = parameter == 1 ? 968 : 29;
    else if (std::uint8_t(self->moveId - 5) <= 1)
        sound = impact >= 20 ? 23 : 22;
    else
        sound = 16;
    vfPlayFightSfx(sound, self, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

struct HitProfile {
    float attackScale[2], impulseScale[4];
    std::int16_t angles[4];
};

static const HitProfile hitProfiles[7] = {{{1, 1}, {1, 1.2f, 1, 1.2f}, {11776, 6144, 9728, 4864}},
    {{1, 1}, {1.5f, 1.8f, 1.2f, 1.44f}, {13824, 9216, 12800, 6400}}, {{1, 1}, {2, 5, 1.2f, 1.44f}, {12288, 14336, 11264, 6144}},
    {{1, 1}, {2.5f, 2.5f, 2.5f, 2.5f}, {14592, 14592, 14592, 14592}}, {{1, 1}, {1.3f, 1.2f, 1.2f, 1.2f}, {11776, 4096, 9728, 4864}},
    {{1, 1}, {3.8f, 3.8f, 3.8f, 3.8f}, {9216, 9216, 9216, 9216}}, {{1, 1}, {2.5f, 2.5f, 2.5f, 2.5f}, {12800, 12800, 12800, 12800}}};

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfmaUpdateCollisionCounter(FightRuntimePrefix *game, std::uint8_t event, std::uint16_t move) {
    struct CollisionCounters {
        std::byte retained000[0x23c];
        std::int16_t total;
        std::uint16_t contacts[128], hits[128];
    };
    if (static_cast<std::uint8_t>(game->type - 4) < 2)
        return;
    switch (event) {
    case 0:
        if (reinterpret_cast<CollisionCounters *>(game)->contacts[move] < 0xffff)
            ++reinterpret_cast<CollisionCounters *>(game)->contacts[move];
        game->stateFlags &= ~0x440U;
        if (move != 96)
            ++reinterpret_cast<CollisionCounters *>(game)->total;
        break;
    case 1:
        if (reinterpret_cast<CollisionCounters *>(game)->hits[move] < 0xffff && !(game->stateFlags & 0x40U)) {
            ++reinterpret_cast<CollisionCounters *>(game)->hits[move];
            game->stateFlags |= 0x40U;
        }
        break;
    case 2:
        if (reinterpret_cast<CollisionCounters *>(game)->contacts[move] && reinterpret_cast<CollisionCounters *>(game)->hits[move] &&
            !(game->stateFlags & 0x400U)) {
            --reinterpret_cast<CollisionCounters *>(game)->hits[move];
            game->stateFlags |= 0x400U;
        }
        break;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void vfSetReactionKnockbackFromProfile(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target, float impulse) {
    const std::uint8_t *entry =
        reinterpret_cast<const std::uint8_t *>(MOTN_FindAnimationExtraRecord(static_cast<std::uint16_t>(g_vfSelectedReactionAnimation) & 0xfffU, 17));
    if (entry) {
        FightCollisionValue<std::uint16_t>(source, 0x87c) = *reinterpret_cast<const std::uint16_t *>(entry + 2);
        FightCollisionValue<float>(source, 0x878) = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint16_t>(entry[7] | entry[6] << 8) |
                                                        static_cast<std::uint16_t>(entry[5] | entry[4] << 8) << 16)) *
            0.000015258789f * impulse;
        return;
    }
    std::uint32_t value = FightCollisionValue<std::uint8_t>(source, 0x2c6);
    if (!(target->hitStateFlags & 0x10U)) {
        if (FightCollisionValue<std::uint32_t>(source, 0x868) != 6) {
            FightCollisionValue<float>(source, 0x874) = hitProfiles[value].angles[0];
            FightCollisionValue<float>(source, 0x870) = hitProfiles[value].impulseScale[0];
        } else {
            FightCollisionValue<float>(source, 0x874) = hitProfiles[value].angles[1];
            FightCollisionValue<float>(source, 0x870) = hitProfiles[value].impulseScale[1];
        }
    } else if (FightCollisionValue<std::uint32_t>(source, 0x868) != 6) {
        FightCollisionValue<float>(source, 0x874) = hitProfiles[value].angles[2];
        FightCollisionValue<float>(source, 0x870) = hitProfiles[value].impulseScale[2];
    } else {
        FightCollisionValue<float>(source, 0x874) = hitProfiles[value].angles[3];
        FightCollisionValue<float>(source, 0x870) = hitProfiles[value].impulseScale[3];
    }
    FightCollisionValue<std::int16_t>(source, 0x87c) = static_cast<std::int16_t>(FightCollisionValue<float>(source, 0x874));
    FightCollisionValue<float>(source, 0x878) = FightCollisionValue<float>(source, 0x870) * impulse;
    value = FightCollisionValue<std::uint8_t>(target, 0x3dd);
    if (value > 1) {
        value = (value - 1) * 0x580;
        const std::int32_t previous = FightCollisionValue<std::int16_t>(source, 0x87c);
        FightCollisionValue<std::int16_t>(source, 0x87c) -= value;
        const std::uint32_t minimum = static_cast<std::uint32_t>(previous) > 0x1000 ? 0x1000 : 0;
        if (static_cast<std::int16_t>(minimum) > FightCollisionValue<std::int16_t>(source, 0x87c))
            FightCollisionValue<std::int16_t>(source, 0x87c) = minimum;
        if (FightCollisionValue<std::int16_t>(source, 0x87c) > 0x3600)
            FightCollisionValue<std::int16_t>(source, 0x87c) = 0x3600;
    }
}

#else

// Authored opcode 17 takes precedence over the seven native hit profiles.
void vfSetReactionKnockbackFromProfile(VFActor *self, VFActor *other, float impulse) {
    const auto entry = FindLoadedMotionSecondaryEntry(std::uint32_t(g_vfSelectedReactionAnimation) & 0xfff, 17);
    if (!entry.empty()) {
        self->reactionVariantWords[0] = ReadFightData<std::uint16_t>(entry.data(), 2);
        const auto low = (std::uint32_t(std::to_integer<std::uint8_t>(entry[6])) << 8) | std::to_integer<std::uint8_t>(entry[7]);
        const auto high = (std::uint32_t(std::to_integer<std::uint8_t>(entry[4])) << 8) | std::to_integer<std::uint8_t>(entry[5]);
        const auto fixed = static_cast<std::int32_t>((high << 16) | low);
        self->hitImpulseMagnitude = (float(fixed) * 0.000015258789f) * impulse;
        return;
    }
    const auto &profile = hitProfiles[self->parameter30e];
    const auto index = ((other->hitStateFlags & VFHSF_SpecialGuardOrParry) ? 2u : 0u) + (self->hitReactionSelection == 6 ? 1u : 0u);
    std::int16_t angle = profile.angles[index];
    self->hitImpulseAngle = float(angle);
    self->hitImpulseMultiplier = profile.impulseScale[index];
    self->reactionVariantWords[0] = angle;
    self->hitImpulseMagnitude = impulse * profile.impulseScale[index];
    const auto repetitions = other->repeatedHitCount;
    if (repetitions > 1) {
        const std::int16_t minimum = std::uint32_t(std::int32_t(angle)) > 0x1000 ? 4096 : 0;
        angle = static_cast<std::int16_t>(angle - 1408 * repetitions + 1408);
        if (minimum > angle)
            angle = minimum;
        if (angle > 13824)
            angle = 13824;
        self->reactionVariantWords[0] = angle;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void vfAdjustReactionForGround(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target) {
    if (g_vfSelectedReactionAnimation == 349 && target->contactPosition.y <= source->targetGroundY + 0.9f) {
        const std::int32_t animation = target->contactPosition.y <= source->targetGroundY + 0.6f ? 351 : 91;
        g_vfSelectedReactionAnimation = animation;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// The attached stage object retains its own velocity and reset state.
void vfStageObjectApplyImpulse(VFActor *self) {
    const auto index = self->parameterAd7;
    vfStageObjectReset(index, nullptr);
    g_fightStageObjects[index]->object.velocity = {
        sin_fixed16(static_cast<std::int16_t>(self->hitDirection)) * (-.2f), .1f, cos_fixed16(self->hitDirection) * (-.2f)};
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void vfAssignKnockbackReaction(float impact, float impulse, FightActorRuntimePrefix *source, FightActorRuntimePrefix *target) {
    static float durationScale = 0.4f;
    std::uint32_t hit, received, parameter, type;
    std::uint8_t guard;
    FightRuntimePrefix *game;
    game = source->game;
    hit = source->hitStateFlags;
    received = target->hitStateFlags;
    parameter = FightCollisionValue<std::uint16_t>(source, 0x2b0);
    guard = FightCollisionValue<std::uint8_t>(source, 0x2a9);
    type = 0;
    if (FightCollisionValue<std::uint16_t>(source, 0x2da)) {
        const std::uint32_t *metadata = reinterpret_cast<const std::uint32_t *>(MOTN_GetDataForAnimId(FightCollisionValue<std::uint16_t>(source, 0x2da)));
        if (metadata)
            type = *metadata;
    }
    if (((received & 0x18000U) && !(target->moveFlags & 0x20000000U)) || ((received & 0x4010U) == 0x4000U && (hit & 0x400000U)))
        type = 0;
    else if (type & 0x4000U)
        type = 2;
    else if (received & 0x4000U)
        type = (hit & 0x400000U) ? 1 : 2;
    else if ((received & 0x1010U) || (parameter & 0x4000U))
        type = 2;
    else if (parameter & 0x400U)
        type = 3;
    else if ((parameter & 0x100U) && (received & 8U))
        type = 2;
    else if (guard != 1)
        type = 3;
    else
        type = received & 0x1000000U ? 2 : 3;
    if (target->activeMotionFlags & 0x1000000U)
        type = 2;
    if (type == 2) {
        if (!(target->hitStateFlags & 0x4000U) && (target->stateFlags & 2U) && !(target->moveFlags & 0x20000000U) && !(target->hitStateFlags & 0x1c000U) &&
            !(parameter & 0x200U))
            type = 3;
        if (type == 2 && FightCollisionValue<float>(target, 0x420) > 150.0f)
            type = 3;
    }
    if (type > 1) {
        float damage = impact;
        if (target->groupIndex == source->game->playerSlot) {
            damage = impact * (1.0f - game->playerAssistScale);
            if (damage < 1.0f)
                damage = 1.0f;
        }
        damage *= FightCollisionValue<float>(target, 0x9dc);
        const std::uint8_t flags = FightCollisionValue<std::uint8_t>(game->formations[target->groupIndex], 0x18);
        if ((((flags & 0x80U) && (source->secondaryMotionFlags & 0x4000008U)) || ((flags & 0x20U) && (source->secondaryMotionFlags & 0x4000000U))) &&
            !(g_pFightStage->battleFlags & 0x20U))
            damage = target->hp;
        if (target->hp <= FightCollisionValue<std::uint8_t>(source, 0x2aa) && (source->secondaryMotionFlags & 0x40000U))
            damage = target->hp - 1.0f;
        if (damage >= target->hp) {
            type = 4;
            target->stateFlags |= 0x40000U;
        }
        if (type == 4 || type == 2 || damage >= 40.0f)
            target->stateFlags |= 0x200U;
    }
    if (target->activeMotionFlags & 0x800000U) {
        switch (type) {
        case 3:
            switch (guard) {
            case 0:
            case 1:
            case 3:
                vfTossActorStageObject(target);
                break;
            case 2:
            case 5:
            case 6:
                vfReleaseActorStageObject(target);
                break;
            }
            break;
        case 0:
        case 1:
        case 2:
            vfReleaseActorStageObject(target);
            break;
        default:
            vfReleaseActorStageObject(target);
        }
    }
    switch (type) {
    case 1:
        FightCollisionValue<std::uint8_t>(target, 0x3dd) = 1;
    case 2: {
        if ((hit & 0x400000U) && (received & 0x810U) == 0x800U)
            goto settle;
        g_vfSelectedReactionAnimation = 7;
        vfclChooseReactionAnim(source, target);
        vfAdjustReactionForGround(source, target);
        FightCollisionValue<std::uint32_t>(target, 0x318) = g_vfSelectedReactionAnimation + 0x05000000U;
        if (impact > 14.0f)
            type = static_cast<std::uint8_t>(static_cast<std::int32_t>(impact - 14.0f + 0.5f));
        else
            type = 1;
        if (type > 30)
            type = 30;
        *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(target) + 0x3a0) =
            static_cast<std::int32_t>(*reinterpret_cast<float *>(reinterpret_cast<std::byte *>(target) + 0x270) * type);
        *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(target) + 0x790) |= 2U;
        vfSetReactionKnockbackFromProfile(source, target, impulse);
        break;
    }
    case 3: {
        g_vfSelectedReactionAnimation = FightCollisionValue<std::uint32_t>(source, 0x868) & 31U;
        if (vfCompareFightGuardLevels(target, source) > 10)
            g_vfSelectedReactionAnimation = 4;
        vfclChooseReactionAnim(source, target);
        FightCollisionValue<std::uint32_t>(target, 0x318) = g_vfSelectedReactionAnimation + 0x07000000U;
        type = FightCollisionValue<std::uint32_t>(source, 0x868) == 6 ? FightCollisionValue<std::uint8_t>(source, 0x2e6) :
                                                                        FightCollisionValue<std::uint8_t>(source, 0x2e5);
        float duration = type;
        if (!type)
            duration = durationScale * impact + 3.0f;
        type = static_cast<std::uint32_t>(duration * FightCollisionValue<float>(target, 0x270) + 0.5f);
        FightCollisionValue<std::uint16_t>(target, 0x3a0) = type;
        std::uint32_t stop = type + 4;
        if (FightCollisionValue<std::uint32_t>(source, 0x868) == 37 || FightCollisionValue<std::uint32_t>(source, 0x868) == 6)
            stop = type + 2;
        stop = static_cast<std::int32_t>(FightCollisionValue<float>(source, 0x270) * stop);
        game->retained44 = stop + 1;
        FightCollisionValue<std::uint16_t>(source, 0x87c) = 0;
        FightCollisionValue<float>(source, 0x878) = impulse;
        break;
    }
    case 0:
    settle:
        g_vfSelectedReactionAnimation = 9;
        vfclChooseReactionAnim(source, target);
        FightCollisionValue<std::uint32_t>(target, 0x318) = g_vfSelectedReactionAnimation + 0x05010000U;
        FightCollisionValue<std::uint16_t>(target, 0x3a0) = 0;
        FightCollisionValue<FightActorRuntimePrefix *>(target, 0x9c8) = source;
        vfHandleImpactSfxAndDefeat(target, static_cast<std::int32_t>(impact + 0.5f), 1);
        FightCollisionValue<std::uint8_t>(source, 0x87e) = 1;
        break;
    case 4:
        if (target->moveFlags & 4U)
            g_vfSelectedReactionAnimation = 7;
        else
            g_vfSelectedReactionAnimation = 8;
        vfclChooseReactionAnim(source, target);
        *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(target) + 0x318) = g_vfSelectedReactionAnimation + 0x05000000U;
        vfSetReactionKnockbackFromProfile(source, target, impulse);
        if (target->activeMotionFlags & 0x800U)
            *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(target->secondActor) + 0x318) = 0x01000000U;
        break;
    }
}

#else

// Hit classification publishes real queued reactions and paired impulses.
void vfAssignKnockbackReaction(float impact, float impulse, VFActor *self, VFActor *other) {
    auto &game = *self->game;
    const auto hit = self->hitStateFlags, received = other->hitStateFlags;
    const auto parameter = self->hitParameter;
    const auto guard = static_cast<std::uint8_t>(self->guardState);
    std::uint32_t authoredFlags = 0;
    if (self->someAnimation[0]) {
        const auto metadata = FindLoadedMotionMetadata(self->someAnimation[0]);
        if (!metadata.empty())
            authoredFlags = ReadFightData<std::uint32_t>(metadata.data(), 0);
    }
    std::uint32_t type;
    if ((received & 0x18000) && !(other->moveFlags & VFMF_Unknown20000000))
        type = 0;
    else if ((received & 0x4010) == 0x4000 && (hit & 0x400000))
        type = 0;
    else if (authoredFlags & 0x4000)
        type = 2;
    else if (other->motionFlags & VFMOF_Unknown4000)
        type = 2 - ((hit & 0x400000) != 0);
    else if (parameter & 0x4000)
        type = 2;
    else if (parameter & 0x400)
        type = 3;
    else if ((parameter & 0x100) && (received & 8))
        type = 2;
    else if (guard == 1)
        type = ((received & 0x01000000) == 0 ? 1u : 0u) | 2u;
    else
        type = 3;
    const auto active = other->activeMotionFlags;
    if (active & 0x1000000)
        type = 2;
    if (type == 2) {
        if (!(other->motionFlags & VFMOF_Unknown4000) && (other->stateFlags & VFSF_Unknown2) && !(other->moveFlags & VFMF_Unknown20000000) &&
            !(other->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) && !(parameter & 0x200))
            type = 3;
        else if (other->knockbackBaseParameter > 150)
            type = 3;
    }
    if (type >= 2) {
        float damage = impact;
        if (other->groupIndex == game.playerSlot) {
#if SHENMUE_IS_PC
            const float assisted = (1 - game.playerAssistScale) * impact;
            damage = 1.0f > assisted ? 1.0f : assisted;
#else
            damage = std::fmax(1.0f, (1 - game.playerAssistScale) * impact);
#endif
        }
        damage *= other->parameterB2c;
        const auto flags = game.groups[other->groupIndex]->selectFlags;
        if ((((flags & 0x80) && (self->secondaryMotionFlags & (VFSMF_Unknown8 | VFSMF_Unknown4000000))) ||
                ((flags & 0x20) && (self->secondaryMotionFlags & VFSMF_Unknown4000000))) &&
            !(g_pFightStage->battleFlags & 0x20))
            damage = other->hp;
        if (damage >= other->hp && (self->secondaryMotionFlags & VFSMF_Unknown40000))
            damage = other->hp - 1;
        if (damage >= other->hp) {
            other->stateFlags |= VFSF_Unknown40000;
            type = 4;
        }
        if (damage >= other->hp || !((type - 2) & 0xfffffffdu) || damage >= 40)
            other->stateFlags |= VFSF_Unknown200;
    }
    if (active & 0x800000) {
        if (type == 3) {
            if (guard == 0 || guard == 1 || guard == 3)
                vfStageObjectApplyImpulse(other);
            else if (guard == 2 || guard == 5 || guard == 6)
                vfStageObjectReset(other->parameterAd7, nullptr);
        } else
            vfStageObjectReset(other->parameterAd7, nullptr);
    }
    if (type == 4) {
        g_vfSelectedReactionAnimation = 8;
        vfclChooseReactionAnim(self, other);
        other->parameter370 = std::uint32_t(g_vfSelectedReactionAnimation) + 0x05000000;
        vfSetReactionKnockbackFromProfile(self, other, impulse);
        if (other->activeMotionFlags & VFSMF_Unknown800)
            other->secondActor->parameter370 = 0x01000000;
        return;
    }
    if (type == 3) {
        g_vfSelectedReactionAnimation = self->hitReactionSelection & 0x1f;
        if (static_cast<std::int8_t>(other->guardLevel - self->guardLevel) > 10)
            g_vfSelectedReactionAnimation = 4;
        vfclChooseReactionAnim(self, other);
        other->parameter370 = std::uint32_t(g_vfSelectedReactionAnimation) + 0x07000000;
        std::uint32_t frames = self->hitReactionSelection == 6 ? self->parameter32e : self->parameter32d;
        const float duration = frames ? float(frames) : .4f * impact + 3;
#if SHENMUE_IS_PC
        frames = static_cast<std::uint32_t>(_mm_cvttsd_si64(_mm_set_sd(double(duration * other->inverseScale) + .5)));
#else
        frames = std::uint32_t(static_cast<std::int32_t>(duration * other->inverseScale + .5f));
#endif
        if (frames > 15)
            frames = 15;
        other->hitPhaseWords[0] = static_cast<std::uint16_t>(frames);
        const auto kind = self->hitReactionSelection;
        const std::uint32_t stop = frames + (kind == 37 || kind == 6 ? 2 : 4);
        game.hitStopFrames = static_cast<std::uint8_t>(static_cast<std::int32_t>(float(stop) * self->inverseScale) + 1);
        self->reactionVariantWords[0] = std::uint16_t{0};
        self->hitImpulseMagnitude = impulse;
        return;
    }
    if (type == 1)
        other->repeatedHitCount = std::uint8_t{1};
    if (type == 0 || ((hit & 0x400000) && (received & 0x810) == 0x800)) {
        g_vfSelectedReactionAnimation = 9;
        vfclChooseReactionAnim(self, other);
        other->hitSourceActor = self;
        other->parameter370 = std::uint32_t(g_vfSelectedReactionAnimation) + 0x05010000;
        other->hitPhaseWords[0] = std::uint16_t{0};
        vfHandleImpactSfxAndDefeat(other, static_cast<std::int16_t>(static_cast<std::int32_t>(double(impact) + .5)), 1);
        self->hitImpulseSuppressed = std::uint8_t{1};
        return;
    }
    g_vfSelectedReactionAnimation = 7;
    vfclChooseReactionAnim(self, other);
    if (g_vfSelectedReactionAnimation == 349 && double(self->targetGroundY) + .9 >= double(other->contactPosition.y)) {
        g_vfSelectedReactionAnimation = double(self->targetGroundY) + .6 >= double(other->contactPosition.y) ? 351 : 91;
    }
    other->parameter370 = std::uint32_t(g_vfSelectedReactionAnimation) + 0x05000000;
    std::uint32_t frames = 1;
    if (impact > 14) {
        frames = static_cast<std::uint8_t>(static_cast<std::int32_t>(impact - 14 + .5f));
        if (frames > 30)
            frames = 30;
    }
    other->contact.priorityMasks[0] = other->contact.priorityMasks[0] | 2u;
    other->hitPhaseWords[0] = static_cast<std::uint16_t>(static_cast<std::int32_t>(float(frames) * other->inverseScale));
    vfSetReactionKnockbackFromProfile(self, other, impulse);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define FIGHT_ACTOR_VALUE(T, actor, offset) (*reinterpret_cast<T *>(reinterpret_cast<std::byte *>(actor) + (offset)))

// @verified - d
static inline void vfRestoreAuthoredHitProgram(FightActorRuntimePrefix *actor, std::uint16_t flag) {
    if (!(FIGHT_ACTOR_VALUE(std::uint16_t, actor, 0x5c8) & flag))
        return;
    FIGHT_ACTOR_VALUE(const void *, actor, 0x5b8) = FIGHT_ACTOR_VALUE(const void *, actor, 0x5cc);
    FIGHT_ACTOR_VALUE(std::uint16_t, actor, 0x5bc) = FIGHT_ACTOR_VALUE(std::uint16_t, actor, 0x5c0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfclApplyHitResolution(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target) {
    FightRuntimePrefix *game = target->game;
    const std::uint32_t sourceHit = source->hitStateFlags, hit = target->hitStateFlags, active = target->activeMotionFlags;
    std::int32_t guarded;
    ++FIGHT_ACTOR_VALUE(std::int16_t, source, 0x3d6);
    const std::int8_t state = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x32b);
    if (state == 14)
        return;
    const std::uint32_t guard = FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x2a9);
    if (state == 23) {
        --FIGHT_ACTOR_VALUE(std::int16_t, source, 0x3d6);
        return;
    }
    if ((!guard && (active & 0x2000000U)) ||
        ((hit & 0x8000U) && target->animationTime >= static_cast<float>(FIGHT_ACTOR_VALUE(std::uint16_t, target, 0x290)) && !(hit & 4U))) {
        --FIGHT_ACTOR_VALUE(std::int16_t, source, 0x3d6);
        return;
    }
    if (FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x2a9) < 2) {
        if ((target->moveFlags & 4U) && target->position.y < target->targetGroundY + 0.5f)
            return;
        if ((target->activeMotionFlags & 1U) && target->animationTime > static_cast<float>(FIGHT_ACTOR_VALUE(std::uint16_t, target, 0x292)))
            return;
    }
    const std::int16_t direction = FIGHT_ACTOR_VALUE(std::int16_t, source, 0x2b2) + source->facingAngle;
    float impact = FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x2aa);
    float impulse = std::sqrt(impact * 0.033333335f) * 30.0f * 0.0098000001f;
    if (hit & 0x100U)
        FIGHT_ACTOR_VALUE(float, source, 0x870) = hitProfiles[FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x2c6)].attackScale[1];
    else
        FIGHT_ACTOR_VALUE(float, source, 0x870) = hitProfiles[FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x2c6)].attackScale[0];
    impulse *= FIGHT_ACTOR_VALUE(float, source, 0x870);
    if ((sourceHit & 0x1010U) == 0x1010U && static_cast<std::int16_t>(FIGHT_ACTOR_VALUE(std::int16_t, source, 0x17c) - direction + 0x4000) >= 0) {
        FIGHT_ACTOR_VALUE(float, source, 0x454) *= 0.5f;
        FIGHT_ACTOR_VALUE(float, source, 0x45c) *= 0.5f;
    }
    std::int16_t delta = target->actorFacingAngles[source->groupIndex] - target->temporaryFacingAngle;
    delta = std::fabs(delta);
    std::int16_t maximum;
    if (FIGHT_ACTOR_VALUE(std::uint16_t, target, 2) == 13)
        maximum = 0x5550;
    else
        maximum = 0x2000;
    std::uint16_t type;
    if ((FIGHT_ACTOR_VALUE(std::uint32_t, source, 0x2b0) & 0x1200U) == 0x1200U)
        type = 1;
    else if ((active & 0x204000U) && delta < maximum) {
        if ((sourceHit & 0x81010U) && !(FIGHT_ACTOR_VALUE(std::uint32_t, source, 0x2c8) & 0x400000U))
            type = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x305) == 0;
        else
            type = 0;
    } else if ((hit & 0x2000U) && delta < 0x2000) {
        const std::uint32_t parameter = FIGHT_ACTOR_VALUE(std::uint16_t, source, 0x2b0);
        if (!(hit & 8U)) {
            type = 1;
            if (guard != 2 && guard != 5 && guard != 6 && ((parameter >> 9) & 1) == 0)
                type = (source->activeMotionFlags >> 31) != 0;
        } else if (FIGHT_ACTOR_VALUE(std::uint32_t, source, 0x2c8) & 2U)
            type = 4;
        else if (guard == 5 || guard == 6 || guard == 2)
            type = (parameter >> 9) & 1;
        else if (parameter & 0x2000U)
            type = 3;
        else if (guard != 1)
            type = 1;
        else
            type = 2;
    } else
        type = 1;
    if (source->activeMotionFlags & 0x20000U)
        type = 1;
    guarded = 0;
    switch (type) {
    case 4: {
        FIGHT_ACTOR_VALUE(std::uint32_t, target, 0x318) = FIGHT_ACTOR_VALUE(std::uint32_t, source, 0x2cc);
        const std::byte *entry = MOTN_FindAnimationExtraRecord(FIGHT_ACTOR_VALUE(std::uint32_t, target, 0x318) & 0xfff, 5);
        FIGHT_ACTOR_VALUE(std::int32_t, source, 0x318) = *reinterpret_cast<const std::int16_t *>(entry + 2) + 0x09000000;
        FIGHT_ACTOR_VALUE(std::byte, source, 0x2aa) = entry[1];
        return;
    }
    case 1:
        if (source->formationFlags & 8U)
            source->runtimeFlags |= 8U;
        FIGHT_ACTOR_VALUE(std::int32_t, source, 0x868) = 5;
        break;
    case 2:
        impact *= 0.75f;
        impulse *= 0.75f;
        FIGHT_ACTOR_VALUE(std::int32_t, source, 0x868) = 5;
        break;
    case 3:
        g_vfSelectedReactionAnimation = 3;
        guarded = 1;
        break;
    case 0:
        --FIGHT_ACTOR_VALUE(std::int16_t, source, 0x3d6);
        ++FIGHT_ACTOR_VALUE(std::int16_t, source, 0x3d8);
        g_vfSelectedReactionAnimation = 0;
        guarded = 1;
        break;
    }
    FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x87e) = 0;
    FIGHT_ACTOR_VALUE(float, source, 0x878) = impulse;
    FIGHT_ACTOR_VALUE(FightActorRuntimePrefix *, target, 0x9c8) = source;
    if (!guarded) {
        if (source->motionFlags & 0x80000U)
            target->activeMotionFlags |= 0x1000000U;
        FIGHT_ACTOR_VALUE(std::int32_t, source, 0x320) = 0x0c000001;
        vfRestoreAuthoredHitProgram(source, 1);
        if (!FIGHT_ACTOR_VALUE(std::int16_t, source, 2)) {
            if (FIGHT_ACTOR_VALUE(std::int8_t, source, 0x87f) != 103)
                vfmaUpdateCollisionCounter(game, 1, FIGHT_ACTOR_VALUE(std::int8_t, source, 0x87f));
            else
                vfmaUpdateCollisionCounter(game, 1, FIGHT_ACTOR_VALUE(std::int8_t, source, 0x880));
            FIGHT_ACTOR_VALUE(std::int8_t, source, 0x87f) = 96;
        }
        impact *= vfclCalculateGuardKnockbackFactor(source, 1.0f);
        impact = vfResolveHitOutcomeAndImpulse(source, target, impact);
        if (FIGHT_ACTOR_VALUE(std::uint16_t, target, 2) == 14 && static_cast<std::int8_t>(game->strikesLeft) > 0) {
            --game->strikesLeft;
            FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59a) = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59d);
            FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59b) = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59d);
            vfResetReactionProgram(source);
            if (!game->strikesLeft) {
                game->elapsedFrames = 30;
                source->game->stateFlags |= 0x20U;
            }
        }
        vfApplyEnvAudio(impact, source);
        vfAssignKnockbackReaction(impact, impulse, source, target);
        if (!FIGHT_ACTOR_VALUE(std::int16_t, target, 2) && (FIGHT_ACTOR_VALUE(std::uint32_t, target, 0x318) & 0xfffU) == 0x1b6U)
            FIGHT_ACTOR_VALUE(std::uint32_t, target, 0x318) = (FIGHT_ACTOR_VALUE(std::uint32_t, target, 0x318) & ~0xfffU) | 0x5fU;
        target->hitResolutionFlags |= 2U;
        if (!FIGHT_ACTOR_VALUE(std::int16_t, target, 2) && camCtrlGetInterpMode() == 3)
            camCtrlShakeFightCamera(0.12f);
    } else {
        const std::uint8_t sourceMode = source->retainedMode, targetMode = target->retainedMode;
        if (!(target->stateFlags & 12U) && (sourceMode > targetMode || (sourceMode == targetMode && !(source->secondaryMotionFlags & 0x4000008U)))) {
            FIGHT_ACTOR_VALUE(std::int32_t, source, 0x868) = 0;
            FIGHT_ACTOR_VALUE(std::int32_t, source, 0x320) = 0x0c000002;
            vfclChooseReactionAnim(source, target);
            FIGHT_ACTOR_VALUE(std::int32_t, target, 0x318) = g_vfSelectedReactionAnimation + 0x04000000;
            if (source->activeMotionFlags & 0x10000000U) {
                FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59a) = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59d);
                FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59b) = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59d);
                vfResetReactionProgram(source);
            }
        } else {
            FIGHT_ACTOR_VALUE(std::int32_t, source, 0x868) = 2;
            g_vfSelectedReactionAnimation = 2;
            vfclChooseReactionAnim(source, target);
            FIGHT_ACTOR_VALUE(std::int32_t, target, 0x318) = g_vfSelectedReactionAnimation + 0x21000000;
            if ((vfSelectHitReactionSequence(source) & 0x33U) || FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x305) == 3) {
                FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59a) = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59d);
                FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59b) = FIGHT_ACTOR_VALUE(std::int8_t, source, 0x59d);
                vfResetReactionProgram(source);
            }
        }
        FIGHT_ACTOR_VALUE(std::int16_t, target, 0x3da) = FIGHT_ACTOR_VALUE(std::int16_t, source, 0x3d4);
        vfRestoreAuthoredHitProgram(source, 2);
        if (target->formationFlags & 0x20U)
            target->runtimeFlags |= 0x20U;
        std::int32_t frames = FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x2e4);
        if (!frames) {
            frames = static_cast<std::uint8_t>(static_cast<std::int32_t>(impact * 0.60000002f + 3.0f + 0.5f));
            const std::uint32_t limit = ScaleFightTransitionFrames(FIGHT_ACTOR_VALUE(float, source, 0x270), 10);
            if (static_cast<std::uint16_t>(frames) > limit)
                frames = static_cast<std::uint8_t>(ScaleFightTransitionFrames(FIGHT_ACTOR_VALUE(float, source, 0x270), 10));
        }
        FIGHT_ACTOR_VALUE(std::uint16_t, target, 0x3a0) = frames;
        frames = static_cast<std::uint8_t>(frames + 4);
        const std::int32_t stop = FIGHT_ACTOR_VALUE(float, target, 0x270) * static_cast<float>(frames);
        game->retained44 = stop;
        FIGHT_ACTOR_VALUE(std::int16_t, source, 0x87c) = 0;
        impact = 0;
        target->hitResolutionFlags |= 1U;
    }
    FIGHT_ACTOR_VALUE(float, target, 0x46c) = 0;
    FIGHT_ACTOR_VALUE(float, target, 0x470) = 0;
    FIGHT_ACTOR_VALUE(float, target, 0x474) = 0;
    if (FIGHT_ACTOR_VALUE(std::uint8_t, source, 0x87e))
        return;
    impulse = FIGHT_ACTOR_VALUE(float, target, 0x420);
    impulse = (impulse - 60.0f) * g_fightKnockbackMaterialBlend + 60.0f;
    impulse = std::sqrt(FIGHT_ACTOR_VALUE(float, source, 0x878) / impulse);
    const float vertical = CalculateFixedSine(FIGHT_ACTOR_VALUE(std::int16_t, source, 0x87c)) * impulse;
    impulse *= CalculateFixedCosine(FIGHT_ACTOR_VALUE(std::int16_t, source, 0x87c));
    if (source->game->type != 9 && FIGHT_ACTOR_VALUE(std::uint16_t, target, 2) != 32) {
        FIGHT_ACTOR_VALUE(float, target, 0x470) = target->game->knockbackAmount * target->game->knockbackAmount * vertical;
        FIGHT_ACTOR_VALUE(float, target, 0x46c) = target->game->knockbackAmount * target->game->knockbackAmount * (CalculateFixedSine(-direction) * impulse);
        FIGHT_ACTOR_VALUE(float, target, 0x474) = target->game->knockbackAmount * target->game->knockbackAmount * (-CalculateFixedCosine(-direction) * impulse);
    }
    if (!guarded) {
        const std::uint8_t flags = FIGHT_ACTOR_VALUE(std::uint8_t, game->formations[target->groupIndex], 0x18);
        if (((flags & 0x80U) && (source->secondaryMotionFlags & 0x4000008U)) || ((flags & 0x20U) && (source->secondaryMotionFlags & 0x4000000U)))
            if (!(g_pFightStage->battleFlags & 0x20U))
                impact = target->hp + 1.0f;
    }
    vfHandleImpactSfxAndDefeat(target, static_cast<std::int32_t>(impact + 0.5f), 1);
}

#else

// This is the actual hit producer, including guard and paired-reaction paths.
// @verified - p
void vfclApplyHitResolution(VFActor *self, VFActor *other) {
    auto &game = *other->game;
    const auto hit = other->hitStateFlags, active = other->activeMotionFlags, selfHit = self->hitStateFlags;
    ++self->activeContactCount;
    const auto state = static_cast<std::uint8_t>(self->queuedAction >> 24);
    if (state == 14)
        return;
    const auto guard = static_cast<std::uint8_t>(self->guardState);
    if (state == 23 || (!self->guardState && (active & 0x2000000)) ||
        ((hit & 0x8000) && other->animationTime >= float(static_cast<std::uint16_t>(other->parameter2d0)) && !(hit & 4))) {
        --self->activeContactCount;
        return;
    }
    if (guard <= 1 &&
        (((other->moveFlags & VFMF_Airborne) && double(other->targetGroundY) + .5 > double(other->position.y)) ||
            ((active & 1) && other->animationTime > float(static_cast<std::uint16_t>(other->reactionMiddleFrame)))))
        return;
    const std::uint16_t direction = self->facingAngle + self->hitDirection;
    float impact = float(static_cast<std::uint8_t>(self->hitDamage));
    const float acceleration = std::sqrt(impact * .033333335f);
    const auto &profile = hitProfiles[self->parameter30e];
    const float multiplier = profile.attackScale[(hit & 0x100) != 0];
    self->hitImpulseMultiplier = multiplier;
    float impulse = ((acceleration * 30) * .0098000001f) * multiplier;
    if ((selfHit & 0x1010) == 0x1010 && static_cast<std::int16_t>(self->parameter1b4 - direction + 0x4000) >= 0) {
        self->knockbackVelocity.x *= .5f;
        self->knockbackVelocity.z *= .5f;
    }
    const auto difference = static_cast<std::int16_t>(other->contactAngles[self->groupIndex] - other->temporaryFacingAngle);
    const auto delta = static_cast<std::int16_t>(difference < 0 ? -int(difference) : int(difference));
    const std::int16_t maximum = other->fightStyle == 13 ? 21840 : other->fightStyle == 8 ? 11008 : 8192;
    const auto parameter = self->hitParameter;
    std::uint8_t type = 1;
    if ((parameter & 0x1200) != 0x1200) {
        if ((active & 0x204000) && delta < maximum) {
            type = (selfHit & 0x81010) && !(self->reactionEntryFlags & 0x400000) ? self->parameter34d == 0 : 0;
        } else if ((hit & 0x2000) && delta < 8192) {
            if (hit & 8) {
                if (self->reactionEntryFlags & 2)
                    type = 4;
                else if (guard == 2 || std::uint32_t(guard - 5) <= 1)
                    type = (parameter & 0x200) != 0;
                else if (parameter & 0x2000)
                    type = 3;
                else
                    type = (guard == 1) + 1;
            } else if (guard != 2 && std::uint32_t(guard - 5) > 1 && !(parameter & 0x200) && static_cast<std::int32_t>(self->activeMotionFlags) >= 0)
                type = 0;
        }
    }
    if (self->activeMotionFlags & VFSMF_Unknown20000)
        type = 1;
    bool guarded = false;
    switch (type) {
    case 0:
        --self->activeContactCount;
        ++self->parameter44c;
        g_vfSelectedReactionAnimation = 0;
        guarded = true;
        break;
    case 1:
        if (self->formationFlags & 8)
            self->formationRuntimeFlags |= 8;
        self->hitReactionSelection = 5u;
        break;
    case 2:
        self->hitReactionSelection = 5u;
        impact = float(double(impact) * .75);
        impulse = float(double(impulse) * .75);
        break;
    case 3:
        g_vfSelectedReactionAnimation = 3;
        guarded = true;
        break;
    case 4: {
        const auto action = self->reactionEntryMask;
        other->parameter370 = action;
        const auto entry = FindLoadedMotionSecondaryEntry(action & 0xfff, 5);
        self->parameter370 = std::uint32_t(std::int32_t(ReadFightData<std::int16_t>(entry.data(), 2)) + 0x09000000);
        self->hitDamage = std::to_integer<std::int8_t>(entry[1]);
        return;
    }
    default:
        break;
    }
    self->hitImpulseMagnitude = impulse;
    self->hitImpulseSuppressed = std::uint8_t{0};
    other->hitSourceActor = self;
    const auto restoreProgram = [self](std::uint8_t flag) {
        if (!(self->currentHitActionBytes[0] & flag))
            return;
        self->savedRootReaction.program = reinterpret_cast<const std::uint8_t *>(self->savedHitReactionProgram);
        self->savedRootReaction.words[0] = self->pendingReactionTicks;
    };
    if (!guarded) {
        if (self->motionFlags & VFMOF_Unknown80000)
            other->activeMotionFlags |= VFSMF_Unknown1000000;
        self->action = 0x0c000001;
        restoreProgram(1);
        if (self->fightStyle == 0) {
            auto move = self->hitImpulseMove;
            const std::uint16_t encoded =
                move == 103 ? static_cast<std::int16_t>(std::bit_cast<std::int8_t>(self->reactionImpulseBytes[0])) : static_cast<std::int16_t>(move);
            vfmaUpdateCollisionCounter(&game, VFCollisionCounterEvent::HitConfirmed, encoded);
            self->hitImpulseMove = std::uint8_t{96};
        }
        impact *= vfclCalculateGuardKnockbackFactor(self);
        impact = vfResolveHitOutcomeAndImpulse(self, other, impact);
        vfApplyEnvAudio(impact, self);
        if (other->fightStyle == 14) {
            if (static_cast<std::int8_t>(game.strikesLeft) <= 0)
                impact = 0;
            else {
                --game.strikesLeft;
                const auto selected = self->reactionCursors.deferred[0];
                self->reactionCursors.tracked[0] = selected;
                self->reactionCursors.tracked[1] = selected;
                vfResetReactionProgram(self);
                if (!game.strikesLeft) {
                    game.guardTimer = 30;
                    self->game->mode |= 0x20;
                }
            }
        }
        vfAssignKnockbackReaction(impact, impulse, self, other);
        if (other->fightStyle == 0 && (other->parameter370 & 0xfff) == 0x1b6)
            other->parameter370 = (other->parameter370 & ~std::uint64_t{0xfff}) | 0x5f;
        other->hitResolutionFlags |= 2u;
        if (other->fightStyle == 0 && camCtrlGetInterpMode() == CameraMotionMode::FirstPerson)
            camCtrlSetRotationXYZ(0, .12f, .5f);
    } else {
        bool resetSlots = false;
        const auto firstLevel = static_cast<std::uint8_t>(self->guardLevel);
        const auto secondLevel = static_cast<std::uint8_t>(other->guardLevel);
        if ((other->stateFlags & (VFSF_Unknown4 | VFSF_Unknown8)) ||
            (firstLevel <= secondLevel && (firstLevel != secondLevel || (self->secondaryMotionFlags & (VFSMF_Unknown8 | VFSMF_Unknown4000000))))) {
            self->hitReactionSelection = 2u;
            g_vfSelectedReactionAnimation = 2;
            vfclChooseReactionAnim(self, other);
            other->parameter370 = std::uint32_t(g_vfSelectedReactionAnimation) + 0x21000000;
            resetSlots = (vfSelectHitReactionSequence(self) & 0x33) || self->parameter34d == 3;
        } else {
            self->hitReactionSelection = 0u;
            self->action = 0x0c000002;
            vfclChooseReactionAnim(self, other);
            other->parameter370 = std::uint32_t(g_vfSelectedReactionAnimation) + 0x04000000;
            resetSlots = (self->activeMotionFlags & VFSMF_Unknown10000000) != 0;
        }
        if (resetSlots) {
            const auto selected = self->reactionCursors.deferred[0];
            self->reactionCursors.tracked[0] = selected;
            self->reactionCursors.tracked[1] = selected;
            vfResetReactionProgram(self);
        }
        other->parameter44e = self->lastHitMove;
        restoreProgram(2);
        if (other->formationFlags & 0x20)
            other->formationRuntimeFlags |= 0x20;
        std::uint8_t frames = self->parameter32c;
        if (!frames) {
            frames = static_cast<std::uint8_t>(static_cast<std::int32_t>((impact * .60000002f + 3) + .5f));
            const auto maximumFrames = static_cast<std::uint16_t>(vfScaleFrameCount(self->inverseScale, 10));
            if (frames > maximumFrames)
                frames = static_cast<std::uint8_t>(vfScaleFrameCount(self->inverseScale, 10));
        }
        impact = 0;
        other->hitPhaseWords[0] = std::uint16_t{frames};
        game.hitStopFrames = static_cast<std::uint8_t>(static_cast<std::int32_t>(float(static_cast<std::uint8_t>(frames + 4)) * other->inverseScale));
        self->reactionVariantWords[0] = std::uint16_t{0};
        other->hitResolutionFlags |= 1u;
    }
    other->pendingKnockback = {};
    if (self->hitImpulseSuppressed)
        return;
    const float magnitude = std::sqrt(self->hitImpulseMagnitude / ((other->knockbackBaseParameter - 60) * .5f + 60));
    const auto pitch = std::bit_cast<std::int16_t>(self->reactionVariantWords[0]);
    const float vertical = sin_fixed16(pitch) * magnitude, horizontal = cos_fixed16(pitch) * magnitude;
    if (self->game->type != 9 && other->fightStyle != 32) {
        const auto scale = other->game->knockbackAmount;
        other->pendingKnockback.y = (scale * scale) * vertical;
        other->pendingKnockback.x = (scale * scale) * (sin_fixed16(static_cast<std::int16_t>(-direction)) * horizontal);
        other->pendingKnockback.z = (scale * scale) * -(cos_fixed16(static_cast<std::uint16_t>(-direction)) * horizontal);
    }
    const auto flags = game.groups[other->groupIndex]->selectFlags;
    if (!guarded &&
        (((flags & 0x80) && (self->secondaryMotionFlags & (VFSMF_Unknown8 | VFSMF_Unknown4000000))) ||
            ((flags & 0x20) && (self->secondaryMotionFlags & VFSMF_Unknown4000000))) &&
        !(g_pFightStage->battleFlags & 0x20))
        impact = other->hp + 1;
    vfHandleImpactSfxAndDefeat(other, static_cast<std::int16_t>(static_cast<std::int32_t>(impact + .5f)), 1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void vfClGetBonePos(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target) {
    std::uint32_t &excluded = FightCollisionValue<std::uint32_t>(target, 0x798);
    excluded = 0;
    if ((source->hitStateFlags & 0x102U) != 0x100U)
        return;
    const std::uint8_t guard = FightCollisionValue<std::uint8_t>(source, 0x2a9);
    if ((target->hitStateFlags & 0x4000U) && guard != 4) {
        const float height = target->targetGroundY + 0.4f;
        const VFCollisionShape *shapes = BodyShapes(target);
        std::uint32_t mask = 0;
        for (std::int32_t index = 0; index < 10; ++index) {
            Vector3 position;
            mtwkGetBonePosition(target->motion, static_cast<MotionBoneId>(shapes[index].bone), &position);
            if (position.y < height)
                mask |= shapes[index].mask;
        }
        excluded = mask;
    } else if (FightCollisionValue<std::int8_t>(target, 0x3de) &&
        (((target->hitStateFlags & 0x1c000U) && guard != 4) || (!(target->hitStateFlags & 0x1c000U) && guard == 4))) {
        excluded = 0xffffffffU;
    }
}

#else

// Guard exclusion uses the motion owner's solved bone translations.
// @verified - p
void vfClGetBonePos(std::uint8_t index, VFActor *first, VFActor *second) {
    VFActor *const self = index ? first : second;
    VFActor *const source = index ? second : first;
    self->contact.excludedMask = 0u;
    if ((source->hitStateFlags & (VFHSF_HitTimeout | VFHSF_AttackActive)) != 0x100)
        return;
    const auto guard = static_cast<std::uint8_t>(source->guardState);
    if ((self->hitStateFlags & VFHSF_SpecialGuardOrParry) && guard != 4) {
        const float height = 0.4f + self->targetGroundY;
        const auto *const shapes = reinterpret_cast<const VFCollisionShape *>(self->parameterAd8);
        std::uint32_t mask = 0;
        Vector3 position;
        for (std::size_t i = 0; i != 10; ++i) {
            (void)mtwkGetBonePosition(self->motion, static_cast<MotionBoneId>(shapes[i].bone), &position);
            if (height > position.y)
                mask |= shapes[i].tag;
        }
        self->contact.excludedMask = mask;
    } else if (self->footContactState &&
        (((self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) && guard != 4) ||
            (!(self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) && guard == 4))) {
        self->contact.excludedMask = 0xffffffffu;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static bool vfClGetBoneStates(std::uint8_t first, std::uint8_t second, VFCL_t *collision) {
    FightActorRuntimePrefix *a = collision->game->actors[first], *b = collision->game->actors[second];
    const std::uint8_t bit = static_cast<std::uint8_t>(1U << second);
    std::uint8_t &latch = ContactState(a).flags;
    if ((!(b->activeMotionFlags & 0x4000U) && ((b->activeMotionFlags & 0x200U) || (b->moveFlags & 0x40000U))) || !(a->hitStateFlags & 0x100U) ||
        (b->hitStateFlags & 0x840000U) || ((b->hitStateFlags & 0x80000U) && (!b->secondActor || (b->secondActor->hitStateFlags & 0x40000U)))) {
        latch &= static_cast<std::uint8_t>(~bit);
        return false;
    }
    if (FightCollisionValue<std::int16_t>(a, 0x42c) != FightCollisionValue<std::int16_t>(a, 0x430))
        latch &= static_cast<std::uint8_t>(~bit);
    if ((latch & bit) || (a->hitStateFlags & 2U))
        return false;
    const VFCollisionShape *shapesA = BodyShapes(a), *shapesB = BodyShapes(b);
    const std::uint32_t categories = vfGetMoveCategoryMaskFor(a);
    std::uint32_t mask = 0;
    for (std::int32_t i = 0; i < 10; ++i) {
        if (!(shapesA[i].mask & categories))
            continue;
        for (std::int32_t j = 0; j < 10; ++j)
            if (shapesA[i].mask & ContactState(b).shapeMaskWords[j])
                mask |= shapesB[j].mask;
    }
    mask &= ~FightCollisionValue<std::uint32_t>(b, 0x798);
    FightCollisionValue<std::uint32_t>(b, 0x79c) = mask;
    if (!mask)
        return false;
    latch |= bit;
    const std::int8_t move = FightCollisionValue<std::int8_t>(a, 0x87f);
    if ((a->formationFlags & 2U) && FightCollisionValue<std::int16_t>(a, 0x934) == move)
        a->runtimeFlags |= 2U;
    FightCollisionValue<std::uint8_t>(b, 0x882) = static_cast<std::uint8_t>(move);
    return true;
}

#else

// Contact edges are latched per attacker/opponent, not per render.
// @verified - p
bool vfClGetBoneStates(std::uint8_t first, std::uint8_t second, VFCL_t *collision) {
    auto *const a = collision->game->actors[first];
    auto *const b = collision->game->actors[second];
    const auto bit = static_cast<std::uint8_t>(1u << second);
    auto latch = a->contact.processed;
    if ((!(b->activeMotionFlags & (VFSMF_Unknown4000 | VFSMF_Unknown200000)) &&
            ((b->activeMotionFlags & VFSMF_Unknown200) || (b->moveFlags & VFMF_NoContactPairing))) ||
        !(a->hitStateFlags & VFHSF_AttackActive) || (b->hitStateFlags & (VFHSF_MidAirRecovery | VFHSF_AlignedToGround)) ||
        ((b->hitStateFlags & VFHSF_AirborneOrDowned) && (!b->secondActor || (b->secondActor->hitStateFlags & 0x40000)))) {
        a->contact.processed = static_cast<std::uint8_t>(latch & ~bit);
        return false;
    }
    if (a->currentAnimation != a->previousAnimation) {
        latch &= ~bit;
        a->contact.processed = latch;
    }
    if ((latch & bit) || (a->hitStateFlags & VFHSF_HitTimeout))
        return false;
    const auto *const shapesA = reinterpret_cast<const VFCollisionShape *>(a->parameterAd8);
    const auto *const shapesB = reinterpret_cast<const VFCollisionShape *>(b->parameterAd8);
    const auto categories = vfGetMoveCategoryMaskFor(a);
    std::uint32_t mask = 0;
    for (std::size_t i = 0; i != 10; ++i) {
        if (!(shapesA[i].tag & categories))
            continue;
        for (std::size_t j = 0; j != 10; ++j)
            if (shapesA[i].tag & b->contact.shapeMasks[j])
                mask |= shapesB[j].tag;
    }
    mask &= ~b->contact.excludedMask;
    b->contact.pendingMask = mask;
    if (!mask)
        return false;
    a->contact.processed = static_cast<std::uint8_t>(latch | bit);
    const auto move = a->hitImpulseMove;
    if ((a->formationFlags & 2) && move == a->formationCameraTimerWords[0])
        a->formationRuntimeFlags |= 2;
    b->reactionImpulseBytes[2] = static_cast<std::uint8_t>(move);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfRob_ResolveHitClash(VFCL_t *collision, std::uint8_t first, std::uint8_t second) {
    FightActorRuntimePrefix *a = collision->game->actors[first], *b = collision->game->actors[second];
    vfClGetBonePos(a, b);
    vfClGetBonePos(b, a);
    const bool firstHit = vfClGetBoneStates(first, second, collision), secondHit = vfClGetBoneStates(second, first, collision);
    std::uint8_t result = 0;
    if (!firstHit) {
        if (secondHit)
            result = 1;
    } else if (!secondHit)
        result = 2;
    else if ((a->motionFlags ^ b->motionFlags) & 0x8000U)
        result = b->motionFlags & 0x8000U ? 1 : 2;
    else {
        const std::int8_t strengthA = FightCollisionValue<std::int8_t>(a, 0x2aa), strengthB = FightCollisionValue<std::int8_t>(b, 0x2aa);
        if (strengthA > strengthB)
            result = 2;
        else if (strengthB > strengthA)
            result = 1;
        else {
            const std::int16_t frameA = FightCollisionValue<std::int16_t>(a, 0x290), frameB = FightCollisionValue<std::int16_t>(b, 0x290);
            result = frameA > frameB ? 2 : frameB > frameA ? 1 : 3;
        }
    }
    if (result == 1)
        vfclApplyHitResolutionForSlots(collision, second, first);
    else if (result == 2)
        vfclApplyHitResolutionForSlots(collision, first, second);
    else if (result == 3) {
        vfclApplyHitResolutionForSlots(collision, first, second);
        vfclApplyHitResolutionForSlots(collision, second, first);
    }
}

#else

// Attack priority determines native hit publication order on a clash.
// @verified - p
void vfRob_ResolveHitClash(VFCL_t *collision, std::uint8_t first, std::uint8_t second) {
    auto *const a = collision->game->actors[first];
    auto *const b = collision->game->actors[second];
    vfClGetBonePos(0, a, b);
    vfClGetBonePos(1, a, b);
    const bool firstHit = vfClGetBoneStates(first, second, collision);
    const bool secondHit = vfClGetBoneStates(second, first, collision);
    std::uint8_t result = secondHit;
    if (firstHit) {
        result = 2;
        if (secondHit) {
            if ((a->motionFlags ^ b->motionFlags) & 0x8000) {
                if (b->motionFlags & VFMOF_Unknown8000)
                    result = 1;
            } else {
                const auto strengthA = static_cast<std::uint8_t>(a->hitDamage);
                const auto strengthB = static_cast<std::uint8_t>(b->hitDamage);
                if (strengthA >= strengthB) {
                    result = 1;
                    if (strengthA == strengthB) {
                        const auto frameA = static_cast<std::uint16_t>(a->parameter2d0);
                        const auto frameB = static_cast<std::uint16_t>(b->parameter2d0);
                        result = frameA < frameB ? 1 : frameA == frameB ? 3 : 2;
                    }
                }
            }
        }
    }
    if (result == 2 || result == 3)
        vfclApplyHitResolution(a, b);
    if (result == 1 || result == 3)
        vfclApplyHitResolution(b, a);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfRob_ResolveHitClash(VFCL_t *collision, std::uint8_t first, std::uint8_t second);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static Vector3 ScaleContactDisplacement(const Vector3 &value, float length) {
    const float scale = length / ::sqrtf((value.y * value.y + value.x * value.x) + value.z * value.z);
    Vector3 result = {value.x * scale, value.y * scale, value.z * scale};
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static Vector3 ReadyContactDisplacement(const Vector3 &separation, const Vector3 &history) {
    const float squared = separation.x * separation.x + separation.z * separation.z;
    if (!(squared > 0))
        return ScaleContactDisplacement(history, 0.8f);
    const float dot = history.x * separation.x + history.z * separation.z;
    const float penetration = 0.8f - ::sqrtf(squared);
    if (dot < 0) {
        const std::uint16_t turn = static_cast<std::uint16_t>(0x8000 - 2 * dc::sdk::ArcTan2Fixed(history.z * separation.x - history.x * separation.z, dot));
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::RotateAroundY_Wrapper(turn);
        Vector3 rotated;
        Matrix4x4::Transform(separation, rotated);
        const Vector3 scaled = ScaleContactDisplacement(rotated, penetration);
        Vector3 result = {rotated.x - separation.x + scaled.x, 0, rotated.z - separation.z + scaled.z};
        return result;
    }
    return ScaleContactDisplacement(separation, penetration);
}

#else

namespace {

template <bool ActiveOpponent> Vector3 ReadyContactDisplacement(Vector3 separation, Vector3 history) {
    const float squared = separation.z * separation.z + separation.x * separation.x;
    if (squared <= 0)
        return ScaleToMagnitude(history, .8f);
    float dot = history.z * separation.z + history.x * separation.x;
    if constexpr (ActiveOpponent)
        dot = float(std::sqrt(double(dot))); // As native, including its NaN result.
    const float penetration = .8f - std::sqrt(squared);
    if (dot < 0) {
        const auto turn = static_cast<std::uint16_t>(2 * (-0x4000 - FixedAtan2(history.z * separation.x - history.x * separation.z, dot)));
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::RotateAroundY_Wrapper(turn);
        Vector3 rotated;
        Matrix4x4::Transform(separation, rotated);
        const Vector3 correction{rotated.x - separation.x, 0, rotated.z - separation.z};
        const auto scaled = ScaleToMagnitude(rotated, penetration);
        return {correction.x + scaled.x, 0, correction.z + scaled.z};
    }
    return ScaleToMagnitude(separation, penetration);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void ResolveReadyActive(VFCLParticipantTmp *participants, std::int32_t readyIndex, std::int32_t activeIndex) {
    VFCLParticipantTmp &ready = participants[readyIndex], &active = participants[activeIndex];
    FCMG_t *group = static_cast<FCMG_t *>(ready.owner);
    FightActorRuntimePrefix *actor = static_cast<FightActorRuntimePrefix *>(active.owner);
    if (actor->hitStateFlags & 0xc0000U)
        return;
    std::uint32_t coefficient = 0;
    if (group->group && static_cast<std::uint8_t>(group->group->activationState - 3) <= 2)
        coefficient = 1;
    else if ((actor->hitStateFlags & 0x10000U) || FightCollisionValue<std::uint32_t>(actor, 0x844) || (actor->activeMotionFlags & 0xc00U))
        coefficient = 2;
    const Vector3 separation = {group->position.x - actor->contactPosition.x, 0, group->position.z - actor->contactPosition.z};
    if (!(separation.x * separation.x + separation.z * separation.z < 0.8f * 0.8f))
        return;
    const Vector3 history = {ready.x - active.x, 0, ready.z - active.z};
    const Vector3 displacement = ReadyContactDisplacement(separation, history);
    static const float weights[3][2] = {{0.5f, 0.5f}, {0, 1}, {1, 0}};
    MoveReadyEnemy(group, displacement.x * weights[coefficient][0], displacement.z * weights[coefficient][0]);
    vfclMoveEntityWithContactHistory(actor, -(displacement.x * weights[coefficient][1]), -(displacement.z * weights[coefficient][1]));
}

#else

namespace {

// Ready entities retain FCMG ownership; active actors publish VFCL history.
// @verified - p
void ResolveReadyActive(VFCLParticipantTmp &ready, VFCLParticipantTmp &active) {
    auto *const group = static_cast<FCMG_t *>(ready.owner);
    auto *const actor = static_cast<VFActor *>(active.owner);
    if (actor->hitStateFlags & (VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned))
        return;
    unsigned coefficient = 0;
    if (group->group && std::uint8_t(static_cast<std::uint8_t>(group->group->activationState) - 3) <= 2)
        coefficient = 1;
    else if ((actor->hitStateFlags & VFHSF_GuardState) || actor->contact.activeShapeMask || (actor->activeMotionFlags & (VFSMF_Unknown400 | VFSMF_Unknown800)))
        coefficient = 2;
    const Vector3 separation{group->position.x - actor->contactPosition.x, 0, group->position.z - actor->contactPosition.z};
    if (separation.z * separation.z + separation.x * separation.x >= .8f * .8f)
        return;
    const auto displacement = ReadyContactDisplacement<true>(separation, {ready.x - active.x, 0, ready.z - active.z});
    constexpr float weights[3][2]{{.5f, .5f}, {0, 1}, {1, 0}};
    MoveReadyEnemy(group, displacement.x * weights[coefficient][0], displacement.z * weights[coefficient][0]);
    vfclMoveEntityWithContactHistory(actor, -(displacement.x * weights[coefficient][1]), -(displacement.z * weights[coefficient][1]));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void ResolveReadyReady(VFCLParticipantTmp *participants, std::int32_t firstIndex, std::int32_t secondIndex) {
    VFCLParticipantTmp &first = participants[firstIndex], &second = participants[secondIndex];
    FCMG_t *a = static_cast<FCMG_t *>(first.owner), *b = static_cast<FCMG_t *>(second.owner);
    std::uint32_t coefficient = a->group && static_cast<std::uint8_t>(a->group->activationState - 3) <= 2 ? 1 : 0;
    if (b->group && static_cast<std::uint8_t>(b->group->activationState - 3) <= 2) {
        if (coefficient == 1)
            return;
        coefficient = 2;
    }
    const Vector3 separation = {a->position.x - b->position.x, 0, a->position.z - b->position.z};
    if (!(separation.x * separation.x + separation.z * separation.z < 0.8f * 0.8f))
        return;
    const Vector3 history = {first.x - second.x, 0, first.z - second.z};
    const Vector3 displacement = ReadyContactDisplacement(separation, history);
    static const float weights[3][2] = {{0.5f, 0.5f}, {0, 1}, {1, 0}};
    MoveReadyEnemy(a, displacement.x * weights[coefficient][0], displacement.z * weights[coefficient][0]);
    MoveReadyEnemy(b, -(displacement.x * weights[coefficient][1]), -(displacement.z * weights[coefficient][1]));
}

#else

namespace {

// The native null-second-formation branch reads absolute address 0x50.
// @verified - p
void ResolveReadyReady(VFCLParticipantTmp &first, VFCLParticipantTmp &second) {
    auto *const a = static_cast<FCMG_t *>(first.owner);
    auto *const b = static_cast<FCMG_t *>(second.owner);
    unsigned coefficient = a->group && std::uint8_t(static_cast<std::uint8_t>(a->group->activationState) - 3) <= 2 ? 1 : 0;
    if (!b->group && std::uint8_t(ReadFightData<std::uint8_t>(reinterpret_cast<const void *>(0x50), 0) - 3) <= 2) {
        if (coefficient == 1)
            return;
        coefficient = 2;
    }
    const Vector3 separation{a->position.x - b->position.x, 0, a->position.z - b->position.z};
    if (separation.z * separation.z + separation.x * separation.x >= .8f * .8f)
        return;
    const auto displacement = ReadyContactDisplacement<false>(separation, {first.x - second.x, 0, first.z - second.z});
    constexpr float weights[3][2]{{.5f, .5f}, {0, 1}, {1, 0}};
    MoveReadyEnemy(a, displacement.x * weights[coefficient][0], displacement.z * weights[coefficient][0]);
    MoveReadyEnemy(b, -(displacement.x * weights[coefficient][1]), -(displacement.z * weights[coefficient][1]));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static std::int32_t ParticipantPairInContactRange(VFCLParticipantTmp *participants, std::uint8_t first, std::uint8_t second) {
    const Vector3 *a, *b;
    if (participants[first].type == 1) {
        const FightActorRuntimePrefix *actor = reinterpret_cast<const FightActorRuntimePrefix *>(participants[first].owner);
        a = &actor->game->collision->samples[*reinterpret_cast<const std::int8_t *>(actor)].points[0];
    } else {
        a = &reinterpret_cast<const FCMG_t *>(participants[first].owner)->position;
    }
    if (participants[second].type == 1) {
        const FightActorRuntimePrefix *actor = reinterpret_cast<const FightActorRuntimePrefix *>(participants[second].owner);
        b = &actor->game->collision->samples[*reinterpret_cast<const std::int8_t *>(actor)].points[0];
    } else {
        b = &reinterpret_cast<const FCMG_t *>(participants[second].owner)->position;
    }
    Vector3 delta;
    delta.x = b->x - a->x;
    delta.y = b->y - a->y;
    delta.z = b->z - a->z;
    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z < 4.409999370574951f)
        return 1;
    return 0;
}

#else

namespace {

const Vector3 &ParticipantPosition(const VFCLParticipantTmp &participant) {
    if (participant.type != 1)
        return static_cast<const FCMG_t *>(participant.owner)->position;
    const auto *const actor = static_cast<const VFActor *>(participant.owner);
    return actor->game->collision->samples[actor->groupIndex].points[0];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static bool PairBlockedByWorld(VFCL_t *collision, const FightActorRuntimePrefix *a, const FightActorRuntimePrefix *b) {
    Vector3 hit;
    const Vector3 *first = &collision->samples[*reinterpret_cast<const std::int8_t *>(a)].points[0];
    const Vector3 *second = &collision->samples[*reinterpret_cast<const std::int8_t *>(b)].points[0];
    const float dx = second->x - first->x, dz = second->z - first->z;
    if ((dx < 0 ? -dx : dx) < 0.00000011920929f && (dz < 0 ? -dz : dz) < 0.00000011920929f)
        return false;
    if (FindNearestRayIntersection(&hit, first, second))
        return true;
    return false;
}

#else

namespace {

// @verified - p
bool PairBlockedByWorld(const VFActor *a, const VFActor *b) {
    const auto &first = a->game->collision->samples[a->groupIndex].points[0];
    const auto &second = b->game->collision->samples[b->groupIndex].points[0];
    if (std::fabs(first.x - second.x) < .00000011920929f && std::fabs(first.z - second.z) < .00000011920929f)
        return false;
    Vector3 hit;
    return FindNearestRayIntersection(&hit, &first, &second);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfBuildContactsAndResolvePushOrHit(VFCL_t *collision, VFCLParticipantTmp *participants, std::int32_t count, std::int32_t phase) {
    FightRuntimePrefix *game = collision->game;
    std::int32_t i, j;
    for (i = count - 2; i >= 0; --i) {
        for (j = i + 1; j < count; ++j) {
            if (phase) {
                if (participants[i].link || participants[j].link)
                    continue;
            } else if (!participants[i].link && !participants[j].link)
                continue;
            if (ParticipantPairInContactRange(participants, i, j) != 1)
                continue;
            if (participants[i].type != 1) {
                if (participants[j].type != 1)
                    ResolveReadyReady(participants, i, j);
                else
                    do {
                        ResolveReadyActive(participants, i, j);
                    } while (0);
            } else if (participants[j].type != 1)
                ResolveReadyActive(participants, j, i);
            else {
                std::int32_t first = participants[i].index, second = participants[j].index;
                vfclPrepareContact(collision, first, second);
                FightActorRuntimePrefix *self = game->actors[first], *other = game->actors[second];
                std::int32_t hit;
                if (game->slotTeams[first] != game->slotTeams[second] || static_cast<std::int32_t>(self->activeMotionFlags | other->activeMotionFlags) < 0)
                    hit = 1;
                else
                    hit = 0;
                if (hit == 1) {
                    FightCollisionValue<std::uint8_t>(other, 0x884) = 2;
                    FightCollisionValue<std::uint8_t>(self, 0x884) = 2;
                } else {
                    FightCollisionValue<std::uint8_t>(other, 0x884) = 0;
                    FightCollisionValue<std::uint8_t>(self, 0x884) = 0;
                }
                if (FightCollisionValue<std::int16_t>(self, 2) == 0)
                    FightCollisionValue<std::uint8_t>(other, 0x884) |= 1;
                if (FightCollisionValue<std::int16_t>(other, 2) == 0)
                    FightCollisionValue<std::uint8_t>(self, 0x884) |= 1;
                if ((self->activeMotionFlags | other->activeMotionFlags) & 0x800U)
                    hit = 0;
                if (hit && PairBlockedByWorld(collision, self, other))
                    hit = 0;
                if (hit == 1)
                    vfRob_ResolveHitClash(collision, first, second);
            }
        }
    }
}

#else

// Backwards sweep visits linked contacts first, then unlinked contacts.
// @verified - p
void vfBuildContactsAndResolvePushOrHit(VFCL_t *collision, VFCLParticipantTmp *participants, std::int32_t count, std::uint8_t phase) {
    for (std::int32_t i = count - 2; i >= 0; --i) {
        for (std::int32_t j = i + 1; j < count; ++j) {
            auto &first = participants[i];
            auto &second = participants[j];
            if (phase ? first.link || second.link : !first.link && !second.link)
                continue;
            const auto &a = ParticipantPosition(first);
            const auto &b = ParticipantPosition(second);
            const float x = b.x - a.x, y = b.y - a.y, z = b.z - a.z;
            if (!(double((y * y + x * x) + z * z) < 4.41))
                continue;
            if (first.type != 1 && second.type != 1)
                ResolveReadyReady(first, second);
            else if (first.type != 1)
                ResolveReadyActive(first, second);
            else if (second.type != 1)
                ResolveReadyActive(second, first);
            else {
                vfclPrepareContact(collision, first.index, second.index);
                auto &game = *collision->game;
                auto *const self = game.actors[first.index];
                auto *const other = game.actors[second.index];
                const bool opponents = game.slotTeams[first.index] != game.slotTeams[second.index] ||
                    static_cast<std::int32_t>(self->activeMotionFlags | other->activeMotionFlags) < 0;
                self->knockbackAngleBytes[0] = static_cast<std::uint8_t>((opponents ? 2 : 0) | (other->fightStyle == 0));
                other->knockbackAngleBytes[0] = static_cast<std::uint8_t>((opponents ? 2 : 0) | (self->fightStyle == 0));
                if (opponents && !((self->activeMotionFlags | other->activeMotionFlags) & 0x800) && !PairBlockedByWorld(self, other) &&
                    (!(self->hitStateFlags & VFHSF_AirborneOrDowned) || self->secondActor != other) &&
                    (!(other->hitStateFlags & VFHSF_AirborneOrDowned) || other->secondActor != self))
                    vfRob_ResolveHitClash(collision, first.index, second.index);
            }
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfc_VF_RobCollision_Callback(void *parameter) {
    VFCL_t *const collision = static_cast<VFCL_t *>(parameter);
    matrix_stack::Matrix4x4_Push(0);
    FightRuntimePrefix *const game = collision->game;
    std::uint8_t index;
    FightActorRuntimePrefix *actor;
    for (index = 0; index < 6; ++index) {
        collision->contactDeltas[index].x = 0, collision->contactDeltas[index].y = 0;
        if (game->characters[index]) {
            actor = game->actors[index];
            if (actor->moveFlags & 0x10000000U) {
                actor->moveFlags &= ~0x10000000U;
                vfclTransformContactShapes(actor);
                std::memcpy(reinterpret_cast<std::byte *>(actor) + 0x794 + offsetof(ActorContactState, previousSamples),
                    reinterpret_cast<std::byte *>(actor) + 0x74, 10 * sizeof(Vector3));
                collision->previousSamples[index] = collision->samples[index];
            } else {
                std::memcpy(reinterpret_cast<std::byte *>(actor) + 0x794 + offsetof(ActorContactState, previousSamples),
                    reinterpret_cast<std::byte *>(actor) + 0x74, 10 * sizeof(Vector3));
                collision->previousSamples[index] = collision->samples[index];
                vfclTransformContactShapes(actor);
            }
            for (std::uint8_t word = 0; word < 10; ++word)
                reinterpret_cast<ActorContactState *>(reinterpret_cast<std::byte *>(actor) + 0x794)->shapeMaskWords[word] = 0;
            actor->stateFlags = (actor->stateFlags & ~0x20U) | 0x40U;
        }
    }
    VFCLParticipantTmp participants[16];
    std::int32_t count = 0;
    for (index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t *const enemy = &g_pFightStage->enemySlots[index];
        if (!enemy->character)
            continue;
        if (enemy->state == 1) {
            participants[count].type = 2;
            participants[count].index = index;
            participants[count].link = 0;
            participants[count].owner = enemy;
            participants[count].x = enemy->position.x;
            participants[count].z = enemy->position.z;
            ++count;
        } else if (enemy->state == 2) {
            actor = enemy->fighter;
            participants[count].type = 1;
            participants[count].index = *reinterpret_cast<std::uint8_t *>(actor);
            if (reinterpret_cast<ActorContactState *>(reinterpret_cast<std::byte *>(actor) + 0x794)->activeShapeMask)
                participants[count].link = 1;
            else
                participants[count].link = 0;
            participants[count].owner = actor;
            participants[count].x = actor->contactPosition.x;
            participants[count].z = actor->contactPosition.z;
            ++count;
        }
    }
    if (g_pFightStage->battleFlags & 0x10U) {
        g_pFightStage->battleFlags &= ~0x10U;
        PopCurrentMatrix(1);
        return;
    }
    vfBuildContactsAndResolvePushOrHit(collision, participants, count, 0);
    vfBuildContactsAndResolvePushOrHit(collision, participants, count, 1);
    vfclEnforceRingBoundary(collision);
    if (!(g_pFightStage->battleFlags & 0x800U))
        vfclResolveWorldContacts(collision);
    for (index = 0; index < 6; ++index) {
        collision->contactMasks[index] = 0;
        if (game->characters[index]) {
            actor = game->actors[index];
            *reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::byte *>(actor) + 0x794 + offsetof(ActorContactState, linked)) = 0;
            *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x794 + offsetof(ActorContactState, activeShapeMask)) = 0;
            actor->moveFlags &= ~0x8000U;
        }
    }
    vfclAdjustToGroundOrSlope(collision);
    for (index = 0; index < 6; ++index)
        if (game->characters[index])
            vfclUpdateHeightFlagsForGrid(game->actors[index]);
    vfUpdateStageObjects(game);
    vfclResolvePairedContactDrift(game);
    vfclAdjustToGroundOrSlope(collision);
    vfClRaycastInWorldGrid();
    vfClCopyMotionData(game);
    PopCurrentMatrix(1);
}

#else

// Preserve current/previous pose ordering before both contact sweeps.
// @verified - p
void vfc_VF_RobCollision_Callback(void *parameter) {
    auto *const collision = static_cast<VFCL_t *>(parameter);
    auto *const game = collision->game;
    Matrix4x4_Push();
    for (std::size_t i = 0; i != 6; ++i) {
        collision->contactDeltas[i] = {};
        if (!game->characters[i])
            continue;
        auto *const actor = game->actors[i];
        const bool resetHistory = (actor->moveFlags & VFMF_RefreshContactHistory) != 0;
        if (resetHistory) {
            actor->moveFlags &= ~VFMF_RefreshContactHistory;
            vfclTransformContactShapes(actor);
        }
        std::memcpy(actor->contact.previousSamples, actor->motionContactSamples, sizeof(actor->motionContactSamples));
        collision->previousSamples[i] = collision->samples[i];
        if (!resetHistory)
            vfclTransformContactShapes(actor);
        for (std::uint64_t &pair : actor->contact.shapeMaskPairs)
            pair = 0;
        actor->stateFlags = (actor->stateFlags & ~VFSF_Unknown20) | 0x40;
    }
    VFCLParticipantTmp participants[16];
    std::int32_t count = 0;
    auto &stage = *g_pFightStage;
    for (std::uint8_t i = 0; i < stage.enemyCount; ++i) {
        auto &enemy = stage.enemySlots[i];
        if (!enemy.character)
            continue;
        if (enemy.state == 1) {
            auto &participant = participants[count++];
            participant.type = 2;
            participant.index = i;
            participant.link = 0;
            participant.owner = &enemy;
            participant.x = enemy.position.x;
            participant.z = enemy.position.z;
        } else if (enemy.state == 2 || (enemy.state == 3 && game->type == 8)) {
            auto *const actor = enemy.fighter;
            auto &participant = participants[count++];
            participant.type = 1;
            participant.index = actor->groupIndex;
            participant.link = actor->contact.activeShapeMask != 0;
            participant.owner = actor;
            participant.x = actor->contactPosition.x;
            participant.z = actor->contactPosition.z;
        }
    }
    if (stage.battleFlags & 0x10)
        stage.battleFlags &= ~0x10u;
    else {
        vfBuildContactsAndResolvePushOrHit(collision, participants, count, 0);
        vfBuildContactsAndResolvePushOrHit(collision, participants, count, 1);
        vfclEnforceRingBoundary(collision);
        for (std::size_t i = 0; i != 6; ++i) {
            collision->contactMasks[i] = 0;
            if (!game->characters[i])
                continue;
            auto *const actor = game->actors[i];
            actor->moveFlags &= ~VFMF_AirborneOrFalling;
            actor->contact.linked = std::uint8_t{0};
            actor->contact.activeShapeMask = std::uint32_t{0};
        }
        vfclAdjustToGroundOrSlope(collision);
        for (std::size_t i = 0; i != 6; ++i)
            if (game->characters[i])
                vfclUpdateHeightFlagsForGrid(game->actors[i]);
        vfUpdateStageObjects(game);
        vfclResolvePairedContactDrift(game);
        vfclAdjustToGroundOrSlope(collision);
        vfClRaycastInWorldGrid();
        vfClCopyMotionData(game);
    }
    Matrix4x4::Pop(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
HLib::Task *Queue_vfc_VF_RobCollision(FightRuntimePrefix *game) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(vfc_VF_RobCollision_Callback, 12, 2, sizeof(VFCL_t), SHEN_NAME4('V', 'F', 'C', 'L'));
    VFCL_t *const collision = static_cast<VFCL_t *>(HLib::GetTaskParameter(task));
    collision->name = SHEN_NAME4('F', 'C', 'O', 'L');
    collision->game = game;
    game->collision = collision;
    return task;
}

#else

// Task-owned collision storage is published to the genuine VFMA owner.
// @verified - p
HLib::Task *Queue_vfc_VF_RobCollision(VFMA_t *game) {
    auto *const task = HLib::EnqueueTaskWithParameter(vfc_VF_RobCollision_Callback, 12, 2, 0x270, SHEN_NAME(VFCL));
    auto *const collision = static_cast<VFCL_t *>(HLib::GetTaskParameter(task));
    collision->name = SHEN_NAME(FCOL);
    collision->game = game;
    game->collision = collision;
    return task;
}

#endif

} // namespace s2
} // namespace shenmue
