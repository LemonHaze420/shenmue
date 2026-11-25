#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

    .section .text.dc_mt7_packets,"ax"
    .balign 32
    .global _shenmue_dc_mt7_single_packets
    .type _shenmue_dc_mt7_single_packets,@function
    .global _shenmue_dc_mt7_single_packets_alpha
    .type _shenmue_dc_mt7_single_packets_alpha,@function
    .global _shenmue_dc_mt7_lit_packets
    .type _shenmue_dc_mt7_lit_packets,@function
    .global _shenmue_dc_mt7_lit_packets_alpha
    .type _shenmue_dc_mt7_lit_packets_alpha,@function
    .global _shenmue_dc_mt7_alternate_packets
    .type _shenmue_dc_mt7_alternate_packets,@function
    .global _shenmue_dc_mt7_alternate_packets_alpha
    .type _shenmue_dc_mt7_alternate_packets_alpha,@function
_shenmue_dc_mt7_single_packets:
    fldi1 fr4
_shenmue_dc_mt7_single_packets_alpha:
    add #0x18,r4
    mov.l .Lmt7_packets_block_0029,r7
    fldi1 fr1
    fmov.s fr15,@-r15
    fmov.s @r7,fr0
    fmov.s fr14,@-r15
    fmov.s fr13,@-r15
    fdiv fr0,fr1
    fmov.s fr12,@-r15
    mov.l r14,@-r15
    mov.l r13,@-r15
    mov.l r12,@-r15
    mov.l r11,@-r15
    mov.l r10,@-r15
    mov.l r9,@-r15
    mov.l r8,@-r15
    mov #0x0,r0
    sts.l PR,@-r15
    add #-0x48,r15
    fldi1 fr7
    fmov fr4,@(r0,r15)
    mov #0x4,r0
    fabs fr0
    fmov fr0,@(r0,r15)
    mov.l .Lmt7_packets_block_0033,r0
    fmov.s fr7,@r7
    mov.l @r0,r1
    mov.l .Lmt7_packets_block_0035,r2
    mov.l r1,@(0x8,r15)
    mov.l @r2,r1
    mov.l .Lmt7_packets_block_0036,r0
    mov.l .Lmt7_packets_block_0034,r2
    mov.l r1,@(0x10,r15)
    mov.l @r2,r3
    mov.l @r0,r1
    mov.l r3,@(0xc,r15)
    fmov.s @r5+,fr10
    mov.l r1,@(0x14,r15)
    fmov.s @r5+,fr8
    fmov.s @r5+,fr9
    mov.l @r5+,r12
    mov #0x38,r0
    fmov fr10,@(r0,r15)
    mov #0x30,r0
    fmov fr8,@(r0,r15)
    mov #0x34,r0
    fmov fr9,@(r0,r15)
    mov.l .Lmt7_packets_block_0032,r0
    mov.l @r0,r0
    cmp/eq #0x0,r0
    bt .Lmt7_packets_block_0003
    mov.l .Lmt7_packets_block_0002,r1
    jmp @r1
    mov #0x48,r0
    .word 0x0009
.Lmt7_packets_block_0002:
    .long .Lmt7_packets_block_0176
.Lmt7_packets_block_0003:
    fmov.s @r5+,fr14
    fmov.s @r5+,fr12
    fmov.s @r5+,fr13
    fmul fr1,fr12
    mov.l @r5+,r0
    fmul fr1,fr13
    mov.l r0,@(0x24,r15)
    mov.l @r5+,r7
    fmul fr1,fr14
    mov.l r7,@(0x3c,r15)
    mov.l .Lmt7_packets_block_0030,r0
    mov.l @r0+,r1
    mov.l @r0+,r2
    mov.l @r0+,r3
    mov.l r1,@(0x18,r15)
    mov.l r2,@(0x1c,r15)
    mov.l r3,@(0x20,r15)
    mov.l .Lmt7_packets_block_0042,r0
    mov.l @r0+,r5
    mov.l r5,@(0x28,r15)
    mov.l @r0+,r6
    mov.l r6,@(0x2c,r15)
    mov.l .Lmt7_packets_block_0027,r10
    mov r15,r3
    mov.l @r4+,r8
    add #0x4,r3
    mov.l @r10,r10
    mov #-0x1,r11
    fmov fr4,fr11
    or r5,r8
    fcmp/gt fr11,fr7
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    or r6,r1
    bf/s .Lmt7_packets_block_0006
    fmov.s @r4+,fr3
    bt .Lmt7_packets_block_0005
    .word 0x0009
.Lmt7_packets_block_0004:
    mov.l @(0x28,r15),r3
    mov.l @(0x2c,r15),r0
    or r3,r8
    mov r15,r3
    fmov.s @r3+,fr11
    or r0,r1
    fldi1 fr7
    fcmp/gt fr11,fr7
    mov r4,r0
    add #0x30,r0
    bf/s .Lmt7_packets_block_0006
    pref @r0
.Lmt7_packets_block_0005:
    mov #0x2,r2
    mov r1,r0
    shll16 r2
    shll8 r2
    mov.l .Lmt7_packets_block_0041,r9
    or r2,r8
    or #0xc0,r0
    swap.w r0,r0
    mov.l @r9,r9
    or #0x10,r0
    swap.b r0,r0
    or #0xfc,r0
    xor r9,r0
    swap.b r0,r0
    swap.w r0,r0
    add r2,r2
    mov r0,r1
    not r2,r2
    and r2,r8
.Lmt7_packets_block_0006:
    mov r8,r0
    fmov.s @r4+,fr4
    shlr16 r0
    fmov.s @r4+,fr5
    shlr8 r0
    fmov.s @r4+,fr6
    and #0x7,r0
    mov #0x4,r2
    shll2 r0
    ftrv xmtrx,fv4
    fmov.s @r3+,fr1
    shll8 r2
    fmov.s @r4+,fr0
    cmp/eq r0,r11
    mov r0,r11
    mov #-0x20,r6
    fmul fr1,fr0
    shll16 r2
    bt/s .Lmt7_packets_block_0007
    mov.l @(r0,r10),r5
    mov #-0x1,r0
    shll16 r0
    shll8 r0
    or #0x38,r0
    mov r5,r9
    shlr16 r9
    shlr8 r9
    mov.l r9,@r0
    mov.l r9,@(0x4,r0)
.Lmt7_packets_block_0007:
    add #-0x1,r2
    lds r12,FPUL
    mov #-0x2,r0
    fadd fr0,fr7
    and r5,r2
    fsts FPUL,fr1
    shll8 r6
    fcmp/gt fr1,fr7
    shll16 r6
    or r2,r6
    fsub fr0,fr7
    and r0,r12
    mov.l .Lmt7_packets_block_0028,r9
    bf/s .Lmt7_packets_block_0011
    fsub fr0,fr7
    fcmp/gt fr1,fr7
    bf/s .Lmt7_packets_block_0011
    add #0x1,r12
.Lmt7_packets_block_0008:
    mov r4,r2
    add #0x2c,r4
    mov.l @r4+,r0
    mov.l r8,@r6
    mov r8,r1
    add r0,r4
    mov.l @r4+,r8
    tst r8,r8
    bf .Lmt7_packets_block_0009
    bra .Lmt7_packets_block_0024
    nop
    .word 0x0009
.Lmt7_packets_block_0009:
    mov r15,r9
    add #0x18,r9
    fmov.s @r9+,fr5
    fmov.s @r9+,fr6
    add #0xc,r2
    fmov.s @r9+,fr7
    mov r1,r0
    tst #0x10,r0
    bf/s .Lmt7_packets_block_0010
    tst #0x20,r0
    bt .Lmt7_packets_block_0010
    fmov.s @r2+,fr0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    fmov.s @r2+,fr3
    fmul fr11,fr0
    mov.l .Lmt7_packets_block_0031,r0
    fmov.s @r0+,fr11
    fmul fr5,fr1
    fmov.s @r2+,fr4
    fmov.s @r2+,fr5
    fmul fr11,fr5
    fmov.s @r0+,fr11
    fmul fr6,fr2
    fmov.s @r2+,fr6
    fmul fr11,fr6
    fmov.s @r0+,fr11
    fmul fr7,fr3
    fmov.s @r2+,fr7
    fschg
    fmul fr11,fr7
    mov #0x0,r0
    mov.l r0,@(0x4,r6)
    mov.l r0,@(0x8,r6)
    mov.l r0,@(0xc,r6)
    pref @r6
    add #0x40,r6
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    mov r11,r0
    fmov.s fr2,@-r6
    add #0x40,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    add #0x20,r6
    mov.l r5,@(r0,r10)
.Lmt7_packets_block_0010:
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0004
    fmov.s @r4+,fr3
    .word 0x0009
.Lmt7_packets_block_0011:
    fmov.s @r3+,fr1
    mov.l .Lmt7_packets_block_0038,r0
    add #0x10,r9
    fmov.s @r0,fr7
    fmul fr4,fr7
    fneg fr0
    fmul fr0,fr1
    fmov.s fr3,@-r9
    fsub fr7,fr5
    fmov.s @r3+,fr3
    mov r4,r2
    mov.l .Lmt7_packets_block_0037,r0
    fcmp/gt fr1,fr5
    fmov.s @r0,fr7
    fmul fr0,fr3
    bf/s .Lmt7_packets_block_0008
    fmul fr4,fr7
    mov.l r1,@-r9
    fsub fr7,fr6
    fmov.s @r3+,fr0
    fcmp/gt fr3,fr6
    fmov.s @r3+,fr7
    fmul fr4,fr0
    bf/s .Lmt7_packets_block_0008
    fsub fr1,fr0
    fcmp/gt fr0,fr5
    fmov.s @r3+,fr5
    fmul fr4,fr7
    bt/s .Lmt7_packets_block_0008
    fmov.s fr2,@-r9
    fsub fr3,fr7
    mov.l r8,@-r9
    mov #0x8,r1
    fcmp/gt fr7,fr6
    fmov.s @r3+,fr6
    bt .Lmt7_packets_block_0008
    mov r8,r0
    tst #0x10,r0
    mov.b @(0x4,r2),r0
    add #0x8,r2
    fmov.s @r3+,fr7
    mov r0,r8
    fmov.s @r2+,fr15
    bf/s .Lmt7_packets_block_0012
    add #0x30,r4
    fmov.s @r2+,fr0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    fmov.s @r2+,fr3
    fmul fr11,fr0
    mov.l .Lmt7_packets_block_0031,r0
    fmov.s @r0+,fr11
    fmul fr5,fr1
    fmov.s @r2+,fr4
    fmov.s @r2+,fr5
    fmul fr11,fr5
    fmov.s @r0+,fr11
    fmul fr6,fr2
    fmov.s @r2+,fr6
    fmul fr11,fr6
    fmov.s @r0+,fr11
    fmul fr7,fr3
    fmov.s @r2+,fr7
    add #0x40,r6
    fmul fr11,fr7
    fschg
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    fschg
    add #-0x20,r6
.Lmt7_packets_block_0012:
    cmp/pz r8
    bt .Lmt7_packets_block_0015
    mov r8,r0
    cmp/eq #-0x1,r0
    bt/s .Lmt7_packets_block_0014
    cmp/eq #-0x2,r0
    bf .Lmt7_packets_block_0013
    mov.l .Lmt7_packets_block_0039,r0
    jsr @r0
    mov.l @(0x3c,r15),r7
    bt .Lmt7_packets_block_0024
    .word 0xf249
    .word 0x6146
    .word 0xaf1b
    .word 0xf349
    .word 0x0009
.Lmt7_packets_block_0013:
    mov.l @(0x3c,r15),r7
    cmp/eq #-0x3,r0
    mov #0x24,r0
    mov.l .Lmt7_packets_block_0040,r1
    bf .Lmt7_packets_block_0014
    fmov.s @(r0,r15),fr1
    jsr @r1
    fmul fr1,fr15
    bt .Lmt7_packets_block_0024
    .word 0xf249
    .word 0x6146
    .word 0xaf0d
    .word 0xf349
    .word 0x0009
.Lmt7_packets_block_0014:
    bra .Lmt7_packets_block_0017
    fldi1 fr15
.Lmt7_packets_block_0015:
    cmp/pl r8
    mov #0x38,r0
    bf .Lmt7_packets_block_0016
    fmov.s @(r0,r15),fr10
    mov #0x30,r0
    fmov.s @(r0,r15),fr8
    mov #0x34,r0
    fmov.s @(r0,r15),fr9
    mov #0x9,r0
    shld r0,r8
    mov.l .Lmt7_packets_block_0043,r0
    add r0,r8
.Lmt7_packets_block_0016:
    mov #0x24,r0
    fmov.s @(r0,r15),fr1
    fmul fr1,fr15
.Lmt7_packets_block_0017:
    mov.l @r4+,r0
.Lmt7_packets_block_0018:
    cmp/pl r0
    mov.l .Lmt7_packets_block_0028,r9
    bt/s .Lmt7_packets_block_0019
    cmp/pz r0
    mov r0,r8
    mov r11,r0
    mov.l r5,@(r0,r10)
    bt .Lmt7_packets_block_0024
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0004
    fmov.s @r4+,fr3
.Lmt7_packets_block_0019:
    mov r0,r1
    mov #-0x5,r2
    mov.l @r9+,r3
    and #0x40,r0
    shld r2,r0
    mov #-0x3,r2
    and r2,r3
    or r3,r0
    tst #0x10,r0
    add #0x20,r4
    bf/s .Lmt7_packets_block_0020
    lds r0,FPUL
    or #0x10,r0
    mov r9,r2
    mov.l r0,@-r2
.Lmt7_packets_block_0020:
    mov.l @(0x28,r15),r2
    movt r3
    mov r1,r0
    cmp/pz r2
    mov #0x1b,r2
    bt/s .Lmt7_packets_block_0021
    and #0x3,r0
    xor #0x1,r0
.Lmt7_packets_block_0021:
    shld r2,r0
    cmp/pl r3
    mov #0x3,r3
    fschg
    shld r2,r3
    pref @r4
    not r3,r3
    mov.l @r9+,r2
    and r3,r2
    fsts FPUL,fr0
    or r0,r2
    lds r2,FPUL
    add #-0x20,r4
    fsts FPUL,fr1
    add #0x10,r6
    fmov.s @r9+,fr2
    fmov.s fr2,@-r6
    add #0x20,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    bf/s .Lmt7_packets_block_0022
    add #0x20,r6
    pref @r6
    add #0x20,r6
    add #0x20,r5
.Lmt7_packets_block_0022:
    rotcr r12
    mov r1,r0
    bt/s .Lmt7_packets_block_0025
    shll r12
    tst #0x8,r0
    bf .Lmt7_packets_block_0023
    bsr .Lmt7_packets_block_0044
    nop
    bra .Lmt7_packets_block_0018
    mov.l @r4+,r0
    .word 0x0009
    .word 0x0009
    .word 0xafb5
    .word 0x6046
    .word 0x0009
.Lmt7_packets_block_0023:
    bsr .Lmt7_packets_block_0139
    nop
    bra .Lmt7_packets_block_0018
    mov.l @r4+,r0
.Lmt7_packets_block_0024:
    add #0x48,r15
    lds.l @r15+,PR
    mov.l @r15+,r8
    mov.l @r15+,r9
    mov.l @r15+,r10
    mov.l @r15+,r11
    mov.l @r15+,r12
    mov.l @r15+,r13
    mov.l @r15+,r14
    fmov.s @r15+,fr12
    fmov.s @r15+,fr13
    fmov.s @r15+,fr14
    fmov.s @r15+,fr15
    rts
    nop
    .word 0x0009
.Lmt7_packets_block_0025:
    mov #0x0,r1
    tst #0x40,r0
    addc r1,r12
    mov.l @(0x3c,r15),r7
    tst #0x10,r0
    bt/s .Lmt7_packets_block_0026
    tst #0x8,r0
    bsr .Lmt7_packets_block_0081
    tst #0x20,r0
    bra .Lmt7_packets_block_0018
    mov.l @r4+,r0
    .word 0x0009
.Lmt7_packets_block_0026:
    bsr .Lmt7_packets_block_0123
    tst #0x20,r0
    bra .Lmt7_packets_block_0018
    mov.l @r4+,r0
.Lmt7_packets_block_0027:
    .long __nj_vertex_buffer_
.Lmt7_packets_block_0028:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch
.Lmt7_packets_block_0029:
    .long __Q37shenmue2dc3sdk.g_modelScale
.Lmt7_packets_block_0030:
    .long __Q37shenmue2dc3sdk.g_mt7DiffuseChannels
.Lmt7_packets_block_0031:
    .long __Q37shenmue2dc3sdk.g_mt7SpecularChannels
.Lmt7_packets_block_0032:
    .long __Q37shenmue2dc3sdk.g_mt7ReflectedDirection
.Lmt7_packets_block_0033:
    .long __Q37shenmue2dc3sdk.g_mt7HorizontalClipScale
.Lmt7_packets_block_0034:
    .long __Q37shenmue2dc3sdk.g_mt7VerticalClipScale
.Lmt7_packets_block_0035:
    .long __Q27shenmue12matrix_stack.g_projectionViewportWidth
.Lmt7_packets_block_0036:
    .long __Q27shenmue12matrix_stack.g_projectionViewportHeight
.Lmt7_packets_block_0037:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginY
.Lmt7_packets_block_0038:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginX
.Lmt7_packets_block_0039:
    .long .Lmt7_packets_block_0352
.Lmt7_packets_block_0040:
    .long .Lmt7_packets_block_0423
.Lmt7_packets_block_0041:
    .long __Q37shenmue2dc3sdk.g_mt7AlphaBlendMask
.Lmt7_packets_block_0042:
    .long _shenmue_dc_mt7_draw_controls
.Lmt7_packets_block_0043:
    .long __Q37shenmue2dc3sdk.g_mt7SpecularLookup-0x4d4
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0044:
    tst r8,r8
    mov r8,r0
    bf .Lmt7_packets_block_0045
    bra .Lmt7_packets_block_0056
.Lmt7_packets_block_0045:
    tst #0x1,r0
    bf .Lmt7_packets_block_0046
    bra .Lmt7_packets_block_0069
    nop
.Lmt7_packets_block_0046:
    mov.l @r4+,r3
    fldi1 fr2
    fschg
    fldi0 fr3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0047
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0047:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0048:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0053
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0049
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0049:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0051
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0050
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0050:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0048
    add #0x38,r6
    .word 0x0009
.Lmt7_packets_block_0051:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0052
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0052
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0050
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0050
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0052:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0055
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0053:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0054
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0054
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0049
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0049
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0054:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0055:
    fmul fr7,fr6
    mov r6,r2
    shar r2
    fmul fr7,fr5
    fmov.s fr0,@-r6
    add #-0x8,r4
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0056:
    mov.l @r4+,r3
    add #0x20,r6
    fschg
    mov.l @r4,r0
    mov r4,r14
    fldi0 fr2
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0057
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0057:
    fmov.s @r14+,fr4
    mov r6,r2
    fmov.s @r14+,fr6
    fadd fr7,fr2
    fldi1 fr7
    fldi0 fr3
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0058:
    mov.l @r4+,r0
    dt r3
    flds fr15,FPUL
    fldi1 fr7
    fdiv fr4,fr7
    mov.l @r4+,r1
    bt/s .Lmt7_packets_block_0065
    fipr fv12,fv0
    tst #0x1,r0
    add r4,r1
    bt/s .Lmt7_packets_block_0059
    fsts FPUL,fr2
    mov r4,r1
    add #0x18,r4
    add #-0x8,r1
.Lmt7_packets_block_0059:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r14,fr0
    add #0x20,r4
    fmov.s @r1+,fr8
    bf .Lmt7_packets_block_0060
    fadd fr3,fr2
.Lmt7_packets_block_0060:
    fldi0 fr3
    dt r3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r1+,fr10
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fadd fr11,fr2
    fldi1 fr11
    fmul fr7,fr5
    fmov.s @r1+,fr0
    add #0x20,r5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    add #0x40,r6
    mov.l @r4+,r0
    mov.l @r4+,r14
    flds fr15,FPUL
    fipr fv12,fv0
    fldi1 fr11
    bt/s .Lmt7_packets_block_0063
    fdiv fr8,fr11
    tst #0x1,r0
    add r4,r14
    bt/s .Lmt7_packets_block_0061
    fsts FPUL,fr2
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0061:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr0
    add #0x20,r4
    fmov.s @r14+,fr4
    bf .Lmt7_packets_block_0062
    fadd fr3,fr2
.Lmt7_packets_block_0062:
    fldi0 fr3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r14+,fr6
    fmul fr11,fr10
    fmov.s fr0,@-r6
    fadd fr7,fr2
    fldi1 fr7
    fmul fr11,fr9
    fmov.s @r14+,fr0
    add #0x20,r5
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    bra .Lmt7_packets_block_0058
    add #0x40,r6
.Lmt7_packets_block_0063:
    cmp/pl r0
    fsts FPUL,fr2
    mov r6,r2
    bf/s .Lmt7_packets_block_0064
    shar r2
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0064
    mov.l @r4+,r0
    tst #0x1,r0
    mov.l @r4+,r14
    bt/s .Lmt7_packets_block_0061
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0061
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0064:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    bra .Lmt7_packets_block_0067
    fmov fr10,fr6
.Lmt7_packets_block_0065:
    cmp/pl r0
    fsts FPUL,fr2
    mov r6,r2
    bf/s .Lmt7_packets_block_0066
    shar r2
    tst #0x80,r0
    mov r1,r3
    bt .Lmt7_packets_block_0066
    mov.l @r4+,r0
    tst #0x1,r0
    mov.l @r4+,r1
    bt/s .Lmt7_packets_block_0059
    add r4,r1
    mov r4,r1
    add #0x18,r4
    bra .Lmt7_packets_block_0059
    add #-0x8,r1
    .word 0x0009
.Lmt7_packets_block_0066:
    add #0x30,r4
    mov r14,r1
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0067:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr10
    bf .Lmt7_packets_block_0068
    fadd fr3,fr2
.Lmt7_packets_block_0068:
    fldi0 fr3
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr10,@-r6
    add #-0x8,r4
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0069:
    mov #0x7f,r2
    mov.l @r4+,r3
    add #-0x20,r6
    mov.l @r4,r0
    add #0x7f,r2
    fschg
    add #0x37,r2
    tst #0x1,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0070
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0070:
    fmov.s @r14+,fr4
.Lmt7_packets_block_0071:
    fmov.s @r14+,fr6
    mov r6,r7
    fldi0 fr2
    fadd fr7,fr2
    fldi1 fr7
    add #0x40,r6
    fldi0 fr3
    add #0x20,r5
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
    fldi0 fr11
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0080
    mov r14,r1
    fipr fv12,fv0
    tst #0x1,r0
    mov.l @r4+,r14
    bf/s .Lmt7_packets_block_0074
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
.Lmt7_packets_block_0072:
    fcmp/gt fr11,fr3
    add r4,r14
    flds fr15,FPUL
    fipr fv0,fv8
    pref @r14
    bf/s .Lmt7_packets_block_0077
    fsts FPUL,fr2
    fadd fr3,fr2
    fldi0 fr3
    bra .Lmt7_packets_block_0076
    ftrc fr11,FPUL
    .word 0x0009
.Lmt7_packets_block_0073:
    add #-0x1c,r4
    fldi1 fr7
.Lmt7_packets_block_0074:
    fdiv fr4,fr7
.Lmt7_packets_block_0075:
    add #0x18,r4
    fcmp/gt fr11,fr3
    flds fr15,FPUL
    fipr fv0,fv8
    mov r4,r14
    add #-0x20,r14
    pref @r4
    bf/s .Lmt7_packets_block_0077
    fsts FPUL,fr2
    fadd fr3,fr2
    ftrc fr11,FPUL
    fldi0 fr3
.Lmt7_packets_block_0076:
    sts FPUL,r0
    cmp/gt r2,r0
    shll2 r0
    bf .Lmt7_packets_block_0078
    fschg
    fmov.s @(r0,r8),fr3
    bra .Lmt7_packets_block_0078
    fschg
    .word 0x0009
.Lmt7_packets_block_0077:
    fldi0 fr3
.Lmt7_packets_block_0078:
    fmov.s @r1,fr0
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr0,@-r6
    tst r3,r3
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0079
    mov.l r7,@r6
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0071
.Lmt7_packets_block_0079:
    pref @r6
    add #0x20,r6
    rts
    fschg
    .word 0x0009
.Lmt7_packets_block_0080:
    mov r6,r7
    shar r7
    cmp/pl r0
    bf/s .Lmt7_packets_block_0073
    fipr fv12,fv0
    tst #0x80,r0
    fldi1 fr7
    bt .Lmt7_packets_block_0073
    mov.l @r4+,r3
    mov.l @r4+,r0
    mov.l @r4+,r14
    tst #0x1,r0
    fdiv fr4,fr7
    bf .Lmt7_packets_block_0075
    bra .Lmt7_packets_block_0072
    pref @r4
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0081:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0082:
    mov #0x0,r11
    bsr .Lmt7_packets_block_0107
    mov #0x0,r9
    bt/s .Lmt7_packets_block_0084
    rotcl r9
    bsr .Lmt7_packets_block_0113
    add #0x1,r11
    bsr .Lmt7_packets_block_0107
    nop
    bt/s .Lmt7_packets_block_0090
    rotcl r9
    bsr .Lmt7_packets_block_0113
    add #0x1,r11
.Lmt7_packets_block_0083:
    bsr .Lmt7_packets_block_0107
    nop
    bt/s .Lmt7_packets_block_0100
    rotcl r9
    bsr .Lmt7_packets_block_0113
    add #0x1,r11
    bf .Lmt7_packets_block_0083
    bt .Lmt7_packets_block_0088
.Lmt7_packets_block_0084:
    dt r3
    add #0x1,r11
    bsr .Lmt7_packets_block_0107
    dt r3
    bf/s .Lmt7_packets_block_0094
    rotcl r9
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf/s .Lmt7_packets_block_0085
    fldi1 fr7
    add #-0x18,r4
    mov.l @(0x4,r14),r14
    add r4,r14
.Lmt7_packets_block_0085:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr5
.Lmt7_packets_block_0086:
    fmov.s @r14+,fr6
    mov #0x20,r1
    mov.l @r4,r0
    add #0x20,r4
    pref @r4
    ftrv xmtrx,fv4
    tst #0x1,r0
    add #-0x20,r4
    bf/s .Lmt7_packets_block_0087
    mov r4,r14
    mov #0x8,r1
    mov.l @(0x4,r14),r14
    add #0x8,r14
    add r4,r14
.Lmt7_packets_block_0087:
    flds fr7,FPUL
    sts FPUL,r0
    cmp/hi r0,r12
    add #0x1,r11
    bf/s .Lmt7_packets_block_0098
    rotcl r9
    dt r3
    fldi1 fr7
    bt .Lmt7_packets_block_0088
    add r1,r4
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0086
    fmov.s @r14+,fr5
.Lmt7_packets_block_0088:
    mov.l @r4+,r0
    cmp/pl r0
    bf/s .Lmt7_packets_block_0089
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0082
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0089:
    lds.l @r15+,PR
    mov #0x1,r0
    mov.l @r15+,r11
    add #-0x4,r4
    mov.l @r15+,r10
    rts
    or r0,r12
.Lmt7_packets_block_0090:
    bsr .Lmt7_packets_block_0115
    dt r3
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0091
    add #-0x18,r4
.Lmt7_packets_block_0091:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0092
    add #-0x18,r4
.Lmt7_packets_block_0092:
    bsr .Lmt7_packets_block_0107
    nop
    bsr .Lmt7_packets_block_0115
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0119
    mov #0x0,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0093
    add #0x18,r4
.Lmt7_packets_block_0093:
    bra .Lmt7_packets_block_0105
    nop
.Lmt7_packets_block_0094:
    bsr .Lmt7_packets_block_0115
    mov #0x1,r11
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0095
    add #-0x18,r4
.Lmt7_packets_block_0095:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0096
    add #-0x18,r4
.Lmt7_packets_block_0096:
    bsr .Lmt7_packets_block_0107
    nop
    bsr .Lmt7_packets_block_0115
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0119
    mov #0x0,r0
    clrt
    bsr .Lmt7_packets_block_0117
    mov #0x1,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0097
    add #0x18,r4
.Lmt7_packets_block_0097:
    bra .Lmt7_packets_block_0105
    nop
.Lmt7_packets_block_0098:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0099
    add #-0x18,r4
.Lmt7_packets_block_0099:
    bsr .Lmt7_packets_block_0107
    nop
.Lmt7_packets_block_0100:
    bsr .Lmt7_packets_block_0115
    nop
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0101
    add #-0x18,r4
.Lmt7_packets_block_0101:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0102
    add #-0x18,r4
.Lmt7_packets_block_0102:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0103
    add #-0x18,r4
.Lmt7_packets_block_0103:
    bsr .Lmt7_packets_block_0107
    nop
    bsr .Lmt7_packets_block_0115
    add #-0x2,r11
    bsr .Lmt7_packets_block_0107
    nop
    bsr .Lmt7_packets_block_0115
    add #0x1,r11
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0104
    add #0x18,r4
.Lmt7_packets_block_0104:
    add #0x1,r11
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0106
    mov r9,r0
    bsr .Lmt7_packets_block_0116
    nop
    bra .Lmt7_packets_block_0106
    mov r9,r0
.Lmt7_packets_block_0105:
    bsr .Lmt7_packets_block_0107
    add #0x1,r11
    bsr .Lmt7_packets_block_0115
    rotcl r9
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0106
    mov r9,r0
    bsr .Lmt7_packets_block_0116
    nop
    mov r9,r0
.Lmt7_packets_block_0106:
    and #0x7,r0
    shll2 r0
    braf r0
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa00c
    .word 0x0009
    .word 0xa019
    .word 0x0009
    .word 0xa024
    .word 0x0009
    .word 0xa02a
    .word 0x0009
    .word 0xa038
    .word 0x0009
    .word 0xa03d
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb123
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0ff
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb11b
    .word 0x70ff
    .word 0x8bd3
    .word 0xaf5d
    .word 0x0009
    .word 0x0008
    .word 0xb0f5
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb111
    .word 0x70ff
    .word 0x4310
    .word 0xb0ee
    .word 0x60b3
    .word 0x8bc6
    .word 0xaf50
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb107
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf22
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb0ff
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0db
    .word 0x70ff
    .word 0x4310
    .word 0xb0d8
    .word 0x60b3
    .word 0x8901
    .word 0xaf06
    .word 0x7b01
    .word 0xaf38
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb0ef
    .word 0x70ff
    .word 0x8ba7
    .word 0xaf31
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x890d
    .word 0x60b3
    .word 0x0008
    .word 0xb0e5
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb0e1
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0dd
    .word 0x70ff
    .word 0xa008
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb0d7
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0d3
    .word 0x70ff
    .word 0x4310
    .word 0xb0b0
    .word 0x60b3
    .word 0x8b88
    .word 0xaf12
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0107:
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0108
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    fldi1 fr7
    fmov.s @r0+,fr2
    fmov.s @r0+,fr0
    ftrv xmtrx,fv4
    fmov.s @r0+,fr1
    fldi0 fr3
    add #0x20,r4
    bra .Lmt7_packets_block_0109
    mov r0,r2
.Lmt7_packets_block_0108:
    fmov.s @r4+,fr5
    fmov.s @r4+,fr6
    fldi1 fr7
    fmov.s @r4+,fr2
    fmov.s @r4+,fr0
    ftrv xmtrx,fv4
    fmov.s @r4+,fr1
    fldi0 fr3
    mov r4,r2
    add #0x28,r4
.Lmt7_packets_block_0109:
    fldi0 fr11
    fipr fv12,fv0
    pref @r4
    add #-0x20,r4
    flds fr7,FPUL
    sts FPUL,r0
    fldi1 fr7
    fdiv fr4,fr7
    fsts FPUL,fr4
    fcmp/gt fr11,fr3
    fipr fv0,fv8
    fmov fr15,fr2
    bf .Lmt7_packets_block_0111
    fadd fr3,fr2
    tst r8,r8
    fldi0 fr3
    bt/s .Lmt7_packets_block_0112
    mov #0x1,r1
    tst r1,r8
    bf/s .Lmt7_packets_block_0110
    ftrc fr11,FPUL
    mov #0x7f,r1
    add #0x7f,r1
    sts FPUL,r0
    add #0x37,r1
    cmp/gt r1,r0
    shll2 r0
    bf .Lmt7_packets_block_0111
    mov.l @(r0,r8),r0
    lds r0,FPUL
    fsts FPUL,fr3
    flds fr4,FPUL
    sts FPUL,r0
    rts
    cmp/hi r0,r12
.Lmt7_packets_block_0110:
    fldi1 fr2
    flds fr4,FPUL
    sts FPUL,r0
    rts
    cmp/hi r0,r12
.Lmt7_packets_block_0111:
    flds fr4,FPUL
    sts FPUL,r0
.Lmt7_packets_block_0112:
    fldi0 fr3
    rts
    cmp/hi r0,r12
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0113:
    fschg
    add #0x20,r6
    fmov.s @r2,fr0
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr0,@-r6
    mov r6,r2
    fmov.s fr6,@-r6
    dt r3
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0114
    fschg
    mov.l r6,@r6
    add #0x20,r5
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0114:
    shar r2
    mov.l r2,@r6
    add #0x20,r5
    sett
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0115:
    mov #0x5,r10
    mov r11,r0
    and #0x3,r0
    fschg
    shld r10,r0
    mov.l .Lmt7_packets_block_0122,r10
    fmov.s @r2,fr0
    add r0,r10
    fmov.s fr2,@-r10
    fmov.s fr0,@-r10
    fmov.s fr6,@-r10
    fmov.s fr4,@-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0116:
    mov #0x5,r1
    mov.l .Lmt7_packets_block_0122,r10
    mov r11,r0
    and #0x3,r0
    mov r10,r2
    shld r1,r0
    fschg
    add r0,r10
    add #-0x8,r10
    mov r11,r0
    fmov.s @r10,fr2
    add #-0x1,r0
    mov r2,r10
    and #0x3,r0
    shld r1,r0
    add r0,r2
    mov r11,r0
    fmov.s fr2,@-r2
    add #-0x2,r0
    and #0x3,r0
    shld r1,r0
    add r0,r10
    fmov.s fr2,@-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0117:
    and #0x3,r0
    mov #0x5,r2
    mov.l .Lmt7_packets_block_0121,r10
    shld r2,r0
    fschg
    add r0,r10
    fmov.s @r10+,fr0
    fmov.s @r10+,fr2
    add #0x20,r6
    fmov.s @r10+,fr4
    fmul fr3,fr2
    fmov.s @r10+,fr6
    fmul fr3,fr1
    fmov.s fr6,@-r6
    mov r6,r2
    fmov.s fr4,@-r6
    add #0x20,r5
    fmov.s fr2,@-r6
    bf/s .Lmt7_packets_block_0118
    fmov.s fr0,@-r6
    shar r2
    sett
.Lmt7_packets_block_0118:
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0119:
    and #0x3,r0
    shll2 r0
    mov.l .Lmt7_packets_block_0121,r10
    shll2 r0
    add r0,r0
    mov r10,r2
    add r0,r10
    mov r11,r0
    and #0x3,r0
    fmov.s @r10,fr2
    shll2 r0
    shll2 r0
    add r0,r0
    add r0,r2
    fmov.s @r2,fr1
    lds r12,FPUL
    fsts FPUL,fr0
    fsub fr2,fr0
    add #0x18,r10
    fsub fr2,fr1
    add #0x18,r2
    fdiv fr1,fr0
    lds r7,FPUL
    fsts FPUL,fr1
    fschg
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    add #0x20,r6
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fschg
    fmov.s fr1,@-r6
    fmov.s @r2,fr5
    add #-0x4,r2
    fmov.s @r10,fr3
    add #-0x4,r10
    fmov.s @r2,fr4
    fmov.s @r10,fr2
    fsub fr3,fr5
    fsub fr2,fr4
    mov #-0x10,r2
    fmac fr0,fr5,fr3
    shll16 r2
    fmac fr0,fr4,fr2
    shll8 r2
    fmul fr1,fr3
    add #0x20,r5
    fmul fr1,fr2
    fmov.s fr3,@-r6
    fmov.s fr2,@-r6
    bt .Lmt7_packets_block_0120
    mov r6,r2
.Lmt7_packets_block_0120:
    mov.l r2,@-r6
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0121:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch+0x40
.Lmt7_packets_block_0122:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch+0x60
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0123:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0124:
    bsr .Lmt7_packets_block_0132
    mov #0x0,r9
    mov r9,r0
    cmp/eq #0x0,r0
    bt .Lmt7_packets_block_0127
    cmp/eq #0x7,r0
    bt .Lmt7_packets_block_0128
    bsr .Lmt7_packets_block_0107
    mov #0x0,r11
    bsr .Lmt7_packets_block_0115
    nop
    bsr .Lmt7_packets_block_0107
    mov #0x1,r11
    bsr .Lmt7_packets_block_0115
    nop
    bsr .Lmt7_packets_block_0107
    mov #0x2,r11
    bsr .Lmt7_packets_block_0115
    nop
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0125
    shll2 r9
    bsr .Lmt7_packets_block_0116
    nop
.Lmt7_packets_block_0125:
    braf r9
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa034
    .word 0x0009
    .word 0xa041
    .word 0x0009
    .word 0xa04f
    .word 0x0009
    .word 0xa05a
    .word 0x0009
    .word 0xa068
    .word 0x0009
    .word 0xa073
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0126:
    dt r3
    bf .Lmt7_packets_block_0124
    mov #0x1,r0
    lds.l @r15+,PR
    or r0,r12
    mov.l @r15+,r11
    rts
    mov.l @r15+,r10
.Lmt7_packets_block_0127:
    mov r3,r9
    bsr .Lmt7_packets_block_0107
    mov #0x3,r3
    bsr .Lmt7_packets_block_0113
    nop
    bsr .Lmt7_packets_block_0107
    nop
    bsr .Lmt7_packets_block_0113
    nop
    bsr .Lmt7_packets_block_0107
    nop
    bsr .Lmt7_packets_block_0113
    nop
    bra .Lmt7_packets_block_0126
    mov r9,r3
.Lmt7_packets_block_0128:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0129
    add #0x18,r4
.Lmt7_packets_block_0129:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0130
    add #0x18,r4
.Lmt7_packets_block_0130:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0131
    add #0x18,r4
