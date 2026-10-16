#include "s2/surface_wave.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_assets.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
namespace shenmue::s2 {
namespace {
constexpr float twoPi = 6.2831855f;
struct SurfaceParameters {
    float wavelength;
    Vector3 motion;
    float bias;
    float scale;
    Vector4 direction;
    std::int32_t rows;
    std::int32_t columns;
    float baseHeight;
    float reserved34;
    std::uint64_t reserved38;
};
static_assert(sizeof(SurfaceParameters) == 0x40);
static_assert(offsetof(SurfaceParameters, motion) == 4);
static_assert(offsetof(SurfaceParameters, bias) == 0x10);
static_assert(offsetof(SurfaceParameters, baseHeight) == 0x30);
SurfaceParameters parameters{};
std::int32_t frame{}, slot{};
float heightOffset{}, phase{}, phaseOffset{};
std::array<MT7Node *, 16> nodes{};
std::array<Vector3, 17 * 257> grid{};
std::array<LegacyWaveVertex, 17 * 257> lighting{};
float SampleWaveHeightOffset(float x, float z) {
    const float number = twoPi / parameters.wavelength;
    const float angle = ((phase * twoPi) * parameters.motion.x) + phaseOffset;
    const float first = (((parameters.bias * z) - (parameters.scale * x)) * number) + angle;
    const float second = (((parameters.scale * z) + (parameters.bias * x)) * number) + angle;
    const float a = cos_fixed16(static_cast<std::uint16_t>(TruncateToInt32(((second * 65536.0f) / twoPi) + 0.5f)));
    const float b = cos_fixed16(static_cast<std::uint16_t>(TruncateToInt32(((first * 65536.0f) / twoPi) + 0.5f)));
    return ((((((0.8399999737739563f * a) + 0.41999998688697815f) * a - 0.4699999988079071f) * a) - 0.20999999344348907f) +
               ((((0.3400000035762787f * b) + 0.14000000059604645f) * b - 0.1850000023841858f) * b - 0.07000000029802322f)) *
        parameters.motion.y;
}

template <class F> void WalkSurfaceVertices(float *vertices, F &&visit) {
    const auto emit = [&](std::size_t point, std::size_t normal) {
        visit(vertices, point, normal);
        vertices += 8;
    };
    for (std::size_t c = 0; c <= 256; ++c) {
        emit(7 * 257 + 256 - c, 7 * 257 + 16 - (c & 15));
        emit(8 * 257 + 256 - c, 8 * 257 + 16 - (c & 15));
    }
    for (std::size_t r = 1; r <= 6; ++r) {
        std::size_t cycle = 0;
        for (std::size_t c = 0; c <= 256; ++c) {
            emit((8 + r) * 257 + c, (8 + r) * 257 + cycle);
            cycle = cycle < 16 ? cycle + 1 : 1;
        }
        for (std::size_t c = 0; c <= 256; ++c)
            emit((7 - r) * 257 + 256 - c, (7 - r) * 257 + 16 - (c & 15));
    }
    for (std::size_t r = 15; r <= 16; ++r) {
        std::size_t cycle = 0;
        for (std::size_t c = 0; c <= 256; ++c) {
            emit(r * 257 + c, r * 257 + cycle);
            cycle = cycle < 16 ? cycle + 1 : 1;
        }
    }
    std::size_t cycle = 0;
    for (std::size_t c = 0; c <= 256; ++c) {
        emit(c, cycle);
        cycle = cycle < 16 ? cycle + 1 : 1;
    }
}
template <class F> void VisitSurfaceBuffers(MT7Node &node, F &&visit, bool rewriteParameters = false) {
    std::uint32_t *const mesh = reinterpret_cast<std::uint32_t *>(node.runtimeMesh);
    auto *packet = mesh + mesh[6] + 6;
    std::uint32_t buffer = 0;
    while (*packet) {
        auto *command = packet + 5;
        while (*command & 0xffU) {
            if (*command == 1)
                buffer = command[8];
            else if (rewriteParameters && *command == 13) {
                auto *values = reinterpret_cast<std::uint8_t *>(command + 10);
                for (std::uint32_t i = 0; i < command[1]; ++i, values += 4)
                    if ((values[-1] == 0 || values[-1] == 1) && values[0] == 3)
                        values[0] = 1;
            }
            command += *command >> 8U;
        }
        auto *vertices = static_cast<float *>(CopyForDirectX(mesh, buffer, 0, 0, 0));
        visit(vertices);
        CopyBufferToDirectX_Wrapper(mesh, buffer);
        packet += *packet >> 8U;
    }
}
// @verified - p
void ReadSurfaceGrid(MT7Node &node) {
    VisitSurfaceBuffers(node, [&](float *vertices) {
        WalkSurfaceVertices(vertices, [&](float *vertex, std::size_t point, std::size_t) {
            grid[point].x = node.position.x + vertex[0];
            grid[point].z = node.position.z + vertex[2];
        });
    });
}
std::uint32_t TruncatePackedComponent(double value) {
    if (!(value >= -9223372036854775808.0 && value < 9223372036854775808.0))
        return static_cast<std::uint32_t>(std::numeric_limits<std::int64_t>::min());
    return static_cast<std::uint32_t>(static_cast<std::int64_t>(value));
}
std::uint32_t PackColour(float red, float green, float blue) {
    return 0xff000000U | (TruncatePackedComponent(red) << 16U) | (TruncatePackedComponent(green) << 8U) | TruncatePackedComponent(blue);
}
// @verified - p
void BuildSurfaceLightingTable() {
    const WaveLightingView view = GetWaveLightingView();
    const auto sample = [&](std::size_t i) { return i < 289 ? view.real[i] : view.imaginary[i - 289]; };
    const float ambient = view.lightAmbient * view.ambient;
    for (std::size_t row = 0; row < 16; ++row) {
        const float rowStart = static_cast<float>(row) * view.cellSize;
        const float dz = (rowStart + view.cellSize) - rowStart;
        for (std::size_t column = 0; column < 256; ++column) {
            const std::size_t i = row * 17 + column;
            const float x = static_cast<float>(column) * view.cellSize;
            const float dx = (x + view.cellSize) - x;
            const float height = sample(i);
            const float right = sample(i + 1) - height;
            const float down = sample(i + 17) - height;
            Vector3 normal{-dz * right, dz * dx, -down * dx};
            Vector3::NormalizeInPlace(normal);
            const float dot = (normal.x * view.lightDirection.x + normal.y * view.lightDirection.y) + normal.z * view.lightDirection.z;
            const float shade = std::fmin(1.0f, dot > 0 ? dot + ambient : ambient) * 255.0f;
            auto &value = lighting[row * 257 + column];
            value.height = height;
            value.normal = TruncatePackedComponent(static_cast<double>(normal.z) * 127.0) |
                ((TruncatePackedComponent(static_cast<double>(normal.y) * 127.0) | (TruncatePackedComponent(static_cast<double>(normal.x) * 127.0) << 8U))
                    << 8U);
            value.diffuse = PackColour(shade * view.diffuse.x, shade * view.diffuse.y, shade * view.diffuse.z);
            value.specular = 0;
            if (view.flags & 1U) {
                const float specular = std::fmin(1.0f, dot > 0 ? fast_powf_approx(dot, view.specularExponent) : 0.0f) * (view.specularIntensity * 255.0f);
                value.specular = PackColour(specular * view.specular.x, specular * view.specular.y, specular * view.specular.z);
            }
        }
        lighting[row * 257 + 256] = lighting[row * 257];
    }
    for (std::size_t column = 0; column < 257; ++column)
        lighting[16 * 257 + column] = lighting[column];
}
// @verified - p
void DeformSurfaceMesh(MT7Node &node) {
    std::uint32_t *const mesh = reinterpret_cast<std::uint32_t *>(node.runtimeMesh);
    const WaveLightingView view = GetWaveLightingView();
    float u = view.scrollU, v = view.scrollV;
    if (mesh[1] & 0x200U)
        u += (mesh[1] & 0x100U) ? 1.0f : -1.0f;
    if (mesh[1] & 0x2000U)
        v += (mesh[1] & 0x1000U) ? 1.0f : -1.0f;
    reinterpret_cast<std::uint8_t *>(mesh)[5] = 0;
    std::uint32_t uRange = 0, vRange = 0;
    const auto range = [](float value) { return value < 0 ? 256U : value < 1 ? 16U : 1U; };
    VisitSurfaceBuffers(
        node,
        [&](float *vertices) {
            WalkSurfaceVertices(vertices, [&](float *vertex, std::size_t point, std::size_t normal) {
                vertex[1] = grid[point].y;
                std::memcpy(vertex + 3, &lighting[normal].normal, 4);
                std::memcpy(vertex + 4, &lighting[normal].diffuse, 4);
                std::memcpy(vertex + 5, &lighting[normal].specular, 4);
                vertex[6] += u;
                vertex[7] += v;
                uRange |= range(vertex[6]);
                vRange |= range(vertex[7]);
                const std::uint32_t odd = std::bit_cast<std::uint32_t>(vertex[7]) | 1U;
                vertex[7] = std::bit_cast<float>(odd);
            });
        },
        true);
    if (uRange == 256)
        mesh[1] |= 0x300U;
    else if (uRange == 1)
        mesh[1] |= 0x200U;
    if (vRange == 256)
        mesh[1] |= 0x3000U;
    else if (vRange == 1)
        mesh[1] |= 0x2000U;
}
// @verified - p
void UpdateSurfaceGridHeightRange(std::int32_t start, std::int32_t end) {
    for (std::size_t row = 0; row < 17; ++row)
        for (std::int32_t c = start; c <= end; ++c) {
            Vector3 &point = grid[row * 257 + static_cast<std::size_t>(c)];
            point.y = parameters.baseHeight + SampleWaveHeightOffset(point.x, point.z);
        }
}
}
// @verified - p
void ConfigureSurfaceWaveModel(std::int32_t modelSlot, float offset) {
    heightOffset = offset;
    nodes[0] = nullptr;
    std::memset(reinterpret_cast<std::byte *>(nodes.data()) + 8, 0, 4);
    frame = 0;
    slot = modelSlot;
    MT7Node *node = GetMapModelRoot(slot)->runtimeChild;
    for (std::int32_t count = 0; node && count < 16; ++count, node = node->runtimeSibling)
        nodes[(static_cast<std::int16_t>(node->id) - 152) % 16] = node;
    ReadSurfaceGrid(*nodes[15]);
}
// @verified - p
void InitializeSurfaceWaveParameters() {
    parameters = {32.558998107910156f, {0.23999999463558197f, 1.4800000190734863f, 0.03333333507180214f}, -0.8500000238418579f, 1.149999976158142f,
        {0, 0, 1, 1}, 16, 256, 0, 0, 0};
    phase = phaseOffset = 0;
}
// @verified - p
void AdvanceSurfacePhaseAndUpdateGrid(std::int32_t sceneFrame) {
    phase += parameters.motion.z;
    if (parameters.motion.x * phase > 2400.0f)
        phase -= 2400.0f / parameters.motion.x;
    if (sceneFrame >= 1850)
        UpdateSurfaceGridHeightRange(0, 128);
    else
        UpdateSurfaceGridHeightRange(48, 256);
}
// @verified - p
float GetSurfaceHeightAt(float x, float z) {
    return (parameters.baseHeight + SampleWaveHeightOffset(x, z)) + 97.99500274658203f;
}
// @verified - p
std::uint32_t DrawSurfaceWaveModel() {
    frame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(frame) + 1U);
    SetMapModelSlotEnabled(slot, 0);
    MT7Node *node = nodes[frame % 16];
    if (!node)
        return 0;
    const std::int32_t previous = GetLegacyVertexColourType();
    SetLegacyVertexColourType(1);
    BuildSurfaceLightingTable();
    if (node->visible && (node->runtimeMesh || node->runtimeChild)) {
        Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
        if (node->runtimeMesh)
            DeformSurfaceMesh(*node);
        Matrix4x4::Pop(1);
    }
    Matrix4x4_Push();
    Matrix4x4::preTranslate(node->position.x, heightOffset + node->position.y, node->position.z);
    MT7Model *model = const_cast<MT7Model *>(GetMapModelSlots().data()[slot].reader);
    const std::uint32_t draws = ExecuteLegacyDrawCommandStream(
        model->source, node->xb01Offset, node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources, LegacyModelShaderPath::Lambert, 1.0f);
    Matrix4x4::Pop(1);
    SetLegacyVertexColourType(previous);
    return draws;
}
}
