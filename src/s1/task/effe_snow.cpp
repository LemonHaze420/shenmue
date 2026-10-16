#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

std::uint32_t NextEffectRandom() {
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    return g_EnvPrngSeed;
}

void GenerateSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    std::int32_t available = data->snow.pool.availableCount;
    if (available <= 0)
        return;
    particle->random = std::bit_cast<RainRandomFunction>(&UpdateGeneratedSnowParticleNative);
    particle->flags = 0x80000000U;
    data->snow.pool.activeCount = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(data->snow.pool.activeCount) + 1U);
    data->snow.pool.availableCount = available - 1;

    particle->position.x = data->bounds.minimumX + static_cast<float>((NextEffectRandom() >> 16U) & 0x7fffU) * 0x1p-15f * data->bounds.dimensionX;
    particle->position.y = data->bounds.maximumY - static_cast<float>((NextEffectRandom() >> 16U) & 0x7fffU) * 0x1p-15f * data->snow.fallHeight;
    particle->position.z = data->bounds.minimumZ + static_cast<float>((NextEffectRandom() >> 16U) & 0x7fffU) * 0x1p-15f * data->bounds.dimensionZ;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const float center = data->snow.velocity[axis];
        const float range = data->snow.velocityRange[axis];
        particle->velocity[axis] = center + (static_cast<float>((NextEffectRandom() >> 16U) & 0x7fffU) * 0x1p-15f - 0.5f) * range;
    }
}

void UpdateGeneratedSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    if (particle->position.y < data->bounds.minimumY) {
        particle->flags = 0x40000000U;
        particle->random = std::bit_cast<RainRandomFunction>(&GenerateSnowParticleNative);
        data->snow.pool.activeCount = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(data->snow.pool.activeCount) - 1U);
        return;
    }

    float deltaX = particle->velocity.x + data->snow.cameraPush.x;
    float deltaY = particle->velocity.y;
    float deltaZ = particle->velocity.z + data->snow.cameraPush.z;
    if (g_bEnableFrameTimeSmoothing) {
        deltaX = (deltaX * 30.0f) * fFrameTimeMs;
        deltaY = (deltaY * 30.0f) * fFrameTimeMs;
        deltaZ = (deltaZ * 30.0f) * fFrameTimeMs;
    }
    particle->position.x += deltaX;
    particle->position.y += deltaY;
    particle->position.z += deltaZ;
    if (particle->position.x > data->bounds.maximumX)
        particle->position.x -= data->bounds.dimensionX;
    else if (particle->position.x < data->bounds.minimumX)
        particle->position.x += data->bounds.dimensionX;
    if (particle->position.z > data->bounds.maximumZ)
        particle->position.z -= data->bounds.dimensionZ;
    else if (particle->position.z < data->bounds.minimumZ)
        particle->position.z += data->bounds.dimensionZ;
}

