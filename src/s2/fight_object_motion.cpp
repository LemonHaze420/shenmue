#include "s2/fight_stage.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/sound_program.hpp"

#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {
void SetObjectEuler(const Vector3i &angles) {
    Matrix4x4::SetViewMatrixFromEuler(angles.y, angles.x, angles.z);
}

// @verified - p
Vector3i ExtractObjectEuler(const Matrix4x4 &matrix) {
    const float sine = -matrix.rows[0].z;
    Vector3i angles;
    if (sine - 1 < -.0000076295109f) {
        if (1 - matrix.rows[0].z > .0000076295109f) {
            angles = {
                FixedAtan2(matrix.rows[1].z, matrix.rows[2].z), FixedAtan2(sine, std::sqrt(1 - sine * sine)), FixedAtan2(matrix.rows[0].y, matrix.rows[0].x)};
        } else {
            angles = {FixedAtan2(-matrix.rows[1].x, matrix.rows[1].y), 49152, 0};
        }
    } else
        angles = {FixedAtan2(matrix.rows[1].x, matrix.rows[1].y), 16384, 0};
    if (std::uint32_t(angles.x) >= 32768 && std::uint32_t(angles.z) >= 32768) {
        angles.x -= 32768;
        angles.z -= 32768;
        angles.y = 32768 - angles.y;
    }
    return angles;
}
void ExtractObjectPlanarEuler(FightStageObjectRuntime &object, const Matrix4x4 &matrix) {
    const float z = matrix.rows[1].z, x = matrix.rows[1].x;
    float magnitude = std::sqrt(x * x + z * z);
    if (z * matrix.rows[0].x - x * matrix.rows[0].z < 0)
        magnitude = -magnitude;
    object.rotation = {FixedAtan2(magnitude, matrix.rows[1].y), -std::int32_t(FixedAtan2(matrix.rows[0].z, matrix.rows[0].x)), 0};
}
std::uint32_t g_vf_hit_object_handled{};
// @verified - p
VFActor *ObjectActor(VFMA_t *game, Character *character) {
    if (!character)
        return nullptr;
    for (std::size_t i = 0; i != 6; ++i)
        if (game->characters[i] == character)
            return game->actors[i];
    return nullptr;
}
bool ObjectHasHitSpeed(const Vector3 &velocity) {
    return std::fabs(velocity.x) > .02f || std::fabs(velocity.y) > .02f || std::fabs(velocity.z) > .02f;
}
}


// @verified - p
std::int32_t vfAllocateStageObjectMotionTracks(VOBJ_t *object) {
    const FightStageObjectDefinition *const definition = object->object.definition;
    const std::int32_t count = definition->motionTrackCount;
    ObjectMotionTrack *const tracks = static_cast<ObjectMotionTrack *>(
        HLib::AllocateTaskData(static_cast<std::uint64_t>(static_cast<std::int64_t>(count) * sizeof(ObjectMotionTrack)), TASK_VOBJ));
    if (!tracks)
        return 1;
    object->motionTracks = tracks;
    const ObjectMotionRecord *record = definition->motionKeys;
    const ObjectMotionNode *const nodes = definition->motionNodes;
    Character *const model = object->object.brokenModel;
    const std::int32_t frame = definition->motionFirstFrame;
    for (std::int32_t i = 0; i < count; ++i) {
        ObjectMotionTrack &track = tracks[i];
        MT7Model *const mt7 = static_cast<MT7Model *>(model->model);
        track.node = mt7 && !mt7->nodes.empty() ? GetChildObjectByIndex(*mt7, mt7->nodes.data(), nodes[i].index) : nullptr;
        const ObjectMotionChannel *channel = reinterpret_cast<const ObjectMotionChannel *>(record + 1);
        for (std::size_t channelIndex = 0; channelIndex != 6; ++channelIndex) {
            const std::int32_t keyCount = channel->keyCount;
            ObjectMotionCurve &curve = track.curves[channelIndex];
            curve.keys = reinterpret_cast<const ObjectMotionKey *>(channel + 1);
            std::int32_t cursor = 1;
            for (; cursor < keyCount; ++cursor)
                if (frame < curve.keys[cursor].frame)
                    break;
            curve.cursor = static_cast<std::int16_t>(cursor);
            curve.count = static_cast<std::int16_t>(keyCount);
            channel = reinterpret_cast<const ObjectMotionChannel *>(curve.keys + keyCount);
        }
        record = reinterpret_cast<const ObjectMotionRecord *>(reinterpret_cast<const std::byte *>(record) + record->byteSize);
    }
    return 0;
}


// @verified - p
void vfStageObjectEvalHitboxKeyframes() {
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next) {
        if (object->object.state != 1)
            continue;
        const FightStageObjectDefinition *const definition = object->object.definition;
        const std::int32_t frame = object->motionFrame;
        ObjectMotionTrack *const tracks = object->motionTracks;
        const std::int32_t count = definition->motionTrackCount;
        for (std::int32_t i = 0; i < count; ++i) {
            ObjectMotionTrack &track = tracks[i];
            for (std::size_t channel = 0; channel != 6; ++channel) {
                ObjectMotionCurve &curve = track.curves[channel];
                std::int16_t cursor = curve.cursor;
                for (; cursor < curve.count; ++cursor)
                    if (frame < curve.keys[cursor].frame)
                        break;
                curve.cursor = cursor;
                float value;
                if (cursor == curve.count)
                    value = curve.keys[cursor - 1].value;
                else {
                    const ObjectMotionKey &previous = curve.keys[cursor - 1];
                    const ObjectMotionKey &next = curve.keys[cursor];
                    value = ((next.value - previous.value) * float(frame - previous.frame)) / float(next.frame - previous.frame) + previous.value;
                }
                if (channel < 3) {
                    const float translated = value * 10;
                    if (channel == 0)
                        track.node->position.x = translated;
                    else if (channel == 1)
                        track.node->position.y = translated;
                    else
                        track.node->position.z = translated;
                } else {
                    const std::int32_t angle = static_cast<std::int32_t>(value * 182.04445f);
                    if (channel == 3)
                        track.node->rotation.x = float(angle);
                    else if (channel == 4)
                        track.node->rotation.y = float(angle);
                    else
                        track.node->rotation.z = float(angle);
                }
            }
        }
        const std::int32_t nextFrame = frame + definition->motionStep;
        const std::int32_t end = definition->motionLastFrame;
        object->motionFrame = nextFrame;
        if (nextFrame >= end) {
            object->motionFrame = end;
            object->object.state = 2;
            HLib::FreeTaskData(object->motionTracks);
            object->motionTracks = nullptr;
        }
    }
}

