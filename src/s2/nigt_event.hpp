#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2 {

inline constexpr shen_name NIGT_AREA{SHEN_NAME(NIGT)}, NIGT_TASK{SHEN_NAME(EVNT)}, NIGT_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name NIGT_YORU{SHEN_NAME(YORU)}, NIGT_LIGHT{SHEN_NAME(0000)}, NIGT_RETURN_AREA{SHEN_NAME(AKA3)};
inline constexpr shen_name NIGT_FINAL_AUTHOR{SHEN_NAME(REDS)};
inline constexpr char NIGT_PACKAGE[] = "NIGT", NIGT_AUTHOR_PACKAGE[] = "0182", NIGT_AUDIO_BANK[] = "AK00_000";

struct NIGTTaskParameter {
    HLib::Task *self;
    HLib::Task *callingParent;
    std::uint32_t authorCommand;
    std::uint32_t state;
};
static_assert(sizeof(NIGTTaskParameter) == 0x18);
static_assert(alignof(NIGTTaskParameter) == 8);
static_assert(offsetof(NIGTTaskParameter, self) == 0x00);
static_assert(offsetof(NIGTTaskParameter, callingParent) == 0x08);
static_assert(offsetof(NIGTTaskParameter, authorCommand) == 0x10);
static_assert(offsetof(NIGTTaskParameter, state) == 0x14);

void NIGTInit();
// The area argument points to the caller's DWORD author command.
void NIGTMain(std::uintptr_t argument);
void NIGTMainCallback(void *taskData);
void NIGTExitCallback(void *taskData);
extern const ScriptAreaDefinition nigtScriptDefinition;

} // namespace shenmue::s2
