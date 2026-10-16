#include "s2/lighting.hpp"

#include "d3t/buffer.hpp"
#include "d3t/object_store.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/magic_room.hpp"
#include "s2/map_assets.hpp"
#include "s2/mt7.hpp"
#include "s2/map_info.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/world_render.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/weather.hpp"
#include "s2/search.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/dx11_renderer.hpp"

#include "common/math.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace shenmue::s2 {
WeatherSunState g_weatherSunState{};
Vector3 &g_sunDirectionVector = g_weatherSunState.direction;
float &g_flareIntensity = g_weatherSunState.sourceIntensity;
std::int32_t &g_hasComputedSun = g_weatherSunState.explicitDirection;
namespace {

constexpr std::uint32_t lghtToken = CHUNK_LGHT.name;
std::array<MT7Model, 14> weatherLensModels;

LightRuntimeEntry defaultLight{
    .indexes = {},
    .unknown02 = 1820,
    .vectors = {Vector3{1.0f, 1.0f, 1.0f}, Vector3{1.0f, 1.0f, 2.0f}},
    .value1c = 0.60000002f,
    .vector20 = {0.0f, 2.0f, 0.0f},
    .values2c = {0.0f, -1.0f},
    .value34 = 0.0f,
};

std::array<LightDefinitionEntry, 24> lightDefinitions{};
std::array<LightRuntimeEntry, 256> lightRuntimeEntries{};
const std::byte *lightChunkEnd = nullptr;

std::array<const std::byte *, 24> lightDefinitionEnds{};
Vector4 globalLighting{0.3f, 0.3f, 0.3f, 0.3f};
Vector4 previousGlobalLighting{};
Vector3 lightRootPosition{};
float weatherLighting = std::bit_cast<float>(0xffffffffU);
float weatherSceneLighting = std::bit_cast<float>(0xffffffffU);
std::array<std::int16_t, 16> selectedLightIndices{};
std::array<std::int16_t, 256> characterLightCandidates{};
std::size_t characterLightCandidateCount{};
std::int32_t previousActiveLight = 1;
std::array<std::int16_t, 5> priorityLightIndices{};
std::size_t priorityLightCount = 0;
std::uint32_t rankedLightLimit = 12;
std::int32_t currentActiveLight = 1;
bool rankMapLights = false;
std::array<MapLightingPlacement, 33> mapLightingPlacements{};
Vector3 *savedLightPositions{};
const Matrix4x4 *currentLightTransform{};

template <typename T> bool Read(const std::byte *address, T &value) {
    if (!address || !lightChunkEnd || address > lightChunkEnd || static_cast<std::size_t>(lightChunkEnd - address) < sizeof(T)) {
        return false;
    }
    std::memcpy(&value, address, sizeof(value));
    return true;
}

float F16toF32(std::int16_t value) {
    if (value == 0)
        return 0.0f;
    std::uint32_t bits = static_cast<std::uint16_t>(value);
    if (value < 0) {
        bits = (bits & 0x7fffU) | 0xfffc0000U;
    }
    bits = (bits << 13U) + 0x38000000U;
    return std::bit_cast<float>(bits);
}

void PublishGlobalLighting() {
    SetLegacyGlobalLighting(globalLighting);
}

// @verified - p
std::uint64_t GetGeometryLightColour(Vector4 *colour) {
    *colour = globalLighting;
    SetLegacyGlobalLighting(*colour);
    colour->y *= 1.4f;
    colour->z *= 1.4f;
    colour->w *= 1.4f;
    return 0;
}

void AllocateGeometryLightPositionScratch() {
    savedLightPositions = static_cast<Vector3 *>(HLib::AllocateTaskData(256 * sizeof(Vector3), TASK_LGTP));
    if (!savedLightPositions)
        return;
    for (std::size_t index = 0; index < lightRuntimeEntries.size(); ++index) {
        LightRuntimeEntry &light = lightRuntimeEntries[index];
        if ((light.indexes[1] & 0xf7U) == 4)
            continue;
        savedLightPositions[index] = light.vector20;
        light.vector20.x += lightRootPosition.x;
        light.vector20.y += lightRootPosition.y;
        light.vector20.z += lightRootPosition.z;
    }
}

// @verified - p
void RestoreGeometryLightPositionScratch() {
    if (!savedLightPositions)
        return;
    for (std::size_t index = 0; index < lightRuntimeEntries.size(); ++index)
        if ((lightRuntimeEntries[index].indexes[1] & 0xf7U) != 4)
            lightRuntimeEntries[index].vector20 = savedLightPositions[index];
    HLib::FreeTaskData(savedLightPositions);
}

void InitializeDefaultDirectionalLight() {
    LightRuntimeEntry &light = lightRuntimeEntries[0];
    ResetLightRuntimeEntry(0);
    light.indexes = {1, 2};
}


void LightModeNullsub(std::intptr_t) {
}

} // namespace


// @verified - p
void ImmediateLightWork(void *) {
    currentActiveLight = previousActiveLight;
    LightModeNullsub(currentActiveLight == 1 ? 0 : 1);
    UpdateGlobalLighting(previousGlobalLighting);
    for (std::uint32_t slot = 0; slot < selectedLightIndices.size(); ++slot) {
        const std::int16_t index = selectedLightIndices[slot];
        LightRuntimeEntry &light = index >= 0 ? lightRuntimeEntries[static_cast<std::size_t>(index)] : defaultLight;
        SetLegacyLight(slot, light);
    }
    UpdateLegacyLightTransforms();
}
// @verified - p
void AssignMagicRoomLighting(const Vector3 &origin, float radius) {
    currentActiveLight = previousActiveLight;
    LightModeNullsub(currentActiveLight == 1 ? 0 : 1);
    UpdateGlobalLighting(previousGlobalLighting);
    for (std::uint32_t slot = 0; slot < selectedLightIndices.size(); ++slot) {
        const std::int16_t index = selectedLightIndices[slot];
        LightRuntimeEntry *light = index >= 0 ? &lightRuntimeEntries[static_cast<std::size_t>(index)] : &defaultLight;
        const std::uint32_t kind = light->indexes[1] & 0xf7U;
        if (kind - 3U <= 1U) {
            Vector3 position = light->vector20;
            if (kind == 3U) {
                position.x += lightRootPosition.x;
                position.y += lightRootPosition.y;
                position.z += lightRootPosition.z;
            }
            const float x = position.x - origin.x, y = position.y - origin.y, z = position.z - origin.z;
            if (!(std::sqrt(x * x + y * y + z * z) <= radius))
                light = &defaultLight;
        }
        SetLegacyLight(slot, *light);
    }
    UpdateLegacyLightTransforms();
}

// @verified - p
void AssignLightParametersForMagicRoomEntity(std::uint32_t index, const MagicRoomEntityLight &light) {
    if (!light.enabled) {
        SetLightEntryIndex(index, 0);
        return;
    }
    SetLightEntryIndex(index, 1);
    SetLightFlagForIndex(index, light.flags);
    SetLightRuntimeColour(index, light.colour);
    SetLightSceneIntensity(index, light.sceneIntensity);
    SetLightRange(index, light.range * 1.2f);
    SetLightRuntimePosition(index, light.position);
    (void)SetLightSunDirection(index, light.direction);
    SetLightConeAngle(index, static_cast<std::int16_t>(light.coneAngle));
}

bool BeginTemporaryGlobalLighting(const Vector4 &colour, TemporaryGlobalLightingState &saved) {
    if (saved.engaged)
        return false;
    saved.colour = globalLighting;
    saved.engaged = true;
    globalLighting = colour;
    PublishGlobalLighting();
    return true;
}

void EndTemporaryGlobalLighting(TemporaryGlobalLightingState &saved) {
    if (!saved.engaged)
        return;
    globalLighting = saved.colour;
    PublishGlobalLighting();
    saved.engaged = false;
}