.Lmt7_packets_block_0131:
    bra .Lmt7_packets_block_0126
    nop
    .word 0x0008
    .word 0xbf36
    .word 0xe000
    .word 0x0008
    .word 0xbf33
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf4f
    .word 0xe000
    .word 0x0018
    .word 0xbf4c
    .word 0xe001
    .word 0xafc9
    .word 0x0009
    .word 0x0008
    .word 0xbf27
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf43
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbf1f
    .word 0xe002
    .word 0x0018
    .word 0xbf3c
    .word 0xe001
    .word 0xafb9
    .word 0x0009
    .word 0x0008
    .word 0xbf17
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf33
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbf2f
    .word 0xe000
    .word 0xafac
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf29
    .word 0xe000
    .word 0x0008
    .word 0xbf06
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf22
    .word 0xe000
    .word 0x0018
    .word 0xbeff
    .word 0xe002
    .word 0xaf9c
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf19
    .word 0xe000
    .word 0x0008
    .word 0xbef6
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbf12
    .word 0xe001
    .word 0xaf8f
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbf0c
    .word 0xe000
    .word 0x0008
    .word 0xbf09
    .word 0xe001
    .word 0x0018
    .word 0xbee6
    .word 0xe002
    .word 0xaf83
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0132:
    mov r4,r1
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0133
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0134
    fldi1 fr7
.Lmt7_packets_block_0133:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
    add #0x18,r4
.Lmt7_packets_block_0134:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv4
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0135
    fmov.s @r4+,fr0
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr0
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    bra .Lmt7_packets_block_0136
    fldi1 fr3
.Lmt7_packets_block_0135:
    fmov.s @r4+,fr1
    fmov.s @r4,fr2
    fldi1 fr3
    add #0x18,r4
.Lmt7_packets_block_0136:
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv0
    mov.l @r4,r0
    tst #0x1,r0
    flds fr7,FPUL
    bf/s .Lmt7_packets_block_0137
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0138
    fldi1 fr7
.Lmt7_packets_block_0137:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
.Lmt7_packets_block_0138:
    mov r1,r4
    sts FPUL,r0
    ftrv xmtrx,fv4
    cmp/hi r0,r12
    flds fr3,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    flds fr7,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    rts
    rotcl r9
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0139:
    tst r8,r8
    mov r8,r0
    bf .Lmt7_packets_block_0140
    bra .Lmt7_packets_block_0151
.Lmt7_packets_block_0140:
    tst #0x1,r0
    bf .Lmt7_packets_block_0141
    bra .Lmt7_packets_block_0164
    nop
.Lmt7_packets_block_0141:
    mov.l @r4+,r9
    fldi1 fr2
    fschg
    fldi0 fr3
    mov #0x3,r3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0142
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0142:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0143:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0148
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0144
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0144:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0146
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0145
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0145:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0143
    add #0x38,r6
.Lmt7_packets_block_0146:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0147
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0145
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0145
    add #-0x8,r14
.Lmt7_packets_block_0147:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0150
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0148:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0149
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0144
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0144
    add #-0x8,r14
.Lmt7_packets_block_0149:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0150:
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    add #-0x8,r4
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0151:
    mov.l @r4+,r9
    add #0x20,r6
    fschg
    mov #0x3,r3
    mov.l @r4,r0
    mov r4,r14
    fldi0 fr2
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0152
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0152:
    fmov.s @r14+,fr4
    mov r6,r2
    fmov.s @r14+,fr6
    fadd fr7,fr2
    fldi1 fr7
    fldi0 fr3
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0153:
    mov.l @r4+,r0
    dt r3
    flds fr15,FPUL
    fldi1 fr7
    fdiv fr4,fr7
    mov.l @r4+,r1
    bt/s .Lmt7_packets_block_0160
    fipr fv12,fv0
    tst #0x1,r0
    add r4,r1
    bt/s .Lmt7_packets_block_0154
    fsts FPUL,fr2
    mov r4,r1
    add #0x18,r4
    add #-0x8,r1
.Lmt7_packets_block_0154:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r14,fr0
    add #0x20,r4
    fmov.s @r1+,fr8
    bf .Lmt7_packets_block_0155
    fadd fr3,fr2
.Lmt7_packets_block_0155:
    fldi0 fr3
    dt r3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r1+,fr10
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fadd fr11,fr2
    fldi1 fr11
    fmul fr7,fr5
    fmov.s @r1+,fr0
    add #0x20,r5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    add #0x40,r6
    mov.l @r4+,r0
    mov.l @r4+,r14
    flds fr15,FPUL
    fipr fv12,fv0
    fldi1 fr11
    bt/s .Lmt7_packets_block_0158
    fdiv fr8,fr11
    tst #0x1,r0
    add r4,r14
    bt/s .Lmt7_packets_block_0156
    fsts FPUL,fr2
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0156:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr0
    add #0x20,r4
    fmov.s @r14+,fr4
    bf .Lmt7_packets_block_0157
    fadd fr3,fr2
.Lmt7_packets_block_0157:
    fldi0 fr3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r14+,fr6
    fmul fr11,fr10
    fmov.s fr0,@-r6
    fadd fr7,fr2
    fldi1 fr7
    fmul fr11,fr9
    fmov.s @r14+,fr0
    add #0x20,r5
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    bra .Lmt7_packets_block_0153
    add #0x40,r6
.Lmt7_packets_block_0158:
    dt r9
    fsts FPUL,fr2
    mov r6,r2
    bt/s .Lmt7_packets_block_0159
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0156
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0156
    add #-0x8,r14
.Lmt7_packets_block_0159:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    bra .Lmt7_packets_block_0162
    fmov fr10,fr6
.Lmt7_packets_block_0160:
    dt r9
    fsts FPUL,fr2
    mov r6,r2
    bt/s .Lmt7_packets_block_0161
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0154
    add r4,r1
    mov r4,r1
    add #0x18,r4
    bra .Lmt7_packets_block_0154
    add #-0x8,r1
    .word 0x0009
.Lmt7_packets_block_0161:
    add #0x30,r4
    mov r14,r1
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0162:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr10
    bf .Lmt7_packets_block_0163
    fadd fr3,fr2
.Lmt7_packets_block_0163:
    fldi0 fr3
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr10,@-r6
    add #0x20,r5
    fmov.s fr6,@-r6
    add #-0x8,r4
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0164:
    mov #0x7f,r2
    mov #0x3,r3
    mov.l @r4+,r9
    add #-0x20,r6
    mov.l @r4,r0
    add #0x7f,r2
    fschg
    add #0x37,r2
    tst #0x1,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0165
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0165:
    fmov.s @r14+,fr4
.Lmt7_packets_block_0166:
    fmov.s @r14+,fr6
    mov r6,r7
    fldi0 fr2
    fadd fr7,fr2
    fldi1 fr7
    add #0x40,r6
    fldi0 fr3
    add #0x20,r5
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
    fldi0 fr11
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0175
    mov r14,r1
    fipr fv12,fv0
    tst #0x1,r0
    mov.l @r4+,r14
    bf/s .Lmt7_packets_block_0169
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
.Lmt7_packets_block_0167:
    fcmp/gt fr11,fr3
    add r4,r14
    flds fr15,FPUL
    fipr fv0,fv8
    pref @r14
    bf/s .Lmt7_packets_block_0172
    fsts FPUL,fr2
    fadd fr3,fr2
    fldi0 fr3
    bra .Lmt7_packets_block_0171
    ftrc fr11,FPUL
.Lmt7_packets_block_0168:
    add #-0x1c,r4
    fldi1 fr7
.Lmt7_packets_block_0169:
    fdiv fr4,fr7
.Lmt7_packets_block_0170:
    add #0x18,r4
    fcmp/gt fr11,fr3
    flds fr15,FPUL
    fipr fv0,fv8
    mov r4,r14
    add #-0x20,r14
    pref @r4
    bf/s .Lmt7_packets_block_0172
    fsts FPUL,fr2
    fadd fr3,fr2
    ftrc fr11,FPUL
    fldi0 fr3
.Lmt7_packets_block_0171:
    sts FPUL,r0
    cmp/gt r2,r0
    shll2 r0
    bf .Lmt7_packets_block_0173
    fschg
    fmov.s @(r0,r8),fr3
    bra .Lmt7_packets_block_0173
    fschg
    .word 0x0009
.Lmt7_packets_block_0172:
    fldi0 fr3
.Lmt7_packets_block_0173:
    fmov.s @r1,fr0
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr0,@-r6
    tst r3,r3
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0174
    mov.l r7,@r6
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0166
.Lmt7_packets_block_0174:
    pref @r6
    add #0x20,r6
    rts
    fschg
    .word 0x0009
.Lmt7_packets_block_0175:
    mov r6,r7
    shar r7
    dt r9
    bt/s .Lmt7_packets_block_0168
    fipr fv12,fv0
    mov.l @r4+,r14
    tst #0x1,r0
    mov #0x3,r3
    fldi1 fr7
    bf/s .Lmt7_packets_block_0170
    fdiv fr4,fr7
    bra .Lmt7_packets_block_0167
    pref @r4
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0176:
    add r15,r0
    fldi0 fr12
    mov.l .Lmt7_packets_block_0182,r1
    fmov.s fr12,@-r0
    mov.l r1,@-r0
    fmov.s @r5+,fr14
    fmov.s @r5+,fr12
    fmov.s @r5+,fr13
    fmul fr1,fr12
    mov.l @r5+,r0
    fmul fr1,fr13
    mov.l r0,@(0x24,r15)
    mov.l @r5+,r7
    fmul fr1,fr14
    mov.l r7,@(0x3c,r15)
    mov.l .Lmt7_packets_block_0206,r0
    mov.l @r0+,r1
    mov.l @r0+,r2
    mov.l @r0+,r3
    mov.l r1,@(0x18,r15)
    mov.l r2,@(0x1c,r15)
    mov.l r3,@(0x20,r15)
    mov.l .Lmt7_packets_block_0218,r0
    mov.l @r0+,r5
    mov.l r5,@(0x28,r15)
    mov.l @r0+,r6
    mov.l r6,@(0x2c,r15)
    mov.l .Lmt7_packets_block_0203,r10
    mov r15,r3
    mov.l @r4+,r8
    add #0x4,r3
    mov.l @r10,r10
    mov #-0x1,r11
    fmov fr4,fr11
    or r5,r8
    fcmp/gt fr11,fr7
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    or r6,r1
    bf/s .Lmt7_packets_block_0179
    fmov.s @r4+,fr3
    bt .Lmt7_packets_block_0178
.Lmt7_packets_block_0177:
    mov.l @(0x28,r15),r3
    mov.l @(0x2c,r15),r0
    or r3,r8
    mov r15,r3
    fmov.s @r3+,fr11
    or r0,r1
    fldi1 fr7
    fcmp/gt fr11,fr7
    mov r4,r0
    add #0x30,r0
    bf/s .Lmt7_packets_block_0179
    pref @r0
.Lmt7_packets_block_0178:
    mov #0x2,r2
    mov r1,r0
    shll16 r2
    shll8 r2
    mov.l .Lmt7_packets_block_0217,r9
    or r2,r8
    or #0xc0,r0
    swap.w r0,r0
    mov.l @r9,r9
    or #0x10,r0
    swap.b r0,r0
    or #0xfc,r0
    xor r9,r0
    swap.b r0,r0
    swap.w r0,r0
    add r2,r2
    mov r0,r1
    not r2,r2
    and r2,r8
.Lmt7_packets_block_0179:
    mov r8,r0
    fmov.s @r4+,fr4
    shlr16 r0
    fmov.s @r4+,fr5
    shlr8 r0
    fmov.s @r4+,fr6
    and #0x7,r0
    mov #0x4,r2
    shll2 r0
    ftrv xmtrx,fv4
    fmov.s @r3+,fr1
    shll8 r2
    fmov.s @r4+,fr0
    cmp/eq r0,r11
    mov r0,r11
    mov #-0x20,r6
    fmul fr1,fr0
    shll16 r2
    bt/s .Lmt7_packets_block_0180
    mov.l @(r0,r10),r5
    mov #-0x1,r0
    shll16 r0
    shll8 r0
    or #0x38,r0
    mov r5,r9
    shlr16 r9
    shlr8 r9
    mov.l r9,@r0
    mov.l r9,@(0x4,r0)
.Lmt7_packets_block_0180:
    add #-0x1,r2
    lds r12,FPUL
    mov #-0x2,r0
    fadd fr0,fr7
    and r5,r2
    fsts FPUL,fr1
    shll8 r6
    fcmp/gt fr1,fr7
    shll16 r6
    or r2,r6
    fsub fr0,fr7
    and r0,r12
    mov.l .Lmt7_packets_block_0204,r9
    bf/s .Lmt7_packets_block_0185
    fsub fr0,fr7
    fcmp/gt fr1,fr7
    bf/s .Lmt7_packets_block_0185
    add #0x1,r12
.Lmt7_packets_block_0181:
    mov r4,r2
    add #0x2c,r4
    mov.l @r4+,r0
    mov.l r8,@r6
    mov r8,r1
    add r0,r4
    mov.l @r4+,r8
    tst r8,r8
    bf .Lmt7_packets_block_0183
    bra .Lmt7_packets_block_0198
    nop
    .word 0x0009
.Lmt7_packets_block_0182:
    .long .Lmt7_packets_block_0201
.Lmt7_packets_block_0183:
    mov r15,r9
    add #0x18,r9
    fmov.s @r9+,fr5
    fmov.s @r9+,fr6
    add #0xc,r2
    fmov.s @r9+,fr7
    mov r1,r0
    tst #0x10,r0
    bf/s .Lmt7_packets_block_0184
    tst #0x20,r0
    bt .Lmt7_packets_block_0184
    fmov.s @r2+,fr0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    fmov.s @r2+,fr3
    fmul fr11,fr0
    mov.l .Lmt7_packets_block_0207,r0
    fmov.s @r0+,fr11
    fmul fr5,fr1
    fmov.s @r2+,fr4
    fmov.s @r2+,fr5
    fmul fr11,fr5
    fmov.s @r0+,fr11
    fmul fr6,fr2
    fmov.s @r2+,fr6
    fmul fr11,fr6
    fmov.s @r0+,fr11
    fmul fr7,fr3
    fmov.s @r2+,fr7
    fschg
    fmul fr11,fr7
    mov #0x0,r0
    mov.l r0,@(0x4,r6)
    mov.l r0,@(0x8,r6)
    mov.l r0,@(0xc,r6)
    pref @r6
    add #0x40,r6
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    mov r11,r0
    fmov.s fr2,@-r6
    add #0x40,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    add #0x20,r6
    mov.l r5,@(r0,r10)
.Lmt7_packets_block_0184:
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0177
    fmov.s @r4+,fr3
    .word 0x0009
.Lmt7_packets_block_0185:
    fmov.s @r3+,fr1
    mov.l .Lmt7_packets_block_0214,r0
    add #0x10,r9
    fmov.s @r0,fr7
    fmul fr4,fr7
    fneg fr0
    fmul fr0,fr1
    fmov.s fr3,@-r9
    fsub fr7,fr5
    fmov.s @r3+,fr3
    mov r4,r2
    mov.l .Lmt7_packets_block_0213,r0
    fcmp/gt fr1,fr5
    fmov.s @r0,fr7
    fmul fr0,fr3
    bf/s .Lmt7_packets_block_0181
    fmul fr4,fr7
    mov.l r1,@-r9
    fsub fr7,fr6
    fmov.s @r3+,fr0
    fcmp/gt fr3,fr6
    fmov.s @r3+,fr7
    fmul fr4,fr0
    bf/s .Lmt7_packets_block_0181
    fsub fr1,fr0
    fcmp/gt fr0,fr5
    fmov.s @r3+,fr5
    fmul fr4,fr7
    bt/s .Lmt7_packets_block_0181
    fmov.s fr2,@-r9
    fsub fr3,fr7
    mov.l r8,@-r9
    mov #0x8,r1
    fcmp/gt fr7,fr6
    fmov.s @r3+,fr6
    bt .Lmt7_packets_block_0181
    mov r8,r0
    tst #0x10,r0
    mov.b @(0x4,r2),r0
    add #0x8,r2
    fmov.s @r3+,fr7
    mov r0,r8
    fmov.s @r2+,fr15
    bf/s .Lmt7_packets_block_0186
    add #0x30,r4
    fmov.s @r2+,fr0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    fmov.s @r2+,fr3
    fmul fr11,fr0
    mov.l .Lmt7_packets_block_0207,r0
    fmov.s @r0+,fr11
    fmul fr5,fr1
    fmov.s @r2+,fr4
    fmov.s @r2+,fr5
    fmul fr11,fr5
    fmov.s @r0+,fr11
    fmul fr6,fr2
    fmov.s @r2+,fr6
    fmul fr11,fr6
    fmov.s @r0+,fr11
    fmul fr7,fr3
    fmov.s @r2+,fr7
    add #0x40,r6
    fmul fr11,fr7
    fschg
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    fschg
    add #-0x20,r6
.Lmt7_packets_block_0186:
    cmp/pz r8
    bt .Lmt7_packets_block_0189
    mov r8,r0
    cmp/eq #-0x1,r0
    bt/s .Lmt7_packets_block_0188
    cmp/eq #-0x2,r0
    bf .Lmt7_packets_block_0187
    mov.l .Lmt7_packets_block_0215,r0
    jsr @r0
    mov.l @(0x3c,r15),r7
    bt .Lmt7_packets_block_0198
    .word 0xf249
    .word 0x6146
    .word 0xaf19
    .word 0xf349
    .word 0x0009
.Lmt7_packets_block_0187:
    mov.l @(0x3c,r15),r7
    cmp/eq #-0x3,r0
    mov #0x24,r0
    mov.l .Lmt7_packets_block_0216,r1
    bf .Lmt7_packets_block_0188
    fmov.s @(r0,r15),fr1
    jsr @r1
    fmul fr1,fr15
    bt .Lmt7_packets_block_0198
    .word 0xf249
    .word 0x6146
    .word 0xaf0b
    .word 0xf349
    .word 0x0009
.Lmt7_packets_block_0188:
    bra .Lmt7_packets_block_0191
    fldi1 fr15
.Lmt7_packets_block_0189:
    cmp/pl r8
    mov #0x38,r0
    bf .Lmt7_packets_block_0190
    fmov.s @(r0,r15),fr10
    mov #0x30,r0
    fmov.s @(r0,r15),fr8
    mov #0x34,r0
    fmov.s @(r0,r15),fr9
    mov #0x9,r0
    shld r0,r8
    mov.l .Lmt7_packets_block_0219,r0
    add r0,r8
.Lmt7_packets_block_0190:
    mov #0x24,r0
    fmov.s @(r0,r15),fr1
    fmul fr1,fr15
.Lmt7_packets_block_0191:
    mov.l @r4+,r0
.Lmt7_packets_block_0192:
    cmp/pl r0
    mov.l .Lmt7_packets_block_0204,r9
    bt/s .Lmt7_packets_block_0193
    cmp/pz r0
    mov r0,r8
    mov r11,r0
    mov.l r5,@(r0,r10)
    bt .Lmt7_packets_block_0198
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0177
    fmov.s @r4+,fr3
.Lmt7_packets_block_0193:
    mov r0,r1
    mov #-0x5,r2
    mov.l @r9+,r3
    and #0x40,r0
    shld r2,r0
    mov #-0x3,r2
    and r2,r3
    or r3,r0
    tst #0x10,r0
    add #0x20,r4
    bf/s .Lmt7_packets_block_0194
    lds r0,FPUL
    or #0x10,r0
    mov r9,r2
    mov.l r0,@-r2
.Lmt7_packets_block_0194:
    mov.l @(0x28,r15),r2
    movt r3
    mov r1,r0
    cmp/pz r2
    mov #0x1b,r2
    bt/s .Lmt7_packets_block_0195
    and #0x3,r0
    xor #0x1,r0
.Lmt7_packets_block_0195:
    shld r2,r0
    cmp/pl r3
    mov #0x3,r3
    fschg
    shld r2,r3
    pref @r4
    not r3,r3
    mov.l @r9+,r2
    and r3,r2
    fsts FPUL,fr0
    or r0,r2
    lds r2,FPUL
    add #-0x20,r4
    fsts FPUL,fr1
    add #0x10,r6
    fmov.s @r9+,fr2
    fmov.s fr2,@-r6
    add #0x20,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    bf/s .Lmt7_packets_block_0196
    add #0x20,r6
    pref @r6
    add #0x20,r6
    add #0x20,r5
.Lmt7_packets_block_0196:
    rotcr r12
    mov r1,r0
    bt/s .Lmt7_packets_block_0199
    shll r12
    tst #0x8,r0
    bf .Lmt7_packets_block_0197
    bsr .Lmt7_packets_block_0220
    nop
    bra .Lmt7_packets_block_0192
    mov.l @r4+,r0
    .word 0x0009
    .word 0x0009
    .word 0xafb5
    .word 0x6046
    .word 0x0009
.Lmt7_packets_block_0197:
    bsr .Lmt7_packets_block_0315
    nop
    bra .Lmt7_packets_block_0192
    mov.l @r4+,r0
.Lmt7_packets_block_0198:
    add #0x48,r15
    lds.l @r15+,PR
    mov.l @r15+,r8
    mov.l @r15+,r9
    mov.l @r15+,r10
    mov.l @r15+,r11
    mov.l @r15+,r12
    mov.l @r15+,r13
    mov.l @r15+,r14
    fmov.s @r15+,fr12
    fmov.s @r15+,fr13
    fmov.s @r15+,fr14
    fmov.s @r15+,fr15
    rts
    nop
    .word 0x0009
.Lmt7_packets_block_0199:
    mov #0x0,r1
    tst #0x40,r0
    addc r1,r12
    mov.l @(0x3c,r15),r7
    tst #0x10,r0
    bt/s .Lmt7_packets_block_0200
    tst #0x8,r0
    bsr .Lmt7_packets_block_0257
    tst #0x20,r0
    bra .Lmt7_packets_block_0192
    mov.l @r4+,r0
    .word 0x0009
.Lmt7_packets_block_0200:
    bsr .Lmt7_packets_block_0299
    tst #0x20,r0
    bra .Lmt7_packets_block_0192
    mov.l @r4+,r0
.Lmt7_packets_block_0201:
    fmov.s fr12,@-r15
    fmov.s fr13,@-r15
    fmov.s fr14,@-r15
    mov.l r0,@-r15
    mov.l .Lmt7_packets_block_0208,r0
    mov.l @r0,r0
    fmov.s @r0+,fr14
    fmov.s @r0+,fr12
    fmov.s @r0+,fr13
    fipr fv12,fv0
    fldi0 fr12
    fcmp/gt fr12,fr3
    bf .Lmt7_packets_block_0202
    fmov fr3,fr12
.Lmt7_packets_block_0202:
    mov.l @r15+,r0
    fmov.s fr12,@r0
    fmov.s @r15+,fr14
    fmov.s @r15+,fr13
    rts
    fmov.s @r15+,fr12
.Lmt7_packets_block_0203:
    .long __nj_vertex_buffer_
.Lmt7_packets_block_0204:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch
.Lmt7_packets_block_0205:
    .long __Q37shenmue2dc3sdk.g_modelScale
.Lmt7_packets_block_0206:
    .long __Q37shenmue2dc3sdk.g_mt7DiffuseChannels
.Lmt7_packets_block_0207:
    .long __Q37shenmue2dc3sdk.g_mt7SpecularChannels
.Lmt7_packets_block_0208:
    .long __Q37shenmue2dc3sdk.g_mt7ReflectedDirection
.Lmt7_packets_block_0209:
    .long __Q37shenmue2dc3sdk.g_mt7HorizontalClipScale
.Lmt7_packets_block_0210:
    .long __Q37shenmue2dc3sdk.g_mt7VerticalClipScale
.Lmt7_packets_block_0211:
    .long __Q27shenmue12matrix_stack.g_projectionViewportWidth
.Lmt7_packets_block_0212:
    .long __Q27shenmue12matrix_stack.g_projectionViewportHeight
.Lmt7_packets_block_0213:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginY
.Lmt7_packets_block_0214:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginX
.Lmt7_packets_block_0215:
    .long .Lmt7_packets_block_0352
.Lmt7_packets_block_0216:
    .long .Lmt7_packets_block_0423
.Lmt7_packets_block_0217:
    .long __Q37shenmue2dc3sdk.g_mt7AlphaBlendMask
.Lmt7_packets_block_0218:
    .long _shenmue_dc_mt7_draw_controls
.Lmt7_packets_block_0219:
    .long __Q37shenmue2dc3sdk.g_mt7SpecularLookup-0x4d4
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0220:
    tst r8,r8
    mov r8,r0
    bf .Lmt7_packets_block_0221
    bra .Lmt7_packets_block_0232
.Lmt7_packets_block_0221:
    tst #0x1,r0
    bf .Lmt7_packets_block_0222
    bra .Lmt7_packets_block_0245
    nop
.Lmt7_packets_block_0222:
    mov.l @r4+,r3
    fldi1 fr2
    fschg
    fldi0 fr3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0223
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0223:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0224:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0229
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0225
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0225:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0227
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0226
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0226:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0224
    add #0x38,r6
    .word 0x0009
.Lmt7_packets_block_0227:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0228
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0228
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0226
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0226
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0228:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0231
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0229:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0230
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0230
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0225
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0225
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0230:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0231:
    fmul fr7,fr6
    mov r6,r2
    shar r2
    fmul fr7,fr5
    fmov.s fr0,@-r6
    add #-0x8,r4
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0232:
    mov.l @r4+,r3
    add #0x20,r6
    fschg
    mov.l @r4,r0
    mov r4,r14
    fldi0 fr2
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0233
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0233:
    fmov.s @r14+,fr4
    mov r6,r2
    fmov.s @r14+,fr6
    fadd fr7,fr2
    fldi1 fr7
    fldi0 fr3
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0234:
    mov #0x40,r0
    add r15,r0
    mov.l @r0+,r1
    sts.l PR,@-r15
    jsr @r1
    fschg
    lds.l @r15+,PR
    fschg
    mov.l @r4+,r0
    dt r3
    flds fr15,FPUL
    fldi1 fr7
    fdiv fr4,fr7
    mov.l @r4+,r1
    bt/s .Lmt7_packets_block_0241
    fipr fv12,fv0
    tst #0x1,r0
    add r4,r1
    bt/s .Lmt7_packets_block_0235
    fsts FPUL,fr2
    mov r4,r1
    add #0x18,r4
    add #-0x8,r1
.Lmt7_packets_block_0235:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r14,fr0
    add #0x20,r4
    fmov.s @r1+,fr8
    bf .Lmt7_packets_block_0236
    fadd fr3,fr2
.Lmt7_packets_block_0236:
    fschg
    mov #0x44,r0
    fmov.s @(r0,r15),fr10
    fadd fr10,fr2
    fschg
    fldi0 fr3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r1+,fr10
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fadd fr11,fr2
    fldi1 fr11
    fmul fr7,fr5
    fmov.s @r1+,fr0
    add #0x20,r5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    add #0x40,r6
    mov #0x40,r0
    add r15,r0
    mov.l @r0+,r14
    sts.l PR,@-r15
    jsr @r14
    fschg
    lds.l @r15+,PR
    fschg
    dt r3
    mov.l @r4+,r0
    mov.l @r4+,r14
    flds fr15,FPUL
    fipr fv12,fv0
    fldi1 fr11
    bt/s .Lmt7_packets_block_0239
    fdiv fr8,fr11
    tst #0x1,r0
    add r4,r14
    bt/s .Lmt7_packets_block_0237
    fsts FPUL,fr2
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0237:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr0
    add #0x20,r4
    fmov.s @r14+,fr4
    bf .Lmt7_packets_block_0238
    fadd fr3,fr2
.Lmt7_packets_block_0238:
    fschg
    mov #0x44,r0
    fmov.s @(r0,r15),fr6
    fadd fr6,fr2
    fschg
    fldi0 fr3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r14+,fr6
    fmul fr11,fr10
    fmov.s fr0,@-r6
    fadd fr7,fr2
    fldi1 fr7
    fmul fr11,fr9
    fmov.s @r14+,fr0
    add #0x20,r5
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    bra .Lmt7_packets_block_0234
    add #0x40,r6
.Lmt7_packets_block_0239:
    cmp/pl r0
    fsts FPUL,fr2
    mov r6,r2
    bf/s .Lmt7_packets_block_0240
    shar r2
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0240
    mov.l @r4+,r0
    tst #0x1,r0
    mov.l @r4+,r14
    bt/s .Lmt7_packets_block_0237
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0237
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0240:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    bra .Lmt7_packets_block_0243
    fmov fr10,fr6
.Lmt7_packets_block_0241:
    cmp/pl r0
    fsts FPUL,fr2
    mov r6,r2
    bf/s .Lmt7_packets_block_0242
    shar r2
    tst #0x80,r0
    mov r1,r3
    bt .Lmt7_packets_block_0242
    mov.l @r4+,r0
    tst #0x1,r0
    mov.l @r4+,r1
    bt/s .Lmt7_packets_block_0235
    add r4,r1
    mov r4,r1
    add #0x18,r4
    bra .Lmt7_packets_block_0235
    add #-0x8,r1
    .word 0x0009
.Lmt7_packets_block_0242:
    add #0x30,r4
    mov r14,r1
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0243:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr10
    bf .Lmt7_packets_block_0244
    fadd fr3,fr2
.Lmt7_packets_block_0244:
    fschg
    mov #0x44,r0
    fmov.s @(r0,r15),fr0
    fadd fr0,fr2
    fschg
    fldi0 fr3
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr10,@-r6
    add #-0x8,r4
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0245:
    mov #0x7f,r2
    mov.l @r4+,r3
    add #-0x20,r6
    mov.l @r4,r0
    add #0x7f,r2
    fschg
    add #0x37,r2
    tst #0x1,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0246
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0246:
    fmov.s @r14+,fr4
.Lmt7_packets_block_0247:
    fmov.s @r14+,fr6
    mov r6,r7
    fldi0 fr2
    fadd fr7,fr2
    fldi1 fr7
    add #0x40,r6
    fldi0 fr3
    add #0x20,r5
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
    fldi0 fr11
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0256
    mov r14,r1
    fipr fv12,fv0
    tst #0x1,r0
    mov.l @r4+,r14
    bf/s .Lmt7_packets_block_0250
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
.Lmt7_packets_block_0248:
    fcmp/gt fr11,fr3
    add r4,r14
    flds fr15,FPUL
    fipr fv0,fv8
    pref @r14
    bf/s .Lmt7_packets_block_0253
    fsts FPUL,fr2
    fadd fr3,fr2
    fldi0 fr3
    bra .Lmt7_packets_block_0252
    ftrc fr11,FPUL
.Lmt7_packets_block_0249:
    add #-0x1c,r4
    fldi1 fr7
.Lmt7_packets_block_0250:
    fdiv fr4,fr7
.Lmt7_packets_block_0251:
    add #0x18,r4
    fcmp/gt fr11,fr3
    flds fr15,FPUL
    fipr fv0,fv8
    mov r4,r14
    add #-0x20,r14
    pref @r4
    bf/s .Lmt7_packets_block_0253
    fsts FPUL,fr2
    fadd fr3,fr2
    ftrc fr11,FPUL
    fldi0 fr3
.Lmt7_packets_block_0252:
    sts FPUL,r0
    cmp/gt r2,r0
    shll2 r0
    bf .Lmt7_packets_block_0254
    fschg
    fmov.s @(r0,r8),fr3
    bra .Lmt7_packets_block_0254
    fschg
    .word 0x0009
.Lmt7_packets_block_0253:
    fldi0 fr3
.Lmt7_packets_block_0254:
    fmov.s @r1,fr0
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr0,@-r6
    tst r3,r3
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0255
    mov.l r7,@r6
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0247
.Lmt7_packets_block_0255:
    pref @r6
    add #0x20,r6
    rts
    fschg
    .word 0x0009
.Lmt7_packets_block_0256:
    mov r6,r7
    shar r7
    cmp/pl r0
    bf/s .Lmt7_packets_block_0249
    fipr fv12,fv0
    tst #0x80,r0
    fldi1 fr7
    bt .Lmt7_packets_block_0249
    mov.l @r4+,r3
    mov.l @r4+,r0
    mov.l @r4+,r14
    tst #0x1,r0
    fdiv fr4,fr7
    bf .Lmt7_packets_block_0251
    bra .Lmt7_packets_block_0248
    pref @r4
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0257:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0258:
    mov #0x0,r11
    bsr .Lmt7_packets_block_0283
    mov #0x0,r9
    bt/s .Lmt7_packets_block_0260
    rotcl r9
    bsr .Lmt7_packets_block_0289
    add #0x1,r11
    bsr .Lmt7_packets_block_0283
    nop
    bt/s .Lmt7_packets_block_0266
    rotcl r9
    bsr .Lmt7_packets_block_0289
    add #0x1,r11
.Lmt7_packets_block_0259:
    bsr .Lmt7_packets_block_0283
    nop
    bt/s .Lmt7_packets_block_0276
    rotcl r9
    bsr .Lmt7_packets_block_0289
    add #0x1,r11
    bf .Lmt7_packets_block_0259
    bt .Lmt7_packets_block_0264
.Lmt7_packets_block_0260:
    dt r3
    add #0x1,r11
    bsr .Lmt7_packets_block_0283
    dt r3
    bf/s .Lmt7_packets_block_0270
    rotcl r9
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf/s .Lmt7_packets_block_0261
    fldi1 fr7
    add #-0x18,r4
    mov.l @(0x4,r14),r14
    add r4,r14
.Lmt7_packets_block_0261:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr5
.Lmt7_packets_block_0262:
    fmov.s @r14+,fr6
    mov #0x20,r1
    mov.l @r4,r0
    add #0x20,r4
    pref @r4
    ftrv xmtrx,fv4
    tst #0x1,r0
    add #-0x20,r4
    bf/s .Lmt7_packets_block_0263
    mov r4,r14
    mov #0x8,r1
    mov.l @(0x4,r14),r14
    add #0x8,r14
    add r4,r14
.Lmt7_packets_block_0263:
    flds fr7,FPUL
    sts FPUL,r0
    cmp/hi r0,r12
    add #0x1,r11
    bf/s .Lmt7_packets_block_0274
    rotcl r9
    dt r3
    fldi1 fr7
    bt .Lmt7_packets_block_0264
    add r1,r4
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0262
    fmov.s @r14+,fr5
.Lmt7_packets_block_0264:
    mov.l @r4+,r0
    cmp/pl r0
    bf/s .Lmt7_packets_block_0265
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0258
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0265:
    lds.l @r15+,PR
    mov #0x1,r0
    mov.l @r15+,r11
    add #-0x4,r4
    mov.l @r15+,r10
    rts
    or r0,r12
.Lmt7_packets_block_0266:
    bsr .Lmt7_packets_block_0291
    dt r3
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0267
    add #-0x18,r4
.Lmt7_packets_block_0267:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0268
    add #-0x18,r4
.Lmt7_packets_block_0268:
    bsr .Lmt7_packets_block_0283
    nop
    bsr .Lmt7_packets_block_0291
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0295
    mov #0x0,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0269
    add #0x18,r4
.Lmt7_packets_block_0269:
    bra .Lmt7_packets_block_0281
    nop
.Lmt7_packets_block_0270:
    bsr .Lmt7_packets_block_0291
    mov #0x1,r11
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0271
    add #-0x18,r4
.Lmt7_packets_block_0271:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0272
    add #-0x18,r4
.Lmt7_packets_block_0272:
    bsr .Lmt7_packets_block_0283
    nop
    bsr .Lmt7_packets_block_0291
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0295
    mov #0x0,r0
    clrt
    bsr .Lmt7_packets_block_0293
    mov #0x1,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0273
    add #0x18,r4
.Lmt7_packets_block_0273:
    bra .Lmt7_packets_block_0281
    nop
.Lmt7_packets_block_0274:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0275
    add #-0x18,r4
.Lmt7_packets_block_0275:
    bsr .Lmt7_packets_block_0283
    nop
.Lmt7_packets_block_0276:
    bsr .Lmt7_packets_block_0291
    nop
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0277
    add #-0x18,r4
.Lmt7_packets_block_0277:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0278
    add #-0x18,r4
.Lmt7_packets_block_0278:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0279
    add #-0x18,r4
.Lmt7_packets_block_0279:
    bsr .Lmt7_packets_block_0283
    nop
    bsr .Lmt7_packets_block_0291
    add #-0x2,r11
    bsr .Lmt7_packets_block_0283
    nop
    bsr .Lmt7_packets_block_0291
    add #0x1,r11
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0280
    add #0x18,r4
.Lmt7_packets_block_0280:
    add #0x1,r11
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0282
    mov r9,r0
    bsr .Lmt7_packets_block_0292
    nop
    bra .Lmt7_packets_block_0282
    mov r9,r0
.Lmt7_packets_block_0281:
    bsr .Lmt7_packets_block_0283
    add #0x1,r11
    bsr .Lmt7_packets_block_0291
    rotcl r9
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0282
    mov r9,r0
    bsr .Lmt7_packets_block_0292
    nop
    mov r9,r0
.Lmt7_packets_block_0282:
    and #0x7,r0
    shll2 r0
    braf r0
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa00c
    .word 0x0009
    .word 0xa019
    .word 0x0009
    .word 0xa024
    .word 0x0009
    .word 0xa02a
    .word 0x0009
    .word 0xa038
    .word 0x0009
    .word 0xa03d
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb133
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb10f
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb12b
    .word 0x70ff
    .word 0x8bd3
    .word 0xaf5d
    .word 0x0009
    .word 0x0008
    .word 0xb105
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb121
    .word 0x70ff
    .word 0x4310
    .word 0xb0fe
    .word 0x60b3
    .word 0x8bc6
    .word 0xaf50
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb117
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf22
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb10f
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0eb
    .word 0x70ff
    .word 0x4310
    .word 0xb0e8
    .word 0x60b3
    .word 0x8901
    .word 0xaf06
    .word 0x7b01
    .word 0xaf38
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb0ff
    .word 0x70ff
    .word 0x8ba7
    .word 0xaf31
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x890d
    .word 0x60b3
    .word 0x0008
    .word 0xb0f5
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb0f1
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0ed
    .word 0x70ff
    .word 0xa008
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb0e7
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0e3
    .word 0x70ff
    .word 0x4310
    .word 0xb0c0
    .word 0x60b3
    .word 0x8b88
    .word 0xaf12
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0283:
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0284
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    fldi1 fr7
    fmov.s @r0+,fr2
    fmov.s @r0+,fr0
    ftrv xmtrx,fv4
    fmov.s @r0+,fr1
    fldi0 fr3
    add #0x20,r4
    bra .Lmt7_packets_block_0285
    mov r0,r2
.Lmt7_packets_block_0284:
    fmov.s @r4+,fr5
    fmov.s @r4+,fr6
    fldi1 fr7
    fmov.s @r4+,fr2
    fmov.s @r4+,fr0
    ftrv xmtrx,fv4
    fmov.s @r4+,fr1
    fldi0 fr3
    mov r4,r2
    add #0x28,r4
