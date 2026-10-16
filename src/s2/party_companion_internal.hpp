#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2::model_component_detail {

inline constexpr shen_name kPartyToken{SHEN_NAME(PTYC)};

inline constexpr std::array<shen_name, 3> kPartyNames{{
    SHEN_NAME(REN_),
    SHEN_NAME(SIN_),
    SHEN_NAME(JOP_),
}};

} // namespace shenmue::s2::model_component_detail