// Snow_FlakeUpdate / EffectCallback_Inactive shared motion: advance by velocity plus the damped camera push, reflect
// through the previous sphere centre after leaving the outer radius, and retire outside the inner radius.
// Returns false once the flake retired to its spawn callback.
bool AdvanceCameraSnowParticle(RainParticleNative *particle, WeatherNodeData *data, RainRandomFunction spawn) {
    const bool smoothing = g_bEnableFrameTimeSmoothing != 0;
    const float frameMs = fFrameTimeMs;
    float velocityX = particle->velocity.x;
    float velocityY = particle->velocity.y;
    float velocityZ = particle->velocity.z;
    float deltaX = velocityX + data->snow.cameraPush.x;
    if (smoothing)
        deltaX = (deltaX * 30.0f) * frameMs;
    float x = deltaX + particle->position.x;
    particle->position.x = x;
    float deltaY = velocityY;
    if (smoothing)
        deltaY = (velocityY * 30.0f) * frameMs;
    float y = deltaY + particle->position.y;
    particle->position.y = y;
    float deltaZ = velocityZ + data->snow.cameraPush.z;
    if (smoothing)
        deltaZ = (deltaZ * 30.0f) * frameMs;
    float z = deltaZ + particle->position.z;
    particle->position.z = z;
    const Vector3 center = data->snowCenters.current;
    const auto distanceSquared = [&center](float px, float py, float pz) {
        return ((py - center.y) * (py - center.y) + (px - center.x) * (px - center.x)) + (pz - center.z) * (pz - center.z);
    };
    const float outerSquared = reinterpret_cast<const WeatherSnowSphere *>(data)->previousRadiusSquared;
    const float distance = distanceSquared(x, y, z);
    bool inside = false;
    if (distance > outerSquared) {
        float pushX = data->snow.cameraPush.x;
        float pushZ = data->snow.cameraPush.z;
        if (smoothing) {
            velocityX = (velocityX * 30.0f) * frameMs;
            pushX = (pushX * 30.0f) * frameMs;
            velocityY = (velocityY * 30.0f) * frameMs;
            velocityZ = (velocityZ * 30.0f) * frameMs;
            pushZ = (pushZ * 30.0f) * frameMs;
        }
        const Vector3 previous = data->snowCenters.reflection;
        particle->position.x = previous.x - ((x - velocityX) - pushX);
        particle->position.y = previous.y - (y - velocityY);
        particle->position.z = previous.z - ((z - velocityZ) - pushZ);
        inside = distanceSquared(particle->position.x, particle->position.y, particle->position.z) <
            reinterpret_cast<const WeatherSnowSphere *>(data)->radiusSquared;
    } else {
        const float innerSquared = reinterpret_cast<const WeatherSnowSphere *>(data)->radiusSquared;
        if (!(outerSquared > innerSquared))
            return true;
        inside = distance < innerSquared;
    }
    if (inside)
        return true;
    particle->flags = 0x40000000U;
    particle->random = spawn;
    data->snow.pool.activeCount = data->snow.pool.activeCount - 1;
    return false;
}

// Snow_FlakeUpdate: the unclipped camera snow draws every live flake.
void UpdateCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    particle->flags = 0xc0000000U;
    AdvanceCameraSnowParticle(particle, data, std::bit_cast<RainRandomFunction>(&SpawnCameraSnowParticleNative));
}

