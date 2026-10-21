#include "s2/world_render.hpp"
#if defined(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS) && SHENMUE_ENHANCEMENT_S2_MAP_SEAMS
#include "s2/enhancements/map_seams.hpp"
#endif
#include "s2/renderer_initialization.hpp"
#include "s2/surface_wave.hpp"

#include "d3t/buffer.hpp"
#include "d3t/archive_handles.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model.hpp"
#include "d3t/model_cache.hpp"
#include "s2/author.hpp"
#include "s2/archive_data.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/flag.hpp"
#include "s2/date_time.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/map_info.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/ui.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_pose.hpp"
#include "s2/motion.hpp"
#include "s2/mt7.hpp"
#include "s2/motion_work.hpp"
#include "s2/hand.hpp"
#include "s2/darts.hpp"
#include "s2/math.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/pause.hpp"
#include "s2/pak.hpp"
#include "s2/search.hpp"
#include "s2/weather.hpp"

#include <cstddef>
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <span>
#include <utility>
#include <vector>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
std::uint32_t g_airClearColour{};
std::int32_t g_previousSkyWeatherMode{}, g_currentSkyWeatherMode{};
std::int32_t g_airDrawDomeEnabled = 1;
std::int32_t g_airDrawCloudsEnabled = 1;
std::int32_t g_airDrawPlanetsEnabled = 1;
Vector3 g_scriptedModelScale{1.0f, 1.0f, 1.0f};
std::int32_t g_modelDeformationSuppressed{};

// @verified - p
void InitModelVector() {
    g_modelDeformationSuppressed = 0;
}
Matrix4x4 g_scriptedIncomingTransform{};
Vector3 g_shadowRange{};
std::uint32_t g_characterShadowDisabled{};
alignas(32) std::array<std::byte, characterShadowStorageSize> g_characterShadowStorage{};
void *g_characterShadowRuntime{};
namespace {

std::int32_t globalShadowLightingEnabled = 1;
std::int32_t globalShadowFadeRequested = 0;
float globalShadowFade = 1.0f;
float globalShadowWeatherAlpha = 0.4f;
float globalShadowRyoAlpha = 1.0f;
float globalShadowOpacity = 0.4f;
Character *scriptedModelEntity{};
std::int32_t characterUngroundedShadowMode{};
void *characterShadowRuntime{};
std::int32_t characterShadowRuntimeIndex{};
std::array<std::int32_t, 4> characterShadowRenderParameters{};
std::int32_t characterShadowDrawReference{};
float characterShadowVerticalExtrusion{};
Vector3 characterShadowLightVector{};
float characterShadowOpacityScale{};
std::int32_t characterShadowPointLightMode{};
std::int32_t characterShadowMode{};
std::int32_t characterShadowStrongestOnly{};
std::array<Matrix4x4, 5> wbtrControllerTransforms{};
std::int32_t dummyRefboardEnabled{};
std::int32_t dummyRefboardKind{};
Vector4 dummyRefboardColour{};
Vector3 dummyRefboardDirection{};
float dummyRefboardRange{};
Vector3 dummyRefboardPosition{};
float dummyRefboardIntensity{};
float dummyRefboardFalloff{};
MT7Model *shadowSprite{};
std::uint32_t previousMotionShadowLoadingFlag{};



class ScriptedMatrixFrame {
  public:
    explicit ScriptedMatrixFrame(bool enabled = true) : active(enabled) {
        if (active) {
            saved = workTransformMatrix;
            pushed = Matrix4x4_Push();
        }
    }
    ScriptedMatrixFrame(const ScriptedMatrixFrame &) = delete;
    ScriptedMatrixFrame &operator=(const ScriptedMatrixFrame &) = delete;
    ~ScriptedMatrixFrame() {
        if (!active)
            return;
        if (pushed)
            Matrix4x4::Pop(1);
        else
            Matrix4x4::SetActiveTransform(&saved);
    }

  private:
    Matrix4x4 saved;
    bool pushed{};
    bool active{};
};

Vector3 TransformPoint(const Vector3 &value, const Matrix4x4 &matrix) {
    return {value.x * matrix.rows[0].x + value.y * matrix.rows[1].x + value.z * matrix.rows[2].x + matrix.rows[3].x,
        value.x * matrix.rows[0].y + value.y * matrix.rows[1].y + value.z * matrix.rows[2].y + matrix.rows[3].y,
        value.x * matrix.rows[0].z + value.y * matrix.rows[1].z + value.z * matrix.rows[2].z + matrix.rows[3].z};
}

Vector3 TransformVector(const Vector3 &value, const Matrix4x4 &matrix) {
    return {value.x * matrix.rows[0].x + value.y * matrix.rows[1].x + value.z * matrix.rows[2].x,
        value.x * matrix.rows[0].y + value.y * matrix.rows[1].y + value.z * matrix.rows[2].y,
        value.x * matrix.rows[0].z + value.y * matrix.rows[1].z + value.z * matrix.rows[2].z};
}

const MT7Node *FindMdcSourceNode(const MT7Model &model, const MT7Node &node);
bool TransformMdcLinkedVertices(std::span<const MT7MdcVertexLink> links, std::size_t destinationIndex, const std::byte *source,
    std::int32_t sourceSize, std::byte *destination, std::int32_t destinationSize, const Matrix4x4 &relative) {
    bool wrote = false;
    for (const MT7MdcVertexLink &link : links) {
        if (link.nodeIndex != destinationIndex)
            continue;
        const std::uint32_t sourceOffset = link.sourceByteOffset & ~3U;
        const std::uint32_t destinationOffset = link.destinationByteOffset & ~3U;
        if (sourceOffset > static_cast<std::uint32_t>(sourceSize) - 24U || destinationOffset > static_cast<std::uint32_t>(destinationSize) - 24U)
            continue;
        Vector3 position{}, normal{};
        std::memcpy(&position, source + sourceOffset, sizeof(position));
        std::memcpy(&normal, source + sourceOffset + 12, sizeof(normal));
        position = TransformPoint(position, relative);
        normal = TransformVector(normal, relative);
        std::uint32_t positionX{};
        std::memcpy(&positionX, &position.x, sizeof(positionX));
        positionX |= 1U;
        std::memcpy(&position.x, &positionX, sizeof(positionX));
        std::memcpy(destination + destinationOffset, &position, sizeof(position));
        std::memcpy(destination + destinationOffset + 12, &normal, sizeof(normal));
        wrote = true;
    }
    return wrote;
}

bool TransformMdcWeightedVertices(
    const MT7Node &destinationNode, std::byte *destination, std::int32_t destinationSize, const Matrix4x4 &relative) {
    bool wrote = false;
    if (g_motionDeformationEnabled && !destinationNode.mdcWeightedVertices.empty()) {
        const std::uint32_t nodeId = static_cast<std::uint32_t>(destinationNode.id) & 0xffffU;
        const bool normalized = nodeId == 7 || nodeId == 12;
        g_mdcWeightTransformMode = normalized ? 3 : 2;
        float previousWeight = 1.0f;
        Matrix4x4 weighted = relative;
        for (const MT7MdcWeightedVertex &vertex : destinationNode.mdcWeightedVertices) {
            if (vertex.weight != previousWeight) {
                if (normalized)
                    InterpolateNormalizedMdcWeightMatrix(vertex.weight, &relative, &destinationNode.mdcWeightMatrix, &weighted);
                else
                    InterpolateMdcWeightMatrix(vertex.weight, &relative, &destinationNode.mdcWeightMatrix, &weighted);
            }
            Vector3 position = TransformPoint(vertex.position, weighted);
            Vector3 normal = TransformVector(vertex.normal, weighted);
            NormalizeVector3(&normal);
            position.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(position.x) | 1U);
            if (vertex.destinationByteOffset <= static_cast<std::uint32_t>(destinationSize) - 24U) {
                std::memcpy(destination + vertex.destinationByteOffset, &position, sizeof(position));
                std::memcpy(destination + vertex.destinationByteOffset + 12, &normal, sizeof(normal));
                wrote = true;
            }
            previousWeight = vertex.weight;
        }
    }
    return wrote;
}

void ApplyMdcNodeVertexLinks(const MT7Model &model, std::size_t destinationIndex, const MT7Node &sourceNode, const Matrix4x4 &sourceTransform,
    const Matrix4x4 &destinationTransform, std::span<const MT7MdcVertexLink> links, bool upload);

void ApplyWbtrDeformation(const MT7Model &model, std::size_t selectedNode = std::numeric_limits<std::size_t>::max()) {
    const std::array<const Matrix4x4 *, 7> &controllers = g_motionDeformationControllers;
    for (const MT7WbtrDeformer &deformer : model.wbtrDeformers) {
        if (selectedNode != std::numeric_limits<std::size_t>::max() && deformer.nodeIndex != selectedNode)
            continue;
        if (deformer.nodeIndex >= model.nodes.size())
            continue;
        const MT7Node &node = model.nodes[deformer.nodeIndex];
        if (g_motionDeformationEnabled) {
            const std::uint32_t lowId = static_cast<std::uint32_t>(node.id) & 0xffffU;
            const Matrix4x4 *const current = lowId == 5 ? controllers[2] : controllers[4];
            if (!current || (node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources).vertexBuffers.empty())
                continue;
            ScriptedMatrixFrame frame;
            for (const auto &[controller, bind] : deformer.bindMatrices) {
                if (!controller || controller > wbtrControllerTransforms.size() || !controllers[controller - 1])
                    continue;
                Matrix4x4::SetActiveTransform(current);
                InvertModelWorkTransform();
                Matrix4x4::MultiplyWithPrePassMatrix(controllers[controller - 1]);
                Matrix4x4::MultiplyWithPrePassMatrix(&bind);
                Matrix4x4::GetTransformMatrix(&wbtrControllerTransforms[controller - 1]);
            }
            const std::uint32_t constraint = node.mdcVertexLinkFlags >> 16U;
            Vector3 constraintStart{}, constraintEnd{};
            const MT7WbtrConstraint *bounds = nullptr;
            if (constraint) {
                bounds = &mt7WbtrConstraints[constraint - 1U];
                constraintStart = bounds->start;
                constraintEnd = bounds->end;
                if (lowId != 5U) {
                    constraintStart.z = -constraintStart.z;
                    constraintEnd.z = -constraintEnd.z;
                }
                const Matrix4x4 *const shoulder = lowId == 5U ? controllers[1] : controllers[3];
                if (shoulder) {
                    ScriptedMatrixFrame constraintFrame;
                    Matrix4x4::SetActiveTransform(current);
                    InvertModelWorkTransform();
                    Matrix4x4::MultiplyWithPrePassMatrix(shoulder);
                    Matrix4x4::TransformPoint(constraintStart, constraintStart);
                }
            }
            auto &evaluated = deformer.evaluatedVertices;
            for (std::size_t index = 0; index < deformer.vertices.size(); ++index) {
                const MT7WeightedVertex &source = deformer.vertices[index];
                Vector3 position{}, normal{};
                for (const MT7VertexInfluence &influence : source.influences) {
                    if (!influence.controller || influence.controller > wbtrControllerTransforms.size())
                        continue;
                    const Matrix4x4 &matrix = wbtrControllerTransforms[influence.controller - 1];
                    const Vector3 transformedPosition = TransformPoint(source.position, matrix);
                    const Vector3 transformedNormal = TransformVector(source.normal, matrix);
                    position.x += transformedPosition.x * influence.weight;
                    position.y += transformedPosition.y * influence.weight;
                    position.z += transformedPosition.z * influence.weight;
                    normal.x += transformedNormal.x * influence.weight;
                    normal.y += transformedNormal.y * influence.weight;
                    normal.z += transformedNormal.z * influence.weight;
                }
                if (bounds)
                    ClampPointToSegmentRange(&position, &constraintStart, &constraintEnd, bounds->startRadius, bounds->endRadius);
                position.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(position.x) | 1U);
                NormalizeVector3(&normal);
                evaluated[index] = {position, normal};
            }
            if (const LegacyVertexBufferBinding *const binding = FindMT7MutableVertexBuffer(node)) {
                const std::int32_t size = d3t::GetSizeForModelBuffer(binding->handle);
                if (size < 24)
                    continue;
                auto *const destination = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, binding->handle, 1, 0));
                if (!destination)
                    continue;
                bool wrote = false;
                for (const MT7VertexMapping &mapping : deformer.mappings) {
                    if (mapping.sourceVertex >= evaluated.size() || mapping.destinationByteOffset > static_cast<std::uint32_t>(size) - 24U)
                        continue;
                    const auto &[position, normal] = evaluated[mapping.sourceVertex];
                    std::memcpy(destination + mapping.destinationByteOffset, &position, sizeof(position));
                    std::memcpy(destination + mapping.destinationByteOffset + 12, &normal, sizeof(normal));
                    wrote = true;
                }
                if (wrote) {
                    d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, binding->handle);
                }
            }
        }
        // WBTR admission also owns the final parent-mesh weld, including when weighted deformation is disabled.
        if (const Matrix4x4 *const current = GetMT7NodeTransformOverride(node); current && (node.mdcVertexLinkFlags & 0xffffU)) {
            if (const MT7Node *const source = FindMdcSourceNode(model, node))
                if (const Matrix4x4 *const sourceTransform = GetMT7NodeTransformOverride(*source))
                    ApplyMdcNodeVertexLinks(model, deformer.nodeIndex, *source, *sourceTransform, *current, model.mdcVertexLinks, true);
        }
    }
}

const MT7Node *FindMdcSourceNode(const MT7Model &, const MT7Node &node) {
    const MT7Node *const parent = node.runtimeParent;
    return parent && parent->xb01Offset ? parent : nullptr;
}

bool HasWbtrDeformer(const MT7Model &model, std::size_t nodeIndex) {
    return std::any_of(
        model.wbtrDeformers.begin(), model.wbtrDeformers.end(), [nodeIndex](const MT7WbtrDeformer &deformer) { return deformer.nodeIndex == nodeIndex; });
}

void ApplyMdcNodeVertexLinks(const MT7Model &model, std::size_t destinationIndex, const MT7Node &sourceNode, const Matrix4x4 &sourceTransform,
    const Matrix4x4 &destinationTransform, std::span<const MT7MdcVertexLink> links, bool upload = true) {
    const MT7Node &destinationNode = model.nodes[destinationIndex];
    const LegacyVertexBufferBinding *const sourceBinding = FindMT7MutableVertexBuffer(sourceNode);
    const LegacyVertexBufferBinding *const destinationBinding = FindMT7MutableVertexBuffer(destinationNode);
    if (!sourceBinding || !destinationBinding)
        return;
    const std::int32_t sourceSize = d3t::GetSizeForModelBuffer(sourceBinding->handle);
    const std::int32_t destinationSize = d3t::GetSizeForModelBuffer(destinationBinding->handle);
    if (sourceSize < 24 || destinationSize < 24)
        return;
    const auto *const source = static_cast<const std::byte *>(d3t::GetBufferCpuData(sourceBinding->handle, nullptr));
    auto *const destination = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, destinationBinding->handle, 1, 0));
    if (!source || !destination)
        return;
    // TransformCharacterRelativePoint subtracts positions before applying inverse rotation.
    // Keep that ordering rather than cancelling large transformed positions.
    Matrix4x4 destinationBasis = destinationTransform;
    destinationBasis.rows[3] = {0.0f, 0.0f, 0.0f, destinationTransform.rows[3].w};
    Matrix4x4 sourceRelative = sourceTransform;
    sourceRelative.rows[3].x -= destinationTransform.rows[3].x;
    sourceRelative.rows[3].y -= destinationTransform.rows[3].y;
    sourceRelative.rows[3].z -= destinationTransform.rows[3].z;
    ScriptedMatrixFrame frame;
    Matrix4x4::SetActiveTransform(&destinationBasis);
    InvertModelWorkTransform();
    Matrix4x4::MultiplyWithPrePassMatrix(&sourceRelative);
    const Matrix4x4 relative = workTransformMatrix;
    bool wrote = TransformMdcLinkedVertices(links, destinationIndex, source, sourceSize, destination, destinationSize, relative);
    wrote |= TransformMdcWeightedVertices(destinationNode, destination, destinationSize, relative);
    if (wrote && upload && destinationNode.meshType != SHEN_NAME(MDC7))
        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, destinationBinding->handle);
}

// @verified - p
std::uint32_t DrawMdcChildHierarchy(const MT7Model &model, const MT7Node *firstNode, LegacyModelShaderPath shaderPath, float alpha,
    const bool *suppressSearchMesh) {
    std::uint32_t draws = 0;
    for (const MT7Node *current = firstNode; current; current = current->runtimeSibling) {
        const MT7Node &node = *current;
        const bool sibling = node.runtimeSibling != nullptr;
        if (sibling)
            Matrix4x4_Push();
        if (!g_modelDeformationSuppressed && node.xb01Offset && static_cast<std::int32_t>(node.mdcVertexLinkFlags) > 0 &&
            (node.meshType == SHEN_NAME(MDC7) || node.meshType == SHEN_NAME(MDCX))) {
            const MT7Node *const parent = FindMdcSourceNode(model, node);
            const LegacyVertexBufferBinding *const sourceBinding = parent ? FindMT7MutableVertexBuffer(*parent) : nullptr;
            const LegacyVertexBufferBinding *const destinationBinding = FindMT7MutableVertexBuffer(node);
            if (sourceBinding && destinationBinding) {
                const std::int32_t sourceSize = d3t::GetSizeForModelBuffer(sourceBinding->handle);
                const std::int32_t destinationSize = d3t::GetSizeForModelBuffer(destinationBinding->handle);
                const std::byte *const source = static_cast<const std::byte *>(d3t::GetBufferCpuData(sourceBinding->handle, nullptr));
                std::byte *const destination = static_cast<std::byte *>(
                    d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, destinationBinding->handle, 1, 0));
                if (source && destination && sourceSize >= 24 && destinationSize >= 24) {
                    Matrix4x4_PushActiveTransform();
                    if (node.rotation.x)
                        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-static_cast<std::uint16_t>(node.rotation.x)));
                    if (node.rotation.y)
                        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-static_cast<std::uint16_t>(node.rotation.y)));
                    if (node.rotation.z)
                        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-static_cast<std::uint16_t>(node.rotation.z)));
                    Matrix4x4::preTranslate(-node.position.x, -node.position.y, -node.position.z);
                    const Matrix4x4 relative = workTransformMatrix;
                    const std::size_t nodeIndex = static_cast<std::size_t>(&node - model.nodes.data());
                    (void)TransformMdcLinkedVertices(model.mdcVertexLinks, nodeIndex, source, sourceSize, destination, destinationSize, relative);
                    (void)TransformMdcWeightedVertices(node, destination, destinationSize, relative);
                    if (node.meshType == SHEN_NAME(MDCX))
                        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, destinationBinding->handle);
                    Matrix4x4::Pop(1);
                }
            }
        }
        Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
        if (node.rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
        if (node.rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
        if (node.rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
        SetLegacyModelScale(g_scriptedModelScale.x);
        if (node.xb01Offset && (!suppressSearchMesh || !*suppressSearchMesh)) {
            const LegacyDrawResources &resources = node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources;
            const bool lambert = shaderPath == LegacyModelShaderPath::Lambert;
            if (alpha < 1.0f)
                draws += lambert ? SetModelFlagDataWithAlpha(model.source, node.xb01Offset, resources, alpha) :
                                   ReadModelDataWithAlpha(model.source, node.xb01Offset, resources, alpha);
            else
                draws += lambert ? ModelFlagDispatch_SetModelFlagDataOpaque(model.source, node.xb01Offset, resources) :
                                   ModelFlagDispatch_ReadModelDataOpaque(model.source, node.xb01Offset, resources);
        }
        draws += DrawMdcChildHierarchy(model, node.runtimeChild, shaderPath, alpha, suppressSearchMesh);
        if (sibling)
            Matrix4x4::Pop(1);
    }
    return draws;
}

std::uint32_t DrawHierarchy(const MT7Model &model, const MT7Node *firstNode, const Matrix4x4 &entityTransform, LegacyModelShaderPath shaderPath, float alpha,
    float nodeYOffset, const LegacyNodeCommandOverride *commandOverride = nullptr, const Matrix4x4 **overrideCursor = nullptr,
    const Matrix4x4 *parentOverride = nullptr, const MT7Node *selectedNode = nullptr, const Matrix4x4 *controllerTransformBase = nullptr,
    bool *suppressSearchMesh = nullptr) {
    std::uint32_t draws = 0;
    for (const MT7Node *current = firstNode; current; current = current->runtimeSibling) {
        const MT7Node &node = *current;
        if (selectedNode && current != selectedNode)
            continue;
        const Matrix4x4 *const nodeOverride = overrideCursor ? *overrideCursor : nullptr;
        float nodeAlpha = alpha;
        const std::uint16_t handNodeId = static_cast<std::uint16_t>(node.id);
        const bool handNode = handNodeId == 0xffbeU || handNodeId == 0xffbfU;
        if (overrideCursor) {
            // PublishModelNodeMatrices stores absolute character-local transforms.
            // Non-mesh nodes apply no authored transform and consume no row. A WBTR node deforms from
            // the solved rig controllers, as in the per-node draw; other MDC nodes weld to their source
            // through the frame rows.
            if (node.xb01Offset) {
                Matrix4x4::SetActiveTransform(&entityTransform);
                Matrix4x4::MultiplyWithPrePassMatrix(nodeOverride);
                if (!g_modelDeformationSuppressed) {
                    const std::size_t nodeIndex = static_cast<std::size_t>(&node - model.nodes.data());
                    if ((handNodeId == 5 || handNodeId == 10) && HasWbtrDeformer(model, nodeIndex)) {
                        ApplyWbtrDeformation(model, nodeIndex);
                    } else if (!model.mdcVertexLinks.empty()) {
                        if (const MT7Node *const source = FindMdcSourceNode(model, node); source && parentOverride)
                            ApplyMdcNodeVertexLinks(model, nodeIndex, *source, *parentOverride, *nodeOverride, model.mdcVertexLinks);
                    }
                }
                const LegacyNodeCommandOverride *const commands =
                    commandOverride && commandOverride->nodeSourceOffset == node.sourceOffset ? commandOverride : node.commandOverride;
                SetLegacyModelScale(g_scriptedModelScale.x);
                const bool alphaPass = nodeAlpha < 1.0f;
                const bool lambert = shaderPath == LegacyModelShaderPath::Lambert;
                if (commands) {
                    if (commands->resources) {
                        if (alphaPass)
                            draws += lambert ? SetModelFlagDataWithAlpha(commands->bytes, commands->xb01Offset, *commands->resources, nodeAlpha) :
                                               ReadModelDataWithAlpha(commands->bytes, commands->xb01Offset, *commands->resources, nodeAlpha);
                        else
                            draws += lambert ? ModelFlagDispatch_SetModelFlagDataOpaque(commands->bytes, commands->xb01Offset, *commands->resources) :
                                               ModelFlagDispatch_ReadModelDataOpaque(commands->bytes, commands->xb01Offset, *commands->resources);
                    }
                } else if (!mtIsMeshTypeOverridden(model, node)) {
                    const LegacyDrawResources &resources = node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources;
                    if (alphaPass)
                        draws += lambert ? SetModelFlagDataWithAlpha(model.source, node.xb01Offset, resources, nodeAlpha) :
                                           ReadModelDataWithAlpha(model.source, node.xb01Offset, resources, nodeAlpha);
                    else
                        draws += lambert ? ModelFlagDispatch_SetModelFlagDataOpaque(model.source, node.xb01Offset, resources) :
                                           ModelFlagDispatch_ReadModelDataOpaque(model.source, node.xb01Offset, resources);
                }
                ++*overrideCursor;
            }


            draws += DrawHierarchy(model, node.runtimeChild, entityTransform, shaderPath, nodeAlpha, 0.0f, commandOverride, overrideCursor, nodeOverride,
                nullptr, controllerTransformBase);
            continue;
        }
        const Matrix4x4 savedTransform = workTransformMatrix;
        const bool pushed = g_matrixStack.top.info.index + 1 < g_matrixStack.top.info.capacity ? Matrix4x4_Push() : false;
        const std::uint32_t lowId = static_cast<std::uint32_t>(node.id) & 0xffffU;
        const bool selectedSearchArm =
            suppressSearchMesh && ((lowId == 5U && (g_motionDrawOverrideFlags & 1U)) || (lowId == 10U && (g_motionDrawOverrideFlags & 2U)));
        if (selectedSearchArm)
            *suppressSearchMesh = false;
        if (const Matrix4x4 *const transform = GetMT7NodeTransformOverride(node)) {
            Matrix4x4::SetActiveTransform(controllerTransformBase ? controllerTransformBase : &entityTransform);
            Matrix4x4::MultiplyWithPrePassMatrix(transform);
        } else {
            ApplyMT7NodeTransform(node);
        }
        if (nodeYOffset != 0.0f) {
            Matrix4x4::preTranslate(0.0f, nodeYOffset, 0.0f);
        }
        const bool restoreHandBusy = handNode && dartsEventActive && GetLegacyBusyPreparingDraw();
        if (restoreHandBusy)
            SetLegacyBusyPreparingDraw(false);
        // The per-node model draw deforms a WBTR node, or welds an MDC node to its source
        // through the two nodes' solved controller matrices.
        if (!handNode && node.xb01Offset && !g_modelDeformationSuppressed) {
            const std::size_t nodeIndex = static_cast<std::size_t>(&node - model.nodes.data());
            if (HasWbtrDeformer(model, nodeIndex)) {
                ApplyWbtrDeformation(model, nodeIndex);
            } else if (const Matrix4x4 *const current = GetMT7NodeTransformOverride(node)) {
                if (const MT7Node *const source = FindMdcSourceNode(model, node))
                    if (const Matrix4x4 *const sourceTransform = GetMT7NodeTransformOverride(*source))
                        ApplyMdcNodeVertexLinks(model, nodeIndex, *source, *sourceTransform, *current, model.mdcVertexLinks);
            }
        }
        if ((handNode && node.xb01Offset) || (!handNode && node.visible && (!suppressSearchMesh || !*suppressSearchMesh))) {
            if (node.xb01Offset) {
                std::uint32_t meshFlags{};
                std::memcpy(&meshFlags, (node.runtimeMesh ? node.runtimeMesh : model.source.data() + node.xb01Offset) + 4, sizeof(meshFlags));
                if (meshFlags & 4U)
                    NoteXb01Flag4Mesh();
            }
            float originalAlphaScale;
            draws += DrawCharacterHandReplacement(static_cast<std::uint16_t>(lowId), &originalAlphaScale, alpha, handNode ? &node : nullptr);
            nodeAlpha *= originalAlphaScale;
            if (handNode && nodeAlpha > 0.0001f && node.xb01Offset && !g_modelDeformationSuppressed) {
                const std::size_t nodeIndex = static_cast<std::size_t>(&node - model.nodes.data());
                if (const Matrix4x4 *const current = GetMT7NodeTransformOverride(node)) {
                    if (const MT7Node *const source = FindMdcSourceNode(model, node))
                        if (const Matrix4x4 *const sourceTransform = GetMT7NodeTransformOverride(*source))
                            ApplyMdcNodeVertexLinks(model, nodeIndex, *source, *sourceTransform, *current, model.mdcVertexLinks);
                }
            }
            const LegacyNodeCommandOverride *const commands =
                commandOverride && commandOverride->nodeSourceOffset == node.sourceOffset ? commandOverride : node.commandOverride;
            if (handNode && nodeAlpha > 0.0001f) {
                SetLegacyModelScale(g_scriptedModelScale.x);
                if ((!suppressSearchMesh || !*suppressSearchMesh) && (!commands || commands->resources)) {
                    const std::span<const std::byte> bytes = commands ? commands->bytes : model.source;
                    const std::uint32_t offset = commands ? commands->xb01Offset : node.xb01Offset;
                    const LegacyDrawResources &resources = commands ? *commands->resources :
                        node.borrowedMesh                           ? node.borrowedMesh->drawResources :
                                                                      node.drawResources;
                    const bool lambert = shaderPath == LegacyModelShaderPath::Lambert;
                    if (nodeAlpha < 1.0f)
                        draws += lambert ? SetModelFlagDataWithAlpha(bytes, offset, resources, nodeAlpha) :
                                           ReadModelDataWithAlpha(bytes, offset, resources, nodeAlpha);
                    else
                        draws += lambert ? ModelFlagDispatch_SetModelFlagDataOpaque(bytes, offset, resources) :
                                           ModelFlagDispatch_ReadModelDataOpaque(bytes, offset, resources);
                }
            } else if (!handNode && node.xb01Offset && commands) {
                if (commands->resources)
                    draws += ExecuteLegacyDrawCommandStream(commands->bytes, commands->xb01Offset, *commands->resources, shaderPath, nodeAlpha);
            } else if (!handNode && node.xb01Offset && !mtIsMeshTypeOverridden(model, node)) {
                draws += ExecuteLegacyDrawCommandStream(
                    model.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources, shaderPath, nodeAlpha);
            }
        }
        if (handNode) {
            if (node.xb01Offset && nodeAlpha > 0.0001f)
                draws += DrawMdcChildHierarchy(model, node.runtimeChild, shaderPath, nodeAlpha, suppressSearchMesh);
        } else {
            draws += DrawHierarchy(model, node.runtimeChild, entityTransform, shaderPath, nodeAlpha, nodeYOffset, commandOverride, nullptr, nullptr, nullptr,
                controllerTransformBase, suppressSearchMesh);
        }
        if (restoreHandBusy)
            SetLegacyBusyPreparingDraw(true);
        if (selectedSearchArm)
            *suppressSearchMesh = true;
        if (pushed) {
            Matrix4x4::Pop(1);
        } else {
            workTransformMatrix = savedTransform;
            *g_matrixStack.top.stack = savedTransform;
        }
    }
    return draws;
}

bool FindScriptedNodeIndex(const MT7Model &model, const MT7Node *node, std::size_t &index) {
    const std::uintptr_t pointer = reinterpret_cast<std::uintptr_t>(node);
    const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(model.nodes.data());
    if (pointer < begin || pointer - begin >= model.nodes.size() * sizeof(MT7Node) || (pointer - begin) % sizeof(MT7Node))
        return false;
    index = (pointer - begin) / sizeof(MT7Node);
    return true;
}

const MT7Model *FindScriptedNodeOwner(const MT7Model &model, const MT7Node &node, std::size_t &index) {
    if (FindScriptedNodeIndex(model, &node, index))
        return &model;
    const MT7Node *const source = node.borrowedMesh ? node.borrowedMesh : &node;
    if (FindScriptedNodeIndex(model, source, index))
        return &model;
    if (const MT7Model *const package = FindLoadedModelNodeOwner(source, index))
        return package;
    const MapControlData *const control = g_mapControlTask ? static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask)) : nullptr;
    const ModelManagerTaskData *const manager =
        control && control->modelManagerTask ? static_cast<const ModelManagerTaskData *>(HLib::GetTaskParameter(control->modelManagerTask)) : nullptr;
    if (manager) {
        for (const ModelManagerEntry &entry : manager->entries) {
            if (!(entry.flags & 1U))
                continue;
            if (entry.instance && FindScriptedNodeIndex(*entry.instance, source, index))
                return entry.instance;
            if (entry.source && FindScriptedNodeIndex(*entry.source, source, index))
                return entry.source;
        }
        for (std::uintptr_t bucket : manager->buckets) {
            for (const ModelImageIndex *image = reinterpret_cast<const ModelImageIndex *>(bucket); image; image = image->next) {
                for (std::uint32_t level = 0; level < image->levelCount; ++level) {
                    const MT7Model *const reader = image->levels[level].model;
                    if (reader && FindScriptedNodeIndex(*reader, source, index))
                        return reader;
                }
            }
        }
    }
    for (const MT7Model &reader : GetLoadedMapModels())
        if (FindScriptedNodeIndex(reader, source, index))
            return &reader;
    return nullptr;
}

