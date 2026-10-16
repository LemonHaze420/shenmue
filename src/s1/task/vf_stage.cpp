#include "s1/task/scen.hpp"
#include "s1/task/vfma.hpp"
#include "s1/input.hpp"
#include "s1/motion.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/time.hpp"
#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/lighting.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/btpk.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/char.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/vfcl.hpp"
#include "s1/task/vf_core.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/tmex.hpp"
#include "s1/subtitles.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <type_traits>

#include "s1/task/vf_internal.hpp"

namespace shenmue::s1 {
using namespace vf_detail;

namespace vf_detail {

std::array<VOBJ *, 8> g_pVOBJ{};
std::array<VFStageObject *, 20> g_vfStageObjs{};
VFStageObject *g_vfObjectRoot = nullptr;
std::int32_t g_vfHitObjectHandled = 0;

void ResetStageObjectMotion(VFStageObject &object) {
    object.localVelocity = {};
    object.angle = {};
    object.stateFlags &= 0xfffffd84U;
}

void vfStageObjectApplyVelocity(VFActor &actor) {
    VFStageObject &object = *g_vfStageObjs[actor.stageHitObjectIndex];
    ResetStageObjectMotion(object);
    const std::uint16_t angle = actor.hitDirectionAngle;
    VFStageObject &target = *g_vfStageObjs[actor.stageHitObjectIndex];
    target.localVelocity.x = static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(angle))) * 0.2);
    target.localVelocity.y = 0.1f;
    const std::int16_t folded = angle <= 0x8000U ? static_cast<std::int16_t>(angle) : static_cast<std::int16_t>(-angle);
    target.localVelocity.z = static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(0x4000 - folded))) * 0.2);
}

void vfStageObjectResetAndActivateForChara(std::int32_t index, Character *character) {
    VFStageObject &object = *g_vfStageObjs[index];
    ResetStageObjectMotion(object);
    if (!character)
        return;
    if ((object.object->entityFlags & 0x40U) != 0) {
        const Vector3 position = GetPositionForEntity(character->entity.node);
        const std::uint16_t bearing = Shenmue_Atan2Fixed(position.z - object.position.z, position.x - object.position.x);
        if (((0U - (static_cast<std::uint32_t>(object.rotation.y) + bearing)) & 0x8000U) == 0)
            object.stateFlags |= 0x200U;
        object.field38 = object.rotation.x;
    }
    object.stateFlags |= 1U;
    object.character = character;
}

constexpr float kStageObjectEpsilon = 1.1920929e-07f;
constexpr std::uint32_t kVobjToken = SHEN_NAME(VOBJ);

struct VFStageHitProbe {
    std::uint32_t hitState;
    Vector3 position;
    VFStageObject *other;
    VFActor *actor;
};
static_assert(sizeof(VFStageHitProbe) == 0x20);
static_assert(sizeof(VFStageObjectKeyframeCursor) == 0x68);

VFCLTaskData &StageObjectCollision(const VFActor &actor) {
    return *static_cast<VFCLTaskData *>(actor.game->collisionData);
}

const std::uint8_t *StageObjectCollisionMask() {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    return control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr;
}

void SetStageCharacterVisible(Character &character, bool visible) {
    const bool previous = (character.motionFlags & 4U) != 0;
    character.motionFlags = visible ? static_cast<std::uint8_t>(character.motionFlags | 4U) : static_cast<std::uint8_t>(character.motionFlags & ~4U);
    if (g_bDebugLogVisibility != 0 && (character.flags.value & 0x400U) != 0 && previous != visible)
        CHAR_LogNPCVisibility(&character, visible);
}

float StageObjectGroundHeight(Vector3 &position) {
    if (std::fabs(position.y) < kStageObjectEpsilon)
        position.y = 0.0f;
    const float y = position.y;
    position.y = static_cast<float>(static_cast<double>(y) + 1.0);
    const float ground = mapUpdateIntersectionStateAtLocalPoint(position);
    position.y = y;
    return ground;
}

bool StageObjectMovingFast(const Vector3 &velocity) {
    return static_cast<double>(std::fabs(velocity.x)) > 0.02 || static_cast<double>(std::fabs(velocity.y)) > 0.02 ||
        static_cast<double>(std::fabs(velocity.z)) > 0.02;
}

void RotateAroundAxis_PrecomputedTrig(const Vector3 &axis, float sine, float cosine) {
    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;
    const float inverse = 1.0f - cosine;
    const float zx = z * x * inverse;
    const float yx = y * x * inverse;
    const float zy = z * y * inverse;
    Matrix4x4 rotation{};
    rotation.rows[0] = {(1.0f - x * x) * cosine + x * x, z * sine + yx, zx - y * sine, 0.0f};
    rotation.rows[1] = {yx - z * sine, (1.0f - y * y) * cosine + y * y, x * sine + zy, 0.0f};
    rotation.rows[2] = {y * sine + zx, zy - x * sine, (1.0f - z * z) * cosine + z * z, 0.0f};
    rotation.rows[3] = {0.0f, 0.0f, 0.0f, 1.0f};
    Matrix4x4::Multiply(&rotation);
}

void CreateAndApplyRotationMatrix(const Vector3 &from, const Vector3 &to) {
    Vector3 axis{to.z * from.y - to.y * from.z, to.x * from.z - from.x * to.z, from.x * to.y - to.x * from.y};
    const float length = std::sqrt(axis.y * axis.y + axis.x * axis.x + axis.z * axis.z);
    if (length > 0.0f) {
        const float inverse = 1.0f / length;
        axis = {axis.x * inverse, axis.y * inverse, axis.z * inverse};
    }
    float cosine = from.x * to.x + to.y * from.y + from.z * to.z;
    float sine = length;
    if (length == 0.0f) {
        if (cosine >= 0.0f)
            return;
        cosine = -1.0f;
        sine = 0.0f;
        axis = {from.y, from.z, from.x};
    }
    RotateAroundAxis_PrecomputedTrig(axis, sine, cosine);
}

void vfApplyLocalAxisRotationToVector(const VFStageObject &object, Vector3 &vector) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
    Matrix4x4_Push();
    Matrix4x4::RotateMatrixCounterClockwise90();
    const Matrix4x4 &local = workTransformMatrix;
    const float y = local.rows[1].y * vector.y + local.rows[0].y * vector.x + local.rows[2].y * vector.z;
    const float z = local.rows[1].z * vector.y + local.rows[0].z * vector.x + local.rows[2].z * vector.z;
    Matrix4x4::Pop(1);
    const Matrix4x4 &world = workTransformMatrix;
    vector.x = world.rows[1].x * y + world.rows[0].x * 0.0f + world.rows[2].x * z;
    vector.y = world.rows[1].y * y + world.rows[0].y * 0.0f + world.rows[2].y * z;
    vector.z = world.rows[1].z * y + world.rows[0].z * 0.0f + world.rows[2].z * z;
}

void vfUpdateStageObjectWorldPos(VFStageObject &object) {
    const VOBJ &source = *object.object;
    if ((source.entityFlags & (object.state ? 4U : 2U)) == 0)
        return;
    for (std::int32_t index = 0; index < source.entryCount; ++index) {
        const float *const local = source.entries[index].values;
        float *const world = object.worldShapes[index].values;
        if (source.entries[index].type == 1) {
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(object.position.x, object.position.y, object.position.z);
            Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
            const Matrix4x4 &m = workTransformMatrix;
            world[0] = m.rows[1].x * local[1] + m.rows[0].x * local[0] + m.rows[2].x * local[2] + m.rows[3].x;
            world[1] = m.rows[1].y * local[1] + m.rows[0].y * local[0] + m.rows[2].y * local[2] + m.rows[3].y;
            world[2] = m.rows[1].z * local[1] + m.rows[0].z * local[0] + m.rows[2].z * local[2] + m.rows[3].z;
        } else if (source.entries[index].type == 2) {
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(object.position.x, object.position.y, object.position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(object.rotation.y));
            const Matrix4x4 &m = workTransformMatrix;
            world[0] = m.rows[0].x * local[0] + m.rows[1].x * 0.0f + m.rows[2].x * local[1] + m.rows[3].x;
            world[1] = m.rows[0].z * local[0] + m.rows[1].z * 0.0f + m.rows[2].z * local[1] + m.rows[3].z;
            world[2] = m.rows[0].x * local[2] + m.rows[1].x * 0.0f + m.rows[2].x * local[3];
            world[3] = m.rows[0].z * local[2] + m.rows[1].z * 0.0f + m.rows[2].z * local[3];
            world[4] = m.rows[0].x * local[4] + m.rows[1].x * 0.0f + m.rows[2].x * local[5];
            world[5] = m.rows[0].z * local[4] + m.rows[1].z * 0.0f + m.rows[2].z * local[5];
        }
    }
}

