#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {

static const std::size_t AUTHOR_CONTROLLED_DOOR_COUNT = 100;
extern std::int32_t g_authorControlledDoorCount;
extern shen_name g_authorControlledDoors[AUTHOR_CONTROLLED_DOOR_COUNT];
std::int32_t RecordAuthorDoor(shen_name subject);
void RestoreAuthorDoors();
std::uint32_t SetDoorSubjectEnabled(shen_name subject, std::int32_t enabled);

} } // namespace shenmue::s2

#endif
