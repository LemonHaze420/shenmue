#include "shenmue.h"

#if SHENMUE_IS_DEMO
#include "auto_test.hpp"
#include "shenmue.h"

#include "s1/task/disp.hpp"
#include "s1/boot.hpp"
#include "s1/input.hpp"
#include "s1/player_stats.hpp"
#include "s1/file_loader.hpp"
#include "s1/screen_space.hpp"
#include "s1/texture_loader.hpp"
#include "s1/task/film.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/krep.hpp"
#if SHEN_AUTO_TEST_ENABLED && defined(_WIN32)
#include "s1/timing.hpp"
#endif

#include <bit>

#if SHENMUE_IS_PC
#include "s1/ui_runtime.hpp"
#include "s1/render_passes.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/texture_cache.hpp"
#include "s1/scene.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include <vector>
#else
#include "platform/dc/sdk.hpp"
#endif

namespace shenmue::s1 {

void ClearScenarioFlags();
void TIME_ResetTrialClock();
extern std::int32_t g_sceneId;
extern std::uint32_t g_areaId;
extern std::uint32_t g_entryPoint;
std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t argument, std::int32_t parameter);
SystemModule g_trialTitleNextModule{};
std::int32_t g_trialTitlePromptAlpha{};
std::int32_t g_trialTitleAlternateAlpha{};
std::uint32_t g_trialTitlePromptColour{};
std::uint32_t g_trialTitleAlternateColour{};
std::int16_t g_trialTitleInputDelay{};
std::int16_t g_trialTitleBlinkTicks{};
std::uint8_t g_trialTitleBlinkPhase{};
std::array<std::int32_t, 3> g_trialTitleTextures{};
std::array<std::int32_t, 71> g_trialSystemTitleTextures{};
std::uint8_t g_trialTitleState{};
std::int8_t g_trialTitleSelection{1};
std::int8_t g_trialTitleAreaSelection{};
std::int8_t g_trialTitleOptionSelection{};
std::int8_t g_trialTitleAudioOption{};
std::int8_t g_trialTitleAreaJumpOption{};
std::int8_t g_trialTitleDialogueOption{};
std::int8_t g_trialTitlePreviousDialogueOption{};
std::uint16_t g_trialTitlePressed{};
std::uint16_t g_trialTitleHeld{};
std::uint16_t g_trialTitleRepeated{};
std::int16_t g_trialTitleRepeatDelay{};
std::uint32_t g_trialTitleCursorColour{};
std::int32_t g_trialTitleCursorAlpha{};
std::int32_t g_trialTitleCursorDelta{};

// @verified - d
void TrialTitle_ResetBlink(std::int8_t reverse) {
    g_trialTitleBlinkTicks = 18;
    g_trialTitleBlinkPhase = 0;
    g_trialTitlePromptAlpha = reverse ? 255 : 0;
    g_trialTitleAlternateAlpha = reverse ? 0 : 255;
    g_trialTitlePromptColour = 0x00ffffff;
    g_trialTitleAlternateColour = 0x00ffffff;
    g_trialTitleNextModule = SystemModule::SetToDisplay;
}

#if SHENMUE_IS_PC
namespace {
struct TrialTitleImage {
    NuklearQuad quad;
    std::int32_t surface;
    float depth;
};
std::vector<TrialTitleImage> g_trialTitleImages;
} // namespace
#endif

void TrialTitle_SubmitStrip(OriginalStrip *strip, std::int32_t surface, float depth) {
#if SHENMUE_IS_PC

    const float scaleX = static_cast<float>(g_uiScreenWidth) / 640.0f;
    const float scaleY = 1080.0f / 480.0f;
    const NuklearQuad quad{.x = strip->x0 * scaleX,
        .y = strip->y0 * scaleY,
        .width = (strip->x1 - strip->x0) * scaleX,
        .height = (strip->y2 - strip->y0) * scaleY,
        .u0 = strip->normalizedY1,
        .v0 = strip->normalizedX3,
        .u1 = strip->normalizedY2,
        .v1 = strip->normalizedX2,
        .colour = strip->corner0};
    g_trialTitleImages.push_back({.quad = quad, .surface = surface, .depth = depth});
#else
    SetRenderFlags_UnmapAndDraw_0(strip, surface, depth);
#endif
}