void vfclUpdateSeparationFromContacts(VFActor &actor, VFStageObject &object, const VFStageObjectShape &shape) {
    const Vector3 &previous = StageObjectCollision(actor).previousContact[actor.slotIndex];
    const auto *const volumes = reinterpret_cast<const VFContactVolumeRecord *>(actor.contactVolumeRecords);
    float best = std::sqrt(object.velocityX * object.velocityX + object.velocityY * object.velocityY);
    for (std::size_t probe = 0; probe < 10; ++probe) {
        const Vector3 &point = actor.contactProbePositions[probe];
        float dx = shape.values[0] - point.x;
        const float dy = shape.values[1] - point.y;
        float dz = shape.values[2] - point.z;
        float distance = std::sqrt(dy * dy + dx * dx + dz * dz);
        const float penetration = static_cast<float>(static_cast<double>(volumes[probe].radius + shape.values[3] - distance) + 0.01);
        if (0.0f >= penetration)
            continue;
        float pushX = penetration;
        float pushZ = penetration;
        bool scaled = true;
        if (std::fabs(distance) < kStageObjectEpsilon) {
            dx = object.position.x - previous.x;
            dz = object.position.z - previous.z;
            distance = std::sqrt(dz * dz + dx * dx);
            if (std::fabs(distance) < kStageObjectEpsilon) {
                pushX = object.velocityX > 0.0f ? penetration : -penetration;
                pushZ = 0.0f;
                scaled = false;
            }
        }
        if (scaled) {
            pushX = penetration * dx / distance;
            pushZ = penetration * dz / distance;
        }
        const bool accept = 0.0f > pushZ * object.velocityY + pushX * object.velocityX ?
            (object.position.z - previous.z) * pushZ + (object.position.x - previous.x) * pushX > 0.0f :
            penetration > best;
        if (accept) {
            object.velocityY = pushZ;
            best = penetration;
            object.velocityX = pushX;
        }
    }
}

void vfclAdjustMovementByTriangleContact(VFActor &actor, VFStageObject &object, const VFStageObjectShape &shape) {
    const VFCLTaskData &collision = StageObjectCollision(actor);
    const Vector3 &previous = collision.previousContact[actor.slotIndex];
    const Vector3 &current = collision.currentContact[actor.slotIndex];
    QueryShape box{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
    std::memcpy(box.packed, shape.values, sizeof(shape.values));
    CharacterVectorCollisionResult result{.flags = 8U, .fraction = 9.9999999e36f, .angle = 0};
    DispatchVectorOrientationTests(box, result, {current.x, current.z}, {previous.x - current.x, previous.z - current.z}, 0.9f);
    const std::uint32_t hit = result.flags & 7U;
    if (hit == 2) {
        const float moveX = previous.x - current.x;
        const float moveZ = previous.z - current.z;
        const float centerX = static_cast<float>(
            static_cast<double>(shape.values[4] + shape.values[2]) * 0.5 + static_cast<double>(shape.values[0]) - static_cast<double>(current.x));
        const float centerZ = static_cast<float>(
            static_cast<double>(shape.values[3] + shape.values[5]) * 0.5 + static_cast<double>(shape.values[1]) - static_cast<double>(current.z));
        if (moveZ * centerZ + moveX * centerX > 0.0f) {
            object.velocityX = moveX;
            const float scale = 0.00125f / std::sqrt(centerX * centerX + 0.0f + centerZ * centerZ);
            object.velocityY = scale * centerZ + moveZ;
            object.velocityX = scale * centerX + object.velocityX;
        }
        return;
    }
    if (hit != 1 || !(1.0f > result.fraction) || result.fraction < 0.0f)
        return;
    const float dx = current.x - previous.x;
    const float dz = current.z - previous.z;
    const float remaining = static_cast<float>(1.0 - static_cast<double>(result.fraction));
    const auto angle = static_cast<std::uint16_t>(result.angle);
    const std::int32_t folded = angle > 0x8000U ? 0x10000 - angle : angle;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    const float slideX = dx * remaining * cosine;
    const float slideZ = dz * remaining * cosine;
    float pushX = 0.0f;
    float pushZ = 0.0f;
    if (std::fabs(slideX) < kStageObjectEpsilon && std::fabs(slideZ) < kStageObjectEpsilon) {
        pushX = dx * cosine;
        pushZ = dz * cosine;
    } else {
        const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
        const float rotatedX = cosine * slideX - sine * slideZ;
        const float rotatedZ = cosine * slideZ + sine * slideX;
        const std::uint16_t direction = Shenmue_Atan2Fixed(rotatedZ, rotatedX);
        const std::int32_t foldedDirection = direction > 0x8000U ? 0x10000 - direction : direction;
        pushX =
            static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedDirection))) * 0.01 + static_cast<double>(rotatedX));
        pushZ = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(direction))) * 0.01 + static_cast<double>(rotatedZ));
    }
    if (pushZ * pushZ + pushX * pushX > object.velocityY * object.velocityY + object.velocityX * object.velocityX) {
        object.velocityX = -pushX;
        object.velocityY = -pushZ;
    }
}

void vfclProcessStageObjectContacts(VFActor &actor, VFStageObject &object) {
    const VOBJ &source = *object.object;
    for (std::int32_t index = 0; index < source.entryCount; ++index) {
        if (source.entries[index].type == 1) {
            object.worldShapes[index].values[3] = source.entries[index].values[3];
            vfclUpdateSeparationFromContacts(actor, object, object.worldShapes[index]);
        } else if (source.entries[index].type == 2) {
            vfclAdjustMovementByTriangleContact(actor, object, object.worldShapes[index]);
        }
    }
}

bool vfclCheckRayIntersection(VFActor &actor, float x, float z) {
    (void)SelectCharacterMapQuery(actor.character);
    const Vector3 &previous = StageObjectCollision(actor).previousContact[actor.slotIndex];
    QueryShape query{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange};
    query.packed[0] = {0.9f, x + previous.x, z + previous.z};
    return ExecuteMathFnWrapper(&query, StageObjectCollisionMask()) != 0;
}

bool vfclCheckContactWithStageObject(VFStageObject &object, VFActor &actor, bool useObjectVelocity) {
    const Vector3 &point = StageObjectCollision(actor).previousContact[actor.slotIndex];
    const float dx = point.x - object.position.x;
    const float dy = point.y - object.position.y;
    const float dz = point.z - object.position.z;
    if (dy * dy + dx * dx + dz * dz > g_maxDist)
        return false;
    const VOBJ &source = *object.object;
    const Vector3 velocity = useObjectVelocity ? object.localVelocity : Vector3{};
    const auto *const volumes = reinterpret_cast<const VFContactVolumeRecord *>(actor.contactVolumeRecords);
    for (std::int32_t index = 0; index < source.entryCount; ++index) {
        const float *const world = object.worldShapes[index].values;
        if (source.entries[index].type == 1) {
            const float x = velocity.x + world[0];
            const float y = velocity.y + world[1];
            const float z = velocity.z + world[2];
            const float radius = source.entries[index].values[3];
            const float px = point.x - x;
            const float py = point.y - y;
            const float pz = point.z - z;
            if (py * py + px * px + pz * pz <= (radius + 0.9f) * (radius + 0.9f))
                object.environmentFlags |= 1 << actor.slotIndex;
            for (std::size_t probe = 0; probe < 10; ++probe) {
                const Vector3 &probePoint = actor.contactProbePositions[probe];
                const float ex = probePoint.x - x;
                const float ey = probePoint.y - y;
                const float ez = probePoint.z - z;
                const float reach = radius + volumes[probe].radius;
                if (ey * ey + ex * ex + ez * ez <= reach * reach)
                    return true;
            }
        } else if (source.entries[index].type == 2) {
            QueryShape circle{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange};
            circle.packed[0] = {0.9f, point.x, point.z};
            QueryShape box{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
            box.packed[0] = {velocity.x + world[0], velocity.z + world[1], world[2]};
            box.packed[1] = {world[3], world[4], world[5]};
            if (CheckVectorWithinConditions(circle, box)) {
                object.environmentFlags |= 1 << actor.slotIndex;
                return true;
            }
        }
    }
    return false;
}


bool vfStageObjectBuildKeyframeCursors(VFStageObject &object) {
    const VOBJ &source = *object.object;
    auto *const cursors = static_cast<VFStageObjectKeyframeCursor *>(HLib_AllocHeapMemBlock(
        static_cast<std::int64_t>(sizeof(VFStageObjectKeyframeCursor)) * source.animatedNodeCount, static_cast<std::int32_t>(kVobjToken)));
    if (!cursors)
        return true;
    object.keyframeCursors = cursors;
    auto *const model = static_cast<Mt5RuntimeNode *>(object.linkedObject->model);
    const std::int32_t *entry = source.keyframeData;
    const std::int16_t *kind = source.animatedNodeKinds;
    for (std::int32_t index = 0; index < source.animatedNodeCount; ++index, kind += 2) {
        Mt5RuntimeNode *node = nullptr;
        if (model && static_cast<std::int16_t>(model->nodeKind) == *kind) {
            node = model;
        } else if (model) {
            std::byte *const base = g_pCurrentTaskData;
            if (model->childOffset)
                node = shenGetNodeByChildIdx(reinterpret_cast<Mt5RuntimeNode *>(base + model->childOffset), *kind);
            if (!node && model->siblingOffset)
                node = shenGetNodeByChildIdx(reinterpret_cast<Mt5RuntimeNode *>(base + model->siblingOffset), *kind);
        }
        VFStageObjectKeyframeCursor &cursor = cursors[index];
        cursor.node = node;
        const std::int32_t *channel = entry + 1;
        for (VFStageObjectKeyframeCursor::Channel &track : cursor.channels) {
            const std::int32_t count = channel[0];
            const std::int32_t *const keys = channel + 1;
            std::int32_t key = 1;
            while (key < count && source.startFrame >= keys[2 * key])
                ++key;
            track = {.keys = keys, .cursor = static_cast<std::int16_t>(key), .count = static_cast<std::int16_t>(count)};
            channel = keys + 2 * count;
        }
        entry = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::byte *>(entry) + entry[0]);
    }
    return false;
}


