#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {




void EffeTuneSpecialEffect(EffeTaskRecord &task, std::int32_t subtype, double sizeScale, double velocityScale) {
    const auto *const slot1 = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[1]);
    const auto *const slot2 = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[2]);
    const auto *const slot3 = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[3]);
    const auto type = static_cast<std::size_t>(subtype);
    const float step = slot3[0x0c];
    const float scale = slot3[0x0d];
    EffeFftData &fft = *g_effeFftData[static_cast<std::size_t>(task.fftOwner->fftIndex)];
    std::array<float, 64> table{};
    std::memcpy(fft.falloff.data(),
        ApplyLogClampVectorScaling(static_cast<float>(static_cast<double>(scale) * 100.0 / 255.0),
            PrecomputeLogTable(static_cast<float>(static_cast<double>(step) * 10.0 / 255.0 - 2.0), table.data())),
        sizeof(fft.falloff));
    task.decayStep = static_cast<float>(static_cast<double>(step) / 255.0 * 0.02 - 0.004);
    task.decayScale = static_cast<float>(static_cast<double>(scale) / 255.0 + 0.7);
    if (subtype != 1) {
        g_specialDecayStep[type] = task.decayStep;
        g_specialDecayScale[type] = task.decayScale;
    } else {
        g_specialIntensity = slot1[0x11];
    }
    if (subtype != 1 && subtype != 2 && (g_inputState.slots[2].held & 0x8001U) == 0x8001U) {
        fft.phaseRateScale = static_cast<float>((slot2[0x11] - 128.0) / 255.0 + (slot2[0x11] - 128.0) / 255.0);
        NormalizeAndScaleFFTInput(fft.phaseRates.data(), fft.phaseRateScale);
    }
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)];
    runtime.colour = slot2[0x0e] | (static_cast<std::uint32_t>(slot2[0x0d]) << 8U) | (static_cast<std::uint32_t>(slot2[0x0c]) << 16U);
    for (std::int32_t index = 0; index < task.particleCount; ++index) {
        EffeParticle &particle = task.particles[static_cast<std::size_t>(index)];
        particle.colour = (particle.colour & 0xff000000U) | runtime.colour;
    }
    task.height = static_cast<float>(slot2[0x0f] / 255.0 * sizeScale);
    task.width = static_cast<float>(slot2[0x10] / 255.0 * sizeScale);
    const bool full = (runtime.flags & 0x20000000U) != 0;
    runtime.halfHeight = full ? task.height : task.height * 0.5f;
    runtime.halfWidth = full ? task.width : task.width * 0.5f;
    const bool centred = subtype == 3 || subtype == 4;
    task.velocity = {static_cast<float>(centred ? (slot3[0x0e] - 127.0) / 255.0 * 0.1 : slot3[0x0e] / 255.0 * velocityScale),
        static_cast<float>(centred ? (slot3[0x0f] - 127.0) / 255.0 * 0.1 : slot3[0x0f] / 255.0 * velocityScale),
        static_cast<float>(centred ? (slot3[0x10] - 127.0) / 255.0 * 0.1 : slot3[0x10] / 255.0 * velocityScale)};
    task.velocityRange = {static_cast<float>(slot1[0x0c] / 255.0 * velocityScale), static_cast<float>(slot1[0x0d] / 255.0 * velocityScale),
        static_cast<float>(slot1[0x0e] / 255.0 * velocityScale)};
    runtime.velocityMin = task.velocity;
    runtime.velocityRange = task.velocityRange;
    task.particleScale = static_cast<float>(slot3[0x11] / 255.0 * 0.5);
    if (subtype != 1)
        g_specialLifetime[type] = task.particleScale;
    const std::uint16_t held = g_inputState.slots[1].held;
    if (!centred || (held & 0x200U) == 0) {
        task.probability = static_cast<float>(slot1[0x0f] / 255.0 * 0.1);
        runtime.spawnProbabilityQ15 = static_cast<std::int32_t>(static_cast<double>(task.probability) * 32767.0);
    }
    if (!centred)
        return;
    const std::size_t index = type - 3;
    if ((held & 0x8000U) != 0) {
        g_specialFadeStep[index] = slot1[0x10] / 3;
        g_specialMultiplier[index] = static_cast<float>(slot1[0x11] / 255.0 * 10.0);
    }
    if ((held & 0x2000U) != 0) {
        g_specialRadial[index] = static_cast<float>(slot1[0x10] / 255.0);
        g_specialDamping[index] = static_cast<float>(slot1[0x11] / 255.0);
    }
}

