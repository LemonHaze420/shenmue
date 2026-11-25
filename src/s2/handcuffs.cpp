#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/handcuffs_dc.hpp"

#include "s2/file_loader_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include <string.h>
#else
#include "s2/handcuffs.hpp"

#include "s2/archive_data.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/world_render.hpp"

#include <cmath>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HLib::Task *g_Handcuffs_Task;

namespace {

std::array<HandcuffsModelResource, 2> models;

Vector3 chainAnchor{};

float chainVelocity{};

std::int32_t chainArc{};

float chainRadius{};

float chainHeight{};

Vector3 chainMidpoint{};

std::uint32_t wristAngle{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

MT7Node *DetachHandcuffsChainLink(HandcuffsModelResource &resource);

} // namespace

void Handcuffs_Callback(void *taskData);
void Handcuffs_ConstrainWrist(std::uint32_t, Vector3 *position, MotionWork *, void *parameter);
std::int32_t Handcuffs_Attach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float maximumDistance);
void cuffsSetFlags();
void Handcuffs_Enqueue();
void resetHandcuffTask();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

MT7Model *LoadHandcuffsModel(std::size_t index, shen_name token) {
    HandcuffsModelResource &resource = models[index];
    const char *const path = HANDCUFF_MODEL_PATHS[index];
    const std::uint32_t size = static_cast<std::uint32_t>(ArchiveData_GetFileSize(path));
    if (!size)
        return nullptr;
    resource.allocation = HLib::AllocateTaskData(size, token);
    if (!resource.allocation)
        return nullptr;
    const std::span<const std::byte> bytes{static_cast<const std::byte *>(resource.allocation), static_cast<std::size_t>(size)};
    if (!ArchiveData_Read(path, 0, resource.allocation, bytes.size())) {
        HLib::FreeTaskData(resource.allocation);
        resource.allocation = nullptr;
        return nullptr;
    }
    return ReadModelFormat7(bytes, path, resource.model, 0) ? &resource.model : nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

HLib::Task *g_Handcuffs_Task;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetHandcuffsWristBone(std::int32_t bone);
void Handcuffs_Callback(void *parameter);
void cuffsSetFlags();
void Handcuffs_ConstrainWrist(std::uint32_t, Vector3 *position, MotionWork *, void *parameter);
std::int32_t Handcuffs_Attach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float distance);
void Handcuffs_Enqueue();
void resetHandcuffTask();
void FreeHandcuffsTaskStorage();

namespace {

MT7Node *DetachHandcuffsChainLink(MT7Node *node);

} // namespace
#else
namespace {

MT7Model *LoadHandcuffsModel(std::size_t index, shen_name token);
MT7Node *DetachHandcuffsChainLink(HandcuffsModelResource &resource);
void DrawCuffAtWrist(const Matrix4x4 *bone, const Vector3 &otherWrist, const Vector3 &attachment, const MT7Model &model, Vector3 &chainEnd);

} // namespace

