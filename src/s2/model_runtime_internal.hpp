#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2::model_component_detail {

#pragma pack(push, 4)
struct RuntimeVertexOwner {
    std::uint32_t cacheHandle, reserved04;
    MT7Node *node;
    std::uint32_t rootHash, vertexCount, enabled, ready, slot;
};
#pragma pack(pop)

#pragma pack(push, 4)
struct RuntimeClothOwner {
    RuntimeVertexOwner owner;
    std::uint32_t hashes[181];
};
#pragma pack(pop)

#pragma pack(push, 4)
struct RuntimeDynamicOwner {
    RuntimeVertexOwner owner;
    std::uint32_t hashes[129];
};
#pragma pack(pop)

#pragma pack(push, 4)
struct RuntimeMatrixEvent {
    std::uint32_t enabled, ready, cacheHandle, nodeHash, headHash;
    Matrix4x4 relative;
    std::uint32_t reserved54;
};
#pragma pack(pop)

static_assert(sizeof(RuntimeVertexOwner) == 36);

static_assert(sizeof(RuntimeClothOwner) == 760);

static_assert(sizeof(RuntimeDynamicOwner) == 552);

static_assert(sizeof(RuntimeMatrixEvent) == 88);

} // namespace shenmue::s2::model_component_detail
