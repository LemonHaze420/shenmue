#include "s1/modules/harrier.hpp"
#include "s1/modules/arcade.hpp"
#include "s1/modules/harrier_cpu.hpp"
#include "s1/modules/harrier_render.hpp"
#include "s1/input.hpp"
#include "s1/task/padc.hpp"
#include "s1/ui_runtime.hpp"

#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

HarrierSession g_harrierSession{};
float g_harrierInputScaleX = 0.85f;
float g_harrierInputScaleY = -0.45f;
std::uint8_t g_harrierInputSelector = 0;
std::uint8_t &g_harrierInputMask = g_harrierMemory[kHarrierInputMaskOffset];
std::uint8_t g_harrierContinueHookSeen = 0;
std::int32_t g_harrierCoinEnabled = 1;
std::int32_t g_harrierInputOrientation = 1;
std::int32_t g_harrierDisplayCredits = 9;
std::int32_t g_harrierRemainingCredits = 9;
std::int32_t g_emu68_state = 2;
std::int32_t g_harrierAttractReady = 0;
std::int32_t g_harrierControlEnableDelay = 0;
std::uint64_t g_harrierDebugHoldCount = 0;
std::uint64_t g_harrierDebugFrameCount = 0;
std::int32_t g_harrierPlaying = 0;
std::int32_t g_harrierDebugEnabled = 0;
std::int32_t g_harrierResumeDelay = 0;
std::int32_t g_harrierStartPulse = 0;
std::int32_t g_harrierCoinPulse = 0;
std::int32_t g_harrierPauseFrames = 0;
std::int32_t g_harrierControlsActive = 0;
std::int32_t g_harrierHelpChoice = 0;
std::int32_t g_harrierPaused = 0;
std::int32_t g_harrierControllerDisconnected = 0;
std::int32_t g_harrierExitAfterGame = 0;
std::int32_t g_harrierResetRequested = 0;
std::int32_t g_harrierExitReason = 0;
std::int32_t g_harrierExitDelay = 0;
std::int32_t g_harrierPlayedFrames = 0;
std::int32_t g_harrierSkipHighScore = 0;
std::int32_t g_harrierContinuePlay = 0;
std::int32_t g_harrierHighScoreEntered = 0;
std::uint32_t g_harrierBestScore = 0;
std::int32_t g_harrierEmulatorFrameCount = 0;

void Harrier_Init() {
    g_harrierCoinEnabled = 1;
    g_harrierControlsActive = 0;
    g_harrierControlEnableDelay = 0;
    g_harrierAttractReady = 0;
    g_harrierPlaying = 0;
    g_harrierExitedEmulator = 0;
    g_harrierPauseFrames = 0;
    g_harrierPaused = 0;
    g_harrierDebugEnabled = 0;
    g_harrierControllerDisconnected = 0;
    g_harrierHelpChoice = 0;
    g_harrierStartPulse = 0;
    g_harrierCoinPulse = 0;
    g_harrierPlayedFrames = 0;
    g_harrierExitDelay = 0;
    g_harrierExitAfterGame = 0;
    g_harrierSkipHighScore = 0;
    g_harrierExitReason = 0;
    g_harrierResetRequested = 0;
    g_harrierDebugHoldCount = 0;
    g_harrierDebugFrameCount = 0;
    emu68_insn_defs[0].pattern = 1;
    g_harrierRemainingCredits = 9;
    g_harrierDisplayCredits = 9;
    g_emu68_state = 2;
    g_harrierInputOrientation = 1;
    emu68_insn_defs[0].a = 1;
}

