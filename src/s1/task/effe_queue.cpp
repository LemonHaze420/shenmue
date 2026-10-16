#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

std::size_t SaturatedMultiply(std::size_t first, std::size_t second) {
    if (first && second > std::numeric_limits<std::size_t>::max() / first) {
        return std::numeric_limits<std::size_t>::max();
    }
    return first * second;
}

void *AllocateEffectBytes(std::size_t size) {
    if (!size) {
        return nullptr;
    }
    return ::operator new(size, std::nothrow);
}

void ReleaseOwnedEffectBuffers() {
    ::operator delete(g_effectTransformContexts);
    ::operator delete(g_effectQueueOrder);
    ::operator delete(g_effectQueueOrderScratch);
    ::operator delete(g_effectAuxiliaryEntries);
    ::operator delete(g_effectAuxiliaryOrder);
    ::operator delete(g_effectQueueEntries);
    ::operator delete(g_effectQueueSortKeys);
    g_effectTransformContexts = nullptr;
    g_effectQueueOrder = nullptr;
    g_effectQueueOrderScratch = nullptr;
    g_effectAuxiliaryEntries = nullptr;
    g_effectAuxiliaryOrder = nullptr;
    g_effectQueueEntries = nullptr;
    g_effectQueueSortKeys = nullptr;
}

std::int64_t InitializeEffectBufferStorage(const EffectBufferConfiguration &configuration) {
    if (configuration.transformContextCount <= 0) {
        return 0;
    }

    g_effectTransformContexts = nullptr;
    g_effectQueueOrder = nullptr;
    g_effectQueueOrderScratch = nullptr;
    g_effectAuxiliaryEntries = nullptr;
    g_effectAuxiliaryOrder = nullptr;
    g_effectQueueEntries = nullptr;
    g_effectQueueSortKeys = nullptr;
    g_effectTransformContextCount = 0;
    g_effectAuxiliaryCount = 0;
    g_effectQueueEntryCount = 0;
    g_effectObjectPassCount = 0;
    g_effectCallbackCount = 0;
    g_effectRunCount = 0;

    const std::int32_t orderCapacity = std::max(configuration.auxiliaryCount, configuration.queueEntryCount);
    if (configuration.storage) {
        auto *cursor = static_cast<std::byte *>(configuration.storage);
        g_effectTransformContexts = cursor;
        cursor += SaturatedMultiply(static_cast<std::size_t>(configuration.transformContextCount), kEffectTransformContextBytes);
        g_effectQueueOrder = reinterpret_cast<std::uint32_t *>(cursor);
        cursor += SaturatedMultiply(static_cast<std::size_t>(orderCapacity), sizeof(std::uint32_t));
        g_effectQueueOrderScratch = reinterpret_cast<std::uint32_t *>(cursor);
        cursor += SaturatedMultiply(static_cast<std::size_t>(orderCapacity), sizeof(std::uint32_t));
        if (configuration.auxiliaryCount > 0) {
            g_effectAuxiliaryEntries = cursor;
            cursor += SaturatedMultiply(static_cast<std::size_t>(configuration.auxiliaryCount), 16);
            g_effectAuxiliaryOrder = reinterpret_cast<std::uint32_t *>(cursor);
            cursor += SaturatedMultiply(static_cast<std::size_t>(configuration.auxiliaryCount), sizeof(std::uint32_t));
        }
        if (configuration.queueEntryCount > 0) {
            g_effectQueueEntries = reinterpret_cast<EffectQueueEntry *>(cursor);
            cursor += SaturatedMultiply(static_cast<std::size_t>(configuration.queueEntryCount), sizeof(EffectQueueEntry));
            g_effectQueueSortKeys = reinterpret_cast<float *>(cursor);
        }
    } else {
        g_effectTransformContexts = static_cast<std::byte *>(
            AllocateEffectBytes(SaturatedMultiply(static_cast<std::size_t>(configuration.transformContextCount), kEffectTransformContextBytes)));
        g_effectQueueOrder =
            static_cast<std::uint32_t *>(AllocateEffectBytes(SaturatedMultiply(static_cast<std::size_t>(orderCapacity), sizeof(std::uint32_t))));
        g_effectQueueOrderScratch =
            static_cast<std::uint32_t *>(AllocateEffectBytes(SaturatedMultiply(static_cast<std::size_t>(orderCapacity), sizeof(std::uint32_t))));
        if (configuration.auxiliaryCount > 0) {
            g_effectAuxiliaryEntries = AllocateEffectBytes(SaturatedMultiply(static_cast<std::size_t>(configuration.auxiliaryCount), 16));
            g_effectAuxiliaryOrder = static_cast<std::uint32_t *>(
                AllocateEffectBytes(SaturatedMultiply(static_cast<std::size_t>(configuration.auxiliaryCount), sizeof(std::uint32_t))));
        }
        if (configuration.queueEntryCount > 0) {
            g_effectQueueEntries = static_cast<EffectQueueEntry *>(
                AllocateEffectBytes(SaturatedMultiply(static_cast<std::size_t>(configuration.queueEntryCount), sizeof(EffectQueueEntry))));
            g_effectQueueSortKeys =
                static_cast<float *>(AllocateEffectBytes(SaturatedMultiply(static_cast<std::size_t>(configuration.queueEntryCount), sizeof(float))));
        }

        const bool auxiliaryReady = configuration.auxiliaryCount <= 0 || (g_effectAuxiliaryEntries && g_effectAuxiliaryOrder);
        const bool queueReady = configuration.queueEntryCount <= 0 || (g_effectQueueEntries && g_effectQueueSortKeys);
        if (!g_effectTransformContexts || !g_effectQueueOrder || !g_effectQueueOrderScratch || !auxiliaryReady || !queueReady) {
            ReleaseOwnedEffectBuffers();
            return 0;
        }
    }

    g_effectTransformContextCapacity = configuration.transformContextCount;
    g_effectAuxiliaryCapacity = configuration.auxiliaryCount;
    g_effectQueueEntryCapacity = configuration.queueEntryCount;
    return 1;
}

