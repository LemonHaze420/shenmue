#pragma once

#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue {
struct D3TModelSprite;
}

namespace shenmue::s1 {
struct RenderArrayEntry;

struct ModelWeatherSpriteBinding {
    const D3TModelSprite *sprite;
    std::uint64_t node;
    const char *textureName;
};
static_assert(sizeof(ModelWeatherSpriteBinding) == 24);

struct ModelLightRecord {
    std::byte transformData[192];
    Vector3 colour;
    std::uint32_t type;
    Vector3 position;
    float range;
    Vector3 direction;
    float coneCosine;
    std::uint32_t shadowMode;
    float shadowBias;
    std::uint32_t shadowIndex;
    std::uint32_t textureHandle;
    std::uint32_t parameterEnabled;
    float parameter104;
    float parameter108;
    std::uint32_t ownerCache;
    std::uint32_t ownerNode;
    std::uint32_t nodeCache;
    std::uint32_t nodeHash;
    Vector3 localRotationDegrees;
    std::uint32_t parameter128;
    std::uint32_t state;
    const char *textureName;
};
struct ModelShadowMappingNode {
    ModelShadowMappingNode *children[2];
    std::int32_t x, y, width, height;
    std::uint32_t slot;
    std::uint32_t reserved24;
    Vector2 *output;
};
struct ModelShadowMappingRequest {
    std::int32_t x, y, width, height;
    std::uint8_t processed;
    std::byte reserved11[3];
    std::uint32_t slot;
    Vector2 *output;
};
struct ModelShadowMapping {
    std::uint64_t reserved00;
    ModelShadowMappingNode *nodes;
    std::uint32_t nodeCount, nodeCapacity;
    ModelShadowMappingRequest *requests;
    std::uint32_t requestCount, requestCapacity;
};
struct ModelLightOverrides {
    std::uint64_t *values;
    std::uint32_t count, capacity;
};
static_assert(sizeof(ModelLightRecord) == 312);
static_assert(offsetof(ModelLightRecord, colour) == 192);
static_assert(offsetof(ModelLightRecord, shadowMode) == 240);
static_assert(offsetof(ModelLightRecord, ownerCache) == 268);
static_assert(offsetof(ModelLightRecord, nodeCache) == 276);
static_assert(offsetof(ModelLightRecord, localRotationDegrees) == 284);
static_assert(offsetof(ModelLightRecord, parameter128) == 296);
static_assert(offsetof(ModelLightRecord, textureName) == 304);
static_assert(sizeof(ModelShadowMappingNode) == 48);
static_assert(offsetof(ModelShadowMappingNode, output) == 40);
static_assert(sizeof(ModelShadowMappingRequest) == 32);
static_assert(sizeof(ModelShadowMapping) == 40);

extern std::array<ModelLightRecord, 16> g_modelLights;
extern std::uint32_t g_modelLightCount;
extern ModelLightOverrides g_modelLightOverrides;
extern std::array<std::uint8_t, 32> g_modelShadowSlots;
extern std::array<std::uint8_t, 32> g_modelShadowDirty;
extern ModelShadowMapping g_modelShadowMapping;
extern std::int32_t g_modelReflectionSize;
extern Vector2 g_modelReflectionAtlasSize;

std::uint32_t AllocateModelShadowSlots(std::uint32_t count);
void RemoveModelShadowMappingRequest(std::uint32_t slot);
void ReleaseModelShadowMapping();
void ReleaseModelLightShadowSlots(ModelLightRecord &light);
void RemoveHmdlLightBinding(std::uint32_t index);
void RemoveHmdlLightOwner(RenderArrayEntry &entry);
bool AllocateHmdlReflectionSlot(RenderArrayEntry &entry);
std::uint64_t ResolveHmdlLightOwner(std::uint64_t node);
void AddHmdlLightOverride(std::uint64_t owner);
std::uint32_t AssignHmdlWeatherSprites(std::uint64_t handle, RenderArrayEntry &entry);

} // namespace shenmue::s1
