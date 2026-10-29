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

namespace emu68 {
void Run();
}

std::uint8_t Harrier_ReadMemory8(std::uint32_t address);
std::uint16_t Harrier_ReadMemory16BigEndian(std::uint32_t address);
std::uint32_t Harrier_ReadMemory32BigEndian(std::uint32_t address);
void Harrier_WriteMemory8(std::uint32_t address, std::uint8_t value);
void Harrier_WriteMemory16BigEndian(std::uint32_t address, std::uint16_t value);
void Harrier_WriteMemory32BigEndian(std::uint32_t address, std::uint32_t value);

namespace emu68 {

std::uint8_t UnmappedRead8(std::uint32_t address);
std::uint16_t UnmappedRead16(std::uint32_t address);
std::uint32_t UnmappedRead32(std::uint32_t address);
void IgnoreWrite8(std::uint32_t address, std::uint8_t value);
void IgnoreWrite16(std::uint32_t address, std::uint16_t value);
void IgnoreWrite32(std::uint32_t address, std::uint32_t value);
std::intptr_t NullOperand(int index);
std::intptr_t ReportWriteModeError(int index, std::uint32_t value);

using ReadOperand = std::intptr_t (*)(int);
using WriteOperand = std::intptr_t (*)(int, std::uint32_t);
using Operation = std::intptr_t (*)();
using OpcodeHandler = void (*)();
using ShiftHandler = std::intptr_t (*)(int, int, int);

void ReportLineFError();
std::intptr_t ReportRuntimeError(const char *Args, int a2);
std::intptr_t ResetOtherCpu();
std::intptr_t FetchWord();
std::intptr_t DecodeBriefIndex(std::uint16_t a1);
std::intptr_t ReportAddressModeError(int a1);
std::intptr_t ReadAddressRegister(int a1);
std::intptr_t AddressDisplacement(int a1);
std::intptr_t AddressIndexed(int a1);
std::intptr_t AddressExtended(int a1);
std::intptr_t ReportByteReadModeError(int a1);
std::intptr_t ReadDataByte(int a1);
std::intptr_t ReadIndirectByte(int a1);
std::intptr_t ReadPostincrementByte(int a1);
std::intptr_t ReadPredecrementByte(int a1);
std::intptr_t ReadDisplacementByte(int a1);
std::intptr_t ReadIndexedByte(int a1);
std::intptr_t ReadExtendedByte(int a1);
std::intptr_t ReadDataWord(int a1);
std::intptr_t ReadAddressWord(int a1);
std::intptr_t ReadIndirectWord(int a1);
std::intptr_t ReadPostincrementWord(int a1);
std::intptr_t ReadPredecrementWord(int a1);
std::intptr_t ReadDisplacementWord(int a1);
std::intptr_t ReadIndexedWord(int a1);
std::intptr_t ReadExtendedWord(int a1);
std::intptr_t ReadDataLong(int a1);
std::intptr_t ReadIndirectLong(int a1);
std::intptr_t ReadPostincrementLong(int a1);
std::intptr_t ReadPredecrementLong(int a1);
std::intptr_t ReadDisplacementLong(int a1);
std::intptr_t ReadIndexedLong(int a1);
std::intptr_t ReadExtendedLong(int a1);
std::intptr_t ReadModifyIndirectByte(int a1);
std::intptr_t ReadModifyPredecrementByte(int a1);
std::intptr_t ReadModifyDisplacementByte(int a1);
std::intptr_t ReadModifyIndexedByte(int a1);
std::intptr_t ReadModifyExtendedByte(int a1);
std::intptr_t ReadModifyIndirectWord(int a1);
std::intptr_t ReadModifyPredecrementWord(int a1);
std::intptr_t ReadModifyDisplacementWord(int a1);
std::intptr_t ReadModifyIndexedWord(int a1);
std::intptr_t ReadModifyExtendedWord(int a1);
std::intptr_t ReadModifyIndirectLong(int a1);
std::intptr_t ReadModifyPredecrementLong(int a1);
std::intptr_t ReadModifyDisplacementLong(int a1);
std::intptr_t ReadModifyIndexedLong(int a1);
std::intptr_t ReadModifyExtendedLong(int a1);
std::intptr_t WriteDataByte(int a1, std::uint32_t a2);
std::intptr_t ReportWriteModeError(int a1);
std::intptr_t WriteEffectiveByte(int a1, std::uint32_t a2);
std::intptr_t WritePostincrementByte(int a1, std::uint32_t a2);
std::intptr_t WriteExtendedByte(int a1, std::uint32_t a2);
std::intptr_t WriteDataWord(int a1, std::uint32_t a2);
std::intptr_t WriteAddressWord(int a1, std::uint32_t a2);
std::intptr_t WriteEffectiveWord(int a1, std::uint32_t a2);
std::intptr_t WritePostincrementWord(int a1, std::uint32_t a2);
std::intptr_t WriteExtendedWord(int a1, std::uint32_t a2);
std::intptr_t WriteDataLong(int a1, std::uint32_t a2);
std::intptr_t WriteAddressLong(int a1, std::uint32_t a2);
std::intptr_t WriteEffectiveLong(int a1, std::uint32_t a2);
std::intptr_t WritePostincrementLong(int a1, std::uint32_t a2);
std::intptr_t WriteExtendedLong(int a1, std::uint32_t a2);
std::intptr_t SetEffectiveIndirect(int a1);
std::intptr_t SetEffectivePredecrementByte(int a1);
std::intptr_t SetEffectiveDisplacement(int a1);
std::intptr_t SetEffectiveIndexed(int a1);
std::intptr_t SetEffectiveExtended(int a1);
std::intptr_t SetEffectivePredecrementWord(int a1);
std::intptr_t SetEffectivePredecrementLong(int a1);
std::intptr_t CompareFlags(int a1, int a2, int a3);
std::intptr_t PushLong(unsigned int a1);
std::intptr_t PopWord();
std::intptr_t PopLong();
std::intptr_t SetStatusRegister(reg68_sr_bits_t a1);
std::intptr_t AddToEffectiveByte(std::int8_t a1);
std::intptr_t AddToEffectiveWord(std::int16_t a1);
std::intptr_t AddToEffectiveLong(int a1);
std::intptr_t SubtractFromEffectiveByte(std::int8_t a1);
std::intptr_t SubtractFromEffectiveWord(std::int16_t a1);
std::intptr_t SubtractFromEffectiveLong(int a1);
std::intptr_t AddDataToEffectiveByte();
std::intptr_t AddDataToEffectiveWord();
std::intptr_t AddDataToEffectiveLong();
std::intptr_t SubtractDataFromEffectiveByte();
std::intptr_t SubtractDataFromEffectiveWord();
std::intptr_t SubtractDataFromEffectiveLong();
std::intptr_t AddEffectiveToDataByte();
std::intptr_t AddEffectiveToDataWord();
std::intptr_t AddEffectiveToDataLong();
std::intptr_t SubtractEffectiveFromDataByte();
std::intptr_t SubtractEffectiveFromDataWord();
std::intptr_t SubtractEffectiveFromDataLong();
std::intptr_t AndEffectiveByte(std::int8_t a1);
std::intptr_t AndEffectiveWord(std::int16_t a1);
std::intptr_t AndEffectiveLong(int a1);
std::intptr_t OrEffectiveByte(std::int8_t a1);
std::intptr_t OrEffectiveWord(std::int16_t a1);
std::intptr_t OrEffectiveLong(int a1);
std::intptr_t XorEffectiveByte(std::int8_t a1);
std::intptr_t XorEffectiveWord(std::int16_t a1);
std::intptr_t XorEffectiveLong(int a1);
std::intptr_t NegateEffectiveByte();
std::intptr_t NegateEffectiveWord();
std::intptr_t NegateEffectiveLong();
std::intptr_t NegateExtendEffectiveByte();
std::intptr_t NegateExtendEffectiveWord();
std::intptr_t NegateExtendEffectiveLong();
std::intptr_t NotEffectiveByte();
std::intptr_t NotEffectiveWord();
std::intptr_t NotEffectiveLong();
std::intptr_t AddBcd();
std::intptr_t SubtractBcd();
std::intptr_t NegateBcd();
std::intptr_t AddExtendByte();
std::intptr_t AddExtendWord();
std::intptr_t AddExtendLong();
std::intptr_t SubtractExtendByte();
std::intptr_t SubtractExtendWord();
std::intptr_t SubtractExtendLong();
std::intptr_t CompareMemoryByte();
std::intptr_t CompareMemoryWord();
std::intptr_t CompareMemoryLong();
std::intptr_t ArithmeticShiftLeft(int a1, int a2, int a3);
std::intptr_t LogicalShiftLeft(int a1, int a2, int a3);
std::intptr_t ArithmeticShiftRight(int a1, int a2, int a3);
std::intptr_t LogicalShiftRight(int a1, int a2, int a3);
std::intptr_t RotateRight(int a1, int a2, int a3);
std::intptr_t RotateLeft(int a1, int a2, int a3);
std::intptr_t RotateExtendRight(int a1, int a2, int a3);
std::intptr_t RotateExtendLeft(int a1, int a2, int a3);
std::intptr_t MoveMultiple(int a1);
std::intptr_t BitTest(std::uint8_t a1);
std::intptr_t BitChange(std::int8_t a1);
std::intptr_t BitClear(std::int8_t a1);
std::intptr_t BitSet(std::int8_t a1);
std::intptr_t MovePeripheral();
std::intptr_t DivideUnsigned(int a1);
std::intptr_t DivideSigned(int a1);
std::intptr_t HandleInterruptRequest(unsigned int a1);
std::intptr_t ResetContext();
std::intptr_t ReturnException();
std::intptr_t OrImmediateByte();
std::intptr_t OrImmediateWord();
std::intptr_t OrImmediateLong();
std::intptr_t AndImmediateByte();
std::intptr_t AndImmediateWord();
std::intptr_t AndImmediateLong();
std::intptr_t XorImmediateByte();
std::intptr_t XorImmediateWord();
std::intptr_t XorImmediateLong();
std::intptr_t SubtractImmediateByte();
std::intptr_t SubtractImmediateWord();
std::intptr_t SubtractImmediateLong();
std::intptr_t AddImmediateByte();
std::intptr_t AddImmediateWord();
std::intptr_t AddImmediateLong();
std::intptr_t CompareImmediateByte();
std::intptr_t CompareImmediateWord();
std::intptr_t CompareImmediateLong();
std::intptr_t BitTestImmediate();
std::intptr_t BitChangeImmediate();
std::intptr_t BitClearImmediate();
std::intptr_t BitSetImmediate();
std::intptr_t BitTestDataOrMovePeripheral();
std::intptr_t BitChangeDataOrMovePeripheral();
std::intptr_t BitClearDataOrMovePeripheral();
std::intptr_t BitSetDataOrMovePeripheral();
std::intptr_t ReportInstructionError();
void DispatchGroup0();
void MoveByte();
void MoveLong();
void MoveWord();
std::intptr_t NegateExtendByteThunk();
std::intptr_t NegateExtendWordThunk();
std::intptr_t NegateExtendLongThunk();
std::intptr_t MoveFromStatus();
std::intptr_t MoveFromConditionCodes();
std::intptr_t MoveToStatus();
std::intptr_t MoveToConditionCodes();
std::intptr_t CheckLongBounds();
std::intptr_t CheckWordBounds();
std::intptr_t LoadEffectiveAddressA0();
std::intptr_t LoadEffectiveAddressA1();
std::intptr_t LoadEffectiveAddressA2();
std::intptr_t LoadEffectiveAddressA3();
std::intptr_t LoadEffectiveAddressA4();
std::intptr_t LoadEffectiveAddressA5();
std::intptr_t LoadEffectiveAddressA6();
std::intptr_t LoadEffectiveAddressA7();
std::intptr_t ClearByte();
std::intptr_t ClearWord();
std::intptr_t ClearLong();
std::intptr_t NegateByteThunk();
std::intptr_t NegateWordThunk();
std::intptr_t NegateLongThunk();
std::intptr_t NotByteThunk();
std::intptr_t NotWordThunk();
std::intptr_t NotLongThunk();
std::intptr_t TestByte();
std::intptr_t TestWord();
std::intptr_t TestLong();
std::intptr_t NegateBcdThunk();
std::intptr_t SwapOrPushEffectiveAddress();
std::intptr_t ExtendByteOrMoveMultipleWord();
std::intptr_t ExtendWordOrMoveMultipleLong();
std::intptr_t MoveMultipleWordToRegisters();
std::intptr_t MoveMultipleLongToRegisters();
std::intptr_t TestAndSet();
std::intptr_t SystemInstruction();
std::intptr_t JumpSubroutine();
std::intptr_t Jump();
void DispatchGroup4();
std::intptr_t AddQuickByte();
std::intptr_t AddQuickWord();
std::intptr_t AddQuickLong();
std::intptr_t SubtractQuickByte();
std::intptr_t SubtractQuickWord();
std::intptr_t SubtractQuickLong();
std::intptr_t SetOrDecrementCondition();
void DispatchGroup5();
void Branch();
void MoveQuick();
void DispatchGroup8();
void DispatchGroupC();
void DispatchGroupB();
void DispatchGroup9();
void DispatchGroupD();
void ShiftRotate();

}

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

