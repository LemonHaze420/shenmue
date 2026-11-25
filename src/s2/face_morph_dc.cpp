#include "s2/face_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/runtime_arithmetic.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

// @verified - d
void InterpolateFacialGroup(FacialMorphVertex *vertices, std::int32_t count, float amount) {
    for (std::int32_t index = 0; index < count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.position->x = ((vertex.target.x - vertex.start.x) * (amount) + (vertex.start.x));
        vertex.position->y = ((vertex.target.y - vertex.start.y) * (amount) + (vertex.start.y));
        vertex.position->z = ((vertex.target.z - vertex.start.z) * (amount) + (vertex.start.z));
        vertex.position->x = u32AsFloat(floatAsU32(vertex.position->x) | 1U);
    }
}

// @verified - d
void InterpolateFacialGroup(FacialMorphVertex *vertices, std::int32_t count, std::int32_t frame, std::int32_t duration) {
    if (!duration)
        return;
    Vector2 values;
    const std::int32_t numerator = static_cast<std::int32_t>(static_cast<std::uint32_t>(frame) << 16U);
    CalculateFixedSinCos(static_cast<std::uint32_t>(dc::SignedDivide(numerator, duration)), &values);
    const float amount = static_cast<float>(frame) / static_cast<float>(duration) - values.x * 0.1f;
    for (std::int32_t index = 0; index < count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.position->x = ((vertex.target.x - vertex.start.x) * (amount) + (vertex.start.x));
        vertex.position->y = ((vertex.target.y - vertex.start.y) * (amount) + (vertex.start.y));
        vertex.position->z = ((vertex.target.z - vertex.start.z) * (amount) + (vertex.start.z));
        vertex.position->x = u32AsFloat(floatAsU32(vertex.position->x) | 1U);
    }
}

// @verified - d
void FinishFacialGroup(FacialMorphVertex *vertices, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index) {
        FacialMorphVertex &vertex = vertices[index];
        vertex.position->x = vertex.target.x;
        vertex.position->y = vertex.target.y;
        vertex.position->z = vertex.target.z;
        vertex.start.x = vertex.target.x;
        vertex.start.y = vertex.target.y;
        vertex.start.z = vertex.target.z;
        vertex.position->x = u32AsFloat(floatAsU32(vertex.position->x) | 1U);
    }
}

// @verified - d
void InterpolateUpperFace(FacialModelData &face) {
    if (face.flags & 1U)
        InterpolateFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[0].offset), face.groups[0].count, face.upperFaceFrame, face.upperFaceDuration);
}

// @verified - d
void FinishUpperFace(FacialModelData &face) {
    if (face.flags & 1U) {
        FinishFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[0].offset), face.groups[0].count);
        face.flags &= ~1U;
    }
}

// @verified - d
void InterpolateLowerFace(FacialModelData &face) {
    if (face.flags & 8U)
        InterpolateFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[3].offset), face.groups[3].count, face.lowerFaceFrame, face.lowerFaceDuration);
}

// @verified - d
void FinishLowerFace(FacialModelData &face) {
    if (face.flags & 8U) {
        FinishFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[3].offset), face.groups[3].count);
        face.flags &= ~8U;
    }
}

// @verified - d
void InterpolateEyelids(FacialModelData &face) {
    if (!face.eyelidDuration)
        return;
    const float amount = static_cast<float>(face.eyelidFrame) / static_cast<float>(face.eyelidDuration);
    for (std::size_t rangeIndex5 = 0; rangeIndex5 < 4; ++rangeIndex5) {
        const std::uint32_t index = ((std::uint32_t[]){1U, 2U, 4U, 5U})[rangeIndex5];

        if (face.flags & (1U << index))
            InterpolateFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[index].offset), face.groups[index].count, amount);
    }
}

// @verified - d
void InterpolateClosingEyelids(FacialModelData &face) {
    if (!face.eyelidDuration)
        return;
    Vector2 values;
    const std::int32_t numerator = static_cast<std::int32_t>(static_cast<std::uint32_t>(face.eyelidFrame) * 32768U);
    CalculateFixedSinCos(static_cast<std::uint32_t>(dc::SignedDivide(numerator, face.eyelidDuration)), &values);
    const float amount = static_cast<float>(face.eyelidFrame) / static_cast<float>(face.eyelidDuration) - values.x * 0.2f;
    for (std::size_t rangeIndex6 = 0; rangeIndex6 < 4; ++rangeIndex6) {
        const std::uint32_t index = ((std::uint32_t[]){1U, 2U, 4U, 5U})[rangeIndex6];

        if (face.flags & (1U << index))
            InterpolateFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[index].offset), face.groups[index].count, amount);
    }
}

// @verified - d
void InterpolateOpeningEyelids(FacialModelData &face) {
    if (!face.eyelidDuration)
        return;
    Vector2 values;
    const std::int32_t numerator = static_cast<std::int32_t>(static_cast<std::uint32_t>(face.eyelidFrame) * 32768U);
    CalculateFixedSinCos(static_cast<std::uint32_t>(dc::SignedDivide(numerator, face.eyelidDuration)), &values);
    const float ratio = static_cast<float>(face.eyelidFrame) / static_cast<float>(face.eyelidDuration);
    const float amount = ((values.x) * (0.2f) + (ratio));
    for (std::size_t rangeIndex7 = 0; rangeIndex7 < 4; ++rangeIndex7) {
        const std::uint32_t index = ((std::uint32_t[]){1U, 2U, 4U, 5U})[rangeIndex7];

        if (face.flags & (1U << index))
            InterpolateFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[index].offset), face.groups[index].count, amount);
    }
}

