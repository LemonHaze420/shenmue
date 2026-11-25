#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/face_data.hpp"
#include "s2/model_dc.hpp"
#include "common/types.hpp"
namespace shenmue {
namespace s2 {

namespace {

// @verified - d
void ResetFacialModelTransitions(FacialModelData &data) {
    data.upperFaceDuration = 0.0f;
    data.lowerFaceDuration = 0.0f;
    data.eyelidDuration = 0.0f;
    data.mouthDuration = 0.0f;
    data.flags &= 0xfffff400U;
    data.expressionScale = 1.0f;
    data.eyelid0OuterWeight = 0.0f;
    data.eyelid1OuterWeight = 0.0f;
    data.eyelid0InnerWeight = 0.0f;
    data.eyelid1InnerWeight = 0.0f;
    data.eyelid0Mode = 1;
    data.eyelid1Mode = 1;
    if (data.shapeCoefficients[11] == 0.0f)
        data.shapeCoefficients[11] = 1.0f;
}

} // namespace

namespace {

// @verified - d
void RelocateFacialVertexBuffers(std::byte *vertices, std::uint32_t modelBase, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index) {
        std::uint32_t &buffer = *reinterpret_cast<std::uint32_t *>(vertices + 0x2c);
        buffer += modelBase;
        vertices += 0x30;
    }
}

} // namespace

// @verified - d
std::int32_t ParseFaceResource(void *face, void *base) {
    FacialModelData &data = *static_cast<FacialModelData *>(face);
    if (data.signature != SHEN_NAME(FACE) || data.version != 4)
        return -1;
    ResetFacialModelTransitions(data);
    const std::uint32_t faceBase = reinterpret_cast<std::uintptr_t>(face);
    for (std::size_t rangeIndex68 = 0; rangeIndex68 < sizeof(data.groups) / sizeof((data.groups)[0]); ++rangeIndex68) {
        FacialMorphGroup &group = (data.groups)[rangeIndex68];

        if (group.count && static_cast<std::uint32_t>(group.offset) < faceBase) {
            group.offset = static_cast<std::int32_t>(faceBase + static_cast<std::uint32_t>(group.offset));
            RelocateFacialVertexBuffers(reinterpret_cast<std::byte *>(group.offset), reinterpret_cast<std::uintptr_t>(base), group.count);
        }
    }
    return 0;
}

// @verified - d
void InitializeFaceResource(void *face) {
    FacialModelData &data = *static_cast<FacialModelData *>(face);
    ResetFacialModelTransitions(data);
    data.signature = SHEN_NAME(FACE);
    data.version = 4;
    data.modelOffset = 0;
    data.reserved14 = 0;
    data.reserved10 = 0;
    for (std::size_t rangeIndex69 = 0; rangeIndex69 < sizeof(data.groups) / sizeof((data.groups)[0]); ++rangeIndex69) {
        FacialMorphGroup &group = (data.groups)[rangeIndex69];
        group.count = 0;
    }
    for (std::size_t rangeIndex70 = 0; rangeIndex70 < sizeof(data.lookLimits) / sizeof((data.lookLimits)[0]); ++rangeIndex70) {
        std::int32_t &limit = (data.lookLimits)[rangeIndex70];
        limit = 0;
    }
}

} // namespace s2
} // namespace shenmue
#endif