inline constexpr emu68::ReadOperand kEmu68_CompareMemoryWordSource[2] = {
    &emu68::ReadDataWord,
    &emu68::ReadPredecrementWord,
};

inline constexpr emu68::WriteOperand kEmu68_WriteLong[8] = {
    &emu68::WriteDataLong,
    &emu68::WriteAddressLong,
    &emu68::WriteEffectiveLong,
    &emu68::WritePostincrementLong,
    &emu68::WriteEffectiveLong,
    &emu68::WriteEffectiveLong,
    &emu68::WriteEffectiveLong,
    &emu68::WriteExtendedLong,
};

inline constexpr emu68::Operation kEmu68_Group0[64] = {
    &emu68::OrImmediateByte,
    &emu68::OrImmediateWord,
    &emu68::OrImmediateLong,
    &emu68::ReportInstructionError,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
    &emu68::AndImmediateByte,
    &emu68::AndImmediateWord,
    &emu68::AndImmediateLong,
    &emu68::ReportInstructionError,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
    &emu68::SubtractImmediateByte,
    &emu68::SubtractImmediateWord,
    &emu68::SubtractImmediateLong,
    &emu68::ReportInstructionError,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
    &emu68::AddImmediateByte,
    &emu68::AddImmediateWord,
    &emu68::AddImmediateLong,
    &emu68::ReportInstructionError,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
    &emu68::BitTestImmediate,
    &emu68::BitChangeImmediate,
    &emu68::BitClearImmediate,
    &emu68::BitSetImmediate,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
    &emu68::XorImmediateByte,
    &emu68::XorImmediateWord,
    &emu68::XorImmediateLong,
    &emu68::ReportInstructionError,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
    &emu68::CompareImmediateByte,
    &emu68::CompareImmediateWord,
    &emu68::CompareImmediateLong,
    &emu68::ReportInstructionError,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
    &emu68::ReportInstructionError,
    &emu68::ReportInstructionError,
    &emu68::ReportInstructionError,
    &emu68::ReportInstructionError,
    &emu68::BitTestDataOrMovePeripheral,
    &emu68::BitChangeDataOrMovePeripheral,
    &emu68::BitClearDataOrMovePeripheral,
    &emu68::BitSetDataOrMovePeripheral,
};

