#pragma once

#include "harrier.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr std::size_t kHarrierMemorySize = 0x100000;
inline constexpr std::uint32_t kHarrierRomSize = 0x50000;
inline constexpr std::uint32_t kEmu68FrameBoundaries[6] = {10000, 20000, 30000, 40000, 43750, 50000};
inline constexpr std::uint8_t kEmu68_QuickCount[8] = {8, 1, 2, 3, 4, 5, 6, 7};

using reg68_read8_fn = std::uint8_t (*)(std::uint32_t);
using reg68_write8_fn = void (*)(std::uint32_t, std::uint8_t);
using reg68_read16_fn = std::uint16_t (*)(std::uint32_t);
using reg68_write16_fn = void (*)(std::uint32_t, std::uint16_t);
using reg68_read32_fn = std::uint32_t (*)(std::uint32_t);
using reg68_write32_fn = void (*)(std::uint32_t, std::uint32_t);

struct reg68_page_map_entry_t {
    reg68_read8_fn read8;
    reg68_write8_fn write8;
    reg68_read16_fn read16;
    reg68_write16_fn write16;
    reg68_read32_fn read32;
    reg68_write32_fn write32;
    std::uint32_t base;
    std::uint32_t mask;
};
static_assert(sizeof(reg68_page_map_entry_t) == 56);
static_assert(offsetof(reg68_page_map_entry_t, base) == 0x30);
static_assert(offsetof(reg68_page_map_entry_t, mask) == 0x34);

union reg68_sr_bits_t {
    std::uint16_t bits;
    std::uint8_t raw[2];
};
enum reg68_cpu_id_t : std::uint16_t { REG68_CPU_0, REG68_CPU_1 };
struct reg68_ctx_t {
    std::uint32_t d[8];
    std::uint32_t a[8];
    std::uint32_t pc;
    std::uint32_t pad_44;
    reg68_page_map_entry_t *map;
    std::uint32_t bus_word_count;
    reg68_sr_bits_t sr;
    reg68_cpu_id_t cpu_id;
};
static_assert(sizeof(reg68_ctx_t) == 88);
static_assert(offsetof(reg68_ctx_t, a) == 0x20);
static_assert(offsetof(reg68_ctx_t, pc) == 0x40);
static_assert(offsetof(reg68_ctx_t, map) == 0x48);
static_assert(offsetof(reg68_ctx_t, bus_word_count) == 0x50);
static_assert(offsetof(reg68_ctx_t, sr) == 0x54);
static_assert(offsetof(reg68_ctx_t, cpu_id) == 0x56);

#pragma pack(push, 4)
struct emu68_ctx_t {
    std::int32_t a;
    std::int32_t pattern;
    std::int32_t bitmask;
    std::int32_t b;
    const char *bits;
    const char *asm_string;
};
#pragma pack(pop)
static_assert(sizeof(emu68_ctx_t) == 32);
static_assert(alignof(emu68_ctx_t) == 4);
static_assert(offsetof(emu68_ctx_t, bits) == 0x10);
static_assert(offsetof(emu68_ctx_t, asm_string) == 0x18);

extern reg68_ctx_t g_reg68_ctx;
extern reg68_ctx_t g_reg68_ctx_2;
extern reg68_ctx_t *reg68;
extern std::uint16_t g_emu68Instruction;
extern std::uint32_t g_emu68_ea;
extern std::uint8_t g_harrierIrqLevel2;
extern std::uint8_t g_harrierIrqLevel4;
extern std::uint8_t g_harrierIrqFrame;

void emu68_Run();
std::uint8_t Harrier_ReadMemory8(std::uint32_t address);
std::uint16_t Harrier_ReadMemory16BigEndian(std::uint32_t address);
std::uint32_t Harrier_ReadMemory32BigEndian(std::uint32_t address);
void Harrier_WriteMemory8(std::uint32_t address, std::uint8_t value);
void Harrier_WriteMemory16BigEndian(std::uint32_t address, std::uint16_t value);
void Harrier_WriteMemory32BigEndian(std::uint32_t address, std::uint32_t value);
std::uint8_t Emu68_UnmappedRead8(std::uint32_t address);
std::uint16_t Emu68_UnmappedRead16(std::uint32_t address);
std::uint32_t Emu68_UnmappedRead32(std::uint32_t address);
void Emu68_IgnoreWrite8(std::uint32_t address, std::uint8_t value);
void Emu68_IgnoreWrite16(std::uint32_t address, std::uint16_t value);
void Emu68_IgnoreWrite32(std::uint32_t address, std::uint32_t value);
std::intptr_t Emu68_NullOperand(int index);
std::intptr_t Emu68_ReportWriteModeError(int index, std::uint32_t value);

using Emu68ReadOperand = std::intptr_t (*)(int);
using Emu68WriteOperand = std::intptr_t (*)(int, std::uint32_t);
using Emu68Operation = std::intptr_t (*)();
using Emu68OpcodeHandler = void (*)();
using Emu68ShiftHandler = std::intptr_t (*)(int, int, int);

