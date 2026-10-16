#include "d3t/material.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/content.hpp"
#include "d3t/shader_load.hpp"
#include "d3t/mip_streaming.hpp"
#include "d3t/texture.hpp"
#include <atomic>

#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <shared_mutex>

namespace shenmue::d3t {
CommonShaderCache g_commonShaderCache{};

namespace {
std::shared_mutex g_commonShaderCacheLock;
std::atomic<std::uint32_t> g_d3tMaterialSortKey{0};


bool GetMaterialTextureSlot(std::uint32_t type, std::uint32_t &slot) {
    switch (type) {
    case 0:
    case 5:
    case 23:
    case 38:
        slot = 0;
        return true;
    case 6:
    case 21:
    case 39:
        slot = 1;
        return true;
    case 7:
    case 40:
        slot = 2;
        return true;
    case 8:
    case 41:
        slot = 3;
        return true;
    case 9:
        slot = 6;
        return true;
    case 10:
        slot = 8;
        return true;
    case 11:
    case 42:
        slot = 4;
        return true;
    case 14:
    case 43:
        slot = 5;
        return true;
    case 15:
        slot = 7;
        return true;
    case 17:
    case 22:
        slot = 9;
        return true;
    case 33:
        slot = 10;
        return true;
    case 34:
        slot = 11;
        return true;
    default:
        return false;
    }
}

std::uint32_t FindCommonShaderCacheIndex(const CommonShaderCache &cache, std::uint32_t hash) {
    if (!cache.capacity)
        return UINT32_MAX;
    std::uint32_t index = hash % cache.capacity;
    for (;;) {
        if (cache.hashes[index] == hash)
            return index;
        if (!cache.links[index])
            return UINT32_MAX;
        index = (index + cache.links[index]) % cache.capacity;
    }
}

bool GrowCommonShaderCache(CommonShaderCache &cache, std::uint32_t capacity) {
    CommonShaderCache grown{.entries = static_cast<CommonShaderCacheEntry *>(std::calloc(capacity, sizeof(CommonShaderCacheEntry))),
        .hashes = static_cast<std::uint32_t *>(std::calloc(capacity, sizeof(std::uint32_t))),
        .links = static_cast<std::uint16_t *>(std::calloc(capacity, sizeof(std::uint16_t))),
        .capacity = capacity,
        .count = 0};
    if (!grown.entries || !grown.hashes || !grown.links) {
        std::free(grown.entries);
        std::free(grown.hashes);
        std::free(grown.links);
        return false;
    }
    for (std::uint32_t index = 0; index < cache.capacity; ++index) {
        if (cache.hashes[index])
            InsertCommonShaderCacheEntry(grown, cache.entries[index], cache.hashes[index]);
    }
    std::free(cache.entries);
    std::free(cache.hashes);
    std::free(cache.links);
    cache = grown;
    return true;
}

void CommonShaderCleanup(void *context) {
    RemoveCachedCommonShader(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(context)));
}
}

