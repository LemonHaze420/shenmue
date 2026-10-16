#pragma once

#include "s1/task/effe.hpp"
#include "s1/lighting.hpp"

#include <array>
#include <cstring>

namespace shenmue::s1::effe_detail {

inline constexpr std::size_t kEffectTransformContextBytes = 0x17f8;

inline constexpr std::size_t kEffectTransformLightSlotsOffset = 0x40;

inline constexpr std::size_t kEffectTransformUnitScaleOffset = 0x1740;

inline constexpr std::size_t kEffectTransformPrimaryVectorOffset = 0x1744;

inline constexpr std::size_t kEffectTransformSecondaryVectorOffset = 0x1754;

inline constexpr std::size_t kEffectTransformSceneLightingYZOffset = 0x1764;

inline constexpr std::size_t kEffectTransformSceneLightingWOffset = 0x176c;

inline constexpr std::size_t kEffectTransformReservedWordOffset = 0x1770;

inline constexpr std::size_t kEffectTransformSubmissionLightOffset = 0x1774;

inline constexpr std::size_t kEffectTransformSubmissionReservedOffset = 0x1780;

inline constexpr std::size_t kEffectTransformFogNearOffset = 0x178c;

inline constexpr std::size_t kEffectTransformFogFarOffset = 0x1790;

inline constexpr std::size_t kEffectTransformFogMaximumOffset = 0x1794;

inline constexpr std::size_t kEffectTransformFlagsOffset = 0x179c;

inline constexpr std::size_t kEffectTransformAuxiliaryMatrixOffset = 0x17a0;

inline constexpr std::size_t kEffectTransformReservedVectorOffset = 0x17e0;

inline constexpr std::size_t kEffectTransformMaxScaleOffset = 0x17f0;

static_assert(kEffectTransformLightSlotsOffset + sizeof(g_effectLightRenderSlots) == 0x1740);

static_assert(kEffectTransformFogMaximumOffset + sizeof(float) <= kEffectTransformFlagsOffset);

static_assert(kEffectTransformAuxiliaryMatrixOffset + sizeof(Matrix4x4) == 0x17e0);

static_assert(kEffectTransformMaxScaleOffset + sizeof(float) <= kEffectTransformContextBytes);

struct EffectBufferConfiguration {
    void *storage;
    std::int32_t transformContextCount;
    std::int32_t auxiliaryCount;
    std::int32_t queueEntryCount;
};

inline constexpr std::uint32_t kEffectLightSlots = 32;

inline constexpr std::array<std::uint32_t, 46> kEffectLightTemplate{
    0x00000000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x3f800000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
    0x37800000U,
    0xff0f0f0fU,
    0x4f800000U,
    0x3f800000U,
    0x47800000U,
    0x00000000U,
    0x00000000U,
    0x00000000U,
};

inline constexpr float kDefaultInverseGeometryScale = 0.00390625f;

} // namespace shenmue::s1::effe_detail
