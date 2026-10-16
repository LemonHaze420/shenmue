#pragma once

#include "s1/d3t/namespace.hpp"
#include "d3t/dx11_renderer.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Rect {
    std::int32_t left;
    std::int32_t top;
    std::int32_t right;
    std::int32_t bottom;
};

using MouseDelta = Vector2;

using DisplayModeInfo = d3t::DisplayModeInfo;

struct MonitorInfo {
    char adapterName[32];
    char displayName[32];
    Rect monitorCoordinates;
    std::uint8_t active;
    std::byte reserved_51[7];
    DisplayModeInfo *displayModes;
    std::uint32_t displayModeCount;
    std::uint32_t displayModeCapacity;
};

struct SavedInputSettings {
    std::uint8_t swapTriggers;
    std::uint8_t invertY;
    std::uint16_t areaJump;
    std::int32_t weatherMode;
    std::array<std::int32_t, 4> mappings;
    std::array<std::int32_t, 4> functionKeys;
    std::array<std::int32_t, 4> modifierAndVerticalKeys;
    std::array<std::int32_t, 4> arrowAndMovementKeys;
    std::array<std::int32_t, 2> userMappedQteKeys;
    std::array<std::int32_t, 4> qteMappings;
    std::array<std::int32_t, 4> fightMappings;
    std::array<std::int32_t, 4> reservedMappings;
    std::array<std::int32_t, 2> modifierMappings;
    std::array<std::int32_t, 4> vehicleMappings;
    std::array<std::int32_t, 4> reservedVehicleMappings;
    std::array<std::int32_t, 4> vehicleMovementMappings;
    std::array<std::int32_t, 4> reservedVehicleMovementMappings;
    std::array<std::int32_t, 2> vehicleTurnMappings;
};

constexpr std::size_t kGameInputMappingCount = 18;
constexpr std::size_t kQteInputMappingCount = 4;
constexpr std::size_t kFightInputMappingCount = 10;
constexpr std::size_t kVehicleInputMappingCount = 18;
constexpr std::size_t kSavedInputMappingCount = kGameInputMappingCount + kQteInputMappingCount + kFightInputMappingCount + kVehicleInputMappingCount;
constexpr std::int32_t kUnboundInputMapping = -1;

std::int32_t GetSavedInputMapping(const SavedInputSettings &settings, std::size_t mappingIndex) noexcept;
bool SetSavedInputMapping(SavedInputSettings &settings, std::size_t mappingIndex, std::int32_t key) noexcept;
bool IsSavedInputMappingKeyAllowed(std::size_t mappingIndex, std::int32_t key) noexcept;
struct DisplaySettings {
    std::int32_t windowMode;
    std::int32_t displayOutput;
    std::int32_t antiAliasingMode;
    float resolutionScale;
    float superSampling;
    std::uint8_t fxaa;
    std::uint8_t secondaryFlag;
    std::uint8_t tertiaryFlag;
    std::uint8_t reserved_17;
    std::int32_t bloomMode;
    std::int32_t aspectRatio;
    float gamma;
    float contrast;
    float uiDisplayArea;
};

struct D3TApplicationSettings {
    std::array<MouseDelta, 10> mouseDeltaHistory;
    Rect windowRect;
    std::int32_t keyMapping;
    SavedInputSettings savedInput;
    std::int32_t savedAudioDevice;
    std::int32_t savedAudioLanguage;
    std::int32_t savedSubtitleLanguage;
    std::int32_t savedDialogueMode;
    float savedDialogueVolume;
    float savedSfxVolume;
    float savedMusicVolume;
    DisplaySettings display;
    std::int32_t reserved_17c;
};

static_assert(sizeof(Rect) == 0x10);
static_assert(sizeof(MouseDelta) == 8);
static_assert(sizeof(DisplayModeInfo) == 0x20);
static_assert(sizeof(MonitorInfo) == 0x68);
static_assert(sizeof(SavedInputSettings) == 0xd0);
static_assert(sizeof(DisplaySettings) == 0x2c);
static_assert(sizeof(D3TApplicationSettings) == 0x180);




struct D3TSettings {
    std::int32_t version;
    SavedInputSettings input;
    std::int32_t audioDevice;
    std::int32_t audioLanguage;
    std::int32_t subtitleLanguage;
    std::int32_t dialogueMode;
    float dialogueVolume;
    float sfxVolume;
    float musicVolume;
    DisplaySettings display;
};
static_assert(sizeof(D3TSettings) == 0x11c);
static_assert(offsetof(D3TApplicationSettings, reserved_17c) - offsetof(D3TApplicationSettings, keyMapping) == sizeof(D3TSettings));
static_assert(offsetof(D3TApplicationSettings, display) - offsetof(D3TApplicationSettings, keyMapping) == offsetof(D3TSettings, display));

extern D3TApplicationSettings g_applicationSettings;
extern D3TSettings g_currentSettings;
extern MonitorInfo *g_monitors;
extern std::uint32_t g_monitorCount;

extern void (*g_audioInitCallback)();

void SetDefaultSettings(SavedInputSettings *settings);
void SelectMonitorForDisplay(DisplaySettings *settings);
void InitializeD3TSettings();
void ReadConfigFromSave();
void ReadSettingsFile();
bool IsValidDisplaySettings();
void SettingsFileParser();
bool D3T_SettingsKeyMatches(const char *key, const char *name);
namespace d3t {
bool WriteSettingsFile();
} // namespace d3t
std::uint32_t GetDisplayInfoByIdx(std::uint32_t displayOutput);
DisplayModeInfo *GetScreenResolution();

#if defined(_WIN32)
std::int32_t EnumerateDisplays();
#endif

} // namespace shenmue::s1
