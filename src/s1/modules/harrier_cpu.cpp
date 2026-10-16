#include "harrier_cpu.hpp"
#include "harrier_audio.hpp"
#include "harrier_render.hpp"

#include "common/archive_data.hpp"

#include <bit>
#include <cstdio>

namespace shenmue::s1 {

using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

std::uint8_t g_harrierMemory[kHarrierMemorySize]{};
reg68_ctx_t g_reg68_ctx{};
reg68_ctx_t g_reg68_ctx_2{};
reg68_ctx_t *reg68 = &g_reg68_ctx;
std::uint16_t g_emu68Instruction{};
std::uint32_t g_emu68_ea{};
std::uint8_t g_harrierIrqLevel2{};
std::uint8_t g_harrierIrqLevel4{};
std::uint8_t g_harrierIrqFrame{};
static char g_emu68DebugText[512]{};

static std::uint32_t &Emu68_Register(reg68_ctx_t *context, unsigned index) {
    return index < 8 ? context->d[index] : context->a[index - 8];
}

static inline std::int32_t Emu68_Add32(std::int32_t left, std::int32_t right) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(left) + static_cast<std::uint32_t>(right));
}
static inline std::int32_t Emu68_Subtract32(std::int32_t left, std::int32_t right) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(left) - static_cast<std::uint32_t>(right));
}
static inline std::int32_t Emu68_Multiply32(std::int32_t left, std::int32_t right) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(left) * static_cast<std::uint32_t>(right));
}

std::uint8_t Harrier_ReadMemory8(std::uint32_t address) {
    return g_harrierMemory[address];
}
std::uint16_t Harrier_ReadMemory16BigEndian(std::uint32_t address) {
    return g_harrierMemory[address + 1] | (g_harrierMemory[address] << 8);
}
std::uint32_t Harrier_ReadMemory32BigEndian(std::uint32_t address) {
    return g_harrierMemory[address + 3] | (g_harrierMemory[address + 2] << 8) | (g_harrierMemory[address + 1] << 16) |
        (std::uint32_t{g_harrierMemory[address]} << 24);
}
void Harrier_WriteMemory8(std::uint32_t address, std::uint8_t value) {
    g_harrierMemory[address] = value;
}
void Harrier_WriteMemory16BigEndian(std::uint32_t address, std::uint16_t value) {
    g_harrierMemory[address] = static_cast<std::uint8_t>(value >> 8);
    g_harrierMemory[address + 1] = static_cast<std::uint8_t>(value);
}
void Harrier_WriteMemory32BigEndian(std::uint32_t address, std::uint32_t value) {
    g_harrierMemory[address + 1] = static_cast<std::uint8_t>(value >> 16);
    g_harrierMemory[address + 2] = static_cast<std::uint8_t>(value >> 8);
    g_harrierMemory[address + 3] = static_cast<std::uint8_t>(value);
    g_harrierMemory[address] = static_cast<std::uint8_t>(value >> 24);
}
std::uint8_t Emu68_UnmappedRead8(std::uint32_t) {
    return 0;
}
std::uint16_t Emu68_UnmappedRead16(std::uint32_t) {
    return 0;
}
std::uint32_t Emu68_UnmappedRead32(std::uint32_t) {
    return 0;
}
void Emu68_IgnoreWrite8(std::uint32_t, std::uint8_t) {
}
void Emu68_IgnoreWrite16(std::uint32_t, std::uint16_t) {
}
void Emu68_IgnoreWrite32(std::uint32_t, std::uint32_t) {
}
std::intptr_t Emu68_NullOperand(int) {
    return 0;
}
std::intptr_t Emu68_ReportWriteModeError(int index, std::uint32_t) {
    return Emu68_ReportWriteModeError(index);
}

void Emu68_ReportLineFError() {

    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "error fxxx\n");
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    return;
}

std::intptr_t Emu68_ReportRuntimeError(const char *reason, int instructionWord) {

    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", reason, instructionWord);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
    return (Harrier_DebugPrint(g_emu68DebugText), 0);
}

std::intptr_t Emu68_ResetOtherCpu() {
    reg68_ctx_t *context3;

    context3 = reg68;
    if (reg68 == &g_reg68_ctx) {
        reg68 = &g_reg68_ctx_2;
        Emu68_ResetContext();
    }
    if (context3 == &g_reg68_ctx_2) {
        reg68 = &g_reg68_ctx;
        Emu68_ResetContext();
    }
    reg68 = context3;
    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "reset instruction encount\n");
    return (Harrier_DebugPrint(g_emu68DebugText), 0);
}

std::intptr_t Emu68_FetchWord() {
    reg68_page_map_entry_t *page0;
    int64_t result;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    result = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return result;
}

std::intptr_t Emu68_DecodeBriefIndex(uint16_t operand) {
    if ((operand & 0x8000u) == 0) {
        if ((operand & 0x800) != 0)
            return reg68->d[((uint64_t)operand >> 12) & 7] + (int8_t)operand;
        else
            return (unsigned int)((int8_t)operand + static_cast<int16_t>(reg68->d[((uint64_t)operand >> 12) & 7]));
    } else if ((operand & 0x800) != 0) {
        return reg68->a[((uint64_t)operand >> 12) & 7] + (int8_t)operand;
    } else {
        return (unsigned int)((int8_t)operand + static_cast<int16_t>(reg68->a[((uint64_t)operand >> 12) & 7]));
    }
}

std::intptr_t Emu68_ReportAddressModeError(int operand) {

    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    return 0;
}

std::intptr_t Emu68_ReadAddressRegister(int operand) {
    return reg68->a[operand];
}

std::intptr_t Emu68_AddressDisplacement(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    return context4->a[value1] + value3;
}

std::intptr_t Emu68_AddressIndexed(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    return (unsigned int)(reg68->a[value1] + value5);
}

std::intptr_t Emu68_AddressExtended(int operand) {

    uint32_t result;
    uint32_t value4;
    reg68_page_map_entry_t *page5;
    uint16_t value6;
    reg68_ctx_t *context7;
    uint32_t pc;
    reg68_page_map_entry_t *page9;
    int16_t value10;
    reg68_ctx_t *context11;
    reg68_page_map_entry_t *page12;
    reg68_ctx_t *context13;
    reg68_page_map_entry_t *page14;
    reg68_ctx_t *context15;

    if (operand) {
        switch (operand) {
        case 1:
            page12 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
            result = page12->read32(page12->base + (reg68->pc & page12->mask));
            context13 = reg68;
            reg68->bus_word_count += 2;
            context13->pc += 4;
            break;
        case 2:
            pc = reg68->pc;
            page9 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
            value10 = page9->read16(page9->base + (pc & page9->mask));
            context11 = reg68;
            ++reg68->bus_word_count;
            context11->pc += 2;
            return pc + value10;
        case 3:
            value4 = reg68->pc;
            page5 = &reg68->map[static_cast<uint8_t>((value4) >> 16)];
            value6 = page5->read16(page5->base + (value4 & page5->mask));
            context7 = reg68;
            ++reg68->bus_word_count;
            context7->pc += 2;
            return value4 + Emu68_DecodeBriefIndex(value6);
        default:
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
            (Harrier_DebugPrint(g_emu68DebugText), 0);
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
            (Harrier_DebugPrint(g_emu68DebugText), 0);
            return 0;
        }
    } else {
        page14 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        result = static_cast<uint16_t>(page14->read16(page14->base + (reg68->pc & page14->mask)));
        context15 = reg68;
        ++reg68->bus_word_count;
        context15->pc += 2;
        return (int16_t)result;
    }
    return result;
}

std::intptr_t Emu68_ReportByteReadModeError(int operand) {

    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    return 0;
}

std::intptr_t Emu68_ReadDataByte(int operand) {
    return static_cast<uint8_t>(reg68->d[operand]);
}

std::intptr_t Emu68_ReadIndirectByte(int operand) {
    reg68_page_map_entry_t *page1;
    int64_t result;

    page1 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[operand]) >> 16)];
    result = page1->read8(page1->base + (reg68->a[operand] & page1->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadPostincrementByte(int operand) {
    int64_t value2;
    reg68_page_map_entry_t *page3;
    int64_t result;
    uint32_t *value5;

    value2 = 4LL * operand + 32;
    page3 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[value2 / 4 - 8]) >> 16)];
    result = page3->read8(page3->base + (reg68->a[value2 / 4 - 8] & page3->mask));
    value5 = &reg68->a[value2 / 4 - 8];
    ++reg68->bus_word_count;
    *value5 += (operand == 7) + 1;
    return result;
}