void Harrier_UpdateStateFromController() {
    if (g_harrierCoinPulse > 0)
        --g_harrierCoinPulse;
    const InputSlot &pad = g_inputState.slots[0];
    const std::uint16_t pressed = pad.pressedPriority;
    if (g_harrierCoinEnabled && g_harrierRemainingCredits && (pressed & 0x0800U) != 0)
        g_harrierCoinPulse = 5;
    if (g_harrierStartPulse > 0)
        --g_harrierStartPulse;
    if (g_harrierAttractReady && (pressed & 0x0800U) != 0)
        g_harrierStartPulse = 5;
    if (g_harrierPlaying == 1 && (pressed & 0x0800U) != 0)
        g_harrierStartPulse = 1;
    if ((pad.held & 0x0801U) == 0x0801U) {
        const std::uint32_t count = static_cast<std::uint32_t>(g_harrierDebugHoldCount) + 1U;
        g_harrierDebugHoldCount = (g_harrierDebugHoldCount & 0xffffffff00000000ULL) | count;
        if (static_cast<std::int32_t>(count) > 180)
            g_harrierResetRequested = 1;
    } else {
        g_harrierDebugHoldCount &= 0xffffffff00000000ULL;
    }
    if (g_harrierControlEnableDelay) {
        const std::int32_t previous = g_harrierControlEnableDelay--;
        if (previous == 1)
            g_harrierControlsActive = 1;
    }
    if (g_harrierResumeDelay > 0)
        --g_harrierResumeDelay;
    if (g_harrierControlsActive) {
        if (g_harrierPaused) {
            if (g_harrierControllerDisconnected) {
                if ((pressed & 0x0800U) != 0)
                    g_harrierControllerDisconnected = 0;
            } else if ((pressed & 0x0a00U) != 0) {
                g_harrierPaused = 0;
                g_harrierResumeDelay = 10;
            }
        } else if ((pressed & 0x0800U) != 0) {
            g_harrierPaused = 1;
            g_harrierDebugEnabled = 0;
            g_harrierPauseFrames = 0;
        }
    } else {
        if (g_harrierPaused) {
            g_harrierPaused = 0;
            g_harrierResumeDelay = 10;
        }
        if (g_harrierControllerDisconnected && (pressed & 0x0800U) != 0)
            g_harrierControllerDisconnected = 0;
    }
    if (g_xinputReserved) {
        if (g_harrierPaused == 1) {
            g_previousHelpTokenId = g_currentHelpTokenId;
            UI_HELPSetEmulatorContext(true);
            g_defaultHelpTokenId = 0x41;
            g_currentHelpTokenId = 0x41;
            g_helpButtonId = 0xff;
            g_currentHelpIconState = 0;
            UI_HELPPreparePage(g_currentHelpTokenId);
            UI_HELPStartTransitionAnim();
            g_uiElements[kUiHelpNew].isActive = 1;
        }
    } else {
        g_previousHelpTokenId = g_currentHelpTokenId;
        UI_HELPSetEmulatorContext(false);
        g_defaultHelpTokenId = 0x42;
        g_currentHelpTokenId = 0x42;
        g_helpButtonId = 0xff;
        g_currentHelpIconState = 0;
        UI_HELPPreparePage(g_currentHelpTokenId);
        UI_HELPStartTransitionAnim();
        g_uiElements[kUiHelpNew].isActive = 1;
        if (!g_harrierPaused) {
            g_harrierPaused = 1;
            g_harrierDebugEnabled = 0;
            g_harrierPauseFrames = 0;
        }
    }
    if (g_emu68_state == 1) {
        if (!g_harrierRemainingCredits) {
            g_harrierExitReason = 0;
            g_harrierExitedEmulator = 1;
        }
        if (g_harrierExitAfterGame) {
            g_harrierExitReason = 1;
            g_harrierExitedEmulator = 1;
        }
    }
    if ((pressed & 2U) != 0 && (g_emu68_state == 1 || g_harrierPaused == 1)) {
        g_harrierExitReason = 0;
        g_harrierExitedEmulator = 1;
    }
    if (g_harrierResetRequested) {
        g_harrierExitReason = 2;
        g_harrierExitedEmulator = 1;
    }
    if (g_harrierExitDelay) {
        const std::int32_t previous = g_harrierExitDelay--;
        if (previous == 1)
            g_harrierExitAfterGame = 1;
    }
}

