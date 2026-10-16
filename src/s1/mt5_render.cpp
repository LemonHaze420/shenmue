#include "s1/mt5_render.hpp"
#include "s1/d3t/namespace.hpp"

#include "s1/task/char.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/came.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/obje.hpp"
#include "s1/face.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/modern_lighting.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/kuti.hpp"
#include "s1/task/pack.hpp"
#include "s1/scene.hpp"
#include "s1/simple_shader.hpp"
#include "s1/texture_cache.hpp"
#include "s1/render_array.hpp"
#include "s1/skinning_cache.hpp"
#ifdef SHENMUE_RT_ARCADE
#include "s1/enhancements/rt_arcade.hpp"
#endif

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <climits>
#include <limits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <unordered_map>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#endif

namespace shenmue::s1 {
void MT5_ReadMeshStrip(void **node, const std::int32_t *textureIds, std::int32_t bufferHandle);
const void *MT5_SetVertexDataOverride(const void *vertices);

extern std::uint8_t g_debugUnknown3;
std::uint8_t g_forcePrimaryCachedModel = 0;

namespace {

#pragma pack(push, 1)
struct Mt5Node {
    std::uint16_t nodeKind;
    std::uint16_t nodeFlags;
    std::int32_t sceneBindingOffset;
    std::int32_t rotX;
    std::int32_t rotY;
    std::int32_t rotZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    float posX;
    float posY;
    float posZ;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t parentOffset;
    std::int32_t transformOffset;
    std::int32_t attachmentOffset;
};

struct Mt5SceneBinding {
    std::uint32_t flags;
    std::uint32_t sceneEntityOffset;
    std::uint32_t auxiliaryOffset;
    std::uint32_t stripCommandOffset;
};
#pragma pack(pop)

static_assert(sizeof(Mt5Node) == 0x40);
static_assert(offsetof(Mt5Node, transformOffset) == 0x38);
static_assert(offsetof(Mt5Node, attachmentOffset) == 0x3c);
static_assert(sizeof(Mt5SceneBinding) == 0x10);

struct MaterialState {
    std::uint32_t sourceBlend = 0;
    std::uint32_t destinationBlend = 0;
    std::uint32_t renderClass = 0;
    std::uint32_t lit0 = 0;
    std::uint32_t lit1 = 0;
    std::uint32_t materialMode = 0;
    std::uint32_t ignoreAlpha = 0;
    std::uint32_t alphaTestEnabled = 0;
    float alphaReference = 0.0f;
    std::uint32_t alphaFunction = 0;
};

struct RenderInputLayoutIndex {
    std::int32_t inputLayoutIndex;
    std::int32_t vertexShaderIndex;
    std::int32_t pixelShaderIndex;
};

std::uint32_t g_currentInputLayout = 0;
std::uint32_t g_drawCount = 0;
std::uint32_t g_visitedNodeCount = 0;
std::uint32_t g_bindingCount = 0;
std::uint32_t g_preparedNodeCount = 0;
std::uint32_t g_missingSceneEntryCount = 0;
CharacterDrawDiagnostics g_characterDrawDiagnostics{};
bool g_recordingRyoDraw = false;
bool g_capturedRyoTransform = false;
bool g_alphaMaskedDepthPass = false;
bool g_characterShadowPass = false;
RenderInputLayoutIndex g_tmpInputLayoutIndex{7, 7, 0};
Matrix4x4 g_mt5RootTransform{};
std::byte *g_activeMt5Base = nullptr;
const std::int32_t *g_mt5ReaderTextureIds = nullptr;
Character *g_activeRiggedCharacter = nullptr;
Mt5Node *g_syncedFaceRoot = nullptr;
Mt5Node *g_syncedFaceSecondary = nullptr;
bool g_submittingSyncedHand = false;



bool g_characterHandlerNodeWalk = false;

#pragma pack(push, 1)
struct OsageNodeRecord {
    std::uint32_t state;
    std::uint32_t reserved04;
    Mt5Node *node;
    std::int32_t nodeKind;
    std::uint32_t nodeHash;
    std::uint32_t slotIndex;
    std::uint32_t reserved1c;
};

struct OsageRenderSlot {
    std::uint32_t cacheHandle;
    std::uint32_t reserved04;
    Mt5Node *node;
    std::uint32_t baseHash;
    std::uint32_t attachmentValue;
    std::uint32_t active;
    std::uint32_t assigned;
    std::uint32_t index;
    std::uint32_t nodeHashes[181];
};
#pragma pack(pop)

static_assert(sizeof(OsageNodeRecord) == 0x20);
static_assert(offsetof(OsageNodeRecord, node) == 0x08);
static_assert(offsetof(OsageNodeRecord, nodeKind) == 0x10);
static_assert(offsetof(OsageNodeRecord, slotIndex) == 0x18);
static_assert(sizeof(OsageRenderSlot) == 0x2f8);
static_assert(offsetof(OsageRenderSlot, node) == 0x08);
static_assert(offsetof(OsageRenderSlot, baseHash) == 0x10);
static_assert(offsetof(OsageRenderSlot, attachmentValue) == 0x14);
static_assert(offsetof(OsageRenderSlot, active) == 0x18);
static_assert(offsetof(OsageRenderSlot, assigned) == 0x1c);
static_assert(offsetof(OsageRenderSlot, index) == 0x20);
static_assert(offsetof(OsageRenderSlot, nodeHashes) == 0x24);



struct HmdlRigFlagSlot {
    std::uint32_t cacheHandle;
    std::uint8_t reserved04[0xc];
    std::uint32_t baseHash;
    std::uint8_t reserved14[8];
    std::uint32_t assigned;
    std::uint8_t reserved20[4];
    std::uint32_t nodeHashes[128];
    std::uint8_t reserved224[4];
};

static_assert(sizeof(HmdlRigFlagSlot) == 0x228);
static_assert(offsetof(HmdlRigFlagSlot, baseHash) == 0x10);
static_assert(offsetof(HmdlRigFlagSlot, assigned) == 0x1c);
static_assert(offsetof(HmdlRigFlagSlot, nodeHashes) == 0x24);




struct HmdlRigState {
    std::uint32_t modelType;
    char boneName[260];
    std::uint32_t initialized;
    std::uint32_t ready;
    std::uint32_t cacheHandle;
    std::uint32_t entryNodeHash;
    std::uint32_t skeletonRootHash;
    std::uint32_t recordCount;
    OsageNodeRecord records[300];
    OsageRenderSlot osageSlots[8];
    HandSkinningAttachmentRecord handRecords[2];
    HeadRigState headRig;
    std::uint8_t reserved3ff4[4];
    HmdlRigFlagSlot flagSlots[32];
};

static_assert(offsetof(HmdlRigState, initialized) == 0x108);
static_assert(offsetof(HmdlRigState, ready) == 0x10c);
static_assert(offsetof(HmdlRigState, cacheHandle) == 0x110);
static_assert(offsetof(HmdlRigState, recordCount) == 0x11c);
static_assert(offsetof(HmdlRigState, records) == 0x120);
static_assert(offsetof(HmdlRigState, osageSlots) == 0x26a0);
static_assert(offsetof(HmdlRigState, handRecords) == 0x3e60);
static_assert(offsetof(HmdlRigState, headRig) == 0x3fa0);
static_assert(offsetof(HmdlRigState, flagSlots) == 0x3ff8);
static_assert(sizeof(HmdlRigState) == sizeof(RenderArrayEntry::rigState));



void HMDL_SetRigNodeVisibility(std::uint64_t handle, const HmdlRigState &rig, std::int32_t operation) {
    SetSkinningNodeDrawFlag(handle, operation != 0 ? 1 : 0);
    const std::uint32_t childCount = handle && (handle >> 32U) ? GetSkinningNodeChildCount(handle) : 0U;
    for (std::uint32_t index = 0; index < childCount; ++index) {
        const std::uint64_t child = GetSkinningNodeChild(handle, index);
        bool record = false;
        for (std::uint32_t entry = 0; entry < rig.recordCount && !record; ++entry)
            record = rig.records[entry].nodeHash == static_cast<std::uint32_t>(child >> 32U);
        if (!record)
            HMDL_SetRigNodeVisibility(child, rig, operation);
    }
}

struct MaterialRenderDefaults {
    std::int32_t renderFlag4 = 1;
    std::int32_t fogMode = 770;
    std::int32_t fogCurve = 771;
    std::int32_t alphaTest = 0;
    std::int32_t alphaFunction = 516;
    std::int32_t alphaReference = 0;
};

MaterialRenderDefaults g_materialRenderDefaults{};

template <typename T> T ReadState(const std::byte *state, std::size_t offset) {
    T value{};
    std::memcpy(&value, state + offset, sizeof(value));
    return value;
}

template <typename T> void WriteState(std::byte *state, std::size_t offset, const T &value) {
    std::memcpy(state + offset, &value, sizeof(value));
}



struct ShadowPlane {
    Vector3 point;
    Vector3 normal;
};



void Matrix4x4_ApplyPlanarShadowProjection(const ShadowPlane &plane, const Vector3 &offset, const Vector3 &origin) {
    const float dz = origin.z - offset.z;
    const float dy = origin.y - offset.y;
    const float dx = origin.x - offset.x;
    const float inverse = 1.0f / (dy * plane.normal.y + plane.normal.x * dx + dz * plane.normal.z);
    const float a = plane.normal.x * inverse;
    const float b = inverse * plane.normal.y;
    const float c = inverse * plane.normal.z;
    const float e = (plane.normal.y * plane.point.y + plane.normal.x * plane.point.x + plane.normal.z * plane.point.z) * inverse;
    Matrix4x4 projection{};
    projection.rows[0] = {1.0f - a * dx, -(a * dy), -(a * dz), 0.0f};
    projection.rows[1] = {-(b * dx), 1.0f - b * dy, -(b * dz), 0.0f};
    projection.rows[2] = {-(c * dx), -(c * dy), 1.0f - c * dz, 0.0f};
    projection.rows[3] = {e * dx, e * dy, e * dz, 1.0f};
    Matrix4x4::Multiply(&projection);
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
}





void UpdateActiveShadowLightsAndTimers(const std::int32_t (&nearest)[2], CharacterShadowState &state, const Vector3 &position) {
    std::int32_t candidates[2];
    for (std::size_t index = 0; index < 2; ++index)
        candidates[index] = static_cast<std::uint32_t>(nearest[index]) < 0x20U ? static_cast<std::int32_t>(LGHT_GetShadowSourceIndex(nearest[index])) : 0x21;
    for (std::size_t slot = 0; slot < 2; ++slot) {
        if (state.flag[slot] == 1U)
            continue;
        if (state.lightIndex[slot] != candidates[0] && state.lightIndex[slot] != candidates[1])
            state.flag[slot] = 1U;
    }
    for (std::size_t slot = 0; slot < 2; ++slot) {
        if (state.lightIndex[slot] == -1 || state.flag[slot] != 1U)
            continue;
        state.fade[slot] = static_cast<float>(static_cast<double>(state.fade[slot]) - 0.025);
        if (!(0.0f < state.fade[slot])) {
            state.fade[slot] = 0.0f;
            state.lightIndex[slot] = -1;
        }
        if (candidates[0] == state.lightIndex[slot])
            state.flag[slot] = 0U;
        if (candidates[1] == state.lightIndex[slot])
            state.flag[slot] = 0U;
    }
    for (std::size_t slot = 0; slot < 2; ++slot) {
        if (state.lightIndex[slot] != -1)
            continue;
        const std::int32_t other = state.lightIndex[slot ^ 1U];
        for (const std::int32_t candidate : candidates) {
            if (candidate == other)
                continue;
            state.lightIndex[slot] = candidate;
            state.flag[slot] = 0U;
            break;
        }
    }
    Vector3 light{};
    const auto distance = [&](std::int32_t index, std::int32_t candidate) {
        const ShenmueLightSlot *const render = static_cast<std::uint32_t>(index) < 0x20U ? LGHT_GetRenderSlot(index) : nullptr;
        if (render && (render->modeFlags & ~8U) == 2U)
            return std::bit_cast<float>(candidate);
        if (const ShenmueLightSlot *const data = static_cast<std::uint32_t>(index) < 0x80U ? LGHT_GetDataSlot(index) : nullptr)
            light = data->position;
        const float dx = position.x - light.x;
        const float dy = position.y - light.y;
        const float dz = position.z - light.z;
        return dy * dy + dx * dx + dz * dz;
    };
    const float first = distance(state.lightIndex[0], candidates[0]);
    const float second = distance(state.lightIndex[1], candidates[1]);
    if (first > second) {
        std::swap(state.lightIndex[0], state.lightIndex[1]);
        std::swap(state.fade[0], state.fade[1]);
        std::swap(state.flag[0], state.flag[1]);
    }
}




std::uint32_t ComputeShadowFadeAndActiveLights(
    const Vector3 &position, float &fade, CharacterShadowState *state, const Character *character, std::int32_t &count, std::int32_t &mode) {
    Vector3 view{};
    Matrix4x4::TransformPoint(position, view);
    fade = static_cast<float>(1.0 - static_cast<double>(std::sqrt(view.x * view.x + view.z * view.z) * g_inverseShadowRange));
    if (0.0f > fade)
        fade = 0.0f;
    else if (fade > 1.0f)
        fade = 1.0f;
    const std::uint32_t mask = character ? character->lightMask : g_defaultLightMask;
    if (g_prevCachedGlobalLightingAdjustFlag != 3) {
        count = 29;
        mode = 0;
        return mask;
    }
    count = 2;
    mode = 1;
    std::int32_t nearest[2]{-1, -1};
    float best = 1000000.0f;
    float second = 1000000.0f;
    for (std::int32_t slot = 0; slot < 7; ++slot) {
        const ShenmueLightSlot *const light = LGHT_GetRenderSlot(static_cast<std::uint32_t>(slot));
        if (!light || light->enabled != 1)
            continue;
        float distance = 0.0f;
        if ((light->modeFlags & ~8U) != 2U) {
            const float dy = position.y - light->position.y;
            const float dx = position.x - light->position.x;
            const float dz = position.z - light->position.z;
            distance = dy * dy + dx * dx + dz * dz;
        }
        if (!(second > distance))
            continue;
        if (best > distance) {
            second = best;
            nearest[1] = nearest[0];
            best = distance;
            nearest[0] = slot;
        } else {
            second = distance;
            nearest[1] = slot;
        }
    }
    UpdateActiveShadowLightsAndTimers(nearest, *state, position);
    return mask;
}



float ShadowAngularIntensity(const Vector3 &rayDirection) {
    const auto degrees = static_cast<float>((static_cast<double>(-rayDirection.y) - 0.25) * 40.0 / 0.75);
    const auto angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
    return std::clamp(sin_fixed16(static_cast<std::int16_t>(angle)), 0.0f, 1.0f);
}





float RecalculateLightParams(const Vector3 &position, Vector3 &lightPoint, CharacterShadowState *state, const ShenmueLightSlot &light, float direction,
    std::size_t slot, bool dynamicLights) {
    float intensity = dynamicLights ? -1.0f : direction;
    if (direction == -2.0f)
        intensity = 1.0f;
    if (light.enabled != 1)
        return 0.0f;
    const std::uint32_t mode = light.modeFlags & ~8U;
    if (mode == 2U) {
        lightPoint = {position.x - light.direction.x, position.y - light.direction.y, position.z - light.direction.z};
        if (intensity == -1.0f)
            intensity = ShadowAngularIntensity(light.direction);
    } else if (mode == 3U || mode == 4U) {
        lightPoint = light.position;
        const float height = lightPoint.y - position.y;
        if (2.0f > height && height > 0.0f)
            lightPoint.y = static_cast<float>(static_cast<double>(position.y) + 2.0);
        if (intensity == -1.0f) {
            Vector3 ray{position.x - lightPoint.x, position.y - lightPoint.y, position.z - lightPoint.z};
            Vector3::NormalizeInPlace(ray);
            intensity = ShadowAngularIntensity(ray);
            if (dynamicLights) {
                if (state->flag[slot] == 0U) {
                    const float delta = intensity - state->fade[slot];
                    if (static_cast<double>(std::fabs(delta)) > 0.025)
                        state->fade[slot] = static_cast<float>(static_cast<double>(state->fade[slot]) + (delta < 0.0f ? -0.025 : 0.025));
                    else
                        state->fade[slot] = intensity;
                }
                if (state->fade[slot] > 1.0f)
                    state->fade[slot] = 1.0f;
                else if (0.0f > state->fade[slot])
                    state->fade[slot] = 0.0f;
                intensity = state->fade[slot];
            }
        }
    }
    return 0.0f > intensity ? 0.0f : (1.0f < intensity ? 1.0f : intensity);
}

struct CharacterShadowCallbackState {
    D3TDeviceContext *context;
    Matrix4x4 workMatrix;
    MatrixStackTable matrixStacks;
    std::uint16_t matrixStackIndex;
    std::array<std::byte, 0xac8> renderState;
    std::array<AlphaRenderDefaults, 4> alphaRenderFlags;
    std::array<EngineAlphaState, 4> engineAlphaState;
    AlphaShaderParameters alphaShaderParameters;
    LegacyShaderParameters legacyShaderParameters;
    ShaderParams renderParameters;
    std::array<std::int32_t, 172> currentRenderFlags;
    std::array<std::uint32_t, 2> legacyAlphaFlags;
    MaterialRenderDefaults materialDefaults;
    RenderInputLayoutIndex shaderSelection;
    Matrix4x4 mt5RootTransform;
    std::byte *activeMt5Base;
    Character *activeRiggedCharacter;
    Mt5Node *syncedFaceRoot;
    Mt5Node *syncedFaceSecondary;
    std::uint32_t currentInputLayout;
    std::uint32_t drawCount;
    std::uint32_t visitedNodeCount;
    std::uint32_t bindingCount;
    std::uint32_t missingSceneEntryCount;
    std::uint32_t preparedNodeCount;
    std::int32_t renderingMode;
    std::uint64_t engineFlags;
    std::uint64_t engineField8;
    std::int32_t stencilReference;
    bool alphaMaskedDepthPass;
    bool characterShadowPass;
    bool recordingRyoDraw;
    bool capturedRyoTransform;
    bool submittingSyncedHand;

