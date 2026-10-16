#include "d3t/dx11_renderer.hpp"
#include "logging.hpp"

#include "platform/performance_counter.hpp"
#include "d3t/pipeline_state.hpp"
#include "d3t/shader.hpp"
#include "d3t/texture.hpp"
#include "d3t/fvf.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <limits>
#include <string>
#include <thread>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <d3d11_1.h>
#include <dxgi.h>
#include <windows.h>
#include <wincodec.h>
#if defined(SHENMUE_HAS_DIRECTXTEX)
#include <DirectXTex.h>
#endif
#endif

namespace shenmue::d3t {

std::uint32_t g_originalScreenHeight = 0;
std::uint32_t g_originalScreenWidth = 0;
std::uint32_t g_defaultTextureFormat = 0;
namespace {
std::uint64_t g_presentFrameCount = 0;
}

#if defined(_WIN32)

ID3D11DeviceContext *g_immediateContext = nullptr;

namespace {

struct BufferPool {
    std::uint32_t count;
    std::uint32_t reserved_04;
    std::uint32_t current;
    std::uint32_t reserved_0c;
    ID3D11Buffer **buffers;
};

static_assert(sizeof(BufferPool) == 0x18);

struct VertexBufferSpec {
    std::uint32_t byteWidth;
    std::uint32_t count;
};

constexpr std::array<std::uint32_t, 8> kConstantBufferCounts{4096, 2048, 512, 1024, 4096, 512, 128, 128};
constexpr std::array<VertexBufferSpec, 4> kVertexBufferSpecs{{
    {128, 2048},
    {1024, 128},
    {131072, 128},
    {4194304, 32},
}};

ID3D11Device *g_device = nullptr;
IDXGISwapChain *g_swapChain = nullptr;
ID3D11Texture2D *g_backBuffer = nullptr;
ID3D11RenderTargetView *g_backBufferRenderTarget = nullptr;
ID3D11ShaderResourceView *g_backBufferShaderResource = nullptr;
ID3D11Texture2D *g_mipsAccessTexture = nullptr;
ID3D11Texture2D *g_readbackTexture = nullptr;
ID3DUserDefinedAnnotation *g_immediateAnnotation = nullptr;
std::array<ID3D11Query *, 3> g_occlusionQueries{};
std::array<D3TDeviceContext, 16> g_deviceContexts{};
std::array<std::atomic<std::uint8_t>, 16> g_deviceContextBusy{};
std::array<BufferPool, 8> g_constantBufferPools{};
std::array<BufferPool, 4> g_vertexBufferPools{};
std::uint32_t g_currentTextureHandle = 0;
std::int64_t g_previousPresentNanoseconds = 0;
std::string g_pendingFrameDump;

bool ShouldDumpPresentedFrame(std::string &filename) {
    if (!g_pendingFrameDump.empty()) {
        filename = std::move(g_pendingFrameDump);
        g_pendingFrameDump.clear();
        return true;
    }

    static const char *const dumpDirectory = std::getenv("SHEN_FRAME_DUMP_DIR");
    static const char *const requestedFrameText = std::getenv("SHEN_FRAME_DUMP_FRAME");
    static const std::uint64_t requestedFrame =
        requestedFrameText ? std::strtoull(requestedFrameText, nullptr, 10) : (std::numeric_limits<std::uint64_t>::max)();
    const bool dumpEveryFrame = dumpDirectory && *dumpDirectory;
    if (!dumpEveryFrame && g_presentFrameCount != requestedFrame) {
        return false;
    }

    const std::filesystem::path directory = dumpEveryFrame ? std::filesystem::path(dumpDirectory) : std::filesystem::current_path();
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    char leaf[64]{};
    std::snprintf(leaf, sizeof(leaf), "s1_frame_%08llu.png", static_cast<unsigned long long>(g_presentFrameCount));
    filename = (directory / leaf).string();
    return true;
}

bool WriteMappedFramePng(const std::string &filename, const D3D11_TEXTURE2D_DESC &description, const D3D11_MAPPED_SUBRESOURCE &mapped) {
#if defined(SHENMUE_HAS_DIRECTXTEX)
    DirectX::Image image{};
    image.width = description.Width;
    image.height = description.Height;
    image.format = description.Format;
    image.rowPitch = mapped.RowPitch;
    image.slicePitch = mapped.RowPitch * description.Height;
    image.pixels = static_cast<std::uint8_t *>(mapped.pData);
    const std::filesystem::path path(filename);
    // Presentation ignores back-buffer alpha, which translucent passes leave below 255; record the displayed RGB only.
    return SUCCEEDED(DirectX::SaveToWICFile(image, DirectX::WIC_FLAGS_NONE, GUID_ContainerFormatPng, path.c_str(), &GUID_WICPixelFormat24bppBGR));
#else
    static_cast<void>(filename);
    static_cast<void>(description);
    static_cast<void>(mapped);
    return false;
#endif
}

std::int64_t PresentClockNanoseconds() {
    const std::int64_t frequency = shenmue::platform::performanceCounterFrequency();
    const std::int64_t counter = shenmue::platform::performanceCounter();
    return frequency > 0 ? 1000000000LL * (counter / frequency) + 1000000000LL * (counter % frequency) / frequency : 0;
}

template <typename T> T ReadState(const std::byte *state, std::size_t offset) {
    T value{};
    std::memcpy(&value, state + offset, sizeof(value));
    return value;
}

ID3D11Buffer *ResolveBuffer(std::uint32_t handle, bool vertex) {
    if (!handle)
        return nullptr;
    if ((handle & 0xfc000000U) == 0x54000000U) {
        const std::uint32_t poolIndex = (handle >> 16U) & 0xfU;
        const std::uint32_t bufferIndex = handle & 0xffffU;
        if (vertex) {
            if (poolIndex >= g_vertexBufferPools.size() || bufferIndex >= g_vertexBufferPools[poolIndex].count)
                return nullptr;
            return g_vertexBufferPools[poolIndex].buffers[bufferIndex];
        }
        if (poolIndex >= g_constantBufferPools.size() || bufferIndex >= g_constantBufferPools[poolIndex].count)
            return nullptr;
        return g_constantBufferPools[poolIndex].buffers[bufferIndex];
    }
    D3TBufferBackendView view{};
    return GetBufferBackendView(static_cast<std::int32_t>(handle), &view) ? static_cast<ID3D11Buffer *>(view.buffer) : nullptr;
}

ID3D11ShaderResourceView *ResolveShaderResource(const std::byte *state, std::uint32_t stage, std::uint32_t slot) {
    const std::uint32_t texture = ReadState<std::uint32_t>(state, 0x134U + stage * 0x80U + slot * 4U);
    D3TTextureBackendView textureView{};
    if (GetTexture2DBackendView(texture, &textureView)) {
        const std::uint32_t flags = ReadState<std::uint32_t>(state, 0x2b4U + stage * 0x80U + slot * 4U);
        if (!flags || (flags & 1U)) {
            auto *const result = static_cast<ID3D11ShaderResourceView *>(textureView.shaderResourceView);
            return result;
        }
        if ((flags & 2U) == 0) {
            return static_cast<ID3D11ShaderResourceView *>(textureView.secondaryShaderResourceView);
        }
        return nullptr;
    }
    if (slot < 8) {
        const std::uint32_t buffer = ReadState<std::uint32_t>(state, 0x734U + (stage * 8U + slot) * 4U);
        D3TBufferBackendView bufferView{};
        if (GetBufferBackendView(static_cast<std::int32_t>(buffer), &bufferView)) {
            return static_cast<ID3D11ShaderResourceView *>(bufferView.shaderResourceView);
        }
    }
    return nullptr;
}

ID3D11RenderTargetView *ResolveRenderTarget(std::uint32_t texture, std::uint32_t flags) {
    D3TTextureBackendView view{};
    if (!GetTexture2DBackendView(texture, &view) || (flags & 2U)) {
        return nullptr;
    }
    if (view.mipLevels <= 1U) {
        return static_cast<ID3D11RenderTargetView *>(view.view);
    }
    auto **views = static_cast<ID3D11RenderTargetView **>(view.view);
    if (!views)
        return nullptr;
    const std::uint32_t mip = (flags >> 2U) & 0xfU;
    const std::uint32_t arrayCode = flags >> 6U;
    const std::uint32_t arrayIndex = arrayCode ? arrayCode - 1U : 0U;
    if (mip >= view.mipLevels || arrayIndex >= view.arraySize)
        return nullptr;
    return views[arrayIndex * view.mipLevels + mip];
}

void InitializeRenderState(D3TDeviceContext &context) {
    context.trailingState[2] = 0;
    std::memset(context.renderState.data(), 0, 0x978);
    auto *bytes = context.renderState.data();
    *reinterpret_cast<std::int32_t *>(bytes + 0x7d8) = -1;
    *reinterpret_cast<std::uint64_t *>(bytes + 0x958) = ~std::uint64_t{0};
    *reinterpret_cast<std::uint64_t *>(bytes + 0x960) = ~std::uint64_t{0};
    *reinterpret_cast<std::uint64_t *>(bytes + 0x968) = ~std::uint64_t{0};
    *reinterpret_cast<std::uint32_t *>(bytes + 0x970) = ~std::uint32_t{0};
    bytes[0x974] = std::byte{0xff};
    bytes[0x975] = std::byte{0xff};
    bytes[0x976] = std::byte{0xff};
}

void SyncResourceBindings(D3TDeviceContext &context, std::uint32_t startIndex, const std::uint32_t *resourceTags, std::uint32_t entryCount) {
    auto *const deviceContext = static_cast<ID3D11DeviceContext *>(context.deviceContext);
    auto *const bytes = reinterpret_cast<std::byte *>(&context);
    std::uint32_t mask = std::rotl(1U, static_cast<int>(startIndex));
    for (std::uint32_t offset = 0; offset < entryCount; ++offset) {
        const std::uint32_t index = startIndex + offset;
        auto *const cache = context.resourceCache.data() + index * 3;
        if (cache[0]) {
            deviceContext->Unmap(reinterpret_cast<ID3D11Resource *>(cache[0]), 0);
            cache[0] = 0;
        }
        auto *const currentTag = reinterpret_cast<std::uint32_t *>(bytes + 0x34 + index * sizeof(std::uint32_t));
        if (*currentTag != resourceTags[offset]) {
            auto *const dirtyMask = reinterpret_cast<std::uint32_t *>(bytes + 0x998);
            *dirtyMask |= mask;
        }
        *currentTag = resourceTags[offset];
        mask = std::rotl(mask, 1);
    }
}

void ResetMappedResources(D3TDeviceContext &context) {
    auto *const deviceContext = static_cast<ID3D11DeviceContext *>(context.deviceContext);
    if (!deviceContext) {
        return;
    }
    for (std::uint32_t index = 0; index < 14; ++index) {
        auto *const cache = context.resourceCache.data() + index * 3;
        if (!cache[0]) {
            continue;
        }
        deviceContext->Unmap(reinterpret_cast<ID3D11Resource *>(cache[0]), 0);
        const std::uint32_t startIndex = *reinterpret_cast<const std::uint32_t *>(&cache[2]);
        cache[0] = 0;
        SyncResourceBindings(context, startIndex, reinterpret_cast<const std::uint32_t *>(&cache[1]), 1);
    }
}

void UnmapAndSyncShaderBindings(D3TDeviceContext &context) {
    auto *const deviceContext = static_cast<ID3D11DeviceContext *>(context.deviceContext);
    if (!deviceContext) {
        return;
    }
    auto *const contextBytes = reinterpret_cast<std::byte *>(&context);
    for (std::uint32_t recordIndex = 0; recordIndex < 14; ++recordIndex) {
        std::byte *const record = contextBytes + 0x9a0 + recordIndex * 0x18;
        auto *&resource = *reinterpret_cast<ID3D11Resource **>(record);
        if (!resource) {
            continue;
        }
        deviceContext->Unmap(resource, 0);
        const std::uint32_t tag = *reinterpret_cast<const std::uint32_t *>(record + 8);
        const std::uint32_t flags = *reinterpret_cast<const std::uint32_t *>(record + 0xc);
        const std::uint32_t resourceIndex = *reinterpret_cast<const std::uint32_t *>(record + 0x10);
        resource = nullptr;
        for (std::uint32_t stage = 0; stage < 3; ++stage) {
            if ((flags & (1U << stage)) == 0) {
                continue;
            }
            auto *const currentTag = reinterpret_cast<std::uint32_t *>(contextBytes + 0xb4 + (stage * 14 + resourceIndex) * sizeof(std::uint32_t));
            if (*currentTag != tag) {
                auto *const dirtyMask = reinterpret_cast<std::uint16_t *>(contextBytes + 0x992 + stage * sizeof(std::uint16_t));
                *dirtyMask = static_cast<std::uint16_t>(*dirtyMask | (1U << resourceIndex));
                *currentTag = tag;
            }
        }
    }
}

} // namespace

void *GetNativeSwapChain() {
    return g_swapChain;
}

bool BeginOcclusionQuery(std::uint32_t index) {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (index >= g_occlusionQueries.size() || !g_occlusionQueries[index] || !context || !context->deviceContext)
        return false;
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Begin(g_occlusionQueries[index]);
    return true;
}

bool EndOcclusionQuery(std::uint32_t index) {
    if (index >= g_occlusionQueries.size() || !g_occlusionQueries[index] || !g_immediateContext)
        return false;
    g_immediateContext->End(g_occlusionQueries[index]);
    return true;
}

std::uint64_t ReadOcclusionQuerySamples(std::uint32_t index) {
    if (index >= g_occlusionQueries.size() || !g_occlusionQueries[index] || !g_immediateContext)
        return 0;
    std::uint64_t samples = 0;
    while (g_immediateContext->GetData(g_occlusionQueries[index], &samples, sizeof(samples), 0) != S_OK) {
    }
    return samples;
}

void CreateUserDefinedAnnotations() {
    for (std::size_t i = 0; i < g_deviceContexts.size(); ++i) {
        D3TDeviceContext &context = g_deviceContexts[i];
        g_deviceContextBusy[i].store(0, std::memory_order_relaxed);
        std::memset(&context, 0, sizeof(context));
        ID3D11DeviceContext *deferred = nullptr;
        g_device->CreateDeferredContext(0, &deferred);
        context.deferredDeviceContext = deferred;
        if (deferred != nullptr) {
            ID3DUserDefinedAnnotation *annotation = nullptr;
            if (SUCCEEDED(deferred->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void **>(&annotation)))) {
                context.deferredUserAnnotation = annotation;
            }
        }
    }
}

