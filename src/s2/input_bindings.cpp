#include "s2/input_bindings.hpp"
#include "s2/display_configuration.hpp"
#include "s2/save.hpp"
#include "d3t/threads.hpp"

#include <stddef.h>
#include <cJSON.h>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace shenmue::s2 {
namespace {
S2UserConfiguration280 runtimeUserConfiguration{};
S2UserConfiguration280 draftUserConfiguration{};
S2UserConfiguration280 savedUserConfiguration{};

struct UserSettingsWriteRequest {
    std::uint32_t archiveToken;
    std::uint32_t size;
    char path[260];
    std::uint32_t reserved10c;
    char *text;
};
static_assert(sizeof(UserSettingsWriteRequest) == 0x118);

std::int32_t WriteUserSettingsTask(void *data) {
    const UserSettingsWriteRequest &request = *static_cast<UserSettingsWriteRequest *>(data);
    std::ofstream output(request.path, std::ios::binary | std::ios::trunc);
    if (!output)
        return -4;
    output.write(request.text, request.size);
    return output ? 0 : -4;
}

void CompleteUserSettingsWrite(void *data) {
    std::free(static_cast<UserSettingsWriteRequest*>(data)->text);
}
}
std::array<std::int32_t, 50> g_gameKeyMappings{};
bool FormatUIInputKeyName(std::int32_t key, std::array<char, 64> &name) {
    if (key >= 256) {
        if (key > 260)
            return false;
        std::snprintf(name.data(), name.size(), "M %d", key - 256);
        return true;
    }
#if defined(_WIN32)
    UINT scan = MapVirtualKeyA(static_cast<UINT>(key), MAPVK_VK_TO_VSC);
    if (scan >= 2 && scan <= 11) {
        name[0] = scan == 11 ? '0' : static_cast<char>('0' + scan - 1);
        name[1] = '\0';
        return true;
    }
    switch (key) {
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 45:
    case 46:
    case 111:
    case 144:
        scan |= 0x100U;
        break;
    default:
        break;
    }
    return GetKeyNameTextA(static_cast<LONG>(scan << 16U), name.data(), static_cast<int>(name.size())) > 0;
#else
    return false;
#endif
}

S2UserConfiguration280 &GetDraftUserConfiguration() {
    return draftUserConfiguration;
}
const S2UserConfiguration280 &GetRuntimeUserConfiguration() {
    return runtimeUserConfiguration;
}

void AcknowledgeS2AudioOutput(std::int32_t device) {
    runtimeUserConfiguration.audio.device = device;
    draftUserConfiguration.audio.device = device;
}

// @verified - p
void SnapshotUserConfiguration() {
    std::memcpy(&savedUserConfiguration, &runtimeUserConfiguration, sizeof(savedUserConfiguration));
}

// @verified - p
bool HasUserConfigurationChanges() {
    return std::memcmp(&savedUserConfiguration, &draftUserConfiguration, sizeof(savedUserConfiguration)) != 0;
}

// @verified - p
bool HasScalingSettingsModified() {
    return !(runtimeUserConfiguration.windowMode == draftUserConfiguration.windowMode &&
        runtimeUserConfiguration.displayOutput == draftUserConfiguration.displayOutput &&
        runtimeUserConfiguration.resolutionIndex == draftUserConfiguration.resolutionIndex &&
        std::fabs(runtimeUserConfiguration.resolutionScale - draftUserConfiguration.resolutionScale) < 0.0099999998f &&
        std::fabs(runtimeUserConfiguration.superSampling - draftUserConfiguration.superSampling) < 0.0099999998f &&
        runtimeUserConfiguration.enableSuperSampling == draftUserConfiguration.enableSuperSampling);
}

void ApplyDraftUserConfiguration() {
    const S2UserConfiguration280 previous = runtimeUserConfiguration;
    std::memcpy(&runtimeUserConfiguration, &draftUserConfiguration, sizeof(runtimeUserConfiguration));
    ApplyS2UserConfigurationChanges(previous);
}

void PreviewDraftUserConfiguration() {
    const S2UserConfiguration280 previous = runtimeUserConfiguration;
    runtimeUserConfiguration.swapTriggers = draftUserConfiguration.swapTriggers;
    runtimeUserConfiguration.invertLookY = draftUserConfiguration.invertLookY;
    runtimeUserConfiguration.keyMappings = draftUserConfiguration.keyMappings;
    runtimeUserConfiguration.fxaa = draftUserConfiguration.fxaa;
    runtimeUserConfiguration.audio.dialogueVolume = draftUserConfiguration.audio.dialogueVolume;
    runtimeUserConfiguration.audio.musicVolume = draftUserConfiguration.audio.musicVolume;
    runtimeUserConfiguration.audio.sfxVolume = draftUserConfiguration.audio.sfxVolume;
    runtimeUserConfiguration.contrast = draftUserConfiguration.contrast;
    runtimeUserConfiguration.gamma = draftUserConfiguration.gamma;
    ApplyS2UserConfigurationChanges(previous);
}