// @verified - p
void vfConvertToStageObjectHitboxSpace(VOBJ_t *object, Vector3 &point) {
    Matrix4x4::SetPrePassToTempTransform();
    SetObjectEuler(object->object.rotation);
    Matrix4x4_Push();
    Matrix4x4::AffineProjectionSwap();
    Vector3 local;
    Matrix4x4::Transform(point, local);
    Matrix4x4::Pop(1);
    local.x = 0;
    Matrix4x4::Transform(local, point);
}


// @verified - p
void vfRecalcStageObjectLocalEuler(VOBJ_t *object) {
    Matrix4x4::SetPrePassToTempTransform();
    const Vector3i impulse = object->object.rotationImpulse;
    if (object->object.reactionFlags & 0x20) {
        SetObjectEuler(impulse);
        SetObjectEuler(object->object.rotation);
    } else {
        SetObjectEuler(object->object.rotation);
        SetObjectEuler(impulse);
    }
    Matrix4x4 matrix;
    Matrix4x4::GetTransformMatrix(&matrix);
    object->object.rotation = ExtractObjectEuler(matrix);
}


// @verified - p
void vfCreateStageObjectRotationMatrix(VOBJ_t *object, const Vector3 &displacement) {
    const float length = std::sqrt((displacement.x * displacement.x + displacement.y * displacement.y) + displacement.z * displacement.z);
    std::int32_t rotation = static_cast<std::uint16_t>(static_cast<std::int32_t>(length * float(object->object.definition->rotationSpeed)));
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-object->object.rotation.y));
    Vector3 local;
    Matrix4x4::Transform(displacement, local);
    if (local.z < 0)
        rotation = -rotation;
    Matrix4x4::SetPrePassToTempTransform();
    SetObjectEuler(object->object.rotation);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation));
    object->object.rotationImpulse = {.x = rotation, .y = 0, .z = 0};
    object->object.reactionFlags |= 0x40;
    Matrix4x4 matrix;
    Matrix4x4::GetTransformMatrix(&matrix);
    ExtractObjectPlanarEuler(object->object, matrix);
}

// Each live effect countdown interpolates the model's actual scale.
// @verified - p
void vfUpdateStageObjectWorldPos(VOBJ_t *object) {
    const std::uint16_t x = object->scaleTicks[0], y = object->scaleTicks[1], z = object->scaleTicks[2];
    if (!(x || y || z))
        return;
    Vector3 scale;
    GetCharacterScale(object->object.model, &scale);
    const auto advance = [](float &value, float &target, std::uint16_t &ticks, std::uint16_t count) {
        if (!count)
            return;
        value += (target - value) / float(count);
        ticks = static_cast<std::uint16_t>(count - 1);
        if (count == 1)
            target = 0.0f;
    };
    advance(scale.x, object->scaleTarget.x, object->scaleTicks[0], x);
    advance(scale.y, object->scaleTarget.y, object->scaleTicks[1], y);
    advance(scale.z, object->scaleTarget.z, object->scaleTicks[2], z);
    SetCharacterScale(object->object.model, &scale);
}


// @verified - p
void vfClAicaReact(const Vector3 &position, std::uint32_t flags) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    VFMA_t *const game = task ? static_cast<VFMA_t *>(HLib::GetTaskParameter(task)) : nullptr;
    std::uint32_t material = game && (g_pFightStage->battleFlags & 0x40) ?
        static_cast<std::uint8_t>(game->cameraParameter) :
        std::uint32_t(static_cast<std::int32_t>(QueryGroundMaterialToken(position, 0)) >> 16);
    if (material > 13)
        material = 13;
    Execute3DSoundCommand(flags | ((material | 0xfe0u) << 19), position, 2, 30);
}

// Authored quadrant deflection clears the high halfword only on admission.
// @verified - p
void vfDeflectStageObjectYaw(VOBJ_t *owner, const Vector3 &direction) {
    FightStageObjectRuntime &object = owner->object;
    if (std::fabs(direction.x) < .00000011920929f && std::fabs(direction.z) < .00000011920929f)
        return;
    if (!(object.definition->flags & 0x8U))
        return;
    const std::uint16_t quadrant = static_cast<std::uint16_t>(0x8000 - object.rotation.y + FixedAtan2(direction.x, direction.z));
    if (quadrant <= 0x2000)
        return;
    if (quadrant <= 0x6000)
        object.rotation.y += 0x4000;
    else if (quadrant <= 0xa000)
        object.rotation.y += 0x8000;
    else if (quadrant <= 0xe000)
        object.rotation.y += 0xc000;
    object.rotation.y &= 0xffff;
}

// Rotation about the authored offset updates both contact and world centres.
// @verified - p
void vfRotateStageObjectAboutOffset(VOBJ_t *owner, const Vector3 &offset) {
    FightStageObjectRuntime &object = owner->object;
    Matrix4x4::SetPrePassToTempTransform();
    SetObjectEuler(object.rotation);
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    Matrix4x4 matrix;
    Matrix4x4::GetTransformMatrix(&matrix);
    object.position = {matrix.rows[3].x + object.contactPosition.x, matrix.rows[3].y + object.contactPosition.y, matrix.rows[3].z + object.contactPosition.z};
    const std::uint32_t flags = object.definition->flags;
    if (flags & 0x80)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(object.yaw));
    else if (flags & 0x100)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(object.yaw));
    else
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(object.yaw));
    Matrix4x4::preTranslate(-offset.x, -offset.y, -offset.z);
    Matrix4x4::GetTransformMatrix(&matrix);
    object.contactPosition.x += matrix.rows[3].x;
    object.contactPosition.y += matrix.rows[3].y;
    object.contactPosition.z += matrix.rows[3].z;
    ExtractObjectPlanarEuler(object, matrix);
}

namespace {
float ObjectDistanceSquared(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x, y = first.y - second.y, z = first.z - second.z;
    return (y * y + x * x) + z * z;
}
bool ObjectHitboxAdmitted(const VOBJ_t *object, std::size_t index) {
    const FightStageObjectDefinition *const definition = object->object.definition;
    const std::int16_t kind = definition->hitboxes[index].kind;
    const std::int32_t state = kind == 1 || kind == 2 ? definition->hitboxes[index].state : 0;
    return state < 0 ? object->object.state != 0 : object->object.state == 0;
}
}


