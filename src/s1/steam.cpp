#if defined(SHENMUE_STEAM)
#include "d3t/steam.hpp"
#endif

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
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::RequireSteamInitialized();
    return true;
#else
    return true;
#endif
}

bool Steam_TryGetCurrentUserId(std::uint64_t *userId) {
#if defined(SHENMUE_STEAM)
    if (!userId) return false;
    *userId = ::shenmue::d3t::GetSteamUserId();
    return true;
#else
    return false;
#endif
}

void SteamInit() {
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::RequireSteamInitialized();
#endif
}
void PublishSteamStats() {
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::PublishSteamStats();
#else
    if (!g_steamRuntime.callbacksEnabled)
        return;
    for (std::size_t index = 0; index < g_steamRuntime.statCount; ++index) {
        const SteamStatDefinition &stat = g_steamTemporaryStats[index];
        if (stat.suppressPublish == 0 && g_steamRuntime.setStat)
            g_steamRuntime.setStat(stat.name, stat.value);
    }
    if (g_steamRuntime.storeStats)
        g_steamRuntime.storeStats();
#endif
}

void UpdateSteamStat(std::uint32_t statIndex) {
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::UpdateSteamStat(static_cast<std::int32_t>(statIndex));
#else
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
#endif
}

void UpdateSteamAchievement(std::uint32_t statIndex) {
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::UpdateSteamAchievement(static_cast<std::int32_t>(statIndex));
#else
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
#endif
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
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::RunSteamCallbacks();
#else
    if (g_steamRuntime.callbacksEnabled && g_steamRuntime.runCallbacks)
        g_steamRuntime.runCallbacks();
#endif
}

void SteamPlatformContextRefresh() {
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::RefreshSteamContext();
#endif
}

namespace d3t {
void SetupAchievements() {
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::RequireSteamInitialized();
    std::memcpy(g_defaultSteamStats.data(), ::shenmue::d3t::OriginalSteamStats(), sizeof(g_defaultSteamStats));
    std::memcpy(g_defaultSteamAchievements.data(), ::shenmue::d3t::OriginalSteamAchievements(), sizeof(g_defaultSteamAchievements));
#else
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
#endif
}

void InitializeSteamAPI() {
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::RequireSteamInitialized();
    g_steamTemporaryStats = static_cast<SteamStatDefinition *>(::shenmue::d3t::OriginalSteamStats());
    g_capsuleToyStatistics = static_cast<CapsuleToyStatistics *>(::shenmue::d3t::OriginalSteamCapsuleStatistics());
    g_capsuleToyStatisticCount = ::shenmue::d3t::OriginalSteamCapsuleCount();
    g_steamRuntime.callbacksEnabled = true;
    g_steamRuntime.stats = g_steamTemporaryStats;
    g_steamRuntime.statCount = 29;
    g_steamRuntime.achievements = static_cast<SteamAchievementDefinition *>(::shenmue::d3t::OriginalSteamAchievements());
    g_steamRuntime.achievementCount = 28;
    g_steamRuntime.indicateAchievementProgress = ::shenmue::d3t::IndicateSteamAchievementProgress;
    g_steamRuntime.runCallbacks = ::shenmue::d3t::RunSteamCallbacks;
#else
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
#endif
}
} // namespace d3t




#if !defined(SHENMUE_STEAM)
void SGAAS_Init(std::uint64_t) {
}
#endif

std::int64_t PollSteamInputEvents(SteamControllerInput *input) {
#if defined(SHENMUE_STEAM)
    if (!input) return 0;
    return ::shenmue::d3t::PollSteamInput(GetKeyStateDataForCurrentInputMode(g_xInputReserved1), input);
#else
    if (input) {
        std::memset(input, 0, sizeof(*input));
    }
    return 0;
#endif
}

} // namespace shenmue::s1
