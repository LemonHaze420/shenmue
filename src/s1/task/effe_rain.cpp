#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

void InitializeRainLayer(WeatherNodeData &data, float intensity, RainRandomFunction generator) {
    if (data.rain.pool.records)
        return;
    const float density = std::min(2.5f, intensity);
    const std::int32_t requested = static_cast<std::int32_t>(density * data.volume);
    if (requested <= 0)
        return;

    const std::int32_t count = std::min(requested, 1100);
    auto *const particles = static_cast<RainParticleNative *>(
        HLib_AllocHeapMemBlock(sizeof(RainParticleNative) * static_cast<std::size_t>(count), static_cast<std::int32_t>(SHEN_NAME(RINP))));
    if (!particles)
        return;
    auto *const fade = static_cast<std::byte *>(HLib_AllocHeapMemBlock(20, static_cast<std::int32_t>(SHEN_NAME(LFLT))));
    if (!fade) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(particles), 0);
        return;
    }

    std::memset(particles, 0, sizeof(RainParticleNative) * static_cast<std::size_t>(count));
    std::memset(fade, 0, 20);
    for (std::int32_t index = 0; index < count; ++index) {
        particles[index].flags = 0x40000000U;
        particles[index].random = generator;
    }
    data.rain.pool.records = particles;
    data.rain.pool.capacity = count;
    data.rain.pool.activeCount = 0;
    data.rain.pool.renderCount = 0;
    data.rain.pool.fade = reinterpret_cast<WeatherDataEntry *>(fade);
    data.rain.pool.availableCount = 0;
    ReadWeatherParticleModel(particles, "/particles/rain/rain.fbx");
    data.rain.damping = 0.06f;
    data.rain.colour = std::uint32_t{0x15b7b9bbU};
    data.rain.width = 0.0035f;
    data.rain.height = 0.15f;
    data.rain.velocity = Vector3{0.0f, -0.28f, 0.0f};
    data.rain.velocityRange = Vector3{};
    Weather_SetRainLayerCount(&data, 0.0f);
    data.rain.flags = 0xc2000000U;
    data.rain.material.texture = 0x02000080U;
    data.rain.material.render = 0x88800000U;
    data.rain.material.alpha = 0x94108020U;
}

void BuildCaraRainLayers(WeatherCaraData &data, float intensity, std::size_t bucketBytes, bool clipSentinels, RainRandomFunction generator) {
    const std::int32_t halfFov = data.fieldOfView / 2;
    const float tangent = std::tan(static_cast<float>(static_cast<std::uint16_t>(halfFov)) * (2.0f * std::numbers::pi_v<float>) / 65536.0f);
    constexpr Vector3 center{};
    const auto ensureLayer = [&](WeatherNodeData *&storage, float depth, float layerIntensity) {
        WeatherNodeData *layer = storage;
        if (!layer) {
            const float extent = tangent * depth * 2.0f;
            layer = AllocateGlobalWeatherData(bucketBytes, center, extent, extent, clipSentinels);
            storage = layer;
        }
        if (!layer)
            return;
        InitializeRainLayer(*layer, layerIntensity, generator);
        layer->bounds.dimensionX = depth;
    };
    ensureLayer(data.rainPrimary, 2.8f, intensity);
    ensureLayer(data.rainSecondary, 6.0f, intensity * 0.47265625f);
}

// Weather_Rain_BuildBucketsForFov: clipped type-1 camera rain in GBLS buckets.
void BuildRainLayersForFov(WeatherCaraData &data, float intensity) {
    BuildCaraRainLayers(data, intensity, kGlobalWeatherBucketBytes, true, std::bit_cast<RainRandomFunction>(&SpawnCaraRainParticleNative));
}

// Shenmue_InitRainParticles: unclipped type-0 camera rain in Shenmue_init_rain_struct buckets.
void InitRainParticles(WeatherCaraData &data, float intensity) {
    BuildCaraRainLayers(data, intensity, kRainWeatherBucketBytes, false, std::bit_cast<RainRandomFunction>(&GenerateRainParticleNative));
}

void GenerateRainParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    std::uint32_t available = std::bit_cast<std::uint32_t>(data->rain.pool.availableCount);
    if (available == 0)
        return;
    data->rain.pool.activeCount = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(data->rain.pool.activeCount) + 1U);
    data->rain.pool.availableCount = std::bit_cast<std::int32_t>(available - 1U);
    particle->random = std::bit_cast<RainRandomFunction>(&UpdateRainParticleNative);
    particle->flags = 0x80000000U;
    const auto randomUnit = [] { return static_cast<float>((NextEffectRandom() >> 16U) & 0x7fffU) * 0x1p-15f; };
    particle->position.x = data->bounds.minimumX + randomUnit() * data->bounds.dimensionY;
    const float heightRandom = randomUnit();
    particle->position.y =
        static_cast<float>(static_cast<double>(data->bounds.maximumY) - static_cast<double>(heightRandom) * (static_cast<double>(data->rain.fallHeight) * 8.0));
    particle->position.z = data->bounds.minimumZ + randomUnit() * data->bounds.dimensionY;
    const float velocityXRange = data->rain.velocityRange.x;
    particle->velocity.x = randomUnit() * velocityXRange - velocityXRange * 0.5f + data->rain.velocity.x;
    const float velocityZRange = data->rain.velocityRange.z;
    particle->velocity.z = randomUnit() * velocityZRange - velocityZRange * 0.5f + data->rain.velocity.z;
}

void UpdateRainParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    if (particle->position.y < data->bounds.minimumY) {
        particle->flags = 0x40000000U;
        particle->random = std::bit_cast<RainRandomFunction>(&GenerateRainParticleNative);
        data->rain.pool.activeCount = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(data->rain.pool.activeCount) - 1U);
        return;
    }
    particle->position.x += data->rain.frameVelocity.x + particle->velocity.x;
    particle->position.y += data->rain.frameVelocity.y;
    particle->position.z += data->rain.frameVelocity.z + particle->velocity.z;
    const float span = data->bounds.dimensionY;
    if (particle->position.x > data->bounds.maximumX)
        particle->position.x -= span;
    else if (data->bounds.minimumX > particle->position.x)
        particle->position.x += span;
    else if (particle->position.z > data->bounds.maximumZ)
        particle->position.z -= span;
    else if (data->bounds.minimumZ > particle->position.z)
        particle->position.z += span;
}

float FlushWeatherEpsilon(float value) {
    return std::fabs(value) < std::numeric_limits<float>::epsilon() ? 0.0f : value;
}

float WeatherRandomUnit(std::uint32_t seed) {
    return static_cast<float>((seed >> 16U) & 0x7fffU) * 0x1p-15f;
}

bool WeatherPointInsideClipQuad(const WeatherClipBounds &bounds, float x, float y, float z) {
    if (y > bounds.maximumY || bounds.minimumY > y)
        return false;
    for (std::size_t edge = 0; edge < 4; ++edge) {
        const float cross = (x - bounds.cornerX[edge]) * bounds.edgeNormal[edge].x - (z - bounds.cornerZ[edge]) * bounds.edgeNormal[edge].y;
        if (cross >= 0.0f)
            return false;
    }
    return true;
}

void SpawnCaraRainParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    const std::int32_t available = data->rain.pool.availableCount;
    if (available <= 0)
        return;
    data->rain.pool.activeCount = data->rain.pool.activeCount + 1;
    data->rain.pool.availableCount = available - 1;
    particle->random = std::bit_cast<RainRandomFunction>(&UpdateCaraRainParticleNative);
    const float span = data->bounds.dimensionY;
    particle->position.x = WeatherRandomUnit(NextEffectRandom()) * span + data->bounds.minimumX;
    const double fall = static_cast<double>(data->rain.fallHeight) * 8.0;
    particle->position.y = static_cast<float>(static_cast<double>(data->bounds.maximumY) - static_cast<double>(WeatherRandomUnit(NextEffectRandom())) * fall);
    particle->position.z = WeatherRandomUnit(NextEffectRandom()) * span + data->bounds.minimumZ;
    particle->velocity.x = 0.0f;
    particle->velocity.z = 0.0f;
    if ((WeatherPointInsideClipQuad(
             reinterpret_cast<const WeatherClippedVolume *>(data)->bounds.clips[0], particle->position.x, particle->position.y, particle->position.z) ||
            WeatherPointInsideClipQuad(
                reinterpret_cast<const WeatherClippedVolume *>(data)->bounds.clips[1], particle->position.x, particle->position.y, particle->position.z))) {
        particle->flags = 0x40000000U;
        particle->random = std::bit_cast<RainRandomFunction>(&SpawnCaraRainParticleNative);
        return;
    }
    particle->flags = 0x80000000U;
}

void UpdateCaraRainParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    if (particle->position.y < data->bounds.minimumY) {
        particle->flags = 0x40000000U;
        particle->random = std::bit_cast<RainRandomFunction>(&SpawnCaraRainParticleNative);
        data->rain.pool.activeCount = data->rain.pool.activeCount - 1;
        return;
    }
    const float span = data->bounds.dimensionY;
    particle->position.x = data->rain.frameVelocity.x + particle->position.x;
    particle->position.y = particle->position.y + data->rain.frameVelocity.y;
    particle->position.z = data->rain.frameVelocity.z + particle->position.z;
    if (particle->position.x > data->bounds.maximumX)
        particle->position.x -= span;
    else if (data->bounds.minimumX > particle->position.x)
        particle->position.x += span;
    else if (particle->position.z > data->bounds.maximumZ)
        particle->position.z -= span;
    else if (data->bounds.minimumZ > particle->position.z)
        particle->position.z += span;
    const bool clipped = WeatherPointInsideClipQuad(reinterpret_cast<const WeatherClippedVolume *>(data)->bounds.clips[0], particle->position.x,
                             particle->position.y, particle->position.z) ||
        WeatherPointInsideClipQuad(
            reinterpret_cast<const WeatherClippedVolume *>(data)->bounds.clips[1], particle->position.x, particle->position.y, particle->position.z);
    particle->flags = clipped ? 0x40000000U : 0x80000000U;
}

void SpawnVolumeRainParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    const std::int32_t available = data->rain.pool.availableCount;
    if (available <= 0)
        return;
    data->rain.pool.activeCount = data->rain.pool.activeCount + 1;
    data->rain.pool.availableCount = available - 1;
    particle->random = std::bit_cast<RainRandomFunction>(&UpdateVolumeRainParticleNative);
    particle->flags = 0x80000000U;
    particle->position.x = WeatherRandomUnit(NextEffectRandom()) * data->bounds.dimensionX + data->bounds.minimumX;
    const double fall = static_cast<double>(data->rain.fallHeight) * 8.0;
    particle->position.y = static_cast<float>(static_cast<double>(data->bounds.maximumY) - static_cast<double>(WeatherRandomUnit(NextEffectRandom())) * fall);
    particle->position.z = WeatherRandomUnit(NextEffectRandom()) * data->bounds.dimensionZ + data->bounds.minimumZ;
    const float rangeX = data->rain.velocityRange.x;
    particle->velocity.x = WeatherRandomUnit(NextEffectRandom()) * rangeX - rangeX * 0.5f + data->rain.velocity.x;
    const float rangeZ = data->rain.velocityRange.z;
    particle->velocity.z = WeatherRandomUnit(NextEffectRandom()) * rangeZ - rangeZ * 0.5f + data->rain.velocity.z;
}

void UpdateVolumeRainParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    if (particle->position.y < data->bounds.minimumY) {
        particle->flags = 0x40000000U;
        particle->random = std::bit_cast<RainRandomFunction>(&SpawnVolumeRainParticleNative);
        data->rain.pool.activeCount = data->rain.pool.activeCount - 1;
        return;
    }
    particle->position.x = data->rain.frameVelocity.x + particle->velocity.x + particle->position.x;
    particle->position.y = particle->position.y + data->rain.velocity.y;
    particle->position.z = data->rain.frameVelocity.z + particle->velocity.z + particle->position.z;
    if (particle->position.x > data->bounds.maximumX)
        particle->position.x -= data->bounds.dimensionX;
    else if (data->bounds.minimumX > particle->position.x)
        particle->position.x += data->bounds.dimensionX;
    else if (particle->position.z > data->bounds.maximumZ)
        particle->position.z -= data->bounds.dimensionZ;
    else if (data->bounds.minimumZ > particle->position.z)
        particle->position.z += data->bounds.dimensionZ;
}

// Shared cycle start: ramp index and the scale window scaled by Rain_VisualBlend.
void BeginRippleCycle(RippleParticleNative &ripple, const WeatherNodeData *group) {
    const auto unit = static_cast<float>(static_cast<double>((NextEffectRandom() >> 16U) & 0x7fffU) * 3.0517578125e-05);
    const auto cycle = static_cast<float>(static_cast<double>(unit) + 1.0);
    const float blend = g_rainVisualBlend;
    ripple.maximumScale = cycle * blend;
    const auto index = static_cast<std::int32_t>(static_cast<double>(cycle * 32.0f) * 0.5);
    ripple.scale = blend * 0.5f;
    ripple.colour = reinterpret_cast<const std::uint32_t *>(group->ripple.colourRamp) + index;
}


