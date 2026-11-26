#include "s2/map_render_profile.hpp"

#if !SHENMUE_IS_DC
#include <array>
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC
const MapRenderProfile *g_mapRenderProfiles = &initialMapRenderProfile;
#else
namespace {
#include "s2/map_render_profiles.inl"
} // namespace
#endif

// @verified - pd
const MapRenderProfile& MatchMapRenderProfile(shen_name area) {
#if SHENMUE_IS_DC
    const MapRenderProfile *profile = g_mapRenderProfiles;
#else
    const MapRenderProfile* profile = mapRenderProfiles.data();
#endif
    while (profile->areaPattern != 0) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        std::uint32_t pattern = profile->areaPattern;
        std::uint32_t name = area;
        if (static_cast<std::uint8_t>(pattern) == '*' || static_cast<std::uint8_t>(pattern) == static_cast<std::uint8_t>(name)) {
            pattern >>= 8;
            name >>= 8;
            if (static_cast<std::uint8_t>(pattern) == '*' || static_cast<std::uint8_t>(pattern) == static_cast<std::uint8_t>(name)) {
                pattern >>= 8;
                name >>= 8;
                if (static_cast<std::uint8_t>(pattern) == '*' || static_cast<std::uint8_t>(pattern) == static_cast<std::uint8_t>(name)) {
                    pattern >>= 8;
                    name >>= 8;
                    if (static_cast<std::uint8_t>(pattern) == '*' || static_cast<std::uint8_t>(pattern) == static_cast<std::uint8_t>(name))
                        return *profile;
                }
            }
        }
#else
        bool matches = true;
        for (std::uint32_t shift = 0; shift != 32; shift += 8) {
            const std::uint8_t pattern = static_cast<std::uint8_t>(profile->areaPattern >> shift);
            if (pattern != '*' && pattern != static_cast<std::uint8_t>(static_cast<std::uint32_t>(area) >> shift)) {
                matches = false;
                break;
            }
        }
        if (matches)
            return *profile;
#endif
        ++profile;
    }
    return *profile;
}

} } // namespace shenmue::s2
