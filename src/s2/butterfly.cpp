#include "s2/butterfly.hpp"
#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
int butterflySlotIndex = 0;
int butterflyRegionIndex = -1;
#else
#include "s2/scenario_internal.hpp"
namespace shenmue::s2 {
using namespace event_runtime_detail;
#endif

// @verified - p
void EVT_MoveButterfly_Callback(void *taskData)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    ButterflyTaskData *data = (ButterflyTaskData *)taskData;
#else
    ButterflyTaskData *data = static_cast<ButterflyTaskData *>(taskData);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    int count = data->butterflyCount;
    ButterflyRegion *region = data->regions;
    ButterflyRegion *regions = region;
#else
    const std::int32_t count = data->butterflyCount;
    const ButterflyRegion *region = data->regions;
    const std::uint32_t transformFlags = 0x38000000U;
#endif
    int i;
    Vector3 position;
    int j;
    for (i = 0; i < count; i++) {
#if defined(SHENMUE_EVENT_OVERLAY)
        ButterflyMotion *motion = &data->motion[i];
        Vector3i rotation;
        int id;
        motion->filteredVelocity.x = 0.99800003f * motion->filteredVelocity.x + 0.0020000001f * motion->filteredVelocity.x;
        motion->filteredVelocity.y = 0.94999999f * motion->filteredVelocity.y + 0.050000001f * motion->filteredVelocity.y;
        motion->filteredVelocity.z = 0.99800003f * motion->filteredVelocity.z + 0.0020000001f * motion->filteredVelocity.z;
        motion->position.x = motion->position.x + motion->filteredVelocity.x;
        motion->position.y = motion->position.y + motion->filteredVelocity.y;
        motion->position.z = motion->position.z + motion->filteredVelocity.z;
        motion->position.x = (motion->target.x - motion->position.x) * 0.0099999998f + motion->position.x;
        motion->position.y = ((motion->target.y - motion->position.y) + 0.0f) * 0.0099999998f + motion->position.y;
        motion->position.z = (motion->target.z - motion->position.z) * 0.0099999998f + motion->position.z;
        rotation.x = 0;
        rotation.y = 0;
        rotation.z = 0;
        rotation.y = FTBL.InverseFixedAtan2(regions[motion->regionIndex].position.x - motion->position.x,
                                            regions[motion->regionIndex].position.z - motion->position.x) +
                     0x8000;
        id = i / 10;
        FTBL.EV_AT_SetAngY_0(SHEN_NAME4('F', 'L', '0', '0') + (id << 16) + ((i - id * 10) << 24), 0x38000000, &rotation, 0);
        position = motion->position;
        position.y +=
            ((FTBL.CalculateFixedSine(motion->phase) + FTBL.CalculateFixedSine(2 * motion->phase) + FTBL.CalculateFixedSine(5 * motion->phase)) *
             FTBL.CalculateFixedCosine(3 * motion->phase)) *
            0.050000001f;
        motion->phase += motion->phaseStep / 5;
        if (motion->position.y < regions[motion->regionIndex].position.y)
            motion->velocity.y = 0.001f;
        id = i / 10;
        FTBL.EV_AT_SetPos(SHEN_NAME4('F', 'L', '0', '0') + (id << 16) + ((i - id * 10) << 24), 0x38000000, &position);
#else
        ButterflyMotion &motion = data->motion[i];
        motion.filteredVelocity.x = motion.filteredVelocity.x * 0.99800003f + motion.filteredVelocity.x * 0.0020000001f;
        motion.filteredVelocity.y = motion.filteredVelocity.y * 0.050000001f + motion.filteredVelocity.y * 0.94999999f;
        motion.filteredVelocity.z = motion.filteredVelocity.z * 0.99800003f + motion.filteredVelocity.z * 0.0020000001f;
        const Vector3 advanced{motion.filteredVelocity.x + motion.position.x, motion.filteredVelocity.y + motion.position.y,
                               motion.filteredVelocity.z + motion.position.z};
        motion.position.x = (motion.target.x - advanced.x) * 0.0099999998f + advanced.x;
        motion.position.y = ((motion.target.y - advanced.y) + 0.0f) * 0.0099999998f + advanced.y;
        motion.position.z = (motion.target.z - advanced.z) * 0.0099999998f + advanced.z;
        const Vector3 &anchor = region[motion.regionIndex].position;
        const std::int32_t yaw = static_cast<std::int16_t>(FixedAtan2(-(anchor.x - motion.position.x), -(anchor.z - motion.position.x))) + 0x8000;
        const std::uint32_t id = (static_cast<std::uint32_t>(i) << 24) - 0x09ff0000U * (static_cast<std::uint32_t>(i) / 10);
        const shen_name character{SHEN_NAME(FL00) + id};
        EV_AT_SetAngY(character, transformFlags, {.x = 0, .y = yaw, .z = 0});
        Vector3 position = motion.position;
        const std::uint32_t phase = static_cast<std::uint32_t>(motion.phase);
        const float first = sin_fixed16(static_cast<std::int16_t>(2U * phase)) + sin_fixed16(static_cast<std::int16_t>(phase));
        const float oscillation = sin_fixed16(static_cast<std::int16_t>(5U * phase)) + first;
        position.y +=
            (sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(3U * phase)))) * oscillation) * 0.050000001f;
        motion.phase = static_cast<std::int32_t>(phase + static_cast<std::uint32_t>(motion.phaseStep / 5));
        if (anchor.y > motion.position.y)
            motion.velocity.y = 0.001f;
        EV_AT_SetPos(character, transformFlags, position);
