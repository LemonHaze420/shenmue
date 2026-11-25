#include "s2/character_control_constructor_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
Character *ctrlGetRootNode() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
}

std::int32_t ctrlIsRootNode(Character *character) {
    return character == static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
}

} // namespace s2
} // namespace shenmue

#endif
