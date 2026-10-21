#include "s1/task/effe_internal.hpp"

namespace shenmue::s1 {

void EFFE_InitializeParticleSystem(std::int32_t slot, std::int32_t count, EffeParticle *particles) {
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(slot)];
    runtime.flags &= ~kAlternateParticleRecordFlag;
    runtime.particleCount = count;
    runtime.particles = particles;
    runtime.descriptor[0] = 0x80000002U;
    EFFE_FillParticleTemplate(particles, count);
    runtime.descriptor[1] = ((runtime.descriptor[0] & 0xfU) | 0xfffffe20U) << 22U;
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

// A special, EFPT or generic task: the first free family slot, the 0x80 record ('EFPT'), an EFFE_Related entry carrying the record and the
// 96 * count particles EFFE_InitParticleSystem borrows. Returns the family slot.
std::int32_t AllocateEffeTask(EffeFamily family, std::int32_t count, EffeRegistryCallback callback) {
    std::array<EffeTaskRecord *, kEffeSlots> &tasks = FamilyTasks(family);
    std::int32_t familySlot = 0;
    while (familySlot < static_cast<std::int32_t>(kEffeSlots) && tasks[static_cast<std::size_t>(familySlot)])
        ++familySlot;
    if (count < 0 || familySlot >= static_cast<std::int32_t>(kEffeSlots))
        return -1;
    auto *const task = static_cast<EffeTaskRecord *>(HLib_AllocHeapMemBlock(sizeof(EffeTaskRecord), static_cast<std::int32_t>(SHEN_NAME(EFPT))));
    if (!task)
        return -1;
    const std::int32_t runtimeSlot = EFFE_Related(0, -1, callback, task, SHEN_NAME(EFPT));
    if (runtimeSlot < 0) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task), 0);
        return -1;
    }
    EffeParticle *particles = nullptr;
    if (count) {
        particles = static_cast<EffeParticle *>(HLib_AllocHeapMemBlock(
            static_cast<std::int64_t>(static_cast<std::size_t>(count) * sizeof(EffeParticle)), static_cast<std::int32_t>(SHEN_NAME(EFPT))));
        if (!particles) {
            EFFE_ReleaseRegistryEntry(g_effeEmitterRegistry[static_cast<std::size_t>(runtimeSlot)]);
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task), 0);
            return -1;
        }
    }
    *task = {};
    task->runtimeSlot = runtimeSlot;
    task->particleCount = count;
    task->particles = particles;
    tasks[static_cast<std::size_t>(familySlot)] = task;
    EFFE_InitializeParticleSystem(runtimeSlot, count, particles);
    return familySlot;
}

// The record into its system. EFPT and generic pick their spawn callback from flag byte 1; special from the record subtype.
void SyncEffeTask(EffeFamily family, const EffeTaskRecord &task) {
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)];
    if (family == EffeFamily::Special && task.subtype >= 0 && task.subtype < 5)
        runtime.particleUpdate = kSpecialSpawns[static_cast<std::size_t>(task.subtype)];
    else if (family == EffeFamily::Efpt && ((task.flags >> 8U) & 0xfU) < 4U)
        runtime.particleUpdate = kEfptSpawns[(task.flags >> 8U) & 0xfU];
    else if (family == EffeFamily::Generic && ((task.flags >> 8U) & 0xffU) < 4U)
        runtime.particleUpdate = kGenericSpawns[(task.flags >> 8U) & 0xffU];
    runtime.spawnProbabilityQ15 = static_cast<std::int32_t>(static_cast<double>(task.probability) * 32767.0);
    runtime.positionMin = task.position;
    runtime.positionRange = task.range;
    runtime.velocityMin = task.velocity;
    runtime.velocityRange = task.velocityRange;
    const float scale = (runtime.flags & 0x20000000U) ? 1.0f : 0.5f;
    runtime.halfWidth = task.width * scale;
    runtime.halfHeight = task.height * scale;
}

