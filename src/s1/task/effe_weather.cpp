#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

WeatherNodeData *FirstWeatherNode() {
    return g_weatherNodeHead;
}

void ResetWccpClipBounds(WeatherClipBounds *bounds) {
    if (!bounds)
        return;
    bounds->maximumY = -99999.8984375f;
    bounds->minimumY = -0.1f;
    std::memset(bounds->cornerX, 0, sizeof(float) * 16);
}

void Weather_ProcessRainIntensityForData(WeatherNodeData *data, float intensity, std::int32_t stepCount) {
    if (!data || !data->rain.pool.records)
        return;
    const float height = data->bounds.maximumY - data->bounds.minimumY;
    const float verticalVelocity = -data->rain.velocity.y;
    data->rain.fallHeight = verticalVelocity;
    const auto target = static_cast<std::int32_t>(static_cast<float>(data->rain.pool.capacity) * intensity / height * verticalVelocity);
    Weather_ApplyWeatherEntry(data->rain.pool.fade, static_cast<float>(target), stepCount);
    data->rain.flags = data->rain.flags & ~1U;
}

void Weather_SetRainLayerCount(WeatherNodeData *data, float intensity) {
    if (!data || !data->rain.pool.records)
        return;
    data->rain.fallHeight = data->bounds.maximumY - data->bounds.minimumY;
    const auto count = static_cast<float>(static_cast<std::int32_t>(static_cast<float>(data->rain.pool.capacity) * intensity));
    *data->rain.pool.fade = {.intensity = count, .targetIntensity = count};
    data->rain.flags = data->rain.flags | 1U;
}

void Weather_UpdateSnowIntensityForData(WeatherNodeData *data, float intensity, std::int32_t stepCount) {
    if (!data || !data->snow.pool.records)
        return;
    const auto capacity = data->snow.pool.capacity;
    const float height = data->bounds.maximumY - data->bounds.minimumY;
    const float verticalVelocity = -data->snow.velocity.y;
    data->snow.fallHeight = verticalVelocity;
    const auto target = static_cast<std::int32_t>(static_cast<float>(capacity) * intensity / height * verticalVelocity);
    Weather_ApplyWeatherEntry(data->snow.pool.fade, static_cast<float>(target), stepCount);
    data->snow.flags = data->snow.flags & ~1U;
}

void Weather_UpdateSnowGroupIntensity(WeatherCaraData &data, float intensity, std::int32_t stepCount) {
    for (WeatherNodeData *snow : {data.snowPrimary, data.snowSecondary}) {
        if (snow && snow->snow.pool.fade)
            Weather_ApplyWeatherEntry(snow->snow.pool.fade, intensity * reinterpret_cast<const WeatherSnowSphere *>(snow)->density, stepCount);
    }
}

void Weather_SetSnowLayerCount(WeatherNodeData *data, float intensity) {
    if (!data || !data->snow.pool.records)
        return;
    data->snow.fallHeight = data->bounds.maximumY - data->bounds.minimumY;
    const auto count = static_cast<float>(static_cast<std::int32_t>(static_cast<float>(data->snow.pool.capacity) * intensity));
    *data->snow.pool.fade = {.intensity = count, .targetIntensity = count};
    data->snow.flags = data->snow.flags | 1U;
}

void Weather_SetSnowGroupCount(const WeatherCaraData &data, float intensity) {
    for (WeatherNodeData *snow : {data.snowPrimary, data.snowSecondary}) {
        if (!snow || !snow->snow.pool.fade)
            continue;
        const float count = intensity * reinterpret_cast<const WeatherSnowSphere *>(snow)->density;
        *snow->snow.pool.fade = {.intensity = count, .targetIntensity = count};
    }
}

WeatherNodeData *WeatherRippleGroup(const WeatherFixc &weather) {
    if (weather.type == 0 || weather.type == 1)
        return static_cast<const WeatherCaraData *>(weather.data)->windSystem;
    return weather.type == 6 ? static_cast<WeatherNodeData *>(weather.data) : nullptr;
}

void Weather_SetRippleCount(const WeatherFixc &weather, float intensity) {
    WeatherNodeData *const ripple = WeatherRippleGroup(weather);
    if (!ripple || !ripple->ripple.pool.records)
        return;
    const auto count = static_cast<float>(static_cast<std::int32_t>(static_cast<float>(ripple->ripple.pool.capacity) * intensity));
    *ripple->ripple.pool.fade = {.intensity = count, .targetIntensity = count};
}

void Weather_ApplyRippleGameData(const WeatherFixc &weather, float intensity, std::int32_t stepCount) {
    WeatherNodeData *const ripple = WeatherRippleGroup(weather);
    if (!ripple || !ripple->ripple.pool.records)
        return;
    const auto scaled = static_cast<std::int32_t>(static_cast<float>(ripple->ripple.pool.capacity) * intensity);
    Weather_ApplyWeatherEntry(ripple->ripple.pool.fade, static_cast<float>(scaled), stepCount);
}

void Weather_UpdateColourRampForData(WeatherNodeData *data, std::uint32_t colour) {
    if (!data)
        return;
    auto *ramp = data->ripple.colourRamp;
    if (!ramp) {
        ramp = static_cast<std::uint32_t *>(HLib_AllocHeapMemBlock(0x80, static_cast<std::int32_t>(SHEN_NAME(RCOL))));
        if (!ramp)
            return;
        data->ripple.colourRamp = ramp;
        std::memset(ramp, 0, 0x80);
    }
    const auto previous = data->ripple.colour;
    if (previous == colour)
        return;
    data->ripple.colour = colour;
    const std::uint32_t rgb = colour & 0xffffffU;
    if ((previous ^ colour) & 0xff000000U) {
        float phase = 0.5f;
        const float alpha = static_cast<float>(colour >> 24U);
        for (std::size_t index = 0; index < 32; ++index) {
            const auto angle = static_cast<std::int32_t>(phase * 16384.0f * 0.5f);
            const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
            const auto value = static_cast<std::int32_t>(alpha * sine * sine * sine);
            ramp[index] = (static_cast<std::uint32_t>(value) << 24U) | rgb;
            phase = static_cast<float>(static_cast<double>(phase) + 0.05);
        }
    } else {
        for (std::size_t index = 0; index < 32; ++index)
            ramp[index] = (ramp[index] & 0xff000000U) | rgb;
    }
}

void Weather_FreeColourRamp(WeatherNodeData *data) {
    if (!data)
        return;
    auto *ramp = data->ripple.colourRamp;
    if (!ramp)
        return;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(ramp), 0);
    data->ripple.colourRamp = static_cast<std::uint32_t *>(nullptr);
}