void BuildMaterialShaderCapabilities(char *destination, const D3TModelMaterial &material, const D3TModelMaterialParameter *parameters, bool instanced,
    bool hasPrepass, bool debug, bool pcf, bool reflection, std::int32_t platform) {
    bool textures[46]{};
    char *cursor = destination;
    *cursor = 0;
    const auto append = [&cursor](const char *capability) {
        const std::size_t size = std::strlen(capability);
        std::memcpy(cursor, capability, size);
        cursor += size;
        *cursor++ = ';';
        *cursor = 0;
    };
    for (std::uint32_t index = 0; index < material.parameterCount; ++index) {
        const D3TModelMaterialParameter &parameter = parameters[index];
        if (parameter.type >= 46)
            continue;
        textures[parameter.type] = parameter.texture != 0;
        if (parameter.type == 32 &&
            (std::bit_cast<std::uint32_t>(parameter.value.x) || std::bit_cast<std::uint32_t>(parameter.value.y) ||
                std::bit_cast<std::uint32_t>(parameter.value.z) || std::bit_cast<std::uint32_t>(parameter.value.w)))
            append("_ANIM");
        if (parameter.type == 45 && std::bit_cast<std::uint32_t>(parameter.value.x))
            append("_VERTANIM");
    }
    if (material.vertexFormat & 0x40U)
        append("_SKIN");
    if (material.vertexFormat & 0x20U)
        append("_VCOL");
    if (material.vertexFormat & 0x10U)
        append("_UV2");
    if ((material.materialType == 2 || material.materialType == 9) && (material.vertexFormat & 0x10U))
        append("_ALB_DTL");
    if (material.materialType == 2 && (material.vertexFormat & 0x10U) && textures[17])
        append("_NRM_DTL");
    if (material.materialType == 6) {
        append("_MLT_DTL");
        if (textures[17] || textures[34])
            append("_MLT_DTL_NRM");
    }
    if (material.materialType == 3)
        append("_WATER");
    if (material.materialType == 8)
        append("_WATERMIRROR");
    if (material.materialType == 13)
        append("_SSSSS");
    if (material.unlit)
        append("_UNLIT");
    if (material.subsurfaceScattering)
        append("_SSS");
    if (material.softParticle)
        append("_SOFT_PART");
    if (textures[14])
        append("_ALB_MSK");
    if (textures[15])
        append("_CAVMAP");
    if (textures[10])
        append("_AOMAP");
    if (textures[9])
        append("_EMIS");
    if (material.materialType == 0)
        append("MAT_PHONG");
    else if (material.materialType == 9)
        append("MAT_SKY");
    else if (material.materialType == 11)
        append("MAT_SMSD");
    else
        append("_SHAD");
    if (material.materialType == 7)
        append("_MIRROR");
    if (material.materialType == 10)
        append("_REFRACT");
    if (material.materialType == 12)
        append("_HAIR");
    switch (material.blendType) {
    case 1:
        append("_A_TEST");
        break;
    case 2:
        append("_A_BLEND");
        break;
    case 4:
        append("_A_CVRG");
        break;
    case 5:
        append("_A_TESTAA");
        break;
    default:
        break;
    }
    if (instanced)
        append("_INST");
    if (hasPrepass)
        append("_HASPRE");
    if (material.doubleSided)
        append("_DBLSIDE");
    if (material.materialType == 4 || material.materialType == 5)
        append("_BILLBOARD");
    if (debug)
        append("_DBG");
    if (pcf)
        append("_PCF");
    if (reflection)
        append("_REFLECT");
    if (platform == 1)
        append("_XBOXONE");
}

void SetMasterShaderBindings(MasterShaderBindings &bindings, const D3TModelMaterial &material, D3TModelMaterialRuntime *runtime,
    const D3TModelMaterialParameter *parameters, bool instanced, std::int32_t platform) {
    char capabilities[1024];
    std::uint32_t count = 0;
    const auto caps = [&](bool prepass, bool debug, bool pcf, bool reflection) {
        BuildMaterialShaderCapabilities(capabilities, material, parameters, instanced, prepass, debug, pcf, reflection, platform);
    };
    const auto add = [&](const char *name, std::uint32_t slot, d3t::ShaderType type) {
        MaterialShaderBinding &binding = bindings.bindings[count++];
        std::snprintf(binding.path, sizeof(binding.path), "/engine/assets/shaders/%s.hlsl?CAPS=%s", name, capabilities);
        binding.destination = runtime ? &runtime->shaders[slot] : nullptr;
        binding.type = type;
    };
    constexpr d3t::ShaderType vertex = d3t::ShaderType::Vertex;
    constexpr d3t::ShaderType pixel = d3t::ShaderType::Pixel;
    caps(true, false, false, false);
    add("uber_vs", 11, vertex);
    add("uber_ps", 12, pixel);
    caps(true, false, true, false);
    add("uber_ps", 13, pixel);
    caps(false, false, false, false);
    add("depth_vs", 0, vertex);
    add("depth_ps", 1, pixel);
    add("depth_prepass_vs", 2, vertex);
    add("depth_prepass_ps", 3, pixel);
    add("uber_vs", 5, vertex);
    add("uber_ps", 6, pixel);
    caps(false, false, false, true);
    add("depth_prepass_ps", 4, pixel);
    caps(false, false, true, true);
    add("uber_ps", 14, pixel);
    caps(false, false, true, false);
    add("uber_ps", 7, pixel);
    caps(true, true, false, false);
    add("uber_vs", 15, vertex);
    add("uber_ps", 16, pixel);
    caps(true, true, true, false);
    add("uber_ps", 17, pixel);
    caps(false, true, false, false);
    add("uber_vs", 8, vertex);
    add("uber_ps", 9, pixel);
    caps(false, true, true, false);
    add("uber_ps", 10, pixel);
    bindings.count = count;
}

