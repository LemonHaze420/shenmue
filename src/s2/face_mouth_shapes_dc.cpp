#include "s2/face_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cmath>

namespace shenmue { namespace s2 {

FacialShapeModifier mouthModifiers[3]= {};

// @verified - d
void ModifyUpperAsymmetricLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float inverse = 1.0f - v.weight0;
    const float amount = (data.shapeCoefficients[8] * 0.001f) * data.expressionScale;
    v.target.x = ((inverse) * ( amount) + ( v.target.x));
    v.target.z = ((amount) * ( v.weight0) + ( v.target.z));
    if (v.neutral.y < 0.0f) {
        float distance = inverse + -0.5f;
        if (distance < 0.0f)
            distance = -distance;
        v.target.z = (((data.shapeCoefficients[8] * 0.002f) * data.expressionScale) * ( 0.5f - distance) + ( v.target.z));
    }
}

// @verified - d
void ModifyLowerAsymmetricLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float amount = (data.shapeCoefficients[9] * 0.001f) * data.expressionScale;
    const float fall = (data.shapeCoefficients[9] * 0.004f) * data.expressionScale;
    v.target.x = ((1.0f - v.weight1) * ( amount) + ( v.target.x));
    const float delta = v.neutral.y > 0.0f ? (v.weight1 * amount) + (v.weight1 * amount) : (v.weight1 + (1.0f - v.weight1)) * fall;
    v.target.z -= delta;
}

// @verified - d
void ModifyUpperSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[8] * data.expressionScale;
    const float weight = (1.0f - v.weight0) * scale;
    const float spread = weight * 0.001f;
    v.target.x = weight * 0.001f + v.target.x;
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((weight) * ( 0.004f) + ( scale * 0.001f)) + v.target.z;
}

// @verified - d
void ModifyLowerSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[12];
    const float scale = data.shapeCoefficients[9] * data.expressionScale;
    const float first = ((1.0f - v.weight0) * scale) * coefficient;
    const float second = (1.0f - v.weight1) * scale;
    const float spread = second * 0.001f;
    const float advance = second * 0.001f - ((v.weight0 * scale) * coefficient) * 0.002f;
    v.target.x = advance + v.target.x;
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((scale) * ( 0.001f) + ( ((second) * ( 0.004f) + ( first * 0.0025f)))) + v.target.z;
}


// @verified - d
void ModifyUpperWideSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[8] * data.expressionScale;
    const float weight = (1.0f - v.weight0) * scale;
    const float spread = weight * 0.006f;
    v.target.x = ((weight) * ( 0.001f) + ( v.target.x));
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((weight) * ( 0.005f) + ( scale * 0.001f)) + v.target.z;
}

// @verified - d
void ModifyLowerWideSmile(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[12];
    const float scale = data.shapeCoefficients[9] * data.expressionScale;
    const float first = ((1.0f - v.weight0) * scale) * coefficient;
    const float second = (1.0f - v.weight1) * scale;
    const float retreat = ((v.weight0 * scale) * coefficient) * 0.002f;
    const float spread = second * 0.006f;
    v.target.x = (second * 0.001f - retreat) + v.target.x;
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((scale) * ( 0.001f) + ( ((second) * ( 0.005f) + ( first * 0.0025f)))) + v.target.z;
}

// @verified - d
void ModifyUpperRaisedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[8];
    const float scale = data.expressionScale;
    const float inverse = ((1.0f - v.weight0) * coefficient) * scale;
    const float tip = v.weight0 < 0.1f ? (0.1f - v.weight0) * 0.0132f : 0.0f;
    const float spread = inverse * 0.002f;
    const float lift = ((coefficient * v.weight0) * scale) * 0.002f;
    v.target.x = ((inverse) * ( 0.001f) + ( v.target.x));
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = (((inverse) * ( 0.002f) + ( tip)) + lift) + v.target.z;
}

// @verified - d
void ModifyLowerRaisedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float inverse = ((1.0f - v.weight1) * coefficient) * data.expressionScale;
    const float spread = inverse * 0.002f;
    const float lift = ((coefficient * v.weight1) * data.expressionScale) * 0.002f;
    v.target.x = ((inverse) * ( 0.001f) + ( v.target.x));
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((inverse) * ( 0.002f) + ( lift)) + v.target.z;
}