bool ScriptedNodeHasDeformer(const MT7Node &node) {
    return node.meshType == SHEN_NAME(MDP7) || node.meshType == SHEN_NAME(MDPX) || node.meshType == SHEN_NAME(MDC7) || node.meshType == SHEN_NAME(MDCX) ||
        node.meshType == SHEN_NAME(MDH7) || node.meshType == SHEN_NAME(MDHX);
}

// BuildTransformTreeRecursive_ScriptedObjects: model-local MDP copies are
// completed before any hash or material read. Siblings inherit the parent,
// not the preceding sibling's terminal child transform.
void BuildScriptedModelTransformTree(const MT7Model &model, const MT7Node *firstNode, const Matrix4x4 *parentTransform) {
    for (const MT7Node *current = firstNode; current; current = current->runtimeSibling) {
        const MT7Node &node = *current;
        {
            ScriptedMatrixFrame frame(node.runtimeSibling != nullptr);
            if (const Matrix4x4 *transform = GetMT7NodeTransformOverride(node)) {
                Matrix4x4::SetPrePassToTempTransform();
                Matrix4x4::MultiplyWithPrePassMatrix(transform);
            } else {
                ApplyMT7NodeTransform(node);
            }
            Matrix4x4 transform{};
            Matrix4x4::GetTransformMatrix(&transform);
            std::size_t index;
            const MT7Model *const owner = FindScriptedNodeIndex(model, &node, index) ? &model :
                (ScriptedNodeHasDeformer(node) || (node.borrowedMesh && ScriptedNodeHasDeformer(*node.borrowedMesh))) ?
                                                                                       FindScriptedNodeOwner(model, node, index) :
                                                                                       nullptr;
            if (parentTransform && owner && !owner->mdpVertexLinks.empty()) {
                if (const MT7Node *source = FindMdcSourceNode(*owner, node))
                    ApplyMdcNodeVertexLinks(*owner, index, *source, *parentTransform, transform, owner->mdpVertexLinks, node.meshType == SHEN_NAME(MDPX));
            }
            BuildScriptedModelTransformTree(model, node.runtimeChild, &transform);
        }
    }
}

void DrawScriptedModelTransformTree(const MT7Model &model, const MT7Node *firstNode, const Matrix4x4 &incomingTransform, float scale, const float *alpha,
    bool prepare = true, float alphaScale = 1.0f) {
    for (const MT7Node *current = firstNode; current; current = current->runtimeSibling) {
        const MT7Node &node = *current;
        {
            ScriptedMatrixFrame frame(node.runtimeSibling != nullptr);
            float nodeScale = scale;
            if (const Matrix4x4 *transform = GetMT7NodeTransformOverride(node)) {
                Matrix4x4::SetActiveTransform(&incomingTransform);
                Matrix4x4::MultiplyWithPrePassMatrix(transform);
                nodeScale = 1.0f;
            } else {
                ApplyMT7NodeTransform(node);
                if (node.scale.x != 1.0f || node.scale.y != 1.0f || node.scale.z != 1.0f)
                    nodeScale *= node.scale.y;
            }

            bool hashed = false;
            float nodeAlpha = (alpha ? *alpha : 1.0f) * alphaScale;
            if (alpha && prepare) {
                // Recursive_PrepareMatrices hashes in entity space while
                // retaining the caller's projected transform for legacy draws.
                ScriptedMatrixFrame entityFrame(scriptedModelEntity != nullptr);
                if (scriptedModelEntity) {
                    Matrix4x4::SetPrePassToTempTransform();
                    ApplyCharacterEntityTransform(scriptedModelEntity);
                }
                hashed = d3t::HashModelWithAlpha(&node, nodeAlpha, GetLegacyPerspectiveProjection());
            }
            if (node.xb01Offset && !hashed) {
                std::size_t index;
                const MT7Model *const owner = FindScriptedNodeIndex(model, &node, index) ? &model :
                    (ScriptedNodeHasDeformer(node) || (node.borrowedMesh && ScriptedNodeHasDeformer(*node.borrowedMesh))) ?
                                                                                           FindScriptedNodeOwner(model, node, index) :
                                                                                           nullptr;
                if (alpha && prepare && !g_modelDeformationSuppressed && owner) {
                    if (HasWbtrDeformer(*owner, index)) {
                        ApplyWbtrDeformation(*owner, index);
                    } else if (const Matrix4x4 *current = GetMT7NodeTransformOverride(node)) {
                        if (const MT7Node *source = FindMdcSourceNode(*owner, node)) {
                            if (const Matrix4x4 *sourceTransform = GetMT7NodeTransformOverride(*source))
                                ApplyMdcNodeVertexLinks(*owner, index, *source, *sourceTransform, *current, owner->mdcVertexLinks);
                        }
                    }
                }
                SetLegacyModelScale(nodeScale);
                if (node.commandOverride && node.commandOverride->resources) {
                    const LegacyNodeCommandOverride &commands = *node.commandOverride;
                    ExecuteLegacyDrawCommandStream(commands.bytes, commands.xb01Offset, *commands.resources, GetLegacyCharacterShaderPath(), nodeAlpha);
                } else if (!mtIsMeshTypeOverridden(model, node)) {
                    ExecuteLegacyDrawCommandStream(model.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources,
                        GetLegacyCharacterShaderPath(), nodeAlpha);
                }
            }
            DrawScriptedModelTransformTree(model, node.runtimeChild, incomingTransform, nodeScale, alpha, prepare, alphaScale);
        }
    }
}

} // namespace
// @verified - p
float GetShadowRange() {
    return g_shadowRange.x;
}

// @verified - p
void SHRG_Reader(const void *chunk) {
    if (chunk) {
        float range;
        std::memcpy(&range, static_cast<const std::byte *>(chunk) + sizeof(MapInfoChunk), sizeof(range));
        g_shadowRange.x = std::fmax(0.1f, range);
        g_shadowRange.y = 1.0f / g_shadowRange.x;
    } else {
        g_shadowRange.x = defaultShadowRange;
        g_shadowRange.y = 1.0f / defaultShadowRange;
    }
}

// @verified - p
void EV_SetAlignedShadowRange(float range) {
    g_shadowRange.x = std::fmax(0.1f, range);
    g_shadowRange.y = 1.0f / g_shadowRange.x;
}

// @verified - p
void SetGlobalShadowLightingEnabled(std::int32_t enabled) {
    globalShadowLightingEnabled = enabled;
}

bool IsGlobalShadowLightingEnabled() {
    return globalShadowLightingEnabled != 0;
}

void ResetGlobalShadowTiming() {
    globalShadowLightingEnabled = 1;
    globalShadowFadeRequested = 0;
    globalShadowFade = 1.0f;
    globalShadowWeatherAlpha = 0.4f;
    globalShadowRyoAlpha = 1.0f;
    globalShadowOpacity = 0.4f;
}

void SetGlobalShadowFadeRequested(std::int32_t requested) {
    globalShadowFadeRequested = requested;
}

float GetGlobalShadowOpacity() {
    return globalShadowOpacity;
}

void UpdateGlobalShadowTiming() {
    // UpdateGlobalShadowTiming; RootTask owns its frame position.
    float weather = GetWeatherPressure();
    if (weather <= 0.1f)
        weather = GetWeatherWindVector();
    globalShadowWeatherAlpha = weather <= 0.1f ? 0.4f : (1.0f - weather) * 0.4f;
    DateTime date{};
    GetCurrentDateTime(date);
    const float minute = static_cast<float>(date.minute) / 60.0f;
    float opacity{};
    if (date.hour == 11)
        opacity = (1.0f - minute) * globalShadowWeatherAlpha;
    else if (date.hour == 12)
        opacity = minute * globalShadowWeatherAlpha;
    else if (date.hour == 16 || date.hour == 17)
        opacity = ((18.0f - (minute + static_cast<float>(date.hour))) * 0.5f) * globalShadowWeatherAlpha;
    else
        opacity = date.hour < 18 ? globalShadowWeatherAlpha : 0.0f;
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control) {
        globalShadowRyoAlpha = 1.0f;
    } else if (!plymIsSearchStartRequested()) {
        if (authGetLoaded())
            globalShadowRyoAlpha = 1.0f;
        opacity *= globalShadowRyoAlpha;
    } else {
        float target = control->ryo ? static_cast<const Character *>(control->ryo)->alpha : 0.0f;
        if (GetScenarioFollowCharacter().name || srchGetCurrentFocus().name || srchHasActiveLockonTarget() || HasScenarioCharacterControl())
            target = 1.0f;
        if (globalShadowRyoAlpha < target)
            globalShadowRyoAlpha = std::fmin(target, globalShadowRyoAlpha + 0.1f);
        else if (globalShadowRyoAlpha > target)
            globalShadowRyoAlpha = std::fmax(target, globalShadowRyoAlpha - 0.1f);
        opacity *= globalShadowRyoAlpha;
    }
    globalShadowFade = std::clamp(globalShadowFade + (globalShadowFadeRequested ? -0.1f : 0.1f), 0.0f, 1.0f);
    globalShadowOpacity = opacity * globalShadowFade;
}

void MaybeDoGlobalShadowLighting() {
    if (GetMainMapModelBytes().empty() || !IsGlobalShadowLightingEnabled() || IsMainIconOverlayActive() || IsWazamakiInactive() != 0 ||
        GetGlobalShadowOpacity() < 0.01f)
        return;
    DateTime date{};
    GetCurrentDateTime(date);
    if (static_cast<std::uint8_t>(date.hour - 8) <= 10U && static_cast<std::uint32_t>(GetCurrentActiveLight() - 2) > 1U)
        DrawQueuedMainMapProjectedShadows();
}



void ResolveGlobalShadowLighting(float opacity) {
    if (!IsGlobalShadowLightingEnabled())
        return;
    if (opacity == 0.0f && (IsMainIconOverlayActive() || IsWazamakiInactive() != 0 || GetGlobalShadowOpacity() < 0.01f))
        return;
    const float alpha = opacity <= 0.0f ? GetGlobalShadowOpacity() : opacity;
    const auto packedAlpha = static_cast<std::uint8_t>(static_cast<std::int32_t>(alpha * 255.0f));
    PerformCommonShaderPass(static_cast<std::uint32_t>(packedAlpha) << 24U);
}

std::uint32_t DrawMT7Model(const MT7Model &model, const Matrix4x4 *entityTransform, LegacyModelShaderPath shaderPath, float alpha,
    const LegacyNodeCommandOverride *commandOverride, const Matrix4x4 *matrixFrames, const Matrix4x4 *controllerTransformBase) {
    const bool searchOverride = !matrixFrames && controllerTransformBase && g_motionDrawOverrideFlags;
    if (searchOverride)
        alpha = SearchIK_GetMaxBlend();
    if (!matrixFrames && d3t::GetModelRuntimeState(GetMT7ReaderData(&model))) {
        ScriptedMatrixFrame frame;
        if (entityTransform)
            Matrix4x4::SetActiveTransform(entityTransform);
        if (d3t::HashModelWithAlpha(GetMT7ReaderData(&model), alpha, GetLegacyPerspectiveProjection()))
            return 0;
    }
    const Matrix4x4 rootTransform = entityTransform ? *entityTransform : workTransformMatrix;
    if (matrixFrames) {
        const Matrix4x4 saved = workTransformMatrix;
        const bool pushed = Matrix4x4_Push();
        const Matrix4x4 *cursor = matrixFrames;
        const std::uint32_t draws = DrawHierarchy(model, model.nodes.empty() ? nullptr : &model.nodes.front(), rootTransform, GetLegacyCharacterShaderPath(),
            alpha, 0.0f, nullptr, &cursor, matrixFrames);
        if (pushed)
            Matrix4x4::Pop(1);
        else
            Matrix4x4::SetActiveTransform(&saved);
        g_scriptedModelScale.x = 1.0f;
        return draws;
    }
    bool suppressSearchMesh = searchOverride;
    return DrawHierarchy(model, model.nodes.empty() ? nullptr : &model.nodes.front(), rootTransform, shaderPath, alpha, 0.0f, commandOverride, nullptr, nullptr,
        nullptr, controllerTransformBase, searchOverride ? &suppressSearchMesh : nullptr);
}

std::uint32_t DrawMT7NodeHierarchy(const MT7Model &model, const MT7Node &node, LegacyModelShaderPath shaderPath, float alpha) {
    const Matrix4x4 rootTransform = workTransformMatrix;
    return DrawHierarchy(model, &node, rootTransform, shaderPath, alpha, 0.0f, nullptr, nullptr, nullptr, &node);
}

Character *SetScriptedModelEntity(Character *entity) {
    Character *const previous = scriptedModelEntity;
    scriptedModelEntity = entity;
    return previous;
}

void charaApplyScale(const MT7Model *model, const MT7Node *node, float) {
    if (model && node) {
        ScriptedMatrixFrame outer;
        Matrix4x4::GetTransformMatrix(&g_scriptedIncomingTransform);
        {
            ScriptedMatrixFrame local;
            Matrix4x4::SetPrePassToTempTransform();
            BuildScriptedModelTransformTree(*model, node, nullptr);
        }
        DrawScriptedModelTransformTree(*model, node, g_scriptedIncomingTransform, g_scriptedModelScale.x, nullptr);
    }
    g_scriptedModelScale.x = 1.0f;
}

// @verified - p
void imgmDrawModelNode(const MT7Model *model, const MT7Node *node, float) {
    float scale = g_scriptedModelScale.x;
    Matrix4x4_Push(nullptr);
    if (const Matrix4x4 *const transform = GetMT7NodeTransformOverride(*node)) {
        Matrix4x4::MultiplyWithPrePassMatrix(transform);
        scale = 1.0f;
    } else {
        ApplyMT7NodeTransform(*node);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
            scale *= node->scale.y;
    }
    if (node->xb01Offset) {
        SetLegacyModelScale(scale);
        ExecuteLegacyDrawCommandStream(model->source, node->xb01Offset, node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources,
            GetLegacyCharacterShaderPath(), 1.0f);
    }
    Matrix4x4::Pop(1);
    g_scriptedModelScale.x = 1.0f;
}

void ConstructMatricesForObjectRecursive(MT7Model &model, MT7Node *node) {
    if (!node) {
        g_scriptedModelScale.x = 1.0f;
        return;
    }
    const std::int32_t sibling = node->nextSibling;
    MT7Node *const runtimeSibling = node->runtimeSibling;
    node->nextSibling = -1;
    node->runtimeSibling = nullptr;
    Matrix4x4_Push(nullptr);
    Matrix4x4_Push(nullptr);
    Matrix4x4::SetPrePassToTempTransform();
    BuildScriptedModelTransformTree(model, node, nullptr);
    Matrix4x4::Pop(1);
    DrawScriptedModelTransformTree(model, node, g_scriptedIncomingTransform, g_scriptedModelScale.x, nullptr);
    Matrix4x4::Pop(1);
    node->nextSibling = sibling;
    node->runtimeSibling = runtimeSibling;
    g_scriptedModelScale.x = 1.0f;
}

// @verified - p
void PrepareProceduralObjects(MT7Model &model, MT7Node *node, float alpha) {
    if (node) {
        const std::int32_t sibling = node->nextSibling;
        MT7Node *const runtimeSibling = node->runtimeSibling;
        node->nextSibling = -1;
        node->runtimeSibling = nullptr;
        Matrix4x4_Push(nullptr);
        Matrix4x4_Push(nullptr);
        Matrix4x4::SetPrePassToTempTransform();
        BuildScriptedModelTransformTree(model, node, nullptr);
        Matrix4x4::Pop(1);
        DrawScriptedModelTransformTree(model, node, g_scriptedIncomingTransform, g_scriptedModelScale.x, &alpha);
        Matrix4x4::Pop(1);
        node->nextSibling = sibling;
        node->runtimeSibling = runtimeSibling;
    }
    g_scriptedModelScale.x = 1.0f;
}

void mtUpdateTransparencyCallback(const MT7Model *model, const MT7Node *node, float alpha) {
    ScriptedMatrixFrame frame;
    Matrix4x4::GetTransformMatrix(&g_scriptedIncomingTransform);
    if (g_motionDrawOverrideFlags)
        alpha = SearchIK_GetMaxBlend();
    if (!d3t::HashModelWithAlpha(GetMT7ReaderData(model), alpha, GetLegacyPerspectiveProjection())) {
        bool suppressSearchMesh = g_motionDrawOverrideFlags != 0;
        DrawHierarchy(*model, node, g_scriptedIncomingTransform, GetLegacyCharacterShaderPath(), alpha, 0.0f, nullptr, nullptr, nullptr, nullptr,
            &g_scriptedIncomingTransform, suppressSearchMesh ? &suppressSearchMesh : nullptr);
    }
    g_scriptedModelScale.x = 1.0f;
}

// @verified - p
void SetupRefboard() {
    dummyRefboardIntensity = 1.0f;
    dummyRefboardColour = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
    dummyRefboardEnabled = 0;
    dummyRefboardRange = 2.0f;
    dummyRefboardFalloff = 0.25f;
    dummyRefboardKind = 3;
}

namespace {
void ApplyDummyModelNodeTransform(const MT7Node &node) {
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
}

void DrawDummyModelMesh(const MT7Model &model, const MT7Node &node) {
    const MT7Node &mesh = node.borrowedMesh ? *node.borrowedMesh : node;
    if (!mesh.xb01Offset)
        return;
    const std::byte *const bytes = mesh.runtimeMesh ? mesh.runtimeMesh : model.source.data() + mesh.xb01Offset;
    if (IsModelXB01(bytes)) {
        Vector3 centre;
        float radius;
        std::memcpy(&centre, bytes + 8, sizeof(centre));
        std::memcpy(&radius, bytes + 20, sizeof(radius));
        if (!GetModelCheck(&centre, radius)) {
            SetLegacyModelScale(1.0f);
            return;
        }
    }
    ExecuteLegacyDrawCommandStream(model.source, mesh.xb01Offset, mesh.drawResources, LegacyModelShaderPath::Lambert, 1.0f);
    SetLegacyModelScale(1.0f);
}
} // namespace

std::uint64_t DrawDummyCharacterModel(const MT7Model &model) {
    const std::int32_t active = GetCurrentActiveLight();
    if (active != 2)
        SetCurrentActiveLight(2);
    const std::uint32_t slot = GetAvailableSelectedLightSlot();
    Matrix4x4_Push(nullptr);
    ReturnZero();
    const MT7Node &root = model.nodes.front();
    ApplyDummyModelNodeTransform(root);
    DrawDummyModelMesh(model, root);
    if (const MT7Node *const child = root.runtimeChild) {
        Matrix4x4 projection;
        if (slot != 17) {
            ResetLightEntry(slot);
            SetLightKind(slot, dummyRefboardKind);
            shenSetLightColour(slot, dummyRefboardColour);
            SetSelectedLightIntensity(slot, dummyRefboardIntensity);
            shenSetLightPosition(slot, &dummyRefboardPosition);
            SetSelectedLightDirection(slot, dummyRefboardDirection);
            shenSetLightScale2(slot, dummyRefboardRange);
            shenSetLightScale(slot, dummyRefboardFalloff);
            SetLegacySelectedLightEnabled(slot, dummyRefboardEnabled != 0);
            camCtrlGetProjectionMatrix(&projection);
            Matrix4x4_Push(nullptr);
            Matrix4x4::SetActiveTransform(&projection);
            UpdateLegacyLightTransforms();
            Matrix4x4::Pop(1);
        }
        ApplyDummyModelNodeTransform(*child);
        DrawDummyModelMesh(model, *child);
        if (slot != 17) {
            ResetLightEntry(slot);
            Matrix4x4::SetActiveTransform(&projection);
            UpdateLegacyLightTransforms();
        }
    }
    if (active != 2)
        SetCurrentActiveLight(active);
    Matrix4x4::Pop(1);
    return ReturnZero();
}

void DrawPIN(shen_name name, std::int32_t, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const pin = GetScenarioCharacterNode(control->scenarioTask, SCENARIO_PIN_CHARACTER);
    Matrix4x4_Push(nullptr);
    ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(pin));
    if (position)
        Matrix4x4::preTranslate(position->x, position->y, position->z);
    if (rotation) {
        if (rotation->z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation->z));
        if (rotation->y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation->y));
        if (rotation->x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation->x));
    }
    if (scale)
        Matrix4x4::Scale(scale->x, scale->y, scale->z);
    MapControlData *const current = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = GetScenarioCharacterNode(current->scenarioTask, name);
    const MT7Model *const model = static_cast<const MT7Model *>(character->model);
    charaApplyScale(model, model ? model->nodes.data() : nullptr);
    Matrix4x4::Pop(1);
}


void ApplyTranslationRotationScaleXYZToCharaByID(
    shen_name name, std::int32_t siblingOrdinal, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const dummy = GetScenarioCharacterNode(control->scenarioTask, SCENARIO_DUMMY_CHARACTER);
    MapControlData *const current = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = GetScenarioCharacterNode(current->scenarioTask, name);
    MT7Model &model = *const_cast<MT7Model *>(static_cast<const MT7Model *>(character->model));
    MT7Node *selected = model.nodes[0].runtimeChild->runtimeChild;
    for (std::int32_t ordinal = 0; ordinal < siblingOrdinal; ++ordinal)
        selected = selected->runtimeSibling;
    Matrix4x4_Push(nullptr);
    ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(dummy));
    if (position)
        Matrix4x4::preTranslate(position->x, position->y, position->z);
    if (rotation) {
        if (rotation->z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation->z));
        if (rotation->y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation->y));
        if (rotation->x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation->x));
    }
    if (scale)
        Matrix4x4::Scale(scale->x, scale->y, scale->z);
    if (selected) {
        const std::int32_t sibling = selected->nextSibling;
        MT7Node *const runtimeSibling = selected->runtimeSibling;
        selected->nextSibling = -1;
        selected->runtimeSibling = nullptr;
        Matrix4x4_Push(nullptr);
        const Matrix4x4 incoming = workTransformMatrix;
        Matrix4x4_Push(nullptr);
        Matrix4x4::SetPrePassToTempTransform();
        BuildScriptedModelTransformTree(model, selected, nullptr);
        Matrix4x4::Pop(1);
        DrawScriptedModelTransformTree(model, selected, incoming, g_scriptedModelScale.x, nullptr);
        Matrix4x4::Pop(1);
        selected->nextSibling = sibling;
        selected->runtimeSibling = runtimeSibling;
    }
    g_scriptedModelScale.x = 1.0f;
    Matrix4x4::Pop(1);
}

void charaPrepareNodesForDraw(const MT7Model *model, const MT7Node *node, float alpha) {
    if (model && node) {
        ScriptedMatrixFrame outer;
        Matrix4x4::GetTransformMatrix(&g_scriptedIncomingTransform);
        {
            ScriptedMatrixFrame local;
            Matrix4x4::SetPrePassToTempTransform();
            BuildScriptedModelTransformTree(*model, node, nullptr);
        }
        DrawScriptedModelTransformTree(*model, node, g_scriptedIncomingTransform, g_scriptedModelScale.x, &alpha);
    }
    g_scriptedModelScale.x = 1.0f;
}
void DrawMT7ProceduralBranch(MT7Model &model, MT7Node *node, std::uint16_t cutoffId, float alpha) {
    if (node) {
        ScriptedMatrixFrame outer;
        const Matrix4x4 incoming = workTransformMatrix;
        const std::int32_t sibling = node->nextSibling;
        MT7Node *const runtimeSibling = node->runtimeSibling;
        node->nextSibling = -1;
        node->runtimeSibling = nullptr;
        MT7Node *const cutoff = cutoffId == 0xffffU ? nullptr : GetChildObjectByIndex(model, node, cutoffId);
        const std::int32_t cutoffChild = cutoff ? cutoff->firstChild : -1;
        const std::int32_t cutoffSibling = cutoff ? cutoff->nextSibling : -1;
        MT7Node *const cutoffRuntimeChild = cutoff ? cutoff->runtimeChild : nullptr;
        MT7Node *const cutoffRuntimeSibling = cutoff ? cutoff->runtimeSibling : nullptr;
        if (cutoff) {
            cutoff->firstChild = -1;
            cutoff->nextSibling = -1;
            cutoff->runtimeChild = nullptr;
            cutoff->runtimeSibling = nullptr;
        }
        {
            ScriptedMatrixFrame local;
            Matrix4x4::SetPrePassToTempTransform();
            BuildScriptedModelTransformTree(model, node, nullptr);
        }
        DrawScriptedModelTransformTree(model, node, incoming, g_scriptedModelScale.x, &alpha);
        if (cutoff) {
            cutoff->firstChild = cutoffChild;
            cutoff->nextSibling = cutoffSibling;
            cutoff->runtimeChild = cutoffRuntimeChild;
            cutoff->runtimeSibling = cutoffRuntimeSibling;
        }
        node->nextSibling = sibling;
        node->runtimeSibling = runtimeSibling;
    }
    g_scriptedModelScale.x = 1.0f;
}

