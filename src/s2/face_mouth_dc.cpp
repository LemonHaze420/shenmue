#include "s2/face_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

// @verified - d
std::int32_t BeginNeutralFacialGroup(FacialMorphVertex *vertices, std::int32_t count, float duration) {
    for (std::int32_t index = 0; index < count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target.x = vertex.neutral.x;
        vertex.target.y = vertex.neutral.y;
        vertex.target.z = vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    return 1;
}

// @verified - d
void PrepareFacialMouth(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale) {
    face.mouthFrame = 0;
    face.mouthDuration = duration;
    const float floatDuration = static_cast<float>(duration);
    const std::int32_t frames = static_cast<std::int32_t>(floatDuration);

    switch (static_cast<FacialMouthExpression>(expression)) {
    case FacialMouthExpression_Closed:
        PrepareMouthClosed(face, frames);
        break;
    case FacialMouthExpression_A:
        PrepareMouthA(face, frames, scale);
        break;
    case FacialMouthExpression_I:
        PrepareMouthI(face, frames, scale);
        break;
    case FacialMouthExpression_U:
        PrepareMouthU(face, frames, scale);
        break;
    case FacialMouthExpression_E:
        PrepareMouthE(face, frames, scale);
        break;
    case FacialMouthExpression_O:
        PrepareMouthO(face, frames, scale);
        break;
    case FacialMouthExpression_LeadIn:
        if (face.mouthExpression == static_cast<std::int32_t>(FacialMouthExpression_A) ||
            face.mouthExpression == static_cast<std::int32_t>(FacialMouthExpression_E)) {
            PrepareMouthLeadIn(face, frames, scale);
        } else {
            const FacialMorphGroup &group = face.groups[7];
            const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count, floatDuration);
            face.flags = (face.flags & ~facialMouthInnerDirty) | ((active & 1U) << 7U);
        }
        break;
    default:
        break;
    }
    face.mouthExpression = expression;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t IsFacialMouthIdle(const FacialModelData *face) {
    if (face->mouthDuration)
        return 0;
    return 1;
}
#endif

void PrepareBrowTilt(FacialModelData &face, float duration, float elevation, float contraction) {
    if (!face.groups[0].count)
        return;

    const float horizontal = contraction * 0.008f;
    const FacialMorphGroup &group = face.groups[0];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        const float shift = horizontal * vertex.weight0;
        vertex.target.y = vertex.neutral.y >= 0.0f ? vertex.neutral.y - shift : shift + vertex.neutral.y;
        vertex.target.z = (elevation * 0.008f) * vertex.weight0 + vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 1U;
}

void PrepareCurvedBrows(FacialModelData &face, float duration, float scale) {
    if (!face.groups[0].count)
        return;

    const FacialMorphGroup &group = face.groups[0];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target.x = vertex.neutral.x;
        vertex.target.y = (1.0f - scale * 0.05f) * vertex.neutral.y;
        const std::int32_t angle = TruncateToInt32((1.0f - vertex.weight0) * 36864.0f + 8192.0f);
        Vector2 values;
        CalculateFixedSinCos(static_cast<std::uint32_t>(angle), &values);
        const float curve = values.y;
        vertex.target.z = (curve + 0.3f) * (scale * 0.008f) + vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 1U;
}

void PrepareBrowElevation(FacialModelData &face, float duration, float elevation) {
    if (!face.groups[0].count)
        return;

    const FacialMorphGroup &group = face.groups[0];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        vertex.target.z = elevation + vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 1U;
}

void PrepareInvertedBrowTilt(FacialModelData &face, float duration, float elevation, float contraction) {
    if (!face.groups[0].count)
        return;

    const float horizontal = contraction * 0.008f;
    const FacialMorphGroup &group = face.groups[0];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        const float shift = horizontal * vertex.weight0;
        vertex.target.y = vertex.neutral.y >= 0.0f ? vertex.neutral.y - shift : shift + vertex.neutral.y;
        vertex.target.z = (0.6f - vertex.weight0) * (elevation * 0.008f) + vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 1U;
}

void PrepareSuppressedBrowTilt(FacialModelData &face, float duration, float elevation, float contraction) {
    if (!face.groups[0].count)
        return;

    const float horizontal = contraction * 0.008f;
    const FacialMorphGroup &group = face.groups[0];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        const float shift = horizontal * vertex.weight0;
        vertex.target.y = vertex.neutral.y >= 0.0f ? vertex.neutral.y - shift : shift + vertex.neutral.y;

        float weight = 0.6f - vertex.weight0;
        if (weight > 0.0f)
            weight *= 0.3f;
        vertex.target.z = weight * (elevation * 0.008f) + vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 1U;
}

void PrepareAsymmetricBrowTilt(FacialModelData &face, float duration, float scale, std::int32_t side) {
    if (!face.groups[0].count)
        return;

    const float displacement = scale * 0.008f;
    const FacialMorphGroup &group = face.groups[0];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        float weight = 0.6f - vertex.weight0;
        if ((side == 1 ? vertex.neutral.y < 0.0f : vertex.neutral.y > 0.0f) && weight > 0.0f)
            weight *= 0.3f;
        vertex.target.z = weight * displacement + vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 1U;
}

void PrepareOneSidedBrow(FacialModelData &face, float duration, float scale) {
    if (!face.groups[0].count)
        return;

    const float displacement = scale * 0.003f;
    const FacialMorphGroup &group = face.groups[0];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        const float shift = vertex.neutral.y <= 0.0f ? ((1.0f - vertex.weight0) + vertex.weight0) * displacement : displacement * vertex.weight0;
        vertex.target.z -= shift;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 1U;
}

void PrepareNegativeCheek(FacialModelData &face, float duration, float scale) {
    if (!face.groups[3].count)
        return;

    const FacialMorphGroup &group = face.groups[3];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        if (vertex.neutral.y < 0.0f) {
            vertex.target.y -= (scale * 0.005f) * vertex.weight0;
            vertex.target.z = (scale * 0.001f) * vertex.weight0 + vertex.neutral.z;
        }
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 8U;
}

void PreparePositiveCheek(FacialModelData &face, float duration, float scale) {
    if (!face.groups[3].count)
        return;

    const FacialMorphGroup &group = face.groups[3];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.target = vertex.neutral;
        if (vertex.neutral.y > 0.0f) {
            vertex.target.y = (scale * 0.005f) * vertex.weight0 + vertex.neutral.y;
            vertex.target.z = (scale * 0.001f) * vertex.weight0 + vertex.neutral.z;
        }
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 8U;
}

void PrepareCheekSmile(FacialModelData &face, float duration, float scale) {
    if (!face.groups[3].count)
        return;
    const float normalScale = (scale * 0.005f) * face.shapeCoefficients[7];
    const float elevation = (scale * 0.01f) * face.shapeCoefficients[6];
    const FacialMorphGroup &group = face.groups[3];
    FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
    for (std::int32_t index = 0; index < group.count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        const Vector3 normal = vertex.position[1];
        vertex.target.x = u32AsFloat(floatAsU32((normalScale * normal.x) * vertex.weight0 + vertex.neutral.x) | 1U);
        vertex.target.y = (normalScale * normal.y) * vertex.weight0 + vertex.neutral.y;
        vertex.target.z = ((normalScale * normal.z) + elevation) * vertex.weight0 + vertex.neutral.z;
        SnapshotFacialTarget(vertex, duration);
    }
    face.flags |= 8U;
}

static void SetOuterFacialEyelidWeights(FacialModelData &face, float duration, float first, float second) {
    if (face.eyelid0Mode) {
        face.eyelid0OuterWeight = first;
        face.eyelid1OuterWeight = second;
        if (!face.eyelidDuration)
            PrepareOuterEyelids(face, duration, first, second);
    }
}

static void SetInnerFacialEyelidWeights(FacialModelData &face, float duration, float first, float second) {
    if (face.eyelid1Mode) {
        face.eyelid0InnerWeight = first;
        face.eyelid1InnerWeight = second;
        if (!face.eyelidDuration)
            PrepareInnerEyelids(face, duration, first, second);
    }
}

void SetOuterEyelidWeights(FacialModelData &face, float duration, float first, float second) {
    if (first == 0.0f && second == 0.0f)
        face.eyelid0Mode = 1;
    else
        face.eyelid0Mode = 0;
    face.eyelid0OuterWeight = first;
    face.eyelid1OuterWeight = second;
    if (!face.eyelidDuration)
        PrepareOuterEyelids(face, duration, first, second);
}

void SetInnerEyelidWeights(FacialModelData &face, float duration, float first, float second) {
    if (first == 0.0f && second == 0.0f)
        face.eyelid1Mode = 1;
    else
        face.eyelid1Mode = 0;
    face.eyelid0InnerWeight = first;
    face.eyelid1InnerWeight = second;
    if (!face.eyelidDuration)
        PrepareInnerEyelids(face, duration, first, second);
}

static void StartFacialEyelidTransition(FacialModelData &face) {
    if (!face.eyelidDuration) {
        face.eyelidFrame = face.upperFaceFrame;
        face.eyelidDuration = face.upperFaceDuration;
    }
}

void PrepareUpperFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale) {
    face.upperExpression = expression;
    face.upperFaceFrame = 0;
    face.upperFaceDuration = duration;
    const float frames = static_cast<float>(duration);
    if (!expression) {
        const FacialMorphGroup &group = face.groups[0];
        const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count, frames);
        face.flags = (face.flags & ~1U) | (active & 1U);
        face.eyelid0OuterWeight = face.eyelid1OuterWeight = 0.0f;
        face.eyelid0InnerWeight = face.eyelid1InnerWeight = 0.0f;
        if (!face.eyelidDuration) {
            StartFacialEyelidTransition(face);
            PrepareOuterEyelids(face, frames, 0.0f, 0.0f);
            PrepareInnerEyelids(face, frames, 0.0f, 0.0f);
        }
        return;
    }
    if (static_cast<std::uint32_t>(expression) > 20U)
        return;
    const float rise = face.shapeCoefficients[0], fall = face.shapeCoefficients[1];
    switch (expression) {
    case 3:
    case 10:
    case 15:
    case 19:
        PrepareInvertedBrowTilt(face, frames, -scale * fall, 0.0f);
        break;
    case 4:
        PrepareBrowElevation(face, frames, (scale * 0.003f) * rise);
        break;
    case 5:
        PrepareBrowElevation(face, frames, (-scale * 0.003f) * fall);
        break;
    case 7:
        PrepareBrowElevation(face, frames, (-scale * 0.004f) * fall);
        break;
    case 8:
        PrepareBrowTilt(face, frames, -scale * fall, 0.0f);
        break;
    case 9:
        PrepareBrowElevation(face, frames, (scale * 0.004f) * rise);
        break;
    }
    const float *const weights = facialEyelidWeights[expression];
    SetOuterFacialEyelidWeights(face, frames, scale * weights[0], scale * weights[1]);
    SetInnerFacialEyelidWeights(face, frames, expression == 13 ? weights[2] : scale * weights[2], expression == 13 ? weights[3] : scale * weights[3]);
    StartFacialEyelidTransition(face);
    switch (expression) {
    case 1:
    case 11:
    case 14:
        PrepareBrowTilt(face, frames, -scale * fall, 0.2f);
        break;
    case 2:
        PrepareCurvedBrows(face, frames, scale * rise);
        break;
    case 6:
        PrepareOneSidedBrow(face, frames, scale);
        break;
    case 12:
        PrepareBrowTilt(face, frames, scale * rise, 0.0f);
        break;
    case 13:
    case 18:
        PrepareSuppressedBrowTilt(face, frames, scale * rise, 0.0f);
        break;
    case 16:
    case 17:
        PrepareAsymmetricBrowTilt(face, frames, scale * rise, expression == 17);
        break;
    case 20:
        PrepareCurvedBrows(face, frames, (scale * rise) * 0.5f);
        break;
    }
}

void PrepareLowerFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale) {
    face.expression = expression;
    face.lowerFaceFrame = 0;
    face.lowerFaceDuration = duration;
    face.expressionScale = scale;
    const float frames = static_cast<float>(duration);
    switch (expression) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
    case 7:
    case 11:
    case 12:
    case 20: {
        const FacialMorphGroup &group = face.groups[3];
        const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count, frames);
        face.flags = (face.flags & ~8U) | ((active & 1U) << 3U);
        break;
    }
    case 3:
    case 8:
    case 9:
    case 10:
    case 14:
    case 15:
    case 19:
        PrepareCheekSmile(face, frames, scale);
        break;
    case 6:
    case 16:
        PrepareNegativeCheek(face, frames, scale);
        break;
    case 13:
        PrepareCheekSmile(face, frames, scale * 0.5f);
        break;
    case 17:
        PreparePositiveCheek(face, frames, scale);
        break;
    }
    if (!face.mouthDuration)
        PrepareFacialMouth(face, face.mouthExpression, duration, scale);
}

} // namespace s2
} // namespace shenmue

#endif