.Lmt7_packets_block_0285:
    mov #0x4c,r0
    add r15,r0
    mov.l @r0+,r1
    sts.l PR,@-r15
    jsr @r1
    nop
    lds.l @r15+,PR
    fldi0 fr11
    fipr fv12,fv0
    pref @r4
    add #-0x20,r4
    flds fr7,FPUL
    sts FPUL,r0
    fldi1 fr7
    fdiv fr4,fr7
    fsts FPUL,fr4
    fcmp/gt fr11,fr3
    fipr fv0,fv8
    fmov fr15,fr2
    bf .Lmt7_packets_block_0287
    fadd fr3,fr2
    tst r8,r8
    fldi0 fr3
    bt/s .Lmt7_packets_block_0288
    mov #0x1,r1
    tst r1,r8
    bf/s .Lmt7_packets_block_0286
    ftrc fr11,FPUL
    mov #0x7f,r1
    add #0x7f,r1
    sts FPUL,r0
    add #0x37,r1
    cmp/gt r1,r0
    shll2 r0
    bf .Lmt7_packets_block_0287
    mov.l @(r0,r8),r0
    lds r0,FPUL
    fsts FPUL,fr3
    flds fr4,FPUL
    sts FPUL,r0
    rts
    cmp/hi r0,r12
.Lmt7_packets_block_0286:
    fldi1 fr2
    flds fr4,FPUL
    sts FPUL,r0
    rts
    cmp/hi r0,r12
.Lmt7_packets_block_0287:
    flds fr4,FPUL
    sts FPUL,r0
.Lmt7_packets_block_0288:
    fldi0 fr3
    rts
    cmp/hi r0,r12
.Lmt7_packets_block_0289:
    mov #0x50,r0
    fmov.s @(r0,r15),fr0
    fadd fr0,fr2
    fschg
    add #0x20,r6
    fmov.s @r2,fr0
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr0,@-r6
    mov r6,r2
    fmov.s fr6,@-r6
    dt r3
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0290
    fschg
    mov.l r6,@r6
    add #0x20,r5
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0290:
    shar r2
    mov.l r2,@r6
    add #0x20,r5
    sett
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0291:
    mov #0x50,r0
    fmov.s @(r0,r15),fr0
    fadd fr0,fr2
    mov #0x5,r10
    mov r11,r0
    and #0x3,r0
    fschg
    shld r10,r0
    mov.l .Lmt7_packets_block_0298,r10
    fmov.s @r2,fr0
    add r0,r10
    fmov.s fr2,@-r10
    fmov.s fr0,@-r10
    fmov.s fr6,@-r10
    fmov.s fr4,@-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0292:
    mov #0x5,r1
    mov.l .Lmt7_packets_block_0298,r10
    mov r11,r0
    and #0x3,r0
    mov r10,r2
    shld r1,r0
    fschg
    add r0,r10
    add #-0x8,r10
    mov r11,r0
    fmov.s @r10,fr2
    add #-0x1,r0
    mov r2,r10
    and #0x3,r0
    shld r1,r0
    add r0,r2
    mov r11,r0
    fmov.s fr2,@-r2
    add #-0x2,r0
    and #0x3,r0
    shld r1,r0
    add r0,r10
    fmov.s fr2,@-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0293:
    and #0x3,r0
    mov #0x5,r2
    mov.l .Lmt7_packets_block_0297,r10
    shld r2,r0
    fschg
    add r0,r10
    fmov.s @r10+,fr0
    fmov.s @r10+,fr2
    add #0x20,r6
    fmov.s @r10+,fr4
    fmul fr3,fr2
    fmov.s @r10+,fr6
    fmul fr3,fr1
    fmov.s fr6,@-r6
    mov r6,r2
    fmov.s fr4,@-r6
    add #0x20,r5
    fmov.s fr2,@-r6
    bf/s .Lmt7_packets_block_0294
    fmov.s fr0,@-r6
    shar r2
    sett
.Lmt7_packets_block_0294:
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0295:
    and #0x3,r0
    shll2 r0
    mov.l .Lmt7_packets_block_0297,r10
    shll2 r0
    add r0,r0
    mov r10,r2
    add r0,r10
    mov r11,r0
    and #0x3,r0
    fmov.s @r10,fr2
    shll2 r0
    shll2 r0
    add r0,r0
    add r0,r2
    fmov.s @r2,fr1
    lds r12,FPUL
    fsts FPUL,fr0
    fsub fr2,fr0
    add #0x18,r10
    fsub fr2,fr1
    add #0x18,r2
    fdiv fr1,fr0
    lds r7,FPUL
    fsts FPUL,fr1
    fschg
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    add #0x20,r6
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fschg
    fmov.s fr1,@-r6
    fmov.s @r2,fr5
    add #-0x4,r2
    fmov.s @r10,fr3
    add #-0x4,r10
    fmov.s @r2,fr4
    fmov.s @r10,fr2
    fsub fr3,fr5
    fsub fr2,fr4
    mov #-0x10,r2
    fmac fr0,fr5,fr3
    shll16 r2
    fmac fr0,fr4,fr2
    shll8 r2
    fmul fr1,fr3
    add #0x20,r5
    fmul fr1,fr2
    fmov.s fr3,@-r6
    fmov.s fr2,@-r6
    bt .Lmt7_packets_block_0296
    mov r6,r2
.Lmt7_packets_block_0296:
    mov.l r2,@-r6
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0297:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch+0x40
.Lmt7_packets_block_0298:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch+0x60
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0299:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0300:
    bsr .Lmt7_packets_block_0308
    mov #0x0,r9
    mov r9,r0
    cmp/eq #0x0,r0
    bt .Lmt7_packets_block_0303
    cmp/eq #0x7,r0
    bt .Lmt7_packets_block_0304
    bsr .Lmt7_packets_block_0283
    mov #0x0,r11
    bsr .Lmt7_packets_block_0291
    nop
    bsr .Lmt7_packets_block_0283
    mov #0x1,r11
    bsr .Lmt7_packets_block_0291
    nop
    bsr .Lmt7_packets_block_0283
    mov #0x2,r11
    bsr .Lmt7_packets_block_0291
    nop
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0301
    shll2 r9
    bsr .Lmt7_packets_block_0292
    nop
.Lmt7_packets_block_0301:
    braf r9
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa034
    .word 0x0009
    .word 0xa041
    .word 0x0009
    .word 0xa04f
    .word 0x0009
    .word 0xa05a
    .word 0x0009
    .word 0xa068
    .word 0x0009
    .word 0xa073
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0302:
    dt r3
    bf .Lmt7_packets_block_0300
    mov #0x1,r0
    lds.l @r15+,PR
    or r0,r12
    mov.l @r15+,r11
    rts
    mov.l @r15+,r10
.Lmt7_packets_block_0303:
    mov r3,r9
    bsr .Lmt7_packets_block_0283
    mov #0x3,r3
    bsr .Lmt7_packets_block_0289
    nop
    bsr .Lmt7_packets_block_0283
    nop
    bsr .Lmt7_packets_block_0289
    nop
    bsr .Lmt7_packets_block_0283
    nop
    bsr .Lmt7_packets_block_0289
    nop
    bra .Lmt7_packets_block_0302
    mov r9,r3
.Lmt7_packets_block_0304:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0305
    add #0x18,r4
.Lmt7_packets_block_0305:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0306
    add #0x18,r4
.Lmt7_packets_block_0306:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0307
    add #0x18,r4
.Lmt7_packets_block_0307:
    bra .Lmt7_packets_block_0302
    nop
    .word 0x0008
    .word 0xbf36
    .word 0xe000
    .word 0x0008
    .word 0xbf33
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf4f
    .word 0xe000
    .word 0x0018
    .word 0xbf4c
    .word 0xe001
    .word 0xafc9
    .word 0x0009
    .word 0x0008
    .word 0xbf27
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf43
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbf1f
    .word 0xe002
    .word 0x0018
    .word 0xbf3c
    .word 0xe001
    .word 0xafb9
    .word 0x0009
    .word 0x0008
    .word 0xbf17
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf33
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbf2f
    .word 0xe000
    .word 0xafac
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf29
    .word 0xe000
    .word 0x0008
    .word 0xbf06
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf22
    .word 0xe000
    .word 0x0018
    .word 0xbeff
    .word 0xe002
    .word 0xaf9c
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf19
    .word 0xe000
    .word 0x0008
    .word 0xbef6
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbf12
    .word 0xe001
    .word 0xaf8f
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbf0c
    .word 0xe000
    .word 0x0008
    .word 0xbf09
    .word 0xe001
    .word 0x0018
    .word 0xbee6
    .word 0xe002
    .word 0xaf83
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0308:
    mov r4,r1
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0309
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0310
    fldi1 fr7
.Lmt7_packets_block_0309:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
    add #0x18,r4
.Lmt7_packets_block_0310:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv4
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0311
    fmov.s @r4+,fr0
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr0
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    bra .Lmt7_packets_block_0312
    fldi1 fr3
.Lmt7_packets_block_0311:
    fmov.s @r4+,fr1
    fmov.s @r4,fr2
    fldi1 fr3
    add #0x18,r4
.Lmt7_packets_block_0312:
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv0
    mov.l @r4,r0
    tst #0x1,r0
    flds fr7,FPUL
    bf/s .Lmt7_packets_block_0313
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0314
    fldi1 fr7
.Lmt7_packets_block_0313:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
.Lmt7_packets_block_0314:
    mov r1,r4
    sts FPUL,r0
    ftrv xmtrx,fv4
    cmp/hi r0,r12
    flds fr3,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    flds fr7,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    rts
    rotcl r9
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0315:
    tst r8,r8
    mov r8,r0
    bf .Lmt7_packets_block_0316
    bra .Lmt7_packets_block_0327
.Lmt7_packets_block_0316:
    tst #0x1,r0
    bf .Lmt7_packets_block_0317
    bra .Lmt7_packets_block_0340
    nop
.Lmt7_packets_block_0317:
    mov.l @r4+,r9
    fldi1 fr2
    fschg
    fldi0 fr3
    mov #0x3,r3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0318
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0318:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0319:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0324
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0320
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0320:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0322
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0321
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0321:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0319
    add #0x38,r6
.Lmt7_packets_block_0322:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0323
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0321
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0321
    add #-0x8,r14
.Lmt7_packets_block_0323:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0326
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0324:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0325
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0320
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0320
    add #-0x8,r14
.Lmt7_packets_block_0325:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0326:
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    add #-0x8,r4
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0327:
    mov.l @r4+,r9
    add #0x20,r6
    fschg
    mov #0x3,r3
    mov.l @r4,r0
    mov r4,r14
    fldi0 fr2
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0328
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0328:
    fmov.s @r14+,fr4
    mov r6,r2
    fmov.s @r14+,fr6
    fadd fr7,fr2
    fldi1 fr7
    fldi0 fr3
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0329:
    mov #0x40,r0
    add r15,r0
    mov.l @r0+,r1
    sts.l PR,@-r15
    jsr @r1
    fschg
    lds.l @r15+,PR
    fschg
    mov.l @r4+,r0
    dt r3
    flds fr15,FPUL
    fldi1 fr7
    fdiv fr4,fr7
    mov.l @r4+,r1
    bt/s .Lmt7_packets_block_0336
    fipr fv12,fv0
    tst #0x1,r0
    add r4,r1
    bt/s .Lmt7_packets_block_0330
    fsts FPUL,fr2
    mov r4,r1
    add #0x18,r4
    add #-0x8,r1
.Lmt7_packets_block_0330:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r14,fr0
    add #0x20,r4
    fmov.s @r1+,fr8
    bf .Lmt7_packets_block_0331
    fadd fr3,fr2
.Lmt7_packets_block_0331:
    fschg
    mov #0x44,r0
    fmov.s @(r0,r15),fr10
    fadd fr10,fr2
    fschg
    fldi0 fr3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r1+,fr10
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fadd fr11,fr2
    fldi1 fr11
    fmul fr7,fr5
    fmov.s @r1+,fr0
    add #0x20,r5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    add #0x40,r6
    mov #0x40,r0
    add r15,r0
    mov.l @r0+,r14
    sts.l PR,@-r15
    jsr @r14
    fschg
    lds.l @r15+,PR
    fschg
    dt r3
    mov.l @r4+,r0
    mov.l @r4+,r14
    flds fr15,FPUL
    fipr fv12,fv0
    fldi1 fr11
    bt/s .Lmt7_packets_block_0334
    fdiv fr8,fr11
    tst #0x1,r0
    add r4,r14
    bt/s .Lmt7_packets_block_0332
    fsts FPUL,fr2
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0332:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr0
    add #0x20,r4
    fmov.s @r14+,fr4
    bf .Lmt7_packets_block_0333
    fadd fr3,fr2
.Lmt7_packets_block_0333:
    fschg
    mov #0x44,r0
    fmov.s @(r0,r15),fr6
    fadd fr6,fr2
    fschg
    fldi0 fr3
    fmov.s fr2,@-r6
    fsub fr2,fr2
    pref @r4
    add #-0x20,r4
    fmov.s @r14+,fr6
    fmul fr11,fr10
    fmov.s fr0,@-r6
    fadd fr7,fr2
    fldi1 fr7
    fmul fr11,fr9
    fmov.s @r14+,fr0
    add #0x20,r5
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    bra .Lmt7_packets_block_0329
    add #0x40,r6
.Lmt7_packets_block_0334:
    dt r9
    fsts FPUL,fr2
    mov r6,r2
    bt/s .Lmt7_packets_block_0335
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0332
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0332
    add #-0x8,r14
.Lmt7_packets_block_0335:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    bra .Lmt7_packets_block_0338
    fmov fr10,fr6
.Lmt7_packets_block_0336:
    dt r9
    fsts FPUL,fr2
    mov r6,r2
    bt/s .Lmt7_packets_block_0337
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0330
    add r4,r1
    mov r4,r1
    add #0x18,r4
    bra .Lmt7_packets_block_0330
    add #-0x8,r1
    .word 0x0009
.Lmt7_packets_block_0337:
    add #0x30,r4
    mov r14,r1
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0338:
    fldi0 fr0
    fcmp/gt fr0,fr3
    fmov.s @r1,fr10
    bf .Lmt7_packets_block_0339
    fadd fr3,fr2
.Lmt7_packets_block_0339:
    fschg
    mov #0x44,r0
    fmov.s @(r0,r15),fr0
    fadd fr0,fr2
    fschg
    fldi0 fr3
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr10,@-r6
    add #0x20,r5
    fmov.s fr6,@-r6
    add #-0x8,r4
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0340:
    mov #0x7f,r2
    mov #0x3,r3
    mov.l @r4+,r9
    add #-0x20,r6
    mov.l @r4,r0
    add #0x7f,r2
    fschg
    add #0x37,r2
    tst #0x1,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0341
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0341:
    fmov.s @r14+,fr4
.Lmt7_packets_block_0342:
    fmov.s @r14+,fr6
    mov r6,r7
    fldi0 fr2
    fadd fr7,fr2
    fldi1 fr7
    add #0x40,r6
    fldi0 fr3
    add #0x20,r5
    fmov.s @r14+,fr0
    ftrv xmtrx,fv4
    fldi0 fr11
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0351
    mov r14,r1
    fipr fv12,fv0
    tst #0x1,r0
    mov.l @r4+,r14
    bf/s .Lmt7_packets_block_0345
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
.Lmt7_packets_block_0343:
    fcmp/gt fr11,fr3
    add r4,r14
    flds fr15,FPUL
    fipr fv0,fv8
    pref @r14
    bf/s .Lmt7_packets_block_0348
    fsts FPUL,fr2
    fadd fr3,fr2
    fldi0 fr3
    bra .Lmt7_packets_block_0347
    ftrc fr11,FPUL
    .word 0x0009
.Lmt7_packets_block_0344:
    add #-0x1c,r4
    fldi1 fr7
.Lmt7_packets_block_0345:
    fdiv fr4,fr7
.Lmt7_packets_block_0346:
    add #0x18,r4
    fcmp/gt fr11,fr3
    flds fr15,FPUL
    fipr fv0,fv8
    mov r4,r14
    add #-0x20,r14
    pref @r4
    bf/s .Lmt7_packets_block_0348
    fsts FPUL,fr2
    fadd fr3,fr2
    ftrc fr11,FPUL
    fldi0 fr3
.Lmt7_packets_block_0347:
    sts FPUL,r0
    cmp/gt r2,r0
    shll2 r0
    bf .Lmt7_packets_block_0349
    fschg
    fmov.s @(r0,r8),fr3
    bra .Lmt7_packets_block_0349
    fschg
    .word 0x0009
.Lmt7_packets_block_0348:
    fldi0 fr3
.Lmt7_packets_block_0349:
    fmov.s @r1,fr0
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr0,@-r6
    tst r3,r3
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0350
    mov.l r7,@r6
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0342
.Lmt7_packets_block_0350:
    pref @r6
    add #0x20,r6
    rts
    fschg
    .word 0x0009
.Lmt7_packets_block_0351:
    mov r6,r7
    shar r7
    dt r9
    bt/s .Lmt7_packets_block_0344
    fipr fv12,fv0
    mov.l @r4+,r14
    tst #0x1,r0
    mov #0x3,r3
    fldi1 fr7
    bf/s .Lmt7_packets_block_0346
    fdiv fr4,fr7
    bra .Lmt7_packets_block_0343
    pref @r4
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0352:
    mov #0x43,r0
    shll8 r0
    or #0x7f,r0
    shll16 r0
    lds r0,FPUL
    fsts FPUL,fr7
    fmul fr7,fr0
    add #-0x14,r4
    fmul fr7,fr1
    fmov.s @r4+,fr4
    fmul fr7,fr2
    fmov.s @r4+,fr5
    fmul fr7,fr3
    fmov.s @r4+,fr6
    ftrc fr0,FPUL
    sts FPUL,r0
    add #0x8,r4
    fldi1 fr0
    ftrc fr1,FPUL
    sts FPUL,r1
    ftrc fr2,FPUL
    sts FPUL,r2
    ftrc fr3,FPUL
    sts FPUL,r8
    fdiv fr6,fr0
    shll8 r0
    or r0,r1
    shll16 r1
    shll8 r2
    or r2,r8
    or r1,r8
    fmov.s fr15,@-r15
    fmov.s fr10,@-r15
    fmov.s fr9,@-r15
    fmul fr6,fr12
    fmov.s fr8,@-r15
    fmul fr6,fr13
    mov.l r13,@-r15
    mova .Lmt7_packets_block_0354,r0
    fmul fr6,fr14
    sts.l PR,@-r15
    fmov.s fr0,@r0
    fmul fr7,fr5
    fmul fr7,fr4
    ftrc fr5,FPUL
    sts FPUL,r0
    ftrc fr4,FPUL
    shll16 r0
    sts FPUL,r13
    shll8 r0
    or r0,r13
    mov.l @r4+,r0
.Lmt7_packets_block_0353:
    cmp/pl r0
    mov.l .Lmt7_packets_block_0363,r9
    bt/s .Lmt7_packets_block_0355
    cmp/pz r0
    mov r0,r8
    mov r11,r0
    mov.l r5,@(r0,r10)
    mova .Lmt7_packets_block_0354,r0
    lds.l @r15+,PR
    fmov.s @r0,fr0
    mov.l @r15+,r13
    fmul fr0,fr12
    fmov.s @r15+,fr8
    fmul fr0,fr13
    fmov.s @r15+,fr9
    fmul fr0,fr14
    fmov.s @r15+,fr10
    rts
    fmov.s @r15+,fr15
    .word 0x0009
.Lmt7_packets_block_0354:
    .long 0x00000000
.Lmt7_packets_block_0355:
    mov r0,r1
    mov.l @r9+,r3
    add #0x20,r4
    mov #-0x31,r2
    and r2,r3
    mov r15,r2
    add #0x18,r2
    mov.l @(0x28,r2),r2
    cmp/pz r2
    bt/s .Lmt7_packets_block_0356
    mov #0x1b,r2
    and #0x3,r0
    xor #0x1,r0
.Lmt7_packets_block_0356:
    lds r3,FPUL
    shld r2,r0
    mov #0x3,r3
    fschg
    shld r2,r3
    pref @r4
    not r3,r3
    mov.l @r9+,r2
    and r3,r2
    fsts FPUL,fr0
    or r0,r2
    lds r2,FPUL
    add #-0x20,r4
    fsts FPUL,fr1
    add #0x10,r6
    fmov.s @r9+,fr2
    fmov.s fr2,@-r6
    add #0x20,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    add #0x20,r6
    rotcr r12
    mov r1,r0
    bt/s .Lmt7_packets_block_0358
    shll r12
    tst #0x8,r0
    bt .Lmt7_packets_block_0357
    bsr .Lmt7_packets_block_0376
    nop
    bra .Lmt7_packets_block_0353
    mov.l @r4+,r0
.Lmt7_packets_block_0357:
    bsr .Lmt7_packets_block_0364
    nop
    bra .Lmt7_packets_block_0353
    mov.l @r4+,r0
    .word 0x0009
.Lmt7_packets_block_0358:
    mov #0x0,r1
    tst #0x40,r0
    addc r1,r12
    tst #0x10,r0
    bt/s .Lmt7_packets_block_0359
    tst #0x8,r0
    bsr .Lmt7_packets_block_0387
    nop
    bra .Lmt7_packets_block_0353
    mov.l @r4+,r0
.Lmt7_packets_block_0359:
    bt .Lmt7_packets_block_0360
    bsr .Lmt7_packets_block_0417
    nop
    bra .Lmt7_packets_block_0353
    mov.l @r4+,r0
    .word 0x0009
.Lmt7_packets_block_0360:
    bra .Lmt7_packets_block_0360
    nop
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0361:
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0362:
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0363:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0364:
    mov.l @r4+,r3
    add #0x20,r6
.Lmt7_packets_block_0365:
    fmov.s @r4+,fr4
    fmov.s @r4+,fr5
    fmov.s @r4+,fr6
    fldi1 fr7
    fmov.s @r4+,fr2
    fmov.s @r4+,fr0
    fmov.s @r4+,fr1
    ftrv xmtrx,fv4
    fldi0 fr3
    fmov.s @r4+,fr10
    mov r13,r0
    fmov.s @r4+,fr8
    shlr16 r0
    fmov.s @r4+,fr9
    fipr fv12,fv0
    fmov.s @r4+,fr2
    fsub fr15,fr15
    fldi1 fr7
    fdiv fr4,fr7
    fmov.s @r4+,fr0
    shll16 r0
    fldi0 fr11
    fcmp/gt fr11,fr3
    fmov.s @r4+,fr1
    add #0x20,r4
    fipr fv12,fv8
    pref @r4
    bt .Lmt7_packets_block_0366
    fldi0 fr3
.Lmt7_packets_block_0366:
    fmov fr3,fr8
    fmul fr8,fr8
    fldi1 fr9
    fipr fv0,fv12
    fldi0 fr10
    fcmp/gt fr11,fr10
    mov r13,r2
    fsub fr8,fr9
    extu.b r2,r2
    bf .Lmt7_packets_block_0367
    fneg fr11
.Lmt7_packets_block_0367:
    movt r0
    fcmp/gt fr15,fr10
    lds r2,FPUL
    fsqrt fr9
    bf .Lmt7_packets_block_0368
    fneg fr15
.Lmt7_packets_block_0368:
    movt r1
    fcmp/gt fr11,fr15
    fmov fr15,fr10
    shll2 r1
    bf .Lmt7_packets_block_0369
    fmov fr11,fr15
    or #0x2,r0
    fmov fr10,fr11
.Lmt7_packets_block_0369:
    or r0,r1
    float FPUL,fr10
    shll r1
    mova .Lmt7_packets_block_0375,r0
    mov.w @(r0,r1),r1
    mova .Lmt7_packets_block_0373,r0
    fmov.s @r0+,fr0
    fdiv fr11,fr15
    shlr r1
    fmov.s @r0+,fr11
    fmul fr10,fr3
    mov.l @r0,r2
    mov r13,r0
    shlr16 r0
    fmul fr10,fr9
    shll16 r0
    ftrc fr3,FPUL
    or r0,r1
    sts FPUL,r0
    add #0x20,r4
    ftrc fr9,FPUL
    pref @r4
    shll16 r0
    add r0,r1
    sts FPUL,r0
    add #-0x40,r4
    fmac fr0,fr15,fr11
    shll8 r0
    ftrc fr11,FPUL
    fldi0 fr15
    add r0,r1
    sts FPUL,r0
    and #0xff,r0
    mov.b @(r0,r2),r0
    bf .Lmt7_packets_block_0370
    neg r0,r0
.Lmt7_packets_block_0370:
    add r1,r0
    mov.l r0,@-r6
    fschg
    mov.l @r4+,r1
    fmul fr7,fr6
    mov.l @r4+,r2
    fmul fr7,fr5
    mov.l r8,@-r6
    mov r0,r14
    mov.l r2,@-r6
    mov r6,r0
    mov.l r1,@-r6
    shar r0
    fmov.s fr6,@-r6
    dt r3
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0371
    fschg
    mov.l r6,@r6
    add #0x20,r5
    pref @r6
    bra .Lmt7_packets_block_0365
    add #0x40,r6
.Lmt7_packets_block_0371:
    mov.l r0,@r6
    add #0x20,r5
    mov.l @r4+,r0
    pref @r6
    add #0x40,r6
    cmp/pl r0
    bf/s .Lmt7_packets_block_0372
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0365
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0372:
    add #-0x4,r4
    rts
    add #-0x20,r6
    .word 0xf79d
    .word 0x7438
    .word 0xf3fd
    .word 0x0483
    .word 0xf743
    .word 0x26e6
    .word 0x7420
    .word 0x2686
    .word 0xf672
    .word 0x0483
    .word 0x74c0
    .word 0xf572
    .word 0xf049
    .word 0x6063
    .word 0xf60b
    .word 0x4021
    .word 0xf66b
    .word 0x4310
    .word 0xf64b
    .word 0x8ddd
    .word 0xf3fd
    .word 0x2662
    .word 0x7520
    .word 0x0683
    .word 0xaf67
    .word 0x7640
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0373:
    .long 0x43000000
    .word 0x0000
    .word 0x3f00
.Lmt7_packets_block_0374:
    .long __Q37shenmue2dc3sdk.g_mt7VertexAngleLookup
    .word 0x0000
    .word 0x3f00
.Lmt7_packets_block_0375:
    .long 0x01010000
    .word 0x0081
    .word 0x0080
    .word 0x01ff
    .word 0x0100
    .word 0x0180
    .word 0x0181
.Lmt7_packets_block_0376:
    mov.l @r4+,r9
    add #0x20,r6
    mov #0x3,r3
.Lmt7_packets_block_0377:
    fmov.s @r4+,fr4
    fmov.s @r4+,fr5
    fmov.s @r4+,fr6
    fldi1 fr7
    fmov.s @r4+,fr2
    fmov.s @r4+,fr0
    fmov.s @r4+,fr1
    ftrv xmtrx,fv4
    fldi0 fr3
    fmov.s @r4+,fr10
    mov r13,r0
    fmov.s @r4+,fr8
    shlr16 r0
    fmov.s @r4+,fr9
    fipr fv12,fv0
    fmov.s @r4+,fr2
    fsub fr15,fr15
    fldi1 fr7
    fdiv fr4,fr7
    fmov.s @r4+,fr0
    shll16 r0
    fldi0 fr11
    fcmp/gt fr11,fr3
    fmov.s @r4+,fr1
    add #0x20,r4
    fipr fv12,fv8
    pref @r4
    bt .Lmt7_packets_block_0378
    fldi0 fr3
.Lmt7_packets_block_0378:
    fmov fr3,fr8
    fmul fr8,fr8
    fldi1 fr9
    fipr fv0,fv12
    fldi0 fr10
    fcmp/gt fr11,fr10
    mov r13,r2
    fsub fr8,fr9
    extu.b r2,r2
    bf .Lmt7_packets_block_0379
    fneg fr11
.Lmt7_packets_block_0379:
    movt r0
    fcmp/gt fr15,fr10
    lds r2,FPUL
    fsqrt fr9
    bf .Lmt7_packets_block_0380
    fneg fr15
.Lmt7_packets_block_0380:
    movt r1
    fcmp/gt fr11,fr15
    fmov fr15,fr10
    shll2 r1
    bf .Lmt7_packets_block_0381
    fmov fr11,fr15
    or #0x2,r0
    fmov fr10,fr11
.Lmt7_packets_block_0381:
    or r0,r1
    float FPUL,fr10
    shll r1
    mova .Lmt7_packets_block_0386,r0
    mov.w @(r0,r1),r1
    mova .Lmt7_packets_block_0384,r0
    fmov.s @r0+,fr0
    fdiv fr11,fr15
    shlr r1
    fmov.s @r0+,fr11
    fmul fr10,fr3
    mov.l @r0,r2
    mov r13,r0
    shlr16 r0
    fmul fr10,fr9
    shll16 r0
    ftrc fr3,FPUL
    or r0,r1
    sts FPUL,r0
    add #0x20,r4
    ftrc fr9,FPUL
    pref @r4
    shll16 r0
    add r0,r1
    sts FPUL,r0
    add #-0x40,r4
    fmac fr0,fr15,fr11
    shll8 r0
    ftrc fr11,FPUL
    fldi0 fr15
    add r0,r1
    sts FPUL,r0
    and #0xff,r0
    mov.b @(r0,r2),r0
    bf .Lmt7_packets_block_0382
    neg r0,r0
.Lmt7_packets_block_0382:
    add r1,r0
    mov.l r0,@-r6
    fschg
    mov.l @r4+,r1
    fmul fr7,fr6
    mov.l @r4+,r2
    fmul fr7,fr5
    mov.l r8,@-r6
    mov r0,r14
    mov.l r2,@-r6
    mov r6,r0
    mov.l r1,@-r6
    shar r0
    fmov.s fr6,@-r6
    dt r3
    fmov.s fr4,@-r6
    bt/s .Lmt7_packets_block_0383
    fschg
    mov.l r6,@r6
    add #0x20,r5
    pref @r6
    bra .Lmt7_packets_block_0377
    add #0x40,r6
.Lmt7_packets_block_0383:
    add #0x20,r5
    mov.l r0,@r6
    dt r9
    pref @r6
    add #0x40,r6
    bf/s .Lmt7_packets_block_0377
    mov #0x3,r3
    rts
    add #-0x20,r6
    .word 0xf79d
    .word 0x7438
    .word 0xf3fd
    .word 0x0483
    .word 0xf743
    .word 0x26e6
    .word 0x7420
    .word 0x2686
    .word 0xf672
    .word 0x0483
    .word 0x74c0
    .word 0xf572
    .word 0xf049
    .word 0x6063
    .word 0xf60b
    .word 0x4021
    .word 0xf66b
    .word 0x4310
    .word 0xf64b
    .word 0x8de2
    .word 0xf3fd
    .word 0x2662
    .word 0x7520
    .word 0x0683
    .word 0xaf6c
    .word 0x7640
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0384:
    .long 0x43000000
    .word 0x0000
    .word 0x3f00
.Lmt7_packets_block_0385:
    .long __Q37shenmue2dc3sdk.g_mt7VertexAngleLookup
    .word 0x0000
    .word 0x3f00
.Lmt7_packets_block_0386:
    .long 0x01010000
    .word 0x0081
    .word 0x0080
    .word 0x01ff
    .word 0x0100
    .word 0x0180
    .word 0x0181
.Lmt7_packets_block_0387:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0388:
    mov #0x0,r11
    bsr .Lmt7_packets_block_0399
    mov #0x0,r9
    bt/s .Lmt7_packets_block_0390
    rotcl r9
    bsr .Lmt7_packets_block_0408
    add #0x1,r11
    bsr .Lmt7_packets_block_0399
    nop
    bt/s .Lmt7_packets_block_0394
    rotcl r9
    bsr .Lmt7_packets_block_0408
    add #0x1,r11
.Lmt7_packets_block_0389:
    bsr .Lmt7_packets_block_0399
    nop
    bt/s .Lmt7_packets_block_0396
    rotcl r9
    bsr .Lmt7_packets_block_0408
    add #0x1,r11
    bf .Lmt7_packets_block_0389
    bra .Lmt7_packets_block_0392
    nop
.Lmt7_packets_block_0390:
    dt r3
    add #0x1,r11
    bsr .Lmt7_packets_block_0399
    dt r3
    bf/s .Lmt7_packets_block_0395
    rotcl r9
.Lmt7_packets_block_0391:
    bsr .Lmt7_packets_block_0399
    add #0x1,r11
    bf/s .Lmt7_packets_block_0396
    rotcl r9
    dt r3
    bf .Lmt7_packets_block_0391
.Lmt7_packets_block_0392:
    mov.l @r4+,r0
    cmp/pl r0
    bf/s .Lmt7_packets_block_0393
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0388
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0393:
    lds.l @r15+,PR
    mov.l @r15+,r11
    mov #0x1,r0
    mov.l @r15+,r10
    add #-0x4,r4
    rts
    or r0,r12
.Lmt7_packets_block_0394:
    bsr .Lmt7_packets_block_0410
    dt r3
    bsr .Lmt7_packets_block_0399
    add #-0x70,r4
    bsr .Lmt7_packets_block_0410
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0413
    mov #0x0,r0
    bra .Lmt7_packets_block_0397
    add #0x38,r4
.Lmt7_packets_block_0395:
    bsr .Lmt7_packets_block_0410
    mov #0x1,r11
    bsr .Lmt7_packets_block_0399
    add #-0x70,r4
    bsr .Lmt7_packets_block_0410
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0413
    mov #0x0,r0
    clrt
    bsr .Lmt7_packets_block_0411
    mov #0x1,r0
    bra .Lmt7_packets_block_0397
    add #0x38,r4
.Lmt7_packets_block_0396:
    bsr .Lmt7_packets_block_0410
    nop
    add #-0x38,r4
    bsr .Lmt7_packets_block_0399
    add #-0x70,r4
    bsr .Lmt7_packets_block_0410
    add #-0x2,r11
    bsr .Lmt7_packets_block_0399
    nop
    bsr .Lmt7_packets_block_0410
    add #0x1,r11
    add #0x38,r4
    add #0x1,r11
    bra .Lmt7_packets_block_0398
    mov r9,r0
.Lmt7_packets_block_0397:
    bsr .Lmt7_packets_block_0399
    add #0x1,r11
    bsr .Lmt7_packets_block_0410
    rotcl r9
    mov r9,r0
.Lmt7_packets_block_0398:
    and #0x7,r0
    shll2 r0
    braf r0
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa00c
    .word 0x0009
    .word 0xa019
    .word 0x0009
    .word 0xa024
    .word 0x0009
    .word 0xa02a
    .word 0x0009
    .word 0xa038
    .word 0x0009
    .word 0xa03d
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb125
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb101
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb11d
    .word 0x70ff
    .word 0x8bd9
    .word 0xafa0
    .word 0x0009
    .word 0x0008
    .word 0xb0f7
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb113
    .word 0x70ff
    .word 0x4310
    .word 0xb0f0
    .word 0x60b3
    .word 0x8bcc
    .word 0xaf93
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb109
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf85
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb101
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0dd
    .word 0x70ff
    .word 0x4310
    .word 0xb0da
    .word 0x60b3
    .word 0x8901
    .word 0xaf68
    .word 0x7b01
    .word 0xaf7b
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb0f1
    .word 0x70ff
    .word 0x8bad
    .word 0xaf74
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x8903
    .word 0x60b3
    .word 0x0008
    .word 0xb0e7
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb0e3
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0df
    .word 0x70ff
    .word 0x4310
    .word 0xb0bc
    .word 0x60b3
    .word 0x8b98
    .word 0xaf5f
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0399:
    fmov.s @r4+,fr4
    fmov.s @r4+,fr5
    fmov.s @r4+,fr6
    fldi1 fr7
    fmov.s @r4+,fr2
    fmov.s @r4+,fr0
    fmov.s @r4+,fr1
    ftrv xmtrx,fv4
    fldi0 fr3
    fmov.s @r4+,fr10
    mov r13,r0
    fmov.s @r4+,fr8
    shlr16 r0
    fmov.s @r4+,fr9
    fipr fv12,fv0
    fmov.s @r4+,fr2
    fsub fr15,fr15
    fmov.s @r4+,fr0
    shll16 r0
    flds fr7,FPUL
    fldi1 fr7
    fdiv fr4,fr7
    fldi0 fr11
    fcmp/gt fr11,fr3
    fsts FPUL,fr4
    fipr fv12,fv8
    fmov.s @r4+,fr1
    bt .Lmt7_packets_block_0400
    fldi0 fr3
.Lmt7_packets_block_0400:
    fmov fr3,fr8
    fmul fr8,fr8
    fldi1 fr9
    fipr fv0,fv12
    fldi0 fr10
    fcmp/gt fr11,fr10
    mov r13,r2
    fsub fr8,fr9
    extu.b r2,r2
    bf .Lmt7_packets_block_0401
    fneg fr11
.Lmt7_packets_block_0401:
    movt r0
    fcmp/gt fr15,fr10
    lds r2,FPUL
    fsqrt fr9
    bf .Lmt7_packets_block_0402
    fneg fr15
.Lmt7_packets_block_0402:
    movt r1
    fcmp/gt fr11,fr15
    fmov fr15,fr10
    shll2 r1
    bf .Lmt7_packets_block_0403
    fmov fr11,fr15
    or #0x2,r0
    fmov fr10,fr11
.Lmt7_packets_block_0403:
    or r0,r1
    float FPUL,fr10
    shll r1
    mova .Lmt7_packets_block_0407,r0
    mov.w @(r0,r1),r1
    mova .Lmt7_packets_block_0405,r0
    fmov.s @r0+,fr0
    fdiv fr11,fr15
    shlr r1
    fmov.s @r0+,fr11
    fmul fr10,fr3
    mov.l @r0,r2
    mov r13,r0
    shlr16 r0
    fmul fr10,fr9
    shll16 r0
    ftrc fr3,FPUL
    or r0,r1
    sts FPUL,r0
    add #0x20,r4
    ftrc fr9,FPUL
    pref @r4
    shll16 r0
    add r0,r1
    sts FPUL,r0
    add #0x20,r4
    fmac fr0,fr15,fr11
    pref @r4
    ftrc fr11,FPUL
    add #-0x40,r4
    shll8 r0
    fldi0 fr15
    add r0,r1
    sts FPUL,r0
    and #0xff,r0
    flds fr4,FPUL
    mov.b @(r0,r2),r0
    bf .Lmt7_packets_block_0404
    neg r0,r0
.Lmt7_packets_block_0404:
    add r1,r0
    sts FPUL,r2
    mov r0,r1
    fmov.s @r4+,fr2
    fmov.s @r4+,fr3
    rts
    cmp/hi r2,r12
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0405:
    .long 0x43000000
    .word 0x0000
    .word 0x3f00
.Lmt7_packets_block_0406:
    .long __Q37shenmue2dc3sdk.g_mt7VertexAngleLookup
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0407:
    .long 0x01010000
    .word 0x0081
    .word 0x0080
    .word 0x01ff
    .word 0x0100
    .word 0x0180
    .word 0x0181
.Lmt7_packets_block_0408:
    fschg
    add #0x20,r6
    mov.l r1,@-r6
    fmul fr7,fr6
    fmul fr7,fr5
    mov.l r8,@-r6
    mov r6,r0
    fmov.s fr2,@-r6
    shar r0
    fmov.s fr6,@-r6
    dt r3
    fmov.s fr4,@-r6
    bt .Lmt7_packets_block_0409
    mov r6,r0