void Special0Tune(void *argument) {
    EffeTaskRecord &task = *static_cast<EffeTaskRecord *>(argument);
    if ((task.flags & 1U) != 0)
        EffeTuneSpecialEffect(task, 0, 10.0, 0.1);
}

void Special1Tune(void *argument) {
    EffeTaskRecord &task = *static_cast<EffeTaskRecord *>(argument);
    if ((task.flags & 1U) != 0)
        EffeTuneSpecialEffect(task, 1, 10.0, 0.1);
}

void Special2Tune(void *argument) {
    EffeTaskRecord &task = *static_cast<EffeTaskRecord *>(argument);
    if ((task.flags & 1U) != 0)
        EffeTuneSpecialEffect(task, 2, 0.2, 0.003);
}

void Special3Tune(void *argument) {
    EffeTaskRecord &task = *static_cast<EffeTaskRecord *>(argument);
    if ((task.flags & 1U) != 0)
        EffeTuneSpecialEffect(task, 3, 10.0, 0.1);
}

void Special4Tune(void *argument) {
    EffeTaskRecord &task = *static_cast<EffeTaskRecord *>(argument);
    if ((task.flags & 1U) != 0)
        EffeTuneSpecialEffect(task, 4, 10.0, 0.1);
}

void EFPT_UpdateCallback(void *argument) {
    EffeTaskRecord &task = *static_cast<EffeTaskRecord *>(argument);
    if ((task.flags & 1U) == 0)
        return;
    const auto *const slot1 = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[1]);
    const auto *const slot2 = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[2]);
    task.velocity = {static_cast<float>(slot1[0x0c] / 255.0 * 0.2 - 0.1), static_cast<float>(slot1[0x0d] / 255.0 * 0.2 - 0.1),
        static_cast<float>(slot1[0x0e] / 255.0 * 0.2 - 0.1)};
    task.velocityRange = {static_cast<float>(slot2[0x0f] / 255.0 * 0.2 - 0.1), static_cast<float>(slot2[0x10] / 255.0 * 0.2 - 0.1),
        static_cast<float>(slot2[0x11] / 255.0 * 0.2 - 0.1)};
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(task.runtimeSlot)];
    runtime.velocityMin = task.velocity;
    runtime.velocityRange = task.velocityRange;
    if ((g_inputState.slots[1].held & 0x800U) != 0) {
        task.probability = static_cast<float>(slot1[0x0f] / 255.0 * 0.1);
        runtime.spawnProbabilityQ15 = static_cast<std::int32_t>(static_cast<double>(task.probability) * 32767.0);
    }
    const auto size = static_cast<float>(slot1[0x10] / 255.0 * 0.1);
    task.width = size;
    task.height = size;
    const float half = (runtime.flags & 0x20000000U) != 0 ? size : size * 0.5f;
    runtime.halfWidth = half;
    runtime.halfHeight = half;
    runtime.colour = slot2[0x0e] | (static_cast<std::uint32_t>(slot2[0x0d]) << 8U) | (static_cast<std::uint32_t>(slot2[0x0c]) << 16U) |
        (static_cast<std::uint32_t>(slot1[0x11]) << 24U);
    for (std::int32_t index = 0; index < task.particleCount; ++index)
        task.particles[static_cast<std::size_t>(index)].colour = runtime.colour;
}


void EFFE_NoOpParticle(EffeParticle *) {
}

// EFFE_InitializeParticleSystem's template record: idle, white, scale 1.
void EFFE_FillParticleTemplate(EffeParticle *particles, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index)
        particles[index] = {.update = sh4_parts_draw, .colour = 0xffffffffU, .scale = 1.0f};
}

std::uint64_t ParticleSpawnThreshold(float probability) {
    std::uint64_t high = 0;
    if (probability >= 0x1p63f) {
        probability -= 0x1p63f;
        if (probability < 0x1p63f)
            high = UINT64_C(0x8000000000000000);
    }
    const std::int64_t low = probability >= -0x1p63f && probability < 0x1p63f ? static_cast<std::int64_t>(probability) : INT64_MIN;
    return high + static_cast<std::uint64_t>(low);
}

