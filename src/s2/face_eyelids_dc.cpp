#include "s2/face_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cmath>

namespace shenmue { namespace s2 {

// @verified - d
void SnapshotFacialTarget(FacialMorphVertex &vertex, float duration) {
    vertex.start.x = vertex.position->x;
    vertex.start.y = vertex.position->y;
    vertex.start.z = vertex.position->z;
    if (duration == 0.0f) {
        vertex.position->x = vertex.target.x;
        vertex.position->y = vertex.target.y;
        vertex.position->z = vertex.target.z;
    }
}

// @verified - d
void PrepareOuterEyelids(FacialModelData &face, float duration, float first, float second) {
    float curves[2]= {first < 0.3f ? first : 0.3f, second < 0.3f ? second : 0.3f};
    float closures[2]= {first, second};
    if (face.shapeCoefficients[29] != 0.0f) {
        for (std::size_t rangeIndex71 = 0; rangeIndex71 < sizeof(closures) / sizeof((closures)[0]); ++rangeIndex71) {
        float & closure = (closures)[rangeIndex71];

            if (closure < 0.8f) {
                closure += face.shapeCoefficients[29];
                if (closure > 0.8f)
                    closure = 0.8f;
            }
        
    }
    }
    float displacements[2];
    for (std::uint32_t side = 0; side < 2; ++side) {
        const float coefficient = face.shapeCoefficients[side + 2];
        displacements[side] = closures[side] * (coefficient * 0.0024f);
        if (face.upperExpression == 2)
            curves[side] *= coefficient * 0.0048f;
        else if (face.upperExpression == 15)
            curves[side] *= coefficient * 0.002f;
    }
    for (std::uint32_t side = 0; side < 2; ++side) {
        const std::uint32_t groupIndex = side + 1;
        const FacialMorphGroup &group = face.groups[groupIndex];
        if (!group.count)
            continue;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
        for (std::int32_t index = 0; index < group.count; ++index) {
            FacialMorphVertex &vertex = vertices[index];
            if (face.upperExpression == 2) {
                const float phase = ((1.0f - vertex.weight0) * ( 36864.0f) + ( 4096.0f));
                std::int32_t angle;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
                angle = TruncateToInt32(phase);
#else
                asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(angle) : "f"(phase) : "fpul");
#endif
                Vector2 values;
                CalculateFixedSinCos(static_cast<std::uint32_t>(angle), &values);
                const float curve = values.y + 0.7f;
                vertex.target.x = vertex.neutral.x;
                vertex.target.y = vertex.neutral.y;
                const float height = vertex.neutral.z - displacements[side] * vertex.weight1;
                vertex.target.z = ((curve) * ( curves[side]) + ( height));
            } else if (face.upperExpression == 15) {
                const float position = 1.0f - vertex.weight0;
                const float difference = position - 0.5f;
                const float fraction = position > 0.5f ? difference + difference : 0.0f;
                const float phase = fraction * 16384.0f;
                std::int32_t angle;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
                angle = TruncateToInt32(phase);
#else
                asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(angle) : "f"(phase) : "fpul");
#endif
                Vector2 values;
                CalculateFixedSinCos(static_cast<std::uint32_t>(angle), &values);
                vertex.target.x = vertex.neutral.x;
                vertex.target.y = vertex.neutral.y;
                const float height = vertex.neutral.z - displacements[side] * vertex.weight1;
                vertex.target.z = height - curves[side] * values.x;
            } else {
                vertex.target.x = vertex.neutral.x;
                vertex.target.y = vertex.neutral.y;
                vertex.target.z = vertex.neutral.z - displacements[side] * vertex.weight1;
            }
            SnapshotFacialTarget(vertex, duration);
        }
        face.flags |= 1U << groupIndex;
    }
}

// @verified - d
void PrepareInnerEyelids(FacialModelData &face, float duration, float first, float second) {
    const float displacements[2]= {first * (face.shapeCoefficients[4] * 0.0045f), second * (face.shapeCoefficients[5] * 0.0045f)};
    for (std::uint32_t side = 0; side < 2; ++side) {
        const float displacement = displacements[side];
        const float horizontal = displacement * 0.3f;
        const std::uint32_t groupIndex = side + 4;
        const FacialMorphGroup &group = face.groups[groupIndex];
        if (!group.count)
            continue;
        FacialMorphVertex *const vertices = reinterpret_cast<FacialMorphVertex *>(group.offset);
        for (std::int32_t index = 0; index < group.count; ++index) {
            FacialMorphVertex &vertex = vertices[index];
            vertex.target.x = ((horizontal) * ( vertex.weight1) + ( vertex.neutral.x));
            vertex.target.x = u32AsFloat(floatAsU32(vertex.target.x) | 1U);
            vertex.target.y = vertex.neutral.y;
            vertex.target.z = ((displacement) * ( vertex.weight1) + ( vertex.neutral.z));
            SnapshotFacialTarget(vertex, duration);
        }
        face.flags |= 1U << groupIndex;
    }
}

} } // namespace shenmue::s2

#endif