WeatherNodeData *AllocateGlobalWeatherData(std::size_t bytes, const Vector3 &center, float extent, float volumeFactor, bool initializeSentinels) {
    auto *const data = static_cast<WeatherNodeData *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(bytes), static_cast<std::int32_t>(SHEN_NAME(GLBS))));
    if (!data)
        return nullptr;
    std::memset(data, 0, bytes);
    data->volume = extent * extent * volumeFactor;
    const float halfExtent = extent * 0.5f;
    data->bounds.maximumY = center.y + halfExtent;
    data->bounds.minimumY = center.y - halfExtent;
    data->bounds.maximumX = center.x + halfExtent;
    data->bounds.minimumX = center.x - halfExtent;
    data->bounds.maximumZ = center.z + halfExtent;
    data->bounds.minimumZ = center.z - halfExtent;
    if (initializeSentinels) {
        reinterpret_cast<WeatherClippedVolume *>(data)->bounds.clips[0].maximumY = -100000.0f;
        reinterpret_cast<WeatherClippedVolume *>(data)->bounds.clips[1].maximumY = -100000.0f;
    }
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


WeatherNodeData *AllocateAuthoredWeatherVolume(const Vector3 &center, float sizeX, float sizeY, float sizeZ) {
    auto *const data =
        static_cast<WeatherNodeData *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(kRainWeatherBucketBytes), static_cast<std::int32_t>(SHEN_NAME(GLBS))));
    std::memset(data, 0, kRainWeatherBucketBytes);
    data->bounds.dimensionX = sizeX;
    data->bounds.dimensionY = sizeY;
    data->bounds.dimensionZ = sizeZ;
    data->volume = sizeX * sizeZ * sizeY;
    const float halfX = data->bounds.dimensionX * 0.5f;
    const float halfZ = data->bounds.dimensionZ * 0.5f;
    const float halfY = data->bounds.dimensionY * 0.5f;
    data->bounds.minimumX = center.x - halfX;
    data->bounds.maximumX = center.x + halfX;
    data->bounds.minimumY = center.y - halfY;
    data->bounds.maximumY = center.y + halfY;
    data->bounds.maximumZ = center.z + halfZ;
    data->bounds.minimumZ = center.z - halfZ;
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
    ++g_effectsTaskState;
    g_pGLBSTaskData = reinterpret_cast<GLBSWeatherDrawState *>(g_weatherNodeHead);
    return data;
}

WeatherFixc *AllocateWeatherFixc(std::int32_t type, std::int32_t sourceOffset, const Vector3 *center, float sizeX, float sizeY, float sizeZ) {
    const std::int32_t token = static_cast<std::int32_t>(type == 6 ? SHEN_NAME(FIXG) : SHEN_NAME(FIXC));
    auto *const fixc = static_cast<WeatherFixc *>(HLib_AllocHeapMemBlock(sizeof(WeatherFixc), token));
    if (!fixc)
        return nullptr;
    std::memset(fixc, 0, sizeof(*fixc));
    fixc->type = type;
    if (type == 6 && center) {
        fixc->data = AllocateAuthoredWeatherVolume(*center, sizeX, sizeY, sizeZ);
    } else {
        auto *const data = static_cast<WeatherCaraData *>(HLib_AllocHeapMemBlock(sizeof(WeatherCaraData), static_cast<std::int32_t>(SHEN_NAME(CARA))));
        if (data) {
            std::memset(data, 0, sizeof(*data));
            data->fieldOfView = sourceOffset;
        }
        fixc->data = data;
    }
    if (!fixc->data) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(fixc), 0);
        return nullptr;
    }
    return fixc;
}

ActiveWeatherNode *FindActiveWeatherNode(const WeatherFixc *fixc) {
    ActiveWeatherNode *node = g_activeWeatherNodes;
    while (node && node->weather != fixc)
        node = node->next;
    return node;
}

void AttachWeatherNode(WeatherFixc *fixc) {
    if (!fixc || !fixc->data || FindActiveWeatherNode(fixc))
        return;
    auto *const active = static_cast<ActiveWeatherNode *>(HLib_AllocHeapMemBlock(sizeof(ActiveWeatherNode), static_cast<std::int32_t>(SHEN_NAME(DLST))));
    if (!active)
        return;
    std::memset(active, 0, sizeof(*active));
    active->type = fixc->type;
    active->weather = fixc;
    if (!g_activeWeatherNodes) {
        g_activeWeatherNodes = active;
    } else {
        ActiveWeatherNode *tail = g_activeWeatherNodes;
        while (tail->next)
            tail = tail->next;
        tail->next = active;
        active->previous = tail;
    }
    Weather_AttachAndDriveSystemNode(fixc);
}

void AllocDLST(WeatherFixc &weather, WeatherWindData *wind) {
    if (!wind)
        return;
    for (WeatherWindLink *link = weather.windLinks; link; link = link->next) {
        if (link->wind == wind)
            return;
    }
    auto *const added = static_cast<WeatherWindLink *>(HLib_AllocHeapMemBlock(sizeof(WeatherWindLink), static_cast<std::int32_t>(SHEN_NAME(DLST))));
    *added = {.wind = wind};
    if (!weather.windLinks) {
        weather.windLinks = added;
        return;
    }
    WeatherWindLink *tail = weather.windLinks;
    while (tail->next)
        tail = tail->next;
    tail->next = added;
    added->previous = tail;
}

void Weather_FreeWeather(WeatherFixc &weather, WeatherWindData *wind) {
    WeatherWindLink *link = weather.windLinks;
    while (link && link->wind != wind)
        link = link->next;
    if (!link)
        return;
    if (weather.windLinks == link)
        weather.windLinks = link->next;
    if (link->previous)
        link->previous->next = link->next;
    if (link->next)
        link->next->previous = link->previous;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(link), 0);
}

void RemoveActiveWeatherNode(WeatherFixc *fixc) {
    if (ActiveWeatherNode *const active = FindActiveWeatherNode(fixc)) {
        if (active->previous)
            active->previous->next = active->next;
        else
            g_activeWeatherNodes = active->next;
        if (active->next)
            active->next->previous = active->previous;
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(active), 0);
    }
    Weather_FreeWeather(*fixc, g_weatherWindData);
}

// Unlink the bucket, then release its rain, snow and ripple pools before the node itself.
void FreeGlobalWeatherData(WeatherNodeData *data) {
    if (!data)
        return;
    if (data->next)
        data->next->backlink = data->backlink;
    if (data->backlink)
        *data->backlink = data->next;
    else
        g_weatherNodeHead = data->next;
    g_pGLBSTaskData = reinterpret_cast<GLBSWeatherDrawState *>(g_weatherNodeHead);
    if (g_effectsTaskState > 0)
        --g_effectsTaskState;
    FreeRainRenderData(data);
    FreeSnowRenderData(data);
    FreeRippleRenderData(data);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
}