void EFFE_RetireParticle(EffeParticle *particle) {
    particle->flags = 0;
    particle->update = sh4_parts_draw;
}

float EffeFoldedCosine(std::int64_t angle) {
    const auto low = static_cast<std::uint16_t>(angle);
    const std::uint32_t folded = low <= 0x8000U ? low : 0x10000U - low;
    return sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
}

void EffeSetAlpha(EffeParticle *particle, std::int32_t alpha) {
    particle->colour = (particle->colour & 0x00ffffffU) | (static_cast<std::uint32_t>(alpha) << 24U);
}

float &EffeRadialX(EffeParticle *particle) {
    return *reinterpret_cast<float *>(&particle->attribute);
}

float &EffeRadialZ(EffeParticle *particle) {
    return *reinterpret_cast<float *>(&particle->phaseStep1);
}

// ---- Sakura (EnqueueEFFE_0) ----
void SakuraGroundedParticle(EffeParticle *particle) {
    if (++particle->attribute > 180)
        EFFE_RetireParticle(particle);
}

void SakuraFallingParticle(EffeParticle *particle) {
    particle->phase1 += particle->phaseStep0;
    particle->phase2 += particle->phaseStep1;
    const float y = particle->position.y;
    if (0.005 > static_cast<double>(y)) {
        particle->position.y = 0.001f;
        particle->velocity = {};
        particle->attribute = 0;
        particle->phase1 = 0;
        particle->update = SakuraGroundedParticle;
        particle->phase0 = 0x4000;
        return;
    }
    particle->position.x = particle->velocity.x + particle->position.x;
    particle->position.y = y + particle->velocity.y;
    particle->position.z = particle->velocity.z + particle->position.z;
    particle->velocity.x = particle->velocity.x + g_sakuraRotation;
}

void SakuraSpawnParticle(EffeParticle *particle) {
    const std::uint32_t first = LcgStep(g_EnvPrngSeed);
    particle->update = SakuraFallingParticle;
    particle->flags = 0x80000000U;
    particle->attribute = 512;
    particle->phaseStep0 = 512;
    particle->phaseStep1 = 512;
    particle->phase0 = 2 * static_cast<std::int64_t>((first >> 16U) & 0x7fffU);
    const std::uint32_t second = LcgStep(first);
    particle->phase1 = 2 * static_cast<std::int64_t>((second >> 16U) & 0x7fffU);
    g_EnvPrngSeed = LcgStep(second);
    particle->phase2 = 2 * static_cast<std::int64_t>((g_EnvPrngSeed >> 16U) & 0x7fffU);
    particle->colour = g_sakuraColour;
}

// ---- EFPT (EnqueueEFPT); attribute points at the task floor ----
void EfptFall(EffeParticle *particle) {
    const float floor = **reinterpret_cast<float *const *>(&particle->attribute);
    if (floor > particle->position.y) {
        EFFE_RetireParticle(particle);
        return;
    }
    const float vy = particle->velocity.y;
    particle->position.y = vy + particle->position.y;
    particle->position.x = particle->velocity.x + particle->position.x;
    particle->position.z = particle->velocity.z + particle->position.z;
    particle->velocity.y = static_cast<float>(static_cast<double>(vy) - 0.005);
}

void EfptFallParticle(EffeParticle *particle) {
    EfptFall(particle);
}

void EfptFallFadeParticle(EffeParticle *particle) {
    EfptFall(particle);
    const double scale = particle->scale;
    if (scale > 0.001)
        particle->scale = static_cast<float>(scale > 0.1 ? scale - 0.01 : scale - 0.001);
}

void EfptFallShrinkParticle(EffeParticle *particle) {
    EfptFall(particle);
    if (static_cast<double>(particle->scale) > 0.2)
        particle->scale = static_cast<float>(static_cast<double>(particle->scale) - 0.005);
}

void EfptSpawnFall(EffeParticle *particle) {
    particle->flags = 0x80000000U;
    particle->update = EfptFallParticle;
}

void EfptSpawnFallFade(EffeParticle *particle) {
    particle->flags = 0x80000000U;
    particle->update = EfptFallFadeParticle;
}

