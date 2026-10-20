#include "s2/osage.hpp"
#include "s2/character.hpp"
#include "s2/model_component.hpp"
#include "s2/mt7.hpp"
#include "s2/hlib.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/weather.hpp"
#include "s2/field_collision.hpp"
#include "s2/face.hpp"
#include "s2/math.hpp"
#include "s2/game_runtime.hpp"
#include "d3t/buffer.hpp"
#include "s2/pause.hpp"
#include "s2/map_control.hpp"
#include "s2/event_runtime.hpp"
#include <cmath>
#include <cstring>
#include <bit>

namespace shenmue::s2 {
namespace {
OsageSpringParameters defaultSpring{0, 1.0f, 0.001f, 0.65f, 0.0f, 0.01f};
OsageParameters defaultParameters{OSAGE_COMPONENT, 0, 0, 0, 0, 0};
OsageCollisionPoint defaultCollisionPoints[28]{
    {1,0,14,12},{5,0,10,8},{1,0,9,9},{0,0,-8,12},{23,-1,-12,16},{23,-1,12,16},{-1,1,8,15},{-1,-1,-8,15},
    {0,0,-14,11},{2,1,4,14},{0,0,14,11},{2,1,-4,14},{0,0,2,10},{24,1,-1,9},{0,0,-2,10},{24,1,1,9},
    {0,1,-2,9},{34,1,0,7},{0,1,2,9},{34,1,0,7},{-1,-2,-8,15},{-1,-2,8,15},{20,-1,7,16},{20,-1,-7,16},
    {0,0,1,14},{34,-2,0,10},{0,1,-1,14},{34,-2,0,10}};
constexpr std::int16_t collisionBones[28]{5, 5, 5, 5, 3, 3, 3, 3, 15, 15, 8, 8, 16, 16, 9, 9, 19, 19, 12, 12, 22, 22, 22, 22, 28, 28, 23, 23};
struct ReferenceAngle {
    std::int32_t bone;
    Vector3 base, positive, negative;
};
constexpr ReferenceAngle referenceAngles[5]{
    {5,{1,0,0},{1,0,-1},{1,0,1}}, {3,{0,-1,0},{-1,-1,0},{1,-1,0}}, {22,{0,1,0},{1,1,0},{-1,1,0}},
    {19,{1,0,0},{1,-1,0},{1,1,0}}, {12,{1,0,0},{1,-1,0},{1,1,0}}};
float hairDistance;
float motionScale;
float inverseMotionScale;
float groundHeight;
Vector2 randomOscillation;
Vector4 secondarySurfaces[28];
bool useSecondarySurfaces;

bool IsOsageKind(std::int32_t kind) {
    for (const std::int16_t candidate : osageNodeKinds) {
        if (candidate < 0)
            break;
        if (static_cast<std::uint16_t>(kind) == candidate)
            return true;
    }
    return false;
}
OsageWork *FindOsage(const Character *character) {
    return static_cast<OsageWork *>(FindModelComponent(character, OSAGE_COMPONENT));
}
Vector3 Translation() {
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}
float Sin(std::int32_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(angle));
}
float Cos(std::int32_t angle) {
    return cos_fixed16(static_cast<std::uint16_t>(angle));
}
std::int32_t Degrees(float value) {
    return static_cast<std::int32_t>(value * 8192.0f / 45.0f);
}
float PhaseSin(float value) {
    return Sin(Degrees(value));
}
void Rotate(const Vector3i &euler) {
    if (euler.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(euler.z));
    if (euler.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(euler.y));
    if (euler.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(euler.x));
}
void BindTransform(OsageNode &node) {
    Matrix4x4::GetTransformMatrix(&node.transform);
    SetMT7NodeTransformOverride(*node.node, &node.transform);
}
void SetViewSpaceTransformFromEuler(const Vector3i &euler) {
    Matrix4x4::SetViewMatrixFromEuler(euler.y, euler.x, euler.z);
}

void ViewSpaceFromEuler(OsageNode &node) {
    SetViewSpaceTransformFromEuler(node.euler);
    BindTransform(node);
}
void BuildTransform(OsageNode &node) {
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(node.current.x, node.current.y, node.current.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.euler.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.euler.x));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.euler.z));
    BindTransform(node);
}
// @verified - p
void CleanupNodes(OsageWork &work) {
    OsageNode *node = work.nodes;
    while (node) {
        OsageNode *const next = node->next;
        if (node->meshAllocation) {
            HLib::FreeTaskData(node->meshAllocation);
            node->meshAllocation = nullptr;
            if (node->node) {
                node->node->runtimeMesh = node->originalMesh;
                node->originalMesh = nullptr;
            }
        }
        HLib::FreeTaskData(node);
        node = next;
    }
    work.nodes = nullptr;
}
// @verified - p
void OsageCleanup(Character *character, void *payload) {
    if (character)
        CleanupNodes(*static_cast<OsageWork *>(payload));
}
// @verified - p
void OsageUpdateNullsub(Character *, void *) {
}
// @verified - p
void ReadParameters(OsageWork &work, MT7Node &model) {
    std::byte *source = static_cast<std::byte *>(model.taskData);
    if (source)
        model.taskData = nullptr;
    else if (ClothComponent *const cloth = static_cast<ClothComponent *>(FindModelComponent(work.owner, SHEN_NAME(CLMD))))
        source = cloth->source;
    if (source) {
        std::uint32_t token;
        std::int32_t offset;
        std::memcpy(&token, source, 4);
        std::memcpy(&offset, source + 0x10, 4);
        if (token == SHEN_NAME(CLSG) && offset) {
            OsageParameters *const parameters = reinterpret_cast<OsageParameters *>(source + offset);
            if (parameters->token.name == OSAGE_COMPONENT.name && parameters->collisionOffset) {
                work.parameters = parameters;
                work.collisionPoints = reinterpret_cast<OsageCollisionPoint *>(source + parameters->collisionOffset);
                const std::int32_t primary = parameters->primaryOffset;
                const std::int32_t secondary = parameters->secondaryOffset;
                parameters->primaryOffset = HLib::GetDynamicHeapOffset(source + primary);
                parameters->secondaryOffset = HLib::GetDynamicHeapOffset(source + secondary);
                if (!parameters->primaryOffset) {
                    // Static defaults are consumed directly, not encoded as heap-relative offsets.
                    parameters->secondaryOffset = 0;
                    parameters->secondaryDepth = 0;
                }
                return;
            }
        }
    }
    work.collisionPoints = work.character.name == SHEN_NAME(CHAR) ? nullptr : defaultCollisionPoints;
    work.parameters = &defaultParameters;
}
// @verified - p
void AllocateNodeTree(OsageWork &work, MT7Node *model) {
    while (model) {
        if (model->runtimeSibling)
            Matrix4x4_Push();
        Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
        SetViewSpaceTransformFromEuler(model->rotation);
        if (IsOsageKind(model->id)) {
            OsageNode *const node = static_cast<OsageNode *>(HLib::AllocateTaskData(sizeof(OsageNode), OSAGE_COMPONENT));
            if (node) {
                node->token = OSAGE_COMPONENT;
                node->owner = &work;
                node->node = model;
                model->taskData = node;
                node->kind = -1;
                node->linkFlags = 0;
                const Vector3 position = Translation();
                if (position.z > 0)
                    node->linkFlags = 0x40;
                if (position.x > 0)
                    node->linkFlags |= 0x20;
                node->updateFlags = OsageUpdateWarmupStart;
                node->current = {};
                node->previous = {};
                node->facingAngle = 0;
                node->euler = {};
                node->motionMagnitude = node->accumulatedSway = node->accumulatedForward = node->accumulatedVertical = 0;
                node->pitchScale = node->yawScale = 0;
                node->pitchPhase = IncrementSharedPrngFloat() * 360.0f;
                node->yawPhase = IncrementSharedPrngFloat() * 360.0f;
                for (Vector2 &pair : node->offsets)
                    pair.x = 0;
                for (std::int16_t &binding : node->vertexBindings)
                    binding = -1;
                Matrix4x4::GetTransformMatrix(&node->transform);
                node->smoothingCountdown = 0;
                node->child = nullptr;
                node->parent = nullptr;
                node->meshAllocation = nullptr;
                node->originalMesh = model->runtimeMesh;
                node->next = work.nodes;
                work.nodes = node;
                if (model->runtimeParent) {
                    OsageNode *const parent = static_cast<OsageNode *>(model->runtimeParent->taskData);
                    if (parent && parent->token.name == OSAGE_COMPONENT.name && parent->node->id == model->id) {
                        node->parent = parent;
                        node->linkFlags = parent->linkFlags + 1;
                        parent->child = node;
                    }
                }
            }
        }
        if (model->runtimeChild)
            AllocateNodeTree(work, model->runtimeChild);
        if (!model->runtimeSibling)
            break;
        Matrix4x4::Pop(1);
        model = model->runtimeSibling;
    }
}
// @verified - p
float MotionMagnitude(OsageNode &node) {
    float result = 0;
    if (node.updateFlags & OsageUpdateWarmupMask) {
        node.smoothingCountdown = 6;
        node.accumulatedForward = node.accumulatedVertical = node.motionMagnitude = node.accumulatedSway = 0;
    } else {
        const float dy = node.current.y - node.previous.y;
        const float dz = node.current.z - node.previous.z;
        const float dx = node.current.x - node.previous.x;
        const float sine = Sin(node.facingAngle), cosine = Cos(node.facingAngle);
        node.accumulatedForward += dy;
        node.accumulatedSway = ((cosine * dx) - (sine * dz)) + node.accumulatedSway;
        node.accumulatedVertical = ((cosine * dz) + (sine * dx)) + node.accumulatedVertical;
        if (--node.smoothingCountdown <= 0) {
            node.smoothingCountdown = 6;
            const float forward = node.accumulatedForward * 5, sway = node.accumulatedSway * 5, vertical = node.accumulatedVertical * 5;
            node.motionMagnitude = std::sqrt(((forward * forward) + (sway * sway)) + (vertical * vertical));
            node.accumulatedForward = node.accumulatedVertical = node.accumulatedSway = 0;
        }
        result = inverseMotionScale * node.motionMagnitude;
    }
    node.previous = node.current;
    return result;
}
// @verified - p
void Spring(OsageNode &node, float pitchRate, float yawRate, float pitchTarget, float yawTarget) {
    const float delta = motionScale * pitchRate;
    if (node.pitchScale <= pitchTarget) {
        node.pitchScale += delta;
        if (node.pitchScale > pitchTarget)
            node.pitchScale = pitchTarget;
    } else {
        node.pitchScale -= delta;
        if (pitchTarget - node.pitchScale > 0)
            node.pitchScale = pitchTarget;
        else if (node.pitchScale < 0)
            node.pitchScale = 0;
    }
    if (node.yawScale <= yawTarget) {
        node.yawScale += motionScale * yawRate;
        if (node.yawScale > yawTarget)
            node.yawScale = yawTarget;
    } else {
        node.yawScale -= motionScale * yawRate;
        if (node.yawScale < 0)
            node.yawScale = 0;
    }
}
// @verified - p
void AdvancePhase(OsageNode &node, float pitch, float yaw) {
    node.pitchPhase += (randomOscillation.x + pitch) * motionScale;
    if (node.pitchPhase >= 360)
        node.pitchPhase = static_cast<float>(static_cast<double>(node.pitchPhase) - 360.0);
    else if (node.pitchPhase < 0)
        node.pitchPhase = static_cast<float>(static_cast<double>(node.pitchPhase) + 360.0);
    node.yawPhase += (randomOscillation.y + yaw) * motionScale;
    if (node.yawPhase >= 360)
        node.yawPhase -= 360;
    else if (node.yawPhase < 0)
        node.yawPhase += 360;
}
void Offsets(OsageNode &node) {
    node.offsets[0] = {PhaseSin(node.pitchPhase) * node.pitchScale, PhaseSin(node.yawPhase) * node.yawScale};
}
void ShiftOffsets(OsageNode &node) {
    for (std::int32_t i = 8; i > 0; --i)
        node.offsets[i] = node.offsets[i - 1];
}
// @verified - p
void ClampDistance(const Vector3 &origin, Vector3 &point, float distance) {
    const Vector3 delta{point.x - origin.x, point.y - origin.y, point.z - origin.z};
    float magnitude = std::sqrt(((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z));
    if (magnitude < 0.0001f)
        magnitude = 0.0001f;
    point = {(distance / magnitude) * delta.x + origin.x, (distance / magnitude) * delta.y + origin.y, (distance / magnitude) * delta.z + origin.z};
}
// @verified - p
bool ProjectSurface(const Vector4 *surface, Vector3 &point, std::int32_t count, float margin) {
    Vector4 center = surface[0];
    for (std::int32_t i = 1; i < count; ++i) {
        const Vector4 &next = surface[i];
        const Vector3 delta{center.x - next.x, center.y - next.y, center.z - next.z};
        float squared = ((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z);
        if (squared < 0.0000001f)
            squared = 0.0000001f;
        float ratio = ((((point.x - next.x) * delta.x) + ((point.y - next.y) * delta.y)) + ((point.z - next.z) * delta.z)) / squared;
        if (count <= 2 || ratio > 0.6)
            ratio -= 0.1f;
        else if (ratio < 0.4)
            ratio += 0.1f;
        ratio = std::fmin(1.0f, std::fmax(0.0f, ratio));
        center = {ratio * delta.x + next.x, ratio * delta.y + next.y, ratio * delta.z + next.z, ratio * (center.w - next.w) + next.w};
    }
    const Vector3 delta{point.x - center.x, point.y - center.y, point.z - center.z};
    const float radius = margin + center.w;
    float magnitude = std::sqrt(((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z));
    if (radius <= magnitude)
        return false;
    if (magnitude < 0.0001f)
        magnitude = 0.0001f;
    point = {(radius / magnitude) * delta.x + center.x, (radius / magnitude) * delta.y + center.y, (radius / magnitude) * delta.z + center.z};
    return true;
}
// @verified - p
bool ProjectEnabled(OsageWork &work, const Vector3 &origin, Vector3 &point, float distance, float margin, std::uint16_t mask) {
    constexpr std::int32_t indices[12]{0, 2, 4, 8, 10, 12, 14, 16, 18, 20, 24, 26};
    constexpr std::int32_t counts[12]{2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2};
    bool projected = false;
    for (std::int32_t i = 0; i < 12; ++i)
        if (mask & (1U << i)) {
            projected |= ProjectSurface(work.collisionSurfaces.data() + indices[i], point, counts[i], margin);
            ClampDistance(origin, point, distance);
        }
    return projected;
}
// @verified - p
void ParentConstraint(const Vector3 &origin, Vector3 &point, const Vector3 &parent, float distance) {
    const Vector3 delta{point.x - origin.x, point.y - origin.y, point.z - origin.z};
    const float ratio = ((((parent.x - origin.x) * delta.x) + ((parent.y - origin.y) * delta.y)) + ((parent.z - origin.z) * delta.z)) /
        (((delta.y * delta.y) + (delta.x * delta.x)) + (delta.z * delta.z));
    if (ratio > 0.2) {
        point = {ratio * delta.x + origin.x, ratio * delta.y + origin.y, ratio * delta.z + origin.z};
        const float length = GetEuclideanDistance(&origin, &parent);
        ClampDistance(parent, point, length);
        ClampDistance(origin, point, distance);
    }
}
Vector3 FacialDisplacement(Character *character) {
    Vector3 displacement{};
    if (!character || !character->facialModel)
        return displacement;
    const FacialModelResource *const resource = reinterpret_cast<const FacialModelResource *>(character->facialModel);
    if (resource->vertices[7].empty())
        return displacement;
    const FacialMorphControl &control = resource->vertices[7][0];
    const std::byte *const bytes = static_cast<const std::byte *>(d3t::GetBufferCpuData(control.buffer->handle, nullptr));
    Vector3 position;
    std::memcpy(&position, bytes + control.byteOffset, sizeof(position));
    displacement.z = -std::fmin(position.x - control.vertex.neutral.x, 0.0f);
    displacement.y = std::fmin(position.z - control.vertex.neutral.z, 0.0f);
    return displacement;
}
// @verified - p
void AlignEndpoint(const Vector3 &origin, Vector3 &point) {
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    point = {point.x - origin.x, point.y - origin.y, point.z - origin.z};
    Vector3 direction = point;
    if (std::sqrt((direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z) < 0.000001f)
        return;
    Vector3::NormalizeInPlace(direction);
    const Vector3 down{0, -1, 0};
    if (GetEuclideanDistance(&direction, &down) < 0.000001f)
        return;
    Matrix4x4_BuildRotationMatrix(down, direction);
}
// @verified - p
float Noise(std::int16_t angle) {
    const float first = Sin(22 * angle) * 0.30000001f;
    const float second = Sin(31 * angle) * 0.37f + first;
    const float third = Sin(103 * angle) * 0.15000001f + second;
    return Sin(147 * angle) * 0.18000001f + third;
}
std::uint32_t Facing(OsageNode &node, std::int32_t index, std::uint32_t extra = 0) {
    return (node.linkFlags & 0x1f) ? node.parent->facingAngle : node.owner->referenceAngles[index] + extra;
}
void BeginResponse(OsageNode &node, std::int32_t index, std::uint32_t extra = 0) {
    node.facingAngle = Facing(node, index, extra);
    const Vector3 &position = node.node->position;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    node.current = Translation();
}
void PublishEulerResponse(OsageNode &node) {
    node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y, node.node->rotation.z + Degrees(node.offsets[0].y)};
    ViewSpaceFromEuler(node);
}
Vector3 EndpointPosition(OsageNode &node) {
    MT7Node *const end = node.node->runtimeChild ? node.node->runtimeChild : node.node;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(end->position.x, end->position.y, end->position.z);
    const Vector3 result = Translation();
    Matrix4x4::Pop(1);
    return result;
}
// @verified - p
void ResetTransform(OsageNode &node) {
    BeginResponse(node, 1);
    node.euler = node.node->rotation;
    ViewSpaceFromEuler(node);
}
// @verified - p
void SimpleWindTransform(OsageNode &node, bool collision) {
    BeginResponse(node, 2);
    const float magnitude = MotionMagnitude(node);
    Spring(node, 0.2f, 0.2f, magnitude + magnitude, magnitude * 4);
    AdvancePhase(node, 10, -15);
    Offsets(node);
    if (!collision) {
        PublishEulerResponse(node);
        return;
    }
    const Matrix4x4 root = workTransformMatrix;
    for (;;) {
        Matrix4x4::SetActiveTransform(&root);
        node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y + Degrees(node.offsets[0].y), node.node->rotation.z};
        ViewSpaceFromEuler(node);
        Vector3 endpoint = EndpointPosition(node);
        if (!ProjectSurface(node.owner->collisionSurfaces.data() + 26, endpoint, 2, 0.0020000001f))
            break;
        node.offsets[0].x -= 3;
        node.updateFlags |= OsageUpdateCurrentFrame;
        if (node.offsets[0].x < -90)
            break;
    }
}
// @verified - p
void WeatherWindTransform(OsageNode &node) {
    BeginResponse(node, 0, 0x8000);
    const float magnitude = MotionMagnitude(node);
    const float wind = std::fmax(0.5f, GetEffectiveWindSpeed(GetWind()) * 30);
    const std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
    (void)Sin(phase);
    (void)Cos(phase);
    if (node.linkFlags & 0x1f) {
        Spring(node, 0.2f, 0.2f, magnitude * 12 + (wind + wind), magnitude * 12 + (wind + wind));
        node.pitchPhase = node.parent->pitchPhase;
        node.yawPhase = node.parent->yawPhase;
        Offsets(node);
        PublishEulerResponse(node);
    } else {
        Spring(node, 0.2f, 0.2f, magnitude * 5 + (wind + wind), magnitude * 5 + (wind + wind));
        AdvancePhase(node, 20, -30);
        Offsets(node);
        node.euler = {Degrees(node.offsets[0].x), static_cast<std::int32_t>(node.facingAngle), Degrees(node.offsets[0].y)};
        BuildTransform(node);
    }
}
// @verified - p
void DelayedWeatherTransform(OsageNode &node) {
    BeginResponse(node, 0);
    ShiftOffsets(node);
    if (!(node.linkFlags & 0x1f)) {
        const float magnitude = MotionMagnitude(node);
        const float wind = std::fmax(0.5f, GetEffectiveWindSpeed(GetWind()) * 30);
        const std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
        (void)Sin(phase);
        (void)Cos(phase);
        if (node.owner->character.name == SHEN_NAME(REN_)) {
            const float amount = std::fmax(1.5f, wind) + magnitude;
            Spring(node, 0.2f, 0.2f, amount * 0.80000001f, amount * 1.2f);
        } else
            Spring(node, 0.2f, 0.2f, magnitude + wind, magnitude + wind);
        AdvancePhase(node, 10, -15);
        Offsets(node);
    } else {
        node.offsets[0] = node.parent->offsets[6];
        if (node.owner->character.name == SHEN_NAME(REN_)) {
            node.offsets[0].x *= 1.2f;
            node.offsets[0].y *= 1.2f;
        }
    }
    PublishEulerResponse(node);
}
// @verified - p
void HairStrandTransform(OsageNode &node, Character *character) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name scenarioName = scenGetCharaName(control ? control->scenarioTask : nullptr, character);
    BeginResponse(node, 0);
    ShiftOffsets(node);
    float amount = MotionMagnitude(node);
    if (node.owner->character.name == SHEN_NAME(SRY_) && node.node->runtimeParent &&
        (node.node->runtimeParent->id == 6 || node.node->runtimeParent->id == 11) && amount > 5)
        amount = 5;
    const bool gentle = node.owner->character.name == SHEN_NAME(TNP_);
    const float rate = gentle ? 0.1f : 0.2f;
    const float pitch = gentle ? 0.0f : 20.0f;
    if (!gentle)
        amount += amount;
    if (node.owner->windSpeedNext > amount)
        amount = node.owner->windSpeedNext;
    Spring(node, rate, rate, amount, amount);
    AdvancePhase(node, pitch, -30);
    Offsets(node);
    node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y, node.node->rotation.z + Degrees(node.offsets[0].y)};
    if (!(node.linkFlags & 0x1f) && node.kind == 132) {
        const Vector3 displacement = FacialDisplacement(node.owner->owner);
        Matrix4x4::preTranslate(-displacement.z, displacement.x, displacement.y);
    }
    if (scenarioName.name == SHEN_NAME(SGT_))
        Matrix4x4::preTranslate(0, node.node->runtimeChild ? 0 : 0.035f, node.node->runtimeChild ? -0.0080000004f : -0.0049999999f);
    ViewSpaceFromEuler(node);
}
// @verified - p
void ObjectWindTransform(OsageNode &node) {
    BeginResponse(node, 1, 0x8000);
    const float magnitude = MotionMagnitude(node);
    const std::int32_t depth = node.linkFlags & 0x1f;
    std::int32_t limit = 4;
    float margin = 0.0099999998f;
    if (node.owner->character.name == SHEN_NAME(REN_)) {
        if (!depth)
            margin = 0.001f;
        if (node.owner->mode == 3)
            limit = 5;
    }
    if (depth >= limit) {
        if (node.node->runtimeChild) {
            if (depth) {
                node.pitchPhase = node.parent->pitchPhase;
                node.yawPhase = node.parent->yawPhase;
            }
            node.euler = node.node->rotation;
        } else {
            Spring(node, 0.2f, 0.2f, magnitude * 8, magnitude * 8);
            node.pitchPhase = node.parent->pitchPhase;
            node.yawPhase = node.parent->yawPhase;
            Offsets(node);
            node.euler = {node.node->rotation.x + Degrees(node.offsets[0].x), node.node->rotation.y, node.node->rotation.z + Degrees(node.offsets[0].y)};
        }
        ViewSpaceFromEuler(node);
        return;
    }
    Spring(node, 0.2f, 0.2f, (static_cast<float>(depth) * 2.5f) * magnitude, static_cast<float>(depth + depth) * magnitude);
    if (depth) {
        node.pitchPhase = node.parent->pitchPhase;
        node.yawPhase = node.parent->yawPhase;
    } else
        AdvancePhase(node, 20, -30);
    Offsets(node);
    if (node.offsets[0].x > 0)
        node.offsets[0].x = -node.offsets[0].x;
    float pitch = node.offsets[0].x;
    node.euler = {Degrees(pitch), static_cast<std::int32_t>(node.facingAngle), Degrees(node.offsets[0].y)};
    BuildTransform(node);
    Vector3 endpoint = EndpointPosition(node);
    if (ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin)) {
        node.updateFlags |= OsageUpdateCurrentFrame;
        do {
            node.euler.x = Degrees(pitch);
            BuildTransform(node);
            endpoint = EndpointPosition(node);
            if (!ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin))
                break;
            pitch = static_cast<float>(static_cast<double>(pitch) - 10.0);
        } while (pitch >= -120);
        do {
            node.euler.x = Degrees(pitch);
            BuildTransform(node);
            endpoint = EndpointPosition(node);
            if (ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin))
                break;
            pitch = static_cast<float>(static_cast<double>(pitch) + 1.0);
        } while (pitch <= 0);
        do {
            node.euler.x = Degrees(pitch);
            BuildTransform(node);
            endpoint = EndpointPosition(node);
            if (!ProjectSurface(node.owner->collisionSurfaces.data() + 4, endpoint, 4, margin))
                break;
            pitch -= 0.1f;
        } while (pitch >= -120);
    }
    if (node.owner->mode == 3) {
        const float height = node.owner->owner ? node.owner->owner->entity.node.position.y - 0.0099999998f : -0.0099999998f;
        if (height > endpoint.y) {
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                endpoint = EndpointPosition(node);
                if (height <= endpoint.y)
                    break;
                pitch -= 10;
            } while (pitch >= -120);
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                endpoint = EndpointPosition(node);
                if (height > endpoint.y)
                    break;
                pitch += 1;
            } while (pitch <= 0);
        }
    }
}
// @verified - p
void OscillationTransform(OsageNode &node) {
    BeginResponse(node, 1);
    float magnitude = 0;
    if (node.updateFlags & OsageUpdateWarmupMask) {
        node.smoothingCountdown = 6;
        node.motionMagnitude = node.accumulatedSway = 0;
    } else {
        node.accumulatedSway =
            (Cos(node.facingAngle) * (node.current.x - node.previous.x) - Sin(node.facingAngle) * (node.current.z - node.previous.z)) + node.accumulatedSway;
        if (--node.smoothingCountdown <= 0) {
            node.smoothingCountdown = 6;
            node.motionMagnitude = node.accumulatedSway * 5;
            node.accumulatedSway = 0;
        }
        magnitude = inverseMotionScale * node.motionMagnitude;
    }
    node.previous = node.current;
    if (node.offsets[1].x > node.current.y) {
        node.offsets[1].x = node.current.y;
        node.offsets[2] = {0, 1};
    }
    if (node.current.y > node.offsets[3].x) {
        node.offsets[3].x = node.current.y;
        node.offsets[4] = {0, 1};
    }
    node.offsets[2].x += motionScale;
    node.offsets[4].x += motionScale;
    bool changed = false;
    if (node.offsets[2].x >= 4 && node.offsets[2].y > 0) {
        node.offsets[5].x = node.offsets[3].x - node.offsets[1].x;
        node.offsets[3].x = -10000;
        node.pitchPhase = 180;
        node.offsets[2].y = -1;
        changed = true;
    }
    if (node.offsets[4].x >= 4 && node.offsets[4].y > 0) {
        node.offsets[5].x = node.offsets[3].x - node.offsets[1].x;
        node.offsets[1].x = 10000;
        node.pitchPhase = 0;
        node.offsets[4].y = -1;
        changed = true;
    }
    if (changed) {
        node.offsets[6].x = std::fabs(node.offsets[4].x - node.offsets[2].x);
        if (node.offsets[5].x < 0 || node.offsets[5].x >= 0.3f || node.offsets[6].x < 3 || node.offsets[6].x >= 20)
            node.offsets[7].x = 0;
        else {
            node.offsets[7].x = node.offsets[5].x;
            node.offsets[8].x = node.offsets[6].x;
        }
    }
    float strength = 0, cap = 0;
    if (node.owner->character.name == SHEN_NAME(SYE_) || node.owner->character.name == SHEN_NAME(SYP_)) {
        strength = 0.05f;
        cap = 0.005f;
    } else if (node.owner->character.name == SHEN_NAME(CHJ_)) {
        strength = 0.13f;
        cap = 0.005f;
    }
    float target = strength * node.offsets[7].x;
    if (target < 0.0005f)
        target = 0;
    target = std::fmin(cap, target);
    if (target > node.pitchScale) {
        node.pitchScale += motionScale * 0.001f;
        if (node.pitchScale > target)
            node.pitchScale = target;
    } else if (node.pitchScale > target) {
        node.pitchScale -= motionScale * 0.00050000002f;
        if (node.pitchScale < target)
            node.pitchScale = target;
    }
    float yawTarget = target > 0 ? std::fabs(magnitude) : target;
    yawTarget *= strength;
    if (yawTarget < 0.0005f)
        yawTarget = 0;
    yawTarget = std::fmin(cap, yawTarget);
    if (yawTarget > node.yawScale) {
        node.yawScale += motionScale * 0.001f;
        if (node.yawScale > yawTarget)
            node.yawScale = yawTarget;
    } else if (node.yawScale > yawTarget) {
        node.yawScale -= motionScale * 0.00050000002f;
        if (node.yawScale < yawTarget)
            node.yawScale = yawTarget;
    }
    float duration = node.offsets[8].x + node.offsets[8].x;
    if (duration < 6)
        duration = 12;
    const float phase = motionScale * 360 / std::fmin(40.0f, duration);
    node.pitchPhase += phase;
    if (node.pitchPhase >= 360)
        node.pitchPhase -= 360;
    node.yawPhase += phase;
    if (node.yawPhase >= 360)
        node.yawPhase -= 360;
    Offsets(node);
    node.euler = node.node->rotation;
    if (!(node.linkFlags & 0x1f))
        Matrix4x4::preTranslate(node.offsets[0].x, 0, node.offsets[0].y);
    ViewSpaceFromEuler(node);
}
const OsageSpringParameters &SpringParameters(OsageNode &node) {
    OsageParameters &parameters = *node.owner->parameters;
    const std::int32_t depth = node.linkFlags & 0x1f;
    if (&parameters == &defaultParameters)
        return defaultSpring;
    if (!parameters.primaryOffset)
        return defaultSpring;
    if (parameters.secondaryOffset && parameters.secondaryDepth <= depth && parameters.secondaryDepth)
        return *static_cast<const OsageSpringParameters *>(HLib::GetPointerFromDynamicHeapOffsetBase(parameters.secondaryOffset));
    return *static_cast<const OsageSpringParameters *>(HLib::GetPointerFromDynamicHeapOffsetBase(parameters.primaryOffset));
}
void SwayOffset(float &position, float &velocity, float acceleration, float bound) {
    float step = velocity < 0 ? -acceleration : acceleration;
    position = (velocity + step) + position;
    if (position > bound) {
        velocity = -std::fabs(velocity);
        if (std::fabs(velocity) < 0.00000011920929f)
            velocity = -step;
    } else if (position < -bound) {
        velocity = IncrementSharedPrngFloat() * std::fabs(step);
        if (std::fabs(velocity) < 0.00000011920929f)
            velocity = step;
    }
}
// @verified - p
void StrandSpringTransform(OsageNode &node) {
    OsageWork &work = *node.owner;
    node.facingAngle = Facing(node, 0, 0x8000);
    const OsageSpringParameters &parameters = SpringParameters(node);
    float oldDistance = 0;
    if (!node.parent)
        oldDistance = GetEuclideanDistance(&node.previous, &node.current);
    const Vector3 oldOrigin = node.current;
    const Vector3 &local = node.node->position;
    Matrix4x4::preTranslate(local.x, local.y, local.z);
    node.current = Translation();
    Vector3 origin = node.current;
    if (!node.parent && GetEuclideanDistance(&origin, &oldOrigin) - oldDistance > 0.25f)
        for (OsageNode *link = &node; link; link = link->child)
            link->updateFlags = (link->updateFlags & OsageUpdateHistoryMask) | OsageUpdateWarmupStart;
    if (work.mode == 2) {
        ShiftOffsets(node);
        if (node.linkFlags & 0x1f)
            node.offsets[0] = node.parent->offsets[3];
        else {
            SwayOffset(node.offsets[0].x, node.offsets[9].x, 0.00083332503f, 0.0125f);
            SwayOffset(node.offsets[0].y, node.offsets[9].y, 0.0016666501f, 0.0166665f);
        }
        const float sine = Sin(-static_cast<std::int32_t>(node.facingAngle)), cosine = Cos(-static_cast<std::int32_t>(node.facingAngle));
        origin.y += node.offsets[0].y;
        origin.x += node.offsets[0].x * cosine - sine * -0.050000001f;
        origin.z += node.offsets[0].x * sine - cosine * 0.050000001f;
    }
    if (node.kind == 120) {
        const float amount = Noise(static_cast<std::int16_t>(Degrees(node.pitchPhase)));
        origin.x += (work.wind.x * 0.0099999998f) * amount;
        origin.z += (work.wind.z * 0.0099999998f) * amount;
        node.pitchPhase += 0.15000001f;
    }
    if (parameters.mode == 1) {
        node.euler = node.node->rotation;
        ViewSpaceFromEuler(node);
        return;
    }
    MT7Node *const endpointNode = node.node->runtimeChild ? node.node->runtimeChild : node.node;
    if (parameters.mode == 2) {
        const float magnitude = MotionMagnitude(node);
        Spring(node, 5, 0, std::fmin(50.0f, node.motionMagnitude * 12), 0);
        (void)magnitude;
        AdvancePhase(node, 20, 0);
        float pitch = std::fmax(0.0f, (PhaseSin(node.pitchPhase) * 0.1f + 1) * node.pitchScale);
        node.euler = {Degrees(pitch), static_cast<std::int32_t>(node.facingAngle), 0};
        BuildTransform(node);
        const Vector3 endpointLocal{
            parameters.length * endpointNode->position.x, parameters.length * endpointNode->position.y, parameters.length * endpointNode->position.z};
        const auto project = [&]() {
            Matrix4x4_Push();
            Matrix4x4::preTranslate(endpointLocal.x, endpointLocal.y, endpointLocal.z);
            Vector3 endpoint = Translation();
            Matrix4x4::Pop(1);
            return ProjectSurface(work.collisionSurfaces.data() + 4, endpoint, 4, parameters.radius);
        };
        if (project()) {
            bool search = true;
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                if (!project())
                    break;
                pitch += 10;
                if (pitch > 120) {
                    pitch = 120;
                    search = false;
                }
                node.updateFlags |= OsageUpdateCurrentFrame;
            } while (search);
            bool settled = true;
            for (;;) {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                if (project())
                    break;
                pitch -= 1;
                if (pitch < 0) {
                    pitch = 0;
                    settled = false;
                    break;
                }
            }
            if (settled)
                node.updateFlags |= OsageUpdateCurrentFrame;
            do {
                node.euler.x = Degrees(pitch);
                BuildTransform(node);
                if (!project())
                    break;
                pitch += 0.1f;
            } while (pitch <= 120);
        }
        return;
    }
    std::int32_t directional = work.character.name == SHEN_NAME(SIN_) ? 1 : 0;
    if (work.mode == 1)
        directional = 2;
    Vector3 direction{};
    if (directional) {
        MotionWork *const motion = FindMotionWork(work.owner);
        Vector3 start{}, end{};
        GetMotionBonePosition(motion, static_cast<MotionBoneId>(directional == 1 ? 3 : 22), &start);
        GetMotionBonePosition(motion, static_cast<MotionBoneId>(directional == 1 ? 5 : 3), &end);
        direction = {start.x - end.x, start.y - end.y, start.z - end.z};
        Vector3::NormalizeInPlace(direction);
    }
    Vector3 movement{};
    if (node.updateFlags & OsageUpdateWarmupMask) {
        if (directional)
            movement = {(node.current.x - oldOrigin.x) + direction.x, ((node.current.y - oldOrigin.y) + direction.y) - 0.1f,
                (node.current.z - oldOrigin.z) + direction.z};
        else
            movement = {node.current.x - oldOrigin.x, node.current.y - oldOrigin.y - 0.1f, node.current.z - oldOrigin.z};
    } else {
        movement = {(node.endpoint.x - node.previous.x) * parameters.damping, (node.endpoint.y - node.previous.y) * parameters.damping,
            (node.endpoint.z - node.previous.z) * parameters.damping};
        if (directional) {
            movement.x += (direction.x * parameters.gravity) * motionScale;
            movement.y = (node.endpoint.y - node.previous.y) + ((direction.y * parameters.gravity) * motionScale);
            movement.z += (direction.z * parameters.gravity) * motionScale;
        } else {
            movement.x += (node.current.x - oldOrigin.x) * parameters.stiffness;
            movement.y = movement.y - motionScale * parameters.gravity + (node.current.y - oldOrigin.y) * parameters.stiffness;
            movement.z += (node.current.z - oldOrigin.z) * parameters.stiffness;
        }
    }
    Vector3 endpoint{movement.x + node.endpoint.x, movement.y + node.endpoint.y, movement.z + node.endpoint.z};
    node.previous = node.endpoint;
    if (groundHeight < -10000)
        groundHeight = shenFindIntersectionWithScene(endpoint, -10000);
    if (groundHeight > endpoint.y) {
        endpoint.y = groundHeight;
        node.updateFlags |= OsageUpdateCurrentFrame;
    }
    const Vector3 &endLocal = endpointNode->position;
    const float length = parameters.length * std::sqrt(((endLocal.x * endLocal.x) + (endLocal.y * endLocal.y)) + (endLocal.z * endLocal.z));
    ClampDistance(origin, endpoint, length);
    if (node.linkFlags & 0x1f)
        ParentConstraint(node.current, endpoint, node.parent->current, length);
    if (work.character.name == SHEN_NAME(REN_) && node.child && !(node.updateFlags & OsageUpdateWarmupMask)) {
        const Vector3 delta{node.child->endpoint.x - endpoint.x, node.child->endpoint.y - endpoint.y, node.child->endpoint.z - endpoint.z};
        const float ratio = (((node.current.y - endpoint.y) * delta.y + (node.current.x - endpoint.x) * delta.x) + (node.current.z - endpoint.z) * delta.z) /
            ((delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z);
        if (ratio > -0.6f) {
            endpoint.x += delta.x * 0.2f;
            endpoint.y += delta.y * 0.2f;
            endpoint.z += delta.z * 0.2f;
            ClampDistance(node.current, endpoint, length);
        }
    }
    const std::uint16_t mask = work.character.name == SHEN_NAME(BKN_) ? 31 : work.character.name == SHEN_NAME(JOP_) ? 511 : 0xffff;
    bool collision = ProjectEnabled(work, node.current, endpoint, length, parameters.radius, mask);
    if (useSecondarySurfaces) {
        collision |= ProjectSurface(secondarySurfaces, endpoint, 2, parameters.radius);
        ClampDistance(node.current, endpoint, length);
        collision |= ProjectSurface(secondarySurfaces + 2, endpoint, 2, parameters.radius);
        ClampDistance(node.current, endpoint, length);
    }
    if (collision)
        node.updateFlags |= OsageUpdateCurrentFrame;
    if (node.updateFlags & OsageUpdateWarmupMask)
        node.previous = endpoint;
    const auto invalidCoordinate = [](float value) {
        const std::uint32_t bits = std::bit_cast<std::uint32_t>(value);
        return bits - 0x00800000U > 0x7effffffU && bits + 0x7f800000U > 0x7effffffU && bits != 0 && bits != 0x80000000U;
    };
    if (invalidCoordinate(endpoint.x) || invalidCoordinate(endpoint.z))
        endpoint = node.current;
    node.endpoint = endpoint;
    Matrix4x4::SetPrePassToTempTransform();
    AlignEndpoint(node.current, endpoint);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.facingAngle));
    if (!(node.linkFlags & 0x1f) && node.kind == 133) {
        Vector3 displacement = FacialDisplacement(work.owner);
        if (work.character.name == SHEN_NAME(SF2_))
            displacement = {};
        Matrix4x4::preTranslate(displacement.x, displacement.y, -displacement.z);
        if (node.node->runtimeParent && static_cast<std::uint16_t>(node.node->runtimeParent->id) == 0xffff) {
            Matrix4x4_Push(&node.parentTransform);
            Matrix4x4::preTranslate(displacement.x, displacement.y, -displacement.z);
            node.parentTransform = workTransformMatrix;
            SetMT7NodeTransformOverride(*node.node->runtimeParent, &node.parentTransform);
            Matrix4x4::Pop(1);
        }
    }
    BindTransform(node);
}
// @verified - p
bool ChildOwnsTransform(const Matrix4x4 *transform, MT7Node *child) {
    for (; child; child = child->runtimeSibling) {
        OsageNode *const node = static_cast<OsageNode *>(child->taskData);
        if (node && node->token.name == OSAGE_COMPONENT.name && &node->parentTransform == transform)
            return true;
    }
    return false;
}
// @verified - p
void SaveParentTransforms(MT7Node *node) {
    for (; node; node = node->runtimeSibling) {
        OsageNode *const osage = static_cast<OsageNode *>(node->taskData);
        if (osage && osage->token.name == OSAGE_COMPONENT.name)
            Matrix4x4::GetTransformMatrix(&osage->parentTransform);
    }
}
// @verified - p
void ProcessNodeTree(MT7Node *model, Character *character) {
    while (model) {
        if (model->runtimeSibling)
            Matrix4x4_Push();
        OsageNode *const node = static_cast<OsageNode *>(model->taskData);
        if (node && node->token.name == OSAGE_COMPONENT.name) {
            if (node->kind >= 0) {
                if (shenIsGamePaused())
                    SetMT7NodeTransformOverride(*model, &node->transform);
                else {
                    node->updateFlags = (node->updateFlags & OsageUpdateWarmupMask) | (2 * (node->updateFlags & OsageUpdatePreviousFrames));
                    randomOscillation = {IncrementSharedPrngFloat() * 5 - 2.5f, IncrementSharedPrngFloat() * 5 - 2.5f};
                    SetMT7NodeTransformOverride(*model, nullptr);
                    switch (node->kind) {
                    case 120:
                    case 133:
                        if (node->owner->distanceLimit > hairDistance)
                            StrandSpringTransform(*node);
                        break;
                    case 121:
                    case 132:
                        if (hairDistance < 16)
                            HairStrandTransform(*node, character);
                        break;
                    case 124:
                        if (hairDistance < 16)
                            DelayedWeatherTransform(*node);
                        break;
                    case 125:
                        if (hairDistance < 64)
                            ObjectWindTransform(*node);
                        break;
                    case 126:
                        if (hairDistance < 16)
                            WeatherWindTransform(*node);
                        break;
                    case 127:
                        if (hairDistance < 64)
                            SimpleWindTransform(*node, true);
                        break;
                    case 128:
                        if (hairDistance < 64)
                            SimpleWindTransform(*node, false);
                        break;
                    case 141:
                        if (hairDistance < 16)
                            OscillationTransform(*node);
                        break;
                    default:
                        break;
                    }
                    ++node->owner->flags;
                    if (!(node->linkFlags & 0x1f) && model->runtimeParent && !GetMT7NodeTransformOverride(*model->runtimeParent))
                        SetMT7NodeTransformOverride(*model->runtimeParent, &node->parentTransform);
                    if (GetMT7NodeTransformOverride(*model)) {
                        if (node->updateFlags & OsageUpdateWarmupMask)
                            node->updateFlags += OsageUpdateWarmupStep;
                    } else {
                        node->updateFlags |= OsageUpdateWarmupStart;
                        ResetTransform(*node);
                    }
                }
            } else {
                node->kind = static_cast<std::int16_t>(model->id);
                switch (node->kind) {
                case 120:
                case 133:
                    node->facingAngle = Facing(*node, 0, 0x8000);
                    break;
                case 121:
                case 124:
                case 126:
                case 132:
                    node->facingAngle = Facing(*node, 0);
                    break;
                case 122:
                    node->facingAngle = Facing(*node, 2, 0x8000);
                    break;
                case 123:
                case 125:
                    node->facingAngle = Facing(*node, 1, 0x8000);
                    break;
                case 127:
                case 128:
                case 130:
                    node->facingAngle = Facing(*node, 2);
                    break;
                default:
                    node->facingAngle = Facing(*node, 1);
                    break;
                }
                Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
                node->current = Translation();
                node->previous = node->current;
                node->euler = model->rotation;
                ViewSpaceFromEuler(*node);
                if (model->runtimeParent && !GetMT7NodeTransformOverride(*model->runtimeParent))
                    SetMT7NodeTransformOverride(*model->runtimeParent, &node->parentTransform);
                node->endpoint = EndpointPosition(*node);
            }
        }
        const Matrix4x4 *const override = GetMT7NodeTransformOverride(*model);
        if (!override || (model->runtimeChild && ChildOwnsTransform(override, model->runtimeChild))) {
            Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
            Rotate(model->rotation);
        } else
            Matrix4x4::SetActiveTransform(override);
        if (model->runtimeChild) {
            SaveParentTransforms(model->runtimeChild);
            ProcessNodeTree(model->runtimeChild, character);
        }
        if (!model->runtimeSibling)
            break;
        Matrix4x4::Pop(1);
        model = model->runtimeSibling;
    }
}
// @verified - p
void CollisionSurfaces(const OsageCollisionPoint *points, MotionWork *motion, Vector4 *output) {
    for (std::int32_t i = 0; i < 28; ++i) {
        Matrix4x4::SetActiveTransform(charaGetBoneTransform(motion, collisionBones[i]));
        Matrix4x4::preTranslate(points[i].x * 0.0099999998f, points[i].y * 0.0099999998f, points[i].z * 0.0099999998f);
        const Vector3 position = Translation();
        output[i] = {position.x, position.y, position.z, points[i].radius * 0.0099999998f};
    }
}
}