    explicit CharacterShadowCallbackState(D3TDeviceContext *renderContext)
        : context(renderContext), workMatrix(workTransformMatrix), matrixStacks(g_matrixStack), matrixStackIndex(g_CurrentMatrixStackIdx),
          renderState(renderContext->renderState), alphaRenderFlags(g_currentAlphaRenderFlags), engineAlphaState(g_engineAlphaState),
          alphaShaderParameters(g_alphaShaderParameters), legacyShaderParameters(g_legacyShaderParameters), renderParameters(g_renderParams),
          currentRenderFlags(g_currentRenderFlags), legacyAlphaFlags(g_legacyAlphaFlags), materialDefaults(g_materialRenderDefaults),
          shaderSelection(g_tmpInputLayoutIndex), mt5RootTransform(g_mt5RootTransform), activeMt5Base(g_activeMt5Base),
          activeRiggedCharacter(g_activeRiggedCharacter), syncedFaceRoot(g_syncedFaceRoot), syncedFaceSecondary(g_syncedFaceSecondary),
          currentInputLayout(g_currentInputLayout), drawCount(g_drawCount), visitedNodeCount(g_visitedNodeCount), bindingCount(g_bindingCount),
          missingSceneEntryCount(g_missingSceneEntryCount), preparedNodeCount(g_preparedNodeCount), renderingMode(g_legacyCurrentRenderingMode),
          engineFlags(g_legacyEngineFlags), engineField8(g_legacyEngineField8), stencilReference(g_legacyStencilReference),
          alphaMaskedDepthPass(g_alphaMaskedDepthPass), characterShadowPass(g_characterShadowPass), recordingRyoDraw(g_recordingRyoDraw),
          capturedRyoTransform(g_capturedRyoTransform), submittingSyncedHand(g_submittingSyncedHand) {
    }

    ~CharacterShadowCallbackState() {
        g_currentAlphaRenderFlags = alphaRenderFlags;
        g_engineAlphaState = engineAlphaState;
        g_alphaShaderParameters = alphaShaderParameters;
        g_legacyShaderParameters = legacyShaderParameters;
        g_renderParams = renderParameters;
        g_currentRenderFlags = currentRenderFlags;
        g_legacyAlphaFlags = legacyAlphaFlags;
        g_materialRenderDefaults = materialDefaults;
        g_tmpInputLayoutIndex = shaderSelection;
        g_mt5RootTransform = mt5RootTransform;
        g_activeMt5Base = activeMt5Base;
        g_activeRiggedCharacter = activeRiggedCharacter;
        g_syncedFaceRoot = syncedFaceRoot;
        g_syncedFaceSecondary = syncedFaceSecondary;
        g_currentInputLayout = currentInputLayout;
        g_legacyCurrentRenderingMode = renderingMode;
        g_legacyEngineFlags = engineFlags;
        g_legacyEngineField8 = engineField8;
        g_legacyStencilReference = stencilReference;
        g_alphaMaskedDepthPass = alphaMaskedDepthPass;
        g_drawCount = drawCount;
        g_visitedNodeCount = visitedNodeCount;
        g_bindingCount = bindingCount;
        g_missingSceneEntryCount = missingSceneEntryCount;
        g_preparedNodeCount = preparedNodeCount;
        g_characterShadowPass = characterShadowPass;
        g_recordingRyoDraw = recordingRyoDraw;
        g_capturedRyoTransform = capturedRyoTransform;
        g_submittingSyncedHand = submittingSyncedHand;
        g_matrixStack = matrixStacks;
        g_CurrentMatrixStackIdx = matrixStackIndex;
        workTransformMatrix = workMatrix;

        context->renderState = renderState;
#if defined(_WIN32)
        if (void *mapped = MapTempConstantBufferForWriteDiscard(context, 3, 1, sizeof(renderParameters))) {
            std::memcpy(mapped, &renderParameters, sizeof(renderParameters));
        }
        std::byte *const state = context->renderState.data();
        std::memset(state + 0x958, 0xff, 0x1c);
        state[0x974] = std::byte{0xff};
        state[0x975] = std::byte{0xff};
        state[0x976] = std::byte{0xff};
        DX11_UnmapAndSyncShaderBindings(context);
        DX11_SetupShadersForDXAndSetViewports(context);
#endif
        context->renderState = renderState;
    }
};

void SetMaterialRenderFlag(std::uint32_t setting, std::int32_t value) {
    SetRenderFlags(setting, value);
    switch (setting) {
    case 64:
        g_materialRenderDefaults.renderFlag4 = value;
        break;
    case 62:
        g_materialRenderDefaults.fogMode = value;
        break;
    case 63:
        g_materialRenderDefaults.fogCurve = value;
        break;
    case 60:
        g_materialRenderDefaults.alphaTest = value;
        break;
    case 58:
        g_materialRenderDefaults.alphaFunction = value;
        break;
    case 61:
        g_materialRenderDefaults.alphaReference = value;
        break;
    default:
        break;
    }
}

void SetRenderFlags_NoIgnoreLighting() {
    SetRenderFlags(64, g_materialRenderDefaults.renderFlag4);
    SetRenderFlags(62, g_materialRenderDefaults.fogMode);
    SetRenderFlags(63, g_materialRenderDefaults.fogCurve);
    SetRenderFlags(60, g_materialRenderDefaults.alphaTest);
    SetRenderFlags(58, g_materialRenderDefaults.alphaFunction);
    SetRenderFlags(61, g_materialRenderDefaults.alphaReference);
    g_renderParams.ignoreLighting = 0;
    g_renderParams.ignoreAlpha = 0;
}

Mt5Node *OffsetNode(std::int32_t offset) {
    std::byte *const base = g_activeMt5Base ? g_activeMt5Base : g_pCurrentTaskData;
    return offset ? reinterpret_cast<Mt5Node *>(base + offset) : nullptr;
}

Mt5SceneBinding *BindingFor(Mt5Node *node, std::byte **modelBase) {
    return static_cast<Mt5SceneBinding *>(MT5_SceneBindingForNode(node, modelBase));
}

std::int32_t SceneKey(const void *binding) {
    return static_cast<std::int32_t>((reinterpret_cast<std::uintptr_t>(binding) + 4U) % 0xffffffffULL);
}

void PushMatrix() {
    *g_matrixStack.top.stack = workTransformMatrix;
    ++g_matrixStack.top.info.index;
    if (g_matrixStack.top.info.index < g_matrixStack.top.info.capacity)
        ++g_matrixStack.top.stack;
}

void PopMatrix() {
    std::int16_t count = g_matrixStack.top.info.index < 1 ? g_matrixStack.top.info.index : 1;
    std::int16_t pointerCount = count;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
        pointerCount = static_cast<std::int16_t>(count + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index);
    }
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - count);
    g_matrixStack.top.stack -= pointerCount;
    workTransformMatrix = *g_matrixStack.top.stack;
}

void ApplyNodeTransform(const Mt5Node &node) {
    if (node.transformOffset) {
        workTransformMatrix = g_mt5RootTransform;
        Matrix4x4::Multiply(HMDL_TransformForNode(&node, node.transformOffset));
    } else {
        Matrix4x4::preTranslate(node.posX, node.posY, node.posZ);
        if (node.rotZ)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotZ));
        if (node.rotY)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotY));
        if (node.rotX)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotX));
        const bool scaled = !g_characterHandlerNodeWalk || (g_areaId == SHEN_NAME(DGCT) && node.nodeKind - 152U <= 99U);
        if (scaled && (node.scaleX != 1.0f || node.scaleY != 1.0f || node.scaleZ != 1.0f)) {
            Matrix4x4::Scale(node.scaleX, node.scaleY, node.scaleZ);
        }
    }
    if (!g_characterHandlerNodeWalk && (node.nodeKind == 8 || node.nodeKind == 13)) {
        if (node.rotZ)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotZ));
        if (node.rotY)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotY));
        if (node.rotX)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotX));
    }
}



void ApplyCachedNodeTransform(const Mt5Node &node) {
    if (node.transformOffset) {
        if (const Matrix4x4 *const transform = HMDL_TransformForNode(&node, node.transformOffset)) {
            workTransformMatrix = *transform;
        }
        return;
    }

    Matrix4x4::preTranslate(node.posX, node.posY, node.posZ);
    if (node.rotZ)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotZ));
    if (node.rotY)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotY));
    if (node.rotX)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotX));
}
struct RiggedNodeSpecialState {
    bool faceChannelRebased = false;
    bool suppressBinding = false;
    bool traverseChildren = true;
    void *deferredFaceRoot = nullptr;
};

void ApplyFaceEyeNodeAngles(std::uint64_t nodePointer, std::uint64_t packedAngles) {
    auto *const node = reinterpret_cast<Mt5Node *>(nodePointer);
    if (!node)
        return;
    node->rotX = 0;
    node->rotY = static_cast<std::int32_t>(static_cast<std::uint32_t>(packedAngles));
    node->rotZ = static_cast<std::int32_t>(static_cast<std::uint32_t>(packedAngles >> 32U));
}
void UpdateDebugFaceEyeTargets(void *model, const FaceWork &face) {
    if (!g_debugUnknown3 || !face.field20)
        return;

    const auto updateChannel = [&](std::uint64_t nodePointer, const std::byte *rotationBytes, std::uint64_t packedYZ, std::uint32_t channel) {
        const auto *const node = reinterpret_cast<const Mt5Node *>(nodePointer);
        if (!node)
            return;
        std::int32_t rotationX = 0;
        std::memcpy(&rotationX, rotationBytes, sizeof(rotationX));

        PushMatrix();
        Matrix4x4::preTranslate(node->posX, node->posY, node->posZ);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(packedYZ >> 32U));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(packedYZ));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotationX));

        const Vector3 origin{
            workTransformMatrix.rows[3].x,
            workTransformMatrix.rows[3].y,
            workTransformMatrix.rows[3].z,
        };
        const Vector3 target{
            workTransformMatrix.rows[0].x * 0.5f + origin.x,
            workTransformMatrix.rows[0].y * 0.5f + origin.y,
            workTransformMatrix.rows[0].z * 0.5f + origin.z,
        };
        FACE_UpdateEyeLookAtModelTarget(model, channel, &origin, &target);
        PopMatrix();
    };

    updateChannel(face.field70, face.reserved80, face.field84, 1);
    updateChannel(face.field78, face.reserved8c, face.field90, 0);
}

void SubmitReplacementNodeTree(Mt5Node *root, float alpha, bool syncedHand = false) {
    if (!root)
        return;

    std::byte *const previousBase = g_activeMt5Base;
    const bool previousHandSubmission = g_submittingSyncedHand;
    const std::int32_t siblingOffset = root->siblingOffset;
    g_activeMt5Base = MT5_ModelBaseForNode(root);
    g_submittingSyncedHand = syncedHand;
    root->siblingOffset = 0;
    PushMatrix();
    d3t::MT5_TransformAndSubmitNodeTree(root, alpha, 0);
    PopMatrix();
    root->siblingOffset = siblingOffset;
    g_submittingSyncedHand = previousHandSubmission;
    g_activeMt5Base = previousBase;
}

bool SubmitHandReplacement(Mt5Node &bodyNode, float alpha) {
    const auto kind = static_cast<std::int16_t>(bodyNode.nodeKind);
    if (kind != -66 && kind != -65)
        return false;
    HandModelWork *const hand = HND_GetWork(g_activeRiggedCharacter, kind == -66);
    const std::int8_t selector = hand ? hand->playback.status : 0;
    if (!selector)
        return false;
    auto *const root = reinterpret_cast<Mt5Node *>(selector > 0 ? hand->primaryRoot : hand->secondaryRoot);
    if (!root)
        return false;

    std::byte *modelBase = nullptr;
    Mt5SceneBinding *const binding = BindingFor(root, &modelBase);
    if (!binding || !modelBase)
        return false;
    if (!g_characterShadowPass && !g_alphaMaskedDepthPass) {
        const void *const mutableVertices = selector > 0 ? hand->mutableVertices : nullptr;

        HMDL_SetTemporaryTaskRelativeOffsets(root, true);
        const std::int32_t previousParent = root->parentOffset;
        const std::int32_t previousTransform = root->transformOffset;
        root->parentOffset = bodyNode.parentOffset;
        root->transformOffset = bodyNode.transformOffset;
        if ((binding->flags & 0x400U) != 0U) {
            GameNode *const entry = FindSceneEntityByID(SceneKey(binding));
            if (entry && entry->bufferHandle) {
                const void *const previousVertices = MT5_SetVertexDataOverride(mutableVertices);
                void *mutableNode = root;
                MT5_ReadMeshStrip(&mutableNode, nullptr, entry->bufferHandle);
                MT5_SetVertexDataOverride(previousVertices);
            }
        }
        root->parentOffset = previousParent;
        root->transformOffset = previousTransform;
        HMDL_SetTemporaryTaskRelativeOffsets(root, false);
    }

    SubmitReplacementNodeTree(root, alpha, true);
    return true;
}

