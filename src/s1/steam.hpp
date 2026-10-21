#pragma once

#include <array>
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
    std::int32_t achievementIndex;
    std::int32_t threshold;
};

static_assert(sizeof(SteamStatDefinition) == 0x98);
static_assert(offsetof(SteamStatDefinition, value) == 0x84);
static_assert(offsetof(SteamStatDefinition, minimum) == 0x8c);
static_assert(offsetof(SteamStatDefinition, suppressPublish) == 0x94);
static_assert(sizeof(SteamAchievementDefinition) == 0x8c);

struct CapsuleToyStatistics {
    char name[128];
    std::int32_t entryCount;
    std::uint32_t trackNewItems;
    std::int32_t *collected;
};
static_assert(sizeof(CapsuleToyStatistics) == 144 && offsetof(CapsuleToyStatistics, entryCount) == 128 &&
    offsetof(CapsuleToyStatistics, trackNewItems) == 132 && offsetof(CapsuleToyStatistics, collected) == 136);
inline constexpr CapsuleToyStatistics kCapsuleToyStatisticsDefinition{.name = "Data_CapsuleToys", .entryCount = 168, .trackNewItems = 1};
inline constexpr std::size_t kCapsuleToyStatisticsDefaultBytes = 140;
extern CapsuleToyStatistics *g_capsuleToyStatistics;
extern std::int32_t g_capsuleToyStatisticCount;
void RefreshSteamStatForItem(std::int32_t itemId);
enum SteamStatId : std::uint32_t {
    Stat_Call_Police = 10,
    Stat_Capsule_Toy_Count = 23,
    Stat_Moves_Learned = 24,
};

enum SteamStatUpdateType : std::uint32_t {
    SS_Set = 0,
    SS_Add = 1,
};