// @verified - p
void SetCharacterOsageMode(Character *character, std::uint8_t mode) {
    if (character->model)
        if (OsageWork *const work = FindOsage(character))
            work->mode = mode;
}

// @verified - p
void mtAssignOsage(Character *character) {
    if (!character->model)
        return;
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    if (model->nodes.empty())
        return;
    bool found = false;
    for (const std::int16_t kind : osageNodeKinds) {
        if (kind < 0)
            break;
        if (GetChildObjectByIndex(*model, &model->nodes.front(), static_cast<std::uint16_t>(kind))) {
            found = true;
            break;
        }
    }
    if (!found)
        return;
    OsageWork *work = FindOsage(character);
    if (!work) {
        work = static_cast<OsageWork *>(AllocateModelComponent(character, OSAGE_COMPONENT, sizeof(OsageWork), OsageUpdateNullsub, OsageCleanup));
        if (!work)
            return;
    }
    work->owner = character;
    work->collisionOwner = nullptr;
    work->nodes = nullptr;
    work->secondaryCollisionPoints = nullptr;
    work->collisionPoints = nullptr;
    work->motionScale = 0;
    work->parameters = nullptr;
    work->mode = 0;
    if (GetChildObjectByIndex(*model, &model->nodes.front(), 120))
        work->distanceLimit = 400;
    else if (GetChildObjectByIndex(*model, &model->nodes.front(), 123) || GetChildObjectByIndex(*model, &model->nodes.front(), 129) ||
        GetChildObjectByIndex(*model, &model->nodes.front(), 130))
        work->distanceLimit = 36;
    else
        work->distanceLimit = 16;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    work->character = scenGetCharaName(control ? control->scenarioTask : nullptr, character);
    if (work->character.name) {
        ReadParameters(*work, model->nodes.front());
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        AllocateNodeTree(*work, &model->nodes.front());
        Matrix4x4::Pop(1);
    }
}

