#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"

namespace shenmue::s2 {
using namespace model_component_detail;

namespace model_component_detail {

// The root's ground height follows its lower foot target. Stepping up while walking blends toward the raised foot's
// ground; PLYM's ground blend then lifts the probe height.
void ccmUpdateRootGroundHeight(CharacterControlModel &control) {
    if ((control.flags & CharacterControlRootOwner) == 0)
        return;
    MotionWork *const motion = FindMotionWork(control.owner);
    if (!motion)
        return;
    Vector3 right{}, left{}, sampled{};
    GetMotionControllerTargetPosition(motion, 6, right);
    GetMotionControllerTargetPosition(motion, 5, left);
    SampleNativeMotionRoot(GetMotionAnimationId(motion, MotionRegion::LowerBody), GetMotionRigBoneId(motion), -1.0f, sampled);
    if (plymIsSearchStartRequested()) {
        right = {right.x + control.frameDisplacement.x, right.y + control.frameDisplacement.y, right.z + control.frameDisplacement.z};
        left = {left.x + control.frameDisplacement.x, left.y + control.frameDisplacement.y, left.z + control.frameDisplacement.z};
    }
    const float rightY = right.y;
    const float leftY = left.y;
    right.y += 1.5f;
    left.y += 1.5f;
    float rightGround = shenFindIntersectionWithScene(right, left.y);
    float leftGround = shenFindIntersectionWithScene(left, rightGround);
    right.y = rightGround != kNoSceneIntersection ? rightGround : rightY;
    left.y = leftGround != kNoSceneIntersection ? leftGround : leftY;
    if (rightGround == kNoSceneIntersection)
        rightGround = control.groundHeight;
    if (leftGround == kNoSceneIntersection)
        leftGround = control.groundHeight;
    float lower = std::fmin(rightGround, leftGround);
    if ((control.flags & CharacterControlHeightStep) == 0)
        lower = std::fmin(lower, control.groundHeight);
    const float rise = std::fmax(rightGround, leftGround) - lower;
    float vertical{}, verticalSecond{}, blendX{}, blendZ{};
    Vector3 firstDelta{}, secondDelta{};
    plymGetControllerVerticalAxis(&vertical, &verticalSecond);
    plymGetControllerGroundBlendedXZ(&blendX, &blendZ);
    plymGetControllerDeltaXZ(&firstDelta, &secondDelta);
    control.adjustedGroundHeight = lower;
    float lift = 0.0f;
    const bool rightRaised = GetIsRHandAboveShoulder(motion);
    const Vector3 &raised = rightRaised ? left : right;
    const Vector3 &other = rightRaised ? right : left;
    const Vector3 &delta = rightRaised ? firstDelta : secondDelta;
    Vector3 raisedLocal{}, otherLocal{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-playerGetRotY()));
    const Vector3 *const search = plymUpdatePlayerSearchLocation(nullptr);
    Matrix4x4::preTranslate(-search->x, -search->y, -search->z);
    Matrix4x4::TransformPoint(raised, raisedLocal);
    Matrix4x4::TransformPoint(other, otherLocal);
    Matrix4x4::Pop(1);
    if (raised.y - other.y > 0.1f) {
        if (delta.z < 0.015f) {
            float weight = std::fmax(0.0f, std::fmax(std::fabs(otherLocal.z), 0.0f) / 0.30000001f);
            if (weight >= 1.0f)
                weight = 1.0f;
            if (plymGetSpeedZ() > 0.0f)
                lift = (1.0f - weight) * rise * 0.75f;
        }
        if (plymGetSpeedZ() > 0.0f) {
            if (plymGetInterpolationValue() >= 0.69999999f)
                lift = rise;
            float height = lift + lower;
            if (control.previousAdjustedGroundHeight > height)
                height = (height - control.previousAdjustedGroundHeight) * 0.050000001f + control.previousAdjustedGroundHeight;
            control.adjustedGroundHeight = height;
        }
    }
    if (plymIsLookAllowed(1) && blendX > 0.16874999f && blendZ > 0.16874999f)
        control.groundHeight = std::fmin(blendX, blendZ) + control.adjustedGroundHeight;
}




void UpdateCharacterControlModel(Character *character, void *payload) {
    auto *const control = static_cast<CharacterControlModel *>(payload);
    if (!flddIsValid())
        control->flags &= ~(CharacterControlUpdateHeight | CharacterControlApplyGravity | CharacterControlEvaluateMotion);
    control->flags &= ~(CharacterControlNoGround | CharacterControlHeightStep);
    control->appliedMovement = {};
    control->residualMovement = {};
    if ((control->flags & CharacterControlResetMotion) != 0) {
        control->field04 = 0;
        control->verticalVelocity = 0.0f;
        control->field0c = 0;
    }
    if ((control->flags & CharacterControlUpdateHeight) == 0)
        return;

    const Vector3 root = GetCharacterWorldPosition(character);
    const Vector3 pending = character->position;
    control->previousGroundHeight = control->groundHeight;
    Vector3 point{pending.x + root.x, (pending.y + root.y + 0.40000001f) + 1.0f, pending.z + root.z};
    control->groundHeight = shenFindIntersectionWithScene(point, point.y);
    point.y -= 1.0f;
    if (control->groundHeight == kNoSceneIntersection) {
        control->flags |= CharacterControlNoGround;
        control->groundHeight = root.y;
        control->groundPitch = 0;
        control->groundYaw = 0;
    } else {
        Vector3s angles{control->groundPitch, control->groundYaw, 0};
        transformIntersectionResult(angles);
        control->groundPitch = angles.x;
        control->groundYaw = angles.y;
        if (control->groundHeight - control->previousGroundHeight >= 0.40000001f) {
            control->flags |= CharacterControlHeightStep;
            control->groundHeight = root.y;
        }
    }
    control->previousAdjustedGroundHeight = control->adjustedGroundHeight;
    control->adjustedGroundHeight = control->groundHeight;
    ccmUpdateRootGroundHeight(*control);
    const float groundCenter = control->adjustedGroundHeight + 0.40000001f;
    if ((control->flags & CharacterControlApplyGravity) != 0) {
        control->verticalVelocity = -0.010896279f * HLib::GetScaledDeltaTick() + control->verticalVelocity;
        point.y += control->verticalVelocity;
    }

    Vector3 offset = pending;
    if ((control->flags & CharacterControlEvaluateMotion) != 0) {
        Vector3 contact{};
        if ((control->flags & CharacterControlSnapToGround) != 0 && groundCenter + 0.03f > point.y) {
            const float previous = point.y - control->verticalVelocity;
            contact.y = groundCenter;
            point.y = (groundCenter - previous) * 0.5f + previous;
            control->verticalVelocity = 0.0f;
        }
        if ((control->flags & CharacterControlFineRaycast) != 0) {
            if ((control->flags & CharacterControlFieldDataDebug) != 0)
                flddToggleFlag(32);
            if (RaycastInWorldGrid(contact, point, control->collisionRadius)) {
                point.x = (contact.x - point.x) * 0.80000001f + point.x;
                point.z = (contact.z - point.z) * 0.80000001f + point.z;
            }
        }
        offset = {point.x - root.x, (point.y - root.y) - 0.40000001f, point.z - root.z};
        control->residualMovement = {(contact.x - root.x) - pending.x, ((contact.y - root.y) - 0.40000001f) - pending.y, (contact.z - root.z) - pending.z};
        if ((control->flags & CharacterControlAlternateProbe) != 0) {
            const Vector3 probe{offset.x + root.x, (offset.y + root.y) + 1.5f, offset.z + root.z};
            if (shenFindIntersectionWithScene(probe, probe.y) == kNoSceneIntersection) {
                if ((control->flags & CharacterControlApplyGravity) != 0)
                    control->verticalVelocity = 0.0f;
                offset.y = 0.0f;
                if ((control->flags & CharacterControlFineCollision) != 0)
                    offset.x = offset.z = 0.0f;
            }
        }
        if ((control->flags & (CharacterControlFineRaycast | CharacterControlFineCollision)) == (CharacterControlFineRaycast | CharacterControlFineCollision)) {
            const Vector3 rayEnd{control->previousWorldPosition.x, control->previousWorldPosition.y + 0.40000001f, control->previousWorldPosition.z};
            Vector3 rayStart{offset.x + root.x, (offset.y + root.y) + 0.40000001f, offset.z + root.z};
            if (SquaredDistance(rayEnd, rayStart) < 0.090000004f) {
                flddToggleFlag(0x0468);
                Vector3 hit{};
                if (RaycastForCollision(&hit, &rayStart, &rayEnd)) {
                    const Vector3 away{hit.x - rayEnd.x, hit.y - rayEnd.y, hit.z - rayEnd.z};
                    if (std::fabs(away.x) >= kCharacterControlEpsilon || std::fabs(away.y) >= kCharacterControlEpsilon ||
                        std::fabs(away.z) >= kCharacterControlEpsilon) {
                        const float scale = -0.0099999998f / std::sqrt(away.x * away.x + away.y * away.y + away.z * away.z);
                        hit = {hit.x + away.x * scale, hit.y + away.y * scale, hit.z + away.z * scale};
                        offset = {hit.x - root.x, (hit.y - root.y) - 0.40000001f, hit.z - root.z};
                    }
                }
            }
        }
    }

    const float vertical = offset.y - pending.y;
    float moveX = offset.x - pending.x;
    float moveZ = offset.z - pending.z;
    if (vertical <= 0.0f && GetCharacterScenarioName(character).name != SHEN_NAME(BCYL))
        control->verticalVelocity = vertical;
    control->collisionAdjustment.y = vertical;
    character->position = {offset.x, pending.y, offset.z};
    MotionWork *const motion = FindMotionWork(character);
    if (motion && (std::fabs(moveX) >= kCharacterControlEpsilon || std::fabs(moveZ) >= kCharacterControlEpsilon))
        ApplyMotionCollisionTranslation(motion, moveX, 0.0f, moveZ);
    if (std::fabs(moveX) < kCharacterControlEpsilon)
        moveX = 0.0f;
    if (std::fabs(moveZ) < kCharacterControlEpsilon)
        moveZ = 0.0f;
    control->appliedMovement = {moveX, 0.0f, moveZ};
    control->residualMovement.x -= moveX;
    control->residualMovement.z -= moveZ;
}

ModelComponentHeader *HeaderForPayload(void *payload) {
    return reinterpret_cast<ModelComponentHeader *>(static_cast<std::byte *>(payload) - sizeof(ModelComponentHeader));
}

CharacterControlModel *LiveCharacterControl(Character *character) {
    CharacterControlModel *const control = FindCharacterControlModel(character);
    return control && control->owner == character ? control : nullptr;
}

void ClearCharacterControlPublication(CharacterControlModel &control) {
    control.collisionCenter = {};
    control.collisionCenterRadius = 0.0f;
}

void CleanupCharacterControlModel(Character *, void *payload) {
    auto *const control = static_cast<CharacterControlModel *>(payload);
    if (!control)
        return;
    control->flags = 0;
    control->collisionAdjustment = {};
    control->pairAdjustment = {};
    ClearCharacterControlPublication(*control);
    control->owner = nullptr;
}




void CharCollisionSort(Character *character, void *) {
    CharacterControlModel *const control = LiveCharacterControl(character);
    if (!control)
        return;

    control->flags &= ~kCharacterControlTransientFlags;
    control->pairAdjustment = {};
    const Vector3 translation = GetCharacterWorldPosition(character);
    control->frameDisplacement = {
        translation.x - control->previousWorldPosition.x, translation.y - control->previousWorldPosition.y, translation.z - control->previousWorldPosition.z};
    control->previousWorldPosition = {translation.x + character->position.x, translation.y + character->position.y, translation.z + character->position.z};
    if ((control->flags & CharacterControlResetMotion) != 0) {
        control->field04 = 0;
        control->verticalVelocity = 0.0f;
        control->field0c = 0;
        control->frameDisplacement = {};
        control->collisionAdjustment = {};
    }
    control->flags &= ~CharacterControlResetMotion;
    if (std::fabs(control->residualMovement.x) >= kCharacterControlEpsilon || std::fabs(control->residualMovement.z) >= kCharacterControlEpsilon)
        control->flags |= CharacterControlMoving;
    control->flags &= ~CharacterControlConversationMember;
    control->displacementSquared = control->frameDisplacement.x * control->frameDisplacement.x + control->frameDisplacement.y * control->frameDisplacement.y +
        control->frameDisplacement.z * control->frameDisplacement.z;
    if (IsConversationCameraCharacter(character))
        control->flags |= CharacterControlConversationMember;

    control->collisionCenter.x = translation.x;
    control->collisionCenter.z = translation.z;
    float clusterRadius = 0.0f;
    if (const MotionWork *const motionWork = FindMotionWork(character)) {
        control->collisionCenter = motionWork->runtime.clusterCenter;
        clusterRadius = motionWork->runtime.clusterRadius;
    }
    control->collisionCenter.y = translation.y;
    const bool preserveRadius = (control->flags & CharacterControlPreserveCollisionRadius) != 0;
    if (!preserveRadius)
        control->collisionCenterRadius = clusterRadius + 0.1f;
    if ((control->flags & CharacterControlFineCollision) != 0) {
        control->collisionCenter = translation;
        if (!preserveRadius)
            control->collisionCenterRadius = control->collisionRadius;
        if (plymIsSearchStartRequested() && plymIsSearchBlocked())
            control->flags |= CharacterControlSearchBlocked;
        if (plymIsLookAllowed(64))
            control->flags |= CharacterControlLookMode;
    }
    control->flags &= ~CharacterControlPreserveCollisionRadius;
    if ((control->flags & CharacterControlIntersectionHit) != 0)
        control->flags = (control->flags & ~CharacterControlIntersectionHit) | CharacterControlSearchBlocked;
    coliPositionAdd(character, &control->collisionAdjustment);
    control->collisionAdjustment = {};
}





void GatherCharacterControl(Character *character, void *context) {
    CharacterControlGatherContext &gather = *static_cast<CharacterControlGatherContext *>(context);
    if (gather.full || (character->flags & CF_Active) == 0)
        return;

    CharacterControlModel *const control = FindCharacterControlModel(character);
    if (!control)
        return;

    const std::uint32_t flags = control->flags;
    control->flags &= ~kCharacterControlGatherClearFlags;
    if ((flags & 0x00000409U) != (CharacterControlUpdateHeight | CharacterControlVoxelSweep) || (HeaderForPayload(control)->flags & 1U) == 0)
        return;
    if (gather.count >= gather.controls.size()) {
        gather.full = true;
        return;
    }
    control->owner = character;
    gather.controls[gather.count++] = control;
}


Vector3 PairProbePoint(const CharacterControlModel &control, const Vector3 &share) {
    return {control.collisionCenter.x + share.x, control.collisionCenter.y + share.y + 0.40000001f, control.collisionCenter.z + share.z};
}



void GetMotionResidualRootDelta(const MotionWork *work, Vector3 &delta) {
    if ((work->runtime.motnFlags & 1U) != 0) {
        delta.x = work->runtime.rootPosition.x - work->runtime.previousRootPosition.x - work->runtime.rootDelta.x;
        delta.y = 0.0f;
        delta.z = work->runtime.rootPosition.z - work->runtime.previousRootPosition.z - work->runtime.rootDelta.z;
    } else {
        delta.x = 0.0f;
        delta.z = 0.0f;
    }
}






void ResolveCharacterControlPair(CharacterControlModel &first, CharacterControlModel &second) {
    const Vector3 firstCenter{
        first.collisionCenter.x + first.pairAdjustment.x, first.collisionCenter.y + first.pairAdjustment.y, first.collisionCenter.z + first.pairAdjustment.z};
    const Vector3 secondCenter{second.collisionCenter.x + second.pairAdjustment.x, second.collisionCenter.y + second.pairAdjustment.y,
        second.collisionCenter.z + second.pairAdjustment.z};
    if (std::fabs(firstCenter.y - secondCenter.y) > 2.5f)
        return;

    Vector3 direction{secondCenter.x - firstCenter.x, 0.0f, secondCenter.z - firstCenter.z};
    const float radius = second.collisionCenterRadius + first.collisionCenterRadius;
    const float distanceSquared = direction.x * direction.x + 0.0f + direction.z * direction.z;
    if (distanceSquared >= radius * radius)
        return;
    const float overlap = radius - std::sqrt(distanceSquared);
    if (std::fabs(distanceSquared) < kCharacterControlEpsilon)
        direction = {1.0f, 0.0f, 0.0f};
    else
        NormalizeVector3(&direction);

    float firstWeight = 0.5f;
    float secondWeight = 0.5f;
    const float displacementSum = first.displacementSquared + second.displacementSquared;
    if (std::fabs(displacementSum) >= kCharacterControlEpsilon) {
        firstWeight = std::fabs(first.displacementSquared / displacementSum);
        secondWeight = std::fabs(second.displacementSquared / displacementSum);
    }
    if ((first.flags & kCharacterControlPairImmovableFlags) != 0) {
        firstWeight = 0.0f;
        secondWeight = (second.flags & kCharacterControlPairImmovableFlags) != 0 ? 0.0f : 1.0f;
    } else if ((second.flags & kCharacterControlPairImmovableFlags) != 0) {
        firstWeight = 1.0f;
        secondWeight = 0.0f;
    }
    const float firstScale = -(firstWeight * overlap);
    const float secondScale = secondWeight * overlap;
    Vector3 firstShare{direction.x * firstScale, direction.y * firstScale, direction.z * firstScale};
    Vector3 secondShare{direction.x * secondScale, direction.y * secondScale, direction.z * secondScale};

    const std::uint32_t combined = first.flags | second.flags;
    if ((combined & CharacterControlFineCollision) != 0 && (combined & CharacterControlImmovable) == 0 && (combined & CharacterControlLookMode) == 0) {
        const bool firstFine = (first.flags & CharacterControlFineCollision) != 0;
        CharacterControlModel &fine = firstFine ? first : second;
        CharacterControlModel &other = firstFine ? second : first;
        Vector3 &fineShare = firstFine ? firstShare : secondShare;
        Vector3 &otherShare = firstFine ? secondShare : firstShare;
        float fineWeight = firstFine ? firstWeight : secondWeight;
        float otherWeight = firstFine ? secondWeight : firstWeight;
        bool rayBlocked = false;
        Vector3 fineHit{};
        Vector3 otherHit{};
        Vector3 fineProbe = PairProbePoint(fine, fineShare);
        Vector3 otherProbe = PairProbePoint(other, otherShare);
        std::int32_t fineWalled = RaycastInWorldGrid(fineHit, fineProbe, fine.collisionRadius * 1.1f);
        std::int32_t otherWalled = RaycastInWorldGrid(otherHit, otherProbe, other.collisionRadius * 1.05f);
        if (fineWeight != 0.0f) {
            if (fineWalled != 0 && RaycastForCollision(&fineHit, &otherProbe, &fineProbe)) {
                rayBlocked = true;
                otherWalled = 0;
                fineWalled = 0;
            }
            if (fineWalled != 0) {
                bool readRootTravel = true;
                bool testReversal = false;
                if (static_cast<double>(fine.displacementSquared) >= 0.001) {
                    testReversal = otherWalled == 0;
                } else if (otherWalled != 0) {
                    fineShare = {};
                    otherShare = {};
                    const float pushScale = overlap * -0.1f;
                    const float pullScale = overlap * 0.1f;
                    firstShare = {direction.x * pushScale, direction.y * pushScale, direction.z * pushScale};
                    secondShare = {direction.x * pullScale, direction.y * pullScale, direction.z * pullScale};
                    readRootTravel = false;
                } else if (fine.displacementSquared < other.displacementSquared) {
                    otherShare = {otherShare.x - fineShare.x, otherShare.y - fineShare.y, otherShare.z - fineShare.z};
                    fineShare = {};
                    readRootTravel = false;
                } else {
                    testReversal = true;
                }
                const bool reversed =
                    testReversal && static_cast<float>(static_cast<std::int16_t>(Vector3_ComputeAngleBetween(fine.residualMovement, fineShare))) < -0.70700002f;
                if (reversed) {
                    fineShare = {fineShare.x * 0.1f, fineShare.y * 0.1f, fineShare.z * 0.1f};
                    otherShare = {otherShare.x - fineShare.x, otherShare.y - fineShare.y, otherShare.z - fineShare.z};
                }
                if (readRootTravel) {
                    GetMotionResidualRootDelta(FindMotionWork(fine.owner), fineShare);
                    fineShare = {fineShare.x * -1.0f, fineShare.y * -1.0f, fineShare.z * -1.0f};
                }
            }
        }

        fineProbe = PairProbePoint(fine, fineShare);
        otherProbe = PairProbePoint(other, otherShare);
        fineWalled = RaycastInWorldGrid(fineHit, fineProbe, fine.collisionRadius);
        otherWalled = RaycastInWorldGrid(otherHit, otherProbe, other.collisionRadius);
        bool resolve = false;
        if (!rayBlocked) {
            if (fineWalled != 0) {
                if (otherWalled == 0) {
                    fineWeight = 0.0f;
                    otherWeight = 1.0f;
                }
                resolve = true;
            } else if (otherWalled != 0 && otherWeight != 0.0f) {
                fineWeight = static_cast<double>(fine.displacementSquared) > 0.001 ? 1.0f : 0.5f;
                otherWeight = 0.0f;
                resolve = true;
            }
        }
        bool fineRebased = false;
        if (resolve) {
            Vector3 separation{otherProbe.x - fineProbe.x, 0.0f, otherProbe.z - fineProbe.z};
            const float separationSquared = separation.x * separation.x + 0.0f + separation.z * separation.z;
            if (radius * radius > separationSquared) {
                if (std::fabs(separationSquared) < kCharacterControlEpsilon)
                    separation = {1.0f, 0.0f, 0.0f};
                else
                    NormalizeVector3(&separation);
                const float remaining = radius - std::sqrt(separationSquared);
                const float fineScale = -(fineWeight * remaining);
                const float otherScale = otherWeight * remaining;
                fineShare = {separation.x * fineScale, separation.y * fineScale, separation.z * fineScale};
                otherShare = {separation.x * otherScale, separation.y * otherScale, separation.z * otherScale};
                fineRebased = true;
            }
        }

        bool otherRebased = fineRebased;
        fineProbe = PairProbePoint(fine, fineShare);
        otherProbe = PairProbePoint(other, otherShare);
        if (RaycastInWorldGrid(fineHit, fineProbe, fine.collisionRadius) != 0 && !rayBlocked) {
            fineHit.y += -0.40000001f;
            fineShare = {fineHit.x - fine.collisionCenter.x, fineHit.y - fine.collisionCenter.y, fineHit.z - fine.collisionCenter.z};
            fineRebased = true;
        }
        if ((std::fabs(otherShare.x) >= kCharacterControlEpsilon || std::fabs(otherShare.y) >= kCharacterControlEpsilon ||
                std::fabs(otherShare.z) >= kCharacterControlEpsilon) &&
            (other.flags & (CharacterControlFineRaycast | CharacterControlEvaluateMotion)) == (CharacterControlFineRaycast | CharacterControlEvaluateMotion)) {
            if (RaycastInWorldGrid(otherHit, otherProbe, other.collisionRadius) != 0 && !rayBlocked) {
                otherHit.y += -0.40000001f;
                otherShare = {otherHit.x - other.collisionCenter.x, otherHit.y - other.collisionCenter.y, otherHit.z - other.collisionCenter.z};
                otherRebased = true;
            }
        }
        if (fineRebased)
            fineShare = {fineShare.x - fine.pairAdjustment.x, fineShare.y - fine.pairAdjustment.y, fineShare.z - fine.pairAdjustment.z};
        if (otherRebased)
            otherShare = {otherShare.x - other.pairAdjustment.x, otherShare.y - other.pairAdjustment.y, otherShare.z - other.pairAdjustment.z};
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (!std::isnormal(fineShare[axis]) && fineShare[axis] != 0.0f)
                fineShare[axis] = 0.0f;
        }
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (!std::isnormal(otherShare[axis]) && otherShare[axis] != 0.0f)
                otherShare[axis] = 0.0f;
        }
    }

    first.pairAdjustment = {first.pairAdjustment.x + firstShare.x, first.pairAdjustment.y + firstShare.y, first.pairAdjustment.z + firstShare.z};
    second.pairAdjustment = {second.pairAdjustment.x + secondShare.x, second.pairAdjustment.y + secondShare.y, second.pairAdjustment.z + secondShare.z};
}