namespace {
bool shadowRelativePositioning = true;
std::int32_t characterShadowStencilReference = -1;

float GetCharacterShadowDistanceFade(const Vector3 &position, const Character &character) {
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    const float dx = position.x - camera.x, dz = position.z - camera.z;
    const float range = GetShadowRange() * ((character.fieldE8 & 0x10U) ? 3.0f : 1.0f);
    const float fade = range - std::sqrt(dx * dx + dz * dz);
    return (fade < 0.0f ? 0.0f : std::fmin(1.0f, fade)) * character.alpha;
}

// ComputeTransformedVector: positioning mode affects directional X/Z only.
float ComputeShadowLightSource(const Vector3 &position, Vector3 &source, float strength, std::uint32_t lightIndex) {
    const LightRuntimeEntry *light = GetLightRuntimeEntry(lightIndex);
    if (!light)
        return 0.0f;
    float coefficient = lightIndex != 0 || strength == -1.0f ? 0.5f : strength;
    if (strength == -2.0f)
        coefficient = 1.0f;
    const std::uint8_t kind = light->indexes[1] & 0xf7U;
    if (kind == 2) {
        source = {position.x, position.y - light->values2c[1], position.z};
        if (shadowRelativePositioning) {
            source.x -= light->values2c[0];
            source.z -= light->value34;
        }
    } else if (kind == 3 || kind == 4) {
        source = light->vector20;
        if (kind == 3) {
            Vector3 root;
            GetLightRootPosition(&root);
            source.x += root.x;
            source.y += root.y;
            source.z += root.z;
        }
        if (source.y - position.y < -0.3f)
            return 0.0f;
        if (source.y - position.y < 1.0f)
            source.y = position.y + 1.0f;
    } else
        return 0.0f;
    return coefficient < 0.0f ? 0.0f : std::fmin(1.0f, coefficient);
}

void PrepareCharacterShadowLighting() {
    SetCurrentActiveLight(1);
    UpdateGlobalLighting({});
    for (std::uint32_t slot = 0; slot < 16; ++slot)
        SetLegacySelectedLightEnabled(slot, false);
    UpdateLegacyLightTransforms();
}

// ComputeDirectionAlignedTransform_0 includes direction pre-translation.
void ApplyDirectionAlignedShadowProjection(Vector3 direction, float groundY, float numerator, float denominator) {
    const float maximumY = -std::sqrt((direction.z * direction.z + direction.x * direction.x) * numerator / denominator);
    if (direction.y > maximumY)
        direction.y = maximumY;
    const float reciprocal = 1.0f / -direction.y;
    const float x = -direction.x, y = -direction.y, z = -direction.z;
    const float plane = (groundY + 0.013f) * reciprocal;
    const Matrix4x4 projection{{{1.0f, 0.0f, 0.0f, 0.0f}, {-reciprocal * x, 1.0f - reciprocal * y, -reciprocal * z, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f},
        {plane * x, plane * y, plane * z, 1.0f}}};
    Matrix4x4::MultiplyWithPrePassMatrix(&projection);
    Matrix4x4::preTranslate(direction.x, direction.y, direction.z);
}

void ApplyShadowPlacement(const Vector3 &position, const Vector3i &rotation, const Vector3 &scale) {
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    if (rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    if (rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    if (rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    const bool scaled = scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f;
    if (scaled)
        Matrix4x4::Scale(scale.x, scale.y, scale.z);
    g_scriptedModelScale.x = (scaled ? std::fmax(scale.z, std::fmax(scale.y, scale.x)) : 1.0f) * 3.0f;
    SetLegacyModelScale(g_scriptedModelScale.x);
}

void DrawBaseShadowModel(const MT7Model &model, float alpha) {
    if (model.nodes.empty())
        return;
    ScriptedMatrixFrame frame;
    Matrix4x4::GetTransformMatrix(&g_scriptedIncomingTransform);
    const Matrix4x4 incoming = workTransformMatrix;
    DrawScriptedModelTransformTree(model, model.nodes.data(), incoming, g_scriptedModelScale.x, &alpha, false);
    g_scriptedModelScale.x = 1.0f;
}


void PublishSharedShadowNodeTransforms(const MT7Model &source, MT7Model &shadow) {
    for (MT7Node &node : shadow.nodes) {
        SetMT7NodeTransformOverride(node, nullptr);
        if (static_cast<std::uint16_t>(node.id) == 0xffffU)
            continue;
        const auto match = std::find_if(source.nodes.begin(), source.nodes.end(), [&](const MT7Node &candidate) { return candidate.id == node.id; });
        if (match != source.nodes.end())
            SetMT7NodeTransformOverride(node, GetMT7NodeTransformOverride(*match));
    }
}


void DrawProjectedSharedCharacterShadow(Character &character, const Vector3 &position, float groundY, std::uint32_t lightIndex, float opacity) {
    const MT7Model *model = static_cast<const MT7Model *>(character.model);
    MT7Model *shared = const_cast<MT7Model *>(GetSHDS());
    if (!model || !shared || !(opacity > 0.003f))
        return;
    if (model != shared)
        PublishSharedShadowNodeTransforms(*model, *shared);
    Vector3 source{};
    const float alpha = opacity * ComputeShadowLightSource(position, source, -1.0f, lightIndex);
    if (alpha >= 0.003f) {
        ScriptedMatrixFrame frame;
        ApplyDirectionAlignedShadowProjection({position.x - source.x, position.y - source.y, position.z - source.z}, groundY, 0.11108889f, 0.88891113f);
        g_scriptedModelScale.x = 3.0f;
        if (GetPreviousActiveLight() != 1)
            SetLegacyStencilReference(static_cast<std::uint32_t>(alpha * 255.0f));
        DrawBaseShadowModel(*shared, alpha);
    }
}

void DrawMotionCharacterShadow(Character &character, const Vector3 &position, float groundY, const MT7Model &model, MT7Model *shadow, float strength) {
    if (g_characterShadowDisabled || !shadow || strength == 0.0f || !IsCharacterDrawControlEnabled(&character) || !(position.y - groundY <= 2.2f))
        return;
    Vector3 probe = position;
    probe.y += 0.5f;
    flddPushIntersectionFlag(32);
    const float intersection = shenFindIntersectionWithScene(probe, position.z);
    std::int32_t phase = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(character.fieldE8 >> 16));
    if (character.type == 3 || intersection > position.y - 1.5f) {
        if (phase < 10)
            phase = 11;
        else if (phase < 20)
            ++phase;
    } else if (phase <= 11)
        phase = 1;
    else
        --phase;
    character.fieldE8 = (character.fieldE8 & ~0x00ff0000U) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(phase)) << 16);
    if (&model != shadow)
        PublishSharedShadowNodeTransforms(model, *shadow);
    const float fade = GetCharacterShadowDistanceFade(position, character);
    if (!(fade >= 0.003f))
        return;
    float projection = 0.33329999f;
    if (phase > 10)
        projection += std::fmin(1.0f, static_cast<float>(phase - 10) * 0.1f) * 0.61669999f;
    const bool stencil = GetPreviousActiveLight() != 1;
    PrepareCharacterShadowLighting();
    for (std::size_t slot = 0; slot < 6 && character.fieldEc[slot] >= 0; ++slot) {
        const float weight = std::sqrt(character.field110[slot]) * std::bit_cast<float>(character.fieldF8[slot]);
        if (!(weight >= 0.003f))
            continue;
        Vector3 source{};
        const float alpha = ComputeShadowLightSource(position, source, strength, character.fieldEc[slot]) * (weight * fade);
        if (!(alpha >= 0.003f))
            continue;
        ScriptedMatrixFrame frame;
        ApplyDirectionAlignedShadowProjection(
            {position.x - source.x, position.y - source.y, position.z - source.z}, groundY, projection * projection, 1.0f - projection * projection);
        g_scriptedModelScale.x = 3.0f;
        if (stencil)
            SetLegacyStencilReference(static_cast<std::uint32_t>(alpha * 255.0f));
        DrawBaseShadowModel(*shadow, alpha);
    }
}

void DrawCharacterShadowSprite(Character &character, const Vector3 &position, float groundY) {
    if (g_characterShadowDisabled || !shadowSprite || shadowSprite->nodes.empty() || !shadowSprite->nodes[0].xb01Offset ||
        !IsCharacterDrawControlEnabled(&character) || !(position.y - groundY <= 2.2f))
        return;
    std::int32_t phase = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(character.fieldE8 >> 16));
    if (character.type == 3) {
        if (phase < 10)
            phase = 11;
        else if (phase < 20)
            ++phase;
    } else if (phase <= 11)
        phase = 1;
    else
        --phase;
    character.fieldE8 = (character.fieldE8 & ~0x00ff0000U) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(phase)) << 16);
    const float fade = GetCharacterShadowDistanceFade(position, character);
    if (!(fade >= 0.003f))
        return;
    float projection = 0.33329999f;
    if (phase > 10)
        projection += std::fmin(1.0f, static_cast<float>(phase - 10) * 0.1f) * 0.61669999f;
    PrepareCharacterShadowLighting();
    const bool suspended = SuspendLegacyCharacterShadowFrame();
    const MT7Node &root = shadowSprite->nodes[0];
    for (std::size_t slot = 0; slot < 6 && character.fieldEc[slot] >= 0; ++slot) {
        const float weight = std::sqrt(character.field110[slot]) * std::bit_cast<float>(character.fieldF8[slot]);
        if (!(weight >= 0.003f))
            continue;
        Vector3 source{};
        const float alpha = ComputeShadowLightSource(position, source, -1.0f, character.fieldEc[slot]) * (weight * fade);
        if (!(alpha >= 0.003f))
            continue;
        ScriptedMatrixFrame frame;
        ApplyDirectionAlignedShadowProjection(
            {position.x - source.x, position.y - source.y, position.z - source.z}, groundY, projection * projection, 1.0f - projection * projection);
        Vector3 direction{source.x - position.x, source.y - position.y, source.z - position.z};
        NormalizeVector3(&direction);
        Matrix4x4::preTranslate(direction.x * 0.2f + position.x, position.y, direction.z * 0.2f + position.z);
        const float horizontal = direction.z * direction.z + direction.x * direction.x;
        if (horizontal > 0.000001f) {
            const float reciprocal = 1.0f / std::sqrt(horizontal);
            ApplyShearToPrePassMatrix(-direction.x * reciprocal, -direction.z * reciprocal);
        }
        Matrix4x4::Scale(0.5f, 1.7f, 1.0f);
        SetLegacyModelScale(5.1000004f);
        ExecuteLegacyDrawCommandStream(shadowSprite->source, root.xb01Offset, root.borrowedMesh ? root.borrowedMesh->drawResources : root.drawResources,
            GetLegacyCharacterShaderPath(), alpha >= 1.0f ? 1.0f : alpha);
    }
    ResumeLegacyCharacterShadowFrame(suspended);
}



void DrawCharacterStencilHierarchy(const MT7Model &model, const MT7Model &shadow, std::int32_t index, const Matrix4x4 &base) {
    while (index >= 0) {
        const MT7Node &node = shadow.nodes[static_cast<std::size_t>(index)];
        const std::uint16_t signature = static_cast<std::uint16_t>(node.id) == 70 ? 0xffbdU : static_cast<std::uint16_t>(node.id);
        const auto match = std::find_if(
            model.nodes.begin(), model.nodes.end(), [&](const MT7Node &candidate) { return static_cast<std::uint16_t>(candidate.id) == signature; });
        Matrix4x4::SetActiveTransform(&base);
        if (match != model.nodes.end()) {
            if (const Matrix4x4 *transform = GetMT7NodeTransformOverride(*match))
                Matrix4x4::MultiplyWithPrePassMatrix(transform);
            else
                ApplyMT7NodeTransform(*match);
        }
        if (node.xb01Offset)
            ExecuteProjectedShadowCommandStream(shadow.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources);
        Matrix4x4::SetActiveTransform(&base);
        DrawCharacterStencilHierarchy(model, shadow, node.firstChild, base);
        index = node.nextSibling;
    }
}

}

void SetShadowPositioningMode(bool relative) {
    shadowRelativePositioning = relative;
}
// @verified - p
void SetCharacterUngroundedShadowMode(std::int32_t mode) {
    characterUngroundedShadowMode = mode;
}
void ResetCharacterShadowStencilReference() {
    characterShadowStencilReference = -1;
}

void ProjectCharacterShadow(
    Character *character, const Vector3 &position, const Vector3i &rotation, const Vector3 &scale, float groundY, const MT7Model *root, float strength) {
    if (g_characterShadowDisabled || !root || !character || !character->prepareForDraw || strength == 0.0f || !IsCharacterDrawControlEnabled(character) ||
        !(position.y - groundY <= 2.2f))
        return;
    const float fade = GetCharacterShadowDistanceFade(position, *character);
    if (!(fade >= 0.003f))
        return;
    PrepareCharacterShadowLighting();
    if (character->fieldEc[0] < 0)
        return;
    const float weight = std::sqrt(character->field110[0]) * std::bit_cast<float>(character->fieldF8[0]);
    if (!(weight >= 0.003f))
        return;
    Vector3 source{};
    const float alpha = ComputeShadowLightSource(position, source, strength, character->fieldEc[0]) * (weight * fade);
    if (!(alpha >= 0.003f))
        return;
    ScriptedMatrixFrame frame;
    ApplyDirectionAlignedShadowProjection({position.x - source.x, position.y - source.y, position.z - source.z}, groundY, 0.44435555f, 0.55564445f);
    ApplyShadowPlacement(position, rotation, scale);
    if (GetPreviousActiveLight() != 1)
        SetLegacyStencilReference(static_cast<std::uint32_t>(alpha * 255.0f));
    character->prepareForDraw(root, root->nodes.data(), alpha);
}

void ProjectFixedCharacterShadow(
    Character *character, const Vector3 &position, const Vector3i &rotation, const Vector3 &scale, float groundY, std::size_t meshIndex, float strength) {
    if (g_characterShadowDisabled || !character || strength == 0.0f || !IsCharacterDrawControlEnabled(character) || !(position.y - groundY <= 2.2f))
        return;
    const float fade = GetCharacterShadowDistanceFade(position, *character);
    if (!(fade >= 0.003f))
        return;
    PrepareCharacterShadowLighting();
    for (std::size_t slot = 0; slot < 6 && character->fieldEc[slot] >= 0; ++slot) {
        const float weight = std::sqrt(character->field110[slot]) * std::bit_cast<float>(character->fieldF8[slot]);
        if (!(weight >= 0.003f))
            continue;
        Vector3 source{};
        const float alpha = ComputeShadowLightSource(position, source, strength, character->fieldEc[slot]) * (weight * fade);
        if (!(alpha >= 0.003f))
            continue;
        ScriptedMatrixFrame frame;
        ApplyDirectionAlignedShadowProjection({position.x - source.x, position.y - source.y, position.z - source.z}, groundY, 0.44435555f, 0.55564445f);
        ApplyShadowPlacement(position, rotation, scale);
        if (alpha < 1.0f && GetPreviousActiveLight() != 1)
            SetLegacyStencilReference(static_cast<std::uint32_t>(alpha * 255.0f));
        DrawFixedShadowMesh(meshIndex, alpha >= 1.0f ? 1.0f : alpha);
    }
}

void DrawShenmueEntity(void *parameter) {
    Character *const character = static_cast<Character *>(parameter);
    if (!character)
        return;
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    const std::int32_t previousLight = GetPreviousActiveLight();
    if (model) {
        MotionWork *const work = FindMotionWork(character);
        if (!work || (work->runtime.seqFlags & MSF_SearchHand))
            return;
        const Vector3 position = work->runtime.drawOverrideData ? GetCharacterWorldPosition(character) : work->runtime.rootPosition;
        float ground = charaGetShadowY(character);
        if (!(character->flags & CF_UseStoredShadowY)) {
            Vector3 probe = position;
            probe.y += 0.5f;
            ground = shenFindIntersectionWithScene(probe, position.z);
            if (ground < -1000.0f || ground > position.y + 0.1f)
                ground = position.y;
        }
        if (work->runtime.drawOverrideData) {
            DrawCharacterShadowSprite(*character, position, ground);
            return;
        }
        if ((g_loadingFlag | previousMotionShadowLoadingFlag) & 0x102U)
            BeginLegacyCharacterShadowFrame();
        if (previousLight == 1)
            SetLegacyColourWriteMask(0);
        switch (character->type) {
        case 1:
        case 10:
            DrawMotionCharacterShadow(
                *character, position, ground, *model, static_cast<MT7Model *>(character->shadowModel), character->type == 1 ? 1.0f : -1.0f);
            break;
        case 2:
        case 9: {
            Vector3 offset{}, scale{}, fixedPosition = position;
            GetCharacterShadowOffset(character, &offset);
            GetCharacterShadowScale(character, &scale);
            fixedPosition.x += offset.x;
            fixedPosition.y += offset.y;
            fixedPosition.z += offset.z;
            const Vector3i rotation{0, GetCharacterWorldRotation(character).y + GetCharacterShadowYaw(character), 0};
            ProjectFixedCharacterShadow(character, fixedPosition, rotation, scale, ground, 1, character->type == 9 ? 1.0f : -1.0f);
            break;
        }
        case 4:
        case 5:
            DrawObject_AssignShadow(character, position, ground);
            break;
        case 6:
            DrawMotionCharacterShadow(*character, position, ground, *model, static_cast<MT7Model *>(character->shadowModel), 0.5f);
            break;
        case 7:
        case 8:
        case 11:
            DrawMotionCharacterShadow(*character, position, ground, *model, const_cast<MT7Model *>(GetSHDS()),
                character->type == 8      ? 1.0f :
                    character->type == 11 ? -2.0f :
                                            -1.0f);
            break;
        default:
            if (previousLight == 1)
                DrawObject_AssignShadow(character, position, ground);
            else
                DrawMotionCharacterShadow(*character, position, ground, *model, const_cast<MT7Model *>(GetSHDS()), -1.0f);
            break;
        }
        RestoreEngineFlag(67);
        if ((g_loadingFlag | previousMotionShadowLoadingFlag) & 0x102U)
            EndLegacyCharacterShadowFrame();
    }
    previousMotionShadowLoadingFlag = g_loadingFlag;
}

void DrawObject_AssignShadow(Character *character, const Vector3 &position, float groundY) {
    const MT7Model *model = character ? static_cast<const MT7Model *>(character->model) : nullptr;
    if (g_characterShadowDisabled || !character || !model || !IsCharacterDrawControlEnabled(character))
        return;
    const bool alternateLighting = GetPreviousActiveLight() != 1;
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    const float dx = position.x - camera.x, dz = position.z - camera.z;
    const float distanceFade = GetShadowRange() * ((character->fieldE8 & 0x10U) ? 3.0f : 1.0f) - std::sqrt(dx * dx + dz * dz);
    const float fade = (distanceFade < 0.0f ? 0.0f : std::fmin(1.0f, distanceFade)) * (character->alpha * characterShadowOpacityScale);
    if (fade < 0.003)
        return;
    PrepareCharacterShadowLighting();
    Vector3 probe = position;
    probe.y += 0.5f;
    flddPushIntersectionFlag(32);
    const float intersection = shenFindIntersectionWithScene(probe, position.z);
    std::array<std::int16_t, 6> lights{};
    lights.fill(-1);
    std::array<float, 6> weights{};
    float strongestWeight = 0.0f;
    std::int32_t strongest = -1;
    for (std::size_t slot = 0; slot < 6 && character->fieldEc[slot] >= 0; ++slot) {
        weights[slot] = std::sqrt(character->field110[slot]) * std::bit_cast<float>(character->fieldF8[slot]);
        if (!(weights[slot] < 0.003)) {
            lights[slot] = character->fieldEc[slot];
            if (weights[slot] > strongestWeight) {
                strongestWeight = weights[slot];
                strongest = static_cast<std::int32_t>(slot);
            }
        }
    }
    const bool strongestOnly = characterShadowStrongestOnly != 0;
    for (std::size_t slot = 0; slot < 6; ++slot) {
        if (lights[slot] >= 0 && (!strongestOnly || static_cast<std::int32_t>(slot) != strongest)) {
            if (!(static_cast<double>(intersection) > static_cast<double>(position.y) - 1.5) && characterUngroundedShadowMode == 0)
                DrawProjectedSharedCharacterShadow(*character, position, groundY, lights[slot], fade * weights[slot]);
            lights[slot] = -1;
        }
    }
    if (alternateLighting) {
        const bool suspended = SuspendLegacyCharacterShadowFrame();
        PerformCommonShaderPass(0);
        ResumeLegacyCharacterShadowFrame(suspended);
    }
    characterShadowRenderParameters[0] &= ~1;
    std::int32_t reference = static_cast<std::int32_t>((1.0 - fade * strongestWeight) * 255.0);
    if (reference > 254)
        return;
    reference = std::max(reference, 0);
    for (std::size_t slot = 0; slot < 6; ++slot) {
        if (lights[slot] < 0)
            continue;
        const LightRuntimeEntry *light = GetLightRuntimeEntry(lights[slot]);
        if (!light)
            continue;
        const std::uint8_t kind = light->indexes[1] & 0xf7U;
        Vector3 direction{};
        float vertical{};
        if (kind == 2) {
            direction = {light->values2c[0], light->values2c[1], light->value34};
            SetProjectedShadowDirection(&direction, GetTexUVW(1));
            NormalizeVector3(&direction);
            vertical = direction.y;
            if (!(vertical < 0.3))
                continue;
            const Vector3 transformed = TransformVector(direction, workTransformMatrix);
            characterShadowRenderParameters[0] &= ~1;
            characterShadowLightVector = {-transformed.x, -transformed.y, -transformed.z};
        } else if (kind == 3 || kind == 4) {
            Vector3 source = light->vector20;
            if (kind == 3) {
                Vector3 root{};
                GetLightRootPosition(&root);
                source.x += root.x;
                source.y += root.y;
                source.z += root.z;
            }
            Vector3 towardLight{source.x - position.x, source.y - position.y, source.z - position.z};
            if (!(towardLight.y >= -0.3))
                continue;
            if (towardLight.y < 1.0f) {
                towardLight.y = 1.0f;
                source.y = position.y + 1.0f;
            }
            direction = {-towardLight.x, -towardLight.y, -towardLight.z};
            SetProjectedShadowDirection(&direction, GetTexUVW(1));
            NormalizeVector3(&towardLight);
            vertical = -towardLight.y;
            if (characterShadowPointLightMode) {
                characterShadowRenderParameters[0] |= 1;
                characterShadowLightVector = TransformPoint(source, workTransformMatrix);
            } else
                characterShadowLightVector = TransformVector(towardLight, workTransformMatrix);
        } else
            continue;
        if (characterShadowStencilReference >= 0)
            reference = characterShadowStencilReference;
        else
            characterShadowStencilReference = reference;
        characterShadowDrawReference = reference;
        ScriptedMatrixFrame frame;
        if (vertical >= 0.0f) {
            characterShadowVerticalExtrusion = 5.4000001f;
            Matrix4x4::preTranslate(0.0f, -0.69999999f, 0.0f);
        } else
            characterShadowVerticalExtrusion = std::fmin(5.4000001f, -1.8f / vertical);
        const bool suspended = SuspendLegacyCharacterShadowFrame();
        SetLegacyProjectedShadowStencilPartition(1);
        const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        const shen_name name = control ? scenGetCharaName(control->scenarioTask, character) : shen_name{};
        const MT7Model *shadow = static_cast<const MT7Model *>(character->data2);
        bool custom = false;
        if (name.name == SHEN_NAME(RCKO) && alternateLighting) {
            SetLegacyProjectedShadowStencilPartition(0);
            ResumeLegacyCharacterShadowFrame(suspended);
            continue;
        }
        if (name.name == SHEN_NAME(SIN_))
            shadow = GetShadowDataModel(2);
        else if (name.name == SHEN_NAME(RYO_))
            shadow = GetShadowDataModel(1);
        else if (!shadow)
            shadow = GetShadowDataModel(0);
        else
            custom = true;
        if (shadow && !shadow->nodes.empty()) {
            ScriptedMatrixFrame hierarchyFrame;
            if (custom) {
                const Vector3 worldPosition = GetCharacterWorldPosition(character);
                const Vector3i rotation = GetCharacterWorldRotation(character);
                Matrix4x4::preTranslate(worldPosition.x, worldPosition.y, worldPosition.z);
                if (rotation.z)
                    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
                if (rotation.y)
                    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
                if (rotation.x)
                    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
            }
            if (BeginLegacyProjectedShadowPass()) {
                const Matrix4x4 base = workTransformMatrix;
                DrawCharacterStencilHierarchy(*model, *shadow, 0, base);
                EndLegacyProjectedShadowPass();
                if (alternateLighting)
                    ResolveGlobalShadowLighting(1.0f - static_cast<float>(reference) / 255.0f);
            }
        }
        SetLegacyProjectedShadowStencilPartition(0);
        ResumeLegacyCharacterShadowFrame(suspended);
    }
}

namespace {

Vector3 temporaryCommandStreamOffset{};
Matrix4x4 objectCommandPlacementTransform{};
// DrawObject latches these once per frame; a PAUS replay of its callbacks reuses the last values.
std::uint32_t g_currentFieldPointCount{};
std::int32_t g_currentDrawTier{};

class ObjectPassMatrixState {
  public:
    ObjectPassMatrixState()
        : selected(g_CurrentMatrixStackIdx), incoming(workTransformMatrix), incomingCell(*g_matrixStack.top.stack), depthModel(GetFieldDepthModelTransform()),
          incomingCellAddress(g_matrixStack.top.stack) {
        SetActiveStack(MatrixStackType::DepthPrepass);
        depthTransform = workTransformMatrix;
        depthCell = *g_matrixStack.top.stack;
        depthCellAddress = g_matrixStack.top.stack;
    }
    ~ObjectPassMatrixState() {
        Matrix4x4::SetActiveTransform(&depthTransform);
        *depthCellAddress = depthCell;
        if (g_CurrentMatrixStackIdx != selected)
            SetActiveStack(static_cast<MatrixStackType>(selected));
        Matrix4x4::SetActiveTransform(&incoming);
        *incomingCellAddress = incomingCell;
        GetFieldDepthModelTransform() = depthModel;
    }