void vfStageObjectBreak(VFStageObject &object) {
    if (vfStageObjectBuildKeyframeCursors(object))
        return;
    object.stateFlags &= ~0x62U;
    const VOBJ &source = *object.object;
    object.state = 1;
    object.frameIndex = source.startFrame;
    if (source.reactionSoundCommand)
        Execute3DSoundCommand(source.reactionSoundCommand, 4, &object.position, 2.0f, 20.0f, 0);
    if ((object.stateFlags & 1U) != 0)
        ResetStageObjectMotion(*g_vfStageObjs[object.objectIndex]);
    if (object.linkedObject)
        SetStageCharacterVisible(*object.linkedObject, true);
    if (object.otherCharacter2)
        SetStageCharacterVisible(*object.otherCharacter2, false);
    object.otherCharacter = object.linkedObject;
    if (object.otherCharacter)
        shenApplyTranslationToAll(object.otherCharacter->entity.node, object.position);
    if (object.otherCharacter)
        ApplyEulerAnglesToNode(object.otherCharacter->entity.node, object.rotation);
    EFFE_SpawnVFFightGroundEffect(5, nullptr, object.otherCharacter);
    object.angle = {};
    object.stateFlags &= ~0x60U;
    object.contactSlotMask = 0;
    object.environmentFlags = 0;
    object.localVelocity.x = 0.0f;
    object.localVelocity.z = 0.0f;
}

void vfProcessHitOutcome(VFStageObject &object, VFActor &self, std::uint8_t guardState, std::int16_t hitParameter, std::int8_t guardStrength) {
    VFMAData &game = *self.game;
    g_vfHitObjectHandled |= 1 << self.slotIndex;
    VFActor *owner = nullptr;
    if (Character *const character = object.character) {
        for (std::size_t slot = 0; slot < kBattleSlots; ++slot) {
            if (game.characters[slot] == character) {
                owner = game.actors[slot];
                break;
            }
        }
    }
    if ((object.stateFlags & 1U) != 0 && owner && (object.flags & 0x4000U) != 0) {
        const std::int8_t guardLevel = owner->guardLevel;
        owner->guardLevel = self.guardLevel;
        VFCL_ApplyImpact(*owner, self);
        owner->guardLevel = guardLevel;
        return;
    }
    VFActor attacker{};
    VFActor *const hitSource = self.hitSourceActor;
    attacker.game = &game;
    VFActor *const target = self.targetActor;
    self.targetActor = &attacker;
    self.hitSourceActor = &attacker;
    attacker.targetActor = &self;
    attacker.hitSourceActor = &self;
    attacker.hitStateFlags2 = 0x100U;
    attacker.fighterRankId = 80;
    attacker.fighterIdAlt = 80;
    if (guardState == 0xffU) {
        const float ground = StageObjectGroundHeight(object.position);
        const float drop = object.position.y - ground;
        guardState = 0.6 > static_cast<double>(drop) ? 2U : (static_cast<double>(drop) <= 1.2 ? 1U : 0U);
    }
    std::uint16_t parameter = attacker.hitParameter;
    if (static_cast<std::uint16_t>(hitParameter - 8) <= 7U) {
        parameter = static_cast<std::uint16_t>(1U << static_cast<std::uint8_t>(hitParameter));
        attacker.hitParameter = parameter;
    }
    attacker.guardState = guardState;
    if (guardState == 0)
        attacker.hitParameter = parameter | 0x1eU;
    else if (guardState == 1)
        attacker.hitParameter = parameter | 0x1fU;
    else if (guardState == 2)
        attacker.hitParameter = parameter | 0x20U;
    attacker.hitDirectionAngle = Shenmue_Atan2Fixed(object.position.x - self.contactCurrentPosition.x, object.position.z - self.contactCurrentPosition.z);
    attacker.guardStrength = static_cast<std::uint8_t>(guardStrength);
    attacker.postReactionAnimationId = 284;
    attacker.guardLevel = 100;
    VFCL_ApplyImpact(attacker, self);
    self.hitSourceActor = hitSource;
    self.targetActor = target;
}

void vfclFilterObjectHit(VFActor &actor, VFStageObject &object) {
    if (((g_vfHitObjectHandled >> actor.slotIndex) & 1) != 0)
        return;
    const std::uint32_t hitState = actor.hitStateFlags2;
    if ((hitState & 0x10064000U) != 0 || (actor.moveFlags & 0x40000U) != 0 ||
        ((hitState & 0x80000U) != 0 && (actor.hitSourceActor->hitStateFlags2 & 0x40000U) != 0)) {
        return;
    }
    Character *const owner = object.character;
    if (owner == actor.character)
        return;
    const VOBJ &source = *object.object;
    if ((object.stateFlags & 1U) != 0 && (object.flags & 0x4000U) != 0) {
        if ((actor.motionFlags & 0x2000000U) != 0 || !owner)
            return;
        std::size_t slot = 0;
        while (slot < kBattleSlots && actor.game->characters[slot] != owner)
            ++slot;
        if (slot == kBattleSlots)
            return;
        const VFActor *const thrower = actor.game->actors[slot];
        if (!thrower || (thrower->hitStateFlags2 & 0x102U) != 0x100U || thrower->moveId >= kVFMoveCategoryMasks.size())
            return;
        const std::uint32_t category = kVFMoveCategoryMasks[thrower->moveId];
        if (source.boneId == 18 ? (category & 0x10U) == 0 : source.boneId != 12 || (category & 0x80U) == 0)
            return;
    } else if ((object.flags & 0x2000U) == 0) {
        if ((object.flags & 0x1000U) == 0 || !StageObjectMovingFast(object.localVelocity))
            return;
        if (!((actor.contactCurrentPosition.z - object.position.z) * object.localVelocity.z +
                    (actor.contactCurrentPosition.x - object.position.x) * object.localVelocity.x >
                0.0f)) {
            return;
        }
    }
    vfProcessHitOutcome(object, actor, static_cast<std::uint8_t>(source.hitReactionGuardType), source.hitParameter, source.hitGuardStrength);
    if (source.hitReactionSoundCommand)
        Execute3DSoundCommand(static_cast<std::uint32_t>(source.hitReactionSoundCommand), 4, &object.position, 2.0f, 20.0f, 0);
}

void CalculateEulerAnglesFromRotationMatrix(VFStageObject &object, const Vector3 &axis) {
    Vector3 up{axis.x, 0.0f, axis.z};
    const float length = std::sqrt(axis.x * axis.x + 0.0f + axis.z * axis.z);
    if (length > 0.0f) {
        const float inverse = 1.0f / length;
        up = {axis.x * inverse, inverse * 0.0f, axis.z * inverse};
    }
    workTransformMatrix = transformMatrix_prePass;
    CreateAndApplyRotationMatrix(axis, up);
    Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
    object.rotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
}

void CalculateAndSetEulerAnglesFromRotationMatrix(VFStageObject &object) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
    const std::uint32_t flags = object.object->entityFlags;
    const Matrix4x4 &m = workTransformMatrix;
    Vector3 axis{};
    if ((flags & 0x1c00U) == 0) {
        float a = 0.0f;
        float b = 0.0f;
        float c = 0.0f;
        if ((flags & 0x2000U) != 0)
            a = 1.0f;
        else if ((flags & 0x4000U) != 0)
            b = 1.0f;
        else
            c = 1.0f;
        axis = {m.rows[1].x * b + m.rows[0].x * a + m.rows[2].x * c, m.rows[1].y * b + m.rows[0].y * a + m.rows[2].y * c,
            m.rows[1].z * b + m.rows[0].z * a + m.rows[2].z * c};
        if (axis.y < 1.0f && axis.y > -1.0f) {
            workTransformMatrix = transformMatrix_prePass;
            CreateAndApplyRotationMatrix(axis, {0.0f, 1.0f, 0.0f});
            Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
            object.rotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
        }
        return;
    }
    if ((flags & 0x400U) != 0) {
        axis = {m.rows[1].x * 0.0f + m.rows[0].x + m.rows[2].x * 0.0f, m.rows[1].y * 0.0f + m.rows[0].y + m.rows[2].y * 0.0f,
            m.rows[1].z * 0.0f + m.rows[0].z + m.rows[2].z * 0.0f};
    } else if ((flags & 0x800U) != 0) {
        axis = {m.rows[0].x * 0.0f + m.rows[1].x + m.rows[2].x * 0.0f, m.rows[0].y * 0.0f + m.rows[1].y + m.rows[2].y * 0.0f,
            m.rows[0].z * 0.0f + m.rows[1].z + m.rows[2].z * 0.0f};
        if (axis.y >= 1.0f || axis.y <= -1.0f) {
            object.rotation = {0x4000, 0, 0};
            return;
        }
        CalculateEulerAnglesFromRotationMatrix(object, axis);
        return;
    } else {
        axis = {m.rows[0].x * 0.0f + m.rows[1].x * 0.0f + m.rows[2].x, m.rows[0].y * 0.0f + m.rows[1].y * 0.0f + m.rows[2].y,
            m.rows[0].z * 0.0f + m.rows[1].z * 0.0f + m.rows[2].z};
    }
    if (axis.y < 1.0f && axis.y > -1.0f)
        CalculateEulerAnglesFromRotationMatrix(object, axis);
    else
        object.rotation = {};
}