void FreeWeatherFixc(WeatherFixc *fixc) {
    if (!fixc)
        return;
    RemoveActiveWeatherNode(fixc);
    if (g_tmpWeatherData == fixc) {
        g_tmpWeatherData = nullptr;
        g_effeWeatherFlag = false;
    }
    void *const allocation = fixc->data;
    fixc->data = nullptr;
    if (fixc->type == 6) {
        FreeGlobalWeatherData(static_cast<WeatherNodeData *>(allocation));
    } else {
        auto *const data = static_cast<WeatherCaraData *>(allocation);
        if (data) {
            FreeGlobalWeatherData(data->rainPrimary);
            FreeGlobalWeatherData(data->rainSecondary);
            FreeGlobalWeatherData(data->snowPrimary);
            FreeGlobalWeatherData(data->snowSecondary);
            FreeGlobalWeatherData(data->windSystem);
        }
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(allocation), 0);
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(fixc), 0);
}

std::uint32_t WeatherFixcOffset(const WeatherFixc *fixc) {
    if (!fixc || !g_pCurrentTaskData)
        return 0;
    const std::ptrdiff_t offset = reinterpret_cast<const std::byte *>(fixc) - g_pCurrentTaskData;
    return offset >= 0 && static_cast<std::uint64_t>(offset) <= (std::numeric_limits<std::uint32_t>::max)() ? static_cast<std::uint32_t>(offset) : 0;
}

void ApplyDampedCameraPushXZ(WeatherNodeData *group, const Vector3 &forces) {
    if (!group)
        return;
    const float damping = group->snow.damping;
    const float pushX = FlushWeatherEpsilon(group->snow.cameraPush.x);
    const float pushZ = FlushWeatherEpsilon(group->snow.cameraPush.z);
    const float z = (pushZ - damping * pushZ) + (forces.z + forces.z);
    group->snow.cameraPush.x = (pushX - damping * pushX) + (forces.x + forces.x);
    group->snow.cameraPush.z = z;
}

std::int16_t FoldWeatherAngle(std::int32_t angle) {
    return static_cast<std::uint16_t>(-angle) <= 0x8000U ? static_cast<std::int16_t>(-angle) : static_cast<std::int16_t>(angle);
}

void Bucket_UpdateOrientationFromCameraDelta(WeatherNodeData *group, const Vector3 &previousTarget, const Vector3 &target) {
    const Vector3 velocity = group->rain.velocity;
    const Vector3 anchor = group->rain.frameVelocity;
    float deltaX = -velocity.x - anchor.x;
    const float deltaY = (velocity.y + target.y) - previousTarget.y;
    float deltaZ = -velocity.z - anchor.z;
    group->rain.frameVelocity.y = deltaY;
    const float residual = (target.y - previousTarget.y) - deltaY;
    deltaX = FlushWeatherEpsilon(deltaX);
    deltaZ = FlushWeatherEpsilon(deltaZ);
    if (std::sqrt(deltaZ * deltaZ + deltaX * deltaX) >= 1.0f)
        return;
    const float smoothedX = FlushWeatherEpsilon(group->rain.smoothedVelocity.x);
    const float smoothedZ = FlushWeatherEpsilon(group->rain.smoothedVelocity.z);
    group->rain.smoothedVelocity.y = residual;
    const Vector3 delta{(deltaX - smoothedX) * 0.25f + smoothedX, residual, (deltaZ - smoothedZ) * 0.25f + smoothedZ};
    group->rain.smoothedVelocity.x = delta.x;
    group->rain.smoothedVelocity.z = delta.z;

    Matrix4x4 view = g_tmp_cam_xform;
    matrix_stack::Matrix4x4_Push(&view);
    const float viewY = view.rows[1].y * delta.y + view.rows[0].y * delta.x + view.rows[2].y * delta.z;
    const float viewZ = view.rows[1].z * delta.y + view.rows[0].z * delta.x + view.rows[2].z * delta.z;
    const float viewX = (view.rows[1].x * delta.y + view.rows[0].x * delta.x + view.rows[2].x * delta.z) * 0.0f;
    Matrix4x4::Pop(1);
    const float crossZ = viewY - viewX;
    const float crossY = viewX - viewZ;
    const float crossX = viewZ * 0.0f - viewY * 0.0f;
    Matrix4x4::InvertInPlace(&view);
    matrix_stack::Matrix4x4_Push(&view);
    const float worldX = view.rows[1].x * crossY + view.rows[0].x * crossX + view.rows[2].x * crossZ;
    const float worldY = view.rows[1].y * crossY + view.rows[0].y * crossX + view.rows[2].y * crossZ;
    const float worldZ = view.rows[1].z * crossY + view.rows[0].z * crossX + view.rows[2].z * crossZ;
    Matrix4x4::Pop(1);

    const std::int32_t pitch = -static_cast<std::int32_t>(atan2_fixed16(worldY, worldZ));
    group->rain.angleX = pitch;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundX(sin_fixed16(static_cast<std::int16_t>(-pitch)), sin_fixed16(static_cast<std::int16_t>(0x4000 - FoldWeatherAngle(pitch))));
    const Matrix4x4 &pitched = workTransformMatrix;
    const float yawX = pitched.rows[1].x * worldY + pitched.rows[0].x * worldX + pitched.rows[2].x * worldZ;
    const float yawZ = worldX * pitched.rows[0].z + pitched.rows[1].z * worldY + pitched.rows[2].z * worldZ;
    const float rotatedX = pitched.rows[1].x * delta.y + pitched.rows[0].x * delta.x + delta.z * pitched.rows[2].x;
    const float rotatedY = delta.x * pitched.rows[0].y + pitched.rows[1].y * delta.y + delta.z * pitched.rows[2].y;
    const float rotatedZ = delta.y * pitched.rows[1].z + delta.x * pitched.rows[0].z + delta.z * pitched.rows[2].z;
    Matrix4x4::Pop(1);

    const std::int32_t yaw = atan2_fixed16(yawX, yawZ);
    group->rain.angleY = yaw;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(-yaw)), sin_fixed16(static_cast<std::int16_t>(0x4000 - FoldWeatherAngle(yaw))));
    const Matrix4x4 &yawed = workTransformMatrix;
    const float rollX = yawed.rows[1].x * rotatedY + yawed.rows[0].x * rotatedX + rotatedZ * yawed.rows[2].x;
    const float rollY = rotatedX * yawed.rows[0].y + yawed.rows[1].y * rotatedY + rotatedZ * yawed.rows[2].y;
    Matrix4x4::Pop(1);
    group->rain.angleZ = static_cast<std::int32_t>(atan2_fixed16(rollY, rollX)) - 0x4000;
}

