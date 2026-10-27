#include "s2/areas/game/afterburner/board.hpp"
#include "s2/areas/game/afterburner/render.hpp"
#include "s2/areas/game/afterburner/sound.hpp"
#include "s2/areas/game/arcade/math.hpp"
#include "s2/input.hpp"
#include "s2/ui_transition.hpp"
#include "s1/archive_data.hpp"
#include "s1/modules/harrier.hpp"
#include "s1/modules/harrier_cpu.hpp"
#include "s1/modules/harrier_render.hpp"
#include <bit>
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {
std::int32_t g_afterBurnerInputMode = 5;
namespace {
GameArcadeRuntime *boardRuntime{};
const std::uint8_t *boardRankings{};
std::array<s1::reg68_page_map_entry_t, 256> primaryPages{}, secondaryPages{};
std::array<ArcadeMathDevice, 2> mathDevices{};
std::uint8_t firstCreditSeen{}, adcSelector{}, roadBank{}, spriteBank{};
std::int32_t reversalTransition{}, idleTransition{}, integrateX{}, integrateY{}, compareX{}, compareY{};
float scaleZ{}, idleDuration{}, reversalWindow{}, stepX{}, stepY{}, integratedX{}, integratedY{}, lastExtreme{};

void AfterBurnerLocalDebugNullsub() {
}
std::uint8_t ReadByte(std::uint32_t address) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 255];
    return page.mask == 0xffffffffU ? 0xff : page.read8(page.base + (address & page.mask));
}
std::uint16_t ReadWord(std::uint32_t address) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 255];
    return page.mask == 0xffffffffU ? 0xffff : page.read16(page.base + (address & page.mask));
}
std::uint32_t ReadLong(std::uint32_t address) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 255];
    return page.mask == 0xffffffffU ? 0xffffffffU : page.read32(page.base + (address & page.mask));
}
void WriteByte(std::uint32_t address, std::uint8_t value) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 255];
    if (page.mask != 0xffffffffU)
        page.write8(page.base + (address & page.mask), value);
}
void WriteWord(std::uint32_t address, std::uint16_t value) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 255];
    if (page.mask != 0xffffffffU)
        page.write16(page.base + (address & page.mask), value);
}
void WriteLong(std::uint32_t address, std::uint32_t value) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 255];
    if (page.mask != 0xffffffffU)
        page.write32(page.base + (address & page.mask), value);
}
void PushReturn(std::uint32_t address) {
    s1::reg68->a[7] -= 4;
    WriteLong(s1::reg68->a[7], address);
}
void SetState(std::int32_t coin, std::int32_t attract, std::int32_t playing, std::int32_t delay, std::int32_t core) {
    boardRuntime->coinEnabled = coin;
    boardRuntime->attractReady = attract;
    boardRuntime->playing = playing;
    boardRuntime->controlsActive = 0;
    boardRuntime->controlEnableDelay = delay;
    boardRuntime->coreState = core;
}
void UpdateBestScore() {
    const std::uint32_t score = ReadLong(0xffff8884);
    if (score > boardRuntime->bestScore)
        boardRuntime->bestScore = score;
}
void HandleAfterBurnerOpcode() {
    GameArcadeRuntime &runtime = *boardRuntime;
    switch (s1::g_emu68Instruction) {
    case 0xa000:
        SetState(0, 0, 0, 0, 2);
        firstCreditSeen = 0;
        WriteByte(0x60892, 0x80);
        s1::reg68->a[6] = 933888;
        s1::reg68->pc = 83112;
        break;
    case 0xa001:
        SetState(0, 0, 0, 0, 1);
        WriteByte(0xffff8902, 0xff);
        s1::reg68->pc = 53620;
        break;
    case 0xa002: {
        SetState(1, 0, 0, 0, 1);
        std::uint16_t credits = ReadWord(0x29c09c) + 1;
        s1::reg68->pc = 53780;
        if (!firstCreditSeen) {
            credits = 10;
            firstCreditSeen = 1;
            WriteLong(0x122e00, 0);
            WriteLong(0x122e40, 0);
            WriteWord(0x29c0aa, 0);
            runtime.displaySuppressed = 0;
        }
        WriteWord(0x29c09c, credits);
        break;
    }
    case 0xa003:
        SetState(0, 1, 0, 0, 1);
        runtime.continuePlay = 0;
        WriteWord(0x29c300, 15);
        s1::reg68->pc = 54692;
        break;
    case 0xa004:
        SetState(0, 0, 1, 10, 0);
        WriteByte(0xffff8b6d, ReadByte(0xffff8b6d) & 0xfb);
        s1::reg68->pc = 55090;
        break;
    case 0xa005:
        SetState(1, 0, 0, 0, 0);
        if (!runtime.continuePlay)
            UpdateBestScore();
        s1::reg68->a[0] = 206142;
        s1::reg68->pc = 60654;
        break;
    case 0xa006:
        SetState(0, 0, 0, 0, 0);
        WriteByte(0xffff8c51, ReadByte(0xffff8c51) | 1);
        s1::reg68->pc = 61040;
        break;
    case 0xa007:
    case 0xa008:
        SetState(0, 0, 1, 0, 0);
        runtime.controlsActive = 1;
        runtime.continuePlay = 1;
        WriteByte(0xffff8c51, ReadByte(0xffff8c51) | 1);
        s1::reg68->pc = s1::g_emu68Instruction == 0xa007 ? 61356 : 61524;
        break;
    case 0xa009:
        SetState(0, 0, 1, 0, 0);
        WriteByte(0xffff91d0, 0xff);
        s1::reg68->pc = 56170;
        break;
    case 0xa100:
        s1::reg68->a[0] = 178192;
        s1::reg68->pc = 58294;
        break;
    case 0xa101:
        s1::reg68->a[0] = 178218;
        s1::reg68->pc = 58308;
        break;
    case 0xa102:
        if (runtime.displayCredits >= 0) {
            s1::reg68->a[0] = runtime.displayCredits > 1 ? 855358 : 855356;
            s1::reg68->d[0] = (s1::reg68->d[0] & 0xffff0000U) | (runtime.displayCredits & 0xffff);
            PushReturn(runtime.displayCredits > 1 ? 50748 : 50734);
            s1::reg68->pc = 57988;
        } else
            s1::reg68->pc = 50772;
        break;
    case 0xa103:
        s1::reg68->pc = 50688;
        break;
    case 0xa104:
        if (runtime.displayCredits > 0)
            --runtime.displayCredits;
        runtime.coinEnabled = 0;
        runtime.attractReady = 0;
        runtime.playing = 0;
        runtime.controlsActive = 0;
        runtime.controlEnableDelay = 0;
        WriteWord(s1::reg68->a[4], ReadWord(s1::reg68->a[4]) + 1);
        s1::reg68->pc = 51056;
        break;
    case 0xa105:
        if (runtime.credits > 0)
            --runtime.credits;
        runtime.coreState = 0;
        WriteByte(0xffff8055, 0);
        s1::reg68->pc = 50644;
        break;
    case 0xa106:
        if (!runtime.continuePlay) {
            runtime.completion = 1;
            UpdateBestScore();
        }
        WriteByte(0xffff91f0, 0xff);
        s1::reg68->pc = 55964;
        break;
    case 0xa107:
        std::memcpy(s1::g_harrierMemory + 0x8046e, boardRankings, 240);
        s1::reg68->pc = ReadLong(s1::reg68->a[7]);
        s1::reg68->a[7] += 4;
        break;
    case 0xa108: {
        const std::uint16_t score = static_cast<std::uint16_t>(s1::reg68->d[0]);
        const std::uint32_t address = s1::reg68->a[0];
        WriteWord(address, score < 10 ? 32 : static_cast<std::uint16_t>(0x8830 + (score % 100) / 10));
        WriteWord(address + 2, static_cast<std::uint16_t>(0x8830 + score % 10));
        s1::reg68->pc += 2;
        break;
    }
    default: {
        s1::Harrier_DebugPrint("error axxx\n");
        char text[64];
        std::snprintf(text, sizeof(text), "CPUID:%d PC=%08X\n", static_cast<int>(s1::reg68->cpu_id), s1::reg68->pc);
        s1::Harrier_DebugPrint(text);
        break;
    }
    }
}
void WriteSound(std::uint32_t address, std::uint8_t value) {
    if (address == 0x8017)
        QueueAfterBurnerSoundCommand(value);
    else
        s1::Emu68_IgnoreWrite8(address + 0xe0000, value);
}
std::uint16_t ReadPrimaryMathWord(std::uint32_t address) {
    return static_cast<std::uint16_t>(ReadArcadeMathValue(mathDevices[0], address));
}
void WritePrimaryMathWord(std::uint32_t address, std::uint16_t value) {
    WriteArcadeMathWord(mathDevices[0], address, value);
}
std::uint32_t ReadPrimaryMathLong(std::uint32_t address) {
    const std::uint32_t low = ReadArcadeMathValue(mathDevices[0], address + 2);
    return low | (ReadArcadeMathValue(mathDevices[0], address) << 16);
}
void WritePrimaryMathLong(std::uint32_t address, std::uint32_t value) {
    WriteArcadeMathWord(mathDevices[0], address, static_cast<std::uint16_t>(value >> 16));
    WriteArcadeMathWord(mathDevices[0], address + 2, static_cast<std::uint16_t>(value));
}
void WriteSpriteByte(std::uint32_t address, std::uint8_t value) {
    if (address == 1)
        spriteBank = (spriteBank - 1) & 1;
    else
        s1::Emu68_IgnoreWrite8(address + 0x110000, value);
}
void WriteSpriteWord(std::uint32_t address, std::uint16_t value) {
    if (!address)
        spriteBank = (spriteBank - 1) & 1;
    else
        s1::Emu68_IgnoreWrite16(address + 0x110000, value);
}
std::uint8_t ReadSecondaryByte(std::uint32_t address) {
    if ((address & 0xfffff000U) == 0xc000)
        return s1::g_harrierMemory[0xe0000 + (address & 0xfff)];
    if (address == 0xe001) {
        roadBank = (roadBank - 1) & 1;
        SnapshotAfterBurnerRoad();
        return 0xff;
    }
    return s1::Emu68_UnmappedRead8(address + 0xe0000);
}
void WriteSecondaryByte(std::uint32_t address, std::uint8_t value) {
    if ((address & 0xfffff000U) == 0xc000)
        s1::g_harrierMemory[0xe0000 + (address & 0xfff)] = value;
    else if (address == 0xe001)
        g_arcadeRoadControl = value;
    else
        s1::Emu68_IgnoreWrite8(address + 0xe0000, value);
}
std::uint32_t ReadSecondaryMathValue(std::uint32_t address) {
    if ((address & 0xfffff000U) == 0xc000)
        return s1::Harrier_ReadMemory16BigEndian(0xe0000 + (address & 0xfff));
    return ReadArcadeMathValue(mathDevices[1], address);
}
std::uint16_t ReadSecondaryWord(std::uint32_t address) {
    return static_cast<std::uint16_t>(ReadSecondaryMathValue(address));
}
void WriteSecondaryWord(std::uint32_t address, std::uint16_t value) {
    if ((address & 0xfffff000U) == 0xc000)
        s1::Harrier_WriteMemory16BigEndian(0xe0000 + (address & 0xfff), value);
    else
        WriteArcadeMathWord(mathDevices[1], address, value);
}
std::uint32_t ReadSecondaryLong(std::uint32_t address) {
    if ((address & 0xfffff000U) == 0xc000)
        return s1::Harrier_ReadMemory32BigEndian(0xe0000 + (address & 0xfff));
    const std::uint32_t low = ReadSecondaryMathValue(address + 2);
    return low | (ReadSecondaryMathValue(address) << 16);
}
void WriteSecondaryLong(std::uint32_t address, std::uint32_t value) {
    if ((address & 0xfffff000U) == 0xc000)
        s1::Harrier_WriteMemory32BigEndian(0xe0000 + (address & 0xfff), value);
    else {
        WriteSecondaryWord(address, static_cast<std::uint16_t>(value >> 16));
        WriteSecondaryWord(address + 2, static_cast<std::uint16_t>(value));
    }
}
std::uint8_t ReadAnalog(std::uint32_t address) {
    if (address != 1)
        return s1::Emu68_UnmappedRead8(address + 0x130000);
    const LegacyInputSlot &pad = g_inputBuffer[0];
    switch (adcSelector) {
    case 0:
        return static_cast<std::uint8_t>(static_cast<std::int32_t>((pad.analog0e - 128) * s1::g_harrierInputScaleX) + 128);
    case 4:
        return static_cast<std::uint8_t>(128 - static_cast<std::int32_t>((pad.analog0f - 128) * s1::g_harrierInputScaleY));
    case 8:
        return static_cast<std::uint8_t>(static_cast<std::int32_t>((pad.analog0d - 128) * scaleZ) + 128);
    default:
        return 0;
    }
}
void WriteAnalog(std::uint32_t address, std::uint8_t value) {
    if (address == 1)
        adcSelector = value & 0x1c;
    else
        s1::Emu68_IgnoreWrite8(address + 0x130000, value);
}
void UpdatePeripheralAudio() {
    if (!(s1::g_harrierMemory[0xd0005] & 1))
        MuteAfterBurnerPeripheralAudio();
}
void WritePeripheralByte(std::uint32_t address, std::uint8_t value) {
    s1::Harrier_WriteMemory8(address, value);
    if ((address & 15) == 5)
        UpdatePeripheralAudio();
}
void WritePeripheralWord(std::uint32_t address, std::uint16_t value) {
    s1::Harrier_WriteMemory16BigEndian(address, value);
    if ((address & 14) == 4)
        UpdatePeripheralAudio();
}
void WritePeripheralLong(std::uint32_t address, std::uint32_t value) {
    s1::Harrier_WriteMemory32BigEndian(address, value);
    if ((((address & 14) - 2) & ~2U) == 0)
        UpdatePeripheralAudio();
}
s1::reg68_page_map_entry_t RamPage(std::uint32_t base, std::uint32_t mask) {
    return {.read8 = s1::Harrier_ReadMemory8,
        .write8 = s1::Harrier_WriteMemory8,
        .read16 = s1::Harrier_ReadMemory16BigEndian,
        .write16 = s1::Harrier_WriteMemory16BigEndian,
        .read32 = s1::Harrier_ReadMemory32BigEndian,
        .write32 = s1::Harrier_WriteMemory32BigEndian,
        .base = base,
        .mask = mask};
}
s1::reg68_page_map_entry_t RomPage(std::uint32_t base) {
    return {.read8 = s1::Harrier_ReadMemory8,
        .write8 = s1::Emu68_IgnoreWrite8,
        .read16 = s1::Harrier_ReadMemory16BigEndian,
        .write16 = s1::Emu68_IgnoreWrite16,
        .read32 = s1::Harrier_ReadMemory32BigEndian,
        .write32 = s1::Emu68_IgnoreWrite32,
        .base = base,
        .mask = 0xffff};
}
void BuildPageMaps() {
    const s1::reg68_page_map_entry_t empty{.read8 = s1::Emu68_UnmappedRead8,
        .write8 = s1::Emu68_IgnoreWrite8,
        .read16 = s1::Emu68_UnmappedRead16,
        .write16 = s1::Emu68_IgnoreWrite16,
        .read32 = s1::Emu68_UnmappedRead32,
        .write32 = s1::Emu68_IgnoreWrite32,
        .base = 0,
        .mask = 0xffffffffU};
    for (std::size_t page = 0; page < 256; ++page) {
        primaryPages[page] = empty;
        secondaryPages[page] = empty;
    }
    for (std::uint32_t page = 0; page < 4; ++page) {
        primaryPages[page] = RomPage(page * 0x10000);
        primaryPages[page + 4] = RomPage(page * 0x10000);
        primaryPages[page + 0x20] = RomPage((page + 4) * 0x10000);
        primaryPages[page + 0x24] = RomPage((page + 4) * 0x10000);
        secondaryPages[page] = RomPage((page + 4) * 0x10000);
    }
    primaryPages[0xc] = RamPage(0x90000, 0xffff);
    primaryPages[0xd] = RamPage(0xa0000, 0xfff);
    primaryPages[0xe] = empty;
    primaryPages[0xe].mask = 0xffff;
    primaryPages[0xe].write8 = WriteSound;
    primaryPages[0xe].read16 = ReadPrimaryMathWord;
    primaryPages[0xe].write16 = WritePrimaryMathWord;
    primaryPages[0xe].read32 = ReadPrimaryMathLong;
    primaryPages[0xe].write32 = WritePrimaryMathLong;
    primaryPages[0x10] = RamPage(0xc0000, 0x1fff);
    primaryPages[0x11] = empty;
    primaryPages[0x11].mask = 0xffff;
    primaryPages[0x11].write8 = WriteSpriteByte;
    primaryPages[0x11].write16 = WriteSpriteWord;
    primaryPages[0x12] = RamPage(0xb0000, 0x3fff);
    primaryPages[0x13] = empty;
    primaryPages[0x13].mask = 0xffff;
    primaryPages[0x13].read8 = ReadAnalog;
    primaryPages[0x13].write8 = WriteAnalog;
    primaryPages[0x14] = RamPage(0xd0000, 15);
    primaryPages[0x14].write8 = WritePeripheralByte;
    primaryPages[0x14].write16 = WritePeripheralWord;
    primaryPages[0x14].write32 = WritePeripheralLong;
    primaryPages[0x15] = RamPage(0xd8000, 15);
    primaryPages[0x29] = RamPage(0x88000, 0x3fff);
    primaryPages[0x2a] = RamPage(0x8c000, 0x3fff);
    primaryPages[0x2e] = {.read8 = ReadSecondaryByte,
        .write8 = WriteSecondaryByte,
        .read16 = ReadSecondaryWord,
        .write16 = WriteSecondaryWord,
        .read32 = ReadSecondaryLong,
        .write32 = WriteSecondaryLong,
        .base = 0,
        .mask = 0xffff};
    primaryPages[0xff] = RamPage(0x80000, 0x7fff);
    secondaryPages[9] = primaryPages[0x29];
    secondaryPages[0xa] = primaryPages[0x2a];
    secondaryPages[0xe] = primaryPages[0x2e];
    secondaryPages[0x29] = primaryPages[0x29];
    secondaryPages[0x2a] = primaryPages[0x2a];
}
void ConfigureInput() {
    switch (g_afterBurnerInputMode) {
    case 0:
        idleDuration = 0;
        integrateY = 0;
        stepY = 0;
        compareY = 0;
        stepX = 0;
        reversalWindow = 0;
        integrateX = 0;
        compareX = 0;
        break;
    case 1:
        reversalWindow = .34999999f;
        idleDuration = 5;
        stepX = std::bit_cast<float>(1038174126U);
        stepY = std::bit_cast<float>(1025758986U);
        compareX = 0;
        compareY = 0;
        integrateX = 1;
        integrateY = 1;
        break;
    case 2:
    case 4:
        reversalWindow = .175f;
        idleDuration = g_afterBurnerInputMode == 2 ? 1.5f : .0099999998f;
        stepX = std::bit_cast<float>(1037503037U);
        stepY = std::bit_cast<float>(1029114429U);
        compareX = 0;
        compareY = 0;
        integrateX = 1;
        integrateY = 1;
        break;
    case 3:
        reversalWindow = .175f;
        idleDuration = .02f;
        stepX = std::bit_cast<float>(1037503037U);
        stepY = std::bit_cast<float>(1029114429U);
        integrateY = 1;
        compareY = 0;
        integrateX = 0;
        compareX = 0;
        break;
    case 5:
        idleDuration = .055f;
        stepY = std::bit_cast<float>(1029114429U);
        integrateY = 1;
        compareY = 1;
        stepX = 0;
        reversalWindow = 0;
        integrateX = 0;
        compareX = 0;
        break;
    }
}
float ClampAxis(float value) {
    if (value < -1)
        return -1;
    if (value > 1)
        return 1;
    return value;
}
std::uint8_t EncodeAxis(float value) {
    if (value > .99000001f)
        return 0xff;
    if (value < -.99000001f)
        return 0;
    return static_cast<std::uint8_t>(static_cast<std::int32_t>((value + 1) * 128));
}
void UpdateInput() {
    LegacyInputSlot &pad = g_inputBuffer[0];
    if (pad.held & 0x1000)
        pad.analog0f = 0;
    if (pad.held & 0x2000)
        pad.analog0f = 0xff;
    if (pad.held & 0x8000)
        pad.analog0e = 0xff;
    if (pad.held & 0x4000)
        pad.analog0e = 0;
    if ((pad.held & 0x4000) || pad.analog0e != 128 || pad.analog0f != 128) {
        RestartUITransition(idleTransition);
        SetUITransitionValue(idleTransition, 0);
    } else if (!IsUITransitionActive(idleTransition)) {
        if (GetUITransitionValue(idleTransition) >= idleDuration) {
            integratedX = 0;
            integratedY = 0;
            lastExtreme = 0;
        }
        UIAnim_Start(idleTransition, 0, idleDuration, idleDuration, 0);
    }
    if (integrateX != 1 && integrateY != 1)
        return;
    const float x = pad.analog0e * .0078125f - 1, y = pad.analog0f * .0078125f - 1;
    if (!compareX || (compareX == 1 && (integratedX <= x - .0099999998f || x + .0099999998f <= integratedX)))
        integratedX = ClampAxis(integratedX + stepX * x);
    if (!compareY || (compareY == 1 && (integratedY <= y - .0099999998f || y + .0099999998f <= integratedY)))
        integratedY = ClampAxis(integratedY + stepY * y);
    if (integrateX == 1) {
        if (GetUITransitionValue(reversalTransition) >= reversalWindow && !IsUITransitionActive(reversalTransition))
            lastExtreme = 0;
        if (x > .99000001f || x < -.99000001f) {
            if (lastExtreme < -.99000001f && x > .99000001f) {
                integratedX = 1;
                lastExtreme = 0;
            } else if (lastExtreme > .99000001f && x < -.99000001f) {
                integratedX = -1;
                lastExtreme = 0;
            } else {
                UIAnim_Start(reversalTransition, 0, reversalWindow, reversalWindow, 0);
                lastExtreme = x;
            }
        }
        pad.analog0e = EncodeAxis(integratedX);
    }
    if (integrateY == 1)
        pad.analog0f = EncodeAxis(integratedY);
}
constexpr std::int32_t patches[]{
    83106,
    0xa000,
    -1,
    53614,
    0xa001,
    -1,
    53774,
    0xa002,
    -1,
    54684,
    0xa003,
    -1,
    55084,
    0xa004,
    -1,
    60648,
    0xa005,
    -1,
    61034,
    0xa006,
    -1,
    61350,
    0xa007,
    -1,
    61518,
    0xa008,
    -1,
    56164,
    0xa009,
    -1,
    58288,
    0xa100,
    -1,
    58302,
    0xa101,
    -1,
    50688,
    0xa102,
    -1,
    52940,
    0xa103,
    -1,
    51054,
    0xa104,
    -1,
    50640,
    0xa105,
    -1,
    55958,
    0xa106,
    -1,
    94600,
    0xa107,
    -1,
    56770,
    0xa108,
    -1,
    55372,
    0xa108,
    -1,
    50700,
    0xd3c,
    -1,
    50786,
    0xd38,
    -1,
    50800,
    0xd38,
    -1,
    50836,
    0xd38,
    -1,
    178246,
    0xd66,
    -1,
    53104,
    0xd38,
    -1,
    100228,
    0xcb4,
    -1,
    100528,
    0xcb4,
    -1,
    100468,
    0x1cc0,
    -1,
    206188,
    0xcd4,
    -1,
    55366,
    0xcdc,
    -1,
    56760,
    0xcdc,
    -1,
    -1,
};
static_assert(sizeof(patches) == 97 * sizeof(std::int32_t));
} // namespace