void Handcuffs_BuildChainTransform(const Vector3 &first, const Vector3 &second, Matrix4x4 &transform);
void Handcuffs_Render(void *taskData);
void Handcuffs_Callback(void *taskData);
void cuffsSetFlags();
void Handcuffs_ConstrainWrist(std::uint32_t, Vector3 *position, MotionWork *, void *parameter);
std::int32_t Handcuffs_Attach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float maximumDistance);
void Handcuffs_Enqueue();
void resetHandcuffTask();
void ReleaseHandcuffsModels();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t GetHandcuffsWristBone(std::int32_t bone) {
    return bone == 17 ? 21 : 14;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void Handcuffs_Callback(void *parameter) {
    HandcuffsTaskData *const data = static_cast<HandcuffsTaskData *>(parameter);
    if (data->active == 1 && data->highModel)
        HLib::ExecuteCallbackNow(Handcuffs_Render, data);
}

#else

// @verified - p
void Handcuffs_Callback(void *taskData) {
    HandcuffsTaskData *const data = static_cast<HandcuffsTaskData *>(taskData);
    if (data->active == 1 && data->highModel)
        HLib::ExecuteCallbackNow(Handcuffs_Render, data);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void Handcuffs_ConstrainWrist(std::uint32_t, Vector3 *position, MotionWork *, void *parameter) {
    const HandcuffConstraint *const constraint = static_cast<HandcuffConstraint *>(parameter);
    const HandcuffConstraint *const peer = constraint->peer;
    Vector3 peerPosition;
    mtwkGetBonePosition(charFindMotionwork(peer->character), static_cast<MotionBoneId>(peer->bone), &peerPosition);
    Vector3 delta;
    delta.x = position->x - peerPosition.x;
    delta.y = position->y - peerPosition.y;
    delta.z = position->z - peerPosition.z;
    const float distance = Vector3::CalcMagnitude(delta);
    if (distance > constraint->maximumDistance && distance < 3.0f) {
        position->x = delta.x / distance * constraint->maximumDistance + peerPosition.x;
        position->y = delta.y / distance * constraint->maximumDistance + peerPosition.y;
        position->z = delta.z / distance * constraint->maximumDistance + peerPosition.z;
    }
}

#else

void Handcuffs_ConstrainWrist(std::uint32_t, Vector3 *position, MotionWork *, void *parameter) {
    const HandcuffConstraint &constraint = *static_cast<const HandcuffConstraint *>(parameter);
    Vector3 peerPosition{};
    mtwkGetBonePosition(FindMotionWork(constraint.peer->character), static_cast<MotionBoneId>(constraint.peer->bone), &peerPosition);
    const Vector3 delta = Vector3::Subtract(*position, peerPosition);
    const float distance = Vector3::CalcMagnitude(delta);
    if (distance > constraint.maximumDistance && distance < 3.0f) {
        position->x = delta.x / distance * constraint.maximumDistance + peerPosition.x;
        position->y = delta.y / distance * constraint.maximumDistance + peerPosition.y;
        position->z = delta.z / distance * constraint.maximumDistance + peerPosition.z;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void cuffsSetFlags() {
    if (!g_Handcuffs_Task)
        return;
    HandcuffsTaskData *const data = static_cast<HandcuffsTaskData *>(HLib::GetTaskParameter(g_Handcuffs_Task));
    if (data->active != 1)
        return;
    data->active = 0;
    MotionWork *const first = charFindMotionwork(data->first.character);
    MotionWork *const second = charFindMotionwork(data->second.character);
    mtwkAssignTargetParameter(first, 0, data->first.bone);
    mtwkAssignTargetParameter(second, 0, data->second.bone);
    mtwkAssignTargetCallback(first, 0, data->first.bone);
    mtwkAssignTargetCallback(second, 0, data->second.bone);
}

#else

// @verified - p
void cuffsSetFlags() {
    if (!g_Handcuffs_Task)
        return;
    HandcuffsTaskData &data = *static_cast<HandcuffsTaskData *>(HLib::GetTaskParameter(g_Handcuffs_Task));
    if (data.active != 1)
        return;
    data.active = 0;
    MotionWork *const firstMotion = FindMotionWork(data.first.character);
    MotionWork *const secondMotion = FindMotionWork(data.second.character);
    mtwkAssignTargetParameter(firstMotion, nullptr, data.first.bone);
    mtwkAssignTargetParameter(secondMotion, nullptr, data.second.bone);
    mtwkAssignTargetCallback(firstMotion, nullptr, data.first.bone);
    mtwkAssignTargetCallback(secondMotion, nullptr, data.second.bone);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t Handcuffs_Attach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float distance) {
    if (!g_Handcuffs_Task)
        return -1;
    Character *const first = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, firstCharacter);
    Character *const second = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, secondCharacter);
    HandcuffsTaskData *const data = static_cast<HandcuffsTaskData *>(HLib::GetTaskParameter(g_Handcuffs_Task));
    if (!first || !second || !data)
        return -1;
    if (0.1f > distance)
        distance = 0.1f;
    if (distance > 0.4f)
        distance = 0.4f;
    data->active = 1;
    HandcuffConstraint *const firstCuff = &data->first;
    HandcuffConstraint *const secondCuff = &data->second;
    firstCuff->peer = secondCuff;
    firstCuff->character = first;
    firstCuff->bone = firstHand ? 17 : 10;
    secondCuff->peer = firstCuff;
    secondCuff->character = second;
    secondCuff->bone = secondHand ? 17 : 10;
    data->chainCount = 10;
    firstCuff->maximumDistance = distance;
    secondCuff->maximumDistance = distance;
    firstCuff->flags = 4;
    secondCuff->flags = 8;
    firstCuff->model = data->highModel;
    secondCuff->model = data->highModel;
    MotionWork *const firstMotion = charFindMotionwork(first);
    MotionWork *const secondMotion = charFindMotionwork(second);
    mtwkAssignTargetParameter(firstMotion, firstCuff, firstCuff->bone);
    mtwkAssignTargetParameter(secondMotion, secondCuff, secondCuff->bone);
    mtwkAssignTargetCallback(firstMotion, Handcuffs_ConstrainWrist, firstCuff->bone);
    mtwkAssignTargetCallback(secondMotion, Handcuffs_ConstrainWrist, secondCuff->bone);
    Vector3 firstPosition, secondPosition;
    mtwkGetBonePosition(firstMotion, static_cast<MotionBoneId>(firstCuff->bone), &firstPosition);
    mtwkGetBonePosition(secondMotion, static_cast<MotionBoneId>(secondCuff->bone), &secondPosition);
    Vector3 firstWrist, secondWrist;
    firstWrist.x = firstPosition.x;
    firstWrist.y = firstPosition.y;
    firstWrist.z = firstPosition.z;
    secondWrist.x = secondPosition.x;
    secondWrist.y = secondPosition.y;
    secondWrist.z = secondPosition.z;
    am2Vector3Combine(&secondWrist, &firstWrist, 0.5f, 0.5f, &g_handcuffChainMidpoint);
    g_handcuffChainAnchor.x = g_handcuffChainMidpoint.x;
    g_handcuffChainAnchor.y = g_handcuffChainMidpoint.y - 2.0f;
    g_handcuffChainAnchor.z = g_handcuffChainMidpoint.z;
    g_handcuffChainVelocity = 0.0f;
    return 0;
}

#else

std::int32_t Handcuffs_Attach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float maximumDistance) {
    if (!g_Handcuffs_Task)
        return -1;
    Character *const first = ctrlGetScenarioChara(firstCharacter);
    Character *const second = ctrlGetScenarioChara(secondCharacter);
    HandcuffsTaskData *const data = static_cast<HandcuffsTaskData *>(HLib::GetTaskParameter(g_Handcuffs_Task));
    if (!first || !second || !data)
        return -1;
    data->active = 1;
    data->first.peer = &data->second;
    data->first.character = first;
    data->first.bone = firstHand ? 17 : 10;
    data->second.peer = &data->first;
    data->second.character = second;
    data->second.bone = secondHand ? 17 : 10;
    data->chainCount = 10;
    const float lowerBoundedDistance = 0.1f > maximumDistance ? 0.1f : maximumDistance;
    data->first.maximumDistance = 0.4f < lowerBoundedDistance ? 0.4f : lowerBoundedDistance;
    data->second.maximumDistance = data->first.maximumDistance;
    data->first.flags = 4;
    data->second.flags = 8;
    data->first.model = data->highModel;
    data->second.model = data->highModel;
    MotionWork *const firstMotion = FindMotionWork(first);
    MotionWork *const secondMotion = FindMotionWork(second);
    mtwkAssignTargetParameter(firstMotion, &data->first, data->first.bone);
    mtwkAssignTargetParameter(secondMotion, &data->second, data->second.bone);
    mtwkAssignTargetCallback(firstMotion, Handcuffs_ConstrainWrist, data->first.bone);
    mtwkAssignTargetCallback(secondMotion, Handcuffs_ConstrainWrist, data->second.bone);
    Vector3 firstPosition{}, secondPosition{};
    mtwkGetBonePosition(firstMotion, static_cast<MotionBoneId>(data->first.bone), &firstPosition);
    mtwkGetBonePosition(secondMotion, static_cast<MotionBoneId>(data->second.bone), &secondPosition);
    chainMidpoint = {(firstPosition.x + secondPosition.x) * 0.5f, (firstPosition.y + secondPosition.y) * 0.5f, (firstPosition.z + secondPosition.z) * 0.5f};
    chainAnchor = chainMidpoint;
    chainAnchor.y -= 2.0f;
    chainVelocity = 0.0f;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void resetHandcuffTask() {
    g_Handcuffs_Task = 0;
}

void FreeHandcuffsTaskStorage() {
    if (g_Handcuffs_Task)
        HLib::FreeTaskData(reinterpret_cast<void *>(static_cast<HandcuffsTaskData *>(HLib::GetTaskParameter(g_Handcuffs_Task))->retained54));
}

#else

// @verified - p
void resetHandcuffTask() {
    g_Handcuffs_Task = nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ReleaseHandcuffsModels() {
    for (HandcuffsModelResource &resource : models) {
        ReleaseModelFormat7(resource.model);
        if (resource.allocation)
            HLib::FreeTaskData(resource.allocation);
        resource.allocation = nullptr;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void Handcuffs_Enqueue() {
    if (g_Handcuffs_Task)
        return;
    MT7Node *model = ParseMT7ModelRoot(shenLoadFile("misc", "TEJOB01G.MT7", MODEL_HANDCUFF_HIGH), 0);
    g_Handcuffs_Task = HLib::EnqueueTaskWithParameter(Handcuffs_Callback, 12, 0, sizeof(HandcuffsTaskData), TASK_CUFF);
    HandcuffsTaskData *const data = static_cast<HandcuffsTaskData *>(HLib::GetTaskParameter(g_Handcuffs_Task));
    std::size_t size = sizeof(*data);
    memset(data, 0, size);
    data->highModel = model;
    if (model) {
        data->highChainLink = DetachHandcuffsChainLink(model);
        data->attachmentOffset.x = data->highChainLink->position.x;
        data->attachmentOffset.y = data->highChainLink->position.y - 0.015f;
        data->attachmentOffset.z = data->highChainLink->position.z + 0.0049999999f;
        data->highChainLink->position.x = 0.0f;
        data->highChainLink->position.y = 0.0f;
        data->highChainLink->position.z = 0.0f;
    }
    model = ParseMT7ModelRoot(shenLoadFile("misc", "TEJOB00G.MT7", MODEL_HANDCUFF_LOW), 0);
    data->lowModel = model;
    if (model) {
        data->lowChainLink = DetachHandcuffsChainLink(model);
        data->lowChainLink->position.x = 0.0f;
        data->lowChainLink->position.y = 0.0f;
        data->lowChainLink->position.z = 0.0f;
    }
}

#else

void Handcuffs_Enqueue() {
    if (g_Handcuffs_Task)
        return;
    MT7Model *const high = LoadHandcuffsModel(0, MODEL_HANDCUFF_HIGH);
    g_Handcuffs_Task = HLib::EnqueueTaskWithParameter(Handcuffs_Callback, 12, 0, sizeof(HandcuffsTaskData), TASK_CUFF);
    HandcuffsTaskData &data = *static_cast<HandcuffsTaskData *>(HLib::GetTaskParameter(g_Handcuffs_Task));
    std::memset(&data, 0, sizeof(data));
    data.highModel = high;
    if (high) {
        data.highChainLink = DetachHandcuffsChainLink(models[0]);
        data.attachmentOffset = data.highChainLink->position;
        data.attachmentOffset.y -= 0.015f;
        data.attachmentOffset.z += 0.0049999999f;
        data.highChainLink->position = {};
    }
    data.lowModel = LoadHandcuffsModel(1, MODEL_HANDCUFF_LOW);
    if (data.lowModel) {
        data.lowChainLink = DetachHandcuffsChainLink(models[1]);
        data.lowChainLink->position = {};
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

MT7Node *DetachHandcuffsChainLink(MT7Node *node) {
    if (!node)
        return 0;
    MT7Node *previous = node;
    for (;;) {
        MT7Node *next;
        if (node->sibling)
            next = node->sibling;
        else if (node->child)
            next = node->child;
        else
            break;
        previous = node;
        node = next;
    }
    if (node == previous->child)
        previous->child = 0;
    else
        previous->sibling = 0;
    return node;
}

} // namespace

#else

namespace {

MT7Node *DetachHandcuffsChainLink(HandcuffsModelResource &resource) {
    MT7Node *current = const_cast<MT7Node *>(GetMT7ReaderData(&resource.model));
    if (!current)
        return nullptr;
    MT7Node *predecessor = current;
    for (;;) {
        MT7Node *next = mtGetSibling(current);
        if (!next)
            next = mtGetTransform(current);
        if (!next)
            break;
        predecessor = current;
        current = next;
    }
    if (current == mtGetTransform(predecessor))
        mt7_UpdateChildOffset(predecessor, nullptr);
    else
        mt7_UpdateSiblingOffset(predecessor, nullptr);
    return current;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void DrawCuffAtWrist(const Matrix4x4 *bone, const Vector3 &otherWrist, const Vector3 &attachment, const MT7Model &model, Vector3 &chainEnd) {
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(bone);
    Matrix4x4 placement{};
    Matrix4x4::GetTransformMatrix(&placement);
    Matrix4x4::InvertInPlace(nullptr);
    Vector3 otherLocal{};
    Matrix4x4::TransformPoint(otherWrist, otherLocal);
    wristAngle = FixedAtan2(otherLocal.y, otherLocal.z);
    Matrix4x4::SetActiveTransform(&placement);
    Matrix4x4::RotateAroundY_Wrapper(0x4000);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(0x4000 - wristAngle));
    Matrix4x4::TransformPoint(attachment, chainEnd);
    Matrix4x4::GetTransformMatrix(&placement);
    Matrix4x4::Pop(1);

    Matrix4x4_Push();
    Matrix4x4::MultiplyWithPrePassMatrix(&placement);
    DrawMT7Model(model);
    Matrix4x4::Pop(1);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void Handcuffs_BuildChainTransform(const Vector3 &first, const Vector3 &second, Matrix4x4 &transform) {
    const Vector3 midpoint{(first.x + second.x) * 0.5f, (first.y + second.y) * 0.5f, (first.z + second.z) * 0.5f};
    Vector3 along = Vector3::Subtract(first, second);
    Vector3 sag = Vector3::Subtract(midpoint, chainAnchor);
    Vector3 perpendicular = Vector3::Cross(along, sag);
    sag = Vector3::Cross(perpendicular, along);
    const float distance = Vector3::CalcMagnitude(along);
    if (!(distance < 0.2f)) {
        chainRadius = distance / (sin_fixed16(6371) * 2.0f);
        chainArc = 12743;
        chainHeight = -cos_fixed16(6371) * chainRadius;
    } else {
        const float arcRadians = std::sqrt(24.0f - distance * 120.0f);
        chainRadius = 0.2f / arcRadians;
        chainArc = static_cast<std::uint16_t>(TruncateToInt32(arcRadians * 10430.378f));
        chainHeight = -cos_fixed16(static_cast<std::uint16_t>(chainArc >> 1)) * chainRadius;
    }
    Vector3::NormalizeInPlace(along);
    Vector3::NormalizeInPlace(sag);
    Vector3::NormalizeInPlace(perpendicular);
    Vector3 force = Vector3::Subtract(midpoint, chainAnchor);
    force.x += HANDCUFF_CHAIN_FORCE.x;
    force.y += HANDCUFF_CHAIN_FORCE.y;
    force.z += HANDCUFF_CHAIN_FORCE.z;
    chainVelocity += (perpendicular.x * force.x + perpendicular.y * force.y) + perpendicular.z * force.z;
    chainAnchor.x += perpendicular.x * chainVelocity;
    chainAnchor.y += perpendicular.y * chainVelocity;
    chainAnchor.z += perpendicular.z * chainVelocity;
    chainMidpoint = midpoint;
    transform = {{{along.x, along.y, along.z, 0.0f}, {sag.x, sag.y, sag.z, 0.0f}, {perpendicular.x, perpendicular.y, perpendicular.z, 0.0f},
        {chainMidpoint.x, chainMidpoint.y, chainMidpoint.z, 1.0f}}};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void Handcuffs_Render(void *taskData) {
    HandcuffsTaskData &data = *static_cast<HandcuffsTaskData *>(taskData);
    MotionWork *const firstMotion = FindMotionWork(data.first.character);
    MotionWork *const secondMotion = FindMotionWork(data.second.character);
    const Matrix4x4 *const firstBone = mtwkGetBoneTransform(firstMotion, data.first.bone == 17 ? 21 : 14);
    const Matrix4x4 *const secondBone = mtwkGetBoneTransform(secondMotion, data.second.bone == 17 ? 21 : 14);
    Vector3 firstWrist{}, secondWrist{};
    mtwkGetBonePosition(firstMotion, static_cast<MotionBoneId>(data.first.bone), &firstWrist);
    mtwkGetBonePosition(secondMotion, static_cast<MotionBoneId>(data.second.bone), &secondWrist);
    const Vector3 midpoint{(firstWrist.x + secondWrist.x) * 0.5f, (firstWrist.y + secondWrist.y) * 0.5f, (firstWrist.z + secondWrist.z) * 0.5f};
    Vector3 viewMidpoint{};
    Matrix4x4::TransformPoint(midpoint, viewMidpoint);
    const bool useLowModel = !(viewMidpoint.z < 0.5f) || viewMidpoint.z <= -8.0f;
    MT7Model *const model = useLowModel ? data.lowModel : data.highModel;
    MT7Node *const chainLink = useLowModel ? data.lowChainLink : data.highChainLink;
    const std::int32_t previousLight = GetCurrentActiveLight();
    SetCurrentActiveLight(3);
    ResetLightEntry(15);
    SetLightKind(15, 2);
    SetDynamicLightEnabled(15, true);
    Vector3 direction = Vector3::Subtract(midpoint, g_cameraControl->nextPosition);
    Vector3::NormalizeInPlace(direction);
    SetSelectedLightDirection(15, direction);
    shenSetLightColour(15, {1.0f, 0.4f, 0.4f, 0.4f});
    UpdateLegacyLightTransforms();

    Vector3 firstEnd{}, secondEnd{};
    DrawCuffAtWrist(firstBone, secondWrist, data.attachmentOffset, *model, firstEnd);
    DrawCuffAtWrist(secondBone, firstWrist, data.attachmentOffset, *model, secondEnd);
    Matrix4x4 chainTransform{}, linkTransform{};
    Handcuffs_BuildChainTransform(firstEnd, secondEnd, chainTransform);
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&chainTransform);
    Matrix4x4::preTranslate(0.0f, chainHeight, 0.0f);
    Matrix4x4::GetTransformMatrix(&chainTransform);
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(0.0f, chainRadius, 0.0f);
    Matrix4x4::RotateAroundZ_Wrapper(0x4000);
    Matrix4x4::GetTransformMatrix(&linkTransform);
    Matrix4x4::Pop(1);
    std::int32_t linkDegrees = 90;
    for (std::int32_t link = 0; link <= data.chainCount; ++link, linkDegrees += 90) {
        Matrix4x4_Push();
        Matrix4x4::MultiplyWithPrePassMatrix(&chainTransform);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(chainArc * link / data.chainCount - chainArc / 2));
        Matrix4x4::MultiplyWithPrePassMatrix(&linkTransform);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(TruncateToInt32(static_cast<float>(linkDegrees) * 8192.0f / 45.0f)));
        DrawMT7NodeHierarchy(*model, *chainLink);
        Matrix4x4::Pop(1);
    }
    SetCurrentActiveLight(previousLight);
    ResetLightEntry(15);
    SetDynamicLightEnabled(15, false);
    UpdateLegacyLightTransforms();
}

#endif

} // namespace s2
} // namespace shenmue