void ApplyCharacterControlPairAdjustment(CharacterControlModel &control) {
    const Vector3 adjustment = control.pairAdjustment;
    if ((std::fabs(adjustment.x) < kCharacterControlEpsilon && std::fabs(adjustment.y) < kCharacterControlEpsilon &&
            std::fabs(adjustment.z) < kCharacterControlEpsilon) ||
        (control.flags & CharacterControlVoxelSweep) == 0) {
        return;
    }

    if ((control.flags & CharacterControlFineCollision) != 0) {
        Vector3 rayEnd = control.previousWorldPosition;
        rayEnd.y += 0.40000001f;
        const Vector3 rayStart{rayEnd.x + adjustment.x, rayEnd.y + adjustment.y, rayEnd.z + adjustment.z};
        Vector3 intersection{};
        if (RaycastForCollision(&intersection, &rayStart, &rayEnd))
            return;
    }

    control.previousWorldPosition.x += adjustment.x;
    control.previousWorldPosition.y += adjustment.y;
    control.previousWorldPosition.z += adjustment.z;
    control.owner->position.x += adjustment.x;
    control.owner->position.y += adjustment.y;
    control.owner->position.z += adjustment.z;
}



void CharacterControlModelPairTask_Callback(void *) {
    ForEachLiveCharacterInMapOrder(CharCollisionSort, nullptr);
    std::array<CharacterControlModel *, kMaximumCharacterCollisionCount> controls{};
    const std::size_t count = GatherCharacterControls(controls);
    if (count <= 1)
        return;
    for (std::size_t left = 0; left < count; ++left) {
        for (std::size_t right = left + 1; right < count; ++right)
            ResolveCharacterControlPair(*controls[left], *controls[right]);
    }
    for (std::size_t index = 0; index < count; ++index)
        ApplyCharacterControlPairAdjustment(*controls[index]);
}

