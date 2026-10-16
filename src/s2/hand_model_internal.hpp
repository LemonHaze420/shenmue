#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2::model_component_detail {

struct RuntimeHandVertex {
    Vector3 position, normal, deformedPosition, deformedNormal;
};

struct RuntimeHandStaging {
    RuntimeHandVertex *vertices;
    std::int32_t *mappings;
    std::int32_t mappingCount;
};

static_assert(sizeof(RuntimeHandVertex) == 48);

inline constexpr shen_name kLeftHandToken{SHEN_NAME(HNDL)};

inline constexpr shen_name kRightHandToken{SHEN_NAME(HNDR)};

} // namespace shenmue::s2::model_component_detail
