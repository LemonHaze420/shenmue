#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

#pragma pack(push, 1)
struct PadCaptureEntry {
    std::uint32_t flags;
    std::uint16_t enabled;
    std::uint16_t unknownMask;
    std::uint16_t held;
    std::uint16_t previousHeld;
    std::uint16_t pressedPriority;
    std::uint16_t repeatedPresses;
    std::uint16_t triggers;
    std::uint16_t rightStick;
    std::uint32_t leftStick;
    std::uint16_t xinputReserved;
    std::uint16_t padding;
};

struct PadCaptureTaskData {
    PadCaptureEntry entries[16];
    PadCaptureEntry *currentBoundEntry;
};
#pragma pack(pop)

static_assert(sizeof(PadCaptureEntry) == 0x1c);
static_assert(offsetof(PadCaptureEntry, flags) == 0x00);
static_assert(offsetof(PadCaptureEntry, enabled) == 0x04);
static_assert(offsetof(PadCaptureEntry, held) == 0x08);
static_assert(offsetof(PadCaptureEntry, repeatedPresses) == 0x0e);
static_assert(offsetof(PadCaptureEntry, triggers) == 0x10);
static_assert(offsetof(PadCaptureEntry, leftStick) == 0x14);
static_assert(offsetof(PadCaptureEntry, xinputReserved) == 0x18);
static_assert(sizeof(PadCaptureTaskData) == 0x1c8);
static_assert(offsetof(PadCaptureTaskData, currentBoundEntry) == 0x1c0);

extern HLib::Task *g_pPadCaptureTask;
extern std::uint16_t g_xinputReserved;

PadCaptureTaskData *PADC_Init();
void PADC_Shutdown();
PadCaptureTaskData *PADC_CurrentData();
void PADC_Task_Callback(HLib::TaskData *taskData);
PadCaptureEntry *AllocateClearedPADCEntry();
PadCaptureEntry *shenUpdateInputsInto(PadCaptureEntry *entry);
bool ReleasePADCEntry(PadCaptureEntry *entry, PadCaptureEntry *restoreEntry = nullptr);
PadCaptureEntry *BindMapControlPADCEntry();
std::uint16_t ReadPadCaptureMode(const PadCaptureEntry *entry, std::uint32_t mode);

} // namespace shenmue::s1