void Emu68_ReportLineFError();
std::intptr_t Emu68_ReportRuntimeError(const char *Args, int a2);
std::intptr_t Emu68_ResetOtherCpu();
std::intptr_t Emu68_FetchWord();
std::intptr_t Emu68_DecodeBriefIndex(std::uint16_t a1);
std::intptr_t Emu68_ReportAddressModeError(int a1);
std::intptr_t Emu68_ReadAddressRegister(int a1);
std::intptr_t Emu68_AddressDisplacement(int a1);
std::intptr_t Emu68_AddressIndexed(int a1);
std::intptr_t Emu68_AddressExtended(int a1);
std::intptr_t Emu68_ReportByteReadModeError(int a1);
std::intptr_t Emu68_ReadDataByte(int a1);
std::intptr_t Emu68_ReadIndirectByte(int a1);
std::intptr_t Emu68_ReadPostincrementByte(int a1);
std::intptr_t Emu68_ReadPredecrementByte(int a1);
std::intptr_t Emu68_ReadDisplacementByte(int a1);
std::intptr_t Emu68_ReadIndexedByte(int a1);
std::intptr_t Emu68_ReadExtendedByte(int a1);
std::intptr_t Emu68_ReadDataWord(int a1);
std::intptr_t Emu68_ReadAddressWord(int a1);
std::intptr_t Emu68_ReadIndirectWord(int a1);
std::intptr_t Emu68_ReadPostincrementWord(int a1);
std::intptr_t Emu68_ReadPredecrementWord(int a1);
std::intptr_t Emu68_ReadDisplacementWord(int a1);
std::intptr_t Emu68_ReadIndexedWord(int a1);
std::intptr_t Emu68_ReadExtendedWord(int a1);
std::intptr_t Emu68_ReadDataLong(int a1);
std::intptr_t Emu68_ReadIndirectLong(int a1);
std::intptr_t Emu68_ReadPostincrementLong(int a1);
std::intptr_t Emu68_ReadPredecrementLong(int a1);
std::intptr_t Emu68_ReadDisplacementLong(int a1);
std::intptr_t Emu68_ReadIndexedLong(int a1);
std::intptr_t Emu68_ReadExtendedLong(int a1);
std::intptr_t Emu68_ReadModifyIndirectByte(int a1);
std::intptr_t Emu68_ReadModifyPredecrementByte(int a1);
std::intptr_t Emu68_ReadModifyDisplacementByte(int a1);
std::intptr_t Emu68_ReadModifyIndexedByte(int a1);
std::intptr_t Emu68_ReadModifyExtendedByte(int a1);
std::intptr_t Emu68_ReadModifyIndirectWord(int a1);
std::intptr_t Emu68_ReadModifyPredecrementWord(int a1);
std::intptr_t Emu68_ReadModifyDisplacementWord(int a1);
std::intptr_t Emu68_ReadModifyIndexedWord(int a1);
std::intptr_t Emu68_ReadModifyExtendedWord(int a1);
std::intptr_t Emu68_ReadModifyIndirectLong(int a1);
std::intptr_t Emu68_ReadModifyPredecrementLong(int a1);
std::intptr_t Emu68_ReadModifyDisplacementLong(int a1);
std::intptr_t Emu68_ReadModifyIndexedLong(int a1);
std::intptr_t Emu68_ReadModifyExtendedLong(int a1);
std::intptr_t Emu68_WriteDataByte(int a1, std::uint32_t a2);
std::intptr_t Emu68_ReportWriteModeError(int a1);
std::intptr_t Emu68_WriteEffectiveByte(int a1, std::uint32_t a2);
std::intptr_t Emu68_WritePostincrementByte(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteExtendedByte(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteDataWord(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteAddressWord(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteEffectiveWord(int a1, std::uint32_t a2);
std::intptr_t Emu68_WritePostincrementWord(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteExtendedWord(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteDataLong(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteAddressLong(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteEffectiveLong(int a1, std::uint32_t a2);
std::intptr_t Emu68_WritePostincrementLong(int a1, std::uint32_t a2);
std::intptr_t Emu68_WriteExtendedLong(int a1, std::uint32_t a2);
std::intptr_t Emu68_SetEffectiveIndirect(int a1);
std::intptr_t Emu68_SetEffectivePredecrementByte(int a1);
std::intptr_t Emu68_SetEffectiveDisplacement(int a1);
std::intptr_t Emu68_SetEffectiveIndexed(int a1);
std::intptr_t Emu68_SetEffectiveExtended(int a1);
std::intptr_t Emu68_SetEffectivePredecrementWord(int a1);
std::intptr_t Emu68_SetEffectivePredecrementLong(int a1);
std::intptr_t Emu68_CompareFlags(int a1, int a2, int a3);
std::intptr_t Emu68_PushLong(unsigned int a1);
std::intptr_t Emu68_PopWord();
std::intptr_t Emu68_PopLong();
std::intptr_t Emu68_SetStatusRegister(reg68_sr_bits_t a1);
std::intptr_t Emu68_AddToEffectiveByte(std::int8_t a1);
std::intptr_t Emu68_AddToEffectiveWord(std::int16_t a1);
std::intptr_t Emu68_AddToEffectiveLong(int a1);
std::intptr_t Emu68_SubtractFromEffectiveByte(std::int8_t a1);
std::intptr_t Emu68_SubtractFromEffectiveWord(std::int16_t a1);
std::intptr_t Emu68_SubtractFromEffectiveLong(int a1);
std::intptr_t Emu68_AddDataToEffectiveByte();
std::intptr_t Emu68_AddDataToEffectiveWord();
std::intptr_t Emu68_AddDataToEffectiveLong();
std::intptr_t Emu68_SubtractDataFromEffectiveByte();
std::intptr_t Emu68_SubtractDataFromEffectiveWord();
std::intptr_t Emu68_SubtractDataFromEffectiveLong();
std::intptr_t Emu68_AddEffectiveToDataByte();
std::intptr_t Emu68_AddEffectiveToDataWord();
std::intptr_t Emu68_AddEffectiveToDataLong();
std::intptr_t Emu68_SubtractEffectiveFromDataByte();
std::intptr_t Emu68_SubtractEffectiveFromDataWord();
std::intptr_t Emu68_SubtractEffectiveFromDataLong();
std::intptr_t Emu68_AndEffectiveByte(std::int8_t a1);
std::intptr_t Emu68_AndEffectiveWord(std::int16_t a1);
std::intptr_t Emu68_AndEffectiveLong(int a1);
std::intptr_t Emu68_OrEffectiveByte(std::int8_t a1);
std::intptr_t Emu68_OrEffectiveWord(std::int16_t a1);
std::intptr_t Emu68_OrEffectiveLong(int a1);
std::intptr_t Emu68_XorEffectiveByte(std::int8_t a1);
std::intptr_t Emu68_XorEffectiveWord(std::int16_t a1);
std::intptr_t Emu68_XorEffectiveLong(int a1);
std::intptr_t Emu68_NegateEffectiveByte();
std::intptr_t Emu68_NegateEffectiveWord();
std::intptr_t Emu68_NegateEffectiveLong();
std::intptr_t Emu68_NegateExtendEffectiveByte();
std::intptr_t Emu68_NegateExtendEffectiveWord();
std::intptr_t Emu68_NegateExtendEffectiveLong();
std::intptr_t Emu68_NotEffectiveByte();
std::intptr_t Emu68_NotEffectiveWord();
std::intptr_t Emu68_NotEffectiveLong();
std::intptr_t Emu68_AddBcd();
std::intptr_t Emu68_SubtractBcd();
std::intptr_t Emu68_NegateBcd();
std::intptr_t Emu68_AddExtendByte();
std::intptr_t Emu68_AddExtendWord();
std::intptr_t Emu68_AddExtendLong();
std::intptr_t Emu68_SubtractExtendByte();
std::intptr_t Emu68_SubtractExtendWord();
std::intptr_t Emu68_SubtractExtendLong();
std::intptr_t Emu68_CompareMemoryByte();
std::intptr_t Emu68_CompareMemoryWord();
std::intptr_t Emu68_CompareMemoryLong();
std::intptr_t Emu68_ArithmeticShiftLeft(int a1, int a2, int a3);
std::intptr_t Emu68_LogicalShiftLeft(int a1, int a2, int a3);
std::intptr_t Emu68_ArithmeticShiftRight(int a1, int a2, int a3);
std::intptr_t Emu68_LogicalShiftRight(int a1, int a2, int a3);
std::intptr_t Emu68_RotateRight(int a1, int a2, int a3);
std::intptr_t Emu68_RotateLeft(int a1, int a2, int a3);
std::intptr_t Emu68_RotateExtendRight(int a1, int a2, int a3);
std::intptr_t Emu68_RotateExtendLeft(int a1, int a2, int a3);
std::intptr_t Emu68_MoveMultiple(int a1);
std::intptr_t Emu68_BitTest(std::uint8_t a1);
std::intptr_t Emu68_BitChange(std::int8_t a1);
std::intptr_t Emu68_BitClear(std::int8_t a1);
std::intptr_t Emu68_BitSet(std::int8_t a1);
std::intptr_t Emu68_MovePeripheral();
std::intptr_t Emu68_DivideUnsigned(int a1);
std::intptr_t Emu68_DivideSigned(int a1);
std::intptr_t emu68_HandleInterruptRequest(unsigned int a1);
std::intptr_t Emu68_ResetContext();
std::intptr_t Emu68_ReturnException();
std::intptr_t Emu68_OrImmediateByte();
std::intptr_t Emu68_OrImmediateWord();
std::intptr_t Emu68_OrImmediateLong();
std::intptr_t Emu68_AndImmediateByte();
std::intptr_t Emu68_AndImmediateWord();
std::intptr_t Emu68_AndImmediateLong();
std::intptr_t Emu68_XorImmediateByte();
std::intptr_t Emu68_XorImmediateWord();
std::intptr_t Emu68_XorImmediateLong();
std::intptr_t Emu68_SubtractImmediateByte();
std::intptr_t Emu68_SubtractImmediateWord();
std::intptr_t Emu68_SubtractImmediateLong();
std::intptr_t Emu68_AddImmediateByte();
std::intptr_t Emu68_AddImmediateWord();
std::intptr_t Emu68_AddImmediateLong();
std::intptr_t Emu68_CompareImmediateByte();
std::intptr_t Emu68_CompareImmediateWord();
std::intptr_t Emu68_CompareImmediateLong();
std::intptr_t Emu68_BitTestImmediate();
std::intptr_t Emu68_BitChangeImmediate();
std::intptr_t Emu68_BitClearImmediate();
std::intptr_t Emu68_BitSetImmediate();
std::intptr_t Emu68_BitTestDataOrMovePeripheral();
std::intptr_t Emu68_BitChangeDataOrMovePeripheral();
std::intptr_t Emu68_BitClearDataOrMovePeripheral();
std::intptr_t Emu68_BitSetDataOrMovePeripheral();
std::intptr_t Emu68_ReportInstructionError();
void Emu68_DispatchGroup0();
void Emu68_MoveByte();
void Emu68_MoveLong();
void Emu68_MoveWord();
std::intptr_t Emu68_NegateExtendByteThunk();
std::intptr_t Emu68_NegateExtendWordThunk();
std::intptr_t Emu68_NegateExtendLongThunk();
std::intptr_t Emu68_MoveFromStatus();
std::intptr_t Emu68_MoveFromConditionCodes();
std::intptr_t Emu68_MoveToStatus();
std::intptr_t Emu68_MoveToConditionCodes();
std::intptr_t Emu68_CheckLongBounds();
std::intptr_t Emu68_CheckWordBounds();
std::intptr_t Emu68_LoadEffectiveAddressA0();
std::intptr_t Emu68_LoadEffectiveAddressA1();
std::intptr_t Emu68_LoadEffectiveAddressA2();
std::intptr_t Emu68_LoadEffectiveAddressA3();
std::intptr_t Emu68_LoadEffectiveAddressA4();
std::intptr_t Emu68_LoadEffectiveAddressA5();
std::intptr_t Emu68_LoadEffectiveAddressA6();
std::intptr_t Emu68_LoadEffectiveAddressA7();
std::intptr_t Emu68_ClearByte();
std::intptr_t Emu68_ClearWord();
std::intptr_t Emu68_ClearLong();
std::intptr_t Emu68_NegateByteThunk();
std::intptr_t Emu68_NegateWordThunk();
std::intptr_t Emu68_NegateLongThunk();
std::intptr_t Emu68_NotByteThunk();
std::intptr_t Emu68_NotWordThunk();
std::intptr_t Emu68_NotLongThunk();
std::intptr_t Emu68_TestByte();
std::intptr_t Emu68_TestWord();
std::intptr_t Emu68_TestLong();
std::intptr_t Emu68_NegateBcdThunk();
std::intptr_t Emu68_SwapOrPushEffectiveAddress();
std::intptr_t Emu68_ExtendByteOrMoveMultipleWord();
std::intptr_t Emu68_ExtendWordOrMoveMultipleLong();
std::intptr_t Emu68_MoveMultipleWordToRegisters();
std::intptr_t Emu68_MoveMultipleLongToRegisters();
std::intptr_t Emu68_TestAndSet();
std::intptr_t Emu68_SystemInstruction();
std::intptr_t Emu68_JumpSubroutine();
std::intptr_t Emu68_Jump();
void Emu68_DispatchGroup4();
std::intptr_t Emu68_AddQuickByte();
std::intptr_t Emu68_AddQuickWord();
std::intptr_t Emu68_AddQuickLong();
std::intptr_t Emu68_SubtractQuickByte();
std::intptr_t Emu68_SubtractQuickWord();
std::intptr_t Emu68_SubtractQuickLong();
std::intptr_t Emu68_SetOrDecrementCondition();
void Emu68_DispatchGroup5();
void Emu68_Branch();
void Emu68_MoveQuick();
void Emu68_DispatchGroup8();
void Emu68_DispatchGroupC();
void Emu68_DispatchGroupB();
void Emu68_DispatchGroup9();
void Emu68_DispatchGroupD();
void Emu68_ShiftRotate();

inline emu68_ctx_t emu68_insn_defs[91] = {
    {.a = 1, .pattern = 1, .bitmask = 1, .b = 0, .bits = "0000000000111100", .asm_string = "ori.%s\t%i,ccr"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0000000001111100", .asm_string = "ori.%s\t%i,sr"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "00000000sseeeeee", .asm_string = "ori.%s\t%i,%e"},
    {.a = 0, .pattern = 0, .bitmask = 0, .b = 0, .bits = "00001000tteeeeee", .asm_string = "b%t\t%i,%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0000001000111100", .asm_string = "andi.%s\t%i,ccr"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0000001001111100", .asm_string = "andi.%s\t%i,sr"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "00000010sseeeeee", .asm_string = "andi.%s\t%i,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "00000100sseeeeee", .asm_string = "subi.%s\t%i,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "00000110sseeeeee", .asm_string = "addi.%s\t%i,%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0000101000111100", .asm_string = "eori.%s\t%i,ccr"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0000101001111100", .asm_string = "eori.%s\t%i,sr"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "00001010sseeeeee", .asm_string = "eori.%s\t%i,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "00001100sseeeeee", .asm_string = "cmpi.%s\t%i,%e"},
    {.a = 0, .pattern = 0, .bitmask = 0, .b = 0, .bits = "0000bbb1tteeeeee", .asm_string = "b%t\td%b,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0000bbb10S001aaa", .asm_string = "movep.%s\t(a%a),d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0000bbb11S001aaa", .asm_string = "movep.%s\td%b,(a%a)"},
    {.a = 0, .pattern = 0, .bitmask = 0, .b = 0, .bits = "0001ffffffeeeeee", .asm_string = "move.%s\t%e,%f"},
    {.a = 0, .pattern = 0, .bitmask = 2, .b = 0, .bits = "0010ffffffeeeeee", .asm_string = "move.%s\t%e,%f"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0011ffffffeeeeee", .asm_string = "move.%s\t%e,%f"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0100000011eeeeee", .asm_string = "move.%s\tsr,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "01000000sseeeeee", .asm_string = "negx.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0100001011eeeeee", .asm_string = "move.%s\tccr,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "01000010sseeeeee", .asm_string = "clr.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0100010011eeeeee", .asm_string = "move.%s\t%e,ccr"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "01000100sseeeeee", .asm_string = "neg.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0100011011eeeeee", .asm_string = "move.%s\t%e,sr"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "01000110sseeeeee", .asm_string = "not.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 0, .b = 0, .bits = "0100100000eeeeee", .asm_string = "nbcd\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100100001000aaa", .asm_string = "swap\td%a"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100100001eeeeee", .asm_string = "pea\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "010010001S000aaa", .asm_string = "ext.%s\td%a"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "010010001S100aaa", .asm_string = "movem.%s\t%M,-(a%a)"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "010010001Seeeeee", .asm_string = "movem.%s\t%m,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "010011001Seeeeee", .asm_string = "movem.%s\t%e,%m"},
    {.a = 0, .pattern = 0, .bitmask = 0, .b = 0, .bits = "0100101011eeeeee", .asm_string = "tas.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "01001010sseeeeee", .asm_string = "tst.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100101011111100", .asm_string = "illegal"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "010011100100aaaa", .asm_string = "trap\t#%a"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0100111001010aaa", .asm_string = "link\t%i,a%a"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0100111001011aaa", .asm_string = "unlk\ta%a"},
    {.a = 0, .pattern = 0, .bitmask = 2, .b = 0, .bits = "0100111001100aaa", .asm_string = "move.%s\ta%a,usp"},
    {.a = 0, .pattern = 0, .bitmask = 2, .b = 0, .bits = "0100111001101aaa", .asm_string = "move.%s\tusp,a%a"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111001110000", .asm_string = "reset"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111001110001", .asm_string = "nop"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111001110010", .asm_string = "stop"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111001110011", .asm_string = "rte"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111001110101", .asm_string = "rts"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111001110110", .asm_string = "trapv"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111001110111", .asm_string = "rtr"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111010eeeeee", .asm_string = "jsr\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100111011eeeeee", .asm_string = "jmp\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "0100bbb110eeeeee", .asm_string = "chk.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0100bbb111eeeeee", .asm_string = "lea\t%e,a%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0101cccc11001aaa", .asm_string = "db%c\td%a,%J"},
    {.a = 0, .pattern = 0, .bitmask = 0, .b = 0, .bits = "0101cccc11eeeeee", .asm_string = "s%c.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0101qqq0sseeeeee", .asm_string = "addq.%s\t#%q,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0101qqq1sseeeeee", .asm_string = "subq.%s\t#%q,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "01100000jjjjjjjj", .asm_string = "bra.%S\t%j"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "01100001jjjjjjjj", .asm_string = "bsr.%S\t%j"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "0110ccccjjjjjjjj", .asm_string = "b%c.%S\t%j"},
    {.a = 0, .pattern = 0, .bitmask = 2, .b = 0, .bits = "0111bbb0aaaaaaaa", .asm_string = "moveq.%s\t#%a,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1000bbb10000raaa", .asm_string = "sbcd\t%r%a%R,%r%b%R"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "1000bbb011eeeeee", .asm_string = "divu.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "1000bbb111eeeeee", .asm_string = "divs.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1000bbb0sseeeeee", .asm_string = "or.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1000bbb1sseeeeee", .asm_string = "or.%s\td%b,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1001bbbS11eeeeee", .asm_string = "suba.%s\t%e,a%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1001bbb1ss00raaa", .asm_string = "subx.%s\t%r%a%R,%r%b%R"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1001bbb0sseeeeee", .asm_string = "sub.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1001bbb1sseeeeee", .asm_string = "sub.%s\td%b,%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "1011bbb011eeeeee", .asm_string = "cmpa.%s\t%e,a%b"},
    {.a = 0, .pattern = 0, .bitmask = 2, .b = 0, .bits = "1011bbb111eeeeee", .asm_string = "cmpa.%s\t%e,a%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1011bbb1ss001aaa", .asm_string = "cmpm.%s\t(a%a)+,(a%b)+"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1011bbb0sseeeeee", .asm_string = "cmp.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1011bbb1sseeeeee", .asm_string = "eor.%s\td%b,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1100bbb101000aaa", .asm_string = "exg\td%a,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1100bbb101001aaa", .asm_string = "exg\ta%a,a%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1100bbb110001aaa", .asm_string = "exg\ta%a,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1100bbb10000raaa", .asm_string = "abcd\t%r%a%R,%r%b%R"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "1100bbb011eeeeee", .asm_string = "mulu.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "1100bbb111eeeeee", .asm_string = "muls.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1100bbb0sseeeeee", .asm_string = "and.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1100bbb1sseeeeee", .asm_string = "and.%s\td%b,%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1101bbbS11eeeeee", .asm_string = "adda.%s\t%e,a%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1101bbb1ss00raaa", .asm_string = "addx.%s\t%r%a%R,%r%b%R"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1101bbb0sseeeeee", .asm_string = "add.%s\t%e,d%b"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1101bbb1sseeeeee", .asm_string = "add.%s\td%b,%e"},
    {.a = 0, .pattern = 0, .bitmask = 1, .b = 0, .bits = "11100TTD11eeeeee", .asm_string = "%T%D.%s\t%e"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1110qqqDss0TTaaa", .asm_string = "%T%D.%s\t#%q,d%a"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = "1110bbbDss1TTaaa", .asm_string = "%T%D.%s\td%b,d%a"},
    {.a = 0, .pattern = 0, .bitmask = 3, .b = 0, .bits = nullptr, .asm_string = nullptr},
};

inline constexpr std::int32_t kHarrierRomPatches[] = {
    4096, 40960, -1, 4304, 40961, -1, 4374, 40962, -1, 5286, 40963, -1,
    5290, 40964, -1, 8288, 40965, -1, 26278, 40966, -1, 26388, 40967, -1,
    26434, 40968, -1, 11964, 40969, -1, 9884, 20085, -1, 24280, 41216, -1,
    24258, 41217, -1, 4314, 41218, -1, 5240, 41219, -1, 24664, 41220, -1,
    8066, 41221, -1, 36750, 41222, -1, 19118, 41223, -1, 8176, 19, -1,
    8206, 4, 13512, -1, 8212, 4, 13502, -1, 8218, 18, -1, 8222,
    19, -1, 8300, 20, -1, 8324, 20, -1, 8332, 17, -1, 8358,
    31751, -1, 8368, 31744, -1, 9042, 31764, -1, 9100, 13, -1, 8426,
    41224, -1, 8432, 41225, -1, 8488, 41226, -1, 19120, 32256, 24832, 7698,
    12348, 179, 24832, 51396, 20936, 65530, 24832, 56944, 24576, 51448, -1, 24256,
    36024, -1, 24328, 36034, -1, 5628, 36066, -1, 13186, 36074, -1, 13222,
    36084, -1, 26696, 36022, -1, 25452, 33130, -1, 25516, 32988, -1, -1,
};

inline constexpr Emu68ReadOperand kEmu68_CompareMemoryWordSource[2] = {
    &Emu68_ReadDataWord,
    &Emu68_ReadPredecrementWord,
};

inline constexpr Emu68WriteOperand kEmu68_WriteLong[8] = {
    &Emu68_WriteDataLong,
    &Emu68_WriteAddressLong,
    &Emu68_WriteEffectiveLong,
    &Emu68_WritePostincrementLong,
    &Emu68_WriteEffectiveLong,
    &Emu68_WriteEffectiveLong,
    &Emu68_WriteEffectiveLong,
    &Emu68_WriteExtendedLong,
};

inline constexpr Emu68Operation kEmu68_Group0[64] = {
    &Emu68_OrImmediateByte,
    &Emu68_OrImmediateWord,
    &Emu68_OrImmediateLong,
    &Emu68_ReportInstructionError,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
    &Emu68_AndImmediateByte,
    &Emu68_AndImmediateWord,
    &Emu68_AndImmediateLong,
    &Emu68_ReportInstructionError,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
    &Emu68_SubtractImmediateByte,
    &Emu68_SubtractImmediateWord,
    &Emu68_SubtractImmediateLong,
    &Emu68_ReportInstructionError,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
    &Emu68_AddImmediateByte,
    &Emu68_AddImmediateWord,
    &Emu68_AddImmediateLong,
    &Emu68_ReportInstructionError,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
    &Emu68_BitTestImmediate,
    &Emu68_BitChangeImmediate,
    &Emu68_BitClearImmediate,
    &Emu68_BitSetImmediate,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
    &Emu68_XorImmediateByte,
    &Emu68_XorImmediateWord,
    &Emu68_XorImmediateLong,
    &Emu68_ReportInstructionError,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
    &Emu68_CompareImmediateByte,
    &Emu68_CompareImmediateWord,
    &Emu68_CompareImmediateLong,
    &Emu68_ReportInstructionError,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
    &Emu68_ReportInstructionError,
    &Emu68_ReportInstructionError,
    &Emu68_ReportInstructionError,
    &Emu68_ReportInstructionError,
    &Emu68_BitTestDataOrMovePeripheral,
    &Emu68_BitChangeDataOrMovePeripheral,
    &Emu68_BitClearDataOrMovePeripheral,
    &Emu68_BitSetDataOrMovePeripheral,
};

inline constexpr Emu68ReadOperand kEmu68_ReadModifyByte[8] = {
    &Emu68_ReadDataByte,
    &Emu68_ReportByteReadModeError,
    &Emu68_ReadModifyIndirectByte,
    &Emu68_ReadModifyIndirectByte,
    &Emu68_ReadModifyPredecrementByte,
    &Emu68_ReadModifyDisplacementByte,
    &Emu68_ReadModifyIndexedByte,
    &Emu68_ReadModifyExtendedByte,
};

inline constexpr Emu68WriteOperand kEmu68_WriteByte[8] = {
    &Emu68_WriteDataByte,
    &Emu68_ReportWriteModeError,
    &Emu68_WriteEffectiveByte,
    &Emu68_WritePostincrementByte,
    &Emu68_WriteEffectiveByte,
    &Emu68_WriteEffectiveByte,
    &Emu68_WriteEffectiveByte,
    &Emu68_WriteExtendedByte,
};

inline constexpr Emu68ReadOperand kEmu68_ReadModifyWord[8] = {
    &Emu68_ReadDataWord,
    &Emu68_ReadAddressWord,
    &Emu68_ReadModifyIndirectWord,
    &Emu68_ReadModifyIndirectWord,
    &Emu68_ReadModifyPredecrementWord,
    &Emu68_ReadModifyDisplacementWord,
    &Emu68_ReadModifyIndexedWord,
    &Emu68_ReadModifyExtendedWord,
};

inline constexpr Emu68WriteOperand kEmu68_WriteWord[8] = {
    &Emu68_WriteDataWord,
    &Emu68_WriteAddressWord,
    &Emu68_WriteEffectiveWord,
    &Emu68_WritePostincrementWord,
    &Emu68_WriteEffectiveWord,
    &Emu68_WriteEffectiveWord,
    &Emu68_WriteEffectiveWord,
    &Emu68_WriteExtendedWord,
};

inline constexpr Emu68ReadOperand kEmu68_ReadModifyLong[8] = {
    &Emu68_ReadDataLong,
    &Emu68_ReadAddressRegister,
    &Emu68_ReadModifyIndirectLong,
    &Emu68_ReadModifyIndirectLong,
    &Emu68_ReadModifyPredecrementLong,
    &Emu68_ReadModifyDisplacementLong,
    &Emu68_ReadModifyIndexedLong,
    &Emu68_ReadModifyExtendedLong,
};

inline constexpr Emu68ReadOperand kEmu68_ReadByte[8] = {
    &Emu68_ReadDataByte,
    &Emu68_ReportByteReadModeError,
    &Emu68_ReadIndirectByte,
    &Emu68_ReadPostincrementByte,
    &Emu68_ReadPredecrementByte,
    &Emu68_ReadDisplacementByte,
    &Emu68_ReadIndexedByte,
    &Emu68_ReadExtendedByte,
};

inline constexpr Emu68ReadOperand kEmu68_ReadWord[8] = {
    &Emu68_ReadDataWord,
    &Emu68_ReadAddressWord,
    &Emu68_ReadIndirectWord,
    &Emu68_ReadPostincrementWord,
    &Emu68_ReadPredecrementWord,
    &Emu68_ReadDisplacementWord,
    &Emu68_ReadIndexedWord,
    &Emu68_ReadExtendedWord,
};

inline constexpr Emu68ReadOperand kEmu68_ReadLong[8] = {
    &Emu68_ReadDataLong,
    &Emu68_ReadAddressRegister,
    &Emu68_ReadIndirectLong,
    &Emu68_ReadPostincrementLong,
    &Emu68_ReadPredecrementLong,
    &Emu68_ReadDisplacementLong,
    &Emu68_ReadIndexedLong,
    &Emu68_ReadExtendedLong,
};

inline constexpr Emu68ReadOperand kEmu68_CompareMemoryByteSource[2] = {
    &Emu68_ReadDataByte,
    &Emu68_ReadPredecrementByte,
};

inline constexpr Emu68ReadOperand kEmu68_PredecrementByteDestination[2] = {
    &Emu68_ReadDataByte,
    &Emu68_ReadModifyPredecrementByte,
};

inline constexpr Emu68WriteOperand kEmu68_WriteBytePair[2] = {
    &Emu68_WriteDataByte,
    &Emu68_WriteEffectiveByte,
};

inline constexpr Emu68ReadOperand kEmu68_PredecrementWordDestination[2] = {
    &Emu68_ReadDataWord,
    &Emu68_ReadModifyPredecrementWord,
};

inline constexpr Emu68WriteOperand kEmu68_WriteWordPair[2] = {
    &Emu68_WriteDataWord,
    &Emu68_WriteEffectiveWord,
};

inline constexpr Emu68ReadOperand kEmu68_CompareMemoryLongSource[2] = {
    &Emu68_ReadDataLong,
    &Emu68_ReadPredecrementLong,
};

inline constexpr Emu68ReadOperand kEmu68_PredecrementLongDestination[2] = {
    &Emu68_ReadDataLong,
    &Emu68_ReadModifyPredecrementLong,
};

inline constexpr Emu68WriteOperand kEmu68_WriteLongPair[2] = {
    &Emu68_WriteDataLong,
    &Emu68_WriteEffectiveLong,
};

inline constexpr Emu68ReadOperand kEmu68_Address[8] = {
    &Emu68_ReportAddressModeError,
    &Emu68_ReportAddressModeError,
    &Emu68_ReadAddressRegister,
    &Emu68_ReportAddressModeError,
    &Emu68_ReportAddressModeError,
    &Emu68_AddressDisplacement,
    &Emu68_AddressIndexed,
    &Emu68_AddressExtended,
};

inline constexpr Emu68ReadOperand kEmu68_SetEffectiveByte[8] = {
    &Emu68_NullOperand,
    &Emu68_ReportWriteModeError,
    &Emu68_SetEffectiveIndirect,
    &Emu68_SetEffectiveIndirect,
    &Emu68_SetEffectivePredecrementByte,
    &Emu68_SetEffectiveDisplacement,
    &Emu68_SetEffectiveIndexed,
    &Emu68_SetEffectiveExtended,
};

inline constexpr Emu68ReadOperand kEmu68_SetEffectiveLong[8] = {
    &Emu68_NullOperand,
    &Emu68_NullOperand,
    &Emu68_SetEffectiveIndirect,
    &Emu68_SetEffectiveIndirect,
    &Emu68_SetEffectivePredecrementLong,
    &Emu68_SetEffectiveDisplacement,
    &Emu68_SetEffectiveIndexed,
    &Emu68_SetEffectiveExtended,
};

inline constexpr Emu68ReadOperand kEmu68_SetEffectiveWord[8] = {
    &Emu68_NullOperand,
    &Emu68_NullOperand,
    &Emu68_SetEffectiveIndirect,
    &Emu68_SetEffectiveIndirect,
    &Emu68_SetEffectivePredecrementWord,
    &Emu68_SetEffectiveDisplacement,
    &Emu68_SetEffectiveIndexed,
    &Emu68_SetEffectiveExtended,
};

inline constexpr Emu68Operation kEmu68_Group5[8] = {
    &Emu68_AddQuickByte,
    &Emu68_AddQuickWord,
    &Emu68_AddQuickLong,
    &Emu68_SetOrDecrementCondition,
    &Emu68_SubtractQuickByte,
    &Emu68_SubtractQuickWord,
    &Emu68_SubtractQuickLong,
    &Emu68_SetOrDecrementCondition,
};

inline constexpr Emu68Operation kEmu68_Group4[64] = {
    &Emu68_NegateExtendByteThunk,
    &Emu68_NegateExtendWordThunk,
    &Emu68_NegateExtendLongThunk,
    &Emu68_MoveFromStatus,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA0,
    &Emu68_ClearByte,
    &Emu68_ClearWord,
    &Emu68_ClearLong,
    &Emu68_MoveFromConditionCodes,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA1,
    &Emu68_NegateByteThunk,
    &Emu68_NegateWordThunk,
    &Emu68_NegateLongThunk,
    &Emu68_MoveToConditionCodes,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA2,
    &Emu68_NotByteThunk,
    &Emu68_NotWordThunk,
    &Emu68_NotLongThunk,
    &Emu68_MoveToStatus,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA3,
    &Emu68_NegateBcdThunk,
    &Emu68_SwapOrPushEffectiveAddress,
    &Emu68_ExtendByteOrMoveMultipleWord,
    &Emu68_ExtendWordOrMoveMultipleLong,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA4,
    &Emu68_TestByte,
    &Emu68_TestWord,
    &Emu68_TestLong,
    &Emu68_TestAndSet,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA5,
    &Emu68_ReportInstructionError,
    &Emu68_ReportInstructionError,
    &Emu68_MoveMultipleWordToRegisters,
    &Emu68_MoveMultipleLongToRegisters,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA6,
    &Emu68_ReportInstructionError,
    &Emu68_SystemInstruction,
    &Emu68_JumpSubroutine,
    &Emu68_Jump,
    &Emu68_CheckLongBounds,
    &Emu68_ReportInstructionError,
    &Emu68_CheckWordBounds,
    &Emu68_LoadEffectiveAddressA7,
};

inline Emu68OpcodeHandler g_emu68OpcodeDispatch[16] = {
    &Emu68_DispatchGroup0,
    &Emu68_MoveByte,
    &Emu68_MoveLong,
    &Emu68_MoveWord,
    &Emu68_DispatchGroup4,
    &Emu68_DispatchGroup5,
    &Emu68_Branch,
    &Emu68_MoveQuick,
    &Emu68_DispatchGroup8,
    &Emu68_DispatchGroup9,
    nullptr,
    &Emu68_DispatchGroupB,
    &Emu68_DispatchGroupC,
    &Emu68_DispatchGroupD,
    &Emu68_ShiftRotate,
    &Emu68_ReportLineFError,
};

inline std::array<reg68_page_map_entry_t, 256> g_reg68_ctx_map_funcs = [] {
    std::array<reg68_page_map_entry_t, 256> pages{};
    for (reg68_page_map_entry_t &page : pages) {
        page = {.read8 = &Emu68_UnmappedRead8,
            .write8 = &Emu68_IgnoreWrite8,
            .read16 = &Emu68_UnmappedRead16,
            .write16 = &Emu68_IgnoreWrite16,
            .read32 = &Emu68_UnmappedRead32,
            .write32 = &Emu68_IgnoreWrite32,
            .base = 0,
            .mask = 0xFFFFFFFF};
    }
    pages[0x00] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Emu68_IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Emu68_IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Emu68_IgnoreWrite32,
        .base = 0x0,
        .mask = 0xFFFF};
    pages[0x01] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Emu68_IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Emu68_IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Emu68_IgnoreWrite32,
        .base = 0x10000,
        .mask = 0xFFFF};
    pages[0x02] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Emu68_IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Emu68_IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Emu68_IgnoreWrite32,
        .base = 0x20000,
        .mask = 0xFFFF};
    pages[0x03] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Emu68_IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Emu68_IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Emu68_IgnoreWrite32,
        .base = 0x30000,
        .mask = 0xFFFF};
    pages[0x04] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0x80000,
        .mask = 0x3FFF};
    pages[0x10] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0x90000,
        .mask = 0xFFFF};
    pages[0x11] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0xB0000,
        .mask = 0xFFF};
    pages[0x12] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0x88000,
        .mask = 0x3FFF};
    pages[0x13] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0xC0000,
        .mask = 0x3FF};
    pages[0x14] = {.read8 = &Harrier_HandleInput,
        .write8 = &Harrier_WriteInputSelector,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0xD0000,
        .mask = 0x3F};
    pages[0xC6] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0xE0000,
        .mask = 0x7FF};
    return pages;
}();

inline std::array<reg68_page_map_entry_t, 256> g_reg68_ctx_map_funcs_2 = [] {
    std::array<reg68_page_map_entry_t, 256> pages{};
    for (reg68_page_map_entry_t &page : pages) {
        page = {.read8 = &Emu68_UnmappedRead8,
            .write8 = &Emu68_IgnoreWrite8,
            .read16 = &Emu68_UnmappedRead16,
            .write16 = &Emu68_IgnoreWrite16,
            .read32 = &Emu68_UnmappedRead32,
            .write32 = &Emu68_IgnoreWrite32,
            .base = 0,
            .mask = 0xFFFFFFFF};
    }
    pages[0x00] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Emu68_IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Emu68_IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Emu68_IgnoreWrite32,
        .base = 0x40000,
        .mask = 0xFFFF};
    pages[0xC6] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0xE0000,
        .mask = 0x7FF};
    pages[0xC7] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &Harrier_WriteMemory8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &Harrier_WriteMemory16BigEndian,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &Harrier_WriteMemory32BigEndian,
        .base = 0x88000,
        .mask = 0x3FFF};
    return pages;
}();

} // namespace shenmue::s1
