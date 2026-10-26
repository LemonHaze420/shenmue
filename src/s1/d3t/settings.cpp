#if defined(SHENMUE_STEAM)
#include "d3t/steam.hpp"
#endif

#include "s1/d3t/settings.hpp"
#include "s1/d3t/namespace.hpp"

#include "s1/audio_stream.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/window.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/render_passes.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <memory>

#include <json/json.h>

#if defined(_WIN32)
#include <dxgi.h>
#include <windows.h>
#endif

namespace shenmue::s1 {
namespace {

constexpr std::array<std::int32_t, 4> kDefaultMappings{0x52, 0x45, 0x51, 0x09};
constexpr std::array<std::int32_t, 4> kDefaultFunctionKeys{0x70, 0x71, 0x72, 0x20};
constexpr std::array<std::int32_t, 4> kDefaultModifierAndVerticalKeys{0xa2, 0xa0, 0x26, 0x28};
constexpr std::array<std::int32_t, 4> kDefaultArrowAndMovementKeys{0x25, 0x27, 0x57, 0x53};
constexpr std::array<std::int32_t, 4> kDefaultQteMappings{0x32, 0x33, 0x31, 0x34};
constexpr std::array<std::int32_t, 4> kDefaultFightMappings{0x65, 0x66, 0x64, 0x68};
constexpr std::array<std::int32_t, 4> kDefaultVehicleMappings{0x45, 0x43, 0x4d, 0x09};
constexpr std::array<std::int32_t, 4> kDefaultVehicleMovementMappings{0x53, 0x57, 0, 0};

} // namespace

D3TApplicationSettings g_applicationSettings = [] {
    D3TApplicationSettings settings{};
    settings.windowRect = Rect{1, 58, 1025, 826};
    return settings;
}();
D3TSettings g_currentSettings{};
MonitorInfo *g_monitors = nullptr;
std::uint32_t g_monitorCount = 0;

std::int32_t GetSavedInputMapping(const SavedInputSettings &settings, std::size_t mappingIndex) noexcept {
    if (mappingIndex < 4)
        return settings.mappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.functionKeys[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.modifierAndVerticalKeys[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.arrowAndMovementKeys[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 2)
        return settings.userMappedQteKeys[mappingIndex];
    mappingIndex -= 2;
    if (mappingIndex < 4)
        return settings.qteMappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.fightMappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.reservedMappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 2)
        return settings.modifierMappings[mappingIndex];
    mappingIndex -= 2;
    if (mappingIndex < 4)
        return settings.vehicleMappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.reservedVehicleMappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.vehicleMovementMappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 4)
        return settings.reservedVehicleMovementMappings[mappingIndex];
    mappingIndex -= 4;
    if (mappingIndex < 2)
        return settings.vehicleTurnMappings[mappingIndex];
    return kUnboundInputMapping;
}

bool SetSavedInputMapping(SavedInputSettings &settings, std::size_t mappingIndex, std::int32_t key) noexcept {
    std::int32_t *mapping = nullptr;
    if (mappingIndex < 4) {
        mapping = &settings.mappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.functionKeys[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.modifierAndVerticalKeys[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.arrowAndMovementKeys[mappingIndex];
    } else if ((mappingIndex -= 4) < 2) {
        mapping = &settings.userMappedQteKeys[mappingIndex];
    } else if ((mappingIndex -= 2) < 4) {
        mapping = &settings.qteMappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.fightMappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.reservedMappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 2) {
        mapping = &settings.modifierMappings[mappingIndex];
    } else if ((mappingIndex -= 2) < 4) {
        mapping = &settings.vehicleMappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.reservedVehicleMappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.vehicleMovementMappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 4) {
        mapping = &settings.reservedVehicleMovementMappings[mappingIndex];
    } else if ((mappingIndex -= 4) < 2) {
        mapping = &settings.vehicleTurnMappings[mappingIndex];
    }
    if (!mapping)
        return false;
    *mapping = key;
    return true;
}

bool IsSavedInputMappingKeyAllowed(std::size_t mappingIndex, std::int32_t key) noexcept {
    if (mappingIndex >= kSavedInputMappingCount || key < 0 || key > 260)
        return false;
    if (mappingIndex >= kGameInputMappingCount && mappingIndex < kGameInputMappingCount + kQteInputMappingCount && key >= 0x25 && key <= 0x28) {
        return false;
    }
    if ((key >= 0x41 && key <= 0x5a) || (key >= 0x60 && key <= 0x97) || (key >= 0x30 && key <= 0x39) || (key >= 0xa0 && key <= 0xa5) ||
        (key >= 0x101 && key <= 0x104)) {
        return true;
    }
    switch (key) {
    case 0x08:
    case 0x09:
    case 0x0d:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x14:
    case 0x20:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0xbb:
    case 0xbc:
    case 0xbd:
    case 0xbe:
    case 0xbf:
    case 0xdc:
    case 0xdd:
    case 0xdf:
        return true;
    default:
        return false;
    }
}

void SetDefaultSettings(SavedInputSettings *settings) {
    settings->swapTriggers = 0;
    settings->invertY = 0;
    settings->areaJump = 1;
    settings->weatherMode = 0;
    settings->mappings = kDefaultMappings;
    settings->functionKeys = kDefaultFunctionKeys;
    settings->modifierAndVerticalKeys = kDefaultModifierAndVerticalKeys;
    settings->arrowAndMovementKeys = kDefaultArrowAndMovementKeys;
    settings->userMappedQteKeys = {0x41, 0x44};
    settings->qteMappings = kDefaultQteMappings;
    settings->fightMappings = kDefaultFightMappings;
    settings->reservedMappings = {};
    settings->modifierMappings = {0xa2, 0xa0};
    settings->vehicleMappings = kDefaultVehicleMappings;
    settings->reservedVehicleMappings = {};
    settings->vehicleMovementMappings = kDefaultVehicleMovementMappings;
    settings->reservedVehicleMovementMappings = {};
    settings->vehicleTurnMappings = {0x41, 0x44};
}

void SelectMonitorForDisplay(DisplaySettings *settings) {
    settings->windowMode = 1;
    settings->antiAliasingMode = 0;

    std::uint32_t monitorIndex = 0;
    while (monitorIndex < g_monitorCount && !g_monitors[monitorIndex].active) {
        ++monitorIndex;
    }
    if (monitorIndex >= g_monitorCount) {
        monitorIndex = 0;
    }
    settings->displayOutput = static_cast<std::int32_t>(monitorIndex);

    std::uint32_t displayModeIndex = 0;
    if (monitorIndex < g_monitorCount) {
        const MonitorInfo &monitor = g_monitors[monitorIndex];
        const std::uint32_t width = static_cast<std::uint32_t>(monitor.monitorCoordinates.right - monitor.monitorCoordinates.left);
        const std::uint32_t height = static_cast<std::uint32_t>(monitor.monitorCoordinates.bottom - monitor.monitorCoordinates.top);
        while (displayModeIndex < monitor.displayModeCount &&
            (monitor.displayModes[displayModeIndex].width != width || monitor.displayModes[displayModeIndex].height != height)) {
            ++displayModeIndex;
        }
        if (displayModeIndex >= monitor.displayModeCount) {
            displayModeIndex = 0;
        }
    }
    settings->antiAliasingMode = static_cast<std::int32_t>(displayModeIndex);
    settings->resolutionScale = 1.0f;
    settings->superSampling = 0.0f;
    settings->fxaa = 1;
    settings->secondaryFlag = 1;
    settings->tertiaryFlag = 0;
    settings->reserved_17 = 0;
    settings->bloomMode = 2;
    settings->aspectRatio = 1;
    settings->gamma = 0.5f;
    settings->contrast = 0.5f;
    settings->uiDisplayArea = 1.0f;
}

void InitializeD3TSettings() {
    g_applicationSettings.keyMapping = 1;
    SetDefaultSettings(&g_applicationSettings.savedInput);
    g_applicationSettings.savedAudioDevice = 1;
    g_applicationSettings.savedAudioLanguage = 0;
    g_applicationSettings.savedSubtitleLanguage = -1;
    g_applicationSettings.savedDialogueMode = 2;
    g_applicationSettings.savedDialogueVolume = 1.0f;
    g_applicationSettings.savedSfxVolume = 1.0f;
    g_applicationSettings.savedMusicVolume = 1.0f;
    SelectMonitorForDisplay(&g_applicationSettings.display);
}

namespace {

void ReadMappingArray(const Json::Value &parent, const char *name, SavedInputSettings &settings, std::size_t firstMapping, std::size_t mappingCount) {
    const Json::Value &values = parent[name];
    if (!values.isArray() || values.size() != static_cast<Json::ArrayIndex>(mappingCount)) {
        return;
    }
    for (Json::ArrayIndex index = 0; index < values.size(); ++index) {
        if (!values[index].isIntegral())
            return;
    }
    for (Json::ArrayIndex index = 0; index < values.size(); ++index) {
        SetSavedInputMapping(settings, firstMapping + static_cast<std::size_t>(index), values[index].asInt());
    }
}

Json::Value WriteMappingArray(const SavedInputSettings &settings, std::size_t firstMapping, std::size_t mappingCount) {
    Json::Value values(Json::arrayValue);
    for (std::size_t index = 0; index < mappingCount; ++index) {
        values.append(GetSavedInputMapping(settings, firstMapping + index));
    }
    return values;
}

template <typename T> T IntegerOr(const Json::Value &parent, const char *name, T fallback) {
    const Json::Value &value = parent[name];
    return value.isIntegral() ? static_cast<T>(value.asInt64()) : fallback;
}

float FloatOr(const Json::Value &parent, const char *name, float fallback) {
    const Json::Value &value = parent[name];
    return value.isNumeric() ? value.asFloat() : fallback;
}

float NormalizedFloatOr(const Json::Value &parent, const char *name, float fallback) {
    return std::clamp(FloatOr(parent, name, fallback), 0.0f, 1.0f);
}

std::uint32_t ValidDisplayModeOr(std::uint32_t output, std::uint32_t mode, std::uint32_t fallback) {
    if (output >= g_monitorCount || !g_monitors || g_monitors[output].displayModeCount == 0) {
        return 0;
    }
    if (mode < g_monitors[output].displayModeCount) {
        return mode;
    }
    return fallback < g_monitors[output].displayModeCount ? fallback : 0;
}

std::uint32_t FindDisplayMode(std::uint32_t output, std::uint32_t width, std::uint32_t height, std::uint32_t fallback) {
    if (!g_monitors || output >= g_monitorCount) {
        return fallback;
    }
    const MonitorInfo &monitor = g_monitors[output];
    for (std::uint32_t index = 0; index < monitor.displayModeCount; ++index) {
        if (monitor.displayModes[index].width == width && monitor.displayModes[index].height == height) {
            return index;
        }
    }
    return fallback;
}

} // namespace

namespace {






void InitialiseGraphicsConfig(const D3TSettings &previous) {
    const DisplaySettings &display = g_currentSettings.display;
    if (display.windowMode != previous.display.windowMode || display.displayOutput != previous.display.displayOutput ||
        display.antiAliasingMode != previous.display.antiAliasingMode)
        g_shouldResetWindow = true;
    if (std::fabs(display.resolutionScale - previous.display.resolutionScale) >= 0.01f ||
        std::fabs(display.superSampling - previous.display.superSampling) >= 0.01f) {
        if (g_hasSetupBuffersForTextures)
            g_bNeedsLoadTextureResources = 1;
    }
    g_fContrastValue = display.contrast;
    g_fGammaValue = (display.gamma - 0.5f) + 1.0f;
    if (display.contrast < 0.5f)
        g_fContrastValue = 0.5f - ((0.5f - display.contrast) / 1.5f);
    g_fFxaaEnabled = display.fxaa != 0;
    g_fBloomEnabled = display.bloomMode != 0;
    if (g_currentSettings.audioDevice != previous.audioDevice)
        QueueAudioDeviceChange(g_currentSettings.audioDevice);
    if (g_audioInitCallback)
        g_audioInitCallback();
}

} // namespace


void ReadConfigFromSave() {
    const D3TSettings previous = g_currentSettings;
    std::memcpy(&g_currentSettings, &g_applicationSettings.keyMapping, sizeof(g_currentSettings));
    InitialiseGraphicsConfig(previous);
}




void ReadSettingsFile() {
    const D3TSettings previous = g_currentSettings;
    const std::uint16_t areaJump = g_currentSettings.input.areaJump;
    g_currentSettings.input = g_applicationSettings.savedInput;
    g_currentSettings.input.areaJump = static_cast<std::uint16_t>((areaJump & 0xff00U) | (g_applicationSettings.savedInput.areaJump & 0xffU));
    g_currentSettings.display.fxaa = g_applicationSettings.display.fxaa;
    g_currentSettings.dialogueVolume = g_applicationSettings.savedDialogueVolume;
    g_currentSettings.musicVolume = g_applicationSettings.savedMusicVolume;
    g_currentSettings.sfxVolume = g_applicationSettings.savedSfxVolume;
    g_currentSettings.display.contrast = g_applicationSettings.display.contrast;
    g_currentSettings.display.gamma = g_applicationSettings.display.gamma;
    InitialiseGraphicsConfig(previous);
}



bool IsValidDisplaySettings() {
    const DisplaySettings &current = g_currentSettings.display;
    const DisplaySettings &display = g_applicationSettings.display;
    return current.windowMode != display.windowMode || current.displayOutput != display.displayOutput || current.antiAliasingMode != display.antiAliasingMode ||
        std::fabs(current.resolutionScale - display.resolutionScale) >= 0.01f || std::fabs(current.superSampling - display.superSampling) >= 0.01f ||
        g_superSamplingTextureActive != display.secondaryFlag;
}

void SettingsFileParser() {
#if defined(SHENMUE_STEAM)
    const std::vector<std::byte> bytes = ::shenmue::d3t::ReadSteamFileBytes("Settings");
    if (bytes.empty()) return;
    const std::string contents(reinterpret_cast<const char *>(bytes.data()), bytes.size());
#else
    std::ifstream input("Settings", std::ios::binary);
    if (!input) {
        return;
    }
    const std::string contents((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
#endif
    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errors;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(contents.data(), contents.data() + contents.size(), &root, &errors)) {
        return;
    }

    g_applicationSettings.keyMapping = IntegerOr(root, "Version", g_applicationSettings.keyMapping);

    const Json::Value &controls = root["Controls"];
    if (controls.isObject()) {
        SavedInputSettings &saved = g_applicationSettings.savedInput;
        saved.swapTriggers = IntegerOr(controls, "SwapTriggers", saved.swapTriggers) == 1;
        saved.invertY = IntegerOr(controls, "InvertLookY", saved.invertY) == 1;
        saved.areaJump = IntegerOr(controls, "AreaJump", saved.areaJump) == 1;
        saved.weatherMode = IntegerOr(controls, "WeatherMode", saved.weatherMode);

        ReadMappingArray(controls, "GameMappings", saved, 0, kGameInputMappingCount);
        ReadMappingArray(controls, "QTEMappings", saved, kGameInputMappingCount, kQteInputMappingCount);
        ReadMappingArray(controls, "FightMappings", saved, kGameInputMappingCount + kQteInputMappingCount, kFightInputMappingCount);
        ReadMappingArray(
            controls, "VehicleMappings", saved, kGameInputMappingCount + kQteInputMappingCount + kFightInputMappingCount, kVehicleInputMappingCount);
    }

    const Json::Value &audio = root["Audio"];
    if (audio.isObject()) {
        g_applicationSettings.savedAudioDevice = IntegerOr(audio, "AudioDevice", g_applicationSettings.savedAudioDevice);
        if (!g_applicationSettings.keyMapping) {
            ++g_applicationSettings.savedAudioDevice;
        }
        g_applicationSettings.savedAudioLanguage = IntegerOr(audio, "AudioLanguage", g_applicationSettings.savedAudioLanguage);
        g_applicationSettings.savedSubtitleLanguage = IntegerOr(audio, "SubtitleLanguage", g_applicationSettings.savedSubtitleLanguage);
        g_applicationSettings.savedDialogueMode = IntegerOr(audio, "DialogueMode", g_applicationSettings.savedDialogueMode);
        g_applicationSettings.savedDialogueVolume = FloatOr(audio, "DialogueVolume", g_applicationSettings.savedDialogueVolume);
        g_applicationSettings.savedSfxVolume = FloatOr(audio, "SFXVolume", g_applicationSettings.savedSfxVolume);
        g_applicationSettings.savedMusicVolume = FloatOr(audio, "MusicVolume", g_applicationSettings.savedMusicVolume);
    }

    const Json::Value &graphics = root["Graphics"];
    if (graphics.isObject()) {
        DisplaySettings &display = g_applicationSettings.display;
        display.windowMode = std::clamp(IntegerOr(graphics, "WindowMode", display.windowMode), 0, 2);
        std::uint32_t output = static_cast<std::uint32_t>(IntegerOr(graphics, "DisplayOutput", display.displayOutput));
        if (!g_monitors || output >= g_monitorCount) {
            output = 0;
            while (g_monitors && output < g_monitorCount && !g_monitors[output].active) {
                ++output;
            }
            if (output >= g_monitorCount) {
                output = 0;
            }
        }
        display.displayOutput = static_cast<std::int32_t>(output);
        const std::uint32_t fallbackMode = ValidDisplayModeOr(output, static_cast<std::uint32_t>(display.antiAliasingMode), 0);
        const Json::Value &width = graphics["ResolutionWidth"];
        const Json::Value &height = graphics["ResolutionHeight"];
        if (width.isIntegral() && height.isIntegral()) {
            display.antiAliasingMode =
                static_cast<std::int32_t>(ValidDisplayModeOr(output, FindDisplayMode(output, width.asUInt(), height.asUInt(), fallbackMode), fallbackMode));
        } else {
            display.antiAliasingMode = static_cast<std::int32_t>(fallbackMode);
        }
        display.resolutionScale = NormalizedFloatOr(graphics, "ResolutionScale", display.resolutionScale);
        display.superSampling = NormalizedFloatOr(graphics, "SuperSample", display.superSampling);
        display.fxaa = IntegerOr(graphics, "FXAA", display.fxaa) == 1;
        display.bloomMode = std::clamp(IntegerOr(graphics, "BloomMode", display.bloomMode), 0, 2);
        display.aspectRatio = std::clamp(IntegerOr(graphics, "AspectRatioMode", display.aspectRatio), 0, 1);
        display.gamma = NormalizedFloatOr(graphics, "Gamma", display.gamma);
        display.contrast = NormalizedFloatOr(graphics, "Contrast", display.contrast);
        display.uiDisplayArea = NormalizedFloatOr(graphics, "UISafeArea", display.uiDisplayArea);
    }
    ReadConfigFromSave();
}

namespace d3t {
bool WriteSettingsFile() {
    Json::Value root(Json::objectValue);
    root["Version"] = g_currentSettings.version;

    const SavedInputSettings &saved = g_currentSettings.input;
    Json::Value &controls = root["Controls"];
    controls["SwapTriggers"] = saved.swapTriggers != 0 ? 1 : 0;
    controls["InvertLookY"] = saved.invertY != 0 ? 1 : 0;
    controls["AreaJump"] = saved.areaJump != 0 ? 1 : 0;
    controls["WeatherMode"] = saved.weatherMode;

    controls["GameMappings"] = WriteMappingArray(saved, 0, kGameInputMappingCount);
    controls["QTEMappings"] = WriteMappingArray(saved, kGameInputMappingCount, kQteInputMappingCount);
    controls["FightMappings"] = WriteMappingArray(saved, kGameInputMappingCount + kQteInputMappingCount, kFightInputMappingCount);
    controls["VehicleMappings"] = WriteMappingArray(saved, kGameInputMappingCount + kQteInputMappingCount + kFightInputMappingCount, kVehicleInputMappingCount);

    Json::Value &audio = root["Audio"];
    audio["AudioDevice"] = g_currentSettings.audioDevice;
    audio["AudioLanguage"] = g_currentSettings.audioLanguage;
    audio["SubtitleLanguage"] = g_currentSettings.subtitleLanguage;
    audio["DialogueMode"] = g_currentSettings.dialogueMode;
    audio["DialogueVolume"] = g_currentSettings.dialogueVolume;
    audio["SFXVolume"] = g_currentSettings.sfxVolume;
    audio["MusicVolume"] = g_currentSettings.musicVolume;

    const DisplaySettings &display = g_currentSettings.display;
    Json::Value &graphics = root["Graphics"];
    graphics["WindowMode"] = display.windowMode;
    graphics["DisplayOutput"] = display.displayOutput;
    const std::uint32_t output = static_cast<std::uint32_t>(display.displayOutput);
    const std::uint32_t mode = static_cast<std::uint32_t>(display.antiAliasingMode);
    if (output < g_monitorCount && mode < g_monitors[output].displayModeCount) {
        graphics["ResolutionWidth"] = g_monitors[output].displayModes[mode].width;
        graphics["ResolutionHeight"] = g_monitors[output].displayModes[mode].height;
    }
    graphics["ResolutionScale"] = display.resolutionScale;
    graphics["SuperSample"] = display.superSampling;
    graphics["FXAA"] = display.fxaa != 0 ? 1 : 0;
    graphics["BloomMode"] = display.bloomMode;
    graphics["AspectRatioMode"] = display.aspectRatio;
    graphics["Gamma"] = display.gamma;
    graphics["Contrast"] = display.contrast;
    graphics["UISafeArea"] = display.uiDisplayArea;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
#if defined(SHENMUE_STEAM)
    const std::string contents = Json::writeString(builder, root);
    return ::shenmue::d3t::WriteSteamFile("Settings", contents.data(), static_cast<std::uint32_t>(contents.size()));
#else
    std::ofstream outputFile("Settings", std::ios::binary | std::ios::trunc);
    if (!outputFile) {
        return false;
    }
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    const bool wrote = writer->write(root, &outputFile) == 0;
    outputFile.flush();
    return wrote && outputFile.good();
#endif
}
} // namespace d3t

std::uint32_t GetDisplayInfoByIdx(std::uint32_t displayOutput) {
    if (displayOutput >= g_monitorCount) {
        return 0;
    }
    const MonitorInfo &monitor = g_monitors[displayOutput];
    const auto width = static_cast<std::uint32_t>(monitor.monitorCoordinates.right - monitor.monitorCoordinates.left);
    const auto height = static_cast<std::uint32_t>(monitor.monitorCoordinates.bottom - monitor.monitorCoordinates.top);
    for (std::uint32_t i = 0; i < monitor.displayModeCount; ++i) {
        if (monitor.displayModes[i].width == width && monitor.displayModes[i].height == height) {
            return i;
        }
    }
    return 0;
}

DisplayModeInfo *GetScreenResolution() {
    const std::uint32_t displayOutput = static_cast<std::uint32_t>(g_currentSettings.display.displayOutput);
    const std::uint32_t displayMode = static_cast<std::uint32_t>(g_currentSettings.display.antiAliasingMode);

    if (displayOutput < g_monitorCount) {
        MonitorInfo &monitor = g_monitors[displayOutput];
        if (displayMode < monitor.displayModeCount) {
            return &monitor.displayModes[displayMode];
        }

        const auto width = static_cast<std::uint32_t>(monitor.monitorCoordinates.right - monitor.monitorCoordinates.left);
        const auto height = static_cast<std::uint32_t>(monitor.monitorCoordinates.bottom - monitor.monitorCoordinates.top);
        for (std::uint32_t i = 0; i < monitor.displayModeCount; ++i) {
            if (monitor.displayModes[i].width == width && monitor.displayModes[i].height == height) {
                return &monitor.displayModes[i];
            }
        }
    }

    std::uint32_t activeMonitor = 0;
    while (activeMonitor < g_monitorCount && !g_monitors[activeMonitor].active) {
        ++activeMonitor;
    }
    if (activeMonitor >= g_monitorCount) {
        activeMonitor = 0;
    }
    const auto fallbackMode = GetDisplayInfoByIdx(displayOutput);
    if (activeMonitor < g_monitorCount && fallbackMode < g_monitors[activeMonitor].displayModeCount) {
        return &g_monitors[activeMonitor].displayModes[fallbackMode];
    }
    static DisplayModeInfo desktopFallback{640, 480, 60, 1, "640 x 480"};
#if defined(_WIN32)
    const int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
    const int desktopHeight = GetSystemMetrics(SM_CYSCREEN);
    if (desktopWidth >= 640 && desktopHeight >= 480) {
        desktopFallback.width = static_cast<std::uint32_t>(desktopWidth);
        desktopFallback.height = static_cast<std::uint32_t>(desktopHeight);
        std::snprintf(desktopFallback.description, sizeof(desktopFallback.description), "%u x %u", desktopFallback.width, desktopFallback.height);
    }
#endif
    return &desktopFallback;
}

#if defined(_WIN32)
namespace {

struct GpuDeviceInfo {
    char gpuName[128];
    std::uint32_t dedicatedVideoMemory;
    std::uint32_t dedicatedSystemMemory;
    std::uint32_t sharedSystemMemory;
    std::uint32_t revision;
    MonitorInfo **outputs;
    std::uint32_t outputCount;
    std::uint32_t outputCapacity;
};

static_assert(sizeof(GpuDeviceInfo) == 0xa0);

GpuDeviceInfo *g_gpuDevices = nullptr;
std::uint32_t g_gpuDeviceCount = 0;
std::uint32_t g_gpuDeviceCapacity = 0;
std::uint32_t g_monitorCapacity = 0;

template <typename T> bool GrowArray(T *&values, std::uint32_t &capacity, std::uint32_t requiredCount) {
    if (requiredCount <= capacity) {
        return true;
    }
    const std::uint32_t next = (std::max)((std::max)(16u, 2u * capacity), requiredCount);
    void *allocation = std::realloc(values, sizeof(T) * next);
    if (allocation == nullptr) {
        return false;
    }
    values = static_cast<T *>(allocation);
    capacity = next;
    return true;
}

DisplayModeInfo *AppendDisplayMode(MonitorInfo &monitor) {
    if (!GrowArray(monitor.displayModes, monitor.displayModeCapacity, monitor.displayModeCount + 1)) {
        return nullptr;
    }
    DisplayModeInfo *result = &monitor.displayModes[monitor.displayModeCount++];
    *result = {};
    return result;
}

void SetMode(DisplayModeInfo &target, const DXGI_MODE_DESC &source) {
    target.width = source.Width;
    target.height = source.Height;
    target.refreshRateNumerator = source.RefreshRate.Numerator;
    target.refreshRateDenominator = source.RefreshRate.Denominator;
    std::snprintf(target.description, sizeof(target.description), "%u x %u", target.width, target.height);
}

void ExtractMonitorDetails(MonitorInfo &monitor, IDXGIOutput *output) {
    DXGI_OUTPUT_DESC outputDescription{};
    if (FAILED(output->GetDesc(&outputDescription))) {
        return;
    }

    char deviceName[128]{};
    std::wcstombs(deviceName, outputDescription.DeviceName, sizeof(deviceName) - 1);
    DISPLAY_DEVICEA displayDevice{};
    displayDevice.cb = sizeof(displayDevice);
    if (EnumDisplayDevicesA(deviceName, 0, &displayDevice, 0)) {
        char monitorName[64]{};
        std::snprintf(monitorName, sizeof(monitorName), "%u. %s", g_monitorCount, displayDevice.DeviceString);
        std::memcpy(monitor.adapterName, monitorName, sizeof(monitorName));
    }
    monitor.monitorCoordinates = {
        outputDescription.DesktopCoordinates.left,
        outputDescription.DesktopCoordinates.top,
        outputDescription.DesktopCoordinates.right,
        outputDescription.DesktopCoordinates.bottom,
    };
    MONITORINFO monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (GetMonitorInfoA(outputDescription.Monitor, &monitorInfo)) {
        monitor.active = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0;
    }
}

void GetDisplayInfo(MonitorInfo &monitor, IDXGIOutput *output) {
    UINT count = 0;
    HRESULT result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &count, nullptr);
    if (SUCCEEDED(result)) {
        auto *modes = new DXGI_MODE_DESC[count];
        if (SUCCEEDED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &count, modes))) {
            for (UINT i = 0; i < count; ++i) {
                const DXGI_MODE_DESC &mode = modes[i];
                if (mode.Width < 640 || mode.Height < 480) {
                    continue;
                }
                DisplayModeInfo *existing = nullptr;
                for (std::uint32_t j = 0; j < monitor.displayModeCount; ++j) {
                    if (monitor.displayModes[j].width == mode.Width && monitor.displayModes[j].height == mode.Height) {
                        existing = &monitor.displayModes[j];
                        break;
                    }
                }
                if (existing == nullptr) {
                    if (DisplayModeInfo *target = AppendDisplayMode(monitor)) {
                        SetMode(*target, mode);
                    }
                    continue;
                }
                if (mode.RefreshRate.Denominator == 0 || existing->refreshRateDenominator == 0) {
                    continue;
                }
                const float oldRate = static_cast<float>(existing->refreshRateNumerator) / static_cast<float>(existing->refreshRateDenominator);
                const float newRate = static_cast<float>(mode.RefreshRate.Numerator) / static_cast<float>(mode.RefreshRate.Denominator);
                const float remainder = std::fmod(newRate, 30.0f);
                if ((remainder < 1.0f || remainder > 29.0f) && newRate > oldRate) {
                    existing->refreshRateNumerator = mode.RefreshRate.Numerator;
                    existing->refreshRateDenominator = mode.RefreshRate.Denominator;
                }
            }
            if (monitor.displayModeCount == 0 && count != 0) {
                if (DisplayModeInfo *target = AppendDisplayMode(monitor)) {
                    SetMode(*target, modes[count - 1]);
                }
            }
        }
        delete[] modes;
        return;
    }

    if (result == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE) {
        DXGI_OUTPUT_DESC description{};
        if (SUCCEEDED(output->GetDesc(&description))) {
            if (DisplayModeInfo *target = AppendDisplayMode(monitor)) {
                target->width = description.DesktopCoordinates.right - description.DesktopCoordinates.left;
                target->height = description.DesktopCoordinates.bottom - description.DesktopCoordinates.top;
                target->refreshRateNumerator = 1;
                target->refreshRateDenominator = 60;
                std::snprintf(target->description, sizeof(target->description), "%u x %u", target->width, target->height);
            }
        }
    }
}

void ProcessGpuAdapterInfo(GpuDeviceInfo &info, IDXGIAdapter *adapter) {
    DXGI_ADAPTER_DESC description{};
    if (SUCCEEDED(adapter->GetDesc(&description))) {
        std::wcstombs(info.gpuName, description.Description, sizeof(info.gpuName));
        info.dedicatedVideoMemory = static_cast<std::uint32_t>(description.DedicatedVideoMemory >> 20);
        info.dedicatedSystemMemory = static_cast<std::uint32_t>(description.DedicatedSystemMemory >> 20);
        info.sharedSystemMemory = static_cast<std::uint32_t>(description.SharedSystemMemory >> 20);
    }
}

void ProcessDisplayOutputs(GpuDeviceInfo &info, IDXGIAdapter *adapter) {
    for (UINT index = 0;; ++index) {
        IDXGIOutput *output = nullptr;
        const HRESULT result = adapter->EnumOutputs(index, &output);
        if (result == DXGI_ERROR_NOT_FOUND) {
            return;
        }
        if (FAILED(result) || output == nullptr) {
            continue;
        }
        if (GrowArray(g_monitors, g_monitorCapacity, g_monitorCount + 1)) {
            MonitorInfo *monitor = &g_monitors[g_monitorCount++];
            *monitor = {};
            if (GrowArray(info.outputs, info.outputCapacity, info.outputCount + 1)) {
                info.outputs[info.outputCount++] = monitor;
            }
            ExtractMonitorDetails(*monitor, output);
            GetDisplayInfo(*monitor, output);
        }
        output->Release();
    }
}

} // namespace

std::int32_t EnumerateDisplays() {
    IDXGIFactory1 *factory = nullptr;
    const HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory1), reinterpret_cast<void **>(&factory));
    if (FAILED(result)) {
        return result;
    }
    for (UINT index = 0;; ++index) {
        IDXGIAdapter *adapter = nullptr;
        const HRESULT enumerateResult = factory->EnumAdapters(index, &adapter);
        if (enumerateResult == DXGI_ERROR_NOT_FOUND) {
            break;
        }
        if (SUCCEEDED(enumerateResult) && adapter != nullptr && GrowArray(g_gpuDevices, g_gpuDeviceCapacity, g_gpuDeviceCount + 1)) {
            GpuDeviceInfo &info = g_gpuDevices[g_gpuDeviceCount++];
            std::memset(&info, 0, sizeof(info));
            ProcessGpuAdapterInfo(info, adapter);
            ProcessDisplayOutputs(info, adapter);
        }
        if (adapter != nullptr) {
            adapter->Release();
        }
    }
    return static_cast<std::int32_t>(factory->Release());
}
#endif


bool D3T_SettingsKeyMatches(const char *key, const char *name) {
    for (;; ++key, ++name) {
        if (std::tolower(static_cast<unsigned char>(*key)) != std::tolower(static_cast<unsigned char>(*name)))
            return false;
        if (!*key)
            return true;
    }
}

} // namespace shenmue::s1