std::intptr_t Emu68_ReadPredecrementByte(int operand) {
    reg68_ctx_t *context1;
    uint32_t *value2;
    reg68_page_map_entry_t *page3;
    int64_t result;

    context1 = reg68;
    value2 = &reg68->a[operand];
    *value2 += -1 - (operand == 7);
    page3 = &context1->map[(uint8_t) static_cast<uint8_t>((*value2) >> 16)];
    result = page3->read8(page3->base + (*value2 & page3->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadDisplacementByte(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;
    reg68_page_map_entry_t *page5;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    page5 = &context4->map[(uint8_t)((context4->a[value1] + value3) >> 16)];
    result = page5->read8(page5->base + ((context4->a[value1] + value3) & page5->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadIndexedByte(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    unsigned int value7;
    reg68_page_map_entry_t *page8;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    value7 = reg68->a[value1] + value5;
    page8 = &reg68->map[static_cast<uint8_t>((value7) >> 16)];
    result = page8->read8(page8->base + (value7 & page8->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadExtendedByte(int operand) {

    int8_t result;
    reg68_page_map_entry_t *page4;
    reg68_ctx_t *context5;
    uint32_t value6;
    reg68_page_map_entry_t *page7;
    uint16_t value8;
    reg68_ctx_t *context9;
    int value10;

    reg68_page_map_entry_t *page12;
    int value13;
    uint32_t pc;
    reg68_page_map_entry_t *page15;
    int16_t value16;
    reg68_ctx_t *context17;
    reg68_page_map_entry_t *page18;
    uint32_t value19;
    reg68_ctx_t *context20;
    reg68_page_map_entry_t *page21;
    reg68_page_map_entry_t *page22;
    int16_t value23;
    reg68_ctx_t *context24;
    reg68_page_map_entry_t *page25;

    switch (operand) {
    case 0:
        page22 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value23 = page22->read16(page22->base + (reg68->pc & page22->mask));
        context24 = reg68;
        ++reg68->bus_word_count;
        context24->pc += 2;
        page25 = &context24->map[(uint8_t)(value23 >> 15)];
        result = page25->read8(page25->base + ((unsigned int)value23 & page25->mask));
        ++reg68->bus_word_count;
        return result;
    case 1:
        page18 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value19 = page18->read32(page18->base + (reg68->pc & page18->mask));
        context20 = reg68;
        reg68->bus_word_count += 2;
        page21 = &context20->map[static_cast<uint8_t>((value19) >> 16)];
        context20->pc += 4;
        result = page21->read8(page21->base + (value19 & page21->mask));
        ++reg68->bus_word_count;
        return result;
    case 2:
        pc = reg68->pc;
        page15 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value16 = page15->read16(page15->base + (pc & page15->mask));
        context17 = reg68;
        ++reg68->bus_word_count;
        context17->pc += 2;
        page12 = &context17->map[(uint8_t)((pc + value16) >> 16)];
        value13 = (pc + value16) & page12->mask;
        goto LABEL_9;
    case 3:
        value6 = reg68->pc;
        page7 = &reg68->map[static_cast<uint8_t>((value6) >> 16)];
        value8 = page7->read16(page7->base + (value6 & page7->mask));
        context9 = reg68;
        ++reg68->bus_word_count;
        context9->pc += 2;
        value10 = value6 + Emu68_DecodeBriefIndex(value8);
        page12 = &reg68->map[static_cast<uint8_t>((value10) >> 16)];
        value13 = value10 & page12->mask;
    LABEL_9:
        result = page12->read8((unsigned int)(page12->base + value13));
        ++reg68->bus_word_count;
        return result;
    case 4:
        page4 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        result = page4->read16(page4->base + (reg68->pc & page4->mask));
        context5 = reg68;
        ++reg68->bus_word_count;
        context5->pc += 2;
        break;
    default:
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        return 0;
    }
    return result;
}

std::intptr_t Emu68_ReadDataWord(int operand) {
    return static_cast<uint16_t>(reg68->d[operand]);
}

std::intptr_t Emu68_ReadAddressWord(int operand) {
    return static_cast<uint16_t>(reg68->a[operand]);
}

std::intptr_t Emu68_ReadIndirectWord(int operand) {
    reg68_page_map_entry_t *page1;
    int64_t result;

    page1 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[operand]) >> 16)];
    result = page1->read16(page1->base + (reg68->a[operand] & page1->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadPostincrementWord(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int64_t result;
    reg68_ctx_t *context4;

    value1 = 4LL * operand + 32;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[value1 / 4 - 8]) >> 16)];
    result = page2->read16(page2->base + (reg68->a[value1 / 4 - 8] & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->a[value1 / 4 - 8] += 2;
    return result;
}

std::intptr_t Emu68_ReadPredecrementWord(int operand) {
    int64_t value1;
    reg68_ctx_t *context2;
    reg68_page_map_entry_t *page3;
    int64_t result;

    value1 = operand;
    context2 = reg68;
    reg68->a[value1] -= 2;
    page3 = &context2->map[(uint8_t) static_cast<uint8_t>((context2->a[value1]) >> 16)];
    result = page3->read16(page3->base + (context2->a[value1] & page3->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadDisplacementWord(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;
    reg68_page_map_entry_t *page5;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    page5 = &context4->map[(uint8_t)((context4->a[value1] + value3) >> 16)];
    result = page5->read16(page5->base + ((context4->a[value1] + value3) & page5->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadIndexedWord(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    unsigned int value7;
    reg68_page_map_entry_t *page8;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    value7 = reg68->a[value1] + value5;
    page8 = &reg68->map[static_cast<uint8_t>((value7) >> 16)];
    result = page8->read16(page8->base + (value7 & page8->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadExtendedWord(int operand) {

    int64_t result;
    reg68_page_map_entry_t *page4;
    reg68_ctx_t *context5;
    uint32_t value6;
    reg68_page_map_entry_t *page7;
    uint16_t value8;
    reg68_ctx_t *context9;
    int value10;

    reg68_page_map_entry_t *page12;
    int value13;
    uint32_t pc;
    reg68_page_map_entry_t *page15;
    int16_t value16;
    reg68_ctx_t *context17;
    reg68_page_map_entry_t *page18;
    uint32_t value19;
    reg68_ctx_t *context20;
    reg68_page_map_entry_t *page21;
    reg68_page_map_entry_t *page22;
    int16_t value23;
    reg68_ctx_t *context24;
    reg68_page_map_entry_t *page25;

    switch (operand) {
    case 0:
        page22 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value23 = page22->read16(page22->base + (reg68->pc & page22->mask));
        context24 = reg68;
        ++reg68->bus_word_count;
        context24->pc += 2;
        page25 = &context24->map[(uint8_t)(value23 >> 15)];
        result = page25->read16(page25->base + ((unsigned int)value23 & page25->mask));
        ++reg68->bus_word_count;
        return result;
    case 1:
        page18 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value19 = page18->read32(page18->base + (reg68->pc & page18->mask));
        context20 = reg68;
        reg68->bus_word_count += 2;
        page21 = &context20->map[static_cast<uint8_t>((value19) >> 16)];
        context20->pc += 4;
        result = page21->read16(page21->base + (value19 & page21->mask));
        ++reg68->bus_word_count;
        return result;
    case 2:
        pc = reg68->pc;
        page15 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value16 = page15->read16(page15->base + (pc & page15->mask));
        context17 = reg68;
        ++reg68->bus_word_count;
        context17->pc += 2;
        page12 = &context17->map[(uint8_t)((pc + value16) >> 16)];
        value13 = (pc + value16) & page12->mask;
        goto LABEL_10;
    case 3:
        value6 = reg68->pc;
        page7 = &reg68->map[static_cast<uint8_t>((value6) >> 16)];
        value8 = page7->read16(page7->base + (value6 & page7->mask));
        context9 = reg68;
        ++reg68->bus_word_count;
        context9->pc += 2;
        value10 = value6 + Emu68_DecodeBriefIndex(value8);
        page12 = &reg68->map[static_cast<uint8_t>((value10) >> 16)];
        value13 = value10 & page12->mask;
    LABEL_10:
        result = page12->read16((unsigned int)(page12->base + value13));
        ++reg68->bus_word_count;
        return result;
    case 4:
        page4 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        result = page4->read16(page4->base + (reg68->pc & page4->mask));
        context5 = reg68;
        ++reg68->bus_word_count;
        context5->pc += 2;
        break;
    default:
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        return 0;
    }
    return result;
}

std::intptr_t Emu68_ReadDataLong(int operand) {
    return reg68->d[operand];
}

std::intptr_t Emu68_ReadIndirectLong(int operand) {
    reg68_page_map_entry_t *page1;
    int64_t result;

    page1 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[operand]) >> 16)];
    result = page1->read32(page1->base + (reg68->a[operand] & page1->mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadPostincrementLong(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int64_t result;
    reg68_ctx_t *context4;

    value1 = 4LL * operand + 32;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[value1 / 4 - 8]) >> 16)];
    result = page2->read32(page2->base + (reg68->a[value1 / 4 - 8] & page2->mask));
    context4 = reg68;
    reg68->bus_word_count += 2;
    context4->a[value1 / 4 - 8] += 4;
    return result;
}

std::intptr_t Emu68_ReadPredecrementLong(int operand) {
    int64_t value1;
    reg68_ctx_t *context2;
    reg68_page_map_entry_t *page3;
    int64_t result;

    value1 = operand;
    context2 = reg68;
    reg68->a[value1] -= 4;
    page3 = &context2->map[(uint8_t) static_cast<uint8_t>((context2->a[value1]) >> 16)];
    result = page3->read32(page3->base + (context2->a[value1] & page3->mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadDisplacementLong(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;
    reg68_page_map_entry_t *page5;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    page5 = &context4->map[(uint8_t)((context4->a[value1] + value3) >> 16)];
    result = page5->read32(page5->base + ((context4->a[value1] + value3) & page5->mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadIndexedLong(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    unsigned int value7;
    reg68_page_map_entry_t *page8;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    value7 = reg68->a[value1] + value5;
    page8 = &reg68->map[static_cast<uint8_t>((value7) >> 16)];
    result = page8->read32(page8->base + (value7 & page8->mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadExtendedLong(int operand) {

    int64_t result;
    reg68_page_map_entry_t *page4;
    reg68_ctx_t *context5;
    uint32_t value6;
    reg68_page_map_entry_t *page7;
    uint16_t value8;
    reg68_ctx_t *context9;
    int value10;

    reg68_page_map_entry_t *page12;
    int value13;
    uint32_t pc;
    reg68_page_map_entry_t *page15;
    int16_t value16;
    reg68_ctx_t *context17;
    reg68_page_map_entry_t *page18;
    uint32_t value19;
    reg68_ctx_t *context20;
    reg68_page_map_entry_t *page21;
    reg68_page_map_entry_t *page22;
    int16_t value23;
    reg68_ctx_t *context24;
    reg68_page_map_entry_t *page25;

    switch (operand) {
    case 0:
        page22 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value23 = page22->read16(page22->base + (reg68->pc & page22->mask));
        context24 = reg68;
        ++reg68->bus_word_count;
        context24->pc += 2;
        page25 = &context24->map[(uint8_t)(value23 >> 15)];
        result = page25->read32(page25->base + ((unsigned int)value23 & page25->mask));
        reg68->bus_word_count += 2;
        return result;
    case 1:
        page18 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value19 = page18->read32(page18->base + (reg68->pc & page18->mask));
        context20 = reg68;
        reg68->bus_word_count += 2;
        page21 = &context20->map[static_cast<uint8_t>((value19) >> 16)];
        context20->pc += 4;
        result = page21->read32(page21->base + (value19 & page21->mask));
        reg68->bus_word_count += 2;
        return result;
    case 2:
        pc = reg68->pc;
        page15 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value16 = page15->read16(page15->base + (pc & page15->mask));
        context17 = reg68;
        ++reg68->bus_word_count;
        context17->pc += 2;
        page12 = &context17->map[(uint8_t)((pc + value16) >> 16)];
        value13 = (pc + value16) & page12->mask;
        goto LABEL_10;
    case 3:
        value6 = reg68->pc;
        page7 = &reg68->map[static_cast<uint8_t>((value6) >> 16)];
        value8 = page7->read16(page7->base + (value6 & page7->mask));
        context9 = reg68;
        ++reg68->bus_word_count;
        context9->pc += 2;
        value10 = value6 + Emu68_DecodeBriefIndex(value8);
        page12 = &reg68->map[static_cast<uint8_t>((value10) >> 16)];
        value13 = value10 & page12->mask;
    LABEL_10:
        result = page12->read32((unsigned int)(page12->base + value13));
        reg68->bus_word_count += 2;
        return result;
    case 4:
        page4 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        result = page4->read32(page4->base + (reg68->pc & page4->mask));
        context5 = reg68;
        reg68->bus_word_count += 2;
        context5->pc += 4;
        break;
    default:
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        return 0;
    }
    return result;
}

std::intptr_t Emu68_ReadModifyIndirectByte(int operand) {
    int64_t value1;
    int64_t result;

    value1 = (uint8_t) static_cast<uint8_t>((reg68->a[operand]) >> 16);
    g_emu68_ea = reg68->a[operand];
    result = reg68->map[value1].read8(reg68->map[value1].base + (g_emu68_ea & reg68->map[value1].mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyPredecrementByte(int operand) {
    reg68_ctx_t *context1;
    uint32_t *value2;
    int64_t value3;
    int64_t result;

    context1 = reg68;
    value2 = &reg68->a[operand];
    *value2 += -1 - (operand == 7);
    value3 = (uint8_t) static_cast<uint8_t>((*value2) >> 16);
    g_emu68_ea = *value2;
    result = context1->map[value3].read8(context1->map[value3].base + (g_emu68_ea & context1->map[value3].mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyDisplacementByte(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;
    reg68_page_map_entry_t *page5;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    g_emu68_ea = context4->a[value1] + value3;
    page5 = &context4->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = page5->read8(page5->base + (g_emu68_ea & page5->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyIndexedByte(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    reg68_page_map_entry_t *page7;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    g_emu68_ea = reg68->a[value1] + value5;
    page7 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = page7->read8(page7->base + (g_emu68_ea & page7->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyExtendedByte(int operand) {

    int8_t result;
    uint32_t value4;
    reg68_page_map_entry_t *page5;
    uint16_t value6;
    reg68_ctx_t *context7;

    reg68_page_map_entry_t *page9;
    uint32_t value10;
    uint32_t pc;
    reg68_page_map_entry_t *page12;
    int16_t value13;
    reg68_ctx_t *context14;
    uint32_t value15;
    reg68_page_map_entry_t *page16;
    uint32_t value17;
    reg68_ctx_t *context18;
    reg68_page_map_entry_t *page19;
    reg68_page_map_entry_t *page20;
    int16_t value21;
    reg68_ctx_t *context22;
    reg68_page_map_entry_t *page23;

    if (operand) {
        if (operand == 1) {
            page16 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
            value17 = page16->read32(page16->base + (reg68->pc & page16->mask));
            context18 = reg68;
            reg68->bus_word_count += 2;
            page19 = &context18->map[static_cast<uint8_t>((value17) >> 16)];
            context18->pc += 4;
            g_emu68_ea = value17;
            result = page19->read8(page19->base + (value17 & page19->mask));
            ++reg68->bus_word_count;
        } else {
            if (operand == 2) {
                pc = reg68->pc;
                page12 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
                value13 = page12->read16(page12->base + (pc & page12->mask));
                context14 = reg68;
                value15 = pc + value13;
                g_emu68_ea = value15;
                ++reg68->bus_word_count;
                context14->pc += 2;
                page9 = &context14->map[static_cast<uint8_t>((value15) >> 16)];
                value10 = value15 & page9->mask;
            } else {
                if (operand != 3) {
                    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
                    (Harrier_DebugPrint(g_emu68DebugText), 0);
                    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
                    (Harrier_DebugPrint(g_emu68DebugText), 0);
                    return 0;
                }
                value4 = reg68->pc;
                page5 = &reg68->map[static_cast<uint8_t>((value4) >> 16)];
                value6 = page5->read16(page5->base + (value4 & page5->mask));
                context7 = reg68;
                ++reg68->bus_word_count;
                context7->pc += 2;
                g_emu68_ea = value4 + Emu68_DecodeBriefIndex(value6);
                page9 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
                value10 = g_emu68_ea & page9->mask;
            }
            result = page9->read8(page9->base + value10);
            ++reg68->bus_word_count;
        }
    } else {
        page20 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value21 = page20->read16(page20->base + (reg68->pc & page20->mask));
        context22 = reg68;
        g_emu68_ea = value21;
        ++reg68->bus_word_count;
        context22->pc += 2;
        page23 = &context22->map[(uint8_t)(value21 >> 15)];
        result = page23->read8(page23->base + ((unsigned int)value21 & page23->mask));
        ++reg68->bus_word_count;
    }
    return result;
}

std::intptr_t Emu68_ReadModifyIndirectWord(int operand) {
    int64_t value1;
    int64_t result;

    value1 = (uint8_t) static_cast<uint8_t>((reg68->a[operand]) >> 16);
    g_emu68_ea = reg68->a[operand];
    result = reg68->map[value1].read16(reg68->map[value1].base + (g_emu68_ea & reg68->map[value1].mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyPredecrementWord(int operand) {
    int64_t value1;
    reg68_ctx_t *context2;
    uint32_t value3;
    int64_t result;

    value1 = operand;
    context2 = reg68;
    reg68->a[value1] -= 2;
    value3 = context2->a[value1];
    g_emu68_ea = value3;
    reg68_page_map_entry_t &memoryPage = context2->map[static_cast<uint8_t>(value3 >> 16)];
    result = memoryPage.read16(memoryPage.base + (value3 & memoryPage.mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyDisplacementWord(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;
    reg68_page_map_entry_t *page5;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    g_emu68_ea = context4->a[value1] + value3;
    page5 = &context4->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = page5->read16(page5->base + (g_emu68_ea & page5->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyIndexedWord(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    reg68_page_map_entry_t *page7;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    g_emu68_ea = reg68->a[value1] + value5;
    page7 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = page7->read16(page7->base + (g_emu68_ea & page7->mask));
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyExtendedWord(int operand) {

    int64_t result;
    uint32_t value4;
    reg68_page_map_entry_t *page5;
    uint16_t value6;
    reg68_ctx_t *context7;

    reg68_page_map_entry_t *page9;
    uint32_t value10;
    uint32_t pc;
    reg68_page_map_entry_t *page12;
    int16_t value13;
    reg68_ctx_t *context14;
    uint32_t value15;
    reg68_page_map_entry_t *page16;
    uint32_t value17;
    reg68_ctx_t *context18;
    reg68_page_map_entry_t *page19;
    int16_t value20;
    reg68_ctx_t *context21;

    if (operand) {
        switch (operand) {
        case 1:
            page16 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
            value17 = page16->read32(page16->base + (reg68->pc & page16->mask));
            context18 = reg68;
            reg68->bus_word_count += 2;
            page9 = &context18->map[static_cast<uint8_t>((value17) >> 16)];
            context18->pc += 4;
            g_emu68_ea = value17;
            value10 = value17 & page9->mask;
            break;
        case 2:
            pc = reg68->pc;
            page12 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
            value13 = page12->read16(page12->base + (pc & page12->mask));
            context14 = reg68;
            value15 = pc + value13;
            g_emu68_ea = value15;
            ++reg68->bus_word_count;
            context14->pc += 2;
            page9 = &context14->map[static_cast<uint8_t>((value15) >> 16)];
            value10 = value15 & page9->mask;
            break;
        case 3:
            value4 = reg68->pc;
            page5 = &reg68->map[static_cast<uint8_t>((value4) >> 16)];
            value6 = page5->read16(page5->base + (value4 & page5->mask));
            context7 = reg68;
            ++reg68->bus_word_count;
            context7->pc += 2;
            g_emu68_ea = value4 + Emu68_DecodeBriefIndex(value6);
            page9 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
            value10 = g_emu68_ea & page9->mask;
            break;
        default:
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
            (Harrier_DebugPrint(g_emu68DebugText), 0);
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
            (Harrier_DebugPrint(g_emu68DebugText), 0);
            return 0;
        }
    } else {
        page19 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value20 = page19->read16(page19->base + (reg68->pc & page19->mask));
        context21 = reg68;
        g_emu68_ea = value20;
        ++reg68->bus_word_count;
        context21->pc += 2;
        page9 = &context21->map[(uint8_t)(value20 >> 15)];
        value10 = value20 & page9->mask;
    }
    result = page9->read16(page9->base + value10);
    ++reg68->bus_word_count;
    return result;
}

std::intptr_t Emu68_ReadModifyIndirectLong(int operand) {
    int64_t value1;
    int64_t result;

    value1 = (uint8_t) static_cast<uint8_t>((reg68->a[operand]) >> 16);
    g_emu68_ea = reg68->a[operand];
    result = reg68->map[value1].read32(reg68->map[value1].base + (g_emu68_ea & reg68->map[value1].mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadModifyPredecrementLong(int operand) {
    int64_t value1;
    reg68_ctx_t *context2;
    uint32_t value3;
    int64_t result;

    value1 = operand;
    context2 = reg68;
    reg68->a[value1] -= 4;
    value3 = context2->a[value1];
    g_emu68_ea = value3;
    reg68_page_map_entry_t &memoryPage = context2->map[static_cast<uint8_t>(value3 >> 16)];
    result = memoryPage.read32(memoryPage.base + (value3 & memoryPage.mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadModifyDisplacementLong(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;
    reg68_page_map_entry_t *page5;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    g_emu68_ea = context4->a[value1] + value3;
    page5 = &context4->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = page5->read32(page5->base + (g_emu68_ea & page5->mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadModifyIndexedLong(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    reg68_page_map_entry_t *page7;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    g_emu68_ea = reg68->a[value1] + value5;
    page7 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = page7->read32(page7->base + (g_emu68_ea & page7->mask));
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_ReadModifyExtendedLong(int operand) {

    int64_t result;
    uint32_t value4;
    reg68_page_map_entry_t *page5;
    uint16_t value6;
    reg68_ctx_t *context7;

    reg68_page_map_entry_t *page9;
    uint32_t value10;
    uint32_t pc;
    reg68_page_map_entry_t *page12;
    int16_t value13;
    reg68_ctx_t *context14;
    uint32_t value15;
    reg68_page_map_entry_t *page16;
    uint32_t value17;
    reg68_ctx_t *context18;
    reg68_page_map_entry_t *page19;
    int16_t value20;
    reg68_ctx_t *context21;

    if (operand) {
        switch (operand) {
        case 1:
            page16 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
            value17 = page16->read32(page16->base + (reg68->pc & page16->mask));
            context18 = reg68;
            reg68->bus_word_count += 2;
            page9 = &context18->map[static_cast<uint8_t>((value17) >> 16)];
            context18->pc += 4;
            g_emu68_ea = value17;
            value10 = value17 & page9->mask;
            break;
        case 2:
            pc = reg68->pc;
            page12 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
            value13 = page12->read16(page12->base + (pc & page12->mask));
            context14 = reg68;
            value15 = pc + value13;
            g_emu68_ea = value15;
            ++reg68->bus_word_count;
            context14->pc += 2;
            page9 = &context14->map[static_cast<uint8_t>((value15) >> 16)];
            value10 = value15 & page9->mask;
            break;
        case 3:
            value4 = reg68->pc;
            page5 = &reg68->map[static_cast<uint8_t>((value4) >> 16)];
            value6 = page5->read16(page5->base + (value4 & page5->mask));
            context7 = reg68;
            ++reg68->bus_word_count;
            context7->pc += 2;
            g_emu68_ea = value4 + Emu68_DecodeBriefIndex(value6);
            page9 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
            value10 = g_emu68_ea & page9->mask;
            break;
        default:
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
            (Harrier_DebugPrint(g_emu68DebugText), 0);
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
            (Harrier_DebugPrint(g_emu68DebugText), 0);
            return 0;
        }
    } else {
        page19 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value20 = page19->read16(page19->base + (reg68->pc & page19->mask));
        context21 = reg68;
        g_emu68_ea = value20;
        ++reg68->bus_word_count;
        context21->pc += 2;
        page9 = &context21->map[(uint8_t)(value20 >> 15)];
        value10 = value20 & page9->mask;
    }
    result = page9->read32(page9->base + value10);
    reg68->bus_word_count += 2;
    return result;
}

std::intptr_t Emu68_WriteDataByte(int index, uint32_t value) {
    reg68_ctx_t *result;

    result = reg68;
    reg68->d[index] = (reg68->d[index] & ~uint32_t{0xFF}) | static_cast<uint8_t>(value);
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_ReportWriteModeError(int operand) {

    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
    return (Harrier_DebugPrint(g_emu68DebugText), 0);
}

std::intptr_t Emu68_WriteEffectiveByte(int index, uint32_t value) {
    reg68_page_map_entry_t *page0;
    reg68_ctx_t *result;

    page0 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    (page0->write8(page0->base + (g_emu68_ea & page0->mask), value), 0);
    result = reg68;
    ++reg68->bus_word_count;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WritePostincrementByte(int index, uint32_t value) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint32_t *value3;
    int64_t result;

    value1 = index;
    page2 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    (page2->write8(page2->base + (g_emu68_ea & page2->mask), value), 0);
    value3 = &reg68->a[value1];
    ++reg68->bus_word_count;
    result = (unsigned int)((uint32_t)value1 == 7) + 1;
    *value3 += result;
    return result;
}

std::intptr_t Emu68_WriteExtendedByte(int index, uint32_t value) {

    reg68_ctx_t *result;
    reg68_page_map_entry_t *page5;

    if (index < 2 || index - 2 < 2) {
        page5 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
        page5->write8(page5->base + (g_emu68_ea & page5->mask), value);
        result = reg68;
        ++reg68->bus_word_count;
    } else {
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", index);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
        return (Harrier_DebugPrint(g_emu68DebugText), 0);
    }
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WriteDataWord(int index, uint32_t value) {
    reg68_ctx_t *result;

    result = reg68;
    reg68->d[index] = (reg68->d[index] & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value);
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WriteAddressWord(int index, uint32_t value) {
    reg68_ctx_t *result;

    result = reg68;
    reg68->a[index] = static_cast<int16_t>(value);
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WriteEffectiveWord(int index, uint32_t value) {
    reg68_page_map_entry_t *page0;
    reg68_ctx_t *result;

    page0 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    (page0->write16(page0->base + (g_emu68_ea & page0->mask), value), 0);
    result = reg68;
    ++reg68->bus_word_count;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WritePostincrementWord(int index, uint32_t value) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int64_t result;
    reg68_ctx_t *context4;

    value1 = index;
    page2 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = (page2->write16(page2->base + (g_emu68_ea & page2->mask), value), 0);
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->a[value1] += 2;
    return result;
}

std::intptr_t Emu68_WriteExtendedWord(int index, uint32_t value) {

    reg68_ctx_t *result;
    reg68_page_map_entry_t *map;

    if (index < 2 || index - 2 < 2) {
        map = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
        map->write16(map->base + (g_emu68_ea & map->mask), value);
        result = reg68;
        ++reg68->bus_word_count;
    } else {
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", index);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
        return (Harrier_DebugPrint(g_emu68DebugText), 0);
    }
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WriteDataLong(int index, uint32_t value) {
    reg68_ctx_t *result;

    result = reg68;
    reg68->d[index] = value;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WriteAddressLong(int index, uint32_t value) {
    reg68_ctx_t *result;

    result = reg68;
    reg68->a[index] = value;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WriteEffectiveLong(int index, uint32_t value) {
    reg68_page_map_entry_t *page0;
    reg68_ctx_t *result;

    page0 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    (page0->write32(page0->base + (g_emu68_ea & page0->mask), value), 0);
    result = reg68;
    reg68->bus_word_count += 2;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_WritePostincrementLong(int index, uint32_t value) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int64_t result;
    reg68_ctx_t *context4;

    value1 = index;
    page2 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
    result = (page2->write32(page2->base + (g_emu68_ea & page2->mask), value), 0);
    context4 = reg68;
    reg68->bus_word_count += 2;
    context4->a[value1] += 4;
    return result;
}

std::intptr_t Emu68_WriteExtendedLong(int index, uint32_t value) {

    reg68_ctx_t *result;
    reg68_page_map_entry_t *page4;

    if (index < 2 || index - 2 < 2) {
        page4 = &reg68->map[static_cast<uint8_t>((g_emu68_ea) >> 16)];
        (page4->write32(page4->base + (g_emu68_ea & page4->mask), value), 0);
        result = reg68;
        reg68->bus_word_count += 2;
    } else {
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", index);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
        return (Harrier_DebugPrint(g_emu68DebugText), 0);
    }
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_SetEffectiveIndirect(int operand) {
    g_emu68_ea = reg68->a[operand];
    return reinterpret_cast<std::intptr_t>(reg68);
}

std::intptr_t Emu68_SetEffectivePredecrementByte(int operand) {
    uint32_t *value1;
    int64_t result;

    value1 = &reg68->a[operand];
    *value1 += -1 - (operand == 7);
    result = *value1;
    g_emu68_ea = *value1;
    return result;
}

std::intptr_t Emu68_SetEffectiveDisplacement(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    int16_t value3;
    reg68_ctx_t *context4;
    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    result = context4->a[value1] + value3;
    g_emu68_ea = result;
    return result;
}

std::intptr_t Emu68_SetEffectiveIndexed(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int value5;

    int64_t result;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    ++reg68->bus_word_count;
    context4->pc += 2;
    value5 = Emu68_DecodeBriefIndex(value3);
    result = (unsigned int)(reg68->a[value1] + value5);
    g_emu68_ea = result;
    return result;
}

std::intptr_t Emu68_SetEffectiveExtended(int operand) {

    uint32_t result;
    uint32_t value4;
    reg68_page_map_entry_t *page5;
    uint16_t value6;
    reg68_ctx_t *context7;
    uint32_t pc;
    reg68_page_map_entry_t *page9;
    int16_t value10;
    reg68_ctx_t *context11;
    reg68_page_map_entry_t *page12;
    reg68_ctx_t *context13;
    reg68_page_map_entry_t *page14;
    reg68_ctx_t *context15;

    if (operand) {
        switch (operand) {
        case 1:
            page12 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
            result = page12->read32(page12->base + (reg68->pc & page12->mask));
            context13 = reg68;
            g_emu68_ea = result;
            reg68->bus_word_count += 2;
            context13->pc += 4;
            break;
        case 2:
            pc = reg68->pc;
            page9 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
            value10 = page9->read16(page9->base + (pc & page9->mask));
            context11 = reg68;
            ++reg68->bus_word_count;
            context11->pc += 2;
            result = pc + value10;
            g_emu68_ea = result;
            break;
        case 3:
            value4 = reg68->pc;
            page5 = &reg68->map[static_cast<uint8_t>((value4) >> 16)];
            value6 = page5->read16(page5->base + (value4 & page5->mask));
            context7 = reg68;
            ++reg68->bus_word_count;
            context7->pc += 2;
            result = value4 + Emu68_DecodeBriefIndex(value6);
            g_emu68_ea = result;
            break;
        default:
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "ADR", operand);
            (Harrier_DebugPrint(g_emu68DebugText), 0);
            std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
            return (Harrier_DebugPrint(g_emu68DebugText), 0);
        }
    } else {
        page14 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        result = static_cast<uint16_t>(page14->read16(page14->base + (reg68->pc & page14->mask)));
        context15 = reg68;
        result = (int16_t)result;
        g_emu68_ea = (int16_t)result;
        ++reg68->bus_word_count;
        context15->pc += 2;
    }
    return result;
}

std::intptr_t Emu68_SetEffectivePredecrementWord(int operand) {
    reg68_ctx_t *context1;
    int64_t result;

    context1 = reg68;
    reg68->a[operand] -= 2;
    result = context1->a[operand];
    g_emu68_ea = result;
    return result;
}

std::intptr_t Emu68_SetEffectivePredecrementLong(int operand) {
    reg68_ctx_t *context1;
    int64_t result;

    context1 = reg68;
    reg68->a[operand] -= 4;
    result = context1->a[operand];
    g_emu68_ea = result;
    return result;
}

std::intptr_t Emu68_CompareFlags(int value, int destination, int signMask) {
    reg68_ctx_t *context3;
    int64_t result;
    int8_t value5;

    context3 = reg68;
    reg68->sr.raw[0] &= 0xF0u;
    result = 0;
    const int difference = Emu68_Subtract32(destination, value);
    value5 = ((signMask & difference) != 0 ? 8 : 0) | (((value & difference | ~destination & (value | difference)) & signMask) != 0) |
        ((signMask & (value & difference & ~destination | destination & ~(value | difference))) != 0 ? 2 : 0);
    if (destination == value)
        result = 4;
    context3->sr.raw[0] |= (uint8_t)result | value5;
    return result;
}

std::intptr_t Emu68_PushLong(unsigned int operand) {
    reg68_ctx_t *context1;
    reg68_page_map_entry_t *page2;
    reg68_ctx_t *result;

    context1 = reg68;
    reg68->a[7] -= 4;
    page2 = &context1->map[(uint8_t) static_cast<uint8_t>((context1->a[7]) >> 16)];
    (page2->write32(page2->base + (context1->a[7] & page2->mask), operand), 0);
    result = reg68;
    reg68->bus_word_count += 2;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_PopWord() {
    reg68_page_map_entry_t *page0;
    int64_t result;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[7]) >> 16)];
    result = page0->read16(page0->base + (reg68->a[7] & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->a[7] += 2;
    return result;
}

std::intptr_t Emu68_PopLong() {
    reg68_page_map_entry_t *page0;
    int64_t result;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[7]) >> 16)];
    result = page0->read32(page0->base + (reg68->a[7] & page0->mask));
    context2 = reg68;
    reg68->bus_word_count += 2;
    context2->a[7] += 4;
    return result;
}

std::intptr_t Emu68_SetStatusRegister(reg68_sr_bits_t operand) {
    reg68_ctx_t *context1;

    context1 = reg68;
    if ((reg68->sr.raw[1] & 0x20) == 0) {
        emu68_HandleInterruptRequest(8u);
        context1 = reg68;
    }
    context1->sr = operand;
    return operand.bits >> 8;
}

std::intptr_t Emu68_AddToEffectiveByte(int8_t operand) {
    int8_t value5;
    reg68_ctx_t *context6;
    int value7;
    int8_t value8;
    int64_t result;

    value5 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, operand + value5);
    context6 = reg68;
    value7 = (int8_t)(operand + value5);
    value8 = ~(operand + value5);
    reg68->sr.raw[0] &= 0xECu;
    if (((value7 & ~(value5 | operand) | value5 & operand & value8) & 0x80u) != 0)
        context6->sr.raw[0] |= 2u;
    if (((value5 & operand | value8 & (value5 | operand)) & 0x80u) != 0)
        context6->sr.raw[0] |= 0x11u;
    context6->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value7)
        result = 4;
    context6->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value7 < 0));
    return result;
}

std::intptr_t Emu68_AddToEffectiveWord(int16_t operand) {
    int16_t value2;
    reg68_ctx_t *context3;
    int value4;
    int16_t value5;
    int64_t result;

    value2 = kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, operand + value2);
    context3 = reg68;
    value4 = (int16_t)(operand + value2);
    value5 = ~(operand + value2);
    reg68->sr.raw[0] &= 0xECu;
    if ((((uint16_t)(value4 & ~(value2 | operand)) | (uint16_t)(value2 & operand & value5)) & 0x8000) != 0)
        context3->sr.raw[0] |= 2u;
    if ((((uint16_t)(value2 & operand) | (uint16_t)(value5 & (value2 | operand))) & 0x8000) != 0)
        context3->sr.raw[0] |= 0x11u;
    context3->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value4)
        result = 4;
    context3->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value4 < 0));
    return result;
}

std::intptr_t Emu68_AddToEffectiveLong(int operand) {
    int value2;
    int value3;
    reg68_ctx_t *context4;
    int64_t result;

    value2 = kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value3 = Emu68_Add32(value2, operand);
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)(Emu68_Add32(value2, operand)));
    context4 = reg68;
    reg68->sr.raw[0] &= 0xECu;
    if ((operand & value2 & ~value3 | value3 & ~(operand | value2)) < 0)
        context4->sr.raw[0] |= 2u;
    if ((operand & value2 | ~(Emu68_Add32(value2, operand)) & (operand | value2)) < 0)
        context4->sr.raw[0] |= 0x11u;
    context4->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value3)
        result = 4;
    context4->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value3 < 0));
    return result;
}

std::intptr_t Emu68_SubtractFromEffectiveByte(int8_t operand) {
    int8_t value5;
    reg68_ctx_t *context6;
    int8_t value7;
    int value8;
    int8_t value9;
    int8_t value10;
    int64_t result;

    value5 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value5 - operand);
    context6 = reg68;
    value7 = value5;
    value8 = (int8_t)(value5 - operand);
    value9 = (value5 - operand) | operand;
    value10 = ~value5;
    reg68->sr.raw[0] &= 0xECu;
    if (((value7 & ~(value8 | operand) | value8 & operand & value10) & 0x80u) != 0)
        context6->sr.raw[0] |= 2u;
    if (((value8 & operand | value10 & value9) & 0x80u) != 0)
        context6->sr.raw[0] |= 0x11u;
    context6->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value8)
        result = 4;
    context6->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value8 < 0));
    return result;
}

std::intptr_t Emu68_SubtractFromEffectiveWord(int16_t operand) {
    int16_t value2;
    reg68_ctx_t *context3;
    int16_t value4;
    int value5;
    int16_t value6;
    int16_t value7;
    int64_t result;

    value2 = kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, Emu68_Subtract32(value2, operand));
    context3 = reg68;
    value4 = value2;
    value5 = (int16_t)(Emu68_Subtract32(value2, operand));
    value6 = (Emu68_Subtract32(value2, operand)) | operand;
    value7 = ~value2;
    reg68->sr.raw[0] &= 0xECu;
    if ((((uint16_t)(value4 & ~(value5 | operand)) | (uint16_t)(value5 & operand & value7)) & 0x8000) != 0)
        context3->sr.raw[0] |= 2u;
    if ((((uint16_t)(value5 & operand) | (uint16_t)(value7 & value6)) & 0x8000) != 0)
        context3->sr.raw[0] |= 0x11u;
    context3->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value5)
        result = 4;
    context3->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value5 < 0));
    return result;
}

std::intptr_t Emu68_SubtractFromEffectiveLong(int operand) {
    int value2;
    int value3;
    int value4;
    reg68_ctx_t *context5;
    int64_t result;

    value2 = kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value3 = Emu68_Subtract32(value2, operand);
    value4 = value2;
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)(Emu68_Subtract32(value2, operand)));
    context5 = reg68;
    reg68->sr.raw[0] &= 0xECu;
    if ((operand & value3 & ~value4 | value4 & ~(operand | value3)) < 0)
        context5->sr.raw[0] |= 2u;
    if ((operand & value3 | ~value4 & (operand | value3)) < 0)
        context5->sr.raw[0] |= 0x11u;
    context5->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value3)
        result = 4;
    context5->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value3 < 0));
    return result;
}

std::intptr_t Emu68_AddDataToEffectiveByte() {
    int8_t value4;
    int8_t value5;
    reg68_ctx_t *context6;
    int value7;
    int8_t value8;
    int64_t result;

    value4 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    value5 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value4 + value5);
    context6 = reg68;
    value7 = (int8_t)(value4 + value5);
    value8 = ~(value4 + value5);
    reg68->sr.raw[0] &= 0xECu;
    if (((value7 & ~(value5 | value4) | value5 & value4 & value8) & 0x80u) != 0)
        context6->sr.raw[0] |= 2u;
    if (((value5 & value4 | value8 & (value5 | value4)) & 0x80u) != 0)
        context6->sr.raw[0] |= 0x11u;
    context6->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value7)
        result = 4;
    context6->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value7 < 0));
    return result;
}

std::intptr_t Emu68_AddDataToEffectiveWord() {
    int16_t value0;
    int16_t value1;
    reg68_ctx_t *context2;
    int value3;
    int16_t value4;
    int64_t result;

    value0 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    value1 = kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value0 + value1);
    context2 = reg68;
    value3 = (int16_t)(value0 + value1);
    value4 = ~(value0 + value1);
    reg68->sr.raw[0] &= 0xECu;
    if ((((uint16_t)(value3 & ~(value1 | value0)) | (uint16_t)(value1 & value0 & value4)) & 0x8000) != 0)
        context2->sr.raw[0] |= 2u;
    if ((((uint16_t)(value1 & value0) | (uint16_t)(value4 & (value1 | value0))) & 0x8000) != 0)
        context2->sr.raw[0] |= 0x11u;
    context2->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value3)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value3 < 0));
    return result;
}

std::intptr_t Emu68_AddDataToEffectiveLong() {
    uint32_t value0;
    int value1;
    int value2;
    reg68_ctx_t *context3;
    int64_t result;

    value0 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    value1 = kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value2 = value1 + value0;
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (value1 + value0));
    context3 = reg68;
    reg68->sr.raw[0] &= 0xECu;
    if (((value0 & value1 & ~value2 | value2 & ~(value0 | value1)) & 0x80000000) != 0)
        context3->sr.raw[0] |= 2u;
    if (((value0 & value1 | ~(value1 + value0) & (value0 | value1)) & 0x80000000) != 0)
        context3->sr.raw[0] |= 0x11u;
    context3->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value2)
        result = 4;
    context3->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value2 < 0));
    return result;
}

std::intptr_t Emu68_SubtractDataFromEffectiveByte() {
    int8_t value4;
    int8_t value5;
    reg68_ctx_t *context6;
    int8_t value7;
    int value8;
    int8_t value9;
    int8_t value10;
    int64_t result;

    value4 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    value5 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value5 - value4);
    context6 = reg68;
    value7 = value5;
    value8 = (int8_t)(value5 - value4);
    value9 = (value5 - value4) | value4;
    value10 = ~value5;
    reg68->sr.raw[0] &= 0xECu;
    if (((value7 & ~(value8 | value4) | value8 & value4 & value10) & 0x80u) != 0)
        context6->sr.raw[0] |= 2u;
    if (((value8 & value4 | value10 & value9) & 0x80u) != 0)
        context6->sr.raw[0] |= 0x11u;
    context6->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value8)
        result = 4;
    context6->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value8 < 0));
    return result;
}

std::intptr_t Emu68_SubtractDataFromEffectiveWord() {
    int16_t value0;
    int16_t value1;
    reg68_ctx_t *context2;
    int16_t value3;
    int value4;
    int16_t value5;
    int16_t value6;
    int64_t result;

    value0 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    value1 = kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value1 - value0);
    context2 = reg68;
    value3 = value1;
    value4 = (int16_t)(value1 - value0);
    value5 = (value1 - value0) | value0;
    value6 = ~value1;
    reg68->sr.raw[0] &= 0xECu;
    if ((((uint16_t)(value3 & ~(value4 | value0)) | (uint16_t)(value4 & value0 & value6)) & 0x8000) != 0)
        context2->sr.raw[0] |= 2u;
    if ((((uint16_t)(value4 & value0) | (uint16_t)(value6 & value5)) & 0x8000) != 0)
        context2->sr.raw[0] |= 0x11u;
    context2->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value4)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value4 < 0));
    return result;
}

std::intptr_t Emu68_SubtractDataFromEffectiveLong() {
    uint32_t value0;
    int value1;
    int value2;
    int value3;
    reg68_ctx_t *context4;
    int64_t result;

    value0 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    value1 = kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value2 = value1 - value0;
    value3 = value1;
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (value1 - value0));
    context4 = reg68;
    reg68->sr.raw[0] &= 0xECu;
    if (((value0 & value2 & ~value3 | value3 & ~(value0 | value2)) & 0x80000000) != 0)
        context4->sr.raw[0] |= 2u;
    if (((value0 & value2 | ~value3 & (value0 | value2)) & 0x80000000) != 0)
        context4->sr.raw[0] |= 0x11u;
    context4->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value2)
        result = 4;
    context4->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value2 < 0));
    return result;
}

std::intptr_t Emu68_AddEffectiveToDataByte() {
    uint64_t value4;
    int8_t value5;
    reg68_ctx_t *context6;
    int64_t value7;
    int8_t value8;
    int8_t value9;
    int64_t result;

    value4 = (uint16_t)g_emu68Instruction;
    value5 = (int8_t)kEmu68_ReadByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    context6 = reg68;
    value7 = (value4 >> 9) & 7;
    value8 = reg68->d[value7];
    value9 = value5 + value8;
    reg68->d[value7] = (reg68->d[value7] & ~uint32_t{0xFF}) | static_cast<uint8_t>(value5 + value8);
    context6->sr.raw[0] &= 0xECu;
    if (((value9 & ~(value8 | value5) | value8 & value5 & ~value9) & 0x80u) != 0)
        context6->sr.raw[0] |= 2u;
    if (((value8 & value5 | ~value9 & (value8 | value5)) & 0x80u) != 0)
        context6->sr.raw[0] |= 0x11u;
    context6->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value9)
        result = 4;
    context6->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value9 < 0));
    return result;
}

std::intptr_t Emu68_AddEffectiveToDataWord() {
    uint64_t value0;
    int16_t value1;
    reg68_ctx_t *context2;
    int64_t value3;
    int16_t value4;
    int16_t value5;
    int64_t result;

    value0 = (uint16_t)g_emu68Instruction;
    value1 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context2 = reg68;
    value3 = (value0 >> 9) & 7;
    value4 = reg68->d[value3];
    value5 = value1 + value4;
    reg68->d[value3] = (reg68->d[value3] & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value1 + value4);
    context2->sr.raw[0] &= 0xECu;
    if ((((uint16_t)(value5 & ~(value4 | value1)) | (uint16_t)(value4 & value1 & ~value5)) & 0x8000) != 0)
        context2->sr.raw[0] |= 2u;
    if ((((uint16_t)(value4 & value1) | (uint16_t)(~value5 & (value4 | value1))) & 0x8000) != 0)
        context2->sr.raw[0] |= 0x11u;
    context2->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value5)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value5 < 0));
    return result;
}

std::intptr_t Emu68_AddEffectiveToDataLong() {
    uint64_t value0;
    int value1;
    reg68_ctx_t *context2;
    int64_t value3;
    uint32_t value4;
    int value5;
    int64_t result;

    value0 = (uint16_t)g_emu68Instruction;
    value1 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context2 = reg68;
    value3 = (value0 >> 9) & 7;
    value4 = reg68->d[value3];
    value5 = value4 + value1;
    reg68->d[value3] = value4 + value1;
    context2->sr.raw[0] &= 0xECu;
    if (((value1 & value4 & ~(value4 + value1) | (value4 + value1) & ~(value1 | value4)) & 0x80000000) != 0)
        context2->sr.raw[0] |= 2u;
    if (((value1 & value4 | ~(value4 + value1) & (value1 | value4)) & 0x80000000) != 0)
        context2->sr.raw[0] |= 0x11u;
    context2->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value5)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value5 < 0));
    return result;
}

