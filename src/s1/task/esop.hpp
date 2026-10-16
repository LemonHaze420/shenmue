#pragma once

#include "s1/hlib.hpp"
#include "s1/task/padc.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

#pragma pack(push, 1)
struct EasyOptionState {
    std::uint16_t field_000;
    std::byte reserved_002[2];
    float field_004;
    std::byte reserved_008[4];
    float field_00c;
    float field_010;
    float field_014;
    std::uint16_t field_018;
    std::byte reserved_01a[2];
    std::uint16_t field_01c[2];
    std::byte reserved_020[2];
    std::uint32_t field_022;
    std::uint8_t field_026;
    std::byte reserved_027;
    std::int16_t field_028;
    std::int16_t field_02a;
    float vector_02c[4];
    float field_03c;
    std::byte reserved_040[2];
    std::uint16_t field_042;
    std::byte reserved_044[0x10];
    std::int16_t fields_054[15];
    std::byte reserved_072[0x12];
    std::uint16_t field_084;
    std::byte reserved_086[6];
    PadCaptureEntry *previousPadEntry;
    PadCaptureEntry *padEntry;
    std::uint8_t optionValues[4];
    std::byte reserved_0a0[4];
    std::uint8_t menuKind;
    std::uint8_t dialogueValueToken;
    std::uint8_t optionRowTokens[10];
    std::uint8_t optionCount;
    std::uint8_t binaryValueTokens[2];
    std::byte reserved_0b3;
    std::uint8_t selectedOption;
    std::uint8_t selectedValue;
    std::uint8_t embeddedOwner;
    std::byte reserved_0b7;
    std::uint8_t state;
    std::uint8_t nextState;
    std::uint8_t previousSicnPausePolicy;
    std::uint8_t previousSicnActive;
    std::uint16_t previousSicnState;
    std::uint16_t previousHelpToken;
    std::byte reserved_0c0[4];
    void *field_0c4;
    float field_0cc;
    float field_0d0;
    float field_0d4;
    std::uint16_t field_0d8;
    std::byte reserved_0da[2];
    std::uint32_t field_0dc;
    std::uint16_t field_0e0;
    std::uint32_t field_0e2;
    std::uint8_t field_0e6;
};
#pragma pack(pop)

static_assert(offsetof(EasyOptionState, fields_054) == 0x54);
static_assert(offsetof(EasyOptionState, previousPadEntry) == 0x8c);
static_assert(offsetof(EasyOptionState, padEntry) == 0x94);
static_assert(offsetof(EasyOptionState, optionValues) == 0x9c);
static_assert(offsetof(EasyOptionState, selectedOption) == 0xb4);
static_assert(offsetof(EasyOptionState, state) == 0xb8);
static_assert(offsetof(EasyOptionState, previousSicnState) == 0xbc);
static_assert(offsetof(EasyOptionState, field_0e2) == 0xe2);
static_assert(sizeof(EasyOptionState) == 0xe7);

struct EasyOptionLabelDefinition {
    const char *text;
    std::uint8_t widthShift;
};

static_assert(offsetof(EasyOptionLabelDefinition, widthShift) == 8);
static_assert(sizeof(EasyOptionLabelDefinition) == 16);

extern EasyOptionState g_easyOptionState;
extern float g_runStartSpeed;

HLib::Task *ESOP_Init();
void ESOP_Shutdown();
HLib::Task *ESOP_GetTask();
void SetupESOP(HLib::TaskData *taskData);
void UP_InitEasyOption_ESOP_Callback(HLib::TaskData *taskData);
bool ESOP_CloseOptionsMenu();

inline constexpr std::array<EasyOptionLabelDefinition, 15> kOptionLabels{{
    {.text = "Dialog/Text", .widthShift = 2},
    {.text = "Sound", .widthShift = 1},
    {.text = "Area Jump", .widthShift = 1},
    {.text = "Toggle Run/", .widthShift = 2},
    {.text = "Zoom Control", .widthShift = 2},
    {.text = "Shenmue Mode", .widthShift = 2},
    {.text = "Cinema Mode", .widthShift = 2},
    {.text = "Text Mode", .widthShift = 1},
    {.text = "Game Mode", .widthShift = 1},
    {.text = "Mono", .widthShift = 0},
    {.text = "Stereo", .widthShift = 1},
    {.text = "ON", .widthShift = 0},
    {.text = "OFF", .widthShift = 0},
    {.text = " :Run ", .widthShift = 1},
    {.text = " :Zoom", .widthShift = 1},
}};

} // namespace shenmue::s1