bool InsertCommonShaderCacheEntry(CommonShaderCache &cache, const CommonShaderCacheEntry &entry, std::uint32_t hash) {
    if (!cache.capacity || cache.count >= cache.capacity - (cache.capacity >> 2U)) {
        if (!GrowCommonShaderCache(cache, cache.capacity >= 64 ? cache.capacity + (cache.capacity >> 2U) : 64))
            return false;
    }
    std::uint32_t index = hash % cache.capacity;
    std::uint32_t empty = UINT32_MAX;
    for (;;) {
        if (cache.hashes[index] == hash)
            return true;
        if (!cache.hashes[index]) {
            if (empty == UINT32_MAX)
                empty = index;
            if (!cache.links[index]) {
                cache.hashes[index] = hash;
                cache.entries[index] = entry;
                ++cache.count;
                return true;
            }
        }
        if (!cache.links[index])
            break;
        index = (index + cache.links[index]) % cache.capacity;
    }
    if (empty != UINT32_MAX) {
        cache.hashes[empty] = hash;
        cache.entries[empty] = entry;
        ++cache.count;
        return true;
    }
    std::uint32_t freeIndex = (index + 1) % cache.capacity;
    while (freeIndex != index && cache.hashes[freeIndex])
        freeIndex = (freeIndex + 1) % cache.capacity;
    if (freeIndex == index)
        return false;
    cache.hashes[freeIndex] = hash;
    cache.entries[freeIndex] = entry;
    ++cache.count;
    cache.links[index] = static_cast<std::uint16_t>(freeIndex < index ? freeIndex + cache.capacity - index : freeIndex - index);
    return true;
}

bool FindCommonShaderCacheEntry(const CommonShaderCache &cache, std::uint32_t hash, CommonShaderCacheEntry &entry) {
    const std::uint32_t index = FindCommonShaderCacheIndex(cache, hash);
    if (index == UINT32_MAX)
        return false;
    entry = cache.entries[index];
    return true;
}

bool RemoveCommonShaderCacheEntry(CommonShaderCache &cache, std::uint32_t hash) {
    if (!cache.capacity)
        return false;
    std::uint32_t index = hash % cache.capacity;
    std::uint32_t previous = UINT32_MAX;
    while (cache.hashes[index] != hash) {
        if (!cache.links[index])
            return false;
        previous = index;
        index = (index + cache.links[index]) % cache.capacity;
    }
    cache.hashes[index] = 0;
    cache.entries[index] = {};
    if (previous != UINT32_MAX && !cache.links[index])
        cache.links[previous] = 0;
    --cache.count;
    return true;
}

bool RemoveCachedCommonShader(std::uint32_t hash) {
    const std::unique_lock lock(g_commonShaderCacheLock);
    CommonShaderCacheEntry entry;
    return FindCommonShaderCacheEntry(g_commonShaderCache, hash, entry) && RemoveCommonShaderCacheEntry(g_commonShaderCache, hash);
}

void CompleteCachedShaderLoad(void *data, std::uint32_t contentHandle, void *bytecode, std::uint64_t bytecodeLength, const char *) {
    const std::shared_lock lock(g_commonShaderCacheLock);
    const std::uint32_t hash = *static_cast<std::uint32_t *>(data);
    const std::uint32_t index = FindCommonShaderCacheIndex(g_commonShaderCache, hash);
    if (index == UINT32_MAX) {
        d3t::ProcessThreadStates(contentHandle, 0);
        return;
    }
    CommonShaderCacheEntry &entry = g_commonShaderCache.entries[index];
    d3t::CreateShaderResource(entry.shader, bytecode, bytecodeLength, entry.type);
    entry.loadState = 0;
    d3t::SetShaderCleanup(entry.shader, CommonShaderCleanup, reinterpret_cast<void *>(static_cast<std::uintptr_t>(hash)));
    d3t::ProcessThreadStates(contentHandle, entry.type == d3t::ShaderType::Vertex ? 1 : 0);
}

