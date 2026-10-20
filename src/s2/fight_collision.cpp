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

namespace shenmue::s2 {
std::int32_t g_vfSelectedReactionAnimation{};
namespace {
template <class T> T ReadFightData(const void *owner, std::size_t offset) {
    T result;
    std::memcpy(&result, static_cast<const std::byte *>(owner) + offset, sizeof(result));
    return result;
}

constexpr VFCollisionShape bodyShapes[10]{
    {{-.055f,.015f,.005f},.295f,3,0,2}, {{.025f,0,.05f},.156f,5,0,4},
    {{.135f,.02f,-.01f},.18f,9,0,8}, {{.23f,-.005f,-.04f},.173f,12,0,16},
    {{.135f,.02f,.01f},.18f,16,0,64}, {{.23f,-.005f,.04f},.173f,19,0,128},
    {{.225f,0,0},.213f,23,0,1024}, {{.305f,.035f,-.005f},.232f,24,0,2048},
    {{.225f,0,0},.213f,28,0,8192}, {{.305f,.035f,.005f},.232f,29,0,16384}};
constexpr VFCollisionShape togyuBodyShapes[10]{
    {{-.155f,.005f,.025f},.489f,3,0,2}, {{.015f,0,.05f},.211f,5,0,4},
    {{.15f,-.03f,-.055f},.267f,9,0,8}, {{.33f,-.01f,.01f},.224f,12,0,16},
    {{.15f,-.03f,.055f},.267f,16,0,64}, {{.33f,-.01f,-.01f},.224f,19,0,128},
    {{.31f,.025f,-.005f},.325f,23,0,1024}, {{.305f,.035f,-.005f},.311f,24,0,2048},
    {{.31f,.025f,.005f},.325f,28,0,8192}, {{.305f,.035f,.005f},.311f,29,0,16384}};
constexpr VFCollisionShape pushShapes[3]{
    {{.04f,-.02f,0},.4f,3,0,2}, {{.3f,0,0},.25f,24,0,2048}, {{.3f,0,0},.25f,29,0,16384}};
constexpr VFCollisionShape togyuPushShapes[3]{
    {{-.05f,-.145f,0},.745f,3,0,2}, {{.34f,0,0},.392f,24,0,2048}, {{.34f,0,0},.392f,29,0,16384}};
struct HitProfile {
    float attackScale[2], impulseScale[4];
    std::int16_t angles[4];
};
static_assert(sizeof(HitProfile) == 32);
constexpr HitProfile hitProfiles[]{
    {{1,1},{1,1.2f,1,1.2f},{11776,6144,9728,4864}},
    {{1,1},{1.5f,1.8f,1.2f,1.44f},{13824,9216,12800,6400}},
    {{1,1},{2,5,1.2f,1.44f},{12288,14336,11264,6144}},
    {{1,1},{2.5f,2.5f,2.5f,2.5f},{14592,14592,14592,14592}},
    {{1,1},{1.3f,1.2f,1.2f,1.2f},{11776,4096,9728,4864}},
    {{1,1},{3.8f,3.8f,3.8f,3.8f},{9216,9216,9216,9216}},
    {{1,1},{2.5f,2.5f,2.5f,2.5f},{12800,12800,12800,12800}}};
const VFCollisionShape *PushShapes(const VFActor *actor) {
    return reinterpret_cast<const VFCollisionShape *>(actor->parameterAe0);
}
Vector3 ScaleToMagnitude(const Vector3 &input, float magnitude) {
    const float scale = magnitude / std::sqrt(input.x * input.x + input.y * input.y + input.z * input.z);
    return {input.x * scale, input.y * scale, input.z * scale};
}
// A ready enemy retains its own FCMG root/position owner.
// @verified - p
void MoveReadyEnemy(FCMG_t *group, float x, float z) {
    if (std::fabs(x) < 0.0000099999997f)
        x = 0;
    if (std::fabs(z) < 0.0000099999997f)
        z = 0;
    group->position.x += x;
    group->position.z += z;
    auto root = GetMotionRootPosition(group->motion);
    root.x += x;
    root.z += z;
    SetMotionRootPosition(group->motion, root);
    shenApplyTranslationToAll(&group->character->entity.node, group->position);
}
std::int32_t StyleAnimation(const VFActor &actor, std::size_t offset) {
    const auto *data = actor.motionParameters[4] + offset;
    const auto high = static_cast<std::int16_t>(std::uint16_t(std::to_integer<std::uint8_t>(data[0])) << 8);
    return int(high) | std::to_integer<std::uint8_t>(data[1]);
}
}

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
            const auto *figure = static_cast<FigurePose *>(FindModelComponent(other->character, MODEL_FIGURE_POSE));
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


// @verified - p
void vfclTransformContactShapes(VFActor *self) {
    const auto *const shapes = reinterpret_cast<const VFCollisionShape *>(self->parameterAd8);
    for (std::size_t i = 0; i != 10; ++i) {
        const auto *const bone = charaGetBoneTransform(self->motion, shapes[i].bone);
        if (!bone)
            continue;
        Matrix4x4::SetActiveTransform(bone);
        Matrix4x4::TransformPoint(shapes[i].point, self->motionContactSamples[i]);
    }
    const auto *const pushes = PushShapes(self);
    for (std::size_t i = 0; i != 3; ++i) {
        Matrix4x4::SetActiveTransform(charaGetBoneTransform(self->motion, pushes[i].bone));
        Matrix4x4::TransformPoint(pushes[i].point, self->game->collision->samples[self->groupIndex].points[i]);
    }
}

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

// Calls vfclApplyDeltaToContactHistory: movement preserves root, character and contact-history ownership.
// @verified - p
void vfclMoveEntityWithContactHistory(VFActor *self, float x, float z) {
    if (std::fabs(x) < 0.00000011920929f && std::fabs(z) < 0.00000011920929f)
        return;
    Vector3 root = GetMotionRootPosition(self->motion);
    root.x += x;
    root.z += z;
    SetMotionRootPosition(self->motion, root);
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

// Publish accumulated collision displacement into the solved MTWK rig.
// @verified - p
void vfClCopyMotionData(VFMA_t *game) {
    for (std::size_t i = 0; i != 6; ++i)
        if (game->characters[i]) {
            const auto &delta = game->collision->contactDeltas[i];
            if (std::fabs(delta.x) >= 0.00000011920929f || std::fabs(delta.y) >= 0.00000011920929f)
                ApplyMotionCollisionTranslation(game->actors[i]->motion, delta.x, 0, delta.y);
        }
}

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
            Matrix4x4::SetActiveTransform(charaGetBoneTransform(group.motion, shape.bone));
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


// @verified - p
void vfclSetContactCategory(VFActor *a, VFActor *b, VFCL_t *collision) {
    struct MaskEntry {
        std::uint32_t first, second;
        float slide, stick;
    };
    static constexpr MaskEntry entries[]{
        {0xffffffff,0xffffffff,.5f,.5f}, {0xffffffff,0xffffffff,0,1},
        {0xffffffff,0xffffffff,1,0}, {0x202,0x202,0,1}, {0x202,0x202,1,0},
        {0xffffffff,0xffffffff,0,1}, {0xffffffff,0xffffffff,1,0}};
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
            (void)GetMotionBonePosition(self->motion, static_cast<MotionBoneId>(shapes[i].bone), &position);
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

// The attached stage object retains its own velocity and reset state.
void vfStageObjectApplyImpulse(VFActor *self) {
    const auto index = self->parameterAd7;
    vfStageObjectReset(index, nullptr);
    g_fightStageObjects[index]->object.velocity = {
        sin_fixed16(static_cast<std::int16_t>(self->hitDirection)) * (-.2f), .1f, cos_fixed16(self->hitDirection) * (-.2f)};
}

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
    float impact = float(static_cast<std::uint8_t>(self->guardStrength));
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
        self->guardStrength = std::to_integer<std::int8_t>(entry[1]);
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
                const auto strengthA = static_cast<std::uint8_t>(a->guardStrength);
                const auto strengthB = static_cast<std::uint8_t>(b->guardStrength);
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

namespace {
template <bool ActiveOpponent> Vector3 ReadyContactDisplacement(Vector3 separation, Vector3 history) {
    const float squared = separation.z * separation.z + separation.x * separation.x;
    if (squared <= 0)
        return ScaleToMagnitude(history, .8f);
    float dot = history.z * separation.z + history.x * separation.x;
    if constexpr (ActiveOpponent)
        dot = float(std::sqrt(double(dot)));
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
const Vector3 &ParticipantPosition(const VFCLParticipantTmp &participant) {
    if (participant.type != 1)
        return static_cast<const FCMG_t *>(participant.owner)->position;
    const auto *const actor = static_cast<const VFActor *>(participant.owner);
    return actor->game->collision->samples[actor->groupIndex].points[0];
}
// @verified - p
bool PairBlockedByWorld(const VFActor *a, const VFActor *b) {
    const auto &first = a->game->collision->samples[a->groupIndex].points[0];
    const auto &second = b->game->collision->samples[b->groupIndex].points[0];
    if (std::fabs(first.x - second.x) < .00000011920929f && std::fabs(first.z - second.z) < .00000011920929f)
        return false;
    Vector3 hit;
    return FindNearestRayIntersection(&hit, &first, &second);
}
}

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

} // namespace shenmue::s2