void RestartRippleCycle(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction respawn) {
    const std::int32_t active = group->ripple.pool.activeCount;
    if (active >= group->ripple.pool.availableCount) {
        group->ripple.pool.activeCount = active - 1;
        ripple.random = respawn;
        ripple.flags = 0x40000000U;
        return;
    }
    BeginRippleCycle(ripple, group);
}

// Five FindHighestValidCollisionBelow probes through the group's +0x160 query node: slopes steeper than 0x1000 reject.
bool ProbeRippleImpact(RippleParticleNative &ripple, WeatherNodeData *group) {
    auto &probe = group->ripple.collisionProbe;
    probe.node.position.x = ripple.position.x;
    probe.node.position.z = ripple.position.z;
    Matrix4x4_TranslateAndRotate(probe);
    probe.node.position.z = static_cast<float>(static_cast<double>(ripple.position.z) + 0.45);
    const float front = FindHighestValidCollisionBelow(probe.node.position, 0.0f, 0.0f);
    probe.node.position.z = static_cast<float>(static_cast<double>(ripple.position.z) - 0.45);
    const float back = FindHighestValidCollisionBelow(probe.node.position, 0.0f, 0.0f);
    ripple.angleX = static_cast<std::int32_t>(atan2_fixed16(-(front - back), 0.5f));
    if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(ripple.angleX))) > 0x1000)
        return false;
    probe.node.position.x = static_cast<float>(static_cast<double>(ripple.position.x) + 0.45);
    probe.node.position.z = ripple.position.z;
    const float right = FindHighestValidCollisionBelow(probe.node.position, 0.0f, 0.0f);
    probe.node.position.x = static_cast<float>(static_cast<double>(ripple.position.x) - 0.45);
    const float left = FindHighestValidCollisionBelow(probe.node.position, 0.0f, 0.0f);
    ripple.angleY = -static_cast<std::int32_t>(atan2_fixed16(right - left, 0.5f));
    if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(ripple.angleY))) > 0x1000)
        return false;
    probe.node.position.x = ripple.position.x;
    ripple.position.y = FindHighestValidCollisionBelow(probe.node.position, 0.0f, 0.0f);
    ripple.angleX -= 0x4000;
    ripple.position.y = static_cast<float>(static_cast<double>(ripple.position.y) + 0.02);
    return true;
}

// EvaluateRainImpactLocation: an accepted impact joins the active set and starts its cycle.
void EvaluateRainImpactLocation(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction update) {
    if (!ProbeRippleImpact(ripple, group))
        return;
    BeginRippleCycle(ripple, group);
    group->ripple.pool.activeCount = group->ripple.pool.activeCount + 1;
    ripple.random = update;
}

void SpawnRippleAtRandom(RippleParticleNative &ripple, WeatherNodeData *group, float spanX, float spanZ, RainRandomFunction update) {
    if (group->ripple.pool.activeCount >= group->ripple.pool.availableCount)
        return;
    const float first = WeatherRandomUnit(NextEffectRandom());
    const float second = WeatherRandomUnit(NextEffectRandom());
    ripple.position.x = first * spanX + group->bounds.minimumX;
    ripple.position.z = second * spanZ + group->bounds.minimumZ;
    EvaluateRainImpactLocation(ripple, group, update);
}

// Re-probe after wrapping inside the camera bucket; a rejected spot returns the record to its spawn state.
void ReprobeRippleImpact(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction spawn, RainRandomFunction update) {
    const std::int32_t active = group->ripple.pool.activeCount;
    if (active >= group->ripple.pool.availableCount || !ProbeRippleImpact(ripple, group)) {
        group->ripple.pool.activeCount = group->ripple.pool.activeCount - 1;
        ripple.random = spawn;
        return;
    }
    ripple.random = update;
}