bool TrialTitle_DrawFrontend(std::uint32_t renderTarget) {
    bool drawn = false;
#if SHENMUE_IS_PC

    for (std::size_t index = 1; index < g_trialTitleImages.size(); ++index) {
        const TrialTitleImage image = g_trialTitleImages[index];
        std::size_t position = index;
        while (position && g_trialTitleImages[position - 1].depth > image.depth) {
            g_trialTitleImages[position] = g_trialTitleImages[position - 1];
            --position;
        }
        g_trialTitleImages[position] = image;
    }
    for (const TrialTitleImage &image : g_trialTitleImages) {
        const SurfaceRecord *const surface = GetPVRSurfaceRecord(image.surface);
        if (surface)
            drawn |= DrawNuklearQuads(g_RenderDeviceContext, renderTarget, surface->secondary.extraParam, std::span{&image.quad, 1});
    }
    g_trialTitleImages.clear();
#else
    (void)renderTarget;
#endif
    return drawn;
}

void TrialTitle_DrawPressStart() {
    if (g_trialTitleInputDelay == 0
#if SHEN_AUTO_TEST_ENABLED
        && !auto_test::TitleLatched()
#endif
    ) {
        if (--g_trialTitleBlinkTicks <= 0) {
            g_trialTitleBlinkTicks = 18;
            g_trialTitleBlinkPhase = (g_trialTitleBlinkPhase + 1) & 1U;
            g_trialTitlePromptAlpha = g_trialTitleBlinkPhase ? 0 : 255;
            g_trialTitleAlternateAlpha = g_trialTitleBlinkPhase ? 255 : 0;
        }
        if (g_trialTitleBlinkPhase) {
            g_trialTitlePromptAlpha += 32;
            if (g_trialTitlePromptAlpha >= 255)
                g_trialTitlePromptAlpha = 255;
            g_trialTitleAlternateAlpha -= 32;
            if (g_trialTitleAlternateAlpha <= 0)
                g_trialTitleAlternateAlpha = 0;
        } else {
            g_trialTitlePromptAlpha -= 32;
            if (g_trialTitlePromptAlpha <= 0)
                g_trialTitlePromptAlpha = 0;
            g_trialTitleAlternateAlpha += 32;
            if (g_trialTitleAlternateAlpha >= 255)
                g_trialTitleAlternateAlpha = 255;
        }
    }
    g_trialTitlePromptColour = (static_cast<std::uint32_t>(g_trialTitlePromptAlpha) << 24U) | 0x00ffffffU;
    g_trialTitleAlternateColour = (static_cast<std::uint32_t>(g_trialTitleAlternateAlpha) << 24U) | 0x00ffffffU;
    OriginalStrip strip{
        .flags = 0x400, .corner0 = 0xffffffff, .normalizedY1 = 0.0f, .normalizedX2 = 0.0f, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f, .normalizedY3 = 0.0f};
    AdjustAspectRatioMatrix(&strip, 5, 320, 240, 1024, 512, 3);
    TrialTitle_SubmitStrip(&strip, g_trialTitleTextures[1], -8.0f);
    strip.corner0 = g_trialTitlePromptColour;
    AdjustAspectRatioMatrix(&strip, 5, 320, 300, 512, 32, 3);
    TrialTitle_SubmitStrip(&strip, g_trialTitleTextures[0], -7.0f);
    strip.corner0 = g_trialTitleAlternateColour;
    AdjustAspectRatioMatrix(&strip, 5, 320, 300, 64, 64, 3);
    TrialTitle_SubmitStrip(&strip, g_trialTitleTextures[2], -7.0f);
}

