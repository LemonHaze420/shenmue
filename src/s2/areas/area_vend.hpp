#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name VEND_AREA{SHEN_NAME(VEND)}, VEND_MAIN_TASK{SHEN_NAME(VND1)}, VEND_PROC_TASK{SHEN_NAME(VND9)};
inline constexpr shen_name VEND_RYO{SHEN_NAME(RYO_)}, VEND_HAND{SHEN_NAME(chnd)}, VEND_BASE{SHEN_NAME(DUMH)}, VEND_CAN{SHEN_NAME(HCAN)};
inline constexpr shen_name VEND_RED{SHEN_NAME(JHNR)}, VEND_WHITE{SHEN_NAME(JHNW)}, VEND_GREEN{SHEN_NAME(JHNG)};
inline constexpr shen_name VEND_ACCEPT{SHEN_NAME(BTEV)}, VEND_CANCEL{SHEN_NAME(BTCC)};
inline constexpr shen_name vendCharacterNames[]{VEND_RYO, EVENT_CHARACTER_INVALID};
enum VendMachState : int32_t {
    VEND_CHECK_CASH,
    VEND_FADE,
    VM_VEND_TRANS_CHARA = 2,
};

enum VendingMachineColour : std::int32_t { VM_COLOR_UNKNOWN = 0, VM_COLOR_RED = 1, VM_COLOR_WHITE = 2, VM_COLOR_GREEN = 3 };
enum VendingCanKind : std::uint16_t { VEND_CAN_STANDARD = 0, VEND_CAN_ALTERNATE1 = 1, VEND_CAN_ALTERNATE2 = 2 };
enum VendingCanSound : std::uint16_t { VEND_SOUND_STANDARD = 0, VEND_SOUND_ALTERNATE = 1 };
enum VendingHandState : std::int32_t { EVVendHand_INIT = 0, EVVendHand_END = 2, EVVendHand_RETIRED = 3, EVVendHand_ERROR = 4 };
struct VendingMachineWork {
    EventReadCharactersWork *characters;
    Vector3 buttons[8];
    std::int32_t buttonCount, upperButtons, selected;
    std::uint32_t reserved74;
    HLib::Task *procTask;
    HLib::Task *task;
    shen_name originalMachine;
    std::byte reserved8c[0x1c];
    std::uint32_t sound, prizeState, prizeItem, contentState, voiceState;
    std::byte reservedbc[0xc];
    std::int32_t colour, previousSelected, archiveRecord, alternateRecord;
    std::uint16_t canKind, reservedda;
    std::int32_t wait, firstSelection;
    std::uint32_t reservede4;
};
static_assert(sizeof(VendingMachineWork) == 0xe8);
static_assert(offsetof(VendingMachineWork, buttons) == 8 && offsetof(VendingMachineWork, buttonCount) == 0x68);
static_assert(offsetof(VendingMachineWork, procTask) == 0x78 && offsetof(VendingMachineWork, originalMachine) == 0x88);
static_assert(offsetof(VendingMachineWork, sound) == 0xa8 && offsetof(VendingMachineWork, voiceState) == 0xb8);
static_assert(offsetof(VendingMachineWork, colour) == 0xc8 && offsetof(VendingMachineWork, canKind) == 0xd8);
static_assert(offsetof(VendingMachineWork, firstSelection) == 0xe0);
struct VendingLampBank {
    std::int32_t selected, parts[8];
    Vector3 positions[8];
};
static_assert(sizeof(VendingLampBank) == 0x84 && offsetof(VendingLampBank, positions) == 0x24);
struct VendingAutoWaitWork {
    HLib::Task *task;
    std::int32_t begin, end, timer, state;
    HLib::TaskCallback callback;
    void *argument;
};
static_assert(sizeof(VendingAutoWaitWork) == 0x28 && offsetof(VendingAutoWaitWork, callback) == 0x18);
struct VendingHandWaitWork {
    std::uint64_t reserved00;
    HLib::Task *task;
    shen_name character;
    std::int32_t side;
    std::int32_t command[57];
    std::byte unusedAdjacentData[456];
    std::int32_t duration, threshold, timer, state;
    std::uint32_t reserved2d4;
};
static_assert(sizeof(VendingHandWaitWork) == 0x2d8 && offsetof(VendingHandWaitWork, duration) == 0x2c4);
struct VendingPartWork {
    shen_name character;
    std::int32_t part;
    Vector3 position;
    Vector3i rotation;
    std::int32_t cleanup;
};
struct VendingItemWork {
    std::int32_t cleanup;
    std::uint32_t item;
};
static_assert(sizeof(VendingPartWork) == 0x24 && offsetof(VendingPartWork, cleanup) == 0x20);
static_assert(sizeof(VendingItemWork) == 8);
struct VendingCanRecord {
    std::int32_t record, alternate;
    std::uint16_t sound, kind;
};
static_assert(sizeof(VendingCanRecord) == 0xc);
inline constexpr VendingCanRecord vendRedCans[]{{.record = 0, .alternate = 1, .sound = VEND_SOUND_ALTERNATE, .kind = VEND_CAN_STANDARD},
    {.record = 0, .alternate = 1, .sound = VEND_SOUND_ALTERNATE, .kind = VEND_CAN_STANDARD},
    {.record = 0, .alternate = 1, .sound = VEND_SOUND_ALTERNATE, .kind = VEND_CAN_STANDARD},
    {.record = 4, .alternate = 5, .sound = VEND_SOUND_ALTERNATE, .kind = VEND_CAN_STANDARD},
    {.record = 2, .alternate = 3, .sound = VEND_SOUND_ALTERNATE, .kind = VEND_CAN_STANDARD},
    {.record = 8, .alternate = 9, .sound = VEND_SOUND_ALTERNATE, .kind = VEND_CAN_STANDARD},
    {.record = 6, .alternate = 7, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_ALTERNATE1},
    {.record = 6, .alternate = 7, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_ALTERNATE1},
    {.record = 10, .alternate = 11, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_STANDARD}};