void SetEffeActive(EffeTaskRecord &task, bool active) {
    g_effeEmitterRegistry[static_cast<std::size_t>(task.runtimeSlot)].state = active ? 2 : 1;
    if (active)
        task.flags |= 2U;
    else
        task.flags &= ~2U;
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)];
    if (active)
        runtime.flags |= kActiveTextureFlag;
    else
        runtime.flags &= ~kActiveTextureFlag;
    EffeFftOwner *const owner = task.fftOwner;
    if (!owner || task.subtype < 0 || task.subtype > 4)
        return;
    const auto index = static_cast<std::size_t>(owner->fftIndex);
    const auto bit = static_cast<std::uint16_t>(1U << static_cast<std::uint32_t>(task.runtimeSlot));
    if (active) {
        if (g_effeFftRegistry[index].state == 1) {
            g_effeFftRegistry[index].state = 2;
            g_effeFftData[index]->flags |= 0x80000000U;
        }
        owner->activeMask |= bit;
        return;
    }
    owner->activeMask &= static_cast<std::uint16_t>(~bit);
    if (!owner->activeMask && g_effeFftRegistry[index].state == 2) {
        g_effeFftRegistry[index].state = 1;
        g_effeFftData[index]->flags &= ~0x80000000U;
    }
}




void FreeEffeTask(EffeFamily family, std::int32_t slot) {
    EffeTaskRecord *const task = TaskAt(family, slot);
    if (!task)
        return;
    if (task->particles)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task->particles), 0);
    FamilyTasks(family)[static_cast<std::size_t>(slot)] = nullptr;
    EffeFftOwner *const owner = task->fftOwner;
    if (owner && task->subtype >= 0 && task->subtype <= 4) {
        if (owner->pixels)
            ReleaseEffeTexture(task->runtimeSlot);
        EFFE_ReleaseFftOwner(owner, task->flags);
    }
    g_effeRuntime[static_cast<std::size_t>(task->runtimeSlot)].flags &= ~kActiveTextureFlag;
    EFFE_ReleaseRegistryEntry(g_effeEmitterRegistry[static_cast<std::size_t>(task->runtimeSlot)]);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task), 0);
}

// Index -1 draws every system. Otherwise the other systems keep their 0x02000000/0x04000000 bits in 0x00800000/0x01000000 and have both set
// while the one system draws; the bits are moved back afterwards and the saved copies stay.
void EFFE_SubmitForDraw(void *rawIndex) {
    const auto index = static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(rawIndex));
    if (index == -1) {
        for (std::size_t slot = 0; slot < g_effeRuntime.size(); ++slot)
            SubmitAlphaQuads(slot);
        return;
    }
    for (std::size_t slot = 0; slot < g_effeRuntime.size(); ++slot) {
        EffeRuntimeTexture &runtime = g_effeRuntime[slot];
        if (static_cast<std::int32_t>(slot) != index)
            runtime.flags = (runtime.flags & 0xfe7fffffU) | (((runtime.flags & 0x06000000U) | 0x18000000U) >> 2U);
    }
    SubmitAlphaQuads(static_cast<std::size_t>(index));
    for (std::size_t slot = 0; slot < g_effeRuntime.size(); ++slot) {
        EffeRuntimeTexture &runtime = g_effeRuntime[slot];
        if (static_cast<std::int32_t>(slot) != index)
            runtime.flags = (runtime.flags & 0xf9ffffffU) | ((runtime.flags & 0x01800000U) << 2U);
    }
}

// Every allocated emitter queues its draw at sort key 900, under its transform when that is enabled.
void EFFE_Update() {
    g_effeSubmittedParticleSystems = 0;
    g_effeSubmittedParticleCount = 0;
    const std::int32_t shouldDraw = g_bShouldDraw;
    for (std::size_t slot = 0; slot < g_effeEmitterRegistry.size(); ++slot) {
        if (!g_effeEmitterRegistry[slot].state || !shouldDraw)
            continue;
        void *const argument = reinterpret_cast<void *>(static_cast<std::intptr_t>(slot));
        const EffeEmitterTransform &transform = g_effeEmitterTransforms[slot];
        if (!transform.enabled) {
            EFFE_AssignCallback(EFFE_SubmitForDraw, argument, 900.0f);
            continue;
        }
        Matrix4x4_Push();
        Matrix4x4::preTranslate(transform.position.x, transform.position.y, transform.position.z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(transform.angleZ));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(transform.angleY));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(transform.angleX));
        Matrix4x4::preTranslate(transform.offset.x, transform.offset.y, transform.offset.z);
        EFFE_AssignCallback(EFFE_SubmitForDraw, argument, 900.0f);
        Matrix4x4::Pop(1);
    }
}

