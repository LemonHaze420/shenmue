#pragma once

#include "s2/d3t_renderer.hpp"
#include "common/keyboard.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

struct cJSON;

namespace shenmue::s2 {

using ::shenmue::KeyboardKey;
using enum KeyboardKey;

inline constexpr std::array<std::int32_t, 50> defaultGameKeyMappings{'R', 'E', 'Q', KeyTab, KeyF1, KeyF2, KeyF3, KeySpace, KeyLeftControl, KeyLeftShift, KeyUp,
    KeyDown, KeyLeft, KeyRight, 'W', 'S', 'A', 'D', '2', '3', '1', '4', KeyNumpad5, KeyNumpad6, KeyNumpad4, KeyNumpad8, KeyUnbound, KeyUnbound, KeyUnbound,
    KeyUnbound, KeyUnbound, KeyUnbound, 'E', 'C', 'M', KeyTab, KeyUnbound, KeyUnbound, KeyUnbound, KeyUnbound, 'S', 'W', KeyUnbound, KeyUnbound, KeyUnbound,
    KeyUnbound, KeyUnbound, KeyUnbound, 'A', 'D'};

inline constexpr std::array<std::uint32_t, 8> keyboardControllerButtonMasks{0x1000, 0x2000, 0x4000, 0x8000, 0x100, 0x200, 0x20, 0x10};
inline constexpr std::array<float, 4> keyboardLookAxisValues{1.0f, -1.0f, -1.0f, 1.0f};
inline constexpr std::array<std::uint32_t, 4> keyboardDirectionKeys{KeyUp, KeyDown, KeyLeft, KeyRight};

struct S2UserConfiguration280 {
    std::int32_t version;
    std::uint8_t swapTriggers;
    std::uint8_t invertLookY;
    std::byte reserved06[2];
    std::array<std::int32_t, 50> keyMappings;
    S2AudioSettings audio;
    std::int32_t windowMode;
    std::int32_t displayOutput;
    std::int32_t resolutionIndex;
    float resolutionScale;
    float superSampling;
    std::uint8_t fxaa;
    std::uint8_t enableSuperSampling;
    std::byte reserved102[2];
    std::int32_t bloomMode;
    std::int32_t aspectRatio;
    float gamma;
    float contrast;
    float uiSafeArea;
};

static_assert(sizeof(S2UserConfiguration280) == 280);
static_assert(offsetof(S2UserConfiguration280, keyMappings) == 0x08);
static_assert(offsetof(S2UserConfiguration280, audio) == 0xd0);
static_assert(offsetof(S2UserConfiguration280, windowMode) == 0xec);
static_assert(offsetof(S2UserConfiguration280, resolutionScale) == 0xf8);
static_assert(offsetof(S2UserConfiguration280, fxaa) == 0x100);
static_assert(offsetof(S2UserConfiguration280, gamma) == 0x10c);
static_assert(offsetof(S2UserConfiguration280, uiSafeArea) == 0x114);

S2UserConfiguration280 &GetDraftUserConfiguration();
const S2UserConfiguration280 &GetRuntimeUserConfiguration();
void AcknowledgeS2AudioOutput(std::int32_t device);
void SnapshotUserConfiguration();
bool HasUserConfigurationChanges();
bool HasScalingSettingsModified();
void ApplyDraftUserConfiguration();
void PreviewDraftUserConfiguration();
void RollbackUserConfiguration();
void ProcessUserSettingsConfig();
bool HasUnboundUserMappings();
void ResetDraftUserControls();
void ResetDraftUserAudio();
void ResetDraftUserGraphics();
inline constexpr std::array<std::int32_t, 18> menuKeyMappings{
    KeyEnter, KeyEscape, '1', '2', 'Z', 'C', KeyF3, KeyUnbound, 'Q', 'E', KeyUp, KeyDown, KeyLeft, KeyRight, 'W', 'S', 'A', 'D'};
inline constexpr std::array<const char *, 69> uiInputActionNames{
    "Confirm",
    "Cancel",
    "Event",
    "System",
    "Filter",
    "Snapshot",
    "Swap",
    "Pause",
    "Look",
    "Run",
    "LookUp",
    "LookDown",
    "LookLeft",
    "LookRight",
    "MoveUp",
    "MoveDown",
    "MoveLeft",
    "MoveRight",
    "QTE1",
    "QTE2",
    "QTE3",
    "QTE4",
    "FightKick",
    "FightThrow",
    "FightPunch",
    "FightBlock",
    "FightUnused1",
    "FightUnused2",
    "FightUnused3",
    "FightUnused4",
    "FightRun",
    "FightAssignedMove",
    "VehicleRaiseLift",
    "VehicleCamera",
    "VehicleMap",
    "VehicleMenu",
    "VehicleUnused1",
    "VehicleUnused2",
    "VehicleUnused3",
    "VehicleUnused4",
    "VehicleAccel",
    "VehicleReverse",
    "VehicleUnused5",
    "VehicleUnused6",
    "VehicleUnused7",
    "VehicleUnused8",
    "VehicleUnused9",
    "VehicleUnused10",
    "VehicleLeft",
    "VehicleRight",
    "MenuBarLeft",
    "MenuBarRight",
    "SubBarLeft",
    "SubBarRight",
    "FightLeft",
    "FightRight",
    "FightRun",
    "LookAll",
    "LookVertical",
    "LookHorizontal",
    "MoveAll",
    "MoveVertical",
    "MoveHorizontal",
    "DpadAll",
    "PressStartButton",
    "GameHudConfirm",
    "GameHudCancel",
    "None",
    "None",
};

extern std::array<std::int32_t, 50> g_gameKeyMappings;
void InitializeInputBindings();
void ReadInputBindingSettings(cJSON *controls);
void ApplyInputBindings();
std::uint32_t GetGameKeyMapping(std::int32_t action);
std::uint32_t GetMenuKeyMapping(std::int32_t action);
std::uint32_t GetQteKeyMapping(std::int32_t action);
std::uint32_t GetFightKeyMapping(std::int32_t action);
std::uint32_t MapInputKeyToCurrentLayout(std::uint32_t key);
bool FormatUIInputKeyName(std::int32_t key, std::array<char, 64> &name);
std::int32_t FindUIInputAction(const char *name, std::int32_t fallback);

} // namespace shenmue::s2