void InitializeAfterBurnerBoard(GameArcadeRuntime &runtime, const std::uint8_t *rankings) {
    boardRuntime = &runtime;
    boardRankings = rankings;
    ConfigureInput();
    reversalTransition = UIAnimAllocSlot();
    idleTransition = UIAnimAllocSlot();
    integratedX = 0;
    integratedY = 0;
    lastExtreme = 0;
    s1::ArchiveData_Read("xab/datas/AB.ROM", 0, s1::g_harrierMemory, 0x80000);
    s1::g_emu68OpcodeDispatch[0xa] = HandleAfterBurnerOpcode;
    InitializeAfterBurnerGraphics();
    InitializeAfterBurnerSound();
    s1::g_harrierInputScaleX = .69999999f;
    s1::g_harrierInputScaleY = -.64999998f;
    scaleZ = 1;
    s1::g_harrierMemory[0xd8007] = runtime.skipHighScore ? 0xed : 0xcd;
    s1::Harrier_WriteMemory16BigEndian(0xd8005, 0xffff);
    s1::g_harrierIrqLevel2 = 2;
    s1::g_harrierIrqLevel4 = 4;
    s1::g_harrierIrqFrame = 0;
    BuildPageMaps();
    s1::g_reg68_ctx.cpu_id = s1::REG68_CPU_0;
    s1::g_reg68_ctx_2.cpu_id = s1::REG68_CPU_1;
    s1::g_reg68_ctx.map = primaryPages.data();
    s1::g_reg68_ctx_2.map = secondaryPages.data();
    s1::reg68 = &s1::g_reg68_ctx;
    s1::Emu68_ResetContext();
    s1::reg68 = &s1::g_reg68_ctx_2;
    s1::Emu68_ResetContext();
    s1::reg68 = &s1::g_reg68_ctx;
    for (std::uint32_t record = 0; s1::emu68_insn_defs[record].asm_string; ++record) {
        s1::emu68_ctx_t &target = s1::emu68_insn_defs[record + 1];
        target.a = 0;
        target.pattern = 0;
        for (std::uint32_t bit = 0; bit < 16; ++bit) {
            const char value = s1::emu68_insn_defs[record].bits[bit];
            if (value == '1')
                target.a |= 1 << (15 - bit);
            if (value == '0' || value == '1')
                target.pattern |= 1 << (15 - bit);
        }
    }
    const std::int32_t *patch = patches;
    while (*patch != -1) {
        std::uint32_t destination = static_cast<std::uint32_t>(*patch++);
        while (*patch != -1) {
            s1::Harrier_WriteMemory16BigEndian(destination, static_cast<std::uint16_t>(*patch++));
            destination += 2;
        }
        ++patch;
    }
    AfterBurnerLocalDebugNullsub();
}
void UpdateAfterBurnerBoard(GameArcadeRuntime &runtime) {
    boardRuntime = &runtime;
    if (runtime.paused || runtime.resumeDelay)
        return;
    std::uint8_t input = runtime.coinPulse ? 0x7f : 0xff;
    s1::g_harrierMemory[0xd8001] = input;
    if (runtime.startPulse) {
        input -= 8;
        s1::g_harrierMemory[0xd8001] = input;
    }
    if (g_inputBuffer[0].held & 0x480) {
        input -= 16;
        s1::g_harrierMemory[0xd8001] = input;
    }
    if (g_inputBuffer[0].held & 0x206)
        s1::g_harrierMemory[0xd8001] = input - 32;
    UpdateInput();
    s1::emu68_Run();
    if (runtime.controlsActive)
        ++runtime.playedFrames;
}
void CopyAfterBurnerRankings(std::uint8_t *rankings) {
    std::memcpy(rankings, s1::g_harrierMemory + 0x8046e, 240);
}
} // namespace shenmue::s2