namespace {

template <typename T> T GeometryRead(const std::byte *bytes, std::size_t offset = 0) {
    T value;
    std::memcpy(&value, bytes + offset, sizeof(value));
    return value;
}
template <typename T> void GeometryWrite(std::byte *bytes, std::size_t offset, const T &value) {
    std::memcpy(bytes + offset, &value, sizeof(value));
}
float LightDot(const Vector3 &first, const Vector3 &second) {
    return (first.y * second.y + first.x * second.x) + first.z * second.z;
}
void NormalizeGeometryLight(Vector3 &value, bool threshold) {
    if (threshold && std::fabs(value.x) <= 0.00001f && std::fabs(value.y) <= 0.00001f && std::fabs(value.z) <= 0.00001f)
        return;
    const float length = std::sqrt(LightDot(value, value));
    if (length == 0.0f)
        return;
    value.x /= length;
    value.y /= length;
    value.z /= length;
}
std::uint32_t PackGeometryColour(const Vector4 &colour) {
    const float *const components = &colour.x;
    std::uint32_t result = 0;
    for (std::uint32_t index = 0; index < 4; ++index) {
        const float value = components[index] >= 0.0f ? std::min(components[index], 1.0f) : 0.0f;
        result = (result << 8) | static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<double>(value) * 255.0));
    }
    return result;
}
std::uint32_t PackGeometryNormal(const Vector3 &normal, std::uint32_t marker) {
    return marker | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(static_cast<std::int32_t>(normal.x * 127.0f))) << 16) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(static_cast<std::int32_t>(normal.y * 127.0f))) << 8) |
        static_cast<std::uint8_t>(static_cast<std::int32_t>(normal.z * 127.0f));
}
Vector3 UnpackGeometryNormal(std::uint32_t packed) {
    return {static_cast<float>(static_cast<double>(static_cast<std::int8_t>(packed >> 16)) / 127.0),
        static_cast<float>(static_cast<double>(static_cast<std::int8_t>(packed >> 8)) / 127.0),
        static_cast<float>(static_cast<double>(static_cast<std::int8_t>(packed)) / 127.0)};
}
void SelectGeometryInfluences(const std::byte *mesh, GeometryLightTreeWork &work, bool xb01) {
    Vector3 centre = GeometryRead<Vector3>(mesh, 8);
    Matrix4x4::TransformPoint(centre, centre);
    const float radius = GeometryRead<float>(mesh, 20);
    work.influenceCount = 0;
    for (const LightRuntimeEntry &light : work.lights) {
        if (!light.indexes[0] || !(light.indexes[1] & 8))
            continue;
        const std::uint32_t kind = light.indexes[1] & 7U;
        if (kind == 3 || kind == 4) {
            const Vector3 delta{centre.x - light.vector20.x, centre.y - light.vector20.y, centre.z - light.vector20.z};
            if (std::sqrt(LightDot(delta, delta)) > radius + light.vectors[1].z)
                continue;
        }
        GeometryLightInfluence influence{};
        influence.kind = kind;
        Matrix4x4_Push();
        Matrix4x4::InvertRigidTransform();
        Matrix4x4::TransformPoint(light.vector20, influence.position);
        Matrix4x4::Transform({light.values2c[0], light.values2c[1], light.value34}, influence.direction);
        NormalizeGeometryLight(influence.direction, xb01);
        Matrix4x4::Pop(1);
        influence.colour = {
            light.vectors[0].x, light.vectors[0].y * light.vectors[1].y, light.vectors[0].z * light.vectors[1].y, light.vectors[1].x * light.vectors[1].y};
        influence.rangeSquared = light.vectors[1].z * light.vectors[1].z;
        const float falloff = light.value1c * light.vectors[1].z;
        const float square = falloff * falloff;
        const float fourth = square * square;
        influence.falloffPower = fourth * fourth;
        const std::uint16_t cone = static_cast<std::uint16_t>(light.unknown02);
        const std::uint16_t magnitude = cone <= 0x8000U ? cone : static_cast<std::uint16_t>(-cone);
        influence.coneCosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - magnitude));
        work.influences[std::min(work.influenceCount, 127U)] = influence;
        if (work.influenceCount < 128)
            ++work.influenceCount;
    }
}
void ShadeGeometryVertex(const Vector3 &position, const Vector3 &normal, const Vector4 &material, Vector4 &colour, const GeometryLightTreeWork &work) {
    for (std::uint32_t index = 0; index < work.influenceCount; ++index) {
        const GeometryLightInfluence &light = work.influences[index];
        float strength = 0.0f;
        if (light.kind == 2) {
            strength = -LightDot(normal, light.direction);
        } else if (light.kind == 3 || light.kind == 4) {
            const Vector3 delta{light.position.x - position.x, light.position.y - position.y, light.position.z - position.z};
            const float distanceSquared = LightDot(delta, delta);
            if (distanceSquared > light.rangeSquared)
                continue;
            const float inverseDistance = distanceSquared == 0.0f ? 3.4028235e38f : 1.0f / std::sqrt(distanceSquared);
            if (inverseDistance <= 0.0f)
                continue;
            const float lambert = LightDot(delta, normal);
            if (lambert <= 0.0f)
                continue;
            const float inverseSquare = inverseDistance * inverseDistance;
            const float inverseFourth = inverseSquare * inverseSquare;
            const float attenuation = std::min(1.0f, inverseFourth * inverseFourth * light.falloffPower);
            strength = lambert * inverseDistance;
            if (light.kind == 4) {
                const float cone = LightDot(delta, light.direction) * inverseDistance;
                if (cone < light.coneCosine)
                    continue;
                strength *= ((cone - light.coneCosine) / (1.0f - light.coneCosine)) * attenuation;
            } else {
                strength *= attenuation;
            }
        }
        if (strength <= 0.0f)
            continue;
        colour.y += (light.colour.y * material.y) * strength;
        colour.z += (light.colour.z * material.z) * strength;
        colour.w += (light.colour.w * material.w) * strength;
    }
}
const std::byte *GetGeometrySourceBuffer(const std::byte *mesh, std::uint32_t buffer) {
    const std::byte *command = mesh + 28 + 4ULL * GeometryRead<std::uint32_t>(mesh, 28);
    for (; GeometryRead<std::uint32_t>(command); command += 4ULL * (GeometryRead<std::uint32_t>(command) >> 8)) {
        if (GeometryRead<std::uint8_t>(command) != 1)
            continue;
        const std::uint32_t count = GeometryRead<std::uint32_t>(command, 4);
        const std::byte *record = command + 8;
        for (std::uint32_t index = 0; index < count; ++index) {
            if (!buffer)
                return record + 8;
            --buffer;
            record += 8 + GeometryRead<std::uint32_t>(record, 4);
        }
    }
    return nullptr;
}

void ParseXB01GeometryLights(MT7Node &node, GeometryLightTreeWork *work, bool apply) {
    std::byte *const mesh = node.runtimeMesh;
    const MT7Node &owner = node.borrowedMesh ? *node.borrowedMesh : node;
    LegacyDrawResources &resources = const_cast<LegacyDrawResources &>(owner.drawResources);
    std::uint32_t flags = GeometryRead<std::uint32_t>(mesh, 4);
    if ((!apply && (std::fabs(GeometryRead<float>(mesh, 20)) < 0.00001f || !(flags & 2))) || (apply && (flags & 2)))
        return;
    flags = apply ? flags | 0x22U : flags & ~0x22U;
    GeometryWrite(mesh, 4, flags);
    if (apply)
        SelectGeometryInfluences(mesh, *work, true);
    std::uint32_t buffer = 0;
    std::uint32_t shader = 0;
    std::byte *shaderCommand = nullptr;
    float intensity = 0.0f;
    Vector4 material{};
    Vector4 specular{};
    std::byte *packet = mesh + 4ULL * GeometryRead<std::uint32_t>(mesh, 24) + 24;
    for (; GeometryRead<std::uint32_t>(packet); packet += 4ULL * (GeometryRead<std::uint32_t>(packet) >> 8)) {
        for (std::byte *command = packet + 20; GeometryRead<std::uint8_t>(command); command += 4ULL * (GeometryRead<std::uint32_t>(command) >> 8)) {
            const std::uint8_t opcode = GeometryRead<std::uint8_t>(command);
            if (opcode == 1) {
                buffer = GeometryRead<std::uint8_t>(command, 8);
            } else if (opcode == 2) {
                shaderCommand = command + 4;
                shader = GeometryRead<std::uint32_t>(shaderCommand);
                if (apply && shader <= 2)
                    GeometryWrite(shaderCommand, 0, std::uint32_t{3});
            } else if (opcode == 4 && apply) {
                intensity = GeometryRead<float>(command, 4);
                material = {
                    GeometryRead<float>(command, 32), GeometryRead<float>(command, 20), GeometryRead<float>(command, 24), GeometryRead<float>(command, 28)};
                specular = {
                    GeometryRead<float>(command, 48), GeometryRead<float>(command, 36), GeometryRead<float>(command, 40), GeometryRead<float>(command, 44)};
            } else if (opcode == 16 && (apply || shader == 3)) {
                const std::uint32_t commandOffset = static_cast<std::uint32_t>(command - mesh);
                const LegacyIndexDraw *draw = nullptr;
                for (const LegacyIndexDraw &candidate : resources.indexDraws)
                    if (candidate.commandOffset == commandOffset) {
                        draw = &candidate;
                        break;
                    }
                if (!draw || buffer >= resources.vertexBuffers.size())
                    continue;
                const std::int32_t vertexHandle = resources.vertexBuffers[buffer].handle;
                std::byte *const vertices = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, vertexHandle, 1, 0));
                const std::uint16_t *const indices = static_cast<const std::uint16_t *>(
                    d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, resources.indexBuffer, 2, 2 * draw->startIndex));
                const std::byte *const source = GetGeometrySourceBuffer(mesh, buffer);
                if (!vertices || !indices || !source)
                    continue;
                for (std::uint32_t index = 0; index < draw->indexCount; ++index) {
                    const std::size_t offset = 32ULL * indices[index];
                    std::byte *const vertex = vertices + offset;
                    const std::uint32_t packed = GeometryRead<std::uint32_t>(vertex, 12);
                    const std::uint32_t marker = packed & 0xff000000U;
                    if (!apply) {
                        if (marker != 0xff000000U && marker != 0xfe000000U)
                            continue;
                        GeometryWrite(shaderCommand, 0, ~(packed >> 23) & 2U);
                        std::memcpy(vertex + 12, source + offset + 12, sizeof(Vector3));
                        continue;
                    }
                    if (shader > 3)
                        continue;
                    Vector4 colour = material;
                    if (shader == 3) {
                        if (marker != 0xff000000U) {
                            if (marker == 0xfe000000U)
                                GeometryWrite(vertex, 16, PackGeometryColour(colour));
                            continue;
                        }
                        GeometryWrite(vertex, 16, PackGeometryColour(colour));
                    }
                    const Vector3 normal = GeometryRead<Vector3>(source, offset + 12);
                    GeometryWrite(vertex, 12, PackGeometryNormal(normal, shader == 2 ? 0xfe000000U : 0xff000000U));
                    if (shader != 2) {
                        colour = {material.x, (material.y * work->baseColour.y) * intensity, (material.z * work->baseColour.z) * intensity,
                            (material.w * work->baseColour.w) * intensity};
                        ShadeGeometryVertex(GeometryRead<Vector3>(vertex), normal, material, colour, *work);
                    }
                    GeometryWrite(vertex, 16, PackGeometryColour(colour));
                    GeometryWrite(vertex, 20, PackGeometryColour(specular));
                }
                d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, resources.indexBuffer);
                d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, vertexHandle);
            }
        }
    }
}

