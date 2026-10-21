#include "s2/model_draw.hpp"
#include "s2/renderer_scratch.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#if defined(_WIN32)
#include <d3d11.h>
#endif

namespace shenmue::s2 {
ModelGeometrySamplerSettings g_modelGeometrySamplerSettings{.magnification = 1, .minification = 1, .mip = 1, .anisotropy = 0};
bool g_modelAlternateShading{};
bool g_sortModelGeometry = true;
float g_modelLodScale = 1.0f;
float g_modelLodBias{};
namespace {
void BindModelConstantBuffer(d3t::D3TDeviceContext *context, std::uint32_t stages, std::uint32_t slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    for (std::uint32_t stage = 0; stage < 3; ++stage)
        if ((stages & (1U << stage)) && state.bindings.constantBufferHandles[stage][slot] != handle) {
            state.constantBufferDirtyMasks[stage] |= static_cast<std::uint16_t>(1U << slot);
            state.bindings.constantBufferHandles[stage][slot] = handle;
        }
}
void BindModelBuffer(d3t::D3TDeviceContext *context, std::uint32_t slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.bindings.bufferHandles[0][slot] != handle) {
        state.shaderResourceDirtyMasks[0] |= 1U << slot;
        state.bindings.bufferHandles[0][slot] = handle;
    }
}
void BindModelTexture(d3t::D3TDeviceContext *context, std::uint32_t slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.bindings.textureHandles[1][slot] != handle || state.bindings.textureFlags[1][slot] != 0) {
        state.shaderResourceDirtyMasks[1] |= 1U << slot;
        state.bindings.textureHandles[1][slot] = handle;
        state.bindings.textureFlags[1][slot] = 0;
    }
}
void BindModelSampler(d3t::D3TDeviceContext *context, std::uint32_t slot, std::uint64_t flags) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    d3t::D3TSamplerStateKey &sampler = state.bindings.samplers[1][slot];
    const std::uint64_t current = sampler.flags[0] | (static_cast<std::uint64_t>(sampler.flags[1]) << 32U);
    if (current != flags) {
        state.samplerDirtyMasks[1] |= static_cast<std::uint16_t>(1U << slot);
        sampler = {.flags = {static_cast<std::uint32_t>(flags), static_cast<std::uint32_t>(flags >> 32U)}, .reserved = {}};
    }
}
void SetModelSampleMask(d3t::D3TDeviceContext *context, std::uint32_t mask) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.blendSampleMask != mask) {
        state.pipelineStateFlags |= d3t::PipelineBlend;
        state.blendSampleMask = mask;
    }
}
} // namespace