  private:
    std::uint16_t selected;
    Matrix4x4 incoming, incomingCell, depthModel, depthTransform, depthCell;
    Matrix4x4 *incomingCellAddress;
    Matrix4x4 *depthCellAddress{};
};

bool IsObjectCommandVisible(const MapModelClipRecord &record, bool screenSpace) {
    if (screenSpace)
        return camCtrlIsScreenSpaceVisible(record.centre, record.radius);
    switch (record.commandType) {
    case 0:
    case 4:
        return camCtrlIsSphereVisible(record.centre, record.radius);
    case 8:
        return camCtrlIsNearSphereVisible(record.centre, record.radius);
    case 12:
        return camCtrlIsFarSphereVisible(record.centre, record.radius);
    case 16:
        return camCtrlIsScreenSpaceVisible(record.centre, record.radius);
    default:
        return false;
    }
}

std::uint32_t ProcessObjectCommandRun(const MT7Model &model, std::span<MapModelClipRecord *const> records, const Matrix4x4 &baseTransform,
    LegacyModelShaderPath shaderPath, bool gridCull, bool screenSpace, const float *alpha) {
    std::uint32_t draws = 0;
    for (const MapModelClipRecord *record : records) {
        Matrix4x4::SetActiveTransform(&baseTransform);
        const MT7Node &node = *static_cast<const MT7Node *>(record->object);
        if (!node.xb01Offset)
            continue;
        const bool offsetHash = gridCull && !alpha;
        const auto hashNode = [&]() {
            ScriptedMatrixFrame frame(offsetHash);
            if (offsetHash) {
                Matrix4x4::SetPrePassToTempTransform();
                Matrix4x4::preTranslate(temporaryCommandStreamOffset.x, temporaryCommandStreamOffset.y, temporaryCommandStreamOffset.z);
            }
            return alpha ? d3t::HashModelWithAlpha(&node, *alpha, GetLegacyPerspectiveProjection()) : d3t::HashModel(&node, GetLegacyPerspectiveProjection());
        };
        if (!d3t::GetShouldHashFlag() && hashNode()) {
            d3t::IncrementHashedModelCount(1, 0);
            continue;
        }
        // ACPD names one mesh, not a hierarchy root: neither children nor siblings are submitted here.
        ApplyMT7NodeTransform(node);
        const bool visible = IsObjectCommandVisible(*record, screenSpace);
        bool culled = !visible;
        if (alpha && !screenSpace) {

            culled = !visible && record->commandType != 4 && !camCtrlIsModelDepthVisible(record->centre, node, record->radius);
        } else if (gridCull && !screenSpace && record->commandType != 4 && !camCtrlIsModelDepthVisible(record->centre, node, record->radius)) {
            culled = true;
        }
        if (!culled) {
            SetLegacyModelScale(record->modelScale);
            if (!d3t::GetShouldHashFlag() || !hashNode()) {
                const MT7Node &meshNode = node.borrowedMesh ? *node.borrowedMesh : node;
                const std::byte *mesh = meshNode.runtimeMesh ? meshNode.runtimeMesh : model.source.data() + node.xb01Offset;
                std::uint32_t flags{};
                std::memcpy(&flags, mesh + 4, sizeof(flags));
                ScriptedMatrixFrame alternateFrame((flags & 0x40U) != 0);
                if (flags & 0x40U)
                    Matrix4x4::SetActiveTransform(&objectCommandPlacementTransform);
                if (node.commandOverride && node.commandOverride->resources) {
                    const LegacyNodeCommandOverride &command = *node.commandOverride;
                    if (alpha)
                        draws += shaderPath == LegacyModelShaderPath::Lambert ?
                            SetModelFlagDataWithAlpha(command.bytes, command.xb01Offset, *command.resources, *alpha) :
                            ReadModelDataWithAlpha(command.bytes, command.xb01Offset, *command.resources, *alpha);
                    else
                        draws += shaderPath == LegacyModelShaderPath::Lambert ?
                            ModelFlagDispatch_SetModelFlagDataOpaque(command.bytes, command.xb01Offset, *command.resources) :
                            ModelFlagDispatch_ReadModelDataOpaque(command.bytes, command.xb01Offset, *command.resources);
                } else if (!mtIsMeshTypeOverridden(model, node)) {
#if defined(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS) && SHENMUE_ENHANCEMENT_S2_MAP_SEAMS
                    const LegacyDrawResources &resources = GetCorrectedMapSeamResources(model, node, meshNode.drawResources);
#else
                    const LegacyDrawResources &resources = meshNode.drawResources;
#endif
                    if (alpha)
                        draws += shaderPath == LegacyModelShaderPath::Lambert ?
                            SetModelFlagDataWithAlpha(model.source, node.xb01Offset, resources, *alpha) :
                            ReadModelDataWithAlpha(model.source, node.xb01Offset, resources, *alpha);
                    else
                        draws += shaderPath == LegacyModelShaderPath::Lambert ?
                            ModelFlagDispatch_SetModelFlagDataOpaque(model.source, node.xb01Offset, resources) :
                            ModelFlagDispatch_ReadModelDataOpaque(model.source, node.xb01Offset, resources);
                }
            }
        }
        d3t::IncrementHashedModelCount(1, culled ? 1 : 0);
    }
    Matrix4x4::SetActiveTransform(&baseTransform);
    return draws;
}

std::uint32_t mapObjectClipRecordIndex{};
const MapModelClipRecord *mapObjectClipRecords{};
const MapModelClipRecord *activeMapObjectClipRecord{};

std::uint32_t HashOrDrawMapObjectHierarchy(const MapModelRuntimeSlot &slot, const MT7Node *node, LegacyModelShaderPath shaderPath, bool gridCull) {
    std::uint32_t draws = 0;
    while (node) {
        const bool siblingFrame = node->runtimeSibling != nullptr;
        const Matrix4x4 incoming = workTransformMatrix;
        const bool pushed = siblingFrame && Matrix4x4_Push();
        ApplyMT7NodeTransform(*node);
        if (node->xb01Offset) {
            const MapModelClipRecord &record = mapObjectClipRecords[mapObjectClipRecordIndex];
            activeMapObjectClipRecord = &record;
            if (record.commandType != 240 && record.commandType != 244 && record.commandType != 255 && IsObjectCommandVisible(record, false) &&
                (!gridCull || camCtrlIsModelDepthVisible(record.centre, *node, record.radius))) {
                SetLegacyModelScale(record.modelScale);
                if (!d3t::HashModel(node, GetLegacyPerspectiveProjection())) {
                    const MT7Node &meshNode = node->borrowedMesh ? *node->borrowedMesh : *node;
                    const std::byte *const mesh = meshNode.runtimeMesh ? meshNode.runtimeMesh : slot.reader->source.data() + node->xb01Offset;
                    std::uint32_t flags{};
                    std::memcpy(&flags, mesh + 4, sizeof(flags));
                    ScriptedMatrixFrame alternateFrame((flags & 0x40U) != 0);
                    if (flags & 0x40U)
                        Matrix4x4::SetActiveTransform(&objectCommandPlacementTransform);
                    if (node->commandOverride && node->commandOverride->resources) {
                        const LegacyNodeCommandOverride &command = *node->commandOverride;
                        draws += shaderPath == LegacyModelShaderPath::Lambert ?
                            ModelFlagDispatch_SetModelFlagDataOpaque(command.bytes, command.xb01Offset, *command.resources) :
                            ModelFlagDispatch_ReadModelDataOpaque(command.bytes, command.xb01Offset, *command.resources);
                    } else {
#if defined(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS) && SHENMUE_ENHANCEMENT_S2_MAP_SEAMS
                        const LegacyDrawResources &resources = GetCorrectedMapSeamResources(*slot.reader, *node, meshNode.drawResources);
#else
                        const LegacyDrawResources &resources = meshNode.drawResources;
#endif
                        draws += shaderPath == LegacyModelShaderPath::Lambert ?
                            ModelFlagDispatch_SetModelFlagDataOpaque(slot.reader->source, node->xb01Offset, resources) :
                            ModelFlagDispatch_ReadModelDataOpaque(slot.reader->source, node->xb01Offset, resources);
                    }
                }
            }
            ++mapObjectClipRecordIndex;
        }
        draws += HashOrDrawMapObjectHierarchy(slot, node->runtimeChild, shaderPath, gridCull);
        if (siblingFrame) {
            if (pushed)
                Matrix4x4::Pop(1);
            else
                Matrix4x4::SetActiveTransform(&incoming);
        }
        node = node->runtimeSibling;
    }
    return draws;
}

std::uint32_t HashOrDrawPlacedMapObjectHierarchy(const MapModelRuntimeSlot &slot, LegacyModelShaderPath shaderPath) {
    if (!slot.reader || slot.reader->nodes.empty())
        return 0;
    mapObjectClipRecordIndex = 0;
    mapObjectClipRecords = slot.clipRecords;
    ScriptedMatrixFrame frame;
    Matrix4x4::preTranslate(slot.position.x, slot.position.y, slot.position.z);
    if (slot.rotationY)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(slot.rotationY));
    Matrix4x4::GetTransformMatrix(&objectCommandPlacementTransform);
    return HashOrDrawMapObjectHierarchy(slot, &slot.reader->nodes.front(), shaderPath, g_currentFieldPointCount && slot.gridCullingEnabled);
}

MapCellOffset PrepareObjectCommandPlacement(const Vector3 &position, std::uint32_t yaw, const Vector3 &cameraPosition) {
    if (position.x == 0.0f && position.y == 0.0f && position.z == 0.0f && yaw == 0) {
        GetFieldDepthModelTransform() = GetFieldDepthTransform();
        return GetFieldDepthCell(cameraPosition.x, cameraPosition.z);
    }
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float cosine = cos_fixed16(static_cast<std::uint16_t>(yaw));
    const float x = cameraPosition.x - position.x;
    const float z = cameraPosition.z - position.z;
    const MapCellOffset cell = GetFieldDepthCell(cosine * x + sine * z, cosine * z - sine * x);
    Matrix4x4 placement{};
    {
        ScriptedMatrixFrame frame;
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
        Matrix4x4::GetTransformMatrix(&placement);
    }
    {
        ScriptedMatrixFrame frame;
        Matrix4x4::SetActiveTransform(&GetFieldDepthTransform());
        Matrix4x4::MultiplyWithPrePassMatrix(&placement);
        Matrix4x4::GetTransformMatrix(&GetFieldDepthModelTransform());
    }
    Matrix4x4::MultiplyWithPrePassMatrix(&placement);
    return cell;
}

} // namespace

void SetTempCommandStreamOffset(const Vector3 *offset) {
    temporaryCommandStreamOffset = *offset;
}
const Vector3 &GetTempCommandStreamOffset() {
    return temporaryCommandStreamOffset;
}

std::uint32_t ProcessObjectRenderCommandStream(const MapModelRuntimeSlot &slot, std::span<const MapCellOffset> visibleCells, MapCellOffset cameraCell,
    LegacyModelShaderPath shaderPath, std::int32_t mode) {
    const Matrix4x4 baseTransform = workTransformMatrix;
    if (mode == 2 && (!IsGlobalShadowLightingEnabled() || GetGlobalShadowOpacity() < 0.01f || !BeginMainMapProjectedPlacementQueue()))
        return 0;
    std::uint32_t draws = 0;
    std::uint32_t firstCell = 0;
    if (mode != 2 && slot.commandCellCount && slot.commandCells[0].x == 0x4fff) {
        const MapModelCommandCell &cell = slot.commandCells[0];
        draws +=
            ProcessObjectCommandRun(*slot.reader, {slot.commandRouting, static_cast<std::size_t>(cell.count)}, baseTransform, shaderPath, false, true, nullptr);
        firstCell = 1;
    }
    for (std::uint32_t index = firstCell; index < slot.commandCellCount; ++index) {
        const MapModelCommandCell &cell = slot.commandCells[index];
        for (const MapCellOffset &visible : visibleCells) {
            if (static_cast<std::int32_t>(cell.x) - cameraCell.x != visible.x || static_cast<std::int16_t>(cell.z - cameraCell.z) != visible.z)
                continue;
            const std::span<MapModelClipRecord *const> records{slot.commandRouting + cell.firstRecord, static_cast<std::size_t>(cell.count)};
            if (mode == 2)
                QueueMainMapProjectedPlacements(records);
            else
                draws += ProcessObjectCommandRun(*slot.reader, records, baseTransform, shaderPath, mode == 1, false, nullptr);
            break;
        }
    }
    return draws;
}

std::uint32_t ProcessAlphaObjectRenderCommandStream(
    const MapModelRuntimeSlot &slot, std::span<const MapCellOffset> visibleCells, MapCellOffset cameraCell, LegacyModelShaderPath shaderPath, float alpha) {
    const Matrix4x4 baseTransform = workTransformMatrix;
    std::uint32_t draws = 0;
    std::uint32_t firstCell = 0;
    if (slot.commandCellCount && slot.commandCells[0].x == 0x4fff) {
        const MapModelCommandCell &cell = slot.commandCells[0];
        draws +=
            ProcessObjectCommandRun(*slot.reader, {slot.commandRouting, static_cast<std::size_t>(cell.count)}, baseTransform, shaderPath, false, true, &alpha);
        firstCell = 1;
    }
    for (std::uint32_t index = firstCell; index < slot.commandCellCount; ++index) {
        const MapModelCommandCell &cell = slot.commandCells[index];
        for (const MapCellOffset &visible : visibleCells) {
            if (static_cast<std::int32_t>(cell.x) - cameraCell.x != visible.x || static_cast<std::int32_t>(cell.z) - cameraCell.z != visible.z)
                continue;
            draws += ProcessObjectCommandRun(
                *slot.reader, {slot.commandRouting + cell.firstRecord, static_cast<std::size_t>(cell.count)}, baseTransform, shaderPath, false, false, &alpha);
            break;
        }
    }
    return draws;
}

Vector3 g_d3tLightDirection{};

namespace {

std::array<std::uint32_t, 9> skyDomeSources{};
std::array<std::uint32_t, 9> skyDomeProbePlacements{};
constexpr std::array<const char *, 9> kSkyDomeModels{"HD_skyDome.fbx", "HD_skyHorizon0.fbx", "HD_skyHorizon1.fbx", "HD_skyHorizon2.fbx", "HD_skyLayer0.fbx",
    "HD_skyLayer1.fbx", "HD_Corona.fbx", "HD_Stars0.fbx", "HD_Moon.fbx"};
}

// @verified - p
void LoadSkyDomeModels(const char *directory) {
    char path[272];
    for (std::size_t index = 0; index < kSkyDomeModels.size(); ++index) {
        std::snprintf(path, sizeof(path), "%s/%s", directory, kSkyDomeModels[index]);
        d3t::RegisterObjectSource(&skyDomeSources[index], path, 0x1a0);
        d3t::RegisterObjectPlacement(&skyDomeSources[index], &skyDomeSources[index], 2);
        d3t::RegisterObjectPlacement(&skyDomeSources[index], &skyDomeProbePlacements[index], 4);
    }
}

void ReleaseSkyDomeModels(const std::uint32_t *handles) {
    for (std::size_t index = 0; index < kSkyDomeModels.size(); ++index) {
        if (const std::uint32_t model = d3t::GetObjectSourceRenderHandle(&handles[index]))
            d3t::ReleaseModelHandle(model);
    }
}

// @verified - p
void SetupRenderBufferHandles() {
    ReleaseSkyDomeModels(skyDomeSources.data());
    ReleaseSkyDomeModels(skyDomeProbePlacements.data());
    d3t::ReleaseModelCache(g_skyModelCache);
    d3t::ReleaseModelCache(g_skyProbeModelCache);
    g_skyModelCache = 0;
    g_skyProbeModelCache = 0;
}

// @verified - p
void SkyDomeProbe(const char *mapDirectory) {
    char air[272];
    char dome[272];
    std::snprintf(air, sizeof(air), "%s/air", mapDirectory);
    std::snprintf(dome, sizeof(dome), "/%s/HD_skyDome.fbx", air);
    LoadSkyDomeModels(d3t::AuthoredAssetExists(dome) ? air : "/air");
    g_skyModelCache = d3t::AllocateModelCache("Sky");
    g_skyProbeModelCache = d3t::AllocateModelCache("SkyProbe");
    g_d3tLightDirection = {0.0f, -1.0f, 0.0f};
}

std::uint32_t DrawObjectWorldPass() {
    if (!d3t::BindSceneFrameRenderTarget())
        return 0;

    ObjectPassMatrixState passState;
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    Vector3 cameraPosition{};
    camCtrlGetNextCamPos(&cameraPosition);
    std::uint32_t draws = 0;
    for (const MapModelRuntimeSlot &slot : slots) {
        if (!slot.enabled || !slot.reader)
            continue;
        {
            ScriptedMatrixFrame hashFrame;
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::preTranslate(slot.position.x, slot.position.y, slot.position.z);
            if (slot.alpha == 1.0f ? d3t::HashModel(GetMT7ReaderData(slot.reader), GetLegacyPerspectiveProjection()) :
                                     d3t::HashModelWithAlpha(GetMT7ReaderData(slot.reader), slot.alpha, GetLegacyPerspectiveProjection()))
                continue;
        }
        const Matrix4x4 savedTransform = workTransformMatrix;
        const bool pushed = g_matrixStack.top.info.index + 1 < g_matrixStack.top.info.capacity ? Matrix4x4_Push() : false;
        const std::int32_t previousVertexColourType = GetLegacyVertexColourType();
        SetLegacyVertexColourType(slot.vertexShaderType ? 2 : 1);
        TemporaryMapWeatherLightingState weatherLighting{};
        std::array<bool, 16> previousVertexColourStates{};
        bool weatherLightingActive = false;
        if (slot.weatherLightingOverride) {
            for (std::uint32_t index = 0; index < previousVertexColourStates.size(); ++index) {
                previousVertexColourStates[index] = GetLegacyVertexColourStateEnabled(index);
            }
            weatherLightingActive = BeginTemporaryMapWeatherLighting(weatherLighting);
            if (weatherLightingActive) {
                for (std::uint32_t index = 0; index < previousVertexColourStates.size(); ++index) {
                    SetLegacyVertexColourStateEnabled(index, false);
                }
                SetLegacyVertexColourStateEnabled(0, true);
                UpdateLegacyLightTransforms();
            }
        }
        const LegacyModelShaderPath shaderPath =
            (GetCurrentActiveLight() == 1 || slot.forceLambertShader) ? LegacyModelShaderPath::Lambert : LegacyModelShaderPath::VertexColour;
        const bool mirrored = g_characterMirroredTransformEnabled && slot.alternateTraversalEnabled;
        if (!slot.usesCommandRouting || mirrored) {
            GetFieldDepthModelTransform() = GetFieldDepthTransform();
            draws += HashOrDrawPlacedMapObjectHierarchy(slot, shaderPath);
            if (mirrored) {
                if (pushed) {
                    Matrix4x4::Pop(1);
                    Matrix4x4_Push();
                } else
                    Matrix4x4::SetActiveTransform(&savedTransform);
                SetLegacyMirroredDrawMode(1);
                Matrix4x4::MultiplyWithPrePassMatrix(GetCharacterMirrorMatrix());
                draws += HashOrDrawPlacedMapObjectHierarchy(slot, shaderPath);
                SetLegacyMirroredDrawMode(0);
            }
        } else {
            const MapCellOffset cell = PrepareObjectCommandPlacement(slot.position, slot.rotationY, cameraPosition);
            Matrix4x4::GetTransformMatrix(&objectCommandPlacementTransform);
            const std::span<const MapCellOffset> visibleCells = camCtrlGetMapVisibleCells(slot.rotationY, g_currentDrawTier);
            if (&slot == &slots[32]) {
                draws += ProcessObjectRenderCommandStream(slot, visibleCells, cell, shaderPath, 2);
            } else if (slot.alpha == 1.0f) {
                draws += ProcessObjectRenderCommandStream(slot, visibleCells, cell, shaderPath, g_currentFieldPointCount && slot.gridCullingEnabled ? 1 : 0);
            } else if (std::abs(slot.alpha) >= 0.00000011920929f) {
                draws += ProcessAlphaObjectRenderCommandStream(slot, visibleCells, cell, shaderPath, slot.alpha);
            }
        }
        if (pushed) {
            Matrix4x4::Pop(1);
        } else {
            workTransformMatrix = savedTransform;
            *g_matrixStack.top.stack = savedTransform;
        }
        if (weatherLightingActive) {
            EndTemporaryMapWeatherLighting(weatherLighting);
            for (std::uint32_t index = 0; index < previousVertexColourStates.size(); ++index) {
                SetLegacyVertexColourStateEnabled(index, previousVertexColourStates[index]);
            }
            UpdateLegacyLightTransforms();
        }
        SetLegacyVertexColourType(previousVertexColourType);
    }

    const std::uint32_t map = g_currentMap.name;
    if (map == SHEN_NAME(0504)) {
        draws += DrawSurfaceWaveModel();
    } else if (map == SHEN_NAME(0055) || map == SHEN_NAME(0059) || map == SHEN_NAME(AB00) || map == SHEN_NAME(AK00) || map == SHEN_NAME(AR01)) {
        draws += DrawWaterTextureSet();
    } else if (map == SHEN_NAME(AR02)) {
        draws += DrawWaterTextureSet();
        draws += DrawSpringTextureNode();
    } else if (map == SHEN_NAME(CWON) || map == SHEN_NAME(0004)) {
        draws += DrawWaterReflectionSet();
        draws += DrawSpringTextureNode();
    } else if (map == SHEN_NAME(Q200)) {
        draws += DrawSpringTextureNode();
    }
    return draws;
}

std::uint32_t DrawMapLightingPlacementWorldPass() {
    if (!d3t::BindSceneFrameRenderTarget())
        return 0;
    ObjectPassMatrixState passState;
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    Vector3 cameraPosition{};
    camCtrlGetNextCamPos(&cameraPosition);
    std::uint32_t draws = 0;
    for (std::int32_t index = 0; index < 33; ++index) {
        const std::uint32_t mapSlot = GetMagicRoomFlag(index);
        if (mapSlot == 0x7fff || !GetMapLightingPlacementEnabled(index) || !slots[mapSlot].reader)
            continue;
        Vector3 position{};
        GetMapLightingPlacementPosition(&position, index);
        {
            ScriptedMatrixFrame frame;
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            if (d3t::HashModel(GetMapLightingPlacement(index), GetLegacyPerspectiveProjection()))
                continue;
        }
        const MapModelRuntimeSlot &slot = slots[mapSlot];
        const std::int32_t previousColourType = GetLegacyVertexColourType();
        SetLegacyVertexColourType(slot.vertexShaderType ? 2 : 1);
        {
            ScriptedMatrixFrame frame;
            const LegacyModelShaderPath shaderPath =
                (GetPreviousActiveLight() == 1 || slot.forceLambertShader) ? LegacyModelShaderPath::Lambert : LegacyModelShaderPath::VertexColour;
            const std::uint32_t yaw = GetMapLightingPlacementRotation(index);
            const MapCellOffset cell = PrepareObjectCommandPlacement(position, yaw, cameraPosition);
            SetTempCommandStreamOffset(&position);
            Matrix4x4::GetTransformMatrix(&objectCommandPlacementTransform);
            draws += ProcessObjectRenderCommandStream(slot, camCtrlGetMapVisibleCells(yaw, g_currentDrawTier), cell, shaderPath,
                g_currentFieldPointCount && GetMapLightingPlacementActive(index) ? 1 : 0);
        }
        SetLegacyVertexColourType(previousColourType);
        const Vector3 zero{};
        SetTempCommandStreamOffset(&zero);
    }
    return draws;
}