void ClearLiveCharacterControlForTeardown(Character *character, void *) {
    if (CharacterControlModel *const control = LiveCharacterControl(character)) {
        CleanupCharacterControlModel(character, control);
    }
}

} // namespace model_component_detail

std::size_t GatherCharacterControls(std::span<CharacterControlModel *> output) {
    CharacterControlGatherContext gather{output.first(std::min(output.size(), kMaximumCharacterCollisionCount))};
    ForEachLiveCharacterInMapOrder(GatherCharacterControl, &gather);
    return gather.count;
}

// @verified - p
bool CheckCharacterCylinderIntersection(const Vector3 &start, const Vector3 &end) {
    g_characterQueryPointFound = 0;
    g_characterQueryPointOwner = nullptr;
    std::array<CharacterControlModel *, kMaximumCharacterCollisionCount> candidates{};
    const std::size_t count = GatherCharacterControls(candidates);
    for (std::size_t index = 0; index < count; ++index) {
        CharacterControlModel &control = *candidates[index];
        Vector3 centre = GetCharacterWorldPosition(control.owner);
        centre.y += -1.25f;
        const bool hit = PartySegmentHitsCylinder(start, end, centre, control.collisionRadius);
        centre.y += 1.25f;
        if (!hit)
            continue;
        control.flags |= 0x10000000U;
        g_characterQueryPointFound = 1;
        g_characterQueryPointControl = &control;
        g_characterQueryPointOwner = control.owner;
        return true;
    }
    return false;
}

