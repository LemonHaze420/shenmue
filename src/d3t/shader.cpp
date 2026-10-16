#include "d3t/shader.hpp"

#include "d3t/buffer.hpp"
#include "d3t/fvf.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <new>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include <dxgi.h>
#endif

namespace shenmue::d3t {
namespace {

struct ShaderResource {
    void *shader;
    std::int32_t type;
    std::atomic<std::int32_t> referenceCount;
    ShaderCleanup cleanup;
    void *cleanupContext;
};

struct VertexShaderResource {
    ShaderResource base;
    void *bytecode;
    std::uint64_t bytecodeLength;
    std::array<void *, 64> inputLayouts;
};

static_assert(sizeof(ShaderResource) == 0x20);
static_assert(sizeof(VertexShaderResource) == 0x230);
static_assert(offsetof(VertexShaderResource, bytecode) == 0x20);
static_assert(offsetof(VertexShaderResource, inputLayouts) == 0x30);

std::array<ShaderResource *, 0x1000> shaders{};
std::array<std::uint32_t, 0x1000> shaderTags{};
std::uint32_t nextShaderIndex = 0;
std::atomic<std::int32_t> shaderReaders{0};
std::atomic<std::uint8_t> shaderWriter{0};
std::atomic_flag inputLayoutLock = ATOMIC_FLAG_INIT;
std::array<std::uint32_t, 64 * 64> fvfOffsets{};
std::array<std::uint32_t, 64 * 64> fvfStrides{};

class ShaderReadGuard {
  public:
    ShaderReadGuard() {
        shaderReaders.fetch_add(1, std::memory_order_seq_cst);
        while (shaderWriter.load(std::memory_order_seq_cst) != 0) {
        }
    }
    ~ShaderReadGuard() {
        shaderReaders.fetch_sub(1, std::memory_order_seq_cst);
    }
};

class ShaderWriteGuard {
  public:
    ShaderWriteGuard() {
        for (;;) {
            while (shaderReaders.load(std::memory_order_seq_cst) != 0) {
            }
            std::uint8_t expected = 0;
            if (!shaderWriter.compare_exchange_strong(expected, 1, std::memory_order_seq_cst)) {
                continue;
            }
            if (shaderReaders.load(std::memory_order_seq_cst) == 0)
                break;
            shaderWriter.store(0, std::memory_order_seq_cst);
        }
    }
    ~ShaderWriteGuard() {
        shaderWriter.store(0, std::memory_order_seq_cst);
    }
};

ShaderResource **GetShader(std::uint32_t handle) {
    const std::uint32_t index = handle & 0xfffU;
    return shaderTags[index] == (handle & 0xfffff000U) ? &shaders[index] : nullptr;
}

void ResetShader(ShaderResource *resource, bool destroy) {
    if (!resource)
        return;
    if (resource->type == static_cast<std::int32_t>(ShaderType::Vertex)) {
        auto *const vertex = reinterpret_cast<VertexShaderResource *>(resource);
#if defined(_WIN32)
        for (void *&layout : vertex->inputLayouts) {
            if (layout) {
                static_cast<ID3D11InputLayout *>(layout)->Release();
                layout = nullptr;
            }
        }
        if (resource->shader) {
            static_cast<ID3D11VertexShader *>(resource->shader)->Release();
        }
#else
        vertex->inputLayouts.fill(nullptr);
#endif
        std::free(vertex->bytecode);
        vertex->bytecode = nullptr;
        vertex->bytecodeLength = 0;
    } else if (resource->type == static_cast<std::int32_t>(ShaderType::Pixel) || resource->type == static_cast<std::int32_t>(ShaderType::Compute)) {
#if defined(_WIN32)
        if (resource->shader) {
            static_cast<ID3D11DeviceChild *>(resource->shader)->Release();
        }
#endif
    }
    resource->shader = nullptr;
    if (destroy && resource->cleanup)
        resource->cleanup(resource->cleanupContext);
}

} // namespace

std::uint32_t CreateShader(std::int32_t type) {
    ShaderWriteGuard guard;
    const std::uint32_t first = nextShaderIndex;
    std::uint32_t index = first;
    do {
        std::uint32_t generation = shaderTags[index] >> 12U;
        if (generation < 0x80000U) {
            ++generation;
            if ((generation & 0xfff80000U) == 0x80000U)
                generation = 1;
            const std::uint32_t tag = (generation | 0xfff80000U) << 12U;
            shaderTags[index] = tag;
            nextShaderIndex = (index + 1U) & 0xfffU;
            const std::size_t size = type == 0 ? sizeof(VertexShaderResource) : sizeof(ShaderResource);
            auto *const resource = static_cast<ShaderResource *>(std::calloc(1, size));
            if (!resource)
                throw std::bad_alloc{};
            shaders[index] = resource;
            resource->type = type;
            resource->referenceCount.store(1, std::memory_order_seq_cst);
            return tag | index;
        }
        index = (index + 1U) & 0xfffU;
    } while (index != first);
    return 0;
}

bool RetainShader(std::uint32_t shader) {
    ShaderReadGuard guard;
    ShaderResource **const slot = GetShader(shader);
    if (!slot || !*slot)
        return false;
    (*slot)->referenceCount.fetch_add(1, std::memory_order_seq_cst);
    return true;
}

void DestroyShader(std::uint32_t shader) {
    ShaderWriteGuard guard;
    ShaderResource **const slot = GetShader(shader);
    if (!slot || !*slot)
        return;
    if ((*slot)->referenceCount.fetch_sub(1, std::memory_order_seq_cst) == 1) {
        ResetShader(*slot, true);
        const std::uint32_t index = shader & 0xfffU;
        if (shaderTags[index] == (shader & 0xfffff000U)) {
            shaderTags[index] &= 0x7fffffffU;
        }
    }
}

void DestroyAllShaders() {
    ShaderWriteGuard guard;
    for (std::uint32_t index = 0; index < shaderTags.size(); ++index) {
        if (shaderTags[index] >= 0x80000000U && shaders[index]) {
            ResetShader(shaders[index], true);
        }
    }
}

bool CreateShaderResource(std::uint32_t shader, const void *bytecode, std::uint64_t bytecodeLength, ShaderType type) {
    if (!shader)
        return false;
    ShaderReadGuard guard;
    ShaderResource **const slot = GetShader(shader);
    if (!slot || !*slot)
        return false;
    ShaderResource *const resource = *slot;
    ResetShader(resource, false);
    resource->type = static_cast<std::int32_t>(type);

    bool succeeded = true;
#if defined(_WIN32)
    auto *const device = static_cast<ID3D11Device *>(g_D3DDevice);
    if (!device)
        return false;
    HRESULT result = E_INVALIDARG;
    if (type == ShaderType::Vertex) {
        auto *const vertex = reinterpret_cast<VertexShaderResource *>(resource);
        vertex->bytecode = const_cast<void *>(bytecode);
        vertex->bytecodeLength = bytecodeLength;
        result = device->CreateVertexShader(bytecode, static_cast<SIZE_T>(bytecodeLength), nullptr, reinterpret_cast<ID3D11VertexShader **>(&resource->shader));
    } else if (type == ShaderType::Pixel) {
        result = device->CreatePixelShader(bytecode, static_cast<SIZE_T>(bytecodeLength), nullptr, reinterpret_cast<ID3D11PixelShader **>(&resource->shader));
    } else {
        result =
            device->CreateComputeShader(bytecode, static_cast<SIZE_T>(bytecodeLength), nullptr, reinterpret_cast<ID3D11ComputeShader **>(&resource->shader));
    }
    succeeded = SUCCEEDED(result);
    device->GetDeviceRemovedReason();
#else
    if (type == ShaderType::Vertex) {
        auto *const vertex = reinterpret_cast<VertexShaderResource *>(resource);
        vertex->bytecode = const_cast<void *>(bytecode);
        vertex->bytecodeLength = bytecodeLength;
    }
#endif
    return succeeded;
}

void SetShaderCleanup(std::uint32_t shader, ShaderCleanup cleanup, void *context) {
    ShaderReadGuard guard;
    if (ShaderResource **const slot = GetShader(shader); slot && *slot) {
        (*slot)->cleanup = cleanup;
        (*slot)->cleanupContext = context;
    }
}

void *GetInputLayout(std::uint32_t vertexShader, std::uint32_t fvfHandle) {
    if (!vertexShader || !fvfHandle)
        return nullptr;
    ShaderReadGuard shaderGuard;
    ShaderResource **const slot = GetShader(vertexShader);
    if (!slot || !*slot || (*slot)->type != static_cast<std::int32_t>(ShaderType::Vertex)) {
        return nullptr;
    }
    auto *const vertex = reinterpret_cast<VertexShaderResource *>(*slot);
    if (!vertex->base.shader)
        return nullptr;

    std::uint32_t fvfIndex = 0;
    const FVFDescription *const description = GetFVFDescription(fvfHandle, &fvfIndex);
    if (!description)
        return nullptr;
    if (vertex->inputLayouts[fvfIndex])
        return vertex->inputLayouts[fvfIndex];

    while (inputLayoutLock.test_and_set(std::memory_order_acquire)) {
    }
    if (!vertex->inputLayouts[fvfIndex]) {
        std::array<std::uint32_t, 32> slotOffsets{};
        std::array<std::uint32_t, 32> slotStrides{};
#if defined(_WIN32)
        std::array<D3D11_INPUT_ELEMENT_DESC, 32> native{};
#endif
        for (std::uint32_t i = 0; i < description->elementCount; ++i) {
            const FVFElement &element = description->elements[i];
            const std::uint32_t inputSlot = element.inputSlot;
            const std::uint32_t offset = slotOffsets[inputSlot];
            fvfOffsets[fvfIndex * 64U + i] = offset;
            std::uint32_t formatStride = 0;
            switch (element.format) {
            case 1:
            case 2:
            case 6:
            case 16:
            case 17:
            case 18:
            case 37:
            case 38:
            case 39:
            case 40:
                formatStride = 4;
                break;
            case 7:
            case 20:
                formatStride = 8;
                break;
            case 21:
                formatStride = 12;
                break;
            case 22:
                formatStride = 16;
                break;
            default:
                break;
            }
            slotOffsets[inputSlot] += formatStride + element.extraStride;
            slotStrides[inputSlot] += formatStride + element.extraStride;
#if defined(_WIN32)
            constexpr std::array<DXGI_FORMAT, 45> formats{DXGI_FORMAT_UNKNOWN, static_cast<DXGI_FORMAT>(31), static_cast<DXGI_FORMAT>(28),
                static_cast<DXGI_FORMAT>(29), static_cast<DXGI_FORMAT>(28), static_cast<DXGI_FORMAT>(29), static_cast<DXGI_FORMAT>(26),
                static_cast<DXGI_FORMAT>(12), static_cast<DXGI_FORMAT>(10), static_cast<DXGI_FORMAT>(61), static_cast<DXGI_FORMAT>(49),
                static_cast<DXGI_FORMAT>(57), static_cast<DXGI_FORMAT>(56), static_cast<DXGI_FORMAT>(54), static_cast<DXGI_FORMAT>(35),
                static_cast<DXGI_FORMAT>(34), static_cast<DXGI_FORMAT>(42), static_cast<DXGI_FORMAT>(41), static_cast<DXGI_FORMAT>(40),
                static_cast<DXGI_FORMAT>(45), static_cast<DXGI_FORMAT>(16), static_cast<DXGI_FORMAT>(6), static_cast<DXGI_FORMAT>(2),
                static_cast<DXGI_FORMAT>(71), static_cast<DXGI_FORMAT>(74), static_cast<DXGI_FORMAT>(77), static_cast<DXGI_FORMAT>(80),
                static_cast<DXGI_FORMAT>(83), static_cast<DXGI_FORMAT>(72), static_cast<DXGI_FORMAT>(75), static_cast<DXGI_FORMAT>(78),
                static_cast<DXGI_FORMAT>(85), static_cast<DXGI_FORMAT>(85), static_cast<DXGI_FORMAT>(86), static_cast<DXGI_FORMAT>(86),
                static_cast<DXGI_FORMAT>(115), static_cast<DXGI_FORMAT>(115), static_cast<DXGI_FORMAT>(87), static_cast<DXGI_FORMAT>(91),
                static_cast<DXGI_FORMAT>(88), static_cast<DXGI_FORMAT>(93), static_cast<DXGI_FORMAT>(20), static_cast<DXGI_FORMAT>(24),
                static_cast<DXGI_FORMAT>(65), DXGI_FORMAT_UNKNOWN};
            D3D11_INPUT_ELEMENT_DESC &output = native[i];
            output.SemanticName = element.semanticName;
            output.SemanticIndex = element.semanticIndex;
            output.Format = formats[element.format];
            output.InputSlot = inputSlot;
            output.AlignedByteOffset = offset;
            output.InputSlotClass = element.inputClassification == 0 ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
            output.InstanceDataStepRate = element.inputClassification == 1 ? 1U : 0U;
#endif
        }
        for (std::uint32_t i = 0; i < description->elementCount; ++i) {
            fvfStrides[fvfIndex * 64U + i] = slotStrides[description->elements[i].inputSlot];
        }
#if defined(_WIN32)
        auto *const device = static_cast<ID3D11Device *>(g_D3DDevice);
        if (device && vertex->bytecode && description->elementCount != 0) {
            device->CreateInputLayout(native.data(), description->elementCount, vertex->bytecode, static_cast<SIZE_T>(vertex->bytecodeLength),
                reinterpret_cast<ID3D11InputLayout **>(&vertex->inputLayouts[fvfIndex]));
        }
#endif
    }
    inputLayoutLock.clear(std::memory_order_release);
    return vertex->inputLayouts[fvfIndex];
}

void *GetNativeShader(std::uint32_t shader, ShaderType expectedType) {
    if (!shader)
        return nullptr;
    ShaderReadGuard guard;
    ShaderResource **const slot = GetShader(shader);
    if (!slot || !*slot || (*slot)->type != static_cast<std::int32_t>(expectedType)) {
        return nullptr;
    }
    return (*slot)->shader;
}

std::uint32_t GetFVFStride(std::uint32_t fvfIndex, std::uint32_t slot) {
    return fvfIndex < 64 && slot < 64 ? fvfStrides[fvfIndex * 64U + slot] : 0;
}

std::uint32_t GetFVFOffset(std::uint32_t fvfIndex, std::uint32_t slot) {
    return fvfIndex < 64 && slot < 64 ? fvfOffsets[fvfIndex * 64U + slot] : 0;
}

} // namespace shenmue::d3t