namespace {

#pragma pack(push, 1)
struct ObjectDrawTaskParameters {
    std::uint64_t reserved00;
    std::int16_t submittedOpaque;
    std::int16_t submittedTransparent;
    std::uint32_t reserved0c;
};
#pragma pack(pop)

static_assert(sizeof(ObjectDrawTaskParameters) == 0x10);
static_assert(offsetof(ObjectDrawTaskParameters, submittedOpaque) == 0x08);
static_assert(offsetof(ObjectDrawTaskParameters, submittedTransparent) == 0x0a);

struct AirDrawTaskParameters {
    std::uint64_t reserved00;
    std::int32_t mode;
    std::uint32_t reserved0c;
};

static_assert(sizeof(AirDrawTaskParameters) == 0x10);

enum class AirModelSlot : std::size_t {
    DomeDay,
    DomeNight,
    CloudA,
    CloudB,
    Moon,
    Count,
};

constexpr std::array<std::uint32_t, static_cast<std::size_t>(AirModelSlot::Count)> airModelTags{
    SHEN_NAME(DOM0),
    SHEN_NAME(DOM1),
    SHEN_NAME(AIR0),
    SHEN_NAME(AIR1),
    SHEN_NAME(MOON),
};
constexpr std::uint32_t xb01Signature = SHEN_NAME(XB01);
constexpr std::uint32_t txt7Signature = SHEN_NAME(TXT7);
constexpr std::uint32_t texnSignature = SHEN_NAME(TEXN);
constexpr std::uint32_t gbixSignature = SHEN_NAME(GBIX);
constexpr std::uint32_t pvrtSignature = SHEN_NAME(PVRT);
constexpr std::uint32_t ddsSignature = make_shen_name('D', 'D', 'S', ' ');
constexpr std::uint32_t defaultDaytimeSkyColour = 0xffacecffU;
constexpr float skyDepth = 0.0000001f;
constexpr float cloudScrollRate = 0.00050000002f;
constexpr std::array<std::int32_t, 7> cloudTextureIndices{
    0, 5, 4, 3, 1, 2, 2};
constexpr std::array<float, 7> cloudBlendMaxValues{
    1.0f, 1.0f, 1.0f, 1.0f, 0.4f, 1.0f, 1.0f};
constexpr std::array<float, 9> cloudGradientWeights{
    0.0f, 1.0f, 1.5f, 2.0f, 5.0f, 7.0f, 7.0f, 10.0f, 10.0f};
constexpr std::array<float, 5> domeGradientWeights{0.0f, 1.0f, 2.0f, 3.0f, 10.0f};



std::array<std::uint8_t, 96> airCloudVertexLevels{
    8, 8, 8, 7, 8, 6, 8, 4, 8, 4, 8, 4, 8, 5, 8, 7, 8, 8, 8, 5, 4, 3, 2, 3, 4, 5, 8, 8, 4, 3, 2, 1, 2, 3, 4, 8, 8, 4, 2, 1, 0,
    1, 2, 4, 8, 8, 4, 3, 2, 1, 2, 3, 4, 8, 8, 5, 4, 3, 2, 3, 4, 5, 8, 8, 7, 5, 4, 4, 4, 5, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
std::array<std::uint8_t, 72> airDomeVertexLevels{
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 3, 2, 3, 2, 3, 4, 4, 4, 4, 4, 3, 3, 2, 2, 2, 3, 4, 1, 2, 2, 1,
    2, 2, 1, 2, 1, 2, 3, 2, 3, 3, 4, 4, 4, 1, 2, 2, 1, 2, 4, 4, 3, 3, 2, 2, 3, 4, 2, 1};



constexpr std::size_t planetLayerParameterStrideFloats = 20;

struct AirPlanetLayerState {

    std::uint32_t colour0{0xffffffffU};
    std::uint32_t colour1{0xffffffffU};
    std::array<std::uint32_t, 9> levelColours{~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U, ~0U};
    Vector2 rate{cloudScrollRate, cloudScrollRate};
    Vector3 offset{0.0f, -115.0f, 0.0f};
};

struct AirPresentation {
    std::vector<std::byte> doomBytes;
    std::array<MT7Model, static_cast<std::size_t>(AirModelSlot::Count)> models;
    std::array<float, 2> cloudBlends{1.0f, 0.0f};
    std::array<float, 2> cloudBlendMax{1.0f, 1.0f};
    std::array<Vector2, 2> cloudUvOffsets{};
    AirPlanetDrawRecord planetDrawRecord{};
    std::array<AirPlanetLayerState, 2> planetLayers{};

    std::array<std::uint32_t, 5> skyLevelColours{~0U, ~0U, ~0U, ~0U, ~0U};
    std::array<float, 2> domeBlends{1.0f, 0.0f};
    bool geometryReady{};
};

struct AirWeatherPresentation {
    AirCloudTextureState clouds;
    std::uint32_t previousSecondsSince1980{};
    std::uint32_t secondsSince1980{};

    std::uint32_t skyColour{0xff204cf8U};
    std::uint32_t cloudColour{defaultDaytimeSkyColour};
};

AirPresentation airPresentation;
AirWeatherPresentation airWeatherPresentation;
std::int32_t airSkyEnabled{};

float airTimeOfDay{};
float airModelScale{};

template <typename Value> bool ReadAirValue(std::span<const std::byte> bytes, std::size_t offset, Value &value) {
    if (offset > bytes.size() || sizeof(value) > bytes.size() - offset) {
        return false;
    }
    std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return true;
}

bool LoadArchivedAirFile(const char *path, std::vector<std::byte> &bytes) {
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (!size || size > (std::numeric_limits<std::size_t>::max)()) {
        return false;
    }
    bytes.resize(static_cast<std::size_t>(size));
    if (ArchiveData_Read(path, 0, bytes.data(), size))
        return true;
    bytes.clear();
    return false;
}

std::span<std::byte> FindAirModelChunk(std::span<std::byte> bytes, std::uint32_t wantedTag) {
    std::size_t offset = 0;
    const std::span<const std::byte> source{bytes.data(), bytes.size()};
    while (offset + 8 <= bytes.size()) {
        std::uint32_t tag{};
        std::uint32_t size{};
        if (!ReadAirValue(source, offset, tag) || !ReadAirValue(source, offset + 4, size) || size < 8 || size > bytes.size() - offset) {
            return {};
        }
        if (tag == wantedTag)
            return bytes.subspan(offset + 8, size - 8);
        offset += size;
    }
    return {};
}


const MT7Node *FindAirGradientMesh(const MT7Model &model) {
    const MT7Node *mesh = nullptr;
    for (const MT7Node &node : model.nodes) {
        if (node.xb01Offset && node.meshVertexCount && node.meshVertexOffset + node.meshVertexCount <= model.vertices.size())
            mesh = &node;
    }
    return mesh;
}

int FloatCompare(const void *left, const void *right) {
    const float first = *static_cast<const float *>(left);
    const float second = *static_cast<const float *>(right);
    if (second > first)
        return 1;
    return (first <= second) - 1;
}


void AllocateCDCX(const MT7Model &model, std::span<std::uint8_t> levels, std::uint32_t capacity) {
    const MT7Node *const mesh = FindAirGradientMesh(model);
    if (!mesh)
        return;
    auto *const heights = static_cast<float *>(HLib::AllocateTaskData(4ULL * capacity, SHEN_NAME(CDCX)));
    if (!heights)
        return;
    const MT7Vertex *const vertices = model.vertices.data() + mesh->meshVertexOffset;
    std::uint32_t count = 0;
    for (std::uint32_t vertex = 0; vertex < mesh->meshVertexCount; ++vertex) {
        const float height = vertices[vertex].position.y;
        std::uint32_t level = 0;
        while (level < count && (height < heights[level] - 5.0f || heights[level] + 5.0f <= height))
            ++level;
        if (level == count && count < capacity)
            heights[count++] = height;
    }
    std::qsort(heights, count, sizeof(float), FloatCompare);
    for (std::uint32_t vertex = 0; vertex < mesh->meshVertexCount && vertex < levels.size(); ++vertex) {
        const float height = vertices[vertex].position.y;
        for (std::uint32_t level = 0; level < count; ++level) {
            if (height >= heights[level] - 5.0f && heights[level] + 5.0f > height) {
                levels[vertex] = static_cast<std::uint8_t>(level);
                break;
            }
        }
    }
    HLib::FreeTaskData(heights);
}


void ComputeCloudScaling(std::uint32_t colour0, std::uint32_t colour1, std::span<const float> weights, std::uint32_t *colours) {
    const float alpha = static_cast<float>(colour0 >> 24U);
    const float red = static_cast<float>((colour0 >> 16U) & 0xffU);
    const float green = static_cast<float>((colour0 >> 8U) & 0xffU);
    const float blue = static_cast<float>(colour0 & 0xffU);
    const float alphaStep = (static_cast<float>(colour1 >> 24U) - alpha) / 10.0f;
    const float redStep = (static_cast<float>((colour1 >> 16U) & 0xffU) - red) / 10.0f;
    const float greenStep = (static_cast<float>((colour1 >> 8U) & 0xffU) - green) / 10.0f;
    const float blueStep = (static_cast<float>(colour1 & 0xffU) - blue) / 10.0f;
    for (const float weight : weights) {
        const std::uint32_t rgb = ((static_cast<std::uint32_t>(static_cast<std::int32_t>(redStep * weight + red)) << 8U |
                                       static_cast<std::uint32_t>(static_cast<std::int32_t>(greenStep * weight + green)))
                                      << 8U) |
            static_cast<std::uint32_t>(static_cast<std::int32_t>(alphaStep * weight + alpha)) << 24U;
        *colours++ = rgb | static_cast<std::uint32_t>(static_cast<std::int32_t>(blueStep * weight + blue));
    }
}


void UploadAirDomeColours() {
    if (!airPresentation.geometryReady)
        return;
    const MT7Node *const mesh = FindAirGradientMesh(airPresentation.models[static_cast<std::size_t>(AirModelSlot::DomeDay)]);
    const LegacyVertexBufferBinding *const binding = mesh ? FindMT7MutableVertexBuffer(*mesh) : nullptr;
    if (!binding)
        return;
    const std::int32_t size = d3t::GetSizeForModelBuffer(binding->handle);
    auto *const destination = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, binding->handle, 1, 0));
    if (!destination)
        return;
    for (std::uint32_t vertex = 0; vertex < mesh->meshVertexCount && vertex < airDomeVertexLevels.size(); ++vertex) {
        const std::size_t offset = static_cast<std::size_t>(vertex) * mesh->meshVertexStride + 16;
        if (offset + 4 > static_cast<std::size_t>(size))
            break;
        std::memcpy(destination + offset, &airPresentation.skyLevelColours[airDomeVertexLevels[vertex]], 4);
    }
    d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, binding->handle);
}

void UpdateTimeOfDay() {
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    const float hours = static_cast<float>(dateTime.hour) * 10000.0f;
    const float minutes = (static_cast<float>(dateTime.minute) / 60.0f) * 10000.0f;
    airTimeOfDay = ((static_cast<float>(dateTime.second) / 60.0f) * 100.0f) * 1.666666f + (minutes + hours);
}

// The night dome fades in from 18:00 to 19:00 and out from 05:00 to 06:00.
void UpdateNightTime() {
    float &blend = airPresentation.domeBlends[1];
    if (airTimeOfDay < 190000.0f && airTimeOfDay > 180000.0f)
        blend = (airTimeOfDay - 180000.0f) / 10000.0f;
    else if (airTimeOfDay >= 50000.0f && airTimeOfDay < 60000.0f)
        blend = 1.0f - (airTimeOfDay - 50000.0f) / 10000.0f;
    else
        blend = airTimeOfDay >= 190000.0f || airTimeOfDay < 50000.0f ? 1.0f : 0.0f;
}

float AdvanceAirUvOffset(float &offset, float delta) {
    const float previous = offset;
    offset += delta;
    if (offset > 1.0f) {
        offset -= 1.0f;
    }
    if (offset < -1.0f) {
        offset += 1.0f;
    }
    return offset - previous;
}

void AdvanceAirLayerUv(std::size_t layerIndex) {
    Vector2 &offset = airPresentation.cloudUvOffsets[layerIndex];
    const Vector2 &rate = airPresentation.planetLayers[layerIndex].rate;
    const float deltaTick = HLib::GetScaledDeltaTick();
    const Vector2 delta{AdvanceAirUvOffset(offset.x, rate.x * deltaTick), AdvanceAirUvOffset(offset.y, rate.y * deltaTick)};
    MT7Model &model = airPresentation.models[static_cast<std::size_t>(AirModelSlot::CloudA) + layerIndex];
    const AirPlanetLayerState &layer = airPresentation.planetLayers[layerIndex];
    for (const MT7Node &node : model.nodes) {
        if (!node.meshVertexCount || node.meshVertexOffset > model.vertices.size() || node.meshVertexCount > model.vertices.size() - node.meshVertexOffset) {
            continue;
        }
        const std::size_t uvOffset = node.meshVertexStride == 32 ? 24U : node.meshVertexStride >= 48 ? 40U : 0U;
        if (!uvOffset)
            continue;

        for (std::uint32_t index = 0; index < node.meshVertexCount; ++index) {
            MT7Vertex &vertex = model.vertices[node.meshVertexOffset + index];
            vertex.uv.x += delta.x;
            vertex.uv.y += delta.y;
            std::uint32_t bits{};
            std::memcpy(&bits, &vertex.uv.y, sizeof(bits));
            bits |= 1U;
            std::memcpy(&vertex.uv.y, &bits, sizeof(bits));
        }

        const LegacyVertexBufferBinding *const binding = FindMT7MutableVertexBuffer(node);
        if (!binding)
            continue;
        const std::int32_t size = d3t::GetSizeForModelBuffer(binding->handle);
        if (size < 8)
            continue;
        auto *const destination = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, binding->handle, 1, 0));
        if (!destination)
            continue;
        bool wrote = false;
        for (std::uint32_t index = 0; index < node.meshVertexCount; ++index) {
            const std::size_t byteOffset = static_cast<std::size_t>(index) * node.meshVertexStride + uvOffset;
            if (byteOffset > static_cast<std::size_t>(size) - 8U)
                continue;
            if (node.meshVertexStride == 32 && index < airCloudVertexLevels.size()) {
                const std::uint32_t colour[2]{layer.levelColours[airCloudVertexLevels[index]], 0};
                std::memcpy(destination + byteOffset - 8, colour, sizeof(colour));
            }
            const Vector2 &uv = model.vertices[node.meshVertexOffset + index].uv;
            std::memcpy(destination + byteOffset, &uv, sizeof(uv));
            wrote = true;
        }
        if (wrote) {
            d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, binding->handle);
        }
    }
}

void AdvanceAirUv() {
    if (!airPresentation.geometryReady)
        return;
    AdvanceAirLayerUv(0);
    AdvanceAirLayerUv(1);
}

std::uint32_t UploadAirPvrTexture(std::span<const std::byte> bytes, std::size_t offset) {
    std::uint32_t signature{};
    if (!ReadAirValue(bytes, offset, signature))
        return 0;
    if (signature == gbixSignature) {
        std::uint32_t gbixSize{};
        if (!ReadAirValue(bytes, offset + 4, gbixSize) || gbixSize > bytes.size() - offset - 8) {
            return 0;
        }
        offset += static_cast<std::size_t>(gbixSize) + 8;
        if (!ReadAirValue(bytes, offset, signature))
            return 0;
    }
    if (signature != pvrtSignature || offset + 16 > bytes.size())
        return 0;
    const std::size_t payload = offset + 16;
    std::uint32_t payloadSignature{};
    if (!ReadAirValue(bytes, payload, payloadSignature) || payloadSignature != ddsSignature) {
        return 0;
    }
    return d3t::CreateImageTextureFromMemory(bytes.data() + payload, bytes.size() - payload, true);
}

void SetAirModelTexture(MT7Model &model, std::size_t textureIndex, std::uint32_t handle) {
    if (textureIndex >= model.textures.size())
        return;
    model.textures[textureIndex].textureIndex = handle;
    for (MT7Node &node : model.nodes) {
        for (LegacyTextureBinding &binding : node.drawResources.textureBindings) {
            if (binding.textureTableIndex == static_cast<std::int32_t>(textureIndex)) {
                binding.textureHandle = handle;
            }
        }
    }
}

bool UploadEmbeddedAirTextures(std::span<const std::byte> modelBytes, MT7Model &model) {
    MT7Header header{};
    if (!ReadAirValue(modelBytes, 0, header) || header.size > modelBytes.size() || modelBytes.size() - header.size < 16) {
        return false;
    }
    const std::size_t txt7 = header.size;
    std::uint32_t signature{};
    std::uint32_t count{};
    if (!ReadAirValue(modelBytes, txt7, signature) || signature != txt7Signature || !ReadAirValue(modelBytes, txt7 + 8, count) ||
        count != model.textures.size()) {
        return false;
    }
    for (std::uint32_t index = 0; index < count; ++index) {
        std::uint32_t textureOffset{};
        if (!ReadAirValue(modelBytes, txt7 + 12 + index * 4U, textureOffset) || textureOffset >= modelBytes.size() - txt7) {
            return false;
        }
        const std::uint32_t handle = UploadAirPvrTexture(modelBytes, txt7 + textureOffset);
        if (!handle)
            return false;
        SetAirModelTexture(model, index, handle);
    }
    return true;
}

void ReleaseAirGeometry() {
    std::array<std::uint32_t, static_cast<std::size_t>(AirModelSlot::Count)> released{};
    std::size_t releasedCount = 0;
    for (MT7Model &model : airPresentation.models) {
        const std::vector<MT7TextureEntry> textures = std::move(model.textures);
        ReleaseModelFormat7(model);
        for (const MT7TextureEntry &texture : textures) {
            if (!texture.textureIndex ||
                std::find(released.begin(), released.begin() + releasedCount, texture.textureIndex) != released.begin() + releasedCount) {
                continue;
            }
            d3t::InitTexture2DResource(texture.textureIndex);
            if (releasedCount < released.size()) {
                released[releasedCount++] = texture.textureIndex;
            }
        }
    }
    airPresentation.doomBytes.clear();
    airPresentation.doomBytes.shrink_to_fit();
    airPresentation.geometryReady = false;
    airWeatherPresentation.clouds = {};
    airPresentation.cloudBlends = {1.0f, 0.0f};
    airPresentation.cloudBlendMax = {1.0f, 1.0f};
    airPresentation.cloudUvOffsets = {};
    airWeatherPresentation.previousSecondsSince1980 = 0;
    airWeatherPresentation.secondsSince1980 = 0;
}

bool LoadAirGeometry() {
    ReleaseAirGeometry();
    if (!LoadArchivedAirFile("/misc/DOOM.BIN", airPresentation.doomBytes)) {
        return false;
    }
    const std::span<std::byte> doom{airPresentation.doomBytes.data(), airPresentation.doomBytes.size()};
    for (std::size_t index = 0; index < airModelTags.size(); ++index) {
        std::span<std::byte> chunk = FindAirModelChunk(doom, airModelTags[index]);
        if (chunk.empty()) {
            ReleaseAirGeometry();
            return false;
        }
        std::array<char, 5> label{static_cast<char>(airModelTags[index]), static_cast<char>(airModelTags[index] >> 8U),
            static_cast<char>(airModelTags[index] >> 16U), static_cast<char>(airModelTags[index] >> 24U), '\0'};
        MT7Model &model = airPresentation.models[index];
        const std::span<const std::byte> source{chunk.data(), chunk.size()};
        if (!ReadModelFormat7(source, label.data(), model) || !UploadEmbeddedAirTextures(source, model)) {
            ReleaseAirGeometry();
            return false;
        }
    }
    AllocateCDCX(airPresentation.models[static_cast<std::size_t>(AirModelSlot::CloudA)], airCloudVertexLevels, 9);
    AllocateCDCX(airPresentation.models[static_cast<std::size_t>(AirModelSlot::DomeDay)], airDomeVertexLevels, 5);
    airPresentation.geometryReady = true;
    return true;
}

bool IsAirTextureUsedElsewhere(const MT7Model *excluded, std::uint32_t handle) {
    return std::any_of(airPresentation.models.begin(), airPresentation.models.end(), [excluded, handle](const MT7Model &model) {
        return &model != excluded &&
            std::any_of(model.textures.begin(), model.textures.end(), [handle](const MT7TextureEntry &texture) { return texture.textureIndex == handle; });
    });
}

void ReadSpriteForSkyboxObjects(std::int32_t textureIndex, std::int32_t layer, float blendMax, const std::byte *sprite) {
    airPresentation.cloudBlendMax[static_cast<std::size_t>(layer)] = blendMax;
    airWeatherPresentation.clouds.textureIndices[static_cast<std::size_t>(layer)] = textureIndex;
    const std::span<const std::byte> bytes{sprite, 0x8800};

    const std::uint32_t handle = UploadAirPvrTexture(bytes, 16);
    if (!handle)
        return;
    MT7Model &cloud = airPresentation.models[static_cast<std::size_t>(AirModelSlot ::CloudA) + static_cast<std::size_t>(layer)];
    const std::uint32_t previous = cloud.textures.empty() ? 0 : cloud.textures[0].textureIndex;
    SetAirModelTexture(cloud, 0, handle);
    if (previous && previous != handle && !IsAirTextureUsedElsewhere(&cloud, previous))
        d3t::InitTexture2DResource(previous);
}

// @verified - p
void ReadSkyboxTexture(std::int32_t textureIndex, std::int32_t layer, float blendMax) {
    if (airWeatherPresentation.clouds.textureIndices[static_cast<std::size_t>(layer)] == textureIndex)
        return;
    char filename[16];
    std::snprintf(filename, sizeof(filename), "AIR_TX%02d.SPR", textureIndex);
    (void)LoadLegacyFiles("sprite", filename, g_armMotionMemory, 0x8800);
    ReadSpriteForSkyboxObjects(textureIndex, layer, blendMax, g_armMotionMemory);
}
// @verified - p
std::int32_t ReadCloudSpriteTexture(std::int32_t textureIndex, std::int32_t layer, float blendMax) {
    AirCloudTextureState &clouds = airWeatherPresentation.clouds;
    if (!clouds.sprite)
        return 1;
    const std::int32_t state = d3t::CheckFileInfo(clouds.sprite);
    if (state == 3) {
        (void)ReturnZero();
        d3t::ReadCachedFile(clouds.sprite);
        ReadSpriteForSkyboxObjects(textureIndex, layer, blendMax, g_armMotionMemory);
        clouds.sprite = nullptr;
        clouds.loadingTextureIndex = -1;
        return 1;
    }
    if (state != 4)
        return 0;
    nullsub();
    return 1;
}

// @verified - p
std::int32_t SetupCloudTexture(std::int32_t textureIndex, std::int32_t frames, std::int32_t mode, float blendMax) {
    AirCloudTextureState &clouds = airWeatherPresentation.clouds;
    if (!clouds.sprite) {
        if (clouds.active)
            return 0;
        clouds.active = 1;
        clouds.frames = frames;
        clouds.loadingTextureIndex = textureIndex;
        char path[32];
        std::snprintf(path, sizeof(path), "sprite\\AIR_TX%02d.SPR", textureIndex);
        clouds.sprite = d3t::LargeFile_OpenPathHandle(path);
        (void)d3t::GetFileFromCachedBuffer(clouds.sprite, 17, g_armMotionMemory, 0);
    }
    if (!ReadCloudSpriteTexture(textureIndex, 1, blendMax))
        return 0;
    const float step = frames ? 1.0f / static_cast<float>(frames) : 0.0f;
    airPresentation.cloudBlends[0] = 1.0f;
    airPresentation.cloudBlends[1] = mode == 1 ? 1.0f : 0.0f;
    clouds.blendSteps[0] = step;
    clouds.blendSteps[1] = -step;
    nullsub();
    return 1;
}
// @verified - p
void ResetCloudScaling() {
    AirCloudTextureState &clouds = airWeatherPresentation.clouds;
    std ::swap(airPresentation.models[static_cast<std::size_t>(AirModelSlot::CloudA)], airPresentation.models[static_cast<std::size_t>(AirModelSlot::CloudB)]);
    std::swap(airPresentation.cloudBlendMax[0], airPresentation.cloudBlendMax[1]);
    std::swap(airPresentation.cloudUvOffsets[0], airPresentation.cloudUvOffsets[1]);
    std::swap(airPresentation.planetLayers[0], airPresentation.planetLayers[1]);
    std::swap(airPresentation.cloudBlends[0], airPresentation.cloudBlends[1]);
    std::swap(clouds.blendSteps[0], clouds.blendSteps[1]);
    std::swap(clouds.textureIndices[0], clouds.textureIndices[1]);
}
// @verified - p
void ResetCloudScaling_0() {
    AirCloudTextureState &clouds = airWeatherPresentation.clouds;
    if (!clouds.active)
        return;
    ResetCloudScaling();
    airPresentation.cloudBlends[0] = 1.0f;
    clouds.blendSteps[0] = 0.0f;
    airPresentation.cloudBlends[1] = 0.0f;
    clouds.blendSteps[1] = 0.0f;
    nullsub();
    clouds.active = 0;
    clouds.frames = 0;
    if (clouds.sprite) {
        (void)ReturnZero();
        d3t::ReadCachedFile(clouds.sprite);
        clouds.sprite = nullptr;
        clouds.loadingTextureIndex = -1;
    }
}

// @verified - p
void AdvanceCloudTransition() {
    AirCloudTextureState &clouds = airWeatherPresentation.clouds;
    if (!clouds.active || clouds.sprite)
        return;
    clouds.frames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(clouds.frames) - 1U);
    float first = airPresentation.cloudBlends[0];
    float second = airPresentation.cloudBlends[1];
    if (airWeatherPresentation.secondsSince1980 - airWeatherPresentation.previousSecondsSince1980 > 60U) {
        first -= 0.050000001f;
        second += 0.050000001f;
    }
    first -= clouds.blendSteps[0];
    second -= clouds.blendSteps[1];
    airPresentation.cloudBlends[0] = first > 1.0f ? 1.0f : first < 0.0f ? 0.0f : first;
    airPresentation.cloudBlends[1] = second > 1.0f ? 1.0f : second < 0.0f ? 0.0f : second;
    nullsub();
    if (clouds.frames < 0)
        ResetCloudScaling_0();
}

// @verified - p
std::uint32_t RecursiveSetTranslationForPlanets(const MT7Model &model, const MT7Node *node, float alpha, bool translate, bool clouds) {
    const bool sibling = node->runtimeSibling != nullptr;
    if (sibling)
        Matrix4x4_Push();
    if (translate)
        Matrix4x4::preTranslate(node->position.x, node->position.y + (clouds ? 0.0099999998f : -0.0099999998f), node->position.z);
    std::uint32_t draws = 0;
    if (node->xb01Offset) {
        SetLegacyModelScale(airModelScale);
        float minimumDepth{}, maximumDepth{};
        d3t::GetSceneViewportDepthRange(&minimumDepth, &maximumDepth);
        d3t::SetSceneViewportDepthRange(skyDepth, skyDepth);
        const LegacyDrawResources &resources = node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources;
        if (alpha >= 1.0f)
            draws += ModelFlagDispatch_SetModelFlagDataOpaque(model.source, node->xb01Offset, resources);
        else
            draws += SetModelFlagDataWithAlpha(model.source, node->xb01Offset, resources, alpha);
        d3t::SetSceneViewportDepthRange(minimumDepth, maximumDepth);
    }
    if (node->runtimeChild)
        draws += RecursiveSetTranslationForPlanets(model, node->runtimeChild, alpha, translate, clouds);
    if (sibling) {
        Matrix4x4::Pop(1);
        draws += RecursiveSetTranslationForPlanets(model, node->runtimeSibling, alpha, translate, clouds);
    }
    return draws;
}

std::uint32_t DrawAirModel(AirModelSlot slot, float alpha, bool translate, bool clouds) {
    if (!airPresentation.geometryReady)
        return 0;
    const MT7Model &model = airPresentation.models[static_cast<std::size_t>(slot)];
    if (model.nodes.empty())
        return 0;
    const bool queueEnabled = GetLegacyRenderQueueEnabled();
    SetLegacyRenderQueueEnabled(false);
    const std::uint32_t draws = RecursiveSetTranslationForPlanets(model, model.nodes.data(), alpha, translate, clouds);
    SetLegacyRenderQueueEnabled(queueEnabled);
    return draws;
}

std::uint32_t DispDome() {
    if (!g_airDrawDomeEnabled)
        return 0;
    nullsub();
    constexpr std::array<float, 2> layerOffsets{115.0f, -205.0f};
    const std::array<float, 2> &blends = airPresentation.domeBlends;
    std::uint32_t draws = 0;
    for (std::size_t index = 0; index < blends.size(); ++index) {
        if (!(blends[index] > 0.0f))
            continue;
        Matrix4x4_Push();
        Matrix4x4::preTranslate(0.0f, layerOffsets[index], 0.0f);
        draws += DrawAirModel(index == 0 ? AirModelSlot::DomeDay : AirModelSlot::DomeNight, blends[index], index != 0, false);
        Matrix4x4::Pop(1);
    }
    nullsub();
    return draws;
}