// @verified - p
bool ResolveCharacterCylinderOverlap(Vector3 &result, const Vector3 &position, float radius) {
    result = position;
    std::array<CharacterControlModel *, kMaximumCharacterCollisionCount> controls{};
    const std::size_t count = GatherCharacterControls(controls);
    bool corrected = false;
    for (std::size_t index = 0; index < count; ++index) {
        CharacterControlModel &control = *controls[index];
        const Vector3 centre = GetCharacterWorldPosition(control.owner);
        Vector3 offset{result.x - centre.x, 0.0f, result.z - centre.z};
        const float combinedRadius = radius + control.collisionRadius;
        const float distanceSquared = (offset.x * offset.x + 0.0f) + offset.z * offset.z;
        const float penetration = combinedRadius * combinedRadius - distanceSquared;
        if (!(penetration > 0.0f))
            continue;
        if (std::fabs(distanceSquared) < std::numeric_limits<float>::epsilon())
            offset = {1.0f, 0.0f, 0.0f};
        else
            Vector3::NormalizeInPlace(offset);
        const float scale = penetration + 0.050000001f;
        result.x += offset.x * scale;
        result.y += offset.y * scale;
        result.z += offset.z * scale;
        control.flags |= 0x10000000U;
        corrected = true;
    }
    return corrected;
}