// @verified - p
void vfTransformStageObjectHitboxes(VOBJ_t *owner) {
    const FightStageObjectRuntime &object = owner->object;
    const FightStageObjectDefinition *const definition = object.definition;
    const std::uint32_t flags = definition->flags;
    if (!(flags & (object.state ? 4 : 2)))
        return;
    const std::int32_t count = definition->hitboxCount;
    for (std::int32_t i = 0; i < count; ++i) {
        if (definition->hitboxes[i].kind != 1)
            continue;
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(object.contactPosition.x, object.contactPosition.y, object.contactPosition.z);
        SetObjectEuler(object.rotation);
        Vector3 world;
        Matrix4x4::TransformPoint(definition->hitboxes[i].position, world);
        owner->object.hitboxes[i].position = world;
    }
}


// @verified - p
bool vfStageObjectOverlapsActor(VOBJ_t *owner, VFActor *actor, std::int32_t withVelocity) {
    const Vector3 &centre = actor->game->collision->samples[actor->groupIndex].points[0];
    if (ObjectDistanceSquared(centre, owner->object.contactPosition) > g_fightStageObjectMaximumDistance)
        return false;
    const std::int32_t motion = GetMotionAnimationId(FindMotionWork(actor->game->characters[actor->groupIndex]), MotionRegion::LowerBody) & 0xfff;
    if (motion == 1608) {
        if (float(actor->parameter2e0) > actor->animationTime)
            return false;
    } else if (motion == 1607 || motion == 1609)
        return false;
    const FightStageObjectDefinition *const definition = owner->object.definition;
    const VFCollisionShape *const body = reinterpret_cast<const VFCollisionShape *>(actor->parameterAd8);
    const Vector3 velocity = withVelocity ? owner->object.velocity : Vector3{};
    const std::int32_t count = definition->hitboxCount;
    for (std::int32_t i = 0; i < count; ++i) {
        if (!ObjectHitboxAdmitted(owner, i) || definition->hitboxes[i].kind != 1)
            continue;
        Vector3 point = owner->object.hitboxes[i].position;
        point.x += velocity.x;
        point.y += velocity.y;
        point.z += velocity.z;
        const float radius = definition->hitboxes[i].radius;
        const float broadRadius = radius + .4f;
        if (ObjectDistanceSquared(centre, point) <= broadRadius * broadRadius)
            owner->object.contactSlotMask |= std::uint64_t{1} << (32 + actor->groupIndex);
        for (std::size_t bone = 0; bone != 10; ++bone) {
            const float combined = radius + body[bone].radius;
            if (ObjectDistanceSquared(actor->motionContactSamples[bone], point) <= combined * combined)
                return true;
        }
    }
    return false;
}


// @verified - p
void vfResolveActorStageObjectPenetration(VFActor *actor, VOBJ_t *owner) {
    FightStageObjectRuntime &object = owner->object;
    const FightStageObjectDefinition *const definition = object.definition;
    const VFCollisionShape *const shapes = reinterpret_cast<const VFCollisionShape *>(actor->parameterAd8);
    const Vector3 &centre = actor->game->collision->samples[actor->groupIndex].points[0];
    const std::int32_t count = definition->hitboxCount;
    for (std::int32_t sphere = 0; sphere < count; ++sphere) {
        if (!ObjectHitboxAdmitted(owner, sphere) || definition->hitboxes[sphere].kind != 1)
            continue;
        std::memcpy(&object.hitboxes[sphere].radius, &definition->hitboxes[sphere].radius, sizeof(float));
        const Vector3 point = owner->object.hitboxes[sphere].position;
        const float radius = owner->object.hitboxes[sphere].radius;
        float deepest = float(std::sqrt(double(object.reactionRotation.y * object.reactionRotation.y + object.reactionRotation.z * object.reactionRotation.z)));
        for (std::size_t i = 0; i != 10; ++i) {
            const Vector3 &sample = actor->motionContactSamples[i];
            const float x = point.x - sample.x, y = point.y - sample.y, z = point.z - sample.z;
            const float distance = float(std::sqrt(double((y * y + x * x) + z * z)));
            const float penetration = (shapes[i].radius + radius - distance) + .01f;
            if (penetration <= 0)
                continue;
            float pushX, pushZ;
            if (std::fabs(distance) >= .00000011920929f) {
                pushX = penetration * x / distance;
                pushZ = penetration * z / distance;
            } else {
                const float deltaX = object.contactPosition.x - centre.x, deltaZ = object.contactPosition.z - centre.z;
                const float planar = float(std::sqrt(double(deltaZ * deltaZ + deltaX * deltaX)));
                if (std::fabs(planar) >= .00000011920929f) {
                    pushX = penetration * deltaX / planar;
                    pushZ = penetration * deltaZ / planar;
                } else {
                    pushX = object.reactionRotation.y <= 0 ? -penetration : penetration;
                    pushZ = 0;
                }
            }
            const bool update = pushZ * object.reactionRotation.z + pushX * object.reactionRotation.y >= 0 ?
                penetration > deepest :
                (object.contactPosition.z - centre.z) * pushZ + (object.contactPosition.x - centre.x) * pushX > 0;
            if (update) {
                object.reactionRotation.y = pushX;
                object.reactionRotation.z = pushZ;
                deepest = penetration;
            }
        }
    }
}



// @verified - p
void vfProcessStageObjectHitOutcome(VOBJ_t *owner, VFActor *self, std::int8_t guardState, std::int16_t hitParameter, std::int8_t guardStrength) {
    g_vf_hit_object_handled |= std::uint32_t{1} << self->groupIndex;
    FightStageObjectRuntime &object = owner->object;
    VFActor *const source = ObjectActor(self->game, object.reactingCharacter);
    if ((object.reactionFlags & 1) && source && (object.motionFlags & 0x4000)) {
        const std::int8_t level = source->guardLevel;
        source->guardLevel = self->guardLevel;
        vfclApplyHitResolution(source, self);
        source->guardLevel = level;
        return;
    }
    VFActor attacker{};
    VFActor *const savedActor = self->actor;
    VFActor *const savedSecond = self->secondActor;
    self->actor = self->secondActor = &attacker;
    attacker.game = self->game;
    attacker.actor = attacker.secondActor = self;
    attacker.hitStateFlags = 0x100;
    attacker.reactionVariantBytes[3] = 80;
    attacker.reactionImpulseBytes[0] = 80;
    if (guardState == -1) {
        const float height = object.contactPosition.y - flddGetIntersectionFor(nullptr, object.contactPosition);
        guardState = height >= .60000002f ? height <= 1.2f : 2;
    }
    if (static_cast<std::uint16_t>(hitParameter - 8) <= 7) {
        attacker.hitParameter = static_cast<std::uint16_t>(1u << hitParameter);
        if (object.definition->index == 7)
            attacker.hitParameter = 0;
    }
    attacker.guardState = guardState;
    if (guardState == 0)
        attacker.hitParameter |= 0x1e;
    else if (guardState == 1)
        attacker.hitParameter |= 0x1f;
    else if (guardState == 2)
        attacker.hitParameter |= 0x20;
    attacker.hitDirection = FixedAtan2(object.contactPosition.x - self->contactPosition.x, object.contactPosition.z - self->contactPosition.z);
    attacker.guardStrength = guardStrength;
    attacker.requestedAnimation = 104;
    attacker.guardLevel = 100;
    vfclApplyHitResolution(&attacker, self);
    self->secondActor = savedSecond;
    self->actor = savedActor;
}


