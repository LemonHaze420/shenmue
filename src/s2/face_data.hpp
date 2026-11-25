#pragma once
#include "shenmue.h"
#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#define SHENMUE_FACE_ARRAY(type, name, count) type name[count]
#else
#include <array>
#include <cstddef>
#include <cstdint>
#define SHENMUE_FACE_ARRAY(type, name, count) std::array<type, count> name
#endif
namespace shenmue { namespace s2 {
#pragma pack(push, 4)
struct FacialMorphGroup {
    std::int32_t count;
    std::int32_t offset;
};

// FACE/FACX version 4, as read by native FACE_Reader.
struct FacialModelData {
    std::uint32_t signature; // 0x000
    std::uint32_t size;
    std::uint32_t version;
    std::int32_t modelOffset;
    std::int32_t reserved10;
    std::int32_t reserved14;
    std::uint32_t reserved18;
    std::int32_t upperExpression;
    std::int32_t expression; // 0x020
    std::int32_t mouthExpression;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t upperFaceFrame;
    std::int32_t upperFaceDuration;
    std::int32_t lowerFaceFrame;
    std::int32_t lowerFaceDuration;
    std::int32_t eyelidFrame;
    std::int32_t eyelidDuration;
    std::int32_t mouthFrame;
    std::int32_t mouthDuration;
#else
    float upperFaceFrame;
    float upperFaceDuration;
    float lowerFaceFrame;
    float lowerFaceDuration;
    float eyelidFrame;
    float eyelidDuration;
    float mouthFrame;
    float mouthDuration;
#endif
    std::uint32_t flags; // 0x048
    float expressionScale;
    float eyelid0OuterWeight;
    float eyelid1OuterWeight;
    float eyelid0InnerWeight;
    float eyelid1InnerWeight;
    SHENMUE_FACE_ARRAY(float, shapeCoefficients, 30); // 0x060
    std::int32_t eyelid0Mode;                // 0x0d8
    std::int32_t eyelid1Mode;
    SHENMUE_FACE_ARRAY(std::int32_t, lookLimits, 6);  // 0x0e0
    SHENMUE_FACE_ARRAY(FacialMorphGroup, groups, 10); // 0x0f8
};

#pragma pack(pop)
#if SHENMUE_S2_GCC295
typedef char FacialMorphGroupSizeCheck[sizeof(FacialMorphGroup) == 8 ? 1 : -1];
typedef char FacialModelDataSizeCheck[sizeof(FacialModelData) == 0x148 ? 1 : -1];
typedef char FacialModelDataAlignmentCheck[__alignof__(FacialModelData) == 4 ? 1 : -1];
typedef char FacialModelFlagsOffsetCheck[offsetof(FacialModelData, flags) == 0x48 ? 1 : -1];
typedef char FacialModelShapesOffsetCheck[offsetof(FacialModelData, shapeCoefficients) == 0x60 ? 1 : -1];
typedef char FacialModelGroupsOffsetCheck[offsetof(FacialModelData, groups) == 0xf8 ? 1 : -1];
#else
static_assert(sizeof(FacialModelData) == 0x148);
static_assert(offsetof(FacialModelData, flags) == 0x48);
static_assert(offsetof(FacialModelData, groups) == 0xf8);
#endif
} } // namespace shenmue::s2

#undef SHENMUE_FACE_ARRAY
