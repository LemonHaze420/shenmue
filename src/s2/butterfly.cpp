#include "s2/butterfly.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;


// @verified - p
void EVT_MoveButterfly_Callback(void *taskData) {
    ButterflyTaskData &data = *static_cast<ButterflyTaskData *>(taskData);
    const std::int32_t count = data.butterflyCount;
    const ButterflyRegion *region = data.regions;
    const std::uint32_t transformFlags = 0x38000000U;
    for (std::int32_t index = 0; index < count; ++index) {
        ButterflyMotion &motion = data.motion[index];
        motion.filteredVelocity.x = motion.filteredVelocity.x * 0.99800003f + motion.filteredVelocity.x * 0.0020000001f;
        motion.filteredVelocity.y = motion.filteredVelocity.y * 0.050000001f + motion.filteredVelocity.y * 0.94999999f;
        motion.filteredVelocity.z = motion.filteredVelocity.z * 0.99800003f + motion.filteredVelocity.z * 0.0020000001f;
        const Vector3 advanced{
            motion.filteredVelocity.x + motion.position.x, motion.filteredVelocity.y + motion.position.y, motion.filteredVelocity.z + motion.position.z};
        motion.position.x = (motion.target.x - advanced.x) * 0.0099999998f + advanced.x;
        motion.position.y = ((motion.target.y - advanced.y) + 0.0f) * 0.0099999998f + advanced.y;
        motion.position.z = (motion.target.z - advanced.z) * 0.0099999998f + advanced.z;
        const Vector3 &anchor = region[motion.regionIndex].position;
        const std::int32_t yaw = static_cast<std::int16_t>(FixedAtan2(-(anchor.x - motion.position.x), -(anchor.z - motion.position.x))) + 0x8000;
        const std::uint32_t id = (static_cast<std::uint32_t>(index) << 24) - 0x09ff0000U * (static_cast<std::uint32_t>(index) / 10);
        const shen_name character{SHEN_NAME(FL00) + id};
        EV_AT_SetAngY(character, transformFlags, {.x = 0, .y = yaw, .z = 0});
        Vector3 position = motion.position;
        const std::uint32_t phase = static_cast<std::uint32_t>(motion.phase);
        const float first = sin_fixed16(static_cast<std::int16_t>(2U * phase)) + sin_fixed16(static_cast<std::int16_t>(phase));
        const float oscillation = sin_fixed16(static_cast<std::int16_t>(5U * phase)) + first;
        position.y += (sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(3U * phase)))) * oscillation) * 0.050000001f;
        motion.phase = static_cast<std::int32_t>(phase + static_cast<std::uint32_t>(motion.phaseStep / 5));
        if (anchor.y > motion.position.y)
            motion.velocity.y = 0.001f;
        EV_AT_SetPos(character, transformFlags, position);
    }
    if ((g_rootTaskIterationCount & 0x3fU) == 0) {
        for (std::int32_t index = 0; index < count; ++index) {
            ButterflyMotion &motion = data.motion[index];
            motion.velocity.x = (IncrementSharedPrngFloat() * 0.0020000001f - 0.001f) * 16.0f;
            motion.velocity.z = (IncrementSharedPrngFloat() * 0.0020000001f - 0.001f) * 16.0f;
            motion.phaseStep = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 1024.0f) + 2048;
        }
    }
    if ((g_rootTaskIterationCount & 3U) == 0) {
        for (std::int32_t index = 0; index < count; ++index) {
            ButterflyMotion &motion = data.motion[index];
            motion.velocity.y = (IncrementSharedPrngFloat() * 0.0020000001f - 0.001f) * (motion.flags ? 5.0f : 20.0f);
        }
    }
    Vector3 playerPosition;
    EV_GetCharaPosition(ryoName, CHARACTER_TRANSLATION_ROOT, &playerPosition, EventTransformFlags::Local);
    std::int32_t regionIndex = 0;
    if (region->totalRuns == 0)
        return;
    while (!ComputeAngle(playerPosition, 6.0f, 0, 0x8000, region->position)) {
        ++region;
        ++regionIndex;
        if (region->totalRuns == 0)
            return;
    }
    if (butterflyRegionIndex == regionIndex)
        return;
    butterflyRegionIndex = regionIndex;
    for (std::int32_t run = 0; run < region->totalRuns; ++run) {
        ButterflyMotion &motion = data.motion[butterflySlotIndex];
        const float scale = IncrementSharedPrngFloat() * 0.2f + 0.80000001f;
        const std::uint32_t scaleId = (static_cast<std::uint32_t>(regionIndex) << 24) - 0x09ff0000U * (static_cast<std::uint32_t>(regionIndex) / 10);
        EV_SetCharacterScale(shen_name{SHEN_NAME(FL00) + scaleId}, {.x = scale, .y = scale, .z = scale});
        motion.flags = region->flags;
        motion.position.x = (IncrementSharedPrngFloat() * 0.2f - 0.1f) * (region->flags ? 2.5f : 5.0f) + region->position.x;
        motion.position.y = (region->position.y + 0.5f) + (IncrementSharedPrngFloat() * 0.2f - 0.1f) * (region->flags ? 1.5f : 3.0f);
        motion.position.z = (IncrementSharedPrngFloat() * 0.2f - 0.1f) * (region->flags ? 2.5f : 5.0f) + region->position.z;
        motion.regionIndex = regionIndex;
        motion.target = motion.position;
        const std::uint32_t id = (static_cast<std::uint32_t>(butterflySlotIndex) << 24) - 0x09ff0000U * (static_cast<std::uint32_t>(butterflySlotIndex) / 10);
        EV_AT_SetPos(shen_name{SHEN_NAME(FL00) + id}, transformFlags, playerPosition);
        motion.velocity.x = (IncrementSharedPrngFloat() * 0.0020000001f - 0.001f) * 10.0f;
        motion.velocity.y = IncrementSharedPrngFloat() * 0.0040000002f - 0.0020000001f;
        const float random = IncrementSharedPrngFloat() * 0.0020000001f;
        butterflySlotIndex = (butterflySlotIndex + 1) % count;
        motion.velocity.z = (random - 0.001f) * 10.0f;
    }
}

} // namespace shenmue::s2