// @verified - d
void ModifyUpperPinchedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[8] * data.expressionScale;
    const float weighted = scale * v.weight0;
    v.target.x = ((weighted) * ( 0.0005f) + ( v.target.x));
    v.target.z = (weighted * 0.002f - scale * 0.001f) + v.target.z;
}

// @verified - d
void ModifyLowerPinchedLip(const FacialModelData &data, FacialMorphVertex &v) {
    const float scale = data.shapeCoefficients[9] * data.expressionScale;
    const float weighted = scale * v.weight1;
    v.target.x = ((weighted) * ( 0.0005f) + ( v.target.x));
    v.target.z = (weighted * 0.002f - scale * 0.001f) + v.target.z;
}

// @verified - d
void ModifyLowerLipDrop(const FacialModelData &data, FacialMorphVertex &v) {
    v.target.z -= ((data.shapeCoefficients[9] * v.weight1) * data.expressionScale) * 0.004f;
}

// @verified - d
void ModifyUpperLipCurl(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[8];
    v.target.z = ((((1.0f - v.weight0) * coefficient) * data.expressionScale) * ( -0.004f) + (
        ((coefficient * v.weight1) * data.expressionScale) * 0.004f)) + v.target.z;
}

// @verified - d
void ModifyLowerLipCurl(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float inverse = ((1.0f - v.weight1) * coefficient) * data.expressionScale;
    const float weighted = (coefficient * v.weight1) * data.expressionScale;
    const float spread = inverse * 0.004f;
    v.target.y = v.neutral.y > 0.0f ? ((inverse) * ( 0.004f) + ( v.target.y)) : v.target.y - spread;
    v.target.z -= ((inverse) * ( 0.006f) + ( weighted * 0.006f));
}

// @verified - d
void ModifyInnerLipSpread(const FacialModelData &data, FacialMorphVertex &v) {
    const float amount = ((1.0f - v.weight1) * data.shapeCoefficients[9]) * data.expressionScale;
    const float spread = amount * 0.001f;
    v.target.y = v.neutral.y > 0.0f ? ((amount) * ( 0.001f) + ( v.target.y)) : v.target.y - spread;
}

// @verified - d
void ModifyUpperLipArch(const FacialModelData &data, FacialMorphVertex &v) {
    const float weight = v.weight0 > 0.25f ? 1.0f - v.weight0 : (v.weight0 + 0.05f) * 2.5f;
    v.target.z = (((weight * data.shapeCoefficients[8]) * data.expressionScale) * ( 0.003f) + ( v.target.z));
}

// @verified - d
void ModifyLowerLipArch(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float scale = data.expressionScale;
    const float pivot = data.shapeCoefficients[12];
    const float lift = (((1.0f - v.weight0) * coefficient) * scale) * pivot;
    v.target.x -= (((v.weight0 * coefficient) * scale) * pivot) * 0.002f;
    const float spreadAmount = ((1.0f - v.weight1) * coefficient) * scale;
    const float spread = spreadAmount * 0.004f;
    v.target.y = v.neutral.y > 0.0f ? ((spreadAmount) * ( 0.004f) + ( v.target.y)) : v.target.y - spread;
    v.target.z -= (coefficient * 0.005f) * scale - lift * 0.0025f;
}

// @verified - d
void ModifyUpperLipRetract(const FacialModelData &data, FacialMorphVertex &v) {
    const float amount = ((1.0f - v.weight0) * data.shapeCoefficients[8]) * data.expressionScale;
    const float spread = amount * 0.0005f;
    v.target.x -= amount * 0.0005f;
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((amount) * ( 0.002f) + ( v.target.z));
}

// @verified - d
void ModifyLowerLipRetract(const FacialModelData &data, FacialMorphVertex &v) {
    const float amount = ((1.0f - v.weight1) * data.shapeCoefficients[9]) * data.expressionScale;
    const float spread = amount * 0.0005f;
    v.target.x -= amount * 0.0005f;
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((amount) * ( 0.002f) + ( v.target.z));
}