// @verified - p
void vfclFilterObjectHit(VFActor *actor, VOBJ_t *owner) {
    if (g_vf_hit_object_handled & (std::uint32_t{1} << actor->groupIndex))
        return;
    if ((actor->hitStateFlags & 0x10064000) || (actor->moveFlags & 0x40000))
        return;
    if ((actor->hitStateFlags & 0x80000) && (actor->secondActor->hitStateFlags & 0x40000))
        return;
    FightStageObjectRuntime &object = owner->object;
    if (object.reactingCharacter == actor->character)
        return;
    const FightStageObjectDefinition *const definition = object.definition;
    if ((object.reactionFlags & 1) && (object.motionFlags & 0x4000)) {
        if (!IsOfThisVisibilityType(object.model, 3) || (actor->activeMotionFlags & 0x2000000))
            return;
        VFActor *const source = ObjectActor(actor->game, object.reactingCharacter);
        if (!source || (source->hitStateFlags & 0x102) != 0x100)
            return;
        const std::uint32_t mask = vfGetMoveCategoryMaskFor(source);
        if (definition->attachmentBone == 14) {
            if (!(mask & 0x10))
                return;
        } else if (definition->attachmentBone != 21 || !(mask & 0x80))
            return;
    } else if (!(object.motionFlags & 0x2000)) {
        if (!(object.motionFlags & 0x1000) || !ObjectHasHitSpeed(object.velocity))
            return;
        if ((actor->contactPosition.z - object.contactPosition.z) * object.velocity.z +
                (actor->contactPosition.x - object.contactPosition.x) * object.velocity.x <=
            0)
            return;
    }
    vfProcessStageObjectHitOutcome(owner, actor, definition->guardState, definition->hitParameter, definition->guardStrength);
    const std::uint32_t flags = definition->hitSound;
    if (flags)
        vfClAicaReact(object.contactPosition, flags);
}

// An attached object borrows its authored bone transform.
// @verified - p
void vfUpdateRotationAnglesFromStageObject(VOBJ_t *owner, VFMA_t *game) {
    FightStageObjectRuntime &object = owner->object;
    VFActor *const actor = ObjectActor(game, object.reactingCharacter);
    if (actor && (((actor->hitStateFlags & 0x4000) && !(object.motionFlags & 0x400000)) || (actor->moveFlags & 0x20))) {
        vfStageObjectReset(static_cast<std::uint8_t>(object.index), nullptr);
        return;
    }
    object.contactPosition = object.definition->attachmentPosition;
    object.rotation = object.definition->attachmentRotation;
    Matrix4x4 *const bone = charaGetBoneTransform(FindMotionWork(object.reactingCharacter), object.definition->attachmentBone);
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(bone);
    Matrix4x4::preTranslate(object.contactPosition.x, object.contactPosition.y, object.contactPosition.z);
    SetObjectEuler(object.rotation);
    Matrix4x4 matrix;
    Matrix4x4::GetTransformMatrix(&matrix);
    object.contactPosition = {matrix.rows[3].x, matrix.rows[3].y, matrix.rows[3].z};
    Matrix4x4::Pop(1);
    object.rotation = ExtractObjectEuler(matrix);
}

