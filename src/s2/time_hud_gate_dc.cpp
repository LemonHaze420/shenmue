#include "s2/time_hud_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/inventory_dc.hpp"
#include "s2/item_selection_queries_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/screen_film.hpp"

namespace shenmue {
namespace s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t UpdateSelectorMemo();

bool CheckTimeHudVisibility() {
    bool visible = false;
    if (GetItemState(2) == 0 || g_clockAndCompassSuppressed)
        return visible;
    if (!UpdateSelectorMemo() && !IsAnyItemSelectActive()) {
        const MapControlData *control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (!IsScreenFilmNotShowing(control->filmTask)) {
            control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            if (!DoesScreenFilmColourExist(control->filmTask))
                return visible;
        }
        if (authGetLoaded() || g_moveUiClockAndNavigationHold || g_fightUiActive || IsFightHealthUIActive())
            return visible;
    }
    visible = true;
    return visible;
}
#else
// @verified - d
bool CheckTimeHudVisibility() {
    if (GetItemState(2) == 0 || g_clockAndCompassSuppressed)
        return false;
    if (IsItemSelectable() || IsAnyItemSelectActive())
        return true;
    const MapControlData *control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!IsScreenFilmNotShowing(control->filmTask)) {
        control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (!DoesScreenFilmColourExist(control->filmTask))
            return false;
    }
    if (authGetLoaded() || g_moveUiClockAndNavigationHold || g_fightUiActive || IsFightHealthUIActive())
        return false;
    return true;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
