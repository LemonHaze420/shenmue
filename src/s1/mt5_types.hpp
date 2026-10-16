#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {


#pragma pack(push, 1)
struct Mt5RuntimeNode {
    std::uint16_t nodeKind;
    std::uint16_t nodeFlags;
    std::int32_t sceneBindingOffset;
    Vector3i rotation;
    Vector3 scale;
    Vector3 position;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t parentOffset;
    std::int32_t transformOffset;
    std::int32_t attachmentOffset;
};
#pragma pack(pop)

static_assert(sizeof(Mt5RuntimeNode) == 0x40);
static_assert(offsetof(Mt5RuntimeNode, sceneBindingOffset) == 0x04);
static_assert(offsetof(Mt5RuntimeNode, rotation) == 0x08);
static_assert(offsetof(Mt5RuntimeNode, scale) == 0x14);
static_assert(offsetof(Mt5RuntimeNode, position) == 0x20);
static_assert(offsetof(Mt5RuntimeNode, childOffset) == 0x2c);
static_assert(offsetof(Mt5RuntimeNode, siblingOffset) == 0x30);
static_assert(offsetof(Mt5RuntimeNode, parentOffset) == 0x34);
static_assert(offsetof(Mt5RuntimeNode, transformOffset) == 0x38);
static_assert(offsetof(Mt5RuntimeNode, attachmentOffset) == 0x3c);

} // namespace shenmue::s1
