#include "shenmue.h"

#if !SHENMUE_IS_DC || !SHENMUE_IS_S2
#error Select a recovered Dreamcast target
#endif

    .section .text.dc_disc_start,"ax"
    .global _dc_disc_start
    .type _dc_disc_start,@function
_dc_disc_start:
    mov.l .Lgame_start,r0
    jmp @r0
    nop
    .align 2
.Lgame_start: .long _start
    .size _dc_disc_start,.-_dc_disc_start

    .section .text.dc_start,"ax"
    .global _start
    .type _start,@function
// @verified - d
_start:
    mova .Lbss_size,r0
    xor r3,r3
    mov.l @r0+,r1
    mov.l @r0,r2
    tst r1,r1
    bt .Lregisters
.Lclear_bss:
    dt r1
    bf/s .Lclear_bss
    mov.b r3,@-r2
    mov.l .Lloader_word,r1
    mov.l .Lloader_flag,r2
    mov #-1,r3
    mov.w @r1,r1
    xor r0,r0
    cmp/eq r1,r3
    movt r0
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    mov #1,r0
#endif
    mov.l r0,@r2
.Lregisters:
    xor r1,r1
    xor r2,r2
    xor r3,r3
    xor r4,r4
    xor r5,r5
    xor r6,r6
    xor r7,r7
    xor r8,r8
    xor r9,r9
    xor r10,r10
    xor r11,r11
    xor r12,r12
    xor r13,r13
    xor r14,r14
    ldc r1,r0_bank
    ldc r1,r1_bank
    ldc r1,r2_bank
    ldc r1,r3_bank
    ldc r1,r4_bank
    ldc r1,r5_bank
    ldc r1,r6_bank
    ldc r1,r7_bank
    clrmac
    lds r1,fpul
    sts fpscr,r0
    or #3,r0
    xor #3,r0
    lds r0,fpscr
    mov #2,r0
.Lclear_float_bank:
    frchg
    fldi0 fr0
    fldi0 fr1
    fldi0 fr2
    fldi0 fr3
    fldi0 fr4
    fldi0 fr5
    fldi0 fr6
    fldi0 fr7
    fldi0 fr8
    fldi0 fr9
    fldi0 fr10
    fldi0 fr11
    fldi0 fr12
    fldi0 fr13
    fldi0 fr14
    dt r0
    bf/s .Lclear_float_bank
    fldi0 fr15
    mov.l .Lentry,r0
    mov.l .Lstack,r15
    jsr @r0
    nop
.Lreturned:
    bra .Lreturned
    nop
    .align 2
.Lloader_word: .long 0x8c000070
.Lloader_flag: .long _shenmue_dc_loader_sentinel
.Lentry: .long _shenmue_dc_runtime_entry
.Lbss_size: .long __dc_bss_size
    .long __dc_bss_end
.Lstack: .long __dc_stack_top
    .size _start,.-_start

    .section .rodata.dc_take,"a"
    .asciz SHENMUE_TAKE
    .section .data.dc_loader_flag,"aw"
    .global _shenmue_dc_loader_sentinel
_shenmue_dc_loader_sentinel:
    .long 0

    .section .text.dc_runtime_start,"ax"
    .global _shenmue_dc_runtime_entry
    .type _shenmue_dc_runtime_entry,@function
_shenmue_dc_runtime_entry:
    sts.l pr,@-r15
    mov.l r8,@-r15
    sts fpscr,r8
    mov r8,r4
    mov.l .Lset_fpscr,r1
    jsr @r1
    nop
    lds r8,fpscr
    mov.l .Lruntime_initialize,r0
    jsr @r0
    nop
    mov.l .Lmain,r0
    jsr @r0
    nop
    mov.l @r15+,r8
    lds.l @r15+,pr
    rts
    nop
    .align 2
.Lset_fpscr: .long ___set_fpscr
.Lruntime_initialize: .long _shenmue_dc_runtime_initialize
.Lmain: .long _shenmue_dc_entry
    .size _shenmue_dc_runtime_entry,.-_shenmue_dc_runtime_entry
