#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

std::uint8_t VFFightGroundEffectHourBand() {
    const std::uint8_t hour = g_currentDateTime.hours;
    if (hour < 7)
        return 0;
    if (hour < 11)
        return 1;
    if (hour < 15)
        return 2;
    if (hour < 19)
        return 3;
    return 0;
}

std::uint32_t VFFightGroundEffectColour(const VFActor *actor) {
    std::uint8_t quadrant = 2;
    if (actor) {
        const Vector3 actorPosition = actor->character ? GetPositionForEntity(actor->character->entity.node) : Vector3{};
        quadrant = VFMA_ResolveEffectQuadrant(actorPosition.x, actorPosition.z, actor->cachedQuadrant);
    }
    std::uint8_t colourIndex = 0;
    const std::size_t bandOffset = static_cast<std::size_t>(VFFightGroundEffectHourBand()) * 3;
    if (quadrant == 0xff) {
        colourIndex = kVFFightGroundEffectColourByTimeAndArea[bandOffset];
    } else if (VFFightStage *const stage = g_pFightStage; stage && stage->areaMasks && quadrant < stage->areaMaskCount) {
        const std::uint8_t areaClass = stage->areaMasks[quadrant].unknown02;
        if (areaClass <= 1) {
            colourIndex = kVFFightGroundEffectColourByTimeAndArea[bandOffset + static_cast<std::size_t>(areaClass) + 1];
        }
    }
    const Vector3 colour = kVFFightGroundEffectColours[static_cast<std::size_t>(colourIndex)];
    const auto channel = [](float value) { return static_cast<std::uint32_t>(static_cast<std::uint8_t>(value * 16.0f)); };
    return (channel(colour.x) << 16U) | (channel(colour.y) << 8U) | channel(colour.z);
}

Vector3 VFFightGroundEffectPosition(std::uint8_t type, VFActor *actor, Character *character) {
    Vector3 local{};
    switch (type) {
    case 0:
        local.x = actor->rightWristPosition.x;
        local.z = actor->rightWristPosition.z;
        break;
    case 1:
        local.x = actor->leftWristPosition.x;
        local.z = actor->leftWristPosition.z;
        break;
    case 2:
    case 3:
    case 4:
        local.x = actor->contactCurrentPosition.x;
        local.z = actor->contactCurrentPosition.z;
        break;
    case 5:
        if (character)
            local = GetPositionForEntity(character->entity.node);
        break;
    default:
        break;
    }

    if (actor)
        character = actor->character;
    (void)SelectCharacterMapQuery(character);
    if (std::fabs(local.y) < 1.1920929e-07f)
        local.y = 0.0f;
    local.y = static_cast<float>(static_cast<double>(local.y) + 1.0);
    float ground = mapUpdateIntersectionStateAtLocalPoint(local);
    if (character) {
        for (CharacterNode *node = character->entity.node.child; node; node = node->child)
            ground += node->position.y;
    }
    local.y = static_cast<float>(static_cast<double>(ground) + 0.05);
    return local;
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {


// @verified - p
void EFFE_InitVFFightGroundEffects() {
    for (VFFightGroundEffectRecord &record : g_vfFightGroundEffects)
        record.taskSlot = -1;
    for (std::size_t index = 0; index < g_vfFightGroundTaskSlots.size(); ++index)
        g_vfFightGroundTaskSlots[index] = CreateSpecialEffe({}, {}, 40, 0.0f, 0, 4);
    g_vfFightGroundEffectCount = 0;
}


// @verified - p
void EFFE_FreeVFFightGroundEffects() {
    for (const std::int32_t taskSlot : g_vfFightGroundTaskSlots) {
        const std::uint8_t slot = static_cast<std::uint8_t>(taskSlot);
        if (EffeTaskRecord *const task = TaskAt(EffeFamily::Special, slot)) {
            const std::size_t runtimeSlot = static_cast<std::size_t>(task->runtimeSlot);
            if (g_effeEmitterRegistry[runtimeSlot].state == 2) {
                g_effeEmitterRegistry[runtimeSlot].state = 1;
                g_effeRuntime[runtimeSlot].flags &= ~kActiveTextureFlag;
            }
            EffeFftOwner *const owner = task->fftOwner;
            if (owner && static_cast<std::uint32_t>(task->subtype) <= 4U) {
                owner->activeMask &= static_cast<std::uint16_t>(~(1U << static_cast<std::uint32_t>(task->runtimeSlot)));
                const std::size_t fftIndex = static_cast<std::size_t>(owner->fftIndex);
                if (!owner->activeMask && g_effeFftRegistry[fftIndex].state == 2) {
                    g_effeFftRegistry[fftIndex].state = 1;
                    g_effeFftData[fftIndex]->flags &= ~kActiveTextureFlag;
                }
            }
            task->flags &= ~2U;
        }
        FreeEffeTask(EffeFamily::Special, slot);
    }
}


// @verified - p
void EFFE_FadeVFFightGroundEffects() {
    std::uint8_t count = g_vfFightGroundEffectCount;
    for (VFFightGroundEffectRecord &record : g_vfFightGroundEffects) {
        if (static_cast<std::uint16_t>(record.taskSlot) == 0xffffU)
            continue;
        float probability = static_cast<float>(static_cast<double>(record.probability) - 0.025);
        if (probability < 0.0f)
            probability = 0.0f;
        record.probability = probability;
        if (--record.framesRemaining == 0) {
            --count;
            record.taskSlot = -1;
            continue;
        }
        if (EffeTaskRecord *const task = TaskAt(EffeFamily::Special, static_cast<std::uint8_t>(record.taskSlot))) {
            task->probability = probability;
            EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task->runtimeSlot)];
            runtime.spawnProbabilityQ15 = static_cast<std::int32_t>(static_cast<double>(probability) * 32767.0);
        }
    }
    g_vfFightGroundEffectCount = count;
}


