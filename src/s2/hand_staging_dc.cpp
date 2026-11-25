#include "s2/hand_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include <cstring>
#endif
#include "s2/motion_work_dc.hpp"
#include <float.h>
#include <cmath>
#define SHENMUE_HAND_NULL 0
#define SHENMUE_HAND_INITIALIZE =
#define SHENMUE_HAND_MAX_FLOAT FLT_MAX
#define SHENMUE_HAND_FMA(a, b, c) ((a) * (b) + (c))
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include <cstdio>
#include "s2/model_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/player_model_dc.hpp"
#endif
#include "shenmue.h"
#include "s2/character_transparency_dc.hpp"
#include "s2/math.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/hand_draw_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/model_render_dc.hpp"

namespace shenmue {
namespace s2 {

std::int32_t g_handPoseMode = 0;
std::int32_t g_handPosePositiveXY;
Vector3 *g_handPoseJointPositions = 0;
const std::int16_t *g_handPoseWristAngles = 0;
Matrix4x4 *g_handPoseMatrixOutput = 0;
#define SHENMUE_HAND_ANGLE HandSignedAngle
#define SHENMUE_HAND_ANGLE HandSignedAngle
#define SHENMUE_HAND_ANGLE HandSignedAngle
#define SHENMUE_HAND_ANGLE HandSignedAngle
#define SHENMUE_HAND_ANGLE HandSignedAngle
#define SHENMUE_HAND_ANGLE HandSignedAngle
#define SHENMUE_HAND_ANGLE HandSignedAngle

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BuildHandJointTransform(float positionX, float positionY, float positionZ, std::int32_t x, std::int32_t y, std::int32_t z) {
    Vector2 yValues, xValues, zValues;
    CalculateFixedSinCos(static_cast<std::uint16_t>(y), &yValues);
    CalculateFixedSinCos(static_cast<std::uint16_t>(x), &xValues);
    CalculateFixedSinCos(static_cast<std::uint16_t>(z), &zValues);
    const float sx = xValues.x, cx = xValues.y, sy = yValues.x, cy = yValues.y, sz = zValues.x, cz = zValues.y;
    const float sxsy = sx * sy, cxsy = cx * sy;
    const float negativeY = -sy, negativeZ = 0.0f - sz;
    Vector4 first, second, third, fourth;
    first.x = sxsy;
    first.y = cx;
    first.z = 0.0f;
    first.w = 0.0f;
    second.x = sz;
    second.y = cz;
    second.z = positionZ;
    second.w = 0.0f;
    const float firstDot = matrix_stack::InnerProduct4(first, second);
    third.x = cz;
    third.y = negativeZ;
    third.z = negativeY;
    third.w = cy;
    const float secondDot = matrix_stack::InnerProduct4(third, first);
    fourth.x = sx;
    fourth.y = cxsy;
    fourth.z = 0.0f;
    fourth.w = 0.0f;
    const float thirdDot = matrix_stack::InnerProduct4(fourth, third);
    second.z = 0.0f;
    second.w = firstDot;
    const float fourthDot = matrix_stack::InnerProduct4(second, fourth);
    const float rows[16] = {cz * cy, (-negativeZ) * cy, negativeY, 0.0f, 0.0f + secondDot, firstDot, cy * sx, 0.0f, fourthDot, -thirdDot, cy * cx, 0.0f,
        positionX, positionY, positionZ, 1.0f};
    Matrix4x4 transform;
    std::memcpy(&transform, rows, sizeof(transform));
    Matrix4x4::Multiply(&transform);
}
#endif

// @verified - d
void PublishHandJointMatrix(std::int32_t joint) {
    matrix_stack::GetTranslation(g_handPoseJointPositions + joint);
    const float *source = g_handPoseJointCursor->inverseBind.rows[0].data();
    float *destination = g_handPoseMatrixOutput[joint].rows[0].data();
    for (std::int32_t row = 0; row < 4; ++row) {
        asm volatile("fmov @%0+,fr0\n\tfmov @%0+,fr1\n\tfmov @%0+,fr2\n\tfmov @%0+,fr3\n\tftrv xmtrx,fv0\n\t"
                     "fmov fr0,@%1\n\tadd #4,%1\n\tfmov fr1,@%1\n\tadd #4,%1\n\tfmov fr2,@%1\n\tadd #4,%1\n\tfmov fr3,@%1\n\tadd #4,%1"
            : "+r"(source), "+r"(destination)
            :
            : "fr0", "fr1", "fr2", "fr3", "memory");
    }
}

std::int32_t InterpolateHandPoseChannel(std::int32_t count, const HandCubicKeyframe *keys, std::int32_t frame) {
    std::int32_t upper = count;
    std::int32_t lower = 0;
    do {
        const std::int32_t middle = (lower + upper) / 2;
        if (keys[middle].frame >= frame)
            upper = middle;
        else
            lower = middle;
    } while (upper > lower + 1);
    float angle;
    if (!upper)
        angle = keys[0].degrees;
    else if (upper == count)
        angle = (keys + upper)[-1].degrees;
    else if ((keys + upper)[-1].frame == frame)
        angle = (keys + upper)[-1].degrees;
    else {
        const HandCubicKeyframe &last = keys[upper];
        const HandCubicKeyframe &first = (&last)[-1];
        const float duration = static_cast<float>(last.frame - first.frame);
        const float t = static_cast<float>(frame - first.frame) / duration;
        const float square = t * t;
        const float cube = square * t;
        const float twiceCube = cube + cube;
        const float threeSquare = square * 3.0f;
        const float firstBasis = (twiceCube - threeSquare) + 1.0f;
        const float firstDegrees = first.degrees;
        const float lastBasis = SHENMUE_HAND_FMA(cube, -2.0f, threeSquare);
        angle = SHENMUE_HAND_FMA(firstBasis, firstDegrees, lastBasis * last.degrees);
        const float firstSlope = (cube - (square + square)) + t;
        const float tangent = SHENMUE_HAND_FMA(firstSlope, first.outgoing, (cube - square) * last.incoming);
        angle = SHENMUE_HAND_FMA(tangent, duration, angle);
    }
    const float fixed = (65536.0f * angle) / 360.0f;
    std::int32_t result;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    result = static_cast<std::int32_t>(fixed);
#else
    asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(result) : "f"(fixed) : "fpul");
#endif
    return result;
}

void StageHandJoint(std::int32_t publishMatrices) {
    const std::int32_t id = g_handPoseJointCursor->id;
    std::int32_t children = g_handPoseJointCursor->childCount;
    std::int32_t rotation[3], base[3];
    rotation[0] = base[0] = g_handPoseJointCursor->rotation.x;
    rotation[1] = base[1] = g_handPoseJointCursor->rotation.y;
    rotation[2] = base[2] = g_handPoseJointCursor->rotation.z;
    if (id != 71) {
        if (g_handPoseMode) {
            if (g_handPosePositiveXY) {
                rotation[0] += g_handPoseOutputAngles[id][0];
                rotation[1] += g_handPoseOutputAngles[id][1];
                rotation[2] += g_handPoseOutputAngles[id][2];
            } else {
                rotation[0] -= g_handPoseOutputAngles[id][0];
                rotation[1] -= g_handPoseOutputAngles[id][1];
                rotation[2] += g_handPoseOutputAngles[id][2];
            }
        } else {
            if (g_handPoseChannelCursor) {
                if (!g_handPosePositiveXY) {
                    rotation[0] = -rotation[0];
                    rotation[1] = -rotation[1];
                }
                if (g_handPoseChannelCursor[0].keyCount > 0)
                    rotation[0] = InterpolateHandPoseChannel(g_handPoseChannelCursor[0].keyCount, g_handPoseChannelCursor[0].keys, g_handPoseEvaluationFrame);
                if (g_handPoseChannelCursor[1].keyCount > 0)
                    rotation[1] = InterpolateHandPoseChannel(g_handPoseChannelCursor[1].keyCount, g_handPoseChannelCursor[1].keys, g_handPoseEvaluationFrame);
                if (g_handPoseChannelCursor[2].keyCount > 0)
                    rotation[2] = InterpolateHandPoseChannel(g_handPoseChannelCursor[2].keyCount, g_handPoseChannelCursor[2].keys, g_handPoseEvaluationFrame);
                g_handPoseChannelCursor += 3;
                if (!g_handPosePositiveXY) {
                    rotation[0] = -rotation[0];
                    rotation[1] = -rotation[1];
                }
            }
            if (g_handPosePositiveXY) {
                std::int16_t (*const angles)[3] = g_handPoseOutputAngles;
                angles[id][0] = rotation[0] - base[0];
                angles[id][1] = rotation[1] - base[1];
                angles[id][2] = rotation[2] - base[2];
            } else {
                std::int16_t (*const angles)[3] = g_handPoseOutputAngles;
                angles[id][0] = base[0] - rotation[0];
                angles[id][1] = base[1] - rotation[1];
                angles[id][2] = rotation[2] - base[2];
            }
        }
        BuildHandJointTransform(
            g_handPoseJointCursor->position.x, g_handPoseJointCursor->position.y, g_handPoseJointCursor->position.z, rotation[0], rotation[1], rotation[2]);
    } else {
        BuildHandJointTransform(
            g_handPoseJointCursor->position.x, g_handPoseJointCursor->position.y, g_handPoseJointCursor->position.z, rotation[0], rotation[1], rotation[2]);
        Matrix4x4::RotateAroundX_Wrapper(-g_handPoseWristAngles[0]);
        Matrix4x4::RotateAroundY_Wrapper(-g_handPoseWristAngles[1]);
        Matrix4x4::RotateAroundZ_Wrapper(-g_handPoseWristAngles[2]);
    }
    if (publishMatrices)
        PublishHandJointMatrix(id);
    else
        matrix_stack::GetTranslation(g_handPoseJointPositions + id);
    ++g_handPoseJointCursor;
    while (children-- > 0) {
        matrix_stack::Matrix4x4_Push(0);
        StageHandJoint(publishMatrices);
        PopCurrentMatrix(1);
    }
}

// @verified - d
void BuildHandPoseMatrices(std::int32_t frame, HandPoseState *state, Matrix4x4 *matrices, std::int32_t publishMatrices) {
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    g_handPoseEvaluationFrame = frame;
    g_handPoseJointCursor = reinterpret_cast<const HandPoseModelJoint *>(state->modelData + 8);
    g_handPoseChannelCursor = state->channels;
    g_handPoseMatrixOutput = matrices;
    g_handPoseMode = static_cast<std::int8_t>(state->poseMode);
    g_handPoseOutputAngles = state->currentJointAngles;
    g_handPoseJointPositions = state->jointPositions;
    g_handPoseWristAngles = state->wristAngles;
    g_handPosePositiveXY = state->selector == 2 || state->selector == 4;
    StageHandJoint(publishMatrices);
    PopCurrentMatrix(1);
}

// @verified - d
void StageHandPose(std::int32_t frame, HandPoseState *state, Matrix4x4 *matrices) {
    BuildHandPoseMatrices(frame, state, matrices, 1);
    HMT_HandCalcSkin(state, matrices);
}

void BuildHandPoseMatricesNoPublish(std::int32_t frame, HandPoseState *state, Matrix4x4 *matrices) {
    BuildHandPoseMatrices(frame, state, matrices, 0);
}

} // namespace s2
} // namespace shenmue

#endif
