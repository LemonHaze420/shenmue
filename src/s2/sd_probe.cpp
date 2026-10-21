#include "s2/sd_probe.hpp"

#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"

#include "d3t/model_cache.hpp"
#include <bit>
#include <cmath>
#include <cstring>

    namespace shenmue::s2 {

std::array<SDProbeRecord, 32> g_sdProbes{};
std::int32_t g_sdProbeCount{};

std::uint32_t g_sdProbeBuffer{};
std::int32_t g_sdGlobalProbeIndex{};
std::array<std::uint32_t, 2> g_sdProbeTextures{};
std::uint32_t g_sdGlobalProbeTexture{};

// @verified - p
void UpdateModelSceneProbes(d3t::D3TDeviceContext * context) {
#if defined(_WIN32)
    SDProbeConstants *const constants = static_cast<SDProbeConstants *>(d3t::MapTempConstantBufferForWriteDiscard(context, 2, 11, 12));
    if (constants)
        *constants = {.disableLocalProbes = 0, .probeMapPass = 0, .globalProbeIndex = g_sdGlobalProbeIndex};
#endif
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.bindings.bufferHandles[1][4] != g_sdProbeBuffer) {
        state.shaderResourceDirtyMasks[1] |= 0x10U;
        state.bindings.bufferHandles[1][4] = g_sdProbeBuffer;
    }
    const std::uint32_t flags[2]{d3t::IsTextureHandleValid(g_sdProbeTextures[0]), d3t::IsTextureHandleValid(g_sdProbeTextures[1])};
    for (std::uint32_t index = 0; index < 3; ++index) {
        const std::uint32_t slot = 24U + index;
        const std::uint32_t texture = index < 2 ? g_sdProbeTextures[index] : g_sdGlobalProbeTexture;
        const std::uint32_t flag = index < 2 ? flags[index] : 0;
        if (state.bindings.textureHandles[1][slot] != texture || state.bindings.textureFlags[1][slot] != flag) {
            state.shaderResourceDirtyMasks[1] |= 1U << slot;
            state.bindings.textureHandles[1][slot] = texture;
            state.bindings.textureFlags[1][slot] = flag;
        }
    }
    for (std::uint32_t index = 0; index < 2; ++index) {
        const std::uint32_t slot = index ? 10U : 2U;
        const std::uint32_t flags = index ? 640U : 661U;
        d3t::D3TSamplerStateKey &sampler = state.bindings.samplers[1][slot];
        if ((sampler.flags[0] | (static_cast<std::uint64_t>(sampler.flags[1]) << 32U)) != flags) {
            state.samplerDirtyMasks[1] |= static_cast<std::uint16_t>(1U << slot);
            sampler = {.flags = {flags, 0}, .reserved = {}};
        }
    }
}
namespace {
// @verified - p
Matrix4x4 BuildSDProbeViewMatrix(const Vector3 &eye, const Vector3 &target, const Vector3 &up) {
    Vector3 forward{target.x - eye.x, target.y - eye.y, target.z - eye.z};
    const float distance = std::sqrt((forward.y * forward.y + forward.x * forward.x) + forward.z * forward.z);
    if (distance > 0.0f) {
        forward.x *= 1.0f / distance;
        forward.y *= 1.0f / distance;
        forward.z *= 1.0f / distance;
    }
    Vector3 right{up.y * forward.z - up.z * forward.y, up.z * forward.x - up.x * forward.z, up.x * forward.y - up.y * forward.x};
    const float magnitude = std::sqrt((right.y * right.y + right.x * right.x) + right.z * right.z);
    if (magnitude > 0.0f) {
        right.x *= 1.0f / magnitude;
        right.y *= 1.0f / magnitude;
        right.z *= 1.0f / magnitude;
    }
    const Vector3 vertical{right.z * forward.y - right.y * forward.z, right.x * forward.z - right.z * forward.x, right.y * forward.x - right.x * forward.y};
    return {{{right.x, right.y, right.z, -((eye.y * right.y + eye.x * right.x) + eye.z * right.z)},
        {vertical.x, vertical.y, vertical.z, -((eye.y * vertical.y + eye.x * vertical.x) + eye.z * vertical.z)},
        {forward.x, forward.y, forward.z, -((eye.y * forward.y + eye.x * forward.x) + eye.z * forward.z)}, {0, 0, 0, 1}}};
}

// @verified - p
void ExtractSDProbeFrustum(std::array<Vector4, 6> &planes, const Matrix4x4 &matrix) {
    const Vector4 x = matrix.rows[0], y = matrix.rows[1], z = matrix.rows[2], w = matrix.rows[3];
    planes = {{z, {w.x - z.x, w.y - z.y, w.z - z.z, w.w - z.w}, {x.x + w.x, x.y + w.y, x.z + w.z, x.w + w.w}, {w.x - x.x, w.y - x.y, w.z - x.z, w.w - x.w},
        {w.x - y.x, w.y - y.y, w.z - y.z, w.w - y.w}, {y.x + w.x, y.y + w.y, y.z + w.z, y.w + w.w}}};
    for (Vector4 &plane : planes) {
        const float inverse = 1.0f / std::sqrt(((plane.x * plane.x + plane.y * plane.y) + plane.z * plane.z) + plane.w * plane.w);
        plane = {plane.x * inverse, plane.y * inverse, plane.z * inverse, plane.w * inverse};
    }
}
} // namespace