void EfptSpawnRing(EffeParticle *particle) {
    const std::uint32_t seed = LcgStep(g_EnvPrngSeed);
    particle->update = EfptFallFadeParticle;
    particle->flags = 0x80000000U;
    g_EnvPrngSeed = seed;
    const std::uint32_t angle = 2U * ((seed >> 16U) & 0x7fffU);
    const float cosine = EffeFoldedCosine(angle);
    particle->position.x = cosine * particle->velocity.x + particle->position.x;
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float speed = particle->velocity.z;
    particle->position.z = sine * particle->velocity.x + particle->position.z;
    particle->velocity.x = EffeFoldedCosine(angle) * speed;
    particle->velocity.z = speed * sine;
    particle->phaseStep1 = 0;
}

void EfptSpawnFallShrink(EffeParticle *particle) {
    particle->flags = 0x80000000U;
    particle->update = EfptFallShrinkParticle;
}

void EffeGenericPhases(EffeParticle *particle) {
    const std::uint32_t first = LcgStep(g_EnvPrngSeed);
    particle->phase0 = 2 * static_cast<std::int64_t>((first >> 16U) & 0x7fffU);
    const std::uint32_t second = LcgStep(first);
    particle->phase1 = 2 * static_cast<std::int64_t>((second >> 16U) & 0x7fffU);
    const std::uint32_t third = LcgStep(second);
    particle->phase2 = 2 * static_cast<std::int64_t>((third >> 16U) & 0x7fffU);
    g_EnvPrngSeed = LcgStep(third);
}

void GenericDriftParticle(EffeParticle *particle) {
    particle->phase1 += particle->phaseStep0;
    particle->phase2 += particle->phaseStep1;
    const float y = particle->position.y;
    if (g_genericFloor > y) {
        EFFE_RetireParticle(particle);
        return;
    }
    const float sine = sin_fixed16(static_cast<std::int16_t>(particle->phase1));
    float drop = sine * particle->velocity.y;
    if (drop > 0.0f)
        drop = -drop;
    const float vx = particle->velocity.x;
    const float vz = particle->velocity.z;
    if (0.016 > static_cast<double>(std::fabs(vz) + std::fabs(vx))) {
        if (static_cast<double>(drop) > -0.016)
            drop = -0.016000001f;
        particle->phase2 += 8 * particle->phaseStep1;
    }
    particle->position.y = y + drop;
    particle->position.x = vx * sine + particle->position.x;
    particle->position.z = vz * sine + particle->position.z;
}

void GenericSpawnDrift(EffeParticle *particle) {
    const std::uint32_t first = LcgStep(g_EnvPrngSeed);
    particle->update = GenericDriftParticle;
    particle->flags = 0x90000000U;
    particle->phase0 = 2 * static_cast<std::int64_t>((first >> 16U) & 0x7fffU);
    const std::uint32_t second = LcgStep(first);
    particle->phase1 = 2 * static_cast<std::int64_t>((second >> 16U) & 0x7fffU);
    g_EnvPrngSeed = LcgStep(second);
    particle->phase2 = 2 * static_cast<std::int64_t>((g_EnvPrngSeed >> 16U) & 0x7fffU);
    particle->phaseStep0 = 512;
    particle->phaseStep1 = 512;
}

// The |cos| term of the falling families: computed in double, negated when negative.
double GenericFallCosine(std::int64_t phase) {
    const double value = static_cast<double>(EffeFoldedCosine(phase)) * 0.015;
    return value >= 0.0 ? value : -value;
}

void GenericFallParticle(EffeParticle *particle) {
    particle->phase1 += particle->phaseStep0;
    particle->phase2 += particle->phaseStep1;
    if (g_genericFloor > particle->position.y) {
        EFFE_RetireParticle(particle);
        return;
    }
    particle->position.y =
        static_cast<float>(static_cast<double>(particle->velocity.y) - GenericFallCosine(particle->phase1) - 0.016 + static_cast<double>(particle->position.y));
    const double wobble = static_cast<double>(sin_fixed16(static_cast<std::int16_t>(particle->phase1))) * 0.01;
    const float x = static_cast<float>(static_cast<double>(particle->velocity.x) + wobble + static_cast<double>(particle->position.x));
    particle->position.x = x;
    const float z = static_cast<float>(static_cast<double>(particle->velocity.z) + wobble + static_cast<double>(particle->position.z));
    particle->position.z = z;
    const float dx = x - g_genericAnchors[1].x;
    const float dz = z - g_genericAnchors[1].z;
    if (dx * dx + dz * dz > 64.0f)
        EFFE_RetireParticle(particle);
}

