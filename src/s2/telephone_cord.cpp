#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/telephone_cord_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/math.hpp"
#include "platform/dc/am2_vector.hpp"
#include <math.h>
#include <string.h>

namespace shenmue {
namespace dc {
namespace sdk {
void DrawSingleLightMT7Mesh(void *mesh);
}
} // namespace dc
} // namespace shenmue
#else
#include "s2/telephone_cord.hpp"

#include "s2/event_runtime.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/pak.hpp"

#include <cmath>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HLib::Task *g_telephoneCordTask;

namespace {

MT7Model telephoneCordModel;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetTelephoneCordAttachments(TelephoneCordGeometry *geometry, const Vector3 *receiver, const Vector3 *base);
void DrawTelephoneCordGeometry(TelephoneCordGeometry *geometry);
void TelephoneCordDrawCallback(void *taskData);
void TelephoneCordCallback(void *taskData);
void EnqueueTelephoneCord();
void CleanupTelephoneCord();
void telCodeInit();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

Vector3 AttachmentPosition(Character *character, const Vector3 &attachment) {
    const Vector3 position = GetCharacterWorldPosition(character);
    const Vector3i rotation = GetCharacterWorldRotation(character);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::preTranslate(attachment.x, attachment.y, attachment.z);
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void InitializeTelephoneCordGeometry(TelephoneCordGeometry *geometry, const MT7Node *mesh, float length, std::int32_t count) {
    std::memset(geometry, 0, sizeof(*geometry));
    geometry->segmentLength = length;
    geometry->mesh = mesh;
    geometry->segmentCount = count;
    geometry->force.y = 100.0f;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float g_telephoneCordReceiverOffsetX = -0.085f;
float g_telephoneCordReceiverOffsetY = 0.07f;
float g_telephoneCordReceiverOffsetZ = -0.09f;
float g_telephoneCordBaseOffsetX = -0.095f;
float g_telephoneCordBaseOffsetY = -0.01f;
float g_telephoneCordBaseOffsetZ = 0.01f;

void InitializeTelephoneCordGeometry(TelephoneCordGeometry *geometry, void *mesh, float length, std::int32_t count);

HLib::Task *g_telephoneCordTask;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetTelephoneCordAttachments(TelephoneCordGeometry *geometry, const Vector3 *receiver, const Vector3 *base) {
    geometry->receiverAttachment.x = receiver->x;
    geometry->receiverAttachment.y = receiver->y;
    geometry->receiverAttachment.z = receiver->z;
    geometry->baseAttachment.x = base->x;
    geometry->baseAttachment.y = base->y;
    geometry->baseAttachment.z = base->z;
}

#else

// @verified - p
void SetTelephoneCordAttachments(TelephoneCordGeometry *geometry, const Vector3 *receiver, const Vector3 *base) {
    geometry->receiverAttachment = *receiver;
    geometry->baseAttachment = *base;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void DrawTelephoneCordGeometry(TelephoneCordGeometry *geometry) {
    Matrix4x4 placement;
    Matrix4x4 segment;
    Vector3 first;
    Vector3 along;
    Vector3 sag;
    Vector3 perpendicular;
    Vector3 midpoint;
    Vector3 force;
    float distance;
    if (!geometry->mesh)
        return;
    Vector3 *const base = &geometry->baseAttachment;
    Vector3 *const receiver = &geometry->receiverAttachment;
    am2Vector3Combine(base, receiver, 0.5f, 0.5f, &midpoint);
    am2Vector3Subtract(&along, base, receiver);
    sag.x = 0.0f;
    sag.y = -1.0f;
    sag.z = 0.0f;
    am2Vector3Cross(&along, &sag, &perpendicular);
    am2Vector3Cross(&perpendicular, &along, &sag);
    distance = GetEuclideanDistance(base, receiver);
    if (distance < 0.2f) {
        const float radians = ::sqrtf(24.0f - 120.0f * distance);
        geometry->arc = static_cast<std::uint16_t>(10430.378f * radians);
        geometry->radius = 0.2f / radians;
        geometry->height = -(geometry->radius * CalculateFixedCosine(geometry->arc / 2));
    } else {
        float degrees = 85.0f - distance / (static_cast<float>(geometry->segmentCount) * geometry->segmentLength * 8.0f) * 80.0f;
        if (degrees > 85.0f)
            degrees = 85.0f;
        else if (degrees < 5.0f)
            degrees = 5.0f;
        geometry->arc = static_cast<std::uint16_t>(8192.0f * degrees / 45.0f);
        const float sine = CalculateFixedSine(static_cast<std::uint32_t>(geometry->arc) >> 1);
        geometry->radius = distance / (sine + sine);
        geometry->height = -geometry->radius * CalculateFixedCosine(geometry->arc / 2);
    }
    am2Vector3NormalizeInPlace(&along);
    am2Vector3NormalizeInPlace(&sag);
    am2Vector3NormalizeInPlace(&perpendicular);
    am2Vector3Subtract(&force, &midpoint, &geometry->anchor);
    am2Vector3AddInPlace(&force, &geometry->force);
    geometry->velocity = am2Vector3Dot(&perpendicular, &force) + geometry->velocity;
    am2Vector3Scale(&force, &perpendicular, geometry->velocity);
    am2Vector3AddInPlace(&geometry->anchor, &force);
    geometry->anchor = midpoint;
    placement.rows[0].x = along.x;
    placement.rows[1].x = sag.x;
    placement.rows[2].x = perpendicular.x;
    placement.rows[0].y = along.y;
    placement.rows[1].y = sag.y;
    placement.rows[2].y = perpendicular.y;
    placement.rows[0].z = along.z;
    placement.rows[1].z = sag.z;
    placement.rows[2].z = perpendicular.z;
    placement.rows[0].w = 0.0f;
    placement.rows[1].w = 0.0f;
    placement.rows[2].w = 0.0f;
    placement.rows[3].w = 1.0f;
    placement.rows[3].x = geometry->anchor.x;
    placement.rows[3].y = geometry->anchor.y;
    placement.rows[3].z = geometry->anchor.z;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(0.0f, geometry->radius, 0.0f);
    matrix_stack::GetMatrix(&segment);
    matrix_stack::LoadMatrix(&placement);
    Matrix4x4::preTranslate(0.0f, geometry->height, 0.0f);
    matrix_stack::GetMatrix(&placement);
    Matrix4x4::RotateAroundZ_Wrapper(-geometry->arc / 2 + geometry->arc / geometry->segmentCount);
    Matrix4x4::MultiplyWithPrePassMatrix(&segment);
    matrix_stack::GetTranslation(&first);
    distance = GetEuclideanDistance(&first, &geometry->baseAttachment);
    if (distance > geometry->segmentLength) {
        distance = distance / geometry->segmentLength;
        matrix_stack::LoadMatrix(&segment);
        Matrix4x4::Scale(distance, 1.0f, 1.0f);
        matrix_stack::GetMatrix(&segment);
    }
    matrix_stack::GetPreviousMatrix(0);
    Matrix4x4::MultiplyWithPrePassMatrix(&placement);
    matrix_stack::Matrix4x4_Push();
    for (std::int32_t index = 0; index < geometry->segmentCount;) {
        Matrix4x4::RotateAroundZ_Wrapper(-geometry->arc / 2 + index * geometry->arc / geometry->segmentCount);
        Matrix4x4::MultiplyWithPrePassMatrix(&segment);
        dc::sdk::DrawSingleLightMT7Mesh(geometry->mesh);
        ++index;
        matrix_stack::GetPreviousMatrix(0);
    }
    PopCurrentMatrix(1);
    PopCurrentMatrix(1);
}

#else

// @verified - p
void DrawTelephoneCordGeometry(TelephoneCordGeometry *geometry) {
    if (!geometry->mesh)
        return;
    const Vector3 midpoint{geometry->baseAttachment.x * 0.5f + geometry->receiverAttachment.x * 0.5f,
        geometry->baseAttachment.y * 0.5f + geometry->receiverAttachment.y * 0.5f, geometry->baseAttachment.z * 0.5f + geometry->receiverAttachment.z * 0.5f};
    Vector3 along = Vector3::Subtract(geometry->baseAttachment, geometry->receiverAttachment);
    Vector3 sag{0.0f, -1.0f, 0.0f};
    Vector3 perpendicular = Vector3::Cross(along, sag);
    sag = Vector3::Cross(perpendicular, along);
    const float distance = Vector3::CalcMagnitude(along);
    if (!(distance < 0.2f)) {
        const float candidate = 85.0f - distance / (static_cast<float>(geometry->segmentCount) * geometry->segmentLength * 8.0f) * 80.0f;
        const float degrees = candidate > 85.0f ? 85.0f : candidate < 5.0f ? 5.0f : candidate;
        geometry->arc = FloatToShort(degrees * 8192.0f / 45.0f);
        geometry->radius = distance / (sin_fixed16(static_cast<std::uint16_t>(geometry->arc >> 1)) * 2.0f);
    } else {
        const float radians = std::sqrt(24.0f - distance * 120.0f);
        geometry->radius = 0.2f / radians;
        geometry->arc = FloatToShort(radians * 10430.378f);
    }
    geometry->height = -(cos_fixed16(static_cast<std::uint16_t>(geometry->arc / 2)) * geometry->radius);
    NormalizeVector3(&along);
    NormalizeVector3(&sag);
    NormalizeVector3(&perpendicular);
    Vector3 force = Vector3::Subtract(midpoint, geometry->anchor);
    force.x += geometry->force.x;
    force.y += geometry->force.y;
    force.z += geometry->force.z;
    geometry->velocity = ((perpendicular.x * force.x + perpendicular.y * force.y) + perpendicular.z * force.z) + geometry->velocity;
    geometry->anchor.x += perpendicular.x * geometry->velocity;
    geometry->anchor.y += perpendicular.y * geometry->velocity;
    geometry->anchor.z += perpendicular.z * geometry->velocity;
    geometry->anchor.x = midpoint.x;
    geometry->anchor.y = midpoint.y;
    geometry->anchor.z = midpoint.z;
    Matrix4x4 placement{{{along.x, along.y, along.z, 0.0f}, {sag.x, sag.y, sag.z, 0.0f}, {perpendicular.x, perpendicular.y, perpendicular.z, 0.0f},
        {geometry->anchor.x, geometry->anchor.y, geometry->anchor.z, 1.0f}}};
    Matrix4x4 segment;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(0.0f, geometry->radius, 0.0f);
    Matrix4x4::GetTransformMatrix(&segment);
    Matrix4x4::SetActiveTransform(&placement);
    Matrix4x4::preTranslate(0.0f, geometry->height, 0.0f);
    Matrix4x4::GetTransformMatrix(&placement);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(geometry->arc / geometry->segmentCount - geometry->arc / 2));
    Matrix4x4::MultiplyWithPrePassMatrix(&segment);
    const Vector3 first{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    const float firstDistance = Vector3::CalcMagnitude(Vector3::Subtract(first, geometry->baseAttachment));
    float scale = 1.1f;
    if (firstDistance > geometry->segmentLength) {
        const float ratio = firstDistance / geometry->segmentLength;
        Matrix4x4::SetActiveTransform(&segment);
        Matrix4x4::Scale(ratio, 1.0f, 1.0f);
        Matrix4x4::GetTransformMatrix(&segment);
        scale = ratio * 1.1f;
    }
    workTransformMatrix = *(g_matrixStack.top.stack - 1);
    Matrix4x4::MultiplyWithPrePassMatrix(&placement);
    Matrix4x4_Push();
    for (std::int32_t index = 0; index < geometry->segmentCount; ++index) {
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(index * geometry->arc / geometry->segmentCount - geometry->arc / 2));
        Matrix4x4::MultiplyWithPrePassMatrix(&segment);
        SetLegacyModelScale(scale);
        ModelFlagDispatch_SetModelFlagDataOpaque(telephoneCordModel.source, geometry->mesh->xb01Offset,
            geometry->mesh->borrowedMesh ? geometry->mesh->borrowedMesh->drawResources : geometry->mesh->drawResources);
        workTransformMatrix = *(g_matrixStack.top.stack - 1);
    }
    Matrix4x4::Pop(1);
    Matrix4x4::Pop(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void TelephoneCordDrawCallback(void *taskData) {
    dc::sdk::SetModelScale(10.0f);
    DrawTelephoneCordGeometry(&static_cast<TelephoneCordTaskData *>(taskData)->geometry);
    dc::sdk::SetModelScale(1.0f);
}

#else

// @verified - p
void TelephoneCordDrawCallback(void *taskData) {
    DrawTelephoneCordGeometry(&static_cast<TelephoneCordTaskData *>(taskData)->geometry);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void TelephoneCordCallback(void *taskData) {
    TelephoneCordTaskData *const data = static_cast<TelephoneCordTaskData *>(taskData);
    if (!data->root)
        return;
    Vector3 receiver, base;
    Vector3i receiverRotation, baseRotation;
    matrix_stack::PushUnitMatrix();
    GetCharacterWorldPosition(data->receiver, &receiver);
    GetCharacterWorldRotation(data->receiver, &receiverRotation);
    Matrix4x4::preTranslate(receiver.x, receiver.y, receiver.z);
    Matrix4x4::SetViewMatrixFromEuler(receiverRotation.x, receiverRotation.y, receiverRotation.z);
    Matrix4x4::preTranslate(g_telephoneCordReceiverOffsetX, g_telephoneCordReceiverOffsetY, g_telephoneCordReceiverOffsetZ);
    matrix_stack::GetTranslation(&receiver);
    matrix_stack::UnitCurrentMatrix();
    GetCharacterWorldPosition(data->base, &base);
    GetCharacterWorldRotation(data->base, &baseRotation);
    Matrix4x4::preTranslate(base.x, base.y, base.z);
    Matrix4x4::SetViewMatrixFromEuler(baseRotation.x, baseRotation.y, baseRotation.z);
    Matrix4x4::preTranslate(g_telephoneCordBaseOffsetX, g_telephoneCordBaseOffsetY, g_telephoneCordBaseOffsetZ);
    matrix_stack::GetTranslation(&base);
    PopCurrentMatrix(1);
    SetTelephoneCordAttachments(&data->geometry, &receiver, &base);
    HLib::ExecuteCallbackNow(TelephoneCordDrawCallback, data);
}

#else

// @verified - p
void TelephoneCordCallback(void *taskData) {
    TelephoneCordTaskData *const data = static_cast<TelephoneCordTaskData *>(taskData);
    if (!data->reader)
        return;
    Matrix4x4_PushActiveTransform();
    const Vector3 receiver = AttachmentPosition(data->receiver, TELEPHONE_CORD_RECEIVER_ATTACHMENT);
    Matrix4x4::SetPrePassToTempTransform();
    const Vector3 base = AttachmentPosition(data->base, TELEPHONE_CORD_BASE_ATTACHMENT);
    Matrix4x4::Pop(1);
    SetTelephoneCordAttachments(&data->geometry, &receiver, &base);
    HLib::ExecuteCallbackNow(TelephoneCordDrawCallback, data);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetTelephoneCordTask() {
    g_telephoneCordTask = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EnqueueTelephoneCord() {
    if (g_telephoneCordTask)
        return;
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const receiver = scenFindChara(control->scenarioTask, TELEPHONE_CORD_RECEIVER);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const base = scenFindChara(control->scenarioTask, TELEPHONE_CORD_BASE);
    if (!receiver || !base)
        return;
    g_telephoneCordTask = HLib::EnqueueTaskWithParameter(TelephoneCordCallback, 12, 0, sizeof(TelephoneCordTaskData), TELEPHONE_CORD_TASK);
    if (!g_telephoneCordTask)
        return;
    TelephoneCordTaskData *const data = static_cast<TelephoneCordTaskData *>(HLib::GetTaskParameter(g_telephoneCordTask));
    if (!data)
        return;
    std::size_t size = sizeof(*data);
    memset(data, 0, size);
    data->receiver = receiver;
    data->base = base;
    data->allocation = shenLoadFile("misc", "DENR003G.MT7", TELEPHONE_CORD_TASK);
    if (!data->allocation)
        return;
    data->root = ParseMT7ModelRoot(data->allocation, 0);
    if (data->root)
        InitializeTelephoneCordGeometry(&data->geometry, data->root->mesh, 0.02f, 30);
}

#else

// @verified - p
void EnqueueTelephoneCord() {
    if (g_telephoneCordTask)
        return;
    const MapControlData *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const receiver = GetScenarioCharacterNode(control->scenarioTask, TELEPHONE_CORD_RECEIVER);
    control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const base = GetScenarioCharacterNode(control->scenarioTask, TELEPHONE_CORD_BASE);
    if (!receiver || !base)
        return;
    g_telephoneCordTask = HLib::EnqueueTaskWithParameter(TelephoneCordCallback, 12, 0, 0x70, TELEPHONE_CORD_TASK);
    if (!g_telephoneCordTask)
        return;
    TelephoneCordTaskData *const data = static_cast<TelephoneCordTaskData *>(HLib::GetTaskParameter(g_telephoneCordTask));
    if (!data)
        return;
    std::memset(data, 0, sizeof(*data));
    data->receiver = receiver;
    data->base = base;
    data->allocation = LoadLegacySceneFile("misc", TELEPHONE_CORD_MODEL, TELEPHONE_CORD_TASK);
    if (!data->allocation)
        return;
    const std::span<const std::byte> bytes{static_cast<const std::byte *>(data->allocation), static_cast<std::size_t>(HLib::GetTaskDataSize(data->allocation))};
    if (!ReadModelFormat7(bytes, "misc/DENR003G.MT7", telephoneCordModel, 0))
        return;
    data->reader = &telephoneCordModel;
    const MT7Node *const root = telephoneCordModel.nodes.empty() ? nullptr : &telephoneCordModel.nodes.front();
    InitializeTelephoneCordGeometry(&data->geometry, root && root->xb01Offset ? root : nullptr, 0.02f, 30);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Port-owned model adaptation retires while the native task/use-space owners remain live.
void ReleaseTelephoneCordHostResources() {
    if (!telephoneCordModel.nodes.empty() || !telephoneCordModel.source.empty())
        ReleaseModelFormat7(telephoneCordModel);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CleanupTelephoneCord() {
    if (!g_telephoneCordTask)
        return;
    TelephoneCordTaskData *const data = static_cast<TelephoneCordTaskData *>(HLib::GetTaskParameter(g_telephoneCordTask));
    if (data && data->allocation)
        HLib::FreeTaskData(data->allocation);
    HLib::DestroyCurrentEventTask();
    g_telephoneCordTask = 0;
}

#else

// @verified - p
void CleanupTelephoneCord() {
    if (!g_telephoneCordTask)
        return;
    TelephoneCordTaskData *const data = static_cast<TelephoneCordTaskData *>(HLib::GetTaskParameter(g_telephoneCordTask));
    if (data && data->allocation) {
        ReleaseTelephoneCordHostResources();
        HLib::FreeTaskData(data->allocation);
    }
    HLib::DestroyCurrentTask();
    g_telephoneCordTask = nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void telCodeInit() {
    g_telephoneCordTask = 0;
}

#else

// @verified - p
void telCodeInit() {
    g_telephoneCordTask = nullptr;
}

#endif

} // namespace s2
} // namespace shenmue
