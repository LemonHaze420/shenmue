#include "d3t/model.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/buffer.hpp"
#include "d3t/fvf.hpp"
#include "d3t/texture.hpp"
#include "d3t/shader.hpp"

#include <bit>
#include <cstdlib>
#include <cstring>

namespace shenmue::d3t {
D3TModelTable g_d3tModels{};

namespace {


std::uint32_t CreateModelInputLayout(std::uint32_t vertexFormat) {
    d3t::FVFDescription description{};
    std::uint32_t count = 0;
    const auto add = [&description, &count](const char *name, std::uint32_t index, std::uint32_t format) {
        description.elements[count++] = {.semanticName = name,
            .semanticIndex = index,
            .format = format,
            .inputClassification = 0,
            .inputSlot = 0,
            .extraStride = 0,
            .reserved_16 = 0,
            .semanticHash = 0,
            .reserved_1c = 0};
    };
    add("POSITION", 0, 21);
    add("NORMAL", 0, 21);
    add("BINORMAL", 0, 21);
    add("TEXCOORD", 0, 20);
    if ((vertexFormat & 0x10U) != 0U)
        add("TEXCOORD", 1, 20);
    if ((vertexFormat & 0x20U) != 0U)
        add("COLOR", 0, 2);
    if ((vertexFormat & 0x40U) != 0U) {
        add("BONEWEIGHTS", 0, 22);
        add("BONEINDICES", 0, 7);
    }
    description.elementCount = count;
    for (std::uint32_t index = 0; index < count; ++index) {
        d3t::FVFElement &element = description.elements[index];
        element.semanticHash = d3t_HashData(element.semanticName, static_cast<std::uint32_t>(std::strlen(element.semanticName)));
    }
    return d3t::CreateFVF(&description);
}


void ReadModelAnimations(std::byte *&cursor, D3TModelAnimationSet &animations) {
    animations = {};
    std::memcpy(&animations.expressionCount, cursor, sizeof(std::uint32_t));
    cursor += sizeof(std::uint32_t);
    animations.expressionCapacity = animations.expressionCount;
    animations.expressions = cursor;
    cursor += static_cast<std::size_t>(animations.expressionCount) * 0xd4;
    std::memcpy(&animations.animationCount, cursor, sizeof(std::uint32_t));
    cursor += sizeof(std::uint32_t);
    animations.animationCapacity = animations.animationCount;
    animations.animations = reinterpret_cast<D3TModelAnimation *>(cursor);
    cursor += static_cast<std::size_t>(animations.animationCount) * sizeof(D3TModelAnimation);
    for (std::uint32_t index = 0; index < animations.animationCount; ++index) {
        D3TModelAnimation &animation = animations.animations[index];
        animation.channels = reinterpret_cast<std::uint32_t *>(cursor);
        cursor += static_cast<std::size_t>(animation.channelCount) * sizeof(std::uint32_t);
        animation.keys = cursor;
        cursor += animation.keyBytes;
    }
    std::memcpy(&animations.boneMapCount, cursor, sizeof(std::uint32_t));
    cursor += sizeof(std::uint32_t);
    animations.boneMapCapacity = animations.boneMapCount;
    animations.boneMaps = reinterpret_cast<D3TModelBoneMap *>(cursor);
    cursor += static_cast<std::size_t>(animations.boneMapCount) * sizeof(D3TModelBoneMap);
    for (std::uint8_t index = 0; index < animations.boneMapCount; ++index) {
        D3TModelBoneMap &boneMap = animations.boneMaps[index];
        boneMap.entries = cursor;
        cursor += static_cast<std::size_t>(boneMap.entryCount) * 0x40;
    }
}

std::uint64_t ModelTextureBytes(const D3TModelTexture &texture, std::int32_t mipMode) {
    std::uint32_t mipCount = mipMode == 0 ? 9U : mipMode == 1 ? 11U : 16U;
    if (mipCount < 10U)
        mipCount = 10U;
    if (mipCount > texture.mipCount)
        mipCount = texture.mipCount;
    std::uint32_t width = texture.width, height = texture.height;
    for (std::uint32_t mip = mipCount; mip < texture.mipCount; ++mip) {
        width >>= 1;
        if (width <= 1U)
            width = 1;
        height >>= 1;
        if (height <= 1U)
            height = 1;
    }
    if (width == 0 || height == 0 || mipCount == 0)
        return 0;
    const std::uint32_t dimensionMips = std::bit_width(width > height ? width : height);
    if (mipCount > dimensionMips)
        mipCount = dimensionMips;
    std::uint64_t bytes = 0;
    for (std::uint32_t mip = 0; mip < mipCount; ++mip) {
        const std::uint32_t mipWidth = width >> mip, mipHeight = height >> mip;
        bytes += GetStride(mipWidth > 1U ? static_cast<std::int32_t>(mipWidth) : 1, mipHeight > 1U ? static_cast<std::int32_t>(mipHeight) : 1, texture.format);
    }
    return bytes;
}

// @verified - p
std::uint32_t AppendModelString(D3TModelRecord &model, const char *text) {
    const std::uint32_t length = static_cast<std::uint32_t>(std::strlen(text) + 1);
    for (std::uint32_t offset = 0; offset < model.stringsSize;) {
        const char *const current = model.strings + offset;
        const std::uint32_t currentLength = static_cast<std::uint32_t>(std::strlen(current) + 1);
        if (currentLength == length && !std::memcmp(current, text, length))
            return offset;
        offset += currentLength;
    }
    const std::uint32_t offset = model.stringsSize;
    if (offset + length > model.stringsCapacity)
        return 0xffffffffU;
    model.stringsSize = offset + length;
    std::memcpy(const_cast<char *>(model.strings) + offset, text, length);
    return offset;
}

} // namespace


void ReadModel(D3TModelRecord &model, void *image, const char *sourcePath, std::int32_t textureMipMode) {
    std::memset(&model, 0, sizeof(model));
    D3TModelFileHeader *const header = static_cast<D3TModelFileHeader *>(image);
    model.file = header;
    if (sourcePath)
        std::strncpy(model.name, sourcePath, sizeof(model.name) - 1);
    if (header->magic != kD3TModelMagic)
        return;

    model.nodeCount = header->nodeCount;
    model.spriteCount = header->spriteCount;
    model.probeCount = header->probeCount;
    model.materialCount = header->materialCount;
    const std::uint32_t materialParameterCount = header->materialParameterCount;
    model.textureCount = header->textureCount;
    model.drawGroupCount = header->drawGroupCount;
    model.submeshCount = header->submeshCount;
    const std::uint32_t geometryCount = header->geometryCount;
    model.skinCount = header->skinCount;
    const std::uint32_t boneCount = header->boneCount;
    model.referenceCount = header->referenceCount;
    model.partCount = header->partCount;

    const std::size_t runtimeSize = 4U * (model.referenceCount + 35U * model.materialCount + 3U * (model.partCount + 4U * geometryCount));
    model.runtimeStorage = static_cast<std::byte *>(std::malloc(runtimeSize));
    std::memset(model.runtimeStorage, 0, runtimeSize);
    model.flag0 = static_cast<std::uint8_t>(header->flags & 1U);
    model.flag1 = static_cast<std::uint8_t>((header->flags >> 1U) & 1U);
    model.stringsSize = header->stringsSize;
    model.stringsCapacity = header->stringsSize;
    std::byte *cursor = reinterpret_cast<std::byte *>(header + 1);
    model.strings = reinterpret_cast<const char *>(cursor);
    cursor = reinterpret_cast<std::byte *>((reinterpret_cast<std::uintptr_t>(cursor) + header->stringsSize + 3U) & ~std::uintptr_t{3});
    std::memcpy(&model.settingsOffset, cursor, sizeof(model.settingsOffset));
    cursor += sizeof(std::uint32_t);

    model.nodes = reinterpret_cast<D3TModelNode *>(cursor);
    cursor += static_cast<std::size_t>(model.nodeCount) * sizeof(D3TModelNode);
    model.nodeTransforms = reinterpret_cast<Matrix4x4 *>(cursor);
    cursor += static_cast<std::size_t>(model.nodeCount) * sizeof(Matrix4x4);
    model.sprites = reinterpret_cast<D3TModelSprite *>(cursor);
    cursor += static_cast<std::size_t>(model.spriteCount) * sizeof(D3TModelSprite);
    model.probes = reinterpret_cast<D3TModelProbe *>(cursor);
    cursor += static_cast<std::size_t>(model.probeCount) * sizeof(D3TModelProbe);
    d3t::ReadModelAnimations(cursor, model.animations);
    model.materials = reinterpret_cast<D3TModelMaterial *>(cursor);
    cursor += static_cast<std::size_t>(model.materialCount) * sizeof(D3TModelMaterial);
    model.materialParameters = reinterpret_cast<D3TModelMaterialParameter *>(cursor);
    cursor += static_cast<std::size_t>(materialParameterCount) * sizeof(D3TModelMaterialParameter);
    model.textures = reinterpret_cast<D3TModelTexture *>(cursor);
    cursor += static_cast<std::size_t>(model.textureCount) * sizeof(D3TModelTexture);
    model.skins = reinterpret_cast<D3TModelSkin *>(cursor);
    cursor += static_cast<std::size_t>(model.skinCount) * sizeof(D3TModelSkin);
    model.boneNodes = reinterpret_cast<std::uint16_t *>(cursor);
    cursor += static_cast<std::size_t>(boneCount) * sizeof(std::uint16_t);
    model.bindPoses = reinterpret_cast<Matrix4x4 *>(cursor);
    cursor += static_cast<std::size_t>(boneCount) * sizeof(Matrix4x4);

    std::byte *runtime = model.runtimeStorage;
    model.materialRuntime = reinterpret_cast<D3TModelMaterialRuntime *>(runtime);
    std::memset(runtime, 0, static_cast<std::size_t>(model.materialCount) * sizeof(D3TModelMaterialRuntime));
    runtime += static_cast<std::size_t>(model.materialCount) * sizeof(D3TModelMaterialRuntime);
    model.geometry = reinterpret_cast<D3TModelGeometryRuntime *>(runtime);
    model.drawGroups = reinterpret_cast<D3TModelDrawGroup *>(cursor);
    cursor += static_cast<std::size_t>(model.drawGroupCount) * sizeof(D3TModelDrawGroup);
    model.geometryCount = geometryCount;
    model.submeshes = reinterpret_cast<D3TModelSubmesh *>(cursor);
    runtime += static_cast<std::size_t>(geometryCount) * sizeof(D3TModelGeometryRuntime);
    cursor += static_cast<std::size_t>(model.submeshCount) * sizeof(D3TModelSubmesh);
    const D3TModelGeometryDescription *const descriptions = reinterpret_cast<const D3TModelGeometryDescription *>(cursor);
    cursor += static_cast<std::size_t>(geometryCount) * sizeof(D3TModelGeometryDescription);
    std::memcpy(&model.instanceCount, cursor, sizeof(model.instanceCount));
    cursor += sizeof(std::uint32_t);

    for (std::uint32_t index = 0; index < geometryCount; ++index) {
        const D3TModelGeometryDescription &description = descriptions[index];
        D3TModelGeometryRuntime &geometry = model.geometry[index];
        const std::byte *const vertices = cursor;
        cursor += description.vertexBytes;
        const std::byte *const indices = cursor;
        cursor += description.indexBytes;
        geometry.vertexBufferCount = 1;
        const BufferDescription vertexBuffer{
            .flags = 1, .size = static_cast<std::int32_t>(description.vertexBytes), .count = 0, .flags3 = 0, .buffer = vertices, .name = nullptr};
        geometry.vertexBuffers[0] = static_cast<std::uint32_t>(CreateBuffer(&vertexBuffer));
        const BufferDescription indexBuffer{
            .flags = 2, .size = static_cast<std::int32_t>(description.indexBytes), .count = 0, .flags3 = 0, .buffer = indices, .name = nullptr};
        geometry.indexBuffer = static_cast<std::uint32_t>(CreateBuffer(&indexBuffer));
        geometry.indexFormat = description.indexFormat ? 11U : 16U;
        geometry.inputLayout = d3t::CreateModelInputLayout(description.vertexFormat);
        model.geometryBytes += description.vertexBytes;
        model.geometryBytes += description.indexBytes;
    }

    std::byte *handles = runtime;
    if (model.instanceCount) {
        const std::byte *const instances = cursor;
        const std::size_t instanceBytes = static_cast<std::size_t>(model.instanceCount) * 52U;
        cursor += instanceBytes;
        model.instanceBounds = cursor;
        cursor += static_cast<std::size_t>(model.instanceCount) * 16U;
        model.references = reinterpret_cast<D3TModelReference *>(cursor);
        cursor += static_cast<std::size_t>(model.referenceCount) * sizeof(D3TModelReference);
        const BufferDescription instanceBuffer{
            .flags = 3, .size = static_cast<std::int32_t>(instanceBytes), .count = 52, .flags3 = 0, .buffer = instances, .name = nullptr};
        model.instanceBuffer = static_cast<std::uint32_t>(CreateBuffer(&instanceBuffer));
        model.referenceHandles = reinterpret_cast<std::uint32_t *>(runtime);
        handles = runtime + static_cast<std::size_t>(model.referenceCount) * sizeof(std::uint32_t);
        model.instanceMirror = std::malloc(instanceBytes);
        std::memcpy(model.instanceMirror, instances, instanceBytes);
    }
    if (model.partCount) {
        model.parts = reinterpret_cast<D3TModelPart *>(cursor);
        cursor += static_cast<std::size_t>(model.partCount) * sizeof(D3TModelPart);
        model.partHandles = reinterpret_cast<std::uint32_t *>(handles);
    }
    std::memcpy(&model.bounds, cursor, sizeof(model.bounds));
    for (std::uint32_t index = 0; index < model.textureCount; ++index)
        model.textureBytes += ModelTextureBytes(model.textures[index], textureMipMode);
}


std::uint32_t AllocateModelHandle() {
    const std::uint32_t first = g_d3tModels.cursor;
    std::uint32_t index = first;
    std::uint32_t counter = 0;
    while (true) {
        counter = g_d3tModels.generations[index] >> 8U;
        if (counter < 0x800000U)
            break;
        index = (index + 1U) & 0xffU;
        if (index == first)
            return 0;
    }
    const std::uint32_t next = counter + 1U;
    const std::uint32_t generation = (((next & 0xff800000U) != 0x800000U ? next : 1U) | 0xff800000U) << 8U;
    g_d3tModels.generations[index] = generation;
    g_d3tModels.cursor = (index + 1U) & 0xffU;
    return generation | index;
}

D3TModelRecord *ResolveModel(std::uint32_t handle) {
    const std::uint32_t index = handle & 0xffU;
    return g_d3tModels.generations[index] == (handle & 0xffffff00U) ? &g_d3tModels.records[index] : nullptr;
}


void ReleaseModel(D3TModelRecord &model) {
    for (std::uint32_t index = 0; index < model.geometryCount; ++index) {
        D3TModelGeometryRuntime &geometry = model.geometry[index];
        for (std::uint32_t buffer = 0; buffer < geometry.vertexBufferCount; ++buffer)
            ValidateAndCommitBuffer(static_cast<std::int32_t>(geometry.vertexBuffers[buffer]));
        ValidateAndCommitBuffer(static_cast<std::int32_t>(geometry.indexBuffer));
    }
    constexpr std::uint32_t kShaderReleaseOrder[18] = {0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 4, 14, 11, 12, 13, 15, 16, 17};
    for (std::uint32_t index = 0; index < model.materialCount; ++index) {
        D3TModelMaterialRuntime &runtime = model.materialRuntime[index];
        for (const std::uint32_t texture : runtime.textures)
            InitTexture2DResource(texture);
        for (const std::uint32_t shader : kShaderReleaseOrder)
            d3t::DestroyShader(runtime.shaders[shader]);
    }
    for (std::uint32_t index = 0; index < model.referenceCount; ++index)
        d3t::ReleaseModelHandle(model.referenceHandles[index]);
    for (std::uint32_t index = 0; index < model.partCount; ++index) {
        for (std::uint32_t reference = 0; reference < 3; ++reference)
            d3t::ReleaseModelHandle(model.partHandles[index * 3 + reference]);
    }
    if (model.animations.ownsStorage) {
        std::free(model.animations.expressions);
        model.animations.expressions = nullptr;
        model.animations.expressionCount = 0;
        model.animations.expressionCapacity = 0;
        std::free(model.animations.boneMaps);
        model.animations.boneMaps = nullptr;
        model.animations.boneMapCount = 0;
        model.animations.boneMapCapacity = 0;
        model.animations.ownsStorage = 0;
    }
    if (model.ownsStrings)
        std::free(const_cast<char *>(model.strings));
    std::free(model.runtimeMaterialParameters.data);
    model.runtimeMaterialParameters = {};
    std::free(model.runtimeTextures.data);
    model.runtimeTextures = {};
    std::free(model.runtimeAllocation180);
    model.runtimeAllocation180 = nullptr;
    model.runtimeAllocation180Size = 0;
    std::free(model.runtimeAllocation190);
    model.runtimeAllocation190 = nullptr;
    model.runtimeAllocation190Size = 0;
    std::free(model.file);
    std::free(model.runtimeStorage);
    std::free(model.instanceMirror);
    std::memset(&model, 0, sizeof(model));
}


void ReleaseModelHandle(std::uint32_t handle) {
    const std::uint32_t index = handle & 0xffU;
    const std::uint32_t generation = handle & 0xffffff00U;
    if (g_d3tModels.generations[index] != generation)
        return;
    d3t::ReleaseModel(g_d3tModels.records[index]);
    if (g_d3tModels.generations[index] == generation)
        g_d3tModels.generations[index] &= 0x7fffffffU;
}


const char *GetModelSettings(std::uint32_t handle) {
    const D3TModelRecord *const model = d3t::ResolveModel(handle);
    return model && model->settingsOffset < model->stringsSize ? model->strings + model->settingsOffset : nullptr;
}

// @verified - p
void CopyModelMaterials(std::uint32_t destinationHandle, std::uint32_t sourceHandle) {
    const D3TModelRecord *const source = ResolveModel(sourceHandle);
    if (!source)
        return;
    D3TModelRecord *const destination = ResolveModel(destinationHandle);
    if (!destination)
        return;
    std::memcpy(destination->materialRuntime, source->materialRuntime, sizeof(D3TModelMaterialRuntime) * destination->materialCount);
    *destination->materials = *source->materials;
    const std::uint32_t nameOffset = source->materials->nameOffset;
    destination->materials->nameOffset = AppendModelString(*destination, nameOffset < source->stringsSize ? source->strings + nameOffset : nullptr);
    std::uint32_t parameterCount = 0;
    for (std::uint32_t index = 0; index < source->materialCount; ++index)
        parameterCount += source->materials[index].parameterCount;
    D3TRawArray<D3TModelTexture> &textures = destination->runtimeTextures;
    if (source->textureCount > textures.capacity) {
        void *const allocation = std::realloc(textures.data, sizeof(D3TModelTexture) * source->textureCount);
        if (allocation) {
            textures.data = static_cast<D3TModelTexture *>(allocation);
            textures.capacity = source->textureCount;
        }
    }
    std::memcpy(textures.data, source->textures, sizeof(D3TModelTexture) * source->textureCount);
    destination->textures = textures.data;
    D3TRawArray<D3TModelMaterialParameter> &parameters = destination->runtimeMaterialParameters;
    if (parameterCount > parameters.capacity) {
        void *const allocation = std::realloc(parameters.data, sizeof(D3TModelMaterialParameter) * parameterCount);
        if (allocation) {
            parameters.data = static_cast<D3TModelMaterialParameter *>(allocation);
            parameters.capacity = parameterCount;
        }
    }
    std::memcpy(parameters.data, source->materialParameters, sizeof(D3TModelMaterialParameter) * parameterCount);
    for (std::uint32_t index = 0; index < destination->materialCount; ++index)
        destination->materialRuntime[index].flags = 2;
    destination->materialParameters = parameters.data;
}


bool SetupShaderReferences(std::uint32_t handle) {
    const D3TModelRecord *const model = d3t::ResolveModel(handle);
    if (!model)
        return true;
    constexpr std::uint32_t kShaderCheckOrder[18] = {11, 12, 13, 15, 16, 17, 0, 1, 2, 3, 4, 14, 5, 6, 7, 8, 9, 10};
    constexpr d3t::ShaderType kVertex = d3t::ShaderType::Vertex;
    constexpr d3t::ShaderType kPixel = d3t::ShaderType::Pixel;
    constexpr d3t::ShaderType kShaderTypes[18] = {
        kVertex, kPixel, kVertex, kPixel, kPixel, kVertex, kPixel, kPixel, kVertex, kPixel, kPixel, kVertex, kPixel, kPixel, kPixel, kVertex, kPixel, kPixel};
    for (std::uint32_t index = 0; index < model->materialCount; ++index) {
        const D3TModelMaterialRuntime &runtime = model->materialRuntime[index];
        for (const std::uint32_t shader : kShaderCheckOrder) {
            if (!d3t::GetNativeShader(runtime.shaders[shader], kShaderTypes[shader]))
                return false;
        }
        for (const std::uint32_t texture : runtime.textures) {
            D3TTextureBackendView view{};
            if (GetTexture2DBackendView(texture, &view) && !view.texture)
                return false;
        }
    }
    return true;
}
} // namespace shenmue::d3t