void RollbackUserConfiguration() {
    std::memcpy(&draftUserConfiguration, &savedUserConfiguration, sizeof(draftUserConfiguration));
    ApplyDraftUserConfiguration();
}

// @verified - p
bool HasUnboundUserMappings() {
    for (std::int32_t key : draftUserConfiguration.keyMappings)
        if (key == -1)
            return true;
    for (std::int32_t key : runtimeUserConfiguration.keyMappings)
        if (key == -1)
            return true;
    return false;
}

void ProcessUserSettingsConfig() {
    const S2UserConfiguration280 &configuration = runtimeUserConfiguration;
    cJSON *const settings = cJSON_CreateObject();
    if (!settings)
        return;
    cJSON_AddNumberToObject(settings, "Version", 1);
    cJSON *const controls = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "Controls", controls);
    cJSON_AddBoolToObject(controls, "SwapTriggers", configuration.swapTriggers);
    cJSON_AddBoolToObject(controls, "InvertLookY", configuration.invertLookY);
    cJSON_AddItemToObject(controls, "GameMappings", cJSON_CreateIntArray(configuration.keyMappings.data(), 18));
    cJSON_AddItemToObject(controls, "QTEMappings", cJSON_CreateIntArray(configuration.keyMappings.data() + 18, 4));
    cJSON_AddItemToObject(controls, "FightMappings", cJSON_CreateIntArray(configuration.keyMappings.data() + 22, 10));
    cJSON_AddItemToObject(controls, "VehicleMappings", cJSON_CreateIntArray(configuration.keyMappings.data() + 32, 18));
    cJSON *const audio = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "Audio", audio);
    cJSON_AddNumberToObject(audio, "AudioDevice", configuration.audio.device);
    cJSON_AddNumberToObject(audio, "AudioLanguage", configuration.audio.audioLanguage);
    cJSON_AddNumberToObject(audio, "SubtitleLanguage", configuration.audio.subtitleLanguage);
    cJSON_AddNumberToObject(audio, "DialogueMode", configuration.audio.dialogueMode);
    cJSON_AddNumberToObject(audio, "DialogueVolume", configuration.audio.dialogueVolume);
    cJSON_AddNumberToObject(audio, "SFXVolume", configuration.audio.sfxVolume);
    cJSON_AddNumberToObject(audio, "MusicVolume", configuration.audio.musicVolume);
    cJSON *const graphics = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "Graphics", graphics);
    cJSON_AddNumberToObject(graphics, "WindowMode", configuration.windowMode);
    cJSON_AddNumberToObject(graphics, "DisplayOutput", configuration.displayOutput);
    const d3t::DisplayModeInfo *resolution = GetS2DisplayResolution(configuration.displayOutput, configuration.resolutionIndex);
    if (!resolution)
        resolution = GetS2DisplayResolution(configuration.displayOutput, GetS2DesktopResolutionIndex(configuration.displayOutput));
    if (resolution) {
        cJSON_AddNumberToObject(graphics, "ResolutionWidth", resolution->width);
        cJSON_AddNumberToObject(graphics, "ResolutionHeight", resolution->height);
    }
    cJSON_AddNumberToObject(graphics, "ResolutionScale", configuration.resolutionScale);
    cJSON_AddNumberToObject(graphics, "SuperSample", configuration.superSampling);
    cJSON_AddBoolToObject(graphics, "FXAA", configuration.fxaa);
    cJSON_AddNumberToObject(graphics, "BloomMode", configuration.bloomMode);
    cJSON_AddNumberToObject(graphics, "AspectRatioMode", configuration.aspectRatio);
    cJSON_AddNumberToObject(graphics, "Gamma", configuration.gamma);
    cJSON_AddNumberToObject(graphics, "Contrast", configuration.contrast);
    cJSON_AddNumberToObject(graphics, "UISafeArea", configuration.uiSafeArea);
    char *const text = cJSON_Print(settings);
    cJSON_Delete(settings);
    if (!text)
        return;
#if defined(SHENMUE_STEAM)
    QueueSaveFileWrite("Settings", text, static_cast<std::uint32_t>(std::strlen(text)),
        [](std::int32_t, const void *buffer) { std::free(const_cast<void *>(buffer)); });