EffectQueueEntry *allocateEffectQueueEntry(EffectQueueEntryKind kind, float sortKey) {
    if (!g_effectQueueEntries || !g_effectQueueOrder || !g_effectQueueSortKeys || g_effectQueueEntryCount >= g_effectQueueEntryCapacity ||
        !g_effectTransformContexts || g_effectTransformContextCount >= g_effectTransformContextCapacity) {
        return nullptr;
    }

    auto *const context =
        reinterpret_cast<Matrix4x4 *>(g_effectTransformContexts + static_cast<std::size_t>(g_effectTransformContextCount) * kEffectTransformContextBytes);
    *context = workTransformMatrix;




    std::uint32_t transformFlags = 0;
    std::memcpy(&transformFlags, reinterpret_cast<const std::byte *>(context) + kEffectTransformFlagsOffset, sizeof(transformFlags));
    if (g_hasSetupLightFlags) {
        transformFlags |= 4U;
        std::memcpy(
            reinterpret_cast<std::byte *>(context) + kEffectTransformLightSlotsOffset, g_effectLightRenderSlots.data(), sizeof(g_effectLightRenderSlots));
    } else {
        transformFlags &= ~4U;
        constexpr std::array<std::uint32_t, 3> reservedSubmission{};
        std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformSubmissionLightOffset, &g_effectSubmissionLightDirection,
            sizeof(g_effectSubmissionLightDirection));
        std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformSubmissionReservedOffset, reservedSubmission.data(), sizeof(reservedSubmission));
    }
    transformFlags = (transformFlags & ~(1U | 2U | 8U | 0x20U)) | 0x10U;
    constexpr float unitScale = 1.0f;
    constexpr Vector3 unitVector{1.0f, 1.0f, 1.0f};
    constexpr std::uint32_t reservedWord = 0;
    constexpr Vector4 reservedVector{};
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformUnitScaleOffset, &unitScale, sizeof(unitScale));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformPrimaryVectorOffset, &unitVector, sizeof(unitVector));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformSecondaryVectorOffset, &unitVector, sizeof(unitVector));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformReservedWordOffset, &reservedWord, sizeof(reservedWord));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformSceneLightingYZOffset, &g_sceneLightingActiveColour.y, 2U * sizeof(float));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformSceneLightingWOffset, &g_sceneLightingActiveColour.w, sizeof(float));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformFogNearOffset, &g_fogRenderNearDistance, sizeof(g_fogRenderNearDistance));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformFogFarOffset, &g_fogRenderFarDistance, sizeof(g_fogRenderFarDistance));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformFogMaximumOffset, &g_fogMaximumDistance, sizeof(g_fogMaximumDistance));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformAuxiliaryMatrixOffset, &g_effectTransform, sizeof(g_effectTransform));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformFlagsOffset, &transformFlags, sizeof(transformFlags));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformReservedVectorOffset, &reservedVector, sizeof(reservedVector));
    std::memcpy(reinterpret_cast<std::byte *>(context) + kEffectTransformMaxScaleOffset, &g_sceneObjectMaxScale, sizeof(g_sceneObjectMaxScale));
    ++g_effectTransformContextCount;

    const std::int32_t index = g_effectQueueEntryCount++;
    EffectQueueEntry &entry = g_effectQueueEntries[index];
    entry = {};
    entry.instance = context;
    entry.kind = kind;
    g_effectQueueOrder[index] = static_cast<std::uint32_t>(index);
    g_effectQueueSortKeys[index] = sortKey;
    return &entry;
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_RemoveQueuedSceneObjectReferences(void *sceneObject) {
    for (std::int32_t index = 0; index < g_effectQueueEntryCount; ++index) {
        EffectQueueEntry &entry = g_effectQueueEntries[index];
        if (entry.sceneObjectOrCommandStream == sceneObject) {
            entry.sceneObjectOrCommandStream = nullptr;
            entry.kind = EffectQueueEntryKind::Empty;
        }
    }
}

