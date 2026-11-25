#include "s2/scenario_character_query_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#include "s2/scenario_talk_dc.hpp"

namespace shenmue {
namespace s2 {

shen_name ctrlGetScenarioCharaName(const Character *character);

// @verified - d
bool HasCharacterScenarioName(const Character *character, const char *name) {
    const shen_name current = ctrlGetScenarioCharaName(character);
    if (current ==
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(name[3])) << 24 | static_cast<std::uint32_t>(static_cast<std::uint8_t>(name[2])) << 16 |
            static_cast<std::uint32_t>(static_cast<std::uint8_t>(name[1])) << 8 | static_cast<std::uint8_t>(name[0])))
        return 1;
    return 0;
}

} // namespace s2
} // namespace shenmue

#endif
