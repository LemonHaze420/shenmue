#include "s1/enhancements/rt_arcade.hpp"
#include "d3t/render_state.hpp"

#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/input.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/modules/harrier.hpp"
#include "s1/modules/harrier_cpu.hpp"
#include "s1/modules/harrier_render.hpp"
#include "s1/render_passes.hpp"
#include "s1/texture_cache.hpp"

#include <array>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "logging.hpp"

namespace shenmue::s1 {
namespace {

struct CabinetScreen {
    std::uint32_t original;
    std::uint32_t atlas;
    std::uint32_t frame;
    std::uint32_t interactiveFrame;
};
CabinetScreen g_hangOnScreen{};
CabinetScreen g_harrierScreen{};
HangOnSharedState g_attractHangOnShared{};
HangOnUpdateBlock g_attractHangOnUpdate{};
std::array<std::byte, 3200> g_attractHangOnAudio{};
void *g_attractHangOnSession;
void *g_attractHangOnMemory;
bool g_attractHarrierReady;
std::chrono::steady_clock::time_point g_nextAttractTick;
std::uint32_t g_atlasPixelShader;

bool InitializeAtlasShader() {
    if (g_atlasPixelShader)
        return true;
    ID3DBlob *code = nullptr, *errors = nullptr;
    const HRESULT result = D3DCompile(kRTArcadeAtlasPixelShader, sizeof(kRTArcadeAtlasPixelShader) - 1, "RTArcadeAtlas", nullptr, nullptr, "main", "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &errors);
    if (FAILED(result)) {
        if (errors)
            errors->Release();
        return false;
    }
    if (errors)
        errors->Release();
    g_atlasPixelShader = CreateShader(static_cast<std::int32_t>(D3TShaderType::Pixel));
    const bool ready = d3t::CreateShaderResource(g_atlasPixelShader, code->GetBufferPointer(), code->GetBufferSize(), D3TShaderType::Pixel);
    code->Release();
    if (!ready) {
        DestroyShader(g_atlasPixelShader);
        g_atlasPixelShader = 0;
    }
    return ready;
}

constexpr std::array<Vector4, 4> kAtlasQuad{{{-1, -1, 0, 1}, {1, -1, 1, 1}, {-1, 1, 0, 0}, {1, 1, 1, 0}}};

constexpr std::array<Vector4, 4> kHangOnScreenQuad{{
    {0.00390625f, -0.984375f, 0.8125f, 0.0625f}, {0.00390625f, -0.76171875f, 0.8125f, 0.9375f},
    {1.0f, -0.984375f, 0.1875f, 0.0625f}, {1.0f, -0.76171875f, 0.1875f, 0.9375f},
}};
constexpr std::array<Vector4, 4> kHarrierScreenQuad{{
    {-1.0f, -0.25390625f, 0, 1}, {0.00390625f, -0.25390625f, 0, 0},
    {-0.99609375f, 0.99609375f, 1, 1}, {0.00390625f, 1.0f, 1, 0},
}};

void DrawAtlasQuad(D3TDeviceContext *context, std::uint32_t source, const std::array<Vector4, 4> &quad) {
    SetShaderState(context, g_texturedQuadVertexShader, g_atlasPixelShader, g_fvfLayouts[34]);
    SetCommonQuadState(context, 0xf0000000ULL);
    SetTopology(context, 1);
    BindPixelTexture8(context, source);
    std::memcpy(GetMappedConstantBuffer(context, 8, sizeof(quad)), quad.data(), sizeof(quad));
    SubmitQuad(context);
}

void UpdateAtlas(D3TDeviceContext *context, CabinetScreen &screen, const std::array<Vector4, 4> &quad) {
    if (!screen.original || (!screen.frame && !screen.interactiveFrame))
        return;
    bool fresh = false;
    std::uint32_t width = 0, height = 0;
    if (!GetTexture2DDimensions(screen.original, &width, &height))
        return;
    if (!screen.atlas) {
        const Texture2DDescription description{.width = width, .height = height, .arraySize = 1, .mipLevels = 1, .format = 3, .bindFlags = 4, .sampleCount = 1};
        screen.atlas = CreateTexture2D(&description);
        fresh = true;
    }
    SetRenderTarget(context, screen.atlas);
    SetViewport(context, static_cast<float>(width), static_cast<float>(height));
    if (fresh)
        DrawAtlasQuad(context, screen.original, kAtlasQuad);
    DrawAtlasQuad(context, screen.interactiveFrame ? screen.interactiveFrame : screen.frame, quad);
}

void RetireScreen(CabinetScreen &screen) {
    if (screen.atlas)
        InitTexture2DResource(screen.atlas);
    if (screen.frame)
        InitTexture2DResource(screen.frame);
    screen = {};
}

void StartHangOnAttract() {
    if (g_attractHangOnSession)
        return;
    if (!LoadHangOnLibrary())
        return;
    g_attractHangOnShared = {};
    const HangOnConfig config{.mode = 2, .shared = &g_attractHangOnShared};
    const std::size_t size = g_hangOnLibrary.computeMemoryRequired(&config);
    g_attractHangOnMemory = std::calloc(1, size);
    g_attractHangOnSession = g_hangOnLibrary.create(&config, g_attractHangOnMemory);
    g_attractHangOnUpdate = {.steering = 127, .audioPlayLength = 800, .audioData = g_attractHangOnAudio.data(), .audioBytes = 3200};
    const Texture2DDescription description{.width = 512, .height = 256, .arraySize = 1, .mipLevels = 1, .format = 0x25, .sampleCount = 1};
    g_hangOnScreen.frame = CreateTexture2D(&description);
}

void StartHarrierAttract() {
    if (g_attractHarrierReady)
        return;
    g_harrierSession = {.mode = 2, .coreKind = 3};
    std::memcpy(g_harrierSession.rankings, kHarrierDefaultRankings.data(), kHarrierDefaultRankings.size());
    Harrier_Init();
    g_harrierRemainingCredits = 0;
    g_harrierDisplayCredits = 0;
    HarrierInit();
    const Texture2DDescription description{.width = 640, .height = 480, .arraySize = 1, .mipLevels = 1, .format = 3, .bindFlags = 4, .sampleCount = 1};
    g_harrierScreen.frame = CreateTexture2D(&description);
    g_attractHarrierReady = true;
}

std::uint32_t FindTexture(std::uint32_t low, std::uint32_t high) {
    const std::int32_t surface = PVR_FindSurface(static_cast<std::int32_t>(low), static_cast<std::int32_t>(high));
    return surface < 0 ? 0 : GetPVRSurfaceTextureHandle(surface);
}

} // namespace

void RTArcade_Tick() {
    const std::uint32_t hangOn = FindTexture(0x61c6a3dbU, 0x5f3238aaU);
    const std::uint32_t harrier = FindTexture(0x94c7a1e7U, 0x5f423860U);
    if (!hangOn && !harrier) {
        RTArcade_Shutdown();
        return;
    }
    if (Arcade_ActiveGame() != ArcadeGame::None || g_isRenderSuppressed)
        return;
    if (!InitializeAtlasShader())
        return;
    if (g_hangOnScreen.original != hangOn) {
        if (g_hangOnScreen.atlas)
            InitTexture2DResource(g_hangOnScreen.atlas);
        g_hangOnScreen.atlas = 0;
        g_hangOnScreen.original = hangOn;
    }
    if (g_harrierScreen.original != harrier) {
        if (g_harrierScreen.atlas)
            InitTexture2DResource(g_harrierScreen.atlas);
        g_harrierScreen.atlas = 0;
        g_harrierScreen.original = harrier;
    }
    if (hangOn)
        StartHangOnAttract();
    if (harrier)
        StartHarrierAttract();
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if (g_nextAttractTick == std::chrono::steady_clock::time_point{})
        g_nextAttractTick = now;
    if (now < g_nextAttractTick)
        return;
    D3TDeviceContext *const previous = DX11::g_ppD3DDeviceContext;
    D3TDeviceContext *const context = d3t_DXGetPipelineContext(1);
    if (!context)
        return;
    DX11::g_ppD3DDeviceContext = context;
    const ShaderParams params = g_renderParams;
    const LegacyShaderParameters legacyParams = g_legacyShaderParameters;
    const LegacyRendererDefaults defaults = g_legacyRendererDefaults;
    const std::array<std::int32_t, 172> flags = g_currentRenderFlags;
    const std::uint64_t engineFlags = g_legacyEngineFlags;
    const std::uint64_t engineField8 = g_legacyEngineField8;
    const std::int32_t renderingMode = g_legacyCurrentRenderingMode;
    const std::array<std::uint32_t, 2> alphaFlags = g_legacyAlphaFlags;
    const InputSlot input = g_inputState.slots[0];
    g_inputState.slots[0] = {.rightStick = 0x8080, .leftStick = 0x8080};
    while (g_nextAttractTick <= now) {
        if (g_attractHangOnSession)
            g_hangOnLibrary.update(g_attractHangOnSession, &g_attractHangOnUpdate);
        if (g_attractHarrierReady) {
            g_harrierInputMask = 0xff;
            emu68_Run();
        }
        g_nextAttractTick += std::chrono::nanoseconds{16666667};
    }
    g_inputState.slots[0] = input;
    if (g_attractHangOnSession && g_attractHangOnUpdate.frame) {
        D3TTextureBackendView view{};
        if (GetTexture2DBackendView(g_hangOnScreen.frame, &view))
            static_cast<ID3D11DeviceContext *>(context->deviceContext)
                ->UpdateSubresource(static_cast<ID3D11Texture2D *>(view.texture), 0, nullptr, g_attractHangOnUpdate.frame, 512 * 4, 0);
        UpdateAtlas(context, g_hangOnScreen, kHangOnScreenQuad);
    }
    if (g_attractHarrierReady) {
        SetRenderTarget(context, g_harrierScreen.frame);
        SetViewport(context, 640.0f, 480.0f);
        SetRenderFlags(7, 0);
        CreateAndDrawEmu();
        UpdateAtlas(context, g_harrierScreen, kHarrierScreenQuad);
    }
    DX11::WaitForCommandListFinish(context);
    DX11::g_ppD3DDeviceContext = previous;
    g_renderParams = params;
    g_legacyShaderParameters = legacyParams;
    g_legacyRendererDefaults = defaults;
    g_currentRenderFlags = flags;
    g_legacyEngineFlags = engineFlags;
    g_legacyEngineField8 = engineField8;
    g_legacyCurrentRenderingMode = renderingMode;
    g_legacyAlphaFlags = alphaFlags;
}

void RTArcade_BeginInteraction(ArcadeGame game) {
    CabinetScreen &screen = game == ArcadeGame::HangOn ? g_hangOnScreen : g_harrierScreen;
    if (game == ArcadeGame::SpaceHarrier && g_attractHarrierReady) {
        HarrierDestroy();
        g_attractHarrierReady = false;
        InitTexture2DResource(screen.frame);
        screen.frame = 0;
    }
    g_nextAttractTick = {};
}

void RTArcade_SetInteractiveFrame(ArcadeGame game, std::uint32_t texture) {
    CabinetScreen &screen = game == ArcadeGame::HangOn ? g_hangOnScreen : g_harrierScreen;
    screen.interactiveFrame = texture;
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (context && screen.atlas) {
        d3t::D3TRenderState &state = d3t::GetRenderState(*context);
        const d3t::D3TRenderState saved = state;
        UpdateAtlas(context, screen, game == ArcadeGame::HangOn ? kHangOnScreenQuad : kHarrierScreenQuad);
        state = saved;
        state.pipelineStateFlags |= 0xbbU;
        state.bufferStateFlags |= d3t::BufferOutputTargets | d3t::BufferTopology;
        state.viewportDirtyMask |= 1U;
        state.shaderResourceDirtyMasks[1] |= 1U << 8U;
        state.samplerDirtyMasks[1] |= 1U;
        state.constantBufferDirtyMasks[0] = 0xffffU;
        state.constantBufferDirtyMasks[1] = 0xffffU;
    }
}

void RTArcade_EndInteraction(ArcadeGame game) {
    CabinetScreen &screen = game == ArcadeGame::HangOn ? g_hangOnScreen : g_harrierScreen;
    screen.interactiveFrame = 0;
    g_nextAttractTick = {};
}

std::uint32_t RTArcade_ResolveTexture(std::uint32_t texture) {
    if (texture == g_hangOnScreen.original && g_hangOnScreen.atlas)
        return g_hangOnScreen.atlas;
    if (texture == g_harrierScreen.original && g_harrierScreen.atlas)
        return g_harrierScreen.atlas;
    return texture;
}

void RTArcade_Shutdown() {
    if (g_attractHangOnSession) {
        g_hangOnLibrary.destroy(g_attractHangOnSession);
        std::free(g_attractHangOnMemory);
        g_attractHangOnSession = nullptr;
        g_attractHangOnMemory = nullptr;
    }
    if (g_attractHarrierReady) {
        HarrierDestroy();
        g_attractHarrierReady = false;
    }
    if (g_atlasPixelShader) {
        DestroyShader(g_atlasPixelShader);
        g_atlasPixelShader = 0;
    }
    RetireScreen(g_hangOnScreen);
    RetireScreen(g_harrierScreen);
    g_nextAttractTick = {};
}

} // namespace shenmue::s1