void CreateShaderAsync(const char *path, d3t::ShaderType type, std::uint32_t &shader, std::uint32_t renderHandle) {
    const std::unique_lock lock(g_commonShaderCacheLock);
    const std::uint32_t hash = d3t::d3t_HashData(path, static_cast<std::uint32_t>(std::strlen(path)));
    CommonShaderCacheEntry entry{};
    if (FindCommonShaderCacheEntry(g_commonShaderCache, hash, entry)) {
        shader = entry.shader;
        d3t::RetainShader(shader);
        return;
    }
    void *data = nullptr;
    const std::uint32_t load = d3t::d3t_AllocateAsyncLoadRecord(&data);
    if (!(load >> 16U))
        return;
    *static_cast<std::uint32_t *>(data) = hash;
    shader = d3t::CreateShader(static_cast<std::int32_t>(type));
    entry.type = type;
    entry.shader = shader;
    std::strncpy(entry.path, path, sizeof(entry.path));
    InsertCommonShaderCacheEntry(g_commonShaderCache, entry, hash);
    d3t::RetainShader(shader);
    const d3t::D3TShaderLoadTask task{.path = path,
        .renderHandle = renderHandle,
        .reserved_0c = 0,
        .thread = {.callback = CompleteCachedShaderLoad, .data = data},
        .destination = nullptr,
        .destinationCapacity = 0,
        .option = 0,
        .reserved_31 = {}};
    d3t::PushShaderLoadToThreadQueue(&task);
}

bool PopulateUberMaterialConstants(UberMaterialConstants &constants, const D3TModelMaterialParameter *parameters, const D3TModelMaterial &material) {
    constants.materialBlendType = static_cast<std::int32_t>(material.blendType);
    constants.wetnessType = material.wetnessType;
    constants.useClearCoat = material.clearCoat;
    for (std::uint32_t index = 0; index < material.parameterCount; ++index) {
        const D3TModelMaterialParameter &parameter = parameters[index];
        const Vector4 &value = parameter.value;
        switch (parameter.type) {
        case 5:
            constants.albedo = value;
            break;
        case 6:
            constants.normalScale = value.x;
            break;
        case 7:
            constants.roughness = value.x;
            break;
        case 8:
            constants.metalness = value.x;
            break;
        case 9:
            constants.emissiveColor = value;
            break;
        case 10:
            constants.ambientOcclusion = value.x;
            break;
        case 11:
            constants.albedoDetail = value;
            break;
        case 12:
            constants.albedoDetailUVScaleBias = value;
            break;
        case 13:
            constants.albedoDetailBlendOp = std::bit_cast<std::int32_t>(value.x);
            break;
        case 14:
            constants.albedoMask = value.x;
            break;
        case 15:
            constants.cavity = value.x;
            break;
        case 16:
            constants.useAlternateUV = std::bit_cast<std::uint32_t>(value.x) & 0xffU;
            break;
        case 17:
            constants.normalDetailScale = value.x;
            break;
        case 18:
            constants.alphaRef = value.x;
            break;
        case 19:
            constants.uvOffset[0] = value.x;
            constants.uvOffset[1] = value.y;
            constants.uvOffsetSpeed = value.z;
            break;
        case 20:
            constants.billboardClampedY = std::bit_cast<std::uint32_t>(value.x) & 0xffU;
            break;
        case 32:
            constants.animationFramesX = std::bit_cast<std::int32_t>(value.x);
            constants.animationFramesY = std::bit_cast<std::int32_t>(value.y);
            constants.animationSpeed = std::bit_cast<std::int32_t>(value.z);
            constants.randomiseStartFrame = std::bit_cast<std::int32_t>(value.w);
            break;
        case 33:
            constants.albedoDetail02 = value;
            break;
        case 35:
            constants.albedoDetail02UVScaleBias = value;
            break;
        case 36:
            constants.baseUVScaleBias = value;
            break;
        case 37:
            constants.uvOffsetDetail[0] = value.x;
            constants.uvOffsetDetail[1] = value.y;
            constants.uvOffsetSpeedDetail = value.z;
            break;
        case 44:
            constants.refractionScale = value.x;
            break;
        case 45:
            constants.vertexAnimID = std::bit_cast<std::int32_t>(value.x);
            break;
        default:
            break;
        }
    }
    return true;
}