void SetRainParametersFromIntensity(WeatherNodeData *group, float intensity, std::int32_t layer) {
    float width = 0.002f;
    float height = 0.2f;
    Vector3 velocity{0.0f, -0.18f, 0.0f};
    if (intensity >= 0.5f) {
        const auto blend = static_cast<float>((static_cast<double>(intensity) - 0.5) + (static_cast<double>(intensity) - 0.5));
        width = (0.0035f - 0.002f) * blend + 0.002f;
        height = (0.15f - 0.2f) * blend + 0.2f;
        velocity.y = (-0.28f - -0.18f) * blend + -0.18f;
    }
    group->rain.width = width;
    group->rain.height = height;
    if (layer == 1) {
        group->rain.width = static_cast<float>(static_cast<double>(width) * 1.5);
        group->rain.height = height + height;
    }
    group->rain.velocity.z = velocity.z;
    group->rain.velocity.x = velocity.x;
    group->rain.velocity.y = velocity.y;
}

void PublishWeatherRenderCount(WeatherParticlePool &pool) {
    const bool live = pool.activeCount || pool.availableCount;
    pool.renderCount = live ? pool.capacity : 0;
}

std::int32_t ClampWeatherTarget(WeatherParticlePool &pool) {
    WeatherDataEntry *const entry = pool.fade;
    entry->intensity = FlushWeatherEpsilon(entry->intensity);
    const std::int32_t capacity = pool.capacity;
    std::int32_t target = static_cast<std::int32_t>(entry->intensity);
    if (target >= capacity)
        target = capacity - 1;
    pool.availableCount = target;
    return target;
}

void Weather_UpdateParticleForTypeFromTable(WeatherNodeData &data, const Vector3 &forces) {
    WeatherNodeData *const group = &data;
    if (group->rain.pool.records) {
        UpdateRainDrift(group, forces);
        Bucket_UpdateOrientationFromCameraDelta(group, g_weatherCameraState->previousTarget, g_weatherCameraState->target);
        const std::int64_t status = Weather_AdvanceWeatherEntry(group->rain.pool.fade);
        ClampWeatherTarget(group->rain.pool);
        if (!status && (group->rain.flags & 1U) != 0)
            ResetRainGroupParticles(group, std::bit_cast<RainRandomFunction>(&SpawnVolumeRainParticleNative), data.bounds.maximumY - data.bounds.minimumY);
        PublishWeatherRenderCount(group->rain.pool);
    }
    if (group->snow.pool.records) {
        ApplyDampedCameraPushXZ(group, forces);
        const std::int64_t status = Weather_AdvanceWeatherEntry(group->snow.pool.fade);
        ClampWeatherTarget(group->snow.pool);
        if (!status && (group->snow.flags & 1U) != 0) {
            group->snow.flags = group->snow.flags & ~1U;
            group->snow.pool.activeCount = 0;
            auto *particle = reinterpret_cast<RainParticleNative *>(group->snow.pool.records);
            const std::int32_t available = group->snow.pool.availableCount;
            const std::int32_t capacity = group->snow.pool.capacity;
            const auto generator = std::bit_cast<RainRandomFunction>(&GenerateSnowParticleNative);
            std::int32_t index = 0;
            if (available > 0) {
                do {
                    generator(particle++, &data);
                    ++index;
                } while (group->snow.pool.availableCount > 0);
            }
            for (; index < capacity; ++particle, ++index) {
                particle->flags = 0x40000000U;
                particle->random = generator;
            }
            const float fall = -group->snow.velocity.y;
            std::int32_t target = static_cast<std::int32_t>(static_cast<float>(available) * fall / (data.bounds.maximumY - data.bounds.minimumY));
            if (target >= capacity)
                target = capacity - 1;
            group->snow.pool.availableCount = target;
            group->snow.fallHeight = fall;
            *group->snow.pool.fade = {.intensity = static_cast<float>(target), .targetIntensity = static_cast<float>(target)};
        }
        PublishWeatherRenderCount(group->snow.pool);
    }
    if (group->ripple.pool.records) {
        group->ripple.cameraEyeY = g_weatherCameraState->previousEye.y;
        Weather_AdvanceWeatherEntry(group->ripple.pool.fade);
        const std::int32_t available = ClampWeatherTarget(group->ripple.pool);
        group->ripple.pool.renderCount = available ? group->ripple.pool.capacity : 0;
    }
}

void Weather_DispatchParticlesForZone(WeatherFixc &weather) {
    Vector3 forces{};
    for (const WeatherWindLink *link = weather.windLinks; link; link = link->next) {
        forces.x += link->wind->gustVectorZ;
        forces.z += link->wind->gustVectorX;
    }
    if (weather.type == 0)
        Weather_UpdateSnowParticles(*static_cast<WeatherCaraData *>(weather.data), forces);
    else if (weather.type == 1)
        Weather_BuildCullBoxAndDispatch(*static_cast<WeatherCaraData *>(weather.data), forces);
    else if (weather.type == 6)
        Weather_UpdateParticleForTypeFromTable(*static_cast<WeatherNodeData *>(weather.data), forces);
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_AttachWindData(void *windData) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next)
        AllocDLST(*node->weather, static_cast<WeatherWindData *>(windData));
}

void EFFE_DetachWindData(void *windData) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next)
        Weather_FreeWeather(*node->weather, static_cast<WeatherWindData *>(windData));
}

