#include "s1/scene_memory.hpp"
#include "s1/boot.hpp"
#include "s1/hlib.hpp"

#include "s1/d3t/d3t_buffer.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

namespace shenmue::s1 {

std::uint32_t g_sceneDataMemoryFlags = 0;
std::uint32_t g_originalStripShaderCapacity = 0;
std::uint32_t g_originalStripBufferCapacity = 0;
std::uint32_t g_originalStripIndexCapacity = 0;
void *g_original_strip00_shader = nullptr;
void *g_original_strip00_buffer = nullptr;
std::int32_t g_original_strip00_Buffer = 0;
std::uint32_t g_originalStrip00VertexCursor = 0;
std::uint32_t g_originalStrip03VertexCursor = 0;
std::int32_t g_originalStripPassCount = 0;
std::uint32_t *g_originalStripBlendBlock = nullptr;

void *AllocateSceneTransient(std::size_t size) {
    if (!size || !g_pSTCH || g_TotalSceneFileReadBytes < 0)
        return nullptr;
    const auto *const owner = static_cast<const HLib::TaskData *>(g_pSTCH) - 1;
    const std::int64_t capacity = owner->content_size - static_cast<std::int64_t>(sizeof(HLib::TaskData));
    if (capacity < 0 || size > static_cast<std::uint64_t>(capacity))
        return nullptr;
    const std::size_t allocationSize = (size + 7U) & ~std::size_t{3};
    if (static_cast<std::uint64_t>(g_TotalSceneFileReadBytes) + allocationSize > static_cast<std::uint64_t>(capacity))
        return nullptr;
    auto *const marker = static_cast<std::byte *>(g_pSTCH) + g_TotalSceneFileReadBytes;
    const std::uint32_t token = SHEN_NAME(USSH);
    std::memcpy(marker, &token, sizeof(token));
    g_TotalSceneFileReadBytes += static_cast<std::int64_t>(allocationSize);
    return marker + 4;
}

std::int32_t InitBuffer(std::uint32_t recordSize) {
    const BufferDescription description{1, static_cast<std::int32_t>(4000U * recordSize), 0, 3, nullptr, nullptr};
    const std::int32_t handle = CreateBuffer(&description);
    auto *record = static_cast<std::byte *>(GetWriteMappedBufferPointer(DX11::g_ppD3DDeviceContext, handle, 1, 0));
    if (!record) {
        return handle;
    }

    constexpr float initializedValue = 1.0f;
    for (std::uint32_t index = 0; index < 4000; ++index) {
        std::memcpy(record + 12, &initializedValue, sizeof(initializedValue));
        record += recordSize;
    }
    DX11_CopySceneNodeToGPU(DX11::g_ppD3DDeviceContext, handle);
    return handle;
}

void FreeSceneDataMemory() {
    if ((g_sceneDataMemoryFlags & 1U) == 0) {
        return;
    }
    ValidateAndCommitBuffer(g_original_strip00_Buffer);
    ValidateAndCommitBuffer(DX11::g_tmp_buffer_handle);
    ::operator delete(g_original_strip00_shader);
    ::operator delete(g_original_strip00_buffer);
    g_original_strip00_shader = reinterpret_cast<void *>(0x8123);
    g_original_strip00_buffer = reinterpret_cast<void *>(0x8123);
    g_sceneDataMemoryFlags = 0;
}

std::int64_t InitSceneDataMemory() {
    if ((g_sceneDataMemoryFlags & 1U) != 0) {
        FreeSceneDataMemory();
    }
    g_originalStripShaderCapacity = 4000;
    g_originalStripBufferCapacity = 4000;
    g_originalStripIndexCapacity = 8000;
    g_original_strip00_shader = ::operator new(2000U * 0x10U);
    g_original_strip00_buffer = ::operator new(2000U * 8U);
    g_original_strip00_Buffer = InitBuffer(0x14);
    DX11::g_tmp_buffer_handle = InitBuffer(0x20);
    g_sceneDataMemoryFlags |= 1U;
    return DX11::g_tmp_buffer_handle;
}

} // namespace shenmue::s1