std::uint8_t Harrier_HandleInput(std::uint32_t backingAddress) {
    if (backingAddress != kHarrierInputSelectorAddress)
        return g_harrierMemory[backingAddress];
    const InputSlot &pad = g_inputState.slots[0];
    if (!g_harrierInputSelector) {
        if ((pad.held & 0x4000U) != 0)
            return static_cast<std::uint8_t>(127 - static_cast<std::int32_t>(g_harrierInputScaleX * -128.0f));
        if ((pad.held & 0x8000U) != 0)
            return static_cast<std::uint8_t>(127 - static_cast<std::int32_t>(g_harrierInputScaleX * 127.0f));
        const std::int32_t axis = static_cast<std::uint8_t>(pad.rightStick) - 128;
        return static_cast<std::uint8_t>(127 - static_cast<std::int32_t>(static_cast<float>(axis) * g_harrierInputScaleX));
    }
    if (g_harrierInputSelector != 4)
        return 0;
    if ((pad.held & 0x1000U) != 0)
        return static_cast<std::uint8_t>(127 - static_cast<std::int32_t>(g_harrierInputScaleY * 127.0f));
    if ((pad.held & 0x2000U) != 0)
        return static_cast<std::uint8_t>(127 - static_cast<std::int32_t>(g_harrierInputScaleY * -128.0f));
    const std::int32_t axis = 128 - static_cast<std::uint8_t>(pad.rightStick >> 8U);
    return static_cast<std::uint8_t>(127 - static_cast<std::int32_t>(static_cast<float>(axis) * g_harrierInputScaleY));
}

void Harrier_WriteInputSelector(std::uint32_t backingAddress, std::uint8_t value) {
    if (backingAddress == kHarrierInputSelectorAddress)
        g_harrierInputSelector = value & 0x1cU;
    else
        g_harrierMemory[backingAddress] = value;
}