EffeTaskRecord *SakuraTaskAt(std::int32_t slot) {
    return slot >= 0 && slot < static_cast<std::int32_t>(g_sakuraTasks.size()) ? g_sakuraTasks[static_cast<std::size_t>(slot)] : nullptr;
}

void FreeSakuraTask(EffeTaskRecord *task) {
    if (!task)
        return;
    if (task->particles)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task->particles), 0);
    for (EffeTaskRecord *&entry : g_sakuraTasks)
        if (entry == task)
            entry = nullptr;
    const auto slot = static_cast<std::size_t>(task->runtimeSlot);
    EffeFftOwner *const owner = task->fftOwner;
    if (owner && task->subtype >= 0 && task->subtype <= 4) {
        if (owner->pixels)
            ReleaseEffeTexture(task->runtimeSlot);
        EFFE_ReleaseFftOwner(owner, task->flags);
    }
    g_effeRuntime[slot].flags &= ~kActiveTextureFlag;
    EFFE_ReleaseRegistryEntry(g_effeEmitterRegistry[slot]);
    g_sakuraRuntimeOwners[slot] = nullptr;
    // Without a shared FFT owner, slot reuse or map teardown owns the retained texture's release.
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task), 0);
}

// @verified - p
void SetSakuraActive(EffeTaskRecord &task, bool active) {
    const std::size_t slot = static_cast<std::size_t>(task.runtimeSlot);
    EffeRegistryEntry &entry = g_effeEmitterRegistry[slot];
    EffeRuntimeTexture &runtime = g_effeRuntime[slot];
    EffeFftOwner *const owner = task.fftOwner;
    if (active) {
        if (entry.state == 1) {
            entry.state = 2;
            runtime.flags |= kActiveTextureFlag;
        }
        if (owner && static_cast<std::uint32_t>(task.subtype) <= 4U) {
            const std::size_t index = static_cast<std::size_t>(owner->fftIndex);
            if (g_effeFftRegistry[index].state == 1) {
                g_effeFftRegistry[index].state = 2;
                g_effeFftData[index]->flags |= kActiveTextureFlag;
            }
            owner->activeMask |= static_cast<std::uint16_t>(1U << static_cast<std::uint32_t>(task.runtimeSlot));
        }
        task.flags |= 2U;
    } else {
        if (entry.state == 2) {
            entry.state = 1;
            runtime.flags &= ~kActiveTextureFlag;
        }
        if (owner && static_cast<std::uint32_t>(task.subtype) <= 4U) {
            owner->activeMask &= static_cast<std::uint16_t>(~(1U << static_cast<std::uint32_t>(task.runtimeSlot)));
            const std::size_t index = static_cast<std::size_t>(owner->fftIndex);
            if (!owner->activeMask && g_effeFftRegistry[index].state == 2) {
                g_effeFftRegistry[index].state = 1;
                g_effeFftData[index]->flags &= ~kActiveTextureFlag;
            }
        }
        task.flags &= ~2U;
    }
}

void SetSakuraPositionRange(EffeTaskRecord &task, const Vector3 &position, const Vector3 &range) {
    task.position = position;
    task.range = range;
    EffeRuntimeTexture &runtime = g_effeRuntime[task.runtimeSlot];
    runtime.positionMin = position;
    runtime.positionRange = range;
}

void SetSakuraVelocityRange(EffeTaskRecord &task, const Vector3 &velocity, const Vector3 &range) {
    task.velocity = velocity;
    task.velocityRange = range;
    EffeRuntimeTexture &runtime = g_effeRuntime[task.runtimeSlot];
    runtime.velocityMin = velocity;
    runtime.velocityRange = range;
}

void SetSakuraProbability(EffeTaskRecord &task, float probability) {
    task.probability = probability;
    g_effeRuntime[task.runtimeSlot].spawnProbabilityQ15 = static_cast<std::int32_t>(static_cast<double>(probability) * 32767.0);
}

void SetSakuraDimensions(EffeTaskRecord &task, float width, float height) {
    task.width = width;
    task.height = height;
    EffeRuntimeTexture &runtime = g_effeRuntime[task.runtimeSlot];
    const float scale = (runtime.flags & kAlternateParticleRecordFlag) ? 1.0f : 0.5f;
    runtime.halfWidth = width * scale;
    runtime.halfHeight = height * scale;
}

