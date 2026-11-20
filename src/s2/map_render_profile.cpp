#include "s2/map_render_profile.hpp"

#include <array>

namespace shenmue { namespace s2 {

namespace {
#include "s2/map_render_profiles.inl"
} // namespace

// @verified - pd
const MapRenderProfile& MatchMapRenderProfile(shen_name area) {
    const MapRenderProfile* profile = mapRenderProfiles.data();
    while (profile->areaPattern != 0) {
        bool matches = true;
        for (std::uint32_t shift = 0; shift != 32; shift += 8) {
            const std::uint8_t pattern = static_cast<std::uint8_t>(profile->areaPattern >> shift);
            if (pattern != '*' &&
                pattern != static_cast<std::uint8_t>(area.name >> shift)) {
                matches = false;
                break;
            }
        }
        if (matches) return *profile;
        ++profile;
    }
    return *profile;
}

} } // namespace shenmue::s2
