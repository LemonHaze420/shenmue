#pragma once

#include "shenmue.h"
#if !defined(SHENMUE_STEAM) || !defined(_WIN32) || !SHENMUE_IS_PC
#error Steam is only supported on Windows
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <vector>

namespace shenmue::d3t {

struct ExecutableInfo {
    const wchar_t *executable;
    std::uint32_t timestamp;
    std::uint32_t minimumImageSize;
    std::uint32_t controllerCall;
    std::array<std::uint8_t, 10> controllerCallBytes;
    std::uint32_t initializeController;
    std::array<std::uint8_t, 12> initializeControllerBytes;
    std::uint32_t platformOwner;
    std::uint32_t contextDescriptor;
    std::uint32_t drainWorkers;
    std::uint32_t joinWorkers;
    std::uint32_t window;
    std::uint32_t swapChain;
    std::uint32_t quitRequested;
    std::uint32_t exitProcess;
    std::uint32_t callbacksEnabled;
    std::uint32_t stats;
    std::uint32_t statCount;
    std::uint32_t achievements;
    std::uint32_t achievementCount;
    std::uint32_t capsuleStatistics;
    std::uint32_t capsuleCount;
    std::uint32_t inputReady;
    std::uint32_t actionSets;
    std::uint32_t inputHandlers;
    std::uint32_t currentInputHandler;
    std::uint32_t readStorage;
    std::uint32_t writeStorage;
    std::uint32_t buildStoragePath;
};

inline constexpr ExecutableInfo S1ExeInfo{
    .executable = L"Shenmue.exe", .timestamp = 0x5be4de30, .minimumImageSize = 0xa01b000,
    .controllerCall = 0x8d6b5, .controllerCallBytes = {0xe8, 0x06, 0xbe, 0x0d, 0x00, 0x48, 0x89, 0x35, 0x9f, 0xa1},
    .initializeController = 0x1694c0, .initializeControllerBytes = {0x48, 0x83, 0xec, 0x28, 0x48, 0x8d, 0x05, 0xe5, 0xe1, 0xff, 0xff, 0x48},
    .platformOwner = 0x132f9c8, .contextDescriptor = 0xa6b270, .drainWorkers = 0x2b8f30, .joinWorkers = 0x2bdbd0,
    .window = 0xed8270, .swapChain = 0xed8128, .quitRequested = 0xeb01eb, .exitProcess = 0x30f528,
    .callbacksEnabled = 0xd87da5, .stats = 0xd881c0, .statCount = 0xd881b0, .achievements = 0xd881b8, .achievementCount = 0xd88054,
    .capsuleStatistics = 0xd88058, .capsuleCount = 0xd881b4,
    .inputReady = 0xc60690, .actionSets = 0xc606a0, .inputHandlers = 0x559780, .currentInputHandler = 0xc60a38,
    .readStorage = 0x2491a0, .writeStorage = 0x249270, .buildStoragePath = 0,
};

inline constexpr ExecutableInfo S2ExeInfo{
    .executable = L"Shenmue2.exe", .timestamp = 0x5be4e77a, .minimumImageSize = 0x829d000,
    .controllerCall = 0x4b29bd, .controllerCallBytes = {0xe8, 0xde, 0xde, 0xf6, 0xff, 0xe8, 0x69, 0xe2, 0x07, 0x00},
    .initializeController = 0x4208a0, .initializeControllerBytes = {0x48, 0x83, 0xec, 0x28, 0x48, 0x8b, 0x05, 0x35, 0xb4, 0x53, 0x00, 0x48},
    .platformOwner = 0x433f478, .contextDescriptor = 0x9f73e0, .drainWorkers = 0x3a1880, .joinWorkers = 0x371030,
    .window = 0x3ee7d28, .swapChain = 0x3ee7be0, .quitRequested = 0x3ee0b8f, .exitProcess = 0x61facc,
    .callbacksEnabled = 0x3db7be4, .stats = 0x3db7bf8, .statCount = 0x3db7bec, .achievements = 0x3db7bf0, .achievementCount = 0x3db7be8,
    .capsuleStatistics = 0, .capsuleCount = 0,
    .inputReady = 0xd14bd0, .actionSets = 0xd14be0, .inputHandlers = 0x95bce0, .currentInputHandler = 0xd14f78,
    .readStorage = 0x18620, .writeStorage = 0x186f0, .buildStoragePath = 0x17b60,
};

inline constexpr const ExecutableInfo &ExeInfo = SHENMUE_IS_S1 ? S1ExeInfo : S2ExeInfo;

struct SteamControllerInput {
    std::int32_t button01, button2000, button4000, button8000;
    std::int32_t dpadRight, dpadLeft, dpadUp, dpadDown;
    std::int32_t button0200, button0100, button0010;
    float normalizedLX, normalizedLY, normalizedRX, normalizedRY, leftTrigger, rightTrigger;
};
static_assert(sizeof(SteamControllerInput) == 0x44 && offsetof(SteamControllerInput, normalizedLX) == 0x2c);

using SteamStorageCompletion = void (*)(std::int32_t stage, std::int32_t status, void *user);

bool InstallSteam() noexcept;
bool IsSteamInitialized() noexcept;
void RequireSteamInitialized();
std::filesystem::path GetExecutableDirectory();
void RunSteamCallbacks();
void ShutdownSteamStorage();
[[noreturn]] void ExitOriginalExecutable(int status);
std::int32_t GetSteamFileSize(const char *name);
bool SteamFileExists(const char *name);
bool DeleteSteamFile(const char *name);
std::uint64_t GetSteamStorageAvailableBytes();
bool ReadSteamFileAsync(const char *name, void *destination, std::uint32_t offset, std::uint32_t size, SteamStorageCompletion completion, void *user);
bool WriteSteamFileAsync(const char *name, const void *source, std::uint32_t size, SteamStorageCompletion completion, void *user);
bool ReadSteamFile(const char *name, void *destination, std::uint32_t offset, std::uint32_t size);
bool WriteSteamFile(const char *name, const void *source, std::uint32_t size);
std::vector<std::byte> ReadSteamFileBytes(const char *name);
std::int64_t PollSteamInput(std::uint32_t mode, void *output);
#if SHENMUE_IS_S1
std::int32_t OriginalSteamGameInputLayout();
#endif
void *OriginalSteamStats();
void *OriginalSteamAchievements();
void *OriginalSteamCapsuleStatistics();
std::int32_t OriginalSteamCapsuleCount();
std::uint64_t GetSteamUserId();
void RefreshSteamContext();
bool IndicateSteamAchievementProgress(const char *name, std::uint32_t current, std::uint32_t maximum);
void ReadSteamStatisticsFile(std::int32_t slot);
void WriteSteamStatisticsFile(std::int32_t slot);
void PublishSteamStats();
void UpdateSteamStat(std::int32_t stat);
void UpdateSteamAchievement(std::int32_t stat);
void IncrementSteamStat(std::int32_t stat);
void ResetSteamStats();
void CheckSteamStats();

} // namespace shenmue::d3t