void ParseLegacyGeometryLights(std::byte *mesh, GeometryLightTreeWork *work, bool apply) {
    if (GeometryRead<std::int32_t>(mesh) < 0)
        return;
    std::uint32_t flags = GeometryRead<std::uint32_t>(mesh, 4);
    if (GeometryRead<std::uint32_t>(mesh, 24) == 0)
        return;
    GeometryWrite(mesh, 4, apply ? flags | 0x20U : flags & ~0x20U);
    if (apply)
        SelectGeometryInfluences(mesh, *work, false);
    std::byte *command = mesh + 24;
    std::int32_t shader = 0;
    float intensity = 0.0f;
    Vector4 material{};
    for (; GeometryRead<std::uint32_t>(command);) {
        if (GeometryRead<std::int32_t>(command) < 0) {
            shader = GeometryRead<std::int32_t>(command, 36);
            if (apply) {
                intensity = GeometryRead<float>(command, 40);
                material = GeometryRead<Vector4>(command, 44);
                GeometryWrite(command, 36, std::int32_t{-3});
            } else if (shader == -3) {
                const std::uint32_t marker = GeometryRead<std::uint32_t>(command, 100);
                if (marker & 0xff000000U)
                    GeometryWrite(command, 36, (marker & 0x01000000U) ? 0 : -1);
            }
            command += 80;
        }
        std::uint32_t count = GeometryRead<std::uint32_t>(command, 4);
        if (GeometryRead<std::uint32_t>(command) & 8U)
            count *= 3;
        command += 8;
        for (std::uint32_t index = 0; index < count; ++index) {
            if (!(GeometryRead<std::uint8_t>(command) & 1)) {
                command += 8;
                continue;
            }
            const std::uint32_t packed = GeometryRead<std::uint32_t>(command, 12);
            if (!apply) {
                if (shader == -3 && (packed & 0xff000000U))
                    GeometryWrite(command, 12, UnpackGeometryNormal(packed));
            } else if (shader != -3 || (packed & 0x01000000U)) {
                Vector3 normal{};
                Vector4 colour{};
                if (shader == -3) {
                    normal = UnpackGeometryNormal(packed);
                    const std::uint32_t existing = GeometryRead<std::uint32_t>(command, 16);
                    colour = {static_cast<float>(existing >> 24) / 255.0f, static_cast<float>((existing >> 16) & 255U) / 255.0f,
                        static_cast<float>((existing >> 8) & 255U) / 255.0f, static_cast<float>(existing & 255U) / 255.0f};
                } else {
                    normal = GeometryRead<Vector3>(command, 12);
                    GeometryWrite(command, 12, PackGeometryNormal(normal, shader == -1 ? 0xfe000000U : 0xff000000U));
                    colour = shader == -1 ? material :
                                            Vector4{material.x, (work->baseColour.y * material.y) * intensity, (work->baseColour.z * material.z) * intensity,
                                                (work->baseColour.w * material.w) * intensity};
                }
                if (shader != -1)
                    ShadeGeometryVertex(GeometryRead<Vector3>(command), normal, material, colour, *work);
                GeometryWrite(command, 16, PackGeometryColour(colour));
            }
            command += 32;
        }
    }
}

void ResetGeometryLightTree(MT7Node *node) {
    for (; node; node = node->runtimeSibling) {
        if (node->runtimeMesh && !(node->id & 0x100000)) {
            if (IsModelXB01(node->runtimeMesh))
                ParseXB01GeometryLights(*node, nullptr, false);
            else
                ParseLegacyGeometryLights(node->runtimeMesh, nullptr, false);
        }
        if (node->runtimeChild)
            ResetGeometryLightTree(node->runtimeChild);
    }
}

void TraverseGeometryLightTree(MT7Node *node, float scale, GeometryLightTreeWork &work) {
    for (; node; node = node->runtimeSibling) {
        const bool sibling = node->runtimeSibling != nullptr;
        if (sibling)
            Matrix4x4_Push();
        float nodeScale = scale;
        if (node->transformOverride) {
            Matrix4x4::SetActiveTransform(&work.initialTransform);
            Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
            nodeScale = 1.0f;
        } else {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
            if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f) {
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
                nodeScale *= node->scale.x;
            }
        }
        if (node->runtimeMesh && !(node->id & 0x100000)) {
            const bool xb01 = IsModelXB01(node->runtimeMesh);
            if (xb01)
                ParseXB01GeometryLights(*node, nullptr, false);
            else
                ParseLegacyGeometryLights(node->runtimeMesh, nullptr, false);
            if (work.lightingRequired) {
                const bool overrideTransform = currentLightTransform && (GeometryRead<std::uint32_t>(node->runtimeMesh, 4) & 0x40);
                if (overrideTransform)
                    Matrix4x4_Push(currentLightTransform);
                SetLegacyModelScale(nodeScale);
                if (xb01)
                    ParseXB01GeometryLights(*node, &work, true);
                else
                    ParseLegacyGeometryLights(node->runtimeMesh, &work, true);
                if (overrideTransform)
                    Matrix4x4::Pop(1);
            }
        }
        if (node->runtimeChild)
            TraverseGeometryLightTree(node->runtimeChild, nodeScale, work);
        if (sibling)
            Matrix4x4::Pop(1);
    }
}

std::uint32_t ApplyGeometryLightTree(MT7Node *root, const Vector4 &colour) {
    if (!root) {
        SetLegacyModelScale(1.0f);
        return 0;
    }
    const std::int32_t vertexColourType = GetLegacyVertexColourType();
    SetLegacyVertexColourType(1);
    Matrix4x4_Push();
    GeometryLightTreeWork work{};
    Matrix4x4::GetTransformMatrix(&work.initialTransform);
    work.lights = lightRuntimeEntries;
    work.baseColour = colour;
    if (currentActiveLight != 1) {
        for (const LightRuntimeEntry &light : work.lights)
            if (light.indexes[1] & 8U) {
                work.lightingRequired = 1;
                break;
            }
    }
    TraverseGeometryLightTree(root, GetLegacyModelScale(), work);
    Matrix4x4::Pop(1);
    SetLegacyVertexColourType(vertexColourType);
    SetLegacyModelScale(1.0f);
    return work.lightingRequired;
}

} // namespace

void DrawPreparedVertexColours(const MT7Model *model, const MT7Node *node, float alpha) {
    const std::int32_t previous = GetLegacyVertexColourType();
    SetLegacyVertexColourType(0);
    charaPrepareNodesForDraw(model, node, alpha);
    SetLegacyVertexColourType(previous);
}