inline constexpr emu68::ReadOperand kEmu68_ReadModifyByte[8] = {
    &emu68::ReadDataByte,
    &emu68::ReportByteReadModeError,
    &emu68::ReadModifyIndirectByte,
    &emu68::ReadModifyIndirectByte,
    &emu68::ReadModifyPredecrementByte,
    &emu68::ReadModifyDisplacementByte,
    &emu68::ReadModifyIndexedByte,
    &emu68::ReadModifyExtendedByte,
};

inline constexpr emu68::WriteOperand kEmu68_WriteByte[8] = {
    &emu68::WriteDataByte,
    &emu68::ReportWriteModeError,
    &emu68::WriteEffectiveByte,
    &emu68::WritePostincrementByte,
    &emu68::WriteEffectiveByte,
    &emu68::WriteEffectiveByte,
    &emu68::WriteEffectiveByte,
    &emu68::WriteExtendedByte,
};

inline constexpr emu68::ReadOperand kEmu68_ReadModifyWord[8] = {
    &emu68::ReadDataWord,
    &emu68::ReadAddressWord,
    &emu68::ReadModifyIndirectWord,
    &emu68::ReadModifyIndirectWord,
    &emu68::ReadModifyPredecrementWord,
    &emu68::ReadModifyDisplacementWord,
    &emu68::ReadModifyIndexedWord,
    &emu68::ReadModifyExtendedWord,
};