void CreateConstantBuffers() {
    auto **storage = static_cast<ID3D11Buffer **>(std::calloc(12544, sizeof(ID3D11Buffer *)));
    D3D11_BUFFER_DESC description{};
    description.Usage = D3D11_USAGE_DYNAMIC;
    description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    std::uint32_t byteWidth = 128;
    for (std::size_t poolIndex = 0; poolIndex < g_constantBufferPools.size(); ++poolIndex) {
        BufferPool &pool = g_constantBufferPools[poolIndex];
        pool.count = kConstantBufferCounts[poolIndex];
        pool.current = 0;
        pool.buffers = storage;
        storage += pool.count;
        description.ByteWidth = byteWidth;
        for (std::uint32_t i = 0; i < pool.count; ++i) {
            g_device->CreateBuffer(&description, nullptr, &pool.buffers[i]);
        }
        byteWidth <<= 1;
    }
}

void CreateVertexBuffers() {
    auto **storage = static_cast<ID3D11Buffer **>(std::calloc(2336, sizeof(ID3D11Buffer *)));
    D3D11_BUFFER_DESC description{};
    description.Usage = D3D11_USAGE_DYNAMIC;
    description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    for (std::size_t poolIndex = 0; poolIndex < g_vertexBufferPools.size(); ++poolIndex) {
        BufferPool &pool = g_vertexBufferPools[poolIndex];
        pool.count = kVertexBufferSpecs[poolIndex].count;
        pool.current = 0;
        pool.buffers = storage;
        storage += pool.count;
        description.ByteWidth = kVertexBufferSpecs[poolIndex].byteWidth;
        for (std::uint32_t i = 0; i < pool.count; ++i) {
            g_device->CreateBuffer(&description, nullptr, &pool.buffers[i]);
        }
    }
}

