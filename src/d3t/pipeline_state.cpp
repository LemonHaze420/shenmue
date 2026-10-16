#include "d3t/pipeline_state.hpp"

#include "d3t/buffer.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#endif

namespace shenmue::d3t {
namespace {

template <typename T> struct NativeStateCache {
    std::array<T *, 256> states{};
    std::array<std::uint32_t, 256> hashes{};
    std::array<std::uint16_t, 256> probeOffsets{};
    std::uint32_t count{};
};

NativeStateCache<void> g_samplerStates;
NativeStateCache<void> g_blendStates;
NativeStateCache<void> g_depthStencilStates;
NativeStateCache<void> g_rasterizerStates;

template <typename T> bool FindState(NativeStateCache<void> &cache, std::uint32_t hash, T *&state) {
    std::uint8_t index = static_cast<std::uint8_t>(hash);
    if (cache.hashes[index] == hash) {
        state = static_cast<T *>(cache.states[index]);
        return true;
    }
    for (std::uint16_t offset = cache.probeOffsets[index]; offset; offset = cache.probeOffsets[index]) {
        index = static_cast<std::uint8_t>(index + offset);
        if (cache.hashes[index] == hash) {
            state = static_cast<T *>(cache.states[index]);
            return true;
        }
    }
    state = nullptr;
    return false;
}

bool InsertState(NativeStateCache<void> &cache, void *state, std::uint32_t hash) {
    if (cache.count >= 256) {
        return false;
    }
    const std::uint8_t home = static_cast<std::uint8_t>(hash);
    std::uint8_t index = home;
    std::int32_t firstEmpty = -1;
    if (cache.hashes[index] == hash) {
        return true;
    }
    if (!cache.hashes[index] && !cache.probeOffsets[index]) {
        cache.hashes[index] = hash;
        cache.states[index] = state;
        ++cache.count;
        return true;
    }
    for (;;) {
        if (!cache.hashes[index] && firstEmpty < 0) {
            firstEmpty = index;
        }
        const std::uint16_t offset = cache.probeOffsets[index];
        if (!offset) {
            break;
        }
        index = static_cast<std::uint8_t>(index + offset);
        if (cache.hashes[index] == hash) {
            return true;
        }
    }
    if (firstEmpty >= 0) {
        index = static_cast<std::uint8_t>(firstEmpty);
    } else {
        std::uint8_t candidate = static_cast<std::uint8_t>(index + 1);
        while (candidate != index && cache.hashes[candidate]) {
            candidate = static_cast<std::uint8_t>(candidate + 1);
        }
        if (candidate == index) {
            return false;
        }
        cache.probeOffsets[index] = static_cast<std::uint8_t>(candidate - index);
        index = candidate;
    }
    cache.hashes[index] = hash;
    cache.states[index] = state;
    ++cache.count;
    return true;
}

std::uint32_t Hash16Bytes(const void *data) {
    const auto *bytes = static_cast<const std::uint8_t *>(data);
    std::uint32_t hash = 0x5eedU;
    for (std::size_t offset = 0; offset < 16; offset += 4) {
        hash = bytes[offset + 3] + 0x1003fU * bytes[offset + 2] + 0x2e86d0bfU * bytes[offset] + 0x7e0f81U * bytes[offset + 1] + 0x43ec5f01U * hash;
    }
    return hash;
}

#if defined(_WIN32)
D3D11_COMPARISON_FUNC Comparison(std::uint32_t type) {
    constexpr std::array<D3D11_COMPARISON_FUNC, 8> values{D3D11_COMPARISON_ALWAYS, D3D11_COMPARISON_NEVER, D3D11_COMPARISON_LESS, D3D11_COMPARISON_EQUAL,
        D3D11_COMPARISON_LESS_EQUAL, D3D11_COMPARISON_GREATER, D3D11_COMPARISON_NOT_EQUAL, D3D11_COMPARISON_GREATER_EQUAL};
    return type < values.size() ? values[type] : D3D11_COMPARISON_ALWAYS;
}

D3D11_STENCIL_OP StencilOp(std::uint32_t type) {
    constexpr std::array<D3D11_STENCIL_OP, 7> values{D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_INCR_SAT,
        D3D11_STENCIL_OP_DECR_SAT, D3D11_STENCIL_OP_INVERT, D3D11_STENCIL_OP_INCR};
    return type < values.size() ? values[type] : D3D11_STENCIL_OP_DECR;
}

D3D11_BLEND SourceBlend(std::uint32_t type) {
    constexpr std::array<D3D11_BLEND, 17> values{D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_DEST_COLOR, D3D11_BLEND_INV_DEST_COLOR,
        D3D11_BLEND_SRC_ALPHA_SAT, D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR, D3D11_BLEND_SRC1_COLOR, D3D11_BLEND_INV_SRC1_COLOR,
        D3D11_BLEND_SRC1_ALPHA, D3D11_BLEND_INV_SRC1_ALPHA};
    return type < values.size() ? values[type] : D3D11_BLEND_ZERO;
}

D3D11_BLEND DestinationBlend(std::uint32_t type) {
    switch (type) {
    case 1:
        return D3D11_BLEND_ONE;
    case 2:
    case 4:
        return D3D11_BLEND_SRC_ALPHA;
    case 3:
    case 5:
        return D3D11_BLEND_INV_SRC_ALPHA;
    case 6:
    case 8:
        return D3D11_BLEND_DEST_ALPHA;
    case 7:
    case 9:
        return D3D11_BLEND_INV_DEST_ALPHA;
    case 10:
        return D3D11_BLEND_SRC_ALPHA_SAT;
    case 11:
        return D3D11_BLEND_BLEND_FACTOR;
    case 12:
        return D3D11_BLEND_INV_BLEND_FACTOR;
    case 13:
    case 15:
        return D3D11_BLEND_SRC1_ALPHA;
    case 14:
    case 16:
        return D3D11_BLEND_INV_SRC1_ALPHA;
    default:
        return D3D11_BLEND_ZERO;
    }
}

D3D11_BLEND_OP BlendOperation(std::uint32_t type) {
    constexpr std::array<D3D11_BLEND_OP, 5> values{
        D3D11_BLEND_OP_ADD, D3D11_BLEND_OP_SUBTRACT, D3D11_BLEND_OP_REV_SUBTRACT, D3D11_BLEND_OP_MIN, D3D11_BLEND_OP_MAX};
    return type < values.size() ? values[type] : D3D11_BLEND_OP_ADD;
}

void *CreateSampler(const std::uint64_t *input) {
    const std::uint64_t flags = *input;
    D3D11_SAMPLER_DESC description{};
    const std::uint32_t mipFilter = (flags >> 4U) & 3U;
    std::uint32_t filter = mipFilter == 1U ? 1U : 0U;
    if ((flags & 3U) == 1U)
        filter |= 4U;
    if (((flags >> 2U) & 3U) == 1U)
        filter |= 0x10U;
    if (mipFilter == 2U)
        filter = D3D11_FILTER_ANISOTROPIC;
    const std::uint32_t comparison = (flags >> 10U) & 0xfU;
    if (comparison)
        filter |= 0x80U;
    description.Filter = static_cast<D3D11_FILTER>(filter);
    const auto address = [](std::uint32_t bits) {
        return bits == 1U ? D3D11_TEXTURE_ADDRESS_MIRROR : bits == 2U ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
    };
    description.AddressU = address((flags >> 6U) & 3U);
    description.AddressV = address((flags >> 8U) & 3U);
    description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    const std::uint32_t mipBiasBits = static_cast<std::uint32_t>(flags >> 18U);
    description.MipLODBias = std::bit_cast<float>(mipBiasBits);
    description.MaxAnisotropy = static_cast<UINT>(((flags >> 14U) & 0xfU) + 1U);
    description.ComparisonFunc = Comparison(comparison);
    description.MaxLOD = std::numeric_limits<float>::max();
    ID3D11SamplerState *state = nullptr;
    auto *device = static_cast<ID3D11Device *>(g_D3DDevice);
    if (device)
        device->CreateSamplerState(&description, &state);
    return state;
}

void *CreateBlend(const std::byte *input) {
    D3D11_BLEND_DESC description{};
    const auto *words = reinterpret_cast<const std::uint64_t *>(input);
    description.IndependentBlendEnable = std::to_integer<std::uint8_t>(input[64]) != 0;
    description.AlphaToCoverageEnable = std::to_integer<std::uint8_t>(input[65]) != 0;
    const std::uint32_t count = description.IndependentBlendEnable ? 8U : 1U;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint32_t flags = static_cast<std::uint32_t>(words[i]);
        auto &target = description.RenderTarget[i];
        target.RenderTargetWriteMask = static_cast<UINT8>((flags >> 28U) & 0xfU);
        target.BlendEnable = (flags & 1U) != 0;
        if (!target.BlendEnable)
            continue;
        const std::uint32_t source = (flags >> 2U) & 0x1fU;
        const std::uint32_t destination = (flags >> 7U) & 0x1fU;
        target.SrcBlend = SourceBlend(source);
        target.DestBlend = SourceBlend(destination);
        target.BlendOp = BlendOperation((flags >> 12U) & 7U);
        if (flags & 2U) {
            target.SrcBlendAlpha = SourceBlend((flags >> 15U) & 0x1fU);
            target.DestBlendAlpha = SourceBlend((flags >> 20U) & 0x1fU);
            target.BlendOpAlpha = BlendOperation((flags >> 25U) & 7U);
        } else {
            target.SrcBlendAlpha = DestinationBlend(source);
            target.DestBlendAlpha = DestinationBlend(destination);
            target.BlendOpAlpha = target.BlendOp;
        }
    }
    ID3D11BlendState *state = nullptr;
    auto *device = static_cast<ID3D11Device *>(g_D3DDevice);
    if (device)
        device->CreateBlendState(&description, &state);
    return state;
}

void *CreateDepthStencil(const std::uint64_t *input) {
    const std::uint64_t flags = input[0];
    D3D11_DEPTH_STENCIL_DESC description{};
    description.DepthEnable = (flags & 1U) != 0;
    description.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>((flags >> 1U) & 1U);
    description.DepthFunc = Comparison(static_cast<std::uint32_t>((flags >> 2U) & 0xfU));
    description.StencilEnable = ((flags >> 6U) & 1U) != 0;
    description.StencilReadMask = static_cast<UINT8>(flags >> 11U);
    description.StencilWriteMask = static_cast<UINT8>(flags >> 19U);
    description.FrontFace.StencilFunc = Comparison(static_cast<std::uint32_t>((flags >> 7U) & 0xfU));
    description.FrontFace.StencilFailOp = StencilOp(static_cast<std::uint32_t>((flags >> 31U) & 0xfU));
    description.FrontFace.StencilDepthFailOp = StencilOp(static_cast<std::uint32_t>((flags >> 27U) & 0xfU));
    description.FrontFace.StencilPassOp = StencilOp(static_cast<std::uint32_t>((flags >> 35U) & 0xfU));
    description.BackFace = description.FrontFace;
    ID3D11DepthStencilState *state = nullptr;
    auto *device = static_cast<ID3D11Device *>(g_D3DDevice);
    if (device)
        device->CreateDepthStencilState(&description, &state);
    return state;
}

void *CreateRasterizer(const std::uint32_t *input) {
    const std::uint32_t flags = *input;
    D3D11_RASTERIZER_DESC description{};
    description.FillMode = (flags & 2U) ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
    const std::uint32_t cull = (flags >> 3U) & 3U;
    description.CullMode = cull == 1U ? D3D11_CULL_FRONT : cull == 2U ? D3D11_CULL_BACK : D3D11_CULL_NONE;
    description.AntialiasedLineEnable = TRUE;
    description.DepthClipEnable = (flags & 1U) != 0;
    description.ScissorEnable = ((flags >> 2U) & 1U) != 0;
    description.FrontCounterClockwise = ((flags >> 13U) & 1U) != 0;


    description.DepthBias = static_cast<std::int16_t>(flags >> 16U);
    ID3D11RasterizerState *state = nullptr;
    auto *device = static_cast<ID3D11Device *>(g_D3DDevice);
    if (device)
        device->CreateRasterizerState(&description, &state);
    return state;
}
#endif

std::uint32_t BlendHash(const std::byte *input) {
    const auto *bytes = reinterpret_cast<const std::uint8_t *>(input);
    const bool independent = bytes[64] != 0;
    std::uint32_t hash = ((bytes[65] & 1U) | (2U * (bytes[64] & 1U))) << 7U;
    const std::uint32_t count = independent ? 8U : 1U;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint8_t *p = bytes + i * 8U;
        const std::uint32_t value = p[7] + 0x162c613fU * p[2] + 0x2e86d0bfU * p[4] + 0x1003fU * p[6] + 0x7e0f81U * p[5] + 0xd62aee81U * p[1] -
            0x5cee4e41U * p[0] - 0x1d66bb13U + 0x43ec5f01U * p[3];
        hash = ((value + hash) >> 16U) + ((value + hash) >> 6U) - i;
    }
    return hash;
}