std::intptr_t Emu68_SubtractEffectiveFromDataByte() {
    uint64_t value4;
    int8_t value5;
    reg68_ctx_t *context6;
    int64_t value7;
    int8_t value8;
    int8_t value9;
    int64_t result;

    value4 = (uint16_t)g_emu68Instruction;
    value5 = (int8_t)kEmu68_ReadByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    context6 = reg68;
    value7 = (value4 >> 9) & 7;
    value8 = reg68->d[value7];
    value9 = value8 - value5;
    reg68->d[value7] = (reg68->d[value7] & ~uint32_t{0xFF}) | static_cast<uint8_t>(value8 - value5);
    context6->sr.raw[0] &= 0xECu;
    if (((value8 & ~(value9 | value5) | value9 & value5 & ~value8) & 0x80u) != 0)
        context6->sr.raw[0] |= 2u;
    if (((value9 & value5 | ~value8 & (value9 | value5)) & 0x80u) != 0)
        context6->sr.raw[0] |= 0x11u;
    context6->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value9)
        result = 4;
    context6->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value9 < 0));
    return result;
}

std::intptr_t Emu68_SubtractEffectiveFromDataWord() {
    uint64_t value0;
    int16_t value1;
    reg68_ctx_t *context2;
    int64_t value3;
    int16_t value4;
    int16_t value5;
    int64_t result;

    value0 = (uint16_t)g_emu68Instruction;
    value1 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context2 = reg68;
    value3 = (value0 >> 9) & 7;
    value4 = reg68->d[value3];
    value5 = value4 - value1;
    reg68->d[value3] = (reg68->d[value3] & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value4 - value1);
    context2->sr.raw[0] &= 0xECu;
    if ((((uint16_t)(value4 & ~(value5 | value1)) | (uint16_t)(value5 & value1 & ~value4)) & 0x8000) != 0)
        context2->sr.raw[0] |= 2u;
    if ((((uint16_t)(value5 & value1) | (uint16_t)(~value4 & (value5 | value1))) & 0x8000) != 0)
        context2->sr.raw[0] |= 0x11u;
    context2->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value5)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value5 < 0));
    return result;
}

std::intptr_t Emu68_SubtractEffectiveFromDataLong() {
    uint64_t value0;
    int value1;
    reg68_ctx_t *context2;
    int64_t value3;
    uint32_t value4;
    int value5;
    int64_t result;

    value0 = (uint16_t)g_emu68Instruction;
    value1 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context2 = reg68;
    value3 = (value0 >> 9) & 7;
    value4 = reg68->d[value3];
    value5 = value4 - value1;
    reg68->d[value3] = value4 - value1;
    context2->sr.raw[0] &= 0xECu;
    if (((value1 & value5 & ~value4 | value4 & ~(value1 | value5)) & 0x80000000) != 0)
        context2->sr.raw[0] |= 2u;
    if (((value1 & value5 | ~value4 & (value1 | value5)) & 0x80000000) != 0)
        context2->sr.raw[0] |= 0x11u;
    context2->sr.raw[0] &= 0xF3u;
    result = 0;
    if (!value5)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value5 < 0));
    return result;
}

std::intptr_t Emu68_AndEffectiveByte(int8_t operand) {
    int8_t value4;
    reg68_ctx_t *context5;
    int64_t result;

    value4 = operand & (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value4);
    context5 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value4)
        result = 4;
    context5->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value4 < 0));
    return result;
}

std::intptr_t Emu68_AndEffectiveWord(int16_t operand) {
    int16_t value1;
    reg68_ctx_t *context2;
    int64_t result;

    value1 = operand & kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value1);
    context2 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value1)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value1 < 0));
    return result;
}

std::intptr_t Emu68_AndEffectiveLong(int operand) {
    int value1;
    reg68_ctx_t *context2;
    int64_t result;

    value1 = operand & kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)value1);
    context2 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value1)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value1 < 0));
    return result;
}

std::intptr_t Emu68_OrEffectiveByte(int8_t operand) {
    int8_t value4;
    reg68_ctx_t *context5;
    int64_t result;

    value4 = operand | (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value4);
    context5 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value4)
        result = 4;
    context5->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value4 < 0));
    return result;
}

std::intptr_t Emu68_OrEffectiveWord(int16_t operand) {
    int16_t value1;
    reg68_ctx_t *context2;
    int64_t result;

    value1 = operand | kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value1);
    context2 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value1)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value1 < 0));
    return result;
}

std::intptr_t Emu68_OrEffectiveLong(int operand) {
    int value1;
    reg68_ctx_t *context2;
    int64_t result;

    value1 = operand | kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)value1);
    context2 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value1)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value1 < 0));
    return result;
}

std::intptr_t Emu68_XorEffectiveByte(int8_t operand) {
    int8_t value4;
    reg68_ctx_t *context5;
    int64_t result;

    value4 = operand ^ (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value4);
    context5 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value4)
        result = 4;
    context5->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value4 < 0));
    return result;
}

std::intptr_t Emu68_XorEffectiveWord(int16_t operand) {
    int16_t value1;
    reg68_ctx_t *context2;
    int64_t result;

    value1 = operand ^ kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value1);
    context2 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value1)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value1 < 0));
    return result;
}

std::intptr_t Emu68_XorEffectiveLong(int operand) {
    int value1;
    reg68_ctx_t *context2;
    int64_t result;

    value1 = operand ^ kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)value1);
    context2 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value1)
        result = 4;
    context2->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value1 < 0));
    return result;
}

std::intptr_t Emu68_NegateEffectiveByte() {
    int8_t value4;
    reg68_sr_bits_t *p_sr;
    int8_t value6;
    int8_t value7;
    int8_t result;

    value4 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, -value4);
    p_sr = &reg68->sr;
    if ((-value4 & value4 & 0x80u) == 0)
        p_sr->raw[0] &= ~2u;
    else
        p_sr->raw[0] |= 2u;
    value6 = 0;
    if (!value4)
        value6 = 4;
    value7 = value6 | (8 * ((-value4 & 0x80u) != 0));
    result = p_sr->raw[0] & 0xF3;
    p_sr->raw[0] = result | value7 | 0x11;
    return result;
}

std::intptr_t Emu68_NegateEffectiveWord() {
    uint16_t value0;
    reg68_sr_bits_t *p_sr;
    int8_t value2;
    int8_t value3;
    int8_t result;

    value0 = kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, Emu68_Subtract32(0, value0));
    p_sr = &reg68->sr;
    if (((uint16_t)Emu68_Subtract32(0, value0) & value0 & 0x8000) != 0)
        p_sr->raw[0] |= 2u;
    else
        p_sr->raw[0] &= ~2u;
    value2 = 0;
    if (!value0)
        value2 = 4;
    value3 = value2 | (8 * ((Emu68_Subtract32(0, value0) & 0x8000u) != 0));
    result = p_sr->raw[0] & 0xF3;
    p_sr->raw[0] = result | value3 | 0x11;
    return result;
}

std::intptr_t Emu68_NegateEffectiveLong() {
    int value0;
    int value1;
    int value2;
    reg68_sr_bits_t *p_sr;
    int8_t value4;
    int8_t value5;
    int8_t result;

    value0 = kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value1 = Emu68_Subtract32(0, value0);
    value2 = value0;
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)Emu68_Subtract32(0, value0));
    p_sr = &reg68->sr;
    if ((value1 & value2) >= 0)
        p_sr->raw[0] &= ~2u;
    else
        p_sr->raw[0] |= 2u;
    value4 = 0;
    if (!value1)
        value4 = 4;
    value5 = value4 | (8 * (value1 < 0));
    result = p_sr->raw[0] & 0xF3;
    p_sr->raw[0] = result | value5 | 0x11;
    return result;
}

std::intptr_t Emu68_NegateExtendEffectiveByte() {
    int8_t value4;
    int8_t value5;
    reg68_sr_bits_t *result;
    bool value7;

    value4 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    value5 = -(int8_t)(value4 + ((reg68->sr.raw[0] & 0x10) != 0));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value5);
    result = &reg68->sr;
    if ((value5 & value4 & 0x80u) == 0)
        reg68->sr.raw[0] &= ~2u;
    else
        reg68->sr.raw[0] |= 2u;
    result->raw[0] |= 0x11u;
    if ((result->raw[0] & 4) != 0) {
        value7 = value5 < 0;
        if (!value5)
            goto LABEL_8;
        result->raw[0] &= ~4u;
    }
    value7 = value5 < 0;
LABEL_8:
    if (value7)
        result->raw[0] |= 8u;
    else
        result->raw[0] &= ~8u;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_NegateExtendEffectiveWord() {
    uint16_t value0;
    uint16_t value1;
    reg68_sr_bits_t *result;
    bool value3;

    value0 = kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value1 = -(int16_t)(value0 + ((reg68->sr.raw[0] & 0x10) != 0));
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value1);
    result = &reg68->sr;
    if ((value1 & value0 & 0x8000) != 0)
        reg68->sr.raw[0] |= 2u;
    else
        reg68->sr.raw[0] &= ~2u;
    result->raw[0] |= 0x11u;
    if ((result->raw[0] & 4) != 0) {
        value3 = (value1 & 0x8000u) != 0;
        if (!value1)
            goto LABEL_8;
        result->raw[0] &= ~4u;
    }
    value3 = (value1 & 0x8000u) != 0;
LABEL_8:
    if (value3)
        result->raw[0] |= 8u;
    else
        result->raw[0] &= ~8u;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_NegateExtendEffectiveLong() {
    int value0;
    int value1;
    reg68_sr_bits_t *result;
    bool value3;

    value0 = kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value1 = Emu68_Subtract32(0, Emu68_Add32(value0, (((uint8_t)reg68->sr.raw[0] >> 4) & 1)));
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)value1);
    result = &reg68->sr;
    if ((value1 & value0) >= 0)
        result->raw[0] &= ~2u;
    else
        result->raw[0] |= 2u;
    result->raw[0] |= 0x11u;
    if ((result->raw[0] & 4) != 0) {
        value3 = value1 < 0;
        if (!value1)
            goto LABEL_8;
        result->raw[0] &= ~4u;
    }
    value3 = value1 < 0;
LABEL_8:
    if (value3)
        result->raw[0] |= 8u;
    else
        result->raw[0] &= ~8u;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_NotEffectiveByte() {
    int8_t value4;
    reg68_ctx_t *context5;
    int64_t result;

    value4 = ~(int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value4);
    context5 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value4)
        result = 4;
    context5->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value4 < 0));
    return result;
}

