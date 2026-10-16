#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2::model_component_detail {

struct DetachObjectXFormsContext {
    Character *source{};
};

inline constexpr shen_name kObjectXFormsToken{SHEN_NAME(FIXO)};

} // namespace shenmue::s2::model_component_detail
