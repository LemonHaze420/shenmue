#include "s2/weather_particles_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/camera_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/weather_dc.hpp"
#include <cstring>
#include "s2/scnw_dc.hpp"

namespace shenmue {
namespace s2 {

void ReadWeatherClipData(const void *data);

void ReadWeatherClipData(const void *data);
void ApplyWeatherClipSet0ToFixcRain(
    WeatherFixcOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1);
void ApplyWeatherClipSet0ToFixcRipple(
    WeatherFixcOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyWeatherRainOwnerClipMode1(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1);
void ApplyWeatherRippleOwnerClipMode1(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1);
#endif

WeatherClipSet weatherClipSets[2];

namespace event_runtime_detail {
ScenarioWindowRecord *scenarioWindowRecord = 0;
ScenarioWindowRecord **g_pSCNW = 0;
} // namespace event_runtime_detail

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {
#endif

// @verified - d
void SetWeatherClipPlanes(
    WeatherParticleGroup *group, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    if (!group)
        return;
    group->clipB.x[0] = first->x;
    group->clipB.z[0] = first->y;
    group->clipB.x[1] = second->x;
    group->clipB.z[1] = second->y;
    group->clipB.x[2] = third->x;
    group->clipB.z[2] = third->y;
    group->clipB.x[3] = fourth->x;
    group->clipB.z[3] = fourth->y;
    group->clipB.clipY0 = clipY0;
    group->clipB.clipY1 = clipY1;
    group->clipB.edgeZ[1] = second->x - first->x;
    group->clipB.edgeZ[3] = third->x - second->x;
    group->clipB.edgeX[1] = fourth->x - third->x;
    group->clipB.edgeX[3] = first->x - fourth->x;
    group->clipB.edgeZ[0] = second->y - first->y;
    group->clipB.edgeZ[2] = third->y - second->y;
    group->clipB.edgeX[0] = fourth->y - third->y;
    group->clipB.edgeX[2] = first->y - fourth->y;
}

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
} // namespace
#endif

namespace {

void ApplyWeatherClipToRain(
    WeatherFixcOwner *fixc, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    if (fixc->mode != 1)
        return;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    ApplyWeatherRainOwnerClipMode1(static_cast<WeatherRainOwner *>(fixc->cara), first, second, third, fourth, clipY0, clipY1);
#else
    WeatherRainOwner *const owner = static_cast<WeatherRainOwner *>(fixc->cara);
    SetWeatherClipPlanes(owner->rain0, first, second, third, fourth, clipY0, clipY1);
    SetWeatherClipPlanes(owner->rain1, first, second, third, fourth, clipY0, clipY1);
#endif
}

} // namespace

namespace {

void ApplyWeatherClipToRipple(
    WeatherFixcOwner *fixc, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    if (fixc->mode != 1)
        return;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    ApplyWeatherRippleOwnerClipMode1(static_cast<WeatherRainOwner *>(fixc->cara), first, second, third, fourth, clipY0, clipY1);
#else
    WeatherRainOwner *const owner = static_cast<WeatherRainOwner *>(fixc->cara);
    SetWeatherClipPlanes(owner->rippleGroup, first, second, third, fourth, clipY0, clipY1);
    SetWeatherClipPlanes(owner->rippleGroup1, first, second, third, fourth, clipY0, clipY1);
#endif
}

} // namespace

namespace {

void ApplyWeatherClipSet0ToRain(const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    clipY0 -= 0.1f;
    clipY1 += 0.1f;
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        ApplyWeatherClipSet0ToFixcRain(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), first, second, third, fourth, clipY0, clipY1);
}

} // namespace

namespace {

void ApplyWeatherClipSet0ToRipple(const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    clipY0 -= 0.1f;
    clipY1 += 0.1f;
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        ApplyWeatherClipSet0ToFixcRipple(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), first, second, third, fourth, clipY0, clipY1);
}

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetWeatherRipplePressureImmediate(float pressure) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        SetWeatherFixcRippleCountImmediate(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), pressure);
    lfltSetImmediate(g_weatherExecution.rippleCount, pressure);
}
#endif

namespace {

void ApplyWeatherClipSet1(const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    clipY0 -= 0.1f;
    clipY1 += 0.1f;
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node)) {
        ApplyWeatherClipToRain(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), first, second, third, fourth, clipY0, clipY1);
        ApplyWeatherClipToRipple(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), first, second, third, fourth, clipY0, clipY1);
    }
}

} // namespace