#else
    const std::filesystem::path path = S2Save_GetDirectory() / "Settings";
    std::error_code error;
    std::filesystem::create_directories(path.parent_path(), error);
    const std::string pathText = path.string();
    if (error || pathText.size() >= 260) {
        std::free(text);
        return;
    }
    UserSettingsWriteRequest request{.size = static_cast<std::uint32_t>(std::strlen(text)), .text = text};
    std::memcpy(request.path, pathText.c_str(), pathText.size() + 1);
    const d3t::D3TTaskDescriptor descriptor{.data = &request, .callback = WriteUserSettingsTask, .completion = CompleteUserSettingsWrite, .priority = 0};
    if (!d3t::d3t_QueueTask(&descriptor, sizeof(request)))
        std::free(text);
#endif
}

// @verified - p
void ResetDraftUserControls() {
    draftUserConfiguration.swapTriggers = 0;
    draftUserConfiguration.invertLookY = 0;
    draftUserConfiguration.keyMappings = defaultGameKeyMappings;
}

// @verified - p
void ResetDraftUserAudio() {
    draftUserConfiguration.audio = {};
}

void ResetDraftUserGraphics() {
    draftUserConfiguration.windowMode = 1;
    draftUserConfiguration.displayOutput = GetS2PrimaryDisplayIndex();
    draftUserConfiguration.resolutionIndex = GetS2DesktopResolutionIndex(draftUserConfiguration.displayOutput);
    draftUserConfiguration.resolutionScale = 1.0f;
    draftUserConfiguration.superSampling = 0.0f;
    draftUserConfiguration.fxaa = 1;
    draftUserConfiguration.enableSuperSampling = 1;
    draftUserConfiguration.bloomMode = 2;
    draftUserConfiguration.aspectRatio = 1;
    draftUserConfiguration.gamma = 0.5f;
    draftUserConfiguration.contrast = 0.5f;
    draftUserConfiguration.uiSafeArea = 1.0f;
}

void InitializeInputBindings() {
    draftUserConfiguration = {};
    draftUserConfiguration.version = 1;
    ResetDraftUserControls();
    ResetDraftUserAudio();
    ResetDraftUserGraphics();
    ApplyDraftUserConfiguration();
}

void ReadInputBindingSettings(cJSON *controls) {
    cJSON *values = cJSON_GetObjectItem(controls, "GameMappings");
    if (cJSON_GetArraySize(values) == 18)
        for (std::int32_t i = 0; i < 18; ++i)
            draftUserConfiguration.keyMappings[static_cast<std::size_t>(i)] = cJSON_GetArrayItem(values, i)->valueint;
    values = cJSON_GetObjectItem(controls, "QTEMappings");
    if (cJSON_GetArraySize(values) == 4)
        for (std::int32_t i = 0; i < 4; ++i)
            draftUserConfiguration.keyMappings[static_cast<std::size_t>(i + 18)] = cJSON_GetArrayItem(values, i)->valueint;
    values = cJSON_GetObjectItem(controls, "FightMappings");
    if (cJSON_GetArraySize(values) == 10)
        for (std::int32_t i = 0; i < 10; ++i)
            draftUserConfiguration.keyMappings[static_cast<std::size_t>(i + 22)] = cJSON_GetArrayItem(values, i)->valueint;
    values = cJSON_GetObjectItem(controls, "VehicleMappings");
    if (cJSON_GetArraySize(values) == 18)
        for (std::int32_t i = 0; i < 18; ++i)
            draftUserConfiguration.keyMappings[static_cast<std::size_t>(i + 32)] = cJSON_GetArrayItem(values, i)->valueint;
}

void ApplyInputBindings() {
    g_gameKeyMappings = runtimeUserConfiguration.keyMappings;
}

// @verified - p
std::uint32_t GetGameKeyMapping(std::int32_t action) {
    return action < 50 ? static_cast<std::uint32_t>(runtimeUserConfiguration.keyMappings[static_cast<std::size_t>(action)]) : 0xffffffffU;
}

// @verified - p
std::uint32_t GetMenuKeyMapping(std::int32_t action) {
    if (action >= 50)
        return 0xffffffffU;
    return static_cast<std::uint32_t>(action < 18 ? menuKeyMappings[static_cast<std::size_t>(action)] : g_gameKeyMappings[static_cast<std::size_t>(action)]);
}

// @verified - p
std::uint32_t GetQteKeyMapping(std::int32_t action) {
    return static_cast<std::uint32_t>(g_gameKeyMappings[static_cast<std::size_t>(action + 18)]);
}

// @verified - p
std::uint32_t GetFightKeyMapping(std::int32_t action) {
    return static_cast<std::uint32_t>(g_gameKeyMappings[static_cast<std::size_t>(action + 22)]);
}

// @verified - p
std::int32_t FindUIInputAction(const char *name, std::int32_t fallback) {
    if (!name)
        return fallback;
    for (std::size_t i = 0; i < uiInputActionNames.size(); ++i)
        if (std::strncmp(name, uiInputActionNames[i], 64) == 0)
            return static_cast<std::int32_t>(i);
    return fallback;
}

} // namespace shenmue::s2