void EFFE_ResetWccpClipSet(std::uint32_t setIndex) {
    if (setIndex > 1)
        return;

    if (setIndex == 0) {
        for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
            if (!node->weather || node->weather->type != 1 || !node->weather->data)
                continue;
            const auto &weather = *static_cast<const WeatherCaraData *>(node->weather->data);
            ResetWccpClipBounds(weather.rainPrimary ? &reinterpret_cast<WeatherClippedVolume *>(weather.rainPrimary)->bounds.clips[setIndex] : nullptr);
            ResetWccpClipBounds(weather.rainSecondary ? &reinterpret_cast<WeatherClippedVolume *>(weather.rainSecondary)->bounds.clips[setIndex] : nullptr);
        }
        for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
            if (!node->weather || node->weather->type != 1 || !node->weather->data)
                continue;
            const auto &weather = *static_cast<const WeatherCaraData *>(node->weather->data);
            ResetWccpClipBounds(weather.snowPrimary ? &reinterpret_cast<WeatherClippedSnowSphere *>(weather.snowPrimary)->clips[setIndex] : nullptr);
            ResetWccpClipBounds(weather.snowSecondary ? &reinterpret_cast<WeatherClippedSnowSphere *>(weather.snowSecondary)->clips[setIndex] : nullptr);
        }
        for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
            if (!node->weather || node->weather->type != 1 || !node->weather->data)
                continue;
            const auto &weather = *static_cast<const WeatherCaraData *>(node->weather->data);
            ResetWccpClipBounds(weather.windSystem ? &reinterpret_cast<WeatherClippedVolume *>(weather.windSystem)->bounds.clips[setIndex] : nullptr);
            ResetWccpClipBounds(weather.windSecondary ? &reinterpret_cast<WeatherClippedVolume *>(weather.windSecondary)->bounds.clips[setIndex] : nullptr);
        }
        return;
    }

    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        if (!node->weather || node->weather->type != 1 || !node->weather->data)
            continue;
        const auto &weather = *static_cast<const WeatherCaraData *>(node->weather->data);
        ResetWccpClipBounds(weather.rainPrimary ? &reinterpret_cast<WeatherClippedVolume *>(weather.rainPrimary)->bounds.clips[setIndex] : nullptr);
        ResetWccpClipBounds(weather.rainSecondary ? &reinterpret_cast<WeatherClippedVolume *>(weather.rainSecondary)->bounds.clips[setIndex] : nullptr);
        ResetWccpClipBounds(weather.snowPrimary ? &reinterpret_cast<WeatherClippedSnowSphere *>(weather.snowPrimary)->clips[setIndex] : nullptr);
        ResetWccpClipBounds(weather.snowSecondary ? &reinterpret_cast<WeatherClippedSnowSphere *>(weather.snowSecondary)->clips[setIndex] : nullptr);
        ResetWccpClipBounds(weather.windSystem ? &reinterpret_cast<WeatherClippedVolume *>(weather.windSystem)->bounds.clips[setIndex] : nullptr);
        ResetWccpClipBounds(weather.windSecondary ? &reinterpret_cast<WeatherClippedVolume *>(weather.windSecondary)->bounds.clips[setIndex] : nullptr);
    }
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void ApplyWccpClipQuad(WeatherClipBounds *bounds, const Vector2 (&corners)[4], float minimumY, float maximumY) {
    if (!bounds)
        return;
    for (std::size_t corner = 0; corner < 4; ++corner) {
        bounds->cornerX[corner] = corners[corner].x;
        bounds->cornerZ[corner] = corners[corner].y;
    }
    bounds->minimumY = minimumY;
    bounds->maximumY = maximumY;
    for (std::size_t corner = 0; corner < 4; ++corner) {
        const Vector2 &from = corners[corner];
        const Vector2 &to = corners[(corner + 1) & 3];
        bounds->edgeNormal[corner].y = to.x - from.x;
        bounds->edgeNormal[corner].x = to.y - from.y;
    }
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_ApplyWccpClipQuad(std::uint32_t setIndex, std::uint32_t groupMask, const Vector2 (&corners)[4], float minimumY, float maximumY) {
    const std::size_t clipIndex = setIndex == 0 ? 0 : 1;
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        if (node->weather->type != 1)
            continue;
        const auto &weather = *static_cast<const WeatherCaraData *>(node->weather->data);
        if (groupMask & 1U) {
            ApplyWccpClipQuad(weather.rainPrimary ? &reinterpret_cast<WeatherClippedVolume *>(weather.rainPrimary)->bounds.clips[clipIndex] : nullptr, corners,
                minimumY, maximumY);
            ApplyWccpClipQuad(weather.rainSecondary ? &reinterpret_cast<WeatherClippedVolume *>(weather.rainSecondary)->bounds.clips[clipIndex] : nullptr,
                corners, minimumY, maximumY);
        }
        if (groupMask & 2U) {
            ApplyWccpClipQuad(weather.snowPrimary ? &reinterpret_cast<WeatherClippedSnowSphere *>(weather.snowPrimary)->clips[clipIndex] : nullptr, corners,
                minimumY, maximumY);
            ApplyWccpClipQuad(weather.snowSecondary ? &reinterpret_cast<WeatherClippedSnowSphere *>(weather.snowSecondary)->clips[clipIndex] : nullptr, corners,
                minimumY, maximumY);
        }
        if (groupMask & 4U) {
            ApplyWccpClipQuad(weather.windSystem ? &reinterpret_cast<WeatherClippedVolume *>(weather.windSystem)->bounds.clips[clipIndex] : nullptr, corners,
                minimumY, maximumY);
            ApplyWccpClipQuad(weather.windSecondary ? &reinterpret_cast<WeatherClippedVolume *>(weather.windSecondary)->bounds.clips[clipIndex] : nullptr,
                corners, minimumY, maximumY);
        }
    }
}

void AllocFIXC() {
    if (g_effeWeatherFlag)
        return;
    if (!g_tmpWeatherData)
        g_tmpWeatherData = AllocateWeatherFixc(1, 14563);
    AttachWeatherNode(g_tmpWeatherData);
    g_effeWeatherFlag = true;
    EFFE_ProcessWeather(1.5f);
    Snow_TickSystemsInit(1.15f);
    Weather_TickPerSystemValues(0.95f);
}

void Weather_SetForcedRainValue(float intensity) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        const WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            const auto &data = *static_cast<const WeatherCaraData *>(weather.data);
            Weather_SetRainLayerCount(data.rainPrimary, intensity);
            Weather_SetRainLayerCount(data.rainSecondary, intensity);
        } else if (weather.type == 6) {
            Weather_SetRainLayerCount(static_cast<WeatherNodeData *>(weather.data), intensity);
        }
    }
    *g_activeRainEntry = {.intensity = intensity, .targetIntensity = intensity};
}

void EFFE_ApplyRainGameData(float intensity, std::int32_t stepCount) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        const WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            const auto &data = *static_cast<const WeatherCaraData *>(weather.data);
            Weather_ProcessRainIntensityForData(data.rainPrimary, intensity, stepCount);
            Weather_ProcessRainIntensityForData(data.rainSecondary, intensity, stepCount);
        } else if (weather.type == 6) {
            Weather_ProcessRainIntensityForData(static_cast<WeatherNodeData *>(weather.data), intensity, stepCount);
        }
    }
}

void Weather_SetForcedSnowValue(float intensity) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        const WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1)
            Weather_SetSnowGroupCount(*static_cast<const WeatherCaraData *>(weather.data), intensity);
        else if (weather.type == 6)
            Weather_SetSnowLayerCount(static_cast<WeatherNodeData *>(weather.data), intensity);
    }
    *g_activeSnowEntry = {.intensity = intensity, .targetIntensity = intensity};
}

