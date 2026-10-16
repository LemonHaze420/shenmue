#pragma once

#include "d3t/render_state.hpp"

#include <cstddef>
#include <cstdint>
#include <array>
#include <atomic>

namespace shenmue::d3t {

struct BufferDescription {
    std::int32_t flags;
    std::int32_t size;
    std::int32_t count;
    std::int32_t flags3;
    const void *buffer;
    const char *name;
};

static_assert(sizeof(BufferDescription) == 0x20);

std::int32_t CreateBuffer(const BufferDescription *description);
std::int64_t ValidateAndCommitBuffer(std::int32_t bufferHandle);
std::int32_t GetSizeForModelBuffer(std::int32_t bufferHandle);
void *GetWriteMappedBufferPointer(void *renderer, std::int32_t bufferHandle, std::int32_t mode, std::uint32_t offsetIntoBuffer);
std::int64_t DX11_CopySceneNodeToGPU(void *renderer, std::int32_t bufferHandle);
std::int64_t FreeBuffers();




bool CreateBuffer_Wrapper(std::int32_t size, std::int32_t *handle, const void *data);
void CreateFixedSizeBuffer(std::int32_t size, std::int32_t *handle, std::int32_t retained);
bool ReleaseBufferByHandle(const std::int32_t *handle);
bool CreateSceneIndexBuffer(std::int32_t count, std::int32_t *handle);
void ReleaseSceneIndexBuffer(std::int32_t handle);
std::int64_t ResetSceneData();

struct D3TBufferBackendView {
    void *buffer;
    void *shaderResourceView;
    void *unorderedAccessView;
    BufferDescription description;
    std::int32_t sizeInBytes;
};



struct BufferResource {
    void *gpuBuffer;
    void *shaderResourceView;
    void *unorderedAccessView;
    BufferDescription description;
    std::int32_t sizeInBytes;
    std::atomic<std::int32_t> referenceCount;
    void *rawData;
};

static_assert(sizeof(BufferResource) == 0x48);
static_assert(offsetof(BufferResource, description) == 0x18);
static_assert(offsetof(BufferResource, sizeInBytes) == 0x38);
static_assert(offsetof(BufferResource, rawData) == 0x40);

bool GetBufferBackendView(std::int32_t bufferHandle, D3TBufferBackendView *view);
const void *GetBufferCpuData(std::int32_t bufferHandle, std::int32_t *sizeInBytes);

extern void *g_D3DDevice;

struct D3TDeviceContext {
    void *deviceContext;
    void *deferredDeviceContext;
    void *commandList;
    void *userAnnotation;
    void *deferredUserAnnotation;
    std::array<std::byte, 0xac8> renderState;
    std::array<std::uintptr_t, 42> resourceCache;
    std::array<std::byte, 0x1a8> remainingResourceState;
    std::int32_t trailingState[4];
};

inline D3TRenderState &GetRenderState(D3TDeviceContext &context) {
    return *reinterpret_cast<D3TRenderState *>(context.renderState.data());
}

inline const D3TRenderState &GetRenderState(const D3TDeviceContext &context) {
    return *reinterpret_cast<const D3TRenderState *>(context.renderState.data());
}

static_assert(offsetof(D3TDeviceContext, renderState) == 0x28);
static_assert(offsetof(D3TDeviceContext, resourceCache) == 0xaf0);
static_assert(offsetof(D3TDeviceContext, trailingState) == 0xde8);
static_assert(sizeof(D3TDeviceContext) == 0xdf8);

void *GetD3DDeviceContext(void *renderer);

namespace DX11 {

extern D3TDeviceContext *g_ppD3DDeviceContext;
extern std::int32_t g_tmp_buffer_handle;

} // namespace DX11

} // namespace shenmue::d3t