inline constexpr emu68::WriteOperand kEmu68_WriteWord[8] = {
    &emu68::WriteDataWord,
    &emu68::WriteAddressWord,
    &emu68::WriteEffectiveWord,
    &emu68::WritePostincrementWord,
    &emu68::WriteEffectiveWord,
    &emu68::WriteEffectiveWord,
    &emu68::WriteEffectiveWord,
    &emu68::WriteExtendedWord,
};

inline constexpr emu68::ReadOperand kEmu68_ReadModifyLong[8] = {
    &emu68::ReadDataLong,
    &emu68::ReadAddressRegister,
    &emu68::ReadModifyIndirectLong,
    &emu68::ReadModifyIndirectLong,
    &emu68::ReadModifyPredecrementLong,
    &emu68::ReadModifyDisplacementLong,
    &emu68::ReadModifyIndexedLong,
    &emu68::ReadModifyExtendedLong,
};

inline constexpr emu68::ReadOperand kEmu68_ReadByte[8] = {
    &emu68::ReadDataByte,
    &emu68::ReportByteReadModeError,
    &emu68::ReadIndirectByte,
    &emu68::ReadPostincrementByte,
    &emu68::ReadPredecrementByte,
    &emu68::ReadDisplacementByte,
    &emu68::ReadIndexedByte,
    &emu68::ReadExtendedByte,
};