void Weather_ApplySnowGameDataForWeather(const WeatherFixc &weather, float intensity, std::int32_t stepCount) {
    if (weather.type == 0 || weather.type == 1)
        Weather_UpdateSnowGroupIntensity(*static_cast<WeatherCaraData *>(weather.data), intensity, stepCount);
    else if (weather.type == 6)
        Weather_UpdateSnowIntensityForData(static_cast<WeatherNodeData *>(weather.data), intensity, stepCount);
}

void EFFE_ApplySnowGameData(float intensity, std::int32_t stepCount) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next)
        Weather_ApplySnowGameDataForWeather(*node->weather, intensity, stepCount);
}

void EFFE_ApplyWindGameData(float intensity, std::int32_t stepCount) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next)
        Weather_ApplyRippleGameData(*node->weather, intensity, stepCount);
}

void EFFE_InitializeSnowWeatherSystems(float intensity) {
    Snow_TickSystemsInit(intensity);
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void Weather_SetRainParticleSize(const WeatherFixc &weather, float width, float height) {
    if (weather.type == 0 || weather.type == 1) {
        const auto &data = *static_cast<const WeatherCaraData *>(weather.data);
        if (data.rainPrimary) {
            data.rainPrimary->rain.width = width;
            data.rainPrimary->rain.height = height;
        }
        if (data.rainSecondary) {
            data.rainSecondary->rain.height = static_cast<float>(static_cast<double>(height) * 0.17 / 0.152);
            data.rainSecondary->rain.width = width;
        }
    } else if (weather.type == 6 && weather.data) {
        static_cast<WeatherNodeData *>(weather.data)->rain.height = height;
        static_cast<WeatherNodeData *>(weather.data)->rain.width = width;
    }
}

void Weather_SetRainEffectVectors(const WeatherCaraData &data, const Vector3 &vector0, const Vector3 &vector1) {
    for (WeatherNodeData *const rain : {data.rainPrimary, data.rainSecondary}) {
        if (!rain)
            continue;
        rain->rain.velocity = vector0;
        rain->rain.velocityRange = vector1;
    }
}

void Weather_SetSnowEffectVectors(const WeatherCaraData &data, const Vector3 &vector0, const Vector3 &vector1) {
    for (WeatherNodeData *const snow : {data.snowPrimary, data.snowSecondary}) {
        if (!snow)
            continue;
        snow->snow.velocity = vector0;
        snow->snow.velocityRange = vector1;
    }
}

void FreeRainRenderData(WeatherNodeData *rain) {
    if (!rain || !rain->rain.pool.records)
        return;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(rain->rain.pool.fade), static_cast<std::int32_t>(SHEN_NAME(LFLT)));
    const std::uintptr_t key = reinterpret_cast<std::uintptr_t>(rain->rain.pool.records);
    (void)ReleaseHmdlRuntimeObject(key);
    (void)ReleaseHmdlRuntimeObject(key + 1);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(rain->rain.pool.records), 0);
    rain->rain.pool.records = static_cast<void *>(nullptr);
    rain->rain.flags = std::uint32_t{0};
}

void FreeSnowRenderData(WeatherNodeData *snow) {
    if (!snow || !snow->snow.pool.records)
        return;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(snow->snow.pool.fade), static_cast<std::int32_t>(SHEN_NAME(LFLT)));
    const std::uintptr_t key = reinterpret_cast<std::uintptr_t>(snow->snow.pool.records);
    (void)ReleaseHmdlRuntimeObject(key);
    (void)ReleaseHmdlRuntimeObject(key + 1);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(snow->snow.pool.records), 0);
    if (snow->snow.textureReference.texture)
        snow->snow.textureReference.texture = nullptr;
    snow->snow.pool.records = static_cast<void *>(nullptr);
    snow->snow.flags = std::uint32_t{0};
}