namespace {

void LightTask_Callback(void *) {
    previousActiveLight = currentActiveLight;
    previousGlobalLighting = globalLighting;
    characterLightCandidateCount = 0;


    SetLegacyLambertFvfReference(nullptr);
    // LightTask_Callback/ImmediateLightWork select in source order. Equal
    // distances retain that order; slots 0 and 13..15 never enter the rank.
    if (rankMapLights && currentActiveLight == 3 && g_cameraControl) {
        struct RankedLight {
            float distance;
            std::int16_t index;
        };
        std::array<RankedLight, 256> ranked{};
        std::size_t count = 0;
        Vector3 camera{};
        camCtrlGetNextCamPos(&camera);
        const float radius = std::min(150.0f, g_cameraControl->farDistance);
        for (std::int16_t index = 1; index < 256; ++index) {
            if (index >= 13 && index <= 15)
                continue;
            const LightRuntimeEntry &light = lightRuntimeEntries[index];
            if (light.indexes[0] != 1)
                continue;
            const std::uint8_t kind = light.indexes[1] & 0xf7U;
            float distance = 0.1f;
            if (kind != 2U) {
                const Vector3 position{
                    light.vector20.x + (kind == 3U ? lightRootPosition.x : 0.0f),
                    light.vector20.y + (kind == 3U ? lightRootPosition.y : 0.0f),
                    light.vector20.z + (kind == 3U ? lightRootPosition.z : 0.0f),
                };
                const float dx = camera.x - position.x;
                const float dz = camera.z - position.z;


                distance = std::sqrt(dx * dx + dz * dz);
                if (distance - light.vectors[1].z > radius)
                    continue;
                const float viewZ = workTransformMatrix.rows[1].z * position.y + workTransformMatrix.rows[0].z * position.x +
                    workTransformMatrix.rows[2].z * position.z + workTransformMatrix.rows[3].z;
                if (viewZ > 0.0f && distance > light.vectors[1].z)
                    continue;
            }
            for (std::size_t priority = 0; priority < priorityLightCount; ++priority) {
                if (priorityLightIndices[priority] == index) {
                    distance = 0.0f;
                    break;
                }
            }
            std::size_t destination = count;
            while (destination > 0 && ranked[destination - 1].distance > distance) {
                ranked[destination] = ranked[destination - 1];
                --destination;
            }
            ranked[destination] = {distance, index};
            ++count;
        }
        for (std::size_t index = 0; index < count; ++index)
            characterLightCandidates[characterLightCandidateCount++] = ranked[index].index;
        selectedLightIndices.fill(-1);
        selectedLightIndices[0] = 0;
        const std::size_t selectedCount = std::min(count, static_cast<std::size_t>(rankedLightLimit));
        for (std::size_t slot = 0; slot < selectedCount; ++slot) {
            selectedLightIndices[slot + 1] = ranked[slot].index;
        }
        selectedLightIndices[13] = 13;
        selectedLightIndices[14] = 14;
        for (std::int16_t index : {std::int16_t{13}, std::int16_t{14}}) {
            if (lightRuntimeEntries[index].indexes[0] != 0)
                characterLightCandidates[characterLightCandidateCount++] = index;
        }
    } else {
        for (std::int16_t slot = 0; slot < 16; ++slot) {
            selectedLightIndices[slot] = slot;
        }
        for (std::int16_t index = 0; index < 16; ++index) {
            if (lightRuntimeEntries[index].indexes[0] != 0)
                characterLightCandidates[characterLightCandidateCount++] = index;
        }
    }
    HLib::ExecuteCallbackNow(ImmediateLightWork, nullptr);
    priorityLightCount = 0;
}
void ApplyWeatherLightStateInternal(std::uint32_t index, const Vector4 *colour, const float *sceneIntensity, const Vector3 *sunDirection) {
    if (colour)
        SetLightRuntimeColour(index, *colour);
    if (sceneIntensity)
        SetLightSceneIntensity(index, *sceneIntensity);
    if (sunDirection)
        SetLightSunDirection(index, *sunDirection);
    // These setters mutate only the runtime owner; publication belongs to LGHT.
}

// @verified - p
void ApplyDefinitionLight(std::uint32_t index, LightRuntimeEntry light) {
    if (index >= lightRuntimeEntries.size())
        return;
    lightRuntimeEntries[index].indexes[1] = light.indexes[1];
    SetLightRuntimeColour(index, {light.vectors[0].x, light.vectors[0].y, light.vectors[0].z, light.vectors[1].x});
    SetLightSceneIntensity(index, light.vectors[1].y == 0.0f ? 1.0f : light.vectors[1].y);
    SetLightRange(index, light.vectors[1].z);
    SetLightFalloff(index, &light.value1c);
    Vector3 position = light.vector20;
    if ((light.indexes[1] & 0xf7U) == 3U)
        position = {lightRootPosition.x + position.x, lightRootPosition.y + position.y, lightRootPosition.z + position.z};
    SetLightRuntimePosition(index, position);
    SetLightSunDirection(index, {light.values2c[0], light.values2c[1], light.value34});
    SetLightConeAngle(index, light.unknown02);
    SetLightEntryIndex(index, light.indexes[0]);
}

const LightDefinitionEntry *ResolveDefinition(std::uint64_t identifier) {
    if (identifier < lightDefinitions.size()) {
        return &lightDefinitions[static_cast<std::size_t>(identifier)];
    }
    for (const LightDefinitionEntry &entry : lightDefinitions) {
        if (entry.block && entry.identifier == identifier)
            return &entry;
    }
    return nullptr;
}

// @verified - p
void ParseModernDefinition(const std::byte *block) {
    PackedLightDefinitionHeader header;
    if (!Read(block, header))
        return;
    if (header.byteSize < sizeof(header) || header.byteSize > static_cast<std::size_t>(lightChunkEnd - block) ||
        header.lightCount > 0 && static_cast<std::size_t>(header.lightCount) > (header.byteSize - sizeof(header)) / sizeof(PackedLightDefinitionRecord))
        return;
    ReinitializeLights(header.mode);
    rankMapLights = header.mode == 3;
    UpdateGlobalLighting({F16toF32(header.colour[0]), F16toF32(header.colour[1]), F16toF32(header.colour[2]), F16toF32(header.colour[3])});
    for (std::int32_t record = 0; record < header.lightCount; ++record) {
        PackedLightDefinitionRecord packed;
        if (!Read(block + sizeof(header) + record * sizeof(packed), packed))
            return;
        LightRuntimeEntry light{};
        light.indexes = {static_cast<std::uint8_t>(packed.enabled), static_cast<std::uint8_t>(packed.flags)};
        light.unknown02 = packed.coneAngle;
        light.vectors[0] = {1.0f, F16toF32(packed.parameters[0]), F16toF32(packed.parameters[1])};
        light.vectors[1] = {F16toF32(packed.parameters[2]), F16toF32(packed.parameters[3]), F16toF32(packed.parameters[4])};
        light.value1c = F16toF32(packed.parameters[5]);
        light.vector20 = {F16toF32(packed.parameters[6]), F16toF32(packed.parameters[7]), F16toF32(packed.parameters[8])};
        light.values2c = {F16toF32(packed.parameters[9]), F16toF32(packed.parameters[10])};
        light.value34 = F16toF32(packed.parameters[11]);
        ApplyDefinitionLight(static_cast<std::uint32_t>(packed.index), light);
    }
}

// @verified - p
void ParseLegacyDefinition(const std::byte *block) {
    std::uint32_t mode{};
    Vector4 colour{};
    if (!Read(block, mode) || !Read(block + 4, colour))
        return;
    ReinitializeLights(static_cast<std::int32_t>(mode));
    UpdateGlobalLighting(colour);
    LightRuntimeEntry primary{};
    if (!Read(block + 0x14, primary))
        return;
    if (primary.vectors[1].y == 0.0f) {
        primary.vectors[1].y = 1.0f;
        const float intensity = 1.0f;
        std::memcpy(const_cast<std::byte *>(block) + 0x28, &intensity, sizeof(intensity));
    }
    if (mode == 1) {
        ApplyDefinitionLight(0, primary);
    } else {
        ApplyDefinitionLight(16, primary);
        for (std::uint32_t slot = 1; slot < 16; ++slot) {
            LightRuntimeEntry light{};
            if (!Read(block + 0x14 + slot * sizeof(LightRuntimeEntry), light))
                return;
            if (light.vectors[1].y == 0.0f) {
                light.vectors[1].y = 1.0f;
                const float intensity = 1.0f;
                std::memcpy(const_cast<std::byte *>(block) + 0x14 + slot * sizeof(LightRuntimeEntry) + 0x14, &intensity, sizeof(intensity));
            }
            ApplyDefinitionLight(slot, light);
        }
    }
}

void BuildDefinitionTable(const MapInfoChunk *chunk) {
    lightDefinitions.fill({nullptr, 0xffffffffU});
    lightDefinitionEnds.fill(nullptr);
    if (!chunk || chunk->nextByteOffset < sizeof(MapInfoChunk))
        return;
    const auto *cursor = reinterpret_cast<const std::byte *>(chunk) + 8;
    lightChunkEnd = reinterpret_cast<const std::byte *>(chunk) + chunk->nextByteOffset;
    for (std::uint32_t index = 0; index < lightDefinitions.size(); ++index) {
        std::uint32_t tag{};
        if (!Read(cursor, tag) || tag == 0)
            break;
        std::uint32_t identifier = index;
        if (tag != lghtToken) {
            identifier = tag;
            cursor += 8;
        }
        std::uint32_t size{};
        if (!Read(cursor + 4, size))
            break;
        lightDefinitions[index] = {cursor, identifier};
        lightDefinitionEnds[index] = lightChunkEnd;
        if (size < 8 || size > static_cast<std::uint64_t>(lightChunkEnd - cursor)) {
            break;
        }
        cursor += size;
    }
}

} // namespace

// @verified - p
void LGHT_Parser_Internal(std::uint64_t identifier) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control || !FindMapInfoChunk("LGHT", control->mapInfo))
        return;
    const LightDefinitionEntry *const definition = ResolveDefinition(identifier);
    if (!definition || !definition->block)
        return;
    lightChunkEnd = lightDefinitionEnds[static_cast<std::size_t>(definition - lightDefinitions.data())];
    std::uint32_t tag{};
    if (!Read(definition->block, tag))
        return;
    if (tag == lghtToken) {
        ParseModernDefinition(definition->block);
    } else {
        ParseLegacyDefinition(definition->block);
    }
}

bool RegisterLightData(const void *block, std::int32_t identifier, std::size_t size) {
    std::size_t index = 0;
    while (index < lightDefinitions.size() && lightDefinitions[index].block)
        ++index;
    if (index >= lightDefinitions.size())
        return false;
    lightDefinitions[index] = {static_cast<const std::byte *>(block), static_cast<std::uint32_t>(identifier)};
    lightDefinitionEnds[index] = static_cast<const std::byte *>(block) + size;
    return true;
}

void UnregisterLightData(std::int32_t identifier) {
    for (std::size_t index = 0; index < lightDefinitions.size(); ++index) {
        if (lightDefinitions[index].identifier != static_cast<std::uint32_t>(identifier))
            continue;
        lightDefinitions[index] = {nullptr, 0xffffffffU};
        lightDefinitionEnds[index] = nullptr;
        return;
    }
}

void ApplyWeatherLightState(std::uint32_t index, const Vector4 *colour, const float *sceneIntensity, const Vector3 *sunDirection) {
    ApplyWeatherLightStateInternal(index, colour, sceneIntensity, sunDirection);
}

void ApplyWeatherGlobalLighting(float value) {
    UpdateGlobalLighting({value, value, value, value});
}

void ReleaseLensflareHostResources() {
    for (MT7Model &model : weatherLensModels)
        if (!model.nodes.empty() || !model.source.empty())
            ReleaseModelFormat7(model);
}

// @verified - p
void LoadLensflare() {
    std::uint32_t size{};
    void *const allocation = LoadLegacySceneFile("misc", "LENSFLAR.BIN", WEATHER_LENS_PACKAGE, &size);
    if (!allocation)
        return;
    const std::byte *cursor = static_cast<const std::byte *>(allocation);
    const std::byte *const end = cursor + size;
    for (MT7Model &model : weatherLensModels) {
        std::uint32_t length{};
        if (end - cursor < 8)
            break;
        std::memcpy(&length, cursor + 4, sizeof(length));
        if (length < 8 || static_cast<std::size_t>(end - cursor) < length)
            break;
        const std::size_t modelSize = length - 8;
        void *const data = HLib::AllocateTaskData(modelSize, WEATHER_LENS_MODEL);
        std::memcpy(data, cursor + 8, modelSize);
        if (!model.nodes.empty() || !model.source.empty())
            ReleaseModelFormat7(model);
        ReadModelFormat7({static_cast<const std::byte *>(data), modelSize}, nullptr, model, 0);
        cursor += length;
    }
    HLib::FreeTaskData(allocation);
}