.Lmt7_packets_block_0409:
    mov.l r0,@r6
    fschg
    add #0x20,r5
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0410:
    mov r11,r0
    mov #0x5,r10
    and #0x3,r0
    shld r10,r0
    mov.l .Lmt7_packets_block_0416,r10
    fschg
    add r0,r10
    mov.l r1,@-r10
    mov.l r8,@-r10
    fmov.s fr2,@-r10
    fmov.s fr6,@-r10
    fmov.s fr4,@-r10
    rts
    fschg
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0411:
    mov #0x5,r1
    mov.l .Lmt7_packets_block_0415,r10
    and #0x3,r0
    fschg
    shld r1,r0
    add r0,r10
    mov #-0x10,r2
    fmov.s @r10+,fr0
    shll16 r2
    fmov.s @r10+,fr2
    add #0x20,r6
    fmov.s @r10+,fr4
    fmul fr3,fr2
    fmov.s @r10+,fr6
    fmul fr3,fr1
    shll8 r2
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    bt .Lmt7_packets_block_0412
    mov r6,r2
.Lmt7_packets_block_0412:
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0413:
    mov #0x5,r1
    mov.l .Lmt7_packets_block_0415,r10
    and #0x3,r0
    shld r1,r0
    mov r10,r2
    add r0,r10
    mov r11,r0
    and #0x3,r0
    fmov.s @r10,fr2
    shld r1,r0
    lds r12,FPUL
    add r0,r2
    fsts FPUL,fr0
    fmov.s @r2,fr1
    fsub fr2,fr0
    add #0x18,r2
    fschg
    add #0x20,r6
    fsub fr2,fr1
    fmov.s @r2,fr4
    add #-0x8,r2
    lds r7,FPUL
    add #0x10,r10
    fdiv fr1,fr0
    fmov.s fr4,@-r6
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsts FPUL,fr1
    fsub fr2,fr4
    fsub fr3,fr5
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fschg
    fmov.s fr1,@-r6
    fmov.s @r2,fr5
    add #-0x4,r2
    fmov.s @r10,fr3
    add #-0x4,r10
    fmov.s @r2,fr4
    fmov.s @r10,fr2
    fsub fr3,fr5
    fsub fr2,fr4
    mov #-0x10,r2
    fmac fr0,fr5,fr3
    shll16 r2
    fmac fr0,fr4,fr2
    shll8 r2
    fmul fr1,fr3
    add #0x20,r5
    fmul fr1,fr2
    fmov.s fr3,@-r6
    fmov.s fr2,@-r6
    bt .Lmt7_packets_block_0414
    mov r6,r2
.Lmt7_packets_block_0414:
    mov.l r2,@-r6
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
.Lmt7_packets_block_0415:
    .long .Lmt7_packets_block_0361
.Lmt7_packets_block_0416:
    .long .Lmt7_packets_block_0362
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0417:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0418:
    bsr .Lmt7_packets_block_0422
    mov #0x0,r9
    mov r9,r0
    cmp/eq #0x0,r0
    bt .Lmt7_packets_block_0420
    cmp/eq #0x7,r0
    bt .Lmt7_packets_block_0421
    bsr .Lmt7_packets_block_0399
    mov #0x0,r11
    bsr .Lmt7_packets_block_0410
    nop
    bsr .Lmt7_packets_block_0399
    mov #0x1,r11
    bsr .Lmt7_packets_block_0410
    nop
    bsr .Lmt7_packets_block_0399
    mov #0x2,r11
    bsr .Lmt7_packets_block_0410
    nop
    shll2 r9
    braf r9
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa026
    .word 0x0009
    .word 0xa033
    .word 0x0009
    .word 0xa041
    .word 0x0009
    .word 0xa04c
    .word 0x0009
    .word 0xa05a
    .word 0x0009
    .word 0xa065
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0419:
    dt r3
    bf .Lmt7_packets_block_0418
    mov #0x1,r0
    lds.l @r15+,PR
    or r0,r12
    mov.l @r15+,r11
    rts
    mov.l @r15+,r10
.Lmt7_packets_block_0420:
    mov r3,r9
    bsr .Lmt7_packets_block_0399
    mov #0x3,r3
    bsr .Lmt7_packets_block_0408
    nop
    bsr .Lmt7_packets_block_0399
    nop
    bsr .Lmt7_packets_block_0408
    nop
    bsr .Lmt7_packets_block_0399
    nop
    bsr .Lmt7_packets_block_0408
    nop
    bra .Lmt7_packets_block_0419
    mov r9,r3
.Lmt7_packets_block_0421:
    add #0x38,r4
    bra .Lmt7_packets_block_0419
    add #0x70,r4
    .word 0x0008
    .word 0xbf49
    .word 0xe000
    .word 0x0008
    .word 0xbf46
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf62
    .word 0xe000
    .word 0x0018
    .word 0xbf5f
    .word 0xe001
    .word 0xafd7
    .word 0x0009
    .word 0x0008
    .word 0xbf3a
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf56
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbf32
    .word 0xe002
    .word 0x0018
    .word 0xbf4f
    .word 0xe001
    .word 0xafc7
    .word 0x0009
    .word 0x0008
    .word 0xbf2a
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf46
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbf42
    .word 0xe000
    .word 0xafba
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf3c
    .word 0xe000
    .word 0x0008
    .word 0xbf19
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf35
    .word 0xe000
    .word 0x0018
    .word 0xbf12
    .word 0xe002
    .word 0xafaa
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf2c
    .word 0xe000
    .word 0x0008
    .word 0xbf09
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbf25
    .word 0xe001
    .word 0xaf9d
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbf1f
    .word 0xe000
    .word 0x0008
    .word 0xbf1c
    .word 0xe001
    .word 0x0018
    .word 0xbef9
    .word 0xe002
    .word 0xaf91
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0422:
    fmov.s @r4+,fr4
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
    add #0x30,r4
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv4
    fmov.s @r4+,fr0
    fmov.s @r4+,fr1
    fmov.s @r4,fr2
    fldi1 fr3
    add #0x30,r4
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv0
    flds fr7,FPUL
    sts FPUL,r0
    fmov.s @r4+,fr4
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    cmp/hi r0,r12
    fldi1 fr7
    add #-0x78,r4
    ftrv xmtrx,fv4
    rotcl r9
    flds fr3,FPUL
    sts FPUL,r0
    cmp/hi r0,r12
    flds fr7,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    rts
    rotcl r9
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0423:
    mov.l .Lmt7_packets_block_0433,r2
    mova .Lmt7_packets_block_0432,r0
    mov.l @r2,r2
    mov r15,r1
    fmov.s @r0,fr0
    mov r2,r0
    fmov.s fr15,@-r15
    cmp/eq #0x0,r0
    fmov.s fr14,@-r15
    bt/s .Lmt7_packets_block_0424
    fmov.s fr13,@-r15
    jmp @r0
.Lmt7_packets_block_0424:
    fmov.s fr12,@-r15
    mov #0x0,r0
    fmov.s @(r0,r1),fr12
    mov #0x18,r0
    fmov.s @(r0,r1),fr13
    mov #0x1c,r0
    fmov.s @(r0,r1),fr14
    mov #0x20,r0
    fmov.s @(r0,r1),fr15
    fmov.s fr11,@-r15
    fmul fr0,fr12
    fmov.s fr10,@-r15
    fmul fr0,fr13
    fmov.s fr9,@-r15
    fmul fr0,fr14
    fmov.s fr8,@-r15
    fmul fr0,fr15
    sts.l PR,@-r15
    mov.l .Lmt7_packets_block_0437,r0
    mov.l r0,@-r15
    mov.l @r4+,r0
.Lmt7_packets_block_0425:
    cmp/pl r0
    mov.l @r15,r9
    bt/s .Lmt7_packets_block_0426
    cmp/pz r0
    mov r0,r8
    mov r11,r0
    mov.l r5,@(r0,r10)
    add #0x4,r15
    lds.l @r15+,PR
    fmov.s @r15+,fr8
    fmov.s @r15+,fr9
    fmov.s @r15+,fr10
    fmov.s @r15+,fr11
    fmov.s @r15+,fr12
    fmov.s @r15+,fr13
    fmov.s @r15+,fr14
    rts
    fmov.s @r15+,fr15
    .word 0x0009
.Lmt7_packets_block_0426:
    mov r0,r1
    mov #-0x5,r2
    mov.l @r9+,r3
    and #0x40,r0
    shld r2,r0
    mov #-0x3,r2
    and r2,r3
    or r0,r3
    add #0x20,r4
    mov #-0x21,r2
    and r2,r3
    mov #0x10,r2
    or r2,r3
    mov r1,r0
    mov r15,r2
    add #0x28,r2
    lds r3,FPUL
    mov.l @(0x28,r2),r2
    cmp/pz r2
    mov #0x1b,r2
    bt/s .Lmt7_packets_block_0427
    and #0x3,r0
    xor #0x1,r0
.Lmt7_packets_block_0427:
    shld r2,r0
    mov #0x3,r3
    fschg
    shld r2,r3
    pref @r4
    not r3,r3
    mov.l @r9+,r2
    and r3,r2
    fsts FPUL,fr0
    or r0,r2
    lds r2,FPUL
    add #-0x20,r4
    fsts FPUL,fr1
    add #0x10,r6
    fmov.s @r9+,fr2
    fmov.s fr2,@-r6
    add #0x20,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    add #0x20,r6
    rotcr r12
    mov r1,r0
    bt/s .Lmt7_packets_block_0429
    shll r12
    tst #0x8,r0
    bt .Lmt7_packets_block_0428
    bsr .Lmt7_packets_block_0447
    nop
    bra .Lmt7_packets_block_0425
    mov.l @r4+,r0
.Lmt7_packets_block_0428:
    bsr .Lmt7_packets_block_0438
    nop
    bra .Lmt7_packets_block_0425
    mov.l @r4+,r0
.Lmt7_packets_block_0429:
    mov #0x0,r1
    tst #0x40,r0
    addc r1,r12
    tst #0x10,r0
    bt/s .Lmt7_packets_block_0430
    tst #0x8,r0
    bsr .Lmt7_packets_block_0455
    nop
    bra .Lmt7_packets_block_0425
    mov.l @r4+,r0
.Lmt7_packets_block_0430:
    bt .Lmt7_packets_block_0431
    bsr .Lmt7_packets_block_0497
    nop
    bra .Lmt7_packets_block_0425
    mov.l @r4+,r0
    .word 0x0009
.Lmt7_packets_block_0431:
    bra .Lmt7_packets_block_0431
    nop
.Lmt7_packets_block_0432:
    .long 0x3b808080
.Lmt7_packets_block_0433:
    .long __Q37shenmue2dc3sdk.g_mt7VertexColourCallback
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0434:
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0435:
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0436:
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
.Lmt7_packets_block_0437:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0438:
    mov.l @r4+,r3
    add #0x20,r6
    fschg
.Lmt7_packets_block_0439:
    mov.l @r4,r0
    tst #0x1,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0440
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0440:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    mov r14,r1
    fldi1 fr7
    add #0x10,r1
    mov.b @(0x7,r14),r0
    mov.l @(0x8,r14),r8
    cmp/pl r0
    extu.b r8,r0
    bf/s .Lmt7_packets_block_0442
    ftrv xmtrx,fv4
    mov.l @(0x4,r14),r2
    exts.b r2,r0
    shlr8 r2
    lds r0,FPUL
    exts.b r2,r0
    float FPUL,fr1
    shlr8 r2
    lds r0,FPUL
    exts.b r2,r0
    float FPUL,fr0
    lds r0,FPUL
    float FPUL,fr2
    fldi0 fr3
    mov r15,r2
    fschg
    add #0x18,r2
    fmov.s @r2+,fr8
    mov #0x3c,r0
    fmov.s @r2+,fr9
    shll8 r0
    fmov.s @r2+,fr10
    or #0x1,r0
    fipr fv8,fv0
    shll8 r0
    fmov.s @r2+,fr2
    or #0x2,r0
    fldi0 fr0
    shll8 r0
    fschg
    or #0x4,r0
    lds r0,FPUL
    extu.b r8,r0
    fcmp/gt fr0,fr3
    fsts FPUL,fr11
    bf .Lmt7_packets_block_0441
    fmul fr11,fr3
    fadd fr3,fr2
.Lmt7_packets_block_0441:
    sett
.Lmt7_packets_block_0442:
    add #0x20,r4
    lds r0,FPUL
    shlr8 r8
    extu.b r8,r0
    pref @r4
    shlr8 r8
    float FPUL,fr11
    add #-0x20,r4
    fldi1 fr7
    fdiv fr4,fr7
    lds r0,FPUL
    float FPUL,fr10
    extu.b r8,r0
    lds r0,FPUL
    shlr8 r8
    float FPUL,fr9
    lds r8,FPUL
    add #-0x8,r6
    float FPUL,fr8
    fmov.s @r1,fr0
    mov r6,r0
    fldi0 fr3
    fmul fr15,fr11
    bf/s .Lmt7_packets_block_0443
    fmov.s fr0,@-r6
    fmul fr2,fr9
    fmul fr2,fr10
    fmul fr2,fr11
.Lmt7_packets_block_0443:
    shar r0
    fldi0 fr2
    dt r3
    fmul fr7,fr6
    bt .Lmt7_packets_block_0444
    mov r6,r0
.Lmt7_packets_block_0444:
    fmul fr7,fr5
    fmov.s fr6,@-r6
    add #0x40,r5
    fmov.s fr4,@-r6
    fmul fr14,fr10
    mov.l r0,@r6
    pref @r6
    mov r4,r14
    mov.l @(0x4,r14),r14
    add #0x40,r6
    mov.l @r4+,r0
    fmul fr13,fr9
    fmov.s fr2,@-r6
    fmul fr12,fr8
    fmov.s fr2,@-r6
    add r4,r14
    fmov.s fr10,@-r6
    add #0x4,r14
    fmov.s fr8,@-r6
    bt/s .Lmt7_packets_block_0445
    pref @r6
    tst #0x1,r0
    add #0x40,r6
    bt/s .Lmt7_packets_block_0440
    add #0x4,r4
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0440
    add #-0x8,r14
.Lmt7_packets_block_0445:
    add #0x40,r6
    cmp/pl r0
    bf/s .Lmt7_packets_block_0446
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0439
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0446:
    fschg
    add #-0x4,r4
    rts
    add #-0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0447:
    mov.l @r4+,r9
    add #0x20,r6
    mov #0x3,r3
    fschg
.Lmt7_packets_block_0448:
    mov.l @r4,r0
    tst #0x1,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0449
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0449:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    mov r14,r1
    fldi1 fr7
    add #0x10,r1
    mov.b @(0x7,r14),r0
    mov.l @(0x8,r14),r8
    cmp/pl r0
    extu.b r8,r0
    bf/s .Lmt7_packets_block_0451
    ftrv xmtrx,fv4
    mov.l @(0x4,r14),r2
    exts.b r2,r0
    shlr8 r2
    lds r0,FPUL
    exts.b r2,r0
    float FPUL,fr1
    shlr8 r2
    lds r0,FPUL
    exts.b r2,r0
    float FPUL,fr0
    lds r0,FPUL
    float FPUL,fr2
    fldi0 fr3
    mov r15,r2
    fschg
    add #0x18,r2
    fmov.s @r2+,fr8
    mov #0x3c,r0
    fmov.s @r2+,fr9
    shll8 r0
    fmov.s @r2+,fr10
    or #0x1,r0
    fipr fv8,fv0
    shll8 r0
    fmov.s @r2+,fr2
    or #0x2,r0
    fldi0 fr0
    shll8 r0
    fschg
    or #0x4,r0
    lds r0,FPUL
    extu.b r8,r0
    fcmp/gt fr0,fr3
    fsts FPUL,fr11
    bf .Lmt7_packets_block_0450
    fmul fr11,fr3
    fadd fr3,fr2
.Lmt7_packets_block_0450:
    sett
.Lmt7_packets_block_0451:
    add #0x20,r4
    lds r0,FPUL
    shlr8 r8
    extu.b r8,r0
    pref @r4
    shlr8 r8
    float FPUL,fr11
    add #-0x20,r4
    fldi1 fr7
    fdiv fr4,fr7
    lds r0,FPUL
    float FPUL,fr10
    extu.b r8,r0
    lds r0,FPUL
    shlr8 r8
    float FPUL,fr9
    lds r8,FPUL
    add #-0x8,r6
    float FPUL,fr8
    fmov.s @r1,fr0
    mov r6,r0
    fldi0 fr3
    fmul fr15,fr11
    bf/s .Lmt7_packets_block_0452
    fmov.s fr0,@-r6
    fmul fr2,fr9
    fmul fr2,fr10
    fmul fr2,fr11
.Lmt7_packets_block_0452:
    shar r0
    fldi0 fr2
    dt r3
    fmul fr7,fr6
    bt .Lmt7_packets_block_0453
    mov r6,r0
.Lmt7_packets_block_0453:
    fmul fr7,fr5
    fmov.s fr6,@-r6
    add #0x40,r5
    fmov.s fr4,@-r6
    fmul fr14,fr10
    mov.l r0,@r6
    pref @r6
    mov r4,r14
    mov.l @(0x4,r14),r14
    add #0x40,r6
    mov.l @r4+,r0
    fmul fr13,fr9
    fmov.s fr2,@-r6
    fmul fr12,fr8
    fmov.s fr2,@-r6
    add r4,r14
    fmov.s fr10,@-r6
    add #0x4,r14
    fmov.s fr8,@-r6
    bt/s .Lmt7_packets_block_0454
    pref @r6
    tst #0x1,r0
    add #0x40,r6
    bt/s .Lmt7_packets_block_0449
    add #0x4,r4
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0449
    add #-0x8,r14
.Lmt7_packets_block_0454:
    add #-0x4,r4
    dt r9
    add #0x40,r6
    bf/s .Lmt7_packets_block_0448
    mov #0x3,r3
    fschg
    rts
    add #-0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0455:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0456:
    mov #0x0,r11
    bsr .Lmt7_packets_block_0481
    mov #0x0,r9
    bt/s .Lmt7_packets_block_0458
    rotcl r9
    bsr .Lmt7_packets_block_0486
    add #0x1,r11
    bsr .Lmt7_packets_block_0481
    nop
    bt/s .Lmt7_packets_block_0464
    rotcl r9
    bsr .Lmt7_packets_block_0486
    add #0x1,r11
.Lmt7_packets_block_0457:
    bsr .Lmt7_packets_block_0481
    nop
    bt/s .Lmt7_packets_block_0474
    rotcl r9
    bsr .Lmt7_packets_block_0486
    add #0x1,r11
    bf .Lmt7_packets_block_0457
    bra .Lmt7_packets_block_0462
    nop
.Lmt7_packets_block_0458:
    dt r3
    add #0x1,r11
    bsr .Lmt7_packets_block_0481
    dt r3
    bf/s .Lmt7_packets_block_0468
    rotcl r9
.Lmt7_packets_block_0459:
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf/s .Lmt7_packets_block_0460
    fldi1 fr7
    add #-0x18,r4
    mov.l @(0x4,r14),r14
    add r4,r14
.Lmt7_packets_block_0460:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr5
    fmov.s @r14+,fr6
    mov #0x20,r1
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    ftrv xmtrx,fv4
    pref @r4
    add #-0x20,r4
    bf/s .Lmt7_packets_block_0461
    mov r4,r14
    mov #0x8,r1
    mov.l @(0x4,r14),r14
    add #0x8,r14
    add r4,r14
.Lmt7_packets_block_0461:
    flds fr7,FPUL
    sts FPUL,r0
    cmp/hi r0,r12
    add #0x1,r11
    bf/s .Lmt7_packets_block_0472
    rotcl r9
    dt r3
    bf .Lmt7_packets_block_0459
.Lmt7_packets_block_0462:
    mov.l @r4+,r0
    cmp/pl r0
    bf/s .Lmt7_packets_block_0463
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0456
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0463:
    lds.l @r15+,PR
    mov #0x1,r0
    mov.l @r15+,r11
    add #-0x4,r4
    mov.l @r15+,r10
    rts
    or r0,r12
.Lmt7_packets_block_0464:
    bsr .Lmt7_packets_block_0488
    dt r3
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0465
    add #-0x18,r4
.Lmt7_packets_block_0465:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0466
    add #-0x18,r4
.Lmt7_packets_block_0466:
    bsr .Lmt7_packets_block_0481
    nop
    bsr .Lmt7_packets_block_0488
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0492
    mov #0x0,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0467
    add #0x18,r4
.Lmt7_packets_block_0467:
    bra .Lmt7_packets_block_0479
    nop
.Lmt7_packets_block_0468:
    bsr .Lmt7_packets_block_0488
    mov #0x1,r11
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0469
    add #-0x18,r4
.Lmt7_packets_block_0469:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0470
    add #-0x18,r4
.Lmt7_packets_block_0470:
    bsr .Lmt7_packets_block_0481
    nop
    bsr .Lmt7_packets_block_0488
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0492
    mov #0x0,r0
    clrt
    bsr .Lmt7_packets_block_0490
    mov #0x1,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0471
    add #0x18,r4
.Lmt7_packets_block_0471:
    bra .Lmt7_packets_block_0479
    nop
.Lmt7_packets_block_0472:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0473
    add #-0x18,r4
.Lmt7_packets_block_0473:
    bsr .Lmt7_packets_block_0481
    nop
.Lmt7_packets_block_0474:
    bsr .Lmt7_packets_block_0488
    nop
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0475
    add #-0x18,r4
.Lmt7_packets_block_0475:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0476
    add #-0x18,r4
.Lmt7_packets_block_0476:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0477
    add #-0x18,r4
.Lmt7_packets_block_0477:
    bsr .Lmt7_packets_block_0481
    nop
    bsr .Lmt7_packets_block_0488
    add #-0x2,r11
    bsr .Lmt7_packets_block_0481
    nop
    bsr .Lmt7_packets_block_0488
    add #0x1,r11
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0478
    add #0x18,r4
.Lmt7_packets_block_0478:
    mov r12,r0
    add #0x1,r11
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0480
    mov r9,r0
    bsr .Lmt7_packets_block_0489
    nop
    bra .Lmt7_packets_block_0480
    mov r9,r0
.Lmt7_packets_block_0479:
    bsr .Lmt7_packets_block_0481
    add #0x1,r11
    bsr .Lmt7_packets_block_0488
    rotcl r9
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0480
    mov r9,r0
    bsr .Lmt7_packets_block_0489
    nop
    mov r9,r0
.Lmt7_packets_block_0480:
    and #0x7,r0
    shll2 r0
    braf r0
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa00c
    .word 0x0009
    .word 0xa019
    .word 0x0009
    .word 0xa024
    .word 0x0009
    .word 0xa02a
    .word 0x0009
    .word 0xa038
    .word 0x0009
    .word 0xa03d
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb137
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb103
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb12f
    .word 0x70ff
    .word 0x8bd3
    .word 0xaf5d
    .word 0x0009
    .word 0x0008
    .word 0xb0f9
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb125
    .word 0x70ff
    .word 0x4310
    .word 0xb0f2
    .word 0x60b3
    .word 0x8bc6
    .word 0xaf50
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb11b
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf27
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb113
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0df
    .word 0x70ff
    .word 0x4310
    .word 0xb0dc
    .word 0x60b3
    .word 0x8901
    .word 0xaf0a
    .word 0x7b01
    .word 0xaf38
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb103
    .word 0x70ff
    .word 0x8ba7
    .word 0xaf31
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x8903
    .word 0x60b3
    .word 0x0008
    .word 0xb0f9
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb0f5
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb0f1
    .word 0x70ff
    .word 0x4310
    .word 0xb0be
    .word 0x60b3
    .word 0x8b92
    .word 0xaf1c
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0481:
    mov.l @r4,r0
    tst #0x1,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0482
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0482:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr5
    fmov.s @r14,fr6
    mov r14,r1
    fldi1 fr7
    mov.b @(0x7,r14),r0
    add #0x10,r1
    mov.l @(0x8,r14),r8
    cmp/pl r0
    extu.b r8,r0
    bf/s .Lmt7_packets_block_0484
    ftrv xmtrx,fv4
    mov.l @(0x4,r14),r2
    exts.b r2,r0
    shlr8 r2
    lds r0,FPUL
    exts.b r2,r0
    float FPUL,fr1
    shlr8 r2
    lds r0,FPUL
    exts.b r2,r0
    float FPUL,fr0
    lds r0,FPUL
    float FPUL,fr2
    fldi0 fr3
    mov r15,r2
    add #0x24,r2
    fmov.s @r2+,fr8
    mov #0x3c,r0
    fmov.s @r2+,fr9
    shll8 r0
    fmov.s @r2+,fr10
    or #0x1,r0
    fipr fv8,fv0
    shll8 r0
    fmov.s @r2+,fr2
    or #0x2,r0
    fldi0 fr0
    shll8 r0
    or #0x4,r0
    lds r0,FPUL
    extu.b r8,r0
    fcmp/gt fr0,fr3
    fsts FPUL,fr11
    bf .Lmt7_packets_block_0483
    fmul fr11,fr3
    fadd fr3,fr2
.Lmt7_packets_block_0483:
    sett
.Lmt7_packets_block_0484:
    fldi1 fr1
    lds r0,FPUL
    shlr8 r8
    extu.b r8,r0
    shlr8 r8
    fdiv fr4,fr1
    float FPUL,fr11
    lds r0,FPUL
    float FPUL,fr10
    extu.b r8,r0
    lds r0,FPUL
    shlr8 r8
    float FPUL,fr9
    lds r8,FPUL
    float FPUL,fr8
    flds fr7,FPUL
    fmul fr15,fr11
    sts FPUL,r0
    fmul fr14,fr10
    fsts FPUL,fr4
    bf/s .Lmt7_packets_block_0485
    fmul fr13,fr9
    fmul fr2,fr9
    fmul fr2,fr10
    fmul fr2,fr11
.Lmt7_packets_block_0485:
    cmp/hi r0,r12
    fmul fr12,fr8
    rts
    fmov fr1,fr7
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0486:
    fschg
    add #0x18,r6
    fmov.s @r1,fr2
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fldi0 fr2
    mov r6,r0
    fldi0 fr3
    shar r0
    fmov.s fr6,@-r6
    dt r3
    fmov.s fr4,@-r6
    bt .Lmt7_packets_block_0487
    mov r6,r0
.Lmt7_packets_block_0487:
    mov.l r0,@r6
    pref @r6
    add #0x40,r6
    fmov.s fr2,@-r6
    add #0x40,r5
    fmov.s fr2,@-r6
    fmov.s fr10,@-r6
    fmov.s fr8,@-r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0488:
    mov r11,r0
    mov #0x6,r10
    and #0x3,r0
    shld r10,r0
    mov.l .Lmt7_packets_block_0495,r10
    fschg
    add r0,r10
    fmov.s @r1,fr2
    fmov.s fr10,@-r10
    fmov.s fr8,@-r10
    fmov.s fr2,@-r10
    fmov.s fr6,@-r10
    fmov.s fr4,@-r10
    rts
    fschg
    .word 0x0009
.Lmt7_packets_block_0489:
    mov #0x6,r1
    mov.l .Lmt7_packets_block_0496,r10
    mov r11,r0
    and #0x3,r0
    shld r1,r0
    fschg
    fmov.s @(r0,r10),fr8
    add #0x8,r0
    fmov.s @(r0,r10),fr10
    add #-0x48,r0
    extu.b r0,r0
    fmov fr8,@(r0,r10)
    add #0x8,r0
    fmov fr10,@(r0,r10)
    add #-0x48,r0
    extu.b r0,r0
    fmov fr8,@(r0,r10)
    add #0x8,r0
    fmov fr10,@(r0,r10)
    rts
    fschg
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0490:
    mov #0x6,r1
    mov.l .Lmt7_packets_block_0494,r10
    and #0x3,r0
    fschg
    shld r1,r0
    fldi0 fr6
    add r0,r10
    fldi0 fr7
    mov #-0x10,r2
    fmov.s @r10+,fr0
    shll16 r2
    fmov.s @r10+,fr2
    add #0x18,r6
    fmov.s @r10+,fr4
    fmul fr3,fr2
    fmov.s @r10+,fr8
    fmul fr3,fr1
    fmov.s @r10,fr10
    shll8 r2
    fmov.s fr4,@-r6
    add #0x40,r5
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    bt .Lmt7_packets_block_0491
    mov r6,r2
.Lmt7_packets_block_0491:
    mov.l r2,@r6
    mov r6,r2
    pref @r6
    add #0x40,r6
    fmov.s fr6,@-r6
    fmov.s fr6,@-r6
    fmov.s fr10,@-r6
    fmov.s fr8,@-r6
    pref @r6
    fschg
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0492:
    mov #0x6,r1
    mov.l .Lmt7_packets_block_0494,r10
    and #0x3,r0
    shld r1,r0
    mov r10,r2
    add r0,r10
    mov r11,r0
    and #0x3,r0
    fmov.s @r10,fr2
    shld r1,r0
    lds r12,FPUL
    add r0,r2
    fsts FPUL,fr0
    fmov.s @r2,fr1
    fsub fr2,fr0
    add #0x20,r2
    fschg
    add #0x30,r6
    fsub fr2,fr1
    fmov.s @r2,fr4
    add #-0x8,r2
    lds r7,FPUL
    add #0x20,r10
    fdiv fr1,fr0
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsts FPUL,fr1
    fsub fr2,fr4
    fsub fr3,fr5
    add #-0x8,r6
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fschg
    fmov.s fr1,@-r6
    fmov.s @r2,fr5
    add #-0x4,r2
    fmov.s @r10,fr3
    add #-0x4,r10
    fmov.s @r2,fr4
    fmov.s @r10,fr2
    fsub fr3,fr5
    fsub fr2,fr4
    mov #-0x10,r2
    fmac fr0,fr5,fr3
    shll16 r2
    fmac fr0,fr4,fr2
    shll8 r2
    fmul fr1,fr3
    add #0x40,r5
    fmul fr1,fr2
    fmov.s fr3,@-r6
    fmov.s fr2,@-r6
    bt .Lmt7_packets_block_0493
    mov r6,r2
.Lmt7_packets_block_0493:
    mov.l r2,@-r6
    pref @r6
    add #0x40,r6
    fldi0 fr2
    fmov.s fr2,@-r6
    fmov.s fr2,@-r6
    fmov.s fr2,@-r6
    fmov.s fr2,@-r6
    pref @r6
    rts
    add #0x10,r6
.Lmt7_packets_block_0494:
    .long .Lmt7_packets_block_0434
.Lmt7_packets_block_0495:
    .long .Lmt7_packets_block_0436
.Lmt7_packets_block_0496:
    .long .Lmt7_packets_block_0435
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0497:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0498:
    bsr .Lmt7_packets_block_0506
    mov #0x0,r9
    mov r9,r0
    cmp/eq #0x0,r0
    bt .Lmt7_packets_block_0501
    cmp/eq #0x7,r0
    bt .Lmt7_packets_block_0502
    bsr .Lmt7_packets_block_0481
    mov #0x0,r11
    bsr .Lmt7_packets_block_0488
    nop
    bsr .Lmt7_packets_block_0481
    mov #0x1,r11
    bsr .Lmt7_packets_block_0488
    nop
    bsr .Lmt7_packets_block_0481
    mov #0x2,r11
    bsr .Lmt7_packets_block_0488
    nop
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0499
    shll2 r9
    bsr .Lmt7_packets_block_0489
    nop
.Lmt7_packets_block_0499:
    braf r9
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa034
    .word 0x0009
    .word 0xa041
    .word 0x0009
    .word 0xa04f
    .word 0x0009
    .word 0xa05a
    .word 0x0009
    .word 0xa068
    .word 0x0009
    .word 0xa073
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0500:
    dt r3
    bf .Lmt7_packets_block_0498
    mov #0x1,r0
    lds.l @r15+,PR
    or r0,r12
    mov.l @r15+,r11
    rts
    mov.l @r15+,r10
.Lmt7_packets_block_0501:
    mov r3,r9
    bsr .Lmt7_packets_block_0481
    mov #0x3,r3
    bsr .Lmt7_packets_block_0486
    nop
    bsr .Lmt7_packets_block_0481
    nop
    bsr .Lmt7_packets_block_0486
    nop
    bsr .Lmt7_packets_block_0481
    nop
    bsr .Lmt7_packets_block_0486
    nop
    bra .Lmt7_packets_block_0500
    mov r9,r3
.Lmt7_packets_block_0502:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0503
    add #0x18,r4
.Lmt7_packets_block_0503:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0504
    add #0x18,r4
.Lmt7_packets_block_0504:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0505
    add #0x18,r4
.Lmt7_packets_block_0505:
    bra .Lmt7_packets_block_0500
    nop
    .word 0x0008
    .word 0xbf16
    .word 0xe000
    .word 0x0008
    .word 0xbf13
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf3f
    .word 0xe000
    .word 0x0018
    .word 0xbf3c
    .word 0xe001
    .word 0xafc9
    .word 0x0009
    .word 0x0008
    .word 0xbf07
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf33
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbeff
    .word 0xe002
    .word 0x0018
    .word 0xbf2c
    .word 0xe001
    .word 0xafb9
    .word 0x0009
    .word 0x0008
    .word 0xbef7
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf23
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbf1f
    .word 0xe000
    .word 0xafac
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf19
    .word 0xe000
    .word 0x0008
    .word 0xbee6
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf12
    .word 0xe000
    .word 0x0018
    .word 0xbedf
    .word 0xe002
    .word 0xaf9c
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbf09
    .word 0xe000
    .word 0x0008
    .word 0xbed6
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbf02
    .word 0xe001
    .word 0xaf8f
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbefc
    .word 0xe000
    .word 0x0008
    .word 0xbef9
    .word 0xe001
    .word 0x0018
    .word 0xbec6
    .word 0xe002
    .word 0xaf83
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0506:
    mov r4,r1
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0507
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0508
    fldi1 fr7
.Lmt7_packets_block_0507:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
    add #0x18,r4
.Lmt7_packets_block_0508:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv4
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0509
    fmov.s @r4+,fr0
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr0
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    bra .Lmt7_packets_block_0510
    fldi1 fr3
.Lmt7_packets_block_0509:
    fmov.s @r4+,fr1
    fmov.s @r4,fr2
    fldi1 fr3
    add #0x18,r4
.Lmt7_packets_block_0510:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv0
    flds fr7,FPUL
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0511
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0512
    fldi1 fr7
.Lmt7_packets_block_0511:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
.Lmt7_packets_block_0512:
    mov r1,r4
    sts FPUL,r0
    ftrv xmtrx,fv4
    cmp/hi r0,r12
    flds fr3,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    flds fr7,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    rts
    rotcl r9
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
_shenmue_dc_mt7_lit_packets:
    fldi1 fr4
_shenmue_dc_mt7_lit_packets_alpha:
    add #0x18,r4
    mov.l .Lmt7_packets_block_0515,r7
    fldi1 fr1
    fmov.s @r7,fr0
    fmov.s fr15,@-r15
    fmov.s fr14,@-r15
    fdiv fr0,fr1
    fmov.s fr13,@-r15
    fmov.s fr12,@-r15
    mov.l r14,@-r15
    mov.l r13,@-r15
    mov.l r12,@-r15
    mov.l r11,@-r15
    mov.l r10,@-r15
    mov.l r9,@-r15
    mov.l r8,@-r15
    mov #0x0,r0
    sts.l PR,@-r15
    add #-0x48,r15
    fldi1 fr7
    fmov fr4,@(r0,r15)
    mov #0x4,r0
    fabs fr0
    fmov fr0,@(r0,r15)
    mov.l .Lmt7_packets_block_0550,r0
    fmov.s fr7,@r7
    mov.l @r0,r1
    mov.l .Lmt7_packets_block_0552,r2
    mov.l r1,@(0x8,r15)
    mov.l @r2,r1
    mov.l .Lmt7_packets_block_0553,r0
    mov.l .Lmt7_packets_block_0551,r2
    mov.l r1,@(0x10,r15)
    mov.l @r2,r3
    mov.l @r0,r1
    mov.l r3,@(0xc,r15)
    mov.l r1,@(0x14,r15)
    add #0xc,r5
    mov.l @r5+,r12
    add #0x10,r5
    mov.l @r5+,r7
    mov.l .Lmt7_packets_block_0548,r0
    mov.l @r0+,r1
    mov.l @r0+,r2
    mov.l @r0+,r3
    mov.l r1,@(0x18,r15)
    mov.l r2,@(0x1c,r15)
    mov.l r3,@(0x20,r15)
    mov.l .Lmt7_packets_block_0561,r0
    mov.l @r0+,r5
    mov.l r5,@(0x28,r15)
    mov.l @r0+,r6
    mov.l r6,@(0x2c,r15)
    mov.l .Lmt7_packets_block_0546,r10
    mov r15,r3
    mov.l @r4+,r8
    add #0x4,r3
    mov.l @r10,r10
    mov #-0x1,r11
    fmov fr4,fr11
    or r5,r8
    fcmp/gt fr11,fr7
    fmov.s @r4+,fr2
    or r6,r1
    mov.l @r4+,r1
    bf/s .Lmt7_packets_block_0518
    fmov.s @r4+,fr3
    bt .Lmt7_packets_block_0517
    .word 0x0009
.Lmt7_packets_block_0515:
    .long __Q37shenmue2dc3sdk.g_modelScale
.Lmt7_packets_block_0516:
    mov.l @(0x28,r15),r3
    mov.l @(0x2c,r15),r0
    or r3,r8
    mov r15,r3
    fmov.s @r3+,fr11
    or r0,r1
    fldi1 fr7
    fcmp/gt fr11,fr7
    mov r4,r0
    add #0x30,r0
    bf/s .Lmt7_packets_block_0518
    pref @r0
.Lmt7_packets_block_0517:
    mov #0x2,r2
    mov r1,r0
    shll16 r2
    shll8 r2
    mov.l .Lmt7_packets_block_0560,r9
    or r2,r8
    or #0xc0,r0
    swap.w r0,r0
    mov.l @r9,r9
    or #0x10,r0
    swap.b r0,r0
    or #0xfc,r0
    xor r9,r0
    swap.b r0,r0
    swap.w r0,r0
    add r2,r2
    mov r0,r1
    not r2,r2
    and r2,r8
.Lmt7_packets_block_0518:
    mov r8,r0
    fschg
    shlr16 r0
    fmov.s @r4+,fr4
    shlr8 r0
    fmov.s @r4+,fr6
    and #0x7,r0
    fschg
    mov #0x4,r2
    fmov fr7,fr0
    shll2 r0
    fldi1 fr7
    shll8 r2
    fmov.s @r3+,fr1
    cmp/eq r0,r11
    ftrv xmtrx,fv4
    mov r0,r11
    mov #-0x20,r6
    fmul fr1,fr0
    shll16 r2
    bt/s .Lmt7_packets_block_0519
    mov.l @(r0,r10),r5
    mov #-0x1,r0
    mov r5,r9
    shll16 r0
    shlr16 r9
    shll8 r0
    shlr8 r9
    or #0x38,r0
    mov.l r9,@r0
    mov.l r9,@(0x4,r0)
