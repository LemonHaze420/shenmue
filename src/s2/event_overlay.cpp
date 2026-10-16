#include "s2/area_event.hpp"

#if SHENMUE_IS_DC

#include <array>
#include <cstdio>
#include <cstring>

extern "C" void CopyEventOverlayWords(void *, const void *, std::size_t) asm("s2_dc_copy_event_words");
extern "C" void CopyEventOverlayAligned(void *, const void *, std::size_t) asm("s2_dc_copy_event_aligned");
extern "C" void FlushEventOperandCache() asm("s2_dc_flush_event_operand_cache");
extern "C" void SetEventCacheControl(std::uint32_t) asm("s2_dc_set_event_cache_control");

// The cache-control tails must execute through P2, with no cached stack accesses while CCR changes.
asm(R"(
    .text
    .align 2
    .global s2_dc_copy_event_words
    .type s2_dc_copy_event_words,@function
s2_dc_copy_event_words:
    add #3,r6
    shlr2 r6
.Levent_copy_words:
    mov.l @r5+,r0
    dt r6
    mov.l r0,@r4
    bf/s .Levent_copy_words
    add #4,r4
    rts
    nop
    .size s2_dc_copy_event_words,.-s2_dc_copy_event_words

    .align 2
    .global s2_dc_copy_event_aligned
    .type s2_dc_copy_event_aligned,@function
s2_dc_copy_event_aligned:
    add #31,r6
    mov #-5,r0
    add #32,r4
    shld r0,r6
    fschg
.Levent_copy_aligned:
    fmov @r5+,dr0
    fmov @r5+,dr2
    fmov @r5+,dr4
    fmov @r5+,dr6
    dt r6
    fmov dr6,@-r4
    fmov dr4,@-r4
    fmov dr2,@-r4
    fmov dr0,@-r4
    bf/s .Levent_copy_aligned
    add #64,r4
    rts
    fschg
    .size s2_dc_copy_event_aligned,.-s2_dc_copy_event_aligned

    .align 2
    .global s2_dc_flush_event_operand_cache
    .type s2_dc_flush_event_operand_cache,@function
s2_dc_flush_event_operand_cache:
    stc sr,r5
    mov.w .Levent_flush_mask,r1
    or r5,r1
    ldc r1,sr
    mov #-12,r2
    extu.b r2,r2
    swap.b r2,r2
    swap.w r2,r2
    mov #2,r3
    swap.b r3,r3
    mov #-1,r4
    extu.b r4,r4
    swap.b r4,r4
    swap.w r4,r4
    mov #28,r0
    or r0,r4
    mov #62,r6
    shll2 r6
    shll2 r6
    mov #-96,r1
    extu.b r1,r1
    swap.b r1,r1
    swap.w r1,r1
    mova .Levent_flush_p2,r0
    or r0,r1
    jmp @r1
    nop
.Levent_flush_mask:
    .word 0x00e0
    .align 2
.Levent_flush_p2:
    mov r2,r7
    mov.l @r2,r0
    and r6,r7
    or r7,r0
    dt r3
    ocbp @r0
    bf/s .Levent_flush_p2
    add #32,r2
    mov #0,r0
    mov.l r0,@r4
    .rept 9
    nop
    .endr
    ldc r5,sr
    nop
    nop
    rts
    nop
    .size s2_dc_flush_event_operand_cache,.-s2_dc_flush_event_operand_cache

    .align 2
    .global s2_dc_set_event_cache_control
    .type s2_dc_set_event_cache_control,@function
s2_dc_set_event_cache_control:
    stc sr,r2
    mov.w .Levent_control_mask,r1
    or r2,r1
    ldc r1,sr
    mov.l .Levent_control_tail,r0
    mov.l .Levent_p2_alias,r1
    or r1,r0
    mov.l .Levent_ccr,r1
    mov.l .Levent_control_return,r3
    jmp @r0
    nop
.Levent_control_mask:
    .word 0x00e0
    .align 2
.Levent_control_tail:
    .long .Levent_control_p2
.Levent_p2_alias:
    .long 0xa0000000
.Levent_ccr:
    .long 0xff00001c
.Levent_control_return:
    .long .Levent_control_done
.Levent_control_p2:
    mov.l @r1,r0
    or r4,r0
    mov.l r0,@r1
    .rept 8
    nop
    .endr
    ldc r2,sr
    jmp @r3
    nop
.Levent_control_done:
    rts
    nop
    .size s2_dc_set_event_cache_control,.-s2_dc_set_event_cache_control
)");

namespace shenmue::s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
constexpr std::array<std::uintptr_t, 4> eventExecutionRegions{0x0df80000U, 0x0c328000U, 0x0c338000U, 0x0c348000U};
#else
constexpr std::array<std::uintptr_t, 4> eventExecutionRegions{0x8df80000U, 0x8c328000U, 0x8c338000U, 0x8c348000U};
#endif

void *GetEventExecutionRegion(EventFunctionType functionType) {
    const std::uint32_t type = static_cast<std::uint32_t>(functionType);
    return type - 1U < 3U ? reinterpret_cast<void *>(eventExecutionRegions[type]) : nullptr;
}

void InitializeEventExecutionRegions() {
    std::memset(reinterpret_cast<void *>(eventExecutionRegions[1]), 0, 0x10000);
    std::memset(reinterpret_cast<void *>(eventExecutionRegions[2]), 0, 0x10000);
    std::memset(reinterpret_cast<void *>(eventExecutionRegions[3]), 0, 0x18000);
}

const void *InstallEventOverlay(const void *payload, const char *stem, std::size_t length, EventFunctionType functionType) {
    void *const destination = GetEventExecutionRegion(functionType);
    if (!destination)
        return nullptr;
    if ((reinterpret_cast<std::uintptr_t>(payload) & 7U) == 0)
        CopyEventOverlayAligned(destination, payload, length);
    else
        CopyEventOverlayWords(destination, payload, length);
    char eventFile[16];
    std::snprintf(eventFile, sizeof(eventFile), "%.8s.EV%01u", stem, static_cast<unsigned>(functionType));
    shenLoadEventFrom(eventFile, functionType);
    return payload;
}

void ExecuteEventOverlay(std::uint32_t slot, std::uintptr_t argument) {
    std::uint32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status = (status & ~0xf0U) | 0xe0U;
    asm volatile("ldc %0,sr" : : "r"(status) : "memory");
    FlushEventOperandCache();
    SetEventCacheControl(0x105);
    asm volatile("stc sr,%0" : "=r"(status));
    status &= ~0xf0U;
    asm volatile("ldc %0,sr" : : "r"(status) : "memory");
    reinterpret_cast<ScriptAreaMainFunction>(eventExecutionRegions[slot])(argument);
}

} // namespace shenmue::s2

#endif
