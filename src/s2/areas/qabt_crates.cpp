#include "s2/areas/area_qabt.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include <cmath>
namespace shenmue::s2::areas {
Vector3 qabtCameraOffset{}, qabtDestinationOffset{};
Vector3i qabtCameraRotation{};
Vector3i qabtCrateRotation{}, qabtDestinationRotation{};
void SetInitialQabtCarrySettings(QabtCarryWork &work, std::int32_t crate) {
    work.forwardSteps = 0;
    work.phase = QabtCarryTurnInit;
    work.turns = 2;
    work.failures = 0;
    work.speed = ARBT_CARRY_SPEEDS[crate];
    work.consecutiveFailures = 0;
    work.idleCount = 0;
    std::int32_t route = shenGetRandomInt32(3);
    if (g_actNumber != 1)
        route += g_actNumber == 2 ? 3 : 6;
    work.route = arbtCarryRoutes[crate + 6 * route];
    work.success = 0;
    work.frames = 0;
    work.direction = 0;
    work.presses = 1;
    work.intensity = 0;
}
std::int32_t SelectNextQabtCrate(ArbtCrateWork &work) {
    if (work.moveTask) {
        work.moveFrames = 0;
        HLib::DestroyTaskInternal(work.moveTask);
        work.moveTask = nullptr;
    }
    if (work.index < work.limit) {
        ++work.index;
        const shen_name crate = work.crates[work.index];
        AT_SetLocalBaseChara(crate, ARBT_BASE);
        AT_SetLocalBaseChara(ARBT_DUMB, crate);
        AT_SetLocalBaseChara(ARBT_EG0R, crate);
        AT_SetLocalBaseChara(ARBT_EG0L, crate);
        AT_SetLocalBaseChara(ARBT_EG1R, crate);
        AT_SetLocalBaseChara(ARBT_EG1L, crate);
        EV_AT_SetPos(ARBT_DUMB, EventTransformFlags::AllAxes, {-0.9f, 0, -0.08f});
        EV_AT_SetPos(ARBT_EG0R, EventTransformFlags::AllAxes, {0.66f, 0.0305f, -0.272f});
        EV_AT_SetPos(ARBT_EG0L, EventTransformFlags::AllAxes, {0.68f, 0.1f, 0.32f});
        EV_AT_SetPos(ARBT_EG1R, EventTransformFlags::AllAxes, {-0.66f, 0.0305f, 0.272f});
        EV_AT_SetPos(ARBT_EG1L, EventTransformFlags::AllAxes, {-0.66f, 0.0305f, -0.272f});
        for (shen_name name : {ARBT_DUMB, ARBT_EG0R, ARBT_EG0L, ARBT_EG1R, ARBT_EG1L})
            EV_AT_SetAngY(name, EventTransformFlags::AllAxes, {0, 49152, 0});
        AT_SetLocalBaseChara(ARBT_DUMN, work.destinations[work.index]);
        EV_AT_SetPos(ARBT_DUMN, EventTransformFlags::AllAxes, qabtDestinationOffset);
        EV_AT_SetAngY(ARBT_DUMN, EventTransformFlags::AllAxes, qabtDestinationRotation);
    }
    return work.limit - work.index;
}
void CreateQabtCrates(ArbtCrateWork &work) {
    work.moveTask = nullptr;
    work.moveFrames = 0;
    work.dropTask = nullptr;
    work.dropHeight = 0;
    work.index = -1;
    work.limit = 6;
    std::int32_t first = 0, second = 0;
    shen_name name = ARBT_CRATE_FIRST;
    for (std::int32_t index = 0; index < 16; ++index, name.name += 0x01000000U) {
        work.crates[index] = name;
        GetRandomNumberInRange(100);
        Vector3 position;
        if (first >= 8) {
            position.x = -4.5f;
            position.y = 0.7f - static_cast<float>(second & 1) * 0.4f;
            position.z = static_cast<float>(second >> 1) * 2.1f - 1.2f;
            position.z += (first & 1) ? 0.04f : -0.08f;
            ++second;
        } else {
            position.x = -6.5f;
            position.y = 0.7f - static_cast<float>(first & 1) * 0.4f;
            position.z = static_cast<float>(first >> 1) * 2.1f - 1.2f;
            if (first > 1)
                position.z += (first & 1) ? 0.04f : -0.08f;
            ++first;
        }
        AT_SetLocalBaseChara(name, ARBT_BASE);
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, qabtCrateRotation);
        AT_SetLocalBaseChara(name, EVENT_CHARACTER_INVALID);
        EV_EnableCharacterDisplay(name);
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        const Vector3 scale{1, 1, 0.6f};
        mtwkSetMotionworkScale3(name, &scale);
        SetScenarioCharacterDrawControl(name, 2, 1);
    }
    name = ARBT_PALLET_FIRST;
    for (std::int32_t index = 0; index < 16; ++index, name.name += 0x01000000U) {
        CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("PLT0H01G"), name);
        work.pallets[index] = name;
        Vector3 position;
        position.y = 0.05f;
        Vector3 scale;
        if (index >= 8) {
            position.x = index >= 12 ? -4.5f : -6.5f;
            position.z = static_cast<float>(index - (index >= 12 ? 12 : 8)) * 2.1f - 1.2f;
            EV_EnableCharacterDisplay(name);
            scale = {1, 1.75f, 1.2f};
        } else {
            if (index >= 4) {
                position.x = -14.8f;
                position.z = (static_cast<float>(index - 4) * 2.1f - 1.2f) - 0.7f;
                EV_DisableCharacterDisplay(name);
            } else {
                position.x = -16.8f;
                position.z = (static_cast<float>(index) * 2.1f - 1.2f) - 0.7f;
                EV_EnableCharacterDisplay(name);
            }
            scale = {1, 1.75f, 1};
        }
        mtwkSetMotionworkScale3(name, &scale);
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        AT_SetLocalBaseChara(name, ARBT_BASE);
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, qabtCrateRotation);
    }
    first = 0;
    second = 0;
    for (std::int32_t index = 0; index < 16; ++index) {
        GetRandomNumberInRange(100);
        if (first >= 8) {
            work.destinations[index] = shen_name{ARBT_PALLET_SECOND_ROW.name + (static_cast<std::uint32_t>(second >> 1) << 24)};
            work.stacked[index] = second++ & 1;
        } else {
            work.destinations[index] = shen_name{ARBT_PALLET_FIRST.name + (static_cast<std::uint32_t>(first >> 1) << 24)};
            work.stacked[index] = first++ & 1;
        }
    }
    SelectNextQabtCrate(work);
}
} // namespace shenmue::s2::areas