// Camera-bucket growth step: returns true when the ripple wrapped and must re-probe.
bool AdvanceBucketRipple(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction reprobe) {
    ripple.scale += g_rainSpawnScale;
    --ripple.colour;
    ripple.flags = 0x40000000U;
    const float span = group->bounds.dimensionY;
    if (ripple.position.x > group->bounds.maximumX) {
        ripple.position.x -= span;
    } else if (group->bounds.minimumX > ripple.position.x) {
        ripple.position.x += span;
    } else if (ripple.position.z > group->bounds.maximumZ) {
        ripple.position.z -= span;
    } else if (group->bounds.minimumZ > ripple.position.z) {
        ripple.position.z += span;
    } else {
        return false;
    }
    ripple.random = reprobe;
    return true;
}

bool RippleInsideClipEdges(const WeatherClipBounds &bounds, float x, float z) {
    for (std::size_t edge = 0; edge < 4; ++edge) {
        const float cross = (x - bounds.cornerX[edge]) * bounds.edgeNormal[edge].x - (z - bounds.cornerZ[edge]) * bounds.edgeNormal[edge].y;
        if (!(cross < 0.0f))
            return false;
    }
    return true;
}

// Rain_CalcPRNGWithCallback: authored FIXG volumes sample their own X/Z extent.
void Rain_CalcPRNGWithCallback(RainParticleNative *record, WeatherNodeData *group) {
    SpawnRippleAtRandom(*reinterpret_cast<RippleParticleNative *>(record), group, group->bounds.dimensionX, group->bounds.dimensionZ,
        std::bit_cast<RainRandomFunction>(&RainCallback));
}

// RainCallback: grow until the cycle ends; only impacts below the camera eye draw.
void RainCallback(RainParticleNative *record, WeatherNodeData *group) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    auto &ripple = *reinterpret_cast<RippleParticleNative *>(record);
    if (!(ripple.maximumScale >= ripple.scale)) {
        RestartRippleCycle(ripple, group, std::bit_cast<RainRandomFunction>(&Rain_CalcPRNGWithCallback));
        return;
    }
    if (!(ripple.position.y < group->ripple.cameraEyeY)) {
        ripple.flags = 0x40000000U;
        return;
    }
    ripple.scale += g_rainSpawnScale;
    --ripple.colour;
    ripple.flags = 0x80000000U;
}

// EvaluateRainImpactLocation_0: type-0 camera bucket spawn.
void EvaluateRainImpactLocation_0(RainParticleNative *record, WeatherNodeData *group) {
    const float span = group->bounds.dimensionY;
    SpawnRippleAtRandom(*reinterpret_cast<RippleParticleNative *>(record), group, span, span, std::bit_cast<RainRandomFunction>(&EvaluateRainImpactLocation_2));
}

// EvaluateRainImpactLocation_1: type-0 re-probe after a bucket wrap.
void EvaluateRainImpactLocation_1(RainParticleNative *record, WeatherNodeData *group) {
    ReprobeRippleImpact(*reinterpret_cast<RippleParticleNative *>(record), group, std::bit_cast<RainRandomFunction>(&EvaluateRainImpactLocation_0),
        std::bit_cast<RainRandomFunction>(&EvaluateRainImpactLocation_2));
}

// EvaluateRainImpactLocation_2: type-0 bucket ripple update.
void EvaluateRainImpactLocation_2(RainParticleNative *record, WeatherNodeData *group) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    auto &ripple = *reinterpret_cast<RippleParticleNative *>(record);
    if (!(ripple.maximumScale > ripple.scale)) {
        RestartRippleCycle(ripple, group, std::bit_cast<RainRandomFunction>(&EvaluateRainImpactLocation_0));
        return;
    }
    if (AdvanceBucketRipple(ripple, group, std::bit_cast<RainRandomFunction>(&EvaluateRainImpactLocation_1)))
        return;
    if (group->ripple.cameraEyeY > ripple.position.y)
        ripple.flags = 0x80000000U;
}

// SpawnRandomRainImpactCandidate: type-1 (WCCP-clipped) camera bucket spawn.
void SpawnRandomRainImpactCandidate(RainParticleNative *record, WeatherNodeData *group) {
    const float span = group->bounds.dimensionY;
    SpawnRippleAtRandom(*reinterpret_cast<RippleParticleNative *>(record), group, span, span, std::bit_cast<RainRandomFunction>(&UpdateClippedRainImpact));
}