std::intptr_t Emu68_NotEffectiveWord() {
    int16_t value0;
    reg68_ctx_t *context1;
    int64_t result;

    value0 = ~(uint16_t)kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value0);
    context1 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value0)
        result = 4;
    context1->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value0 < 0));
    return result;
}

std::intptr_t Emu68_NotEffectiveLong() {
    signed int value0;
    reg68_ctx_t *context1;
    int64_t result;

    value0 = ~kEmu68_ReadModifyLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), (unsigned int)value0);
    context1 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value0)
        result = 4;
    context1->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value0 < 0));
    return result;
}

std::intptr_t Emu68_AddBcd() {
    uint8_t value4;
    uint8_t value5;
    int8_t value6;
    uint8_t value7;
    int8_t value8;
    int8_t value9;
    reg68_ctx_t *context10;
    int64_t result;

    value4 = (int8_t)kEmu68_CompareMemoryByteSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1]((g_emu68Instruction & 7));
    value5 = kEmu68_PredecrementByteDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value6 = reg68->sr.raw[0];
    value7 = (value4 & 0xF) + (value5 & 0xF) + ((value6 & 0x10) != 0) + 10 * ((value4 >> 4) + (value5 >> 4));
    if (value7 < 0x64u) {
        value8 = value6 & 0xEE;
    } else {
        value8 = value6 | 0x11;
        value7 -= 100;
    }
    reg68->sr.raw[0] = value8;
    value9 = (value7 % 0xAu) | (16 * (value7 / 0xAu));
    kEmu68_WriteBytePair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value9);
    context10 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF3u;
    if (!value9)
        result = 4;
    context10->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value9 < 0));
    return result;
}

std::intptr_t Emu68_SubtractBcd() {
    uint8_t value4;
    uint8_t value5;
    int8_t value6;
    int8_t value7;
    int8_t value8;
    int8_t value9;
    reg68_ctx_t *context10;
    int64_t result;

    value4 = (int8_t)kEmu68_CompareMemoryByteSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1]((g_emu68Instruction & 7));
    value5 = kEmu68_PredecrementByteDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value6 = reg68->sr.raw[0];
    value7 = (value5 & 0xF) + 10 * ((value5 >> 4) - (value4 >> 4)) - ((value6 & 0x10) != 0) - (value4 & 0xF);
    if (value7 >= 0) {
        value8 = value6 & 0xEE;
    } else {
        value8 = value6 | 0x11;
        value7 += 100;
    }
    reg68->sr.raw[0] = value8;
    value9 = ((uint8_t)value7 % 0xAu) | (16 * ((uint8_t)value7 / 0xAu));
    kEmu68_WriteBytePair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value9);
    context10 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF3u;
    if (!value9)
        result = 4;
    context10->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value9 < 0));
    return result;
}

std::intptr_t Emu68_NegateBcd() {
    uint8_t value4;
    int8_t value5;
    int8_t value6;
    int8_t value7;
    int8_t value8;
    reg68_ctx_t *context9;
    int64_t result;

    value4 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    value5 = reg68->sr.raw[0];
    value6 = -10 * (value4 >> 4) - ((value5 & 0x10) != 0) - (value4 & 0xF);
    if (value6 >= 0) {
        value7 = value5 & 0xEE;
    } else {
        value7 = value5 | 0x11;
        value6 += 100;
    }
    reg68->sr.raw[0] = value7;
    value8 = ((uint8_t)value6 % 0xAu) | (16 * ((uint8_t)value6 / 0xAu));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value8);
    context9 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF3u;
    if (!value8)
        result = 4;
    context9->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value8 < 0));
    return result;
}

std::intptr_t Emu68_AddExtendByte() {
    unsigned int value4;
    int8_t value5;
    int value6;
    int8_t value7;
    reg68_ctx_t *context8;
    int8_t value9;
    int value10;
    int8_t value11;
    bool value12;
    int8_t value13;
    int8_t value14;
    int64_t result;

    value4 = (int8_t)kEmu68_CompareMemoryByteSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1]((g_emu68Instruction & 7));
    value5 = kEmu68_PredecrementByteDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value6 = value5;
    value7 = value4 + value5 + ((reg68->sr.raw[0] & 0x10) != 0);
    kEmu68_WriteBytePair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value7);
    context8 = reg68;
    value9 = 0;
    value10 = value7;
    value11 = 8 * (value7 < 0);
    value12 = value7 == 0;
    value13 = ~value7;
    if (value12)
        value9 = 4;
    value14 = reg68->sr.raw[0] & 0xE0 | (value9 | value11) & 0xEC;
    result = value10 & ~(value6 | value4);
    reg68->sr.raw[0] = value14;
    if (((value10 & ~(value6 | value4) | value6 & value4 & value13) & 0x80u) != 0)
        context8->sr.raw[0] = value14 | 2;
    if (((value6 & value4 | value13 & (value6 | value4)) & 0x80u) != 0)
        context8->sr.raw[0] |= 0x11u;
    return result;
}

std::intptr_t Emu68_AddExtendWord() {
    int16_t value4;
    unsigned int value5;
    int16_t value6;
    int value7;
    int16_t value8;
    reg68_ctx_t *context9;
    int8_t value10;
    int value11;
    int8_t value12;
    uint16_t value13;
    int8_t value14;
    int64_t result;

    value4 = static_cast<uint8_t>((int8_t)kEmu68_CompareMemoryWordSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1]((g_emu68Instruction & 7)));
    value5 = value4;
    value6 = kEmu68_PredecrementWordDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value7 = value6;
    value8 = value5 + value6 + ((reg68->sr.raw[0] & 0x10) != 0);
    kEmu68_WriteWordPair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value8);
    context9 = reg68;
    value10 = 0;
    value11 = value8;
    value12 = 8 * (value8 < 0);
    value13 = ~value8;
    if (!value11)
        value10 = 4;
    value14 = reg68->sr.raw[0] & 0xE0 | (value10 | value12) & 0xEC;
    result = value11 & ~(value7 | value5);
    reg68->sr.raw[0] = value14;
    if ((((uint16_t)(value11 & ~(value7 | value5)) | value7 & value5 & value13) & 0x8000) != 0)
        context9->sr.raw[0] = value14 | 2;
    if ((((uint16_t)(value7 & value5) | value13 & (value7 | value5)) & 0x8000) != 0)
        context9->sr.raw[0] |= 0x11u;
    return result;
}

std::intptr_t Emu68_AddExtendLong() {
    int value0;
    int value1;
    int value2;
    reg68_ctx_t *context3;
    int8_t value4;
    int value5;
    int8_t value6;
    int64_t result;

    value0 = kEmu68_CompareMemoryLongSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](g_emu68Instruction & 7);
    value1 = kEmu68_PredecrementLongDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value2 = Emu68_Add32(Emu68_Add32(value0, value1), (((uint8_t)reg68->sr.raw[0] >> 4) & 1));
    kEmu68_WriteLongPair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value2);
    context3 = reg68;
    value4 = 0;
    value5 = ~value2;
    if (!value2)
        value4 = 4;
    value6 = reg68->sr.raw[0] & 0xE0 | (value4 | (8 * (value2 < 0))) & 0xEC;
    result = value0 & value1 & (unsigned int)value5;
    reg68->sr.raw[0] = value6;
    if ((int)(result | value2 & ~(value0 | value1)) < 0)
        context3->sr.raw[0] = value6 | 2;
    if ((value0 & value1 | value5 & (value0 | value1)) < 0)
        context3->sr.raw[0] |= 0x11u;
    return result;
}

std::intptr_t Emu68_SubtractExtendByte() {
    unsigned int value4;
    int8_t value5;
    int value6;
    int8_t value7;
    reg68_ctx_t *context8;
    int8_t value9;
    int value10;
    int8_t value11;
    int8_t value12;
    int64_t result;

    value4 = (int8_t)kEmu68_CompareMemoryByteSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1]((g_emu68Instruction & 7));
    value5 = kEmu68_PredecrementByteDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value6 = value5;
    value7 = value5 - ((reg68->sr.raw[0] & 0x10) != 0) - value4;
    kEmu68_WriteBytePair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value7);
    context8 = reg68;
    value9 = 0;
    value10 = value6;
    value11 = ~(uint8_t)value6;
    if (!value7)
        value9 = 4;
    value12 = reg68->sr.raw[0] & 0xE0 | (value9 | (8 * (value7 < 0))) & 0xEC;
    result = value10 & ~(value7 | value4);
    reg68->sr.raw[0] = value12;
    if (((value10 & ~(value7 | value4) | value7 & value4 & value11) & 0x80u) != 0)
        context8->sr.raw[0] = value12 | 2;
    if (((value7 & value4 | value11 & (value7 | value4)) & 0x80u) != 0)
        context8->sr.raw[0] |= 0x11u;
    return result;
}

std::intptr_t Emu68_SubtractExtendWord() {
    int16_t value4;
    unsigned int value5;
    int16_t value6;
    int value7;
    int16_t value8;
    reg68_ctx_t *context9;
    int8_t value10;
    int value11;
    uint16_t value12;
    int8_t value13;
    int64_t result;

    value4 = static_cast<uint8_t>((int8_t)kEmu68_CompareMemoryWordSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1]((g_emu68Instruction & 7)));
    value5 = value4;
    value6 = kEmu68_PredecrementWordDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value7 = value6;
    value8 = value6 - ((reg68->sr.raw[0] & 0x10) != 0) - value5;
    kEmu68_WriteWordPair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value8);
    context9 = reg68;
    value10 = 0;
    value11 = value7;
    value12 = ~(uint16_t)value7;
    if (!value8)
        value10 = 4;
    value13 = reg68->sr.raw[0] & 0xE0 | (value10 | (8 * (value8 < 0))) & 0xEC;
    result = value11 & ~(value8 | value5);
    reg68->sr.raw[0] = value13;
    if ((((uint16_t)(value11 & ~(value8 | value5)) | value8 & value5 & value12) & 0x8000) != 0)
        context9->sr.raw[0] = value13 | 2;
    if ((((uint16_t)(value8 & value5) | value12 & (value8 | value5)) & 0x8000) != 0)
        context9->sr.raw[0] |= 0x11u;
    return result;
}

std::intptr_t Emu68_SubtractExtendLong() {
    int value0;
    int value1;
    int value2;
    reg68_ctx_t *context3;
    int8_t value4;
    int value5;
    int8_t value6;
    int64_t result;

    value0 = kEmu68_CompareMemoryLongSource[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](g_emu68Instruction & 7);
    value1 = kEmu68_PredecrementLongDestination[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7);
    value2 = Emu68_Subtract32(Emu68_Subtract32(value1, (((uint8_t)reg68->sr.raw[0] >> 4) & 1)), value0);
    kEmu68_WriteLongPair[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 1](((uint16_t)g_emu68Instruction >> 9) & 7, value2);
    context3 = reg68;
    value4 = 0;
    value5 = ~value1;
    if (!value2)
        value4 = 4;
    value6 = reg68->sr.raw[0] & 0xE0 | (value4 | (8 * (value2 < 0))) & 0xEC;
    result = value0 & value2 & (unsigned int)value5;
    reg68->sr.raw[0] = value6;
    if ((int)(result | value1 & ~(value0 | value2)) < 0)
        context3->sr.raw[0] = value6 | 2;
    if ((value0 & value2 | value5 & (value0 | value2)) < 0)
        context3->sr.raw[0] |= 0x11u;
    return result;
}

std::intptr_t Emu68_CompareMemoryByte() {
    unsigned int value0;
    int8_t value1;
    reg68_ctx_t *context2;
    unsigned int value3;
    unsigned int value4;
    int8_t value5;
    unsigned int value6;
    int8_t value7;
    int8_t value8;
    int8_t value9;
    int8_t result;

    value0 = (int8_t)Emu68_ReadPostincrementByte(g_emu68Instruction & 7);
    value1 = Emu68_ReadPostincrementByte(((uint16_t)g_emu68Instruction >> 9) & 7);
    context2 = reg68;
    value3 = value0 >> 7;
    reg68->sr.raw[0] &= 0xF0u;
    value4 = value1 - value0;
    value5 = ((unsigned int)value1 >> 7) & (~value0 >> 7) & (~value4 >> 7);
    value6 = (unsigned int)~value1 >> 7;
    value7 = (4 * ((value4 & 0x80) != 0)) | (value6 & (value4 >> 7) & value3 | value5) & 1;
    value8 = 0;
    if (!value4)
        value8 = 2;
    value9 = value8 | value7;
    result = ((value4 >> 7) & value3 | value6 & ((value4 >> 7) | value3)) & 1;
    context2->sr.raw[0] |= result | (uint8_t)(2 * value9);
    return result;
}

std::intptr_t Emu68_CompareMemoryWord() {
    int64_t value0;
    reg68_page_map_entry_t *page1;
    int16_t value2;
    reg68_ctx_t *context3;
    unsigned int value4;
    uint64_t value5;
    reg68_page_map_entry_t *page6;
    int16_t value7;
    reg68_ctx_t *context8;
    unsigned int value9;
    unsigned int value10;
    int8_t value11;
    int8_t value12;
    int8_t value13;
    int8_t result;

    value0 = g_emu68Instruction & 7;
    page1 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[value0]) >> 16)];
    value2 = page1->read16(page1->base + (reg68->a[value0] & page1->mask));
    context3 = reg68;
    value4 = value2;
    ++reg68->bus_word_count;
    context3->a[value0] += 2;
    value5 = ((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7;
    page6 = &context3->map[(uint8_t) static_cast<uint8_t>((context3->a[value5]) >> 16)];
    value7 = page6->read16(page6->base + (context3->a[value5] & page6->mask));
    context8 = reg68;
    ++reg68->bus_word_count;
    value9 = Emu68_Subtract32(value7, value4);
    context8->a[value5] += 2;
    context8->sr.raw[0] &= 0xF0u;
    value10 = (unsigned int)~value7 >> 15;
    value11 =
        (4 * ((value9 & 0x8000) != 0)) | (value10 & (value9 >> 15) & (value4 >> 15) | ((unsigned int)value7 >> 15) & (~value4 >> 15) & (~value9 >> 15)) & 1;
    value12 = 0;
    if (!value9)
        value12 = 2;
    value13 = value12 | value11;
    result = ((value9 >> 15) & (value4 >> 15) | value10 & ((value9 >> 15) | (value4 >> 15))) & 1;
    context8->sr.raw[0] |= result | (uint8_t)(2 * value13);
    return result;
}

std::intptr_t Emu68_CompareMemoryLong() {
    int64_t value0;
    reg68_page_map_entry_t *page1;
    int value2;
    reg68_ctx_t *context3;
    int value4;
    uint64_t value5;
    reg68_page_map_entry_t *page6;
    int value7;
    reg68_ctx_t *context8;
    int value9;
    unsigned int value10;
    bool value11;
    int8_t value12;
    int8_t value13;
    int8_t value14;
    int8_t result;

    value0 = g_emu68Instruction & 7;
    page1 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[value0]) >> 16)];
    value2 = page1->read32(page1->base + (reg68->a[value0] & page1->mask));
    context3 = reg68;
    value4 = value2;
    reg68->bus_word_count += 2;
    context3->a[value0] += 4;
    value5 = ((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7;
    page6 = &context3->map[(uint8_t) static_cast<uint8_t>((context3->a[value5]) >> 16)];
    value7 = page6->read32(page6->base + (context3->a[value5] & page6->mask));
    context8 = reg68;
    value9 = Emu68_Subtract32(value7, value4);
    reg68->bus_word_count += 2;
    context8->a[value5] += 4;
    context8->sr.raw[0] &= 0xF0u;
    value10 = (unsigned int)(Emu68_Subtract32(value7, value4)) >> 31;
    value11 = value7 >= 0;
    value12 = (4 * (value9 < 0)) | (value4 < 0 && value7 >= 0 && value9 < 0 || value7 < 0 && value4 >= 0 && value9 >= 0);
    value13 = 0;
    if (!value9)
        value13 = 2;
    value14 = value13 | value12;
    result = value11 & ((value4 < 0) | value10);
    context8->sr.raw[0] |= (value4 < 0) & value10 | result | (2 * value14);
    return result;
}

std::intptr_t Emu68_ArithmeticShiftLeft(int value, int count, int signMask) {
    reg68_ctx_t *context3;

    context3 = reg68;
    if (count)
        reg68->sr.raw[0] &= ~1u;
    for (context3->sr.raw[0] &= ~2u; count > 0; value = static_cast<int>(static_cast<uint32_t>(value) * 2U)) {
        if (((value ^ ((static_cast<uint32_t>(value) * 2U))) & signMask) != 0)
            context3->sr.raw[0] |= 2u;
        --count;
    }
    return value;
}

std::intptr_t Emu68_LogicalShiftLeft(int value, int count, int signMask) {
    reg68_ctx_t *context3;
    unsigned int value4;

    context3 = reg68;
    if ((((static_cast<uint32_t>(value) << (static_cast<unsigned>(count - 1) & 31))) & signMask) != 0) {
        reg68->sr.raw[0] |= 0x11u;
        context3->sr.raw[0] &= ~2u;
        return (unsigned int)((static_cast<uint32_t>(value) << (static_cast<unsigned>(count) & 31)));
    } else {
        reg68->sr.raw[0] &= 0xEEu;
        value4 = (static_cast<uint32_t>(value) << (static_cast<unsigned>(count) & 31));
        context3->sr.raw[0] &= ~2u;
    }
    return value4;
}

std::intptr_t Emu68_ArithmeticShiftRight(int value, int count, int signMask) {
    reg68_ctx_t *context4;

    if (count) {
        context4 = reg68;
        if (((static_cast<uint32_t>(value) >> (static_cast<unsigned>((uint8_t)(count - 1)) & 31)) & 1))
            reg68->sr.raw[0] |= 0x11u;
        else
            reg68->sr.raw[0] &= 0xEEu;
        context4->sr.raw[0] &= ~2u;
        return (unsigned int)((static_cast<int32_t>(value) >> (static_cast<unsigned>(count) & 31)));
    } else {
        reg68->sr.raw[0] &= 0xFCu;
        return (unsigned int)value;
    }
}

std::intptr_t Emu68_LogicalShiftRight(int value, int count, int signMask) {
    int value3;
    reg68_sr_bits_t *p_sr;

    value3 = value;
    if (count) {
        p_sr = &reg68->sr;
        if (((static_cast<uint32_t>(value3) >> (static_cast<unsigned>((uint8_t)(count - 1)) & 31)) & 1))
            reg68->sr.raw[0] |= 0x11u;
        else
            reg68->sr.raw[0] &= 0xEEu;
        for (; count > 0; value3 = (static_cast<uint32_t>(signMask) - 1U) & (value3 >> 1))
            --count;
        p_sr->raw[0] &= ~2u;
        return (unsigned int)value3;
    } else {
        reg68->sr.raw[0] &= 0xFCu;
        return value;
    }
}

std::intptr_t Emu68_RotateRight(int value, int count, int signMask) {
    reg68_ctx_t *context3;

    context3 = reg68;
    if ((uint32_t)count) {
        if ((int)count > 0) {
            count = (unsigned int)count;
            do {
                if ((value & 1) != 0) {
                    context3->sr.raw[0] |= 1u;
                    value = signMask | (value >> 1);
                } else {
                    context3->sr.raw[0] &= ~1u;
                    value = ~signMask & (value >> 1);
                }
                --count;
            } while (count);
        }
        context3->sr.raw[0] &= ~2u;
        return (unsigned int)value;
    } else {
        reg68->sr.raw[0] &= ~1u;
        context3->sr.raw[0] &= ~2u;
        return (unsigned int)value;
    }
}

std::intptr_t Emu68_RotateLeft(int value, int count, int signMask) {
    reg68_ctx_t *context3;

    context3 = reg68;
    if ((uint32_t)count) {
        if ((int)count > 0) {
            count = (unsigned int)count;
            do {
                if ((value & signMask) != 0) {
                    context3->sr.raw[0] |= 1u;
                    value = ((static_cast<uint32_t>(value) * 2U)) | 1;
                } else {
                    context3->sr.raw[0] &= ~1u;
                    value = static_cast<int>(static_cast<uint32_t>(value) * 2U);
                }
                --count;
            } while (count);
        }
        context3->sr.raw[0] &= ~2u;
        return value;
    } else {
        reg68->sr.raw[0] &= ~1u;
        context3->sr.raw[0] &= ~2u;
        return value;
    }
}

std::intptr_t Emu68_RotateExtendRight(int value, int count, int signMask) {
    reg68_ctx_t *context3;
    int64_t value5;
    int8_t value6;
    int8_t value8;
    int64_t value9;
    int8_t value10;
    int value11;

    context3 = reg68;
    value5 = (unsigned int)count;
    if (count) {
        if (count > 0) {
            value8 = reg68->sr.raw[0];
            value9 = value5;
            do {
                value10 = value8;
                if ((value & 1) != 0)
                    value8 |= 0x10u;
                else
                    value8 &= ~0x10u;
                value11 = value >> 1;
                if ((value10 & 0x10) != 0)
                    value = signMask | value11;
                else
                    value = ~signMask & value11;
                --value9;
            } while (value9);
            reg68->sr.raw[0] = value8;
        }
        context3->sr.raw[0] &= ~2u;
        return (unsigned int)value;
    } else {
        value6 = reg68->sr.raw[0];
        if ((value6 & 0x10) != 0)
            reg68->sr.raw[0] = value6 | 1;
        else
            reg68->sr.raw[0] = value6 & 0xFE;
        context3->sr.raw[0] &= ~2u;
        return (unsigned int)value;
    }
}

std::intptr_t Emu68_RotateExtendLeft(int value, int count, int signMask) {
    reg68_ctx_t *context3;
    int8_t value5;
    uint8_t value7;
    int64_t value8;
    int value9;

    context3 = reg68;
    if (count) {
        if (count > 0) {
            value7 = reg68->sr.raw[0];
            value8 = (unsigned int)count;
            do {
                value9 = (value7 >> 4) & 1;
                if ((value & signMask) != 0)
                    value7 |= 0x10u;
                else
                    value7 &= ~0x10u;
                value = value9 | ((static_cast<uint32_t>(value) * 2U));
                --value8;
            } while (value8);
            reg68->sr.raw[0] = value7;
        }
        context3->sr.raw[0] &= ~2u;
        return value;
    } else {
        value5 = reg68->sr.raw[0];
        if ((value5 & 0x10) != 0)
            reg68->sr.raw[0] = value5 | 1;
        else
            reg68->sr.raw[0] = value5 & 0xFE;
        context3->sr.raw[0] &= ~2u;
        return value;
    }
}

std::intptr_t Emu68_MoveMultiple(int operand) {
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    uint16_t value5;
    int64_t value6;
    uint32_t value7;
    int64_t i;
    reg68_page_map_entry_t *page9;
    int64_t j;
    reg68_page_map_entry_t *page11;
    int64_t k;
    reg68_page_map_entry_t *page13;
    int64_t m;
    reg68_page_map_entry_t *page15;
    int64_t n;
    reg68_page_map_entry_t *page17;
    int64_t ii;
    reg68_page_map_entry_t *page19;
    int64_t jj;
    reg68_page_map_entry_t *page21;
    int64_t kk;
    reg68_page_map_entry_t *page23;
    int64_t mm;
    reg68_page_map_entry_t *page25;
    int64_t nn;
    reg68_page_map_entry_t *page27;
    int64_t i1;
    reg68_page_map_entry_t *page29;
    int64_t i2;
    reg68_page_map_entry_t *page31;
    int value32;
    int value33;
    int value34;

    value1 = operand;
    page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
    context4 = reg68;
    value5 = value3;
    ++reg68->bus_word_count;
    context4->pc += 2;
    if ((uint32_t)value1 == 1 || (uint32_t)value1 == 3 || (uint32_t)value1 == 5 || (uint32_t)value1 == 7) {
        value6 = g_emu68Instruction & 7;
        value7 = context4->a[value6];
    } else {
        value6 = static_cast<uint32_t>(kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7));
        context4 = reg68;
        value7 = value6;
    }
    switch (value1) {
    case 0LL:
        for (i = 0; i < 8; ++i) {
            if ((value5 & 1) != 0) {
                page9 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>((page9->write16(page9->base + (value7 & page9->mask), static_cast<uint16_t>(Emu68_Register(context4, i))), 0));
                context4 = reg68;
                ++reg68->bus_word_count;
                value7 += 2;
            }
            value5 >>= 1;
        }
        for (j = 8; j < 16; ++j) {
            if ((value5 & 1) != 0) {
                page11 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 =
                    static_cast<uint32_t>((page11->write16(page11->base + (value7 & page11->mask), static_cast<uint16_t>(Emu68_Register(context4, j))), 0));
                context4 = reg68;
                ++reg68->bus_word_count;
                value7 += 2;
            }
            value5 >>= 1;
        }
        break;
    case 1LL:
        for (k = 15; k > 7; --k) {
            if ((value5 & 1) != 0) {
                value7 -= 2;
                page13 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 =
                    static_cast<uint32_t>((page13->write16(page13->base + (value7 & page13->mask), static_cast<uint16_t>(Emu68_Register(context4, k))), 0));
                context4 = reg68;
                ++reg68->bus_word_count;
            }
            value5 >>= 1;
        }
        for (m = 7; m > -1; --m) {
            if ((value5 & 1) != 0) {
                value7 -= 2;
                page15 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 =
                    static_cast<uint32_t>((page15->write16(page15->base + (value7 & page15->mask), static_cast<uint16_t>(Emu68_Register(context4, m))), 0));
                context4 = reg68;
                ++reg68->bus_word_count;
            }
            value5 >>= 1;
        }
        break;
    case 2LL:
        for (n = 0; n < 8; ++n) {
            if ((value5 & 1) != 0) {
                page17 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>((page17->write32(page17->base + (value7 & page17->mask), Emu68_Register(context4, n)), 0));
                context4 = reg68;
                reg68->bus_word_count += 2;
                value7 += 4;
            }
            value5 >>= 1;
        }
        for (ii = 8; ii < 16; ++ii) {
            if ((value5 & 1) != 0) {
                page19 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>((page19->write32(page19->base + (value7 & page19->mask), Emu68_Register(context4, ii)), 0));
                context4 = reg68;
                reg68->bus_word_count += 2;
                value7 += 4;
            }
            value5 >>= 1;
        }
        break;
    case 3LL:
        for (jj = 15; jj > 7; --jj) {
            if ((value5 & 1) != 0) {
                value7 -= 4;
                page21 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>((page21->write32(page21->base + (value7 & page21->mask), Emu68_Register(context4, jj)), 0));
                context4 = reg68;
                reg68->bus_word_count += 2;
            }
            value5 >>= 1;
        }
        for (kk = 7; kk > -1; --kk) {
            if ((value5 & 1) != 0) {
                value7 -= 4;
                page23 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>((page23->write32(page23->base + (value7 & page23->mask), Emu68_Register(context4, kk)), 0));
                context4 = reg68;
                reg68->bus_word_count += 2;
            }
            value5 >>= 1;
        }
        break;
    case 4LL:
    case 5LL:
        for (mm = 0; mm < 8; ++mm) {
            if ((value5 & 1) != 0) {
                page25 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>(page25->read16(page25->base + (value7 & page25->mask)));
                context4 = reg68;
                ++reg68->bus_word_count;
                value7 += 2;
                Emu68_Register(context4, mm) = (Emu68_Register(context4, mm) & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value6);
            }
            value5 >>= 1;
        }
        for (nn = 8; nn < 16; ++nn) {
            if ((value5 & 1) != 0) {
                page27 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint16_t>(page27->read16(page27->base + (value7 & page27->mask)));
                context4 = reg68;
                value6 = static_cast<uint32_t>((int16_t)value6);
                ++reg68->bus_word_count;
                value7 += 2;
                Emu68_Register(context4, nn) = (int16_t)value6;
            }
            value5 >>= 1;
        }
        break;
    case 6LL:
    case 7LL:
        for (i1 = 0; i1 < 8; ++i1) {
            if ((value5 & 1) != 0) {
                page29 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>(page29->read32(page29->base + (value7 & page29->mask)));
                context4 = reg68;
                reg68->bus_word_count += 2;
                value7 += 4;
                Emu68_Register(context4, i1) = value6;
            }
            value5 >>= 1;
        }
        for (i2 = 8; i2 < 16; ++i2) {
            if ((value5 & 1) != 0) {
                page31 = &context4->map[static_cast<uint8_t>((value7) >> 16)];
                value6 = static_cast<uint32_t>(page31->read32(page31->base + (value7 & page31->mask)));
                context4 = reg68;
                reg68->bus_word_count += 2;
                value7 += 4;
                Emu68_Register(context4, i2) = value6;
            }
            value5 >>= 1;
        }
        break;
    default:
        break;
    }
    value32 = value1 - 1;
    if (!value32 || (value33 = value32 - 2) == 0 || (value34 = value33 - 2) == 0 || value34 == 2) {
        value6 = g_emu68Instruction & 7;
        context4->a[value6] = value7;
    }
    return value6;
}

