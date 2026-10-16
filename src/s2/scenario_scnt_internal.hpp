#pragma once
#include "s2/scenario_scnt.hpp"
#include <algorithm>

namespace shenmue::s2::event_runtime_detail {


struct ScntBranchHistory {
    std::array<std::size_t, 100> targets{};
    std::size_t count{};

    bool Visit(std::size_t target) {
        for (std::size_t index = 0; index < count; ++index)
            if (targets[index] == target)
                return true;
        if (count == targets.size()) {
            std::move(targets.begin() + 1, targets.end(), targets.begin());
            --count;
        }
        targets[count++] = target;
        return false;
    }
};
} // namespace shenmue::s2::event_runtime_detail