bool PrepareFaceChannelBinding(Mt5Node &node) {
    FaceClipControllerState *const clip = CLIP_GetWork(g_activeRiggedCharacter);
    if (!clip || clip->channelTag != static_cast<std::uint16_t>(-68) ||
        (!static_cast<std::uint8_t>(clip->field18 >> 8U) && !clip->activeLipCues && clip->morphComplete) || !clip->clip || clip->channelIndex >= 3) {
        return false;
    }
    std::byte *bindingBase = nullptr;
    return FACE_GetChannelVertices(clip->channelIndex) && BindingFor(&node, &bindingBase) && bindingBase;
}
void SyncFaceReplacementMesh(Mt5Node &bodyNode, FaceWork &face) {
    if (g_alphaMaskedDepthPass)
        return;
    auto *const root = reinterpret_cast<Mt5Node *>(face.field20);
    auto *const faceNode = reinterpret_cast<Mt5Node *>(face.field28);
    Mt5Node *primary = faceNode ? faceNode : root;
    Mt5Node *secondary = nullptr;
    if (faceNode && faceNode->parentOffset) {
        primary = root;
        secondary = faceNode;
    }
    if (!primary)
        return;

    std::byte *primaryBase = nullptr;
    Mt5SceneBinding *const primaryBinding = BindingFor(primary, &primaryBase);
    std::byte *secondaryBase = nullptr;
    Mt5SceneBinding *const secondaryBinding = BindingFor(secondary, &secondaryBase);
    Mt5SceneBinding *const mutableBinding = secondaryBinding ? secondaryBinding : primaryBinding;

    const void *const mutableVertices = face.field118 ? reinterpret_cast<const void *>(face.field118) : nullptr;

    HMDL_SetTemporaryTaskRelativeOffsets(primary, true);
    const std::int32_t previousParent = primary->parentOffset;
    const std::int32_t previousTransform = primary->transformOffset;
    primary->parentOffset = bodyNode.parentOffset;
    primary->transformOffset = bodyNode.transformOffset;

    std::int32_t bufferHandle = 0;
    if (primaryBinding && (primaryBinding->flags & 0x400U) != 0U) {
        GameNode *const entry = FindSceneEntityByID(SceneKey(primaryBinding));
        if (entry)
            bufferHandle = entry->bufferHandle;
    }
    const auto readStrips = [](Mt5Node *node, std::int32_t handle, const void *vertexOverride) {
        if (!node)
            return;
        std::byte *const previousBase = g_activeMt5Base;
        const void *const previousVertices = MT5_SetVertexDataOverride(vertexOverride);
        g_activeMt5Base = MT5_ModelBaseForNode(node);
        void *mutableNode = node;
        MT5_ReadMeshStrip(&mutableNode, nullptr, handle);
        g_activeMt5Base = previousBase;
        MT5_SetVertexDataOverride(previousVertices);
    };
    readStrips(primary, bufferHandle, primaryBinding == mutableBinding ? mutableVertices : nullptr);
    if (secondary) {
        if (secondaryBinding && (secondaryBinding->flags & 0x400U) != 0U) {
            GameNode *const entry = FindSceneEntityByID(SceneKey(secondaryBinding));
            if (entry)
                bufferHandle = entry->bufferHandle;
        }
        readStrips(secondary, bufferHandle, secondaryBinding == mutableBinding ? mutableVertices : nullptr);
    }

    primary->parentOffset = previousParent;
    primary->transformOffset = previousTransform;
    HMDL_SetTemporaryTaskRelativeOffsets(primary, false);
    g_syncedFaceRoot = primary;
    g_syncedFaceSecondary = secondary;
}

const std::byte *OsageAttachmentForNode(Mt5Node &node) {
    return static_cast<const std::byte *>(HMDL_CycleDataForNode(&node));
}

std::uint32_t ProcessOsageSlot(OsageRenderSlot &slot) {
    if (!slot.active || !slot.assigned || !slot.cacheHandle || !slot.baseHash || !slot.node) {
        return 0;
    }
    const std::byte *const attachment = OsageAttachmentForNode(*slot.node);
    if (!attachment)
        return 0;
    std::uint32_t identifier = 0;
    std::memcpy(&identifier, attachment, sizeof(identifier));
    if (identifier == SHEN_NAME(OSAG))
        return 0;

    const std::uint32_t count = std::min<std::uint32_t>(
        static_cast<std::uint8_t>(attachment[1]) * static_cast<std::uint8_t>(attachment[2]), static_cast<std::uint32_t>(std::size(slot.nodeHashes)));
    const std::byte *records = nullptr;
    std::memcpy(&records, attachment + 0x68, sizeof(records));
    if (!records)
        return 0;

    std::uint32_t writes = 0;
    for (std::uint32_t index = 0; index < count; ++index) {
        const std::byte *const source = records + static_cast<std::size_t>(index) * 0x80;
        const Vector3 *position = nullptr;
        const Vector3 *directionTarget = nullptr;
        std::uint16_t directionIndex = 0xffff;
        std::memcpy(&position, source + 0x10, sizeof(position));
        std::memcpy(&directionIndex, source + 0x20, sizeof(directionIndex));
        std::memcpy(&directionTarget, source + 0x28, sizeof(directionTarget));
        if (!position || !slot.nodeHashes[index])
            continue;
        if (directionIndex == 0xffff)
            directionTarget = nullptr;
        const std::uint64_t handle = slot.cacheHandle | (static_cast<std::uint64_t>(slot.nodeHashes[index]) << 32U);
        writes += PublishOsageSkinningTransform(handle, *position, directionTarget) ? 1U : 0U;
    }
    return writes;
}

void PrepareRiggedNodeSpecialState(Mt5Node &node) {
    const auto kind = static_cast<std::int16_t>(node.nodeKind);
    if (kind == -69) {
        FaceClipControllerState *const clip = CLIP_GetWork(g_activeRiggedCharacter);
        if (clip && clip->field54)
            node.rotY = static_cast<std::int32_t>(clip->rotationY);
        return;
    }
    if (kind == -68) {
        PrepareFaceChannelBinding(node);
        return;
    }
    if (kind != -67)
        return;

    FaceWork *const face = FACE_GetWork(g_activeRiggedCharacter);
    if (!face || !face->field20 || (static_cast<std::uint16_t>(face->eyeMode) & 1U) == 0U || !FACE_IsBoundToCachedModel(*face)) {
        return;
    }
    ApplyFaceEyeNodeAngles(face->field70, face->field84);
    ApplyFaceEyeNodeAngles(face->field78, face->field90);
    UpdateDebugFaceEyeTargets(g_activeRiggedCharacter->model, *face);
    SyncFaceReplacementMesh(node, *face);
}

void PrepareRiggedNodeTree(Mt5Node *node) {
    while (node) {
        ++g_preparedNodeCount;
        const bool hasSibling = node->siblingOffset != 0;
        if (hasSibling)
            PushMatrix();
        ApplyCachedNodeTransform(*node);
        PrepareRiggedNodeSpecialState(*node);
        if (Mt5Node *const child = OffsetNode(node->childOffset))
            PrepareRiggedNodeTree(child);
        if (!hasSibling)
            break;
        PopMatrix();
        node = OffsetNode(node->siblingOffset);
    }
}

RiggedNodeSpecialState ApplyRiggedNodeSpecialState(Mt5Node &node, float alpha) {
    RiggedNodeSpecialState result{};
    const auto kind = static_cast<std::int16_t>(node.nodeKind);
    if ((kind == -66 || kind == -65) && SubmitHandReplacement(node, alpha)) {
        result.suppressBinding = true;
        result.traverseChildren = false;
        return result;
    }
    if (kind == -67) {
        FaceWork *const face = FACE_GetWork(g_activeRiggedCharacter);
        if (face && face->field20 && (static_cast<std::uint16_t>(face->eyeMode) & 1U) != 0U && FACE_IsBoundToCachedModel(*face)) {
            result.suppressBinding = face->field28 != 0;
            result.deferredFaceRoot = reinterpret_cast<void *>(face->field20);
        }
        return result;
    }
    if (kind == -69)
        return result;
    if (kind == -68) {
        FaceWork *const face = FACE_GetWork(g_activeRiggedCharacter);
        if (face && (static_cast<std::uint16_t>(face->eyeMode) & 1U) != 0U) {
            result.suppressBinding = true;
            return result;
        }
        if (VFMA_IsFighting())
            return result;
        result.faceChannelRebased = PrepareFaceChannelBinding(node);
    }
    return result;
}

bool ResolveSceneShaders() {
    const std::int32_t layoutIndex = g_tmpInputLayoutIndex.inputLayoutIndex;
    g_currentInputLayout =
        layoutIndex >= 0 && static_cast<std::size_t>(layoutIndex) < g_fvfLayouts.size() ? g_fvfLayouts[static_cast<std::size_t>(layoutIndex)] : 0;
    return g_originalLambVertexShaders[g_tmpInputLayoutIndex.vertexShaderIndex] && g_currentInputLayout;
}

void BindVertexOrPixelShader(D3TDeviceContext *context, std::uint32_t stage, std::uint32_t shader) {
    std::byte *const state = context->renderState.data();
    const std::size_t offset = stage == 0 ? 0U : 4U;
    if (ReadState<std::uint32_t>(state, offset) == shader)
        return;
    WriteState(state, offset, shader);
    if (stage == 0) {
        state[0x975] |= std::byte{0x81};
        WriteState(state, 0x970, ~std::uint32_t{0});
    } else {
        state[0x975] |= std::byte{2};
    }
}

} // namespace

const std::int32_t *g_effectBufferHandleOverride = nullptr;

void ConvertAndPushCurrentMatrixToShaderBlock() {
    const Matrix4x4 model = workTransformMatrix;
    *g_matrixStack.top.stack = model;

    SetActiveStack(MatrixStackType::Transparent);
    PushMatrix();
    Matrix4x4::Multiply(&model);
    Matrix4x4::RotateMatrixCounterClockwise90();
    g_renderParams.constants[0] = workTransformMatrix.rows[0];
    g_renderParams.constants[1] = workTransformMatrix.rows[1];
    g_renderParams.constants[2] = workTransformMatrix.rows[2];
    g_renderParams.constants[3] = workTransformMatrix.rows[3];
    PopMatrix();

    SetActiveStack(MatrixStackType::DepthPrepass);
    Matrix4x4::RotateMatrixCounterClockwise90();
    g_renderParams.constants[9] = workTransformMatrix.rows[0];
    g_renderParams.constants[10] = workTransformMatrix.rows[1];
    g_renderParams.constants[11] = workTransformMatrix.rows[2];
    Matrix4x4::RotateMatrixCounterClockwise90();
}

namespace {

void PublishLegacyShaderParameters(ShaderParams &parameters) {
    parameters.fogColour = g_legacyShaderParameters.fogColour;
    parameters.fogEnabled = g_legacyShaderParameters.fogEnabled;
    parameters.alphaTestEnabled = g_legacyShaderParameters.alphaTestEnabled;
    parameters.alphaReference = g_legacyShaderParameters.alphaReference;
    parameters.alphaFunction = g_legacyShaderParameters.alphaFunction;
    parameters.alphaOperation = g_alphaShaderParameters.operation;
    parameters.alphaArgument0 = g_alphaShaderParameters.argument0;
    parameters.alphaArgument1 = g_alphaShaderParameters.argument1;
    parameters.alphaArgument2 = g_alphaShaderParameters.argument2;
    parameters.specularEnabled = g_legacyShaderParameters.specularEnabled;
}

void BindTexture(D3TDeviceContext *context, std::uint32_t texture) {
    std::byte *const state = context->renderState.data();




    constexpr std::size_t kTextureOffset = 0x1b4;
    constexpr std::size_t kViewFlagsOffset = 0x334;
    if (ReadState<std::uint32_t>(state, kTextureOffset) != texture || ReadState<std::uint32_t>(state, kViewFlagsOffset) != 0U) {
        auto dirty = ReadState<std::uint32_t>(state, 0x95c);
        dirty |= 1U;
        WriteState(state, 0x95c, dirty);
        WriteState(state, kTextureOffset, texture);
        WriteState(state, kViewFlagsOffset, std::uint32_t{0});
    }
    const EngineAlphaState &sampler = g_engineAlphaState[0];
    if (std::memcmp(state + 0x534, &sampler, sizeof(sampler)) != 0) {
        auto dirty = ReadState<std::uint16_t>(state, 0x966);
        dirty |= 1U;
        WriteState(state, 0x966, dirty);
        std::memcpy(state + 0x534, &sampler, sizeof(sampler));
    }
}

void CommitLegacyRenderState(D3TDeviceContext *context, ShaderParams &parameters, bool transparent) {
    PublishLegacyShaderParameters(parameters);
#if defined(_WIN32)
    if (void *mapped = MapTempConstantBufferForWriteDiscard(context, 3, 1, sizeof(parameters))) {
        std::memcpy(mapped, &parameters, sizeof(parameters));
    }
#else
    static_cast<void>(parameters);
#endif

    std::byte *const state = context->renderState.data();
    static_cast<void>(transparent);
    if (ReadState<std::uint64_t>(state, 0x940) != g_legacyEngineFlags ||
        ReadState<std::uint32_t>(state, 0x950) != static_cast<std::uint32_t>(g_legacyStencilReference)) {
        state[0x975] |= std::byte{0x10};
        WriteState(state, 0x940, g_legacyEngineFlags);
        WriteState(state, 0x948, g_legacyEngineField8);
        WriteState(state, 0x950, static_cast<std::uint32_t>(g_legacyStencilReference));
    }
    const std::uint32_t rasterizer = static_cast<std::uint32_t>(g_legacyCurrentRenderingMode);
    if (ReadState<std::uint32_t>(state, 0x954) != rasterizer) {
        state[0x975] |= std::byte{0x20};
        WriteState(state, 0x954, rasterizer);
    }
    if (std::memcmp(state + 0x794, g_legacyAlphaFlags.data(), sizeof(g_legacyAlphaFlags)) != 0) {
        state[0x975] |= std::byte{8};
        std::memcpy(state + 0x794, g_legacyAlphaFlags.data(), sizeof(g_legacyAlphaFlags));
    }
    std::uint16_t samplerDirty = ReadState<std::uint16_t>(state, 0x966);
    const std::uint16_t originalSamplerDirty = samplerDirty;
    for (std::size_t lane = 0; lane < g_engineAlphaState.size(); ++lane) {
        const EngineAlphaState &sampler = g_engineAlphaState[lane];
        std::byte *const destination = state + 0x534 + lane * sizeof(sampler);
        if (std::memcmp(destination, &sampler, sizeof(sampler)) != 0) {
            samplerDirty |= static_cast<std::uint16_t>(1U << lane);
            std::memcpy(destination, &sampler, sizeof(sampler));
        }
    }
    if (samplerDirty != originalSamplerDirty)
        WriteState(state, 0x966, samplerDirty);
}

void ParseDrawMaterialCommand(const std::uint16_t *&command, float alphaScale, std::uint8_t shaderMode, MaterialState &material, bool &transparent) {
    material = {};
    const std::uint16_t size = command[1];
    std::array<std::uint32_t, 3> words{};
    std::memcpy(words.data(), command + 2, (std::min)(static_cast<std::size_t>(size) * sizeof(std::uint16_t), sizeof(words)));
    const std::uint32_t flags = words[0];
    const std::uint32_t materialFlags = words[2];
    command = reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(command + 2) + size);
    const std::uint32_t alphaMode = (flags >> 24U) & 7U;