std::intptr_t Emu68_BitTest(uint8_t operand) {
    uint64_t value5;
    int value6;
    int value7;
    int8_t value8;
    reg68_ctx_t *result;

    value5 = ((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7;
    value6 = (g_emu68Instruction & 7);
    if ((g_emu68Instruction & 0x38) != 0) {
        value7 = (uint8_t)(int8_t)kEmu68_ReadByte[value5](value6);
        value8 = 7;
    } else {
        value7 = kEmu68_ReadLong[value5]((int)value6);
        value8 = 31;
    }
    result = reg68;
    if (((static_cast<uint32_t>(value7) >> (static_cast<unsigned>(operand & (uint8_t)value8) & 31)) & 1))
        reg68->sr.raw[0] &= ~4u;
    else
        reg68->sr.raw[0] |= 4u;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_BitChange(int8_t operand) {
    uint64_t value4;
    int8_t value5;
    int value6;
    int value7;
    uint64_t value8;

    value4 = ((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7;
    if ((g_emu68Instruction & 0x38) != 0) {
        value5 = operand & 7;
        value6 = (uint8_t)(int8_t)kEmu68_ReadModifyByte[value4]((g_emu68Instruction & 7));
    } else {
        value5 = operand & 0x1F;
        value6 = kEmu68_ReadModifyLong[value4](g_emu68Instruction & 7);
    }
    value7 = 1 << value5;
    if ((value6 & (1 << value5)) != 0)
        reg68->sr.raw[0] &= ~4u;
    else
        reg68->sr.raw[0] |= 4u;
    value8 = ((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7;
    if ((g_emu68Instruction & 0x38) != 0)
        return kEmu68_WriteByte[value8](g_emu68Instruction & 7, (uint8_t)value6 ^ (uint8_t)value7);
    else
        return kEmu68_WriteLong[value8](g_emu68Instruction & 7, value6 ^ (unsigned int)value7);
}

std::intptr_t Emu68_BitClear(int8_t operand) {
    uint64_t value4;
    int8_t value5;
    int value6;
    int value7;
    uint64_t value8;

    value4 = ((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7;
    if ((g_emu68Instruction & 0x38) != 0) {
        value5 = operand & 7;
        value6 = (uint8_t)(int8_t)kEmu68_ReadModifyByte[value4]((g_emu68Instruction & 7));
    } else {
        value5 = operand & 0x1F;
        value6 = kEmu68_ReadModifyLong[value4](g_emu68Instruction & 7);
    }
    value7 = 1 << value5;
    if ((value6 & (1 << value5)) != 0)
        reg68->sr.raw[0] &= ~4u;
    else
        reg68->sr.raw[0] |= 4u;
    value8 = ((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7;
    if ((g_emu68Instruction & 0x38) != 0)
        return kEmu68_WriteByte[value8](g_emu68Instruction & 7, (uint8_t)value6 & (uint8_t) ~(uint8_t)value7);
    else
        return kEmu68_WriteLong[value8](g_emu68Instruction & 7, value6 & (unsigned int)~value7);
}

std::intptr_t Emu68_BitSet(int8_t operand) {
    uint64_t value4;
    int8_t value5;
    int value6;
    int value7;
    uint64_t value8;

    value4 = ((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7;
    if ((g_emu68Instruction & 0x38) != 0) {
        value5 = operand & 7;
        value6 = (uint8_t)(int8_t)kEmu68_ReadModifyByte[value4]((g_emu68Instruction & 7));
    } else {
        value5 = operand & 0x1F;
        value6 = kEmu68_ReadModifyLong[value4](g_emu68Instruction & 7);
    }
    value7 = 1 << value5;
    if ((value6 & (1 << value5)) != 0)
        reg68->sr.raw[0] &= ~4u;
    else
        reg68->sr.raw[0] |= 4u;
    value8 = ((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7;
    if ((g_emu68Instruction & 0x38) != 0)
        return kEmu68_WriteByte[value8](g_emu68Instruction & 7, (uint8_t)value6 | (uint8_t)value7);
    else
        return kEmu68_WriteLong[value8](g_emu68Instruction & 7, value6 | (unsigned int)value7);
}

std::intptr_t Emu68_MovePeripheral() {
    reg68_page_map_entry_t *page0;
    int16_t value1;
    uint8_t value2;
    reg68_ctx_t *context3;
    int64_t value4;
    uint64_t value5;
    int value6;
    int64_t value7;
    uint32_t value8;
    reg68_ctx_t *context9;
    reg68_page_map_entry_t *page10;
    reg68_ctx_t *context11;
    reg68_page_map_entry_t *page12;
    int value13;
    reg68_ctx_t *context14;
    reg68_page_map_entry_t *page15;
    uint64_t result;
    uint8_t value17;
    reg68_ctx_t *context18;
    int value19;
    reg68_page_map_entry_t *page20;
    uint8_t value21;
    reg68_ctx_t *context22;
    int value23;
    reg68_page_map_entry_t *page24;
    uint8_t value25;
    reg68_ctx_t *context26;
    int value27;
    reg68_page_map_entry_t *page28;
    uint8_t value29;
    reg68_ctx_t *context30;
    int value31;
    reg68_page_map_entry_t *page32;
    uint8_t value33;
    reg68_ctx_t *context34;
    int16_t value35;
    reg68_page_map_entry_t *page36;
    uint8_t value37;
    reg68_ctx_t *context38;
    int16_t value39;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    value2 = g_emu68Instruction;
    context3 = reg68;
    value4 = g_emu68Instruction & 7;
    value5 = (uint16_t)g_emu68Instruction;
    ++reg68->bus_word_count;
    context3->pc += 2;
    value6 = context3->a[value4] + value1;
    if (value2 >> 6) {
        if (value2 >> 6 == 1) {
            reg68_page_map_entry_t &memoryPage = context3->map[static_cast<uint8_t>(value6 >> 16)];
            value17 = memoryPage.read8(memoryPage.base + (value6 & memoryPage.mask));
            context18 = reg68;
            value19 = value17;
            ++reg68->bus_word_count;
            page20 = &context18->map[(uint8_t)((unsigned int)(value6 + 2) >> 16)];
            value21 = page20->read8(page20->base + ((unsigned int)(value6 + 2) & page20->mask));
            context22 = reg68;
            value23 = value21;
            ++reg68->bus_word_count;
            page24 = &context22->map[(uint8_t)((unsigned int)(value6 + 4) >> 16)];
            value25 = page24->read8(page24->base + ((unsigned int)(value6 + 4) & page24->mask));
            context26 = reg68;
            value27 = value25;
            ++reg68->bus_word_count;
            page28 = &context26->map[(uint8_t)((unsigned int)(value6 + 6) >> 16)];
            value29 = page28->read8(page28->base + ((unsigned int)(value6 + 6) & page28->mask));
            context30 = reg68;
            ++reg68->bus_word_count;
            value31 = value29 | ((value27 | ((value23 | (value19 << 8)) << 8)) << 8);
            result = ((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7;
            context30->d[result] = value31;
        } else {
            value7 = (value5 >> 9) & 7;
            if (value2 >> 6 == 2) {
                value8 = static_cast<uint16_t>(context3->d[value7]);
                reg68_page_map_entry_t &memoryPage = context3->map[static_cast<uint8_t>(value6 >> 16)];
                memoryPage.write8(memoryPage.base + (value6 & memoryPage.mask), static_cast<uint8_t>(value8 >> 8));
                value13 = value6 + 2;
            } else {
                value8 = context3->d[value7];
                reg68_page_map_entry_t &memoryPage = context3->map[static_cast<uint8_t>(value6 >> 16)];
                memoryPage.write8(memoryPage.base + (value6 & memoryPage.mask), static_cast<uint8_t>(value8 >> 24));
                context9 = reg68;
                ++reg68->bus_word_count;
                page10 = &context9->map[(uint8_t)((unsigned int)(value6 + 2) >> 16)];
                (page10->write8(page10->base + ((unsigned int)(value6 + 2) & page10->mask), static_cast<uint8_t>((value8) >> 16)), 0);
                context11 = reg68;
                ++reg68->bus_word_count;
                page12 = &context11->map[(uint8_t)((unsigned int)(value6 + 4) >> 16)];
                (page12->write8(page12->base + ((unsigned int)(value6 + 4) & page12->mask), static_cast<uint8_t>((value8) >> 8)), 0);
                value13 = value6 + 6;
            }
            context14 = reg68;
            ++reg68->bus_word_count;
            page15 = &context14->map[static_cast<uint8_t>((value13) >> 16)];
            (page15->write8(page15->base + ((unsigned int)value13 & page15->mask), (uint8_t)value8), 0);
            result = (uint64_t)reg68;
            ++reg68->bus_word_count;
        }
    } else {
        page32 = &context3->map[static_cast<uint8_t>((value6) >> 16)];
        value33 = page32->read8(page32->base + ((unsigned int)value6 & page32->mask));
        context34 = reg68;
        value35 = value33;
        ++reg68->bus_word_count;
        page36 = &context34->map[(uint8_t)((unsigned int)(value6 + 2) >> 16)];
        value37 = page36->read8(page36->base + ((unsigned int)(value6 + 2) & page36->mask));
        context38 = reg68;
        ++reg68->bus_word_count;
        value39 = value37 | (uint16_t)(value35 << 8);
        result = ((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7;
        context38->d[result] = (context38->d[result] & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value39);
    }
    return result;
}

std::intptr_t Emu68_DivideUnsigned(int operand) {
    int64_t value1;
    uint16_t value2;
    reg68_ctx_t *context3;
    int *value4;
    uint16_t value5;
    unsigned int value6;
    int value7;
    int64_t result;

    value1 = operand;
    value2 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context3 = reg68;
    value4 = (int *)&reg68->d[value1];
    value6 = *value4 % (unsigned int)value2;
    value5 = *value4 / (unsigned int)value2;
    value7 = (int16_t)value5;
    *value4 = value5 | (static_cast<uint32_t>(static_cast<uint16_t>(value6)) << 16);
    result = 0;
    context3->sr.raw[0] &= 0xF0u;
    if (!value7)
        result = 4;
    context3->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value7 < 0));
    return result;
}

std::intptr_t Emu68_DivideSigned(int operand) {
    int64_t value1;
    int16_t value2;
    reg68_ctx_t *context3;
    int *value4;
    uint16_t value5;
    int value6;
    int value7;
    int64_t result;

    value1 = operand;
    value2 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context3 = reg68;
    value4 = (int *)&reg68->d[value1];
    value6 = *value4 % value2;
    value5 = *value4 / value2;
    value7 = (int16_t)value5;
    *value4 = value5 | (static_cast<uint32_t>(static_cast<uint16_t>(value6)) << 16);
    result = 0;
    context3->sr.raw[0] &= 0xF0u;
    if (!value7)
        result = 4;
    context3->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value7 < 0));
    return result;
}

std::intptr_t emu68_HandleInterruptRequest(unsigned int operand) {
    reg68_ctx_t *context1;
    int64_t pc;
    reg68_page_map_entry_t *reg68_page_map_entry_t_1;
    reg68_ctx_t *context5;
    int16_t value6;
    int16_t value7;
    reg68_page_map_entry_t *page8;
    reg68_ctx_t *context9;
    reg68_page_map_entry_t *reg68_page_map_entry_t;
    int64_t result;
    reg68_ctx_t *context12;

    context1 = reg68;
    pc = reg68->pc;
    reg68->a[7] -= 4;
    reg68_page_map_entry_t_1 = &context1->map[(uint8_t) static_cast<uint8_t>((context1->a[7]) >> 16)];
    reg68_page_map_entry_t_1->write32(reg68_page_map_entry_t_1->base + (context1->a[7] & reg68_page_map_entry_t_1->mask), pc);
    context5 = reg68;
    value6 = (uint8_t)reg68->sr.raw[0];
    value7 = (uint8_t)reg68->sr.raw[1];
    reg68->bus_word_count += 2;
    context5->a[7] -= 2;
    page8 = &context5->map[(uint8_t) static_cast<uint8_t>((context5->a[7]) >> 16)];
    (page8->write16(page8->base + (context5->a[7] & page8->mask), (uint16_t)(value6 | (value7 << 8))), 0);
    context9 = reg68;
    reg68->sr.raw[1] &= 7u;
    ++context9->bus_word_count;
    context9->sr.raw[1] |= 0x20u;
    reg68_page_map_entry_t = &context9->map[(uint8_t)(operand >> 14)];
    result = reg68_page_map_entry_t->read32(reg68_page_map_entry_t->base + ((4 * operand) & reg68_page_map_entry_t->mask));
    context12 = reg68;
    reg68->bus_word_count += 2;
    context12->pc = result;
    return result;
}

std::intptr_t Emu68_ResetContext() {
    reg68_ctx_t *context0;
    uint32_t value1;
    reg68_ctx_t *context2;
    int64_t result;
    reg68_ctx_t *context4;

    context0 = reg68;
    for (unsigned index = 0; index < 8; ++index)
        reg68->d[index] = 0;
    context0->pc = 0;
    context0->sr.bits = 0x0700 | 0x2000;
    context0->bus_word_count = 0;
    value1 = context0->map->read32(context0->map->base);
    context2 = reg68;
    reg68->bus_word_count += 2;
    context2->a[7] = value1;
    result = context2->map->read32(context2->map->base + (context2->map->mask & 4));
    context4 = reg68;
    reg68->bus_word_count += 2;
    context4->pc = result;
    return result;
}

std::intptr_t Emu68_ReturnException() {
    reg68_page_map_entry_t *page0;
    uint16_t value1;
    reg68_ctx_t *context2;
    reg68_sr_bits_t status3;
    reg68_page_map_entry_t *page4;
    int64_t result;
    reg68_ctx_t *context6;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->a[7]) >> 16)];
    value1 = page0->read16(page0->base + (reg68->a[7] & page0->mask));
    context2 = reg68;
    status3.bits = value1;
    ++reg68->bus_word_count;
    context2->a[7] += 2;
    if ((context2->sr.raw[1] & 0x20) == 0) {
        emu68_HandleInterruptRequest(8u);
        context2 = reg68;
    }
    context2->sr = status3;
    page4 = &context2->map[(uint8_t) static_cast<uint8_t>((context2->a[7]) >> 16)];
    result = page4->read32(page4->base + (context2->a[7] & page4->mask));
    context6 = reg68;
    reg68->bus_word_count += 2;
    context6->a[7] += 4;
    context6->pc = result;
    return result;
}

std::intptr_t Emu68_OrImmediateByte() {
    uint32_t pc;
    int8_t value1;
    int8_t value2;
    reg68_ctx_t *context3;
    int8_t result;
    reg68_page_map_entry_t *page5;
    int8_t value6;
    reg68_ctx_t *context7;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x3F) == 0x3C) {
        value1 = reg68->sr.raw[0];
        reg68_page_map_entry_t &memoryPage = reg68->map[static_cast<uint8_t>(pc >> 16)];
        value2 = memoryPage.read16(memoryPage.base + (pc & memoryPage.mask));
        context3 = reg68;
        ++reg68->bus_word_count;
        context3->pc += 2;
        result = value1 | value2;
        context3->sr.raw[0] = result;
    } else {
        page5 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value6 = page5->read16(page5->base + (pc & page5->mask));
        context7 = reg68;
        ++reg68->bus_word_count;
        context7->pc += 2;
        return Emu68_OrEffectiveByte(value6);
    }
    return result;
}

std::intptr_t Emu68_OrImmediateWord() {
    uint32_t pc;
    reg68_sr_bits_t status1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    reg68_sr_bits_t status5;
    reg68_page_map_entry_t *page7;
    int16_t value8;
    reg68_ctx_t *context9;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x3F) == 0x3C) {
        status1.bits = reg68->sr.bits;
        page2 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value3 = page2->read16(page2->base + (pc & page2->mask));
        context4 = reg68;
        status5.bits = status1.bits | value3;
        ++reg68->bus_word_count;
        context4->pc += 2;
        if ((context4->sr.raw[1] & 0x20) == 0) {
            emu68_HandleInterruptRequest(8u);
            context4 = reg68;
        }
        context4->sr = status5;
        return status5.bits >> 8;
    } else {
        page7 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value8 = page7->read16(page7->base + (pc & page7->mask));
        context9 = reg68;
        ++reg68->bus_word_count;
        context9->pc += 2;
        return Emu68_OrEffectiveWord(value8);
    }
}

std::intptr_t Emu68_OrImmediateLong() {
    reg68_page_map_entry_t *page0;
    int value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read32(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    reg68->bus_word_count += 2;
    context2->pc += 4;
    return Emu68_OrEffectiveLong(value1);
}

std::intptr_t Emu68_AndImmediateByte() {
    uint32_t pc;
    int8_t value1;
    int8_t value2;
    reg68_ctx_t *context3;
    int8_t result;
    reg68_page_map_entry_t *page5;
    int8_t value6;
    reg68_ctx_t *context7;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x3F) == 0x3C) {
        value1 = reg68->sr.raw[0];
        reg68_page_map_entry_t &memoryPage = reg68->map[static_cast<uint8_t>(pc >> 16)];
        value2 = memoryPage.read16(memoryPage.base + (pc & memoryPage.mask));
        context3 = reg68;
        ++reg68->bus_word_count;
        context3->pc += 2;
        result = value1 & value2;
        context3->sr.raw[0] = result;
    } else {
        page5 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value6 = page5->read16(page5->base + (pc & page5->mask));
        context7 = reg68;
        ++reg68->bus_word_count;
        context7->pc += 2;
        return Emu68_AndEffectiveByte(value6);
    }
    return result;
}

std::intptr_t Emu68_AndImmediateWord() {
    uint32_t pc;
    reg68_sr_bits_t status1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    reg68_sr_bits_t status5;
    reg68_page_map_entry_t *page7;
    int16_t value8;
    reg68_ctx_t *context9;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x3F) == 0x3C) {
        status1.bits = reg68->sr.bits;
        page2 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value3 = page2->read16(page2->base + (pc & page2->mask));
        context4 = reg68;
        status5.bits = status1.bits & value3;
        ++reg68->bus_word_count;
        context4->pc += 2;
        if ((context4->sr.raw[1] & 0x20) == 0) {
            emu68_HandleInterruptRequest(8u);
            context4 = reg68;
        }
        context4->sr = status5;
        return status5.bits >> 8;
    } else {
        page7 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value8 = page7->read16(page7->base + (pc & page7->mask));
        context9 = reg68;
        ++reg68->bus_word_count;
        context9->pc += 2;
        return Emu68_AndEffectiveWord(value8);
    }
}

std::intptr_t Emu68_AndImmediateLong() {
    reg68_page_map_entry_t *page0;
    int value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read32(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    reg68->bus_word_count += 2;
    context2->pc += 4;
    return Emu68_AndEffectiveLong(value1);
}

std::intptr_t Emu68_XorImmediateByte() {
    uint32_t pc;
    int8_t value1;
    int8_t value2;
    reg68_ctx_t *context3;
    int8_t result;
    reg68_page_map_entry_t *page5;
    int8_t value6;
    reg68_ctx_t *context7;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x3F) == 0x3C) {
        value1 = reg68->sr.raw[0];
        reg68_page_map_entry_t &memoryPage = reg68->map[static_cast<uint8_t>(pc >> 16)];
        value2 = memoryPage.read16(memoryPage.base + (pc & memoryPage.mask));
        context3 = reg68;
        ++reg68->bus_word_count;
        context3->pc += 2;
        result = value1 ^ value2;
        context3->sr.raw[0] = result;
    } else {
        page5 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value6 = page5->read16(page5->base + (pc & page5->mask));
        context7 = reg68;
        ++reg68->bus_word_count;
        context7->pc += 2;
        return Emu68_XorEffectiveByte(value6);
    }
    return result;
}

std::intptr_t Emu68_XorImmediateWord() {
    uint32_t pc;
    reg68_sr_bits_t status1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    reg68_sr_bits_t status5;
    reg68_page_map_entry_t *page7;
    int16_t value8;
    reg68_ctx_t *context9;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x3F) == 0x3C) {
        status1.bits = reg68->sr.bits;
        page2 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value3 = page2->read16(page2->base + (pc & page2->mask));
        context4 = reg68;
        status5.bits = status1.bits ^ value3;
        ++reg68->bus_word_count;
        context4->pc += 2;
        if ((context4->sr.raw[1] & 0x20) == 0) {
            emu68_HandleInterruptRequest(8u);
            context4 = reg68;
        }
        context4->sr = status5;
        return status5.bits >> 8;
    } else {
        page7 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value8 = page7->read16(page7->base + (pc & page7->mask));
        context9 = reg68;
        ++reg68->bus_word_count;
        context9->pc += 2;
        return Emu68_XorEffectiveWord(value8);
    }
}

std::intptr_t Emu68_XorImmediateLong() {
    reg68_page_map_entry_t *page0;
    int value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read32(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    reg68->bus_word_count += 2;
    context2->pc += 4;
    return Emu68_XorEffectiveLong(value1);
}

std::intptr_t Emu68_SubtractImmediateByte() {
    reg68_page_map_entry_t *page0;
    int8_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_SubtractFromEffectiveByte(value1);
}

std::intptr_t Emu68_SubtractImmediateWord() {
    reg68_page_map_entry_t *page0;
    int16_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_SubtractFromEffectiveWord(value1);
}

std::intptr_t Emu68_SubtractImmediateLong() {
    reg68_page_map_entry_t *page0;
    int value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read32(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    reg68->bus_word_count += 2;
    context2->pc += 4;
    return Emu68_SubtractFromEffectiveLong(value1);
}

std::intptr_t Emu68_AddImmediateByte() {
    reg68_page_map_entry_t *page0;
    int8_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_AddToEffectiveByte(value1);
}

std::intptr_t Emu68_AddImmediateWord() {
    reg68_page_map_entry_t *page0;
    int16_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_AddToEffectiveWord(value1);
}

std::intptr_t Emu68_AddImmediateLong() {
    reg68_page_map_entry_t *page0;
    int value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read32(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    reg68->bus_word_count += 2;
    context2->pc += 4;
    return Emu68_AddToEffectiveLong(value1);
}

std::intptr_t Emu68_CompareImmediateByte() {
    reg68_page_map_entry_t *page0;
    int8_t value1;
    reg68_ctx_t *context2;
    unsigned int value3;
    int8_t value4;
    uint64_t value5;
    int8_t value6;
    reg68_ctx_t *context7;
    unsigned int value8;
    unsigned int value9;
    int8_t value10;
    int8_t value11;
    int8_t value12;
    int8_t result;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    value3 = value1;
    value4 = g_emu68Instruction;
    value5 = (uint16_t)g_emu68Instruction;
    ++reg68->bus_word_count;
    context2->pc += 2;
    value6 = (int8_t)kEmu68_ReadByte[(value5 >> 3) & 7]((value4 & 7));
    context7 = reg68;
    value8 = value6 - value3;
    reg68->sr.raw[0] &= 0xF0u;
    value9 = (unsigned int)~value6 >> 7;
    value10 = (4 * ((value8 & 0x80) != 0)) | (value9 & (value8 >> 7) & (value3 >> 7) | ((unsigned int)value6 >> 7) & (~value3 >> 7) & (~value8 >> 7)) & 1;
    value11 = 0;
    if (!value8)
        value11 = 2;
    value12 = value11 | value10;
    result = ((value8 >> 7) & (value3 >> 7) | value9 & ((value8 >> 7) | (value3 >> 7))) & 1;
    context7->sr.raw[0] |= result | (uint8_t)(2 * value12);
    return result;
}

std::intptr_t Emu68_CompareImmediateWord() {
    reg68_page_map_entry_t *page0;
    int16_t value1;
    reg68_ctx_t *context2;
    unsigned int value3;
    uint64_t value4;
    int16_t value5;
    reg68_ctx_t *context6;
    unsigned int value7;
    unsigned int value8;
    int8_t value9;
    int8_t value10;
    int8_t value11;
    int8_t result;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    value3 = value1;
    value1 = static_cast<uint8_t>(g_emu68Instruction);
    value4 = (uint16_t)g_emu68Instruction;
    ++reg68->bus_word_count;
    context2->pc += 2;
    value5 = kEmu68_ReadWord[(value4 >> 3) & 7](value1 & 7);
    context6 = reg68;
    value7 = value5 - value3;
    reg68->sr.raw[0] &= 0xF0u;
    value8 = (unsigned int)~value5 >> 15;
    value9 = (4 * ((value7 & 0x8000) != 0)) | (value8 & (value7 >> 15) & (value3 >> 15) | ((unsigned int)value5 >> 15) & (~value3 >> 15) & (~value7 >> 15)) & 1;
    value10 = 0;
    if (!value7)
        value10 = 2;
    value11 = value10 | value9;
    result = ((value7 >> 15) & (value3 >> 15) | value8 & ((value7 >> 15) | (value3 >> 15))) & 1;
    context6->sr.raw[0] |= result | (uint8_t)(2 * value11);
    return result;
}

std::intptr_t Emu68_CompareImmediateLong() {
    reg68_page_map_entry_t *page0;
    int value1;
    reg68_ctx_t *context2;
    int value3;
    int value4;
    reg68_ctx_t *context5;
    int value6;
    bool value7;
    unsigned int value8;
    int8_t value9;
    int8_t value10;
    int8_t value11;
    int8_t result;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read32(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    value3 = value1;
    reg68->bus_word_count += 2;
    context2->pc += 4;
    value4 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context5 = reg68;
    value6 = Emu68_Subtract32(value4, value3);
    reg68->sr.raw[0] &= 0xF0u;
    value7 = value4 >= 0;
    value8 = static_cast<unsigned int>(value6) >> 31;
    value9 = (4 * (value6 < 0)) | (value6 < 0 && value4 >= 0 && value3 < 0 || value4 < 0 && value3 >= 0 && value6 >= 0);
    value10 = 0;
    if (!value6)
        value10 = 2;
    value11 = value10 | value9;
    result = value7 & (value8 | (value3 < 0));
    context5->sr.raw[0] |= value8 & (value3 < 0) | result | (2 * value11);
    return result;
}

std::intptr_t Emu68_BitTestImmediate() {
    reg68_page_map_entry_t *page0;
    uint8_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_BitTest(value1);
}

std::intptr_t Emu68_BitChangeImmediate() {
    reg68_page_map_entry_t *page0;
    int8_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_BitChange(value1);
}

std::intptr_t Emu68_BitClearImmediate() {
    reg68_page_map_entry_t *page0;
    int8_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_BitClear(value1);
}

std::intptr_t Emu68_BitSetImmediate() {
    reg68_page_map_entry_t *page0;
    int8_t value1;
    reg68_ctx_t *context2;

    page0 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
    value1 = page0->read16(page0->base + (reg68->pc & page0->mask));
    context2 = reg68;
    ++reg68->bus_word_count;
    context2->pc += 2;
    return Emu68_BitSet(value1);
}

std::intptr_t Emu68_BitTestDataOrMovePeripheral() {
    if ((g_emu68Instruction & 0x38) == 8)
        return Emu68_MovePeripheral();
    else
        return Emu68_BitTest(reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
}

std::intptr_t Emu68_BitChangeDataOrMovePeripheral() {
    if ((g_emu68Instruction & 0x38) == 8)
        return Emu68_MovePeripheral();
    else
        return Emu68_BitChange(reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
}

std::intptr_t Emu68_BitClearDataOrMovePeripheral() {
    if ((g_emu68Instruction & 0x38) == 8)
        return Emu68_MovePeripheral();
    else
        return Emu68_BitClear(reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
}

std::intptr_t Emu68_BitSetDataOrMovePeripheral() {
    if ((g_emu68Instruction & 0x38) == 8)
        return Emu68_MovePeripheral();
    else
        return Emu68_BitSet(reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
}

std::intptr_t Emu68_ReportInstructionError() {

    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "INST", (uint16_t)g_emu68Instruction);
    (Harrier_DebugPrint(g_emu68DebugText), 0);
    std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
    return (Harrier_DebugPrint(g_emu68DebugText), 0);
}

void Emu68_DispatchGroup0() {
    kEmu68_Group0[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 0x3F]();
}

void Emu68_MoveByte() {
    int value4;
    reg68_ctx_t *context5;
    int64_t result;

    value4 = (int8_t)kEmu68_ReadByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_SetEffectiveByte[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 7]((((uint16_t)g_emu68Instruction >> 9) & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 7](((uint16_t)g_emu68Instruction >> 9) & 7, value4);
    context5 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!(uint8_t)value4)
        result = 4;
    context5->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value4 < 0));
    return;
}

void Emu68_MoveLong() {
    int value0;
    reg68_ctx_t *context1;

    value0 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_SetEffectiveLong[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 7]((((uint16_t)g_emu68Instruction >> 9) & 7));
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 7]((((uint16_t)g_emu68Instruction >> 9) & 7), (unsigned int)value0);
    context1 = reg68;
    reg68->sr.raw[0] &= 0xF0u;
    context1->sr.raw[0] |= 4 * ((value0 == 0) | (2 * (value0 < 0)));
    return;
}

void Emu68_MoveWord() {
    int value0;
    reg68_ctx_t *context1;
    int64_t result;

    value0 = (int16_t)kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    kEmu68_SetEffectiveWord[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 7]((((uint16_t)g_emu68Instruction >> 9) & 7));
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 7](((uint16_t)g_emu68Instruction >> 9) & 7, value0);
    context1 = reg68;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value0)
        result = 4;
    context1->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value0 < 0));
    return;
}

std::intptr_t Emu68_NegateExtendByteThunk() {
    return Emu68_NegateExtendEffectiveByte();
}

std::intptr_t Emu68_NegateExtendWordThunk() {
    return Emu68_NegateExtendEffectiveWord();
}

std::intptr_t Emu68_NegateExtendLongThunk() {
    return Emu68_NegateExtendEffectiveLong();
}

std::intptr_t Emu68_MoveFromStatus() {
    kEmu68_SetEffectiveWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    return kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, reg68->sr.bits);
}

std::intptr_t Emu68_MoveFromConditionCodes() {
    kEmu68_SetEffectiveWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    return kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, (uint8_t)reg68->sr.raw[0]);
}