// @verified - p
bool CheckCharacterSphereIntersection(const Vector3 &point, float radius) {
    g_characterQueryPointFound = 0;
    g_characterQueryPointOwner = nullptr;
    std::array<CharacterControlModel *, kMaximumCharacterCollisionCount> candidates{};
    const std::size_t count = GatherCharacterControls(candidates);
    for (std::size_t index = 0; index < count; ++index) {
        CharacterControlModel &control = *candidates[index];
        const Vector3 centre = GetCharacterWorldPosition(control.owner);
        const Vector3 delta{point.x - centre.x, point.y - centre.y, point.z - centre.z};
        const float combinedRadius = radius + control.collisionRadius;
        const float distanceSquared = (delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z;
        if (!(combinedRadius * combinedRadius - distanceSquared > 0.0f))
            continue;
        control.flags |= 0x10000000U;
        g_characterQueryPointFound = 1;
        g_characterQueryPointControl = &control;
        g_characterQueryPointOwner = control.owner;
        return true;
    }
    return false;
}

// @verified - p
Character *GetPointFound() {
    return g_characterQueryPointFound ? g_characterQueryPointControl->owner : nullptr;
}

// @verified - p
Character *CheckPointInCylinder_Wrapper(const Vector3 &start, const Vector3 &end) {
    g_characterQueryPointFound = 0;
    g_characterQueryPointOwner = nullptr;
    std::array<CharacterControlModel *, kMaximumCharacterCollisionCount> candidates{};
    const std::size_t count = GatherCharacterControls(candidates);
    float nearest = (std::numeric_limits<float>::max)();
    Character *result = nullptr;
    for (std::size_t index = 0; index < count; ++index) {
        CharacterControlModel &control = *candidates[index];
        const Vector3 center{control.collisionCenter.x, control.collisionCenter.y - 1.25f, control.collisionCenter.z};
        if (!PartySegmentHitsCylinder(start, end, center, control.collisionCenterRadius * 0.85000002f))
            continue;
        const float distance = PartyHorizontalDistanceSquared(center, start);
        if (distance < nearest) {
            control.flags |= 0x10000000U;
            nearest = distance;
            result = control.owner;
            g_characterQueryPointFound = 1;
            g_characterQueryPointControl = &control;
            g_characterQueryPointOwner = control.owner;
        }
    }
    return result;
}

CharacterControlModel *FindCharacterControlModel(const Character *character) {
    return static_cast<CharacterControlModel *>(FindModelComponent(character, kCharacterControlToken));
}



// @verified - p
void toggleCCMflagsByAmt(Character *character, std::uint32_t flags) {
    CharacterControlModel *const control = FindCharacterControlModel(character);
    if (!control)
        return;
    if (flags & 0x100000U) {
        flags = (flags & ~0x401000dfU) | 0x400000dfU;
        if (character == ctrlGetRootNode())
            flags |= 0x04400200U;
    }
    control->flags |= flags;
}


// @verified - p
void ccmSetFlagsFor(Character *character, std::uint32_t flags) {
    if (CharacterControlModel *const control = FindCharacterControlModel(character))
        control->flags &= ~flags;
}


// @verified - p
void ccmUpdateEntityFlag(Character *character) {
    if (CharacterControlModel *const control = FindCharacterControlModel(character))
        control->flags |= 0x400U;
}

float GetCharacterCollisionRadius(const Character *character) {
    const CharacterControlModel *const control = FindCharacterControlModel(character);
    return control ? control->collisionRadius : 0.0f;
}

float GetCharacterCollisionCapsuleMargin(const Character *character) {
    const CharacterControlModel *const control = FindCharacterControlModel(character);
    return control ? control->collisionCenterRadius : 0.0f;
}

CharacterControlModel *SetCharacterCollisionCapsuleMargin(Character *character, float margin) {
    CharacterControlModel *const control = FindCharacterControlModel(character);
    if (control) {
        control->flags |= CharacterControlPreserveCollisionRadius;
        control->collisionCenterRadius = margin;
    }
    return control;
}

bool AttachCharacterControlModel(Character *character) {
    if (!character)
        return false;

    auto *const control = static_cast<CharacterControlModel *>(
        AllocateModelComponent(character, kCharacterControlToken, sizeof(CharacterControlModel), UpdateCharacterControlModel));
    if (!control)
        return false;

    control->flags = kCharacterControlInitialFlags;
    control->owner = character;
    control->maximumStepHeight = 0.40000001f;
    control->collisionRadius = 0.40000001f;
    control->collisionHeight = 0.40000001f;
    control->previousWorldPosition = GetCharacterWorldPosition(character);
    if (character == ctrlGetRootNode())
        control->flags |= kCharacterControlRootFlags;
    return true;
}

bool QueueCharacterControlModelPairTask(std::uint8_t nextFunctionIndex) {
    characterControlPairTask =
        HLib::EnqueueTaskWithParameter(CharacterControlModelPairTask_Callback, nextFunctionIndex, 4, sizeof(std::uint32_t), SHEN_NAME(CCT_));
    return characterControlPairTask != nullptr;
}

void DestroyCharacterControlModelPairTask() {
    HLib::Task *const task = characterControlPairTask;
    characterControlPairTask = nullptr;
    if (task)
        HLib::DestroyTask(task);
    ForEachLiveCharacterInMapOrder(ClearLiveCharacterControlForTeardown, nullptr);
}

} // namespace shenmue::s2