    g_renderParams.constants[16] = {1.0f, 1.0f, 1.0f, alphaScale};
    transparent = alphaScale != 1.0f || alphaMode == 2 || alphaMode == 3;
    material.renderClass = alphaMode;
    material.sourceBlend = (materialFlags >> 17U) & 3U;
    material.destinationBlend = (materialFlags >> 15U) & 3U;
    material.ignoreAlpha = (flags >> 2U) & 1U;
    material.materialMode = (flags >> 4U) & 3U;
    if ((shaderMode & 2U) == 0U)
        SetRenderFlags(171, 1);
    SetRenderFlags(7, 1);
    if (alphaScale != 1.0f) {
        if ((shaderMode & 2U) == 0U) {
            SetMaterialRenderFlag(62, 770);
            SetMaterialRenderFlag(63, 771);
        }
        SetRenderFlags(59, 1);
        SetMaterialRenderFlag(64, 0);
        material.renderClass = 2;
    } else {
        material.lit0 = (materialFlags >> 20U) & 1U;
        material.lit1 = (materialFlags >> 19U) & 1U;
        SetRenderFlags(59, 0);
    }
    if (alphaScale == 1.0f && (shaderMode & 8U) != 0U) {
        SetMaterialRenderFlag(60, 1);
        SetMaterialRenderFlag(61, 160);
        SetMaterialRenderFlag(58, 516);
        SetMaterialRenderFlag(64, 1);
    } else if (alphaScale == 1.0f) {
        SetMaterialRenderFlag(60, 0);
        SetMaterialRenderFlag(64, 1);
        if (alphaMode == 2 || alphaMode == 3) {
            if ((shaderMode & 2U) == 0U)
                SetRenderFlags(59, 1);
            SetMaterialRenderFlag(64, 0);
        } else if (alphaMode == 4) {
            SetMaterialRenderFlag(60, 1);
            SetMaterialRenderFlag(61, 255);
            SetMaterialRenderFlag(58, 514);
        }
    }
    if (g_alphaMaskedDepthPass) {
        // charaSetRenderFlags redraws CPCT characters into the original-strip
        // depth target with only fully opaque texels admitted.
        SetRenderFlags(60, 1);
        SetRenderFlags(61, 255);
        SetRenderFlags(58, 514);
    }
    material.alphaTestEnabled = g_legacyShaderParameters.alphaTestEnabled;
    material.alphaReference = g_legacyShaderParameters.alphaReference;
    material.alphaFunction = g_legacyShaderParameters.alphaFunction;





    constexpr std::array<std::uint32_t, 6> kBlendFactorTable{
        1U,
        3U,
        0U,
        0U,
        2U,
        3U,
    };
    SetAlphaFlags(0, 0, kBlendFactorTable[(materialFlags >> 16U) & 5U]);
    SetAlphaFlags(0, 1, kBlendFactorTable[(materialFlags >> 15U) & 5U]);
    SetRenderFlags(92, (materialFlags & 0x00c00000U) != 0x00800000U);
}

void ParseGeometryCommand(const std::uint16_t *&command) {
    const std::uint16_t opcode = command[0];
    std::uint32_t format = 0;
    if (opcode == 0x11 || opcode == 0x19)
        format = 2;
    else if (opcode == 0x12 || opcode == 0x1a)
        format = 1;
    else if (opcode == 0x14 || opcode == 0x1c)
        format = 3;
    const std::uint16_t stripCount = command[2];
    command += 3;
    for (std::uint16_t strip = 0; strip < stripCount; ++strip) {
        std::int16_t count{};
        std::memcpy(&count, command, sizeof(count));
        ++command;
        const std::uint16_t vertexCount = static_cast<std::uint16_t>(count < 0 ? -count : count);
        for (std::uint16_t vertex = 0; vertex < vertexCount; ++vertex) {
            ++command;
            if (format == 2 || format == 3)
                command += 2;
            if (format == 1 || format == 3)
                command += 2;
        }
    }
}


void PushStripMetaIntoShaderConstants(const std::uint16_t *&command, std::uint8_t shaderMode) {
    const auto *const bytes = reinterpret_cast<const std::uint8_t *>(command);
    Vector4 colour{
        static_cast<float>(bytes[2]) / 255.0f,
        static_cast<float>(bytes[1]) / 255.0f,
        static_cast<float>(bytes[0]) / 255.0f,
        static_cast<float>(bytes[3]) / 255.0f,
    };
    if ((shaderMode & 2U) != 0U) {
        colour.x = 0.0f;
        colour.y = 0.0f;
        colour.z = 0.0f;
    }
    if (g_perspectiveProjection == 1) {
        colour = {1.0f, 0.0f, 0.0f, colour.w};
    }
    g_renderParams.constants[4] = {};
    g_renderParams.constants[5] = colour;
    g_renderParams.constants[6] = {};
    g_renderParams.constants[7] = {0.0f, 1.0f, 0.0f, 0.0f};
    command += 4;
}

void DrawObject(D3TDeviceContext *context, GameNode *object, std::int32_t index, std::uint32_t startVertex, const MaterialState &material, bool transparent,
    std::uint8_t shaderMode, ShaderParams &parameters) {
    const auto *const records = &object->vertexCount;
    const std::int32_t surface = static_cast<std::int32_t>(records[index * 3 + 1]);
    const std::uint32_t materialFlags = records[index * 3 + 2];
    std::uint32_t texture = surface >= 0 ? GetPVRSurfaceTextureHandle(surface) : 0;
    if (!texture)
        texture = d3t::g_defaultWhiteTexture;
#ifdef SHENMUE_RT_ARCADE
    texture = RTArcade_ResolveTexture(texture);
#endif
    BindTexture(context, ResolveStripTextureHandle(texture));

    SetRenderFlags(147, (shaderMode & 4U) != 0U ? 2305U : 2304U);
    parameters.alphaTestEnabled = material.alphaTestEnabled;
    parameters.alphaReference = material.alphaReference;
    parameters.alphaFunction = material.alphaFunction;

    parameters.ignoreLighting = ((object->renderFlags & 2U) != 0U) || (!material.materialMode && !material.ignoreAlpha);
    parameters.ignoreAlpha = ((materialFlags | object->renderFlags) & 0x10U) != 0U;
        CommitLegacyRenderState(context, parameters, transparent);
    if ((shaderMode & 8U) != 0U) {
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        if (GetTexture2DDimensions(g_fullScreenTexture2D, &width, &height)) {
            MapForSuperSampling(context, static_cast<float>(width));
        }
    }

#if defined(_WIN32)
    const std::uint32_t vertexCount = records[index * 3];
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (native && vertexCount) {
        native->Draw(vertexCount, startVertex);
        ++g_drawCount;
    }
#else
    static_cast<void>(startVertex);
#endif
}

Matrix4x4 TransposeAttachmentTransform(const Matrix4x4 &transform) {
    Matrix4x4 transposed{};
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            (&transposed.rows[row].x)[column] = (&transform.rows[column].x)[row];
        }
    }
    return transposed;
}

void SetCachedAttachmentAlpha(std::uint64_t handle, float alpha) {
    const std::uint32_t cacheHandle = static_cast<std::uint32_t>(handle);
    const std::uint32_t key = static_cast<std::uint32_t>(handle >> 32U);
    if (!cacheHandle || !key)
        return;

    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(cacheHandle);
    if (!slot || !slot->entries || !slot->floatValues)
        return;
    const std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, key);
    std::uint16_t count = 0;
    std::memcpy(&count, slot->entries + static_cast<std::size_t>(index) * 20U + 6U, sizeof(count));
    std::fill_n(slot->floatValues + index, count, alpha);
}

} // namespace

bool CachedAttachmentOwnsSubmission(void *model, float alpha, const Matrix4x4 &absoluteTransformRoot) {
    const std::uintptr_t primary = reinterpret_cast<std::uintptr_t>(model);

    std::uintptr_t selected = primary;
    if (!g_forcePrimaryCachedModel && g_perspectiveProjection == 0 && ResolveHmdlEntry(primary + 1U)) {
        selected = primary + 1U;
    }

    auto *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(selected));
    if (!entry)
        return g_perspectiveProjection == 1;
    if (!entry->owner)
        return false;

    const std::uint32_t ownerFlags = entry->owner->flags;
    bool updateCache = true;
    bool directFallback = true;
    if ((ownerFlags & 0x10U) != 0U) {
        updateCache = g_perspectiveProjection != 1;
        directFallback = false;
    }
    if (g_perspectiveProjection == 4) {
        updateCache = false;
        directFallback = false;
    } else if (g_perspectiveProjection == 1) {
        directFallback = false;
    }

    if (updateCache && entry->state == 1) {
        if ((ownerFlags & 0x2002U) != 0U) {
            *g_matrixStack.top.stack = absoluteTransformRoot;
            const Matrix4x4 cachedTransform = TransposeAttachmentTransform(absoluteTransformRoot);
            static_cast<void>(AppendHashForSkinningBuffer(entry->handle, cachedTransform));
            if ((ownerFlags & 2U) != 0U) {
                SetCachedAttachmentAlpha(entry->handle, alpha);
                ModifyArrayElementsByMurmurHash(entry->handle, 1, 1);
            }
        }
        ModifyArrayElementsByMurmurHash(entry->handle, 8, 1);
        if (g_perspectiveProjection == 2)
            directFallback = false;
    }
    return !directFallback;
}

namespace {

void ApplySceneAttachmentTransform(const Vector3 &rootPosition, const Vector3i &rootRotation, const Vector3 &localPosition, const Vector3i &localRotation) {
    Matrix4x4::preTranslate(rootPosition.x, rootPosition.y, rootPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(rootRotation.y, rootRotation.x, rootRotation.z);
    Matrix4x4::preTranslate(localPosition.x, localPosition.y, localPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(localRotation.y, localRotation.x, localRotation.z);
}




void HMDL_PublishRigNodeTransforms(HmdlRigState &rig, Mt5Node *node, std::uint32_t flags) {
    while (node) {
        PushMatrix();
        if (const Matrix4x4 *const stored = node->transformOffset ? HMDL_TransformForNode(node, node->transformOffset) : nullptr) {
            workTransformMatrix = *stored;
        } else {
            Matrix4x4::preTranslate(node->posX, node->posY, node->posZ);
            if (node->rotZ)
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotZ));
            if (node->rotY)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotY));
            if (node->rotX)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotX));
            if ((flags & 1U) != 0U)
                Matrix4x4::Scale(node->scaleX, node->scaleY, node->scaleZ);
        }
        for (std::uint32_t index = 0; index < rig.recordCount; ++index) {
            if (rig.records[index].node != node)
                continue;
            const std::int32_t operation = (flags & 2U) != 0U ? (node->nodeFlags == 0 ? 1 : 0) : 1;
            const std::uint64_t handle = rig.cacheHandle | (static_cast<std::uint64_t>(rig.records[index].nodeHash) << 32U);
            if (rig.cacheHandle && rig.records[index].nodeHash) {
                const Matrix4x4 transposed = TransposeAttachmentTransform(workTransformMatrix);
                HMDL_SetRigNodeVisibility(handle, rig, operation);
                static_cast<void>(AppendHashForSkinningBuffer(handle, transposed));
            }
            break;
        }
        HMDL_PublishRigNodeTransforms(rig, OffsetNode(node->childOffset), flags);
        PopMatrix();
        node = OffsetNode(node->siblingOffset);
    }
}



void HMDL_ConvertRigNodesToSkinSpace(const HmdlRigState &rig, std::int32_t mode) {
    std::uint64_t rootHandle = 0;
    Matrix4x4 rootWorld{};
    for (std::uint32_t index = 0; index < rig.recordCount; ++index) {
        const OsageNodeRecord &record = rig.records[index];
        if (!record.node || !rig.cacheHandle || !record.nodeHash)
            continue;
        const std::uint64_t handle = rig.cacheHandle | (static_cast<std::uint64_t>(record.nodeHash) << 32U);
        Matrix4x4 published{};
        if (ConvertSkinningNodeToParentSpace(handle, published) && !index && !mode) {
            rootHandle = handle;
            rootWorld = published;
        }
    }
    if (rootHandle)
        static_cast<void>(AppendHashForSkinningBuffer(rootHandle, rootWorld));
}



OsageRenderSlot *HMDL_AcquireRigOsageSlot(HmdlRigState &rig, Mt5Node *node, std::uint32_t &slotIndex) {
    if (!node)
        return nullptr;
    for (std::uint32_t index = 0; index < 8; ++index) {
        if (rig.osageSlots[index].node == node) {
            slotIndex = index;
            return &rig.osageSlots[index];
        }
    }
    for (std::uint32_t index = 0; index < 8; ++index) {
        if (rig.osageSlots[index].node)
            continue;
        OsageRenderSlot &slot = rig.osageSlots[index];
        std::memset(&slot, 0, sizeof(slot));
        slot.index = index;
        slot.node = node;
        slotIndex = index;
        return &slot;
    }
    return nullptr;
}





void HMDL_CollectRigRecords(Mt5Node *node, Character *character, HmdlRigState &rig, std::uint32_t flags) {
    while (node) {
        const std::uint32_t count = rig.recordCount;
        if (count >= 300)
            return;
        const std::int32_t kind = static_cast<std::int16_t>(node->nodeKind);
        bool keep = !(rig.modelType > 2 && static_cast<std::uint32_t>(kind - 77) <= 1U);
        if (keep && rig.modelType == 500)
            keep = (kind <= 23 && kind >= 0 && ((0xe75ef2U >> kind) & 1U) != 0U) || static_cast<std::uint32_t>(kind + 67) <= 2U;
        if (keep) {
            OsageNodeRecord &record = rig.records[count];
            rig.recordCount = count + 1;
            record.node = node;
            record.nodeKind = kind;
            record.state = 1;
            const std::byte *const attachment = OsageAttachmentForNode(*node);
            if (attachment && static_cast<std::uint16_t>(node->nodeKind + 0x4e) <= 8U && std::memcmp(attachment, "OSAG", 4) != 0) {
                if (OsageRenderSlot *const slot = HMDL_AcquireRigOsageSlot(rig, node, record.slotIndex)) {
                    if (!slot->active) {
                        slot->active = 1;
                        std::memcpy(&slot->attachmentValue, attachment + 8, sizeof(slot->attachmentValue));
                    }
                    if (slot->active)
                        record.state = 2;
                }
            }
            if (character) {
                HMDL_CreateHandAttachmentRecords(node, character, rig.handRecords, record.state, record.slotIndex, flags);
                if (kind == -67) {
                    record.state = 4;
                    std::memset(&rig.headRig, 0, sizeof(rig.headRig));
                    rig.headRig.active = 1;
                }
            }
        }
        HMDL_CollectRigRecords(OffsetNode(node->childOffset), character, rig, flags);
        node = OffsetNode(node->siblingOffset);
    }
}



std::uint32_t HMDL_SkeletonNodeVersion(const char *name, const char *boneName) {
    if (!name || !std::strstr(name, "CSESkeleton"))
        return 0;
    const char *const version = std::strstr(name, "VERSION");
    const char *const marker = version ? version + 7 : std::strchr(name, 'V');
    if (!marker)
        return 0;
    const std::int32_t value = std::atoi(version ? marker : marker + 1);
    if (value > 1 && value != 500 && !std::strstr(name, boneName))
        return 0;
    return static_cast<std::uint32_t>(value - 1) <= 5U || value == 500 ? static_cast<std::uint32_t>(value) : 0U;
}


std::uint64_t HMDL_FindSkeletonRoot(const char *boneName, std::uint32_t &modelType, std::uint64_t handle) {
    if (const std::uint32_t version = HMDL_SkeletonNodeVersion(GetSkinningNodeName(handle), boneName)) {
        modelType = version;
        return handle;
    }
    const std::uint32_t childCount = handle && (handle >> 32U) ? GetSkinningNodeChildCount(handle) : 0U;
    for (std::uint32_t child = 0; child < childCount; ++child) {
        const std::uint64_t found = HMDL_FindSkeletonRoot(boneName, modelType, GetSkinningNodeChild(handle, child));
        if (found && (found >> 32U))
            return found;
    }
    return 0;
}