// EvaluateRainImpactLocation_3: type-1 re-probe after a bucket wrap.
void EvaluateRainImpactLocation_3(RainParticleNative *record, WeatherNodeData *group) {
    ReprobeRippleImpact(*reinterpret_cast<RippleParticleNative *>(record), group, std::bit_cast<RainRandomFunction>(&SpawnRandomRainImpactCandidate),
        std::bit_cast<RainRandomFunction>(&UpdateClippedRainImpact));
}

// Type-1 bucket ripple update; ripples inside either WCCP clip quad stay hidden.
void UpdateClippedRainImpact(RainParticleNative *record, WeatherNodeData *group) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    auto &ripple = *reinterpret_cast<RippleParticleNative *>(record);
    if (!(ripple.maximumScale > ripple.scale)) {
        RestartRippleCycle(ripple, group, std::bit_cast<RainRandomFunction>(&SpawnRandomRainImpactCandidate));
        return;
    }
    if (AdvanceBucketRipple(ripple, group, std::bit_cast<RainRandomFunction>(&EvaluateRainImpactLocation_3)))
        return;
    if (!(group->ripple.cameraEyeY > ripple.position.y))
        return;
    ripple.flags = 0x80000000U;
    if (RippleInsideClipEdges(reinterpret_cast<const WeatherClippedVolume *>(group)->bounds.clips[0], ripple.position.x, ripple.position.z) ||
        RippleInsideClipEdges(reinterpret_cast<const WeatherClippedVolume *>(group)->bounds.clips[1], ripple.position.x, ripple.position.z))
        ripple.flags = 0x40000000U;
}

// AssignHamonTex2D: bind the 64x64 w_hamon.pvr sprite to the ripple material words.
void AssignHamonTex2D(WeatherNodeData &data) {
    data.ripple.textureReference.texture = static_cast<void *>(&g_weatherHamonTexture);
    data.ripple.material.texture = std::uint32_t{0x8000000aU};
    data.ripple.material.render = std::uint32_t{0x8a800000U};
    data.ripple.material.alpha = WeatherSpriteSizeBits(g_weatherHamonTexture) | 0x940242c0U;
    data.ripple.textureHandle = g_weatherHamonTexture.handle;
    data.ripple.flags = data.ripple.flags | 0x4000000U;
}

// SetCallbackFunc_AssignHamonTex2D: reset every record, bind the material and seed the ground-probe node from Ryo's section.
void SetCallbackFunc_AssignHamonTex2D(WeatherNodeData &data, RainRandomFunction callback) {
    data.ripple.material.texture = std::uint32_t{0x80000002U};
    data.ripple.material.render = std::uint32_t{0x88800000U};
    const std::uint64_t initialAlphaAndHandle = 0x208000c0U;
    std::memcpy(&data.ripple.material.alpha, &initialAlphaAndHandle, sizeof(initialAlphaAndHandle));
    data.ripple.flags = std::uint32_t{0x82000000U};
    auto *const records = reinterpret_cast<RippleParticleNative *>(data.ripple.pool.records);
    for (std::int32_t index = 0; index < data.ripple.pool.capacity; ++index)
        records[index] = {.flags = 0x40000000U, .random = callback};
    AssignHamonTex2D(data);
    data.ripple.material.alpha = data.ripple.material.alpha | 0x100000U;
    data.ripple.material.texture = data.ripple.material.texture | 0x2000000U;
    data.ripple.flags = data.ripple.flags | 0x40000000U;
    reinterpret_cast<std::uint8_t *>(&data.ripple.material.alpha)[3] = 0x94U;
    std::memset(&data.ripple.collisionProbe, 0, sizeof(data.ripple.collisionProbe));
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const auto *const ryo = control ? static_cast<const Character *>(control->ryo) : nullptr;
    data.ripple.collisionProbe.layer = ryo ? ryo->eventGroup : 0U;
    data.ripple.collisionProbe.node.position.y = 100.0f;
}