namespace {

// @verified - p
void ApplyObjectAxisRotation(const Vector3 &from, const Vector3 &to) {
    Vector3 axis{from.y * to.z - to.y * from.z, from.z * to.x - from.x * to.z, from.x * to.y - to.x * from.y};
    float sine = std::sqrt((axis.y * axis.y + axis.x * axis.x) + axis.z * axis.z);
    float cosine = (from.x * to.x + to.y * from.y) + from.z * to.z;
    if (sine > 0) {
        const float inverse = 1 / sine;
        axis.x *= inverse;
        axis.y *= inverse;
        axis.z *= inverse;
    }
    if (sine == 0) {
        if (cosine >= 0)
            return;
        cosine = -1;
        axis = {from.y, from.z, from.x};
    }
    const float x = axis.x, y = axis.y, z = axis.z;
    const float xz = z * x * (1 - cosine), xy = y * x * (1 - cosine), yz = z * y * (1 - cosine);
    Matrix4x4 rotation{};
    rotation.rows[0] = {(1 - x * x) * cosine + x * x, z * sine + xy, xz - y * sine, 0};
    rotation.rows[1] = {xy - z * sine, (1 - y * y) * cosine + y * y, x * sine + yz, 0};
    rotation.rows[2] = {y * sine + xz, yz - x * sine, (1 - z * z) * cosine + z * z, 0};
    rotation.rows[3].w = 1;
    Matrix4x4::MultiplyWithPrePassMatrix(&rotation);
}
// Authored flags select an axis to align with the floor or its normal.
// @verified - p
void AlignStageObjectAxis(VOBJ_t *owner) {
    FightStageObjectRuntime &object = owner->object;
    Matrix4x4::SetPrePassToTempTransform();
    SetObjectEuler(object.rotation);
    const std::uint32_t flags = object.definition->flags;
    Vector3 axis;
    const bool planar = (flags & 0x1c00) != 0;
    if (planar)
        axis = flags & 0x400 ? Vector3{1, 0, 0} : flags & 0x800 ? Vector3{0, 1, 0} : Vector3{0, 0, 1};
    else
        axis = flags & 0x2000 ? Vector3{1, 0, 0} : flags & 0x4000 ? Vector3{0, 1, 0} : Vector3{0, 0, 1};
    Vector3 world;
    Matrix4x4::Transform(axis, world);
    if (world.y >= 1 || world.y <= -1) {
        if (planar)
            object.rotation = flags & 0x800 ? Vector3i{16384, 0, 0} : Vector3i{};
        return;
    }
    Vector3 target{0, 1, 0};
    if (planar) {
        target = {world.x, 0, world.z};
        const float length = std::sqrt((target.y * target.y + target.x * target.x) + target.z * target.z);
        if (length > 0) {
            const float inverse = 1 / length;
            target.x *= inverse;
            target.y *= inverse;
            target.z *= inverse;
        }
    }
    Matrix4x4::SetPrePassToTempTransform();
    ApplyObjectAxisRotation(world, target);
    SetObjectEuler(object.rotation);
    Matrix4x4 matrix;
    Matrix4x4::GetTransformMatrix(&matrix);
    object.rotation = ExtractObjectEuler(matrix);
}
struct StageObjectProbe {
    std::uint32_t flags;
    Vector3 previousPosition;
    VOBJ_t *object;
    VFActor *actor;
};
static_assert(sizeof(StageObjectProbe) == 32);

// @verified - p
void UpdateStageObjectBody(VOBJ_t *owner, StageObjectProbe &probe) {
    FightStageObjectRuntime &object = owner->object;
    const FightStageObjectDefinition *const definition = object.definition;
    object.contactPosition.x += object.reactionRotation.y;
    object.contactPosition.z += object.reactionRotation.z;
    const float groundOffset = definition->groundOffset;
    const float ground = flddGetIntersectionFor(nullptr, object.contactPosition) + groundOffset;
    if (ground < object.contactPosition.y) {
        Vector3 point{object.contactPosition.x + object.velocity.x, object.contactPosition.y + 10, object.contactPosition.z + object.velocity.z};
        float nextGround = flddGetIntersectionFor(nullptr, point) + groundOffset;
        if (nextGround > object.contactPosition.y) {
            object.velocity.x = object.velocity.z = 0;
            nextGround = ground;
        }
        object.velocity.y -= .032666668f;
        if (nextGround >= object.velocity.y + object.contactPosition.y)
            object.velocity.y = nextGround - object.contactPosition.y;
        const std::uint32_t axes = definition->flags;
        if ((object.reactionFlags & 2) && (axes & 0x380)) {
            Matrix4x4::SetPrePassToTempTransform();
            SetObjectEuler(object.rotation);
            const std::uint16_t angle = static_cast<std::uint16_t>(definition->launchYaw);
            if (axes & 0x80)
                Matrix4x4::RotateAroundX_Wrapper(angle);
            else if (axes & 0x100)
                Matrix4x4::RotateAroundY_Wrapper(angle);
            else
                Matrix4x4::RotateAroundZ_Wrapper(angle);
            Matrix4x4 matrix;
            Matrix4x4::GetTransformMatrix(&matrix);
            object.rotation = ExtractObjectEuler(matrix);
        }
    } else {
        if (object.reactionFlags & 0x10) {
            const std::uint32_t axes = definition->flags;
            if (axes & 0x40)
                vfConvertToStageObjectHitboxSpace(owner, object.velocity);
            object.velocity.x *= .60000002f;
            object.velocity.z *= .60000002f;
            if (axes & 0xfc00)
                AlignStageObjectAxis(owner);
            probe.flags |= 0x10;
        }
        if (std::fabs(object.velocity.x) < .00000011920929f && std::fabs(object.velocity.z) < .00000011920929f &&
            std::fabs(object.reactionRotation.y) < .00000011920929f && std::fabs(object.reactionRotation.z) < .00000011920929f) {
            object.velocity.y = 0;
            if (ground <= object.contactPosition.y)
                return;
            object.contactPosition.y = ground;
            probe.flags |= 0x10;
            return;
        }
        object.velocity.y = ground - object.contactPosition.y;
        if (object.motionFlags & 0x1000000)
            object.velocity.x = object.velocity.z = 0;
        else {
            object.velocity.x *= .92000002f;
            object.velocity.z *= .92000002f;
            Vector3 point{object.contactPosition.x + object.velocity.x, object.contactPosition.y + 10, object.contactPosition.z + object.velocity.z};
            if (flddGetIntersectionFor(nullptr, point) + groundOffset > ground)
                object.velocity.x = object.velocity.z = 0;
        }
    }
    object.contactPosition.x += object.velocity.x;
    object.contactPosition.y += object.velocity.y;
    object.contactPosition.z += object.velocity.z;
    if (std::fabs(object.velocity.x) < .0099999998f && std::fabs(object.velocity.z) < .0099999998f)
        object.velocity.x = object.velocity.z = 0;
    probe.flags |= 0x10;
}

// @verified - p
void CalculateStageObjectPoint(VOBJ_t *owner, StageObjectProbe &probe) {
    FightStageObjectRuntime &object = owner->object;
    const FightStageObjectDefinition *const definition = object.definition;
    const Vector3 offset = definition->positionOffset;
    object.contactPosition.x += object.reactionRotation.y;
    object.contactPosition.z += object.reactionRotation.z;
    const float ground = (flddGetIntersectionFor(nullptr, object.position) + definition->groundOffset) + offset.y;
    const Vector3 velocity = object.velocity;
    object.contactPosition.x += velocity.x;
    object.contactPosition.y += velocity.y;
    object.contactPosition.z += velocity.z;
    object.position.x += velocity.x;
    object.position.y += velocity.y;
    object.position.z += velocity.z;
    if (velocity.y == 0) {
        object.velocity.x *= .60000002f;
        object.velocity.z *= .60000002f;
        if (object.yaw) {
            const std::int16_t first = static_cast<std::int16_t>((object.rotation.z & 0xc000) - object.rotation.z);
            const std::int16_t second = static_cast<std::int16_t>(first + 0x4000);
            object.yaw = (std::abs(std::int32_t(first)) >= std::abs(std::int32_t(second)) ? second : first) / 2;
            if (std::fabs(float(object.yaw) / 182.04445f) < 2.5f)
                object.yaw = 0;
        }
    } else if (ground < object.position.y) {
        object.velocity.y -= .032666668f;
        const std::int16_t yaw = static_cast<std::int16_t>(static_cast<std::int32_t>(((float(object.yaw) / 182.04445f) * .94999999f) * 182.0f));
        if (std::fabs(float(yaw)) > 256)
            object.yaw = yaw;
    } else {
        vfClAicaReact(object.contactPosition, definition->groundSound);
        object.velocity.x *= .80000001f;
        object.velocity.z *= .80000001f;
        object.velocity.y *= -.5f;
        if (std::fabs(object.velocity.y) <= .065333337f)
            object.velocity = {};
    }
    if (ground >= object.position.y) {
        object.contactPosition.y += ground - object.position.y;
        object.position.y = ground;
    }
    if (object.yaw) {
        Matrix4x4::SetPrePassToTempTransform();
        SetObjectEuler(object.rotation);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(object.yaw));
        Matrix4x4 matrix;
        Matrix4x4::GetTransformMatrix(&matrix);
        ExtractObjectPlanarEuler(object, matrix);
        vfRotateStageObjectAboutOffset(owner, offset);
    }
    probe.flags |= 0x10;
}
}