// Snow_SpawnOne / EffectCallback_Active: pick a point on the camera sphere. The delayed burst after a volume growth
// biases the radius inward; both branches consume three draws for the radius, two for yaw, one for pitch and three for velocity.
void SpawnCameraSnowParticleCore(RainParticleNative *particle, WeatherNodeData *data, bool clipped) {
    const std::int32_t delay = reinterpret_cast<const WeatherSnowSphere *>(data)->burstCount;
    const float outerRadius = reinterpret_cast<const WeatherSnowSphere *>(data)->radius;
    float product = 0.0f;
    if (delay > 0) {
        reinterpret_cast<WeatherSnowSphere *>(data)->burstCount = delay - 1;
        const float radialScale = (outerRadius - reinterpret_cast<const WeatherSnowSphere *>(data)->previousRadius) / outerRadius;
        const float first = WeatherRandomUnit(NextEffectRandom());
        const float second = WeatherRandomUnit(NextEffectRandom());
        const float third = WeatherRandomUnit(NextEffectRandom());
        product = radialScale * first * (third * second);
    } else {
        if (data->snow.pool.availableCount <= data->snow.pool.activeCount)
            return;
        const float first = WeatherRandomUnit(NextEffectRandom());
        const float second = WeatherRandomUnit(NextEffectRandom());
        const float third = WeatherRandomUnit(NextEffectRandom());
        product = third * second * first;
    }
    data->snow.pool.activeCount = data->snow.pool.activeCount + 1;
    const auto radius = static_cast<float>((1.0 - static_cast<double>(product)) * (static_cast<double>(outerRadius) - 0.04));
    const float yawScale = WeatherRandomUnit(NextEffectRandom());
    const float yawBias = WeatherRandomUnit(NextEffectRandom());
    const auto yaw = static_cast<std::int32_t>((static_cast<double>(yawBias) - 0.5) * static_cast<double>(yawScale) * 32767.0 + 16384.0);
    float vertical = WeatherAngleCosine(yaw) * radius;
    if (clipped)
        vertical = static_cast<float>(static_cast<double>(vertical) * 1.1);
    const float horizontal = sin_fixed16(static_cast<std::int16_t>(yaw)) * radius;
    const auto pitch = static_cast<std::int32_t>(((NextEffectRandom() >> 16U) & 0x7fffU) * 2U);
    const float offsetX = sin_fixed16(static_cast<std::int16_t>(pitch)) * horizontal;
    const float offsetZ = WeatherAngleCosine(pitch) * horizontal;
    const Vector3 center = data->snowCenters.current;
    particle->position.x = offsetX + center.x;
    particle->position.y = vertical + center.y;
    particle->position.z = offsetZ + center.z;
    const Vector3 range = data->snow.velocityRange;
    if (clipped) {
        const Vector3 base = reinterpret_cast<const WeatherClippedSnowSphere *>(data)->minimumVelocity;
        particle->velocity.x = WeatherRandomUnit(NextEffectRandom()) * range.x + base.x;
        particle->velocity.y = WeatherRandomUnit(NextEffectRandom()) * range.y + base.y;
        particle->velocity.z = WeatherRandomUnit(NextEffectRandom()) * range.z + base.z;
    } else {
        const Vector3 base = data->snow.velocity;
        const auto velocity = [](float unit, float spread, float centre) {
            return static_cast<float>(static_cast<double>(unit * spread) - static_cast<double>(spread) * 0.5 + static_cast<double>(centre));
        };
        particle->velocity.x = velocity(WeatherRandomUnit(NextEffectRandom()), range.x, base.x);
        particle->velocity.y = velocity(WeatherRandomUnit(NextEffectRandom()), range.y, base.y);
        particle->velocity.z = velocity(WeatherRandomUnit(NextEffectRandom()), range.z, base.z);
    }
    particle->flags = 0x40000000U;
    particle->random = clipped ? std::bit_cast<RainRandomFunction>(&UpdateClippedCameraSnowParticleNative) :
                                 std::bit_cast<RainRandomFunction>(&UpdateCameraSnowParticleNative);
}

void SpawnCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    SpawnCameraSnowParticleCore(particle, data, false);
}

// Log2 texel-size bits of a weather sprite's particle alpha word.
std::uint32_t WeatherSpriteSizeBits(const WeatherSpriteTexture &texture) {
    std::int32_t widthBits = 0;
    for (std::int32_t width = static_cast<std::int32_t>(texture.width); width; width >>= 1)
        ++widthBits;
    std::int32_t heightBits = 0;
    for (std::int32_t height = static_cast<std::int32_t>(texture.height); height; height >>= 1)
        ++heightBits;
    return static_cast<std::uint32_t>(widthBits * 8 - 32) | static_cast<std::uint32_t>(heightBits - 4);
}

// AssignTex2D material words around SetTexture2DProperties, which binds the w_snow.pvr texture.
void AssignSnowTexture(WeatherNodeData &data) {
    data.snow.textureReference.texture = static_cast<void *>(&g_weatherSnowTexture);
    data.snow.material.texture = std::uint32_t{0xa000000aU};
    data.snow.material.render = std::uint32_t{0x8a800000U};
    data.snow.material.alpha = WeatherSpriteSizeBits(g_weatherSnowTexture) | 0x940242c0U | 0x100000U;
    data.snow.textureHandle = g_weatherSnowTexture.handle;
    data.snow.material.texture = std::uint32_t{0xa200000aU};
    data.snow.flags = std::uint32_t{0xcc000000U};
    reinterpret_cast<std::uint8_t *>(&data.snow.material.alpha)[3] = 0x94U;
}






