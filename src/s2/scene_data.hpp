#pragma once

#include "shenmue.h"
#include "common/types.hpp"

#if SHENMUE_S2_GCC295
#define SHENMUE_SCENE_NOEXCEPT
#else
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <span>
#define SHENMUE_SCENE_NOEXCEPT noexcept
#endif

namespace shenmue {
namespace s2 {

struct SceneTextHeap {
    std::byte buffer[0x4000];
};
#if SHENMUE_S2_GCC295
using ::shenmue::shen_name;
#define SCENE_TEXT_HEAP shen_name(SHEN_NAME4('S', 'T', 'C', 'H'))
#else
inline constexpr shen_name SCENE_TEXT_HEAP{SHEN_NAME(STCH)};
#endif

#if SHENMUE_S2_GCC295
typedef char sizeofSceneTextHeapCheck[sizeof(SceneTextHeap) == 0x4000 ? 1 : -1];
#else
static_assert(sizeof(SceneTextHeap) == 0x4000);
#endif

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
std::byte *allocateInGlobalHeap(std::intptr_t size);
void SetTaskHeapData(void *allocation);
std::byte *AllocateLCDM();
std::byte *LoadSystemTalk();
bool InitializeWeatherParameters();
bool LoadWeatherConditionData();
#if SHENMUE_S2_GCC295
namespace FilepathGenMode {
typedef std::int32_t Type;
enum Value {
#else
enum class FilepathGenMode : std::int32_t {
#endif
    SceneDirectory = 0,
    MiscDirectory = 1,
    SoundDirectory = 2,
    StreamDirectory = 3,
    SpriteDirectory = 4,
    ItemDirectory = 5,
    CharaDirectory = 6,
    PackDirectory = 7,
};
#if SHENMUE_S2_GCC295
} // namespace FilepathGenMode
#endif

// Native Filepath_Gen: the returned path lives in one shared buffer until the next call.
#if SHENMUE_S2_GCC295
const char *Filepath_Gen(FilepathGenMode::Type mode);
#else
const char *Filepath_Gen(FilepathGenMode mode);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void AllocateFlagWork();
#else
bool AllocateFlagWork(std::int32_t actNumber);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
void LoadMemoData();
#else
bool LoadMemoData();
#endif
extern std::int32_t g_s2JapaneseVersion;
extern bool g_s2AllSubtitleLanguages;
extern std::int32_t g_s2SelectedSubtitleLanguage;
bool GetS2IsEnglishVersion() SHENMUE_SCENE_NOEXCEPT;
#if !SHENMUE_S2_GCC295
std::span<const std::int32_t> GetS2AvailableSubtitleLanguages() noexcept;
#endif
std::int32_t GetLoadingScreenTextType(std::int32_t requestedLanguage) SHENMUE_SCENE_NOEXCEPT;
void ApplyS2SubtitleLanguage(std::int32_t requestedLanguage) SHENMUE_SCENE_NOEXCEPT;
std::int32_t ResolveSceneTextLanguage(std::int32_t requestedLanguage = -1);
bool LoadLocalizedSceneText(std::int32_t requestedLanguage = -1);
std::int32_t ResetSceneTextLanguage();
#if !SHENMUE_S2_GCC295
std::string_view GetActionSelectorLabel(std::uint16_t labelId);
// PC subtitle records carry a zero-padded ASCII token of up to twelve bytes.
// The returned view is owned by the loaded language table and remains valid
// until LoadLocalizedSceneText is called again.
std::string_view GetLocalizedSceneText(std::string_view token);
#endif
// Native twelve-byte scene-text lookup for voice presentation. `key` points at twelve raw bytes,
// compared as stored (eight for a version-1 table). With japaneseMode set, a key in the ZZ remap index resolves
// through its "ZZ" + first ten bytes alternate. A miss returns the native empty string; only an all-zero key with no table loaded yields null.
const char *FindSceneTextRemapped(const char *key, std::int32_t japaneseMode);

// Native CSV-override entry: an eight-byte scene-text key and its offset in the UI string buffer.
struct SceneTextCsvEntry {
#if SHENMUE_S2_GCC295
    std::byte key[8];
#else
    std::array<std::byte, 8> key;
#endif
    std::uint32_t textOffset;
};
#if SHENMUE_S2_GCC295
typedef char sizeofSceneTextCsvEntryCheck[sizeof(SceneTextCsvEntry) == 12 ? 1 : -1];
#else
static_assert(sizeof(SceneTextCsvEntry) == 12);
#endif
// Native g_UIStringBuffer: the CSV override's string pool.
struct UIStringBuffer {
    char *base;
    std::uint32_t used;
    std::uint32_t capacity;
};
#if !SHENMUE_IS_DC
#if SHENMUE_S2_GCC295
typedef char sizeofUIStringBufferCheck[sizeof(UIStringBuffer) == 16 ? 1 : -1];
#else
static_assert(sizeof(UIStringBuffer) == 16);
#endif
#endif
#if SHENMUE_S2_GCC295
static const std::size_t kSceneTextCsvOverrideCapacity = 98000;
#else
inline constexpr std::size_t kSceneTextCsvOverrideCapacity = 98000;
#endif

extern SceneTextHeap *g_sceneTextHeap;
extern std::size_t g_sceneTextHeapCursor;
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
#if SHENMUE_S2_GCC295
extern std::byte g_weatherConditions[0xf60];
#else
extern std::array<std::byte, 0xf60> g_weatherConditions;
#endif
extern std::byte *g_flagDescriptorData;
extern std::uint64_t g_flagDescriptorDataSize;
extern std::uint16_t *g_flagWork;
extern std::byte *g_memoData;
extern std::byte *g_memoMasterTable;
extern std::byte *g_memoRubyTable;
extern std::uint32_t g_memoRecordCount;
extern std::int32_t g_sceneTextLanguage;
extern bool g_sceneTextAvailable;

} // namespace s2
} // namespace shenmue

#undef SHENMUE_SCENE_NOEXCEPT