// Allocate the broken-model tracks before changing display or state.
// @verified - p
void vfBreakStageObject(VOBJ_t *owner) {
    if (vfAllocateStageObjectMotionTracks(owner))
        return;
    FightStageObjectRuntime &object = owner->object;
    object.reactionFlags &= 0xffffff9d;
    object.state = 1;
    owner->motionFrame = object.definition->motionFirstFrame;
    const std::uint32_t sound = object.definition->breakSound;
    if (sound)
        vfClAicaReact(object.contactPosition, sound);
    if (object.reactionFlags & 1)
        vfStageObjectReset(static_cast<std::uint8_t>(object.index), nullptr);
    Character *const broken = owner->object.brokenModel;
    EV_SetCharDisplay(broken, 3, 1);
    EV_SetCharDisplay(object.controlCharacter, 3, 0);
    object.model = broken;
    shenApplyTranslationToAll(GetCharacterNode(object.model), object.contactPosition);
    SetCharacterWorldRotation(object.model, object.rotation);
    EmitFightSmoke(5, reinterpret_cast<VFActor *>(object.model));
    owner->object.rotationImpulse = {};
    object.reactionFlags &= 0xffffff9f;
    object.contactSlotMask = 0;
    object.velocity.x = object.velocity.z = 0;
    if (object.definition->index == 8) {
        if (HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(10, 1)) {
            if (VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task)))
                game->reserved2c |= 2U;
        }
    }
}

