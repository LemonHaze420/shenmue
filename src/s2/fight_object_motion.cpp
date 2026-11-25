#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/fight_stage.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/math.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/model_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/fight.hpp"
#include "s2/character_pose_dc.hpp"
#include <cmath>
#include <cstring>
#else
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
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfActorMotionNullsub(CharacterNode *node);
std::int32_t vfStageObjectReturnZero(void *probe, std::byte *state);
std::byte *GetMapRetainedCharacterState();

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t g_vf_hit_object_handled;

struct StageObjectProbe {
    std::uint32_t flags;
    Vector3 previousPosition;
    VOBJ_t *object;
    VFActor *actor;
};

static_assert(sizeof(StageObjectProbe) == 32);

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

Vector3i ExtractObjectEuler(const Matrix4x4 &matrix);
void ExtractObjectPlanarEuler(FightStageObjectRuntime &object, const Matrix4x4 &matrix);
VFActor *ObjectActor(VFMA_t *game, Character *character);
void ApplyObjectAxisRotation(const Vector3 &from, const Vector3 &to);
void AlignStageObjectAxis(VOBJ_t *owner);
void UpdateStageObjectBody(VOBJ_t *owner, StageObjectProbe &probe);
void CalculateStageObjectPoint(VOBJ_t *owner, StageObjectProbe &probe);
bool ObjectActorCanBreak(const FightStageObjectRuntime &object, const VFActor &actor);
void ClearObjectRotationImpulse(VOBJ_t *owner);
void ProbeStageObjectWorld(VOBJ_t *owner, StageObjectProbe &probe);
void ProbeStageObjectPair(VOBJ_t *owner, StageObjectProbe &probe);
void ProbeStageObjectActor(VOBJ_t *owner, StageObjectProbe &probe, VFMA_t *game);
void ResolveStageObjectCollisionResponse(VOBJ_t *owner, VFMA_t *game, StageObjectProbe &probe);
void ResolveStageObjectProbeContacts(VOBJ_t *owner, VFMA_t *game, StageObjectProbe &probe);

} // namespace

std::int32_t vfAllocateStageObjectMotionTracks(VOBJ_t *object);
void vfStageObjectEvalHitboxKeyframes();
void vfConvertToStageObjectHitboxSpace(VOBJ_t *object, Vector3 &point);
void vfRecalcStageObjectLocalEuler(VOBJ_t *object);
void vfCreateStageObjectRotationMatrix(VOBJ_t *object, const Vector3 &displacement);
void vfUpdateStageObjectWorldPos(VOBJ_t *object);
void vfClAicaReact(const Vector3 &position, std::uint32_t flags);
void vfDeflectStageObjectYaw(VOBJ_t *owner, const Vector3 &direction);
void vfRotateStageObjectAboutOffset(VOBJ_t *owner, const Vector3 &offset);
bool vfStageObjectOverlapsActor(VOBJ_t *owner, VFActor *actor, std::int32_t withVelocity);
void vfResolveActorStageObjectPenetration(VFActor *actor, VOBJ_t *owner);
void vfProcessStageObjectHitOutcome(VOBJ_t *owner, VFActor *self, std::int8_t guardState, std::int16_t hitParameter, std::int8_t hitDamage);
void vfclFilterObjectHit(VFActor *actor, VOBJ_t *owner);
void vfUpdateRotationAnglesFromStageObject(VOBJ_t *owner, VFMA_t *game);
void vfBreakStageObject(VOBJ_t *owner);
void vfclProcessCollisionAndSound(VFMA_t *game);
void vfclUpdateStageObjectHitboxesPhysics(VFMA_t *game);
void vfclTriggerStageObjectHitReaction(VFMA_t *game);
void vfUpdateStageObjects(VFMA_t *game);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfClAicaReact(std::uint32_t flags, const Vector3 *position);

struct ObjectMotionKey {
    std::int32_t frame;
    float value;
};

struct ObjectMotionCurve {
    const ObjectMotionKey *keys;
    std::int16_t cursor, count;
};

struct ObjectMotionTrack {
    MT7Node *node;
    ObjectMotionCurve curves[6];
};

struct ObjectMotionChannel {
    std::int32_t keyCount;
};

typedef char ObjectMotionTrackSizeCheck[sizeof(ObjectMotionTrack) == 0x34 ? 1 : -1];

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

template <class T> inline T &ObjectActorValue(void *owner, std::size_t offset) {
    return *reinterpret_cast<T *>(static_cast<std::byte *>(owner) + offset);
}

struct ObjectActorSamples {
    std::byte retained00[0x74];
    Vector3 samples[10];
};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint64_t &ObjectContactMask(FightStageObjectRuntime &object) {
    return *reinterpret_cast<std::uint64_t *>(&object.contactState);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 ObjectVector(float x, float y, float z) {
    Vector3 v = {x, y, z};
    return v;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3i ObjectRotation(std::int32_t x, std::int32_t y, std::int32_t z) {
    Vector3i v = {x, y, z};
    return v;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t ObjectIntegerMagnitude(std::int32_t value) {
    return value < 0 ? -value : value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float ObjectMaximum(float first, float second) {
    return first > second ? first : second;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfclApplyHitResolution(FightActorRuntimePrefix *source, FightActorRuntimePrefix *target);

void vfclMoveEntityWithContactHistory(FightActorRuntimePrefix *actor, float x, float z);

void vfConvertToStageObjectHitboxSpace(VOBJ_t *object, Vector3 &point);

void vfDeflectStageObjectYaw(VOBJ_t *owner, const Vector3 &direction);

void vfBreakStageObject(VOBJ_t *owner);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void SetObjectEuler(const Vector3i &angles) {
    Matrix4x4::SetViewMatrixFromEuler(angles.x, angles.y, angles.z);
}

} // namespace

#else

namespace {

void SetObjectEuler(const Vector3i &angles) {
    Matrix4x4::SetViewMatrixFromEuler(angles.y, angles.x, angles.z);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3i ExtractObjectEuler(const Matrix4x4 &matrix) {
    Vector3i angles;
    ExtractMatrixEulerAnglesFixed(&matrix, &angles);
    return angles;
}

} // namespace

#else

namespace {

// Native Matrix4x4::ExtractEulerAnglesFixed: this fixed-angle extractor retains the native gimbal-lock branches.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ExtractObjectPlanarEuler(const Matrix4x4 &matrix, Vector3i &rotation) {
    rotation.z = 0;
    float magnitude = matrix.rows[0].x * matrix.rows[1].z - matrix.rows[0].z * matrix.rows[1].x;
    if (magnitude < 0.0f) {
        magnitude = std::sqrt(matrix.rows[1].x * matrix.rows[1].x + matrix.rows[1].z * matrix.rows[1].z);
        magnitude = -magnitude;
    } else
        magnitude = std::sqrt(matrix.rows[1].x * matrix.rows[1].x + matrix.rows[1].z * matrix.rows[1].z);
    rotation.x = dc::sdk::ArcTan2Fixed(magnitude, matrix.rows[1].y);
    rotation.y = -dc::sdk::ArcTan2Fixed(matrix.rows[0].z, matrix.rows[0].x);
}

} // namespace

#else

namespace {

void ExtractObjectPlanarEuler(FightStageObjectRuntime &object, const Matrix4x4 &matrix) {
    const float z = matrix.rows[1].z, x = matrix.rows[1].x;
    float magnitude = std::sqrt(x * x + z * z);
    if (z * matrix.rows[0].x - x * matrix.rows[0].z < 0)
        magnitude = -magnitude;
    object.rotation = {FixedAtan2(magnitude, matrix.rows[1].y), -std::int32_t(FixedAtan2(matrix.rows[0].z, matrix.rows[0].x)), 0};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint32_t g_vf_hit_object_handled;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

FightActorRuntimePrefix *ObjectActor(Character *character, FightRuntimePrefix *game) {
    if (!character)
        return 0;
    for (std::int32_t i = 0; i < 6; ++i)
        if (game->characters[i] == character)
            return game->actors[i];
    return 0;
}

} // namespace

#else

namespace {

// @verified - p
VFActor *ObjectActor(VFMA_t *game, Character *character) {
    if (!character)
        return nullptr;
    for (std::size_t i = 0; i != 6; ++i)
        if (game->characters[i] == character)
            return game->actors[i];
    return nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool ObjectHasHitSpeed(const Vector3 &velocity) {
    return std::fabs(velocity.x) > .02f || std::fabs(velocity.y) > .02f || std::fabs(velocity.z) > .02f;
}

} // namespace

#else

namespace {

bool ObjectHasHitSpeed(const Vector3 &velocity) {
    return std::fabs(velocity.x) > .02f || std::fabs(velocity.y) > .02f || std::fabs(velocity.z) > .02f;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void FindStageObjectKeyframeSpan(const std::int32_t *channel, std::int32_t frame, ObjectMotionCurve *curve) {
    const std::int32_t count = *channel++;
    const ObjectMotionKey *const keys = reinterpret_cast<const ObjectMotionKey *>(channel);
    std::int32_t cursor = 1;
    for (; cursor < count; ++cursor)
        if (frame < keys[cursor].frame)
            break;
    curve->keys = keys;
    curve->cursor = static_cast<std::int16_t>(cursor);
    curve->count = static_cast<std::int16_t>(count);
}

std::int32_t vfAllocateStageObjectMotionTracks(VOBJ_t *object) {
    std::int32_t i;
    const FightStageObjectDefinition *const definition = object->object.definition;
    ObjectMotionTrack *track = static_cast<ObjectMotionTrack *>(
        HLib::AllocateTaskData(definition->motionTrackCount * static_cast<std::int32_t>(sizeof(ObjectMotionTrack)), TASK_VOBJ));
    if (!track)
        return 1;
    object->motionTracks = track;
    const ObjectMotionNode *node;
    MT7Node *const model = object->object.brokenModel->model;
    node = definition->motionNodes;
    const ObjectMotionRecord *record = definition->motionKeys;
    for (i = 0; i < definition->motionTrackCount; ++i, ++node, ++track) {
        track->node = FindMT7Node(model, node->index);
        const std::int32_t *channel = reinterpret_cast<const std::int32_t *>(record + 1);
        FindStageObjectKeyframeSpan(channel, definition->motionFirstFrame, &track->curves[0]);
        channel = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const ObjectMotionKey *>(channel) + *channel) + 1;
        FindStageObjectKeyframeSpan(channel, definition->motionFirstFrame, &track->curves[1]);
        channel = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const ObjectMotionKey *>(channel) + *channel) + 1;
        FindStageObjectKeyframeSpan(channel, definition->motionFirstFrame, &track->curves[2]);
        channel = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const ObjectMotionKey *>(channel) + *channel) + 1;
        FindStageObjectKeyframeSpan(channel, definition->motionFirstFrame, &track->curves[3]);
        channel = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const ObjectMotionKey *>(channel) + *channel) + 1;
        FindStageObjectKeyframeSpan(channel, definition->motionFirstFrame, &track->curves[4]);
        channel = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const ObjectMotionKey *>(channel) + *channel) + 1;
        FindStageObjectKeyframeSpan(channel, definition->motionFirstFrame, &track->curves[5]);
        record = reinterpret_cast<const ObjectMotionRecord *>(reinterpret_cast<const std::byte *>(record) + record->byteSize);
    }
    return 0;
}

#else

// Native AllocateVOBJ_3: tracks borrow packed source keys and the selected authored MT7 nodes.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float EvalStageObjectHitboxChannel(ObjectMotionCurve *curve, std::int32_t frame) {
    const ObjectMotionKey *const keys = curve->keys;
    std::int16_t cursor = curve->cursor;
    for (; cursor < curve->count; ++cursor)
        if (frame < keys[cursor].frame)
            break;
    curve->cursor = cursor;
    if (cursor == curve->count)
        return keys[curve->count - 1].value;
    frame -= keys[cursor - 1].frame;
    return keys[cursor - 1].value + float(frame) * (keys[cursor].value - keys[cursor - 1].value) / float(keys[cursor].frame - keys[cursor - 1].frame);
}

void vfStageObjectEvalHitboxKeyframes() {
    for (VOBJ_t *object = g_fightStageObjectRoot; object; object = object->next) {
        if (object->object.state == 1) {
            std::int32_t i;
            const FightStageObjectDefinition *definition;
            const std::int32_t frame = object->motionFrame;
            definition = object->object.definition;
            ObjectMotionTrack *track = object->motionTracks;
            for (i = 0; i < definition->motionTrackCount; ++i, ++track) {
                MT7Node *const node = track->node;
                Vector3 *const position = &node->position;
                Vector3i *const rotation = &node->rotation;
                position->x = EvalStageObjectHitboxChannel(&track->curves[0], frame) * 10;
                position->y = EvalStageObjectHitboxChannel(&track->curves[1], frame) * 10;
                position->z = EvalStageObjectHitboxChannel(&track->curves[2], frame) * 10;
                rotation->x = static_cast<std::int32_t>(EvalStageObjectHitboxChannel(&track->curves[3], frame) * 182.04445f);
                rotation->y = static_cast<std::int32_t>(EvalStageObjectHitboxChannel(&track->curves[4], frame) * 182.04445f);
                rotation->z = static_cast<std::int32_t>(EvalStageObjectHitboxChannel(&track->curves[5], frame) * 182.04445f);
            }
            object->motionFrame += definition->motionStep;
            if (object->motionFrame >= definition->motionLastFrame) {
                object->motionFrame = definition->motionLastFrame;
                object->object.state = 2;
                HLib::FreeTaskData(object->motionTracks);
                object->motionTracks = 0;
            }
        }
    }
}

#else

// Six independent native cursors interpolate before frame advancement.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfConvertToStageObjectHitboxSpace(VOBJ_t *object, Vector3 &point) {
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::SetViewMatrixFromEuler(object->object.rotation.x, object->object.rotation.y, object->object.rotation.z);
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::AffineProjectionSwap();
    Vector3 local;
    Matrix4x4::Transform(point, local);
    PopCurrentMatrix(1);
    local.x = 0;
    Matrix4x4::Transform(local, point);
}

#else

// Native ConvertToHitboxSpace: project into the authored object's YZ plane, then return to world space.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfRecalcStageObjectLocalEuler(VOBJ_t *object) {
    matrix_stack::UnitCurrentMatrix();
    if (object->object.reactionFlags & 0x20) {
        Matrix4x4::SetViewMatrixFromEuler(object->object.rotationImpulse.x, object->object.rotationImpulse.y, object->object.rotationImpulse.z);
        Matrix4x4::SetViewMatrixFromEuler(object->object.rotation.x, object->object.rotation.y, object->object.rotation.z);
    } else {
        Matrix4x4::SetViewMatrixFromEuler(object->object.rotation.x, object->object.rotation.y, object->object.rotation.z);
        Matrix4x4::SetViewMatrixFromEuler(object->object.rotationImpulse.x, object->object.rotationImpulse.y, object->object.rotationImpulse.z);
    }
    Matrix4x4 matrix;
    matrix_stack::GetMatrix(&matrix);
    ExtractMatrixEulerAnglesFixed(&matrix, &object->object.rotation);
}