.Lmt7_packets_block_0519:
    add #-0x1,r2
    lds r12,FPUL
    mov #-0x2,r0
    fadd fr0,fr7
    and r5,r2
    fsts FPUL,fr1
    shll8 r6
    fcmp/gt fr1,fr7
    shll16 r6
    fsub fr0,fr7
    or r2,r6
    mov.l .Lmt7_packets_block_0547,r9
    and r0,r12
    bf/s .Lmt7_packets_block_0523
    fsub fr0,fr7
    fcmp/gt fr1,fr7
    bf/s .Lmt7_packets_block_0523
    add #0x1,r12
.Lmt7_packets_block_0520:
    mov r4,r2
    add #0x2c,r4
    mov.l @r4+,r0
    mov r8,r1
    add r0,r4
    mov.l @r4+,r8
    tst r8,r8
    bf .Lmt7_packets_block_0521
    bra .Lmt7_packets_block_0542
    nop
.Lmt7_packets_block_0521:
    mov r15,r9
    add #0x18,r9
    fmov.s @r9+,fr5
    fmov.s @r9+,fr6
    add #0xc,r2
    fmov.s @r9+,fr7
    mov r1,r0
    tst #0x10,r0
    bf/s .Lmt7_packets_block_0522
    tst #0x20,r0
    bt .Lmt7_packets_block_0522
    fmov.s @r2+,fr0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    fmov.s @r2+,fr3
    fmul fr11,fr0
    fmul fr5,fr1
    fmul fr6,fr2
    fmul fr7,fr3
    fschg
    mov.l .Lmt7_packets_block_0565,r0
    fmov.s fr2,@-r0
    fmov.s fr0,@-r0
    fschg
.Lmt7_packets_block_0522:
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0516
    fmov.s @r4+,fr3
.Lmt7_packets_block_0523:
    fmov.s @r3+,fr1
    mov.l .Lmt7_packets_block_0555,r0
    add #0x10,r9
    fmov.s @r0,fr7
    fmul fr4,fr7
    fneg fr0
    fmul fr0,fr1
    fmov.s fr3,@-r9
    fsub fr7,fr5
    fmov.s @r3+,fr3
    mov r4,r2
    mov.l .Lmt7_packets_block_0554,r0
    fcmp/gt fr1,fr5
    fmov.s @r0,fr7
    fmul fr0,fr3
    bf/s .Lmt7_packets_block_0520
    fmul fr4,fr7
    mov.l r1,@-r9
    fsub fr7,fr6
    fmov.s @r3+,fr0
    fcmp/gt fr3,fr6
    fmov.s @r3+,fr7
    fmul fr4,fr0
    bf/s .Lmt7_packets_block_0520
    fsub fr1,fr0
    fcmp/gt fr0,fr5
    fmov.s @r3+,fr5
    fmul fr4,fr7
    bt/s .Lmt7_packets_block_0520
    fsub fr3,fr7
    fmov.s fr2,@-r9
    mov #0x10,r1
    fcmp/gt fr7,fr6
    fmov.s @r3+,fr6
    bt .Lmt7_packets_block_0520
    mov r8,r0
    or r1,r8
    mov #-0x21,r1
    and r1,r8
    mov.l r8,@-r9
    mov #0x8,r1
    tst #0x10,r0
    mov.b @(0x4,r2),r0
    add #0x8,r2
    fmov.s @r3+,fr7
    mov r0,r8
    fmov.s @r2+,fr15
    bf/s .Lmt7_packets_block_0524
    add #0x30,r4
    fmov.s @r2+,fr0
    mov #-0x2,r0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    cmp/eq r0,r8
    fmov.s @r2+,fr3
    fmul fr11,fr0
    fmov fr5,fr11
    fmul fr5,fr1
    fmul fr6,fr2
    fmul fr7,fr3
    bt .Lmt7_packets_block_0525
    mov.l .Lmt7_packets_block_0565,r0
    fschg
    fmov.s fr2,@-r0
    fmov.s fr0,@-r0
    fschg
.Lmt7_packets_block_0524:
    cmp/pz r8
    bt .Lmt7_packets_block_0530
    mov r8,r0
    cmp/eq #-0x1,r0
    bt/s .Lmt7_packets_block_0529
    cmp/eq #-0x2,r0
    bf .Lmt7_packets_block_0527
.Lmt7_packets_block_0525:
    mov.l .Lmt7_packets_block_0557,r0
    fmov.s @r0+,fr14
    fmov.s @r0+,fr12
    fmov.s @r0+,fr13
    mov.l .Lmt7_packets_block_0558,r0
    jsr @r0
    nop
    bf .Lmt7_packets_block_0526
    bra .Lmt7_packets_block_0542
    nop
.Lmt7_packets_block_0526:
    .word 0xf249
    .word 0x6146
    .word 0xaf33
    .word 0xf349
.Lmt7_packets_block_0527:
    cmp/eq #-0x3,r0
    bf .Lmt7_packets_block_0529
    mov.l .Lmt7_packets_block_0559,r0
    jsr @r0
    nop
    bf .Lmt7_packets_block_0528
    bra .Lmt7_packets_block_0542
    nop
.Lmt7_packets_block_0528:
    .word 0xf249
    .word 0x6146
    .word 0xaf27
    .word 0xf349
.Lmt7_packets_block_0529:
    mov.l .Lmt7_packets_block_0563,r1
    fldi0 fr0
    add #0x10,r1
    fldi1 fr1
    mov #-0x1,r8
    fmov.s fr1,@-r1
    fmov.s fr1,@-r1
    fmov.s fr1,@-r1
    bra .Lmt7_packets_block_0531
    fldi1 fr15
.Lmt7_packets_block_0530:
    mov.l .Lmt7_packets_block_0562,r0
    mov.l .Lmt7_packets_block_0563,r1
    add #0x4,r0
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    fmul fr15,fr1
    fmul fr15,fr2
    fmov.s @r0+,fr0
    fmov.s fr1,@r1
    add #0x4,r1
    fmul fr15,fr0
    fmov.s fr2,@r1
    add #0x4,r1
    fmov.s fr0,@r1
.Lmt7_packets_block_0531:
    cmp/pz r8
    mov.l .Lmt7_packets_block_0556,r0
    bf .Lmt7_packets_block_0532
    jsr @r0
    nop
.Lmt7_packets_block_0532:
    mov.l @r4+,r0
.Lmt7_packets_block_0533:
    cmp/pl r0
    mov.l .Lmt7_packets_block_0547,r9
    bt/s .Lmt7_packets_block_0535
    cmp/pz r0
    mov r0,r8
    mov r11,r0
    mov.l r5,@(r0,r10)
    bf .Lmt7_packets_block_0534
    bra .Lmt7_packets_block_0542
    nop
.Lmt7_packets_block_0534:
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0516
    fmov.s @r4+,fr3
.Lmt7_packets_block_0535:
    mov r0,r1
    mov #-0x5,r2
    mov.l @r9+,r3
    and #0x40,r0
    shld r2,r0
    mov #-0x3,r2
    and r2,r3
    or r3,r0
    add #0x20,r4
    pref @r4
    fschg
    lds r0,FPUL
    mov.l @(0x28,r15),r2
    mov r1,r0
    cmp/pz r2
    mov #0x1b,r2
    bt/s .Lmt7_packets_block_0536
    and #0x3,r0
    xor #0x1,r0
.Lmt7_packets_block_0536:
    shld r2,r0
    mov #0x3,r3
    shld r2,r3
    not r3,r3
    mov.l @r9+,r2
    and r3,r2
    fsts FPUL,fr0
    or r0,r2
    lds r2,FPUL
    add #-0x20,r4
    fsts FPUL,fr1
    mov.l .Lmt7_packets_block_0564,r0
    add #0x10,r6
    fmov.s @r9+,fr2
    rotcr r12
    mov.l @r0,r0
    bt/s .Lmt7_packets_block_0537
    rotcl r12
    tst r0,r0
    mov #-0x2,r0
    bt .Lmt7_packets_block_0539
    cmp/pz r8
    bf .Lmt7_packets_block_0539
.Lmt7_packets_block_0537:
    fmov.s fr2,@-r6
.Lmt7_packets_block_0538:
    add #0x20,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    bra .Lmt7_packets_block_0540
    add #0x20,r6
.Lmt7_packets_block_0539:
    cmp/eq r0,r8
    fmov.s fr2,@-r6
    bt .Lmt7_packets_block_0538
    add #0x40,r5
    flds fr0,FPUL
    sts FPUL,r0
    or #0x30,r0
    fschg
    xor #0x10,r0
    fmov.s fr1,@-r6
    add #-0x10,r9
    mov.l r0,@-r6
    or #0x30,r0
    pref @r6
    fschg
    mov.l r0,@r9
    add #0x40,r6
    mov.l .Lmt7_packets_block_0565,r0
    fldi0 fr4
    add #-0x10,r0
    fldi0 fr5
    fmov.s @r0+,fr0
    fmov.s @r0+,fr2
    fmov.s fr4,@-r6
    fmov.s fr4,@-r6
    mov.l .Lmt7_packets_block_0563,r0
    fmov.s @r0+,fr4
    fmul fr5,fr1
    fmov.s @r0+,fr4
    fmul fr4,fr2
    fmul fr5,fr3
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    fschg
    pref @r6
    mov #-0x2,r8
    add #0x20,r6
.Lmt7_packets_block_0540:
    rotcr r12
    mov r1,r0
    bt/s .Lmt7_packets_block_0543
    shll r12
    tst #0x8,r0
    bt .Lmt7_packets_block_0541
    bsr .Lmt7_packets_block_0603
    nop
    bra .Lmt7_packets_block_0533
    mov.l @r4+,r0
.Lmt7_packets_block_0541:
    bsr .Lmt7_packets_block_0566
    nop
    bra .Lmt7_packets_block_0533
    mov.l @r4+,r0
.Lmt7_packets_block_0542:
    add #0x48,r15
    lds.l @r15+,PR
    mov.l @r15+,r8
    mov.l @r15+,r9
    mov.l @r15+,r10
    mov.l @r15+,r11
    mov.l @r15+,r12
    mov.l @r15+,r13
    mov.l @r15+,r14
    fmov.s @r15+,fr12
    fmov.s @r15+,fr13
    fmov.s @r15+,fr14
    rts
    fmov.s @r15+,fr15
.Lmt7_packets_block_0543:
    mov #0x0,r1
    tst #0x40,r0
    addc r1,r12
    tst #0x10,r0
    bt/s .Lmt7_packets_block_0544
    tst #0x8,r0
    bsr .Lmt7_packets_block_0639
    tst #0x20,r0
    bra .Lmt7_packets_block_0533
    mov.l @r4+,r0
.Lmt7_packets_block_0544:
    bt .Lmt7_packets_block_0545
    bsr .Lmt7_packets_block_0700
    tst #0x20,r0
    bra .Lmt7_packets_block_0533
    mov.l @r4+,r0
.Lmt7_packets_block_0545:
    trapa #0x58
    bra .Lmt7_packets_block_0545
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0546:
    .long __nj_vertex_buffer_
.Lmt7_packets_block_0547:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch
.Lmt7_packets_block_0548:
    .long __Q37shenmue2dc3sdk.g_mt7DiffuseChannels
.Lmt7_packets_block_0549:
    .long __Q37shenmue2dc3sdk.g_mt7SpecularChannels
.Lmt7_packets_block_0550:
    .long __Q37shenmue2dc3sdk.g_mt7HorizontalClipScale
.Lmt7_packets_block_0551:
    .long __Q37shenmue2dc3sdk.g_mt7VerticalClipScale
.Lmt7_packets_block_0552:
    .long __Q27shenmue12matrix_stack.g_projectionViewportWidth
.Lmt7_packets_block_0553:
    .long __Q27shenmue12matrix_stack.g_projectionViewportHeight
.Lmt7_packets_block_0554:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginY
.Lmt7_packets_block_0555:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginX
.Lmt7_packets_block_0556:
    .long _shenmue_dc_mt7_select_float_lights
.Lmt7_packets_block_0557:
    .long __Q37shenmue2dc3sdk.g_mt7LightingParameters+0x10
.Lmt7_packets_block_0558:
    .long .Lmt7_packets_block_0352
.Lmt7_packets_block_0559:
    .long .Lmt7_packets_block_0423
.Lmt7_packets_block_0560:
    .long __Q37shenmue2dc3sdk.g_mt7AlphaBlendMask
.Lmt7_packets_block_0561:
    .long _shenmue_dc_mt7_draw_controls
.Lmt7_packets_block_0562:
    .long __Q37shenmue2dc3sdk.g_ambientLight
.Lmt7_packets_block_0563:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x240
.Lmt7_packets_block_0564:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x250
.Lmt7_packets_block_0565:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x10
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0566:
    cmp/pz r8
    bt .Lmt7_packets_block_0567
    bra .Lmt7_packets_block_0593
    nop
.Lmt7_packets_block_0567:
    mov.l @r4+,r3
    fldi0 fr0
    add #0x40,r6
    fldi0 fr1
    fschg
    fmov.s fr0,@-r6
    fmov.s fr0,@-r6
    fschg
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0592,r13
    bf/s .Lmt7_packets_block_0568
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0568:
    mov.l .Lmt7_packets_block_0590,r0
    fschg
    mov.l @r13,r13
    fmov.s @r0+,fr12
    fmov.s @r0+,fr14
    fmov.s @r14+,fr4
    add #0x20,r4
    fmov.s @r14+,fr6
    bra .Lmt7_packets_block_0571
    fschg
    .word 0x0009
.Lmt7_packets_block_0569:
    mov.l .Lmt7_packets_block_0590,r0
    add #0x20,r6
    mov.l @r13,r13
    mov r6,r1
    fmov.s @r0+,fr12
    add #0x40,r5
.Lmt7_packets_block_0570:
    fmov.s @r0+,fr14
    add #0x40,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    pref @r1
.Lmt7_packets_block_0571:
    fmov fr7,fr8
    tst r13,r13
    fmov.s @r14+,fr9
    bt/s .Lmt7_packets_block_0575
    fmov.s @r14+,fr10
    shlr r13
    mov.l .Lmt7_packets_block_0591,r2
    bf .Lmt7_packets_block_0574
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0572:
    fldi0 fr3
    cmp/eq #0x2,r0
    mov #0x74,r1
    bf/s .Lmt7_packets_block_0576
    add r2,r1
    fldi1 fr7
    tst r13,r13
    add #0xc,r1
    bf/s .Lmt7_packets_block_0573
    fmov.s @r1+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0573:
    fmov.s @r1+,fr1
    mov r2,r0
    fmov.s @r1+,fr2
    add #0x18,r0
    fipr fv0,fv8
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    fmov.s @r0+,fr3
    fldi0 fr0
    fcmp/gt fr11,fr0
    fmov fr11,fr0
    bf/s .Lmt7_packets_block_0580
    tst r13,r13
.Lmt7_packets_block_0574:
    mov #-0x50,r0
    bt .Lmt7_packets_block_0584
    shlr r13
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0582
    add r0,r2
    bra .Lmt7_packets_block_0572
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0575:
    fldi1 fr7
    bra .Lmt7_packets_block_0584
    ftrv xmtrx,fv4
    .word 0x0009
.Lmt7_packets_block_0576:
    fmov.s @r1+,fr0
    tst r13,r13
    fmov.s @r1+,fr1
    fsub fr4,fr0
    fmov.s @r1+,fr2
    fsub fr5,fr1
    fldi1 fr7
    add #0x18,r1
    fldi0 fr11
    bf/s .Lmt7_packets_block_0577
    fsub fr6,fr2
    ftrv xmtrx,fv4
.Lmt7_packets_block_0577:
    fipr fv0,fv8
    fldi0 fr12
    fipr fv0,fv0
    fcmp/gt fr11,fr12
    fmov.s @r1,fr12
    bt .Lmt7_packets_block_0581
    fcmp/gt fr3,fr12
    bf .Lmt7_packets_block_0581
    cmp/eq #0x3,r0
    flds fr3,FPUL
    bt/s .Lmt7_packets_block_0578
    fsrra fr3
    add #-0x18,r1
    fmov.s @r1+,fr12
    fmul fr0,fr12
    fmov.s @r1+,fr0
    fmac fr0,fr1,fr12
    fmov.s @r1,fr0
    add #-0x30,r1
    fmac fr0,fr2,fr12
    fmov.s @r1,fr2
    fneg fr12
    fmul fr3,fr12
    add #0x44,r1
    fcmp/gt fr12,fr2
    fsub fr2,fr12
    fmov.s @r1,fr2
    bt .Lmt7_packets_block_0581
    fmul fr2,fr12
    add #-0x4,r1
    fmul fr12,fr11
.Lmt7_packets_block_0578:
    add #-0x44,r1
    fmul fr3,fr11
    add #-0x8,r1
    fmul fr3,fr3
    fmov.s @r1,fr12
    fmul fr3,fr3
    fldi1 fr2
    mov r2,r0
    fmul fr3,fr3
    add #0x18,r0
    fmul fr3,fr12
    fmov fr11,fr0
    fcmp/gt fr2,fr12
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    bt/s .Lmt7_packets_block_0579
    fmov.s @r0+,fr3
    fmul fr12,fr0
.Lmt7_packets_block_0579:
    tst r13,r13
.Lmt7_packets_block_0580:
    fmac fr0,fr1,fr13
    mov #-0x50,r0
    fmac fr0,fr2,fr14
    bt/s .Lmt7_packets_block_0584
    fmac fr0,fr3,fr15
    bra .Lmt7_packets_block_0583
    shlr r13
.Lmt7_packets_block_0581:
    tst r13,r13
    mov #-0x50,r0
    bt/s .Lmt7_packets_block_0584
.Lmt7_packets_block_0582:
    shlr r13
.Lmt7_packets_block_0583:
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0582
    add r0,r2
    bra .Lmt7_packets_block_0572
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0584:
    mov.l .Lmt7_packets_block_0589,r2
    fschg
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
    add #-0x20,r4
    fmov.s @r2+,fr8
    mov r6,r1
    shar r1
    fmov.s @r2+,fr10
    fmul fr13,fr9
    mov.l @r4,r0
    fmul fr14,fr10
    fmov.s @r14+,fr2
    fmul fr15,fr11
    mov.l .Lmt7_packets_block_0592,r13
    dt r3
    fmov.s fr10,@-r6
    mov r4,r14
    fmov.s fr8,@-r6
    add #-0x8,r6
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    bt/s .Lmt7_packets_block_0586
    fmov.s fr4,@-r6
    tst #0x1,r0
    mov.l r6,@r6
    bt .Lmt7_packets_block_0585
    bra .Lmt7_packets_block_0569
.Lmt7_packets_block_0585:
    pref @r6
    add #0x8,r4
    mov.l @(0x4,r14),r14
    add #0x20,r6
    mov.l .Lmt7_packets_block_0590,r0
    add r4,r14
    mov.l @r13,r13
    add #0x40,r5
    fmov.s @r0+,fr12
    mov r6,r1
    fmov.s @r0+,fr14
    add #0x20,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    bra .Lmt7_packets_block_0571
    pref @r1
.Lmt7_packets_block_0586:
    mov.l @r4+,r0
    mov.l r1,@r6
    cmp/pl r0
    pref @r6
    add #0x20,r6
    bf .Lmt7_packets_block_0588
    tst #0x80,r0
    mov.l .Lmt7_packets_block_0592,r13
    bt .Lmt7_packets_block_0588
    mov.l @r4+,r3
    mov r6,r1
    mov.l @r4,r0
    mov r4,r14
    mov.l @r13,r13
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0590,r0
    add #0x40,r5
    bt .Lmt7_packets_block_0587
    bra .Lmt7_packets_block_0570
.Lmt7_packets_block_0587:
    fmov.s @r0+,fr12
    add #0x8,r4
    mov.l @(0x4,r14),r14
    add #0x50,r6
    fmov.s @r0+,fr14
    add r4,r14
    pref @r1
    fmov.s @r14+,fr4
    add #0x20,r4
    fmov.s @r14+,fr6
    bra .Lmt7_packets_block_0571
    fschg
.Lmt7_packets_block_0588:
    add #-0x4,r4
    fschg
    add #0x40,r5
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
.Lmt7_packets_block_0589:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace
.Lmt7_packets_block_0590:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x240
.Lmt7_packets_block_0591:
    .long __Q37shenmue2dc3sdk.g_lightRecords
.Lmt7_packets_block_0592:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x250
.Lmt7_packets_block_0593:
    mov.l @r4+,r3
    fldi1 fr2
    fschg
    fldi1 fr3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0594
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0594:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0595:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0600
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0596
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0596:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0598
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0597
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0597:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0595
    add #0x38,r6
    .word 0x0009
.Lmt7_packets_block_0598:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0599
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0599
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0597
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0597
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0599:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0602
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0600:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0601
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0601
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0596
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0596
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0601:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0602:
    fmul fr7,fr6
    mov r6,r2
    shar r2
    fmul fr7,fr5
    fmov.s fr0,@-r6
    add #-0x8,r4
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0603:
    cmp/pz r8
    bt .Lmt7_packets_block_0604
    bra .Lmt7_packets_block_0629
    nop
.Lmt7_packets_block_0604:
    mov.l @r4+,r9
    mov #0x3,r3
    fldi0 fr0
    add #0x40,r6
    fldi0 fr1
    fschg
    fmov.s fr0,@-r6
    fmov.s fr0,@-r6
    fschg
.Lmt7_packets_block_0605:
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0628,r13
    bf/s .Lmt7_packets_block_0606
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0606:
    mov.l .Lmt7_packets_block_0626,r0
    fschg
    mov.l @r13,r13
    fmov.s @r0+,fr12
    fmov.s @r0+,fr14
    fmov.s @r14+,fr4
    add #0x20,r4
    fmov.s @r14+,fr6
    bra .Lmt7_packets_block_0608
    fschg
.Lmt7_packets_block_0607:
    mov.l .Lmt7_packets_block_0626,r0
    add #0x20,r6
    mov.l @r13,r13
    mov r6,r1
    fmov.s @r0+,fr12
    add #0x40,r5
    fmov.s @r0+,fr14
    add #0x40,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    pref @r1
.Lmt7_packets_block_0608:
    fmov fr7,fr8
    tst r13,r13
    fmov.s @r14+,fr9
    bt/s .Lmt7_packets_block_0612
    fmov.s @r14+,fr10
    shlr r13
    mov.l .Lmt7_packets_block_0627,r2
    bf .Lmt7_packets_block_0611
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0609:
    fldi0 fr3
    cmp/eq #0x2,r0
    mov #0x74,r1
    bf/s .Lmt7_packets_block_0613
    add r2,r1
    fldi1 fr7
    tst r13,r13
    add #0xc,r1
    bf/s .Lmt7_packets_block_0610
    fmov.s @r1+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0610:
    fmov.s @r1+,fr1
    mov r2,r0
    fmov.s @r1+,fr2
    add #0x18,r0
    fipr fv0,fv8
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    fmov.s @r0+,fr3
    fldi0 fr0
    fcmp/gt fr11,fr0
    fmov fr11,fr0
    bf/s .Lmt7_packets_block_0617
    tst r13,r13
.Lmt7_packets_block_0611:
    mov #-0x50,r0
    bt .Lmt7_packets_block_0621
    shlr r13
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0619
    add r0,r2
    bra .Lmt7_packets_block_0609
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0612:
    fldi1 fr7
    bra .Lmt7_packets_block_0621
    ftrv xmtrx,fv4
    .word 0x0009
.Lmt7_packets_block_0613:
    fmov.s @r1+,fr0
    tst r13,r13
    fmov.s @r1+,fr1
    fsub fr4,fr0
    fmov.s @r1+,fr2
    fsub fr5,fr1
    fldi1 fr7
    add #0x18,r1
    fldi0 fr11
    bf/s .Lmt7_packets_block_0614
    fsub fr6,fr2
    ftrv xmtrx,fv4
.Lmt7_packets_block_0614:
    fipr fv0,fv8
    fldi0 fr12
    fipr fv0,fv0
    fcmp/gt fr11,fr12
    fmov.s @r1,fr12
    bt .Lmt7_packets_block_0618
    fcmp/gt fr3,fr12
    bf .Lmt7_packets_block_0618
    cmp/eq #0x3,r0
    flds fr3,FPUL
    bt/s .Lmt7_packets_block_0615
    fsrra fr3
    add #-0x18,r1
    fmov.s @r1+,fr12
    fmul fr0,fr12
    fmov.s @r1+,fr0
    fmac fr0,fr1,fr12
    fmov.s @r1,fr0
    add #-0x30,r1
    fmac fr0,fr2,fr12
    fmov.s @r1,fr2
    fneg fr12
    fmul fr3,fr12
    add #0x44,r1
    fcmp/gt fr12,fr2
    fsub fr2,fr12
    fmov.s @r1,fr2
    bt .Lmt7_packets_block_0618
    fmul fr2,fr12
    add #-0x4,r1
    fmul fr12,fr11
.Lmt7_packets_block_0615:
    add #-0x44,r1
    fmul fr3,fr11
    add #-0x8,r1
    fmul fr3,fr3
    fmov.s @r1,fr12
    fmul fr3,fr3
    fldi1 fr2
    mov r2,r0
    fmul fr3,fr3
    add #0x18,r0
    fmul fr3,fr12
    fmov fr11,fr0
    fcmp/gt fr2,fr12
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    bt/s .Lmt7_packets_block_0616
    fmov.s @r0+,fr3
    fmul fr12,fr0
.Lmt7_packets_block_0616:
    tst r13,r13
.Lmt7_packets_block_0617:
    fmac fr0,fr1,fr13
    mov #-0x50,r0
    fmac fr0,fr2,fr14
    bt/s .Lmt7_packets_block_0621
    fmac fr0,fr3,fr15
    bra .Lmt7_packets_block_0620
    shlr r13
.Lmt7_packets_block_0618:
    tst r13,r13
    mov #-0x50,r0
    bt/s .Lmt7_packets_block_0621
.Lmt7_packets_block_0619:
    shlr r13
.Lmt7_packets_block_0620:
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0619
    add r0,r2
    bra .Lmt7_packets_block_0609
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0621:
    mov.l .Lmt7_packets_block_0625,r2
    fschg
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
    add #-0x20,r4
    fmov.s @r2+,fr8
    mov r6,r1
    shar r1
    fmov.s @r2+,fr10
    fmul fr13,fr9
    mov.l @r4,r0
    fmul fr14,fr10
    fmov.s @r14+,fr2
    fmul fr15,fr11
    mov.l .Lmt7_packets_block_0628,r13
    dt r3
    fmov.s fr10,@-r6
    mov r4,r14
    fmov.s fr8,@-r6
    add #-0x8,r6
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    bt/s .Lmt7_packets_block_0623
    fmov.s fr4,@-r6
    tst #0x1,r0
    mov.l r6,@r6
    bt .Lmt7_packets_block_0622
    bra .Lmt7_packets_block_0607
.Lmt7_packets_block_0622:
    pref @r6
    add #0x8,r4
    mov.l @(0x4,r14),r14
    add #0x20,r6
    mov.l .Lmt7_packets_block_0626,r0
    add r4,r14
    mov.l @r13,r13
    add #0x40,r5
    fmov.s @r0+,fr12
    mov r6,r1
    fmov.s @r0+,fr14
    add #0x20,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    bra .Lmt7_packets_block_0608
    pref @r1
.Lmt7_packets_block_0623:
    mov.l r1,@r6
    dt r9
    pref @r6
    add #0x20,r6
    fschg
    add #0x40,r5
    pref @r6
    add #-0x20,r6
    bt/s .Lmt7_packets_block_0624
    add #0x70,r6
    bra .Lmt7_packets_block_0605
    mov #0x3,r3
.Lmt7_packets_block_0624:
    rts
    add #-0x30,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0625:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace
.Lmt7_packets_block_0626:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x240
.Lmt7_packets_block_0627:
    .long __Q37shenmue2dc3sdk.g_lightRecords
.Lmt7_packets_block_0628:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x250
.Lmt7_packets_block_0629:
    mov.l @r4+,r9
    fldi1 fr2
    fschg
    fldi1 fr3
    mov #0x3,r3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0630
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0630:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0631:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0636
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0632
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0632:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0634
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0633
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0633:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0631
    add #0x38,r6
.Lmt7_packets_block_0634:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0635
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0633
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0633
    add #-0x8,r14
.Lmt7_packets_block_0635:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0638
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0636:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0637
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0632
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0632
    add #-0x8,r14
.Lmt7_packets_block_0637:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0638:
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    add #-0x8,r4
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0639:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0640:
    mov #0x0,r11
    bsr .Lmt7_packets_block_0665
    mov #0x0,r9
    bt/s .Lmt7_packets_block_0642
    rotcl r9
    bsr .Lmt7_packets_block_0682
    add #0x1,r11
    bsr .Lmt7_packets_block_0665
    nop
    bt/s .Lmt7_packets_block_0648
    rotcl r9
    bsr .Lmt7_packets_block_0682
    add #0x1,r11
.Lmt7_packets_block_0641:
    bsr .Lmt7_packets_block_0665
    nop
    bt/s .Lmt7_packets_block_0658
    rotcl r9
    bsr .Lmt7_packets_block_0682
    add #0x1,r11
    bf .Lmt7_packets_block_0641
    bra .Lmt7_packets_block_0646
    nop
.Lmt7_packets_block_0642:
    dt r3
    add #0x1,r11
    bsr .Lmt7_packets_block_0665
    dt r3
    bf/s .Lmt7_packets_block_0652
    rotcl r9
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf/s .Lmt7_packets_block_0643
    fldi1 fr7
    add #-0x18,r4
    mov.l @(0x4,r14),r14
    add r4,r14
.Lmt7_packets_block_0643:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr5
.Lmt7_packets_block_0644:
    fmov.s @r14+,fr6
    mov #0x20,r1
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    ftrv xmtrx,fv4
    pref @r4
    add #-0x20,r4
    bf/s .Lmt7_packets_block_0645
    mov r4,r14
    mov #0x8,r1
    mov.l @(0x4,r14),r14
    add #0x8,r14
    add r4,r14
.Lmt7_packets_block_0645:
    flds fr7,FPUL
    sts FPUL,r0
    cmp/hi r0,r12
    add #0x1,r11
    bf/s .Lmt7_packets_block_0656
    rotcl r9
    dt r3
    fldi1 fr7
    bt .Lmt7_packets_block_0646
    add r1,r4
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0644
    fmov.s @r14+,fr5
.Lmt7_packets_block_0646:
    mov.l @r4+,r0
    cmp/pl r0
    bf/s .Lmt7_packets_block_0647
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0640
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0647:
    lds.l @r15+,PR
    mov #0x1,r0
    mov.l @r15+,r11
    add #-0x4,r4
    mov.l @r15+,r10
    rts
    or r0,r12
.Lmt7_packets_block_0648:
    bsr .Lmt7_packets_block_0684
    dt r3
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0649
    add #-0x18,r4
.Lmt7_packets_block_0649:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0650
    add #-0x18,r4
.Lmt7_packets_block_0650:
    bsr .Lmt7_packets_block_0665
    nop
    bsr .Lmt7_packets_block_0684
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0688
    mov #0x0,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0651
    add #0x18,r4
.Lmt7_packets_block_0651:
    bra .Lmt7_packets_block_0663
    nop
.Lmt7_packets_block_0652:
    bsr .Lmt7_packets_block_0684
    mov #0x1,r11
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0653
    add #-0x18,r4
.Lmt7_packets_block_0653:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0654
    add #-0x18,r4
.Lmt7_packets_block_0654:
    bsr .Lmt7_packets_block_0665
    nop
    bsr .Lmt7_packets_block_0684
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0688
    mov #0x0,r0
    clrt
    bsr .Lmt7_packets_block_0686
    mov #0x1,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0655
    add #0x18,r4
.Lmt7_packets_block_0655:
    bra .Lmt7_packets_block_0663
    nop
.Lmt7_packets_block_0656:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0657
    add #-0x18,r4
.Lmt7_packets_block_0657:
    bsr .Lmt7_packets_block_0665
    nop
.Lmt7_packets_block_0658:
    bsr .Lmt7_packets_block_0684
    nop
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0659
    add #-0x18,r4
.Lmt7_packets_block_0659:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0660
    add #-0x18,r4
.Lmt7_packets_block_0660:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0661
    add #-0x18,r4
.Lmt7_packets_block_0661:
    bsr .Lmt7_packets_block_0665
    nop
    bsr .Lmt7_packets_block_0684
    add #-0x2,r11
    bsr .Lmt7_packets_block_0665
    nop
    bsr .Lmt7_packets_block_0684
    add #0x1,r11
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0662
    add #0x18,r4
.Lmt7_packets_block_0662:
    mov r12,r0
    add #0x1,r11
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0664
    mov r9,r0
    bsr .Lmt7_packets_block_0685
    nop
    bra .Lmt7_packets_block_0664
    mov r9,r0
.Lmt7_packets_block_0663:
    bsr .Lmt7_packets_block_0665
    add #0x1,r11
    bsr .Lmt7_packets_block_0684
    rotcl r9
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0664
    mov r9,r0
    bsr .Lmt7_packets_block_0685
    nop
    mov r9,r0
.Lmt7_packets_block_0664:
    and #0x7,r0
    shll2 r0
    braf r0
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa00c
    .word 0x0009
    .word 0xa019
    .word 0x0009
    .word 0xa024
    .word 0x0009
    .word 0xa02a
    .word 0x0009
    .word 0xa038
    .word 0x0009
    .word 0xa03d
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb168
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb13d
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb160
    .word 0x70ff
    .word 0x8bd3
    .word 0xaf5d
    .word 0x0009
    .word 0x0008
    .word 0xb133
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb156
    .word 0x70ff
    .word 0x4310
    .word 0xb12c
    .word 0x60b3
    .word 0x8bc6
    .word 0xaf50
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb14c
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf22
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb144
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb119
    .word 0x70ff
    .word 0x4310
    .word 0xb116
    .word 0x60b3
    .word 0x8901
    .word 0xaf05
    .word 0x7b01
    .word 0xaf38
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb134
    .word 0x70ff
    .word 0x8ba7
    .word 0xaf31
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x8903
    .word 0x60b3
    .word 0x0008
    .word 0xb12a
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb126
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb122
    .word 0x70ff
    .word 0x4310
    .word 0xb0f8
    .word 0x60b3
    .word 0x8b92
    .word 0xaf1c
    .word 0x0009
.Lmt7_packets_block_0665:
    mov.l @r4,r0
    fschg
    mov.l .Lmt7_packets_block_0696,r13
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0695,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0666
    add #0x20,r4
    add #-0x18,r4
    mov.l @(0x4,r14),r14
    add r4,r14
.Lmt7_packets_block_0666:
    mov.l @r13,r13
    fmov.s @r0+,fr12
    fmov.s @r0+,fr14
    fmov.s @r14+,fr4
    fmov.s @r14+,fr6
    fschg
    cmp/pz r8
    fmov fr7,fr8
    bf/s .Lmt7_packets_block_0670
    tst r13,r13
    fmov.s @r14+,fr9
    bt/s .Lmt7_packets_block_0671
    fmov.s @r14+,fr10
    shlr r13
    mov.l .Lmt7_packets_block_0698,r2
    bf .Lmt7_packets_block_0669
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0667:
    fldi0 fr3
    cmp/eq #0x2,r0
    mov #0x74,r1
    bf/s .Lmt7_packets_block_0672
    add r2,r1
    fldi1 fr7
    tst r13,r13
    add #0xc,r1
    bf/s .Lmt7_packets_block_0668
    fmov.s @r1+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0668:
    fmov.s @r1+,fr1
    mov r2,r0
    fmov.s @r1+,fr2
    add #0x18,r0
    fipr fv0,fv8
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    fmov.s @r0+,fr3
    fldi0 fr0
    fcmp/gt fr11,fr0
    fmov fr11,fr0
    bf/s .Lmt7_packets_block_0676
    tst r13,r13
.Lmt7_packets_block_0669:
    mov #-0x50,r0
    bt .Lmt7_packets_block_0681
    shlr r13
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0678
    add r0,r2
    bra .Lmt7_packets_block_0667
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0670:
    fldi1 fr7
    add #0x8,r14
    bra .Lmt7_packets_block_0680
    ftrv xmtrx,fv4
.Lmt7_packets_block_0671:
    fldi1 fr7
    bra .Lmt7_packets_block_0681
    ftrv xmtrx,fv4
    .word 0x0009
.Lmt7_packets_block_0672:
    fmov.s @r1+,fr0
    tst r13,r13
    fmov.s @r1+,fr1
    fsub fr4,fr0
    fmov.s @r1+,fr2
    fsub fr5,fr1
    fldi1 fr7
    add #0x18,r1
    fldi0 fr11
    bf/s .Lmt7_packets_block_0673
    fsub fr6,fr2
    ftrv xmtrx,fv4
.Lmt7_packets_block_0673:
    fipr fv0,fv8
    fldi0 fr12
    fipr fv0,fv0
    fcmp/gt fr11,fr12
    fmov.s @r1,fr12
    bt .Lmt7_packets_block_0677
    fcmp/gt fr3,fr12
    bf .Lmt7_packets_block_0677
    cmp/eq #0x3,r0
    flds fr3,FPUL
    bt/s .Lmt7_packets_block_0674
    fsrra fr3
    add #-0x18,r1
    fmov.s @r1+,fr12
    fmul fr0,fr12
    fmov.s @r1+,fr0
    fmac fr0,fr1,fr12
    fmov.s @r1,fr0
    add #-0x30,r1
    fmac fr0,fr2,fr12
    fmov.s @r1,fr2
    fneg fr12
    fmul fr3,fr12
    add #0x44,r1
    fcmp/gt fr12,fr2
    fsub fr2,fr12
    fmov.s @r1,fr2
    bt .Lmt7_packets_block_0677
    fmul fr2,fr12
    add #-0x4,r1
    fmul fr12,fr11
.Lmt7_packets_block_0674:
    add #-0x44,r1
    fmul fr3,fr11
    add #-0x8,r1
    fmul fr3,fr3
    fmov.s @r1,fr12
    fmul fr3,fr3
    fldi1 fr2
    mov r2,r0
    fmul fr3,fr3
    add #0x18,r0
    fmul fr3,fr12
    fmov fr11,fr0
    fcmp/gt fr2,fr12
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    bt/s .Lmt7_packets_block_0675
    fmov.s @r0+,fr3
    fmul fr12,fr0
.Lmt7_packets_block_0675:
    tst r13,r13
.Lmt7_packets_block_0676:
    fmac fr0,fr1,fr13
    mov #-0x50,r0
    fmac fr0,fr2,fr14
    bt/s .Lmt7_packets_block_0681
    fmac fr0,fr3,fr15
    bra .Lmt7_packets_block_0679
    shlr r13