// @verified - d
void FinishEyelids(FacialModelData &face) {
    for (std::size_t rangeIndex8 = 0; rangeIndex8 < 4; ++rangeIndex8) {
        const std::uint32_t index = ((std::uint32_t[]){1U, 2U, 4U, 5U})[rangeIndex8];

        if (face.flags & (1U << index)) {
            FinishFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[index].offset), face.groups[index].count);
            face.flags &= ~(1U << index);
        }
    }
}

// @verified - d
void InterpolateMouth(FacialModelData &face) {
    for (std::size_t rangeIndex9 = 0; rangeIndex9 < 4; ++rangeIndex9) {
        const std::uint32_t index = ((std::uint32_t[]){6U, 8U, 9U, 7U})[rangeIndex9];

        const std::uint32_t mask = index == 9U ? 0x800U : 1U << index;
        if (face.flags & mask)
            InterpolateFacialGroup(
                reinterpret_cast<FacialMorphVertex *>(face.groups[index].offset), face.groups[index].count, face.mouthFrame, face.mouthDuration);
    }
}

// @verified - d
void FinishMouth(FacialModelData &face) {
    for (std::size_t rangeIndex10 = 0; rangeIndex10 < 4; ++rangeIndex10) {
        const std::uint32_t index = ((std::uint32_t[]){6U, 8U, 9U, 7U})[rangeIndex10];

        const std::uint32_t mask = index == 9U ? 0x800U : 1U << index;
        if (face.flags & mask) {
            FinishFacialGroup(reinterpret_cast<FacialMorphVertex *>(face.groups[index].offset), face.groups[index].count);
            face.flags &= ~mask;
        }
    }
}

// @verified - d
void BeginFacialBlink(FacialModelData &face) {
    if (face.eyelidDuration || face.upperFaceDuration)
        return;
    face.flags |= 0x600U;
    face.eyelidFrame = 0;
    face.eyelidDuration = 2;
    const float first = face.eyelid0OuterWeight > 1.0f ? face.eyelid0OuterWeight : 2.0f - face.eyelid0InnerWeight;
    const float second = face.eyelid1OuterWeight > 1.0f ? face.eyelid1OuterWeight : 2.0f - face.eyelid1InnerWeight;
    PrepareOuterEyelids(face, 2, first, second);
}

// @verified - d
void BeginFacialBlinkRecovery(FacialModelData &face) {
    face.flags &= ~0x200U;
    face.eyelidFrame = 0;
    face.eyelidDuration = 3;
    PrepareOuterEyelids(face, 3, face.eyelid0OuterWeight, face.eyelid1OuterWeight);
    PrepareInnerEyelids(face, 3, face.eyelid0InnerWeight, face.eyelid1InnerWeight);
}

// @verified - d
void AdvanceFacialModel(FacialModelData &face) {
    if (face.eyelidDuration) {
        if (face.eyelidFrame < face.eyelidDuration) {
            if (face.eyelidFrame) {
                if (!(face.flags & 0x200U))
                    InterpolateEyelids(face);
                else if (face.flags & 0x400U)
                    InterpolateClosingEyelids(face);
                else
                    InterpolateOpeningEyelids(face);
            }
            face.eyelidFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(face.eyelidFrame) + 1U);
        } else if (!(face.flags & 0x200U)) {
            FinishEyelids(face);
            face.eyelidDuration = 0;
        } else if (face.flags & 0x400U) {
            FinishEyelids(face);
            face.flags &= ~0x400U;
            face.eyelidFrame = face.eyelidDuration = 1;
            BeginFacialBlinkRecovery(face);
        }
    }
    if (face.upperFaceDuration) {
        if (face.upperFaceFrame < face.upperFaceDuration) {
            if (face.upperFaceFrame)
                InterpolateUpperFace(face);
            face.upperFaceFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(face.upperFaceFrame) + 1U);
        } else {
            FinishUpperFace(face);
            face.upperFaceDuration = 0;
        }
    }
    if (face.lowerFaceDuration) {
        if (face.lowerFaceFrame < face.lowerFaceDuration) {
            if (face.lowerFaceFrame)
                InterpolateLowerFace(face);
            face.lowerFaceFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(face.lowerFaceFrame) + 1U);
        } else {
            FinishLowerFace(face);
            face.lowerFaceDuration = 0;
        }
    }
    if (face.mouthDuration) {
        if (face.mouthFrame < face.mouthDuration) {
            if (face.mouthFrame)
                InterpolateMouth(face);
            face.mouthFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(face.mouthFrame) + 1U);
        } else {
            FinishMouth(face);
            face.mouthDuration = 0;
        }
    }
}

} // namespace s2
} // namespace shenmue

#endif