// @verified - p
void shenSetLensflare() {
    for (const WeatherLensMaterialParameter &entry : WEATHER_LENS_MATERIAL_PARAMETERS) {
        MT7Model &model = weatherLensModels[entry.model];
        if (model.nodes.empty())
            continue;
        const std::size_t offset = model.nodes.front().xb01Offset;
        if (!offset || offset > model.source.size() || model.source.size() - offset < 28)
            continue;
        std::uint32_t signature{}, packetOffset{};
        std::memcpy(&signature, model.source.data() + offset, sizeof(signature));
        if (signature != SHEN_NAME(XB01))
            continue;
        std::memcpy(&packetOffset, model.source.data() + offset + 24, sizeof(packetOffset));
        const std::size_t command = offset + static_cast<std::size_t>(packetOffset) * 4;
        if (command > model.source.size() || model.source.size() - command < 40)
            continue;
        std::uint32_t flags{};
        std::memcpy(&flags, model.source.data() + command + 24, sizeof(flags));
        flags |= 8U;
        std::memcpy(const_cast<std::byte *>(model.source.data()) + command + 24, &flags, sizeof(flags));
        std::memcpy(const_cast<std::byte *>(model.source.data()) + command + 36, &entry.parameter, sizeof(entry.parameter));
    }
}

// @verified - p
void shenLoadSetLensflare() {
    LoadLensflare();
    g_weatherSunState.diskScale = 0.5f;
    g_weatherSunState.control = 0;
    g_weatherSunState.enabled = 1;
    g_weatherSunState.explicitDirection = 0;
    shenSetLensflare();
}