std::uint32_t DispPlanets() {
    if (!g_airDrawPlanetsEnabled)
        return 0;
    nullsub();
    const AirPlanetDrawRecord &planet = airPresentation.planetDrawRecord;
    if (!(planet.intensity > 0.0f)) {
        nullsub();
        return 0;
    }
    const Vector3 &scale = planet.scale;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(planet.position.x, planet.position.y, planet.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(planet.rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(planet.rotation.x));
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    airModelScale = scale.x;
    SetShouldDomeRenderTo(1);
    const std::uint32_t draws = DrawAirModel(AirModelSlot::Moon, std::pow(planet.intensity, 0.45454544f), false, false);
    SetShouldDomeRenderTo(0);
    Matrix4x4::Pop(1);
    nullsub();
    return draws;
}

std::uint32_t DispAir() {
    if (!g_airDrawCloudsEnabled)
        return 0;
    nullsub();
    const std::array<float, 2> &blends = airPresentation.cloudBlends;
    const Vector3 &offset = airPresentation.planetLayers[1].offset;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    Matrix4x4::Scale(1.3f, 2.5999999f, 1.3f);
    airModelScale = 2.5999999f;
    std::uint32_t draws = 0;
    for (std::int32_t index = 1; index >= 0; --index) {
        const std::size_t slot = static_cast<std::size_t>(index);
        if (!(blends[slot] > 0.0f))
            continue;
        Matrix4x4_Push();
        SetLegacyFogMode(2);
        draws += DrawAirModel(index == 0 ? AirModelSlot::CloudA : AirModelSlot::CloudB, blends[slot] * airPresentation.cloudBlendMax[slot], index != 0, true);
        SetLegacyFogMode(0);
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
    nullsub();
    return draws;
}



void Air_Draw(void *) {
    DrawAirWorldPass();
}


void ObjectPlayerDrawNullCallback(void *) {
}

void DrawOrdinaryMapCallback(void *) {
    DrawObjectWorldPass();
}
void DrawMapPlacementCallback(void *) {
    DrawMapLightingPlacementWorldPass();
}



bool IsPropObjectVisible(const MT7Model &model, const MT7Node &object, const Vector3 &point) {
    Matrix4x4_Push();
    float radius = IsMDPXorMDP7(&object) == 0x74U ? 5.0f : 3.0f;
    Vector3 centre = object.position;
    if (object.xb01Offset) {
        Matrix4x4::preTranslate(object.position.x, object.position.y, object.position.z);
        Matrix4x4::SetViewMatrixFromEuler(object.rotation.y, object.rotation.x, object.rotation.z);
        const std::byte *const xb01 = model.source.data() + object.xb01Offset;
        float bound{};
        std::memcpy(&centre, xb01 + 8, sizeof(centre));
        std::memcpy(&bound, xb01 + 20, sizeof(bound));
        radius = std::fmax(radius, bound);
    }
    if (!camCtrlIsSphereVisible(centre, radius))
        radius = -1.0f;
    Matrix4x4::Pop(1);
    return radius > 0.0f && camCtrlIsModelDepthVisible(point, object, 2.0f);
}


std::uint32_t DrawPropObjectHierarchy(
    const MT7Model &model, const MT7Node *node, const Matrix4x4 &objectBase, LegacyModelShaderPath shaderPath, std::uint32_t drawMeshes) {
    std::uint32_t draws = 0;
    while (node) {
        const bool siblingFrame = node->runtimeSibling != nullptr;
        const Matrix4x4 incoming = workTransformMatrix;
        const bool pushed = siblingFrame && Matrix4x4_Push();
        if (const Matrix4x4 *const transform = GetMT7NodeTransformOverride(*node)) {
            Matrix4x4::SetActiveTransform(&objectBase);
            Matrix4x4::MultiplyWithPrePassMatrix(transform);
        } else
            ApplyMT7NodeTransform(*node);
        if (drawMeshes && node->visible && (static_cast<std::uint32_t>(node->id) >> 16U) == 0 && node->xb01Offset) {
            const MT7Node &meshNode = node->borrowedMesh ? *node->borrowedMesh : *node;
            const std::byte *const mesh = meshNode.runtimeMesh ? meshNode.runtimeMesh : model.source.data() + node->xb01Offset;
            std::uint32_t flags{};
            std::memcpy(&flags, mesh + 4, sizeof(flags));
            if (flags & 4U)
                NoteXb01Flag4Mesh();
            if (node->commandOverride && node->commandOverride->resources) {
                const LegacyNodeCommandOverride &command = *node->commandOverride;
                draws += shaderPath == LegacyModelShaderPath::Lambert ?
                    ModelFlagDispatch_SetModelFlagDataOpaque(command.bytes, command.xb01Offset, *command.resources) :
                    ModelFlagDispatch_ReadModelDataOpaque(command.bytes, command.xb01Offset, *command.resources);
            } else {
                draws += shaderPath == LegacyModelShaderPath::Lambert ?
                    ModelFlagDispatch_SetModelFlagDataOpaque(model.source, node->xb01Offset, meshNode.drawResources) :
                    ModelFlagDispatch_ReadModelDataOpaque(model.source, node->xb01Offset, meshNode.drawResources);
            }
        }
        draws += DrawPropObjectHierarchy(model, node->runtimeChild, objectBase, shaderPath, drawMeshes);
        if (siblingFrame) {
            if (pushed)
                Matrix4x4::Pop(1);
            else
                Matrix4x4::SetActiveTransform(&incoming);
        }
        node = node->runtimeSibling;
    }
    return draws;
}

std::uint32_t DrawPropObject(const MT7Model &model, MT7Node &object, const Matrix4x4 &objectBase, LegacyModelShaderPath shaderPath) {
    MT7Node *const sibling = object.runtimeSibling;
    object.runtimeSibling = nullptr;
    Matrix4x4_Push();
    const std::uint32_t draws = DrawPropObjectHierarchy(model, &object, objectBase, shaderPath, 1);
    Matrix4x4::Pop(1);
    object.runtimeSibling = sibling;
    return draws;
}

const MT7Model *FindPropObjectReader(const MT7Node &object) {
    for (const PropReaderSlot &slot : GetPropReaderSlots()) {
        if (slot.reader && !slot.reader->nodes.empty() && &object >= slot.reader->nodes.data() &&
            &object < slot.reader->nodes.data() + slot.reader->nodes.size())
            return slot.reader;
    }
    return nullptr;
}





void DrawPropObjectCallback(void *) {
    const std::int32_t previousVertexColourType = GetLegacyVertexColourType();
    GetFieldDepthModelTransform() = GetFieldDepthTransform();
    const Matrix4x4 objectBase = workTransformMatrix;
    SetLegacyVertexColourType(1);
    Matrix4x4_Push();
    Vector3 propOffset{};
    GetPropVector4(&propOffset);
    Matrix4x4::preTranslate(propOffset.x, propOffset.y, propOffset.z);
    const LegacyModelShaderPath shaderPath =
        GetPreviousActiveLight() == 1 || GetPropLightingTraversalResult() != 0 ? LegacyModelShaderPath::Lambert : LegacyModelShaderPath::VertexColour;
    MT7Node *const propRoot = GetPropRoot();
    if (propRoot) {
        for (const PropReaderSlot &slot : GetPropReaderSlots()) {
            if (!slot.reader || slot.reader->nodes.empty() || !slot.lastSibling)
                continue;
            MT7Model &model = *slot.reader;
            MT7Node *const root = &model.nodes.front();
            MT7Node *const following = slot.lastSibling->runtimeSibling;
            slot.lastSibling->runtimeSibling = nullptr;
            for (MT7Node *object = root->runtimeChild; object; object = object->runtimeSibling)
                object->runtimeParent = root;
            MT7Node *visible = root->runtimeChild;
            while (visible && !IsPropObjectVisible(model, *visible, propOffset))
                visible = visible->runtimeSibling;
            if (visible) {
                Matrix4x4_PushActiveTransform();
                const bool legacy = !d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection());
                UpdateModelFrame(&model, nullptr, 1, 3);
                Vector3 offset{};
                GetPropVector4(&offset);
                Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
                d3t::SetHashedObjectTransform(GetMT7ReaderData(&model));
                Matrix4x4::Pop(1);
                if (legacy) {
                    for (MT7Node *object = root->runtimeChild; object; object = object->runtimeSibling) {
                        if (IsPropObjectVisible(model, *object, propOffset))
                            DrawPropObject(model, *object, objectBase, shaderPath);
                    }
                }
            }
            for (MT7Node *object = root->runtimeChild; object; object = object->runtimeSibling)
                object->runtimeParent = propRoot;
            slot.lastSibling->runtimeSibling = following;
        }
    }
    Matrix4x4::Pop(1);
    if (g_characterMirroredTransformEnabled) {
        Matrix4x4_Push();
        SetLegacyMirroredDrawMode(1);
        Matrix4x4::MultiplyWithPrePassMatrix(GetCharacterMirrorMatrix());
        const Vector3 origin{};
        for (MT7Node *object = GetPropRoot() ? GetPropRoot()->runtimeChild : nullptr; object; object = object->runtimeSibling) {
            const float radius = IsMDPXorMDP7(object) == 0x74U ? 5.0f : 3.0f;
            if (!camCtrlIsSphereVisible(object->position, radius) || !camCtrlIsModelDepthVisible(origin, *object, 2.0f))
                continue;
            if (const MT7Model *const model = FindPropObjectReader(*object))
                DrawPropObject(*model, *object, objectBase, shaderPath);
        }
        SetLegacyMirroredDrawMode(0);
        Matrix4x4::Pop(1);
    }
    SetLegacyVertexColourType(previousVertexColourType);
}

// @verified - p
void CtrlAir_Callback(void *taskData) {
    auto *const parameters = static_cast<AirDrawTaskParameters *>(taskData);
    if (!parameters)
        return;
    parameters->mode = 0;
    UpdateTimeOfDay();
    airWeatherPresentation.previousSecondsSince1980 = airWeatherPresentation.secondsSince1980;
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    airWeatherPresentation.secondsSince1980 = static_cast<std::uint32_t>(ComputeSecondsSince1980(dateTime));
    UpdateCelestialCoordinates();
    UpdateNightTime();
    AdvanceCloudTransition();
    for (AirPlanetLayerState &layer : airPresentation.planetLayers)
        ComputeCloudScaling(layer.colour0, layer.colour1, cloudGradientWeights, layer.levelColours.data());
    ComputeCloudScaling(airWeatherPresentation.skyColour, airWeatherPresentation.cloudColour, domeGradientWeights, airPresentation.skyLevelColours.data());
    UploadAirDomeColours();
    AdvanceAirUv();
    if (airSkyEnabled)
        HLib::ExecuteCallbackNow(Air_Draw, nullptr);
    d3t::SetSceneClearColour(airSkyEnabled ? airWeatherPresentation.cloudColour : g_airClearColour);
}

void OBJ_Object_Callback(void *taskData) {

    auto *const parameters = static_cast<ObjectDrawTaskParameters *>(taskData);
    if (!parameters)
        return;
    parameters->submittedOpaque = 0;
    parameters->submittedTransparent = 0;
    if (!IsWorldDrawingEnabled())
        return;
    g_currentFieldPointCount = GetFieldDepthPointCount();
    g_currentDrawTier = camCtrlGetDrawTier(camCtrlGetCurrentFarDistance());
    if (!d3t::BindSceneFrameRenderTarget())
        return;
    const MapControlData *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    HLib::ExecuteCallbackNow(ObjectPlayerDrawNullCallback, control ? control->ryo : nullptr);
    HLib::ExecuteCallbackNow(DrawOrdinaryMapCallback, nullptr);
    HLib::ExecuteCallbackNow(DrawMapPlacementCallback, nullptr);
    UpdateWindForFlag();
    HLib::ExecuteCallbackNow(DrawPropObjectCallback, nullptr);
}

} // namespace

HLib::Task *QueueObjectDrawTask(std::uint8_t nextFunctionIndex) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(OBJ_Object_Callback, nextFunctionIndex, 5, sizeof(ObjectDrawTaskParameters), SHEN_NAME(OBJE));
    if (auto *const parameters = static_cast<ObjectDrawTaskParameters *>(HLib::GetTaskParameter(task))) {
        parameters->submittedOpaque = 0;
        parameters->submittedTransparent = 0;
    }
    return task;
}
std::uint32_t DrawAirWorldPass() {
    if (!airSkyEnabled || !airPresentation.geometryReady || !d3t::BindSceneFrameRenderTarget()) {
        return 0;
    }
    nullsub();
    const std::int32_t previousVertexColourType = GetLegacyVertexColourType();
    SetLegacyVertexColourType(0);
    std::array<bool, 16> previousVertexColourStates{};
    for (std::uint32_t slot = 0; slot < previousVertexColourStates.size(); ++slot) {
        previousVertexColourStates[slot] = GetLegacyVertexColourStateEnabled(slot);
        SetLegacyVertexColourStateEnabled(slot, false);
    }
    UpdateLegacyLightTransforms();
    airModelScale = 1.0f;
    std::uint32_t draws = GetLegacyPerspectiveProjection() == 0 ? DispDome() : 0;
    draws += DispPlanets();
    if (GetLegacyPerspectiveProjection() == 0)
        draws += DispAir();
    for (std::uint32_t slot = 0; slot < previousVertexColourStates.size(); ++slot) {
        SetLegacyVertexColourStateEnabled(slot, previousVertexColourStates[slot]);
    }
    UpdateLegacyLightTransforms();
    SetLegacyVertexColourType(previousVertexColourType);
    nullsub();
    return draws;
}
// @verified - p
void SetClearColourForClouds(std::uint32_t colour) {
    g_airClearColour = colour;
}

void SetCloudAndSkyColour(std::uint32_t skyColour, std::uint32_t cloudColour) {
    airWeatherPresentation.skyColour = skyColour | 0xff000000U;
    airWeatherPresentation.cloudColour = cloudColour | 0xff000000U;
}

// @verified - p
std::uint32_t GetWeatherCloudOrSkyColour(std::int32_t channel) {
    return channel ? airWeatherPresentation.cloudColour : airWeatherPresentation.skyColour;
}

// @verified - p
void SetIsSkySetup(std::int32_t value) {
    airSkyEnabled = value;
}

float *SetPlanetLayerParameter(float value, std::int32_t layer) {
    float *const base = &airPresentation.planetDrawRecord.intensity;
    const std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(layer) * static_cast<std::ptrdiff_t>(planetLayerParameterStrideFloats);
    std::memcpy(base + offset, &value, sizeof(value));
    return base;
}

float GetPlanetLayerParameter(std::int32_t layer) {
    const float *const base = &airPresentation.planetDrawRecord.intensity;
    const std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(layer) * static_cast<std::ptrdiff_t>(planetLayerParameterStrideFloats);
    return base[offset];
}

// @verified - p
void SetPlanetRotByIndex(const Vector3i &rotation, std::int32_t index) {
    std::byte *const record = reinterpret_cast<std::byte *>(&airPresentation.planetDrawRecord) + static_cast<std::ptrdiff_t>(index) * 0x50;
    std::memcpy(record + offsetof(AirPlanetDrawRecord, intermediateRotation), &rotation.x, sizeof(rotation.x));
    std::memcpy(record + offsetof(AirPlanetDrawRecord, intermediateRotation) + 4, &rotation.y, sizeof(rotation.y));
    std::memcpy(record + offsetof(AirPlanetDrawRecord, intermediateRotation) + 8, &rotation.z, sizeof(rotation.z));
}

// @verified - p
void UpdateCelestialRotation(const Vector3i &angles, const Vector3 &input, Vector3 &output) {
    const std::uint16_t xAngle = static_cast<std::uint16_t>(angles.x), yAngle = static_cast<std::uint16_t>(angles.y);
    const float x = input.x, y = input.y, z = input.z;
    const float sineY = FixedAngleSin(yAngle), cosineY = FixedAngleCos(yAngle);
    const float rotatedX = x * cosineY - z * sineY, rotatedZ = z * cosineY + x * sineY;
    const float sineX = FixedAngleSin(xAngle), cosineX = FixedAngleCos(xAngle);
    output.x = rotatedX;
    output.y = rotatedZ * sineX + y * cosineX;
    output.z = rotatedZ * cosineX - y * sineX;
}

// @verified - p
void UpdateCelestialCoordinates() {
    AirPlanetDrawRecord &planet = airPresentation.planetDrawRecord;
    planet.timeRotation.x = static_cast<std::int32_t>(airTimeOfDay * 0.27306667f) - 16384;
    const Vector3i angles{planet.tilt, planet.timeRotation.x, planet.timeRotation.y};
    UpdateCelestialRotation(angles, planet.orbitInput, planet.position);
    planet.rotation.x = planet.intermediateRotation.x;
    planet.rotation.y = planet.intermediateRotation.y + 16384 + planet.timeRotation.z - planet.timeRotation.x;
    planet.rotation.z = planet.intermediateRotation.z;
}

void shenUpdatePlanetByLayer(float value, std::int32_t channel, std::int32_t layer) {
    Vector2 &rate = airPresentation.planetLayers[static_cast<std::size_t>(layer)].rate;
    if (channel)
        rate.y = value;
    else
        rate.x = value;
}

void EV_SetPlanetLayerColours_Impl(std::uint32_t firstColour, std::uint32_t secondColour, std::int32_t layer) {
    AirPlanetLayerState &destination = airPresentation.planetLayers[static_cast<std::size_t>(layer)];
    destination.colour0 = firstColour | 0xff000000U;
    destination.colour1 = secondColour | 0xff000000U;
}

std::uint64_t GetPlanetLayerColour(std::int32_t channel, std::int32_t layer) {
    const AirPlanetLayerState &source = airPresentation.planetLayers[static_cast<std::size_t>(layer)];
    return static_cast<std::uint64_t>(channel ? source.colour1 : source.colour0);
}

std::uint64_t SetPlanetLayerOffset(const Vector3 &offset, std::int32_t layer) {
    AirPlanetLayerState &destination = airPresentation.planetLayers[static_cast<std::size_t>(layer)];

    std::uint64_t xy{};
    std::memcpy(&xy, &offset, sizeof(xy));
    std::memcpy(&destination.offset, &xy, sizeof(xy));
    std::uint32_t z{};
    const std::byte *const sourceBytes = reinterpret_cast<const std::byte *>(&offset);
    std::memcpy(&z, sourceBytes + sizeof(xy), sizeof(z));
    std::byte *const destinationBytes = reinterpret_cast<std::byte *>(&destination.offset);
    std::memcpy(destinationBytes + sizeof(xy), &z, sizeof(z));
    return static_cast<std::uint64_t>(z);
}

std::uint64_t ResetPlanetLayerOffset(std::int32_t layer) {
    AirPlanetLayerState &destination = airPresentation.planetLayers[static_cast<std::size_t>(layer)];
    destination.offset = {0.0f, -115.0f, 0.0f};
    return std::uint64_t{0};
}

// @verified - p
std::int32_t GetHasInitCloudTextures() {
    return airWeatherPresentation.clouds.active;
}

// @verified - p
std::int32_t GetCurrentAirLayerIndex() {
    return airWeatherPresentation.clouds.loadingTextureIndex;
}

// @verified - p
bool GetIsCurrentCloudTextureValid() {
    return airWeatherPresentation.clouds.sprite != nullptr;
}

// @verified - p
void SetSkyWeatherModes(std::int32_t previous, std::int32_t current) {
    g_previousSkyWeatherMode = previous;
    g_currentSkyWeatherMode = current;
}

// @verified - p
void UpdateWeatherClouds(WeatherAir *air, std::int32_t frames) {
    if (air->field44 == 0) {
        if (air->rain >= 0.1f || air->snow >= 0.1f)
            air->field44 = 1;
    } else if (air->field44 == 1 && air->rain < 0.050000001f && air->snow < 0.050000001f) {
        air->field44 = 0;
    }
    if (air->field40 == 0) {
        if (air->cloud >= 0.25f)
            air->field40 = 1;
    } else if (air->field40 == 1 && air->cloud < 0.2f) {
        air->field40 = 0;
    }
    std::int32_t mode = 1;
    if (air->field44)
        mode = 5;
    else if (air->field40)
        mode = 6;
    else {
        const std::int32_t minute = air->hour * 60 + air->minute;
        if (minute < 330)
            mode = 4;
        else if (minute < 630)
            mode = 0;
        else if (minute < 900)
            mode = 1;
        else if (minute < 960)
            mode = 2;
        else if (minute < 1080)
            mode = 3;
        else
            mode = 4;
    }
    if (air->mode == mode)
        return;
    const std::int32_t textureIndex = cloudTextureIndices[static_cast<std::size_t>(mode)];
    if (GetHasInitCloudTextures() && GetCurrentAirLayerIndex() != textureIndex)
        ResetCloudScaling_0();
    DateTime time;
    GetCurrentDateTime(time);
    if (frames == 500) {
        if (static_cast<std::uint8_t>(time.hour - 5) > 12U && air->mode <= 4 && mode >= 5)
            frames = 800;
    } else if (mode < 5 && static_cast<std::uint32_t>(air->mode - 5) > 1U) {
        frames = 500;
    }
    const std::int32_t previousTexture = air->mode == -1 ? 0 : cloudTextureIndices[static_cast<std::size_t>(air->mode)];
    if (SetupCloudTexture(textureIndex, frames, previousTexture == 2 ? 1 : 0, cloudBlendMaxValues[static_cast<std::size_t>(mode)])) {
        SetSkyWeatherModes(air->mode, mode);
        air->mode = mode;
    }
}

// @verified - p
void UpdateClouds(WeatherAir *air) {
    air->field44 = air->rain >= 0.1f || air->snow >= 0.1f;
    air->field40 = air->cloud > 0.25f;

    std::int32_t mode = 0;
    if (air->field44)
        mode = 5;
    else if (air->field40)
        mode = 6;
    else {
        const std::int32_t minute = air->hour * 60 + air->minute;
        if (minute < 330)
            mode = 4;
        else if (minute < 630)
            mode = 0;
        else if (minute < 900)
            mode = 1;
        else if (minute < 960)
            mode = 2;
        else if (minute < 1080)
            mode = 3;
        else
            mode = 4;
    }

    ReadSkyboxTexture(cloudTextureIndices[static_cast<std::size_t>(mode)], 0, cloudBlendMaxValues[static_cast<std::size_t>(mode)]);
    SetSkyWeatherModes(air->mode, mode);
    air->mode = mode;
}

void InitializeAirPresentation(const void *mapInfo) {
    g_airClearColour = 0;
    // CreateSkybox/InitPlanets reset the AIR owner before interpreting SCRL.
    // A previous map's weather mode must not replace the authored texture.
    airWeatherPresentation = {};
    airPresentation.planetLayers = {};
    airPresentation.planetDrawRecord = {};
    airSkyEnabled = 1;
    std::int32_t textureIndex = -1;
    if (const MapInfoChunk *const scroll = FindMapInfoChunk("SCRL", mapInfo); scroll && scroll->nextByteOffset >= 12) {
        std::memcpy(&textureIndex, reinterpret_cast<const std::byte *>(scroll) + 8, sizeof(textureIndex));
        airSkyEnabled = textureIndex != -1;
    }
    LoadAirGeometry();
    if (airSkyEnabled && textureIndex >= 0)
        ReadSkyboxTexture(textureIndex, 0, 1.0f);

    d3t::SetSceneClearColour(airSkyEnabled ? airWeatherPresentation.cloudColour : g_airClearColour);
}

void ResetAirPresentation() {
    ReleaseAirGeometry();
    airSkyEnabled = 0;
    d3t::SetSceneClearColour(g_airClearColour);
}

// @verified - p
HLib::Task *QueueAirDrawTask(std::uint8_t nextFunctionIndex) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(CtrlAir_Callback, nextFunctionIndex, 6, sizeof(AirDrawTaskParameters), TASK_AIRD);
    if (auto *const parameters = static_cast<AirDrawTaskParameters *>(HLib::GetTaskParameter(task))) {
        parameters->mode = 0;
    }
    return task;
}

namespace {
struct WaveBin {
    float amplitude{};
    float phaseA{};
    float speedA{};
    float phaseB{};
    float speedB{};
};
struct WaveComplex {
    float real{};
    float imaginary{};
};
struct WaveRuntime {
    float cellSize{};
    float lightAmbient{};
    Vector3 lightDirection{};
    std::uint32_t lightingFlags{};
    float ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
    float specularExponent{16.0f};
    float specularIntensity{1.0f};
    float scrollU{};
    float scrollV{};
    std::array<WaveBin, 64> bins{};
    std::array<WaveComplex, 8> twiddles{};
    std::array<float, 289> real{};
    std::array<float, 289> imaginary{};
    std::array<LegacyWaveVertex, 289> vertices{};
};
WaveRuntime wave;
float textureAnimationU{};
float textureAnimationV{};

float WaveLog(float value) {
    float result = 0.0f;
    if (value <= 0.0f)
        return result;
    while (value >= 2.0f) {
        value *= 0.5f;
        result += 0.69314718f;
    }
    while (value < 1.0f) {
        value += value;
        result += -0.69314718f;
    }
    const float z = (value - 1.0f) / (value + 1.0f);
    float term = z + z;
    const float square = z * z;
    for (int denominator = 1;; denominator += 2) {
        const float previous = result;
        const float current = term;
        term *= square;
        result += current / static_cast<float>(denominator);
        if (result == previous)
            return result;
    }
}

float WavePower(float value, float exponent) {

    const float logarithm = WaveLog(value) * exponent;
    const int power = static_cast<int>(logarithm / 0.69314718f + (logarithm < 0.0f ? -0.5f : 0.5f));
    const float remainder = logarithm - static_cast<float>(power) * 0.69314718f;
    const float square = remainder * remainder;
    float fraction = square / 14.0f;
    for (int denominator = 10; denominator >= 6; denominator -= 4)
        fraction = square / (static_cast<float>(denominator) + fraction);
    return std::ldexp(((fraction + 2.0f) + remainder) / ((fraction + 2.0f) - remainder), power);
}

float ReadWaveKernel(const void *kernel, std::size_t index) {
    float value{};
    std::memcpy(&value, static_cast<const std::byte *>(kernel) + index * 4, 4);
    return value;
}
void WriteWaveKernel(void *kernel, std::size_t index, float value) {
    std::memcpy(static_cast<std::byte *>(kernel) + index * 4, &value, 4);
}

WaveComplex WaveAdd(WaveComplex first, WaveComplex second) {
    return {first.real + second.real, first.imaginary + second.imaginary};
}
WaveComplex WaveSubtract(WaveComplex first, WaveComplex second) {
    return {first.real - second.real, first.imaginary - second.imaginary};
}
WaveComplex WaveRotate(WaveComplex value, WaveComplex twiddle) {
    return {value.real * twiddle.real + twiddle.imaginary * value.imaginary, -twiddle.imaginary * value.real + twiddle.real * value.imaginary};
}

void TransformWaveLine(std::array<WaveComplex, 16> &line) {
    // TransformWaveLine performs four-bit reversal, radix-four and radix-two passes,
    // then the final scaled pair pass with factor 1/16.
    for (unsigned index = 0; index < 16; ++index) {
        const unsigned reversed = ((index & 1U) << 3U) | ((index & 2U) << 1U) | ((index & 4U) >> 1U) | ((index & 8U) >> 3U);
        if (index < reversed)
            std::swap(line[index], line[reversed]);
    }
    for (std::size_t group = 0; group < 4; ++group) {
        const auto index = group * 4;
        const auto first = WaveAdd(line[index], line[index + 1]);
        const auto second = WaveAdd(line[index + 2], line[index + 3]);
        const auto third = WaveRotate(WaveSubtract(line[index], line[index + 1]), wave.twiddles[group * 2]);
        const auto fourth = WaveRotate(WaveSubtract(line[index + 2], line[index + 3]), wave.twiddles[group * 2 + 1]);
        line[index] = WaveAdd(first, second);
        line[index + 2] = WaveRotate(WaveSubtract(first, second), wave.twiddles[group]);
        line[index + 1] = WaveAdd(third, fourth);
        line[index + 3] = WaveRotate(WaveSubtract(third, fourth), wave.twiddles[group]);
    }
    for (std::size_t index = 0; index < 4; ++index) {
        const auto difference = WaveSubtract(line[index], line[index + 4]);
        line[index] = WaveAdd(line[index], line[index + 4]);
        line[index + 4] = difference;
        const auto rotated = WaveSubtract(line[index + 8], line[index + 12]);
        line[index + 8] = WaveAdd(line[index + 8], line[index + 12]);
        line[index + 12] = {-rotated.imaginary, rotated.real};
    }
    for (std::size_t index = 0; index < 8; ++index) {
        const auto sum = WaveAdd(line[index], line[index + 8]);
        const auto difference = WaveSubtract(line[index], line[index + 8]);
        line[index] = {sum.real * 0.0625f, sum.imaginary * 0.0625f};
        line[index + 8] = {difference.real * 0.0625f, difference.imaginary * 0.0625f};
    }
}

void UpdateWaveSpectrum() {
    const std::uint8_t stabilize = g_stabilizeFps;
    const float frameTime = g_stabilizedFrameTime;
    for (WaveBin &bin : wave.bins) {
        const auto advance = [stabilize, frameTime](float phase, float speed) {
            if (stabilize)
                speed = (speed * 30.0f) * frameTime;
            phase += speed;
            if (!(phase <= 3.1415927f))
                phase = static_cast<float>(static_cast<double>(phase) - 6.283185482025146);
            else if (!(phase >= -3.1415927f))
                phase = static_cast<float>(static_cast<double>(phase) + 6.283185482025146);
            return phase;
        };
        bin.phaseA = advance(bin.phaseA, bin.speedA);
        bin.phaseB = advance(bin.phaseB, bin.speedB);
    }
    // GenerateFFT embeds authored phase tables and conjugates in
    // opposite quadrants of the 17-stride work buffers.
    for (std::size_t row = 0; row < 8; ++row) {
        for (std::size_t column = 0; column < 8; ++column) {
            const auto &bin = wave.bins[row * 8 + column];
            const auto oscillate = [&bin](float phase) {
                const auto angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(phase * 10430.378f));
                const auto cosineAngle = static_cast<std::uint16_t>(0x4000 - (angle > 0x8000 ? 0x10000 - angle : angle));
                return WaveComplex{sin_fixed16(cosineAngle) * bin.amplitude, sin_fixed16(static_cast<std::uint16_t>(angle)) * bin.amplitude};
            };
            const auto first = oscillate(bin.phaseA);
            const auto second = oscillate(bin.phaseB);
            wave.real[row * 17 + column] = first.real;
            wave.imaginary[row * 17 + column] = first.imaginary;
            wave.real[(16 - row) * 17 + 16 - column] = first.real;
            wave.imaginary[(16 - row) * 17 + 16 - column] = -first.imaginary;
            wave.real[row * 17 + 16 - column] = second.real;
            wave.imaginary[row * 17 + 16 - column] = second.imaginary;
            wave.real[(16 - row) * 17 + column] = second.real;
            wave.imaginary[(16 - row) * 17 + column] = -second.imaginary;
        }
    }
    for (const unsigned index : {0U, 16U, 272U, 288U}) {
        wave.real[index] = 0.0f;
        wave.imaginary[index] = 0.0f;
    }
    std::array<WaveComplex, 16> line{};
    for (std::size_t row = 0; row < 16; ++row) {
        for (std::size_t column = 0; column < 16; ++column)
            line[column] = {.real = wave.real[row * 17 + column], .imaginary = wave.imaginary[row * 17 + column]};
        TransformWaveLine(line);
        for (std::size_t column = 0; column < 16; ++column) {
            wave.real[row * 17 + column] = line[column].real;
            wave.imaginary[row * 17 + column] = line[column].imaginary;
        }
    }
    for (std::size_t column = 0; column < 16; ++column) {
        for (std::size_t row = 0; row < 16; ++row)
            line[row] = {.real = wave.real[row * 17 + column], .imaginary = wave.imaginary[row * 17 + column]};
        TransformWaveLine(line);
        for (std::size_t row = 0; row < 16; ++row) {
            wave.real[row * 17 + column] = line[row].real;
            wave.imaginary[row * 17 + column] = line[row].imaginary;
        }
    }
    for (std::size_t row = 0; row < 16; ++row) {
        wave.real[row * 17 + 16] = wave.real[row * 17];
        wave.imaginary[row * 17 + 16] = wave.imaginary[row * 17];
    }
    for (std::size_t column = 0; column <= 16; ++column) {
        wave.real[272 + column] = wave.real[column];
        wave.imaginary[272 + column] = wave.imaginary[column];
    }
}