void GenericSpawnFall(EffeParticle *particle) {
    particle->update = GenericFallParticle;
    particle->flags = 0x90000000U;
    particle->phaseStep1 = 512;
    EffeGenericPhases(particle);
    particle->phaseStep0 = static_cast<std::int64_t>((g_EnvPrngSeed >> 20U) & 0x7ffU) + 256;
}

Vector4 GenericBezier(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2, const Vector4 &p3, float t) {
    const float inverse = 1.0f - t;
    const float w0 = inverse * inverse * inverse;
    const double w1 = static_cast<double>(inverse * t * inverse) * 3.0;
    const float w3 = t * t * t;
    const double w2 = static_cast<double>(inverse * (t * t)) * 3.0;
    return {static_cast<float>(static_cast<double>(w0 * p0.x) + p1.x * w1 + p2.x * w2 + static_cast<double>(w3 * p3.x)),
        static_cast<float>(p1.y * w1 + static_cast<double>(w0 * p0.y) + p2.y * w2 + static_cast<double>(w3 * p3.y)),
        static_cast<float>(p1.z * w1 + static_cast<double>(w0 * p0.z) + p2.z * w2 + static_cast<double>(w3 * p3.z)),
        static_cast<float>(p1.w * w1 + static_cast<double>(w0 * p0.w) + p2.w * w2 + static_cast<double>(w3 * p3.w))};
}


void GenericCurveParticle(EffeParticle *particle) {
    const float time = particle->velocity.y;
    if (1.0f > time || 2.0f > time) {
        const Vector4 point = 1.0f > time ?
            GenericBezier(kGenericCurve[0], kGenericCurve[1], kGenericCurve[2], kGenericCurve[3], time) :
            GenericBezier(kGenericCurve[3], kGenericCurve[4], kGenericCurve[5], kGenericCurve[6], static_cast<float>(static_cast<double>(time) - 1.0));
        const Vector3 &anchor = g_genericAnchors[2];
        const std::int64_t phase = particle->phase1;
        particle->position.x = static_cast<float>(
            static_cast<double>(sin_fixed16(static_cast<std::int16_t>(phase))) * static_cast<double>(point.w) + static_cast<double>(anchor.x + point.x));
        particle->position.y =
            static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(3 * (phase / 2)))) * (static_cast<double>(point.w) * 0.3) +
                static_cast<double>(anchor.y + point.y));
        particle->position.z = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(5 * (phase / 2)))) * static_cast<double>(point.w) +
            static_cast<double>(anchor.z + point.z));
    } else {
        EFFE_RetireParticle(particle);
    }
    particle->phase1 += particle->phaseStep0;
    particle->phase2 += particle->phaseStep1;
    particle->velocity.y = static_cast<float>(static_cast<double>(particle->velocity.y) + 0.02222222222222222);
}

void GenericSpawnCurve(EffeParticle *particle) {
    particle->update = GenericCurveParticle;
    particle->flags = 0x90000000U;
    particle->phaseStep1 = 512;
    EffeGenericPhases(particle);
    particle->phaseStep0 = static_cast<std::int64_t>((g_EnvPrngSeed >> 21U) & 0x3ffU) + 256;
    particle->velocity.y = 0.0f;
}