constexpr std::array<std::array<const char *, 7>, 3> kSharedSkeletonGroups{{
    {"ykb_m", "ykb_z", "ykc_m", "ykd_m", "yke_m", "ykf_m", "ykg_m"},
    {"fuc_m", "fub_m", "fuk_m", nullptr, nullptr, nullptr, nullptr},
    {"ame_l", "amg_l", nullptr, nullptr, nullptr, nullptr, nullptr},
}};



std::uint64_t HMDL_FindSharedSkeletonRoot(HmdlRigState &rig, std::uint64_t handle) {
    for (const std::array<const char *, 7> &group : kSharedSkeletonGroups) {
        bool member = false;
        for (const char *const name : group) {
            if (!name)
                continue;
            std::size_t index = 0;
            while (std::tolower(static_cast<unsigned char>(rig.boneName[index])) == std::tolower(static_cast<unsigned char>(name[index])) && name[index])
                ++index;
            if (!rig.boneName[index] && !name[index]) {
                member = true;
                break;
            }
        }
        if (!member)
            continue;
        for (const char *const name : group) {
            if (!name)
                continue;
            const std::uint64_t found = HMDL_FindSkeletonRoot(name, rig.modelType, handle);
            if (found && (found >> 32U)) {
                std::strcpy(rig.boneName, name);
                return found;
            }
        }
    }
    return 0;
}



std::uint32_t HMDL_InitRigState(RenderArrayEntry &entry) {
    if (entry.state != 1 || !entry.handle || !(entry.handle >> 32U))
        return 0;
    HmdlRigState &rig = *reinterpret_cast<HmdlRigState *>(entry.rigState);
    const char *const path = entry.owner->path;
    const char *stem = path;
    std::size_t end = std::strlen(path);
    while (end > 1) {
        if (path[end - 1] == '/' || path[end - 1] == '\\') {
            stem = path + end;
            break;
        }
        --end;
    }
    std::snprintf(rig.boneName, sizeof(rig.boneName), "%s", stem);
    if (char *const dot = std::strchr(rig.boneName, '.'))
        *dot = '\0';
    std::uint64_t root = HMDL_FindSkeletonRoot(rig.boneName, rig.modelType, entry.handle);
    if (!root || !(root >> 32U))
        root = HMDL_FindSharedSkeletonRoot(rig, entry.handle);
    if (!root || !(root >> 32U) || !rig.modelType)
        return 0;
    rig.cacheHandle = static_cast<std::uint32_t>(entry.handle);
    rig.entryNodeHash = static_cast<std::uint32_t>(entry.handle >> 32U);
    rig.skeletonRootHash = static_cast<std::uint32_t>(root >> 32U);
    return rig.modelType;
}


constexpr std::array<std::array<const char *, 2>, 61> kRigNodeNameAliases{{
    {"Node_fuk_m_024_KP00122", "Node_fuk_m_030_KP00122"},
    {"Node_fuk_m_025_KP00122", "Node_fuk_m_031_KP00122"},
    {"Node_fuk_m_026_KP00122", "Node_fuk_m_032_KP00122"},
    {"Node_fuk_m_027_KP00122", "Node_fuk_m_033_KP00122"},
    {"Node_fuk_m_028_KP00122", "Node_fuk_m_034_KP00122"},
    {"Node_fuk_m_029_KP00122", "Node_fuk_m_035_KP00122"},
    {"Node_fuk_m_030_KP00016", "Node_fuk_m_024_KP00016"},
    {"Node_fuk_m_031_KP00017", "Node_fuk_m_025_KP00017"},
    {"Node_fuk_m_032_KP00018", "Node_fuk_m_026_KP00018"},
    {"Node_fuk_m_033_KP00021", "Node_fuk_m_027_KP00021"},
    {"Node_fuk_m_034_KP00022", "Node_fuk_m_028_KP00022"},
    {"Node_fuk_m_035_KP00023", "Node_fuk_m_029_KP00023"},
    {"Node_ine_m_013_KP00009", "Node_ine_m_022_KP00009"},
    {"Node_ine_m_014_KP00010", "Node_ine_m_023_KP00010"},
    {"Node_ine_m_015_KP00011", "Node_ine_m_024_KP00011"},
    {"Node_ine_m_016_KM00066", "Node_ine_m_025_KM00066"},
    {"Node_ine_m_017_KP00040", "Node_ine_m_026_KP00040"},
    {"Node_ine_m_018_KP00043", "Node_ine_m_027_KP00043"},
    {"Node_ine_m_019_KP00044", "Node_ine_m_028_KP00044"},
    {"Node_ine_m_020_KP00046", "Node_ine_m_029_KP00046"},
    {"Node_ine_m_021_KP00047", "Node_ine_m_030_KP00047"},
    {"Node_ine_m_022_KP00004", "Node_ine_m_013_KP00004"},
    {"Node_ine_m_023_KP00005", "Node_ine_m_014_KP00005"},
    {"Node_ine_m_024_KP00006", "Node_ine_m_015_KP00006"},
    {"Node_ine_m_025_KM00065", "Node_ine_m_016_KM00065"},
    {"Node_ine_m_026_KP00025", "Node_ine_m_017_KP00025"},
    {"Node_ine_m_027_KP00028", "Node_ine_m_018_KP00028"},
    {"Node_ine_m_028_KP00029", "Node_ine_m_019_KP00029"},
    {"Node_ine_m_029_KP00031", "Node_ine_m_020_KP00031"},
    {"Node_ine_m_030_KP00032", "Node_ine_m_021_KP00032"},
    {"Node_amg_l_002_KM00001", "Node_amg_l_027_KM00001"},
    {"Node_amg_l_004_KP00120", "Node_amg_l_029_KP00120"},
    {"Node_amg_l_005_KP00120", "Node_amg_l_030_KP00120"},
    {"Node_amg_l_006_KP00120", "Node_amg_l_031_KP00120"},
    {"Node_amg_l_007_KP00120", "Node_amg_l_032_KP00120"},
    {"Node_amg_l_008_KP00120", "Node_amg_l_033_KP00120"},
    {"Node_amg_l_009_KP00120", "Node_amg_l_034_KP00120"},
    {"Node_amg_l_010_KP00120", "Node_amg_l_035_KP00120"},
    {"Node_amg_l_011_KP00120", "Node_amg_l_036_KP00120"},
    {"Node_amg_l_012_KP00120", "Node_amg_l_037_KP00120"},
    {"Node_amg_l_013_KP00120", "Node_amg_l_038_KP00120"},
    {"Node_amg_l_014_KP00120", "Node_amg_l_039_KP00120"},
    {"Node_amg_l_015_KP00120", "Node_amg_l_040_KP00120"},
    {"Node_amg_l_016_KP00120", "Node_amg_l_041_KP00120"},
    {"Node_amg_l_017_KP00120", "Node_amg_l_042_KP00120"},
    {"Node_amg_l_018_KP00120", "Node_amg_l_043_KP00120"},
    {"Node_amg_l_019_KP00120", "Node_amg_l_044_KP00120"},
    {"Node_amg_l_020_KP00120", "Node_amg_l_045_KP00120"},
    {"Node_amg_l_021_KP00120", "Node_amg_l_046_KP00120"},
    {"Node_amg_l_022_KP00120", "Node_amg_l_047_KP00120"},
    {"Node_amg_l_023_KP00120", "Node_amg_l_048_KP00120"},
    {"Node_amg_l_024_KP00120", "Node_amg_l_049_KP00120"},
    {"Node_amg_l_025_KP00120", "Node_amg_l_050_KP00120"},
    {"Node_amg_l_026_KP00120", "Node_amg_l_051_KP00120"},
    {"Node_amg_l_027_KP00120", "Node_amg_l_052_KP00120"},
    {"Node_amg_l_028_KP00120", "Node_amg_l_053_KP00120"},
    {"Node_amg_l_029_KP00120", "Node_amg_l_054_KP00120"},
    {"Node_amg_l_030_KP00120", "Node_amg_l_055_KP00120"},
    {"Node_amg_l_031_KP00120", "Node_amg_l_056_KP00120"},
    {"Node_bti_l_000_KP00001", "Node_bti_l_002_KP00001"},
    {"Node_bti_l_002_KM00001", "Node_bti_l_005_KM00001"},
}};


std::uint64_t HMDL_FindSkinNodeWithText(const char *text, std::uint64_t handle) {
    const char *const name = GetSkinningNodeName(handle);
    if (name && std::strstr(name, text))
        return handle;
    const std::uint32_t childCount = handle && (handle >> 32U) ? GetSkinningNodeChildCount(handle) : 0U;
    for (std::uint32_t child = 0; child < childCount; ++child) {
        if (const std::uint64_t found = HMDL_FindSkinNodeWithText(text, GetSkinningNodeChild(handle, child)))
            return found;
    }
    return 0;
}

bool HMDL_RigHasNodeHash(const HmdlRigState &rig, std::uint32_t hash) {
    for (std::uint32_t index = 0; index < rig.recordCount; ++index) {
        if (rig.records[index].nodeHash == hash)
            return true;
    }
    return false;
}

bool HMDL_NamesMatch(const char *left, const char *right) {
    std::size_t index = 0;
    while (std::tolower(static_cast<unsigned char>(left[index])) == std::tolower(static_cast<unsigned char>(right[index])) && right[index])
        ++index;
    return !left[index] && !right[index];
}




std::uint64_t HMDL_AssignRigNodeHash(const char *name, const HmdlRigState &rig, const RenderArrayEntry &entry, std::int32_t nodeKind) {
    std::uint64_t result = AssignNode(entry.handle, name);
    if (result && (result >> 32U) && !HMDL_RigHasNodeHash(rig, static_cast<std::uint32_t>(result >> 32U)))
        return result;
    for (const std::array<const char *, 2> &alias : kRigNodeNameAliases) {
        if (!HMDL_NamesMatch(alias[0], name))
            continue;
        result = AssignNode(entry.handle, alias[1]);
        if (result && (result >> 32U) && !HMDL_RigHasNodeHash(rig, static_cast<std::uint32_t>(result >> 32U)))
            return result;
    }
    if (!entry.owner || entry.owner->remapType != 1 || nodeKind == -1)
        return 0;
    char suffix[256]{};
    std::snprintf(suffix, sizeof(suffix), "_K%s%05u", nodeKind < 0 ? "M" : "P", static_cast<unsigned>(nodeKind < 0 ? -nodeKind : nodeKind));
    result = HMDL_FindSkinNodeWithText(suffix, entry.handle);
    if (result && (result >> 32U) && !HMDL_RigHasNodeHash(rig, static_cast<std::uint32_t>(result >> 32U)))
        return result;
    return 0;
}



void HMDL_RigNodeName(char (&name)[256], std::uint32_t recordIndex, std::int32_t nodeKind, const char *boneName, std::uint32_t modelType) {
    const char *const sign = nodeKind < 0 ? "M" : "P";
    const unsigned magnitude = static_cast<unsigned>(static_cast<std::uint16_t>(nodeKind < 0 ? -nodeKind : nodeKind));
    if (modelType == 1)
        std::snprintf(name, sizeof(name), "Node_%03u_K%s%05u", recordIndex, sign, magnitude);
    else
        std::snprintf(name, sizeof(name), "Node_%s_%03u_K%s%05u", boneName, recordIndex, sign, magnitude);
}



void HMDL_NameHandNodes(HandSkinningAttachmentRecord &hand, std::uint32_t modelType, const char *boneName) {
    if (hand.ready)
        return;
    hand.ready = 1;
    if (!hand.cacheHandle || !hand.nodeHash)
        return;
    const std::uint64_t base = hand.cacheHandle | (static_cast<std::uint64_t>(hand.nodeHash) << 32U);
    const std::uint32_t subtree = GetSkinningNodeDescendantCount(base) + 1U;
    const std::uint32_t count = subtree > 20U ? 20U : subtree;
    const std::int32_t handId = hand.nodeKind < 0 ? -hand.nodeKind : hand.nodeKind;
    for (std::uint32_t index = 0; index < count; ++index) {
        char name[256]{};
        const unsigned matrix = hand.matrixIndices[index];
        if (modelType == 1)
            std::snprintf(name, sizeof(name), "Node_Hand_%d_%03u_%03u", handId, index, matrix);
        else
            std::snprintf(name, sizeof(name), "Node_%s_Hand_%02u_%03u_%03u", boneName, static_cast<unsigned>(handId), index, matrix);
        const std::uint64_t node = AssignNode(base, name);
        if (node && (node >> 32U))
            hand.boneHashes[index] = static_cast<std::uint32_t>(node >> 32U);
    }
}


void HMDL_NameFlagNodes(HmdlRigFlagSlot &slot, std::uint32_t slotIndex, const char *boneName) {
    if (slot.assigned)
        return;
    slot.assigned = 1;
    if (!slot.cacheHandle || !slot.baseHash)
        return;
    const std::uint64_t base = slot.cacheHandle | (static_cast<std::uint64_t>(slot.baseHash) << 32U);
    const std::uint32_t children = GetSkinningNodeChildCount(base);
    const std::uint32_t count = children > 128U ? 128U : children;
    for (std::uint32_t index = 0; index < count; ++index) {
        char name[256]{};
        std::snprintf(name, sizeof(name), "Node_%s_Flag_%03u_%03u", boneName, slotIndex, index);
        const std::uint64_t node = AssignNode(base, name);
        if (node && (node >> 32U))
            slot.nodeHashes[index] = static_cast<std::uint32_t>(node >> 32U);
    }
}


void HMDL_NameClothNodes(OsageRenderSlot &slot, std::uint32_t slotIndex, const char *boneName, std::uint32_t modelType) {
    if (slot.assigned)
        return;
    slot.assigned = 1;
    if (!slot.cacheHandle || !slot.baseHash)
        return;
    const std::uint64_t base = slot.cacheHandle | (static_cast<std::uint64_t>(slot.baseHash) << 32U);
    const std::uint32_t children = GetSkinningNodeChildCount(base);
    const std::uint32_t count = children > 180U ? 180U : children;
    for (std::uint32_t index = 0; index < count; ++index) {
        char name[256]{};
        if (modelType == 1)
            std::snprintf(name, sizeof(name), "Node_Cloth_%03u_%03u", slotIndex, index);
        else
            std::snprintf(name, sizeof(name), "Node_%s_Cloth_%03u_%03u", boneName, slotIndex, index);
        const std::uint64_t node = AssignNode(base, name);
        if (node && (node >> 32U))
            slot.nodeHashes[index] = static_cast<std::uint32_t>(node >> 32U);
    }
}




