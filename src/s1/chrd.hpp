#pragma once

#include <array>
#include <cstdint>

namespace shenmue::s1 {


struct ShadowModelCacheEntry {
    void *model;
    char name[16];
};
static_assert(sizeof(ShadowModelCacheEntry) == 24);

extern std::array<ShadowModelCacheEntry, 10> g_shadowModelCache;
extern std::int32_t g_shadowModelCacheCount;

void CHRD_Init(void *data, std::uint32_t entryPoint, std::int32_t allocationToken);
void CHRD_InitMapInfoChunk(void *data, std::uint32_t size, std::uint32_t entryPoint, std::int32_t allocationToken);
void CHRD_InitCurrentMapInfo();

} // namespace shenmue::s1