inline constexpr emu68::ReadOperand kEmu68_ReadWord[8] = {
    &emu68::ReadDataWord,
    &emu68::ReadAddressWord,
    &emu68::ReadIndirectWord,
    &emu68::ReadPostincrementWord,
    &emu68::ReadPredecrementWord,
    &emu68::ReadDisplacementWord,
    &emu68::ReadIndexedWord,
    &emu68::ReadExtendedWord,
};

inline constexpr emu68::ReadOperand kEmu68_ReadLong[8] = {
    &emu68::ReadDataLong,
    &emu68::ReadAddressRegister,
    &emu68::ReadIndirectLong,
    &emu68::ReadPostincrementLong,
    &emu68::ReadPredecrementLong,
    &emu68::ReadDisplacementLong,
    &emu68::ReadIndexedLong,
    &emu68::ReadExtendedLong,
};

inline constexpr emu68::ReadOperand kEmu68_CompareMemoryByteSource[2] = {
    &emu68::ReadDataByte,
    &emu68::ReadPredecrementByte,
};

inline constexpr emu68::ReadOperand kEmu68_PredecrementByteDestination[2] = {
    &emu68::ReadDataByte,
    &emu68::ReadModifyPredecrementByte,
};

inline constexpr emu68::WriteOperand kEmu68_WriteBytePair[2] = {
    &emu68::WriteDataByte,
    &emu68::WriteEffectiveByte,
};

