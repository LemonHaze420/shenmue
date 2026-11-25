#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/dynamic_tracks.hpp"
#include "s2/map_info.hpp"
#include "s2/pak_dc.hpp"
#else
#include "s2/dynamic_tracks.hpp"
#include "s2/scenario_internal.hpp"
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

using namespace event_runtime_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace event_runtime_detail {

template <typename T> T ReadDynamicValue(const std::byte *source);

} // namespace event_runtime_detail

std::uint64_t dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MapInfoChunk *GetCurrentDynamicMap();
void dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation);

#define DYNAMIC_VALUE(T, source) (*reinterpret_cast<const T *>(source))

namespace {

struct DynamicKey {
    std::int32_t frame;
    float value;
};

float EvaluateDynamicChannel(const std::byte *channel, std::int32_t frame);

} // namespace
#else
namespace event_runtime_detail {

template <typename T> T ReadDynamicValue(const std::byte *source);
float EvaluateDynamicChannel(const std::byte *&channel, std::int32_t frame);
std::int32_t DynamicFixedAngle(float degrees);

} // namespace event_runtime_detail

const std::byte *dynaRead(const std::uint8_t name[4]);
std::uint64_t dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation);
std::uint64_t dynaReadEvaluate(std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation, const std::uint8_t name[4]);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
float EvaluateDynamicChannel(const std::byte *channel, std::int32_t frame) {
    const std::int32_t count = DYNAMIC_VALUE(std::int32_t, channel);
    channel += 4;
    const DynamicKey *keys = reinterpret_cast<const DynamicKey *>(channel);
    std::int32_t upper = 1;
    while (upper < count && frame >= keys[upper].frame)
        ++upper;
    float value;
    if (upper != count) {
        frame -= keys[upper - 1].frame;
        value = keys[upper - 1].value +
            static_cast<float>(frame) * (keys[upper].value - keys[upper - 1].value) / static_cast<float>(keys[upper].frame - keys[upper - 1].frame);
    } else {
        value = keys[count - 1].value;
    }
    return value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation) {
    const std::byte *channel = static_cast<const std::byte *>(data);
    for (std::int32_t index = 0; index < track; ++index)
        channel += DYNAMIC_VALUE(std::int32_t, channel);
    channel += 4;
    position[0] = EvaluateDynamicChannel(channel, frame);
    channel = channel + DYNAMIC_VALUE(std::int32_t, channel) * 8 + 4;
    position[1] = EvaluateDynamicChannel(channel, frame);
    channel = channel + DYNAMIC_VALUE(std::int32_t, channel) * 8 + 4;
    position[2] = EvaluateDynamicChannel(channel, frame);
    channel = channel + DYNAMIC_VALUE(std::int32_t, channel) * 8 + 4;
    rotation->x = static_cast<std::int32_t>(EvaluateDynamicChannel(channel, frame) / 360.0f * 65536.0f);
    channel = channel + DYNAMIC_VALUE(std::int32_t, channel) * 8 + 4;
    rotation->y = static_cast<std::int32_t>(EvaluateDynamicChannel(channel, frame) / 360.0f * 65536.0f);
    channel = channel + DYNAMIC_VALUE(std::int32_t, channel) * 8 + 4;
    rotation->z = static_cast<std::int32_t>(EvaluateDynamicChannel(channel, frame) / 360.0f * 65536.0f);
}

#else

std::uint64_t dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation) {
    const auto *block = static_cast<const std::byte *>(data);
    for (std::int32_t index = 0; index < track; ++index)
        block += ReadDynamicValue<std::int32_t>(block);
    const std::byte *channel = block + 4;
    position[0] = EvaluateDynamicChannel(channel, frame);
    position[1] = EvaluateDynamicChannel(channel, frame);
    position[2] = EvaluateDynamicChannel(channel, frame);
    rotation->x = DynamicFixedAngle(EvaluateDynamicChannel(channel, frame));
    rotation->y = DynamicFixedAngle(EvaluateDynamicChannel(channel, frame));
    rotation->z = DynamicFixedAngle(EvaluateDynamicChannel(channel, frame));
    return static_cast<std::uint32_t>(rotation->z);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint64_t dynaReadEvaluate(std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation, const std::uint8_t name[4]) {
    return dynaEvaluate(dynaRead(name) + 8, track, frame, position, rotation);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
MapInfoChunk *GetCurrentDynamicMap() {
    return FindNativeChunk("DYNA", GetCurrentMapInfo());
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace event_runtime_detail {

template <typename T> T ReadDynamicValue(const std::byte *source) {
    T value;
    std::memcpy(&value, source, sizeof(value));
    return value;
}

float EvaluateDynamicChannel(const std::byte *&channel, std::int32_t frame) {
    const std::int32_t count = ReadDynamicValue<std::int32_t>(channel);
    std::int32_t upper = 1;
    while (upper < count && frame >= ReadDynamicValue<std::int32_t>(channel + 4 + upper * 8LL))
        ++upper;
    float value;
    if (upper == count) {
        value = ReadDynamicValue<float>(channel + count * 8LL);
    } else {
        const std::byte *const lowerKey = channel + 4 + (upper - 1) * 8LL;
        const std::int32_t lowerFrame = ReadDynamicValue<std::int32_t>(lowerKey);
        const std::int32_t upperFrame = ReadDynamicValue<std::int32_t>(lowerKey + 8);
        const float lowerValue = ReadDynamicValue<float>(lowerKey + 4);
        const float upperValue = ReadDynamicValue<float>(lowerKey + 12);
        // Native SUB is DWORD-wrapped before signed conversion to float.
        const auto elapsed = std::bit_cast<std::int32_t>(std::uint32_t(frame) - std::uint32_t(lowerFrame));
        const auto duration = std::bit_cast<std::int32_t>(std::uint32_t(upperFrame) - std::uint32_t(lowerFrame));
        value = ((upperValue - lowerValue) * static_cast<float>(elapsed)) / static_cast<float>(duration) + lowerValue;
    }
    channel += 4 + count * 8LL;
    return value;
}

std::int32_t DynamicFixedAngle(float degrees) {
    const double fixed = (static_cast<double>(degrees) / 360.0) * 65536.0;
    // Native CVTTSD2SI's integer-indefinite result includes NaN/infinity.
    return fixed >= -2147483648.0 && fixed < 2147483648.0 ? static_cast<std::int32_t>(fixed) : std::numeric_limits<std::int32_t>::min();
}
} // namespace event_runtime_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const std::byte *dynaRead(const std::uint8_t name[4]) {
    if (const std::byte *package = FindPackageDynamicMap(name))
        return package;
    const MapControlData *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (control)
        control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const auto *map = control ? static_cast<const std::byte *>(control->mapInfo) : nullptr;
    while (ReadDynamicValue<std::uint32_t>(map) != SHEN_NAME(DYNA)) {
        const std::uint32_t next = ReadDynamicValue<std::uint32_t>(map + 4);
        if (!next) {
            map = nullptr;
            break;
        }
        map += next;
    }
    const std::uint32_t wanted = std::uint32_t{name[0]} | (std::uint32_t{name[1]} << 8U) | (std::uint32_t{name[2]} << 16U) | (std::uint32_t{name[3]} << 24U);
    const std::byte *chunk = map + 8;
    for (;;) {
        if (ReadDynamicValue<std::uint32_t>(chunk) == wanted)
            return chunk;
        const std::uint32_t next = ReadDynamicValue<std::uint32_t>(chunk + 4);
        if (!next)
            return nullptr;
        chunk += next;
    }
}

#endif

} // namespace s2
} // namespace shenmue