void HMDL_NameRigRecords(RenderArrayEntry &entry) {
    HmdlRigState &rig = *reinterpret_cast<HmdlRigState *>(entry.rigState);
    if (!rig.initialized || rig.ready || !entry.handle || !(entry.handle >> 32U))
        return;
    rig.ready = 1;
    if (!rig.modelType)
        return;
    for (std::uint32_t index = 0; index < rig.recordCount; ++index) {
        OsageNodeRecord &record = rig.records[index];
        char name[256]{};
        if (record.node && record.state == 3 && record.slotIndex < 2) {
            HandSkinningAttachmentRecord &hand = rig.handRecords[record.slotIndex];
            const std::int32_t handId = hand.nodeKind < 0 ? -hand.nodeKind : hand.nodeKind;
            const unsigned matrix = hand.matrixIndices[0];
            if (rig.modelType == 1)
                std::snprintf(name, sizeof(name), "Node_Hand_%d_%03u_%03u", handId, 0U, matrix);
            else
                std::snprintf(name, sizeof(name), "Node_%s_Hand_%02u_%03u_%03u", rig.boneName, static_cast<unsigned>(handId), 0U, matrix);
            const std::uint64_t node = HMDL_AssignRigNodeHash(name, rig, entry, record.nodeKind);
            if (node && (node >> 32U)) {
                record.nodeHash = static_cast<std::uint32_t>(node >> 32U);
                hand.cacheHandle = rig.cacheHandle;
                hand.nodeHash = record.nodeHash;
                HMDL_NameHandNodes(hand, rig.modelType, rig.boneName);
            }
        }
        if (!record.node)
            continue;
        if (record.state == 4) {
            HMDL_RigNodeName(name, index, record.nodeKind, rig.boneName, rig.modelType);
            const std::uint64_t node = HMDL_AssignRigNodeHash(name, rig, entry, record.nodeKind);
            if (node && (node >> 32U)) {
                record.nodeHash = static_cast<std::uint32_t>(node >> 32U);
                rig.headRig.cacheHandle = rig.cacheHandle;
                rig.headRig.sourceHash = record.nodeHash;
                HeadRig_Init(&rig.headRig, static_cast<std::uint32_t>(entry.handle >> 32U));
            }
        }
        HMDL_RigNodeName(name, index, record.nodeKind, rig.boneName, rig.modelType);
        const std::uint64_t node = HMDL_AssignRigNodeHash(name, rig, entry, record.nodeKind);
        if (node && (node >> 32U))
            record.nodeHash = static_cast<std::uint32_t>(node >> 32U);
        else if (record.state == 1)
            record.node = nullptr;
        if (!record.node)
            continue;
        if (record.state == 5 && record.slotIndex < 32) {
            HmdlRigFlagSlot &slot = rig.flagSlots[record.slotIndex];
            slot.cacheHandle = rig.cacheHandle;
            slot.baseHash = record.nodeHash;
            HMDL_NameFlagNodes(slot, record.slotIndex, rig.boneName);
        }
        if (record.node && record.state == 2 && record.slotIndex < 8) {
            OsageRenderSlot &slot = rig.osageSlots[record.slotIndex];
            slot.cacheHandle = rig.cacheHandle;
            slot.baseHash = record.nodeHash;
            HMDL_NameClothNodes(slot, record.slotIndex, rig.boneName, rig.modelType);
        }
    }
}

void PrepareActiveRiggedNodeTree(Mt5Node *node, const Matrix4x4 &absoluteTransformRoot) {
    if (g_characterShadowPass)
        return;
    PushMatrix();
    g_mt5RootTransform = absoluteTransformRoot;
    PrepareRiggedNodeTree(node);
    PopMatrix();
}

void DrawActiveRiggedNodeTree(Mt5Node *node, float alpha, const Matrix4x4 &absoluteTransformRoot, bool includeRootSiblings, std::uint32_t mode) {
    const std::int32_t siblingOffset = node->siblingOffset;
    if (!includeRootSiblings)
        node->siblingOffset = 0;
    PushMatrix();
    g_mt5RootTransform = absoluteTransformRoot;
    d3t::MT5_TransformAndSubmitNodeTree(node, alpha, mode);
    PopMatrix();
    if (!includeRootSiblings)
        node->siblingOffset = siblingOffset;
}



void HMDL_TransformObjectTree(void *model, Mt5Node *node) {
    while (node) {
        PushMatrix();
        if (const Matrix4x4 *const stored = node->transformOffset ? HMDL_TransformForNode(node, node->transformOffset) : nullptr) {
            workTransformMatrix = *stored;
        } else {
            Matrix4x4::preTranslate(node->posX, node->posY, node->posZ);
            if (node->rotZ)
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotZ));
            if (node->rotY)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotY));
            if (node->rotX)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotX));
            Matrix4x4::Scale(node->scaleX, node->scaleY, node->scaleZ);
        }
        if (static_cast<std::int16_t>(node->nodeKind) == -67 && g_pCurrentFaceActor)
            UpdateDebugFaceEyeTargets(model, *static_cast<const FaceWork *>(g_pCurrentFaceActor));
        HMDL_TransformObjectTree(model, OffsetNode(node->childOffset));
        PopMatrix();
        node = OffsetNode(node->siblingOffset);
    }
}


void HMDL_UpdateMutableFaceGeometry(void *model) {
    Matrix4x4::SetActiveTransformToPrePass();
    HMDL_TransformObjectTree(model, static_cast<Mt5Node *>(model));
    PopMatrix();
}

} // namespace






std::uint32_t SubmitRiggedSceneObjectForDrawing(void *model, Character *character, std::int32_t mode, std::uint32_t flags) {
    RenderArrayEntry *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model)));
    if (!entry || entry->state != 1)
        return 0;
    HmdlRigState &rig = *reinterpret_cast<HmdlRigState *>(entry->rigState);
    void *tree = model;
    if ((entry->flags & 0x10U) != 0U && entry->owner)
        tree = reinterpret_cast<void *>(entry->owner->handle);
    GAME_BindModelOffsets(tree, character);
    if (tree && model && !rig.initialized) {
        rig.initialized = 1;
        rig.modelType = HMDL_InitRigState(*entry);
        if (rig.modelType)
            HMDL_CollectRigRecords(static_cast<Mt5Node *>(tree), character, rig, flags);
    }
    std::uint32_t result = 0;
    if (rig.initialized) {
        HMDL_NameRigRecords(*entry);
        if (rig.ready && rig.modelType && (flags & 4U) == 0U) {
            PushMatrix();
            workTransformMatrix = transformMatrix_prePass;
            HMDL_PublishRigNodeTransforms(rig, static_cast<Mt5Node *>(tree), flags);
            PopMatrix();
            HMDL_ConvertRigNodesToSkinSpace(rig, mode);
            std::uint32_t activeSlots = 0;
            std::uint32_t writes = 0;
            for (OsageRenderSlot &slot : rig.osageSlots) {
                if (!slot.node)
                    continue;
                activeSlots += slot.active ? 1U : 0U;
                writes += ProcessOsageSlot(slot);
            }
            if (g_recordingRyoDraw) {
                g_characterDrawDiagnostics.ryoOsageSlots = activeSlots;
                g_characterDrawDiagnostics.ryoOsageWrites = writes;
            }
            HeadRig_Publish(&rig.headRig);
            HMDL_UpdateMutableFaceGeometry(model);
            if (entry->cewpInitialized) {
                HMDL_PerformFacialExpressions(*entry);
                HMDL_PerformBlink(*entry, 0);
                HMDL_PerformBlink(*entry, 1);
            }
            result = 1;
        }
    }
    GAME_RestoreModelOffsets(tree, character);
    return result;
}


void HMDL_ClearNodeTransformLinks(Mt5RuntimeNode *node) {
    while (node) {
        node->transformOffset = 0;
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->childOffset)
            HMDL_ClearNodeTransformLinks(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset));
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset) : nullptr;
    }
}



void HMDL_LinkNodeTransformsByKind(Mt5RuntimeNode *source, Mt5RuntimeNode *target) {
    while (target) {
        if (target->nodeKind != 0xffffU) {
            if (const Mt5RuntimeNode *const node = shenGetNodeByChildIdx(source, static_cast<std::int16_t>(target->nodeKind)))
                target->transformOffset = node->transformOffset ? HMDL_TransformOffsetForNode(target, HMDL_TransformForNode(node, node->transformOffset)) : 0;
        }
        std::byte *const base = MT5_ModelBaseForNode(target);
        if (target->childOffset)
            HMDL_LinkNodeTransformsByKind(source, reinterpret_cast<Mt5RuntimeNode *>(base + target->childOffset));
        target = target->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + target->siblingOffset) : nullptr;
    }
}

namespace {
void SubmitCharacterShadowInternal(Character *character, void *posedModel, void *model, const Vector3 &position, const Vector3i *rotation, const Vector3 *scale,
    float groundHeight, float direction) {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !model || direction == 0.0f || groundHeight >= g_CameraEyePos.y || !std::isfinite(groundHeight) || !std::isfinite(direction)) {
        return;
    }
    // DoFullDedicatedShadow stops at a perspective projection before tracking lights; SubmitForRender only at its first lit slot.
    const bool posed = !rotation || !scale;
    if (posed && g_perspectiveProjection != 0)
        return;
    // SubmitForRender always clears the shadow model's links and DoFullDedicatedShadow only for a different posed model; both then
    // relink them from a distinct posed model (MOTM passes one model).
    auto *const shadowRoot = static_cast<Mt5RuntimeNode *>(model);
    if (!posed || posedModel != model)
        HMDL_ClearNodeTransformLinks(shadowRoot);
    if (posedModel && posedModel != model) {
        HMDL_ClearNodeTransformLinks(shadowRoot);
        HMDL_LinkNodeTransformsByKind(static_cast<Mt5RuntimeNode *>(posedModel), shadowRoot);
    }


    CharacterShadowState *const state = character ? &character->shadowState : nullptr;
    if (!state && g_prevCachedGlobalLightingAdjustFlag == 3)
        return;

    float distanceFade = 0.0f;
    std::int32_t lightCount = 0;
    std::int32_t lightMode = 0;
    const std::uint32_t lightMask = ComputeShadowFadeAndActiveLights(position, distanceFade, state, character, lightCount, lightMode);
    const bool dynamicLights = lightMode == 1;

    const ShadowPlane plane{{0.0f, static_cast<float>(static_cast<double>(groundHeight) + 0.013), 0.0f}, {0.0f, 1.0f, 0.0f}};
    Vector3 lightPoint{};
    CharacterShadowCallbackState savedState(context);
    g_characterShadowPass = true;
    g_alphaMaskedDepthPass = false;
    g_recordingRyoDraw = false;
    for (std::int32_t slot = 0; slot < lightCount; ++slot) {
        if (dynamicLights && state->lightIndex[slot] < 0)
            continue;
        if (((lightMask >> slot) & 1U) == 0U)
            continue;
        const ShenmueLightSlot *const light = dynamicLights ? LGHT_GetDataSlot(static_cast<std::uint32_t>(state->lightIndex[slot])) : LGHT_GetRenderSlot(slot);
        if (!light)
            continue;


        const float alpha = RecalculateLightParams(position, lightPoint, state, *light, direction, slot, dynamicLights) * distanceFade;
        if (alpha == 0.0f)
            continue;
        if (!posed && g_perspectiveProjection != 0)
            break;


        PushMatrix();
        Matrix4x4_ApplyPlanarShadowProjection(plane, {position.x - lightPoint.x, position.y - lightPoint.y, position.z - lightPoint.z}, {});
        if (posed) {
            Recursive_SubmitRiggedNodes(model, alpha, workTransformMatrix, character);
            PopMatrix();
            continue;
        }

        Matrix4x4::preTranslate(position.x, position.y, position.z);
        if (rotation->z) {
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation->z));
        }
        if (rotation->y) {
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation->y));
        }
        if (rotation->x) {
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation->x));
        }

        float maxScale = 1.0f;
        if (scale->x != 1.0f || scale->y != 1.0f || scale->z != 1.0f) {
            Matrix4x4::Scale(scale->x, scale->y, scale->z);
            const float wider = scale->y > scale->x ? scale->y : scale->x;
            maxScale = scale->z > wider ? scale->z : wider;
        }
        g_sceneObjectMaxScale = maxScale;
        d3t::MT5_PushAndSubmitNodeTree(model, alpha);
        g_sceneObjectMaxScale = 1.0f;
        PopMatrix();
    }
}
} // namespace

void SubmitCharacterShadow(Character *character, void *posedModel, void *model, const Vector3 &position, const Vector3i &rotation, const Vector3 &scale,
    float groundHeight, float direction) {
    SubmitCharacterShadowInternal(character, posedModel, model, position, &rotation, &scale, groundHeight, direction);
}

void SubmitPosedCharacterShadow(Character *character, void *model, const Vector3 &position, float groundHeight, float direction) {
    SubmitCharacterShadowInternal(character, model, model, position, nullptr, nullptr, groundHeight, direction);
}


void TransformPrelightLights(EffectLightRenderSlot *lights, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index) {
        EffectLightRenderSlot &light = lights[index];
        light.shaderConstants04[5] = light.intensity * light.colourR;
        light.shaderConstants04[6] = light.intensity * light.colourG;
        light.shaderConstants04[7] = light.intensity * light.colourB;
        Matrix4x4::Transform(light.negativeNormalizedDirection, light.transformedDirection);
        Matrix4x4::TransformPoint(light.position, light.transformedPosition);
    }
}

namespace {



void TranslateAllLocalToWorld(EffectLightRenderSlot *lights, std::int32_t count) {
    PushMatrix();
    Matrix4x4::NormalizeAndSetTransformMatrix();
    for (std::int32_t index = 0; index < count; ++index) {
        EffectLightRenderSlot &light = lights[index];
        Vector3 direction{};
        Vector3 position{};
        Matrix4x4::Transform(light.transformedDirection, direction);
        light.shaderConstants5C[3] = direction.x;
        light.shaderConstants5C[4] = direction.y;
        light.shaderConstants5C[5] = direction.z;
        Matrix4x4::TransformPoint(light.transformedPosition, position);
        light.shaderConstants5C[0] = position.x;
        light.shaderConstants5C[1] = position.y;
        light.shaderConstants5C[2] = position.z;
    }
    PopMatrix();
}

} // namespace





std::int32_t ModelCopyToGPU(void *binding, std::int32_t *attachment, EffectLightRenderSlot *lights, std::int32_t count) {
    std::uint32_t &bindingFlags = *static_cast<std::uint32_t *>(binding);
    if ((bindingFlags & 0x10U) != 0U)
        return 0;
    TranslateAllLocalToWorld(lights, count);
    const GameNode *const entry = FindSceneEntityByID(SceneKey(binding));
    if (!entry || !entry->bufferHandle)
        return 0;
    const std::int32_t source = entry->bufferHandle;
    const std::int32_t size = GetSizeForModelBuffer(source);
    const std::uint32_t vertexCount = static_cast<std::uint32_t>(size) >> 6U;
    const BufferDescription description{
        .flags = 1, .size = size, .count = 1, .flags3 = 3, .buffer = GetWriteMappedBufferPointer(DX11::g_ppD3DDeviceContext, source, 1, 0), .name = nullptr};
    const std::int32_t copy = CreateBuffer(&description);
    DX11_CopySceneNodeToGPU(DX11::g_ppD3DDeviceContext, source);
    g_UploadedSceneRenderHandles[g_UploadedSceneRenderHandleCount++] = copy;
    *attachment = copy;
    static_cast<void>(GetSizeForModelBuffer(copy));
    if (float *const vertices = static_cast<float *>(GetWriteMappedBufferPointer(DX11::g_ppD3DDeviceContext, copy, 1, 0))) {
        bindingFlags |= 0x10U;
        for (std::uint32_t vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex) {
            float *const vertex = vertices + vertexIndex * 16U;
            vertex[15] = 1.0f;
            for (std::int32_t index = 0; index < count; ++index) {
                const EffectLightRenderSlot &light = lights[index];
                if (light.modeFlags != 11)
                    continue;
                const float dx = light.shaderConstants5C[0] - vertex[0];
                const float dy = light.shaderConstants5C[1] - vertex[1];
                const float dz = light.shaderConstants5C[2] - vertex[2];
                const float distanceSquared = (dy * dy + dx * dx) + dz * dz;
                if (distanceSquared > light.range * light.range)
                    continue;
                const float inverseDistance = 1.0f / std::sqrt(distanceSquared);
                if (!(inverseDistance > 0.0f))
                    continue;
                const float facing = (dy * vertex[4] + dx * vertex[3]) + dz * vertex[5];
                if (!(facing > 0.0f))
                    continue;
                float falloff = inverseDistance * inverseDistance;
                falloff *= falloff;
                falloff *= falloff;
                falloff *= light.attenuationShape;
                const float strength = facing * inverseDistance * (1.0f < falloff ? 1.0f : falloff);
                for (std::uint32_t channel = 0; channel < 3; ++channel) {
                    const float colour = light.shaderConstants04[5 + channel];
                    vertex[12 + channel] = (colour < 1.0f ? colour : 1.0f) * strength + vertex[12 + channel];
                }
            }
        }
    }
    DX11_CopySceneNodeToGPU(DX11::g_ppD3DDeviceContext, copy);
    return 0;
}