inline constexpr emu68::ReadOperand kEmu68_PredecrementWordDestination[2] = {
    &emu68::ReadDataWord,
    &emu68::ReadModifyPredecrementWord,
};

inline constexpr emu68::WriteOperand kEmu68_WriteWordPair[2] = {
    &emu68::WriteDataWord,
    &emu68::WriteEffectiveWord,
};

inline constexpr emu68::ReadOperand kEmu68_CompareMemoryLongSource[2] = {
    &emu68::ReadDataLong,
    &emu68::ReadPredecrementLong,
};

inline constexpr emu68::ReadOperand kEmu68_PredecrementLongDestination[2] = {
    &emu68::ReadDataLong,
    &emu68::ReadModifyPredecrementLong,
};

inline constexpr emu68::WriteOperand kEmu68_WriteLongPair[2] = {
    &emu68::WriteDataLong,
    &emu68::WriteEffectiveLong,
};

inline constexpr emu68::ReadOperand kEmu68_Address[8] = {
    &emu68::ReportAddressModeError,
    &emu68::ReportAddressModeError,
    &emu68::ReadAddressRegister,
    &emu68::ReportAddressModeError,
    &emu68::ReportAddressModeError,
    &emu68::AddressDisplacement,
    &emu68::AddressIndexed,
    &emu68::AddressExtended,
};

inline constexpr emu68::ReadOperand kEmu68_SetEffectiveByte[8] = {
    &emu68::NullOperand,
    &emu68::ReportWriteModeError,
    &emu68::SetEffectiveIndirect,
    &emu68::SetEffectiveIndirect,
    &emu68::SetEffectivePredecrementByte,
    &emu68::SetEffectiveDisplacement,
    &emu68::SetEffectiveIndexed,
    &emu68::SetEffectiveExtended,
};

inline constexpr emu68::ReadOperand kEmu68_SetEffectiveLong[8] = {
    &emu68::NullOperand,
    &emu68::NullOperand,
    &emu68::SetEffectiveIndirect,
    &emu68::SetEffectiveIndirect,
    &emu68::SetEffectivePredecrementLong,
    &emu68::SetEffectiveDisplacement,
    &emu68::SetEffectiveIndexed,
    &emu68::SetEffectiveExtended,
};

inline constexpr emu68::ReadOperand kEmu68_SetEffectiveWord[8] = {
    &emu68::NullOperand,
    &emu68::NullOperand,
    &emu68::SetEffectiveIndirect,
    &emu68::SetEffectiveIndirect,
    &emu68::SetEffectivePredecrementWord,
    &emu68::SetEffectiveDisplacement,
    &emu68::SetEffectiveIndexed,
    &emu68::SetEffectiveExtended,
};

inline constexpr emu68::Operation kEmu68_Group5[8] = {
    &emu68::AddQuickByte,
    &emu68::AddQuickWord,
    &emu68::AddQuickLong,
    &emu68::SetOrDecrementCondition,
    &emu68::SubtractQuickByte,
    &emu68::SubtractQuickWord,
    &emu68::SubtractQuickLong,
    &emu68::SetOrDecrementCondition,
};