// @verified - d
void ModifyUpperLipTension(const FacialModelData &data, FacialMorphVertex &v) {
    float weight = 0.8f - v.weight0;
    if (weight < 0.0f)
        weight = -weight;
    const float amount = weight * (data.shapeCoefficients[8] * data.expressionScale);
    const float spread = amount * 0.0042f;
    v.target.x -= amount * 0.002f;
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((amount) * ( 0.004f) + ( v.target.z));
}

// @verified - d
void ModifyLowerLipTension(const FacialModelData &data, FacialMorphVertex &v) {
    float weight = 0.8f - v.weight1;
    if (weight < 0.0f)
        weight = -weight;
    const float coefficient = data.shapeCoefficients[9];
    const float scale = data.expressionScale;
    const float pivot = data.shapeCoefficients[12];
    const float amount = weight * (scale * coefficient);
    const float retreat = (((v.weight0 * coefficient) * scale) * pivot) * 0.001f;
    const float lift = (((1.0f - v.weight0) * coefficient) * scale) * pivot;
    const float spread = amount * 0.0042f;
    v.target.x -= ((amount) * ( 0.002f) + ( retreat));
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((amount) * ( 0.004f) + ( lift * 0.0015f)) + v.target.z;
}

// @verified - d
void ModifyUpperLipCorner(const FacialModelData &data, FacialMorphVertex &v) {
    const float inverse = 1.0f - v.weight0;
    const float amount = (inverse * data.shapeCoefficients[8]) * data.expressionScale;
    float spread = amount * 0.001f;
    v.target.x = amount * 0.001f + v.target.x;
    float liftScale = 0.002f;
    if (inverse > 0.9f && inverse < 1.0f) {
        liftScale = 0.004f;
        spread *= 0.5f;
    }
    v.target.z = ((amount) * ( liftScale) + ( v.target.z));
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
}

// @verified - d
void ModifyLowerLipCorner(const FacialModelData &data, FacialMorphVertex &v) {
    const float inverse = 1.0f - v.weight1;
    const float amount = (inverse * data.shapeCoefficients[9]) * data.expressionScale;
    float spread = amount * 0.001f;
    v.target.x = amount * 0.001f + v.target.x;
    float liftScale = 0.002f;
    if (inverse > 0.9f && inverse < 1.0f) {
        liftScale = 0.004f;
        spread *= 0.5f;
    }
    v.target.z = ((amount) * ( liftScale) + ( v.target.z));
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
}

// @verified - d
void ModifyUpperLipTip(const FacialModelData &data, FacialMorphVertex &v) {
    const float relative = v.weight0 - 0.9f;
    const float weight = relative > 0.0f ? (0.1f - relative) * 9.0f : relative + 0.9f;
    const float amount = (data.shapeCoefficients[8] * data.expressionScale) * weight;
    const float shift = amount * 0.001f;
    v.target.x = shift + v.target.x;
    v.target.y = v.neutral.y > 0.0f ? shift + v.target.y : v.target.y - shift;
    v.target.z = ((amount) * ( 0.001f) + ( v.target.z));
}

// @verified - d
void ModifyLowerLipTip(const FacialModelData &data, FacialMorphVertex &v) {
    const float relative = v.weight1 - 0.9f;
    const float weight = relative > 0.0f ? (0.1f - relative) * 9.0f : relative + 0.9f;
    const float scale = data.expressionScale;
    const float coefficient = data.shapeCoefficients[9];
    const float pivot = data.shapeCoefficients[12];
    const float amount = weight * (scale * coefficient);
    const float spread = amount * 0.001f;
    v.target.x = (amount * 0.001f - (((v.weight0 * coefficient) * scale) * pivot) * 0.005f) + v.target.x;
    v.target.y = v.neutral.y > 0.0f ? spread + v.target.y : v.target.y - spread;
    v.target.z = ((amount) * ( 0.002f) + ( ((((1.0f - v.weight0) * coefficient) * scale) * pivot) * 0.0025f)) + v.target.z;
}

// @verified - d
void ModifyUpperNegativeCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y < 0.0f)
        ModifyUpperLipCorner(data, v);
}
// @verified - d
void ModifyLowerNegativeCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y < 0.0f)
        ModifyLowerLipCorner(data, v);
}
// @verified - d
void ModifyUpperPositiveCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y > 0.0f)
        ModifyUpperLipCorner(data, v);
}
// @verified - d
void ModifyLowerPositiveCorner(const FacialModelData &data, FacialMorphVertex &v) {
    if (v.neutral.y > 0.0f)
        ModifyLowerLipCorner(data, v);
}