void emu68_instruction_handler() {
    switch (g_emu68Instruction) {
    case 0xa000: {
        g_harrierCoinEnabled = 0;
        g_harrierAttractReady = 0;
        g_harrierPlaying = 0;
        reg68->a[7] -= 4;
        const std::uint32_t address = reg68->a[7];
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 0;
        g_harrierContinueHookSeen = 0;
        g_emu68_state = 2;
        reg68_page_map_entry_t &page = reg68->map[(address >> 16U) & 0xffU];
        if (page.mask != 0xffffffffU)
            page.write32(page.base + (address & page.mask), 4108);
        reg68->pc = 4102;
        break;
    }
    case 0xa001: {
        g_harrierCoinEnabled = 0;
        g_harrierAttractReady = 1;
        g_emu68_state = 1;
        g_harrierPlaying = 0;
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 0;
        reg68_page_map_entry_t &page = reg68->map[4];
        const std::uint8_t value = page.mask != 0xffffffffU ? page.read8(page.base + (0x40088U & page.mask)) : 0xffU;
        reg68->pc = value ? 4374 : 4314;
        break;
    }
    case 0xa002: {
        g_harrierCoinEnabled = 0;
        g_harrierAttractReady = 0;
        g_harrierPlaying = 1;
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 60;
        g_emu68_state = 0;
        if (g_harrierRemainingCredits > 0)
            --g_harrierRemainingCredits;
        g_harrierContinuePlay = 0;
        reg68_page_map_entry_t &page = reg68->map[4];
        if (page.mask != 0xffffffffU)
            page.write16(page.base + (0x40408U & page.mask), 0xffffU);
        reg68->pc = 4382;
        break;
    }
    case 0xa003:
        g_harrierAttractReady = 0;
        g_harrierCoinEnabled = 1;
        g_harrierPlaying = 0;
        reg68->pc = 5464;
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 0;
        g_emu68_state = 1;
        break;
    case 0xa004:
        g_harrierAttractReady = 0;
        g_harrierCoinEnabled = 1;
        g_harrierPlaying = 0;
        reg68->pc = 25176;
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 0;
        g_emu68_state = 1;
        break;
    case 0xa005: {
        reg68_page_map_entry_t &page = reg68->map[4];
        const std::uint16_t value = page.mask != 0xffffffffU ? page.read16(page.base + (0x40408U & page.mask)) : 0xffffU;
        if (value) {
            reg68->pc = 8298;
            g_harrierCoinEnabled = 0;
            g_harrierAttractReady = 0;
            g_harrierPlaying = 1;
            g_harrierControlsActive = 0;
            g_harrierControlEnableDelay = 0;
            g_emu68_state = 0;
        } else {
            reg68->pc = 8342;
        }
        break;
    }
    case 0xa006: {
        if (!g_harrierContinuePlay) {
            reg68_page_map_entry_t &page = reg68->map[4];
            const std::uint32_t score = page.mask != 0xffffffffU ? page.read32(page.base + (0x40484U & page.mask)) : 0xffffffffU;
            if (score > g_harrierBestScore)
                g_harrierBestScore = score;
        }
        reg68->a[0] = 26460;
        g_harrierAttractReady = 0;
        g_harrierPlaying = 0;
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 0;
        g_emu68_state = 0;
        if (g_harrierSkipHighScore) {
            g_harrierCoinEnabled = 0;
            reg68->pc = 19120;
        } else {
            reg68->pc = 26284;
            g_harrierCoinEnabled = 1;
        }
        break;
    }
    case 0xa007:
        g_harrierCoinEnabled = 0;
        g_harrierAttractReady = 1;
        g_harrierPlaying = 0;
        reg68->a[0] = 1082566;
        reg68->pc = 26394;
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 0;
        g_emu68_state = 0;
        break;
    case 0xa008:
        g_harrierCoinEnabled = 0;
        g_harrierAttractReady = 0;
        g_harrierPlaying = 1;
        g_harrierControlsActive = 1;
        g_harrierControlEnableDelay = 0;
        g_emu68_state = 0;
        if (g_harrierRemainingCredits > 0)
            --g_harrierRemainingCredits;
        g_harrierContinuePlay = 1;
        reg68->a[0] = 1082564;
        reg68->pc = 26440;
        break;
    case 0xa009:
        if (g_harrierContinueHookSeen)
            reg68->d[0] |= 0x10U;
        g_harrierContinueHookSeen = 1;
        reg68->pc = 11968;
        break;
    case 0xa100: {
        std::uint8_t *const text = g_harrierMemory + kHarrierCreditsTextOffset;
        text[0] = 'C';
        text[1] = 'R';
        text[2] = 'E';
        text[3] = 'D';
        text[4] = 'I';
        text[5] = 'T';
        text[6] = g_harrierDisplayCredits >= 2 ? 'S' : ' ';
        text[7] = ' ';
        text[8] = ' ';
        text[9] = static_cast<std::uint8_t>(g_harrierDisplayCredits + 48);
        text[10] = 0;
        reg68->pc = 24290;
        break;
    }
    case 0xa101:
        reg68->pc = g_harrierDisplayCredits < 0 ? 24268 : 24280;
        break;
    case 0xa102:
        reg68->a[0] = 1083718;
        reg68->a[1] = 5310;
        reg68->pc = 4326;
        break;
    case 0xa103:
        reg68->a[0] = 1083718;
        reg68->a[1] = 5310;
        reg68->pc = 5252;
        break;
    case 0xa104: {
        if (g_harrierDisplayCredits > 0)
            --g_harrierDisplayCredits;
        g_harrierCoinEnabled = 0;
        g_harrierAttractReady = 0;
        g_harrierPlaying = 0;
        const std::uint32_t address = reg68->a[4];
        g_harrierControlsActive = 0;
        g_harrierControlEnableDelay = 0;
        reg68_page_map_entry_t &page = reg68->map[(address >> 16U) & 0xffU];
        const std::uint16_t value = page.mask != 0xffffffffU ? page.read16(page.base + (address & page.mask)) : 0xffffU;
        if (page.mask != 0xffffffffU)
            page.write16(page.base + (address & page.mask), static_cast<std::uint16_t>(value + 1U));
        reg68->pc = 24666;
        g_harrierStartPulse = 5;
        break;
    }
    case 0xa105: {
        std::memcpy(g_harrierMemory + kHarrierRankingOffset, g_harrierSession.rankings, sizeof(g_harrierSession.rankings));
        reg68_page_map_entry_t &page = reg68->map[4];
        if (page.mask != 0xffffffffU)
            page.write32(page.base + (0x4003cU & page.mask), 0x99999999U);
        reg68->pc = 8076;
        break;
    }
    case 0xa106:
        if (!g_harrierContinuePlay)
            g_harrierHighScoreEntered = 1;
        reg68->d[0] = (reg68->d[0] & 0xffff0000U) | 183U;
        reg68->pc = 36754;
        break;
    case 0xa107:
        reg68->pc = 19184;
        break;
    case 0xa108: {
        std::uint16_t value = static_cast<std::uint16_t>(reg68->d[7] + 1U);
        if (value < 7)
            value = 7;
        reg68->d[6] = value;
        reg68->pc = 8428;
        break;
    }
    case 0xa109: {
        const std::uint16_t value = static_cast<std::uint16_t>(reg68->d[7]);
        reg68->d[6] = (reg68->d[6] & 0xffff0000U) | (value >= 6 ? static_cast<std::uint16_t>(value - 6U) : 0U);
        reg68_page_map_entry_t &page = reg68->map[4];
        if (page.mask != 0xffffffffU)
            page.write8(page.base + (0x40460U & page.mask), 48);
        reg68->pc = 8440;
        break;
    }
    case 0xa10a: {
        const std::uint16_t value = static_cast<std::uint16_t>(reg68->d[7]);
        reg68->d[6] = (reg68->d[6] & 0xffff0000U) | (value >= 6 ? static_cast<std::uint16_t>(value - 6U) : 0U);
        reg68->pc = 8490;
        break;
    }
    default: {
        char text[80]{};
        std::snprintf(text, sizeof(text), "error axxx\n");
        Harrier_DebugPrint(text);
        std::snprintf(text, sizeof(text), "CPUID:%d PC=%08X\n", static_cast<int>(reg68->cpu_id), reg68->pc);
        Harrier_DebugPrint(text);
        break;
    }
    }
}

