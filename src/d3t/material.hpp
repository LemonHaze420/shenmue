#pragma once

#include "d3t/model.hpp"
#include "d3t/buffer.hpp"
#include "d3t/shader.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue ::d3t {
bool GetMaterialTextureSlot(std::uint32_t type, std::uint32_t &slot);
}

namespace shenmue {
struct UberMaterialConstants {
    Vector4 albedo;
    Vector4 albedoDetail;
    Vector4 albedoDetail02;
    Vector4 albedoDetailUVScaleBias;
    Vector4 albedoDetail02UVScaleBias;
    Vector4 baseUVScaleBias;
    Vector4 emissiveColor;
    float roughness;
    float metalness;
    float ambientOcclusion;
    float cavity;
    float albedoMask;
    float alphaRef;
    std::int32_t albedoDetailBlendOp;
    std::int32_t materialBlendType;
    float uvOffset[2];
    float uvOffsetSpeed;
    float normalScale;
    std::int32_t animationFramesX;
    std::int32_t animationFramesY;
    std::int32_t animationSpeed;
    std::int32_t randomiseStartFrame;
    std::int32_t materialUID;
    std::int32_t vertexAnimID;
    float normalDetailScale;
    std::int32_t useAlternateUV;
    std::int32_t billboardClampedY;
    std::int32_t useClearCoat;
    float uvOffsetDetail[2];
    float uvOffsetSpeedDetail;
    float refractionScale;
    std::int32_t wetnessType;
};


struct WaterMaterialConstants {
    Vector4 color;
    Vector4 parameter24;
    Vector4 parameter25;
    float waveDirection[4];
    float roughness;
    float metalness;
    std::uint32_t materialBlendType;
    float parameter21;
    float parameter31;
    float parameter30;
    float parameter22;
    float parameter27;
    float parameter28;
    float parameter29;
    float parameter21Secondary;
    float parameter22Secondary;
};

struct SkyMaterialConstants {
    float uvOffset[2];
    float uvOffsetSpeed;
    float emissiveAlpha;
    float uvOffsetDetail[2];
    float uvOffsetSpeedDetail;
    std::uint32_t materialBlendType;
};

struct CommonShaderCacheEntry {
    d3t::ShaderType type;
    std::uint32_t shader;
    std::uint32_t loadState;
    char path[1024];
};

struct CommonShaderCache {
    CommonShaderCacheEntry *entries;
    std::uint32_t *hashes;
    std::uint16_t *links;
    std::uint32_t capacity;
    std::uint32_t count;
};

struct MaterialShaderBinding {
    char path[1024];
    std::uint32_t *destination;
    d3t::ShaderType type;
};

struct MasterShaderBindings {
    MaterialShaderBinding bindings[18];
    std::uint32_t count;
};

static_assert(sizeof(UberMaterialConstants) == 220);
static_assert(offsetof(UberMaterialConstants, materialUID) == 176);
static_assert(offsetof(UberMaterialConstants, wetnessType) == 216);
static_assert(sizeof(WaterMaterialConstants) == 112);
static_assert(sizeof(SkyMaterialConstants) == 32);
static_assert(sizeof(CommonShaderCacheEntry) == 1036);
static_assert(sizeof(CommonShaderCache) == 32);
static_assert(sizeof(MaterialShaderBinding) == 1040);
static_assert(offsetof(MasterShaderBindings, count) == 0x4920);

namespace d3t {
extern CommonShaderCache g_commonShaderCache;
void BindModelMaterialTextures(D3TModelRecord &model, std::uint32_t renderHandle, std::int32_t textureMipMode);
void CreateModelMaterials(D3TModelRecord &model, std::uint32_t renderHandle, std::int32_t textureMipMode);

void BuildMaterialShaderCapabilities(char *destination, const D3TModelMaterial &material, const D3TModelMaterialParameter *parameters, bool instanced,
    bool hasPrepass, bool debug, bool pcf, bool reflection, std::int32_t platform);
void SetMasterShaderBindings(MasterShaderBindings &bindings, const D3TModelMaterial &material, D3TModelMaterialRuntime *runtime,
    const D3TModelMaterialParameter *parameters, bool instanced, std::int32_t platform);
bool InsertCommonShaderCacheEntry(CommonShaderCache &cache, const CommonShaderCacheEntry &entry, std::uint32_t hash);
bool FindCommonShaderCacheEntry(const CommonShaderCache &cache, std::uint32_t hash, CommonShaderCacheEntry &entry);
bool RemoveCommonShaderCacheEntry(CommonShaderCache &cache, std::uint32_t hash);
bool RemoveCachedCommonShader(std::uint32_t hash);
void CompleteCachedShaderLoad(void *data, std::uint32_t contentHandle, void *bytecode, std::uint64_t bytecodeLength, const char *sourcePath);
void CreateShaderAsync(const char *path, d3t::ShaderType type, std::uint32_t &shader, std::uint32_t renderHandle);
bool PopulateUberMaterialConstants(UberMaterialConstants &constants, const D3TModelMaterialParameter *parameters, const D3TModelMaterial &material);
bool PopulateWaterMaterialConstants(WaterMaterialConstants &constants, const D3TModelMaterialParameter *parameters, const D3TModelMaterial &material);
bool PopulateSkyMaterialConstants(SkyMaterialConstants &constants, const D3TModelMaterialParameter *parameters, const D3TModelMaterial &material);
void GetOrCreateSceneEntity(
    const D3TModelMaterial &material, const D3TModelMaterialParameter *parameters, D3TModelMaterialRuntime &runtime, D3TDeviceContext *context);

} // namespace d3t

} // namespace shenmue
