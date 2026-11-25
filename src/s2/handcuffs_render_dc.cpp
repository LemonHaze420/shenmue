#include "s2/handcuffs_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/math.hpp"
#include "s2/model_clone_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include <math.h>

namespace shenmue {
namespace s2 {

Vector3 g_handcuffChainAnchor;
Vector3 g_handcuffChainMidpoint;
float g_handcuffChainVelocity;
HandcuffChainArc g_handcuffChainArc;
std::uint32_t g_handcuffWristAngle;

std::int32_t GetHandcuffsWristBone(std::int32_t bone);

void Handcuffs_UpdateChainArc(float distance, HandcuffChainArc *arc) {
    if (distance < 0.2f) {
        const float radians = ::sqrtf(24.0f - distance * 120.0f);
        arc->angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(radians * 10430.378f));
        arc->radius = 0.2f / radians;
        arc->height = -(arc->radius * CalculateFixedCosine(static_cast<std::uint32_t>(arc->angle / 2)));
    } else {
        std::int32_t angle = 12743;
        const float sine = dc::sdk::SinFixed(angle / 2);
        const float radius = distance / (sine + sine);
        const float height = -radius * dc::sdk::CosFixed(angle / 2);
        arc->angle = angle;
        arc->radius = radius;
        arc->height = height;
    }
}

void Handcuffs_BuildChainTransform(const Vector3 &first, const Vector3 &second, Matrix4x4 &transform) {
    Vector3 midpoint;
    const volatile float firstWeightedX = first.x * 0.5f;
    const volatile float firstWeightedY = first.y * 0.5f;
    const volatile float firstWeightedZ = first.z * 0.5f;
    midpoint.x = second.x * 0.5f + firstWeightedX;
    midpoint.y = second.y * 0.5f + firstWeightedY;
    midpoint.z = second.z * 0.5f + firstWeightedZ;
    Vector3 along, sag;
    along.x = first.x - second.x;
    along.y = first.y - second.y;
    along.z = first.z - second.z;
    sag.x = midpoint.x - g_handcuffChainAnchor.x;
    sag.y = midpoint.y - g_handcuffChainAnchor.y;
    sag.z = midpoint.z - g_handcuffChainAnchor.z;
    Vector3 perpendicular;
    dc::sdk::njOuterProduct(&along, &sag, &perpendicular);
    dc::sdk::njOuterProduct(&perpendicular, &along, &sag);
    const float distance = CalculateDistanceBetweenPoints(&first, &second);
    Handcuffs_UpdateChainArc(distance, &g_handcuffChainArc);
    dc::sdk::njUnitVector(&along);
    dc::sdk::njUnitVector(&sag);
    dc::sdk::njUnitVector(&perpendicular);
    Vector3 force;
    force.x = midpoint.x - g_handcuffChainAnchor.x;
    force.y = midpoint.y - g_handcuffChainAnchor.y;
    force.z = midpoint.z - g_handcuffChainAnchor.z;
    force.x += 0.0f;
    force.y += 100.0f;
    force.z += 0.0f;
    g_handcuffChainVelocity += dc::sdk::njInnerProduct(&perpendicular, &force);
    force.x = perpendicular.x * g_handcuffChainVelocity;
    force.y = perpendicular.y * g_handcuffChainVelocity;
    force.z = perpendicular.z * g_handcuffChainVelocity;
    g_handcuffChainAnchor.x += force.x;
    g_handcuffChainAnchor.y += force.y;
    g_handcuffChainAnchor.z += force.z;
    g_handcuffChainMidpoint = midpoint;
    transform.rows[0].x = along.x;
    transform.rows[1].x = sag.x;
    transform.rows[2].x = perpendicular.x;
    transform.rows[0].y = along.y;
    transform.rows[1].y = sag.y;
    transform.rows[2].y = perpendicular.y;
    transform.rows[0].z = along.z;
    transform.rows[1].z = sag.z;
    transform.rows[2].z = perpendicular.z;
    transform.rows[0].w = 0.0f;
    transform.rows[1].w = 0.0f;
    transform.rows[2].w = 0.0f;
    transform.rows[3].w = 1.0f;
    transform.rows[3].x = g_handcuffChainMidpoint.x;
    transform.rows[3].y = g_handcuffChainMidpoint.y;
    transform.rows[3].z = g_handcuffChainMidpoint.z;
}

void Handcuffs_Render(void *parameter) {
    HandcuffsTaskData *const data = static_cast<HandcuffsTaskData *>(parameter);
    Matrix4x4 firstTransform, secondTransform;
    Vector3 firstEnd, secondEnd, firstWrist, secondWrist, midpoint, viewMidpoint, direction;
    Vector4 colour;
    Vector3 firstLocal, secondLocal;
    Matrix4x4 chainTransform, linkTransform;
    MT7Node *model, *chain;
    MotionWork *firstMotion, *secondMotion;
    const Matrix4x4 *firstBone, *secondBone;
    std::int32_t previousLight;
    HandcuffConstraint *const first = &data->first;
    HandcuffConstraint *const second = &data->second;
    firstMotion = charFindMotionwork(first->character);
    secondMotion = charFindMotionwork(second->character);
    firstBone = mtwkGetBoneTransform(firstMotion, GetHandcuffsWristBone(first->bone));
    secondBone = mtwkGetBoneTransform(secondMotion, GetHandcuffsWristBone(second->bone));
    mtwkGetBonePosition(firstMotion, static_cast<MotionBoneId>(first->bone), &firstWrist);
    mtwkGetBonePosition(secondMotion, static_cast<MotionBoneId>(second->bone), &secondWrist);
    midpoint.x = (firstWrist.x + secondWrist.x) * 0.5f;
    midpoint.y = (firstWrist.y + secondWrist.y) * 0.5f;
    midpoint.z = (firstWrist.z + secondWrist.z) * 0.5f;
    Matrix4x4::TransformPoint(midpoint, viewMidpoint);
    if (viewMidpoint.z < 0.5f && viewMidpoint.z > -8.0f) {
        model = data->highModel;
        chain = data->highChainLink;
    } else {
        model = data->lowModel;
        chain = data->lowChainLink;
    }
    std::uint32_t light = 15;
    previousLight = GetCurrentActiveLight();
    camCtrlGetNextCamPos(&direction);
    SetCurrentActiveLight(3);
    ResetSelectedLightEntry(light);
    SetLightKind(light, 2);
    EnableSelectedLight(light);
    direction.x = midpoint.x - direction.x;
    direction.y = midpoint.y - direction.y;
    direction.z = midpoint.z - direction.z;
    am2Vector3NormalizeFast(&direction);
    SetSelectedLightDirection(light, direction);
    colour.x = 1.0f;
    colour.y = colour.z = colour.w = 0.4f;
    shenSetLightColour(light, colour);
    PublishGlobalLighting();

    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(firstBone);
    matrix_stack::GetMatrix(&firstTransform);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::TransformPoint(secondWrist, firstLocal);
    g_handcuffWristAngle = dc::sdk::ArcTan2Fixed(firstLocal.y, firstLocal.z);
    matrix_stack::LoadMatrix(&firstTransform);
    Matrix4x4::RotateAroundY_Wrapper(0x4000);
    Matrix4x4::RotateAroundZ_Wrapper(0x4000 - g_handcuffWristAngle);
    Matrix4x4::TransformPoint(data->attachmentOffset, firstEnd);
    matrix_stack::GetMatrix(&firstTransform);
    PopCurrentMatrix(1);
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::MultiplyWithPrePassMatrix(&firstTransform);
    DrawModelNodeTreePushed(model);
    PopCurrentMatrix(1);

    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(secondBone);
    matrix_stack::GetMatrix(&secondTransform);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::TransformPoint(firstWrist, secondLocal);
    g_handcuffWristAngle = dc::sdk::ArcTan2Fixed(secondLocal.y, secondLocal.z);
    matrix_stack::LoadMatrix(&secondTransform);
    Matrix4x4::RotateAroundY_Wrapper(0x4000);
    Matrix4x4::RotateAroundZ_Wrapper(0x4000 - g_handcuffWristAngle);
    Matrix4x4::TransformPoint(data->attachmentOffset, secondEnd);
    matrix_stack::GetMatrix(&secondTransform);
    PopCurrentMatrix(1);
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::MultiplyWithPrePassMatrix(&secondTransform);
    DrawModelNodeTreePushed(model);
    PopCurrentMatrix(1);

    Handcuffs_BuildChainTransform(firstEnd, secondEnd, chainTransform);
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&chainTransform);
    Matrix4x4::preTranslate(0.0f, g_handcuffChainArc.height, 0.0f);
    matrix_stack::GetMatrix(&chainTransform);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(0.0f, g_handcuffChainArc.radius, 0.0f);
    Matrix4x4::RotateAroundZ_Wrapper(0x4000);
    matrix_stack::GetMatrix(&linkTransform);
    PopCurrentMatrix(1);
    for (std::int32_t index = 0; index <= data->chainCount; index++) {
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::MultiplyWithPrePassMatrix(&chainTransform);
        Matrix4x4::RotateAroundZ_Wrapper(-g_handcuffChainArc.angle / 2 + g_handcuffChainArc.angle * index / data->chainCount);
        Matrix4x4::MultiplyWithPrePassMatrix(&linkTransform);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(static_cast<float>((index + 1) * 0xb4000) / 45.0f));
        DrawModelNodeTreePushed(chain);
        PopCurrentMatrix(1);
    }
    std::uint32_t restoredLight = 15;
    SetCurrentActiveLight(previousLight);
    ResetSelectedLightEntry(restoredLight);
    DisableSelectedLight(restoredLight);
    PublishGlobalLighting();
}

} // namespace s2
} // namespace shenmue
#endif