.Lmt7_packets_block_0677:
    tst r13,r13
    mov #-0x50,r0
    bt/s .Lmt7_packets_block_0681
.Lmt7_packets_block_0678:
    shlr r13
.Lmt7_packets_block_0679:
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0678
    add r0,r2
    bra .Lmt7_packets_block_0667
    mov.w @(0x2,r2),r0
    .word 0x0009
.Lmt7_packets_block_0680:
    fldi1 fr13
    fldi1 fr15
    fldi1 fr14
.Lmt7_packets_block_0681:
    flds fr7,FPUL
    add #0x20,r4
    pref @r4
    fschg
    fldi1 fr7
    fdiv fr4,fr7
    mov.l .Lmt7_packets_block_0694,r2
    add #-0x20,r4
    sts FPUL,r0
    fmov.s @r2+,fr8
    fmov.s @r2+,fr10
    cmp/hi r0,r12
    fmul fr13,fr9
    mov.l @r14+,r1
    fmul fr14,fr10
    mov.l @r14+,r2
    fmul fr15,fr11
    fsts FPUL,fr4
    rts
    fschg
.Lmt7_packets_block_0682:
    dt r3
    lds r6,FPUL
    bf .Lmt7_packets_block_0683
    mov r6,r0
    shar r0
    sett
    lds r0,FPUL
.Lmt7_packets_block_0683:
    fschg
    add #0x18,r6
    fsts FPUL,fr4
    fmul fr7,fr6
    mov.l r2,@-r6
    fmul fr7,fr5
    mov.l r1,@-r6
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    pref @r6
    add #0x40,r6
    fldi0 fr0
    fldi0 fr1
    fmov.s fr0,@-r6
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    add #0x40,r5
    fmov.s fr8,@-r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0684:
    mov #0x6,r10
    mov r11,r0
    and #0x3,r0
    fldi0 fr0
    shld r10,r0
    fldi0 fr1
    fschg
    mov.l .Lmt7_packets_block_0693,r10
    add r0,r10
    fmov.s fr0,@-r10
    fmov.s fr0,@-r10
    fmov.s fr10,@-r10
    fmov.s fr8,@-r10
    add #-0x8,r10
    mov.l r2,@-r10
    mov.l r1,@-r10
    fmov.s fr6,@-r10
    fmov.s fr4,@-r10
    rts
    fschg
.Lmt7_packets_block_0685:
    mov #0x6,r1
    mov r11,r0
    and #0x3,r0
    mov.l .Lmt7_packets_block_0692,r10
    shld r1,r0
    fschg
    mov r10,r1
    add r0,r1
    fmov.s @r1+,fr12
    add #-0x40,r0
    fmov.s @r1+,fr14
    and #0xff,r0
    add #0x10,r10
    mov r10,r1
    add r0,r1
    fmov.s fr14,@-r1
    add #-0x40,r0
    fmov.s fr12,@-r1
    and #0xff,r0
    mov r10,r1
    add r0,r1
    fmov.s fr14,@-r1
    fmov.s fr12,@-r1
    rts
    fschg
.Lmt7_packets_block_0686:
    mov #0x6,r1
    mov.l .Lmt7_packets_block_0691,r10
    and #0x3,r0
    fschg
    shld r1,r0
    fldi0 fr6
    add r0,r10
    fldi0 fr7
    mov #-0x10,r2
    fmov.s @r10+,fr0
    add #0x40,r6
    fmov.s @r10+,fr2
    add #-0x30,r10
    fmov.s fr6,@-r6
    fmov.s fr6,@-r6
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    fmov.s @r10+,fr0
    shll16 r2
    fmov.s @r10+,fr2
    fmov.s @r10+,fr4
    fmul fr3,fr2
    fmov.s @r10+,fr6
    fmul fr3,fr1
    shll8 r2
    fmov.s fr6,@-r6
    add #0x40,r5
    fmov.s fr4,@-r6
    fmov.s fr2,@-r6
    bt/s .Lmt7_packets_block_0687
    fmov.s fr0,@-r6
    mov r6,r2
.Lmt7_packets_block_0687:
    mov.l r2,@r6
    pref @r6
    add #0x20,r6
    pref @r6
    fschg
    rts
    add #0x20,r6
.Lmt7_packets_block_0688:
    mov #0x6,r1
    mov.l .Lmt7_packets_block_0690,r10
    and #0x3,r0
    mov r10,r2
    shld r1,r0
    lds r12,FPUL
    add r0,r10
    fsts FPUL,fr0
    mov r11,r0
    fmov.s @r10,fr2
    and #0x3,r0
    shld r1,r0
    fsub fr2,fr0
    add r0,r2
    fmov.s @r2,fr1
    fschg
    add #0x10,r2
    fsub fr2,fr1
    add #0x10,r10
    lds r7,FPUL
    fdiv fr1,fr0
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    fsub fr2,fr4
    add #-0x8,r10
    fsub fr3,fr5
    fsts FPUL,fr1
    fmac fr0,fr4,fr2
    add #0x18,r6
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fschg
    fmov.s fr1,@-r6
    fmov.s @r2,fr5
    add #-0x4,r2
    fmov.s @r10,fr3
    add #-0x4,r10
    fmov.s @r2,fr4
    fmov.s @r10,fr2
    fsub fr3,fr5
    fsub fr2,fr4
    fmac fr0,fr5,fr3
    add #0x34,r10
    fmac fr0,fr4,fr2
    mov r6,r0
    fmul fr1,fr3
    add #0x40,r5
    fmul fr1,fr2
    fmov.s fr3,@-r6
    bf/s .Lmt7_packets_block_0689
    fmov.s fr2,@-r6
    shar r0
    sett
.Lmt7_packets_block_0689:
    mov.l r0,@-r6
    add #0x34,r2
    pref @r6
    add #0x40,r6
    fschg
    fldi0 fr2
    add #-0x10,r2
    fldi0 fr3
    add #-0x10,r10
    fmov.s fr2,@-r6
    fmov.s fr2,@-r6
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    fmov.s @r2,fr6
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s @r10,fr4
    fmov.s fr2,@-r6
    fsub fr4,fr6
    fsub fr5,fr7
    fmac fr0,fr6,fr4
    fmac fr0,fr7,fr5
    fmov.s fr4,@-r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0690:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x40
.Lmt7_packets_block_0691:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x60
.Lmt7_packets_block_0692:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x60
.Lmt7_packets_block_0693:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x80
.Lmt7_packets_block_0694:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace
.Lmt7_packets_block_0695:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x240
.Lmt7_packets_block_0696:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x250
.Lmt7_packets_block_0697:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x240
.Lmt7_packets_block_0698:
    .long __Q37shenmue2dc3sdk.g_lightRecords
.Lmt7_packets_block_0699:
    .long __Q37shenmue2dc3sdk.g_mt7FloatingLightWorkspace+0x250
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0700:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0701:
    bsr .Lmt7_packets_block_0709
    mov #0x0,r9
    mov r9,r0
    cmp/eq #0x0,r0
    bt .Lmt7_packets_block_0704
    cmp/eq #0x7,r0
    bt .Lmt7_packets_block_0705
    bsr .Lmt7_packets_block_0665
    mov #0x0,r11
    bsr .Lmt7_packets_block_0684
    nop
    bsr .Lmt7_packets_block_0665
    mov #0x1,r11
    bsr .Lmt7_packets_block_0684
    nop
    bsr .Lmt7_packets_block_0665
    mov #0x2,r11
    bsr .Lmt7_packets_block_0684
    nop
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0702
    shll2 r9
    bsr .Lmt7_packets_block_0685
    nop
.Lmt7_packets_block_0702:
    braf r9
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa034
    .word 0x0009
    .word 0xa041
    .word 0x0009
    .word 0xa04f
    .word 0x0009
    .word 0xa05a
    .word 0x0009
    .word 0xa068
    .word 0x0009
    .word 0xa073
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0703:
    dt r3
    bf .Lmt7_packets_block_0701
    mov #0x1,r0
    lds.l @r15+,PR
    or r0,r12
    mov.l @r15+,r11
    rts
    mov.l @r15+,r10
.Lmt7_packets_block_0704:
    mov r3,r9
    bsr .Lmt7_packets_block_0665
    mov #0x3,r3
    bsr .Lmt7_packets_block_0682
    nop
    bsr .Lmt7_packets_block_0665
    nop
    bsr .Lmt7_packets_block_0682
    nop
    bsr .Lmt7_packets_block_0665
    nop
    bsr .Lmt7_packets_block_0682
    nop
    bra .Lmt7_packets_block_0703
    mov r9,r3
.Lmt7_packets_block_0705:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0706
    add #0x18,r4
.Lmt7_packets_block_0706:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0707
    add #0x18,r4
.Lmt7_packets_block_0707:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0708
    add #0x18,r4
.Lmt7_packets_block_0708:
    bra .Lmt7_packets_block_0703
    nop
    .word 0x0008
    .word 0xbf05
    .word 0xe000
    .word 0x0008
    .word 0xbf02
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf25
    .word 0xe000
    .word 0x0018
    .word 0xbf22
    .word 0xe001
    .word 0xafc9
    .word 0x0009
    .word 0x0008
    .word 0xbef6
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf19
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbeee
    .word 0xe002
    .word 0x0018
    .word 0xbf12
    .word 0xe001
    .word 0xafb9
    .word 0x0009
    .word 0x0008
    .word 0xbee6
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf09
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbf05
    .word 0xe000
    .word 0xafac
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbeff
    .word 0xe000
    .word 0x0008
    .word 0xbed5
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbef8
    .word 0xe000
    .word 0x0018
    .word 0xbece
    .word 0xe002
    .word 0xaf9c
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbeef
    .word 0xe000
    .word 0x0008
    .word 0xbec5
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbee8
    .word 0xe001
    .word 0xaf8f
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbee2
    .word 0xe000
    .word 0x0008
    .word 0xbedf
    .word 0xe001
    .word 0x0018
    .word 0xbeb5
    .word 0xe002
    .word 0xaf83
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0709:
    mov r4,r1
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0710
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0711
    fldi1 fr7
.Lmt7_packets_block_0710:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
    add #0x18,r4
.Lmt7_packets_block_0711:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv4
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0712
    fmov.s @r4+,fr0
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr0
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    bra .Lmt7_packets_block_0713
    fldi1 fr3
.Lmt7_packets_block_0712:
    fmov.s @r4+,fr1
    fmov.s @r4,fr2
    fldi1 fr3
    add #0x18,r4
.Lmt7_packets_block_0713:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv0
    flds fr7,FPUL
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0714
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0715
    fldi1 fr7
.Lmt7_packets_block_0714:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
.Lmt7_packets_block_0715:
    mov r1,r4
    sts FPUL,r0
    ftrv xmtrx,fv4
    cmp/hi r0,r12
    flds fr3,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    flds fr7,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    rts
    rotcl r9
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
_shenmue_dc_mt7_alternate_packets:
    fldi1 fr4
_shenmue_dc_mt7_alternate_packets_alpha:
    add #0x18,r4
    mov.l .Lmt7_packets_block_0718,r7
    fldi1 fr1
    fmov.s @r7,fr0
    fmov.s fr15,@-r15
    fmov.s fr14,@-r15
    fdiv fr0,fr1
    fmov.s fr13,@-r15
    fmov.s fr12,@-r15
    mov.l r14,@-r15
    mov.l r13,@-r15
    mov.l r12,@-r15
    mov.l r11,@-r15
    mov.l r10,@-r15
    mov.l r9,@-r15
    mov.l r8,@-r15
    mov #0x0,r0
    sts.l PR,@-r15
    add #-0x48,r15
    fldi1 fr7
    fmov fr4,@(r0,r15)
    mov #0x4,r0
    fabs fr0
    fmov fr0,@(r0,r15)
    mov.l .Lmt7_packets_block_0754,r0
    fmov.s fr7,@r7
    mov.l @r0,r1
    mov.l .Lmt7_packets_block_0756,r2
    mov.l r1,@(0x8,r15)
    mov.l @r2,r1
    mov.l .Lmt7_packets_block_0757,r0
    mov.l .Lmt7_packets_block_0755,r2
    mov.l r1,@(0x10,r15)
    mov.l @r2,r3
    mov.l @r0,r1
    mov.l r3,@(0xc,r15)
    mov.l r1,@(0x14,r15)
    add #0xc,r5
    mov.l @r5+,r12
    add #0x10,r5
    mov.l @r5+,r7
    mov.l .Lmt7_packets_block_0752,r0
    mov.l @r0+,r1
    mov.l @r0+,r2
    mov.l @r0+,r3
    mov.l r1,@(0x18,r15)
    mov.l r2,@(0x1c,r15)
    mov.l r3,@(0x20,r15)
    mov.l .Lmt7_packets_block_0765,r0
    mov.l @r0+,r5
    mov.l r5,@(0x28,r15)
    mov.l @r0+,r6
    mov.l r6,@(0x2c,r15)
    mov.l .Lmt7_packets_block_0750,r10
    mov r15,r3
    mov.l @r4+,r8
    add #0x4,r3
    mov.l @r10,r10
    mov #-0x1,r11
    fmov fr4,fr11
    or r5,r8
    fcmp/gt fr11,fr7
    fmov.s @r4+,fr2
    or r6,r1
    mov.l @r4+,r1
    bf/s .Lmt7_packets_block_0721
    fmov.s @r4+,fr3
    bt .Lmt7_packets_block_0720
    .word 0x0009
.Lmt7_packets_block_0718:
    .long __Q37shenmue2dc3sdk.g_modelScale
.Lmt7_packets_block_0719:
    mov.l @(0x28,r15),r3
    mov.l @(0x2c,r15),r0
    or r3,r8
    mov r15,r3
    fmov.s @r3+,fr11
    or r0,r1
    fldi1 fr7
    fcmp/gt fr11,fr7
    mov r4,r0
    add #0x30,r0
    bf/s .Lmt7_packets_block_0721
    pref @r0
.Lmt7_packets_block_0720:
    mov #0x2,r2
    mov r1,r0
    shll16 r2
    shll8 r2
    mov.l .Lmt7_packets_block_0764,r9
    or r2,r8
    or #0xc0,r0
    swap.w r0,r0
    mov.l @r9,r9
    or #0x10,r0
    swap.b r0,r0
    or #0xfc,r0
    xor r9,r0
    swap.b r0,r0
    swap.w r0,r0
    add r2,r2
    mov r0,r1
    not r2,r2
    and r2,r8
.Lmt7_packets_block_0721:
    mov r8,r0
    fschg
    shlr16 r0
    fmov.s @r4+,fr4
    shlr8 r0
    fmov.s @r4+,fr6
    and #0x7,r0
    fschg
    mov #0x4,r2
    fmov fr7,fr0
    shll2 r0
    fldi1 fr7
    shll8 r2
    fmov.s @r3+,fr1
    cmp/eq r0,r11
    ftrv xmtrx,fv4
    mov r0,r11
    mov #-0x20,r6
    fmul fr1,fr0
    shll16 r2
    bt/s .Lmt7_packets_block_0722
    mov.l @(r0,r10),r5
    mov #-0x1,r0
    mov r5,r9
    shll16 r0
    shlr16 r9
    shll8 r0
    shlr8 r9
    or #0x38,r0
    mov.l r9,@r0
    mov.l r9,@(0x4,r0)
.Lmt7_packets_block_0722:
    add #-0x1,r2
    lds r12,FPUL
    mov #-0x2,r0
    fadd fr0,fr7
    and r5,r2
    fsts FPUL,fr1
    shll8 r6
    fcmp/gt fr1,fr7
    shll16 r6
    fsub fr0,fr7
    or r2,r6
    mov.l .Lmt7_packets_block_0751,r9
    and r0,r12
    bf/s .Lmt7_packets_block_0726
    fsub fr0,fr7
    fcmp/gt fr1,fr7
    bf/s .Lmt7_packets_block_0726
    add #0x1,r12
.Lmt7_packets_block_0723:
    mov r4,r2
    add #0x2c,r4
    mov.l @r4+,r0
    mov r8,r1
    add r0,r4
    mov.l @r4+,r8
    tst r8,r8
    bf .Lmt7_packets_block_0724
    bra .Lmt7_packets_block_0746
    nop
.Lmt7_packets_block_0724:
    mov r15,r9
    add #0x18,r9
    fmov.s @r9+,fr5
    fmov.s @r9+,fr6
    add #0xc,r2
    fmov.s @r9+,fr7
    mov r1,r0
    tst #0x10,r0
    bf/s .Lmt7_packets_block_0725
    tst #0x20,r0
    bt .Lmt7_packets_block_0725
    fmov.s @r2+,fr0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    fmov.s @r2+,fr3
    fmul fr11,fr0
    mov.l .Lmt7_packets_block_0753,r0
    fmov.s @r0+,fr11
    fmul fr5,fr1
    fmov.s @r2+,fr4
    fmov.s @r2+,fr5
    fmul fr11,fr5
    fmov.s @r0+,fr11
    fmul fr6,fr2
    fmov.s @r2+,fr6
    fmul fr11,fr6
    fmov.s @r0+,fr11
    fmul fr7,fr3
    fmov.s @r2+,fr7
    fschg
    fmul fr11,fr7
    mov.l .Lmt7_packets_block_0769,r0
    fmov.s fr6,@-r0
    fmov.s fr4,@-r0
    fmov.s fr2,@-r0
    fmov.s fr0,@-r0
    fschg
.Lmt7_packets_block_0725:
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0719
    fmov.s @r4+,fr3
.Lmt7_packets_block_0726:
    fmov.s @r3+,fr1
    mov.l .Lmt7_packets_block_0759,r0
    add #0x10,r9
    fmov.s @r0,fr7
    fmul fr4,fr7
    fneg fr0
    fmul fr0,fr1
    fmov.s fr3,@-r9
    fsub fr7,fr5
    fmov.s @r3+,fr3
    mov r4,r2
    mov.l .Lmt7_packets_block_0758,r0
    fcmp/gt fr1,fr5
    fmov.s @r0,fr7
    fmul fr0,fr3
    bf/s .Lmt7_packets_block_0723
    fmul fr4,fr7
    mov.l r1,@-r9
    fsub fr7,fr6
    fmov.s @r3+,fr0
    fcmp/gt fr3,fr6
    fmov.s @r3+,fr7
    fmul fr4,fr0
    bf/s .Lmt7_packets_block_0723
    fsub fr1,fr0
    fcmp/gt fr0,fr5
    fmov.s @r3+,fr5
    fmul fr4,fr7
    bt/s .Lmt7_packets_block_0723
    fsub fr3,fr7
    fmov.s fr2,@-r9
    mov #0x10,r1
    fcmp/gt fr7,fr6
    fmov.s @r3+,fr6
    bt .Lmt7_packets_block_0723
    mov r8,r0
    or r1,r8
    mov #-0x21,r1
    and r1,r8
    mov.l r8,@-r9
    mov #0x8,r1
    tst #0x10,r0
    mov.b @(0x4,r2),r0
    add #0x8,r2
    fmov.s @r3+,fr7
    mov r0,r8
    fmov.s @r2+,fr15
    bf/s .Lmt7_packets_block_0727
    add #0x30,r4
    fmov.s @r2+,fr0
    mov #-0x2,r0
    fmov.s @r2+,fr1
    fmov.s @r2+,fr2
    cmp/eq r0,r8
    fmov.s @r2+,fr3
    fmul fr11,fr0
    mov.l .Lmt7_packets_block_0753,r0
    fmov.s @r0+,fr11
    fmul fr5,fr1
    fmov.s @r2+,fr4
    fmov.s @r2+,fr5
    fmul fr11,fr5
    fmov.s @r0+,fr11
    fmul fr6,fr2
    fmov.s @r2+,fr6
    fmul fr11,fr6
    fmov.s @r0+,fr11
    fmul fr7,fr3
    fmov.s @r2+,fr7
    fmul fr11,fr7
    bt .Lmt7_packets_block_0728
    mov.l .Lmt7_packets_block_0769,r0
    fschg
    fmov.s fr6,@-r0
    fmov.s fr4,@-r0
    fmov.s fr2,@-r0
    fmov.s fr0,@-r0
    fschg
.Lmt7_packets_block_0727:
    cmp/pz r8
    bt .Lmt7_packets_block_0733
    mov r8,r0
    cmp/eq #-0x1,r0
    bt/s .Lmt7_packets_block_0732
    cmp/eq #-0x2,r0
    bf .Lmt7_packets_block_0730
.Lmt7_packets_block_0728:
    mov.l .Lmt7_packets_block_0761,r0
    fmov.s @r0+,fr14
    fmov.s @r0+,fr12
    fmov.s @r0+,fr13
    mov.l .Lmt7_packets_block_0762,r0
    jsr @r0
    nop
    bf .Lmt7_packets_block_0729
    bra .Lmt7_packets_block_0746
    nop
.Lmt7_packets_block_0729:
    .word 0xf249
    .word 0x6146
    .word 0xaf1a
    .word 0xf349
.Lmt7_packets_block_0730:
    cmp/eq #-0x3,r0
    bf .Lmt7_packets_block_0732
    mov.l .Lmt7_packets_block_0763,r0
    jsr @r0
    nop
    bf .Lmt7_packets_block_0731
    bra .Lmt7_packets_block_0746
    nop
.Lmt7_packets_block_0731:
    .word 0xf249
    .word 0x6146
    .word 0xaf0e
    .word 0xf349
.Lmt7_packets_block_0732:
    mov.l .Lmt7_packets_block_0767,r1
    fldi0 fr0
    add #0x10,r1
    fldi1 fr1
    mov #-0x1,r8
    fmov.s fr1,@-r1
    fmov.s fr1,@-r1
    fmov.s fr1,@-r1
    bra .Lmt7_packets_block_0734
    fldi1 fr15
.Lmt7_packets_block_0733:
    mov.l .Lmt7_packets_block_0766,r0
    mov.l .Lmt7_packets_block_0767,r1
    add #0x4,r0
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    fmul fr15,fr1
    fmul fr15,fr2
    fmov.s @r0+,fr0
    fmov.s fr1,@r1
    add #0x4,r1
    fmul fr15,fr0
    fmov.s fr2,@r1
    add #0x4,r1
    fmov.s fr0,@r1
.Lmt7_packets_block_0734:
    cmp/pz r8
    mov.l .Lmt7_packets_block_0760,r0
    bf .Lmt7_packets_block_0735
    jsr @r0
    nop
.Lmt7_packets_block_0735:
    mov.l @r4+,r0
.Lmt7_packets_block_0736:
    cmp/pl r0
    mov.l .Lmt7_packets_block_0751,r9
    bt/s .Lmt7_packets_block_0738
    cmp/pz r0
    mov r0,r8
    mov r11,r0
    mov.l r5,@(r0,r10)
    bf .Lmt7_packets_block_0737
    bra .Lmt7_packets_block_0746
    nop
.Lmt7_packets_block_0737:
    fmov.s @r4+,fr2
    mov.l @r4+,r1
    bra .Lmt7_packets_block_0719
    fmov.s @r4+,fr3
    .word 0x0009
.Lmt7_packets_block_0738:
    mov r0,r1
    mov #-0x5,r2
    mov.l @r9+,r3
    and #0x40,r0
    shld r2,r0
    mov #-0x3,r2
    and r2,r3
    or r3,r0
    add #0x20,r4
    pref @r4
    fschg
    lds r0,FPUL
    mov.l @(0x28,r15),r2
    mov r1,r0
    cmp/pz r2
    mov #0x1b,r2
    bt/s .Lmt7_packets_block_0739
    and #0x3,r0
    xor #0x1,r0
.Lmt7_packets_block_0739:
    shld r2,r0
    mov #0x3,r3
    shld r2,r3
    not r3,r3
    mov.l @r9+,r2
    and r3,r2
    fsts FPUL,fr0
    or r0,r2
    lds r2,FPUL
    add #-0x20,r4
    fsts FPUL,fr1
    mov.l .Lmt7_packets_block_0768,r0
    add #0x10,r6
    fmov.s @r9+,fr2
    rotcr r12
    mov.l @r0,r0
    bt/s .Lmt7_packets_block_0740
    rotcl r12
    tst r0,r0
    mov #-0x2,r0
    bt .Lmt7_packets_block_0742
    cmp/pz r8
    bf .Lmt7_packets_block_0742
.Lmt7_packets_block_0740:
    fmov.s fr2,@-r6
.Lmt7_packets_block_0741:
    add #0x20,r5
    fmov.s fr0,@-r6
    fschg
    pref @r6
    bra .Lmt7_packets_block_0744
    add #0x20,r6
.Lmt7_packets_block_0742:
    cmp/eq r0,r8
    fmov.s fr2,@-r6
    bt .Lmt7_packets_block_0741
    add #0x40,r5
    flds fr0,FPUL
    sts FPUL,r0
    or #0x30,r0
    fschg
    xor #0x10,r0
    fmov.s fr1,@-r6
    add #-0x10,r9
    mov.l r0,@-r6
    or #0x30,r0
    pref @r6
    fschg
    mov.l r0,@r9
    add #0x30,r6
    mov.l .Lmt7_packets_block_0769,r0
    add #-0x20,r0
    fmov.s @r0+,fr0
    fmov.s @r0+,fr2
    mov.l .Lmt7_packets_block_0767,r0
    fmov.s @r0+,fr4
    fmul fr5,fr1
    fmov.s @r0+,fr6
    fmul fr6,fr2
    mov #-0x1,r0
    fmul fr7,fr3
    fmov.s fr2,@-r6
    cmp/eq r0,r8
    fmov.s fr0,@-r6
    bt/s .Lmt7_packets_block_0743
    add #0x20,r6
    mov.l .Lmt7_packets_block_0769,r0
    add #-0x10,r0
    fldi1 fr4
    fsub fr4,fr5
    fsub fr4,fr6
    fsub fr4,fr7
    fmov.s @r0+,fr0
    fmov.s @r0+,fr2
    fmul fr5,fr1
    fmul fr6,fr2
    fmul fr7,fr3
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    add #-0x10,r6
    fschg
    pref @r6
    mov #-0x2,r8
    bra .Lmt7_packets_block_0744
    add #0x20,r6
.Lmt7_packets_block_0743:
    fldi0 fr0
    fldi0 fr1
    fmov.s fr0,@-r6
    fmov.s fr0,@-r6
    add #-0x10,r6
    fschg
    pref @r6
    mov #-0x2,r8
    add #0x20,r6
.Lmt7_packets_block_0744:
    rotcr r12
    mov r1,r0
    bt/s .Lmt7_packets_block_0747
    shll r12
    tst #0x8,r0
    bt .Lmt7_packets_block_0745
    bsr .Lmt7_packets_block_0807
    nop
    bra .Lmt7_packets_block_0736
    mov.l @r4+,r0
.Lmt7_packets_block_0745:
    bsr .Lmt7_packets_block_0770
    nop
    bra .Lmt7_packets_block_0736
    mov.l @r4+,r0
.Lmt7_packets_block_0746:
    add #0x48,r15
    lds.l @r15+,PR
    mov.l @r15+,r8
    mov.l @r15+,r9
    mov.l @r15+,r10
    mov.l @r15+,r11
    mov.l @r15+,r12
    mov.l @r15+,r13
    mov.l @r15+,r14
    fmov.s @r15+,fr12
    fmov.s @r15+,fr13
    fmov.s @r15+,fr14
    rts
    fmov.s @r15+,fr15
.Lmt7_packets_block_0747:
    mov #0x0,r1
    tst #0x40,r0
    addc r1,r12
    tst #0x10,r0
    bt/s .Lmt7_packets_block_0748
    tst #0x8,r0
    bsr .Lmt7_packets_block_0843
    tst #0x20,r0
    bra .Lmt7_packets_block_0736
    mov.l @r4+,r0
.Lmt7_packets_block_0748:
    bt .Lmt7_packets_block_0749
    bsr .Lmt7_packets_block_0908
    tst #0x20,r0
    bra .Lmt7_packets_block_0736
    mov.l @r4+,r0
.Lmt7_packets_block_0749:
    trapa #0x58
    bra .Lmt7_packets_block_0749
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0750:
    .long __nj_vertex_buffer_
.Lmt7_packets_block_0751:
    .long __Q37shenmue2dc3sdk.g_mt7PacketScratch
.Lmt7_packets_block_0752:
    .long __Q37shenmue2dc3sdk.g_mt7DiffuseChannels
.Lmt7_packets_block_0753:
    .long __Q37shenmue2dc3sdk.g_mt7SpecularChannels
.Lmt7_packets_block_0754:
    .long __Q37shenmue2dc3sdk.g_mt7HorizontalClipScale
.Lmt7_packets_block_0755:
    .long __Q37shenmue2dc3sdk.g_mt7VerticalClipScale
.Lmt7_packets_block_0756:
    .long __Q27shenmue12matrix_stack.g_projectionViewportWidth
.Lmt7_packets_block_0757:
    .long __Q27shenmue12matrix_stack.g_projectionViewportHeight
.Lmt7_packets_block_0758:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginY
.Lmt7_packets_block_0759:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginX
.Lmt7_packets_block_0760:
    .long _shenmue_dc_mt7_select_packed_lights
.Lmt7_packets_block_0761:
    .long __Q37shenmue2dc3sdk.g_mt7LightingParameters+0x10
.Lmt7_packets_block_0762:
    .long .Lmt7_packets_block_0352
.Lmt7_packets_block_0763:
    .long .Lmt7_packets_block_0423
.Lmt7_packets_block_0764:
    .long __Q37shenmue2dc3sdk.g_mt7AlphaBlendMask
.Lmt7_packets_block_0765:
    .long _shenmue_dc_mt7_draw_controls
.Lmt7_packets_block_0766:
    .long __Q37shenmue2dc3sdk.g_ambientLight
.Lmt7_packets_block_0767:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x280
.Lmt7_packets_block_0768:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x290
.Lmt7_packets_block_0769:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x20
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0770:
    cmp/pz r8
    bt .Lmt7_packets_block_0771
    bra .Lmt7_packets_block_0797
    nop
.Lmt7_packets_block_0771:
    mov.l @r4+,r3
    add #0x30,r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0796,r13
    bf/s .Lmt7_packets_block_0772
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0772:
    mov.l .Lmt7_packets_block_0794,r0
    fschg
    mov.l @r13,r13
    fmov.s @r0+,fr12
    fmov.s @r0+,fr14
    fmov.s @r14+,fr4
    add #0x20,r4
    fmov.s @r14+,fr6
    bra .Lmt7_packets_block_0775
    fschg
    .word 0x0009
.Lmt7_packets_block_0773:
    mov.l .Lmt7_packets_block_0794,r0
    add #0x20,r6
    mov.l @r13,r13
    mov r6,r1
    fmov.s @r0+,fr12
    add #0x40,r5
.Lmt7_packets_block_0774:
    fmov.s @r0+,fr14
    add #0x40,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    pref @r1
.Lmt7_packets_block_0775:
    fmov fr7,fr8
    tst r13,r13
    fmov.s @r14+,fr9
    bt/s .Lmt7_packets_block_0779
    fmov.s @r14+,fr10
    shlr r13
    mov.l .Lmt7_packets_block_0795,r2
    bf .Lmt7_packets_block_0778
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0776:
    fldi0 fr3
    cmp/eq #0x2,r0
    mov #0x74,r1
    bf/s .Lmt7_packets_block_0780
    add r2,r1
    fldi1 fr7
    tst r13,r13
    add #0xc,r1
    bf/s .Lmt7_packets_block_0777
    fmov.s @r1+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0777:
    fmov.s @r1+,fr1
    mov r2,r0
    fmov.s @r1+,fr2
    add #0x18,r0
    fipr fv0,fv8
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    fmov.s @r0+,fr3
    fldi0 fr0
    fcmp/gt fr11,fr0
    fmov fr11,fr0
    bf/s .Lmt7_packets_block_0784
    tst r13,r13
.Lmt7_packets_block_0778:
    mov #-0x50,r0
    bt .Lmt7_packets_block_0788
    shlr r13
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0786
    add r0,r2
    bra .Lmt7_packets_block_0776
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0779:
    fldi1 fr7
    bra .Lmt7_packets_block_0788
    ftrv xmtrx,fv4
    .word 0x0009
.Lmt7_packets_block_0780:
    fmov.s @r1+,fr0
    tst r13,r13
    fmov.s @r1+,fr1
    fsub fr4,fr0
    fmov.s @r1+,fr2
    fsub fr5,fr1
    fldi1 fr7
    add #0x18,r1
    fldi0 fr11
    bf/s .Lmt7_packets_block_0781
    fsub fr6,fr2
    ftrv xmtrx,fv4
.Lmt7_packets_block_0781:
    fipr fv0,fv8
    fldi0 fr12
    fipr fv0,fv0
    fcmp/gt fr11,fr12
    fmov.s @r1,fr12
    bt .Lmt7_packets_block_0785
    fcmp/gt fr3,fr12
    bf .Lmt7_packets_block_0785
    cmp/eq #0x3,r0
    flds fr3,FPUL
    bt/s .Lmt7_packets_block_0782
    fsrra fr3
    add #-0x18,r1
    fmov.s @r1+,fr12
    fmul fr0,fr12
    fmov.s @r1+,fr0
    fmac fr0,fr1,fr12
    fmov.s @r1,fr0
    add #-0x30,r1
    fmac fr0,fr2,fr12
    fmov.s @r1,fr2
    fneg fr12
    fmul fr3,fr12
    add #0x44,r1
    fcmp/gt fr12,fr2
    fsub fr2,fr12
    fmov.s @r1,fr2
    bt .Lmt7_packets_block_0785
    fmul fr2,fr12
    add #-0x4,r1
    fmul fr12,fr11
.Lmt7_packets_block_0782:
    add #-0x44,r1
    fmul fr3,fr11
    add #-0x8,r1
    fmul fr3,fr3
    fmov.s @r1,fr12
    fmul fr3,fr3
    fldi1 fr2
    mov r2,r0
    fmul fr3,fr3
    add #0x18,r0
    fmul fr3,fr12
    fmov fr11,fr0
    fcmp/gt fr2,fr12
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    bt/s .Lmt7_packets_block_0783
    fmov.s @r0+,fr3
    fmul fr12,fr0
.Lmt7_packets_block_0783:
    tst r13,r13
.Lmt7_packets_block_0784:
    fmac fr0,fr1,fr13
    mov #-0x50,r0
    fmac fr0,fr2,fr14
    bt/s .Lmt7_packets_block_0788
    fmac fr0,fr3,fr15
    bra .Lmt7_packets_block_0787
    shlr r13
.Lmt7_packets_block_0785:
    tst r13,r13
    mov #-0x50,r0
    bt/s .Lmt7_packets_block_0788
.Lmt7_packets_block_0786:
    shlr r13
.Lmt7_packets_block_0787:
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0786
    add r0,r2
    bra .Lmt7_packets_block_0776
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0788:
    mov.l .Lmt7_packets_block_0793,r2
    fschg
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
    add #-0x20,r4
    fmov.s @r2+,fr8
    mov r6,r1
    shar r1
    fmov.s @r2+,fr10
    fmul fr13,fr9
    mov.l @r4,r0
    fmul fr14,fr10
    add #0x10,r6
    fmul fr15,fr11
    fmov.s @r14+,fr2
    fldi0 fr12
    fldi1 fr0
    fsub fr0,fr13
    mov.l .Lmt7_packets_block_0793,r2
    fsub fr0,fr14
    add #0x10,r2
    fsub fr0,fr15
    fmov.s @r2+,fr0
    fmul fr1,fr13
    fmov.s @r2+,fr0
    fmul fr0,fr14
    fmul fr1,fr15
    fmov.s fr14,@-r6
    fmov.s fr12,@-r6
    mov.l .Lmt7_packets_block_0796,r13
    dt r3
    fmov.s fr10,@-r6
    mov r4,r14
    fmov.s fr8,@-r6
    add #-0x8,r6
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    bt/s .Lmt7_packets_block_0790
    fmov.s fr4,@-r6
    tst #0x1,r0
    mov.l r6,@r6
    bt .Lmt7_packets_block_0789
    bra .Lmt7_packets_block_0773
.Lmt7_packets_block_0789:
    pref @r6
    add #0x8,r4
    mov.l @(0x4,r14),r14
    add #0x20,r6
    mov.l .Lmt7_packets_block_0794,r0
    add r4,r14
    mov.l @r13,r13
    add #0x40,r5
    fmov.s @r0+,fr12
    mov r6,r1
    fmov.s @r0+,fr14
    add #0x20,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    bra .Lmt7_packets_block_0775
    pref @r1
    .word 0x0009
.Lmt7_packets_block_0790:
    mov.l @r4+,r0
    mov.l r1,@r6
    cmp/pl r0
    pref @r6
    add #0x20,r6
    bf .Lmt7_packets_block_0792
    tst #0x80,r0
    mov.l .Lmt7_packets_block_0796,r13
    bt .Lmt7_packets_block_0792
    mov.l @r4+,r3
    mov r6,r1
    mov.l @r4,r0
    mov r4,r14
    mov.l @r13,r13
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0794,r0
    add #0x40,r5
    bt .Lmt7_packets_block_0791
    bra .Lmt7_packets_block_0774
.Lmt7_packets_block_0791:
    fmov.s @r0+,fr12
    add #0x8,r4
    mov.l @(0x4,r14),r14
    add #0x50,r6
    fmov.s @r0+,fr14
    add r4,r14
    pref @r1
    fmov.s @r14+,fr4
    add #0x20,r4
    fmov.s @r14+,fr6
    bra .Lmt7_packets_block_0775
    fschg
.Lmt7_packets_block_0792:
    add #-0x4,r4
    fschg
    add #0x40,r5
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0793:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace
.Lmt7_packets_block_0794:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x280
.Lmt7_packets_block_0795:
    .long __Q37shenmue2dc3sdk.g_lightRecords
.Lmt7_packets_block_0796:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x290
.Lmt7_packets_block_0797:
    mov.l @r4+,r3
    fldi1 fr2
    fschg
    fldi1 fr3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0798
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0798:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0799:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0804
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0800
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0800:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0802
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0801
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0801:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0799
    add #0x38,r6
    .word 0x0009
.Lmt7_packets_block_0802:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0803
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0803
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0801
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0801
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0803:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0806
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0804:
    cmp/pl r0
    mov.l @r4+,r2
    bf .Lmt7_packets_block_0805
    tst #0x80,r0
    mov r14,r3
    bt .Lmt7_packets_block_0805
    mov r2,r0
    mov r6,r2
    shar r2
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0800
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0800
    add #-0x8,r14
    .word 0x0009
.Lmt7_packets_block_0805:
    add #0x2c,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0806:
    fmul fr7,fr6
    mov r6,r2
    shar r2
    fmul fr7,fr5
    fmov.s fr0,@-r6
    add #-0x8,r4
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0807:
    cmp/pz r8
    bt .Lmt7_packets_block_0808
    bra .Lmt7_packets_block_0833
    nop
.Lmt7_packets_block_0808:
    mov.l @r4+,r9
    mov #0x3,r3
    add #0x30,r6