std::intptr_t Emu68_MoveToStatus() {
    int64_t result;
    reg68_ctx_t *context1;
    reg68_sr_bits_t status2;

    result = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context1 = reg68;
    status2.bits = result;
    if ((reg68->sr.raw[1] & 0x20) == 0) {
        result = emu68_HandleInterruptRequest(8u);
        context1 = reg68;
    }
    context1->sr = status2;
    return result;
}

std::intptr_t Emu68_MoveToConditionCodes() {
    int64_t result;

    result = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->sr.raw[0] = result;
    return result;
}

std::intptr_t Emu68_CheckLongBounds() {
    int64_t result;
    int value1;

    result = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value1 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    if (value1 < 0 || (int)result < value1)
        return emu68_HandleInterruptRequest(6u);
    return result;
}

std::intptr_t Emu68_CheckWordBounds() {
    int64_t result;
    int16_t value1;

    result = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    value1 = reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    if (value1 < 0 || (int16_t)result < value1)
        return emu68_HandleInterruptRequest(6u);
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA0() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[0] = result;
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA1() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[1] = result;
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA2() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[2] = result;
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA3() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[3] = result;
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA4() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[4] = result;
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA5() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[5] = result;
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA6() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[6] = result;
    return result;
}

std::intptr_t Emu68_LoadEffectiveAddressA7() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->a[7] = result;
    return result;
}

std::intptr_t Emu68_ClearByte() {
    reg68_ctx_t *result;

    kEmu68_SetEffectiveByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, 0);
    result = reg68;
    reg68->sr.raw[0] &= 0xF4u;
    result->sr.raw[0] |= 4u;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_ClearWord() {
    reg68_ctx_t *result;

    kEmu68_SetEffectiveWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, 0);
    result = reg68;
    reg68->sr.raw[0] &= 0xF4u;
    result->sr.raw[0] |= 4u;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_ClearLong() {
    reg68_ctx_t *result;

    kEmu68_SetEffectiveLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    kEmu68_WriteLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7), 0);
    result = reg68;
    reg68->sr.raw[0] &= 0xF4u;
    result->sr.raw[0] |= 4u;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_NegateByteThunk() {
    return Emu68_NegateEffectiveByte();
}

std::intptr_t Emu68_NegateWordThunk() {
    return Emu68_NegateEffectiveWord();
}

std::intptr_t Emu68_NegateLongThunk() {
    return Emu68_NegateEffectiveLong();
}

std::intptr_t Emu68_NotByteThunk() {
    return Emu68_NotEffectiveByte();
}

std::intptr_t Emu68_NotWordThunk() {
    return Emu68_NotEffectiveWord();
}

std::intptr_t Emu68_NotLongThunk() {
    return Emu68_NotEffectiveLong();
}

std::intptr_t Emu68_TestByte() {
    int8_t value4;
    reg68_ctx_t *context5;
    int value6;
    int64_t result;

    value4 = (int8_t)kEmu68_ReadByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    context5 = reg68;
    value6 = value4;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value6)
        result = 4;
    context5->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value6 < 0));
    return result;
}

std::intptr_t Emu68_TestWord() {
    int16_t value0;
    reg68_ctx_t *context1;
    int value2;
    int64_t result;

    value0 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context1 = reg68;
    value2 = value0;
    result = 0;
    reg68->sr.raw[0] &= 0xF0u;
    if (!value2)
        result = 4;
    context1->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value2 < 0));
    return result;
}

std::intptr_t Emu68_TestLong() {
    int value0;
    reg68_ctx_t *context1;
    int8_t value2;
    bool result;

    value0 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context1 = reg68;
    reg68->sr.raw[0] &= 0xF0u;
    value2 = 2 * (value0 < 0);
    result = value0 == 0;
    context1->sr.raw[0] |= 4 * (result | value2);
    return result;
}

std::intptr_t Emu68_NegateBcdThunk() {
    return Emu68_NegateBcd();
}

std::intptr_t Emu68_SwapOrPushEffectiveAddress() {
    reg68_ctx_t *context0;
    int64_t value1;
    int value2;
    std::intptr_t result;
    unsigned int value4;
    reg68_ctx_t *context5;
    reg68_page_map_entry_t *page6;

    if ((g_emu68Instruction & 0x38) != 0) {
        value4 = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context5 = reg68;
        reg68->a[7] -= 4;
        page6 = &context5->map[(uint8_t) static_cast<uint8_t>((context5->a[7]) >> 16)];
        (page6->write32(page6->base + (context5->a[7] & page6->mask), value4), 0);
        result = reinterpret_cast<std::intptr_t>(reg68);
        reg68->bus_word_count += 2;
    } else {
        context0 = reg68;
        value1 = g_emu68Instruction & 7;
        value2 = std::rotl<uint32_t>(reg68->d[value1], 16);
        reg68->d[value1] = value2;
        context0->sr.raw[0] &= 0xF0u;
        context0->sr.raw[0] |= 4 * ((value2 == 0) | (2 * (value2 < 0)));
        return value2 == 0;
    }
    return result;
}

std::intptr_t Emu68_ExtendByteOrMoveMultipleWord() {
    unsigned int value0;
    int result;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    uint16_t value5;
    reg68_ctx_t *context6;
    int64_t value7;
    int value8;
    reg68_page_map_entry_t *page9;
    int64_t i;
    reg68_page_map_entry_t *page11;
    reg68_ctx_t *context12;
    int8_t value13;
    int64_t value14;
    int value15;

    value0 = ((uint16_t)g_emu68Instruction >> 3) & 7;
    if (value0) {
        if (value0 != 2) {
            if (value0 == 4)
                return Emu68_MoveMultiple(1);
            if (value0 <= 4)
                return Emu68_ReportRuntimeError("INST", (uint16_t)g_emu68Instruction);
        }
        page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
        context4 = reg68;
        value5 = value3;
        ++reg68->bus_word_count;
        context4->pc += 2;
        result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context6 = reg68;
        value7 = 0;
        value8 = result;
        do {
            if ((value5 & 1) != 0) {
                page9 = &context6->map[static_cast<uint8_t>((value8) >> 16)];
                result = (page9->write16(page9->base + ((unsigned int)value8 & page9->mask), static_cast<uint16_t>(Emu68_Register(context6, value7))), 0);
                context6 = reg68;
                ++reg68->bus_word_count;
                value8 += 2;
            }
            value5 >>= 1;
            ++value7;
        } while (value7 < 8);
        for (i = 8; i < 16; ++i) {
            if ((value5 & 1) != 0) {
                page11 = &context6->map[static_cast<uint8_t>((value8) >> 16)];
                result = (page11->write16(page11->base + ((unsigned int)value8 & page11->mask), static_cast<uint16_t>(Emu68_Register(context6, i))), 0);
                context6 = reg68;
                ++reg68->bus_word_count;
                value8 += 2;
            }
            value5 >>= 1;
        }
    } else {
        context12 = reg68;
        value13 = 0;
        value14 = g_emu68Instruction & 7;
        value15 = static_cast<int8_t>(Emu68_Register(reg68, value14));
        Emu68_Register(reg68, value14) = (Emu68_Register(reg68, value14) & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value15);
        result = value15;
        context12->sr.raw[0] &= 0xF0u;
        if (!value15)
            value13 = 4;
        context12->sr.raw[0] |= value13 | (uint8_t)(8 * (value15 < 0));
    }
    return result;
}

