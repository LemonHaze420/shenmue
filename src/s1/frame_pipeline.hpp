#pragma once

#include "s1/d3t/namespace.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <array>
#include <cstdint>

namespace shenmue::d3t {
struct D3TDeviceContext;
}

namespace shenmue::s1 {




struct CameraConstantBlock {
    Matrix4x4 postRender;
    Matrix4x4 transparent;
    Matrix4x4 inverseTransparent;
    Matrix4x4 transparentPostRender;
    Matrix4x4 inversePostRender;
    Matrix4x4 postEffectTransform;
    Matrix4x4 previousPostEffectTransform;
    Matrix4x4 reserved_1c0;
    Vector4 projectedOrigin;
};

static_assert(sizeof(CameraConstantBlock) == 0x210);

struct ModernRendererDebugConstants {
    std::int32_t renderMode;
    std::uint32_t visualizeCascade;
    std::uint32_t environmentProbesEnabled;
    std::uint32_t probeVisualizationIndex;
    std::int32_t specularBrdfMode;
    std::int32_t probeVisualizationMode;
    std::uint32_t useDualQuaternionSkinning;
    std::uint32_t visualizeClusters;
    float clampDirectRoughness;
};

static_assert(sizeof(ModernRendererDebugConstants) == 36);
extern ModernRendererDebugConstants g_modernRendererDebugConstants;
extern float g_modernDayFraction;
void UploadModernRendererDebugConstants(d3t::D3TDeviceContext *context);
void InitGameBuffers();

struct UIBaseRuntime {
    const char *name;
    std::int32_t index;
    std::int32_t type;
    std::uint32_t (*parseDefinitionFromFile)();
    std::int64_t (*isSelected)();
    std::int64_t (*isPressed)();
    std::int64_t (*unknown28)();
    std::int64_t (*tick)();
    std::int64_t (*postInit)();
    std::int64_t (*unknown40)();
    void (*render)();
    void (*initialize)();
    std::int64_t (*destroy)();
    std::int64_t (*unknown60)();
    std::int64_t (*reset)();
    std::int64_t (*reloadUI)();
    std::int64_t (*loadState)();
    std::int64_t (*unknown80)();
    std::int64_t (*getContentPanel)();
    std::int64_t (*elementDraw)();
    std::int64_t (*getSearchType)();
    std::int32_t isVisible;
    std::int32_t isActive;
    std::int32_t isHandledExternally;
    std::int32_t field_ac;
    std::int32_t setup;
    std::int32_t someFlag3;
    char uiConfigFilepath[260];
    struct {
        std::int32_t init;
        std::int16_t screenWidth;
        std::int16_t screenHeight;
        std::int32_t uiHandleIndex;
        std::int32_t isEnabled;
        std::int32_t status;
    } config;
};

static_assert(offsetof(UIBaseRuntime, tick) == 0x30);
static_assert(offsetof(UIBaseRuntime, destroy) == 0x58);
static_assert(offsetof(UIBaseRuntime, isVisible) == 0xa0);
static_assert(offsetof(UIBaseRuntime, setup) == 0xb0);
static_assert(offsetof(UIBaseRuntime, config) == 0x1bc);
static_assert(sizeof(UIBaseRuntime) == 0x1d0);

extern UIBaseRuntime &LoadScreen;
extern std::array<UIBaseRuntime, 80> g_uiElements;


extern UIBaseRuntime *const g_cinemaBars;
extern std::int32_t g_currUIState;
extern std::int32_t g_waitingForNextTick;
extern std::int32_t g_NumFramesProcessed;
extern std::int32_t g_FrameCounter;
extern std::int32_t g_FramePacingFactor;
extern std::uint8_t b60fps;
extern std::int32_t g_NuklearInitialized;
extern std::int32_t g_bNeedsLoadTextureResources;
extern std::int32_t g_currentSplashScreenIndex;
extern void (*g_pfnDrawNuklear)();
extern std::uint32_t g_Nuklear_num_indices;
extern std::uint32_t g_Nuklear_num_verts;
extern std::int32_t g_busy_reset;

void DispatchRenderPipeline(void *renderContext, void *frameInfo);
void RenderScreen();
void FlushRenderQueue();
void Shenmue_DispatchRenderPipeline();
void RenderScreen_Wrapper(const std::uint32_t *renderHandles, std::int32_t count);
void PresentLatestFrame();
[[noreturn]] void Shutdown();
namespace d3t {
void ProfilePerf();
} // namespace d3t
void RenderPipeline(std::int64_t unused, std::int32_t deferred, std::int32_t renderType);
void BeginSceneFrameSubmission();
void BeginSceneCompositionSubmission();
void FinishSceneFrameSubmission();


d3t::D3TDeviceContext *AcquireScenePipelineContext();
void SubmitAndReleasePipelineContext();
void UpdateMouseForNuklear();
namespace d3t {
void ProcessMessages();
void ActivateElementByIdx(std::int32_t elementIndex);
bool ResetElementByIdx(std::int32_t elementIndex);
} // namespace d3t

} // namespace shenmue::s1