.Lmt7_packets_block_0809:
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0832,r13
    bf/s .Lmt7_packets_block_0810
    add #0x20,r4
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0810:
    mov.l .Lmt7_packets_block_0830,r0
    fschg
    mov.l @r13,r13
    fmov.s @r0+,fr12
    fmov.s @r0+,fr14
    fmov.s @r14+,fr4
    add #0x20,r4
    fmov.s @r14+,fr6
    bra .Lmt7_packets_block_0812
    fschg
.Lmt7_packets_block_0811:
    mov.l .Lmt7_packets_block_0830,r0
    add #0x20,r6
    mov.l @r13,r13
    mov r6,r1
    fmov.s @r0+,fr12
    add #0x40,r5
    fmov.s @r0+,fr14
    add #0x40,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    pref @r1
.Lmt7_packets_block_0812:
    fmov fr7,fr8
    tst r13,r13
    fmov.s @r14+,fr9
    bt/s .Lmt7_packets_block_0816
    fmov.s @r14+,fr10
    shlr r13
    mov.l .Lmt7_packets_block_0831,r2
    bf .Lmt7_packets_block_0815
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0813:
    fldi0 fr3
    cmp/eq #0x2,r0
    mov #0x74,r1
    bf/s .Lmt7_packets_block_0817
    add r2,r1
    fldi1 fr7
    tst r13,r13
    add #0xc,r1
    bf/s .Lmt7_packets_block_0814
    fmov.s @r1+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0814:
    fmov.s @r1+,fr1
    mov r2,r0
    fmov.s @r1+,fr2
    add #0x18,r0
    fipr fv0,fv8
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    fmov.s @r0+,fr3
    fldi0 fr0
    fcmp/gt fr11,fr0
    fmov fr11,fr0
    bf/s .Lmt7_packets_block_0821
    tst r13,r13
.Lmt7_packets_block_0815:
    mov #-0x50,r0
    bt .Lmt7_packets_block_0825
    shlr r13
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0823
    add r0,r2
    bra .Lmt7_packets_block_0813
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0816:
    fldi1 fr7
    bra .Lmt7_packets_block_0825
    ftrv xmtrx,fv4
    .word 0x0009
.Lmt7_packets_block_0817:
    fmov.s @r1+,fr0
    tst r13,r13
    fmov.s @r1+,fr1
    fsub fr4,fr0
    fmov.s @r1+,fr2
    fsub fr5,fr1
    fldi1 fr7
    add #0x18,r1
    fldi0 fr11
    bf/s .Lmt7_packets_block_0818
    fsub fr6,fr2
    ftrv xmtrx,fv4
.Lmt7_packets_block_0818:
    fipr fv0,fv8
    fldi0 fr12
    fipr fv0,fv0
    fcmp/gt fr11,fr12
    fmov.s @r1,fr12
    bt .Lmt7_packets_block_0822
    fcmp/gt fr3,fr12
    bf .Lmt7_packets_block_0822
    cmp/eq #0x3,r0
    flds fr3,FPUL
    bt/s .Lmt7_packets_block_0819
    fsrra fr3
    add #-0x18,r1
    fmov.s @r1+,fr12
    fmul fr0,fr12
    fmov.s @r1+,fr0
    fmac fr0,fr1,fr12
    fmov.s @r1,fr0
    add #-0x30,r1
    fmac fr0,fr2,fr12
    fmov.s @r1,fr2
    fneg fr12
    fmul fr3,fr12
    add #0x44,r1
    fcmp/gt fr12,fr2
    fsub fr2,fr12
    fmov.s @r1,fr2
    bt .Lmt7_packets_block_0822
    fmul fr2,fr12
    add #-0x4,r1
    fmul fr12,fr11
.Lmt7_packets_block_0819:
    add #-0x44,r1
    fmul fr3,fr11
    add #-0x8,r1
    fmul fr3,fr3
    fmov.s @r1,fr12
    fmul fr3,fr3
    fldi1 fr2
    mov r2,r0
    fmul fr3,fr3
    add #0x18,r0
    fmul fr3,fr12
    fmov fr11,fr0
    fcmp/gt fr2,fr12
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    bt/s .Lmt7_packets_block_0820
    fmov.s @r0+,fr3
    fmul fr12,fr0
.Lmt7_packets_block_0820:
    tst r13,r13
.Lmt7_packets_block_0821:
    fmac fr0,fr1,fr13
    mov #-0x50,r0
    fmac fr0,fr2,fr14
    bt/s .Lmt7_packets_block_0825
    fmac fr0,fr3,fr15
    bra .Lmt7_packets_block_0824
    shlr r13
.Lmt7_packets_block_0822:
    tst r13,r13
    mov #-0x50,r0
    bt/s .Lmt7_packets_block_0825
.Lmt7_packets_block_0823:
    shlr r13
.Lmt7_packets_block_0824:
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0823
    add r0,r2
    bra .Lmt7_packets_block_0813
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0825:
    mov.l .Lmt7_packets_block_0829,r2
    fschg
    fldi1 fr7
    fdiv fr4,fr7
    pref @r4
    add #-0x20,r4
    fmov.s @r2+,fr8
    mov r6,r1
    shar r1
    fmov.s @r2+,fr10
    fmul fr13,fr9
    mov.l @r4,r0
    fmul fr14,fr10
    add #0x10,r6
    fmul fr15,fr11
    fmov.s @r14+,fr2
    fldi0 fr12
    fldi1 fr0
    fsub fr0,fr13
    mov.l .Lmt7_packets_block_0829,r2
    fsub fr0,fr14
    add #0x10,r2
    fsub fr0,fr15
    fmov.s @r2+,fr0
    fmul fr1,fr13
    fmov.s @r2+,fr0
    fmul fr0,fr14
    fmul fr1,fr15
    fmov.s fr14,@-r6
    fmov.s fr12,@-r6
    mov.l .Lmt7_packets_block_0832,r13
    dt r3
    fmov.s fr10,@-r6
    mov r4,r14
    fmov.s fr8,@-r6
    add #-0x8,r6
    fmul fr7,fr6
    fmov.s fr2,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    bt/s .Lmt7_packets_block_0827
    fmov.s fr4,@-r6
    tst #0x1,r0
    mov.l r6,@r6
    bt .Lmt7_packets_block_0826
    bra .Lmt7_packets_block_0811
.Lmt7_packets_block_0826:
    pref @r6
    add #0x8,r4
    mov.l @(0x4,r14),r14
    add #0x20,r6
    mov.l .Lmt7_packets_block_0830,r0
    add r4,r14
    mov.l @r13,r13
    add #0x40,r5
    fmov.s @r0+,fr12
    mov r6,r1
    fmov.s @r0+,fr14
    add #0x20,r4
    fmov.s @r14+,fr4
    add #0x50,r6
    fmov.s @r14+,fr6
    fschg
    bra .Lmt7_packets_block_0812
    pref @r1
    .word 0x0009
.Lmt7_packets_block_0827:
    mov.l r1,@r6
    dt r9
    pref @r6
    add #0x20,r6
    fschg
    add #0x40,r5
    pref @r6
    add #-0x20,r6
    bt/s .Lmt7_packets_block_0828
    add #0x70,r6
    bra .Lmt7_packets_block_0809
    mov #0x3,r3
.Lmt7_packets_block_0828:
    rts
    add #-0x30,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0829:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace
.Lmt7_packets_block_0830:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x280
.Lmt7_packets_block_0831:
    .long __Q37shenmue2dc3sdk.g_lightRecords
.Lmt7_packets_block_0832:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x290
.Lmt7_packets_block_0833:
    mov.l @r4+,r9
    fldi1 fr2
    fschg
    fldi1 fr3
    mov #0x3,r3
    fmov.s fr2,@-r6
    add #0x20,r6
    fmov.s fr2,@r6
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf .Lmt7_packets_block_0834
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0834:
    fmov.s @r14+,fr4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmov.s @r14,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0835:
    fldi1 fr7
    fdiv fr4,fr7
    dt r3
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0840
    mov.l @r4+,r14
    tst #0x1,r0
    mov r6,r2
    bt/s .Lmt7_packets_block_0836
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0836:
    fmov.s @r14+,fr8
    add #0x20,r4
    pref @r4
    fmov.s @r14,fr10
    add #0x10,r14
    fldi1 fr11
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    ftrv xmtrx,fv8
    fmov.s fr4,@-r6
    add #-0x20,r4
    mov.l r2,@r6
    fmov.s @r14,fr0
    dt r3
    pref @r6
    add #0x38,r6
    mov r6,r2
    fldi1 fr11
    fdiv fr8,fr11
    mov.l @r4+,r0
    bt/s .Lmt7_packets_block_0838
    mov.l @r4+,r14
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0837
    add r4,r14
    mov r4,r14
    add #0x18,r4
    add #-0x8,r14
.Lmt7_packets_block_0837:
    add #0x20,r4
    fmov.s @r14+,fr4
    pref @r4
    fmov.s @r14,fr6
    add #0x10,r14
    fldi1 fr7
    fmul fr11,fr10
    add #-0x20,r4
    fmul fr11,fr9
    fmov.s fr0,@-r6
    fmov.s fr10,@-r6
    ftrv xmtrx,fv4
    fmov.s fr8,@-r6
    add #0x40,r5
    mov.l r2,@r6
    fmov.s @r14,fr0
    pref @r6
    bra .Lmt7_packets_block_0835
    add #0x38,r6
.Lmt7_packets_block_0838:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0839
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0837
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0837
    add #-0x8,r14
.Lmt7_packets_block_0839:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
    fmov fr8,fr4
    add #0x20,r5
    bra .Lmt7_packets_block_0842
    fmov fr10,fr6
    .word 0x0009
.Lmt7_packets_block_0840:
    dt r9
    mov r6,r2
    bt/s .Lmt7_packets_block_0841
    shar r2
    tst #0x1,r0
    mov #0x3,r3
    bt/s .Lmt7_packets_block_0836
    add r4,r14
    mov r4,r14
    add #0x18,r4
    bra .Lmt7_packets_block_0836
    add #-0x8,r14
.Lmt7_packets_block_0841:
    add #0x30,r4
    pref @r4
    add #-0x30,r4
.Lmt7_packets_block_0842:
    fmul fr7,fr6
    fmov.s fr0,@-r6
    fmul fr7,fr5
    fmov.s fr6,@-r6
    add #0x20,r5
    fmov.s fr4,@-r6
    add #-0x8,r4
    mov.l r2,@r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0843:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0844:
    mov #0x0,r11
    bsr .Lmt7_packets_block_0869
    mov #0x0,r9
    bt/s .Lmt7_packets_block_0846
    rotcl r9
    bsr .Lmt7_packets_block_0886
    add #0x1,r11
    bsr .Lmt7_packets_block_0869
    nop
    bt/s .Lmt7_packets_block_0852
    rotcl r9
    bsr .Lmt7_packets_block_0886
    add #0x1,r11
.Lmt7_packets_block_0845:
    bsr .Lmt7_packets_block_0869
    nop
    bt/s .Lmt7_packets_block_0862
    rotcl r9
    bsr .Lmt7_packets_block_0886
    add #0x1,r11
    bf .Lmt7_packets_block_0845
    bra .Lmt7_packets_block_0850
    nop
.Lmt7_packets_block_0846:
    dt r3
    add #0x1,r11
    bsr .Lmt7_packets_block_0869
    dt r3
    bf/s .Lmt7_packets_block_0856
    rotcl r9
    mov.l @r4,r0
    mov r4,r14
    tst #0x1,r0
    add #0x20,r4
    bf/s .Lmt7_packets_block_0847
    fldi1 fr7
    add #-0x18,r4
    mov.l @(0x4,r14),r14
    add r4,r14
.Lmt7_packets_block_0847:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr5
.Lmt7_packets_block_0848:
    fmov.s @r14+,fr6
    mov #0x20,r1
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    ftrv xmtrx,fv4
    pref @r4
    add #-0x20,r4
    bf/s .Lmt7_packets_block_0849
    mov r4,r14
    mov #0x8,r1
    mov.l @(0x4,r14),r14
    add #0x8,r14
    add r4,r14
.Lmt7_packets_block_0849:
    flds fr7,FPUL
    sts FPUL,r0
    cmp/hi r0,r12
    add #0x1,r11
    bf/s .Lmt7_packets_block_0860
    rotcl r9
    dt r3
    fldi1 fr7
    bt .Lmt7_packets_block_0850
    add r1,r4
    fmov.s @r14+,fr4
    bra .Lmt7_packets_block_0848
    fmov.s @r14+,fr5
.Lmt7_packets_block_0850:
    mov.l @r4+,r0
    cmp/pl r0
    bf/s .Lmt7_packets_block_0851
    tst #0x80,r0
    bf/s .Lmt7_packets_block_0844
    mov.l @r4+,r3
    add #-0x4,r4
.Lmt7_packets_block_0851:
    lds.l @r15+,PR
    mov #0x1,r0
    mov.l @r15+,r11
    add #-0x4,r4
    mov.l @r15+,r10
    rts
    or r0,r12
.Lmt7_packets_block_0852:
    bsr .Lmt7_packets_block_0890
    dt r3
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0853
    add #-0x18,r4
.Lmt7_packets_block_0853:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0854
    add #-0x18,r4
.Lmt7_packets_block_0854:
    bsr .Lmt7_packets_block_0869
    nop
    bsr .Lmt7_packets_block_0890
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0896
    mov #0x0,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0855
    add #0x18,r4
.Lmt7_packets_block_0855:
    bra .Lmt7_packets_block_0867
    nop
.Lmt7_packets_block_0856:
    bsr .Lmt7_packets_block_0890
    mov #0x1,r11
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0857
    add #-0x18,r4
.Lmt7_packets_block_0857:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0858
    add #-0x18,r4
.Lmt7_packets_block_0858:
    bsr .Lmt7_packets_block_0869
    nop
    bsr .Lmt7_packets_block_0890
    mov #0x0,r11
    mov #0x1,r11
    clrt
    bsr .Lmt7_packets_block_0896
    mov #0x0,r0
    clrt
    bsr .Lmt7_packets_block_0894
    mov #0x1,r0
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0859
    add #0x18,r4
.Lmt7_packets_block_0859:
    bra .Lmt7_packets_block_0867
    nop
.Lmt7_packets_block_0860:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0861
    add #-0x18,r4
.Lmt7_packets_block_0861:
    bsr .Lmt7_packets_block_0869
    nop
.Lmt7_packets_block_0862:
    bsr .Lmt7_packets_block_0890
    nop
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0863
    add #-0x18,r4
.Lmt7_packets_block_0863:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0864
    add #-0x18,r4
.Lmt7_packets_block_0864:
    add #-0x8,r4
    mov.l @(0x4,r4),r0
    tst #0x1,r0
    bt .Lmt7_packets_block_0865
    add #-0x18,r4
.Lmt7_packets_block_0865:
    bsr .Lmt7_packets_block_0869
    nop
    bsr .Lmt7_packets_block_0890
    add #-0x2,r11
    bsr .Lmt7_packets_block_0869
    nop
    bsr .Lmt7_packets_block_0890
    add #0x1,r11
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0866
    add #0x18,r4
.Lmt7_packets_block_0866:
    mov r12,r0
    add #0x1,r11
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0868
    mov r9,r0
    bsr .Lmt7_packets_block_0893
    nop
    bra .Lmt7_packets_block_0868
    mov r9,r0
.Lmt7_packets_block_0867:
    bsr .Lmt7_packets_block_0869
    add #0x1,r11
    bsr .Lmt7_packets_block_0890
    rotcl r9
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0868
    mov r9,r0
    bsr .Lmt7_packets_block_0893
    nop
    mov r9,r0
.Lmt7_packets_block_0868:
    and #0x7,r0
    shll2 r0
    braf r0
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa00c
    .word 0x0009
    .word 0xa019
    .word 0x0009
    .word 0xa024
    .word 0x0009
    .word 0xa02a
    .word 0x0009
    .word 0xa038
    .word 0x0009
    .word 0xa03d
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb193
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb166
    .word 0x70ff
    .word 0x60b3
    .word 0x4310
    .word 0xb18b
    .word 0x70ff
    .word 0x8bd3
    .word 0xaf5d
    .word 0x0009
    .word 0x0008
    .word 0xb15c
    .word 0x60b3
    .word 0x60b3
    .word 0x0008
    .word 0xb181
    .word 0x70ff
    .word 0x4310
    .word 0xb155
    .word 0x60b3
    .word 0x8bc6
    .word 0xaf50
    .word 0x0009
    .word 0x60b3
    .word 0x0018
    .word 0xb177
    .word 0x70fe
    .word 0x4310
    .word 0x890f
    .word 0xaf22
    .word 0x0009
    .word 0x60b3
    .word 0x0008
    .word 0xb16f
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb142
    .word 0x70ff
    .word 0x4310
    .word 0xb13f
    .word 0x60b3
    .word 0x8901
    .word 0xaf05
    .word 0x7b01
    .word 0xaf38
    .word 0x0009
    .word 0x60b3
    .word 0x4310
    .word 0xb15f
    .word 0x70ff
    .word 0x8ba7
    .word 0xaf31
    .word 0x0009
    .word 0x60b3
    .word 0xc801
    .word 0x8903
    .word 0x60b3
    .word 0x0008
    .word 0xb155
    .word 0x70ff
    .word 0x60b3
    .word 0x0008
    .word 0xb151
    .word 0x70fe
    .word 0x60b3
    .word 0x0008
    .word 0xb14d
    .word 0x70ff
    .word 0x4310
    .word 0xb121
    .word 0x60b3
    .word 0x8b92
    .word 0xaf1c
    .word 0x0009
.Lmt7_packets_block_0869:
    mov.l @r4,r0
    fschg
    mov.l .Lmt7_packets_block_0904,r13
    tst #0x1,r0
    mov.l .Lmt7_packets_block_0903,r0
    mov r4,r14
    bf/s .Lmt7_packets_block_0870
    add #0x20,r4
    add #-0x18,r4
    mov.l @(0x4,r14),r14
    add r4,r14
.Lmt7_packets_block_0870:
    mov.l @r13,r13
    fmov.s @r0+,fr12
    fmov.s @r0+,fr14
    fmov.s @r14+,fr4
    fmov.s @r14+,fr6
    fschg
    cmp/pz r8
    fmov fr7,fr8
    bf/s .Lmt7_packets_block_0874
    tst r13,r13
    fmov.s @r14+,fr9
    bt/s .Lmt7_packets_block_0875
    fmov.s @r14+,fr10
    shlr r13
    mov.l .Lmt7_packets_block_0906,r2
    bf .Lmt7_packets_block_0873
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0871:
    fldi0 fr3
    cmp/eq #0x2,r0
    mov #0x74,r1
    bf/s .Lmt7_packets_block_0876
    add r2,r1
    fldi1 fr7
    tst r13,r13
    add #0xc,r1
    bf/s .Lmt7_packets_block_0872
    fmov.s @r1+,fr0
    ftrv xmtrx,fv4
.Lmt7_packets_block_0872:
    fmov.s @r1+,fr1
    mov r2,r0
    fmov.s @r1+,fr2
    add #0x18,r0
    fipr fv0,fv8
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    fmov.s @r0+,fr3
    fldi0 fr0
    fcmp/gt fr11,fr0
    fmov fr11,fr0
    bf/s .Lmt7_packets_block_0880
    tst r13,r13
.Lmt7_packets_block_0873:
    mov #-0x50,r0
    bt .Lmt7_packets_block_0885
    shlr r13
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0882
    add r0,r2
    bra .Lmt7_packets_block_0871
    mov.w @(0x2,r2),r0
.Lmt7_packets_block_0874:
    fldi1 fr7
    add #0x8,r14
    bra .Lmt7_packets_block_0884
    ftrv xmtrx,fv4
.Lmt7_packets_block_0875:
    fldi1 fr7
    bra .Lmt7_packets_block_0885
    ftrv xmtrx,fv4
    .word 0x0009
.Lmt7_packets_block_0876:
    fmov.s @r1+,fr0
    tst r13,r13
    fmov.s @r1+,fr1
    fsub fr4,fr0
    fmov.s @r1+,fr2
    fsub fr5,fr1
    fldi1 fr7
    add #0x18,r1
    fldi0 fr11
    bf/s .Lmt7_packets_block_0877
    fsub fr6,fr2
    ftrv xmtrx,fv4
.Lmt7_packets_block_0877:
    fipr fv0,fv8
    fldi0 fr12
    fipr fv0,fv0
    fcmp/gt fr11,fr12
    fmov.s @r1,fr12
    bt .Lmt7_packets_block_0881
    fcmp/gt fr3,fr12
    bf .Lmt7_packets_block_0881
    cmp/eq #0x3,r0
    flds fr3,FPUL
    bt/s .Lmt7_packets_block_0878
    fsrra fr3
    add #-0x18,r1
    fmov.s @r1+,fr12
    fmul fr0,fr12
    fmov.s @r1+,fr0
    fmac fr0,fr1,fr12
    fmov.s @r1,fr0
    add #-0x30,r1
    fmac fr0,fr2,fr12
    fmov.s @r1,fr2
    fneg fr12
    fmul fr3,fr12
    add #0x44,r1
    fcmp/gt fr12,fr2
    fsub fr2,fr12
    fmov.s @r1,fr2
    bt .Lmt7_packets_block_0881
    fmul fr2,fr12
    add #-0x4,r1
    fmul fr12,fr11
.Lmt7_packets_block_0878:
    add #-0x44,r1
    fmul fr3,fr11
    add #-0x8,r1
    fmul fr3,fr3
    fmov.s @r1,fr12
    fmul fr3,fr3
    fldi1 fr2
    mov r2,r0
    fmul fr3,fr3
    add #0x18,r0
    fmul fr3,fr12
    fmov fr11,fr0
    fcmp/gt fr2,fr12
    fmov.s @r0+,fr1
    add #0x4,r1
    fmov.s @r0+,fr2
    bt/s .Lmt7_packets_block_0879
    fmov.s @r0+,fr3
    fmul fr12,fr0
.Lmt7_packets_block_0879:
    tst r13,r13
.Lmt7_packets_block_0880:
    fmac fr0,fr1,fr13
    mov #-0x50,r0
    fmac fr0,fr2,fr14
    bt/s .Lmt7_packets_block_0885
    fmac fr0,fr3,fr15
    bra .Lmt7_packets_block_0883
    shlr r13
.Lmt7_packets_block_0881:
    tst r13,r13
    mov #-0x50,r0
    bt/s .Lmt7_packets_block_0885
.Lmt7_packets_block_0882:
    shlr r13
.Lmt7_packets_block_0883:
    extu.b r0,r0
    bf/s .Lmt7_packets_block_0882
    add r0,r2
    bra .Lmt7_packets_block_0871
    mov.w @(0x2,r2),r0
    .word 0x0009
.Lmt7_packets_block_0884:
    fldi1 fr13
    fldi1 fr15
    fldi1 fr14
.Lmt7_packets_block_0885:
    flds fr7,FPUL
    add #0x20,r4
    pref @r4
    fschg
    fldi1 fr7
    fdiv fr4,fr7
    mov.l .Lmt7_packets_block_0902,r2
    add #-0x20,r4
    sts FPUL,r0
    fmov.s @r2+,fr8
    fmov.s @r2+,fr10
    cmp/hi r0,r12
    fmul fr13,fr9
    mov.l @r14+,r1
    fmul fr14,fr10
    mov.l @r14+,r2
    fmul fr15,fr11
    fsts FPUL,fr4
    rts
    fschg
.Lmt7_packets_block_0886:
    dt r3
    lds r6,FPUL
    bf .Lmt7_packets_block_0887
    mov r6,r0
    shar r0
    sett
    lds r0,FPUL
.Lmt7_packets_block_0887:
    fschg
    add #0x18,r6
    fsts FPUL,fr4
    fmul fr7,fr6
    mov.l r2,@-r6
    fmul fr7,fr5
    mov.l r1,@-r6
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    pref @r6
    add #0x40,r6
    cmp/pz r8
    fldi0 fr12
    bf .Lmt7_packets_block_0888
    fldi1 fr0
    fsub fr0,fr13
    fsub fr0,fr14
    fsub fr0,fr15
    mov.l .Lmt7_packets_block_0902,r0
    add #0x10,r0
    fmov.s @r0+,fr0
    fmul fr1,fr13
    fmov.s @r0+,fr0
    fmul fr0,fr14
    fmul fr1,fr15
    fmov.s fr14,@-r6
    bra .Lmt7_packets_block_0889
    fmov.s fr12,@-r6
.Lmt7_packets_block_0888:
    fldi0 fr13
    fmov.s fr12,@-r6
    fmov.s fr12,@-r6
.Lmt7_packets_block_0889:
    tst r3,r3
    fmov.s fr10,@-r6
    add #0x40,r5
    fmov.s fr8,@-r6
    fschg
    pref @r6
    rts
    add #0x20,r6
.Lmt7_packets_block_0890:
    mov #0x6,r10
    mov r11,r0
    and #0x3,r0
    fldi0 fr0
    shld r10,r0
    fldi0 fr1
    fschg
    mov.l .Lmt7_packets_block_0901,r10
    add r0,r10
    cmp/pz r8
    fldi0 fr12
    bf .Lmt7_packets_block_0891
    fldi1 fr0
    fsub fr0,fr13
    fsub fr0,fr14
    fsub fr0,fr15
    mov.l .Lmt7_packets_block_0902,r0
    add #0x10,r0
    fmov.s @r0+,fr0
    fmul fr1,fr13
    fmov.s @r0+,fr0
    fmul fr0,fr14
    fmul fr1,fr15
    fmov.s fr14,@-r10
    bra .Lmt7_packets_block_0892
    fmov.s fr12,@-r10
.Lmt7_packets_block_0891:
    fldi0 fr13
    fmov.s fr12,@-r10
    fmov.s fr12,@-r10
.Lmt7_packets_block_0892:
    fmov.s fr10,@-r10
    fmov.s fr8,@-r10
    add #-0x8,r10
    mov.l r2,@-r10
    mov.l r1,@-r10
    fmov.s fr6,@-r10
    fmov.s fr4,@-r10
    rts
    fschg
.Lmt7_packets_block_0893:
    mov #0x6,r1
    mov r11,r0
    and #0x3,r0
    mov.l .Lmt7_packets_block_0900,r10
    shld r1,r0
    fschg
    mov r10,r1
    add r0,r1
    fmov.s @r1+,fr12
    add #-0x40,r0
    fmov.s @r1+,fr14
    and #0xff,r0
    fmov.s @r1+,fr0
    fmov.s @r1+,fr2
    add #0x20,r10
    mov r10,r1
    add r0,r1
    fmov.s fr2,@-r1
    fmov.s fr0,@-r1
    fmov.s fr14,@-r1
    add #-0x40,r0
    fmov.s fr12,@-r1
    and #0xff,r0
    mov r10,r1
    add r0,r1
    fmov.s fr2,@-r1
    fmov.s fr0,@-r1
    fmov.s fr14,@-r1
    fmov.s fr12,@-r1
    rts
    fschg
.Lmt7_packets_block_0894:
    mov #0x6,r1
    mov.l .Lmt7_packets_block_0899,r10
    and #0x3,r0
    fschg
    shld r1,r0
    fldi0 fr6
    add r0,r10
    fldi0 fr7
    mov #-0x10,r2
    fmov.s @r10+,fr0
    add #0x40,r6
    fmov.s @r10+,fr2
    fmov.s @r10+,fr4
    fmov.s @r10+,fr6
    add #-0x40,r10
    fmov.s fr6,@-r6
    fmov.s fr4,@-r6
    fmov.s fr2,@-r6
    fmov.s fr0,@-r6
    fmov.s @r10+,fr0
    shll16 r2
    fmov.s @r10+,fr2
    fmov.s @r10+,fr4
    fmul fr3,fr2
    fmov.s @r10+,fr6
    fmul fr3,fr1
    shll8 r2
    fmov.s fr6,@-r6
    add #0x40,r5
    fmov.s fr4,@-r6
    fmov.s fr2,@-r6
    bt/s .Lmt7_packets_block_0895
    fmov.s fr0,@-r6
    mov r6,r2
.Lmt7_packets_block_0895:
    mov.l r2,@r6
    pref @r6
    add #0x20,r6
    pref @r6
    fschg
    rts
    add #0x20,r6
.Lmt7_packets_block_0896:
    mov #0x6,r1
    mov.l .Lmt7_packets_block_0898,r10
    and #0x3,r0
    mov r10,r2
    shld r1,r0
    lds r12,FPUL
    add r0,r10
    fsts FPUL,fr0
    mov r11,r0
    fmov.s @r10,fr2
    and #0x3,r0
    shld r1,r0
    fsub fr2,fr0
    add r0,r2
    fmov.s @r2,fr1
    fschg
    add #0x10,r2
    fsub fr2,fr1
    add #0x10,r10
    lds r7,FPUL
    fdiv fr1,fr0
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    fsub fr2,fr4
    add #-0x8,r10
    fsub fr3,fr5
    fsts FPUL,fr1
    fmac fr0,fr4,fr2
    add #0x18,r6
    fmac fr0,fr5,fr3
    fmov.s fr2,@-r6
    fschg
    fmov.s fr1,@-r6
    fmov.s @r2,fr5
    add #-0x4,r2
    fmov.s @r10,fr3
    add #-0x4,r10
    fmov.s @r2,fr4
    fmov.s @r10,fr2
    fsub fr3,fr5
    fsub fr2,fr4
    fmac fr0,fr5,fr3
    add #0x34,r10
    fmac fr0,fr4,fr2
    mov r6,r0
    fmul fr1,fr3
    add #0x40,r5
    fmul fr1,fr2
    fmov.s fr3,@-r6
    bf/s .Lmt7_packets_block_0897
    fmov.s fr2,@-r6
    shar r0
    sett
.Lmt7_packets_block_0897:
    mov.l r0,@-r6
    add #0x34,r2
    pref @r6
    add #0x40,r6
    fschg
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    fmov.s @r2,fr6
    fmac fr0,fr4,fr2
    add #-0x8,r2
    fmac fr0,fr5,fr3
    fmov.s @r10,fr4
    fmov.s fr2,@-r6
    fsub fr4,fr6
    add #-0x8,r10
    fsub fr5,fr7
    fmac fr0,fr6,fr4
    fmac fr0,fr7,fr5
    fmov.s fr4,@-r6
    fmov.s @r2,fr4
    add #-0x8,r2
    fmov.s @r10,fr2
    add #-0x8,r10
    fsub fr2,fr4
    fsub fr3,fr5
    fmov.s @r2,fr6
    fmac fr0,fr4,fr2
    fmac fr0,fr5,fr3
    fmov.s @r10,fr4
    fmov.s fr2,@-r6
    fsub fr4,fr6
    fsub fr5,fr7
    fmac fr0,fr6,fr4
    fmac fr0,fr7,fr5
    fmov.s fr4,@-r6
    fschg
    pref @r6
    rts
    add #0x20,r6
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0898:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x80
.Lmt7_packets_block_0899:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0xa0
.Lmt7_packets_block_0900:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0xa0
.Lmt7_packets_block_0901:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0xc0
.Lmt7_packets_block_0902:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace
.Lmt7_packets_block_0903:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x280
.Lmt7_packets_block_0904:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x290
.Lmt7_packets_block_0905:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x280
.Lmt7_packets_block_0906:
    .long __Q37shenmue2dc3sdk.g_lightRecords
.Lmt7_packets_block_0907:
    .long __Q37shenmue2dc3sdk.g_mt7PackedLightWorkspace+0x290
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0908:
    mov.l r10,@-r15
    mov.l r11,@-r15
    sts.l PR,@-r15
    mov.l @r4+,r3
.Lmt7_packets_block_0909:
    bsr .Lmt7_packets_block_0917
    mov #0x0,r9
    mov r9,r0
    cmp/eq #0x0,r0
    bt .Lmt7_packets_block_0912
    cmp/eq #0x7,r0
    bt .Lmt7_packets_block_0913
    bsr .Lmt7_packets_block_0869
    mov #0x0,r11
    bsr .Lmt7_packets_block_0890
    nop
    bsr .Lmt7_packets_block_0869
    mov #0x1,r11
    bsr .Lmt7_packets_block_0890
    nop
    bsr .Lmt7_packets_block_0869
    mov #0x2,r11
    bsr .Lmt7_packets_block_0890
    nop
    mov r12,r0
    tst #0x1,r0
    bt/s .Lmt7_packets_block_0910
    shll2 r9
    bsr .Lmt7_packets_block_0893
    nop
.Lmt7_packets_block_0910:
    braf r9
    nop
    .word 0x0009
    .word 0x0009
    .word 0xa034
    .word 0x0009
    .word 0xa041
    .word 0x0009
    .word 0xa04f
    .word 0x0009
    .word 0xa05a
    .word 0x0009
    .word 0xa068
    .word 0x0009
    .word 0xa073
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0911:
    dt r3
    bf .Lmt7_packets_block_0909
    mov #0x1,r0
    lds.l @r15+,PR
    or r0,r12
    mov.l @r15+,r11
    rts
    mov.l @r15+,r10
.Lmt7_packets_block_0912:
    mov r3,r9
    bsr .Lmt7_packets_block_0869
    mov #0x3,r3
    bsr .Lmt7_packets_block_0886
    nop
    bsr .Lmt7_packets_block_0869
    nop
    bsr .Lmt7_packets_block_0886
    nop
    bsr .Lmt7_packets_block_0869
    nop
    bsr .Lmt7_packets_block_0886
    nop
    bra .Lmt7_packets_block_0911
    mov r9,r3
.Lmt7_packets_block_0913:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0914
    add #0x18,r4
.Lmt7_packets_block_0914:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0915
    add #0x18,r4
.Lmt7_packets_block_0915:
    mov.l @r4,r0
    tst #0x1,r0
    add #0x8,r4
    bt .Lmt7_packets_block_0916
    add #0x18,r4
.Lmt7_packets_block_0916:
    bra .Lmt7_packets_block_0911
    nop
    .word 0x0008
    .word 0xbeee
    .word 0xe000
    .word 0x0008
    .word 0xbeeb
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbf10
    .word 0xe000
    .word 0x0018
    .word 0xbf0d
    .word 0xe001
    .word 0xafc9
    .word 0x0009
    .word 0x0008
    .word 0xbedf
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbf04
    .word 0xe000
    .word 0x0008
    .word 0xeb02
    .word 0xbed7
    .word 0xe002
    .word 0x0018
    .word 0xbefd
    .word 0xe001
    .word 0xafb9
    .word 0x0009
    .word 0x0008
    .word 0xbecf
    .word 0xe000
    .word 0x0008
    .word 0xeb01
    .word 0xbef4
    .word 0xe000
    .word 0x0018
    .word 0xeb02
    .word 0xbef0
    .word 0xe000
    .word 0xafac
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbeea
    .word 0xe000
    .word 0x0008
    .word 0xbebe
    .word 0xe001
    .word 0x0008
    .word 0xeb02
    .word 0xbee3
    .word 0xe000
    .word 0x0018
    .word 0xbeb7
    .word 0xe002
    .word 0xaf9c
    .word 0x0009
    .word 0x0008
    .word 0xeb01
    .word 0xbeda
    .word 0xe000
    .word 0x0008
    .word 0xbeae
    .word 0xe001
    .word 0x0018
    .word 0xeb02
    .word 0xbed3
    .word 0xe001
    .word 0xaf8f
    .word 0x0009
    .word 0x0008
    .word 0xeb02
    .word 0xbecd
    .word 0xe000
    .word 0x0008
    .word 0xbeca
    .word 0xe001
    .word 0x0018
    .word 0xbe9e
    .word 0xe002
    .word 0xaf83
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0917:
    mov r4,r1
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0918
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0919
    fldi1 fr7
.Lmt7_packets_block_0918:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
    add #0x18,r4
.Lmt7_packets_block_0919:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv4
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0920
    fmov.s @r4+,fr0
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr0
    fmov.s @r0+,fr1
    fmov.s @r0+,fr2
    bra .Lmt7_packets_block_0921
    fldi1 fr3
.Lmt7_packets_block_0920:
    fmov.s @r4+,fr1
    fmov.s @r4,fr2
    fldi1 fr3
    add #0x18,r4
.Lmt7_packets_block_0921:
    pref @r4
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    ftrv xmtrx,fv0
    flds fr7,FPUL
    mov.l @r4,r0
    tst #0x1,r0
    bf/s .Lmt7_packets_block_0922
    fmov.s @r4+,fr4
    mov.l @r4+,r0
    add r4,r0
    fmov.s @r0+,fr4
    fmov.s @r0+,fr5
    fmov.s @r0+,fr6
    bra .Lmt7_packets_block_0923
    fldi1 fr7
.Lmt7_packets_block_0922:
    fmov.s @r4+,fr5
    fmov.s @r4,fr6
    fldi1 fr7
.Lmt7_packets_block_0923:
    mov r1,r4
    sts FPUL,r0
    ftrv xmtrx,fv4
    cmp/hi r0,r12
    flds fr3,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    flds fr7,FPUL
    rotcl r9
    sts FPUL,r0
    cmp/hi r0,r12
    rts
    rotcl r9
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    .word 0x0000
    add #0x18,r4
    mov.l .Lmt7_packets_block_0942,r1
    fldi1 fr1
    fmov.s fr15,@-r15
    fmov.s @r1,fr0
    fmov.s fr14,@-r15
    fmov.s fr13,@-r15
    fdiv fr0,fr1
    fmov.s fr12,@-r15
    mov.l r14,@-r15
    mov.l r13,@-r15
    mov.l r12,@-r15
    mov.l r11,@-r15
    mov.l r10,@-r15
    mov.l r9,@-r15
    mov.l r8,@-r15
    mov r7,r13
    sts.l PR,@-r15
    add #-0x20,r15
    fldi1 fr7
    mov #0x0,r0
    fabs fr0
    fmov fr0,@(r0,r15)
    mov.l .Lmt7_packets_block_0943,r0
    fmov.s fr7,@r1
    mov.l @r0,r1
    mov.l .Lmt7_packets_block_0945,r2
    mov.l r1,@(0x4,r15)
    mov.l @r2,r1
    mov.l .Lmt7_packets_block_0946,r0
    mov.l .Lmt7_packets_block_0944,r2
    mov.l r1,@(0xc,r15)
    mov.l @r2,r3
    mov.l @r0,r1
    mov.l r3,@(0x8,r15)
    mov.l @r5+,r12
    mov.l r1,@(0x10,r15)
    mov.l r12,@(0x14,r15)
    mov.l @r5+,r12
    mov.l @r5+,r1
    mov.l r12,@(0x18,r15)
    mov.l r1,@(0x1c,r15)
    mov.l @r5,r12
    add #0x14,r5
    mov.l .Lmt7_packets_block_0941,r10
    mov.l @r5,r7
    mov.l @r10,r10
    mov #0x4,r2
    shll8 r2
    mov.l @(0x4,r10),r5
    shll16 r2
    add #-0x1,r2
    and r5,r2
    mov r6,r8
    mov #-0x20,r6
    shll8 r6
    shll16 r6
    or r2,r6
    add #0x10,r4
    nop