namespace {
inline constexpr ColourBlendPair kTrialTitleFadeIn{.colourA = {.red = 255}, .colourB = {}, .interpolationFrames = 15};
inline constexpr ColourBlendPair kTrialTitleFadeOut{.colourA = {}, .colourB = {.red = 255}, .interpolationFrames = 15};

void DrawTrialTitleQuad(OriginalStrip &strip, const TrialTitleQuad &quad, std::int32_t texture, float x) {
    AdjustAspectRatioMatrix(
        &strip, 5, static_cast<std::int32_t>(x), static_cast<std::int32_t>(quad.y), quad.width, quad.height, SHENMUE_IS_PC ? 1 : 0);
    TrialTitle_SubmitStrip(&strip, g_trialSystemTitleTextures[texture], quad.depth);
}
} // namespace

// @verified - d
void TrialTitle_ResetState() {
    g_trialTitleState = 0;
}

void TrialTitle_ResetGame() {
    ClearScenarioFlags();
    g_sceneId = 80;
    g_entryPoint = 0;
    shenInitDefaultStats();
    shenInitDefaultCassetteInventory();
    CDTT_SetMode(CDTTMode::GameplayDateSetup);
    TIME_ResetTrialClock();
#if SHEN_AUTO_TEST_ENABLED && defined(_WIN32)
    if (auto_test::SceneIs("ws-face-ryo")) {
        g_EnvPrngSeed = 1;
        g_bEnableFrameTimeSmoothing = 0;
    }
#endif
}

void TrialTitle_Module(HLib::TaskData *taskData) {
    const std::uint32_t titleSize = static_cast<std::uint32_t>(shenOpenFile("sprite", "TNTITLE.SPR"));
    void *const title = HLib_AllocHeapMemBlock(titleSize, kTrialTitleToken.name);
    shenLoadFile("sprite", "TNTITLE.SPR", title, static_cast<std::int32_t>(titleSize));
    LoadSpriteTextureList(title, titleSize, g_trialTitleTextures.data());
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(title), 0);
    const std::uint32_t systemSize = static_cast<std::uint32_t>(shenOpenFile("sprite", "TNSYSNCG.SPR"));
    void *const system = HLib_AllocHeapMemBlock(systemSize, kTrialSystemTitleToken.name);
    shenLoadFile("sprite", "TNSYSNCG.SPR", system, static_cast<std::int32_t>(systemSize));
    LoadSpriteTextureList(system, systemSize, g_trialSystemTitleTextures.data());
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(system), 0);
    krepEnqueue(0);
    TrialTitle_ResetBlink(0);
    ModuleTaskData &module = *reinterpret_cast<ModuleTaskData *>(taskData);
    TrialTitleWork &work = *reinterpret_cast<TrialTitleWork *>(module.data);
    work.film = FILM_Init(8);
    if (g_trialTitleState == 1) {
        g_trialTitleInputDelay = 0;
        const std::int8_t area = g_trialTitleAreaSelection;
        TrialTitle_ResetGame();
        g_trialTitleState = 1;
        g_trialTitleAreaSelection = area;
        HLib_SetCurrentTaskMainCallback(TrialTitle_EnterMenu);
        SOND_PlayCdAudio(0xf0006, 15);
    } else {
        ConstructColours(work.film, &kTrialTitleFadeIn, ConstructColourMode::ApplyImmediately);
        work.fadeDelay = 15;
        g_trialTitleInputDelay = 60;
        work.attractDelay = 600;
        HLib_SetCurrentTaskMainCallback(TrialTitle_FadeIn);
        SOND_PlayCdAudio(0xf0008, 0);
    }
}

// @verified - d
void TrialTitle_FadeIn(HLib::TaskData *taskData) {
    TrialTitle_DrawPressStart();
    TrialTitleWork &work = *reinterpret_cast<TrialTitleWork *>(reinterpret_cast<ModuleTaskData *>(taskData)->data);
    if (--work.fadeDelay < 0)
        HLib_SetCurrentTaskMainCallback(TrialTitle_PressStart);
}