void Weather_AttachAndDriveSystemNode(WeatherFixc *weather) {
    const bool cara = weather->type == 0 || weather->type == 1;
    auto *const data = cara ? static_cast<WeatherCaraData *>(weather->data) : nullptr;
    WeatherNodeData *const rain = cara ? data->rainPrimary : static_cast<WeatherNodeData *>(weather->data);
    if (rain && rain->rain.flags) {
        if (std::fabs(g_activeRainEntry->intensity) < std::numeric_limits<float>::epsilon())
            g_activeRainEntry->intensity = 0.0f;
        if (cara) {
            Weather_SetRainLayerCount(data->rainPrimary, g_activeRainEntry->intensity);
            Weather_SetRainLayerCount(data->rainSecondary, g_activeRainEntry->intensity);
        } else if (weather->type == 6) {
            Weather_SetRainLayerCount(static_cast<WeatherNodeData *>(weather->data), g_activeRainEntry->intensity);
        }
        if (g_activeRainEntry->currentStep != g_activeRainEntry->maxStepCount) {
            const std::int32_t remaining = g_activeRainEntry->maxStepCount - g_activeRainEntry->currentStep;
            if (std::fabs(g_activeRainEntry->targetIntensity) < std::numeric_limits<float>::epsilon())
                g_activeRainEntry->targetIntensity = 0.0f;
            if (cara) {
                Weather_ProcessRainIntensityForData(data->rainPrimary, g_activeRainEntry->targetIntensity, remaining);
                Weather_ProcessRainIntensityForData(data->rainSecondary, g_activeRainEntry->targetIntensity, remaining);
            } else if (weather->type == 6) {
                Weather_ProcessRainIntensityForData(static_cast<WeatherNodeData *>(weather->data), g_activeRainEntry->targetIntensity, remaining);
            }
        }
        if (cara) {
            if (data->rainPrimary)
                data->rainPrimary->rain.colour = g_rainEffectOffset;
            if (data->rainSecondary)
                data->rainSecondary->rain.colour = g_rainEffectOffset;
        } else if (weather->type == 6 && weather->data) {
            static_cast<WeatherNodeData *>(weather->data)->rain.colour = g_rainEffectOffset;
        }
        Weather_SetRainParticleSize(*weather, g_rainParticleWidth, g_rainParticleHeight);
        if (cara) {
            Weather_SetRainEffectVectors(*data, g_rainEffectVector0, g_rainEffectVector1);
        } else if (weather->type == 6 && weather->data) {
            static_cast<WeatherNodeData *>(weather->data)->rain.velocity = g_rainEffectVector0;
            static_cast<WeatherNodeData *>(weather->data)->rain.velocityRange = g_rainEffectVector1;
        }
    }
    WeatherNodeData *const snow = cara ? data->snowPrimary : static_cast<WeatherNodeData *>(weather->data);
    if (snow && std::bit_cast<float>(snow->snow.flags) != 0.0f) {
        if (std::fabs(g_activeSnowEntry->intensity) < std::numeric_limits<float>::epsilon())
            g_activeSnowEntry->intensity = 0.0f;
        if (cara)
            Weather_SetSnowGroupCount(*data, g_activeSnowEntry->intensity);
        else if (weather->type == 6)
            Weather_SetSnowLayerCount(static_cast<WeatherNodeData *>(weather->data), g_activeSnowEntry->intensity);
        if (g_activeSnowEntry->currentStep != g_activeSnowEntry->maxStepCount) {
            const std::int32_t remaining = g_activeSnowEntry->maxStepCount - g_activeSnowEntry->currentStep;
            if (std::fabs(g_activeSnowEntry->targetIntensity) < std::numeric_limits<float>::epsilon())
                g_activeSnowEntry->targetIntensity = 0.0f;
            Weather_ApplySnowGameDataForWeather(*weather, g_activeSnowEntry->targetIntensity, remaining);
        }
        for (WeatherNodeData *const layer : {cara ? data->snowPrimary :
                     weather->type == 6           ? static_cast<WeatherNodeData *>(weather->data) :
                                                    nullptr,
                 cara ? data->snowSecondary : nullptr}) {
            if (layer)
                layer->snow.colour = g_rainUpdateOffset;
        }
        for (WeatherNodeData *const layer : {cara ? data->snowPrimary :
                     weather->type == 6           ? static_cast<WeatherNodeData *>(weather->data) :
                                                    nullptr,
                 cara ? data->snowSecondary : nullptr}) {
            if (layer)
                layer->snow.size = g_snowParticleSize;
        }
        if (cara) {
            Weather_SetSnowEffectVectors(*data, g_snowEffectVector0, g_snowEffectVector1);
        } else if (weather->type == 6 && weather->data) {
            static_cast<WeatherNodeData *>(weather->data)->snow.velocity = g_snowEffectVector0;
            static_cast<WeatherNodeData *>(weather->data)->snow.velocityRange = g_snowEffectVector1;
        }
    }
    WeatherNodeData *const ripple = cara ? data->windSystem : static_cast<WeatherNodeData *>(weather->data);
    if (ripple && std::bit_cast<float>(ripple->ripple.flags) != 0.0f) {
        if (std::fabs(g_activeWindEntry->intensity) < std::numeric_limits<float>::epsilon())
            g_activeWindEntry->intensity = 0.0f;
        Weather_SetRippleCount(*weather, g_activeWindEntry->intensity);
        if (g_activeWindEntry->currentStep != g_activeWindEntry->maxStepCount) {
            const std::int32_t remaining = g_activeWindEntry->maxStepCount - g_activeWindEntry->currentStep;
            if (std::fabs(g_activeWindEntry->targetIntensity) < std::numeric_limits<float>::epsilon())
                g_activeWindEntry->targetIntensity = 0.0f;
            Weather_ApplyRippleGameData(*weather, g_activeWindEntry->targetIntensity, remaining);
        }
        if (WeatherNodeData *const group = WeatherRippleGroup(*weather)) {
            Weather_UpdateColourRampForData(group, g_weatherColourRampValue);
            group->ripple.size = g_rippleParticleSize;
        }
    }
    if (g_weatherWindData)
        AllocDLST(*weather, g_weatherWindData);
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_ProcessWeather(float intensity) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        if (node->weather->type == 0)
            InitRainParticles(*static_cast<WeatherCaraData *>(node->weather->data), intensity);
        else if (node->weather->type == 1)
            BuildRainLayersForFov(*static_cast<WeatherCaraData *>(node->weather->data), intensity);
        else if (node->weather->type == 6)
            InitializeRainLayer(
                *static_cast<WeatherNodeData *>(node->weather->data), intensity, std::bit_cast<RainRandomFunction>(&SpawnVolumeRainParticleNative));
    }
}

void Weather_TickPerSystemValues(float intensity) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        WeatherFixc &weather = *node->weather;
        if (weather.type == 0) {
            BuildCaraRippleLayer(*static_cast<WeatherCaraData *>(weather.data), intensity, kRainWeatherBucketBytes, false,
                std::bit_cast<RainRandomFunction>(&EvaluateRainImpactLocation_0));
        } else if (weather.type == 1) {
            BuildCaraRippleLayer(*static_cast<WeatherCaraData *>(weather.data), intensity, kGlobalWeatherBucketBytes, true,
                std::bit_cast<RainRandomFunction>(&SpawnRandomRainImpactCandidate));
        } else if (weather.type == 6) {
            InitializeRippleLayer(*static_cast<WeatherNodeData *>(weather.data), intensity, std::bit_cast<RainRandomFunction>(&Rain_CalcPRNGWithCallback));
        }
    }
}


void CreateDDSorPVR(const void *file) {
    const auto *pvr = static_cast<const EffePvrHeader *>(file);
    if (pvr->magic == 0x58494247U)
        pvr = reinterpret_cast<const EffePvrHeader *>(static_cast<const std::byte *>(file) + 0x0c);
    const std::uint32_t handle = CreatePvrEffeTexture(pvr);
    if (handle)
        g_weatherSnowTexture = {.handle = handle, .width = pvr->width, .height = pvr->height};
}

// Shenmue_InitSpriteBank's w_hamon.pvr branch: a fixed 64x64 ripple sprite.
void EFFE_CreateHamonTexture(const void *file) {
    const auto *pvr = static_cast<const EffePvrHeader *>(file);
    if (pvr->magic == 0x58494247U)
        pvr = reinterpret_cast<const EffePvrHeader *>(static_cast<const std::byte *>(file) + 0x0c);
    const std::uint32_t handle = CreatePvrEffeTexture(pvr);
    if (handle)
        g_weatherHamonTexture = {.handle = handle, .width = 64, .height = 64};
}

void EFFE_FreeRainRenderData() {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            auto &data = *static_cast<WeatherCaraData *>(weather.data);
            FreeRainRenderData(data.rainPrimary);
            data.rainPrimary = nullptr;
            FreeRainRenderData(data.rainSecondary);
            data.rainSecondary = nullptr;
        } else if (weather.type == 6) {
            FreeRainRenderData(static_cast<WeatherNodeData *>(weather.data));
        }
    }
}

void EFFE_SetRainParticleSize(float width, float height) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next)
        Weather_SetRainParticleSize(*node->weather, width, height);
    g_rainParticleHeight = height;
    g_rainParticleWidth = width;
}