namespace {
bool ObjectContactAdmitted(const FightStageObjectRuntime &object) {
    const std::uint32_t mask = object.definition->flags;
    return (mask & 0x2U) || (object.state && (mask & 0x4U));
}
bool ObjectActorCanBreak(const FightStageObjectRuntime &object, const VFActor &actor) {
    return (object.motionFlags & 0x20) || ((object.motionFlags & 0x10) && ObjectHasHitSpeed(object.velocity)) ||
        ((object.motionFlags & 0x200) && ((actor.hitStateFlags & 0x80000) || (actor.activeMotionFlags & 0x2000) || (actor.hitStateFlags & 0x4010) == 0x4010));
}
void ClearObjectRotationImpulse(VOBJ_t *owner) {
    owner->object.reactionFlags &= 0xffffff9d;
    owner->object.rotationImpulse = {};
}
void ClearObjectPlanarCorrection(FightStageObjectRuntime &object) {
    object.reactionRotation.y = object.reactionRotation.z = 0;
}
// @verified - p
void ProbeStageObjectWorld(VOBJ_t *owner, StageObjectProbe &probe) {
    const std::int32_t count = owner->object.definition->hitboxCount;
    for (std::int32_t i = 0; i < count; ++i) {
        if (!ObjectHitboxAdmitted(owner, i))
            continue;
        const Vector3 point = owner->object.hitboxes[i].position;
        const Vector3 next{point.x + owner->object.velocity.x, point.y, point.z + owner->object.velocity.z};
        if (GlobalRayIntersectionTest(point, next, owner->object.hitboxes[i].radius)) {
            probe.flags |= 2;
            return;
        }
    }
}
// @verified - p
void ProbeStageObjectPair(VOBJ_t *owner, StageObjectProbe &probe) {
    for (VOBJ_t *other = g_fightStageObjectRoot; other; other = other->next) {
        const FightStageObjectRuntime &object = other->object;
        const std::uint32_t mask = object.definition->flags;
        if (!(mask & (object.state ? 0x4U : 0x2U)) || other == owner)
            continue;
        if (ObjectDistanceSquared(object.contactPosition, owner->object.contactPosition) > g_fightStageObjectMaximumDistance)
            continue;
        const std::int32_t otherCount = object.definition->hitboxCount;
        const std::int32_t ownCount = owner->object.definition->hitboxCount;
        for (std::int32_t i = 0; i < otherCount; ++i) {
            if (!ObjectHitboxAdmitted(other, i) || object.definition->hitboxes[i].kind != 1)
                continue;
            const Vector3 point = other->object.hitboxes[i].position;
            for (std::int32_t j = 0; j < ownCount; ++j) {
                if (!ObjectHitboxAdmitted(owner, j) || owner->object.definition->hitboxes[j].kind != 1)
                    continue;
                Vector3 moved = owner->object.hitboxes[j].position;
                moved.x += owner->object.velocity.x;
                moved.y += owner->object.velocity.y;
                moved.z += owner->object.velocity.z;
                const float radius = owner->object.definition->hitboxes[j].radius + object.definition->hitboxes[i].radius;
                if (ObjectDistanceSquared(point, moved) <= radius * radius) {
                    probe.flags |= 8;
                    probe.object = other;
                    return;
                }
            }
        }
    }
}
// @verified - p
void ProbeStageObjectActor(VOBJ_t *owner, StageObjectProbe &probe, VFMA_t *game) {
    for (std::size_t i = 0; i != 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const actor = game->actors[i];
        if ((owner->object.reactionFlags & 1) && owner->object.reactingCharacter == actor->character)
            continue;
        if (vfStageObjectOverlapsActor(owner, actor, 1)) {
            probe.flags |= 4;
            probe.actor = actor;
            return;
        }
    }
}

// @verified - p
void ResolveStageObjectCollisionResponse(VOBJ_t *owner, VFMA_t *game, StageObjectProbe &probe) {
    FightStageObjectRuntime &object = owner->object;
    if (!ObjectContactAdmitted(object))
        return;
    ProbeStageObjectWorld(owner, probe);
    if (probe.flags & 3) {
        if ((object.motionFlags & 0x100) && ((object.motionFlags & 0x20) || ((object.motionFlags & 0x10) && ObjectHasHitSpeed(object.velocity)))) {
            vfDeflectStageObjectYaw(owner, {-object.velocity.x, 0, -object.velocity.z});
            vfBreakStageObject(owner);
        }
        ClearObjectRotationImpulse(owner);
        object.contactPosition.x = probe.previousPosition.x;
        object.contactPosition.z = probe.previousPosition.z;
        object.velocity.x *= -.60000002f;
        object.velocity.z *= -.60000002f;
        ClearObjectPlanarCorrection(object);
        object.yaw = -object.yaw;
        return;
    }
    ProbeStageObjectPair(owner, probe);
    if (probe.flags & 8) {
        VOBJ_t *const other = probe.object;
        FightStageObjectRuntime &target = other->object;
        const float approach = (target.contactPosition.z - probe.previousPosition.z) * object.velocity.z +
            (target.contactPosition.x - probe.previousPosition.x) * object.velocity.x;
        if ((object.motionFlags & 0x80) &&
            ((object.motionFlags & 0x20) || ((object.motionFlags & 0x10) && ObjectHasHitSpeed(object.velocity) && approach > 0))) {
            vfDeflectStageObjectYaw(owner, {-object.velocity.x, 0, -object.velocity.z});
            vfBreakStageObject(owner);
        }
        ClearObjectRotationImpulse(owner);
        object.contactPosition.x = probe.previousPosition.x;
        object.contactPosition.z = probe.previousPosition.z;
        const std::uint32_t targetFlags = target.motionFlags;
        if ((targetFlags & 0x80) && ((targetFlags & 0x20) || ((targetFlags & 0x10) && ObjectHasHitSpeed(object.velocity)))) {
            vfDeflectStageObjectYaw(other, {object.velocity.x, 0, object.velocity.z});
            vfBreakStageObject(other);
            ClearObjectRotationImpulse(other);
            ClearObjectPlanarCorrection(target);
            ClearObjectPlanarCorrection(object);
            object.velocity.x *= .5f;
            object.velocity.z *= .5f;
        } else {
            ClearObjectRotationImpulse(other);
            if ((targetFlags & 1) && !(target.reactionFlags & 1)) {
                target.velocity.x += object.velocity.x;
                target.velocity.z += object.velocity.z;
                target.reactionRotation.y += object.reactionRotation.y;
                target.reactionRotation.z += object.reactionRotation.z;
                if (target.definition->flags & 0x40U) {
                    vfConvertToStageObjectHitboxSpace(other, target.velocity);
                    Vector3 correction{target.reactionRotation.y, 0, target.reactionRotation.z};
                    vfConvertToStageObjectHitboxSpace(other, correction);
                    target.reactionRotation.y = correction.x;
                    target.reactionRotation.z = correction.z;
                }
            }
            ClearObjectPlanarCorrection(object);
            object.velocity.x = object.velocity.z = 0;
        }
        return;
    }
    ProbeStageObjectActor(owner, probe, game);
    if (probe.flags & 4) {
        VFActor *const actor = probe.actor;
        vfclFilterObjectHit(actor, owner);
        if (!object.state && (object.motionFlags & 0x40) && ObjectActorCanBreak(object, *actor)) {
            vfDeflectStageObjectYaw(owner, {-object.velocity.x, 0, -object.velocity.z});
            vfBreakStageObject(owner);
        }
        if (ObjectContactAdmitted(object)) {
            object.contactSlotMask |= std::uint64_t{1} << actor->groupIndex;
            actor->stateFlags |= 0x20;
        }
        ClearObjectRotationImpulse(owner);
        object.contactPosition.x = probe.previousPosition.x;
        object.contactPosition.z = probe.previousPosition.z;
        object.velocity.x *= -.5f;
        object.velocity.z *= -.5f;
    }
    ClearObjectPlanarCorrection(object);
}

// @verified - p
void ResolveStageObjectProbeContacts(VOBJ_t *owner, VFMA_t *game, StageObjectProbe &probe) {
    FightStageObjectRuntime &object = owner->object;
    if (object.reactionFlags & 1) {
        vfUpdateRotationAnglesFromStageObject(owner, game);
        probe.flags |= 0x10;
    } else if (object.definition->index == 7)
        CalculateStageObjectPoint(owner, probe);
    else
        UpdateStageObjectBody(owner, probe);
    if (!(object.reactionFlags & 1) && (probe.flags & 0x10))
        ResolveStageObjectCollisionResponse(owner, game, probe);
    Vector3 &point = object.definition->flags & 0x40000U ? object.position : object.contactPosition;
    const float ground = flddGetIntersectionFor(nullptr, point) + object.definition->groundOffset;
    const float difference = point.y - ground;
    if (difference < 0 || std::fabs(difference) < .00000011920929f) {
        if (object.reactionFlags & 8)
            object.reactionFlags &= 0xffffffef;
        else {
            const std::uint32_t sound = object.definition->groundSound;
            if (object.velocity.y < -.039200004f && sound)
                vfClAicaReact(object.contactPosition, sound);
            object.reactionFlags = (object.reactionFlags & 0xffffffe5) | 0x18;
        }
    } else
        object.reactionFlags &= 0xffffffe7;
}
}

// Current-pose contacts and break admission precede object integration.
// @verified - p
void vfclProcessCollisionAndSound(VFMA_t *game) {
    for (std::size_t i = 0; i != 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const actor = game->actors[i];
        for (VOBJ_t *owner = g_fightStageObjectRoot; owner; owner = owner->next) {
            FightStageObjectRuntime &object = owner->object;
            if (!ObjectContactAdmitted(object))
                continue;
            if ((object.reactionFlags & 1) && (object.reactingCharacter == actor->character || (actor->hitStateFlags & 0xc4000)))
                continue;
            if (!vfStageObjectOverlapsActor(owner, actor, 0))
                continue;
            vfclFilterObjectHit(actor, owner);
            if (!object.state && (object.motionFlags & 0x40)) {
                const float approach = (actor->contactPosition.z - object.contactPosition.z) * object.velocity.z +
                    (actor->contactPosition.x - object.contactPosition.x) * object.velocity.x;
                const bool breaks = (object.motionFlags & 0x20) || ((object.motionFlags & 0x10) && ObjectHasHitSpeed(object.velocity) && approach > 0) ||
                    ((object.motionFlags & 0x200) &&
                        ((actor->hitStateFlags & 0x80000) || (actor->activeMotionFlags & 0x2000) || (actor->hitStateFlags & 0x4010) == 0x4010));
                if (breaks) {
                    vfBreakStageObject(owner);
                    if (!(object.definition->flags & (object.state ? 0x4U : 0x2U)))
                        continue;
                }
            }
            object.contactSlotMask |= std::uint64_t{1} << actor->groupIndex;
            actor->stateFlags |= 0x20;
            vfResolveActorStageObjectPenetration(actor, owner);
            float pushX, pushZ;
            if (!(object.motionFlags & 1) || (object.reactionFlags & 1)) {
                pushX = -object.reactionRotation.y;
                pushZ = -object.reactionRotation.z;
                ClearObjectPlanarCorrection(object);
            } else {
                const float factor = std::fmax(0.0f, 1 - object.definition->responseCoefficient * .045000002f);
                pushX = -(object.reactionRotation.y * (1 - factor));
                pushZ = -(object.reactionRotation.z * (1 - factor));
                Vector3 correction{object.reactionRotation.y, 0, object.reactionRotation.z};
                if (object.definition->flags & 0x40U)
                    vfConvertToStageObjectHitboxSpace(owner, correction);
                object.reactionRotation.y = correction.x * factor;
                object.reactionRotation.z = correction.z * factor;
            }
            VFCL_t &collision = *actor->game->collision;
            const Vector3 &centre = collision.samples[actor->groupIndex].points[0];
            const Vector3 candidate{centre.x + pushX, centre.y, centre.z + pushZ};
            Vector3 result;
            if (RaycastInWorldGrid(result, candidate, .40000001f)) {
                vfclMoveEntityWithContactHistory(actor, game->collision->previousSamples[i].points[0].x - game->collision->samples[i].points[0].x,
                    game->collision->previousSamples[i].points[0].z - game->collision->samples[i].points[0].z);
                break;
            }
            vfclMoveEntityWithContactHistory(actor, pushX, pushZ);
        }
    }
}