// @verified - d
void TrialTitle_FadeOut(HLib::TaskData *taskData) {
    TrialTitleWork &work = *reinterpret_cast<TrialTitleWork *>(reinterpret_cast<ModuleTaskData *>(taskData)->data);
    if (--work.fadeDelay < 0)
        HLib_SetCurrentTaskMainCallback(TrialTitle_Exit);
    else
        TrialTitle_DrawPressStart();
}

void TrialTitle_PressStart(HLib::TaskData *taskData) {
#if SHEN_AUTO_TEST_ENABLED
    if (auto_test::TitleLatched()) {
        TrialTitle_DrawPressStart();
        return;
    }
#if defined(_WIN32)
    g_inputState.slots[0].pressedPriority = auto_test::SceneIs("ws-face-ryo") && !g_trialTitleInputDelay ? 0x800U : 0;
#else
    g_inputState.slots[0].pressedPriority = 0;
#endif
#endif
    g_isOnMainMenu = 1;
    if (--g_trialTitleInputDelay < 0)
        g_trialTitleInputDelay = 0;
    if (!g_trialTitleInputDelay && (g_inputState.slots[0].pressedPriority & 0x800U)) {
        shenExecuteAICACommand(0x900ab, 0, 0);
        TrialTitle_ResetGame();
        HLib_SetCurrentTaskMainCallback(TrialTitle_EnterMenu);
    } else {
        TrialTitleWork &work = *reinterpret_cast<TrialTitleWork *>(reinterpret_cast<ModuleTaskData *>(taskData)->data);
        if (--work.attractDelay < 0) {
            ConstructColours(work.film, &kTrialTitleFadeOut, ConstructColourMode::ApplyImmediately);
            work.fadeDelay = 15;
            g_trialTitleNextModule = SystemModule::SetToDisplay;
            HLib_SetCurrentTaskMainCallback(TrialTitle_FadeOut);
        }
    }
    TrialTitle_DrawPressStart();
#if SHEN_AUTO_TEST_ENABLED
    const TrialTitleWork &captureWork = *reinterpret_cast<TrialTitleWork *>(reinterpret_cast<ModuleTaskData *>(taskData)->data);
    auto_test::LatchTitleFrame(
#if defined(_WIN32)
        !auto_test::SceneIs("ws-face-ryo") &&
#endif
        !g_trialTitleInputDelay && captureWork.attractDelay > 0 && g_trialTitlePromptAlpha == 255);
#endif
}

void TrialTitle_EnterMenu(HLib::TaskData *) {
    g_isOnMainMenu = 0;
    if (g_trialTitleState == 1)
        g_trialTitleSelection = 1;
    else {
        g_trialTitleState = 0;
        TrialTitle_DrawPressStart();
    }
    g_trialTitleCursorColour = 0xff000000;
    g_trialTitleCursorAlpha = 0;
    g_trialTitleCursorDelta = 3;
    SOND_PlayCdAudio(0xf0007, 15);
    HLib_SetCurrentTaskMainCallback(TrialTitle_Menu);
}