void GenericAttractParticle(EffeParticle *particle) {
    const float y = particle->position.y;
    if (y < -0.5f || y > 8.0f) {
        particle->update = sh4_parts_draw;
        particle->flags = 0;
        return;
    }
    const float dz = g_genericAnchors[3].z - particle->position.z;
    const float x = particle->position.x;
    const float dx = g_genericAnchors[3].x - x;
    float distance = dz * dz + dx * dx;
    if (0.01 > static_cast<double>(distance))
        distance = 0.0099999998f;
    particle->position.x = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(particle->phase1))) * 0.01 +
        static_cast<double>(particle->velocity.x) + static_cast<double>(x));
    particle->position.y = static_cast<float>(static_cast<double>(particle->velocity.y) - GenericFallCosine(particle->phase1) - 0.016 + static_cast<double>(y));
    const float normalX = dx / distance;
    const float normalZ = dz / distance;
    const float vz = particle->velocity.z;
    particle->position.z = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(particle->phase1))) * 0.01 + static_cast<double>(vz) +
        static_cast<double>(particle->position.z));
    const float vx = normalX * 0.014117647f - normalZ * 0.0019607844f + particle->velocity.x;
    float sine = sin_fixed16(static_cast<std::int16_t>(particle->phase1));
    if (sine < 0.0f)
        sine = -sine;
    particle->velocity.x = vx * 0.97843134f;
    particle->velocity.y = sine / distance * 0.077254899f;
    particle->velocity.z = (normalX * 0.0019607844f + normalZ * 0.014117647f + vz) * 0.97843134f;
    particle->phase1 += particle->phaseStep0;
    particle->phase2 += particle->phaseStep1;
}

void GenericSpawnAttract(EffeParticle *particle) {
    particle->update = GenericAttractParticle;
    particle->flags = 0x90000000U;
    particle->phaseStep1 = 512;
    EffeGenericPhases(particle);
    particle->phaseStep0 = static_cast<std::int64_t>((g_EnvPrngSeed >> 21U) & 0x3ffU) + 512;
}

// ---- Special (EnqueueSpecialEffect); per-subtype globals ----
void EffeSpecialIntegrate(EffeParticle *particle) {
    const float y = particle->velocity.y + particle->position.y;
    particle->position.x = particle->velocity.x + particle->position.x;
    const float z = particle->velocity.z + particle->position.z;
    particle->position.y = y;
    particle->position.z = z;
}

void Special0FadeParticle(EffeParticle *particle) {
    EffeSpecialIntegrate(particle);
    particle->phase1 += particle->phaseStep0;
    const float scale = g_specialDecayStep[0] + particle->scale;
    particle->scale = scale;
    const float value = 1.0f - scale * g_specialDecayScale[0];
    if (value < 0.0f)
        EFFE_RetireParticle(particle);
    EffeSetAlpha(particle, static_cast<std::int32_t>(value * 128.0f));
}

void Special0RiseParticle(EffeParticle *particle) {
    EffeSpecialIntegrate(particle);
    particle->phase1 += particle->phaseStep0;
    const float scale = g_specialDecayStep[0] + particle->scale;
    const std::int32_t rising = static_cast<std::int32_t>(particle->colour >> 24U) + 6;
    particle->colour &= 0x00ffffffU;
    particle->scale = scale;
    const double limit = static_cast<double>(1.0f - scale * g_specialDecayScale[0]) * 128.0;
    if (rising < limit) {
        particle->colour |= static_cast<std::uint32_t>(rising) << 24U;
        return;
    }
    particle->colour |= static_cast<std::uint32_t>(static_cast<std::int32_t>(limit)) << 24U;
    particle->update = Special0FadeParticle;
}

void Special0Spawn(EffeParticle *particle) {
    particle->update = Special0RiseParticle;
    particle->scale = g_specialLifetime[0];
    particle->flags = 0x80000000U;
    particle->phaseStep0 = 80;
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    particle->phase1 = (g_EnvPrngSeed >> 16U) & 0x7fffU;
}

void Special1DriftParticle(EffeParticle *particle) {
    const float x = particle->velocity.x + particle->position.x;
    EffeSetAlpha(particle, static_cast<std::int32_t>(g_specialIntensity));
    const float y = particle->velocity.y + particle->position.y;
    particle->position.x = x;
    const float z = particle->velocity.z + particle->position.z;
    particle->position.y = y;
    particle->position.z = z;
}

void Special1Spawn(EffeParticle *particle) {
    particle->flags = 0x80000000U;
    particle->update = Special1DriftParticle;
    particle->scale = 1.0f;
}

void Special2FadeParticle(EffeParticle *particle) {
    EffeSpecialIntegrate(particle);
    particle->phase1 += particle->phaseStep0;
    const float scale = g_specialDecayStep[2] + particle->scale;
    particle->scale = scale;
    const float value = static_cast<float>(1.0 - static_cast<double>(scale * g_specialDecayScale[2]));
    if (value < 0.0f)
        EFFE_RetireParticle(particle);
    EffeSetAlpha(particle, static_cast<std::int32_t>(static_cast<double>(static_cast<float>(static_cast<double>(value) * 0.8)) * 128.0));
}

