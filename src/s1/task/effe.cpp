#include "s1/task/effe_internal.hpp"

namespace shenmue::s1 {



Vector4 g_effeObjectLightColour{};

EffectQueueEntry *g_effectQueueEntries = nullptr;

std::int32_t g_effectQueueEntryCount = 0;

std::int32_t g_effectQueueEntryCapacity = 0;

std::uint32_t *g_effectQueueOrder = nullptr;

std::uint32_t *g_effectQueueOrderScratch = nullptr;

float *g_effectQueueSortKeys = nullptr;

bool g_replayingEffectQueue = false;

bool g_effectQueueReplayedCommandStream = false;

std::uint32_t g_effectScriptWordParameters[2]{0U, 3U};

Vector4 g_effectScriptMaximumIntensity{1.0f, 1.0f, 1.0f, 1.0f};

Vector3 g_effectScriptVectorParameters[2]{};

float g_effectScriptScalarParameters[3]{2.0f, 1.0f, 0.25f};

HLib::Task *g_effeTask = nullptr;

std::int32_t g_effectsTaskState = 0;

std::int32_t g_effePauseState = 0;

GLBSWeatherDrawState *g_pGLBSTaskData = nullptr;

WeatherNodeData *g_weatherNodeHead = nullptr;

std::array<EffeRegistryEntry, 16> g_effeEmitterRegistry{};

std::array<EffeRegistryEntry, 2> g_effeFftRegistry{};

std::array<EffeRegistryEntry, 4> g_effeTextureRegistry{};

std::array<EffeRegistryEntry, 2> g_effeTerrainRegistry{};

std::array<EffeEmitterTransform, 16> g_effeEmitterTransforms{};

std::array<EffeFftData *, 2> g_effeFftData{};

std::array<EffeFftOwner *, 16> g_effeFftOwners{};

// InitEffectTables' record state, which DebugEffect and FreeEffects restore when they release a record.
void EFFE_ReleaseRegistryEntry(EffeRegistryEntry &entry) {
    entry.state = 0;
    entry.reserved04 = -1;
    entry.fftIndex = -1;
    entry.callback = nullptr;
    entry.argument = nullptr;
    entry.token = kEffeEmptyRegistryToken;
    entry.reserved28 = 0;
}



std::int32_t EFFE_Related(std::int32_t table, std::int32_t fftIndex, EffeRegistryCallback callback, void *argument, std::uint32_t token) {
    EffeRegistryEntry *entries = nullptr;
    std::size_t count = 0;
    if (table == 0) {
        entries = g_effeEmitterRegistry.data();
        count = g_effeEmitterRegistry.size();
    } else if (table == 1) {
        entries = g_effeFftRegistry.data();
        count = g_effeFftRegistry.size();
    } else if (table == 3) {
        entries = g_effeTerrainRegistry.data();
        count = g_effeTerrainRegistry.size();
    } else if (table == 4) {
        entries = g_effeTextureRegistry.data();
        count = g_effeTextureRegistry.size();
    } else {
        return -1;
    }
    for (std::size_t index = 0; index < count; ++index) {
        EffeRegistryEntry &entry = entries[index];
        if (entry.state)
            continue;
        entry.state = 1;
        if (table == 0)
            entry.fftIndex = fftIndex;
        entry.callback = callback;
        entry.argument = argument;
        entry.token = token;
        if (table == 1) {
            auto *const fft = static_cast<EffeFftData *>(HLib_AllocHeapMemBlock(sizeof(EffeFftData), SHEN_NAME(PUFY)));
            g_effeFftData[index] = fft;
            InitializeFFTData(fft);
        }
        return static_cast<std::int32_t>(index);
    }
    return -1;
}

void EFFE_SetScriptPosition(const Vector4 &position) {
    g_effeObjectLightColour = position;
}

void EFFE_SetScriptWordParameter(std::uint32_t mode, std::uint32_t value) {
    if (mode < 2)
        g_effectScriptWordParameters[mode] = value;
}

void EFFE_SetScriptVector3Parameter(std::uint32_t mode, const Vector3 &value) {
    if (mode >= 3 && mode <= 4)
        g_effectScriptVectorParameters[mode - 3] = value;
}

void EFFE_SetScriptVector4Parameter(std::uint32_t mode, const Vector4 &value) {
    if (mode == 2)
        g_effectScriptMaximumIntensity = value;
}

void EFFE_SetScriptScalarParameter(std::uint32_t mode, float value) {
    if (mode >= 5 && mode <= 7)
        g_effectScriptScalarParameters[mode - 5] = value;
}

void EFFE_ResetScriptLightParametersForMap() {
    g_effectScriptWordParameters[0] = 0;
    g_effectScriptWordParameters[1] = 3;
    g_effectScriptMaximumIntensity = {1.0f, 1.0f, 1.0f, 1.0f};
    g_effectScriptScalarParameters[0] = 2.0f;
    g_effectScriptScalarParameters[1] = 1.0f;
    g_effectScriptScalarParameters[2] = 0.25f;
}

std::array<EffeRuntimeTexture, 16> g_effeRuntime{};



Vector3 g_effectSubmissionLightDirection{};

std::int32_t g_shouldLoadWeatherSprites = 1;

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

std::array<EffeTextureResource, kEffeSlots> g_effeResources{};

std::uint32_t g_effeSubmittedParticleSystems = 0;

std::uint32_t g_effeSubmittedParticleCount = 0;

std::int32_t g_effeRenderValidationSlot = -1;

bool g_effeRenderValidationSubmitted = false;


std::array<EffeTaskRecord *, kEffeSlots> g_specialTasks{};

std::array<EffeTaskRecord *, kEffeSlots> g_efptTasks{};

std::array<EffeTaskRecord *, kSakuraSlots> g_sakuraTasks{};

std::array<EffeTaskRecord *, kEffeSlots> g_sakuraRuntimeOwners{};

std::array<EffeTaskRecord *, kEffeSlots> g_genericTasks{};

std::array<std::int32_t, 5> g_vfFightGroundTaskSlots{-1, -1, -1, -1, -1};

std::array<VFFightGroundEffectRecord, 5> g_vfFightGroundEffects{};

std::uint8_t g_vfFightGroundEffectCount = 0;

std::array<Vector3, 4> g_genericAnchors{};

std::array<float, 5> g_specialLifetime{
    0.1f,
    0.1f,
    0.1f,
    0.1f,
    0.1f,
};

std::array<float, 5> g_specialDecayScale{
    1.2019608f,
    0.91176468f,
    1.2019608f,
    1.2019608f,
    1.2019608f,
};

std::array<float, 5> g_specialDecayStep{
    0.0060392157f,
    0.0060392157f,
    0.0060392157f,
    0.0060392157f,
    0.0060392157f,
};

std::array<float, 2> g_specialRadial{};

std::array<float, 2> g_specialDamping{};

std::array<float, 2> g_specialMultiplier{1.0f, 1.0f};

std::array<std::int32_t, 2> g_specialFadeStep{3, 3};

float g_specialIntensity = 32.0f;

std::int32_t g_particleSpawnSeed = 1;

std::uint32_t g_specialPhase = 0;

float g_genericFloor = -1.0f;

float g_sakuraRotation = 0.0f;

float g_sakuraRotationScale = 0.0001f;

float g_sakuraRotationBase = 0.0f;

std::uint32_t g_sakuraStrengthBase = 0;

std::int32_t g_sakuraStrength = 0;

const std::byte *g_sakuraPvr = nullptr;

std::size_t g_sakuraPvrBytes = 0;

std::uint32_t g_sakuraColour = 0;

Matrix4x4 g_particleScreenProjection{};

HLib::Task *g_effeUpdateTask = nullptr;

HLib::Task *g_sakuraUpdateTask = nullptr;

std::uint32_t g_effectSceneGeneration = 0;

WeatherSpriteTexture g_weatherSnowTexture{};

WeatherSpriteTexture g_weatherHamonTexture{};

ActiveWeatherNode *g_activeWeatherNodes = nullptr;

WeatherFixc *g_tmpWeatherData = nullptr;

bool g_effeWeatherFlag = false;

std::int32_t g_bForceDisableSnow = 1;

std::uint32_t g_rainEffectOffset = 0x15b7b9bbU;

std::uint32_t g_rainUpdateOffset = 0xe8cdc8d1U;

std::uint32_t g_weatherColourRampValue = 0xa0ffffffU;

float g_rainParticleWidth = 0.0035f;

float g_rainParticleHeight = 0.15f;

Vector3 g_rainEffectVector0{0.0f, -0.28f, 0.0f};

Vector3 g_rainEffectVector1{};

float g_snowParticleSize = 0.08f;

Vector3 g_snowEffectVector0{0.0f, -0.095f, 0.0f};

Vector3 g_snowEffectVector1{0.013f, 0.02f, 0.013f};

float g_rippleParticleSize = 0.15f;

EffeRuntimeTexture *g_effeCurrentSystem = nullptr;

float g_weatherSnowBudget = 0.0f;

std::byte *g_effectTransformContexts = nullptr;

void *g_effectAuxiliaryEntries = nullptr;

std::uint32_t *g_effectAuxiliaryOrder = nullptr;

std::int32_t g_effectTransformContextCapacity = 0;

std::int32_t g_effectAuxiliaryCapacity = 0;

std::int32_t g_effectTransformContextCount = 0;

std::int32_t g_effectAuxiliaryCount = 0;

std::int32_t g_effectObjectPassCount = 0;

std::int32_t g_effectCallbackCount = 0;

std::int32_t g_effectRunCount = 0;

Matrix4x4 g_effectTransform{};

std::uint64_t g_effectReserved = 0;

std::uint32_t g_effectPackedColour = 0xff7f7f7fU;

std::int32_t g_effectBuffersEnabled = 0;

std::int32_t g_effectSortEnabled = 0;

float g_effectDefaultSortKey = 0.0f;

void ResetRuntimeEffectTablesNative() {
    EffeRuntimeTexture initial{};
    initial.descriptor[1] = 0x8a000000U;
    initial.descriptor[2] = 0x208000c0U;
    initial.particleUpdate = EFFE_NoOpParticle;
    initial.halfWidth = 1.0f;
    initial.halfHeight = 1.0f;
    initial.colour = 0xffffffffU;
    g_effeRuntime.fill(initial);
}

void ResetEffectRegistryTablesNative() {
    g_effeResources = {};
    for (std::size_t slot = 0; slot < g_effeEmitterRegistry.size(); ++slot) {
        EFFE_ReleaseRegistryEntry(g_effeEmitterRegistry[slot]);
        g_effeEmitterTransforms[slot].enabled = 0;
    }
    for (std::size_t index = 0; index < g_effeFftRegistry.size(); ++index) {
        EFFE_ReleaseRegistryEntry(g_effeFftRegistry[index]);
        EFFE_ReleaseRegistryEntry(g_effeTerrainRegistry[index]);
    }
    for (EffeRegistryEntry &entry : g_effeTextureRegistry)
        EFFE_ReleaseRegistryEntry(entry);
    g_effectQueueEntryCount = 0;
    g_effectTransformContextCount = 0;
    g_effectAuxiliaryCount = 0;
    g_effectObjectPassCount = 0;
    g_effectCallbackCount = 0;
    g_effectRunCount = 0;
    g_replayingEffectQueue = false;
    g_effectQueueReplayedCommandStream = false;
}

void InitEffectTablesNative() {
    ResetRuntimeEffectTablesNative();
    g_effeObjectLightColour = {1.0f, 0.3f, 0.3f, 0.3f};
    refbSetGlobalActive(1);
    detail::TRWV_InitializeEffectTablesNative();
    g_effeUpdateTask = nullptr;
    ResetEffectRegistryTablesNative();
    g_effePauseState = 0;
}

void EnsureEffeOwnerNative() {
    g_pauseRuntime.setEffectsPaused = SetEffeParticlesPaused;
    if (g_effeTask)
        return;
    g_effeTask = HLib_EnqueueTaskWithoutParameter(EFFE_Callback, 14, 8, static_cast<std::int32_t>(SHEN_NAME(EFFE)));
}

void ResetPrivateSceneEffectGlobalsNative() {
    g_sakuraPvr = nullptr;
    g_sakuraPvrBytes = 0;
    g_specialTasks = {};
    g_efptTasks = {};
    g_sakuraTasks = {};
    g_sakuraRuntimeOwners = {};
    g_genericTasks = {};
    g_vfFightGroundTaskSlots.fill(-1);
    g_vfFightGroundEffects = {};
    g_vfFightGroundEffectCount = 0;
    g_effeFftOwners.fill(nullptr);
    g_effeRenderValidationSlot = -1;
    g_effeRenderValidationSubmitted = false;
    g_genericAnchors = {};
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

// SCN3_Process: InitEffectTables drops the previous EFFE_Callback_0 owner, and a new one is enqueued.
void EFFE_ResetSceneStateNative() {
    InitEffectTablesNative();
    EnsureEffeUpdateTask();
    ++g_effectSceneGeneration;
    ResetPrivateSceneEffectGlobalsNative();
    detail::TRWV_ResetSceneGlobalsNative();
}

HLib::Task *EFFE_Init() {
    g_activeWeatherNodes = nullptr;
    g_tmpWeatherData = nullptr;
    g_weatherNodeHead = nullptr;
    g_effeTask = nullptr;
    g_effeUpdateTask = nullptr;
    g_sakuraUpdateTask = nullptr;
    ResetEffeRuntimeForScene();
    g_weatherCameraState = ICAR();
    g_effectsTaskState = 0;
    g_pGLBSTaskData = nullptr;
    g_weatherNodeHead = nullptr;
    g_rainParticleWidth = 0.0035f;
    g_rainEffectOffset = 0x15b7b9bbU;
    g_rainEffectVector0 = {0.0f, -0.28f, 0.0f};
    g_rainEffectVector1 = {};
    g_rainParticleHeight = 0.15f;
    g_rainUpdateOffset = 0xe8cdc8d1U;
    g_snowParticleSize = 0.08f;
    g_snowEffectVector0 = {0.0f, -0.095f, 0.0f};
    g_snowEffectVector1 = {0.013f, 0.02f, 0.013f};
    g_rippleParticleSize = 0.15f;
    g_weatherColourRampValue = 0xa0ffffffU;
    EnsureEffeOwnerNative();
    WIND_InitializeSakuraWindState();
    WEXE_Init();
    AllocFIXC();

    if (g_tmpWeatherData && g_effeWeatherFlag) {
        RemoveActiveWeatherNode(g_tmpWeatherData);
        g_effeWeatherFlag = false;
    }
    return g_effeTask;
}

void EFFE_Destroy() {
    if (g_pauseRuntime.setEffectsPaused == SetEffeParticlesPaused)
        g_pauseRuntime.setEffectsPaused = nullptr;
    if (g_effeTask) {
        HLib_DestroyTask(g_effeTask);
        g_effeTask = nullptr;
    }
    if (g_effeUpdateTask) {
        HLib_DestroyTask(g_effeUpdateTask);
        g_effeUpdateTask = nullptr;
    }
    if (g_sakuraUpdateTask) {
        HLib_DestroyTask(g_sakuraUpdateTask);
        g_sakuraUpdateTask = nullptr;
    }

    while (g_activeWeatherNodes)
        FreeWeatherFixc(g_activeWeatherNodes->weather);
    if (g_tmpWeatherData)
        FreeWeatherFixc(g_tmpWeatherData);
    while (WeatherNodeData *const data = FirstWeatherNode())
        FreeGlobalWeatherData(data);
    EFFE_FreeVFFightGroundEffects();
    for (std::int32_t slot = 0; slot < static_cast<std::int32_t>(kEffeSlots); ++slot) {
        FreeEffeTask(EffeFamily::Special, slot);
        FreeEffeTask(EffeFamily::Efpt, slot);
        FreeEffeTask(EffeFamily::Generic, slot);
    }
    for (std::size_t slot = 0; slot < g_sakuraRuntimeOwners.size(); ++slot) {
        FreeSakuraTask(g_sakuraRuntimeOwners[slot]);
        ReleaseEffeTexture(static_cast<std::int32_t>(slot));
    }
    ResetEffeRuntimeForScene();
    WEXE_Destroy();
}

void ResetEffeRuntimeForScene() {
    g_tmpWeatherData = nullptr;
    g_effeWeatherFlag = false;
    g_shouldLoadWeatherSprites = 1;
    g_sakuraPvr = nullptr;
    g_sakuraPvrBytes = 0;
    ResetRuntimeEffectTablesNative();
    g_effeResources = {};
    for (EffeRegistryEntry &entry : g_effeEmitterRegistry)
        EFFE_ReleaseRegistryEntry(entry);
    g_specialTasks = {};
    g_efptTasks = {};
    g_sakuraTasks = {};
    g_sakuraRuntimeOwners = {};
    g_effeRenderValidationSlot = -1;
    g_effeRenderValidationSubmitted = false;
    g_genericTasks = {};
    g_effeFftOwners.fill(nullptr);
    g_genericAnchors = {};
    g_specialLifetime.fill(0.1f);
    g_specialDecayScale = {1.2019608f, 0.91176468f, 1.2019608f, 1.2019608f, 1.2019608f};
    g_specialDecayStep.fill(0.0060392157f);
    g_specialRadial = {};
    g_specialDamping = {};
    g_specialMultiplier = {1.0f, 1.0f};
    g_specialFadeStep = {3, 3};
    g_specialIntensity = 32;
    g_specialPhase = 0;
    g_genericFloor = -1.0f;
    g_sakuraRotation = 0.0f;
    g_sakuraRotationScale = 0.0001f;
    g_sakuraRotationBase = 0.0f;
    g_sakuraStrengthBase = 0;
    g_sakuraStrength = 0;
    g_pGLBSTaskData = nullptr;
    g_weatherNodeHead = nullptr;
    g_effectQueueEntryCount = 0;
    g_effectTransformContextCount = 0;
    g_effectAuxiliaryCount = 0;
    g_replayingEffectQueue = false;
    g_effectQueueReplayedCommandStream = false;
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

std::array<std::int32_t, 4> g_effectValidationOrder{};

std::array<float, 4> g_effectValidationMatrices{};

std::int32_t g_effectValidationCount = 0;

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_Callback(HLib::TaskData *taskData) {
    if (!g_effectsTaskState || !g_updateRenderFrameMode)
        return;
    EFFE_Run(taskData, g_pGLBSTaskData, 0.0f);
}

void EFFE_Callback2(void *sceneData) {
    if (!sceneData || !g_matrixStack.top.stack || !g_matrixStack.entries[2].stack) {
        return;
    }

    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = {};
    workTransformMatrix.rows[0].x = 320.0f;
    workTransformMatrix.rows[1].y = -240.0f;
    workTransformMatrix.rows[2].z = 1.0f;
    workTransformMatrix.rows[3] = {320.0f, 240.0f, 0.0f, 1.0f};
    Matrix4x4::Multiply(g_matrixStack.entries[2].stack);
    g_particleScreenProjection = workTransformMatrix;
    const float screenScaleX = workTransformMatrix.rows[0].x;
    const float screenScaleY = workTransformMatrix.rows[1].y;
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);

    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext ? DX11::g_ppD3DDeviceContext : g_RenderDeviceContext;
    for (auto *owner = static_cast<WeatherNodeData *>(sceneData); owner; owner = owner->next) {
        const std::uint32_t rainFlags = owner->rain.flags;
        const std::int32_t rainCount = owner->rain.pool.renderCount;
        auto *const rainRecords = reinterpret_cast<std::byte *>(owner->rain.pool.records);
        if ((rainFlags & 0x80000000U) != 0 && rainCount && rainRecords) {
            const float width = owner->rain.width;
            const float height = owner->rain.height;
            const WeatherDrawGroup group{.owner = owner,
                .records = rainRecords,
                .count = rainCount,
                .stride = 0x28,
                .flags = rainFlags,
                .width = width,
                .height = height,
                .scaledWidth = width * screenScaleX,
                .scaledHeight = height * screenScaleY};
            SubmitWeatherParticleGroup(context, group, owner->rain.material, owner->rain.textureHandle);
        }
        const std::uint32_t snowFlags = owner->snow.flags;
        const std::int32_t snowCount = owner->snow.pool.renderCount;
        auto *const snowRecords = reinterpret_cast<std::byte *>(owner->snow.pool.records);
        if ((snowFlags & 0x80000000U) != 0 && snowCount && snowRecords) {
            const float size = owner->snow.size;
            const WeatherDrawGroup group{.owner = owner,
                .records = snowRecords,
                .count = snowCount,
                .stride = 0x28,
                .flags = snowFlags,
                .width = size,
                .height = 0.0f,
                .scaledWidth = size * screenScaleX,
                .scaledHeight = 0.0f};
            SubmitWeatherParticleGroup(context, group, owner->snow.material, owner->snow.textureHandle);
        }
        const std::uint32_t rippleFlags = owner->ripple.flags;
        const std::int32_t rippleCount = owner->ripple.pool.renderCount;
        auto *const rippleRecords = reinterpret_cast<std::byte *>(owner->ripple.pool.records);
        if ((rippleFlags & 0x80000000U) != 0 && rippleCount && rippleRecords) {
            const WeatherDrawGroup group{.owner = owner,
                .records = rippleRecords,
                .count = rippleCount,
                .stride = 0x38,
                .flags = rippleFlags,
                .width = 0.0f,
                .height = 0.0f,
                .scaledWidth = owner->ripple.size,
                .scaledHeight = 0.0f};
            SubmitWeatherParticleGroup(context, group, owner->ripple.material, owner->ripple.textureHandle);
        }
    }

    Matrix4x4::Pop();
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

float g_inverseGeometryScale = kDefaultInverseGeometryScale;

} // namespace shenmue::s1::effe_detail