void vfCalculateAndTransformPoint(VFStageObject &object, VFStageHitProbe &probe) {
    const VOBJ &source = *object.object;
    object.position.x = object.position.x + object.velocityX;
    object.position.z = object.velocityY + object.position.z;
    const float floor = StageObjectGroundHeight(object.position) + source.groundOffset;
    const float y = object.position.y;
    if (floor < y) {
        Vector3 ahead{object.localVelocity.x + object.position.x, 0.0f, object.localVelocity.z + object.position.z};
        ahead.y = static_cast<float>(static_cast<double>(y) + 10.0);
        float aheadFloor = StageObjectGroundHeight(ahead) + source.groundOffset;
        const float currentY = object.position.y;
        if (aheadFloor > currentY) {
            object.localVelocity.x = 0.0f;
            aheadFloor = floor;
            object.localVelocity.z = 0.0f;
        }
        const float fall = static_cast<float>(static_cast<double>(object.localVelocity.y) - 0.03266666666666666);
        object.localVelocity.y = fall;
        if (aheadFloor >= fall + currentY)
            object.localVelocity.y = aheadFloor - currentY;
        if ((object.stateFlags & 2U) != 0 && (source.entityFlags & 0x380U) != 0) {
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
            if ((source.entityFlags & 0x80U) != 0)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(source.axisRotateStep));
            else if ((source.entityFlags & 0x100U) != 0)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(source.axisRotateStep));
            else
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(source.axisRotateStep));
            object.rotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
        }
    } else {
        if ((object.stateFlags & 0x10U) != 0) {
            if ((object.object->entityFlags & 0x40U) != 0)
                vfApplyLocalAxisRotationToVector(object, object.localVelocity);
            object.localVelocity.x = static_cast<float>(static_cast<double>(object.localVelocity.x) * 0.6);
            object.localVelocity.z = static_cast<float>(static_cast<double>(object.localVelocity.z) * 0.6);
            if ((source.entityFlags & 0xfc00U) != 0)
                CalculateAndSetEulerAnglesFromRotationMatrix(object);
            probe.hitState |= 0x10U;
        }
        const float velocityX = object.localVelocity.x;
        if (std::fabs(velocityX) < kStageObjectEpsilon && std::fabs(object.localVelocity.z) < kStageObjectEpsilon &&
            std::fabs(object.velocityX) < kStageObjectEpsilon && std::fabs(object.velocityY) < kStageObjectEpsilon) {
            object.localVelocity.y = 0.0f;
            if (floor <= object.position.y)
                return;
            object.position.y = floor;
            probe.hitState |= 0x10U;
            return;
        }
        if ((object.flags & 0x1000000U) != 0) {
            object.localVelocity.x = 0.0f;
            object.localVelocity.z = 0.0f;
            object.localVelocity.y = floor - object.position.y;
        } else {
            const float currentY = object.position.y;
            object.localVelocity.y = floor - currentY;
            object.localVelocity.x = static_cast<float>(static_cast<double>(velocityX) * 0.92);
            object.localVelocity.z = static_cast<float>(static_cast<double>(object.localVelocity.z) * 0.92);
            Vector3 ahead{object.localVelocity.x + object.position.x, static_cast<float>(static_cast<double>(currentY) + 10.0),
                object.localVelocity.z + object.position.z};
            if (StageObjectGroundHeight(ahead) + source.groundOffset > floor) {
                object.localVelocity.x = 0.0f;
                object.localVelocity.z = 0.0f;
            }
        }
    }
    const float moveX = object.localVelocity.x;
    const float moveZ = object.localVelocity.z;
    object.position.x = moveX + object.position.x;
    object.position.y = object.position.y + object.localVelocity.y;
    object.position.z = moveZ + object.position.z;
    if (static_cast<double>(std::fabs(moveX)) < 0.01 && static_cast<double>(std::fabs(moveZ)) < 0.01) {
        object.localVelocity.x = 0.0f;
        object.localVelocity.z = 0.0f;
    }
    probe.hitState |= 0x10U;
}

void vfCheckStageObjectCollisionHitProbe(VFStageObject &object, VFStageHitProbe &probe) {
    (void)SelectCharacterMapQuery(object.otherCharacter);
    const VOBJ &source = *object.object;
    QueryShape query{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange};
    for (std::int32_t index = 0; index < source.entryCount; ++index) {
        const float *const world = object.worldShapes[index].values;
        if (source.entries[index].type == 1) {
            query.packed[0] = {source.entries[index].values[3], world[0] + object.localVelocity.x, world[2] + object.localVelocity.z};
        } else if (source.entries[index].type == 2) {
            const float edgeA = world[2] * world[2] + world[3] * world[3];
            const float edgeB = world[5] * world[5] + world[4] * world[4];
            const float radius = static_cast<float>(static_cast<double>(std::sqrt(edgeA > edgeB ? edgeA : edgeB)) * 0.5);
            const float x = static_cast<float>(static_cast<double>(world[0] + object.localVelocity.x) + static_cast<double>(world[2] + world[4]) * 0.5);
            const float z = static_cast<float>(static_cast<double>(world[1] + object.localVelocity.z) + static_cast<double>(world[3] + world[5]) * 0.5);
            query.packed[0] = {radius, x, z};
        }
        if (ExecuteMathFnWrapper(&query, StageObjectCollisionMask())) {
            probe.hitState |= 2U;
            return;
        }
    }
}

