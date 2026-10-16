#pragma once

#include "s1/task/effe.hpp"

#include <array>
#include <cstring>

namespace shenmue::s1::effe_detail {

inline constexpr std::size_t kEffeSlots = 16;

inline constexpr std::size_t kSakuraSlots = 8;

// PAUS_Callback sets this on the existing particle array;
// Shenmue_ModifyTasksForPauseScreen clears it on resume.
// It selects the alpha-table draw callbacks, not a different allocation.
inline constexpr std::uint32_t kParticleCallbacksSuppressedFlag = 0x02000000U;

inline constexpr std::uint32_t kCallbackOnlyParticleFlag = 0x04000000U;

inline constexpr std::uint32_t kTransparentParticlePassFlag = 0x40000000U;

// SubmitAlphaQuads selects FVF/VS 26, PS 10, stride 16 for bit 29.
// Its selected table entries only walk 0x28 records; none emit vertices.
inline constexpr std::uint32_t kAlternateParticleRecordFlag = 0x20000000U;

enum class EffeFamily : std::uint32_t {
    Special,
    Efpt,
    Generic,
};




struct EffeAlternateParticle {
    std::byte reserved00[0x08];
    void (*update)(EffeAlternateParticle *);
    std::byte reserved10[0x18];
};

static_assert(sizeof(EffeAlternateParticle) == 0x28);

static_assert(offsetof(EffeAlternateParticle, update) == 0x08);



inline constexpr std::array<Vector4, 7> kGenericCurve{{
    {0.0f, 0.0f, 0.0f, 0.2f},
    {0.0f, 1.5f, 0.0f, 0.6f},
    {-1.5f, 1.5f, 0.0f, 0.8f},
    {-1.5f, 2.0f, -1.5f, 0.8f},
    {-1.5f, 2.5f, -3.0f, 0.8f},
    {1.0f, 2.5f, -3.0f, 0.8f},
    {1.0f, 5.0f, -2.5f, 2.0f},
}};

struct EffeProjectionBasis {
    Vector3 right;
    Vector3 up;
    Vector3 forward;
    float tangent;
    float aspect;
    float logicalWidthScale;
};

} // namespace shenmue::s1::effe_detail