// @verified - p
void UpdateSunForDateTime(const DateTime &date, float latitudeTilt, Vector3 &direction, float intensityScale) {
    constexpr std::array<std::int32_t, 12> accumulatedDays{31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    const float timeAngle =
        (((static_cast<float>(date.minute) / 60.0f + static_cast<float>(date.hour)) + static_cast<float>(date.second) / 3600.0f) - 12.0f) * -15.0f;
    const float season = ((static_cast<float>(accumulatedDays[5] - accumulatedDays[date.month - 1] - date.day + 21) / 365.0f) * 360.0f / 180.0f) * 3.1400001f;
    const float axialTilt = latitudeTilt - std::sin(1.5707964f - std::fabs(season)) * 23.5f;
    const std::uint16_t elevation =
        FloatToShort(((std::sin(1.5707964f - std::fabs((timeAngle * 3.1400001f) / 180.0f)) * (90.0f - axialTilt)) * 8192.0f) / 45.0f);
    direction.y = FixedAngleSin(elevation) * intensityScale;
    const float cosine = FixedAngleCos(elevation) * intensityScale;
    const std::uint16_t angle = FloatToShort((timeAngle * 8192.0f) / 45.0f);
    direction.x = FixedAngleSin(angle) * cosine;
    direction.z = FixedAngleCos(angle) * cosine;
}

// @verified - p
std::int32_t SetWeatherSunControl(std::int32_t control) {
    const std::int32_t previous = g_weatherSunState.control;
    g_weatherSunState.control = control;
    return previous;
}

namespace {

void BeginWeatherLensFlareAnnotation(const char *) {
}
void EndWeatherLensFlareAnnotation() {
}

// @verified - p
void RotateLensflareAlignment(const Vector3 &direction) {
    Vector3 horizontal{direction.x, 0.0f, direction.z};
    const Vector3 axis{0.0f, 0.0f, 1.0f};
    NormalizeVector3(&horizontal);
    Matrix4x4_BuildRotationMatrix(axis, horizontal);
    Vector3 vertical{direction.y, 0.0f, std::sqrt(direction.x * direction.x + direction.z * direction.z)};
    Matrix4x4::RotateAroundZ_Wrapper(16384);
    NormalizeVector3(&vertical);
    Matrix4x4_BuildRotationMatrix(axis, vertical);
    Matrix4x4::RotateAroundZ_Wrapper(49152);
}

// @verified - p
void ScaleForTransformWithPosZ(std::int32_t index, float positionZ, float scale, float alpha) {
    const float totalScale = scale * 0.15000001f;
    Matrix4x4 billboard{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
    Matrix4x4_Push();
    Matrix4x4::preTranslate(0.0f, 0.0f, positionZ);
    Matrix4x4 current;
    Matrix4x4::GetTransformMatrix(&current);
    billboard.rows[3].x = current.rows[3].x;
    billboard.rows[3].y = current.rows[3].y;
    billboard.rows[3].z = current.rows[3].z;
    Matrix4x4::SetActiveTransform(&billboard);
    Matrix4x4::Scale(totalScale, totalScale, totalScale);
    const MT7Model &model = weatherLensModels[index];
    if (!model.nodes.empty()) {
        const MT7Node &node = model.nodes.front();
        SetModelFlagDataWithAlpha(model.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources, alpha);
    }
    Matrix4x4::Pop(1);
}

// @verified - p
void UpdateWeatherEffectFromDirection(void *data) {
    const Vector3 *const direction = static_cast<const Vector3 *>(data);
    const std::byte *const state = static_cast<const std::byte *>(data);
    float scale, jitter, sourceIntensity;
    std::memcpy(&scale, state + 0x0c, sizeof(scale));
    std::memcpy(&jitter, state + 0x10, sizeof(jitter));
    std::memcpy(&sourceIntensity, state + 0x20, sizeof(sourceIntensity));
    if (GetLegacyRenderMask())
        return;
    BeginWeatherLensFlareAnnotation("Lens Flare");
    g_weatherSunState.sunlightIntensity = 0.0f;
    Vector3 screenPosition;
    Matrix4x4::TransformPoint(*direction, screenPosition);
    if (screenPosition.z >= 0.0f) {
        EndWeatherLensFlareAnnotation();
        return;
    }
    const float width = static_cast<float>(d3t::g_originalScreenWidth), height = static_cast<float>(d3t::g_originalScreenHeight);
    PushProjectionPrePass(width, height);
    Vector2 screen;
    PushTransform(*direction, screen);
    if (screen.x <= -(width / 2.1329999f) || width / 0.68000001f <= screen.x || screen.y <= -(height / 2.4000001f) || height / 0.70499998f <= screen.y) {
        EndWeatherLensFlareAnnotation();
        return;
    }
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(camCtrlGetViewMatrix(nullptr));
    Vector3 view{0.0f, 0.0f, -1.0f};
    Matrix4x4::Transform(view, view);
    Matrix4x4::Pop(1);
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    Vector3 alignment{direction->x - eye.x, direction->y - eye.y, direction->z - eye.z};
    const Vector3 baseOffset{0.0f, 0.0f, -0.40000001f};
    float falloff = 0.99000001f, center = 0.99000001f;
    if (!(-(width / 6.4000001f) <= screen.x))
        falloff = ((width / 2.1329999f) + screen.x) / (width / 3.2f);
    else if (screen.x > width / 0.86000001f)
        falloff = ((width / 0.68000001f) - screen.x) / (width / 3.2f);
    if (!(-(height / 6.4000001f) <= screen.y))
        falloff *= ((height / 3.2f) + screen.y) / (height / 6.4000001f);
    else if (screen.y > height / 0.82700002f)
        falloff *= ((height / 0.70499998f) - screen.y) / (height / 6.4000001f);
    if (screen.x <= -(width * 0.03125f) || width / 0.96899998f <= screen.x || screen.y <= -(height / 24.0f) || height / 0.95999998f <= screen.y)
        center = 0.0f;
    else {
        if (!(width * 0.03125f <= screen.x))
            center = ((width * 0.03125f) + screen.x) / (width * 0.0625f);
        else if (screen.x > width / 1.03f)
            center = ((width / 0.96899998f) - screen.x) / (width * 0.0625f);
        if (!(height * 0.03125f <= screen.y))
            center *= ((height * 0.03125f) + screen.y) / (height * 0.0625f);
        else if (screen.y > height / 1.04f)
            center *= ((height / 0.95999998f) - screen.y) / (height * 0.0625f);
    }
    const float projectedHeight = jitter * falloff;
    NormalizeVector3(&view);
    NormalizeVector3(&alignment);
    const float dot = (view.x * alignment.x + view.y * alignment.y) + view.z * alignment.z;
    float intensity = static_cast<float>(static_cast<std::int32_t>(ComputeResolutionScaledIntensity())) / 2200.0f;
    g_weatherSunState.sunlightIntensity = intensity;
    if (intensity > 1.0f)
        g_weatherSunState.sunlightIntensity = intensity = 1.0f;
    else if (intensity < 0.0099999998f)
        g_weatherSunState.sunlightIntensity = intensity = 0.0f;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    filmSetFadeColourTo(
        control->screenFilmTask, static_cast<std::uint32_t>(static_cast<std::int32_t>(((intensity * falloff) * 64.0f) * (intensity * falloff))));
    Vector3 projection{(screenPosition.x * -1.0f) / screenPosition.z, (screenPosition.y * -1.0f) / screenPosition.z, -1.0f};
    const Vector3 toScreen{baseOffset.x - projection.x, baseOffset.y - projection.y, baseOffset.z - -1.0f};
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    if (scale > 0.0f) {
        Matrix4x4_Push();
        projection.x *= sourceIntensity;
        projection.y *= sourceIntensity;
        projection.z *= sourceIntensity;
        Matrix4x4::preTranslate(projection.x, projection.y, projection.z);
        const float diskScale = scale * (sourceIntensity * 0.15000001f);
        Matrix4x4::Scale(diskScale, diskScale, diskScale);
        Matrix4x4_Push();
        Matrix4x4::Scale(0.33000001f, 0.33000001f, 0.33000001f);
        const MT7Model &disk = weatherLensModels[13];
        if (!disk.nodes.empty())
            AssignGeoLayout(disk.source, disk.nodes.front().xb01Offset,
                disk.nodes.front().borrowedMesh ? disk.nodes.front().borrowedMesh->drawResources : disk.nodes.front().drawResources, 0);
        Matrix4x4::Pop(1);
        SetLegacyBusyPreparingDraw(false);
        if (!disk.nodes.empty())
            SetModelFlagDataWithAlpha(disk.source, disk.nodes.front().xb01Offset,
                disk.nodes.front().borrowedMesh ? disk.nodes.front().borrowedMesh->drawResources : disk.nodes.front().drawResources, center * 0.80000001f);
        Matrix4x4_Push();
        Matrix4x4::Scale(0.16500001f, 0.16500001f, 0.16500001f);
        const MT7Model &halo = weatherLensModels[12];
        if (!halo.nodes.empty())
            SetModelFlagDataWithAlpha(halo.source, halo.nodes.front().xb01Offset,
                halo.nodes.front().borrowedMesh ? halo.nodes.front().borrowedMesh->drawResources : halo.nodes.front().drawResources,
                g_weatherSunState.sunlightIntensity * projectedHeight);
        Matrix4x4::Pop(1);
        const float facingScale = (dot * 4.0f) * 0.15000001f;
        Matrix4x4::Scale(facingScale, facingScale, facingScale);
        Matrix4x4::Pop(1);
    }
    Matrix4x4::preTranslate(baseOffset.x, baseOffset.y, baseOffset.z);
    if (g_weatherSunState.sunlightIntensity > 0.0f) {
        Matrix4x4_Push();
        Matrix4x4::Scale(0.1f, 0.1f, 0.1f);
        const MT7Model &centerModel = weatherLensModels[3];
        if (!centerModel.nodes.empty())
            SetModelFlagDataWithAlpha(centerModel.source, centerModel.nodes.front().xb01Offset,
                centerModel.nodes.front().borrowedMesh ? centerModel.nodes.front().borrowedMesh->drawResources : centerModel.nodes.front().drawResources,
                g_weatherSunState.sunlightIntensity * projectedHeight);
        Matrix4x4::Pop(1);
        RotateLensflareAlignment(toScreen);
        const float distance = std::sqrt((toScreen.y * toScreen.y + toScreen.x * toScreen.x) + toScreen.z * toScreen.z);
        ScaleForTransformWithPosZ(11, (distance * 0.69999999f) * -1.7f, 0.69999999f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(0, (distance * 0.69999999f) * -0.30000001f, 0.5f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(1, (distance * 0.69999999f) * -0.25f, 0.5f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(2, (distance * 0.69999999f) * -0.15000001f, 0.5f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(4, (distance * 0.69999999f) * 0.1f, 0.1f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(5, (distance * 0.69999999f) * 0.125f, 0.5f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(7, (distance * 0.69999999f) * 0.15000001f, 0.5f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(6, (distance * 0.69999999f) * 0.16f, 0.5f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(8, (distance * 0.69999999f) * 0.2f, 0.5f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(9, (distance * 0.69999999f) * 0.25f, 0.30000001f, g_weatherSunState.sunlightIntensity * projectedHeight);
        ScaleForTransformWithPosZ(10, (distance * 0.69999999f) * 0.30000001f, 0.30000001f, g_weatherSunState.sunlightIntensity * projectedHeight);
    }
    Matrix4x4::Pop(1);
    EndWeatherLensFlareAnnotation();
}
} // namespace

// @verified - p
void PerformWeatherRenderPass() {
    if (!g_weatherSunState.enabled)
        return;
    if (!g_hasComputedSun) {
        if (GetCurrentActiveLight() != 1 || GetWeatherWindVector() > 0.1f || GetWeatherPressure() > 0.1f)
            return;
        DateTime date;
        GetCurrentDateTime(date);
        UpdateSunForDateTime(date, 35.0f, g_sunDirectionVector, 10000.0f);
        if (g_sunDirectionVector.y < 300.0f)
            return;
        g_flareIntensity = 2200.0f;
    }
    g_weatherSunState.jitterCountdown -= shenGetScaledDeltaTickTime(1.0f);
    if (g_weatherSunState.jitterCountdown <= 0.0f) {
        const float jitter = IncrementSharedPrngFloat();
        g_weatherSunState.jitterCountdown = 1.0f;
        g_weatherSunState.jitterScale = jitter * 0.1f + 0.30000001f;
    }
    HLib::ExecuteCallbackNow(UpdateWeatherEffectFromDirection, &g_sunDirectionVector);
}

// @verified - p
void SetSunDirectionVector(const Vector3 *direction) {
    if (direction) {
        g_sunDirectionVector.x = direction->x;
        g_sunDirectionVector.y = direction->y;
        g_sunDirectionVector.z = direction->z;
        g_flareIntensity = 1.0f;
        g_hasComputedSun = 1;
    } else
        g_hasComputedSun = 0;
}
// @verified - p
void UpdateGlobalLighting(const Vector4 &colour) {
    globalLighting = colour;
    PublishGlobalLighting();
}

// @verified - p
void GetGlobalLighting(Vector4 *colour) {
    *colour = globalLighting;
}

// @verified - p: ExchangeRetainedGlobalLighting.
void ExchangeRetainedGlobalLighting(const Vector4 &colour, Vector4 *saved) {
    static Vector4 retainedColour{.x = 1, .y = .2f, .z = .2f, .w = .2f};
    if (saved)
        *saved = retainedColour;
    retainedColour = colour;
}

// @verified - p
void CopyLightRuntimeEntry(std::uint32_t destination, std::uint32_t source) {
    if (destination < 256 && source < 256)
        lightRuntimeEntries[destination] = lightRuntimeEntries[source];
}

// @verified - p
void SetLightRuntimeColour(std::uint32_t index, const Vector4 &colour) {
    if (index >= lightRuntimeEntries.size())
        return;
    LightRuntimeEntry &light = lightRuntimeEntries[index];
    light.vectors[0] = {colour.x, colour.y < 0.00001f ? 0.0f : colour.y, colour.z < 0.00001f ? 0.0f : colour.z};
    light.vectors[1].x = colour.w < 0.00001f ? 0.0f : colour.w;
}

// @verified - p
void SetLightSceneIntensity(std::uint32_t index, float intensity) {
    if (index < lightRuntimeEntries.size())
        lightRuntimeEntries[index].vectors[1].y = std::fabs(intensity) < 0.0001f ? 0.0f : intensity;
}

// @verified - p
void SetLightRange(std::uint32_t index, float range) {
    if (index < lightRuntimeEntries.size())
        lightRuntimeEntries[index].vectors[1].z = range < 0.01f ? 0.01f : range;
}

// @verified - p
void SetLightFalloff(std::uint32_t index, const float *falloff) {
    if (index < lightRuntimeEntries.size())
        lightRuntimeEntries[index].value1c = *falloff < 0.01f ? 0.01f : *falloff;
}

// @verified - p
void SetLightRuntimePosition(std::uint32_t index, const Vector3 &position) {
    if (index >= lightRuntimeEntries.size())
        return;
    LightRuntimeEntry &light = lightRuntimeEntries[index];
    if ((light.indexes[1] & 0xf7U) == 3U)
        light.vector20 = {position.x - lightRootPosition.x, position.y - lightRootPosition.y, position.z - lightRootPosition.z};
    else
        light.vector20 = position;
}

// @verified - p
std::uint64_t SetLightSunDirection(std::uint32_t index, const Vector3 &direction) {
    if (index >= lightRuntimeEntries.size())
        return 0;
    LightRuntimeEntry &light = lightRuntimeEntries[index];
    light.values2c = {direction.x, direction.y};
    light.value34 = direction.z;
    return std::bit_cast<std::uint32_t>(direction.z);
}

// @verified - p
void SetLightGeometryEnabled(std::uint32_t index, std::int32_t enabled) {
    if (index >= lightRuntimeEntries.size())
        return;
    std::uint8_t &flags = lightRuntimeEntries[index].indexes[1];
    if (enabled)
        flags |= 8U;
    else
        flags &= 0xf7U;
}

// @verified - p
void ResetLightRuntimeEntry(std::uint32_t index) {
    if (index >= lightRuntimeEntries.size())
        return;
    LightRuntimeEntry &light = lightRuntimeEntries[index];
    light.indexes = index == 0 && currentActiveLight == 1 ? std::array<std::uint8_t, 2>{1, 2} : std::array<std::uint8_t, 2>{0, 0};
    const LightRuntimeEntry &defaults = defaultLight;
    SetLightRuntimeColour(index, {defaults.vectors[0].x, defaults.vectors[0].y, defaults.vectors[0].z, defaults.vectors[1].x});
    SetLightSceneIntensity(index, 1.0f);
    SetLightRange(index, 2.0f);
    const float falloff = 0.60000002f;
    SetLightFalloff(index, &falloff);
    SetLightRuntimePosition(index, defaults.vector20);
    SetLightSunDirection(index, {defaults.values2c[0], defaults.values2c[1], defaults.value34});
    SetLightConeAngle(index, 1820);
}

void SetWeatherLighting(float value) {
    weatherLighting = value;
}

void SetWeatherSceneLighting(float value) {
    weatherSceneLighting = value;
}

// @verified - p
float GetWeatherLighting() {
    return weatherLighting;
}

// @verified - p
float GetWeatherSceneLighting() {
    return weatherSceneLighting;
}

bool BeginTemporaryMapWeatherLighting(TemporaryMapWeatherLightingState &saved) {
    if (saved.engaged)
        return false;
    saved.activeLight = currentActiveLight;
    saved.globalLighting = globalLighting;
    saved.sceneIntensity = lightRuntimeEntries[0].vectors[1].y;
    saved.engaged = true;

    currentActiveLight = 1;
    globalLighting = {weatherLighting, weatherLighting, weatherLighting, weatherLighting};
    lightRuntimeEntries[0].vectors[1].y = std::abs(weatherSceneLighting) < 0.0001f ? 0.0f : weatherSceneLighting;
    SetLegacyGlobalLighting(globalLighting);
    SetLegacyLight(0, lightRuntimeEntries[0]);
    return true;
}

void EndTemporaryMapWeatherLighting(TemporaryMapWeatherLightingState &saved) {
    if (!saved.engaged)
        return;
    currentActiveLight = saved.activeLight;
    globalLighting = saved.globalLighting;
    lightRuntimeEntries[0].vectors[1].y = saved.sceneIntensity;
    SetLegacyGlobalLighting(globalLighting);
    SetLegacyLight(0, lightRuntimeEntries[0]);
    saved.engaged = false;
}

std::int32_t GetCurrentActiveLight() {
    return currentActiveLight;
}

std::span<const std::int16_t> GetCharacterLightCandidates() {
    return {characterLightCandidates.data(), characterLightCandidateCount};
}
const LightRuntimeEntry *GetLightRuntimeEntry(std::uint32_t index) {
    return index < lightRuntimeEntries.size() ? &lightRuntimeEntries[index] : nullptr;
}
// @verified - p
std::uint64_t GetLightRootPosition(Vector3 *position) {
    *position = lightRootPosition;
    return std::bit_cast<std::uint32_t>(lightRootPosition.z);
}
std::int32_t GetPreviousActiveLight() {
    return previousActiveLight;
}

void ResetMapLightingPlacements() {
    mapLightingPlacements.fill({1, 0x7fff, {}, 0, 1});
}
std::uint32_t GetMapLightingPlacementEnabled(std::int32_t index) {
    return mapLightingPlacements[index].enabled;
}
std::uint32_t GetMagicRoomFlag(std::int32_t index) {

    if (index >= 33)
        return static_cast<std::uint32_t>(mapEntryLayers[7 * (index - 33)]);
    return mapLightingPlacements[index].mapSlot;
}
std::uint32_t GetMapLightingPlacementPosition(Vector3 *output, std::int32_t index) {
    *output = mapLightingPlacements[index].position;
    return std::bit_cast<std::uint32_t>(output->z);
}
std::uint32_t GetMapLightingPlacementRotation(std::int32_t index) {
    return static_cast<std::uint32_t>(mapLightingPlacements[index].rotationY);
}
std::uint32_t GetMapLightingPlacementActive(std::int32_t index) {
    return mapLightingPlacements[index].active;
}
MapLightingPlacement *GetMapLightingPlacement(std::int32_t index) {
    return &mapLightingPlacements[index];
}
void SetMapLightingPlacementEnabled(std::int32_t value, std::int32_t index) {
    if (index < 33)
        mapLightingPlacements[index].enabled = static_cast<std::uint32_t>(value);
}
std::uint32_t SetMapLightingPlacementPosition(const Vector3 *position, std::int32_t index) {
    if (index >= 33)
        return 0;
    mapLightingPlacements[index].position = *position;
    return std::bit_cast<std::uint32_t>(position->z);
}
void SetMapLightingPlacementRotation(std::int32_t value, std::int32_t index) {
    mapLightingPlacements[index].rotationY = value;
}
std::int64_t AttachMapLightingPlacement(std::int32_t mapSlot, std::int32_t placement) {
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    const MT7Model *const model = mapSlot >= 0 ? slots[mapSlot].reader : nullptr;
    if (!model || model->nodes.empty())
        mapSlot = 0x7fff;
    MapLightingPlacement &record = mapLightingPlacements[placement];
    if (record.mapSlot != 0x7fff)
        d3t::RetireObjectPlacement(&record);
    record.mapSlot = static_cast<std::uint32_t>(mapSlot);
    if (mapSlot == 0x7fff)
        return placement;
    d3t::RetireObjectPlacement(GetMT7ReaderData(model));
    d3t::SetObjectSourceActiveFlag(GetMT7ReaderData(model), 1);
    return d3t::RegisterObjectPlacement(GetMT7ReaderData(model), &record, 0);
}
std::int64_t AssignLightForMapLayer(Vector3 *output, std::int32_t mapSlot) {
    for (const MapLightingPlacement &record : mapLightingPlacements) {
        if (record.mapSlot != static_cast<std::uint32_t>(mapSlot))
            continue;
        *output = record.position;
        return 1;
    }
    *output = GetMapModelSlots()[mapSlot].position;
    return 1;
}

void AssignLightProgram(std::uint32_t mapSlot) {
    if (mapSlot > 32)
        return;
    Vector4 colour;
    if (GetGeometryLightColour(&colour))
        return;
    AllocateGeometryLightPositionScratch();
    const MT7Model *const model = GetMapModelSlots()[mapSlot].reader;
    if (model && !model->nodes.empty()) {
        Vector3 position;
        AssignLightForMapLayer(&position, static_cast<std::int32_t>(mapSlot));
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4 transform;
        Matrix4x4::GetTransformMatrix(&transform);
        currentLightTransform = &transform;
        const std::uint32_t result = ApplyGeometryLightTree(const_cast<MT7Node *>(&model->nodes.front()), colour);
        currentLightTransform = nullptr;
        Matrix4x4::Pop(1);
        SetMapModelLightingTraversalResult(static_cast<std::int32_t>(mapSlot), result);
    }
    RestoreGeometryLightPositionScratch();
}

void PushGlobalLightingProgram() {
    Vector4 colour;
    if (GetGeometryLightColour(&colour))
        return;
    AllocateGeometryLightPositionScratch();
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    for (std::int32_t index = 0; index < 33; ++index) {
        const MT7Model *const model = slots[index].reader;
        if (!model || model->nodes.empty())
            continue;
        Vector3 position{};
        AssignLightForMapLayer(&position, index);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4 transform;
        Matrix4x4::GetTransformMatrix(&transform);
        currentLightTransform = &transform;
        const std::uint32_t result = ApplyGeometryLightTree(const_cast<MT7Node *>(&model->nodes.front()), colour);
        currentLightTransform = nullptr;
        Matrix4x4::Pop(1);
        SetMapModelLightingTraversalResult(index, result);
    }
    if (MT7Node *const root = GetPropRoot()) {
        Vector3 position{};
        GetPropVector4(&position);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const std::uint32_t result = ApplyGeometryLightTree(root, colour);
        Matrix4x4::Pop(1);
        SetPropLightingTraversalResult(result);
    }
    RestoreGeometryLightPositionScratch();
}


void ApplyCharacterGeometryLighting(Character *character, std::int32_t replaceDrawCallback) {
    Vector4 colour;
    if (GetGeometryLightColour(&colour))
        return;
    const Vector3 position = GetCharacterWorldPosition(character);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    MT7Node *const root = model && !model->nodes.empty() ? model->nodes.data() : nullptr;
    (void)ApplyGeometryLightTree(root, colour);
    Matrix4x4::Pop(1);
    if (replaceDrawCallback)
        imgmAssignUpdateCallback(character, DrawPreparedVertexColours);
}

// @verified - p
void SetupLightConfig() {
    Vector4 colour;
    if (GetGeometryLightColour(&colour))
        return;
    AllocateGeometryLightPositionScratch();
    MT7Node *const root = GetPropRoot();
    if (root) {
        Vector3 position;
        GetPropVector4(&position);
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        const std::uint32_t result = ApplyGeometryLightTree(root, colour);
        Matrix4x4::Pop(1);
        SetPropLightingTraversalResult(result);
    }
    RestoreGeometryLightPositionScratch();
}

// @verified - p
void SetupModelDataDefaultVS(MT7Node *root) {
    if (!root)
        return;
    const std::int32_t vertexColourType = GetLegacyVertexColourType();
    SetLegacyVertexColourType(1);
    Matrix4x4_Push();
    ResetGeometryLightTree(root);
    Matrix4x4::Pop(1);
    SetLegacyVertexColourType(vertexColourType);
}

// @verified - p
void SetDefaultVS() {
    MT7Node *const root = GetPropRoot();
    if (!root)
        return;
    SetupModelDataDefaultVS(root);
    SetPropLightingTraversalResult(0);
}

// @verified - p
void SetCurrentActiveLight(std::int32_t mode) {
    currentActiveLight = mode;
    LightModeNullsub(mode == 1 ? 0 : 1);
}

void ResetLightEntry(std::uint32_t slot) {
    ResetLegacySelectedLight(slot, currentActiveLight, defaultLight);
}

void SetLightKind(std::uint32_t slot, std::int32_t kind) {
    SetLegacySelectedLightKind(slot, kind);
}

void SetSelectedLightDirection(std::uint32_t slot, const Vector3 &direction) {
    SetLegacySelectedLightDirection(slot, direction);
}

void shenSetLightColour(std::uint32_t slot, const Vector4 &colour) {
    SetLegacySelectedLightColour(slot, colour);
}

bool BeginTemporaryReflectionLight(std::uint32_t slot, const Vector3 &sourcePosition, const Vector3 &cameraPosition, const Vector4 &colour, std::uint32_t mode,
    std::int16_t yaw, TemporaryReflectionLightState &saved) {
    constexpr std::uint32_t nativeLightSlotCount = 16;
    if (slot >= nativeLightSlotCount || saved.engaged || !g_cameraControl)
        return false;

    saved.slot = slot;
    saved.activeLight = currentActiveLight;
    saved.engaged = true;

    currentActiveLight = 3;
    LightRuntimeEntry &light = lightRuntimeEntries[slot];
    light = defaultLight;
    light.indexes = {1, 2};
    light.vectors[0] = {colour.x, colour.y, colour.z};
    light.vectors[1].x = colour.w;

    Vector3 origin = sourcePosition;
    if (std::fabs(origin.x) < 0.00001f)
        origin.x = 0.0f;
    if (std::fabs(origin.y) < 0.00001f)
        origin.y = 0.0f;
    if (std::fabs(origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 direction{};
    if (mode == 0 || mode == 3) {
        direction = {origin.x - cameraPosition.x, origin.y - cameraPosition.y, origin.z - cameraPosition.z};
    } else if (mode == 1 || mode == 2 || mode == 4 || mode == 5) {
        const Vector3 &current = g_cameraControl->targetPosition;
        const float deltaX = current.x - cameraPosition.x;
        const float deltaZ = current.z - cameraPosition.z;
        const float distance = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
        const std::int16_t relativeYaw = static_cast<std::int16_t>(yaw - g_cameraControl->nextRotation.yaw);
        const Vector3 projected{
            current.x - sin_fixed16(relativeYaw) * distance,
            current.y,
            current.z + sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(relativeYaw)))) * distance,
        };
        direction = {origin.x - projected.x, origin.y - projected.y, origin.z - projected.z};
    }
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
        direction.z /= length;
    }
    if (mode >= 3 && mode <= 5) {
        light.indexes[1] = 3;
        light.vector20 = {
            origin.x - direction.x * 0.40000001f,
            origin.y - direction.y * 0.40000001f,
            origin.z - direction.z * 0.40000001f,
        };
        light.value1c = 0.25f;
        light.vectors[1].z = 1.5f;
    } else if (mode <= 2) {
        light.values2c = {direction.x, direction.y};
        light.value34 = direction.z;
    }
    selectedLightIndices[slot] = static_cast<std::int16_t>(slot);
    ImmediateLightWork(nullptr);
    return true;
}

void EndTemporaryReflectionLight(TemporaryReflectionLightState &saved) {
    if (!saved.engaged || saved.slot >= selectedLightIndices.size())
        return;
    currentActiveLight = saved.activeLight;
    lightRuntimeEntries[saved.slot] = defaultLight;
    selectedLightIndices[saved.slot] = static_cast<std::int16_t>(saved.slot);
    ImmediateLightWork(nullptr);
    saved.engaged = false;
}

bool BeginTemporaryAttachedLight(std::uint32_t slot, const Vector3 &sourceOrigin, std::int16_t yaw, const Vector4 &colour, TemporaryAttachedLightState &saved) {
    constexpr std::uint32_t nativeLightSlotCount = 16;
    if (slot >= nativeLightSlotCount || saved.engaged || !g_cameraControl)
        return false;

    saved.slot = slot;
    saved.activeLight = currentActiveLight;
    saved.entry = lightRuntimeEntries[slot];
    saved.selectedIndex = selectedLightIndices[slot];
    saved.engaged = true;

    currentActiveLight = 1;
    LightRuntimeEntry &light = lightRuntimeEntries[slot];
    light = defaultLight;
    light.indexes = {1, 2};
    light.vectors[0] = {colour.x, colour.y, colour.z};
    light.vectors[1].x = colour.w;

    const Vector3 &current = g_cameraControl->currentPosition;
    const Vector3 &next = g_cameraControl->nextPosition;
    const float deltaX = current.x - next.x;
    const float deltaZ = current.z - next.z;
    const float distance = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
    const std::int16_t relativeYaw = static_cast<std::int16_t>(yaw - g_cameraControl->nextRotation.yaw);
    const Vector3 projected{
        current.x - sin_fixed16(relativeYaw) * distance,
        current.y,
        current.z + sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(relativeYaw)))) * distance,
    };
    Vector3 origin = sourceOrigin;
    if (std::fabs(origin.x) < 0.00001f)
        origin.x = 0.0f;
    if (std::fabs(origin.y) < 0.00001f)
        origin.y = 0.0f;
    if (std::fabs(origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 direction{
        origin.x - projected.x,
        origin.y - projected.y,
        origin.z - projected.z,
    };
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
        direction.z /= length;
    }
    light.values2c = {direction.x, direction.y};
    light.value34 = direction.z;
    selectedLightIndices[slot] = static_cast<std::int16_t>(slot);
    PublishGlobalLighting();
    SetLegacyLight(slot, light);
    UpdateLegacyLightTransforms();
    return true;
}

void EndTemporaryAttachedLight(TemporaryAttachedLightState &saved) {
    if (!saved.engaged || saved.slot >= selectedLightIndices.size())
        return;
    lightRuntimeEntries[saved.slot] = saved.entry;
    currentActiveLight = saved.activeLight;
    selectedLightIndices[saved.slot] = saved.selectedIndex;
    ImmediateLightWork(nullptr);
    saved.engaged = false;
}

// @verified - p
void SetLightEntryIndex(std::uint32_t index, std::uint32_t value) {
    if (index < lightRuntimeEntries.size() && value <= 1) {
        lightRuntimeEntries[index].indexes[0] = static_cast<std::uint8_t>(value);
    }
}

// @verified - p
std::uint64_t GetLightEntryIndex(std::uint32_t index) {
    return index < lightRuntimeEntries.size() ? static_cast<std::uint32_t>(static_cast<std::int8_t>(lightRuntimeEntries[index].indexes[0])) : 3;
}

// @verified - p
void SetLightConeAngle(std::uint32_t index, std::int16_t angle) {
    if (index < lightRuntimeEntries.size())
        lightRuntimeEntries[index].unknown02 = angle;
}

// @verified - p
void SetLightFlagForIndex(std::uint32_t index, std::int32_t value) {
    if (index >= lightRuntimeEntries.size() || value >= 5)
        return;
    std::uint8_t &flags = lightRuntimeEntries[index].indexes[1];
    const std::uint8_t retained = flags & 8U;
    flags = static_cast<std::uint8_t>((value & 0xf7) | retained);
}

// @verified - p
std::uint64_t SetLightRootPosition(const Vector3 &position) {
    lightRootPosition = position;
    return std::bit_cast<std::uint32_t>(position.z);
}

// @verified - p
void TranslateLightDefinition(std::uint32_t identifier, const Vector3 &translation) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control || !FindMapInfoChunk("LGHT", control->mapInfo))
        return;
    const LightDefinitionEntry *const definition = ResolveDefinition(identifier);
    if (definition)
        lightChunkEnd = lightDefinitionEnds[static_cast<std::size_t>(definition - lightDefinitions.data())];
    std::uint32_t tag;
    if (!definition || !Read(definition->block, tag) || tag != lghtToken)
        return;
    for (std::uint32_t index = 0; index < lightRuntimeEntries.size(); ++index) {
        const LightRuntimeEntry &light = lightRuntimeEntries[index];
        if ((light.indexes[1] & 0xf7U) != 3U)
            continue;
        const Vector3 position{(lightRootPosition.x + light.vector20.x) + translation.x, (lightRootPosition.y + light.vector20.y) + translation.y,
            (lightRootPosition.z + light.vector20.z) + translation.z};
        SetLightRuntimePosition(index, position);
    }
}

// @verified - p
void PrioritizeLight(std::int16_t index) {
    if (priorityLightCount < priorityLightIndices.size()) {
        priorityLightIndices[priorityLightCount++] = index;
    }
}

// @verified - p
std::uint64_t SetRankedLightLimit(std::uint32_t count) {
    if (count <= 12U)
        rankedLightLimit = count;
    return rankedLightLimit;
}

void ConfigureDynamicLight(std::uint32_t slot, std::uint8_t kind, const Vector4 &colour, float scale) {
    if (slot >= selectedLightIndices.size())
        return;
    LightRuntimeEntry &light = lightRuntimeEntries[slot];
    light = defaultLight;
    light.indexes = {0, kind};
    light.vectors[0] = {colour.x, colour.y, colour.z};
    light.vectors[1] = {colour.w, 1.0f, scale};
    selectedLightIndices[slot] = static_cast<std::int16_t>(slot);
    ImmediateLightWork(nullptr);
}

void SetDynamicLightPosition(std::uint32_t slot, const Vector3 &position) {
    if (slot >= selectedLightIndices.size())
        return;
    lightRuntimeEntries[slot].vector20 = position;
}

void SetDynamicLightEnabled(std::uint32_t slot, bool enabled) {
    SetLegacySelectedLightEnabled(slot, enabled);
}

// @verified - p
void ReinitializeLights(std::int32_t mode) {
    currentActiveLight = 0;
    UpdateGlobalLighting({0.3f, 0.3f, 0.3f, 0.3f});
    for (std::uint32_t index = 0; index < lightRuntimeEntries.size(); ++index)
        ResetLightRuntimeEntry(index);
    const LightRuntimeEntry &defaults = defaultLight;
    for (std::uint32_t slot = 0; slot < selectedLightIndices.size(); ++slot)
        ResetLegacySelectedLight(slot, 0, defaults);
    UpdateLegacyLightTransforms();
    currentActiveLight = mode;
    if (mode == 1)
        InitializeDefaultDirectionalLight();
    LightModeNullsub(mode == 1 ? 0 : 1);
    lightRootPosition = {};
}

std::int64_t LightTask_Enqueue() {
    rankMapLights = false;
    currentActiveLight = 1;
    previousActiveLight = 1;
    characterLightCandidateCount = 0;
    previousGlobalLighting = globalLighting;
    lightRootPosition = {};
    priorityLightCount = 0;
    rankedLightLimit = 12;
    lightChunkEnd = nullptr;
    lightDefinitions.fill({nullptr, 0xffffffffU});
    lightDefinitionEnds.fill(nullptr);
    lightRuntimeEntries.fill(defaultLight);

    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const MapInfoChunk *const chunk = control ? FindMapInfoChunk("LGHT", control->mapInfo) : nullptr;
    if (chunk) {
        BuildDefinitionTable(chunk);
        LGHT_Parser_Internal(0);
    } else {
        ReinitializeLights(1);
        lightRuntimeEntries[0].values2c = {0.0f, -0.70710701f};
        lightRuntimeEntries[0].value34 = -0.70710701f;
    }
    previousActiveLight = currentActiveLight;
    previousGlobalLighting = globalLighting;

    HLib::EnqueueTaskWithoutParameter(LightTask_Callback, 0x0d, 2, static_cast<std::int32_t>(lghtToken));
    return currentActiveLight;
}

} // namespace shenmue::s2