void HMDL_PrelightNodeRecursive(Mt5RuntimeNode *node, EffectLightRenderSlot *lights, std::int32_t count) {
    std::byte *const modelBase = MT5_ModelBaseForNode(node);
    for (;;) {
        if (node->siblingOffset)
            PushMatrix();
        if (node->transformOffset) {
            workTransformMatrix = g_mt5RootTransform;
            Matrix4x4::Multiply(HMDL_TransformForNode(node, node->transformOffset));
        } else {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            if (node->rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
            if (node->rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
            if (node->rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
            if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
                Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        }
        if (void *const binding = MT5_SceneBindingForNode(node, nullptr)) {
            *static_cast<std::uint32_t *>(binding) &= ~0x10U;
            if (node->attachmentOffset)
                ModelCopyToGPU(binding, reinterpret_cast<std::int32_t *>(g_pCurrentTaskData + node->attachmentOffset), lights, count);
        }
        if (node->childOffset)
            HMDL_PrelightNodeRecursive(reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->childOffset), lights, count);
        if (!node->siblingOffset)
            return;
        PopMatrix();
        node = reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->siblingOffset);
    }
}


void HMDL_PrelightSpotLights(void *model, EffectLightRenderSlot *lights, std::int32_t count) {
    if (!model)
        return;
    PushMatrix();
    g_mt5RootTransform = workTransformMatrix;
    HMDL_PrelightNodeRecursive(static_cast<Mt5RuntimeNode *>(model), lights, count);
    PopMatrix();
}


void SetMt5AlphaMaskedDepthPass(bool enabled) {
    g_replayingEffectQueue = enabled;
    g_alphaMaskedDepthPass = enabled;
}
void BindLegacyPixelTexture(D3TDeviceContext *context, std::uint32_t texture) {
    if (context)
        BindTexture(context, texture);
}

void CommitLegacyScreenRenderState(D3TDeviceContext *context, bool transparent) {
    if (context)
        CommitLegacyRenderState(context, g_renderParams, transparent);
}

void SetMt5RenderShaderSelection(std::int32_t inputLayoutIndex, std::int32_t vertexShaderIndex, std::int32_t pixelShaderIndex) {
    g_tmpInputLayoutIndex = {inputLayoutIndex, vertexShaderIndex, pixelShaderIndex};
}


bool SetRenderPass(std::int32_t renderPass, std::int32_t pipelinePreset) {
    if (static_cast<std::uint32_t>(renderPass) > 2U)
        return false;
    g_renderPassIndex = renderPass;
    const RenderPipelinePreset &preset = g_inputLayoutTypes[static_cast<std::size_t>(pipelinePreset)];
    g_renderVertexStride = preset.vertexStride;
    g_renderVertexShaderAndLayoutIndex = preset.vertexShaderAndLayoutIndex;
    g_renderPixelShaderIndex = preset.pixelShaderIndex;
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    BindVertexOrPixelShader(context, 0, g_originalLambVertexShaders[static_cast<std::size_t>(preset.vertexShaderAndLayoutIndex)]);
    std::byte *const state = context->renderState.data();
    const std::uint32_t inputLayout = g_fvfLayouts[static_cast<std::size_t>(preset.vertexShaderAndLayoutIndex)];
    if (ReadState<std::uint32_t>(state, 0x938) != inputLayout) {
        state[0x975] |= std::byte{0x80};
        WriteState(state, 0x938, inputLayout);
    }
    BindVertexOrPixelShader(context, 1, g_simplePixelShaders[static_cast<std::size_t>(g_tmpInputLayoutIndex.pixelShaderIndex)]);
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    if (GetTexture2DDimensions(g_fullScreenTexture2D, &width, &height))
        MapForSuperSampling(context, static_cast<float>(width));
    return true;
}




void DrawSceneObjectFromCommandStream(void *commandStreamOwner, float alphaScale, std::uint8_t shaderMode) {
    if (!commandStreamOwner || g_perspectiveProjection == 1 || g_perspectiveProjection == 4 || !DX11::g_ppD3DDeviceContext || !ResolveSceneShaders()) {
        return;
    }
    auto *const binding = static_cast<Mt5SceneBinding *>(commandStreamOwner);
    GameNode *const entry = FindSceneEntityByID(SceneKey(binding));
    if (!entry || !binding->stripCommandOffset) {
        if (!entry)
            ++g_missingSceneEntryCount;
        return;
    }
    const Matrix4x4 modelTransform = workTransformMatrix;
    ConvertAndPushCurrentMatrixToShaderBlock();
    PrepareSceneLightingForRendering();
    const std::int32_t *const bufferHandleOverride = g_effectBufferHandleOverride;
    g_effectBufferHandleOverride = nullptr;
    const std::int32_t bufferHandle = (binding->flags & 0x10U) != 0U && bufferHandleOverride ? *bufferHandleOverride : entry->bufferHandle;

    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    std::byte *const state = context->renderState.data();
    BindVertexOrPixelShader(context, 0, g_originalLambVertexShaders[g_tmpInputLayoutIndex.vertexShaderIndex]);
    BindVertexOrPixelShader(context, 1, g_simplePixelShaders[g_tmpInputLayoutIndex.pixelShaderIndex]);
    if (ReadState<std::uint32_t>(state, 0x938) != g_currentInputLayout) {
        state[0x975] |= std::byte{0x80};
        WriteState(state, 0x938, g_currentInputLayout);
    }
    if (ReadState<std::uint32_t>(state, 0xc) != static_cast<std::uint32_t>(bufferHandle)) {
        auto dirty = ReadState<std::uint32_t>(state, 0x970);
        dirty |= 1U;
        WriteState(state, 0x970, dirty);
        WriteState(state, 0xc, static_cast<std::uint32_t>(bufferHandle));
    }
    if (ReadState<std::uint32_t>(state, 0x93c) != 0U) {
        state[0x976] |= std::byte{1};
        WriteState(state, 0x93c, std::uint32_t{0});
    }

    ShaderParams &parameters = g_renderParams;
    parameters.fogColour = g_legacyShaderParameters.fogColour;
    parameters.fogEnabled = g_legacyShaderParameters.fogEnabled;
    parameters.alphaTestEnabled = g_legacyShaderParameters.alphaTestEnabled;
    parameters.alphaReference = g_legacyShaderParameters.alphaReference;
    parameters.alphaFunction = g_legacyShaderParameters.alphaFunction;
    parameters.alphaOperation = g_alphaShaderParameters.operation;
    parameters.alphaArgument0 = g_alphaShaderParameters.argument0;
    parameters.alphaArgument1 = g_alphaShaderParameters.argument1;
    if (g_recordingRyoDraw && !g_capturedRyoTransform) {
        g_characterDrawDiagnostics.ryoFirstViewX = workTransformMatrix.rows[3].x;
        g_characterDrawDiagnostics.ryoFirstViewY = workTransformMatrix.rows[3].y;
        g_characterDrawDiagnostics.ryoFirstViewZ = workTransformMatrix.rows[3].z;
        g_capturedRyoTransform = true;
    }
    parameters.alphaArgument2 = g_alphaShaderParameters.argument2;
    parameters.specularEnabled = g_legacyShaderParameters.specularEnabled;

    parameters.constants[4] = {g_modernAmbientColour.x, g_modernAmbientColour.y, g_modernAmbientColour.z, 1.0f};
    parameters.constants[5] = {1.0f, 1.0f, 1.0f, 1.0f};
    parameters.constants[6] = {};
    parameters.constants[7] = {0.0f, 1.0f, 0.0f, 0.0f};



    workTransformMatrix = modelTransform;

    auto *command = reinterpret_cast<const std::uint16_t *>((g_activeMt5Base ? g_activeMt5Base : g_pCurrentTaskData) + binding->stripCommandOffset);
    MaterialState material{};
    bool transparent = false;
    std::int32_t drawIndex = 0;
    std::uint32_t startVertex = 0;
    while (command && *command != 0x8000U && drawIndex < entry->abc) {
        const std::uint16_t opcode = *command;
        switch (opcode) {
        case 0:
        case 0xffff:
            ++command;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            ParseDrawMaterialCommand(command, alphaScale, shaderMode, material, transparent);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            command += 2;
            break;
        case 12:
        case 13:
        case 21:
        case 29:
            ++command;
            break;
        case 14:
        case 15:
            command += 2;
            PushStripMetaIntoShaderConstants(command, shaderMode);
            break;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28: {
            parameters.useEnvironmentMap = opcode == 19U;
            ParseGeometryCommand(command);
            if (g_replayingEffectQueue ? material.renderClass == 2U : material.renderClass != 2U) {
                DrawObject(context, entry, drawIndex, startVertex, material, transparent, shaderMode, parameters);
            }
            SetRenderFlags_NoIgnoreLighting();
            startVertex += (&entry->vertexCount)[drawIndex * 3];
            ++drawIndex;
            break;
        }
        default:
            return;
        }
    }
}



void SubmitSceneObjectForDrawing(void *commandStreamOwner, void *modelBase) {
    const GameNode *const entry = FindSceneEntityByID(SceneKey(commandStreamOwner));
    const std::uint8_t shaderMode = g_is_busy_transform ? 4 : 0;
    std::byte *const previousBase = g_activeMt5Base;
    g_activeMt5Base = static_cast<std::byte *>(modelBase);
    if (entry && (entry->renderFlags & 1U) != 0U && !g_replayingEffectQueue)
        SubmitToEffectBuffer(commandStreamOwner, 1.0f, shaderMode, modelBase);
    if (!g_effectsNeedInitialization)
        g_effectSubmissionLightDirection = g_effectLightRenderSlots[0].negativeNormalizedDirection;
    DrawSceneObjectFromCommandStream(commandStreamOwner, 1.0f, shaderMode);
    g_activeMt5Base = previousBase;
}

void DrawSceneObjectFromEffectBuffer(void *commandStreamOwner, float alphaScale, std::uint8_t shaderMode, void *modelBase) {
    std::byte *const previousBase = g_activeMt5Base;
    g_activeMt5Base = static_cast<std::byte *>(modelBase);
    DrawSceneObjectFromCommandStream(commandStreamOwner, alphaScale, shaderMode);
    g_activeMt5Base = previousBase;
}

namespace d3t {
void MT5_TransformAndSubmitNodeTree(void *nodePointer, float alpha, std::uint32_t mode) {
    auto *node = static_cast<Mt5Node *>(nodePointer);
    while (node) {
        ++g_visitedNodeCount;
        const bool hasSibling = node->siblingOffset != 0;
        if (hasSibling)
            PushMatrix();
        ApplyNodeTransform(*node);
        const RiggedNodeSpecialState special = ApplyRiggedNodeSpecialState(*node, alpha);
        std::byte *bindingBase = nullptr;
        Mt5SceneBinding *const binding = special.suppressBinding ? nullptr : BindingFor(node, &bindingBase);
        if (binding && binding->stripCommandOffset) {
            ++g_bindingCount;
            std::byte *const nodeBase = g_activeMt5Base;
            const bool ordinaryMutable = !g_alphaMaskedDepthPass && !g_submittingSyncedHand && (binding->flags & 0x400U) != 0U && node != g_syncedFaceRoot &&
                node != g_syncedFaceSecondary;
            if (!g_characterShadowPass && (ordinaryMutable || special.faceChannelRebased)) {
                GameNode *const entry = FindSceneEntityByID(SceneKey(binding));
                if (entry && entry->bufferHandle) {
                    FaceClipControllerState *const clip = CLIP_GetWork(g_activeRiggedCharacter);
                    const void *const previousVertices =
                        MT5_SetVertexDataOverride(special.faceChannelRebased && clip ? FACE_GetChannelVertices(clip->channelIndex) : nullptr);
                    void *mutableNode = node;
                    MT5_ReadMeshStrip(&mutableNode, g_mt5ReaderTextureIds, entry->bufferHandle);
                    MT5_SetVertexDataOverride(previousVertices);
                }
            }
            g_activeMt5Base = bindingBase;
            if (alpha == 1.0f) {
                SubmitSceneObjectForDrawing(binding, bindingBase);
            } else if (alpha != 0.0f) {
                SubmitToEffectBuffer(binding, alpha, mode, g_activeMt5Base ? g_activeMt5Base : g_pCurrentTaskData);
            }
            g_activeMt5Base = nodeBase;
        }
        if (special.deferredFaceRoot) {
            SubmitReplacementNodeTree(static_cast<Mt5Node *>(special.deferredFaceRoot), alpha);
        }
        if (special.traverseChildren) {
            if (Mt5Node *const child = OffsetNode(node->childOffset)) {
                d3t::MT5_TransformAndSubmitNodeTree(child, alpha, mode);
            }
        }
        if (!hasSibling)
            break;
        PopMatrix();
        node = OffsetNode(node->siblingOffset);
    }
}



void MT5_PushAndReadNodeTree(void *node, const std::int32_t *textureIds, float alpha) {
    if (!node)
        return;
    std::byte *const previousBase = g_activeMt5Base;
    const std::int32_t *const previousTextureIds = g_mt5ReaderTextureIds;
    g_activeMt5Base = MT5_ModelBaseForNode(node);
    g_mt5ReaderTextureIds = textureIds;
    PushMatrix();
    g_mt5RootTransform = workTransformMatrix;
    d3t::MT5_TransformAndSubmitNodeTree(node, alpha, 0);
    PopMatrix();
    g_mt5ReaderTextureIds = previousTextureIds;
    g_activeMt5Base = previousBase;
}


void MT5_PushAndSubmitNodeTree(void *node, float alpha) {
    if (!node)
        return;
    std::byte *const previousBase = g_activeMt5Base;
    g_activeMt5Base = MT5_ModelBaseForNode(node);
    PushMatrix();
    g_mt5RootTransform = workTransformMatrix;
    d3t::MT5_TransformAndSubmitNodeTree(node, alpha, 2);
    PopMatrix();
    g_activeMt5Base = previousBase;
}

void MT5_SubmitObjectNode(void *nodePointer, float alpha) {
    if (!nodePointer)
        return;
    auto *const node = static_cast<Mt5Node *>(nodePointer);
    const std::int32_t siblingOffset = node->siblingOffset;
    std::byte *const previousBase = g_activeMt5Base;
    const Matrix4x4 previousRootTransform = g_mt5RootTransform;
    node->siblingOffset = 0;
    g_activeMt5Base = MT5_ModelBaseForNode(node);
    PushMatrix();
    g_mt5RootTransform = workTransformMatrix;
    d3t::MT5_TransformAndSubmitNodeTree(node, alpha, 0);
    PopMatrix();
    g_mt5RootTransform = previousRootTransform;
    g_activeMt5Base = previousBase;
    node->siblingOffset = siblingOffset;
}
} // namespace d3t

void Recursive_SubmitRiggedNodes(void *node, float alpha, const Matrix4x4 &absoluteTransformRoot, Character *character) {
    if (!node)
        return;
    auto *const root = static_cast<Mt5Node *>(node);
    std::byte *const previousBase = g_activeMt5Base;
    Character *const previousCharacter = g_activeRiggedCharacter;
    Mt5Node *const previousSyncedFaceRoot = g_syncedFaceRoot;
    Mt5Node *const previousSyncedFaceSecondary = g_syncedFaceSecondary;
    const Matrix4x4 previousRootTransform = g_mt5RootTransform;
    g_activeMt5Base = MT5_ModelBaseForNode(node);
    g_activeRiggedCharacter = character;
    g_syncedFaceRoot = nullptr;
    g_syncedFaceSecondary = nullptr;
    // The body pass owns pose preparation and mutable-buffer publication.
    // Shadows deliberately reuse that posed geometry instead of advancing
    // OSAGE, face/hand deformation, or cached skinning a second time.
    PrepareActiveRiggedNodeTree(root, absoluteTransformRoot);
    DrawActiveRiggedNodeTree(root, alpha, absoluteTransformRoot, true, g_characterShadowPass ? 2U : 0U);
    g_mt5RootTransform = previousRootTransform;
    g_syncedFaceSecondary = previousSyncedFaceSecondary;
    g_syncedFaceRoot = previousSyncedFaceRoot;
    g_activeRiggedCharacter = previousCharacter;
    g_activeMt5Base = previousBase;
}
void PrepareRiggedSceneObjectForDrawing(void *model, const Matrix4x4 &absoluteTransformRoot, Character *character) {
    if (!model)
        return;
    auto *const root = static_cast<Mt5Node *>(model);
    std::byte *const previousBase = g_activeMt5Base;
    Character *const previousCharacter = g_activeRiggedCharacter;
    Mt5Node *const previousSyncedFaceRoot = g_syncedFaceRoot;
    Mt5Node *const previousSyncedFaceSecondary = g_syncedFaceSecondary;
    const Matrix4x4 previousRootTransform = g_mt5RootTransform;
    g_activeMt5Base = MT5_ModelBaseForNode(model);
    g_activeRiggedCharacter = character;
    g_syncedFaceRoot = nullptr;
    g_syncedFaceSecondary = nullptr;
    PrepareActiveRiggedNodeTree(root, absoluteTransformRoot);
    g_mt5RootTransform = previousRootTransform;
    g_syncedFaceSecondary = previousSyncedFaceSecondary;
    g_syncedFaceRoot = previousSyncedFaceRoot;
    g_activeRiggedCharacter = previousCharacter;
    g_activeMt5Base = previousBase;
}
void SubmitRiggedCharacterModel(void *model, Character *character, float alpha) {
    if (!model || !character)
        return;
    auto *const root = static_cast<Mt5Node *>(model);
    std::byte *const previousBase = g_activeMt5Base;
    Character *const previousCharacter = g_activeRiggedCharacter;
    Mt5Node *const previousSyncedFaceRoot = g_syncedFaceRoot;
    Mt5Node *const previousSyncedFaceSecondary = g_syncedFaceSecondary;
    const Matrix4x4 previousRootTransform = g_mt5RootTransform;
    const Matrix4x4 incomingTransform = workTransformMatrix;
    g_activeMt5Base = MT5_ModelBaseForNode(model);
    g_activeRiggedCharacter = character;
    g_syncedFaceRoot = nullptr;
    g_syncedFaceSecondary = nullptr;

    Matrix4x4::SetActiveTransformToPrePass();
    const Vector3 rootPosition = GetPositionForEntity(character->entity.node);
    const Vector3i rootRotation = GetEulerAnglesForEntity(character->entity.node);
    ApplySceneAttachmentTransform(rootPosition, rootRotation, {}, {});
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f) {
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
    }
    const Matrix4x4 cachedTransform = workTransformMatrix;
    auto *const primaryEntry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model)));
    if (primaryEntry && primaryEntry->state == 1) {
        PrepareActiveRiggedNodeTree(root, cachedTransform);
    }
    static_cast<void>(SubmitRiggedSceneObjectForDrawing(model, character, 0, 0));
    const bool cachedSubmission = CachedAttachmentOwnsSubmission(model, alpha, cachedTransform);
    PopMatrix();

    if (!cachedSubmission) {
        const bool previousNodeWalk = g_characterHandlerNodeWalk;
        g_characterHandlerNodeWalk = true;
        DrawActiveRiggedNodeTree(root, alpha, incomingTransform, true, 0);
        g_characterHandlerNodeWalk = previousNodeWalk;
    }

    g_mt5RootTransform = previousRootTransform;
    g_syncedFaceSecondary = previousSyncedFaceSecondary;
    g_syncedFaceRoot = previousSyncedFaceRoot;
    g_activeRiggedCharacter = previousCharacter;
    g_activeMt5Base = previousBase;
}

