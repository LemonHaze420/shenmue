#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <span>

namespace shenmue::s2 {

struct SceneTextHeap {
    std::byte buffer[0x4000];
};

static_assert(sizeof(SceneTextHeap) == 0x4000);

std::int64_t InitializeHLibMemory(std::int32_t size);
std::int64_t AllocateHLibMemory_USE_Space(std::int32_t size);
std::byte *AllocateVMPL(std::uint64_t size);
std::int64_t FreeSceneBuffer(void *data);
std::int64_t AllocateModelVertexMemory(std::int32_t size, const void *modelRoot, void **data);
std::int64_t FreeModelVertexMemory(const void *modelRoot);
std::int64_t InitializeSceneData();
std::int64_t FreeUnusedSceneData();

SceneTextHeap *AllocateSTCH();
void ClearSTCH();
std::byte *allocateInGlobalHeap(std::int64_t size);
void SetTaskHeapData(void *allocation);
std::byte *AllocateLCDM();
std::byte *LoadSystemTalk();
bool InitializeWeatherParameters();
bool LoadWeatherConditionData();
enum class FilepathGenMode : std::int32_t {
    SceneDirectory = 0,
    MiscDirectory = 1,
    SoundDirectory = 2,
    StreamDirectory = 3,
    SpriteDirectory = 4,
    ItemDirectory = 5,
    CharaDirectory = 6,
    PackDirectory = 7,
};


const char *Filepath_Gen(FilepathGenMode mode);
bool AllocateFlagWork(std::int32_t actNumber);
bool LoadMemoData();
extern std::int32_t g_s2JapaneseVersion;
extern bool g_s2AllSubtitleLanguages;
extern std::int32_t g_s2SelectedSubtitleLanguage;
bool GetS2IsEnglishVersion() noexcept;
std::span<const std::int32_t> GetS2AvailableSubtitleLanguages() noexcept;
std::int32_t GetLoadingScreenTextType(std::int32_t requestedLanguage) noexcept;
void ApplyS2SubtitleLanguage(std::int32_t requestedLanguage) noexcept;
std::int32_t ResolveSceneTextLanguage(std::int32_t requestedLanguage = -1);
bool LoadLocalizedSceneText(std::int32_t requestedLanguage = -1);
std::int32_t ResetSceneTextLanguage();
std::string_view GetActionSelectorLabel(std::uint16_t labelId);



std::string_view GetLocalizedSceneText(std::string_view token);



const char *FindSceneTextRemapped(const char *key, std::int32_t japaneseMode);


struct SceneTextCsvEntry {
    std::array<std::byte, 8> key;
    std::uint32_t textOffset;
};
static_assert(sizeof(SceneTextCsvEntry) == 12);

struct UIStringBuffer {
    char *base;
    std::uint32_t used;
    std::uint32_t capacity;
};
static_assert(sizeof(UIStringBuffer) == 16);
inline constexpr std::size_t kSceneTextCsvOverrideCapacity = 98000;

extern SceneTextHeap *g_sceneTextHeap;
extern std::uint64_t g_sceneTextHeapCursor;
extern std::byte *g_lcdMemory;
extern std::uint32_t g_lcdMemorySize;
extern std::byte *g_systemTalkData;
extern std::byte *g_weatherData;
extern std::uint32_t g_weatherDataSize;
extern std::uint32_t *g_weatherCloudTimelineNodes;
extern std::uint32_t *g_weatherRainTimelineNodes;
extern std::uint32_t *g_weatherSnowTimelineNodes;
extern std::int32_t g_weatherCloudTimelineNodeCount;
extern std::int32_t g_weatherRainTimelineNodeCount;
extern std::int32_t g_weatherSnowTimelineNodeCount;
extern std::array<std::byte, 0xf60> g_weatherConditions;
extern std::byte *g_flagDescriptorData;
extern std::uint64_t g_flagDescriptorDataSize;
extern std::uint16_t *g_flagWork;
extern std::byte *g_memoData;
extern std::byte *g_memoMasterTable;
extern std::byte *g_memoRubyTable;
extern std::uint32_t g_memoRecordCount;
extern std::int32_t g_sceneTextLanguage;
extern bool g_sceneTextAvailable;

} // namespace shenmue::s2
