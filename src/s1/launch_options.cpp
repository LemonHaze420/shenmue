#include "s1/launch_options.hpp"

namespace shenmue::s1 {

MapLaunchOptions g_mapLaunchOptions{};
MapLaunchOptions g_transitionValidationOptions{};
FrontendLaunchTarget g_frontendLaunchTarget = FrontendLaunchTarget::None;
bool g_skipFrontendSplash = false;
bool g_frontendOptionsHold = false;
bool g_hideValidationWindow = false;

} // namespace shenmue::s1