std::intptr_t Emu68_ExtendWordOrMoveMultipleLong() {
    unsigned int value0;
    int64_t value1;
    reg68_page_map_entry_t *page2;
    uint16_t value3;
    reg68_ctx_t *context4;
    int64_t value5;
    int8_t value6;
    uint16_t value7;
    uint32_t value8;
    reg68_page_map_entry_t *page9;
    int64_t j;
    reg68_page_map_entry_t *page11;
    reg68_page_map_entry_t *page12;
    uint16_t value13;
    reg68_ctx_t *context14;
    uint16_t value15;
    reg68_ctx_t *context16;
    int64_t value17;
    unsigned int value18;
    reg68_page_map_entry_t *page19;
    int64_t i;
    reg68_page_map_entry_t *page21;
    reg68_ctx_t *context22;
    int8_t value23;
    int64_t value24;
    int value25;

    value0 = ((uint16_t)g_emu68Instruction >> 3) & 7;
    if (value0) {
        if (value0 == 2) {
        LABEL_15:
            page12 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
            value13 = page12->read16(page12->base + (reg68->pc & page12->mask));
            context14 = reg68;
            value15 = value13;
            ++reg68->bus_word_count;
            context14->pc += 2;
            value1 = static_cast<uint32_t>(kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7));
            context16 = reg68;
            value17 = 0;
            value18 = value1;
            do {
                if ((value15 & 1) != 0) {
                    page19 = &context16->map[static_cast<uint8_t>((value18) >> 16)];
                    value1 = static_cast<uint8_t>((page19->write32(page19->base + (value18 & page19->mask), Emu68_Register(context16, value17)), 0));
                    context16 = reg68;
                    reg68->bus_word_count += 2;
                    value18 += 4;
                }
                value15 >>= 1;
                ++value17;
            } while (value17 < 8);
            for (i = 8; i < 16; ++i) {
                if ((value15 & 1) != 0) {
                    page21 = &context16->map[static_cast<uint8_t>((value18) >> 16)];
                    value1 = static_cast<uint8_t>((page21->write32(page21->base + (value18 & page21->mask), Emu68_Register(context16, i)), 0));
                    context16 = reg68;
                    reg68->bus_word_count += 2;
                    value18 += 4;
                }
                value15 >>= 1;
            }
            return value1;
        }
        if (value0 != 4) {
            if (value0 <= 4) {
                value1 = static_cast<uint8_t>(Emu68_ReportRuntimeError("INST", (uint16_t)g_emu68Instruction));
                return value1;
            }
            goto LABEL_15;
        }
        page2 = &reg68->map[(uint8_t) static_cast<uint8_t>((reg68->pc) >> 16)];
        value3 = page2->read16(page2->base + (reg68->pc & page2->mask));
        context4 = reg68;
        value5 = 15;
        value6 = g_emu68Instruction;
        value7 = value3;
        ++reg68->bus_word_count;
        context4->pc += 2;
        value8 = context4->a[value6 & 7];
        do {
            if ((value7 & 1) != 0) {
                value8 -= 4;
                page9 = &context4->map[static_cast<uint8_t>((value8) >> 16)];
                (page9->write32(page9->base + (value8 & page9->mask), Emu68_Register(context4, value5)), 0);
                context4 = reg68;
                reg68->bus_word_count += 2;
            }
            value7 >>= 1;
            --value5;
        } while (value5 > 7);
        for (j = 7; j > -1; --j) {
            if ((value7 & 1) != 0) {
                value8 -= 4;
                page11 = &context4->map[static_cast<uint8_t>((value8) >> 16)];
                (page11->write32(page11->base + (value8 & page11->mask), Emu68_Register(context4, j)), 0);
                context4 = reg68;
                reg68->bus_word_count += 2;
            }
            value7 >>= 1;
        }
        value1 = g_emu68Instruction & 7;
        context4->a[value1] = value8;
    } else {
        context22 = reg68;
        value23 = 0;
        value24 = g_emu68Instruction & 7;
        value25 = static_cast<int16_t>(Emu68_Register(reg68, value24));
        Emu68_Register(reg68, value24) = value25;
        context22->sr.raw[0] &= 0xF0u;
        if (!value25)
            value23 = 4;
        value1 = static_cast<uint8_t>(value23 | (8 * (value25 < 0)));
        context22->sr.raw[0] |= value1;
    }
    return value1;
}

std::intptr_t Emu68_MoveMultipleWordToRegisters() {
    uint32_t pc;
    reg68_page_map_entry_t *page1;
    uint16_t value2;
    reg68_ctx_t *context3;
    uint16_t value4;
    int64_t value5;
    uint32_t value6;
    int64_t i;
    reg68_page_map_entry_t *page8;
    int16_t value9;
    int64_t j;
    reg68_page_map_entry_t *page11;
    int16_t value12;
    int64_t value13;
    reg68_page_map_entry_t *page14;
    uint16_t value15;
    reg68_ctx_t *context16;
    uint16_t value17;
    reg68_ctx_t *context18;
    int64_t value19;
    unsigned int value20;
    reg68_page_map_entry_t *page21;
    int64_t k;
    reg68_page_map_entry_t *page23;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x38) == 0x18) {
        page1 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value2 = page1->read16(page1->base + (pc & page1->mask));
        context3 = reg68;
        value4 = value2;
        value5 = g_emu68Instruction & 7;
        ++reg68->bus_word_count;
        context3->pc += 2;
        value6 = context3->a[value5];
        for (i = 0; i < 8; ++i) {
            if ((value4 & 1) != 0) {
                page8 = &context3->map[static_cast<uint8_t>((value6) >> 16)];
                value9 = page8->read16(page8->base + (value6 & page8->mask));
                context3 = reg68;
                ++reg68->bus_word_count;
                value6 += 2;
                Emu68_Register(context3, i) = (Emu68_Register(context3, i) & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value9);
            }
            value4 >>= 1;
        }
        for (j = 8; j < 16; ++j) {
            if ((value4 & 1) != 0) {
                page11 = &context3->map[static_cast<uint8_t>((value6) >> 16)];
                value12 = page11->read16(page11->base + (value6 & page11->mask));
                context3 = reg68;
                ++reg68->bus_word_count;
                value6 += 2;
                Emu68_Register(context3, j) = value12;
            }
            value4 >>= 1;
        }
        value13 = g_emu68Instruction & 7;
        context3->a[value13] = value6;
    } else {
        page14 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value15 = page14->read16(page14->base + (pc & page14->mask));
        context16 = reg68;
        value17 = value15;
        ++reg68->bus_word_count;
        context16->pc += 2;
        value13 = static_cast<uint32_t>(kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7));
        context18 = reg68;
        value19 = 0;
        value20 = value13;
        do {
            if ((value17 & 1) != 0) {
                page21 = &context18->map[static_cast<uint8_t>((value20) >> 16)];
                value13 = static_cast<uint32_t>(page21->read16(page21->base + (value20 & page21->mask)));
                context18 = reg68;
                ++reg68->bus_word_count;
                value20 += 2;
                Emu68_Register(context18, value19) = (Emu68_Register(context18, value19) & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value13);
            }
            value17 >>= 1;
            ++value19;
        } while (value19 < 8);
        for (k = 8; k < 16; ++k) {
            if ((value17 & 1) != 0) {
                page23 = &context18->map[static_cast<uint8_t>((value20) >> 16)];
                value13 = static_cast<uint16_t>(page23->read16(page23->base + (value20 & page23->mask)));
                context18 = reg68;
                value13 = static_cast<uint32_t>((int16_t)value13);
                ++reg68->bus_word_count;
                value20 += 2;
                Emu68_Register(context18, k) = (int16_t)value13;
            }
            value17 >>= 1;
        }
    }
    return value13;
}

std::intptr_t Emu68_MoveMultipleLongToRegisters() {
    uint32_t pc;
    reg68_page_map_entry_t *page1;
    uint16_t value2;
    reg68_ctx_t *context3;
    uint16_t value4;
    int64_t value5;
    uint32_t value6;
    int64_t i;
    reg68_page_map_entry_t *page8;
    uint32_t value9;
    int64_t j;
    reg68_page_map_entry_t *page11;
    uint32_t value12;
    int64_t result;
    reg68_page_map_entry_t *page14;
    uint16_t value15;
    reg68_ctx_t *context16;
    uint16_t value17;
    reg68_ctx_t *context18;
    int64_t value19;
    unsigned int value20;
    reg68_page_map_entry_t *page21;
    int64_t k;
    reg68_page_map_entry_t *page23;

    pc = reg68->pc;
    if ((g_emu68Instruction & 0x38) == 0x18) {
        page1 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value2 = page1->read16(page1->base + (pc & page1->mask));
        context3 = reg68;
        value4 = value2;
        value5 = g_emu68Instruction & 7;
        ++reg68->bus_word_count;
        context3->pc += 2;
        value6 = context3->a[value5];
        for (i = 0; i < 8; ++i) {
            if ((value4 & 1) != 0) {
                page8 = &context3->map[static_cast<uint8_t>((value6) >> 16)];
                value9 = page8->read32(page8->base + (value6 & page8->mask));
                context3 = reg68;
                reg68->bus_word_count += 2;
                value6 += 4;
                Emu68_Register(context3, i) = value9;
            }
            value4 >>= 1;
        }
        for (j = 8; j < 16; ++j) {
            if ((value4 & 1) != 0) {
                page11 = &context3->map[static_cast<uint8_t>((value6) >> 16)];
                value12 = page11->read32(page11->base + (value6 & page11->mask));
                context3 = reg68;
                reg68->bus_word_count += 2;
                value6 += 4;
                Emu68_Register(context3, j) = value12;
            }
            value4 >>= 1;
        }
        result = g_emu68Instruction & 7;
        context3->a[result] = value6;
    } else {
        page14 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value15 = page14->read16(page14->base + (pc & page14->mask));
        context16 = reg68;
        value17 = value15;
        ++reg68->bus_word_count;
        context16->pc += 2;
        result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context18 = reg68;
        value19 = 0;
        value20 = result;
        do {
            if ((value17 & 1) != 0) {
                page21 = &context18->map[static_cast<uint8_t>((value20) >> 16)];
                result = page21->read32(page21->base + (value20 & page21->mask));
                context18 = reg68;
                reg68->bus_word_count += 2;
                value20 += 4;
                Emu68_Register(context18, value19) = result;
            }
            value17 >>= 1;
            ++value19;
        } while (value19 < 8);
        for (k = 8; k < 16; ++k) {
            if ((value17 & 1) != 0) {
                page23 = &context18->map[static_cast<uint8_t>((value20) >> 16)];
                result = page23->read32(page23->base + (value20 & page23->mask));
                context18 = reg68;
                reg68->bus_word_count += 2;
                value20 += 4;
                Emu68_Register(context18, k) = result;
            }
            value17 >>= 1;
        }
    }
    return result;
}

std::intptr_t Emu68_TestAndSet() {
    int8_t value5;
    reg68_ctx_t *context6;
    int8_t value7;
    int8_t value8;
    int8_t value9;
    unsigned int value10;

    if ((g_emu68Instruction & 0x3F) == 0x3C)
        return emu68_HandleInterruptRequest(4u);
    value5 = (int8_t)kEmu68_ReadModifyByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
    context6 = reg68;
    value7 = 0;
    value8 = value5;
    reg68->sr.raw[0] &= 0xF0u;
    value9 = 8 * (value5 < 0);
    if (!value5)
        value7 = 4;
    value10 = (uint16_t)g_emu68Instruction;
    context6->sr.raw[0] |= value7 | value9;
    return kEmu68_WriteByte[(value10 >> 3) & 7](value10 & 7, value8 | 0x80u);
}

std::intptr_t Emu68_SystemInstruction() {
    int64_t result;
    int16_t value4;
    reg68_ctx_t *context5;
    int64_t value6;
    int value7;
    int64_t value8;
    reg68_page_map_entry_t *page9;
    reg68_ctx_t *context10;
    reg68_ctx_t *context11;
    int64_t value12;
    uint32_t value13;
    reg68_page_map_entry_t *page14;
    reg68_ctx_t *context15;
    reg68_sr_bits_t status16;
    reg68_sr_bits_t status17;

    result = g_emu68Instruction & 0x3F;
    switch (g_emu68Instruction & 0x3F) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
        result = emu68_HandleInterruptRequest((g_emu68Instruction & 0xFu) + 32);
        break;
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
        value4 = Emu68_FetchWord();
        context5 = reg68;
        value6 = g_emu68Instruction & 7;
        value7 = value4;
        value8 = reg68->a[value6];
        reg68->a[7] -= 4;
        page9 = &context5->map[(uint8_t) static_cast<uint8_t>((context5->a[7]) >> 16)];
        (page9->write32(page9->base + (context5->a[7] & page9->mask), value8), 0);
        context10 = reg68;
        reg68->bus_word_count += 2;
        result = context10->a[7];
        context10->a[value6] = result;
        context10->a[7] += value7;
        break;
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
        context11 = reg68;
        value12 = g_emu68Instruction & 7;
        value13 = reg68->a[value12];
        reg68->a[7] = value13;
        page14 = &context11->map[static_cast<uint8_t>((value13) >> 16)];
        result = page14->read32(page14->base + (value13 & page14->mask));
        context15 = reg68;
        reg68->bus_word_count += 2;
        context15->a[7] += 4;
        context15->a[value12] = result;
        break;
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:
    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x2F:
        result = Emu68_ReportRuntimeError("USP", (uint16_t)g_emu68Instruction);
        break;
    case 0x30:
        result = Emu68_ResetOtherCpu();
        break;
    case 0x31:
        return result;
    case 0x32:
        status16.bits = Emu68_FetchWord();
        Emu68_SetStatusRegister(status16);
        result = Emu68_ReportRuntimeError("STOP", (uint16_t)g_emu68Instruction);
        break;
    case 0x33:
        result = Emu68_ReturnException();
        break;
    case 0x35:
        result = Emu68_PopLong();
        reg68->pc = result;
        break;
    case 0x36:
        result = (int64_t)reg68;
        if ((reg68->sr.raw[0] & 2) != 0)
            result = emu68_HandleInterruptRequest(7u);
        break;
    case 0x37:
        status17.bits = Emu68_PopWord();
        Emu68_SetStatusRegister(status17);
        result = Emu68_PopLong();
        reg68->pc = result;
        break;
    default:
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "emu68 runtime error [%s];%d\n", "INST", (uint16_t)g_emu68Instruction);
        (Harrier_DebugPrint(g_emu68DebugText), 0);
        std::snprintf(g_emu68DebugText, sizeof(g_emu68DebugText), "CPUID:%d PC=%08X\n", reg68->cpu_id, reg68->pc);
        result = (Harrier_DebugPrint(g_emu68DebugText), 0);
        break;
    }
    return result;
}

std::intptr_t Emu68_JumpSubroutine() {
    uint32_t value0;
    reg68_ctx_t *context1;
    uint32_t value2;
    int64_t pc;
    reg68_page_map_entry_t *page4;
    reg68_ctx_t *result;

    value0 = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    context1 = reg68;
    value2 = value0;
    pc = reg68->pc;
    reg68->a[7] -= 4;
    page4 = &context1->map[(uint8_t) static_cast<uint8_t>((context1->a[7]) >> 16)];
    (page4->write32(page4->base + (context1->a[7] & page4->mask), pc), 0);
    result = reg68;
    reg68->bus_word_count += 2;
    result->pc = value2;
    return reinterpret_cast<std::intptr_t>(result);
}