// @verified - d
void ModifyLowerLipArchWeighted(const FacialModelData &data, FacialMorphVertex &v) {
    const float coefficient = data.shapeCoefficients[9];
    const float scale = data.expressionScale;
    const float pivot = data.shapeCoefficients[12];
    const float lift = (((1.0f - v.weight0) * coefficient) * scale) * pivot;
    v.target.x -= (((v.weight0 * coefficient) * scale) * pivot) * 0.002f;
    const float amount = ((1.0f - v.weight1) * coefficient) * scale;
    const float spread = amount * 0.004f;
    v.target.y = v.neutral.y > 0.0f ? ((amount) * ( 0.004f) + ( v.target.y)) : v.target.y - spread;
    v.target.z -= amount * 0.005f - lift * 0.0025f;
}

// @verified - d
void ModifyUpperLipArchWeighted(const FacialModelData &data, FacialMorphVertex &v) {
    const float weight = v.weight0 > 0.25f ? 1.0f - v.weight0 : (v.weight0 + 0.05f) * 2.5f;
    v.target.z = (((weight * data.shapeCoefficients[8]) * data.expressionScale) * ( 0.003f) + ( v.target.z));
}

// @verified - d
void SelectMouthModifiers(const FacialModelData &data) {
    mouthModifiers[0] = 0;
    mouthModifiers[1] = 0;
    mouthModifiers[2] = 0;
    switch (data.expression) {
    case 1:
        mouthModifiers[0] = ModifyUpperLipTip;
        mouthModifiers[1] = ModifyLowerLipTip;
        break;
    case 2:
    case 5:
        mouthModifiers[0] = ModifyUpperPinchedLip;
        mouthModifiers[1] = ModifyLowerPinchedLip;
        break;
    case 3:
        mouthModifiers[0] = ModifyUpperSmile;
        mouthModifiers[1] = ModifyLowerSmile;
        break;
    case 6:
        mouthModifiers[0] = ModifyUpperAsymmetricLip;
        mouthModifiers[1] = ModifyLowerAsymmetricLip;
        break;
    case 9:
        mouthModifiers[1] = ModifyLowerLipDrop;
        break;
    case 10:
        mouthModifiers[0] = ModifyUpperLipCurl;
        mouthModifiers[1] = ModifyLowerLipCurl;
        mouthModifiers[2] = ModifyInnerLipSpread;
        break;
    case 11:
    case 14:
        mouthModifiers[0] = ModifyUpperLipArch;
        mouthModifiers[1] = ModifyLowerLipArch;
        break;
    case 12:
        mouthModifiers[0] = ModifyUpperLipRetract;
        mouthModifiers[1] = ModifyLowerLipRetract;
        break;
    case 13:
        mouthModifiers[0] = ModifyUpperLipTension;
        mouthModifiers[1] = ModifyLowerLipTension;
        break;
    case 15:
        mouthModifiers[0] = ModifyUpperRaisedLip;
        mouthModifiers[1] = ModifyLowerRaisedLip;
        break;
    case 16:
        mouthModifiers[0] = ModifyUpperNegativeCorner;
        mouthModifiers[1] = ModifyLowerNegativeCorner;
        break;
    case 17:
        mouthModifiers[0] = ModifyUpperPositiveCorner;
        mouthModifiers[1] = ModifyLowerPositiveCorner;
        break;
    case 18:
        mouthModifiers[0] = ModifyUpperLipCorner;
        mouthModifiers[1] = ModifyLowerLipCorner;
        break;
    case 19:
        mouthModifiers[0] = ModifyUpperWideSmile;
        mouthModifiers[1] = ModifyLowerWideSmile;
        break;
    case 20:
        mouthModifiers[0] = ModifyUpperLipArchWeighted;
        mouthModifiers[1] = ModifyLowerLipArchWeighted;
        break;
    }
}