std::uint32_t PackWaveColour(float red, float green, float blue) {
    return 0xff000000U | (static_cast<std::uint32_t>(static_cast<std::int32_t>(red)) << 16U) |
        (static_cast<std::uint32_t>(static_cast<std::int32_t>(green)) << 8U) | static_cast<std::uint32_t>(static_cast<std::int32_t>(blue));
}

void UpdateWaveNormals() {
    const float ambient = wave.ambient * wave.lightAmbient;
    const float cell = wave.cellSize;
    for (std::size_t row = 0; row < 16; ++row) {
        const float rowPosition = static_cast<float>(row) * cell;
        const float dz = (rowPosition + cell) - rowPosition;
        for (std::size_t column = 0; column < 16; ++column) {
            const auto index = row * 17 + column;
            const float height = wave.real[index];
            const float columnPosition = static_cast<float>(column) * cell;
            const float dx = (columnPosition + cell) - columnPosition;
            Vector3 normal{-dz * (wave.real[index + 1] - height), dz * dx, -(wave.real[index + 17] - height) * dx};
            const float magnitude = std::sqrt((normal.y * normal.y + normal.x * normal.x) + normal.z * normal.z);
            if (magnitude > 0.0f) {
                const float inverse = 1.0f / magnitude;
                normal.x *= inverse;
                normal.y *= inverse;
                normal.z *= inverse;
            }
            const float dot = (wave.lightDirection.y * normal.y + normal.x * wave.lightDirection.x) + wave.lightDirection.z * normal.z;
            const float shade = std::fmax(0.0f, std::fmin(1.0f, ambient + (dot > 0.0f ? dot : 0.0f))) * 255.0f;
            auto &vertex = wave.vertices[index];
            vertex.height = height;
            vertex.normal = (static_cast<std::uint32_t>(static_cast<std::int32_t>(normal.x * 127.0f)) << 16U) |
                (static_cast<std::uint32_t>(static_cast<std::int32_t>(normal.y * 127.0f)) << 8U) |
                static_cast<std::uint32_t>(static_cast<std::int32_t>(normal.z * 127.0f));
            vertex.diffuse = PackWaveColour(shade * wave.diffuse.x, shade * wave.diffuse.y, shade * wave.diffuse.z);
            if (wave.lightingFlags & 1U) {
                const float highlight = std::fmin(1.0f, dot > 0.0f ? WavePower(dot, wave.specularExponent) : 0.0f) * (wave.specularIntensity * 255.0f);
                vertex.specular =
                    PackWaveColour(highlight * (wave.specular.x * 1.7f), highlight * (wave.specular.y * 1.7f), highlight * (wave.specular.z * 1.7f));
            } else
                vertex.specular = 0;
        }
        wave.vertices[row * 17 + 16] = wave.vertices[row * 17];
    }
    for (std::size_t column = 0; column <= 16; ++column)
        wave.vertices[272 + column] = wave.vertices[column];
}

bool ApplyAnimatedStripTree(MT7Model &model, std::size_t &cursor, std::int32_t parent, bool waveform) {
    bool succeeded = true;
    while (cursor < model.nodes.size() && model.nodes[cursor].parent == parent) {
        const std::size_t index = cursor++;
        MT7Node &node = model.nodes[index];
        if ((static_cast<std::uint32_t>(node.id) & 0x100000U) != 0) {
            while (cursor < model.nodes.size() && model.nodes[cursor].parent >= static_cast<std::int32_t>(index))
                ++cursor;
            continue;
        }
        const bool hasChild = cursor < model.nodes.size() && model.nodes[cursor].parent == static_cast<std::int32_t>(index);
        if (!node.xb01Offset && !hasChild)
            continue;
        if (waveform) {
            Matrix4x4_Push();
            Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
            if (node.rotation.z != 0.0f)
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.z)));
            if (node.rotation.y != 0.0f)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.y)));
            if (node.rotation.x != 0.0f)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.x)));
        }
        if (node.xb01Offset) {
            const bool updated = waveform ? UpdateLegacyWaveGeometry(model.source, node.xb01Offset,
                                                node.borrowedMesh ? const_cast<LegacyDrawResources &>(node.borrowedMesh->drawResources) : node.drawResources,
                                                wave.vertices, shenGetScaledDeltaTickTime(wave.scrollU), shenGetScaledDeltaTickTime(wave.scrollV)) :
                                            UpdateLegacyTextureUVs(model.source, node.xb01Offset,
                                                node.borrowedMesh ? const_cast<LegacyDrawResources &>(node.borrowedMesh->drawResources) : node.drawResources,
                                                textureAnimationU, textureAnimationV);
            succeeded = updated && succeeded;
        }
        if (hasChild)
            succeeded = ApplyAnimatedStripTree(model, cursor, static_cast<std::int32_t>(index), waveform) && succeeded;
        if (waveform)
            Matrix4x4::Pop(1);
    }
    return succeeded;
}

bool ApplyWaveReflectionTree(MT7Model &model, MT7Node *node, std::uint32_t diffuse, std::uint32_t specular) {
    bool succeeded = true;
    for (; node; node = node->runtimeSibling) {
        if ((static_cast<std::uint32_t>(node->id) & 0x100000U) || (!node->runtimeMesh && !node->runtimeChild))
            continue;
        (void)Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
        if (node->runtimeMesh)
            succeeded =
                UpdateLegacyWaveColours(model.source, node->xb01Offset,
                    node->borrowedMesh ? const_cast<LegacyDrawResources &>(node->borrowedMesh->drawResources) : node->drawResources, diffuse, specular) &&
                succeeded;
        if (node->runtimeChild)
            succeeded = ApplyWaveReflectionTree(model, node->runtimeChild, diffuse, specular) && succeeded;
        Matrix4x4::Pop(1);
    }
    return succeeded;
}

std::uint32_t PackWaveReflectionColour(const Vector3 &colour, float scale) {
    const std::uint32_t red = static_cast<std::uint32_t>(static_cast<std::int32_t>((colour.x * 255.0f) * scale));
    const std::uint32_t green = static_cast<std::uint32_t>(static_cast<std::int32_t>((colour.y * 255.0f) * scale));
    const std::uint32_t blue = static_cast<std::uint32_t>(static_cast<std::int32_t>((colour.z * 255.0f) * scale));
    return 0xff000000U | (red << 16U) | (green << 8U) | blue;
}
} // namespace

void GenerateFFT(float cellSize) {
    wave.specularExponent = 16.0f;
    wave.specularIntensity = 1.0f;
    wave.lightingFlags = 0;
    wave.cellSize = cellSize;
    wave.scrollU = 0.0f;
    wave.scrollV = 0.0f;
    for (unsigned index = 0; index < 8; ++index) {
        const unsigned reversed = ((index & 1U) << 2U) | (index & 2U) | ((index & 4U) >> 2U);
        const float angle = static_cast<float>(index) * 0.39269909f;
        wave.twiddles[reversed] = {std::sin(1.5707964f - std::fabs(angle)), -std::sin(angle)};
    }
    SetSharedPrngSeed(777);
    for (WaveBin &bin : wave.bins) {
        bin.phaseA = IncrementSharedPrngFloat() * 6.2831855f;
        bin.phaseB = IncrementSharedPrngFloat() * 6.2831855f;
        bin.speedA = bin.speedB = 0.0f;
    }
}

void *GenerateDistanceFalloffRowMajor(float exponent, std::span<std::byte, 0x100> kernel) {
    WriteWaveKernel(kernel.data(), 0, 0.0f);
    for (std::size_t index = 1; index < 64; ++index) {
        const int x = static_cast<int>(index % 8), y = static_cast<int>(index / 8);
        WriteWaveKernel(kernel.data(), index, 1.0f / WavePower(std::sqrt(static_cast<float>(x * x + y * y)), exponent));
    }
    return kernel.data();
}
void *GenerateDistanceFalloffKernel8x8(float exponent, std::span<std::byte, 0x100> kernel) {
    for (std::size_t index = 0; index < 64; ++index) {
        const int x = static_cast<int>(index % 8), y = static_cast<int>(index / 8);
        WriteWaveKernel(kernel.data(), index, WavePower(std::sqrt(static_cast<float>(x * x + y * y)), exponent));
    }
    return kernel.data();
}
void *FillFftKernel(float scale, void *kernel) {
    if (!kernel)
        return nullptr;
    WriteWaveKernel(kernel, 0, 0.0f);
    for (std::size_t index = 1; index < 64; ++index) {
        float value = scale * ReadWaveKernel(kernel, index);
        if (value < 0.00001f && value > -0.00001f)
            value = 0.0000099999997f;
        WriteWaveKernel(kernel, index, value);
    }
    return kernel;
}
void FillFftDistance(const void *kernel) {
    if (!kernel)
        return;
    for (std::size_t index = 0; index < 64; ++index)
        wave.bins[index].amplitude = ReadWaveKernel(kernel, index);
}
void ApplyFft(const void *first, const void *second) {
    if (!first || !second)
        return;
    for (std::size_t index = 0; index < 64; ++index) {
        wave.bins[index].speedA = ReadWaveKernel(first, index);
        wave.bins[index].speedB = ReadWaveKernel(second, index);
    }
}
void FinalizeFft(float scrollU, float scrollV) {
    wave.scrollU = scrollU;
    wave.scrollV = scrollV;
}
std::span<const LegacyWaveVertex, 289> GetWaveVertices() {
    return wave.vertices;
}
std::span<const float, 289> GetWaveHeights() {
    return wave.real;
}
WaveLightingView GetWaveLightingView() {
    return {wave.real, wave.imaginary, wave.lightingFlags, wave.cellSize, wave.lightAmbient, wave.lightDirection, wave.ambient, wave.diffuse, wave.specular,
        wave.specularExponent, wave.specularIntensity, wave.scrollU, wave.scrollV};
}
void SetWaveLighting(float ambient, const Vector3 &direction) {
    wave.lightAmbient = ambient;
    wave.lightDirection = direction;
}
// @verified - p
void BuildWaveLightingTable(std::uint32_t flags, float ambient, float, const Vector3 &diffuse, const Vector3 &specular, float exponent, float intensity) {
    wave.lightingFlags = flags;
    wave.ambient = ambient;
    wave.diffuse = diffuse;
    wave.specular = flags & 1U ? specular : Vector3{};
    wave.specularExponent = exponent;
    wave.specularIntensity = intensity;
}
bool ApplyWaveTextureMeshResource(std::int32_t slotIndex) {
    UpdateWaveSpectrum();
    UpdateWaveNormals();
    MT7Model *const model = const_cast<MT7Model *>(GetMapModelSlots().data()[slotIndex].reader);
    MT7Node *const node = model->nodes.data()->runtimeChild;
    return UpdateLegacyWaveGeometry(model->source, node->xb01Offset,
        node->borrowedMesh ? const_cast<LegacyDrawResources &>(node->borrowedMesh->drawResources) : node->drawResources, wave.vertices,
        shenGetScaledDeltaTickTime(wave.scrollU), shenGetScaledDeltaTickTime(wave.scrollV));
}

bool ApplyWaveTextureResource(std::int32_t slotIndex) {
    UpdateWaveSpectrum();
    UpdateWaveNormals();
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    if (slotIndex < 0 || static_cast<std::size_t>(slotIndex) >= slots.size())
        return false;
    MT7Model *const model = const_cast<MT7Model *>(slots[slotIndex].reader);
    if (!model)
        return false;
    std::size_t cursor = 0;
    return ApplyAnimatedStripTree(*model, cursor, -1, true);
}
bool ApplyWaveReflectionResource(std::int32_t slotIndex) {
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    if (slotIndex < 0 || static_cast<std::size_t>(slotIndex) >= slots.size())
        return false;
    MT7Model *const model = const_cast<MT7Model *>(slots[slotIndex].reader);
    if (!model)
        return false;
    return ApplyWaveReflectionTree(*model, model->nodes.data(), PackWaveReflectionColour(wave.diffuse, 1.0f), PackWaveReflectionColour(wave.specular, 0.7f));
}

// @verified - p
void SetTextureAnimationUV(float scrollU, float scrollV) {
    textureAnimationU = scrollU;
    textureAnimationV = scrollV;
}

bool ApplyTextureAnimationResource(std::int32_t slotIndex) {
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    if (slotIndex < 0 || static_cast<std::size_t>(slotIndex) >= slots.size())
        return false;
    MT7Model *const model = const_cast<MT7Model *>(slots[slotIndex].reader);
    if (!model)
        return false;
    std::size_t cursor = 0;
    return ApplyAnimatedStripTree(*model, cursor, -1, false);
}

namespace {
std::int32_t springTextureFrame{};
std::int32_t springTexturePreset{};
MT7Node *springTextureNodes[32]{};
std::uint32_t *springTextureSamples[4][32]{};
std::uint32_t *springTextureDestinations[4]{};
std::int32_t waterTextureFrame{};
std::int32_t waterTextureSlots[4]{};
MT7Node *waterTextureNodes[16]{};
MT7Node *waterReflectionNodes[16]{};
std::uint32_t waterTextureSamples[16]{};
std::uint32_t *waterTextureDestination{};
Vector3 waterTexturePositions[64]{};
std::int32_t waterTexturePositionCount{};
MT7Node *waterTextureExtraNode{};

std::uint32_t *FindSpringTextureCommand(MT7Node *node, std::int32_t ordinal) {
    std::uint32_t *const mesh = reinterpret_cast<std::uint32_t *>(node->runtimeMesh);
    std::uint32_t *strip = mesh + mesh[6] + 6;
    for (; *strip != 0; strip += *strip >> 8U) {
        for (std::uint32_t *command = strip + 5; *command != 0; command += *command >> 8U) {
            if (static_cast<std::uint8_t>(*command) != 11)
                continue;
            if (ordinal == 0)
                return command + 3;
            --ordinal;
        }
    }
    return nullptr;
}

// @verified - p
MT7Node *BindSpringTextureAnimation() {
    MT7Model *const model = const_cast<MT7Model *>(GetMapModelSlots().data()[springTexturePreset].reader);
    MT7Node *node = model->nodes.data()->runtimeChild;
    for (std::int32_t command = 0; command < 4; ++command)
        springTextureDestinations[command] = FindSpringTextureCommand(node, command);
    for (std::int32_t count = 0; node && count < 32; ++count, node = node->runtimeSibling) {
        const std::int16_t sample = static_cast<std::int16_t>((static_cast<std::int16_t>(node->id) - 152) % 32);
        springTextureNodes[sample] = node;
        springTextureSamples[0][sample] = FindSpringTextureCommand(node, 0);
        springTextureSamples[1][sample] = FindSpringTextureCommand(node, 1);
        springTextureSamples[2][sample] = sample == 1 ? FindSpringTextureCommand(node, 2) : springTextureSamples[1][sample];
        springTextureSamples[3][sample] = FindSpringTextureCommand(node, sample == 1 ? 3 : 2);
    }
    return node;
}
}

// @verified - p
MT7Node *InitializeSpringTextureAnimation(std::int32_t preset) {
    std::memset(springTextureNodes, 0, sizeof(springTextureNodes));
    springTextureFrame = 0;
    springTexturePreset = preset;
    return BindSpringTextureAnimation();
}

// @verified - p
std::uint32_t *AdvanceSpringTextureAnimation() {
    springTextureFrame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(springTextureFrame) + 1U);
    const std::int32_t sample = springTextureFrame % 32;
    *springTextureDestinations[0] = *springTextureSamples[0][sample];
    *springTextureDestinations[1] = *springTextureSamples[1][sample];
    *springTextureDestinations[2] = *springTextureSamples[2][sample];
    *springTextureDestinations[3] = *springTextureSamples[3][sample];
    return springTextureDestinations[3];
}

// @verified - p
MT7Node *InitializeWaterTextureAnimation(std::int32_t preset) {
    std::memset(waterTextureNodes, 0, sizeof(waterTextureNodes));
    std::memset(waterReflectionNodes, 0, sizeof(waterReflectionNodes));
    std::memset(waterTextureSamples, 0, sizeof(waterTextureSamples));
    waterTextureFrame = 0;
    for (std::int32_t index = 0; index < 4; ++index)
        waterTextureSlots[index] = waterTexturePresets[preset][index];
    MT7Model *model = const_cast<MT7Model *>(GetMapModelSlots().data()[waterTextureSlots[0]].reader);
    MT7Node *node = model->nodes.data()->runtimeChild;
    waterTextureDestination = FindSpringTextureCommand(node, 0);
    for (std::int32_t count = 0; node && count < 16; ++count, node = node->runtimeSibling) {
        const std::int32_t sample = (static_cast<std::int16_t>(node->id) - 152) % 16;
        waterTextureNodes[sample] = node;
        waterTextureSamples[sample] = *FindSpringTextureCommand(node, 0);
    }
    model = const_cast<MT7Model *>(GetMapModelSlots().data()[waterTextureSlots[1]].reader);
    node = model->nodes.data()->runtimeChild;
    waterTexturePositions[0] = node->position;
    waterTexturePositionCount = 1;
    for (node = node->runtimeSibling; node && waterTexturePositionCount < 64; node = node->runtimeSibling)
        waterTexturePositions[waterTexturePositionCount++] = node->position;
    model = const_cast<MT7Model *>(GetMapModelSlots().data()[waterTextureSlots[2]].reader);
    node = model->nodes.data()->runtimeChild;
    for (std::int32_t count = 0; node && count < 16; ++count, node = node->runtimeSibling)
        waterReflectionNodes[(static_cast<std::int16_t>(node->id) - 152) % 16] = node;
    if (preset == 5)
        waterTextureExtraNode = nullptr;
    else {
        model = const_cast<MT7Model *>(GetMapModelSlots().data()[waterTextureSlots[3]].reader);
        node = model->nodes.data()->runtimeChild;
        waterTextureExtraNode = node;
    }
    return node;
}

// @verified - p
void AdvanceWaterTextureAnimation() {
    waterTextureFrame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(waterTextureFrame) + 1U);
    const std::int32_t sample = waterTextureFrame % 16;
    MT7Model *const model = const_cast<MT7Model *>(GetMapModelSlots().data()[waterTextureSlots[0]].reader);
    if (model) {
        (void)ApplyWaveTextureMeshResource(waterTextureSlots[0]);
        *waterTextureDestination = waterTextureSamples[sample];
    }
    (void)ApplyWaveReflectionResource(waterTextureSlots[2]);
}

namespace {


std::uint32_t DispatchModelFlag2WithOptionalAltTransform(const MT7Model &model, const MT7Node &node) {
    const std::byte *const mesh = node.runtimeMesh ? node.runtimeMesh : model.source.data() + node.xb01Offset;
    std::uint32_t flags{};
    std::memcpy(&flags, mesh + 4, sizeof(flags));
    ScriptedMatrixFrame alternateFrame((flags & 0x40U) != 0);
    if (flags & 0x40U)
        Matrix4x4::SetActiveTransform(&objectCommandPlacementTransform);
    if (IsModelXB01(mesh)) {
        Vector3 centre{};
        float radius{};
        std::memcpy(&centre, mesh + 8, sizeof(centre));
        std::memcpy(&radius, mesh + 20, sizeof(radius));
        if (!GetModelCheck(&centre, radius))
            return 0;
    }
    return ExecuteLegacyDrawCommandStream(
        model.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources, LegacyModelShaderPath::Lambert, 1.0f);
}


std::uint32_t DrawWaterSetNode(const MT7Model &model, const MT7Node &node, const Vector3 &position, std::int32_t vertexColourType, bool refreshLights) {
    const std::int32_t previous = GetLegacyVertexColourType();
    SetLegacyVertexColourType(vertexColourType);
    Matrix4x4_Push();
    if (refreshLights)
        UpdateLegacyLightTransforms();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    const std::uint32_t draws = DispatchModelFlag2WithOptionalAltTransform(model, node);
    Matrix4x4::Pop(1);
    SetLegacyVertexColourType(previous);
    return draws;
}

bool HashWaterSetNode(const MT7Node *node) {
    Matrix4x4_PushActiveTransform();
    const bool hashed = node && d3t::HashModel(node, GetLegacyPerspectiveProjection());
    Matrix4x4::Pop(1);
    return hashed;
}



std::uint32_t DrawWaterReflectionNode(const MT7Model &model, const MT7Node &node) {
    for (std::uint32_t index = 0; index < 16; ++index)
        SetLegacyVertexColourStateEnabled(index, false);
    SetLegacyVertexColourStateEnabled(0, true);
    TemporaryMapWeatherLightingState lighting{};
    BeginTemporaryMapWeatherLighting(lighting);
    return DrawWaterSetNode(model, node, node.position, 2, true);
}
} // namespace



std::uint32_t DrawWaterTextureSet() {
    const std::int32_t sample = waterTextureFrame % 16;
    for (const std::int32_t slot : waterTextureSlots)
        SetMapModelSlotEnabled(slot, 0);
    TemporaryMapWeatherLightingState lighting{};
    BeginTemporaryMapWeatherLighting(lighting);
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    std::uint32_t draws = 0;
    if (waterTextureNodes[sample] && !HashWaterSetNode(waterTextureNodes[sample])) {
        const MT7Model &model = *slots[waterTextureSlots[0]].reader;
        for (std::int32_t index = 0; index < waterTexturePositionCount; ++index)
            draws += DrawWaterSetNode(model, *model.nodes.data()->runtimeChild, waterTexturePositions[index], 2, true);
    }
    MT7Node *const reflection = waterReflectionNodes[sample];
    if (reflection && !HashWaterSetNode(reflection))
        draws += DrawWaterSetNode(*slots[waterTextureSlots[2]].reader, *reflection, reflection->position, 2, true);
    if (waterTextureExtraNode)
        draws += DrawWaterSetNode(*slots[waterTextureSlots[3]].reader, *waterTextureExtraNode, waterTextureExtraNode->position, 2, true);
    EndTemporaryMapWeatherLighting(lighting);
    return draws;
}


std::uint32_t DrawWaterReflectionSet() {
    const std::int32_t sample = waterTextureFrame % 16;
    SetMapModelSlotEnabled(waterTextureSlots[2], 0);
    SetMapModelSlotEnabled(waterTextureSlots[3], 0);
    MT7Node *const reflection = waterReflectionNodes[sample];
    if (HashWaterSetNode(reflection))
        return 0;
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    std::uint32_t draws = 0;
    if (reflection)
        draws += DrawWaterReflectionNode(*slots[waterTextureSlots[2]].reader, *reflection);
    if (waterTextureExtraNode)
        draws += DrawWaterReflectionNode(*slots[waterTextureSlots[3]].reader, *waterTextureExtraNode);
    return draws;
}


std::uint32_t DrawSpringTextureNode() {
    SetMapModelSlotEnabled(springTexturePreset, 0);
    MT7Node *const node = springTextureNodes[1];
    if (!node || d3t::HashModel(node, GetLegacyPerspectiveProjection()))
        return 0;
    return DrawWaterSetNode(*GetMapModelSlots()[springTexturePreset].reader, *node, node->position, 1, false);
}


MT7Node *InitializeWaterReflectionSet() {
    std::memset(waterTextureNodes, 0, sizeof(waterTextureNodes));
    std::memset(waterReflectionNodes, 0, sizeof(waterReflectionNodes));
    std::memset(waterTextureSamples, 0, sizeof(waterTextureSamples));
    waterTextureSlots[2] = 24;
    waterTextureFrame = 0;
    waterTextureSlots[3] = 25;
    const std::span<const MapModelRuntimeSlot, 33> slots = GetMapModelSlots();
    std::int32_t count = 0;
    MT7Node *node = slots[24].reader ? slots[24].reader->nodes.data()->runtimeChild : nullptr;
    for (; node && count < 16; node = node->runtimeSibling, ++count)
        waterReflectionNodes[(static_cast<std::int16_t>(node->id) - 152) % 16] = node;
    waterReflectionNodes[1] = waterReflectionNodes[0];
    waterTextureExtraNode = slots[25].reader ? slots[25].reader->nodes.data()->runtimeChild : nullptr;
    return waterTextureExtraNode;
}


void AdvanceWaterReflectionSet() {
    waterTextureFrame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(waterTextureFrame) + 1U);
    (void)ApplyWaveReflectionResource(waterTextureSlots[2]);
}