void SubmitBoneEffectCharacterModel(void *model, Character *character) {
    if (!model || !character || !g_updateRenderFrameMode)
        return;

    auto *const root = static_cast<Mt5Node *>(model);
    std::byte *const previousBase = g_activeMt5Base;
    g_activeMt5Base = MT5_ModelBaseForNode(model);

    const auto applyCharacterTransform = [character] {
        const Vector3 position = GetPositionForEntity(character->entity.node);
        const Vector3i rotation = GetEulerAnglesForEntity(character->entity.node);
        ApplySceneAttachmentTransform(position, rotation, {}, {});
        if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f) {
            Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
        }
    };
    const auto applyNodeTransform = [](const Mt5Node &node) {
        Matrix4x4::preTranslate(node.posX, node.posY, node.posZ);
        if (node.rotZ)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotZ));
        if (node.rotY)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotY));
        if (node.rotX)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotX));
    };
    const auto submitNode = [](Mt5Node *node) {
        std::byte *bindingBase = nullptr;
        Mt5SceneBinding *const binding = BindingFor(node, &bindingBase);
        if (!binding)
            return;
        std::byte *const nodeBase = g_activeMt5Base;
        g_activeMt5Base = bindingBase;
        SubmitSceneObjectForDrawing(binding, bindingBase);
        g_activeMt5Base = nodeBase;
    };

    Matrix4x4::SetActiveTransformToPrePass();
    applyCharacterTransform();
    const bool cachedSubmission = CachedAttachmentOwnsSubmission(model, 1.0f, workTransformMatrix);
    PopMatrix();

    if (!cachedSubmission) {
        PushMatrix();
        applyCharacterTransform();

        const std::int32_t savedLightingMode = g_cachedGlobalLightingAdjustFlag;
        if (savedLightingMode != 2) {
            g_effectsNeedInitialization = 1;
            g_cachedGlobalLightingAdjustFlag = 2;
        }

        std::uint32_t temporaryLight = 0;
        while (temporaryLight < 32) {
            const ShenmueLightSlot *const slot = LGHT_GetDataSlot(temporaryLight);
            if (!slot || !slot->enabled)
                break;
            ++temporaryLight;
        }

        PushMatrix();
        applyNodeTransform(*root);
        if (root->sceneBindingOffset)
            submitNode(root);
        if (Mt5Node *const child = OffsetNode(root->childOffset)) {
            const bool ownsTemporaryLight = temporaryLight < 32;
            if (ownsTemporaryLight) {
                lightResetSlotToDefaults(temporaryLight);
                if (g_effectScriptWordParameters[1] < 5U) {
                    SetLightBaseType(temporaryLight, g_effectScriptWordParameters[1]);
                }
                SetRGBStruct(temporaryLight, &g_effectScriptMaximumIntensity);
                SetLightIntensity(temporaryLight, g_effectScriptScalarParameters[1]);
                SetLightDirectionVectorByIndex(temporaryLight, &g_effectScriptVectorParameters[1]);
                SetEffectTransformByIndex(temporaryLight, &g_effectScriptVectorParameters[0]);
                SetLightFalloffRangeByIndex(temporaryLight, g_effectScriptScalarParameters[0]);
                SetLightFalloff(temporaryLight, g_effectScriptScalarParameters[2]);
                if (g_effectScriptWordParameters[0] <= 1U) {
                    lightSetSlotEnabled(temporaryLight, g_effectScriptWordParameters[0]);
                }
                const Matrix4x4 nodeTransform = workTransformMatrix;
                workTransformMatrix = g_tmp_cam_xform;
                Shenmue_InitEffectByIndexExact();
                workTransformMatrix = nodeTransform;
            }

            applyNodeTransform(*child);
            if (child->sceneBindingOffset)
                submitNode(child);

            if (ownsTemporaryLight) {
                lightResetSlotToDefaults(temporaryLight);
                const Matrix4x4 nodeTransform = workTransformMatrix;
                workTransformMatrix = g_tmp_cam_xform;
                Shenmue_InitEffectByIndexExact();
                workTransformMatrix = nodeTransform;
            }
        }
        PopMatrix();

        if (savedLightingMode != 2) {
            g_cachedGlobalLightingAdjustFlag = savedLightingMode;
            g_effectsNeedInitialization = savedLightingMode != 1;
        }
        PopMatrix();
    }

    g_activeMt5Base = previousBase;
}

void SubmitRiggedSceneAttachment(
    void *model, float alpha, const Vector3 &rootPosition, const Vector3i &rootRotation, const Vector3 &localPosition, const Vector3i &localRotation) {
    if (!model)
        return;
    auto *const root = static_cast<Mt5Node *>(model);
    std::byte *const previousBase = g_activeMt5Base;
    Character *const previousCharacter = g_activeRiggedCharacter;
    Mt5Node *const previousSyncedFaceRoot = g_syncedFaceRoot;
    Mt5Node *const previousSyncedFaceSecondary = g_syncedFaceSecondary;
    const Matrix4x4 previousRootTransform = g_mt5RootTransform;
    g_activeMt5Base = MT5_ModelBaseForNode(model);
    g_activeRiggedCharacter = nullptr;
    g_syncedFaceRoot = nullptr;
    g_syncedFaceSecondary = nullptr;




    Matrix4x4::SetActiveTransformToPrePass();
    ApplySceneAttachmentTransform(rootPosition, rootRotation, localPosition, localRotation);
    const Matrix4x4 cachedTransform = workTransformMatrix;
    auto *const primaryEntry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model)));
    if (primaryEntry && primaryEntry->state == 1) {
        PrepareActiveRiggedNodeTree(root, cachedTransform);
    }
    static_cast<void>(SubmitRiggedSceneObjectForDrawing(model, nullptr, 0, 0));
    const bool cachedSubmission = CachedAttachmentOwnsSubmission(model, alpha, cachedTransform);
    PopMatrix();

    if (!cachedSubmission) {
        PushMatrix();
        ApplySceneAttachmentTransform(rootPosition, rootRotation, localPosition, localRotation);
        const Matrix4x4 directTransform = workTransformMatrix;
        DrawActiveRiggedNodeTree(root, alpha, directTransform, false, 0);
        PopMatrix();
    }

    g_mt5RootTransform = previousRootTransform;
    g_syncedFaceSecondary = previousSyncedFaceSecondary;
    g_syncedFaceRoot = previousSyncedFaceRoot;
    g_activeRiggedCharacter = previousCharacter;
    g_activeMt5Base = previousBase;
}

void DrawCharacterObjects() {
    if (!DX11::g_ppD3DDeviceContext || !g_mapControlTask || !g_mapControlTask->task_data_ptr) {
        return;
    }

    constexpr std::size_t kMaxHumanDraws = 5;
    struct HumanDraw {
        Character *character;
        float distance;
    };

    g_characterDrawDiagnostics = {};
    g_capturedRyoTransform = false;
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    SetActiveStack(MatrixStackType::DepthPrepass);
    const Matrix4x4 cameraView = workTransformMatrix;

    const auto submit = [&](Character *character) {
        if (!character || !character->model) {
            return;
        }
        ++g_characterDrawDiagnostics.submittedCharacters;
        const std::uint32_t visitedBefore = g_visitedNodeCount;
        const std::uint32_t bindingsBefore = g_bindingCount;
        const std::uint32_t drawsBefore = g_drawCount;
        const std::uint32_t preparedBefore = g_preparedNodeCount;
        const std::uint32_t missingBefore = g_missingSceneEntryCount;
        workTransformMatrix = cameraView;
        g_recordingRyoDraw = character == control->ryo;
        Character_SubmitForDrawing(character);
        SetDefaultRenderFlags();
        if (character == control->ryo) {
            g_characterDrawDiagnostics.ryoVisitedNodes = g_visitedNodeCount - visitedBefore;
            g_characterDrawDiagnostics.ryoBindings = g_bindingCount - bindingsBefore;
            g_characterDrawDiagnostics.ryoDraws = g_drawCount - drawsBefore;
            g_characterDrawDiagnostics.ryoMissingSceneEntries = g_missingSceneEntryCount - missingBefore;
            g_characterDrawDiagnostics.ryoPreparedNodes = g_preparedNodeCount - preparedBefore;
        }
        g_recordingRyoDraw = false;
    };

    // DRWP owns Ryo before the two CEWP table passes.
    submit(static_cast<Character *>(control->ryo));



    std::array<HumanDraw, kMaxHumanDraws> humans{};
    std::size_t humanCount = 0;
    for (void *head : control->characterBuckets) {
        for (auto *link = static_cast<CharacterBucketNode *>(head); link; link = link->next) {
            Character *const character = link->character;
            if (!character || character == control->ryo || !MOTM_GetWork(character) || (character->motionFlags & 0x3fU) != 0x3fU ||
                character->renderOpacity == 0.0f) {
                continue;
            }
            const float distance = character->viewPosition.z < 0.0f ? character->viewDistance : 1000.0f;
            std::size_t insertion = 0;
            while (insertion < humanCount && humans[insertion].distance <= distance) {
                ++insertion;
            }
            if (insertion >= kMaxHumanDraws)
                continue;
            const std::size_t newCount = std::min(humanCount + 1, kMaxHumanDraws);
            for (std::size_t index = newCount - 1; index > insertion; --index) {
                humans[index] = humans[index - 1];
            }
            humans[insertion] = {character, distance};
            humanCount = newCount;
        }
    }
    for (std::size_t index = 0; index < humanCount; ++index)
        submit(humans[index].character);

    // DRWO submits the remaining CEWP entities in bucket/list order.
    for (void *head : control->characterBuckets) {
        for (auto *link = static_cast<CharacterBucketNode *>(head); link; link = link->next) {
            Character *const character = link->character;
            if (character && !MOTM_GetWork(character))
                submit(character);
        }
    }

    workTransformMatrix = cameraView;
}

void DrawCharacterShadows() {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !g_mapControlTask || !g_mapControlTask->task_data_ptr) {
        return;
    }

    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    const Matrix4x4 cameraView = workTransformMatrix;
    SetupCharacterShadowTarget(context);
    for (void *head : control->characterBuckets) {
        for (auto *link = static_cast<CharacterBucketNode *>(head); link; link = link->next) {
            Character *const character = link->character;
            if (!character || !character->processShadowCallback ||
                (((character->flags.value & 1U) == 0U) && (((character->flags.value & 2U) == 0U) || ((character->motionFlags & 1U) == 0U))) ||
                (character->flags.value & 0x10U) == 0U) {
                continue;
            }
            workTransformMatrix = cameraView;
            character->processShadowCallback(character);
        }
    }
    workTransformMatrix = cameraView;
    SetupPPFX(context);
}

CharacterDrawDiagnostics GetCharacterDrawDiagnostics() {
    return g_characterDrawDiagnostics;
}

} // namespace shenmue::s1