// Shared by EvaluateRain and Weather_EvaluateRain: allocate the 0x38 ripple pool (at most 600) and its ramp.
void InitializeRippleLayer(WeatherNodeData &data, float intensity, RainRandomFunction callback) {
    if (data.ripple.pool.records)
        return;
    std::int32_t count = static_cast<std::int32_t>(std::fmin(kWeatherRippleDensityCap, intensity) * data.volume);
    data.ripple.pool.capacity = count;
    if (!count)
        return;
    if (count > 600)
        count = 600;
    data.ripple.pool.capacity = count;
    data.ripple.pool.records =
        HLib_AllocHeapMemBlock(static_cast<std::int64_t>(sizeof(RippleParticleNative)) * count, static_cast<std::int32_t>(SHEN_NAME(RPLP)));
    const std::uint64_t initialCounts = 0;
    std::memcpy(&data.ripple.pool.activeCount, &initialCounts, sizeof(initialCounts));
    auto *const fade = static_cast<WeatherDataEntry *>(HLib_AllocHeapMemBlock(sizeof(WeatherDataEntry), static_cast<std::int32_t>(SHEN_NAME(LFLT))));
    *fade = {};
    data.ripple.pool.fade = fade;
    data.ripple.pool.availableCount = 0;
    Weather_UpdateColourRampForData(&data, 0xa0ffffffU);
    data.ripple.size = 0.15f;
    if (data.ripple.pool.records) {
        const float zero = static_cast<float>(count) * 0.0f;
        *fade = {.intensity = zero, .targetIntensity = zero};
    }
    SetCallbackFunc_AssignHamonTex2D(data, callback);
}

// Weather_Calculate_1 (type 0) and Weather_Calculate (type 1): the ripple bucket spans tan(fov / 2) * 4 each side of the camera.
void BuildCaraRippleLayer(WeatherCaraData &data, float intensity, std::size_t bucketBytes, bool clipSentinels, RainRandomFunction callback) {
    if (!data.windSystem) {
        const float tangent = std::tan(static_cast<float>(static_cast<std::uint16_t>(data.fieldOfView / 2)) * (2.0f * std::numbers::pi_v<float>) / 65536.0f);
        data.windSystem = AllocateGlobalWeatherData(bucketBytes, Vector3{}, tangent * 4.0f + tangent * 4.0f, 1.0f, clipSentinels);
    }
    auto *const ripple = data.windSystem;
    InitializeRippleLayer(*ripple, intensity, callback);
    ripple->bounds.dimensionX = 4.0f;
}

void UpdateRainDrift(WeatherNodeData *group, const Vector3 &forces) {
    if (!group)
        return;
    const float damping = FlushWeatherEpsilon(group->rain.damping);
    const float driftX = FlushWeatherEpsilon(group->rain.frameVelocity.x);
    const float driftZ = FlushWeatherEpsilon(group->rain.frameVelocity.z);
    const float keptX = FlushWeatherEpsilon(driftX - FlushWeatherEpsilon(damping * driftX));
    const float keptZ = FlushWeatherEpsilon(driftZ - FlushWeatherEpsilon(damping * driftZ));
    group->rain.frameVelocity.x = FlushWeatherEpsilon(keptX + FlushWeatherEpsilon(forces.x + forces.x));
    group->rain.frameVelocity.z = FlushWeatherEpsilon(keptZ + FlushWeatherEpsilon(forces.z + forces.z));
}

void ResetRainGroupParticles(WeatherNodeData *group, RainRandomFunction generator, float divisor) {
    group->rain.flags = group->rain.flags & ~1U;
    group->rain.pool.activeCount = 0;
    auto *particle = reinterpret_cast<RainParticleNative *>(group->rain.pool.records);
    const std::int32_t available = group->rain.pool.availableCount;
    const std::int32_t capacity = group->rain.pool.capacity;
    std::int32_t index = 0;
    if (available > 0) {
        do {
            generator(particle++, group);
            ++index;
        } while (group->rain.pool.availableCount > 0);
    }
    for (; index < capacity; ++particle, ++index) {
        particle->flags = 0x40000000U;
        particle->random = generator;
    }
    const float fall = -group->rain.velocity.y;
    group->rain.fallHeight = fall;
    std::int32_t target = static_cast<std::int32_t>(static_cast<float>(available) * fall / divisor);
    if (target >= capacity)
        target = capacity - 1;
    group->rain.pool.availableCount = target;
    *group->rain.pool.fade = {.intensity = static_cast<float>(target), .targetIntensity = static_cast<float>(target)};
}

