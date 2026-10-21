#include "s1/d3t/namespace.hpp"
#include "s1/initialization.hpp"
#include "s1/input.hpp"
#include "s1/steam.hpp"

#include <bit>
#include <cstring>
#include <cstdlib>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace shenmue::s1 {
SteamStatDefinition *g_steamTemporaryStats = nullptr;
CapsuleToyStatistics *g_capsuleToyStatistics = nullptr;
std::int32_t g_capsuleToyStatisticCount = 0;
namespace {

std::array<SteamStatDefinition, 29> g_defaultSteamStats{};
std::array<SteamAchievementDefinition, 28> g_defaultSteamAchievements{};
SteamRuntimeBindings g_steamRuntime;

} // namespace

void Steam_BindRuntime(const SteamRuntimeBindings &bindings) noexcept {
    g_steamRuntime = bindings;
    g_steamTemporaryStats = bindings.stats;
}

bool Steam_InitApp(const SteamAppInfo *) {
    return true;
}

bool Steam_TryGetCurrentUserId(std::uint64_t *) {
    return false;
}

void SteamInit() {
}
void PublishSteamStats() {
    if (!g_steamRuntime.callbacksEnabled)
        return;
    for (std::size_t index = 0; index < g_steamRuntime.statCount; ++index) {
        const SteamStatDefinition &stat = g_steamTemporaryStats[index];
        if (stat.suppressPublish == 0 && g_steamRuntime.setStat)
            g_steamRuntime.setStat(stat.name, stat.value);
    }
    if (g_steamRuntime.storeStats)
        g_steamRuntime.storeStats();
}

void UpdateSteamStat(std::uint32_t statIndex) {
    if (!g_steamTemporaryStats || statIndex >= g_steamRuntime.statCount)
        return;
    SteamStatDefinition *stats = g_steamTemporaryStats;
    SteamStatDefinition &stat = stats[statIndex];
    if (stat.value > stat.maximum && stat.type <= SS_Add)
        stat.value = stat.maximum;
    if (stat.value < stat.minimum && stat.type <= SS_Add)
        stat.value = stat.minimum;

    for (std::size_t index = 0; index < g_steamRuntime.achievementCount; ++index) {
        const SteamAchievementDefinition &achievement = g_steamRuntime.achievements[index];
        if (achievement.statIndex == static_cast<std::int32_t>(statIndex) && stats[statIndex].type <= SS_Add &&
            stats[statIndex].value >= achievement.threshold && g_steamRuntime.callbacksEnabled) {
            if (g_steamRuntime.setAchievement)
                g_steamRuntime.setAchievement(achievement.name);
            PublishSteamStats();
            stats = g_steamTemporaryStats;
        }
    }
}

void UpdateSteamAchievement(std::uint32_t statIndex) {
    if (!g_steamTemporaryStats || statIndex >= g_defaultSteamStats.size())
        return;
    const SteamStatDefinition &definition = g_defaultSteamStats[statIndex];
    if (definition.type != SS_Add || definition.suppressPublish != 0)
        return;
    for (const SteamAchievementDefinition &achievement : g_defaultSteamAchievements) {
        if (achievement.statIndex != static_cast<std::int32_t>(statIndex))
            continue;
        const std::uint32_t current = static_cast<std::uint32_t>(g_steamTemporaryStats[statIndex].value);
        const std::uint32_t maximum = static_cast<std::uint32_t>(achievement.threshold);
        if (current < maximum) {
            if (g_steamRuntime.callbacksEnabled && g_steamRuntime.indicateAchievementProgress)
                g_steamRuntime.indicateAchievementProgress(achievement.name, current, maximum);

            return;
        }
        if (current == maximum)
            return;
    }
}

// @verified - p
void RefreshSteamStatForItem(std::int32_t itemId) {
    const std::int32_t index = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(itemId) - 67U);
    if (g_capsuleToyStatistics->collected[index] != 0)
        return;
    if (g_capsuleToyStatistics->trackNewItems == 1)
        g_capsuleToyStatistics->collected[index] = 1;
    SteamStatDefinition &stat = g_steamTemporaryStats[Stat_Capsule_Toy_Count];
    if (stat.type == SS_Set)
        stat.value = 1;
    else if (stat.type == SS_Add)
        stat.value = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(stat.value) + 1U);
    UpdateSteamStat(Stat_Capsule_Toy_Count);
    UpdateSteamAchievement(Stat_Capsule_Toy_Count);
}