void TrialTitle_Menu(HLib::TaskData *taskData) {
#if SHEN_AUTO_TEST_ENABLED && defined(_WIN32)
    if (auto_test::SceneIs("ws-face-ryo")) {
        g_trialTitleSelection = 1;
        g_trialTitleAreaSelection = 0;
        g_inputState.slots[0].pressedPriority = 0x400U;
    }
#endif
    g_trialTitleHeld = g_inputState.slots[0].held;
    g_trialTitlePressed = g_inputState.slots[0].pressedPriority;
    if (!(g_trialTitleHeld & 0xf000U) && !(g_trialTitlePressed & 0xf000U)) {
        g_trialTitleRepeatDelay = 16;
        g_trialTitleRepeated = 0;
    } else if (g_trialTitleHeld & 0xf000U) {
        if (--g_trialTitleRepeatDelay == 0) {
            g_trialTitleRepeatDelay = 4;
            g_trialTitleRepeated = g_trialTitleHeld & 0xf000U;
        } else
            g_trialTitleRepeated = g_trialTitlePressed & g_trialTitleHeld & 0xf000U;
    }
    g_trialTitleCursorAlpha += g_trialTitleCursorDelta;
    if (g_trialTitleCursorAlpha <= 0)
        g_trialTitleCursorDelta = 3;
    else if (g_trialTitleCursorAlpha >= 159)
        g_trialTitleCursorDelta = -3;
    g_trialTitleCursorColour = 0xff000000U | static_cast<std::uint32_t>(g_trialTitleCursorAlpha) << 16U;
    if (g_trialTitleState == 1)
        TrialTitle_AreaSelect();
    else if (g_trialTitleState == 3)
        TrialTitle_Options();
    else if (g_trialTitleState == 0) {
        if (g_trialTitleRepeated & 0x1000U) {
            shenExecuteAICACommand(0xab, 0, 0);
            if (--g_trialTitleSelection < 1)
                g_trialTitleSelection = 3;
        }
        if (g_trialTitleRepeated & 0x2000U) {
            shenExecuteAICACommand(0xab, 0, 0);
            if (++g_trialTitleSelection > 3)
                g_trialTitleSelection = 1;
        }
        if (g_trialTitlePressed & 0x400U) {
            shenExecuteAICACommand(0x900ab, 0, 0);
            g_trialTitleState = static_cast<std::uint8_t>(g_trialTitleSelection);
            if (g_trialTitleSelection == 2) {
                g_areaId = kTrialAreaMain.name;
                g_trialTitleNextModule = SystemModule::Map;
                HLib_SetCurrentTaskMainCallback(TrialTitle_Exit);
            } else if (g_trialTitleSelection == 1)
                SOND_PlayCdAudio(0xf0006, 15);
            else if (g_trialTitleSelection == 3) {
                g_trialTitleAudioOption = static_cast<std::int8_t>(SOND_GetSystemOutputMode());
                g_trialTitleAreaJumpOption = static_cast<std::int8_t>(GetAreaJumpEnabled());
                g_trialTitleDialogueOption = static_cast<std::int8_t>(GetDialogueMode());
                SOND_PlayCdAudio(0xe0005, 15);
            }
        } else if (g_trialTitlePressed & 0x200U) {
            shenExecuteAICACommand(0x800ab, 0, 0);
            TrialTitle_ResetBlink(1);
            reinterpret_cast<TrialTitleWork *>(reinterpret_cast<ModuleTaskData *>(taskData)->data)->attractDelay = 600;
            HLib_SetCurrentTaskMainCallback(TrialTitle_PressStart);
            SOND_PlayCdAudio(0xf0008, 0);
        }
        OriginalStrip strip{.flags = 0x400, .corner0 = 0xffffffff, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f};
        for (const TrialTitleQuad &quad : kTrialMainMenuQuads) {
            const std::int32_t texture = GetAreaJumpEnabled() && quad.alternate != 71 && quad.alternate != 69 ? quad.alternate : quad.texture;
            DrawTrialTitleQuad(strip, quad, texture, quad.x);
        }
        strip.vertexCount = g_trialTitleCursorColour;
        const TrialTitleQuad &row = kTrialMainMenuQuads[static_cast<std::size_t>(g_trialTitleSelection) * 2];
        UILayout_BuildQuad(&strip, 5, 191, static_cast<std::int32_t>(row.y + 2.0f), 32, 32, std::bit_cast<float>(0x3fd99999U), 2.0f, 0,
            SHENMUE_IS_PC ? 1 : 0);
        TrialTitle_SubmitStrip(&strip, g_trialSystemTitleTextures[2], -7.0f);
    }
    OriginalStrip background{.flags = 0x400, .corner0 = 0xffffffff, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f};
    AdjustAspectRatioMatrix(&background, 5, 320, 240, 1024, 512, SHENMUE_IS_PC ? 1 : 0);
    TrialTitle_SubmitStrip(&background, g_trialSystemTitleTextures[1], -8.0f);
}