// @verified - d
void PrepareMouthClosed(FacialModelData &face, std::int32_t duration) {
    SelectMouthModifiers(face);
    {
        const std::int32_t count = face.groups[6].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[6].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = v.neutral;
            if (mouthModifiers[0])
                mouthModifiers[0](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    {
        const std::int32_t count = face.groups[8].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[8].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = v.neutral;
            if (mouthModifiers[1])
                mouthModifiers[1](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    {
        const std::int32_t count = face.groups[7].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[7].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = v.neutral;
            if (mouthModifiers[2])
                mouthModifiers[2](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    if (face.groups[6].count)
        face.flags |= 1U << 6U;
    if (face.groups[8].count)
        face.flags |= 1U << 8U;
    {
        const FacialMorphGroup &group = face.groups[9];
        const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count,
            static_cast<float>(duration));
        face.flags = (face.flags & ~(1U << 11U)) | ((active & 1U) << 11U);
    }
    if (face.groups[7].count)
        face.flags |= 1U << 7U;
}

// @verified - d
void PrepareMouthA(FacialModelData &face, std::int32_t duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face);
    const float (&coefficient)[30] = face.shapeCoefficients;
    const float upperDrop = (amount * 0.004f) * coefficient[8];
    const float width = 1.0f - (amount * 0.05f) * coefficient[10];
    {
        const std::int32_t count = face.groups[6].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[6].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x, width * v.neutral.y, v.neutral.z - (0.9f - v.weight0) * upperDrop};
            if (mouthModifiers[0])
                mouthModifiers[0](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float lowerDrop = (amount * 0.011f) * coefficient[9];
    const float retreat = (amount * 0.0033f) * coefficient[9];
    {
        const std::int32_t count = face.groups[8].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[8].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x - retreat, width * v.neutral.y, v.neutral.z - lowerDrop * v.weight1};
            if (mouthModifiers[1])
                mouthModifiers[1](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float innerDrop = (amount * 0.0066f) * coefficient[9];
    {
        const std::int32_t count = face.groups[7].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[7].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float weight = v.weight1 + v.weight0;
            v.target = (__typeof__(v.target)){v.neutral.x - weight * innerDrop, v.neutral.y, v.neutral.z - weight * (innerDrop * 1.2f)};
            if (mouthModifiers[2])
                mouthModifiers[2](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    if (face.groups[6].count)
        face.flags |= 1U << 6U;
    if (face.groups[8].count)
        face.flags |= 1U << 8U;
    {
        const FacialMorphGroup &group = face.groups[9];
        const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count,
            static_cast<float>(duration));
        face.flags = (face.flags & ~(1U << 11U)) | ((active & 1U) << 11U);
    }
    if (face.groups[7].count)
        face.flags |= 1U << 7U;
}

// @verified - d
void PrepareMouthI(FacialModelData &face, std::int32_t duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face);
    const float (&coefficient)[30] = face.shapeCoefficients;
    const float size = amount * 0.005f;
    const float retreat = size * coefficient[11];
    const float elevation = (amount * 0.002f) * coefficient[8];
    const float width = ((amount * 0.1f) * ( coefficient[10]) + ( 1.0f));
    {
        const std::int32_t count = face.groups[6].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[6].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float offset = v.weight0 < 0.3f ? (0.3f - v.weight0) * retreat : 0.0f;
            v.target = (__typeof__(v.target)){v.neutral.x - offset * 4.0f, width * v.neutral.y, ((elevation) * ( v.weight0) + ( v.neutral.z))};
            if (mouthModifiers[0])
                mouthModifiers[0](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float lowerDrop = size * coefficient[9];
    {
        const std::int32_t count = face.groups[8].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[8].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float offset = v.weight1 < 0.3f ? (0.3f - v.weight1) * retreat : 0.0f;
            v.target = (__typeof__(v.target)){v.neutral.x - offset * 4.0f, width * v.neutral.y, v.neutral.z - v.weight1 * lowerDrop};
            if (mouthModifiers[1])
                mouthModifiers[1](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    {
        const FacialMorphGroup &group = face.groups[7];
        const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count,
            static_cast<float>(duration));
        face.flags = (face.flags & ~(1U << 7U)) | ((active & 1U) << 7U);
    }
    const float cornerWidth = ((amount * 0.05f) * ( coefficient[10]) + ( 1.0f));
    {
        const std::int32_t count = face.groups[9].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[9].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z};
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    if (face.groups[9].count)
        face.flags |= 1U << 11U;
    if (face.groups[6].count)
        face.flags |= 1U << 6U;
    if (face.groups[8].count)
        face.flags |= 1U << 8U;
}

// @verified - d
void PrepareMouthU(FacialModelData &face, std::int32_t duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face);
    const float (&coefficient)[30] = face.shapeCoefficients;
    const float advanceScale = amount * 0.003f;
    const float advance = advanceScale * coefficient[8];
    const float drop = (amount * 0.005f) * coefficient[11];
    const float elevation = (amount * 0.0015f) * coefficient[8];
    const float width = 1.0f - (amount * 0.3f) * coefficient[10];
    {
        const std::int32_t count = face.groups[6].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[6].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float offset = v.weight0 < 0.3f ? (0.3f - v.weight0) * drop : 0.0f;
            v.target = (__typeof__(v.target)){((advance) * ( v.weight0) + ( v.neutral.x)), width * v.neutral.y, (((elevation) * ( v.weight0) + ( v.neutral.z))) - offset};
            if (mouthModifiers[0])
                mouthModifiers[0](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float lowerAdvance = advanceScale * coefficient[9];
    {
        const std::int32_t count = face.groups[8].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[8].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float offset = v.weight1 < 0.3f ? (0.3f - v.weight1) * drop : 0.0f;
            v.target = (__typeof__(v.target)){((lowerAdvance) * ( v.weight1) + ( v.neutral.x)), width * v.neutral.y, v.neutral.z - offset};
            if (mouthModifiers[1])
                mouthModifiers[1](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float cornerWidth = 1.0f - (amount * 0.1f) * coefficient[10];
    {
        const std::int32_t count = face.groups[9].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[9].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z};
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    if (face.groups[9].count)
        face.flags |= 1U << 11U;
    if (face.groups[6].count)
        face.flags |= 1U << 6U;
    if (face.groups[8].count)
        face.flags |= 1U << 8U;
    {
        const FacialMorphGroup &group = face.groups[7];
        const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count,
            static_cast<float>(duration));
        face.flags = (face.flags & ~(1U << 7U)) | ((active & 1U) << 7U);
    }
}

// @verified - d
void PrepareMouthE(FacialModelData &face, std::int32_t duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face);
    const float (&coefficient)[30] = face.shapeCoefficients;
    const float widthScale = amount * 0.05f;
    const float elevation = (amount * 0.0015f) * coefficient[8];
    const float retreat = (amount * 0.016f) * coefficient[11];
    const float width = ((widthScale) * ( coefficient[10]) + ( 1.0f));
    {
        const std::int32_t count = face.groups[6].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[6].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float offset = v.weight0 < 0.3f ? (0.3f - v.weight0) * retreat : 0.0f;
            v.target = (__typeof__(v.target)){v.neutral.x - offset, width * v.neutral.y, ((elevation) * ( v.weight0) + ( v.neutral.z))};
            if (mouthModifiers[0])
                mouthModifiers[0](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float lowerRetreat = retreat + retreat;
    const float lowerDrop = (amount * 0.007f) * coefficient[9];
    {
        const std::int32_t count = face.groups[8].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[8].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float offset = v.weight1 < 0.3f ? (0.3f - v.weight1) * lowerRetreat : 0.0f;
            v.target = (__typeof__(v.target)){v.neutral.x - offset, width * v.neutral.y, v.neutral.z - v.weight1 * lowerDrop};
            if (mouthModifiers[1])
                mouthModifiers[1](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float innerDrop = (amount * 0.004f) * coefficient[9];
    {
        const std::int32_t count = face.groups[7].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[7].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float offset = (v.weight1 + v.weight0) * innerDrop;
            v.target = (__typeof__(v.target)){v.neutral.x - offset, v.neutral.y, v.neutral.z - offset};
            if (mouthModifiers[2])
                mouthModifiers[2](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float cornerWidth = ((widthScale) * ( coefficient[10]) + ( 1.0f));
    {
        const std::int32_t count = face.groups[9].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[9].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z};
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    if (face.groups[9].count)
        face.flags |= 1U << 11U;
    if (face.groups[6].count)
        face.flags |= 1U << 6U;
    if (face.groups[8].count)
        face.flags |= 1U << 8U;
    if (face.groups[7].count)
        face.flags |= 1U << 7U;
}

// @verified - d
void PrepareMouthO(FacialModelData &face, std::int32_t duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face);
    const float (&coefficient)[30] = face.shapeCoefficients;
    const float advanceScale = amount * 0.003f;
    const float advance = advanceScale * coefficient[8];
    const float upperDrop = (amount * 0.001f) * coefficient[8];
    const float width = 1.0f - (amount * 0.4f) * coefficient[10];
    {
        const std::int32_t count = face.groups[6].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[6].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){((advance) * ( v.weight0) + ( v.neutral.x)), width * v.neutral.y, ((v.weight0 - 1.0f) * ( upperDrop) + ( v.neutral.z))};
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float lowerDrop = (amount * 0.005f) * coefficient[9];
    const float lowerAdvance = coefficient[9] * advanceScale;
    const float lowerWidth = (amount * 0.24f) * coefficient[10];
    {
        const std::int32_t count = face.groups[8].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[8].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){((lowerAdvance) * ( v.weight1) + ( v.neutral.x)), (1.0f - (v.weight1 + 1.0f) * lowerWidth) * v.neutral.y,
                v.neutral.z - lowerDrop * v.weight1};
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float cornerWidth = 1.0f - (amount * 0.1f) * coefficient[10];
    {
        const std::int32_t count = face.groups[9].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[9].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x, cornerWidth * v.neutral.y, v.neutral.z};
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float innerDrop = (amount * 0.004f) * coefficient[9];
    {
        const std::int32_t count = face.groups[7].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[7].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            const float weight = v.weight1 + v.weight0;
            v.target = (__typeof__(v.target)){v.neutral.x - weight * (innerDrop * 0.5f), v.neutral.y, v.neutral.z - weight * innerDrop};
            if (mouthModifiers[2])
                mouthModifiers[2](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    if (face.groups[6].count)
        face.flags |= 1U << 6U;
    if (face.groups[8].count)
        face.flags |= 1U << 8U;
    if (face.groups[9].count)
        face.flags |= 1U << 11U;
    if (face.groups[7].count)
        face.flags |= 1U << 7U;
}

// @verified - d
void PrepareMouthLeadIn(FacialModelData &face, std::int32_t duration, float scale) {
    const float amount = scale * 0.8f;
    SelectMouthModifiers(face);
    {
        const std::int32_t count = face.groups[6].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[6].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = v.neutral;
            if (mouthModifiers[0])
                mouthModifiers[0](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float lowerDrop = (amount * 0.005f) * face.shapeCoefficients[9];
    {
        const std::int32_t count = face.groups[8].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[8].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x, v.neutral.y, v.neutral.z - lowerDrop * v.weight1};
            if (mouthModifiers[1])
                mouthModifiers[1](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    const float innerDrop = (amount * 0.002f) * face.shapeCoefficients[9];
    {
        const std::int32_t count = face.groups[7].count;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(face.groups[7].offset);
        for (std::int32_t index = 0; index < count; ++index) {
            FacialMorphVertex &v = vertices[index];
            v.target = (__typeof__(v.target)){v.neutral.x - v.weight0 * (innerDrop * 0.5f), v.neutral.y, v.neutral.z - v.weight0 * innerDrop};
            if (mouthModifiers[2])
                mouthModifiers[2](face, v);
            v.target.x = u32AsFloat(floatAsU32(v.target.x) | 1U);
            SnapshotFacialTarget(v, static_cast<float>(duration));
        }
    }
    if (face.groups[6].count)
        face.flags |= 1U << 6U;
    if (face.groups[8].count)
        face.flags |= 1U << 8U;
    {
        const FacialMorphGroup &group = face.groups[9];
        const std::uint32_t active = BeginNeutralFacialGroup(reinterpret_cast<FacialMorphVertex *>(group.offset), group.count,
            static_cast<float>(duration));
        face.flags = (face.flags & ~(1U << 11U)) | ((active & 1U) << 11U);
    }
    if (face.groups[7].count)
        face.flags |= 1U << 7U;
}

} } // namespace shenmue::s2

#endif
