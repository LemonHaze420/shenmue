#include "s2/areas/game/outrun/board.hpp"
#include "s2/areas/game/outrun/render.hpp"
#include "s2/areas/game/outrun/sound.hpp"

#include "s1/archive_data.hpp"
#include "s1/input.hpp"
#include "s1/modules/harrier.hpp"
#include "s1/modules/harrier_cpu.hpp"

#include <cstring>

namespace shenmue::s2 {

std::array<OutRunSoundCommand, 32> g_outRunSoundCommands{};
std::uint32_t g_outRunSoundCommandCount{};
std::uint8_t g_outRunRoadBank{};
std::uint8_t g_outRunFlipBank{};
std::array<std::uint8_t, 0x800> g_outRunRoadSnapshot{};
std::uint32_t g_outRunRoadOffset0{}, g_outRunRoadOffset1{};
std::int32_t g_outRunGear{};

namespace {

GameArcadeRuntime *boardRuntime{};
const std::uint8_t *boardRankings{};
std::uint8_t adcSelector{};
std::int32_t firstCreditSeen{};
std::array<s1::reg68_page_map_entry_t, 256> primaryPages{}, secondaryPages{};

std::uint8_t ReadRoadControl(std::uint32_t address) {
    if (address == 1) {
        g_outRunRoadBank = (g_outRunRoadBank - 1) & 1;
        return 0;
    }
    return s1::Emu68_UnmappedRead8(address + 0x90000);
}
void WriteRoadControl(std::uint32_t address, std::uint8_t value) {
    if (address == 1)
        g_arcadeRoadControl = value & 3;
    else
        s1::Emu68_IgnoreWrite8(address + 0x90000, value);
}
std::uint8_t ReadSoundByte(std::uint32_t address) {
    return s1::Harrier_ReadMemory8(address + 0xf0000);
}
std::uint16_t ReadSoundWord(std::uint32_t address) {
    return s1::Harrier_ReadMemory16BigEndian(address + 0xf0000);
}
std::uint32_t ReadSoundLong(std::uint32_t address) {
    return s1::Harrier_ReadMemory32BigEndian(address + 0xf0000);
}
void WriteSoundByte(std::uint32_t address, std::uint8_t value) {
    if (address != 7) {
        s1::Harrier_WriteMemory8(address + 0xf0000, value);
    } else if (g_outRunSoundCommandCount != 32) {
        g_outRunSoundCommands[g_outRunSoundCommandCount].type = s1::g_harrierMemory[0x80909] & 7;
        ++g_outRunSoundCommandCount;
        g_outRunSoundCommands[g_outRunSoundCommandCount - 1].value = value;
    }
}
void WriteSoundWord(std::uint32_t address, std::uint16_t value) {
    s1::Harrier_WriteMemory16BigEndian(address + 0xf0000, value);
}
void WriteSoundLong(std::uint32_t address, std::uint32_t value) {
    s1::Harrier_WriteMemory32BigEndian(address + 0xf0000, value);
}
std::uint8_t ReadAnalog(std::uint32_t address) {
    if (address != 0xd0031)
        return s1::Harrier_ReadMemory8(address);
    if (adcSelector == 4)
        return static_cast<std::uint8_t>(s1::g_inputState.slots[0].triggers);
    if (adcSelector == 8)
        return static_cast<std::uint8_t>(s1::g_inputState.slots[0].triggers >> 8);
    if (adcSelector != 0)
        return 0;
    const std::uint32_t held = s1::g_inputState.slots[0].held;
    float value;
    if (held & 0x4000)
        value = -128.0f * s1::g_harrierInputScaleX;
    else if (held & 0x8000)
        value = 128.0f * s1::g_harrierInputScaleX;
    else
        value = (static_cast<std::int32_t>(s1::g_inputState.slots[0].rightStick & 0xff) - 128) * s1::g_harrierInputScaleX;
    std::int32_t result = static_cast<std::int32_t>(value);
    if (result < 0) {
        result += 16;
        if (result > 0)
            result = 0;
    } else {
        result -= 16;
        if (result < 0)
            result = 0;
    }
    return static_cast<std::uint8_t>(result + 128);
}
void WriteAnalog(std::uint32_t address, std::uint8_t value) {
    if (address == 0xd0031)
        adcSelector = value & 0xc;
    else if (address == 0xd0071)
        g_outRunFlipBank = (g_outRunFlipBank - 1) & 1;
    else
        s1::Harrier_WriteMemory8(address, value);
}
std::uint8_t ReadMappedByte(std::uint32_t address) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 0xff];
    return page.mask == 0xffffffffu ? 0xff : page.read8(page.base + (address & page.mask));
}
std::uint16_t ReadMappedWord(std::uint32_t address) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 0xff];
    return page.mask == 0xffffffffu ? 0xffff : page.read16(page.base + (address & page.mask));
}
std::uint32_t ReadMappedLong(std::uint32_t address) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 0xff];
    return page.mask == 0xffffffffu ? 0xffffffffu : page.read32(page.base + (address & page.mask));
}
void WriteMappedByte(std::uint32_t address, std::uint8_t value) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 0xff];
    if (page.mask != 0xffffffffu)
        page.write8(page.base + (address & page.mask), value);
}
void WriteMappedWord(std::uint32_t address, std::uint16_t value) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 0xff];
    if (page.mask != 0xffffffffu)
        page.write16(page.base + (address & page.mask), value);
}
void WriteMappedLong(std::uint32_t address, std::uint32_t value) {
    const s1::reg68_page_map_entry_t &page = s1::reg68->map[(address >> 16) & 0xff];
    if (page.mask != 0xffffffffu)
        page.write32(page.base + (address & page.mask), value);
}
void PushReturn(std::uint32_t address) {
    s1::reg68->a[7] -= 4;
    WriteMappedLong(s1::reg68->a[7], address);
}
void SetState(std::int32_t coin, std::int32_t attract, std::int32_t playing, std::int32_t delay, std::int32_t core) {
    boardRuntime->coinEnabled = coin;
    boardRuntime->attractReady = attract;
    boardRuntime->playing = playing;
    boardRuntime->controlsActive = 0;
    boardRuntime->controlEnableDelay = delay;
    boardRuntime->coreState = core;
}
void HandleOutRunOpcode() {
    GameArcadeRuntime &runtime = *boardRuntime;
    switch (s1::g_emu68Instruction) {
    case 0xa000:
        SetState(0, 0, 0, 0, 2);
        firstCreditSeen = 0;
        WriteMappedByte(0x60892, 0x80);
        s1::reg68->a[0] = 0xffffff20u;
        s1::reg68->pc = 31522;
        break;
    case 0xa001:
        SetState(0, 0, 0, 0, 2);
        WriteMappedByte(0x6090a, 0xff);
        s1::reg68->pc = 45536;
        break;
    case 0xa002: {
        SetState(1, 0, 0, 0, 1);
        std::uint16_t credits = ReadMappedWord(0x6080c) + 1;
        if (!firstCreditSeen) {
            firstCreditSeen = 1;
            credits = 4;
        }
        s1::reg68->pc = 45596;
        WriteMappedWord(0x6080c, credits);
        break;
    }
    case 0xa003:
        if (ReadMappedByte(0x60c50))
            WriteMappedByte(0x60052, 1);
        s1::reg68->pc = 45614;
        break;
    case 0xa004:
        PushReturn(45782);
        s1::reg68->pc = 33940;
        break;
    case 0xa005:
        SetState(0, 1, 0, 0, 0);
        g_outRunGear = 0;
        WriteMappedWord(0x60b5e, 0xffff);
        s1::reg68->pc = 45898;
        break;
    case 0xa006:
        SetState(0, 0, 1, 60, 0);
        WriteMappedByte(0x60b6f, ReadMappedByte(0x60b6f) & 0xdf);
        s1::reg68->pc = 46056;
        break;
    case 0xa007:
        SetState(0, 0, 1, 0, 0);
        runtime.completion = 1;
        WriteMappedWord(0x60864, ReadMappedWord(0x60862));
        s1::reg68->pc = 46480;
        break;
    case 0xa008:
        SetState(0, 0, 1, 0, 0);
        WriteMappedWord(0x60b5e, 0xffff);
        s1::reg68->pc = 46550;
        break;
    case 0xa009:
        SetState(0, 0, 0, 0, 0);
        WriteMappedWord(s1::reg68->a[4], ReadMappedWord(s1::reg68->a[4]) + 1);
        s1::reg68->pc = 28264;
        break;
    case 0xa100:
        if (!ReadMappedByte(0x60052)) {
            PushReturn(45850);
            s1::reg68->pc = 46902;
        } else
            s1::reg68->pc = 45880;
        break;
    case 0xa101:
        s1::reg68->d[0] = (s1::reg68->d[0] & 0xffff0000u) | (runtime.displayCredits & 0xffff);
        s1::reg68->pc = 27894;
        break;
    case 0xa102:
        s1::reg68->pc = runtime.displayCredits > 1 ? 27924 : 27910;
        break;
    case 0xa103:
        runtime.coreState = 0;
        if (runtime.displayCredits > 0)
            --runtime.displayCredits;
        if (runtime.credits > 0)
            --runtime.credits;
        s1::reg68->d[0] = 0;
        s1::reg68->pc = 28292;
        break;
    case 0xa104:
        std::memcpy(s1::g_harrierMemory + 0x8046e, boardRankings, 0xf0);
        s1::reg68->pc = ReadMappedLong(s1::reg68->a[7]);
        s1::reg68->a[7] += 4;
        break;
    case 0xa105: {
        const std::uint32_t score = ReadMappedLong(0x60884);
        if (score > runtime.bestScore)
            runtime.bestScore = score;
        PushReturn(46730);
        s1::reg68->pc = 32330;
        break;
    }
    case 0xa106: {
        const std::uint8_t index = ReadMappedByte(0x60416);
        if (index < 20) {
            const std::uint32_t score = ReadMappedLong(14 * index + 394350);
            if (score >= runtime.bestScore)
                runtime.bestScore = score;
        }
        WriteMappedWord(0x6080c, ReadMappedWord(0x6080c) + 1);
        break;
    }
    case 0xa107:
        s1::reg68->pc = runtime.credits < 0 ? 27948 : 27880;
        if (ReadMappedByte(0x60c50))
            s1::reg68->pc = 27948;
        break;
    case 0xa800: {
        const std::uint16_t a = ReadMappedWord(0x60510);
        const std::uint16_t b = ReadMappedWord(0x60514);
        WriteMappedWord(s1::reg68->a[5] + 0x510, static_cast<std::uint16_t>(s1::reg68->d[0]));
        if (a == 0 && b == 0)
            WriteMappedWord(s1::reg68->a[5] + 0x514, static_cast<std::uint16_t>(s1::reg68->d[0]));
        s1::reg68->pc = 5318;
        break;
    }
    }
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
        .mask = 0xffffffffu};
    for (std::uint32_t page = 0; page < 256; ++page) {
        primaryPages[page] = empty;
        secondaryPages[page] = empty;
    }
    for (std::uint32_t page = 0; page < 4; ++page) {
        primaryPages[page] = RomPage(page * 0x10000);
        primaryPages[page + 0x20] = RomPage((page + 4) * 0x10000);
        secondaryPages[page] = RomPage((page + 4) * 0x10000);
    }
    primaryPages[6] = RamPage(0x80000, 0x7fff);
    primaryPages[0x10] = RamPage(0x90000, 0xffff);
    primaryPages[0x11] = RamPage(0xa0000, 0xfff);
    primaryPages[0x12] = RamPage(0xb0000, 0x3fff);
    primaryPages[0x13] = RamPage(0xc0000, 0x7ff);
    primaryPages[0x14] = RamPage(0xd0000, 0x7f);
    primaryPages[0x14].read8 = ReadAnalog;
    primaryPages[0x14].write8 = WriteAnalog;
    primaryPages[0x26] = RamPage(0x88000, 0x7fff);
    primaryPages[0xff] = {.read8 = ReadSoundByte,
        .write8 = WriteSoundByte,
        .read16 = ReadSoundWord,
        .write16 = WriteSoundWord,
        .read32 = ReadSoundLong,
        .write32 = WriteSoundLong,
        .base = 0,
        .mask = 0x3f};
    secondaryPages[6] = RamPage(0x88000, 0x7fff);
    secondaryPages[8] = RamPage(0xe0000, 0xfff);
    secondaryPages[9] = empty;
    secondaryPages[9].read8 = ReadRoadControl;
    secondaryPages[9].write8 = WriteRoadControl;
    secondaryPages[9].mask = 0xffff;
}
constexpr std::int32_t patches[]{
    0x7b1e,
    0xa000,
    -1,
    0xb1d8,
    0xa001,
    -1,
    0xb216,
    0xa002,
    -1,
    0xb21c,
    0xa003,
    -1,
    0xb2d2,
    0xa004,
    -1,
    0xb342,
    0xa005,
    -1,
    0xb3e0,
    0xa006,
    -1,
    0xb586,
    0xa007,
    -1,
    0xb5ce,
    0xa008,
    -1,
    0x6e66,
    0xa009,
    -1,
    0x414c2,
    0xa800,
    -1,
    0xb316,
    0xa100,
    -1,
    0x6cf0,
    0xa101,
    -1,
    0x6cfa,
    0xa102,
    -1,
    0x6e82,
    0xa103,
    -1,
    0xd1c2,
    0xa104,
    -1,
    0xb686,
    0xa105,
    -1,
    0xb71e,
    0xa106,
    -1,
    0x6cde,
    0xa107,
    -1,
    0x6b46,
    0xd38,
    -1,
    0xba22,
    0xc3a,
    -1,
    0xbc74,
    0xc40,
    -1,
    0xbc80,
    0xcc0,
    -1,
    0x8b9a,
    0xcf6,
    -1,
    0xbca8,
    0xd74,
    -1,
    0x9038,
    0xd6c,
    -1,
    0x6cec,
    0xd3e,
    -1,
    0x6d3a,
    0xd38,
    -1,
    0x6d4a,
    0xd38,
    -1,
    0x6d5a,
    0xd38,
    -1,
    0x6d6e,
    0xd38,
    -1,
    0xbcf4,
    0xd66,
    -1,
    0xbd06,
    0xd70,
    -1,
    0xbc5c,
    0xb6,
    -1,
    0xbc68,
    0x136,
    -1,
    -1,
};
static_assert(sizeof(patches) == 106 * sizeof(std::int32_t));

} // namespace