#else

// Native vfRecalcLocalEulerFromWorld_SAMEASS1: the native impulse flag chooses the multiplication order.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfCreateStageObjectRollMatrix(VOBJ_t *object, const Vector3 &displacement) {
    Vector3i impulse;
    Vector3 axis;
    Vector3 rotated;
    Matrix4x4 matrix;
    const float length = am2Vector3Length(&displacement);
    std::int32_t rotation = static_cast<std::int32_t>(float(object->object.definition->rotationSpeed) * length);
    rotation &= 0xffff;
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::RotateAroundY_Wrapper(dc::sdk::ArcTan2Fixed(displacement.x, displacement.z) & 0xffff);
    Matrix4x4::RotateAroundX_Wrapper(rotation);
    axis.x = 0.0f;
    axis.y = 1.0f;
    axis.z = 0.0f;
    Matrix4x4::Transform(axis, rotated);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4_BuildRotationMatrix(axis, rotated);
    matrix_stack::GetMatrix(&matrix);
    ExtractMatrixEulerAnglesFixed(&matrix, &impulse);
    Matrix4x4::SetViewMatrixFromEuler(object->object.rotation.x, object->object.rotation.y, object->object.rotation.z);
    matrix_stack::GetMatrix(&matrix);
    ExtractMatrixEulerAnglesFixed(&matrix, &object->object.rotation);
    object->object.rotationImpulse.x = impulse.x;
    object->object.rotationImpulse.y = impulse.y;
    object->object.rotationImpulse.z = impulse.z;
    object->object.reactionFlags |= 0x20;
}

void vfCreateStageObjectRotationMatrix(VOBJ_t *object, const Vector3 &displacement) {
    Matrix4x4 matrix;
    Vector3 local;
    const float length = am2Vector3Length(&displacement);
    std::int32_t rotation = static_cast<std::int32_t>(length * float(object->object.definition->rotationSpeed));
    rotation = static_cast<std::uint16_t>(rotation);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-object->object.rotation.y);
    Matrix4x4::Transform(displacement, local);
    if (local.z < 0)
        rotation = -rotation;
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::SetViewMatrixFromEuler(object->object.rotation.x, object->object.rotation.y, object->object.rotation.z);
    Matrix4x4::RotateAroundX_Wrapper(rotation);
    object->object.rotationImpulse.x = rotation;
    object->object.rotationImpulse.y = 0;
    object->object.rotationImpulse.z = 0;
    object->object.reactionFlags |= 0x40;
    matrix_stack::GetMatrix(&matrix);
    ExtractObjectPlanarEuler(matrix, object->object.rotation);
}

#else

