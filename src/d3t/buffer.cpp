#include "d3t/buffer.hpp"

#include <array>
#include <atomic>
#include <cstring>
#include <new>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#endif

namespace shenmue::d3t {

void *g_D3DDevice = nullptr;

namespace DX11 {

D3TDeviceContext *g_ppD3DDeviceContext = nullptr;
std::int32_t g_tmp_buffer_handle = 0;

} // namespace DX11

void *GetD3DDeviceContext(void *renderer) {
    if (renderer == nullptr) {
        return nullptr;
    }
    return static_cast<D3TDeviceContext *>(renderer)->deviceContext;
}

namespace {

constexpr std::uint32_t kBufferIndexMask = 0x7fffU;
constexpr std::uint32_t kBufferTagMask = 0xffff8000U;
constexpr std::uint32_t kBufferOccupied = 0x80000000U;
constexpr std::size_t kBufferCount = 0x8000;

struct BufferStorage {
    std::array<BufferResource, kBufferCount> buffers{};
    std::array<std::uint32_t, kBufferCount> tags{};
    std::uint32_t last = 0;
};

BufferStorage g_buffers;
std::atomic<std::uint32_t> g_bufferReaders{0};
std::atomic<std::uint8_t> g_bufferCreationLock{0};

struct SceneBufferEntry {
    std::int32_t handle;
    std::int32_t retained;
    std::int32_t flags;
    std::int32_t size;
};
static_assert(sizeof(SceneBufferEntry) == 0x10);
static_assert(offsetof(SceneBufferEntry, retained) == 4);
std::array<SceneBufferEntry, 4000> sceneVertexBuffers{};
std::array<SceneBufferEntry, 4000> sceneIndexBuffers{};
std::int32_t sceneBufferCount{};
std::int32_t sceneBufferBytes{};
std::int32_t sceneBufferPeak{};

class BufferReadLock {
  public:
    BufferReadLock() {
        g_bufferReaders.fetch_add(1, std::memory_order_acquire);
        while (g_bufferCreationLock.load(std::memory_order_acquire)) {
        }
    }

    ~BufferReadLock() {
        g_bufferReaders.fetch_sub(1, std::memory_order_release);
    }
};

class BufferWriteLock {
  public:
    BufferWriteLock() {
        for (;;) {
            while (g_bufferReaders.load(std::memory_order_acquire)) {
            }
            std::uint8_t expected = 0;
            if (!g_bufferCreationLock.compare_exchange_strong(expected, 1, std::memory_order_acquire)) {
                continue;
            }
            if (!g_bufferReaders.load(std::memory_order_acquire)) {
                break;
            }
            g_bufferCreationLock.store(0, std::memory_order_release);
        }
    }

