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



const std::byte *dynaRead(const std::uint8_t name[4]);
std::uint64_t dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation);
std::uint64_t dynaReadEvaluate(std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation, const std::uint8_t name[4]);

} // namespace shenmue::s2