void vfCheckStageObjectProximityCollision(VFStageObject &object, VFStageHitProbe &probe) {
    const float limit = g_maxDist;
    const VOBJ &source = *object.object;
    for (VFStageObject *other = g_vfObjectRoot; other; other = other->next) {
        const VOBJ &otherSource = *other->object;
        if ((otherSource.entityFlags & (other->state ? 4U : 2U)) == 0 || other == &object)
            continue;
        const float dx = other->position.x - object.position.x;
        const float dy = other->position.y - object.position.y;
        const float dz = other->position.z - object.position.z;
        if (dy * dy + dx * dx + dz * dz > limit)
            continue;
        for (std::int32_t otherIndex = 0; otherIndex < otherSource.entryCount; ++otherIndex) {
            const float *const ow = other->worldShapes[otherIndex].values;
            for (std::int32_t index = 0; index < source.entryCount; ++index) {
                const float *const sw = object.worldShapes[index].values;
                const float selfRadius = source.entries[index].values[3];
                bool hit = false;
                if (otherSource.entries[otherIndex].type == 1) {
                    const float otherRadius = otherSource.entries[otherIndex].values[3];
                    if (source.entries[index].type == 1) {
                        const float ex = ow[0] - (object.localVelocity.x + sw[0]);
                        const float ey = ow[1] - (object.localVelocity.y + sw[1]);
                        const float ez = ow[2] - (object.localVelocity.z + sw[2]);
                        const float reach = selfRadius + otherRadius;
                        hit = ey * ey + ex * ex + ez * ez <= reach * reach;
                    } else {
                        QueryShape circle{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange};
                        circle.packed[0] = {otherRadius, ow[0], ow[2]};
                        QueryShape box{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
                        box.packed[0] = {object.localVelocity.x + sw[0], object.localVelocity.z + sw[1], sw[2]};
                        box.packed[1] = {sw[3], sw[4], sw[5]};
                        hit = CheckVectorWithinConditions(circle, box);
                    }
                } else if (source.entries[index].type == 1) {
                    QueryShape circle{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange};
                    circle.packed[0] = {selfRadius, object.localVelocity.x + sw[0], sw[2] + object.localVelocity.z};
                    QueryShape box{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
                    box.packed[0] = {ow[0], ow[1], ow[2]};
                    box.packed[1] = {ow[3], ow[4], ow[5]};
                    hit = CheckVectorWithinConditions(circle, box);
                } else {
                    QueryShape record{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
                    record.packed[0] = {ow[0], ow[1], ow[2]};
                    record.packed[1] = {ow[3], ow[4], ow[5]};
                    QueryShape box{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
                    box.packed[0] = {object.localVelocity.x + sw[0], sw[1] + object.localVelocity.z, sw[2]};
                    box.packed[1] = {sw[3], sw[4], sw[5]};
                    hit = CheckVectorWithinConditions(record, box);
                }
                if (hit) {
                    probe.hitState |= 8U;
                    probe.other = other;
                    return;
                }
            }
        }
    }
}

void vfCheckRingParticipantContact(VFStageObject &object, VFStageHitProbe &probe, VFMAData &game) {
    for (std::size_t slot = 0; slot < kBattleSlots; ++slot) {
        if (!game.characters[slot])
            continue;
        VFActor *const actor = game.actors[slot];
        if (((object.stateFlags & 1U) == 0 || object.character != actor->character) && vfclCheckContactWithStageObject(object, *actor, true)) {
            probe.hitState |= 4U;
            probe.actor = actor;
            return;
        }
    }
}


void SnapStageObjectTowardImpactAndBreak(VFStageObject &object, float x, float z) {
    if ((std::fabs(x) >= kStageObjectEpsilon || std::fabs(z) >= kStageObjectEpsilon) && (object.object->entityFlags & 8U) != 0) {
        const std::int32_t yaw = object.rotation.y;
        const auto relative = static_cast<std::uint16_t>(0x8000 - yaw + Shenmue_Atan2Fixed(x, z));
        if (relative > 0x2000U) {
            if (relative <= 0x6000U)
                object.rotation.y = yaw + 0x4000;
            else if (relative <= 0xa000U)
                object.rotation.y = yaw + 0x8000;
            else if (relative <= 0xe000U)
                object.rotation.y = yaw + 0xc000;
            object.rotation.y = static_cast<std::uint16_t>(object.rotation.y);
        }
    }
    vfStageObjectBreak(object);
}

void vfResolveStageObjectCollisionResponse(VFStageObject &object, VFMAData &game, VFStageHitProbe &probe) {
    if ((object.object->entityFlags & (object.state ? 4U : 2U)) == 0)
        return;
    vfCheckStageObjectCollisionHitProbe(object, probe);
    if ((probe.hitState & 3U) != 0) {
        if (!object.state && (object.flags & 0x100U) != 0 &&
            ((object.flags & 0x20U) != 0 || ((object.flags & 0x10U) != 0 && StageObjectMovingFast(object.localVelocity)))) {
            SnapStageObjectTowardImpactAndBreak(object, -object.localVelocity.x, -object.localVelocity.z);
        }
        object.stateFlags &= ~2U;
        object.angle = {};
        const float velocityX = object.localVelocity.x;
        const float velocityZ = object.localVelocity.z;
        object.stateFlags &= ~0x60U;
        object.position.x = probe.position.x;
        object.position.z = probe.position.z;
        object.velocityX = 0.0f;
        object.velocityY = 0.0f;
        object.localVelocity.x = static_cast<float>(static_cast<double>(velocityX) * -0.4);
        object.localVelocity.z = static_cast<float>(static_cast<double>(velocityZ) * -0.4);
        return;
    }
    vfCheckStageObjectProximityCollision(object, probe);
    if ((probe.hitState & 8U) == 0) {
        vfCheckRingParticipantContact(object, probe, game);
        if ((probe.hitState & 4U) == 0) {
            object.velocityX = 0.0f;
            object.velocityY = 0.0f;
            return;
        }
        VFActor &actor = *probe.actor;
        vfclFilterObjectHit(actor, object);
        if (!object.state && (object.flags & 0x40U) != 0) {
            const std::uint32_t hitState = actor.hitStateFlags2;
            if ((object.flags & 0x20U) != 0 || ((object.flags & 0x10U) != 0 && StageObjectMovingFast(object.localVelocity)) ||
                ((object.flags & 0x200U) != 0 && ((hitState & 0x80000U) != 0 || ((hitState & 0x10U) != 0 && (hitState & 0x4000U) != 0)))) {
                SnapStageObjectTowardImpactAndBreak(object, -object.localVelocity.x, -object.localVelocity.z);
            }
        }
        if ((object.object->entityFlags & (object.state ? 4U : 2U)) != 0) {
            object.contactSlotMask |= 1 << actor.slotIndex;
            actor.stateFlags |= 0x20U;
        }
        object.stateFlags &= ~2U;
        object.angle = {};
        const float velocityX = object.localVelocity.x;
        const float velocityZ = object.localVelocity.z;
        object.stateFlags &= ~0x60U;
        object.position.x = probe.position.x;
        object.position.z = probe.position.z;
        object.localVelocity.x = static_cast<float>(static_cast<double>(velocityX) * -0.5);
        object.localVelocity.z = static_cast<float>(static_cast<double>(velocityZ) * -0.5);
        object.velocityX = 0.0f;
        object.velocityY = 0.0f;
        return;
    }
    VFStageObject &other = *probe.other;
    if (!object.state && (object.flags & 0x80U) != 0 &&
        ((object.flags & 0x20U) != 0 ||
            ((object.flags & 0x10U) != 0 && StageObjectMovingFast(object.localVelocity) &&
                (other.position.z - probe.position.z) * object.localVelocity.z + (other.position.x - probe.position.x) * object.localVelocity.x > 0.0f))) {
        SnapStageObjectTowardImpactAndBreak(object, -object.localVelocity.x, -object.localVelocity.z);
    }
    object.stateFlags &= ~2U;
    object.angle = {};
    object.stateFlags &= ~0x60U;
    object.position.x = probe.position.x;
    object.position.z = probe.position.z;
    if (!other.state && (other.flags & 0x80U) != 0 &&
        ((other.flags & 0x20U) != 0 || ((other.flags & 0x10U) != 0 && StageObjectMovingFast(object.localVelocity)))) {
        SnapStageObjectTowardImpactAndBreak(other, object.localVelocity.x, object.localVelocity.z);
        other.stateFlags &= ~2U;
        other.velocityX = 0.0f;
        other.velocityY = 0.0f;
        other.angle = {};
        other.stateFlags &= 0xffffff9fU;
        const float velocityZ = object.localVelocity.z;
        const float velocityX = object.localVelocity.x;
        object.velocityX = 0.0f;
        object.velocityY = 0.0f;
        object.localVelocity.x = static_cast<float>(static_cast<double>(velocityX) * 0.5);
        object.localVelocity.z = static_cast<float>(static_cast<double>(velocityZ) * 0.5);
        return;
    }
    other.stateFlags &= ~2U;
    other.angle = {};
    other.stateFlags &= 0xffffff9fU;
    if ((other.flags & 1U) != 0 && (other.stateFlags & 1U) == 0) {
        other.localVelocity.x = object.localVelocity.x + other.localVelocity.x;
        other.localVelocity.z = object.localVelocity.z + other.localVelocity.z;
        other.velocityX = object.velocityX + other.velocityX;
        other.velocityY = object.velocityY + other.velocityY;
        if ((other.object->entityFlags & 0x40U) != 0) {
            vfApplyLocalAxisRotationToVector(other, other.localVelocity);
            Vector3 slide{other.velocityX, 0.0f, other.velocityY};
            vfApplyLocalAxisRotationToVector(other, slide);
            other.velocityX = slide.x;
            other.velocityY = slide.z;
        }
    }
    object.velocityY = 0.0f;
    object.localVelocity.x = 0.0f;
    object.localVelocity.z = 0.0f;
    object.velocityX = 0.0f;
}

void vfUpdateRotationAnglesFromStageObject(VFStageObject &object, VFMAData &game) {
    Character *const character = object.character;
    if (character) {
        for (std::size_t slot = 0; slot < kBattleSlots; ++slot) {
            if (game.characters[slot] != character)
                continue;
            const VFActor *const actor = game.actors[slot];
            if (actor && ((((actor->hitStateFlags2 & 0x4000U) != 0 && (object.flags & 0x400000U) == 0)) || (actor->moveFlags & 0x20U) != 0)) {
                ResetStageObjectMotion(*g_vfStageObjs[object.objectIndex]);
                return;
            }
            break;
        }
    }
    const VOBJ &source = *object.object;
    if ((object.flags & 0x800000U) == 0) {
        object.position = source.position;
        object.rotation = source.rotation;
        Matrix4x4_TransformMatrixAndCalculateEulerAngles(
            GetTransformForFACE_ForSpecificChara(character, static_cast<std::int32_t>(source.boneId)), object.position, object.rotation);
        return;
    }
    workTransformMatrix = transformMatrix_prePass;
    Vector3 bone{};
    mtwkGetCharaBonePosition(object.character, source.boneId, &bone);
    Matrix4x4::preTranslate(bone.x, bone.y, bone.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(charaGetWorldYaw(object.character)));
    Matrix4x4::preTranslate(source.position.x, source.position.y, source.position.z);
    Matrix4x4::SetViewMatrixFromEuler(source.rotation.y, source.rotation.x, source.rotation.z);
    if ((source.entityFlags & 0x40U) == 0) {
        object.position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        object.rotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
        return;
    }
    if ((object.stateFlags & 0x200U) != 0) {
        const float cosine = sin_fixed16(-16384);
        Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(0x8000)), cosine);
    }
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(object.field38));
    const Matrix4x4 &m = workTransformMatrix;
    object.position = {m.rows[3].x, m.rows[3].y, m.rows[3].z};
    object.rotation.z = 0;
    const float horizontal = m.rows[1].x * m.rows[1].x + m.rows[1].z * m.rows[1].z;
    const float sine = m.rows[1].z * m.rows[0].x - m.rows[1].x * m.rows[0].z >= 0.0f ? std::sqrt(horizontal) : -std::sqrt(horizontal);
    object.rotation.x = Shenmue_Atan2Fixed(sine, m.rows[1].y);
    object.rotation.y = -static_cast<std::int32_t>(Shenmue_Atan2Fixed(m.rows[0].z, m.rows[0].x));
}

void vfCreateRotationMatrix(VFStageObject &object, const Vector3 &direction) {
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    const float amount = static_cast<float>(object.object->rotationAmount);
    workTransformMatrix = transformMatrix_prePass;
    const auto pitch = static_cast<std::uint16_t>(static_cast<std::int32_t>(length * amount));
    Matrix4x4::RotateAroundY_Wrapper(Shenmue_Atan2Fixed(direction.x, direction.z));
    Matrix4x4::RotateAroundX_Wrapper(pitch);
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 up{m.rows[0].x * 0.0f + m.rows[1].x + m.rows[2].x * 0.0f, m.rows[0].y * 0.0f + m.rows[1].y + m.rows[2].y * 0.0f,
        m.rows[0].z * 0.0f + m.rows[1].z + m.rows[2].z * 0.0f};
    workTransformMatrix = transformMatrix_prePass;
    CreateAndApplyRotationMatrix({0.0f, 1.0f, 0.0f}, up);
    const Vector3i angle = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
    Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
    object.rotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
    object.angle = angle;
    object.stateFlags |= 0x20U;
}

void vfAdjustStageObjectRotationFromVector(VFStageObject &object, const Vector3 &direction) {
    const float amount =
        std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z) * static_cast<float>(object.object->rotationAmount);
    workTransformMatrix = transformMatrix_prePass;
    std::int32_t pitch = static_cast<std::uint16_t>(static_cast<std::int32_t>(amount));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-object.rotation.y));
    const Matrix4x4 &yawed = workTransformMatrix;
    if (yawed.rows[1].z * direction.y + yawed.rows[0].z * direction.x + yawed.rows[2].z * direction.z < 0.0f)
        pitch = -pitch;
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(pitch));
    object.angle = {pitch, 0, 0};
    object.stateFlags |= 0x40U;
    const Matrix4x4 &m = workTransformMatrix;
    object.rotation.z = 0;
    const float horizontal = m.rows[1].x * m.rows[1].x + m.rows[1].z * m.rows[1].z;
    const float sine = m.rows[1].z * m.rows[0].x - m.rows[1].x * m.rows[0].z >= 0.0f ? std::sqrt(horizontal) : -std::sqrt(horizontal);
    object.rotation.x = Shenmue_Atan2Fixed(sine, m.rows[1].y);
    object.rotation.y = -static_cast<std::int32_t>(Shenmue_Atan2Fixed(m.rows[0].z, m.rows[0].x));
}

