#pragma once

#include "s2/area_event.hpp"

#if SHENMUE_IS_DC
extern "C" void CopyEventOverlayWords(void *, const void *, std::size_t) asm("s2_dc_copy_event_words");
extern "C" void CopyEventOverlayAligned(void *, const void *, std::size_t) asm("s2_dc_copy_event_aligned");
extern "C" void FlushEventOperandCache() asm("s2_dc_flush_event_operand_cache");
extern "C" void SetEventCacheControl(std::uint32_t) asm("s2_dc_set_event_cache_control");
#endif

namespace shenmue {
namespace s2 {

struct EventOverlaySlot {
    char name[12];
    std::uint32_t reserved0c;
};
#if SHENMUE_S2_GCC295
typedef char EventOverlaySlotSizeCheck[sizeof(EventOverlaySlot) == 0x10 ? 1 : -1];
#else
static_assert(sizeof(EventOverlaySlot) == 0x10);
#endif

extern EventOverlaySlot g_eventOverlaySlots[4];
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if SHENMUE_S2_GCC295
const std::size_t EVENT_FUNCTION_COUNT = 850;
#else
inline constexpr std::size_t EVENT_FUNCTION_COUNT = 850;
#endif
extern std::uintptr_t g_eventFunctionTable[EVENT_FUNCTION_COUNT] asm("_shenmue_s2_event_function_table");
std::int32_t shenGetAreaSlotByName(const char *name);
void scenFreeEventByIndex(std::uint32_t slot);
#endif

} } // namespace shenmue::s2
