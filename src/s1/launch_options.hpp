#pragma once

#include "common/types.hpp"

#include <cstdint>

#if defined(SHENMUE_TARGET_WHATS_PC)
#include <filesystem>
#endif

namespace shenmue::s1 {

#if defined(SHENMUE_TARGET_WHATS_PC)
#pragma region WhatsShenmue
struct TrialLaunchOptions {
    std::filesystem::path gameDirectory;
    std::filesystem::path hostDirectory;
};
extern TrialLaunchOptions g_trialLaunchOptions;
#pragma endregion
#endif

inline constexpr shen_name AREA_OPENING{SHEN_NAME(OP00)};

struct MapLaunchOptions {
    bool active;
    std::int32_t sceneId;
    shen_name areaId;
    std::uint32_t entryPoint;
};

enum class FrontendLaunchTarget : std::uint8_t {
    None,
    MainMenu,
    NewGame,
    Continue,
    Options,
};

extern bool g_skipFrontendSplash;
extern FrontendLaunchTarget g_frontendLaunchTarget;
extern bool g_frontendOptionsHold;
extern bool g_hideValidationWindow;

extern MapLaunchOptions g_mapLaunchOptions;
extern MapLaunchOptions g_transitionValidationOptions;

} // namespace shenmue::s1