// Publish model pose and runtime hitboxes only when a probe moved it.
// @verified - p
void vfclUpdateStageObjectHitboxesPhysics(VFMA_t *game) {
    for (VOBJ_t *owner = g_fightStageObjectRoot; owner; owner = owner->next) {
        FightStageObjectRuntime &object = owner->object;
        if (!(object.motionFlags & 0x700003))
            continue;
        const FightStageObjectDefinition *const definition = object.definition;
        StageObjectProbe probe{};
        probe.previousPosition = object.contactPosition;
        ResolveStageObjectProbeContacts(owner, game, probe);
        if (probe.flags & 0x10) {
            const Vector3 displacement{object.contactPosition.x - probe.previousPosition.x, object.contactPosition.y - probe.previousPosition.y,
                object.contactPosition.z - probe.previousPosition.z};
            if (std::fabs(displacement.x) >= .00000011920929f || std::fabs(displacement.z) >= .00000011920929f) {
                const std::uint32_t axes = definition->flags;
                if ((object.reactionFlags & 8) && !(axes & 0x20) && (axes & 0x40))
                    vfCreateStageObjectRotationMatrix(owner, displacement);
            } else if (std::fabs(displacement.y) < .00000011920929f) {
                owner->object.rotationImpulse = {};
                object.reactionFlags &= 0xffffff9f;
            }
            if ((object.reactionFlags & 0x60) && (!(definition->flags & 0x60U) || !(object.reactionFlags & 8)))
                vfRecalcStageObjectLocalEuler(owner);
            shenApplyTranslationToAll(GetCharacterNode(object.model), object.contactPosition);
            SetCharacterWorldRotation(object.model, object.rotation);
            vfTransformStageObjectHitboxes(owner);
        }
        vfUpdateStageObjectWorldPos(owner);
    }
}

// The first eligible fighter alone launches contacted stage objects.
// @verified - p
void vfclTriggerStageObjectHitReaction(VFMA_t *game) {
    VFActor *actor = nullptr;
    bool airborne = false;
    for (std::size_t i = 0; i != 6; ++i) {
        if (!game->characters[i])
            continue;
        VFActor *const candidate = game->actors[i];
        if (candidate->hitStateFlags & 0xc0000) {
            actor = candidate;
            airborne = true;
            break;
        }
        if (!(g_vf_hit_object_handled & (std::uint32_t{1} << candidate->groupIndex)) && (candidate->hitStateFlags & 0x100)) {
            actor = candidate;
            break;
        }
    }
    if (!actor)
        return;
    for (VOBJ_t *owner = g_fightStageObjectRoot; owner; owner = owner->next) {
        FightStageObjectRuntime &object = owner->object;
        if ((object.reactionFlags & 1) || !(object.motionFlags & 2) || !(object.contactSlotMask & (std::uint64_t{1} << actor->groupIndex)))
            continue;
        const std::uint16_t direction = FixedAtan2(actor->contactPosition.x - object.contactPosition.x, actor->contactPosition.z - object.contactPosition.z);
        const std::int16_t angle = static_cast<std::int16_t>(0x4000 - actor->temporaryFacingAngle + direction);
        if ((!airborne && angle < 0) || object.velocity.y > .1f)
            continue;
        if (object.velocity.y == 0)
            vfClAicaReact(object.contactPosition, object.definition->launchSound);
        float vertical = 0;
        const bool tilted = std::abs(object.rotation.x) >= 0x2000 || std::abs(object.rotation.z) >= 0x2000;
        if (object.definition->index == 7) {
            const std::uint8_t parameter = static_cast<std::uint8_t>(actor->hitParameter);
            vertical = tilted ? .15000001f : parameter <= 0x12 && (262682u & (1u << parameter)) ? .27000001f : .2f;
        }
        const std::uint16_t yaw = airborne ? direction : actor->temporaryFacingAngle;
        const float magnitude = .5f - object.definition->responseCoefficient * .0166f;
        FightStageObjectRuntime &bank = g_fightStageObjects[static_cast<std::uint8_t>(object.index)]->object;
        bank.velocity = {-sin_fixed16(static_cast<std::int16_t>(yaw)) * magnitude, vertical, -cos_fixed16(yaw) * magnitude};
        object.yaw = object.definition->launchYaw;
        if (object.definition->index == 7)
            object.yaw = tilted ? 1024 : 6144;
        if (object.definition->flags & 0x40U)
            vfConvertToStageObjectHitboxSpace(owner, object.velocity);
        object.reactingCharacter = actor->character;
    }
}


// @verified - p
void vfUpdateStageObjects(VFMA_t *game) {
    Matrix4x4_Push();
    for (VOBJ_t *owner = g_fightStageObjectRoot; owner; owner = owner->next)
        owner->object.contactSlotMask = 0;
    g_vf_hit_object_handled = 0;
    vfclProcessCollisionAndSound(game);
    vfclUpdateStageObjectHitboxesPhysics(game);
    vfclTriggerStageObjectHitReaction(game);
    vfStageObjectEvalHitboxKeyframes();
    Matrix4x4::Pop(1);
}

} // namespace shenmue::s2