void Special2SwirlParticle(EffeParticle *particle) {
    const std::int32_t alpha = static_cast<std::int32_t>(particle->colour >> 24U);
    const float angle = sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(2 * alpha) * 8192.0f / 45.0f)));
    const float vy = particle->velocity.y;
    particle->position.x = angle * particle->velocity.x + particle->position.x;
    particle->position.z = angle * particle->velocity.z + particle->position.z;
    particle->position.y = vy + particle->position.y;
    std::uint32_t next = static_cast<std::uint32_t>(alpha) + 1U;
    if (next >= 0x7fU) {
        next = 127;
        const std::uint32_t first = LcgStep(g_EnvPrngSeed);
        particle->velocity.x = static_cast<float>(static_cast<double>((first >> 16U) & 0x7fffU) * 0x1p-15 * 0.002 - 0.001);
        const std::uint32_t second = LcgStep(first);
        g_EnvPrngSeed = LcgStep(second);
        particle->velocity.z = static_cast<float>(static_cast<double>((second >> 16U) & 0x7fffU) * 0x1p-15 * 0.002 - 0.001);
        particle->update = Special2FadeParticle;
        particle->velocity.y = static_cast<float>(static_cast<double>(static_cast<float>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0x1p-15f * vy) + 0.001);
    }
    particle->colour = (particle->colour & 0x00ffffffU) | (next << 24U);
}

void Special2Spawn(EffeParticle *particle) {
    const std::uint32_t angle = g_specialPhase;
    particle->scale = g_specialLifetime[2];
    particle->update = Special2SwirlParticle;
    particle->flags = 0x80000000U;
    particle->phase1 = 0;
    particle->phaseStep0 = 80;
    particle->velocity.x = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(angle))) * 0.0004);
    const double cosine = EffeFoldedCosine(angle);
    g_specialPhase = angle + 546;
    const float vz = static_cast<float>(cosine * 0.0004);
    particle->velocity.z = vz;
    if (particle->velocity.x < 0.0f)
        particle->phaseStep0 = -particle->phaseStep0;
    if (vz < 0.0f)
        particle->phaseStep0 = -particle->phaseStep0;
}

// Specials 3 and 4 share one shape over their own globals (slot 0 = subtype 3, slot 1 = subtype 4).
void EffeSpecialRadialMove(EffeParticle *particle, std::size_t slot) {
    const float damping = g_specialDamping[slot];
    particle->position.x = particle->velocity.x + EffeRadialX(particle) + particle->position.x;
    particle->position.y = particle->velocity.y + particle->position.y;
    particle->position.z = particle->velocity.z + EffeRadialZ(particle) + particle->position.z;
    EffeRadialX(particle) = damping * EffeRadialX(particle);
    particle->phase1 += particle->phaseStep0;
    EffeRadialZ(particle) = damping * EffeRadialZ(particle);
}

void EffeSpecialRadialFade(EffeParticle *particle, std::size_t slot) {
    EffeSpecialRadialMove(particle, slot);
    const std::size_t subtype = slot + 3;
    const float scale = g_specialDecayStep[subtype] + particle->scale;
    particle->scale = scale;
    const float value = static_cast<float>(1.0 - static_cast<double>(scale * g_specialDecayScale[subtype] * g_specialMultiplier[slot]));
    if (static_cast<double>(value) < 0.1)
        EFFE_RetireParticle(particle);
    EffeSetAlpha(particle, static_cast<std::int32_t>(static_cast<double>(value) * 128.0));
}