void vfRecalcLocalEulerFromWorld(VFStageObject &object) {
    workTransformMatrix = transformMatrix_prePass;
    if ((object.stateFlags & 0x20U) != 0) {
        Matrix4x4::SetViewMatrixFromEuler(object.angle.y, object.angle.x, object.angle.z);
        Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
    } else {
        Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
        Matrix4x4::SetViewMatrixFromEuler(object.angle.y, object.angle.x, object.angle.z);
    }
    object.rotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
}

void vfclResolveProbeContacts(VFStageObject &object, VFMAData &game, VFStageHitProbe &probe) {
    if ((object.stateFlags & 1U) != 0) {
        vfUpdateRotationAnglesFromStageObject(object, game);
        probe.hitState |= 0x10U;
    } else {
        vfCalculateAndTransformPoint(object, probe);
    }
    if ((object.stateFlags & 1U) == 0 && (probe.hitState & 0x10U) != 0)
        vfResolveStageObjectCollisionResponse(object, game, probe);
    const float ground = StageObjectGroundHeight(object.position);
    const VOBJ &source = *object.object;
    if (ground + source.groundOffset < object.position.y) {
        object.stateFlags &= ~0x18U;
    } else if ((object.stateFlags & 8U) != 0) {
        object.stateFlags &= ~0x10U;
    } else {
        if (-0.03919999999999999 > static_cast<double>(object.localVelocity.y) && source.landingSoundCommand)
            Execute3DSoundCommand(static_cast<std::uint32_t>(source.landingSoundCommand), 4, &object.position, 2.0f, 20.0f, 0);
        object.stateFlags = (object.stateFlags & ~2U) | 0x18U;
    }
}

void vfclCheckAllIntersectionsWithStageObjects(VFMAData &game) {
    VFCLTaskData &collision = *static_cast<VFCLTaskData *>(game.collisionData);
    for (std::size_t slot = 0; slot < kBattleSlots; ++slot) {
        if (!game.characters[slot])
            continue;
        VFActor &actor = *game.actors[slot];
        for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
            if ((object->object->entityFlags & (object->state ? 4U : 2U)) == 0)
                continue;
            if ((object->stateFlags & 1U) != 0 && (object->character == actor.character || (actor.hitStateFlags2 & 0xc4000U) != 0))
                continue;
            if (!vfclCheckContactWithStageObject(*object, actor, false))
                continue;
            vfclFilterObjectHit(actor, *object);
            if (!object->state && (object->flags & 0x40U) != 0) {
                const std::uint32_t flags = object->flags;
                const std::uint32_t hitState = actor.hitStateFlags2;
                const bool movingToward = (flags & 0x10U) != 0 && StageObjectMovingFast(object->localVelocity) &&
                    (actor.contactCurrentPosition.z - object->position.z) * object->localVelocity.z +
                            (actor.contactCurrentPosition.x - object->position.x) * object->localVelocity.x >
                        0.0f;
                if ((flags & 0x20U) != 0 || movingToward || ((flags & 0x200U) != 0 && ((hitState & 0x80000U) != 0 || (hitState & 0x4010U) == 0x4010U))) {
                    vfStageObjectBreak(*object);
                    if ((object->object->entityFlags & (object->state ? 4U : 2U)) == 0)
                        continue;
                }
            }
            object->contactSlotMask |= 1 << actor.slotIndex;
            actor.stateFlags |= 0x20U;
            vfclProcessStageObjectContacts(actor, *object);
            float slideX = 0.0f;
            float slideZ = 0.0f;
            if ((object->flags & 1U) == 0 || (object->stateFlags & 1U) != 0) {
                slideX = -object->velocityX;
                slideZ = -object->velocityY;
                object->velocityX = 0.0f;
                object->velocityY = 0.0f;
            } else {
                const float velocityY = object->velocityY;
                const float damping = static_cast<float>(1.0 - static_cast<double>(object->object->timeScale) * 0.045);
                const float keep = 0.0f > damping ? 0.0f : damping;
                const double transfer = 1.0 - static_cast<double>(keep);
                slideX = static_cast<float>(static_cast<double>(-object->velocityX) * transfer);
                slideZ = static_cast<float>(static_cast<double>(-velocityY) * transfer);
                if ((object->object->entityFlags & 0x40U) != 0) {
                    Vector3 slide{object->velocityX, 0.0f, velocityY};
                    vfApplyLocalAxisRotationToVector(*object, slide);
                    object->velocityX = slide.x * keep;
                    object->velocityY = slide.z * keep;
                } else {
                    object->velocityX = object->velocityX * keep;
                    object->velocityY = velocityY * keep;
                }
            }
            if (vfclCheckRayIntersection(actor, slideX, slideZ)) {
                vfclMoveEntityWithContactHistory(collision, actor, collision.currentContact[slot].x - collision.previousContact[slot].x,
                    collision.currentContact[slot].z - collision.previousContact[slot].z);
                break;
            }
            vfclMoveEntityWithContactHistory(collision, actor, slideX, slideZ);
        }
    }
}

void vfclUpdateStageObjectHitboxesPhysics(VFMAData &game) {
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
        if ((object->flags & 0x700003U) == 0)
            continue;
        const VOBJ &source = *object->object;
        VFStageHitProbe probe{.hitState = 0, .position = object->position, .other = nullptr, .actor = nullptr};
        vfclResolveProbeContacts(*object, game, probe);
        if ((probe.hitState & 0x10U) == 0)
            continue;
        const Vector3 delta{object->position.x - probe.position.x, object->position.y - probe.position.y, object->position.z - probe.position.z};
        if (std::fabs(delta.x) >= kStageObjectEpsilon || std::fabs(delta.z) >= kStageObjectEpsilon) {
            if ((object->stateFlags & 8U) != 0) {
                if ((source.entityFlags & 0x20U) != 0)
                    vfCreateRotationMatrix(*object, delta);
                else if ((source.entityFlags & 0x40U) != 0)
                    vfAdjustStageObjectRotationFromVector(*object, delta);
            }
        } else if (std::fabs(delta.y) < kStageObjectEpsilon) {
            object->angle = {};
            object->stateFlags &= ~0x60U;
        }
        if ((object->stateFlags & 0x60U) != 0 && ((source.entityFlags & 0x60U) == 0 || (object->stateFlags & 8U) == 0))
            vfRecalcLocalEulerFromWorld(*object);
        if (object->otherCharacter)
            shenApplyTranslationToAll(object->otherCharacter->entity.node, object->position);
        if (object->otherCharacter)
            ApplyEulerAnglesToNode(object->otherCharacter->entity.node, object->rotation);
        vfUpdateStageObjectWorldPos(*object);
    }
}