std::intptr_t Emu68_Jump() {
    int64_t result;

    result = kEmu68_Address[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
    reg68->pc = result;
    return result;
}

void Emu68_DispatchGroup4() {
    kEmu68_Group4[(g_emu68Instruction >> 6) & 0x3F]();
}

std::intptr_t Emu68_AddQuickByte() {
    int64_t result;

    if ((g_emu68Instruction & 0x38) != 8)
        return Emu68_AddToEffectiveByte(kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
    result = (unsigned int)kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    reg68->a[g_emu68Instruction & 7] += result;
    return result;
}

std::intptr_t Emu68_AddQuickWord() {
    int64_t result;

    if ((g_emu68Instruction & 0x38) != 8)
        return Emu68_AddToEffectiveWord(kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
    result = (unsigned int)kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    reg68->a[g_emu68Instruction & 7] += result;
    return result;
}

std::intptr_t Emu68_AddQuickLong() {
    int64_t result;

    if ((g_emu68Instruction & 0x38) != 8)
        return Emu68_AddToEffectiveLong(kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
    result = (unsigned int)kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    reg68->a[g_emu68Instruction & 7] += result;
    return result;
}

std::intptr_t Emu68_SubtractQuickByte() {
    int64_t result;

    if ((g_emu68Instruction & 0x38) != 8)
        return Emu68_SubtractFromEffectiveByte(kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);

    result = (unsigned int)kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    reg68->a[g_emu68Instruction & 7] -= result;
    return result;
}

std::intptr_t Emu68_SubtractQuickWord() {
    int64_t result;

    if ((g_emu68Instruction & 0x38) != 8)
        return Emu68_SubtractFromEffectiveWord(kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
    result = (unsigned int)kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    reg68->a[g_emu68Instruction & 7] -= result;
    return result;
}

std::intptr_t Emu68_SubtractQuickLong() {
    int64_t result;

    if ((g_emu68Instruction & 0x38) != 8)
        return Emu68_SubtractFromEffectiveLong(kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
    result = (unsigned int)kEmu68_QuickCount[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7];
    reg68->a[g_emu68Instruction & 7] -= result;
    return result;
}

std::intptr_t Emu68_SetOrDecrementCondition() {
    reg68_ctx_t *context4;
    int value5;
    uint8_t value6;
    uint8_t value7;
    int64_t value8;
    int value9;
    int16_t value10;

    context4 = reg68;
    switch (static_cast<uint8_t>((g_emu68Instruction) >> ((sizeof(g_emu68Instruction) - 1) * 8)) & 0xF) {
    case 0:
        goto LABEL_20;
    case 1:
        goto LABEL_19;
    case 2:
        value5 = (reg68->sr.raw[0] & 5) == 0;
        break;
    case 3:
        value5 = (reg68->sr.raw[0] & 5) != 0;
        break;
    case 4:
        value5 = (reg68->sr.raw[0] & 1) == 0;
        break;
    case 5:
        value5 = reg68->sr.raw[0] & 1;
        break;
    case 6:
        value5 = (reg68->sr.raw[0] & 4) == 0;
        break;
    case 7:
        value5 = reg68->sr.raw[0] & 4;
        break;
    case 8:
        value5 = (reg68->sr.raw[0] & 2) == 0;
        break;
    case 9:
        value5 = reg68->sr.raw[0] & 2;
        break;
    case 0xA:
        value5 = (reg68->sr.raw[0] & 8) == 0;
        break;
    case 0xB:
        value5 = reg68->sr.raw[0] & 8;
        break;
    case 0xC:
        value5 = (((uint8_t)reg68->sr.raw[0] ^ (unsigned int)(uint8_t) ~((uint8_t)reg68->sr.raw[0] >> 2)) >> 1) & 1;
        break;
    case 0xD:
        value5 = ((unsigned int)((uint8_t)reg68->sr.raw[0] ^ ((uint8_t)reg68->sr.raw[0] >> 2)) >> 1) & 1;
        break;
    case 0xE:
        value6 = reg68->sr.raw[0];
        if (((value6 ^ (value6 >> 2)) & 2) != 0)
            goto LABEL_19;
        if ((value6 & 4) == 0)
            goto LABEL_20;
        value5 = 0;
        break;
    default:
        value7 = reg68->sr.raw[0];
        if (((value7 ^ (value7 >> 2)) & 2) != 0 || (value7 & 4) != 0)
        LABEL_20:
            value5 = 1;
        else
        LABEL_19:
            value5 = 0;
        break;
    }
    value8 = g_emu68Instruction & 7;
    value9 = static_cast<uint8_t>(g_emu68Instruction & 0x38);
    if ((g_emu68Instruction & 0x38) == 8) {
        if (value5) {
            reg68->pc += 2;
        } else {
            if (static_cast<uint16_t>(reg68->d[value8])) {
                value10 = Emu68_FetchWord();
                context4 = reg68;
                value9 = value10 - 2;
                reg68->pc += value9;
            } else {
                reg68->pc += 2;
            }
            context4->d[value8] = (context4->d[value8] & 0xffff0000U) | static_cast<uint16_t>(context4->d[value8] - 1U);
        }
    } else {
        kEmu68_SetEffectiveByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
        value9 = static_cast<uint8_t>((uint8_t)kEmu68_WriteByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, -(value5 != 0)));
    }
    return value9;
}

void Emu68_DispatchGroup5() {
    kEmu68_Group5[((uint64_t)(uint16_t)g_emu68Instruction >> 6) & 7]();
    return;
}

void Emu68_Branch() {
    reg68_ctx_t *context0;
    uint16_t value1;
    uint32_t pc;
    int value3;
    reg68_page_map_entry_t *page4;
    uint32_t value5;
    int value6;
    bool value7;
    uint8_t value8;
    uint8_t value9;

    context0 = reg68;
    value1 = g_emu68Instruction;
    pc = reg68->pc;
    if ((uint8_t)g_emu68Instruction) {
        value3 = (int8_t)g_emu68Instruction;
    } else {
        page4 = &reg68->map[static_cast<uint8_t>((pc) >> 16)];
        value3 = static_cast<uint16_t>(page4->read16(page4->base + (pc & page4->mask)));
        context0 = reg68;
        value1 = g_emu68Instruction;
        value3 = (int16_t)value3;
        ++reg68->bus_word_count;
        context0->pc += 2;
    }
    value5 = value3 + pc;
    value6 = static_cast<uint8_t>((value1) >> ((sizeof(value1) - 1) * 8)) & 0xF;
    switch (value6) {
    case 0:
        goto LABEL_30;
    case 1:
        Emu68_PushLong(context0->pc);
        reg68->pc = value5;
        return;
    case 2:
        if ((context0->sr.raw[0] & 5) == 0)
            context0->pc = value5;
        return;
    case 3:
        value7 = (context0->sr.raw[0] & 5) == 0;
        goto LABEL_29;
    case 4:
        if ((context0->sr.raw[0] & 1) == 0)
            context0->pc = value5;
        return;
    case 5:
        value7 = (context0->sr.raw[0] & 1) == 0;
        goto LABEL_29;
    case 6:
        if ((context0->sr.raw[0] & 4) == 0)
            context0->pc = value5;
        return;
    case 7:
        value7 = (context0->sr.raw[0] & 4) == 0;
        goto LABEL_29;
    case 8:
        if ((context0->sr.raw[0] & 2) == 0)
            context0->pc = value5;
        return;
    case 9:
        value7 = (context0->sr.raw[0] & 2) == 0;
        goto LABEL_29;
    case 10:
        if ((context0->sr.raw[0] & 8) == 0)
            context0->pc = value5;
        return;
    case 11:
        value7 = (context0->sr.raw[0] & 8) == 0;
        goto LABEL_29;
    case 12:
        if (((context0->sr.raw[0] ^ ((uint8_t)context0->sr.raw[0] >> 2)) & 2) == 0)
            context0->pc = value5;
        return;
    case 13:
        value7 = ((context0->sr.raw[0] ^ ((uint8_t)context0->sr.raw[0] >> 2)) & 2) == 0;
        goto LABEL_29;
    case 14:
        value8 = context0->sr.raw[0];
        value6 = static_cast<uint8_t>(value8 ^ (value8 >> 2));
        if ((value6 & 2) == 0 && (value8 & 4) == 0)
            context0->pc = value5;
        return;
    case 15:
        value9 = context0->sr.raw[0];
        value6 = static_cast<uint8_t>(value9 ^ (value9 >> 2));
        if ((value6 & 2) != 0)
            goto LABEL_30;
        value7 = (value9 & 4) == 0;
    LABEL_29:
        if (!value7)
        LABEL_30:
            context0->pc = value5;
        return;
    }
}

void Emu68_MoveQuick() {
    reg68_ctx_t *context0;
    int value1;
    int64_t result;

    context0 = reg68;
    value1 = (int8_t)g_emu68Instruction;
    result = 0;
    reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7] = (int8_t)g_emu68Instruction;
    context0->sr.raw[0] &= 0xF0u;
    if (!value1)
        result = 4;
    context0->sr.raw[0] |= (uint8_t)result | (uint8_t)(8 * (value1 < 0));
    return;
}

void Emu68_DispatchGroup8() {
    int64_t value4;
    int8_t value5;
    reg68_ctx_t *context6;
    int8_t value7;
    int8_t value8;
    int16_t value9;
    reg68_ctx_t *context10;
    int8_t value11;
    int16_t value12;
    int value13;
    reg68_ctx_t *context14;
    int8_t value15;
    signed int value16;

    value4 = ((uint16_t)g_emu68Instruction >> 9) & 7;
    switch (((uint16_t)g_emu68Instruction >> 6) & 7) {
    case 0:
        value5 = (int8_t)kEmu68_ReadByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
        context6 = reg68;
        value7 = 0;
        value8 = value5 | static_cast<uint8_t>(reg68->d[value4]);
        reg68->d[value4] = (reg68->d[value4] & ~uint32_t{0xFF}) | static_cast<uint8_t>(value8);
        context6->sr.raw[0] &= 0xF0u;
        if (!value8)
            value7 = 4;
        context6->sr.raw[0] |= value7 | (uint8_t)(8 * (value8 < 0));
        break;
    case 1:
        value9 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context10 = reg68;
        value11 = 0;
        value12 = value9 | static_cast<uint16_t>(reg68->d[value4]);
        reg68->d[value4] = (reg68->d[value4] & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value12);
        context10->sr.raw[0] &= 0xF0u;
        if (!value12)
            value11 = 4;
        context10->sr.raw[0] |= value11 | (uint8_t)(8 * (value12 < 0));
        break;
    case 2:
        value13 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context14 = reg68;
        value15 = 0;
        value16 = value13 | reg68->d[value4];
        reg68->d[value4] = value16;
        context14->sr.raw[0] &= 0xF0u;
        if (!value16)
            value15 = 4;
        context14->sr.raw[0] |= value15 | (uint8_t)(8 * (value16 < 0));
        break;
    case 3:
        Emu68_DivideUnsigned(value4);
        break;
    case 4:
        if ((g_emu68Instruction & 0x30) != 0)
            Emu68_OrEffectiveByte(reg68->d[value4]);
        else
            Emu68_SubtractBcd();
        break;
    case 5:
        Emu68_OrEffectiveWord(reg68->d[value4]);
        break;
    case 6:
        Emu68_OrEffectiveLong(reg68->d[value4]);
        break;
    case 7:
        Emu68_DivideSigned(value4);
        break;
    }
}

void Emu68_DispatchGroupC() {
    int64_t value3;
    int8_t value4;
    reg68_ctx_t *context5;
    int8_t value6;
    int value7;
    int8_t value8;
    signed int value9;
    int16_t value10;
    int16_t value11;
    int value12;
    reg68_ctx_t *context13;
    int8_t value14;
    reg68_ctx_t *context15;
    int value16;
    int value17;
    uint32_t *value18;
    signed int *value19;
    signed int value20;
    uint32_t value21;
    uint32_t value22;
    uint32_t *value23;
    int16_t value24;
    int value25;
    int8_t value26;

    value3 = ((uint16_t)g_emu68Instruction >> 9) & 7;
    switch (((uint16_t)g_emu68Instruction >> 6) & 7) {
    case 0:
        value4 = (int8_t)kEmu68_ReadByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
        context5 = reg68;
        value6 = reg68->d[value3] & value4;
        reg68->d[value3] = (reg68->d[value3] & ~uint32_t{0xFF}) | static_cast<uint8_t>(value6);
        value7 = value6;
        goto LABEL_3;
    case 1:
        value10 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context5 = reg68;
        value11 = reg68->d[value3] & value10;
        reg68->d[value3] = (reg68->d[value3] & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value11);
        value7 = value11;
    LABEL_3:
        context5->sr.raw[0] &= 0xF0u;
        value8 = 0;
        if (!value7)
            value8 = 4;
        value9 = static_cast<uint8_t>(value8 | (8 * (value7 < 0)));
        context5->sr.raw[0] |= value9;
        return;
    case 2:
        value12 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context13 = reg68;
        value14 = 0;
        value9 = reg68->d[value3] & value12;
        reg68->d[value3] = value9;
        context13->sr.raw[0] &= 0xF0u;
        if (!value9)
            value14 = 4;
        context13->sr.raw[0] |= value14 | (uint8_t)(8 * (value9 < 0));
        return;
    case 3:
        value16 = static_cast<uint16_t>(kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7));
        context15 = reg68;
        value16 = (uint16_t)value16;
        value17 = static_cast<uint16_t>(reg68->d[value3]);
        goto LABEL_23;
    case 4:
        if ((g_emu68Instruction & 0x30) != 0)
            value9 = static_cast<uint8_t>(Emu68_AndEffectiveByte(reg68->d[value3]));
        else
            value9 = static_cast<uint8_t>(Emu68_AddBcd());
        return;
    case 5:
        if ((g_emu68Instruction & 0x30) != 0) {
            value9 = static_cast<uint8_t>(Emu68_AndEffectiveWord(reg68->d[value3]));
        } else {
            value18 = &reg68->d[value3];
            value19 = (signed int *)&reg68->d[g_emu68Instruction & 7];
            if ((g_emu68Instruction & 8) != 0) {
                value20 = reg68->a[value3];
                value9 = reg68->a[g_emu68Instruction & 7];
                reg68->a[value3] = value9;
                reg68->a[g_emu68Instruction & 7] = value20;
            } else {
                value21 = *value18;
                value9 = *value19;
                *value18 = *value19;
                *value19 = value21;
            }
        }
        return;
    case 6:
        if ((g_emu68Instruction & 0x38) == 8) {
            value22 = reg68->d[value3];
            value23 = &reg68->a[g_emu68Instruction & 7];
            value9 = *value23;
            reg68->d[value3] = value9;
            *value23 = value22;
        } else {
            value9 = static_cast<uint8_t>(Emu68_AndEffectiveLong(reg68->d[value3]));
        }
        return;
    case 7:
        value24 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        context15 = reg68;
        value17 = value24;
        value16 = static_cast<int16_t>(reg68->d[value3]);
    LABEL_23:
        value25 = Emu68_Multiply32(value16, value17);
        value26 = 0;
        context15->d[value3] = value25;
        context15->sr.raw[0] &= 0xF0u;
        if (!value25)
            value26 = 4;
        value9 = static_cast<uint8_t>(value26 | (8 * (value25 < 0)));
        context15->sr.raw[0] |= value9;
        return;
    }
}

void Emu68_DispatchGroupB() {
    int64_t value4;
    int8_t value5;
    int8_t result;
    int16_t value7;
    int value8;
    int16_t value9;
    int value10;

    value4 = ((uint16_t)g_emu68Instruction >> 9) & 7;
    switch (((uint16_t)g_emu68Instruction >> 6) & 7) {
    case 0:
        value5 = (int8_t)kEmu68_ReadByte[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7]((g_emu68Instruction & 7));
        result = Emu68_CompareFlags(value5, static_cast<int8_t>(reg68->d[value4]), 128);
        break;
    case 1:
        value7 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        result = Emu68_CompareFlags(value7, static_cast<int16_t>(reg68->d[value4]), 0x8000);
        break;
    case 2:
        value8 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        result = Emu68_CompareFlags(value8, reg68->d[value4], 0x80000000);
        break;
    case 3:
        value9 = kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        result = Emu68_CompareFlags(value9, reg68->a[value4], 0x80000000);
        break;
    case 4:
        if ((g_emu68Instruction & 0x38) == 8)
            result = Emu68_CompareMemoryByte();
        else
            result = Emu68_XorEffectiveByte(reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
        break;
    case 5:
        if ((g_emu68Instruction & 0x38) == 8)
            result = Emu68_CompareMemoryWord();
        else
            result = Emu68_XorEffectiveWord(reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
        break;
    case 6:
        if ((g_emu68Instruction & 0x38) == 8)
            result = Emu68_CompareMemoryLong();
        else
            result = Emu68_XorEffectiveLong(reg68->d[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7]);
        break;
    case 7:
        value10 = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        result = Emu68_CompareFlags(value10, reg68->a[value4], 0x80000000);
        break;
    }
    return;
}

void Emu68_DispatchGroup9() {
    int64_t result;
    int value5;

    switch (((uint16_t)g_emu68Instruction >> 6) & 7) {
    case 0:
        result = Emu68_SubtractEffectiveFromDataByte();
        break;
    case 1:
        result = Emu68_SubtractEffectiveFromDataWord();
        break;
    case 2:
        result = Emu68_SubtractEffectiveFromDataLong();
        break;
    case 3:
        value5 = (int16_t)kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        result = (int64_t)reg68;
        reg68->a[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7] -= value5;
        break;
    case 4:
        if ((g_emu68Instruction & 0x30) != 0)
            result = Emu68_SubtractDataFromEffectiveByte();
        else
            result = Emu68_SubtractExtendByte();
        break;
    case 5:
        if ((g_emu68Instruction & 0x30) != 0)
            result = Emu68_SubtractDataFromEffectiveWord();
        else
            result = Emu68_SubtractExtendWord();
        break;
    case 6:
        if ((g_emu68Instruction & 0x30) != 0)
            result = Emu68_SubtractDataFromEffectiveLong();
        else
            result = Emu68_SubtractExtendLong();
        break;
    case 7:
        result = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        reg68->a[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7] -= result;
        break;
    }
    return;
}

void Emu68_DispatchGroupD() {
    int64_t result;
    int value5;

    switch (((uint16_t)g_emu68Instruction >> 6) & 7) {
    case 0:
        result = Emu68_AddEffectiveToDataByte();
        break;
    case 1:
        result = Emu68_AddEffectiveToDataWord();
        break;
    case 2:
        result = Emu68_AddEffectiveToDataLong();
        break;
    case 3:
        value5 = (int16_t)kEmu68_ReadWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        result = (int64_t)reg68;
        reg68->a[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7] += value5;
        break;
    case 4:
        if ((g_emu68Instruction & 0x30) != 0)
            result = Emu68_AddDataToEffectiveByte();
        else
            result = Emu68_AddExtendByte();
        break;
    case 5:
        if ((g_emu68Instruction & 0x30) != 0)
            result = Emu68_AddDataToEffectiveWord();
        else
            result = Emu68_AddExtendWord();
        break;
    case 6:
        if ((g_emu68Instruction & 0x30) != 0)
            result = Emu68_AddDataToEffectiveLong();
        else
            result = Emu68_AddExtendLong();
        break;
    case 7:
        result = kEmu68_ReadLong[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
        reg68->a[((uint64_t)(uint16_t)g_emu68Instruction >> 9) & 7] += result;
        break;
    }
    return;
}

void Emu68_ShiftRotate() {
    int64_t value0;
    int64_t value1;
    int64_t value2;
    int16_t value3;
    int16_t value4;
    reg68_ctx_t *context5;
    int8_t value6;
    int64_t value7;
    reg68_ctx_t *context8;
    int8_t value9;
    int64_t value10;
    int16_t value11;
    reg68_ctx_t *context12;
    int value13;
    int64_t value14;
    int8_t value15;
    int8_t value16;
    Emu68ShiftHandler shiftHandlers[9];

    shiftHandlers[0] = Emu68_ArithmeticShiftRight;
    shiftHandlers[1] = Emu68_LogicalShiftRight;
    value0 = g_emu68Instruction & 7;
    shiftHandlers[2] = Emu68_RotateExtendRight;
    value1 = ((uint16_t)g_emu68Instruction >> 9) & 7;
    shiftHandlers[3] = Emu68_RotateRight;
    shiftHandlers[4] = Emu68_ArithmeticShiftLeft;
    shiftHandlers[5] = Emu68_LogicalShiftLeft;
    shiftHandlers[6] = Emu68_RotateExtendLeft;
    shiftHandlers[7] = Emu68_RotateLeft;
    if ((g_emu68Instruction & 0x20) != 0)
        value2 = reg68->d[value1] & 0x3F;
    else
        value2 = (unsigned int)kEmu68_QuickCount[value1];
    if ((uint8_t)g_emu68Instruction >> 6) {
        if ((uint8_t)g_emu68Instruction >> 6 != 1) {
            if ((uint8_t)g_emu68Instruction >> 6 == 2) {
                value7 = value0;
                value1 = static_cast<uint32_t>(shiftHandlers[4 * ((g_emu68Instruction >> 8) & 1) + ((g_emu68Instruction >> 3) & 3)](
                    reg68->d[value0], value2, static_cast<int32_t>(0x80000000U)));
                context8 = reg68;
                reg68->d[value7] = value1;
                context8->sr.raw[0] &= 0xF3u;
                value9 = 2 * ((int)value1 < 0);
                value1 = static_cast<uint8_t>((uint32_t)value1 == 0);
                context8->sr.raw[0] |= 4 * (value1 | value9);
            } else if ((uint8_t)g_emu68Instruction >> 6 == 3) {
                value3 = kEmu68_ReadModifyWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7);
                value4 = shiftHandlers[4 * ((g_emu68Instruction >> 8) & 1) + ((g_emu68Instruction >> 9) & 3)](static_cast<int32_t>(value3), 1, 0x8000);
                kEmu68_WriteWord[((uint64_t)(uint16_t)g_emu68Instruction >> 3) & 7](g_emu68Instruction & 7, value4);
                context5 = reg68;
                value6 = 0;
                value1 = static_cast<uint8_t>(value4);
                reg68->sr.raw[0] &= 0xF3u;
                if (!value4)
                    value6 = 4;
                context5->sr.raw[0] |= value6 | (uint8_t)(8 * (value4 < 0));
            }
            return;
        }
        value10 = value0;
        value11 = shiftHandlers[4 * ((g_emu68Instruction >> 8) & 1) + ((g_emu68Instruction >> 3) & 3)](static_cast<int16_t>(reg68->d[value0]), value2, 0x8000);
        context12 = reg68;
        value13 = value11;
        reg68->d[value10] = (reg68->d[value10] & ~uint32_t{0xFFFF}) | static_cast<uint16_t>(value11);
    } else {
        value14 = value0;
        value15 = shiftHandlers[4 * ((g_emu68Instruction >> 8) & 1) + ((g_emu68Instruction >> 3) & 3)](static_cast<int8_t>(reg68->d[value0]), value2, 128);
        context12 = reg68;
        value13 = value15;
        reg68->d[value14] = (reg68->d[value14] & ~uint32_t{0xFF}) | static_cast<uint8_t>(value15);
    }
    context12->sr.raw[0] &= 0xF3u;
    value16 = 0;
    if (!value13)
        value16 = 4;
    value1 = static_cast<uint8_t>(value16 | (8 * (value13 < 0)));
    context12->sr.raw[0] |= value1;
    return;
}

void emu68_Run() {
    g_reg68_ctx.bus_word_count = 0;
    for (unsigned phase = 0; phase < 6; ++phase) {
        do {
            reg68 = &g_reg68_ctx;
            reg68_page_map_entry_t &mainPage = g_reg68_ctx.map[(g_reg68_ctx.pc >> 16) & 0xFF];
            g_emu68Instruction = mainPage.read16(mainPage.base + (g_reg68_ctx.pc & mainPage.mask));
            ++reg68->bus_word_count;
            reg68->pc += 2;
            g_emu68OpcodeDispatch[g_emu68Instruction >> 12]();

            reg68 = &g_reg68_ctx_2;
            reg68_page_map_entry_t &subPage = g_reg68_ctx_2.map[(g_reg68_ctx_2.pc >> 16) & 0xFF];
            g_emu68Instruction = subPage.read16(subPage.base + (g_reg68_ctx_2.pc & subPage.mask));
            ++reg68->bus_word_count;
            reg68->pc += 2;
            g_emu68OpcodeDispatch[g_emu68Instruction >> 12]();
        } while (g_reg68_ctx.bus_word_count < kEmu68FrameBoundaries[phase]);

        const std::uint8_t *const request = phase < 4 ? &g_harrierIrqLevel2 : phase == 4 ? &g_harrierIrqLevel4 : &g_harrierIrqFrame;
        std::uint8_t level = *request;
        if (level) {
            reg68 = &g_reg68_ctx;
            if (level == 7 || (g_reg68_ctx.sr.raw[1] & 7) < level) {
                emu68_HandleInterruptRequest(level + 24);
                reg68->sr.raw[1] &= 0xA0;
                reg68->sr.raw[1] |= level;
            }
            level = *request;
            reg68 = &g_reg68_ctx_2;
            if (level == 7 || (g_reg68_ctx_2.sr.raw[1] & 7) < level) {
                emu68_HandleInterruptRequest(level + 24);
                reg68->sr.raw[1] &= 0xA0;
                reg68->sr.raw[1] |= level;
            }
        }
    }
}

void HarrierInit() {
    g_harrierMusicFileIndex = -1;
    const std::uint64_t fileSize = archive_data::ArchiveData_GetFileSize("XHAR\\datas\\HAR.ROM");
    const std::uint64_t loadSize = fileSize < kHarrierRomSize ? fileSize : kHarrierRomSize;
    if (loadSize)
        archive_data::ArchiveData_Read("XHAR\\datas\\HAR.ROM", 0, g_harrierMemory, loadSize);
    g_emu68OpcodeDispatch[0xA] = &emu68_instruction_handler;
    HarrierLoadAndInitResources();
    mallocHarrierSFX();
    Harrier_ResetAudioForCoreInit();
    g_harrierMemory[kHarrierDipSwitchOffset] = 0xff;
    g_harrierMemory[kHarrierDipSwitchOffset + 1] = 0xff;
    g_harrierMemory[kHarrierDipSwitchOffset + 2] = 0xff;
    g_harrierMemory[kHarrierDipSwitchOffset + 3] = 0xfd;
    g_harrierInputScaleX = 0.85000002F;
    g_harrierInputScaleY = -0.44999999F;
    g_harrierIrqFrame = 0;
    g_harrierIrqLevel2 = 2;
    g_harrierIrqLevel4 = 4;
    g_reg68_ctx.cpu_id = REG68_CPU_0;
    g_reg68_ctx_2.cpu_id = REG68_CPU_1;
    g_reg68_ctx.map = g_reg68_ctx_map_funcs.data();
    g_reg68_ctx_2.map = g_reg68_ctx_map_funcs_2.data();
    reg68 = &g_reg68_ctx;
    Emu68_ResetContext();
    reg68 = &g_reg68_ctx_2;
    Emu68_ResetContext();
    reg68 = &g_reg68_ctx;

    for (unsigned record = 0; emu68_insn_defs[record].asm_string; ++record) {
        emu68_ctx_t &target = emu68_insn_defs[record + 1];
        target.a = 0;
        target.pattern = 0;
        for (unsigned bit = 0; bit < 16; ++bit) {
            const char value = emu68_insn_defs[record].bits[bit];
            if (value == '1')
                target.a |= 1 << (15 - bit);
            if (value == '0' || value == '1')
                target.pattern |= 1 << (15 - bit);
        }
    }
    const std::int32_t *patch = kHarrierRomPatches;
    while (*patch != -1) {
        std::uint32_t destination = static_cast<std::uint32_t>(*patch++);
        while (*patch != -1) {
            const std::uint16_t value = static_cast<std::uint16_t>(*patch++);
            g_harrierMemory[destination++] = static_cast<std::uint8_t>(value >> 8);
            g_harrierMemory[destination++] = static_cast<std::uint8_t>(value);
        }
        ++patch;
    }
}

} // namespace shenmue::s1