void EffeSpecialRadialRise(EffeParticle *particle, std::size_t slot, EffeParticleUpdate fade) {
    EffeSpecialRadialMove(particle, slot);
    const std::size_t subtype = slot + 3;
    const float scale = g_specialDecayStep[subtype] + particle->scale;
    particle->scale = scale;
    const float value = static_cast<float>(1.0 - static_cast<double>(scale * g_specialDecayScale[subtype] * g_specialMultiplier[slot]));
    if (0.1 > static_cast<double>(value)) {
        particle->flags = 0;
        particle->update = sh4_parts_draw;
        return;
    }
    const std::int32_t rising = g_specialFadeStep[slot] + 1 + static_cast<std::int32_t>(particle->colour >> 24U);
    particle->colour &= 0x00ffffffU;
    const double limit = static_cast<double>(value) * 128.0;
    if (static_cast<double>(rising) >= limit) {
        particle->colour |= static_cast<std::uint32_t>(static_cast<std::int32_t>(limit)) << 24U;
        particle->update = fade;
        return;
    }
    particle->colour |= static_cast<std::uint32_t>(rising) << 24U;
}

void EffeSpecialRadialSpawn(EffeParticle *particle, std::size_t slot, EffeParticleUpdate rise) {
    const std::uint32_t first = LcgStep(g_EnvPrngSeed);
    const std::uint32_t angle = 2U * ((first >> 16U) & 0x7fffU);
    const float radial = g_specialRadial[slot];
    EffeRadialX(particle) = EffeFoldedCosine(angle) * radial;
    EffeRadialZ(particle) = sin_fixed16(static_cast<std::int16_t>(angle)) * radial;
    particle->update = rise;
    particle->scale = g_specialLifetime[slot + 3];
    particle->flags = 0x80000000U;
    particle->phaseStep0 = 80;
    g_EnvPrngSeed = LcgStep(first);
    particle->phase1 = (g_EnvPrngSeed >> 16U) & 0x7fffU;
}

void Special3FadeParticle(EffeParticle *particle) {
    EffeSpecialRadialFade(particle, 0);
}

void Special3RiseParticle(EffeParticle *particle) {
    EffeSpecialRadialRise(particle, 0, Special3FadeParticle);
}

void Special3Spawn(EffeParticle *particle) {
    EffeSpecialRadialSpawn(particle, 0, Special3RiseParticle);
}

void Special4FadeParticle(EffeParticle *particle) {
    EffeSpecialRadialFade(particle, 1);
}

void Special4RiseParticle(EffeParticle *particle) {
    EffeSpecialRadialRise(particle, 1, Special4FadeParticle);
}

void Special4Spawn(EffeParticle *particle) {
    EffeSpecialRadialSpawn(particle, 1, Special4RiseParticle);
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

// sh4_parts_draw: a separate signed-32 seed and six-tap offsets, not the environment LCG the spawn callbacks use.
void sh4_parts_draw(EffeParticle *particle) {
    const EffeRuntimeTexture &runtime = *g_effeCurrentSystem;
    constexpr std::uint64_t multiplier = 1103515245U;
    std::uint64_t random = static_cast<std::uint64_t>(static_cast<std::int64_t>(g_particleSpawnSeed)) * multiplier;
    float probability = static_cast<float>(static_cast<std::uint64_t>(static_cast<std::int64_t>(runtime.spawnProbabilityQ15)));
    if (g_bEnableFrameTimeSmoothing)
        probability = (probability * 30.0f) * fFrameTimeMs;
    if (((random >> 16U) & 0x7fffU) < ParticleSpawnThreshold(probability)) {
        particle->update = runtime.particleUpdate;
        const auto offset = [&random]() {
            random *= multiplier;
            std::uint32_t sum = static_cast<std::uint32_t>((random >> 16U) & 0x7fffU);
            for (std::uint32_t shift = 1; shift <= 5; ++shift)
                sum += static_cast<std::uint32_t>((random >> shift) & 0x7fffU);
            return static_cast<float>(sum) * 0x1.fffff8p-16f - 3.0f;
        };
        particle->position = {
            offset() * runtime.positionRange.x + runtime.positionMin.x,
            offset() * runtime.positionRange.y + runtime.positionMin.y,
            offset() * runtime.positionRange.z + runtime.positionMin.z,
        };
        particle->velocity = {
            offset() * runtime.velocityRange.x + runtime.velocityMin.x,
            offset() * runtime.velocityRange.y + runtime.velocityMin.y,
            offset() * runtime.velocityRange.z + runtime.velocityMin.z,
        };
        if ((runtime.flags & kAlternateParticleRecordFlag) == 0)
            particle->colour = runtime.colour;
    }
    g_particleSpawnSeed = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(random));
}

} // namespace shenmue::s1