namespace {
std::array<Vector3, 289> waterSurfacePositions{};
Vector3 waterSurfaceQuery{};
std::int32_t waterSurfaceCell{-1};
Vector3 floatingSurfaceOrigin{};
std::int32_t floatingSurfaceNodeCount{};

const std::int16_t floatingSurfacePreviousAngles[30][2]{};

bool ContainsWaterSurfaceQuad(std::int32_t first, std::int32_t stride) {
    if (waterSurfaceCell != -1)
        return false;
    const Vector3 &a = waterSurfacePositions[first];
    const Vector3 &b = waterSurfacePositions[first + stride];
    const Vector3 &c = waterSurfacePositions[first + 18 * stride];
    const Vector3 &d = waterSurfacePositions[first + 17 * stride];
    const float x = waterSurfaceQuery.x, z = waterSurfaceQuery.z;
    const float ab = (x - a.x) * (b.z - a.z) - (z - a.z) * (b.x - a.x);
    const float bc = (x - b.x) * (c.z - b.z) - (z - b.z) * (c.x - b.x);
    const float cd = (x - c.x) * (d.z - c.z) - (z - c.z) * (d.x - c.x);
    const float da = (x - d.x) * (a.z - d.z) - (z - d.z) * (a.x - d.x);
    return (ab >= 0.0f && bc >= 0.0f && cd >= 0.0f && da >= 0.0f) || (ab <= 0.0f && bc <= 0.0f && cd <= 0.0f && da <= 0.0f);
}

void SelectWaterSurfaceCell(std::int32_t first, std::int32_t stride) {
    if (waterSurfaceCell != -1)
        return;
    while (stride != 1) {
        stride >>= 1;
        if (ContainsWaterSurfaceQuad(first, stride))
            SelectWaterSurfaceCell(first, stride);
        if (ContainsWaterSurfaceQuad(first + stride, stride))
            SelectWaterSurfaceCell(first + stride, stride);
        if (ContainsWaterSurfaceQuad(first + 18 * stride, stride))
            SelectWaterSurfaceCell(first + 18 * stride, stride);
        first += 17 * stride;
        if (!ContainsWaterSurfaceQuad(first, stride) || waterSurfaceCell != -1)
            return;
    }
    waterSurfaceCell = first;
}

void SampleWaterSurfaceHeight() {
    waterSurfaceCell = -1;
    for (std::int32_t first : {0, 8, 144, 136})
        if (ContainsWaterSurfaceQuad(first, 8))
            SelectWaterSurfaceCell(first, 8);
    if (waterSurfaceCell == -1) {
        for (std::int32_t row = 0; row < 289 && waterSurfaceCell == -1; row += 17) {
            for (std::int32_t column = 0; column < 17; ++column) {
                if (ContainsWaterSurfaceQuad(row + column, 1)) {
                    waterSurfaceCell = row + column;
                    break;
                }
            }
        }
    }
    const std::int32_t index = waterSurfaceCell;

    const float x = waterSurfaceQuery.x, z = waterSurfaceQuery.z, dy = 0.0f - waterSurfaceQuery.y;
    const Vector3 &near = waterSurfacePositions[index];
    const Vector3 &far = waterSurfacePositions[index + 18];
    const float nearDistance = std::sqrt(((near.x - x) * (near.x - x) + dy * dy) + (near.z - z) * (near.z - z));
    const float farDistance = std::sqrt((dy * dy + (far.x - x) * (far.x - x)) + (far.z - z) * (far.z - z));
    const std::int32_t originIndex = nearDistance > farDistance ? index + 18 : index;
    const Vector3 &origin = waterSurfacePositions[originIndex];
    const Vector3 &first = waterSurfacePositions[index + 1];
    const Vector3 &second = waterSurfacePositions[index + 17];
    const float height = wave.vertices[originIndex].height;
    const float firstHeight = wave.vertices[index + 1].height, secondHeight = wave.vertices[index + 17].height;
    waterSurfaceQuery.y = height -
        (((z - origin.z) * ((firstHeight - height) * (first.x - origin.x) - (second.x - origin.x) * (secondHeight - height)) +
             (x - origin.x) * ((second.z - origin.z) * (secondHeight - height) - (firstHeight - height) * (first.z - origin.z))) /
            ((second.x - origin.x) * (first.z - origin.z) - (second.z - origin.z) * (first.x - origin.x)));
}

// @verified - p
void UpdateFloatingSurfaceNode(MT7Node *node) {
    for (; node; node = node->runtimeSibling) {
        if (node->runtimeMesh) {
            const float sine = sin_fixed16(static_cast<std::uint16_t>(node->rotation.y));
            const float cosine = sin_fixed16(fixed16_cosine_angle(static_cast<std::uint16_t>(node->rotation.y)));
            float heights[3];
            for (std::int32_t sample = 0; sample < 3; ++sample) {
                const Vector2 &point = floatingSurfaceSamplePoints[sample];
                const float x = point.x * cosine + point.y * sine + node->position.x;
                const float z = point.y * cosine - point.x * sine + node->position.z;
                waterSurfaceQuery = {.x = x - floatingSurfaceOrigin.x, .y = 0.0f, .z = z - floatingSurfaceOrigin.z};
                waterSurfaceCell = -1;
                SampleWaterSurfaceHeight();
                heights[sample] = waterSurfaceQuery.y + floatingSurfaceOrigin.y;
            }
            if (floatingSurfaceNodeCount < 30) {
                const std::int16_t pitch = static_cast<std::int16_t>(
                    FixedAtan2((heights[2] + heights[1]) * 0.5f - heights[0], floatingSurfaceSamplePoints[0].y - floatingSurfaceSamplePoints[1].y));
                const std::int16_t roll =
                    static_cast<std::int16_t>(FixedAtan2(heights[1] - heights[2], floatingSurfaceSamplePoints[1].x - floatingSurfaceSamplePoints[2].x));
                node->rotation.z = (roll + floatingSurfacePreviousAngles[floatingSurfaceNodeCount][0]) / 2;
                node->rotation.x = (pitch + floatingSurfacePreviousAngles[floatingSurfaceNodeCount][1]) / 2;
                ++floatingSurfaceNodeCount;
                node->position.y = ((heights[1] + heights[0]) + heights[2]) / 3.0f;
            }
        }
        if (node->runtimeChild)
            UpdateFloatingSurfaceNode(node->runtimeChild);
    }
}
}

// @verified - p
void UpdateFloatingSurfaceModels(std::int32_t surfaceSlot, const FloatingSurfaceBinding *bindings) {
    MT7Model *const surface = const_cast<MT7Model *>(GetMapModelSlots().data()[surfaceSlot].reader);
    MT7Node *const mesh = surface->nodes.data()->runtimeChild;
    if (!ReadLegacyWaveGridPositions(surface->source, mesh->xb01Offset,
            mesh->borrowedMesh ? const_cast<LegacyDrawResources &>(mesh->borrowedMesh->drawResources) : mesh->drawResources, waterSurfacePositions))
        std::abort();
    for (const FloatingSurfaceBinding *binding = bindings; binding->slot >= 0; ++binding) {
        floatingSurfaceOrigin = binding->origin;
        floatingSurfaceNodeCount = 0;
        MT7Model *const model = const_cast<MT7Model *>(GetMapModelSlots().data()[binding->slot].reader);
        UpdateFloatingSurfaceNode(model->nodes.data());
    }
}

namespace {

constexpr std::array<std::uint32_t, 413> fixedShadowMesh0{{
    0x31304258U, 0x00000000U, 0x00000000U, 0x3f000000U, 0x32800000U, 0x3f000001U, 0x00000008U, 0x00000091U,
    0x00000001U, 0x00000001U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00008800U, 0x00000000U,
    0x00000000U, 0x00000000U, 0x00000000U, 0x00000401U, 0x00000001U, 0x00200000U, 0x00000000U, 0x00000202U,
    0x00000000U, 0x00000f04U, 0x3f36db6eU, 0x3f36db6eU, 0x3f36db6eU, 0x3f800000U, 0x3f333333U, 0x3f333333U,
    0x3f333333U, 0x3f800000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x3f800000U, 0x00000000U, 0x3f800000U,
    0x00000216U, 0x00000000U, 0x0000040bU, 0x00000001U, 0x0000ffffU, 0x00000000U, 0x0000060dU, 0x00000004U,
    0x00010000U, 0x00010100U, 0x00010300U, 0x00010400U, 0x0000040eU, 0x00000001U, 0x00000600U, 0x00000000U,
    0x00005d10U, 0x00000005U, 0x000000b4U, 0x00020000U, 0x00010001U, 0x00030002U, 0x00000004U, 0x00050005U,
    0x00010000U, 0x00040006U, 0x00070007U, 0x00050004U, 0x00060008U, 0x00090009U, 0x00070006U, 0x00030001U,
    0x000a000aU, 0x000b0003U, 0x00010005U, 0x000c000cU, 0x000a0001U, 0x00050007U, 0x000d000dU, 0x000c0005U,
    0x00070009U, 0x000e000eU, 0x000d0007U, 0x000b000aU, 0x000f000fU, 0x0010000bU, 0x000a000cU, 0x00110011U,
    0x000f000aU, 0x000c000dU, 0x00120012U, 0x0011000cU, 0x000d000eU, 0x00130013U, 0x0012000dU, 0x0010000fU,
    0x00140014U, 0x00150010U, 0x000f0011U, 0x00160016U, 0x0014000fU, 0x00110012U, 0x00170017U, 0x00160011U,
    0x00120013U, 0x00180018U, 0x00170012U, 0x00150014U, 0x00190019U, 0x001a0015U, 0x00140016U, 0x001b001bU,
    0x00190014U, 0x00160017U, 0x001c001cU, 0x001b0016U, 0x00170018U, 0x001d001dU, 0x001c0017U, 0x001a0019U,
    0x00000000U, 0x0002001aU, 0x0019001bU, 0x00040004U, 0x00000019U, 0x001b001cU, 0x00060006U, 0x0004001bU,
    0x001c001dU, 0x00080008U, 0x0006001cU, 0x0008001dU, 0x0002001eU, 0x001f001aU, 0x001d0018U, 0x001a001eU,
    0x001f0015U, 0x00180013U, 0x0015001eU, 0x001f0010U, 0x0013000eU, 0x0010001eU, 0x001f000bU, 0x000e0009U,
    0x000b001eU, 0x001f0003U, 0x00090008U, 0x0003001eU, 0x001f0002U, 0x00000000U, 0x00000000U, 0x00000000U,
    0x00010401U, 0x00000001U, 0x01120020U, 0x00000400U, 0x3eddb3d7U, 0x3f400000U, 0x00000000U, 0x3f5cb228U,
    0x3f01ba55U, 0xb3deb6b5U, 0x00000000U, 0x00000001U, 0x3e5db3dfU, 0x3f400000U, 0x3ec00000U, 0x3edcb22bU,
    0x3f01ba55U, 0x3f3f20d4U, 0x00000000U, 0x00000001U, 0x3e800001U, 0x3f6ed9ecU, 0x00000000U, 0x3efc11a0U,
    0x3f5ed366U, 0xb3800001U, 0x00000000U, 0x00000001U, 0x3e000007U, 0x3f6ed9ecU, 0x3e5db3d7U, 0x3e7c11a4U,
    0x3f5ed367U, 0x3eda4c48U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x3f000000U, 0x00000000U, 0x3f800000U,
    0x33000000U, 0xb4000000U, 0x00000000U, 0x00000001U, 0x3e800005U, 0x3f000000U, 0x3eddb3d6U, 0x3f000002U,
    0x00000000U, 0x3f5db3d6U, 0x00000000U, 0x00000001U, 0x3eddb3d9U, 0x3e800000U, 0x00000000U, 0x3f5cb228U,
    0xbf01ba53U, 0xb31479cdU, 0x00000000U, 0x00000001U, 0x3e5db3dfU, 0x3e800000U, 0x3ebfffffU, 0x3edcb22bU,
    0xbf01ba54U, 0x3f3f20d5U, 0x00000000U, 0x00000001U, 0x3e800001U, 0x3d8930a4U, 0x00000000U, 0x3efc11a0U,
    0xbf5ed366U, 0xb3130e7eU, 0x00000000U, 0x00000001U, 0x3e000005U, 0x3d8930a4U, 0x3e5db3d6U, 0x3e7c11a4U,
    0xbf5ed366U, 0x3eda4c4aU, 0x00000000U, 0x00000001U, 0xbe5db3d5U, 0x3f400000U, 0x3ec00002U, 0xbedcb222U,
    0x3f01ba55U, 0x3f3f20d6U, 0x00000000U, 0x00000001U, 0xbdfffffbU, 0x3f6ed9ecU, 0x3e5db3dbU, 0xbe7c1198U,
    0x3f5ed367U, 0x3eda4c4cU, 0x00000000U, 0x00000001U, 0xbe7ffffbU, 0x3f000000U, 0x3eddb3d9U, 0xbefffffaU,
    0xb313cd3aU, 0x3f5db3d9U, 0x00000000U, 0x00000001U, 0xbe5db3d3U, 0x3e800000U, 0x3ec00002U, 0xbedcb224U,
    0xbf01ba53U, 0x3f3f20d9U, 0x00000000U, 0x00000001U, 0xbdfffffbU, 0x3d8930a4U, 0x3e5db3d9U, 0xbe7c119cU,
    0xbf5ed365U, 0x3eda4c4cU, 0x00000000U, 0x00000001U, 0xbeddb3d9U, 0x3f400000U, 0x33800000U, 0xbf5cb228U,
    0x3f01ba55U, 0x34000001U, 0x00000000U, 0x00000001U, 0xbe800001U, 0x3f6ed9ecU, 0x33400000U, 0xbefc119fU,
    0x3f5ed366U, 0x33800001U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x3f000000U, 0x33a00000U, 0xbf800000U,
    0x00000000U, 0x34200000U, 0x00000000U, 0x00000001U, 0xbeddb3d9U, 0x3e800000U, 0x33a00000U, 0xbf5cb228U,
    0xbf01ba54U, 0x34200000U, 0x00000000U, 0x00000001U, 0xbe800001U, 0x3d8930a4U, 0x33200000U, 0xbefc119fU,
    0xbf5ed366U, 0x33dc95bdU, 0x00000000U, 0x00000001U, 0xbe5db3dbU, 0x3f400000U, 0xbec00000U, 0xbedcb22aU,
    0x3f01ba55U, 0xbf3f20d5U, 0x00000000U, 0x00000001U, 0xbe000003U, 0x3f6ed9ecU, 0xbe5db3d8U, 0xbe7c11a4U,
    0x3f5ed365U, 0xbeda4c4aU, 0x00000000U, 0x00000001U, 0xbe800003U, 0x3f000000U, 0xbeddb3d7U, 0xbf000002U,
    0x00000000U, 0xbf5db3d7U, 0x00000000U, 0x00000001U, 0xbe5db3dbU, 0x3e800000U, 0xbec00000U, 0xbedcb22bU,
    0xbf01ba53U, 0xbf3f20d6U, 0x00000000U, 0x00000001U, 0xbe000003U, 0x3d8930a4U, 0xbe5db3d7U, 0xbe7c11a4U,
    0xbf5ed365U, 0xbeda4c49U, 0x00000000U, 0x00000001U, 0x3e5db3d7U, 0x3f400000U, 0xbec00000U, 0x3edcb226U,
    0x3f01ba54U, 0xbf3f20d7U, 0x00000000U, 0x00000001U, 0x3e000001U, 0x3f6ed9ecU, 0xbe5db3daU, 0x3e7c119dU,
    0x3f5ed367U, 0xbeda4c4cU, 0x00000000U, 0x00000001U, 0x3e7fffffU, 0x3f000000U, 0xbeddb3d8U, 0x3f000000U,
    0x33800000U, 0xbf5db3d8U, 0x00000000U, 0x00000001U, 0x3e5db3d7U, 0x3e800000U, 0xbec00001U, 0x3edcb226U,
    0xbf01ba53U, 0xbf3f20d7U, 0x00000000U, 0x00000001U, 0x3dffffffU, 0x3d8930a4U, 0xbe5db3d8U, 0x3e7c119fU,
    0xbf5ed366U, 0xbeda4c4cU, 0x00000000U, 0x00000001U, 0x00000001U, 0x00000000U, 0x00000000U, 0x00000000U,
    0xbf800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0x00000001U, 0x3f800000U, 0x00000000U, 0x00000000U,
    0x3f800000U,
    0x00000000U,
    0x00000000U,
    0x00000001U,
    0x00000000U,
}};

constexpr std::array<std::uint32_t, 285> fixedShadowMesh1{{
    0x31304258U, 0x00000000U, 0x00000000U, 0x3f000000U, 0x00000000U, 0x3f5db3d7U, 0x00000008U, 0x00000051U,
    0x00000001U, 0x00000001U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00004300U, 0x00000000U,
    0x00000000U, 0x00000000U, 0x00000000U, 0x00000401U, 0x00000001U, 0x00200000U, 0x00000000U, 0x00000202U,
    0x00000000U, 0x00000f04U, 0x3f36db6eU, 0x3f36db6eU, 0x3f36db6eU, 0x3f800000U, 0x3f333333U, 0x3f333333U,
    0x3f333333U, 0x3f800000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x3f800000U, 0x00000000U, 0x3f800000U,
    0x00000216U, 0x00000000U, 0x00000309U, 0x00000001U, 0x1d000042U, 0x0000040bU, 0x00000001U, 0x0000ffffU,
    0x00000000U, 0x0000060dU, 0x00000004U, 0x00010000U, 0x00010100U, 0x00010300U, 0x00010400U, 0x0000040eU,
    0x00000001U, 0x00000600U, 0x00000000U, 0x00001510U, 0x00000005U, 0x00000024U, 0x00020000U, 0x00010001U,
    0x00030002U, 0x00060004U, 0x00050005U, 0x00070006U, 0x000a0008U, 0x00090009U, 0x000b000aU, 0x000e000cU,
    0x000d000dU, 0x000f000eU, 0x00120010U, 0x00110011U, 0x00130012U, 0x00160014U, 0x00150015U, 0x00170016U,
    0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
    0x0000c401U, 0x00000001U, 0x01120020U, 0x00000300U, 0xbf000001U, 0x3f800000U, 0x3f000000U, 0x00000000U,
    0x3f800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x3f800000U, 0xbf000000U, 0x00000000U,
    0x3f800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x3f800000U, 0x3f000000U, 0x00000000U,
    0x3f800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x3f800000U, 0xbf000000U, 0x00000000U,
    0x3f800000U, 0x80000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x00000000U, 0x3f000000U, 0x00000000U,
    0xbf800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x00000000U, 0xbf000000U, 0x00000000U,
    0xbf800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x00000000U, 0x3f000000U, 0x00000000U,
    0xbf800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x00000000U, 0xbf000000U, 0x00000000U,
    0xbf800000U, 0x00000000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x00000000U, 0xbf000000U, 0x00000000U,
    0x00000000U, 0xbf800000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x00000000U, 0xbf000000U, 0x00000000U,
    0x00000000U, 0xbf800000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x3f800000U, 0xbf000000U, 0x00000000U,
    0x00000000U, 0xbf800000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x3f800000U, 0xbf000000U, 0x80000000U,
    0x80000000U, 0xbf800000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x00000000U, 0xbf000000U, 0x3f800000U,
    0x00000000U, 0x80000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x00000000U, 0x3f000000U, 0x3f800000U,
    0x00000000U, 0x80000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x3f800000U, 0xbf000000U, 0x3f800000U,
    0x00000000U, 0x80000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x3f800000U, 0x3f000000U, 0x3f800000U,
    0x00000000U, 0x00000000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x00000000U, 0x3f000000U, 0x00000000U,
    0x00000000U, 0x3f800000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x00000000U, 0x3f000000U, 0x00000000U,
    0x00000000U, 0x3f800000U, 0x00000000U, 0x00000001U, 0x3f000001U, 0x3f800000U, 0x3f000000U, 0x00000000U,
    0x00000000U, 0x3f800000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x3f800000U, 0x3f000000U, 0x80000000U,
    0x00000000U, 0x3f800000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x00000000U, 0x3f000000U, 0xbf800000U,
    0x80000000U, 0x80000000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x00000000U, 0xbf000000U, 0xbf800000U,
    0x80000000U, 0x80000000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x3f800000U, 0x3f000000U, 0xbf800000U,
    0x80000000U, 0x80000000U, 0x00000000U, 0x00000001U, 0xbf000001U, 0x3f800000U, 0xbf000000U, 0xbf800000U,
    0x80000000U,
    0x00000000U,
    0x00000000U,
    0x00000001U,
    0x00000000U,
}};
struct ShadowModelResource {
    void *allocation{};
    MT7Model model;
};
std::array<ShadowModelResource, 3> shadowDataModels;
ShadowModelResource sharedShadowModel;
std::vector<std::byte> sharedShadowOriginalBytes;
std::array<LegacyDrawResources, 2> fixedShadowResources;
std::array<ScenarioShadowModelCacheEntry, scenarioShadowModelCacheCapacity> scenarioShadowModelCache;
std::int32_t scenarioShadowModelCacheCount;
std::vector<std::unique_ptr<ShadowModelResource>> scenarioShadowModels;

void ReleaseShadowModel(ShadowModelResource &resource, bool releaseBacking = true) {
    if (!resource.model.source.empty())
        ReleaseModelFormat7(resource.model);
    if (releaseBacking && resource.allocation)
        HLib::FreeTaskData(resource.allocation);
    resource.allocation = nullptr;
}

// SYD_S.MT7 loads through LoadFile_Wrapper into the heap head; ReadMT7 reads the SYD_xS models through FileRead, which
// takes them from the heap tail.
bool LoadShadowFile(ShadowModelResource &resource, const char *path, shen_name token, bool retainOriginal, bool fromTail) {
    ReleaseShadowModel(resource);
    const auto size = ArchiveData_GetFileSize(path);
    if (!size || size > (std::numeric_limits<std::size_t>::max)())
        return false;
    resource.allocation = fromTail ? HLib::AllocateStaticOrDynamicBlock(size, token) : HLib::AllocateTaskData(size, token);
    if (!resource.allocation)
        return false;
    if (!ArchiveData_Read(path, 0, resource.allocation, size)) {
        ReleaseShadowModel(resource);
        return false;
    }
    if (retainOriginal) {
        const auto *bytes = static_cast<const std::byte *>(resource.allocation);
        const auto *const allocation = static_cast<const HLib::TaskData *>(resource.allocation) - 1;
        const std::size_t payloadSize = static_cast<std::size_t>(allocation->contentSize - sizeof(HLib::TaskData));
        sharedShadowOriginalBytes.assign(bytes, bytes + payloadSize);
    }
    if (ReadModelFormat7({static_cast<const std::byte *>(resource.allocation), static_cast<std::size_t>(size)}, path, resource.model, 0))
        return true;
    ReleaseShadowModel(resource);
    return false;
}

MT7Model *LoadScenarioShadowFile(const char *directory, const char *filename, shen_name token) {
    std::uint32_t size{};
    void *const allocation = LoadLegacySceneFile(directory, filename, token, &size);
    if (!allocation)
        return nullptr;
    std::unique_ptr<ShadowModelResource> resource(new (std::nothrow) ShadowModelResource{});
    if (!resource)
        return nullptr;
    resource->allocation = allocation;
    char path[261];
    std::snprintf(path, sizeof(path), "%s/%s", directory, filename);
    if (!ReadModelFormat7({static_cast<const std::byte *>(allocation), size}, path, resource->model, 0)) {
        ReleaseShadowModel(*resource, false);
        return nullptr;
    }
    MT7Model *const model = &resource->model;
    scenarioShadowModels.push_back(std::move(resource));
    return model;
}

bool PrepareFixedShadowMeshes() {


    const bool first = PrepareLegacyDrawResources(std::as_bytes(std::span(fixedShadowMesh0)), 0, fixedShadowResources[0]);
    const bool second = PrepareLegacyDrawResources(std::as_bytes(std::span(fixedShadowMesh1)), 0, fixedShadowResources[1]);
    return first && second;
}

void RetireSharedShadowReader() {
    // SHDS backing remains owned by its HLib arena; replacement only retires decoded resources.
    ReleaseShadowModel(sharedShadowModel, false);
    for (LegacyDrawResources &resources : fixedShadowResources)
        ReleaseLegacyDrawResources(resources);
}
} // namespace

MT7Model *LoadShadowSprite() {
    scenarioShadowModelCacheCount = 0;
    scenarioShadowModelCache = {};
    shadowSprite = LoadScenarioShadowFile("misc", "KAGE200G.MT7", MEMORY_SHADOW_SPRITE);
    return shadowSprite;
}

MT7Model *CreateMT7(const char *image) {
    char directory[64], filename[16]{};
    const bool sceneModel = image[0] == '@';
    const bool areaModel = image[0] == '$';
    const char *const stem = image + (sceneModel || areaModel ? 1 : 0);
    if (sceneModel)
        std::snprintf(directory, sizeof(directory), "scene/%02d/model", g_actNumber);
    else if (areaModel)
        std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_actNumber, g_currentMap.characters[0], g_currentMap.characters[1],
            g_currentMap.characters[2], g_currentMap.characters[3]);
    else
        std::strcpy(directory, "/model/chara");
    std::snprintf(filename, sizeof(filename), "%s.MT7", stem);
    if (GetPakLoadParams())
        return LoadGameModel(pakLoadGetNext(), filename);
    for (std::int32_t index = 0; index < scenarioShadowModelCacheCount; ++index) {
        const ScenarioShadowModelCacheEntry &entry = scenarioShadowModelCache[static_cast<std::size_t>(index)];
        if (std::strcmp(entry.filename, filename))
            continue;
        if (entry.model)
            return entry.model;
        break;
    }
    MT7Model *const model = LoadScenarioShadowFile(directory, filename, shen_name{make_shen_name(stem[0], stem[1], stem[2], 'S')});
    if (scenarioShadowModelCacheCount < static_cast<std::int32_t>(scenarioShadowModelCacheCapacity)) {
        ScenarioShadowModelCacheEntry &entry = scenarioShadowModelCache[static_cast<std::size_t>(scenarioShadowModelCacheCount++)];
        entry.model = model;
        std::memcpy(entry.filename, filename, sizeof(filename));
    }
    return model;
}

void ResetShadowData() {
    // ResetShadowData retires exactly these three readers before their SVMD backing.
    for (auto &resource : shadowDataModels)
        ReleaseShadowModel(resource);
}

constexpr const char *shadowDataPaths[]{"/misc/SYD_NS.MT7", "/misc/SYD_RS.MT7", "/misc/SYD_SS.MT7"};

// Each module change reloads the three models after ResetShadowData freed them.
bool Read_SYDNSRSSS() {
    bool succeeded = true;
    for (std::size_t index = 0; index < shadowDataModels.size(); ++index)
        succeeded = LoadShadowFile(shadowDataModels[index], shadowDataPaths[index], SHEN_NAME(SVMD), false, true) && succeeded;
    return succeeded;
}

bool LoadSYD() {
    characterShadowRuntime = g_characterShadowRuntime;
    characterShadowRuntimeIndex = 0;
    // LoadSYD reads all three independently, even if an earlier file is absent.
    bool succeeded = true;
    for (std::size_t index = 0; index < shadowDataModels.size(); ++index)
        succeeded = LoadShadowFile(shadowDataModels[index], shadowDataPaths[index], SHEN_NAME(SVMD), false, true) && succeeded;
    characterShadowRenderParameters = {0, 1000, 1000, 1000};
    characterShadowOpacityScale = 0.5f;
    characterShadowPointLightMode = 0;
    nullsub(characterShadowRuntime, characterShadowRenderParameters.data());
    characterUngroundedShadowMode = 0;
    characterShadowMode = 0;
    return succeeded;
}

// @verified - p
shen_name MVTR_Reader() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    (void)FindMapInfoChunk("MVTR", control->mapInfo);
    characterShadowOpacityScale = 0.5f;
    characterShadowMode = 0;
    characterShadowPointLightMode = 0;
    characterShadowStrongestOnly = 1;
    if (g_currentMap == AREA_QF40 || g_currentMap == AREA_QF39)
        characterShadowStrongestOnly = 0;
    return g_currentMap;
}

void RetireSharedShadowModel() {
    RetireSharedShadowReader();
    for (const std::unique_ptr<ShadowModelResource> &resource : scenarioShadowModels)
        ReleaseShadowModel(*resource, false);
    scenarioShadowModels.clear();
    shadowSprite = nullptr;
}

void *LoadSYD_S() {
    RetireSharedShadowReader();
    if (!LoadShadowFile(sharedShadowModel, "/misc/SYD_S.MT7", MEMORY_SHARED_SHADOW, true, false))
        return nullptr;
    if (!PrepareFixedShadowMeshes()) {
        RetireSharedShadowReader();
        return nullptr;
    }
    return sharedShadowModel.allocation;
}

void *AllocateSHDS() {

    RetireSharedShadowReader();
    if (sharedShadowOriginalBytes.empty())
        return nullptr;
    sharedShadowModel.allocation = HLib::AllocateTaskData(sharedShadowOriginalBytes.size(), MEMORY_SHARED_SHADOW);
    if (!sharedShadowModel.allocation)
        return nullptr;
    std::memcpy(sharedShadowModel.allocation, sharedShadowOriginalBytes.data(), sharedShadowOriginalBytes.size());
    if (!ReadModelFormat7(
            {static_cast<const std::byte *>(sharedShadowModel.allocation), sharedShadowOriginalBytes.size()}, "/misc/SYD_S.MT7", sharedShadowModel.model, 0) ||
        !PrepareFixedShadowMeshes()) {
        RetireSharedShadowReader();
        return nullptr;
    }
    return sharedShadowModel.allocation;
}

const MT7Model *GetSHDS() {
    return sharedShadowModel.allocation ? &sharedShadowModel.model : nullptr;
}

const MT7Model *GetShadowDataModel(std::size_t index) {
    if (index >= shadowDataModels.size() || !shadowDataModels[index].allocation)
        return nullptr;
    return &shadowDataModels[index].model;
}

std::uint32_t DrawFixedShadowMesh(std::size_t index, float alpha) {
    if (index >= fixedShadowResources.size())
        return 0;
    const auto bytes = index == 0 ? std::as_bytes(std::span(fixedShadowMesh0)).first(sizeof(fixedShadowMesh0)) :
                                    std::as_bytes(std::span(fixedShadowMesh1)).first(sizeof(fixedShadowMesh1));
    return ExecuteLegacyDrawCommandStream(bytes, 0, fixedShadowResources[index], LegacyModelShaderPath::Lambert, alpha);
}

} // namespace shenmue::s2
