#include "s2/areas/area_pvsj.hpp"
#include "s2/event_runtime.hpp"
#include "s2/npc_package.hpp"
#include "s2/motion_work.hpp"
#include "s2/scene_data.hpp"
#include "s2/map_control.hpp"
#include "s2/debug.hpp"
#include <cstring>
namespace shenmue::s2 {

void PvsjInitializeBoards(PvsjEventWork *work) {
    PatiTableData tables;
    PATI_TBL_Parser(&tables);
    Vector3 player;
    EV_GetCharaPosition(PVSJ_PLAYER, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    std::int32_t nearest = 0;
    float distance = 100000000.0f;
    for (std::int32_t index = 0; tables.tables[index].area.name; ++index) {
        const auto &table = tables.tables[index];
        if (table.area.name != g_currentMap.name)
            continue;
        for (std::int32_t board = static_cast<std::int32_t>(table.firstBoard); board < static_cast<std::int32_t>(table.firstBoard) + table.boardCount;
             ++board) {
            const float dx = player.x - table.position.x, dy = player.y - table.position.y, dz = player.z - table.position.z;
            const float candidate = (dy * dy + dx * dx) + dz * dz;
            if (distance > candidate) {
                distance = candidate;
                nearest = index;
            }
        }
    }
    std::memcpy(&work->owner, tables.tables[nearest].npc, 4);
    pvsjBoardCount = 0;
    for (std::int32_t index = 0; tables.tables[index].area.name; ++index) {
        const auto &table = tables.tables[index];
        shen_name owner;
        std::memcpy(&owner, table.npc, 4);
        if (table.area.name != g_currentMap.name || owner.name != work->owner.name)
            continue;
        for (std::int32_t board = static_cast<std::int32_t>(table.firstBoard); board < static_cast<std::int32_t>(table.firstBoard) + table.boardCount;
             ++board) {
            auto &position = pvsjBoardPositions[pvsjBoardCount];
            std::memcpy(&position.character, tables.boards[board].character, 4);
            EV_GetCharaPosition(position.character, CHARACTER_TRANSLATION_ROOT, &position.position, EventTransformFlags::World);
            ++pvsjBoardCount;
        }
    }
    pvsjBoardPositions[pvsjBoardCount].character = {};
    for (auto *group = pvsjBoardGroups; group->area.name; ++group)
        if (group->area.name == g_currentMap.name && group->owner.name == work->owner.name) {
            work->boards = group;
            break;
        }
    for (auto *customer = work->boards->customers; customer->character.name; ++customer)
        customer->currentPoint = customer->initialPoint;
    work->pointCount = 0;
    for (auto *point = work->boards->points + 1; point->name; ++point)
        ++work->pointCount;
    work->admittedCustomers = 0;
    for (auto *customer = work->boards->customers + 1; customer->kind == 1; ++customer)
        ++work->admittedCustomers;
    std::int32_t customers = 0;
    for (auto *customer = work->boards->customers + 1; customer->character.name; ++customer)
        ++customers;
    work->missingCustomers = customers - work->admittedCustomers;
}

void PvsjChooseCustomer(PvsjEventWork *work) {
    const shen_name previous = work->customer;
    work->customerActive = 1;
    work->greeting = work->boards->greetings[work->greetingIndex];
    work->greetingIndex = (work->greetingIndex + 1) & 15;
    if (static_cast<std::uint32_t>(work->greeting) <= 1U) {
        if (work->admittedCustomers) {
            const auto index = shenGetRandomInt32(work->admittedCustomers - 1) + 1;
            work->customer = work->boards->customers[index].character;
            if (work->customer.name == previous.name) {
                const auto replacement = work->boards->customers[index + 1].kind == 1 ? index + 1 : 1;
                work->customer = work->boards->customers[replacement].character;
            }
            return;
        }
    } else if (const auto count = work->admittedCustomers + work->missingCustomers) {
        const auto index = shenGetRandomInt32(count - 1) + 1;
        work->customer = work->boards->customers[index].character;
        if (work->customer.name == previous.name)
            work->customer = work->boards->customers[1].character;
        return;
    }
    work->customer = work->owner;
}

std::int32_t PvsjMoveCustomer(PvsjEventWork *work, shen_name character, shen_name target) {
    auto *customer = work->boards->customers;
    for (; customer->character.name && customer->character.name != character.name; ++customer) {
    }
    shen_name previous = customer->currentPoint;
    if (!previous.name) {
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 2068, "PVSJ:Can not move.\n");
        return 0;
    }
    customer->currentPoint = target;
    constexpr std::uint32_t goal = SHEN_NAME(GOL1) & 0x00ffffffU;
    if ((previous.name & 0x00ffffffU) == goal)
        previous.name = (target.name & 0xff000000U) | goal;
    if ((target.name & 0x00ffffffU) == goal)
        target.name = (previous.name & 0xff000000U) | goal;
    return static_cast<std::int32_t>(StartNpcScriptAreaRoute(character, previous, target, static_cast<std::uint16_t>(work->boards->yaw)));
}

std::int32_t PvsjRestoreCustomerRoute(PvsjEventWork *work, shen_name character) {
    const auto *customer = work->boards->customers;
    for (; customer->character.name && customer->character.name != character.name; ++customer) {
    }
    if (!customer->currentPoint.name) {
        EV_debug_log("PVSJ\\ePVSJ_et_PVSJ.c", 2116, "PVSJ:Can not move.\n");
        return 0;
    }
    return static_cast<std::int32_t>(PlaceNpcAtAreaPoint(character, customer->currentPoint, nullptr, work->boards->yaw));
}
} // namespace shenmue::s2
