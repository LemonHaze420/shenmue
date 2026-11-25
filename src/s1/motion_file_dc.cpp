#include "shenmue.h"

#if SHENMUE_IS_DC

#if SHENMUE_IS_S2
#include "s2/motion_registry_dc.hpp"
namespace shenmue {
namespace s2 {
#else
#include "s1/motion_registry.hpp"
namespace shenmue {
namespace s1 {
#endif

namespace motion_detail {
// @verified - d
float F16ToF32(std::int16_t value) {
    const std::uint32_t packed = static_cast<std::uint16_t>(value);
    if (!(packed & 0x7fffU))
        return 0.0f;
    return u32AsFloat(((packed & 0x7fffU) << 13) + ((packed & 0x8000U) << 16) + 0x38000000U);
}

std::int16_t F32ToF16(float value) {
    const std::uint32_t bits = floatAsU32(value);
    if (!(bits << 1))
        return 0;
    return static_cast<std::int16_t>(((((bits - 0x38000000U) << 4) >> 1) | (bits & 0x80000000U)) >> 16);
}
} // namespace motion_detail

// @verified - d
MotnFileNode *MOTN_FindFileForAnimId(std::uint16_t animationId) {
#if SHENMUE_IS_S2
    MotnFileNode *node = g_motionData;
#else
    MotnFileNode *node = g_baseMotionFile;
#endif
    while (animationId <= node->firstAnimation || animationId >= node->endAnimation) {
        node = node->next;
        if (!node)
            return 0;
    }
    return node;
}

// @verified - d
const std::byte *MOTN_GetMarkerData(std::uint16_t animationId) {
    const MotnFileNode *const file = MOTN_FindFileForAnimId(animationId);
    if (!file)
        return 0;
    const std::byte *const data = reinterpret_cast<const std::byte *>(file->data);
    const std::uint32_t index = static_cast<std::uint16_t>(animationId - 1U) - static_cast<std::uint32_t>(file->firstAnimation);
    const std::uint32_t tableOffset = *reinterpret_cast<const std::uint32_t *>(data);
    const std::uint32_t offset = *reinterpret_cast<const std::uint32_t *>(data + tableOffset + index * 8U + 4);
    const std::uintptr_t result = reinterpret_cast<std::uintptr_t>(data) + offset;
    return result ? reinterpret_cast<const std::byte *>(result) : s1::kEmptyMotionMarkerData;
}

// @verified - d
const MotnAnimationHeader *MOTN_GetDataForAnimId(std::uint16_t animationId) {
    const MotnFileNode *const file = MOTN_FindFileForAnimId(animationId);
    if (!file)
        return 0;
    const std::byte *const data = reinterpret_cast<const std::byte *>(file->data);
    const std::uint32_t index = static_cast<std::uint16_t>(animationId - 1U) - static_cast<std::uint32_t>(file->firstAnimation);
    const std::uint32_t tableOffset = *reinterpret_cast<const std::uint32_t *>(data);
    const std::uint32_t dataOffset = *reinterpret_cast<const std::uint32_t *>(data + 8);
    const std::uint32_t offset = *reinterpret_cast<const std::uint32_t *>(data + tableOffset + index * 8U);
    return reinterpret_cast<const MotnAnimationHeader *>(data + offset + dataOffset);
}
}
} // namespace shenmue

#endif
