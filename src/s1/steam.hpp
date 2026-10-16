#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

#pragma pack(push, 1)
struct SteamControllerInput {
    std::int32_t button01;
    std::int32_t button2000;
    std::int32_t button4000;
    std::int32_t button8000;
    std::int32_t dpadRight;
    std::int32_t dpadLeft;
    std::int32_t dpadUp;
    std::int32_t dpadDown;
    std::int32_t button0200;
    std::int32_t button0100;
    std::int32_t button0010;
    float normalizedLX;
    float normalizedLY;
    float normalizedRX;
    float normalizedRY;
    float leftTrigger;
    float rightTrigger;
};
#pragma pack(pop)

static_assert(sizeof(SteamControllerInput) == 0x44);

struct SteamStatDefinition {
    char name[128];
    std::uint32_t type;
    std::int32_t value;
    std::int32_t reserved88;
    std::int32_t minimum;
    std::int32_t maximum;
    std::uint32_t suppressPublish;
};

struct SteamAchievementDefinition {
    char name[128];
    std::int32_t statIndex;
    std::int32_t reserved84;
    std::int32_t threshold;
};

static_assert(sizeof(SteamStatDefinition) == 0x98);
static_assert(offsetof(SteamStatDefinition, value) == 0x84);
static_assert(offsetof(SteamStatDefinition, minimum) == 0x8c);
static_assert(offsetof(SteamStatDefinition, suppressPublish) == 0x94);
static_assert(sizeof(SteamAchievementDefinition) == 0x8c);

using SteamInputHandle = std::uint64_t;
using SteamInputPoll = std::int64_t (*)(std::uint32_t mode, SteamControllerInput *input, const SteamInputHandle *controllers, std::size_t controllerCount);
using SteamCallResultCompletion = void (*)(void *user, std::uint64_t call, std::int32_t result, std::uint32_t byteCount, bool ioFailure);

struct SteamRuntimeBindings {
    bool callbacksEnabled{};
    bool (*initializeInput)(){};
    std::size_t (*getConnectedControllers)(SteamInputHandle *controllers, std::size_t capacity){};
    void (*activateActionSet)(SteamInputHandle controller, SteamInputHandle actionSet){};
    void (*runInputFrame)(){};
    SteamInputPoll pollInput{};
    SteamInputHandle inputActionSets[9]{};

    SteamStatDefinition *stats{};
    std::size_t statCount{};
    SteamAchievementDefinition *achievements{};
    std::size_t achievementCount{};
    bool (*setStat)(const char *name, std::int32_t value){};
    bool (*setAchievement)(const char *name){};
    bool (*indicateAchievementProgress)(const char *name, std::uint32_t current, std::uint32_t maximum){};
    bool (*storeStats)(){};

    bool (*remoteFileExists)(const char *path){};
    std::int32_t (*remoteGetFileSize)(const char *path){};
    std::uint64_t (*remoteFileReadAsync)(const char *path, std::uint32_t offset, std::uint32_t size){};
    bool (*remoteFileRead)(std::uint64_t call, void *destination, std::uint32_t size){};
    std::uint64_t (*remoteFileWriteAsync)(const char *path, const void *source, std::uint32_t size){};
    bool (*remoteGetQuota)(std::uint64_t *total, std::uint64_t *available){};
    void (*registerCallResult)(std::uint64_t call, SteamCallResultCompletion completion, void *user){};
    void (*unregisterCallResult)(std::uint64_t call, SteamCallResultCompletion completion, void *user){};
    void (*runCallbacks)(){};
};

void Steam_BindRuntime(const SteamRuntimeBindings &bindings) noexcept;
extern bool g_steamInputOverrideActive;
extern std::int32_t g_steamInputOverrideState;

std::uint32_t GetKeyStateDataForCurrentInputMode(std::int32_t argument);
std::int64_t PollSteamInputEvents(SteamControllerInput *input);

using SteamStorageCallback = void (*)(std::int32_t event, std::int32_t status, void *context);
bool SteamRemoteStorage_IsEnabled() noexcept;
std::uint64_t SteamRemoteStorage_FileSize(const char *nativePath) noexcept;
std::int32_t SteamRemoteStorage_CheckOpen(const char *nativePath) noexcept;
bool SteamRemoteStorage_ReadAsync(
    const char *nativePath, void *destination, std::uint32_t offset, std::uint32_t size, SteamStorageCallback callback, void *context) noexcept;
bool SteamRemoteStorage_WriteAsync(const char *nativePath, const void *source, std::uint32_t size, SteamStorageCallback callback, void *context) noexcept;
std::uint64_t SteamRemoteStorage_AvailableBytes() noexcept;
void SteamRemoteStorage_RunCallbacks() noexcept;

} // namespace shenmue::s1