std::int64_t InitEffectBuffers() {
    g_effectReserved = 0;
    g_effectBuffersEnabled = 1;
    g_effectPackedColour = 0xff7f7f7fU;
    g_effectTransform = Matrix4x4{{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    const EffectBufferConfiguration configuration{nullptr, 1000, 0, 1000};
    const std::int64_t result = InitializeEffectBufferStorage(configuration);
    g_effectSortEnabled = 1;
    g_effectDefaultSortKey = 0.0f;
    return result;
}

void SetupEffeTransform() {
    std::memcpy(g_effectLightRenderSlots.data(), kEffectLightTemplate.data(), sizeof(kEffectLightTemplate));
    std::byte *source = reinterpret_cast<std::byte *>(g_effectLightRenderSlots.data());
    std::byte *destination = source + 0xb8;
    for (std::int32_t index = 0; index < 178; ++index) {
        std::memcpy(destination, source, 0x20);
        source += 0x20;
        destination += 0x20;
    }
    std::memcpy(destination, source, 8);
    g_effectEnabledStandardLightMask = 0;
    g_effectEnabledSpotLightMask = 0;
}

void EFFE_SetLightSlotEnabled(std::uint32_t lightIndex, std::uint32_t enabled) {
    if (lightIndex >= kEffectLightSlots)
        return;
    std::byte *const slot = reinterpret_cast<std::byte *>(g_effectLightRenderSlots.data()) + lightIndex * 0xb8U;
    const std::int16_t enabledValue = static_cast<std::int16_t>(enabled);
    std::memcpy(slot, &enabledValue, sizeof(enabledValue));

    std::uint16_t modeFlags = 0;
    std::memcpy(&modeFlags, slot + 2, sizeof(modeFlags));
    const std::uint32_t bit = 1U << lightIndex;
    g_effectEnabledStandardLightMask &= ~bit;
    g_effectEnabledSpotLightMask &= ~bit;
    if (enabledValue == 1) {
        if ((modeFlags & 8U) != 0U) {
            g_effectEnabledSpotLightMask |= bit;
        } else {
            g_effectEnabledStandardLightMask |= bit;
        }
    }
}

void EFFE_SetLightSlotModeFlags(std::uint32_t lightIndex, std::uint16_t modeFlags) {
    if (lightIndex >= kEffectLightSlots)
        return;
    std::byte *const slot = reinterpret_cast<std::byte *>(g_effectLightRenderSlots.data()) + lightIndex * 0xb8U;
    std::memcpy(slot + 2, &modeFlags, sizeof(modeFlags));
    std::int16_t enabled = 0;
    std::memcpy(&enabled, slot, sizeof(enabled));
    EFFE_SetLightSlotEnabled(lightIndex, static_cast<std::uint32_t>(enabled));
}

std::uint32_t EFFE_GetEnabledStandardLightMask() {
    return static_cast<std::uint32_t>(g_effectEnabledStandardLightMask);
}

std::uint32_t EFFE_GetEnabledSpotRenderLightMask() {
    return g_effectEnabledSpotLightMask;
}

void EFFE_AssignObjectPass(EffectQueueCallback callback, void *argument, float sortKey) {
    if (!callback)
        return;
    EffectQueueEntry *const entry = allocateEffectQueueEntry(EffectQueueEntryKind::ObjectPass, sortKey);
    if (!entry)
        return;
    entry->dispatchFunction = std::bit_cast<void *>(callback);
    entry->dispatchArgument = argument;
}

void EFFE_AssignCallback(EffectQueueCallback callback, void *argument, float sortKey) {
    if (!callback)
        return;
    EffectQueueEntry *const entry = allocateEffectQueueEntry(EffectQueueEntryKind::Callback, sortKey);
    if (!entry)
        return;
    entry->dispatchFunction = std::bit_cast<void *>(callback);
    entry->dispatchArgument = argument;
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {


void CommitOrCombineGeometryBuffers(EffeGeometryChunk *chunk, std::int32_t renderPass) {
#if defined(_WIN32)
    CreateAlphaTextureForParticleEffects(chunk->textureWords);
    SetRenderFlags(0x40, 0);
    std::int32_t stripCount = chunk->stripCount;
    g_currentRenderFlags[0x40] = 0;
    const std::byte *cursor = reinterpret_cast<const std::byte *>(chunk + 1);
    SetRenderPass(renderPass, 1);
    while (stripCount) {
        std::int32_t count = 0;
        std::memcpy(&count, cursor, sizeof(count));
        --stripCount;
        cursor += sizeof(count);
        std::uint32_t vertexCount = count < 0 ? 0U - static_cast<std::uint32_t>(count) : static_cast<std::uint32_t>(count);
        g_numVertices = static_cast<std::int32_t>(vertexCount);
        auto *output = static_cast<std::byte *>(GetMappedConstantBuffer(DX11::g_ppD3DDeviceContext, count >> 31, 28U * vertexCount));
        if (!output)
            continue;
        const float scale = g_inverseGeometryScale;
        for (; vertexCount; --vertexCount) {
            std::uint32_t first = 0;
            std::memcpy(&first, cursor, sizeof(first));
            const std::byte *const vertex = (first & 1U) != 0U ? cursor : cursor - (first & ~3U);
            std::uint16_t v = 0;
            std::uint16_t u = 0;
            std::memcpy(&v, vertex + 0x18, sizeof(v));
            std::memcpy(&u, vertex + 0x1a, sizeof(u));
            const float scaledU = static_cast<float>(u) * scale;
            const float scaledV = static_cast<float>(v) * scale;
            std::memcpy(output, vertex, 12);
            std::memcpy(output + 0x14, &scaledU, sizeof(scaledU));
            std::memcpy(output + 0x18, &scaledV, sizeof(scaledV));
            std::memcpy(output + 0x0c, vertex + 0x1c, 8);
            output += 28;
            cursor += (first & 1U) != 0U ? 36 : 4;
        }
        const std::int32_t diffuseInput = g_renderVertexShaderAndLayoutIndex == 24 ? 0 : 1;
        SetRenderFlags(0x66, diffuseInput);
        g_currentRenderFlags[0x66] = diffuseInput;
        g_mainDrawingFunctions[static_cast<std::size_t>(g_renderPassIndex)]();
    }
    SetRenderAndAlphaFlags_0();
#else
    static_cast<void>(chunk);
    static_cast<void>(renderPass);
#endif
}

void CombineGeometryBuffersIgnoredAlpha_RenderPass0(EffeGeometryChunk *chunk) {
    g_renderParams.ignoreAlpha = 1;
    CommitOrCombineGeometryBuffers(chunk, 0);
    g_renderParams.ignoreAlpha = 0;
}

void CommitOrCombineGeometryBuffers_RenderPass1(EffeGeometryChunk *chunk) {
    CommitOrCombineGeometryBuffers(chunk, 1);
}

void CommitOrCombineGeometryBuffers_RenderPass2(EffeGeometryChunk *chunk) {
    CommitOrCombineGeometryBuffers(chunk, 2);
}


void GeometryChunkNoOp(EffeGeometryChunk *) {
}

void SetInverseGeometryScale(EffeGeometryChunk *chunk) {
    std::int32_t period = 0;
    std::memcpy(&period, reinterpret_cast<const std::byte *>(chunk) + 4, sizeof(period));
    g_inverseGeometryScale = 1.0f / static_cast<float>(period);
}

void ConstructFullXFormsForEntity(void *argument) {
    Matrix4x4_Push();
    ConvertAndPushCurrentMatrixToShaderBlock();
    auto *chunk = static_cast<std::byte *>(argument);
    std::int32_t header = 0;
    std::memcpy(&header, chunk, sizeof(header));
    g_inverseGeometryScale = kDefaultInverseGeometryScale;
    while ((header & 0xff) != 0xff) {
        kGeometryChunkFunctions[static_cast<std::size_t>(header & 0xff)](reinterpret_cast<EffeGeometryChunk *>(chunk));
        chunk += header >> 8;
        std::memcpy(&header, chunk, sizeof(header));
    }
    Matrix4x4::Pop(1);
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void DrawForWeather(EffeGeometryMesh *mesh) {
    Vector3 centre{};
    Matrix4x4::TransformPoint(mesh->centre, centre);
    g_sceneObjectMaxScale = 1.0f;
    if (g_tempNearClip > mesh->radius - centre.z || -centre.z - mesh->radius > g_tempFarClip)
        return;
    EFFE_AssignCallback(ConstructFullXFormsForEntity, mesh->chunkOffset ? g_pCurrentTaskData + mesh->chunkOffset : nullptr, centre.z);
}

std::int64_t EFFE_Run(void *, void *sceneData, float sortKey) {
    const std::int64_t failureResult = static_cast<std::int64_t>(g_effectQueueEntryCount) + 1;
    EffectQueueEntry *const entry = allocateEffectQueueEntry(EffectQueueEntryKind::Run, sortKey);
    if (!entry)
        return failureResult;
    entry->dispatchFunction = std::bit_cast<void *>(&EFFE_Callback2);
    entry->dispatchArgument = sceneData;
    return static_cast<std::int64_t>(g_effectQueueEntryCount - 1);
}



void SubmitToEffectBuffer(void *commandStream, float alphaScale, std::uint32_t renderFlags, void *modelBase) {
    if (!commandStream)
        return;
    Vector3 centre{};
    std::memcpy(&centre, static_cast<const std::byte *>(commandStream) + 0x10, sizeof(centre));
    const float sortKey = workTransformMatrix.rows[1].z * centre.y + workTransformMatrix.rows[0].z * centre.x + workTransformMatrix.rows[2].z * centre.z +
        workTransformMatrix.rows[3].z;
    EffectQueueEntry *const entry = allocateEffectQueueEntry(EffectQueueEntryKind::CommandStream, sortKey);
    if (!entry)
        return;
    entry->sceneObjectOrCommandStream = commandStream;
    entry->renderFlags = renderFlags;
    entry->alphaScale = alphaScale;
    entry->field_20 = reinterpret_cast<std::uintptr_t>(modelBase);
    entry->savedSceneReference = const_cast<std::int32_t *>(g_effectBufferHandleOverride);
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void resetEffectQueueOrder() {
    if (!g_effectQueueOrder)
        return;
    for (std::int32_t index = 0; index < g_effectQueueEntryCount; ++index) {
        g_effectQueueOrder[index] = static_cast<std::uint32_t>(index);
    }
}

void sortEffectQueueOrder() {
    if (!g_effectQueueSortKeys || !g_effectQueueOrder || !g_effectQueueOrderScratch || g_effectQueueEntryCount <= 0) {
        return;
    }
    RadixSortIndicesBy4ByteKey(g_effectQueueSortKeys, static_cast<std::uint32_t>(g_effectQueueEntryCount), g_effectQueueOrder, g_effectQueueOrderScratch);
}

void clearEffectQueue() {
    g_effectQueueEntryCount = 0;
    g_effectTransformContextCount = 0;
    g_effectAuxiliaryCount = 0;
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {






void EFFE_ReplayQueue(std::int32_t mode, std::uint32_t sceneFlagMask, EffectQueueEntryKind kind) {
    const Vector4 sceneLighting = g_sceneLightingActiveColour;
    bool restoreLights = false;
    const Matrix4x4 *pushedInstance = nullptr;
    bool pushed = false;
    const std::array<EffectLightRenderSlot, kEffectLightSlots> lights = g_effectLightRenderSlots;
    for (std::int32_t order = 0; order < g_effectQueueEntryCount; ++order) {
        EffectQueueEntry &entry = g_effectQueueEntries[g_effectQueueOrder[order]];
        Matrix4x4 *const instance = entry.instance;
        const std::int32_t sceneKey = static_cast<std::int32_t>((reinterpret_cast<std::uintptr_t>(entry.sceneObjectOrCommandStream) + 4U) % 0xffffffffULL);
        const GameNode *const scene = FindSceneEntityByID(sceneKey);
        const bool selected = (mode == 0 && (!scene || (scene->renderFlags & sceneFlagMask) == 0U)) ||
            (mode == 1 && (!scene || (scene->renderFlags & sceneFlagMask) != 0U)) || sceneFlagMask == 0U;
        if (!selected)
            continue;
        const bool callback = entry.kind == EffectQueueEntryKind::Callback && kind == EffectQueueEntryKind::Callback;
        const bool run = entry.kind == EffectQueueEntryKind::Run && kind == EffectQueueEntryKind::Run;
        const bool commandStream =
            entry.kind == EffectQueueEntryKind::CommandStream && kind == EffectQueueEntryKind::CommandStream && (entry.renderFlags & 2U) == 0U;
        bool dispatch = true;
        if (instance && pushedInstance != instance) {
            dispatch = run || callback || commandStream;
            if (dispatch) {
                const std::byte *const instanceBytes = reinterpret_cast<const std::byte *>(instance);
                Matrix4x4_Push(instance);
                pushed = true;
                std::uint32_t transformFlags = 0;
                std::memcpy(&transformFlags, instanceBytes + kEffectTransformFlagsOffset, sizeof(transformFlags));
                std::memcpy(&g_sceneObjectMaxScale, instanceBytes + kEffectTransformMaxScaleOffset, sizeof(g_sceneObjectMaxScale));
                if ((transformFlags & 4U) != 0U) {
                    restoreLights = true;
                    std::memcpy(g_effectLightRenderSlots.data(), instanceBytes + kEffectTransformLightSlotsOffset, sizeof(g_effectLightRenderSlots));
                }
                std::memcpy(&g_sceneLightingActiveColour.y, instanceBytes + kEffectTransformSceneLightingYZOffset, 2U * sizeof(float));
                std::memcpy(&g_sceneLightingActiveColour.w, instanceBytes + kEffectTransformSceneLightingWOffset, sizeof(float));
                pushedInstance = instance;
            }
        }
        if (dispatch) {
            if (callback || run) {
                std::bit_cast<EffectQueueCallback>(entry.dispatchFunction)(entry.dispatchArgument);
            } else if (commandStream) {
                entry.renderFlags |= 1U;
                g_effectBufferHandleOverride = static_cast<const std::int32_t *>(entry.savedSceneReference);
                DrawSceneObjectFromEffectBuffer(
                    entry.sceneObjectOrCommandStream, entry.alphaScale, static_cast<std::uint8_t>(entry.renderFlags), reinterpret_cast<void *>(entry.field_20));
            }
        }
        if (pushed) {
            Matrix4x4::Pop(1);
            pushed = false;
        }
        if (restoreLights) {
            restoreLights = false;
            g_sceneLightingActiveColour = sceneLighting;
            g_effectLightRenderSlots = lights;
        }
    }
}



void DrawEffectABufferWritePasses() {
    EFFE_ReplayQueue(1, 8, EffectQueueEntryKind::CommandStream);
    EFFE_ReplayQueue(0, 8, EffectQueueEntryKind::CommandStream);
    if (g_effectQueueEntryCount >= 1)
        sortEffectQueueOrder();
    EFFE_ReplayQueue(0, 0, EffectQueueEntryKind::Callback);
    for (const std::uint32_t setting : {60U, 61U, 58U, 59U, 64U, 57U}) {
        SetRenderFlags(setting, g_defaultRenderFlags[setting]);
        g_currentRenderFlags[setting] = g_defaultRenderFlags[setting];
    }
}



void DrawEffectCommandStreamDepthPass() {
    g_replayingEffectQueue = true;
    SetupCharacterAlphaDepthTarget(DX11::g_ppD3DDeviceContext);
    const Matrix4x4 *pushedInstance = nullptr;
    for (std::int32_t order = 0; order < g_effectQueueEntryCount; ++order) {
        EffectQueueEntry &entry = g_effectQueueEntries[g_effectQueueOrder[order]];
        Matrix4x4 *const instance = entry.instance;
        const bool draw = entry.kind == EffectQueueEntryKind::CommandStream && (entry.renderFlags & 2U) == 0U;
        bool pushed = false;
        if (instance && pushedInstance != instance) {
            if (!draw)
                continue;
            Matrix4x4_Push(instance);
            pushed = true;
            pushedInstance = instance;
            std::memcpy(&g_sceneObjectMaxScale, reinterpret_cast<const std::byte *>(instance) + kEffectTransformMaxScaleOffset, sizeof(g_sceneObjectMaxScale));
        }
        if (draw) {
            entry.renderFlags |= 8U;
            DrawSceneObjectFromEffectBuffer(
                entry.sceneObjectOrCommandStream, entry.alphaScale, static_cast<std::uint8_t>(entry.renderFlags), reinterpret_cast<void *>(entry.field_20));
        }
        if (pushed)
            Matrix4x4::Pop(1);
    }
    g_replayingEffectQueue = false;
    SetupPPFX(DX11::g_ppD3DDeviceContext);
}




void EFFE_DrawObjectPasses() {
    const Matrix4x4 *pushedInstance = nullptr;
    bool pushed = false;
    GetAspectRatio_0(DX11::g_ppD3DDeviceContext);
    for (std::int32_t order = 0; order < g_effectQueueEntryCount; ++order) {
        EffectQueueEntry &entry = g_effectQueueEntries[g_effectQueueOrder[order]];
        if (entry.kind == EffectQueueEntryKind::ObjectPass) {
            if (Matrix4x4 *const instance = entry.instance) {
                const std::byte *const instanceBytes = reinterpret_cast<const std::byte *>(instance);
                std::uint32_t transformFlags = 0;
                std::memcpy(&transformFlags, instanceBytes + kEffectTransformFlagsOffset, sizeof(transformFlags));
                const std::int32_t flag64 = (transformFlags & 1U) != 0U ? static_cast<std::int32_t>((transformFlags >> 3U) & 1U) : 0;
                const std::array<std::pair<std::uint32_t, std::int32_t>, 5> flags{{{60U, 1}, {61U, 0}, {58U, 517}, {59U, 1}, {64U, flag64}}};
                for (const std::pair<std::uint32_t, std::int32_t> &flag : flags) {
                    SetRenderFlags(flag.first, flag.second);
                    g_currentRenderFlags[flag.first] = flag.second;
                }
                if (pushedInstance != instance) {
                    Matrix4x4_Push(instance);
                    pushed = true;
                    pushedInstance = instance;
                    std::memcpy(&g_sceneObjectMaxScale, instanceBytes + kEffectTransformMaxScaleOffset, sizeof(g_sceneObjectMaxScale));
                }
            }
            std::bit_cast<EffectQueueCallback>(entry.dispatchFunction)(entry.dispatchArgument);
        }
        if (pushed) {
            Matrix4x4::Pop(1);
            pushed = false;
        }
    }
    for (const std::uint32_t setting : {60U, 61U, 58U, 59U, 64U, 57U}) {
        SetRenderFlags(setting, g_defaultRenderFlags[setting]);
        g_currentRenderFlags[setting] = g_defaultRenderFlags[setting];
    }
}



void EFFE_ReplayQueuedEffects(std::int32_t objectPasses) {
    SetupPPFX(DX11::g_ppD3DDeviceContext);
    g_replayingEffectQueue = true;
    resetEffectQueueOrder();
    if (objectPasses) {
        EFFE_DrawObjectPasses();
    } else {
        SetMt5RenderShaderSelection(7, 36, 20);
        DrawEffectABufferWritePasses();
    }
    SetMt5RenderShaderSelection(7, 7, 0);
    g_replayingEffectQueue = false;
}






void EFFE_DrawProjectedShadows() {
    D3TDeviceContext *const renderer = DX11::g_ppD3DDeviceContext;
    if (!renderer || !g_effectQueueEntries || !g_effectQueueOrder || g_effectQueueEntryCount <= 0) {
        return;
    }

    const Matrix4x4 *pushedInstance = nullptr;
    bool pushed = false;
    std::array<float, 6> window{};
    std::memcpy(window.data(), renderer->renderState.data() + 0x870, sizeof(window));
    g_replayingEffectQueue = true;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    GetTexture2DDimensions(g_fullScreenTexture2D, &width, &height);
    SetDirectXWindowConfig(0, 0, static_cast<std::int32_t>(width), static_cast<std::int32_t>(height), window[4], window[5]);
    if (g_effectQueueEntryCount >= 1)
        sortEffectQueueOrder();
    constexpr std::array<std::pair<std::uint32_t, std::int32_t>, 10> kSupersamplingRenderFlags{{
        {171, 5},
        {62, 770},
        {63, 772},
        {52, 1},
        {53, 7680},
        {55, 7680},
        {56, 517},
        {1, 0},
        {2, 15},
        {54, 7680},
    }};
    for (const std::pair<std::uint32_t, std::int32_t> &flag : kSupersamplingRenderFlags) {
        SetRenderFlags(flag.first, flag.second);
        g_currentRenderFlags[flag.first] = flag.second;
    }
    SetupCharacterShadowTarget(renderer);
    SetMt5RenderShaderSelection(39, 39, 23);
    for (std::int32_t order = 0; order < g_effectQueueEntryCount; ++order) {
        const std::uint32_t index = g_effectQueueOrder[order];
        if (index >= static_cast<std::uint32_t>(g_effectQueueEntryCount)) {
            continue;
        }
        EffectQueueEntry &entry = g_effectQueueEntries[index];
        if (entry.kind == EffectQueueEntryKind::CommandStream && entry.sceneObjectOrCommandStream && (entry.renderFlags & 2U) != 0U) {
            if (entry.instance && pushedInstance != entry.instance) {
                Matrix4x4_Push(entry.instance);
                pushed = true;
                pushedInstance = entry.instance;
                std::memcpy(&g_sceneObjectMaxScale, reinterpret_cast<const std::byte *>(entry.instance) + kEffectTransformMaxScaleOffset,
                    sizeof(g_sceneObjectMaxScale));
            }
            SetRenderFlags(56, 514);
            g_currentRenderFlags[56] = 514;
            SetRenderFlags(55, 7682);
            SetMt5RenderShaderSelection(39, 39, 24);
            g_currentRenderFlags[55] = 7682;
            DrawSceneObjectFromEffectBuffer(
                entry.sceneObjectOrCommandStream, entry.alphaScale, static_cast<std::uint8_t>(entry.renderFlags), reinterpret_cast<void *>(entry.field_20));
            constexpr std::array<std::pair<std::uint32_t, std::int32_t>, 5> kProjectionRenderFlags{{
                {171, 5},
                {62, 770},
                {63, 772},
                {55, 7680},
                {56, 517},
            }};
            for (const std::pair<std::uint32_t, std::int32_t> &flag : kProjectionRenderFlags) {
                SetRenderFlags(flag.first, flag.second);
                g_currentRenderFlags[flag.first] = flag.second;
            }
            SetMt5RenderShaderSelection(39, 39, 23);
            DrawSceneObjectFromEffectBuffer(
                entry.sceneObjectOrCommandStream, entry.alphaScale, static_cast<std::uint8_t>(entry.renderFlags), reinterpret_cast<void *>(entry.field_20));
        }
        if (pushed) {
            Matrix4x4::Pop(1);
            pushed = false;
        }
    }
    SetMt5RenderShaderSelection(7, 7, 0);
    g_replayingEffectQueue = false;


    for (const std::uint32_t setting : {171U, 62U, 63U, 52U, 53U, 55U, 56U, 1U, 2U, 54U}) {
        SetRenderFlags(setting, g_defaultRenderFlags[setting]);
        g_currentRenderFlags[setting] = g_defaultRenderFlags[setting];
    }
    SetupPPFX(renderer);
    SetDirectXWindowConfig(static_cast<std::int32_t>(window[0]), static_cast<std::int32_t>(window[1]), static_cast<std::int32_t>(window[2]),
        static_cast<std::int32_t>(window[3]), window[4], window[5]);
    for (const std::uint32_t setting : {60U, 61U, 58U, 59U, 64U, 57U}) {
        SetRenderFlags(setting, g_defaultRenderFlags[setting]);
        g_currentRenderFlags[setting] = g_defaultRenderFlags[setting];
    }
    DrawCharacterShadowComposite(renderer);
}

void EFFE_FinishQueuedEffects() {
    clearEffectQueue();
}

} // namespace shenmue::s1