bool PopulateWaterMaterialConstants(WaterMaterialConstants &constants, const D3TModelMaterialParameter *parameters, const D3TModelMaterial &material) {
    constants.materialBlendType = material.blendType;
    for (std::uint32_t index = 0; index < material.parameterCount; ++index) {
        const D3TModelMaterialParameter &parameter = parameters[index];
        const Vector4 &value = parameter.value;
        switch (parameter.type) {
        case 5:
        case 23:
            constants.color = value;
            break;
        case 7:
            constants.roughness = value.x;
            break;
        case 8:
            constants.metalness = value.x;
            break;
        case 21:
            constants.parameter21 = value.x;
            constants.parameter21Secondary = value.y;
            break;
        case 22:
            constants.parameter22 = value.x;
            constants.parameter22Secondary = value.y;
            break;
        case 24:
            constants.parameter24 = value;
            break;
        case 25:
            constants.parameter25 = value;
            break;
        case 26:
            constants.waveDirection[0] = std::sin(value.x * 0.017453279f);
            constants.waveDirection[1] = std::cos(value.x * 0.017453279f);
            constants.waveDirection[2] = std::sin((value.x + 90.0f) * 0.017453279f);
            constants.waveDirection[3] = std::cos((value.x + 90.0f) * 0.017453279f);
            break;
        case 27:
            constants.parameter27 = value.x;
            break;
        case 28:
            constants.parameter28 = value.x;
            break;
        case 29:
            constants.parameter29 = value.x;
            break;
        case 30:
            constants.parameter30 = value.x;
            break;
        case 31:
            constants.parameter31 = value.x;
            break;
        default:
            break;
        }
    }
    return true;
}

bool PopulateSkyMaterialConstants(SkyMaterialConstants &constants, const D3TModelMaterialParameter *parameters, const D3TModelMaterial &material) {
    constants.materialBlendType = material.blendType;
    for (std::uint32_t index = 0; index < material.parameterCount; ++index) {
        const D3TModelMaterialParameter &parameter = parameters[index];
        const Vector4 &value = parameter.value;
        switch (parameter.type) {
        case 9:
            constants.emissiveAlpha = value.w;
            break;
        case 19:
            constants.uvOffset[0] = value.x;
            constants.uvOffset[1] = value.y;
            constants.uvOffsetSpeed = value.z;
            break;
        case 37:
            constants.uvOffsetDetail[0] = value.x;
            constants.uvOffsetDetail[1] = value.y;
            constants.uvOffsetSpeedDetail = value.z;
            break;
        default:
            break;
        }
    }
    return true;
}

void GetOrCreateSceneEntity(
    const D3TModelMaterial &material, const D3TModelMaterialParameter *parameters, D3TModelMaterialRuntime &runtime, D3TDeviceContext *context) {
    void *mapped = nullptr;
    if (runtime.constantBuffer && context)
        mapped = GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(runtime.constantBuffer), 1, 0);
    else
        ValidateAndCommitBuffer(static_cast<std::int32_t>(runtime.constantBuffer));
    union {
        UberMaterialConstants uber;
        WaterMaterialConstants water;
        SkyMaterialConstants sky;
        std::byte bytes[220];
    } constants{};
    std::size_t size = 0;
    switch (material.materialType) {
    case 0:
        for (std::uint32_t index = 0; index < 4; ++index)
            std::memcpy(constants.bytes + index * 16, &parameters[index].value, 16);
        std::memcpy(constants.bytes + 64, &parameters[4].value.x, 4);
        size = 68;
        break;
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 10:
    case 12:
    case 13:
        PopulateUberMaterialConstants(constants.uber, parameters, material);
        constants.uber.materialUID = static_cast<std::int32_t>(runtime.sortKey);
        size = sizeof(constants.uber);
        break;
    case 3:
    case 8:
        PopulateWaterMaterialConstants(constants.water, parameters, material);
        size = sizeof(constants.water);
        break;
    case 9:
        PopulateSkyMaterialConstants(constants.sky, parameters, material);
        size = sizeof(constants.sky);
        break;
    case 11:
        for (std::uint32_t index = 0; index < 3; ++index)
            std::memcpy(constants.bytes + index * 16, &parameters[index].value, 16);
        std::memcpy(constants.bytes + 48, &parameters[3].value.x, 4);
        std::memcpy(constants.bytes + 52, &parameters[4].value.x, 4);
        size = 56;
        break;
    default:
        if (mapped)
            DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(runtime.constantBuffer));
        return;
    }
    if (mapped) {
        std::memcpy(mapped, constants.bytes, size);
        DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(runtime.constantBuffer));
    } else {
        const BufferDescription description{.flags = 0, .size = 220, .count = 0, .flags3 = 2, .buffer = constants.bytes, .name = nullptr};
        runtime.constantBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));
    }
}