    ~BufferWriteLock() {
        g_bufferCreationLock.store(0, std::memory_order_release);
    }
};

BufferResource *GetSceneBuffer(std::int32_t handle) {
    const std::uint32_t value = static_cast<std::uint32_t>(handle);
    const std::size_t index = value & kBufferIndexMask;
    if (g_buffers.tags[index] != (value & kBufferTagMask)) {
        return nullptr;
    }
    return &g_buffers.buffers[index];
}

void *AllocateRawBuffer(std::size_t size) {
    return ::operator new (size, std::align_val_t{16}, std::nothrow);
}

void FreeRawBuffer(void *buffer) {
    ::operator delete (buffer, std::align_val_t{16});
}

void ReleaseBufferResources(BufferResource &slot) {
#if defined(_WIN32)
    if (slot.shaderResourceView) {
        static_cast<ID3D11ShaderResourceView *>(slot.shaderResourceView)->Release();
        slot.shaderResourceView = nullptr;
    }
    if (slot.gpuBuffer) {
        static_cast<ID3D11Buffer *>(slot.gpuBuffer)->Release();
        slot.gpuBuffer = nullptr;
    }
#else
    slot.shaderResourceView = nullptr;
    slot.gpuBuffer = nullptr;
#endif
    if (slot.rawData) {
        FreeRawBuffer(slot.rawData);
        slot.rawData = reinterpret_cast<void *>(std::uintptr_t{0xfefefefefefefefdULL});
    }
}

#if defined(_WIN32)
void CreateD3D11Buffer(const BufferDescription &description, BufferResource &slot) {
    auto *const device = static_cast<ID3D11Device *>(g_D3DDevice);

    D3D11_BUFFER_DESC nativeDescription{};
    nativeDescription.ByteWidth = static_cast<UINT>(description.size);
    if (description.flags3 & 2) {
        nativeDescription.Usage = D3D11_USAGE_DYNAMIC;
        nativeDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }

    switch (description.flags) {
    case 0:
        nativeDescription.ByteWidth = (nativeDescription.ByteWidth + 15U) & ~15U;
        nativeDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        break;
    case 1:
        nativeDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        break;
    case 2:
        nativeDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        break;
    case 3:
        nativeDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        nativeDescription.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        nativeDescription.StructureByteStride = static_cast<UINT>(description.count);
        break;
    case 4:
        nativeDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        nativeDescription.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        nativeDescription.StructureByteStride = static_cast<UINT>(description.count);
        break;
    default:
        break;
    }

    D3D11_SUBRESOURCE_DATA initialData{};
    initialData.pSysMem = description.buffer;
    const D3D11_SUBRESOURCE_DATA *const initialDataPointer = description.buffer ? &initialData : nullptr;
    const HRESULT createResult = device->CreateBuffer(&nativeDescription, initialDataPointer, reinterpret_cast<ID3D11Buffer **>(&slot.gpuBuffer));
    if (FAILED(createResult)) {
        device->GetDeviceRemovedReason();
    }

    if (description.flags == 4 && description.count != 0) {
        D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedView{};
        unorderedView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        unorderedView.Buffer.NumElements = static_cast<UINT>(description.size / description.count);
        device->CreateUnorderedAccessView(
            static_cast<ID3D11Buffer *>(slot.gpuBuffer), &unorderedView, reinterpret_cast<ID3D11UnorderedAccessView **>(&slot.unorderedAccessView));
    } else if (nativeDescription.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
        device->CreateShaderResourceView(
            static_cast<ID3D11Buffer *>(slot.gpuBuffer), nullptr, reinterpret_cast<ID3D11ShaderResourceView **>(&slot.shaderResourceView));
    }
}
#else
void CreateD3D11Buffer(const BufferDescription &, BufferResource &) {
}
#endif

} // namespace

std::int32_t CreateBuffer(const BufferDescription *description) {

    const BufferWriteLock lock;
    const std::uint32_t first = g_buffers.last;
    std::uint32_t index = first;
    while (g_buffers.tags[index] & kBufferOccupied) {
        index = (index + 1) & kBufferIndexMask;
        if (index == first) {
            return 0;
        }
    }

    std::uint32_t generation = (g_buffers.tags[index] >> 15) + 1;
    if ((generation & 0xffff0000U) == 0x00010000U) {
        generation = 1;
    }
    const std::uint32_t tag = ((generation | 0xffff0000U) << 15) & kBufferTagMask;
    const std::uint32_t handle = tag | index;
    g_buffers.tags[index] = tag;
    g_buffers.last = (index + 1) & kBufferIndexMask;

    BufferResource &slot = g_buffers.buffers[index];
    slot.gpuBuffer = nullptr;
    slot.shaderResourceView = nullptr;
    slot.unorderedAccessView = nullptr;
    slot.description = *description;
    slot.sizeInBytes = description->size;
    slot.referenceCount.store(1, std::memory_order_release);
    slot.rawData = nullptr;

    if (description->flags3 & 1) {
        slot.rawData = AllocateRawBuffer(static_cast<std::size_t>(description->size));
        if (slot.rawData && description->buffer) {
            std::memcpy(slot.rawData, description->buffer, static_cast<std::size_t>(description->size));
        }
    }
    CreateD3D11Buffer(*description, slot);
    return static_cast<std::int32_t>(handle);
}

std::int64_t ValidateAndCommitBuffer(std::int32_t bufferHandle) {
    const BufferWriteLock lock;
    BufferResource *const slot = GetSceneBuffer(bufferHandle);
    if (!slot) {
        return static_cast<std::uint32_t>(bufferHandle) & kBufferTagMask;
    }

    const std::int32_t previous = slot->referenceCount.fetch_sub(1, std::memory_order_acq_rel);
    if (previous == 1) {
        ReleaseBufferResources(*slot);
        const std::size_t index = static_cast<std::uint32_t>(bufferHandle) & kBufferIndexMask;
        const std::uint32_t tag = static_cast<std::uint32_t>(bufferHandle) & kBufferTagMask;
        if (g_buffers.tags[index] == tag) {
            g_buffers.tags[index] &= ~kBufferOccupied;
        }
        return tag;
    }
    return previous;
}

bool CreateBuffer_Wrapper(std::int32_t size, std::int32_t *handle, const void *data) {
    if (!handle)
        return false;
    for (SceneBufferEntry &entry : sceneVertexBuffers) {
        if (entry.handle)
            continue;
        const BufferDescription description{1, size, 1, 3, data, nullptr};
        *handle = CreateBuffer(&description);
        if (!*handle)
            return false;
        entry.handle = *handle;
        ++sceneBufferCount;
        sceneBufferBytes += size;
        if (sceneBufferBytes > sceneBufferPeak)
            sceneBufferPeak = sceneBufferBytes;
        return true;
    }
    return false;
}

void CreateFixedSizeBuffer(std::int32_t size, std::int32_t *handle, std::int32_t retained) {
    if (!handle)
        return;
    for (SceneBufferEntry &entry : sceneVertexBuffers) {
        if (entry.handle)
            continue;
        const BufferDescription description{1, size, 1, 3, nullptr, nullptr};
        *handle = CreateBuffer(&description);
        if (!*handle)
            return;
        entry.handle = *handle;
        entry.retained = retained;
        ++sceneBufferCount;
        sceneBufferBytes += size;
        if (sceneBufferBytes > sceneBufferPeak)
            sceneBufferPeak = sceneBufferBytes;
        return;
    }
}

bool ReleaseBufferByHandle(const std::int32_t *handle) {
    if (!handle)
        return false;
    SceneBufferEntry *found = nullptr;
    for (SceneBufferEntry &entry : sceneVertexBuffers) {
        if (entry.handle == *handle) {
            found = &entry;
            break;
        }
    }
    const std::int32_t size = GetSizeForModelBuffer(*handle);
    ValidateAndCommitBuffer(*handle);
    if (!found)
        return false;
    found->handle = 0;
    --sceneBufferCount;
    sceneBufferBytes -= size;
    if (sceneBufferBytes > sceneBufferPeak)
        sceneBufferPeak = sceneBufferBytes;
    return true;
}

bool CreateSceneIndexBuffer(std::int32_t count, std::int32_t *handle) {
    for (std::size_t index = sceneIndexBuffers.size(); index != 0; --index) {
        SceneBufferEntry &entry = sceneIndexBuffers[index - 1];
        if (entry.handle)
            continue;
        const BufferDescription description{2, count * 2, 0, 3, nullptr, nullptr};
        *handle = CreateBuffer(&description);
        entry.handle = *handle;
        return true;
    }
    return false;
}

void ReleaseSceneIndexBuffer(std::int32_t handle) {
    if (!handle)
        return;
    for (SceneBufferEntry &entry : sceneIndexBuffers) {
        if (entry.handle != handle)
            continue;
        ValidateAndCommitBuffer(handle);
        entry.handle = 0;
        return;
    }
    ValidateAndCommitBuffer(handle);
}

std::int64_t ResetSceneData() {
    for (std::size_t index = sceneVertexBuffers.size(); index != 0; --index) {
        SceneBufferEntry &entry = sceneVertexBuffers[index - 1];
        if (entry.handle && !entry.retained) {
            ValidateAndCommitBuffer(entry.handle);
            entry.handle = 0;
        }
    }
    for (std::size_t index = sceneIndexBuffers.size(); index != 0; --index) {
        SceneBufferEntry &entry = sceneIndexBuffers[index - 1];
        if (entry.handle) {
            ValidateAndCommitBuffer(entry.handle);
            entry.handle = 0;
        }
    }
    for (std::size_t index = sceneVertexBuffers.size(); index != 0; --index) {
        SceneBufferEntry &entry = sceneVertexBuffers[index - 1];
        if (entry.retained)
            continue;
        if (!entry.flags)
            sceneBufferBytes -= entry.size;
        entry = {};
    }
    if (sceneBufferBytes > sceneBufferPeak)
        sceneBufferPeak = sceneBufferBytes;
    sceneBufferCount = 0;
    return 1;
}

std::int32_t GetSizeForModelBuffer(std::int32_t bufferHandle) {
    const BufferReadLock lock;
    BufferResource *const slot = GetSceneBuffer(bufferHandle);
    return slot ? slot->sizeInBytes : 0;
}

bool GetBufferBackendView(std::int32_t bufferHandle, D3TBufferBackendView *view) {
    if (!view) {
        return false;
    }
    BufferReadLock lock;
    const BufferResource *const slot = GetSceneBuffer(bufferHandle);
    if (!slot) {
        *view = {};
        return false;
    }
    view->buffer = slot->gpuBuffer;
    view->shaderResourceView = slot->shaderResourceView;
    view->unorderedAccessView = slot->unorderedAccessView;
    view->description = slot->description;
    view->sizeInBytes = slot->sizeInBytes;
    return true;
}

const void *GetBufferCpuData(std::int32_t bufferHandle, std::int32_t *sizeInBytes) {
    BufferReadLock lock;
    const BufferResource *const slot = GetSceneBuffer(bufferHandle);
    if (sizeInBytes)
        *sizeInBytes = slot ? slot->sizeInBytes : 0;
    return slot ? slot->rawData : nullptr;
}

void *GetWriteMappedBufferPointer(void *renderer, std::int32_t bufferHandle, std::int32_t mode, std::uint32_t offsetIntoBuffer) {
    const BufferReadLock lock;
    BufferResource *const slot = GetSceneBuffer(bufferHandle);
    if (slot->rawData) {
        return static_cast<std::byte *>(slot->rawData) + offsetIntoBuffer;
    }
#if defined(_WIN32)
    auto *const context = static_cast<ID3D11DeviceContext *>(GetD3DDeviceContext(renderer));
    auto *const buffer = static_cast<ID3D11Buffer *>(slot->gpuBuffer);
    D3D11_MAPPED_SUBRESOURCE mapped{};
    const D3D11_MAP mapType = mode == 1 ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
    context->Map(buffer, 0, mapType, 0, &mapped);
    return static_cast<std::byte *>(mapped.pData) + offsetIntoBuffer;
#else
    static_cast<void>(renderer);
    static_cast<void>(mode);
    return static_cast<std::byte *>(slot->rawData) + offsetIntoBuffer;
#endif
}

std::int64_t DX11_CopySceneNodeToGPU(void *renderer, std::int32_t bufferHandle) {
    BufferResource *const slot = GetSceneBuffer(bufferHandle);
#if defined(_WIN32)
    auto *const context = static_cast<ID3D11DeviceContext *>(GetD3DDeviceContext(renderer));
    auto *const buffer = static_cast<ID3D11Buffer *>(slot->gpuBuffer);
    if (slot->rawData) {
        D3D11_MAPPED_SUBRESOURCE mapped{};
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        std::memcpy(mapped.pData, slot->rawData, static_cast<std::size_t>(slot->sizeInBytes));
    }
    context->Unmap(buffer, 0);
#else
    static_cast<void>(renderer);
#endif
    return 0;
}

std::int64_t FreeBuffers() {
    for (std::size_t index = 0; index < kBufferCount; ++index) {
        if (g_buffers.tags[index] & kBufferOccupied) {
            ReleaseBufferResources(g_buffers.buffers[index]);
        }
    }
    return static_cast<std::int64_t>(kBufferCount);
}

} // namespace shenmue::d3t