void EFFE_SpawnVFFightGroundEffect(std::uint8_t type, VFActor *actor, Character *stageCharacter) {
    if (type > 5 || (type < 5 && !actor))
        return;
    if (g_areaId != SHEN_NAME(MA00)) {
        if (g_areaId == SHEN_NAME(JHD0))
            return;
        if (g_areaId != SHEN_NAME(M3FB) && g_areaId != SHEN_NAME(MGCT)) {
            const VFMAData *const game = g_areaId == SHEN_NAME(MFSY) ? vfMainGetCurrent() : nullptr;
            if ((!game || game->battleType != 4) && static_cast<double>(WEXE_GetRainIntensity()) > 0.1)
                return;
        }
    }
    std::size_t index = 0;
    while (index < g_vfFightGroundEffects.size() && g_vfFightGroundEffects[index].taskSlot != -1)
        ++index;
    if (index == g_vfFightGroundEffects.size())
        return;
    const std::int32_t slot = g_vfFightGroundTaskSlots[index];
    EffeTaskRecord *const task = TaskAt(EffeFamily::Special, slot);
    if (!task)
        return;

    const VFFightGroundEffectConfig &config = kVFFightGroundEffectConfigs[type];
    task->position = VFFightGroundEffectPosition(type, type == 5 ? nullptr : actor, stageCharacter);
    task->range = {0.1f, 0.0f, 0.1f};
    task->probability = config.probability;
    task->particleCount = config.particleCount;
    task->flags = 0;
    task->subtype = 4;
    task->particleScale = 0.1f;
    task->decayScale = 1.2019608f;
    task->decayStep = 0.0060392157f;
    g_specialLifetime[4] = 0.1f;
    g_specialDecayScale[4] = 1.2019608f;
    g_specialDecayStep[4] = 0.0060392157f;
    if (task->fftOwner) {
        EffeFftData &fft = *g_effeFftData[static_cast<std::size_t>(task->fftOwner->fftIndex)];
        std::array<float, 64> table;
        std::memcpy(fft.falloff.data(), ApplyLogClampVectorScaling(5.0f, PrecomputeLogTable(2.8f, table.data())), sizeof(fft.falloff));
        fft.phaseRateScale = 0.3f;
        NormalizeAndScaleFFTInput(fft.phaseRates.data(), 0.3f);
    }
    ++g_vfFightGroundEffectCount;

    g_specialMultiplier[1] = 2.0f;
    g_specialFadeStep[1] = 16;
    g_specialRadial[1] = 0.0f;
    g_specialDamping[1] = g_runStartSpeed;
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task->runtimeSlot)];
    runtime.descriptor[2] = ((runtime.descriptor[2] | 0x00100000U) & 0x00ffbfffU) | 0x94002000U;
    runtime.descriptor[0] |= 0x02000000U;
    runtime.flags |= kTransparentParticlePassFlag;
    const std::uint32_t colour = VFFightGroundEffectColour(type == 5 ? nullptr : actor);
    runtime.colour = colour;
    for (std::int32_t particle = 0; particle < task->particleCount; ++particle)
        task->particles[particle].colour = colour;

    if (type < 2) {
        task->velocity = {
            static_cast<float>(static_cast<double>(actor->forwardX) * 0.03), 0.0005f, static_cast<float>(static_cast<double>(actor->forwardZ) * 0.03)};
    } else if (type == 2) {
        task->velocity = {actor->forwardX * 0.03f, 0.0005f, actor->forwardZ * 0.03f};
    } else {
        task->velocity = {0.0f, 0.0005f, 0.0f};
    }
    task->velocityRange = {config.velocityRange, 0.0005f, config.velocityRange};
    task->width = 2.0f;
    task->height = 2.0f;
    SyncEffeTask(EffeFamily::Special, *task);
    const std::size_t runtimeSlot = static_cast<std::size_t>(task->runtimeSlot);
    if (g_effeEmitterRegistry[runtimeSlot].state == 1) {
        g_effeEmitterRegistry[runtimeSlot].state = 2;
        runtime.flags |= kActiveTextureFlag;
    }
    EffeFftOwner *const owner = task->fftOwner;
    if (owner && static_cast<std::uint32_t>(task->subtype) <= 4U) {
        const std::size_t fftIndex = static_cast<std::size_t>(owner->fftIndex);
        if (g_effeFftRegistry[fftIndex].state == 1) {
            g_effeFftRegistry[fftIndex].state = 2;
            g_effeFftData[fftIndex]->flags |= kActiveTextureFlag;
        }
        owner->activeMask |= static_cast<std::uint16_t>(1U << static_cast<std::uint32_t>(task->runtimeSlot));
    }
    task->flags |= 2U;
    VFFightGroundEffectRecord &record = g_vfFightGroundEffects[index];
    record.framesRemaining = config.variant;
    record.probability = config.probability;
    record.taskSlot = static_cast<std::int16_t>(slot);
}

} // namespace shenmue::s1