void CreateNewSRVTexture() {
    D3D11_TEXTURE2D_DESC description{};
    g_backBuffer->GetDesc(&description);
    description.Usage = D3D11_USAGE_DEFAULT;
    description.BindFlags = 0;
    description.CPUAccessFlags = 0;
    description.SampleDesc = {1, 0};
    description.MiscFlags = 0;
    g_device->CreateTexture2D(&description, nullptr, &g_mipsAccessTexture);
    description.Usage = D3D11_USAGE_STAGING;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    g_device->CreateTexture2D(&description, nullptr, &g_readbackTexture);
}

void ShutdownDX11() {
    if (g_immediateContext)
        g_immediateContext->ClearState();

    for (D3TDeviceContext &context : g_deviceContexts) {
        if (context.commandList) {
            static_cast<ID3D11CommandList *>(context.commandList)->Release();
        }
        if (context.deferredUserAnnotation) {
            static_cast<ID3DUserDefinedAnnotation *>(context.deferredUserAnnotation)->Release();
        }
        if (context.deferredDeviceContext) {
            static_cast<ID3D11DeviceContext *>(context.deferredDeviceContext)->Release();
        }
        context = {};
    }
    for (ID3D11Query *&query : g_occlusionQueries) {
        if (query)
            query->Release();
        query = nullptr;
    }

    auto releasePools = [](auto &pools) {
        auto **allocation = pools.empty() ? nullptr : pools[0].buffers;
        for (BufferPool &pool : pools) {
            for (std::uint32_t index = 0; index < pool.count; ++index) {
                if (pool.buffers && pool.buffers[index]) {
                    pool.buffers[index]->Release();
                }
            }
            pool = {};
        }
        std::free(allocation);
    };
    releasePools(g_constantBufferPools);
    releasePools(g_vertexBufferPools);


    {
        const std::lock_guard<std::mutex> mipsLock(g_generateMipsMutex);
        std::vector<std::uint32_t>().swap(g_generateMipsTextures);
    }
    {
        const TextureCreationLock queueLock;
        std::vector<QueuedTextureCopy>().swap(g_queuedTextureCopies);
    }

    auto release = [](auto *&object) {
        if (object)
            object->Release();
        object = nullptr;
    };
    release(g_immediateAnnotation);
    release(g_backBufferShaderResource);
    release(g_backBufferRenderTarget);
    release(g_backBuffer);
    release(g_mipsAccessTexture);
    release(g_readbackTexture);
    release(g_immediateContext);
    release(g_swapChain);
    release(g_device);
    g_D3DDevice = nullptr;
}