void vfclTriggerStageObjectHitReaction(VFMAData &game) {
    VFActor *player = nullptr;
    for (std::size_t slot = 0; slot < kBattleSlots && !player; ++slot) {
        if (!game.characters[slot])
            continue;
        VFActor *const actor = game.actors[slot];
        if (actor->slotIndex == static_cast<std::uint8_t>(game.playerSlot) && ((g_vfHitObjectHandled >> actor->slotIndex) & 1) == 0)
            player = actor;
    }
    if (!player)
        return;
    const auto hit = static_cast<std::uint8_t>(player->hitParameter);
    if ((player->hitStateFlags2 & 0x100U) == 0 || hit > 15U || ((0x801aU >> hit) & 1U) == 0)
        return;
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
        if ((object->stateFlags & 1U) != 0 || (object->flags & 2U) == 0 || ((object->contactSlotMask >> player->slotIndex) & 1) == 0)
            continue;
        const std::uint16_t toward =
            Shenmue_Atan2Fixed(player->contactCurrentPosition.x - object->position.x, player->contactCurrentPosition.z - object->position.z);
        if (static_cast<std::int16_t>(0x4000 - player->temporaryFacingAngle + toward) < 0)
            continue;
        Execute3DSoundCommand(object->object->soundCommand, 4, &object->position, 2.0f, 20.0f, 0);
        const std::uint16_t facing = player->temporaryFacingAngle;
        const std::int32_t folded = facing <= 0x8000U ? facing : 0x10000 - facing;
        const double speed = 0.5 - static_cast<double>(object->object->timeScale) * 0.0166;
        const float z = static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(0x4000 - folded))) * speed);
        VFStageObject &target = *g_vfStageObjs[object->objectIndex];
        target.localVelocity.x = static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(facing))) * speed);
        target.localVelocity.y = 0.0f;
        target.localVelocity.z = z;
        if ((object->object->entityFlags & 0x40U) != 0)
            vfApplyLocalAxisRotationToVector(*object, object->localVelocity);
        object->character = player->character;
    }
}

void vfStageObjectEvalHitboxKeyframes() {
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
        if (object->state != 1)
            continue;
        const VOBJ &source = *object->object;
        const std::int32_t frame = object->frameIndex;
        VFStageObjectKeyframeCursor *cursor = object->keyframeCursors;
        for (std::int32_t index = 0; index < source.animatedNodeCount; ++index, ++cursor) {
            float values[6]{};
            for (std::size_t channel = 0; channel < 6; ++channel) {
                VFStageObjectKeyframeCursor::Channel &track = cursor->channels[channel];
                std::int16_t key = track.cursor;
                while (key < track.count && frame >= track.keys[2 * key])
                    ++key;
                track.cursor = key;
                const std::int32_t *const next = track.keys + 2 * key;
                if (key == track.count) {
                    values[channel] = std::bit_cast<float>(next[-1]);
                } else {
                    const float from = std::bit_cast<float>(next[-1]);
                    values[channel] =
                        (std::bit_cast<float>(next[1]) - from) * static_cast<float>(frame - next[-2]) / static_cast<float>(next[0] - next[-2]) + from;
                }
            }
            Mt5RuntimeNode *const node = cursor->node;
            node->position.x = static_cast<float>(static_cast<double>(values[0]) * 10.0);
            node->position.y = static_cast<float>(static_cast<double>(values[1]) * 10.0);
            node->position.z = static_cast<float>(static_cast<double>(values[2]) * 10.0);
            node->rotation.x = static_cast<std::int32_t>(static_cast<double>(values[3]) * 182.0444444444445);
            node->rotation.y = static_cast<std::int32_t>(static_cast<double>(values[4]) * 182.0444444444445);
            node->rotation.z = static_cast<std::int32_t>(static_cast<double>(values[5]) * 182.0444444444445);
        }
        object->frameIndex += static_cast<std::int32_t>(source.frameRate);
        if (static_cast<std::uint32_t>(object->frameIndex) >= source.maximumFrame) {
            object->frameIndex = static_cast<std::int32_t>(source.maximumFrame);
            object->state = 2;
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(object->keyframeCursors), 0);
            object->keyframeCursors = nullptr;
        }
    }
}

void MarkFreeVOBJ(std::int32_t index, std::uint8_t state) {
    auto *const object = static_cast<VOBJ *>(HLib_AllocHeapMemBlock(sizeof(VOBJ), static_cast<std::int32_t>(kVobjToken)));
    if (!object)
        return;
    std::memset(object, 0, sizeof(VOBJ));
    g_pVOBJ[index] = object;
    object->state = state;
}

void vfSetupVOBJFromDYNA(std::int32_t index, const std::byte *descriptor, std::int32_t animatedNodeCount, const std::int16_t *animatedNodeKinds,
    std::uint32_t frameRate, std::int32_t startFrame, std::uint32_t maximumFrame, std::uint32_t reactionSoundCommand) {
    VOBJ &object = *g_pVOBJ[index];
    object.entityFlags |= 1U;
    object.frameRate = frameRate;
    object.startFrame = startFrame;
    object.maximumFrame = maximumFrame;
    object.reactionSoundCommand = reactionSoundCommand;
    object.animatedNodeCount = animatedNodeCount;
    object.animatedNodeKinds = animatedNodeKinds;
    std::uint32_t id = 0;
    std::memcpy(&id, descriptor, sizeof(id));
    object.keyframeData = reinterpret_cast<const std::int32_t *>(FindDynaEntry(id));
}

void vfCreateStageObject(std::int32_t index, Character *character, std::int32_t vobjIndex) {
    auto *const object = static_cast<VFStageObject *>(HLib_AllocHeapMemBlock(sizeof(VFStageObject), static_cast<std::int32_t>(kVobjToken)));
    if (!object)
        return;
    std::memset(object, 0, sizeof(VFStageObject));
    g_vfStageObjs[index] = object;
    object->next = g_vfObjectRoot;
    object->objectIndex = static_cast<std::uint8_t>(index);
    g_vfObjectRoot = object;
    if (character) {
        constexpr std::uint32_t kSavedFlags[]{0x80U, 2U, 4U, 1U};
        for (const std::uint32_t flag : kSavedFlags) {
            if (ccowHasAnyCharaFlags(character, flag))
                object->savedCollisionFlags |= flag;
        }
        ccowEnableCharaFlags(character, 0x80U);
        ccowDisableCharaFlags(character, 7U);
    }
    object->object = g_pVOBJ[vobjIndex];
    object->otherCharacter = character;
    object->otherCharacter2 = character;
    if (character) {
        SetStageCharacterVisible(*character, true);
        object->position = GetPositionForEntity(character->entity.node);
        object->rotation = GetEulerAnglesForEntity(character->entity.node);
    }
}

void vfAssignStageObjectByChara(std::int32_t index, Character *character, std::uint32_t flags) {
    VFStageObject &object = *g_vfStageObjs[index];
    object.flags |= flags;
    if (character) {
        ccowEnableCharaFlags(character, 0x80U);
        ccowDisableCharaFlags(character, 7U);
    }
    object.linkedObject = character;
    if (character)
        SetStageCharacterVisible(*character, false);
}

void vfUpdateAdvanceAllStageObjects() {
    Matrix4x4_Push();
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next)
        vfUpdateStageObjectWorldPos(*object);
    Matrix4x4::Pop(1);
}
} // namespace vf_detail
  // namespace

void vfFreeStageObjects() {
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
        if (object->state)
            continue;
        Character *const character = object->otherCharacter2;
        if ((object->savedCollisionFlags & 0x80U) == 0)
            ccowDisableCharaFlags(character, 0x80U);
        if ((object->savedCollisionFlags & 2U) != 0)
            ccowEnableCharaFlags(character, 2U);
        if ((object->savedCollisionFlags & 4U) != 0)
            ccowEnableCharaFlags(character, 4U);
        if ((object->savedCollisionFlags & 1U) != 0)
            ccowEnableCharaFlags(character, 1U);
    }
    for (VOBJ *&object : g_pVOBJ) {
        if (object) {
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(object), 0);
            object = nullptr;
        }
    }
    for (VFStageObject *&object : g_vfStageObjs) {
        if (object) {
            if (object->keyframeCursors)
                HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(object->keyframeCursors), 0);
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(object), 0);
            object = nullptr;
        }
    }
    g_vfObjectRoot = nullptr;
}

void vfUpdateStageObjects(VFMAData *game) {
    Matrix4x4_Push();
    for (VFStageObject *object = g_vfObjectRoot; object; object = object->next) {
        object->contactSlotMask = 0;
        object->environmentFlags = 0;
    }
    g_vfHitObjectHandled = 0;
    vfclCheckAllIntersectionsWithStageObjects(*game);
    vfclUpdateStageObjectHitboxesPhysics(*game);
    vfclTriggerStageObjectHitReaction(*game);
    vfStageObjectEvalHitboxKeyframes();
    Matrix4x4::Pop(1);
}



