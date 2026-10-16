#pragma once

#include <cstdint>

namespace shenmue::s1 {


template <typename Table, typename Store> bool InsertProbedHash(Table &table, std::uint32_t mask, std::uint32_t capacity, std::uint32_t hash, Store store) {
    const std::uint32_t count = table.count;
    if (count >= capacity)
        return false;
    const auto insert = [&](std::uint32_t slot) {
        table.count = count + 1;
        table.hashes[slot] = hash;
        store(slot);
        return true;
    };
    std::uint32_t slot = hash & mask;
    std::int64_t freeSlot = -1;
    const std::uint32_t first = table.hashes[slot];
    if (first == hash)
        return true;
    if (!first) {
        freeSlot = slot;
        if (!table.probes[slot])
            return insert(slot);
    }
    for (std::uint16_t distance = table.probes[slot]; distance; distance = table.probes[slot]) {
        slot = (distance + slot) & mask;
        const std::uint32_t chained = table.hashes[slot];
        if (chained == hash)
            return true;
        if (!chained) {
            if (freeSlot == -1)
                freeSlot = slot;
            if (!table.probes[slot])
                return insert(slot);
        }
    }
    if (freeSlot != -1)
        return insert(static_cast<std::uint32_t>(freeSlot));
    std::uint32_t next = (slot + 1) & mask;
    if (next == slot)
        return false;
    while (table.hashes[next]) {
        next = (next + 1) & mask;
        if (next == slot)
            return false;
    }
    insert(next);
    std::uint32_t target = next;
    if (next < slot)
        target = ((slot - next - 1) & (~mask & 0xffffU)) + next + mask + 1;
    table.probes[slot] = static_cast<std::uint16_t>(target - slot);
    return true;
}


template <typename Table, typename Clear> bool RemoveProbedHash(Table &table, std::uint32_t mask, std::uint32_t hash, Clear clear) {
    std::uint32_t slot = hash & mask;
    if (table.hashes[slot] == hash) {
        table.hashes[slot] = 0;
        clear(slot);
        --table.count;
        return true;
    }
    std::uint32_t previous = slot;
    std::uint16_t distance = table.probes[slot];
    if (!distance)
        return false;
    while (true) {
        slot = (distance + slot) & mask;
        if (table.hashes[slot] == hash)
            break;
        distance = table.probes[slot];
        previous = slot;
        if (!distance)
            return false;
    }
    table.hashes[slot] = 0;
    clear(slot);
    if (!table.probes[slot])
        table.probes[previous] = 0;
    --table.count;
    return true;
}

} // namespace shenmue::s1