inline constexpr emu68::Operation kEmu68_Group4[64] = {
    &emu68::NegateExtendByteThunk,
    &emu68::NegateExtendWordThunk,
    &emu68::NegateExtendLongThunk,
    &emu68::MoveFromStatus,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA0,
    &emu68::ClearByte,
    &emu68::ClearWord,
    &emu68::ClearLong,
    &emu68::MoveFromConditionCodes,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA1,
    &emu68::NegateByteThunk,
    &emu68::NegateWordThunk,
    &emu68::NegateLongThunk,
    &emu68::MoveToConditionCodes,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA2,
    &emu68::NotByteThunk,
    &emu68::NotWordThunk,
    &emu68::NotLongThunk,
    &emu68::MoveToStatus,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA3,
    &emu68::NegateBcdThunk,
    &emu68::SwapOrPushEffectiveAddress,
    &emu68::ExtendByteOrMoveMultipleWord,
    &emu68::ExtendWordOrMoveMultipleLong,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA4,
    &emu68::TestByte,
    &emu68::TestWord,
    &emu68::TestLong,
    &emu68::TestAndSet,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA5,
    &emu68::ReportInstructionError,
    &emu68::ReportInstructionError,
    &emu68::MoveMultipleWordToRegisters,
    &emu68::MoveMultipleLongToRegisters,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA6,
    &emu68::ReportInstructionError,
    &emu68::SystemInstruction,
    &emu68::JumpSubroutine,
    &emu68::Jump,
    &emu68::CheckLongBounds,
    &emu68::ReportInstructionError,
    &emu68::CheckWordBounds,
    &emu68::LoadEffectiveAddressA7,
};

inline emu68::OpcodeHandler g_emu68OpcodeDispatch[16] = {
    &emu68::DispatchGroup0,
    &emu68::MoveByte,
    &emu68::MoveLong,
    &emu68::MoveWord,
    &emu68::DispatchGroup4,
    &emu68::DispatchGroup5,
    &emu68::Branch,
    &emu68::MoveQuick,
    &emu68::DispatchGroup8,
    &emu68::DispatchGroup9,
    nullptr,
    &emu68::DispatchGroupB,
    &emu68::DispatchGroupC,
    &emu68::DispatchGroupD,
    &emu68::ShiftRotate,
    &emu68::ReportLineFError,
};

inline std::array<reg68_page_map_entry_t, 256> g_reg68_ctx_map_funcs = [] {
    std::array<reg68_page_map_entry_t, 256> pages{};
    for (reg68_page_map_entry_t &page : pages) {
        page = {.read8 = &emu68::UnmappedRead8,
            .write8 = &emu68::IgnoreWrite8,
            .read16 = &emu68::UnmappedRead16,
            .write16 = &emu68::IgnoreWrite16,
            .read32 = &emu68::UnmappedRead32,
            .write32 = &emu68::IgnoreWrite32,
            .base = 0,
            .mask = 0xFFFFFFFF};
    }
    pages[0x00] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &emu68::IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &emu68::IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &emu68::IgnoreWrite32,
        .base = 0x0,
        .mask = 0xFFFF};
    pages[0x01] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &emu68::IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &emu68::IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &emu68::IgnoreWrite32,
        .base = 0x10000,
        .mask = 0xFFFF};
    pages[0x02] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &emu68::IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &emu68::IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &emu68::IgnoreWrite32,
        .base = 0x20000,
        .mask = 0xFFFF};
    pages[0x03] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &emu68::IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &emu68::IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &emu68::IgnoreWrite32,
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
        page = {.read8 = &emu68::UnmappedRead8,
            .write8 = &emu68::IgnoreWrite8,
            .read16 = &emu68::UnmappedRead16,
            .write16 = &emu68::IgnoreWrite16,
            .read32 = &emu68::UnmappedRead32,
            .write32 = &emu68::IgnoreWrite32,
            .base = 0,
            .mask = 0xFFFFFFFF};
    }
    pages[0x00] = {.read8 = &Harrier_ReadMemory8,
        .write8 = &emu68::IgnoreWrite8,
        .read16 = &Harrier_ReadMemory16BigEndian,
        .write16 = &emu68::IgnoreWrite16,
        .read32 = &Harrier_ReadMemory32BigEndian,
        .write32 = &emu68::IgnoreWrite32,
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