void InitializeOutRunBoard(GameArcadeRuntime &runtime, const std::uint8_t *rankings) {
    boardRuntime = &runtime;
    boardRankings = rankings;
    s1::ArchiveData_Read("XOUT\\datas\\OUT.ROM", 0, s1::g_harrierMemory, 0x80000);
    s1::g_emu68OpcodeDispatch[0xa] = HandleOutRunOpcode;
    InitializeOutRunGraphics();
    InitializeOutRunSound();
    s1::g_harrierInputScaleX = 1.0f;
    s1::g_harrierInputScaleY = 1.0f;
    runtime.inputOrientation = 0;
    s1::g_harrierMemory[0xd0015] = 0xff;
    s1::g_harrierMemory[0xd0017] = 0xfc;
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
    InitializeOutRunRoadVertices();
}
void UpdateOutRunBoard(GameArcadeRuntime &runtime) {
    boardRuntime = &runtime;
    if (runtime.paused || runtime.resumeDelay)
        return;
    const std::uint32_t pressed = s1::g_inputState.slots[0].pressedPriority;
    if (pressed & 0x400)
        g_outRunGear = 0;
    if (pressed & 4)
        g_outRunGear = 1;
    std::uint8_t input = runtime.coinPulse ? 0xbf : 0xff;
    s1::g_harrierMemory[0xd0011] = input;
    if (runtime.startPulse) {
        input -= 8;
        s1::g_harrierMemory[0xd0011] = input;
    }
    if (g_outRunGear == 0)
        s1::g_harrierMemory[0xd0011] = input - 16;
    std::memcpy(g_outRunRoadSnapshot.data(), s1::g_harrierMemory + 0xc0000, 0x800);
    g_outRunRoadOffset0 = s1::Harrier_ReadMemory16BigEndian(0xa0e90);
    g_outRunRoadOffset1 = s1::Harrier_ReadMemory16BigEndian(0xa0e92);
    s1::emu68_Run();
    if (runtime.controlsActive)
        ++runtime.playedFrames;
}
void CopyOutRunRankings(std::uint8_t *rankings) {
    std::memcpy(rankings, s1::g_harrierMemory + 0x8046e, 0xf0);
}

} // namespace shenmue::s2