// @verified - p
void FindOSAG(Character *character) {
    if (OsageWork *const work = FindOsage(character)) {
        CleanupNodes(*work);
        work->character = {};
    }
    mtAssignOsage(character);
}

// @verified - p
void toggleOsagFlags(Character *character) {
    if (OsageWork *const work = FindOsage(character)) {
        for (OsageNode *node = work->nodes; node; node = node->next)
            node->updateFlags = (node->updateFlags & OsageUpdateHistoryMask) | OsageUpdateWarmupStart;
        SetClothFlag(character, 0x10, 1);
    }
}

// @verified - p
void UpdateCharacterOsage(MT7Node *model, Character *character) {
    MotionWork *const motion = FindMotionWork(character);
    if (!motion)
        return;
    OsageWork *const work = FindOsage(character);
    if (!work)
        return;
    if (authGetLoaded())
        hairDistance = 0;
    else if (character->entity.name.name == SHEN_NAME(CEWP) && (character->flags & CF_Valid) && IsOfThisVisibilityType(character, 0) &&
        charaGetAlphaDraw(character) >= 0.01f) {
        const Vector3 position = GetCharacterWorldPosition(character);
        Vector3 camera;
        camCtrlGetNextCamPos(&camera);
        hairDistance = ((camera.z - position.z) * (camera.z - position.z)) + ((camera.x - position.x) * (camera.x - position.x));
    } else
        hairDistance = 10000;
    Matrix4x4_Push();
    for (std::int32_t i = 0; i < 5; ++i) {
        const ReferenceAngle &reference = referenceAngles[i];
        Matrix4x4::SetActiveTransform(charaGetBoneTransform(motion, reference.bone));
        const Vector3 joint = Translation();
        Matrix4x4_Push();
        Matrix4x4::preTranslate(reference.base.x, reference.base.y, reference.base.z);
        Vector3 endpoint = Translation();
        Matrix4x4::Pop(1);
        const float vertical = endpoint.y - joint.y;
        if (vertical > 0.5f || vertical < -0.5f) {
            const float blend = vertical > 0.5f ? vertical * 2 - 1 : vertical * -2 - 1;
            const Vector3 &target = vertical > 0.5f ? reference.positive : reference.negative;
            const Vector3 offset{blend * target.x + (1 - blend) * reference.base.x, blend * target.y + (1 - blend) * reference.base.y,
                blend * target.z + (1 - blend) * reference.base.z};
            Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
            endpoint = Translation();
        }
        work->referenceAngles[i] = FixedAtan2(-(endpoint.x - joint.x), -(endpoint.z - joint.z));
    }
    if (work->mode == 4) {
        std::int8_t *const points = reinterpret_cast<std::int8_t *>(work->collisionPoints);
        points[12] = -4;
        points[32] = points[33] = points[40] = points[41] = 0;
    }
    if (work->collisionPoints) {
        CollisionSurfaces(work->collisionPoints, motion, work->collisionSurfaces.data());
        useSecondarySurfaces = false;
        if (work->collisionOwner && work->secondaryCollisionPoints)
            if (MotionWork *const secondary = FindMotionWork(work->collisionOwner)) {
                CollisionSurfaces(work->secondaryCollisionPoints, secondary, secondarySurfaces);
                useSecondarySurfaces = true;
            }
    }
    groundHeight = -100000;
    motionScale = work->motionScale;
    if (std::fabs(motionScale) < 0.00000011920929f) {
        motionScale = GetMotionSpeed(motion, MotionRegion::LowerBody);
        if (std::fabs(motionScale) < 0.00000011920929f)
            motionScale = 1;
    }
    work->flags = 0;
    inverseMotionScale = 1 / motionScale;
    work->windSpeedScaled = std::fmax(0.5f, GetEffectiveWindSpeed(GetWind()) * 30);
    std::int32_t phase = GetCurrentWindPhaseAccumulator(GetWind());
    work->wind = {Sin(phase) * work->windSpeedScaled, 0, Cos(phase) * work->windSpeedScaled};
    work->windSpeedNext = GetEffectiveWindSpeed(GetWind()) * 30;
    phase = GetCurrentWindPhaseAccumulator(GetWind());
    work->wind = {Sin(phase) * work->windSpeedNext, 0, Cos(phase) * work->windSpeedNext};
    Matrix4x4::SetPrePassToTempTransform();
    const Vector3 position = GetCharacterWorldPosition(character);
    const Vector3i euler = GetCharacterWorldRotation(character);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    SetViewSpaceTransformFromEuler(euler);
    ProcessNodeTree(model, character);
    Matrix4x4::Pop(1);
}
}