void TranslateSakuraParticles(EffeTaskRecord &task, const Vector3 &amount) {
    for (std::int32_t index = 0; index < task.particleCount; ++index) {
        Vector3 &position = task.particles[index].position;
        position.x += amount.x;
        position.y += amount.y;
        position.z += amount.z;
    }
}

std::array<std::uint8_t, 12> ReadSakuraControls() {
    std::array<std::uint8_t, 12> controls;

    std::memcpy(controls.data(), &g_inputState.slots[1].triggers, 6);
    std::memcpy(controls.data() + 6, &g_inputState.slots[3].triggers, 6);
    return controls;
}

// Sakura's EFFE::GlobalState callback: the dormant flag-bit-0 tuning, then the petal colour from light slot 0.
void UpdateSakuraOwner(void *argument) {
    EffeTaskRecord &task = *static_cast<EffeTaskRecord *>(argument);
    if ((task.flags & 1U) != 0) {
        const auto controls = ReadSakuraControls();
        SetSakuraDimensions(task, static_cast<float>(2.0 * controls[6] / 255.0), static_cast<float>(2.0 * controls[7] / 255.0));
        const auto velocity = [](std::uint8_t value) { return static_cast<float>(static_cast<double>(static_cast<std::int32_t>(value) - 128) / 255.0 * 0.04); };
        SetSakuraVelocityRange(task, {velocity(controls[8]), velocity(controls[9]), velocity(controls[10])},
            {velocity(controls[0]), velocity(controls[1]), velocity(controls[2])});
        task.particleScale = static_cast<float>(static_cast<double>(controls[11]) / 255.0);
        SetSakuraProbability(task, static_cast<float>(static_cast<double>(controls[3]) / 255.0 * 0.001));
    }
    const ShenmueLightSlot *const data = LGHT_GetDataSlot(0);
    const ShenmueLightSlot *const render = LGHT_GetRenderSlot(0);
    if (!data || !render)
        return;
    const auto channel = [](float value) { return static_cast<std::uint32_t>(static_cast<std::int32_t>(value * 255.0f)); };
    g_sakuraColour = (channel(render->colourScale) << 24U) | (channel(render->colourR * data->intensity) << 16U) |
        (channel(render->colourG * data->intensity) << 8U) | channel(render->colourB * data->intensity);
}

// EFFE_Callback_0: the FFT records, then every active emitter's callback and FFT texture upload, then EFFE_Update.
void EffectTaskCallback(HLib::TaskData *) {
    for (std::size_t index = 0; index < g_effeFftRegistry.size(); ++index) {
        const EffeRegistryEntry &entry = g_effeFftRegistry[index];
        if (entry.state != 2)
            continue;
        if (!g_effePauseState && entry.callback)
            entry.callback(entry.argument);
        ProcessAndQuantizeFFTOutput(g_effeFftData[index]);
    }
    for (std::size_t slot = 0; slot < g_effeEmitterRegistry.size(); ++slot) {
        const EffeRegistryEntry &entry = g_effeEmitterRegistry[slot];
        if (entry.state != 2)
            continue;
        if (entry.callback)
            entry.callback(entry.argument);
        if (entry.fftIndex < 0 || g_effePauseState)
            continue;
        const auto *const pixels = reinterpret_cast<const std::byte *>(
            (reinterpret_cast<std::uintptr_t>(g_effeFftData[static_cast<std::size_t>(entry.fftIndex)]->output) + 31U) & ~std::uintptr_t{31});
        const Texture2DDescription description{.width = 16,
            .height = 16,
            .arraySize = 1,
            .mipLevels = 1,
            .format = 2,
            .bindFlags = 0,
            .flags = 0,
            .sampleCount = 1,
            .sampleQuality = 0,
            .reserved_24 = 0,
            .initialData = pixels};
        const EffeRuntimeTexture &runtime = g_effeRuntime[slot];
        if (runtime.resource && runtime.resource->textureHandle)
            ReplaceTexture2DResource(runtime.resource->textureHandle, &description);
    }
    EFFE_Update();
}