void EFFE_FreeSnowRenderData() {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            auto &data = *static_cast<WeatherCaraData *>(weather.data);
            FreeSnowRenderData(data.snowPrimary);
            data.snowPrimary = nullptr;
            FreeSnowRenderData(data.snowSecondary);
            data.snowSecondary = nullptr;
        } else if (weather.type == 6) {
            FreeSnowRenderData(static_cast<WeatherNodeData *>(weather.data));
        }
    }
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void FreeRippleRenderData(WeatherNodeData *ripple) {
    if (!ripple)
        return;
    if (void *const ramp = reinterpret_cast<void *>(ripple->ripple.colourRamp))
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(ramp), 0);
    if (!ripple->ripple.pool.records)
        return;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(ripple->ripple.pool.fade), static_cast<std::int32_t>(SHEN_NAME(LFLT)));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(ripple->ripple.pool.records), 0);
    if (ripple->ripple.textureReference.words[0])
        ripple->ripple.textureReference.words[0] = std::uint32_t{0};
    ripple->ripple.pool.records = static_cast<void *>(nullptr);
    ripple->ripple.flags = std::uint32_t{0};
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_FreeRippleRenderData() {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            auto &data = *static_cast<WeatherCaraData *>(weather.data);
            FreeRippleRenderData(data.windSystem);
            data.windSystem = nullptr;
        } else if (weather.type == 6) {
            FreeRippleRenderData(static_cast<WeatherNodeData *>(weather.data));
        }
    }
}

void EFFE_SetSnowParticleSize(float size) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        const WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            const auto &data = *static_cast<const WeatherCaraData *>(weather.data);
            if (data.snowPrimary)
                data.snowPrimary->snow.size = size;
            if (data.snowSecondary)
                data.snowSecondary->snow.size = size;
        } else if (weather.type == 6 && weather.data) {
            static_cast<WeatherNodeData *>(weather.data)->snow.size = size;
        }
    }
    g_snowParticleSize = size;
}

void EFFE_SetRippleParticleSize(float size) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        const WeatherFixc &weather = *node->weather;
        WeatherNodeData *ripple = nullptr;
        if (weather.type == 0 || weather.type == 1)
            ripple = static_cast<const WeatherCaraData *>(weather.data)->windSystem;
        else if (weather.type == 6)
            ripple = static_cast<WeatherNodeData *>(weather.data);
        if (ripple)
            ripple->ripple.size = size;
    }
    g_rippleParticleSize = size;
}

void EFFE_SetRainEffectVectors(const Vector3 &vector0, const Vector3 &vector1) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        const WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            Weather_SetRainEffectVectors(*static_cast<const WeatherCaraData *>(weather.data), vector0, vector1);
        } else if (weather.type == 6 && weather.data) {
            static_cast<WeatherNodeData *>(weather.data)->rain.velocity = vector0;
            static_cast<WeatherNodeData *>(weather.data)->rain.velocityRange = vector1;
        }
    }
    g_rainEffectVector0 = vector0;
    g_rainEffectVector1 = vector1;
}

void EFFE_SetSnowEffectVectors(const Vector3 &vector0, const Vector3 &vector1) {
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next) {
        const WeatherFixc &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            Weather_SetSnowEffectVectors(*static_cast<const WeatherCaraData *>(weather.data), vector0, vector1);
        } else if (weather.type == 6 && weather.data) {
            static_cast<WeatherNodeData *>(weather.data)->snow.velocity = vector0;
            static_cast<WeatherNodeData *>(weather.data)->snow.velocityRange = vector1;
        }
    }
    g_snowEffectVector0 = vector0;
    g_snowEffectVector1 = vector1;
}

void SetRainEffectOffset(std::uint32_t colour) {
    for (auto *node = g_activeWeatherNodes; node; node = node->next) {
        auto &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            auto &data = *static_cast<WeatherCaraData *>(weather.data);
            if (data.rainPrimary)
                data.rainPrimary->rain.colour = colour;
            if (data.rainSecondary)
                data.rainSecondary->rain.colour = colour;
        } else if (weather.type == 6 && weather.data) {
            static_cast<WeatherNodeData *>(weather.data)->rain.colour = colour;
        }
    }
    g_rainEffectOffset = colour;
}

void Weather_UpdateRainOffset(std::uint32_t colour) {
    for (auto *node = g_activeWeatherNodes; node; node = node->next) {
        auto &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1) {
            auto &data = *static_cast<WeatherCaraData *>(weather.data);
            if (data.snowPrimary)
                data.snowPrimary->snow.colour = colour;
            if (data.snowSecondary)
                data.snowSecondary->snow.colour = colour;
        } else if (weather.type == 6 && weather.data) {
            static_cast<WeatherNodeData *>(weather.data)->snow.colour = colour;
        }
    }
    g_rainUpdateOffset = colour;
}

void Weather_UpdateColourRamp(std::uint32_t colour) {
    for (auto *node = g_activeWeatherNodes; node; node = node->next) {
        auto &weather = *node->weather;
        if (weather.type == 0 || weather.type == 1)
            Weather_UpdateColourRampForData(static_cast<WeatherCaraData *>(weather.data)->windSystem, colour);
        else if (weather.type == 6)
            Weather_UpdateColourRampForData(static_cast<WeatherNodeData *>(weather.data), colour);
    }
    g_weatherColourRampValue = colour;
}

void EFFE_UpdateWeatherDrawBits(std::int32_t shouldDraw) {
    const std::uint32_t drawBit = shouldDraw ? 0x80000000U : 0U;
    for (GLBSWeatherDrawState *state = g_pGLBSTaskData; state;) {
        state->drawFlags10 = (state->drawFlags10 & ~0x80000000U) | drawBit;
        state->drawFlags9c = (state->drawFlags9c & ~0x80000000U) | drawBit;
        state->drawFlags110 = (state->drawFlags110 & ~0x80000000U) | drawBit;
        state = reinterpret_cast<GLBSWeatherDrawState *>(static_cast<std::uintptr_t>(state->next));
    }
}

void EFFE_UpdateWeatherState() {
    Weather_AdvanceWeatherEntry(g_activeRainEntry);
    Weather_AdvanceWeatherEntry(g_activeSnowEntry);
    Weather_AdvanceWeatherEntry(g_activeWindEntry);
    for (ActiveWeatherNode *node = g_activeWeatherNodes; node; node = node->next)
        Weather_DispatchParticlesForZone(*node->weather);
    if (g_activeRainEntry && std::fabs(g_activeRainEntry->intensity) < std::numeric_limits<float>::epsilon())
        g_activeRainEntry->intensity = 0.0f;
}

void EFFE_SetSnowParticleBudgetEnabled(bool enabled) {
    g_bForceDisableSnow = enabled ? 1 : 0;
}

bool EFFE_IsSnowParticleBudgetEnabled() {
    return g_bForceDisableSnow != 0;
}

std::int32_t EFFE_GetWeatherSpriteLoadPolicy() {
    return g_shouldLoadWeatherSprites;
}

} // namespace shenmue::s1