void CommitModelGeometryDraw(d3t::D3TDeviceContext *context, const ModelGeometryDraw &draw, const ModelScenePass &pass) {
    const D3TModelRecord &model = *draw.model;
    const D3TModelDrawGroup &group = model.drawGroups[draw.drawGroup];
    const D3TModelGeometryRuntime &geometry = model.geometry[group.geometryIndex];
    const D3TModelSubmesh &mesh = model.submeshes[group.firstSubmesh + draw.submesh];
    const D3TModelMaterial &material = model.materials[mesh.material];
    const D3TModelMaterialRuntime &runtime = model.materialRuntime[mesh.material];
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::uint32_t vertexShader = state.vertexShader;
    std::uint32_t pixelShader = state.pixelShader;
    bool noPixelShader = false;
    switch (pass.type) {
    case 0:
    case 6:
        if (pass.deferred && (material.blendType & ~5U) == 0) {
            if (g_modelAlternateShading) {
                vertexShader = runtime.shaders[15];
                pixelShader = runtime.shaders[pass.alternateShader ? 17 : 16];
                noPixelShader = runtime.shaders[16] == 0;
            } else {
                vertexShader = runtime.shaders[11];
                pixelShader = runtime.shaders[pass.alternateShader ? (pass.type == 6 ? 14 : 13) : 12];
                noPixelShader = runtime.shaders[pass.alternateShader ? 16 : 12] == 0;
            }
            break;
        }
        [[fallthrough]];
    case 5:
    case 7:
        if (g_modelAlternateShading) {
            vertexShader = runtime.shaders[8];
            pixelShader = runtime.shaders[pass.alternateShader ? 10 : 9];
            noPixelShader = runtime.shaders[9] == 0;
        } else {
            vertexShader = runtime.shaders[5];
            pixelShader = runtime.shaders[pass.alternateShader ? 7 : 6];
            noPixelShader = runtime.shaders[pass.alternateShader ? 9 : 6] == 0;
        }
        break;
    case 1:
        vertexShader = runtime.shaders[0];
        pixelShader = (pass.materialMask & 6U) ? runtime.shaders[1] : 0;
        noPixelShader = (pass.materialMask & 6U) == 0;
        break;
    case 2:
    case 3:
        vertexShader = runtime.shaders[2];
        pixelShader = (pass.materialMask & 6U) ? runtime.shaders[pass.type == 2 ? 3 : 4] : 0;
        noPixelShader = (pass.materialMask & 6U) == 0;
        break;
    case 4:
        vertexShader = pass.vertexShader;
        pixelShader = pass.pixelShader;
        noPixelShader = pixelShader == 0;
        break;
    }
    if (state.vertexShader != vertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = UINT32_MAX;
    }
    state.vertexShader = vertexShader;
    if (state.pixelShader != pixelShader)
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    state.pixelShader = pixelShader;
    if (draw.debugOverride) {
#if defined(_WIN32)
        ModelDebugConstants *const debug = static_cast<ModelDebugConstants *>(d3t::MapTempConstantBufferForWriteDiscard(context, 3, 10, 40));
        if (!debug)
            return;
        *debug = {.values = draw.debugValues, .enabled = draw.debugOverride, .index = draw.debugIndex};
#endif
    } else {
        BindModelConstantBuffer(context, 3, 10, g_dedicatedShadowBuffer);
    }
    BindModelConstantBuffer(context, 3, 1, draw.objectBuffer);
    const std::uint32_t savedRasterizer = state.rasterizerState;
    std::uint32_t rasterizer = material.doubleSided ? savedRasterizer & ~0x18U : savedRasterizer;
    const std::int32_t signedBias = TruncateToInt32(material.depthBias);
    const std::uint32_t bias = signedBias < 0 ? (0U - static_cast<std::uint32_t>(signedBias)) | 0x80U : static_cast<std::uint32_t>(signedBias);
    rasterizer ^= (rasterizer ^ (bias * 32U)) & 0x1fe0U;
    if (state.rasterizerState != rasterizer) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = rasterizer;
    }
    if (draw.opacity < 1.0f && pass.type != 1) {
        const float mask = draw.opacity * 255.0f;
        const std::int64_t value = mask >= -9223372036854775808.0f && mask < 9223372036854775808.0f ? static_cast<std::int64_t>(mask) : INT64_MIN;
        SetModelSampleMask(context, static_cast<std::uint32_t>(value));
    }
    for (std::uint32_t vertex = 0; vertex < geometry.vertexBufferCount; ++vertex)
        if (state.vertexBufferHandles[vertex] != geometry.vertexBuffers[vertex]) {
            state.vertexBufferHandles[vertex] = geometry.vertexBuffers[vertex];
            state.vertexBufferDirtyMask |= 1U << vertex;
        }
    if (!noPixelShader) {
        const std::byte alphaToCoverage =
            static_cast<std::byte>(material.blendType == 4 && !pass.deferred && (pass.type == 0 || pass.type == 5 || pass.type == 6));
        if (state.blendState[65] != alphaToCoverage) {
            state.pipelineStateFlags |= d3t::PipelineBlend;
            state.blendState[65] = alphaToCoverage;
        }
        BindModelConstantBuffer(context, 3, 5, runtime.constantBuffer);
        if (runtime.textures[0]) {
            for (std::uint32_t texture = 0; texture < 14; ++texture)
                BindModelTexture(context, 8U + texture, runtime.textures[texture]);
        } else {
            BindModelTexture(context, 8, d3t::g_defaultQuarterTexture);
            BindModelTexture(context, 9, d3t::g_defaultNormalTexture);
            BindModelTexture(context, 10, d3t::g_defaultThreeQuarterTexture);
            BindModelTexture(context, 11, d3t::g_defaultBlackTexture);
        }
        const ModelGeometrySamplerSettings &settings = g_modelGeometrySamplerSettings;
        std::uint64_t sampler = (settings.mip & 3U) * 16U | (settings.magnification & 3U) * 4U | (settings.minification & 3U);
        if (settings.magnification == 2) {
            const std::uint32_t anisotropy = settings.anisotropy >= 1 ? static_cast<std::uint32_t>(settings.anisotropy) : 1U;
            sampler ^= (sampler ^ ((anisotropy - 1U) << 14U)) & 0x3c000U;
        }
        if (material.anisotropyLevel > 0) {
            const std::uint32_t requestedBits = 1U << ((static_cast<std::uint32_t>(material.anisotropyLevel) - 1U) & 31U);
            const std::int32_t requested = static_cast<std::int32_t>(requestedBits);
            const std::uint32_t anisotropy = requested <= 1 ? 1U : requested > 16 ? 16U : static_cast<std::uint32_t>(requested);
            sampler = (sampler & ~0x3fULL) | 42U;
            sampler ^= (sampler ^ ((anisotropy - 1U) << 14U)) & 0x3c000U;
        }
        sampler |= static_cast<std::uint64_t>(material.addressU & 3U) << 6U;
        sampler |= static_cast<std::uint64_t>(material.addressV & 3U) << 8U;
        BindModelSampler(context, 0, sampler | (static_cast<std::uint64_t>(std::bit_cast<std::uint32_t>(material.minimumLod)) << 18U));
        const float clampedLod = material.maximumLod <= material.minimumLod ? material.maximumLod : material.minimumLod;
        BindModelSampler(context, 1, sampler | (static_cast<std::uint64_t>(std::bit_cast<std::uint32_t>(clampedLod)) << 18U));
        BindModelSampler(context, 2, 21);
        BindModelSampler(context, 3, 21);
        BindModelSampler(context, 10, 640);
        BindModelSampler(context, 11, 661);
    }
    if (pass.drawCallback) {
        ModelDrawCallbackContext callback{.context = context, .transform = draw.transform};
        pass.drawCallback(&callback);
    }
    if (material.materialType == 4 || material.materialType == 5) {
        const std::uint32_t firstBillboard = group.firstBillboard + mesh.firstIndex / 6U;
        Vector4 norms;
        Matrix4x4::ComputeColumnNorms(&draw.transform, &norms);
#if defined(_WIN32)
        void *const billboard = d3t::MapTempConstantBufferForWriteDiscard(context, 1, 2, 8);
        if (billboard) {
            std::memcpy(billboard, &firstBillboard, 4);
            std::memcpy(static_cast<std::byte *>(billboard) + 4, &norms.x, 4);
        }
#endif
        if (state.inputLayout) {
            state.inputLayout = 0;
            state.pipelineStateFlags |= d3t::PipelineInputLayout;
        }
    } else {
        if (state.indexBufferHandle != geometry.indexBuffer)
            state.pipelineStateFlags |= d3t::PipelineIndexBuffer;
        state.indexBufferHandle = geometry.indexBuffer;
        state.indexFormat = geometry.indexFormat;
    }