void Rain_UpdateBucket(WeatherNodeData *group, const Vector3 &previousTarget, const Vector3 &target, const Vector3 &forces, std::int32_t layer, bool caraRain) {
    const std::int64_t status = Weather_AdvanceWeatherEntry(group->rain.pool.fade);
    if (caraRain) {
        g_activeRainEntry->intensity = FlushWeatherEpsilon(g_activeRainEntry->intensity);
        SetRainParametersFromIntensity(group, std::fmin(1.0f, g_activeRainEntry->intensity), layer);
    }
    UpdateRainDrift(group, forces);
    Bucket_UpdateOrientationFromCameraDelta(group, previousTarget, target);
    ClampWeatherTarget(group->rain.pool);
    if (!status && (group->rain.flags & 1U) != 0) {
        const RainRandomFunction generator =
            caraRain ? std::bit_cast<RainRandomFunction>(&SpawnCaraRainParticleNative) : std::bit_cast<RainRandomFunction>(&GenerateRainParticleNative);
        ResetRainGroupParticles(group, generator, group->bounds.dimensionY);
    }
    PublishWeatherRenderCount(group->rain.pool);
}

void UpdateWeatherGroupCullBox(WeatherNodeData *group, const Vector3 &direction) {
    const float depth = group->bounds.dimensionX;
    const auto halfFov = static_cast<std::uint16_t>(currentPerspectiveFov / 2);
    const float tangent = std::tan(static_cast<float>(halfFov) * 6.2831855f * 0x1p-16f);
    const float span = tangent * depth + tangent * depth;
    group->bounds.dimensionY = span;
    const Vector3 &eye = g_weatherCameraState->previousEye;
    const float x = depth * direction.x + eye.x;
    const float y = depth * direction.y + eye.y;
    const float z = depth * direction.z + eye.z;
    const float half = span * 0.5f;
    group->bounds.maximumX = x + half;
    group->bounds.minimumX = x - half;
    group->bounds.maximumY = y + half;
    group->bounds.minimumY = y - half;
    group->bounds.maximumZ = z + half;
    group->bounds.minimumZ = z - half;
}

void Weather_UpdateRippleTarget(WeatherNodeData *group) {
    if (!group)
        return;
    group->ripple.cameraEyeY = g_weatherCameraState->previousEye.y;
    Weather_AdvanceWeatherEntry(group->ripple.pool.fade);
    const std::int32_t available = ClampWeatherTarget(group->ripple.pool);
    group->ripple.pool.renderCount = available ? group->ripple.pool.capacity : 0;
}

void Rain_UpdateFromGameDataAndAdvanceTimers(WeatherNodeData *group) {
    g_activeRainEntry->intensity = FlushWeatherEpsilon(g_activeRainEntry->intensity);
    if (g_activeRainEntry->intensity >= 0.5f) {
        const double blend = (static_cast<double>(g_activeRainEntry->intensity) - 0.5) + (static_cast<double>(g_activeRainEntry->intensity) - 0.5);
        g_rainSpawnScale = static_cast<float>((blend * 0.5 + 0.5) * 0.05);
        g_rainVisualBlend = static_cast<float>(blend * 0.1666 + 0.5);
    } else {
        g_rainSpawnScale = 0.025f;
        g_rainVisualBlend = 0.5f;
    }
    Weather_UpdateRippleTarget(group);
}

void Weather_BuildCullBoxAndDispatch(WeatherCaraData &data, const Vector3 &forces) {
    const Vector3 &direction = g_weatherCameraState->direction;
    const Vector3 &previousTarget = g_weatherCameraState->previousTarget;
    const Vector3 &target = g_weatherCameraState->target;
    if (data.rainPrimary) {
        UpdateWeatherGroupCullBox(data.rainPrimary, direction);
        Rain_UpdateBucket(data.rainPrimary, previousTarget, target, forces, 0, true);
    }
    if (data.rainSecondary) {
        UpdateWeatherGroupCullBox(data.rainSecondary, direction);
        Rain_UpdateBucket(data.rainSecondary, previousTarget, target, forces, 1, true);
    }
    if (data.windSystem) {
        UpdateWeatherGroupCullBox(data.windSystem, direction);
        Rain_UpdateFromGameDataAndAdvanceTimers(data.windSystem);
    }
    if (data.snowPrimary) {
        UpdateWeatherSnowCenter(data.snowPrimary, direction);
        Weather_UpdateContainmentVolumes(data.snowPrimary, 0);
        Snow_UpdateBucket(data.snowPrimary, forces, 0, true);
    }
    if (data.snowSecondary) {
        UpdateWeatherSnowCenter(data.snowSecondary, direction);
        Weather_UpdateContainmentVolumes(data.snowSecondary, 1);
        Snow_UpdateBucket(data.snowSecondary, forces, 1, true);
    }
}

} // namespace shenmue::s1::effe_detail