void DX11_D3D11CreateDeviceAndSwapChain(const DisplayModeInfo *config, void *windowHandle) {
    LARGE_INTEGER frequency{};
    QueryPerformanceFrequency(&frequency);
    g_originalScreenHeight = config->height;
    g_originalScreenWidth = config->width;

    DXGI_SWAP_CHAIN_DESC swapChainDescription{};
    swapChainDescription.BufferDesc.Width = config->width;
    swapChainDescription.BufferDesc.Height = config->height;
    swapChainDescription.BufferDesc.RefreshRate.Numerator = config->refreshRateNumerator;
    swapChainDescription.BufferDesc.RefreshRate.Denominator = config->refreshRateDenominator;
    swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDescription.BufferCount = 1;
    swapChainDescription.OutputWindow = static_cast<HWND>(windowHandle);
    swapChainDescription.SampleDesc = {1, 0};
    swapChainDescription.Windowed = TRUE;
    swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    g_defaultTextureFormat = 3;
    constexpr D3D_FEATURE_LEVEL featureLevels[]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    D3D_FEATURE_LEVEL selectedFeatureLevel{};
    constexpr UINT createFlags = 0;
    HRESULT result =
        D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags, featureLevels, static_cast<UINT>(std::size(featureLevels)),
            D3D11_SDK_VERSION, &swapChainDescription, &g_swapChain, &g_device, &selectedFeatureLevel, &g_immediateContext);
    if (result == E_INVALIDARG) {
        constexpr D3D_FEATURE_LEVEL fallback = D3D_FEATURE_LEVEL_10_0;
        result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags, &fallback, 1, D3D11_SDK_VERSION, &swapChainDescription,
            &g_swapChain, &g_device, &selectedFeatureLevel, &g_immediateContext);
    }
    if (FAILED(result)) {
        return;
    }
    g_D3DDevice = g_device;

    CreateUserDefinedAnnotations();
    CreateConstantBuffers();
    CreateVertexBuffers();
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&g_backBuffer));
    g_device->CreateRenderTargetView(g_backBuffer, nullptr, &g_backBufferRenderTarget);
    g_device->CreateShaderResourceView(g_backBuffer, nullptr, &g_backBufferShaderResource);
    CreateNewSRVTexture();

    D3D11_VIEWPORT viewport{};
    viewport.Width = static_cast<float>(g_originalScreenWidth);
    viewport.Height = static_cast<float>(g_originalScreenHeight);
    viewport.MaxDepth = 1.0f;
    g_immediateContext->RSSetViewports(1, &viewport);

    D3D11_DEPTH_STENCIL_DESC depthDescription{};
    depthDescription.DepthEnable = TRUE;
    depthDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDescription.DepthFunc = D3D11_COMPARISON_GREATER;
    ID3D11DepthStencilState *depthState = nullptr;
    g_device->CreateDepthStencilState(&depthDescription, &depthState);
    g_immediateContext->OMSetDepthStencilState(depthState, 0);
    if (depthState != nullptr)
        depthState->Release();

    D3D11_RASTERIZER_DESC rasterDescription{};
    rasterDescription.FillMode = D3D11_FILL_SOLID;
    rasterDescription.CullMode = D3D11_CULL_BACK;
    rasterDescription.DepthClipEnable = TRUE;
    rasterDescription.AntialiasedLineEnable = TRUE;
    ID3D11RasterizerState *rasterState = nullptr;
    g_device->CreateRasterizerState(&rasterDescription, &rasterState);
    g_immediateContext->RSSetState(rasterState);
    if (rasterState != nullptr)
        rasterState->Release();

    if (FAILED(g_immediateContext->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void **>(&g_immediateAnnotation)))) {
        g_immediateAnnotation = nullptr;
    }
    D3D11_QUERY_DESC queryDescription{D3D11_QUERY_OCCLUSION, 0};
    for (ID3D11Query *&query : g_occlusionQueries) {
        if (FAILED(g_device->CreateQuery(&queryDescription, &query))) {
            query = nullptr;
        }
    }
}

D3TDeviceContext *DXGetPipelineContext(std::int32_t mode) {
    for (std::size_t i = 0; i < g_deviceContexts.size(); ++i) {
        std::uint8_t expected = 0;
        if (!g_deviceContextBusy[i].compare_exchange_strong(expected, 1, std::memory_order_acquire)) {
            continue;
        }
        D3TDeviceContext &context = g_deviceContexts[i];
        ResetMappedResources(context);
        UnmapAndSyncShaderBindings(context);
        if (mode == 0) {
            context.deviceContext = g_immediateContext;
            context.userAnnotation = g_immediateAnnotation;
        } else if (mode == 1) {
            context.deviceContext = context.deferredDeviceContext;
            context.userAnnotation = context.deferredUserAnnotation;
        }
        InitializeRenderState(context);
        return &context;
    }
    return nullptr;
}