template <typename T, typename Create> void *Retrieve(NativeStateCache<void> &cache, std::uint32_t hash, Create &&create) {
    T *state = nullptr;
    if (FindState(cache, hash, state)) {
        return state;
    }
    void *const created = create();
    if (created)
        InsertState(cache, created, hash);
    return created;
}

} // namespace

void *RetrieveHashedSamplerState(const std::uint64_t *flags) {
    const std::uint32_t hash = Hash16Bytes(flags);
#if defined(_WIN32)
    return Retrieve<ID3D11SamplerState>(g_samplerStates, hash, [flags] { return CreateSampler(flags); });
#else
    static_cast<void>(flags);
    static_cast<void>(hash);
    return nullptr;
#endif
}

void *RetrieveHashedBlendState(const std::byte *flags) {
    const std::uint32_t hash = BlendHash(flags);
#if defined(_WIN32)
    return Retrieve<ID3D11BlendState>(g_blendStates, hash, [flags] { return CreateBlend(flags); });
#else
    static_cast<void>(flags);
    static_cast<void>(hash);
    return nullptr;
#endif
}

void *RetrieveHashedStencilState(const std::uint64_t *flags) {
    const std::uint32_t hash = Hash16Bytes(flags);
#if defined(_WIN32)
    return Retrieve<ID3D11DepthStencilState>(g_depthStencilStates, hash, [flags] { return CreateDepthStencil(flags); });
#else
    static_cast<void>(flags);
    static_cast<void>(hash);
    return nullptr;
#endif
}

void *RetrieveHashedRasterizerState(const std::uint32_t *flags) {
    const auto *bytes = reinterpret_cast<const std::uint8_t *>(flags);
    const std::uint32_t hash = 0x2e86d0bfU * bytes[0] + 0x1003fU * bytes[2] + 0x7e0f81U * bytes[1] + bytes[3] - 0x5349ae13U;
#if defined(_WIN32)
    return Retrieve<ID3D11RasterizerState>(g_rasterizerStates, hash, [flags] { return CreateRasterizer(flags); });
#else
    static_cast<void>(flags);
    static_cast<void>(hash);
    return nullptr;
#endif
}

void ReleaseD3TPipelineStateCaches() {
#if defined(_WIN32)
    auto release = [](NativeStateCache<void> &cache) {
        for (void *&state : cache.states) {
            if (state)
                static_cast<ID3D11DeviceChild *>(state)->Release();
            state = nullptr;
        }
        cache = {};
    };
    release(g_samplerStates);
    release(g_blendStates);
    release(g_depthStencilStates);
    release(g_rasterizerStates);
#endif
}

} // namespace shenmue::d3t
