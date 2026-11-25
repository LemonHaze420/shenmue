#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue { namespace s2 {
struct MapInfoChunk;
MapInfoChunk *GetCurrentDynamicMap();
const std::byte *dynaRead(const std::uint8_t name[4]);
void dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation);
void dynaReadEvaluate(std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation, const std::uint8_t name[4]);
} } // namespace shenmue::s2

#else

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

// Native DYNA data is borrowed from a ready package, then current MAPINFO.
// Valid names/chunks and six nonempty channels are native caller preconditions.
const std::byte *dynaRead(const std::uint8_t name[4]);
std::uint64_t dynaEvaluate(const void *data, std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation);
std::uint64_t dynaReadEvaluate(std::int32_t track, std::int32_t frame, float *position, Vector3i *rotation, const std::uint8_t name[4]);

} // namespace shenmue::s2

#endif