// Native vfCreateRotationMatrix: rolling distance produces a fixed-angle local X impulse.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfUpdateStageObjectWorldPos(VOBJ_t *object) {
    Vector3 scale;
    std::uint16_t *ticks = object->scaleTicks;
    if (ticks[0] || ticks[1] || ticks[2]) {
        GetCharacterScale(object->object.model, &scale);
        if (ticks[0]) {
            scale.x += (object->scaleTarget.x - scale.x) / ticks[0];
            if (--ticks[0] == 0)
                object->scaleTarget.x = 0;
        }
        if (ticks[1]) {
            scale.y += (object->scaleTarget.y - scale.y) / ticks[1];
            if (--ticks[1] == 0)
                object->scaleTarget.y = 0;
        }
        if (ticks[2]) {
            scale.z += (object->scaleTarget.z - scale.z) / ticks[2];
            if (--ticks[2] == 0)
                object->scaleTarget.z = 0;
        }
        SetCharacterScale(object->object.model, &scale);
    }
}

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native ground-category flags select the existing SGTS/AICA sound owner.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfDeflectStageObjectYaw(VOBJ_t *owner, const Vector3 &direction) {
    if ((direction.x < 0.0f ? -direction.x : direction.x) < .00000011920929f && (direction.z < 0.0f ? -direction.z : direction.z) < .00000011920929f)
        return;
    if (!(owner->object.definition->flags & 0x8U))
        return;
    std::int32_t quadrant = dc::sdk::ArcTan2Fixed(direction.x, direction.z);
    const std::int32_t facing = owner->object.rotation.y - 0x8000;
    quadrant = (quadrant - facing) & 0xffff;
    if (quadrant <= 0x2000)
        return;
    if (quadrant <= 0x6000)
        owner->object.rotation.y += 0x4000;
    else if (quadrant <= 0xa000)
        owner->object.rotation.y += 0x8000;
    else if (quadrant <= 0xe000)
        owner->object.rotation.y += 0xc000;
    owner->object.rotation.y &= 0xffff;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfRotateStageObjectAboutOffset(VOBJ_t *owner, const Vector3 &offset) {
    Matrix4x4 matrix;
    Vector3 translation;
    const FightStageObjectDefinition *const definition = owner->object.definition;
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::SetViewMatrixFromEuler(owner->object.rotation.x, owner->object.rotation.y, owner->object.rotation.z);
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    matrix_stack::GetTranslation(&translation);
    owner->object.position.x = owner->object.contactPosition.x + translation.x;
    owner->object.position.y = owner->object.contactPosition.y + translation.y;
    owner->object.position.z = owner->object.contactPosition.z + translation.z;
    if (definition->flags & 0x80)
        Matrix4x4::RotateAroundX_Wrapper(owner->object.yaw);
    else if (definition->flags & 0x100)
        Matrix4x4::RotateAroundY_Wrapper(owner->object.yaw);
    else
        Matrix4x4::RotateAroundZ_Wrapper(owner->object.yaw);
    Matrix4x4::preTranslate(-offset.x, -offset.y, -offset.z);
    matrix_stack::GetTranslation(&translation);
    owner->object.contactPosition.x += translation.x;
    owner->object.contactPosition.y += translation.y;
    owner->object.contactPosition.z += translation.z;
    matrix_stack::GetMatrix(&matrix);
    ExtractMatrixEulerAnglesFixed(&matrix, &owner->object.rotation);
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool AreStageObjectsNear(const VOBJ_t *first, const VOBJ_t *second) {
    Vector3 delta;
    delta.x = first->object.contactPosition.x - second->object.contactPosition.x;
    delta.y = first->object.contactPosition.y - second->object.contactPosition.y;
    delta.z = first->object.contactPosition.z - second->object.contactPosition.z;
    if ((delta.x * delta.x + delta.y * delta.y) + delta.z * delta.z > g_fightStageObjectMaximumDistance)
        return false;
    return true;
}

bool AreStageObjectSpheresTouching(const Vector3 &first, const Vector3 &second, float firstRadius, float secondRadius) {
    Vector3 delta;
    delta.x = first.x - second.x;
    delta.y = first.y - second.y;
    delta.z = first.z - second.z;
    firstRadius += secondRadius;
    if ((delta.x * delta.x + delta.y * delta.y) + delta.z * delta.z > firstRadius * firstRadius)
        return false;
    return true;
}

namespace {

float ObjectDistanceSquared(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x, y = first.y - second.y, z = first.z - second.z;
    return (y * y + x * x) + z * z;
}

} // namespace

#else

namespace {

float ObjectDistanceSquared(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x, y = first.y - second.y, z = first.z - second.z;
    return (y * y + x * x) + z * z;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t ObjectHitboxAdmitted(const VOBJ_t *object, std::size_t index) {
    const FightStageObjectDefinition *const definition = object->object.definition;
    std::uint16_t state = 0;
    if (static_cast<std::int32_t>(index) >= definition->hitboxCount)
        return 0;
    std::int32_t closed = object->object.state == 0;
    const std::uint16_t kind = static_cast<std::uint16_t>(definition->hitboxes[index].kind);
    if (kind == 1)
        goto readState;
    if (kind != 2)
        goto stateReady;
readState:
    state = static_cast<std::uint16_t>(definition->hitboxes[index].state);
stateReady:
    if (state & 0x8000U)
        closed ^= 1;
    return closed;
}

} // namespace

#else

namespace {

bool ObjectHitboxAdmitted(const VOBJ_t *object, std::size_t index) {
    const FightStageObjectDefinition *const definition = object->object.definition;
    const std::int16_t kind = definition->hitboxes[index].kind;
    const std::int32_t state = kind == 1 || kind == 2 ? definition->hitboxes[index].state : 0;
    return state < 0 ? object->object.state != 0 : object->object.state == 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native Matrix4x4::CalculateMatrixEulerAngles: local authored spheres become runtime world-space probes.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool vfStageObjectOverlapsActor(VOBJ_t *owner, FightActorRuntimePrefix *actor, std::int32_t withVelocity) {
    Vector3 point;
    Vector3 velocity;
    const FightStageObjectDefinition *definition;
    const Vector3 *centre;
    std::int32_t i;
    std::int32_t bone;
    if (!IsActorBoneNearStageObject(actor, owner))
        return false;
    definition = owner->object.definition;
    if (withVelocity) {
        velocity = owner->object.velocity;
    } else {
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = 0;
    }
    centre = &actor->game->collision->samples[actor->groupIndex].points[0];
    for (i = 0; i < definition->hitboxCount; i++) {
        if (!ObjectHitboxAdmitted(owner, i))
            continue;
        if (definition->hitboxes[i].kind != 1)
            continue;
        point.x = owner->object.hitboxes[i].position.x + velocity.x;
        point.y = owner->object.hitboxes[i].position.y + velocity.y;
        point.z = owner->object.hitboxes[i].position.z + velocity.z;
        if (AreStageObjectSpheresTouching(*centre, point, pushShapes[0].radius, definition->hitboxes[i].radius))
            owner->object.contactSlotMask |= 1 << actor->groupIndex;
        const Vector3 *samples = reinterpret_cast<ObjectActorSamples *>(actor)->samples;
        for (bone = 0; bone < 10; bone++) {
            if (AreStageObjectSpheresTouching(
                    samples[bone], point, ObjectActorValue<const VFCollisionShape *>(actor, 0x9a8)[bone].radius, definition->hitboxes[i].radius))
                return true;
        }
    }
    return false;
}

#else

// Broad-phase and motion exclusions precede native per-bone overlaps.
// @verified - p
bool vfStageObjectOverlapsActor(VOBJ_t *owner, VFActor *actor, std::int32_t withVelocity) {
    const Vector3 &centre = actor->game->collision->samples[actor->groupIndex].points[0];
    if (ObjectDistanceSquared(centre, owner->object.contactPosition) > g_fightStageObjectMaximumDistance)
        return false;
    const std::int32_t motion = mtwkGetAnimationId(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(actor->game->characters[actor->groupIndex]), MotionRegion::LowerBody) & 0xfff;
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ComputeActorStageObjectPenetration(FightActorRuntimePrefix *actor, VOBJ_t *owner, FightStageObjectWorldHitbox *hitbox) {
    FightStageObjectRuntime &object = owner->object;
    const Vector3 &centre = actor->game->collision->samples[actor->groupIndex].points[0];
    float deepest = std::sqrt((object.reactionRotation.y * object.reactionRotation.y + object.reactionRotation.z * object.reactionRotation.z));
    for (std::int32_t i = 0; i < 10; ++i) {
        const Vector3 &sample = reinterpret_cast<ObjectActorSamples *>(actor)->samples[i];
        float x = hitbox->position.x - sample.x;
        const float y = hitbox->position.y - sample.y;
        float z = hitbox->position.z - sample.z;
        float distance = std::sqrt(((x * x + y * y) + z * z));
        const float penetration =
            (ObjectActorValue<const VFCollisionShape *>(&ObjectActorValue<std::byte>(actor, 0x97c), 0x2c)[i].radius + hitbox->radius - distance) + .01f;
        if (!(penetration > 0))
            continue;
        if ((distance < 0 ? -distance : distance) < .00000011920929f) {
            x = object.contactPosition.x - centre.x;
            z = object.contactPosition.z - centre.z;
            distance = std::sqrt((x * x + z * z));
            if ((distance < 0 ? -distance : distance) < .00000011920929f) {
                if (object.reactionRotation.y > 0) {
                    x = penetration;
                    z = 0;
                } else {
                    x = -penetration;
                    z = 0;
                }
            } else {
                x = x * penetration / distance;
                z = z * penetration / distance;
            }
        } else {
            x = x * penetration / distance;
            z = z * penetration / distance;
        }
        if (x * object.reactionRotation.y + z * object.reactionRotation.z < 0) {
            if ((object.contactPosition.x - centre.x) * x + (object.contactPosition.z - centre.z) * z > 0) {
                object.reactionRotation.y = x;
                object.reactionRotation.z = z;
                deepest = penetration;
            }
        } else if (penetration > deepest) {
            object.reactionRotation.y = x;
            object.reactionRotation.z = z;
            deepest = penetration;
        }
    }
}

void vfResolveActorStageObjectPenetration(FightActorRuntimePrefix *actor, VOBJ_t *owner) {
    const FightStageObjectDefinition *definition;
    std::int32_t sphere;
    definition = owner->object.definition;
    for (sphere = 0; sphere < definition->hitboxCount; sphere++) {
        if (!ObjectHitboxAdmitted(owner, sphere))
            continue;
        if (definition->hitboxes[sphere].kind != 1)
            continue;
        owner->object.hitboxes[sphere].radius = owner->object.definition->hitboxes[sphere].radius;
        ComputeActorStageObjectPenetration(actor, owner, &owner->object.hitboxes[sphere]);
    }
}

#else

// Native VF_Same_S1: the deepest admitted sphere contact owns the planar correction.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool vfclIsActorPushBlocked(FightActorRuntimePrefix *actor, float x, float z) {
    struct PushProbe {
        std::int32_t kind;
        float radius, x, z;
        std::byte retained10[0x18];
    };
    vfActorMotionNullsub(GetCharacterNode(actor->character));
    const Vector3 &centre = actor->game->collision->samples[actor->groupIndex].points[0];
    PushProbe probe;
    probe.kind = 3;
    probe.radius = pushShapes[0].radius;
    probe.x = x + centre.x;
    probe.z = z + centre.z;
    if (vfStageObjectReturnZero(&probe, GetMapRetainedCharacterState()))
        return true;
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfProcessStageObjectHitOutcome(VOBJ_t *owner, FightActorRuntimePrefix *self, std::uint8_t guardState, std::uint16_t hitParameter, std::uint8_t hitDamage) {
    FightActorRuntimePrefix *source;
    FightActorRuntimePrefix *savedActor;
    FightActorRuntimePrefix *savedSecond;
    FightActorRuntimePrefix attacker;
    std::uint8_t level;
    float floor;
    std::int16_t direction;
    g_vf_hit_object_handled |= 1 << self->groupIndex;
    source = ObjectActor(owner->object.reactingCharacter, self->game);
    if ((owner->object.reactionFlags & 1) && source && (owner->object.motionFlags & 0x4000)) {
        level = ObjectActorValue<std::uint8_t>(source, 0x3a5);
        ObjectActorValue<std::uint8_t>(source, 0x3a5) = ObjectActorValue<std::uint8_t>(self, 0x3a5);
        vfclApplyHitResolution(source, self);
        ObjectActorValue<std::uint8_t>(source, 0x3a5) = level;
        return;
    }
    std::memset(&attacker, 0, sizeof(attacker));
    attacker.game = self->game;
    savedActor = self->actor;
    savedSecond = self->secondActor;
    self->actor = &attacker;
    self->secondActor = &attacker;
    attacker.actor = self;
    attacker.secondActor = self;
    attacker.hitStateFlags = 0x100;
    ObjectActorValue<std::uint8_t>(&attacker, 0x87f) = 80;
    ObjectActorValue<std::uint8_t>(&attacker, 0x880) = 80;
    if (guardState == 0xff) {
        floor = flddGetIntersectionFor(0, owner->object.contactPosition);
        if (owner->object.contactPosition.y - floor < .6f)
            guardState = 2;
        else
            guardState = owner->object.contactPosition.y - floor <= 1.2f;
    }
    if (static_cast<std::uint16_t>(hitParameter - 8) <= 7)
        attacker.hitParameter = 1 << hitParameter;
    ObjectActorValue<std::uint8_t>(&attacker, 0x2a9) = guardState;
    switch (guardState) {
    case 0:
        attacker.hitParameter |= 0x1e;
        break;
    case 1:
        attacker.hitParameter |= 0x1f;
        break;
    case 2:
        attacker.hitParameter |= 0x20;
        break;
    }
    direction = dc::sdk::ArcTan2Fixed(owner->object.contactPosition.x - self->contactPosition.x, owner->object.contactPosition.z - self->contactPosition.z);
    ObjectActorValue<std::int16_t>(&attacker, 0x2b2) = direction;
    ObjectActorValue<std::uint8_t>(&attacker, 0x2aa) = hitDamage;
    ObjectActorValue<std::uint8_t>(&attacker, 0x3a5) = 100;
    ObjectActorValue<std::uint16_t>(&attacker, 0x28e) = 104;
    vfclApplyHitResolution(&attacker, self);
    self->actor = savedActor;
    self->secondActor = savedSecond;
}

#else

// Native vfProcessHitOutcome: an unowned object constructs the native, zeroed stack attacker.
// It exists only for the synchronous resolution, just as the native producer does.
// @verified - p
void vfProcessStageObjectHitOutcome(VOBJ_t *owner, VFActor *self, std::int8_t guardState, std::int16_t hitParameter, std::int8_t hitDamage) {
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
    attacker.hitDamage = hitDamage;
    attacker.requestedAnimation = 104;
    attacker.guardLevel = 100;
    vfclApplyHitResolution(&attacker, self);
    self->secondActor = savedSecond;
    self->actor = savedActor;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfclFilterObjectHit(FightActorRuntimePrefix *actor, VOBJ_t *owner) {
    if (g_vf_hit_object_handled & (1U << actor->groupIndex))
        return;
    if ((actor->hitStateFlags & (0x4000U | 0x20000U | 0x40000U | 0x10000000U)) || (actor->moveFlags & 0x40000U))
        return;
    if ((actor->hitStateFlags & 0x80000U) && (actor->secondActor->hitStateFlags & 0x40000))
        return;
    FightStageObjectRuntime &object = owner->object;
    if (object.reactingCharacter == actor->character)
        return;
    const FightStageObjectDefinition *const definition = object.definition;
    if ((object.reactionFlags & 1) && (object.motionFlags & 0x4000)) {
        if (!IsOfThisVisibilityType(object.model, CVM_DisplayEnabled) || (actor->activeMotionFlags & 0x2000000U))
            return;
        FightActorRuntimePrefix *const source = ObjectActor(object.reactingCharacter, actor->game);
        if (!source || (source->hitStateFlags & (0x2U | 0x100U)) != 0x100)
            return;
        if (definition->attachmentBone == 14) {
            if (!(vfGetMoveCategoryMaskFor(source) & 0x10U))
                return;
        } else if (definition->attachmentBone != 21 || !(vfGetMoveCategoryMaskFor(source) & 0x80U))
            return;
    } else if (!(object.motionFlags & 0x2000)) {
        if (!(object.motionFlags & 0x1000))
            return;
        const float threshold = .02f;
        if (!((object.velocity.x < 0.0f ? -object.velocity.x : object.velocity.x) > threshold ||
                (object.velocity.y < 0.0f ? -object.velocity.y : object.velocity.y) > threshold ||
                (object.velocity.z < 0.0f ? -object.velocity.z : object.velocity.z) > threshold))
            return;
        const float approach = (actor->contactPosition.x - object.contactPosition.x) * object.velocity.x +
            (actor->contactPosition.z - object.contactPosition.z) * object.velocity.z;
        if (!(approach > 0.0f))
            return;
    }
    vfProcessStageObjectHitOutcome(owner, actor, definition->guardState, definition->hitParameter, definition->hitDamage);
    const std::uint32_t flags = definition->hitSound;
    if (flags)
        vfClAicaReact(flags, &object.contactPosition);
}

#else

// Object admission precedes the native producer and material-aware sound.
// @verified - p
void vfclFilterObjectHit(VFActor *actor, VOBJ_t *owner) {
    if (g_vf_hit_object_handled & (std::uint32_t{1} << actor->groupIndex))
        return;
    if ((actor->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_Unknown20000 | VFHSF_MidAirRecovery | VFHSF_ForcedReaction)) ||
        (actor->moveFlags & VFMF_NoContactPairing))
        return;
    if ((actor->hitStateFlags & VFHSF_AirborneOrDowned) && (actor->secondActor->hitStateFlags & 0x40000))
        return;
    FightStageObjectRuntime &object = owner->object;
    if (object.reactingCharacter == actor->character)
        return;
    const FightStageObjectDefinition *const definition = object.definition;
    if ((object.reactionFlags & 1) && (object.motionFlags & 0x4000)) {
        if (!IsOfThisVisibilityType(object.model, 3) || (actor->activeMotionFlags & VFSMF_Unknown2000000))
            return;
        VFActor *const source = ObjectActor(actor->game, object.reactingCharacter);
        if (!source || (source->hitStateFlags & (VFHSF_HitTimeout | VFHSF_AttackActive)) != 0x100)
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
    vfProcessStageObjectHitOutcome(owner, actor, definition->guardState, definition->hitParameter, definition->hitDamage);
    const std::uint32_t flags = definition->hitSound;
    if (flags)
        vfClAicaReact(object.contactPosition, flags);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfUpdateRotationAnglesFromStageObject(VOBJ_t *owner, FightRuntimePrefix *game) {
    Matrix4x4 matrix;
    Vector3 position;
    FightActorRuntimePrefix *const actor = ObjectActor(owner->object.reactingCharacter, game);
    if (actor && (((actor->hitStateFlags & 0x4000U) && !(owner->object.motionFlags & 0x400000)) || (actor->moveFlags & 0x20U))) {
        vfResetStageObjectSlot(owner);
        return;
    }
    const FightStageObjectDefinition *const definition = owner->object.definition;
    if (owner->object.motionFlags & 0x800000) {
        matrix_stack::UnitCurrentMatrix();
        charaGetMotWorldPos(owner->object.reactingCharacter, definition->attachmentBone, &position);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::RotateAroundY_Wrapper(GetCharacterMotionWorldYaw(owner->object.reactingCharacter));
        Matrix4x4::preTranslate(definition->attachmentPosition.x, definition->attachmentPosition.y, definition->attachmentPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(definition->attachmentRotation.x, definition->attachmentRotation.y, definition->attachmentRotation.z);
        if (definition->flags & 0x40) {
            if (owner->object.reactionFlags & 0x200)
                Matrix4x4::RotateAroundY_Wrapper(0x8000);
            Matrix4x4::RotateAroundX_Wrapper(ObjectActorValue<std::int32_t>(&owner->object, 0x30));
            matrix_stack::GetTranslation(&owner->object.contactPosition);
            matrix_stack::GetMatrix(&matrix);
            ExtractObjectPlanarEuler(matrix, owner->object.rotation);
        } else {
            matrix_stack::GetTranslation(&owner->object.contactPosition);
            matrix_stack::GetMatrix(&matrix);
            ExtractMatrixEulerAnglesFixed(&matrix, &owner->object.rotation);
        }
    } else {
        owner->object.contactPosition = definition->attachmentPosition;
        owner->object.rotation.x = definition->attachmentRotation.x;
        owner->object.rotation.y = definition->attachmentRotation.y;
        owner->object.rotation.z = definition->attachmentRotation.z;
        TransformCharacterPoseByMatrix(
            mtwkGetBoneTransform(owner->object.reactingCharacter, definition->attachmentBone), &owner->object.contactPosition, &owner->object.rotation);
    }
}

#else

// An attached object borrows its authored bone transform.
// @verified - p
void vfUpdateRotationAnglesFromStageObject(VOBJ_t *owner, VFMA_t *game) {
    FightStageObjectRuntime &object = owner->object;
    VFActor *const actor = ObjectActor(game, object.reactingCharacter);
    if (actor && (((actor->hitStateFlags & VFHSF_SpecialGuardOrParry) && !(object.motionFlags & 0x400000)) || (actor->moveFlags & VFMF_ForceDefeat))) {
        vfStageObjectReset(static_cast<std::uint8_t>(object.index), nullptr);
        return;
    }
    object.contactPosition = object.definition->attachmentPosition;
    object.rotation = object.definition->attachmentRotation;
    Matrix4x4 *const bone = mtwkGetBoneTransform(
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(object.reactingCharacter), object.definition->attachmentBone);
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native Matrix4x4::BuildRotationMatrix and BuildAndApplyRotationMatrix: retain the opposite-vector axis, including its length.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void AlignStageObjectAxisToTarget(VOBJ_t *owner, const Vector3 &world, std::int32_t vertical) {
    Matrix4x4 matrix;
    Vector3 target;
    if (vertical) {
        target.x = 0.0f;
        target.y = 1.0f;
        target.z = 0.0f;
    } else {
        target.x = world.x;
        target.y = 0.0f;
        target.z = world.z;
        am2Vector3NormalizeFast(&target);
    }
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4_BuildRotationMatrix(world, target);
    Matrix4x4::SetViewMatrixFromEuler(owner->object.rotation.x, owner->object.rotation.y, owner->object.rotation.z);
    matrix_stack::GetMatrix(&matrix);
    ExtractMatrixEulerAnglesFixed(&matrix, &owner->object.rotation);
}

namespace {

void AlignStageObjectAxis(VOBJ_t *owner) {
    Vector3 axis;
    Vector3 world;
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::SetViewMatrixFromEuler(owner->object.rotation.x, owner->object.rotation.y, owner->object.rotation.z);
    if (owner->object.definition->flags & 0x1c00) {
        if (owner->object.definition->flags & 0x400) {
            axis.x = 1.0f;
            axis.y = 0.0f;
            axis.z = 0.0f;
            Matrix4x4::Transform(axis, world);
            if (world.y >= 1.0f || world.y <= -1.0f) {
                owner->object.rotation.x = 0;
                owner->object.rotation.y = 0;
                owner->object.rotation.z = 0;
            } else
                AlignStageObjectAxisToTarget(owner, world, 0);
        } else if (owner->object.definition->flags & 0x800) {
            axis.x = 0.0f;
            axis.y = 1.0f;
            axis.z = 0.0f;
            Matrix4x4::Transform(axis, world);
            if (world.y >= 1.0f || world.y <= -1.0f) {
                owner->object.rotation.x = 0x4000;
                owner->object.rotation.y = 0;
                owner->object.rotation.z = 0;
            } else
                AlignStageObjectAxisToTarget(owner, world, 0);
        } else {
            axis.x = 0.0f;
            axis.y = 0.0f;
            axis.z = 1.0f;
            Matrix4x4::Transform(axis, world);
            if (world.y >= 1.0f || world.y <= -1.0f) {
                owner->object.rotation.x = 0;
                owner->object.rotation.y = 0;
                owner->object.rotation.z = 0;
            } else
                AlignStageObjectAxisToTarget(owner, world, 0);
        }
    } else {
        if (owner->object.definition->flags & 0x2000) {
            axis.x = 1.0f;
            axis.y = 0.0f;
            axis.z = 0.0f;
        } else if (owner->object.definition->flags & 0x4000) {
            axis.x = 0.0f;
            axis.y = 1.0f;
            axis.z = 0.0f;
        } else {
            axis.x = 0.0f;
            axis.y = 0.0f;
            axis.z = 1.0f;
        }
        Matrix4x4::Transform(axis, world);
        if (1.0f > world.y && world.y > -1.0f)
            AlignStageObjectAxisToTarget(owner, world, 1);
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct StageObjectProbe {
    std::uint32_t flags;
    Vector3 previousPosition;
    VOBJ_t *object;
    FightActorRuntimePrefix *actor;
};

typedef char StageObjectProbeSizeCheck[sizeof(StageObjectProbe) == 24 ? 1 : -1];

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define OBJECT_MAGNITUDE(value) ((value) < 0 ? -(value) : (value))

void UpdateStageObjectBody(VOBJ_t *owner, StageObjectProbe &probe) {
    Vector3 point;
    Matrix4x4 matrix;
    owner->object.contactPosition.x += owner->object.reactionRotation.y;
    owner->object.contactPosition.z += owner->object.reactionRotation.z;
    const FightStageObjectDefinition *const definition = owner->object.definition;
    float ground = flddGetIntersectionFor(0, owner->object.contactPosition) + definition->groundOffset;
    float nextGround;
    if (owner->object.contactPosition.y <= ground) {
        if (owner->object.reactionFlags & 0x10) {
            if (owner->object.definition->flags & 0x40)
                vfConvertToStageObjectHitboxSpace(owner, owner->object.velocity);
            owner->object.velocity.x *= .60000002f;
            owner->object.velocity.z *= .60000002f;
            if (definition->flags & 0xfc00)
                AlignStageObjectAxis(owner);
            probe.flags |= 0x10;
        }
        if (OBJECT_MAGNITUDE(owner->object.velocity.x) < .00000011920929f && OBJECT_MAGNITUDE(owner->object.velocity.z) < .00000011920929f &&
            OBJECT_MAGNITUDE(owner->object.reactionRotation.y) < .00000011920929f && OBJECT_MAGNITUDE(owner->object.reactionRotation.z) < .00000011920929f) {
            owner->object.velocity.y = 0;
            if (ground <= owner->object.contactPosition.y)
                return;
            owner->object.contactPosition.y = ground;
            probe.flags |= 0x10;
            return;
        }
        if (owner->object.motionFlags & 0x1000000) {
            owner->object.velocity.y = ground - owner->object.contactPosition.y;
            owner->object.velocity.x = 0;
            owner->object.velocity.z = 0;
        } else {
            owner->object.velocity.y = ground - owner->object.contactPosition.y;
            owner->object.velocity.x *= .92000002f;
            owner->object.velocity.z *= .92000002f;
            point.x = owner->object.contactPosition.x + owner->object.velocity.x;
            point.y = owner->object.contactPosition.y + 10;
            point.z = owner->object.contactPosition.z + owner->object.velocity.z;
            nextGround = flddGetIntersectionFor(0, point) + definition->groundOffset;
            if (nextGround > ground) {
                owner->object.velocity.x = 0;
                owner->object.velocity.z = 0;
            }
        }
    } else {
        point.x = owner->object.contactPosition.x + owner->object.velocity.x;
        point.y = owner->object.contactPosition.y + 10;
        point.z = owner->object.contactPosition.z + owner->object.velocity.z;
        nextGround = flddGetIntersectionFor(0, point) + definition->groundOffset;
        if (nextGround > owner->object.contactPosition.y) {
            owner->object.velocity.x = 0;
            owner->object.velocity.z = 0;
            nextGround = ground;
        }
        owner->object.velocity.y -= .032666668f;
        if (nextGround >= owner->object.contactPosition.y + owner->object.velocity.y)
            owner->object.velocity.y = nextGround - owner->object.contactPosition.y;
        if ((owner->object.reactionFlags & 2) && (definition->flags & 0x380)) {
            matrix_stack::UnitCurrentMatrix();
            Matrix4x4::SetViewMatrixFromEuler(owner->object.rotation.x, owner->object.rotation.y, owner->object.rotation.z);
            if (definition->flags & 0x80)
                Matrix4x4::RotateAroundX_Wrapper(definition->launchYaw);
            else if (definition->flags & 0x100)
                Matrix4x4::RotateAroundY_Wrapper(definition->launchYaw);
            else
                Matrix4x4::RotateAroundZ_Wrapper(definition->launchYaw);
            matrix_stack::GetMatrix(&matrix);
            ExtractMatrixEulerAnglesFixed(&matrix, &owner->object.rotation);
        }
    }
    if (owner->object.definition->flags & 0x10000) {
        if (owner->object.velocity.x != 0 || owner->object.velocity.z != 0) {
            if (owner->object.velocity.y == 0)
                owner->object.velocity.y = (std::fabs(owner->object.velocity.x) + std::fabs(owner->object.velocity.z)) * .25f;
        }
    }
    owner->object.contactPosition.x += owner->object.velocity.x;
    owner->object.contactPosition.y += owner->object.velocity.y;
    owner->object.contactPosition.z += owner->object.velocity.z;
    if (OBJECT_MAGNITUDE(owner->object.velocity.x) < .0099999998f && OBJECT_MAGNITUDE(owner->object.velocity.z) < .0099999998f) {
        owner->object.velocity.x = 0;
        owner->object.velocity.z = 0;
    }
    probe.flags |= 0x10;
}

#undef OBJECT_MAGNITUDE

} // namespace

#else

namespace {

// Native vfUpdateBody: one native gravity step; world and actor probes remain separate.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define OBJECT_MAGNITUDE(value) ((value) < 0 ? -(value) : (value))
struct StageObjectBounceView {
    std::byte retained00[0x48];
    Vector3 bounce;
};
#define OBJECT_BOUNCE(owner) (reinterpret_cast<StageObjectBounceView *>(owner)->bounce)

void CalculateStageObjectPoint(VOBJ_t *owner, StageObjectProbe &probe) {
    Vector3 point;
    Matrix4x4 matrix;
    owner->object.contactPosition.x += owner->object.reactionRotation.y;
    owner->object.contactPosition.z += owner->object.reactionRotation.z;
    const FightStageObjectDefinition *const definition = owner->object.definition;
    float ground = flddGetIntersectionFor(0, owner->object.position) + definition->groundOffset + definition->positionOffset.y;
    if (owner->object.position.y <= ground && owner->object.velocity.y < .00000011920929f) {
        if (OBJECT_BOUNCE(owner).y > .00000011920929f) {
            owner->object.velocity.x = OBJECT_BOUNCE(owner).x;
            owner->object.velocity.y = OBJECT_BOUNCE(owner).y;
            owner->object.velocity.z = OBJECT_BOUNCE(owner).z;
            owner->object.yaw /= 2;
            OBJECT_BOUNCE(owner).x = OBJECT_BOUNCE(owner).y = OBJECT_BOUNCE(owner).z = 0;
            if (owner->object.definition->groundSound)
                vfClAicaReact(owner->object.definition->groundSound, &owner->object.contactPosition);
        } else {
            if (owner->object.reactionFlags & 0x10) {
                if (owner->object.definition->flags & 0x40)
                    vfConvertToStageObjectHitboxSpace(owner, owner->object.velocity);
                owner->object.velocity.x *= .60000002f;
                owner->object.velocity.z *= .60000002f;
                probe.flags |= 0x10;
            }
            if (OBJECT_MAGNITUDE(owner->object.velocity.x) < .00000011920929f && OBJECT_MAGNITUDE(owner->object.velocity.z) < .00000011920929f &&
                OBJECT_MAGNITUDE(owner->object.reactionRotation.y) < .00000011920929f &&
                OBJECT_MAGNITUDE(owner->object.reactionRotation.z) < .00000011920929f) {
                owner->object.velocity.y = 0;
                if (!(ground > owner->object.position.y))
                    return;
                owner->object.contactPosition.y += ground - owner->object.position.y;
                owner->object.position.y = ground;
                probe.flags |= 0x10;
                return;
            }
            owner->object.velocity.y = ground - owner->object.position.y;
            owner->object.velocity.x = 0;
            owner->object.velocity.z = 0;
        }
    } else {
        point.x = owner->object.contactPosition.x + owner->object.velocity.x;
        point.y = owner->object.contactPosition.y + 10.0f;
        point.z = owner->object.contactPosition.z + owner->object.velocity.z;
        float nextGround = flddGetIntersectionFor(0, point) + definition->groundOffset + definition->positionOffset.y;
        if (nextGround > owner->object.position.y)
            nextGround = ground;
        owner->object.velocity.y -= .032666668f;
        if (!(owner->object.position.y + owner->object.velocity.y > nextGround)) {
            OBJECT_BOUNCE(owner).x = owner->object.velocity.x * .5f;
            OBJECT_BOUNCE(owner).y = owner->object.velocity.y * -.5f;
            OBJECT_BOUNCE(owner).z = owner->object.velocity.z * .5f;
            const std::int16_t angle = static_cast<std::int16_t>(owner->object.rotation.z);
            std::int16_t target = static_cast<std::int16_t>(angle & 0xc000);
            const std::int16_t second = static_cast<std::int16_t>(target + 0x4000);
            const std::int16_t firstDelta = static_cast<std::int16_t>(target - angle);
            const std::int16_t secondDelta = static_cast<std::int16_t>(second - angle);
            if (std::fabs(float(firstDelta)) < std::fabs(float(secondDelta))) {
                owner->object.yaw = static_cast<std::int32_t>(-std::fabs(float(owner->object.yaw)) * .5f);
            } else {
                target = second;
                owner->object.yaw = static_cast<std::int32_t>(std::fabs(float(owner->object.yaw)) * .5f);
            }
            if (!(OBJECT_BOUNCE(owner).y > .065333337f)) {
                OBJECT_BOUNCE(owner).y = 0;
                owner->object.yaw = 0;
                owner->object.rotation.z = target;
            }
            owner->object.velocity.y = nextGround - owner->object.position.y;
            if (owner->object.definition->groundSound)
                vfClAicaReact(owner->object.definition->groundSound, &owner->object.contactPosition);
        }
        if ((owner->object.reactionFlags & 2) && (definition->flags & 0x380)) {
            matrix_stack::UnitCurrentMatrix();
            Matrix4x4::SetViewMatrixFromEuler(owner->object.rotation.x, owner->object.rotation.y, owner->object.rotation.z);
            if (definition->flags & 0x80)
                Matrix4x4::RotateAroundX_Wrapper(owner->object.yaw);
            else if (definition->flags & 0x100)
                Matrix4x4::RotateAroundY_Wrapper(owner->object.yaw);
            else
                Matrix4x4::RotateAroundZ_Wrapper(owner->object.yaw);
            matrix_stack::GetMatrix(&matrix);
            ExtractMatrixEulerAnglesFixed(&matrix, &owner->object.rotation);
        }
        vfRotateStageObjectAboutOffset(owner, owner->object.definition->positionOffset);
    }
    owner->object.contactPosition.x += owner->object.velocity.x;
    owner->object.contactPosition.y += owner->object.velocity.y;
    owner->object.contactPosition.z += owner->object.velocity.z;
    owner->object.position.x += owner->object.velocity.x;
    owner->object.position.y += owner->object.velocity.y;
    owner->object.position.z += owner->object.velocity.z;
    if (OBJECT_MAGNITUDE(owner->object.velocity.x) < .0099999998f && OBJECT_MAGNITUDE(owner->object.velocity.z) < .0099999998f) {
        owner->object.velocity.x = 0;
        owner->object.velocity.z = 0;
    }
    probe.flags |= 0x10;
}

#undef OBJECT_BOUNCE
#undef OBJECT_MAGNITUDE

} // namespace

#else

namespace {

// Native vfCalculateAndTransformPoint: definition seven rolls around its authored position offset.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfBreakStageObject(VOBJ_t *owner) {
    if (vfAllocateStageObjectMotionTracks(owner))
        return;
    owner->object.reactionFlags &= 0xffffff9d;
    owner->object.state = 1;
    owner->motionFrame = owner->object.definition->motionFirstFrame;
    if (owner->object.definition->breakSound)
        vfClAicaReact(owner->object.definition->breakSound, &owner->object.contactPosition);
    if (owner->object.reactionFlags & 1)
        vfResetStageObjectSlot(owner);
    EV_SetCharDisplay(owner->object.brokenModel, CVM_DisplayEnabled, 1);
    EV_SetCharDisplay(owner->object.controlCharacter, CVM_DisplayEnabled, 0);
    owner->object.model = owner->object.brokenModel;
    SetCharacterWorldPosition(owner->object.model, &owner->object.contactPosition);
    charaSetWorldRot(owner->object.model, owner->object.rotation);
    EmitFightSmoke(5, reinterpret_cast<FightActorRuntimePrefix *>(owner->object.model));
    vfResetStageObjectRotationImpulse(owner);
    owner->object.contactState = 0;
    owner->object.contactSlotMask = 0;
    owner->object.velocity.x = 0;
    owner->object.velocity.z = 0;
    if (owner->object.definition->index == 8) {
        FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(GetFightTaskParameter());
        if (game)
            game->frame |= 2U;
    }
}

#else

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
    EV_SetCharDisplay(broken, CVM_DisplayEnabled, 1);
    EV_SetCharDisplay(object.controlCharacter, CVM_DisplayEnabled, 0);
    object.model = broken;
    shenApplyTranslationToAll(GetCharacterNode(object.model), object.contactPosition);
    charaSetWorldRot(object.model, object.rotation);
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool ObjectContactAdmitted(const FightStageObjectRuntime &object) {
    const std::uint32_t mask = object.definition->flags;
    return (mask & 0x2U) || (object.state && (mask & 0x4U));
}

} // namespace

#else

namespace {

bool ObjectContactAdmitted(const FightStageObjectRuntime &object) {
    const std::uint32_t mask = object.definition->flags;
    return (mask & 0x2U) || (object.state && (mask & 0x4U));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool ObjectActorCanBreak(const FightStageObjectRuntime &object, const FightActorRuntimePrefix &actor) {
    return (object.motionFlags & 0x20) || ((object.motionFlags & 0x10) && ObjectHasHitSpeed(object.velocity)) ||
        ((object.motionFlags & 0x200) &&
            ((actor.hitStateFlags & 0x80000U) || (actor.activeMotionFlags & 0x2000U) || (actor.hitStateFlags & (0x10U | 0x4000U)) == 0x4010));
}

} // namespace

#else

namespace {

bool ObjectActorCanBreak(const FightStageObjectRuntime &object, const VFActor &actor) {
    return (object.motionFlags & 0x20) || ((object.motionFlags & 0x10) && ObjectHasHitSpeed(object.velocity)) ||
        ((object.motionFlags & 0x200) &&
            ((actor.hitStateFlags & VFHSF_AirborneOrDowned) || (actor.activeMotionFlags & VFSMF_Unknown2000) ||
                (actor.hitStateFlags & (VFHSF_FallLandingRequired | VFHSF_SpecialGuardOrParry)) == 0x4010));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ClearObjectRotationImpulse(VOBJ_t *owner) {
    owner->object.reactionFlags &= 0xffffff9d;
    owner->object.rotationImpulse = ObjectRotation(0, 0, 0);
}

} // namespace

#else

namespace {

void ClearObjectRotationImpulse(VOBJ_t *owner) {
    owner->object.reactionFlags &= 0xffffff9d;
    owner->object.rotationImpulse = {};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ClearObjectPlanarCorrection(FightStageObjectRuntime &object) {
    object.reactionRotation.y = object.reactionRotation.z = 0;
}

} // namespace

#else

namespace {

void ClearObjectPlanarCorrection(FightStageObjectRuntime &object) {
    object.reactionRotation.y = object.reactionRotation.z = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ProbeStageObjectWorld(VOBJ_t *owner, StageObjectProbe &probe) {
    struct WorldProbe {
        std::int32_t kind;
        float radius, x, z;
        std::byte retained10[0x18];
    };
    WorldProbe query;
    const FightStageObjectDefinition *definition;
    std::int32_t i;
    vfActorMotionNullsub(GetCharacterNode(owner->object.model));
    query.kind = 3;
    definition = owner->object.definition;
    for (i = 0; i < definition->hitboxCount; i++) {
        if (!ObjectHitboxAdmitted(owner, i))
            continue;
        if (definition->hitboxes[i].kind == 1) {
            query.radius = definition->hitboxes[i].radius;
            query.x = owner->object.hitboxes[i].position.x + owner->object.velocity.x;
            query.z = owner->object.hitboxes[i].position.z + owner->object.velocity.z;
        }
        if (vfStageObjectReturnZero(&query, GetMapRetainedCharacterState())) {
            probe.flags |= 2;
            return;
        }
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ProbeStageObjectPair(VOBJ_t *owner, StageObjectProbe &probe) {
    VOBJ_t *other;
    for (other = g_fightStageObjectRoot; other; other = other->next) {
        if (((!other->object.state && (other->object.definition->flags & 2)) || (other->object.state && (other->object.definition->flags & 4))) &&
            owner != other && AreStageObjectsNear(other, owner)) {
            const FightStageObjectDefinition *const otherDefinition = other->object.definition;
            const FightStageObjectDefinition *const ownDefinition = owner->object.definition;
            for (std::int32_t i = 0; i < otherDefinition->hitboxCount; ++i) {
                if (ObjectHitboxAdmitted(other, i) && static_cast<std::uint16_t>(otherDefinition->hitboxes[i].kind) == 1) {
                    Vector3 point;
                    point.x = other->object.hitboxes[i].position.x;
                    point.y = other->object.hitboxes[i].position.y;
                    point.z = other->object.hitboxes[i].position.z;
                    for (std::int32_t j = 0; j < ownDefinition->hitboxCount; ++j) {
                        if (ObjectHitboxAdmitted(owner, j) && static_cast<std::uint16_t>(ownDefinition->hitboxes[j].kind) == 1) {
                            Vector3 moved;
                            moved.x = owner->object.hitboxes[j].position.x + owner->object.velocity.x;
                            moved.y = owner->object.hitboxes[j].position.y + owner->object.velocity.y;
                            moved.z = owner->object.hitboxes[j].position.z + owner->object.velocity.z;
                            if (AreStageObjectSpheresTouching(point, moved, otherDefinition->hitboxes[i].radius, ownDefinition->hitboxes[j].radius))
                                goto hit;
                        }
                    }
                }
            }
        }
    }
    return;
hit:
    probe.object = other;
    probe.flags |= 8;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ProbeStageObjectActor(VOBJ_t *owner, StageObjectProbe &probe, FightRuntimePrefix *game) {
    for (std::int32_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[i];
        if ((owner->object.reactionFlags & 1) && owner->object.reactingCharacter == actor->character)
            continue;
        if (vfStageObjectOverlapsActor(owner, actor, 1)) {
            probe.flags |= 4;
            probe.actor = actor;
            return;
        }
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define OBJECT_MAGNITUDE(value) ((value) < 0 ? -(value) : (value))
#define OBJECT_HAS_HIT_SPEED(velocity) (OBJECT_MAGNITUDE((velocity).x) > .02f || OBJECT_MAGNITUDE((velocity).y) > .02f || OBJECT_MAGNITUDE((velocity).z) > .02f)

void ResolveStageObjectCollisionResponse(VOBJ_t *owner, FightRuntimePrefix *game, StageObjectProbe &probe) {
    Vector3 direction;
    std::uint32_t mask = owner->object.definition->flags;
    if (!(mask & 2) && (!owner->object.state || !(mask & 4)))
        return;
    ProbeStageObjectWorld(owner, probe);
    if (probe.flags & 3) {
        const std::uint32_t flags = owner->object.motionFlags;
        if ((flags & 0x100) && ((flags & 0x20) || ((flags & 0x10) && OBJECT_HAS_HIT_SPEED(owner->object.velocity)))) {
            direction.x = -owner->object.velocity.x;
            direction.y = 0;
            direction.z = -owner->object.velocity.z;
            vfDeflectStageObjectYaw(owner, direction);
            vfBreakStageObject(owner);
        }
        owner->object.reactionFlags &= ~2U;
        vfResetStageObjectRotationImpulse(owner);
        owner->object.contactPosition.x = probe.previousPosition.x;
        owner->object.contactPosition.z = probe.previousPosition.z;
        owner->object.velocity.x *= -.4f;
        owner->object.velocity.z *= -.4f;
        owner->object.reactionRotation.y = 0;
        owner->object.reactionRotation.z = 0;
        owner->object.yaw = -owner->object.yaw;
        return;
    }
    ProbeStageObjectPair(owner, probe);
    if (probe.flags & 8) {
        VOBJ_t *const other = probe.object;
        const std::uint32_t flags = owner->object.motionFlags;
        if ((flags & 0x80) &&
            ((flags & 0x20) ||
                ((flags & 0x10) && OBJECT_HAS_HIT_SPEED(owner->object.velocity) &&
                    (other->object.contactPosition.x - probe.previousPosition.x) * owner->object.velocity.x +
                            (other->object.contactPosition.z - probe.previousPosition.z) * owner->object.velocity.z >
                        0))) {
            direction.x = -owner->object.velocity.x;
            direction.y = 0;
            direction.z = -owner->object.velocity.z;
            vfDeflectStageObjectYaw(owner, direction);
            vfBreakStageObject(owner);
        }
        owner->object.reactionFlags &= ~2U;
        vfResetStageObjectRotationImpulse(owner);
        owner->object.contactPosition.x = probe.previousPosition.x;
        owner->object.contactPosition.z = probe.previousPosition.z;
        const std::uint32_t targetFlags = other->object.motionFlags;
        if ((targetFlags & 0x80) && ((targetFlags & 0x20) || ((targetFlags & 0x10) && OBJECT_HAS_HIT_SPEED(owner->object.velocity)))) {
            direction.x = owner->object.velocity.x;
            direction.y = 0;
            direction.z = owner->object.velocity.z;
            vfDeflectStageObjectYaw(other, direction);
            vfBreakStageObject(other);
            other->object.reactionRotation.y = 0;
            other->object.reactionRotation.z = 0;
            other->object.reactionFlags &= ~2U;
            vfResetStageObjectRotationImpulse(other);
            owner->object.velocity.x *= .5f;
            owner->object.velocity.z *= .5f;
            owner->object.reactionRotation.y = 0;
            owner->object.reactionRotation.z = 0;
            return;
        }
        other->object.reactionFlags &= ~2U;
        vfResetStageObjectRotationImpulse(other);
        if ((other->object.motionFlags & 1) && (other->object.reactionFlags & 1) == 0) {
            other->object.velocity.x += owner->object.velocity.x;
            other->object.velocity.z += owner->object.velocity.z;
            other->object.reactionRotation.y += owner->object.reactionRotation.y;
            other->object.reactionRotation.z += owner->object.reactionRotation.z;
            if (other->object.definition->flags & 0x40U) {
                vfConvertToStageObjectHitboxSpace(other, other->object.velocity);
                direction.x = other->object.reactionRotation.y;
                direction.y = 0;
                direction.z = other->object.reactionRotation.z;
                vfConvertToStageObjectHitboxSpace(other, direction);
                other->object.reactionRotation.y = direction.x;
                other->object.reactionRotation.z = direction.z;
            }
        }
        owner->object.velocity.x = 0;
        owner->object.velocity.z = 0;
        owner->object.reactionRotation.y = 0;
        owner->object.reactionRotation.z = 0;
        return;
    }
    ProbeStageObjectActor(owner, probe, game);
    if (probe.flags & 4) {
        FightActorRuntimePrefix *const actor = probe.actor;
        vfclFilterObjectHit(actor, owner);
        if (!owner->object.state && (owner->object.motionFlags & 0x40)) {
            const std::uint32_t flags = owner->object.motionFlags;
            if ((flags & 0x20) || ((flags & 0x10) && OBJECT_HAS_HIT_SPEED(owner->object.velocity))) {
                direction.x = -owner->object.velocity.x;
                direction.y = 0;
                direction.z = -owner->object.velocity.z;
                vfDeflectStageObjectYaw(owner, direction);
                vfBreakStageObject(owner);
            } else if ((owner->object.motionFlags & 0x200) &&
                ((actor->hitStateFlags & 0x80000U) || (actor->activeMotionFlags & 0x2000U) || (actor->hitStateFlags & (0x10U | 0x4000U)) == 0x4010)) {
                direction.x = -owner->object.velocity.x;
                direction.y = 0;
                direction.z = -owner->object.velocity.z;
                vfDeflectStageObjectYaw(owner, direction);
                vfBreakStageObject(owner);
            }
        }
        mask = owner->object.definition->flags;
        if ((mask & 2) || (owner->object.state && (mask & 4))) {
            owner->object.contactState |= 1 << actor->groupIndex;
            actor->stateFlags |= 0x20U;
        }
        owner->object.reactionFlags &= ~2U;
        vfResetStageObjectRotationImpulse(owner);
        owner->object.contactPosition.x = probe.previousPosition.x;
        owner->object.contactPosition.z = probe.previousPosition.z;
        owner->object.velocity.x *= -.5f;
        owner->object.velocity.z *= -.5f;
    }
    owner->object.reactionRotation.y = 0;
    owner->object.reactionRotation.z = 0;
}

#undef OBJECT_HAS_HIT_SPEED
#undef OBJECT_MAGNITUDE

} // namespace

#else

namespace {

// Native vfResolveStageObjectCollisionResponse: world, object and character response are ordered and exclusive.
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
            actor->stateFlags |= VFSF_Unknown20;
        }
        ClearObjectRotationImpulse(owner);
        object.contactPosition.x = probe.previousPosition.x;
        object.contactPosition.z = probe.previousPosition.z;
        object.velocity.x *= -.5f;
        object.velocity.z *= -.5f;
    }
    ClearObjectPlanarCorrection(object);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ResolveStageObjectProbeContacts(VOBJ_t *owner, FightRuntimePrefix *game, StageObjectProbe &probe) {
    if (owner->object.reactionFlags & 1) {
        vfUpdateRotationAnglesFromStageObject(owner, game);
        probe.flags |= 0x10;
    } else if (owner->object.definition->index == 7)
        CalculateStageObjectPoint(owner, probe);
    else
        UpdateStageObjectBody(owner, probe);
    if ((owner->object.reactionFlags & 1) == 0 && (probe.flags & 0x10))
        ResolveStageObjectCollisionResponse(owner, game, probe);
    float ground;
    float *height;
    if (owner->object.definition->flags & 0x40000U) {
        ground = flddGetIntersectionFor(0, owner->object.position);
        height = &owner->object.position.y;
    } else {
        ground = flddGetIntersectionFor(0, owner->object.contactPosition);
        height = &owner->object.contactPosition.y;
    }
    ground = *height - (ground + owner->object.definition->groundOffset);
    if (!(ground > 0.0f)) {
        if (owner->object.reactionFlags & 8)
            owner->object.reactionFlags &= 0xffffffef;
        else {
            if (owner->object.velocity.y < -.039200004f && owner->object.definition->groundSound)
                vfClAicaReact(owner->object.definition->groundSound, &owner->object.contactPosition);
            owner->object.reactionFlags = (owner->object.reactionFlags | 0x18) & 0xfffffffd;
        }
    } else
        owner->object.reactionFlags &= 0xffffffe7;
}

} // namespace

#else

namespace {

// Native vfclResolveProbeContacts: attachment or physics first; ground transition flags are published last.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ResetStageObjectFrameContacts() {
    for (VOBJ_t *owner = g_fightStageObjectRoot; owner; owner = owner->next) {
        owner->object.contactState = 0;
        owner->object.contactSlotMask = 0;
    }
    g_vf_hit_object_handled = 0;
}

void vfclProcessCollisionAndSound(FightRuntimePrefix *game) {
    for (std::int32_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[i];
        for (VOBJ_t *owner = g_fightStageObjectRoot; owner; owner = owner->next) {
            if (!((owner->object.definition->flags & 2) || (owner->object.state && (owner->object.definition->flags & 4))))
                continue;
            if ((owner->object.reactionFlags & 1) && (owner->object.reactingCharacter == actor->character || (actor->hitStateFlags & 0xc4000U)))
                continue;
            if (!vfStageObjectOverlapsActor(owner, actor, 0))
                continue;
            vfclFilterObjectHit(actor, owner);
            if (!owner->object.state && (owner->object.motionFlags & 0x40)) {
                if (owner->object.motionFlags & 0x20) {
                    vfBreakStageObject(owner);
                    if (!((!owner->object.state && (owner->object.definition->flags & 2)) || (owner->object.state && (owner->object.definition->flags & 4))))
                        continue;
                } else if ((owner->object.motionFlags & 0x10) &&
                    ((owner->object.velocity.x < 0 ? -owner->object.velocity.x : owner->object.velocity.x) > .02f ||
                        (owner->object.velocity.y < 0 ? -owner->object.velocity.y : owner->object.velocity.y) > .02f ||
                        (owner->object.velocity.z < 0 ? -owner->object.velocity.z : owner->object.velocity.z) > .02f) &&
                    (actor->contactPosition.x - owner->object.contactPosition.x) * owner->object.velocity.x +
                            (actor->contactPosition.z - owner->object.contactPosition.z) * owner->object.velocity.z >
                        0) {
                    vfBreakStageObject(owner);
                    if (!((!owner->object.state && (owner->object.definition->flags & 2)) || (owner->object.state && (owner->object.definition->flags & 4))))
                        continue;
                } else if ((owner->object.motionFlags & 0x200) &&
                    ((actor->hitStateFlags & 0x80000U) || (actor->activeMotionFlags & 0x2000U) || (actor->hitStateFlags & 0x4010U) == 0x4010)) {
                    vfBreakStageObject(owner);
                    if (!((!owner->object.state && (owner->object.definition->flags & 2)) || (owner->object.state && (owner->object.definition->flags & 4))))
                        continue;
                }
            }
            owner->object.contactState |= 1 << actor->groupIndex;
            actor->stateFlags |= 0x20;
            vfResolveActorStageObjectPenetration(actor, owner);
            float pushX, pushZ;
            if ((owner->object.motionFlags & 1) && (owner->object.reactionFlags & 1) == 0) {
                float factor = 1 - owner->object.definition->responseCoefficient * .045f;
                if (factor < 0)
                    factor = 0;
                pushX = -owner->object.reactionRotation.y * (1 - factor);
                pushZ = -owner->object.reactionRotation.z * (1 - factor);
                if (owner->object.definition->flags & 0x40) {
                    Vector3 correction;
                    correction.x = owner->object.reactionRotation.y;
                    correction.y = 0;
                    correction.z = owner->object.reactionRotation.z, vfConvertToStageObjectHitboxSpace(owner, correction),
                    owner->object.reactionRotation.y = correction.x * factor, owner->object.reactionRotation.z = correction.z * factor;
                } else {
                    owner->object.reactionRotation.y *= factor;
                    owner->object.reactionRotation.z *= factor;
                }
            } else {
                pushX = -owner->object.reactionRotation.y;
                pushZ = -owner->object.reactionRotation.z;
                owner->object.reactionRotation.y = 0;
                owner->object.reactionRotation.z = 0;
            }
            if (vfclIsActorPushBlocked(actor, pushX, pushZ)) {
                const Vector3 *previous, *current;
                previous = game->collision->previousSamples[i].points, current = game->collision->samples[i].points,
                vfclMoveEntityWithContactHistory(actor, previous->x - current->x, previous->z - current->z);
                break;
            }
            vfclMoveEntityWithContactHistory(actor, pushX, pushZ);
        }
    }
}

#else

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
            if ((object.reactionFlags & 1) &&
                (object.reactingCharacter == actor->character ||
                    (actor->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned))))
                continue;
            if (!vfStageObjectOverlapsActor(owner, actor, 0))
                continue;
            vfclFilterObjectHit(actor, owner);
            if (!object.state && (object.motionFlags & 0x40)) {
                const float approach = (actor->contactPosition.z - object.contactPosition.z) * object.velocity.z +
                    (actor->contactPosition.x - object.contactPosition.x) * object.velocity.x;
                const bool breaks = (object.motionFlags & 0x20) || ((object.motionFlags & 0x10) && ObjectHasHitSpeed(object.velocity) && approach > 0) ||
                    ((object.motionFlags & 0x200) &&
                        ((actor->hitStateFlags & VFHSF_AirborneOrDowned) || (actor->activeMotionFlags & VFSMF_Unknown2000) ||
                            (actor->hitStateFlags & (VFHSF_FallLandingRequired | VFHSF_SpecialGuardOrParry)) == 0x4010));
                if (breaks) {
                    vfBreakStageObject(owner);
                    if (!(object.definition->flags & (object.state ? 0x4U : 0x2U)))
                        continue;
                }
            }
            object.contactSlotMask |= std::uint64_t{1} << actor->groupIndex;
            actor->stateFlags |= VFSF_Unknown20;
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfclUpdateStageObjectHitboxesPhysics(FightRuntimePrefix *game) {
    for (VOBJ_t *owner = g_fightStageObjectRoot; owner; owner = owner->next) {
        if (!(owner->object.motionFlags & 0x700003))
            continue;
        StageObjectProbe probe;
        probe.flags = 0;
        probe.previousPosition = owner->object.contactPosition;
        const FightStageObjectDefinition *const definition = owner->object.definition;
        ResolveStageObjectProbeContacts(owner, game, probe);
        if (probe.flags & 0x10) {
            Vector3 displacement;
            displacement.x = owner->object.contactPosition.x - probe.previousPosition.x;
            displacement.y = owner->object.contactPosition.y - probe.previousPosition.y;
            displacement.z = owner->object.contactPosition.z - probe.previousPosition.z;
            if ((displacement.x < 0.0f ? -displacement.x : displacement.x) >= .00000011920929f ||
                (displacement.z < 0.0f ? -displacement.z : displacement.z) >= .00000011920929f) {
                if (owner->object.reactionFlags & 8) {
                    if (definition->flags & 0x20)
                        vfCreateStageObjectRollMatrix(owner, displacement);
                    else if (definition->flags & 0x40)
                        vfCreateStageObjectRotationMatrix(owner, displacement);
                }
            } else if ((displacement.y < 0.0f ? -displacement.y : displacement.y) < .00000011920929f)
                vfResetStageObjectRotationImpulse(owner);
            if ((owner->object.reactionFlags & 0x60) && (!(definition->flags & 0x60U) || !(owner->object.reactionFlags & 8)))
                vfRecalcStageObjectLocalEuler(owner);
            SetCharacterWorldPosition(owner->object.model, &owner->object.contactPosition);
            charaSetWorldRot(owner->object.model, owner->object.rotation);
            vfTransformStageObjectHitboxes(owner);
        }
        vfUpdateStageObjectWorldPos(owner);
    }
}

#else

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
            charaSetWorldRot(object.model, object.rotation);
            vfTransformStageObjectHitboxes(owner);
        }
        vfUpdateStageObjectWorldPos(owner);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfclTriggerStageObjectHitReaction(FightRuntimePrefix *game) {
    FightActorRuntimePrefix *actor;
    VOBJ_t *owner;
    std::int16_t angle;
    float vertical;
    std::uint8_t parameter;
    std::int32_t airborne = 0;
    for (std::uint8_t i = 0; i < 6; i++) {
        if (game->characters[i]) {
            actor = game->actors[i];
            if (actor->hitStateFlags & 0xc0000) {
                airborne = 1;
                goto found;
            }
            if (!(g_vf_hit_object_handled & (1 << actor->groupIndex)) && (actor->hitStateFlags & 0x100))
                goto found;
        }
        continue;
    found:
        for (owner = g_fightStageObjectRoot; owner; owner = owner->next) {
            if (owner->object.reactionFlags & 1)
                continue;
            if (!(owner->object.motionFlags & 2))
                continue;
            if (!(owner->object.contactState & (1 << actor->groupIndex)))
                continue;
            angle =
                dc::sdk::ArcTan2Fixed(actor->contactPosition.x - owner->object.contactPosition.x, actor->contactPosition.z - owner->object.contactPosition.z) -
                actor->temporaryFacingAngle + 0x4000;
            if (!airborne && (angle & 0x8000))
                continue;
            if (owner->object.velocity.y == 0)
                vfClAicaReact(owner->object.definition->launchSound, &owner->object.contactPosition);
            vertical = 0;
            if (owner->object.definition->index == 7) {
                parameter = static_cast<std::uint8_t>(actor->hitParameter);
                if (parameter == 1 || parameter == 4 || parameter == 18 || parameter == 9)
                    vertical = .25f;
                else
                    vertical = .17f;
            }
            if (!airborne)
                angle = actor->temporaryFacingAngle;
            else
                angle = dc::sdk::ArcTan2Fixed(
                    actor->contactPosition.x - owner->object.contactPosition.x, actor->contactPosition.z - owner->object.contactPosition.z);
            vfStageObjectAssignVelocityXYZ(static_cast<std::uint8_t>(owner->object.index),
                -CalculateFixedSine(angle) * (owner->object.definition->responseCoefficient * -0.0166f + 0.5f), vertical,
                -CalculateFixedCosine(angle) * (owner->object.definition->responseCoefficient * -0.0166f + 0.5f));
            owner->object.yaw = owner->object.definition->launchYaw;
            if (owner->object.definition->flags & 0x40)
                vfConvertToStageObjectHitboxSpace(owner, owner->object.velocity);
            owner->object.reactingCharacter = actor->character;
        }
        return;
    }
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfUpdateStageObjects(FightRuntimePrefix *game) {
    matrix_stack::Matrix4x4_Push(0);
    ResetStageObjectFrameContacts();
    vfclProcessCollisionAndSound(game);
    vfclUpdateStageObjectHitboxesPhysics(game);
    vfclTriggerStageObjectHitReaction(game);
    vfStageObjectEvalHitboxKeyframes();
    PopCurrentMatrix(1);
}

#else

// Retain all four native passes and their shared per-frame hit mask.
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

#endif

} // namespace s2
} // namespace shenmue