.Lmt7_packets_block_0924:
    fldi1 fr7
    mov r4,r0
    add #0x30,r0
    pref @r0
    mov r15,r3
    fmov.s @r4+,fr4
    fmov.s @r4+,fr5
    fmov.s @r4+,fr6
    ftrv xmtrx,fv4
    fmov.s @r3+,fr1
    fmov.s @r4+,fr0
    fmul fr1,fr0
    lds r12,FPUL
    mov #-0x2,r0
    fadd fr0,fr7
    fsts FPUL,fr1
    fcmp/gt fr1,fr7
    fsub fr0,fr7
    and r0,r12
    bf/s .Lmt7_packets_block_0926
    fsub fr0,fr7
    fcmp/gt fr1,fr7
    bf/s .Lmt7_packets_block_0926
    add #0x1,r12
.Lmt7_packets_block_0925:
    add #0x2c,r4
    mov.l @r4+,r0
    add r0,r4
    mov.l @r4,r0
    tst r0,r0
    add #0x10,r4
    bf .Lmt7_packets_block_0924
    bra .Lmt7_packets_block_0939
    nop
    .word 0x0009
.Lmt7_packets_block_0926:
    fmov.s @r3+,fr1
    mov.l .Lmt7_packets_block_0948,r0
    fmov.s @r0,fr7
    fmul fr4,fr7
    fneg fr0
    fmul fr0,fr1
    fsub fr7,fr5
    fmov.s @r3+,fr3
    mov.l .Lmt7_packets_block_0947,r0
    fcmp/gt fr1,fr5
    fmov.s @r0,fr7
    fmul fr0,fr3
    bf/s .Lmt7_packets_block_0925
    fmul fr4,fr7
    fsub fr7,fr6
    fmov.s @r3+,fr0
    fcmp/gt fr3,fr6
    fmov.s @r3+,fr7
    fmul fr4,fr0
    bf/s .Lmt7_packets_block_0925
    fsub fr1,fr0
    fcmp/gt fr0,fr5
    fmul fr4,fr7
    bt/s .Lmt7_packets_block_0925
    fsub fr3,fr7
    fcmp/gt fr7,fr6
    bt .Lmt7_packets_block_0925
    add #0x30,r4
    mov.l @r4+,r0
    mov r0,r1
    tst r13,r13
    bt .Lmt7_packets_block_0929
    mov r8,r0
    mov.l .Lmt7_packets_block_0933,r2
    cmp/eq #-0x1,r0
    mov.l @r2,r2
    mov r1,r0
    mov.l @(0x14,r15),r3
    and #0x3,r0
    bt/s .Lmt7_packets_block_0927
    cmp/pz r2
    bra .Lmt7_packets_block_0928
    mov #0x0,r0
.Lmt7_packets_block_0927:
    bt .Lmt7_packets_block_0928
    xor #0x1,r0
.Lmt7_packets_block_0928:
    mov #0x1b,r2
    shld r2,r0
    mov.l r3,@r6
    add #0x20,r5
    mov.l r0,@(0x4,r6)
    pref @r6
    add #0x20,r6
    mov.l r0,@(0x18,r15)
    add #0x20,r4
    pref @r4
    add #-0x20,r4
    mov r6,r11
    mov #-0x1,r2
    add #-0x1,r13
.Lmt7_packets_block_0929:
    rotcr r12
    mov r1,r0
    add r12,r12
    bf/s .Lmt7_packets_block_0931
    tst #0x8,r0
    bf .Lmt7_packets_block_0930
    bra .Lmt7_packets_block_0973
    and #0x3,r0
.Lmt7_packets_block_0930:
    bra .Lmt7_packets_block_1021
    and #0x3,r0
.Lmt7_packets_block_0931:
    bf .Lmt7_packets_block_0932
    bra .Lmt7_packets_block_0949
    and #0x3,r0
.Lmt7_packets_block_0932:
    bra .Lmt7_packets_block_0962
    and #0x3,r0
.Lmt7_packets_block_0933:
    .long _shenmue_dc_mt7_draw_controls
.Lmt7_packets_block_0934:
    tst r13,r13
    bt .Lmt7_packets_block_0936
.Lmt7_packets_block_0935:
    pref @r6
    add #0x20,r6
    mov.l @(0x14,r15),r0
    add #0x40,r5
    mov.l @(0x18,r15),r1
    mov.l @(0x1c,r15),r2
    pref @r6
    add #-0x60,r6
    mov.l r0,@r6
    or #0x40,r0
    mov.l r1,@(0x4,r6)
    or r1,r2
    pref @r6
    add #0x20,r6
    mov.l r0,@r6
    mov.l r2,@(0x4,r6)
    pref @r6
    add #0x60,r6
.Lmt7_packets_block_0936:
    mov.l @r4,r0
    tst r0,r0
    add #0x10,r4
    bt .Lmt7_packets_block_0939
    bra .Lmt7_packets_block_0924
    nop
.Lmt7_packets_block_0937:
    tst r13,r13
    bt/s .Lmt7_packets_block_0938
    mov.l @r4,r0
    cmp/eq r6,r11
    bf .Lmt7_packets_block_0935
    add #-0x20,r6
    add #-0x20,r5
.Lmt7_packets_block_0938:
    tst r0,r0
    add #0x10,r4
    bt .Lmt7_packets_block_0939
    bra .Lmt7_packets_block_0924
    nop
.Lmt7_packets_block_0939:
    tst r13,r13
    add #0x1,r13
    bf .Lmt7_packets_block_0940
    cmp/eq r6,r11
    add #-0x10,r4
    bf .Lmt7_packets_block_0935
    add #-0x20,r5
.Lmt7_packets_block_0940:
    mov.l r5,@(0x4,r10)
    add #0x20,r15
    lds.l @r15+,PR
    mov.l @r15+,r8
    mov.l @r15+,r9
    mov.l @r15+,r10
    mov.l @r15+,r11
    mov.l @r15+,r12
    mov.l @r15+,r13
    mov.l @r15+,r14
    fmov.s @r15+,fr12
    fmov.s @r15+,fr13
    fmov.s @r15+,fr14
    fmov.s @r15+,fr15
    rts
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0941:
    .long __nj_vertex_buffer_
.Lmt7_packets_block_0942:
    .long __Q37shenmue2dc3sdk.g_modelScale
.Lmt7_packets_block_0943:
    .long __Q37shenmue2dc3sdk.g_mt7HorizontalClipScale
.Lmt7_packets_block_0944:
    .long __Q37shenmue2dc3sdk.g_mt7VerticalClipScale
.Lmt7_packets_block_0945:
    .long __Q27shenmue12matrix_stack.g_projectionViewportWidth
.Lmt7_packets_block_0946:
    .long __Q27shenmue12matrix_stack.g_projectionViewportHeight
.Lmt7_packets_block_0947:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginY
.Lmt7_packets_block_0948:
    .long __Q27shenmue12matrix_stack.g_projectionViewportOriginX
.Lmt7_packets_block_0949:
    mov #0x1,r9
    mov.l @r4+,r3
    fschg
    lds r9,FPUL
.Lmt7_packets_block_0950:
    add #-0x2,r3
    mov r0,r9
    add #0x1,r9
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0951
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0951:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr6
    float FPUL,fr7
    ftrv xmtrx,fv4
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0952
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0952:
    fldi1 fr7
    fdiv fr4,fr7
    pref @r14
    mov r9,r0
    tst #0x1,r0
    mov #0x28,r0
    bf .Lmt7_packets_block_0953
    mov #0x10,r0
.Lmt7_packets_block_0953:
    add r6,r0
    fmul fr7,fr6
    fmul fr7,fr5
    fmov.s fr6,@-r0
    fmov.s fr4,@-r0
    fmov.s @r14+,fr8
    fmov.s @r14+,fr10
    float FPUL,fr11
    ftrv xmtrx,fv8
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0954
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0954:
    fldi1 fr11
    fdiv fr8,fr11
    pref @r14
    mov r9,r0
    tst #0x1,r0
    mov #0x10,r0
    bf .Lmt7_packets_block_0955
    mov #0x28,r0
.Lmt7_packets_block_0955:
    add r6,r0
    fmul fr11,fr10
    fmul fr11,fr9
    fmov.s fr10,@-r0
    fmov.s fr8,@-r0
    mov r6,r0
    shar r0
    mov.l r0,@r6
.Lmt7_packets_block_0956:
    add #0x1,r9
    fmov.s @r14+,fr12
    dt r3
    fmov.s @r14+,fr14
    float FPUL,fr15
    mov r4,r14
    bt/s .Lmt7_packets_block_0957
    ftrv xmtrx,fv12
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0957
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0957:
    fldi1 fr15
    fdiv fr12,fr15
    add #0x20,r6
    pref @r6
    add #-0x20,r6
    pref @r14
    fschg
    mov #0x1c,r0
    fmul fr15,fr14
    add r6,r0
    fmul fr15,fr13
    fmov.s fr15,@-r0
    fmov.s fr14,@-r0
    fmov.s fr13,@-r0
    pref @r0
    tst r3,r3
    mov.l @r4,r0
    bt .Lmt7_packets_block_0959
    mov r9,r0
    tst #0x1,r0
    mov #0x28,r0
    bt .Lmt7_packets_block_0958
    mov #0x10,r0
.Lmt7_packets_block_0958:
    add r6,r0
    fmov.s fr15,@-r0
    add #0x40,r6
    fmov.s fr14,@-r0
    add #0x40,r5
    fmov.s fr13,@-r0
    bra .Lmt7_packets_block_0956
    fschg
.Lmt7_packets_block_0959:
    cmp/pl r0
    add #0x40,r6
    bt .Lmt7_packets_block_0960
    bra .Lmt7_packets_block_0934
.Lmt7_packets_block_0960:
    add #0x40,r5
    tst #0x80,r0
    add #0x4,r4
    bt/s .Lmt7_packets_block_0961
    mov.l @r4+,r3
    bra .Lmt7_packets_block_0950
    fschg
.Lmt7_packets_block_0961:
    tst #0x10,r0
    fschg
    bf .Lmt7_packets_block_0950
    bra .Lmt7_packets_block_0963
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0962:
    mov #0x1,r9
    mov.l @r4+,r3
    fschg
    lds r9,FPUL
.Lmt7_packets_block_0963:
    mov r0,r9
    add #0x1,r9
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0964
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0964:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr6
    float FPUL,fr7
    ftrv xmtrx,fv4
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0965
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0965:
    fldi1 fr7
    fdiv fr4,fr7
    pref @r14
    mov r9,r0
    tst #0x1,r0
    mov #0x28,r0
    bf .Lmt7_packets_block_0966
    mov #0x10,r0
.Lmt7_packets_block_0966:
    add r6,r0
    fmul fr7,fr6
    fmul fr7,fr5
    fmov.s fr6,@-r0
    fmov.s fr4,@-r0
    fmov.s @r14+,fr8
    fmov.s @r14+,fr10
    float FPUL,fr11
    ftrv xmtrx,fv8
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0967
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0967:
    fldi1 fr11
    fdiv fr8,fr11
    pref @r14
    mov r9,r0
    tst #0x1,r0
    mov #0x10,r0
    bf .Lmt7_packets_block_0968
    mov #0x28,r0
.Lmt7_packets_block_0968:
    add r6,r0
    fmul fr11,fr10
    fmul fr11,fr9
    fmov.s fr10,@-r0
    fmov.s fr8,@-r0
    mov r6,r0
    shar r0
    mov.l r0,@r6
    add #0x1,r9
    fmov.s @r14+,fr12
    dt r3
    fmov.s @r14+,fr14
    float FPUL,fr15
    mov r4,r14
    bt/s .Lmt7_packets_block_0969
    ftrv xmtrx,fv12
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0969
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0969:
    fldi1 fr15
    fdiv fr12,fr15
    add #0x20,r6
    pref @r6
    add #-0x20,r6
    pref @r14
    fschg
    mov #0x1c,r0
    fmul fr15,fr14
    add r6,r0
    fmul fr15,fr13
    fmov.s fr15,@-r0
    fmov.s fr14,@-r0
    fmov.s fr13,@-r0
    pref @r0
    tst r3,r3
    mov.l @r4,r0
    bt .Lmt7_packets_block_0970
    add #0x40,r6
    add #0x40,r5
    add #0x1,r9
    bra .Lmt7_packets_block_0964
    fschg
.Lmt7_packets_block_0970:
    cmp/pl r0
    add #0x40,r6
    bt .Lmt7_packets_block_0971
    bra .Lmt7_packets_block_0934
.Lmt7_packets_block_0971:
    add #0x40,r5
    tst #0x80,r0
    add #0x4,r4
    bt/s .Lmt7_packets_block_0972
    mov.l @r4+,r3
    bra .Lmt7_packets_block_0963
    fschg
.Lmt7_packets_block_0972:
    tst #0x10,r0
    fschg
    bt .Lmt7_packets_block_0963
    bra .Lmt7_packets_block_0950
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_0973:
    lds r12,FPUL
    mov #0x1,r9
    mov.l @r4+,r3
    fschg
    fsts FPUL,fr1
    lds r9,FPUL
.Lmt7_packets_block_0974:
    add #-0x2,r3
    mov r0,r9
    add #0x1,r9
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0975
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0975:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr6
    float FPUL,fr7
    ftrv xmtrx,fv4
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0976
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0976:
    pref @r14
    flds fr7,FPUL
    mov #0x1,r0
    fldi1 fr7
    fdiv fr4,fr7
    fsts FPUL,fr4
    lds r0,FPUL
    fmov.s @r14+,fr8
    fmov.s @r14+,fr10
    float FPUL,fr11
    ftrv xmtrx,fv8
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_0977
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_0977:
    pref @r14
    flds fr11,FPUL
    mov #0x1,r0
    fldi1 fr11
    fdiv fr8,fr11
    fsts FPUL,fr8
    lds r0,FPUL
    fcmp/gt fr4,fr1
    bf .Lmt7_packets_block_0983
    mov r9,r0
    tst #0x1,r0
    mov #0x28,r0
    bf .Lmt7_packets_block_0978
    mov #0x10,r0
.Lmt7_packets_block_0978:
    fmov fr6,fr14
    add r6,r0
    fmul fr15,fr14
    fmov fr4,fr12
    fmul fr15,fr13
    fmov.s fr14,@-r0
    fmov.s fr12,@-r0
    fcmp/gt fr8,fr1
    bf .Lmt7_packets_block_0980
    mov r9,r0
    tst #0x1,r0
    mov #0x10,r0
    bf .Lmt7_packets_block_0979
    mov #0x28,r0
.Lmt7_packets_block_0979:
    fmov fr10,fr14
    add r6,r0
    fmul fr15,fr14
    fmov fr8,fr12
    fmul fr15,fr13
    fmov.s fr14,@-r0
    fmov.s fr12,@-r0
    mov r6,r0
    shar r0
    mov.l r0,@r6
    mov #-0x1,r0
    cmp/eq r0,r8
    bt .Lmt7_packets_block_0990
    cmp/eq r0,r2
    bf .Lmt7_packets_block_0990
    bt .Lmt7_packets_block_0996
.Lmt7_packets_block_0980:
    fschg
    add #0x1,r9
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    add #-0x1,r9
    mov r6,r0
    shar r0
    fschg
    mov.l r0,@r6
    mov #-0x1,r0
    cmp/eq r0,r8
    bf .Lmt7_packets_block_0981
    bra .Lmt7_packets_block_1000
.Lmt7_packets_block_0981:
    nop
    cmp/eq r0,r2
    bt .Lmt7_packets_block_0982
    bra .Lmt7_packets_block_1000
.Lmt7_packets_block_0982:
    nop
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    bra .Lmt7_packets_block_1000
    lds r0,FPUL
.Lmt7_packets_block_0983:
    fcmp/gt fr8,fr1
    bf .Lmt7_packets_block_0987
    mov r9,r0
    tst #0x1,r0
    mov #0x10,r0
    bf .Lmt7_packets_block_0984
    mov #0x28,r0
.Lmt7_packets_block_0984:
    fmov fr10,fr14
    add r6,r0
    fmul fr15,fr14
    fmov fr8,fr12
    fmul fr15,fr13
    fmov.s fr14,@-r0
    fmov.s fr12,@-r0
    fschg
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    mov r6,r0
    shar r0
    mov.l r0,@r6
    mov #-0x1,r0
    fschg
    cmp/eq r0,r8
    bf .Lmt7_packets_block_0985
    bra .Lmt7_packets_block_1006
.Lmt7_packets_block_0985:
    nop
    cmp/eq r2,r0
    bt .Lmt7_packets_block_0986
    bra .Lmt7_packets_block_1006
.Lmt7_packets_block_0986:
    nop
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    bra .Lmt7_packets_block_1006
    lds r0,FPUL
.Lmt7_packets_block_0987:
    mov r8,r0
    cmp/eq #-0x1,r0
    mov r6,r0
    shar r0
    bf .Lmt7_packets_block_0988
    bra .Lmt7_packets_block_1012
.Lmt7_packets_block_0988:
    mov.l r0,@r6
    mov r2,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_0989
    bra .Lmt7_packets_block_1012
.Lmt7_packets_block_0989:
    nop
    bra .Lmt7_packets_block_1015
    nop
.Lmt7_packets_block_0990:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_0992
    bsr .Lmt7_packets_block_1064
    add #0x20,r6
    tst r3,r3
    bf .Lmt7_packets_block_0991
    bra .Lmt7_packets_block_1017
.Lmt7_packets_block_0991:
    nop
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_0990
    fmov fr14,fr10
.Lmt7_packets_block_0992:
    bsr .Lmt7_packets_block_1068
    fschg
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_0993
    bsr .Lmt7_packets_block_1070
    mov r9,r0
.Lmt7_packets_block_0993:
    tst r3,r3
    bf .Lmt7_packets_block_0994
    bra .Lmt7_packets_block_1017
.Lmt7_packets_block_0994:
    nop
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_0995
    bsr .Lmt7_packets_block_1072
    mov r9,r0
.Lmt7_packets_block_0995:
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_1000
    fmov fr14,fr10
.Lmt7_packets_block_0996:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_0998
    bsr .Lmt7_packets_block_1064
    add #0x20,r6
    tst r3,r3
    bf .Lmt7_packets_block_0997
    bra .Lmt7_packets_block_1017
.Lmt7_packets_block_0997:
    nop
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_0996
    fmov fr14,fr10
.Lmt7_packets_block_0998:
    bsr .Lmt7_packets_block_1068
    fschg
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    tst r3,r3
    lds r0,FPUL
    bf .Lmt7_packets_block_0999
    bra .Lmt7_packets_block_1017
.Lmt7_packets_block_0999:
    nop
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_1000
    fmov fr14,fr10
.Lmt7_packets_block_1000:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_1004
    bsr .Lmt7_packets_block_1064
    add #0x20,r6
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1001
    mov r9,r0
    bsr .Lmt7_packets_block_1070
    add #0x1,r0
.Lmt7_packets_block_1001:
    tst r3,r3
    bf .Lmt7_packets_block_1002
    bra .Lmt7_packets_block_1017
.Lmt7_packets_block_1002:
    nop
    mov r9,r0
    bsr .Lmt7_packets_block_1065
    add #0x1,r0
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1003
    bsr .Lmt7_packets_block_1075
    mov r9,r0
.Lmt7_packets_block_1003:
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_1006
    fmov fr14,fr10
.Lmt7_packets_block_1004:
    bsr .Lmt7_packets_block_1068
    fschg
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1005
    mov r9,r0
    bsr .Lmt7_packets_block_1070
    add #0x1,r0
.Lmt7_packets_block_1005:
    tst r3,r3
    bt .Lmt7_packets_block_1017
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_1012
    fmov fr14,fr10
.Lmt7_packets_block_1006:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_1009
    bsr .Lmt7_packets_block_1068
    fschg
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1007
    bsr .Lmt7_packets_block_1070
    mov r9,r0
.Lmt7_packets_block_1007:
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1008
    bsr .Lmt7_packets_block_1072
    mov r9,r0
.Lmt7_packets_block_1008:
    bsr .Lmt7_packets_block_1074
    add #0x20,r6
    tst r3,r3
    bt .Lmt7_packets_block_1017
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_0990
    fmov fr14,fr10
.Lmt7_packets_block_1009:
    fmov fr14,fr6
    fmov fr12,fr4
    fschg
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1010
    bsr .Lmt7_packets_block_1070
    mov r9,r0
.Lmt7_packets_block_1010:
    tst r3,r3
    bt .Lmt7_packets_block_1017
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1011
    bsr .Lmt7_packets_block_1072
    mov r9,r0
.Lmt7_packets_block_1011:
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_1000
    fmov fr14,fr10
.Lmt7_packets_block_1012:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bt .Lmt7_packets_block_1013
    fmov fr10,fr6
    fmov fr8,fr4
    fmov fr14,fr10
    fmov fr12,fr8
    tst r3,r3
    bf .Lmt7_packets_block_1012
    bra .Lmt7_packets_block_1017
    fschg
.Lmt7_packets_block_1013:
    bsr .Lmt7_packets_block_1077
    fschg
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    bsr .Lmt7_packets_block_1074
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1014
    bsr .Lmt7_packets_block_1079
    add #0x20,r6
.Lmt7_packets_block_1014:
    tst r3,r3
    bt .Lmt7_packets_block_1017
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_1006
    fmov fr14,fr10
.Lmt7_packets_block_1015:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bt .Lmt7_packets_block_1016
    fmov fr10,fr6
    fmov fr8,fr4
    fmov fr14,fr10
    fmov fr12,fr8
    tst r3,r3
    bf .Lmt7_packets_block_1015
    bra .Lmt7_packets_block_1017
    fschg
.Lmt7_packets_block_1016:
    bsr .Lmt7_packets_block_1077
    fschg
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    lds r0,FPUL
    bsr .Lmt7_packets_block_1074
    add #0x20,r6
    tst r3,r3
    bt .Lmt7_packets_block_1017
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1067
    fschg
    bra .Lmt7_packets_block_1006
    fmov fr14,fr10
.Lmt7_packets_block_1017:
    mov.l @r4,r0
    cmp/pl r0
    bt .Lmt7_packets_block_1018
    bra .Lmt7_packets_block_0937
.Lmt7_packets_block_1018:
    tst #0x80,r0
    add #0x4,r4
    bt/s .Lmt7_packets_block_1019
    mov.l @r4+,r3
    bra .Lmt7_packets_block_0974
    fschg
.Lmt7_packets_block_1019:
    tst #0x10,r0
    fschg
    bt .Lmt7_packets_block_1020
    bra .Lmt7_packets_block_0974
.Lmt7_packets_block_1020:
    nop
    bra .Lmt7_packets_block_1022
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_1021:
    lds r12,FPUL
    mov #0x1,r9
    mov.l @r4+,r3
    fschg
    fsts FPUL,fr1
    lds r9,FPUL
.Lmt7_packets_block_1022:
    mov r0,r9
    add #0x1,r9
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_1023
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_1023:
    fmov.s @r14+,fr4
    fmov.s @r14+,fr6
    float FPUL,fr7
    ftrv xmtrx,fv4
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_1024
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_1024:
    pref @r14
    flds fr7,FPUL
    mov #0x1,r0
    fldi1 fr7
    fdiv fr4,fr7
    fsts FPUL,fr4
    lds r0,FPUL
    fmov.s @r14+,fr8
    fmov.s @r14+,fr10
    float FPUL,fr11
    ftrv xmtrx,fv8
    mov r4,r14
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_1025
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_1025:
    pref @r14
    flds fr11,FPUL
    mov #0x1,r0
    fldi1 fr11
    fdiv fr8,fr11
    fsts FPUL,fr8
    lds r0,FPUL
    fcmp/gt fr4,fr1
    bf .Lmt7_packets_block_1029
    mov r9,r0
    tst #0x1,r0
    mov #0x28,r0
    bf .Lmt7_packets_block_1026
    mov #0x10,r0
.Lmt7_packets_block_1026:
    fmov fr6,fr14
    add r6,r0
    fmul fr15,fr14
    fmov fr4,fr12
    fmul fr15,fr13
    fmov.s fr14,@-r0
    fmov.s fr12,@-r0
    fcmp/gt fr8,fr1
    bf .Lmt7_packets_block_1028
    mov r9,r0
    tst #0x1,r0
    mov #0x10,r0
    bf .Lmt7_packets_block_1027
    mov #0x28,r0
.Lmt7_packets_block_1027:
    fmov fr10,fr14
    add r6,r0
    fmul fr15,fr14
    fmov fr8,fr12
    fmul fr15,fr13
    fmov.s fr14,@-r0
    fmov.s fr12,@-r0
    mov r6,r0
    shar r0
    mov.l r0,@r6
    mov #-0x1,r0
    cmp/eq r0,r8
    bt .Lmt7_packets_block_1034
    cmp/eq r0,r2
    bf .Lmt7_packets_block_1034
    bt .Lmt7_packets_block_1039
.Lmt7_packets_block_1028:
    fschg
    add #0x1,r9
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    add #-0x1,r9
    mov r6,r0
    shar r0
    fschg
    mov.l r0,@r6
    mov #-0x1,r0
    cmp/eq r0,r8
    bt .Lmt7_packets_block_1042
    cmp/eq r0,r2
    bf .Lmt7_packets_block_1042
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    bra .Lmt7_packets_block_1042
    lds r0,FPUL
.Lmt7_packets_block_1029:
    fcmp/gt fr8,fr1
    bf .Lmt7_packets_block_1031
    mov r9,r0
    tst #0x1,r0
    mov #0x10,r0
    bf .Lmt7_packets_block_1030
    mov #0x28,r0
.Lmt7_packets_block_1030:
    fmov fr10,fr14
    add r6,r0
    fmul fr15,fr14
    fmov fr8,fr12
    fmul fr15,fr13
    fmov.s fr14,@-r0
    fmov.s fr12,@-r0
    fschg
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    mov r6,r0
    shar r0
    mov.l r0,@r6
    mov #-0x1,r0
    fschg
    cmp/eq r8,r0
    bt .Lmt7_packets_block_1046
    cmp/eq r2,r0
    bf .Lmt7_packets_block_1046
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    bra .Lmt7_packets_block_1046
    lds r0,FPUL
.Lmt7_packets_block_1031:
    mov r8,r0
    cmp/eq #-0x1,r0
    bf .Lmt7_packets_block_1032
    bra .Lmt7_packets_block_1051
.Lmt7_packets_block_1032:
    nop
    mov r2,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1033
    bra .Lmt7_packets_block_1051
.Lmt7_packets_block_1033:
    nop
    bra .Lmt7_packets_block_1054
    nop
.Lmt7_packets_block_1034:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_1036
    bsr .Lmt7_packets_block_1064
    add #0x20,r6
    tst r3,r3
    add #0x1,r9
    bf .Lmt7_packets_block_1035
    bra .Lmt7_packets_block_1056
.Lmt7_packets_block_1035:
    nop
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1036:
    bsr .Lmt7_packets_block_1068
    fschg
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1037
    bsr .Lmt7_packets_block_1070
    mov r9,r0
.Lmt7_packets_block_1037:
    tst r3,r3
    add #0x1,r9
    bf .Lmt7_packets_block_1038
    bra .Lmt7_packets_block_1056
.Lmt7_packets_block_1038:
    nop
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1039:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_1041
    bsr .Lmt7_packets_block_1064
    add #0x20,r6
    tst r3,r3
    add #0x1,r9
    bf .Lmt7_packets_block_1040
    bra .Lmt7_packets_block_1056
.Lmt7_packets_block_1040:
    nop
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1041:
    bsr .Lmt7_packets_block_1068
    fschg
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    lds r0,FPUL
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1042:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_1044
    bsr .Lmt7_packets_block_1064
    add #0x20,r6
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1043
    mov r9,r0
    bsr .Lmt7_packets_block_1070
    add #0x1,r0
.Lmt7_packets_block_1043:
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1044:
    bsr .Lmt7_packets_block_1068
    fschg
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1045
    mov r9,r0
    bsr .Lmt7_packets_block_1070
    add #0x1,r0
.Lmt7_packets_block_1045:
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1046:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bf .Lmt7_packets_block_1049
    bsr .Lmt7_packets_block_1068
    fschg
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1047
    bsr .Lmt7_packets_block_1070
    mov r9,r0
.Lmt7_packets_block_1047:
    bsr .Lmt7_packets_block_1065
    mov r9,r0
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1048
    bsr .Lmt7_packets_block_1072
    mov r9,r0
.Lmt7_packets_block_1048:
    bsr .Lmt7_packets_block_1074
    add #0x20,r6
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1049:
    fmov fr14,fr6
    fmov fr12,fr4
    fschg
    bsr .Lmt7_packets_block_1069
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1050
    bsr .Lmt7_packets_block_1070
    mov r9,r0
.Lmt7_packets_block_1050:
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1051:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bt .Lmt7_packets_block_1052
    fschg
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1052:
    bsr .Lmt7_packets_block_1077
    fschg
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    bsr .Lmt7_packets_block_1074
    add #0x20,r6
    mov r8,r0
    cmp/eq #-0x1,r0
    bt .Lmt7_packets_block_1053
    bsr .Lmt7_packets_block_1079
    add #0x20,r6
.Lmt7_packets_block_1053:
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1054:
    bsr .Lmt7_packets_block_1060
    add #0x1,r9
    bt .Lmt7_packets_block_1055
    fschg
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1055:
    bsr .Lmt7_packets_block_1077
    fschg
    bsr .Lmt7_packets_block_1062
    fmov fr5,fr13
    flds fr14,FPUL
    sts FPUL,r8
    flds fr13,FPUL
    mov #0x1,r0
    sts FPUL,r2
    lds r0,FPUL
    bsr .Lmt7_packets_block_1074
    add #0x20,r6
    tst r3,r3
    add #0x1,r9
    bt .Lmt7_packets_block_1056
    bra .Lmt7_packets_block_1023
    fschg
.Lmt7_packets_block_1056:
    mov.l @r4,r0
    cmp/pl r0
    bt .Lmt7_packets_block_1057
    bra .Lmt7_packets_block_0937
.Lmt7_packets_block_1057:
    tst #0x80,r0
    add #0x4,r4
    bt/s .Lmt7_packets_block_1058
    mov.l @r4+,r3
    bra .Lmt7_packets_block_1022
    fschg
.Lmt7_packets_block_1058:
    tst #0x10,r0
    fschg
    bf .Lmt7_packets_block_1059
    bra .Lmt7_packets_block_1022
.Lmt7_packets_block_1059:
    nop
    bra .Lmt7_packets_block_0974
    nop
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
.Lmt7_packets_block_1060:
    fmov.s @r14+,fr12
    dt r3
    fmov.s @r14+,fr14
    float FPUL,fr15
    mov r4,r14
    bt/s .Lmt7_packets_block_1061
    ftrv xmtrx,fv12
    mov.l @r4,r0
    add #0x20,r4
    tst #0x1,r0
    bf .Lmt7_packets_block_1061
    mov.l @(0x4,r14),r14
    add #-0x18,r4
    add r4,r14
.Lmt7_packets_block_1061:
    pref @r14
    fcmp/gt fr15,fr1
    flds fr15,FPUL
    mov #0x1,r0
    fldi1 fr15
    fdiv fr12,fr15
    fsts FPUL,fr12
    rts
    lds r0,FPUL
    .word 0x0009
.Lmt7_packets_block_1062:
    fmov fr4,fr12
    fsub fr8,fr12
    fmov fr1,fr0
    fsub fr8,fr0
    fmov fr6,fr3
    fsub fr10,fr3
    fmov fr5,fr2
    fsub fr9,fr2
    fmov fr10,fr14
    fdiv fr12,fr0
    fmov fr9,fr13
    lds r7,FPUL
    fsts FPUL,fr15
    mov #0x1,r0
    lds r0,FPUL
    mov r9,r0
    tst #0x1,r0
    mov #0xc,r0
    bt .Lmt7_packets_block_1063
    mov #0x24,r0
.Lmt7_packets_block_1063:
    fmac fr0,fr3,fr14
    fmac fr0,fr2,fr13
    fmul fr15,fr14
    fmul fr15,fr13
    fmov fr15,@(r0,r6)
    add #-0x4,r0
    fmov fr14,@(r0,r6)
    add #-0x4,r0
    rts
    fmov fr13,@(r0,r6)
.Lmt7_packets_block_1064:
    pref @r6
    mov r6,r0
    fmov fr14,fr6
    fmul fr15,fr14
    fmov fr12,fr4
    fmul fr15,fr13
    add #-0x4,r0
    fschg
    fmov.s fr15,@-r0
    add #0x20,r6
    fmov.s fr14,@-r0
    add #0x40,r5
    fmov.s fr13,@-r0
    rts
    pref @r0
    .word 0x0009
.Lmt7_packets_block_1065:
    tst #0x1,r0
    mov #0x24,r0
    bt .Lmt7_packets_block_1066
    mov #0xc,r0
.Lmt7_packets_block_1066:
    fmov fr15,@(r0,r6)
    add #-0x4,r0
    fmov fr14,@(r0,r6)
    add #-0x4,r0
    rts
    fmov fr13,@(r0,r6)
.Lmt7_packets_block_1067:
    fmov fr4,fr12
    fmov fr6,fr14
    fmov fr8,fr4
    fmov fr10,fr6
    rts
    fmov fr12,fr8
.Lmt7_packets_block_1068:
    add #0x20,r6
    pref @r6
    fsub fr12,fr4
    fmov fr1,fr0
    fsub fr12,fr0
    fmov fr6,fr3
    fsub fr14,fr3
    fmov fr5,fr2
    fsub fr13,fr2
    fdiv fr4,fr0
    fmov fr15,fr7
    fmov fr14,fr6
    fmov fr13,fr5
    fmov fr12,fr4
    lds r7,FPUL
    fsts FPUL,fr15
    mov #0x1,r0
    lds r0,FPUL
    fmac fr0,fr3,fr14
    fmac fr0,fr2,fr13
    mov r6,r0
    fmul fr15,fr14
    add #-0x4,r0
    fmul fr15,fr13
    fmov.s fr15,@-r0
    add #0x20,r6
    fmov.s fr14,@-r0
    add #0x40,r5
    fmov.s fr13,@-r0
    rts
    pref @r0
    .word 0x0009
.Lmt7_packets_block_1069:
    pref @r6
    fmov fr4,fr12
    fsub fr8,fr12
    fmov fr1,fr0
    fsub fr8,fr0
    fmov fr6,fr3
    fsub fr10,fr3
    fmov fr5,fr2
    fsub fr9,fr2
    fmov fr10,fr14
    fdiv fr12,fr0
    fmov fr9,fr13
    lds r7,FPUL
    fsts FPUL,fr15
    mov #0x1,r0
    lds r0,FPUL
    fmac fr0,fr3,fr14
    fmac fr0,fr2,fr13
    mov r6,r0
    fmul fr15,fr14
    add #-0x4,r0
    fmul fr15,fr13
    fmov.s fr15,@-r0
    add #0x20,r6
    fmov.s fr14,@-r0
    add #0x40,r5
    fmov.s fr13,@-r0
    rts
    pref @r0
    .word 0x0009
.Lmt7_packets_block_1070:
    tst #0x1,r0
    mov #0x10,r0
    bt .Lmt7_packets_block_1071
    mov #0x28,r0
.Lmt7_packets_block_1071:
    add r6,r0
    mov.l r7,@-r0
    add #0x20,r6
    mov.l r8,@-r0
    add #0x40,r5
    pref @r6
    add #0x20,r6
    mov.l r2,@-r0
    rts
    pref @r0
.Lmt7_packets_block_1072:
    tst #0x1,r0
    mov #0x10,r0
    bt .Lmt7_packets_block_1073
    mov #0x28,r0
.Lmt7_packets_block_1073:
    fmov fr11,fr15
    add r6,r0
    fmov fr10,fr14
    fmul fr15,fr14
    fmov fr9,fr13
    fmul fr15,fr13
    fmov.s fr15,@-r0
    fmov.s fr14,@-r0
    rts
    fmov.s fr13,@-r0
.Lmt7_packets_block_1074:
    pref @r6
    mov r6,r0
    fmov fr7,fr15
    add #-0x4,r0
    fmov fr6,fr14
    fmul fr15,fr14
    fmov fr5,fr13
    fmul fr15,fr13
    fmov.s fr15,@-r0
    add #0x20,r6
    fmov.s fr14,@-r0
    add #0x40,r5
    fmov.s fr13,@-r0
    rts
    pref @r0
    .word 0x0009
.Lmt7_packets_block_1075:
    tst #0x1,r0
    mov #0x28,r0
    bt .Lmt7_packets_block_1076
    mov #0x10,r0
.Lmt7_packets_block_1076:
    fmov fr7,fr15
    add r6,r0
    fmov fr6,fr14
    fmul fr15,fr14
    fmov fr5,fr13
    fmul fr15,fr13
    fmov.s fr15,@-r0
    fmov.s fr14,@-r0
    rts
    fmov.s fr13,@-r0
.Lmt7_packets_block_1077:
    fsub fr12,fr4
    fmov fr1,fr0
    fsub fr12,fr0
    fmov fr6,fr3
    fsub fr14,fr3
    fmov fr5,fr2
    fsub fr13,fr2
    fdiv fr4,fr0
    fmov fr15,fr7
    fmov fr14,fr6
    fmov fr13,fr5
    fmov fr12,fr4
    lds r7,FPUL
    fsts FPUL,fr15
    mov #0x1,r0
    lds r0,FPUL
    mov r9,r0
    tst #0x1,r0
    mov #0x28,r0
    bt .Lmt7_packets_block_1078
    mov #0x10,r0
.Lmt7_packets_block_1078:
    fmac fr0,fr3,fr14
    fmac fr0,fr2,fr13
    fmul fr15,fr14
    add r6,r0
    fmul fr15,fr13
    fmov.s fr15,@-r0
    fmov.s fr14,@-r0
    rts
    fmov.s fr13,@-r0
.Lmt7_packets_block_1079:
    pref @r6
    mov r6,r0
    add #-0x4,r0
    mov.l r7,@-r0
    add #0x20,r6
    mov.l r8,@-r0
    add #0x40,r5
    mov.l r2,@-r0
    rts
    pref @r0
    .word 0x0000
    .word 0x0000
    .word 0xfbfd
    .word 0xf149
    .word 0xf248
    .word 0x7408
    .word 0xf348
    .word 0xf049
    .word 0xf34d
    .word 0xf549
    .word 0xf648
    .word 0x7408
    .word 0xf748
    .word 0xf449
    .word 0xf74d
    .word 0xf949
    .word 0xfa48
    .word 0x7408
    .word 0xfb48
    .word 0xf849
    .word 0xfb4d
    .word 0xfd49
    .word 0xfe48
    .word 0x7408
    .word 0xff48
    .word 0xfc49
    .word 0xff4d
    .word 0x000b
    .word 0xfbfd
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009
    .word 0x0009

#endif