void BindModelMaterialTextures(D3TModelRecord &model, std::uint32_t instanceHandle, std::int32_t textureMipMode) {
    for (std::uint32_t materialIndex = 0; materialIndex < model.materialCount; ++materialIndex) {
        const D3TModelMaterial &material = model.materials[materialIndex];
        D3TModelMaterialRuntime &runtime = model.materialRuntime[materialIndex];
        for (std::uint32_t parameterIndex = 0; parameterIndex < material.parameterCount; ++parameterIndex) {
            const D3TModelMaterialParameter &parameter = model.materialParameters[material.firstParameter + parameterIndex];
            std::uint32_t slot = 0;
            if (!d3t::GetMaterialTextureSlot(parameter.type, slot))
                continue;
            if (parameter.texture) {
                const D3TModelTexture &texture = model.textures[parameter.texture - 1];
                std::uint32_t source = 0;
                switch (parameter.type) {
                case 6:
                case 17:
                case 21:
                case 22:
                case 34:
                    source = 2;
                    break;
                case 7:
                case 8:
                case 10:
                case 14:
                case 15:
                    source = 1;
                    break;
                default:
                    break;
                }
                std::uint32_t textureHandle = 0;
                const char *const path = texture.pathOffset < model.stringsSize ? model.strings + texture.pathOffset : nullptr;
                SelectMipForPVR(path, reinterpret_cast<const PvrMipDescription *>(&texture), source, &textureHandle, instanceHandle, textureMipMode);
                runtime.textures[slot] = textureHandle;
                continue;
            }
            std::uint32_t textureHandle = d3t::g_defaultWhiteTexture;
            if (parameter.type == 6 || parameter.type == 17 || parameter.type == 34)
                textureHandle = d3t::g_defaultNormalTexture;
            else if (parameter.type == 9)
                textureHandle = d3t::g_defaultBlackTexture;
            runtime.textures[slot] = textureHandle;
            IncrementTextureById(textureHandle);
        }
    }
    for (std::uint32_t index = 0; index < model.spriteCount; ++index) {
        D3TModelSprite &sprite = model.sprites[index];
        if (!sprite.texture)
            continue;
        const D3TModelTexture &texture = model.textures[sprite.texture - 1];
        std::uint32_t textureHandle = 0;
        const char *const path = texture.pathOffset < model.stringsSize ? model.strings + texture.pathOffset : nullptr;
        SelectMipForPVR(path, reinterpret_cast<const PvrMipDescription *>(&texture), 0, &textureHandle, instanceHandle, textureMipMode);
        sprite.textureHandle = textureHandle;
    }
}

void CreateModelMaterials(D3TModelRecord &model, std::uint32_t instanceHandle, std::int32_t textureMipMode) {
    BindModelMaterialTextures(model, instanceHandle, textureMipMode);
    MasterShaderBindings bindings;
    for (std::uint32_t index = 0; index < model.materialCount; ++index) {
        D3TModelMaterialRuntime &runtime = model.materialRuntime[index];
        const D3TModelMaterial &material = model.materials[index];
        const D3TModelMaterialParameter *const parameters = model.materialParameters + material.firstParameter;
        runtime.sortKey = g_d3tMaterialSortKey.fetch_add(1, std::memory_order_seq_cst);
        SetMasterShaderBindings(bindings, material, &runtime, parameters, model.flag0 != 0, 0);
        for (std::uint32_t shader = 0; shader < bindings.count; ++shader) {
            const MaterialShaderBinding &binding = bindings.bindings[shader];
            CreateShaderAsync(binding.path, binding.type, *binding.destination, instanceHandle);
        }
        GetOrCreateSceneEntity(material, parameters, runtime, nullptr);
    }
}
} // namespace shenmue::d3t