namespace {

#if SHENMUE_S2_GCC295 && (SHENMUE_IS_DC && SHENMUE_IS_S2)
struct WeatherClipPayload {
    WeatherClipVolume volume;
    Vector3 center;
    float scale;
};

typedef char WeatherClipPayloadSizeCheck[sizeof(WeatherClipPayload) == sizeof(WeatherClipEntry) - sizeof(std::uint32_t) ? 1 : -1];
typedef char
    WeatherClipPayloadCenterCheck[offsetof(WeatherClipPayload, center) + offsetof(WeatherClipEntry, points) == offsetof(WeatherClipEntry, center) ? 1 : -1];

inline const WeatherClipPayload *SelectWeatherClipEntry(std::uint32_t count, const WeatherClipEntry *entry) {
    float nearest = 1.0e10f;
    const WeatherClipPayload *selected = 0;
    while (count-- != 0) {
        const WeatherClipPayload *const payload = reinterpret_cast<const WeatherClipPayload *>(entry->points);
        if (entry->enabled && ScreenVisCheck(entry->center, entry->scale, 32.0f)) {
            const float dx = entry->center.x - g_weatherClipCameraPosition.x;
            const float dz = entry->center.z - g_weatherClipCameraPosition.z;
            const float distance = dx * dx + dz * dz;
            if (nearest > distance) {
                nearest = distance;
                selected = payload;
            }
        }
        ++entry;
    }
    return selected;
}
#else
const WeatherClipEntry *SelectWeatherClipEntry(const WeatherClipSet &set) {
    const WeatherClipEntry *selected = 0;
    float distance = 1.0e10f;
    const WeatherClipEntry *entry = set.entries;
    std::uint32_t remaining = static_cast<std::uint32_t>(set.count);
    while (remaining-- != 0) {
        if (entry->enabled != 0 && ScreenVisCheck(entry->center, entry->scale, 32.0f)) {
            const float dx = entry->center.x - g_weatherClipCameraPosition.x;
            const float dz = entry->center.z - g_weatherClipCameraPosition.z;
            float score = dz * dz;
            __asm__("fmac fr0,%1,%0" : "+f"(score) : "f"(dx), "w"(dx));
            if (distance > score) {
                distance = score;
                selected = entry;
            }
        }
        ++entry;
    }
    return selected;
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void EV_wthrApplyRainGroups(const Vector2 *clip) {
    ApplyWeatherClipSet0ToRain(&clip[0], &clip[1], &clip[2], &clip[3], clip[4].x, clip[4].y);
    ApplyWeatherClipSet0ToRipple(&clip[0], &clip[1], &clip[2], &clip[3], clip[4].x, clip[4].y);
}
#endif

// @verified - d
void ConfigureWeatherVolume(const WeatherClipVolume *volume) {
    ApplyWeatherClipSet1(&volume->points[0], &volume->points[1], &volume->points[2], &volume->points[3], volume->clipY0, volume->clipY1);
}

// @verified - d
void ReleaseWeatherVolumeGroup(std::int32_t index) {
    if (weatherClipSets[index].entries) {
        HLib::FreeTaskData(weatherClipSets[index].entries);
        weatherClipSets[index].enabled = 0;
        weatherClipSets[index].count = 0;
        weatherClipSets[index].entries = 0;
    }
}

// @verified - d
void SetWeatherVolumeGroupEnabled(std::int32_t enabled, std::int32_t index) {
    if (weatherClipSets[index].entries)
        weatherClipSets[index].enabled = enabled;
}

WeatherClipEntry *GetWeatherVolumeGroupEntries(std::int32_t index) {
    return weatherClipSets[index].entries;
}

std::int32_t GetWeatherVolumeGroupEnabled(std::int32_t index) {
    return weatherClipSets[index].enabled;
}

// @verified - d
void SetWeatherVolumeEnabled(std::int32_t index, std::uint32_t enabled, std::int32_t group) {
    if (weatherClipSets[group].entries && weatherClipSets[group].count > index) {
        WeatherClipEntry *entry = weatherClipSets[group].entries;
        for (std::int32_t current = 0; current < index; ++current)
            ++entry;
        entry->enabled = enabled;
    }
}

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex) {
#if SHENMUE_S2_GCC295 && (SHENMUE_IS_DC && SHENMUE_IS_S2)
    weatherClipSets[setIndex].enabled = 1;
    weatherClipSets[setIndex].count = count;
    if (weatherClipSets[setIndex].entries)
        HLib::FreeTaskData(weatherClipSets[setIndex].entries);
    WeatherClipEntry *target =
        static_cast<WeatherClipEntry *>(HLib::AllocateStaticOrDynamicBlock(static_cast<std::uint32_t>(count) * sizeof(WeatherClipEntry), WEATHER_CLIP_STORAGE));
    weatherClipSets[setIndex].entries = target;
    for (std::int32_t index = 0; index < count; ++index) {
        target->enabled = 1;
        *reinterpret_cast<WeatherClipPayload *>(target->points) = *static_cast<const WeatherClipPayload *>(entries);
        ++target;
        entries = static_cast<const WeatherClipPayload *>(entries) + 1;
    }
#else
    WeatherClipSet &set = weatherClipSets[setIndex];
    set.enabled = 1;
    set.count = count;
    if (set.entries)
        HLib::FreeTaskData(set.entries);
    set.entries = static_cast<WeatherClipEntry *>(HLib::AllocateStaticOrDynamicBlock(static_cast<std::uint32_t>(count) * 60U, WEATHER_CLIP_STORAGE));
    const std::byte *source = reinterpret_cast<const std::byte *>(entries);
    for (std::int32_t index = 0; index < count; ++index) {
        set.entries[index].enabled = 1;
        std::memcpy(&set.entries[index].points, source, 56);
        source += 56;
    }
#endif
}

// @verified - d
void Weather_ResetWCCPClipSet0() {
    Vector2 point;
    point.x = point.y = 0.0f;
    ApplyWeatherClipSet0ToRain(&point, &point, &point, &point, 0.0f, -100000.0f);
    ApplyWeatherClipSet0ToRipple(&point, &point, &point, &point, 0.0f, -100000.0f);
}

// @verified - d
void Weather_ResetWCCPClipSet1() {
    Vector2 point;
    point.x = point.y = 0.0f;
    ApplyWeatherClipSet1(&point, &point, &point, &point, 0.0f, -100000.0f);
}

void Weather_UpdateWCCPClipSet0() {
#if SHENMUE_S2_GCC295 && (SHENMUE_IS_DC && SHENMUE_IS_S2)
    const WeatherClipEntry *const entries = weatherClipSets[0].entries;
    if (!entries || !weatherClipSets[0].enabled)
        return;
    const WeatherClipPayload *const selected = SelectWeatherClipEntry(static_cast<std::uint32_t>(weatherClipSets[0].count), entries);
    if (selected) {
        const Vector2 *const points = selected->volume.points;
        ApplyWeatherClipSet0ToRain(points, &points[1], &points[2], &points[3], selected->volume.clipY0, selected->volume.clipY1);
        ApplyWeatherClipSet0ToRipple(points, &points[1], &points[2], &points[3], selected->volume.clipY0, selected->volume.clipY1);
    } else {
        Weather_ResetWCCPClipSet0();
    }
#else
    const WeatherClipSet &set = weatherClipSets[0];
    if (!set.entries || set.enabled == 0)
        return;
    const WeatherClipEntry *const selected = SelectWeatherClipEntry(set);
    if (selected) {
        ApplyWeatherClipSet0ToRain(&selected->points[0], &selected->points[1], &selected->points[2], &selected->points[3], selected->clipY0, selected->clipY1);
        ApplyWeatherClipSet0ToRipple(
            &selected->points[0], &selected->points[1], &selected->points[2], &selected->points[3], selected->clipY0, selected->clipY1);
    } else {
        Weather_ResetWCCPClipSet0();
    }
#endif
}

void Weather_UpdateWCCPClipSet1() {
#if SHENMUE_S2_GCC295 && (SHENMUE_IS_DC && SHENMUE_IS_S2)
    const WeatherClipEntry *const entries = weatherClipSets[1].entries;
    if (!entries || !weatherClipSets[1].enabled)
        return;
    const WeatherClipPayload *const selected = SelectWeatherClipEntry(static_cast<std::uint32_t>(weatherClipSets[1].count), entries);
    if (selected) {
        const Vector2 *const points = selected->volume.points;
        ApplyWeatherClipSet1(points, &points[1], &points[2], &points[3], selected->volume.clipY0, selected->volume.clipY1);
    } else {
        Weather_ResetWCCPClipSet1();
    }
#else
    const WeatherClipSet &set = weatherClipSets[1];
    if (!set.entries || set.enabled == 0)
        return;
    const WeatherClipEntry *const selected = SelectWeatherClipEntry(set);
    if (selected)
        ApplyWeatherClipSet1(&selected->points[0], &selected->points[1], &selected->points[2], &selected->points[3], selected->clipY0, selected->clipY1);
    else
        Weather_ResetWCCPClipSet1();
#endif
}

// @verified - d
ScenarioWindowRecord *AllocateSCNW(HLib::Task *task) {
    ScenarioWindowRecord *const record = static_cast<ScenarioWindowRecord *>(HLib::AllocateTaskData(sizeof(ScenarioWindowRecord), SCENARIO_WINDOW_STORAGE));
    record->retained04 = 0;
    record->retained08 = 0;
    record->task = task;
    event_runtime_detail::scenarioWindowRecord = record;
    event_runtime_detail::g_pSCNW = &event_runtime_detail::scenarioWindowRecord;
    return record;
}

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" void __attribute__((renesas)) shenmue_s2_event_EV_wthrApplyRainGroups(const shenmue::Vector2 *clip) {
    shenmue::s2::EV_wthrApplyRainGroups(clip);
}
#endif

#endif