void HarrierExecute() {
    Harrier_UpdateStateFromController();
    if (!g_harrierPaused && !g_harrierResumeDelay) {
        std::uint8_t mask = g_harrierCoinPulse ? 0xfeU : 0xffU;
        g_harrierInputMask = mask;
        if (g_harrierStartPulse) {
            mask = static_cast<std::uint8_t>(mask - 16U);
            g_harrierInputMask = mask;
        }
        const std::uint16_t held = g_inputState.slots[0].held;
        if ((held & 0x0600U) != 0) {
            mask = static_cast<std::uint8_t>(mask - 32U);
            g_harrierInputMask = mask;
        }
        if ((held & 0x0006U) != 0) {
            mask = static_cast<std::uint8_t>(mask - 64U);
            g_harrierInputMask = mask;
        }
        if ((held & 0x0080U) != 0)
            g_harrierInputMask = static_cast<std::uint8_t>(mask + 128U);
        emu68_Run();
        if (g_harrierControlsActive) {
            ++g_harrierPlayedFrames;
            g_forceXInputAnalogProcessing = 1;
        }
    }
    DoAudio(g_harrierPaused != 0);
    AudioFlushFromArchivesAndPlay();
    if (!g_harrierPaused)
        Harrier_UpdateAudioCommands();
    if (!g_harrierEmulatorFrameCount) {
        CreateAndDrawEmu();
        HarrierControllerRelated();
    }
}

void HarrierDestroy() {
    std::memcpy(g_harrierSession.rankings, g_harrierMemory + kHarrierRankingOffset, sizeof(g_harrierSession.rankings));
    Harrier_ReleaseRenderResources();
    Harrier_ReleaseSoundEffects();
    CleanupAudioSources();
}

} // namespace shenmue::s1
