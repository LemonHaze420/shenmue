#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {


#pragma pack(push, 1)
struct FaceModelCacheSlot {
    void *modelRoot;
    void *modelOwner;
    char filename[0x10];
    std::int16_t state;
    std::byte reserved22[6];
};
#pragma pack(pop)

static_assert(sizeof(FaceModelCacheSlot) == 0x28);
static_assert(offsetof(FaceModelCacheSlot, filename) == 0x10);
static_assert(offsetof(FaceModelCacheSlot, state) == 0x20);

extern FaceModelCacheSlot g_faceModelCache[8];

} // namespace shenmue::s1