// @verified - p
void UpdateSDProbeFrustums(std::uint32_t index) {
    SDProbeRecord &probe = g_sdProbes[index];
    const Matrix4x4 world = d3t::ComputeModelCacheGlobalTransform(probe.node);
    const Vector3 eye{world.rows[0].w, world.rows[1].w, world.rows[2].w};
    const float scale = 1.0f / std::tan(SD_PROBE_HALF_FOV);
    const std::array<Vector3, 6> targets{{{eye.x - 1.0f, eye.y, eye.z}, {eye.x + 1.0f, eye.y, eye.z}, {eye.x, eye.y - 1.0f, eye.z},
        {eye.x, eye.y + 1.0f, eye.z}, {eye.x, eye.y, eye.z + 1.0f}, {eye.x, eye.y, eye.z - 1.0f}}};
    Matrix4x4 projection{{{scale, 0, 0, 0}, {0, scale, 0, 0}, SD_PROBE_FRUSTUM_DEPTH, SD_PROBE_PROJECTION_W}};
    for (std::uint32_t face = 0; face < 6; ++face) {
        probe.views[face] = BuildSDProbeViewMatrix(eye, targets[face], SD_PROBE_UP[face]);
        probe.projections[face] = {{{scale, 0, 0, 0}, {0, scale, 0, 0}, SD_PROBE_PROJECTION_DEPTH, SD_PROBE_PROJECTION_W}};
        const Matrix4x4 &view = probe.views[face];
        Matrix4x4 viewProjection;
        for (std::uint32_t row = 0; row < 4; ++row)
            for (std::uint32_t column = 0; column < 4; ++column)
                viewProjection.rows[row][column] = ((projection.rows[row].x * view.rows[0][column] + projection.rows[row].y * view.rows[1][column]) +
                                                       projection.rows[row].z * view.rows[2][column]) +
                    projection.rows[row].w * view.rows[3][column];
        ExtractSDProbeFrustum(probe.frustums[face], viewProjection);
    }
}

// @verified - p
void ConfigureSDProbe(std::uint32_t index, std::uint64_t node, const D3TModelProbe *parameters, const void *owner, std::uint64_t root) {
    SDProbeRecord &probe = g_sdProbes[index];
    probe.node = node;
    if (owner) {
        probe.root = root;
        probe.owner = owner;
    }
    if (parameters) {
        probe.parameter580 = parameters->parameter00;
        probe.parameter584 = parameters->parameter04;
        probe.parameter548 = parameters->parameter08;
        probe.parameter550 = parameters->parameter10;
        probe.parameter554 = parameters->parameter14;
        probe.parameter55c = parameters->parameter1c;
        probe.parameter570 = parameters->parameter2c;
        probe.parameter574[0] = parameters->parameter20[0];
        probe.parameter574[1] = parameters->parameter20[1];
        probe.parameter57c = parameters->parameter28;
        probe.parameter58c = parameters->parameter30;
    }
    UpdateSDProbeFrustums(index);
}

// @verified - p
void ConfigureOwnerSDProbes(std::int32_t count, const std::uint64_t *nodes, const D3TModelProbe *parameters, const void *owner, std::uint64_t root) {
    if (!g_sdProbeCount) {
        SDProbeRecord &global = g_sdProbes[0];
        global.node = 0;
        global.parameter548 = 0;
        global.parameter550 = 0;
        global.parameter554 = 0;
        global.parameter55c = 0;
        global.parameter574[0] = global.parameter574[1] = 1.0f;
        global.parameter580 = global.parameter584 = 99999.0f;
        global.parameter570 = 0;
        global.parameter57c = 1.0f;
        global.parameter58c = 0;
        UpdateSDProbeFrustums(0);
        ++g_sdProbeCount;
    }
    if (std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_sdProbeCount) + static_cast<std::uint32_t>(count)) < 32) {
        std::int32_t index = g_sdProbeCount;
        while (index < std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_sdProbeCount) + static_cast<std::uint32_t>(count))) {
            if (g_sdProbeCount >= 32)
                return;
            ConfigureSDProbe(index, nodes[index - g_sdProbeCount], &parameters[index - g_sdProbeCount], owner, root);
            ++index;
        }
        g_sdProbeCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_sdProbeCount) + static_cast<std::uint32_t>(count));
    }
}

// @verified - p
void RemoveOwnerSDProbes(const void *owner) {
    std::int32_t index = 0;
    while (index < g_sdProbeCount) {
        SDProbeRecord &probe = g_sdProbes[index];
        if (probe.owner == owner) {
            if (index < g_sdProbeCount - 1) {
                SDProbeRecord removed;
                std::memcpy(&removed, &probe, sizeof(removed));
                std::memmove(&probe, &g_sdProbes[index + 1], sizeof(probe) * (g_sdProbeCount - index - 1));
                std::memcpy(&g_sdProbes[g_sdProbeCount - 1], &removed, sizeof(removed));
            }
            --g_sdProbeCount;
            g_sdProbes[g_sdProbeCount].owner = nullptr;
        } else {
            ++index;
        }
    }
}

} // namespace shenmue::s2
