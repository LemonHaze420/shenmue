#include "s2/telephone_cord.hpp"

#include "s2/event_runtime.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/pak.hpp"

#include <cmath>
#include <cstring>

namespace shenmue::s2 {

HLib::Task *g_telephoneCordTask = nullptr;

namespace {
MT7Model telephoneCordModel;

Vector3 AttachmentPosition(Character *character, const Vector3 &attachment) {
    const Vector3 position = GetCharacterWorldPosition(character);
    const Vector3i rotation = GetCharacterWorldRotation(character);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::preTranslate(attachment.x, attachment.y, attachment.z);
    return {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
}
} // namespace

// @verified - p
void InitializeTelephoneCordGeometry(TelephoneCordGeometry *geometry, const MT7Node *mesh, float length, std::int32_t count) {
    std::memset(geometry, 0, sizeof(*geometry));
    geometry->segmentLength = length;
    geometry->mesh = mesh;
    geometry->segmentCount = count;
    geometry->force.y = 100.0f;
}

// @verified - p
void SetTelephoneCordAttachments(TelephoneCordGeometry *geometry, const Vector3 *receiver, const Vector3 *base) {
    geometry->receiverAttachment = *receiver;
    geometry->baseAttachment = *base;
}

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

// @verified - p
void TelephoneCordDrawCallback(void *taskData) {
    DrawTelephoneCordGeometry(&static_cast<TelephoneCordTaskData *>(taskData)->geometry);
}

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


void ReleaseTelephoneCordHostResources() {
    if (!telephoneCordModel.nodes.empty() || !telephoneCordModel.source.empty())
        ReleaseModelFormat7(telephoneCordModel);
}

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

// @verified - p
void telCodeInit() {
    g_telephoneCordTask = nullptr;
}

} // namespace shenmue::s2