void ReadWeatherParticleModel(void *particles, const char *modelPath) {
    const auto key = reinterpret_cast<std::uintptr_t>(particles);
    if (ResolveHmdlRuntimeObject(key)) {
        if (auto *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(key))) {
            if (entry->retire != 2)
                entry->retire = 1;
            g_hmdlEntryDirty = 1;
        }
        ReleaseHmdlRuntimeObject(key);
        ReleaseHmdlRuntimeObject(key + 1);
    }
    d3t::FileRead_ALL(key, modelPath, 0, 0x1d80);
}

void InitializeNativeSnowParticles(WeatherNodeData &data, float intensity, RainRandomFunction initialCallback, bool cameraRelative) {
    if (data.snow.pool.records)
        return;
    const float density = std::min(4.0f, intensity);
    const std::int32_t requested = static_cast<std::int32_t>(density * data.volume);
    data.snow.pool.capacity = requested;
    if (requested <= 0)
        return;

    const std::int32_t count = std::min(requested, 3000);
    auto *const particles = static_cast<RainParticleNative *>(
        HLib_AllocHeapMemBlock(sizeof(RainParticleNative) * static_cast<std::size_t>(count), static_cast<std::int32_t>(SHEN_NAME(SNWP))));
    if (!particles)
        return;
    auto *const fade = static_cast<WeatherDataEntry *>(HLib_AllocHeapMemBlock(sizeof(WeatherDataEntry), static_cast<std::int32_t>(SHEN_NAME(LFLT))));
    if (!fade) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(particles), 0);
        return;
    }

    std::memset(particles, 0, sizeof(RainParticleNative) * static_cast<std::size_t>(count));
    std::memset(fade, 0, sizeof(*fade));
    for (std::int32_t index = 0; index < count; ++index) {
        particles[index].flags = 0x40000000U;
        particles[index].random = initialCallback;
    }
    data.snow.pool.records = particles;
    data.snow.pool.capacity = count;
    data.snow.pool.activeCount = std::bit_cast<std::int32_t>(0U);
    if (cameraRelative)
        data.snow.pool.renderCount = std::bit_cast<std::int32_t>(0U);
    data.snow.pool.fade = fade;
    data.snow.pool.availableCount = 0;
    ReadWeatherParticleModel(particles, "/particles/snow/snow.fbx");
    data.snow.colour = std::uint32_t{0xe8cdc8d1U};
    data.snow.size = std::bit_cast<float>(0x3da3d70aU);
    data.snow.velocity = Vector3{0.0f, -0.095f, 0.0f};
    data.snow.velocityRange = Vector3{0.013f, 0.02f, 0.013f};
    data.snow.damping = std::bit_cast<float>(0x3d178d50U);
    AssignSnowTexture(data);
    if (cameraRelative) {
        reinterpret_cast<WeatherSnowSphere *>(&data)->density = density;
        reinterpret_cast<WeatherSnowSphere *>(&data)->burstCount = 0;
    } else {
        Weather_SetSnowLayerCount(&data, 0.0f);
    }
}


float CameraSnowRadius(std::int32_t fieldOfView, float depth) {
    return static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(fieldOfView / 2)) * depth * 5.0f * 0.25f) * 1.2);
}

// GetScaledScreenResolution: aspect-scaled sphere volume, 2.7 for the near layer and 1.1 for the far layer.
void SetCameraSnowVolume(WeatherNodeData &data, float radius, float radiusSquared, std::int32_t layer) {
    const DisplayModeInfo *const resolution = GetScreenResolution();
    const float aspect = resolution && resolution->height ? static_cast<float>(resolution->width) / static_cast<float>(resolution->height) : 1.0f;
    data.volume =
        static_cast<float>(static_cast<double>(aspect) * 3.141592654 * static_cast<double>(radiusSquared) * static_cast<double>(radius) * (layer ? 1.1 : 2.7));
}