// @verified - p
void RefreshMirrorAntiquesStat() {
    const std::int32_t previous = g_steamTemporaryStats[Stat_Take_Mirror_To_Antiques].value;
    SteamStatDefinition &stat = g_steamTemporaryStats[Stat_Take_Mirror_To_Antiques];
    if (stat.type <= SS_Add)
        stat.value = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_steamTemporaryStats[Stat_Take_Mirror_To_Antiques1].value) +
            static_cast<std::uint32_t>(g_steamTemporaryStats[Stat_Take_Mirror_To_Antiques2].value));
    UpdateSteamStat(Stat_Take_Mirror_To_Antiques);
    const std::int32_t current = g_steamTemporaryStats[Stat_Take_Mirror_To_Antiques].value;
    const SteamAchievementDefinition &achievement = g_defaultSteamAchievements[Stat_Take_Mirror_To_Antiques];
    if (current < achievement.threshold && current != previous && g_steamRuntime.callbacksEnabled && g_steamRuntime.indicateAchievementProgress)
        g_steamRuntime.indicateAchievementProgress(achievement.name, static_cast<std::uint32_t>(current), static_cast<std::uint32_t>(achievement.threshold));
}

void SteamRemoteStorage_RunCallbacks() noexcept {
    if (g_steamRuntime.callbacksEnabled && g_steamRuntime.runCallbacks)
        g_steamRuntime.runCallbacks();
}

void SteamPlatformContextRefresh() {
}

namespace d3t {
void SetupAchievements() {
    for (std::size_t index = 0; index < g_defaultSteamStats.size(); ++index) {
        SteamStatDefinition &stat = g_defaultSteamStats[index];
        const SteamStatDefinition &definition = kSteamStatDefinitions[index];
        std::strcpy(stat.name, definition.name);
        stat.type = definition.type;
        stat.reserved88 = definition.reserved88;
        stat.minimum = definition.minimum;
        stat.maximum = definition.maximum;
        stat.suppressPublish = definition.suppressPublish;
    }
    for (std::size_t index = 0; index < g_defaultSteamAchievements.size(); ++index) {
        SteamAchievementDefinition &achievement = g_defaultSteamAchievements[index];
        const SteamAchievementDefinition &definition = kSteamAchievementDefinitions[index];
        std::strcpy(achievement.name, definition.name);
        achievement.statIndex = definition.statIndex;
        achievement.achievementIndex = definition.achievementIndex;
        achievement.threshold = definition.threshold;
    }
}

void InitializeSteamAPI() {
    g_steamRuntime.statCount = g_defaultSteamStats.size();
    g_steamTemporaryStats = static_cast<SteamStatDefinition *>(std::malloc(sizeof(g_defaultSteamStats)));
    g_steamRuntime.stats = g_steamTemporaryStats;
    std::memcpy(g_steamTemporaryStats, g_defaultSteamStats.data(), sizeof(g_defaultSteamStats));
    g_steamRuntime.achievementCount = g_defaultSteamAchievements.size();
    g_steamRuntime.achievements = static_cast<SteamAchievementDefinition *>(std::malloc(sizeof(g_defaultSteamAchievements)));
    std::memcpy(g_steamRuntime.achievements, g_defaultSteamAchievements.data(), sizeof(g_defaultSteamAchievements));
    g_capsuleToyStatisticCount = 1;
    CapsuleToyStatistics *const capsuleData = static_cast<CapsuleToyStatistics *>(std::malloc(sizeof(CapsuleToyStatistics)));
    const std::int32_t count = g_capsuleToyStatisticCount;
    g_capsuleToyStatistics = capsuleData;
    std::memcpy(capsuleData, &kCapsuleToyStatisticsDefinition, kCapsuleToyStatisticsDefaultBytes * count);
    if (count > 0) {
        std::int32_t index = 0;
        do {
            std::int32_t *const collected =
                static_cast<std::int32_t *>(std::malloc(static_cast<std::size_t>(g_capsuleToyStatistics[index].entryCount) * sizeof(std::int32_t)));
            CapsuleToyStatistics &record = g_capsuleToyStatistics[index];
            record.collected = collected;
            std::memset(collected, 0, static_cast<std::size_t>(record.entryCount) * sizeof(std::int32_t));
        } while (++index < g_capsuleToyStatisticCount);
    }
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