void check_radius_intersections() {
    QueryShape query{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange, .packed = {{kVFContactRadius, 0.0f, 0.0f}}};
    for (std::uint8_t index = 0; index < g_pFightStage->maxEnemies; ++index) {
        VFEnemySlot &slot = g_pFightStage->enemySlots[index];
        if (!slot.character)
            continue;
        if (slot.state == SS_Active) {
            const auto &context = *static_cast<const VFCLTaskData *>(slot.fighter->game->collisionData);
            const Vector3 &contact = context.previousContact[static_cast<std::uint8_t>(slot.battleIndex)];
            query.packed[0].y = contact.x;
            query.packed[0].z = contact.z;
            SelectCharacterMapQuery(slot.character);
            if (!ExecuteMathFnWrapper(&query, StageObjectCollisionMask())) {
                slot.positionX = contact.x;
                slot.positionZ = contact.z;
                slot.shouldSetPosition = 1;
            }
        } else if (slot.state == SS_Ready) {
            if (slot.wave && slot.wave->contactType == 3U)
                continue;
            const auto *const motion = static_cast<const MotionInformationWork *>(slot.motion);
            const MotionNodeRuntime *const row = vfFindEffectorRow(motion->previousNodes, kVFContactRootNode);
            const Matrix4x4 &transform = row && row->transform ? *row->transform : *g_matrixStack.top.stack;
            workTransformMatrix = transform;
            const float x =
                ((transform.rows[1].x * kVFContactRootOffset.y + transform.rows[0].x * kVFContactRootOffset.x) + transform.rows[2].x * kVFContactRootOffset.z) +
                transform.rows[3].x;
            const float z =
                ((transform.rows[1].z * kVFContactRootOffset.y + transform.rows[0].z * kVFContactRootOffset.x) + transform.rows[2].z * kVFContactRootOffset.z) +
                transform.rows[3].z;
            query.packed[0].y = x;
            query.packed[0].z = z;
            SelectCharacterMapQuery(slot.character);
            if (!ExecuteMathFnWrapper(&query, StageObjectCollisionMask())) {
                slot.positionX = x;
                slot.positionZ = z;
                slot.shouldSetPosition = 1;
            } else if (slot.shouldSetPosition) {
                vfSetNearbyFighterXZ(slot, slot.positionX - x, slot.positionZ - z);
            }
        }
    }
}

void Script_InterpolateAndScaleAngles(ScenarioContext *, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    if (!params)
        return;
    const std::byte *const idBytes = ScenarioPointer<std::byte>(params[4]);
    Vector3 *const position = ScenarioPointer<Vector3>(params[2]);
    Vector3i *const rotation = ScenarioPointer<Vector3i>(params[3]);
    if (!idBytes || !position || !rotation)
        return;
    std::uint32_t id = 0;
    std::memcpy(&id, idBytes, sizeof(id));
    const auto *entry = reinterpret_cast<const std::int32_t *>(FindDynaEntry(id));
    if (!entry)
        return;
    for (auto group = static_cast<std::int32_t>(params[0]); group > 0; --group)
        entry = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::byte *>(entry) + entry[0]);
    const auto frame = static_cast<std::int32_t>(params[1]);
    const std::int32_t *channel = entry + 1;
    float values[6]{};
    for (float &value : values) {
        const std::int32_t count = channel[0];
        const std::int32_t *const keys = channel + 1;
        std::int32_t key = 1;
        while (key < count && frame >= keys[2 * key])
            ++key;
        const std::int32_t *const next = keys + 2 * key;
        if (key == count) {
            value = std::bit_cast<float>(next[-1]);
        } else {
            const float from = std::bit_cast<float>(next[-1]);
            value = (std::bit_cast<float>(next[1]) - from) * static_cast<float>(frame - next[-2]) / static_cast<float>(next[0] - next[-2]) + from;
        }
        channel = keys + 2 * count;
    }
    *position = {values[0], values[1], values[2]};
    rotation->x = static_cast<std::int32_t>(static_cast<double>(values[3]) / 360.0 * 65536.0);
    rotation->y = static_cast<std::int32_t>(static_cast<double>(values[4]) / 360.0 * 65536.0);
    rotation->z = static_cast<std::int32_t>(static_cast<double>(values[5]) / 360.0 * 65536.0);
}

void Script_vfStageObjectUtils(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = Words(rawParams);
    if (!params)
        return;
    const std::uint32_t command = params[0];
    const auto index = static_cast<std::int32_t>(params[1]);
    const std::uint32_t int8 = params[2];
    const std::uint32_t intc = params[3];
    const std::uint32_t int10 = params[4];
    const std::uint32_t dword14 = params[5];
    const std::uint32_t dword18 = params[6];
    const std::uint32_t uint1c = params[7];
    const std::uint32_t uint20 = params[8];
    const std::uint32_t nextCommand = params[9];
    const bool vobjCommand = command <= 5U || command == 0x14U;
    const bool stageCommand = command >= 7U && command <= 0xfU && command != 0xdU;
    if ((vobjCommand && (index < 0 || index >= static_cast<std::int32_t>(g_pVOBJ.size()) || (command != 0 && !g_pVOBJ[index]))) ||
        (stageCommand && (index < 0 || index >= static_cast<std::int32_t>(g_vfStageObjs.size()) || (command != 7U && !g_vfStageObjs[index])))) {
        return;
    }
    switch (command) {
    case 0:
        MarkFreeVOBJ(index, static_cast<std::uint8_t>(int8));
        break;
    case 1: {
        const std::byte *const descriptor = ScenarioPointer<std::byte>(int8);
        if (descriptor)
            vfSetupVOBJFromDYNA(index, descriptor, static_cast<std::int32_t>(intc), ScenarioPointer<std::int16_t>(int10), dword14,
                static_cast<std::int32_t>(dword18), uint1c, uint20);
        break;
    }
    case 2: {
        VOBJ &object = *g_pVOBJ[index];
        if (intc == 3U) {
            object.entityFlags &= 0xfffffff9U;
            object.entryCount = 0;
            break;
        }
        object.entityFlags |= int8;
        if (object.entryCount >= 0 && object.entryCount < 3) {
            VFStageObjectShape &shape = object.entries[object.entryCount];
            shape = {.type = intc};
            if (intc == 1U || intc == 2U) {
                const std::uint32_t values[]{int10, dword14, dword18, uint1c, uint20, nextCommand};
                std::memcpy(shape.values, values, intc == 2U ? sizeof(values) : 4U * sizeof(values[0]));
            }
            ++object.entryCount;
        }
        if (intc == 1U) {
            const float radius = std::bit_cast<float>(uint1c) + std::bit_cast<float>(uint1c) + 1.0f;
            if (radius * radius > g_maxDist)
                g_maxDist = radius * radius;
        } else if (intc == 2U) {
            const float x = std::bit_cast<float>(uint20) + std::bit_cast<float>(dword18);
            const float z = std::bit_cast<float>(nextCommand) + std::bit_cast<float>(uint1c);
            if (z * z + x * x > g_maxDist)
                g_maxDist = z * z + x * x;
        }
        break;
    }
    case 3: {
        VOBJ &object = *g_pVOBJ[index];
        object.hitGuardStrength = static_cast<std::int8_t>(int8);
        object.hitReactionGuardType = static_cast<std::int8_t>(intc);
        object.hitParameter = static_cast<std::int16_t>(int10);
        object.hitReactionSoundCommand = static_cast<std::int32_t>(dword14);
        break;
    }
    case 4: {
        VOBJ &object = *g_pVOBJ[index];
        object.entityFlags |= int10;
        object.landingSoundCommand = static_cast<std::int32_t>(int8);
        object.soundCommand = intc;
        object.axisRotateStep = static_cast<std::int32_t>(dword14);
        object.rotationAmount = static_cast<std::int32_t>(dword18);
        const float scale = std::bit_cast<float>(uint1c);
        object.timeScale = 0.5 - static_cast<double>(scale) * 0.0166 >= 0.01 ? scale : 29.518072f;
        object.groundOffset = std::bit_cast<float>(uint20);
        break;
    }
    case 5: {
        VOBJ &object = *g_pVOBJ[index];
        const Vector3i *const rotation = ScenarioPointer<Vector3i>(int10);
        const Vector3 *const position = ScenarioPointer<Vector3>(intc);
        object.boneId = static_cast<std::int8_t>(int8);
        if (position)
            object.position = *position;
        if (rotation)
            object.rotation = *rotation;
        break;
    }
    case 7:
        vfCreateStageObject(index, FindCharacter(context, int8), static_cast<std::int32_t>(intc));
        break;
    case 8:
        vfAssignStageObjectByChara(index, FindCharacter(context, int8), intc);
        break;
    case 9: {
        VFStageObject &object = *g_vfStageObjs[index];
        object.position = {std::bit_cast<float>(int8), std::bit_cast<float>(intc), std::bit_cast<float>(int10)};
        if (object.otherCharacter)
            shenApplyTranslationToAll(object.otherCharacter->entity.node, object.position);
        break;
    }
    case 0xa: {
        VFStageObject &object = *g_vfStageObjs[index];
        object.rotation = {static_cast<std::int32_t>(int8 & 0xffffU), static_cast<std::int32_t>(intc & 0xffffU), static_cast<std::int32_t>(int10 & 0xffffU)};
        if (object.otherCharacter)
            ApplyEulerAnglesToNode(object.otherCharacter->entity.node, object.rotation);
        break;
    }
    case 0xb:
    case 0xc:
        g_vfStageObjs[index]->flags |= int8;
        break;
    case 0xd:
        vfUpdateAdvanceAllStageObjects();
        break;
    case 0xe:
        vfStageObjectResetAndActivateForChara(index, FindCharacter(context, int8));
        break;
    case 0xf:
        g_vfStageObjs[index]->localVelocity = {std::bit_cast<float>(int8), std::bit_cast<float>(intc), std::bit_cast<float>(int10)};
        break;
    case 0x11:
        g_vfShouldLoad70mbAssets = index;
        break;
    case 0x14:
        g_pVOBJ[index]->entityFlags |= int8;
        break;
    default:
        break;
    }
}
} // namespace shenmue::s1