// The unclipped (0x1e0) and SPHR clipped (0x280 with both WCCP sentinels) allocators build the camera sphere nodes.
WeatherNodeData *AllocateCameraSnowNode(bool clipped, float radius, std::int32_t layer) {
    const std::size_t allocationSize = clipped ? 0x280U : 0x1e0U;
    auto *const data = static_cast<WeatherNodeData *>(HLib_AllocHeapMemBlock(allocationSize, static_cast<std::int32_t>(SHEN_NAME(SPHR))));
    if (!data)
        return nullptr;
    std::memset(data, 0, allocationSize);
    if (clipped) {
        reinterpret_cast<WeatherClippedSnowSphere *>(data)->clips[0].maximumY = -100000.0f;
        reinterpret_cast<WeatherClippedSnowSphere *>(data)->clips[1].maximumY = -100000.0f;
    }
    reinterpret_cast<WeatherSnowSphere *>(data)->radius = radius;
    reinterpret_cast<WeatherSnowSphere *>(data)->radiusSquared = radius * radius;
    SetCameraSnowVolume(*data, radius, radius * radius, layer);
    WeatherNodeData *const head = FirstWeatherNode();
    if (!head) {
        g_weatherNodeHead = data;
    } else {
        WeatherNodeData *tail = head;
        while (tail->next)
            tail = tail->next;
        tail->next = data;
        data->backlink = &tail->next;
    }
    g_pGLBSTaskData = reinterpret_cast<GLBSWeatherDrawState *>(g_weatherNodeHead);
    ++g_effectsTaskState;
    return data;
}

// The unclipped and clipped layer builders: allocate from the CARA field of view, fill the pool, then resize the sphere
// from the live camera field of view.
void ConfigureCameraSnowLayer(
    WeatherNodeData *&storage, bool clipped, std::int32_t fieldOfView, float depth, std::int32_t layer, float intensity, RainRandomFunction initialCallback) {
    if (!storage)
        storage = AllocateCameraSnowNode(clipped, CameraSnowRadius(fieldOfView, depth), layer);
    WeatherNodeData *const data = storage;
    if (!data)
        return;
    InitializeNativeSnowParticles(*data, intensity, initialCallback, true);
    reinterpret_cast<WeatherSnowSphere *>(data)->depth = depth;
    const float radius = CameraSnowRadius(currentPerspectiveFov, depth);
    const float radiusSquared = radius * radius;
    reinterpret_cast<WeatherSnowSphere *>(data)->radius = radius;
    reinterpret_cast<WeatherSnowSphere *>(data)->radiusSquared = radiusSquared;
    data->snow.pool.availableCount = 0;
    reinterpret_cast<WeatherSnowSphere *>(data)->previousRadius = radius;
    reinterpret_cast<WeatherSnowSphere *>(data)->previousRadiusSquared = radiusSquared;
    SetCameraSnowVolume(*data, radius, radiusSquared, layer);
}

bool CameraSnowOutsideFrustumSet(const WeatherClipBounds &bounds, const Vector3 &position) {
    if (position.y > bounds.maximumY || position.y < bounds.minimumY) {
        return true;
    }
    for (std::size_t plane = 0; plane < 4; ++plane) {
        const float x = position.x - bounds.cornerX[plane];
        const float z = position.z - bounds.cornerZ[plane];
        const float cross = x * bounds.edgeNormal[plane].x - z * bounds.edgeNormal[plane].y;
        if (cross >= 0.0f)
            return true;
    }
    return false;
}

// EffectCallback_Inactive: a live clipped flake draws only while it sits outside both WCCP clip volumes.
void UpdateClippedCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;
    if (!AdvanceCameraSnowParticle(particle, data, std::bit_cast<RainRandomFunction>(&SpawnClippedCameraSnowParticleNative)))
        return;
    const Vector3 position{particle->position.x, particle->position.y, particle->position.z};
    const bool outsideNear = CameraSnowOutsideFrustumSet(reinterpret_cast<const WeatherClippedSnowSphere *>(data)->clips[0], position);
    const bool outsideFar = outsideNear && CameraSnowOutsideFrustumSet(reinterpret_cast<const WeatherClippedSnowSphere *>(data)->clips[1], position);
    if (outsideNear && outsideFar) {
        particle->flags = 0xc0000000U;
    } else {
        particle->flags = 0x40000000U;
        particle->random = std::bit_cast<RainRandomFunction>(&UpdateClippedCameraSnowParticleNative);
    }
}

void SpawnClippedCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data) {
    SpawnCameraSnowParticleCore(particle, data, true);
}

void InitializeCameraSnowSystems(WeatherFixc &fixc, float intensity) {
    if ((fixc.type != 0 && fixc.type != 1) || !fixc.data)
        return;
    auto &weather = *static_cast<WeatherCaraData *>(fixc.data);
    const bool clipped = fixc.type == 1;
    const RainRandomFunction callback =
        clipped ? std::bit_cast<RainRandomFunction>(&SpawnClippedCameraSnowParticleNative) : std::bit_cast<RainRandomFunction>(&SpawnCameraSnowParticleNative);
    ConfigureCameraSnowLayer(weather.snowPrimary, clipped, weather.fieldOfView, 2.8f, 0, intensity, callback);
    ConfigureCameraSnowLayer(weather.snowSecondary, clipped, weather.fieldOfView, 7.8000002f, 1, intensity, callback);
}

void Snow_LUTBlend_ParamsFromIntensity(WeatherNodeData *group, float intensity) {
    float size = 0.05f;
    Vector3 velocity{0.0f, -0.05f, 0.0f};
    Vector3 range{0.032f, 0.02f, 0.032f};
    float drift = 0.037f;
    if (intensity >= 0.5f) {
        const auto blend = static_cast<float>((static_cast<double>(intensity) - 0.5) + (static_cast<double>(intensity) - 0.5));
        size = (0.08f - 0.05f) * blend + 0.05f;
        velocity.y = (-0.095f - -0.05f) * blend + -0.05f;
        range.x = (0.013f - 0.032f) * blend + 0.032f;
        range.z = (0.013f - 0.032f) * blend + 0.032f;
    }
    group->snow.velocity.x = velocity.x;
    group->snow.velocity.y = velocity.y;
    group->snow.size = size;
    group->snow.velocityRange.x = range.x;
    group->snow.velocityRange.y = range.y;
    reinterpret_cast<WeatherClippedSnowSphere *>(group)->minimumVelocity.x = velocity.x - range.x * 0.5f;
    group->snow.velocityRange.z = range.z;
    group->snow.damping = drift;
    reinterpret_cast<WeatherClippedSnowSphere *>(group)->minimumVelocity.y = velocity.y - range.y * 0.5f;
    group->snow.velocity.z = velocity.z;
    reinterpret_cast<WeatherClippedSnowSphere *>(group)->minimumVelocity.z = velocity.z - range.z * 0.5f;
}

