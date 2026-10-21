#include "s1/d3t/namespace.hpp"
#include "s1/initialization.hpp"
#include "s1/input.hpp"
#include "s1/steam.hpp"

#include <cstring>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace shenmue::s1 {
namespace {

SteamRuntimeBindings g_steamRuntime;

} // namespace

void Steam_BindRuntime(const SteamRuntimeBindings &bindings) noexcept {
    g_steamRuntime = bindings;
}

bool Steam_InitApp(const SteamAppInfo *) {
    return true;
}

bool Steam_TryGetCurrentUserId(std::uint64_t *) {
    return false;
}

void SteamInit() {
}
void UpdateSteamStat(std::uint32_t) {
}

void UpdateSteamAchievement(std::uint32_t statIndex) {
    if (!g_steamRuntime.stats || statIndex >= g_steamRuntime.statCount) {
        return;
    }

    const SteamStatDefinition &stat = g_steamRuntime.stats[statIndex];
    if (stat.type != 1 || stat.suppressPublish != 0) {
        return;
    }

    for (std::size_t index = 0; index < g_steamRuntime.achievementCount; ++index) {
        const SteamAchievementDefinition &achievement = g_steamRuntime.achievements[index];
        if (achievement.statIndex != static_cast<std::int32_t>(statIndex)) {
            continue;
        }
        const auto current = static_cast<std::uint32_t>(stat.value);
        const auto maximum = static_cast<std::uint32_t>(achievement.threshold);
        if (current < maximum) {
            if (g_steamRuntime.callbacksEnabled && g_steamRuntime.indicateAchievementProgress) {
                g_steamRuntime.indicateAchievementProgress(achievement.name, current, maximum);
            }
            return;
        }
        if (current == maximum) {
            return;
        }
    }
}

void SteamRemoteStorage_RunCallbacks() noexcept {
    if (g_steamRuntime.callbacksEnabled && g_steamRuntime.runCallbacks)
        g_steamRuntime.runCallbacks();
}

void SteamPlatformContextRefresh() {
}

namespace d3t {
void SetupAchievements() {
}

void InitializeSteamAPI() {
    SteamInit();
}
} // namespace d3t




void SGAAS_Init(std::uint64_t) {
}

std::int64_t PollSteamInputEvents(SteamControllerInput *input) {
    if (input) {
        std::memset(input, 0, sizeof(*input));
    }
    return 0;
}

} // namespace shenmue::s1