void TrialTitle_AreaSelect() {
    const std::uint16_t directions[4]{0x4000, 0x8000, 0x1000, 0x2000};
    for (std::int8_t index = 0; index < 4; ++index) {
        if ((g_trialTitlePressed & directions[index]) && g_trialTitleAreaSelection != index) {
            shenExecuteAICACommand(0xab, 0, 0);
            g_trialTitleAreaSelection = index;
        }
    }
    if (g_trialTitlePressed & 0x400U) {
        shenExecuteAICACommand(0x900ab, 0, 0);
        g_areaId = kTrialTitleAreas[static_cast<std::size_t>(g_trialTitleAreaSelection)].name;
        g_trialTitleNextModule = SystemModule::Map;
        HLib_SetCurrentTaskMainCallback(TrialTitle_Exit);
    } else if (g_trialTitlePressed & 0x200U) {
        shenExecuteAICACommand(0x800ab, 0, 0);
        g_trialTitleState = 0;
        SOND_PlayCdAudio(0xf0007, 15);
    }
    OriginalStrip strip{.flags = 0x400, .corner0 = 0xffffffff, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f};
    for (std::size_t index = 0; index < kTrialAreaMenuQuads.size(); ++index) {
        const TrialTitleQuad &quad = kTrialAreaMenuQuads[index];
        const bool alternate = index < 4 || index >= 22 ? (index >= 22 ? index - 22 : index) == static_cast<std::size_t>(g_trialTitleAreaSelection) :
                                                          GetAreaJumpEnabled() && quad.alternate != 71 && quad.alternate != 69;
        float x = quad.x;
        if (index >= 4 && index < 22 && alternate) {
            if (quad.alternate == 63)
                x += 16.0f;
            else if (quad.alternate == 13)
                x -= 8.0f;
        }
        DrawTrialTitleQuad(strip, quad, alternate ? quad.alternate : quad.texture, x);
    }
}

void TrialTitle_Options() {
    g_trialTitlePreviousDialogueOption = g_trialTitleDialogueOption;
    const std::int8_t maximum = GetAreaJumpEnabled() ? 1 : 2;
    if (g_trialTitleRepeated & 0x1000U) {
        shenExecuteAICACommand(0xab, 0, 0);
        if (--g_trialTitleOptionSelection < 0)
            g_trialTitleOptionSelection = maximum;
    }
    if (g_trialTitleRepeated & 0x2000U) {
        shenExecuteAICACommand(0xab, 0, 0);
        if (++g_trialTitleOptionSelection > maximum)
            g_trialTitleOptionSelection = 0;
    }
    if (g_trialTitleRepeated & 0x4000U) {
        if (g_trialTitleOptionSelection == 0 && g_trialTitleAudioOption != 1) {
            shenExecuteAICACommand(0xab, 0, 0);
            g_trialTitleAudioOption = 1;
        } else if (g_trialTitleOptionSelection == 1 && g_trialTitleAreaJumpOption != 0) {
            shenExecuteAICACommand(0xab, 0, 0);
            g_trialTitleAreaJumpOption = 0;
        } else if (g_trialTitleOptionSelection == 2 && g_trialTitleDialogueOption > 0) {
            shenExecuteAICACommand(0xab, 0, 0);
            --g_trialTitleDialogueOption;
            if (g_trialTitleDialogueOption < 0)
                g_trialTitleDialogueOption = 0;
        }
    }
    if (g_trialTitleRepeated & 0x8000U) {
        if (g_trialTitleOptionSelection == 0 && g_trialTitleAudioOption != 0) {
            shenExecuteAICACommand(0xab, 0, 0);
            g_trialTitleAudioOption = 0;
        } else if (g_trialTitleOptionSelection == 1 && g_trialTitleAreaJumpOption != 1) {
            shenExecuteAICACommand(0xab, 0, 0);
            g_trialTitleAreaJumpOption = 1;
            g_trialTitleDialogueOption = 2;
        } else if (g_trialTitleOptionSelection == 2 && g_trialTitleDialogueOption < 3) {
            shenExecuteAICACommand(0xab, 0, 0);
            ++g_trialTitleDialogueOption;
            if (g_trialTitleDialogueOption > 3)
                g_trialTitleDialogueOption = 3;
        }
    }
    SetAreaJumpEnabled(static_cast<std::uint8_t>(g_trialTitleAreaJumpOption));
    SetDialogueMode(static_cast<std::uint8_t>(g_trialTitleDialogueOption));
    TrialTitle_DrawOptions();
    if (g_trialTitlePressed & 0x600U) {
        shenExecuteAICACommand(0x900ab, 0, 0);
        g_trialTitleState = 0;
        SOND_SetSystemOutputMode(g_trialTitleAudioOption);
        SOND_PlayCdAudio(0xf0007, 15);
    }
}