#if defined(_WIN32)
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    ID3D11DeviceContext *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (material.materialType == 5)
        native->DrawIndexed(draw.instanceCount, draw.firstInstance, 0);
    else if (material.materialType == 4)
        native->Draw(6U * (mesh.primitiveCount >> 1U), 0);
    else if (draw.instanceCount)
        native->DrawIndexedInstanced(3U * mesh.primitiveCount, draw.instanceCount, mesh.firstIndex + group.firstIndex, group.baseVertex, draw.firstInstance);
    else
        native->DrawIndexed(3U * mesh.primitiveCount, mesh.firstIndex + group.firstIndex, group.baseVertex);
#endif
    if (draw.opacity < 1.0f)
        SetModelSampleMask(context, 0xff);
    if (material.doubleSided && state.rasterizerState != savedRasterizer) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = savedRasterizer;
    }
}

void SubmitModelGeometry(ModelGeometryVisibility &visibility, d3t::D3TDeviceContext *context, const ModelScenePass &pass) {
    ModelGeometryContext &geometry = *visibility.geometry;
    d3t::ModelCacheSlot *const cache = geometry.cache;
    if (!cache) {
        geometry.submittedDraws = 0;
        geometry.culledDraws = 0;
        return;
    }
#if defined(_WIN32)
    ModelPassConstants *const constants = static_cast<ModelPassConstants *>(d3t::MapTempConstantBufferForWriteDiscard(context, 3, 7, 16));
    if (constants)
        *constants = {.writeVelocity = pass.type != 6 && pass.disableVelocity != 1,
            .fogDisabled = pass.fogDisabled,
            .clusterNear = pass.clusterNear,
            .clusterFar = pass.clusterFar};
#endif
    const bool sort = (pass.materialMask & 8U) != 0 || (g_sortModelGeometry && (pass.materialMask & 7U) != 0);
    std::uint32_t *const order = static_cast<std::uint32_t *>(GetRendererScratchBlock(static_cast<std::size_t>(geometry.drawCapacity) * 4U));
    ModelGeometryDraw *const draws =
        static_cast<ModelGeometryDraw *>(GetRendererScratchBlock(static_cast<std::size_t>(geometry.drawCapacity) * sizeof(ModelGeometryDraw)));
    float *const distances = static_cast<float *>(GetRendererScratchBlock(static_cast<std::size_t>(geometry.drawCapacity) * 4U));
    std::uint32_t count = 0, culled = 0, billboardIndexCount = 0;
    for (std::uint32_t node = 0; node < cache->nodeCount; ++node) {
        const std::uint16_t flags = geometry.nodeFlags[node];
        const D3TModelNode &entry = cache->nodes[node];
        if (((flags & 2U) && pass.excludeMoving) || ((flags & 0x80U) && !pass.alternateNodes) || ((flags & 0x40U) && pass.alternateNodes) ||
            ((flags & 8U) && (pass.type & ~3U) == 0 && pass.type != 1) || (flags & 1U) || !entry.drawGroup)
            continue;
        D3TModelRecord *model = d3t::ResolveModel(cache->modelHandles[node]);
        if (!model)
            continue;
        std::uint16_t groupIndex = entry.drawGroup - 1U;
        const D3TModelDrawGroup *group = model->drawGroups + groupIndex;
        if (group->part) {
            const D3TModelPart &part = model->parts[group->part - 1U];
            const std::uint32_t *const handles = model->partHandles + 3U * (group->part - 1U);
            if (part.pathCount) {
                const Matrix4x4 &transform = geometry.transforms[node];
                const float x = transform.rows[0].w - pass.cameraPosition.x;
                const float y = transform.rows[1].w - pass.cameraPosition.y;
                const float z = transform.rows[2].w - pass.cameraPosition.z;
                const float distance = (std::sqrt((y * y + x * x) + z * z) * g_modelLodScale + g_modelLodBias) + pass.lodBias;
                for (std::uint32_t index = 0; index < part.pathCount; ++index) {
                    if (part.distances[index] >= distance)
                        break;
                    D3TModelRecord *const replacement = d3t::ResolveModel(handles[index]);
                    if (replacement) {
                        model = replacement;
                        group = model->drawGroups;
                        groupIndex = 0;
                    }
                }
            }
        }
        for (std::uint32_t meshIndex = 0; meshIndex < group->submeshCount; ++meshIndex) {
            const D3TModelSubmesh &mesh = model->submeshes[group->firstSubmesh + meshIndex];
            const D3TModelMaterial &material = model->materials[mesh.material];
            if (pass.type == 1 && (!material.castsShadows || material.unlit))
                continue;
            if (pass.type == 6 && (material.materialType == 7 || material.materialType == 8 || (flags & 0x100U)))
                continue;
            if (material.materialType == 10 ? (pass.type & ~3U) == 0 && pass.type != 1 : pass.type == 7)
                continue;
            bool included = false;
            switch (material.blendType) {
            case 0:
                included = (pass.materialMask & 1U) != 0;
                break;
            case 1:
            case 5:
                included = (pass.materialMask & 2U) != 0;
                break;
            case 2:
            case 3:
                included = (pass.materialMask & 8U) != 0;
                break;
            case 4:
                included = (pass.materialMask & 4U) != 0;
                break;
            }
            ModelGeometryDraw *draw = nullptr;
            if (included) {
                if (visibility.nodeMask && !(visibility.nodeMask[node / 32U] & (1U << (node % 32U))) && !(flags & 2U) && !group->skin) {
                    ++culled;
                    continue;
                }
                order[count] = count;
                draw = draws + count;
                std::memset(draw, 0, sizeof(*draw));
                draw->transform = geometry.transforms[node];
                draw->objectBuffer = cache->constantBuffers[node];
                draw->model = model;
                draw->drawGroup = groupIndex;
                draw->submesh = static_cast<std::uint16_t>(meshIndex);
                draw->nodeIndex = node;
                draw->opacity = cache->alpha[node];
                if (geometry.debugIndices[node]) {
                    draw->debugOverride = 1;
                    std::memcpy(draw->debugValues.data(), geometry.debugData + static_cast<std::size_t>(geometry.debugIndices[node]) * 32U, 32);
                    draw->debugIndex = geometry.debugIndices[node];
                }
                if (sort) {
                    Vector3 minimum = mesh.boundsMinimum;
                    Vector3 maximum = mesh.boundsMaximum;
                    TransformBounds(minimum, maximum, draw->transform);
                    distances[count] = BoundsDistanceSquared(minimum, maximum, pass.cameraPosition);
                }
                ++count;
            }
            if (material.materialType == 5) {
                const std::uint32_t billboardCount = mesh.primitiveCount >> 1U;
                const std::uint32_t firstBillboard = group->firstBillboard + mesh.firstIndex / 6U;
                ModelNearBillboard *const nearBillboards =
                    static_cast<ModelNearBillboard *>(GetRendererScratchBlock(static_cast<std::size_t>(billboardCount) * 40U));
                std::uint32_t *const indices = static_cast<std::uint32_t *>(GetRendererScratchBlock(static_cast<std::size_t>(billboardCount) * 4U));
                std::uint32_t indexCount = 0;
                for (std::uint32_t reference = 0; reference < model->referenceCount; ++reference) {
                    D3TModelRecord *const referenced = d3t::ResolveModel(model->referenceHandles[reference]);
                    if (!referenced)
                        continue;
                    const D3TModelReference &definition = model->references[reference];
                    std::uint32_t nearCount = 0;
                    for (std::uint32_t billboard = firstBillboard; billboard < firstBillboard + billboardCount; ++billboard) {
                        const ModelBillboardInstance &instance = *reinterpret_cast<const ModelBillboardInstance *>(
                            static_cast<const std::byte *>(model->instanceMirror) + static_cast<std::size_t>(billboard) * 52U);
                        const ModelBillboardBounds &bounds =
                            *reinterpret_cast<const ModelBillboardBounds *>(model->instanceBounds + static_cast<std::size_t>(billboard) * 16U);
                        if (bounds.reference != reference)
                            continue;
                        const Vector4 local{instance.position.x, instance.position.y, instance.position.z, 0};
                        Vector4 world;
                        Vector4::ProjectPoint(geometry.transforms + node, &world, &local);
                        const float scale =
                            (instance.halfHeight + instance.halfHeight) / (referenced->submeshes[0].boundsMaximum.y - referenced->submeshes[0].boundsMinimum.y);
                        const float x = world.x - pass.cameraPosition.x;
                        const float y = world.y - pass.cameraPosition.y;
                        const float z = world.z - pass.cameraPosition.z;
                        const float distance = std::sqrt((y * y + x * x) + z * z) / scale;
                        bool farVisible = !(definition.distance > distance);
                        if (!farVisible) {
                            float fade = 0.0f;
                            if (definition.fadeRange > 0.0f) {
                                const float value = (distance - (definition.distance - definition.fadeRange)) / definition.fadeRange;
                                const float limited = 1.0f < value ? 1.0f : value;
                                fade = 0.0f > limited ? 0.0f : limited;
                            }
                            if (!(definition.fadeRange > 0.0f) || fade < 1.0f)
                                nearBillboards[nearCount++] = {.position = {instance.position.x, instance.position.y, instance.position.z, 1},
                                    .bounds = bounds.position,
                                    .scale = scale,
                                    .opacity = 1.0f - fade,
                                    .parameter24 = instance.parameter2c};
                            farVisible = fade > 0.0f;
                        }
                        if (farVisible)
                            for (std::uint32_t vertex = 0; vertex < 6; ++vertex)
                                indices[indexCount++] = 6U * billboard + vertex;
                    }
                }
                if (draw) {
                    if (indexCount) {
                        void *const mapped = d3t::GetWriteMappedBufferPointer(context, g_billboardBuffer, 1, 24U * firstBillboard);
                        if (mapped)
                            std::memcpy(mapped, indices, 4U * indexCount);
                        d3t::DX11_CopySceneNodeToGPU(context, g_billboardBuffer);
                    }
                    draw->firstInstance = static_cast<std::uint16_t>(billboardIndexCount);
                    billboardIndexCount += indexCount;
                    draw->instanceCount = static_cast<std::uint16_t>(indexCount);
                }
                if (indices)
                    g_rendererScratch.current = reinterpret_cast<std::byte *>(indices);
                if (nearBillboards)
                    g_rendererScratch.current = reinterpret_cast<std::byte *>(nearBillboards);
            }
        }
    }
    if (sort) {
        if (pass.materialMask & 8U)
            std::sort(order, order + count, [distances](std::uint32_t left, std::uint32_t right) { return distances[left] > distances[right]; });
        else
            std::sort(order, order + count, [distances](std::uint32_t left, std::uint32_t right) { return distances[left] < distances[right]; });
    }
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    for (std::uint32_t index = 0; index < count; ++index) {
        const ModelGeometryDraw &draw = draws[order[index]];
        const D3TModelDrawGroup &group = draw.model->drawGroups[draw.drawGroup];
        const D3TModelSubmesh &mesh = draw.model->submeshes[group.firstSubmesh + draw.submesh];
        const std::uint32_t layout = draw.model->geometry[group.geometryIndex].inputLayout;
        if (state.inputLayout != layout) {
            state.inputLayout = layout;
            state.pipelineStateFlags |= d3t::PipelineInputLayout;
        }
        BindModelBuffer(context, 1, cache->skinningBuffer);
        BindModelBuffer(context, 2, draw.model->instanceBuffer);
        BindModelBuffer(context, 3, g_instanceBuffer);
        if (draw.model->materials[mesh.material].materialType == 5) {
            if (state.indexBufferHandle != g_billboardBuffer)
                state.pipelineStateFlags |= d3t::PipelineIndexBuffer;
            state.indexBufferHandle = g_billboardBuffer;
            state.indexFormat = 16;
        }
        CommitModelGeometryDraw(context, draw, pass);
        BindModelBuffer(context, 1, 0);
        BindModelBuffer(context, 2, 0);
        BindModelBuffer(context, 3, 0);
    }
    if (distances)
        g_rendererScratch.current = reinterpret_cast<std::byte *>(distances);
    if (draws)
        g_rendererScratch.current = reinterpret_cast<std::byte *>(draws);
    if (order)
        g_rendererScratch.current = reinterpret_cast<std::byte *>(order);
    geometry.submittedDraws = count;
    geometry.culledDraws = culled;
}

} // namespace shenmue::s2
