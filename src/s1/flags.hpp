#pragma once

#include <cstdint>

namespace shenmue::s1 {

enum ShenmueFlag : std::uint16_t {
#define SHENMUE_S1_FLAG(name, value) SHENMUE_FLAG_##name = value,
#include "common/script_catalogue.def"
#undef SHENMUE_S1_FLAG
};

} // namespace shenmue::s1