void TrialTitle_DrawOptions() {
    OriginalStrip strip{.flags = 0x1000400, .corner0 = 0xffffffff, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f};
    for (std::size_t index = 0; index < 16; ++index) {
        const TrialTitleQuad &quad = kTrialOptionsQuads[index];
        const bool alternate = GetAreaJumpEnabled() && quad.alternate != 71 && quad.alternate != 69;
        float x = quad.x;
        if (alternate && quad.alternate == 62)
            x -= 13.0f;
        else if (alternate && quad.alternate == 20)
            x += 2.0f;
        DrawTrialTitleQuad(strip, quad, alternate ? quad.alternate : quad.texture, x);
    }
    for (std::size_t index = 16; index < kTrialOptionsQuads.size(); ++index) {
        const TrialTitleQuad &quad = kTrialOptionsQuads[index];
        std::int32_t texture = quad.texture;
        if (index == 16 || index == 17)
            texture = g_trialTitleAudioOption == 1 ? quad.alternate : quad.texture;
        else if (index == 18 || index == 19)
            texture = g_trialTitleAudioOption == 0 ? quad.alternate : quad.texture;
        else if (index == 20) {
            if (GetAreaJumpEnabled() || !g_trialTitleDialogueOption)
                continue;
            texture = g_trialTitleDialogueOption < g_trialTitlePreviousDialogueOption ? 6 : 3;
        } else if (index == 22) {
            if (GetAreaJumpEnabled() || g_trialTitleDialogueOption == 3)
                continue;
            texture = g_trialTitlePreviousDialogueOption < g_trialTitleDialogueOption ? 8 : 5;
        } else if (index >= 23 && index <= 25)
            texture = kTrialDialogueGlyphs[g_trialTitleDialogueOption][index - 23];
        else if (index == 26) {
            if (g_trialTitleDialogueOption != 1)
                continue;
            texture = GetAreaJumpEnabled() ? quad.alternate : quad.texture;
        } else if (index == 27 || index == 28)
            texture = !g_trialTitleAreaJumpOption ? quad.alternate : quad.texture;
        else if (index == 29 || index == 30)
            texture = g_trialTitleAreaJumpOption == 1 ? quad.alternate : quad.texture;
        DrawTrialTitleQuad(strip, quad, texture, quad.x);
    }
    strip.vertexCount = g_trialTitleCursorColour;
    const TrialTitleQuad &row = kTrialOptionsQuads[static_cast<std::size_t>(g_trialTitleOptionSelection) + 2];
    UILayout_BuildQuad(&strip, 5, 36, static_cast<std::int32_t>(row.y + 2.0f), 32, 32, 1.5f, 1.5f, 0, 0);
    TrialTitle_SubmitStrip(&strip, g_trialSystemTitleTextures[2], -7.0f);
}

void TrialTitle_Exit(HLib::TaskData *) {
    SOND_StopCdAudio();
#if SHENMUE_IS_DC
    dc::sdk::njSetBackColor(0xff000000, 0xff000000, 0xff000000);
#else
    g_ClearColour = 0xff000000;
#endif
    HLib_UpdateTaskQueue(g_trialTitleNextModule, 0);
}

} // namespace shenmue::s1

#endif