inline constexpr VendingCanRecord vendOtherCans[]{{.record = 2, .alternate = 3, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_ALTERNATE2},
    {.record = 2, .alternate = 3, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_ALTERNATE2},
    {.record = 6, .alternate = 7, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_STANDARD},
    {.record = 6, .alternate = 7, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_STANDARD},
    {.record = 4, .alternate = 5, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_STANDARD},
    {.record = 4, .alternate = 5, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_STANDARD},
    {.record = 2, .alternate = 3, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_ALTERNATE2},
    {.record = 4, .alternate = 5, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_STANDARD},
    {.record = 0, .alternate = 1, .sound = VEND_SOUND_STANDARD, .kind = VEND_CAN_STANDARD}};
struct VendingMapColour {
    shen_name map;
    std::int32_t colour;
};
inline constexpr VendingMapColour vendMapColours[]{{.map = SHEN_NAME(AR01), .colour = VM_COLOR_GREEN}, {.map = SHEN_NAME(AR02), .colour = VM_COLOR_GREEN},
    {.map = SHEN_NAME(AR02), .colour = VM_COLOR_GREEN}, {.map = SHEN_NAME(AR03), .colour = VM_COLOR_GREEN}, {.map = SHEN_NAME(AK00), .colour = VM_COLOR_RED},
    {.map = SHEN_NAME(AKS0), .colour = VM_COLOR_RED}, {.map = SHEN_NAME(AB00), .colour = VM_COLOR_WHITE}, {.map = SHEN_NAME(WS00), .colour = VM_COLOR_GREEN},
    {.map = SHEN_NAME(WK00), .colour = VM_COLOR_GREEN}, {.map = SHEN_NAME(WT00), .colour = VM_COLOR_WHITE}, {.map = SHEN_NAME(WN00), .colour = VM_COLOR_WHITE},
    {.map = SHEN_NAME(WR00), .colour = VM_COLOR_WHITE}, {.map = SHEN_NAME(WE00), .colour = VM_COLOR_GREEN}, {.map = SHEN_NAME(WEG0), .colour = VM_COLOR_GREEN},
    {.map = SHEN_NAME(WES1), .colour = VM_COLOR_GREEN}, {.map = SHEN_NAME(Q100), .colour = VM_COLOR_GREEN}, {.map = SHEN_NAME(Q200), .colour = VM_COLOR_RED},
    {.map = SHEN_NAME(Q300), .colour = VM_COLOR_WHITE}, {.map = SHEN_NAME(QE01), .colour = VM_COLOR_WHITE}, {.map = SHEN_NAME(QE00), .colour = VM_COLOR_WHITE}};
inline constexpr float vendInterpolation[]{0.0f, .022727f, .113636f, .318182f, .681818f, .886364f, .977273f, 1.0f};
inline constexpr Vector3 vendHandOffset{.x = -.186f, .y = .01f, .z = .427f};
inline constexpr Vector3i vendHandAngle{.x = 0x9555, .y = 0x8000, .z = 0};
inline constexpr Vector3i vendSelectionHandRotation{.x = 0xd8e6, .y = 0, .z = 0xccc};
inline constexpr Vector3i vendCanHandRotation{.x = 0xfb6e, .y = 0xfec3, .z = 0x1c2};
inline constexpr Vector3i vendAlternateHandRotation{.x = 0xfb6e, .y = 0xfec3, .z = 0xfcc8};
inline constexpr Vector3 vendCanAttachment{.x = .112f, .y = .029f, .z = .002f};
inline constexpr Vector3i vendCanAttachmentRotation{.x = 0x1b05, .y = 0x2e93, .z = 0xe38};
void InitializeVendingMachineEvent();
void RunVendingMachineEvent(std::uintptr_t argument);
inline constexpr ScriptAreaDefinition vendingMachineScriptDefinition{
    .areaId = VEND_AREA, .field04 = 0, .init = InitializeVendingMachineEvent, .main = RunVendingMachineEvent};
} // namespace shenmue::s2