void DX11::WaitForCommandListFinish(D3TDeviceContext *context) {
    if (!context) {
        return;
    }
    ResetMappedResources(*context);
    UnmapAndSyncShaderBindings(*context);
    auto *const deviceContext = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (deviceContext != g_immediateContext) {
        deviceContext->FinishCommandList(FALSE, reinterpret_cast<ID3D11CommandList **>(&context->commandList));
        auto *const commandList = static_cast<ID3D11CommandList *>(context->commandList);
        if (commandList) {
            g_immediateContext->ExecuteCommandList(commandList, FALSE);
            commandList->Release();
            context->commandList = nullptr;
        }
    }
    for (std::size_t index = 0; index < g_deviceContexts.size(); ++index) {
        if (&g_deviceContexts[index] == context) {
            g_deviceContextBusy[index].store(0, std::memory_order_release);
            return;
        }
    }
}

std::int64_t DX11_ResetMappedResources(D3TDeviceContext *context) {
    if (context)
        ResetMappedResources(*context);
    return 0;
}

void DX11_UnmapAndSyncShaderBindings(D3TDeviceContext *context) {
    if (context)
        UnmapAndSyncShaderBindings(*context);
}

void UnmapAndClearViews(D3TDeviceContext *context, std::uint32_t texture2D, std::int64_t unused, const float *clearValue) {
    static_cast<void>(unused);
    if (!context || !context->deviceContext || !clearValue)
        return;
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);

    D3TTextureBackendView view{};
    if (!GetTexture2DBackendView(texture2D, &view) || !view.view)
        return;
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if ((view.bindFlags & 8U) != 0) {
        native->ClearDepthStencilView(
            static_cast<ID3D11DepthStencilView *>(view.view), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearValue[0], static_cast<UINT8>(clearValue[1]));
        return;
    }
    if ((view.bindFlags & 0x20U) != 0) {
        constexpr std::uint32_t kFloatClearFormats = 0x20144U;
        if (view.format <= 17U && (kFloatClearFormats & (1U << view.format)) != 0) {
            native->ClearUnorderedAccessViewFloat(static_cast<ID3D11UnorderedAccessView *>(view.view), clearValue);
        }
        return;
    }
    if ((view.bindFlags & 4U) == 0)
        return;
    if (view.mipLevels <= 1U) {
        native->ClearRenderTargetView(static_cast<ID3D11RenderTargetView *>(view.view), clearValue);
        return;
    }
    auto **const renderTargets = static_cast<ID3D11RenderTargetView **>(view.view);
    for (std::uint32_t arrayIndex = 0; arrayIndex < view.arraySize; ++arrayIndex) {
        for (std::uint32_t mip = 0; mip < view.mipLevels; ++mip) {
            native->ClearRenderTargetView(renderTargets[arrayIndex * view.mipLevels + mip], clearValue);
        }
    }
}