void SakuraTaskCallback(HLib::TaskData *) {
    float sum = 0.0f;
    const float strength = static_cast<float>(std::bit_cast<std::int32_t>((g_sakuraStrengthBase << (g_sakuraStrength & 31)) * 65535U));
    for (const EffeTaskRecord *task : g_sakuraTasks)
        if (task)
            sum += sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(strength * task->probability) / 60.0)));
    g_sakuraRotation = g_sakuraRotationScale * sum + g_sakuraRotationBase;
    const auto controls = ReadSakuraControls();
    const float probability = static_cast<float>(controls[9] * 0.005 / 255.0);
    const std::int32_t threshold = static_cast<std::int32_t>(static_cast<double>(probability) * 32767.0);
    for (std::size_t index = 0; index < g_sakuraTasks.size(); ++index) {
        EffeTaskRecord *const task = g_sakuraTasks[index];
        if (!task)
            continue;
        task->probability = static_cast<float>(static_cast<double>(controls[index]) / 255.0);
        g_effeRuntime[task->runtimeSlot].spawnProbabilityQ15 = threshold;
    }
    g_sakuraRotationScale = static_cast<float>(controls[8] * 0.001 / 255.0);
    g_sakuraRotationBase = static_cast<float>((static_cast<std::int32_t>(controls[10]) - 128) * 0.0005 / 255.0);
    g_sakuraStrength = static_cast<std::int32_t>(10 * controls[11] / 255.0);

    if (g_bDebugLog) {
        g_debugTextPosition = {1.0f, 5.0f};
        std::array<char, 64> line{};
        for (std::size_t row = 0; row < 2; ++row) {
            const std::size_t base = row * 6;
            std::snprintf(line.data(), line.size(), "%d %d %d %d %d %d", controls[base], controls[base + 1], controls[base + 2], controls[base + 3],
                controls[base + 4], controls[base + 5]);
            (void)QueueDebugText(g_debugTextPosition[0], g_debugTextPosition[1], line.data());
        }
    }
    ++g_sakuraStrengthBase;
}

void EnsureEffeUpdateTask() {
    if (!g_effeUpdateTask)
        g_effeUpdateTask = HLib_EnqueueTaskWithoutParameter(EffectTaskCallback, 8, 8, static_cast<std::int32_t>(SHEN_NAME(EFFE)));
}

std::int32_t CreateSpecialEffe(
    const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags, std::int32_t subtype) {
    const std::int32_t familySlot =
        AllocateEffeTask(EffeFamily::Special, count, subtype >= 0 && subtype <= 4 ? kSpecialTunes[static_cast<std::size_t>(subtype)] : nullptr);
    if (familySlot < 0)
        return -1;
    EffeTaskRecord *const task = g_specialTasks[static_cast<std::size_t>(familySlot)];
    task->position = position;
    task->range = range;
    task->velocity = {0.0f, 0.008f, 0.0f};
    task->velocityRange = {0.013f, 0.004f, 0.013f};
    task->probability = probability;
    task->width = 3.0f;
    task->height = 3.0f;
    task->flags = flags;
    task->subtype = subtype;
    const std::size_t type = static_cast<std::size_t>(subtype);
    if (subtype >= 0 && subtype <= 4) {
        task->particleScale = 0.1f;
        task->decayScale = subtype == 1 ? 0.91176468f : 1.2019608f;
        task->decayStep = 0.0060392157f;
    }
    if (subtype == 0 || (subtype >= 2 && subtype <= 4)) {
        g_specialDecayStep[type] = 0.0060392157f;
        g_specialDecayScale[type] = 1.2019608f;
        g_specialLifetime[type] = 0.1f;
    }
    if (subtype == 3 || subtype == 4) {
        const std::size_t slot = static_cast<std::size_t>(subtype - 3);
        g_specialMultiplier[slot] = 1.0f;
        g_specialRadial[slot] = 0.0f;
        g_specialDamping[slot] = 0.0f;
        g_specialFadeStep[slot] = 3;
    }
    if (subtype == 1) {
        task->velocity = {};
        task->velocityRange = {};
        task->width = 8.0f;
        task->height = 1.0f;
        g_specialIntensity = 32;
    }
    task->fftOwner = EFFE_AcquireFftOwner(flags, subtype == 1 ? 54.0f : 128.0f);
    g_effeEmitterRegistry[static_cast<std::size_t>(task->runtimeSlot)].fftIndex = task->fftOwner->fftIndex;
    const auto *const pixels = reinterpret_cast<const std::byte *>((reinterpret_cast<std::uintptr_t>(task->fftOwner->pixels) + 31U) & ~std::uintptr_t{31});
    SyncEffeTask(EffeFamily::Special, *task);
    AttachEffeTexture(task->runtimeSlot, CreateEffeTexture(16, 16, pixels), pixels, true, 16, 16);
    SetEffeActive(*task, false);
    EnsureEffeUpdateTask();
    return familySlot;
}