inline constexpr std::array<SteamStatDefinition, 29> kSteamStatDefinitions{{
    {.name = "SM1_Stat_Meet_Charlie", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_translate_Letter", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Reach_Harbor", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Break_And_Enter", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Find_Phoenix_Mirror", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Get_A_Job", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Rescue_Nozomi", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Brawl", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Beat_Chai_Harbor", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Win_Game", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Call_Police", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Tree_Flashback", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Kitchen_Flashback", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Dojo_Flashback", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Teach_Fuku_San", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Help_Lady_Find_House", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Buy_Guy_Soda", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Take_Mirror_To_Antiques", .type = SS_Add, .maximum = 2, .suppressPublish = 0},
    {.name = "SM1_Stat_Find_Kitten", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Learn_From_Tom", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Win_Forklift_Race", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Visit_Fortune_Teller", .type = SS_Set, .maximum = 1, .suppressPublish = 0},
    {.name = "SM1_Stat_Arcade_Games_Played", .type = SS_Set, .maximum = 2, .suppressPublish = 0},
    {.name = "SM1_Stat_Capsule_Toy_Count", .type = SS_Add, .maximum = 168, .suppressPublish = 0},
    {.name = "SM1_Stat_Moves_Learned", .type = SS_Add, .maximum = 50, .suppressPublish = 0},
    {.name = "Data_SpaceHarrier", .type = SS_Set, .maximum = 1, .suppressPublish = 1},
    {.name = "Data_Hangon", .type = SS_Set, .maximum = 1, .suppressPublish = 1},
    {.name = "Data_Take_Mirror_To_Antiques1", .type = SS_Set, .maximum = 1, .suppressPublish = 1},
    {.name = "Data_Take_Mirror_To_Antiques2", .type = SS_Set, .maximum = 1, .suppressPublish = 1},
}};

inline constexpr std::array<SteamAchievementDefinition, 28> kSteamAchievementDefinitions{{
    {.name = "SM1_Achievement_Meet_Charlie", .statIndex = 0, .achievementIndex = 0, .threshold = 1},
    {.name = "SM1_Achievement_translate_Letter", .statIndex = 1, .achievementIndex = 1, .threshold = 1},
    {.name = "SM1_Achievement_Reach_Harbor", .statIndex = 2, .achievementIndex = 2, .threshold = 1},
    {.name = "SM1_Achievement_Break_And_Enter", .statIndex = 3, .achievementIndex = 3, .threshold = 1},
    {.name = "SM1_Achievement_Find_Phoenix_Mirror", .statIndex = 4, .achievementIndex = 4, .threshold = 1},
    {.name = "SM1_Achievement_Get_A_Job", .statIndex = 5, .achievementIndex = 5, .threshold = 1},
    {.name = "SM1_Achievement_Rescue_Nozomi", .statIndex = 6, .achievementIndex = 6, .threshold = 1},
    {.name = "SM1_Achievement_Brawl", .statIndex = 7, .achievementIndex = 7, .threshold = 1},
    {.name = "SM1_Achievement_Beat_Chai_Harbor", .statIndex = 8, .achievementIndex = 8, .threshold = 1},
    {.name = "SM1_Achievement_Win_Game", .statIndex = 9, .achievementIndex = 9, .threshold = 1},
    {.name = "SM1_Achievement_Call_Police", .statIndex = 10, .achievementIndex = 10, .threshold = 1},
    {.name = "SM1_Achievement_Tree_Flashback", .statIndex = 11, .achievementIndex = 11, .threshold = 1},
    {.name = "SM1_Achievement_Kitchen_Flashback", .statIndex = 12, .achievementIndex = 12, .threshold = 1},
    {.name = "SM1_Achievement_Dojo_Flashback", .statIndex = 13, .achievementIndex = 13, .threshold = 1},
    {.name = "SM1_Achievement_Teach_Fuku_San", .statIndex = 14, .achievementIndex = 14, .threshold = 1},
    {.name = "SM1_Achievement_Help_Lady_Find_House", .statIndex = 15, .achievementIndex = 15, .threshold = 1},
    {.name = "SM1_Achievement_Buy_Guy_Soda", .statIndex = 16, .achievementIndex = 16, .threshold = 1},
    {.name = "SM1_Achievement_Take_Mirror_To_Antiques", .statIndex = 17, .achievementIndex = 17, .threshold = 2},
    {.name = "SM1_Achievement_Find_Kitten", .statIndex = 18, .achievementIndex = 18, .threshold = 1},
    {.name = "SM1_Achievement_Learn_From_Tom", .statIndex = 19, .achievementIndex = 19, .threshold = 1},
    {.name = "SM1_Achievement_Win_Forklift_Race", .statIndex = 20, .achievementIndex = 20, .threshold = 1},
    {.name = "SM1_Achievement_Visit_Fortune_Teller", .statIndex = 21, .achievementIndex = 21, .threshold = 1},
    {.name = "SM1_Achievement_Play_An_Arcade_Game", .statIndex = 22, .achievementIndex = 22, .threshold = 1},
    {.name = "SM1_Achievement_Collect_Capsule_Toy", .statIndex = 23, .achievementIndex = 23, .threshold = 1},
    {.name = "SM1_Achievement_Collect_25_Capsule_Toys", .statIndex = 23, .achievementIndex = 24, .threshold = 25},
    {.name = "SM1_Achievement_Collect_50_Capsule_Toys", .statIndex = 23, .achievementIndex = 25, .threshold = 50},
    {.name = "SM1_Achievement_Learn_New_Move", .statIndex = 24, .achievementIndex = 26, .threshold = 1},
    {.name = "SM1_Achievement_Learn_5_Moves", .statIndex = 24, .achievementIndex = 27, .threshold = 5},
}};

extern SteamStatDefinition *g_steamTemporaryStats;
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
extern SteamStorageCallback g_steamSaveFunction;
void SteamSaveFn(std::int32_t event, std::int32_t status, void *data);
bool SteamRemoteStorage_IsEnabled() noexcept;
std::uint64_t SteamRemoteStorage_FileSize(const char *nativePath) noexcept;
std::int32_t SteamRemoteStorage_CheckOpen(const char *nativePath) noexcept;
bool SteamRemoteStorage_ReadAsync(
    const char *nativePath, void *destination, std::uint32_t offset, std::uint32_t size, SteamStorageCallback callback, void *context) noexcept;
bool SteamRemoteStorage_WriteAsync(const char *nativePath, const void *source, std::uint32_t size, SteamStorageCallback callback, void *context) noexcept;
std::uint64_t SteamRemoteStorage_AvailableBytes() noexcept;
void SteamRemoteStorage_RunCallbacks() noexcept;

} // namespace shenmue::s1