void *MapTempConstantBufferForWriteDiscard(D3TDeviceContext *context, std::int32_t stageMask, std::uint32_t registerIndex, std::int32_t bufferSize) {
    if (!context || !context->deviceContext || registerIndex >= 14 || bufferSize <= 0)
        return nullptr;
    auto *deviceContext = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    auto *const contextBytes = reinterpret_cast<std::byte *>(context);
    std::byte *const record = contextBytes + 0x9a0U + registerIndex * 0x18U;
    if (auto *mappedResource = *reinterpret_cast<ID3D11Resource **>(record)) {
        deviceContext->Unmap(mappedResource, 0);
    }

    const std::uint32_t doubled = static_cast<std::uint32_t>(bufferSize * 2 - 1);
    const std::int32_t highestBit = doubled ? static_cast<std::int32_t>(std::bit_width(doubled) - 1U) : 4;
    const std::uint32_t poolIndex = static_cast<std::uint32_t>((std::max)(highestBit - 7, 0));
    if (poolIndex >= g_constantBufferPools.size())
        return nullptr;
    BufferPool &pool = g_constantBufferPools[poolIndex];
    if (!pool.count || !pool.buffers)
        return nullptr;
    std::atomic_ref<std::uint32_t> allocationCounter(pool.reserved_04);
    const std::uint32_t bufferIndex = allocationCounter.fetch_add(1, std::memory_order_seq_cst) % pool.count;
    const std::uint32_t handle = 0x54000000U | (poolIndex << 16U) | bufferIndex;
    ID3D11Buffer *const buffer = pool.buffers[bufferIndex];
    if (!buffer)
        return nullptr;
    D3D11_MAPPED_SUBRESOURCE mapped{};
    if (FAILED(deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        return nullptr;
    *reinterpret_cast<ID3D11Resource **>(record) = buffer;
    *reinterpret_cast<std::uint32_t *>(record + 8) = handle;
    *reinterpret_cast<std::int32_t *>(record + 0xc) = stageMask;
    *reinterpret_cast<std::uint32_t *>(record + 0x10) = registerIndex;
    return mapped.pData;
}

void *GetMappedConstantBuffer(D3TDeviceContext *context, std::int64_t unused, std::uint32_t size) {
    static_cast<void>(unused);
    if (!context || !context->deviceContext || size == 0)
        return nullptr;

    auto *const deviceContext = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    auto *const cache = context->resourceCache.data();
    if (cache[0]) {
        deviceContext->Unmap(reinterpret_cast<ID3D11Resource *>(cache[0]), 0);
    }

    std::size_t poolIndex = 0;
    while (poolIndex < kVertexBufferSpecs.size() && size >= kVertexBufferSpecs[poolIndex].byteWidth) {
        ++poolIndex;
    }
    if (poolIndex >= g_vertexBufferPools.size())
        return nullptr;

    BufferPool &pool = g_vertexBufferPools[poolIndex];
    if (!pool.count || !pool.buffers)
        return nullptr;
    std::atomic_ref<std::uint32_t> allocationCounter(pool.reserved_04);
    const std::uint32_t bufferIndex = allocationCounter.fetch_add(1, std::memory_order_seq_cst) % pool.count;
    ID3D11Buffer *const buffer = pool.buffers[bufferIndex];
    if (!buffer)
        return nullptr;

    D3D11_MAPPED_SUBRESOURCE mapped{};
    if (FAILED(deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
        return nullptr;
    }
    cache[0] = reinterpret_cast<std::uintptr_t>(buffer);
    cache[1] = 0x54000000U | (static_cast<std::uint32_t>(poolIndex) << 16U) | bufferIndex;
    cache[2] = 0;
    return mapped.pData;
}

std::int64_t DX11_SetupShadersForDXAndSetViewports(D3TDeviceContext *context) {
    if (!context || !context->deviceContext)
        return 0;
    auto *deviceContext = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    std::byte *const state = context->renderState.data();
    const std::uint8_t pipelineFlags = std::to_integer<std::uint8_t>(state[0x975]);
    const std::uint8_t bufferFlags = std::to_integer<std::uint8_t>(state[0x976]);

    if (pipelineFlags & 1U) {
        const std::uint32_t handle = ReadState<std::uint32_t>(state, 0);
        if (void *shader = GetNativeShader(handle, ShaderType::Vertex)) {
            deviceContext->VSSetShader(static_cast<ID3D11VertexShader *>(shader), nullptr, 0);
        }
    }
    if (pipelineFlags & 2U) {
        const std::uint32_t handle = ReadState<std::uint32_t>(state, 4);
        auto *shader = static_cast<ID3D11PixelShader *>(GetNativeShader(handle, ShaderType::Pixel));
        deviceContext->PSSetShader(shader, nullptr, 0);
    }
    if (pipelineFlags & 4U) {
        const std::uint32_t handle = ReadState<std::uint32_t>(state, 8);
        if (void *shader = GetNativeShader(handle, ShaderType::Compute)) {
            deviceContext->CSSetShader(static_cast<ID3D11ComputeShader *>(shader), nullptr, 0);
        }
    }

    if (bufferFlags & 2U) {
        deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    }

    for (std::uint32_t stage = 0; stage < 3; ++stage) {
        const std::uint16_t constantMask = ReadState<std::uint16_t>(state, 0x96aU + stage * 2U);
        if (constantMask) {
            const std::uint32_t start = std::countr_zero(constantMask);
            const std::uint32_t end = 15U - std::countl_zero(constantMask);
            const std::uint32_t count = (std::min)(14U, end - start + 1U);
            std::array<ID3D11Buffer *, 14> buffers{};
            for (std::uint32_t i = 0; i < count; ++i) {
                buffers[i] = ResolveBuffer(ReadState<std::uint32_t>(state, 0x8cU + stage * 56U + (start + i) * 4U), false);
            }
            if (stage == 0)
                deviceContext->VSSetConstantBuffers(start, count, buffers.data());
            else if (stage == 1)
                deviceContext->PSSetConstantBuffers(start, count, buffers.data());
            else
                deviceContext->CSSetConstantBuffers(start, count, buffers.data());
        }

        const std::uint32_t resourceMask = ReadState<std::uint32_t>(state, 0x958U + stage * 4U);
        if (resourceMask) {
            const std::uint32_t start = std::countr_zero(resourceMask);
            const std::uint32_t end = 31U - std::countl_zero(resourceMask);
            const std::uint32_t count = end - start + 1U;
            std::array<ID3D11ShaderResourceView *, 32> views{};
            for (std::uint32_t i = 0; i < count; ++i) {
                views[i] = ResolveShaderResource(state, stage, start + i);
            }
            if (stage == 0)
                deviceContext->VSSetShaderResources(start, count, views.data());
            else if (stage == 1)
                deviceContext->PSSetShaderResources(start, count, views.data());
            else
                deviceContext->CSSetShaderResources(start, count, views.data());
        }

        const std::uint16_t samplerMask = ReadState<std::uint16_t>(state, 0x964U + stage * 2U);
        if (samplerMask) {
            const std::uint32_t start = std::countr_zero(samplerMask);
            const std::uint32_t end = 15U - std::countl_zero(samplerMask);
            const std::uint32_t count = end - start + 1U;
            std::array<ID3D11SamplerState *, 16> samplers{};
            for (std::uint32_t i = 0; i < count; ++i) {
                const auto *descriptor = reinterpret_cast<const std::uint64_t *>(state + 0x434U + stage * 0x100U + (start + i) * 0x10U);
                samplers[i] = static_cast<ID3D11SamplerState *>(RetrieveHashedSamplerState(descriptor));
            }
            if (stage == 0)
                deviceContext->VSSetSamplers(start, count, samplers.data());
            else if (stage == 1)
                deviceContext->PSSetSamplers(start, count, samplers.data());
            else
                deviceContext->CSSetSamplers(start, count, samplers.data());
        }
    }

    if (pipelineFlags & 8U) {
        auto *blend = static_cast<ID3D11BlendState *>(RetrieveHashedBlendState(state + 0x794));
        if (blend) {
            constexpr std::array<float, 4> factor{};
            deviceContext->OMSetBlendState(blend, factor.data(), ReadState<std::uint32_t>(state, 0x7d8));
        }
    }
    if (pipelineFlags & 0x10U) {
        auto *depth = static_cast<ID3D11DepthStencilState *>(RetrieveHashedStencilState(reinterpret_cast<const std::uint64_t *>(state + 0x940)));
        deviceContext->OMSetDepthStencilState(depth, ReadState<std::uint32_t>(state, 0x950));
    }
    if (pipelineFlags & 0x20U) {
        auto *rasterizer = static_cast<ID3D11RasterizerState *>(RetrieveHashedRasterizerState(reinterpret_cast<const std::uint32_t *>(state + 0x954)));
        deviceContext->RSSetState(rasterizer);
    }
    if ((pipelineFlags & 0x40U) && ReadState<std::uint32_t>(state, 0x930)) {
        D3TBufferBackendView view{};
        if (GetBufferBackendView(static_cast<std::int32_t>(ReadState<std::uint32_t>(state, 0x930)), &view)) {
            deviceContext->IASetIndexBuffer(
                static_cast<ID3D11Buffer *>(view.buffer), static_cast<DXGI_FORMAT>(GetD3TNativeTextureFormat(ReadState<std::uint32_t>(state, 0x934))), 0);
        }
    }

    const std::uint32_t fvfHandle = ReadState<std::uint32_t>(state, 0x938);
    if (pipelineFlags & 0x80U) {
        deviceContext->IASetInputLayout(static_cast<ID3D11InputLayout *>(GetInputLayout(ReadState<std::uint32_t>(state, 0), fvfHandle)));
    }
    if (bufferFlags & 1U) {
        constexpr std::array<D3D11_PRIMITIVE_TOPOLOGY, 3> topologies{
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, D3D11_PRIMITIVE_TOPOLOGY_LINELIST};
        const std::uint32_t topology = ReadState<std::uint32_t>(state, 0x93c);
        deviceContext->IASetPrimitiveTopology(topology < topologies.size() ? topologies[topology] : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    }

    if (bufferFlags & 2U) {
        std::array<ID3D11RenderTargetView *, 8> renderTargets{};
        UINT renderTargetCount = 0;
        for (; renderTargetCount < renderTargets.size(); ++renderTargetCount) {
            const std::size_t offset = 0x7dcU + renderTargetCount * 8U;
            const std::uint32_t texture = ReadState<std::uint32_t>(state, offset);
            D3TTextureBackendView view{};
            if (!GetTexture2DBackendView(texture, &view))
                break;
            renderTargets[renderTargetCount] = ResolveRenderTarget(texture, ReadState<std::uint32_t>(state, offset + 4U));
        }
        std::array<ID3D11UnorderedAccessView *, 8> unorderedViews{};
        UINT unorderedCount = 0;
        for (std::uint32_t i = 0; i < 8; ++i) {
            const std::size_t offset = 0x81cU + i * 8U;
            D3TTextureBackendView texture{};
            if (GetTexture2DBackendView(ReadState<std::uint32_t>(state, offset), &texture) && (texture.bindFlags & 0x20U)) {
                unorderedViews[unorderedCount++] = static_cast<ID3D11UnorderedAccessView *>(texture.view);
                continue;
            }
            D3TBufferBackendView buffer{};
            if (GetBufferBackendView(static_cast<std::int32_t>(ReadState<std::uint32_t>(state, offset + 4U)), &buffer) && buffer.unorderedAccessView) {
                unorderedViews[unorderedCount++] = static_cast<ID3D11UnorderedAccessView *>(buffer.unorderedAccessView);
            }
        }
        ID3D11DepthStencilView *depthView = nullptr;
        D3TTextureBackendView depthTexture{};
        if (GetTexture2DBackendView(ReadState<std::uint32_t>(state, 0x85c), &depthTexture)) {
            depthView = static_cast<ID3D11DepthStencilView *>(depthTexture.view);
        }
        deviceContext->OMSetRenderTargetsAndUnorderedAccessViews(
            renderTargetCount, renderTargets.data(), depthView, 2, unorderedCount, unorderedViews.data(), nullptr);
    }

    if (bufferFlags & 4U) {
        const auto *packed = reinterpret_cast<const std::int32_t *>(state + 0x860);
        const D3D11_RECT rect{packed[0], packed[1], packed[2], packed[3]};
        deviceContext->RSSetScissorRects(1, &rect);
    }

    const std::uint32_t vertexMask = ReadState<std::uint32_t>(state, 0x970);
    if (vertexMask) {
        const std::uint32_t start = std::countr_zero(vertexMask);
        const std::uint32_t end = 31U - std::countl_zero(vertexMask);
        const std::uint32_t span = end - start + 1U;
        std::uint32_t count = 0;
        std::array<ID3D11Buffer *, 32> buffers{};
        std::array<UINT, 32> strides{};
        std::array<UINT, 32> offsets{};
        std::uint32_t fvfIndex = 0;
        const FVFDescription *description = GetFVFDescription(fvfHandle, &fvfIndex);
        if (description) {
            for (std::uint32_t i = 0; i < span; ++i) {
                const std::uint32_t slot = start + i;
                const std::uint32_t handle = ReadState<std::uint32_t>(state, 0xcU + slot * 4U);
                buffers[i] = ResolveBuffer(handle, true);
                if (handle)
                    ++count;
                strides[i] = GetFVFStride(fvfIndex, slot);
                offsets[i] = GetFVFOffset(fvfIndex, slot);
            }
            count = (std::min)(count, description->elementCount);
            if (count) {
                deviceContext->IASetVertexBuffers(start, count, buffers.data(), strides.data(), offsets.data());
            }
        }
    }

    if (state[0x974] != std::byte{0}) {
        deviceContext->RSSetViewports(1, reinterpret_cast<const D3D11_VIEWPORT *>(state + 0x870));
    }

    std::memset(state + 0x958, 0, 0x1c);
    state[0x974] = std::byte{0};
    state[0x975] = std::byte{0};
    state[0x976] = std::byte{0};
    return 0;
}

std::uint32_t CreateNewTexture2D() {
    g_currentTextureHandle = AdoptBackBufferTexture2D(g_currentTextureHandle, g_originalScreenWidth, g_originalScreenHeight, g_defaultTextureFormat,
        g_backBuffer, g_backBufferRenderTarget, g_backBufferShaderResource);
    return g_currentTextureHandle;
}





void DirectXUpdateWindowDimensions(std::uint32_t width, std::uint32_t height) {
    if ((width == g_originalScreenWidth && height == g_originalScreenHeight) || !g_swapChain)
        return;
    g_immediateContext->OMSetRenderTargets(0, nullptr, nullptr);
    const auto release = [](auto *&object) {
        if (object)
            object->Release();
        object = nullptr;
    };
    release(g_backBufferRenderTarget);
    release(g_backBufferShaderResource);
    release(g_backBuffer);
    release(g_mipsAccessTexture);
    release(g_readbackTexture);
    ReleaseBackBufferTexture2D(g_currentTextureHandle);
    g_originalScreenHeight = height;
    g_originalScreenWidth = width;
    for (D3TDeviceContext &context : g_deviceContexts) {
        ID3D11DeviceContext *const deferred = static_cast<ID3D11DeviceContext *>(context.deferredDeviceContext);
        if (!deferred)
            continue;
        deferred->ClearState();
        deferred->FinishCommandList(FALSE, reinterpret_cast<ID3D11CommandList **>(&context.commandList));
        if (ID3D11CommandList *const commandList = static_cast<ID3D11CommandList *>(context.commandList)) {
            commandList->Release();
            context.commandList = nullptr;
        }
    }
    g_immediateContext->ClearState();
    g_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&g_backBuffer));
    g_device->CreateRenderTargetView(g_backBuffer, nullptr, &g_backBufferRenderTarget);
    g_device->CreateShaderResourceView(g_backBuffer, nullptr, &g_backBufferShaderResource);
    CreateNewSRVTexture();
    const D3D11_VIEWPORT viewport{
        .TopLeftX = 0.0f, .TopLeftY = 0.0f, .Width = static_cast<float>(width), .Height = static_cast<float>(height), .MinDepth = 0.0f, .MaxDepth = 1.0f};
    g_immediateContext->RSSetViewports(1, &viewport);
}



void PresentFrame(std::int32_t renderMode) {
    if (!g_backBuffer || !g_immediateContext || !g_swapChain) {
        return;
    }

    std::string frameDumpFilename;
    if (ShouldDumpPresentedFrame(frameDumpFilename)) {
        if (g_mipsAccessTexture && g_readbackTexture) {
            D3D11_TEXTURE2D_DESC description{};
            g_backBuffer->GetDesc(&description);
            if (description.SampleDesc.Count == 1) {
                g_immediateContext->CopyResource(g_mipsAccessTexture, g_backBuffer);
            } else {
                g_immediateContext->ResolveSubresource(g_mipsAccessTexture, 0, g_backBuffer, 0, description.Format);
            }
            g_immediateContext->CopyResource(g_readbackTexture, g_mipsAccessTexture);

            D3D11_MAPPED_SUBRESOURCE mapped{};
            const HRESULT mapResult = g_immediateContext->Map(g_readbackTexture, 0, D3D11_MAP_READ, 0, &mapped);
            if (SUCCEEDED(mapResult)) {
                const bool written = WriteMappedFramePng(frameDumpFilename, description, mapped);
                g_immediateContext->Unmap(g_readbackTexture, 0);
                std::fflush(stdout);
            } else {
                std::fflush(stdout);
            }
        } else {
            std::fflush(stdout);
        }
    }

    const UINT syncInterval = renderMode >= 1 && renderMode <= 2 ? 1U : 0U;
    g_swapChain->Present(syncInterval, 0);

    if (g_presentFrameCount == 0) {
        g_previousPresentNanoseconds = PresentClockNanoseconds();
    }
    if (renderMode != 0 && g_presentFrameCount != 0) {
        const std::int64_t frameDuration = renderMode == 1 ? 33333333LL : 16666666LL;
        const std::int64_t target = g_previousPresentNanoseconds + frameDuration;
        const std::int64_t sleepTarget = g_previousPresentNanoseconds + frameDuration * 9 / 10;
        std::int64_t now = PresentClockNanoseconds();
        if (now < sleepTarget) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(sleepTarget - now));
        }
        while ((now = PresentClockNanoseconds()) < target) {
            std::this_thread::yield();
        }
        g_previousPresentNanoseconds = now;
    }
    ++g_presentFrameCount;

    for (BufferPool &pool : g_constantBufferPools) {
        pool.current = pool.reserved_04;
    }
}