std::int32_t CreateSakuraEffe(const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags) {
    const auto free = std::find(g_sakuraTasks.begin(), g_sakuraTasks.end(), nullptr);
    if (count < 0 || free == g_sakuraTasks.end())
        return -1;
    auto *const task = static_cast<EffeTaskRecord *>(HLib_AllocHeapMemBlock(sizeof(EffeTaskRecord), static_cast<std::int32_t>(SHEN_NAME(EFPT))));
    if (!task)
        return -1;
    *task = {};
    task->particleCount = count;
    *free = task;
    task->particles = static_cast<EffeParticle *>(
        HLib_AllocHeapMemBlock(static_cast<std::int64_t>(count) * sizeof(EffeParticle), static_cast<std::int32_t>(SHEN_NAME(TRCR))));
    if (!task->particles) {
        *free = nullptr;
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task), 0);
        return -1;
    }
    const std::int32_t runtimeSlot = EFFE_Related(0, -1, UpdateSakuraOwner, task, SHEN_NAME(TRCR));
    if (runtimeSlot < 0) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task->particles), 0);
        *free = nullptr;
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(task), 0);
        return -1;
    }
    task->runtimeSlot = runtimeSlot;
    task->fftOwner = nullptr;
    task->decayScale = 0.0f;
    task->decayStep = 0.0f;
    g_sakuraRuntimeOwners[runtimeSlot] = task;
    EFFE_FillParticleTemplate(task->particles, count);
    EffeRuntimeTexture &runtime = g_effeRuntime[runtimeSlot];
    ReleaseEffeTexture(runtimeSlot);
    runtime.flags &= ~kAlternateParticleRecordFlag;
    runtime.flags |= kTransparentParticlePassFlag | kActiveTextureFlag | 0x08000000U;
    runtime.particleCount = count;
    runtime.particles = task->particles;
    runtime.particleUpdate = SakuraSpawnParticle;
    task->flags = flags;
    task->particleScale = 1.0f;
    SetSakuraPositionRange(*task, position, range);
    SetSakuraVelocityRange(*task, {0.0f, -0.008f, 0.0f}, {0.002f, 0.001f, 0.002f});
    SetSakuraProbability(*task, probability);
    SetSakuraDimensions(*task, 0.03f, 0.04f);
    ResetEffeDescriptor(runtime);
    runtime.descriptor[0] |= 0x02000000U;
    runtime.descriptor[2] = ((runtime.descriptor[2] | 0x00100000U) & 0x00ffffffU) | 0x94000000U;
    runtime.pvrSource = g_sakuraPvr;
    if (g_sakuraPvr && g_sakuraPvrBytes) {


        std::uint32_t magic = 0;
        std::memcpy(&magic, g_sakuraPvr, (std::min)(g_sakuraPvrBytes, sizeof(magic)));
        const std::size_t offset = g_sakuraPvr[0] == std::byte{'D'} ? 0U : 4U;
        if (magic == 0x54525650U || g_sakuraPvrBytes > offset) {
            const std::uint32_t texture = magic == 0x54525650U ?
                CreatePvrEffeTexture(reinterpret_cast<const EffePvrHeader *>(g_sakuraPvr)) :
                d3t::CreateImageTextureFromMemory(g_sakuraPvr + offset, (std::min)(g_sakuraPvrBytes - offset, std::size_t{0x100000}), true);
            if (texture) {
                EffeTextureResource &resource = g_effeResources[runtimeSlot];
                resource.textureHandle = texture;
                runtime.resource = &resource;
                runtime.descriptor[0] = 0x8200000aU;
                runtime.descriptor[1] = 0x8a800000U;
                runtime.descriptor[2] = 0x941242d2U;
                runtime.descriptor[3] = texture;
                runtime.flags |= 0x10000000U;
                ++g_TextureRefCount;
            }
        }
    }
    EnsureEffeUpdateTask();


    if (EffeTaskRecord *inactive = SakuraTaskAt(runtimeSlot))
        SetSakuraActive(*inactive, false);
    return static_cast<std::int32_t>(free - g_sakuraTasks.begin());
}

