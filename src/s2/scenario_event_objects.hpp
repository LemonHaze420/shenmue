#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {





void PublishScenarioEventObjectMapNames(std::span<const shen_name> names);
std::span<const shen_name> GetScenarioEventObjectMapNames();
void ResetScenarioEventObjectMapNames();

enum class ScenarioEventObjectRegionShape : std::uint8_t {
    Rectangle,
    Circle,
    HeightRange,
};

struct ScenarioEventObjectSpatialRegion {
    shen_name eventObject;
    shen_name map;
    ScenarioEventObjectRegionShape shape;
    float minimumX;
    float minimumZ;
    float maximumX;
    float maximumZ;
    float centerX;
    float centerZ;
    float radius;
    float minimumY;
    float maximumY;
};

struct ScenarioEventObjectFallback {
    shen_name eventObject;
    shen_name map;
};

struct ScenarioEventObjectRelation {
    shen_name eventObject;
    shen_name currentName;
};

} // namespace shenmue::s2