void Snow_ClampActiveCountByBudget(WeatherNodeData *group, std::int32_t kind, std::int32_t layer) {
    if (!g_bForceDisableSnow)
        return;
    std::int32_t current = group->snow.pool.availableCount;
    const std::int32_t capacity = group->snow.pool.capacity;
    float budget = g_weatherSnowBudget;
    if (!layer) {
        budget = g_weatherSnowBudget - static_cast<float>(current) * kWeatherSnowBudgetCost[static_cast<std::size_t>(kind)];
    } else if (g_weatherSnowBudget < 0.0f) {
        if (current > kWeatherSnowSecondaryCap[static_cast<std::size_t>(layer)])
            group->snow.pool.availableCount = kWeatherSnowSecondaryCap[static_cast<std::size_t>(layer)];
        if (group->snow.pool.availableCount > capacity)
            group->snow.pool.availableCount = capacity - 1;
        return;
    } else {
        const float cost = static_cast<float>(current) * kWeatherSnowBudgetCost[static_cast<std::size_t>(kind)];
        if (cost > 0.0f) {
            if (g_weatherSnowBudget > 0.0f) {
                if (g_weatherSnowBudget <= cost) {
                    const auto allowed = static_cast<std::int32_t>(g_weatherSnowBudget * kWeatherSnowBudgetCapacity[static_cast<std::size_t>(kind)]);
                    const std::int32_t limit = capacity > allowed ? allowed : capacity - 1;
                    if (current > limit)
                        group->snow.pool.availableCount = limit;
                    budget = 0.0f;
                } else {
                    budget = g_weatherSnowBudget - cost;
                }
            } else if (current > kWeatherSnowSecondaryCap[static_cast<std::size_t>(layer)]) {
                group->snow.pool.availableCount = kWeatherSnowSecondaryCap[static_cast<std::size_t>(layer)];
            }
        }
    }
    g_weatherSnowBudget = budget;
    if (group->snow.pool.availableCount > capacity)
        group->snow.pool.availableCount = capacity - 1;
}

void UpdateWeatherSnowCenter(WeatherNodeData *group, const Vector3 &direction) {
    const float depth = reinterpret_cast<const WeatherSnowSphere *>(group)->depth;
    const Vector3 &eye = g_weatherCameraState->previousEye;
    group->snowCenters.current.x = depth * direction.x + eye.x;
    group->snowCenters.current.y = depth * direction.y + eye.y;
    group->snowCenters.current.z = depth * direction.z + eye.z;
}

void Weather_UpdateContainmentVolumes(WeatherNodeData *group, std::int32_t layer) {
    const float radius = static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(currentPerspectiveFov / 2)) *
                                                reinterpret_cast<const WeatherSnowSphere *>(group)->depth * 5.0f * 0.25f) *
        1.2);
    const DisplayModeInfo *const resolution = GetScreenResolution();
    const float aspect = resolution && resolution->height ? static_cast<float>(resolution->width) / static_cast<float>(resolution->height) : 1.0f;
    const double cube = static_cast<double>(aspect) * 3.141592654 * radius * radius * radius;
    const auto volume = static_cast<float>(layer ? cube * 1.1 : cube * 2.7);
    const float previousVolume = group->volume;
    const float grown = volume - previousVolume;
    auto *const entry = group->snow.pool.fade;
    entry->intensity = FlushWeatherEpsilon(entry->intensity);
    group->volume = volume;
    std::int32_t burst = static_cast<std::int32_t>(grown * entry->intensity);
    if (burst > group->snow.pool.capacity)
        burst = group->snow.pool.capacity;
    const float previousRadius = reinterpret_cast<const WeatherSnowSphere *>(group)->radius;
    reinterpret_cast<WeatherSnowSphere *>(group)->radius = radius;
    reinterpret_cast<WeatherSnowSphere *>(group)->previousRadius = previousRadius;
    const float previousRadiusSquared = reinterpret_cast<const WeatherSnowSphere *>(group)->radiusSquared;
    reinterpret_cast<WeatherSnowSphere *>(group)->burstCount = burst;
    reinterpret_cast<WeatherSnowSphere *>(group)->radiusSquared = radius * radius;
    reinterpret_cast<WeatherSnowSphere *>(group)->previousRadiusSquared = previousRadiusSquared;
}

void ShiftWeatherSnowReflectionCenter(WeatherNodeData *group) {
    const Vector3 current = group->snowCenters.current;
    const Vector3 previous = group->snowCenters.previous;
    group->snowCenters.reflection.x = previous.x + current.x;
    group->snowCenters.reflection.y = previous.y + current.y;
    group->snowCenters.reflection.z = previous.z + current.z;
    group->snowCenters.previous = current;
}

