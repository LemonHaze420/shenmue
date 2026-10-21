#pragma once

#include <cstdint>

namespace shenmue::s2 {
struct S2UserConfiguration280;
void ApplyS2UserConfigurationChanges(const S2UserConfiguration280 &previous);
extern float g_s2CurrentGamma;
extern float g_s2CurrentContrast;
extern bool g_s2FxaaEnabled;
extern bool g_s2PostEffectsRequested;
extern bool g_s2NeedsLoadTextureResources;

extern std::int32_t g_PPFX_Enabled;
void SetPPFXEnabled(std::int32_t enabled) noexcept;


struct S2AudioSettings {
    std::int32_t device{1};
    std::int32_t audioLanguage{};
    std::int32_t subtitleLanguage{-1};
    std::int32_t dialogueMode{2};
    float dialogueVolume{1.0f};
    float sfxVolume{1.0f};
    float musicVolume{1.0f};
};
static_assert(sizeof(S2AudioSettings) == 0x1c);
const S2AudioSettings &GetS2AudioSettings() noexcept;
std::int32_t GetS2DialogueMode() noexcept;
std::uint32_t GetS2GameConfigurationFlags() noexcept;
void SetS2GameConfigurationFlags(std::uint32_t flags) noexcept;
std::int32_t GetS2ControlMode() noexcept;
void ApplyS2DialogueMode(std::int32_t mode) noexcept;
void ApplyS2ControlMode(std::int32_t mode) noexcept;
void ApplyS2TriggerSwapMode(std::int32_t swapped) noexcept;
bool GetVoiceSettingEnabled() noexcept;


bool InitializeD3TRenderer(std::uint32_t width, std::uint32_t height, bool hidden);
extern float g_uiClientWidth, g_uiClientHeight, g_uiWindowWidth, g_uiWindowHeight;
std::int32_t CreateDXTextureForNuklearBufferForScreen();
extern float g_nuklearMessageX, g_nuklearMessageY;
void UpdateMouseForNuklear();
#if defined(_WIN32)
std::int64_t NuklearHandleWindowMessages(void *window, std::uint32_t message, std::uint64_t wParam, std::int64_t lParam);
#endif
bool AreS2TriggersSwapped() noexcept;
float GetS2UserInterfaceSafeArea() noexcept;
std::int32_t GetLogicalScreenHeight() noexcept;
bool ShouldScaleS2AspectRatio() noexcept;
bool GetAspectRatioModeShouldScale() noexcept;
float GetS2AspectRatioAdjustment() noexcept;
void ApplyS2AspectRatio() noexcept;
void ApplyUnscaledS2AspectRatio() noexcept;
void shenSetForcedFilmbars(std::int32_t enabled) noexcept;
extern std::int32_t g_aspectRatioRenderSuppressed;
extern std::int32_t renQteDoorMotionPending;

extern std::uint32_t g_backBufferCaptureTexture2D;

void DX_CreateTexture2D(
    std::uint32_t width, std::uint32_t height, std::uint32_t mipLevels, std::uint32_t format, std::uint32_t *texture, std::uint32_t bindFlags);

bool ReleaseTexture2DHandle(const std::uint32_t *texture);
void CaptureBackbuffer_SetupImmutableDXTex2D();
void AllocateAndCopyTexture();

void CopyBackBufferCaptureTexture(std::uint32_t destinationTexture);
bool BeginD3TRenderPass(bool clearTargets);
void ExecuteCurrentCommandList();
bool ProcessD3TMessages();
bool IsD3TWindowInputEnabled() noexcept;
void PresentD3TFrame(std::int32_t renderMode);
void Set60FPSForCredits();
void ShutdownD3TRenderer();

} // namespace shenmue::s2
