#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

    .section .text.dc_mt7_packed_lights,"ax"
    .balign 32
    .global _shenmue_dc_mt7_select_packed_lights
    .type _shenmue_dc_mt7_select_packed_lights,@function
_shenmue_dc_mt7_select_packed_lights:
    mov.l r12,@-r15
    mov.l r6,@-r15
    mov r4,r1
    add #-0x40,r1
    mov.l .Lmt7_packed_lights_block_0010,r0
    fmov.s @r1+,fr4
    fmov.s @r1+,fr5
    mov #0x1,r12
    fmov.s @r1+,fr6
    fmov.s @r1,fr7
    mov #0x0,r1
    mov.l .Lmt7_packed_lights_block_0008,r13
    mov.l r1,@r0
    mov #0x0,r9
    mov.l @r13,r13
    mov.l .Lmt7_packed_lights_block_0009,r14
    shlr r13
    mov.w .Lmt7_packed_lights_block_0007,r2
.Lmt7_packed_lights_block_0001:
    bf .Lmt7_packed_lights_block_0005
    mov.w @(0x2,r14),r0
    mov r14,r6
    add #0x74,r6
    cmp/eq #0x2,r0
    mov.w .Lmt7_packed_lights_block_0006,r3
    bt .Lmt7_packed_lights_block_0004
    fmov.s @r6+,fr0
    cmp/eq #0x3,r0
    bt/s .Lmt7_packed_lights_block_0002
    fsub fr4,fr0
    cmp/eq #0x4,r0
.Lmt7_packed_lights_block_0002:
    fmov.s @r6+,fr1
    bf/s .Lmt7_packed_lights_block_0005
    fmov fr0,fr9
    fmul fr9,fr9
    fmov.s @r6+,fr2
    add r14,r3
    fsub fr5,fr1
    pref @r3
    fsub fr6,fr2
    fldi0 fr3
    fmov.s @r3,fr8
    fadd fr7,fr8
    fipr fv0,fv0
    cmp/eq #0x3,r0
    fmul fr8,fr8
    bt/s .Lmt7_packed_lights_block_0003
    fcmp/gt fr8,fr3
    bt/s .Lmt7_packed_lights_block_0005
    fldi0 fr15
    fcmp/gt fr3,fr7
    mov #-0x80,r0
    fmov fr3,fr9
    extu.b r0,r0
    fsrra fr9
    add r14,r0
    bt .Lmt7_packed_lights_block_0004
    fmov.s @r0+,fr12
    fmov.s @r0+,fr13
    fldi0 fr3
    fmov.s @r0+,fr14
    fipr fv0,fv12
    fneg fr15
    fcmp/gt fr15,fr3
    bt .Lmt7_packed_lights_block_0005
    add #-0x34,r0
    fmul fr9,fr15
    fmov.s @r0,fr12
    add #0x4c,r0
    fcmp/gt fr15,fr12
    fmov.s @r0+,fr13
    bf .Lmt7_packed_lights_block_0004
    fmul fr7,fr9
    fmul fr9,fr13
    fldi1 fr14
    fmul fr9,fr9
    fsub fr9,fr14
    fsqrt fr14
    fmul fr14,fr12
    fsub fr13,fr12
    fcmp/gt fr15,fr12
.Lmt7_packed_lights_block_0003:
    bt .Lmt7_packed_lights_block_0005
.Lmt7_packed_lights_block_0004:
    or r12,r9
.Lmt7_packed_lights_block_0005:
    shll r12
    tst r13,r13
    add r2,r14
    bf/s .Lmt7_packed_lights_block_0001
    shlr r13
    mov.l .Lmt7_packed_lights_block_0010,r0
    mov.l r9,@r0
    mov.l @r15+,r6
    mov.l @r15+,r12
    rts
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packed_lights_block_0006:
    .word 0x00a0
.Lmt7_packed_lights_block_0007:
    .word 0x00b0
.Lmt7_packed_lights_block_0008:
    .long __Q37shenmue2dc3sdk.g_enabledLightMask
.Lmt7_packed_lights_block_0009:
    .long __Q37shenmue2dc3sdk.g_lightRecords
.Lmt7_packed_lights_block_0010:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x290
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009

#endif