void DumpFramePng(const char *filename) {
    if (filename && *filename)
        g_pendingFrameDump = filename;
}

bool WriteCapturedFrameJpeg(const char *filename, const FramePixelCapture &capture) {
#if defined(SHENMUE_HAS_DIRECTXTEX)
    if (!filename || !*filename || !capture.width || !capture.height || capture.width > std::numeric_limits<std::size_t>::max() / 4U) {
        return false;
    }
    const std::size_t rowPitch = static_cast<std::size_t>(capture.width) * 4U;
    if (capture.height > std::numeric_limits<std::size_t>::max() / rowPitch || capture.pixels.size() < rowPitch * static_cast<std::size_t>(capture.height)) {
        return false;
    }
    DirectX::Image image{};
    image.width = capture.width;
    image.height = capture.height;
    image.format = static_cast<DXGI_FORMAT>(capture.format);
    image.rowPitch = rowPitch;
    image.slicePitch = rowPitch * static_cast<std::size_t>(capture.height);
    image.pixels = reinterpret_cast<std::uint8_t *>(const_cast<std::byte *>(capture.pixels.data()));
    const std::filesystem::path path(filename);
    const auto setQuality = [](IPropertyBag2 *properties) {
        if (!properties)
            return;
        PROPBAG2 option{};
        option.pstrName = const_cast<wchar_t *>(L"ImageQuality");
        VARIANT value{};
        VariantInit(&value);
        value.vt = VT_R4;
        value.fltVal = 0.85f;
        properties->Write(1, &option, &value);
        VariantClear(&value);
    };
    return SUCCEEDED(DirectX::SaveToWICFile(image, DirectX::WIC_FLAGS_NONE, GUID_ContainerFormatJpeg, path.c_str(), nullptr, setQuality));
#else
    static_cast<void>(filename);
    static_cast<void>(capture);
    return false;
#endif
}
#endif

std::uint64_t GetNumRenderedFrames() noexcept {
    return g_presentFrameCount;
}

} // namespace shenmue::d3t