void RetireSnowParticles(WeatherNodeData *group, RainRandomFunction liveCallback, RainRandomFunction spawnCallback) {
    auto *particle = reinterpret_cast<RainParticleNative *>(group->snow.pool.records);
    while (group->snow.pool.availableCount < group->snow.pool.activeCount) {
        if (particle->random == liveCallback) {
            particle->flags = 0x40000000U;
            particle->random = spawnCallback;
            group->snow.pool.activeCount = group->snow.pool.activeCount - 1;
        }
        ++particle;
    }
    PublishWeatherRenderCount(group->snow.pool);
}

void Snow_UpdateBucket(WeatherNodeData *group, const Vector3 &forces, std::int32_t layer, bool clipped) {
    if (!group)
        return;
    ShiftWeatherSnowReflectionCenter(group);
    if (!group->snow.pool.records)
        return;
    if (clipped) {
        Weather_AdvanceWeatherEntry(group->snow.pool.fade);
        g_activeSnowEntry->intensity = FlushWeatherEpsilon(g_activeSnowEntry->intensity);
        Snow_LUTBlend_ParamsFromIntensity(group, std::fmin(1.0f, g_activeSnowEntry->intensity));
        ApplyDampedCameraPushXZ(group, forces);
    } else {
        ApplyDampedCameraPushXZ(group, forces);
        Weather_AdvanceWeatherEntry(group->snow.pool.fade);
    }
    auto *const entry = group->snow.pool.fade;
    entry->intensity = FlushWeatherEpsilon(entry->intensity);
    std::int32_t current = static_cast<std::int32_t>(group->volume * entry->intensity);
    if (current >= group->snow.pool.capacity)
        current = group->snow.pool.capacity - 1;
    group->snow.pool.availableCount = current;
    Snow_ClampActiveCountByBudget(group, clipped ? 1 : 0, layer);
    if (clipped)
        RetireSnowParticles(group, std::bit_cast<RainRandomFunction>(&UpdateClippedCameraSnowParticleNative),
            std::bit_cast<RainRandomFunction>(&SpawnClippedCameraSnowParticleNative));
    else
        RetireSnowParticles(
            group, std::bit_cast<RainRandomFunction>(&UpdateCameraSnowParticleNative), std::bit_cast<RainRandomFunction>(&SpawnCameraSnowParticleNative));
}

void Weather_UpdateSnowParticles(WeatherCaraData &data, const Vector3 &forces) {
    const Vector3 &direction = g_weatherCameraState->direction;
    const Vector3 &previousTarget = g_weatherCameraState->previousTarget;
    const Vector3 &target = g_weatherCameraState->target;
    if (data.rainPrimary) {
        UpdateWeatherGroupCullBox(data.rainPrimary, direction);
        Rain_UpdateBucket(data.rainPrimary, previousTarget, target, forces, 0, false);
    }
    if (data.rainSecondary) {
        UpdateWeatherGroupCullBox(data.rainSecondary, direction);
        Rain_UpdateBucket(data.rainSecondary, previousTarget, target, forces, 1, false);
    }
    if (data.windSystem) {
        UpdateWeatherGroupCullBox(data.windSystem, direction);
        Weather_UpdateRippleTarget(data.windSystem);
    }
    if (data.snowPrimary) {
        UpdateWeatherSnowCenter(data.snowPrimary, direction);
        Weather_UpdateContainmentVolumes(data.snowPrimary, 0);
        Snow_UpdateBucket(data.snowPrimary, forces, 0, false);
    }
    if (data.snowSecondary) {
        UpdateWeatherSnowCenter(data.snowSecondary, direction);
        Weather_UpdateContainmentVolumes(data.snowSecondary, 1);
        Snow_UpdateBucket(data.snowSecondary, forces, 1, false);
    }
}

void Snow_TickSystemsInit(float intensity) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1)
            InitializeCameraSnowSystems(weather, intensity);
        else if (weather.type == 6)
            InitializeNativeSnowParticles(
                *static_cast<WeatherNodeData *>(weather.data), intensity, std::bit_cast<RainRandomFunction>(&GenerateSnowParticleNative), false);
    }
}

} // namespace shenmue::s1::effe_detail