std::int32_t CreateEfptEffe(const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags, float floor,
    const char *root, const char *filename) {
    EnsureEffeUpdateTask();
    const std::int32_t familySlot = AllocateEffeTask(EffeFamily::Efpt, count, EFPT_UpdateCallback);
    if (familySlot < 0)
        return -1;
    EffeTaskRecord *const task = g_efptTasks[static_cast<std::size_t>(familySlot)];
    task->position = position;
    task->range = range;
    task->velocity = {0.001f, 0.1f, 0.001f};
    task->velocityRange = {0.001f, 0.001f, 0.001f};
    task->probability = probability;
    task->width = 0.02f;
    task->height = 0.02f;
    task->particleScale = 1.0f;
    task->flags = flags;
    task->floor = floor;

    const EffePvrHeader *const pvr = root && filename ? FindEffePvr(ReadFile(root, filename, static_cast<std::int32_t>(SHEN_NAME(EFPT)))) : nullptr;
    SyncEffeTask(EffeFamily::Efpt, *task);
    std::array<std::uint32_t, 256> defaultPixels{};
    defaultPixels.fill(0x7fffffffU);
    AttachEffeTexture(
        task->runtimeSlot, pvr ? CreatePvrEffeTexture(pvr) : CreateEffeTexture(16, 16, defaultPixels.data()), pvr, false, pvr ? 32 : 16, pvr ? 32 : 16);
    SetEffeActive(*task, false);
    for (std::int32_t index = 0; index < task->particleCount; ++index)
        task->particles[static_cast<std::size_t>(index)].attribute = reinterpret_cast<std::intptr_t>(&task->floor);
    return familySlot;
}

std::int32_t CreateGenericEffe(const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags, float floor,
    const char *root, const char *filename) {
    EnsureEffeUpdateTask();
    const std::int32_t familySlot = AllocateEffeTask(EffeFamily::Generic, count, nullptr);
    if (familySlot < 0)
        return -1;
    EffeTaskRecord *const task = g_genericTasks[static_cast<std::size_t>(familySlot)];
    task->position = position;
    task->range = range;
    task->velocity = {0.006f, -0.015f, 0.006f};
    task->velocityRange = {0.012f, 0.001f, 0.012f};
    task->probability = probability;
    task->width = 0.04f;
    task->height = 0.08f;
    task->particleScale = 1.0f;
    task->flags = flags;
    g_genericFloor = floor;
    const std::uint32_t genericSubtype = (flags >> 8U) & 0xffU;
    if (genericSubtype >= 1U && genericSubtype <= 3U)
        g_genericAnchors[genericSubtype] = position;
    const EffePvrHeader *const pvr = root && filename ? FindEffePvr(ReadFile(root, filename, static_cast<std::int32_t>(SHEN_NAME(EFPT)))) : nullptr;
    SyncEffeTask(EffeFamily::Generic, *task);
    AttachEffeTexture(task->runtimeSlot, CreatePvrEffeTexture(pvr), pvr, false, 16, 32);
    SetEffeActive(*task, false);
    return familySlot;
}

void SetEffePositionRange(EffeTaskRecord &task, const Vector3 &position, const Vector3 &range) {
    task.position = position;
    task.range = range;
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)];
    runtime.positionMin = position;
    runtime.positionRange = range;
}

void SetEffeVelocityRange(EffeTaskRecord &task, const Vector3 &velocity, const Vector3 &range) {
    task.velocity = velocity;
    task.velocityRange = range;
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)];
    runtime.velocityMin = velocity;
    runtime.velocityRange = range;
}

void SetEffeDimensions(EffeTaskRecord &task, float width, float height) {
    task.width = width;
    task.height = height;
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)];
    const float scale = (runtime.flags & 0x20000000U) ? 1.0f : 0.5f;
    runtime.halfWidth = width * scale;
    runtime.halfHeight = height * scale;
}

void SetEffeProbability(EffeTaskRecord &task, float probability) {
    task.probability = probability;
    g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)].spawnProbabilityQ15 = static_cast<std::int32_t>(static_cast<double>(probability) * 32767.0);
}

void TranslateEffe(EffeTaskRecord &task, const Vector3 &amount) {
    for (std::int32_t index = 0; task.particles && index < task.particleCount; ++index) {
        EffeParticle &particle = task.particles[static_cast<std::size_t>(index)];
        particle.position.x += amount.x;
        particle.position.y += amount.y;
        particle.position.z += amount.z;
    }
}

} // namespace shenmue::s1::effe_detail