#endif
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    if ((g_frameCounter & 0x3f) == 0) {
#else
    if ((g_rootTaskIterationCount & 0x3f) == 0) {
#endif
        for (i = 0; i < count; i++) {
            ButterflyMotion *motion = &data->motion[i];
            motion->velocity.x = (FTBL.GetRandomNumberInRange(0.0020000001f) - 0.001f) * 16.0f;
            motion->velocity.z = (FTBL.GetRandomNumberInRange(0.0020000001f) - 0.001f) * 16.0f;
            motion->phaseStep = FTBL.shenGetRandomInt32(1024) + 2048;
        }
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    if ((g_frameCounter & 3) == 0) {
#else
    if ((g_rootTaskIterationCount & 3) == 0) {
#endif
        for (i = 0; i < count; i++) {
            ButterflyMotion *motion = &data->motion[i];
#if defined(SHENMUE_EVENT_OVERLAY)
            if (!motion->flags)
                motion->velocity.y = (FTBL.GetRandomNumberInRange(0.0020000001f) - 0.001f) * 20.0f;
            else
                motion->velocity.y = (FTBL.GetRandomNumberInRange(0.0020000001f) - 0.001f) * 5.0f;
#else
            motion->velocity.y = (IncrementSharedPrngFloat() * 0.0020000001f - 0.001f) * (motion->flags ? 5.0f : 20.0f);
#endif
        }
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.GetCharaPosition(SHEN_NAME4('R', 'Y', 'O', '_'), 0xffffffff, &position, 0);
#else
    EV_GetCharaPosition(ryoName, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    for (i = 0; region->totalRuns; region++, i++) {
        if (FTBL.ComputeAngle(&position, 6.0f, 0, 0x8000, &region->position)) {
#else
    std::int32_t regionIndex = 0;
    if (region->totalRuns == 0)
        return;
    while (!ComputeAngle(position, 6.0f, 0, 0x8000, region->position)) {
        ++region;
        ++regionIndex;
        if (region->totalRuns == 0)
            return;
    }
    i = regionIndex;
#endif
            if (butterflyRegionIndex == i)
                return;
            butterflyRegionIndex = i;
            for (j = 0; j < region->totalRuns; j++) {
                ButterflyMotion *motion = &data->motion[butterflySlotIndex];
#if defined(SHENMUE_EVENT_OVERLAY)
                Vector3 scales = {1.0f, 1.0f, 1.0f};
                int scaleId;
                unsigned int id;
                float random;
                scales.x = scales.y = scales.z = FTBL.GetRandomNumberInRange(0.2f) + 0.80000001f;
                scaleId = i / 10;
                FTBL.mtwkSetMotionworkScale3(SHEN_NAME4('F', 'L', '0', '0') + (scaleId << 16) + ((i - scaleId * 10) << 24), &scales);
#else
        const float scale = IncrementSharedPrngFloat() * 0.2f + 0.80000001f;
        const std::uint32_t scaleId = (static_cast<std::uint32_t>(i) << 24) - 0x09ff0000U * (static_cast<std::uint32_t>(i) / 10);
        std::uint32_t id;
        float random;
        EV_SetCharacterScale(shen_name{SHEN_NAME(FL00) + scaleId}, {.x = scale, .y = scale, .z = scale});
#endif
                motion->flags = region->flags;
#if defined(SHENMUE_EVENT_OVERLAY)
                if (!region->flags) {
                    motion->position.x = region->position.x + (FTBL.GetRandomNumberInRange(0.2f) - 0.1f) * 5.0f;
                    motion->position.y = (region->position.y + 0.5f) + (FTBL.GetRandomNumberInRange(0.2f) - 0.1f) * 3.0f;
                    motion->position.z = region->position.z + (FTBL.GetRandomNumberInRange(0.2f) - 0.1f) * 5.0f;
                } else {
                    motion->position.x = region->position.x + (FTBL.GetRandomNumberInRange(0.2f) - 0.1f) * 2.5f;
                    motion->position.y = (region->position.y + 0.5f) + (FTBL.GetRandomNumberInRange(0.2f) - 0.1f) * 1.5f;
                    motion->position.z = region->position.z + (FTBL.GetRandomNumberInRange(0.2f) - 0.1f) * 2.5f;
                }
#else
        motion->position.x = (IncrementSharedPrngFloat() * 0.2f - 0.1f) * (region->flags ? 2.5f : 5.0f) + region->position.x;
        motion->position.y = (region->position.y + 0.5f) + (IncrementSharedPrngFloat() * 0.2f - 0.1f) * (region->flags ? 1.5f : 3.0f);
        motion->position.z = (IncrementSharedPrngFloat() * 0.2f - 0.1f) * (region->flags ? 2.5f : 5.0f) + region->position.z;
        motion->regionIndex = i;
#endif
                motion->target = motion->position;
#if defined(SHENMUE_EVENT_OVERLAY)
                motion->regionIndex = i;
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
                id = butterflySlotIndex / 10;
                FTBL.EV_AT_SetPos(SHEN_NAME4('F', 'L', '0', '0') + (id << 16) + ((butterflySlotIndex - id * 10) << 24), 0x38000000, &position);
#else
        id = (static_cast<std::uint32_t>(butterflySlotIndex) << 24) - 0x09ff0000U * (static_cast<std::uint32_t>(butterflySlotIndex) / 10);
        EV_AT_SetPos(shen_name{SHEN_NAME(FL00) + id}, 0x38000000U, position);
#endif
                motion->velocity.x = (FTBL.GetRandomNumberInRange(0.0020000001f) - 0.001f) * 10.0f;
                motion->velocity.y = FTBL.GetRandomNumberInRange(0.0040000002f) - 0.0020000001f;
                random = FTBL.GetRandomNumberInRange(0.0020000001f);
#if defined(SHENMUE_EVENT_OVERLAY)
                motion->velocity.z = (random - 0.001f) * 10.0f;
                butterflySlotIndex = (butterflySlotIndex + 1) % count;
#else
        butterflySlotIndex = (butterflySlotIndex + 1) % count;
        motion->velocity.z = (random - 0.001f) * 10.0f;
#endif
            }
#if defined(SHENMUE_EVENT_OVERLAY)
            return;
        }
    }
#endif
}
#if defined(SHENMUE_EVENT_OVERLAY)

void QueueButterflies(ButterflyRegion *regions, int count)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_MoveButterfly_Callback, 4, 13, sizeof(ButterflyTaskData), SHEN_NAME4('b', 'f', 'l', 'y'));
    ButterflyTaskData *work = (ButterflyTaskData *)FTBL.HLib_GetTaskParameter(task);
    int i;
    for (i = 0; i < count; i++) {
        ButterflyMotion *motion = &work->motion[i];
        motion->velocity.x = 0.0f;
        motion->velocity.y = 0.0f;
        motion->velocity.z = 0.0f;
        motion->filteredVelocity.x = 0.0f;
        motion->filteredVelocity.y = 0.0f;
        motion->filteredVelocity.z = 0.0f;
        motion->target.x = 0.0f;
        motion->target.y = 0.0f;
        motion->target.z = 0.0f;
        motion->regionIndex = 0;
        motion->phase = 0;
        motion->phaseStep = 0;
        motion->flags = 0;
    }
    work->butterflyCount = count;
    work->regions = regions;
}
#else
} // namespace shenmue::s2
#endif
